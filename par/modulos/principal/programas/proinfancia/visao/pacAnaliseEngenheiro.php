<?php 

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

$preid  = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];
$qrpid = pegaQrpidAnalisePAC( $preid, 49 );
// dbg($qrpid, 1);
$muncod = $_SESSION['par']['muncod'];
$docid  = prePegarDocid($preid);
$estadoAtual = wf_pegarEstadoAtual($docid);
$esdid  = prePegarEstadoAtual($docid);
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
//ver($qrpid, d);

//print enviaEmailDiligenciaProinfancia($preid);
//die();

if ( $_POST['atualizaBarraNavegacao'] ){
	die( wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid )) );
}

echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid=".$preid, $preid);

monta_titulo( 'Análise - Engenharia', "Análise Técnica de Engenharia - CGEST/DIGAP/FNDE"  );

$obPreObraControle = new PreObraControle();

$arDados 		= $obPreObraControle->recuperarDadosAnaliseEngenharia($preid);
$arRespostas 	= $obPreObraControle->recuperarRespostasQuestionario($qrpid);
$arPreAnalise 	= $obPreObraControle->recuperarDadosPorPreid($preid);

$municipio = $obPreObraControle->pegaMuncodPorPreid($preid);

$consideracoesFinais = $obPreObraControle->recuperarConsideracoesFinais($preid);

$muncodIBGE 	= substr($municipio['muncod'], 0, 6);
$mundescricao 	= str_replace(" ","%20", $municipio['mundescricao']);

if( (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)) && $esdid == WF_TIPO_EM_CORRECAO){
	$boMostraWFEquipe = true;
}

cabecalho();

