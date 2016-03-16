<?php

function nao_possui_historico_execucao($tcpid = null) {
    global $db;

    if ($tcpid) {

        if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
            return true;
        }

        $strSQL = "
            select
                count(*) as is_execucao
            from
                workflow.historicodocumento
            where
                docid = (select docid from ted.termocompromisso where tcpid = {$tcpid}) and
                aedid in (1609, 1618, 1650)
        ";

        $is_execucao = $db->pegaUm($strSQL);
        $db->close();

        return ($is_execucao == 0) ? true : false;
    }
}

function possui_historico_execucao($tcpid = null) {
    global $db;

    if ($tcpid) {

        if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
            return true;
        }

        $strSQL = "
            select
                count(*) as is_execucao
            from
                workflow.historicodocumento
            where
                docid = (select docid from ted.termocompromisso where tcpid = {$tcpid}) and
                aedid in (1609, 1618, 1650)
        ";

        $is_execucao = $db->pegaUm($strSQL);
        $db->close();

        return ($is_execucao > 0) ? true : false;
    }
}

/**
 * Verifica se o perfil UO/Equipe Tecnica é vinculado a algum Orgão Concedente
 * @return bool
 */
function uoEquipeTecnicaConcedente($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    $message = 'Apenas o UO/Equipe Técnica do Concedente pode tramitar';

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil_gestor(array(UO_EQUIPE_TECNICA))) {

        $strSQL = "
            select * from ted.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                ungcod = (
                    select
                        CASE WHEN (SELECT true from ted.termocompromisso where tcpid = {$tcpid} and ungcodconcedente = '".UG_FNDE."') THEN
                            CASE when (SELECT true from ted.termocompromisso where tcpid = {$tcpid} AND ungcodpoliticafnde is not null) then
                                    (SELECT ungcodpoliticafnde from ted.termocompromisso where tcpid = {$tcpid})
                                 when (SELECT true from ted.termocompromisso where tcpid = {$tcpid} and dircodpoliticafnde::text is not null) then
                                    (SELECT ungcodconcedente from ted.termocompromisso where tcpid = {$tcpid})
                            END
                            ELSE
                                (select ungcodconcedente from ted.termocompromisso where tcpid = {$tcpid})
                        END as ungcodconcedente
                    from ted.termocompromisso where tcpid = {$tcpid}
                )
        ";

        $linha = $db->pegaLinha($strSQL);
        return ($linha) ? true : $message;
    }

    return $message;
}

function uoEquipeTecnicaProponente($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $message = 'Apenas o UO/Equipe Técnica do Proponente pode tramitar';

    if (possui_perfil_gestor(array(PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA))) {

        $strSQL = "
            select * from ted.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                ungcod = (select ungcodproponente from ted.termocompromisso where tcpid = {$tcpid})
        ";

        $linha = $db->pegaLinha($strSQL);
        $db->close();

        return ($linha) ? true : $message;
    }

    return $message;
}

function verificaTermoEmCadastramento($tcpid) {
    global $db;
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if ($tcpid) {

        $retorno = uoEquipeTecnicaProponente();
        if (is_string($retorno)) {
            return $retorno;
        }

        $sql = "
            select
                t.tcpid as termo,
                (select count(proid) from ted.previsaoorcamentaria p where p.tcpid = t.tcpid) as totalprevisao,
                (select count(arptipo) from ted.arquivoprevorcamentaria apo where apo.tcpid = t.tcpid AND apo.arptipo = 'A') as totalanexos,
                t.ungcodproponente,
                t.ungcodconcedente,
                (select identificacao from ted.justificativa where tcpid = t.tcpid) as tcpdscobjetoidentificacao
            from ted.termocompromisso t
            where t.tcpid = {$tcpid}
        ";
        $dados = $db->pegaLinha($sql);
        $db->close();

        $arErro = array();
        if (!$dados['ungcodproponente']) 			$arErro[] = 'Proponente(1)';
        if (!$dados['ungcodconcedente']) 			$arErro[] = 'Concedente(2)';
        if (!$dados['tcpdscobjetoidentificacao']) 	$arErro[] = 'Objeto e Justificativa da Descentralização do Crédito(3)';
        if (!$dados['totalprevisao']) 				$arErro[] = 'Previsão Orçamentária(4)';
        if (!$dados['totalanexos']) 				$aErro[] = 'Anexo(5)';

        if (empty($arErro)) {
            return true;
        } else {
            return 'É necessário preencher a(s) aba(s) '.implode(', ', $arErro);
        }
    }

    return 'Somente o UO/Equipe Técnica do Proponente pode tramitar.';
}

