<?php
/**
 * Classe de mapeamento da entidade proporc.despesagnd.
 *
 * $Id: Despesagnd.php 98059 2015-06-02 12:21:46Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.despesagnd.
 *
 * @see Modelo
 */
class Proporc_Model_Despesagnd extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.despesagnd';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dsgid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'gndcod' => array('tabela' => 'public.gnd', 'pk' => 'gndcod'),
        'dspid' => array('tabela' => 'proporc.despesa', 'pk' => 'dspid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dsgid' => null,
        'dspid' => null,
        'gndcod' => null,
    );

    static public function queryTodosOsGnds()
    {
        return <<<DML
SELECT g.gndcod AS codigo,
       g.gndcod || ' - ' || g.gnddsc AS descricao
  FROM public.gnd g
  WHERE g.gndstatus = 'A'
DML;
    }

    static public function queryGndsSelecionados($dspid)
    {
        $sql = self::queryTodosOsGnds()
            . <<<DML
    AND EXISTS (SELECT 1
                  FROM proporc.despesagnd dpg
                  WHERE g.gndcod = dpg.gndcod
                    AND dpg.dspid = %d)
DML;
        return sprintf($sql, $dspid);
    }
}