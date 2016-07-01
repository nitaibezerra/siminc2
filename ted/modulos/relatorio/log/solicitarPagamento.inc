<?php
function responds_to_parse(array $row) {

    $search = "<?xml version=1.0 encoding=iso-8859-1?>";
    $replace = "<?xml version='1.0' encoding='iso-8859-1'?>";

    if (!empty($row['logxmlenvio'])) {
        $row['logxmlenvio'] = str_replace($search, $replace, $row['logxmlenvio']);
    }

    if (!empty($row['logxmlretorno'])) {
        $row['logxmlretorno'] = str_replace($search, $replace, $row['logxmlretorno']);
    }

    return $row;
}


if (Ted_Utils_Model::isAjax() && isset($_GET['cod']) && is_numeric($_GET['cod'])) {

    $strSQL = sprintf("
                select
                    l.logid,
                    u.usunome ||' - '|| l.usucpf AS usuario,
                    l.tcpid,
                    l.logmsg,
                    TO_CHAR(l.logdata, 'DD/MM/YYYY') as logdata,
                    l.logxmlenvio,
                    l.logxmlretorno,
                    l.logerro
                from ted.log l
                JOIN seguranca.usuario u ON (u.usucpf = l.usucpf)
                where l.logtipo = '%s'
                AND l.logid = %d
            ", Ted_Model_Log::ENVIA_NOTA_CREDITO_PAGAMENTO_FNDE, (int) $_GET['cod']);

    if ($log = $db->pegaLinha($strSQL)) {

        $log = responds_to_parse($log);

        if (!empty($log['logxmlenvio'])) {
            try {
                $xmlEnvio = new SimpleXMLElement($log['logxmlenvio']);
                //$xmlEnvio->body->auth->senha = '**********';
                $domEnvio = dom_import_simplexml($xmlEnvio)->ownerDocument;
                $domEnvio->formatOutput = true;
                $request = $domEnvio->saveXML();
            } catch(Exception $e) {
                echo $e->getMessage(); die;
            }
        } else {
            $request = '';
        }

        if (!empty($log['logxmlretorno'])) {
            $xmlRetorno = new SimpleXMLElement($log['logxmlretorno']);
            $domRetorno = dom_import_simplexml($xmlRetorno)->ownerDocument;
            $domRetorno->formatOutput = true;
            $response = $domRetorno->saveXML();
        } else
            $response = '';

        echo "
            <table class='table table-striped table-bordered table-hover'>
                <tr>
                    <td class='text-right'><strong>Ted:</strong></td>
                    <td>{$log['tcpid']}</td>
                </tr>
                <tr>
                    <td class='text-right'><strong>Data solicitação:</strong></td>
                    <td>{$log['logdata']}</td>
                </tr>
                <tr>
                    <td class='text-right'><strong>Usuário:</strong></td>
                    <td>{$log['usuario']}</td>
                </tr>
                <tr>
                    <td class='text-right' valign='top'><strong>Xml envio:</strong></td>
                    <td>
                        <textarea class='form-control' name='xmlEnvio' cols='100' rows='10' readonly='readonly'>{$request}</textarea>
                    </td>
                </tr>
                <tr>
                    <td class='text-right' valign='top'><strong>Xml resposta:</strong></td>
                    <td>
                        <textarea class='form-control' name='xmlEnvio' cols='100' rows='10' readonly='readonly'>{$response}</textarea>
                    </td>
                </tr>
        </table>
        ";
    }
    die;
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li class="active">Log de envio para Pagamento - SIGEF (FNDE)</li>
    </ol>

    <section class="well col-md-12">
        <h4 class="text-center">Log SIGEF (FNDE) - Pagamento da Nota de Crédito</h4>
    </section>

    <div class="col-md-12">
        <?php
        $strSQL = sprintf("
                SELECT
                    l.logid,
                    u.usunome ||' - '|| l.usucpf AS usuario,
                    l.tcpid,
                    l.logmsg,
                    TO_CHAR(l.logdata, 'DD/MM/YYYY'),
                    l.logerro
                FROM ted.log l
                JOIN seguranca.usuario u ON (u.usucpf = l.usucpf)
                WHERE l.logtipo = '%s'
                ORDER BY l.logid DESC
            ", Ted_Model_Log::SOLICITA_NOTA_CREDITO_FNDE);

        require APPRAIZ . 'includes/library/simec/Listagem.php';

        $list = new Simec_Listagem();
        $list->setCabecalho(array(
            'Usuário Nome',
            'Ted',
            'Mensagem',
            'Data',
            'Status'
        ));
        $list->addAcao('edit', 'viewXml');
        $list->setQuery($strSQL);

        $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $list->turnOnPesquisator();
        $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
        ?>
    </div>

    <div class="modal fade" id="modal" tabindex="-1" role="dialog" aria-labelledby="ncModalLabel" aria-hidden="true">
        <div class="modal-dialog" style="width:70%;">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">
                        <span aria-hidden="true">&times;</span>
                        <span class="sr-only">Close</span>
                    </button>
                    <h4 class="modal-title text-center">Log Requisições ao SIGEF</h4>
                </div>
                <div class="modal-body">

                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    var viewXml = function(cod) {
        if (!cod) return false;

        var url = "ted.php?modulo=relatorio/log/solicitarNcSigef&acao=A"
            , $promisse = $.ajax({url:url+"&cod="+cod})
            , $modal = $("#modal");

        $promisse.done(function(response) {
            $modal.find(".modal-body").html(response);
            $modal.modal("show");
        });
    };
</script>