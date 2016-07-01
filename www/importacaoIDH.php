<?php

define('ARQUIVO', '../IDH 2000.csv');


header('content-type: text/plain');

$file = file(realpath(dirname(__FILE__)) . DIRECTORY_SEPARATOR . ARQUIVO);

foreach ($file as $i => $linha) {
    $dado = explode("\t", trim($linha));
    //print_r($dado);

    echo 'INSERT INTO TABELA_DO_IDH (muncod, estuf, campo_idh) VALUES ("' , $dado[0] , '", ' , $dado[1], ', ' , str_replace(",", ".", $dado[3]) , ");\n";
}





