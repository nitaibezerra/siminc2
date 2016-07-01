<?php

class Acomporc_Model_SnapshotSubacao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.snapshotsubacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ssaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ssaid' => null,
        'prfid' => null,
        'sbacod' => null,
        'ptres' => null,
        'metafisica' => null,
        'prddescricao' => null,
        'unmdescricao' => null,
        'plocod' => null,
        'plodsc' => null,
        'dataultimaatualizacao' => null,
        'vlrdotacao' => null,
        'vlrempenhado' => null,
        'vlrliquidado' => null,
        'vlrpago' => null
    );

    /**
     * Popular prfid com __set antes de chamar esta função.
     * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
     * @return array('sbacod','titulo','sigla','descricao','exercicio')|array()
     */
    public function listarSubacoesVinculadas($prgcod,$acacod,$loccod,$unicod)
    {
        $query = <<<DML
            SELECT DISTINCT
                ss.sbacod,
                ss.sbacod||' - '||sb.sbatitulo AS titulo,
                sb.sbasigla AS sigla,
                sb.sbadsc AS descricao,
                sb.sbacod AS codigo,
                pr.prsano AS exercicio
            FROM {$this->stNomeTabela} ss
            JOIN acomporc.periodoreferencia pr ON (pr.prfid = {$this->arAtributos['prfid']})
            JOIN monitora.ptres pt ON (ss.ptres = pt.ptres AND pt.ptrano = pr.prsano)
            JOIN monitora.pi_subacao sb ON (ss.sbacod = sb.sbacod AND sb.sbaano = pr.prsano)
            WHERE pt.prgcod = '{$prgcod}'
                AND pt.acacod = '{$acacod}'
                AND pt.loccod = '{$loccod}'
                AND pt.unicod = '{$unicod}'
                AND ss.prfid = (SELECT MAX(prfid) FROM acomporc.periodoreferencia pr2 WHERE pr2.prsano = pr.prsano AND pr2.prftipo = 'S');
DML;
        $result = $this->carregar($query);
        if(!$result){
            return array();
        }
        return $result;
    }
}