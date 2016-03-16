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
function atualiza_ator($pje,$usu,$influ,$inter)
{
	// esta função verifica se o usuário informado já foi cadastrado no projeto. Se não foi faz inclusão. Se foi faz update.
	global $db;
	//dbg(1,1);
	$sql = "Select usucpf from monitora.projeto_ator where usucpf='$usu' and pjeid=$pje";
	if ($db->pegaUm($sql))
	{
		$sql ="update monitora.projeto_ator set peainfluencia='$influ',peainteresse='$inter',peastatus='A' where usucpf='$usu' and pjeid=$pje";
		
		$saida = $db->executar($sql);
	}
	else 
	{
		$sql ="insert into monitora.projeto_ator (pjeid,usucpf,peainfluencia,peainteresse) values ($pje,'".$usu."','$influ','$inter')";
		$saida = $db->executar($sql);
		// verifica se o usuário está cadastrado no módulo de projetos especiais. se não estiver, o cadastra com perfil de consulta.
		$sql = "select usucpf from seguranca.usuario_sistema where sisid=".$_SESSION['sisid']." and usucpf='$usu'";
		if ( ! $db->pegaUm($sql))
		{
			// então o usuário não foi cadastrado ainda
			$sql ="insert into seguranca.usuario_sistema (sisid,usucpf,susstatus,pflcod) values (".$_SESSION['sisid'].",'$usu','A',59)";
            $saida = $db->executar($sql);
            // agora insere em perfil_usuario
            $sql ="insert into seguranca.perfilusuario (usucpf,pflcod) values ('$usu',59)";
            $saida = $db->executar($sql);
            // agora insere em usuarioresponsabilidade
            $sql ="insert into monitora.usuarioresponsabilidade (pjeid,usucpf,pflcod,prsano) values ($pje,'$usu',59,'".$_SESSION['exercicio']."')";
            $saida = $db->executar($sql);            
		}
	}
    
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
  // atualização do cadastro de atores
  //print_r();
  $sql = "update monitora.projeto_ator set peastatus='I' where pjeid=$pjeid and usucpf not in (select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and pjeid=$pjeid)";
  $db->executar($sql);
  $usucpf = $_POST['usucpf'];
  $nlinhas = count($usucpf)-1;
  for ($j=0; $j<=$nlinhas;$j++)
  {    	
  	 atualiza_ator($pjeid,$_POST['usucpf'][$j],$_POST['infl'.$_POST['usucpf'][$j]],$_POST['inter'.$_POST['usucpf'][$j]]);
  }
  $db -> commit();
  $db->sucesso($modulo);   
}

