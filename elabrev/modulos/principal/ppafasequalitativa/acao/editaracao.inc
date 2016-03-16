<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadsubacao.inc
   Finalidade: permitir o controle do cadastro de sub acao
   */
$modulo=$_REQUEST['modulo'] ;//

$sql="select acaid from acao where acasnrap='t' and acaid=".$_SESSION['acaid'];
$registro=$db->recuperar($sql);
if (is_array($registro)) {
?>
	<script>alert('Esta ação é de Restos a Pagar e não é editada!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

if ($_REQUEST['act']== 'alterar')
{
       // atualização dos atributos de ação

       $sql = "update acao set acasnmetanaocumulativa='".$_REQUEST['acasnmetanaocumulativa']."' where acaid=".$_SESSION['acaid'];
       $saida = $db->executar($sql);
        $sql= "select * from acao where acaid=".$_SESSION['acaid'];
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;

  if ( !  $_REQUEST['fisqtdecroninicial1'] ) $_REQUEST['fisqtdecroninicial1']='null';
  if ( !  $_REQUEST['fisqtdecroninicial2'] ) $_REQUEST['fisqtdecroninicial2']='null';
  if ( !  $_REQUEST['fisqtdecroninicial3'] )  $_REQUEST['fisqtdecroninicial3']='null';
  if ( !  $_REQUEST['fisqtdecroninicial4'] )  $_REQUEST['fisqtdecroninicial4']='null';
  if ( !  $_REQUEST['fisqtdecroninicial5'] ) $_REQUEST['fisqtdecroninicial5']='null';
  if ( !  $_REQUEST['fisqtdecroninicial6'] ) $_REQUEST['fisqtdecroninicial6']='null';
  if ( !  $_REQUEST['fisqtdecroninicial7'] ) $_REQUEST['fisqtdecroninicial7']='null';
  if ( !  $_REQUEST['fisqtdecroninicial8'] ) $_REQUEST['fisqtdecroninicial8']='null';
  if ( !  $_REQUEST['fisqtdecroninicial9'] ) $_REQUEST['fisqtdecroninicial9']='null';
  if ( !  $_REQUEST['fisqtdecroninicial10'] ) $_REQUEST['fisqtdecroninicial10']='null';
  if ( !  $_REQUEST['fisqtdecroninicial11'] ) $_REQUEST['fisqtdecroninicial11']='null';
  if ( !  $_REQUEST['fisqtdecroninicial12'] ) $_REQUEST['fisqtdecroninicial12']='null';
  if ( !  $_REQUEST['fisqtdecronogramado1'] ) $_REQUEST['fisqtdecronogramado1']='null';
  if ( !  $_REQUEST['fisqtdecronogramado2'] ) $_REQUEST['fisqtdecronogramado2']='null';
  if ( !  $_REQUEST['fisqtdecronogramado3'] ) $_REQUEST['fisqtdecronogramado3']='null';
  if ( !  $_REQUEST['fisqtdecronogramado4'] ) $_REQUEST['fisqtdecronogramado4']='null';
  if ( !  $_REQUEST['fisqtdecronogramado5'] ) $_REQUEST['fisqtdecronogramado5']='null';
  if ( !  $_REQUEST['fisqtdecronogramado6'] ) $_REQUEST['fisqtdecronogramado6']='null';
  if ( !  $_REQUEST['fisqtdecronogramado7'] ) $_REQUEST['fisqtdecronogramado7']='null';
  if ( !  $_REQUEST['fisqtdecronogramado8'] ) $_REQUEST['fisqtdecronogramado8']='null';
  if ( !  $_REQUEST['fisqtdecronogramado9'] )  $_REQUEST['fisqtdecronogramado9']='null';
  if ( !  $_REQUEST['fisqtdecronogramado10'] ) $_REQUEST['fisqtdecronogramado10']='null';
  if ( !  $_REQUEST['fisqtdecronogramado11'] ) $_REQUEST['fisqtdecronogramado11']='null';
  if ( !  $_REQUEST['fisqtdecronogramado12'] ) $_REQUEST['fisqtdecronogramado12']='null';

  if ( !  $_REQUEST['fisqtdeprevistoano'] ) $_REQUEST['fisqtdeprevistoano'] ='null';
  if ( !  $_REQUEST['fisqtdecroninicialano'] ) $_REQUEST['fisqtdecroninicialano'] ='null';
  if ( !  $_REQUEST['fisqtdeatualano'] )  $_REQUEST['fisqtdeatualano'] ='null';
  if ( !  $_REQUEST['fisqtdecronogramadoano'] )  $_REQUEST['fisqtdecronogramadoano'] ='null';
  if ( !  $_REQUEST['fisqtderealizadoano'] )  $_REQUEST['fisqtderealizadoano'] ='null';

  
     $sql = "select acacod from dadofisico  where acaid=".$_SESSION['acaid'];
     //acacod' and  prgcod='$prgcod' and prgano='$prgano' and regcod='$regcod' and saccod='$saccod'";
      $saida = $db->recuperar($sql,$res);
       if(is_array($saida)){
  
       // atualização dos atributos do dadofisico
     //  $sql = "update dadofisico set fisqtdecroninicial1 = ". $_REQUEST['fisqtdecroninicial1'].",fisqtdecroninicial2 = ". $_REQUEST['fisqtdecroninicial2'].",fisqtdecroninicial3 = ". $_REQUEST['fisqtdecroninicial3'].",fisqtdecroninicial4 = ". $_REQUEST['fisqtdecroninicial4'].",fisqtdecroninicial5 = ". $_REQUEST['fisqtdecroninicial5'].",fisqtdecroninicial6 = ". $_REQUEST['fisqtdecroninicial6'].",fisqtdecroninicial7 = ". $_REQUEST['fisqtdecroninicial7'].",fisqtdecroninicial8 = ". $_REQUEST['fisqtdecroninicial8'].",fisqtdecroninicial9 = ". $_REQUEST['fisqtdecroninicial9'].",fisqtdecroninicial10= ". $_REQUEST['fisqtdecroninicial10'].",fisqtdecroninicial11= ". $_REQUEST['fisqtdecroninicial11'].",fisqtdecroninicial12= ". $_REQUEST['fisqtdecroninicial12'].",fisqtdecronogramado1 = ". $_REQUEST['fisqtdecronogramado1'].",fisqtdecronogramado2 = ". $_REQUEST['fisqtdecronogramado2'].",fisqtdecronogramado3 = ". $_REQUEST['fisqtdecronogramado3'].",fisqtdecronogramado4 = ". $_REQUEST['fisqtdecronogramado4'].",fisqtdecronogramado5=". $_REQUEST['fisqtdecronogramado5'].",fisqtdecronogramado6=". $_REQUEST['fisqtdecronogramado6'].",fisqtdecronogramado7 = ". $_REQUEST['fisqtdecronogramado7'].",fisqtdecronogramado8 = ". $_REQUEST['fisqtdecronogramado8'].",fisqtdecronogramado9=". $_REQUEST['fisqtdecronogramado9'].",fisqtdecronogramado10= ". $_REQUEST['fisqtdecronogramado10'].",fisqtdecronogramado11= ". $_REQUEST['fisqtdecronogramado11'].",fisqtdecronogramado12=". $_REQUEST['fisqtdecronogramado12']." where acacod='$acacod' and  prgcod='$prgcod' and prgano='$prgano' and regcod='$regcod' and saccod='$saccod'";

        $sql = "update dadofisico set fisqtdeprevistoano = ". $_REQUEST['fisqtdeprevistoano']." where acaid=".$_SESSION['acaid'];
        //$acacod' and  prgcod='$prgcod' and prgano='$prgano' and regcod='$regcod' and saccod='$saccod'";
       
   }
   else
   {
     // $sql = "insert into dadofisico (fisqtdecroninicial1,fisqtdecroninicial2,fisqtdecroninicial3,fisqtdecroninicial4 ,fisqtdecroninicial5,fisqtdecroninicial6,fisqtdecroninicial7,fisqtdecroninicial8,fisqtdecroninicial9,fisqtdecroninicial10,fisqtdecroninicial11,fisqtdecroninicial12,fisqtdecronogramado1,fisqtdecronogramado2,fisqtdecronogramado3,fisqtdecronogramado4,fisqtdecronogramado5,fisqtdecronogramado6,fisqtdecronogramado7,fisqtdecronogramado8,fisqtdecronogramado9,fisqtdecronogramado10,fisqtdecronogramado11,fisqtdecronogramado12,fisqtdeprevistoano,fisqtdecroninicialano,fisqtdeatualano,fisqtdecronogramadoano,fisqtderealizadoano,acacod,prgcod,prgano,regcod,saccod,acaid, loccod) values (".$_REQUEST['fisqtdecroninicial1'].",".$_REQUEST['fisqtdecroninicial2'].",".$_REQUEST['fisqtdecroninicial3'].",".$_REQUEST['fisqtdecroninicial4'].",".$_REQUEST['fisqtdecroninicial5'].",".$_REQUEST['fisqtdecroninicial6'].",".$_REQUEST['fisqtdecroninicial7'].",".$_REQUEST['fisqtdecroninicial8'].",".$_REQUEST['fisqtdecroninicial9'].",".$_REQUEST['fisqtdecroninicial10'].",".$_REQUEST['fisqtdecroninicial11'].",".$_REQUEST['fisqtdecroninicial12'].",".$_REQUEST['fisqtdecronogramado1'].",".$_REQUEST['fisqtdecronogramado2'].",".$_REQUEST['fisqtdecronogramado3'].",".$_REQUEST['fisqtdecronogramado4'].",".$_REQUEST['fisqtdecronogramado5'].",".$_REQUEST['fisqtdecronogramado6'].",".$_REQUEST['fisqtdecronogramado7'].",".$_REQUEST['fisqtdecronogramado8'].",".$_REQUEST['fisqtdecronogramado9'].",".$_REQUEST['fisqtdecronogramado10'].",".$_REQUEST['fisqtdecronogramado11'].",".$_REQUEST['fisqtdecronogramado12'].",".$_REQUEST['fisqtdeprevistoano'].",".$_REQUEST['fisqtdecroninicialano'].",".$_REQUEST['fisqtdeatualano'].",".$_REQUEST['fisqtdecronogramadoano'].",".$_REQUEST['fisqtderealizadoano'].",'$acacod','$prgcod','$prgano','$regcod','$saccod',$acaid,'$loccod')";

        $sql = "insert into dadofisico (fisqtdeprevistoano,acacod,prgcod,prgano,regcod,saccod,acaid,loccod) values (".$_REQUEST['fisqtdeprevistoano'].",'$acacod','$prgcod','$prgano','$regcod','$saccod',$acaid,'$loccod')";
   }
      $saida = $db->executar($sql);
      $acaid = $_SESSION['acaid'];
   // atualização da tabela execucaopto, campo expplanejado
   /*for ($i=1; $i<13;$i++)
   {
     $sql = 'select e.expid from execucaopto e, referencia r where  execucaopto.acaid='.$_SESSION['acaid'].' and referencia.refcod = execucaopto.refcod and referencia.refmes_ref::integer = '.$i;
      $saida = $db->recuperar($sql,$res);
       if(is_array($saida)){
             $sql = 'update execucaopto set expplanejado ='.$_REQUEST['fisqtdecronogramado'.$i].' where execucaopto.acaid='.$_SESSION['acaid'].' and referencia.refcod = execucaopto.refcod and referencia.refmes_ref::integer = '.$i;
       } else
       {
          unset($refcod);
          $sql2 = 'select r.refcod from referencia r where r.refmes_ref::integer = '.$i;
           $saida = $db->recuperar($sql2,$res);
          if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
          if ($refcod){
             $sql = "insert into execucaopto (exprealizado,expobs,acaid,refcod,usucpf,tpscod,expplanejado) values (0,'',$acaid,$refcod,'".$_SESSION['usucpf']."',8,".$_REQUEST['fisqtdecronogramado'.$i].")";
             }
       }
             $saida = $db->executar($sql);
       //print $sql.'<br>';
      }
    $db->commit();
*/
	//$db->sucesso($modulo);

	?>
	<script>
    	//alert ("Alteração realizada com sucesso!");
    	window.close();
 	      opener.location.reload();

 </script>
	<?
	exit();
}

if($_SESSION['acaid'] ) {
        $sql= "select * from acao where acaid=".$_SESSION['acaid'];
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
        $sql= "select fisqtdeprevistoano,fisqtdecroninicialano,fisqtdecroninicial1,fisqtdecroninicial2,fisqtdecroninicial3,fisqtdecroninicial4,fisqtdecroninicial5,fisqtdecroninicial6,fisqtdecroninicial7,fisqtdecroninicial8,fisqtdecroninicial9,fisqtdecroninicial11,fisqtdecroninicial10,fisqtdecroninicial12, fisqtdecronogramado1,fisqtdecronogramado2,fisqtdecronogramado3,fisqtdecronogramado4,fisqtdecronogramado5,fisqtdecronogramado6,fisqtdecronogramado7,fisqtdecronogramado8,fisqtdecronogramado9,fisqtdecronogramado10,fisqtdecronogramado11,fisqtdecronogramado12,fisdsccomentexecucao from dadofisico where acacod='$acacod' and prgcod='$prgcod' and prgano = '$prgano' and saccod='$saccod' and regcod='$regcod'";
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
}


?>

<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='includes/listagem.css'>
<script language="JavaScript" src="includes/funcoes.js"></script>

<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<table width='100%' class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td colspan=12 align='right' class="SubTituloEsquerda"><h2>Editar Ação</h2></td>
</tr>


<?
if ($acaid)
{?>
      <tr>
        <td align='right' class="SubTituloDireita">Código:</td>
	    <td colspan=11><?=$acacod.'.'.$unicod;?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
	    <td colspan=11><?=$acadsc;?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Meta:</td>
	    <td colspan=11><?=campo_texto('fisqtdeprevistoano','N','S','',5,15,'###############','');?></td>
      </tr>
     <!-- <tr>
        <td align='right' class="SubTituloDireita">Planejamento Inicial:</td>
	    <td colspan=11></td>
      </tr> -->
      <tr>
        <td align='right' class="SubTituloDireita">Meta cumulativa?</td>
	    <td colspan=11><input type="radio" name="acasnmetanaocumulativa" value="f" <?=($acasnmetanaocumulativa=='f'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="acasnmetanaocumulativa" value="t" <?=($acasnmetanaocumulativa=='t'?"CHECKED":"")?>> Não</td>
      </tr>
      <!--
      <tr>
        <td colspan=12 align='right' class="SubTituloEsquerda">Dados Físicos:</td>
      </tr>
    <tr><td colspan=12>Planejamento Inicial</td></tr>
    <tr><td>Jan</td>
    <td>Fev</td><td>Mar</td><td>Abr</td><td>Mai</td><td>Jun</td><td>Jul</td><td>Ago</td><td>Set</td><td>Out</td><td>Nov</td><td>dez</td>
    </tr>
    <tr>
    <td><?=campo_texto('fisqtdecroninicial1','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial2','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial3','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial4','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial5','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial6','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial7','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial8','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial9','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial10','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial11','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecroninicial12','N','S','',5,15,'###############','');?></td>
    </tr>
    
    <tr><td colspan=12 >Planejamento Revisado</td></tr>
    <tr><td>Jan</td>
    <td>Fev</td><td>Mar</td><td>Abr</td><td>Mai</td><td>Jun</td><td>Jul</td><td>Ago</td><td>Set</td><td>Out</td><td>Nov</td><td>dez</td>
    </tr>
    <tr>
    <td><?=campo_texto('fisqtdecronogramado1','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado2','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado3','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado4','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado5','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado6','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado7','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado8','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado9','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado10','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado11','N','S','',5,15,'###############','');?></td>
    <td><?=campo_texto('fisqtdecronogramado12','N','S','',5,15,'###############','');?></td>
    </tr>

      
 <!--

   numeric(9),
   numeric(9),
  fisqtdeatualano numeric(9),
  fisqtdecronogramadoano numeric(9),

 -->
       <tr bgcolor="#CCCCCC">
        <td></td>
        <td td colspan=12><input type="button" name="btalterar" value="OK!" onclick="validar_alteracao()" class="botao"></td>
      </tr>

   </table>

<?}?>
</form>

<script>
   function validar_alteracao()
   {
     document.formulario.act.value="alterar";
   	document.formulario.submit();
}

</script>

