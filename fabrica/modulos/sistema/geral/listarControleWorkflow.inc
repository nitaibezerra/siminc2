<?php
include APPRAIZ."fabrica/classes/ControleWorkFlow.class.inc";


if($_REQUEST['requisicao'] && $_REQUEST['ctwid']){
	$ctwid = $_REQUEST['ctwid'];
	$controleWorkFlow = new ControleWorkFlow($ctwid);
	$controleWorkFlow->$_REQUEST['requisicao']();
	if($_REQUEST['reload']){
		Header("Location: fabrica.php?modulo=sistema/geral/listarControleWorkflow&acao=A");
	}
}


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
$menu[0] = array("descricao" => "Listar Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/listarControleWorkflow&acao=A");
$menu[1] = array("descricao" => "Cadastrar Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/cadControleWorkflow&acao=A");
$menu[2] = array("descricao" => "Executar Script de Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/controleWorkflow&acao=A");
echo montarAbasArray($menu, "fabrica.php?modulo=sistema/geral/listarControleWorkflow&acao=A");

$titulo = "Lista de Controle de WorkFlow";
monta_titulo( $titulo, '&nbsp;' );

$controleWorkFlow = new ControleWorkFlow();

if($_REQUEST['requisicao'] && !$_REQUEST['ctwid']){
	$controleWorkFlow->$_REQUEST['requisicao']();
}


extract($_POST);

?>
<script>
	function alterarControleWorkFlow(ctwid)
	{
		window.location.href = 'fabrica.php?modulo=sistema/geral/cadControleWorkflow&acao=A&ctwid=' + ctwid;
	}
	
	function excluirControleWorkFlow(ctwid)
	{
		if(confirm('Deseja realmente excluir a regra de controle do WorkFlow?')){
			window.location.href = 'fabrica.php?modulo=sistema/geral/listarControleWorkflow&acao=A&requisicao=excluirControleWorkFlow&reload=true&ctwid=' + ctwid;
		}
	}
	
	function novoControleWorkFlow()
	{
		window.location.href = 'fabrica.php?modulo=sistema/geral/cadControleWorkflow&acao=A';
	}
	
	function pesquisarControleWorkFlow()
	{
		document.getElementById('form_listar_controle_workflow').submit();
	}
	
</script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<form name="form_listar_controle_workflow" id="form_listar_controle_workflow" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita">Estado do WorkFlow:</td>
			<td>
				<?php $sql = "	select
									esd.esdid as codigo,
									tpd.tpddsc || ' - ' || esd.esddsc as descricao
								from
									workflow.estadodocumento esd
								inner join
									workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
								where
									esd.esdstatus = 'A'
								and
									sisid = ".$_SESSION['sisid']."
								order by
									tpd.tpddsc,
									esd.esddsc"; ?>
				<?=$db->monta_combo("esdid",$sql,"S","Selecione...","","","","","N") ?>
				<input type="hidden" name="requisicao" value="pesquisarControleWorkFlow" />
			</td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="pesquisarControleWorkFlow()" value="Pesquisar" >
				<input type="button" onclick="novoControleWorkFlow()" value="Novo Controle de WorkFlow" >
			</td>
		</tr>
	</table>
</form>
<?php $controleWorkFlow->showListaControleWorkFlow(); ?>