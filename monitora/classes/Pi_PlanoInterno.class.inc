<?php
	
class Pi_PlanoInterno extends Modelo{

    const K_TIPO_DETALHE_PTRES      = 'PT';
    const K_TIPO_DETALHE_PI         = 'PI';
    const K_TIPO_DETALHE_DELEGADO   = 'DL';

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
    protected $arChavePrimaria = array( "pliid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
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
        if(!empty($aFiltro['uno'])){
            $where[] = "uno.unoid = {$aFiltro['uno']}";
        }

        $sql = "SELECT  ppi.ppiid, uno.unonome, suo.suonome, mom.momnome, fun.funcionalcampobusca, 
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
                WHERE mom.momid = (select momid from planointerno.momento where prsano = '$exercicio' and momstatus = 'A' order by momid limit 1)
                AND  ppi.ppistatus = 'A'"
                . implode(' and ', $where) .
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
                        INNER JOIN planointerno.ppaacao aca on aca.acaid = pmu.acaid and aca.acastatus = 'A'
                        INNER JOIN planointerno.ppaprograma prg on prg.prgid = pmu.prgid and prg.prgstatus = 'A'        
                        INNER JOIN planointerno.ppalocalizador loc on loc.locid = pmu.locid and loc.locstatus = 'A'
                        INNER JOIN planointerno.ppaplanoorcamentario plo on plo.ploid = pmu.ploid and plo.plostatus = 'A'
                        INNER JOIN planointerno.subunidadeorcamentaria suo ON suo.suoid = pmu.suoid AND suo.suostatus = 'A'
                        INNER JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid AND uno.unostatus = 'A'
                        INNER JOIN planointerno.enquadramentodespesa eqd ON eqd.eqdid = ppi.eqdid AND eqd.eqdstatus = 'A'
                        LEFT JOIN planointerno.areacultural are ON are.areid = ppi.areid AND are.arestatus = 'A'
                        LEFT JOIN planointerno.produtopi pri ON pri.priid = ppi.priid AND pri.pristatus = 'A'
                        LEFT JOIN planointerno.unidademedidapi ump ON ump.umpid = ppi.umpid AND ump.umpstatus = 'A'        
                        LEFT JOIN planointerno.manutencaoitem mai ON mai.maiid = ppi.maiid and mai.maistatus = 'A'
                        LEFT JOIN planointerno.manutencaosubitem mas on mas.masid = ppi.masid and mas.masstatus = 'A'
                        LEFT JOIN planointerno.ppaobjetivo obj on obj.objid = pmu.objid and obj.objstatus = 'A'
                        LEFT JOIN planointerno.ppainiciativa ini on ini.iniid = pmu.iniid and ini.inistatus = 'A'
                        LEFT JOIN planointerno.metappa mpp on mpp.mppid = ppi.mppid and mpp.mppstatus = 'A'
                        LEFT JOIN planointerno.metapnc mpn on mpn.mpnid = ppi.mpnid and mpn.mpnstatus = 'A'        
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
        $dadosPI['eqdid'] = $this->pegaUm("select eqdid from monitora.pi_enquadramentodespesa eqd where eqdcod = '{$dadosProposta['eqdsigla']}' and eqd.eqdano = '{$dadosProposta['prsano']}' and eqd.eqdstatus = 'A'");
        $dadosPI['maiid'] = $this->pegaUm("select maiid from planacomorc.manutencaoitem mai where upper(mai.mainome) = upper('{$dadosProposta['mainome']}') and mai.prsano = '{$dadosProposta['prsano']}' and mai.maistatus = 'A'");
        $dadosPI['masid'] = $this->pegaUm("select masid from planacomorc.manutencaosubitem mas where upper(mas.masnome) = upper('{$dadosProposta['masnome']}') and mas.prsano = '{$dadosProposta['prsano']}' and mas.masstatus = 'A'");
        $dadosPI['mppid'] = $this->pegaUm("select mppid from public.metappa mpp where mpp.mppcod = '{$dadosProposta['mppcod']}' and mpp.prsano = '{$dadosProposta['prsano']}' and mpp.mppstatus = 'A'");
        $dadosPI['mdeid'] = $this->pegaUm("select mdeid from monitora.pi_modalidadeensino mde where upper(mdedsc) = upper('{$dadosProposta['arenome']}') and mde.mdeano = '{$dadosProposta['prsano']}' and mde.mdestatus = 'A'");
        $dadosPI['pprid'] = $this->pegaUm("select pprid from monitora.pi_produto ppr where upper(ppr.pprnome) = upper('{$dadosProposta['prinome']}') and prsano = '{$dadosProposta['prsano']}' and ppr.pprstatus = 'A'");
        $dadosPI['pumid'] = $this->pegaUm("select pumid from monitora.pi_unidade_medida pum where upper(pum.pumnome) = upper('{$dadosProposta['umpnome']}') and pum.prsano = '{$dadosProposta['prsano']}' and pum.pumstatus = 'A'");
        $dadosPI['oppid'] = $this->pegaUm("select oppid from public.objetivoppa opp where opp.oppcod = '{$dadosProposta['objcod']}' and opp.prsano = '{$dadosProposta['prsano']}' and opp.oppstatus = 'A'");
        $dadosPI['ippid'] = $this->pegaUm("select ippid from public.iniciativappa ipp where ipp.ippcod = '{$dadosProposta['inicod']}' and ipp.prsano = '{$dadosProposta['prsano']}' and ipp.ippstatus = 'A'");

        $MetaPnc = $this->pegaLinha("select mpnid, ipnid from public.indicadorpnc ipn where ipn.ipncod = '" . trim($dadosProposta['mpncod']) . "' and ipn.prsano = '{$dadosProposta['prsano']}' and ipn.ipnstatus = 'A'");

        $dadosPI['mpnid'] = $MetaPnc['mpnid'];
        $dadosPI['ipnid'] = $MetaPnc['ipnid'];

        // Recuperando PTRES
        $sql = "select ptrid 
                from monitora.ptres ptr
                where ptr.acacod = '{$dadosProposta['acacod']}'
                and ptr.prgcod = '{$dadosProposta['prgcod']}'
                and ptr.loccod = '{$dadosProposta['loccod']}'
                and ptr.unicod = '{$dadosProposta['unocod']}'
                and ptr.plocod = '{$dadosProposta['plocod']}'
                and ptr.ptrano = '{$dadosProposta['prsano']}'
                and ptr.ptrstatus = 'A'";
        
        $dadosPI['ptrid'] = $this->pegaUm($sql);

        return $dadosPI;
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
        $aFiltro[] = "suo.suostatus = 'A'";
        $where = $this->montarFiltros($aFiltro);

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
                pli.pliano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.ptrid,
                lmu.lmuvlr,
                sum(pic.picvalorcapital) picvalorcapital,
                sum(pic.picvalorcusteio) picvalorcusteio,
                sum(ptr.ptrdotacao) / 2 ptrdotacao_capital,
                sum(ptr.ptrdotacao) / 2 ptrdotacao_custeio
            from public.vw_subunidadeorcamentaria suo
                inner join spo.ptressubunidade psu on psu.suoid = suo.suoid
                inner join monitora.vw_ptres ptr on ptr.ptrid = psu.ptrid
                left  join monitora.pi_planointernoptres ppt on ppt.ptrid = ptr.ptrid
                left  join monitora.pi_planointerno pli on pli.pliid = ppt.pliid
                left join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                left join planacomorc.unidadegestora_limite lmu on lmu.ungcod = suo.suocod and lmu.lmustatus = 'A' and lmu.prsano = suo.prsano
            where 
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
                pli.pliano,
                ptr.funcional,
                ptr.acatitulo,
                ptr.ptrid,
                lmu.lmuvlr
            ORDER BY
                suo.unonome,
                suo.suonome,
                ptr.funcional
        ";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];
        $dadosAgrupados = [];
        foreach ($dados as $dado){
            $dadosAgrupados[$dado['suocod']]['detalhe'][] = $dado;
            $dadosAgrupados[$dado['suocod']]['dotacaoCapital'] += $dado['ptrdotacao_capital'];
            $dadosAgrupados[$dado['suocod']]['dotacaoCusteio'] += $dado['ptrdotacao_custeio'];
            $dadosAgrupados[$dado['suocod']]['planejadoCapital'] += $dado['picvalorcapital'];
            $dadosAgrupados[$dado['suocod']]['planejadoCusteio'] += $dado['picvalorcusteio'];
            $dadosAgrupados[$dado['suocod']]['totalDotacao'] += ($dadosAgrupados[$dado['suocod']]['dotacaoCapital'] + $dadosAgrupados[$dado['suocod']]['dotacaoCusteio']);
            $dadosAgrupados[$dado['suocod']]['totalPlanejado'] = ($dadosAgrupados[$dado['suocod']]['planejadoCapital'] + $dadosAgrupados[$dado['suocod']]['planejadoCusteio']);
        }
        
        return $dadosAgrupados;
    }

