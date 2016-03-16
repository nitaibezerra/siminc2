<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));

$estado = wf_pegarEstadoAtual( $_SESSION['sismedio']['universidade']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

if(!$consulta) $consulta = verificaPermissao();

$_SESSION['sismedio']['universidade']['ecuid'] = pegarEstruturaCurso(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<link href="./css/jquery.alerts.css" rel="stylesheet" type="text/css"/>
<script language="javascript" type="text/javascript" src="./js/alerts.js"></script>

<script>

function salvarEquipeIES(goto) {

	divCarregando();
	
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}

function excluirEquipeRecursosHumanos(iusd) {
	var conf = confirm('Deseja realmente excluir este usuário? Caso este(a) seja formador(a), sua(s) turma(s) será deletada e os Orientadores de Estudo deverão ser vinculados a outra turma.');
	
	if(conf) {
		window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&requisicao=excluirEquipeRecursosHumanos&iusd='+iusd;
	}

}

function inserirEquipe(iusd) {
	var param='';
	if(iusd!='') {
		param += '&iusd='+iusd;
	}
	window.open('sismedio.php?modulo=principal/universidade/inserirequipe&acao=A'+param,'Equipe','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function carregarEquipeRecursosHumanos() {
	ajaxatualizar('requisicao=carregarEquipeRecursosHumanos&uncid=<?=$_SESSION['sismedio']['universidade']['uncid'] ?>','td_equipeRecursosHumanos');
}

function abrirDocumentosRecursosHumanos(iusd) {
	window.open('sismedio.php?modulo=principal/universidade/inserirdocumento&acao=A&iusd='+iusd,'Documento','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function removerAnexoPortaria(ponid) {
	var conf = confirm('Deseja realmente excluir este anexo?');
	
	if(conf) {
		divCarregando();
		window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&requisicao=removerAnexoPortaria&ponid='+ponid;
	}
}

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='cadastrarfuncao']").remove();
});
<? endif; ?>


</script>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="curid" value="<?=$_SESSION['sismedio']['universidade']['curid'] ?>">
<input type="hidden" name="uncid" value="<?=$_SESSION['sismedio']['universidade']['uncid'] ?>">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Equipe IES</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($abaativa); ?></td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sismedio']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type="button" name="cadastrarfuncao" id="cadastrarfuncao" value="Cadastrar Equipe" onclick="inserirEquipe('');"></td>
	</tr>

	<tr>
		<td colspan="2" id="td_equipeRecursosHumanos">
		<?=carregarEquipeRecursosHumanos(array("consulta"=>$consulta,"uncid" => $_SESSION['sismedio']['universidade']['uncid'])); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=estrutura_curso';">
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=orcamento';">
		</td>
	</tr>
</table>
</form>