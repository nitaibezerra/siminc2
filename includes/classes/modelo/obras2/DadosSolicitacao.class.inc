<?php

class DadosSolicitacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.dados_solicitacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dslid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dslid' => null,
        'slcid' => null,
        'dslcomentario' => null,
        'dsldata' => null,
        'usucpf' => null,
        'dslpendencias' => null,
        'dsltipodesbloqueio' => null,
    );

}