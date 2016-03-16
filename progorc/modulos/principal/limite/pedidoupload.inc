<?php
/**
 * Upload de arquivo de previsão de captação / receita
 * $Id: pedidoupload.inc 99009 2015-06-23 12:16:55Z werteralmeida $
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

function verificaExistenciaPdlnl($pdlnl,$anxcod){
    $anxcod = str_replace('/', '-', $anxcod);
    global $db;
    $sql = <<<DML
        SELECT pdl.pdlnl FROM progorc.pedidolimite pdl
        INNER JOIN progorc.pedidodetalhe pdd ON pdl.pdlid = pdd.pdlid
        INNER JOIN progorc.contacorrente cc ON pdd.ccrid = cc.ccrid
        WHERE pdlnl = '{$pdlnl}'
            AND cc.anxcod = '{$anxcod}'
DML;
    return ($db->pegaUm($sql)) ? true : false;
}

function cadastraPedidoLimite($unicod,$pdlreferencia,$docid,$pdlnl,$dataPedido){
    global $db;
    $sql = <<<DML
        INSERT INTO progorc.pedidolimite(
            unicod,
            pdljustificativa,
            pdlreferencia,
            usucpf, docid, pdlnl, pdldatapedido)
        values (%d, '%s', '%s', '%s', %d, %d, %s)
        RETURNING pdlid
DML;
    $stmt = sprintf($sql, $unicod, 'Cadastrado por carga automática', $pdlreferencia, $_SESSION ['usucpf'], $docid, $pdlnl, $dataPedido);
    $pdlid = $db->pegaUm($stmt);
    if (!$db->commit()) {
        $db->rollback();
        setFlashMessage(false, 'Não foi possível executar sua requisição.');
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $unicod);
        die();
    }
    return $pdlid;
}

function cadastraPedidoDetalhe($anxcod,$ccrcod,$pdlid,$valorAmpliacao1,$valorReducao1){
    global $db;
    $anxcod = str_replace('/','-',$anxcod);
    $sqlConta = <<<DML
        SELECT ccrid FROM progorc.contacorrente WHERE anxcod = '{$anxcod}' AND ccrcod = '{$ccrcod}';
DML;
    $ccrid = $db->pegaUm($sqlConta);
    $sqldetalhe = <<<DML
        INSERT INTO progorc.pedidodetalhe(
            pdlid,
            pddvaloramplicacao,
            pddvalorreducao,
            pddvaloramplicacaoatendido,
            pddvalorreducaoatendido,
            ccrid)
        values (%d,%s, %s, %s, %s, %s)
        RETURNING pddid
DML;
    $stmt = sprintf($sqldetalhe, $pdlid, $valorAmpliacao1, $valorReducao1, $valorAmpliacao1, $valorReducao1, $ccrid);
    $pddid = $db->pegaUm($stmt);

    if (!$db->commit()) {
        $db->rollback();
        setFlashMessage(false, 'Não foi possível executar sua requisição.');
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $unicod);
        die();
    }
    return $pddid;
}

/**
 * @global cls_banco $db
 * @param type $dados
 * @return string
 */
function upsertPedido($dados) {
    $sucesso = true;
    $msg = 'Requisição executada com sucesso.';
    $pdlndNok = 0;
    $pdlidOK = 0;
    foreach ($dados as $limite) {
        if (verificaExistenciaPdlnl($limite['2'], $limite['3'])) {
            $pdlndNok ++;
            continue;
        }
        $valorAmpliacao1 = str_replace(",", ".", $limite[5]);
        $valorReducao1 = str_replace(",", ".", $limite[6]);
        $valorAmpliacao2 = str_replace(",", ".", $limite[8]);
        $valorReducao2 = str_replace(",", ".", $limite[9]);
        $data = data_banco($limite['10']);
        if ($data == '') {
            setFlashMessage(false, 'Não foi possível executar sua requisição. Alguma DATA no arquivo está no formato incorreto. (Ex.: 14 ao invés de 2014).');
            header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $limite['0']);
            die();
        }
        $docid = wf_cadastrarDocumento(TPDID_PROGORC_1, 'Fluxo de solicitação de Limite.');
        $pdlidR = cadastraPedidoLimite($limite['0'],$limite['1'],$docid,$limite['2'],$data);

        if ($limite['3'] && $limite['4']) {
            $pddid1 = cadastraPedidoDetalhe($limite['3'], $limite['4'], $pdlidR, $valorAmpliacao1, $valorReducao1);
        }

        if ($limite['3'] && $limite['7']) {
            $pddid2 = cadastraPedidoDetalhe($limite['3'], $limite['7'], $pdlidR, $valorAmpliacao2, $valorReducao2);
        }

        $justificativa = 'Carga Automática';
        $aedid = AEDID_NAO_ENVIADO_ATENDIDO_CARGA;
        wf_alterarEstado($docid, $aedid, $justificativa, array());
        $pdlidOK ++;
        
    }
    setFlashMessage($sucesso, $msg);
    ?>
    <script>window.location = 'progorc.php?modulo=principal/limite/pedidoupload&acao=A&execucao=resultado&cad=<?php echo $pdlidOK; ?>&ncad=<?php echo $pdlndNok; ?>';</script>
    <?php
    die();
}

