<?php
header('Content-type: text/html; charset="iso-8859-1"',true);
include  "_funcoes_novasturmas.php";
//Recupera Turmas do Município
$sql = "SELECT
				    tre.tirdescricao, 
				    tur.ttuid,
				    CASE WHEN tur.ttuid = 1 THEN 'Exclusivo Creche'
						 WHEN tur.ttuid = 2 THEN 'Exclusivo Pré-escola'
						 WHEN tur.ttuid = 3 THEN 'Creche e Pré-escola (Misto)'
				    END AS tipodependencia,  
				    CASE WHEN tur.turtipoestabelecimento = 1 THEN 'Municipal ou Distrital'
						 WHEN tur.turtipoestabelecimento = 3 THEN 'Conveniado (Comunitário, confessional ou filantrópico)'
				    END AS tipoestabelecimento,  	
				    tur.turdsc,
				    tur.turid,
					tur.turnomeescola
		FROM 		proinfantil.turma tur
		INNER JOIN 	proinfantil.tiporede tre ON tre.tirid = tur.tirid
		LEFT JOIN   entidade.entidade ent ON trim(ent.entcodent) = trim(tur.entcodent)
		WHERE muncod = '{$_SESSION['proinfantil']['muncod']}'
		AND turstatus = 'A'
		AND turid = {$_REQUEST['turma']}";

$rs_turma = $db->pegaLinha($sql); ?>
<script	type="text/javascript" src="/includes/estouvivo.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />

<?php 
$sql_repasse = "SELECT 			ana.anatipo,(CASE WHEN mds.timid = 1 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.titid = 5 THEN 'Integral' ELSE 'Parcial' END) AS nome,
								mds.titid, mds.timid, ala.total_alunos, vr.vrtvalor, tur.periodo_repasse,
								(ala.total_alunos*tur.periodo_repasse*vr.vrtvalor)/12 AS valor_total
				FROM 			proinfantil.mdsalunoatendidopbf mds
				INNER JOIN 		(SELECT alaid, sum(alaquantidade) AS total_alunos FROM proinfantil.mdsalunoatendidopbf GROUP BY alaid) as ala on ala.alaid = mds.alaid
				INNER JOIN 		(SELECT turid, muncod, turstatus, ttuid, turdtinicio, tirid, (select proinfantil.calculorepasse2012(turdtinicio)) AS periodo_repasse		
				                FROM proinfantil.turma) AS tur ON tur.turid = mds.turid
				                
				INNER JOIN ( SELECT vrtvalor, ttuid, tirid, tatid  
			                 FROM proinfantil.valorreferencianovasturmas where vrtano = '{$_SESSION['exercicio']}' and vrtstatus = 'A' ) 
			                 AS vr on vr.ttuid = tur.ttuid
			                 and vr.tirid = tur.tirid 
			                 and ( case when (mds.titid = 5) then  vr.tatid = 1 else vr.tatid = 2 end )
				LEFT JOIN 		proinfantil.analisenovasturmasaprovacao ana ON ana.turid = tur.turid
				WHERE			muncod = '{$_SESSION['proinfantil']['muncod']}'
				AND				tur.turstatus = 'A'
				AND 			tur.turid = {$rs_turma['turid']}
				AND 			tur.ttuid = {$rs_turma['ttuid']}
				GROUP BY  		ana.anatipo,mds.titid, mds.timid, tur.turdtinicio, vr.vrtvalor, tur.periodo_repasse, ala.total_alunos
				ORDER BY  		mds.timid DESC, mds.titid";

$aryRepasse = $db->carregar($sql_repasse);	

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
	    	<th style=" background-color:#7BA2B6;">Período de Repasse (meses)</th>
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
	        <td style=" background-color:<?php echo $corlinha ?>;"><?php echo $repasse['vrtvalor'] ? number_format($repasse['vrtvalor'],2,",",".") : '0,00'; ?></td>
	        <td style=" background-color:<?php echo $corlinha ?>;"><?php echo $repasse['periodo_repasse']; ?>/12</td>
	        <td style=" background-color:<?php echo $corlinha ?>;"><?php echo $repasse['valor_total'] ? number_format($repasse['valor_total'],2,",",".") : '0,00';?></td>
	    </tr>	
	    <?php 
	    $corlinha = "#E0F1F9" ? "#F7F7F7" : "#E0F1F9";
	    $alunos_geral += $repasse['total_alunos'];
	    $valor_geral += $repasse['valor_total']; 
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