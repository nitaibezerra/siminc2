<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   módulo:novoparecprg.inc
   finalidade: permitir administrar o parecer de alta gestão
   */
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
// testar se houve solicitação de gravar
if ($_REQUEST['tipograva'])
{
     // é uma gravação sem liberação
     // verifico se o cod é zero, se for, será um insert, senão será update
     if ($_REQUEST['tipograva']=='i')
     {
        $sql= "insert into avaliacaoparecer (prgid,avptexto,avpstatus,usucpf,avporigem,tpaid) values (".
        $_SESSION['prgid'].",'".
        $_REQUEST['avptexto']."','A','".
        $_SESSION['usucpf']."',9,3)";
      } else
         $sql= "update avaliacaoparecer set avptexto='".$_REQUEST['avptexto']."' where avpid=".$_REQUEST['tipograva'];

     $db->executar($sql);
     $db->commit();
     // envia-email para os monitores do programa e para o proprio
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
        $assunto = 'Parecer de Alta Gestão em Programa';
        $mensagem = 'Foi incluído ou alterado no SIMEC um Parecer de Alta Gestão no Programa '.$prgcod.'-'.$prgdsc.'<br> Por '.$gestor;
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
<title>Parecer de Alta Gestão</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<br>
<?

$titulo_modulo='Parecer de Alta Gestão';
monta_titulo($titulo_modulo,'');

$sql= "select * from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."'  order by prgcod";
if ($_REQUEST['prgid']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";
if ($_post['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_post['procura']."'  order by prgcod";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);


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
   // verifica se é alta gestão
  $alta_gestao = false;
  $super=false;
$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=11 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro))
  $alta_gestao = true;

  // testo se é super usuário

$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=6 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro))  {$alta_gestao = true;$super=true;}
?>

<div align="center">

<?
if ($_REQUEST['pag'])
  {
    if (! $super)
    $sql = 'select * from avaliacaoparecer where avpid='.$_REQUEST['pag']." and usucpf='".$_SESSION['usucpf']."'";
    else
    $sql = 'select * from avaliacaoparecer where avpid='.$_REQUEST['pag'];
    $saida = $db->recuperar($sql);
    if(is_array($saida))
	foreach($saida as $k=>$v) ${$k}=$v;
  }
?>

<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<? if ($_REQUEST['pag'])
  print '<input type=hidden name="tipograva" value='.$_REQUEST['pag'].'>';
  else print '<input type=hidden name="tipograva" value="i">';
    ?>


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
  <tr><td colspan=2 width='100%'><?=campo_textarea("avptexto",'N','S','',110,28,'');?></td></tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
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
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envio_de_E-mail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480'");
    }
    function validar_cadastro()
    {

	document.formulario.avptexto.value = tinyMCE.getContent('avptexto');
	if (!validaBranco(document.formulario.avptexto, 'Parecer')) return tinyMCE.execCommand('mceFocus', true, 'avptexto');
       document.formulario.submit();
    }
</script>
