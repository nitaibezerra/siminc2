<?
if( trim($_REQUEST['requisicao']) == 'filtraemail' ){
	$post = $_POST;
	//ver($post,d);
	if( $_POST['postPTA'] ){
		$post[] = (array)json_decode( urldecode($_POST['postPTA']) );
	}
	$obEmenda = new Emenda();
	$filtros = filtroPtaAnalise( ($post[0] ? $post[0] : $post) );
	$arEmail = $obEmenda->enviaEmailAnalisePTA( $filtros, ($post[0] ? $post[0] : $post) );
	
	$arEmail = $arEmail ? $arEmail : array();
	if( empty($arEmail) ){
		?>
		      <script>
		         alert ('Não foi encontrado e-mail cadastrado para essa entidade.');
		         window.close();
		      </script>
		   <?
	}
	$strEmailTo = array();
	$enbid = '';
	
	$emailusario = ( $post['emailentidade'] ? 'enbemail' : 'usuemail' );
	
	foreach ($arEmail as $key => $v) {
		if( $v['enbid'] != $enbid ){
			$strEmailTo[] = $v[$emailusario];
			$enbid = $v['enbid'];
		}
	}
	$postPTA = urlencode( simec_json_encode(($post[0] ? $post[0] : $post)) );
	$requisicao = 'filtraemail';
} else {
	$obEntidade = new EntidadeBeneficiada();	
	$strEmailTo = $obEntidade->enviaEmailEntidade();
	$strEmailTo = $strEmailTo ? $strEmailTo : array();
	$requisicao = 'entidade';
}
$strAssunto = ($post['strAssunto'] ? $post['strAssunto'] : "SIMEC - Emenda");

if ($_REQUEST['email']){
	
	if (ereg_replace("<[^>]*>","",$_REQUEST['email']) == ''){
		   ?>
		      <script>
		         alert ('O texto do e-mail não pode estar vazio.');
		         history.back();
		      </script>
		   <?
		     exit();
	} else {
		  // envia email
		  $arstrEmailTo = array();
		  $strAssunto = $_REQUEST['strAssunto'];
		  $strMensagem = $_REQUEST['email'];
		  if( !empty($_POST['emailUsu']) ){
		  	  $arstrEmailTo = ($_POST['emailUsu'] ? explode( ';', $_POST['emailUsu'] ) : $strEmailTo);
		  	  $strEmailTo = array();
			  foreach ($arstrEmailTo as $v){
			  	$v = trim($v);
			  	if( !empty($v) ) array_push( $strEmailTo, $v );
			  }
		  }
		  if( enviaEmailAnalise($strAssunto, $strMensagem, $strEmailTo) ){  
			  //enviar_email_usuario($emailSession, $paraUsu, $assunto, $mensagem, $cc, $cco);
		  	if( !empty($arEmail) ){
		  		$enbid = '';
				foreach ($arEmail as $key => $v) {
					if( $v['enbid'] != $enbid ){
						$sql = "INSERT INTO emenda.ptmail( ptrid, usucpf, mailtxtconteudo, maildataenvio, maildatainclusao )
								VALUES ( {$v['ptrid']},
										 '".$_SESSION['usucpf']."',
										 '".$strMensagem."',
										 now(),
										 now()
									)";
						$enbid = $v['enbid']; 
						$db->executar( $sql );
					}
				}
				$db->commit( $sql );
			}
			  ?>
		      <script>
		         alert('Email enviado com sucesso. Esta janela será fechada.')
		         window.close();
		      </script>
			  <?
			  exit();	
		} else {
			 ?>
		      <script>
		         alert('Ocorreu problema no envio do e-mail.');
		      </script>
			  <?
		}
	}
}
$emailUsu = implode( '; ', $strEmailTo );
?>
<html>
<head>
<title>Envio de Email</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>

</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="<?=$requisicao; ?>">
<input type="hidden" name="postPTA" id="postPTA" value="<?=$postPTA; ?>">
<input type="hidden" name="texto" id="texto" value="<?=simec_htmlentities( str_ireplace( '\"', '"', $post['strMensagem']) ); ?>">
    <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Enviar Email</td>
	 </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Cco:</td> 
        <td><?echo campo_textarea( 'emailUsu', 'S', 'S', '', '115', '6', '', '' , 0, ''); //=campo_texto('usunome','N','N','',70,100, '', '','','','','','',$emailUsu,'');?></td>
     </tr>    
	  <tr>
        <td align="right" class="subtitulodireita">Assunto:</td> 
        <td><?=campo_texto('strAssunto','S','S','',70,100,'','');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">ATENÇÃO:</td> 
        <td><b><font color="red">EVITE COPIAR TEXTOS FORMATADOS DO WORD PORQUE SE O DESTINATÁRIO UTILIZAR O OUTLOOK, A MENSAGEM PODE FICAR CONFUSA E INTELIGÍVEL!</font></td>
     </tr> 
     <tr>    
        <? $email= '';?>
        <td colspan=2>
        	<div>
				<textarea id="email" name="email" rows="20" cols="132" style="width:100%" class="emendatinymce"></textarea>
			</div
        </td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita"><input type='button' class="botao" value='Enviar E-mail' onclick="envia_email()">&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form>
<script language="JavaScript" src="../includes/tiny_mce.js"></script> 
<script type="text/javascript">
	document.getElementById('email').value = document.getElementById('texto').value;
	// Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		editor_selector : "emendatinymce",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
	});
	function fechar_janela() {
				window.close();
			}
			
	function envia_email() {
		if ( !validaBranco( document.formulario.strAssunto, 'Assunto' ) ) {
			return;
		}
		document.formulario.submit();
	}
</script>
</body>
</html>
