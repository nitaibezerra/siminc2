<?php

$oConfiguracaoControle 	= new ConfiguracaoControle();
$oInstrumento 			= new Instrumento();
$oDimensao 				= new Dimensao();
$oArea 					= new Area();
$oIndicador 			= new Indicador();
$oCriterio 				= new Criterio();
$oPropostaAcao 			= new PropostaAcao();

$arCampos = array();

$boInstrumento 	= 'N';
$boDimensao 	= 'N';
$boArea 		= 'N';
$boIndicador 	= 'N';
$boCriterio 	= 'N';
$boAcao		 	= 'N';

switch($_GET['tipoGuia']){
	
	case 'dimensao':
		$oConfiguracaoControle->recuperaDadosFormGuiaDimensao($_GET['codigo'], $boDimensao, $stTitulo, $itrdsc, $itrid, $dimid, $dimdsc, $ordcod, $arCampos);						
		break;
	case 'area':
		$oConfiguracaoControle->recuperaDadosFormGuiaArea($_GET['codigo'], $boArea, $stTitulo, $itrdsc, $dimdsc, $dimid, $areid, $aredsc, $ordcod, $arCampos);		
		break;
	case 'indicador':
		$oConfiguracaoControle->recuperaDadosFormGuiaIndicador($_GET['codigo'], $boIndicador, $stTitulo, $itrdsc, $dimdsc, $aredsc, $areid, $indid, $inddsc, $ordcod, $arCampos);	
		break;
	case 'criterio':
		$oConfiguracaoControle->recuperaDadosFormGuiaCriterio($_GET['codigo'], $boCriterio, $stTitulo, $itrdsc, $dimdsc, $aredsc, $inddsc, $indid, $crtid, $crtdsc, $crtpontuacao, $crtpeso, $arCampos);
		break;
	case 'acao':
		$oConfiguracaoControle->recuperaDadosFormGuiaAcao($_GET['codigo'], $boAcao, $stTitulo, $itrdsc, $dimdsc, $aredsc, $inddsc, $crtdsc, $crtid, $ppaid, $ppadsc, $arCampos);
		break;
	
}

if($_POST['tipo'] == 'dimensao'){
	
	$dimid = $_POST['dimid'] ? $_POST['dimid'] : null;	
	
	$oDimensao->dimid 		= $dimid;
	$oDimensao->dimcod 		= $_POST['ordcod'];
	$oDimensao->dimdsc 		= $_POST['dimdsc'];
	$oDimensao->dimstatus 	= 'A';
	$oDimensao->itrid 		= $_POST['itrid'];
	$oDimensao->salvar();
	$oDimensao->commit();
	
	echo "<script>window.close();window.opener.location.reload();</script>";
	exit();
}

if($_POST['tipo'] == 'area'){
	
	$areid = $_POST['areid'] ? $_POST['areid'] : null;
	
	$oArea->areid 		= $areid;
	$oArea->arecod 		= $_POST['ordcod'];
	$oArea->aredsc 		= $_POST['aredsc'];
	$oArea->arestatus 	= 'A';
	$oArea->dimid 		= $_POST['dimid'];
	$oArea->salvar();
	$oArea->commit();
	
	echo "<script>window.close();window.opener.location.reload();</script>";
	exit();
}

if($_POST['tipo'] == 'indicador'){	
	
	$indid = $_POST['indid'] ? $_POST['indid'] : null;
	
	$oIndicador->indid 		= $indid;
	$oIndicador->indcod 	= $_POST['ordcod'];
	$oIndicador->inddsc 	= $_POST['inddsc'];
	$oIndicador->indstatus 	= 'A';
	$oIndicador->areid 		= $_POST['areid'];
	$oIndicador->salvar();
	$oIndicador->commit();
	
	echo "<script>window.close();window.opener.location.reload();</script>";
	exit();
}

