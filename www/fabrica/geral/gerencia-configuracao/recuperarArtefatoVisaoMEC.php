<?php
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

$detalhesAuditoriaRepositorio = new DetalhesAuditoriaRepositorio();
$detalhesAuditoria = $detalhesAuditoriaRepositorio->recuperePorIdServicoFaseProduto($servicoFaseProduto->getId());

if ($detalhesAuditoria->getId() != null){
	$itemAuditoriaDetalhesAuditoriaRepositorio = new ItemAuditoriaDetalhesAuditoriaRepositorio();
	$listaItemAuditoriaDetalhesAuditoria = $itemAuditoriaDetalhesAuditoriaRepositorio->recupereItensAuditoria($detalhesAuditoria->getId());
	
	if ( count($listaItemAuditoriaDetalhesAuditoria) > 0 ){
		foreach ($listaItemAuditoriaDetalhesAuditoria as $itemAuditoriaDetalhesAuditoria){
			$itensAuditoriaAssociados[] = array( "id" => $itemAuditoriaDetalhesAuditoria->getItemAuditoria()->getId() );
		}
	} else {
		$itensAuditoriaAssociados = array( "id" => "" );
	}
	
	$auditoria = $detalhesAuditoria->getAuditoria();
	
	$aud = array(
		"id" => $auditoria->getId()
	);
} else {
	$itensAuditoriaAssociados[] = array( "id" => "" );
	
	$solicitacaoRepositorio = new SolicitacaoRepositorio();
	$solicitacao = $solicitacaoRepositorio->recuperePorIdDaAnaliseSolicitacao($servicoFaseProduto->getAnaliseSolicitacao()->getId());
	$aud = array("id" => $solicitacao->getAuditoria()->getId());
}

$sfp = array(
	"id" => $idServicoFaseProdutoEProdutoContratado,
	"atualizado" => utf8_encode($servicoFaseProduto->getAtualizado()),
	"encontrado" => utf8_encode($servicoFaseProduto->getEncontrado()),
	"necessario" => utf8_encode($servicoFaseProduto->getNecessario()),
	"padraoDiretorio" => utf8_encode($servicoFaseProduto->getPadraoDiretorio()),
	"padraoNome" => utf8_encode($servicoFaseProduto->getPadraoNome()),
	"repositorio" => utf8_encode($servicoFaseProduto->getRepositorio()),
	"repositorioHttp" => utf8_encode($servicoFaseProduto->getRepositorioParaDownload())
);

$da = array(
	"id" => $detalhesAuditoria->getId(),
	"dataAuditoria" => $detalhesAuditoria->getDataAuditoria()->format("d-m-Y"),
	"resultado" => $detalhesAuditoria->getResultado(),
	"motivo" => $detalhesAuditoria->getMotivo(),
	"observacao" => $detalhesAuditoria->getObservacao(),
	"servicoFaseProduto" => $sfp,
	"auditoria" => $aud,
	"itensAuditoriaAssociados" => $itensAuditoriaAssociados
);

print simec_json_encode($da);