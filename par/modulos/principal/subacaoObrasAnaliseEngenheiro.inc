<?php
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

$preid = ($_SESSION ['par'] ['preid']) ? $_SESSION ['par'] ['preid'] : $_REQUEST ['preid'];
$qrpid = pegaQrpidAnalisePAC ( $preid, 49 );

$muncod = $_SESSION ['par'] ['muncod'];
$docid = prePegarDocid ( $preid );
$esdid = prePegarEstadoAtual ( $docid );
$perfil = pegaArrayPerfil ( $_SESSION ['usucpf'] );

if ($_POST ['atualizaBarraNavegacao']) {
	die ( wf_desenhaBarraNavegacao ( $docid, array (
			'preid' => $preid,
			'qrpid' => $qrpid 
	) ) );
}

// $preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
// $lnkabas = "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=AnaliseEngenheiro".$preidTx;

$preidTx = $_SESSION ['par'] ['preid'] ? '&preid=' . $_SESSION ['par'] ['preid'] : '';
$rSbaid = $_REQUEST ['sbaid'] ? '&sbaid=' . $_REQUEST ['sbaid'] : '';
$rAno = $_REQUEST ['ano'] ? '&ano=' . $_REQUEST ['ano'] : '';
$lnkabas = "par.php?modulo=principal/subacaoObras&acao=A" . $rSbaid . $rAno . $preidTx . "&aba=AnaliseEngenheiro";

monta_titulo ( 'Analise de Obra', '' );

$obPreObraControle = new PreObraControle ();

$arDados = $obPreObraControle->recuperarDadosAnaliseEngenharia ( $preid );
$arRespostas = $obPreObraControle->recuperarRespostasQuestionario ( $qrpid );
$arPreAnalise = $obPreObraControle->recuperarDadosPorPreid ( $preid );
//ver($arPreAnalise);

$municipio = $obPreObraControle->pegaMuncodPorPreid ( $preid );

$consideracoesFinais = $obPreObraControle->recuperarConsideracoesFinais ( $preid );

$muncodIBGE = substr ( $municipio ['muncod'], 0, 6 );
$mundescricao = str_replace ( " ", "%20", $municipio ['mundescricao'] );

if ((in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil )) && $esdid == WF_PAR_EM_DILIGENCIA) {
	$boMostraWFEquipe = true;
}

cabecalho();

?>
<script>
	jQuery(document).ready(function(){

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
		  url: 'par.php?modulo=principal/subacaoObras&acao=A',
		  success: function(data)
		  { 
			 if(data.valida)
			 {		
				alert("Análise finalizada com sucesso!");
				window.location.href = 'par.php?modulo=principal/subacaoObras&acao=A&sbaid=<?=$rSbaid ?>&ano=<?=$rAno ?>&preid=<?=$_GET['preid'] ?>&aba=<?=$_GET['tipoAba'] ?>';
			 }
			 else
			 {
				alert("Para finalizar a análise todas as perguntas devem ter sido respondidas.");
			 }
		  }
		});
	}

	function liberaParaFinalizar( perid, qrpid ){
//		divCarregando();
		jQuery.ajax
		({
		  type: "post",
		  data: "ajaxFinalizarAnalise=1&docid=<?php echo $docid; ?>&qrpid=<?php echo $qrpid; ?>",
		  async: false,
		  dataType: "json",
		  url: 'par.php?modulo=principal/subacaoObras&acao=A',
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
		  url: "par.php?modulo=principal/subacaoObras&acao=A&sbaid=<?=$rSbaid ?>&ano=<?=$rAno ?>&preid=<?=$_GET['preid'] ?>&aba=<?=$_GET['tipoAba'] ?>",
		  complete: function(data)
		  { 
			jQuery('#td_barra_navegacao').html( data.responseText );  
		  }
		});
	}
	
	
</script>
<?php

