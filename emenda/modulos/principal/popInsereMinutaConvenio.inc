<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

?>

<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script type="text/javascript" src="/includes/prototype.js"></script>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script> $('loader-container').show(); </script>

<!-- TinyMCE -->
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript">
	tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
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
</script>
<!-- /TinyMCE -->

<table class="tabela" bgcolor="#f5f5f5" width="100%" cellSpacing="1" cellPadding="3" align="center">
	<tr height="10px">
		<td align="center" bgcolor="#dcdcdc"><b>Minuta de Convênio</b></td>
	</tr>
	<tr>
		<td align="center">
			<div>
				<textarea id="texto" name="texto" rows="30" cols="80" style="width:100%"></textarea>
			</div>
		</td>
	</tr>
	<tr height="10px">
		<td align="center" bgcolor="#dcdcdc"><b>Dados a serem adicionados</b></td>
	</tr>
	<tr>
		<td align="center">
			<br />
			<?
				$sql = "SELECT
							miccampo as codigo,
							micdescricao as descricao
						FROM
							emenda.minutacampos
						WHERE
							micstatus = 'A'
						order by descricao";
				$db->monta_combo("miccampo", $sql, 'S', "Selecione...", '', '', '', '200', 'N', '');
			?>
			<input type="button" id="adicionar" value="Adicionar" onclick="adicionaMacro();"/>
			<br />&nbsp;
		</td>
	</tr>
	<tr>
		<td align="center" bgcolor="#c0c0c0">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarMinuta();" />
			<input type="button" id="bt_fechar" value="Fechar" onclick="self.close();" />
		</td>
	</tr>
</table>

<script type="text/javascript">

var btSalvar 		= document.getElementById("bt_salvar");
var btFechar 		= document.getElementById("bt_fechar");
var micCampo		= document.getElementsByName("miccampo")[0];
var minutaConvenio 	= window.opener.document.getElementById("imitexto");

document.getElementById('texto').value = minutaConvenio.value;

function salvarMinuta() {
	btSalvar.disabled = true;
	btFechar.disabled = true;
	
	minutaConvenio.value = tinyMCE.get('texto').getContent();
	alert("Dados gravados com sucesso.");
	self.close();
}

function adicionaMacro() {
	var macro = micCampo.options[micCampo.selectedIndex].value;
	tinyMCE.execCommand('mceInsertContent', false, macro);
}

$('loader-container').hide();

</script>