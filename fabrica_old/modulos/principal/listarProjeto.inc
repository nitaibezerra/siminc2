<?php
include APPRAIZ."fabrica/classes/Projeto.class.inc";


if($_REQUEST['requisicao'] && $_REQUEST['prjid']){
	$prjid = $_REQUEST['prjid'];
	$projeto = new Projeto($prjid);
	$projeto->$_REQUEST['requisicao']();
	if($_REQUEST['reload']){
		Header("Location: fabrica.php?modulo=principal/listarProjeto&acao=A");
	}
}


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Lista de Projetos";
monta_titulo( $titulo, '&nbsp;' );

$projeto = new Projeto();

if($_REQUEST['requisicao'] && !$_REQUEST['prjid']){
	$projeto->$_REQUEST['requisicao']();
}


extract($_POST);

?>
<script>
	function alterarProjeto(prjid)
	{
		window.location.href = 'fabrica.php?modulo=principal/cadProjeto&acao=A&prjid=' + prjid;
	}
	
	function excluirProjeto(prjid)
	{
		if(confirm('Deseja realmente excluir o projeto?')){
			window.location.href = 'fabrica.php?modulo=principal/listarProjeto&acao=A&requisicao=excluirProjeto&reload=true&prjid=' + prjid;
		}
	}
	
	function novoProjeto()
	{
		window.location.href = 'fabrica.php?modulo=principal/cadProjeto&acao=A';
	}
	
	function pesquisarProjeto()
	{
		document.getElementById('form_listar_projeto').submit();
	}
	
</script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<form name="form_listar_projeto" id="form_listar_projeto" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Nome do Projeto:</td>
			<td>
				<input type="hidden" name="requisicao" value="pesquisarProjeto" />
				<?=campo_texto("prjnome","N","S","Nome do Projeto","60","120","","") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Sigla:</td>
			<td><?=campo_texto("prjsigla","N","S","Sigla do Projeto","40","120","","") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Unidade:</td>
			<td>
				<?php $sql = "	select
									ungcod as codigo,
									ungdsc as descricao
								from
									public.unidadegestora
								where
									ungstatus = 'A'
								order by
									ungdsc"; ?>
				<?=$db->monta_combo("ungcod",$sql,"S","Selecione...","","","","","N") ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="pesquisarProjeto()" value="Pesquisar" >
				<input type="button" onclick="novoProjeto()" value="Novo Projeto" >
			</td>
		</tr>
	</table>
</form>
<?php $projeto->showListaProjetos(); ?>