if (! $arPreAnalise ['poaid'] || $_POST ['poaid']) {
	
	if ($_POST ['poaid']) {
		$arDados ['poaid'] = $_POST ['poaid'];
	} else {
		$arDados ['poaid'] = null;
	}
	
	if (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil )) {
		
		$arDados ['preid'] = $preid;
		$arDados ['poadataanalise'] = date ( 'Y-m-d H:i:s' );
		$arDados ['poastatus'] = 'A';
		
		$arDados ['poaindeferido'] = $_REQUEST ['poaindeferido'];
		$arDados ['poajustificativa'] = $_REQUEST ['poajustificativa'];
		$arDados ['qrpid'] = $qrpid;
		// ver($arDados['poaid'], $arPreAnalise['poausucpfinclusao'], d);
		if (trim ( $arPreAnalise ['poausucpfinclusao'] ) == trim ( $_SESSION ['usucpf'] ) || ($arDados ['poaid'] == null || (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ))))) {
			
			$arDados ['poausucpfinclusao'] = $_SESSION ['usucpf'];
			$id = $obPreObraControle->salvarObraAnalise ( $arDados );
			
			if ($_POST ['poaindeferido'] == 'S') {
				$aedid = WF_PAR_AEDID_ANALISE_FNDE_ENVIAR_PARA_ANALISE;
			} else if ($_POST ['poaindeferido'] == 'N') {
				$aedid = WF_PAR_AEDID_ANALISE_FNDE_ENVIAR_VALIDACAO_INDEFERIMENTO;
			}
			
			// Sem comentário
			// Sem dados no array já que não há chamada de função após a ação.
			$dados = array (
					'preid' => $preid 
			);
			
			if (isset ( $aedid )) {
				// Realiza a alteração do estado da entidade no ano corrente.
				wf_alterarEstado ( $docid, $aedid, '', $dados );
			}
			$aba = $_GET ['tipoAba'];
			if ($_POST ['poaid']) {
				echo "<script>
						alert('Operação realizada com sucesso.');
						document.location.href = 'par.php?modulo=principal/subacaoObras&acao=A&sbaid=$rSbaid&ano=$rAno&preid=$preid&aba=$aba';
					  </script>";
			}
		} else {
			echo "<script>
					alert('Permissão negada.');
					document.location.href = 'par.php?modulo=principal/subacaoObras&acao=A&sbaid=$rSbaid&ano=$rAno&preid=$preid&aba=$aba';
				  </script>";
		}
	}
}

$poaid = ($arPreAnalise ['poaid']) ? $arPreAnalise ['poaid'] : $id;

?>
<!-- habilita o tiny -->
<script language="javascript" type="text/javascript"
	src="../includes/tiny_mce.js"></script>
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
	
function informarDia(preid){
	url = 'par.php?modulo=principal/programas/proinfancia/visao/informarDia&acao=A&preid='+preid+'&tipo=PAR';
	window.open(url,'Impressão do PAR',"height=200,width=400,scrollbars=yes,top=50,left=200" );
}
</script>
<?php
if ($poaid) {
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td colspan="2" class="" style="text-align: center;"><a
			style="cursor: pointer;"
			onclick="window.open('http://cidades.ibge.gov.br/xtras/perfil.php?lang=&codmun=<?php echo $muncodIBGE ?>&search=Mafra','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no');"
			target="_blank"><img style="border: 1px solid black;"
				src="../imagens/logo_ibge.png" border="0"></a> <a
			style="cursor: pointer;"
			onclick="window.open('http://ide.mec.gov.br/2011/municipios/relatorio/coibge/<?php echo $_SESSION['par']['muncod'] ?>','Indicadores','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"
			target="_blank"><img style="border: 1px solid black;"
				src="../imagens/logo_demograficos.png" border="0"></a></td>
	</tr>
	<tr>
		<td colspan="2" class="subtituloesquerda" style="text-align: center;">IDENTIFICAÇÃO</td>
	</tr>
	<tr>
		<td width="150" class="subtitulodireita">Análise N.º:</td>
		<td>
				<?php echo $poaid?>
			</td>
	</tr>
	<tr>
		<td width="150" class="subtitulodireita">N.º de identificação:</td>
		<td>
				<?php echo $arDados['preid']?>
			</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Interessado:</td>
		<td>
			<?=$arDados['entnome']?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Tipo de obra:</td>
		<td>
				<?php echo $arDados['ptodescricao']?>
			</td>
	</tr>	
	<?php
	$boHabilitado = 'disabled="disabled"';
	
	if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil )) {
		$boHabilitado = '';
	}
	
	// if(($esdid == WF_PAR_EM_ANALISE_FNDE ) || ($esdid == WF_PAR_EM_REVISAO_DE_ANALISE && $arPreAnalise['poaindeferido'] == 'N' ) ){
