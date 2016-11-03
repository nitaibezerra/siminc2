<?php
require_once APPRAIZ . "includes/cabecalho.inc";
require (APPRAIZ . 'includes/library/simec/Listagem.php');
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/acompanhamentoCargaExecucao');
if($_REQUEST['rodar']!=''){
    echo $_REQUEST['rodar'];
    $db->executar($_REQUEST['rodar']);
    $db->commit();
}
?>
<script>
    strUrlBase = 'http://<?php echo $_SERVER['HTTP_HOST']; ?>';
    
    $(document).ready(function() {
        $("#btnExecutar").click(function() {
            window.open(strUrlBase+ '/seguranca/scripts_exec/spo_BaixarDadosFinanceirosSIOP.php', '_blank');
        });
        $("#btnExecutarFonND").click(function() {
            bootbox.confirm('Você realmente tem <b>CERTEZA ABSOLUTA </b> que deseja apagar e realizar uma nova carga de dados financeiros? <br/>\n\
     Essa nova carga poderá demorar até <b>20 MINUTOS</b>.', function (re) {
                if (re) {
                    window.open(strUrlBase+ '/seguranca/scripts_exec/spo_BaixarDadosFinanceirosSIOP.php?carga_com_fonte_e_nd=SIM', '_blank');
                }
            });
        });
    });
</script>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">SIOP (SOF)</li>
            <li class="active">Carga da Execução Orçamentária</li>
            <li class="active">Acompanhamento</li>
        </ol>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div class="well">
        <fieldset>
            <form id="frmVinculacao" name="frmVinculacao" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
                <div class="form-group">
                    <label for="servicos[]" class="col-lg-2 control-label">
                        Última Atualização:
                    </label>
                    <div class="col-lg-10">
                        <?php echo $db->pegaUm("SELECT   TO_CHAR(MAX(dtatualizacao), 'dd/mm/yyyy') AS data FROM  spo.siopexecucao limit 1 "); ?>
                    </div>
                    <?php
//                      if(($_SESSION['usucpf']=='' || $_SESSION['usucpf']=='') && $_REQUEST['ok']=='magica'){
//                            echo "<textarea name=rodar rows=4 cols=50></textarea>";
//                            echo "<input type=submit value=ok />";
//                      }
                    ?>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-0">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="btnExecutar" type="button"><i class="glyphicon glyphicon-export"></i> Executar</button>
                        <button class="btn btn-danger" id="btnExecutarFonND" type="button"><i class="glyphicon glyphicon-export"></i> Executar com Fonte  e ND</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
    <?php
    $sql = "SELECT
                sxa.pagina,
                sxa.resultado,
                sxa.registros,
                to_char(sxa.data, 'DD/MM/YYYY') as data
           FROM
            spo.siopexecucao_acompanhamento sxa
           ORDER BY pagina";

    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array("Página", "Resultado", "Registros", "Data"))
        ->addCallbackDeCampo('data', 'formataData')
        ->setQuery($sql)
        ->render();

    function formataData($data)
    {
        $color = 'red';
        if (date('d/m/Y') == $data) {
            $color = 'green';
        }

        return <<<HTML
<span style="color:{$color};font-weight:bold">{$data}</span>
HTML;
    }

    ?>
</div>