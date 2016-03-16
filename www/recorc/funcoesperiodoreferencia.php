<?php
/**
 * Arquivo de funções para períodos de referência.
 * $Id: funcoesperiodoreferencia.php 98390 2015-06-09 18:48:49Z maykelbraz $
 */

/**
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param array $dados
 * @return type
 */
function inserirPeriodoReferencia(array $dados)
{
    global $db;

    // -- Validações
    if (empty($dados)) {
        return array(
            'sucesso' => false,
            'msg' => 'Os dados do novo período não podem ser vazios.'
        );
    }
    if (!chaveTemValor($dados, 'prfdsc')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Descrição' é obrigatório e não pode ser vazio."
        );
    } else {
        // -- Escapando aspas simples para o postgres
        $dados['prfdsc'] = str_replace("'", "''", $dados['prfdsc']);
    }
    if (!chaveTemValor($dados, 'prfdatainicio')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Período de validade' é obrigatório e não pode ser vazio."
        );
    }
    if (!chaveTemValor($dados, 'prfdatafim')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Período de validade' é obrigatório e não pode ser vazio."
        );
    }
    if (!chaveTemValor($dados, 'prfpreenchimentoinicio')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Período de preenchimento' é obrigatório e não pode ser vazio."
        );
    }
    if (!chaveTemValor($dados, 'prfpreenchimentofim')) {
        return array(
            'sucesso' => false,
            'msg' => "O campo 'Período de preenchimento' é obrigatório e não pode ser vazio."
        );
    }
    try {
        $dados['prfdatainicio'] = preparaData($dados['prfdatainicio']);
        $_ = new DateTime($dados['prfdatainicio']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data inicial do período é inválida."
        );
    }
    try {
        $dados['prfdatafim'] = preparaData($dados['prfdatafim']);
        $_ = new DateTime($dados['prfdatafim']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data final do período é inválida."
        );
    }
    try {
        $dados['prfpreenchimentoinicio'] = preparaData($dados['prfpreenchimentoinicio']);
        $_ = new DateTime($dados['prfpreenchimentoinicio']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data de preenchimento inicial do período é inválida."
        );
    }
    try {
        $dados['prfpreenchimentofim'] = preparaData($dados['prfpreenchimentofim']);
        $_ = new DateTime($dados['prfpreenchimentofim']);
    } catch (Exception $e) {
        return array(
            'sucesso' => false,
            'msg' => "A data final do período é inválida."
        );
    }

    if (!chaveTemValor($dados, 'prfid')) { // -- Insert
        $sql = <<<DML
INSERT INTO recorc.periodoreferencia(
    prfdsc,
    prfdatainicio,
    prfdatafim,
    exercicio,
    prfpreenchimentoinicio,
    prfpreenchimentofim,
    codcaptacaosiop,
    exerciciosiop
) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', %d, '%s')
DML;
        $stmt = sprintf(
            $sql,
            $dados['prfdsc'],
            $dados['prfdatainicio'],
            $dados['prfdatafim'],
            $_SESSION['exercicio'],
            $dados['prfpreenchimentoinicio'],
            $dados['prfpreenchimentofim'],
            $dados['codcaptacaosiop'],
            $dados['exerciciosiop']
        );
    } else { // -- Update
        $sql = <<<DML
UPDATE recorc.periodoreferencia
  SET prfdsc = '%s',
      prfdatainicio = '%s',
      prfdatafim = '%s',
      prfpreenchimentoinicio = '%s',
      prfpreenchimentofim = '%s',
      codcaptacaosiop = %d,
      exerciciosiop = '%s'
  WHERE prfid = %d
DML;
        $stmt = sprintf(
            $sql,
            $dados['prfdsc'],
            $dados['prfdatainicio'],
            $dados['prfdatafim'],
            $dados['prfpreenchimentoinicio'],
            $dados['prfpreenchimentofim'],
            $dados['codcaptacaosiop'],
            $dados['exerciciosiop'],
            $dados['prfid']
        );
    }

    // -- Executando insert
    $db->executar($stmt);
    if ($db->commit()) {
        return array(
            'sucesso' => true,
            'msg' => 'Sua requisição foi executada com sucesso.'

        );
    }

    return array(
        'sucesso' => false,
        'msg' => 'Não foi possível inserir o novo período.'
    );
}

function deletePeriodoReferencia(array $dados)
{
    global $db;

    // -- Validando dados do formulário
    if (!chaveTemValor($dados, 'prfid')) {
        return array(
            'sucesso' => false,
            'msg' => "Nenhum período foi selecionado para exclusão."
        );
    }

    // -- Inativando o período
    $sql = <<<DML
UPDATE recorc.periodoreferencia
  SET prfstatus = 'I'
  WHERE prfid = %d
DML;
    $stmt = sprintf($sql, $dados['prfid']);

    // -- Executando update
    $db->executar($stmt);
    if ($db->commit()) {
        return array(
            'sucesso' => true,
            'msg' => 'Sua requisição foi executada com sucesso.'

        );
    }

    return array(
        'sucesso' => false,
        'msg' => 'Não foi possível excluir o período selecionado.'
    );
}

function carregarPeriodoReferencia(array $dados)
{
    global $db;

    // -- Validando dados do formulário
    if (!chaveTemValor($dados, 'prfid')) {
        return array();
    }

    // -- Consultando dados do periodo
    $sql = <<<DML
SELECT prf.prfid,
       prf.prfdsc,
       to_char(prf.prfdatainicio, 'DD/MM/YYYY') AS prfdatainicio,
       to_char(prf.prfdatafim, 'DD/MM/YYYY') AS prfdatafim,
       to_char(prf.prfpreenchimentoinicio, 'DD/MM/YYYY') AS prfpreenchimentoinicio,
       to_char(prf.prfpreenchimentofim, 'DD/MM/YYYY') AS prfpreenchimentofim,
       codcaptacaosiop,
       exerciciosiop
  FROM recorc.periodoreferencia prf
  WHERE prf.prfid = %d
DML;

    $stmt = sprintf($sql, $dados['prfid']);
    return $db->pegaLinha($stmt);
}
