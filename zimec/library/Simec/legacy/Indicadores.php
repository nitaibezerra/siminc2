<?php

require_once dirname(__FILE__) . '/Indicadores/Renderer/Padrao.php';
require_once dirname(__FILE__) . '/Indicadores/Renderer/Bootstrap.php';

class Simec_Indicadores{

    const K_HTML_PADRAO = 'P';
    const K_HTML_BOOTSTRAP = 'B';

    protected $indids = array();
    protected $indicadores = array();
    protected $renderer;

    public function __construct($indicador = null, $incluirBiblioteca = true, $tipoHtml = self::K_HTML_BOOTSTRAP)
    {
        global $db;

        $this->indids = (array) $indicador;

        if(is_array($this->indids) && count($this->indids)){
            $sql = "select indid, acaid, perid, indnome, indcumulativo, indcumulativovalor from painel.indicador where indid in (" . implode(', ', $this->indids) . ") and indstatus = 'A'";
            $dados = $db->carregar($sql);
            $dados = $dados ? $dados : array();

            foreach ($dados as $dado) {
                $this->indicadores[$dado['indid']] = $dado;
            }
        }

        switch ($tipoHtml) {
            case self::K_HTML_BOOTSTRAP:
                $this->renderer = new Simec_Indicadores_Renderer_Bootstrap($incluirBiblioteca);
                break;
            default:
                $this->renderer = new Simec_Indicadores_Renderer_Padrao($incluirBiblioteca);
        }
    }

    /**
     * @return array
     */
    public function getIndids()
    {
        return $this->indids;
    }

    /**
     * @param array $indids
     */
    public function setIndids($indids)
    {
        $this->indids = $indids;
    }

    /**
     * @return array|void
     */
    public function getIndicadores()
    {
        return $this->indicadores;
    }

    /**
     * @param array|void $indicadores
     */
    public function setIndicadores($indicadores)
    {
        $this->indicadores = $indicadores;
    }

    /**
     * @return mixed
     */
    public function getRenderer()
    {
        return $this->renderer;
    }

    /**
     * @param mixed $renderer
     */
    public function setRenderer($renderer)
    {
        $this->renderer = $renderer;
    }



    public function getByAcaoEstrategica($acaid)
    {

    }

    public function getByRegionalizacao($regid)
    {

    }

    public function getByTema($temid)
    {

    }

    public function gerarGraficoComparativo($params = array())
    {
        $sql = $this->recuperarSqlGraficoLinha($params);
        $this->renderer->gerarGraficoLinha($sql);
//        ver($sql, d);
    }

    public function gerarGraficoDetalhes($params = array())
    {
        $sqls = $this->recuperarSqlDetalhes($params);
        $this->renderer->gerarGraficoDetalhes($sqls);
    }

    public function gerarResumoIndicador($indid = null)
    {
        global $db;
        $sql = "select ind.regid, reg.regdescricao, ind.perid, per.perdsc, ind.acaid, aca.acadsc, ind.secid, sec.secdsc,
                    ind.exoid, exo.exodsc, tem.temid, tem.temdsc, etp.etpid, etp.etpdsc,
                    ind.indid, ind.indnome, ind.indobjetivo
                from painel.indicador ind
                    inner join painel.regionalizacao reg on reg.regid = ind.regid
                    inner join painel.periodicidade per on per.perid = ind.perid
                    inner join painel.acao aca on aca.acaid = ind.acaid
                    inner join painel.secretaria sec on sec.secid = ind.secid
                    inner join painel.eixo exo on exo.exoid = ind.exoid
                    left join painel.indicadoretapaeducacao ie on ie.indid = ind.indid
                    left join painel.indicadortemamec it on it.indid = ind.indid
                    left join painel.etapaeducacao etp on etp.etpid = ie.etpid
                    left join painel.temamec tem on tem.temid = it.temid
                where ind.indid = {$indid} ";
        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();

        $dadosAgrupados = array();
        foreach($dados as $key => $dado){
            if(!$key){
                $dadosAgrupados = $dado;
                unset($dadosAgrupados['temid'], $dadosAgrupados['temdsc'], $dadosAgrupados['etpid'], $dadosAgrupados['etpdsc']);
            }
            $dadosAgrupados['temas'][] = array($dado['temid'] => $dado['temdsc'] );
            $dadosAgrupados['etapas'][] = array($dado['etpid'] => $dado['etpdsc'] );
        }
        $this->renderer->gerarResumoIndicador($dadosAgrupados);
    }

    public function gerarTabelaRelatorio($indid = null, $params = array(), $agrupadores = array())
    {
        global $db;
        $sql = "select distinct d.dpeanoref as periodo
                from  painel.v_detalheindicadorsh d
                where sehstatus <> 'I'
                and d.indid = {$indid}
                order by dpeanoref";
        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();

        $periodos = array();
        foreach($dados as $periodo){
            $periodos[] = $periodo['periodo'];
        }

        $sql = "select estuf, d.dpeanoref, sum(qtde+valor) as valor
                from  painel.v_detalheindicadorsh d
                where sehstatus <> 'I'
                and d.indid = {$indid}
                group by d.dpeanoref, estuf
                order by estuf";
        $dados = $db->carregar($sql);
        $dados = $dados ? $dados : array();

        $dadosAgrupados = array();
        foreach($dados as $dado){
            $dadosAgrupados[$dado['estuf']][$dado['dpeanoref']] = $dado['valor'];
        }

        $this->renderer->gerarTabelaRelatorio($periodos, $dadosAgrupados);
    }

