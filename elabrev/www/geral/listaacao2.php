<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listaacao2.php
   
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');

include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$ordem = $_REQUEST['ordem'];
$acaid = str_replace('|',chr(39),$_REQUEST['acaid']);
$tipoacao1 = $_REQUEST['tipoacao1'];
$tipoacao2 = $_REQUEST['tipoacao2'];
$tipoacao3 = $_REQUEST['tipoacao3'];

//filtros de tipo de ação
if ($tipoacao1 and $tipoacao2 and !$tipoacao3) $wh = " and a.acasnemenda='f' ";
elseif ($tipoacao1 and !$tipoacao2 and $tipoacao3) $wh = " and a.acasnrap='f'";
elseif (!$tipoacao1 and $tipoacao2 and $tipoacao3) $wh = " and a.acasnrap='t' and a.acasnemenda='t' ";
elseif (!$tipoacao1 and $tipoacao2 and !$tipoacao3) $wh = " and a.acasnrap='t' ";
elseif (!$tipoacao1 and !$tipoacao2 and $tipoacao3) $wh = " and a.acasnemenda='t' ";
else $wh = "";

//Lista Ações
if ($_REQUEST['prgid'] and !$_REQUEST['codigo'] and !$acaid)
  {
  	$prgid = $_REQUEST['prgid'];
   	if ($ordem == 'L') 
	{
		$tit1 = 'Região:';
		 $sql = "select a.regcod as codigo, regdsc as descricao, count(*) as total from elabrev.ppaacao_proposta a left join regiao r on a.regcod=r.regcod where prsano = '".$_SESSION['exercicio']."'  and prgid in ('".$_REQUEST['prgid']."') and acastatus='A' ".$wh." group by a.regcod, regdsc, a.prgcod order by a.regcod";
	} 
	elseif ($ordem == 'U') 
	{	
		$tit1 = 'Unidades:';
		$sql= "select a.unicod as codigo, b.unidsc as descricao, count(*) as total from elabrev.ppaacao_proposta a left join unidade b on a.unicod = b.unicod  where prsano = '".$_SESSION['exercicio']."'  and prgid in ('".$_REQUEST['prgid']."') and acastatus='A' ".$wh." group by a.unicod, b.unidsc order by a.unicod";
	} 
	else
	{
		$tit1 = 'Ações:';
		$sql= "select a.prgcod||'.'||a.acacod as codigo, acadsc as descricao, count(*) as total , acacod from elabrev.ppaacao_proposta a  where prsano = '".$_SESSION['exercicio']."'  and prgid in ('".$_REQUEST['prgid']."') and acastatus='A' ".$wh." group by prgcod||'.'||acacod, acadsc, acacod order by a.acacod";
	}

  ?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#003F7E;">
<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top"><?=$tit1?></td>
	  <td valign="top" align="right">Qtd:</td>
    </tr>
  <?
	 $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
	if ($nlinhas<0) print "</table><font color='red'>Não foram encontrados Registros</font>";
	else {
		for ($i=0; $i<=$nlinhas;$i++){
	  	$res = $db->carrega_registro($RS,$i);
	  	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" style="border-top: 1px solid #cccccc; padding:2px;" onclick="abreconteudo('geral/listaacao2.php?codigo=<?=$codigo?>&prgid=<?=$prgid?>&ordem=<?=$ordem?>&tipoacao=<?=$tipoacao?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>','<?=$prgid.$codigo?>')"><img src="../imagens/mais.gif" width="9" height="9" alt="" border="0" name="+" id="img<?=$prgid.$codigo?>"> <?=$codigo?> - <?=$descricao?></td>
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
if ($_REQUEST['codigo'] or $acaid)
  {

  $codigo = $_REQUEST['codigo'];
  $prgid = $_REQUEST['prgid'];
  if ($prgid<>'') $wh .= " and a.prgid in('".$prgid."')";
  	if ($acaid)
	{
	$wh .= " and a.acaid in('".$acaid."')";
	//$tit1 = 'Código:'; $tit2 = 'Ação:'; //$tit3 = 'Responsabilidade';
	$tit1 = 'Ação:'; //$tit2 = 'Ação:'; //$tit3 = 'Responsabilidade';	
	$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.acacod, a.acacod as cod1,   a.acadsc as desc1  from elabrev.ppaacao_proposta a where a.prsano = '".$_SESSION['exercicio']."'  and a.acastatus in ('A','N') ".$wh." order by a.acacod";
	}
	else
	{
	//$tit1 = 'Código:'; $tit2 = 'Ação:'; //$tit3 = 'Responsabilidade';
	$tit1 = 'Ação:'; //$tit2 = 'Ação:'; //$tit3 = 'Responsabilidade';
		$sql = "select distinct a.prgid, a.prgcod, a.acaid, a.loccod, a.acacod, a.unicod, a.sacdsc, b.unidsc, a.unicod as cod1, a.loccod as cod2,  b.unidsc as desc1, a.sacdsc as desc2 from elabrev.ppaacao_proposta a left join unidade b on a.unicod = b.unicod where a.prsano = '".$_SESSION['exercicio']."'  and a.acastatus in ('A','N') and a.prgcod||'.'||a.acacod ='".$codigo."' ".$wh." order by a.acacod, a.unicod, a.loccod";
	} 

  ?>
  
<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top"><?=$tit1?></td>
	  
    </tr>
  <?
	 $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas<0) print "</table><font color='red'>Não foram encontrados Registros</font>";
	else {
			for ($i=0; $i<=$nlinhas;$i++){
			$res = $db->carrega_registro($RS,$i);
			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
      <td >
      <img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td >
	  <a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/acao/cadacao&acao=C&acaid=<?=$acaid?>&prgid=<?=$prgid?>"><?=$prgcod.'-'.$acacod.'  '.$desc1?></a></td>
 
    </tr>
<?
}
?>
<?if ($tit2){?><tr><td align="right" style="color:000000;border-top: 2px solid #000000;">Total de Ações: (<?=$nlinhas+1?>)</td></tr><?}?>
</table>
<?$db -> close(); exit();
}
}
?>



     
         
