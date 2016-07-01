<?php

if($_REQUEST['requisicao']=='telaSubirArquivo') {
	$dadosarquivo = $db->pegaLinha("SELECT arqnome||'.'||arqextensao as nomearquivo, arqtamanho FROM public.arquivo WHERe arqid='".$_REQUEST['arqid']."'");
	die("<script>function limpaUpload(arqid){document.getElementById('arquivo_' + arqid).value = \"\";}</script><form method=post enctype=multipart/form-data id=formulario_ name=formulario><input type=hidden name=requisicao value=subirarquivo><input type=hidden name=_sisdiretorio value=obras><table class=tabela><tr><td class=SubTituloDireita>Nome do arquivo:</td><td>".$dadosarquivo['nomearquivo']."</td></tr><tr><td class=SubTituloDireita>Tamanho (bytes):</td><td>".$dadosarquivo['arqtamanho']."</td></tr><tr><td class=SubTituloDireita>Selecione novo arquivo:</td><td><input type=file name=arquivo[".$_REQUEST['arqid']."] id=arquivo_".$_REQUEST['arqid']." > <img onclick=limpaUpload('".$_REQUEST['arqid']."') src=../imagens/excluir.gif /></td></tr><tr><td colspan=2 class=SubTituloCentro><input type=button value=Enviar onclick=document.getElementById('formulario_').submit();> <input type=button name=fechar value=Fechar onclick=closeMessage();></td></tr></table></form>");
}

if($_REQUEST['requisicao']=='subirarquivo') {
	
	if($_FILES['arquivo']) {
		
		include APPRAIZ ."includes/funcoes_public_arquivo.php";
		
		$resp = atualizarPublicArquivo($arrValidacao = array(''));
		
		if($resp['TRUE']) $msg = 'Arquivo atualizado com sucesso';
		else {
			if($resp['FALSE']) {
				$msg .= 'Problemas encontrados:'.'\n';
				foreach($resp['FALSE'] as $k => $v) {
					$msg .= $v .'\n';
				}
			}
		}
		
		die("<script>
				alert('".$msg."');
				window.location = window.location;
			 </script>");
	}
	
}

if($_REQUEST['requisicao']=='validararquivo') {
	ob_clean();
	$db->executar("UPDATE public.arquivo_recuperado SET arqvalidacao=true WHERE arqid='".$_REQUEST['arqid']."'");
	$db->commit();
	die("TRUE");
}


$obPreObra 	= new PreObraControle();
$db 		= new cls_banco();

$preid = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];
$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$arTipoObraDocumentos = $obPreObra->recuperarTiposObraDocumentosProInfancia($preid, true);
$arTipoObraDocumentos = $arTipoObraDocumentos ? $arTipoObraDocumentos : array();

if($_REQUEST['download'] == 's'){	
	
	$obPreObra->documentoDownloadAnexo($_GET['arqid']);
	die();
}

$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';
if( $esdid ){
	
	$obSubacaoControle2 = new SubacaoControle();
	$obPreObra2 = new PreObra();
	
	if($preid){
		$arDados = $obSubacaoControle2->recuperarPreObra($preid);
	}
	
	// Regra passada pelo Daniel - 9/6/11
	if(possuiPerfil($arrPerfil = array(PAR_PERFIL_COORDENADOR_GERAL)) && $esdid == WF_TIPO_OBRA_APROVADA && $arDados['ptoid'] == OBRA_TIPO_A) {
		$boAtivo = 'S';
		$stAtivo = '';
	} else {
		$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO,PAR_PERFIL_SUPER_USUARIO);
		$arrSituacao = array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO, WF_TIPO_EM_REFORMULACAO);
		if( in_array($esdid, $arrSituacao) && possuiPerfil($arrPerfil) ){
			$boAtivo = 'S';
			$stAtivo = '';
		}
	}
}

