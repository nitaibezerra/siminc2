<?php

class TipoAnexoOsMi extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoanexoosmi";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("taoid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'taoid' => null,
        'taonome' => null,
        'taodsc' => null,
        'tomid' => null,
    );


    public function getTiposPorTomid($tomid)
    {
        $sql = "SELECT * FROM obras2.tipoanexoosmi where tomid = $tomid";
        return $this->carregar($sql);
    }
}