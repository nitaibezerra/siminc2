<?php
$filename = $_GET['file'];
header( 'Content-type: image/png' );
header( 'Content-Disposition: attachment; filename='. $filename);
readfile( $filename );


/*$filename = '/var/www/simec_dev/simec/www/arquivos/sca/407/407560.png';
header( 'Content-type: image/png' );
header( 'Content-Disposition: attachment; filename='. $filename);
readfile( $filename );*/