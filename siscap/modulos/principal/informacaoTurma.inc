<?php
if( empty($_GET['turid']) ){
	echo "<script>
				alert('Selecione uma tuma no cronogramas de cursos para inscrição!');
				window.location.href = 'siscap.php?modulo=principal/cronogramaCurso&acao=A';
			</script>";
	exit();
}

if( $_REQUEST['popup'] != 'popup' ){
	include APPRAIZ . 'includes/cabecalho.inc';
	print "<br>";
	$db->cria_aba( $abacod_tela, $url, '' );

	$_SESSION['siscap']['turid'] = !empty( $_SESSION['siscap']['turid'] ) ? $_SESSION['siscap']['turid'] : $_GET['turid'];
	$_SESSION['siscap']['curid'] = !empty( $_SESSION['siscap']['curid'] ) ? $_SESSION['siscap']['curid'] : $_GET['curid'];

} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
monta_titulo( 'Informações sobre a Turma', '' );

$sql = "SELECT cu.curid, a.aredsc, cu.curdsc, cu.curobj, cu.curcont, cu.curinfo,
			to_char(tu.turdtini, 'DD/MM/YYYY')||' a '||to_char(tu.turdtfim, 'DD/MM/YYYY') as periodo,
		    tu.turcarga, tu.turhrini||' às '||tu.turhrfim as horario,
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
		   tu.turpublicoalvo, tu.turinfo, tu.turid,
		   turhrinipm||' às '||turhrfimpm as periodopm,
		   turperiodo as tipoperiodo
		FROM
			siscap.curso cu
		    inner join siscap.turma tu on tu.curid = cu.curid
		    inner join siscap.area a on a.areid = cu.areid
		WHERE tu.turid = ".$_GET['turid'];

$arCurso = $db->pegaLinha( $sql );
$arCurso = $arCurso ? $arCurso : array();
?>
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
	<tr>
		<td class="subtitulodireita"><b>Período:</b></td>
		<td><?=$arCurso['periodo']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Horário:</b></td>
		<td><?
		$horarioX = str_replace( ':', 'h', $arCurso['horario'] );
		if( $arCurso['tipoperiodo'] == 'IN' && !empty( $arCurso['periodopm'] ) ){
			echo str_replace( ':', 'h', $arCurso['horario']).' Intervalo '.str_replace( ':', 'h', $arCurso['periodopm']);
		} else {
			echo $horarioX;
		}
		?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Público Alvo:</b></td>
		<td><?=$arCurso['turpublicoalvo']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Carga Horária:</b></td>
		<td><?=$arCurso['turcarga']; ?> h/a</td>
	</tr>
	<!--<tr>
		<td class="subtitulodireita"><b>Status:</b></td>
		<td><?=$arCurso['turstatus']; ?></td>
	</tr>-->
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
	</tr><!--
	<tr>
		<td width="15%" class="subtitulodireita"><b>Conhecimentos:</b></td>
		<td><?=$v['facconhecimento']; ?></td>
	</tr>
	--><tr>
		<td colspan="2" height="2px"></td>
	</tr>
	<?}
	if( $_REQUEST['popup'] != 'popup' ){
	?>
	<tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
			<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/cronogramaCurso&acao=A'" />
			<input type="button" id="btImprimir" value="Visualizar Impressão" onclick="imprimir();" />
			<input type="button" id="btInscricao" value="Inscrição" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/observacoesImportantes&acao=A'" />
		</td>
	</tr>
	<?} else { ?>
		<tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
				<input type="button" id="btImprimir" value="Fechar" onclick="window.close();" />
			</td>
		</tr>
	<?} ?>
</table>
<script>
function imprimir(){
	window.open('siscap.php?modulo=principal/popupImprimirTurma&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
}
</script>