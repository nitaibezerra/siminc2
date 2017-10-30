<?php
set_time_limit(0);

include "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "public/classes/Model/Omc.inc";

$ano = 2017;

$file = fopen('omc/' . $ano . '.csv', 'r');

$count = 0;
while (($line = fgetcsv($file, 0, ';')) !== false)
{
	if(!$count++){ continue; }

	$omc = new Public_Model_Omc();

	if($ano == 2017){
        $line = array_map('utf8_decode', $line);
    }

    $line = array_map('trim', $line);

	$omc->segmento = $line[0];
	$omc->paragrafo2 = $line[1];
	$omc->nome = str_replace(['?'], ['-'], $line[2]);
	$omc->nome_artistico = str_replace(['?'], ['-'], $line[3]);
	$omc->indicacao = $line[4];
	$omc->sexo = $line[5];
	$omc->endereco = str_replace(['?'], ['-'], $line[6]);
	$omc->cep = trim(str_replace(['?', '.', '(', ')'], '', $line[7]));
	$omc->telefone_residencial = trim(str_replace(['?', '.', '(', ')'], '', $line[8]));
	$omc->telefone_celular = trim(str_replace(['?', '.', '(', ')'], '', $line[9]));
	$omc->email = $line[10];
	$omc->justificativa = str_replace(['?'], ['-'], $line[11]);
	$omc->paragrafo13 = $line[12];
	$omc->nome_indicou = str_replace(['?'], ['-'], $line[13]);
	$omc->sexo_indicou = $line[14];
	$omc->endereco_indicou = str_replace(['?'], ['-'], $line[15]);
	$omc->cep_indicou = trim(str_replace(['?', '.', '(', ')'], '', $line[16]));
	$omc->telefone_residencial_indicou = trim(str_replace(['?', '.', '(', ')'], '', $line[16]));
	$omc->telefone_celular_indicou = trim(str_replace(['?', '.', '(', ')'], '', $line[18]));
	$omc->email_indicou = $line[19];

	$omc->ano = $ano;

    $omc->salvar();
    $omc->commit();

    unset($omc);
}
fclose($file);

echo "$count linhas inseridas.";

//$omc = new Public_Model_Omc();
//$a2016 = $omc->recuperarTodos('*', ["ano = 2016"]);
//$a2017 = $omc->recuperarTodos('*', ["ano = 2017"]);
//
//$aAtributos = $omc->arAtributos;
//
//$dados = ['iguais', 'diferentes'];
//foreach($aAtributos as $atributo => $valor){
//
//}