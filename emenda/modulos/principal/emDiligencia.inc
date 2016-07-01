<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

verificaPermissao();
validaSessionPTA( $_SESSION['emenda']['federal'] );

if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	!$_REQUEST['popup'] ? include APPRAIZ . 'includes/cabecalho.inc' : '';
	print "<br/>";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
exibeInstituicaoBenefiada();
$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

!$_REQUEST['popup'] ? print "<br/>" : "";
!$_REQUEST['popup'] ? montaAbasPTA( $abacod_tela, $url, '', $estadoAtual ) : '';
!$_REQUEST['popup'] ? monta_titulo( 'Em diligência', '') : '';

echo !$_REQUEST['popup'] ? cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]) : '';
echo "<br />";

if($_REQUEST['popup']){
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
}

$ptrid = ($_SESSION["emenda"]["ptrid"]) ? $_SESSION["emenda"]["ptrid"] : $_REQUEST["ptrid"];
$ptrid = !$ptrid ? $_SESSION["emenda"]["ptridAnalise"] : $ptrid;
validaSessionPTA( $ptrid );

$sql = "SELECT 
			anasituacaoparecer, 
			anaparecer, 
			anadisponibilizardiligencia 
		FROM emenda.analise 
		WHERE 
			ptrid = $ptrid 
			and anatipo = 'T'
			and anasituacaoparecer = 'E'";
$arAnalise = $db->carregar( $sql );
$arAnalise = $arAnalise ? $arAnalise : array();

?>
<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
 <tbody>
	<tr>
		<td colspan="2" width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
			PARECER TÉCNICO DO FNDE - Em Diligência
		</td>
	</tr>
	<?
	foreach ($arAnalise as $key => $v): ?>
		<tr>
			<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
				<b>Análise Técnica do FNDE:</b>
			</td>
			<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
			<?
				echo $v['anaparecer'];
			?>
			</td>
		</tr>
	<?endforeach;?>
</tbody>
</table>