    public function recuperarSqlGraficoLinha($params = array())
    {
        global $db;

        $whereLinha = array();
        $whereWith = $with = $sqlUnion = $sqlWith = '';

        $filtros = $this->recuperarFiltros($params);
        $where = $filtros['where'];
        $join  = $filtros['join'];

        $where .= " and d.indid in ( " . implode(', ', $this->indids) . " )";

        foreach($this->indids as $key => $indid){
            // Iniciando configuração do union de interseção
            if(!$key){
                $whereLinhaWith = $whereLinha;
            } else {
                $with[] = "
                    tmp{$key} as (SELECT distinct d.dshcod, dpeanoref FROM painel.v_detalheindicadorsh d WHERE  d.indid= $indid) ";

                $whereWith .= "
                    and exists ( select 1 from tmp{$key} t where (t.dshcod, t.dpeanoref) = (d.dshcod, d.dpeanoref) )";
            }
        }

        if(is_array($with) && count($with)){
            $with = 'with ' . implode (', ', $with);

            $sqlUnion .= ' union ';
            $sqlWith .= $this->recuperarSqlCruzamento($join, $where);
        }


//        $sql = $with;
        $sql .= $this->recuperarSqlCruzamento($join, $where);
//        $sql .= $sqlUnion;
//        $sql .= $sqlWith;

        $sql .= 'order by categoria';
        return $sql;
/*
select case
       when d.indid = 723 then 'Matrículas em escolas públicas rurais sem energia...'
   end as descricao,
   d.dpeanoref as categoria, sum(qtde)+sum(valor) as valor
from  painel.v_detalheindicadorsh d

where sehstatus <> 'I'
and d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = 150)
and d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = 1224)
and (
  ( d.indid = 723 )
)

group by descricao, d.dpeanoref
order by categoria


-----------------------------------------------------------------------
Não Acumulada
-----------------------------------------------------------------------

with coletanaoacumulada as (
	select max(dpedatainicio) as dpedatainicio, dpeanoref
	from  painel.v_detalheindicadorsh d
	where sehstatus <> 'I'
	and d.indid in (547)
	group by dpeanoref
)

select sum(valor + qtde), d.dpeid, d.dpedsc, d.dpedatainicio, d.dpeanoref
from  painel.v_detalheindicadorsh d
	inner join coletanaoacumulada c on c.dpedatainicio = d.dpedatainicio
where sehstatus <> 'I'
and d.indid in (547)
group by dpeid, dpedsc, d.dpedatainicio, d.dpeanoref
order by dpedatainicio

*/

    }

    function recuperarSqlDetalhes($params = array())
    {
        global $db;
        $sqls = array();
        $filtros = $this->recuperarFiltros($params);
        $where = $filtros['where'];
        $join  = $filtros['join'];

        foreach ($this->indicadores as $indid => $indicador) {
            $sqlDetalhes = "select tdinumero, tdidsc from painel.detalhetipoindicador where indid = $indid";
            $detalhes = $db->carregar($sqlDetalhes);
            $detalhes = $detalhes ? $detalhes : array();

            if(count($detalhes)){ ?>
                <?php foreach($detalhes as $detalhe) {
                    $sql = "select tiddsc{$detalhe['tdinumero']} descricao, d.dpeanoref categoria, sum(qtde+valor) as valor
                            from  painel.v_detalheindicadorsh d
                                " . implode(' ', $join).  "
                            where d.indid = {$indid}
                            and sehstatus <> 'I'
                            $where
                            group by descricao, d.dpeanoref
                            order by d.dpeanoref";
                    $sqls[$indid]['sql'][] = $sql;
                    $sqls[$indid]['indnome'] = $indicador['indnome'];
                    $sqls[$indid]['tdidsc'][] = $detalhe['tdidsc'];
                    ?>
                <?php } ?>
            <?php }

        }
        return $sqls;
    }

    function recuperarSqlCruzamento($join, $where)
    {
        $sql = "
            select d.indnome as descricao,
                   d.dpeanoref as categoria, sum(qtde+valor) as valor
            from  painel.v_detalheindicadorsh d
                " . implode(' ', $join).  "
            where sehstatus <> 'I'
            $where
            group by descricao, d.dpeanoref
            ";

        return $sql;
    }

    function recuperarFiltros($params = array())
    {
        $where = '';
        $join = array();

        if (isset($params['regcod']) && is_array($params['regcod'])) {
            $where .= " and d.regcod in ('" . implode ("', '", $params['regcod']) . "') ";
        }
        if (isset($params['estuf']) && is_array($params['estuf'])) {
            $where .= " and d.estuf in ('" . implode ("', '", $params['estuf']) . "') ";
        }
        if (isset($params['muncod']) && is_array($params['muncod'])) {
            $where .= " and d.muncod in ('" . implode ("', '", $params['muncod']) . "') ";
        }
        if (isset($params['tpmid']) && is_array($params['tpmid'])) {
            $where .= " and mtm.tpmid in ('" . implode ("', '", $params['tpmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = d.muncod ';
        } elseif (isset($params['gtmid']) && is_array($params['gtmid'])) {
            $where .= " and tm.gtmid in ('" . implode ("', '", $params['gtmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = d.muncod ';
            $join[] = ' inner join territorios.tipomunicipio tm on tm.tpmid = mtm.tpmid ';
        }
        return array(
            'where' => $where,
            'join' => $join,
        );
    }
}
