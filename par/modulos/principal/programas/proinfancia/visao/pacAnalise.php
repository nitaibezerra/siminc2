<?php

function desenhaBotaoAprovacao(){

	?>
<table cellspacing="0" cellpadding="3" border="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
	<tr>
		<td onmouseout="this.style.backgroundColor='';" onmouseover="this.style.backgroundColor='#ffffdd';" style="font-size: 7pt; text-align: center; 
			border-top: 2px solid rgb(208, 208, 208);">
			<a id="abre_aprovacao" title="Enviar para obra aprovada" alt="Enviar para obra aprovada" href="#">
				<img border="0" align="absmiddle" src="../imagens/workflow/1.png"> <br>
				Enviar para obra aprovada</a>
		</td>
	</tr>
</table>
<?php
}

if( $_POST['recusarProrrogacao'] == true ){
	global $db;
	
	$sql = "UPDATE obras.preobraprorrogacao SET popstatus = 'F' WHERE popstatus = 'P' AND preid = ".$_POST['preid'];
	$db->executar($sql);
	
	$docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='".$_POST['preid']."'");
	$result = wf_alterarEstado( $docid, $aedid = WF_AEDID_AGUARDANDO_PRORROGACAO_ENVIAR_PARA_OBRA_APROVADA, true, $d = array('preid' => $_POST['preid']));
	
	if( $db->commit() ){
		return 'sucesso';
	} else {
		return 'erro';
	}
	die();
}

if( $_POST['aceitarProrrogacao'] == true ){
	
	global $db;
	
	if( gerarDocumentoProrrogacao($_POST['preid']) ){
	
		include_once APPRAIZ . 'includes/workflow.php';
		$docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='".$_POST['preid']."'");
		
		$sql = "SELECT tooid FROM obras.preobra WHERE preid = {$_POST['preid']}";
		$tooid = $db->pegaUm($sql);
		
		$esdidorigem = WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO;
		
		$esdiddestino = $db->pegaUm("SELECT esdidorigem FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid=".$_POST['preid']);
		$aedid = $db->pegaUm("SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = ".$esdidorigem." AND esdiddestino = ".$esdiddestino);
		
		wf_alterarEstado( $docid, $aedid, true, $d = array('preid' => $_POST['preid']));
		
		return true;
	}else{
		
		return false;
	}
	die();
}

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

$preid = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];
 
echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analise&preid=".$preid, $preid);
monta_titulo( 'Verificar pendências', $obraDescricao  );

$oPreObra = new PreObra();
$oSubacaoControle = new SubacaoControle();
$pacFNDE  = $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
$arDados  = $oSubacaoControle->recuperarPreObra($preid);

if( $preid ){	
	
	$qrpid = pegaQrpidPAC( $preid, 43 );	
	
	$pacDados = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);	
	$pacFotos = $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
	$pacDocumentos = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
	if($pacFNDE == 'f'){
		$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
	}
	$pacQuestionario = $oPreObra->verificaQuestionario($qrpid);	
	$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
	$pacCronograma = $oPreObra->verificaCronograma($preid);
	$pacLatitude   = $oPreObra->verificaLatitudePreObra($preid);
}

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);	

$reformulaMI = verificaMi( $preid );

$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];

$ptoid = $oPreObra->verificaTipoObra($preid, $sistema);

$msgPlanilha = 'Falta(m) ' . $boPlanilhaOrcamentaria['faltam'] . ' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.';

if( $ptoid == '' ){
	echo "
		<script>
			window.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid=$preid';
		</script>";
	die();
}

$sql = "SELECT ptopreencher FROM obras.pretipoobra WHERE ptoid = $ptoid";
$ptopreencher = $db->pegaUm( $sql );

if( $ptopreencher != 't' ) $msgPlanilha = "Falta salvar a planilha orçamentaria.";

$arPendencias = array('Dados do terreno' => 'Falta o preenchimento dos dados.',
					  'Latitude e Longitude dos Dados do Terreno' => 'Falta o preenchimento da Latitude e Longitude.',
					  'Relatório de vistoria' => 'Falta o preenchimento dos dados.',
					  'Cadastro de fotos do terreno' => 'Deve conter no mínimo 3 fotos do terreno.',
					  'Cronograma físico-financeiro' => 'Falta o preenchimento dos dados.',
					  'Documentos anexos' => 'Falta anexar os arquivos.',
					  'Projetos - Tipo A' => 'Falta anexar os arquivos.',
					  'Itens Planilha orçamentária' => $msgPlanilha,
					  'Planilha orçamentária' => $msgPlanilha,
// 					  'Valor da planilha orçamentária' => 'O valor R$ '.formata_valor($boPlanilhaOrcamentaria['valor']).' não confere, deve estar entre R$ '.formata_valor($boPlanilhaOrcamentaria['minimo']).' e R$ '.formata_valor($boPlanilhaOrcamentaria['maximo']).'.');
					  'Valor da planilha orçamentária' => 'O valor R$ '.formata_valor($boPlanilhaOrcamentaria['valor']).' não confere, deve seve ser maior que R$ '.formata_valor($boPlanilhaOrcamentaria['minimo']).'.');

