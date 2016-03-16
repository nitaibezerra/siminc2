<?php

include("config.inc");

error_reporting( E_ALL );

header( 'Content-Type: text/plain; charset=utf-8' );

$dados = simplexml_load_file( APPRAIZ . 'arquivos/SIGPLAN/importacao/2008/apoio/CargaProduto.xml' );

echo "<pre>

start transaction;
-- commit;
-- rollback;

";

$sqlBase = "
update public.produto
set
	prodsc = upper('%s')
where
	procod = '%s';
";

$codigosUtilizados = array();

foreach ( $dados->Produto as $produto )
{	
	$sql = sprintf(
		$sqlBase,
			str_replace( "'", "\"", $produto->PRODsc ),
			str_replace( "'", "\"", $produto->PROCod ),
			
			$produto->PRODsc,
			$produto->PROCod
	);
	echo $sql . "\n";
}
echo "</pre>";
