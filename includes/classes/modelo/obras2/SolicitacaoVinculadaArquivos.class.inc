<?php

class SolicitacaoVinculadaArquivos extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao_vinculada_arquivos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("svaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'svaid' => null,
        'slvid' => null,
        'arqid' => null,
        'svastatus' => null,
    );

    public function pegaArquivosPorSlvid($slvid)
    {
        $sql = "
            SELECT a.*, m.svaid FROM obras2.solicitacao_vinculada_arquivos m
            JOIN arquivo a ON a.arqid = m.arqid
            WHERE m.svastatus = 'A' AND slvid = $slvid
        ";
        return $this->carregar($sql);
    }
}