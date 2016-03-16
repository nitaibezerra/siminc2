<?php
header('Content-Type: text/html; charset=iso-8859-1');

/**
 * 
 */
include "config.inc";
/**
 * 
 */
include APPRAIZ."includes/classes_simec.inc";
/**
 * 
 */
include APPRAIZ."includes/funcoes.inc";


/**
 * Referência a obras2.hashacesso_tipo
 */
define('FOTOS_TERRENOS', 1);

$_SESSION['usucpf'] = '';
$_SESSION['sisid'] = 4;
$_SESSION['mnuid'] = 1;


/**
 * 
 * @return boolean
 */
function checaOrigemRequisicao()
{
    return true;
}

/**
 * 
 * @param string $transacao Nome da transação para verificação
 * @return int
 */
function checaTransacao($transacao)
{
    $transacao = strtoupper($transacao);
    if (defined($transacao)) {
        return constant($transacao);
    }
    return 0;
}

/**
 * 
 * @global cls_banco $db
 * @param type $preid
 * @return int
 */
function checaPreObra($preid)
{
    global $db;
    $sql = <<<DML
SELECT preid
  FROM obras.preobra pob
  WHERE pob.preid = %d
DML;
    $sql = sprintf($sql, $preid);
    return $db->pegaUm($sql);
}

function geraHash($preid, $hstid, $hship)
{
    global $db;
    // -- Componentes do MD5
    $prehash = $preid . date('Y-m-d H:i:s') . rand(0, 1000);
    $md5 = md5($prehash);

    // -- Inserção na base de dados para posterior verificação
    $sql = <<<DML
INSERT INTO obras2.hashacesso(preid, hstid, hshmd5, hship)
  VALUES(%d, %d, '%s', '%s')
DML;
    $sql = sprintf($sql, $preid, $hstid, $md5, $hship);
    $db->executar($sql);
    if (!$db->commit()) {
        imprimeRetorno(
            array('mensagem'=>'1','status' => 'ERRO')
        );
    }
    return $md5;
}

/**
 * 
 * @param type $retorno
 */
function imprimeRetorno($retorno, $closedb = false)
{
    global $db;
    if ($closedb) {
        $db->close();
    }

    print(simec_json_encode($retorno));
    exit();
}

// ----- 

$db = new cls_banco();

$transacao = $_REQUEST['transacao'];
$preid = $_REQUEST['preid'];

if (!($hship = checaOrigemRequisicao())) {
//    print "<font color='red'>Acesso negado!</font>";
    imprimeRetorno(
        array('mensagem'=>'2', 'status' => 'ERRO')
    );
}

if (!($hstid = checaTransacao($transacao))) {
//    print "<font color='red'>Transação inválida!</font>";
    imprimeRetorno(
        array('mensagem'=>'3', 'status' => 'ERRO')
    );
}

if (!checaPreObra($preid)) {
//    print "<font color='red'>Pré-obras inválido!</font>";
    imprimeRetorno(
        array('mensagem'=>'4', 'status' => 'ERRO'),
        true
    );
}

// -- Geração do novo hash
$retorno = array(
    'hash' => geraHash($preid, $hstid, $hship),
    'criacao' => date('d-m-Y'),
    'transacao' => $transacao,
    'preid' => $preid,
    'status' => 'OK'
);

$db->close();

imprimeRetorno($retorno);
