<?php

class Usuario extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.usuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "usucpf" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'usucpf' => null,
									  	'regcod' => null,
									  	'usunome' => null,
									  	'usuemail' => null,
									  	'usustatus' => null,
									  	'usufoneddd' => null,
									  	'usufonenum' => null,
									  	'ususenha' => null,
									  	'usudataultacesso' => null,
									  	'usunivel' => null,
									  	'ususexo' => null,
									  	'orgcod' => null,
									  	'unicod' => null,
									  	'usuchaveativacao' => null,
									  	'usutentativas' => null,
									  	'usuprgproposto' => null,
									  	'usuacaproposto' => null,
									  	'usuobs' => null,
									  	'ungcod' => null,
									  	'usudatainc' => null,
									  	'usuconectado' => null,
									  	'pflcod' => null,
									  	'suscod' => null,
									  	'usunomeguerra' => null,
									  	'orgao' => null,
									  	'muncod' => null,
									  	'usudatanascimento' => null,
									  	'usudataatualizacao' => null,
									  	'entid' => null,
									  	'tpocod' => null,
									  	'carid' => null
									  );

    public function loginWebService($usuario, $senha){

        $usuario          = corrige_cpf( $usuario );
        $senha            = trim( $senha );
        $limiteTentativas = 4;
        $mensagem         = "Usuário autenticado com sucesso";
        $retorno          = true;

        if(empty($usuario) || empty($senha)){
            $mensagem = "Usuário ou senha inválidos";
            $retorno  = false;
        }

        if($retorno){
            $sql = sprintf("SELECT
                    			u.usucpf,
                    			u.ususenha,
                    			u.suscod,
                    			u.usutentativas,
                    			u.usunome,
                    			u.usuemail
    		               FROM
    			                seguranca.usuario u
                		   WHERE
                			    u.usucpf = '%s'", $usuario);

            $usuario = (object) $this->recuperar( $sql );

            if ( !$usuario->usucpf ) {
                $retorno  = false;
                $mensagem = "Usuário não existe";
            }else{
                // verifica o status do usuário
                switch ( $usuario->suscod ) {
                    case 'A': // Ativo
                        break;
                    case 'P': // Pendente
                        $retorno  = false;
                        $mensagem = "Usuário pendente";
                    case 'B': // Bloqueado
                    default:
                        $retorno  = false;
                        $mensagem = "Usuário bloqueado";
                }
            }
        }
        if($retorno){
            // valida a senha informada
            if (md5_decrypt_senha( $usuario->ususenha, '' ) != $senha ) {

                $usuario->usutentativas += 1;

                if ( $usuario->usutentativas > $limiteTentativas ) {
                    $sql = sprintf( "UPDATE seguranca.usuario SET usutentativas = 0 WHERE usucpf = '%s'", $usuario->usucpf );
                    $this->executar( $sql );

                    $mensagem = "Usuário bloqueado por exceder a quantidade de tentativas de login com senha inválida.";
                    $retorno  = false;

                    $this->alterar_status_usuario( $usuario->usucpf, 'B', $mensagem );
                } else {

                    $mensagem = "A senha informada não é válida. Tentativas restantes: " . ($limiteTentativas - $usuario->usutentativas + 1);
                    $retorno  = false;

                    $sql = sprintf("UPDATE seguranca.usuario SET usutentativas = %d WHERE usucpf = '%s'",
                                    $usuario->usutentativas,
                                    $usuario->usucpf);
                    $this->executar( $sql );
                }
            }else{
                if ( $usuario->usutentativas > 0 ) {
                    $sql = sprintf( "UPDATE seguranca.usuario SET usutentativas = 0 WHERE usucpf = '%s'", $usuario->usucpf );
                    $this->executar( $sql );
                }
            }
            $this->commit();
        }

        //se chegou até esse ponto adiciona informações do usuário
        $arRetorno = array("codigo" => ($retorno ? '0' : '1'), "mensagem" => $mensagem);
        if($retorno){
            $arRetorno['dados'] = array ('nome'         => $usuario->usunome,
                                         //'nomeResumido' => resumirNomeCompleto($usuario->nome),
                                         'nomeResumido' => $usuario->nome,
                                         'email'        => $usuario->usuemail);
        }

        return $arRetorno;
    }

    /**
     * Rertorna os dados do usuário
     *
     * @param $cpf cpf sem formatação
     * @return object
     */
    public function getDadosPorCpf($cpf){

        // verifica se o cpf já está cadastrado no sistema
        $sql = sprintf(
            "SELECT
			u.ususexo,
			u.usucpf,
			u.regcod,
			u.usunome,
			u.usuemail,
			u.usustatus,
			u.usufoneddd,
			u.usufonenum,
			u.ususenha,
			u.usudataultacesso,
			u.usunivel,
			u.usufuncao,
			u.ususexo,
			u.entid,
			u.unicod,
			u.usuchaveativacao,
			u.usutentativas,
			u.usuobs,
			u.ungcod,
			u.usudatainc,
			u.usuconectado,
			u.suscod,
			u.muncod,
			u.carid
		FROM
			seguranca.usuario u
		WHERE
			u.usucpf = '%s'",
            $cpf
        );

        $usuario = (object) $this->pegaLinha( $sql );

        return $usuario;
    }

    public function isCpfCadastrado($usuario){
        if ( $usuario->usucpf ) {
            $cpf_cadastrado = true;
        } else {
            $cpf_cadastrado = false;
        }

        return $cpf_cadastrado;
    }

    public function cadastrarDadosGerais($dados){
        // Gerando a senha que poderá ser usada no SSD e no simec
        $senhageral = $this->gerar_senha();

        $cpf        = $dados['usucpf'];

        $usunome    = $dados['usunome'];
        $usuemail   = $dados['usuemail'];
        $usufoneddd = $dados['usufoneddd'];
        $usufonenum = $dados['usufonenum'];

        $regcod     = $dados['regcod'];
        $ususexo    = $dados['ususexo'];
        $muncod     = $dados['muncod'];
        $usufuncao  = $dados['usufuncao'];
        $orgao      = $dados['orgao'];

        // insere informações gerais do usuário
        $sql = sprintf(
            "INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, usuchaveativacao, regcod,
					ususexo, ususenha, suscod, orgao,
					muncod
				) values (
					'%s', '%s', '%s', '%s',
					'%s', '%s', '%s',
					'%s', '%s', '%s', '%s',
					'%s', '%s'
				)",
            $cpf,
            str_to_upper( $usunome ),
            strtolower( $usuemail ),
            $usufoneddd,
            $usufonenum,
            $usufuncao,
            'f',
            $regcod,
            $ususexo,
            md5_encrypt_senha( $senhageral, '' ),
            'P',
            $orgao,
            $muncod
        );

        $this->executar( $sql );
    }
}