<?php
function retornaColunasELabels()
{
    return array(
        'entcodent' => 'Cód',
        'entnome' => 'Escola',
        'tipo' => 'Tipo',
        'eixo' => 'Eixo Temático',
        'estuf' => 'UF',
        'mundescricao' => 'Município',
        'situacao' => 'Situação',
        'mcemodalidadeensino' => 'Modadelidade de Ensino',
        'classificacao' => 'Classificação',
    );
}

/**
 * Callback para array_walk. Coloca todos os itens do array entre aspas.
 * @param string $var Item do array.
 */
function quote(&$var)
{
    $var = "'{$var}'";
}

function monta_sql_escalar(&$sql, $campo)
{
    if (!empty($_REQUEST[$campo['filtro']])) {
        $where = "{$campo['campo']} = '{$_REQUEST[$campo['filtro']]}'";

        // -- Verifica se há algum placeholder para substituir
        if ($campo['placeholder']) {
            $sql = ' AND ' . str_replace($campo['placeholder'], $where, $sql);
            return;
        }
        return $where;
    }
    // -- Removendo placeholders não substituídos
    if ($campo['placeholder']) {
        $sql = str_replace($campo['placeholder'], '', $sql);
    }
    return;
}

function monta_sql_vetor(&$sql, $campo)
{
    if (!empty($_REQUEST[$campo['filtro']][0])) {
        if (1 == count($_REQUEST[$campo['filtro']])) {
            $where = "{$campo['campo']} = '{$_REQUEST[$campo['filtro']][0]}'";
        } else {
            array_walk($_REQUEST[$campo['filtro']], 'quote');
            $where = "{$campo['campo']} IN(" . implode(', ', $_REQUEST[$campo['filtro']]) . ')';
        }

        // -- Verifica se há algum placeholder para substituir
        if ($campo['placeholder']) {
            $sql = str_replace($campo['placeholder'], " AND {$where}", $sql);
            return;
        }
        return $where;
    }
    // -- Removendo placeholders não substituídos
    if ($campo['placeholder']) {
        $sql = str_replace($campo['placeholder'], '', $sql);
    }
    return;
}

/**
 *
 * array(
 *     'filtro' => 'eixo',
 *     'campo' => 'extid',
 *     'quote' => false, -- true/false
 *     'escalar' => false, -- true/false
 *     'placeholder' => 'w_eixo'
 * )
 * @param type $sql
 * @param type $arConfig
 * @return type
 */
function monta_sql(&$sql, $arConfig)
{
    $where = array();
    // -- Filtros do relatório
    foreach ($arConfig as $campo) {
        $return = null;
        if ($campo['escalar']) {
            $return = monta_sql_escalar($sql, $campo);
        } else {
            $return = monta_sql_vetor($sql, $campo);
        }

        // -- Se houve algum retorno, adiciona ao array de restrições da consulta
        if ($return) {
            $where[] = $return;
        }
    }

    // -- Processando os filtros escolhidos para incluir na query
    if (!empty($where)) {
        $where = 'AND ' . implode(' AND ', $where);
    } else {
        $where = '';
    }
    $sql = str_replace('__WHERE__', $where, $sql);
}

function monta_agp()
{
    $agrupador = $_REQUEST['agrupador'];
    $arLabels = retornaColunasELabels();
    $agp = array(
        'agrupador' => array(),
        'agrupadoColuna' => array(
            'estuf',
            'mundescricao',
            'classificacao',
            'eixo',
            'mcemodalidadeensino',
            'situacao',
            'tipo',
            'entcodent',
            'entnome',
        )
    );

    foreach ($agrupador as $grp) {
        array_push($agp['agrupador'], array('campo' => $grp, 'label' => $arLabels[$grp]));
    }

    array_push($agp['agrupador'], array('campo' => 'entcodent', 'label' => $arLabels['entcodent']));
    return $agp;
}

/**
 * Conjunto de colunas para avaliação do relatório
 */
function monta_colunas()
{
    $colunas = array();
    foreach (retornaColunasELabels() as $coluna => $label) {
        $type = 'string';
        $colunas[] = array('campo' => $coluna, 'label' => $label, 'type' => $type);
    }
    return $colunas;
}

