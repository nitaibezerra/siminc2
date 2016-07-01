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

        $sql= "select * from acao where acaid=".$_SESSION['acaid'];
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;

       $sql = "update monitora.acao set acasnmetanaocumulativa='".$_REQUEST['acasnmetanaocumulativa']."' where acacod='$acacod' and prgcod='$prgcod' and prgano='".$_SESSION['exercicio']."'";

       $saida = $db->executar($sql);

  if ( !  $_REQUEST['fisqtdeprevistoano'] ) $_REQUEST['fisqtdeprevistoano'] =0;


  
     $sql = "select acacod from monitora.dadofisico  where acaid=".$_SESSION['acaid'];
       if($db->pegaUm($sql)){
  
       // atualização dos atributos do dadofisico
        $sql = "update dadofisico set fisqtdeprevistoano = ". $_REQUEST['fisqtdeprevistoano']." where acaid=".$_SESSION['acaid'];

    
   }
   else
   {
      $sql = "insert into monitora.dadofisico (fisqtdeprevistoano,acacod,prgcod,prgano,regcod,saccod,acaid,loccod) values (".$_REQUEST['fisqtdeprevistoano'].",'$acacod','$prgcod','$prgano','$regcod','$saccod',$acaid,'$loccod')";
   }
      $saida = $db->executar($sql);
      $db->commit();
      $db->sucesso($modulo);


}

if($_SESSION['acaid'] ) {
        $sql= "select * from acao where acaid=".$_SESSION['acaid'];
        $saida = $db->recuperar($sql,$res);
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
        $sql= "select fisqtdeprevistoano from monitora.dadofisico where acaid=".$_SESSION['acaid'];

        $fisqtdeprevistoano = $db->pegaUm($sql);
  
}


?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<table width='100%' class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td align='right' class="SubTituloEsquerda"><h2>Editar Ação</h2></td>
</tr>


<?
if ($acaid)
{?>
      <tr>
        <td align='right' class="SubTituloDireita">Código:</td>
	    <td ><?=$prgcod.'-'.$acacod.'.'.$unicod;?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
	    <td ><?=$acadsc;?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Meta:</td>
	    <td ><?=campo_texto('fisqtdeprevistoano','N','S','',5,15,'###############','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Meta cumulativa?</td>
	    <td ><input type="radio" name="acasnmetanaocumulativa" value="f" <?=($acasnmetanaocumulativa=='f'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="acasnmetanaocumulativa" value="t" <?=($acasnmetanaocumulativa=='t'?"CHECKED":"")?>> Não</td>
      </tr>
   
       <tr bgcolor="#CCCCCC">
        <td></td>
        <td ><input type="button" name="btalterar" value="OK!" onclick="validar_alteracao()" class="botao"></td>
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

