<?php

class Pi_PlanoInterno extends Modelo
{

    const K_TIPO_DETALHE_PTRES = 'PT';
    const K_TIPO_DETALHE_PI = 'PI';
    const K_TIPO_DETALHE_DELEGADO = 'DL';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitora.pi_planointerno";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("pliid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pliid' => null,
        'mdeid' => null,
        'eqdid' => null,
        'neeid' => null,
        'capid' => null,
        'sbaid' => null,
        'obrid' => null,
        'plisituacao' => null,
        'plititulo' => null,
        'plidata' => null,
        'plistatus' => null,
        'plicodsubacao' => null,
        'plicod' => null,
        'plilivre' => null,
        'plidsc' => null,
        'usucpf' => null,
        'unicod' => null,
        'ungcod' => null,
        'pliano' => null,
        'docid' => null,
    );

    public function recuperarPropostasSiminc($exercicio, $aFiltro)
    {
        $where = [];
        if (!empty($aFiltro['uno'])) {
            $where[] = " AND uno.unoid = {$aFiltro['uno']}";
        }

        # Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
        $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object)array('usucpf' => $_SESSION['usucpf']));

        if (count($listaSubUnidadeUsuario)) {
            $where[] = " AND  suo.suocod IN('" . join("','", $listaSubUnidadeUsuario) . "') ";
        }

        $sql = "SELECT  ppi.ppiid, ppi.ppinome, ppi.ppidescricao, uno.unonome, suo.suonome, mom.momnome, fun.funcionalcampobusca, 
                        CASE 
                                WHEN NOT ppi.maiid IS NULL THEN COALESCE(ppi.ppinome,mai.mainome)
                                ELSE ppi.ppinome
                        END AS ppinome,
                        eqd.eqdnome, are.arenome, pri.prinome, ump.umpnome, esd.esddsc, ppi.ppiqtdmeta, ppi.ppivlrcusteio, ppi.ppivlrcapital,
                        ( COALESCE(ppi.ppivlrcusteio,0) + COALESCE(ppi.ppivlrcapital,0) ) AS total,
                        COALESCE(ims.imsvlrdespesaempenhada, 0.00) AS imsvlrdespesaempenhada,
                        COALESCE(ims.imsvlrdespesaliquidada, 0.00) AS imsvlrdespesaliquidada,
                        COALESCE(ims.imsvlrpago, 0.00) AS imsvlrpago,
                        CASE
                            WHEN ( ppi.ppivlrcusteio + ppi.ppivlrcapital ) = 0 THEN '0.00%'
                            ELSE ROUND(COALESCE(ims.imsvlrdespesaempenhada, 0) / ( ppi.ppivlrcusteio + ppi.ppivlrcapital ) * 100, 2)  || '%'
                        END AS percentualempenhadoaprovado,
                        COALESCE(dpisoma.vlrdetalhamento, 0.00) AS vlrdetalhamento,
                        mom.momflgpossuidetalhe, ppi.ppiid, eqd.eqdid, mom.momflgpossuidetalhe, mom.momid, ppi.docid
                FROM planointerno.propostapi ppi
                JOIN planointerno.ppamomentounidade pmu ON ppi.pmuid = pmu.pmuid AND pmu.pmustatus = 'A' AND pmu.prsano = '$exercicio'
                JOIN planointerno.subunidadeorcamentaria suo ON suo.suoid = pmu.suoid AND suo.suostatus = 'A'
                JOIN planointerno.momento mom ON mom.momid = pmu.momid AND mom.momstatus = 'A'
                left JOIN planointerno.vw_funcional_2016 fun ON fun.pmuid = pmu.pmuid
                JOIN planointerno.enquadramentodespesa eqd ON eqd.eqdid = ppi.eqdid AND eqd.eqdstatus = 'A'
                LEFT JOIN planointerno.areacultural are ON are.areid = ppi.areid AND are.arestatus = 'A'
                JOIN planointerno.produtopi pri ON pri.priid = ppi.priid AND pri.pristatus = 'A'
                LEFT JOIN planointerno.unidademedidapi ump ON ump.umpid = ppi.umpid AND ump.umpstatus = 'A'
                JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid AND uno.unostatus = 'A'
                LEFT JOIN planointerno.manutencaoitem mai ON mai.maiid = ppi.maiid
                LEFT JOIN workflow.documento doc ON doc.docid = ppi.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
                LEFT JOIN ( 
                        SELECT  SUM(imsvlrdespesaliquidada) AS imsvlrdespesaliquidada, SUM(imsvlrpago) AS imsvlrpago, SUM(imsvlrdespesaempenhada) AS imsvlrdespesaempenhada, dpi2.ppiid
                        FROM planointerno.importacaosiafi ims2
                                JOIN planointerno.detalhepi dpi2 ON dpi2.dpicodigopi = ims2.imsnumeropi AND dpi2.dpistatus = 'A'
                        WHERE ims2.imsexercicio = '$exercicio'
                        GROUP BY 4
                ) ims ON ppi.ppiid = ims.ppiid
                LEFT JOIN ( 
                        SELECT SUM( ( dpivlrcusteio + dpivlrcapital ) ) AS vlrdetalhamento, ppiid
                        FROM planointerno.detalhepi
                        WHERE dpistatus = 'A'
                        GROUP BY ppiid
                ) dpisoma ON dpisoma.ppiid = ppi.ppiid
                WHERE mom.momid = (select momid from planointerno.momento where prsano = '$exercicio' AND momstatus = 'A' order by momid limit 1)
                AND  ppi.ppistatus = 'A'"
            . implode(' ', $where) .
            "order by uno.unonome, suo.suonome";

        // Carregando dados da base de dados do SIMINC antigo
        $dados = adapterConnection::oldSiminc()->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarDadosPropostasSiminc($ppiid)
    {
        $sql = "select  ppi.*, pmu.prsano, uno.unoid, uno.unonome, uno.unocod, suo.suoid, suo.suonome, suo.suocod, aca.acaid, aca.acacod, prg.prgid, prg.prgcod, loc.locid, loc.loccod, plo.ploid, plo.plocod,
                        eqd.eqdnome, mai.mainome, masnome, obj.objid, obj.objcod, objnome, ini.iniid, ini.inicod, ini.ininome, mpp.mppid, mpp.mppcod, mpp.mppnome, mpn.mpnid, mpn.mpncod, mpn.mpnnome, 
                        are.areid, arenome, pri.priid, pri.prinome, ump.umpid, ump.umpnome, eqd.eqdsigla, are.aresigla
                from planointerno.propostapi ppi
                        INNER JOIN planointerno.ppamomentounidade pmu ON ppi.pmuid = pmu.pmuid AND pmu.pmustatus = 'A'
                        INNER JOIN planointerno.ppaacao aca on aca.acaid = pmu.acaid AND aca.acastatus = 'A'
                        INNER JOIN planointerno.ppaprograma prg on prg.prgid = pmu.prgid AND prg.prgstatus = 'A'        
                        INNER JOIN planointerno.ppalocalizador loc on loc.locid = pmu.locid AND loc.locstatus = 'A'
                        INNER JOIN planointerno.ppaplanoorcamentario plo on plo.ploid = pmu.ploid AND plo.plostatus = 'A'
                        INNER JOIN planointerno.subunidadeorcamentaria suo ON suo.suoid = pmu.suoid AND suo.suostatus = 'A'
                        INNER JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid AND uno.unostatus = 'A'
                        INNER JOIN planointerno.enquadramentodespesa eqd ON eqd.eqdid = ppi.eqdid AND eqd.eqdstatus = 'A'
                        LEFT JOIN planointerno.areacultural are ON are.areid = ppi.areid AND are.arestatus = 'A'
                        LEFT JOIN planointerno.produtopi pri ON pri.priid = ppi.priid AND pri.pristatus = 'A'
                        LEFT JOIN planointerno.unidademedidapi ump ON ump.umpid = ppi.umpid AND ump.umpstatus = 'A'        
                        LEFT JOIN planointerno.manutencaoitem mai ON mai.maiid = ppi.maiid AND mai.maistatus = 'A'
                        LEFT JOIN planointerno.manutencaosubitem mas on mas.masid = ppi.masid AND mas.masstatus = 'A'
                        LEFT JOIN planointerno.ppaobjetivo obj on obj.objid = pmu.objid AND obj.objstatus = 'A'
                        LEFT JOIN planointerno.ppainiciativa ini on ini.iniid = pmu.iniid AND ini.inistatus = 'A'
                        LEFT JOIN planointerno.metappa mpp on mpp.mppid = ppi.mppid AND mpp.mppstatus = 'A'
                        LEFT JOIN planointerno.metapnc mpn on mpn.mpnid = ppi.mpnid AND mpn.mpnstatus = 'A'        
                where ppi.ppiid = $ppiid";

        // Carregando dados da base de dados do SIMINC antigo
        $dadosProposta = adapterConnection::oldSiminc()->pegaLinha($sql);

//        ver($dadosProposta, $sql, d);

        $dadosPI = [
            'plititulo' => $dadosProposta['ppinome'],
            'plidsc' => $dadosProposta['ppidescricao'],
            'pliano' => $dadosProposta['prsano'],
            'unicod' => $dadosProposta['unocod'],
            'ungcod' => $dadosProposta['suocod'],
            'picquantidade' => $dadosProposta['ppiqtdmeta'],
            'picvalorcusteio' => $dadosProposta['ppivlrcusteio'],
            'picvalorcapital' => $dadosProposta['ppivlrcapital'],
        ];

        // Carregando dados do SIMINC 2 de acordo com dados do SIMINC antigo
        $dadosPI['eqdid'] = $this->pegaUm("select eqdid from monitora.pi_enquadramentodespesa eqd where eqdcod = '{$dadosProposta['eqdsigla']}' AND eqd.eqdano = '{$dadosProposta['prsano']}' AND eqd.eqdstatus = 'A'");
        $dadosPI['maiid'] = $this->pegaUm("select maiid from planacomorc.manutencaoitem mai where upper(mai.mainome) = upper('{$dadosProposta['mainome']}') AND mai.prsano = '{$dadosProposta['prsano']}' AND mai.maistatus = 'A'");
        $dadosPI['masid'] = $this->pegaUm("select masid from planacomorc.manutencaosubitem mas where upper(mas.masnome) = upper('{$dadosProposta['masnome']}') AND mas.prsano = '{$dadosProposta['prsano']}' AND mas.masstatus = 'A'");
        $dadosPI['mppid'] = $this->pegaUm("select mppid from public.metappa mpp where mpp.mppcod = '{$dadosProposta['mppcod']}' AND mpp.prsano = '{$dadosProposta['prsano']}' AND mpp.mppstatus = 'A'");
        $dadosPI['mdeid'] = $this->pegaUm("select mdeid from monitora.pi_modalidadeensino mde where upper(mdedsc) = upper('{$dadosProposta['arenome']}') AND mde.mdeano = '{$dadosProposta['prsano']}' AND mde.mdestatus = 'A'");
        $dadosPI['pprid'] = $this->pegaUm("select pprid from monitora.pi_produto ppr where upper(ppr.pprnome) = upper('{$dadosProposta['prinome']}') AND prsano = '{$dadosProposta['prsano']}' AND ppr.pprstatus = 'A'");
        $dadosPI['pumid'] = $this->pegaUm("select pumid from monitora.pi_unidade_medida pum where upper(pum.pumnome) = upper('{$dadosProposta['umpnome']}') AND pum.prsano = '{$dadosProposta['prsano']}' AND pum.pumstatus = 'A'");
        $dadosPI['oppid'] = $this->pegaUm("select oppid from public.objetivoppa opp where opp.oppcod = '{$dadosProposta['objcod']}' AND opp.prsano = '{$dadosProposta['prsano']}' AND opp.oppstatus = 'A'");
        $dadosPI['ippid'] = $this->pegaUm("select ippid from public.iniciativappa ipp where ipp.ippcod = '{$dadosProposta['inicod']}' AND ipp.prsano = '{$dadosProposta['prsano']}' AND ipp.ippstatus = 'A'");

        $MetaPnc = $this->pegaLinha("select mpnid, ipnid from public.indicadorpnc ipn where ipn.ipncod = '" . trim($dadosProposta['mpncod']) . "' AND ipn.prsano = '{$dadosProposta['prsano']}' AND ipn.ipnstatus = 'A'");

        $dadosPI['mpnid'] = $MetaPnc['mpnid'];
        $dadosPI['ipnid'] = $MetaPnc['ipnid'];

        // Recuperando PTRES
        $sql = "select ptrid 
                from monitora.ptres ptr
                where ptr.acacod = '{$dadosProposta['acacod']}'
                AND ptr.prgcod = '{$dadosProposta['prgcod']}'
                AND ptr.loccod = '{$dadosProposta['loccod']}'
                AND ptr.unicod = '{$dadosProposta['unocod']}'
                AND ptr.plocod = '{$dadosProposta['plocod']}'
                AND ptr.ptrano = '{$dadosProposta['prsano']}'
                AND ptr.ptrstatus = 'A'";

        $dadosPI['ptrid'] = $this->pegaUm($sql);

        return $dadosPI;
    }

    public function montarSqlExecucaoOrcamentaria($filtro = []){
        $filtro[] = "suo.suostatus = 'A'";
        $where = $this->montarFiltros($filtro);

        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suosigla,
                suo.suocod,
                suo.suonome,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                psu.ptrdotacaocapital ptrdotacao_capital,
                psu.ptrdotacaocusteio ptrdotacao_custeio,                
                sum(COALESCE(pic.picvalorcapital, 0)) picvalorcapital,
                sum(COALESCE(pic.picvalorcusteio, 0)) picvalorcusteio,
                (
                    SELECT
                        COUNT(1)
                    FROM spo.ptressubunidade
                    WHERE
                        ptressubunidade.ptrid = ptr.ptrid
                ) AS compartilhada
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
                LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
                LEFT JOIN monitora.pi_planointerno pli on(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND plistatus = 'A')
                LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                LEFT JOIN planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
            WHERE 
                {$where}
            GROUP BY
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                suo.suosigla,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                ptrdotacao_capital,
                ptrdotacao_custeio
            ORDER BY
                suo.unonome,
                suo.suonome,
                ptr.funcional
        ";

        return $sql;
    }
    public function montarSqlRelFuncionais($filtro = []){
        $filtro[] = "suo.suostatus = 'A'";
        $where = $this->montarFiltros($filtro);
//ver($filtro, d);
        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suosigla,
                suo.suocod,
                suo.suonome,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                ptr.ptres,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                psu.ptrdotacaocapital ptrdotacao_capital,
                psu.ptrdotacaocusteio ptrdotacao_custeio,                
                SUM(COALESCE(pic.picvalorcapital, 0)) picvalorcapital,
                SUM(COALESCE(pic.picvalorcusteio, 0)) picvalorcusteio,
		COALESCE(sec.provisionado, 0.00) AS provisionado,
		COALESCE(sec.empenhado, 0.00) AS empenhado,
		COALESCE(sec.liquidado, 0.00) AS liquidado,
		COALESCE(sec.pago, 0.00) as pago,
                (
                    SELECT
                        COUNT(1)
                    FROM spo.ptressubunidade
                    WHERE
                        ptressubunidade.ptrid = ptr.ptrid
                ) AS compartilhada
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu on psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
                LEFT JOIN monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
                LEFT JOIN monitora.pi_planointerno pli on(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND plistatus = 'A')
                LEFT JOIN planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                LEFT JOIN planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
                LEFT JOIN(
                    SELECT
                        siopexecucao.unicod,
                        pi_planointerno.ungcod,
                        siopexecucao.ptres,
                        SUM(COALESCE(siopexecucao.vlrautorizado, 0.00))::NUMERIC AS provisionado,
                        SUM(COALESCE(siopexecucao.vlrempenhado, 0.00))::NUMERIC AS empenhado,
                        SUM(COALESCE(siopexecucao.vlrliquidado, 0.00))::NUMERIC AS liquidado,
                        SUM(COALESCE(siopexecucao.vlrpago, 0.00))::NUMERIC AS pago
                    FROM spo.siopexecucao
                        LEFT JOIN monitora.pi_planointerno ON(siopexecucao.plicod = pi_planointerno.plicod AND siopexecucao.exercicio = pi_planointerno.pliano AND pi_planointerno.plistatus = 'A')
                    WHERE
                        siopexecucao.exercicio = '". (int)$_SESSION['exercicio']. "' -- Inserindo o ano direto na subquery por motivo de performance da consulta.
                    GROUP BY
                        siopexecucao.ptres,
                        siopexecucao.unicod,
                        pi_planointerno.ungcod
		) sec ON sec.ptres = ptr.ptres AND sec.unicod = pli.unicod AND sec.ungcod = pli.ungcod
            WHERE 
                {$where}
            GROUP BY
                suo.suoid,
                suo.unoid,
                suo.unocod,
                suo.unosigla,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                suo.suosigla,
                ptr.ptrano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.irpcod,
                ptr.plodsc,
                ptr.ptrid,
                ptr.ptres,
                lmu.lmuvlr,
                lmu.lmuid,
                psu.psuid,
                ptrdotacao_capital,
                ptrdotacao_custeio,
		sec.provisionado,
		sec.empenhado,
		sec.liquidado,
		sec.pago
            ORDER BY
                suo.unosigla,
                suo.unocod,
                suo.suonome,
                suo.suocod,
                ptr.funcional
        ";

        return $sql;
    }

    /**
     * Busca lista de PI.
     * @example array( 'suo.unocod' => UNIDADEORCAMENTARIA_SUOCOD_FNC, 'suo.unocod != '. UNIDADEORCAMENTARIA_SUOCOD_FNC, 'suo.unocod' => array(UNIDADEORCAMENTARIA_SUOCOD_FNC, 42101)
     *
     * @param integer $exercicio
     * @param array $aFiltro
     * @return array
     */
    public function recuperarExecucaoOrcamentaria($aFiltro = [])
    {
        $sql = $this->montarSqlExecucaoOrcamentaria($aFiltro);
//ver($sql, d);
        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['suocod']]['detalhe'][] = $dado;
            $dadosAgrupados[$dado['suocod']]['dotacaoCapital'] += $dado['ptrdotacao_capital'];
            $dadosAgrupados[$dado['suocod']]['dotacaoCusteio'] += $dado['ptrdotacao_custeio'];
            $dadosAgrupados[$dado['suocod']]['planejadoCapital'] += $dado['picvalorcapital'];
            $dadosAgrupados[$dado['suocod']]['planejadoCusteio'] += $dado['picvalorcusteio'];
            $dadosAgrupados[$dado['suocod']]['totalPlanejado'] = ($dadosAgrupados[$dado['suocod']]['planejadoCapital'] + $dadosAgrupados[$dado['suocod']]['planejadoCusteio']);
        }

        return $dadosAgrupados;
    }

    public function recuperarDadosFncPorAcao($exercicio)
    {

        $sql = "SELECT  distinct 'CUSTEIO' tipo, suo.unoid, suo.unocod, suo.unosigla, suo.unonome, lmu.lmuvlr, lmu.lmuid, psu.ptrid, vptr.funcional, vptr.acatitulo, vptr.irpcod, vptr.plodsc,
                        ptr.ptrdotacaocusteio dotacao
                from public.vw_subunidadeorcamentaria suo
                    inner join spo.ptressubunidade psu on psu.suoid = suo.suoid
                    
                    -- ESSE JOIN DE PTRES COM A VW_PTRES É NECESSÁRIO PARA RECUPERAR O VALOR DE DOTAÇÃO E CUSTEIO CORRETO EM CASO DO PNC
                    inner join monitora.vw_ptres vptr on vptr.ptrid = psu.ptrid AND vptr.ptrano = suo.prsano
                    inner join monitora.ptres ptr on ptr.ptrid = vptr.ptrid
                    
                    left join planacomorc.unidadegestora_limite lmu on lmu.unoid = suo.unoid AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
                where suo.suostatus = 'A'
                and suo.unofundo = 't' 
                and suo.suocod not in ('203003', '420017')
                and suo.prsano = '$exercicio'                
                union 
                SELECT  distinct 'CAPITAL' tipo, suo.unoid, suo.unocod, suo.unosigla, suo.unonome, lmu.lmuvlr, lmu.lmuid, psu.ptrid, vptr.funcional, vptr.acatitulo, vptr.irpcod, vptr.plodsc,
                        ptr.ptrdotacaocapital dotacao              
                from public.vw_subunidadeorcamentaria suo
                    inner join spo.ptressubunidade psu on psu.suoid = suo.suoid
                    
                    -- ESSE JOIN DE PTRES COM A VW_PTRES É NECESSÁRIO PARA RECUPERAR O VALOR DE DOTAÇÃO E CUSTEIO CORRETO EM CASO DO PNC
                    inner join monitora.vw_ptres vptr on vptr.ptrid = psu.ptrid AND vptr.ptrano = suo.prsano
                    inner join monitora.ptres ptr on ptr.ptrid = vptr.ptrid
                    
                    left join planacomorc.unidadegestora_limite lmu on lmu.unoid = suo.unoid AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
                where suo.suostatus = 'A'
                and suo.unofundo = 't' 
                and suo.suocod not in ('203003', '420017')
                and suo.prsano = '$exercicio'                
                ORDER BY funcional, tipo desc";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarValoresPorEstado($exercicio)
    {

        $sql = "select pli.pliid, pip.ptrid, pic.picvalorcapital, pic.picvalorcusteio, doc.esdid
                from monitora.pi_planointerno pli
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo on suo.suocod = pli.ungcod and suo.unocod = pli.unicod and suo.prsano = pli.pliano
                        inner join workflow.documento doc on doc.docid = pli.docid
                        inner join monitora.pi_planointernoptres pip on pip.pliid = pli.pliid
                where
                    pli.plistatus = 'A'        
                    and pli.pliano = '$exercicio'
                    and suo.suocod not in ('203003', '420017')
                    and suo.unofundo = 't'
                ";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $aValores = [];
        foreach($dados as $dado){
            $aValores[$dado['ptrid']][$dado['esdid']]['CAPITAL'] += $dado['picvalorcapital'];
            $aValores[$dado['ptrid']][$dado['esdid']]['CUSTEIO'] += $dado['picvalorcusteio'];
        }
        return $aValores;
    }

    public function recuperarValoresPorExecucao($exercicio)
    {

        $sql = "select  ptr.ptrid, 
                        case 
                                when substring(sec.natcod, 1, 1) = '3' then 'CUSTEIO'  
                                when substring(sec.natcod, 1, 1) = '4' then 'CAPITAL'  
                                else substring(sec.natcod, 1, 1)
                        end nat, 
                        sum(COALESCE (sec.vlrempenhado, 0))::DECIMAL AS vlrempenhado,
                        sum(COALESCE (sec.vlrpago, 0))::DECIMAL AS vlrpago,
                        sum(COALESCE (sec.vlrliquidado, 0))::DECIMAL AS vlrliquidado
                from spo.siopexecucao sec
                        inner join monitora.ptres ptr on sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano
                        inner join monitora.pi_planointernoptres ppt ON ptr.ptrid = ppt.ptrid
                        inner JOIN monitora.pi_planointerno pli ON ppt.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo ON pli.ungcod = suo.suocod AND pli.pliano = suo.prsano
                where
                    suo.prsano = '$exercicio'
                    AND suo.suocod not in ('203003', '420017')
                    AND suo.unofundo = 't'
                    AND pli.plistatus = 'A'
                group by
                    ptr.ptrid,
                    nat
                ";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $aValores = [];
        foreach($dados as $dado){
            $aValores[$dado['ptrid']][$dado['nat']]['vlrempenhado'] += $dado['vlrempenhado'];
            $aValores[$dado['ptrid']][$dado['nat']]['vlrpago'] += $dado['vlrpago'];
            $aValores[$dado['ptrid']][$dado['nat']]['vlrliquidado'] += $dado['vlrliquidado'];
        }

        return $aValores;
    }

    public function recuperarDadosFncPorUnidade($exercicio)
    {

        $sql = "SELECT  DISTINCT suo.unoid, suo.unocod, suo.suocod, suo.unosigla, suo.unonome, suo.suoid, suo.suonome, suo.suosigla,
                    case when suo.suocod in ('344042', '344001', '344041', '403201', '423001', '403101') then 1 else 2 end ordem
                from public.vw_subunidadeorcamentaria suo
                    inner join spo.ptressubunidade psu on psu.suoid = suo.suoid
                where suo.suostatus = 'A'
                and suo.unofundo = 't' 
                and suo.prsano = '$exercicio'
                and suo.suocod not in ('203003', '420017')
                order by ordem, suo.suonome";

        $dadosUnidade = $this->carregar($sql);
        $dados['unidade'] = $dadosUnidade ? $dadosUnidade : [];
        $dados['estado'] = $this->recuperarValoresPorUnidadeEstado($exercicio);
        $dados['execucao'] = $this->recuperarValoresPorUnidadeExecucao($exercicio);

        return $dados;
    }

    public function recuperarValoresPorUnidadeEstado($exercicio)
    {
        $sql = "select pli.pliid, suo.suoid, (coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0)) valor, doc.esdid
                from monitora.pi_planointerno pli
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo on suo.suocod = pli.ungcod and suo.unocod = pli.unicod and suo.prsano = pli.pliano
                        inner join workflow.documento doc on doc.docid = pli.docid
                        inner join monitora.pi_planointernoptres pip on pip.pliid = pli.pliid
                where
                    pli.plistatus = 'A'
                    and pli.pliano = '$exercicio'
                    and suo.suocod not in ('203003', '420017')
                    and suo.unofundo = 't'
                ";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $aValores = [];

        foreach($dados as $dado){
            $aValores[$dado['suoid']][$dado['esdid']] += $dado['valor'];
        }
        return $aValores;
    }

    public function recuperarValoresPorUnidadeExecucao($exercicio)
    {

        $sql = "select  suo.suoid, 
                        sum(COALESCE (sec.vlrempenhado, 0))::DECIMAL AS vlrempenhado,
                        sum(COALESCE (sec.vlrpago, 0))::DECIMAL AS vlrpago,
                        sum(COALESCE (sec.vlrliquidado, 0))::DECIMAL AS vlrliquidado
                from spo.siopexecucao sec
                        inner join monitora.ptres ptr on sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano
                        inner join monitora.pi_planointernoptres ppt ON ptr.ptrid = ppt.ptrid
                        inner JOIN monitora.pi_planointerno pli ON ppt.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo ON pli.ungcod = suo.suocod AND pli.pliano = suo.prsano
                where
                    suo.prsano = '$exercicio'
                    AND suo.suocod not in ('203003', '420017')
                    AND suo.unofundo = 't'
                    AND pli.plistatus = 'A'
                group by suo.suoid
                ";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $aValores = [];
        foreach($dados as $dado){
            $aValores[$dado['suoid']]['vlrempenhado'] += $dado['vlrempenhado'];
            $aValores[$dado['suoid']]['vlrpago'] += $dado['vlrpago'];
            $aValores[$dado['suoid']]['vlrliquidado'] += $dado['vlrliquidado'];
        }

        return $aValores;
    }

    public function recuperarExecucaoOrcamentariaFnc()
    {
        $sql = "SELECT  distinct suo.unoid, suo.unocod, suo.unosigla, suo.unonome, lmu.lmuvlr, lmu.lmuid, 
                        ptr.ptrdotacaocapital ptrdotacao_capital, ptr.ptrdotacaocusteio ptrdotacao_custeio, 
                        psu.ptrid, vptr.funcional, vptr.acatitulo, vptr.irpcod, vptr.plodsc                
                from public.vw_subunidadeorcamentaria suo
                    inner join spo.ptressubunidade psu on psu.suoid = suo.suoid
                    
                    -- ESSE JOIN DE PTRES COM A VW_PTRES É NECESSÁRIO PARA RECUPERAR O VALOR DE DOTAÇÃO E CUSTEIO CORRETO EM CASO DO PNC
                    inner join monitora.vw_ptres vptr on vptr.ptrid = psu.ptrid AND vptr.ptrano = suo.prsano
                    inner join monitora.ptres ptr on ptr.ptrid = vptr.ptrid
                    
                    left join planacomorc.unidadegestora_limite lmu on lmu.unoid = suo.unoid AND lmu.lmustatus = 'A' AND lmu.prsano = suo.prsano
                where suo.suostatus = 'A'
                and suo.unofundo = 't' 
                and suo.suocod not in ('203003', '420017')
                and suo.prsano = '{$_SESSION['exercicio']}'                
                ORDER BY suo.unonome";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['unocod']]['detalhe'][] = $dado;
            $dadosAgrupados[$dado['unocod']]['dotacaoCapital'] += $dado['ptrdotacao_capital'];
            $dadosAgrupados[$dado['unocod']]['dotacaoCusteio'] += $dado['ptrdotacao_custeio'];
        }

        return $dadosAgrupados;
    }

    /**
     * Busca Unidades com estatistica de orçamento e valores de pagamento.
     *
     * @param stdClass $filtro
     * @return array
     */
    public function recuperarEstatisticaPagamento(stdClass $filtro)
    {
        $sql = "
            SELECT DISTINCT
                1 AS ordem,
                'Dotação' AS descricao,
                suo.unosigla AS categoria,
                (sum(COALESCE (psu.ptrdotacaocusteio, 0)::DECIMAL) + sum(COALESCE (psu.ptrdotacaocapital, 0)::DECIMAL)) AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                LEFT JOIN monitora.vw_ptres ptr ON ptr.ptrid = psu.ptrid
                AND ptr.ptrano = suo.prsano
            WHERE
                suo.prsano = '{$filtro->exercicio}'
                AND suo.unofundo = {$filtro->unofundo}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                2 AS ordem,
                'Limite' AS descricao,
                suo.unosigla AS categoria,
                sum(COALESCE (lmu.lmuvlr, 0)::DECIMAL) AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN planacomorc.unidadegestora_limite lmu ON suo.suocod = lmu.ungcod
                AND suo.prsano = lmu.prsano
            WHERE
                suo.prsano = '{$filtro->exercicio}'
                AND suo.unofundo = {$filtro->unofundo}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3 AS ordem,
                'Planejado' AS descricao,
                suo.unosigla AS categoria,
                sum(COALESCE (pic.picvalorcapital, 0) + COALESCE (pic.picvalorcusteio, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.unicod = suo.unocod AND pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
            WHERE
                suo.prsano = '{$filtro->exercicio}'
                AND suo.unofundo = {$filtro->unofundo}
                AND suo.suostatus = 'A'
                AND pli.plistatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3.5 AS ordem,
                'Provisionado' AS descricao,
                suo.unosigla AS categoria,
                sum(COALESCE (sec.vlrautorizado, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.unicod = suo.unocod AND pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '{$filtro->exercicio}'
                AND suo.unofundo = {$filtro->unofundo}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                4 AS ordem,
                'Empenhado' AS descricao,
                suo.unosigla AS categoria,
                sum(COALESCE (sec.vlrempenhado, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.unicod = suo.unocod AND pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '{$filtro->exercicio}'
                AND suo.unofundo = {$filtro->unofundo}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                6 AS ordem,
                'Pago' AS descricao,
                suo.unosigla AS categoria,
                sum(COALESCE (sec.vlrpago, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.unicod = suo.unocod AND pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '{$filtro->exercicio}'
                AND suo.unofundo = {$filtro->unofundo}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                5 AS ordem,
                'Liquidado' AS descricao,
                suo.unosigla AS categoria,
                sum(COALESCE (sec.vlrliquidado, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.unicod = suo.unocod AND pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '{$filtro->exercicio}'
                AND suo.unofundo = {$filtro->unofundo}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            ORDER BY
                ordem,
                categoria
        ";
//ver($sql, d);
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    /**
     * Busca Subunidades por UO com estatistica de orçamento e valores de pagamento.
     *
     * @param stdClass $filtro
     * @return array
     */
    public function recuperarEstatisticaPagamentoDetalhe(stdClass $filtro)
    {
        $filtroUO = $filtro->unosigla? "AND suo.unosigla = '{$filtro->unosigla}'": NULL;

        $sql = "
            SELECT DISTINCT
                1 AS ordem,
                'Dotação' AS descricao,
                suo.suosigla AS categoria,
                (sum(COALESCE (psu.ptrdotacaocusteio, 0)::DECIMAL) + sum(COALESCE (psu.ptrdotacaocapital, 0)::DECIMAL)) AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                LEFT JOIN monitora.vw_ptres ptr ON ptr.ptrid = psu.ptrid
                AND ptr.ptrano = suo.prsano
            WHERE
                suo.prsano = '$filtro->exercicio' {$filtroUO}
                AND suo.unofundo = $filtro->unofundo
                and {$filtro->unidades}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                2 AS ordem,
                'Limite' AS descricao,
                suo.suosigla AS categoria,
                sum(COALESCE (lmu.lmuvlr, 0)::DECIMAL) AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN planacomorc.unidadegestora_limite lmu ON suo.suocod = lmu.ungcod
                AND suo.prsano = lmu.prsano
            WHERE
                suo.prsano = '$filtro->exercicio' {$filtroUO}
                AND suo.unofundo = $filtro->unofundo
                and {$filtro->unidades}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3 AS ordem,
                'Planejado' AS descricao,
                suo.suosigla AS categoria,
                sum(COALESCE (pic.picvalorcapital, 0) + COALESCE (pic.picvalorcusteio, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
            WHERE
                suo.prsano = '$filtro->exercicio' {$filtroUO}
                AND suo.unofundo = $filtro->unofundo
                AND {$filtro->unidades}
                AND suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3.5 AS ordem,
                'Provisionado' AS descricao,
                suo.suosigla AS categoria,
                sum(COALESCE (sec.vlrautorizado, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '$filtro->exercicio' {$filtroUO}
                AND suo.unofundo = $filtro->unofundo
                and {$filtro->unidades}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                4 AS ordem,
                'Empenhado' AS descricao,
                suo.suosigla AS categoria,
                sum(COALESCE (sec.vlrempenhado, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '$filtro->exercicio' {$filtroUO}
                AND suo.unofundo = $filtro->unofundo
                and {$filtro->unidades}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                6 AS ordem,
                'Pago' AS descricao,
                suo.suosigla AS categoria,
                sum(COALESCE (sec.vlrpago, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '$filtro->exercicio' {$filtroUO}
                AND suo.unofundo = $filtro->unofundo
                and {$filtro->unidades}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                5 AS ordem,
                'Liquidado' AS descricao,
                suo.suosigla AS categoria,
                sum(COALESCE (sec.vlrliquidado, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN monitora.pi_planointerno pli ON(pli.ungcod = suo.suocod AND pli.pliano = suo.prsano AND pli.plistatus = 'A')
                LEFT JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                LEFT JOIN monitora.ptres ptr ON ptr.ptrid = ppt.ptrid
                LEFT JOIN spo.siopexecucao sec ON sec.ptres = ptr.ptres AND sec.exercicio = ptr.ptrano and sec.plicod = pli.plicod
                AND sec.exercicio = ptr.ptrano
            WHERE
                suo.prsano = '$filtro->exercicio' {$filtroUO}
                AND suo.unofundo = $filtro->unofundo
                and {$filtro->unidades}
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            ORDER BY
                ordem,
                categoria
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarEstatisticaUnidade($exercicio, $aFiltro = [])
    {
        $sql = "select uno.unosigla descricao, sum(e.autorizado::decimal) valor
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria AND uno.prsano = '$exercicio'
                where e.planointerno is not null
                AND e.anoexercicio = '$exercicio'
                AND e.anoreferencia = '$exercicio'
                group by uno.unosigla
                order by descricao";

        // Carregando dados da base de dados do SIMINC antigo
        $dados = adapterConnection::oldSiminc()->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarEstatisticaPiUnidade($exercicio, $aFiltro = [])
    {
        $sql = "select uno.unosigla descricao, count(distinct e.planointerno) valor
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria AND uno.prsano = '$exercicio'
                where e.planointerno is not null
                AND e.anoexercicio = '$exercicio'
                AND e.anoreferencia = '$exercicio'
                group by uno.unosigla
                order by descricao;";

        // Carregando dados da base de dados do SIMINC antigo
        $dados = adapterConnection::oldSiminc()->carregar($sql);
        return $dados ? $dados : [];
    }

    public function alterarCodigoPi($plicod)
    {
        $this->plicod = $plicod;
        $this->salvar();
        $this->commit();

        echo $this->plicod;
    }

    public function verificarPermissaoEditar($estadoAtual, $perfis)
    {
        switch ($estadoAtual['esdid']) {
            case ESD_PI_APROVADO:
            case ESD_PI_CANCELADO:
            case ESD_PI_AGUARDANDO_APROVACAO:
                return (bool)count(array_intersect($perfis, [PFL_ADMINISTRADOR, PFL_SUPERUSUARIO, PFL_PLANEJAMENTO]));
        }

        return true;
    }

    /**
     * Casol não exista Janela cadastrada para o periodo vigente o sistema não permite as unidades fazerem inserção ou alteração no PI.
     *
     * @param array $perfis
     * @param array $listaJanelasDeAlteracao
     * @return boolean
     */
    public function verificarPermissaoEditarUnidade($perfis, $listaJanelasDeAlteracao){
        if(in_array(PFL_SUBUNIDADE, $perfis) && !$listaJanelasDeAlteracao){
            return false;
        }

        return true;
    }

    public function verificarPermissaoEditarFnc($estadoAtual, $perfis, $listaJanelasDeAlteracao)
    {
        # Casol não exista Janela cadastrada para o periodo vigente o sistema não permite as unidades fazerem inserção ou alteração no PI.
        if(!self::verificarPermissaoEditarUnidade($perfis, $listaJanelasDeAlteracao)){
            return false;
        }

        switch ($estadoAtual['esdid']) {
            case ESD_FNC_PI_APROVADO:
            case ESD_FNC_PI_BANCO_PROJETOS:
            case ESD_FNC_PI_DELIBERACAO_CFNC:
            case ESD_FNC_PI_EM_ANALISE:
            case ESD_FNC_PI_SELECIONADO_CFNC:
                return (bool)count(array_intersect($perfis, [PFL_ADMINISTRADOR, PFL_SUPERUSUARIO, PFL_PLANEJAMENTO]));
        }

        return true;
    }

    public function getDadosGerais($exercicio, $fundo = null)
    {

        $where = " pli.pliano = '$exercicio' AND pli.plistatus = 'A' ";

        if($fundo){
            $where .= " AND suo.unofundo = '{$fundo}' ";
        }

        # Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
        $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object)array('usucpf' => $_SESSION['usucpf']));
        if (count($listaSubUnidadeUsuario)) {
            $where .= " AND suo.suocod IN('" . join("','", $listaSubUnidadeUsuario) . "') ";
        }

        $sql = "
            select
                pli.pliid,
                esd.esddsc,
                pli.plicod,
                suo.unocod,
                suo.unonome,
                suo.suocod,
                suo.suonome,
                (select array_to_string(array(
                        select distinct s.unonome 
                        from planacomorc.pi_delegacao d 
                                inner join public.vw_subunidadeorcamentaria s on s.suoid = d.suoid
                        where d.pliid = pli.pliid
                ), '; ')) unodelegada,

                (select array_to_string(array(
                        select distinct s.suonome 
                        from planacomorc.pi_delegacao d 
                                inner join public.vw_subunidadeorcamentaria s on s.suoid = d.suoid
                        where d.pliid = pli.pliid
                ), '; ')) suodelegada,

                eqd.eqddsc, rp.rpcod || ' - ' || rp.redsc as resultadoprimario, mai.mainome, mas.masnome, ptr.prgcod, ptr.prgdsc, opp.oppcod, opp.oppnome, ipp.ippcod, ipp.ippnome, 
                ptr.acacod, ptr.acatitulo, ptr.loccod, ptr.locdsc, ptr.plocod, plodsc po, ptr.ptres,
                mde.mdedsc, mpn.mpncod || ' - ' || mpn.mpnnome mpnnome, mpp.mppcod || ' - ' || mpp.mppnome mppnome,
                pli.plititulo, pli.plidsc, nee.needsc, cap.capdsc, 
                case when pli.pliemenda = true then 'Sim' else 'Não' end as pliemenda,
                case when pic.picedital = true then 'Sim' else 'Não' end as picedital, esf.esfdsc,

                case 
                        when pic.esfid in (1, 2, 3) then 'Brasil'
                        else pic.esfid::char
                end as pais,                                       

                case 
                        when pic.esfid = 2 then 
                                (select array_to_string(array(
                                        select distinct e.estdescricao
                                        from planacomorc.pi_localizacao l 
                                                inner join territorios.estado e on e.estuf = l.estuf
                                        where l.pliid = pli.pliid                                
                                ), '; '))      
                        when pic.esfid = 3 then 
                                (select array_to_string(array(
                                        select distinct e.estdescricao
                                        from planacomorc.pi_localizacao l 
                                                inner join territorios.municipio m on m.muncod = l.muncod
                                                inner join territorios.estado e on e.estuf = m.estuf
                                        where l.pliid = pli.pliid                               
                                ), '; '))                                              
                        else ''
                end estado,              

                (select array_to_string(array(
                        select distinct m.mundescricao
                        from planacomorc.pi_localizacao l 
                                inner join territorios.municipio m on m.muncod = l.muncod
                                inner join territorios.estado e on e.estuf = m.estuf
                        where l.pliid = pli.pliid                           
                ), '; ')) municipio,

                (select array_to_string(array(
                        select distinct usu.usunome || ' - ' || replace(to_char(usu.usucpf::numeric, '000:000:000-00'), ':', '.') || ' (' || usu.usufoneddd || ' ' || usu.usufonenum || ' - ' || usu.usuemail || ')'
                        from planacomorc.pi_responsavel pir                
                                inner join seguranca.usuario usu on usu.usucpf = pir.usucpf
                        where pir.pliid = pli.pliid                           
                ), '; ')) usuario,                        

                case when pic.picted then 'Sim' else 'Não' end ted,
                case when pic.picedital then 'Sim' else 'Não' end edital,                        
                mes.mesdsc,

                (select array_to_string(array(
                       select distinct pco.pcoconvenio
                        from planacomorc.pi_convenio pco
                        where pco.pliid = pli.pliid                           
                ), '; ')) convenio,                          

                (select array_to_string(array(
                       select distinct pis.pissniic
                        from planacomorc.pi_sniic pis
                        where pis.pliid = pli.pliid                           
                ), '; ')) sniic,                      

                (select array_to_string(array(
                       select distinct pse.psesei
                        from planacomorc.pi_sei pse
                        where pse.pliid = pli.pliid                           
                ), '; ')) sei,   


                (select array_to_string(array(
                       select distinct ppr.pprpronac
                        from planacomorc.pi_pronac ppr
                        where ppr.pliid = pli.pliid                           
                ), '; ')) pronac,                          

                ppr.pprnome, pum.pumnome, picquantidade,

                COALESCE(pic.picvalorcusteio, 0) + COALESCE(pic.picvalorcapital, 0) AS valortotal,
                COALESCE(pic.picvalorcusteio, 0) AS picvalorcusteio,
                COALESCE(pic.picvalorcapital, 0) AS picvalorcapital,

                -- Cronograma Físico
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '01' AND pcr.pliid = pli.pliid) fisico_1,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '02' AND pcr.pliid = pli.pliid) fisico_2,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '03' AND pcr.pliid = pli.pliid) fisico_3,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '04' AND pcr.pliid = pli.pliid) fisico_4,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '05' AND pcr.pliid = pli.pliid) fisico_5,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '06' AND pcr.pliid = pli.pliid) fisico_6,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '07' AND pcr.pliid = pli.pliid) fisico_7,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '08' AND pcr.pliid = pli.pliid) fisico_8,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '09' AND pcr.pliid = pli.pliid) fisico_9,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '10' AND pcr.pliid = pli.pliid) fisico_10,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '11' AND pcr.pliid = pli.pliid) fisico_11,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 AND mescod = '12' AND pcr.pliid = pli.pliid) fisico_12,

                -- Cronograma Financeiro Capital
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '01' AND pcr.pliid = pli.pliid) fin_capital_1,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '02' AND pcr.pliid = pli.pliid) fin_capital_2,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '03' AND pcr.pliid = pli.pliid) fin_capital_3,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '04' AND pcr.pliid = pli.pliid) fin_capital_4,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '05' AND pcr.pliid = pli.pliid) fin_capital_5,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '06' AND pcr.pliid = pli.pliid) fin_capital_6,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '07' AND pcr.pliid = pli.pliid) fin_capital_7,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '08' AND pcr.pliid = pli.pliid) fin_capital_8,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '09' AND pcr.pliid = pli.pliid) fin_capital_9,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '10' AND pcr.pliid = pli.pliid) fin_capital_10,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '11' AND pcr.pliid = pli.pliid) fin_capital_11,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 AND mescod = '12' AND pcr.pliid = pli.pliid) fin_capital_12,

                -- Cronograma Financeiro Custeio
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '01' AND pcr.pliid = pli.pliid) fin_custeio_1,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '02' AND pcr.pliid = pli.pliid) fin_custeio_2,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '03' AND pcr.pliid = pli.pliid) fin_custeio_3,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '04' AND pcr.pliid = pli.pliid) fin_custeio_4,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '05' AND pcr.pliid = pli.pliid) fin_custeio_5,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '06' AND pcr.pliid = pli.pliid) fin_custeio_6,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '07' AND pcr.pliid = pli.pliid) fin_custeio_7,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '08' AND pcr.pliid = pli.pliid) fin_custeio_8,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '09' AND pcr.pliid = pli.pliid) fin_custeio_9,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '10' AND pcr.pliid = pli.pliid) fin_custeio_10,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '11' AND pcr.pliid = pli.pliid) fin_custeio_11,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 AND mescod = '12' AND pcr.pliid = pli.pliid) fin_custeio_12,        


                -- Cronograma Orçamentário Capital
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '01' AND pcr.pliid = pli.pliid) orc_capital_1,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '02' AND pcr.pliid = pli.pliid) orc_capital_2,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '03' AND pcr.pliid = pli.pliid) orc_capital_3,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '04' AND pcr.pliid = pli.pliid) orc_capital_4,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '05' AND pcr.pliid = pli.pliid) orc_capital_5,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '06' AND pcr.pliid = pli.pliid) orc_capital_6,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '07' AND pcr.pliid = pli.pliid) orc_capital_7,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '08' AND pcr.pliid = pli.pliid) orc_capital_8,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '09' AND pcr.pliid = pli.pliid) orc_capital_9,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '10' AND pcr.pliid = pli.pliid) orc_capital_10,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '11' AND pcr.pliid = pli.pliid) orc_capital_11,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 AND mescod = '12' AND pcr.pliid = pli.pliid) orc_capital_12,

                -- Cronograma Orçamentário Custeio                                                                               
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '01' AND pcr.pliid = pli.pliid) orc_custeio_1,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '02' AND pcr.pliid = pli.pliid) orc_custeio_2,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '03' AND pcr.pliid = pli.pliid) orc_custeio_3,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '04' AND pcr.pliid = pli.pliid) orc_custeio_4,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '05' AND pcr.pliid = pli.pliid) orc_custeio_5,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '06' AND pcr.pliid = pli.pliid) orc_custeio_6,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '07' AND pcr.pliid = pli.pliid) orc_custeio_7,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '08' AND pcr.pliid = pli.pliid) orc_custeio_8,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '09' AND pcr.pliid = pli.pliid) orc_custeio_9,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '10' AND pcr.pliid = pli.pliid) orc_custeio_10,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '11' AND pcr.pliid = pli.pliid) orc_custeio_11,
                (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 AND mescod = '12' AND pcr.pliid = pli.pliid) orc_custeio_12,         

                vlrautorizado, vlrempenhado, vlrliquidado, vlrpago
                           
            from monitora.pi_planointerno pli
                inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                inner join public.vw_subunidadeorcamentaria suo on suo.suocod = pli.ungcod AND suo.unocod = pli.unicod AND suo.prsano = pli.pliano
                inner join monitora.pi_enquadramentodespesa eqd on eqd.eqdid = pli.eqdid
                left join planacomorc.resultadoprimario rp on rp.rpcod = eqd.eqdrp
                left join planacomorc.manutencaoitem mai on mai.maiid = pic.maiid
                left join planacomorc.manutencaosubitem mas on mas.masid = pic.masid
                left join monitora.pi_planointernoptres ppt on ppt.pliid = pli.pliid
                left join monitora.vw_ptres ptr on ptr.ptrid = ppt.ptrid AND ptr.ptrano = pli.pliano
                left join public.objetivoppa opp on opp.oppid = pic.oppid
                left join public.iniciativappa ipp on ipp.ippid = pic.ippid
                left join monitora.pi_modalidadeensino mde on mde.mdeid = pli.mdeid
                left join monitora.pi_niveletapaensino nee on nee.neeid = pli.neeid
                left join public.metappa mpp on mpp.mppid = pic.mppid
                left join public.metapnc mpn on mpn.mpnid = pic.mpnid                        
                left join monitora.pi_categoriaapropriacao cap on cap.capid = pli.capid
                left join monitora.pi_produto ppr on ppr.pprid = pic.pprid
                left join monitora.pi_unidade_medida pum on pum.pumid = pic.pumid        
                left join workflow.documento doc on doc.docid = pli.docid
                left join workflow.estadodocumento esd on esd.esdid = doc.esdid
                left join territorios.esfera esf on esf.esfid = pic.esfid
                left join public.meses mes on mes.mescod = pic.mescod
                left join (
                        select sex.exercicio, sex.plicod, sum(sex.vlrautorizado) vlrautorizado, sum(sex.vlrempenhado) vlrempenhado, sum(sex.vlrliquidado) vlrliquidado, sum(sex.vlrpago) vlrpago
                        from spo.siopexecucao sex
                        where coalesce(sex.plicod, '') != ''
                        group by sex.exercicio, sex.plicod                                
                ) sex on sex.plicod = pli.plicod AND sex.exercicio = pli.pliano
                
            where $where
        ";

        $dados = $this->carregar($sql);
        return $dados? $dados: [];
    }

    public function getCorPainelPorValorDisponivel($valorDisponivel)
    {
        if ($valorDisponivel > 0) return 'blue';

        if ($valorDisponivel < 0) return 'red';

        return 'green';
    }

    public function recuperarPiPorSubunidade($suoid)
    {
        $dadosPi = $this->recuperarDadosDetalhe((object) array('suoid' => $suoid, 'tipo' => self::K_TIPO_DETALHE_PI));
        $dadosDelegado = $this->recuperarDadosDetalhe((object) array('suoid' => $suoid, 'tipo' => self::K_TIPO_DETALHE_DELEGADO));

        include_once APPRAIZ . 'planacomorc/modulos/principal/unidade/detalhe-subunidade.inc';
    }

    public function recuperarPiPorFuncional(stdClass $parametros)
    {
        $dados = $this->recuperarDadosDetalhe((object) array('suoid' => $parametros->suoid, 'ptrid' => $parametros->ptrid, 'tipo' => self::K_TIPO_DETALHE_PTRES));
        include_once APPRAIZ . 'planacomorc/modulos/principal/unidade/detalhe-ptres.inc';
    }

    public function recuperarDetalhesValoresGrafico($graficoSubunidade = TRUE, $tipoValor, $sigla)
    {
        if($graficoSubunidade){
            $filtro = (object)array('suosigla' => $sigla);
        } else {
            $filtro = (object)array('unosigla' => $sigla);
        }

        switch($tipoValor){
            case 'Dot':
                $sql = self::montarSqlGraficoPizzaRp($filtro);
                break;
            case 'Pla':
                $sql = self::montarSqlGraficoPizzaEnquadramento($filtro);
                break;
        }

        $dados = $this->carregar($sql);
        if($dados){
            $grafico = new Grafico(Grafico::K_TIPO_PIZZA, false);
            $grafico->gerarGrafico($dados);
        } else {
            echo "<script>$('#detalhe-grafico').modal('hide'); swal({title: 'Atenção', text: 'Nenhum dado encontrado.', type: 'warning', html: true});</script>";
        }
    }

    public static function montarSqlGraficoPizzaRp(stdClass $filtro){
        $where = '';
        $where .= $filtro->suosigla? "\n AND suo.suosigla = '$filtro->suosigla'": NULL;
        $where .= $filtro->unosigla? "\n AND suo.unosigla = '$filtro->unosigla'": NULL;

        $sql = "
            SELECT
                'RP ' || ptr.irpcod descricao,
                SUM(COALESCE(ptr.ptrdotacaocapital, 0) + COALESCE(ptr.ptrdotacaocusteio, 0)) valor
            FROM monitora.vw_ptres ptr
                JOIN spo.ptressubunidade psu ON ptr.ptrid = psu.ptrid
                JOIN public.vw_subunidadeorcamentaria suo ON psu.suoid = suo.suoid AND ptr.ptrano = suo.prsano
            WHERE
                ptr.ptrano = '{$_SESSION['exercicio']}' $where
            GROUP BY
                ptr.irpcod
        ";
//ver($sql);
        return $sql;
    }

    public static function montarSqlGraficoPizzaEnquadramento(stdClass $filtro){
        $where = '';
        $where .= $filtro->suosigla? "\n AND suo.suosigla = '$filtro->suosigla'": NULL;
        $where .= $filtro->unosigla? "\n AND suo.unosigla = '$filtro->unosigla'": NULL;
        $sql = "
            SELECT
                eqd.eqddsc descricao,
                SUM(COALESCE(pic.picvalorcapital, 0) + COALESCE(pic.picvalorcusteio, 0)) valor
            FROM public.vw_subunidadeorcamentaria suo
                JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                JOIN monitora.vw_ptres ptr ON ptr.ptrid = psu.ptrid AND ptr.ptrano = suo.prsano
                JOIN monitora.pi_planointernoptres ppt ON ppt.ptrid = ptr.ptrid
                JOIN monitora.pi_planointerno pli ON(pli.pliid = ppt.pliid AND pli.ungcod = suo.suocod AND pli.unicod = suo.unocod AND pli.plistatus = 'A')
                JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid
            WHERE
                ptr.ptrano = '{$_SESSION['exercicio']}' $where
            GROUP BY
                eqd.eqddsc
        ";
//ver($sql);
        return $sql;
    }

    public function recuperarDadosDetalhe(stdClass $parametros)
    {
        $where = '';
        switch ($parametros->tipo) {
            case self::K_TIPO_DETALHE_PTRES:
                $where .= $parametros->ptrid? " AND ptr.ptrid = '$parametros->ptrid'": NULL;
                $where .= $parametros->suoid? " AND suo.suoid = '$parametros->suoid'": NULL;
                break;
            case self::K_TIPO_DETALHE_PI:
                $where .= " AND suo.suoid = '$parametros->suoid'";
                break;
            case self::K_TIPO_DETALHE_DELEGADO:
                $where .= " AND pde.suoid = '$parametros->suoid'";
                break;
        }

        $sql = "
            SELECT
                pli.pliid,
                pli.plititulo,
                pli.plicod,
                pli.pliemenda,
                pic.picted,
                pic.picedital,
                pic.picvalorcapital,
                pic.picvalorcusteio,
		SUM(COALESCE(ex.vlrautorizado,0)) AS autorizado,
		SUM(COALESCE(ex.vlrempenhado,0)) AS empenhado,
		SUM(COALESCE(ex.vlrliquidado,0)) AS liquidado,
		SUM(COALESCE(ex.vlrpago,0)) AS pago,
                suo.suocod,
                suo.suonome,
                suo.unocod,
                suo.unonome,
                suo.unosigla,
                ptr.funcional,
                ptr.acatitulo,
                ptr.ptrdotacao,
                eqd.eqddsc,
                eqd.eqdrp
            FROM
                monitora.pi_planointerno pli
                INNER JOIN planacomorc.pi_complemento pic ON pic.pliid = pli.pliid
                LEFT JOIN planacomorc.pi_delegacao pde ON pde.pliid = pli.pliid
                INNER JOIN public.vw_subunidadeorcamentaria suo ON suo.suocod = pli.ungcod
                    AND suo.unocod = pli.unicod
                    AND suo.prsano = pli.pliano
                INNER JOIN monitora.pi_planointernoptres ppt ON ppt.pliid = pli.pliid
                INNER JOIN monitora.vw_ptres ptr ON ptr.ptrid = ppt.ptrid
                    AND ptr.ptrano = pli.pliano
                LEFT JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid
                LEFT JOIN spo.siopexecucao ex ON ptr.ptres = ex.ptres AND ex.exercicio = ptr.ptrano and ex.plicod = pli.plicod
            WHERE
                pli.plistatus = 'A'
                $where
            GROUP BY
                pli.pliid,
                pli.plititulo,
                pli.plicod,
                pli.pliemenda,
                pic.picted,
                pic.picedital,
                pic.picvalorcapital,
                pic.picvalorcusteio,
                suo.suocod,
                suo.suonome,
                suo.unocod,
                suo.unonome,
                suo.unosigla,
                ptr.funcional,
                ptr.acatitulo,
                ptr.ptrdotacao,
                eqd.eqddsc,
                eqd.eqdrp
            ";
//ver($sql,d);
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarPis($filtro)
    {
        if(is_array($filtro['eqdid']) && count($filtro['eqdid'])){
            $where = ' and pli.eqdid in (' . implode(',', $filtro['eqdid']) . ')';
        }

        $sql = "select  pli.docid, doc.esdid, pli.pliid, pli.plicod, pli.plidsc, pli.plititulo, ptr.acatitulo, ptr.funcional,
                        doc.esdid, ptr.ptres, ptr.ptrid, ptr.prgcod, ptr.acacod, ptr.loccod, ptr.plocod, ptr.plodsc, ptr.irpcod,
                        suo.unocod, suo.unosigla, suo.unonome, suo.suocod, suo.suosigla, suo.suonome, suo.unofundo
                from monitora.pi_planointerno pli
                        inner join workflow.documento doc on doc.docid = pli.docid
                        inner join monitora.pi_planointernoptres pip on pip.pliid = pli.pliid
                        inner join monitora.vw_ptres ptr on ptr.ptrid = pip.ptrid
                        inner join public.vw_subunidadeorcamentaria suo on suo.suocod = pli.ungcod and suo.unocod = pli.unicod and suo.prsano = pli.pliano
                where pli.pliano = '{$filtro['exercicio']}'
                and pli.plistatus = 'A'
                and doc.esdid = '{$filtro['esdid']}'
                $where
                ";
        return $this->carregar($sql);
    }

    public function tramitarLote($aedid, $aDocid, $cmddsc = null)
    {
        $cmddsc = $cmddsc ? $cmddsc : 'Tramitado em lote';
        if(is_array($aDocid)){
            foreach($aDocid as $pliid => $docid){
                wf_alterarEstado($docid, $aedid, $cmddsc, array('pliid' => $pliid));
            }
        }

    }

    public function recuperarResumoPorMetaESubunidade($mppid, $suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "select  count(*) qtd, sum(coalesce(pic.picvalorcapital, 0)) picvalorcapital, sum(coalesce(pic.picvalorcusteio, 0)) picvalorcusteio, 
                        sum(coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0)) previsto,
                        sum(coalesce(sex.vlrautorizado,0)) as provisionado,
                        sum(coalesce(sex.vlrempenhado,0)) as empenhado,
                        sum(coalesce(sex.vlrliquidado,0)) as liquidado,
                        sum(coalesce(sex.vlrpago,0)) as pago
                from monitora.pi_planointerno pli
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo on suo.unocod = pli.unicod and suo.suocod = pli.ungcod and suo.prsano = pli.pliano
                        left join (
                                select sex.exercicio, sex.plicod, sum(sex.vlrautorizado) vlrautorizado, sum(sex.vlrempenhado) vlrempenhado, sum(sex.vlrliquidado) vlrliquidado, sum(sex.vlrpago) vlrpago
                                from spo.siopexecucao sex
                                where coalesce(sex.plicod, '') != ''
                                group by sex.exercicio, sex.plicod                                
                        ) sex on sex.plicod = pli.plicod AND sex.exercicio = pli.pliano        
                where pli.plistatus = 'A'
                and pli.pliano = '$exercicio'
                and mppid = " . (int) $mppid . "
                and suo.suoid = " . (int) $suoid . "
                ;";

        return $this->pegaLinha($sql);
    }

    public function recuperarPiPorMetaESubunidade($mppid, $suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "select  pli.pliid, pli.plicod, pli.plititulo, 
                        coalesce(pic.picvalorcapital, 0) picvalorcapital, coalesce(pic.picvalorcusteio, 0) picvalorcusteio, 
                        coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0) previsto,
                        coalesce(sex.vlrautorizado,0) as autorizado,
                        coalesce(sex.vlrempenhado,0) as empenhado,
                        coalesce(sex.vlrliquidado,0) as liquidado,
                        coalesce(sex.vlrpago,0) as pago
                from monitora.pi_planointerno pli
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo on suo.unocod = pli.unicod and suo.suocod = pli.ungcod and suo.prsano = pli.pliano
                        left join (
                                select sex.exercicio, sex.plicod, sum(sex.vlrautorizado) vlrautorizado, sum(sex.vlrempenhado) vlrempenhado, sum(sex.vlrliquidado) vlrliquidado, sum(sex.vlrpago) vlrpago
                                from spo.siopexecucao sex
                                where coalesce(sex.plicod, '') != ''
                                group by sex.exercicio, sex.plicod                                
                        ) sex on sex.plicod = pli.plicod AND sex.exercicio = pli.pliano        
                where pli.plistatus = 'A'
                and pli.pliano = '$exercicio'
                and mppid = " . (int) $mppid . "
                and suo.suoid = " . (int) $suoid . "
                ;";

        return $this->carregar($sql);
    }

    public function recuperarPiPorIndicadorESubunidade($ipnid, $suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "select  pli.pliid, pli.plicod, pli.plititulo, 
                        coalesce(pic.picvalorcapital, 0) picvalorcapital, coalesce(pic.picvalorcusteio, 0) picvalorcusteio, 
                        coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0) previsto,
                        coalesce(sex.vlrautorizado,0) as autorizado,
                        coalesce(sex.vlrempenhado,0) as empenhado,
                        coalesce(sex.vlrliquidado,0) as liquidado,
                        coalesce(sex.vlrpago,0) as pago
                from monitora.pi_planointerno pli
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo on suo.unocod = pli.unicod and suo.suocod = pli.ungcod and suo.prsano = pli.pliano
                        left join (
                                select sex.exercicio, sex.plicod, sum(sex.vlrautorizado) vlrautorizado, sum(sex.vlrempenhado) vlrempenhado, sum(sex.vlrliquidado) vlrliquidado, sum(sex.vlrpago) vlrpago
                                from spo.siopexecucao sex
                                where coalesce(sex.plicod, '') != ''
                                group by sex.exercicio, sex.plicod                                
                        ) sex on sex.plicod = pli.plicod AND sex.exercicio = pli.pliano        
                where pli.plistatus = 'A'
                and pli.pliano = '$exercicio'
                and pic.ipnid = " . (int) $ipnid . "
                and suo.suoid = " . (int) $suoid . "
                ";

        return $this->carregar($sql);
    }

    public function recuperarPorUnidade($suoid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];

        $sql = "select pli.pliid, pli.plititulo, pli.plicod, coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0) valor, pic.picquantidade, aco.acoid, suo.unofundo,
                       pic.pprid, pic.pumid, ppr.pprnome, pum.pumnome, ptr.funcional, ptr.plocod, ptr.plodsc, ptr.acatitulo, opp.oppid, opp.oppcod, opp.oppdsc, opp.oppnome
                from monitora.pi_planointerno pli
                        inner join public.vw_subunidadeorcamentaria suo on suo.unocod = pli.unicod and suo.suocod = pli.ungcod and suo.prsano = pli.pliano
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        left  join monitora.pi_planointernoptres ppt on ppt.pliid = pli.pliid
                        inner join monitora.vw_ptres ptr on ptr.ptrid = ppt.ptrid AND ptr.ptrano = pli.pliano
                        left join monitora.pi_produto ppr on ppr.pprid = pic.pprid
                        left join monitora.pi_unidade_medida pum on pum.pumid = pic.pumid
                        left join public.objetivoppa opp on opp.oppid = pic.oppid
                        left join acompanhamento.acompanhamento aco on aco.pliid = pli.pliid
                where pli.plistatus = 'A'        
                and pli.pliano = '$exercicio'
                and coalesce(pli.plicod, '') != ''
                and pli.ungcod = (select suocod from public.vw_subunidadeorcamentaria where suoid = " . (int)$suoid . ")
                order by suo.unofundo, pum.pumnome, pli.plititulo;
                ";

        return $this->carregar($sql);
    }

    public function recuperarQuantidadePrevista($mes = null)
    {
        $mes = $mes ? $mes : date('m');
        $sql = "select sum(pcrvalor)
                from planacomorc.pi_cronograma pcr 
                where crvid = 1 
                AND mescod::int <= $mes
                AND pcr.pliid = " . (int)$this->pliid;

        return $this->pegaUm($sql);
    }

    public function recuperarUltimoHistoricoAcompanhamento($mes = null)
    {
        $sql = "select his.*, usu.usunome, to_char(hisdata, 'DD/MM/YYYY HH24:MI:SS') hisdata
                from acompanhamento.historico his
                        inner join acompanhamento.acompanhamento aco on aco.acoid = his.acoid
                        inner join seguranca.usuario usu on usu.usucpf = his.usucpf
                where aco.pliid = " . (int)$this->pliid . "
                order by his.hisdata desc";

        return $this->pegaLinha($sql);
    }

    public function recuperarDados(array $filtro = [], $order = null)
    {
        $filtro['pliano'] = $filtro['exercicio'] ? $filtro['exercicio'] : $_SESSION['exercicio'];
        $order = $order ? $order : ['pli.suonome', 'pli.suonome', 'pli.funcional'];

        if($this->pliid){
            $where[] = ' pli.pliid = ' . (int) $this->pliid;
        }

        foreach($filtro as $campo => $valor){

            if(is_array($valor) && count($valor)){
                $where[] = " pli.{$campo} in '" . implode("', '", $valor) . "' ";
            } elseif(!empty($valor)) {
                $where[] = " pli.{$campo} = '$valor' ";
            }
        }

        $where = ' where ' . implode(' and ', $where);
        $order = ' order by ' . implode(', ', $order);

        $sql = "select * 
                from monitora.vw_planointerno pli
                $where
                $order
                "
        ;

        return $this->carregar($sql);
    }

}
