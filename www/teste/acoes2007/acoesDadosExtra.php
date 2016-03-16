<?php

error_reporting( E_ALL );

header( 'Content-Type: text/plain; charset=utf-8' );

define( 'ANO', 2007 );

$dados = simplexml_load_file( 'acoesDadosExtra.xml' );

echo "

start transaction;
-- commit;
-- rollback;

";

$sqlBase = "
update elabrev.ppaacao_orcamento
set
	acadsc          = '%s',
	acafinalidade   = '%s',
	acadescricao    = '%s',
	acabaselegal    = '%s',
	acadetalhamento = '%s'
where
	prgano = '2007' and
	acacod = '%s' and
	prgcod = '%s';
";

$codigosUtilizados = array();

foreach ( $dados->PPAAcao as $acao )
{
	$codigo = $acao->ACACod . '-' . $acao->PRGCod;
	if ( array_key_exists( $codigo, $codigosUtilizados ) )
	{
		continue;
	}
	$codigosUtilizados[$codigo] = true;
	$sql = sprintf(
		$sqlBase,
			str_replace( "'", "\"", $acao->ACADsc ),
			str_replace( "'", "\"", $acao->ACAFinalidade ),
			str_replace( "'", "\"", $acao->ACADescricao ),
			str_replace( "'", "\"", $acao->ACABaseLegal ),
			str_replace( "'", "\"", $acao->ACADetalhamento ),
			
			$acao->ACACod,
			$acao->PRGCod
	);
	echo $sql . "\n";
}

