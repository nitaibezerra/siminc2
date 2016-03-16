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
//$pjeid = md5_decrypt($_REQUEST['id'],'');

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
$data_ini = $_REQUEST['data_ini'];
$data_final = $_REQUEST['data_final'];
$grau = $_REQUEST['grau'];

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
     
<?
   $sql = "select distinct pfl.pflcod,pfl.pfldsc as descricao,pfl.pflsncumulativo as mostra,usu.usucpf, usu.usuemail, usu.usunome || ' ('|| ee.entnome ||')' as usuario,usu.usufoneddd||'-'||usu.usufonenum as fone from seguranca.perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = ".$_SESSION['pjeid']." and rpu.rpustatus='A' left join seguranca.usuario usu on usu.usucpf=rpu.usucpf left join entidade.entidade ee on ee.entid = usu.entid where   pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E') order by pfl.pflcod";

   //pfl.pflcod<>12 and

    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($i=0; $i<=$nlinhas;$i++){
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            if ($usuario ){
	        $linha = "<tr><td align='right' class='SubTituloDireita'>".$descricao.":</td><td>".$usuario.' Telefone:'.$fone;
	        if ($usucpf <> $_SESSION['usucpf'])
	        {
	        	$linha .= '&nbsp;&nbsp;&nbsp;<img src="../imagens/email.gif" title="Envia e-mail" border="0" onclick="envia_email(\''.$usucpf.'\');"> ';
	        }
	        $linha .= "</td></tr>";
            print $linha;
            }
        }
     }     
?>
<tr>
</table>
<br> 
<script language="JavaScript" src="../includes/calendario.js"></script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr align="center">
<td colspan="2">
<b>MONTE A EQUIPE DE SEU PROJETO</b>
</td>
</tr>
<tr>
     <td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">Período considerado:</td><td>
     <?=campo_data('data_ini', 'S','S','','S').' a '.campo_data('data_final', 'S','S','','S');?>
     </td>
</tr>
<tr>
<td align='right' class="SubTituloDireita" style="height:1px;padding:0px;">
Grau de Comprometimento
</td>
<td >
	<input type="radio" name="grau" value="3" <? if ($grau==3) print ' checked'?>>Indiferente
	<input type="radio" name="grau" value="1" <? if ($grau==1) print ' checked'?>>50% disponível
<input type="radio" name="grau" value="2" <? if ($grau==2) print ' checked'?>>100% disponível
</td>
</tr>
<tr >
<td colspan="2" align='center' style="height:1px;padding:0px;">
<input type="button" name="btflitrar" value="Buscar" onclick="filtraequipe()">
</td>
</tr>
</table>


<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr align="center">
<td colspan="2">
<b> LISTA DE USUÁRIOS COM PERFIL DE EQUIPE DE APOIO A PROJETO ESPECIAL
</b>
</td>
</tr>
<?
if ($_REQUEST['act']== 'filtra')
{
  // montagem da equipe
   $sql= "select distinct u.usucpf as CODIGO,u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum || ' - '||ee.entnome||' - '||un.unidsc   as DESCRICAO, u.usunome from seguranca.usuario u inner join entidade.entidade ee on ee.entid = u.entid left join unidade un on un.unicod=u.unicod left join monitora.usuarioresponsabilidade ur on ur.usucpf=u.usucpf where  ur.rpustatus ='A' ";
   if ($_REQUEST['grau']==2 or $_REQUEST['grau']==1)
   {
   	  $sql.= " and ur.rpucompromisso=".$_REQUEST['grau'];
   }
   $sql .= " and (ur.rpudata_ini >= '".$_REQUEST['data_ini']."' or ur.rpudata_ini is null) and (ur.rpudata_fim <='".$_REQUEST['data_final']."' or ur.rpudata_fim is null) and u.usucpf in (select pu.usucpf from seguranca.perfilusuario pu where pu.pflcod=51) 
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

function filtraequipe()
{
	// checa as datas
	if (!validaData(document.formulario.data_ini))
	{
		alert("Data Inicial Inválida.");
		document.formulario.data_ini.focus();
		return;
	}
	// a data inicial é válida. Tenho que verificar se ela é menor ou igual a de início do projeto
	if (!validaDataMaior(document.formulario.data_ini , document.formulario.data_final))
	{
		alert("Data Inicial não pode ser Anterior que Data Inicial do Projeto.");
		document.formulario.data_ini.focus();
		return;
	}	

	//data existe e é válida, nesta caso tem de existir uma data fim
	if (!validaBranco(document.formulario.data_final, 'Data Final')) return;
	// a data fim exite. verifico se é válida
	if (!validaData(document.formulario.data_final))
	{
		alert("Data Final Inválida.");
		document.formulario.data_final.focus();
		return;
	}

	// a data fim é válida. Tenho que verificar se ela é menor ou igual a de início.
	if (!validaDataMaior(document.formulario.data_ini , document.formulario.data_final))
	{
		alert("Data Final não pode ser Anterior que Data Inicial.");
		document.formulario.data_final.focus();
		return;
	}	

	if ( !validar_radio( document.formulario.grau, 'Grau de comprometimento' ) ) {
			alert("Grau de Comprometimento é obrigatório.");
			document.formulario.grau.focus();
			return;
		}

  document.formulario.act.value = 'filtra';
  document.formulario.submit();
}
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }

</script>
