<?php 
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$memorandoRepositorio = new MemorandoRepositorio();
$validacao = false;
if ($_POST['memo']==""){
	$validacao = $memorandoRepositorio->verificaSeMemorandoExisteNaBase($_POST['numeroMemorando']);
} 
if ($_POST['memo']!=""){
	$memorandoRecuperado = $memorandoRepositorio->recuperePeloNumeroMemorando($_POST['numeroMemorando']);
	if(!is_null($memorandoRecuperado)){
		if ($memorandoRecuperado->getId() != null && $memorandoRecuperado->getId() != $_POST['memo']){
			$validacao = true;
		}
	}
}
print simec_json_encode($validacao);