?>
<script>
	jQuery(document).ready(function(){

		jQuery('#td_acao_516').css('display', 'none');
		
		jQuery('.mostra').click(function(){			
			arDados = this.id.split("_");	

			return window.open('par.php?modulo=principal/programas/proinfancia/questionarioImpressao&acao=A&preid='+arDados[0]+'&muncod='+arDados[1]+'&qrpid='+arDados[2]+'&queid='+arDados[3], 
					   'questionarioImpressao',
					   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
		});
		
		jQuery('input[type=radio][name=poaindeferido]').click(function(){
			if(jQuery('input[type=radio][name=poaindeferido]:checked').val() == 'N'){
				jQuery('.poajustificativa').show();
			}else{
				jQuery('.poajustificativa').hide();
			}
		});	

		jQuery('.enviaForm').click(function(){
			jQuery('#formulario').submit();	
		});	
				
		jQuery('#formulario').submit(function(){	
			if(jQuery('input[type=radio][name=poaindeferido]:checked').val() == 'S'){
				return true;
			}else if(jQuery('input[type=radio][name=poaindeferido]:checked').val() == 'N'){
				
				var resposta = trim( tinyMCE.getContent('poajustificativa') );
				
				if(resposta == ''){
					alert('O campo justificativa deve ser preenchido.');
					jQuery('textarea[name=poajustificativa]').focus();
					return false;
				}else{
					if(!confirm("Tem certeza que deseja indeferir a obra?")){
						return false;
					}
					return true;
				}	
			}else{
				alert("Escola uma resposta.");				
				return false;
			}			
		});
	});

	function finalizarAnalise()
	{
		$.ajax
		({
		  type: "post",
		  data: "ajaxFinalizarAnalise=1&docid=<?php echo $docid; ?>&qrpid=<?php echo $qrpid; ?>",
		  dataType: "json",
		  url: 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A',
		  success: function(data)
		  { 
			 if(data.valida)
			 {		
				alert("Análise finalizada com sucesso!");
				window.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=listaObras&preid=<?php echo $preid; ?>';
			 }
			 else
			 {
				alert("Para finalizar a análise todas as perguntas devem ter sido respondidas.");
			 }
		  }
		});
	}

	function liberaParaFinalizar( perid, qrpid ){
			
		return false;
		
//		divCarregando();
		jQuery.ajax
		({
		  type: "post",
		  data: "ajaxFinalizarAnalise=1&docid=<?php echo $docid; ?>&qrpid=<?php echo $qrpid; ?>",
		  async: false,
		  dataType: "json",
		  url: 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A',
		  success: function(data)
		  { 
			if(data.valida){	
				alert("Análise finalizada com sucesso!");
				self.location.href = location.href.replace("#", "");
			}
			 
		  }
		});
		atualizarTelaPergunta();		  					 
	}

	function atualizarTelaPergunta(){
		jQuery('#td_barra_navegacao').html(""); 
//		var url = ;
		jQuery.ajax
		({
		  type: "post",
		  data: "atualizaBarraNavegacao=1",
//		  dataType: "json",
		  async: false,
		  url: "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=<?=$_GET['tipoAba'] ?>&preid=<?=$_GET['preid'] ?>&muncod=<?=$_GET['muncod'] ?>",
		  complete: function(data)
		  { 
			jQuery('#td_barra_navegacao').html( data.responseText );  
		  }
		});
	}
	
	
</script>
<?php 

if(!$arPreAnalise['poaid'] || $_POST['poaid']){
	
	if($_POST['poaid']){
		$arDados['poaid'] 			= $_POST['poaid'];		
	}else{
		$arDados['poaid'] 			= null;
	}
	
	if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
		in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ){
		
		$arDados['preid'] 				= $preid;
		$arDados['poadataanalise'] 		= date('Y-m-d H:i:s');
		$arDados['poastatus'] 			= 'A';
		
		$arDados['poaindeferido'] 		= $_REQUEST['poaindeferido'];
		$arDados['poajustificativa'] 	= $_REQUEST['poajustificativa'];
		$arDados['qrpid'] 				= $qrpid;
//		ver($arDados['poaid'], $arPreAnalise['poausucpfinclusao'], d);
		if(trim($arPreAnalise['poausucpfinclusao']) == trim($_SESSION['usucpf']) || 
		   ($arDados['poaid'] == null || (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		   (in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil))))){
		   	
			$arDados['poausucpfinclusao'] 	= $_SESSION['usucpf'];	
			$id = $obPreObraControle->salvarObraAnalise($arDados);
			
			if( $_POST['poaindeferido'] == 'S' )
			{
				$aedid = WF_AEDID_ANALISE_FNDE_ENVIAR_PARA_ANALISE;
			}
			else if($_POST['poaindeferido'] == 'N')
			{
				$aedid = WF_AEDID_ANALISE_FNDE_ENVIAR_VALIDACAO_INDEFERIMENTO;
			}
			
			
			// Sem comentário
			// Sem dados no array já que não há chamada de função após a ação.
			$dados = array( 'preid' => $preid);

			if(isset($aedid)){			
				// Realiza a alteração do estado da entidade no ano corrente.
				wf_alterarEstado( $docid, $aedid, '', $dados );
			}
			
			if($_POST['poaid']){
				echo "<script>
						alert('Operação realizada com sucesso.');
						document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid={$preid}';
					  </script>";
			}
			
		}else{
			echo "<script>
					alert('Permissão negada.');
					document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analiseEngenheiro&preid={$preid}';
				  </script>";
		}
		
		
	}
}

$poaid = ($arPreAnalise['poaid']) ? $arPreAnalise['poaid'] : $id;  
?>
<!-- habilita o tiny -->
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">

function recusarProrrogacao(preid) {
    window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&recusar=S&preid=' + preid,
            'Prorrogação',
            "height=400,width=600,scrollbars=yes,top=50,left=200").focus();
}

function editarProrrogacao(preid){

window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&editar=1&preid='+preid, 
		   'Prorrogação', 
		   "height=400,width=600,scrollbars=yes,top=50,left=200" ).focus();
}

function aceitarProrrogacao(preid){

window.open('par.php?modulo=principal/popupProrrogacaoPACAprovacao&acao=A&preid='+preid, 
		   'Prorrogação', 
		   "height=400,width=600,scrollbars=yes,top=50,left=200" ).focus();
}

