<?php
/**
 * Funções de apoio à gestão da ploa.
 * $Id: _funcoesgestaoploa.php 102352 2015-09-11 14:52:35Z maykelbraz $
 */

/**
 * Funções do workflow
 * @see workflow.php
 */
include_once APPRAIZ . 'includes/workflow.php';

/**
 * Carrega informações da unidade selecionada e as armazena na sessão (unicod e unidsc).
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param array $dados Informações para processamento contendo unicod.
 */
function carregarDadosDaUnidade($dados) {
    global $db;
    $query = <<<DML
SELECT uni.unidsc,
       uni.unicod
  FROM public.unidade uni
  WHERE uni.unicod = '%d'
DML;
    $stmt = sprintf($query, $dados['unicod']);
    if (!($dadosdb = $db->pegaLinha($stmt)) || empty($dadosdb)) {
        return;
    }

    $_SESSION[$_SESSION['sisdiretorio']]['gestaoploa']['dados'] = $dadosdb;
}

function verificarPropostaOrcamentaria($exercicioatual) {
    global $db;

    $query = <<<DML
SELECT prf.prfid AS ppoid,
       prf.prftitulo AS ppodsc
  FROM proporc.periodoreferencia prf
  WHERE prf.prsano = '%s'
DML;

    $stmt = sprintf($query, $exercicioatual, $exercicioatual + 1);
    if (!$dadosdb = $db->pegaLinha($stmt)) {
        return false;
    }

    $_SESSION[$_SESSION['sisdiretorio']]['gestaoploa']['dados'] += $dadosdb;
    return true;
}

function periodoReferenciaAberto($exercicioatual, $tipo = 'p')
{
    global $db;

    switch ($tipo) {
        default:
            $sql = <<<DML
SELECT now() BETWEEN prffim AND prfinicio AS periodoaberto
  FROM proporc.periodoreferencia
  WHERE prsano = '%s'
DML;
            $stmt = sprintf($sql, $exercicioatual);
    }

    $status = $db->pegaUm($stmt);

    if ((!$status) || ('f' == $status)) {
        return false;
    }

    return true;
}

/**
 * Faz a consulta dos valores de limites para um UO em uma determinada proposta.
 * O resultado pode ser detalhado ou apenas um somatório geral.
 *
 * @global cls_banco $db Conexão com o banco de dados.
 * @param array $dados Dados para processamento e utilização na consulta.
 * @param bool $detalhar Indica se o resultado é detalhado (padrão) ou não detalhado.
 * @return array|bool
 */
function consultarLimites($dados, $retorno = 'grupo&coluna') {
    global $db;

    $extraSelect = $extraGroup = $extraOrder = $extraWhere = $extraSums = '';

    // -- Detalhando por grupo e coluna (matriz)
    switch ($retorno) {
        case 'grupo&coluna':
            $extraSelect = 'codgrupo, descgrupo, codigo, descricao, ';
            $extraGroup = '  GROUP BY codgrupo, descgrupo, gdpordem, codigo, descricao';
            $extraOrder = '  ORDER BY gpmordem, descgrupo, descricao';
            break;
        case 'consolidadogrupocoluna':
            $extraSelect = 'descgrupo, descricao, ';
            $extraSums = ', SUM(valortesouro) + SUM(valoroutros) - SUM(valorprogramado) AS saldo';
            $extraGroup = '  GROUP BY descgrupo, descricao';
            $extraWhere = ' AND gdp.gdpid = :gpmid AND dsp.dspid = :mtrid';
            break;
        case 'grupo':
            $extraSelect = 'codgrupo, descgrupo, ';
            $extraSums = ', SUM(valorprogramado) AS valorProgramado, SUM(valortesouro) + SUM(valoroutros) - SUM(valorprogramado) AS saldo';
            $extraGroup = '  GROUP BY codgrupo, descgrupo, gdpordem';
            $extraOrder = '  ORDER BY gdpordem, descgrupo';

            if ($limMatrizes = limitarMatrizes($_SESSION['usucpf'])) {
                $extraWhere .= ' AND mtr.mtrid = :mtrid_lim';
            }
            break;
        case 'coluna':
            $extraSums = ', SUM(valorprogramado) AS valorprogramado, SUM(valortesouro) + SUM(valoroutros) - SUM(valorprogramado) AS saldo';
            $extraSelect = 'codigo, descricao, codgrupo, ';
            $extraGroup = '  GROUP BY codigo, descricao, codgrupo';
            $extraOrder = '  ORDER BY descricao';
            $extraWhere = ' AND dsp.gdpid = :gpmid';

            if ($limMatrizes = limitarMatrizes($_SESSION['usucpf'])) {
                $extraWhere .= ' AND mtr.mtrid = :mtrid_lim';
            }
            break;
        case 'resumo':
            break;
        default:
            throw new Exception('Opção de configuração da query inválida.');
    }

    $query = <<<DML
 SELECT {$extraSelect}SUM(COALESCE(valortesouro, 0)) AS valortesouro,
        SUM(COALESCE(valoroutros, 0)) AS valoroutros,
        SUM(COALESCE(valortesouro, 0)) + SUM(COALESCE(valoroutros, 0)) AS valortotal{$extraSums}
  FROM (SELECT gdp.gdpid AS codGrupo, -- Recurso do tesouro
               gdp.gdpnome AS descGrupo,
               dsp.dspid AS codigo,
               dsp.dspnome AS descricao,
               gdp.gdpordem,
               SUM(COALESCE(lfu.vlrlimite, 0)) AS valorTesouro,
               0 AS valorOutros,
               0 AS valorProgramado
          FROM proporc.grupodespesa gdp
            INNER JOIN proporc.despesa dsp USING(gdpid)
            INNER JOIN proporc.limitesfonteunidadeorcamentaria lfu USING(dspid)
            INNER JOIN public.fonterecurso frs USING(foncod)
          WHERE gdp.prfid = :prfid
            AND lfu.unicod = :unicod
            AND frs.clasproporc = 'T'{$extraWhere}
          GROUP BY gdp.gdpid,
                   gdp.gdpnome,
                   dsp.dspid,
                   dsp.dspnome,
                   gdp.gdpordem
        UNION ALL
        SELECT gdp.gdpid AS codGrupo, -- Recurso próprio
               gdp.gdpnome AS descGrupo,
               dsp.dspid AS codigo,
               dsp.dspnome AS descricao,
               gdp.gdpordem,
               0 AS valorTesouro,
               SUM(COALESCE(lfu.vlrlimite, 0)) AS valorOutros,
               0 AS valorProgramado
          FROM proporc.grupodespesa gdp
            INNER JOIN proporc.despesa dsp USING(gdpid)
            INNER JOIN proporc.limitesfonteunidadeorcamentaria lfu USING(dspid)
            INNER JOIN public.fonterecurso frs USING(foncod)
          WHERE gdp.prfid = :prfid
            AND lfu.unicod = :unicod
            AND frs.clasproporc = 'P'{$extraWhere}
          GROUP BY gdp.gdpid,
                   gdp.gdpnome,
                   dsp.dspid,
                   dsp.dspnome,
                   gdp.gdpordem
        UNION ALL
        SELECT gdp.gdpid AS codGrupo, -- Valores programados
               gdp.gdpnome AS descGrupo,
               dsp.dspid AS codigo,
               dsp.dspnome AS descricao,
               gdp.gdpordem,
               0 AS valorTesouro,
               0 AS valorOutros,
               SUM(COALESCE(plf.plfvalor, 0)) AS valorProgramado
          FROM proporc.ploafinanceiro plf
            INNER JOIN proporc.despesa dsp ON (plf.mtrid = dsp.dspid)
            INNER JOIN proporc.grupodespesa gdp USING(gdpid)
            LEFT JOIN elabrev.despesaacao dpa USING(dpaid)
            INNER JOIN elabrev.ppaacao_orcamento pao USING(acaid)
          WHERE dpa.ppoid = :prfid
            AND pao.unicod = :unicod{$extraWhere}
          GROUP BY gdp.gdpid,
                   gdp.gdpnome,
                   dsp.dspid,
                   dsp.dspnome,
                   gdp.gdpordem) a
{$extraGroup}
HAVING SUM(valortesouro) > 0 OR SUM(valoroutros) > 0
{$extraOrder}
DML;
    extract($dados);

    $dml = new Simec_DB_DML($query);
    $dml->addParam('prfid', $ppoid)
            ->addParam('unicod', $unicod);
    if (isset($gpmid)) {
        $dml->addParam('gpmid', $gpmid);
    }
    if (isset($mtrid)) {
        $dml->addParam('mtrid', $mtrid);
    }
    // -- Restrição de perfil de matriz/coluna
    if ($limMatrizes) {
        $dml->addParam('mtrid_lim', $limMatrizes);
    }

    if (!in_array($retorno, array('resumo', 'consolidadogrupocoluna'))) {
        $dadosdb = $db->carregar($dml);
    } else {
        $dadosdb = $db->pegaLinha($dml);
    }

    return $dadosdb;
}