if ($_REQUEST['act']== 'incluinovo')
{
  // montagem do cadastro de atores
  $usucpf = $_POST['usucpf'];
  $nlinhas = count($usucpf)-1;
  for ($j=0; $j<=$nlinhas;$j++)
  {  
  	 atualiza_ator($pjeid,$_POST['usucpf'][$j],$_POST['infl'.$_POST['usucpf'][$j]],$_POST['inter'.$_POST['usucpf'][$j]]);  	 
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
<input type='hidden' name='busca' value='0'>
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
<?
if ($_REQUEST['act']=='inclui')
{
	// novo ator
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	     <tr>
        <td align='right' class="SubTituloDireita">Entre com todo ou parte do nome do Ator<br>Pressione "Buscar".</td>
	    <td><?=campo_texto('nomeator','S','S','',77,100,'','');?><input type="button" name="btbuscar" value="Buscar" onclick="busca_ator()" class="botao"></td>
      </tr>
      </table>
      <table  border="1" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr align="center">
		<td></td><td><b>Influência</td><td><b>Interesse</td><td><b>Ator</td></tr>
<?
if ($_REQUEST['busca']=='1')
{
	
 $sql = "SELECT DISTINCT
 			u.usucpf AS CODIGO,
 			u.usunome||' - '||
 			u.usufoneddd||'-'||
 			u.usufonenum || ' - '||
 			ee.entnome||' - '||
 			un.unidsc AS DESCRICAO, 
 			u.usunome,
 			pa.peainfluencia AS influ,
 			pa.peainteresse AS inter 
 		 FROM 
 		 	seguranca.usuario u 
 		 INNER JOIN
 		 	entidade.entidade ee ON ee.entid = u.entid 
 		 LEFT JOIN 
 		 	unidade un ON un.unicod = u.unicod 
 		 LEFT JOIN 
 		 	monitora.projeto_ator pa ON pa.pjeid = " . $_SESSION['pjeid'] . " AND 
 		 								pa.peastatus = 'A' AND 
 		 								pa.usucpf = u.usucpf 
		 WHERE 
		 	u.usucpf NOT IN ( SELECT 
		 					  	usucpf 
		 					  FROM 
		 					  	monitora.projeto_ator
		 					  WHERE 
		 					  	pjeid = $pjeid AND 
		 					  	peastatus = 'A') AMD 
		 	u.suscod='A' AND 
		 	usunome ILIKE '%" . trim($_REQUEST['nomeator']) . "%' 
		 ORDER BY 
		 	usunome";
$rs = @$db->carregar( $sql );

if (  $rs && count($rs) > 0 )
{
	foreach ( $rs as $linha )
	{
		$ok=0;
		foreach($linha as $k=>$v) ${$k}=$v;
		if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
        $sql = "select rpuid from monitora.usuarioresponsabilidade where usucpf='$codigo' and pjeid=$pjeid and pflcod=51";
        $ok=$db->pegaUm($sql);        
		?>
		<tr bgcolor='<?=$marcado?>' onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" ><td><input type="checkbox" name="usucpf[]" value='<?=$codigo."'";
		if ($ok) {print " checked";}?>>
				</td>
		<td><input type="radio" name="infl<?=$codigo?>" value="B" <?=($influ=='B'?"CHECKED":"")?>>  Baixa<br><input type="radio" name="infl<?=$codigo?>" value="A" <?=($influ=='A'?"CHECKED":"")?>> Alta<br><input type="radio" name="infl<?=$codigo?>" value="T" <?=($influ=='T'?"CHECKED":"")?>> Total</td>
		<td><input type="radio" name="inter<?=$codigo?>" value="D" <?=($inter=='D'?"CHECKED":"")?>>  Direto<br><input type="radio" name="inter<?=$codigo?>" value="I" <?=($inter=='I'?"CHECKED":"")?>> Indireto</td>
		</td><td><?=$descricao?></td></tr><?
	}
}
else {
	?>
	<tr><td colspan='4'><font color="Red">Nenhum nome atendeu aos critérios de busca ou já está cadastrado!</font></td></tr>
	<?
}
?>
      <tr bgcolor="#CCCCCC">
        <td colspan="4"><?if ($coordpje)
             
        {   //Caso o usuário possa alterar faça
        	?>
              <input type="button" name="btincluir" value="Confirmar inclusões" onclick="validar_cadastro()" class="botao"><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao"></td><?}?>
      </tr> 
      <?    

}
?>
      </table>
      <?
}
?>
<table  border="1" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr align="center">
<td colspan="4">
<b> LISTA DE USUÁRIOS INCLUÍDOS NO CADASTRO DE ATORES ENVOLVIDOS NO PROJETO
</b>
</td>
</tr>
<tr align="center">
<td></td><td><b>Influência</td><td><b>Interesse</td><td><b>Ator</td></tr>
<?

 $sql = "SELECT DISTINCT 
 			u.usucpf AS CODIGO,
 			u.usunome||' - '||
 			u.usufoneddd||'-'||
 			u.usufonenum || ' - '||
 			ee.entnome||' - '||
 			un.unidsc AS DESCRICAO, 
 			u.usunome, 
 			pa.peainfluencia AS influ, 
 			pa.peainteresse AS inter 
 		 FROM 
 		 	seguranca.usuario u 
 		 INNER JOIN 
 		 	entidade.entidade ee ON ee.entid = u.entid 
 		 LEFT JOIN 
 		 	unidade un ON un.unicod = u.unicod
 		 INNER JOIN 
 		 	monitora.projeto_ator pa ON pa.pjeid = " . $_SESSION['pjeid'] . " AND 
 		 								pa.peastatus = 'A' AND 
 		 								pa.usucpf = u.usucpf  
 		 ORDER BY 
 		 	usunome";
  
$rs = @$db->carregar( $sql );

if (  $rs && count($rs) > 0 )
{
	foreach ( $rs as $linha )
	{
		$ok=0;
		foreach($linha as $k=>$v) ${$k}=$v;
		if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
        $sql = "select usucpf from monitora.projeto_ator where usucpf='$codigo' and pjeid=$pjeid";
        $ok=$db->pegaUm($sql);        
		?>
		<tr bgcolor='<?=$marcado?>' onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
		<td><input type="checkbox" name="usucpf[]" value='<?=$codigo."'";?>
		<?
		   if ($ok) {print " checked";}
		?>
		></td>
		<td><input type="radio" name="infl<?=$codigo?>" value="B" <?=($influ=='B'?"CHECKED":"")?>>  Baixa<br><input type="radio" name="infl<?=$codigo?>" value="A" <?=($influ=='A'?"CHECKED":"")?>> Alta<br><input type="radio" name="infl<?=$codigo?>" value="T" <?=($influ=='T'?"CHECKED":"")?>> Total</td>
		<td><input type="radio" name="inter<?=$codigo?>" value="D" <?=($inter=='D'?"CHECKED":"")?>>  Direto<br><input type="radio" name="inter<?=$codigo?>" value="I" <?=($inter=='I'?"CHECKED":"")?>> Indireto</td>
		<td><?=$descricao?></td></tr><?
	}
}

?>
<tr bgcolor="#CCCCCC">
        <td></td>
        <td colspan='3'><?if ($coordpje)
             
        {   //Caso o usuário possa alterar faça
        	?>
              <input type="button" name="btincluir" value="Incluir novo Ator" onclick="incluir_ator()" class="botao"><input type="button" name="btalterar" value="Alterar cadastro atual" onclick="altera_cadastro()" class="botao"><input type="button" name="btvoltar" value="Voltar" onclick="history.back()" class="botao"></td><?}?>
      </tr>      

   </table>
</center>
</form>



<script>
function incluir_ator()
{
		document.formulario.act.value = 'inclui';
		document.formulario.submit();
}
function busca_ator()
{
	if (!validaBranco(document.formulario.nomeator, 'Nome ou parte do nome!')) return;
		document.formulario.busca.value = '1';
		document.formulario.act.value = 'inclui';		
		document.formulario.submit();

}
function validar_cadastro()
{
	if( window.confirm( "Confirma a lista de atores conforme os nomes marcados?") )
	{
		document.formulario.act.value = 'incluinovo';
		document.formulario.submit();
	} 
}
function altera_cadastro()
{
	if( window.confirm( "Confirma a lista de atores conforme os nomes marcados?") )
	{
		document.formulario.act.value = 'altera';
		document.formulario.submit();
	} 
}
  function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }
</script>
