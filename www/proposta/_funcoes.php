<?php

/**
 * Monta consulta do relatório geral
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