/**
 * Verifica se há necessidade de limitar as matrizes exibidas para um usuário com base no seu perfi.
 * @global cls_banco $db DBAL
 * @param string $usucpf O número do CPF do usuário para verificação de perfis.
 * @return null|array
 */
function limitarMatrizes($usucpf) {
    global $db;
    // -- Restrições de usuário responsabilidade para usuário com o perfil AD / Equipe técnica
    $perfisUsuario = pegaPerfilGeral();
    if (is_array($perfisUsuario) && in_array(PFL_AD_EQUIPE_TECNICA, $perfisUsuario)) {
        $extraWhere = "mtr.mtrid = :mtrid_rest_perfil";
        $query = <<<DML
SELECT rpu.mtrid
  FROM proporc.usuarioresponsabilidade rpu
  WHERE rpu.pflcod = :pflcod
    AND rpu.usucpf = :usucpf
    AND rpu.rpustatus = 'A'
    AND rpu.mtrid IS NOT NULL
DML;
        $dml = new Simec_DB_DML($query);
        $dml->addParam('usucpf', $usucpf)
                ->addParam('pflcod', PFL_AD_EQUIPE_TECNICA);
        $dadosdb = $db->carregar($dml);
        if ($dadosdb) {
            foreach ($dadosdb as &$matriz) {
                $matriz = $matriz['mtrid'];
            }
            return $dadosdb;
        }
    }
    return null;
}

function limitarUOs($usucpf) {
    global $db;

    $listaPerfisUsuario = pegaPerfilGeral();
    $uos = array();

    // -- Usuários com perfil PFL_AD_EQUIPE_TECNICA tem acesso à 26101
    if (in_array(PFL_AD_EQUIPE_TECNICA, $listaPerfisUsuario)) {
        $dadosuo[] = '26101';
    }
    // -- Usuário com o perfil PFL_UO_EQUIPE_TECNICA tem limitações de acesso acerca de UOs
    if (in_array(PFL_UO_EQUIPE_TECNICA, $listaPerfisUsuario)) {
        $query = <<<DML
SELECT rpu.unicod
  FROM proporc.usuarioresponsabilidade rpu
  WHERE rpu.pflcod = :pflcod
    AND rpu.usucpf = :usucpf
    AND rpu.rpustatus = 'A'
    AND rpu.unicod IS NOT NULL
DML;
        $dml = new Simec_DB_DML($query);
        $dml->addParam('pflcod', PFL_UO_EQUIPE_TECNICA)
                ->addParam('usucpf', $usucpf);

        $dadosdb = $db->carregar($dml);
        if (is_array($dadosdb)) {
            foreach ($dadosdb as $unicod) {
                $dadosuo[] = $unicod['unicod'];
            }
        }
    }
    return $dadosuo;
}

function consultarFontes($dados, $exercicio) {
    global $db;

    $query = <<<DML
SELECT codigo,
       descricao,
       SUM(COALESCE(vllimite, 0)) AS vlLimite,
       SUM(COALESCE(vldespesa, 0)) AS vlDespesa,
       SUM(COALESCE(vllimite, 0) - COALESCE (vldespesa, 0)) AS vlSaldo
  FROM (SELECT lfu.foncod AS Codigo,
               ftr.fondsc AS Descricao,
               SUM(COALESCE(lfu.vlrlimite, 0)) AS vlLimite,
               0 AS vlDespesa
          FROM proporc.limitesfonteunidadeorcamentaria lfu
            INNER JOIN public.unidade uni USING(unicod)
            INNER JOIN public.fonterecurso ftr USING(foncod)
            INNER JOIN proporc.despesa dsp USING(dspid)
            INNER JOIN proporc.grupodespesa gdp USING(gdpid)
          WHERE gdp.prfid = %d
            AND uni.unicod = '%s'
          GROUP BY lfu.foncod,
                   ftr.fondsc
        UNION ALL
        SELECT fr2.foncod,
               fr2.fondsc,
               0 AS vlLimite,
               SUM(COALESCE(da.dpavalor + da.dpavalorexpansao, 0)) AS vlDespesa
          FROM elabrev.despesaacao da
          INNER JOIN elabrev.ppaacao_orcamento ac ON ac.acaid = da.acaid
          INNER JOIN public.fonterecurso fr2 ON fr2.foncod = da.foncod
        WHERE  ac.unicod = '%s'
          AND da.ppoid = %d
        GROUP  BY fr2.foncod,
                  fr2.fondsc) AS foo
  GROUP BY codigo,
           descricao
  ORDER BY codigo
DML;

    $stmt = sprintf($query, $dados['ppoid'], $dados['unicod'], $dados['unicod'], $dados['ppoid']);

    $dadosdb = $db->carregar($stmt);
    return $dadosdb;
}

function imprimirAcoes($dados, $ppoid, $exercicio, $modoListagem = 'despesas') {
    switch ($modoListagem) {
        case 'despesas':
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            break;
        default:
            $tipoRelatorio = Simec_Listagem::RELATORIO_PAGINADO;
    }

    $list = new Simec_Listagem($tipoRelatorio);
    $callbackProgramatica = 'concatenaProgramatica';
    $acoes = array();
    switch ($modoListagem) {
        case 'despesas':
            $cabecalho = array('Programática', 'Ação', 'Status');
            $acoes = array(
                'edit' => array(
                    'func' => 'detalharDespesas',
                    'external-params' => array('gpmid' => $dados['gpmid'], 'mtrid' => $dados['mtrid'])
                )
            );
            $camposStatus = 'stsacao';
            break;
        case 'metas':
            $cabecalho = array('Programática', 'Ação', 'Status');
            $acoes = array('edit' => 'detalharAcao');
            $camposStatus = 'stsacao';
            break;
        case 'tramitacao':
            $cabecalho = array('Programática', 'Ação', 'Status' => array('Financeiro', 'Físico', 'Fonte'), 'Situação', 'Retorno SIOP');
            $camposStatus = array('stsfinanceiro', 'stsfisico');
            $callbackProgramatica = 'concatenaProgramaticaCompleta';
            $list->esconderColunas('docid');
            $list->addCallbackDeCampo('retsiop', 'formatarResultadoEnvio')
                ->addCallbackDeCampo('statusfonte', 'formatarStatusFonte')
                ->addCallbackDeCampo('acaid_2', 'formatarAcaidComoCheckbox');
            $list->turnOnPesquisator();
            $list->addRegraDeLinha(
                array('campo' => 'acasituacao', 'op' => 'contem', 'valor' => 'acertos', 'classe' => 'tr_erro')
            );
            $acoes = array('workflow' => array('func' => 'drawWorkflow', 'extra-params' => array('docid')));
            // -- Exibição das ações de tramitação
            $listaPerfis = pegaPerfilGeral();
            if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $listaPerfis) || in_array(PFL_ADMINISTRADOR, $listaPerfis)) {
                array_unshift($cabecalho, 'Enviar SIOP');
            } else {
                $list->esconderColunas('acaid_2');
            }
            // -- Ação de workflow é condicional se o perfil for UO_EQUIPE_TECNICA
            if (in_array(PFL_UO_EQUIPE_TECNICA, $listaPerfis)) {
                $list->setAcaoComoCondicional(
                    'workflow',
                    array(
                        array('campo' => 'stsfinanceiro', 'valor' => 'A', 'op' => 'igual'),
                        array('campo' => 'stsfisico', 'valor' => 'A', 'op' => 'igual'),
                        array('campo' => 'statusfonte', 'valor' => 'O', 'op' => 'igual')
                    )
                );
            }
            break;
    }

    $list->setCabecalho($cabecalho)
        ->setAcoes($acoes)
        ->setDados(consultarAcoes($dados, $ppoid, $exercicio, $modoListagem))
        ->addCallbackDeCampo('prgcod', $callbackProgramatica)
        ->addCallbackDeCampo('acacod', 'concatenaAcao')
        ->addCallbackDeCampo($camposStatus, 'cbStatusDespesa')
        ->esconderColunas(array('prgid', 'loccod', 'locdsc', 'sacdsc', 'unicod', 'acadsc', 'sfucod', 'funcod', 'esfcod'))
        ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

