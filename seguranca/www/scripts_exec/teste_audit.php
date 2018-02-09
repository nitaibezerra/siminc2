<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "4024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

//$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

$db = new cls_banco();

if($_REQUEST['parar1']) {
	echo '<pre>';
	print_r($configDbAuditoria);
	exit;
}

$sql_audit = "insert into auditoria (usucpf, mnuid, audsql, auddata, audtabela, audtipo, audip, sisid, audscript) values ('91112796134', 4, 'sql_teste', '".date('Y-m-d H:i:s')."', 'teste.teste', 'I', '".$_SERVER["REMOTE_ADDR"]."','teste', '".pg_escape_string($_SERVER['REQUEST_URI'])."')";
//pg_query(self::$link[$this->nome_bd], $sql_audit);

adapterConnection::auditoria()->auditar($sql_audit);


$db->close();
