<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:restprov_acao.inc
   finalidade: permitir administrar as restriçoes e providências da ação
   */
$modulo=$_REQUEST['modulo'] ;//
$sql="select acaid from acao where acasnrap='t' and acaid=".$_SESSION['acaid'];
$registro=$db->recuperar($sql);
if (is_array($registro)) {
?>
	<script>alert('Esta ação é de Restos a Pagar e não é monitorada!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
	if ($_SESSION['exercicioaberto']=='f') {
?>
	<script>alert('O exercício não está aberto para entrar com Restrições e Providências!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];

// testar se houve solicitação de gravar
if ($_REQUEST['tipograva'])
{

	$_REQUEST['rsadscrestricao'] = str_replace("'","''",stripslashes($_REQUEST['rsadscrestricao']));
	$_REQUEST['rsadscprovidencia'] = str_replace("'","''",stripslashes($_REQUEST['rsadscprovidencia']));
	
    // é umagravação sem liberação
     if ($_REQUEST['tipograva']=='i')
     {
        $sql = 'select rsanum from restricaoacao order by rsanum desc';
        $RS = $db->record_set($sql);
        $res =  $db->carrega_registro($RS,0);
        $libera='t';
        if (is_array($res)) $num = $res['rsanum']+1; else $num=1;
        $sql= "insert into restricaoacao (rsanum,trscod,acaid,rsadatainclusao,rsadscrestricao,rsadscprovidencia,rsasntempohabil,usucpf,rsasnliberado) values (".
        $num.",".
        $_REQUEST['trscod'].",".
        $_SESSION['acaid'].",'".
        date('Y/m/d')."','".
        $_REQUEST['rsadscrestricao']."','".
        $_REQUEST['rsadscprovidencia']."','f','".
        $_SESSION['usucpf']."','".
        $libera."')";
      } else
      {
        if ($_REQUEST['atdata'])
         $sql= "update restricaoacao set trscod=".$_REQUEST['trscod'].",acaid=".$_SESSION['acaid'].",rsadatainclusao='".date('Y/m/d')."',rsadscrestricao='".$_REQUEST['rsadscrestricao']."',rsadscprovidencia='".$_REQUEST['rsadscprovidencia']."',rsadatasuperacao='".$_REQUEST['rsadatasuperacao']."',rsasntempohabil='".$_REQUEST['rsasntempohabil']."',usucpf='".$_SESSION['usucpf']."' where rsaid=".$_REQUEST['tipograva'];
         else
            $sql= "update restricaoacao set trscod=".$_REQUEST['trscod'].",acaid=".$_SESSION['acaid'].",rsadatainclusao='".date('Y/m/d')."',rsadscrestricao='".$_REQUEST['rsadscrestricao']."',rsadscprovidencia='".$_REQUEST['rsadscprovidencia']."',rsasntempohabil='f',usucpf='".$_SESSION['usucpf']."' where rsaid=".$_REQUEST['tipograva'];
    }
    // dbg($sql);
	 $db->executar($sql);
     $db->commit();
     if ($_REQUEST['tipograva']=='i'){
     // envia-email para os monitores do programa e para o proprio
     $sql="select trsdsc from tiporestricao where trscod=".$_REQUEST['trscod'];
      $RSr = $db->record_set($sql);
      $resr =  $db->carrega_registro($RSr,0);
      if(is_array($resr)) foreach($resr as $k=>$v) ${$k}=$v;
     $sql="select usunome as gestor,usuemail as emailproprio from usuario where usucpf='".$_SESSION['usucpf']."'";
       $RSu = $db->record_set($sql);
       $resu =  $db->carrega_registro($RSu,0);
       if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
  
     $sql="select distinct u.usunome,u.usuemail, prg.prgcod,prg.prgdsc, aca.acacod, aca.unicod, aca.acadsc from usuario u inner join perfilusuario pfu on pfu.pflcod in (19,20) and pfu.usucpf=u.usucpf inner join usuarioresponsabilidade urp on urp.prgid in (select prgid from acao where acaid=".$_SESSION['acaid'].") and urp.usucpf=u.usucpf and urp.rpustatus='A' inner join programa prg on prg.prgid = urp.prgid inner join acao aca on aca.acaid=".$_SESSION['acaid'];

     $RSu = $db->record_set($sql);
     $nlinhas = $db->conta_linhas($RSu);
     for ($i=0;$i<=$nlinhas;$i++)
     {
        $resu =  $db->carrega_registro($RSu,$i);
        if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
        $assunto = 'Inclusão de restrição';
        $mensagem = "Sr. Monitor,<br>Foi registrada uma restrição na ação $acacod.$unicod-$acadsc <br>Do programa $prgcod-$prgdsc<br> Pelo usuário $gestor<br><br>Restrição:<br>".$_REQUEST['rsadscrestricao']."<br><br>Providência:<br>".$_REQUEST['rsadscprovidencia']."<br><br>---------------------------------------------------<br>SIMEC<br>Sistema de Informações do MEC";
        email(str_to_upper($usunome), $usuemail, $assunto, $mensagem);

     }
     email(str_to_upper($gestor), $emailproprio, $assunto, $mensagem);
     }
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
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<br>
<?

$titulo_modulo='Restrições e Providências de Ação';
monta_titulo($titulo_modulo,'');

$sql="select acacod, unicod, acadsc from acao where acaid=".$_SESSION['acaid'];
$RS = $db->record_set($sql);
$res = $db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
 <tr>
        <td align='right' class="SubTituloDireita">Ação :</td>
        <td><b><?=$acacod?>&nbsp;-&nbsp;<?=$unicod?>&nbsp;-&nbsp;<?=$acadsc?></b></td>
  </tr>
</table>
<br>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<form method="post"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="refcod" value=<?=md5_decrypt($_REQUEST['cod'],'')?>>

<center>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
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

<?

 if ($db->testa_gerente_exe($_SESSION['acaid'],'A')) $gerex = true;
  // verifica se é coordenador de ação
  if ($db->testa_coordenador($_SESSION['acaid'],'A')) $coordaca = true;
  // verifica se é digitador
  if ($db->testa_digitador($_SESSION['acaid'],'A')) $digit = true;
  // caso a autorização não tenha sido feita, testo se é super usuário
  if ($db->testa_superuser() ) $coordaca = true;

//}
?>

<div align="center">

<?
if ($_REQUEST['rsa'])
  {
    $sql = 'select * from restricaoacao where rsaid='.$_REQUEST['rsa'];
    $saida = $db->recuperar($sql);
    if(is_array($saida))
	foreach($saida as $k=>$v) ${$k}=$v;
  }
?>

<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<? if ($_REQUEST['rsa'])
  print '<input type=hidden name="tipograva" value='.$_REQUEST['rsa'].'>';
  else print '<input type=hidden name="tipograva" value="i">';
    ?>


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
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

		<?=campo_textarea("rsadscrestricao",'N','S','',95,8,'');?>
	    </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Providência:</td>
        <td>

		<?=campo_textarea("rsadscprovidencia",'N','S','',95,8,'');?>
	    </td>
    </tr>

    <? if ($_REQUEST['rsa']) {?>
    <input type=hidden name="atdata" value=0>
    <tr>
        <td align='right' class="SubTituloDireita">A restrição foi superada? </td>
        <td class="CampoEsquerda">
        <input type="radio" name="rsasntempohabil" value="t" <?=($rsasntempohabil=='t'?"CHECKED":"")?>> Sim
            <input type="radio" name="rsasntempohabil" value="f" <?=($rsasntempohabil=='f'?"CHECKED":"")?>> Não &nbsp;&nbsp;&nbsp;&nbsp;Se <b>SIM</b> Entre com a data de superação&nbsp;&nbsp;<?=campo_data('rsadatasuperacao', 'S','S','','S');?>
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

    function validar_cadastro()
    {
  	   if (!validaBranco(document.formulario.trscod, 'Tipo de Restrição')) return;
	document.formulario.rsadscrestricao.value = tinyMCE.getContent('rsadscrestricao');
	if (!validaBranco(document.formulario.rsadscrestricao, 'Restrição')) return tinyMCE.execCommand('mceFocus', true, 'rsadscrestricao');
	document.formulario.rsadscprovidencia.value = tinyMCE.getContent('rsadscprovidencia');
	if (!validaBranco(document.formulario.rsadscprovidencia, 'Providência')) return tinyMCE.execCommand('mceFocus', true, 'rsadscprovidencia');

  <? if ($_REQUEST['rsa']) {?>
		if (document.formulario.rsasntempohabil[0].checked)
		{
            if (!validaData(document.formulario.rsadatasuperacao))
		      {
                 alert("Data de superação inválida.");
			     document.formulario.rsadatasuperacao.focus();
			     return;
		    }
		    document.formulario.atdata.value='d';
         }
       <? }?>
       document.formulario.submit();
    }
</script>
