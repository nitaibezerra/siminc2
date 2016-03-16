<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:cadacao.inc
   Finalidade: permitir o controle do cadastro de acao
   */
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];

if ($_REQUEST['acacod']) $_SESSION['acacod'] = $_REQUEST['acacod'];
else $_REQUEST['acacod'] =$_SESSION['acacod'];


$sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao a, ".$_SESSION['sisdiretorio'].".ppaprograma b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prsano ='" .$_SESSION['exercicio']."' and a.prgid=".$_SESSION['prgid'];

if ($_REQUEST['acacod'] )
{
    $sql = $sql. "  and a.acacod='".$_REQUEST['acacod']."' limit 1";
}
elseif ($_SESSION['acaid'])
{
	$sql = $sql. "  and a.acaid=".$_SESSION['acaid'];
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

$nl=$nlinhas;
if ($nlinhas >= 0) {


   // as linhas abaixo não permitem navegar para além dos limites

// fim do controle de navegação
    $res =  $db->carrega_registro($RS,$_POST['registro']);
// a linha abaixo transforma em variÃ¡veis todos os campos do array
    if(is_array($res)){
       foreach($res as $k=>$v) {${$k}=$v;};
            $_SESSION['prgid'] = $prgid;
            $_SESSION['acaid']= $acaid;

            if ($acasnmetanaocumulativa=='t') $tipometa ='Não cumulativa';
            if ($acasnmetanaocumulativa=='f') $tipometa ='Cumulativa';
            if ($acasnmetanaocumulativa=='') $tipometa ='Não informado';
       }
	// encontrar o desafio (macro-objetivo)
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descriÃ§Ã£o da unidade de medida
        $sql= "select unmdsc from unidademedida where unmcod='".$unmcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descriÃ§Ã£o do tipo de aÃ§Ã£o
        $sql= "select tacdsc from tipoacao where taccod='".$taccod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a funÃ§Ã£o
       if ($funcod) {
        $sql= "select fundsc from elabrev.ppafuncao where funcod='".$funcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
        // encontrar a subfunÃ§Ã£o
       if ($sfucod) {
        $sql= "select sfudsc from elabrev.ppasubfuncao where sfucod='".$sfucod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       }
    // encontrar o Ã³rgÃ£o responsÃ¡vel
        $sql= "select orgdsc from elabrev.ppaorgao where orgcod='".$orgcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    // encontrar o orgao responsÃ¡vel
        $sql= "select orgdsc from elabrev.ppaorgao where substr(orgcod,0,3)='".substr($unicod,0,2)."'";

        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        
    // encontrar a unidade responsÃ¡vel
        $sql= "select unicod||'-'||unidsc as unidsc from unidade where unicod='".$unicod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar o produto
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a meta
        //$sql= "select fisqtdeprevistoano as meta from elabrev.dadofisico where acaid='$acaid' and prsano='".$_SESSION['exercicio']."'";
       //$res = $db->recuperar($sql);
       // if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a esfera
       if ($esfcod) {
        $sql= "select esfdsc from esfera where esfcod='".$esfcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
 } else
 {
   ?>
<script>
    alert('Código da Ação inexistente.! \nVocê pode estar tentando abrir ação de outro exercício.');
    history.back();
</script>
   <?
   exit();
 }

include APPRAIZ."includes/cabecalho.inc";		
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Fusão da Ação';
monta_titulo($titulo_modulo,'');
?>
<? // inclusão do menu de setas de navegação
$nlinhas=$nl;
?> 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="navega" value=0>
<input type=hidden name="procura" value=0>

	 <tr align="left">
        <td align='left' class="SubTituloDireita">Programa:</td>
        <td><b><?=$prgcod?></b> - <?=$prgdsc?>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão Orçamentário Responsável:</td>
        <td><?=campo_texto('orgdsc','N','N','',50,50,'','','','dica','');?></td>       
      </tr>  
      <tr>
        <td align='right' class="SubTituloDireita">Unidade Orçamentária Responsável:</td>
        <td><?=campo_texto('unidsc','N','N','',50,50,'','','','dica',"");?></td>         
      </tr>
 
      <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
        <td><b><?=$acacod?></b>.<?=$unicod.'.'.$loccod?>-<?=campo_texto('acadsc','N','N','',100,100,'','','','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita"></td>
        <td><input type="button" name="btnAbrirResp" value="Escolher as ações que serão fundidas" onclick="popfun('<?=$acacod?>','<?=$prgcod?>')"></td>
      </tr>      

    </table>
 </form>  

<script>
  function popfun(acacod,prgcod)
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostafusao&acao=A&acacod='+acacod+'&prgcod='+prgcod;
		window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=450");
  }
 

</script>

