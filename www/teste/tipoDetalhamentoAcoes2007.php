<?php

error_reporting( E_ALL );

header( 'Content-Type: text/plain;' );

define( 'ANO', 2007 );

echo "

start transaction;
-- commit;
-- rollback;

";


$sql_base = "
insert into elabrev.tipodetalhamentoacao
(
	acaid,
	tpdid
) values (
	( select acaid from elabrev.ppaprograma_orcamento where prgano = '%s' and acacodreferenciasof = %d and acastatus = 'A' ),
	( select tpdid from elabrev.tipodetalhamento where tpdcod = '%d' and tpdano = '%s' and tpdstatus = 'A' )
);
";

$handle = fopen ( "detalhamentoacao.csv", "r" );
$nomesCampo = fgetcsv( $handle, 1000, ";" );

$tipos = array();

$quantidade = 0;
while ( ( $linha = fgetcsv( $handle, 1000, ";" ) ) !== FALSE)
{
	$dados = array_combine( $nomesCampo, $linha );
	if ( !$dados['COD_LOCG'] )
	{
		continue;
	}
	$tipos[$dados['COD_MOMENTO']] = $dados['COD_MOMENTO'];
	$sql = sprintf(
		$sql_base,
			ANO,
			$dados['COD_REFERENCIA'],
			$dados['COD_MOMENTO'],
			ANO
	);
	echo $sql . "\n";
	$quantidade++;
}
fclose( $handle );
echo "-- " . $quantidade . " inclusões";

