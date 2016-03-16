<?php

$municipios = cte_pegarMunicipiosPermitidos();
//ver($municipios, d);
if( $municipios && (!cte_possuiperfil(array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR))) ){
	if( $municipios[1] ){
		header( "Location: ?modulo=principal/monitoraFinanceiro/monitora_lista&acao=M" );
	} else {	
		header( "Location: ?modulo=principal/monitoraFinanceiro/monitora_lista&acao=M&evento=selecionar&muncod=".$municipios[0] );
	}
} else {
	header( "Location: cte.php?modulo=principal/monitoraFinanceiro/monitora_lista&acao=E" );
}	 
?>