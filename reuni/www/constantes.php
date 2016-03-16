<?php

// ----- PERFIS
	
	// IFES
	define( 'REUNI_PERFIL_IFES_APR', 108 ); // aprovacao
	define( 'REUNI_PERFIL_IFES_CAD', 107); // cadastro
	define( 'REUNI_PERFIL_IFES_CON', 106 ); // consulta
	
	// SESU
	define( 'REUNI_PERFIL_SESU_APR', 111 ); // aprovacao
	define( 'REUNI_PERFIL_SESU_CON', 109 ); // consulta
	define( 'REUNI_PERFIL_SESU_PAR', 110 ); // parecer
	
	// ADHOC
	define( 'REUNI_PERFIL_ADHOC', 115 );
	
	// COMISSAO
	define( 'REUNI_PERFIL_COMISSAO', 116 );
	


// -----TIPO  PARECER
	
	// RESPOSTA
	define( 'REUNI_PARECER_SESU',     1 );
	define( 'REUNI_PARECER_ADHOC',    2 );
	define( 'REUNI_PARECER_COMISSAO', 3 );
	
	// GLOBAL
	define( 'REUNI_PARECER_SESU_GLOBAL',     4 );
	define( 'REUNI_PARECER_ADHOC_GLOBAL',    5 );
	define( 'REUNI_PARECER_COMISSAO_GLOBAL', 6 );
	define( 'REUNI_PARECER_SESU_FINAL',      7 );
	


// ----- ESTADO DOCUMENTO
	
	define( 'REUNI_ESTADO_ELABORACAO', 3 );
	define( 'REUNI_ESTADO_IFES',       4 );
	define( 'REUNI_ESTADO_SESU',       5 );
	define( 'REUNI_ESTADO_ADHOC',      6 );
	define( 'REUNI_ESTADO_COMISSAO',   7 );
	define( 'REUNI_ESTADO_SESU_FINAL', 8 );
	define( 'REUNI_ESTADO_APROVADO',   9 );
	
// -------- ESTADO MONITORAMENTO

define( 'STATUS_NAO_INICIADO', 1 );
define( 'STATUS_EM_ANDAMENTO', 2 );
define( 'STATUS_SUSPENSO',     3 );
define( 'STATUS_CANCELADO',    4 );
define( 'STATUS_CONCLUIDO',    5 );
define( 'NAO_SE_APLICA',       6 );
?>