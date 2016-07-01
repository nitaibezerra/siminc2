<?php

class PesCelulaAcompNatDespesa extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pes.pescelulaacompnatdespesa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("cancodigo");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'cancodigo' => null,
									  	'cndcodigo' => null,
									  	'canano' => null,
									  	'canmes' => null,
									  	'canvalor' => null,
									  	'cantipovalor' => null,
									  );
}