<?php

class Par3_Model_IndicadorPainel extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.indicadorpainel";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "invid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'invid' => null,
									  	'indid' => null,
									  	'indidpainel' => null,
									  );

	protected $stOrdem = null;



}