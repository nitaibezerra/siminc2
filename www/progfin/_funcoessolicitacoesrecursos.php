<?php
/**
 * Funções de apoio às solicitações recurso.
 * $Id: _funcoessolicitacoesrecursos.php 102315 2015-09-10 17:45:07Z maykelbraz $
 */
/**
 *
 */
require_once APPRAIZ . 'www/progfin/_funcoes.php';
require_once APPRAIZ . 'www/progfin/_funcoesliberacoes.php';


function deleteItemPedido($pedido) {
    global $db;

    $strSQL = "
        DELETE FROM progfin.liberacoesfinanceiras WHERE lfnid = {$pedido}
    ";

    $db->executar($strSQL);
    return $db->commit();
}

function deletePedido($pedido) {
    global $db;

    $strSQL = "
        DELETE FROM progfin.liberacoesfinanceiraserro WHERE lfnid IN (SELECT lfnid FROM progfin.liberacoesfinanceiras WHERE plfid = $pedido);
        DELETE FROM progfin.liberacoesfinanceiras WHERE plfid = $pedido;
        DELETE FROM progfin.pedidoliberacaofinanceira WHERE plfid = $pedido;
    ";

    $db->executar($strSQL);
    return $db->commit();
}

/**
 *
 * @param type $dados
 */
function dadosNovaSolicitacaoRecurso($dados) {
    global $db;

    $dadosSessao = array();
    $_SESSION['progfin']['solicitacao']['dados'] = &$dadosSessao;

    $dadosSessao['unicod'] = $dados['unicod'];
    $dadosSessao['fdsid'] = $dados['fdsid'];

    $query = <<<DML
SELECT
    uni.unicod,
    uni.unicod || ' - ' || uni.unidsc AS unidsc
  FROM public.unidade uni
  WHERE uni.unicod = '%d'
DML;
    $stmt = sprintf($query, $dados['unicod']);
    if (!($dadosdb = $db->carregar($stmt))) {
        throw new Exception("Unidade Orçamentária não encontrado: {$dados['unicod']}");
    }
    $dadosSessao['unidsc'] = $dadosdb[0]['unidsc'];
    $dadosSessao['unicod'] = $dadosdb[0]['unicod'];
}

/**
 *
 * @return boolean
 */
function dadosSolicitacaoRecursoNaSessao() {
    return (
            isset($_SESSION['progfin']) && isset($_SESSION['progfin']['solicitacao']) && isset($_SESSION['progfin']['solicitacao']['dados'])
            );
}

function getDadosFormItemPedido($lfnid) {
    global $db;

    $dml = <<<DML
SELECT
        lbf.*,
        esdid
  FROM progfin.liberacoesfinanceiras lbf
    INNER JOIN workflow.documento doc USING(docid)
  WHERE lfnid = %d
DML;
    //ver($dml, d);
    return $db->pegaLinha(sprintf($dml, (int) $lfnid));
}

function getItensPedidoRecursoFinanceiro($id, $podeEditar) {
    global $db;
    $enviado_sucesso = ESDID_LIBERACAO_ENVIO_SUCESSO;
    $sql = <<<DML
    SELECT lfn.lfnid,
           plf.plfid,
           TO_CHAR( lfn.lfninclusao , 'dd/mm/yyyy' ),
           ung.ungcod || ' - ' || ung.ungdsc as ungdsc,
           cp.clpdsc,
           COALESCE(lfn.acacod, lfn.acacod_2) AS acacod,
           lfn.stccod,
           lfn.ftrcod,
           lfn.ctgcod,
           lfn.vincod,
           lfn.lfnobservacao,
           lfn.lfnvalorsolicitado,
           uni.unicod AS unicod,
           esddsc,
        (
        SELECT
           TO_CHAR(htddata, 'dd/mm/yyyy' )
        FROM
           workflow.historicodocumento wf
           inner join workflow.documento wd using (docid)
        WHERE
           docid = lfn.docid
           AND wd.esdid = {$enviado_sucesso}
        ORDER BY
           htddata DESC limit 1 ) as datasituacao
  FROM progfin.liberacoesfinanceiras lfn
    LEFT JOIN progfin.pedidoliberacaofinanceira plf ON (plf.plfid = lfn.plfid)
    LEFT JOIN public.unidadegestora ung ON lfn.ungcodfavorecida = ung.ungcod
    LEFT JOIN public.unidade uni ON ung.unicod = uni.unicod
    LEFT JOIN progfin.classificacaopedido cp ON (cp.clpid = lfn.clpid)
    LEFT JOIN workflow.documento doc ON (doc.docid = plf.docid)
    LEFT JOIN workflow.documento doc2 ON (lfn.docid = doc2.docid)
    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc2.esdid
  WHERE plf.plfid = {$id}
  ORDER BY 3,4,5,6,7,8
DML;
#ver($sql);
    $colunas = array(
        'Criação',
        'Unidade Gestora (UG)',
        'Classificação do Pedido',
        'Ação Orçamentária',
        'Situação Contábil',
        'Fonte',
        'Cat. Gasto',
        'Vinculação',
        'Observação',
        'Valor solicitado (R$)',
        'Situação',
        'Data da Liberação'
    );

    $arrColunas = array(
        'Itens do Pedido - Recurso Financeiro' => $colunas);
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho($arrColunas)
            ->esconderColunas(array('plfid', 'unicod', 'esdid'))
            ->addCallbackDeCampo('lfnvalorsolicitado', 'mascaraMoeda')
            ->addCallbackDeCampo('lfnobservacao', 'diminuirFonte')
            ->addAcao($podeEditar ? 'delete' : 'view', array('func' => 'delPedido', 'extra-params' => array('plfid')))
            ->addAcao($podeEditar ? 'edit' : 'view', array('func' => 'editarPedido', 'extra-params' => array('plfid')))
           ->setQuery($sql);

    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "lfnvalorsolicitado")
            ->turnOnPesquisator()
            ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}

