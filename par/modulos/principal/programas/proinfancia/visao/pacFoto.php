<?php

if($_REQUEST['requisicao']=='telaSubirArquivo') {
	$dadosarquivo = $db->pegaLinha("SELECT arqnome||'.'||arqextensao as nomearquivo, arqtamanho FROM public.arquivo WHERe arqid='".$_REQUEST['arqid']."'");
	die("<script>
			function limpaUpload(arqid){document.getElementById('arquivo_' + arqid).value = \"\";}
		 </script>
		 <form method=post enctype=multipart/form-data id=formulario_ name=formulario>
		 	<input type=hidden name=requisicao value=subirarquivo>
		 	<input type=hidden name=_sisdiretorio value=obras>
		 	<table class=tabela>
		 		<tr>
		 			<td class=SubTituloDireita>Nome do arquivo:</td>
		 			<td>".$dadosarquivo['nomearquivo']."</td>
		 		</tr>
		 		<tr>
		 			<td class=SubTituloDireita>Tamanho (bytes):</td>
		 			<td>".$dadosarquivo['arqtamanho']."</td>
		 		</tr>
		 		<tr>
		 			<td class=SubTituloDireita>Selecione novo arquivo:</td>
		 			<td>
		 				<input type=file name=arquivo[".$_REQUEST['arqid']."] id=arquivo_".$_REQUEST['arqid']." > 
		 				<img onclick=limpaUpload('".$_REQUEST['arqid']."') src=../imagens/excluir.gif />
		 			</td>
		 		</tr>
		 		<tr>
		 			<td colspan=2 class=SubTituloCentro>
		 				<input type=button value=Enviar onclick=document.getElementById('formulario_').submit();> 
		 				<input type=button name=fechar value=Fechar onclick=closeMessage();>
		 			</td>
		 		</tr>
		 	</table>
		 </form>");
}


if($_REQUEST['requisicao']=='subirarquivo') {
	
	if($_FILES['arquivo']) {
		
		include APPRAIZ ."includes/funcoes_public_arquivo.php";
		
		$resp = atualizarPublicArquivo($arrValidacao = array());
		
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

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

$preid = $_SESSION['par']['preid'] ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

if($_POST['acao'] == 'anterior'){
	$stAba = "questionario";
}elseif($_POST['acao'] == 'proximo'){
	$stAba = "planilhaOrcamentaria";
}else{
	$stAba = "foto";
}

$caminho_atual = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='.$stAba.'&preid='.$preid;
$subid = $_REQUEST['preid'] ? $_REQUEST['preid'] : 1; 

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';
$excluir = '';
$travaCorrecao = true;

if( $esdid ){
	if( is_array($respSim) ){
		$travaCorrecao = !in_array(QUESTAO_FOTOS,$respSim);
	}
	
	
	$obSubacaoControle = new SubacaoControle();
	$obPreObra = new PreObra();
	
	if($preid){
		$arDados = $obSubacaoControle->recuperarPreObra($preid);
	}
	
	// Regra passada pelo Daniel - 9/6/11
	
	# Código refeito em 22/10/2012. Regra para liberação da tela para (cadastramento e ateração) dos perfil abaixos listados nas seguintes situações também listadas abaixo.
	# Foi inserido os perfis Estaduais e a situação em Diligência.
	# Foi também inserido o os perfis. (não havia perfil, era verificado apenas o estado).
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	if(in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) && $esdid == WF_TIPO_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
		$boAtivo = 'S';
		$stAtivo = '';
		$excluir = ", '<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao";
	}else{

		$arrReformulacao = Array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO, WF_TIPO_EM_ANALISE_DILIGENCIA, WF_TIPO_EM_REFORMULACAO, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL, WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL);
		if(	in_array($esdid, $arrReformulacao) &&
				(
						in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
						in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||				
						in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
						in_array(PAR_PERFIL_PREFEITO, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) ||
						in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
						in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)
				)
		){
			$boAtivo = 'S';
			$stAtivo = '';
			$excluir = ", '<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao";
		}
		
		/*
		if( ( WF_TIPO_EM_CORRECAO == $esdid || WF_TIPO_EM_REFORMULACAO == $esdid || WF_TIPO_EM_ANALISE_DILIGENCIA == $esdid ) && possuiPerfil($perfil) ){
			if(!$travaCorrecao){
				$boAtivo = 'N';
				$stAtivo = 'disabled="disabled"';
				$excluir = '';
			}
		}
		*/		
	}

	// nova situação, se tiver mais de 0% de execução da obra... desabilitar
	if((float)$arDados['percexec'] > 0) {
		$boAtivo = 'N';
		$stAtivo = 'disabled="disabled"';
		$excluir = '';
	}
	
	// Se for reformulação libera edição
	if(possuiPerfil(array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_PREFEITO, PAR_PERFIL_ENGENHEIRO_FNDE, PAR_PERFIL_COORDENADOR_GERAL)) && in_array($esdid, array(WF_TIPO_EM_CORRECAO, WF_TIPO_EM_REFORMULACAO))){
		$boAtivo = 'S';
		$stAtivo = '';
		$excluir = ", '<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao";
	}
}

