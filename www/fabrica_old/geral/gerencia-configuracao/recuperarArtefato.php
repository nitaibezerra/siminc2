<?php
// monta cabeçalho
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$idServicoFaseProdutoEProdutoContratado = $_POST['idServicoFaseProduto'];

$idServicoFaseProduto = substr($idServicoFaseProdutoEProdutoContratado, 0, strpos($idServicoFaseProdutoEProdutoContratado, '--'));
$idProdutoContratado = substr($idServicoFaseProdutoEProdutoContratado, (strpos($idServicoFaseProdutoEProdutoContratado, '--')+2));

$servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();
$servicoFaseProduto = $servicoFaseProdutoRepositorio->recuperePorId($idServicoFaseProduto);

$sfp = array(
	"id" => $idProdutoContratado,
	"atualizado" => utf8_encode($servicoFaseProduto->getAtualizado()),
	"encontrado" => utf8_encode($servicoFaseProduto->getEncontrado()),
	"necessario" => utf8_encode($servicoFaseProduto->getNecessario()),
	"padraoDiretorio" => utf8_encode($servicoFaseProduto->getPadraoDiretorio()),
	"padraoNome" => utf8_encode($servicoFaseProduto->getPadraoNome()),
	"repositorio" => utf8_encode($servicoFaseProduto->getRepositorio())
);
print simec_json_encode($sfp);