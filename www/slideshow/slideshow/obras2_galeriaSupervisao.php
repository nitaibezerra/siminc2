<?php
ini_set("memory_limit","256M");

$_SESSION['obras2']['semMarcadagua'] = true;

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

//	$filtro = Array('1=1');
//	$arrChavesFalsas = Array('pagina','arqid','getFiltro','_sisarquivo','tabela','campo','id','ZDEDebuggerPresent','PHPSESSID');
//	foreach($_REQUEST as $chave => $valor){
//		if( !in_array($chave,$arrChavesFalsas) ){
//			$filtro[] = $chave." = ".$valor;
//		}
//	}
//
//	if( $_REQUEST['tabela'] != '' ){
//		$status = $_REQUEST['tabela'] == 'arquivoquestaosupervisao' ? 'aqsstatus' : 'arsstatus';
//		$innerQuestãoObra = "INNER JOIN obras2.{$_REQUEST['tabela']} t ON t.arqid = arq.arqid AND t.{$_REQUEST['campo']} = {$_REQUEST['id']} AND t.$status = 'A' ";
//	}
//
//	$filtro = implode(' AND ', $filtro);

	$where = array();
	$join  = array();

	if ( $_SESSION['obras2']['sueid'] ){
		$where[] = "ars.sueid = " . $_SESSION['obras2']['sueid'];
	}else{
		$where[] = "ars.smiid = " . $_SESSION['obras2']['smiid'];
	}

	if ( $_GET['qtsid'] && is_numeric( $_GET['qtsid'] ) ){
		$join[]  = "JOIN obras2.arquivoquestaosupervisao aqs ON aqs.arqid = arq.arqid AND aqs.aqsstatus = 'A' AND aqs.qtsid = {$_GET['qtsid']}";
	}

	if ( $_GET['rsqid'] && is_numeric( $_GET['rsqid'] ) ){
		$join[]  = "JOIN obras2.arquivorespostasubquestao asq ON asq.arqid = arq.arqid AND
														   		 asq.arsstatus = 'A' AND
														   		 asq.rsqid = {$_GET['rsqid']}";
	}

	$sql = "SELECT DISTINCT
				arq.arqid,
				arq.arqdescricao,
				arqdata
			FROM
				obras2.arquivosupervisao ars
			LEFT JOIN public.arquivo AS arq ON arq.arqid = ars.arqid
			" . ($join  ? implode(" ", $join) : '') . "
			WHERE
				" . ($where  ? implode(" ", $where) : '') . "
				AND ars.aqsstatus = 'A'
				AND (arqtipo = 'image/jpeg' OR
				 	 arqtipo = 'image/gif' OR
				 	 arqtipo = 'image/png')
			ORDER BY
				arq.arqid
			LIMIT 16 OFFSET ".($pagina*16);

	return $db->carregar($sql);
}
function buscarTotalRegistros() {
	global $db;

//	$filtro = Array('1=1');
//	$filtro = Array('1=1');
//	$arrChavesFalsas = Array('pagina','arqid','getFiltro','_sisarquivo','tabela','campo','id','ZDEDebuggerPresent','PHPSESSID');
//	foreach($_REQUEST as $chave => $valor){
//		if( !in_array($chave,$arrChavesFalsas) ){
//			$filtro[] = $chave." = ".$valor;
//		}
//	}
//
//	if( $_REQUEST['tabela'] != '' ){
//		$status = $_REQUEST['tabela'] == 'arquivoquestaosupervisao' ? 'aqsstatus' : 'arsstatus';
//		$innerQuestãoObra = "INNER JOIN obras2.{$_REQUEST['tabela']} t ON t.arqid = arq.arqid AND t.{$_REQUEST['campo']} = {$_REQUEST['id']} AND t.$status = 'A' ";
//	}
//
//	$filtro = implode(' AND ', $filtro);

	$where = array();
	$join  = array();

	if ( $_SESSION['obras2']['sueid'] ){
		$where[] = "ars.sueid = " . $_SESSION['obras2']['sueid'];
	}else{
		$where[] = "ars.smiid = " . $_SESSION['obras2']['smiid'];
	}

	if ( $_GET['qtsid'] && is_numeric( $_GET['qtsid'] ) ){
		$join[]  = "JOIN obras2.arquivoquestaosupervisao aqs ON aqs.arqid = arq.arqid AND
														   aqs.aqsstatus = 'A' AND
														   aqs.qtsid = {$_GET['qtsid']}";
	}

	if ( $_GET['rsqid'] && is_numeric( $_GET['rsqid'] ) ){
		$join[]  = "JOIN obras2.arquivorespostasubquestao asq ON asq.arqid = arq.arqid AND
														   		 asq.arsstatus = 'A' AND
														   		 asq.rsqid = {$_GET['rsqid']}";
	}

	$sql = "SELECT DISTINCT
				count(arq.arqid) as qtd
			FROM
				obras2.arquivosupervisao ars
			LEFT JOIN public.arquivo AS arq ON arq.arqid = ars.arqid
			" . ($join  ? implode(" ", $join) : '') . "
			WHERE
				" . ($where  ? implode(" ", $where) : '') . "
				AND ars.aqsstatus = 'A'
				AND (arqtipo = 'image/jpeg' OR
				 	 arqtipo = 'image/gif' OR
				 	 arqtipo = 'image/png')";

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
<script type="text/javascript" src="../../includes/JQuery/jquery-1.7.2.min.js"></script>
<script>jQuery.noConflict();</script>
</head>
<body>
	<input type=hidden id=fotoinicio value="-1">
	<div id="container">
		<div id="example">
		<div id="slideshowContainer" class="slideshowContainer"></div>
			<div id="thumbnails">
<?php
if($_SESSION['obras2']['sueid'] || $_SESSION['obras2']['smiid']) {
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
		$vjscript[] = "show.descricao[". $i ."] = '". preg_replace("/\r\n|\n|\r/", "<br>", addslashes($foto['arqdescricao'])) ."<br><b>Data de inclusão:</b> ".formata_data($foto['arqdata'])."';";
?>
		<a href="verimagem.php?newwidth=&newheight=&_sisarquivo=obras2&arqid=<? echo $foto['arqid']; ?>" class="slideshowThumbnail">
			<img src="verimagem.php?newwidth=50&newheight=50&_sisarquivo=obras2&arqid=<? echo $foto['arqid']; ?>" id="<? echo $foto['arqid']; ?>" onclick="show.paused = true; document.getElementById('bplay').style.display = 'inline'; document.getElementById('pause').style.display = 'none';"  border="0" />
		</a>
<?php
		if( $foto['arqid'] == $_REQUEST['arqid'] || (empty($_REQUEST['arqid']) && $foto['arqid'] == $fotos[0]['arqid']) ){
			echo "<script>document.getElementById(\"fotoinicio\").value = ". $i .";</script>";
		}
		$i++;
	}
	$total = buscarTotalRegistros();
//	$param = "";
//	$filtro = Array('1=1');
//	$arrChavesFalsas = Array('pagina','arqid','getFiltro','_sisarquivo','tabela','campo','id','ZDEDebuggerPresent','PHPSESSID');
//	foreach($_REQUEST as $chave => $valor){
//		if( !in_array($chave,$arrChavesFalsas) ){
//			$filtro[] = $chave." = ".$valor;
//			$param.="&$chave={$_GET[$chave]}";
//		}
//	}
	$param = "";
	if ( $_GET['qtsid'] && is_numeric( $_GET['qtsid'] ) ){
		$param .= "&qtsid=" . $_GET['qtsid'];
	}

	if ( $_GET['rsqid'] && is_numeric( $_GET['rsqid'] ) ){
		$param .= "&rsqid=" . $_GET['rsqid'];
	}

	for($i = 0; $i < ceil(current($total)/16); $i++ ) {
		$page[] = "<a href=obras2_galeriaSupervisao.php?pagina=". $i . $param.">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
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
                jQuery('#slideshowContainer').css('overflow','overlay');
                jQuery('#slideshowContainer div:eq(2)').css('overflow','overlay');

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

<style>
    .slideshowContainer img {
        width: 640px;
    }
</style>