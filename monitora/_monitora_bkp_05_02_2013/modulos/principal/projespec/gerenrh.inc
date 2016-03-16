 <?
  /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:cadprojespec.inc
   finalidade: permitir a administração do projeto especial
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

function projetovisivel($cod)
{
	global $db;
	// verifica se o projeto temvisibilidade aberta a todos
	$sql="Select pjesnvisivel from monitora.projetoespecial where pjeid=$cod";
	if ($db->pegaUm($sql)=='t') return true;
	else return false;
}
  
if (! $db->testa_responsavel_projespec(md5_decrypt($_REQUEST['id'],'')) and ! $_SESSION['pjeid'] and ! $db->testa_superuser() ) 
{
	?>
	<script>alert('Você não tem acesso a esta aplicação!!');
	history.back();
	</script>
	<?
	$db->close();
	exit();
}  
$pjeid = md5_decrypt($_REQUEST['id'],'');

$pjeid=$_SESSION['pjeid'];
if ($_REQUEST['act']== 'altera')
{
  // montagem da equipe
  $sql = "delete from monitora.usuarioresponsabilidade where pflcod=51 and pjeid=$pjeid";
  $db->executar($sql);
  $usucpf = $_POST['usucpf'];
  $nlinhas = count($usucpf)-1;
  for ($j=0; $j<=$nlinhas;$j++)
  {  
	 $sql = "insert into monitora.usuarioresponsabilidade (pflcod, pjeid,usucpf) VALUES (51,$pjeid,'".$_POST['usucpf'][$j]."')";
     $db->executar($sql);
  }
  $db -> commit();
  $db->sucesso($modulo);

}



if($_SESSION['pjeid']) {
	    $pjeid = $_SESSION['pjeid'];
        $sql= "select pe.*,ug.ungabrev from monitora.projetoespecial pe left join unidadegestora ug on ug.ungcod=pe.ungcod where pjeid=$pjeid";
        $saida = $db->recuperar($sql,$res);        
        if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
        
        $sql = "select usucpf from monitora.usuarioresponsabilidade where pjeid=$pjeid and pflcod=47 and rpustatus ='A' limit 1";
        $usucpfcoord=$db->pegaUm($sql);

        }


  $coordpje=false;
  if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) {
  	$coordpje = true;

  }
   $superuser = false;
   // verific se é super-usuário
  if ($db->testa_superuser())  {
  	$coordpje = true; 
  	$superuser = true;

  }


include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$db->cria_aba($abacod_tela,$url,'');
if ($_REQUEST['acao']=='I') $titulo_modulo='Montagem da Equipe do Projeto';

monta_titulo($titulo_modulo,'');


?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name='act' >
<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita">Denominação:</td>
        <td><?=$pjecod.'.'.$pjedsc?></td>
      </tr>
        <tr>
  <td></td>
  <td>
  <input type="button" name="btequipe" value="Equipe do Projeto" onclick="mostra_equipe()">
  </td>
  </tr>

</table>
<br> 

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr align="center">
<td colspan="2">
<b> LISTA DE USUÁRIOS COM PERFIL DE EQUIPE DE APOIO A PROJETO ESPECIAL
</b>
</td>
</tr>
<?

 $sql = "select distinct u.usucpf as CODIGO,u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum || ' - '||ee.entnome||' - '||un.unidsc   as DESCRICAO, u.usunome from seguranca.usuario u inner join entidade.entidade ee ON ee.entid=u.entid left join unidade un on un.unicod=u.unicod
where u.usucpf in (select pu.usucpf from seguranca.perfilusuario pu where pu.pflcod=51) 
or u.usucpf in (select us.usucpf from seguranca.usuario_sistema us where us.pflcod=51)
and u.usustatus='A' order by usunome ";

$rs = @$db->carregar( $sql );

if (  $rs && count($rs) > 0 )
{
	foreach ( $rs as $linha )
	{
		$ok=0;
		foreach($linha as $k=>$v) ${$k}=$v;
		if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
        $sql = "select rpuid from monitora.usuarioresponsabilidade where usucpf='$codigo' and pjeid=$pjeid and pflcod=51";
//print $sql;
        $ok=$db->pegaUm($sql);

		?><tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" ><td><input type="checkbox" name="usucpf[]" value='<?=$codigo."'";
		if ($ok) {print ' checked';}?>></td><td><?=$descricao?></td></tr><?
	}
}

?>
<tr bgcolor="#CCCCCC">
        <td></td>
        <td><?if ($coordpje)
             
        {   //Caso o usuário possa alterar faça
        	?>
              <input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao"><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao"></td><?}?>
      </tr>      
   </table>
</center>
</form>



<script>
function validar_cadastro()
{
	if( window.confirm( "Confirma montagem da equipe conforme os nomes marcados?") )
	{
		document.formulario.act.value = 'altera';
		document.formulario.submit();
	} 
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }	

}
  function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }

</script>
