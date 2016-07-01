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
		
  
  /*
    $sql= "select pjecod from monitora.projetoespecial where (pjestatus='A') and (pjecod='".str_to_upper($_REQUEST['pjecod'])."' and (ungcod='".$_REQUEST['ungcod']."') or pjedsc='".str_to_upper($_REQUEST['pjedsc'])."')";

    $usu = $db->recuperar($sql);
	if (is_array($usu)) {
	   // existe projeto identico, logo, tem que bloquear
	   $sql="select ungabrev from unidadegestora where ungcod='".$_REQUEST['ungcod']."'";
	   $ungcod=$db->pegaUm($sql);
	   ?>
	      <script>
	         alert ('O Projeto: <?=$ungabrev.'-'.$_REQUEST['pjecod']?> já se encontra cadastrado com o mesmo código ou o mesmo nome no sistema. Os nomes dos projetos não podem ser repetidos.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
	   if (! $_REQUEST['pjeprevistoano']) $_REQUEST['pjeprevistoano']=0;
	   if (! $_REQUEST['pjevlrano']) $_REQUEST['pjevlrano']=0;	   
	   if (! $_REQUEST['procod']) $_REQUEST['procod']=0;	   
	   if (! $_REQUEST['unmcod']) $_REQUEST['unmcod']=0;
	   if (! $_REQUEST['usucpfcoord'] ) $_REQUEST['usucpfcoord']=null;
	   if (! $_REQUEST['pjesndatafechada'] ) $_REQUEST['pjesndatafechada']='f';
	   if (! $_REQUEST['pjesnvisivel'] ) $_REQUEST['pjesnvisivel']='f';	   
	   		   
        $sql = "insert into monitora.projetoespecial (pjestatus,pjecod,pjedsc,prsano,procod,unmcod,pjeprevistoano,pjevlrano,pjedescricao,pjefinalidade,ungcod,pjedataini,pjedatafim,pjesndatafechada,pjesnvisivel, tpscod) values ('A',".
   "'".str_to_upper($_REQUEST['pjecod'])."',".
   "'".str_to_upper($_REQUEST['pjedsc'])."','".$_SESSION['exercicio']."',".$_REQUEST['procod'].",".$_REQUEST['unmcod'].",".$_REQUEST['pjeprevistoano'].",".$_REQUEST['pjevlrano'].",'".$_REQUEST['pjedescricao']."','".$_REQUEST['pjefinalidade']."','".$_REQUEST['ungcod']."','".$_REQUEST['pjedataini']."','".$_REQUEST['pjedatafim']."','".$_REQUEST['pjesndatafechada']."','".$_REQUEST['pjesnvisivel']."',10)";
       $saida = $db->executar($sql);
       	$sql =  "Select pjeid from monitora.projetoespecial where oid = ".pg_last_oid($saida);
	    $pjeid = $db->pegaUm($sql);
       
       atualiza_pto($pjeid, $_REQUEST['ptocod'] );
       atualiza_acao($pjeid, $_REQUEST['acacod'] ); 
       atualiza_altagestao($pjeid, $_REQUEST['usucpf'] ); 
       // cria registro em usuárioresponsabilidade
       $sql ="insert into monitora.usuarioresponsabilidade (pjeid,pflcod,usucpf,prsano) values ($pjeid,12,'".$_SESSION['usucpf']."','".$_SESSION['exercicio']."')";
       $saida = $db->executar($sql);       
       $db->commit();

    if ($_REQUEST['usucpfcoord'] and $_REQUEST['usucpfcoord'] <>null) {    	
    	// inclui o usuário na tabela de usuário responsabilidade
    		$sql = "insert into monitora.usuarioresponsabilidade (pjeid,usucpf,pflcod,prsano) values ($pjeid,'".$_REQUEST['usucpfcoord']."',47,'".$_SESSION['exercicio']."')";
    		$db->executar($sql);
    		// verifica se o usuario possui o perfil 47 na tabela perfilusuario
    		$sql ="select usucpf from seguranca.perfilusuario where pflcod=47 and usucpf='".$_REQUEST['usucpfcoord']."'";
    		if (! $db->pegaUm($sql))
    		{
    			// insere
    			$sql ="insert into seguranca.perfilusuario (pflcod,usucpf) values (47,'".$_REQUEST['usucpfcoord']."')";
    			$db->executar($sql);
    		}
    	 	$db->commit();
    	 	
    // envia email para o coordenador avisando o ocorrido
		$sql="select ug.ungabrev,p.pjecod ,p.pjedsc from monitora.projetoespecial p inner join unidadegestora ug on ug.ungcod=p.ungcod where p.pjeid=".$pjeid;
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;       
         // envia email
        $assunto = 'Inclusão como responsável em Projeto Especial';
		$sexo = 'Prezado Sr.  ';
		$sql="select ususexo,usunome,usuemail from seguranca.usuario where usucpf='".$_REQUEST['usucpfcoord']."'";
		$res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
		if ($ususexo == 'F') $sexo = 'Prezada Sra. ';
		
        $mensagem = $sexo. str_to_upper($usunome).chr(13)."Reportamos que seu nome foi associado, no SIMEC, como responsável do Projeto Especial ".$ungabrev.$pjecod.' - '.$pjedsc;
        email($usunome, $usuemail, $assunto, $mensagem);
    } 
    */      
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
     
<?
   $sql = " SELECT DISTINCT 
   		   		pfl.pflcod,
   		   		pfl.pfldsc AS descricao,
   		   		pfl.pflsncumulativo AS mostra,
   		   		usu.usucpf, 
   		   		usu.usuemail, 
   		   		usu.usunome || ' ('|| ee.entnome ||')' AS usuario,
   		   		usu.usufoneddd||'-'||usu.usufonenum AS fone 
   		    FROM 
   		    	seguranca.perfil pfl 
   		    LEFT JOIN 
   		    	monitora.usuarioresponsabilidade rpu ON rpu.pflcod = pfl.pflcod AND 
   		    											rpu.pjeid = " . $_SESSION['pjeid'] . " AND 
   		    											rpu.rpustatus = 'A' 
			LEFT JOIN 
				seguranca.usuario usu ON usu.usucpf = rpu.usucpf 
			LEFT JOIN 
				entidade.entidade ee ON ee.entid = usu.entid 
			WHERE 
				pfl.pflstatus = 'A' AND 
				pfl.pflresponsabilidade IN ('E') 
			ORDER BY 
				pfl.pflcod";

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

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr align="center">
<td colspan="2">
<b> LISTA DE USUÁRIOS COM PERFIL DE EQUIPE DE APOIO A PROJETO ESPECIAL
</b>
</td>
</tr>
<?

 $sql = "SELECT DISTINCT 
 			u.usucpf AS CODIGO,
 			u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum || ' - '||
 			ee.entnome||' - '||un.unidsc AS DESCRICAO, 
 			u.usunome 
 		 FROM 
 		 	seguranca.usuario u 
 		 INNER JOIN 
 		 	entidade.entidade ee ON ee.entid = u.entid 
 		 LEFT JOIN 
 		 	unidade un ON un.unicod = u.unicod
		 WHERE 
		 	u.usucpf IN ( SELECT 
		 				  	pu.usucpf 
		 				  FROM 
		 				  	seguranca.perfilusuario pu 
		 				  WHERE 
		 				  	pu.pflcod = 51 ) OR 
		 	u.usucpf IN ( SELECT 
		 					us.usucpf 
		 				  FROM 
		 				  	seguranca.usuario_sistema us 
		 				  WHERE 
		 				  	us.pflcod = 51 ) AND 
		  	u.usustatus = 'A' 
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

</script>
