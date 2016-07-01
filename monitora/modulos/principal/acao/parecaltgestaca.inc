<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:parecaltgestaca.inc
   finalidade: permitir o monitoramento de parecer de alta gestãode ação
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
include APPRAIZ."includes/cabecalho.inc";

if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid']= $_REQUEST['acaid']; else $_REQUEST['acaid'] = $_SESSION['acaid'];

if ($_POST['exclui'])
{
if ($_SESSION['exercicioaberto']=='f') {
?>
	<script>alert('O exercício nâo está aberto para entrar parecer!');
	history.back();</script>
	<?
	$db->close();
	exit();
}	
    $sql = "update avaliacaoparecer set avpstatus='I' where avpid=".$_POST['exclui'];
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}
if ($_POST['arquiva'])
{
	if ($_SESSION['exercicioaberto']=='f') {
?>
	<script>alert('O exercício nâo está aberto para entrar parecer!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
    $sql = "update avaliacaoparecer set avpstatus='D' where avpid=".$_POST['arquiva'];
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}


?>
<title>Parecer da Alta Gestão</title>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Parecer da Alta Gestão';
monta_titulo($titulo_modulo,'');

$sql="select acacod, unicod, acadsc from acao where acaid=".$_SESSION['acaid'];
$RS = $db->record_set($sql);
$res = $db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>

<form method="post"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="tipograva" >
<input type=hidden name="exclui" >
<input type=hidden name="arquiva" >

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
      <?@$db -> cabecalho_acao($_SESSION['acaid']);?>
<tr><td align='right' class="SubTituloDireita">Filtro de Parecer:</td>
<td><input type="radio" name="filtro" value="A" onclick="filtra();">Ainda válidos&nbsp;&nbsp;<input type="radio" name="filtro" value="D" onclick="filtra();">Arquivados&nbsp;&nbsp;<input type="radio" name="filtro" value="x" onclick="filtra();">Todos
</td></tr>
</table>
  <br>
<?
  // verifica se é alta gestão
  $alta_gestao = false;
  $super = false;
$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=11 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro))
  $alta_gestao = true;

  // testo se é super usuário

$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=6 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro))  { $alta_gestao = true;   $super = true;}

?>
<?
if ($alta_gestao and $_SESSION['exercicioaberto']=='t')
   {
       print '<table><tr><td ><input type="button" name="newrestprov" value="Entrar novo parecer" onclick="Novo_parecer()"></td></tr></table>';
   }
?>
<input type='hidden' name='status' value='<?=$status?>'>
<table width='100%' border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="#F5F5F5" class="tabela">
  <tbody>
     
     <?
      if (! $_REQUEST['filtro'] or $_REQUEST['filtro']=='A')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid, avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus='A' and acaid=".$_SESSION['acaid'];
      else if ($_REQUEST['filtro']=='D')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid,avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus='D' and acaid=".$_SESSION['acaid'];
      else if ($_REQUEST['filtro']=='x')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid, avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus<>'I' and acaid=".$_SESSION['acaid'];
       
   $RS2=$db->record_set($sql);
   $nlinhas2 = $db->conta_linhas($RS2);
   for ($j=0;$j<=$nlinhas2;$j++)
   {
       $res =  $db->carrega_registro($RS2,$j);
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
          if ($j >0) {
            print '<tr><td><hr></td></tr>';
            }
          print "<tr><td valign='top' >";
                print "<table width='100%' cellspacing='1' cellpadding='0'>";
                print '<tr><td colspan=4 ><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:90px; BORDER:2px SOLID #ECECEC; background-color: White;">'.$avptexto.'</div><img src="../imagens/preview.gif" border="0" align="absmiddle"><a href="javascript:visualiza_parec(\''.md5_encrypt($avpid,'').'\')">Ver em tela cheia.</a></td></tr>';
                print '<tr><td align="right" ><b>Data de inclusão:</b>&nbsp;&nbsp; </td><td>'.$avpdata.'</td>';
                $sql="select u.usunome,u.usuemail,u.usufoneddd,u.usufonenum,o.orgdsc from usuario u left join orgao o on u.orgcod = o.orgcod where u.usucpf='".$usucpf."'";
                $RSu = $db->record_set($sql);
                $resu =  $db->carrega_registro($RSu,0);
                if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
                print '<td align="right" ><b>Responsável:</b>&nbsp;&nbsp; </td><td>'.$usunome.' / '.$orgdsc.'</td></tr>';
                if ($super or $usucpf == $_SESSION['usucpf'])
                {// ou é superusuáro ou é quem criou o parecer
                   if ($avpstatus=='A' and $_SESSION['exercicioaberto']=='t'){
                     // pode arquivá-lo ou editá-lo
                     ?>
                  <tr><td colspan=3><br><input type='button' class='botao' name='btaltera' value='Editar Parecer' onclick="edita_parec(<?=$avpid?>)">&nbsp;&nbsp;&nbsp;<input type='button' class='botao' name='btarquiva' value='Arquivar Parecer' onclick="arquiva_parec(<?=$avpid?>)">&nbsp;&nbsp;&nbsp;<input type='button' class='botao' name='btcancela' value='Cancelar Parecer' onclick="cancela_parec(<?=$avpid?>)">
                     <?
                   }

                }
                print '</table>';
    }
   print '</tr></table>';

?>

   </td>
   </tr>
</tbody>
</table>
</center>
</div>
</form>

<script>
	function Novo_parecer()
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/novoparecaca&acao=A";
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}
	function edita_parec(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/novoparecaca&acao=A&pag="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}

    function visualiza_parec(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/mostraparec&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480");
	}

	function filtra()
	{
      	document.formulario.submit();
    }
  function cancela_parec(cod) {

    if( window.confirm( "Confirma o cancelamento do parecer?") )
    {
	document.formulario.exclui.value = cod;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;

  }
  function arquiva_parec(cod) {

    if( window.confirm( "Confirma o arquivamento do parecer?") )
    {
	document.formulario.arquiva.value = cod;
	document.formulario.submit();
    } else document.formulario.arquiva.value = 0;

  }
</script>
