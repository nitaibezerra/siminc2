<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:restprov.inc
   finalidade: permitir o monitoramento de restrições e providências de programa
   */
$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
if ($_REQUEST['prgid']) $_SESSION['prgid']= $_REQUEST['prgid']; else $_REQUEST['prgid'] = $_SESSION['prgid'];

?>
<title>Restrições e Providências</title>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Restrições e Providências de Programa';
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
<input type=hidden name="procura" value=0>


<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Código:</td>
        <td><?=campo_texto('prgcod','N','S','',8,100,'','');?>
	&nbsp;&nbsp;<img border="0" src="../imagens/busca.gif"  alt='procura programa' title='procura programa' onclick="ProcuraPrograma()">
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
<tr><td align='right' class="SubTituloDireita">Filtro de Restrições:</td>
<td><input type="radio" name="filtro" value="f" onclick="filtra();">Não superadas&nbsp;&nbsp;<input type="radio" name="filtro" value="t" onclick="filtra();">Superadas&nbsp;&nbsp;<input type="radio" name="filtro" value="x" onclick="filtra();">Todas
</td></tr>
</table>
  <br>
<?
  // verifica se é gerente
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=2 and prgid='.$_SESSION['prgid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) {$geren = true;$status='gerente';}
  // verifica se é gerente-executivo
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=3 and prgid='.$_SESSION['prgid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) {$gerex = true;$status='gerexec';}
  // verifica se é digitador
  $sql= 'select usucpf from usuarioresponsabilidade where pflcod=8 and prgid='.$_SESSION['prgid']." and usucpf = '".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) {$digit = true;$status='digit';}

  // testo se é super usuário

$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=6 and pu.usucpf ='".$_SESSION['usucpf']."'";
  $registro=$db->recuperar($sql);
  if (is_array($registro)) {
     $geren = true;
     $status='gerente';
  }

?>
<?
if (($geren or $gerex or $digit ) and $_SESSION['anoexercicio']==$_SESSION['exercicio'])
   {
       print '<table><tr><td ><input type="button" name="newrestprov" value="Entrar nova restrição e providência" onclick="Nova_restprov()"></td></tr></table>';
   }
?>
<input type='hidden' name='status' value='<?=$status?>'>
<table border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="#F5F5F5" class="tabela">
  <tbody>
     <tr><td width='50%'><b>Restrições</b><td></td><td width='50%'><b>Providências</b></td></tr>
     <?
      if (! $_REQUEST['filtro'] or $_REQUEST['filtro']=='f')
       $sql = "select rsp.* , trs.trsdsc from restricaoprograma rsp, tiporestricao trs where trs.trscod=rsp.trscod and rsp.rspsntempohabil='f' and prgid=".$_SESSION['prgid'];
      else if ($_REQUEST['filtro']=='t')
        $sql = "select rsp.* , trs.trsdsc from restricaoprograma rsp, tiporestricao trs where trs.trscod=rsp.trscod and rsp.rspsntempohabil='t' and prgid=".$_SESSION['prgid'];
      else if ($_REQUEST['filtro']=='x')
        $sql = "select rsp.* , trs.trsdsc from restricaoprograma rsp, tiporestricao trs where trs.trscod=rsp.trscod and prgid=".$_SESSION['prgid'];
   $RS2=$db->record_set($sql);
   $nlinhas2 = $db->conta_linhas($RS2);
   for ($j=0;$j<=$nlinhas2;$j++)
   {
       $res =  $db->carrega_registro($RS2,$j);
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
          if ($j>0) {
            print '<tr><td><hr></td><td></td><td><hr></td></tr>';
            }
          print "<tr><td valign='top' >";
          // tudo de restrições
                print "<table class='tabela' cellspacing='1' cellpadding='0'>";
                print '<tr><td colspan=2><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:90px; BORDER:2px SOLID #ECECEC; background-color: White;">'.$rspdscrestricao.'</div><img src="../imagens/preview.gif" border="0" align="absmiddle"><a href="javascript:visualiza_rest(\''.md5_encrypt($rspnum,'').'\')">Ver em tela cheia.</a></td></tr>';
                print '<tr><td align="right" class="subtitulodireita">Tipo de Restrição:&nbsp;&nbsp;</td><td>'.$trsdsc.'</td></tr>';
                print '<tr><td align="right" class="subtitulodireita">Data de inclusão:&nbsp;&nbsp; </td><td>'.formata_data($rspdatainclusao).'</td></tr>';
                $sql="select u.usunome,u.usuemail,u.usufoneddd,u.usufonenum,o.orgdsc from usuario u left join orgao o on u.orgcod = o.orgcod where u.usucpf='".$usucpf."'";
                $RSu = $db->record_set($sql);
                $resu =  $db->carrega_registro($RSu,0);
                if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
                print '<tr><td align="right" class="subtitulodireita">Responsável:&nbsp;&nbsp; </td><td>'.$usunome.'</td></tr>';
                print '<tr><td align="right" class="subtitulodireita">Órgão:&nbsp;&nbsp; </td><td>'.$orgdsc.'</td></tr>';
                print '</table>';
          
          print "</td><td></td><td valign='top' >";
          
                // tudo de providencias
                print "<table class='tabela'  cellspacing='1' cellpadding='0'>";
                print '<tr valign="top" ><td colspan=2><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:90px; BORDER:2px SOLID #ECECEC; background-color: White;">'.$rspdscprovidencia.'</div><img src="../imagens/preview.gif" border="0" align="absmiddle"><a href="javascript:visualiza_prov(\''.md5_encrypt($rspnum,'').'\')">Ver em tela cheia.</a></td></tr>';
                if ($rspsntempohabil=='t') $th='Sim'; else $th='Não';
                print '<tr><td align="right" class="subtitulodireita">Restrição superada?&nbsp; </td><td>'.$th.'</td></tr>';
                if ($rspsntempohabil=='t')
                print '<tr><td align="right" class="subtitulodireita">Data de Superação:&nbsp;&nbsp; </td><td>'.formata_data($rspdatasuperacao).'</td></tr>';

                print '</table>';
          print "</td></tr>";
         if ($geren or $gerex or $digit )
         {
          if ($rsasntempohabil=='f' and $_SESSION['anoexercicio']==$_SESSION['exercicio']) {         	
              print '<tr><td><input type="button" class="botao" name="editav" value="Editar Restrição e Providência" onclick="edita('.$rspnum.')"></td><td></td></tr>';}
         }
   }

   print '</table>';
?>

   </td>
   </tr>
</tbody>
</table>
</center>
</div>
</form>

<script>
    function verregistro(cod) {
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
    function ProcuraPrograma() {
	document.formulario.procura.value = document.formulario.prgcod.value;
	document.formulario.submit();
    }
    
	function Nova_restprov()
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/restprov_prg&acao=A";
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}
	function edita(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/restprov_prg&acao=A&rsp="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}

    function visualiza_rest(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/mostrarestprov&acao=A&tp=1&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480");
	}
    function visualiza_prov(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/mostrarestprov&acao=A&tp=2&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480");
	}
	function filtra()
	{
      	document.formulario.submit();
 }
	
</script>
