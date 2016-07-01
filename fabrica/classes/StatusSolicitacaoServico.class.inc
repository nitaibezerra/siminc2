<?php
/**
 * Enum de Status da Solicitaчуo de Serviчo. Nуo poderс ser instanciada
 * @author rayner
 *
 */
final class StatusSolicitacaoServico {
	
	const EM_ANALISE = 246;
	
	const EM_DETALHAMENTO = 247;
	
	const EM_AVALIACAO = 248;
	
	const EM_APROVACAO = 249;
	
	const CANCELADA_SEM_CUSTO = 251;
	
	const EM_EXECUCAO = 252;
	
	const FINALIZADA = 253;
	
	const EM_REVISAO = 250;
	
	const CANCELADA_COM_CUSTO = 300;
	
	const AGUARDANDO_PAGAMENTO = 361;
	
	const REABERTA = 364;
	
	private function __construct(){}
}