// 	ver($arPreAnalise['poaindeferido']);
	if ( (($esdid == WF_PAR_EM_ANALISE)  || ($esdid == WF_PAR_EM_ANALISE_FNDE) || ($esdid == WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA) || ($esdid == WF_PAR_EM_REVISAO_DE_ANALISE)) && $arPreAnalise ['poaindeferido'] == '' || (in_array($esdid, Array(WF_PAR_EM_REVISAO_DE_ANALISE, WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA) ) && $arPreAnalise ['poaindeferido'] == 'N')) {
		?>
			<tr>
		<td colspan="2" class="subtituloesquerda" style="text-align: center;">CONSIDERAÇÕES
			INICIAIS</td>
	</tr>
	<tr>
		<td colspan="2" class="" style="text-align: center;">
			<table>
				<tr>
					<td align="center" width="100%">
						<form action="" method="post" id="formulario" name="formulario">
							Com base nos documentos apresentados, fotos, estudo de demanda,
							planta de localização, é justificável a construção da obra no
							local? <br />
						<?php
		
		if (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) || in_array ( PAR_PERFIL_ADM_OBRAS, $perfil )) {
			if (in_array ( $esdid, Array (
					WF_PAR_EM_ANALISE_FNDE,
					WF_PAR_EM_ANALISE,
					WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA,
					WF_PAR_EM_REVISAO_DE_ANALISE,
					WF_PAR_VALIDACAO_DILIGENCIA,
					WF_PAR_OBRA_EM_ANALISE_REFORMULAÇÃO,
					WF_PAR_OBRA_EM_ANALISE_REFORMULACAO_MI_CONVENCIONAL,
					WF_PAR_OBRA_EM_ANALISE_REFORMULACAO_MI_CONVENCIONAL_RETORNO_DILIGENCIA 
			) )) {
				$boHabilitado = '';
			}
		}
		if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array ( PAR_PERFIL_ADM_OBRAS, $perfil )) {
			if (! in_array ( $esdid, Array (
					WF_PAR_VALIDACAO_DEFERIMENTO,
					WF_PAR_OBRA_INDEFERIDA,
					WF_PAR_OBRA_DEFERIDA,
					WF_PAR_OBRA_INDEFERIDA_PRAZO 
			) )) {
				$boHabilitado = '';
			}
		}
		
		// if (in_array ( PAR_PERFIL_ADM_OBRAS, $perfil )) {
		// $boHabilitado = '';
		// }
		
		$obSubacaoControle2 = new SubacaoControle ();
		$obPreObra2 = new PreObra ();
		
		if ($preid) {
			$arDados = $obSubacaoControle2->recuperarPreObra ( $preid );
		}
		
		// Regra passada pelo Daniel - 9/6/11
		if (possuiPerfil ( $arrPerfil = array (
				PAR_PERFIL_COORDENADOR_GERAL 
		) ) && $esdid == WF_PAR_OBRA_APROVADA && $arDados ['ptoprojetofnde'] == 'f') {
			$boHabilitado = '';
		}
		?>
						<input type="radio" name="poaindeferido" value="S"
								<?=$arPreAnalise['poaindeferido'] == 'S' ? 'checked' : ''?>
								<?=$boHabilitado ?>><label>Sim.</label> <input type="radio"
								name="poaindeferido" value="N"
								<?=$arPreAnalise['poaindeferido'] == 'N' ? 'checked' : ''?>
								<?=$boHabilitado ?>><label>Não.</label>
							<div class="poajustificativa" style="display:<?=($arPreAnalise['poaindeferido'] == 'S' || $arPreAnalise['poaindeferido'] == '' ) ? 'none' : '' ?>;"  style="text-align:left;">
								<table align="center">
									<tr>
										<td>Justificativa:<br> <textarea name="poajustificativa"
												class="text_editor_simple" cols="50" rows="8"><?=$arPreAnalise['poajustificativa']?></textarea>
										</td>
									</tr>
								</table>
							</div>
							<input type="hidden" name="poaid" value="<?php echo $poaid ?>"> <br />

						</form>
					</td>
					<td valign="top">
					<?php
		$draw = true;
		
		if ((in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil )) && $draw) {
			if ($esdid == WF_PAR_EM_CADASTRAMENTO || $esdid == WF_PAR_EM_DILIGENCIA) {
				wf_desenhaBarraNavegacao ( $docid, array (
						'preid' => $preid,
						'qrpid' => $qrpid 
				) );
				$draw = false;
			}
		} elseif (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) && $draw) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                                       // if( in_array($esdid, Array(WF_PAR_EM_ANALISE_FNDE, WF_PAR_EM_ANALISE, WF_PAR_OBRA_DEFERIDA, WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA ) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
			$draw = false;
			// }
		} elseif (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) && $draw) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                                         // if( !in_array($esdid, Array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_INDEFERIDA_PRAZO, WF_PAR_OBRA_APROVADA, WF_PAR_OBRA_ARQUIVADA) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
			// }
		} elseif ($db->testa_superuser () && $draw) {
			if (! in_array ( $esdid, Array (
					WF_PAR_EM_CADASTRAMENTO,
					WF_PAR_OBRA_CONDICIONADA,
					WF_PAR_OBRA_INDEFERIDA_PRAZO,
					WF_PAR_OBRA_ARQUIVADA 
			) )) {
				wf_desenhaBarraNavegacao ( $docid, array (
						'preid' => $preid,
						'qrpid' => $qrpid 
				) );
				$draw = false;
			}
			// wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );
		}
		?>
						<?php //wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) ); ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr bgcolor="#c0c0c0">
		<td></td>
		<td><input class="enviaForm" type="button" value="Salvar"
			<?=$boHabilitado ?>></td>
	</tr>
		<?php
	} elseif ($arPreAnalise ['poaindeferido'] == 'S' || $esdid == WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL || $esdid == WF_PAR_EM_DILIGENCIA) {
		$questionarioHabilitado = 'N';
 		if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil )) {
 			$questionarioHabilitado = 'S';
 		}
		?>
			<tr>
		<td colspan="2" class="subtituloesquerda" style="text-align: center;">ANÁLISE
			DE ENGENHARIA</td>
	</tr>
	<tr>
		<td colspan="2" style="text-align: left;">
			<table>
				<tr>
					<td>
					<?php
		
		if (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) || in_array ( PAR_PERFIL_ADM_OBRAS, $perfil )) {
			if (in_array ( $esdid, Array (
					WF_PAR_EM_ANALISE_FNDE,
					WF_PAR_EM_ANALISE,
					WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA,
					WF_PAR_EM_REVISAO_DE_ANALISE,
					WF_PAR_VALIDACAO_DILIGENCIA,
					WF_PAR_OBRA_EM_ANALISE_REFORMULAÇÃO,
					WF_PAR_OBRA_EM_ANALISE_REFORMULACAO_MI_CONVENCIONAL,
					WF_PAR_OBRA_EM_ANALISE_REFORMULACAO_MI_CONVENCIONAL_RETORNO_DILIGENCIA,
					WF_PAR_OBRA_CONDICIONADA 
			) )) {
				$questionarioHabilitado = 'S';
			}
		}
		if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array ( PAR_PERFIL_ADM_OBRAS, $perfil )) {
			if (in_array ( $esdid, Array (
					WF_PAR_VALIDACAO_DEFERIMENTO,
					WF_PAR_OBRA_INDEFERIDA,
					WF_PAR_OBRA_DEFERIDA,
					WF_PAR_OBRA_INDEFERIDA_PRAZO 
			// WF_PAR_EM_REVISAO_DE_ANALISE,
			// WF_PAR_VALIDACAO_DEFERIMENTO,
			// WF_PAR_OBRA_INDEFERIDA,
			// WF_PAR_OBRA_DEFERIDA,
			// WF_PAR_OBRA_CONDICIONADA,
			// WF_PAR_OBRA_INDEFERIDA_PRAZO,
			// WF_PAR_OBRA_APROVADA,
			// WF_PAR_OBRA_ARQUIVADA
						) )) {
				$questionarioHabilitado = 'S';
			}
		}
		$obSubacaoControle2 = new SubacaoControle ();
		$obPreObra2 = new PreObra ();
		
		if ($preid) {
			$arDados = $obSubacaoControle2->recuperarPreObra ( $preid );
		}
		
		// Regra passada pelo Daniel - 9/6/11
		if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) && $esdid == WF_PAR_OBRA_APROVADA && $arDados ['ptoprojetofnde'] == 'f') {
			$questionarioHabilitado = 'N';
		}
		
		if ($esdid == WF_PAR_EM_DILIGENCIA) {
			$boHabilitado = 'S';
		}
		
		if (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) || in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil )) {
			$tela = new Tela ( array (
					"qrpid" => $qrpid,
					'tamDivArvore' => 25,
					'tamDivPx' => 250,
					'habilitado' => $questionarioHabilitado,
					'perid' => $_GET ['perid'],
					'relatorio' => 'modulo=principal/programas/proinfancia/questionarioImpressao&acao=A' 
			) );
		} else {
			$tela = new Tela ( array (
					"qrpid" => $qrpid,
					'tamDivArvore' => 25,
					'tamDivPx' => 250,
					'habilitado' => $questionarioHabilitado,
					'perid' => $_GET ['perid'] 
			) );
		}
		?>
					</td>
					<td valign="top" id="td_barra_navegacao">
					<?php
		$draw = true;
		
		if ((in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil )) && $draw) {
			if ($esdid == WF_PAR_EM_CADASTRAMENTO || $esdid == WF_PAR_EM_DILIGENCIA) {
				wf_desenhaBarraNavegacao ( $docid, array (
						'preid' => $preid,
						'qrpid' => $qrpid 
				) );
				$draw = false;
			}
		}
		if (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) && $draw) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                                 // if( in_array($esdid, Array(WF_PAR_EM_REVISAO_DE_ANALISE,WF_PAR_EM_ANALISE_FNDE, WF_PAR_EM_ANALISE, WF_PAR_OBRA_DEFERIDA, WF_PAR_EM_REVISAO_DE_ANALISE ) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
			// }
		}
		if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) && $draw) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                                   // if( !in_array($esdid, Array(WF_PAR_EM_REVISAO_DE_ANALISE, WF_PAR_EM_CADASTRAMENTO, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_APROVADA ) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
			// }
		}
		if ($db->testa_superuser () && $draw) {
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
		}
		?>
					<?php if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) ||  in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)): ?>
					<table
							style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;"
							cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr
									style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;"><span
										title="estado atual"> <b>análise</b>
									</span></td>
								</tr>
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<!-- a onclick="javascript: arvore.s(<?php echo $qrpid ?>);" href="javascript:imprimeQ(49)" class="node" id="sarvore44">Imprimir questionário</a -->
										<a id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>"
										href="javascript:void(0)" class="mostra">Imprimir questionário</a>
									</td>
								</tr>
						
						</table>
					<?php endif; ?>
					<?php
		if ((in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil )) && (in_array ( $esdid, array (
				WF_PAR_VALIDACAO_DILIGENCIA 
		) ) || (in_array ( $esdid, array (
				WF_PAR_EM_DILIGENCIA 
		) )))) {
			?>
						<br> <br>
						<table
							style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;"
							cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr
									style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;"><span
										title="estado atual"> <b>Diligência</b>
									</span></td>
								</tr>
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;"><a
										style="cursor: pointer" alt="Versão para impressão"
										onclick="informarDia('<?php echo $preid; ?>')">Informar
											Quantidade de Dias</a></td>
								</tr>
						
						</table>
						<?php } ?>
					</td>
				</tr>
					<?php if(!empty($consideracoesFinais['cmddsc'])): ?>
						<tr>
					<td colspan="2" height="20"></td>
				</tr>
				<tr>
					<td valign="top" class="subtituloesquerda" colspan="2"
						style="padding: 5px; text-align: center;"><b>CONSIDERAÇÕES FINAIS</b>
					</td>
				</tr>
				<tr>
					<td colspan="2">
								<?php echo $consideracoesFinais['cmddsc']?>
							</td>
				</tr>
					<?php endif; ?>
					</table>
		</td>
	</tr>
		<?php
	} elseif ($arPreAnalise['poaindeferido'] == 'N') {
		?>
			<tr>
		<td colspan="2" class="subtituloesquerda" style="text-align: center;">OBRA
			INDEFERIDA</td>
	</tr>
	<tr>
		<td valign="top" class="subtitulodireita">Justificativa:</td>
		<td>
			<table width="100%">
				<tr>
					<td valign="top" align="left">
						<?php echo $arPreAnalise['poajustificativa']?>
						</td>
					<td></td>
					<td valign="top" align="right">
					<?php
		if ((in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil ))) {
			if ($esdid == WF_PAR_EM_CADASTRAMENTO || $esdid == WF_PAR_EM_DILIGENCIA) {
				wf_desenhaBarraNavegacao ( $docid, array (
						'preid' => $preid,
						'qrpid' => $qrpid 
				) );
			}
		}
		if (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil )) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                        // if( in_array($esdid, Array(WF_PAR_EM_ANALISE_FNDE, WF_PAR_EM_ANALISE, WF_PAR_OBRA_DEFERIDA, WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA ) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			// }
		}
		if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil )) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                          // if( !in_array($esdid, Array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_INDEFERIDA_PRAZO, WF_PAR_OBRA_APROVADA, WF_PAR_OBRA_ARQUIVADA) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			// }
		}
		if ($db->testa_superuser ()) {
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
		}
		?>
					<?php if(in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)): ?>
					<table
							style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;"
							cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr
									style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;"><span
										title="estado atual"> <b>análise</b>
									</span></td>
								</tr>
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;"><a
										id="<?php echo $preid."_".$muncod."_".$qrpid."_49" ?>"
										href="javascript:void(0)" class="mostra">Imprimir questionário</a>
									</td>
								</tr>
						
						</table>
					<?php endif; ?>
					<?php
		if ((in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil )) && (in_array ( $esdid, array (
				WF_PAR_VALIDACAO_DILIGENCIA 
		) ) || (in_array ( $esdid, array (
				WF_PAR_EM_DILIGENCIA 
		) )))) {
			?>
						<br> <br>
						<table
							style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;"
							cellpadding="3" cellspacing="0" border="0">
							<tbody>
								<tr
									style="background-color: rgb(201, 201, 201); text-align: center;">
									<td style="font-size: 7pt; text-align: center;"><span
										title="estado atual"> <b>Diligência</b>
									</span></td>
								</tr>
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;"><a
										style="cursor: pointer" alt="Versão para impressão"
										onclick="informarDia('<?php echo $preid; ?>')">Informar
											Quantidade de Dias</a></td>
								</tr>
						
						</table>
						<?php } ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
			<?php
		if (! empty ( $consideracoesFinais ['cmddsc'] )) {
			?>
				<tr>
		<td valign="top" class="subtituloesquerda" colspan="2"
			style="padding: 5px; text-align: center;"><b>CONSIDERAÇÕES FINAIS</b>
		</td>
		<td>
						<?php echo $consideracoesFinais['cmddsc']?>
					</td>
	</tr>
			<?php
		}
		;
	} elseif ($arPreAnalise ['poaindeferido'] != 'S' && $arPreAnalise ['poaindeferido'] != 'N' && $esdid == WF_PAR_OBRA_ARQUIVADA) {
		?>
			<tr>
		<td colspan="2" class="subtituloesquerda" style="text-align: center;">SITUAÇÃO</td>
	</tr>
	<tr>
		<td colspan="2" style="text-align: center;">
					A análise desta obra não foi realizada.
					<?php if($db->testa_superuser() || in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)): ?>
						<div style="float: right;">
							<?php wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ) );?>
						</div>
					<?php endif; ?>
				</td>
	</tr>
		<?php
	} else {
		?>
		<tr>
		<td colspan="2">
			<div style="float: right;">
					<?php
		$draw = true;
		if ((in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil )) && $draw) {
			if ($esdid == WF_PAR_EM_CADASTRAMENTO || $esdid == WF_PAR_EM_DILIGENCIA) {
				wf_desenhaBarraNavegacao ( $docid, array (
						'preid' => $preid,
						'qrpid' => $qrpid 
				) );
				$draw = false;
			}
		}
		if (in_array ( PAR_PERFIL_ENGENHEIRO_FNDE, $perfil ) && $draw) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                                 // if( in_array($esdid, Array(WF_PAR_EM_ANALISE_FNDE, WF_PAR_EM_ANALISE, WF_PAR_OBRA_DEFERIDA, WF_PAR_EM_ANALISE_RETORNO_DA_DILIGENCIA ) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
			// }
		}
		if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) && $draw) { // Daniel disse que engenheiro_fnde e coodenador_geral pode ver em qlquer estado.
		                                                                   // if( !in_array($esdid, Array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_OBRA_CONDICIONADA, WF_PAR_OBRA_INDEFERIDA_PRAZO, WF_PAR_OBRA_APROVADA, WF_PAR_OBRA_ARQUIVADA) ) ){
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
			// }
		}
		if ($db->testa_superuser () && $draw) {
			wf_desenhaBarraNavegacao ( $docid, array (
					'preid' => $preid,
					'qrpid' => $qrpid 
			) );
			$draw = false;
		}
		?>
				</div>
		</td>
	</tr>
		<?php }; ?>
	</table>
<?php }else{ ?>
<center>
	<p>Sem análise.</p>
</center>
<?php } ?>