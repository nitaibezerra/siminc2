<? 
 /*
   Sistema Simec
   Setor respons·vel: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   MÛdulo:celulas.inc
   Finalidade: permitir o cadastro dos celulas
   */

########### A«’ES associaÁ„o de cÈlula ###########
$modulo=$_REQUEST['modulo'] ;
if ($_REQUEST['act2']=='associar' and $_POST['celid']<>'') {
  $sql = 'delete from demandas.sistemacelula where celid='.$_POST['celid'];

  $db->executar($sql);
  $sidid = $_POST['sidid'];
  $celid = $_POST['celid'];
  //print_r($_POST['sidid']);
  $nlinhas = count($sidid)-1;

	for ($j=0; $j<=$nlinhas;$j++)
		{
		  $sql = "insert into demandas.sistemacelula (celid, sidid) VALUES (".$celid.",".$_POST['sidid'][$j].")";
		 // print $_POST['sidid'][$j].'<br>';
		  $db->executar($sql);
		 }
  $db -> commit();
  //$db->sucesso($modulo);
    ?>
    	<script>
            alert('OperaÁ„o realizada com sucesso!');
		  	location.href='demandas.php?modulo=sistema/apoio/celulas&acao=A&celid=<?=$celid?>&op=update';
        </script>
    <?
	exit();  
}



########### A«’ES Cadastro/AtualizaÁ„o de cÈlula ###########
if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro))):
	
    $sql= "SELECT
  	  	   celid 
  		  FROM
  		   demandas.celula 
  		  WHERE
  		   celnome = '".$_REQUEST['dsc']."' ;";
   $usu = $db->recuperar($sql);
   unset($sql);

   if (is_array($usu)) {
	   // existe perfil identico, logo, tem que bloquear
	   ?>
	      <script>
	         alert ('A CÈlula: <?=$_REQUEST['celnome']?> j· est· cadastrado.');
	         history.back();
	      </script>
	   <?
	     exit();
	}


		   
///////// Cadastro do sistema //////////	   
    $sql = "INSERT INTO demandas.celula (celnome, ordid) VALUES (".
   		   "'".$_REQUEST['dsc']."',".$_REQUEST['ordid'].") RETURNING celid";
   $sisidNew = $db->pegaUm($sql);
   $db->commit();
   unset($sql);
   
    ?>
    	<script>
            alert('OperaÁ„o realizada com sucesso!');
		  	location.href='demandas.php?modulo=sistema/apoio/celulas&acao=A';
        </script>
    <?
	exit();
endif;

if ( ($_REQUEST['act']=='alterar') and (! is_array($msgerro)) ):
   // fazer alteraÁ„o de perfil na base de dados.
  	$sql = "UPDATE 
   			 demandas.celula 
   			SET 
   			 ordid 		= '".$_REQUEST['ordid']."',
		   	 celnome 		= '".$_REQUEST['dsc']."',
		  	 celstatus 	= '".$_REQUEST['status']."'
		  	WHERE
		  	 celid = ".$_POST['sisidEdit'];
    $saida = $db->executar($sql);
	$db->commit();
	?>
    	<script>
            alert('OperaÁ„o realizada com sucesso!');
		   	location.href='demandas.php?modulo=sistema/apoio/celulas&acao=A';
        </script>
    <?
	exit();
endif;

if ( ($_REQUEST['act']=='excluir') and (! is_array($msgerro)) ):
   // fazer exclus„o de perfil na base de dados.
   	
	$sql1 = "SELECT u.celid 
			 FROM demandas.usuarioresponsabilidade AS u
			 LEFT JOIN demandas.celula AS d ON u.celid = d.celid 
			 WHERE u.celid = ". $_REQUEST['celid'];
	 
	$carrega = $db->carregar($sql1);
 	 
	if(count( $carrega ) > 1 )
	{
		?>
		<script >
			alert('OperaÁ„o n„o pode ser realizada devido a dados vinculados com usuario');
			location.href = 'demandas.php?modulo=sistema/apoio/celulas&acao=A';
		</script>
		<? 
	}else{
     
    $sql  = "DELETE FROM demandas.celula WHERE celid = ".$_REQUEST['celid'];
    $saida = $db->executar( $sql );
    $db->commit();
	?>
    	<script>
            alert('OperaÁ„o realizada com sucesso!');
		   	location.href='demandas.php?modulo=sistema/apoio/celulas&acao=A';
        </script>
    <?
	}
	exit();
