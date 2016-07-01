<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Cadastra um novo subprograma na base de dados do scrum.
 * @global cls_banco $db
 *      Conexão com a base de dados.
 * @param array $dados
 *      Dados para cadastro. Chaves obrigatórias: subprgdsc, prgid, sisid.
 */
function salvarSubprograma($dados)
{
    // -- Validação dos parametros
    if (!isset($dados['subprgdsc']) || !isset($dados['subprgdsc'])
        || empty($dados['prgid']) || empty($dados['prgid'])
        || empty($dados['sisid']) || empty($dados['sisid'])
        || !isset($dados['subprgcolor']) || empty($dados['subprgcolor'])
        || empty($dados['sidid'])) {
        return false;
    }

    if (isset($dados['subprgid']) && !empty($dados['subprgid'])) { // -- update
        $dml = <<<DML
UPDATE scrum.subprg
  SET subprgdsc = '%s',
      prgid = %d,
      sisid = %d,
      subprgcolor = '%s',
      sidid = %d
  WHERE subprgid = %d
DML;
        $dml = sprintf($dml, $dados['subprgdsc'], $dados['prgid'], $dados['sisid'], $dados['subprgcolor'], $dados['sidid'], $dados['subprgid']);
    } else { // -- insert
        $dml = <<<DML
INSERT INTO scrum.subprg(subprgdsc, prgid, sisid, subprgcolor , sidid)
  VALUES('%s', %d, %d, '%s', %d)
DML;
        $dml = sprintf($dml, $dados['subprgdsc'], $dados['prgid'], $dados['sisid'], $dados['subprgcolor'], $dados['sidid']);
    }
    
    global $db;
    $db->executar($dml);
    return $db->commit();
}

/**
 * Carrega e retorna os dados de um subprograma.
 * @global cls_banco $db
 *      Conexão com a base de dados
 * @param array $dados
 *      Dados da requisição para processamento.
 * @return boolean|array
 */
function carregarSubprograma($dados)
{
    // -- Validação dos parâmetros
    if (!isset($dados['subprgid']) || empty($dados['subprgid'])) {
        return false;
    }

    $query = <<<DML
SELECT subprgid, subprgdsc, prgid, sisid, subprgcolor, sidid
  FROM scrum.subprg
  WHERE subprgid = %d
DML;
    $query = sprintf($query, $dados['subprgid']);
    
    
//    ver($query,d);
    global $db;
    return $db->pegaLinha($query);
}

/**
 * Prepara os parâmetros para serem inclusos na URL e filtrar a listagem de subprogramas.
 * 
 * @param array $dados
 *      Dados para filtragem dos subprogramas.
 * @return string
 */
function filtrarSubprograma($dados)
{
    return criaFiltroURI(
        array(
            'subprgdsc',
            'prgid',
            'sisid'
        ),
        $dados
    );
}

/**
 * Lista os subprogramas cadastrados com base em filtros de busca.
 * 
 * @global cls_banco $db
 *      Conexão com a base de dados
 * @param type $dados
 *      Dados de filtragem dos subprogramas. Parâmetros enviados via $_GET.
 */
function listarSubprogramas($dados)
{
    $where = array();
    if (!empty($dados['subprgdsc'])) {
        $where[] = sprintf("subprgdsc ILIKE '%%%s%%'", $dados['subprgdsc']);
    }
    if (!empty($dados['prgid'])) {
        $where[] = sprintf("prgid = %d", $dados['prgid']);
    }
    if (!empty($dados['sisid'])) {
        $where[] = sprintf("sisid = %d", $dados['sisid']);
    }
    if (!empty($where)) {
        $where = 'WHERE ' . implode(' AND ', $where);
    } else {
        $where = '';
    }

    $sql = <<<DML
SELECT '&nbsp;&nbsp;<input type="image" src="../imagens/alterar.gif" onclick="carregarItem('
            || spg.subprgid || ', \'subprgid\')" class="gui" title="Editar subprograma" />' AS codigo,
       spg.subprgdsc AS descricao,
       prg.prgdsc AS programa,
       sis.sisdsc AS sistema
  FROM scrum.subprg spg
    INNER JOIN scrum.programa prg USING(prgid)
    INNER JOIN seguranca.sistema sis USING(sisid) {$where}
DML;
    global $db;
    $db->monta_lista($sql, array('&nbsp', 'Subprograma', 'Programa', 'Sistema'), 20, 5, false, 'center', 'N');
}

/**
 * Consulta a lista de subprogrmas para inserção em um select.
 * 
 * @global cls_banco $db
 *      Conexão com a base de dados
 * @param array $dados
  *      Dados para consulta de subprogramas.
 * @return type
 */
function jsonSubprograma($dados)
{
    $sql = <<<DML
SELECT spg.subprgid AS codigo,
       spg.subprgdsc AS descricao
  FROM scrum.subprg spg
  WHERE spg.prgid = %d
  ORDER BY 2
DML;
    $sql = sprintf($sql, $dados['prgid']);

    global $db;
    $result = $db->carregar($sql);
    foreach ($result as &$item) {
        $item['descricao'] = utf8_encode($item['descricao']);
    }
    return $result;
}
