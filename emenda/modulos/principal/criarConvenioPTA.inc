<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/workflow.php';



if($_REQUEST['email']){
	header('content-type: text/html; charset=ISO-8859-1');
	enviarEmailAnexosConvenio($_POST);
	exit;
}

//ver( $_REQUEST,d );

if( $_REQUEST['submeter'] == 'salvar' ){
	#manipular anexos de arquivos para envio de email

	if( empty( $_POST['mailid'] ) ){
		$sql = "INSERT INTO 
				  emenda.ptmail(
				  ptrid,
				  usucpf,
				  mailtxtconteudo,
				  maildatainclusao
				) 
				VALUES (
				  ".$_SESSION['emenda']['ptridAnalise'].",
				  '".$_SESSION['usucpf']."',
				  '".simec_htmlspecialchars($_REQUEST["texto"], ENT_QUOTES)."',
				  'now()'
				)";
		
		$db->executar($sql);
		$db->commit();
		$db->sucesso('principal/criarConvenioPTA');
	} else {	
		$sql = "UPDATE 
				  	emenda.ptmail  
				SET 
				  	mailtxtconteudo = '".simec_htmlspecialchars($_REQUEST["texto"], ENT_QUOTES)."',
				  	maildatainclusao = 'now()',
				  	usucpf = '".$_SESSION['usucpf']."'
				 
				WHERE 
				  	mailid = ".$_POST['mailid'];
	
		$db->executar($sql);
		$db->commit();
		$db->sucesso('principal/criarConvenioPTA');
	}
}

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

validaSessionPTA( $_SESSION["emenda"]["ptridAnalise"] );

if( $_REQUEST['mailid'] ){
	$sql = "SELECT
			ptmc.pmcid, 
			ptm.mailid,
			ptm.mailtxtconteudo
		FROM 
			emenda.ptminutaconvenio ptmc
		    inner join emenda.planotrabalho ptr
		    	on ptr.ptrid = ptmc.ptrid
		    left join emenda.ptmail ptm
		    	on ptm.ptrid = ptr.ptrid
		WHERE
		  	ptm.mailid = ".$_REQUEST['mailid']."
		    and ptmc.pmcstatus = 'A'";
} else {

	$sql = "SELECT
				ptmc.pmcid, 
				ptm.mailid,
				ptm.mailtxtconteudo
			FROM 
				emenda.ptminutaconvenio ptmc
			    inner join emenda.planotrabalho ptr
			    	on ptr.ptrid = ptmc.ptrid
			    left join emenda.ptmail ptm
			    	on ptm.ptrid = ptr.ptrid
			WHERE
			  	ptmc.ptrid = {$_SESSION["emenda"]["ptridAnalise"]}
			    and ptmc.pmcstatus = 'A'
			ORDER BY
            	ptm.mailid desc";
}

$arConvenio = $db->pegaLinha($sql);

$pmcid = $arConvenio['pmcid'];
$mailid = $arConvenio['mailid'];

if( empty($mailid) ){
	$desabilita = 'disabled="disabled"'; 	
} else {
	$desabilita = '';
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
monta_titulo( 'Criar Convênio do PTA', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptridAnalise"], true);

montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], false, $boReformulacao );

$imitexto = trim($arConvenio['mailtxtconteudo']);
$imitexto = str_replace('"', "'", $imitexto);
?>
<html>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript">
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
</script>
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
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
<input type="hidden" name="submeter" id="submeter" value="1">
<input type="hidden" name="mailid" id="mailid" value="<?php echo $mailid; ?>">
<input type="hidden" id="imitexto" value="<?php echo $imitexto; ?>" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Convênio Associado ao PTA:</b></td>
		<?php
		if( $pmcid ){
			echo '<td ><a href="#" onclick="imprimirConvenioPTA(\''.$pmcid.'\')">Convênio do PTA</a></td>';
		} else {
			echo '<td style="color: red;">Não Existe convênio associado ao PTA</td>';
		}
		?>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Corpo do e-mail:</b></td>
		<td><textarea id="texto" name="texto" rows="25" cols="80" style="width:100%" class="emendatinymce"></textarea></td>
		<td>
		<?php
		$docid = criarDocumento( $_SESSION["emenda"]["ptridAnalise"] );
		wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $_SESSION["emenda"]["ptridAnalise"], 'tipo' => "" ) );
		?>
		</td>
	</tr>
	<?php
	
	if( pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] ) != EM_CRIACAO_DO_COVENIO){ //EM_CRIACAO_DO_COVENIO
		$disabled = 'disabled="disabled"'; 
	} else{
		/*if(!possuiPermissao()) {
			$disabled = 'disabled="disabled"';
		} else {
			$disabled = '';
		}*/
	}
	?>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="3">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarEmail();" <?=(!empty($disabled) ? $disabled : verificaPermissaoPerfil('analise', '', '', true)); ?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="history.back(-1);" />
			&nbsp;
			<input type="button" id="bt_enviar" value="Enviar e-mail" <?=($imitexto ? '': 'disabled="disabled"'); ?> onclick="enviarEmail();" <?=verificaPermissaoPerfil('analise', '', '', true); ?> />
		</td>
	</tr>
</table>
</form>
<div id="erro"></div>
</body>
<?
$sql = "SELECT
			'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaEmailConvenio('||p.mailid||');\" border=\"0\"> ' as acao,	
			usu.usunome,
		    p.maildataenvio,
		    p.mailtxtconteudo
		FROM
			emenda.ptmail p
		    inner join seguranca.usuario usu on usu.usucpf = p.usucpf
		WHERE 
			p.ptrid = ".$_SESSION["emenda"]["ptridAnalise"];

monta_titulo( '', 'Lista de e-mail' );
$cabecalho = array("Ação","Nome", "Data de Envio", "e-mail");
echo $db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center','','form');

?>
<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
		//$('#td_acao_445').css('display', 'none');
	});
});

document.getElementById('texto').value = document.getElementById('imitexto').value;

function salvarEmail(){
	var ed = tinyMCE.get('texto').getContent();

	$('submeter').value = 'salvar';
	if( ed == "" ){
		alert('Informe o Corpo do e-mail');
		return false;
	}
	document.getElementById('formulario').submit();
}

function imprimirConvenioPTA(pmcid) {
	var janela = window.open( '/emenda/emenda.php?modulo=principal/popImprimeMinutaPTA&acao=A&pmcid='+pmcid+'', 'imprimeminuta', 'width=450,height=400,status=0,menubar=1,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}

var objForm = document.forms["formulario"];

if(objForm.arqid.value != ''){
	objForm.arquivo.disabled = true;
}
 
function enviarEmail(){
	$('loader-container').show();
		var myajax = new Ajax.Request('emenda.php?modulo=principal/criarConvenioPTA&acao=A', {
			        method:     'post',
			        parameters: '&email=true&'+$('formulario').serialize(),
			        asynchronous: false,
			        onComplete: function (res){
			        	 //$('erro').innerHTML = res.responseText; 
						if(res.responseText == "1"){
							alert('E-mail enviado com sucesso');
						} else if(res.responseText == ""){
							alert('Não foi possivel enviar o e-mail');
						}else {
							alert(res.responseText);
						}
			        }
			  });
	$('loader-container').hide();
}

function carregaEmailConvenio( mailid ){
	window.location.href = 'emenda.php?modulo=principal/criarConvenioPTA&acao=A&mailid='+mailid;
}
</script>
</html>