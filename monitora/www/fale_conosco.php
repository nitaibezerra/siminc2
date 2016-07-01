<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:envia_email.inc
   Finalidade: permitir escrever e enviar email
   */
include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();


  $email = $_SESSION['ittemail'];

 

if ($_REQUEST['email']){
if (ereg_replace("<[^>]*>","",$_REQUEST['email']) == '')
{
	   ?>
	      <script>
	         alert ('O texto do e-mail não pode estar vazio.');
	         history.back();
	      </script>
	   <?
	     exit();
}
else
{
  // envia email
 
  $assunto = $_REQUEST['assunto'];
  $mensagem = $_REQUEST['email'];
  $email = $_SESSION['ittemail'];
  email('Administradores do SIMEC', $email, $assunto, $mensagem);
  ?>
      <script>
         alert('Email enviado com sucesso. Esta janela será fechada.')
         window.close();
      </script>
  <?
  exit();

}
}

$email= '';
?>
<html>
<head>
<title>Envio de Email</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
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
</script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="cpf" value="<?=$_REQUEST['cpf']?>">
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
<tr><td><font size="3"><p><b>Prezado Usuário (a)</b></p>Caso tenha qualquer dúvida, reclamação ou sugestão, entre em contato conosco por telefone ou por e-mail.
<p>Dúvidas sobre o PPA, preenchimento e datas:61-2104.8584 ou 61-2104.9827.
</p>
<p>Caso prefira o meio eletrônico, utilize nosso sistema de e-mail (abaixo disponibilizado) ou envie sua mensagem para o endereço <A HREF="mailto:<?php echo $_SESSION['email_sistema']; ?>"><?php echo $_SESSION['email_sistema']; ?></a></p></font>
<p>A sua opinião é muito importante para nós</p>
</td></tr></table>
    <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Enviar Email</td>
	 </tr>
	 <tr>
        <td align="right" class="subtitulodireita">Assunto:</td> 
        <td><?=campo_texto('assunto','S','S','',70,100,'','');?></td>
     </tr>
     <tr>
        <td colspan=2><?=campo_textarea('email','N','S','','97%',13,'');?></td>
     </tr>
	 <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">
     <input type='button' class="botao" value='Enviar E-mail' onclick="envia_email()">&nbsp;&nbsp;&nbsp;
     <input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 
<script>
  function fechar_janela()
  {
    window.close();

  }
    function envia_email()
  {
  	
  	if (!validaBranco(document.formulario.assunto, 'Assunto')) return;
	//verificação do campo corpo email
	document.formulario.email.value = tinyMCE.getContent('email');
	if (!validaBranco(document.formulario.email, 'Texto da Mensagem')) return tinyMCE.execCommand('mceFocus', true, 'email');
	
	document.formulario.submit();

  }

</script>
</body>
</html>
