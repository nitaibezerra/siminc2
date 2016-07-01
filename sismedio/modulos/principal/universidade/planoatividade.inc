<?

$consulta = verificaPermissao();

$_SESSION['sismedio']['universidade']['ecuid'] = pegarEstruturaCurso(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));

$info = carregarCadastroIESProjeto(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));

$estruturacurso = carregarEstruturaCurso(array("ecuid"=>$_SESSION['sismedio']['universidade']['ecuid']));

if($estruturacurso) {
	extract($estruturacurso);
}

?>
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>

function atualizarPlanoAtividadeExecucao() {

	jQuery('#formulario').submit();

}
</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="atualizarPlanoAtividadeExecucao">
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
	
		<h3>1ª Etapa do Curso de Formação dos Formadores Regionais</h3>
		<? carregarPlanoAtividadesExecucao(array("ecuid"=>$_SESSION['sismedio']['universidade']['ecuid'],"attitipo"=>"1")); ?>
		<h3>2ª Etapa do Curso de Formação dos Formadores Regionais</h3>
		<? carregarPlanoAtividadesExecucao(array("ecuid"=>$_SESSION['sismedio']['universidade']['ecuid'],"attitipo"=>"2")); ?>
		<br/>
		<p>Comentários sobre o cronograma (plano de atividades):</p>
		<?=campo_textarea('ecuobsplanoatividadesexecucao', 'N', 'S', '', '100', '12', '5000'); ?>
	
	
	</td>
</tr>

	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=solicitacaotroca';"> <input type="button" name="salvar" value="Salvar" onclick="atualizarPlanoAtividadeExecucao();">
		</td>
	</tr>
</table>
</form>