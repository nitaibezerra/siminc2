<?php
// INDICA O PROJETO PDE
define('PROJETO_PDE', 3);

// INDICA O SISTEMA PPA
define('SISTEMA_PPA', 1);



define('PERFIL_COORDEUNIDMONITORA', 4);
define('PERFIL_UNIDMONITORAAVALIA', 18);

define('PERFIL_GESTORUNIDPLANEJAM', 112);
define('PERFIL_EQUIPAPOIOGESTORUP', 113);

# Gestor da Unidade de Orçamento
define('PERFIL_MONITORA_GESTORUNIDORCAMENTO', 406);
# Equipe de Apoio ao Gestor da Unidade de Orçamento
define('PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO', 407);

define('PERFIL_COORDACAO', 1);
define('PERFIL_EQCOOACAO', 8);

// usado na tela de listagem de projetos
define( 'PERFIL_ADMINISTRADOR', 103 );
define( 'PERFIL_CONSULTA', 104 );
		
define( 'PERFIL_GESTOR',  98 );
define( 'PERFIL_GERENTE', 101 );
define( 'PERFIL_EQUIPE_APOIO_GESTOR',  99 );
define( 'PERFIL_EQUIPE_APOIO_GERENTE', 102 );
if ( $_SESSION['projeto'] == PROJETO_PDE ) {
	$_SESSION['projeto'] = null;
}

// INDICA O PROJETO A SER TRABALHADO
define( 'PROJETO', $_SESSION['projeto'] ? $_SESSION['projeto'] : null );


// INDICA OS ESTADOS DAS ATIVIDADES
define( 'STATUS_NAO_INICIADO', 1 );
define( 'STATUS_EM_ANDAMENTO', 2 );
define( 'STATUS_SUSPENSO',     3 );
define( 'STATUS_CANCELADO',    4 );
define( 'STATUS_CONCLUIDO',    5 );

?>