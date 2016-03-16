<?php
/**
 * @package SiMEC
 * @subpackage alteracoes-orcamentarias
 * @version $Id: _funcoes.php 102362 2015-09-11 19:01:00Z maykelbraz $
 */

/**
 * Consulta os tipos de creditos associados a um momento de crédito em um exercicio.
 *
 * @global cls_banco $db
 * @param int $mcrid ID do momento de crédito
 * @param int $exercicio Exercicio
 * @param bool $asJSON Indica se retorna um array ou uma string JSON.
 * @return array|string
 */
function carregarTipoCredito($exercicio, $mcrid = null, $asJSON = true) {
    global $db;
    $params = array($exercicio);

    $sql = <<<DML
SELECT tcr.tcrid AS codigo,
       tcr.tcrcod || ' - ' || tcr.tcrdsc  AS descricao
  FROM altorc.tipocredito tcr
  WHERE tcr.tcrstatus = 'A'
    AND tcr.tcrano = '%s'
DML;

    if (!is_null($mcrid)) {
        $sql .= <<<DML
    AND EXISTS (SELECT 1
                  FROM altorc.momentotipocredito mtc
                  WHERE mtc.tcrid = tcr.tcrid
                    AND mtc.mcrid = %d)
DML;
        $params[] = $mcrid;
    }
    $sql .= <<<DML
    ORDER BY 2
DML;

    $stmt = vsprintf($sql, $params);
    $data = $db->carregar($stmt);
    if (!$data) {
        $data = array();
    }
    // -- Se for retornar como JSON, faz o encode da descrição do tipo de crédito
    if ($asJSON) {
        foreach ($data as &$_data) {
            $_data['descricao'] = utf8_encode($_data['descricao']);
        }
        return simec_json_encode($data);
    }
    return $data;
}

/**
 * Consulta os programas associados a uma ação em um exercicio.
 *
 * @global cls_banco $db
 * @param int $acacod Código da ação
 * @param int $exercicio Exercicio
 * @param bool $asJSON Indica se retorna um array ou uma string JSON.
 * @return array|string
 */
function carregarProgramas($acacod, $exercicio, $asJSON = true, $unicod = null) {
    global $db;
    $sql = <<<DML
SELECT DISTINCT sna.prgcod AS codigo,
                sna.prgcod || ' - ' || sna.prgdsc AS descricao
  FROM altorc.snapshotacao sna
  WHERE sna.snaexercicio = '%d'
DML;
    $params = array($exercicio);
    if (!empty($acacod)) {
        $sql .= <<<DML
    AND sna.acacod = '%s'
DML;
        $params[] = $acacod;
    }
    if (!empty($unicod)) {
        $sql .= <<<DML
    AND sna.unicod = '%s'
DML;
        $params[] = $unicod;
    }
    $stmt = vsprintf($sql, $params);
    $data = $db->carregar($stmt);
    if (!$data) {
        $data = array();
    }
    // -- Se for retornar como JSON, faz o encode da descrição do tipo de crédito
    if ($asJSON) {
        foreach ($data as &$_data) {
            $_data['descricao'] = utf8_encode($_data['descricao']);
        }
        return simec_json_encode($data);
    }
    return $data;
}

if (!is_callable('chaveTemValor')) {

    /**
     * Verifica se existe uma chave em um array e se ela tem um valor definido.
     *
     * @param array $lista Lista para verificação da chave.
     * @param string $chave Chave do array para verificação.
     * @return bool
     */
    function chaveTemValor($lista, $chave) {
        if (!is_array($lista)) {
            $lista = array();
        }

        return isset($lista[$chave]) && !empty($lista[$chave]);
    }

}

/* Função para montar o Relatório Dinâmico */

