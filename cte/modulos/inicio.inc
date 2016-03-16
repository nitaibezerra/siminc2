<?php

$unidades = cte_pegarUfsPermitidas();
$municipios = cte_pegarMunicipiosPermitidos();

if ( count( $unidades ) == 1 && count( $municipios ) == 0 ) {
	cte_selecionarUf( INSTRUMENTO_DIAGNOSTICO_ESTADUAL, WF_TIPO_CTE, $unidades[0] );
	$db->commit();
	header( "Location: ?modulo=principal/estrutura_avaliacao&acao=A" );
	exit();
}

if ( count( $unidades ) == 0 && count( $municipios ) == 1 ) {
	cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_CTE, $municipios[0] );
	$db->commit();
	header( "Location: ?modulo=principal/estrutura_avaliacao&acao=A" );
	exit();
}

if ( count( $unidades ) > 0 ) {
	header( "Location: ?modulo=lista&acao=E" );
	exit();
} else {
	header( "Location: ?modulo=lista&acao=M" );
	exit();
}

?>