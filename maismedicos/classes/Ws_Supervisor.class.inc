<?php
	
class Ws_Supervisor extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "maismedicos.ws_supervisores";
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array( "wssid" );
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
		
	protected $arAtributos     = array(
								  'wssid'=> null,
								  'wssdata'=> null,
								  'noSupervisor'=> null,
								  'nuCpf'=> null,
								  'deEmail'=> null,
								  'deFone'=> null,
								  'deLogradouro'=> null,
								  'deBairro'=> null,
								  'deComplemento'=> null,
								  'nuCep'=> null,
								  'municSupervisor'=> null,
								  'ufSupervisor'=> null,
								  'ibgeSupervisor'=> null,
								  'dsTitulacao'=> null,
								  'noFormacaoProfissional'=> null,
								  'dsComplementoFormacao'=> null,
								  'idInstituicao'=> null,
								  'nuCnpjInstituicao'=> null,
								  'noInstituicao'=> null,
								  'municInstituicao'=> null,
								  'ufInstituicao'=> null,
								  'noMae'=> null,
								  'dtNascimento'=> null,
								  'bancoComum_agenciaConta'=> null,
								  'bancoComum_dvAgenciaConta'=> null,
								  'bancoComum_contaCorrente'=> null,
								  'bancoComum_dvContaCorrente'=> null,
								  'bancoComum_bancoConta'=> null,
								  'bancoComum_municipioConta'=> null,
								  'bancoComum_ufConta'=> null,
								  'bancoBolsa_agenciaContaBolsa'=> null,
								  'bancoBolsa_dvAgenciaContaBolsa'=> null,
								  'bancoBolsa_bancoContaBolsa'=> null,
								  'bancoBolsa_municipioContaBolsa'=> null,
								  'bancoBolsa_ufContaBolsa'=> null,
								  'tutorResponsavel'=> null,
								  'situacao'=> null,
								  'validado'=> null,
								  'wssstatus'=> null,
								  
	);
	 
}