<?php

/*** SISID do módulo ***/
define("SISID_PDE_ESCOLA", 34);

/************************/
/*** Perfis do módulo ***/
/************************/

/*** SUPER USUÁRIO ***/
define( "PDEESC_PERFIL_SUPER_USUARIO",	 				    200);

/*** Perfis do PDE ESCOLA ***/
define( "PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL",	    	223);
define( "PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL",			    224);
define( "PDEESC_PERFIL_COMITE_ESTADUAL",		 		    225);
define( "PDEESC_PERFIL_COMITE_MUNICIPAL", 		    		226);
define( "PDEESC_PERFIL_EQUIPE_TECNICA_MEC", 			    227);
define( "PDEESC_PERFIL_MONITORAMENTO_ESTADUAL",		    	249);
define( "PDEESC_PERFIL_CONSULTA",	 			 	    	250);
define( "PDEESC_PERFIL_MONITORAMENTO_MUNICIPAL",			253);

/*** Perfis do MAIS EDUCAÇÃO ***/
define( "PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO",		264);
define( "PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO",				290);
define( "PDEESC_PERFIL_CAD_MAIS_EDUCACAO",	 	    		383);
define( "PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO", 		385);
define( "PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO",		386);

/*** Perfis do ESCOLA ACESSÍVEL ***/
define( "PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL",	 	    	416);
define( "PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL", 		418);
define( "PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL",		419);
define( "PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ACESSIVEL",		420);
define( "PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL",			421);

/*** Perfis do ESCOLA ABERTA ***/
define( "PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA", 		473);
define( "PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA",		474);
define( "PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ABERTA",		471);
define( "PDEESC_PERFIL_CONSULTA_ESCOLA_ABERTA",				472);
define( "PDEESC_PERFIL_CAD_ESCOLA_ABERTA",	 	    		470);

if($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	/*** Perfis do QUESTIONÁRIO SEESP ***/
	define( "PDEESC_PERFIL_SEC_ESTADUAL_QUEST_SEESP", 			483);
	define( "PDEESC_PERFIL_SEC_MUNICIPAL_QUEST_SEESP",			482);
	define( "PDEESC_PERFIL_ESCOLA_QUEST_SEESP",	 	    		481);
	define( "PDEESC_PERFIL_ADM_QUEST_SEESP",	 	    		485);
} else {
	/*** Perfis do QUESTIONÁRIO SEESP ***/
	define( "PDEESC_PERFIL_SEC_ESTADUAL_QUEST_SEESP", 			483);
	define( "PDEESC_PERFIL_SEC_MUNICIPAL_QUEST_SEESP",			482);
	define( "PDEESC_PERFIL_ESCOLA_QUEST_SEESP",	 	    		481);
	define( "PDEESC_PERFIL_ADM_QUEST_SEESP",	 	    		480);
}

/*** Tipos de Turnos (PDE ESCOLA) ***/
define( "TURNO_MATUTINO", 	1 );
define( "TURNO_VESPERTINO", 2 );
define( "TURNO_NOTURNO", 	3 );
define( "TURNO_INTEGRAL", 	4 );

/*** Tipo nivel modalidade ensino (PDE ESCOLA) ***/
define( "PRE", 9 );
define( "EDUCACAOPREESCOLAR", 		1 );
define( "FUNDAMENTAL1A4SERIE", 		2 );
define( "FUNDAMENTAL5A8SERIE", 		3 );
define( "FUNDAMENTAL1A8SERIE", 		4 );
define( "EDUCACAOESPECIAL", 		5 );
define( "ENSINOMEDIO1A3", 			6 );
define( "ALFABETIZACAOALUNOS", 		7 );
define( "CURSOSSUPLETIVO", 			8 );
define( "ENSINODEJOVENS1A4SERIE", 	10 );

/*** Tipos de Ciclo (PDE ESCOLA) ***/
define( "CICLO1",	1 );
define( "CICLO2", 	2 );
define( "CICLO3", 	3 );

