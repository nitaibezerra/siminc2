<?php
if ($_REQUEST['exibirfoto']) {
	echo '<img src="/slideshow/slideshow/verimagem.php?&arqid=' . $_REQUEST['arqid'] . '" width="800" height="600"/>';
	die;
}

include_once "_funcoes_proinfancia.php";
include APPRAIZ ."includes/workflow.php";

if( empty($_SESSION['proinfantil']['popup']) ){
	include APPRAIZ . "includes/cabecalho.inc";
	echo '<br/>';
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}

ini_set("memory_limit","2048M");

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}
if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	die;
}
$obrid = $_GET['obrid'] ? $_GET['obrid'] : $_SESSION['proinfantil']['obrid'];
unset($_GET['obrid']);
$_SESSION['proinfantil']['obrid'] = $obrid;
$pinid = $_GET['pinid'] ? $_GET['pinid'] : $_SESSION['proinfantil']['pinid'];
$_SESSION['proinfantil']['pinid'] = $_SESSION['proinfantil']['pinid'] ? $_SESSION['proinfantil']['pinid'] : $pinid;
unset($_GET['pinid']);

if(empty($obrid)){
	echo "<script>
			alert('A variável de sessão não existe, tente novamente.');
			document.location.href = 'proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A';
		  </script>";
	exit();
}

$perfis = pegaPerfil($_SESSION["usucpf"]);

montaAbasProinfantil( $abacod_tela, $url );

cabecalhoProInfantil($obrid);
$habilitado = verificaLiberacao();

$_SESSION['downloadfiles']['pasta'] = array("origem" => "proinfantil","destino" => "proinfantil");
$_SESSION['imgparams']  			= array("filtro" => "fotstatus = 'A'", "tabela" => "proinfantil.fotos");
$_SESSION['sisarquivo'] 			= 'proinfantil';

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

<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script type="text/javascript">
	$jq	 = $.noConflict(); 
</script>


<!-- Add fancyBox main JS and CSS files -->
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />

<!-- Add Button helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

<!-- Add Thumbnail helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>

<!-- Add Media helper (this is optional) -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>

<script type="text/javascript">
$(document).ready(function() {
	$jq('.fancybox_img').fancybox({
		openEffect	: 'elastic',
    	closeEffect	: 'elastic',
    	helpers : {
    		title : {
    			type : 'inside'
    		},
			thumbs	: {
				width	: 100,
				height	: 50
			}
    	}
	});
	
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
	 	$('[id=div_sub_foto]').addClass('hidden');
	 });
	 <?php if($_SESSION['proinfantil']['mgs']): ?>
	 	alert('<?php echo $_SESSION['proinfantil']['mgs'] ?>');
	 	<?php unset($_SESSION['proinfantil']['mgs']) ?>
	 <?php endif; ?>
});

function addFoto(salid) {
	$('[name=salid]').val(salid);
	$('[id=div_sub_foto]').removeClass('hidden');
	$('[id=div_sub_foto]').show();
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' ); 
}

function removerFotoSala(arqid) {
	if(confirm("Deseja realmente excluir esta foto?")){
		$.ajax({
		url: window.location + "&requisicaoAjax=removerFotoSala&arqid=" + arqid,
		success: function(data) {
			$('[id=div_foto_' + arqid + ']').remove();
			window.location.reload();
	    	}
		});
	}
}
function abrirGaleria(arqid,pagina,salid,pinid){
	window.open("../slideshow/slideshow/index.php?_sisarquivo=proinfantil&getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&pinid=" + pinid,"imagem","width=850,height=600,resizable=yes");
}
	
function checaAmbiente(id){
	if(document.getElementById("chk_ambiente_"+id).checked == true){
		document.getElementById("field_sala_"+id).style.display = '';
	} else {
		document.getElementById("field_sala_"+id).style.display = 'none';
	}
}

</script>

<?php $arrSalas = recuperaSalas($obrid,$pinid); ?>

