<?php 

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

$preid  = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];
$qrpid = pegaQrpidAnalisePAC( $preid, 49 );

$muncod = $_SESSION['par']['muncod'];
$docid  = prePegarDocid($preid);
$esdid  = prePegarEstadoAtual($docid);
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

if ( $_POST['atualizaBarraNavegacao'] ){
	die( wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) ) );
}

$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=AnaliseEngenheiro".$preidTx;

echo carregaAbasPronatec($lnkabas);
monta_titulo( 'Analise Instituto Federal', ''  );

$obPreObraControle = new PreObraControle();

$arDados 		= $obPreObraControle->recuperarDadosAnaliseEngenharia($preid);
$arRespostas 	= $obPreObraControle->recuperarRespostasQuestionario($qrpid);
$arPreAnalise 	= $obPreObraControle->recuperarDadosPorPreid($preid);

$municipio = $obPreObraControle->pegaMuncodPorPreid($preid);

$consideracoesFinais = $obPreObraControle->recuperarConsideracoesFinais($preid);

$muncodIBGE 	= substr($municipio['muncod'], 0, 6);
$mundescricao 	= str_replace(" ","%20", $municipio['mundescricao']);

?>
<script>
	jQuery(document).ready(function(){

//		jQuery('.mostra').click(function(){			
//			arDados = this.id.split("_");	
//
//			return window.open('par.php?modulo=principal/programas/proinfancia/questionarioImpressao&acao=A&preid='+arDados[0]+'&muncod='+arDados[1]+'&qrpid='+arDados[2]+'&queid='+arDados[3], 
//					   'questionarioImpressao',
//					   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
//		});
				
	});

</script>
<?php 

$poaid = ($arPreAnalise['poaid']) ? $arPreAnalise['poaid'] : $id;  

?>
<!-- habilita o tiny -->
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	theme : "advanced",
	mode: "specific_textareas",
	editor_selector : "text_editor_simple",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,link,bold,italic,underline,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	width : "450px",
	entity_encoding : "raw"
	});
