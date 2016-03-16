<?php

class Ted_Model_Financeiro extends modelo
{
    /**
     * Nome da Tabela
     * @var String
     */
    protected $stNomeTabela = 'ted.financeiro';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('previd');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'previd' => NULL,
        'proid' => NULL,
        'tcpid' => NULL,
        'prevalor' => NULL,
        'prevdata' => NULL,
        'prevstatus' => NULL,
        'prevprazodias' => NULL,
        'prevdsc' => NULL,
        'prevpercentual' => NULL
    );

    public function getByTcpid($tcpid = null, $prevstatus = 'A')
    {
        if (!$tcpid) return false;

        $strSQL = "
            SELECT
                previd, proid, tcpid, prevalor, TO_CHAR(prevdata, 'DD/MM/YYYY') AS prevdata,
                prevstatus, prevprazodias, prevdsc, prevpercentual
            FROM {$this->stNomeTabela}
            WHERE tcpid = {$tcpid} AND prevstatus = '$prevstatus'
        ";

        $rs = $this->carregar($strSQL);
        return ($rs) ? $rs : null;
    }
}