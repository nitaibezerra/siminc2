<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:acompsubacao.php
   
   */
include_once("config.inc");
header('Content-Type: text/html; charset=iso-8859-1');
include "includes/classes_simec.inc";
include "includes/funcoes.inc";
$db = new cls_banco();
$ptoid=$_REQUEST['ptoid'];

$sql = "select p.ptoid as ptoid2, p.ptoid_pai,p.ptotipo,p.ptocod, case when p.ptotipo='S' then 'Sub ação' when p.ptotipo='E' then 'Etapa' else 'Fase' end as tipo,p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptostatus='A' and p.acaid=".$_SESSION['acaid']." and p.ptoid_pai=".$_REQUEST['ptoid']."  group by p.ptoid,p.ptoid_pai,p.ptotipo, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma,p.ptoordem, u.unmdsc order by p.ptoordem,p.ptotipo desc,p.ptoid_pai, p.ptocod";

?>
<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#003F7E;">
  <?
	 $RSp = $db->record_set($sql);
     $nlinhasp = $db->conta_linhas($RSp);

	 if ($nlinhasp<0) {
       $ptoid3=$ptoid;
    include "modulos/principal/acao/etapafase2.inc";
  }
	 else {
       $ptoid3=$_REQUEST['ptoid'];
        include "modulos/principal/acao/etapafase2.inc";
		for ($im=0; $im<=$nlinhasp;$im++){
	  	$resp = $db->carrega_registro($RSp,$im);
	  	if(is_array($resp)) foreach($resp as $k=>$v) ${$k}=$v;
	  	$sql = "select ptoid from planotrabalho where ptostatus='A' and ptoid_pai=".$ptoid2;
        $RS2 = $db->record_set($sql);
	    $nlinhas2 = $db->conta_linhas($RS2);
        if ($nlinhas2 < 0) $filhos=0; else $filhos=1;

?>
  <?
      if ($ptotipo=='S' and $filhos==1) {?>
   <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
 	 <td colspan=7 onclick="abreconteudo('geral/acompsubacao.php?ptoid=<?=$ptoid2?>','<?=$ptoid2?>')"><img src="imagens/seta_filho.gif" width="13" height="13" alt="" border="0"><img src="imagens/mais.gif" name="+" border="0" id="img<?=$ptoid2?>">&nbsp;&nbsp;<b><?=$ptocod?>-<?=$ptodsc?></b></td>
	<td colspan=7 valign="top" align="left" style="border-top: 1px solid #cccccc; padding:2px;"> <b><?=$tipo?></b></td>
    </tr>
    <?} else {
      $ptoid3=$ptoid2;
    include "modulos/principal/acao/etapafase2.inc";
    ?>
        <?}?>
	<tr><td colspan="14" id="td<?=$ptoid2?>" style="margin-left: 10px;"></td></tr>
<?
}
?>
</table>
<? $db -> close(); exit();
}

?>




     
         
