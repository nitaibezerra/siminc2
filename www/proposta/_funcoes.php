<?php

/**
 * Monta consulta do relatório geral de Propostas
 * 
 * @param stdClass $filtros
 * @return string
 */
function montarSqlRelatorioGeralProposta(stdClass $filtros){
    $sql = "
        SELECT
            pro.proid,
            suo.unosigla || ' - ' || suo.suonome subunidade,
            eqd.eqddsc,
	    ptr.irpcod,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc,
            aca.locquantidadeproposta,
            pro.proquantidade,
            pro.proquantidadeexpansao,
            pro.projustificativa,
	    pro.projustificativaexpansao,
	    prd.ndpid,
	    prd.iducod,
	    prd.foncod,
	    prd.idoid,
	    prd.prdvalor,
	    prd.prdvalorexpansao
        FROM proposta.proposta pro
            JOIN monitora.vw_ptres ptr ON pro.ptrid = ptr.ptrid
	    JOIN monitora.acao aca ON ptr.acaid = aca.acaid
            JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pro.suoid
            JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pro.eqdid -- SELECT eqddsc,* FROM monitora.pi_enquadramentodespesa
            LEFT JOIN proposta.propostadetalhe prd ON prd.proid = pro.proid -- SELECT * FROM proposta.propostadetalhe
        WHERE
            pro.prsano = '". (int)$filtros->exercicio. "'
            AND prostatus = 'A'
        ORDER BY
            pro.proid,
            subunidade,
            eqd.eqddsc,
	    ptr.irpcod,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc
    ";
    return $sql;
}

/**
 * Monta consulta do relatório geral de Pre-PIs
 * 
 * @param stdClass $filtros
 * @return string
 */
function montarSqlRelatorioGeralPrePi(stdClass $filtros){
    $sql = "
        SELECT
	    suo.unosigla || ' - ' || suo.suonome subunidade,
            ptr.funcional,
            ptr.acatitulo,
            ptr.plodsc,
            eqd.eqddsc,
            esd.esddsc,
            pli.pliid,
            pli.plititulo,
            pli.plidsc,
            pli.suoid,
            pli.eqdid,
            pli.maiid,
            pli.masid,
            pli.ptrid,
            pli.oppid,
            pli.mppid,
            pli.ippid,
            pli.mpnid,
            pli.ipnid,
            pli.pprid,
            pli.pumid,
            pli.pliquantidade,
            pli.mdeid,
            pli.neeid,
            pli.plivalorcusteio,
            pli.plivalorcapital,
            pli.docid,
            pli.prsano,
            pli.plistatus,
            pli.plivalorcusteioadicional,
            pli.plivalorcapitaladicional,
            pli.pliquantidadeadicional,
            pli.plijustificativaadicional,
            pli.esfid
        FROM proposta.preplanointerno pli
            JOIN monitora.vw_ptres ptr ON pli.ptrid = ptr.ptrid
            JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = pli.suoid
            JOIN monitora.pi_enquadramentodespesa eqd ON eqd.eqdid = pli.eqdid
            LEFT JOIN workflow.documento doc ON doc.docid = pli.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        WHERE
            pli.prsano = '". (int)$filtros->exercicio. "'
            AND plistatus = 'A'
    ";
    return $sql;
}

