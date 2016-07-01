<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include_once APPRAIZ . 'includes/workflow.php';
validaSessionPTA( $_SESSION["emenda"]["ptrid"] );
if ( $_SESSION["emenda"]["ptrid"] ){
	$arPTA = $db->pegaLinha("SELECT resid, refid FROM emenda.planotrabalho WHERE ptrid = {$_SESSION["emenda"]["ptrid"]}");	
}

if( $_POST['requisicao'] == 'enviaanalise' ){
	$sql = "select docid from emenda.planotrabalho where ptrid = ".$_SESSION["emenda"]["ptrid"]." and ptrstatus = 'A'";
	$docid = $db->pegaUm( $sql );
	
	$sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";								
	$esdid = $db->pegaUm( $sql );
	
	$sql = "SELECT ed.esdid, ed.esddsc, ac.aeddscrealizada, * 
		    FROM workflow.historicodocumento hd
		    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
		    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
		    WHERE hd.docid = $docid
		    order by hd.htddata desc limit 1";	
	$estadoDestino = $db->pegaUm( $sql );
	
	if( $estadoDestino == EM_ANALISE_TECNICA || $estadoDestino == EM_DILIGENCIA ){
		$esdiddestino = EM_ANALISE_TECNICA;
		$link = 'analiseTecnicaPTA';
	} else {
		$esdiddestino = EM_ANALISE_DO_FNDE; #EM IDENTIFICAÇÃO DO PROCESSO
		$link = 'informacoesGerais';
	}
	
	$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
	$aedid = $db->pegaUm( $sql );
	
	if( $aedid && $docid ){
		$arDados = array( 'ptrid' => $_SESSION["emenda"]["ptrid"], 'resid' => $arPTA['resid'] , 'url' => $_SESSION['favurl'], 'refid' => $arPTA['refid'] );
		if(wf_alterarEstado( $docid, $aedid, '', $arDados )){
			if( $db->testa_superuser() ){
				echo "<script>
						alert('Plano de Trabalho enviado para análise com sucesso!');
						window.location.href = 'emenda.php?modulo=principal/$link&acao=A&ptridAnalise={$_SESSION["emenda"]["ptrid"]}';
					 </script>";
			} else {
				echo "<script>
						alert('Plano de Trabalho enviado para análise com sucesso!');
						window.location.href = 'emenda.php?modulo=principal/validacaoPTA&acao=A';
					 </script>";
			}
		} else {
			echo "<script>
					alert('Plano de Trabalho não enviado para análise!');
					window.location.href = 'emenda.php?modulo=principal/validacaoPTA&acao=A';
				 </script>";
		}
		exit();
	}
}
verificaPermissao();
validaSessionPTA( $_SESSION['emenda']['federal'] );
include APPRAIZ . 'emenda/classes/ValidacaoPlanoTrabalho.class.inc';
if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	!$_REQUEST['popup'] ? include APPRAIZ . 'includes/cabecalho.inc' : '';
	print "<br/>";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
exibeInstituicaoBenefiada();

$emendaFederal = $_SESSION['emenda']['federal'];

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

$boImpositivo = verificaOrcamentoImpositivo( $_SESSION["emenda"]["ptrid"] );
if( $boImpositivo ){
	$estadoWork = EM_ELABORACAO_IMPOSITIVO;
} else {
	$estadoWork = EM_ELABORACAO;	
}

!$_REQUEST['popup'] ? print "<br/>" : "";
!$_REQUEST['popup'] ? montaAbasPTA( $abacod_tela, $url, '', $estadoAtual ) : '';
!$_REQUEST['popup'] ? monta_titulo( 'Validação do Plano de Trabalho', 'Pendências') : '';

echo !$_REQUEST['popup'] ? cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]) : '';
echo "<br />";

if($_REQUEST['popup']){
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
}

$ptrid = ($_SESSION["emenda"]["ptrid"]) ? $_SESSION["emenda"]["ptrid"] : $_REQUEST["ptrid"];

$ptrid = !$ptrid ? $_SESSION["emenda"]["ptridAnalise"] : $ptrid;

if ( $ptrid ){
	$arPTA = $db->pegaLinha("SELECT resid, refid, ptrexercicio FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");	
}
validaSessionPTA( $ptrid );

// Recupera ou cria o docid.
$docid = criarDocumento( $ptrid );

$obj = new ValidacaoPlanoTrabalho($ptrid);
$obj->validacaoPlanoTrabalho();

// flag de erros
$existePendencias = false;

