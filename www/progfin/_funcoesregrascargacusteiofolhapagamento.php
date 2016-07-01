<?php

/**
 * Funções de apoio ao gerenciamento de momentos de crédito.
 * $Id: _funcoesregrascargacusteiofolhapagamento.php 96245 2015-04-09 14:20:39Z werteralmeida $
 */

/**
 * Apaga um momento de crédito
 * @global cls_banco $db
 * @param array $dados Dados para apagar o momento de crédito.
 * @return bool
 */
function apagarRegra($dados) {
    global $db;
    $rccnomecoluna = $db->pegaUm("SELECT rccnomecoluna FROM progfin.regrascargacusteiofolhapagamento WHERE rccid = {$dados['rccid']}");
    #ver($rccnomecoluna,"SELECT rccnomecoluna FROM progfin.regrascargacusteiofolhapagamento WHERE rccid = {$dados['rccid']}",d);

    $sql = <<<DML
        DELETE FROM progfin.regrascargacusteiofolhapagamento  WHERE rccid = %d
DML;
    $stmt = sprintf($sql, $dados['rccid']);
    $db->executar($stmt);

    /* Apagando a coluna na tabela de Dados */
    $sql = "SELECT \"progfin\".\"acertacolunasdadoscusteiofolhapagamento\"('{$rccnomecoluna}', 'excluir')";
    $db->executar($sql);

    if (!$db->commit()) {
        $db->rollback();
        return false;
    }

    return true;
}

function carregarDadosRegras($dados) {
    global $db;

    $sql = <<<DML
SELECT  rcc.rccid,
        rcc.rccnomecoluna,
        rcc.rccdsccoluna,
        rcc.rccelementodespesa,
        rcc.rccrubrica,
        rcc.rcctipooperacao
  FROM progfin.regrascargacusteiofolhapagamento rcc
  WHERE rcc.rccid = %d
DML;
    $stmt = sprintf($sql, $dados['rccid']);
    if (!($dadosRegra = $db->carregar($stmt))) {
        return false;
    }

    $linhaAtual = current($dadosRegra);
    $dados['rccid'] = $linhaAtual['rccid'];
    $dados['rccdsccoluna'] = $linhaAtual['rccdsccoluna'];
    $dados['rccnomecoluna'] = $linhaAtual['rccnomecoluna'];
    $dados['rccelementodespesa'] = $linhaAtual['rccelementodespesa'];
    $dados['rccrubrica'] = $linhaAtual['rccrubrica'];
    $dados['rcctipooperacao'] = $linhaAtual['rcctipooperacao'];

    return $dados;
}

function salvarRegra($dados) {
    global $db;

    if (isset($dados['rccid']) && !empty($dados['rccid'])) {
        $sql = <<<DML
UPDATE progfin.regrascargacusteiofolhapagamento
  SET 
    rccdsccoluna= '%s',
    rccelementodespesa= '%s',
    rccrubrica= '%s'
  WHERE  rccid = %d
  RETURNING rccid
DML;
        $stmt = sprintf(
                $sql, $dados['rccdsccoluna'], $dados['rccelementodespesa'], $dados['rccrubrica'],  $dados['rccid']
        );
        if (!($db->pegaUm($stmt))) {
            $db->rollback();
            return false;
        }
    } else {
        $sql = <<<DML
INSERT INTO progfin.regrascargacusteiofolhapagamento(
        rccdsccoluna,
        rccelementodespesa,
        rccrubrica
        )
  VALUES('%s', '%s', '%s')
  RETURNING rccid
DML;
        $stmt = sprintf(
                $sql, $dados['rccdsccoluna'], $dados['rccelementodespesa'], $dados['rccrubrica']
        );

        if (!($rccid = $db->pegaUm($stmt))) {
            $db->rollback();
            return false;
        } else {
            $dados['rccid'] = $rccid;
        }
        $dados['rccnomecoluna'] = "regra_{$rccid}";
        #ver($dados,d);
        /* Cadastrando a Nova coluna na tabela de Dados */
        $sql = "SELECT \"progfin\".\"acertacolunasdadoscusteiofolhapagamento\"('{$dados['rccnomecoluna']}', 'adiciona')";
        $db->executar($sql);
        
        /* Faz um update para setar o nome da coluna */
                $sql = <<<DML
UPDATE progfin.regrascargacusteiofolhapagamento
  SET 
    rccnomecoluna = '%s'
  WHERE  rccid = %d
  RETURNING rccid
DML;
        $stmt = sprintf(
                $sql, $dados['rccnomecoluna'],  $dados['rccid']
        );
        if (!($db->pegaUm($stmt))) {
            $db->rollback();
            return false;
        }
    }


    if (!$db->commit()) {
        $db->rollback();
        return false;
    }

    return true;
}
