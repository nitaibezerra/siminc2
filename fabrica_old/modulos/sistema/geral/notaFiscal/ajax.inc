<?php
include APPRAIZ."fabrica/classes/NotaFiscalMemorando.class.inc";

$ajax = new NotaFiscalMemorando();

$result = $ajax->recuperarDataCadastro($_POST['memoid']);
echo $result['datacadastro'];

exit; // evita a inclusão de código-lixo