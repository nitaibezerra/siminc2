<?php

class Acompanhamento_Mi_Arquivos extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.acompanhamento_mi_arquivos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("amaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'amaid' => null,
        'amiid' => null,
        'arqid' => null,
        'amastatus' => null,
    );

    public function pegaArquivosPorAmiid($amiid)
    {
        $sql = "
            SELECT a.*, m.amaid FROM obras2.acompanhamento_mi_arquivos m
            JOIN arquivo a ON a.arqid = m.arqid
            WHERE m.amastatus = 'A' AND amiid = $amiid
        ";

        return $this->carregar($sql);
    }
}