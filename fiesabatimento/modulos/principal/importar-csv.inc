<?php
    include  APPRAIZ."includes/cabecalho.inc";
    echo "<br>";
    
    define("PRIMEIRA_VALIDACAO",1);
    define("SEGUNDA_VALIDACAO",2);
    define("TERCEIRA_VALIDACAO",3);
    
    # Verifica arquivo enviado via POST(Do formulario)
    if (isset($_FILES['hidFile'])) {
        //Importar o arquivo transferido para o banco de dados
        $handle = fopen($_FILES['hidFile']['tmp_name'], "r");

        $numLinhas = 0;
        while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
            // Verifica se não é a primeira linha @example: "Cpf;Count"
            if($numLinhas > 0){ // && $numLinhas <= 10
                $listaLinha = explode(';',$data[0]);
                $cpf = $listaLinha[0];
                $qtd = $listaLinha[1];
                // Verifica se o CPF já foi validado
                if(verificarCpfTestado($cpf) === FALSE){
                    $nuValidacaoPendente = 'NULL';
                    $dsValidacaoPendente = 'NULL';
                    $stValidacao = "'S'";
                    $dtValidacao = 'NULL';

                    $sqlInserirCpf = "
                        INSERT INTO fiesabatimento.relatorio_validacao_docente
                        (
                            nu_cpf,
                            nu_quantidade_acesso,
                            nu_validacao_pendente,
                            ds_validacao_pendente,
                            st_validacao,
                            dt_validacao,
                            dt_cadastro
                        )
                        VALUES
                    ";

                    # Primeira validação
                    $dadosUsu = pegarInfoUsuario($cpf);
                    $listaPendencia = validarDadosIdentificacaoFreire($dadosUsu?$dadosUsu:Array());
                    if(count($listaPendencia) > 0 ){
                        $nuValidacaoPendente = PRIMEIRA_VALIDACAO;
                        $dsValidacaoPendente = join(',', $listaPendencia);
                        $stValidacao = "'N'";
                        $dtValidacao = "'".date('Y-m-d H:i:s')."'";
                    } else {

                        # Segunda Validação
                        $erro = testeWS( $dadosUsu );
                        if($erro!=''){
                            $nuValidacaoPendente = SEGUNDA_VALIDACAO;
                            $dsValidacaoPendente = $erro;
                            $stValidacao = "'N'";
                            $dtValidacao = "'".date('Y-m-d H:i:s')."'";
                        } else {
                            # Terceira Validação
                            $dados = pegarAtuacaoUsuario($cpf);
                            $teste = calculaMeses($dados);
                            if( !$teste['meses'] ) $teste['meses'] = '0';
                             $dadosAtu = pegarAtuacaoUsuario($cpf);
                            if(!$dadosAtu[0]){
                                $erro = "Dados de atuação profissional não preenchidos. Efetue a correção na Plataforma Freire.";
                                $nuValidacaoPendente = TERCEIRA_VALIDACAO;
                                $dsValidacaoPendente = $erro;
                                $stValidacao = "'N'";
                                $dtValidacao = "'".date('Y-m-d H:i:s')."'";
                            } else {
                                if($teste['meses'] < 12){
                                    $erro = "Para solicitar o abatimento é necessário ter jornada de, no mínimo, 12 (doze) meses de trabalho 
                                    ininterrupto com no mínimo 20 (vinte) horas semanais como docente na rede pública de educação básica.";
                                    $nuValidacaoPendente = TERCEIRA_VALIDACAO;
                                    $dsValidacaoPendente = $erro;
                                    $stValidacao = "'N'";
                                    $dtValidacao = "'".date('Y-m-d H:i:s')."'";
                                }
                            }
                        }
                    }

                    $sqlInserirCpf .= "
                        (
                            '$cpf',
                            $qtd,
                            $nuValidacaoPendente,
                            '$dsValidacaoPendente',
                            $stValidacao,
                            $dtValidacao,
                            now()
                        );";
                        $db->executar($sqlInserirCpf);
                        $db->commit();
                }
            }
            $numLinhas++;
        }
        # Fecha o arquivo e libera da memoria
        fclose($handle);
        # Executa inserção
        
        # Resposta da solicitação
        echo "<script> alert('Quantidade de linhas Lidas: ".($numLinhas-1)."'); </script>";
//        echo "<br />";
//        echo "Quantidade de CPF importados: ". count($listaCpf);
    }
    
    /**
     * Realiza a primeira validação verificando se existe campos essenciais não preenchidos
     * oriundos da plataforma freire
     * 
     * @param array
     * @return array informa campos não preenchidos
     */
    function validarDadosIdentificacaoFreire($dadosUsu){
        $pendencia = array();

        if((trim($dadosUsu[nu_cpf])== false)){
            $pendencia[] = 'CPF';
        }
        if((trim($dadosUsu[nu_rg])== false)){
            $pendencia[] = 'RG';
        }
        if((trim($dadosUsu[nu_cep])== false)){
            $pendencia[] = 'CEP';
        }
        if((trim($dadosUsu[ds_numero])== false)){
            $pendencia[] = 'Número do Endereço';
        }
        if((trim($dadosUsu[ds_bairro])== false)){
            $pendencia[] = 'Bairro';
        }
        if((trim($dadosUsu[nu_ddd])== false)){
            $pendencia[] = 'DDD do telefone';
        }
        if((trim($dadosUsu[no_pessoa])== false)){
            $pendencia[] = 'Nome completo';
        }
        if((trim($dadosUsu[ds_logradouro])== false)){
            $pendencia[] = 'Logradouro';
        }
        if((trim($dadosUsu[nu_telefone])== false)){
            $pendencia[] = 'Telefone';
        }
        if((trim($dadosUsu[dt_nascimento])== false)){
            $pendencia[] = 'Data de nascimento';
        }
        if((trim($dadosUsu[no_municipio_acento])== false)){
            $pendencia[] = 'Município';
        }
        if((trim($dadosUsu[ds_contato_eletronico])== false)){
            $pendencia[] = 'Contato Eletrônico';
        }
        
        return $pendencia;
    }
    
    function verificarCpfTestado($cpf){
        global $db;
        $sql = "
            SELECT 
                nu_cpf
            FROM 
                fiesabatimento.relatorio_validacao_docente
            WHERE
                nu_cpf = '$cpf'";
        $cpf = $db->pegaUm($sql);
        return !empty($cpf)?TRUE:FALSE;
    }
?>

<div class="col-lg-12">
    <div class="page-header">
        <h4 id="forms">Batimento de CPF que supostamente passaram pela validação do sistema.</h4>
    </div>
    <br />
    
    <div class="well">
        <fieldset>
            <form enctype='multipart/form-data' method='post'>
                <div class="form-group">
                        <label for="file" class="col-lg-2 control-label text-right">
                            Arquivo:
                        </label>
                        <div class="col-lg-6">
                            <input type="file" class="btn btn-primary start" name="hidFile" id="hidFile" title="Selecionar arquivo" />
                        </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-2 col-lg-offset-2">
                        &nbsp;
                    </div>
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" id='btnLimpar' type="reset">Limpar</button>
                        <button class="btn btn-success" id="btnSalvar" type="submit">
                            <i class="glyphicon glyphicon-upload"></i>
                            Enviar
                        </button>
                    </div>
                </div>
            </form>
        </fieldset>    
    </div>
    <!--Implements here-->
    
</div>

<script>
    jQuery(document).ready(function(){
        jQuery('#btnLimpar').click(function(){
            limpar();
        });
    });
    
    function limpar(){
        window.location = window.location;
    }
</script>