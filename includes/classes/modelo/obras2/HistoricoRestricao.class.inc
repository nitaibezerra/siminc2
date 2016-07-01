<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of HistoricoRestricao
 *
 * @author lindalbertofilho
 */
class HistoricoRestricao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.historico_restricao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hrtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'hrtid' => NULL,
        'rstid' => NULL,
        'fsrid' => NULL,
        'usucpf' => NULL,
        'rstdsc' => NULL,
        'rstdtprevisaoregularizacao' => NULL,
        'rstdscprovidencia' => NULL,
        'rstdtinclusao' => NULL,
        'rststatus' => NULL,
        'obrid' => NULL,
        'rstitem' => NULL
    );

    public function listaHistorico($rstid)
    {
        $sql = <<<DML
            SELECT
                hrtid,
                usunome,
                CASE WHEN rstitem = 'I' THEN 'Inconformidade' ELSE 'Restrição' END AS rstitem,
                TO_CHAR(rstdtinclusao,'DD/MM/YYYY') as rstdtinclusao,
                rstdsc,
                rstdscprovidencia,
                TO_CHAR(rstdtprevisaoregularizacao,'DD/MM/YYYY') as rstdtprevisaoregularizacao
            FROM {$this->stNomeTabela} hr
            INNER JOIN seguranca.usuario usu ON (hr.usucpf = usu.usucpf)
            WHERE rstid = $rstid
                AND rststatus = 'A'
DML;
        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }
    
}