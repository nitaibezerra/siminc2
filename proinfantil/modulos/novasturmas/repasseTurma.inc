<?php
header('Content-type: text/html; charset="iso-8859-1"',true);
 include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once "_funcoes_novasturmas.php";
$obNovasTurmas = new NovasTurmas( $_REQUEST );
$aryRepasse = $obNovasTurmas->carregaRepassePorTurma();

?>


<?php if(!empty($aryRepasse)){?>
<table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<thead>
		<tr class="subtituloDireita">
	    	<th colspan="5">Valor de Repasse para o Município</th>
	    </tr>				    
	    <tr class="subtituloDireita">
	    	<th style=" background-color:#7BA2B6;">Etapa</th>
	    	<th style=" background-color:#7BA2B6;">Quantidade de Alunos</th>
	    	<th style=" background-color:#7BA2B6;">Valor Unitário</th>
	    	<th style=" background-color:#7BA2B6;">Qtd de meses para Repasse dos recursos</th>
	    	<th style=" background-color:#7BA2B6;">Valor Total</th>
	    </tr>
    </thead>
    <tbody>
    	<?php $cor = "#F7F7F7"; $corlinha = "#E0F1F9";  ?>
    	<?php 
    	
    	foreach($aryRepasse as $repasse){?>
	    <tr bgcolor="<?php echo $cor; ?>" onmouseout="this.bgColor='<?php echo $cor; ?>';" onmouseover="this.bgColor='#ffffcc';" >
	        <td style=" background-color:#C7E0ED;"><?php echo $repasse['nome']; ?></td>
	        <td style=" background-color:<?php echo $corlinha ?>;"><?php echo $repasse['total_alunos']; ?></td>
	        <td style=" background-color:<?php echo $corlinha ?>;"><?php echo $repasse['vaavalor']; ?></td>
	        <td style=" background-color:<?php echo $corlinha ?>;"><?php echo $repasse['periodo']; ?></td>
	        <td style=" background-color:<?php echo $corlinha ?>;"><?php echo $repasse['valor_total'];?></td>
	    </tr>	
	    <?php 
	    $corlinha = "#E0F1F9" ? "#F7F7F7" : "#E0F1F9";
	    
	    $totalGeral = str_replace(".","", $repasse['valor_total']);
		$totalGeral = str_replace(",",".", $totalGeral);
	    
	    $alunos_geral += $repasse['total_alunos'];
	    $valor_geral += $totalGeral; 
	    $status_turma = $repasse['anatipo'];
	    ?>
	    <?php $cor = "#F7F7F7" ? "#FFFFFF" : "#F7F7F7"; ?>
	    <?php } ?>
	    <tr>
	        <td style=" background-color:#C7E0ED;">Total Geral</td>
       		<td style=" background-color:#E0F1F9;"><?php echo $alunos_geral; ?></td>
        	<td colspan="2" style=" background-color:#E0F1F9;"></td>
        	<td style=" background-color:#E0F1F9;"><?php echo $valor_geral ? number_format($valor_geral,2,",",".") : '0,00';?></td>
    	</tr>
    </tbody>			    
</table>

<?php 
switch($status_turma){
	case "I": ?>
	
	<table class="tabela" border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloCentro" style=" background-color:#C7E0ED;" > Esta turma não foi aprovada pelo Analista Técnico. </td>
		</tr>
	</table>
	<?php break; 
	case "A": ?>
	
	<table class="tabela" border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloCentro" style=" background-color:#C7E0ED;" > Esta turma foi aprovada pelo Técnico. </td>
	 	</tr>
	</table>
	<?php break; 
	case "D": ?>
	
	<table class="tabela" border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloCentro" style=" background-color:#50A56C;"> Esta turma está em diligência. </td>
	 	</tr>
	</table>
	<?php break; 
	case "": ?>
	
	<table class="tabela" border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloCentro" > Esta turma não foi analisada até o momento. <br> Necessário fazer a análise técnica da turma. </td>
		</tr>
	</table>
	<?php break; 
} } ?>

<br>