/*** Quantidade de questões por modalidade de ensino (PDE ESCOLA) ***/
define("MAX_QUESTOES", 			  221);
define("MAX_QUESTOES_MEDIA",	  276);
define("MAX_QUESTOES_ENS_1_A_4",  213);
define("MAX_QUESTOES_ENS_5_A_8",  212);
define("MAX_QUESTOES_ENS_MEDIO",  212);
define("MAX_QUESTOES_EJA",		  212);
define("MAX_QUESTOES_CICLO", 	  212);
define("MAX_QUESTOES_CRECHE",	  210);
define("MAX_QUESTOES_PRE_ESCOLA", 211);

/*** estado "em elaboração" ***/
define( "ESTADO_EM_ELABORACAO", 76 );
define( "ESTADO_EM_CORRECAO", 	37 );

/*** estado "finalizado" ***/
define( "ENVIADO_PARA_PAGAMENTO", 90 );

/*** Situações do "Mais Educação" ***/
define( "ME_SIT_NAO_CADASTRADO",     1);
define( "ME_SIT_CADASTRADO", 	     2);
define( "ME_SIT_APROVADO", 		     3);
define( "ME_SIT_NAO_APROVADO_SEC",   4);
define( "ME_SIT_FINALIZADO", 	     5);
define( "ME_SIT_NAO_APROVADO_SECAD", 6);

/*** tpdid dos submódulos do módulo "Escola" ***/
define( "TPDID_MAIS_EDUCACAO", 		5  );
define( "TPDID_ESCOLA_ACESSIVEL",	17 );
define( "TPDID_ESCOLA_ABERTA",		21 );
define( "TPDID_PDE_ESCOLA", 		29 );

/*** Estado dos documentos do submódulo (MAIS EDUCAÇÃO) ***/
define( "CADASTRAMENTO_ME", 	   		32 );
define( "AVALIACAO_SECRETARIA_ME", 		31 );
define( "AVALIACAO_MEC_ME", 	   		33 );
define( "FINALIZADO_ME", 		   		34 );
define( "CORRECAO_CADASTRAMENTO_ME", 	35 );
define( "CORRECAO_SECRETARIA_ME", 	 	36 );

/*** Estado dos documentos do submódulo (ESCOLA ACESSÍVEL) ***/
define( "CADASTRAMENTO_EA", 	   				147 );
define( "AGUARDANDO_CORRECAO_CADASTRAMENTO_EA",	148 );
define( "AVALIACAO_SECRETARIA_EA", 				149 );
define( "AGUARDANDO_CORRECAO_SECRETARIA_EA",	150 );
define( "AVALIACAO_MEC_EA", 	   				151 );
define( "FINALIZADO_EA", 		   				152 );
define( "RELATORIO_CONSOLIDADO_EMITIDO_EA",		153 );
define( "ENVIADO_PAGAMENTO_EA",					154 );

/*** Ação finalizado -> relatório emitido do ESCOLA ACESSÍVEL ***/
define( "EA_AEDID_FINALIZADO_REL_EMITIDO",		329 );

/*** Estado dos documentos do submódulo (ESCOLA ABERTA) ***/
define( "CADASTRAMENTO_EAB", 	   					182 );
define( "AGUARDANDO_CORRECAO_CADASTRAMENTO_EAB",	183 );
define( "AVALIACAO_SECRETARIA_EAB", 				184 );
define( "AGUARDANDO_CORRECAO_SECRETARIA_EAB",		185 );
define( "AVALIACAO_MEC_EAB", 	   					186 );
define( "FINALIZADO_EAB", 		   					187 );
define( "RELATORIO_CONSOLIDADO_EMITIDO_EAB",		188 );
define( "ENVIADO_PAGAMENTO_EAB",					189 );

/*** Ação finalizado -> relatório emitido do ESCOLA ABERTA ***/
define( "EAB_AEDID_FINALIZADO_REL_EMITIDO",		431 );