/*
 * REGRA TEMPORARIA 02/05/2012
 * SOLICITADO PELO DANIEL AREAS
 * LIBERA EDIÇÃO PARA
 * MUNICIPIO COM OBRA
 * EM REFORMULAÇÃO
 * 
 * MUNICIPIOS: SORRISO/MT
 */
//if(in_array($_SESSION['par']['muncod'], array(5107925)) && in_array($esdid, array(WF_TIPO_EM_REFORMULACAO)) ){
//	$boAtivo = 'S';
//	$stAtivo = '';
//	$excluir = ", '<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao";
//}

if($_GET['requisicao'] == 'excluir' ){
	
	if(possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO)) && $boAtivo == 'S' ){
		
		$sql2 = "SELECT * FROM obras.preobra WHERE preidpai = ".$preid;
		$rsReformulacao = $db->pegaLinha($sql2);
		
		if(!$rsReformulacao){
			$sql .= "DELETE FROM public.arquivo_recuperado WHERE arqid = {$_GET['arqid']};";	
		}
		
		$sql .= "DELETE FROM obras.preobrafotos WHERE arqid = {$_GET['arqid']} and preid = ".$preid.";";
// 		ver($sql,d);
		if($db->executar($sql)){
// 			if(!$rsReformulacao){
// // 				$sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
// 				$db->executar($sql);
// 			}			
		}				
		$db->commit();
		echo '<script>
				alert("Foto excluída com sucesso!");
				document.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=foto&preid='.$preid.'\';
			  </script>';
	}else{
		echo '<script>
				alert("Operação não permitida!");
				document.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=foto&preid='.$preid.'\';
			  </script>';
	}
	exit;
}
?>				
<?php echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=foto&preid=".$preid, $preid, $descricaoItem ); ?>
<?php
## UPLOAD DE ARQUIVO
$campos	= array("preid"	=> $preid,
				"pofdescricao" => "'".$_POST['fotdescricao']."'");
	
$file = new FilesSimec("preobrafotos", $campos, 'obras');
if($_FILES["Arquivo"] && $boAtivo == 'S'){	
	
	if( substr($_FILES["Arquivo"]["type"],0,5) == 'image' ){
		$arquivoSalvo = $file->setUpload($_POST['fotdescricao']);	
	}else{
		echo '<script type="text/javascript"> 
					alert("Tipo de arquivo inválido. \n O arquivo deve ser uma imagem. ");
			  </script>';
	}
	if($arquivoSalvo){
		echo '<script type="text/javascript"> 
					alert("Foto gravada com sucesso.");
					document.location.href = \''.$caminho_atual.'\';
			  </script>';
	}
}
//echo "<script>
//		jQuery(document).ready(function(){
//			jQuery('.enviar').removeAttr('disabled'); 
//		});
//	  </script>";
?>
<script>

var boAtivo = '<?=$boAtivo ?>';

jQuery.noConflict();

jQuery(document).ready(function(){	

	jQuery('.enviar').click(function(){
		
		if(jQuery('input[name=fotdescricao]').val() == ''){
			alert('O campo descrição da foto é obrigatório.');
			jQuery('input[name=fotdescricao]').focus();
			return false;
		}
		
		if(this.value == 'Salvar'){
			jQuery('#acao').val('salvar');
		}

		if(this.value == 'Salvar e próximo'){			
			jQuery('#acao').val('proximo');
		}

		if(this.value == 'Salvar e anterior'){			
			jQuery('#acao').val('anterior');
		}		

		jQuery('.enviar').attr('disabled',true);
		
		document.formulario.submit();
	});

	jQuery('.navegar').click(function(){

		if(this.value == 'Próximo'){
			aba = 'planilhaOrcamentaria';
		}

		if(this.value == 'Anterior'){
			aba = 'questionario';
		}

		document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
	});
});

