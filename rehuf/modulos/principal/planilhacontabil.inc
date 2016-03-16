<?php
/* TRATAMENTO DE SEGURANÇA */
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
// Validando os acessos as funções autorizadas
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('salvarRegistroEntidade'=>true);
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}
/* FIM - TRATAMENTO DE SEGURANÇA */

if(!$_REQUEST['planilha'])
	$_REQUEST['planilha'] = "planilhacontabil2009.inc";

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/planilhacontabil&acao=A");

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);
/* fim montando cabeçalho */
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloDireita">Selecione a planilha contabil :
<?
$planilha=$_REQUEST['planilha'];
$planil = array(0 => array("codigo" => "planilhacontabil2008.inc", "descricao" => "Planilha Contabil 2004-2008"),
				1 => array("codigo" => "planilhacontabil2009.inc", "descricao" => "Planilha Contabil 2009-2011"));
$db->monta_combo('planillha', $planil, 'S', 'Selecione', '', '', '', '200', 'N', 'planilha', false, $planilha);
?>
 <input type="button" value="Acessar" onclick="window.location='?modulo=principal/planilhacontabil&acao=A&planilha='+document.getElementById('planilha').value;">
</td>
</tr>
</table>
<?

if($planilha)
	include APPRAIZ ."rehuf/modulos/principal/".$planilha;

?>