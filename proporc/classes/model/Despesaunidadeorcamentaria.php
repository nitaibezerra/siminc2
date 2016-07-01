<?php
/**
 * Classe de mapeamento da entidade proporc.despesaunidadeorcamentaria.
 *
 * $Id: Despesaunidadeorcamentaria.php 99948 2015-07-09 20:27:35Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.despesaunidadeorcamentaria.
 *
 * @see Modelo
 */
class Proporc_Model_Despesaunidadeorcamentaria extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.despesaunidadeorcamentaria';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dsuid',
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
        'dsuid' => null,
        'dspid' => null,
        'unicod' => null,
    );

    static public function queryTodasAsUnidades()
    {
        return <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unistatus = 'A'
    AND (uni.orgcod = '26000' OR uni.unicod IN('74902', '73107'))
  ORDER BY uni.unicod
DML;
    }

    static public function queryUnidadesSelecionadas($dspid)
    {
        $sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unistatus = 'A'
    AND (uni.orgcod = '26000' OR uni.unicod IN('74902', '73107'))
    AND EXISTS (SELECT 1
                  FROM proporc.despesaunidadeorcamentaria dpu
                  WHERE uni.unicod = dpu.unicod
                    AND dpu.dspid = %d)
  ORDER BY uni.unicod
DML;
        return sprintf($sql, $dspid);
    }

    public function existe()
    {
        return (bool)$this->recuperarTodos(
            'COUNT(1) AS qtd',
            array(
                sprintf('t1.dspid = %d', $this->dspid),
                sprintf("t1.unicod = '%s'", $this->unicod)
            )
        )[0]['qtd'];
    }
}
