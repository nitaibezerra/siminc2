<?php
/**
 * Cabeçalho do sistema
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "includes/cabecalho.inc";
ini_set("memory_limit", "2048M");
set_time_limit(300000);
$fm = new Simec_Helper_FlashMessage('altorc/carga/cargapedidoalt');

function multipleArrayToArray(&$item) {
    $item = $item['cod'];
}

function explodeCSVLine($line) {
    return explode(';', trim($line));
}

if (isset($_POST['action'])) {
    if (!$_REQUEST['dados']['mcrid']) {
        $fm->addMensagem(
                'O campo Momento de crédito é obrigatório.', Simec_Helper_FlashMessage::ERRO
        );
        ?>
        <script>window.location = 'altorc.php?modulo=principal/previsaoupload&acao=A&execucao=erro';</script>';
        <?php
    }

    //ver($_SESSION,d);
    $errors = array(
        UPLOAD_ERR_OK => 'Arquivo carregado com sucesso.',
        UPLOAD_ERR_INI_SIZE => 'O tamanho do arquivo é maior que o permitido.',
        UPLOAD_ERR_PARTIAL => 'Ocorreu um problema durante a transferência do arquivo.',
        UPLOAD_ERR_NO_FILE => 'O arquivo enviado estava vazio.',
        UPLOAD_ERR_NO_TMP_DIR => 'O servidor não pode processar o arquivo.',
        UPLOAD_ERR_CANT_WRITE => 'O servidor não pode processar o arquivo.',
        UPLOAD_ERR_EXTENSION => 'O arquivo recebido não é um arquivo válido.'
    );
    // -- Processando o arquivo
    if (isset($_FILES['limite']['tmp_name']) && is_file($_FILES['limite']['tmp_name'])) {
        $data = file($_FILES['limite']['tmp_name']);

// -- Eliminando cabeçalho
        array_shift($data);
        // -- Quebrando as linhas em colunas
        $data = array_map(explodeCSVLine, $data);

        //Verificando se o arquivo esta igual ao modelo com 10 colunas
        $qnt = count($data[0]);
        if ($qnt == 10) {
            global $db;
            //Limpando a tabela temporaria
            $sqlTruncate = "TRUNCATE TABLE altorc.cargapedido";
            $db->executar($sqlTruncate);
            $db->commit();

            //inserindo os registros do arquivo csv
            $exercicioValido = true;
            $sql = 'INSERT INTO altorc.cargapedido (tipocredito, unicod, acacod, loccod, plocod, natcod, foncod,
       vlrsuplementacao, tiposuplementacao, vlrcancelamento
) values';
            foreach ($data as $campos) {
                $campos[7] = str_replace(',', '.', $campos[7]);
                $campos[9] = str_replace(',', '.', $campos[9]);
                $sql .= "('$campos[0]', '$campos[1]','$campos[2]', '$campos[3]', '$campos[4]', $campos[5],'$campos[6]', '$campos[7]','$campos[8]', '$campos[9]'),";
            }
            $sqlF = substr($sql, 0, -1);
            $db->executar($sqlF);
            $db->commit();

            //chamando a função para pegar os registros da tabela temporaria e incluir ou alterar na outra tabela
            $sqlDadosBanco = "SELECT * FROM altorc.cargaPedidosAlteracaoFinanceira('{$_SESSION['exercicio']}', {$_REQUEST['dados']['mcrid']}, '{$_SESSION['usucpf']}')";
            $resposta = $db->carregar($sqlDadosBanco);
            foreach (is_array($resposta)?$resposta:array() as $chave=>$valor){
                $temp = explode('-',$valor['cargapedidosalteracaofinanceira']);
                $dadosSaida[$chave]['funcional'] = $temp[0];
                $dadosSaida[$chave]['resultado'] = $temp[1];
            }
            $fm->addMensagem(
                'Carga realizada com sucesso.', Simec_Helper_FlashMessage::SUCESSO
            );
            $cabecalho = array(
                "Funcional",
                "Resposta"
            );
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setCabecalho($cabecalho);
            $listagem->setDados($dadosSaida);
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            exit;
            ?>

            <?php
        } else {
            $fm->addMensagem(
                    'O arquivo não possui todas as colunas necessárias.', Simec_Helper_FlashMessage::ERRO
            );
            ?>
            <script>window.location = 'altorc.php?modulo=principal/previsaoupload&acao=A&execucao=erro';</script>';
            <?php
        }
    }
    die();
}
?>
<style>
.pad-12{padding-top:12px!important}
.botao{text-align:center}
.button{position:absolute;top:50%}
</style>
<script type="text/javascript" language="JavaScript">
    function voltar() {
        window.location = 'altorc.php?modulo=principal/previsaoupload&acao=A';
    }

    $(document).ready(function() {
        $('input[type=file]').bootstrapFileInput();
        $(document).ready(function() {
            $('#buttonSend').click(function() {
                $('#previsaoupload').submit();
            })
        });
        $("input[type=checkbox]").bootstrapSwitch('setSizeClass', 'switch-mini');
        $('#buttonSend').click(function() {
            $('#previsaoupload').submit();
        });
        $('#import-data').click(function() {
            $('.has-error').removeClass('has-error');
            if ('' == $('#mcrid').val()) {
                var label = $('#label').text();
                $('#prfid_group').addClass('has-error');
                var msg = '<div class="alert alert-danger text-center">' + '<p>O campo <strong>' + label.replace(':', '') + '</strong> é obrigatório e não pode ser deixado em branco.</p>' + '</div>';
                $('.modal-body').html(msg);
                $('#modal-alert').modal();
                return false;
            }
            $('#dados_prfid').val($('#prfid').val());

            // -- na segunda etapa, executar a validação de período de referência
            $('#previsaoupload').submit();
        });

    });
</script>
<?php if ($_REQUEST['execucao'] != 'resultado' && $_REQUEST['execucao'] != 'erro') { ?>
    <div class="row">
        <div class="col-md-12">
            <ol class="breadcrumb">
                <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
                <li class="active">Carga de Pedidos</li>
            </ol>
            <div class="well">
                <form name="previsaoupload" id="previsaoupload" enctype="multipart/form-data" method="POST">
                    <div class="form-group row">
                        <div class="col-md-2">
                            <label class="control-label" for="mcrid">Momento de crédito:</label>
                        </div>
                        <div class="col-md-10">
                            <?php
                            $sqlMomentoCredito = <<<DML
SELECT mcr.mcrid AS codigo,
       mcr.mcrdsc AS descricao
  FROM altorc.momentocredito mcr
  WHERE mcr.mcrstatus = 'A'
    AND mcr.mcrano = '%s'
DML;
                            $sqlMomentoCredito = sprintf($sqlMomentoCredito, $_SESSION['exercicio']);
                            $db->monta_combo('dados[mcrid]', $sqlMomentoCredito, 'S', 'Selecione um Momento de crédito', 'carregaTipoCredito', null, null, null, 'N', 'mcrid', null, $mcrid, null, 'class="form-control chosen-select" style="width=100%"');
                            ?>
                        </div>
                    </div>
                    <div class="form-group row">
                        <div class="col-md-2">
                            <label class="control-label" for="mcrid"></label>
                        </div>
                        <div class="col-md-10">
                            <input type="file" title="Carregar arquivo de previsão (.csv)"
                                   name="limite" class="btn btn-primary start" />
                            <input type="hidden" name="action" id="action" value="carregar" />
                        </div>

                    </div>
                    <div class="form-group row">
                        <div class="col-md-2">
                            <label class="control-label" for="mcrid"></label>
                        </div>
                        <div class="col-md-10">
                            <table class="table table-striped table-bordered table-hover" style="width: 30%">
                                <tr><td colspan="2" align="center"><b>Tipo Suplementação</b></td></tr>
                                <tr>
                                    <td>1</td>
                                    <td>Suplementação por Cancelamento</td>
                                </tr>
                                <tr>
                                    <td>2</td>
                                    <td>Suplementação por Excesso</td>
                                </tr>
                                <tr>
                                    <td>3</td>
                                    <td>Suplementação por Superávit</td>
                                </tr>
                                <tr>
                                    <td>4</td>
                                    <td>Suplementação por Operação de Crédito</td>
                                </tr>
                            </table>
                        </div>
                    </div>
                    <div class="form-group row"><div class="col-md-2" ></div>
                        <div class="col-md-2" >
                            <div class="alert alert-info" style="margin-bottom:0">
                                <p>
                                    <i class="glyphicon glyphicon-download-alt"></i>
                                    <a href="arquivos/modelo_carga_alteracao_orcamentaria.csv" target="_blank">Download do modelo</a>.
                                </p>
                            </div>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-10">
                            <button class="btn btn-success" id="buttonSend" type="submit"><i class="glyphicon glyphicon-upload"></i> Enviar</button>
                            <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
                        </div>
                    </div>
                    <div style="clear:both"></div>
                </form>
            </div>
            <?php
        } elseif ($_REQUEST['execucao'] == 'erro') {
            echo $fm->getMensagens() . '<div class="botao"><button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button></div>';
        } elseif ($_REQUEST['execucao'] == 'resultado') {
            echo $fm->getMensagens() . '<br><table class="table table-striped table-bordered table-hover"><tr><td>Funcional Programática</td><td>Resultado</td></tr>' . $_REQUEST['resultado'] . '</table><br><div class="botao"><button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button></div>';
        }
        ?>
    </div>
