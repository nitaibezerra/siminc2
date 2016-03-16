<?php

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function setarInuId(estuf) {
	window.location='snf.php?modulo=principal/diagnosticoPar&acao=A&requisicao=setarInuIdPorEstuf&estuf='+estuf+'&goto=sintese';
}
</script>
<?

/* montando o menu */
echo montarAbasArray(montaMenuAtendimentoForum(), $_SERVER['REQUEST_URI']);
/* fim montando o menu */

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<th class="tdDegradde01" colspan="2" align="left">
			<h1 class="notprint" colspan="6">
				Sintese: <?=$db->pegaUm("select estdescricao||' - '||estuf from territorios.estado where estuf='".$_SESSION['snf']['estuf']."'"); ?>
		<? if($db->testa_superuser()) : 
		
			$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado ORDER BY estdescricao"; 
			$db->monta_combo('estuf', $sql, 'S', 'Selecione o Estado', 'setarInuId', '', '', '200', 'N', 'estuf', '', $_SESSION['snf']['estuf']); 
			
			endif; ?>
			</h1>
		</th>
	</thead>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#dddddd" >
		<td width="10%"><b>Área Temática</b></td>
		<td><b>Nome do Curso</b></td>
		<td><b>Vagas Demandadas</b></td>
		<td><b>Pré-Oferta</b></td>
		<td><b>Vagas Atendidas</b></td>
		<td><b>IES</b></td>
	<tr>
<?php 

$sql = "SELECT DISTINCT
			ate.ateid,
			ate.atedesc,
			cur.curid,
			cur.curdesc||' - '||mod.moddesc||' - '||ncu.ncudesc as curso,
			coalesce(sum(privagassolicitadas),0) as vagas_demanda,
			coalesce(snf.retorna_preoferta(cur.curid, mod.modid, 2012, 'AC'),'0') as preoferta,
			coalesce(snf.retorna_vagas_atendidas(cur.curid, mod.modid, 2012, 'AC'),'0') as atendidas,
			coalesce(snf.retorna_ies(cur.curid, mod.modid, 2012, 'AC'),'0') as ies
		FROM
			catalogocurso.areatematica ate
		INNER JOIN catalogocurso.curso cur ON cur.ateid = ate.ateid AND curstatus = 'A'
		INNER JOIN catalogocurso.modalidadecurso mod ON mod.modid = cur.modid
		INNER JOIN catalogocurso.nivelcurso ncu ON ncu.ncuid = cur.ncuid
		--vagas_demanda
		LEFT  JOIN snf.prioridadecursoescola pdf ON pristatus = 'A' 
							    AND pdf.estuf = 'AC' 
							    AND pdf.ateid = ate.ateid
							    AND pdf.curid = cur.curid
							    AND pdf.ncuid = cur.ncuid
							    AND pdf.modid = cur.modid
							    AND pdf.pcfano = 2012
		WHERE
			atestatus = 'A'
		GROUP BY
			ate.ateid,
			ate.atedesc,
			cur.curid,
			cur.curdesc,
			mod.moddesc,
			ncu.ncudesc,
			mod.modid
		ORDER BY
			2,4";
$dados = $db->carregar($sql);
$ateid = '';
$i = 0;
foreach( $dados as $dado ){
	if( $ateid != $dado['ateid'] ){
		$sql = "SELECT DISTINCT
					count(DISTINCT cur.curid) as qtd
				FROM
					catalogocurso.areatematica ate
				INNER JOIN catalogocurso.curso cur ON cur.ateid = ate.ateid AND curstatus = 'A'
				INNER JOIN catalogocurso.modalidadecurso mod ON mod.modid = cur.modid
				INNER JOIN catalogocurso.nivelcurso ncu ON ncu.ncuid = cur.ncuid
				WHERE
					ate.ateid = ".$dado['ateid']."
					AND atestatus = 'A' ";
		$rowspan = $db->pegaUm($sql);
		$i++;
		$ateid = $dado['ateid'];
		$atedesc = $dado['atedesc'];
		$atedesc = '<td rowspan="'.$rowspan.'">'.$atedesc.'</td>';
	}else{
		$rowspan = '';
		$atedesc = '';
	}
	$cor = $i%2==0 ? 'white' : ''; 
?>
	<tr bgcolor="<?=$cor ?>">
		<?=$atedesc ?>
		<td><?=$dado['curso'] ?></td>
		<td><?=$dado['vagas_demanda'] ?></td>
		<td><?=$dado['preoferta'] ?></td>
		<td><?=$dado['atendidas'] ?></td>
		<td><?=$dado['ies'] ?></td>
	</tr>
<?php 
}
?>
</table>

