<?
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
if ($_REQUEST['recebe']=='recebe')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo.inc";

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
<title>Envio de Email</title>
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
    <tr><td><font face="Verdana" size="3"></font>O recebimento de dados do SIGPLAN deverá ocorrer uma vez ao ano, dando início a um exercício.<p>Ao pressionar o botão "Carga da LOA", o sistema irá ler o conjunto de arquivos no formato XML e fazer uma migração seletiva de dados, incluindo os novos e alterando, se for o caso, os antigos.</p>
</td>
	 </tr>
	 <tr>
	 <td ><input type='button' class="botao" value='Carga da LOA' onclick="recebe_dados('1')"></td>
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
        <td align='left' class="SubTituloDireita">Ano origem / Ano Destino:</td>
        <td><?=campo_texto('anoorigem','N','S','',6,4,'####','');?>  /  <?=campo_texto('anodestino','S','S','',6,4,'####','');?></td>
      </tr>	
      <tr>
        <td align='left' class="SubTituloDireita">O que será migrado:</td>
        <td><input type='checkbox' name='resp' value='r' <?if ($_REQUEST['resp']) print '   checked'?>>Responsabilidades em Programas e Ações<br>
        <input type='checkbox' name='plantrab' value='p' <?if ($_REQUEST['plantrab']) print '   checked'?>>Plano de Trabalho<br>
        <input type='checkbox' name='projesp' value='j' <?if ($_REQUEST['projesp']) print '   checked'?>>Projeto Especial
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
if (!validaBranco(document.formulario.anodestino, 'Ano de destino')) return ;
  if( window.confirm( "Este processo pode levar vários minutos. \n Confirma a importação de dados do SIGPLAN ?") )
    {
      if (cod=='1'){
  document.formulario.recebe.value='recebe';
    document.formulario.submit();}
  }

}


function migra_dados()
{
	if (!validaBranco(document.formulario.anoorigem, 'Ano de origem')) return ;
	if (!validaBranco(document.formulario.anodestino, 'Ano de destino')) return ;
	if (document.formulario.anoorigem.value >= document.formulario.anodestino.value)
		{
		  alert ("Ano de destino tem que ser posterior ao de origem!");
		  document.formulario.anoorigem.focus();
		  return;
		}
	if (! document.formulario.plantrab.checked && ! document.formulario.resp.checked && ! document.formulario.projesp.checked ) {alert ('Escolher pelo menos uma ação de migração!'); return;}
		document.formulario.recebe.value='migra';
        document.formulario.submit();	
}


</script>
</body>
</html>