$arperfil = pegaArrayPerfil($_SESSION['usucpf']);

?>
<?php echo cabecalho();?>
<!-- <script type="text/javascript" src="/includes/prototype.js"></script> -->
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript">

	jQuery(document).ready(function(){

		jQuery('#td_acao_516').css('display', 'none');
	
		jQuery('#abre_aprovacao').click(function(){
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "&req=popupAprovacaoReformulacao",
		   		async: false,
		   		success: function(msg){
					jQuery( '#dialog-aprovacaoRef' ).show();
		   			jQuery( "#dialog-aprovacaoRef" ).html(msg);		
					jQuery( "#dialog-aprovacaoRef" ).dialog({
						resizable: false,
						height:600,
						width:600,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							"Fechar": function() {
								jQuery( this ).dialog( "close" );
								window.location.reload();								
							}
							
						}
					});
		   		}
			});
		});
	});
	
//	function recusarProrrogacao(preid){
//		if( confirm("Tem certeza que deseja recusar essa prorrogação?") ){
//			jQuery.ajax({
//		   		type: "POST",
//		   		url: 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=analise&preid='+preid,
//		   		data: '&recusarProrrogacao=true&preid='+preid,
//		   		async: false,
//		   		success: function(msg){
//		   			if( msg == 'erro' ){
//		        		alert('Problema na execução');
//		        	} else {
//		        		alert('Prorrogação cancelada');
//		        		window.location.href = window.location.href;
//		        	}
//		   		}
//			});
//	  	}
//	}
	
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
</script>
<table class="tabela" align="center">
<colgroup>
	<col width="91%" align="center" />
	<col width="9%" />
</colgroup>
<tr>
	<td>
	<table align="center">
		
	<?php //ver($boPlanilhaOrcamentaria, d) ?>
	<?php $x=0 ?>
	<?php foreach($arPendencias as $k => $v): ?>
		<?php
			$cor = ($x % 2) ? 'white' : '#d9d9d9;';  
			if(  ( !$pacDados && $k == 'Dados do terreno' ) || 
				 ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) || 
				 ( $k == 'Latitude e Longitude dos Dados do Terreno' && !$pacLatitude ) || 
				 ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
				 ( $k == 'Itens Planilha orçamentária' && ( $boPlanilhaOrcamentaria['faltam'] > 0 || $boPlanilhaOrcamentaria['ppoid'] < 1 ) ) ||
				 ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $arDados['ptoprojetofnde'] == 't' && !($reformulaMI)) ||
				 ( $k == 'Valor da planilha orçamentária' && ( str_replace(',','',number_format($boPlanilhaOrcamentaria['valor'],2)) < $boPlanilhaOrcamentaria['minimo'] /*|| str_replace(',','',number_format($boPlanilhaOrcamentaria['valor'],2)) > $boPlanilhaOrcamentaria['maximo']*/) && $pacFNDE == 't' && !($reformulaMI) ) ||
				 ( $k == 'Cronograma físico-financeiro' && !in_array($ptoid, Array(73,74)) && !$pacCronograma && $arDados['ptoprojetofnde'] == 't' && !($reformulaMI) ) ||
				 ( ($pacDocumentosTipoA['arqid'] != $pacDocumentosTipoA['podid'] || !$pacDocumentosTipoA) && $k == 'Projetos - Tipo A' && $arDados['ptoprojetofnde'] == 'f' ) || 
				 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' ) 
				 ): ?>
			<?php if(!$boMsg){
			?>
				<tr>
					<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
						O sistema verificou que alguns dados não foram preenchidos:
					</td>
				</tr>
				<?php $boMsg = true; ?>
			<?php }else{
				  } ?>
			<tr style="background-color: <?php echo $cor ?>;">
				<td>
					<?php 
					switch($k){
						case 'Dados do terreno':
							$aba = 'dados';
							break;
						case 'Relatório de vistoria':
							$aba = 'questionario';
							break;
						case 'Cadastro de fotos do terreno':
							$aba = 'foto';
							break;
						case 'Itens Planilha orçamentária':
							$aba = 'planilhaOrcamentaria';
							break;
						case 'Planilha orçamentária':
							$aba = 'planilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo B 110v':
							$aba = 'planilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo B 220v':
							$aba = 'planilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo C 110v':
							$aba = 'planilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo C 220v':
							$aba = 'planilhaOrcamentaria';
							break;							
						case 'Cronograma físico-financeiro':
							$aba = 'cronograma';
							break;
						case 'Documentos anexos':
							$aba = 'documento';
							break;
						default:
							$aba = "dados";
							break;
					}
					?>
					<a href="par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=<?php echo $aba ?>&preid=<?php echo $preid ?>">
					<img border="0" src='/imagens/consultar.gif' onclick='javascript:void(0)'>
					</a>
				</td>
				<td>
					<b><?php echo $k ?></b>
					<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - 
					<?php echo str_replace("{valor}","R$ ".formata_valor($boPlanilhaOrcamentaria['valor']), $v) ?><br />
				</td>
				<td style="background:white;width:100px;"></td>
			</tr>
			<?php $x++ ?>
		<?php endif; ?>			
	<?php endforeach; 
		if( $oPreObra->verificaArquivoCorrompido( $preid ) ){
			$boMsg = true;
	?>
		<tr>
			<td colspan="3" style="text-align:center;font-size:12px;font-weight:bold;color:#900;height:50px;">
				Prezada(o) Usuaria(o) - Esta obra contém arquivos corrompidos. <br>
				Favor anexar novamente os documentos ou fotos anexados em vermelho clicando no botão 'Substituir' ,<br> 
				nas abas 'Cadastro de fotos do terreno', 'Documentos Anexos' e 'Projetos - Tipo A', se houver. <br>
				Posteriormente, tramitar a obra para analise do FNDE - Atenciosamente - Equipe SIMEC/PAR.
			</td>
		</tr>
	<?php 
		}
	?>
	<?php if(!$boMsg): ?>
		<tr>
			<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
				O sistema não encontrou pendências de preenchimento. 
			</td>
		</tr>
	<?php endif; ?>
	<tr>
		<td colspan="3" height="270px;" style="position:; marmargin-bottom: 100%; "></td>
	</tr>		
