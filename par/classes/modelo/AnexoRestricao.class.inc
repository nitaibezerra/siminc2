<?php

class AnexoRestricao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.anexorestricao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("arestid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'arestid' => null,
        'arqid' => null,
        'areststatus' => null,
        'arestinclusao' => null,
        'resid' => null,
    );
}
