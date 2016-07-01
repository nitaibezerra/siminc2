<?php

header( 'Content-Type: text/plain; charset=utf-8' );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$a = md5_encrypt('unicod=26106&cpf=');

dump($a);

dump(md5_decrypt($a));

?>