//$ptrimprimirpta = $db->pegaUm("SELECT ptrimprimirpta FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");

?>

<script language="JavaScript" src="../../includes/funcoes.js"></script>
	<?php 
		/*if( $ptrimprimirpta != 'f' ){
			//echo abrePTAconsolidado($ptrid, '', 'convenio');
		}*/ 
	?>
<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
 <tbody>
	<tr>
		<td width="10%" style="text-align: left;" class="SubTituloEsquerda" <?=$estadoAtual == $estadoWork ? 'colspan="2"' : '' ?>>Pendências existentes no Plano de Trabalho</td>
		<?if( $estadoAtual != $estadoWork){ ?>
		<td	width="90%" style="text-align: center;" class="SubTituloEsquerda">Enviar PTA</td>
		<?} ?>
	</tr>
	<tr>
		<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<br />
			<table style="border-color:#ffffff;" align="center" class="Tabela" cellpadding="10" cellspacing="0">
			 <tbody>
			 	<tr>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita" colspan="2">
						<b><font size="2">Legenda:</font></b>
						<br />
						<img src="../imagens/valida2.gif" /> Existem pendências nos dados informados na aba.
						<br />
						<img src="../imagens/valida1.gif" /> Dados validados e em situação correta.
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
						ABA
					</td>
					<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
						PENDÊNCIAS
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Recursos:</b>
						<?= ((!empty($obj->retorno["recursospta"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["recursospta"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["recursospta"]); $i++) {
								echo '- ' . $obj->retorno["recursospta"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<!--  <tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Orçamento Impositivo:</b>
						<?= ((!empty($obj->retorno["impositivo"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["impositivo"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["impositivo"]); $i++) {
								echo '- ' . $obj->retorno["impositivo"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>-->
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Iniciativas:</b>
						<?= ((!empty($obj->retorno["iniciativas"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>	
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["iniciativas"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["iniciativas"]); $i++) {
								echo '- ' . $obj->retorno["iniciativas"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Cronograma de Execução e Desembolso:</b>
						<?= ((!empty($obj->retorno["cronograma"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["cronograma"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["cronograma"]); $i++) {
								echo '- ' . $obj->retorno["cronograma"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<?if( $emendaFederal == 'N' ){ ?>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Cadastro de Escolas Beneficiadas:</b>
						<?= ((!empty($obj->retorno["escolasbeneficiadas"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["escolasbeneficiadas"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["escolasbeneficiadas"]); $i++) {
								echo '- ' . $obj->retorno["escolasbeneficiadas"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<?} ?>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Dados Adicionais:</b>
						<?= ((!empty($obj->retorno["dadosadicionais"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["dadosadicionais"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["dadosadicionais"]); $i++) {
								echo '- ' . $obj->retorno["dadosadicionais"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<?
				$boGuia = $db->pegaUm("SELECT count(gp.gptid) FROM emenda.guia_planotrabalho gp WHERE gp.ptrid = {$_SESSION['emenda']['ptrid']}");
				
				if( $emendaFederal == 'S' || $boGuia > 0 ){ ?>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Anexos:</b>
						<?= (( (!empty($obj->retorno["anexo"]) && $emendaFederal == 'S') || !empty($obj->retorno["anexoguia"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
					$boPede = false;
					if( $emendaFederal == 'S' ){
						if(!empty($obj->retorno["anexo"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["anexo"]); $i++) {
								echo '- ' . $obj->retorno["anexo"][$i] . '<br />';
							}
						} else {
							if( !$boPede ){
								echo '- ' . "Não existem pendências.";
								$boPede = true;
							}
						}
					}
					if( $boGuia > 0 ){
						if(!empty($obj->retorno["anexoguia"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["anexoguia"]); $i++) {
								echo '- ' . $obj->retorno["anexoguia"][$i] . '<br />';
							}
						} else {
							if( !$boPede ){
								echo '- ' . "Não existem pendências.";
								$boPede = true;
							}
						}
					}
					?>
					</td>
				</tr>
				<?} 
				if( $emendaFederal == 'S' ){
				?>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Entidade:</b>
						<?= ((!empty($obj->retorno["entidade"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["entidade"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["entidade"]); $i++) {
								echo '- ' . $obj->retorno["entidade"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<?}
				$sql = "SELECT anasituacaoparecer, anaparecer, anadisponibilizardiligencia FROM emenda.analise 
						WHERE ptrid = $ptrid and anatipo = 'T' and analote = (SELECT max(analote)  FROM emenda.analise 
												                        WHERE anatipo = 'T' and anastatus = 'A' and ptrid = $ptrid)";
				$arAnalise = $db->pegaLinha( $sql );
										
				if( $arAnalise['anadisponibilizardiligencia'] == 'S' ){ ?>
					<tr>
						<td colspan="2" width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
							PARECER TÉCNICO DO FNDE - Em Diligência
						</td>
					</tr>
					<tr>
						<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
							<b>Análise Técnica do FNDE:</b>
						</td>
						<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
						<?
							echo $arAnalise['anaparecer'];
						?>
						</td>
					</tr><?
				}
				if( $boImpositivo < 1 ){
					$docid =  $db->pegaUm("SELECT docid FROM emenda.planotrabalho WHERE ptrid = $ptrid");
					
					$sql = "SELECT ed.esdid
						    FROM workflow.historicodocumento hd
						    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
						    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
						    WHERE hd.docid = $docid 
				            	and hd.hstid = (select max(hstid) from workflow.historicodocumento where docid = $docid)
						    order by hd.hstid asc";
					
					$esdidAnterior = $db->pegaUm($sql);
										    
					$boMostraWork = false;				
					if( $estadoAtual == $estadoWork && !$existePendencias && $arPTA['ptrexercicio'] == $_SESSION['exercicio_atual'] ){ ?>
						<tr>
							<td width="100" style="border:solid 1px;" class="SubTituloCentro" colspan="2">
								<b>A elaboração do Plano de Trabalho foi concluída com sucesso!<br>
								Para da prosseguimento ao feito, é necessário enviá-lo para análise do concedente.<br>
								Deseja enviá-lo agora?<br></b>
								<input type="radio" name="radioenvio" id="radsim" onclick="validaEnvioAnalise('S');" value="S"> Sim
								<input type="radio" name="radioenvio" id="radnao" onclick="validaEnvioAnalise('N');" value="N"> Não
							</td>
						</tr>
					<?} else {
						if( $estadoAtual == $estadoWork && $arPTA['ptrexercicio'] != $_SESSION['exercicio_atual'] ){
							if( $db->testa_superuser() || !empty($esdidAnterior) ){
								$boMostraWork = true;
							} else {
								echo '<tr><td colspan="2" style="color: red">';
								echo '<font size="2"><b>O prazo para elaboração do plano de trabalho expirou em 31/12. <br>Não é possível enviar para análise do FNDE!</b></font>';
								echo '</td></tr>';
							}
						}
					}
				}
				?>
			 </tbody>
			</table>
		</td>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
		<?
//			if($existePendencias) {
//				echo "<font color='red'>Não é possível enviar seu Plano de Trabalho devido às pendências listadas ao lado.</font>";
//			}
//			else if(pegarEstadoAtual( $ptrid ) != $estadoWork) {
//				echo "<font color='red'>O seu Plano de Trabalho já foi enviado para análise.</font>";
//			}
//			else if( $docid ) {
//				// barra de estado atual,ações e historico
//				wf_desenhaBarraNavegacao( $docid , array( 'ptrid' => $ptrid, 'resid' => $resid , 'obj' => $obj ) );
//			}
			$boHistorico = null;
			if( in_array( INSTITUICAO_BENEFICIADA, pegaPerfilArray( $_SESSION['usucpf'] ) ) ){
				$boHistorico = array('historico' => true);
			} else {
				$boHistorico = array();
			}
			if( $estadoAtual != $estadoWork || $boMostraWork || ($boImpositivo > 0) )
				wf_desenhaBarraNavegacao( $docid , array( 'ptrid' => $ptrid, 'resid' => $arPTA['resid'] , 'url' => $_SESSION['favurl'], 'tipo' => '', 'refid' => $arPTA['refid'] ), $boHistorico );
				
				
			$arFluxo = verificaFluxo( $ptrid );
				
			if( $arFluxo ){
				$fluxo = implode(', ', $arFluxo);
			}
			
		?>
		</td>
	</tr>
 </tbody>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
</form>
<script>
jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
		//$('#td_acao_445').css('display', 'none');
	});
});

function validaEnvioAnalise( radio ){
	if( radio == 'S' ){
		if( confirm('Deseja realmente enviar o plano de trabalho para análise') ){
			document.getElementById('radsim').checked = false;
			document.getElementById('requisicao').value = 'enviaanalise';
			document.getElementById('formulario').submit();
		}
		document.getElementById('radsim').checked = false;
	} else if( radio == 'N' ){
		alert('Plano de Trabalho não enviado para análise!');
		document.getElementById('radnao').checked = false;
		return false;
	}
}
</script>