if($_POST['tipo'] == 'criterio'){
	
	$crtid = $_POST['crtid'] ? $_POST['crtid'] : null;
	
	$nrPeso		 = 100-($oCriterio->verificaPesoPontuacao($_POST['indid']));
	$arPontuacao = $oCriterio->verificaPontuacao($_POST['indid'], $crtid);
	$arPontuacao = $arPontuacao ? $arPontuacao : array();
	
	foreach($arPontuacao as $pontuacao){
		$arPto[] = $pontuacao['peso'];	
	}
	
	$arPto = ($arPto) ? $arPto : array();
		
	if(in_array($_POST['crtpontuacao'], $arPto)){
		
		alert("Pontuação já cadastrada!");
		
		$indid 		= $_POST['indid'];
		$crtdsc 	= $_POST['crtdsc'];
		$crtpeso 	= $_POST['crtpeso'];
		
	}elseif($_POST['crtpeso'] > $nrPeso){
		
		alert("Valor do peso maior que o restante {$nrPeso}.");
		
		$indid 			= $_POST['indid'];
		$crtdsc 		= $_POST['crtdsc'];
		$crtpontuacao 	= $_POST['crtpontuacao'];
		
	}else{
	
		$oCriterio->crtid 			= $crtid;
		$oCriterio->indid 			= $_POST['indid'];
		$oCriterio->crtpontuacao	= $_POST['crtpontuacao'];
		$oCriterio->crtdsc 			= $_POST['crtdsc'];
		$oCriterio->crtpeso 		= $_POST['crtpeso'];
		$oCriterio->salvar();
		$oCriterio->commit();
		
		echo "<script>window.close();window.opener.location.reload();</script>";
		exit();
	}
}

if($_POST['tipo'] == 'acao'){
	//ver($_POST,d);
	$ppaid = $_POST['ppaid'] ? $_POST['ppaid'] : null;
	
	if($_POST['crtid'] && $_POST['acao'] == 'incluir'){
		
		$arAcoes = $oPropostaAcao->recuperarPropostaAcoesGuia($_POST['crtid']);
		if($arAcoes){
			alert('Somente uma ação por critério!');
			echo "<script>window.close();window.opener.location.reload();</script>";
			exit();
		}
	}
	
	$oPropostaAcao->ppaid 			= $ppaid;
	$oPropostaAcao->crtid 			= $_POST['crtid'];
	$oPropostaAcao->ppadsc			= $_POST['ppadsc'];
	$oPropostaAcao->salvar();
	$oPropostaAcao->commit();
	
	echo "<script>window.close();window.opener.location.reload();</script>";
	exit();
}

