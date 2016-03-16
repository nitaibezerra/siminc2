<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
$db = new cls_banco();

if (!$_SESSION['usucpf']){
	die('<script>
			alert(\'Acesso negado!\');
			window.close();
		 </script>');
}

function EnviarArquivoRegras($arquivo, $arqdescricao){
	global $db;
	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		$parametros = http_build_query( (array) $parametros, '', '&' );
		header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
		exit();
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
	values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".substr($arqdescricao,0,255)."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	if( !is_dir( APPRAIZ.'arquivos' ) ) mkdir(APPRAIZ.'arquivos/', 0777);
	if( !is_dir( APPRAIZ.'arquivos/'.$_SESSION['sisdiretorio'] ) ) mkdir(APPRAIZ.'arquivos/'.$_SESSION['sisdiretorio'], 0777);
	if( !is_dir( APPRAIZ.'arquivos/'.$_SESSION['sisdiretorio'].'/'.floor($arqid/1000) ) ) mkdir(APPRAIZ.'arquivos/'.$_SESSION['sisdiretorio'].'/'.floor($arqid/1000), 0777);
	
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	
	switch($arquivo["type"]) {
		case 'image/jpeg':
			
			try {
			
				ini_set("memory_limit", "256M");
				list($width, $height) = getimagesize($arquivo['tmp_name']);
				$original_x = $width;
				$original_y = $height;
				// se a largura for maior que altura
				if($original_x > $original_y) {
					$porcentagem = (100 * 640) / $original_x;
				}else {
					$porcentagem = (100 * 480) / $original_y;
				}
				$tamanho_x = $original_x * ($porcentagem / 100);
				$tamanho_y = $original_y * ($porcentagem / 100);
				$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
				$image   = imagecreatefromjpeg($arquivo['tmp_name']);
				imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
				imagejpeg($image_p, $caminho, 100);
				//Clean-up memory
				ImageDestroy($image_p);
				//Clean-up memory
				ImageDestroy($image);
			
			} catch (Exception $e) {
				
				if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
					$db->rollback();
					echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
					exit;
				}
			}
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
				exit;
			}
	}
	return $arqid;
}


if( $_POST['requisicao'] == 'salvar' ){
	if( $_FILES["arquivo"]['name'] ){
		$idArquivo = EnviarArquivoRegras($_FILES, $_POST['arqdescricao']);
	}
	extract( $_POST );
	$rgaregra = simec_htmlspecialchars($rgaregra, ENT_QUOTES);
	$rgacod = $rgacod ? $rgacod : $db->pegaUm( "SELECT COALESCE(max(rgacod), 0) FROM seguranca.regra WHERE mnuid = ".$_GET['mnuid'] ) + 1;
		
	$rgaid = $rgaid ? $rgaid : 'null';
	$idArquivo = $idArquivo ? $idArquivo : 'null';
	$rgadata = formata_data_sql($rgadata);
	
	if( strlen($rgaregra) == $sizeRegra ){
		$sql = "UPDATE seguranca.regra SET mnuid = '{$_GET['mnuid']}', rgadesc = '{$rgadesc}', rgacampo = '{$rgacampo}', rgaregra = '{$rgaregra}',
  					rgasolicitante = '{$rgasolicitante}', rgadesenvolvedor = '{$rgadesenvolvedor}', rgadata = '{$rgadata}', 
  					rgademandante = '{$rgademandante}', arqid = $idArquivo
				WHERE rgaid = $rgaid";
	} else {
		$sql = "INSERT INTO seguranca.regra(rgadesc, mnuid, rgacampo, rgaregra, rgasolicitante, rgadesenvolvedor, rgademandante, rgaidpai, rgadata, rgacod, arqid) 
				VALUES ('{$rgadesc}', '{$_GET['mnuid']}', '{$rgacampo}', '{$rgaregra}', '{$rgasolicitante}', '{$rgadesenvolvedor}', '{$rgademandante}', $rgaid, '{$rgadata}', $rgacod, $idArquivo)";
	}
	$db->executar( $sql );	
	if( $db->commit() ){
		echo "<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.reload();
				window.close();
			</script>";
		exit();
	} else {
		echo "<script>
				alert('Falha na Operação!');
			</script>";
	}
}

