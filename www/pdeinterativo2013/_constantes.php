<?php
	/*** SISID do módulo ***/
	define("SISID_PDE_INTERATIVO", 149);
	define("SISID_PDE_INTERATIVO_2012", 98);
	
	/************************/
	/*** Perfis do módulo ***/
	/************************/
	
	/*** SUPER USUÁRIO ***/
	define( "PDEINT_PERFIL_SUPER_USUARIO",	 				    855);
	
	/*** Perfis do PDE INTERATIVO ***/
	define( "PDEINT_PERFIL_ADMINISTRADOR",		 		    	857);
	define( "PDEINT_PERFIL_COMITE_ESTADUAL",		 		    862);
	define( "PDEINT_PERFIL_COMITE_MUNICIPAL", 		    		863);
	define( "PDEINT_PERFIL_EQUIPE_MEC",         			    859);
	define( "PDEINT_PERFIL_EQUIPE_FNDE",         			    858);
	define( "PDEESC_PERFIL_CONSULTA",           		    	865);
	define( "PDEESC_PERFIL_DIRETOR",	 			 	    	864);
	define( "PDEINT_PERFIL_CONSULTA_ESTADUAL",	 			 	866);
	define( "PDEINT_PERFIL_CONSULTA_MUNICIPAL",	 			 	867);
	define( "PDEINT_PERFIL_COMITE_PAR_MUNICIPAL", 			 	861);
	define( "PDEINT_PERFIL_COMITE_PAR_ESTADUAL", 			 	860);
	define( "PDEINT_PERFIL_CONSULTA_DIRETOR_PDE2013",		 	882);
	define( "PDEINT_PERFIL_DIRIGENTE_ESTADUAL",		 			873);
	define( "PDEINT_PERFIL_DIRIGENTE_MUNICIPAL",	 			874);
	define( "PDEINT_PERFIL_COORDENADOR_ESTADUAL",				878);
	define( "PDEINT_PERFIL_COORDENADOR_MUNICIPAL",	 			879);
	define( "PDEINT_PERFIL_EQUIPE_APOIO_ESTADUAL",				880);
	define( "PDEINT_PERFIL_EQUIPE_APOIO_MUNICIPAL",	 			881);
	
	
	
	/* PERFIL DA DIREÇÃO - pdeinterativo2013.tipoperfil */
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
	
	/*** Áreas de Perfil da tabela 'pdeinterativo2013.areaperfil' ***/
	define( "APE_GRUPO_TRABALHO",								1);
	define( "APE_DIRETOR",										2);
	define( "APE_VICEDIRETOR",									3);
	define( "APE_SECRETARIA",									4);
	define( "APE_EQUIPEPEDAGOGICA",								5);
	define( "APE_MEMBROSCONSELHO",								6);
	
	/*** Tipos de documento da tabela 'pdeinterativo2013.tipodocumento' ***/
	define( "TPDID_ARQ_IMPORTACAO",								1);
	define( "TPDID_GRUPO_TRABALHO",								2);
	
	/*** Tipos de documento da tabela 'pdeinterativo2013.areaatuacao' ***/
	define( "AAD_OUTRA",										7);
	
	/*** Tipos de documento da tabela 'pdeinterativo2013.aba' ***/
	define( "ABA_DIAGNOSTICO",									1);
	define( "ABA_DIAGNOSTICO_TAXASINDICADORES",					3);
	
	/** Fluxo do workflow para pdeinterativo 'workflow.tipodocumento' **/
	define( "TPD_WF_FLUXO", 									93);
	define( "TPD_WF_FLUXO_SEMPDE",								95);
	define( "TPD_WF_FLUXO_FEDERAL",								96);
	define( "TPD_WF_FORMACAO", 									94);
	
	/** Fluxo do workflow para pdeinterativo 'workflow.estadodocumento' **/
	define( "WF_ESD_ELABORACAO",								613);
	define( "WF_ESD_COMITE", 									614);
	define( "WF_ESD_MEC", 										617);
	define( "WF_ESD_VALIDADO_MEC",								616);
	
	define( "WF_ESD_ELABORACAO_SEMPDE",							611);
	define( "WF_ESD_COMITE_SEMPDE",								619);
	
	define( "WF_ESD_ELABORACAO_FEDERAL",						620);
	define( "WF_ESD_ANALISE_FEDERAL",							621);
	define( "WF_ESD_VALIDADO_FEDERAL",							622);
	
	
	/** Fluxo do workflow para pdeinterativo 'workflow.acaoestadodoc' **/	
	define( "WF_AED_DEVOLVER_COMITE_MEC", 						1478);
	
	/** Fluxo do workflow para pdeinterativo 'pdeinterativo2013.opcaopergunta' **/	
	define( "OPP_SEMPRE", 						3);
	define( "OPP_MAIORIA_DAS_VEZES",			4);
	define( "OPP_RARAMENTE",					5);
	define( "OPP_NUNCA",						6);
	
	/** Plano de Formação **/
	
	//Ano Censo
	define( "ANO_CENSO", 2010);
	
	/** Fluxo do workflow para plano de formação **/	
	
	define( "WF_EM_ELABORACAO",		609);
	define( "WF_EM_ANALISE_NO_SNF", 610);
	define( "WF_VALIDADO_PELO_SNF", 618);
	
	/*** Ano do Exercício (PDE Interativo) ***/
	define( "ANO_EXERCICIO_PDE_INTERATIVO", 2011 );
	
	define("CACHE_FILE", false);
	define("CACHE_MEM",  false);
?>
