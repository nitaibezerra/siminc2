<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 2', '');
echo cabecalhoPDE();

if(isset($_SESSION["pdeid"])) {
	$pdeid = $_SESSION["pdeid"];
}
else {
	echo "<script language=\javascript\" type=\"text/javascript\">
			alert('Nenhum dado para este ano Exercício Base de Comparação');
			window.history.go(-1);
		  </script>";
	exit;	 
}
	if( $uf == 'PR'){
		$descricao 		= "acedescricaoPR";
		$descricaoJoin  = "ace.acedescricaoPR";
		$descricaoJoin2 = "ace2.acedescricaoPR";
		$descricaoJoin3 = "ace3.acedescricaoPR";
	}
	else
	{
		$descricao 		= "acedescricao";
		$descricaoJoin  = "ace.acedescricao";
		$descricaoJoin2 = "ace2.acedescricao";
		$descricaoJoin3 = "ace3.acedescricao";
	}	
$criterios = $db->carregar("SELECT 
								aceid, $descricao
							FROM 
								pdeescola.analisecriterioeficacia 
							WHERE 
								aceidpai is null AND 
								aceseq is null AND 
								aceano = ".ANO_EXERCICIO_PDE_ESCOLA."
							ORDER BY
								acecodigo ASC");

$num = count($criterios);

$cDado = array("instrumento" => 2);
$cForm = new formulario($cDado);
$cForm->direcPag();

?>

<br />
<?=subCabecalho('Ficha-resumo 2 - Total de Pontos'); ?>	
<form action="" method="post">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<thead>
	<tr>
		<th rowspan="2" width="20%">PONTUAÇÃO</th>
		<th colspan="<?=($num + 1)?>" width="80%">CRITÉRIOS</th>
	</tr>
	<tr>
		<?php
			$width = floor(80 / ($num + 1));
			
			$total_max = array();
			$total_soma = array();
			$porcentagem = array();
			
			
			if($_SESSION["exercicio"] != $_SESSION['exercicio_atual']) {
							echo "<script language=\javascript\" type=\"text/javascript\">
									alert('Erro no envio dos parâmetros necessários.');
									window.history.go(-1);
		 						</script>";
							exit;	
			}
			for($i=0; $i<count($criterios); $i++) {
				echo "<th width=\"".$width."%\">".$criterios[$i]["acedescricao"]."</th>";

				$resultado = $db->carregar("SELECT
												count(ace2.aceid) as total, sum(esc.esavalor) as soma
											FROM
												pdeescola.analisecriterioeficacia ace
											INNER JOIN
												pdeescola.analisecriterioeficacia ace2 ON ace2.aceidpai = ace.aceid
											LEFT JOIN
												pdeescola.detalheanalisecriterioeficacia dac ON dac.aceid = ace2.aceid 
																							 AND dac.pdeid = ".$pdeid."
											LEFT JOIN 
												pdeescola.escalaace esc ON esc.esaid = dac.esaid
											WHERE
												ace.aceidpai = ".$criterios[$i]["aceid"]." AND ace.aceano = ".ANO_EXERCICIO_PDE_ESCOLA);
				
				$total_max[$i] = ($resultado[0]["total"] == NULL) ? 0 : ($resultado[0]["total"] * 5);
				$total_soma[$i] = ($resultado[0]["soma"] ==  NULL) ? 0 : (int)$resultado[0]["soma"];
				
				$porcentagem[$i] = (($total_soma[$i] / $total_max[$i]) * 100);
				
				$porcentagem[$i] = is_float($porcentagem[$i]) ? number_format($porcentagem[$i], 0) : $porcentagem[$i];
				$porcentagem[$i] = is_string($porcentagem[$i]) ? str_replace('.', ',', $porcentagem[$i]) : $porcentagem[$i];
			}
			
		?>
		<th width="10%">Total</th>
	</tr>
</thead>
<tbody>
	<tr height="50px" align="center">
		<td bgcolor="#f0f0f0"><b>(A) Total máximo de pontos no critério</b></td>
		<?php
			$cont_max = 0;
			for($i=0; $i<count($total_max); $i++) {				
				echo "<td align=\"center\" bgcolor=\"#f0f0f0\">".$total_max[$i]."</td>";
				$cont_max += $total_max[$i];
			}
		?>
		<td align="center" bgcolor="#f0f0f0"><?=$cont_max?></td>
	</tr>
	<tr height="50px" align="center">
		<td bgcolor="#f0f0f0"><b>(B) Total de pontos no critério</b></td>
		<?php
			$cont_soma = 0;
			for($i=0; $i<count($total_soma); $i++) {				
				echo "<td align=\"center\" bgcolor=\"#f0f0f0\">".$total_soma[$i]."</td>";
				$cont_soma += $total_soma[$i];
			}
		?>		
		<td align="center" bgcolor="#f0f0f0"><?=$cont_soma?></td>
	</tr>
	<tr height="50px" align="center">
		<td bgcolor="#f0f0f0"><b>(B) / (A) x 100</b></td>
		<?php
			for($i=0; $i<count($porcentagem); $i++) {
				echo "<td align=\"center\" bgcolor=\"#f0f0f0\">".$porcentagem[$i]."%</td>";
			}
			
			$cont_porcentagem = (($cont_soma / $cont_max) * 100);
			
			$cont_porcentagem = is_float($cont_porcentagem) ? number_format($cont_porcentagem, 0) : $cont_porcentagem;
			$cont_porcentagem = is_string($cont_porcentagem) ? str_replace('.', ',', $cont_porcentagem) : $cont_porcentagem;
		?>
		<td align="center" bgcolor="#f0f0f0"><?=$cont_porcentagem?>%</td>
	</tr>
</tbody>
<tfoot>
	<? echo $cForm->montarbuttons(); ?>
</tfoot>
</table>
</form>