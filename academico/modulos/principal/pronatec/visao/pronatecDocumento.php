<?php

function delimitador($texto, $valor = null){
	
	if(!ereg('[^0-9]',$texto)){
		return $texto;
	}
	
	if(!$valor){
		$valor = 280;
	}
	
	if(strlen($texto) > $valor){
			$texto = substr($texto,0,$valor).'...';
	}
		
	return $texto;
}

if($_REQUEST['requisicao']=='validararquivo') {
	ob_clean();
	$db->executar("UPDATE public.arquivo_recuperado SET arqvalidacao=true WHERE arqid='".$_REQUEST['arqid']."'");
	$db->commit();
	die("TRUE");
}

$obPreObra 	= new PreObraControle();
//$db 		= new cls_banco();

$preid = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];
$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$respSim = $respSim ? $respSim : array();

$arTipoObraDocumentos = $obPreObra->recuperarArTiposObraDocumentos($preid);
$arTipoObraDocumentos = $arTipoObraDocumentos ? $arTipoObraDocumentos : array();

$boDocDominialidade = $obPreObra->verificaDocumentoDominialidade($preid);

if($_REQUEST['download'] == 's'){	
	
	$obPreObra->documentoDownloadAnexo($_GET['arqid']);
	die();
}

$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';

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

			document.location.href = 'academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
		});

		jQuery(".anexo").click(function(){
			document.location.href = 'academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=documento&preid='+<?php echo $preid ?>+'&download=s&arqid='+this.id;
		});
	});
	 
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
$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Documento".$preidTx;

echo carregaAbasPronatec($lnkabas);
monta_titulo( 'Documentos Anexos', ''  );
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
										$trav = true;
	
										$arAnexos = $obPreObra->recuperarDocumentosAnexoPorPodid($tipos['codigo'], $preid);
										$arAnexos = $arAnexos ? $arAnexos : array();
										$Anexar   = true;
										echo '<table width=100% border="0" cellspacing="0" cellpadding="0">';
										foreach($arAnexos as $anexo){
											$excluir = '';
											$subtituir = true;
											$restricao = '';
											if($tipos['situacao'] == 'B' || $boAtivo == 'N' || $trav){
//												echo '<img src="../imagens/excluir_01.gif" align="absmiddle" title="Excluir documento" style="padding-right:5px;padding-bottom:5px;" />';
											}else{
												if( $esdid == WF_PRONATEC_EM_DILIGENCIA ){
													$subtituir = true;
												}
												$excluir = '<img class="excluir" id="'.$anexo['codigo'].'" src="../imagens/excluir.gif" align="absmiddle" title="Excluir documento" style="cursor:pointer;padding-right:5px;padding-bottom:5px;" />';
											}
											$arqvalidacao = $db->pegaUm("SELECT arqvalidacao FROM public.arquivo_recuperado WHERE arqid='".$anexo['codigo']."'");
											if(!is_file(APPRAIZ."arquivos/obras/".floor($anexo['codigo']/1000)."/".$anexo['codigo'])) {
												if( $esdid == WF_PRONATEC_EM_CADASTRAMENTO ){
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
								<?php if(!$boDocDominialidade && $tipos['codigo'] == 7): ?>
									<a href="javascript:alert('Deve ser gerada a declaração de dominialidade primeiro.')" >
										<img border="0" alt="Visualizar o modelo" title="Visualizar o modelo" src="../imagens/consultar.gif" style="cursor:pointer;">
									</a>
								<?php else: ?>
									<img class="modelo" id="<?php echo $tipos['codigo']."_".$preid ?>" alt="Visualizar o modelo" title="Visualizar o modelo" src="../imagens/consultar.gif" style="cursor:pointer;">
								<?php endif; ?>
							<?php endif; ?>
							<?php if(!empty($tipos['podajuda'])): ?>
								<a href="<?php echo str_replace('par/','',$tipos['podajuda']) ?>" target="_blank">
									<img alt="Orientações" title="Orientações" src="../imagens/IconeAjuda.gif" border="0" style="cursor:pointer;">
								</a>
							<?php endif; ?>
							<?php if(!$trav && count($respSim)): ?>
								<?php
								$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">";
								echo $imgAjuda; 
								?>
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