function imprimirColunasDoGrupo($dados, $ppoid, $unicod) {
    $dadosconsulta = array(
        'gpmid' => current($dados),
        'ppoid' => $ppoid,
        'unicod' => $unicod
    );

    $dadosdb = consultarLimites($dadosconsulta, 'coluna');
    if(is_array($dadosdb)){
        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $list->setDados($dadosdb)
                ->setCabecalho(array('Coluna', 'Limites (R$)' => array('Tesouro', 'Próprios', 'Total'), 'Programado (R$)', 'Saldo (R$)'))
                ->addAcao('plus', array('func' => 'detalharColuna', 'extra-params' => array('codgrupo')))
                ->setTotalizador(
                        Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valortesouro', 'valoroutros', 'valortotal', 'valorprogramado', 'saldo')
                )->addCallbackDeCampo(array('valortesouro', 'valoroutros', 'valortotal', 'valorprogramado', 'saldo'), 'mascaraNumero')
                ->addCallbackDeCampo('descricao', 'alinharEsquerda')
                ->esconderColunas('codgrupo');
        $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }else{
        echo <<<HTML
            <section class="alert alert-danger text-center col-md-6 col-md-offset-3" style="margin-top:0;margin-bottom:0;">Dados não encontrados.</section>
HTML;
    }

}

function consultarAcoes($dados, $ppoid, $exercicio, $modoListagem = 'despesas') {
    global $db;

    // -- Complemento do select
    $compSelect = $compWhere = '';

    /* Filtro por UO */
    $filtroUnidade = '';
    if (isset($dados['unicod']) && $dados['unicod']) {
        $filtroUnidade = ' AND u.unicod = :unicod ';
    }

    switch ($modoListagem) {
        case 'despesas':
            // -- Subselect repetido em 'tramitacao'
            $compSelect = <<<DML
                COALESCE((SELECT 'A'::text
                            WHERE EXISTS (SELECT 1
                                            FROM proporc.ploafinanceiro plf
                                              INNER JOIN elabrev.despesaacao ac USING(dpaid)
                                            WHERE ac.acaid = a.acaid
                                              AND ac.ppoid = :ppoid
                                              AND plf.mtrid = :mtrid)), 'S') AS stsacao
DML;
            $compWhere = <<<DML
                AND EXISTS (SELECT 1
                              FROM proporc.despesaacao dsa
                              WHERE dsa.acacod = a.acacod
                                AND dsa.dspid = :mtrid)
DML;
            break;
        case 'metas':
            // -- campo repetido em 'tramitacao'
            $compSelect = <<<DML
                COALESCE(a.acaalteracao, 'S') AS stsacao
DML;
            break;
        case 'tramitacao':
            // -- Informações de limites de fontes para controlar envio de ações
            // -- que tem sua despesa composta por recursos de uma fonte que, atualmente, excede seu limite.
            $fontesComLimiteExcedido = array();

            if ($retornoFontes = consultarFontes($dados, $_SESSION['exercicio'])) {
                foreach ($retornoFontes as $fonte) {
                    if ((double) $fonte['vldespesa'] > (double) $fonte['vllimite']) {
                        $fontesComLimiteExcedido[] = $fonte['codigo'];
                    }
                }
                unset($retornoFontes);
            }
            $dados['foncod'] = $fontesComLimiteExcedido;

            $compSelectAcaid_2 = <<<DML
                a.acaid AS acaid_2,
DML;
            $compSelect = <<<DML
                a.esfcod,
                a.sfucod,
                a.funcod,
                COALESCE((SELECT 'A'::text
                            WHERE EXISTS (SELECT 1
                                            FROM proporc.ploafinanceiro plf
                                              INNER JOIN elabrev.despesaacao ac USING(dpaid)
                                            WHERE ac.acaid = a.acaid
                                              AND ac.ppoid = :ppoid)), 'S') AS stsfinanceiro,
                COALESCE(a.acaalteracao, 'S') AS stsfisico,
                COALESCE((SELECT 'P'::text
                            WHERE EXISTS (SELECT 1
                                            FROM elabrev.despesaacao dpa
                                            WHERE dpa.acaid = a.acaid
                                              AND dpa.foncod = :foncod)), 'O') AS statusfonte,
                COALESCE((SELECT esd.esddsc
                            FROM workflow.documento doc
                              LEFT JOIN workflow.estadodocumento esd USING(esdid)
                            WHERE doc.docid = a.docid), 'Em preenchimento') AS acasituacao,
                COALESCE(a.acastatusultimoenvio, '-') AS retsiop,
                a.docid
DML;
            if ($dados['filtros']['fonte'] == 'P') {
                $compWhere .= " AND EXISTS (SELECT 1
                                            FROM elabrev.despesaacao dpa
                                            WHERE dpa.acaid = a.acaid
                                              AND dpa.foncod = :foncod)";
            } elseif ($dados['filtros']['fonte'] == 'O') {
                $compWhere .= " AND NOT EXISTS (SELECT 1
                                            FROM elabrev.despesaacao dpa
                                            WHERE dpa.acaid = a.acaid
                                              AND dpa.foncod = :foncod)";
            }

            if ($dados['filtros']['retornosiop'] == 'ERRO') {
                $compWhere .= " AND a.acastatusultimoenvio = 'E' ";
            } elseif ($dados['filtros']['retornosiop'] == 'OK') {
                $compWhere .= " AND a.acastatusultimoenvio = 'S'";
            }

            break;
    }

    /* Filtros da Consulta */
    if (isset($dados['filtros'])) {

        /* Filtro por ACACOD */
        if (isset($dados['filtros']['acacod']) && $dados['filtros']['acacod'] <> '') {
            $compWhere .= " AND  a.acacod = '{$dados['filtros']['acacod']}'";
        }

        /* Filtro por ACACOD */
        if (isset($dados['filtros']['esdid']) && $dados['filtros']['esdid'] <> '') {
            $compWhere .= " AND  doc.esdid = {$dados['filtros']['esdid']}";
        }
    }


    $query = <<<DML
SELECT DISTINCT a.acaid,
{$compSelectAcaid_2}
                a.prgid,
                a.prgcod,
                a.loccod,
                a.sacdsc as locdsc,
                a.acacod,
                a.unicod,
                a.acadsc,
                a.sacdsc,
{$compSelect}
  FROM elabrev.ppaacao_orcamento a
    INNER JOIN unidade u ON a.unicod = u.unicod
/* Retirei para aparecer as unidades do FNDE (74902 / etc)
    INNER JOIN unidade unijoin ON (unijoin.unicod != '26100' AND unijoin.unicod != '26000'
                                                             AND unijoin.orgcod = '26000'
                                                             AND a.unicod = unijoin.unicod)
*/
    LEFT JOIN workflow.documento doc using (docid)
  WHERE a.prgano = :prgano
    AND a.acastatus = 'A'
    {$filtroUnidade}
    AND a.acasnrap = 'f'
{$compWhere}
  ORDER BY a.acacod,
           a.unicod,
           a.loccod
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParams($dados)
        ->addParam('ppoid', $ppoid)
        ->addParam('prgano', $exercicio, true)
        ->forceParamAsString(array('unicod', 'foncod'));
    $result = $db->carregar($dml);
    if (!$result && 'despesas' == $modoListagem) {
        $query = str_replace($compWhere, '', $query);
        $dml->setString($query)
            ->addParams($dados)
            ->addParam('ppoid', $ppoid)
            ->addParam('prgano', $exercicio)
            ->forceParamAsString('unicod');
        return $db->carregar($dml);
    }

    return $result ? $result : array();
}