function excluirFoto(url, arqid, fotid){
	if( boAtivo = 'S' ){
		if(confirm("Deseja realmente excluir esta foto ?")){
			window.location = url+'&fotid='+fotid+'&arqid='+arqid+'&itemid='+<?php echo $preid ?>;
		}
	}
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
<?php monta_titulo( 'Cadastro de fotos', $obraDescricao  ); ?>
<?php echo cabecalho();?>
<?php if($boAtivo == 'S' && count($respSim)): ?>
	<?php
	$txtAjuda = "Encaminhar digitalmente fotografias em tamanho adequado, coloridas, devidamente identificadas, com resolução e em número suficientes para que se possa visualizar claramente o terreno proposto, seu entorno e quaisquer informações pertinentes, conforme o Relatório de Vistoria do Terreno.";
	$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">"; 
	?>
	<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
		<tr>
			<td width="100" style="text-align: right;" class="SubTituloDireita">Ajuda:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?php echo $imgAjuda ?>
			</td>
		</tr>
	</table>
<?php endif; ?>
<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">			
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Descrição da foto:</td>
			<td>
				<input type="hidden" name="acao" id="acao" value="" />						
				<?php echo campo_texto('fotdescricao', 'S', $boAtivo, '', 30, 255, '', '') ?>						
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Enviar foto:</td>
			<td>
				<input type="file" name="Arquivo" <?php echo $stAtivo ?>>							
			</td>
		</tr>				
		<tr>
			<td colspan="2" class="SubTituloEsquerda">
				<table width="100%">
					<tr>
						<td align="left">
							<input class="navegar" type="button" value="Anterior" />
						</td>
						<td align="center">
							<!-- input class="enviar" type="button" value="Salvar e anterior" <?php echo $stAtivo ?>/ -->
							<?php 
							if( $boAtivo == 'S' ){
							?>
								<input class="enviar" type="button" value="Salvar" />
							<?php 
							}
							?>
							<!-- input class="enviar" type="button" value="Salvar e próximo" <?php echo $stAtivo ?>/ -->
							<input class="fechar" type="button" value="Fechar" onclick="atualizarObra();" />
						</td>
						<td align="right">
							<input class="navegar" type="button" value="Próximo" />
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro">
				<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
				<script type="text/javascript" src="../includes/remedial.js"></script>
				<script type="text/javascript" src="../includes/superTitle.js"></script>
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
				<?
				$sql = "SELECT 
							arqnome, arq.arqid, 
							arq.arqextensao, arq.arqtipo, 
							arq.arqdescricao,							
						 	to_char(arq.arqdata, 'DD/MM/YYYY') as data, 
						 	arc.arqvalidacao
							{$excluir} 
						FROM 
							public.arquivo arq 
						LEFT JOIN 
							public.arquivo_recuperado arc ON arc.arqid = arq.arqid 
						INNER JOIN 
							obras.preobrafotos pof ON arq.arqid = pof.arqid
						INNER JOIN 
							obras.preobra pre ON pre.preid = pof.preid
						--INNER JOIN 
							--seguranca.usuario seg ON seg.usucpf = oar.usucpf 
						WHERE							
							pre.preid = {$preid} 
						AND							
							(substring(arqtipo,1,5) = 'image') 
						ORDER BY 
							arq.arqid";
						//LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
				$fotos = ($db->carregar($sql));				
				$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");				

				if( $fotos ){
					$_SESSION['imgparams'] = array("filtro" => "cnt.preid={$preid}", 
												   "tabela" => "obras.preobrafotos");
					//title=\"". $fotos[$k]["arqdescricao"] ."\"
					for( $k=0; $k < count($fotos); $k++ ){
						$restricao = '';
						if(!is_file(APPRAIZ."arquivos/obras/".floor($fotos[$k]["arqid"]/1000)."/".$fotos[$k]["arqid"])) {
							if($boAtivo == 'S'){
								$restricao = "<br /> <img src=../imagens/restricao.png align=absmiddle border=0><br/><input type=button name=b value=Substituir onclick=\"displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=".$fotos[$k]["arqid"]."',false);\">";
								$alerta = 'background-color: red;';
							}else{
								$restricao = '<br/><br/><input type="button" name="b" value="Substituir" disabled="disabled">';
							}
						}elseif($fotos[$k]["arqvalidacao"]=="f") {
							$restricao = "<img src=../imagens/restricao.png align=absmiddle border=0>";
							if( possuiPerfil(Array(PAR_PERFIL_COORDENADOR_GERAL, PAR_PERFIL_SUPER_USUARIO)) ){
								$restricao .= "<br /><input type=button name=b value=Validar onclick=\"validarFoto('".$fotos[$k]["arqid"]."');\">";
							}
							$restricao .= "<br /><input type=button name=b value=Substituir onclick=\"displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=".$fotos[$k]["arqid"]."',false);\">";
						} else {
							$restricao = "";
							$alerta = '';
						}
						
						echo "<div style=\"{$alerta}float:left; width:90px; height:140px; text-align:center; margin:2px;\" >
								" . $restricao . "
								<img title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."&_sisarquivo=obras' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=obras&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>
								" . $fotos[$k]["data"] . " <br/>
								" . $fotos[$k]["acao"] . "
							  </div>";
						
					}
					
				}else {
					echo "Não existem fotos cadastradas";
				}
				?>
			</td>
		</tr>					
		<!--<tr>
			<td align="center">
				<?
//				if(!$_REQUEST['pagina']) $_REQUEST['pagina'] = 0;
//				$sql = "SELECT COUNT(arq.arqid) AS totalregistros FROM obras.preobrafotos AS cnt 
//						LEFT JOIN public.arquivo AS arq ON arq.arqid = cnt.arqid 
//						WHERE preid = {$preid}  AND 
//						((substring(arqtipo,1,5) = 'image') )";
//				$paginacao = current($db->carregar($sql));
//				if($paginacao) {
//					for($i = 0; $i < ceil(current($paginacao)/16); $i++ ) {
//						$page[] = "<a href=?modulo=principal/album&acao=A&pagina=". $i .">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
//					}
//					if(count($page) > 1) {
//						echo implode(" | ", $page);
//					}
//				}
				?>
			</td>
		</tr>
	--></table>
</form>