<?

function inserirQuestionario($dados) {
    global $db;
    $sql = "INSERT INTO planacomorc.tcuquestionario(
            qtdexercicio, tqtdsc, tqtestado)
    		VALUES ('".$dados['qtdexercicio']."','". $dados['tqtdsc'] . "', 'A') RETURNING tqtid;";

    $tqtid = $db->pegaUm($sql);    

    $db->commit();

    carregarQuestionario(array('tqtid' => $tqtid, 'naoredirecionar' => true));

    $al = array("alert" => "Questionário inserido com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=cadastrarperguntas");
    alertlocation($al);
}

function carregarQuestionario($dados) {	
    global $db;
	
    $sql = "SELECT * FROM planacomorc.tcuquestionario WHERE tqtid='" . $dados['tqtid'] . "'";
    $monsubacaquestionario = $db->pegaLinha($sql);
    if ($monsubacaquestionario) {
        $_SESSION['planacomorc']['tqtid'] = $monsubacaquestionario['tqtid'];
    }


    if (!$dados['naoredirecionar']) {
        $al = array("location" => "planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=dados");
        alertlocation($al);
    }
}

function montaAbasQuestionario($abaativa = null) {
    global $db;

    $menu[] = array("id" => 1, "descricao" => "Lista Questionário","link" => "/planacomorc/planacomorc.php?modulo=principal/questionariorelatoriogestao/listaquestionario&acao=A");
	$menu[] = array("id" => 2, "descricao" => "Questionário","link" => "/planacomorc/planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=dados");
	if(null != $_SESSION['planacomorc']['tqtid'])
	$menu[] = array("id" => 3, "descricao" => "Cadastrar perguntas", "link" => "/planacomorc/planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=cadastrarperguntas");

    //echo "<br>";

    echo montarAbasArray($menu, $abaativa);
}

function atualizarQuestionario($dados) {
    global $db;

    $sql = "UPDATE planacomorc.tcuquestionario
			SET tqtdsc='" . $dados['tqtdsc'] . "', qtdexercicio='" . $dados['qtdexercicio'] . "'
			WHERE tqtid='" . $_SESSION['planacomorc']['tqtid'] . "'";
    $db->executar($sql);
    

    $db->commit();
    $al = array("alert" => "Questionário atualizado com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=dados");
    alertlocation($al);
}

function excluirQuestionario($dados) {
    global $db;
    $sql = "UPDATE planacomorc.tcuquestionario SET tqtestado='I' WHERE tqtid='" . $dados['tqtid'] . "'";
    $db->executar($sql);
    $db->commit();

    $al = array("alert" => "Questionário excluído com sucesso",
        "location" => "planacomorc.php?modulo=principal/questionariorelatoriogestao/listaquestionario&acao=A");
    alertlocation($al);
}

/**
 * 
 * @param array $dados
 */
function salvarPergunta(array $dados) {
    
    if (isset($dados['tqpid']) && is_numeric($dados['tqpid'])) {
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

    $sql = "INSERT INTO planacomorc.tcuquestpergunta(tqtid, tqppergunta, tqpestado)
              VALUES ('" . $_SESSION['planacomorc']['tqtid'] . "', '" . $dados['tqppergunta'] . "', 'A');";

    $db->executar($sql);
    $db->commit();
    
    alertlocation(array(
        'alert' => 'Pergunta inserida com sucesso',
        'location' => 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 * 
 * @global resource $db
 * @param array $dados
 */
function alterarPergunta(array $dados) {
    global $db;
    
    $allowKeys = array_flip(array('tqppergunta'));
    $sql = "UPDATE planacomorc.tcuquestpergunta SET tqppergunta = '".$dados['tqppergunta']."' WHERE tqpid = '{$dados['tqpid']}'";   
    
    $db->executar($sql);
    $db->commit();
    
    alertlocation(array(
        'alert' => 'Pergunta alterada com sucesso',
        'location' => 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 * Rotina para buscar dados da pergunta para alterar
 * @global resource $db
 * @param array $post
 */
function carregaPergunta(array $post) {
    global $db;
    
    $strSQL = "SELECT * FROM planacomorc.tcuquestpergunta WHERE tqpid = '%s' AND tqtid = '%s' LIMIT 1";
    $strSQL = sprintf($strSQL, (int)$post['tqpid'], (int)$post['tqtid']);
    $row = $db->pegaLinha($strSQL);
    
    if ($row)
        return $row;
    
    alertlocation(array(
        'alert' => 'Pergunta selecionada nao foi carregada, tente novamente',
        'location' => 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
    ));
}

/**
 * Apaga uma pergunta do questionário
 * @param int $mqpid
 */
function excluiPergunta($tqpid) {
    if (is_numeric($tqpid)) {
        global $db;
        
        $sql = "UPDATE planacomorc.tcuquestpergunta SET tqpestado = 'I' WHERE tqpid = '%s'";
        $sql = sprintf($sql, (int)$tqpid);

        $db->executar($sql);
        $db->commit();
        
        alertlocation(array(
            'alert' => 'Pergunta excluída com sucesso',
            'location' => 'planacomorc.php?modulo=principal/questionariorelatoriogestao/gerenciarquestionario&acao=A&aba=cadastrarperguntas'
        ));
    }
}

?>