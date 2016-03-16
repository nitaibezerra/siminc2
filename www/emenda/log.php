<?php

require_once('../../global/config.inc');

//$lines = file('/home/gustavoguarda/www2/simec/arquivos/log.txt');
$lines = file(APPRAIZ.'arquivos/log.txt');
//$lines = file('http://www.fnde.gov.br/webservices/sigef/index.php/empenho/solicitar');

foreach ($lines as $line_num => $line) {
    echo "Line #<b>{$line_num}</b> : " . simec_htmlspecialchars($line) . "<br />\n";
}