</table>
</td>
<td>
<?php 
if(!$db->testa_superuser()){
	//echo '<div style="position:relative; margin-top:-300px; margin-left:830px;">';
//	echo '<div style="position:relative; margin-top:-32%; margin-left:88%;">';
	wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg ) );
//	echo '</div>';
	if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $arperfil) ){
		if( $esdid == WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO ){
			desenhaBotaoAprovacao();
		}
	}
//}elseif( $escrita && !$boMsg ){
}elseif( $escrita ){
	//echo '<div style="position:relative;margin-top: -200px;margin-left: 830px;">';
//	echo '<div style="position:absolute;margin-top: -200px;margin-left: 88%;">';
		$arSituacao = array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO, WF_TIPO_EM_REFORMULACAO);
		if($db->testa_superuser()){
			wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg ));
			if( $esdid == WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO ){
				desenhaBotaoAprovacao();
			}
		}elseif(in_array($esdid, $arSituacao) && (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arperfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arperfil) || in_array(PAR_PERFIL_PREFEITO, $arperfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arperfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arperfil)) ){
			wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg ),  array('historico'=>true));	
			if( $esdid == WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO ){
				desenhaBotaoAprovacao();
			}
		}elseif( in_array(PAR_PERFIL_COORDENADOR_GERAL, $arperfil) ){
			wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg ));
			if( $esdid == WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO ){
				desenhaBotaoAprovacao();
			}
		}
		
//	echo '</div>';
}
if( in_array( PAR_PERFIL_ADMINISTRADOR,$arperfil ) ||
	in_array( PAR_PERFIL_SUPER_USUARIO,$arperfil ) ||
	in_array( PAR_PERFIL_COORDENADOR_GERAL,$arperfil )
){
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
<?php } }

	if( ( in_array( PAR_PERFIL_SUPER_USUARIO, $arperfil ) || in_array(PAR_PERFIL_ADMINISTRADOR, $arperfil) || in_array( PAR_PERFIL_COORDENADOR_GERAL, $arperfil ) ) && ( in_array( $esdid, array(WF_TIPO_VALIDACAO_DILIGENCIA) ) /*|| (in_array( $esdid, array(WF_TIPO_EM_CORRECAO) ))*/ ) ){
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
					} ?>
</td>
</tr>
</table>
<div id="dialog-aprovacaoRef" title="Aprovação de Reformulação de Pre-Obra" style="display:none;">
</div>
<script>
	function informarDia(preid, qrpid){
		url = 'par.php?modulo=principal/programas/proinfancia/visao/informarDia&acao=A&preid='+preid+'&qrpid='+qrpid+'&tipo=PAC';
		window.open(url,'Impressão do PAR',"height=350,width=550,scrollbars=yes,top=50,left=200" );
	}
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