function carregarDadosAcao($dados, $exercicio, $carregarDadosColuna = true) {
    global $db;
    $query = <<<DML
SELECT pao.acacod,
       pao.acadsc,
       pao.prgcod,
       pro.prgdsc,
       pao.loccod,
       pao.sacdsc as locdsc,
       COALESCE(pao.proddesc, '-') AS proddesc,
       COALESCE(CASE WHEN '' = pao.acadscunmsof THEN '-' ELSE pao.acadscunmsof END, '-') AS acadscunmsof,
       CASE WHEN '' = pao.acadscprosof THEN '-' ELSE pao.acadscprosof END AS acadscprosof,
       pao.acaqtdefisico,
       SUM(dpa.dpavalor) AS acaqtdefinanceiro,
       pao.justificativa,
       doc.esdid,
       COALESCE(esd.esddsc, 'Em preenchimento') AS esddsc,
       pao.memcalculo
  FROM elabrev.ppaacao_orcamento pao
    INNER JOIN elabrev.ppaprograma_orcamento pro USING(prgcod, prgano, prgid)
    LEFT JOIN elabrev.despesaacao dpa USING(acaid)
    LEFT JOIN workflow.documento doc USING(docid)
    LEFT JOIN workflow.estadodocumento esd USING(esdid)
  WHERE pao.prgano = '%s'
    AND acaid = %d
  GROUP BY pao.acacod,
           pao.acadsc,
           pao.prgcod,
           pro.prgdsc,
           pao.loccod,
           pao.sacdsc,
           pao.proddesc,
           pao.acadscunmsof,
           pao.acadscprosof,
           pao.acaqtdefisico,
           pao.justificativa,
           doc.esdid,
           esd.esddsc,
           pao.memcalculo
DML;
    $stmt = sprintf($query, $exercicio, $dados['acaid']);
    if (!$dadosaca = $db->pegaLinha($stmt)) {
        return array('sucesso' => false, 'mensagem' => 'Não foi possível encontrar dados da ação solicitada.');
    }

    if ($carregarDadosColuna) {
        $dadosdb = consultarLimites($dados, 'consolidadogrupocoluna');

        $_SESSION[MODULO]['gestaoploa']['dados']['mtrid'] = $dados['mtrid'];
        $_SESSION[MODULO]['gestaoploa']['dados']['gpmid'] = $dados['gpmid'];
        $_SESSION[MODULO]['gestaoploa']['dados']['gpmdsc'] = $dadosdb['descgrupo'];
        $_SESSION[MODULO]['gestaoploa']['dados']['mtrdsc'] = $dadosdb['descricao'];
        $_SESSION[MODULO]['gestaoploa']['dados']['mtmvlrlimite'] = $dadosdb['valortotal'];
        $_SESSION[MODULO]['gestaoploa']['dados']['saldocoluna'] = $dadosdb['saldo'];
    }

    $_SESSION[MODULO]['gestaoploa']['dados']['acao'] = $dadosaca;
    $_SESSION[MODULO]['gestaoploa']['dados']['acaid'] = $dados['acaid'];

    return array('sucesso' => true);
}

function inputComboPOPLOA($dados, $id = '')
{
    global $db;

    $query = <<<DML
SELECT COUNT(1) AS qtd
  FROM elabrev.ppaacao_orcamento pao
    INNER JOIN proporc.despesaplanoorcamentario dpo
      ON (pao.acacod = dpo.acacod
          AND pao.unicod = dpo.unicod
          AND pao.prgcod = dpo.prgcod
          AND pao.loccod = dpo.loccod)
  WHERE pao.acaid = %d
    AND dpo.dspid = %d
DML;
    $stmt = sprintf($query, $dados['acaid'], $dados['mtrid']);
    $qtdPONaRegra = $db->pegaLinha($stmt);
    $qtdPONaRegra = (int)$qtdPONaRegra['qtd'];
    $whereAdicional = '';

    $dadosQuery[] = $dados['acaid'];

    if ($qtdPONaRegra > 0) {
        $whereAdicional = <<<DML
    AND EXISTS (SELECT 1
                  FROM elabrev.ppaacao_orcamento pao
                    INNER JOIN proporc.despesaplanoorcamentario dpo
                      ON (pao.acacod = dpo.acacod
                          AND pao.unicod = dpo.unicod
                          AND pao.prgcod = dpo.prgcod
                          AND pao.loccod = dpo.loccod
                          AND dpo.plocod = plo.plocodigo)
                  WHERE plo.acaid = pao.acaid
                    AND dpo.dspid = %d)
DML;
        $dadosQuery[] = $dados['mtrid'];
    }

    $query = <<<DML
SELECT ploid AS codigo,
       plocodigo || ' - ' || TRIM(plotitulo) AS descricao
  FROM elabrev.planoorcamentario plo
  WHERE acaid = %d
    AND plostatus = 'A'
{$whereAdicional}
  ORDER BY plocodigo
DML;

    $stmt = vsprintf($query, $dadosQuery);
    inputCombo('dados[ploid]', $stmt, null, $id ? $id : 'ploid');
}

function inputComboSubacaoPLOA($dados, $exercicio) {
    global $db;
    $query = <<<DML
SELECT COUNT(1) AS qtd
  FROM proporc.despesasubacao dsb
    INNER JOIN elabrev.subacaoprogramacao sbp USING(sbaid)
  WHERE dsb.dspid = %d
    AND sbp.exercicio = '%d'
    AND sbp.acacod = '%s'
DML;
    $stmt = sprintf($query, $dados['mtrid'], $exercicio, $dados['acao']['acacod']);
    $qtdSubacaoNaRegra = $db->pegaUm($stmt);

    $paramsQuery = array(
        'exercicio' => $exercicio,
        'acacod' => $dados['acao']['acacod'],
        'unicod' => $dados['unicod']
    );
    $whereAdicional = '';
    if ($qtdSubacaoNaRegra > 0) {
        $whereAdicional = <<<DML
    AND EXISTS (SELECT 1
                  FROM proporc.despesasubacao dsb
                  WHERE dsb.sbaid = sba.sbaid
                    AND dsb.dspid = %d)
DML;
        $paramsQuery['mtrid'] = $dados['mtrid'];
    }

    $query = <<<DML
SELECT DISTINCT sba.sbaid AS codigo,
       sba.sbacod || ' - ' || sba.sbatitulo AS descricao
  FROM elabrev.subacao sba
    INNER JOIN elabrev.subacaoprogramacao sa ON sa.sbaid = sba.sbaid
  WHERE sba.sbastatus = 'A'
    AND sa.exercicio = '%d'
--    AND sa.acacod = '%s'
    AND sba.unicod = '%s'
{$whereAdicional}
  ORDER BY
        sba.sbacod || ' - ' || sba.sbatitulo,
        sba.sbaid
DML;
    $stmt = vsprintf($query,$paramsQuery);
    inputCombo('dados[sbaid]', $stmt, null, 'sbaid');
}

function inputComboFonteRecursoPLOA($dados) {
    $query = <<<DML
SELECT f.foncod AS codigo,
       f.foncod || ' - ' || f.fondsc AS descricao
  FROM proporc.despesafonterecurso dsf
    INNER JOIN public.fonterecurso f USING(foncod)
  WHERE f.fonstatus = 'A'
    AND EXISTS (SELECT 1
                  FROM proporc.limitesfonteunidadeorcamentaria lfu
                  WHERE lfu.unicod = :unicod)
    AND dsf.dspid = :mtrid
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParam('unicod', $dados['unicod'])
        ->addParam('mtrid', $dados['mtrid']);
    inputCombo('dados[foncod]', $dml, null, 'foncod');
}

function inputComboNaturezaPLOA($dados, $exercicio) {
    $query = <<<DML
SELECT ndp.ndpid AS codigo,
       ndp.ndpcod || ' - ' || ndpdsc AS descricao, *
  FROM public.naturezadespesa ndp
  WHERE ndp.ndpstatus = 'A'
    AND EXISTS (SELECT 1
                  FROM proporc.despesagnd dsg
                  WHERE dsg.dspid = :mtrid
                    AND ndp.ndpcod LIKE '_' || dsg.gndcod || '%')
    AND ndp.ndpano::integer = :ndpano
  ORDER BY ndpcod
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParam('mtrid', $dados['mtrid'])
        ->addParam('ndpano', $exercicio);
    inputCombo('dados[ndpid]', $dml, null, 'ndpid');
}