function rco_prazo_vencido($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $rco = new Ted_Model_Business_RelatorioCumprimentoObjeto();
    $isFillRCO = $db->pegaUm("
        select rel.recid
        from ted.relatoriocumprimento rel
        join ted.relcumprimentonc nc on nc.recid = rel.recid
        where
        rel.tcpid = {$tcpid} AND
        rel.recstatus = 'A' AND
        nc.rpustatus = 'A'
    ");

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO)) && $rco->termoVencido($tcpid)) {
        if ($isFillRCO)
            return true;
        else
            return 'Falta preencher o relatório de cumprimento.';
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA))) {

        $strSQL = "
            select * from ted.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                rpustatus = 'A' and
                ungcod = (select ungcodproponente from ted.termocompromisso where tcpid = {$tcpid})
        ";
        $linha = $db->pegaLinha($strSQL);
        $db->close();

        if ($linha && $rco->termoVencido($tcpid)) {
            if ($isFillRCO)
                return true;
            else
                return 'Falta preencher o relatório de cumprimento.';

        } else
            return false;
    }

    return false;
}

/**
 * Verifica se existe pendencia no relatorio de cumprimento do objeto
 * @param null $tcpid
 * @return bool
 */
function verificar_pendencia_relatorio_cumprimento($tcpid = null) {
    global $db;

    if (!$tcpid) {
        return false;
    }

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
    if ($db->testa_superuser() || in_array(PERFIL_UG_REPASSADORA, $perfis) || in_array(PERFIL_CGSO, $perfis)) {
        return true;
    }

    $ungcod = $db->pegaUm("
	    select
            tcp.ungcodconcedente
        from ted.termocompromisso tcp
        join ted.usuarioresponsabilidade usr on ( usr.ungcod = tcp.ungcodpoliticafnde or usr.ungcod = tcp.ungcodconcedente )
            and usr.rpustatus = 'A' and usr.usucpf = '{$_SESSION['usucpf']}'
        where tcp.tcpid = {$tcpid} and usr.pflcod = ".UO_EQUIPE_TECNICA."
	");
    $db->close();

    $objeto = new Ted_Model_RelatorioCumprimento_Business($tcpid);
    $objeto->_carregaTermosVencidos();
    if ($objeto->getPendenciaTermoRelacionado() && !$db->testa_superuser()) {
        return 'Unidade proponente com pendência em preenchimento do relatório de cumprimento do objeto.';
    }

    $return = verificaAprovacaoGestor();
    if (is_bool($return)) {
        return verificaTermoSemAlteracao($tcpid);
    } else {
        return $return;
    }
}

function verificaTermoSemAlteracaoDiligencia($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if ($tcpid) {
        $sql = "select docid from ted.termocompromisso where tcpid = {$tcpid}";
        $docid = $db->pegaUm($sql);
    }

    $sql = "
        select hstid from workflow.historicodocumento h
        inner join workflow.acaoestadodoc a on a.aedid = h.aedid
        where h.docid = {$docid}
        and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO.",".EM_DILIGENCIA." ) limit 1
    ";
    $rs = $db->pegaUm($sql);
    $db->close();

    return ($rs) ? false : true;
}

function verificaTermoAlteracaoDiligencia($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if ($tcpid) {
        $sql = "select docid from ted.termocompromisso where tcpid = {$tcpid}";
        $docid = $db->pegaUm($sql);
    }

    $sql = "
        select hstid from workflow.historicodocumento h
        inner join workflow.acaoestadodoc a on a.aedid = h.aedid
        where h.docid = {$docid}
        and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO.", ".EM_DILIGENCIA." ) limit 1
    ";
    $rs = $db->pegaUm($sql);
    $db->close();

    return ($rs) ? true : false;
}

function enviarParaCoordenacao($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if ($tcpid) {
        $sql = "select cooid from ted.termocompromisso where tcpid = {$tcpid}";
        $cooid = $db->pegaUm($sql);

        $sql = "
            select count(*) as total from
            ted.termocompromisso t
            inner join ted.coordenacao c on c.ungcodconcedente = t.ungcodconcedente
            where t.tcpid = {$tcpid}
        ";

        $dado = $db->pegaUm($sql);
        $db->close();

        if($cooid>0 || $dado==0){
            return true;
        }
    }
    return false;
}

function verificaPodeEnviarDiligencia($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    $ungcod = $db->pegaUm("
	    select
            tcp.ungcodconcedente
        from ted.termocompromisso tcp
        join ted.usuarioresponsabilidade usr on ( usr.ungcod = tcp.ungcodpoliticafnde or usr.ungcod = tcp.ungcodconcedente )
            and usr.rpustatus = 'A' and usr.usucpf = '{$_SESSION['usucpf']}'
        where tcp.tcpid = {$tcpid} and usr.pflcod = ".UO_EQUIPE_TECNICA."
	");
    $db->close();

    if (in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis)) {
        return true;
    } else if ($ungcod && in_array(UO_EQUIPE_TECNICA, $perfis)) {
        return true;
    }

    return false;
}

function verificaTermoSemAlteracao($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $sql = "select docid from ted.termocompromisso where tcpid = {$tcpid}";
    $docid = $db->pegaUm($sql);
    $estadoAtual = pegarEstadoAtual($tcpid);

    if ($estadoAtual != TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP) {
        $sql = "select hstid from workflow.historicodocumento h
				inner join workflow.acaoestadodoc a on a.aedid = h.aedid
				where h.docid = {$docid}
				and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO." ) limit 1 ";

        if ($db->pegaUm($sql)) {
            $db->close();
            return false;
        }
    }

    $db->close();
    if ($estadoAtual == EM_ANALISE_OU_PENDENTE) {
        if (!verificaPreenchimentoEmAnaliseCoordenacao($tcpid)) {
            return false;
        }
    }

    return true;
}

function verificaPreenchimentoEmAnaliseCoordenacao($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $sql = "
        select
            considentproponente,
            considproposta,
            considobjeto,
            considobjetivo,
            considjustificativa,
            considvalores,
            considcabiveis,
            usucpfparecer
        from
            ted.parecertecnico
        where
            tcpid = {$tcpid}
    ";
    $dadoParecer = $db->pegaLinha($sql);

    if(empty($dadoParecer['considentproponente']) ||
        empty($dadoParecer['considproposta']) ||
        empty($dadoParecer['considobjeto']) ||
        empty($dadoParecer['considobjetivo']) ||
        empty($dadoParecer['considjustificativa']) ||
        empty($dadoParecer['considvalores']) ||
        empty($dadoParecer['considcabiveis']) ||
        empty($dadoParecer['usucpfparecer']))
    {
        return false;
    }

    $sql = "select * from ted.previsaoorcamentaria where tcpid = {$tcpid} and prostatus = 'A'";
    $rs = $db->carregar($sql);
    $db->close();

    if (!$rs) return 'É necessário preencher a programação orçamentária';

    foreach($rs as $dado) {
        if(empty($dado['ptrid']) || empty($dado['pliid']) || empty($dado['crdmesliberacao'])) {
            return false;
        }
    }

    return true;
}

function verificaTermoAlteracao($tcpid = null) {
    global $db;

    if (!$tcpid) {
        return false;
    }

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $return = verificaPendenciaAbaPrevisaoOrcamentaria($tcpid);
    if ($return) {
        foreach ($return as $linha) {
            if (empty($linha['ptrid_descricao']) ||
                empty($linha['pliid_descricao']) ||
                empty($linha['crdmesliberacao']))
            {
                return 'Verifique a pendência na Previsão Orçamentária';
            }
        }
        $previsao = true;
    }

    $linha = verificaPendenciaAbaParecer($tcpid);
    if ($linha) {
        if (empty($linha['considentproponente']) ||
            empty($linha['considproposta']) ||
            empty($linha['considobjeto']) ||
            empty($linha['considobjetivo']) ||
            empty($linha['considjustificativa']) ||
            empty($linha['considvalores']) ||
            empty($linha['considcabiveis']) ||
            empty($linha['usucpfparecer']))
        {
            return 'Verifique a pendência no Parecer Técnico';
        }
        $parecer = true;
    }

    $sql = "select docid from ted.termocompromisso where tcpid = {$tcpid}";
    $docid = $db->pegaUm($sql);

    $sql = "select hstid from workflow.historicodocumento h
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			where h.docid = {$docid}
			and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO." ) limit 1 ";

    $rs = $db->pegaUm($sql);
    $db->close();

    if ($rs && $previsao && $parecer) {
        return true;
    }

    return false;
}

function verificaPendenciaAbaPrevisaoOrcamentaria($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $sql = "
        SELECT DISTINCT
            pro.proid,
            ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
            substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
            substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
            pro.ptrid,
            a.acacod,
            pro.pliid,
            case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
            pro.ndpid,
            to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
            coalesce(pro.provalor, 0) as valor,
            crdmesliberacao,
            crdmesexecucao,
            pro.proid,
            pro.proanoreferencia,
            pro.prodata,
            (select ppa2.codncsiafi from ted.previsaoparcela ppa2 where ppa2.ppaid = (select max(ppa1.ppaid) from ted.previsaoparcela ppa1 where ppa1.proid = pro.proid) and ppa2.ppacancelarnc = 'f') as lote,
            pp.codsigefnc,
            pp.codncsiafi,
            tc.ungcodconcedente
        FROM ted.previsaoorcamentaria pro
        LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
        LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
        LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
        LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
        LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
        LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
        LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
        LEFT JOIN ted.previsaoparcela pp		ON (pp.proid = pro.proid)
        LEFT JOIN ted.termocompromisso tc       ON (tc.tcpid = pro.tcpid)
        LEFT JOIN public.unidadegestora unc         ON (unc.ungcod = tc.ungcodconcedente)
        WHERE pro.prostatus = 'A'
        AND pro.tcpid = {$tcpid}
        ORDER BY lote, pro.proanoreferencia DESC, crdmesliberacao
    ";
    //ver($sql,d);
    return !empty($tcpid) ? $db->carregar($sql) : array();
}

function verificaPendenciaAbaParecer($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $sql = "
        select
			considentproponente,
            considproposta,
            considobjeto,
            considobjetivo,
            considjustificativa,
            considvalores,
            considcabiveis,
            usucpfparecer
		from
			ted.parecertecnico
		where
			tcpid = {$tcpid}
    ";

    return !empty($tcpid) ? $db->pegaLinha($sql) : array();
}

/**
 * Busca todos os termos que tem o FNDE como concedente
 * E que o responsável pela politica é alguma diretoria do FNDE
 * Se a condição for verdadeira retorna false
 * para ocultar a ação de "Enviar para representante legal do proponente"
 * @param $tcpid
 */
function verificaConcedenteFNDEpoliticaSecretariaFnde($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $rs = $db->pegaLinha("select tc.tcpid, tc.ungcodconcedente from ted.termocompromisso tc where tc.tcpid = {$tcpid}");
    if ($rs) {
        if ($rs['ungcodconcedente'] != '153173') return false;
    }

    $strSQL = "
        select
          tc.tcpid, d.dircod, d.dirdsc, d.dirstatus, d.ungcod
        from ted.termocompromisso tc
        INNER JOIN ted.diretoria d ON (ungcodpoliticafnde = d.ungcod)
        where
            ungcodpoliticafnde is not null
            and ungcodconcedente = '153173'
            and tc.tcpid = {$tcpid}
    ";

    $rs = $db->carregar($strSQL);
    $db->close();

    return ($rs) ? true : false;
}

/**
 * Condição do workflow,
 * Quando o termo esta em aprovação pela Diretoria
 * E o concedente é o FNDE, não tramitar direto para
 * Representante Legal do Concedente
 * @return bool
 */
function verificaTermoFNDEcomPolitica($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (verificaDiretoraAutarquiaFNDE()) {
        return false;
    }

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $strSQL = "
        SELECT DISTINCT
            tc.tcpid, tc.ungcodconcedente, tc.ungcodproponente
        FROM ted.termocompromisso tc
        WHERE
            (ungcodpoliticafnde is not null OR dircodpoliticafnde is not null) AND
            tc.tcpstatus = 'A' AND
            tc.ungcodconcedente  = '153173' AND
            tc.tcpid = {$tcpid};
    ";

    $rs = $db->pegaLinha($strSQL);
    $db->close();

    return ($rs) ? FALSE : TRUE;
}

/**
 * Busca todos os termos que tem o FNDE como concedente
 * E que o responsável pela politica é alguma diretoria do FNDE
 * Se a condição for verdadeira retorna false
 * para ocultar a ação de "Enviar para representante legal do proponente"
 * @param $tcpid
 */
function verificaConcedenteFNDEpoliticaDiretoriaFnde($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (verificaDiretoraAutarquiaFNDE()) {
        return false;
    }

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $rs = $db->pegaLinha("select tc.tcpid, tc.ungcodconcedente from ted.termocompromisso tc where tc.tcpid = {$tcpid}");
    if ($rs) {
        if ($rs['ungcodconcedente'] != '153173') return false;
    }

    $strSQL = "
        select
            tc.tcpid, d.dircod, d.dirdsc, d.dirstatus, d.ungcod
        from ted.termocompromisso tc
        INNER JOIN ted.diretoria d ON (dircodpoliticafnde = d.dircod)
        where
            dircodpoliticafnde is not null
            and ungcodconcedente = '153173'
            and tc.tcpid = {$tcpid}
    ";

    $rs = $db->carregar($strSQL);
    $db->close();

    return ($rs) ? true : false;
}

function verificaConcedenteNaoFnde($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $sql = sprintf("
        select true
        from ted.termocompromisso
        where
            tcpid = %d
            and ungcodconcedente = '%s'
    ", $tcpid, UG_FNDE);
    $rsFNDE = $db->pegaUm($sql);
    $db->close();

    return ($rsFNDE) ? false : true;
}

function verificaConcedenteFnde($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $sql = sprintf("
        select true from
            ted.termocompromisso
        where tcpid = %d
        and ungcodconcedente = '%s'
    ", $tcpid, UG_FNDE);

    $rsFNDE = $db->pegaUm($sql);
    $db->close();

    return ($rsFNDE == 't') ? true : false;
}

/**
 * Verifica se existe nota de crédito cadastrada
 * antes de permitir o envio para execucao
 * @param null $tcpid
 * @return bool|string
 */
function verificaConcedenteFndeEnviarNc($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $isFnde = $db->pegaUm(sprintf("select true from ted.termocompromisso where ungcodconcedente = '%s'", UG_FNDE));

    $sql = "
        SELECT
            sigefid, codsigefnc
        FROM ted.previsaoorcamentaria
        WHERE
            tcpid = {$tcpid}
            AND prostatus = 'A'
            AND codsigefnc is null
            AND sigefid is not null
    ";
    $rsFNDE = $db->pegaLinha($sql);
    //$db->close();

    $vigencia = $db->carregar("select * from ted.aditivovigencia where tcpid = {$tcpid}");
    if (!$vigencia) {
        return 'Falta cadastrar uma data para o fim da vigência.';
    }

    if ($isFnde && $rsFNDE) {
        if (!is_null($rsFNDE['sigefid'])  && !empty($rsFNDE['codsigefnc'])) {
            return true;
        }
        return 'Falta enviar ou efetivar a nota de crédito junto ao SIGEF!';
    } else {
        if (permiteEnviarExecucao($tcpid)) {
            return true;
        }
        return 'Falta cadastrar nota de crédito na aba previsão orçamentária!';
    }
}

/**
 * Verifica se existe NC cadastrada para cada previsao orcamentaria, beaseado no mes de liberacao
 * @param $tpcid
 * @return bool
 */
function permiteEnviarExecucao($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $estadoAtual = pegarEstadoAtual($tcpid);

    $sqlParcelas = "
        select
            count(proid) as qtd, proanoreferencia, crdmesliberacao
        from ted.previsaoorcamentaria
        where
            tcpid = {$tcpid} and
            crdmesliberacao is not null and
            proanoreferencia = '{$_SESSION['exercicio']}' and
            prostatus = 'A'
        group by
            crdmesliberacao, proanoreferencia
    ";

    $previsoes = $db->carregar($sqlParcelas);

    if (is_array($previsoes)) {
        foreach ($previsoes as $previsao) {
            $sql = "
                select * from ted.previsaoparcela where proid in (
                    select proid from ted.previsaoorcamentaria
                    where tcpid = {$tcpid}
                    and prostatus = 'A' and proanoreferencia = '{$_SESSION['exercicio']}'
                )
            ";

            $result = $db->carregar($sql);
            if ($result) {
                $db->close();
                return true;
            }
        }
    }

    $db->close();
    return false;
}

function verificaPreenchimentoRelCumprimento($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
    if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
        return true;
    }

    if ($tcpid) {
        $retorno = uoEquipeTecnicaProponente($tcpid);
        if (is_string($retorno) && !in_array(PERFIL_COORDENADOR_SEC, $perfis)) {
            return $retorno;
        }

        $strSQL = "
          select rel.recid
          from ted.relatoriocumprimento rel
		  join ted.ncrelatoriocumprimento nc on nc.recid = rel.recid
		  where
		    rel.tcpid = {$tcpid} AND
		    rel.recstatus = 'A' AND
		    nc.rpustatus = 'A'
		";
        //ver($strSQL, d);
        $recid = $db->pegaUm($strSQL);
        $db->close();
        if($recid && in_array(UO_EQUIPE_TECNICA, $perfis)){
            return true;
        }
    }

    return 'Falta preencher o relatório de cumprimento.';
}

function verificaEquipeTecnicaConcedente($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA))) {

        /**
         * Se o termo for FNDE
         * e politica for alguma secretaria (SETEC, SECAD, SEB)
         * pega como codigo concedente o codigo da UG da secretaria
         */
        $secretarias = array(
            '150016',
            '150028',
            '150019',
        );

        $sqlComplement = "(select ungcodconcedente from ted.termocompromisso where tcpid = {$tcpid})";
        $rsSec = $db->pegaUm("select ungcodpoliticafnde from ted.termocompromisso where tcpid = {$tcpid}");
        //ver($rsSec);
        if ($rsSec) {
            if (in_array($rsSec, $secretarias)) {
                $sqlComplement = "(select ungcodpoliticafnde from ted.termocompromisso where tcpid = {$tcpid})";
            }
        }

        $strSQL = "
            select * from ted.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                rpustatus = 'A' and
                ungcod = {$sqlComplement}
        ";

        //ver($strSQL);
        $linha = $db->pegaLinha($strSQL);
        $db->close();

        return ($linha) ? true : false;
    }

    $estadoAtual = pegarEstadoAtual($tcpid);
    if (($estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD) && verificaEquipeCoordenacao($tcpid)) {
        return true;
    }

    $db->close();
    return false;
}