if( !empty($_GET['rgaid']) ){
	$sql = "SELECT * FROM seguranca.regra  WHERE rgaid = ".$_GET['rgaid'];
	
	$arRegistro = $db->pegaLinha( $sql );
	$arRegistro = $arRegistro ? $arRegistro : array();
	extract($arRegistro);
	$rgadata = formata_data($rgadata);
	$rgaregraTam = strlen($rgaregra);
}
//ver( $rgaregra, strlen($rgaregra),d );
$sql = "SELECT sisdsc FROM seguranca.sistema WHERE sisid = ".$_SESSION['sisid'];
$sisdsc = $db->pegaUm($sql);
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>

<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="rgaid" id="rgaid" value="<?=$_GET['rgaid']; ?>">
	<input type="hidden" name="rgacod" id="rgacod" value="<?=$rgacod; ?>">
	<input type="hidden" name="sizeRegra" id="sizeRegra" value="<?=$rgaregraTam; ?>">
	
	<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="notprint">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Cadastro de Regras</label></td>
    	</tr>
    	<tr>
    		<td style="" align="center" bgcolor="#e9e9e9"><?=$sisdsc; ?></td>
    	</tr>
    	</tbody>
    </table>
<table id="tblform" class="notprint" width="100%" cellspacing="0" cellpadding="2" align="center">
	<tr>
		<td class="subtitulodireita">Descrição da Regra:</td>
		<td><?=campo_texto( 'rgadesc', 'N', 'S', 'Descrição da Regra', 80, 100, '', '','','','','id="rgadesc"'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Campo:</td>
		<td><?=campo_texto( 'rgacampo', 'N', 'S', 'Campo', 30, 50, '', '','','','','id="rgacampo"'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Demandante/Gestor:</td>
		<td><?
		$sql = "SELECT DISTINCT
				    u.usucpf as codigo,
				    u.usunome as descricao
				FROM
				    seguranca.usuario u
				    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
				    inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				    inner join seguranca.perfil per on per.pflcod = pu.pflcod
				WHERE
				    us.sisid = ".$_SESSION['sisid']."
				    and per.pfldsc ilike '%admin%'
				    and per.sisid = ".$_SESSION['sisid']."
				    order by u.usunome";
		echo $db->monta_combo("rgademandante", $sql, 'S','-- Selecione um demandante --','', '', '',350,'N','rgademandante');?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Solicitante/Analista:</td>
		<td><?
		$sql = "SELECT DISTINCT
				    u.usucpf as codigo,
				    u.usunome as descricao
				FROM
				    seguranca.usuario u
				    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
				    inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				    inner join seguranca.perfil per on per.pflcod = pu.pflcod
				WHERE
				    us.sisid = ".$_SESSION['sisid']."
				    and per.pfldsc ilike '%super%'
				    and per.sisid = ".$_SESSION['sisid']."
				    order by u.usunome";
		echo $db->monta_combo("rgasolicitante", $sql, 'S','-- Selecione um solicitante --','', '', '',350,'N','rgasolicitante');?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Desenvolvedor:</td>
		<td><?
		$sql = "SELECT DISTINCT
				    u.usucpf as codigo,
				    u.usunome as descricao
				FROM
				    seguranca.usuario u
				    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
				    inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				    inner join seguranca.perfil per on per.pflcod = pu.pflcod
				WHERE
				    us.sisid = ".$_SESSION['sisid']."
				    and per.pfldsc ilike '%super%'
				    and per.sisid = ".$_SESSION['sisid']."
				    order by u.usunome";
		echo $db->monta_combo("rgadesenvolvedor", $sql, 'S','-- Selecione um desenvolvedor --','', '', '',350,'N','rgadesenvolvedor');?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Data da Solicitação:</td>
		<td><?=campo_data2('rgadata', 'S','S','Data da Solicitação','','','')?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Anexos</td>
		<td><input type="file" name="arquivo"></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Descrição do Arquivo</td>
		<td><?=campo_texto( 'arqdescricao', 'N', 'S', 'Campo', 80, 50, '', '','','','','id="arqdescricao"'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Regras</td>
		<td><?=campo_textarea('rgaregra', 'N', 'S', 'Regras', 50, 20, 10000, '', '', '', '', '');?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td></td>
		<td>
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" style="cursor: pointer;" onclick="salvarRegras();">
			<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="window.close();">
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function salvarRegras(){
	$('requisicao').value = 'salvar';
	$('formulario').submit();
}
tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
		theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,visualchars,nonbreaking,template,pagebreak",
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
</script>