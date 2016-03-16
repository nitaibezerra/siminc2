<?php
/**
 * Description of Snapshot
 *
 * @author LindalbertoFilho
 */
class Spo_Model_SnapshotLocalizador extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.snapshotlocalizador";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sslid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sslid' => NULL,
        'acaid' => NULL,
        'prfid' => NULL,
        'acacod' => NULL,
        'unicod' => NULL,
        'loccod' => NULL,
        'funcod' => NULL,
        'sfncod' => NULL,
        'prgcod' => NULL,
        'tipoinclusaoacao' => NULL,
        'tipoinclusaolocalizador' => NULL,
        'prddescricao' => NULL,
        'unmdescricao' => NULL,
        'dotacaoinicial' => NULL,
        'dotacaoatual' => NULL,
        'empenhado' => NULL,
        'liquidado' => NULL,
        'pago' => NULL,
        'rapinscritoliquido' => NULL,
        'rapliquidadoapagar' => NULL,
        'rappago' => NULL,
        'rapliquidadoefetivo' => NULL,
        'metafisica' => NULL,
        'financeiro' => NULL,
        'dataultimatualizacao' => NULL
    );

    private $queryCarga = <<<DML
        INSERT INTO acomporc.snapshotlocalizador (acaid,prfid,acacod,unicod,loccod,funcod,sfncod,prgcod,tipoinclusaoacao,tipoinclusaolocalizador,
            prddescricao,unmdescricao,dotacaoinicial,dotacaoatual,empenhado,liquidado,pago,rapinscritoliquido,rapliquidadoapagar,rappago,
            rapliquidadoefetivo,metafisica,financeiro,dataultimatualizacao)
            SELECT
                aca.acaid,
                pr.prfid as prfid,
                aca.acacod,
                aca.unicod,
                aca.loccod,
                aca.funcod,
                aca.sfucod,
                aca.prgcod,
                COALESCE(aca.codtipoinclusaoacao, 1) as tipoinclusaoacao,
                COALESCE(aca.codtipoinclusaolocalizador, 1) as tipoinclusaolocalizador,
                aca.prodsc,
                aca.unmdsc,
                COALESCE(SUM(se.vlrdotacaoinicial),0),
                COALESCE(SUM(se.vlrdotacaoatual),0),
                COALESCE(SUM(se.vlrempenhado),0),
                COALESCE(SUM(se.vlrliquidado),0),
                COALESCE(SUM(se.vlrpago),0),
                COALESCE(SUM(se.vlrrapnaoprocessadoinscritoliquido),0),
                COALESCE(SUM(se.vlrrapnaoprocessadoliquidadoapagar),0),
                COALESCE(SUM(se.vlrrapnaoprocessadopago),0),
                COALESCE(SUM(se.vlrrapnaoprocessadoliquidadoefetivo),0),
                COALESCE(aca.metalocalizador,0),
                COALESCE(aca.financeirolocalizador,0),
                se.dtatualizacao
            FROM acomporc.periodoreferencia pr
            INNER JOIN monitora.acao aca ON (aca.prgano = pr.prsano)
            INNER JOIN spo.siopexecucao se ON (aca.prgano = se.exercicio AND aca.acacod = se.acacod AND aca.unicod = se.unicod AND aca.loccod = se.loccod AND aca.prgano = se.anoreferencia)
            WHERE pr.prfid = %d
            GROUP BY aca.acaid,prfid,aca.acacod,aca.unicod,aca.loccod,aca.funcod,aca.sfucod,aca.prgcod,tipoinclusaoacao,tipoinclusaolocalizador,aca.prodsc,aca.unmdsc,aca.metalocalizador,aca.financeirolocalizador,se.dtatualizacao
            ORDER BY aca.acaid
DML;

    private $queryCargaUsuResponsabilidade = <<<DML
        INSERT INTO acomporc.usuarioresponsabilidade (usucpf, rpustatus, rpudata_inc , prfid , unicod)
            SELECT
                usucpf,
                'A',
                rpudata_inc,
                %s,
                unicod
            FROM acomporc.usuarioresponsabilidade
            WHERE pflcod IS NULL
                AND rpustatus = 'A'
                AND prfid = (SELECT prfid FROM acomporc.periodoreferencia WHERE prsano = '%s' AND prftipo = 'A' ORDER BY prfid DESC LIMIT 1)
DML;

    private $queryUsuResponsabilidadeDelete = <<<DML
        UPDATE acomporc.usuarioresponsabilidade SET rpustatus = 'I' WHERE pflcod IS NULL AND prfid = %s
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
