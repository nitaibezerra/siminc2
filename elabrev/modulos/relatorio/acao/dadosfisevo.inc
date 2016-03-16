<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:dadosfisevo.inc
   Finalidade: permitir a construção de relatorio gerencial acompanhamento fisico
*/
 	if (! $_REQUEST['dataini']) $refcod1='01'; else $refcod1=substr($_REQUEST['dataini'],3,2);
	if (! $_REQUEST['datafim']) $refcod2='12'; else $refcod2=substr($_REQUEST['datafim'],3,2);
$sql= "select case when a.acasnmetanaocumulativa='f' then 'Cumulativa' else 'Não cumulativa' end as tipometa, a.acaid,a.acacod, a.unicod, a.loccod, a.acadsc, a.sacdsc,p.prgcod, p.prgdsc, a.acaqtdcustototal as total, dad.fisqtdeprevistoano as previsto, u.unmdsc, pr.prodsc from acao a left join programa p on p.prgid = a.prgid left join dadofisico dad on dad.acaid=a.acaid left join unidademedida u on u.unmcod=a.unmcod left join produto pr on pr.procod=a.procod where  a.acaid =$acaoid ";
$RSaca= $db->record_set($sql);
unset($soma);
$_SESSION['rsfisevo'][] = $db->carrega_tudo($RSaca);
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
          else 
          {
            if ($maior == 0) $maior = $exprealizado;
            if ($exprealizado > $maior)
            {
              	$soma = $exprealizado;
              	$maior = $exprealizado;
             }              	
            }
          }                          
      }
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
// verificar o acompfisico da ação
    
     $sql = " SELECT a.acaid,a.acasnmetanaocumulativa, exe.exprealizado,avp.refcod, r.refdsc,
s.tpsdsc,to_char(avp.avpdata,'DD/MM/YYYY HH:MM') as pardata,u.usunome ||' / '|| o.orgdsc as autor FROM acao a left join avaliacaoparecer avp on a.acaid=avp.acaid and avp.tpaid=1 inner join referencia r on avp.refcod=r.refcod and r.refmes_ref >='$refcod1' and r.refmes_ref <='$refcod2' and refano_ref='".$_SESSION['exercicio']."' inner join tiposituacao s on avp.tpscod=s.tpscod inner join usuario u on u.usucpf=avp.usucpf inner join orgao o on u.orgcod=o.orgcod and avp.avpliberada='t' left join execucaopto exe on avp.acaid=exe.acaid and avp.refcod=exe.refcod WHERE a.acaid=$acaoid order by r.refano_ref,r.refmes_ref";
     
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
?>

    