endif;

include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Cadastro de CÈlula';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo ObrigatÛrio.');
?>
<div align="center">
<center>

<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//
//$celid = md5_decrypt($_REQUEST['sisidEdit'],'');
$celid = $_REQUEST['celid'];

if ($celid):
	$sql= "SELECT
			 celid,
			 celnome as dsc,
			 celstatus AS status,
			 ordid
		   FROM
		     demandas.celula c
		   WHERE 
		     celid = ".$celid."
		   ORDER BY
		     celnome";
	$dados = (array) $db->carregar($sql);
	
	#### Transforma $dados[0] em Vari·veis ####
	extract($dados[0]);
	
else:
	$celid 		   = "";
	$dsc   		   = "";
	$status        = "A";
endif;
	$act = '';
	
?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="sisidEdit" value="<?=$celid?>">
<input type='hidden' name="act" value=<?=$act?>>
    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	  <tr >
		<td align='right' class="subtitulodireita">Origem:</td>
		<td>
			<?php
				$sql = "SELECT
						 ordid AS codigo,
						 orddescricao AS descricao
						FROM
						 demandas.origemdemanda WHERE ordstatus = 'A' 
						ORDER BY
						 orddescricao;";
				$ordid = $ordid ? $ordid : $_REQUEST['ordid'];
				$db->monta_combo("ordid",$sql,'S',"-- Informe a Origem da demanda --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	  </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Nome:</td>
	     <td>
	     	<?=campo_texto('dsc','S','S','',50,100,'','');?>
	     </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Status:</td>
	       <td>
            <input type="radio" name="status" value="A" <?=($status=='A'?"CHECKED":"")?>> Ativo
            <input type="radio" name="status" value="I" <?=($status=='I'?"CHECKED":"")?>> Inativo
        </td>
      </tr>
<? if ($celid) { ?>
	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
   		<input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
 	</tr>
<? } else { ?>
	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao">
   		<input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao"></td>
	</tr>
<? } ?>
      
    </table>

  
</form>


<? 

//exibe listagem dos celulas cadastrados
$sql= "select 
		'<div align=center><a href=\"demandas.php?modulo=sistema/apoio/celulas&acao=A&celid=' || c.celid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/celulas&acao=A&celid=' || c.celid || '&act=excluir\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>' AS op,
		c.celid || ' ' as codigo2, 
		c.celnome as descricao2,
		u.usunome, 
		CASE
			WHEN c.celstatus = 'I' THEN '<font color=red>Inativo</font>' 
			WHEN c.celstatus = 'A' THEN '<font color=blue>Ativo</font>'
		END as status2
 		from demandas.celula c
 			LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
 			LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
 		order by celnome ";
$cabecalho = array("AÁ„o","CÛd","CÈlula","Gerente","Status");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');

?>



<script>

function strSpecial(string){
	var search = '·‚‡„¡¬¿√ÈÍË… »ÌÓÏÕŒÃÛıÚı”‘Ã’˙˚˘⁄€Ÿ!@#$%®&*()-+][}{~^¥`?:;,"';

	for (i = 0; i < search.length; i++){
		if ( string.indexOf(search.substr(i,1)) >= 0 ){
			alert('Esse caractere È inv·lido para o campo: " '+search.substr(i,1)+' "');
			return false;
		}		
	}
	return true;
}



function altera_cadastro(cod) {
    document.formulario.sisidEdit.value = cod;
    document.formulario.act.value = 'alterar';
    document.formulario.submit();
}



function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este item?")
	if (questao){
		window.location = url;
	}
}

function inicio() {
 	document.formulario.act.value = '';
 	location.href='demandas.php?modulo=sistema/apoio/celulas&acao=A';
}
  
function validar_cadastro(cod) {

	if (document.formulario.ordid.value == "") 
	{
		alert("Informe a Origem.");
		document.formulario.ordid.focus();
		return;
	}
	
	if (document.formulario.dsc.value == "") 
	{
		alert("Informe o Nome.");
		document.formulario.dsc.focus();
		return;
	}
	
	if ((document.formulario.status[0].checked == 0) && (document.formulario.status[1].checked == 0)) 
	{
		alert("Informe o Status.");
		document.formulario.status.focus();
		return;
	}
	
	
	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
    document.formulario.submit();

}    
</script>






