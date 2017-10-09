<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova ". SIGLA_SISTEMA, 'Identificação' );

if( $_POST['requisicao'] == 'Salvar' ){
	global $db;

	$sql = "INSERT INTO prova.identificacao (nome, cpf) VALUES ('".$_POST['nome']."','".$_POST['cpf']."')";
	$db->executar($sql);
	$db->commit();
	
	echo '<script>alert("Dados salvos com sucesso!"); window.location.href="prova.php?modulo=principal/prova1&acao=A";</script>';
	exit;
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
	function salvar(){
		document.getElementById('requisicao').value = 'Salvar';
		document.getElementById('formu').submit();
	}
</script>
<form name="formu" id="formu" method="post">
<table style="background-color: #F5F5F5;" align="center" border="1" width="50%" class="tabela" cellpadding="3" cellspacing="2">
	<input type="hidden" id="requisicao" name="requisicao" value="">
	<tr>
		<td width="195" class="SubTituloDireita" valign="top">
			<b>Nome</b>
		</td>
		<td>
			<input type="text" name="nome" id="nome" value="">
		</td>
	</tr>
	<tr>
		<td width="195" class="SubTituloDireita" valign="top">
			<b>CPF (Sem pontos e traço)</b>
		</td>
		<td>
			<input type="text" name="cpf" id="cpf" value="">
		</td>
	</tr>
</table>
<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="95%" class="tabela">
	<tbody>
		<tr bgcolor="#cccccc">
			<td>
				<div>
					<input value="Salvar" type="button" onclick="salvar()">
				</div>
			</td>
		</tr>
	</tbody>
</table>
</form>