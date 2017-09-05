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

    public function recuperarPtresSubunidade($prsano, $tipo = null)
    {
        switch ($tipo){
            // Somente Vinculadas
            case 'V':
                $where = "and uo.unocod not in ('42101', '42902', '74912')";
                break;
            // Somente Administração Direta
            case 'D':
                $where = "and uo.unocod in ('42101')";
                break;
            // Somente Fundo
            case 'F':
                $where = "and uo.unocod in ('42902', '74912')";
                break;
            // Todas
            default:
                $where = '';
        }

        $sql = "select  p.ptrid, p.ptres, p.acaid, p.ptrano, p.funcod, p.sfucod, p.prgcod, p.acacod, p.loccod, p.plocod, p.esfcod,  
                        uo.unocod, uo.unonome, uo.suocod, uo.suonome, uo.unofundo, uo.suosigla, uo.unosigla, uo.unoid, uo.suoid
                from monitora.ptres p
                        inner join public.vw_subunidadeorcamentaria uo on uo.unocod = p.unicod and uo.prsano = '$prsano' and uo.suostatus = 'A'
                where ptrano = '$prsano'
                and p.ptrstatus = 'A'
                $where
                order by uo.unofundo, uo.unonome, uo.suonome, p.acacod, p.prgcod, p.loccod, p.plocod";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}