<?php
//include_once APPRAIZ . 'emenda/classes/MinutaConvenio.class.inc';

if($_POST['requisicao'] == 'alteraTipoDocumentoDOU'){
	$sql = "SELECT
				mdoconteudo
			FROM
				emenda.modelosdocumentos
			WHERE
				mdoid = {$_POST["mdoid"]}";
	
	$mdoconteudo = $db->pegaUm( $sql );
	
	$obConvenio = new MinutaConvenio( $_SESSION["emenda"]["ptridAnalise"] );
	$obConvenio->alteraMacrosConvenioDOU($mdoconteudo);
	
	//$mdoconteudo = iconv("ISO-8859-1", "UTF-8", $mdoconteudo);	
	echo $mdoconteudo;
	die;	
}

if($_GET['imprimeWord']){
	header("Content-type: application/vnd.ms-word");
	header("Content-type: application/force-download");
	header("Content-Disposition: attachment; filename=MinutaConvenioDou.doc");
	header("Pragma: no-cache");
	
	$sql = "SELECT pubtxtpublicacao FROM emenda.ptpublicacao WHERE pmcid = ".$_REQUEST["pmcid"]." and pubstatus = 'A'";
	
	$pubtxtpublicacao = $db->pegaUm( $sql );
	
	# echo "<html>".htmlspecialchars_decode($pubtxtpublicacao)."</html>";
	echo "<html>".html_entity_decode($pubtxtpublicacao)."</html>";
	exit;
}

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

validaSessionPTA($_SESSION["emenda"]["ptridAnalise"]);
include_once APPRAIZ . 'includes/workflow.php';

if($_POST['action'] == 'salvar'){
	
	$dataPublicacao = ($_POST['pubdatapublicacao']?"'".formata_data_sql($_POST['pubdatapublicacao'])."'":"NULL");
	$pmcid = ( !empty($_POST['pmcid']) ? $_POST['pmcid'] : $db->pegaUm("select pmcid from emenda.ptminutaconvenio where ptrid =".$_POST['ptrid']." and pmcstatus = 'A'") );
	
	if( !empty($pmcid) ){
		if( !empty($_POST['pubid']) ){	
			$sql = "UPDATE emenda.ptpublicacao SET pubstatus = 'I'
		  				WHERE
		  					pmcid = ".$_POST['pmcid']." and pubstatus = 'A'";
			
			$db->executar($sql);
		}
		
		$sql = "INSERT INTO emenda.ptpublicacao( pmcid, refid, pubtxtpublicacao, pubdatapublicacao, pubpagdou, pubdatainclusao, usucpfinclusao, mdoid, pubstatus, pubnumportaria) 
				VALUES (".$pmcid.", null, '".simec_htmlspecialchars($_POST['texto'], ENT_QUOTES)."',
					 	".$dataPublicacao.", '".$_POST['pubpagdou']."', 'now()', 
					 	'".$_SESSION['usucpf']."', ".$_POST['mdoid'].", 'A', '".$_POST['pubnumportaria']."')";
	
		$db->executar($sql);
		$db->commit();
		$db->sucesso('principal/minutaConvenioDOU');
	} else {
		echo "<script>alert('É necessário cadastrar um convênio na aba Minuta de Convênio!');
			window.location.href = 'emenda.php?modulo=principal/minuta&acao=A';
			</script>";
	}
}

