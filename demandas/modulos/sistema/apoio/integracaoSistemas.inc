<? 
 /*
   Sistema Simec
   Setor responsável: CTI-MEC
   Desenvolvedor: Equipe Simec
   Analista: Térzio Vieira
   Programador: Wesley Silva
   Módulo:sistemas.inc
   Finalidade: permitir o cadastro de integração de sistemas
   */

if ( $_POST['act'] == 'inserir' ):
	
    $sql= "SELECT
  	  	   	count(isiid) as total
  		  FROM
  		   	demandas.sistemaintegracao 
  		  WHERE
  		   	sididsistema1 = '".$_POST['sididsistema1']."'
  		   	and sididsistema2 = '".$_POST['sididsistema2']."'
  		   	and isistatus = 'A'";
    
   $total = $db->pegaUm($sql);
   
   
   unset($sql);
	
   if ($total > 0) {
	   ?>
	      <script>
	         alert ('A integração entre esses sistemas ja foi cadastrada!.');
	         history.back();
	      </script>
	   <?
	     exit();
	} else {
		$sql = "INSERT INTO demandas.sistemaintegracao (   		    
	    			sididsistema1,
	    			sididsistema2,
	 				tisid,  
	 				isistatusleitura,
	 				isistatusescrita, 			
	    			isidsc,
	    			isistatus
	    			) 
	    			VALUES (".
			   "'".$_REQUEST['sididsistema1']."',".
			   "'".$_REQUEST['sididsistema2']."',".
			   "'".$_REQUEST['integracao']."',".   
			   "'".$_REQUEST['leitura']."',".
	   		   "'".$_REQUEST['escrita']."',".
	    	   "'".$_REQUEST['isidsc']."', 'A')";
		
	   $db->executar($sql);
	   $db->commit();
	   $db->sucesso('sistema/apoio/integracaoSistemas');
	   unset($sql);		
	}
	exit();
endif;

if ( ($_REQUEST['act']=='alterar') and (! is_array($msgerro)) ):
   // fazer alteração de perfil na base de dados.
  	$sql = "UPDATE 
   			 demandas.sistemaintegracao 
   			SET 
   			 sididsistema1      = '".$_REQUEST['sididsistema1']."',
   			 sididsistema2      = '".$_REQUEST['sididsistema2']."',   			 
		  	 tisid 				= '".$_REQUEST['integracao']."',
		  	 isistatusleitura   = '".$_REQUEST['leitura']."',
		  	 isistatusescrita   = '".$_REQUEST['escrita']."',		  	 
		  	 isidsc  			= '".$_REQUEST['isidsc']."'

		  	WHERE
		  	 isiid = ".$_POST['isiidEdit'];
  	
    $db->executar($sql);
	$db->commit();
	$db->sucesso('sistema/apoio/integracaoSistemas');
	exit();
endif;

if ( ($_REQUEST['act']=='excluir') ):
   // fazer exclusão de perfil na base de dados.
     
    $sql  = "DELETE FROM demandas.sistemaintegracao WHERE isiid = ".$_REQUEST['isiid'];
    $db->executar( $sql );
    $db->commit();
    $db->sucesso('sistema/apoio/integracaoSistemas');
	exit();
endif;

include APPRAIZ."includes/cabecalho.inc";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Cadastro de Integrações de Sistemas';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>
<?
//atribui valores as variaveis
$modulo=$_REQUEST['modulo'] ;//
//$sidid = md5_decrypt($_REQUEST['isiidEdit'],'');
$isiid = $_REQUEST['isiid'];

