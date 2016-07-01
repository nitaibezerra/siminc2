<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * 
 */
define('BACKLOG_ITEM_ORDEM', 99999);
/**
 * 
 */
define('NEXT_SPRINT_ITEM_START', 4000);

/**
 * Cadastra um novo entregável na base de dados do scrum.
 * @global cls_banco $db
 *      Conexão com a base de dados.
 * @param array $dados
 *      Dados para cadastro. Chaves obrigatórias: subprgdsc, prgid, sisid.
 */
function salvarEntregavel($dados)
{
    // -- Validação dos parametros
    if (!isset($dados['entdsc']) || empty($dados['entdsc'])
        || !isset($dados['usucpfsol']) || empty($dados['usucpfsol'])) {
        return false;
    }

    // -- Campos obrigatórios
    $campos = array(
        'entdsc' => "'%s'",
        'usucpfsol' => "'%s'"
    );

    // -- Capturando os campos preenchidos pelo usuário (opcionais)
    if (!empty($dados['entstid'])) {
        $campos['entstid'] = '%d';
    }
    if (!empty($dados['estid'])) {
        $campos['estid'] = '%d';
    } else {
        $campos['estid'] = "'%s'";
    }
    if (!empty($dados['enthrsexec'])) {
        $campos['enthrsexec'] = '%d';
    }
    if (!empty($dados['entordsprint'])) {
        $campos['entordsprint'] = '%d';
    }
    if (!empty($dados['usucpfsol'])) {
        $campos['usucpfsol'] = "'%s'";
    }
    if (!empty($dados['entdsc'])) {
        $campos['entdsc'] = "'%s'";
    }

    global $db;
    if (isset($dados['entid']) && !empty($dados['entid'])) { // -- update
        $dml = atualizarEntregavel($campos, $dados);
//        $dmlDemanda = atualizarDemanda($dados);
        
        $db->executar($dml);
//        $db->executar($dmlDemanda);
    } else { // -- insert
//        $dmlDemanda = inserirDemanda($dados);
//        if (!($dmdid = $db->pegaUm($dmlDemanda))) {
//            $db->rollback();
//            return false;
//        }
        $dmlEntregavel = inserirEntregavel($campos, $dados);
        if (!($entid = $db->pegaUm($dmlEntregavel))) {
            $db->rollback();
            return false;
        }
//        $dmlRelacionamento = inserirDemandaEntregavel($dmdid, $entid);
//        $db->executar($dmlRelacionamento);
    }
    return $db->commit();
}

function inserirEntregavel($campos, $dados)
{
    $dml = "
INSERT INTO scrum.entregavel(" . implode(', ', array_keys($campos)) . ")
  VALUES(" . implode(', ', $campos) . ")
  RETURNING entid";
    foreach ($campos as $key => &$campo) {
        $campo = str_replace("'", "''", $dados[$key]);
    }
    return vsprintf($dml, $campos);
}

function atualizarEntregavel($campos, $dados)
{
    $dml = array();
    foreach ($campos as $key => $campo) {
        $dml[] = "{$key} = {$campo}";
    }
    $dml = "UPDATE scrum.entregavel SET " . implode(', ', $dml) . " WHERE entid = %d";
    $campos['entid'] = '%d';
    foreach ($campos as $key => &$campo) {
        $campo = str_replace(
            "'",
            "''",
            empty($dados[$key])
                ?'null'
                :$dados[$key]);
    }
    return vsprintf($dml, $campos);
}

/**
 * Carrega e retorna os dados de um entregável.
 * @global cls_banco $db
 *      Conexão com a base de dados
 * @param array $dados
 *      Dados da requisição para processamento.
 * @return boolean|array
 */
