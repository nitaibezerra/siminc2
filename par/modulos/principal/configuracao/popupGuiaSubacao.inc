<?php 

if($_POST['filtraForm'] == 1){
	$oConfiguracaoControle = new ConfiguracaoControle();
	$oConfiguracaoControle->filtraGuiaFormSubacao($_POST['frmid']);
	unset($oConfiguracaoControle);
	die();
}

$oPropostaSubacao 			= new PropostaSubacao();
$oCriterioPropostaSubacao 	= new CriterioPropostaSubacao();
if( $_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3 ){
	$oPropostaSubacaoEstados 	= new PropostaSubacaoEstados();
} else {
	$oPropostaSubacaoMunicipio 	= new PropostaSubacaoMunicipio();
}
$oPropostaSubacaoIdeb 		= new PropostaSubacaoIdeb();
$oPropostaSubacaoAnos		= new PropostaSubacaoAnos();

if($_POST['ppsdsc']){

	$oPropostaSubacao->ppsid 					  = $_POST['ppsid'] ? $_POST['ppsid'] : null;
	$oPropostaSubacao->ppsdsc 					  = $_POST['ppsdsc'] ? $_POST['ppsdsc'] : null;
	$oPropostaSubacao->ppsordem 				  = $_POST['ppsordem'] ? str_replace(array(".",","),"",$_POST['ppsordem']) : null;	
	$oPropostaSubacao->frmid 					  = $_POST['frmid'] ? $_POST['frmid'] : null;
	$oPropostaSubacao->ppsobjetivo 				  = $_POST['ppsobjetivo'] ? $_POST['ppsobjetivo'] : null;
	$oPropostaSubacao->prgid 					  = $_POST['prgid'] ? $_POST['prgid'] : null;
	$oPropostaSubacao->indid 					  = $_POST['indid'] ? $_POST['indid'] : null;
	$oPropostaSubacao->ppsestrategiaimplementacao = $_POST['ppsestrategiaimplementacao'] ? $_POST['ppsestrategiaimplementacao'] : null;
	$oPropostaSubacao->undid 					  = $_POST['undid'] ? $_POST['undid'] : null;
	$oPropostaSubacao->ppspeso 					  = $_POST['ppspeso'] ? $_POST['ppspeso'] : null;
	$oPropostaSubacao->foaid 					  = $_POST['foaid'] ? $_POST['foaid'] : null;
	$oPropostaSubacao->ppscarga					  = $_POST['ppscarga'];
	$oPropostaSubacao->ppsobra 					  = 1;
	$oPropostaSubacao->ppsptres 				  = 1;
	$oPropostaSubacao->ppscobertura				  = $_POST['ppscobertura'] ? $_POST['ppscobertura'] : null;
	$oPropostaSubacao->ppsnaturezadespesa		  = 1;
	$oPropostaSubacao->ppstexto					  = $_POST['ppstexto'] ? $_POST['ppstexto'] : null;
	$oPropostaSubacao->ppscronograma			  = $_POST['cronograma'] ? $_POST['cronograma'] : null;
	$oPropostaSubacao->ppsmonitora				  = $_POST['ppsmonitora'] ? $_POST['ppsmonitora'] : null;
	$oPropostaSubacao->ppscobertura				  = $_POST['ppscobertura'] ? $_POST['ppscobertura'] : null;
	$oPropostaSubacao->ptsid				      = $_POST['ptsid'] ? $_POST['ptsid'] : null;
	$ppsid = $oPropostaSubacao->salvar();
	
	$ppsid = is_numeric($ppsid) ? $ppsid : $_POST['ppsid'];	
	if($ppsid){
		$oCriterioPropostaSubacao->excluirPorPpsid($ppsid);
		if( $_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3 ){
			$oPropostaSubacaoEstados->excluirPorPpsid($ppsid);
		}else{
			$oPropostaSubacaoMunicipio->excluirPorPpsid($ppsid);
		}
		$oPropostaSubacaoIdeb->excluirPorPpsid($ppsid);
		$oPropostaSubacaoAnos->excluirPorPpsid($ppsid);
	}

	if($_POST['anos'] !== NULL){
		foreach($_POST['anos'] as $ano){
			$idAno = $oPropostaSubacaoAnos->recuperaDadosAnosPorAno($ano);
			if($idAno !== NULL){
				$oPropostaSubacaoAnos->psaid = null;
				$oPropostaSubacaoAnos->praid = $ano;
				$oPropostaSubacaoAnos->ppsid = $ppsid;
				$oPropostaSubacaoAnos->salvar();
			}
		}
	}
	
	foreach($_POST['pontuacao'] as $criterio){
		$oCriterioPropostaSubacao->cpsid = null;
		$oCriterioPropostaSubacao->crtid = $criterio; 
		$oCriterioPropostaSubacao->ppsid = $ppsid;
		$oCriterioPropostaSubacao->salvar();
		
	}
	
	if( $_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3 ){
		if($_POST['estados'][0]){
			foreach($_POST['estados'] as $estado){
				$oPropostaSubacaoEstados->pseid 	= null;
				$oPropostaSubacaoEstados->ppsid 	= $ppsid;
				$oPropostaSubacaoEstados->estuf 	= $estado; 
				$oPropostaSubacaoEstados->salvar();
			}
		}
	} else {
		if($_POST['municipios'][0]){
			foreach($_POST['municipios'] as $municipio){
				$oPropostaSubacaoMunicipio->psmid 	= null;
				$oPropostaSubacaoMunicipio->ppsid 	= $ppsid;
				$oPropostaSubacaoMunicipio->muncod 	= $municipio; 
				$oPropostaSubacaoMunicipio->salvar();
			}
		}
	}
//ver($oPropostaSubacaoEstados, d);	
	if($_POST['ideb'][0]){
		foreach($_POST['ideb'] as $ideb){
			$oPropostaSubacaoIdeb->psiid 	= null;
			$oPropostaSubacaoIdeb->ppsid 	= $ppsid;
			$oPropostaSubacaoIdeb->tpmid 	= $ideb; 
			$oPropostaSubacaoIdeb->salvar();
		}
	}
	
	if($_POST['grupo'][0]){
		foreach($_POST['grupo'] as $ideb){
			$oPropostaSubacaoIdeb->psiid 	= null;
			$oPropostaSubacaoIdeb->ppsid 	= $ppsid;
			$oPropostaSubacaoIdeb->tpmid 	= $ideb; 
			$oPropostaSubacaoIdeb->salvar();
		}
	}
	
	if($_POST['ppsid']){
		$sql = "UPDATE 
					par.subacao
				SET 
					frmid = {$_POST['frmid']},
					sbadsc = '{$_POST['ppsdsc']}',
					sbaestrategiaimplementacao = '{$_POST['ppsestrategiaimplementacao']}',
					sbacronograma = {$_POST['cronograma']},
					prgid = {$_POST['prgid']},
					sbaordem = {$_POST['ppsordem']},
					ptsid	= {$_POST['ptsid']}
				WHERE 
					ppsid = ".$_POST['ppsid'];
		
		$db->executar( $sql );
	}
	
	$db->executar("DELETE FROM par.propostasubacaoiniciativaemenda WHERE ppsid = $ppsid");
	if($_POST['iniid'][0]){
				
		foreach ($_POST['iniid'] as $iniid) {
			$sql = "INSERT INTO par.propostasubacaoiniciativaemenda(ppsid, iniid) 
					VALUES ($ppsid, $iniid)";
			$db->executar( $sql );	
		}
	}
	
	if($db->commit()){
		$oPropostaSubacao->carregarPorId($ppsid);		
		$arCriterios = $oCriterioPropostaSubacao->carregarPorPpsid($ppsid);
	}
	
}