/**
 * Cria uma nova entrada do financeiro (e acrescenta seu docid) ou atualiza uma entrada existente. Depois de
 * alterar os valores relacionados ao novo módulo de proposta (em ploafinanceiro) faz uma atualização do
 * valor total no modulo elabrev.
 *
 * @global cls_banco $db DBAL
 * @param array $dados Dados para criação ou alteração da entrada financeiro.
 * @param string $usucpf CPF do usuário que está criando a nova entrada.
 * @param array $dadossessao Dados da sessão para consultar e atualizar o saldo disponível atualmente para a coluna.
 * @return array
 */
function alterarFinanceiro($dados, $usucpf, $dadossessao) {
    global $db;

    $plfvalor = removeMascaraMoeda($dados['plfvalor']);

    $update = <<<DML
UPDATE proporc.ploafinanceiro
  SET plfvalor = %f
  WHERE dpaid = %d
    AND mtrid = %d
  RETURNING plfid
DML;
    // -- Tentando atualizar o registro financeiro
    $stmt = sprintf($update, $plfvalor, $dados['dpaid'], $dados['mtrid']);
    if (!$db->carregar($stmt)) {
        // -- Inserindo uma nova ocorrência
        $insert = <<<DML
INSERT INTO proporc.ploafinanceiro(dpaid, mtrid, usucpf, plfvalor)
  VALUES(%d, %d, '%s', %f)
DML;
        $stmt = sprintf($insert, $dados['dpaid'], $dados['mtrid'], $usucpf, $plfvalor);
        if (!$db->executar($stmt)) {
            $db->rollback();
            return array('sucesso' => false, 'mensagem' => 'Não foi possível inserir a nova despesa.');
        }
    }

    // -- Atualizando ungcod e sbaid
    if (isset($dados['sbaid'])) {
        $sql = <<<DML
UPDATE elabrev.despesaacao
  SET ungcod = '%s',
      sbaid = %d
  WHERE dpaid = %d
DML;
        $stmt = sprintf($sql, $dados['ungcod'], $dados['sbaid'], $dados['dpaid']);
        $db->executar($stmt);
    }

    // -- Atualizando a entrada em despesa ação de acordo com todos os financeiros que referenciam a ação
    $result = updateSomatorioDespesaAcao($dados['dpaid']);
    if ($result['sucesso']) {
        $acaid = $result['acaid'];
    } else {
        $db->rollback();
        return $result;
    }

    if (!$db->commit()) {
        $db->rollback();
        return array('sucesso' => false, 'mensagem' => 'Não foi possível executar sua solicitação.');
    }

    // -- Update docid se estiver nulo
    $result = criarDocid($acaid);
    if (!$result['sucesso']) {
        return $result;
    }

    // -- Update nos dados da sessão
    $dadosdb = consultarLimites($dadossessao, 'consolidadogrupocoluna');
    $_SESSION[MODULO]['gestaoploa']['dados']['saldocoluna'] = $dadosdb['saldo'];

    return array('sucesso' => true);
}

function novoFinanceiro($dados, $usucpf, $dadossessao) {
    global $db;

    $extraFields = $extraParams = '';
    if (isset($dados['sbaid'])) {
        $extraFields .= ', sbaid';
        $extraParams .= ', :sbaid';
    }
    if (isset($dados['ungcod'])) {
        $extraFields .= ', ungcod';
        $extraParams .= ', :ungcod';
    }

    $insert = <<<DML
INSERT INTO elabrev.despesaacao(acaid, ppoid, iducod, foncod, idoid, dpavalor, tpdid, ploid, ndpid{$extraFields})
  VALUES(:acaid, :ppoid, :iducod, :foncod, :idoid, :dpavalor, :tpdid, :ploid, :ndpid{$extraParams})
  RETURNING dpaid
DML;

    // -- Extraíndo valores de $dados
    extract($dados);

    $dml = new Simec_DB_DML($insert);
    $dml->addParam('acaid', $dadossessao['acaid'])
            ->addParam('ppoid', $dadossessao['ppoid'])
            ->addParam('iducod', 0)
            ->addParam('foncod', $foncod)
            ->addParam('idoid', 1) // -- idoc = 9999
            ->addParam('dpavalor', trim(str_replace(array('.', ','), array('', '.'), $plfvalor)))
            ->addParam('tpdid', consultarTipoDetalhamento($dadossessao['acaid'], $ndpcod{1}))
            ->addParam('ploid', $ploid)
            ->addParam('ndpid', $ndpid);
    if (isset($sbaid)) {
        $dml->addParam('sbaid', $sbaid);
    }
    if (isset($ungcod)) {
        $dml->addParam('ungcod', $ungcod);
    }

    if (!$dpaid = $db->pegaUm($dml)) {
        $db->rollback();
        return array('sucesso' => false, 'mensagem' => 'Não foi possível criar uma nova despesa para a ação.');
    }

    $dados['dpaid'] = $dpaid;

    return alterarFinanceiro($dados, $usucpf, $dadossessao);
}

function consultarTipoDetalhamento($acaid, $gnd) {
    global $db;

    $query = <<<DML
SELECT t.tpdid,
       t.tpdcod,
       ta.acaid
  FROM elabrev.tipodetalhamentoacao ta
    INNER JOIN elabrev.tipodetalhamento t ON (t.tpdid = ta.tpdid)
  WHERE ta.acaid = :acaid
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParam('acaid', $acaid);

    if (!$dadosdb = $db->carregar($dml)) {
        return null;
    }

    $temTpd5 = false;
    foreach ($dadosdb as $linha) {
        if (5 == $linha['tpdid']) {
            $temTpd5 = true;
        }
    }

    if (1 == count($dadosdb) && !$temTpd5) {
        return $dadosdb[0]['tpdid'];
    }

    if (1 == $gnd) {
        return 5;
    } else {
        foreach ($dadosdb as $linha) {
            if (5 != $linha['tpdid']) {
                return $linha['tpdid'];
            }
        }
    }
    return null;
}

function detalharFinanceiro($dados, $comoHTML = false) {
    global $db;

    $query = <<<DML
SELECT gdp.gdpnome,
       dsp.dspnome,
       plf.plfvalor,
       usu.usunome,
       TO_CHAR(plfinclusao, 'DD/MM/YYYY') AS plfinclusao
  FROM proporc.ploafinanceiro plf
    INNER JOIN proporc.despesa dsp ON (dsp.dspid = plf.mtrid)
    INNER JOIN proporc.grupodespesa gdp USING(gdpid)
    INNER JOIN seguranca.usuario usu USING(usucpf)
  WHERE plf.dpaid = %d
DML;
    $stmt = sprintf($query, $dados['dpaid']);

    if (!$comoHTML) {
        return $db->carregar($stmt);
    }

    $list = new Simec_Listagem();
    $list->setQuery($stmt)
            ->setCabecalho(array('Grupo', 'Coluna', 'Valor&nbsp;(R$)', 'Responsável', 'Quando'))
            ->addCallbackDeCampo('plfvalor', 'mascaraNumero')
            ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, 'plfvalor');
    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

function salvarFisico($dados) {
    global $db;

    $dados['memcalculo'] = str_replace(array("\\", "'"), "", $dados['memcalculo']);

    $update = <<<DML
UPDATE elabrev.ppaacao_orcamento
  SET acaqtdefisico = :acaqtdefisico,
      memcalculo = :memcalculo
  WHERE acaid = :acaid
DML;

    if ('' == $dados['acaqtdefisico']) {
        $dados['acaqtdefisico'] = null;
    }

    $dml = new Simec_DB_DML($update);
    $dml->addParams($dados);

    $db->executar($dml);

    // -- Atualizando status de alteração da ação conforme os campos preenchidos
    $resultStatus = atualizarStatusAcao($dados);
    if (!$resultStatus) {
        $db->rollback();
        return $resultStatus;
    }

    if (!$db->commit()) {
        $db->rollback();
        return array('sucesso' => false, 'mensagem' => 'Não foi possível atualizar o valor físico.');
    }

    // -- Update docid se estiver nulo
    $result = criarDocid($dados['acaid']);
    if (!$result['sucesso']) {
        return $result;
    }

    $_SESSION[MODULO]['gestaoploa']['dados']['acao']['acaqtdefisico'] = $dados['acaqtdefisico'];
    $_SESSION[MODULO]['gestaoploa']['dados']['acao']['memcalculo'] = $dados['memcalculo'];
    return array('sucesso' => true, 'mensagem' => 'Informações do físico atualizadas com sucesso.');
}

