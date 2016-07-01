<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:msg_geral.inc
   Finalidade: permitir a visualização de mensagens genéricas em um pop-up
   */
?>

<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='includes/listagem.css'>
<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">

<table  border=1 class="tabela" bgcolor="yellow" cellspacing="1" cellpadding="3" align="center" style="color:#000000;">
<?
for ($i=0;$i<=count($_SESSION['texto']);$i++)
{
   ?>
   <tr>
<td>
<?=$_SESSION['texto'][$i];?>
</td>
</tr>
<?
}
?>

<tr>
<td>
<input type='button' value='Fechar' onclick="window.close();">
</td>
</tr>
</table>
</body>

<?flush();
unset($_SESSION['texto']);
?>