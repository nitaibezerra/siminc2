<?php

if($_REQUEST['itcid']){
	$sql = "SELECT
			  inc.itcid,
			  inc.entidinterveniente as entid_interveniente,
			  ent.entnumcpfcnpj,
			  ent.entnome,
			  inc.entiddirigente as entid_dirigente,
			  ent1.entnumcpfcnpj as cpfdirigente,
			  ent1.entnome as nomedirigente
			  
			FROM 
			  emenda.intervenienteconvenio inc
			  inner join entidade.entidade ent
			  	on inc.entidinterveniente = ent.entid
			  inner join entidade.entidade ent1
			    on inc.entiddirigente = ent1.entid
			WHERE
				inc.itcstatus = 'A'
			    AND inc.itcid = ".$_REQUEST['itcid'];
	
	$arConvenio = $db->pegaLinha($sql);
	extract($arConvenio);
}

if($_REQUEST['submeter']){

	if( $_POST['itcid'] ){
		alteraIntervenienteConvenio($_POST);
	} else {
		insereIntervenienteConvenio($_POST);
	}	
}

function alteraIntervenienteConvenio($post){
	global $db;

	$sql = "UPDATE 
			  emenda.intervenienteconvenio  
			SET 
			  entidinterveniente = ".$post['entid_interveniente'].",
			  entiddirigente = ".$post['entid_dirigente']."
			 
			WHERE 
			  itcid = ".$post['itcid'];
	
	$db->executar($sql);
	$db->commit();

	echo "<script>
			alert('Operação Realizada com Sucesso!');
			window.opener.document.getElementById('formMinutaConvenio').submit();
			//window.opener.listaIntervenienteConvenio(".$post['pmcid'].");
			window.close();
		  </script>";
}

function insereIntervenienteConvenio($post){
	global $db;
	
	$sql = "INSERT INTO 
			  emenda.intervenienteconvenio(
			  entidinterveniente,
			  entiddirigente,
			  pmcid
			) 
			VALUES (
			  ".$post['entid_interveniente'].",
			  ".$post['entid_dirigente'].",
			  ".$post['pmcid']."
			)";
		
	$db->executar($sql);
	$db->commit();

	echo "<script>
			alert('Operação Realizada com Sucesso!');
			window.opener.document.getElementById('formMinutaConvenio').submit();
			window.opener.document.getElementById('bointercadastrado').value = 'true';
			//window.opener.listaIntervenienteConvenio(".$post['pmcid'].");
			window.close();
		  </script>";
}
?>
<html>
<head>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
</head>
<body>
<form id="formMinutaConvenioDados" method="post" action="">
<input type="hidden" name="submeter" id="submeter" value="">
<input type="hidden" name="pmcid" id="pmcid" value="<?=$_REQUEST['pcmid']; ?>">
<input type="hidden" name="itcid" id="itcid" value="<?=$itcid; ?>">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="2"><b>Associa Interveniente</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Dados do Interveniente:</b></td>
		<td>
			<a href="#" onclick="popUpIncluirCNPJIntervenienteDirigente('I');" style="cursor:pointer">
					<label id="labelCNPF"><img src="../imagens/gif_inclui.gif" border="0">
					Clique aqui para inserir o CNPJ do Interveniente</label>
				</a>
				<br /><br />
			&nbsp;
			<input type="hidden" name="entid_interveniente" value="<?=$entid_interveniente?>" />
			<?=campo_texto( 'entnumcpfcnpj', 'S', 'N', '', 30, 20, '', '', '', '', 0, 'id="cnpjinterveniente"' );?>
			<br /><br />
			&nbsp;
			<?=campo_texto( 'entnome', 'N', 'N', '', 80, 100, '', '', '', '', 0); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Dados do Dirigente:</b></td>
		<td>
			<a href="#" onclick="popUpIncluirCNPJIntervenienteDirigente('D');" style="cursor:pointer">
					<label id="labelCPF"><img src="../imagens/gif_inclui.gif" border="0">
					Clique aqui para inserir o CPF do Dirigente</label>
				</a>
				<br /><br />
			&nbsp;
			<input type="hidden" name="entid_dirigente" value="<?=$entid_dirigente; ?>" />
			<?=campo_texto( 'cpfdirigente', 'S', 'N', '', 30, 20, '', '', '', '', 0, 'id="cpfdirigente"' );?>
			<br /><br />
			&nbsp;
			<?=campo_texto( 'nomedirigente', 'N', 'N', '', 80, 100, '', '', '', '', 0, 'id="nomedirigente"'); ?>
		</td>
	</tr>
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="2">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarMinutaInterveniente();" <?=disabled(pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] )); ?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="window.close();" />
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function popUpIncluirCNPJIntervenienteDirigente(tipo) {
	if(tipo == 'D'){
		var entid = document.getElementsByName("entid_dirigente")[0];
	} else {
		var entid = document.getElementsByName("entid_interveniente")[0];
	}	
	var janela = window.open( 'emenda.php?modulo=principal/popInsereIntervenienteMinuta&acao=A&entid=' + entid.value + '&tipo='+tipo, 'popup_dados', 'width=800,height=700,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}
function salvarMinutaInterveniente(){
	if($('cnpjinterveniente').value == ""){
		alert('O campo CNPJ do Interveniente é de preenchimento obrigatório!');
		$('cnpjinterveniente').focus();
		return false;
	} else if($('cpfdirigente').value == ""){
		alert('O campo CPF do Dirigente é de preenchimento obrigatório!');
		$('cpfdirigente').focus();
		return false;
	} 
	$('bt_salvar').disabled = 'disabled';
	$('bt_cancelar').disabled = 'disabled';
	$('submeter').value = '1';	
	$('formMinutaConvenioDados').submit();
}
</script>
</body>
</html>