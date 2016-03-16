<?php

class UnidadeMedidaCronograma extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.unidademedidacronograma";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "umcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'umcid'		    => null,
									  	'umcdsc'	    => null,
									  	'umcstatus'	    => null
									  );

}