<?
$estado = wf_pegarEstadoAtual( $_SESSION['sisindigena2']['coordenadoradjuntoies']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function inserirCustos(orcid) {
	var param='';
	if(orcid!='') {
		param = '&orcid='+orcid;
	}
	window.open('sisindigena2.php?modulo=principal/coordenadoradjuntoies/inserircustos&acao=A'+param,'Custos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function carregarListaCustos() {
	ajaxatualizar('requisicao=carregarListaCustos&picid=<?=$_SESSION['sisindigena2']['coordenadoradjuntoies']['picid'] ?>','div_listacustos');
}

function carregarNaturezaDespesasCustos() {
	ajaxatualizar('requisicao=carregarNaturezaDespesasCustos&picid=<?=$_SESSION['sisindigena2']['coordenadoradjuntoies']['picid'] ?>','td_naturezadespesas');
}

function excluirCustos(orcid) {
	var conf = confirm('Deseja realmente excluir este registro?');
	
	if(conf) {
		ajaxatualizar('requisicao=excluirCustos&orcid='+orcid,'');
		ajaxatualizar('requisicao=carregarListaCustos&picid=<?=$_SESSION['sisindigena2']['coordenadoradjuntoies']['picid'] ?>','div_listacustos');
	}

}

function importarOrcamentoSisindigena2014() {
	var conf = confirm('Deseja realmente importar o "Orçamento" do SISIndígena 2014?');
	if(conf) {
		ajaxatualizar('requisicao=importarOrcamentoSisindigena2014','');
		ajaxatualizar('requisicao=carregarListaCustos&picid=<?=$_SESSION['sisindigena2']['coordenadoradjuntoies']['picid'] ?>','div_listacustos');
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
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<p><input type="button" value="Inserir Custos" id="inserircustos" onclick="inserirCustos('');"></p>
		<div id="div_listacustos"><?
		carregarListaCustos(array("consulta"=>$consulta,"picid"=>$_SESSION['sisindigena2']['coordenadoradjuntoies']['picid']));
		?></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=definirequipe';">
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=visualizacao_projeto';">
		</td>
	</tr>
</table>
</form>