if ($isiid):
	$sql= "SELECT
			 isiid, 
			 isidsc AS isidsc,
			 sididsistema1 as codsistema1,
			 sididsistema2 as codsistema2,
			 isistatusleitura as leitura,
			 isistatusescrita as escrita,
			 tisid as integracao 
		   FROM
		     demandas.sistemaintegracao isi		     
		   WHERE 
		     isiid = ".$isiid."
		   ";
	$dados = $db->carregar($sql);
	
	#### Transforma $dados[0] em Variáveis ####
	extract($dados[0]);
	
	#### Recupera nome do Sistema sistema1 ####	
	$sql= "SELECT
			 sidid||' - '||siddescricao
		   FROM
		     demandas.sistemadetalhe sid		     
		   WHERE 
		     sidid = ".$codsistema1;
	
	$dadossistema1 = $db->pegaUm($sql);
	$sididsistema1 = array("value" => $codsistema1, "descricao" => $dadossistema1);
	
	#### Recupera nome do Sistema sistema2 ####	
	$sql= "SELECT
			 sidid||' - '||siddescricao 
		   FROM
		     demandas.sistemadetalhe sid		     
		   WHERE 
		     sidid = ".$codsistema2;
	
	$dadossistema2 = $db->pegaUm($sql);
	$sididsistema2 = array("value" => $codsistema2, "descricao" => $dadossistema2);

else:
		
	$isiid 		   = "";
	$sistema1	   = "";
	$sistema2	   = "";
	$integracao    = "";
	$isidsc  	   = "";
	$leitura	   = "";
	$escrita       = "";
	$status        = "A";
	
endif;
	$act = '';
	
?>

<form method="post"  name="formulario" id="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="isiidEdit" value="<?=$isiid?>">
<input type='hidden' name="act" value=<?=$act?>>
    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <!-- 
      <tr>
        <td align='right' class="SubTituloDireita">Código:</td>
	    <td><?//=campo_texto('sisid','S',($sisid != ""?'N':'S'),'',5,5,'','');?></td>
      </tr>
      -->
      <tr>
        <td align='right' class="SubTituloDireita">Sistema 1:</td>
		<td>
			<?
			$sql = "SELECT 
					  	sidid as codigo,
					  	sidabrev as descricao
					FROM 
					  	demandas.sistemadetalhe
					WHERE
						sidstatus = 'A'";
	
			campo_popup('sididsistema1',$sql,'Selecione','','400x800','100', array(
																	array("codigo" => "sidid",
																		  "descricao" => "<b>Código:</b>","numeric" => "1"),
																	array("codigo" => "sidabrev",
																		  "descricao" => "<b>Descrição:</b>","string" => "1")
																	)
																	);
			?>			
		</td>

      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Sistema 2:</td>
	    <td>
	    <?
			$sql = "SELECT 
					  	sidid as codigo,
					  	sidabrev as descricao
					FROM 
					  	demandas.sistemadetalhe
					WHERE
						sidstatus = 'A'";
	
			campo_popup('sididsistema2',$sql,'Selecione','','400x800','100', array(
																	array("codigo" => "sidid",
																		  "descricao" => "<b>Código:</b>","numeric" => "1"),
																	array("codigo" => "sidabrev",
																		  "descricao" => "<b>Descrição:</b>","string" => "1")
																	)
																	);
			?>
	    </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Tipo Integração:</td>
	    <td><?php
  	
	    	
	    	$sql = "select 	tisid as codigo, 
               				tisdsc as descricao
						from demandas.sistematipointegracao
						order by descricao";	
	    	                             	    
			$db->monta_combo('integracao', $sql, 'S', "Selecione...", 'filtraPlataforma', '', '', '200', 'S', 'integracao',false,null,'Integracao');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Leitura:</td>
	       <td>
            <input type="radio" name="leitura" value="S" <?=($leitura=='S'?"CHECKED":"")?>> Sim
            <input type="radio" name="leitura" value="N" <?=($leitura=='N'?"CHECKED":"")?>> Não
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Escrita:</td>
	       <td>
            <input type="radio" name="escrita" value="S" <?=($escrita=='S'?"CHECKED":"")?>> Sim
            <input type="radio" name="escrita" value="N" <?=($escrita=='N'?"CHECKED":"")?>> Não
        </td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
	    <td><?=campo_textarea('isidsc','S','S','',75,3,'');?></td>
      </tr>
      <!--  <tr>
        <td align='right' class="SubTituloDireita">Tabelas de segurança:</td>
	       <td>
            <input type="radio" name="tblseg" <?=$sisid ? 'disabled="disabled"' : '';?> value="A" <?=($tblseg=='A'?"CHECKED":"")?>> Ativo
            <input type="radio" name="tblseg" <?=$sisid ? 'disabled="disabled"' : '';?> value="I" <?=($tblseg=='I'?"CHECKED":"")?>> Inativo
        </td>
      </tr>-->
