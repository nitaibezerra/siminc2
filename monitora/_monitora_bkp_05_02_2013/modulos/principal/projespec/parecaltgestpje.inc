<?
 /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:parecaltgestpje.inc
   finalidade: permitir o monitoramento de parecer de alta gestão no projeto especial
   */
$modulo=$_REQUEST['modulo'] ;//

if (! $_SESSION['pjeid']) 
{
	?>
	<script>alert('Escolha primeiro o Projeto Especial!');
	history.back();</script>
	<?
	$db->close();
	exit();
}
include APPRAIZ."includes/cabecalho.inc";

if ($_POST['exclui'])
{
    $sql = "update monitora.avaliacaoparecer set avpstatus='I' where avpid=".$_POST['exclui'];
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}
if ($_POST['arquiva'])
{
    $sql = "update monitora.avaliacaoparecer set avpstatus='D' where avpid=".$_POST['arquiva'];
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}


?>
<title>Parecer da Alta Gestão em Projetos Especiais</title>
<br>
<?
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Parecer da Alta Gestão';
monta_titulo($titulo_modulo,'');

$sql="select pjecod, pjedsc from monitora.projetoespecial where pjeid=".$_SESSION['pjeid'];
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
      <tr>
        <td align='right' class="SubTituloDireita">Projeto :</td>
        <td><b><?=$pjecod?>&nbsp;-&nbsp;<?=$pjedsc?></b></td>
      </tr>
  <tr>
  <td></td>
  <td>
  <input type="button" name="btequipe" value="Equipe do Projeto" onclick="mostra_equipe()">
  </td>
  </tr>      

<tr><td align='right' class="SubTituloDireita">Filtro de Parecer:</td>
<td><input type="radio" name="filtro" value="A" onclick="filtra();" <?if($_REQUEST['filtro']=='A')print 'checked'?>>Ainda válidos&nbsp;&nbsp;<input type="radio" name="filtro" value="D" onclick="filtra();" <?if($_REQUEST['filtro']=='D')print 'checked'?>>Arquivados&nbsp;&nbsp;<input type="radio" name="filtro" value="x" onclick="filtra();" <?if($_REQUEST['filtro']=='x')print 'checked'?>>Todos
</td></tr>
</table>
  <br>
<?
  
  
  $autoriza = false;

 	   //Verifica se é o gerente do projeto
       $sql="select ur.rpuid from monitora.usuarioresponsabilidade ur where ur.pflcod=12 and ur.pjeid=".$_SESSION['pjeid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' order by ur.rpudata_inc limit 1";
                
       $res =  $db->pegalinha($sql);
       	//Caso seja autoriza
       	if(is_array($res)) $autoriza = true;
       
  	  //verifica se é alta gestão
      $sql="select u.usunome as altgest,u.usuemail as emailcoord from seguranca.usuario u inner join seguranca.perfilusuario pfu on pfu.pflcod=58 and pfu.usucpf=u.usucpf inner join monitora.usuarioresponsabilidade urp on urp.pjeid=".$_SESSION['pjeid']." and u.usucpf='".$_SESSION['usucpf']."' and urp.usucpf=u.usucpf";

    
      
      $res =  $db->pegalinha($sql);
        //Caso seja altoriza
      	if(is_array($res)) $autoriza = true;

     //verifica se é super usuário
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod=57 and p.sisid=".$_SESSION['sisid']." and pu.usucpf ='".$_SESSION['usucpf']."'";

	$registro=$db->recuperar($sql);
  		//Caso seja super usuário autoriza
		if (is_array($registro)) $autoriza = true;
  
  //Caso seja autorizado mostra o botão enviar
  if($autoriza)
  {
     print '<table><tr><td ><input type="button" name="newrestprov" value="Entrar novo parecer" onclick="Novo_parecer()">
</td></tr></table>';
  }
?>
<input type='hidden' name='status' value='<?=$status?>'>
<table width='100%' border="0" cellspacing="0" cellpadding="0" align="center" bgcolor="#F5F5F5" class="tabela">
  <tbody>
     
     <?
      if (! $_REQUEST['filtro'] or $_REQUEST['filtro']=='A')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid, avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus='A' and pjeid=".$_SESSION['pjeid'];
      else if ($_REQUEST['filtro']=='D')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid,avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus='D' and pjeid=".$_SESSION['pjeid'];
      else if ($_REQUEST['filtro']=='x')
       $sql = "select to_char(avp.avpdata,'DD/MM/YYYY HH:MM')as avpdata, avp.avptexto,avp.usucpf,avp.avpid, avp.avpstatus from avaliacaoparecer avp  where avp.avporigem = 9 and avp.avpstatus<>'I' and pjeid=".$_SESSION['pjeid'];
       
       
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
                
                   ?>
                  <tr>
                  <?
	                   //Caso seja o usuário que fez o parecer 
	                  if ($usucpf == $_SESSION['usucpf'])
	                  {
	                  ?>
		                  <td colspan=3><br><input type='button' class='botao' name='btaltera' value='Editar Parecer' onclick="edita_parec(<?=$avpid?>)">
		                  &nbsp;&nbsp;&nbsp;
	                <?  }  	if ($coordpje) {?>
	                  
                  <input type='button' class='botao' name='btarquiva' value='Arquivar Parecer' onclick="arquiva_parec(<?=$avpid?>)">&nbsp;&nbsp;&nbsp;<input type='button' class='botao' name='btcancela' value='Cancelar Parecer' onclick="cancela_parec(<?=$avpid?>)">
                     <?}
                  

                
                print '</table>';
    }
   print '</tr></table>';
   
   if($j==0)
   {
   	 print "<font color='red'>Não há parecer para o filtro selecionado!</font>";
   }
   

?>

   </td>
   </tr>
</tbody>
</table>
</center>
</div>
</form>

<script>
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
	function Novo_parecer()
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/projespec/novoparecpje&acao=A";
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
       
	          
	}
	function edita_parec(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/projespec/novoparecpje&acao=A&pag="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=580");
	}

    function visualiza_parec(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/projespec/mostraparec&acao=A&cod="+cod;
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
    function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }
  
  
</script>
