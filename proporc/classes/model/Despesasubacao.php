<?php
/**
 * Classe de mapeamento da entidade proporc.despesasubacao.
 *
 * $Id: Despesasubacao.php 98198 2015-06-03 19:50:35Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.despesasubacao.
 *
 * @see Modelo
 */
class Proporc_Model_Despesasubacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.despesasubacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dpsid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'sbaid' => array('tabela' => 'pi_subacao', 'pk' => 'sbaid'),
        'dspid' => array('tabela' => 'proporc.despesa', 'pk' => 'dspid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dpsid' => null,
        'dspid' => null,
        'sbaid' => null,
    );

    static public function queryTodasAsSubacoes($exercicio)
    {
        return <<<DML
SELECT sba.sbaid AS codigo,
       sba.sbacod || ' - ' || sba.sbatitulo AS descricao
  FROM monitora.pi_subacao sba
  WHERE sba.sbastatus = 'A'
    AND sba.sbaano = '{$exercicio}'
    AND sba.sbasituacao = 'A'
    AND sba.pieid IS NOT NULL
  ORDER BY sba.sbacod
DML;
    }

    static public function querySubacoesSelecionadas($dspid, $exercicio)
    {
        $sql = <<<DML
SELECT sba.sbaid AS codigo,
       sba.sbacod || ' - ' || sba.sbatitulo AS descricao
  FROM monitora.pi_subacao sba
  WHERE sba.sbastatus = 'A'
    AND sba.sbaano = '{$exercicio}'
    AND sba.sbasituacao = 'A'
    AND sba.pieid IS NOT NULL
    AND EXISTS (SELECT 1
                  FROM proporc.despesasubacao dpa
                  WHERE sba.sbaid = dpa.sbaid
                    AND dpa.dspid = %d)
  ORDER BY sba.sbacod
DML;
        return sprintf($sql, $dspid);
    }
}