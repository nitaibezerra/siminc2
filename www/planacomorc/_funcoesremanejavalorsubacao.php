<?php

function remanejamentoDeSubacao($dados) {
    global $db;

    if ($rmsid = criaTransacao($dados)) {
        // -- Decrementa pi
        criaTransacaoSubacao($rmsid, $dados['rem_sba'], '-');
        // -- Incrementa pi
        criaTransacaoSubacao($rmsid, $dados['rem_adc_sba'], '+');
    }

    // -- Aplicando alterações de valores do PI
    if ('E' == $dados['tipotransacao']) {
        incrementaSubacao($dados['ptrid'], $dados['rem_adc_sba']);
        decrementaSubacao($dados['ptrid'], $dados['rem_sba']);
    }

    // -- Concluíndo a transação de origem
    if (!empty($dados['rmsidorigem'])) {
        concluiTransacaoOrigem($dados['rmsidorigem']);
    }

    if ($db->commit()) {

        // -- Envio de e-mail durante a concretização das transações
        if ('E' == $dados['tipotransacao']) {
            // -- Notificando o gestor de subações
            notificarGestoresDaSubacao($dados['ptrid'], $dados['rem_adc_sba'], $dados['rem_sba']);
        }

        return true;
    }
    $db->rollback();
    return false;
}

function notificarGestoresDaSubacao($ptrid, $adicoes, $reducoes) {
    if (!$adicoes) {
        $adicoes = array();
    }
    if (!$reducoes) {
        $reducoes = array();
    }

    $adicoes = array_filter($adicoes);
    $reducoes = array_filter($reducoes);
    $subacoesModificadas = array_merge(array_keys($adicoes), array_keys($reducoes));
    $gestores = carregarGestoresDeSubacao($subacoesModificadas, $gestoresDesagrupados);

    if (!$gestores) {
        $gestores = array();
    }

    $emailHeader = <<<EMAIL
Caro gestor,
<p style="padding-left:25px">As seguintes subações sob sua gestão foram remanejadas:</p>
<ul>
EMAIL;
    $emailFooter = <<<EMAIL
</ul>
<p style="padding-left:25px">Para informações completas sobre este(s) remanejamento(s), acesse o módulo Planejamento e
Acompanhamento Orçamentário no <a href="http://simec.mec.gov.br">SIMEC</a>.</p>
<p style="font-size:9px">Este é um e-mail automático, favor não responder.</p>
EMAIL;

    foreach ($gestores as $nomeGestor => $dadosGestor) {
        $emailGestor = $dadosGestor['usuemail'];
        $emailBody = '';
        foreach ($dadosGestor['subacoes'] as $subacao) {
            // -- Verifica se a subação está no array de acréscimos
            if (in_array($subacao['sbaid'], array_keys($adicoes))) {
                $emailBody .= <<<BODY
  <li>{$subacao['sbacod']} sofreu um acréscimo de R$ {$adicoes[$subacao['sbaid']]}</li>
BODY;
            }
            // -- Verifica se a subação está no array de decrécimos
            if (in_array($subacao['sbaid'], array_keys($reducoes))) {
                $emailBody .= <<<BODY
  <li>{$subacao['sbacod']} sofreu um decréscimo de R$ {$reducoes[$subacao['sbaid']]}</li>
BODY;
            }
        }
        // -- Manda e-mail para o gestor da subacao
        enviar_email(
                array('nome' => 'simec@mec.gov.br', 'email' => 'simec@mec.gov.br'), array('nome' => $nomeGestor, 'email' => $emailGestor), 'Remanejamento de Subações', $emailHeader . $emailBody . $emailFooter
        );
    }
}

function carregarGestoresDeSubacao($subacoes, &$gestoresDesagrupados) {
    global $db;
    $sql = <<<DML
SELECT DISTINCT urp.id_subacao AS sbaid,
                sba.codigo || ' - ' || sba.sigla as sbacod,
                usu.usunome,
                usu.usuemail
  FROM planacomorc.usuarioresponsabilidade urp
    INNER JOIN seguranca.usuario usu USING(usucpf)
    INNER JOIN planacomorc.subacao sba USING(id_subacao)
  WHERE id_subacao IN(%s)
DML;
    if (!is_array($subacoes) || empty($subacoes)) {
        $subacoes = array(-1);
    }
    $query = sprintf($sql, implode(', ', $subacoes));
    $gestoresDesagrupados = $db->carregar($query);
    $gestoresAgrupados = array();
    if (is_array($gestoresAgrupados) && count($gestoresAgrupados) > 0) {
        foreach ($gestoresDesagrupados as $dadosGestor) {
            if (!isset($gestoresAgrupados[$dadosGestor['usunome']])) {
                $gestoresAgrupados[$dadosGestor['usunome']] = array(
                    'usuemail' => $dadosGestor['usuemail'],
                    'subacoes' => array()
                );
            }
            $gestoresAgrupados[$dadosGestor['usunome']]['subacoes'][] = array(
                'sbaid' => $dadosGestor['sbaid'],
                'sbacod' => $dadosGestor['sbacod']
            );
        }
    }
    return $gestoresAgrupados;
}

