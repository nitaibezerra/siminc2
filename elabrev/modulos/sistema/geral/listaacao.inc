<?
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
Header("Cache-control: private, no-cache");
header("Pragma: no-cache");
include "includes/classes_simec.inc";
include "includes/funcoes.inc";
$db = new cls_banco();

//Lista Ações
if ($_REQUEST['prgid'] and !$_REQUEST['acacod'])
  {$prgid = $_REQUEST['prgid'];
  ?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#003F7E;">
<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top">Ações:</td>
	  <td valign="top" align="right">Qtd:</td>
    </tr>
  <?
     $sql= "select acacod as codigo, acadsc as descricao, count(*) as total from acao  where prgano = '".$_SESSION['exercicio']."' and prgid =".$_REQUEST['prgid']." and acastatus='A' group by acacod, acadsc";
	 $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
	if ($nlinhas<0) print "</table><font color='red'>Não foram encontrados Registros</font>";
	else {
		for ($i=0; $i<=$nlinhas;$i++){
	  	$res = $db->carrega_registro($RS,$i);
	  	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
      <td valign="top" width="12" style="padding:2px;"><img src="imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" style="border-top: 1px solid #cccccc; padding:2px;" onclick="abreconteudo('geral/listaacao.php?acacod=<?=$codigo?>&prgid=<?=$prgid?>','<?=$prgid.$codigo?>')"><img src="imagens/mais.gif" width="9" height="9" alt="" border="0" name="+" id="img<?=$prgid.$codigo?>"> <?=$codigo?> - <?=$descricao?></td>
	<td valign="top" align="right" style="border-top: 1px solid #cccccc; padding:2px;"> (<?=$total?>)</td>
    </tr>
	<tr><td></td><td colspan="2" id="td<?=$prgid.$codigo?>" style="margin-left: 10px;"></td></tr>
<?
}
?>
</table>
<? $db -> close(); exit();
}
}
?>


<?
//Lista detalhamento das Ações
if ($_REQUEST['acacod'] and $_REQUEST['prgid'])
  {
  $acacod = $_REQUEST['acacod'];
  $prgid = $_REQUEST['prgid'];
  ?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top">Código</td>
	  <td valign="top">Unidade:</td>
	  <td valign="top">Localizador:</td>
    </tr>
  <?
     $sql = "select distinct a.prgid, a.acaid, a.loccod, a.acacod as codigo, a.unicod, a.sacdsc, b.unidsc from acao a left join unidade b on a.unicod = b.unicod where a.prgano = '".$_SESSION['exercicio']."' and a.acastatus='A' and a.acacod ='".$acacod."' and a.prgid=".$prgid;
	 //$sql= "select * from acao where acacod='".$acacod."'";
	 $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas<0) print "</table><font color='red'>Não foram encontrados Registros</font>";
	else {
			for ($i=0; $i<=$nlinhas;$i++){
			$res = $db->carrega_registro($RS,$i);
			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
      <td valign="top" width="12" style="padding:2px;"><img src="imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" style="border-top: 1px solid #cccccc; padding:2px;"><a href="simec.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$acaid?>&prgid=<?=$prgid?>"><?=$acacod?>-<?=$unicod?>-<?=$loccod?></a></td>
	  <td valign="top" style="border-top: 1px solid #cccccc; padding:2px;"><?=$unicod?> - <?=$unidsc?></td>
	  <td valign="top" style="border-top: 1px solid #cccccc; padding:2px;"><?=$sacdsc?></td>
    </tr>
<?
}
?>
</table>
<?$db -> close(); exit();
}
}
?>



     
         
