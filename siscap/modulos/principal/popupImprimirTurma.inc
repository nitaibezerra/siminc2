<?php
$_SESSION['siscap']['turid'] = !empty($_SESSION['siscap']['turid']) ? $_SESSION['siscap']['turid'] : $_REQUEST['turid'];
if( empty($_SESSION['siscap']['turid']) ){
	echo "<script>
			alert('Falta dados na sessão!');
			window.close();
		  </script>";
	die;
}

$turid = $_SESSION['siscap']['turid'];

monta_titulo( 'Informações sobre a Turma', '' );

$sql = "SELECT cu.curid, a.aredsc, cu.curdsc, cu.curobj, cu.curcont, cu.curinfo,
			to_char(tu.turdtini, 'DD/MM/YYYY')||' a '||to_char(tu.turdtfim, 'DD/MM/YYYY') as periodo,
		    tu.turcarga, tu.turhrini as horarioinicio,tu.turhrfim as horariofim,
		    case when tu.turperiodo = 'MA' then 'Matutino'
		    	when tu.turperiodo = 'VE' then 'Vespertino'
		        when tu.turperiodo = 'IN' then 'Integral'
		        when tu.turperiodo = 'NO' then 'Noturno'
		    end as turperiodo,
		    case when tu.turstatus = 'EA' then 'Em andamento'
		    	when tu.turstatus = 'IA' then 'Inscrições Abertas'
		        when tu.turstatus = 'IE' then 'Inscrições Encerradas'
		        when tu.turstatus = 'CO' then 'Concluído'
		        when tu.turstatus = 'AD' then 'Adiado'
		        when tu.turstatus = 'CA' then 'Cancelado' end as turstatus, tu.turturma,
		   tu.turpublicoalvo, tu.turinfo, tu.turid
		FROM
			siscap.curso cu
		    inner join siscap.turma tu on tu.curid = cu.curid
		    inner join siscap.area a on a.areid = cu.areid
		WHERE tu.turid = ".$turid;

$arCurso = $db->pegaLinha( $sql );
$arCurso = $arCurso ? $arCurso : array();

$arHoraI = explode( ':', $arCurso['horarioinicio'] );
$horarioFormatadoI = $arHoraI[0].'h'.$arHoraI[1];
$arHoraF = explode( ':', $arCurso['horariofim'] );
$horarioFormatadoF = $arHoraF[0].'h'.$arHoraF[1];

$horarioFormatado = $horarioFormatadoI . ' às ' . $horarioFormatadoF;
?>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }
	@media screen {.notscreen { display: none; }

	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}

</style>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<th colspan="2">Curso: <?=$arCurso['curdsc'] ?></th>
	</tr>
	<tr>
		<td width="15%" class="subtitulodireita"><b>Área:</b></td>
		<td width="35%"><?=$arCurso['aredsc']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Objetivos de Aprendizagem:</b></td>
		<td><?=$arCurso['curobj']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Principais Conteúdos:</b></td>
		<td><?=$arCurso['curcont']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Outras Informações:</b></td>
		<td><?=$arCurso['curinfo']; ?></td>
	</tr>
	<!--<tr>
		<td class="subtitulodireita"><b>Período:</b></td>
		<td><?=$arCurso['turperiodo']; ?></td>
	</tr>
	--><tr>
		<td class="subtitulodireita"><b>Período:</b></td>
		<td><?=$arCurso['periodo']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Horário:</b></td>
		<td><?=$horarioFormatado; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Público Alvo:</b></td>
		<td><?=$arCurso['turpublicoalvo']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Carga Horária:</b></td>
		<td><?=$arCurso['turcarga']; ?> hora(s)</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b></b></td>
		<td><?=$arCurso['turstatus']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Outras Informações:</b></td>
		<td><?=$arCurso['turinfo']; ?></td>
	</tr>
	<tr>
		<th colspan="2">Facilitador</th>
	</tr>
	<?
	$sql = "SELECT distinct fa.facid, fa.faccpf, fa.facnome, fa.facmincurriculo, fa.facconhecimento
			FROM siscap.facilitador  fa
            	inner join siscap.turma_facilitador tf on tf.facid = fa.facid
            WHERE tf.turid =".$arCurso['turid'];

	$arFac = $db->carregar( $sql );
	$arFac = $arFac ? $arFac : array();

	foreach ($arFac as $v) {
	?>
	<tr>
		<td width="15%" class="subtitulodireita"><b>Nome:</b></td>
		<td><?=$v['facnome']; ?></td>
	</tr>
	<tr>
		<td width="15%" class="subtitulodireita"><b>Mini-currículo:</b></td>
		<td><?=$v['facmincurriculo']; ?></td>
	</tr>
	<tr>
		<td width="15%" class="subtitulodireita"><b>Conhecimentos:</b></td>
		<td><?=$v['facconhecimento']; ?></td>
	</tr>
	<tr>
		<td colspan="2" height="2px"></td>
	</tr>
	<?} ?>
	<tr bgcolor="#D0D0D0">
		<td colspan="6" style="text-align: center" class="div_rolagem">
			<input type="button" id="btImprimir" value="Imprimir" onclick="javascript: window.print();" />
			<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
		</td>
	</tr>
</table>