if( $_POST['poasituacao'] && $boAtivo == 'S' ){
	
	$sql = "UPDATE obras.preobraanexo SET poasituacao = 'A' WHERE preid = {$preid}";
	$db->executar($sql);
	
	$arDados = $_POST['poasituacao'];
	foreach($arDados as $podid){
		$sql = "UPDATE obras.preobraanexo SET poasituacao = 'B' WHERE preid = {$preid} AND podid = {$podid}";
		$db->executar($sql);
	}
	$db->commit();
}

if( $_GET['arqidDel'] && $boAtivo == 'S' ){
	$obPreObra->excluirDocumentosPreObra($_GET['arqidDel'],$preid, true);
}

if($esdid == WF_TIPO_EM_ANALISE_FNDE){
	$boAnalise = true;
}
?>
<script language="javascript" type="text/javascript">

	var boAtivo = '<?=$boAtivo?>';

	jQuery(document).ready(function(){

		jQuery('.navegar').click(function(){

			if(this.value == 'Anterior'){
				aba = 'cronograma';
			}

			if(this.value == 'Próximo'){
				aba = 'analise';
			}

			document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
		});

		jQuery('.excluir').click(function(){

			if(!confirm('Deseja deletar este documento?')){
				return false;
			}
			document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoTipoA&preid='+<?php echo $preid ?>+'&arqidDel='+this.id;
		});

		jQuery("#formulario").validate({
			ignoreTitle: true,
			rules: {
				podid: "required",
				arquivo: "required",
				poadescricao: "required"
			}
		});

		jQuery(".modelo").click(function(){

			var arDados = this.id.split("_");
			
			if(arDados[0] == 8 || arDados[0] == 9){
				return window.open('documentos/modeloDados.php?modelo='+arDados[0]+'&preid='+arDados[1], 
						   'modelo', 
						   "height=600,width=950,scrollbars=yes,top=50,left=200" );
			}else{
				return window.open('documentos/modelo.php?modelo='+arDados[0]+'&preid='+arDados[1], 
						   'modelo', 
						   "height=600,width=950,scrollbars=yes,top=50,left=200" );
			}
		});		

		jQuery(".anexo").click(function(){
			document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoTipoA&preid='+<?php echo $preid ?>+'&download=s&arqid='+this.id;
		});
	});
	 
	function excluirAnexo( arqid ){
		var preid = jQuery('#preid').val();
		if( boAtivo == 'S' ){
	 		if ( confirm( 'Deseja excluir o Documento?' ) ) {
	 			location.href= window.location+'&arqidDel='+arqid+'&preid='+preid;
	 		}
		}
 	}

 	function popup_arquivo(podid, preid, sisid)
 	{
 	 	if(podid == 'disabled'){
			alert("Documento já analisado e bloqueado pelo FNDE.");
			return false;
 	 	}
 	 	
 		return window.open('geral/upload_arquivo.php?podid='+podid+'&preid='+preid+'&sisid='+sisid, 
				   'anexo', 
				   "height=300,width=450,scrollbars=yes,top=50,left=200" );
 	}

 	function popup_ajuda(link)
 	{
 		return window.open('geral/popup_ajuda.php?link='+link, 
				   'ajuda', 
				   "height=300,width=450,scrollbars=yes,top=50,left=200" );
 	}
 
</script>
<?php 
echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoTipoA&preid=".$preid, $preid, $descricaoItem);
monta_titulo( 'Documentos anexo', $obraDescricao  );
?>
<?php echo cabecalho();?>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript">
				messageObj = new DHTML_modalMessage();	// We only create one object of this class
				messageObj.setShadowOffset(5);	// Large shadow
				
				function displayMessage(url) {
					messageObj.setSource(url);
					messageObj.setCssClassMessageBox(false);
					messageObj.setSize(690,400);
					messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
					messageObj.display();
				}
				function displayStaticMessage(messageContent,cssClass) {
					messageObj.setHtmlContent(messageContent);
					messageObj.setSize(600,150);
					messageObj.setCssClassMessageBox(cssClass);
					messageObj.setSource(false);	// no html source since we want to use a static message here.
					messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
					messageObj.display();
				}
				function closeMessage() {
					messageObj.close();	
				}
				
 	function validarFoto(arqid){
		if(confirm('Deseja realmente validar esta foto ?')){
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "requisicao=validararquivo&arqid="+arqid,
		   		success: function(msg){
		   			if(msg=="TRUE") {
		   				alert("Arquivo validado com sucesso");
		   				window.location=window.location;
		   			} else {
		   				alert("Arquivo não validado com sucesso");
		   				window.location=window.location;
		   			}
		   		}
		 		});
			
		}
	}
