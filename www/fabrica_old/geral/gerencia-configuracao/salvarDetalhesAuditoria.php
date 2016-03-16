<?php
// monta cabeçalho
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc'; 
$fiscalRepositorio = new FiscalRepositorio();
$fiscal = $fiscalRepositorio->recuperePorId($_SESSION['usucpf']);

$idServicoFaseProdutoEProdutoContratado = $_POST['idServicoFaseProdutoVisaoMEC'];

$idServicoFaseProduto = substr($idServicoFaseProdutoEProdutoContratado, 0, strpos($idServicoFaseProdutoEProdutoContratado, '--'));
$idProdutoContratado = substr($idServicoFaseProdutoEProdutoContratado, (strpos($idServicoFaseProdutoEProdutoContratado, '--')+2));

$produtoContratadoServico = new ProdutoContratadoServico();
$arrProdutoContratado = $produtoContratadoServico->recupereProdutoContratadoPorId($idProdutoContratado);
$produtoContratado = $arrProdutoContratado[$idProdutoContratado];

$detalhesAuditoriaRepositorio = new DetalhesAuditoriaRepositorio();
$detalhesAuditoria = new DetalhesAuditoria();
$auditoriaRepositorio = new AuditoriaRepositorio();
$historioAuditoriaRepositorio = new HistoricoAuditoriaRepositorio();
$itemAuditoriaDetalhesAuditoriaRepositorio = new ItemAuditoriaDetalhesAuditoriaRepositorio();

$listaServicoFaseProduto = $produtoContratado->getListaServicoFaseProduto();

foreach ($listaServicoFaseProduto as $servicoFaseProduto){
	$detalhesAuditoria = $detalhesAuditoriaRepositorio->recuperePorIdServicoFaseProduto($servicoFaseProduto->getId());
	$detalhesAuditoria->setDataAuditoria(DateTimeUtil::now("Y-m-d H:i:s"));
	$detalhesAuditoria->setMotivo($_POST['motivoAuditoriaVisaoMEC']);
	$detalhesAuditoria->setObservacao($_POST['observacaoAuditoriaVisaoMEC']);
	$detalhesAuditoria->setResultado($_POST['resultadoAuditoriaVisaoMEC']);
	$auditoria = $auditoriaRepositorio->recuperePorId($_POST['idAuditoriaVisaoMEC']);
	$auditoria->setFiscal($fiscal);
	if ($auditoria->getSituacaoAuditoria()->getId() == ''){
		$situacaoAuditoria = new SituacaoAuditoria();
		$situacaoAuditoria->setId(1);
		$auditoria->setSituacaoAuditoria($situacaoAuditoria);
	}
	$auditoriaRepositorio->salvar($auditoria);
	$detalhesAuditoria->setAuditoria($auditoria);
	$detalhesAuditoria->setServicoFaseProduto($servicoFaseProduto);
	$idDetalhesAuditoria = $detalhesAuditoriaRepositorio->salvar($detalhesAuditoria);
	$detalhesAuditoria = $detalhesAuditoriaRepositorio->recuperePorId($idDetalhesAuditoria);
	
	$arrItensAuditoria = $_POST['itemAuditoria'];
	if (count($arrItensAuditoria)==0){
		$arrItensAuditoria = array();
	}
	
	//gravando tambem em histórico
	$historioAuditoriaRepositorio->salvar($detalhesAuditoria, $_SESSION['usucpf'], $arrItensAuditoria);
	$itemAuditoriaDetalhesAuditoriaRepositorio->salvarItensAuditoria($idDetalhesAuditoria, $arrItensAuditoria);
	$detalhesAuditoriaRepositorio->commit();
	$itemAuditoriaDetalhesAuditoriaRepositorio->commit();
}
	
print $idDetalhesAuditoria;