function getPedidoRecursoFinanceiro($id) {
    global $db;

    $strSQL = <<<DML
SELECT plf.plfid,
       plf.unicod,
       plf.plfobservacao,
       plf.docid,
       plf.usucpf,
       plf.dataultimaatualizacao,
       doc.esdid
  FROM progfin.pedidoliberacaofinanceira plf
    LEFT JOIN workflow.documento doc USING(docid)
  WHERE plfid = {$id}
DML;
    //ver($strSQL, d);
    $result = $db->pegaLinha($strSQL);
    return ($result) ? $result : null;
}

function salvarPedidoRecursoFinanceiro(array $post) {
    global $db;

    $docid = wf_cadastrarDocumento(TPDOC_PEDIDO_LIBERACAO, '');
    $db->commit();

    $sql = <<<DML
INSERT INTO progfin.pedidoliberacaofinanceira(
    unicod,
    plfobservacao,
    docid,
    usucpf,
    dataultimaatualizacao
) VALUES('%d', '%s', '%d', '%s', 'NOW()')
RETURNING plfid
DML;
    $post['dados']['lfnobservacao'] = str_replace(array(";", '"', "'", '¬', '<', '&', '>', '=', '%', '#', "\\"), '', $post['dados']['lfnobservacao']);
    $stmt = sprintf(
            $sql, $post['unicod'], str_replace("'", "''", $post['dados']['lfnobservacao']), $docid, $_SESSION['usucpf'], $docid
    );

    $plfid = $db->pegaUm($stmt);
    $db->commit();

    return $plfid;
}

function salvarItemPedidoRecursoFinanceiro(array $post) {
    global $db;

    $docid = wf_cadastrarDocumento(TPDOC_LIBERACAO_FINANCEIRA, '');
    $db->commit();

    $sql = <<<DML
SELECT unicod
  FROM public.unidadegestora
  WHERE ungcod = '{$post['ungcod']}'
DML;

    if (!empty($post['acacod_2'])) {
        $complemento = 'acacod_2';
        $acacod = $post['acacod_2'];
    } else {
        $complemento = 'acacod';
        $acacod = $post['acacod'];
    }

    $sql = <<<DML
INSERT INTO progfin.liberacoesfinanceiras(
    lfninclusao,
    ungcodemitente,
    ungcodfavorecida,
    stccod,
    ftrcod,
    ctgcod,
    vincod,
    lfnvalorsolicitado,
    lfnobservacao,
    usucpf,
    docid,
    clpid,
    plfid,
    {$complemento}
) VALUES('NOW()', '%s', '%s', '%s', '%s', '%s', '%s', %f, '%s', '%s', %d, %d, %d, '%s')
RETURNING lfnid
DML;
    $lfnvalorsolicitado = str_replace(array('.', ','), array('', '.'), $post['lfnvalor']);
    $post['lfnobservacao'] = str_replace(array(";", '"', "'", '¬', '<', '&', '>', '=', '%', '#', "\\"), '', $post['lfnobservacao']);
    $stmt = sprintf(
            $sql, CGF_UNIDADE_EMITENTE, $post['ungcod'], $post['stccod'], $post['fdsid'], $post['ctgcod'], $post['vincod'], $lfnvalorsolicitado, str_replace("'", "''", $post['lfnobservacao']), $_SESSION['usucpf'], $docid, $post['clpid'], $post['plfid'], $acacod
    );

    $lfnid = $db->pegaUm($stmt);
    $db->commit();

    return $lfnid;
}

function alterarPedidoRecursoFinanceiro(array $post) {
    #ver($_POST);
    global $db;

    //$docid = wf_cadastrarDocumento(TPDOC_LIBERACAO_FINANCEIRA, '');
    //$db->commit();

    $sql = <<<DML
    UPDATE progfin.liberacoesfinanceiras
        SET usucpf = '%s',
        ungcodemitente = '%s',
        ungcodfavorecida = '%s',
        lfnobservacao = '%s',
        numdocsiafi = %d,
        lfnvalorsolicitado = %f,
        ctgcod = '%s',
        stccod = '%s',
        llfid = null,
        lfntransferencia = '%s',
        vincod = '%s',
        ftrcod = '%s',
        trccod = %d

	WHERE lfnid = %d

DML;
    $post['dados']['lfnobservacao'] = str_replace(array(";", '"', "'", '¬', '<', '&', '>', '=', '%', '#', "\\"), '', $post['dados']['lfnobservacao']);
    $stmt = sprintf(
            $sql, $_SESSION['usucpf'], CGF_UNIDADE_EMITENTE, $post['dados']['ungcod'], $post['dados']['lfnobservacao'], 'null', str_replace(array('.', ','), array('', '.'), $post['dados']['lfnvalor']), $post['dados']['ctgcod'], $post['dados']['trccod'], 0, $post['dados']['vincod'], $post['dados']['fdsid'], 0, $post['dados']['lfnid']
    );

    ver($stmt, d);
    $lfnid = $db->pegaUm($stmt);
    $db->commit();
    return $lfnid;
}

function salvaAnaliseSolicitacaoRecurso(array $post) {
    global $db;

    $docid = wf_cadastrarDocumento(TPDOC_LIBERACAO_FINANCEIRA, '');
    $db->commit();

    $sql = <<<DML
UPDATE progfin.liberacoesfinanceiras
  SET lfnvalorautorizado = %f,
      lfnvaloratendido = %f
  WHERE lfnid = %d;
DML;

    $stmt = sprintf(
            $sql, str_replace(array('.', ','), array('', '.'), $post['dados']['lfnvalorautorizado']), str_replace(array('.', ','), array('' . '.'), $post['dados']['lfnvaloratendido']), $post['dados']['lfnid']
    );

    $lfnid = $db->pegaUm($stmt);
    $db->commit();
    return $lfnid;
}

