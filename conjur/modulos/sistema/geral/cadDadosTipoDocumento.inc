<?php 

// salva/altera os dados
if($_REQUEST["submetido"])
{
	if(!$_REQUEST["tpdid"])
	{
		$sql = "INSERT INTO 
					conjur.tipodocumento(tpddsc,tpdstatus,tpdgeranumeracao)
				VALUES(
				 	   '".pg_escape_string($_REQUEST["tpddsc"])."',
					   '".$_REQUEST["prodtstatus"]."','".$_REQUEST['tpdgeranumeracao']."')";
	}
	else
	{
		$sql = "UPDATE
					conjur.tipodocumento
				SET
					tpddsc = '".pg_escape_string($_REQUEST["tpddsc"])."',
					tpdstatus = '".$_REQUEST["prodtstatus"]."',
					tpdgeranumeracao = '".$_REQUEST['tpdgeranumeracao']."'
				WHERE
					tpdid = ".$_REQUEST["tpdid"];
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso.');
			window.opener.location.href = '?modulo=sistema/geral/listaTipoDocumento&acao=A';
			self.close();
		  </script>";
	die();
}
// recupera os dados
if($_REQUEST["tpdid"])
{
	$sql = "SELECT * FROM conjur.tipodocumento WHERE tpdid = ".$_REQUEST["tpdid"];
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

<form id="formDadosTipoDocumento" action="" method="post" enctype="multipart/form-data">
	<input type="hidden" name="submetido" value="1">
	<input type="hidden" name="tpdid" value="<?=$tpdid?>">
 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	    	<td class ="SubTituloDireita" align="right">Descrição:</td>
	        <td>
	        	<?=campo_texto('tpddsc', 'S', 'S', '', 70, 100, '', '', 'left', '',  0, 'id="tpddsc"', '', '', ''); ?>
	       	</td>
	    </tr>
	    <tr>
        	<td class ="SubTituloDireita" align="right">Produz Numeração?</td>
            <td>
            	<input type="radio" name="tpdgeranumeracao" <? if($tpdgeranumeracao=='S' || !$tpdgeranumeracao) echo 'checked="checked"'; ?> value="S"> Sim
            	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <input type="radio" name="tpdgeranumeracao" <? if($tpdgeranumeracao=='N') echo 'checked="checked"'; ?> value="N"> Não
                &nbsp;<img src="../imagens/obrig.gif" border="0">
           	</td>
		</tr> 
	    <tr>
        	<td class ="SubTituloDireita" align="right">Status:</td>
            <td>
            	<input type="radio" name="prodtstatus" <? if($tpdstatus=='A' || !$tpdstatus) echo 'checked="checked"'; ?> value="A"> Ativo
            	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <input type="radio" name="prodtstatus" <? if($tpdstatus=='I') echo 'checked="checked"'; ?> value="I"> Inativo
                &nbsp;<img src="../imagens/obrig.gif" border="0">
           	</td>
		</tr> 
		<tr style="background-color: #c0c0c0; text-align: center;">
			<td colspan="2">
				<input type="button" id="btSalvar" value="Salvar" style="cursor:pointer" onclick="submeteFormTipoDocumento();" />
				<input type="button" id="btFechar" value="Fechar" style="cursor:pointer" onclick="self.close();" />
			</td>
		</tr>
	</table>
</form>

<script>

function submeteFormTipoDocumento()
{
	var form		=	document.getElementById("formDadosTipoDocumento");
	var desc		=	document.getElementById("tpddsc");
	var unidade		=	document.getElementById("tpdid");

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