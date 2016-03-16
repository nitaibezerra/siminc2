<?php
/**
 * Funções de apoio ao gerenciamento de momentos de crédito.
 * $Id: _funcoesmomentocredito.php 78244 2014-04-01 18:26:00Z maykelbraz $
 */

/**
 * Apaga um momento de crédito
 * @global cls_banco $db
 * @param array $dados Dados para apagar o momento de crédito.
 * @return bool
 */
function apagarMomentoCredito($dados)
{
    global $db;

    $sql = <<<DML
UPDATE altorc.momentocredito
  SET mcrstatus = 'I'
  WHERE mcrid = %d
DML;
    $stmt = sprintf($sql, $dados['mcrid']);
    $db->executar($stmt);

    if (!$db->commit()) {
        $db->rollback();
        return false;
    }

    return true;
}

function carregarDadosDoMomentoDeCredito($dados)
{
    global $db;

    $sql = <<<DML
SELECT mcr.mcrid,
       mcr.mcrdsc,
       mcr.mcrcod,
       TO_CHAR(mcr.mcrrefinicio, 'DD/MM/YYYY') AS mcrrefinicio,
       TO_CHAR(mcr.mcrreffim, 'DD/MM/YYYY') AS mcrreffim,
       TO_CHAR(mcr.mcrincinicio, 'DD/MM/YYYY') AS mcrincinicio,
       TO_CHAR(mcr.mcrincfim, 'DD/MM/YYYY') AS mcrincfim,
       TO_CHAR(mcr.mcraltinicio, 'DD/MM/YYYY') AS mcraltinicio,
       TO_CHAR(mcr.mcraltfim, 'DD/MM/YYYY') AS mcraltfim,
       tcr.tcrcod,
       tcr.tcrid
  FROM altorc.momentocredito mcr
    INNER JOIN altorc.momentotipocredito mtc USING(mcrid)
    INNER JOIN altorc.tipocredito tcr USING(tcrid)
  WHERE mcr.mcrid = %d
DML;
    $stmt = sprintf($sql, $dados['mcrid']);
    if (!($dadosMomentoCredito = $db->carregar($stmt))) {
        return false;
    }

    $linhaAtual = current($dadosMomentoCredito);
    $dados['mcrid'] = $linhaAtual['mcrid'];
    $dados['mcrdsc'] = $linhaAtual['mcrdsc'];
    $dados['mcrcod'] = $linhaAtual['mcrcod'];
    $dados['mcrrefinicio'] = $linhaAtual['mcrrefinicio'];
    $dados['mcrreffim'] = $linhaAtual['mcrreffim'];
    $dados['mcrincinicio'] = $linhaAtual['mcrincinicio'];
    $dados['mcrincfim'] = $linhaAtual['mcrincfim'];
    $dados['mcraltinicio'] = $linhaAtual['mcraltinicio'];
    $dados['mcraltfim'] = $linhaAtual['mcraltfim'];

    $dados['tcrid'] = array();

    foreach ($dadosMomentoCredito as $dado) {
        $dados['tcrid'][] = $dado['tcrid'];
    }
    return $dados;
}

function salvarMomentoCredito($dados, $mcrano)
{
    global $db;

    $dados['mcrdsc'] = str_replace("'", "''", $dados['mcrdsc']);
    $dados['mcrcod'] = str_replace("'", "''", $dados['mcrcod']);

    if (isset($dados['mcrid']) && !empty($dados['mcrid'])) {
        $sql = <<<DML
UPDATE altorc.momentocredito
  SET mcrdsc = '%s',
      mcrcod = '%s',
      mcrrefinicio = '%s',
      mcrreffim = '%s',
      mcrincinicio = '%s',
      mcrincfim = '%s',
      mcraltinicio = '%s',
      mcraltfim = '%s'
  WHERE mcrid = %d
  RETURNING mcrid
DML;
        $stmt = sprintf(
            $sql,
            $dados['mcrdsc'],
            $dados['mcrcod'],
            $dados['mcrrefinicio'],
            $dados['mcrreffim'],
            $dados['mcrincinicio'],
            $dados['mcrincfim'],
            $dados['mcraltinicio'],
            $dados['mcraltfim'],
            $dados['mcrid']
        );
    } else {
        $sql = <<<DML
INSERT INTO altorc.momentocredito(
    mcrdsc,
    mcrcod,
    mcrrefinicio,
    mcrreffim,
    mcrincinicio,
    mcrincfim,
    mcraltinicio,
    mcraltfim,
    mcrano)
  VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')
  RETURNING mcrid
DML;
        $stmt = sprintf(
            $sql,
            $dados['mcrdsc'],
            $dados['mcrcod'],
            $dados['mcrrefinicio'],
            $dados['mcrreffim'],
            $dados['mcrincinicio'],
            $dados['mcrincfim'],
            $dados['mcraltinicio'],
            $dados['mcraltfim'],
            $mcrano
        );
    }

    if (!($mcrid = $db->pegaUm($stmt))) {
        $db->rollback();
        return false;
    }

    // -- Atualizando os tipos de creditos associados ao momento
    if (!(salvarTipoCredito($mcrid, $dados['tcrid']))) {
        $db->rollback();
        return false;
    }

    if (!$db->commit()) {
        $db->rollback();
        return false;
    }

    return true;
}

function salvarTipoCredito($mcrid, $tiposCredito)
{
    global $db;

    $sql = <<<DML
DELETE FROM altorc.momentotipocredito
  WHERE mcrid = %d
DML;
    $stmt = sprintf($sql, $mcrid);
    if (!$db->executar($stmt)) {
        return false;
    }

    $sql = <<<DML
INSERT INTO altorc.momentotipocredito(mcrid, tcrid)
  VALUES(%d, %d)
DML;
    foreach ($tiposCredito as $tpCredito) {
        $stmt = sprintf($sql, $mcrid, $tpCredito);
        if (!$db->executar($stmt)) {
            return false;
        }
    }
    return true;
}