/*** Funções(funid) MAIS EDUCAÇÃO ***/
define( "FUN_DIRETOR_ME", 	   19 );
define( "FUN_COORDENADOR_ME",  41 );
define( "FUN_PARCEIRO_ME_PJ",  42 );
define( "FUN_PARCEIRO_ME_PF",  48 );
define( "FUN_COORDENADOR_ME_ESTADUAL",  95 );
define( "FUN_COORDENADOR_ME_MUNICIPAL", 96 );

/*** Funções(funid) ESCOLA ABERTA ***/
define( "FUN_DIRETOR_EAB",  78 );
define( "FUN_COORDENADOR_EAB",  79 );

/*** Funções(funid) ESCOLA ACESSÍVEL ***/
define( "FUN_DIRETOR_EA",  80 );
define( "FUN_COORDENADOR_EA",  81 );

/*** Ano do Exercício (PDE Escola) ***/
define( "ANO_EXERCICIO_PDE_ESCOLA", 2008 );

/*** Data limite para Comite Estadual, Comite Municipal e Equipe Municipal e Estadual tramitarem. (PDE ESCOLA) ***/
define( "DATA_LIMITE_EQUIPE", "12-12-2009" );
define( "DATA_LIMITE_COMITE", "19-12-2009" );

if($_SESSION['baselogin'] == 'simec_desenvolvimento'){
	/*** Questionários ***/
	define( "QUESTIONARIO_I", 	17 ); //estadual
	define( "QUESTIONARIO_II", 	26 ); //estadual
	define( "QUESTIONARIO_III", 27 ); //municipal
	define( "QUESTIONARIO_IV", 	28 ); //estadual
	define( "QUESTIONARIO_V",   29 ); //estadual
	define( "QUESTIONARIO_VI", 	30 ); //escola
	define( "QUESTIONARIO_VII",	31 ); //escola
	
	#QUESTIONARIO PDE-ESCOLA: MONITORAMENTO FÍSICO-FINANCEIRO
	define("QUESTIONARIO_MONIT_FISICO_FINANC", 87);
	
} else {
	/*** Questionários ***/
	define( "QUESTIONARIO_I", 	57 ); //estadual
	define( "QUESTIONARIO_II", 	51 ); //estadual
	define( "QUESTIONARIO_III", 53 ); //municipal
	define( "QUESTIONARIO_IV", 	52 ); //estadual
	define( "QUESTIONARIO_V",   54 ); //estadual
	define( "QUESTIONARIO_VI", 	55 ); //escola
	define( "QUESTIONARIO_VII",	56 ); //escola
	
	#QUESTIONARIO PDE-ESCOLA: MONITORAMENTO FÍSICO-FINANCEIRO
	define("QUESTIONARIO_MONIT_FISICO_FINANC", 87);
	
}

/*** QUESTIONÁRIO DO MAIS EDUCAÇÃO ***/
define( "QUESTIONARIO_MAISEDUC",	69 );


/*** ESTADOS WORKFLOW (PDE ESCOLA) ***/
define( "AVALIACAO_MEC_WF",							 87 );
define( "VALIDACAO_PELO_MEC_WF",					 90 );
define( "AUTOAVALIACAO_WF",							 61 );
define( "EM_ELABORACAO_PC_WF",						141 );
define( "DEVOLVIDO_PARA_ESCOLA_WF",					 37 );
define( "DEVOLVIDO_PARA_ESCOLA_PC_WF",				142 );
define( "AVALIACAO_COMITE_ME_WF",					143 );
define( "DEVOLVIDO_PARA_COMITE_WF",					144 );
define( "AVALIACAO_MEC_PARCERIA_COMPLEMENTAR_WF",	145 );
define( "ENVIADO_PARA_PAGAMENTO_WF",				146 );

/*** tpdid na tabela workflow.estadodocumento ***/
define( "TPDID_WF", 5 );

?>