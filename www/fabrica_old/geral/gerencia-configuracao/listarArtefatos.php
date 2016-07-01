<?php
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';
 
//recuperando Fiscal
$fiscalRepositorio = new FiscalRepositorio();
$fiscal = $fiscalRepositorio->recuperePorId($_POST['gc-fiscais']);

//recuperando situacaoAuditoria
if ($_POST['gc-situacoes'] != null) {
	$situacaoAuditoriaRepositorio = new SituacaoAuditoriaRepositorio();
	$situacaoAuditoria = $situacaoAuditoriaRepositorio->recuperePeloId($_POST['gc-situacoes']);
}

//recuperando Solicitacao
if($_POST['gc-analiseSolicitacao'] != null){
	$solicitacaoRepositorio = new SolicitacaoRepositorio();
	$solicitacao = $solicitacaoRepositorio->recuperePorIdDaAnaliseSolicitacao($_POST['gc-analiseSolicitacao']);
	
	//recuperando Analise Solicitacao
	$analiseSolicitacaoRepositorio = new AnaliseSolicitacaoRepositorio();
	$analiseSolicitacao = $analiseSolicitacaoRepositorio->recuperePorId($_POST['gc-analiseSolicitacao']);
}

if ($solicitacao->possuiAuditoria()){
	$auditoria = $solicitacao->getAuditoria();
} else {
	$auditoria = new Auditoria();
}

$auditoria->setAnaliseSolicitacao($analiseSolicitacao);
$auditoria->setFiscal($fiscal);
$auditoria->setNomeResponsavelFabrica(utf8_decode($_POST['audrespfabrica']));

$situacaoAuditoria = new SituacaoAuditoria();
$situacaoAuditoria->setId(SituacaoAuditoria::PENDENTE);
$auditoria->setSituacaoAuditoria($situacaoAuditoria);


$auditoriaRepositorio = new AuditoriaRepositorio();

print $auditoriaRepositorio->salvar($auditoria);
