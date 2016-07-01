<?php
	/*** SISID do módulo ***/
	define("SISID_PDE_INTERATIVO", 98);
	
	/************************/
	/*** Perfis do módulo ***/
	/************************/
	
	/*** SUPER USUÁRIO ***/
	define( "PDEINT_PERFIL_SUPER_USUARIO",	 				    523);
	
	/*** Perfis do PDE INTERATIVO ***/
	define( "PDEINT_PERFIL_ADMINISTRADOR",		 		    	682);
	define( "PDEINT_PERFIL_COMITE_ESTADUAL",		 		    490);
	define( "PDEINT_PERFIL_COMITE_MUNICIPAL", 		    		491);
	define( "PDEINT_PERFIL_EQUIPE_MEC",         			    493);
	define( "PDEINT_PERFIL_EQUIPE_FNDE",         			    589);
	define( "PDEESC_PERFIL_CONSULTA",           		    	495);
	define( "PDEESC_PERFIL_DIRETOR",	 			 	    	544);
	define( "PDEINT_PERFIL_CONSULTA_ESTADUAL",	 			 	584);
	define( "PDEINT_PERFIL_CONSULTA_MUNICIPAL",	 			 	585);
	define( "PDEINT_PERFIL_COMITE_PAR_MUNICIPAL", 			 	678);
	define( "PDEINT_PERFIL_COMITE_PAR_ESTADUAL", 			 	677);
	define( "PDEINT_PERFIL_CONSULTA_DIRETOR_PDE2013",		 	882);
	
	
	
	/* PERFIL DA DIREÇÃO - pdeinterativo.tipoperfil */
	define( "TPE_PROFESSOR",	 			 	    		1);
	define( "TPE_DIRETOR",	 			 	    			2);
	define( "TPE_FUNC_NAO_DOCENTE",		 	    			3);
	define( "TPE_PAIS_RESPONSAVEL",		 	    			4);
	define( "TPE_ESTUDANTE",	 			 	    		5);
	define( "TPE_OUTRO",	 				 	    		6);
	define( "TPE_VICEDIRETOR", 			 	    			7);
	define( "TPE_SECRETARIO",			 	    			8);
	define( "TPE_COORDENADOR", 			 	    			9);
	define( "TPE_SUPERVISOR", 			 	    			10);
	define( "TPE_ORIENTADOR", 			 	    			11);
	
	/*** Áreas de Perfil da tabela 'pdeinterativo.areaperfil' ***/
	define( "APE_GRUPO_TRABALHO",								1);
	define( "APE_DIRETOR",										2);
	define( "APE_VICEDIRETOR",									3);
	define( "APE_SECRETARIA",									4);
	define( "APE_EQUIPEPEDAGOGICA",								5);
	define( "APE_MEMBROSCONSELHO",								6);
	
	/*** Tipos de documento da tabela 'pdeinterativo.tipodocumento' ***/
	define( "TPDID_ARQ_IMPORTACAO",								1);
	define( "TPDID_GRUPO_TRABALHO",								2);
	
	/*** Tipos de documento da tabela 'pdeinterativo.areaatuacao' ***/
	define( "AAD_OUTRA",										7);
	
	/*** Tipos de documento da tabela 'pdeinterativo.aba' ***/
	define( "ABA_DIAGNOSTICO",									1);
	define( "ABA_DIAGNOSTICO_TAXASINDICADORES",					3);
	
	/** Fluxo do workflow para pdeinterativo 'workflow.tipodocumento' **/
	define( "TPD_WF_FLUXO", 									43);
	define( "TPD_WF_FLUXO_SEMPDE",								57);
	define( "TPD_WF_FLUXO_FEDERAL",								63);
	define( "TPD_WF_FORMACAO", 									55);
	
	/** Fluxo do workflow para pdeinterativo 'workflow.estadodocumento' **/
	define( "WF_ESD_ELABORACAO",								305);
	define( "WF_ESD_COMITE", 									306);
	define( "WF_ESD_MEC", 										307);
	define( "WF_ESD_VALIDADO_MEC",								310);
	define( "WF_ESD_ELABORACAO_SEMPDE",							423);
	define( "WF_ESD_COMITE_SEMPDE",								424);
	
	define( "WF_ESD_ELABORACAO_FEDERAL",						459);
	define( "WF_ESD_ANALISE_FEDERAL",							460);
	define( "WF_ESD_VALIDADO_FEDERAL",							461);
	
	
	/** Fluxo do workflow para pdeinterativo 'workflow.acaoestadodoc' **/	
	define( "WF_AED_DEVOLVER_COMITE_MEC", 						975);
	
	/** Fluxo do workflow para pdeinterativo 'pdeinterativo.opcaopergunta' **/	
	define( "OPP_SEMPRE", 						3);
	define( "OPP_MAIORIA_DAS_VEZES",			4);
	define( "OPP_RARAMENTE",					5);
	define( "OPP_NUNCA",						6);
	
	/** Plano de Formação **/
	
	//Ano Censo
	define( "ANO_CENSO", 2010);
	
	/** Fluxo do workflow para plano de formação **/	
	
	define( "WF_EM_ELABORACAO",		416);
	define( "WF_EM_ANALISE_NO_SNF", 417);
	define( "WF_VALIDADO_PELO_SNF", 418);
	
	/*** Ano do Exercício (PDE Interativo) ***/
	define( "ANO_EXERCICIO_PDE_INTERATIVO", 2011 );
	
	define("CACHE_FILE", false);
	define("CACHE_MEM",  false);
?>