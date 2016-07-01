<?php
cte_verificaSessao();
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&indid='. $_REQUEST['indid'] );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
if(cte_pegarEstUf($_SESSION['inuid'])) {
	$params = 'uf='.	cte_pegarEstUf($_SESSION['inuid']);
}
//elseif(cte_pegarMuncod($_SESSION['inuid'])) {
//	$params = 'cm='. cte_pegarMuncod($_SESSION['inuid']) .'&uf='.cte_pegarEstufDeMunicipio(cte_pegarMuncod($_SESSION['inuid']));
//}
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td>
			<!-- <IFRAME name=palco src="http://10.1.3.113/par/simec/cadplanejamento.php?<? echo $params; ?>" frameBorder=0 width=100% height=500 scrolling=auto></IFRAME> -->
			<IFRAME name=palco src="http://planejamentopar.mec.gov.br/simec/cadplanejamento.php?<? echo $params; ?>" frameBorder=0 width=100% height=500 scrolling=auto></IFRAME>
			
			</td>
		</tr>
	</table>


