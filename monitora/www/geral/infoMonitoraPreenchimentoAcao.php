<?php



header( 'Content-type: text/html; charset=iso-8859-1' );

// captura entrada
$intVerde		= $_REQUEST['verde'];
$intAmarelo		= $_REQUEST['amarelo'];
$intBranco		= $_REQUEST['branco'];
$intVermelho	= $_REQUEST['vermelho'];
$strMes			= $_REQUEST['mes'];
$strTitle		= $_REQUEST['title'];
$intTotal		= $intVerde + $intAmarelo + $intBranco + $intVermelho;

if( $strTitle == '' )
{
	$strTitle = 'Preenchidas';
}
$intVerdePercent	= number_format( $intVerde		* 100	/ $intTotal, 0, '.', ',' );
$intAmareloPercent	= number_format( $intAmarelo	* 100	/ $intTotal, 0, '.', ',' );
$intBrancoPercent	= number_format( $intBranco		* 100	/ $intTotal, 0, '.', ',' );
$intVermelhoPercent	= number_format( $intVermelho	* 100	/ $intTotal, 0, '.', ',' );

?>
<b><?= $strMes ?></b> - Total: <?= $intTotal ?> Ações<br/>
<?= $intVerde + $intAmarelo ?> <?= $strTitle ?> <?= $intVerdePercent + $intAmareloPercent ?>%<br/>
<table border="0">
	<tr>
		<!--  verde -->
		<td><?= $intVerde ?></td>
		<td>
			<img src="../../imagens/cor1.gif" style="height:7;width:<?= $intVerdePercent ?>;border:1px solid #888888;"> Em Cadastramento (<?= $intVerdePercent ?>%)
		</td>
	</tr>
	<tr>
		<!--  amarelo -->
		<td><?= $intAmarelo ?></td>
		<td>
			<img src="../../imagens/cor2.gif" style="height:7;width:<?= $intAmareloPercent ?>;border:1px solid #888888;"> Em Validação (<?= $intAmareloPercent ?>%)
		</td>
	</tr>
	<? 
		/*
	<tr>
		<!--  vermelho -->
		<td><?= $intVermelhoPercent ?></td>
		<td>
			<img src="../../imagens/cor3.gif" style="height:7;width:<?= $intVermelhoPercent ?>;border:1px solid #888888;"> Não Preenchido (<?= $intVermelhoPercent ?>%)
		</td>
	</tr>
		*/
	?>
	<tr>
		<!-- branco -->
		<td><?= $intBranco ?></td>
		<td>
			<img src="../../imagens/cor0.gif" style="height:7;width:<?= $intBrancoPercent ?>;border:1px solid #888888;"> Finalizada (<?= $intBrancoPercent ?>%)
		</td>
	</tr>
</table>