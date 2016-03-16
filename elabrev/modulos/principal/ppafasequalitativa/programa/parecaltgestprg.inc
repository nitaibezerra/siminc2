<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:parecaltgestprg.inc
   finalidade: permitir o monitoramento de parecer de alta gestão
   */
$modulo=$_REQUEST['modulo'] ;//
include "includes/cabecalho.inc";
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];

if ($_POST['exclui'])
{
	if ($_SESSION['anoexercicio']<>$_SESSION['exercicio']) {
?>
	<script>alert('O exercício escolhido não é o corrente!');
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
		if ($_SESSION['anoexercicio']<>$_SESSION['exercicio']) {
?>
	<script>alert('O exercício escolhido não é o corrente!');
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

$sql= "select * from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."'  order by prgcod";
if ($_REQUEST['prgid']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid']."  order by prgcod";
if ($_POST['procura']) $sql= "select * from programa where prgano = '".$_SESSION['exercicio']."' and prgcod='".$_POST['procura']."'  order by prgcod";

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

// as linhas abaixo não permitem navegar para além dos limites
//controla_navegacao($nlinhas);
if ($nlinhas < 0){
	?>
	<script>alert('Programa Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
// fim do controle de navegação
$res =  $db->carrega_registro($RS,0);
// a linha abaixo transforma em variÃ¡veis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

	$_SESSION['prgid']=$prgid;
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

<form method="POST"  name="formulario" onsubmit="ProcuraPrograma();">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="tipograva" >
<input type=hidden name="exclui" >
<input type=hidden name="arquiva" >
<input type=hidden name="procura" value=0>


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Código:</td>
        <td><?=campo_texto('prgcod','N','S','',10,100,'','');?>
	&nbsp;&nbsp;<img border="0" src="imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="ProcuraPrograma()">
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Orgão Responsável:</td>
        <td><?=$orgdsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Unidade Responsável:</td>
         <td><?=$unidsc?></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$prgdsc?></td>
      </tr>
<?

   $sql = "select pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$_SESSION['prgid']." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where  pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X','D') order by pfl.pfldsc";

    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if (! $mostra ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario."</td></tr>";
            print $linha;
            }

        }
     }

?>
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


if ($alta_gestao and $_SESSION['anoexercicio']==$_SESSION['exercicio'])
   {
       print '<table><tr><td ><input type="button" name="newrestprov" value="Entrar novo parecer" onclick="Novo_parecer()"></td></tr></table>';
   }
?>
<input type='hidden' name='status' value='<?=$status?>'>
<table width='100%' border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="#F5F5F5" class="tabela">
  <tbody>
     
     <?
      if (! $_REQUEST['filtro'] or $_REQUEST['filtro']=='A')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid, avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus='A' and prgid=".$_SESSION['prgid'];
      else if ($_REQUEST['filtro']=='D')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid,avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus='D' and prgid=".$_SESSION['prgid'];
      else if ($_REQUEST['filtro']=='x')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid, avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus<>'I' and prgid=".$_SESSION['prgid'];
       
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
                print '<tr><td colspan=4 ><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:90px; BORDER:2px SOLID #ECECEC; background-color: White;">'.$avptexto.'</div><img src="imagens/preview.gif" border="0" align="absmiddle"><a href="javascript:visualiza_parec(\''.md5_encrypt($avpid,'').'\')">Ver em tela cheia.</a></td></tr>';
                print '<tr><td align="right" ><b>Data de inclusão:</b>&nbsp;&nbsp; </td><td>'.$avpdata.'</td>';
                $sql="select u.usunome,u.usuemail,u.usufoneddd,u.usufonenum,o.orgdsc from usuario u left join orgao o on u.orgcod = o.orgcod where u.usucpf='".$usucpf."'";
                $RSu = $db->record_set($sql);
                $resu =  $db->carrega_registro($RSu,0);
                if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
                print '<td align="right" ><b>Responsável:</b>&nbsp;&nbsp; </td><td>'.$usunome.' / '.$orgdsc.'</td></tr>';
                if ($super or $usucpf == $_SESSION['usucpf'])
                {// ou é superusuáro ou é quem criou o parecer
                   if ($avpstatus=='A'){
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

    function ProcuraPrograma(form) {
	document.formulario.procura.value = document.formulario.prgcod.value;
	document.formulario.submit();
    }
    
	function Novo_parecer()
	{
		e = "simec.php?modulo=principal/ppafasequalitativa/programa/novoparecprg&acao=A";
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}
	function edita_parec(cod)
	{
		e = "simec.php?modulo=principal/ppafasequalitativa/programa/novoparecprg&acao=A&pag="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}

    function visualiza_parec(cod)
	{
		e = "simec.php?modulo=principal/ppafasequalitativa/programa/mostraparec&acao=A&cod="+cod;
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
