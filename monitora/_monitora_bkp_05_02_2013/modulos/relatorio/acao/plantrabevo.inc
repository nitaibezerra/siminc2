<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:plantrabevo.inc
   Finalidade: permitir a construção de relatorio gerencial acompanhamento fisico do plano de trabalho
*/
  	if (! $_REQUEST['dataini']) $refcod1='01'; else $refcod1=substr($_REQUEST['dataini'],3,2);
	if (! $_REQUEST['datafim']) $refcod2='12'; else $refcod2=substr($_REQUEST['datafim'],3,2);
$sql= "select case when a.acasnmetanaocumulativa='f' then 'Cumulativa' else 'Não cumulativa' end as tipometa, a.acaid,a.acacod, a.unicod, a.loccod, a.acadsc, a.sacdsc,p.prgcod, p.prgdsc, a.acaqtdcustototal as total, dad.fisqtdeprevistoano as previsto, u.unmdsc, pr.prodsc from acao a left join programa p on p.prgid = a.prgid left join dadofisico dad on dad.acaid=a.acaid left join unidademedida u on u.unmcod=a.unmcod left join produto pr on pr.procod=a.procod where  a.acaid =$acaoid ";
$RSaca= $db->record_set($sql);
unset($soma);
$_SESSION['rsplantrab'][] = $db->carrega_tudo($RSaca);
$nlinhas = $db->conta_linhas($RSaca);
unset($res);
if ($nlinhas >=0 )
{
   $res = $db->carrega_registro($RSaca,0);
   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
   $soma=0;
   $maior=0;
   $sql4= "SELECT aca.acasnmetanaocumulativa, exe.exprealizado FROM acao aca left join avaliacaoparecer avp on aca.acaid=avp.acaid and avp.tpaid=1 and avp.avpliberada='t' left join execucaopto exe on avp.acaid=exe.acaid and avp.refcod=exe.refcod inner join referencia r on r.refcod=avp.refcod and r.refmes_ref >='$refcod1' and r.refmes_ref <='$refcod2' and refano_ref='".$_SESSION['exercicio']."'  WHERE aca.acaid=$acaoid";
        $RS4soma= $db->record_set($sql4);
         $nlinhas4 = $db->conta_linhas($RS4soma);
         if ($nlinhas4 >= 0)
         {
           for ($n4=0;$n4<=$nlinhas4;$n4++)
           {
              $res4 = $db->carrega_registro($RS4soma,$n4);
              if(is_array($res4)) foreach($res4 as $k=>$v) ${$k}=$v;
              if ($acasnmetanaocumulativa=='f')
              {
                $soma = $soma + $exprealizado;
              }
              else {
            if ($maior == 0) $maior = $exprealizado;
            if ($exprealizado > $maior)
            {
              	$soma = $exprealizado;
              	$maior = $exprealizado;
             }
              }
           }
         }
      $_SESSION['soma2t']['soma'][]=$soma;

    ?>
   <BR><table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>EXECUÇÃO FÍSICA DA AÇÃO</b></td></tr></table>
 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
   <tr>
     <td align='right' class="subtitulodireita" width="20%">Código do Programa:</td>
     <td><?=$prgcod?></td>
   </tr>
   <tr>
      <td align='right' class="subtitulodireita" width="20%">Título do Programa:</td>
      <td><?=$prgdsc?></td>
   </tr>
   <tr>
      <td align='right' class="subtitulodireita" width="20%">Código da Ação:</td>
      <td><?=$acacod.'.'.$unicod.'.'.$loccod?></td>
   </tr>
   <tr>
     <td align='right' class="subtitulodireita" width="20%">Título da Ação:</td>
     <td><?=$acadsc?></td>
   </tr>
   <tr>
     <td align='right' class="subtitulodireita" width="20%">Localizador da Ação:</td>
     <td><?=$sacdsc?></td>
   </tr>   
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Produto:</td>
        <td><?=$prodsc?></td>
      </tr>   
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Unidade de Medida:</td>
        <td><?=$unmdsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Meta:</td>
        <td><?=$previsto?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Tipo de Meta:</td>
        <td><?=$tipometa?></td>
      </tr>      
       <? if ($previsto > 0) {?>
        <tr>
          <td align='right' class="subtitulodireita" width="20%">Realizado no período:</td>
          <td><?=$soma. '   (Correspondente a '.number_format(($soma/$previsto)*100,2,',','.').'%)'?></td>
        </tr>
        <?}?>


<?
// plano de trabalho
?>
      <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr align='center'>
        <td colspan=2><strong style="color:red;">PLANO DE TRABALHO</strong></td>
      </tr>
<?

$sql="select acaid,taccod from acao where acaid=".$acaid;
//print $sql;
$registro=$db->recuperar($sql);
if ($registro['taccod'] == 1) {$etapafase = 'Etapas'; $et='E';} else {$etapafase='Fases';$et='F';};

$sql = "select p.acaid,p.ptoid, ptoid_pai,ptotipo,p.ptocod, case when p.ptotipo='S' then 'Subação' when p.ptotipo='E' then 'Etapa' else 'Fase' end as tipo,p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado,to_char(p.ptodata_ini,'DD/MM/YYYY') as inicio, to_char(p.ptodata_fim,'DD/MM/YYYY') as fim from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptostatus='A' and p.acaid=".$acaoid." and to_char(p.ptodata_ini,'MM')>= '$refcod1' and  to_char(p.ptodata_ini,'MM')<= '$refcod2' and to_char(p.ptodata_ini,'YYYY')= '".$_SESSION['exercicio']."' group by p.acaid,p.ptoid_pai,p.ptoid,p.ptotipo, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma,p.ptoordem, u.unmdsc,p.ptodata_ini,p.ptodata_fim order by p.ptoordem,p.ptotipo desc,p.ptoid_pai, p.ptocod";


$RSpto= $db->record_set($sql);
unset($soma);
$nlinhaspto = $db->conta_linhas($RSpto);
if ($nlinhaspto >= 0)
{
       unset($_SESSION['ptoid']);
     $_SESSION['ptoid']=array();
     $_SESSION['rspto'][] = $db->carrega_tudo($RSpto);

   for ($nm=0;$nm<=$nlinhaspto;$nm++)
   {
      $res = $db->carrega_registro($RSpto,$nm);
      $db->relatsubacao($res,$acaoid);
   }
}
  else
{
    // não existe plano de trabalho
     ?>
   <tr align='center'>
     <td colspan=2>Não existe Plano de Trabalho</td>
   </tr>
     <?
}
print '</table>';
}
?>
