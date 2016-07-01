<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Cadastra um novo programa na base de dados do scrum.
 * @global cls_banco $db
 *      Conexão com a base de dados.
 * @param array $dados
 *      Dados para cadastro. Chaves obrigatórias: prgdsc, prghrsprint.
 */
function salvarPrograma($dados)
{
    // -- Validação dos parametros
    if (!isset($dados['prgdsc']) || !isset($dados['prghrsprint'])
        || empty($dados['prgdsc']) || empty($dados['prghrsprint'])) {
        return false;
    }

    if (isset($dados['prgid']) && !empty($dados['prgid'])) { // -- update
        $dml = <<<DML
UPDATE scrum.programa
  SET prgdsc = '%s',
      prghrsprint = %d
  WHERE prgid = %d
DML;
        $dml = sprintf($dml, $dados['prgdsc'], $dados['prghrsprint'], $dados['prgid']);
    } else { // -- insert
        $dml = <<<DML
INSERT INTO scrum.programa(prgdsc, prghrsprint)
  VALUES('%s', %d)
DML;
        $dml = sprintf($dml, $dados['prgdsc'], $dados['prghrsprint']);
    }
    global $db;
    $db->executar($dml);
    return $db->commit();
}

/**
 * Carrega e retorna os dados de um programa.
 * @global cls_banco $db
 *      Conexão com a base de dados
 * @param array $dados
 *      Dados da requisição para processamento.
 * @return boolean|array
 */
function carregarPrograma($dados)
{
    // -- Validação dos parâmetros
    if (!isset($dados['prgid']) || empty($dados['prgid'])) {
        return false;
    }

    $query = <<<DML
SELECT prgid, prgdsc, prghrsprint
  FROM scrum.programa
  WHERE prgid = %d
DML;
    $query = sprintf($query, $dados['prgid']);
    global $db;
    return $db->pegaLinha($query);
}

/**
 * Prepara os parâmetros para serem inclusos na URL e filtrar a listagem de programas.
 * 
 * @param array $dados
 *      Dados para filtragem dos programas.
 * @return string
 */
function filtrarPrograma($dados)
{
    return criaFiltroURI(
        array(
            'prgdsc',
            'prghrsprint'
        ),
        $dados
    );
}

/**
 * Lista os programas cadastrados com base em filtros de busca.
 * 
 * @global cls_banco $db
 *      Conexão com a base de dados
 * @param type $dados
 *      Dados de filtragem dos programas. Parâmetros enviados via $_GET.
 */
function listarProgramas($dados)
{
    $where = array();
    if (!empty($dados['prgdsc'])) {
        $where[] = sprintf("prgdsc ILIKE '%%%s%%'", $dados['prgdsc']);
    }
    if (!empty($dados['prghrsprint'])) {
        $where[] = sprintf("prghrsprint = %d", $dados['prghrsprint']);
    }
    if (!empty($where)) {
        $where = 'WHERE ' . implode(' AND ', $where);
    } else {
        $where = '';
    }

    $sql = <<<DML
SELECT '&nbsp;&nbsp;<input type="image" src="../imagens/alterar.gif" onclick="carregarItem('
            || prgid || ', \'prgid\')" class="gui" title="Editar programa" />' AS codigo,
       prgdsc AS descricao,
       prghrsprint || ' hs' AS duracao
  FROM scrum.programa {$where}
DML;

    global $db;
    $db->monta_lista($sql, array('&nbsp', 'Programa', "Duração da sprint"), 20, 5, false, 'center', 'N');
}

function montaComboPrograma($prgid, $callback = null)
{
    global $db;
    
    $sql = <<<DML
SELECT prg.prgid AS codigo,
       prg.prgdsc AS descricao
  FROM scrum.programa prg
DML;
    
    
    $db->monta_combo(
            'prgid',
            $sql,
            'S',
            'Selecione um programa',
            (is_null($callback)?'':$callback),
            null,
            null,
            null,
            'N',
            'prgid',
            null,
            $prgid,
            null,
            'style="width:250px"'
    );
}


function recuperaHorasSprint($prgid)
{
    return array(
        array('total' => 20, 'usadas' => 10),
        array('total' => 20, 'usadas' => 10)
    );
}