function carregarEntregavel($dados)
{
    
    
    // -- Validação dos parâmetros
    if (!isset($dados['entid']) || empty($dados['entid'])) {
        return false;
    }

    $query = <<<DML
SELECT DISTINCT ent.entid,
       ent.entdsc,
       ent.enthrsexec,
       ent.entordsprint,
       ent.estid,
       ent.entstid,
       ent.usucpfsol,
       ent.usucpfresp,
       spg.subprgid,
       prg.prgid,
       u.usucpf AS usucpfsol, 
       u.usunome AS usucpfsol_dsc
  FROM scrum.entregavel AS ent
  LEFT JOIN scrum.estoria AS est USING(estid)
  LEFT JOIN scrum.subprg AS spg USING(subprgid)
  LEFT JOIN scrum.programa AS prg USING(prgid)
  LEFT JOIN seguranca.usuario AS u ON (u.usucpf = ent.usucpfsol)
  LEFT JOIN demandas.usuarioresponsabilidade AS ur ON (ur.usucpf = ent.usucpfsol)
  LEFT JOIN seguranca.usuario_sistema AS us ON (us.usucpf = ent.usucpfsol)
  WHERE ent.entid = %d
DML;
    $query = sprintf($query, $dados['entid']);
    global $db;
    return $db->pegaLinha($query);
}

/**
 * Prepara os parâmetros para serem inclusos na URL e filtrar a listagem de entregáveis.
 * 
 * @param array $dados
 *      Dados para filtragem de entregáveis.
 * @return string
 */
function filtrarEntregavel($dados)
{
    return criaFiltroURI(
        array(
            'prgid', 
            'subprgid', 
            'estid',
            'usucpfsol',
            'entdsc',
            'usucpfresp',
            'entstid',
            'enthrsexec',
            'entordsprint',
        ),
        $dados
    );
}

/**
 * Lista os entregáveis cadastrados com base em filtros de busca.
 * 
 * @global cls_banco $db
 *      Conexão com a base de dados
 * @param type $dados
 *      Dados de filtragem dos entregáveis. Parâmetros enviados via $_GET.
 */
function listarEntregaveis($dados)
{
    $where = array();

    if (!empty($dados['estid'])) {
        $where[] = sprintf('estid = %d', $dados['estid']);
    }
    if (!empty($dados['usucpfsol'])) {
        $where[] = sprintf("usucpfsol = '%s'", $dados['usucpfsol']);
    }
    if (!empty($dados['entdsc'])) {
        $where[] = sprintf("entdsc ILIKE '%%%s%%'", $dados['entdsc']);
    }
    if (!empty($dados['usucpfresp'])) {
        $where[] = sprintf("usucpfresp = '%s'", $dados['usucpfresp']);
    }
    if (!empty($dados['entstid'])) {
        $where[] = sprintf('entstid = %d', $dados['entstid']);
    }
    if (!empty($dados['enthrsexec'])) {
        $where[] = sprintf('enthrsexec = %d', $dados['enthrsexec']);
    }
    if (!empty($dados['entordsprint'])) {
        $where[] = sprintf('entordsprint = %d', $dados['entordsprint']);
    }
    if (!empty($where)) {
        $where = 'WHERE ' . implode(' AND ', $where);
    } else {
        $where = '';
    }

    $sql = <<<DML
SELECT '&nbsp;&nbsp;<input type="image" src="../imagens/alterar.gif" onclick="carregarItem('
            || ent.entid || ', \'entid\')" class="gui" title="Editar entregável" />' AS codigo,
       prg.prgdsc AS programa,
       spg.subprgdsc AS subprograma,
       SUBSTRING(ent.entdsc, 1, 200) || '...' AS descricao
  FROM scrum.entregavel ent
    LEFT JOIN scrum.estoria est USING(estid)
    LEFT JOIN scrum.subprg spg USING(subprgid)
    LEFT JOIN scrum.programa prg USING(prgid) {$where}
DML;

    global $db;
    $db->monta_lista($sql, array('&nbsp', 'Programa', 'Subprograma', 'Resumo do entregável'), 20, 5, false, 'center', 'N');
}

/**
 * Consulta todos os integráveis que compõem um programa e os ordena conforma
 * seu range de ordenação: primeiro itens da sprint, segundo itens da próxima
 * sprint e, por fim, itens do backlog.
 * 
 * @global cls_banco $db Conexão com a base de dados.
 * @param int $prgid ID do programa em questão.
 * @return \StdClass Informações de duração e composição das sprints e backlog.
 * 
 * @todo arrumar query da proxima sprint
 */
