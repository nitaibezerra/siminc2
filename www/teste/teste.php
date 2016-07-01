<style type="text/css">
<!--

td {
	font-size: 14px;
	font-weight: bold;
	color: #0066CC;
}
-->
</style>

<?

//$teste = file_get_contents ('http://portal.mec.gov.br/ide/layout_tabelas/gerarTabelas.php?municipio=1200054');
$url = 'http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=2008&p_uf=AC&p_municipio=1200104&p_tp_entidade=&p_cgc=04508933000145';
if ($stream = fopen($url, 'r')) {
    // print the first 5 bytes
    echo stream_get_contents($stream, 5);

    fclose($stream);
}


//echo $teste;



/*
$teste = file_get_contents ('http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=2008&p_uf=AC&p_municipio=1200104&p_tp_entidade=&p_cgc=04508933000145');
echo $teste;	
//urlencode()
*/

/*

function my_file_get_contents( $site_url ){
    $ch = curl_init();
    $timeout = 10;
    curl_setopt ($ch, CURLOPT_URL, $site_url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt ($ch, CURLOPT_CONNECTTIMEOUT, $timeout);
    $file_contents = curl_exec($ch);
    curl_close($ch);
    return $file_contents;
}

$teste = my_file_get_contents('http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=2008&p_uf=AC&p_municipio=1200104&p_tp_entidade=&p_cgc=04508933000145');
echo $teste;
*/
//$url = "http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=2008&p_uf=AC&p_municipio=1200104&p_tp_entidade=&p_cgc=04508933000145";


?>

