<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:restprov_aca.inc
   finalidade: permitir o monitoramento de restrições e providências de ação
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

?>
<title>Restrições e Providências</title>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Restrições e Providências de Ação';
monta_titulo($titulo_modulo,'');
$sql="select acacod, unicod, acadsc from acao where acaid=".$_SESSION['acaid'];
$RS = $db->record_set($sql);
$res = $db->carrega_registro($RS,0);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
?>

<form method="post"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="tipograva" >
<input type=hidden name="procura" value=0>

<center>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
      <?@$db -> cabecalho_acao($_SESSION['acaid']);?>
<tr><td align='right' class="SubTituloDireita">Filtro de Restrições:</td>
<td><input type="radio" name="filtro" value="f" onclick="filtra();">Não superadas&nbsp;&nbsp;<input type="radio" name="filtro" value="t" onclick="filtra();">Superadas&nbsp;&nbsp;<input type="radio" name="filtro" value="x" onclick="filtra();">Todas
</td></tr>
</table>
  <br>
<?

 if ($db->testa_gerente_exe($_SESSION['acaid'],'A')) {$gerex = true;$status='gerexec';}
  // verifica se é coordenador de ação
  if ($db->testa_coordenador($_SESSION['acaid'],'A')) {$coordaca = true;$status='coordaca';}
  // verifica se é digitador
  if ($db->testa_digitador($_SESSION['acaid'],'A')) {$digit = true;$status='digit';}
  // caso a autorização não tenha sido feita, testo se é super usuário
  if ($db->testa_superuser() ) {$coordaca = true;$status='coordaca';}

if (($coordaca or $gerex or $digit ) and $_SESSION['exercicioaberto']=='t')
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
      { $filtrado= 'Não superadas';
       $sql = "select rsa.* , trs.trsdsc from restricaoacao rsa, tiporestricao trs where trs.trscod=rsa.trscod and rsa.rsasntempohabil='f' and acaid=".$_SESSION['acaid'];
      }
      else if ($_REQUEST['filtro']=='t')
      { $filtrado= 'Superadas';
        $sql = "select rsa.* , trs.trsdsc from restricaoacao rsa, tiporestricao trs where trs.trscod=rsa.trscod and rsa.rsasntempohabil='t' and acaid=".$_SESSION['acaid'];
      }
      else if ($_REQUEST['filtro']=='x')
      { $filtrado= 'Superadas ou Não superadas';
        $sql = "select rsa.* , trs.trsdsc from restricaoacao rsa, tiporestricao trs where trs.trscod=rsa.trscod and acaid=".$_SESSION['acaid'];
      }

   $RS2=$db->record_set($sql);
   $nlinhas2 = $db->conta_linhas($RS2);

   if ($nlinhas2>=0)
   {
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
                print '<tr><td colspan=2><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:90px; BORDER:2px SOLID #ECECEC; background-color: White;">'.$rsadscrestricao.'</div><img src="../imagens/preview.gif" border="0" align="absmiddle"><a href="javascript:visualiza_rest(\''.md5_encrypt($rsanum,'').'\')">Ver em tela cheia.</a></td></tr>';
                print '<tr><td align="right" class="subtitulodireita">Tipo de Restrição:&nbsp;&nbsp;</td><td>'.$trsdsc.'</td></tr>';
                print '<tr><td align="right" class="subtitulodireita">Data de inclusão:&nbsp;&nbsp; </td><td>'.formata_data($rsadatainclusao).'</td></tr>';
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
                print '<tr valign="top" ><td colspan=2><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:90px; BORDER:2px SOLID #ECECEC; background-color: White;">'.$rsadscprovidencia.'</div><img src="../imagens/preview.gif" border="0" align="absmiddle"><a href="javascript:visualiza_prov(\''.md5_encrypt($rsanum,'').'\')">Ver em tela cheia.</a></td></tr>';
                if ($rsasntempohabil=='t') $th='Sim'; else $th='Não';
                print '<tr><td align="right" class="subtitulodireita">Restrição superada?&nbsp; </td><td>'.$th.'</td></tr>';
                if ($rsasntempohabil=='t')
                print '<tr><td align="right" class="subtitulodireita">Data de Superação:&nbsp;&nbsp; </td><td>'.formata_data($rsadatasuperacao).'</td></tr>';

                print '</table>';
          print "</td></tr>";
         if ($coordaca or $gerex or $digit )
         {
          if ($rsasntempohabil=='f' and $_SESSION['exercicioaberto']=='t') {        	              
          	print '<tr><td><input type="button" class="botao" name="editav" value="Editar Restrição e Providência" onclick="edita('.$rsaid.')"></td><td></td></tr>';}
         }
   }
}
else {
	print '<tr><td colspan=4 align="center" ><font color ="#E41C1A">Não há restrições <b>'.$filtrado.'</b>  cadastradas.</font></td></tr>';
	
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
    function ProcuraAcao(form) {
	document.formulario.procura.value = document.formulario.acacodbusca.value;
	document.formulario.submit();
    }
    
	function Nova_restprov(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/restprov_acao&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}
	function edita(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/restprov_acao&acao=A&rsa="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}

    function visualiza_rest(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/mostrarestprovaca&acao=A&tp=1&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480");
	}
    function visualiza_prov(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/mostrarestprovaca&acao=A&tp=2&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480");
	}
	function filtra()
	{
      	document.formulario.submit();
 }
	
</script>