/**
 * Verifica se CPF possui perfil de Coordenacao
 * @param null $tcpid
 * @return bool
 */
function verificaEquipeCoordenacao($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil(array(PERFIL_COORDENADOR_SEC))) {

        $concedente = $db->pegaUm("select ungcodconcedente from ted.termocompromisso where tcpid = {$tcpid}");

        $strSQL = "
            select * from ted.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".PERFIL_COORDENADOR_SEC." and
                rpustatus = 'A'
        ";

        $linha = $db->pegaLinha($strSQL);
        $db->close();

        if ($linha) {
            return ($concedente == $linha['ungcod']) ? true : false;
        }

        return false;
    }
}


/**
 * Trava para termos que venceram a data de analise do RCO pelo coordenador
 * @return bool|string
 */
function verificaAprovacaoGestor($tcpid = null) {
    global $db;

    if (!$tcpid) {
        return false;
    }

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $objeto = new Ted_Model_RelatorioCumprimento_Business($tcpid);
    if ($objeto->termosPendenciaAprovacaoCoordenacao() && !$db->testa_superuser()) {
        $db->close();
        return 'Relatório de Cumprimento do Objeto, pendente de aprovação pelo Gestor Orçamentário do Proponente';
    } else {
        return true;
    }
}

/**
 * Função que chama a classe de commit do histórico do termo
 * E salva uma versão do termo em determinada data
 * @return bool
 */
