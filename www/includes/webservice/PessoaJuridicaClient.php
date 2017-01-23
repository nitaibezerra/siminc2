<?php

include_once 'RestReceitaFederal.php';
include_once 'AdapterReceitaFederalSimec.php';

/**
 * Classe para acesso ao webservice de pessoa jurídica.
 * 
 * PS: Não esqueça de ler o leiame.txt
 *
 */
final class PessoaJuridicaClient
{
	/**
	 * Coloca o objeto do cliente do webservice.
	 *
	 * @var SoapClient
	 */
	private $soapClient;

	
	/**
	 * Construtor da classe.
	 *
	 * @param string $wsdl
	 */
	public function __construct($wsdl)
	{

//		try{
//			$this->soapClient = new SoapClient( $wsdl );
//		} catch (Exception $e){
//			exit("Não está conectado!");
//		}
		
	}
	
	/**
	 * Retorna dados de pessoa jurídica pelo CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosResumidoPessoaJuridicaPorCnpj( $cnpj )
	{
		return AdapterReceitaFederalSimec::solicitarDadosPessoaJuridicaPorCnpj($cnpj);
	}
	
	/**
	 * Retorna dados completo de pessoa jurídica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosPessoaJuridicaPorCnpj( $cnpj )
	{
		return AdapterReceitaFederalSimec::solicitarDadosPessoaJuridicaPorCnpj($cnpj);
	}
	
	/**
	 * Retorna dados de Endereço da pessoa jurídica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna dados de Contato da pessoa jurídica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna as informações do sócio da pessoa jurídica.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj ) );
	}
}