function listaEntregaveis($prgid)
{
     global $db;
    
    $oRetorno = new StdClass();
    $oRetorno->horasNaSprint = $oRetorno->horasGastas = $oRetorno->horasGastasProximaSprint = $oRetorno->idProximaSprint = $oRetorno->idSprintAtual = 0;
    $oRetorno->sprint = $oRetorno->proximaSprint = $oRetorno->backlog = array();

    $sqlSprintAtual = 'SELECT * FROM scrum.sprint WHERE now() between sptinicio and sptfim';
    $sprintAtual = $db->pegaLinha($sqlSprintAtual);

    // SPRINT ATUAL
    if($sprintAtual){
        $sqlSptrinAtual = "SELECT 
                        prg.prghrsprint,
                        ent.enthrsexec,
                        ent.entordsprint,
                        ent.entdsc,
                        ent.entid,
                        spg.subprgdsc,
                        spg.subprgcolor,
                        est.esttitulo,
                        ent.sptid,
                        COALESCE(usu.usunome, '') AS usucpfresp_dsc
                FROM scrum.entregavel ent
                    INNER JOIN scrum.estoria est USING(estid)
                    INNER JOIN scrum.subprg spg USING(subprgid)
                    INNER JOIN scrum.programa prg USING(prgid)
                    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                    LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
                WHERE prgid = {$prgid}
                AND spt.sptid = {$sprintAtual['sptid']}
                ORDER BY ent.entordsprint ASC, ent.entid DESC";
        $entregaveisSprintAtual = $db->carregar($sqlSptrinAtual);
        
        $oRetorno->idSprintAtual = $sprintAtual['sptid'];
        if(is_array($entregaveisSprintAtual)){
            $oRetorno->sprint = $entregaveisSprintAtual;
            
            foreach($oRetorno->sprint as $sprint){
                $oRetorno->horasGastas += $sprint['enthrsexec'];
            } 
        }
        
        $idProximaSprint = $sprintAtual['sptid'] + 1;
        $oRetorno->idProximaSprint = $idProximaSprint;
        
        //PROXIMA SPRINT 
        $sqlProximaSptrin = "SELECT 
                        prg.prghrsprint,
                        ent.enthrsexec,
                        ent.entordsprint,
                        ent.entdsc,
                        ent.entid,
                        spg.subprgdsc,
                        spg.subprgcolor,
                        est.esttitulo,
                        ent.sptid,
                        COALESCE(usu.usunome, '') AS usucpfresp_dsc
                FROM scrum.entregavel ent
                    INNER JOIN scrum.estoria est USING(estid)
                    INNER JOIN scrum.subprg spg USING(subprgid)
                    INNER JOIN scrum.programa prg USING(prgid)
                    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                    LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
                WHERE prgid = {$prgid}
                AND spt.sptid = {$idProximaSprint}
                ORDER BY ent.entordsprint ASC, ent.entid DESC";
        $entregaveisProximaSprint = $db->carregar($sqlProximaSptrin);
        
        if(is_array($entregaveisProximaSprint)){
            $oRetorno->proximaSprint = $entregaveisProximaSprint;
            
            foreach($oRetorno->proximaSprint as $sprint){
                $oRetorno->horasGastasProximaSprint += $sprint['enthrsexec'];
            } 
        }
        
    } 
    
    // BACKLOG
    $sqlEntregaveisBackLog = "SELECT 
                                    prg.prghrsprint,
                                    ent.enthrsexec,
                                    ent.entordsprint,
                                    ent.entdsc,
                                    ent.entid,
                                    spg.subprgdsc,
                                    spg.subprgcolor,
                                    est.esttitulo,
                                    ent.sptid,
                                    COALESCE(usu.usunome, '') AS usucpfresp_dsc
                            FROM scrum.entregavel ent
                                INNER JOIN scrum.estoria est USING(estid)
                                INNER JOIN scrum.subprg spg USING(subprgid)
                                INNER JOIN scrum.programa prg USING(prgid)
                                LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                                --LEFT JOIN scrum.sprint ON spt (spt.sptid = ent.sptid)
                            WHERE prgid = {$prgid}
                            AND sptid IS NULL
                                ORDER BY ent.entordsprint ASC, ent.entid DESC";
    $oRetorno->backlog = $db->carregar($sqlEntregaveisBackLog);
    
//    ver($oRetorno,d);
    
    return $oRetorno;
}

