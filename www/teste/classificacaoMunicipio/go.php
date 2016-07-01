<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/cte/_constantes.php";
include APPRAIZ . "www/cte/_funcoes.php";

function getCsvContent( $file )
{
	if ( !is_readable( $file ) )
	{
		return;
	}
	$handler = fopen( $file, "r" );
	$data = array();
	while ( $line = fgetcsv( $handler ) )
	{
		array_push( $data, array( "muncod" => $line[0], "nome" => $line[1] ) );
	}
	fclose( $handler );
	return $data;
}

function showInsertType( $group, $type )
{
	echo "
insert into territorios.tipomunicipio ( tpmdsc, tpmstatus, gtmid )
values (
	'" . $type . "',
	'A',
	( select gtmid from territorios.grupotipomunicipio where gtmdsc = '" . $group . "' )
);
	";
}

function showInsertCities( $type, $cities )
{
	foreach ( $cities as $city )
	{
		echo "
insert into territorios.muntipomunicipio
( muncod, estuf, tpmid )
values
(
	'" . $city['muncod'] . "', -- " . $city['nome'] . "
	( select estuf from territorios.municipio where muncod = '" . $city['muncod'] . "' ),
	( select tpmid from territorios.tipomunicipio where tpmdsc = '" . $type . "' )
);
		";
	}
}

$db = new cls_banco();

header( 'Content-Type: text/plain; charset=iso-8859-1' );

echo "

start transaction;

-- commit
-- rollback

";

// -- GRUPO
$nomeGrupo = "Classificação IDEB";
echo "
insert into territorios.grupotipomunicipio ( gtmdsc, gtmcumulativo, gtmstatus )
values (
	'" . $nomeGrupo . "',
	false,
	'A'
);
";

// -- TIPOS DO GRUPO

$nomeTipoPriorizados = "Priorizados";
$nomeTipoAbaixoNacional = "Abaixo da média nacional";
$nomeTipoAbaixoIdeb = "Abaixo da média IDEB híbrido";
$nomeTipoAnalfabetismo = "Priorizados pela taxa de analfabetismo de 10 a 15 anos";
$nomeTipoMediaNacional = "Na média e acima da média nacional";
$nomeTipoSemIdeb = "Sem IDEB";
showInsertType( $nomeGrupo, $nomeTipoPriorizados );
showInsertType( $nomeGrupo, $nomeTipoAbaixoNacional );
showInsertType( $nomeGrupo, $nomeTipoAbaixoIdeb );
showInsertType( $nomeGrupo, $nomeTipoAnalfabetismo );
showInsertType( $nomeGrupo, $nomeTipoMediaNacional );
showInsertType( $nomeGrupo, $nomeTipoSemIdeb );

$dados = getCsvContent( "priorizados.csv" );
showInsertCities( $nomeTipoPriorizados, $dados );

$dados = getCsvContent( "abaixo_da_media_nacional.csv" );
showInsertCities( $nomeTipoAbaixoNacional, $dados );

$dados = getCsvContent( "abaixo_da_media_ideb_hibrido.csv" );
showInsertCities( $nomeTipoAbaixoIdeb, $dados );

$dados = getCsvContent( "priorizados_pela_taxa_de_analfabetismo_de_10_a_15_anos.csv" );
showInsertCities( $nomeTipoAnalfabetismo, $dados );

$dados = getCsvContent( "na_media_e_acima_da_media_nacional.csv" );
showInsertCities( $nomeTipoMediaNacional, $dados );

$dados = getCsvContent( "sem_ideb.csv" );
showInsertCities( $nomeTipoSemIdeb, $dados );

?>