/**
 * Salva a justificativa da proposta orçamentária de uma ação. Incluí no final da justificativa,
 * um identificador do usuário que preencheu a proposta.
 *
 * @global cls_banco $db DBAL
 * @param array $dados Identificador da ação e o texto da justificativa.
 * @param string $usucpf Num. do CPF do usuário que preencheu a justificativa.
 * @param string $usunome Nom do usuário que preencheu a justificativa.
 * @return array
 */
function salvarJustificativa($dados, $usucpf, $usunome) {
    global $db;
    $update = <<<DML
UPDATE elabrev.ppaacao_orcamento
  SET justificativa = :justificativa
  WHERE acaid = :acaid
DML;

    $dados['justificativa'] = str_replace(array("\\", "'"), "", $dados['justificativa']);
    $dados['justificativa'] = substr($dados['justificativa'], 0, 2900);

    // -- Adicionando a identificação do usuário que preencheu a justificativa
    $padraoUsuario = "/\|$/";
    if (!preg_match($padraoUsuario, $dados['justificativa'])) {
        $dados['justificativa'] .= "\n|{$usucpf} - {$usunome}|";
    }

    $dml = new Simec_DB_DML($update);
    $dml->addParams($dados);
    $db->executar($dml);

    // -- Atualizando status de alteração da ação conforme os campos preenchidos
    $resultStatus = atualizarStatusAcao($dados);
    if (!$resultStatus) {
        $db->rollback();
        return $resultStatus;
    }

    if (!$db->commit()) {
        $db->rollback();
        return array('sucesso' => false, 'mensagem' => 'Não foi possível atualizar a justificativa.');
    }

    // -- Update docid se estiver nulo
    $result = criarDocid($dados['acaid']);
    if (!$result['sucesso']) {
        return $result;
    }

    $_SESSION[MODULO]['gestaoploa']['dados']['acao']['justificativa'] = $dados['justificativa'];
    return array('sucesso' => true, 'mensagem' => 'Justificativa atualizada com sucesso.');
}

function salvarMeta($dados) {
    global $db;
    $dados['memcalculo'] = str_replace(array("\\", "'"), "", $dados['memcalculo']);
    if ('' == $dados['mpovalor']) {
        $dados['mpovalor'] = null;
    }
    $update = <<<DML
UPDATE elabrev.metaplanoorcamentario
  SET mpovalor = :mpovalor,
      memcalculo = :memcalculo
  WHERE ploid = :ploid
  RETURNING mpoid
DML;
    $dml = new Simec_DB_DML($update);
    $dml->addParams($dados);
    if (!$mpoid = $db->pegaUm($dml)) {
        $insert = <<<DML
INSERT INTO elabrev.metaplanoorcamentario(ploid, mpovalor, memcalculo, mpostatus)
  VALUES(:ploid, :mpovalor, :memcalculo, 'A')
  RETURNING mpoid
DML;

        $dml->setString($insert);
        $dml->addParams($dados);
        if (!$db->pegaUm($dml)) {
            $db->rollback();
            return array('sucesso' => false, 'mensagem' => 'Não foi possível atualizar a meta do PO.');
        }
    }

    $resultStatus = atualizarStatusAcao($dados);
    if (!$resultStatus) {
        $db->rollback();
        return $resultStatus;
    }

    $db->commit();

    // -- Update docid se estiver nulo
    $result = criarDocid($dados['acaid']);
    if (!$result['sucesso']) {
        return $result;
    }

    return array('sucesso' => true, 'mensagem' => 'A meta do PO foi atualizada com sucesso.');
}

function atualizarStatusAcao($dados, $commit = false) {
    global $db;

    $dml = new Simec_DB_DML();

    if (isset($dados['ploid']) && !isset($dados['acaid'])) {
        $query = <<<DML
SELECT plo.acaid
  FROM elabrev.planoorcamentario plo
  WHERE plo.ploid = :ploid
DML;
        $dml->setString($query);
        $dml->addParam('ploid', $dados['ploid']);
        if (!$acaid = $db->pegaUm($dml)) {
            return array('sucesso' => false, 'mensagem' => 'Não foi possível encontrar a ação associada a este PO.');
        }
    } else {
        $acaid = $dados['acaid'];
    }

    $query = <<<DML
SELECT pao.acaid,
       pao.justificativa,
       pao.acaqtdefisico,
       pao.acadscprosof,
       -- POs sem produto não devem ser considerados para fins de bloqueio de tramitação,
       -- sendo assim, contamos apenas os POs que apresentam um produto (1 e 2) e que foram preenchidos (2)
       -- Edit1: POs sem meta física que não apresentam detalhamento financeiro não devem ser considerados no bloqueio de tramitação (3)
       COUNT(CASE WHEN '' != ploproduto -- (1)
                    AND EXISTS (SELECT 1 -- (3)
                                  FROM elabrev.despesaacao dpa
                                  WHERE dpa.ploid = plo.ploid
                                    AND dpa.acaid = pao.acaid
                                  HAVING SUM(dpa.dpavalor) > 0) THEN 1 ELSE NULL END) AS qtdplo,
       COUNT(CASE WHEN '' != ploproduto AND mpoid IS NOT NULL THEN 1 ELSE NULL END) AS qtdmetas -- (2)
  FROM elabrev.ppaacao_orcamento pao
    LEFT JOIN elabrev.planoorcamentario plo USING(acaid)
    LEFT JOIN elabrev.metaplanoorcamentario mpo ON(plo.ploid = mpo.ploid AND mpostatus = 'A')
  WHERE pao.acaid = :acaid
  GROUP BY pao.acaid, pao.justificativa, pao.acaqtdefisico, pao.acadscprosof
DML;

    $dml->setString($query);
    $dml->addParam('acaid', $acaid);

    if (!$dadosdb = $db->pegaLinha($dml)) {
        $db->rollback();
        return array('sucesso' => false, 'mensagem' => 'Não foi possível encontrar os dados da ação para atualização de status.');
    }

//    $acaalteracao = 'I';
//    if (
//        !empty($dadosdb['justificativa']) // -- Justificativa não pode ser vazia
//        && ((empty($dadosdb['acaqtdefisico']) && empty($dadosdb['acadscprosof'])) // -- A meta física tem que ser vazia, se não existir produto
//             || (!empty($dadosdb['acaqtdefisico']) && !empty($dadosdb['acadscprosof']))) // -- Meta física não pode ser vazia se existir produto
//        && $dadosdb['qtdplo'] == $dadosdb['qtdmetas'] // -- O preenchimento da meta dos POs deve atender a regra na query acima
//    ) {
        $acaalteracao = 'A';
//    }

    $update = <<<DML
UPDATE elabrev.ppaacao_orcamento
  SET acaalteracao = :acaalteracao
  WHERE acaid = :acaid
  RETURNING acaid
DML;
    $dml->setString($update)
            ->addParam('acaid', $acaid)
            ->addParam('acaalteracao', $acaalteracao);

    if (!$db->pegaUm($dml)) {
        $db->rollback();
        return array('sucesso' => false, 'mensagem' => 'Não foi possível atualizar o status da ação.');
    }

    if ($commit) {
        $db->commit();
    }
    return array('sucesso' => true, 'mensagem' => 'O status da ação foi atualizado com sucesso.');
}

