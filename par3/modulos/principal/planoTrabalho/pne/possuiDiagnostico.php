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
$controllerPne   = new Par3_Controller_Pne();
$controleUnidade = new Par3_Controller_InstrumentoUnidade();

$inuid       = $_REQUEST['inuid'];
$itrid       = $controleUnidade->pegarItrid($inuid);
$esfera      = $controleUnidade->pegarDescricaoEsfera($inuid);
$situacaoPne = $controllerPne->situacaoPne($inuid);

$inuid = $_REQUEST['inuid'];

switch ($_REQUEST['req']) {
	case 'salvar':
	    break;
	default:
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="req" value="salvar"/>

    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Diagnostico</h3>
    	</div>
    	<div class="ibox-content">
    		A situação atual do PNE do <?php echo $esfera;?> é <?php echo $situacaoPne; ?>
    	</div>
    </div>
</form>