function criaTransacao($dados) {
    global $db;
    
    $sql = <<<DML
INSERT INTO planacomorc.remanejamentosubacao(
    tipotransacao,
    ptrid,
    funcprogramatica,
    po,
    podsc,
    ptrdotacao,
    ptrempenhado,
    ptrsaldo,
    detalhadosubacao,
    usucpf,
    rmpsaldosubtraido,
    rmpsaldoadicionado,
    rmsidorigem,
    dscalteracao,
    dscjustificativa,
    dscexecucao
) VALUES ('%s', %d, '%s', '%s', '%s', %f, %f, %f, %f, '%s', %f, %f, %d, '%s', '%s', '%s')
RETURNING rmsid
DML;
     $stmt= sprintf(
            $sql, $dados['tipotransacao'], $dados['ptrid'], $dados['funcprogramatica'], $dados['po'], $dados['podsc'], $dados['ptrdotacao'], $dados['ptrempenhado'], $dados['ptrsaldo'], $dados['detalhadosubacao'], $_SESSION['usucpf'], $dados['rmpsaldosubtraido'], $dados['rmpsaldoadicionado'], empty($dados['rmsidorigem']) ? 'null' : $dados['rmsidorigem'], $dados['dscalteracao'], $dados['dscjustificativa'], $dados['dscexecucao']
    );
    #ver($stmt,d);
    return $db->pegaUm($stmt);
}

function criaTransacaoSubacao($rmsid, &$subacoes, $tipomovimento) {
    global $db;
    // -- Grava a transação de cada PI
    $sql = <<<DML
INSERT INTO planacomorc.rmsmovimentacao(rmsid, tipomovimento, sbaid, vlrmovimento)
  VALUES(%d, '%s', %d, %f)
DML;

    if (!empty($subacoes)) {
        foreach ($subacoes as $sbaid => &$valor) {
            $valor = str_replace(array('.', ','), array('', '.'), $valor);
            if ($valor != 0.00) {
                $stmt = sprintf($sql, $rmsid, $tipomovimento, $sbaid, $valor);
                $db->executar($stmt);
            }
        }
    }
}

function incrementaSubacao($ptrid, $subacoes) {
    global $db;
    $sqlCheck = <<<DML
SELECT COUNT(1)
  FROM monitora.pi_subacaodotacao
  WHERE sbaid = %d
    AND ptrid = %d
DML;
    $sqlInsert = <<<DML
INSERT INTO monitora.pi_subacaodotacao(sbaid, ptrid, sadvalor)
  VALUES(%d, %d, %f)
DML;
    $sqlUpdate = <<<DML
UPDATE monitora.pi_subacaodotacao
  SET sadvalor = sadvalor + %f
  WHERE ptrid = %d
    AND sbaid = %d
DML;
    if (!empty($subacoes)) {
        foreach ($subacoes as $sbaid => $valor) {
            if ($valor != 0.00) {
                $stmtCheck = sprintf($sqlCheck, $sbaid, $ptrid);
                if ($db->pegaUm($stmtCheck)) {
                    $stmt = sprintf($sqlUpdate, $valor, $ptrid, $sbaid);
                } else {
                    $stmt = sprintf($sqlInsert, $sbaid, $ptrid, $valor);
                }
                $db->executar($stmt);
            }
        }
    }
}

function decrementaSubacao($ptrid, $subacoes) {
    global $db;
    $sql = <<<DML
UPDATE monitora.pi_subacaodotacao
  SET sadvalor = sadvalor - %f
  WHERE ptrid = %d
    AND sbaid = %d
DML;

    if (!empty($subacoes)) {
        foreach ($subacoes as $sbaid => $valor) {
            $stmt = sprintf($sql, $valor, $ptrid, $sbaid);
            $db->executar($stmt);
        }
    }
}

function concluiTransacaoOrigem($rmsidorigem) {
    global $db;
    $sql = <<<DML
UPDATE planacomorc.remanejamentosubacao
  SET rmpstprocessado = TRUE
  WHERE rmsid = %d
DML;
    $stmt = sprintf($sql, $rmsidorigem);
    $db->executar($stmt);
}

function excluiTransacaoRms($rmsid) {
    global $db;
    if (is_array($rmsid)) {
        $rmsid = $rmsid['rmsid'];
    }
    if (trim($rmsid) == '') {
        echo 'false';
        die();
    }

    $sql = <<<DML
DELETE
  FROM planacomorc.rmsmovimentacao
  WHERE rmsid = %d
DML;
    $stmt = sprintf($sql, $rmsid);
    try {
        $db->executar($stmt);
    } catch (Exception $ex) {
        echo $ex->getMessage();
        die();
    }

    $sql = <<<DML
DELETE
  FROM planacomorc.remanejamentosubacao
  WHERE rmsid = %d
DML;

    $stmt = sprintf($sql, $rmsid);
    try {
        $db->executar($stmt);
        $db->commit();
        echo 'true';
    } catch (Exception $ex) {
        echo 'false';
    }
}

