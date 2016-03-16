<?php

if (  cte_possuiPerfilExclusivo(CTE_PERFIL_MONITORA_SUBACAO) ) {
	header('Location: brasilpro.php?modulo=principal/monitoramentoInterno/listaUfs&acao=A');
	exit();
}

$unidades = cte_pegarUfsPermitidas();
$municipios = cte_pegarMunicipiosPermitidos();


if ( count( $unidades ) == 1 && count( $municipicos ) == 0 ) {
	cte_selecionarUf( INSTRUMENTO_DIAGNOSTICO_ESTADUAL, WF_TIPO_BRASILPRO, $unidades[0] );
	header( "Location: ?modulo=principal/estrutura_avaliacao&acao=A" );
	exit();
}

if ( count( $unidades ) == 0 && count( $municipicos ) == 1 ) {
	cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_BRASILPRO, $municipicos[0] );
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