function criarDocid($acaid) {
    global $db;

    $sql = <<<DML
SELECT docid
  FROM elabrev.ppaacao_orcamento pao
  WHERE pao.acaid = :acaid
DML;
    $dml = new Simec_DB_DML($sql);
    $dml->addParam('acaid', $acaid);

    $docid = $db->pegaUm($dml);
    if (!$docid) {
        $docid = wf_cadastrarDocumento(
                TPDOC_PROPOSTA_ORCAMENTARIA, "Proposta orçamentária acaid: {$acaid}"
        );

        $sql = <<<DML
UPDATE elabrev.ppaacao_orcamento
  SET docid = :docid
  WHERE acaid = :acaid
DML;
        $dml->setString($sql);
        $dml->addParam('docid', $docid)
                ->addParam('acaid', $acaid);

        if (!$db->executar($dml)) {
            return array(
                'sucesso' => false,
                'mensagem' => 'Não foi possível cadastrar um documento para esta proposta.'
            );
        }
        $db->commit();
    }
    return array('sucesso' => true);
}

function excluirFinanceiro($dados, $dadossessao) {
    global $db;

    // -- Apagando o registro do banco de dados do proporc
    $sql = <<<DML
DELETE FROM proporc.ploafinanceiro
  WHERE dpaid = :dpaid
    AND mtrid = :mtrid
DML;
    $dml = new Simec_DB_DML($sql);
    $dml->addParam('dpaid', $dados['dpaid'])
            ->addParam('mtrid', $dados['mtrid']);
    $db->executar($dml);

    $sql = <<<DML
SELECT COUNT(1) AS qtd
  FROM proporc.ploafinanceiro
  WHERE dpaid = :dpaid
DML;
    $dml->setString($sql)
        ->addParam('dpaid', $dados['dpaid']);

    if (0 == $db->pegaUm($dml)) { // -- Apaga a despesa ação
        $sql = <<<DML
DELETE FROM elabrev.despesaacao
  WHERE dpaid = :dpaid
DML;
        $dml->setString($sql)
                ->addParam('dpaid', $dados['dpaid']);
        $db->executar($dml);
    } else { // -- Atualiza o total da despesa ação
        $result = updateSomatorioDespesaAcao($dados['dpaid']);
        if (!$result['sucesso']) {
            $db->rollback();
            return $result;
        }
    }

    if (!$db->commit()) {
        return array('sucesso' => true, 'mensagem' => 'Não foi possível apagar o registro financeiro solicitado.');
    }

    // -- Atualizar o valor do saldo na sessão
    $dadosdb = consultarLimites($dadossessao, 'consolidadogrupocoluna');
    $_SESSION[MODULO]['gestaoploa']['dados']['saldocoluna'] = $dadosdb['saldo'];

    return array('sucesso' => true, 'mensagem' => 'Sua solicitação foi executada com sucesso.');
}

function updateSomatorioDespesaAcao($dpaid) {
    global $db;

    // -- Atualizando a entrada em despesa ação de acordo com todos os financeiros que referenciam a ação
    $dml = <<<DML
UPDATE elabrev.despesaacao
  SET dpavalor = (SELECT SUM(plfvalor)
                    FROM proporc.ploafinanceiro plf
                    WHERE plf.dpaid = :dpaid)
  WHERE dpaid = :dpaid
  RETURNING acaid
DML;
    $dml = new Simec_DB_DML($dml);
    $dml->addParam('dpaid', $dpaid);

    if (!$acaid = $db->pegaUm($dml)) {
        $db->rollback();
        return array('sucesso' => false, 'mensagem' => 'Não foi possível atualizar o somatório de despesas da ação.');
    }

    return array('sucesso' => true, 'acaid' => $acaid);
}

function removeLinks($html) {
    return preg_replace('/<a(.)*>|<\/a>/', '', $html);
}

/**
 * Envia uma proposta ao SIOP/SOF. Se o parâmetro $tramitarNoSucesso for true, faz a tramitação no workflow da<br />
 * aplicação para o status 'ENVIADO AO SIOP'.
 *
 * @global cls_banco $db Abstração da base de dados.
 * @param array $dados Conjunto de dadas para tramitação da proposta identificada por acaid.
 * @param bool $tramitarNoSucesso Indica se tem que atualizar o status do workflow para 'ENVIADO AO SIOP'.
 * @return array
 */
function enviarProposta($dados) {
    global $db;

    // -- skipEnvio é informado pela mudança de estado manual no fim da execução deste método.
    // -- Ela cancela(retornando sucesso) a execução deste método ao ser chamado como pós-ação da tramitação manual.
    if ($dados['skipEnvio']) {
        return true;
    }

    $wsQuant = new Spo_Ws_Sof_Quantitativo(
        'proporc',
        (('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin']))
            ?Spo_Ws_Sof_Quantitativo::STAGING:Spo_Ws_Sof_Quantitativo::PRODUCTION
    );

    $dadosproposta = getDadosDaProposta($dados['acaid']);
    $proposta = new PropostaDTO();

    $propriedades = get_class_vars('propostaDTO');

    foreach ($dadosproposta as $prop => $valor) {
        if (key_exists($prop, $propriedades)) {
            $proposta->$prop = $valor;
        }
    }

    // -- Carregar dados das metas do PO
    $proposta->metaPlanoOrcamentario = getDadosPropostaMetasPO($dados['acaid']);
    // -- Carregar dados financeiros
    $proposta->financeiros = getDadosPropostaFinanceiros($dados['acaid']);

    // -- Removendo listas vazias da estrutura da requisição
    if (empty($proposta->financeiros)) {
        unset($proposta->financeiros);
    }
    if (empty($proposta->metaPlanoOrcamentario)) {
        unset($proposta->metaPlanoOrcamentario);
    }
    if (empty($proposta->receitas)) {
        unset($proposta->receitas);
    }

    // -- Processando o retorno do webservice: ERRO
    $wsRetorno = $wsQuant->cadastrarProposta($proposta);

    ver($wsRetorno,d);
    if (!$wsRetorno->return->sucesso) {
        $mensagens = array();
        if (!is_array($wsRetorno->return->mensagensErro)) {
            $mensagens[] = $wsRetorno->return->mensagensErro;
        } else {
            foreach ($wsRetorno->return->mensagensErro as $msg) {
                $mensagens[] = $msg;
            }
        }

        // -- Salvando mensagens de erro no banco de dados
        salvarLogDeErrosEnvioProposta($dados['acaid'], $mensagens);
        atualizarStatusProposta($dados['acaid'], 'E');

        $mensagens = '<ul style="list-style:square"><li>' . implode('</li><li>', $mensagens) . '</li></ul>';
        $mensagens = 'Não foi possível cadastrar a proposta no SIOP. Motivo(s):<br />' . $mensagens;
        return array('sucesso' => false, 'mensagem' => $mensagens);
    }

    // -- Processando o retorno do webservice: SUCESSO
    atualizarStatusProposta($dados['acaid'], 'S');

    if ($dados['tramitar']) {
        $sql = <<<SQL
SELECT pao.docid
  FROM elabrev.ppaacao_orcamento pao
  WHERE pao.acaid = %d
SQL;
        $stmt = sprintf($sql, $dados['acaid']);
        if ($docid = $db->pegaUm($stmt)) {
            // -- Transferindo o lote para "Processando"
            wf_alterarEstado(
                $docid,
                AESDID_SPO_PARA_SIOP,
                'Envio ao SIOP',
                array('acaid' => $dados['acaid'], 'skipEnvio' => false)
            );
        }
    }

    return array('sucesso' => true, 'mensagem' => 'Proposta cadastrada com sucesso no SIOP.');
}

