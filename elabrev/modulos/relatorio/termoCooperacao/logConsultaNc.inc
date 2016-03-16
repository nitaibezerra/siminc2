<?php
/**
 * Created by PhpStorm.
 * User: LucasGomes
 * Date: 21/05/14
 * Time: 11:48
 */

/**
 * Ajusta o XML para ser parseado pelo SimpleXMLElement
 * @param array $row
 * @return array
 */
function responds_to_parse(array $row) {

    $search = "<?xml version=1.0 encoding=iso-8859-1?>";
    $replace = "<?xml version='1.0' encoding='iso-8859-1'?>";

    if (!empty($row['lwsncrequestcontent'])) {
        $row['lwsncrequestcontent'] = str_replace($search, $replace, $row['lwsncrequestcontent']);
    }

    if (!empty($row['lwsncresponsecontent'])) {
        $row['lwsncresponsecontent'] = str_replace($search, $replace, $row['lwsncresponsecontent']);
    }

    return $row;
}


if (isset($_POST['idLog'])) {
    $sqlLog = "select * from elabrev.logwsconsultancsigef WHERE lwsnc = ".(int) $_POST['idLog'];
    $linha = $db->pegaLinha($sqlLog);
    if ($linha) {

        /*
        $linha['lwsncrequestcontent'] = utf8_decode($linha['lwsncrequestcontent']);
        $linha['lwsncresponsecontent'] = utf8_decode($linha['lwsncresponsecontent']);
        $linha['lwsnc'];
        ver($linha, d);


        $sqlinsert = "update elabrev.logwsconsultancsigef set lwsncrequestcontent = '%s', lwsncresponsecontent = '%s' where lwsnc = %d";

        $stmt = sprintf($sqlinsert, $linha['lwsncrequestcontent'], $linha['lwsncresponsecontent'], $linha['lwsnc']);
        ver($stmt, d);
        if (in_array($linha['lwsnc'], array(1,3,4,6,8))) {
            $db->executar($stmt);
            $db->commit();
            echo '<script> alert("ok"); location.reload(); </script>';
        }
        */

        $linha = responds_to_parse($linha);

        if (!empty($linha['lwsncrequestcontent'])) {
            try {
                $xmlEnvio = new SimpleXMLElement($linha['lwsncrequestcontent']);
                $xmlEnvio->body->auth->senha = '**********';
                $domEnvio = dom_import_simplexml($xmlEnvio)->ownerDocument;
                $domEnvio->formatOutput = true;
                $request = $domEnvio->saveXML();
            } catch(Exception $e) {
                echo $e->getMessage(); die;
            }
        } else {
            $request = '';
        }

        if (!empty($linha['lwsncresponsecontent'])) {
            $xmlRetorno = new SimpleXMLElement($linha['lwsncresponsecontent']);
            $domRetorno = dom_import_simplexml($xmlRetorno)->ownerDocument;
            $domRetorno->formatOutput = true;
            $response = $domRetorno->saveXML();
        } else
            $response = '';

        ?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
            <tr>
                <td colspan="2" align="center" class="TituloTela">Log de consulta NC - SIGEF</td>
            </tr>
            <tr>
                <td width="12%" class="subtituloDireita" valign="top">Id do Termo</td>
                <td><?php echo $linha['lwsnctcpid']; ?></td>
            </tr>
            <tr>
                <td width="12%" class="subtituloDireita" valign="top">Data de envio</td>
                <td><?php echo $linha['lwsncrequesttimestamp']; ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita" valign="top">XML de envio</td>
                <td>
                    <textarea name="xmlEnvio" cols="90" rows="12" readonly="readonly"><?php echo $request; ?></textarea>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita" valign="top">XML de retorno</td>
                <td>
                    <textarea name="xmlRetorno" cols="90" rows="12" readonly="readonly"><?php echo $response; ?></textarea>
                </td>
            </tr>
            <tr>
                <td colspan="2" align="center">
                    <input type="button" class="fecharBtn" name="fecharBtn" value="Fechar">
                </td>
            </tr>
        </table>
    <?php
    }
    exit;
}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Relatório - Consultar NC', '');

$strSQL = "SELECT
        '<input type=\'button\' class=\'verLog\' data-target-id=\'' || lwsnc || '\' name=\'lwsnc[]\' value=\'Ver\' />' AS acao,
        lwsnctcpid, lwsncmetodo, lwsncrequesttimestamp
    from elabrev.logwsconsultancsigef
    ORDER BY lwsnc DESC";

$cabecalho = array('Ação', 'Termo', 'metodo', 'Data envio');
$db->monta_lista($strSQL, $cabecalho, 100, 5, 'N', 'center', 'N', 'formulario', null, null, null, array());
?>

<div class="tb_modal"></div>

<script type="text/javascript">
    jQuery(".verLog").click(function(e){
        e.preventDefault();
        var idLog = jQuery(this).attr("data-target-id");
        if (!idLog) return false;

        overLay();
        jQuery.ajax({
            url:"/elabrev/elabrev.php?modulo=relatorio/termoCooperacao/logConsultaNc&acao=A"
            , type:"POST"
            , data:{idLog:idLog}
            , beforeSend: function(){

            }
            , success: function(data){
                jQuery(".tb_modal").css({
                    "width":"950px",
                    "height":"660px",
                    "top": "50%",
                    "left": "50%",
                    "margin-left":"-475px",
                    "margin-top": "-350px",
                    "border": "1px solid #000",
                    "display": "block",
                    "position": "fixed",
                    "background": "#fff",
                    "overflow": "auto",
                    "z-index": 19009990
                }).html(data);
            }
            , complete: function(){
                modalClose();
            }
        });
    });

    function overLay() {
        var backgroundElement = jQuery("<div id=\"overlay\" ></div>");
        backgroundElement.appendTo("body");
        backgroundElement.css({
            'width': jQuery(document).width(),
            'height': jQuery(document).height(),
            'display': 'none',
            'background-color': '#555555',
            'position': 'absolute',
            'top': 0,
            'left': 0,
            'z-index': 9990
        });
        backgroundElement.fadeTo('fast', 0.8);
    }

    function modalClose() {
        jQuery(".fecharBtn").click(function(e){
            e.preventDefault();
            jQuery(".tb_modal").css("display", "none");
            jQuery("#overlay").remove();
        });
    }
</script>
<style type="text/css">
    *html .tb_modal {
        top:expression(eval(document.documentElement.scrollTop)) !important;
    }
</style>