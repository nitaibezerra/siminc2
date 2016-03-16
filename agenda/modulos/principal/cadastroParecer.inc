<?php

switch ( $_REQUEST['op'] ){
	case 'salvar':
		$histEncaminhamento 			= new HistEncaminhamento( $_POST['hseid'] );
		$dados 			 				= $_POST;
		$dados['usucpfparecer'] 		= $_SESSION['usucpf'];
		$dados['hsedtinclusaoparecer'] 	= ($_POST['hseid'] ? date('Y-m-d H:i:s') : null);
		$hseid 			 				= $histEncaminhamento->popularDadosObjeto( $dados )
			   				 		 		         		 ->salvar();
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				window.opener.location.href = \'?modulo=principal/vinculoArea&acao=E\';
				window.close();
			 </script>');
}

$evaid 		  = $_SESSION['agenda']['evaid'];
$eventoArea   = new EventoArea();
$dadosEvtArea = $eventoArea->pegaTodosDados( $evaid );
extract( $dadosEvtArea );

if ( $_GET['acao'] == 'E' ){
	if ( empty($_REQUEST['hseid']) ){
		die('<script>
				window.close();
			 </script>');		
	}
	$hseid  = $_REQUEST['hseid'];
	
	$histEncaminhamento = new HistEncaminhamento( $hseid );
	$dados  			= $histEncaminhamento->pegaTodosDados( $hseid );
	extract( $dados );	
}


//if ($_REQUEST['acao'] != 'P' || ){
//	include APPRAIZ . 'includes/cabecalho.inc';
//	print '<br/>';
//}else{
?>
<html>
	<head>
		<title>Cadastro de Informações</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<body marginheight="0" marginwidth="0">
<?php
//}
monta_titulo( 'Cadastro de Informações', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>

<script type="text/javascript">

<!--

 

function validarForm(){
	var errorText = new Array();
	 
	if(  tinyMCE.get('hseparecer').getContent() == ''){
		errorText.push('Parecer');
	}  
	
	if ( errorText.length > 0 ){
		errorText = errorText.join("\n");
		errorText = 'Os campos listados abaixo são de preenchimento obrigatório:\n' + errorText;
		alert( errorText );
		return false;
	}
	
	$('[name=op]').val('salvar');
	$('[name=formParecer]').submit();
}

tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "emendatinymce",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_buttons4 : "",
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

//-->
</script>
<form name="formParecer" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="hseid" id="hseid" value="<?php echo $hseid; ?>" type="hidden">
<input name="evaid" id="evaid" value="<?php echo $evaid; ?>" type="hidden">
<input name="esdid" id="esdid" value="<?php echo $esdid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Área:</td>
		<td>
		<?php
			echo $aevdsc;
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Estado:</td>
		<td>
		<?php
			echo $esddsc;
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Informações:</td>
		<td>
			<div>
				<textarea id="hseparecer" name="hseparecer" rows="10" cols="30" style="width:100%" class="emendatinymce"><?php echo $dados['hseparecer'] ?></textarea>
			</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Informações Complementares:</td>
		<td>
			<div>
				<textarea id="hseinfrespagenda" name="hseinfrespagenda" rows="10" cols="30" style="width:100%" class="emendatinymce"><?php echo $dados['hseinfrespagenda'] ?></textarea>
			</div>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btsalvar" value="Salvar" onclick="validarForm()" class="botao">
	   </td>
	</tr> 	
</table>
</form>
<?php 
//if ( $_REQUEST['acao'] == 'P' ){
?>	
	</body>
</html>
<?php
//}
?>

