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

$sql = "SELECT 
			codigo,
		  	--empenho,
		  	quantidade,
		  	to_char(dataini, 'DD/MM/YYYY HH24:MI:SS') as dataini,
		  	to_char(datafim, 'DD/MM/YYYY HH24:MI:SS') as datafim,
		  	tempoexec,
		  	sistema
		FROM 
		  	par.empenho_temp t
		ORDER BY t.sistema";

monta_titulo('Lista de Empenhos', $linha2);
$cabecalho = array("Codigo","Quantidade","Data Ini","Data Fim","Tempo","Sistema");
$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

$sql = "SELECT 
		  	codigo,
		  	--pagamento,
		  	quantidade,
		  	to_char(dataini, 'DD/MM/YYYY HH24:MI:SS') as dataini,
		  	to_char(datafim, 'DD/MM/YYYY HH24:MI:SS') as datafim,
		  	tempoexec,
		  	sistema
		FROM 
		  	par.pagamento_temp
		ORDER BY sistema";

monta_titulo('Lista de Pagamentos', $linha2);
$cabecalho = array("Codigo","Quantidade","Data Ini","Data Fim","Tempo","Sistema");
$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

$db->close();