function historico_commit($tcpid) {
    require_once APPRAIZ . 'ted/classes/Ted/Model/Historico/Commit.php';

    //$args = func_get_args();

    $model = new Ted_Model_Historico_Commit($tcpid);
    try {
        $model->save();
        return true;
    } catch (Exception $e) {
        echo $e->getMessage() . '<br>';
        echo $e->getFile() . '<br>';
        echo $e->getLine();
        die;
    }
}

/**
 * @param null $tcpid
 * @return bool
 */
function aguardando_verificacao_cgso_rco($tcpid = null) {
    global $db;

    if (!$tcpid) {
        return false;
    }

    if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
        return true;
    }

    return false;
}

/**
 * @param null $tcpid
 * @return bool
 */
function permite_tramitacao_gestor($tcpid = null) {
    if ($tcpid) {
        if (possui_perfil_gestor(array(PERFIL_CGSO, PERFIL_UG_REPASSADORA, PERFIL_SUPER_USUARIO))) {
            return true;
        }
    }
    return false;
}

/**
 * Se perfil for Diretoria da secretaria autarquia
 * O concedente for FNDE
 * E o termo se encontrar nas situações: Aguardando aprovação pela Diretoria [642]
 */
function verificaDiretoraAutarquiaFNDE() {
    $situacao = Ted_Utils_Model::pegaSituacaoTed();

    if (possui_perfil_gestor(array(PERFIL_DIRETORIA))
        && Ted_Utils_Model::concedenteFNDE()
        && ($situacao['esdid'] == 642)) {
        return true;
    }

    return false;
}

function verificaSeTermoFNDE($tcpid = null) {
    if (!$tcpid) {
        return false;
    }

    if (verificaDiretoraAutarquiaFNDE()) {
        return false;
    }

    return true;
}