function informarDia(preid, qrpid){
	url = 'par.php?modulo=principal/programas/proinfancia/visao/informarDia&acao=A&preid='+preid+'&qrpid='+qrpid+'&tipo=PAC';
	window.open(url,'Impressão do PAR',"height=350,width=550,scrollbars=yes,top=50,left=200" );
}

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
<!-- fim tiny -->
<?php if($poaid):

?>



	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2" class="" style="text-align:center;">
				<!--  <a style="cursor: pointer;" onclick="window.open('http://www.ibge.gov.br/cidadesat/xtras/perfilwindow.php?nomemun=<?php echo $mundescricao ?>&amp;codmun=<?php echo $muncodIBGE ?>&amp;r=2','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_ibge.png" border="0"></a>-->
				<a style="cursor: pointer;" onclick="window.open('http://cidades.ibge.gov.br/xtras/perfil.php?lang=&codmun=<?php echo $muncodIBGE ?>&search=Mafra','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_ibge.png" border="0"></a> 
				<!-- <a style="cursor: pointer;" onclick="window.open('http://portal.mec.gov.br/ide/2008/gerarTabela.php?municipio=<?php echo $_SESSION['par']['muncod'] ?>','Indicadores','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_demograficos.png" border="0"></a>  -->
				<a style="cursor: pointer;" onclick="window.open('http://ide.mec.gov.br/2011/municipios/relatorio/coibge/<?php echo $_SESSION['par']['muncod'] ?>','Indicadores','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');" target="_blank"><img style="border: 1px solid black;" src="../imagens/logo_demograficos.png" border="0"></a>
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
				<?=$arDados['entnome'] ?>
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
		in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || 
		in_array(PAR_PERFIL_CONSULTA, $perfil) || 
        in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) ||
        in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil) ||
        in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||
		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
		in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
		in_array(PAR_PERFIL_PREFEITO, $perfil) ||  
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||  
		in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||  
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||  
		in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)){

		if( ($esdid == WF_TIPO_EM_ANALISE_FNDE ) && $arPreAnalise['poaindeferido'] == '' || ($esdid == WF_TIPO_EM_ANALISE_VALIDACAO && $arPreAnalise['poaindeferido'] == 'N' )){ ?>
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
							if( in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ){
								if(in_array($esdid, Array( WF_TIPO_EM_ANALISE_FNDE, WF_TIPO_EM_ANALISE, WF_TIPO_EM_ANALISE_DILIGENCIA, WF_TIPO_OBRA_CONDICIONADA))){
									$boHabilitado = '';
								}
							}
							if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ){
								if(!in_array($esdid, Array(WF_TIPO_VALIDACAO_DEFERIMENTO, WF_TIPO_OBRA_INDEFERIDA, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA))){
									$boHabilitado = '';
								}
							}
							if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ){
								$boHabilitado = '';
							}
							
							$obSubacaoControle2 = new SubacaoControle();
							$obPreObra2 = new PreObra();
							
							if($preid){
								$arDados = $obSubacaoControle2->recuperarPreObra($preid);
							}
							
							// Regra passada pelo Daniel - 9/6/11
							if(possuiPerfil($arrPerfil = array(PAR_PERFIL_COORDENADOR_GERAL)) && $esdid == WF_TIPO_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
								$boHabilitado = '';
							}
							// Regra passada pelo Daniel - 22/8/11
							if($arDados['preidpai']) {
								$boHabilitado = 'disabled="disabled"';
							}
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
					<?php 
					$draw = true;
					if(!$arDados['preidpai']) {
						if( (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ) && $draw ){
							if( $esdid == WF_TIPO_EM_CADASTRAMENTO || $esdid == WF_TIPO_EM_CORRECAO  ){
								wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
								$draw = false;
							}
						}elseif( in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) && $draw  ){
							if( in_array($esdid, Array(WF_TIPO_EM_ANALISE_FNDE, WF_TIPO_EM_ANALISE, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_EM_ANALISE_DILIGENCIA ) )  ){
								wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
								$draw = false;
								$draw = false;
							}
						}elseif( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) && $draw  ){
							if( !in_array($esdid, Array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) )  ){
								wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
								$draw = false;
							}
						}elseif( $db->testa_superuser() && $draw  ){
							if( !in_array($esdid, Array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) )  ){
								wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
								$draw = false;
							}
	//						wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
						}
					}
					?>
						<?php //wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) ); ?>
					</td></tr>
				</table>
				</td>
			</tr>
			<tr bgcolor="#c0c0c0">
				<td></td>
				<td><input class="enviaForm" type="button" value="Salvar" <?=$boHabilitado ?>></td>
			</tr>
		<?php 
		}elseif(
			$arPreAnalise['poaindeferido'] == 'S' 
			|| $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO
			|| $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL
			|| $esdid == WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA 
		){
		?>
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

						$obSubacaoControle2 = new SubacaoControle();
							
						if($preid){
							$arDados = $obSubacaoControle2->recuperarPreObra($preid);
						}
						
						// Regra passada pelo Daniel - 22/8/11
						if( !$arDados['preidpai'] ){
							if( in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil)  ){
								$arrEsdidHab = Array( 
										WF_TIPO_EM_ANALISE_VALIDACAO,
										WF_TIPO_EM_ANALISE_FNDE, 
										WF_TIPO_EM_ANALISE, 
										WF_TIPO_EM_ANALISE_DILIGENCIA, 
										WF_TIPO_OBRA_CONDICIONADA,
										WF_TIPO_OBRA_APROVACAO_CONDICIONAL,
										WF_TIPO_EM_ANALISE_REFORMULACAO,
										WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL,
										WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA
									);
								if(in_array($esdid, $arrEsdidHab)){
									$boHabilitado = 'S';
								}
							}
							if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ){
								$arrEsdidHab = Array(
										WF_TIPO_EM_ANALISE_VALIDACAO,
										WF_TIPO_VALIDACAO_DEFERIMENTO, 
										WF_TIPO_OBRA_INDEFERIDA, 
										WF_TIPO_OBRA_DEFERIDA,
										WF_TIPO_OBRA_CONDICIONADA, 
										WF_TIPO_OBRA_INDEFERIDA_PRAZO, 
										WF_TIPO_OBRA_APROVADA, 
										WF_TIPO_OBRA_ARQUIVADA,
										WF_TIPO_EM_ANALISE_REFORMULACAO,
										WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL,
										WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA
									);
								if(!in_array($esdid, $arrEsdidHab)){
									$boHabilitado = 'S';
								}
							}
							if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ){
								$boHabilitado = 'S';
							}
						}
							
						// Regra passada pelo Daniel - 9/6/11
						if( possuiPerfil( Array(PAR_PERFIL_COORDENADOR_GERAL) ) && $esdid == WF_TIPO_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
							$boHabilitado = 'S';
						}
						