function podeEditarPedido($esdid) {
    switch ($esdid) {
        case ESDID_PEDIDO_ANALISE_SPO:
            $perfisGerenciais = array(PFL_SUPER_USUARIO, PFL_CGF_EQUIPE_FINANCEIRA);
            if (array_intersect($perfisGerenciais, pegaPerfilGeral($_SESSION['usucpf']))) {
                return true;
            }
        case ESDID_PEDIDO_EM_PREENCHIMENTO:
        case ESDID_PEDIDO_AJUSTES_UO:
        case '':
            return true;
        // -- no break
    }

    return false;
}

function podeEditarLiberacao($esdid, $llfid) {
    // -- Liberações feitas dentro de um lote não podem ser editadas
    if (!empty($llfid)) {
        return false;
    }

    switch ($esdid) {
        case ESDID_LIBERACAO_CADASTRADO:
        case ESDID_LIBERACAO_AJUSTES_UO:
            return true;
        // -- no break
    }

    $perfisGerenciais = array(PFL_SUPER_USUARIO, PFL_CGF_EQUIPE_FINANCEIRA);
    if (array_intersect($perfisGerenciais, pegaPerfilGeral($_SESSION['usucpf']))) {
        return true;
    }

    return false;
}

function colocaIcone2($texto) {
    switch (trim($texto)) {
        case 'Cadastrado':
            return '<span class="glyphicon glyphicon-minus text-warning"></span>';
        case 'Análise SPO':
            return '<span class="glyphicon glyphicon-transfer text-success"></span>';
        case 'Ajustes UO':
            return '<span class="glyphicon glyphicon-transfer text-danger"></span>';
        case 'Aguardando comunicação':
            return '<span class="glyphicon glyphicon-refresh text-warning"></span>';
        case 'Enviado com sucesso':
            return '<span class="glyphicon glyphicon-check text-success"></span>';
        case 'Erro ao enviar':
            return '<span class="glyphicon glyphicon-remove text-danger"></span>';
        case 'Falha ao enviar':
            return '<span class="glyphicon glyphicon-remove text-danger"></span>';
        case 'Processando':
            return '<span class="glyphicon glyphicon-refresh text-warning"></span>';
        default:
            return '';
    }
}

/* Função para montar o Relatório Dinâmico */

function montaExtratoDinamicoLiberacoesFinanceirasProgfin($post, $whereUO) {
    global $db;
    $listagem = new Simec_Listagem();
    /* Muda o tipo do objeto  */
    if ($post['requisicao'] == 'exportarXLS') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    }
    $cabecalho = array();
    $orderby = 1;
    /* Retorna vazio caso não seja selecionada nenhuma coluna. */
    if (count($post['dados']['cols-qualit']) == 0 || count($post['dados']['cols-qualit']) == 0) {
        $sql = "SELECT 1 WHERE 1 <> 1 ";
    }

    /* Tratando as colunas do Qualitativo */
    if (count($post['dados']['cols-qualit']) > 0) {
        $i = 1;

        $groupby_array = array();
        foreach ($post['dados']['cols-qualit'] as $valor) {
            if (trim($valor) == '')
                continue;

            $colunaExpressao = $db->pegaLinha("SELECT crlcod,crldsc,crlexpcallback FROM progfin.colunasextrato_lf WHERE crlexpaddgroupby = '{$valor}' AND crltipo = 'QL'");
            if (!$colunaExpressao)
                continue;

            /* Caso tenha função Callback */
            if ($colunaExpressao['crlexpcallback'] != '') {
                $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
            }

            /* Definindo o Order By pela coluna que possui callback de texto alinhado à esquerda. */
            if ($colunaExpressao['crlexpcallback'] == 'alinhaParaEsquerda') {
                if ($orderby == 1)
                    $orderby = $i++;
                else
                    $orderby .= ',' . $i++;
            }

            // Cabeçalho
            array_push($cabecalho, $colunaExpressao['crldsc']);

            // Query
            $select .= " {$colunaExpressao['crlcod']} ,";
            $groupby_array[] = $valor;

            if ($valor == 'esddsc') {
                array_push($cabecalho, '');
                $listagem->addCallbackDeCampo("simbolo", 'colocaIcone2');
                $select .= " esddsc as simbolo,";
                $groupby_array[] = 'simbolo';
            }
        }
        $select = substr($select, 0, strlen($select) - 1);
        $groupby = implode(',', $groupby_array);
        /* Resolução de problema da apresentação da unidade gestora. Bolar uma solução no futuro. */
        if (in('ungcodfavorecida', $groupby_array)) {
            $groupby = $groupby ? $groupby . ',ungcod,ungdsc' : 'ungcod,ungdsc';
        }

        if (in('uni.unicod', $groupby_array)) {
            $groupby = $groupby ? $groupby . ',uni.unicod,uni.unidsc' : 'uni.unicod,uni.unidsc';
        }

        $groupby_array = null;
    }
