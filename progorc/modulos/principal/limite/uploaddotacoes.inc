<?php
/**
 * Upload de arquivo de previsão de captação / receita
 * $Id: pedidoupload.inc 81067 2014-05-30 20:21:17Z werteralmeida $
 */
include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "www/recorc/_funcoes.php";
ini_set("memory_limit", "2048M");
set_time_limit(300000);

function data_banco($data) {
    if (preg_match('#(\d{1,2})\D(\d{1,2})\D(\d{4})#', $data, $match)) {
        $time = mktime(0, 0, 0, $match[2], $match[1], $match[3]);
        return "'" . date('Y-m-d', $time) . "'";
    }
}

/**
 * @global cls_banco $db
 * @param type $dados
 * @return string
 */
function upsertPedido() {
    $sucesso = true;
    $msg = 'Requisição executada com sucesso.';
    global $db;
    $dtcidUpdate = 0;
    $dtcidCAdd = 0;
    $sql = 'SELECT * FROM progorc.cargadotacaoconta';
    $dotacoes = $db->carregar($sql);
    foreach ($dotacoes as $dotacao) {
        $sql = "select dtcid from progorc.dotacaoconta where ccrid = '{$dotacao['ccrid']}' and unicod = '{$dotacao[unicod]}' and exercicio= '{$dotacao['exercicio']}' and referencia = '{$dotacao['referencia']}'";
        $dtcidC = $db->pegaUm($sql);
     if ($dtcidC) {
            $sql = <<<DML
UPDATE progorc.dotacaoconta
   SET dtcdotacaoatual='%d'
 WHERE dtcid = '%d'
RETURNING dtcid
DML;
            $stmt1 = sprintf($sql, $dotacao['dtcdotacaoatual'], $dtcidC);
            $dtcidR = $db->pegaUm($stmt1);
            $dtcidUpdate ++;
            if (!$db->commit()) {
                $db->rollback();
                setFlashMessage(false, 'Não foi possível executar sua requisição.');
                header('Location: progorc.php?modulo=principal/limite/uploaddotacoes&acao=A');
                die();
            }
        } if ($dtcidC == '' || $dtcidC == null) {
                $sqlInsert = <<<DML
INSERT INTO progorc.dotacaoconta(
    unicod,
    ccrid,
    dtcdotacaoatual, 	
    exercicio,
    referencia)
values (%d,%d, %s, %d, '{$dotacao['referencia']}')
DML;
                
                $stmt = sprintf($sqlInsert, $dotacao['unicod'], $dotacao['ccrid'], $dotacao['dtcdotacaoatual'], $dotacao['exercicio'],(string) $dotacao['referencia']);
                $dtcidP = $db->pegaUm($stmt);
                 if (!$db->commit()) {
                $db->rollback();
                setFlashMessage(false, 'Não foi possível executar sua requisição.');
                header('Location: progorc.php?modulo=principal/limite/uploaddotacoes&acao=A');
                die();
            }
            $dtcidCAdd ++;
        }
    }
    setFlashMessage($sucesso, $msg);
    ?>
    <script>window.location = 'progorc.php?modulo=principal/limite/uploaddotacoes&acao=A&execucao=resultado&cad=<?php echo $dtcidCAdd; ?>&ncad=<?php echo $dtcidUpdate; ?>';</script>
    <?php
//header ( 'Location: progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista&pdlid=' + $pdlidR );
    die();
}

function multipleArrayToArray(&$item) {
    $item = $item['cod'];
}

function explodeCSVLine($line) {
    return explode(';', trim($line));
}

