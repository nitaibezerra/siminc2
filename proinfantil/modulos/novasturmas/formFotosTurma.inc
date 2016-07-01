<?php
if ($_REQUEST['exibirfoto']) {
	
	/* $sql = "SELECT arqtipo, arqid, arqnome, arqextensao  FROM public.arquivo WHERE arqid = '" . $_REQUEST['arqid'] . "'";
	$dados = $db->pegaLinha($sql);
	
	$caminho = APPRAIZ . 'arquivos/' . (($_REQUEST["_sisarquivo"]) ? $_REQUEST["_sisarquivo"] : $_SESSION["sisarquivo"]) . '/' . floor($dados['arqid'] / 1000) . '/' . $dados['arqid'];
	
	$filename = str_replace(" ", "_", $dados['arqnome'].'.'.$dados['arqextensao']);
	header( 'Content-type: '. $dados['arqtipo'] );
	header( 'Content-Disposition: attachment; filename='.$filename);
	
	$html = '<img src="http://'.$_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/includes/image.php?image='.$caminho.'&amp;largura=400&amp;cropaltura=200" width="800" height="600" />';
	echo $html;
		
	die; */
	echo '<img src="/slideshow/slideshow/verimagem.php?&arqid=' . $_REQUEST['arqid'] . '" width="800" height="600"/>';
	die;
}

$_SESSION['proinfantil']['turid'] = trim($_REQUEST['turid']);

if(empty($_SESSION['proinfantil']['turid']) || !is_numeric($_SESSION['proinfantil']['turid']) || $_SESSION['proinfantil']['turid'] < 1){
	echo "<script>
			alert('A variável de sessão não existe, tente novamente.');
			document.location.href = 'proinfantil.php?modulo=novasturmas/listaTurmas&acao=A';
		  </script>";
	die;
}

//Chamada de programa
include_once APPRAIZ . "includes/workflow.php";
include  "_funcoes_novasturmas.php";
include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
echo "<br/>";

if( $_REQUEST['turid'] && $_REQUEST['novasturma'] ){
	$obNovasTurmas = new NovasTurmas( $_REQUEST );
	
	$docidAtual = $obNovasTurmas->criaDocumentoNovasTurmas();
	$esdid = $obNovasTurmas->pegaEstadoAtualNovasTurmas($docidAtual);
} else {
	$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
	$esdid = pegaEstadoAtualNovasTurmas($docid);
}

$arrSituacao = array(WF_NOVASTURMAS_EM_CADASTRAMENTO, WF_NOVASTURMAS_EM_DILIGENCIA);

$perfis = pegaPerfil($_SESSION['usucpf']);

// Chama metodo
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Fotos', $titulo2);

cabecalhoTurma();

$_SESSION['imgparams'] = array("filtro" => "1=1", "tabela" => "proinfantil.fotos");
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
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

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

<body onunload="opener.location.reload();">
<?php 
$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

$sql = "SELECT 		distinct sal.salid,
					tis.tisdescricao
		FROM		proinfantil.tiposala tis
		INNER JOIN	proinfantil.sala sal ON sal.tisid = tis.tisid
		WHERE		sal.modid = 2
		ORDER BY	tis.tisdescricao";

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
								$sql = "SELECT		arq.arqid,
													arq.arqnome||'.'||arq.arqextensao,
													arq.arqdescricao,
													arq.arqtamanho,
													fot.pinid
										FROM		proinfantil.fotos fot
										INNER JOIN	public.arquivo arq ON arq.arqid = fot.arqid
										WHERE		fot.salid = {$sala['salid']}
										AND			fot.turid = {$_SESSION['proinfantil']['turid']}
										AND			fot.fotstatus = 'A'";
								
								$arrFotos = $db->carregar($sql); ?>
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
											<?php if($acesso['cadastro'] == 'S'){ ?>
													<?if( (in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( EQUIPE_MUNICIPAL, $perfis )) && in_array($esdid, $arrSituacao) && $_SESSION['exercicio'] == date('Y')  ){ ?>
														<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoSala('<?php echo $foto['arqid'] ?>')" />
													<?php } else { ?>
														<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" />
													<?} ?>
											<?php } else { ?>
													<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" />
												<?} 
												$img = "../slideshow/slideshow/verimagem.php?arqid=".$foto['arqid']."&newwidth=100&newheight=85&_sisarquivo=".(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]);
												$link = "proinfantil.php?modulo=novasturmas/formFotosTurma&acao=A&turid=".$_REQUEST['turid']."&exibirfoto=1&arqid=".$foto['arqid'];
												?>
												<a id="fancybox_img_<?php echo $foto['arqid'] ?>" class="fancybox_img fancybox.ajax" href="<?php echo $link; ?>" data-fancybox-group="gallery" title="<?php echo $foto['arquivo'] ?>">
													<img <?php echo $tamanho ?> class="img_foto" src="<?php echo $img; ?>" />
                                        		</a>
										</div>
									<?php endforeach; ?>
								<?php endif; ?>
							</td>
							<?php if($acesso['cadastro'] == 'S' && $_SESSION['exercicio'] == date('Y')){ ?>
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
			<input type="hidden" name="turid" id="turid" value="'.$_SESSION['proinfantil']['turid'].'" />
			<input type="hidden" name="requisicao" value="salvarFotosSalaNovasTurmas" />
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
$_SESSION['imgparams'] = array("filtro" => "1=1", "tabela" => "proinfantil.fotos");					
?>

<script>
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
	 	$('[id=div_sub_foto]').hide();
	 });

	 <?php if($_SESSION['proinfantil']['mgs']): ?>
	 	alert('<?php echo $_SESSION['proinfantil']['mgs'] ?>');
	 	<?php unset($_SESSION['proinfantil']['mgs']) ?>
	 <?php endif; ?>
});

function addFoto(salid){
	$('[name=salid]').val(salid);
	$('[id=div_sub_foto]').show();
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' ); 
}

function removerFotoSala(arqid){
	if(confirm("Deseja realmente excluir esta foto?")){
		$.ajax({
		url: window.location + "&requisicao=removerFotoSalaNovasTurmas&arqid=" + arqid,
		success: function(data) {
			$('[id=div_foto_' + arqid + ']').remove();
	    	}
		});
	}
}

function abrirGaleria(arqid,pagina,salid,pinid, turid){	
	window.open("../slideshow/slideshow/index.php?getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&turid="+turid,"imagem","width=850,height=600,resizable=yes");
}
</script>
</body>   