if( $_POST['action'] == 'email' ){
	if( !empty($_POST['pmcid']) && !empty($_POST['pubid']) ){
		
		$strAssunto = 'Publicação - SIMEC Convênio '.$_POST['ptrnumconvenio'].'/'.$_POST['ptranoconvenio'];
		//$strMensagem = simec_htmlspecialchars($_POST['texto'], ENT_QUOTES);
		$strMensagem = $_POST['texto'];
		$usrLogado = $db->pegaUm("select usuemail from seguranca.usuario where usucpf = '{$_SESSION['usucpf']}'");
		if( $usrLogado )
			$strEmailTo = array('dicom_publicacao@fnde.gov.br', $usrLogado);
		else
			$strEmailTo = 'dicom_publicacao@fnde.gov.br';			
		
		if(enviaEmailAnalise( $strAssunto, $strMensagem, $strEmailTo )){
			echo "<script>alert('E-mail enviado com sucesso!');
						window.location.href = 'emenda.php?modulo=principal/minutaConvenioDOU&acao=A';
				  </script>";
		} else {
			echo "<script>alert('Não foi possível enviar o e-mail!');
						window.location.href = 'emenda.php?modulo=principal/minutaConvenioDOU&acao=A';
				  </script>";
		}
	} else {
		echo "<script>alert('Não foi possível enviar o e-mail, é necessário cadastrar uma publicação!');
					window.location.href = 'emenda.php?modulo=principal/minutaConvenioDOU&acao=A';
			  </script>";
	}
	exit();
}

$obConvenio = new MinutaConvenio( $_SESSION["emenda"]["ptridAnalise"] );

if( $_POST['action'] == 'alterar' ){
	$ptminutaconvenio = $obConvenio->carregaPublicacaoPTA( $_SESSION["emenda"]["ptridAnalise"], $_POST['pubid'] );
} else {
	$ptminutaconvenio = $obConvenio->carregaPublicacaoPTA( $_SESSION["emenda"]["ptridAnalise"] );
}

$pubdatapublicacao = $ptminutaconvenio['pubdatapublicacao'];
$pubpagdou = $ptminutaconvenio['pubpagdou'];
$pubnumportaria = $ptminutaconvenio['pubnumportaria'];
$pmcid = $ptminutaconvenio['pmcid'];
$pubid = $ptminutaconvenio['pubid'];
$mdoid = $ptminutaconvenio['mdoid'];
$ptrnumconvenio = $ptminutaconvenio['ptrnumconvenio'];
$ptranoconvenio = $ptminutaconvenio['ptranoconvenio'];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

if($ptminutaconvenio['pubtxtpublicacao']){
	$imitexto = $ptminutaconvenio['pubtxtpublicacao'];
} else {
//	$sql = "SELECT
//				mdo.mdoconteudo
//			FROM 
//				emenda.modelosdocumentos mdo
//			WHERE 
//                mdo.tpdcod = 100 and mdo.mdoid = {$_GET["mdoid"]}
//				and mdo.mdostatus = 'A'";
//	$imitexto = $db->pegaUm($sql);

//	if($imitexto){
//		$obConvenio->alteraMacrosConvenioDOU($imitexto);
//	}
//	else{
		$imitexto = "Não existem informações sobre esta minuta.";
	//}
}

$imitexto = str_replace('"', "'", $imitexto);

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
monta_titulo( 'Minuta Convênio DOU', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptridAnalise"], true);

montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], false, $boReformulacao );

$filtro = ( $boReformulacao ? " and ptv.vigtipo = 'A'" : " and ptv.vigtipo = 'P'" );

$sql = "SELECT
			to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio
		FROM 
			emenda.ptvigencia ptv
		WHERE 
			ptv.ptrid = ".$_SESSION["emenda"]["ptridAnalise"]."
			and ptv.vigstatus = 'A' $filtro";

