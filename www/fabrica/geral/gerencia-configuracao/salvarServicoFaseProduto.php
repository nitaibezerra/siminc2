<?php
// monta cabeçalho
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';
 
$servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();
$produtoContratadoServico = new ProdutoContratadoServico();
$arrProdutoContratado = $produtoContratadoServico->recupereProdutoContratadoPorId($_POST['idServicoFaseProdutoCasoDeUso']);
$produtoContratado = $arrProdutoContratado[$_POST['idServicoFaseProdutoCasoDeUso']];

$repositorio = $_POST['repositorioCasoDeUso'];
$repositorio = str_replace("\"", "", $repositorio);
$repositorio = str_replace("'", "", $repositorio);
$repositorio = str_replace("\\", "", $repositorio);
$repositorio = str_replace("\\", "", $repositorio);
//verifica se a url passada contem um "/" no final caso tenha deve ser retirada para validação no svn
$tamanhoUrl = strlen($repositorio);
if ($tamanhoUrl == (strrpos($repositorio, "/") + 1)){
	$repositorio = rtrim($repositorio, "/");
}

$statusBanco = '';

$listaServicoFaseProduto = $produtoContratado->getListaServicoFaseProduto();
foreach ($listaServicoFaseProduto as $servicoFaseProduto){
	
	$servicoFaseProduto->setRepositorio(utf8_decode($repositorio));
	$servicoFaseProduto->setPadraoNome($_POST['padraoNomesCasoDeUso']);
	$servicoFaseProduto->setPadraoDiretorio($_POST['padraoDiretorioCasoDeUso']);
	$servicoFaseProduto->setEncontrado($_POST['encontradoCasoDeUso']);
	$servicoFaseProduto->setAtualizado($_POST['atualizadoCasoDeUso']);
	$servicoFaseProduto->setNecessario($_POST['necessarioCasoDeUso']);
	
	$statusBanco = $servicoFaseProdutoRepositorio->atualizar($servicoFaseProduto);
	
}

print $statusBanco;































