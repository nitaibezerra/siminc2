<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listaprograma.php
   
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');

include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$prgidl = str_replace('|',chr(39),$_REQUEST['prgid']);
//filtros de tipo de ação
$wh = "";

//Lista programas

if ($_REQUEST['prgid'] )//and !$_REQUEST['codigo'] and !$prgid)
  {

  	
   	$tit1 = 'Programas:';
		$sql= "select p.prgcod as codigo, prgdsc as descricao, p.prgid from elabrev.ppaprograma_proposta p  where prsano = '".$_SESSION['exercicio']."'  and prgid in ('".$prgidl."') and prgstatus in ('A','N') ".$wh." and p.orgcod='".$_SESSION['ittorgao']."' group by prgcod, prgdsc,prgid order by p.prgcod";
  ?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#003F7E;">
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
      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" style="border-top: 1px solid #cccccc; padding:2px;" onclick="abreconteudo('geral/listaprograma.php?codigo=<?=$codigo?>&prgid=<?=$prgid?>)">
	  <a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/programa/cadprograma&acao=C&prgid=<?=$prgid?>"><?=$codigo?> - <?=$descricao?></a> </td>
    </tr>

<?
}
?>
</table>
<? $db -> close(); exit();
}
}
?>




     
         
