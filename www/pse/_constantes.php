<?php

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define( "SUPER_USUARIO", 				303 );
	define( "MEC",							317 );
	define( "SECRETARIA_ESTADUAL", 			318 );
	define( "SECRETARIA_MUNICIPAL", 		322 );
	define( "ESCOLA_ESTADUAL", 				320 );
	define( "ESCOLA_MUNICIPAL",				321 );
	define( "CONSULTA",						397 );
	define( "PARCEIRO",						398 );
	define( "DIRETOR_ESCOLA",				704 );
	define( "EDUCADOR_ESCOLA",				705 );
	define( "SAUDE",						321 );
}
else {
	define( "SUPER_USUARIO", 				303 );
	define( "MEC",							317 );
	define( "SECRETARIA_ESTADUAL", 			318 );
	define( "SECRETARIA_MUNICIPAL", 		322 );
	define( "ESCOLA_ESTADUAL", 				320 );
	define( "ESCOLA_MUNICIPAL",				321 );
	define( "CONSULTA",						397 );
	define( "PARCEIRO",						398 );
	define( "DIRETOR_ESCOLA",				704 );
	define( "EDUCADOR_ESCOLA",				705 );
	define( "SAUDE",						321 );
}
define( "PRAZO_JUSTIFICATICA_META_INICIO", 	'20121201' );
define( "PRAZO_JUSTIFICATICA_META_FIM",		'20121210' );

define("ANO_CENSO", $_SESSION['exercicio'] - 2)
?>