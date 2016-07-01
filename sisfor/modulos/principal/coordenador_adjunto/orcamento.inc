<?php
$estado = wf_pegarEstadoAtual( $_SESSION['sisfor']['docidprojeto'] );

if($estado['esdid'] != ESD_PROJETO_EMCADASTRAMENTO || in_array(PFL_CONSULTAGERAL,$perfil)) {
	$consulta = true;
} ?>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function inserirCustos(orcid) {
	var param='';
	if(orcid!='') {
		param = '&orcid='+orcid;
	}
	window.open('sisfor.php?modulo=principal/coordenador_curso/inserircustos&acao=A'+param,'Custos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function carregarListaCustos() {
	ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_listacustos');
	ajaxatualizar('requisicao=calcularCustoAlunoCusteio&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_custoaluno');
}

function carregarNaturezaDespesasCustos() {
	ajaxatualizar('requisicao=carregarNaturezaDespesasCustos&uncid=<?=$_SESSION['sispacto']['universidade']['uncid'] ?>','td_naturezadespesas');
}

function excluirCustos(orcid) {
	var conf = confirm('Deseja realmente excluir este registro?');
	
	if(conf) {
		ajaxatualizar('requisicao=excluirCustos&orcid='+orcid,'');
		ajaxatualizar('requisicao=carregarListaCustos&sifid=<?=$_SESSION['sisfor']['sifid'] ?>','div_listacustos');
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
		<td>Orientação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<p><input type="button" value="Inserir Custos" id="inserircustos" onclick="inserirCustos('');"></p>
		<div id="div_listacustos"><?
		carregarListaCustos(array("consulta"=>$consulta,"sifid"=>$_SESSION['sisfor']['sifid']));
		?></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Custo Aluno (custeio)</td>
		<td id="div_custoaluno">
		<? 
		calcularCustoAlunoCusteio(array("sifid"=>$_SESSION['sisfor']['sifid']));
		?>
		</td>
	</tr>
</table>
</form>