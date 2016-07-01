<?

header( 'Content-Type: application/vnd.ms-excel' );
header( 'Expires: 0' );
header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' ); 
header( 'Content-Disposition: attachment; filename="relatorio.xls"' );




$output = preg_replace("/<img[^>]+\>/i", "", $_SESSION['REL_PESQUISAPA']);

$output = preg_replace("/<a[^>]+\>/i", "", $output);

$output = str_replace('</a>','',$output);

$output = str_replace('&nbsp;&nbsp;&nbsp;','',$output);

print str_replace('<td align="center">&nbsp;&nbsp;','',$output);

?>