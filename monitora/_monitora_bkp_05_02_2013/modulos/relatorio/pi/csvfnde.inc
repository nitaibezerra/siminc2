<?php

switch($_REQUEST['buscar']) {
	case '1':
		include "csvfnde_resultado.inc";
		exit;
	case '2':
		if($_POST['filano'])
			$_SESSION['rehuf_var']['filano']=$_POST['filano'];
		if($_POST['entid'])
			$_SESSION['rehuf_var']['hospitalentid']=$_POST['entid'];
			
		if($_REQUEST['agrupador'] && !$_REQUEST['gitid']) {
			echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
			
			foreach($_REQUEST['agrupador'] as $tblid) {
				// Pegando dados da TABELA (pai de toda a estrutura)
				$tabela = $db->pegaLinha("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid ='".$tblid."'");
				echo "<table class=\"tabela\" align=\"center\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\">";
				echo "<tr><td class='SubTituloCentro' colspan='2'>".$tabela['tabtdsc']."</td></tr>";
				$grupoitem = (array) $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ORDER BY gitordem");
				foreach($grupoitem as $grp) {
					echo "<tr><td class='SubTituloDireita' width='50%'>".$grp['gitdsc']."</td><td><form action='' method='post' name='formulario'><input type='hidden' name='buscar' value='2'><input type='hidden' name='agrupador[]' value='".$tblid."'><input type='hidden' name='gitid' value='".$grp['gitid']."'><input type='submit' value='Clique aqui'></form></td></tr>";
				}
				echo "</table>";

			}
			exit;			
		}
		include "relatoriogeralexcel_resultado.inc";
		exit;
	case '3':
		include "relatoriogeralagrupado_resultado.inc";
		exit;
		
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "PPA - Monitoramento e Avaliação", "Relatório CSV" );

$_SESSION['rehuf_var']['filano']=array();

?>
<script type="text/javascript">
function exibirRelatorio() {
	// submete formulario
	formulario.target = 'relatorioppamonitora';
	var janela = window.open( '', 'relatorioppamonitora', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
    <tr>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
	</tr>
</table>