    public function recuperarEstatisticaPagamento($exercicio, $aFiltro = [])
    {
        $sql = "select distinct 2 ordem, 'Limite' as descricao, suo.unosigla categoria, sum(coalesce(lmu.lmuvlr, 0)::decimal) valor
                from public.vw_subunidadeorcamentaria suo 
                        left join planacomorc.unidadegestora_limite lmu on suo.suocod = lmu.ungcod and suo.prsano = lmu.prsano
                where suo.prsano = '$exercicio'
                and suo.unofundo = false
                group by ordem, descricao, categoria
                union all
                select distinct 3 ordem, 'Planejado' as descricao, suo.unosigla categoria, sum(coalesce(pic.picvalorcapital, 0) + coalesce(pic.picvalorcusteio, 0))::decimal valor
                from public.vw_subunidadeorcamentaria suo 
                         left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.pliano = suo.prsano
                         left join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                where suo.prsano = '$exercicio'
                and suo.unofundo = false
                group by ordem, descricao, categoria
                union all
                select distinct 4 ordem, 'Empenhado' as descricao, suo.unosigla categoria, sum(coalesce(sec.vlrempenhado, 0))::decimal valor
                from public.vw_subunidadeorcamentaria suo 
                         left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.pliano = suo.prsano
                         left join monitora.pi_planointernoptres ppt on ppt.pliid = pli.pliid
                         left join monitora.ptres ptr on ptr.ptrid = ppt.ptrid
                         left join spo.siopexecucao sec on sec.ptres = ptr.ptres and sec.exercicio = ptr.ptrano
                where suo.prsano = '$exercicio'
                and suo.unofundo = false
                group by ordem, descricao, categoria
                union all
                select distinct 4 ordem, 'Pago' as descricao, suo.unosigla categoria, sum(coalesce(sec.vlrpago, 0))::decimal valor
                from public.vw_subunidadeorcamentaria suo 
                         left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.pliano = suo.prsano
                         left join monitora.pi_planointernoptres ppt on ppt.pliid = pli.pliid
                         left join monitora.ptres ptr on ptr.ptrid = ppt.ptrid
                         left join spo.siopexecucao sec on sec.ptres = ptr.ptres and sec.exercicio = ptr.ptrano
                where suo.prsano = '$exercicio'
                and suo.unofundo = false
                group by ordem, descricao, categoria
                union all
                select distinct 4 ordem, 'Liquidado' as descricao, suo.unosigla categoria, sum(coalesce(sec.vlrliquidado, 0))::decimal valor
                from public.vw_subunidadeorcamentaria suo 
                         left join monitora.pi_planointerno pli on pli.ungcod = suo.suocod and pli.pliano = suo.prsano
                         left join monitora.pi_planointernoptres ppt on ppt.pliid = pli.pliid
                         left join monitora.ptres ptr on ptr.ptrid = ppt.ptrid
                         left join spo.siopexecucao sec on sec.ptres = ptr.ptres and sec.exercicio = ptr.ptrano
                where suo.prsano = '$exercicio'
                and suo.unofundo = false
                group by ordem, descricao, categoria
                
                order by ordem, categoria
                ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarEstatisticaUnidade($exercicio, $aFiltro = [])
    {
        $sql = "select uno.unosigla descricao, sum(e.autorizado::decimal) valor
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria and uno.prsano = '$exercicio'
                where e.planointerno is not null
                and e.anoexercicio = '$exercicio'
                and e.anoreferencia = '$exercicio'
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
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria and uno.prsano = '$exercicio'
                where e.planointerno is not null
                and e.anoexercicio = '$exercicio'
                and e.anoreferencia = '$exercicio'
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

        switch ($estadoAtual['esdid']){
            case ESD_PI_APROVADO:
            case ESD_PI_CANCELADO:
            case ESD_PI_AGUARDANDO_APROVACAO:
                return (bool)count(array_intersect($perfis, [PFL_ADMINISTRADOR, PFL_SUPERUSUARIO, PFL_PLANEJAMENTO]));
        }

        return true;
    }

    public function getDadosGerais($exercicio)
    {
        $sql = "select  pli.pliid, esd.esddsc, pli.plicod, suo.unocod, suo.unonome, suo.suocod, suo.suonome, 

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
                        
                        eqd.eqddsc, mai.mainome, mas.masnome, ptr.prgcod, ptr.prgdsc, opp.oppcod, opp.oppnome, ipp.ippcod, ipp.ippnome, 
                        ptr.acacod, ptr.acatitulo, ptr.loccod, ptr.locdsc, ptr.plocod, 'PO' po, ptr.ptres,
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
                                                
                        ppr.pprnome, pum.pumnome, picquantidade, (pic.picvalorcusteio + pic.picvalorcapital) valortotal,  pic.picvalorcusteio, pic.picvalorcapital,
                        
                        -- Cronograma Físico
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '01' and pcr.pliid = pli.pliid) fisico_1,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '02' and pcr.pliid = pli.pliid) fisico_2,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '03' and pcr.pliid = pli.pliid) fisico_3,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '04' and pcr.pliid = pli.pliid) fisico_4,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '05' and pcr.pliid = pli.pliid) fisico_5,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '06' and pcr.pliid = pli.pliid) fisico_6,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '07' and pcr.pliid = pli.pliid) fisico_7,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '08' and pcr.pliid = pli.pliid) fisico_8,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '09' and pcr.pliid = pli.pliid) fisico_9,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '10' and pcr.pliid = pli.pliid) fisico_10,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '11' and pcr.pliid = pli.pliid) fisico_11,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 1 and mescod = '12' and pcr.pliid = pli.pliid) fisico_12,
                        
                        -- Cronograma Financeiro Capital
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '01' and pcr.pliid = pli.pliid) fin_capital_1,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '02' and pcr.pliid = pli.pliid) fin_capital_2,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '03' and pcr.pliid = pli.pliid) fin_capital_3,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '04' and pcr.pliid = pli.pliid) fin_capital_4,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '05' and pcr.pliid = pli.pliid) fin_capital_5,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '06' and pcr.pliid = pli.pliid) fin_capital_6,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '07' and pcr.pliid = pli.pliid) fin_capital_7,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '08' and pcr.pliid = pli.pliid) fin_capital_8,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '09' and pcr.pliid = pli.pliid) fin_capital_9,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '10' and pcr.pliid = pli.pliid) fin_capital_10,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '11' and pcr.pliid = pli.pliid) fin_capital_11,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 5 and mescod = '12' and pcr.pliid = pli.pliid) fin_capital_12,
                        
                        -- Cronograma Financeiro Custeio
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '01' and pcr.pliid = pli.pliid) fin_custeio_1,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '02' and pcr.pliid = pli.pliid) fin_custeio_2,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '03' and pcr.pliid = pli.pliid) fin_custeio_3,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '04' and pcr.pliid = pli.pliid) fin_custeio_4,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '05' and pcr.pliid = pli.pliid) fin_custeio_5,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '06' and pcr.pliid = pli.pliid) fin_custeio_6,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '07' and pcr.pliid = pli.pliid) fin_custeio_7,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '08' and pcr.pliid = pli.pliid) fin_custeio_8,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '09' and pcr.pliid = pli.pliid) fin_custeio_9,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '10' and pcr.pliid = pli.pliid) fin_custeio_10,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '11' and pcr.pliid = pli.pliid) fin_custeio_11,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 4 and mescod = '12' and pcr.pliid = pli.pliid) fin_custeio_12,        
                           
                
                        -- Cronograma Orçamentário Capital
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '01' and pcr.pliid = pli.pliid) orc_capital_1,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '02' and pcr.pliid = pli.pliid) orc_capital_2,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '03' and pcr.pliid = pli.pliid) orc_capital_3,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '04' and pcr.pliid = pli.pliid) orc_capital_4,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '05' and pcr.pliid = pli.pliid) orc_capital_5,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '06' and pcr.pliid = pli.pliid) orc_capital_6,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '07' and pcr.pliid = pli.pliid) orc_capital_7,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '08' and pcr.pliid = pli.pliid) orc_capital_8,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '09' and pcr.pliid = pli.pliid) orc_capital_9,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '10' and pcr.pliid = pli.pliid) orc_capital_10,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '11' and pcr.pliid = pli.pliid) orc_capital_11,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 3 and mescod = '12' and pcr.pliid = pli.pliid) orc_capital_12,
                                                                                                                                         
                        -- Cronograma Orçamentário Custeio                                                                               
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '01' and pcr.pliid = pli.pliid) orc_custeio_1,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '02' and pcr.pliid = pli.pliid) orc_custeio_2,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '03' and pcr.pliid = pli.pliid) orc_custeio_3,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '04' and pcr.pliid = pli.pliid) orc_custeio_4,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '05' and pcr.pliid = pli.pliid) orc_custeio_5,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '06' and pcr.pliid = pli.pliid) orc_custeio_6,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '07' and pcr.pliid = pli.pliid) orc_custeio_7,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '08' and pcr.pliid = pli.pliid) orc_custeio_8,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '09' and pcr.pliid = pli.pliid) orc_custeio_9,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '10' and pcr.pliid = pli.pliid) orc_custeio_10,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '11' and pcr.pliid = pli.pliid) orc_custeio_11,
                        (select pcrvalor from planacomorc.pi_cronograma pcr where crvid = 2 and mescod = '12' and pcr.pliid = pli.pliid) orc_custeio_12,         
                           
                        vlrempenhado, vlrliquidado, vlrpago
                           
                from monitora.pi_planointerno pli
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo on suo.suocod = pli.ungcod and suo.unocod = pli.unicod and suo.prsano = pli.pliano
                        inner join monitora.pi_enquadramentodespesa eqd on eqd.eqdid = pli.eqdid
                        left join planacomorc.manutencaoitem mai on mai.maiid = pic.maiid
                        left join planacomorc.manutencaosubitem mas on mas.masid = pic.masid
                        left join monitora.pi_planointernoptres ppt on ppt.pliid = pli.pliid
                        left join monitora.vw_ptres ptr on ptr.ptrid = ppt.ptrid and ptr.ptrano = pli.pliano
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
                        left join (
                                select sex.exercicio, sex.plicod, sum(sex.vlrempenhado) vlrempenhado, sum(sex.vlrliquidado) vlrliquidado, sum(sex.vlrpago) vlrpago
                                from spo.siopexecucao sex
                                where coalesce(sex.plicod, '') != ''
                                group by sex.exercicio, sex.plicod                                
                        ) sex on sex.plicod = pli.plicod and sex.exercicio = pli.pliano
                
                -- where pli.pliano = '$exercicio'
                ";

