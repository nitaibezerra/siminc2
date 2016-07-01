<?php
if($_REQUEST['requisicaoAjax']){
	header( 'Content-type: text/html; charset=iso-8859-1' );
	$arrValor = explode(",",$_REQUEST['valor']);
	$arrSituacao = explode(",",$_REQUEST['situacao']);
	$arrCor = explode(",",$_REQUEST['cor']);
	$total_geral = number_format( array_sum($arrValor) , 0, '.', ',' );
	?>
	<b>Total: <?php echo $total_geral ?> Ações</b></br>
	<?php
	
	foreach($arrSituacao as $n => $situacao){?>
		<?php echo number_format( $arrValor[$n] , 0, '.', ',' )." Ações - ".$situacao ?><br/>
		<?php $tamanho_cor = round( ($arrValor[$n]/$total_geral)*100 , 2 ); ?>
		<div style="height:10px;width:<?=$tamanho_cor?>px;border:1px solid black;background-color:<?php echo $arrCor[$n] ?>" ></div>
	<?php }
	exit;
}


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
			<img src="../../imagens/cor1.gif" style="height:7;width:<?= $intVerdePercent ?>;border:1px solid #888888;"> Preenchido e Liberado (<?= $intVerdePercent ?>%)
		</td>
	</tr>
	<tr>
		<!--  amarelo -->
		<td><?= $intAmarelo ?></td>
		<td>
			<img src="../../imagens/cor2.gif" style="height:7;width:<?= $intAmareloPercent ?>;border:1px solid #888888;"> Preenchido e não Liberado (<?= $intAmareloPercent ?>%)
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
			<img src="../../imagens/cor0.gif" style="height:7;width:<?= $intBrancoPercent ?>;border:1px solid #888888;"> Não Preenchido (<?= $intBrancoPercent ?>%)
		</td>
	</tr>
</table>