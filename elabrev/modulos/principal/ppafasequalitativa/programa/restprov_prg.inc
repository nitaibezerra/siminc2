<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:restprov_prg.inc
   finalidade: permitir administrar as restriçoes e providências
   */
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
	if ($_SESSION['anoexercicio']<>$_SESSION['exercicio']) {
?>
	<script>alert('O exercício escolhido não é o corrente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
// testar se houve solicitação de gravar

if ($_REQUEST['tipograva'])
{
	$_REQUEST['rspdscrestricao'] = str_replace("'","''",stripslashes($_REQUEST['rspdscrestricao']));
	$_REQUEST['rspdscprovidencia'] = str_replace("'","''",stripslashes($_REQUEST['rspdscprovidencia']));
     // é umagravação sem liberação
     // verifico se o cod é zero, se for, será um insert, senão será update
     if ($_REQUEST['tipograva']=='i')
     {
        $sql = 'select rspnum from restricaoprograma order by rspnum desc';
        $RS = $db->record_set($sql);
        $res =  $db->carrega_registro($RS,0);
        $libera='t';
        if (is_array($res)) $num = $res['rspnum']+1; else $num=1;
        $sql= "insert into restricaoprograma (rspnum,trscod,prgid,rspdatainclusao,rspdscrestricao,rspdscprovidencia,rspsntempohabil,usucpf,rspsnliberado) values (".
        $num.",".
        $_REQUEST['trscod'].",".
        $_SESSION['prgid'].",'".
        date('Y/m/d')."','".
        $_REQUEST['rspdscrestricao']."','".
        $_REQUEST['rspdscprovidencia']."','f','".
        $_SESSION['usucpf']."','".
        $libera."')";
      } else
      {
        if ($_REQUEST['atdata'])
         $sql= "update restricaoprograma set trscod=".$_REQUEST['trscod'].",prgid=".$_SESSION['prgid'].",rspdatainclusao='".date('Y/m/d')."',rspdscrestricao='".$_REQUEST['rspdscrestricao']."',rspdscprovidencia='".$_REQUEST['rspdscprovidencia']."',rspdatasuperacao='".$_REQUEST['rspdatasuperacao']."',rspsntempohabil='".$_REQUEST['rspsntempohabil']."',usucpf='".$_SESSION['usucpf']."' where rspnum=".$_REQUEST['tipograva'];
         else
            $sql= "update restricaoprograma set trscod=".$_REQUEST['trscod'].",prgid=".$_SESSION['prgid'].",rspdatainclusao='".date('Y/m/d')."',rspdscrestricao='".$_REQUEST['rspdscrestricao']."',rspdscprovidencia='".$_REQUEST['rspdscprovidencia']."',rspsntempohabil='f',usucpf='".$_SESSION['usucpf']."' where rspnum=".$_REQUEST['tipograva'];
    }
     $db->executar($sql);
     $db->commit();
     // envia-email para os monitores do programa e para o proprio
     $sql="select trsdsc from tiporestricao where trscod=".$_REQUEST['trscod'];
      $RSr = $db->record_set($sql);
      $resr =  $db->carrega_registro($RSr,0);
      if(is_array($resr)) foreach($resr as $k=>$v) ${$k}=$v;
       
     $sql="select usunome as gestor,usuemail as emailproprio from usuario where usucpf='".$_SESSION['usucpf']."'";
       $RSu = $db->record_set($sql);
       $resu =  $db->carrega_registro($RSu,0);
       if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;

     $sql="select distinct u.usunome,u.usuemail, prg.prgcod,prg.prgdsc from usuario u inner join perfilusuario pfu on pfu.pflcod in (19,20) and pfu.usucpf=u.usucpf inner join usuarioresponsabilidade urp on urp.prgid =".$_SESSION['prgid']." and urp.usucpf=u.usucpf  and urp.rpustatus='A' inner join programa prg on prg.prgid = urp.prgid ";

     $RSu = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RSu);
     for ($i=0;$i<=$nlinhas;$i++)
     {
        $resu =  $db->carrega_registro($RSu,$i);
        if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
        $assunto = 'Inclusão de restrição';
        $mensagem = "Sr. Monitor,<br>Foi registrada uma restrição no Programa <br>$prgcod -$prgdsc<br>Do tipo $trsdsc.<br> Pelo usuário $gestor.<br><br>Restrição:<br>".$_REQUEST['rspdscrestricao']."<br><br>Providência:<br>".$_REQUEST['rspdscprovidencia']."<br><br>---------------------------------------------------<br>SIMEC<br>Sistema de Informações do MEC";
        email(str_to_upper($usunome), $usuemail, $assunto, $mensagem);

     }
     email(str_to_upper($gestor), $emailproprio, $assunto, $mensagem);
     $db->close();

   ?>
    <script>
    alert('Operação realizada com sucesso!');
    opener.location.reload();
    self.close();
    </script>
    <?
    exit();
}


?>
<title>Restrições e Providências</title>
<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='includes/listagem.css'>
<br>
<?

$titulo_modulo='Restrições e Providências de Programa';
monta_titulo($titulo_modulo,'');

$sql= "select * from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."'  order by prgcod";
if ($_REQUEST['prgid']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";
if ($_post['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_post['procura']."'  order by prgcod";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
// as linhas abaixo não permitem navegar para além dos limites
//controla_navegacao($nlinhas);

// fim do controle de navegação

$res =  $db->carrega_registro($RS,$_post['registro']);
// a linha abaixo transforma em variÃ¡veis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o orgao
    $sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a unidade
    $sql= "select unidsc from unidade where unicod='".$unicod."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar o tipo de programa

?>

<script language="JavaScript" src="includes/calendario.js"></script>
<script language="JavaScript" src="includes/funcoes.js"></script>
<form method="post"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="atdata" >

<center>
<script language="javascript" type="text/javascript" src="includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw"
	});
</script>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita">Programa:</td>
        <td><b><?=$prgcod.' - '.$prgdsc;?></b></td>
      </tr>
</table>

<?
  // verifica se é gerente
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=2 and prgid='.$_SESSION['prgid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) $geren = true;
  // verifica se é gerente-executivo
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=3 and prgid='.$_SESSION['prgid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) $gerex = true;
  // verifica se é digitador
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=8 and prgid='.$_SESSION['prgid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) $digit = true;

  // testo se é super usuário

