<?php
/**
 * Classe de mapeamento da entidade proporc.despesaacao.
 *
 * $Id: Despesaacao.php 98511 2015-06-11 13:32:07Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.despesaacao.
 *
 * @see Modelo
 */
class Proporc_Model_Despesaacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.despesaacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dpaid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'dspid' => array('tabela' => 'proporc.despesa', 'pk' => 'dspid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dpaid' => null,
        'dspid' => null,
        'acacod' => null,
    );

    static public function queryTodasAsAcoes($exercicio)
    {
        return <<<DML
SELECT DISTINCT aca.acacod AS codigo,
                aca.acacod || ' - ' || aca.acadsc AS descricao
  FROM elabrev.ppaacao_orcamento aca
  WHERE aca.prgano = '{$exercicio}'
    AND aca.acasnrap = 'f'
  ORDER BY aca.acacod
DML;
    }

    static public function queryAcoesSelecionadas($dspid, $exercicio)
    {
        $sql = <<<DML
SELECT DISTINCT aca.acacod AS codigo,
                aca.acacod || ' - ' || aca.acadsc AS descricao
  FROM elabrev.ppaacao_orcamento aca
  WHERE aca.prgano = '{$exercicio}'
    AND aca.acasnrap = 'f'
    AND EXISTS (SELECT 1
                  FROM proporc.despesaacao dpa
                  WHERE aca.acacod = dpa.acacod
                    AND dpa.dspid = %d)
  ORDER BY acacod
DML;
        return sprintf($sql, $dspid);
    }
}
