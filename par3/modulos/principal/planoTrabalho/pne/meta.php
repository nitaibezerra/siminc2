<?php
/**
 * Tela de dados da prefeitura
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
require_once APPRAIZ.'sase/classes/Metas.class.inc';

global $simec;

$inuid = $_REQUEST['inuid'];
$menu  = $_REQUEST['menu'];
$metid = $_REQUEST['menu'];

$controllerPne   = new Par3_Controller_Pne();
$modelSasePne    = new Sase_Model_Pne();
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
$metas           = new Metas($metid);

$itrid       = $modelUnidade->itrid;
$esfera      = $controleUnidade->pegarDescricaoEsfera($inuid);
$situacaoPne = $controllerPne->situacaoPne($inuid);
$arrAnosPne = Array(
    '2015'=>'2015',
    '2016'=>'2016',
    '2017'=>'2017',
    '2018'=>'2018',
    '2019'=>'2019',
    '2020'=>'2020',
    '2021'=>'2021',
    '2022'=>'2022',
    '2023'=>'2023',
    '2024'=>'2024',
    '2025'=>'2025',
);


switch ($_REQUEST['req']) {
	case 'salvar':
	    break;
	case 'carregarValorPne':
	    ob_clean();
	    $modelSasePne = $modelSasePne->carregarPneUnidade($_REQUEST['campo'], $_REQUEST['valor'], $_REQUEST['ano'], $_REQUEST['subid']);
	    echo $modelSasePne->pnevalor;
	    die();
	    break;
	default:
	    if ($itrid === '2') {
            $campo = 'muncod';
            $valor = $modelUnidade->muncod;
	    } else {
            $campo = 'estuf';
            $valor = $modelUnidade->estuf;
	    }
	    $ano = date('Y');
	    $arrSubmetas = $metas->retornarArraySubmetas();
	    break;
}
?>
<style>
.irs-single{
    background: #2ac3a4;
}
.irs-bar{
    border-top: 1px solid #0aa384;
    border-bottom: 1px solid #0aa384;
    background: #2ac3a4;
    background: linear-gradient(to top, #0aa384 0%,#2ac3a4 100%);
}
.irs-bar-edge{
    border-top: 1px solid #0aa384;
    border-bottom: 1px solid #0aa384;
    background: #2ac3a4;
    background: linear-gradient(to top, #0aa384 0%,#2ac3a4 100%);
}
</style>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="req" value="salvar"/>

    <div class="ibox">
    	<div class="ibox-title">
    	    <h3><?php echo $metas->metchamada ?></h3>
    	</div>
    	<div class="ibox-content">
    		<?php echo $metas->mettitulo ?>
    	</div>
    </div>

	<div class="row">
        <div class="col-md-3">
        	<div class="ibox-title">
        	    Verifique a sua situação em comparações com outras regiões
        	</div>
        	<div class="ibox-content">
    		  <?php echo $controllerPne->camposComparacaoMetasPNE($_POST); ?>
        	</div>
        </div>
        <div class="col-md-9">
    	    <?php //ver($arrSubmetas);?>
            <ul class="nav nav-tabs">
    	    <?php foreach ($arrSubmetas as $k => $submeta) {?>
                <li role="presentation" class="<?php echo ($k === 0) ? 'active' : ''; ?>">
                    <a subid="<?php echo $submeta['subid']?>" class="abaIndicador" >
                        Indicador <?php echo $submeta['subordem']?>
                    </a>
                </li>
    	    <?php }?>
            </ul>
    	    <?php
    	    foreach ($arrSubmetas as $k => $submeta) {

                $modelSasePne = $modelSasePne->carregarPneUnidade($campo, $valor, $ano, $submeta['subid']);
            ?>
    	    <div class="row indicador<?php echo $submeta['subid']?>"
    	         <?php echo ($k === 0) ? '' : 'style="display:none"'; ?>>
                <div class="col-md-12">
                    <div class="row" style="height: 220px;">
                        <div class="col-md-8">
                        	<div class="ibox-title">
                        	<h4><?php echo $submeta['subtitulo']?></h4>
                        	</div>
                        	<div class="ibox-content">
                                <h5>Meta <?php echo $esfera; ?>:</h5>
                            	<input type="text" id="meta_<?php echo $submeta['subid']?>"
                            	       name="meta[<?php echo $submeta['subid']?>]"
                            	       value="<?php echo $modelSasePne->pnevalor?>" />
                        	</div>
                        	<div class="ibox-content">
                                <h5>Ano previsto:</h5>
                                <?php
                                echo $simec->select('pneano['.$submeta['subid'].']', '', $ano, $arrAnosPne, $arrAttr);
                                ?>
                        	</div>
                        </div>
                        <div class="col-md-4" style="height: 100%">
                        	<div class="ibox-title">
                        	<h4>Meta <?php echo $controleUnidade->pegarNomeEntidade($inuid);?>:</h4>
                        	</div>
                        	<div class="ibox-content" style="height: 100%;">
                                <h6>Situação:</h6>
                                <?php
                                $cor = '#236B8E';
                                $descricao = 'Teste';
                                $mundescricao = 'UF';
                                $metaTotal = 100;
                                $metaBrasil = 40;
                                $dadosGrafico = array(
                                    'cor' => $cor,
                                    'descricao' => str_replace("'", '', $descricao),
                                    'valor' => '30',
                                    'metaTotal' => $metaTotal,
                                    'metaBrasil' => $metaBrasil,
                                    'tipo' => 'P',
                                    'plotBandsCor' => '#f7b850',
                                    'plotBandsOuterRadius' => '115%',
                                    'title' => "Meta {$mundescricao}:",
                                    'anoprevisto' => $anoprevisto
                                );
                                $controllerPne->geraGraficoPNE('teste', $dadosGrafico);
                                ?>
                        	</div>
                        </div>
                    </div>
                    <br>
                    <div class="row" style="margin-top:50px;">
                        <div class="col-md-12">
                            <ul class="nav nav-tabs">
                                <li role="presentation" class="active">
                                    <a><span class="glyphicon glyphicon-signal"></span></a>
                                </li>
                            </ul>
                        </div>
                    </div>
                </div>
    	    </div>
    	    <?php }?>
        </div>
	</div>
</form>
<script>
$(document).ready(function()
{
    $('.abaIndicador').click(function()
    {
        var subid = $(this).attr('subid');

        $('[class*="row indicador"]').hide();
        $('.indicador'+subid).show();

        $('.abaIndicador').parent().removeClass('active');
        $(this).parent().addClass('active');

        $("#meta_"+subid).ionRangeSlider({
        	min: 0,
        	step: 0.1
        });
    });

    $("#meta_<?php echo $arrSubmetas[0]['subid']?>").ionRangeSlider({
    	min: 0,
    	step: 0.1
    });

    $('[name*="pneano["]').change(function()
    {
        var campo = '<?php echo $campo ?>';
        var valor = '<?php echo $valor ?>';
        var ano   = $(this).val();
        var subid = $(this).attr('id').split('-');

        subid = subid[1];

        $.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: '&req=carregarValorPne&campo='+campo+'&valor='+valor+'&ano='+ano+'&subid='+subid,
       		async: false,
       		success: function(resp){
       			$("#meta_"+subid).data("ionRangeSlider").update({from: resp});
       		}
     	});

    });

});
</script>