<?php


/**
 * Description of Snapshot
 *
 * @author LindalbertoFilho
 */
class Spo_Model_SnapshotSubacao extends Modelo
{
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

    private $queryCarga = <<<DML
        INSERT INTO acomporc.snapshotsubacao (prfid, sbacod, ptres, metafisica, prddescricao, unmdescricao, plocod, plodsc, dataultimaatualizacao, vlrdotacao, vlrempenhado, vlrliquidado, vlrpago)
            SELECT
                pr.prfid,
                ps.sbacod,
                p.ptres,
                0 AS metafisica,
                a.unmdsc AS unmdescricao,
                coalesce(a.prodsc,prds.prodsc) AS prddescricao,
                po.plocodigo AS plocod,
                po.plotitulo AS plodsc,
                ps.sbadata AS dataultimaatualizacao,
                coalesce(psd.sadvalor,0.00) AS vlrdotacao,
                coalesce(ptr.total,0.00) AS vlrempenhado,
                coalesce(sum(vlrliquidado),0.00) AS vlrliquidado,
                coalesce(sum(vlrpago),0.00) AS vlrpago
            FROM acomporc.periodoreferencia pr
            INNER JOIN monitora.pi_subacao ps ON (pr.prsano = ps.sbaano AND ps.sbastatus = 'A')
            INNER JOIN monitora.pi_subacaodotacao psd ON (ps.sbaid = psd.sbaid)
            INNER JOIN monitora.ptres p ON (psd.ptrid = p.ptrid AND p.ptrstatus = 'A')
            INNER JOIN monitora.acao a ON (p.acaid = a.acaid)
            LEFT JOIN monitora.produtosof prds ON (a.procod = prds.procodsof )
            LEFT JOIN monitora.planoorcamentario po ON (p.plocod = po.plocodigo AND p.acaid = po.acaid AND pr.prsano = po.exercicio)
            LEFT JOIN siafi.ptrempenho ptr ON (p.ptres = ptr.ptres AND pr.prsano = ptr.exercicio)
            LEFT JOIN spo.siopexecucao se ON (substr(se.plicod,2,4) = ps.sbacod AND se.exercicio = pr.prsano AND se.ptres = p.ptres AND se.anoreferencia = se.exercicio)
            WHERE pr.prfid = %d
            GROUP BY pr.prfid, ps.sbacod, p.ptres, metafisica, a.unmdsc, prddescricao, po.plocodigo, po.plotitulo, ps.sbadata, vlrdotacao, ptr.total
            ORDER BY ps.sbacod;
DML;

    private $queryCargaUsuResponsabilidade = <<<DML
        INSERT INTO acomporc.usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc , prfid , sbacod)
            SELECT
                pflcod,
                usucpf,
                'A',
                rpudata_inc,
                %s,
                sbacod
            FROM acomporc.usuarioresponsabilidade
            WHERE pflcod = 1351
                AND rpustatus = 'A'
                AND prfid = (SELECT prfid FROM acomporc.periodoreferencia WHERE prsano = '%s' AND prftipo = 'S' ORDER BY prfid DESC LIMIT 1)
DML;

    private $queryUsuResponsabilidadeDelete = <<<DML
        UPDATE acomporc.usuarioresponsabilidade SET rpustatus = 'I' WHERE pflcod = 1351 AND prfid = %s
DML;
    public function inserirCarga()
    {
        $query = sprintf($this->queryCarga,$this->arAtributos['prfid']);
        if($this->executar($query)){
            return $this->commit();
        }
        return false;
    }

    public function inserirCargaUsuResponsabilidade()
    {
        $query = sprintf($this->queryCargaUsuResponsabilidade,$this->arAtributos['prfid'],((int) $_SESSION['exercicio']) - 1);
        if($this->executar($query)){
            return $this->commit();
        }
        return false;
    }


    public function deletarCargaUsuResponsabilidade()
    {
        $query = sprintf($this->queryUsuResponsabilidadeDelete,$this->arAtributos['prfid']);
        if($this->executar($query)){
            return $this->commit();
        }
        return false;
    }
}
