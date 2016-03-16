<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));
verificarValidacaoEstruturaFormacao(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));

$estado = wf_pegarEstadoAtual( $_SESSION['sismedio']['universidade']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

if(!$consulta) $consulta = verificaPermissao();

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function inserirCustos(orcid) {
	var param='';
	if(orcid!='') {
		param = '&orcid='+orcid;
	}
	window.open('sismedio.php?modulo=principal/universidade/inserircustos&acao=A'+param,'Custos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function carregarListaCustos() {
	ajaxatualizar('requisicao=carregarListaCustos&uncid=<?=$_SESSION['sismedio']['universidade']['uncid'] ?>','div_listacustos');
}

function carregarNaturezaDespesasCustos() {
	ajaxatualizar('requisicao=carregarNaturezaDespesasCustos&uncid=<?=$_SESSION['sismedio']['universidade']['uncid'] ?>','td_naturezadespesas');
}

function excluirCustos(orcid) {
	var conf = confirm('Deseja realmente excluir este registro?');
	
	if(conf) {
		ajaxatualizar('requisicao=excluirCustos&orcid='+orcid,'');
		ajaxatualizar('requisicao=carregarListaCustos&uncid=<?=$_SESSION['sismedio']['universidade']['uncid'] ?>','div_listacustos');
	}

}

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='inserircustos']").remove();
});
<? endif; ?>

</script>
<form method="post" id="formulario">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orçamento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
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
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<p><input type="button" value="Inserir Custos" id="inserircustos" onclick="inserirCustos('');"></p>
		<div id="div_listacustos"><?
		carregarListaCustos(array("consulta"=>$consulta,"uncid"=>$_SESSION['sismedio']['universidade']['uncid']));
		?></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=recursos_humanos';">
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=visualizacao_projeto';">
		</td>
	</tr>
</table>
</form>