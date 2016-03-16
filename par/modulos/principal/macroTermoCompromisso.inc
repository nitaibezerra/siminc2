<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$preid = 1001;

$mdoid = ($_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $ptminutaconvenio['mdoid']);

if($_REQUEST["submetido"]) {
	if($_REQUEST['mdoid'] != "") {
		
		$sql = "SELECT mdoconteudo FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_REQUEST['mdoid'];
		$imitexto = $db->pegaUm($sql);
		
		if($imitexto){
			$obMacro = new AlteraMacroDocumento($preid);
			$obMacro->alteraMacrosDocumento( $imitexto, $preid );
		}else{
			$imitexto = "Não existem informações sobre esta minuta.";
		}
	}
	if($_REQUEST['pmcid'] != "" && !$_REQUEST['mdoid']) {
		$imitexto = $ptminutaconvenio["pmctexto"];
	}
} else {
	if($ptminutaconvenio) {
		$imitexto = $ptminutaconvenio["pmctexto"];
	}
}

$imitexto = str_replace('"', "'", $imitexto);


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo( 'Criação da Termo de Compromisso', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');
?>	
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<form id="formulario" method="post" action="">

<input type="hidden" name="submetido" id="submetido" value="1" />
<input type="hidden" name="mdoid" id="mdoid" value="<?=($_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $ptminutaconvenio['mdoid'])?>" />
<input type="hidden" name="salvar1" id="salvar1" value="" />
<?php

if($ptminutaconvenio){
	echo '<input type="hidden" id="imitexto" value="'. (($imitexto) ? $imitexto : 'Clique na Minuta desejada para recuperá-la.').'" />';
} else {
	echo '<input type="hidden" id="imitexto" value="" />';
}
?>


<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td>
		<table class="tabela" align="left" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="3"><b>Termo Compromisso</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Modelos de Minutas Cadastradas:</b></td>
		<td >
		<?
		$modelo = (  $_REQUEST['mdoid'] ? $_REQUEST['mdoid'] : $ptminutaconvenio['mdoid'] );
		$sql = "SELECT
					mdo.mdoid as codigo,
					mdo.mdoid ||' - '|| case when mdo.mdoid is null then 'Não existe documento vinculado' else mdo.mdonome end as descricao
				FROM 
					par.modelosdocumentos mdo
				WHERE
					mdo.tpdcod = 99
					and mdo.mdostatus = 'A'";
		
		$db->monta_combo("modelo", $sql, $habilita, 'Selecione...', 'carregaMinutaIniciativa', '', '', '', '', 'modelo', '', '', 'Lista de modelo(s) vinculado(s) a minuta convênio' );
		?>
		</td>
	</tr>
	<?php
	if($ptminutaconvenio["pmctexto"]) {
		$style = (!$mdoid) ? "style=\"color:#ff0000\"" : "";
		echo '<tr>
				<td class="SubTituloDireita" valign="middle"><b>Minuta Associada ao PTA:</b></td>
			    <td ><a href="#" '.$style.' onclick="carregaMinutaPTA('.$ptminutaconvenio["pmcid"].')">Minuta de Convênio do PTA</a>&nbsp;<input type="button" value="Imprimir" onclick="imprimirMinutaPTA('.$ptminutaconvenio["pmcid"].');" /></td>
			  </tr>';
	}
	?>
	<tr>
		<td colspan="3">
			<div>
				<textarea id="texto" name="texto" rows="30" cols="80" style="width:100%" class="emendatinymce"></textarea>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="3">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarTermoCompromisso();" />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="history.back(-1);" />
		</td>
	</tr>
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<div id="erro"></div>
</body>
<script type="text/javascript">
var btSalvar	= document.getElementById("bt_salvar");
var btCancelar	= document.getElementById("bt_cancelar");
var form		= document.getElementById("formulario");

document.getElementById('texto').value = document.getElementById('imitexto').value;

tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "emendatinymce",
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

function salvarTermoCompromisso() {
	$('bt_salvar').disabled 	= true;
	$('bt_cancelar').disabled 	= true;
	document.getElementById("submetido").value = "1";
	document.getElementById("salvar1").value = "1";
	form.submit();
}

function carregaMinutaIniciativa() {
	var mdoid = document.getElementById('modelo').value;
	document.getElementById("submetido").value = "1";
	document.getElementById("mdoid").value = mdoid;
	$('bt_salvar').disabled 	= true;
	$('bt_cancelar').disabled 	= true;
	if( mdoid ){
		form.submit();
	}
}
</script>