<!-- LISTA ASSOCIA«√O -->


<br><br>
<form method="POST" name="formulario2">
<input type='hidden' name="celid" value="<?=$celid?>">
<input type=hidden name="act2" value=0>
<?
 if ($celid) {
	?>
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr><td ALIGN=right>
			<B>AssociaÁ„o de Sistemas para:</B>
<?
	  $sql = "select 
	  			--c.celid as CODIGO, 
	  			c.celnome || ' - Gerente: ' || 
				CASE 
					WHEN u.usunome != '' THEN u.usunome
					ELSE ' - '
				END as DESCRICAO  
				from demandas.celula c
	 			LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
	 			LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
	  		  where 
	  		  	c.celstatus='A'
	  		  	and
	  		  	C.CELID = ".$celid." 
	  		  order by c.celnome ";
	  echo $db->PegaUm($sql);
	?>	
		
		

		</td>
		<td align='right'>
			<input type='button' value='Inserir Novos Sistemas' onclick="inserirsis()">
		</td>
	</tr>
	</table>
 	<?
//teste utilizando a funÁ„o Monta Lista
$cabecalho = array('Sim/N„o','CÛdigo','<div align=left>Nome</div>');
//$sql = "select acacod, acadsc from acao";
/*
$sql= "select  
			case when c.sidid is null then '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||' \">' else '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||'\" checked>' end as acao, 
			s.sidid  as cod, 
			upper(s.sidabrev) || ' - ' || s.siddescricao as descricao 
		from 
			demandas.sistemadetalhe s
		left join demandas.sistemacelula c on s.sidid = c.sidid and c.celid=".$celid." 
		where  s.sidstatus = 'A' 
		order by s.sidabrev";
*/

/*
$sql= "select  
			case 
				when c.celid is null then '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||' \">' 
				when c.celid = {$celid} then '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||'\" checked>'
				else c1.celnome end as acao, 
			s.sidid  as cod, 
			upper(s.sidabrev) || ' - ' || s.siddescricao as descricao 
		from 
			demandas.sistemadetalhe s
		left join demandas.sistemacelula c on s.sidid = c.sidid and (c.celid={$celid} or c.celid is not null)
		left join demandas.celula c1 on c1.celid = c.celid  
		where  s.sidstatus = 'A' 
		order by s.sidabrev"; 
 */
/*
$sql= "select  
			case when c.sidid is null then '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||' \">' else '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||'\" checked>' end as acao, 
			s.sidid  as cod, 
			upper(s.sidabrev) || ' - ' || s.siddescricao as descricao 
		from 
			demandas.sistemadetalhe s
		left join demandas.sistemacelula c on s.sidid = c.sidid  
		where  s.sidstatus = 'A' 
		and c.celid={$celid}
		
		UNION
		
		select  
			case when c.sidid is null then '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||' \">' else '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||'\" checked>' end as acao, 
			s.sidid  as cod, 
			upper(s.sidabrev) || ' - ' || s.siddescricao as descricao 
		from 
			demandas.sistemadetalhe s
		left join demandas.sistemacelula c on s.sidid = c.sidid  
		where  s.sidstatus = 'A' 
		and c.celid IS NULL";
*/

$sql= "select  
			case when c.sidid is null then '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||' \">' else '<input type=\"Checkbox\" name=\"sidid[]\" value=\"'||s.sidid||'\" checked>' end as acao, 
			s.sidid  as cod, 
			upper(s.sidabrev) || ' - ' || s.siddescricao as descricao 
		from 
			demandas.sistemadetalhe s
		left join demandas.sistemacelula c on s.sidid = c.sidid  
		where  s.sidstatus = 'A' 
		and c.celid={$celid}
		order by 3";


//print $sql;
$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr  bgcolor="#CCCCCC"><td colspan=2><input type='button' value='Alterar' onclick="validar_associa()"></td></tr>
</table>
<?}?>
</form>
<script language="JavaScript">

  function validar_associa() {

    document.formulario2.act2.value = 'associar';
	document.formulario2.submit();
  }
  
  function inserirsis() {
    window.open( 'demandas.php?modulo=sistema/apoio/associa_celula&acao=A&celid='+<?=$celid?>, 'popsis', 'width=580,height=360,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
  }  
</script>




