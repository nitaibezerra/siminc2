<?php

require_once 'config.inc';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if($_REQUEST['acao'] == 'E'){
    $db = new cls_banco();
    $sql = "insert into tarefa.entidadesolicitante (entid, esostatus) values('ABC', 'A')";

    $db->executar($sql);
    $db->close();

} else {
    number_format('', 2);
}

