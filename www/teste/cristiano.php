<?php
ini_set("memory_limit", "1024M");
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

header( 'Content-Type: text/plain' );

restore_error_handler();
restore_exception_handler();
error_reporting( E_ALL );


# abre conexão com o banco
$nome_bd     = 'dbsimec';
$servidor_bd = 'mecsrv152';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpsegurancasimec';
$db          = new cls_banco();

	# captura os dados do simec
	$sql = sprintf(
		"select audsql||';' as sql from seguranca.auditoria5 where auddata >= '2009-03-16 11:03:08' order by auddata, usucpf, audtipo"
	);
	$RS = $db->carregar($sql);
	$nlinhas = $RS ? count($RS) : 0;
	for ($i=0;$i<$nlinhas;$i++){
		foreach($RS[$i] as $k=>$v) {
		print str_replace(chr(9),'',str_replace(chr(10),'',str_replace(chr(13), ' ',$v))) . chr(10) . chr(13);
		}
	}

	//pg_set_client_encoding($db->link,'UTF-8');
	//$db->monta_lista_simples($sql,"",500000,500000);
	//$db->sql_to_excel($sql,'obras');

?>