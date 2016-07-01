<?php 
//include_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "includes/classes/AbaAjax.class.inc";
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';


if($_GET['tipoDiagnostico'] == 'arvore'){
	$tipoDiagnostico = 'arvore';
	$checkArvore = "checked=\"checked\"";	
} else {
	$tipoDiagnostico = 'listaAgrupada';
	$checkListaAgrupada = "checked=\"checked\"";
}

$obEntidadeParControle = new EntidadeParControle();

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);

monta_titulo( $nmEntidadePar, '<strong>PAC 2 - Programa de Aceleração do crescimento</strong>' );
?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
jQuery.noConflict();

jQuery(document).ready(function() {
	jQuery('.verificaVisualizacao').click(function() {
		if(jQuery('input[type=radio]:checked').val() == 'arvore'){
			//return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore');
		} else {
			return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada');
		}
	})
	
});

function popupSubacao(sbaid)
{
	url = 'par.php?modulo=principal/popupSubacao&acao=A&anoref='+<?php echo date('Y')+1 ?>+'&sbaid='+sbaid;
	window.open(url, 'popupSubacao', "height=600,width=800,scrollbars=yes,top=50,left=200" );
}

//-->
</script>
<?php 
$obEntidadeParControle->visualizacaoDiagnostico('listaAgrupada');
?>