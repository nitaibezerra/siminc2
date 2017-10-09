<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$db->executar("delete from spo.carga_funcional");

$file = fopen('carga_funcional.csv', 'r');

while (($line = fgetcsv($file, null, ';')) !== false)
{
ver( str_replace(['.', ','], ['', '.'], $line[19]));
    $prsano = $line[0];
    $unocod = $line[2];
    $acacod = str_pad($line[4], 4, '0', STR_PAD_LEFT);
    $prgcod = str_pad($line[6], 4, '0', STR_PAD_LEFT);
    $categoria = substr(trim($line[8]), 0, 1);
    $plocod = str_pad($line[10], 4, '0', STR_PAD_LEFT);
    $objcod = str_replace(['-', 'ND', ' '], [''], $line[12]);
    $inicod = str_replace(['-', 'ND', ' '], [''], $line[14]);
    $rp = substr(trim($line[16]), 0, 1);
    $loccod = str_pad($line[17], 4, '0', STR_PAD_LEFT);
    $valor = str_replace(['.', ','], ['', '.'], $line[19]);

    $sql = "INSERT INTO spo.carga_funcional(prsano, unocod, acacod, 
                                            prgcod, categoria, plocod, objcod, 
                                            inicod, rp, loccod, valor)
                                    VALUES ('$prsano', '$unocod', '$acacod',
                                            '$prgcod', '$categoria', '$plocod', '$objcod', 
                                            '$inicod', '$rp', '$loccod', $valor)";


    $db->executar($sql);
}
fclose($file);

$db->commit();


