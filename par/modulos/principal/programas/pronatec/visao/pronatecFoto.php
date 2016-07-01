<?php

if($_REQUEST['requisicao']=='validararquivo') {
	ob_clean();
	$db->executar("UPDATE public.arquivo_recuperado SET arqvalidacao=true WHERE arqid='".$_REQUEST['arqid']."'");
	$db->commit();
	die("TRUE");
}

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

$preid = $_SESSION['par']['preid'] ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

if($_POST['acao'] == 'anterior'){
	$stAba = "Questionario";
}elseif($_POST['acao'] == 'proximo'){
	$stAba = "Documento";
}else{
	$stAba = "Foto";
} 

$caminho_atual = 'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba='.$stAba.'&preid='.$preid;
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
	if(possuiPerfil($arrPerfil = array(PAR_PERFIL_COORDENADOR_GERAL)) && 
	   $esdid == WF_PRONATEC_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
		$boAtivo = 'S';
		$stAtivo = '';
		$excluir = ", '<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao";
	} else {
	
		$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO,PAR_PERFIL_SUPER_USUARIO);
		
		if( ($esdid == WF_PRONATEC_EM_CADASTRAMENTO) && possuiPerfil($arrPerfil) ){
			$boAtivo = 'S';
			$stAtivo = '';
			$excluir = ", '<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao";
		}
		
		if( ($esdid == WF_PRONATEC_EM_DILIGENCIA) && possuiPerfil($arrPerfil) ){
			
			if(!$travaCorrecao){
				$boAtivo = 'N';
				$stAtivo = 'disabled="disabled"';
				$excluir = '';
			}else{
				$boAtivo = 'S';
				$stAtivo = '';
				$excluir = ", '<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao";
			}
		}
	
	}
}

if($_GET['requisicao'] == 'excluir' ){
	
	if(possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO)) && $boAtivo == 'S' ){
		$sql = "DELETE FROM public.arquivo_recuperado WHERE arqid = {$_GET['arqid']};
				DELETE FROM obras.preobrafotos WHERE pofid = {$_GET['fotid']}";
		if($db->executar($sql)){
			$sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
			$db->executar($sql);
			$db->commit();
		}				
		echo '<script>
				alert("Foto excluída com sucesso!");
				document.location.href = \'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Foto&preid='.$preid.'\';
			  </script>';
	}else{
		echo '<script>
				alert("Operação não permitida!");
				document.location.href = \'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Foto&preid='.$preid.'\';
			  </script>';
	}
	exit;
}

$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Foto".$preidTx;

echo carregaAbasPronatec($lnkabas);
monta_titulo( 'Cadastro de Fotos do Imóvel', ''  );

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

		document.location.href = 'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
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
<?php 
	echo cabecalho();
	if($boAtivo == 'S' && count($respSim)){
		
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
<?php 
	}
?>
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
							<?php 
							if( $boAtivo == 'S' ){
							?>
								<input class="enviar" type="button" value="Salvar" />
							<?php 
							}
							?>
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
				$fotos = ($db->carregar($sql));				
				$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");				

				if( $fotos ){
					$_SESSION['imgparams'] = array("filtro" => "cnt.preid={$preid}", 
												   "tabela" => "obras.preobrafotos");
					for( $k=0; $k < count($fotos); $k++ ){
						$restricao = '';
						if($fotos[$k]["arqvalidacao"]=="f") {
							$restricao = "<img src=../imagens/restricao.png align=absmiddle border=0>";
							if( possuiPerfil(Array(PAR_PERFIL_COORDENADOR_GERAL,PAR_PERFIL_SUPER_USUARIO)) ){
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
		</table>
</form>