function getDadosDaProposta($acaid) {
    global $db;

    $query = <<<DML
SELECT ppo.prgano::integer + 1 AS exercicio,
       ppo.esfcod AS "codigoEsfera",
       ppo.unicod AS "codigoOrgao",
       ppo.funcod AS "codigoFuncao",
       ppo.sfucod AS "codigoSubFuncao",
       ppo.prgcod AS "codigoPrograma",
       ppo.acacod AS "codigoAcao",
       1 AS "codigoTipoInclusaoAcao",
       ppo.loccod AS "codigoLocalizador",
       1 as "codigoTipoInclusaoLocalizador",
       2000 AS "codigoMomento", -- Momento órgão setorial
       tda.tpdid AS "codigoTipoDetalhamento",
       1 AS "snAtual",
       ppo.acaqtdefisico AS "quantidadeFisico",
       SUM(COALESCE(dpa.dpavalor, 0.00)) AS "valorFisico",
       ppo.justificativa,
       ppo.acaidentificadorsiop AS "identificadorUnicoAcao",
       dpa.acaid,
       ppo.acapropostaenviada
  FROM elabrev.despesaacao dpa
    INNER JOIN elabrev.ppaacao_orcamento ppo USING(acaid)
    LEFT JOIN elabrev.tipodetalhamentoacao tda USING(acaid)
  WHERE dpa.acaid = :acaid
  GROUP BY ppo.prgano,
           ppo.esfcod,
           ppo.unicod,
           ppo.funcod,
           ppo.sfucod,
           ppo.prgcod,
           ppo.acacod,
           ppo.loccod,
           tda.tpdid,
           ppo.acaqtdefisico,
           ppo.acaqtdefinanceiro,
           ppo.justificativa,
           ppo.acaidentificadorsiop,
           dpa.acaid,
           ppo.acapropostaenviada
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParam('acaid', $acaid);
    return $db->pegaLinha($dml);
}

function getDadosPropostaMetasPO($acaid) {
    global $db;

    $query = <<<DML
SELECT mpo.mpovalor AS "quantidadeFisico",
       plo.ploidentificadorunicosiop as "identificadorUnicoPlanoOrcamentario"
  FROM elabrev.metaplanoorcamentario mpo
    INNER JOIN elabrev.planoorcamentario plo USING(ploid)
  WHERE plo.acaid = :acaid
    AND COALESCE(ploproduto, '') != ''
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParam('acaid', $acaid);

    if (!$dadosdb = $db->carregar($dml)) {
        return null;
    }
    $retorno = array();
    foreach ($dadosdb as $meta) {
        $metaPODTO = new metaPlanoOrcamentarioDTO();
        foreach ($meta as $prop => $value) {
            $metaPODTO->$prop = $value;
        }
        $retorno[] = $metaPODTO;
    }
    return $retorno?$retorno:null;
}

function getDadosPropostaFinanceiros($acaid) {
    global $db;

    $query = <<<DML
SELECT dpa.iducod AS "idUso",
       ido.idocod AS "idOC",
       ndp.ndpcod AS "naturezaDespesa",
       dpa.foncod AS fonte,
       dpa.iducod AS "resultadoPrimarioAtual",
       dpa.iducod AS "resultadoPrimarioLei",
       plo.ploidentificadorunicosiop AS "identificadorPlanoOrcamentario",
       plo.plocodigo AS "codigoPlanoOrcamentario",
       SUM(COALESCE(dpa.dpavalor, 0.00)) AS valor
  FROM elabrev.despesaacao dpa
    INNER JOIN public.idoc ido USING(idoid)
    INNER JOIN public.naturezadespesa ndp USING(ndpid)
    INNER JOIN elabrev.planoorcamentario plo USING(acaid, ploid)
  WHERE dpa.acaid = :acaid
  GROUP BY dpa.iducod,
           ido.idocod,
           ndp.ndpcod,
           dpa.foncod,
           dpa.iducod,
           dpa.iducod,
           plo.ploidentificadorunicosiop,
           plo.plocodigo
DML;

    $dml = new Simec_DB_DML($query);
    $dml->addParam('acaid', $acaid);

    if (!$dadosdb = $db->carregar($dml)) {
        return array(new financeiroDTO());
    }

    $retorno = array();
    foreach ($dadosdb as $financeiro) {
        $finDTO = new financeiroDTO();
        foreach ($financeiro as $prop => $value) {
            $finDTO->$prop = $value;
        }

        $retorno[] = $finDTO;
    }
    return $retorno;
}

function salvarLogDeErrosEnvioProposta($acaid, $mensagens) {
    global $db;

    $query = <<<DML
INSERT INTO proporc.falhasenviosiop(acaid, fesmensagem, festipo)
  VALUES(:acaid, :fesmensagem, 'E')
DML;

    $dml = new Simec_DB_DML($query);
    $dml->addParam('acaid', $acaid);

    foreach ($mensagens as $msg) {
        $dml->addParam('fesmensagem', $msg ? $msg : 'ERROR NÃO IDENTIFICADO / SIOP');
        $db->executar($dml);
    }
    $db->commit();
}

function atualizarStatusProposta($acaid, $status) {
    global $db;

    $dml = <<<DML
UPDATE elabrev.ppaacao_orcamento
  SET acapropostaenviada = true,
      acadtenviosiop = now(),
      acastatusultimoenvio = :status
  WHERE acaid = :acaid
DML;
    $dml = new Simec_DB_DML($dml);
    $dml->addParam('acaid', $acaid)
            ->addParam('status', $status);
    $db->executar($dml);
    $db->commit();
}

function imprimirMotivoFalhaSIOP($dados) {
    global $db;
    $sql = <<<DML
SELECT fes.fesmensagem,
       TO_CHAR(fes.fesdata, 'DD/MM/YYYY HH24:MI:SS') AS fesdata
  FROM proporc.falhasenviosiop fes
  WHERE fes.acaid = :acaid
  ORDER BY fes.fesdata DESC
DML;
    $dml = new Simec_DB_DML($sql);
    $dml->addParam('acaid', $dados['acaid']);

    if (!$dadosdb = $db->carregar($dml)) {
        echo '<p style="text-align:center">Nenhuma falha foi retornada pela SOF.</p>';
        return;
    }

    $html = <<<HTML
<table class="table table-striped table-bordered table-hover">
    <thead>
        <tr>
            <th colspan="2" style="text-align:center">Mensagens</th>
        </tr>
        <tr>
            <th>Quando</th>
            <th>Descrição</th>
        </tr>
    </thead>
    <tbody>
HTML;

    foreach ($dadosdb as $mensagem) {
        $html .= <<<HTML
      <tr>
        <td>{$mensagem['fesdata']}</td>
        <td>{$mensagem['fesmensagem']}</td>
      </tr>
HTML;
    }

    $html .= <<<HTML
    </tbody>
</table>
HTML;
    echo $html;
}

function exibirSeletorDeUO() {
    $listaPerfis = pegaPerfilGeral();
    return in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $listaPerfis) || in_array(PFL_ADMINISTRADOR, $listaPerfis) || $_SESSION[$_SESSION['sisdiretorio']]['gestaoploa']['escolheruo'];
}

/**
 * Indica se um usuário pode realizar alterações em uma proposta. A decisão é tomada<br />
 * levando em consideração o status atual do documento e o perfi do usuário.
 * @param integer $esdid Estado do documento
 * @return boolean
 */
function podeEditar($esdid) {

    $listaPerfis = pegaPerfilGeral();

    switch ($esdid) {
        case ESDOC_ANALISE_SPO:
            return in_array(PFL_ADMINISTRADOR, $listaPerfis) || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $listaPerfis);
        case ESDOC_ENVIADO_SOF:
            return in_array(PFL_ADMINISTRADOR, $listaPerfis);
        case ESDOC_ACERTOS_UO:
        case ESDOC_EM_PREENCHIMENTO:
            break;
    }
    return true;
}

function exerciciosElabrev($exercicioUsuario, $func, $sistema) {
    global $db;

    $query = <<<DML
SELECT ppoid, ppoanoexercicio
  FROM elabrev.propostaorcamento
  WHERE tppid = 1
    AND ppostatus = 'A'
  ORDER BY ppoanoexercicio DESC
DML;
    $html = '';
    foreach ($db->carregar($query) as $prop) {
        if ($prop['ppoanoexercicio'] == $exercicioUsuario) {
            continue;
        }

        $html .= <<<HTML
    <li><a href="#" onclick="{$func}({$prop['ppoanoexercicio']}, {$prop['ppoid']}, '{$sistema}')">{$prop['ppoanoexercicio']}</a></li>
HTML;
    }
    return <<<HTML
    <div class="btn-group">
        <button type="button" class="btn btn-default dropdown-toggle btn-info" data-toggle="dropdown">
            Exercício: {$exercicioUsuario} <span class="caret"></span>
        </button>
        <ul class="dropdown-menu" role="menu">
{$html}
        </ul>
    </div>
HTML;
}