#ver($post['dados']['cols-quant'],d);
    /* Tratando as colunas do Quantitativo */
    if (count($post['dados']['cols-quant']) > 0) {
        $cabecalho_qualitativo = array();
        $select .= ",";
        foreach ($post['dados']['cols-quant'] as $valor) {
            if (trim($valor) == '')
                continue;

            $titulo = $db->pegaLinha("SELECT crldsc FROM progfin.colunasextrato_lf WHERE crlcod = '{$valor}' AND crltipo = 'QT'");
            $titulo = $titulo['crldsc'];
            //array_push($cabecalho, $titulo);
            $cabecalho_qualitativo[] = $titulo;
            // Query
            /* Testa se a coluna quantitativa é de Expressão */
            $colunaExpressao = $db->pegaLinha("SELECT crlexpquantitativo, crlexpcallback, crlexpcomtotal, crlexpaddgroupby FROM progfin.colunasextrato_lf WHERE crlcod = '{$valor}' AND crltipo = 'QT' AND crlexpquantitativo IS NOT NULL");

            if (!$colunaExpressao) {
                $select .= " SUM({$valor}) AS {$valor} ,";
                $listagem->addCallbackDeCampo("{$valor}", 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
            } else {
                $select .= " {$colunaExpressao['crlexpquantitativo']} AS {$valor} ,";
                /* Caso tenha função Callback */
                if ($colunaExpressao['crlexpcallback'] != '') {
                    $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
                }
                /* Caso seja para totalizar */
                if ($colunaExpressao['crlexpcallback']) {
                    $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
                }
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
                $groupby_array[] = $colunaExpressao['crlexpaddgroupby'];
            }
        }
        /* Adicionando diferença caso exista a coluna Solicitado e Atendido. */
        if (in('Autorizado', $cabecalho_qualitativo) && in('Atendido', $cabecalho_qualitativo)) {
            $cabecalho_qualitativo[] = "Restante";
            $select .= " (SUM(lfnvalorautorizado) - SUM(lfnvaloratendido)) AS diferenca ,";
            $listagem->addCallbackDeCampo("diferenca", 'mascaraMoeda');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "diferenca");
            $groupby_array[] = "diferenca";
        }
        /* Adicionando o cabeçalho de liberação de recursos caso exista alguma coluna quantitativa. */
        if (sizeof($cabecalho_qualitativo) > 0)
            $cabecalho['Liberação de Recursos (R$)'] = $cabecalho_qualitativo;

        $select = substr($select, 0, strlen($select) - 1);
        #$groupby .= ",".implode(',',$groupby_array);
    }

    /* Filtros */

    if (count($post['dados']['filtros']) > 0) {
        foreach ($post['dados']['filtros'] as $chave => $valor) {
            /* @TODO  Lembrar de tratar tipo de dado depois que organizar a tabela */
            if ('filtipopedido' == $chave) {
                switch ($valor) {
                    case 'I':
                        $where .= ' AND lfn.llfid IS NULL';
                        break;
                    case 'L':
                        $where .= ' AND lfn.llfid IS NOT NULL';
                        break;
                    default:
                }
            } elseif ($valor) {
                $valor = implode($valor, "','");
                $where .= " AND $chave IN ('{$valor}')";
            }
        }
    }

    // -- Where UO
    $where .= $whereUO;

    /* Montando a Query */
    if ($select != ''
        &&  (count($post['dados']['cols-qualit']) > 0)
        &&  (count($post['dados']['cols-quant']) > 0)) {
        $sql = " SELECT DISTINCT {$select}
        FROM
            progfin.liberacoesfinanceiras lfn
        INNER JOIN public.unidadegestora ung ON ung.ungcod = lfn.ungcodfavorecida
        INNER JOIN public.unidade uni ON ung.unicod = uni.unicod
        LEFT JOIN progfin.loteliberacoesfinanceiras llf ON (llf.llfid = lfn.llfid)
        LEFT JOIN progfin.classificacaopedido clp ON (clp.clpid = lfn.clpid)
        LEFT JOIN workflow.documento doc ON doc.docid = lfn.docid
        LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        WHERE
            1 = 1
        {$where}
        GROUP BY
        {$groupby}
        ORDER BY {$orderby} ";
    } else {
        $sql = "SELECT 1 WHERE 1 <> 1 ";
    }

    $dados = $db->carregar($sql);
    if (!is_array($dados)) {
        $dados = array();
    }
    $listagem->setDados($dados);

    try { // -- Exportação não suporta pesquisator
        $listagem->turnOnPesquisator();
    } catch (Exception $e) {

    }
#ver($select,d)
    $listagem->setCabecalho($cabecalho);
    $listagem->setFormOff();

    /* Mostrar a query em um hidden na tela */
    $saida['listagem'] = $listagem;
    $saida['sql'] = $sql;

    return $saida;
}

/**
 * Retorna o devido html caso já exista um pedido.
 * @return string
 */
function apresentaNumeroPedido($id) {

    if ((isset($id) && (0 != $id))) {
        global $db;
        $sql = "SELECT
     plf.unicod || ' - ' || uni.unidsc
    FROM
     progfin.pedidoliberacaofinanceira plf
    JOIN
     public.unidade uni
    USING
     ( unicod )
    WHERE
     plfid = {$_GET['id']}";

        $unidsc = $db->pegaUm($sql);
        // -- Formatando o ID para exibição
        $id = str_pad($_GET['id'], 7, '0', STR_PAD_LEFT);

        // -- Resumo financeiro
        $sql = <<<DML
SELECT clpdsc,
       SUM(lbf.lfnvalorsolicitado) AS lfnvalorsolicitado,
       SUM(lbf.lfnvalorautorizado) AS lfnvalorautorizado,
       SUM(lbf.lfnvaloratendido) AS lfnvaloratendido
  FROM progfin.liberacoesfinanceiras lbf
    INNER JOIN progfin.classificacaopedido clp USING(clpid)
  WHERE lbf.plfid = %d
  GROUP BY clpdsc
  ORDER BY clpdsc
DML;
        $stmt = sprintf($sql, $id);

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $totais = array('lfnvalorsolicitado', 'lfnvalorautorizado', 'lfnvaloratendido');
        $listagem->setQuery($stmt)
                ->setCabecalho(array('Classificação', 'Valores (R$)' => array('Solicitado', 'Autorizado', 'Atendido')))
                ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $totais)
                ->addCallbackDeCampo($totais, 'mascaraMoeda')
                ->setFormFiltros('formBusca');

        $accordion = montaItemAccordion(
                '<span class="glyphicon glyphicon-info-sign"></span> Resumo por classificação do pedido (R$)', 'resumocat', $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM), array('accordionID' => 'accordion2', 'retorno' => true)
        );

        // -- Informações do pedido
        echo <<<HTML
    <style type="text/css">
        .table .panel{margin-bottom:0}
    </style>
    <div class="panel panel-info">
        <div class="panel-heading">
          <h3 class="panel-title">Informações do pedido</h3>
        </div>
        <table class="table">
            <tbody>
                <tr>
                    <td style="font-weight:bold;text-align:right;width:25%">Número do Pedido:</td>
                    <td>{$id}</td>
                </tr>
                <tr>
                    <td style="font-weight:bold;text-align:right;width:25%">Unidade Orçamentária:</td>
                    <td>{$unidsc}</td>
                </tr>
                <tr>
                    <td colspan="2">
                        {$accordion}
                    </td>
                </tr>
            </tbody>
        </table>
    </div>
