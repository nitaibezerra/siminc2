<?php 

if(!$_SESSION['proinfantil']['mds']['entcodent']){
	$_SESSION['proinfantil']['mds']['entcodent'] = $_REQUEST['entcodent'];
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_mds.php";
echo "<br/>";

// Chama metodo
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}

$abacod_tela = 57559;
$arMnuid = array();

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Fotos', $titulo2);

cabecalhoMds();
?>
<style>
	.field_foto {text-align:right}
	.field_foto legend{font-weight:bold;}
	.img_add_foto{cursor:pointer}
	.hidden{display:none}
	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<?php 

$habilitado = 'S';

$sql = "select distinct
				sal.salid,
				tis.tisdescricao
			from
				proinfantil.tiposala tis
			inner join
				proinfantil.sala sal ON sal.tisid = tis.tisid
			where
				sal.modid = 2
			order by
				tis.tisdescricao";

$arrSalas = $db->carregar($sql);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">				
	<?php if($arrSalas): ?>
		<?php $i=0 ?>
		<?php foreach($arrSalas as $sala): ?>
			<?php echo $i%2 == 0 ? "<tr>" : "" ?>
			<td width="50%" valign="top" id="td_sala_<?php echo $sala['salid'] ?>">
				<fieldset id="field_sala_<?php echo $sala['salid'] ?>" class="field_foto" >
					<legend>Fotos - <?php echo $sala['tisdescricao'] ?></legend>
					<table width="100%">
						<tr>
							<td valign="top">
								<?php 

								$sql = "select
											arq.arqid,
											arq.arqnome||'.'||arq.arqextensao,
											arq.arqdescricao,
											arq.arqtamanho
										from 
											proinfantil.fotos fot
										inner join
											public.arquivo arq ON arq.arqid = fot.arqid
										inner join 
											proinfantil.suplementacaomds mds on mds.mdsid = fot.mdsid
										where
											fot.salid = {$sala['salid']}
										and
											mds.entcodent = '{$_SESSION['proinfantil']['mds']['entcodent']}'
										and
											fot.fotstatus = 'A'";
								
								$arrFotos = $db->carregar($sql);
								
								?>
								<?php if($arrFotos): ?>
									<?php foreach($arrFotos as $foto): ?>
										<?php $pagina = floor($n/16); ?>
										<?php
												$imgend = APPRAIZ.'arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
										
												if(is_file($imgend)){
													$img_max_dimX = 100;
													$img_max_dimY = 85;
													
													$imginfo = getimagesize($imgend);
													
													$width = $imginfo[0];
													$height = $imginfo[1];
												
													if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
														if ($width > $height){
														  	$w = $width * 0.9;
															  while ($w > $img_max_dimX){
																  $w = $w * 0.9;
															  }
															  $w = round($w);
															  $h = ($w * $height)/$width;
														  }else{
															  $h = $height * 0.9;
															  while ($h > $img_max_dimY){
																  $h = $h * 0.9;
															  }
															  $h = round($h);
															  $w = ($h * $width)/$height;
														  }
													}else{
														  $w = $width;
														  $h = $height;
													}
													
													$tamanho = " width=\"$w\" height=\"$h\" ";
												}else{
													$tamanho = "";
												}
										?>
										<div id="div_foto_<?php echo $foto['arqid'] ?>" class="div_foto">
											<?php if($habilitado == 'S'){ ?>
												<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoSala('<?php echo $foto['arqid'] ?>')" />
											<?php } ?>
											<img onClick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>','<?php echo $sala['salid'] ?>','<?php echo $pinid ?>')" <?php echo $tamanho ?> onmouseover="return escape('<b>Descrição:</b> <?php echo $foto['arqdescricao'] ?><br /><b>Tamanho(Kb):</b> <?php echo $foto['arqtamanho'] ? round($foto['arqtamanho']/1024,2) : "N/A" ?>');" class="img_foto" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?=(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]) ?>" />
										</div>
									<?php endforeach; ?>
								<?php endif; ?>
							</td>
							<?php if($habilitado == 'S'){ ?>
								<td valign="top" width="10"><img class="img_add_foto" src="../imagens/gif_inclui.gif" title="Adicionar Foto" onclick="addFoto('<?php echo $sala['salid'] ?>')"  /></td>
							<?php } ?>
						</tr>
					</table>
				</fieldset>
			</td>
			<?php echo $i%2 == 1 ? "</tr>" : "" ?>
			<?php $i++ ?>
		<?php endforeach; ?>
	<?php endif; ?>
</table>

<?php 

$html = '<form id="form_upload" name="form_upload" method=post enctype="multipart/form-data" >
			<input type="hidden" name="salid" id="salid" value="" />
			<input type="hidden" name="mdsid" id="mdsid" value="'.$_SESSION['proinfantil']['mds']['mdsid'].'" />
			<input type="hidden" name="requisicao" value="salvarFotosSalaMds" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td width="30%" class="SubtituloDireita" >Foto:</td>
					<td><input type="file" name="arquivo" /></td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >Descrição:</td>
					<td>'.campo_texto("arqdescricao","S","S","",40,60,"","","","","","","").'</td>
				</tr>
				<tr>
					<td class="SubtituloDireita" ></td>
					<td class="SubtituloEsquerda" >
						<input type="button" name="btn_upload" value="Salvar" />
						<input type="button" name="btn_cancelar" value="Cancelar" />
					</td>
				</tr>
			</table>
	   </form>'; 
			   
popupAlertaGeral($html,"400px","120px","div_sub_foto","hidden");
	   
?>

<script>

$(function() {
	
	 $('[name=btn_upload]').click(function() {
	 	var erro = 0;
	 	if(!$('[name=arquivo]').val()){
	 		alert('Favor selecionar a foto!');
	 		erro = 1;
	 		return false;
	 	}
	 	if(!$('[name=arqdescricao]').val()){
	 		alert('Favor informar uma descrição!');
	 		erro = 1;
	 		return false;
	 	}
	 	if(erro == 0){
	 		$('[name=btn_upload]').val("Carregando...");
	 		$('[name=btn_cancelar]').val("Carregando...");
	 		$('[name=btn_upload]').attr("disabled","disabled");
	 		$('[name=btn_cancelar]').attr("disabled","disabled");
	 		$('[name=form_upload]').submit();
	 	}
	 });
	 
	 $('[name=btn_cancelar]').click(function() {
	 	$('[id=div_sub_foto]').hide();
	 });

	 <?php if($_SESSION['proinfantil']['mgs']): ?>
	 	alert('<?php echo $_SESSION['proinfantil']['mgs'] ?>');
	 	<?php unset($_SESSION['proinfantil']['mgs']) ?>
	 <?php endif; ?>
});

function addFoto(salid)
{
	$('[name=salid]').val(salid);
	$('[id=div_sub_foto]').show();
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' ); 
}

function removerFotoSala(arqid)
{
	if(confirm("Deseja realmente excluir esta foto?")){
		$.ajax({
		url: window.location + "&requisicao=removerFotoSalaMds&arqid=" + arqid,
		success: function(data) {
			$('[id=div_foto_' + arqid + ']').remove();
	    	}
		});
	}
}

function abrirGaleria(arqid,pagina,salid,pinid)
{
	window.open("../slideshow/slideshow/index.php?_sisarquivo=proinfantil&getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&pinid=" + pinid,"imagem","width=850,height=600,resizable=yes");
}

</script>