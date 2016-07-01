<?php

$obPreObra 	= new PreObraControle();

if($_REQUEST['download'] == 's'){	
	
	$obPreObra->documentoDownloadAnexo($_GET['arqid']);
	die();
}

$preid = $_SESSION['par']['preid'] ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

if($_POST['acao'] == 'anterior'){
	$stAba = "listaObras";
}else{
	$stAba = "documentoFNDE";
}

$subid = $_REQUEST['preid'] ? $_REQUEST['preid'] : 1; 

if($_REQUEST['excluir'] == 's' ){
	
	if( possuiPerfil( array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_ENGENHEIRO_FNDE,PAR_PERFIL_COORDENADOR_GERAL) ) ){
		$sql = "DELETE FROM obras.preobradocumentosfnde WHERE arqid = {$_REQUEST['arqid']} AND preid = $preid";
		if($db->executar($sql)){
			$db->commit();
		}				
		echo '<script>
				alert("Arquivo excluído com sucesso!");
				document.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&preid='.$preid.'\';
			  </script>';
	}else{
		echo '<script>
				alert("Operação não permitida!");
				document.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&preid='.$preid.'\';
			  </script>';
	}
	exit;
}
			
## UPLOAD DE ARQUIVO
$campos	= array("preid"	=> $preid,
				"pdfdescricao" => "'".$_POST['pdfdescricao']."'");
	
$file = new FilesSimec("preobradocumentosfnde", $campos, 'obras');
if($_FILES["Arquivo"]){	
	
	$arquivoSalvo = $file->setUpload($_POST['fotdescricao']);
		
	if($arquivoSalvo){
		echo '<script type="text/javascript"> 
					alert("Arquivo anexado com sucesso.");
					document.location.href = \''.$caminho_atual.'\';
			  </script>';
	}
}
//$boAtivo = 'S';
$stAtivo = '';

$sql = "SELECT COALESCE(obr.obrpercentultvistoria, 0)FROM obras2.obras obr WHERE obr.preid = $preid";
	
$percexec = $db->pegaUm( $sql );
// nova situação, se tiver mais de 0% de execução da obra... desabilitar
if((float)$percexec > 0) {
	$boAtivo = 'N';
	$stAtivo = 'disabled="disabled"';
	$erro = 'A obra está aprovada e já possui vistorias no módulo de Monitoramento de Obras.
			<br>Não é permitido incluir documentos para obras que iniciaram a execução.';
}

cabecalho();

?>
<script>

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

		if(this.value == 'Anterior'){
			aba = 'listaObras';
		}

		document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
	});
});

function excluirArquivo(arqid){
	if(confirm("Deseja realmente excluir este arquivo ?")){
		document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&excluir=s&arqid='+arqid+'&preid=<?=$preid ?>';
	}
}

function baixarArquivo(arqid){

	if( arqid != '' ){
		document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&download=s&arqid='+arqid;
	}
}

</script>
<form action="" method="post" enctype="multipart/form-data"
	id="formulario" name="formulario">	
	<?php if( $erro != '' ){?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
		align="center">
		<tr>
			<td style="color: red; text-align: center">
				<?=$erro; ?>		
			</td>
		</tr>
	</table>
	<?php }?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita">Descrição do Arquivo:</td>
			<td><input type="hidden" name="acao" id="acao" value="" />						
				<?php echo campo_texto('pdfdescricao', 'S', $boAtivo, '', 30, 255, '', '') ?>						
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Arquivo:</td>
			<td><input type="file" name="Arquivo" <?=$stAtivo ?> <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?>></td>
		</tr>				
		<?php if($readonly != "readonly='readonly'"){?>
		<tr>
			<td colspan="2" class="SubTituloEsquerda">
				<table width="100%">
					<tr>
						<td align="left" width="30%"></td>
						<td align="center"><input class="enviar" type="button"
							value="Anexar" <?=$stAtivo ?> /></td>
						<td align="rigth" width="30%"></td>
					</tr>
				</table>
			</td>
		</tr>
		<?php }?>
		<tr>
			<td colspan="2" class="SubTituloCentro">
				<table width="95%" align="center" border="0" cellspacing="2"
					cellpadding="2" class="listagem">
					<tr bgcolor="">
						<td align="center" width="10%">Ação</td>
						<td align="center" width="70%">Descrição</td>
						<td align="center" width="20%">Data de Inclusão</td>
					</tr>
				<?php 
					$x = 0; 
					$sql = "SELECT 
								pdfdescricao as descricao, 
								arqid as codigo,
								to_char(pdfdatainclusao,'DD/MM/YYYY') as data
							FROM 
								obras.preobradocumentosfnde
							WHERE
								pdfstatus = 'A'
								AND preid = ".$_SESSION['par']['preid'];
					$arquivos = $db->carregar($sql);
					$arquivos = is_array($arquivos) ? $arquivos : array();
					foreach($arquivos as $arquivo){ 
						$x++;
						$cor = ($x % 2) ? "#F7F7F7" : "white"; 
				?>
					<tr bgcolor="<?php echo $cor ?>"
						onmouseover="this.bgColor='#ffffcc';"
						onmouseout="this.bgColor='<?php echo $cor ?>';">
						<td align="center">
						<?if( $boAtivo == 'S' ){ ?>
							<img class="excluir" src="../imagens/excluir.gif"
							align="absmiddle" title="Excluir documento"
							style="cursor: pointer; padding-right: 5px; padding-bottom: 5px;"
							onclick="excluirArquivo(<?=$arquivo['codigo'] ?>)" />
						<?} else { ?>
							<img class="excluir" src="../imagens/excluir_01.gif"
							align="absmiddle" title="Excluir documento"
							style="cursor: pointer; padding-right: 5px; padding-bottom: 5px;" "/>
						<?} ?>		 
						</td>
						<td align="center"><a
							onclick="baixarArquivo(<?=$arquivo['codigo'] ?>)"><?=$arquivo['descricao'] ?></a></td>
						<td align="center"><?=$arquivo['data'] ?></td>
					</tr>
				<?php 
					}
				?>
				</table>
			</td>
		</tr>
	</table>
</form>