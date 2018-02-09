<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_desenvolvimento";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$_DEPARA['AUXILIAR DE ENFERMAGEM'] = 1;
$_DEPARA['ENFERMEIRO-AREA'] = 2;
$_DEPARA['MEDICO-AREA'] = 3;
$_DEPARA['TECNICO EM ENFERMAGEM'] = 4;
$_DEPARA['TECNICO DE LABORATORIO AREA'] = 5;
$_DEPARA['PSICOLOGO-AREA'] = 6;
$_DEPARA['ASSISTENTE SOCIAL'] = 7;
$_DEPARA['NUTRICIONISTA-HABILITACAO'] = 8;
$_DEPARA['FISIOTERAPEUTA'] = 9;
$_DEPARA['FARMACEUTICO-HABILITACAO'] = 10;
$_DEPARA['TECNICO EM FARMACIA'] = 12;
$_DEPARA['INSTRUMENTADOR CIRURGICO'] = 13;
$_DEPARA['FARMACEUTICO BIOQUIMICO'] = 14;
$_DEPARA['BIOMEDICO'] = 15;
$_DEPARA['DOCENTE'] = 17;
$_DEPARA['FARMACEUTICO BIOQUIMICO'] = 18;
$_DEPARA['TERAPEUTA OCUPACIONAL'] = 21;
$_DEPARA['BIOLOGO'] = 23;
$_DEPARA['FONOAUDIOLOGO'] = 24;
$_DEPARA['ODONTOLOGO -  DL 1445-76'] = 25;
$_DEPARA['AUXILIAR DE LABORATORIO'] = 25;

$lines = file("./servidoreshu.csv");

echo "TOTAL: ".count($lines)."<br>";

$i=0;
foreach($lines as $line) {
	$ds = explode(";",$line);
	
	$existe = $db->pegaUm("SELECT fcoid FROM rehuf.funcionarioplantao WHERE fcocodigosiape='".$ds[0]."'");
	
	if(!$existe) {
		if($_DEPARA[$ds[2]]) {
			
			$sql = "INSERT INTO rehuf.funcionarioplantao(
		            carid, fcocodigosiape, fconome, fcostatus, fcoins)
		    		VALUES ('".$_DEPARA[$ds[2]]."', '".$ds[0]."', '".$ds[1]."', 'A', NOW()) RETURNING fcoid";
			
			$fcoid = $db->pegaUm($sql);
			
			$sql = "INSERT INTO rehuf.funcionarioplantaohospital(fcoid, entid, ins)
    				VALUES ('".$fcoid."', '389672', NOW());";
			
			$db->executar($sql);
			
			$i++;
		
		} else {
			echo "Cargo ".$ds[2]." não foi identificado<br>";
		}
	} else {
		echo "SIAPE ".$ds[0]." ja existe<br>";
	}
	
	$db->commit();
}

echo "Realizados: ".$i."<br>";


?>