</script>
<?php 
	if($poaid){
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="" style="text-align:center;">
				<a style="cursor: pointer;" onclick="window.open('http://www.ibge.gov.br/cidadesat/xtras/perfilwindow.php?nomemun=<?php echo $mundescricao ?>&amp;codmun=<?php echo $muncodIBGE ?>&amp;r=2','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_ibge.png" border="0"></a> 
				<a style="cursor: pointer;" onclick="window.open('http://portal.mec.gov.br/ide/2008/gerarTabela.php?municipio=<?php echo $_SESSION['par']['muncod'] ?>','Indicadores','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_demograficos.png" border="0"></a>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloesquerda" style="text-align:center;">IDENTIFICAÇÃO</td>
		</tr>
		<tr>
			<td width="150" class="subtitulodireita">Análise N.º:</td>
			<td>
				<?php echo $poaid ?>
			</td>
		</tr>
		<tr>
			<td width="150" class="subtitulodireita">N.º de identificação:</td>
			<td>
				<?php echo $arDados['preid'] ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Interessado:</td>
			<td>
				Prefeitura Municipal de <?php echo $arDados['mundescricao'] ?> - <?php echo $arDados['estuf'] ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Tipo de obra:</td>
			<td>
				<?php echo $arDados['ptodescricao'] ?>
			</td>
		</tr>	
	<?php 
	
	if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || 
		in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		in_array(PAR_PERFIL_CONSULTA, $perfil) || 
		in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||  
		in_array(PAR_PERFIL_COORDENADOR_TECNICO, $perfil)){ 
			 
		if(($esdid == WF_PRONATEC_EM_ANALISE_FNDE ) || ($esdid == WF_PRONATEC_EM_REVISAO_DE_ANALISE && $arPreAnalise['poaindeferido'] == 'N' )){ ?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">CONSIDERAÇÕES INICIAIS</td>
			</tr>
			<tr>
				<td colspan="2" class="" style="text-align:center;">
				<table>
					<tr><td align="center" width="100%">			
					<form action="" method="post" id="formulario" name="formulario">
						Com base nos documentos apresentados, fotos, estudo de demanda, planta de localização, é justificável a construção da obra no local?
						<br/>
						<?php 
							$boHabilitado = 'disabled="disabled"';
						?>
						<input type="radio" name="poaindeferido" value="S" <?=$arPreAnalise['poaindeferido'] == 'S' ? 'checked' : '' ?> <?=$boHabilitado ?>><label>Sim.</label>
						<input type="radio" name="poaindeferido" value="N" <?=$arPreAnalise['poaindeferido'] == 'N' ? 'checked' : '' ?> <?=$boHabilitado ?>><label>Não.</label>
						<div class="poajustificativa" style="display:<?=($arPreAnalise['poaindeferido'] == 'S' || $arPreAnalise['poaindeferido'] == '' ) ? 'none' : '' ?>;"  style="text-align:left;">
							<table align="center"><tr><td>Justificativa:<br>
							<textarea name="poajustificativa" class="text_editor_simple" cols="50" rows="8"><?=$arPreAnalise['poajustificativa']?></textarea>
							</td></tr></table>
						</div>
						<input type="hidden" name="poaid" value="<?php echo $poaid ?>">
						<br/>
						
					</form>
					</td><td valign="top">
					</td></tr>
				</table>
				</td>
			</tr>
			<tr bgcolor="#c0c0c0">
				<td></td>
				<td><input class="enviaForm" type="button" value="Salvar" <?=$boHabilitado ?>></td>
			</tr>
		<?php 
		}elseif($arPreAnalise['poaindeferido'] == 'S'){ ?>
		
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">ANÁLISE DE ENGENHARIA</td>
			</tr>
			<tr>
			
				<td colspan="2" style="text-align:left;">	
				<table>
					<tr>
						<td>
					<?php
						$boHabilitado = 'N';
						
						$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $boHabilitado, 'perid' => $_GET['perid'] ) );
						?>
					</td>
					<td valign="top" id="td_barra_navegacao">
					<?php if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)): ?>
					<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
						<tbody>
							<tr style="background-color: rgb(201, 201, 201); text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<span title="estado atual">
										<b>análise</b>
									</span>
								</td>
							</tr>
						<tr style="text-align: center;">
							<td style="font-size: 7pt; text-align: center;">
							</td>
						</tr>
					</table>
					<?php endif; ?>
					</td>
					</tr>
					<?php if(!empty($consideracoesFinais['cmddsc'])): ?>
						<tr>
							<td colspan="2" height="20">
								
							</td>
						</tr>
						<tr>
							<td valign="top" class="subtituloesquerda" colspan="2" style="padding:5px;text-align:center;">						
								<b>CONSIDERAÇÕES FINAIS</b>				
							</td>							
						</tr>
						<tr>
							<td colspan="2">
								<?php echo $consideracoesFinais['cmddsc'] ?>
							</td>
						</tr>
					<?php endif; ?>
					</table>
				</td>
			</tr>
		<?php 
		}elseif($arPreAnalise['poaindeferido'] == 'N'){  ?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">OBRA INDEFERIDA</td>
			</tr>
			<tr>
				<td valign="top" class="subtitulodireita">
					Justificativa:
				</td>
				<td>
					<table width="100%">
					<tr>
						<td valign="top" align="left">
						<?php echo $arPreAnalise['poajustificativa'] ?>
						</td>
						<td>
					</td>
					<td valign="top" align="right">
					<?php if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)): ?>
					<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
						<tbody>
							<tr style="background-color: rgb(201, 201, 201); text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<span title="estado atual">
										<b>análise</b>
									</span>
								</td>
							</tr>
						<tr style="text-align: center;">
							<td style="font-size: 7pt; text-align: center;">
							</td>
						</tr>
					</table>
					<?php endif; ?>
					</td></tr></table>
				</td>
			</tr>
			<?php 
			if(!empty($consideracoesFinais['cmddsc'])){ ?>
				<tr>
					<td valign="top" class="subtituloesquerda" colspan="2" style="padding:5px;text-align:center;">						
						<b>CONSIDERAÇÕES FINAIS</b>
					</td>
					<td>
						<?php echo $consideracoesFinais['cmddsc'] ?>
					</td>
				</tr>
			<?php 
			}; 
		}elseif($arPreAnalise['poaindeferido'] != 'S' && $arPreAnalise['poaindeferido'] != 'N' && $esdid == WF_PRONATEC_OBRA_ARQUIVADA){ ?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">SITUAÇÃO</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align:center;">
					A análise desta obra não foi realizada.
				</td>
			</tr>
		<?php 
		}else{?>
		<tr>
			<td colspan="2">
				<div style="float:right;">
				</div>
			</td>
		</tr>
		<?php }; ?>
	</table>
	<?php }; ?>
<?php }else{ ?>
	<center><p>Sem análise.</p></center>
<?php } ?>