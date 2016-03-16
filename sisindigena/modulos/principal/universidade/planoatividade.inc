<?

$sql = "SELECT a.atiid, a.atidesc, s.suaid, s.suadesc, 
		(SELECT au.aundatainicioprev FROM sispacto.atividadeuniversidade au 
		 INNER JOIN sispacto.estruturacurso es ON es.ecuid = au.ecuid 
		 WHERE au.suaid = s.suaid AND es.uncid='".$_SESSION['sispacto']['universidade']['uncid']."') as aundatainicioprev, 
		(SELECT au.aundatafimprev FROM sispacto.atividadeuniversidade au 
		 INNER JOIN sispacto.estruturacurso es ON es.ecuid = au.ecuid
		 WHERE au.suaid = s.suaid AND es.uncid='".$_SESSION['sispacto']['universidade']['uncid']."') as aundatafimprev,
		(SELECT au.aundatainicio FROM sispacto.atividadeuniversidade au 
		 INNER JOIN sispacto.estruturacurso es ON es.ecuid = au.ecuid 
		 WHERE au.suaid = s.suaid AND es.uncid='".$_SESSION['sispacto']['universidade']['uncid']."') as aundatainicio, 
		(SELECT au.aundatafim FROM sispacto.atividadeuniversidade au 
		 INNER JOIN sispacto.estruturacurso es ON es.ecuid = au.ecuid
		 WHERE au.suaid = s.suaid AND es.uncid='".$_SESSION['sispacto']['universidade']['uncid']."') as aundatafim 
		 
		FROM sispacto.subatividades s 
		INNER JOIN sispacto.atividades a ON a.atiid = s.atiid 
		WHERE a.attitipo IN('E') AND s.suavisivel=true ORDER BY a.atidesc, s.suadesc";

$subatividades = $db->carregar($sql);

if($subatividades[0]) {
	foreach($subatividades as $sub) {
		$arrRsAgrupado[$sub['atidesc']]['atividade'] = $sub['atidesc'];
		$arrRsAgrupado[$sub['atidesc']]['subatividades'][] = array("suaid"=>$sub['suaid'],
																   "suadesc"=>$sub['suadesc'],
																   "aunid"=>$sub['aunid'],
																   "aundatainicio"=>$sub['aundatainicio'],
																   "aundatainicioprev"=>$sub['aundatainicioprev'],
																   "aundatafim"=>$sub['aundatafim'],
																   "aundatafimprev"=>$sub['aundatafimprev']);
	}
}

?>
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="atualizarPlanoAtividade">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Plano de atividades - Execução</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" width="40%">Atividade</td>
		<td class="SubTituloCentro" width="10%">Início<br>(Previsto)</td>
		<td class="SubTituloCentro" width="10%">Término<br>(Previsto)</td>
		<td class="SubTituloCentro" width="20%">Início</td>
		<td class="SubTituloCentro" width="20%">Término</td>
	</tr>
	<? 
	if($arrRsAgrupado) :
		foreach($arrRsAgrupado as $atidesc => $subs) :
		?>
		<tr>
			<td><img src="../imagens/menos.gif"> <b><?=$atidesc ?></b></td>
			<td colspan="4">&nbsp;</td>
		</tr>
		<? 
		if($subs['subatividades']) :
			foreach($subs['subatividades'] as $subatividade) :
			?>
			<tr>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> <?=$subatividade['suadesc'] ?></td>
				<td><?=(($subatividade['aundatainicioprev'])?formata_data($subatividade['aundatainicioprev']):"-") ?></td>
				<td><?=(($subatividade['aundatafimprev'])?formata_data($subatividade['aundatafimprev']):"-") ?></td>
				<td><? echo campo_data2('aundatainicio['.$subatividade['suaid'].']','N','S', 'Início', 'S', '', '', $subatividade['aundatainicio'] ); ?></td>
				<td><? echo campo_data2('aundatafim['.$subatividade['suaid'].']','N','S', 'Término', 'S', '', '', $subatividade['aundatafim'] ); ?></td>
			</tr>
			<?
			endforeach;
		endif;
		endforeach;  
	endif; 
	?>
	</table>
	
	</td>
</tr>

	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=dados';"> <input type="button" name="salvar" value="Salvar" onclick="document.getElementById('formulario').submit()">
		</td>
	</tr>
</table>
</form>