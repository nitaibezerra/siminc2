<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

require_once APPRAIZ . "www/sismedio/_constantes.php";
require_once APPRAIZ . "www/sismedio/_funcoes_escola.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select * from sismedio.listaescolasensinomedio l 
		left join seguranca.usuario u on u.usucpf = l.lemcpfgestor 
		left join workflow.documento d on d.docid = l.docid 
		where (esdid='".ESD_FLUXOESCOLA_EMELABORACAO."' OR esdid IS NULL) and lemcpfgestor is not null";

$listaescolasensinomedio = $db->carregar($sql);

if($listaescolasensinomedio[0]) {
	foreach($listaescolasensinomedio as $lee) {
		
		$arr = array('usucpf'   => $lee['lemcpfgestor'],
					 'usunome'  => $lee['lemnomegestor'],
					 'usuemail' => $lee['lememailgestor'],
					 'lemcodigoinep' => $lee['lemcodigoinep'],
					 'suscod'   => 'A',
					 'naoredirecionar' => true);
		
		inserirGestorEscolaGerenciamento($arr);
		
		echo $lee['lemcpfgestor']."<br>";
		
	}
	
}

$db->close();

?>
