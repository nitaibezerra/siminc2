<?php

$memcache_obj = memcache_connect("tcp://10.1.3.157:11212?persistent=0&amp;weight=1&amp;timeout=1&amp;retry_interval=15", 11211);
if($_REQUEST['limpafila']) {
	memcache_set($memcache_obj, 'filalogado', array(), 0, 0);
	memcache_set($memcache_obj, 'filaespera', array(), 0, 0);
	exit;
}



include APPRAIZ . "includes/classes/dateTime.inc";
function kickDelay($fila,$tempo) {

	$data = new Data();
	foreach($fila as $cpf => $dt) {
		$retorno = $data->diferencaEntreDatas( $dt, date("Y-m-d h:i:s"), 'tempoEntreDadas', 'array','');
		print_r($retorno);
		if($retorno['minutos'] >= $tempo) {
			unset($fila[$cpf]);
		}
	}
	return $fila;
	
}



$filaLogado = memcache_get($memcache_obj, 'filalogado');
$filaEspera = memcache_get($memcache_obj, 'filaespera');

echo "<pre>";
echo "Eu sou:".$_SESSION['usunome'];
$filaLogado = kickDelay($filaLogado,2);
memcache_set($memcache_obj, 'filalogado', $filaLogado, 0, 0);

print_r($filaLogado);
print_r($filaEspera);

if(!$filaLogado[$_SESSION['usucpf']]) {
	
	if(count($filaLogado) > 0) {
		if(!$filaEspera[$_SESSION['usucpf']]) {
			$filaEspera[$_SESSION['usucpf']] = date("Y-m-d h:i:s");
			memcache_set($memcache_obj, 'filaespera', $filaEspera, 0, 0);
		}
		
		die("<p>Você é o número xx da fila.</p><script>setTimeout( \"window.location='pdeinterativo.php?modulo=inicio&acao=C'\", 5000 );</script>");
	} else {
		
		if($filaEspera) {
			echo "eeeee".key($filaEspera)."<br>";
			if(key($filaEspera)==$_SESSION['usucpf']) {
				$filaLogado[$_SESSION['usucpf']] = date("Y-m-d h:i:s");
				memcache_set($memcache_obj, 'filalogado', $filaLogado, 0, 0);
				unset($filaEspera[$_SESSION['usucpf']]);
				memcache_set($memcache_obj, 'filaespera', $filaEspera, 0, 0);
			} else {
				die("<p>Você é o número xx22222222222 da fila.</p><script>setTimeout( \"window.location='pdeinterativo.php?modulo=inicio&acao=C'\", 5000 );</script>");				
			}

		} else {
			$filaLogado[$_SESSION['usucpf']] = date("Y-m-d h:i:s");
			memcache_set($memcache_obj, 'filalogado', $filaLogado, 0, 0);
		}

	}
} else {
	$filaLogado[$_SESSION['usucpf']] = date("Y-m-d h:i:s");
	memcache_set($memcache_obj, 'filalogado', $filaLogado, 0, 0);
}

?>