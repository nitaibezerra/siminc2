<?php

/**
 * Funções de apoio ao questionário.
 * $Id: _funcoesquestionario.php 90842 2014-11-24 12:42:05Z lindalbertofilho $
 */

/**
 *
 * @global resource $db
 * @param type $dados
 */
function inserirQuestionario($dados) {

    global $db;
    $sql = "INSERT INTO planacomorc.monquestionario(
            qstnome, qstnumcaracteres, id_periodo_referencia, qststatus, dtcriacao, dtalteracao,
            cpfalteracao)
    		VALUES ('" . $dados['qstnome'] . "', NULL, '" . $dados['id_periodo_referencia'] . "', 'A', NOW(), NOW(),
            		'" . $_SESSION['usucpf'] . "') RETURNING qstid;";

    $qstid = $db->pegaUm($sql);

    if ($dados['id_acao_programatica']) {
        foreach ($dados['id_acao_programatica'] as $aca) {

            $sql = "INSERT INTO planacomorc.monqstacaperiodo(
            				qstid, id_acao_programatica)
    						VALUES ('" . $qstid . "', '" . $aca . "');";

            $db->executar($sql);
        }
    }

    $db->commit();

    carregarQuestionario(array('qstid' => $qstid, 'naoredirecionar' => true));

    $al = array("alert" => "Questionário inserido com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=cadastrarperguntas");
    alertlocation($al);
}

function carregarQuestionario($dados) {
    global $db;

    $sql = "SELECT * FROM planacomorc.monquestionario WHERE qstid='" . $dados['qstid'] . "'";
    $monquestionario = $db->pegaLinha($sql);

    if ($monquestionario) {
        $_SESSION['planacomorc']['qstid'] = $monquestionario['qstid'];
        $_SESSION['planacomorc']['qstnome'] = $monquestionario['qstnome'];
    }

    if (!$dados['naoredirecionar']) {
        $al = array("location" => "planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=dados");
        alertlocation($al);
    }
}

function montaAbasQuestionario($abaativa = null) {
    global $db;

    $menu[] = array("id" => 1, "descricao" => "Lista Questionário", "link" => "/planacomorc/planacomorc.php?modulo=principal/questionario/listaquestionario&acao=A");
    $menu[] = array("id" => 2, "descricao" => "Dados", "link" => "/planacomorc/planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=dados");
    $menu[] = array("id" => 3, "descricao" => "Cadastrar perguntas", "link" => "/planacomorc/planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=cadastrarperguntas");

    

    echo montarAbasArray($menu, $abaativa);
}

function atualizarQuestionario($dados) {
    global $db;
#ver($dados,d);
    $sql = "UPDATE planacomorc.monquestionario
			SET
			    qstnome='" . $dados['qstnome'] . "',
			    id_periodo_referencia='" . $dados['id_periodo_referencia'] . "',
			    dtalteracao=NOW(),
			    cpfalteracao='" . $_SESSION['usucpf'] . "'
			WHERE
			    qstid='" . $_SESSION['planacomorc']['qstid'] . "'";

    $db->executar($sql);
    $db->executar("DELETE FROM planacomorc.monqstacaperiodo WHERe qstid='" . $_SESSION['planacomorc']['qstid'] . "'");

    if (is_array($dados['id_acao_programatica'])) {
        foreach ($dados['id_acao_programatica'] as $aca) {
            if ($aca <> '') {
                $sql = "INSERT INTO planacomorc.monqstacaperiodo(
            				qstid, id_acao_programatica)
    						VALUES ('" . $_SESSION['planacomorc']['qstid'] . "', '" . $aca . "');";
                $db->executar($sql);
            }
        }
    }

    $db->commit();
    $al = array("alert" => "Questionário atualizado com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=dados");
    alertlocation($al);
}

