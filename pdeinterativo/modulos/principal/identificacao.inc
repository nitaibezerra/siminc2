<?php
if($_GET['exibeGaleria'] && $_GET['pdeid']){
	$_SESSION['downloadfiles']['pasta'] = array("origem" => "pdeinterativo","destino" => "pdeinterativo");
	$_SESSION['imgparams'] = array("filtro" => "gfostatus = 'A' AND pdeid = ".$_GET['pdeid']."", "tabela" => "pdeinterativo.galeriafoto");
	$sql = "select arqid from pdeinterativo.galeriafoto where pdeid = {$_GET['pdeid']} and gfostatus = 'A' limit 1";
	$arqid = $db->pegaUm($sql);
	header("Location: ../slideshow/slideshow/index.php?pagina=&$arqid");
}

if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}
if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}

$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);
$_SESSION['pdeinterativo_vars']['usucpfdiretor']   = $arrDados['usucpf'];
$_SESSION['pdeinterativo_vars']['pdicodinep']      = $arrDados['pdicodinep'];
$_SESSION['pdeinterativo_vars']['pdeid']           = $arrDados['pdeid'];
$_SESSION['pdeinterativo_vars']['pdenome'] 		   = $arrDados['pdenome'];
$_SESSION['pdeinterativo_vars']['pditempdeescola'] = $arrDados['pditempdeescola'];

if(!$_SESSION['pdeinterativo_vars']['pdicodinep'] || !$_SESSION['pdeinterativo_vars']['pdeid']){
	echo "<script>alert('Escola não encontrada!');window.location.href='pdeinterativo.php?modulo=principal/principal&acao=A'</script>";
	exit;
}


// se for super usuario e não tiver tentando simular nenhum direto, direciona-lo para a tela com a lista de diretores
if($db->testa_superuser() && !$_SESSION['pdeinterativo_vars']['usucpfdiretor']) {
	die("<script>
			alert('Você não é diretor. Por favor selecione um diretor');
			window.location='pdeinterativo.php?modulo=principal/principal&acao=A';
		 </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
barraProgressoPDEInterativo();
print '<br/>';
/*** Array com os itens da aba de identificação ***/
$menu = array(0 => array("id" => 1, "descricao" => "Apresentação", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Apresentacao"),
			  1 => array("id" => 2, "descricao" => "Diretor(a)", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Diretor"),
			  2 => array("id" => 3, "descricao" => "Escola", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola"),
			  3 => array("id" => 4, "descricao" => "Galeria de Fotos", "link" => "/pdeinterativo/pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Galeria")
			  );

$aba = !$_GET['aba'] ? "Apresentacao" : $_GET['aba'];
$abaAtiva = "/pdeinterativo/pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=$aba";

echo montarAbasArray($menu, $abaAtiva);

/*** Monta a página do segundo conjunto de abas ***/
include_once "identificacao".$aba.".inc";

verificaPermissao(PDEESC_PERFIL_DIRETOR);