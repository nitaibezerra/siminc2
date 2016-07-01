<?php


// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/workflow.php";
//include APPRAIZ . "www/reuni/constantesPerfil.php";

$db = new cls_banco();

$codResposta = trim(base64_decode($_REQUEST['codPergunta']));
$tipoParecer = trim($_REQUEST['tipoParecer']);
$parecer	 = trim($_REQUEST['parecer']);
$situacao	 = trim($_REQUEST['situacao']);
$usucpf 	 = $_SESSION['usucpf'];

if (!$situacao || $situacao == 'false')
    $situacao = 'null';

if (trim($codResposta) != '' && trim($tipoParecer) != '') {
    $sql = "select count(*) as qtd from reuni.parecer where rspcod = $codResposta and tpacod = $tipoParecer";

    if ($db->pegaUm($sql) > 0) {
        $sql = "update
                    reuni.parecer
                set
                    sitcod     = $situacao,
                    usucpf     = '$usucpf',
                    pardtatual = now(),
                    pardsc     = '$parecer'
                where
                    tpacod     = $tipoParecer
                    and rspcod = $codResposta ";

        $mes = "Parecer atualizado com sucesso!";
    } else {
        $sql = "insert into reuni.parecer (sitcod,
                                           pflcod,
                                           usucpf,
                                           pardtatual,
                                           tpacod,
                                           pardsc,
                                           rspcod,
                                           unpid) values ($situacao, null, '$usucpf', now(), $tipoParecer, '$parecer', $codResposta, null)";
        $mes = "Parecer salvo com sucesso!";
    }
} else {
    $mes = "Não foi possível atualizar ou inserir o parecer!\\nCódigo de resposta ou tipo de parecer inválido!";
}


$db->executar($sql);
$db->commit();
die($mes);