<table class="tabela" border="0" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
    	<td width="25%" class="SubTituloDireita">
    		Ambientes:
		</td>
		<td>
			<table border="0" bgcolor="#f5f5f5" width="100%">
				<tr>
		   		    <?php if($arrSalas): ?>
						<?php $i=0; ?>
						<?php  foreach($arrSalas as $s): 
							$checado = "";
							$chkbloqueado = false;
												
							$sql = "SELECT 			COUNT(arq.arqid) as total
									FROM 			proinfantil.fotos fot
									INNER JOIN		public.arquivo arq ON arq.arqid = fot.arqid
									WHERE			fot.salid = {$s['salid']}
									AND				fot.pinid = {$pinid}
									AND				fot.fotstatus = 'A'";
							$total = $db->pegaUm($sql);
		
							if($total > 0) $checado = "checked";
		
							/*
							tpoid=9 - "Escola de Educação Infantil Tipo B"
							15;"Geral da Unidade"
							16;"Cantina e Refeitório"
							17;"Pátio"
							*/
							if($s['salid'] == 15 || $s['salid'] == 16 || $s['salid'] == 17){
								$checado = "checked";
								$chkbloqueado = true;
							}
							
							/*
							tpoid=10 - "Escola de Educação Infantil Tipo C"
							5;"Geral da Unidade"
							6;"Cantina e Refeitório"
							7;"Pátio"
							*/
							if($s['salid'] == 5 || $s['salid'] == 6 || $s['salid'] == 7){
								$checado = "checked";
								$chkbloqueado = true;
							}
		
							/*
							tpoid=10 - "Escola de Educação Infantil Tipo C"
							5;"Geral da Unidade"
							6;"Cantina e Refeitório"
							7;"Pátio"
							*/
							if($s['salid'] == 42 || $s['salid'] == 43 || $s['salid'] == 44){
								$checado = "checked";
								$chkbloqueado = true;
							} ?>
							<td width="40%"><input type="checkbox" name="chk_ambiente" id="chk_ambiente_<?=$s['salid']?>" value="<?=$s['salid']?>" onclick="checaAmbiente(<?=$s['salid']?>)" <?=$checado?> <?if($chkbloqueado) echo "disabled";?>> <?=$s['tisdescricao'];?> <br> </td>
							<?php 
							if($i%2 == 1){ 
								echo "</tr><tr>";
							} ?>
							<?php $i++ ?>
						<?php endforeach;  ?>
					<?php endif; ?>
				</tr>
			</table>
		</td>
	</tr>
</table>

<br><br>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th>Observação: Anexar fotos que evidenciem a presença de crianças nos diferentes ambientes da instituição.</th>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
    	<td>
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
										<?php $arrFotos = recuperaFotosSala($sala['salid'],$pinid) ?>
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
													<?php if($habilitado == 'S' && empty($_SESSION['proinfantil']['popup'])){ ?>
														<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoSala('<?php echo $foto['arqid'] ?>')" />
													<?php } else { ?>
														<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar"  />
													<?} 
													$img = "../slideshow/slideshow/verimagem.php?arqid=".$foto['arqid']."&newwidth=100&newheight=85&_sisarquivo=".(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]);
													$link = "proinfantil.php?modulo=principal/fotosObraMunicipio&acao=A&pinid=".$pinid."&exibirfoto=1&arqid=".$foto['arqid'];
													?>
													<a id="fancybox_img_<?php echo $sala['salid'] ?>" class="fancybox_img fancybox.ajax" href="<?php echo $link; ?>" data-fancybox-group="gallery" title="<?php echo $foto['arquivo'] ?>">
														<img <?php echo $tamanho ?> class="img_foto" src="<?php echo $img; ?>" />
                                        			</a>
												</div>
											<?php endforeach; ?>
										<?php endif; ?>
									</td>
									<?php if($habilitado == 'S' && empty($_SESSION['proinfantil']['popup']) ){ ?>
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
		</td>
		<?if( empty($_SESSION['proinfantil']['popup']) ){ ?>
		<td width="80" valign="top" align="right">
			<?php wf_desenhaBarraNavegacao( recuperaDocumentoProInfantil($pinid) , array( 'pinid' => $pinid ) );  ?>
		</td>
		<?} ?>
	</tr>
</table>
<?php $html = '<form id="form_upload" name="form_upload" method=post enctype="multipart/form-data" >
					<input type="hidden" name="salid" id="salid" value="" />
					<input type="hidden" name="requisicao" value="salvarFotosSala" />
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
			   </form>' ?>
<?php popupAlertaGeral($html,"400px","120px","div_sub_foto","hidden"); ?>

<script>

function checaAmbienteTodos(){
	var obj = document.getElementsByName("chk_ambiente");
	var tot = obj.length;
	
	for(i=0; i<tot; i++){
		if(obj[i].checked == true){
			document.getElementById("field_sala_"+obj[i].value).style.display = '';
		}
		else{
			document.getElementById("field_sala_"+obj[i].value).style.display = 'none';
		}
	}
}

checaAmbienteTodos();

<?php if($habilitado =='N'){ ?>
$('[name=chk_ambiente]').attr('disabled','disabled');
<?php } ?>
</script>