<?php

class SolicitacaoDesembolsoArquivos extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao_desembolso_arquivos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sdaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sdaid' => null,
        'sldid' => null,
        'arqid' => null,
        'sdastatus' => null,
    );

    public function pegaArquivosPorSlvid($sldid)
    {
        $sql = "
            SELECT a.*, m.sdaid FROM obras2.solicitacao_desembolso_arquivos m
            JOIN arquivo a ON a.arqid = m.arqid
            WHERE m.sdastatus = 'A' AND sldid = $sldid
        ";
        return $this->carregar($sql);
    }
}