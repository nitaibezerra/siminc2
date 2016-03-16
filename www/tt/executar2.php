<?php
$bat_filename = "/var/www/simec/www/tt/teste.txt";
$bat_log_filename = "/var/www/simec/www/tt/newsletterNew_log.txt";
$bat_file = fopen($bat_filename, "w");
if($bat_file) {
    fwrite($bat_file, "@echo off"."\n");
    fwrite($bat_file, "echo Starting proces >> ".$bat_log_filename."\n");
    fwrite($bat_file, "php /var/www/simec/www/tt/teste1.php >> ".$bat_log_filename."\n");
    fwrite($bat_file, "echo End proces >> ".$bat_log_filename."\n");
    fwrite($bat_file, "EXIT"."\n");
    fclose($bat_file);
}
           
//
// Start the process in the background
//
$exe = "start /b ".$bat_filename;
if( pclose(popen($exe, 'r')) ) {
    echo "sim";
}
echo "nao";
?>