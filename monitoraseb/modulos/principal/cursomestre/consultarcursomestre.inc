<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$titulo = "Consultar Curso Mestre";
monta_titulo( $titulo, '&nbsp;' );

include APPRAIZ."monitoraseb/classes/CursoMestre.class.inc";
$oCursoMestre = new CursoMestre();

if(!empty($_REQUEST['excluir'])){
	$oCursoMestre->excluirCursoMestre($_REQUEST['excluir']);
}
?>

<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
	function pesquisar(acao){
		var url = 'monitoraseb.php?modulo=principal/cursomestre/consultarcursomestre&acao=A&pesquisar='+acao;
		var formulario=document.getElementById("formulario");
		formulario.action = url;
		formulario.submit();
	}
	function listarSubacao(coonid){
		mostraComboSubAcao(coonid,'','','','S','');

	}

	function detalharCursoMestre(cursoId){
		window.location = "?modulo=principal/cursomestre/cadastrarcursomestre&acao=A&cmtid="+cursoId;

	}
	function removerCursoMestre(cursoId){
		window.location = "?modulo=principal/cursomestre/consultarcursomestre&acao=A&excluir="+cursoId;

	}


</script>
<form name="formulario" id="formulario" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubTituloDireita">Coordenação:</td>
			<td><? $oCursoMestre->mostraComboCoordenacao();?></td>
		</tr>

		<tr>
			<td align='left' class="SubTituloDireita">Subacao:</td>
			<td>
				<div id="sbaid_on" style="display:none;">
				</div>
				<div id="sbaid_off" style="color:#909090;">Selecione uma Coordenacao.</div>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC" colspan="2" align="center">
				<input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="pesquisar('filtrar');">
				<input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="pesquisar('todos');">
			</td>
		</tr>
		<tr>
			<td colspan=3>
				<a href="?modulo=principal/cursomestre/cadastrarcursomestre&acao=A"><img src="/imagens/gif_inclui.gif" border="0"/> Adicionar</a>
			</td>
		</tr>
	</table>
</form>
<?php

if(!empty($_REQUEST['pesquisar'])){
	if($_REQUEST['pesquisar'] =='filtrar'){
		$oCursoMestre->listarCursoMestrePorCoordenacaoSubacao($_REQUEST['coonid'],$_REQUEST['sbaid_disable'], $_SESSION['usucpf']);
	}elseif ($_REQUEST['pesquisar'] =='todos'){
		$oCursoMestre->listarCursoMestrePorCoordenacaoSubacao('', '', $_SESSION['usucpf']);
	}
}
?>
