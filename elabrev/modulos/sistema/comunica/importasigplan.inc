<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importasigplan.inc
   Finalidade: permitir importar os dados do SIGPLAN
   */
$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";

if ($_REQUEST['recebe']=='recebe1')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo.inc";

}
if ($_REQUEST['recebe']=='recebe2')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase2.inc";

}
if ($_REQUEST['recebe']=='recebe3')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase3.inc";

  }
if ($_REQUEST['recebe']=='recebe4')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase4.inc";
}  

if ($_REQUEST['recebe']=='recebe5')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_sidor.inc";

}
if ($_REQUEST['recebe']=='recebe6')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase2_sidor.inc";

}
if ($_REQUEST['recebe']=='recebe7')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase3_sidor.inc";

  }
if ($_REQUEST['recebe']=='recebe9')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importaacao_sidor.inc";
} 

if ($_REQUEST['recebe']=='recebe8')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase4_sidor.inc";
} 

if ($_REQUEST['recebe']=='migra')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/migradados.inc";
}
?>
<br>
<div class="notprint">
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Importação de Dados do SIGPLAN';
monta_titulo($titulo_modulo,'');
$anoorigem=$_REQUEST['anoorigem'];
$anodestino=$_REQUEST['anodestino'];

?>
<html>
<head>
<title>Importação da Dados do SIGPLAN</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="recebe" >
    <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	 <tr>
	 <td ><HR> </td>
    <tr align="center"><td><font face="Verdana" size="3"></font><b>INÍCIO DE EXERCÍCIO</td>
	 </tr>
	 <tr>
	 <td ><HR> </td></TR>
     
    <tr><td><font face="Verdana" size="3"></font>O recebimento de dados do SIGPLAN deverá ocorrer uma vez ao ano, dando início a um exercício.<p>Ao pressionar o botão "Receber dados", o sistema irá ler o conjunto de arquivos no formato XML e fazer uma migração seletiva de dados, incluindo os novos e alterando, se for o caso, os antigos.</p>
</td>
	 </tr>
	 <tr>
	 <td ><input type='button' class="botao" value='Receber todos (exceto ações, financeiro e físico) ' onclick="recebe_dados('1')">
<input type='button' class="botao" value='Receber Ações' onclick="recebe_dados('4')">	 
	 <input type='button' class="botao" value='Receber dados Financeiros' onclick="recebe_dados('2')">
	 <input type='button' class="botao" value='Receber dados Físicos' onclick="recebe_dados('3')">
    </td>
	 </tr>
	 <td ><HR> </td>
    <tr align="center"><td><font face="Verdana" size="3"></font><b>RETORNO DO SIGPLAN APÓS FASE QUALITATIVA</td>
	 </tr>
	 <tr>
	 <td ><HR> </td></TR>	 

	 <tr>
	 <td ><input type='button' class="botao" value='Receber todos (exceto ações, financeiro e físico) ' onclick="recebe_dados('5')">
<input type='button' class="botao" value='Receber Ações' onclick="recebe_dados('8')">	 
	 <input type='button' class="botao" value='Receber dados Financeiros' onclick="recebe_dados('6')">
	 <input type='button' class="botao" value='Receber dados Físicos' onclick="recebe_dados('7')">
	 <input type='button' class="botao" value='Receber Ações SIDOR' onclick="recebe_dados('9')">
    </td>
	 </tr>
	 <tr>
	 <td ><HR> </td>	 
	 </tr>
	 <tr>
	 <td ><B>MIGRAÇÃO DE DADOS </b></td>
	 </tr>
</table>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='left' class="SubTituloDireita">Exercício base de comparação:</td>
        <td><?=campo_texto('anoorigem','N','S','',6,4,'####','');?>  </td>
      </tr>	
      <tr>
        <td align='left' class="SubTituloDireita">O que será migrado:</td>
        <td><input type='checkbox' name='migrap' value='r' <?if ($_REQUEST['resp']) print '   checked'?>>Programas e Ações<br>

        
        </td>
      </tr>	 
      <tr>
        <td align='center' colspan="2" >
        <input type='button' class="botao" value='Executar Migração' onclick="migra_dados()"></td>
      </tr>	


  </table>
</form> 
<script>
function recebe_dados(cod)
{
  if( window.confirm( "Este processo pode levar vários minutos. \n Confirma a importação de dados do SIGPLAN ?") )
    {
      document.formulario.recebe.value='recebe'+cod;
      document.formulario.submit();
    } 
}


function migra_dados()
{
	if (!validaBranco(document.formulario.anoorigem, 'Exercício base de comparação')) return ;

	if (! document.formulario.migrap.checked ) {alert ('Escolher a ação de migração!'); return;}
		document.formulario.recebe.value='migra';
        document.formulario.submit();	
}


</script>
</body>
</html>
