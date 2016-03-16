<?php 

// salva/altera os dados
if($_REQUEST["submetido"])
{
	if(!$_REQUEST["proid"])
	{
		$sql = "INSERT INTO 
					conjur.procedencia(unpid,prodsc,prodtinclusao,prodtstatus)
				VALUES(".$_REQUEST["unpid"].",
				 	   '".pg_escape_string($_REQUEST["prodsc"])."',
					   now(),
					   '".$_REQUEST["prodtstatus"]."')";
	}
	else
	{
		$sql = "UPDATE
					conjur.procedencia
				SET
					unpid = ".$_REQUEST["unpid"].",
					prodsc = '".pg_escape_string($_REQUEST["prodsc"])."',
					prodtstatus = '".$_REQUEST["prodtstatus"]."'
				WHERE
					proid = ".$_REQUEST["proid"];
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso.');
			window.opener.location.href = '?modulo=sistema/geral/listaProcedencia&acao=A';
			self.close();
		  </script>";
	die();
}
// recupera os dados
if($_REQUEST["proid"])
{
	$sql = "SELECT * FROM conjur.procedencia WHERE proid = ".$_REQUEST["proid"];
	$dados = $db->carregar($sql);
	extract($dados[0]);
}

monta_titulo( 'CONJUR', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formDadosProcedencia" action="" method="post" enctype="multipart/form-data">
	<input type="hidden" name="submetido" value="1">
	<input type="hidden" name="proid" value="<?=$proid?>">
 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	    	<td class ="SubTituloDireita" align="right">Descrição:</td>
	        <td>
	        	<?=campo_texto('prodsc', 'S', 'S', '', 70, 100, '', '', 'left', '',  0, 'id="prodsc"', '', '', ''); ?>
	       	</td>
	    </tr>
	    <tr>
	    	<td class ="SubTituloDireita" align="right">Unidade:</td>
	        <td>
	        	<?
	        		$sql = "SELECT unpid as codigo, unpdsc as descricao FROM conjur.unidadeprocedencia WHERE undpstatus = 'A'";
	        		$db->monta_combo('unpid', $sql, 'S', "Selecione...", '', '', '', '370', 'S', 'unpid');
	        	?>
	       	</td>
	    </tr>
	    <tr>
        	<td class ="SubTituloDireita" align="right">Status:</td>
            <td>
            	<input type="radio" name="prodtstatus" <? if($prodtstatus=='A' || !$prodtstatus) echo 'checked="checked"'; ?> value="A"> Ativo
            	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <input type="radio" name="prodtstatus" <? if($prodtstatus=='I') echo 'checked="checked"'; ?> value="I"> Inativo
                &nbsp;<img src="../imagens/obrig.gif" border="0">
           	</td>
		</tr> 
		<tr style="background-color: #c0c0c0; text-align: center;">
			<td colspan="2">
				<input type="button" id="btSalvar" value="Salvar" style="cursor:pointer" onclick="submeteFormProcedencia();" />
				<input type="button" id="btFechar" value="Fechar" style="cursor:pointer" onclick="self.close();" />
			</td>
		</tr>
	</table>
</form>

<script>

function submeteFormProcedencia()
{
	var form		=	document.getElementById("formDadosProcedencia");
	var desc		=	document.getElementById("prodsc");
	var unidade		=	document.getElementById("unpid");

	var btSalvar	=	document.getElementById("btSalvar");
	var btFechar	=	document.getElementById("btFechar");

	btSalvar.disabled	= true;
	btFechar.disabled	= true;
	
	if(desc.value == "") {
		alert("O campo 'Descrição' deve ser preenchido.");
		desc.focus();
		btSalvar.disabled	= false;
		btFechar.disabled	= false;
		return;
	}
	if(unidade.value == "") {
		alert("O campo 'Unidade' deve ser selecionado corretamente.");
		unidade.focus();
		btSalvar.disabled	= false;
		btFechar.disabled	= false;
		return;
	}
	
	form.submit();
}

</script>