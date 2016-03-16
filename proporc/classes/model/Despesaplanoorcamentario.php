<?php
/**
 * Classe de mapeamento da entidade proporc.despesaplanoorcamentario.
 *
 * $Id: Despesaplanoorcamentario.php 98916 2015-06-22 12:37:10Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.despesaplanoorcamentario.
 *
 * @see Modelo
 */
class Proporc_Model_Despesaplanoorcamentario extends Modelo
{
    /**
     * Tamanho de uma programatica completa: 26101.2109.00C5.0001.0000
     *
     * Unidade, programa, ação, localizador, plano orçamentário.
     */
    const TAMANHO_PROGRAMATICA_COMPLETA = 25;

    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela = 'proporc.despesaplanoorcamentario';

    /**
     * @var array Chave primaria.
     */
    protected $arChavePrimaria = array(
        'dpoid',
    );

    /**
     * @var array Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'dspid' => array('tabela' => 'proporc.despesa', 'pk' => 'dspid'),
    );

    /**
     * @var array Atributos
     */
    protected $arAtributos = array(
        'dpoid' => null,
        'dspid' => null,
        'unicod' => null,
        'prgcod' => null,
        'acacod' => null,
        'loccod' => null,
        'plocod' => null,
    );

    static public function queryTodosOsPlanos($exercicio)
    {
        return <<<DML
SELECT pao.unicod || '.' || pao.prgcod || '.' || pao.acacod || '.' || pao.loccod || '.' || plo.plocodigo AS codigo,
        pao.unicod || '.' || pao.prgcod || '.' || pao.acacod || '.' || pao.loccod || '.' || plo.plocodigo || ' - ' || plo.plotitulo AS descricao
  FROM elabrev.planoorcamentario plo
    INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid)
  WHERE pao.prgano = '{$exercicio}'
  ORDER BY pao.unicod,
           pao.acacod,
           pao.prgcod,
           plo.plocodigo,
           plo.plotitulo
DML;
    }

    static public function queryPlanosSelecionados($dspid, $exercicio)
    {
        $sql = <<<DML
SELECT t1.unicod || '.' || t1.prgcod || '.' || t1.acacod || '.' || t1.loccod || '.' || t1.plocod AS codigo,
       t1.unicod || '.' || t1.prgcod || '.' || t1.acacod || '.' || t1.loccod AS programatica,
       t1.plocod || '.' || plo.plotitulo AS descricao
  FROM proporc.despesaplanoorcamentario t1
    INNER JOIN elabrev.planoorcamentario plo ON plo.plocodigo = t1.plocod
    INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid, unicod, prgcod, acacod, loccod)
  WHERE t1.dspid = %d
    AND pao.prgano = '%s'
DML;

        return sprintf($sql, $dspid, $exercicio);
    }

    static public function querySelecaoDePlanoOrcamentario($exercicio, $where = '')
    {
        if (!empty($where)) {
            $where = " AND {$where}";
        }

        return <<<DML
SELECT pao.unicod || '.' || pao.prgcod || '.' || pao.acacod || '.' || pao.loccod || '.' || plo.plocodigo AS id,
       pao.unicod || '.' || pao.prgcod || '.' || pao.acacod || '.' || pao.loccod AS programatica,
       plo.plocodigo || ' - ' || plo.plotitulo AS descricao
  FROM elabrev.planoorcamentario plo
    INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid)
  WHERE prgano = '{$exercicio}'{$where}
  ORDER BY pao.unicod,
           pao.acacod,
           pao.prgcod,
           plo.plocodigo,
           plo.plotitulo
DML;
    }

    static public function queryUnidades($dspid, $exercicio)
    {
        return <<<DML
SELECT DISTINCT uni.unicod AS codigo,
                uni.unicod || ' - ' || uni.unidsc AS descricao
  FROM elabrev.planoorcamentario plo
    INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid)
    INNER JOIN public.unidade uni USING(unicod)
  WHERE pao.prgano = '{$exercicio}'
  ORDER BY uni.unicod
DML;
    }

    static public function queryAcoes($dspid, $exercicio)
    {
        return <<<DML
SELECT DISTINCT pao.acacod AS codigo,
                pao.acacod || ' - ' || pao.acadsc AS descricao
  FROM elabrev.planoorcamentario plo
    INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid)
  WHERE pao.prgano = '{$exercicio}'
  ORDER BY pao.acacod
DML;
    }

    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array())
    {
        if (self::TAMANHO_PROGRAMATICA_COMPLETA !== strlen($this->plocod)) {
            throw new Exception(
                'Para salvar uma associação de despesa e plano orçamentário, deve-se informar a programática completa.'
            );
        }

        list($this->unicod, $this->prgcod, $this->acacod, $this->loccod, $this->plocod)
            = explode('.', $this->plocod);

        return parent::salvar($boAntesSalvar, $boDepoisSalvar, $arCamposNulo);
    }
}
