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

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

include "_constantes.php";
include "_funcoes_coordenadorlocal.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select * from sispacto2.identificacaousuario i 
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
		where t.pflcod=1119 and i.picid is not null";

$listacoordenadoreslocais = $db->carregar($sql);

if($listacoordenadoreslocais[0]) {
	foreach($listacoordenadoreslocais as $lcl) {
		
		$arr = array('iuscpf'            => $lcl['iuscpf'],
					 'iusnome'           => $lcl['iusnome'],
					 'iusemailprincipal' => $lcl['iusemailprincipal'],
					 'picid'             => $lcl['picid'],
					 'suscod'            => 'A',
					 'naoredirecionar'    => true);
		
		inserirCoordenadorLocalGerenciamento($arr);
		
		echo $lcl['iuscpf']."<br>";
		
		
	}
	
}

?>
