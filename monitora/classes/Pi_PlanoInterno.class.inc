<?php
	
class Pi_PlanoInterno extends Modelo{
	
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

    public function recuperarExecucaoOrcamentaria($exercicio, $aFiltro = [])
    {
        $sql = "select e.unidadeorcamentaria, e.acao, e.localizador, e.programa, e.planoorcamentario, e.planointerno, e.numeroptres, count(*)
                    , sum(e.autorizado::decimal) autorizado, sum(e.bloqueadoremanejamento::decimal) bloqueadoremanejamento, sum(e.bloqueadosof::decimal) bloqueadosof
                    , sum(e.creditocontidosof::decimal) creditocontidosof, sum(e.disponivel::decimal) disponivel, sum(e.dotatual::decimal) dotatual
                    , sum(e.dotinicialsiafi::decimal) dotinicialsiafi, sum(e.dotacaoantecipada::decimal) dotacaoantecipada, sum(e.dotacaoinicial::decimal) dotacaoinicial
                    , sum(e.empliquidado::decimal) empliquidado, sum(e.empenhadoaliquidar::decimal) empenhadoaliquidar, sum(e.indisponivel::decimal) indisponivel, sum(e.pago::decimal) pago
                from wssof.ws_execucaoorcamentariadto e
                where e.planointerno is not null
                and e.anoexercicio = '$exercicio'
                and e.anoreferencia = '$exercicio'
                -- and e.unidadeorcamentaria = '42206'
                --and e.planointerno = '7CNM0020001'
                group by e.unidadeorcamentaria, e.acao, e.localizador, e.programa, e.planoorcamentario, e.planointerno, e.numeroptres
                order by e.unidadeorcamentaria, e.acao, e.localizador, e.programa, e.planoorcamentario";

        // Carregando dados da base de dados do SIMINC antigo
        $dados = adapterConnection::oldSiminc()->carregar($sql);

        return $dados ? $dados : [];
    }

    public function recuperarEstatisticaPagamento($exercicio, $aFiltro = [])
    {
        $sql = "select 1 ordem, 'Autorizado' as descricao, uno.unosigla categoria, sum(e.autorizado::decimal) valor
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria and uno.prsano = '$exercicio'
                where e.planointerno is not null
                and e.anoexercicio = '$exercicio'
                and e.anoreferencia = '$exercicio'
                group by uno.unosigla
                union all
                select 2, 'Pago', uno.unosigla, sum(e.pago::decimal) pago
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria and uno.prsano = '$exercicio'
                where e.planointerno is not null
                and e.anoexercicio = '$exercicio'
                and e.anoreferencia = '$exercicio'
                group by uno.unosigla
                union all
                select 3, 'A Liquidar', uno.unosigla, sum(e.empenhadoaliquidar::decimal)
                from wssof.ws_execucaoorcamentariadto e
                        inner join planointerno.unidadeorcamentaria uno on unocod = e.unidadeorcamentaria and uno.prsano = '$exercicio'
                where e.planointerno is not null
                and e.anoexercicio = '$exercicio'
                and e.anoreferencia = '$exercicio'
                group by uno.unosigla                
                order by ordem, descricao, categoria
                ";

        // Carregando dados da base de dados do SIMINC antigo
        $dados = adapterConnection::oldSiminc()->carregar($sql);
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
}