/**
 * Reordena todo um segmento de entregáveis incluíndo o novo item na posição
 * designada pelo usuário. Reordena o segmento sprint.
 * 
 * @global cls_banco $db Conexão com a base de dados.
 * @param int $prgid Programa em questão.
 * @param int $entid Entregável manipulado pelo usuário.
 * @param int $novaPosicao A nova posição que o entregável assumirá entre seus pares.
 * @return bool|int Resultado do commit da transação de atualização.
 */
function updateSprint($prgid, $entid, $novaPosicao , $idSprint)
{
    global $db;

    $sql = <<<DML
SELECT ent.entid, ent.entordsprint
  FROM scrum.entregavel ent
    INNER JOIN scrum.estoria est USING(estid)
    INNER JOIN scrum.subprg spg USING(subprgid)
  WHERE spg.prgid = %d
    AND ent.entordsprint < %d
    AND ent.entid <> %d
    AND ent.entstid = 1
  ORDER BY ent.entordsprint ASC
DML;
    $sql = sprintf($sql, $prgid, NEXT_SPRINT_ITEM_START, $entid);
    $result = (array)$db->carregar($sql);
    enfileirarEntregavel($result, $novaPosicao, $entid , $idSprint);
    return reordenarEntregaveis($result, $idSprint);
}

/**
 * Reordena todo um segmento de entregáveis incluíndo o novo item na posição
 * designada pelo usuário. Reordena o segmento próxima sprint.
 * 
 * @global cls_banco $db Conexão com a base de dados.
 * @param int $prgid Programa em questão.
 * @param int $entid Entregável manipulado pelo usuário.
 * @param int $novaPosicao A nova posição que o entregável assumirá entre seus pares.
 * @return bool|int Resultado do commit da transação de atualização.
 */
function updateProximaSprint($prgid, $entid, $novaPosicao)
{
    global $db;

    $sql = <<<DML
SELECT ent.entid, ent.entordsprint
  FROM scrum.entregavel ent
    INNER JOIN scrum.estoria est USING(estid)
    INNER JOIN scrum.subprg spg USING(subprgid)
  WHERE spg.prgid = %d
    AND ent.entordsprint >= %d
    AND ent.entordsprint < %d
    AND ent.entid <> %d
    AND ent.entstid = 1
  ORDER BY ent.entordsprint ASC
DML;
    $sql = sprintf($sql, $prgid, NEXT_SPRINT_ITEM_START, BACKLOG_ITEM_ORDEM, $entid);
    $result = (array)$db->carregar($sql);
    enfileirarEntregavel($result, $novaPosicao, $entid);
    return reordenarEntregaveis($result, NEXT_SPRINT_ITEM_START);
}

/**
 * Reordena todo um segmento de entregáveis incluíndo o novo item na posição
 * designada pelo usuário. Reordena o segmento backlog.
 * 
 * @global cls_banco $db Conexão com a base de dados.
 * @param int $prgid Programa em questão.
 * @param int $entid Entregável manipulado pelo usuário.
 * @param int $novaPosicao A nova posição que o entregável assumirá entre seus pares.
 * @return bool|int Resultado do commit da transação de atualização.
 */
function updateBacklog($prgid, $entid, $novaPosicao)
{
    global $db;

    $sql = <<<DML
SELECT ent.entid, ent.entordsprint
  FROM scrum.entregavel ent
    INNER JOIN scrum.estoria est USING(estid)
    INNER JOIN scrum.subprg spg USING(subprgid)
  WHERE spg.prgid = %d
    AND ent.entordsprint >= %d
    AND ent.entid <> %d
    AND ent.entstid = 1
  ORDER BY ent.entordsprint ASC
DML;
    $sql = sprintf($sql, $prgid, BACKLOG_ITEM_ORDEM, $entid);
    $result = (array)$db->carregar($sql);
    enfileirarEntregavel($result, $novaPosicao, $entid);
    return reordenarEntregaveis($result, BACKLOG_ITEM_ORDEM);
}

