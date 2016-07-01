<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function editarPlanejamento(pieid) {

	window.location='<?=$_SERVER['REQUEST_URI'] ?>&pieid='+pieid;
	
}

function exibirInsercaoPlanejamento() {
	
	jQuery("#modalInserirPlanejamento").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function salvarPlanejamentoInstituicaoExercicio() {

	if(jQuery('#pieexercicio').val()=='') {
		alert('Selecione um Exercício');
		return false;
	}
	
	if(jQuery('#insid').val()=='') {
		alert('Selecione uma instituição');
		return false;
	}
	
	jQuery('#form_planejamento').submit();

}
</script>

<div id="modalInserirPlanejamento" style="display:none;">

<form method="post" name="form_planejamento" id="form_planejamento">
<input type="hidden" name="requisicao" value="salvarPlanejamentoInstituicaoExercicio">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Exercício</td>
	<td>
	<?
	
	$sql = "SELECT ano as codigo, ano as descricao FROM public.anos ORDER BY ano";
    $db->monta_combo('pieexercicio', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'pieexercicio',''); 
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Instituição</td>
	<td>
	<?
	
	$sql = "SELECT insid as codigo, inssigla || ' / ' || insnome as descricao FROM smfc.instituicoes WHERE insstatus='A'";
    $db->monta_combo('insid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'insid',''); 
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="inserir" value="Salvar" onclick="salvarPlanejamentoInstituicaoExercicio();"></td>
</tr>
</table>
</form>

</div>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao(""); ?></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2"><input type="button" name="inserir" value="Inserir Instituicão" onclick="exibirInsercaoPlanejamento();"></td>
</tr>
<tr>
	<td colspan="2">
	<?
	
	$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"editarPlanejamento();\">' as acao, 
				   inssigla,
				   insnome,
				   insemail
			FROM smfc.instituicoes 
			WHERE insstatus='A'";
	
	$cabecalho = array("&nbsp;","Sigla","Instituição","Email");
	$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));
	
	?>	
	</td>
</tr>
</table>