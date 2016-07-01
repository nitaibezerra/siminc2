<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:dadosfisevotot.inc
   Finalidade: permitir a construção de relatorio gerencial acompanhamento fisico totais
*/
 
	if (! $_REQUEST['dataini']) $refcod1='01'; else $refcod1=substr($_REQUEST['dataini'],3,2);
	if (! $_REQUEST['datafim']) $refcod2='12'; else $refcod2=substr($_REQUEST['datafim'],3,2);	
     $sql = "select a.prgcod||'.'||a.acacod||'.'||a.unicod||'.'||a.loccod||' - '||a.acadsc as codigo, u.unidsc, r.regdsc, pro.prodsc, 
		case when acasnmetanaocumulativa='t' then 'Não Cumulativa' else 'Cumulativa' end as acasnmetanaocumulativa,
		unm.unmdsc, 
Coalesce(
  (case when a.acasnmetanaocumulativa='t' then 
	(select max(exp.exprealizado) 
             from execucaopto exp where exp.acaid=a.acaid 
                and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2' and refano_ref='".$_SESSION['exercicio']."')) 
        else 
            (select sum(exp.exprealizado) from execucaopto exp where exp.acaid=a.acaid 
               and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2'  and refano_ref='".$_SESSION['exercicio']."')) end),'0') as executado,
   d.fisqtdeprevistoano as meta,    
Coalesce(
  (case when a.acasnmetanaocumulativa='t' then 
	(select max(exp.exprealizado)/d.fisqtdeprevistoano/0.01  
             from execucaopto exp where exp.acaid=a.acaid 
                and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2' and refano_ref='".$_SESSION['exercicio']."')) 
        else 
            (select sum(exp.exprealizado)/d.fisqtdeprevistoano/0.01  from execucaopto exp where exp.acaid=a.acaid 
               and refcod in(select refcod from referencia where refmes_ref>='$refcod1' and refmes_ref<='$refcod2'  and refano_ref='".$_SESSION['exercicio']."')) end),'0') as percent
		from acao a
		inner join unidade u on a.unicod=u.unicod
		inner join regiao r on a.regcod=r.regcod
		inner join produto pro on a.procod=pro.procod and prodsc <> '-'
		inner join unidademedida unm on a.unmcod=unm.unmcod
		inner join dadofisico d on a.acaid=d.acaid
		where a.acasnrap='f' and a.prgano='".$_SESSION['exercicio']."' and a.acaid=$acaoid order by 1";
 

		 
/*
$sql= "select a.acaid,a.acacod, a.unicod, a.loccod, a.acadsc, a.sacdsc,p.prgcod, p.prgdsc, a.acaqtdcustototal as total, dad.fisqtdeprevistoano as previsto, u.unmdsc, pr.prodsc from acao a left join programa p on p.prgid = a.prgid left join dadofisico dad on dad.acaid=a.acaid left join unidademedida u on u.unmcod=a.unmcod left join produto pr on pr.procod=a.procod where a.acasnrap='f' and a.acasnemenda='f' and a.acaid =$acaoid ";
$RSaca= $db->record_set($sql);
unset($soma);
$_SESSION['rsfisevo'][] = $db->carrega_tudo($RSaca);
$nlinhas = $db->conta_linhas($RSaca);
unset($res);
if ($nlinhas >=0 )
{
   $res = $db->carrega_registro($RSaca,0);
   if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

   $sql4= "SELECT aca.acasnmetanaocumulativa, exe.exprealizado FROM acao aca left join avaliacaoparecer avp on aca.acaid=avp.acaid and avp.tpaid=1 and avp.avpliberada='t' left join execucaopto exe on avp.acaid=exe.acaid and avp.refcod=exe.refcod WHERE aca.acaid=$acaoid";
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
              else $soma = $exprealizado;
           }
         }
      $_SESSION['soma2']['acaid'][]=$acaoid;
      $_SESSION['soma2']['soma'][]=$soma;

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
       <? if ($previsto > 0) {?>
        <tr>
          <td align='right' class="subtitulodireita" width="20%">Realizado até o momento:</td>
          <td><?=$soma. '   (Correspondente a '.number_format(($soma/$previsto)*100,2,',','.').'%)'?></td>
        </tr>
        <?}?>

<?
// verificar o acompfisico da ação
    
     $sql = " SELECT a.acaid,a.acasnmetanaocumulativa, exe.exprealizado,avp.refcod, r.refdsc,
s.tpsdsc,to_char(avp.avpdata,'DD/MM/YYYY HH:MM') as pardata,u.usunome ||' / '|| o.orgdsc as autor FROM acao a left join avaliacaoparecer avp on a.acaid=avp.acaid and avp.tpaid=1 inner join referencia r on avp.refcod=r.refcod inner join tiposituacao s on avp.tpscod=s.tpscod inner join usuario u on u.usucpf=avp.usucpf inner join orgao o on u.orgcod=o.orgcod and avp.avpliberada='t' left join execucaopto exe on avp.acaid=exe.acaid and avp.refcod=exe.refcod WHERE a.acaid=$acaoid order by r.refano_ref,r.refmes_ref";
     unset($soma);
     $RSpta= $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RSpta);
     if ($nlinhas >= 0)
     {
            $_SESSION['rspta'][] = $db->carrega_tudo($RSpta);
     for ($n=0;$n<=$nlinhas;$n++)
     {
     	if (fmod($n,2) == 0) $marcado = 'White' ; else $marcado='';
        $res = $db->carrega_registro($RSpta,$n);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        ?>
        <tr bgcolor="<?=$marcado?>">
          <td align='right' class="subtitulodireita" width="20%">Referência:</td>
          <td><b><?=$refdsc?></b></td>
        </tr>
        <tr bgcolor="<?=$marcado?>">
          <td align='right' class="subtitulodireita" width="20%">Coordenador de Ação:</td>
          <td><?=$autor?></td>
        </tr>
        <tr bgcolor="<?=$marcado?>">
          <td align='right' class="subtitulodireita" width="20%">Lançado em:</td>
          <td><?=$pardata?></td>
        </tr>
        <tr bgcolor="<?=$marcado?>">
          <td align='right' class="subtitulodireita" width="20%">Realizado:</td>
          <td><?=$exprealizado?></td>
        </tr>
        <tr bgcolor="<?=$marcado?>">
          <td align='right' class="subtitulodireita" width="20%">Situação:</td>
          <td><?=$tpsdsc?></td>
        </tr>

      <?
    }
    }
    else
    {
      ?>
        <tr>
          <td align='right' class="subtitulodireita" width="20%">Sem lançamentos</td>
          <td>Não há lançamentos de acompanhamento físico da ação!</td>
        </tr>
      <?
    }
    ?> 
    </table>
    <?

}
*/
?>

    