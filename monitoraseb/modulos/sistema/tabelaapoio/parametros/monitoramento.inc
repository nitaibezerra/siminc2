<?php
// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';  

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Parâmetros','');

include APPRAIZ."monitoraseb/classes/ParametroMonitoramento.class.inc";
$parametroMonitoramento = new ParametroMonitoramento();

if($_REQUEST['requisicao']=='salvar'){
	
	$mensagem = $parametroMonitoramento->salvarMonitoramento($_REQUEST[parid],$_REQUEST[pardscmonitoramento]);
	$parametroMonitoramento->commit();
	
	if($mensagem != ""){?>
		<script>
			alert('<?=$mensagem?>');
		</script>
	<?php }
}

//Carrega dasdos
$idRegistro = $parametroMonitoramento->recuperaIdRegistro();

if($idRegistro!=null && $idRegistro!="") {
	$parametroMonitoramento->carregarPorId($idRegistro);
	$arrDados = $parametroMonitoramento->getDados();
	extract($arrDados);
}

?>
<script language="JavaScript">
	window.onbeforeunload = verificaAlteracao;
 	function gravar(){
 		var conteudo = tinyMCE.activeEditor.getContent();
 		
  		if (conteudo && conteudo.length > 3000){
  			alert("O campo tamanho máximo do campo e de 3000 caracteres.");
  		}else{
			document.getElementById('alteracaodados').value = 0;
			$('#formularioCadastro').submit();
  		}
	}

 	function verificaAlteracao() {
 		if ( document.getElementById('alteracaodados').value == "1" ) {
 				return 'Atenção. Existem dados do formulário que não foram guardados.';
 		}
 	}

	
</script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript">
	tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "txareanormal",
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
		media_external_list_url : "lists/media_list.js"

	});
</script>

<form method="POST"  name="formularioCadastro" action="" id="formularioCadastro">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Monitoramento:</td>
			<td>
				<?php $parametroMonitoramento ? $pardscmonitoramento : '' ?>
				<?=campo_textarea('pardscmonitoramento','N', 'S', '', 80, 25, '','onkeypress="document.getElementById(\'alteracaodados\').value = 1;"','','','','',''); ?>
				<input type="hidden" name="parid" value="<?=$idRegistro?>"/>
				<input type="hidden" name="requisicao" value="salvar"/>
				<input type="hidden" id="alteracaodados" value="0"/>
			</td>
		</tr>
		<tr>
			<th></th>
			<th style="text-align: left;">
				<input style="cursor: pointer;"  type="button" value="Salvar" name="btnGravar" onclick="gravar();">
				<input style="cursor: pointer;"  type="button" value="Cancelar" name="btnCancelar" onclick="document.getElementById('alteracaodados').value = 0;">
			</th>
		</tr>
	</table>
</form>