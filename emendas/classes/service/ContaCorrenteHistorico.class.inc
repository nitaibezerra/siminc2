<?php
	
class ContaCorrenteHistorico extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.contacorrentehistorico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cchid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cchid' => null, 
									  	'cocid' => null, 
									  	'cocandamento' => null, 
									  	'cocsituacao_conta' => null, 
									  	'usucpf' => null, 
									  	'ccdatainclusao' => null, 
									  	'cocdsproblema' => null, 
									  );
}