HTML;
    }
}

function tramitarLiberacoes($liberacoes, $siafiUser, $siafiPass, $siafiUG, $siafiComment, $tramitar = true) {
    global $db;

    /* Colocando no Comentário a observação igual ao do pedido */
    // -- Atualizando usuário, senha e uo em todos os pedidos
    $sql = <<<DML
UPDATE progfin.liberacoesfinanceiras
  SET siafi_username = :siafiUser,
      siafi_password = :siafiPass,
      siafi_ug = :siafiUg,
      siafi_comment = lfnobservacao
  WHERE lfnid = :lfnid
DML;
    $dml = new Simec_DB_DML($sql);
    $dml->addParam('lfnid', $liberacoes)
            ->addParam('siafiUser', AES256_CBC_enc(trim(str_replace(array('.', '-'), '', $siafiUser))))
            ->addParam('siafiPass', AES256_CBC_enc(trim($siafiPass)))
            ->addParam('siafiUg', $siafiUG);
    $db->executar($dml);
    $db->commit();

    if ($tramitar) {
        // -- Alterando o status do documento
        $sql = <<<DML
SELECT lfnid,
       docid,
       plfid
  FROM progfin.liberacoesfinanceiras lfnid
  WHERE lfnid.lfnid = :lbfid
DML;
        $dml->setString($sql);
        $dml->addParam('lbfid', $liberacoes);

        if ($dados = $db->carregar($dml)) {
            foreach ($dados as $liberacao) {
                if (wf_alterarEstado(
                                $liberacao['docid'], TRANS_LIBERACAO_ANALISE_SPO_AGD_COMUNICACAO, $siafiComment, array()
                        )) {
                    /* Fazendo a alteração automática do status do pedido */
                    mudancaAutomaticaStatusPedido($liberacao['plfid']);
                };
            }
        }
    }
}