if (isset($_POST['action'])) {
	if(!in_array($_FILES['limite']['type'], array('application/vnd.ms-excel', 'text/csv'))){
		echo "<script>
				alert('Formato de Arquivo inválido.');
				window.location = 'progorc.php?modulo=principal/limite/uploaddotacoes&acao=A';
			</script>";
		die();
	}
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
        global $db;
        //    $sqlTruncate = "truncate recorc.dadoscargaprevisao";
        $sqlTruncate = "DELETE FROM progorc.cargadotacaoconta";
        $db->executar($sqlTruncate);
        $db->commit();


        $exercicioValido = true;
        $sql = 'INSERT INTO progorc.cargadotacaoconta(unicod, ccrid, dtcdotacaoatual, exercicio,referencia) values';
        foreach ($data as $campos) {
            if(strlen(trim($campos[3])) > 1){
                echo "<script>
                    alert('Referência inserida possui mais de 1 caráctere.');
                    window.location = 'progorc.php?modulo=principal/limite/uploaddotacoes&acao=A';
                    </script>";
		die();
            }
                
            $sqlConta = "SELECT ccrid FROM progorc.contacorrente WHERE ccrcod = '{$campos[1]}'";
            $idconta = $db->pegaUm($sqlConta);
            $valorDotacao = str_replace(",", ".", $campos[2]);
            $sql .= "('$campos[0]', '$idconta','$valorDotacao', '{$_SESSION['exercicio']}','{$campos[3]}'),";
        }
        $sqlF = substr($sql, 0, -1);
        $db->executar($sqlF);
        $db->commit();

        $sqlDadosBanco = <<<DML
SELECT cdc.unicod || ' - ' || unidsc AS unidade,
       cdc.ccrid AS conta,
       cdc.dtcdotacaoatual AS valor_dotacao
  FROM progorc.cargadotacaoconta cdc
    LEFT JOIN public.unidade uni ON (uni.unicod = cdc.unicod)
  ORDER BY uni.unicod
DML;
        $qtdRegistros = $db->pegaUm("SELECT COUNT(1) FROM({$sqlDadosBanco}) data");
    }
    upsertPedido();
    header('Location: ' . $_SERVER['REQUEST_URI']);
    die();
}


/**
 * Cabeçalho do sistema
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script type="text/javascript" src="../library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
<link rel="stylesheet" type="text/css" href="../library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<style>
    .pad-12{padding-top:12px!important}
</style>
<script type="text/javascript" language="JavaScript">
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
            if ('' == $('#prfid').val()) {
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
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Parâmetros SPO</li>
        <li class="active">Carga das Dotações  - <?php echo $_SESSION['exercicio']; ?></li>
    </ol>
</div>
<div class="row">
    <div class="col-md-12">
        <div class="well">
            <form name="previsaoupload" id="previsaoupload" enctype="multipart/form-data" method="POST">
                <div class="form-group">
                    <div class="col-md-10">
                        <input type="file"  title="Carregar arquivo de previsão (.csv)"
                               name="limite" class="btn btn-primary start filestyle" />
                        <p class="help-block">Todo anexo deve estar *obrigatoriamente no formato CSV e de acordo com o modelo. <br>Em caso de dúvidas o modelo pode ser baixado clicando no botão ao lado.</p>
                        <input type="hidden" class="" name="action" id="action" value="carregar" />
                    </div>
                    <div class="col-md-2">
                        <div class="alert alert-info" style="margin-bottom:0">
                            <p>
                                <i class="glyphicon glyphicon-download-alt"></i>
                                <a href="arquivos/modelo_carga_dotacao.csv" target="_blank">Download do modelo</a>.
                            </p>
                        </div>
                    </div>
                    <div class="form-group">
                        <div class="col-lg-10">
                            <button class="btn btn-success" id="buttonSend" type="submit"><i class="glyphicon glyphicon-upload"></i> Enviar</button>
                            <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
                        </div>
                    </div>
                    <div style="clear:both"></div>
                </div>
            </form>
        </div>
        <?php
        echo getFlashMessage() . '<br style="clear:both;">';
        if ($_REQUEST['execucao'] == 'resultado') {
            if ($_REQUEST['cad'] > 0) {
                echo '<h4><center><br>' . $_REQUEST['cad'] . ' dotação(ões) nova(s) cadastrada(s) com sucesso.<br>';
            } else {
                echo '<h4><center><br>Nenhuma dotação nova cadastrada.<br>';
            }
            if ($_REQUEST['ncad'] > 0) {
                echo $_REQUEST['ncad'] . ' dotações foram alterados, pois já havia o cadastro dos mesmos.</center></h4>';
            } else {
                echo 'Nenhuma dotação não foi cadastrada por já haver o cadastro do mesmo.</center></h4>';
            }
        }
        ?>
    </div>
</div>