// 						$arrEsdidCad = Array( 
// 								WF_TIPO_EM_REFORMULACAO, 
// 								WF_TIPO_EM_CORRECAO, 
// 								WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL
// 							);
// 						if( in_array($esdid, $arrEsdidCad ) ){
// 							$boHabilitado = "N";
// 						}

// 						if( in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil)  ){
// 							$boHabilitado = "N";
//                        	}
                                                
						if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)){
                        	$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $boHabilitado, 'perid' => $_GET['perid'], 'relatorio' => 'modulo=principal/programas/proinfancia/questionarioImpressao&acao=A' ) ); 
						}else{
                        	$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $boHabilitado, 'perid' => $_GET['perid'] ) );
						}
						?>
					</td>
					<td valign="top" id="td_barra_navegacao">
					<?php 
					$draw = true;

					if(!$arDados['preidpai']) {
						if( (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || 
							 in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || 
							 in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) || 
							 in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil)
							) && 
							$draw ){
							if( $esdid == WF_TIPO_EM_CADASTRAMENTO || $esdid == WF_TIPO_EM_CORRECAO || $esdid == WF_TIPO_EM_REFORMULACAO ){
								wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
								$draw = false;
							}
						}
						if( in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) && $draw ){ 
							if( in_array($esdid, Array(WF_TIPO_EM_ANALISE_VALIDACAO,WF_TIPO_EM_ANALISE_FNDE, WF_TIPO_OBRA_APROVADA, WF_TIPO_EM_ANALISE, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_EM_ANALISE_DILIGENCIA, 
														WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL, WF_TIPO_EM_ANALISE_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL_RETORNO_DILIGENCIA ) )  ){
								wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
								$draw = false;
							}
						}
						if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) && $draw ){
							if( !in_array($esdid, Array(WF_TIPO_EM_ANALISE_VALIDACAO,WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_OBRA_CONDICIONADA ) )  ){
								wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
								$draw = false;
							}
						}
						if( $db->testa_superuser() && $draw ){
							wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
							$draw = false;
						}
					}
					
					if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil)): ?>
					<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
                                            <tbody>
                                                <tr style="background-color: rgb(201, 201, 201); text-align: center;">
                                                    <td style="font-size: 7pt; text-align: center;">
                                                        <span title="estado atual">
                                                            <b>Estado atual</b>
                                                        </span>
                                                    </td>
                                                </tr>
                                                <tr style="text-align: center;">
                                                    <td style="font-size: 7pt; text-align: center;">
                                                        <?php echo $estadoAtual['esddsc']; ?>
                                                    </td>
                                                </tr>
                                            </tbody>
					</table>
                                        <br />
                                        <br />
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
                                                        <a id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>" href="javascript:void(0)" class="mostra">Imprimir análise</a>
                                                    </td>
                                                </tr>
                                            </tbody>
					</table>
                    <?php 
                    endif; 
                    if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)): ?>
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
								<!-- a onclick="javascript: arvore.s(<?php echo $qrpid ?>);" href="javascript:imprimeQ(49)" class="node" id="sarvore44">Imprimir questionário</a -->
								<a id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>" href="javascript:void(0)" class="mostra">Imprimir análise</a>
							</td>
						</tr>
					</table>
					<?php
						if(  	
							in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
							in_array(PAR_PERFIL_ADMINISTRADOR, $perfil ) || 
							in_array( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) 
							){
 
							if( in_array( $esdid, array(WF_TIPO_VALIDACAO_DILIGENCIA, WF_TIPO_EM_CORRECAO) )  ) {
							?>
							<br><br>
							<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
								<tbody>
									<tr style="background-color: rgb(201, 201, 201); text-align: center;">
										<td style="font-size: 7pt; text-align: center;">
											<span title="estado atual">
												<b>Diligência</b>
											</span>
										</td>
									</tr>
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<a style="cursor: pointer" alt="Versão para impressão" onclick="informarDia('<?php echo $preid; ?>',  '<?php echo $qrpid; ?>')">
											<?=( in_array( $esdid, Array(WF_TIPO_EM_CORRECAO) ) ? "Ajustar prazo diligência" : "Enviar para diligência" )  ?>
										</a>
									</td>
								</tr>
							</table>
							<?php
							}
							
							if( verificaProrrogacao($preid) ){
								$data = $db->pegaUm("SELECT popdataprazo FROM obras.preobraprorrogacao WHERE popstatus = 'P' AND preid = ".$preid);
								?>
									<table align="center" border="0" cellpadding="5" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #d0d0d0; width: 80px;">
										<tr style="background-color: #c9c9c9; text-align: center;">
											<td style="font-size: 7pt; text-align: center;">
												<span title=""><strong>Prorrogação para o dia <? echo formata_data($data); ?></strong></span> 
											</td>
										</tr>
												
										<tr style="text-align: center;">
											<td style="font-size: 7pt; text-align: center;">
												<a style="cursor: pointer" onclick="aceitarProrrogacao(<?php echo $preid; ?>);" title="Aprovar Prorrogação">Aprovar Prorrogação</a>
											</td>
										</tr>
										<tr style="background-color: #c9c9c9; text-align: center;">
											<td style="font-size: 7pt; text-align: center;">
												<span title=""><strong></strong></span> 
											</td>
										</tr>
										<tr style="text-align: center;">
											<td style="font-size: 7pt; text-align: center;">
												<a style="cursor: pointer" onclick="editarProrrogacao(<?php echo $preid; ?>);" title="Editar Prorrogação">Editar Prorrogação</a>
											</td>
										</tr>
										<tr style="background-color: #c9c9c9; text-align: center;">
											<td style="font-size: 7pt; text-align: center;">
												<span title=""><strong></strong></span> 
											</td>
										</tr>
										<tr style="text-align: center;">
											<td style="font-size: 7pt; text-align: center;">
												<a style="cursor: pointer" onclick="recusarProrrogacao(<?php echo $preid; ?>);" title="Recusar Prorrogação">Recusar Prorrogação</a>
											</td>
										</tr>
									</table>
							<?php 
							} 
						}
					endif; ?>
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
			<?php //if($arPreAnalise['poausucpfinclusao'] == $_SESSION['usucpf'] && in_array($esdid, $arSituacao) ){ ?>
