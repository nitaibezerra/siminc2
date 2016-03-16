<?php

require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

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

$escrita = true;

$picid = $_REQUEST['picid'];

$boAtivo = 'S';
$stAtivo = '';
$excluir = '';
$travaCorrecao = true;

if($_GET['requisicao'] == 'excluir' ){
	
	if(possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO)) && $boAtivo == 'S' ){
		$sql = "DELETE FROM public.arquivo_recuperado WHERE arqid = {$_GET['arqid']};
				DELETE FROM obras.preobrafotos WHERE arqid = {$_GET['arqid']}";
		if($db->executar($sql)){
			$sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
			$db->executar($sql);
			$db->commit();
		}				
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

$caminho_atual = 'par.php?modulo=principal/popupCadastraImagensItens&acao=A&picid='.$picid;

## UPLOAD DE ARQUIVO
$campos	= array("picid"	=> $picid,
				"pifdescricao" => "'".$_POST['fotdescricao']."'");
	
$file = new FilesSimec("propostaitemcomposicaofotos", $campos, 'par');
if($_FILES["Arquivo"] && $boAtivo == 'S'){
	
	if( substr($_FILES["Arquivo"]["type"],0,5) == 'image' ){
		$arquivoSalvo = $file->setUpload($_POST['fotdescricao']);	
	}else{
		echo '<script type="text/javascript"> 
					alert("Tipo de arquivo inválido. \n O arquivo deve ser uma imagem. ");
			  </script>';
	}
	if($arquivoSalvo){
		
		$sql = "SELECT 
					arqnome, 
					arq.arqid, 
					arq.arqextensao, 
					arq.arqtipo, 
					arq.arqdescricao,							
					to_char(arq.arqdata, 'DD/MM/YYYY') as data, 
					arc.arqvalidacao
				FROM 
					public.arquivo arq 
				LEFT JOIN 
					public.arquivo_recuperado arc ON arc.arqid = arq.arqid 
				INNER JOIN 
					par.propostaitemcomposicaofotos pif ON pif.arqid = arq.arqid 
				WHERE							
					picid = {$picid}
				AND							
					(substring(arqtipo,1,5) = 'image') 
				ORDER BY 
					arq.arqid";
				//LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
		$fotos = ($db->carregar($sql));				
		$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");				
	
		if( $fotos ){
			$_SESSION['imgparams'] = array("filtro" => "cnt.picid={$picid}", 
										   "tabela" => "par.propostaitemcomposicaofotos");
			//title=\"". $fotos[$k]["arqdescricao"] ."\"
			for( $k=0; $k < count($fotos); $k++ ){
				
				$html .= "<div style=\"float:left; width:90px; height:140px; text-align:center; margin:2px;\" ><img title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' src='../includes/highslide/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."&_sisarquivo=par' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=1&_sisarquivo=par&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>".$fotos[$k]["data"]."<br/>".$fotos[$k]["acao"]."</div>";
				
			}
			
		}else {
			$html = "Não existem fotos cadastradas";
		}
		
		$html = str_ireplace("'","\'",$html);
		
		echo "<script type='text/javascript'> 
					alert('Foto gravada com sucesso.');
					window.opener.jQuery( '#tdFotos' ).html( '".$html."' );
					document.location.href = '".$caminho_atual."';
			  </script>";
	}
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script>

var boAtivo = 'S';

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

});


function excluirFoto(url, arqid, fotid){
	if( boAtivo = 'S' ){
		if(confirm("Deseja realmente excluir esta foto ?")){
			window.location = url+'&fotid='+fotid+'&arqid='+arqid;
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
						<td align="center">
							<input class="enviar" type="button" value="Salvar" />
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

				</script>
				<?
				$sql = "SELECT 
							arqnome, 
							arq.arqid, 
							arq.arqextensao, 
							arq.arqtipo, 
							arq.arqdescricao,							
							to_char(arq.arqdata, 'DD/MM/YYYY') as data, 
							arc.arqvalidacao
						FROM 
							public.arquivo arq 
						LEFT JOIN 
							public.arquivo_recuperado arc ON arc.arqid = arq.arqid 
						INNER JOIN 
							par.propostaitemcomposicaofotos pif ON pif.arqid = arq.arqid 
						WHERE							
							picid = {$picid}
						AND							
							(substring(arqtipo,1,5) = 'image') 
						ORDER BY 
							arq.arqid";
						//LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
				$fotos = ($db->carregar($sql));				
				$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");				

				if( $fotos ){
					$_SESSION['imgparams'] = array("filtro" => "cnt.picid={$picid}", 
												   "tabela" => "par.propostaitemcomposicaofotos");
					//title=\"". $fotos[$k]["arqdescricao"] ."\"
					for( $k=0; $k < count($fotos); $k++ ){
						$restricao = '';
						if(!is_file(APPRAIZ."arquivos/par/".floor($fotos[$k]["arqid"]/1000)."/".$fotos[$k]["arqid"])) {
							$restricao = "<br /> <img src=../imagens/restricao.png align=absmiddle border=0><br/><input type=button name=b value=Substituir onclick=\"displayMessage(window.location.href+'&requisicao=telaSubirArquivo&arqid=".$fotos[$k]["arqid"]."',false);\">";
							$alerta = 'background-color: red;';
						}elseif($fotos[$k]["arqvalidacao"]=="f") {
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
								<img title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' src='../includes/highslide/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."&_sisarquivo=par' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=par&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>
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