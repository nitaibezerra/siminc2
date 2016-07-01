<?

function inserirQuestionario($dados) {
    global $db;
    $sql = "INSERT INTO planacomorc.monsubacaquestionario(
            qstnome, qstnumcaracteres, id_periodo_referencia, qststatus, dtcriacao, dtalteracao,
            cpfalteracao)
    		VALUES ('" . $dados['qstnome'] . "', NULL, '" . $dados['percod'] . "', 'A', NOW(), NOW(),
            		'" . $_SESSION['usucpf'] . "') RETURNING qstid;";

    $qstid = $db->pegaUm($sql);

    if ($dados['id_subacoes']) {
        foreach ($dados['id_subacoes'] as $aca) {

            $sql = "INSERT INTO planacomorc.monqstsubacaperiodo(
            				qstid, id_subacao)
    						VALUES ('" . $qstid . "', '" . $aca . "');";

            $db->executar($sql);
        }
    }

    $db->commit();

    carregarQuestionario(array('qstid' => $qstid, 'naoredirecionar' => true));

    $al = array("alert" => "Questionário inserido com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=cadastrarperguntas");
    alertlocation($al);
}

function carregarQuestionario($dados) {
    global $db;

    $sql = "SELECT * FROM planacomorc.monsubacaquestionario WHERE qstid='" . $dados['qstid'] . "'";
    $monsubacaquestionario = $db->pegaLinha($sql);

    if ($monsubacaquestionario) {
        $_SESSION['planacomorc']['qstid'] = $monsubacaquestionario['qstid'];
        $_SESSION['planacomorc']['qstnome'] = $monsubacaquestionario['qstnome'];
    }


    if (!$dados['naoredirecionar']) {
        $al = array("location" => "planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=dados");
        alertlocation($al);
    }
}

function montaAbasQuestionario($abaativa = null) {
    global $db;

    $menu[] = array("id" => 1, "descricao" => "Lista Questionário", "link" => "/planacomorc/planacomorc.php?modulo=principal/questionariosubacoes/listaquestionario&acao=A");
    $menu[] = array("id" => 2, "descricao" => "Dados", "link" => "/planacomorc/planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=dados");
    $menu[] = array("id" => 3, "descricao" => "Cadastrar perguntas", "link" => "/planacomorc/planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=cadastrarperguntas");

    //echo "<br>";

    echo montarAbasArray($menu, $abaativa);
}

function atualizarQuestionario($dados) {
    global $db;

    $sql = "UPDATE planacomorc.monsubacaquestionario
			SET qstnome='" . $dados['qstnome'] . "', id_periodo_referencia='" . $dados['percod'] . "', dtalteracao=NOW(), cpfalteracao='" . $_SESSION['usucpf'] . "'
			WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "'";
    $db->executar($sql);
    $db->executar("DELETE FROM planacomorc.monqstsubacaperiodo WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "'");
    if ($dados['id_subacoes']) {
        foreach ($dados['id_subacoes'] as $aca) {
            $sql = "INSERT INTO planacomorc.monqstsubacaperiodo(
            				qstid, id_subacao)
    						VALUES ('" . $_SESSION['planacomorc']['qstid'] . "', '" . $aca . "');";
            $db->executar($sql);
        }
    }

    $db->commit();
    $al = array("alert" => "Questionário atualizado com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=dados");
    alertlocation($al);
}

function excluirQuestionario($dados) {
    global $db;
    $sql = "UPDATE planacomorc.monsubacaquestionario SET qststatus='I' WHERE qstid='" . $dados['qstid'] . "'";
    $db->executar($sql);
    $db->commit();

    $al = array("alert" => "Questionário excluído com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionariosubacoes/listaquestionario&acao=A");
    alertlocation($al);
}

/**
 *
 * @param array $dados
 */
function salvarPergunta(array $dados) {

    if (isset($dados['mqpid']) && is_numeric($dados['mqpid'])) {
        alterarPergunta($dados);
    } else {
        inserirPerguntas($dados);
    }
}

/**
 *
 * @global resource $db
 * @param array $dados
 */
function inserirPerguntas(array $dados) {
    global $db;
    $dados['mqpfacultativo'] = $dados['mqpfacultativo'] ? $dados['mqpfacultativo'] : 2;
    $sql = "INSERT INTO planacomorc.monqstsubacaperguntas(qstid, mqpdescricao, mqprespnumcaracteres, mqpfacultativo)
              VALUES ('" . $_SESSION['planacomorc']['qstid'] . "', '" . $dados['mqpdescricao'] . "', '" . $dados['mqprespnumcaracteres'] . "', {$dados['mqpfacultativo']});";

    $db->executar($sql);
    $sql = "UPDATE planacomorc.monsubacaquestionario
              SET qstnumcaracteres = (SELECT coalesce(sum(length(mqpdescricao)+mqprespnumcaracteres)+1,0) as tot
                                        FROM planacomorc.monqstsubacaperguntas
                                        WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "')
              WHERE qstid='" . $_SESSION['planacomorc']['qstid'] . "'";

    $db->executar($sql);
    $db->commit();

    alertlocation(array(
        'alert' => 'Perguntas inseridas com sucesso',
        'location' => 'planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 *
 * @global resource $db
 * @param array $dados
 */
function alterarPergunta(array $dados) {
    global $db;

    $allowKeys = array_flip(array('mqpdescricao', 'mqprespnumcaracteres', 'mqpfacultativo'));
    $sql = "UPDATE planacomorc.monqstsubacaperguntas SET ";

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
                                        FROM planacomorc.monqstsubacaperguntas
                                        WHERE qstid='".$_SESSION['planacomorc']['qstid']."')
              WHERE qstid='".$_SESSION['planacomorc']['qstid']."'";

    $db->executar($sql);
    $db->commit();

    alertlocation(array(
        'alert' => 'Perguntas alterada com sucesso',
        'location' => 'planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 * Rotina para buscar dados da pergunta para alterar
 * @global resource $db
 * @param array $post
 */
function carregaPergunta(array $post) {
    global $db;

    $strSQL = "SELECT * FROM planacomorc.monqstsubacaperguntas WHERE mqpid = '%s' AND qstid = '%s' LIMIT 1";
    $strSQL = sprintf($strSQL, (int)$post['mqpid'], (int)$post['qstid']);
    $row = $db->pegaLinha($strSQL);

    if ($row)
        return $row;

    alertlocation(array(
        'alert' => 'Pergunta selecionada nao foi carregada, tente novamente',
        'location' => 'planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 * Apaga uma pergunta do questionário
 * @param int $mqpid
 */
function excluiPergunta($mqpid) {
    if (is_numeric($mqpid)) {
        global $db;

        $sql = "DELETE FROM planacomorc.monqstsubacaperguntas WHERE mqpid = '%s'";
        $sql = sprintf($sql, (int)$mqpid);

        $db->executar($sql);
        $db->commit();

        alertlocation(array(
            'alert' => 'Pergunta excluída com sucesso',
            'location' => 'planacomorc.php?modulo=principal/questionariosubacoes/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
        ));
    }
}

?>