<!--				<tr id="tr_finalizar" style="display:<?php //=($obPreObraControle->validaPermissaoFinalizarQuestionario( $qrpid ) ? "" : "none" ) ?>;">-->
<!--					<td colspan="2" class="subtituloesquerda" style="text-align:center;">FINALIZAR ANÁLISE</td>-->
<!--				</tr>-->
<!--				<tr id="tr_finalizar_link" style="display:<?php //=($obPreObraControle->validaPermissaoFinalizarQuestionario( $qrpid ) ? "" : "none" ) ?>;">-->
<!--					<td colspan="2" class="" style="text-align:center;">-->
<!--						<input type="button" value="Finalizar" onclick="finalizarAnalise();" />-->
<!--						<input type="button" style="cursor:pointer;" onclick="finalizarAnalise();" value="Finalizar">-->
<!--					</td>-->
<!--				</tr>-->
			<?php //}; ?>
		<?php 
		}elseif($arPreAnalise['poaindeferido'] == 'N'){ ?>
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
					<?php 
					$draw = true;
					if( (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil)) && $draw  ){
						if( $esdid == WF_TIPO_EM_CADASTRAMENTO || $esdid == WF_TIPO_EM_CORRECAO || $esdid == WF_TIPO_EM_REFORMULACAO ){
							wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
							$draw = false;
						}
					}
					if( in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) && $draw  ){
						if( in_array($esdid, Array(WF_TIPO_EM_ANALISE_FNDE, WF_TIPO_OBRA_APROVADA, WF_TIPO_EM_ANALISE, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_EM_ANALISE_DILIGENCIA ) )  ){
							wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
							$draw = false;
						}
					}
					if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) && $draw  ){
						if( !in_array($esdid, Array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) )  ){
							wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
							$draw = false;
						}
					}
					if( $db->testa_superuser() && $draw  ){
						wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
						$draw = false;
					}
					?>
					<?php //if(!in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) && !in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || $boMostraWFEquipe): ?>
						<?php //wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );?>
					<?php //endif; ?>
					<?php if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)): ?>
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
								<a id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>" href="javascript:void(0)" class="mostra">Imprimir questionário</a>
							</td>
						</tr>
					</table>
					<?php
					if( ( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) ) && ( in_array( $esdid, array(WF_TIPO_VALIDACAO_DILIGENCIA) ) /*|| (in_array( $esdid, array(WF_TIPO_EM_CORRECAO) ))*/ ) ){
						?>
						<br><br>
						<table style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;" cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<span title="estado atual">
											<b>Diligência</b>
										</span>
									</td>
								</tr>
							<tr style="text-align: center;">
								<td style="font-size: 7pt; text-align: center;">
									<a style="cursor: pointer" alt="Versão para impressão" onclick="informarDia('<?php echo $preid; ?>',  '<?php echo $qrpid; ?>')">Enviar para diligência</a>
								</td>
							</tr>
						</table>
						<?php
					}
					endif; ?>
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
		}elseif($arPreAnalise['poaindeferido'] != 'S' && $arPreAnalise['poaindeferido'] != 'N' && $esdid == WF_TIPO_OBRA_ARQUIVADA){?>
			<tr>
				<td colspan="2" class="subtituloesquerda" style="text-align:center;">SITUAÇÃO</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align:center;">
					A análise desta obra não foi realizada.
					<?php if($db->testa_superuser() || in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)): ?>
						<div style="float:right;">
							<?php wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );?>
						</div>
					<?php endif; ?>
				</td>
			</tr>
		<?php 
		}else{?>
		<tr>
			<td colspan="2">
				<div style="float:right;">
					<?php 
					$draw = true;
					if( (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ) && $draw ){
						if( $esdid == WF_TIPO_EM_CADASTRAMENTO || $esdid == WF_TIPO_EM_CORRECAO || $esdid == WF_TIPO_EM_REFORMULACAO ){
							wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
							$draw = false;
						}
					}
					if( in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) && $draw ){
						if( in_array($esdid, Array(WF_TIPO_EM_ANALISE_FNDE, WF_TIPO_EM_ANALISE, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_DEFERIDA, WF_TIPO_EM_ANALISE_DILIGENCIA ) )  ){
							wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
							$draw = false;
						}
					}
					if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) && $draw ){
						if( !in_array($esdid, Array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_OBRA_CONDICIONADA, WF_TIPO_OBRA_INDEFERIDA_PRAZO, WF_TIPO_OBRA_APROVADA, WF_TIPO_OBRA_ARQUIVADA) )  ){
							wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
							$draw = false;
						}
					}
					if( $db->testa_superuser() && $draw ){
						wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
						$draw = false;
					}
					?>
				</div>
			</td>
		</tr>
		<?php }; ?>
	</table>
	<?php }; ?>
