<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select
		    dopid,
		    proid,
		    obra
		from(
		select distinct
				    d.dopid,
				    d.proid,
				    array_to_string(array(select preid from par.termocomposicao t where t.dopid = d.dopid and t.preid is not null), ', ') as obra
				from par.vm_documentopar_ativos d 
					inner join par.modelosdocumentos m on m.mdoid = d.mdoid
				where 
					d.dopvalortermo is null 
				    and d.proid is not null
					and m.tpdcod in (21, 102, 103)
		) as foo
		where
			obra <> ''";
$arrDoc = $db->carregar($sql);
$arrDoc = $arrDoc ? $arrDoc : array();

foreach ($arrDoc as $v) {
	
	$sql = "SELECT DISTINCT 
				po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				CASE WHEN tpo.ptocategoria IS NOT NULL THEN
					(SELECT sum(itc2.itcvalorunitario*itc2.itcquantidade) FROM obras.preitenscomposicaomi itc2 WHERE po.ptoid = itc2.ptoid AND itc2.preid = po.preid AND itc2.ptoid = po.ptoid) 
				ELSE
					( SELECT sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) FROM obras.preitenscomposicao itc 
						INNER JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid WHERE tpo.ptocategoria IS NULL AND itc.ptoid = po.ptoid)
				END as valor,
				po.prebairro, po.precep,
				po.prelogradouro, mun.mundescricao, est.estdescricao, s.sbaid, d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo
			FROM par.empenhoobrapar  p
				INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
				INNER JOIN par.subacaoobra so ON so.preid = po.preid
				inner join par.subacao s on s.sbaid = so.sbaid and s.sbastatus = 'A'
				inner join par.acao a on a.aciid = s.aciid
				inner join par.pontuacao pon on pon.ptoid = a.ptoid
				inner join par.criterio c on c.crtid = pon.crtid
				inner join par.indicador i on i.indid = c.indid
				inner join par.area are on are.areid = i.areid
				inner join par.dimensao d on d.dimid = are.dimid
				INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid 
				LEFT JOIN territorios.municipio mun on mun.muncod = po.muncod
				LEFT JOIN territorios.estado est on est.estuf = po.estuf
				INNER JOIN par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
				INNER JOIN par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = {$v['proid']}
				and po.preid in ({$v['obra']})
			group by
				po.ptoid, tpo.ptocategoria, po.preid, p.eobvalorempenho, po.muncod, po.estuf, po.predescricao,
				po.prebairro, po.precep, po.prelogradouro, mun.mundescricao, est.estdescricao, tpo.ptodescricao, d.dimcod,
				are.arecod, i.indcod, sbaordem, s.sbaid
			ORDER BY
				codigo";
	$arrTermo = $db->carregar($sql);
	$arrTermo = $arrTermo ? $arrTermo : array();
	
	$vrlTermo = 0;
	foreach ($arrTermo as $termo) {
		$vrlTermo += (float)$termo['valor'];
	}
	$sql = "UPDATE par.documentopar SET dopvalortermo = ".round($vrlTermo, 2)." WHERE dopid = {$v['dopid']} and proid = {$v['proid']}";
	$db->executar($sql);
	$db->commit();
}

$sql = "select
		    dopid,
		    prpid,
		    subacao
		from(
			select distinct
			    d.dopid,
			    d.prpid,
			    array_to_string(array(select sbdid from par.termocomposicao t where t.dopid = d.dopid and t.sbdid is not null), ', ') as subacao
			from par.vm_documentopar_ativos d 
				inner join par.modelosdocumentos m on m.mdoid = d.mdoid
			where 
				d.dopvalortermo is null 
			    and d.prpid is not null
				and m.tpdcod in (21, 102, 103)
		) as foo
		where
			subacao <> ''";
$arrDoc = $db->carregar($sql);
$arrDoc = $arrDoc ? $arrDoc : array();

foreach ($arrDoc as $v) {
	$sql = "SELECT
				foo.codigo,
                foo.picdescricao,
                foo.ptsdescricao,
                foo.picid,
                foo.sbdano,
                foo.sbaid,
                foo.quantidade,
                foo.valor,
                ( foo.quantidade * foo.valor ) as total
			FROM (
				SELECT
                	pic.picdescricao,
                    pts.ptsdescricao,
                    pic.picid,
                    sd.sbdano as sbdano,
                    s.sbaid,
                    CASE WHEN sbacronograma = 1 --global
	                    THEN
		                    CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
		                    	sum(coalesce(sic.icoquantidadetecnico,0))
		                    END
	                    ELSE -- escolas
	                    	par.recuperaquantidadeitemvalidado( sic.icoid )	
                    END as quantidade,
                    sic.icovalor as valor,
                    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo
				FROM
                	par.processopar pp
                INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
				INNER JOIN par.subacaodetalhe 			sd  ON sd.sbdid = ppc.sbdid
                INNER JOIN par.subacao       			s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
                LEFT JOIN par.propostasubacao 			pps 
					INNER JOIN par.propostatiposubacao		pts ON pts.ptsid = pps.ptsid
                	ON pps.ppsid = s.ppsid
                INNER JOIN par.acao 					a   ON a.aciid = s.aciid
				INNER JOIN par.pontuacao 				pon ON pon.ptoid = a.ptoid
				INNER JOIN par.criterio 				c   ON c.crtid = pon.crtid
				INNER JOIN par.indicador 				i   ON i.indid = c.indid
				INNER JOIN par.area 					are ON are.areid = i.areid
				INNER JOIN par.dimensao 				d   ON d.dimid = are.dimid
                LEFT  JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND icostatus = 'A'
				LEFT  JOIN par.subacaoescolas 			se
					INNER JOIN par.escolas 				esc ON esc.escid = se.escid
					INNER JOIN entidade.entidade 		en  ON en.entid = esc.entid and en.tpcid = 3
					INNER JOIN entidade.funcaoentidade 	f   ON f.entid = en.entid AND f.funid = 3
					ON se.sbaid = s.sbaid
				LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
                INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
				WHERE
					sd.sbdid in ({$v['subacao']})
                    and s.sbastatus = 'A' AND
                    pp.prpid = {$v['prpid']} AND
                    pp.prpstatus = 'A' and
                    CASE WHEN sbacronograma = 1
	                    THEN sic.icovalidatecnico <> 'N'
	                    ELSE
		                    CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
		                    	se.sesvalidatecnico <> 'N'
		                    ELSE
		                    	sic.icovalidatecnico <> 'N'
		                    END
                    END
				GROUP BY
                	s.sbaid,
					sd.sbdano,
					sic.icoid,
					pic.picdescricao,
					pic.picid,
					s.sbacronograma,
					sic.icovalidatecnico,
					sic.icoquantidade,
					s.frmid,
					s.ptsid,
					se.sesvalidatecnico,
					sic.icovalor,
					d.dimcod,
					are.arecod,
					i.indcod,
					sbaordem,
					pts.ptsdescricao
			) as foo ORDER BY foo.codigo";
	
	$arrTermo = $db->carregar($sql);
	$arrTermo = $arrTermo ? $arrTermo : array();
	
	$vrlTermo = 0;
	foreach ($arrTermo as $termo) {
		$vrlTermo += (float)$termo['total'];
	}
	$sql = "UPDATE par.documentopar SET dopvalortermo = ".round($vrlTermo, 2)." WHERE dopid = {$v['dopid']} and prpid = {$v['prpid']}";
	$db->executar($sql);
	$db->commit();
}
?>