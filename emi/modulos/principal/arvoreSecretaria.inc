<?php

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$arMnuid = array();

if( !emiPossuiPerfil(EMI_PERFIL_ADMINISTRADOR) ){
	$arMnuid = array(5887);
}

$db->cria_aba($abacod_tela,$url,$parametros,$arMnuid);
monta_titulo("Ensino Médio Inovador - Exercício {$_SESSION['exercicio']}", "");

if ( $_REQUEST["estuf"] ){
	
	if ( !emiVerificaUf( $_REQUEST["estuf"] ) ){
		
		print "<script>
				  alert('O estado selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
		
	}
	
	$dadosSecretaria = emiBuscaDadosSecretaria( $_REQUEST["estuf"] );
	
	unset( $_SESSION["emi"] );
	
	$_SESSION["emi"]["estuf"]	 = $_REQUEST["estuf"];
	$_SESSION["emi"]["entidpai"] = $dadosSecretaria["id"];
	$_SESSION["emi"]["nomepai"]  = $dadosSecretaria["nome"];
	
	$emi->InsereEntidade( $dadosSecretaria );
	
}

?>

<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore"></td>
	</tr>
</table>

<?php $emi->montaArvore( $dadosSecretaria ); ?>
