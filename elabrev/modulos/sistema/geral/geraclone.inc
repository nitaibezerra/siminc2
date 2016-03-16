<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:geraclone.inc
   Finalidade: permitir a clonagem de uma ação
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['acao']=='A')
$titulo_modulo='Proposta de Clonagem de uma Ação';
else 
$titulo_modulo='Proposta de Clonagem de um Programa';
monta_titulo($titulo_modulo,'');

if ( $_REQUEST['clone'])
{
	$_SESSION['clone']=$_REQUEST['acaid'];
	$_SESSION['passa']=1;
	?>
	<script>
	    opener.location.reload();
        window.close();
	</script>
	<?

	
}

?>

<html>
<head>
<title>Proposta de Clonagem</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="clone" value="0">

<center>
    <table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
<?
if ($_REQUEST['acao']=='A')
{?>
        <td align="right" class="subtitulodireita">Escolha a ação que servirá de clone:</td>
<td colspan=2><?
	$sql= "select a.acaid as codigo, a.prgcod||'-'||a.acacod||'-'||a.acadsc as descricao from elabrev.ppaacao_proposta a where a.prsano= '".$_SESSION['exercicio']."' and a.acastatus='A' order by a.acadsc";
	$db->monta_combo('acaid',$sql,'S',"Escolha a ação que servirá de clone.",'carrega','','',400);?> 
	<?}?> 

<?
if ($_REQUEST['acao']=='P')
{?>
        <td align="right" class="subtitulodireita">Escolha o programa que servirá de clone:</td>
<td colspan=2><?
	$sql= "select p.prgid as codigo, p.prgcod||'-'||p.prgdsc as descricao from elabrev.ppaprograma_proposta p where p.prsano= '".$_SESSION['exercicio']."' and p.prgstatus='A' order by p.prgdsc";
	$db->monta_combo('prgid',$sql,'S',"Escolha o programa que servirá de clone.",'carregaprg','','',400);?> 
	<?}?>	
        </td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita">
	 <input type='button' class="botao" value='Fechar' onclick="window.close();"></td>
	 </tr>
  </table>
</form> 

<script>
 
   function carrega(cod)
  {
   //alert (cod);
   e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/acao/novaacao&acao=A&clone='+cod;
   opener.location = e;
   window.close();
  }
   function carregaprg(cod)
  {
   //alert (cod);
   e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/programa/novoprograma&acao=I&clone='+cod;
   opener.location = e;
   window.close();
  }  

        
</script>
</body>
</html>

