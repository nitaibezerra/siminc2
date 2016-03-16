<?php
ini_set("memory_limit","256M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

function buscarFotos() {
	global $db;
	
	$pagina = ($_GET['pagina'] ? $_GET['pagina'] : '');
	
	if(!$pagina) $pagina = 0;
	
	$where = array();	
	$join  = array();	
	
	$obrid = ($_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid']);
//	$where[] = "fot.obrid = " . $_SESSION['obras2']['obrid'];
	
	$order = 'arqid';
	
	if ( $_GET['supid'] && is_numeric( $_GET['supid'] ) ){
		$join[] = "JOIN obras2.fotos AS fot ON fot.arqid = arq.arqid AND fot.supid = {$_GET['supid']} AND fot.obrid = {$obrid}
		           LEFT JOIN obras2.itenscomposicaoobra i ON i.icoid = fot.icoid
                   LEFT JOIN obras2.itenscomposicao c ON c.itcid = i.itcid
		";
		$order  = 'fotordem';
        $column = ', c.itcdesc';
	}
        
	if ( $_GET['emiid'] && is_numeric( $_GET['emiid'] ) ){
		$join[] = "JOIN obras2.fotos AS fot ON fot.arqid = arq.arqid AND fot.emiid = {$_GET['emiid']} AND fot.obrid = {$obrid}";
		$order  = 'fotordem';
	}
	
	if ( $_GET['tipo'] == 'abaGaleria' ){
		$join[]  = "JOIN obras2.obras_arquivos oar ON oar.arqid = arq.arqid AND oar.oarstatus = 'A' 
                                                                                    AND (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
                                                                                    AND oar.obrid = {$obrid}";
		$order = 'arq.arqid';
	}
		
	$sql = "SELECT 
                    arq.arqid,
                    arq.arqdescricao,
                    arq.arqdata
                    {$column}
                FROM 
                    public.arquivo AS arq
                " . ($join  ? implode(" ", $join) : '') . "	 
                ORDER BY 
                                    {$order}
                LIMIT 16 OFFSET ".($pagina*16);	

	return $db->carregar($sql);
}
function buscarTotalRegistros() {
	global $db;
	
	$where = array();	
	$join  = array();	
	
	$obrid = ($_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid']);
//	$where[] = "fot.obrid = " . $_SESSION['obras2']['obrid'];
	
	if ( $_GET['supid'] && is_numeric( $_GET['supid'] ) ){
		$join[]  = "JOIN obras2.fotos AS fot ON fot.arqid = arq.arqid AND fot.supid = {$_GET['supid']} AND fot.obrid = {$obrid}";
	}
        
	if ( $_GET['emiid'] && is_numeric( $_GET['emiid'] ) ){
		$join[]  = "JOIN obras2.fotos AS fot ON fot.arqid = arq.arqid AND fot.emiid = {$_GET['emiid']} AND fot.obrid = {$obrid}";
	}
	
	if ( $_GET['tipo'] == 'abaGaleria' ){
		$join[]  = "JOIN obras2.obras_arquivos oar ON oar.arqid = arq.arqid AND oar.oarstatus = 'A' 
                                                                                    AND (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
                                                                                    AND oar.obrid = {$obrid}";
	}
	
	$sql = "SELECT count(arq.arqid) as qtd 
                FROM 
                    public.arquivo AS arq
                " . ($join  ? implode(" ", $join) : '');		
	
	return current($db->carregar($sql));
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Galeria de fotos</title>
<link rel="stylesheet" href="../_common/css/main.css" type="text/css" media="all">
<link href="slideshow.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="../_common/js/mootools.js"></script>
<script type="text/javascript" src="../utils/backgroundSlider.js"></script>
<script type="text/javascript" src="slideshow.js"></script>
</head>
<body>
	<input type=hidden id=fotoinicio value="-1">
	<div id="container">
		<div id="example">
		<div id="slideshowContainer" class="slideshowContainer"></div>
			<div id="thumbnails">
<?php
if($_SESSION['obras2']['obrid'] || $_GET['obrid']) {
	$fotos = buscarFotos();
} else {
	echo "<script>
                    alert('Erro de parâmetro da sessão. Acesse a área \"FALE CONOSCO\" no rodapé da página e entre em contato com o suporte.');
                    window.close();
              </script>";
}

if($fotos) {
	$i = -1;
	foreach($fotos as $foto) {
//		$_REQUEST["_sisarquivo"] = ($foto['obrid_1'] ? 'obras' : $_REQUEST["_sisarquivo"]);
		$vjscript[] = "show.descricao[". $i ."] = '". preg_replace("/\r\n|\n|\r/", "<br>", ($foto['itcdesc'] ? $foto['itcdesc'] . ' | ' : '') . addslashes($foto['arqdescricao'])) ."<br><b>Data de inclusão:</b> ".formata_data($foto['arqdata'])."';";
?>
		<a href="verimagem.php?newwidth=640&newheight=480&_sisarquivo=obras2&arqid=<?php echo $foto['arqid']; ?>" class="slideshowThumbnail">
			<img src="verimagem.php?newwidth=50&newheight=50&_sisarquivo=obras2&arqid=<? echo $foto['arqid']; ?>" id="<? echo $foto['arqid']; ?>" onclick="show.paused = true; document.getElementById('bplay').style.display = 'inline'; document.getElementById('pause').style.display = 'none';"  border="0" />
		</a>
<?php
		if( $foto['arqid'] == $_REQUEST['arqid'] || (empty($_REQUEST['arqid']) && $foto['arqid'] == $fotos[0]['arqid']) ){ 
			echo "<script>document.getElementById(\"fotoinicio\").value = ". $i .";</script>";
		}	
		$i++;
	}
	$total = buscarTotalRegistros();
	$param = "";
	if ( $_GET['tipo'] ){
		$param .= "&tipo=" . $_GET['tipo'];
	}
	
	if ( $_GET['supid'] && is_numeric( $_GET['supid'] ) ){
		$param .= "&supid=" . $_GET['supid'];
	}
        
	if ( $_GET['emiid'] && is_numeric( $_GET['emiid'] ) ){
		$param .= "&emiid=" . $_GET['emiid'];
	}
	
	if ( $_GET['obrid'] && is_numeric( $_GET['obrid'] ) ){
		$param .= "&obrid=" . $_GET['obrid'];
	}
	
	for($i = 0; $i < ceil(current($total)/16); $i++ ) {
		$page[] = "<a href=obras2_galeriaGaleriaFotos.php?pagina=". $i . $param.">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
	}
}
?>
			<p style="text-align: center; ">
			<?php
			if(count($page) > 1) { 
				echo implode(" | ", $page);
			} 
			?></p>
			<br />
			<form method="post" action="../../geral/downloadfiles.php" target="popup" onsubmit="window.open('', 'popup', 'width=5,height=5');" id="download">
			<p style="text-align: center;"><input type="image" src="../_common/img/bdownload.jpg" title="Baixar todas"><br /><b>Baixar todas</b></a></p>
			<?php 
			if($fotos[0]) { 
				foreach($fotos as $fot) {
					echo "<input type='hidden' name='fotosselecionadas[]' value='".$fot['arqid']."'>";				
				}
			} 
			?>
			</form>
		  </div>
			<script type="text/javascript">
		  	window.addEvent('domready',function(){
				var obj = {
					wait: 3000, 
					effect: 'fade',
					duration: 1000, 
					loop: true, 
					thumbnails: true,
					backgroundSlider: true,
					onClick: false
				}
				show = new SlideShow('slideshowContainer','slideshowThumbnail',obj);
				<?php echo ((count($vjscript) > 0)?implode("", $vjscript):''); ?>
				show.play();
			});
		  </script>
		</div>
	</div>
	<div id="rodape">
		<img src="../_common/img/bplay.jpg" id="bplay" title="Play" border="0"
			 onclick="show.paused = false; this.style.display = 'none'; document.getElementById('pause').style.display = 'inline'; show.play(); return false;" > 
		<img src="../_common/img/bpause.jpg" id="pause" style="display:none;" border="0" 
			 onclick="this.style.display = 'none'; document.getElementById('bplay').style.display = 'inline'; show.stop(); show.paused = false; return false;" > 
		<img src="../_common/img/bprevious.jpg" title="Voltar foto" border="0"
			 onclick="show.previous(); document.getElementById('bplay').style.display = 'inline'; document.getElementById('pause').style.display = 'none'; show.paused = true; return false;" > 
		<img src="../_common/img/bnext.jpg" border="0"
			 onclick="show.play(); document.getElementById('bplay').style.display = 'inline'; document.getElementById('pause').style.display = 'none'; show.paused = true; return false;" > 
		<img src="../_common/img/bdownload.jpg" border="0" title="Download da foto" 
			 onclick="window.open('../../geral/downloadfiles.php?enderecoabsolutoarquivo='+show.images[show.image], 'popup', 'width=5,height=5');">
	</div>
	<div class="descricao" id="descricaoimagem"></div>
</body>
</html>