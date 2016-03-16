<?php
class Educriativa_Model_Questionario extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 * 
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela =  "criatividadeeducacao.questionario";
	
	/**
	 * Chave primaria.
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
		"queid" 
	);
	
	/**
	 * Atributos
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'queid' => null,
			'orgid' => null,
			'parid' => null,
			'quedtcriacao' => null,
			'quesituacao' => null
	);
	
	public function atualizar() {
		try {
			$data = $_REQUEST;
	
			$this->carregarPorId($_SESSION['queid']);
	
			$resposta = new Educriativa_Model_Resposta();
			$resposta->salvarRespostas(simec_utf8_decode_recursive($_REQUEST['perid']));
				
			$this->commit();
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}
	
	public function finalizar() 
	{
		try {
			$this->carregarPorId($_SESSION['queid']);

            $this->verificarQuestionario();

			$this->quesituacao = 'F';

			$this->alterar();
			$this->commit();
            $this->enviarEmailFinalizacao();

		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}

    public function enviarEmailFinalizacao()
    {
        $participante = new Educriativa_Model_Participante($this->parid);
        if($participante->paremail){
            $saudacao = $participante->parsexo == 'F' ? 'Prezada Sra.' : 'Prezado Sr.';

            $assunto = 'Inovação e Criatividade na Educação Básica';

            $conteudo = "<pre>{$saudacao} {$participante->parnome},

Sua inscrição na Chamada Pública Inovação e Criatividade na Educação Básica foi finalizada corretamente.
Obrigado por sua participação.
</pre>";
            simec_email(array('nome'=>'Inovação e Criatividade na Educação Básica', 'email'=>'simec@mec.gov.br'), $participante->paremail, $assunto, $conteudo);
        }
    }

	public function verificarQuestionario()
	{
		$sql = "select orgrazaosocial, orglogradouro, orgbairro, orgnumeroendereco, o.estuf, o.muncod, gruid, ortid, orgqtdestudantes, orgqtdfuncionarios
                from criatividadeeducacao.questionario  q
                    inner join criatividadeeducacao.organizacao o on o.orgid = q.orgid
                where queid = {$this->queid}";

        $dados = $this->pegaLinha($sql);
        $dados = $dados ? $dados : array();

        $campoVazio = array();
        foreach ($dados as $key => $dado) {
            if(!$dado){
                $campoVazio[$key] = $key;
            }
        }

        if(in_array('ortid', $campoVazio) && 5 == $dados['gruid']){
            unset($campoVazio['ortid']);
        }

        if(count($campoVazio)){
            echo 'Favor preencher todos os campos obrigatórios (marcados com *)';
            die;
        }
	}

	public function carregarPorCpf($cpfSomenteNumero) {
		$sql = "SELECT q.*
				  FROM criatividadeeducacao.questionario q
			inner join criatividadeeducacao.participante p on p.parid = q.parid
			     WHERE p.parcpf = '{$cpfSomenteNumero}'";
	
		$arResultado = $this->pegaLinha( $sql );
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
	}
}