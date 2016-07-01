<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/par/_funcoesPar.php";
include_once APPRAIZ . "www/par/_funcoes.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '';
$_SESSION['usucpf'] 		= '';

$db = new cls_banco();
//63230_49

if($_REQUEST['download'] == 'S'){
	/*  $file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
	$arquivo = $file->getDownloadArquivo($arqid);  */
	
	$dtanomearquivo = $db->pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.dtaid = ".$_REQUEST['dtaid']);
	
	$diretorio = APPRAIZ . 'arquivos/par/documentoTermo/'.$dtanomearquivo;
	if( is_file($diretorio) ){
		header( 'Content-type: text/plain' );
		header( 'Content-Disposition: attachment; filename='.$dtanomearquivo);
		readfile( $diretorio );
	} else {
		echo "<script>
				alert('Arquivo não encontrado');
				window.location.href = 'carrega_documento_texto.php';
		</script>";
	}
	exit;
}

if( $_REQUEST['requisicao'] == 'salvar' ){
	ob_clean();
	
	if( $_REQUEST['tipo'] == 'PAC' ){
		$dtanomearquivo = $db->pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.terid = ".$_REQUEST['dopid']." and d.dtastatus = 'A'");
	} else {	
		$dtanomearquivo = $db->pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.dopid = ".$_REQUEST['dopid']." and d.dtastatus = 'A'");
	}
	
	$doptexto = $_REQUEST['texto'];
	
	if( $doptexto ){
		if( strpos($doptexto, '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
			$doptexto = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
		} else {
			$doptexto = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
		}
	}
	$doptexto = !empty($doptexto) ? simec_htmlspecialchars($doptexto, ENT_QUOTES, 'ISO-8859-1') : 'null';
	
	if( !empty($dtanomearquivo) ){
		
		$nomeArquivo 		= $dtanomearquivo;
		$diretorio		 	= APPRAIZ . 'arquivos/par/documentoTermo';
		$diretorioArquivo 	= APPRAIZ . 'arquivos/par/documentoTermo/'.$nomeArquivo;
		
		if( !is_dir($diretorio) ){
			mkdir($diretorio, 0777);
		}
		
		$fp = fopen($diretorioArquivo, "w");
		if ($fp) {
			stream_set_write_buffer($fp, 0);
			fwrite($fp, $doptexto);
			fclose($fp);
		}
	} else {
		gravaHtmlDocumento($doptexto, $_REQUEST['dopid'], $_REQUEST['processo'], $_REQUEST['tipo']);
	}
	echo "<script>
				alert('Operação Realizada com Sucesso');
				window.location.href = window.location;
		</script>";
	exit();
}

$boMostraLista = 'N';
if( $_REQUEST['requisicao'] == 'carregarDocumento' ){
	if( $_REQUEST['tipo'] == 'PAC' ){
		$dopid = '';
		$terid = $_REQUEST['dopid'];
	} else {
		$dopid = $_REQUEST['dopid'];
		$terid = '';
	}
	
	$doptexto = pegaTermoCompromissoArquivo($dopid, $terid);
	
	$doptexto = str_replace('"', "'", $doptexto);
	$boMostraLista = 'S';
}

?>
<hmtl>
<script type="text/javascript" src="/../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
<script type="text/javascript" src="../../library/jquery/jquery-1.10.2.js"></script>
<script language="javascript" type="text/javascript" src="../../includes/tinymce/tiny_mce.js"></script>
<body>
	<input type="hidden" name="doptexto"		id="doptexto" 		value="<?=$doptexto ?>" />
	
	<form id="formulario" name="formulario" method="post" action="">
		<input type="hidden" name="requisicao" 	id="requisicao" value="" />
		<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" valign="middle" width="25%"><b>Id do Documento:</b></td>
			<td>
			<?php
				echo campo_texto('dopid', 'N', 'S', '', 5, 10, '[#]', '', '', '', 0, 'id="dopid"','', $_REQUEST['dopid'], '' );
			?>
			<input type="button" id="bt_carrega" value="Carregar" onclick="carregarDocumento();" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="middle" width="25%"><b>Id do Processo:</b></td>
			<td>
			<?php
				echo campo_texto('processo', 'N', 'S', '', 5, 10, '[#]', '', '', '', 0, 'id="processo"','', $_REQUEST['processo'], '' );
			?>
			</td>
		</tr>
		<tr>
	        <td class="SubTituloDireita" style="width: 19%;">
	            <b>Tipo:</b>
	        </td> 
	        <td>
		    <?php 
		    	$tipo = $_REQUEST['tipo'];
		    	$db->monta_combo("tipo", array(
		    			array('codigo' => 'PAR', 'descricao' => 'PAR'),
		    			array('codigo' => 'OBRA', 'descricao' => 'OBRA'),
		    			array('codigo' => 'PAC', 'descricao' => 'PAC')		    			
		    	), 'S', 'Selecione...', '', '', '', '', '', 'tipo', '', '', '' );
	        ?>
	        </td>    
	    </tr>
	    <tr>
	    	<td colspan="5">
	    	<?php
	    	if( $boMostraLista == 'S' ){
	    	
	    		if( $_REQUEST['tipo'] == 'PAC' ){
	    			$sql = "SELECT dtaid, dopid, terid, arqid, dtatipo, dtaprocesso, dtanomearquivo, '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'carrega_documento_texto.php?download=S&dtaid='||dtaid||'\'\">'||dtanomearquivo||'</a>' as nomearquivo, dtastatus
	    			FROM par.documentotermoarquivo WHERE terid = {$_REQUEST['dopid']}";
	    		} else {
	    			$sql = "SELECT dtaid, dopid, terid, arqid, dtatipo, dtaprocesso, dtanomearquivo, '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'carrega_documento_texto.php?download=S&dtaid='||dtaid||'\'\">'||dtanomearquivo||'</a>' as nomearquivo, dtastatus
	    			FROM par.documentotermoarquivo WHERE dopid = {$_REQUEST['dopid']}";
	    		}
	    		$arrDados = $db->carregar($sql);
	    		$arrDados = $arrDados ? $arrDados : array();
	    		
	    		$arrRegistro = array();
	    		foreach ($arrDados as $v) {
	    			
	    			$arquivo = APPRAIZ."arquivos/par/documentoTermo/".$v['dtanomearquivo'];
	    			//$diretorio = dir($path);
	    			
	    			$result = '0 Kb';
	    			if(is_file($arquivo)){
	    					$bytes = filesize($arquivo);
	    					$result = $bytes / 1024;
	    					$result = str_replace(".", "," , strval(round($result, 2)))." Kb";
	    			}
	    			//ver($result, $arquivo, $v, d);
	    			
	    			array_push($arrRegistro, array(
	    										'dtaid' => $v['dtaid'],
	    										'dopid' => $v['dopid'],
	    										'terid' => $v['terid'],
	    										'arqid' => $v['arqid'],
	    										'dtatipo' => $v['dtatipo'],
	    										'dtaprocesso' => $v['dtaprocesso'],
	    										'nomearquivo' => $v['nomearquivo'],
	    										'tamanho' => $result,
	    										'dtastatus' => $v['dtastatus']
	    									));	
	    		}
	    		$cabecalho = array("dtaid", "dopid", "terid", "arqid", "dtatipo", "dtaprocesso", "dtanomearquivo", "Tamanho", "dtastatus");
	    		$db->monta_lista_simples($arrRegistro,$cabecalho,500,5,'N','100%','');
	    	}
	    	?>
	    	</td>
	    </tr>
		<tr>
			<td colspan="2">
				<div>
					<textarea id="texto" name="texto" rows="28" cols="80" style="width:100%" class="minutatinymce"></textarea>
				</div>
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#c0c0c0" colspan="2">		
				<input type="button" id="bt_salvar" value="Salvar" onclick="salvarMinutaTermoAditivo();" />
			</td>
		</tr>
		</table>
	</form>
</body>
<script type="text/javascript">
$(document).ready(function(){

	$('#texto').val($('#doptexto').val());
});

tinyMCE.init({
	// General options
	mode : "textareas",
	theme : "advanced",
	language: "pt",
	editor_selector : "minutatinymce",
	plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

	// Theme options
	theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
	theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
	theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
	theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	theme_advanced_statusbar_location : "",
	theme_advanced_resizing : true,

	// Example content CSS (should be your site CSS)
	content_css : "css/content.css",

	// Drop lists for link/image/media/template dialogs
	template_external_list_url : "lists/template_list.js",
	external_link_list_url : "lists/link_list.js",
	external_image_list_url : "lists/image_list.js",
	media_external_list_url : "lists/media_list.js",

	// Replace values for the template plugin
	template_replace_values : {
		username : "Some User",
		staffid : "991234"
	}
});

function carregarDocumento(){
	if( $('#dopid').val() != '' ){
		$('#requisicao').val("carregarDocumento");
		$('#formulario').submit();
	} else {
		alert('Informe o codido do Documento');
	}
}

function salvarMinutaTermoAditivo() {
	
	var texto = tinyMCE.get('texto').getContent();
	
	if( texto != '' ){
		$('#requisicao').val("salvar");
		$('#formulario').submit();
	} else {
		alert('É necessário carregar o documento antes de salvar.');
		return false;
	}
}
</script>
</html>
<?php 


/* header("Content-Type: ".$tipo); // informa o tipo do arquivo ao navegador 
header("Content-Length: ".filesize($arquivo)); // informa o tamanho do arquivo ao navegador 
header("Content-Disposition: attachment; filename=".basename($arquivo)); // informa ao navegador que é tipo anexo e faz abrir a janela de download, tambem informa o nome do arquivo 
readfile($arquivo); // lê o arquivo 
exit; // aborta pós-ações */

/* $path = APPRAIZ."arquivos/par/documentoTermo/"; 
$diretorio = dir($path); 
$tot = 1;
echo "Lista de Arquivos do diretório '<strong>".$path."</strong>':<br />";

while($arquivo = $diretorio -> read()){ 
	if( $arquivo != '.' && $arquivo != '..' ){
		
		$bytes = filesize($path.$arquivo);
		$result = $bytes / 1024; 
		$result = str_replace(".", "," , strval(round($result, 2)))." Kb";		
		
		echo "<a href='".$path.$arquivo."'>".$tot.' - '.$arquivo.' - '.$result."</a><br />";
		$tot++;
	}
} 
$diretorio -> close(); */
