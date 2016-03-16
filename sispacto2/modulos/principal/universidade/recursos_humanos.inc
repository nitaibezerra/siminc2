<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid']));
verificarValidacaoEstruturaFormacao(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid']));

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto2']['universidade']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

$_SESSION['sispacto2']['universidade']['ecuid'] = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto2']['universidade']['uncid']));
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
	var conf = confirm('Deseja realmente excluir este usuário?');
	
	if(conf) {
		window.location='sispacto2.php?modulo=principal/universidade/universidade&acao=A&requisicao=excluirEquipeRecursosHumanos&iusd='+iusd;
	}

}

function inserirEquipe(iusd) {
	var param='';
	if(iusd!='') {
		param += '&iusd='+iusd;
	}
	window.open('sispacto2.php?modulo=principal/universidade/inserirequipe&acao=A'+param,'Equipe','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function carregarEquipeRecursosHumanos() {
	ajaxatualizar('requisicao=carregarEquipeRecursosHumanos&uncid=<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>','td_equipeRecursosHumanos');
}

function abrirDocumentosRecursosHumanos(iusd) {
	window.open('sispacto2.php?modulo=principal/universidade/inserirdocumento&acao=A&iusd='+iusd,'Documento','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function removerAnexoPortaria(ponid) {
	var conf = confirm('Deseja realmente excluir este anexo?');
	
	if(conf) {
		divCarregando();
		window.location='sispacto2.php?modulo=principal/universidade/universidade&acao=A&requisicao=removerAnexoPortaria&ponid='+ponid;
	}
}

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='cadastrarfuncao']").remove();
jQuery("[name='importar']").css('display','none');
});
<? endif; ?>


</script>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="curid" value="<?=$_SESSION['sispacto2']['universidade']['curid'] ?>">
<input type="hidden" name="uncid" value="<?=$_SESSION['sispacto2']['universidade']['uncid'] ?>">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Equipe IES</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td colspan="2" align="right"><input type="button" name="importar" value="Importar SISPACTO 2013" onclick="importarInformacoesSispacto('recursos_humanos');"></td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto2']['universidade']['curid']));
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
		<?=carregarEquipeRecursosHumanos(array("consulta"=>$consulta,"uncid" => $_SESSION['sispacto2']['universidade']['uncid'])); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'])) ?>
		</td>
	</tr>
</table>
</form>