</script>
<form action="" method="post">
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td valign="top" width="40" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Item</b></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Descrição</b></td>
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Anexo(s)</b></td>
					<?php if($boAnalise): ?>
						<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Bloquear</b></td>
					<?php endif; ?>				
					<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>Ações</b></td>
				</tr>
			</thead>
			<tbody>
				<?php $x = 0; ?>
				<?php foreach($arTipoObraDocumentos as $tipos): ?>
					<?php
					$cor = ($x % 2) ? "#F7F7F7" : "white"; 
					?>
					<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';">
						<td align="center"><?php echo $x+1 //$tipos['codigo'] ?></td>			
						<td><?php echo $tipos['descricao'] ?></td>
						<td align="right" width="350px">
							<table width="100%" border="0" cellspacing="0" cellpadding="0">
								<tr>
									<td>
										<?php 
										$respSim = $respSim ? $respSim : array();
										$trav = true;
	
										if( ($esdid == WF_TIPO_EM_CADASTRAMENTO) && possuiPerfil($arrPerfil) ){
											$trav = false;
										}
										if(possuiPerfil(array(PAR_PERFIL_COORDENADOR_GERAL)) && $esdid == WF_TIPO_OBRA_APROVADA && $arDados['ptoid'] == OBRA_TIPO_A) {
											$trav = false;
										}
										if( ($esdid == WF_TIPO_EM_CORRECAO) && possuiPerfil($arrPerfil) ){
											switch ($tipos['codigo']) {
											    case "3":
											        $trav = in_array(QUESTAO_ESTUDO_DEM,$respSim);
											        break;
											    case "1":
											        $trav = in_array(QUESTAO_PLANILHA_LOCALIZACAO,$respSim);
											        break;
											    case "4":
											        $trav = in_array(QUESTAO_PLANILHA_SIT,$respSim);
											        break;
											    case "6":
											        $trav = in_array(QUESTAO_PLANILHA_LOCACAO,$respSim);
											        break;
											    case "5":
											        $trav = in_array(QUESTAO_PLANIALTIMETRICO,$respSim);
											        break;
											    case "9":
											        $trav = in_array(QUESTAO_DOMINIALLIDADE,$respSim);
											        break;
											    case "7":
											        $trav = in_array(QUESTAO_FORNECIMENTO_INFRA,$respSim);
											        break;
											    case "8":
											        $trav = in_array(QUESTAO_COMPATIBILIDADE_FUNDACAO,$respSim);
											        break;
											}
										}
										if( ($esdid == WF_TIPO_EM_REFORMULACAO) && possuiPerfil($arrPerfil) ){
											$trav = false;
										}
										$arAnexos = $obPreObra->recuperarDocumentosAnexoPorPodid($tipos['codigo'], $preid);
										$arAnexos = $arAnexos ? $arAnexos : array();
										$Anexar = true;
										echo '<table width=100% border="0" cellspacing="0" cellpadding="0">';
										foreach($arAnexos as $anexo){
											$excluir = '';
											$subtituir = true;
											$restricao = '';
											if($tipos['situacao'] == 'B' || $boAtivo == 'N' || $trav){
												$excluir = '<img src="../imagens/excluir_01.gif" align="absmiddle" title="Excluir documento" style="padding-right:5px;padding-bottom:5px;" />';
											}else{
												if( $esdid == WF_TIPO_EM_CORRECAO ){
													$subtituir = true;
												}
												$excluir = '<img class="excluir" id="'.$anexo['codigo'].'" src="../imagens/excluir.gif" align="absmiddle" title="Excluir documento" style="cursor:pointer;padding-right:5px;padding-bottom:5px;" />';
											}
											$arqvalidacao = $db->pegaUm("SELECT arqvalidacao FROM public.arquivo_recuperado WHERE arqid='".$anexo['codigo']."'");
											if(!is_file(APPRAIZ."arquivos/obras/".floor($anexo['codigo']/1000)."/".$anexo['codigo'])) {
												if( $esdid == WF_TIPO_EM_CADASTRAMENTO ){
													$Anexar   = false;
												}
												if( $subtituir ){
													$restricao = "<img src=../imagens/restricao.png align=absmiddle border=0> <input type=button name=b value=Substituir onclick=\"displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=".$anexo['codigo']."',false);\">";
												}else{
													$restricao = "";
												}
												$alerta = 'background-color: red;';
											}elseif($arqvalidacao=="f") {
												if( possuiPerfil(Array(PAR_PERFIL_COORDENADOR_GERAL,PAR_PERFIL_SUPER_USUARIO)) ){
													$restricao = "<br /> <img src=../imagens/restricao.png align=absmiddle border=0> <input type=button name=b value=Validar onclick=\"validarFoto('".$anexo['codigo']."');\">";
												}
												$restricao .= "<br /><img src=../imagens/restricao.png align=absmiddle border=0> <input type=button name=b value=Substituir onclick=\"displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=".$anexo['codigo']."',false);\">";
											} else {
												$restricao = "&nbsp;";
												$alerta = '';
											}
											
											
											echo '<tr><td width=60%>'.$excluir.' <a title="Baixar arquivo" href="javascript:void(0)" id="'.$anexo['codigo'].'" class="anexo" style="'.$alerta.'">'.delimitador($anexo['descricao'], 20).'.'.$anexo['extensao'].'</a></td><td>'.$restricao.'</td></tr>';
										}
										echo '</table>';
										?>
									</td>
									<td width="70px;" align="center">
									<?php if( $Anexar ){?>
										<input type="button" value="Anexar" onclick="popup_arquivo('<?php echo ($tipos['situacao'] == 'B') ? "disabled" : $tipos['codigo'] ?>', '<?php echo $preid ?>', '<?php echo SIS_OBRAS ?>')" <?php echo $stAtivo ?> <?= $trav ? 'disabled="disabled"' : ''; ?>/>
									<?php }?>
									</td>
								</tr>						
							</table>															
						</td>
						<?php if($boAnalise): ?>
							<td width="80px" align="center">
								<input type="checkbox" name="poasituacao[]" value="<?php echo $tipos['codigo'] ?>" <?php echo ($tipos['situacao'] == 'B') ? "checked" : "" ?>>
							</td>
						<?php endif; ?>
						<td align="right" width="80">
							<?php if($tipos['anexo']): ?>
								<img alt="Contém anexo(s)" title="Contém anexo(s)" src="../imagens/clipe.gif">
							<?php endif; ?>
							<?php if(!empty($tipos['podmodelo'])): ?>
								<img class="modelo" id="<?php echo $tipos['codigo']."_".$preid ?>" alt="Visualizar o modelo" title="Visualizar o modelo" src="../imagens/consultar.gif" style="cursor:pointer;">
							<?php endif; ?>
							<?php if(!empty($tipos['podajuda'])): ?>
								<a href="<?php echo str_replace('par/','',$tipos['podajuda']) ?>" target="_blank">
									<img alt="Orientações" title="Orientações" src="../imagens/IconeAjuda.gif" border="0" style="cursor:pointer;">
								</a>
							<?php endif; ?>
						</td>
					</tr>
					<?php $x++ ?>
				<?php endforeach; ?>
			</tbody>
	</table>
	<table width="95%" align="center" bgcolor="#DCDCDC">
		<tr>
			<td align="left">
				<input class="navegar" type="button" value="Anterior" />
			</td>
			<td align="center">
				<?php if($boAtivo == 'S'): ?>
					<input type="submit" value="Salvar" />
				<?php endif; ?>
			</td>
			<td align="right">
				<input class="navegar" type="button" value="Próximo" />
			</td>
		</tr>
	</table>
</form>