<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listasubacao.php
   
   */
header('Content-Type: text/html; charset=iso-8859-1');
include "includes/classes_simec.inc";
include "includes/funcoes.inc";
$db = new cls_banco();
$ptoid=$_SESSION['ptoid'];


$sql = "select p.ptoid,ptoid_pai, p.ptotipo,p.ptocod, case when p.ptotipo='S' then 'Sub ação' when p.ptotipo='E' then 'Etapa' else 'Fase' end as tipo, ptodsc from planotrabalho p where p.ptostatus='A' and p.ptoid in (select ptoid from plantrabacao where acaid=".$_SESSION['acaid'].") and ptoid_pai=".$_REQUEST['ptoid']." order by p.ptotipo desc,p.ptoid_pai, p.ptoordem,p.ptocod";
?>
<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#003F7E;">
  <tr style="color:#000000;">
        <td width=12></td>
	  <td colspan=2 valign="top"><b>Itens subordinados</b></td>
  </tr>
    <tr>
      <td width=12></td>
      <td valign="top" class="title"><strong>Código - Descrição </strong></td>
      <td valign="top" class="title"><strong>Tipo</strong></td>
    </tr>
  <?
	 $RS = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RS);
	 if ($nlinhas<0) print "</table><font color='red'>Não foram encontrados Registros</font>";
	 else {
		for ($i=0; $i<=$nlinhas;$i++){
	  	$res = $db->carrega_registro($RS,$i);
	  	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	  	$sql = "select ptoid from planotrabalho where ptostatus='A' and ptoid_pai=".$ptoid;

        $RS2 = $db->record_set($sql);
	    $nlinhas2 = $db->conta_linhas($RS2);
        if ($nlinhas2 < 0) $filhos=0; else $filhos=1;
?>
  <?
      if ($ptotipo=='S' and $filhos==1) {?>

   <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
    <td><img border="0" src="imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=md5_encrypt($ptoid,'')?>')">&nbsp;&nbsp;&nbsp;<img border="0" src="imagens/excluir.gif" title="Excluir a atividade." onclick="excluir_pto('<?=md5_encrypt($ptoid,'')?>','<?=$ptocod?>')"></td>
 	 <td onclick="abreconteudo('geral/listasubacao.php?ptoid=<?=$ptoid?>','<?=$ptoid?>')"><img src="imagens/seta_filho.gif" width="12" height="13" alt="" border="0"><img src="imagens/mais.gif" name="+" border="0" id="img<?=$ptoid?>">&nbsp;&nbsp;<?=$ptocod?> - <?=$ptodsc?></td>
	<td valign="top" align="left" style="border-top: 1px solid #cccccc; padding:2px;"> <?=$tipo?></td>
    </tr>
    <?} else {?>
   <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
        <td><img border="0" src="imagens/alterar.gif" title="Alterar a atividade." onclick="altera_pto('<?=md5_encrypt($ptoid,'')?>')">&nbsp;&nbsp;&nbsp;<img border="0" src="imagens/excluir.gif" title="Excluir a atividade." onclick="excluir_pto('<?=md5_encrypt($ptoid,'')?>','<?=$ptocod?>')"></td>
        <td align=left><img src="imagens/seta_filho.gif" width="12" height="13" alt="" border="0"><?=$ptocod?> - <?=$ptodsc?></td>
	   <td valign="top" align="left" style="border-top: 1px solid #cccccc; padding:2px;"> <?='Tarefa'//$tipo
       ?></td>
    </tr>
        <?}?>
	<tr><td colspan="3" id="td<?=$ptoid?>" style="margin-left: 10px;"></td></tr>
<?
}
?>
</table>
<? $db -> close(); exit();
}

?>




     
         