function excluirQuestionario($dados) {
    global $db;
    $sql = "UPDATE planacomorc.monquestionario SET qststatus='I' WHERE qstid='" . $dados['qstid'] . "'";
    $db->executar($sql);
    $db->commit();

    $al = array("alert" => "Questionário excluído com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionario/listaquestionario&acao=A");
    alertlocation($al);
}

/**
 * 
 * @param array $dados
 * @return void(0)
 */
function salvarPergunta(array $dados) {

    if (isset($dados['mqpid']) && is_numeric($dados['mqpid'])) {
        alterarPergunta($dados);
    } else {
        inserirPerguntas($dados);
    }
}

function inserirPerguntas($dados) {
    global $db;

    $sql = <<<DML
INSERT INTO planacomorc.monqstperguntas(qstid, mqpdescricao, mqprespnumcaracteres, mqpfacultativo, mqpresppadrao,mqpordem)
  VALUES (
    '{$_SESSION['planacomorc']['qstid']}',
    '{$dados['mqpdescricao']}',
    '{$dados['mqprespnumcaracteres']}',
    {$dados['mqpfacultativo']},
    '{$dados['mqpresppadrao']}',
    '{$dados['mqpordem']}'
  )
DML;

    $db->executar($sql);
    $sql = "UPDATE planacomorc.monquestionario
              SET qstnumcaracteres = (SELECT coalesce(sum(length(mqpdescricao)+mqprespnumcaracteres)+1,0) as tot
                                        FROM planacomorc.monqstperguntas
                                        WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "')
              WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "'";

    $db->executar($sql);
    $db->commit();
    $al = array("alert" => "Perguntas inseridas com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=cadastrarperguntas");
    alertlocation($al);
}

/**
 * 
 * @global resource $db
 * @param array $dados
 */
function alterarPergunta(array $dados) {
    global $db;

    $allowKeys = array_flip(array('mqpdescricao', 'mqprespnumcaracteres', 'mqpfacultativo', 'mqpresppadrao','mqpordem'));
    $sql = "UPDATE planacomorc.monqstperguntas SET ";

    foreach ($dados as $key => $value) {
        if (array_key_exists($key, $allowKeys)) {
            if (!empty($value)) {
                $sql .= "{$key} = '{$value}',";
            }
        }
    }

    $sql = substr($sql, 0, -1);
    $sql.= " WHERE mqpid = '{$dados['mqpid']}'";
    $db->executar($sql);

    $sql = "UPDATE planacomorc.monquestionario
              SET qstnumcaracteres = (SELECT coalesce(sum(length(mqpdescricao)+mqprespnumcaracteres)+1,0) as tot
                                        FROM planacomorc.monqstperguntas
                                        WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "')
              WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "'";

    $db->executar($sql);
    $db->commit();

    alertlocation(array(
        'alert' => 'Perguntas alterada com sucesso',
        'location' => 'planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 * Rotina para buscar dados da pergunta para alterar
 * @global resource $db
 * @param array $post
 */
function carregaPergunta(array $post) {
    global $db;

    $strSQL = "SELECT * FROM planacomorc.monqstperguntas WHERE mqpid = '%s' AND qstid = '%s' LIMIT 1";
    $strSQL = sprintf($strSQL, (int) $post['mqpid'], (int) $post['qstid']);
    $row = $db->pegaLinha($strSQL);

    if ($row)
        return $row;

    alertlocation(array(
        'alert' => 'Pergunta selecionada nao foi carregada, tente novamente',
        'location' => 'planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 * Apaga uma pergunta do questionário
 * @param int $mqpid
 */
function excluiPergunta($mqpid) {
    if (is_numeric($mqpid)) {
        global $db;

        $sql = "DELETE FROM planacomorc.monqstperguntas WHERE mqpid = '%s'";
        $sql = sprintf($sql, (int) $mqpid);

        $db->executar($sql);
        $db->commit();

        alertlocation(array(
            'alert' => 'Pergunta excluída com sucesso',
            'location' => 'planacomorc.php?modulo=principal/questionario/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
        ));
    }
}

?>