//        ver($sql);
        
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function getCorPainelPorValorDisponivel($valorDisponivel)
    {
        if($valorDisponivel > 0) return 'blue';

        if($valorDisponivel < 0) return 'red';

        return 'green';
    }

    public function recuperarPiPorSubunidade($suoid)
    {
        $dadosPi = $this->recuperarDadosDetalhe($suoid, self::K_TIPO_DETALHE_PI);
        $dadosDelegado = $this->recuperarDadosDetalhe($suoid, self::K_TIPO_DETALHE_DELEGADO);

        include_once APPRAIZ . 'planacomorc/modulos/principal/unidade/detalhe-subunidade.inc';
    }

    public function recuperarPiPorFuncional($ptrid)
    {
        $dados = $this->recuperarDadosDetalhe($ptrid, self::K_TIPO_DETALHE_PTRES);

        include_once APPRAIZ . 'planacomorc/modulos/principal/unidade/detalhe-ptres.inc';
    }

    public function recuperarDadosDetalhe($id, $tipo)
    {
        $where = '';
        switch ($tipo){
            case self::K_TIPO_DETALHE_PTRES:    $where = " and ptr.ptrid = '$id'"; break;
            case self::K_TIPO_DETALHE_PI:       $where = " and suo.suoid = '$id'"; break;
            case self::K_TIPO_DETALHE_DELEGADO: $where = " and pde.suoid = '$id'"; break;
        }

        $sql = "select  distinct pli.pliid, pli.plititulo, pli.plicod, pli.pliemenda, pic.picted, pic.picedital, pic.picvalorcapital, pic.picvalorcusteio, 
                        suo.suocod, suo.suonome, suo.unocod, suo.unonome, suo.unosigla, ptr.funcional, ptr.acatitulo, ptr.ptrdotacao, eqd.eqddsc
                from monitora.pi_planointerno pli
                        inner join planacomorc.pi_complemento pic on pic.pliid = pli.pliid
                        left join planacomorc.pi_delegacao pde on pde.pliid = pli.pliid
                        inner join public.vw_subunidadeorcamentaria suo on suo.suocod = pli.ungcod and suo.unocod = pli.unicod and suo.prsano = pli.pliano
                        inner join monitora.pi_planointernoptres ppt on ppt.pliid = pli.pliid
                        inner join monitora.vw_ptres ptr on ptr.ptrid = ppt.ptrid and ptr.ptrano = pli.pliano
                        left join monitora.pi_enquadramentodespesa eqd on eqd.eqdid = pli.eqdid
                where pli.plistatus = 'A'        
                $where
                ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}