$oConfiguracaoControle = new ConfiguracaoControle();

$indid = $_GET['indid'];
if($_GET['acaoGuia'] == 'editar'){

	$oPropostaSubacao->carregarPorId($_GET['ppsid']);			
	$indid = $oPropostaSubacao->indid;
	$ppsid = $_GET['ppsid'];
	$arCriterios = $oCriterioPropostaSubacao->carregarPorPpsid($ppsid); 
}

$arDados = $oConfiguracaoControle->recuperaDadosFormGuiaSubacao($indid);
$_SESSION['par']['itrid'] = $_REQUEST['itrid'];
?>
<?php $ptsid = $oPropostaSubacao->ptsid ?>
<html>
	<head>
		<title>PAR - Cadastro de subação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
		<script type="text/javascript">

			jQuery.noConflict();
			<?php $ppsid = $_GET['ppsid']; ?>
			function abrepopupMunicipio(){
				window.open('http://<?=$_SERVER['SERVER_NAME']?>/par/combo_municipios_proposta_subacao.php?ppsid=<?=$ppsid ?>','Municipios','width=400,height=400,scrollbars=1');
			}

			function abrepopupEstado(){
				window.open('http://<?=$_SERVER['SERVER_NAME']?>/par/par.php?modulo=principal/popupEstados&acao=A','Estados','width=400,height=400,scrollbars=1');
			}

			jQuery(document).ready(function(){
				
				//filtraTipoSubacao( jQuery('[name=frmid]').val());
				carregaAbasSubacao(jQuery('[name=ptsid]').val());
				
			});
			
			function filtraTipoSubacao(frmid)
			{
					
				if(frmid){
					jQuery.ajax({
						  type		: 'post',
						  url		: 'ajax.php',
						  <?php if($ppsid): ?>
						  data		: 'requisicao=guiaSubacao&filtraForm=1&frmid='+frmid+'&acaoGuia=editar&indid='+<?=$indid?>+'&ppsid='+<?=$ppsid?>,
						  <?php else: ?>
						  data		: 'requisicao=guiaSubacao&filtraForm=1&frmid='+frmid+'&indid='+<?=$indid?>,
						  <?php endif; ?>
						  success	: function(res) {
							jQuery('#formaExecucao').html(res);
						  }
					});

					if(frmid){
						jQuery.ajax({
							  type		: 'post',
							  url		: 'ajax.php',
							  data		: 'requisicao=filtraTipoSubAcao&frmid='+frmid + '&ptsid=<?php echo $ptsid ?>',
							  success	: function(res) {
							  	if(res){
									jQuery('#tr_combo_tiposubaacao').show();
									jQuery('[name=ptsid]').attr("class",'CampoEstilo obrigatorio');
									jQuery('#td_combo_tiposubaacao').html(res);
								}else{
									jQuery('[name=ptsid]').attr("class",'CampoEstilo');
									jQuery('#tr_combo_tiposubaacao').hide();
								}
							  }
						});
					}
					else{
						jQuery('[name=ptsid]').attr("class",'CampoEstilo');
						jQuery('#tr_combo_tiposubaacao').hide();
					}
				}
			}
			
		</script>
	</head>	
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
		<br />		
		<?php
		monta_titulo( 'Cadastro de subação (Guia) ', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
		?>
		<form action="" id="formulario" name="formulario" method="post">
			<input type="hidden" id="ppsid" name="ppsid" value="<?=($oPropostaSubacao->ppsid)?>" />
			<input type="hidden" id="indid" name="indid" value="<?=($oPropostaSubacao->indid) ? $oPropostaSubacao->indid : $_GET['indid']?>" />
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
				<tr bgcolor="#e9e9e9" align="center">
					<td colspan="2"><b>Forma de execução</b></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Forma de execução:</td>
					<td>
						<?php $frmid = $oPropostaSubacao->frmid ? $oPropostaSubacao->frmid : $_GET['frmid'] ?>
						<?php
						$sql = "SELECT 
									frmid as codigo, 
									frmdsc as descricao 
								FROM par.formaexecucao
								ORDER BY frmdsc";
 
						$db->monta_combo('frmid', $sql, 'S', 'Selecione...', 'filtraTipoSubacao','','','','S');
						?>
					</td>
				</tr>
				<?
					$displayTipoSubAcao = 'S';
					if(!$frmid || $frmid == '2' || $frmid == '4') $displayTipoSubAcao = 'N';
					
					$arrTipoSubacao = array();
					if($frmid){
						$sql = "SELECT
									ptsid as codigo,
									tps.tpsdescricao as descricao
								FROM
									par.propostatiposubacao pts
								
								INNER JOIN  par.tiposubacao tps on tps.tpsid = pts.tpsid
								WHERE
									frmid = $frmid
								AND
									ptsstatus = 'A'
								ORDER BY
									descricao";
						$arrTipoSubacao = $db->carregar($sql);
					}
					if($arrTipoSubacao){
						$diplay = "";
						$displayTipoSubAcao = "S";
					}else{
						$diplay = "none";
						$displayTipoSubAcao = "N";
					}
					
				?>
				<tr id="tr_combo_tiposubaacao" style="display: <?php echo $diplay;?>;">
					<td class="SubTituloDireita">Tipo da subação:</td>
					<td id="td_combo_tiposubaacao">
						<?php 
						if($arrTipoSubacao){
							$db->monta_combo('ptsid', $arrTipoSubacao, 'S', 'Selecione...', 'carregaAbasSubacao','', '','',$displayTipoSubAcao,'','',$ptsid);
						}
						?>
					</td>
				</tr>
				<tr>
					<td id="formExecucao">
						
					</td>
				</tr>		
				<!-- Aqui muda conforme forma de execução -->
				<?php if($_REQUEST['abaSubacao']): ?>
				<tr>
					<td colspan="2" id="formaExecucao" style="padding:0px;margin:0px;">	
						<?php $oConfiguracaoControle->filtraGuiaAbasSubacao($frmid); ?>
					</td>
				</tr>
				<?php else: ?>
				<tr>
					<td colspan="2" id="formaExecucao" style="padding:0px;margin:0px;">	
						<?php $oConfiguracaoControle->filtraGuiaFormSubacao($frmid, $ppsid); ?>
					</td>
				</tr>
				<?php endif; ?>
			</table>
		</form>
		<br />
	</body>
</html>