/**
 * De posse da lista de pares de um entregável, posiciona o novo entregável
 * na posição correta da lista.
 * 
 * @param array $entregaveis
 *      Lista de entregáveis de um segmento (excluíndo o entregável no caso de ordenação na mesma coluna).
 * @param int $novaPosicao
 *      Nova posição do item na lista.
 * @param int $entid
 *      O ID do entregável.
 */
function enfileirarEntregavel(array &$entregaveis, $novaPosicao, $entid)
{
    if (key_exists($novaPosicao, $entregaveis)) {
        $entregaveis = array_merge(
                array_slice($entregaveis, 0, $novaPosicao),
                array(array('entid' => $entid)),
                array_slice($entregaveis, $novaPosicao)
        );
    } else {
        $entregaveis[$novaPosicao] = array('entid' => $entid);
    }
}

    // -- Atualizando as prioridades de todas as tarefas
function reordenarEntregaveis(array $entregaveis, $offset = 0, $idSprint = '')
{
    
    echo $idSprint;
    exit;
    
    global $db;
    $sql = <<<DML
UPDATE scrum.entregavel
  SET entordsprint = %d
  WHERE entid = %d
DML;

    // -- Persistindo as novas posições na base de dados.
    foreach ($entregaveis as $key => $intregavel) {
        $stmt = sprintf($sql, $key + $offset, $intregavel['entid']);
        $db->executar($stmt);
    }
    return $db->commit();
}

function inserirDemanda($dados)
{
    $dados['entdsc_ttl'] = substr($dados['entdsc'], 0, 250);
    
    $sql = <<<DML
INSERT INTO demandas.demanda(usucpfdemandante,
                             usucpfinclusao,
                             tipid,
                             sidid,
                             laaid,
                             dmdsalaatendimento,
                             dmdtitulo,
                             dmddsc,
                             dmddatainclusao,
                             unaid,
                             celid,
                             dmdstatus,
                             dmdatendremoto,
                             dmdatendurgente,
                             dmdjudicial)
  VALUES('{$dados['usucpfsol']}',
         '',
         905,
         207,
         15,
         'SIMEC',
         '{$dados['entdsc_ttl']}',
         '{$dados['entdsc']}',
         'now()',
         15,
         2,
         'A',
         'f',
         'f',
         'f')
  RETURNING dmdid 
DML;

    return $sql;
}

function inserirDemandaEntregavel($dmdid, $entid)
{
    $sql = <<<DML
INSERT INTO scrum.entdmdacomp(dmdid, entid)
  VALUES(%d, %d)
DML;
    $sql = sprintf($sql, $dmdid, $entid);
    return $sql;
}


function jsonResponsavelTempoExecucao($dados)
{
    global $db;

    $sql = <<<DML
SELECT ent.enthrsexec,
       ent.usucpfresp,
       COALESCE(usu.usunome, 'Selecione...') AS usucpfresp_dsc,
       ent.entdsc
  FROM scrum.entregavel ent
    LEFT JOIN seguranca.usuario usu
      ON ent.usucpfresp = usu.usucpf
  WHERE ent.entid = %d
DML;
    $stmt = sprintf($sql, $dados['entid']);
    $result = $db->carregar($stmt);
    if (is_array($result)) {
        $result = array_shift($result);
        foreach ($result as $k => $data) {
            $result[$k] = utf8_encode($data);
        }
        header('Content-Type: application/json; charset=utf-8', true, 200);
        return $result;
    }
    return array();
}

function jsonUpdateEntregavel($dados)
{
    global $db;

    $sql = <<<DML
UPDATE scrum.entregavel
  SET entdsc = '%s',
      enthrsexec = %d,
      usucpfresp = '%s'
  WHERE entid = %d
DML;
    
    $entid = (int)$dados['entid'];
    foreach ($dados as $k => $v) {
        $dados[$k] = utf8_decode($v);
    }
    
    $stmt = sprintf($sql, $dados['entdsc'], $dados['enthrsexec'], $dados['usucpfresp'], $entid);
    $db->executar($stmt);
    return (bool)$db->commit();
}