<?php 

$oPropostaSubacao 			= new PropostaSubacao();

if($_POST['requisicao'] == "gravaritemcomposicao") {

	if( $_POST['picid'] ){
		$sql = "INSERT INTO par.propostasubacaoitem(
	            picid, ppsid)
	    		VALUES ('".$_POST['picid']."', '".$_REQUEST['ppsid']."');";
	}
	
	if( $_POST['gicid'] ){
		$sql = "INSERT INTO par.propostagrupoitem(
	            gicid, ppsid)
	    		VALUES ('".$_POST['gicid']."', '".$_REQUEST['ppsid']."');";
	}
	
	if( $sql ){
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Gravado com sucesso');
			window.location='par.php?modulo=principal/configuracao/popupGuiaSubacaoItemComposicao&acao=A&acaoGuia=editar&ppsid=".$_REQUEST['ppsid']."';
		 </script>");
}

if($_REQUEST['requisicao'] == "removervinculo") {
	
	if( $_REQUEST['tipo'] == 'item' ){
		$sql = "DELETE FROM par.propostasubacaoitem WHERE picid='".$_REQUEST['id']."' AND ppsid='".$_REQUEST['ppsid']."'";
	}
	if( $_REQUEST['tipo'] == 'grupo' ){
		$sql = "DELETE FROM par.propostagrupoitem WHERE gicid='".$_REQUEST['id']."' AND ppsid='".$_REQUEST['ppsid']."'";
	}
	
	if( $sql ){
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Removido com sucesso');
			window.location='par.php?modulo=principal/configuracao/popupGuiaSubacaoItemComposicao&acao=A&acaoGuia=editar&ppsid=".$_REQUEST['ppsid']."';
		 </script>");
}


$oConfiguracaoControle = new ConfiguracaoControle();

if($_GET['acaoGuia'] == 'editar'){
	
	$oPropostaSubacao->carregarPorId($_GET['ppsid']);
	$indid = $oPropostaSubacao->indid;
	$frmid = $oPropostaSubacao->frmid;
	$ppsid = $_GET['ppsid'];
	
	$sql = "SELECT DISTINCT
				   '<center><img src=../imagens/excluir.gif style=cursor:pointer; onclick=retirarVinculo(\''||pic.picid||'\',\''||psi.ppsid||'\',\'item\')></center>' as acao, 
				   pic.picdescricao
			--	   CASE WHEN ptp.ptpnumeroata IS NOT NULL THEN 'ata '||ptp.ptpnumeroata ELSE '-' END as pregao,
			--	   to_char(dic.dicdatainicial, 'YYYY') as anoini,
			--	   to_char(dic.dicdatafinal, 'YYYY') as anofim 
			FROM par.propostaitemcomposicao pic
			LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A'
			LEFT JOIN par.propostasubacaoitem psi ON psi.picid = pic.picid
			LEFT JOIN par.propostatipopregao ptp ON ptp.ptpid = dic.ptpid AND ptpstatus = 'A'
			LEFT JOIN par.pregaouf p   ON p.ptpid   = ptp.ptpid
			WHERE psi.ppsid = '".$_GET['ppsid']."'
			AND pic.picstatus = 'A'
			ORDER BY pic.picdescricao";

	$arrItensComposicao_ = $db->carregar($sql);

	if($arrItensComposicao_[0]) {
		foreach($arrItensComposicao_ as $key => $arrIC) {
			$arrItensComposicao[$key]['acao'] = $arrIC['acao'];
	//		$arrItensComposicao[$key]['picdescricao'] = utf8_decode($arrIC['picdescricao']);
			$arrItensComposicao[$key]['picdescricao'] = $arrIC['picdescricao'];
		//	$arrItensComposicao[$key]['pregao'] = $arrIC['pregao'];
			
		//	unset($anos);
		//	for($i=$arrIC['anoini'];$i<=$arrIC['anofim'];$i++) {
		//		$anos[] = $i;
		//	}

		//	if($anos) $arrItensComposicao[$key]['anos'] = implode(", ",$anos);
		}
	}
	
	// Grupos
	$sql = "SELECT DISTINCT
				   '<center><img src=../imagens/excluir.gif style=cursor:pointer; onclick=retirarVinculo(\''||gic.gicid||'\',\''||pgi.ppsid||'\',\'grupo\')></center>' as acao, 
				   gic.gicdescricao
			FROM par.grupo_itemcomposicao gic
			LEFT JOIN par.propostagrupoitem pgi ON pgi.gicid = gic.gicid
			WHERE pgi.ppsid = '".$_GET['ppsid']."'
			AND gic.gicstatus = 'A'
			ORDER BY gic.gicdescricao";

	$arrGrupo_ = $db->carregar($sql);

	if($arrGrupo_[0]) {
		foreach($arrGrupo_ as $key => $arrGr) {
			$arrGrupo[$key]['acao'] = $arrGr['acao'];
			$arrGrupo[$key]['gicdescricao'] = $arrGr['gicdescricao'];
		}
	}

}

$arDados = $oConfiguracaoControle->recuperaDadosFormGuiaSubacao($indid);

?>
<html>
	<head>
		<title>PAR - Ligação item composição a subação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css' />
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript">
			function submeterVinculo() {
			
				var erro = 0;
				if(document.getElementById('picid').value == '') {
					erro = erro + 1;
//					alert('Selecione um item de composição');
//					return false;
				}
				if(document.getElementById('gicid').value == '') {
					erro = erro + 1;
//					alert('Selecione um Grupo item de composição');
//					return false;
				}
				
				if( erro == 2 ){
					alert('Você precisa selecionar um Item ou um Grupo para Gravar.');
					return false;
				}
				if( erro == 0 ){
					alert('Você precisa selecionar ou um Item ou um Grupo para Gravar.');
					return false;
				}

				document.getElementById('formulario').submit();
			}
					
			function retirarVinculo(id, ppsid, tipo) {
				var conf = confirm('Deseja remover o vículo da subação?');
				if(conf) {
					window.location='par.php?modulo=principal/configuracao/popupGuiaSubacaoItemComposicao&acao=A&requisicao=removervinculo&ppsid='+ppsid+'&id='+id+'&tipo='+tipo;
				}
			}

			jQuery.noConflict();

			jQuery(document).ready(function(){
	
			});
			
		</script>
	</head>	
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
		<br />		
		<?php
		monta_titulo( 'Ligação item composição (Guia) ', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
		?>
		<form action="" id="formulario" name="formulario" method="post">
		<input type="hidden" name="requisicao" value="gravaritemcomposicao">
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
		<br/>
		<?php print carregaAbasPropostaSubacao("par.php?modulo=principal/configuracao/popupGuiaSubacaoItemComposicao&acao=A&acaoGuia=editar&ppsid={$_GET['ppsid']}", $oPropostaSubacao->frmid, $_GET['ppsid'],$oPropostaSubacao->indid,$oPropostaSubacao->ptsid); ?>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr bgcolor="#e9e9e9" align="center">
					<td colspan="2"><b>Vinculação da subação com itens de composição</b></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width:250px;">Itens de composição:</td>
					<td>
					<?php
					$sql = "SELECT distinct pc.picid as codigo, pc.picdescricao as descricao FROM par.propostasubacao p 
							INNER JOIN par.propostaitemtiposubacao pi ON p.ptsid = pi.ptsid 
							INNER JOIN par.propostaitemcomposicao pc ON pc.picid = pi.picid and pc.picstatus = 'A'  
							--LEFT JOIN par.propostasubacaoitem ps ON ps.picid = pc.picid AND ps.ppsid IS NULL
							WHERE p.ppsid='".$_REQUEST['ppsid']."'
							AND ppsid NOT IN ( SELECT ps.ppsid FROM par.propostasubacaoitem ps WHERE ps.picid = pc.picid)
							AND picstatus = 'A'
							ORDER BY pc.picdescricao";

					$db->monta_combo('picid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'picid'); 
					?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width:250px;">Grupo de Itens de composição:</td>
					<td>
					<?php
					$sql = "SELECT 
								gic.gicid as codigo, 
								gic.gicdescricao as descricao
							FROM 
								par.grupo_itemcomposicao gic
							INNER JOIN par.propostagrupotiposubacao pgs ON pgs.gicid = gic.gicid 
							INNER JOIN par.propostasubacao p ON p.ptsid = pgs.ptsid AND p.ppsid='".$_REQUEST['ppsid']."'
							WHERE  
								gic.gicstatus = 'A'
								AND ppsid NOT IN ( SELECT pg.ppsid FROM par.propostagrupoitem pg WHERE pg.gicid = gic.gicid)
							ORDER BY 
								gic.gicdescricao";

					$db->monta_combo('gicid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'gicid'); 
					?></td>
				</tr>
				<tr>
					<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="submeterVinculo();"></td>
				</tr>
				<tr bgcolor="#e9e9e9" align="center">
					<td colspan="2"><b>Lista de itens de composição</b></td>
				</tr>
			</table>
		</form>
		<?php $cabecalho = array("Ação","Descrição") ?>
		<?php $db->monta_lista_array($arrItensComposicao,$cabecalho,30,5,"N","center"); ?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Lista de Grupos de itens de composição</b></td>
			</tr>
		</table>
		<?php $db->monta_lista_array($arrGrupo,$cabecalho,30,5,"N","center"); ?>
	</body>
</html>