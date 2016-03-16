<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:acomppje.inc
   finalidade: permitir o acompanhamento físico do projeto especial
   */

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();
$ptoid=$_REQUEST['ptoid'];

function projetoaberto()
 {
 	 global $db;
 	// verifica se o projeto está aberto para ser acompanhado, ou seja, se ele não está concluido, cancelado etc.
/* 	"1","Atrasado"       "
"2","Cancelado"       "
"3","Concluído"      "
"4","Em dia"     "
"5","Não iniciado"    "
"6","Paralisado
"7","Suspenso"
"8","Sem andamento
"9","Iniciado"
*/
  $sql=" select tpscod from monitora.projetoespecial where pjeid=".$_SESSION['pjeid'];
  $sit=$db->pegaUm($sql);
  if ($sit=='1' or $sit=='4' or $sit=='9' or $sit=='6' or $sit=='8')
  // se o projeto estiver atrasasdo, ou em dia, ou iniciado, ou sem andamento ou paralisado então pode acompanhar
  return true;
  else return false;

 }

$sql = "select p.ptoid as ptoid2, p.ptoid_pai,p.ptotipo,p.ptocod, case when p.ptotipo='M' then 'Macro-Etapa' when p.ptotipo='P' then 'Etapa' end as tipo,p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado from monitora.planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join monitora.execucaopto e on p.ptoid=e.ptoid where p.ptostatus='A' and p.pjeid=".$_SESSION['pjeid']." and p.ptoid_pai=".$_REQUEST['ptoid']."  group by p.ptoid,p.ptoid_pai,p.ptotipo, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma,p.ptoordem, u.unmdsc order by p.ptoordem,p.ptotipo desc,p.ptoid_pai, p.ptocod";

?>
<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#003F7E;">
  <?
	 $RSp = $db->record_set($sql);
     $nlinhasp = $db->conta_linhas($RSp);

	 if ($nlinhasp<0) {
       $ptoid3=$ptoid;
    include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/projespec/etapamacro_etapa2.inc";

  }
	 else {
       $ptoid3=$_REQUEST['ptoid'];
        include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/projespec/etapamacro_etapa2.inc";
		for ($im=0; $im<=$nlinhasp;$im++){
	  	$resp = $db->carrega_registro($RSp,$im);
	  	if(is_array($resp)) foreach($resp as $k=>$v) ${$k}=$v;
	  	$sql = "select ptoid from monitora.planotrabalho where ptostatus='A' and ptoid_pai=".$ptoid2;
        $RS2 = $db->record_set($sql);
	    $nlinhas2 = $db->conta_linhas($RS2);
        if ($nlinhas2 < 0) $filhos=0; else $filhos=1;

?>
  <?
      if ($ptotipo=='S' and $filhos==1) {?>
   <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
 	 <td colspan=7 onclick="abreconteudo('geral/acomppje.php?ptoid=<?=$ptoid2?>','<?=$ptoid2?>')"><img src="../imagens/seta_filho.gif" width="13" height="13" alt="" border="0"><img src="../imagens/mais.gif" name="+" border="0" id="img<?=$ptoid2?>">&nbsp;&nbsp;<b><?=$ptocod?>-<?=$ptodsc?></b></td>
	<td colspan=7 valign="top" align="left" style="border-top: 1px solid #cccccc; padding:2px;"> <b><?=$tipo?></b></td>

    </tr>
    <?} else {
      $ptoid3=$ptoid2;
    include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/projespec/etapamacro_etapa2.inc";
    ?>
        <?}?>
	<tr><td colspan="14" id="td<?=$ptoid2?>" style="margin-left: 10px;"></td>

<?
}

?>
			 <script type="text/javascript">
		 	abreconteudo('geral/acomppje.php?ptoid=<?=$ptoid2?>','<?=$ptoid2?>');
		 </script></tr>
</table>
<? $db -> close(); exit();
}

?>




     
         