function form_captarCredenciais() {
    $siafiUsuario = inputTexto('siafi_usuario', null, 'siafi_usuario', 14, false, array('return' => true, 'masc' => '###.###.###-##', 'obrig' => 'S'));
    $siafiUg = inputTexto('siafi_ug', null, 'siafi_ug', 6, false, array('return' => true, 'masc' => '######', 'obrig' => 'S'));
    $siafiComment = str_replace(
            '<br>', '', inputTextArea(
                    'siafi_comment', null, 'siafi_comment', 300, array(
        'return' => true,
        'obrig' => 'S',
        'complemento' => array(
            'required' => true
        )
                    )
            )
    );

    echo <<<HTML
<style type="text/css">
.ui-dialog-titlebar-close{display:none}
.ui-widget-header{background:#d9edf7;border-color:#bce8f1;color:#31708f}
.form-group.row img, .form-group.row>label.error{display:none}
</style>
<div class="form-horizontal">
    <input type="hidden" name="lfnid" id="popup_lfnid" />
    <div class="form-group row">
        <label class="control-label col-md-4" for="siafi_usuario">Usuário SIAFI:</label>
        <div class="col-md-8">{$siafiUsuario}</div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-4" for="siafi_password">Senha:</label>
        <div class="col-md-8">
            <input type="password" class="form-control required" maxlength="50" id="siafi_password" name="siafi_password" />
        </div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-4" for="siafi_ug">UG (usuário):</label>
        <div class="col-md-8">{$siafiUg}</div>
    </div>
    <div class="form-group row">
        <label class="control-label col-md-4" for="siafi_comment">Observação:</label>
        <div class="col-md-8">{$siafiComment}</div>
    </div>
</div>
<script type="text/javascript">
setTimeout(function(){
    $('.ui-dialog-buttonset button:first').addClass('btn btn-danger');
    $('.ui-dialog-buttonset button:last').addClass('btn btn-primary');
    $('.ui-dialog-title').text('Enviar Pedido de Liberação Financeira ao SIAFI');
    $('#siafi_comment').addClass('required').removeProp('required'); // -- oO - precisa para funcionar o validate
    $('#div_dialog_workflow').css('overflow', 'hidden');
    $('#popup_lfnid').val($('#lfnid').val());
}, 100);
</script>
HTML;
}

function captarCredenciais() {
    tramitarLiberacoes(
            $_POST['lfnid'], $_POST['siafi_usuario'], $_POST['siafi_password'], $_POST['siafi_ug'], $_POST['siafi_comment'], false
    );

    echo simec_json_encode(Array('boo' => true, 'msg' => ''));
}

function situacaoDocumentosPedido() {
    return array(
        'Todos' => '',
        'Em preenchimento' => ESDID_PEDIDO_EM_PREENCHIMENTO,
        'Análise SPO' => ESDID_PEDIDO_ANALISE_SPO,
        'Em atendimento' => ESDID_PEDIDO_EM_ATENDIMENTO,
        'Ajustes UO' => ESDID_PEDIDO_AJUSTES_UO,
        'Atendimento Concluído' => ESDID_PEDIDO_ATENDIMENTO_CONCLUIDO,
    );
}

function situacaoDocumentosLiberacao() {
    return array(
        'Todos' => '',
        'Em preenchimento' => ESDID_LIBERACAO_CADASTRADO,
        'Análise SPO' => ESDID_LIBERACAO_ANALISE_SPO,
        'Ajustes UO' => ESDID_LIBERACAO_AJUSTES_UO,
        'Enviado com Sucesso' => ESDID_LIBERACAO_ENVIO_SUCESSO,
    );
}
function inputComboClassificacao($clpid, $cpltipo = 'OUTROS', $opcoes = array()) {

    if ($cpltipo != 'TODOS') {
        $where = "WHERE cpltipo = '$cpltipo'";
    }
    $sql = <<<DML
SELECT clp.clpid AS codigo,
       clp.clpdsc AS descricao
  FROM progfin.classificacaopedido clp
    {$where}
DML;
    inputCombo('dados[clpid]', $sql, $clpid, 'clpid', $opcoes);
}

function inputComboAcaoOrcamentaria($unicod, $acacod = null) {
    $sql = <<<DML
SELECT '?' AS codigo,
       '--- Ação fora da LOA ---' AS descricao
UNION
SELECT acacod AS codigo,
       acacod ||' - '|| acadsc AS descricao
  FROM monitora.acao
  WHERE unicod = '{$unicod}'
    AND prgano = '{$_SESSION['exercicio']}'
  ORDER BY 1
DML;

    //ver($sql, d);
    inputCombo('dados[acacod]', $sql, $acacod, 'acacod');
}

function alterarItemPedidoRecursoFinanceiro(array $post) {
    global $db;

    if (empty($post['acacod_2'])) {
        $complemento = "acacod = '%s', acacod_2 = null";
        $acacod = $post['acacod'];
    } else {
        $complemento = "acacod = NULL, acacod_2 = '%s'";
        $acacod = $post['acacod_2'];
    }

    $sql = <<<DML
UPDATE progfin.liberacoesfinanceiras
  SET usucpf = '%s',
      ungcodemitente = '%s',
      ungcodfavorecida = '%s',
      lfnobservacao = '%s',
      lfnvalorsolicitado = %f,
      ctgcod = '%s',
      stccod = '%s',
      vincod = '%s',
      ftrcod = '%s',
      clpid = %d,
      {$complemento}
  WHERE lfnid = %d
DML;

    $lfnvalorsolicitado = str_replace(array('.', ','), array('', '.'), $post['lfnvalor']);
    $observacao = str_replace(array(";", '"', "'", '¬', '<', '&', '>', '=', '%', '#', "\\"), '', $post['lfnobservacao']);
    $stmt = sprintf(
            $sql, $_SESSION['usucpf'], CGF_UNIDADE_EMITENTE, $post['ungcod'], $observacao, $lfnvalorsolicitado, $post['ctgcod'], $post['stccod'], $post['vincod'], $post['fdsid'], $post['clpid'], $acacod, $post['lfnid']
    );


    $lfnid = $db->pegaUm($stmt);
    $db->commit();
    return $lfnid;
}

/*
 * Carregar os dados da Análise
 */

function carregarDadosAnalise($plfid) {
    global $db;
    if (isset($plfid)) {

        $enviado_sucesso = ESDID_LIBERACAO_ENVIO_SUCESSO;
        /* Dados do PEDIDO */

        $detalhes = "COALESCE(clp.clpdsc, '-') AS classificacao,";
        /* Dados do DAS LIBERAÇÕES */
        $sql = <<<DML
SELECT lfn.lfnid,
       lfn.lfnidorigem,
       lfn.lfnid AS enviarpedido,
       TO_CHAR(lfn.lfninclusao, 'DD/MM/YYYY') AS data,
       lfn.ungcodfavorecida || ' - ' || ung.ungdsc AS unidade,
       {$detalhes}
       lfn.stccod,
       lfn.ftrcod AS fonte,
       lfn.ctgcod AS gnd,
       lfn.vincod,
       lfn.lfnobservacao,
       lfnvalorsolicitado AS solicitado,
       COALESCE(lfnvalorautorizado,0) AS autorizado,
       COALESCE(lfnvaloratendido,0) AS atendido,
       COALESCE(numdocsiafi::varchar, '-') AS numdocsiafi,
       CASE WHEN lfn.lfntransferencia IS NULL THEN esd.esddsc
            WHEN lfn.lfntransferencia = 'S' THEN 'Enviado com sucesso'
            WHEN lfn.lfntransferencia = 'E' THEN 'Erro ao enviar'
       END AS esddsc,
       esd.esdid,
       doc.docid,
       plfid,
       lfn.lfnid AS lfnid2,
       (SELECT to_char(lfe.lfecriacao, 'DD/MM/YYYY HH:MI:SS') || ' - ' || lfe.lfnmensagem
          FROM progfin.liberacoesfinanceiraserro lfe
          WHERE lfe.lfnid = lfn.lfnid
          ORDER BY lfe.lfecriacao DESC
          LIMIT 1) AS lfnmensagem,
       (
        SELECT
           TO_CHAR(htddata, 'dd/mm/yyyy' )
        FROM
           workflow.historicodocumento wf
           inner join workflow.documento wd using (docid)
        WHERE
           docid = lfn.docid
           AND wd.esdid = {$enviado_sucesso}
        ORDER BY
           htddata DESC limit 1 ) as datasituacao,
        COALESCE(lfnidorigem, lfnid) AS origem
  FROM progfin.liberacoesfinanceiras lfn
    INNER JOIN public.unidadegestora ung ON lfn.ungcodfavorecida = ung.ungcod
    INNER JOIN workflow.documento doc ON doc.docid = lfn.docid
    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
    LEFT JOIN spo.gruponaturezadespesa gnd ON lfn.ctgcod = gnd.gndcod::varchar
    LEFT JOIN progfin.classificacaopedido clp ON (clp.clpid = lfn.clpid)
  WHERE plfid  = {$plfid} ORDER BY 6,7,8,9,10,2 DESC
DML;

  #ver($sql,d);
        $result['liberacoes'] = $sql;
        return ($result) ? $result : array();
    } else {
        return false;
    }
}

/*
 * Transforma o campo Atendido em Editavel
 */
function campoEditavelAtendido($atendido, $dados = array()) {
    $atendido = mascaraMoeda($atendido, false);
    if ((ESDID_LIBERACAO_ANALISE_SPO == $dados['esdid']) && (array_intersect(
                    pegaPerfilGeral($_SESSION['usuario']), array(PFL_SUPER_USUARIO, PFL_CGF_EQUIPE_FINANCEIRA)))
    ) {
        return inputTexto('atendido_' . $dados['lfnid2'], $atendido, 'atendido_' . $dados['lfnid2'], 17, true, array('return' => true, 'size' => '20'));
    } else {
        return $atendido;
    }
}

/*
 * Tramita todas as liberações de um pedido para Análise SPO
 */

function tramitarLiberacoesParaAnaliseSPO($plfid) {
    global $db;

    /**
     * Constantes da programação financeira.
     * @see _constantes.php
     */
    require_once(APPRAIZ . 'www/progfin/_constantes.php');

    // -- Alterando o status do documento
    $sql = <<<DML
SELECT lfnid,
       docid
  FROM progfin.liberacoesfinanceiras lfnid
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
  WHERE plfid = {$plfid}
    AND esdid = %d
DML;

    $stmt = sprintf($sql, ESDID_LIBERACAO_CADASTRADO);

    if ($dados = $db->carregar($stmt)) {
        foreach ($dados as $liberacao) {
            wf_alterarEstado(
                    $liberacao['docid'], TRANS_LIBERACAO_CADASTRADO_ANALISE_SPO, ' - ', array(), array()
            );
        }
    }

    return true;
}

function tramitarLiberacoesParaAjustesUO($plfid) {
    global $db;

    /**
     * Constantes da programação financeira.
     * @see _constantes.php
     */
    require_once(APPRAIZ . 'www/progfin/_constantes.php');

    // -- Alterando o status do documento
    $sql = <<<DML
SELECT lfnid,
       docid,
       usucpf
  FROM progfin.liberacoesfinanceiras lfnid
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
  WHERE plfid = {$plfid}
    AND esdid = %d
DML;

    $stmt = sprintf($sql, ESDID_LIBERACAO_ANALISE_SPO);

    if ($dados = $db->carregar($stmt)) {
        foreach ($dados as $liberacao) {
            wf_alterarEstado(
                $liberacao['docid'], TRANS_LIBERACAO_ANALISE_SPO_CORRECOES_UO, ' - ', array(), array()
            );

            enviarEmailPosTramiteLiberacoesParaAjustesUO($liberacao['usucpf'], $_POST['dados']['cmddsc']);
        }
    }

    return true;
}

function tramitarLiberacoesParaRetornoAnaliseSPO($plfid) {
    global $db;

    /**
     * Constantes da programação financeira.
     * @see _constantes.php
     */
    require_once(APPRAIZ . 'www/progfin/_constantes.php');

    // -- Alterando o status do documento
    $sql = <<<DML
SELECT lfnid,
       docid
  FROM progfin.liberacoesfinanceiras lfnid
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
  WHERE plfid = {$plfid}
    AND esdid = %d
DML;

    $stmt = sprintf($sql, ESDID_LIBERACAO_AJUSTES_UO);

    if ($dados = $db->carregar($stmt)) {
        foreach ($dados as $liberacao) {
            wf_alterarEstado(
                    $liberacao['docid'], TRANS_LIBERACAO_ACERTOS_PARA_ANALISE_SPO, ' - ', array(), array()
            );
        }
    }

    // -- Tramitando novos registros que apareceram durante os acertos
    tramitarLiberacoesParaAnaliseSPO($plfid);

    return true;
}

function salvarValoresFinanceiros($dados, $parse = true) {
    global $db, $fm;

    if ($parse) {
        $parsedData = array(
            'aprovado' => array(),
            'atendido' => array()
        );
        foreach ($dados as $name => $value) {
            if (strstr($name, 'aprovado_')) {
                list(, $id) = explode('aprovado_', $name);
                $parsedData['aprovado'][$id] = $value;
            } elseif (strstr($name, 'atendido_')) {
                list(, $id) = explode('atendido_', $name);
                $parsedData['atendido'][$id] = $value;
            }
        }
        $dados = $parsedData;
    }

    // -- Atualizando os valores
    foreach ($dados['aprovado'] as $id => $valorAprovado) {
        $dml = <<<DML
UPDATE progfin.liberacoesfinanceiras
  SET lfnvalorautorizado = %f,
      lfnvaloratendido = %f
  WHERE lfnid = %d
DML;
        $stmt = sprintf(
            $dml, str_replace(array('.', ','), array('', '.'), $valorAprovado), str_replace(array('.', ','), array('', '.'), $valorAprovado), $id
        );

        $db->executar($stmt);
        $db->commit();
    }

    $fm->addMensagem('Valores financeiros atualizados com sucesso.');
}

function gravaLinhaDiferenca($dados) {

    $dml = sprintf("
        select * from progfin.liberacoesfinanceiras where lfnid = %d
    ");

    foreach ($dados as $id) {

    }

}

/*
 * Valida uma única linha no pedido com
 * Classificação do Pedido + Fonte + Cat. Gasto + Cat. Gasto +
 */

function validaPedido(array $post) {
    global $db;

    if (empty($post['dados']['plfid'])) {
        return true;
    }

    $sql = <<<DML
SELECT COUNT(*)
  FROM progfin.liberacoesfinanceiras
  WHERE stccod = '%s'
    AND ftrcod = '%s'
    AND ctgcod = '%s'
    AND clpid = %d
    AND plfid = %d
DML;
    $params = array(
        $post['dados']['stccod'],
        $post['dados']['fdsid'],
        $post['dados']['ctgcod'],
        $post['dados']['clpid'],
        $post['dados']['plfid']
    );

    // -- Editando um registro já existente
    if (array_key_exists('lfnid', $post['dados'])) {
        $sql .= ' AND lfnid != %d';
        $params[] = $post['dados']['lfnid'];
    }

    $strSQL = vsprintf($sql, $params);
    return ($db->pegaUm($strSQL)) ? false : true;
}

/**
 * Callback de processamento de liberações finaceiras - qdo um registro
 * é tramitado para o estado de ERRO, essa função zera o valor atendido daquela
 * solicitação.
 *
 * @global cls_banco $db
 * @param int $lfnid
 * @return bool
 */
function zeraValorAtendido($lfnid) {
    global $db;

    $sql = <<<DML
UPDATE progfin.liberacoesfinanceiras
  SET lfnvaloratendido = 0
  WHERE lfnid = %d
DML;
    $stmt = sprintf($sql, $lfnid);
    $result = (bool) $db->executar($stmt);
    $db->commit();

    return $result;
}

function tramitarLiberacoesDeProcessandoParaErro($lfnid, $docid) {
    global $db;

    $mensagem = 'Erro de comunicação: E00WM6';
    wf_alterarEstado(
            $docid, TRANS_LIBERACAO_PROC_PARA_ERRO, $mensagem, array('lfnid' => $lfnid), array()
    );

    $sql = <<<DML
INSERT INTO progfin.liberacoesfinanceiraserro(lfnid, lfnmensagem)
  VALUES (%d, '%s')
DML;
    $stmt = sprintf($sql, $lfnid, $mensagem);
    $db->executar($stmt);
    $db->commit();
}

/*
 * Função para mudar o status do pedido
 * (Status "de fora")
 */

function mudancaAutomaticaStatusPedido($plfid) {
    global $db;

    $estatosFinais = '(' . ESDID_LIBERACAO_AGD_COMUNICACAO . ',' . ESDID_LIBERACAO_PROCESSANDO . ',' . ESDID_LIBERACAO_ANALISE_SPO . ',' . ESDID_LIBERACAO_ENVIO_SUCESSO . ',' . ESDID_LIBERACAO_ENVIO_FALHA_AJUSTES_UO . ',' . ESDID_LIBERACAO_CANCELADO . ')';
    /* buscando pedidos em que todas as linhas já tem situacao final */
    $sql = <<<DML
SELECT
    COUNT(0) as total
FROM
 progfin.liberacoesfinanceiras
JOIN
 workflow.documento
USING
 ( docid )
WHERE
 plfid = {$plfid}
 AND
 esdid NOT IN $estatosFinais
DML;
    $pedidosEmAtendimento = $db->pegaLinha($sql);

    /* buscando o DOCID do PEDIDO */
    $sql = <<<DML
SELECT
    docid
FROM
 progfin.pedidoliberacaofinanceira
WHERE
 plfid = {$plfid}
DML;
    $docidPedido = $db->pegaLinha($sql);

    /* alternado o pedido para concluido */
    if ($pedidosEmAtendimento['total'] >= 1) {
        wf_alterarEstado($docidPedido['docid'], TRANS_PEDIDO_EM_ATENDIMENTO, '', array());
    } else {
        wf_alterarEstado($docidPedido['docid'], TRANS_PEDIDO_EM_ATENDIMENTO, '', array());
        wf_alterarEstado($docidPedido['docid'], TRANS_PEDIDO_CONCLUIR_ATENDIMENTO, '', array());
    }
}

/**
 * Envio de e-mail para tramitação em lote
 * @param $email
 */
function enviarEmailPosTramiteLiberacoesParaAjustesUO($usucpf, $cmddsc) {
    global $db;

    $sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", $usucpf);
    $email = $db->pegaUm($sql);

    if (IS_PRODUCAO) {
        $cc = $atual;
        $conteudo = $cmddsc;
    } else {
        $email = $atual;
        $cc = array($_SESSION['email_sistema']);
        $conteudo = $cmddsc;
    }

    $_SESSION['_progfin_']['_cmddsc_'] = $cmddsc;
    $remetente = array('nome' => 'SPO - Programação Financeira', 'email' => $_SESSION['email_sistema']);
    $assunto  = '[SPO - Programação Financeira] Solicitação de Programação Financeira';
    if (enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco)) {
        $_SESSION['_progfin_']['_destinatarios_'][] = $email;
    }
}

/**
 *
 */
function enviaEmailConfirmacaoTramite() {
    global $db;

    $sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", $_SESSION['usucpf']);
    $atual = $db->pegaUm($sql);

    $txtDestinatarios = '';
    if (is_array($_SESSION['_progfin_']['_destinatarios_'])) {
        foreach ($_SESSION['_progfin_']['_destinatarios_'] as $email) {
            $txtDestinatarios .= $email.'<br />';
        }

        $remetente = array('nome' => 'SPO - Programação Financeira', 'email' => $_SESSION['email_sistema']);
        $assunto  = '[SPO - Programação Financeira] Solicitação de Programação Financeira';
        $conteudo = "<p>{$_SESSION['_progfin_']['_cmddsc_']}</p>";
        $conteudo.= "<p>Usuários que receberam este envio de e-mail:</p>";
        $conteudo.= "<p>{$txtDestinatarios}</p>";

        enviar_email($remetente, $atual, $assunto, $conteudo);
    }

    unset($_SESSION['_progfin_']);
}