function montaExtratoDinamico($post) {

    global $db;
    $listagem = new Simec_Listagem();
    /* Muda o tipo do objeto  */
    if ($post['requisicao'] == 'exportarXLS') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    }
    $cabecalho = array();
    /* Retorna vazio caso não seja selecionada nenhuma coluna. */
    if (count($post['dados']['cols-qualit']) == 0 || count($post['dados']['cols-qualit']) == 0) {
        $sql = "SELECT 1 WHERE 1 <> 1 ";
    }

    /* Tratando as colunas do Qualitativo */
    if (count($post['dados']['cols-qualit']) > 0) {
        foreach ($post['dados']['cols-qualit'] as $valor) {
            $titulo = $db->pegaLinha("SELECT crldsc FROM altorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QL'");
            $titulo = $titulo['crldsc'];
            // Cabeçalho
            array_push($cabecalho, $titulo);
            // Query
            $select .= " {$valor} ,";
        }
        $select = substr($select, 0, strlen($select) - 1);
        $groupby = $select;
    }

    /* Tratando as colunas do Quantitativo */
    if (count($post['dados']['cols-quant']) > 0) {
        $select .= ", ";
        foreach ($post['dados']['cols-quant'] as $valor) {
            $titulo = $db->pegaLinha("SELECT crldsc FROM altorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QT'");
            $titulo = $titulo['crldsc'];
            array_push($cabecalho, $titulo);
            // Query
            /* Testa se a coluna quantitativa é de Expressão */
            $colunaExpressao = $db->pegaLinha("SELECT crlexpquantitativo, crlexpcallback, crlexpcomtotal, crlexpaddgroupby FROM altorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QT' AND crlexpquantitativo IS NOT NULL");

            if (!$colunaExpressao) {
                $select .= " SUM({$valor}) AS {$valor} ,";
                $listagem->addCallbackDeCampo("{$valor}", 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
            } else {
                $select .= " {$colunaExpressao['crlexpquantitativo']} AS {$valor} ,";
                /* Caso tenha função Callback */
                if ($colunaExpressão['crlexpcallback'] != '') {
                    $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
                }
                /* Caso seja para totalizar */
                if ($colunaExpressao['crlexpcallback']) {
                    $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
                }
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
                $groupby .= $colunaExpressao['crlexpaddgroupby'];
            }
        }
        $select = substr($select, 0, strlen($select) - 1);
    }

    /* Filtros */
    if (count($post['dados']['filtros']) > 0) {
        foreach ($post['dados']['filtros'] as $chave => $valor) {
            /* @TODO  Lembrar de tratar tipo de dado depois que organizar a tabela */
            $valor = implode($valor, "','");
            $where .= " AND $chave IN ('{$valor}')";
        }
    }

    /* Montando a Query */
    if ($select != '' && $groupby != '') {
        $sql = " SELECT DISTINCT {$select}
        FROM
            altorc.vwpedidoscompleta vpc
        LEFT JOIN
            altorc.tipocredito tcr ON tcr.tcrid = vpc.tcrid
        LEFT JOIN
            public.unidade uni ON uni.unicod = vpc.unicod
        LEFT JOIN
            altorc.momentocredito mcr ON mcr.mcrid = vpc.mcrid
        {$join}
        WHERE
            vpc.paoano = '{$_SESSION['exercicio']}'
        {$where}
        GROUP BY
        {$groupby}
        ORDER BY 1 ";
    }

    #ver($post, $sql, $cabecalho, d);

    $dados = $db->carregar($sql);
    if (!is_array($dados)) {
        $dados = array();
    }
    $listagem->setDados($dados);
    $listagem->setCabecalho($cabecalho);
    $listagem->setFormOff();
    /* Mostrar a query em um hidden na tela */
    $saida['listagem'] = $listagem;
    $saida['sql'] = $sql;

    /* Imprime de acordo com a chamada */
    if ($post['requisicao'] == 'exportarXLS') {
        $_REQUEST['_p'] = 'all';
        $listagem->render();
        die();
    } else {
        return $saida;
    }
}

function statusNoSIOP($status, $dados)
{
    $html = <<<HTML
<span class="glyphicon glyphicon-%s" style="color:%s" %s></span>
HTML;

    switch ($status) {
        case 'f':
            return sprintf($html, 'thumbs-down', '#D9534F;cursor:pointer', ' onclick="detalharRetornoSIOP(' . $dados['paoid2'] . ')"');
        case 't':
            return sprintf($html, 'thumbs-up', '#5CB85C; cursor:pointer', ' onclick="detalharRetornoSIOP(' . $dados['paoid2'] . ')"');
        default:
            return sprintf($html, 'minus', '#F0AD4E', '');
    }
}

/**
 * Função de callback utilizada na listagem de localizadores.
 * @param integer $dpastatus ID do status do detalhamento do pedido referente ao localizador.
 * @return string
 * @see listarFuncionais.inc
 */
function statusLocalizador($dpastatus)
{
    switch ($dpastatus) {
        case -1:
            return '<span class="label label-warning">Sem alteração</span>';
        default:
            return '<span class="label label-success">Alterado</span>';
    }
}

/**
 * Função de callback utilizada na listagem de pos.
 * @param string $spoalterado Status de alteração do PO
 * @return string
 * @see listarAlteracoesPO.inc
 */
function statusAlteracaoPO($spoalterado)
{
    if ('M' == $spoalterado) {
        return '<span class="label label-success">Alterado</span>';
    } elseif ('N' == $spoalterado) {
        return '<span class="label label-info">Criado</span>';
    } else {
        return '<span class="label label-warning">Sem alteração</span>';
    }
}

/**
 * Função de callback de formatação da fonte de recurso.
 * @param string $tfrcod O valor do campo para formatação.
 * @return string
 */
function fonteRecurso($tfrcod) {
    switch ($tfrcod) {
        case 1: return 'Cancelamento';
        case 2: return 'Excesso';
        case 3: return 'Superavit';
        default: return $tfrcod;
    }
    return;
}

function ajusteFisico($valor)
{
    $valorAbsoluto = abs($valor);
    if ('0' == $valor) {
        return <<<HTML
<span class="glyphicon glyphicon-minus" style="color:orange"></span>
HTML;
    } elseif (false == strstr($valor, '-')) {
        return <<<HTML
<span class="glyphicon glyphicon-chevron-up" style="color:green"></span> {$valorAbsoluto}
HTML;
    } else {
        return <<<HTML
<span class="glyphicon glyphicon-chevron-down" style="color:red"></span> {$valorAbsoluto}
HTML;
    }
}

function tipoErroMensagem($tipo)
{
    switch ($tipo) {
        case 'E': return '<span class="label label-danger">Erro</span>';
        case 'S': return '<span class="label label-success">Sucesso</span>';
        case 'A': return '<span class="label label-warning">Aviso</span>';
    }
}