$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=6 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) {
     $geren = true;
  }

?>

<div align="center">

<?
if ($_REQUEST['rsp'])
  {
    $sql = 'select * from restricaoprograma where rspnum='.$_REQUEST['rsp'];
    $saida = $db->recuperar($sql);
    if(is_array($saida))
	foreach($saida as $k=>$v) ${$k}=$v;
  }
?>

<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<? if ($_REQUEST['rsp']) 
  print '<input type=hidden name="tipograva" value='.$_REQUEST['rsp'].'>';
  else print '<input type=hidden name="tipograva" value="i">';
    ?>


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
   <tr>
      <td align='right' class="SubTituloDireita">Tipo de Restrição:</td>
      <td>
		<?	  $sql = "select trscod as CODIGO,trsdsc as DESCRICAO from tiporestricao where trsstatus='A' order by trscod ";
	  $db->monta_combo('trscod',$sql,'S',"Selecione o Tipo de Restrição",'S','');?><?=obrigatorio();?>
	  </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Restrição:</td>
        <td>

		<?=campo_textarea("rspdscrestricao",'N','S','',95,8,'');?>
	    </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Providência:</td>
        <td>

		<?=campo_textarea("rspdscprovidencia",'N','S','',95,8,'');?>
	    </td>
    </tr>
    <? if ($_REQUEST['rsp']) {?>
    <tr>
        <td align='right' class="SubTituloDireita">A restrição foi superada? </td>
        <td class="CampoEsquerda">
        <input type="radio" name="rspsntempohabil" value="t" <?=($rspsntempohabil=='t'?"CHECKED":"")?>> Sim
            <input type="radio" name="rspsntempohabil" value="f" <?=($rspsntempohabil=='f'?"CHECKED":"")?>> Não &nbsp;&nbsp;&nbsp;&nbsp;Se <b>SIM</b> Entre com a data de superação&nbsp;&nbsp;<?=campo_data('rspdatasuperacao', '','S','','S');?>
        </td>
    </tr>

    <?}?>

</table>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr bgcolor="#C0C0C0">
   <td align='right'><input type="button" class="botao" name="btgravar" value="Gravar" onclick="validar_cadastro()">&nbsp;&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>

</tr>
 </table>
 </center>
 </div>

</form>

<script>
  function fechar_janela()
  {
    window.close();
    opener.location.reload();
  }

    function envia_email(cpf)
    {
          e = "simec.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envio_de_E-mail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480'");
    }
    function validar_cadastro()
    {
  	   if (!validaBranco(document.formulario.trscod, 'Tipo de Restrição')) return;
	document.formulario.rspdscrestricao.value = tinyMCE.getContent('rspdscrestricao');
	if (!validaBranco(document.formulario.rspdscrestricao, 'Restrição')) return tinyMCE.execCommand('mceFocus', true, 'rspdscrestricao');
	document.formulario.rspdscprovidencia.value = tinyMCE.getContent('rspdscprovidencia');
	if (!validaBranco(document.formulario.rspdscprovidencia, 'Providência')) return tinyMCE.execCommand('mceFocus', true, 'rspdscprovidencia');
      <? if ($_REQUEST['rsp']) {?>
		if (document.formulario.rspsntempohabil[0].checked)
		{
            if (!validaData(document.formulario.rspdatasuperacao))
		      {
			alert("Data de superação inválida.");
			document.formulario.rspdatasuperacao.focus();
			return;
		      }
		    document.formulario.atdata.value='d';
         }
       <? }?>
       document.formulario.submit();
    }
</script>
