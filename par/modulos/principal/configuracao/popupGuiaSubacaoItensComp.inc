<?php 

$oConfiguracaoControle 		= new ConfiguracaoControle();
$oPropostaSubacao 			= new PropostaSubacao();
$oCriterioPropostaSubacao 	= new CriterioPropostaSubacao();
$oPropostaSubacaoMunicipio 	= new PropostaSubacaoMunicipio();
$oPropostaSubacaoParecer 	= new PropostaSubacaoParecer();

if($_GET['ppsid']){
	$oPropostaSubacao->carregarPorId($_GET['ppsid']);
}
//ver("par.php?modulo=principal/configuracao/popupGuiaSubacaoParecer&acao=A&acaoGuia=editar&ppsid={$_GET['ppsid']}");

$indid = $_GET['indid'];
if($_GET['acaoGuia'] == 'editar'){
	
	$oPropostaSubacao->carregarPorId($_GET['ppsid']);			
	$indid = $oPropostaSubacao->indid;
	$ppsid = $_GET['ppsid'];
	$arCriterios = $oCriterioPropostaSubacao->carregarPorPpsid($ppsid); 
}

$arDados = $oConfiguracaoControle->recuperaDadosFormGuiaSubacao($indid);

if($_POST['ppsid']){
	
//	$oPropostaSubacaoParecer->pspid 		= $_POST['pspid'];
	$oPropostaSubacaoParecer->ppsid 		= $_POST['ppsid'];
	$oPropostaSubacaoParecer->frmid 		= $oPropostaSubacao->frmid;
	$oPropostaSubacaoParecer->pspdescricao 	= $_POST['pspdescricao'];
	$oPropostaSubacaoParecer->pspstatus 	= $_POST['pspstatus'];
//	$oPropostaSubacaoParecer->pspordem 		= $_POST['pspordem'];
	$oPropostaSubacaoParecer->pspdata 		= date('Y-m-d H:i:s');
	$oPropostaSubacaoParecer->salvar();
	$oPropostaSubacaoParecer->commit();		
}

$arPareceres = $oPropostaSubacaoParecer->recuperarPareceresPorPpsid($ppsid);
$arPareceres = $arPareceres ? $arPareceres : array();
?>
<html>
	<head>
		<title>PAR - Cadastro de subação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>	
	</head>
	<body>
	<?php	
	monta_titulo( 'Cadastro de itens de composição', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
	?>
	<form action="" method="post">
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
		<?php print carregaAbasPropostaSubacao("par.php?modulo=principal/configuracao/popupGuiaSubacaoItensComp&acao=A&acaoGuia=editar&ppsid={$_GET['ppsid']}", $oPropostaSubacao->frmid, $_GET['ppsid']); ?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Preechimento dos itens de composição</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Descrição do item:</td>
				<td>
					<?php echo campo_textarea('pspdescricao', 'S', 'S', '', 70, 6, '','', 0, '', false, null, $pspdescricao) ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Status ativo:</td>
				<td>
					<input type="radio" name="pspstatus" value="t">Sim
					<input type="radio" name="pspstatus" value="f">Não
				</td>
			</tr>
			<tr class="SubTituloCentro" align="center">
				<td colspan="2">
					<input type="hidden" name="ppsid" value="<?php echo $ppsid ?>">
					<input type="submit" value="Gravar">
				</td>
			</tr>
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Lista dos itens de composição</b></td>
			</tr>		
			<?php if($arPareceres): ?>
			<tr>
				<td colspan="2">
				<table width="100%">
					<tr>
						<td width="100px" class="SubTituloEsquerda">Ações</td>
						<td class="SubTituloEsquerda">Descrição</td>
					</tr>
					<?php foreach($arPareceres as $parecer): ?>				
						<tr>
							<td>
								<img src="../imagens/alterar.gif">
								<img src="../imagens/excluir.gif">
							</td>
							<td>
								<?php  echo $parecer['pspdescricao'] ?>
							</td>
						</tr>				
					<?php endforeach; ?>
					</table>
				</td>
			</tr>
			<?php else: ?>
				<tr>
					<td colspan="2">Não existem itens de composição cadastrados</td>
				</tr>
			<?php endif; ?>
		</table>
	</form>
	</body>
</html>