<?php

function excluirCPFListaNegra($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto2.listanegrasgb WHERE lnscpf='".str_replace(array(".","-"),array("",""),$dados['lnscpf'])."'"; 
	$db->executar($sql);
	$db->commit();
	
	echo "<script>alert('CPF excluído com sucesso');window.location='sispacto2.php?modulo=sistema/geral/listanegra&acao=A';</script>";
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
monta_titulo( "Lista negra", 'Lista de bolsistas com erro na comunicação com SGB' );

?>
<script>
function removerCPF(cpf) {
	var conf = confirm('Deseja realmente remover o CPF da lista negra?');
	
	if(conf) {
		window.location='sispacto2.php?modulo=sistema/geral/listanegra&acao=A&requisicao=excluirCPFListaNegra&lnscpf='+cpf;
	}
	
}
</script>
<?

$sql = "SELECT '<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerCPF(\''||lnscpf||'\');\">' as acao, replace(to_char(lnscpf::numeric, '000:000:000-00'), ':', '.') FROM sispacto2.listanegrasgb";

$cabecalho = array("&nbsp;","CPF");
$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);

?>