$ptminuta = $db->pegaLinha($sql);
$vigdatainicio = $ptminuta['vigdatainicio'];

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );
?>
<html>
<head>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
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
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
</head>
<body>
<form id="formMinutaConvenioDOU" method="post" action="">
<input type="hidden" id="imitexto" value="<?php echo (($imitexto) ? $imitexto : 'Clique na Minuta desejada para recuperá-la.')?>" />
<input type="hidden" name="action" id="action" value="" />
<input type="hidden" name="pmcid" id="pmcid" value="<?=$pmcid; ?>" />
<input type="hidden" name="pubid" id="pubid" value="<?=$pubid; ?>" />
<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid; ?>" />
<input type="hidden" name="ptrnumconvenio" id="ptrnumconvenio" value="<?=$ptrnumconvenio; ?>" />
<input type="hidden" name="ptranoconvenio" id="ptranoconvenio" value="<?=$ptranoconvenio; ?>" />
<input type="hidden" name="vigdatainicio" id="vigdatainicio" value="<?=$vigdatainicio; ?>" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="subtitulodireita" width="200px">Tipo do Documento:</td>
		<td colspan="3">
			<?php 
			
				$sql = "SELECT em.mdoid as codigo,
						       em.mdonome as descricao
						FROM emenda.modelosdocumentos em
							INNER JOIN public.tipodocumento tp ON tp.tpdcod = em.tpdcod
						WHERE em.mdostatus = 'A' and tp.tpdcod = 100
							ORDER BY descricao
						";
				
				$db->monta_combo("mdoid", $sql, 'S', "Selecione...", 'alteraTipoDocumentoDOU', '', '', '500', 'N', 'mdoid');
				
			?>
		</td>
	</tr>
</table>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td>
		<table class="tabela" align="left" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="3"><b>Minuta de Convênio DOU</b></td>
	</tr>
	<tr>
		<td colspan="3">
			<div>
				<textarea id="texto" name="texto" rows="30" cols="80" style="width:100%" class="emendatinymce"></textarea>
			</div>
		</td>
	</tr>
	<?php
	if( !in_array( ADMINISTRADOR_INST, $arPerfil ) && !$db->testa_superuser() ){
		$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );
		if($estadoAtual != EM_GERACAO_DA_MINUTA_DE_CONVENIO && $estadoAtual != EM_PUBLICACAO_NO_DOU && $estadoAtual != EM_PUBLICACAO_REFORMULACAO){
			$disabled = 'disabled="disabled"'; 
		} elseif($estadoAtual == EM_PUBLICACAO_REFORMULACAO) {
			if(!possuiPermissao(EM_PUBLICACAO_REFORMULACAO)) {
				$disabled = 'disabled="disabled"';
			} else {
				$disabled = '';
			}
		} else {
			if(!possuiPermissao()) {
				$disabled = 'disabled="disabled"';
			} else {
				$disabled = '';
			}
		}
	}
	
	if( !empty($ptminutaconvenio['pmcid']) ){
	?>
	<tr bgcolor="#c0c0c0">
		<td>
		<?
			echo 'Data de publicação: <br>';
			echo campo_data2( 'pubdatapublicacao','S', 'S', 'Data do Publicação', 'S', '', '');	
		?>
		</td>
		<td>
		<?
			echo 'Página da publicação: <br>';
			echo campo_texto( 'pubpagdou', 'N', 'S', 'Página da publicação', 80, 100, '', '','','','','id="pubpagdou"');
		?>
		</td>
		<td>
		<?
			echo 'Secão da publicação: <br>';
			echo campo_texto( 'pubnumportaria', 'N', 'S', 'Portaria da publicação', 40, 50, '', '','','','','id="pubnumportaria"');
		?>
		</td>
	</tr>
	<?} else {
		  echo '&nbsp;';
	  } ?>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="3">
			<?
			if( in_array( CONSULTA_GERAL, $arPerfil ) ) $disabled = 'disabled="disabled"';
			//if(!$disabled) {
			?>
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarMinutaDOU();" <?=(!empty($disabled) ? $disabled : verificaPermissaoPerfil('analise', '', EM_PUBLICACAO_REFORMULACAO, true));?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="history.back(-1);" />&nbsp;
			<?php
			//}
			if( !empty($ptminutaconvenio['pmcid']) && empty($disabled) ){
				echo '<input type="button" value="Imprimir" onclick="imprimirMinutaPTA(\''.$ptminutaconvenio["pmcid"].'\');" />';
				echo '&nbsp;<input type="button" value="Imprime Word" onclick="minutaConvenioWord(\''.$ptminutaconvenio["pmcid"].'\');" />';	
				echo '&nbsp;<input type="button" id="bt_enviaemail" value="Enviar e-mail" onclick="enviarEmailPublicacao(\''.$ptminutaconvenio["pmcid"].'\');" />';	
			} else {
				echo '<input type="button" value="Imprime Word" disabled="disabled"/>';
				echo '<input type="button" value="Imprimir" disabled="disabled"/>';
				echo '<input type="button" value="Enviar e-mail" disabled="disabled"/>';
			}
			?>
		</td>
	</tr>