<? if ($isiid) { ?>
	<tr bgcolor="#CCCCCC">
   		<td></td>
   		<td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
   		    <input type="button" name="btcancela" value="Cancelar" onclick="inicio();" class="botao">
   		</td>
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

//exibe listagem dos sistemas cadastrados
$sql= "select 
		'<div align=center><a href=\"demandas.php?modulo=sistema/apoio/integracaoSistemas&acao=A&isiid=' || isiid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/integracaoSistemas&acao=A&isiid=' || isiid || '&act=excluir\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>' AS op,
		sid1.sidabrev || ' - ' || sid1.siddescricao as sistema1,
		sid2.sidabrev || ' - ' || sid2.siddescricao as sistema2,
		tisdsc as tipointegracao, 
	    CASE WHEN upper(isistatusleitura) = 'S' THEN 'Sim'
	        WHEN upper(isistatusleitura) = 'N' THEN 'Não'
	    ELSE 'N/A' END AS leitura2,
	    CASE WHEN upper(isistatusescrita) = 'S' THEN 'Sim'
	        WHEN upper(isistatusescrita) = 'N' THEN 'Não'
	    ELSE 'N/A'
			   END AS escrita2,						   
		isidsc as descricao2 
 		from demandas.sistemaintegracao isi
 		inner join demandas.sistematipointegracao tis on isi.tisid = tis.tisid
 		inner join demandas.sistemadetalhe sid1 on isi.sididsistema1 = sid1.sidid
 		inner join demandas.sistemadetalhe sid2 on isi.sididsistema2 = sid2.sidid 
 		order by isiid ";
$cabecalho = array("Ação","Sistema 1","Sistema 2","Tipo Integração","Leitura","Escrita", "Descrição");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '', '');

?>


<script type="text/javascript">

	var d=document;

function altera_cadastro(cod) {
    document.formulario.isiidEdit.value = cod;
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
 	window.location.href='demandas.php?modulo=sistema/apoio/integracaoSistemas&acao=A';
}
  
function validar_cadastro(cod) {

	if (document.formulario.sididsistema1.value == "") 
	{
		alert("Favor informar Sistema 1 para integração.");
		document.formulario.sididsistema1.focus();
		return;
	}
	if (document.formulario.sididsistema2.value == "") 
	{
		alert("Favor informar Sistema 2 para integração.");
		document.formulario.sididsistema2.focus();
		return;
	}
	
	if (document.formulario.integracao.value == "") 
	{
		alert("Favor informar Tipo de Integração.");
		document.formulario.integracao.focus();
		return;
	}


	if ((document.formulario.leitura[0].checked == 0) && (document.formulario.leitura[1].checked == 0)) 
	{
		alert("Favor informar opção de leitura.");
		document.formulario.leitura.focus();
		return;
	}

	if ((document.formulario.escrita[0].checked == 0) && (document.formulario.escrita[1].checked == 0)) 
	{
		alert("Favor informar opção de escrita.");
		document.formulario.escrita.focus();
		return;
	}

	if (document.formulario.isidsc.value == "")
	{
		alert("Favor informar descrição da integração.");
		document.formulario.isidsc.focus();
		return;		
	}
	
	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
    document.formulario.submit();

}

</script>
