<?php
	
class FiscalRegistroAtividade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.fiscalregistroatividade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fraid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'fraid' => null,
                                        'usucpf' => null,
                                        'entidempresa' => null,
                                        'oarid' => null
                                      );
}