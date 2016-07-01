<?php 

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

if($_POST['ppsid'] && $_POST['requisicao'] == "salvar" ){
	$oPropostaSubacaoParecer->pspid 		= $_POST['pspid']? $_POST['pspid'] : false;
	$oPropostaSubacaoParecer->ppsid 		= $_POST['ppsid'];
	$oPropostaSubacaoParecer->frmid 		= $oPropostaSubacao->frmid;
	$oPropostaSubacaoParecer->pspdescricao 	= $_POST['pspdescricao'];
	$oPropostaSubacaoParecer->pspstatus 	= "A";
	$oPropostaSubacaoParecer->pspdata 		= date('Y-m-d H:i:s');
	$oPropostaSubacaoParecer->salvar();
	$oPropostaSubacaoParecer->commit();
	
	$oPropostaSubacaoParecer->pspid = false;
	$oPropostaSubacaoParecer->ppsid = false;
	$oPropostaSubacaoParecer->frmid = false;
	$oPropostaSubacaoParecer->pspdescricao = false;
	$oPropostaSubacaoParecer->pspdata = false;
	$oPropostaSubacaoParecer->pspstatus = false;
}

if($_POST['ppsid'] && $_POST['requisicao'] == "excluir" ){
	$oPropostaSubacaoParecer->pspid 		= $_POST['pspid'];
	$oPropostaSubacaoParecer->pspstatus 	= "I";
	$oPropostaSubacaoParecer->salvar();
	$oPropostaSubacaoParecer->commit();		
}

$arPareceres = $oPropostaSubacaoParecer->recuperarPareceresPorPpsid($_REQUEST['ppsid']);
$arPareceres = $arPareceres ? $arPareceres : array();
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
	
	function excluirParecer(pspid)
	{
		if(confirm("Deseja realmente excluir?")){
			jQuery("[name=requisicao]").val("excluir");
			jQuery("[name=pspid]").val(pspid);
			jQuery("#formulario").submit();
		}
	}
	
	function editarParecer(pspid)
	{
		jQuery("[name=requisicao]").val("carregar");
		jQuery("[name=pspid]").val(pspid);
		jQuery("#formulario").submit();
	}
	</script>
	<?php	
	monta_titulo( 'Cadastro de pareceres', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
	?>
	<form name="formulario"  id="formulario" action="" method="post">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Localização da subação</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width:250px;">Dimensão:</td>
				<td><?php echo $arDados[0]['codigodimensao'] . ' - ' . $arDados[0]['descricaodimensao']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Área:</td>
				<td><?php echo $arDados[0]['codigodimensao'] . '.' . $arDados[0]['codigoarea'] . ' - ' . $arDados[0]['descricaoarea']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Indicador:</td>
				<td><?php echo $arDados[0]['codigodimensao'] . '.' . $arDados[0]['codigoarea'] . '.' . $arDados[0]['codigoindicador'] . ' - ' . $arDados[0]['descricaoindicador']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Subação:</td>
				<td><?php echo $oPropostaSubacao->ppsdsc; ?></td>
			</tr>
		</table>
		<br />
		<?php print carregaAbasPropostaSubacao("par.php?modulo=principal/configuracao/popupGuiaSubacaoParecer&acao=A&acaoGuia=editar&ppsid={$_GET['ppsid']}", $oPropostaSubacao->frmid, $_GET['ppsid'],$oPropostaSubacao->indid,$oPropostaSubacao->ptsid); ?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Preechimento dos dados do parecer</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Parecer padrão:</td>
				<td>
					<?php $pspdescricao = $oPropostaSubacaoParecer->pspdescricao ?>
					<?php echo campo_textarea('pspdescricao', 'S', 'S', '', 70, 6, '','', 0, '', false, null, $pspdescricao) ?>
				</td>
			</tr>
			<tr class="SubTituloCentro" align="center">
				<td colspan="2">
					<input type="hidden" name="ppsid" value="<?php echo $ppsid ?>">
					<input type="hidden" name="pspid" value="<?php echo $oPropostaSubacaoParecer->pspid ?>">
					<input type="button" id="btn_salvar" value="Salvar">
					<input type="hidden" name="requisicao" value="salvar">
				</td>
			</tr>
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Lista de pareceres cadastrados</b></td>
			</tr>
		</table>
	</form>
	<?php $cabecalho = array("Ação","Descrição","Data") ?>
	<?php $db->monta_lista_array($arPareceres,$cabecalho,30,5,"N","center"); ?>
	</body>
</html>