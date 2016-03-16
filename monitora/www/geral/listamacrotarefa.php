<?
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$ptoid=$_SESSION['ptoid'];


$sql = "select p.ptoid,ptoid_pai, p.ptotipo,p.ptocod, case when p.ptotipo='M' then 'Macro-Tarefa' when p.ptotipo='P' then 'Etapa' end as tipo, ptodsc from monitora.planotrabalho p where p.ptostatus='A' and p.ptoid in (select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid'].") and ptoid_pai=".$_REQUEST['ptoid']." order by p.ptotipo desc,p.ptoid_pai, p.ptoordem,p.ptocod";
?>
<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#003F7E;">
  <tr style="color:#000000;">
        <td width=12></td>
	  <td colspan=2 valign="top"><b>Itens subordinados</b></td>
  </tr>
    <tr>
      <td width=12></td>
      <td valign="top" class="title"><strong>Código - Descrição </strong></td>
      <td valign="top" class="title"><strong>Período</strong></td>      
      <td valign="top" class="title"><strong>Tipo</strong></td>
    </tr>
  <?
  $rs = @$db->carregar( $sql );
  if (  $rs && count($rs) > 0 )
  {
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
       	 $sql = "select ptoid from monitora.planotrabalho where ptostatus='A' and ptoid_pai=".$ptoid;
       	 $rs2 = @$db->carregar( $sql );
       	   if (  $rs2 && count($rs2) > 0 ) $filhos=1; else $filhos=0;
      if ($ptotipo=='S' and $filhos==1) {?>

   <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
    <td><img border="0" src="../imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=md5_encrypt($ptoid,'')?>')">&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir a atividade." onclick="excluir_pto('<?=md5_encrypt($ptoid,'')?>','<?=$ptocod?>')"></td>
 	 <td onclick="abreconteudo('geral/listamacrotarefa.php?ptoid=<?=$ptoid?>','<?=$ptoid?>')"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"><img src="../imagens/mais.gif" name="+" border="0" id="img<?=$ptoid?>">&nbsp;&nbsp;<?=$ptocod?> - <?=$ptodsc?></td>
 	 <td>$periodo   </td>
	<td valign="top" align="left" style="border-top: 1px solid #cccccc; padding:2px;"> <?=$tipo?></td>
    </tr>
    <?} else {?>
   <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
        <td><img border="0" src="../imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=md5_encrypt($ptoid,'')?>')">&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir a atividade." onclick="excluir_pto('<?=md5_encrypt($ptoid,'')?>','<?=$ptocod?>')"></td>
        <td align=left><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"><?=$ptocod?> - <?=$ptodsc?></td>
        <td>$periodo</td>
	   <td valign="top" align="left" style="border-top: 1px solid #cccccc; padding:2px;"> <?='Tarefa'//$tipo
       ?></td>
    </tr>
        <?}?>
	<tr><td colspan="3" id="td<?=$ptoid?>" style="margin-left: 10px;"></td></tr>
<?
  		 $i++;
		}
	}
	else print "</table><font color='red'>Não foram encontrados Registros</font>";
  
 
?>
</table>
<? $db -> close(); exit();


?>




     
         
