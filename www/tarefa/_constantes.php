<?php 
define( 'STATUS_NAO_INICIADO', 1 );
define( 'STATUS_EM_ANDAMENTO', 2 );
define( 'STATUS_SUSPENSO',     3 );
define( 'STATUS_CANCELADO',    4 );
define( 'STATUS_CONCLUIDO',    5 );
define( 'FUN_SOLICITANTE_TAREFA', 53 );


if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define("TAREFA_PERFIL_SUPER_USUARIO", 272);
	define("TAREFA_PERFIL_GERENTE",		  291);
	define("TAREFA_PERFIL_TECNICO",		  292);
} else{
	define("TAREFA_PERFIL_SUPER_USUARIO", 293);
	define("TAREFA_PERFIL_GERENTE",		  294);
	define("TAREFA_PERFIL_TECNICO",		  296);
}

define('REMETENTE_EMAIL', 'tarefa@mec.gov.br');
define('REMETENTE_NOME',  'SIMEC - Módulo de Gestão de Tarefas');

?>