function cadastraCargaPedidos(){
    global $db;
    $data = file($_FILES['limite']['tmp_name']);
    // -- Eliminando cabeçalho
    array_shift($data);
    // -- Quebrando as linhas em colunas
    $data = array_map(explodeCSVLine, $data);
    
    $sqlTruncate = "DELETE FROM progorc.cargapedidos";
    $db->executar($sqlTruncate);
    $db->commit();

    $exercicioValido = true;
    $sql = <<<DML
        INSERT INTO progorc.cargapedidos
            (unicod, pdlreferencia, pdlnl, tipo, conta1, valorampliacao1, valorreducao1,conta2, valorampliacao2, valorreducao2, pdldatapedido)
        values
DML;
    foreach ($data as $campos) {
        $valorAmpliacao1 = str_replace(",", ".", $campos[5]);
        $valorReducao1 = str_replace(",", ".", $campos[6]);
        $valorAmpliacao2 = str_replace(",", ".", $campos[8]);
        $valorReducao2 = str_replace(",", ".", $campos[9]);
        $sql .= <<<DML
            ('$campos[0]', '$campos[1]','$campos[2]', '$campos[3]', '$campos[4]','$valorAmpliacao1',
            '$valorReducao1', '$campos[7]','$valorAmpliacao2', '$valorReducao2', '$campos[10]'),
DML;
    }
    $sqlF = substr($sql, 0, -1);
    $db->executar($sqlF);
    $db->commit();
    return $data;
}

function multipleArrayToArray(&$item) {
    $item = $item['cod'];
}

function explodeCSVLine($line) {
    return explode(';', trim($line));
}

if (isset($_POST['action'])) {
    // -- Processando o arquivo
    if (isset($_FILES['limite']['tmp_name']) && is_file($_FILES['limite']['tmp_name'])) {
        upsertPedido(cadastraCargaPedidos());
    }
    header('Location: ' . $_SERVER['REQUEST_URI']);
    die();
}

/**
 * Cabeçalho do SIMEC
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
<div class="row">
    <div class="col-md-12">
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Carga Pedidos de Limite</li>
        </ol>
        <div class="well">
            <form name="previsaoupload" id="previsaoupload" enctype="multipart/form-data" method="POST">
                <div class="form-group">
                    <div class="col-md-10">
                        <input type="file" title="Carregar arquivo de previsão (.csv)"
                               name="limite" class="btn btn-primary start" />
                        <p class="help-block">Todo anexo deve estar *obrigatoriamente no formato CSV e de acordo com o modelo. <br>Em caso de dúvidas o modelo pode ser baixado clicando no botão ao lado.</p>                               
                        <input type="hidden" name="action" id="action" value="carregar" />
                        
                    </div>
                    <div class="col-md-2">
                        <div class="alert alert-info" style="margin-bottom:0">
                            <p>
                                <i class="glyphicon glyphicon-download-alt"></i>
                                <a href="arquivos/pedido.csv" target="_blank">Download do modelo</a>.
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
                echo '<h4><center><br>' . $_REQUEST['cad'] . ' pedidos cadastrados com sucesso.<br>';
            } else {
                echo '<h4><center><br>Nenhum pedido cadastrado.<br>';
            }
            if ($_REQUEST['ncad'] > 0) {
                echo $_REQUEST['ncad'] . ' pedidos não foram cadastrados, pois já havia o cadastro dos mesmos.</center></h4>';
            } else {
                echo 'Nenhum pedido não foi cadastrado por já haver o cadastro do mesmo.</center></h4>';
            }
        }
        ?>
    </div>