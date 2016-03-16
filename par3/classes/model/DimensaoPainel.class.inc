<?php

class Par3_Model_DimensaoPainel extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.dimensaopainel";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "divid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'divid' => null,
									  	'dimid' => null,
									  	'indidpainel' => null,
									  );

	protected $stOrdem = null;



}