<?php

class Acompanhamento_Mi extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.acompanhamento_mi";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("amiid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'amiid' => null,
        'obrid' => null,
        'amiobraexecucao' => null,
        'amipercexecucao' => null,
        'amiobs' => null,
        'amiproblema' => null,
        'amidescproblema' => null,
        'amiedicao' => null,
        'amidatainclusao' => null,
        'amidataacompanhamento' => null,
        'amiusuario' => null,
        'amistatus' => null,
    );
}