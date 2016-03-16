<?php
/**
 * Classe de mapeamento da entidade proporc.despesa.
 *
 * $Id: Despesa.php 101146 2015-08-11 21:22:58Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.despesa.
 *
 * @see Modelo
 */
class Proporc_Model_Despesa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.despesa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dspid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'gdpid' => array('tabela' => 'proporc.grupodespesa', 'pk' => 'gdpid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dspid' => null,
        'gdpid' => null,
        'dspsigla' => null,
        'dspnome' => null,
        'dspobservacao' => null,
        'vlrmontante' => null
    );

    /**
     * Antes de excluir um grupo de despesa, verifica se não há nenhuma categoria cadastrada para ele.
     * @param type $id
     * @return type
     */
    public function antesExcluir($id = null)
    {
        $sql = <<<DML
DELETE FROM proporc.despesafonterecurso
  WHERE dspid = :dspid;
DELETE FROM proporc.despesagnd
  WHERE dspid = :dspid;
DELETE FROM proporc.despesaplanoorcamentario
  WHERE dspid = :dspid;
DELETE FROM proporc.despesasubacao
  WHERE dspid = :dspid;
DELETE FROM proporc.despesaunidadeorcamentaria
  WHERE dspid = :dspid;
DELETE FROM proporc.limitesfonteunidadeorcamentaria
  WHERE dspid = :dspid;
DELETE FROM proporc.despesaacao
  WHERE dspid = :dspid;
DELETE FROM proporc.despesa
  WHERE dspid = :dspid;
DML;
        $dml = new Simec_DB_DML($sql);
        $dml->addParam('dspid', $this->dspid);
        $this->executar($dml);
        $this->commit();
        return true;
    }

    public function antesSalvar()
    {
        $this->vlrmontante = str_replace(array('.', ','), array('', '.'), $this->vlrmontante);
        return true;
    }
}
