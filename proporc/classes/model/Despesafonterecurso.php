<?php
/**
 * Classe de mapeamento da entidade proporc.despesafonterecurso.
 *
 * $Id: Despesafonterecurso.php 99948 2015-07-09 20:27:35Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.despesafonterecurso.
 *
 * @see Modelo
 */
class Proporc_Model_Despesafonterecurso extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.despesafonterecurso';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dsfid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'foncod' => array('tabela' => 'public.fonterecurso', 'pk' => 'foncod'),
        'dspid' => array('tabela' => 'proporc.despesa', 'pk' => 'dspid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dsfid' => null,
        'dspid' => null,
        'foncod' => null,
    );

    static public function queryTodasAsFontes()
    {
        return <<<DML
SELECT f.foncod AS codigo,
       f.foncod || ' - ' || f.fondsc AS descricao
  FROM public.fonterecurso f
  WHERE f.fonstatus = 'A'
DML;
    }

    static public function queryFontesSelecionadas($dspid)
    {
        $sql = self::queryTodasAsFontes()
            . <<<DML
    AND EXISTS (SELECT 1
                  FROM proporc.despesafonterecurso dpf
                  WHERE f.foncod = dpf.foncod
                    AND dpf.dspid = %d)
DML;
        return sprintf($sql, $dspid);
    }

    public function fonteValida()
    {
        if (empty($this->dspid) || empty($this->foncod)) {
            throw new Exception("Para verificar se uma fonte existe, informe o 'dspid' e o 'foncod'.");
        }

        return (bool)$this->recuperarTodos(
            'COUNT(1) AS qtd',
            array(
                sprintf("t1.dspid = '%d'", $this->dspid),
                sprintf("t1.foncod = '%s'", $this->foncod)
            )
        )[0]['qtd'];
    }
}