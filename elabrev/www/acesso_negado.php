<?

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:elabrev.php
   Finalidade: permitir a abertura de todas as páginas do sistema com segurança
   */
?>
<html>
<head>
<title>Acesso Negado</title>
</head>
<script>
   alert ('Acesso negado');
   history.back();
</script>
<? exit();
?>
<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
<table width="100%%" border="0" cellpadding="0" cellspacing="0">
  <tr bgcolor="#FFCC00"> 
    <td><img src="imagens/logo_mec_br.gif"></td>
    <td></td>
    <td align="right"><img src="imagens/logo_brasil.gif"></td>
  </tr>
</table>
<!--Fim cabeçalho Governo Federal-->
<table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
  <tr bgcolor="#006108"> 
    <td align="left"> </td>
    <td align="center"> </td>
    <td align="right"></td>
  </tr>
<table width="100%" border="0" cellpadding="0" cellspacing="0">
  <tr bgcolor="#294054"> 
    <td height="81" background="imagens/topo.jpg"></td>
	<tr>
</table>
<table width="100%" border="0" cellpadding="0" cellspacing="0">
  <tr> 
    <td background="imagens/fundo.gif">
<table width=94% height=250 align=center cellpadding=5 cellspacing=1>
  <tr>
    <td align="center" valign="middle"><font face="Verdana" size="2"><b><font color="red">Acesso Negado!</font></b><br><br>
	Você não tem permissão para executar esta operação.<br><br>
	A Página que você tentou acessar é: <BR><font color="#0000ff"><?=$_SERVER['HTTP_REFERER']?></font><br><br><a href="login.php">Clique aqui para entrar no sistema.</a></font></td>
  </tr>
</table>
   </td>
   </tr>
   <tr>
   <td align="right" bgcolor="#2A4159"><font color="#FFFFFF" size="1"><?php echo SIGLA_SISTEMA; ?> - Ministério da Educação - Sistema em Desenvolvimento</font></td>
	</tr>
</table>

</body>
</html>
