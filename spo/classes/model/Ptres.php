<?php
/**
 * Classe de mapeamento da entidade monitora.ptres.
 *
 * $Id: Ptres.php 100401 2015-07-22 21:06:58Z maykelbraz $
 */

/**
 * Mapeamento da entidade monitora.ptres.
 *
 * @see Modelo
 */
class Spo_Model_Ptres extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.ptres';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ptrid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acaid' => array('tabela' => 'acao', 'pk' => 'acaid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ptrid' => null,
        'ptres' => null,
        'acaid' => null,
        'ptrano' => null,
        'funcod' => null,
        'sfucod' => null,
        'prgcod' => null,
        'acacod' => null,
        'loccod' => null,
        'unicod' => null,
        'irpcod' => null,
        'ptrdotacao' => null,
        'ptrstatus' => null,
        'ptrdata' => null,
        'plocod' => null,
        'esfcod' => null,
    );

    public static function queryCombo($exercicio, $obrigatorias = false)
    {
        if ($obrigatorias) {
            $obrigatorias = ' AND aca.unicod NOT IN(' . Spo_Model_Unidade::getObrigatorias(true) . ')';
        } else {
            $obrigatorias = '';
        }

        return <<<DML
SELECT pt.ptrid AS codigo,
       '(PTRES:'||pt.ptres||') - '|| aca.unicod ||'.'|| aca.prgcod ||'.'|| aca.acacod AS descricao
  FROM monitora.ptres pt
    INNER JOIN monitora.acao aca USING(acaid)
  WHERE aca.prgano = '{$exercicio}'
    AND pt.ptrano = '{$exercicio}'
    AND aca.acasnrap = false
    {$obrigatorias}
  GROUP BY codigo,
           descricao
  ORDER BY 1
DML;
    }
}