<?php else: ?>
	<center><p>Sem análise.</p></center>
<?php endif; ?>
<script>
	function wf_alterarEstado( aedid, docid, esdid, acao )
		{
			if(aedid=='1772') {
				if ( !confirm( 'Senhor(a) Gestor(a),\n\n\tAntes de aderir à ata de Registro de Preços verifique a situação do local onde a obra será edificada. Caso haja necessidade na alteração do local da obra o proponente deve formalizar essa solicitação ao FNDE mediante Oficio assinado pelo(a) Prefeito(a) Municipal, encaminhar para o e-mail: reformulacao_cgest@fnde.gov.br e aguardar análise. \n\n\tRessaltamos que após adesão à ata de Registro de Preços para construção de Creche/Escola – Proinfância é vedado ao município efetuar alterações no local já aprovado para a obra. \n\n\tA partir da validação de adesão à Ata de Registro de Preços o proponente deverá providenciar à adequação do terreno disponibilizado para a construção da obra conforme exigências do FNDE, descrito na resolução 25 de 14 de Junho de 2013, art. 5º.' ) )
				{
					return;
				}
			
			} else{
				if(acao) {
					var f = acao.charAt(0).toLowerCase();
	  				acao = f + acao.substr(1);
				}
				
				if ( !confirm( 'Deseja realmente ' + acao + ' ?' ) )
				{
					return;
				}
			}
			var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/alterar_estado.php' +
			'?aedid=' + aedid +
			'&docid=' + docid +
			'&esdid=' + esdid +
			'&verificacao=<?php echo urlencode( serialize( array( 'preid' => $preid, 'qrpid' => $qrpid )) ); ?>';
		var janela = window.open(
			url,
			'alterarEstado',
			'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
		);
		janela.focus();
	}
</script>