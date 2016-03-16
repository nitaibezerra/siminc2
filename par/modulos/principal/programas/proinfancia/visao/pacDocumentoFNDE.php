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

$caminho_atual = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='.$stAba.'&preid='.$preid;
$subid = $_REQUEST['preid'] ? $_REQUEST['preid'] : 1; 

if($_REQUEST['excluir'] == 's' ){
	
	if( possuiPerfil( array(PAR_PERFIL_SUPER_USUARIO,PAR_PERFIL_ENGENHEIRO_FNDE,PAR_PERFIL_COORDENADOR_GERAL) ) ){
		$sql = "DELETE FROM obras.preobradocumentosfnde WHERE arqid = {$_REQUEST['arqid']} AND preid = $preid";
		if($db->executar($sql)){
// 			$sql = "DELETE FROM public.arquivo WHERE arqid = {$_REQUEST['arqid']}";
// 			$db->executar($sql);
			$db->commit();
		}				
		echo '<script>
				alert("Arquivo exclu�do com sucesso!");
				document.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&preid='.$preid.'\';
			  </script>';
	}else{
		echo '<script>
				alert("Opera��o n�o permitida!");
				document.location.href = \'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&preid='.$preid.'\';
			  </script>';
	}
	exit;
}
			
echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=documentoFNDE&preid=".$preid, $preid, $descricaoItem );

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
$boAtivo = 'S';
$stAtivo = '';

//$sql = "select COALESCE(oi.obrpercexec, 0)from obr as.o brainfraestrutura oi where oi.preid = $preid";

$sql = "SELECT COALESCE(obr.obrpercentultvistoria, 0)FROM obras2.obras obr WHERE obr.preid = $preid";
	
$percexec = $db->pegaUm( $sql );
// nova situa��o, se tiver mais de 0% de execu��o da obra... desabilitar
if((float)$percexec > 0) {
	$boAtivo = 'N';
	$stAtivo = 'disabled="disabled"';
	$erro = 'A obra est� aprovada e j� possui vistorias no m�dulo de Monitoramento de Obras.
			<br>N�o � permitido incluir documentos para obras que iniciaram a execu��o.';
}

?>
<script>

jQuery.noConflict();

jQuery(document).ready(function(){	

	jQuery('.enviar').click(function(){
		
		if(jQuery('input[name=fotdescricao]').val() == ''){
			alert('O campo descri��o da foto � obrigat�rio.');
			jQuery('input[name=fotdescricao]').focus();
			return false;
		}
		
		if(this.value == 'Salvar'){
			jQuery('#acao').val('salvar');
		}

		if(this.value == 'Salvar e pr�ximo'){			
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
<?php monta_titulo( 'Documentos FNDE', $obraDescricao  ); ?>
<?php echo cabecalho();?>
<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">	
	<?php if( $erro != '' ){?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td style="color:red;text-align:center">
				<?=$erro; ?>		
			</td>
		</tr>
	</table>
	<?php }?>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Descri��o do Arquivo:</td>
			<td>
				<input type="hidden" name="acao" id="acao" value="" />						
				<?php echo campo_texto('pdfdescricao', 'S', 'S', '', 30, 255, '', '') ?>						
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Arquivo:</td>
			<td>
				<input type="file" name="Arquivo" <?=$stAtivo ?>>							
			</td>
		</tr>				
		<tr>
			<td colspan="2" class="SubTituloEsquerda">
				<table width="100%">
					<tr>
						<td align="left" width="30%">
							<input class="navegar" type="button" value="Anterior" />
						</td>
						<td align="center">
							<input class="enviar" type="button" value="Anexar" <?=$stAtivo ?> />
							<input class="fechar" type="button" value="Fechar" onclick="atualizarObra();" />
						</td>
						<td align="rigth" width="30%">
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro">
				<table width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<tr bgcolor="">
						<td align="center" width="10%">A��o</td>
						<td align="center" width="70%">Descri��o</td>
						<td align="center" width="20%">Data de Inclus�o</td>
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
					<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';">
						<td align="center">
						<?if( $boAtivo == 'S' ){ ?>
							<img class="excluir" src="../imagens/excluir.gif" align="absmiddle" 
								 title="Excluir documento" style="cursor:pointer;padding-right:5px;padding-bottom:5px;" 
								 onclick="excluirArquivo(<?=$arquivo['codigo'] ?>)"/>
						<?} else { ?>
							<img class="excluir" src="../imagens/excluir_01.gif" align="absmiddle" 
								 title="Excluir documento" style="cursor:pointer;padding-right:5px;padding-bottom:5px;""/>
						<?} ?>		 
						</td>
						<td align="center"><a onclick="baixarArquivo(<?=$arquivo['codigo'] ?>)"><?=$arquivo['descricao'] ?></a></td>
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