?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script src="../includes/funcoes.js" type="text/javascript"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script src="../includes/jquery-validate/jquery.validate.js" type="text/javascript"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
		<script type="text/javascript">
		<!--
				
			$(document).ready(function(){
				/*$(window).bind("beforeunload", function(){
					window.opener.location.reload(); 
				});*/
				
				//validação
				$(".formulario").validate({
					//ignoreTitle: true,
					rules: {
						dimdsc: "required",
						aredsc: "required",
						inddsc: "required",
						ordcod: "required",
						crtpontuacao: "required",
						crtdsc: "required",
						crtpeso: "required",
						ppadsc: "required"					   
					}				
														
				});

				$('#btFechar').click(function(){
					window.opener.location.reload();
					window.close();
				});
				
			});
			
		//-->
		</script>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" unload="alert(44)">
		<form action="" method="post" name="formulario" id="formulario" class="formulario">
			<?php
			print '<br />';
			monta_titulo( 'Manutenção '.$stTitulo, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
				align="center">
				<tr>
					<td class="SubTituloDireita" align="right">Instrumento:</td>
					<td>
						<?= campo_texto( 'itrdsc', 'S', $boInstrumento, '', 65, 500, '', '' ); ?>
					</td>
				</tr>	
				<tr>
					<td class="SubTituloDireita" align="right">Dimensão:</td>
					<td>
						<?= campo_texto( 'dimdsc', 'S', $boDimensao, '', 65, 500, '', '', '', '', '', 'id="dimdsc" title="O campo dimensão é obrigatório."' ); ?>
					</td>
				</tr>
				<?php if(in_array('area', $arCampos)): ?>
				<tr>
					<td class="SubTituloDireita" align="right">Área:</td>
					<td>
						<?= campo_texto( 'aredsc', 'S', $boArea, '', 65, 500, '', '','','','','title="O campo área é obrigatório."' ); ?>
					</td>
				</tr>
				<?php				 
				endif;				 
				if(in_array('indicador', $arCampos)): 
				?>
				<tr>
					<td class="SubTituloDireita" align="right">Indicador:</td>
					<td>
						<?= campo_texto( 'inddsc', 'S', $boIndicador, '', 65, 500, '', '','','','','title="O campo indicador é obrigatório."' ); ?>
					</td>
				</tr>
				<?php 
				endif;
				if($_GET['tipoGuia'] != 'criterio' && $_GET['tipoGuia'] != 'acao'): 
				?>
				<tr>
					<td class="SubTituloDireita" align="right">Ordem:</td>
					<td>
						<?= campo_texto( 'ordcod', 'S', 'S', '', 5, 10, '', '','','','','title="O campo ordem é obrigatório."' ); ?>
					</td>
				</tr>	
				<?php
				endif; 
				if(in_array('criterio', $arCampos)): 
				?>
				<tr>
					<td class="SubTituloDireita" align="right">Pontuação:</td>
					<td>
						<?= campo_texto( 'crtpontuacao', 'S', 'S', '', 5, 10, '', '','','','','title="O campo pontuação é obrigatório."' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Critério:</td>
					<td>
						<?= campo_textarea( 'crtdsc', 'S', 'S', '', 65, 5, '', '','','','','title="O campo critério é obrigatório."' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Peso:</td>
					<td>
						<?= campo_texto( 'crtpeso', 'S', 'S', '', 5, 10, '', '','','','','title="O campo peso é obrigatório."' ); ?>
					</td>
				</tr>
				<?php endif; ?>
				<?php if(in_array('acao', $arCampos)): ?>
				<tr>
					<td class="SubTituloDireita" align="right">Critério:</td>
					<td>
						<?= campo_texto( 'crtdsc', 'S', $boCriterio, '', 65, 2000, '', '','','','','title="O critério área é obrigatório."' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" align="right">Ação:</td>
					<td>
						<?= campo_texto( 'ppadsc', 'S', $boAcao, '', 65, 2000, '', '','','','','title="O campo ação é obrigatório."' ); ?>
					</td>
				</tr>
				<?php endif; ?>						
				<tr bgcolor="#C0C0C0">
					<td>&nbsp;</td>
					<td>						
						<?php if($_GET['acaoGuia'] == 'incluir'){ ?>
						<input type='submit' name='incluir' value='Salvar' />
						<!-- input type='button' name='incluir' value='Salvar' onclick="validar()" / -->
						<?php }else{ ?>
						<input type='submit' name='alterar' value='Alterar' />
						<!-- input type='button' name='alterar' value='Alterar' onclick="validar()" / -->
						<?php } ?>
						<input type='button' value='Fechar' id='btFechar' name='btFechar' onclick='fecharJanela()' />
						<input type='hidden' value='<?php echo $itrid;?>' id='itrid' name='itrid'/>
						<input type='hidden' value='<?php echo $dimid;?>' id='dimid' name='dimid'/>
						<input type='hidden' value='<?php echo $areid;?>' id='areid' name='areid'/>
						<input type='hidden' value='<?php echo $indid;?>' id='indid' name='indid'/>
						<input type='hidden' value='<?php echo $crtid;?>' id='crtid' name='crtid'/>
						<input type='hidden' value='<?php echo $ppaid;?>' id='ppaid' name='ppaid'/>
						<input type='hidden' value='<?php echo $_GET['acaoGuia'];?>' id='acao' name='acao'/>
						<input type='hidden' value='<?php echo $_GET['tipoGuia'];?>' id='tipo' name='tipo'/>										
					</td>			
				</tr>
			</table>
		</form>	
	</body>
</html>