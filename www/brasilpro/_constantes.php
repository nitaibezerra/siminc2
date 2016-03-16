<?php

//Sistema
define( "CTE_SISTEMA", 14 );

// Perfil
define( "CTE_PERFIL_EQUIPE_MUNICIPAL", 139 );
define( "CTE_PERFIL_EQUIPE_LOCAL", 140 );
define( "CTE_PERFIL_EQUIPE_LOCAL_APROVACAO", 141 );
define( "CTE_PERFIL_EQUIPE_TECNICA", 143 );
define( "CTE_PERFIL_CONSULTORES", 144 );
define( "CTE_PERFIL_ALTA_GESTAO", 142 );
define( "CTE_PERFIL_CONSULTA_GERAL", 146 );
define( "CTE_PERFIL_SUPER_USUARIO", 145 );
define( "CTE_PERFIL_ADMINISTRADOR", 150 );
define( "CTE_BRASIL_PROECERISTA_FNDE", 169 );
define( "CTE_BRASIL_COORDENADOR_FNDE", 613 );
define( "CTE_BRASIL_ANALISTA_FNDE",    615 );
define( "CTE_PERFIL_ADMINISTRATOR_TEMP", 390 );
define( "CTE_PERFIL_MONITORA_SUBACAO", 431 );

define( "CTE_PERFIL_ENGENHEIRO_FNDE", 698 );
define( "CTE_PERFIL_COORDENADOR_GERAL", 697 );

// Instrumentos
define( "INSTRUMENTO_DIAGNOSTICO_ESTADUAL", 3 );
define( "INSTRUMENTO_DIAGNOSTICO_MUNICIPAL", 4 );

// workflow tipo documento
define("WF_FLUXO_OBRAS_BRASILPRO", 50);

// Formas de Atentimento
define( "ATENDIMENTO_BRASIL_PROFISSIONALIZADO", 1 );

// Formas de execucao
define( "FORMA_EXECUCAO_ASS_TEC", 15 );
define( "FORMA_EXECUCAO_ASS_FIN", 16 );

############################# Desenvolvimento #############################
// Estado Documento
//verifica se o banco щ do desenvolvimento ou produчуo ou espelho produчуo
/*
	define( "CTE_ESTADO_DIAGNOSTICO", 51 );
	define( "CTE_ESTADO_BRASIL_PRO", 53 );
	define( "CTE_ESTADO_ANALISE", 55 );
	define( "CTE_ESTADO_FINALIZADO", 61 );
	define( "CTE_ESTADO_BRASIL_PROECER", 63 );
	define( "CTE_ESTADO_FNDE", 69 );
*/
############################# Produчуo #############################
// Estado Documento

define( "CTE_ESTADO_DIAGNOSTICO", 16 );
define( "CTE_ESTADO_BRASIL_PRO", 17 );
define( "CTE_ESTADO_PAR", 17 );
define( "CTE_ESTADO_ANALISE", 20 );
define( "CTE_ESTADO_FIANCEIRA", 21 );
define( "CTE_ESTADO_FINALIZADO", 18 );
define( "CTE_ESTADO_PARECER", 22 );
define( "CTE_ESTADO_FNDE", 23 );


// Status da Subaчуo
define( "STATUS_SUBACAO_JA_CONTEMPLADA", 5 );
define( "STATUS_SUBACAO_NAO_ATENDIDA", 6 );

//Origem daObras
define("ORIGEM_OBRA_BRASILPRO", 6 );

//
//Obras Brasil Pro
// 

define( "OBRAS_SISID", 45 );
define( "OBRAS_ORIGEM_BRASILPRO", 6 );
define( "OBRAS_QUESTIONARIO", 65 );
define( "OBRAS_ANALISE_ENGENHEIRO", 66 );

// Formas de execuчуo
define( "EXECUCAO_ESDATUAL_OBRAS", 21);

//Estados Workflow ObrasBrasil Pro

define( "WF_VALIDACAO_DILIGENCIA", 384 );
define( "WF_VALIDACAO_INDEFERIMENTO", 385 );
define( "WF_EM_CADASTRAMENTO", 386 );
define( "WF_AGUARDANDO_ANALISE", 387 );
define( "WF_EM_DELIGENCIA", 388 );
//define( "WF_TIPO_EM_CORRECAO", 388);
define( "WF_VALIDACAO_DEFERIMENTO", 389 );
define( "WF_REVISAO_ANALISE", 390 );
define( "WF_ANALISE_RET_DILIGENCIA", 391 );
define( "WF_INDEFERIDO", 386 );
define( "WF_DEFERIDO", 393 );
define( "WF_DEFERIDO_CONDICIONADO_ENGENHARIA", 394 );
define( "WF_INDEFERIDO_PRAZO", 395 );
define( "WF_EM_ANALISE", 396 );
define( "WF_APROVADA", 397 );
define( "WF_ARQUIVADA", 398 );

//Fluxo

define( "FLUXO_OBRAS_BRASIL_PRO", 50 );

// PERFILS

define("BRASIL_PRO_PERFIL_SUPER_USUARIO", 					441);
define("BRASIL_PRO_PERFIL_ADMINISTRADOR", 					459);
define("BRASIL_PRO_PERFIL_ALTA_GESTAO", 					462);
define("BRASIL_PRO_PERFIL_EQUIPE_ESTADUAL", 				461);
define("BRASIL_PRO_PERFIL_EQUIPE_FINANCEIRA", 				458);
define("BRASIL_PRO_PERFIL_EQUIPE_MUNICIPAL", 				460);
define("BRASIL_PRO_PERFIL_PREFEITO", 						556);
define("BRASIL_PRO_PERFIL_EQUIPE_TECNICA", 					457);
define("BRASIL_PRO_PERFIL_ENGENHEIRO_FNDE", 				468);
define("BRASIL_PRO_PERFIL_COORDENADOR_GERAL", 				477);
define("BRASIL_PRO_PERFIL_COORDENADOR_TECNICO", 			478);
define("BRASIL_PRO_PERFIL_CONSULTA", 						484);
define("BRASIL_PRO_PERFIL_CONSULTA_ESTADUAL",				488);
define("BRASIL_PRO_PERFIL_CONSULTA_MUNICIPAL",	    		489);
define("BRASIL_PRO_PERFIL_ANALISE_PI",			    		587);
define("BRASIL_PRO_PERFIL_PROFUNC_PREANALISEPF",			539);
define("BRASIL_PRO_PERFIL_PROFUNC_ANALISEPF",	   			540);
define("BRASIL_PRO_PERFIL_PROFUNC_PREANALISEPF_TUTOR",		546);
define("BRASIL_PRO_PERFIL_PROFUNC_ANALISEPF_TUTOR",			548);
define("BRASIL_PRO_PERFIL_MANUTENCAO_TABELAS_APOIO",		545);

//
// Aчѕes Obras Brasil pro
//

define("WF_AEDID_ANALISE_FNDE_ENVIAR_PARA_ANALISE",			   1080);
define("WF_AEDID_ANALISE_FNDE_ENVIAR_VALIDACAO_INDEFERIMENTO", 1079);
?>