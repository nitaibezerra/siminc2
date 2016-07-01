<?php 

// salva/altera os dados
if($_REQUEST["submetido"])
{
	if(!$_REQUEST["tacid"])
	{
		$sql = "INSERT INTO 
					conjur.tipoacao(tacdsc,tacstatus)
				VALUES(
				 	   '".pg_escape_string($_REQUEST["tacdsc"])."',
					   '".$_REQUEST["tacstatus"]."')";
	}
	else
	{
		$sql = "UPDATE
					conjur.tipoacao
				SET
					tacdsc = '".pg_escape_string($_REQUEST["tacdsc"])."',
					tacstatus = '".$_REQUEST["tacstatus"]."'
				WHERE
					tacid = ".$_REQUEST["tacid"];
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso.');
			window.opener.location.href = '?modulo=sistema/geral/listaTipoAcao&acao=A';
			self.close();
		  </script>";
	die();
}
// recupera os dados
if($_REQUEST["tacid"])
{
	$sql = "SELECT * FROM conjur.tipoacao WHERE tacid = ".$_REQUEST["tacid"];
	$dados = $db->carregar($sql);
	if($dados) {
		extract($dados[0]);
	}
}

monta_titulo( 'CONJUR', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formDadosTipoAcao" action="" method="post" enctype="multipart/form-data">
	<input type="hidden" name="submetido" value="1">
	<input type="hidden" name="tacid" value="<?=$tacid?>">
 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	    	<td class ="SubTituloDireita" align="right">Descrição:</td>
	        <td>
	        	<?=campo_texto('tacdsc', 'S', 'S', '', 70, 100, '', '', 'left', '',  0, 'id="tacdsc"', '', '', ''); ?>
	       	</td>
	    </tr>
	    <tr>
        	<td class ="SubTituloDireita" align="right">Status:</td>
            <td>
            	<input type="radio" name="tacstatus" <? if($tacstatus=='A' || !$tacstatus) echo 'checked="checked"'; ?> value="A"> Ativo
            	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <input type="radio" name="tacstatus" <? if($tacstatus=='I') echo 'checked="checked"'; ?> value="I"> Inativo
                &nbsp;<img src="../imagens/obrig.gif" border="0">
           	</td>
		</tr> 
		<tr style="background-color: #c0c0c0; text-align: center;">
			<td colspan="2">
				<input type="button" id="btSalvar" value="Salvar" style="cursor:pointer" onclick="submeteFormTipoAcao();" />
				<input type="button" id="btFechar" value="Fechar" style="cursor:pointer" onclick="self.close();" />
			</td>
		</tr>
	</table>
</form>

<script>

function submeteFormTipoAcao()
{
	var form		=	document.getElementById("formDadosTipoAcao");
	var desc		=	document.getElementById("tacdsc");

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
	
	form.submit();
}

</script>