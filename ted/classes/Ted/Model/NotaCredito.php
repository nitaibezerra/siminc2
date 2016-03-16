<?php

class Ted_Model_NotaCredito extends modelo
{
    /**
     * Nome da Tabela
     * @var String
     */
    protected $stNomeTabela = 'ted.nota_credito';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('ppaid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ppaid' => null,
        'proid' => null,
        'vlrparcela' => null,
        'notacredito' => null,
        'numtransfsiafi' => null,
        'ppadata' => null
    );

    /**
     * @param null $tcpid
     * @param string $prostatus
     * @return array|bool|mixed|NULL
     */
    public function getByTcpid($tcpid = null, $prostatus = 'A')
    {
        if (!$tcpid) return false;

        $strSQL = "
            SELECT
                ppaid, proid, vlrparcela, notacredito, numtransfsiafi, TO_CHAR(ppadata, 'DD/MM/YYYY') AS ppadata
            FROM {$this->stNomeTabela}
            WHERE proid IN (
                SELECT proid FROM ted.orcamentario WHERE tcpid = {$tcpid} AND prostatus = '$prostatus'
            )
        ";

        $rs = $this->carregar($strSQL);
        return ($rs) ? $rs : null;
    }
}