</table>
	</td>
	<td valign="top" style="text-align: center; width: 20px;">
			<?php
				$docid = criarDocumento( $_SESSION["emenda"]["ptridAnalise"] );
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $_SESSION["emenda"]["ptridAnalise"], 'tipo' => "" ) );
			?>
		</td>
	</tr>
</table>
</form>
<?
$_POST['texto'] = '';
echo $obConvenio->listaPublicacaoPTA( $pmcid, $arPerfil );
?>

<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
</body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
document.getElementById('texto').value = document.getElementById('imitexto').value;

var form = document.getElementById('formMinutaConvenioDOU');
var data1 = '<? echo date("d/m/Y") ?>';

jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
		//$('#td_acao_445').css('display', 'none');
	});
});

function salvarMinutaDOU(mdoid) {
	if(document.getElementById('pubdatapublicacao') ) {

		if(document.getElementById('pubdatapublicacao').value != "") {
			if(!validaData(document.getElementById('pubdatapublicacao'))) {
				alert('Data de publicação incorreta');
				return false;
			}
			if( !validaDataMaior( document.getElementById('vigdatainicio'), document.getElementById('pubdatapublicacao') ) ){
				alert("A data informada não pode ser menor que data celebração: " + document.getElementById('vigdatainicio').value);
					document.getElementById('pubdatapublicacao').focus();
				return false;
			}
			/*if(parseInt( document.getElementById('pubdatapublicacao').value.split( "/" )[2].toString() + document.getElementById('pubdatapublicacao').value.split( "/" )[1].toString() + document.getElementById('pubdatapublicacao').value.split( "/" )[0].toString() ) < parseInt( data1.split( "/" )[2].toString() + data1.split( "/" )[1].toString() + data1.split( "/" )[0].toString() ) ) {
				alert('Data de publicação não pode ser menor do que a data atual');
				return false;
			}*/
		}
	}

	if(document.getElementById('mdoid').value == '') {
		alert('O Tipo de Documento é um campo obrigatório.');
		return false;
	}
		
	document.getElementById("action").value = "salvar";
	form.submit();
}
function imprimirMinutaPTA(pmcid) {
	var janela = window.open( '/emenda/emenda.php?modulo=principal/popImprimeMinutaPTA&acao=A&tipo=D&pmcid='+pmcid+'', 'imprimeminuta', 'width=450,height=400,status=0,menubar=1,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}

function enviarEmailPublicacao(pmcid){
	if( confirm('Deseja enviar e-mail para o responsável da publicação?') ){
		document.getElementById("action").value = "email";
		form.submit();
	}
}

function alteraTipoDocumentoDOU( mdoid ){
	if(mdoid){
		var url = 'emenda.php?modulo=principal/minutaConvenioDOU&acao=A';
		var parametros = '&requisicao=alteraTipoDocumentoDOU&mdoid=' + mdoid;
	
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					//$('divTeste').update(resp.responseText);
					tinyMCE.get('texto').setContent( resp.responseText );		
				}
			}
		);	
	} else {
		tinyMCE.get('texto').setContent('');
	}
}
function minutaConvenioWord(pmcid){
	if(pmcid){
		window.location.href='emenda.php?modulo=principal/minutaConvenioDOU&acao=A&pmcid='+pmcid+'&imprimeWord=1';
	}
}

function carregaPublicacao( pubid, pmcid ){
	document.getElementById("pubid").value = pubid;
	document.getElementById("pmcid").value = pmcid;
	document.getElementById("action").value = "alterar";
	form.submit();
}
</script>
</html>
<div id='divTeste'></div>