<?php
/**
 * Enum de Status da Ordem de Serviço. Não poderá ser instanciada
 * @author rayner
 *
 */
final class StatusOrdemServico {
	
	const FINALIZADA = 257;
	
	const EM_REVISAO = 261;
	
	const EM_VALIDACAO = 262;
	
	const AGUARDANDO_HOMOLOGACAO_GESTOR = 263;
	
	const EM_APROVACAO = 264;
	
	const PENDENTE = 254;
	
	const EM_EXECUCAO = 255;
	
	const EM_AVALIACAO = 256;
	
	const CANCELADA_SEM_CUSTO = 301;
	
	const CANCELADA_COM_CUSTO = 302;
	
	const REABERTA = 362;
	
	private function __construct(){}
}