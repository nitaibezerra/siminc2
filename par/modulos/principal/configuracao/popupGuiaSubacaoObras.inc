<?php
function salvarTipoPreObra()
{
	global $db;
	extract($_POST);
	
	$sql = "select ppsid from par.propostasubacaoobra where ppsid = $ppsid and ptoid = $ptoid";
	if(!$db->pegaUm($sql)){
		$sql = "insert into par.propostasubacaoobra (ppsid,ptoid) values ($ppsid,$ptoid)";
		$db->executar($sql);
		$db->commit();
	}
	
}

function excluirTipoObra()
{
	global $db;
	extract($_POST);
	
	$sql = "delete from par.propostasubacaoobra where ppsid = $ppsid and ptoid = $ptoiddel";
	$db->executar($sql);
	$db->commit();
	
}


$oConfiguracaoControle 		= new ConfiguracaoControle();
$oPropostaSubacao 			= new PropostaSubacao();
$oCriterioPropostaSubacao 	= new CriterioPropostaSubacao();
$oPropostaSubacaoMunicipio 	= new PropostaSubacaoMunicipio();
$oPropostaSubacaoParecer 	= new PropostaSubacaoParecer();

if($_GET['ppsid']){
	$oPropostaSubacao->carregarPorId($_GET['ppsid']);
}

if($_POST['requisicao'] == "carregar" && $_POST['pspid']){
	$oPropostaSubacaoParecer->carregarPorId($_POST['pspid']);
}

$indid = $_GET['indid'];
if($_GET['acaoGuia'] == 'editar'){
	
	$oPropostaSubacao->carregarPorId($_GET['ppsid']);			
	$indid = $oPropostaSubacao->indid;
	$ppsid = $_GET['ppsid'];
	$arCriterios = $oCriterioPropostaSubacao->carregarPorPpsid($ppsid); 
}

$arDados = $oConfiguracaoControle->recuperaDadosFormGuiaSubacao($indid);


if($_POST['requisicao']){
	$_POST['requisicao']();
}

?>
<html>
	<head>
		<title>PAR - Cadastro de subação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
	</head>
	<body>
	<script type="text/javascript">

	jQuery.noConflict();
	
	jQuery(document).ready(function(){
		 jQuery('#btn_salvar').click(function() {
			var erro = 0;
			jQuery("[class~=obrigatorio]").each(function() { 
				if(!this.value){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios!');
					this.focus();
					return false;
				}
			});
			if(erro == 0){
				jQuery("#formulario").submit();
			}
		});
	
	});
	
	function excluirTipoObra(ptoid)
	{
		if(confirm("Deseja realmente excluir?")){
			jQuery("[name=requisicao]").val("excluirTipoObra");
			jQuery("[name=ptoiddel]").val(ptoid);
			jQuery("#formulario").submit();
		}
	}
	
	function editarTipoObra(ptoid)
	{
		jQuery("[name=requisicao]").val("carregarTipoObra");
		jQuery("[name=ptoid]").val(ptoid);
		jQuery("#formulario").submit();
	}
	</script>
	<?php	
	monta_titulo( 'Tipo de Obras', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
	?>
	<form name="formulario"  id="formulario" action="" method="post">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Localização da subação</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width:250px;">Dimenssão:</td>
				<td><?php echo $arDados[0]['descricaodimensao']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Área:</td>
				<td><?php echo $arDados[0]['descricaoarea']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Indicador:</td>
				<td><?php echo $arDados[0]['descricaoindicador']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Subação:</td>
				<td><?php echo $oPropostaSubacao->ppsdsc; ?></td>
			</tr>
		</table>
		<br />
		<?php print carregaAbasPropostaSubacao("par.php?modulo=principal/configuracao/popupGuiaSubacaoObras&acao=A&acaoGuia=editar&ppsid={$_GET['ppsid']}", $oPropostaSubacao->frmid, $_GET['ppsid'],$oPropostaSubacao->indid,$oPropostaSubacao->ptsid); ?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Preechimento dos dados do tipo de obra</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tipo de Obra:</td>
				<td>
					<?php
					$sql = "SELECT
								ptoid as codigo,
								ptodescricao as descricao
							FROM
								obras.pretipoobra
							WHERE
								ptostatus = 'A'
							AND
								ptoid not in (select ptoid from par.propostasubacaoobra where ppsid = $ppsid) 
							";
					$db->monta_combo('ptoid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'ptoid'); 
					?>
				</td>
			</tr>
			<tr class="SubTituloCentro" align="center">
				<td colspan="2">
					<input type="hidden" name="ppsid" value="<?php echo $ppsid ?>">
					<input type="hidden" name="ptoiddel" value="">
					<input type="button" id="btn_salvar" value="Salvar">
					<input type="hidden" name="requisicao" value="salvarTipoPreObra">
				</td>
			</tr>
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Lista de tipo de obras cadastradas</b></td>
			</tr>
		</table>
	</form>
	<?php $sql = "	SELECT
						'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirTipoObra(' || pso.ptoid || ')\"   />' as acao,
						ptodescricao as descricao
					FROM
						obras.pretipoobra pto
					INNER JOIN
						par.propostasubacaoobra pso ON pto.ptoid = pso.ptoid
					WHERE
						ptostatus = 'A'
					AND
						ppsid = $ppsid "; ?>
	<?php $cabecalho = array("Ação","Tipo de Obra") ?>
	<?php $db->monta_lista($sql,$cabecalho,30,5,"N","center"); ?>
	</body>
</html>