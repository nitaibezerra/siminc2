<?
$descricao = '';
$prgid = $_REQUEST['prgid'];
 
// tratando a atualização, inclusão e exclusão de programas 
if ($_REQUEST['acao_n']=='inclui' or $_REQUEST['acao_n']=='altera')
{
    $descricao       = $_REQUEST['prgdsc'];
    $link       	 = $_REQUEST['prglink'];
    $prgplanointerno = $_REQUEST['prgplanointerno'];
    $pgoid			 = $_REQUEST['pgoid'];

    if ($_REQUEST['acao_n']=='altera') 
    { 
    	$sql = "UPDATE par.programa SET
                    prgdsc          = '%s',
                    pgoid           = '%d',
                    prglink         = '%s'
                WHERE
                    prgid           = %d";
    }
    else 
    {
    	
    	$sql = "INSERT INTO par.programa (
                    prgdsc,pgoid,prglink
                ) VALUES ('%s','%d','%s')";
  	
    }
    $res = $db->executar(sprintf($sql, pg_escape_string($descricao), $pgoid, $link, $prgid));
    $db->commit();
    //$db->sucesso($modulo);
    echo "<script>
			alert('Dados gravados com sucesso.');
			

location.href='par.php?modulo=sistema/tabelasapoio/cadprograma&acao=A';
		</script>";
    die;
}

if ($_REQUEST['acao_n']=='excluir'){
	
	
	$msg  = '';
	$prop = 0;
	$prop=(integer)$db->pegaUm( "SELECT	COUNT(*) FROM par.propostasubacao WHERE prgid = ".$prgid);
	
	$subacao = 0;
	$subacao =(integer)$db->pegaUm( "SELECT	COUNT(*) FROM par.subacao WHERE sbastatus = 'A' AND prgid = ".$prgid);
	
	if (($prop == 0) && ($subacao == 0))
	{		
		$sql = "DELETE FROM par.programa WHERE prgid = $prgid";
	
	    $saida = $db->executar($sql);
	    $db->commit();
	    //$db->sucesso($modulo);
	    echo "<script>
				alert('Dados excluídos com sucesso.');
				location.href='par.php?modulo=sistema/tabelasapoio/cadprograma&acao=A';
			  </script>";
    	die;    
	} else {
		if($prop > 0){
			$msg = "\\nExiste uma ou mais propostas de sub-ação associadas ao programa.";				
		}
		if($subacao > 0){
			$msg .= "\\nExiste uma ou mais sub-ações associadas ao programa.";
		}		
			
		echo "<script>
				alert('Não foi possível excluir pois:".$msg."\\nÉ necessário resolver as pendências acima para poder excluir esse programa.');
				window.location.href = 'par.php?modulo=sistema/tabelasapoio/cadprograma&acao=A';
			 </script>";
		exit();
			
	}
}	
 

if ($_REQUEST['acao_n']=='exibir')
{
    $sql   = "SELECT * FROM par.programa WHERE prgid = $prgid";
    $saida = $db->recuperar($sql);
    if (is_array($saida)) {
        foreach($saida as $k=>$v) ${$k} = $v;
    }
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,'(Configuração do Programa)');
if($prgid === NULL){
	$prgid = 0;
}
?>

<form method="post" name="formCadprograma" id="formCadprograma" action="">
<div align="center">
<input type="hidden" name="acao_n" value="" />
<input type="hidden" name="prgid" value="<?=$_REQUEST['prgid']?>" />
<input type="hidden" name="act" value="0" />
<center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" 

align="center">
      <tr>
        <td colspan="2" class="SubTituloDireita" ></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Descrição:</td>
        <td >
		<?=campo_texto('prgdsc','S','','',60,250,'','');?></td>
      </tr>
      <tr>
			<td width="25%" class="SubtituloDireita" >Programa Origem:</td>
			<td><?php
			$sql = "SELECT pgoid as codigo, pgodsc as descricao 

FROM par.programaorigem where pgostatus = 'A'";
			$db->monta_combo('pgoid', $sql, 'S', 'Selecione', '', 

'', '', '', 'S', 'pgoid');
			?></td>
		</tr>
		 <tr>
        <td align='right' class="SubTituloDireita" >Link sobre o Programa:</td>
        <td >
		<?=campo_texto('prglink','N','','',60,250,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" ></td>
        <td class="SubTituloEsquerda" ><a  style="cursor:pointer;" 

onclick="cadastroPI(<?=$prgid;?>);"><img align="absmiddle" 

src="/imagens/gif_inclui.gif"/> Cadastrar Plano Interno</a></td>
      </tr>
      
<?   if (! $_REQUEST['prgid']){?>
      <tr>
      	<td colspan="2" class="SubTituloDireita" align="center"><input 

type='button' class="botao" value='Incluir' 

onclick="gravar_programa('I')"></td>
      </tr>
      <? } else {?>
      <tr>
      	<td colspan="2" class="SubTituloDireita" align="center"><input 

type='button' class="botao" value='Alterar' onclick="gravar_programa('A')">
      	<input type='button' class='botao' value='Cancelar' id='btcancelar' 

name='btcancelar' onclick='history.back();' />
      	</td>
      </tr>      
	<? } ?>
 </table>
 </center>
 <br><br>
</div>
</form>
<?php

$cabecalho = array('Ações', 'Descrição','Programa Origem','Link do Programa');
$sql = "SELECT 
			'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Programa\" onclick=\"altera_programa('||p.prgid||')\" /> 
			 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Programa\" onclick=\"excluir_programa('||p.prgid||')\" />' as acao, 
			 p.prgdsc,
			 COALESCE(pgo.pgodsc,'N/A') as pgodsc,
			 p.prglink
		FROM 
			par.programa p 
		LEFT JOIN
			 par.programaorigem pgo ON p.pgoid = pgo.pgoid AND 

pgo.pgostatus = 'A'
		WHERE 
			p.prgstatus = 'A' 
		ORDER BY 
			p.prgdsc";

$sql1= "SELECT p.prgdsc FROM par.programa p ORDER BY p.prgdsc";

$db->monta_lista($sql,$cabecalho,100,20,'','','');
?>


<script>
	var form = document.getElementById('formCadprograma');

	function cadastroPI(prgid){
		window.open('par.php?modulo=sistema/tabelasapoio/cadPlanoInterno&acao=A&prgid='+prgid,'gera','width=700,height=350,scrollbars=1');
	}
 
   function gravar_programa(cod)
   {

   	if (!validaBranco(document.formCadprograma.prgdsc, 'Descrição'))return;			  	
   	// if (!validaBranco(document.formCadprograma.prgplanointerno, 'PI')) return;
	 

	if (cod=='I') document.formCadprograma.acao_n.value='inclui';
   	   	else document.formCadprograma.acao_n.value='altera';
   	   	// checar consistencias
   	   	
   	   	document.formCadprograma.submit();
   }
   
   
  function altera_programa(cod)
  {
	var acao 	= document.getElementsByName('acao_n')[0];
	var prgid 	= document.getElementsByName('prgid')[0];
	  
	acao.value = 'exibir';
	prgid.value = cod;
	form.submit();
  }   


  function excluir_programa(cod)
  {
    if( window.confirm( "Confirma a exclusão do programa?") )
    {
	document.formCadprograma.acao_n.value = 'excluir';
	document.formCadprograma.prgid.value = cod;
	document.formCadprograma.submit();
    } else return;
  }   

</script>