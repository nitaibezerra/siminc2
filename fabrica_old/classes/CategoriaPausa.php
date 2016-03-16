<?php
class CategoriaPausa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.categoriapausa";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ctpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ctpid'         => null,
        'ctpdsc'        => null,
        'ctpdtcadastro' => null,
        'ctpstatus'     => null
    );
}
   