<?php 
if($_SESSION['baselogin']=='simec_desenvolvimento'){ 
	//Estado Civil
	define("ECI_CASADO", 1);
	define("ECI_UNIAO_ESTAVEL", 7);
	
	//Formaзгo Escolaridade
	define("FOE_ESPECIALIZACAO", 				 8);
	define("FOE_MESTRADO", 						 9);
	define("FOE_DOUTORADO", 					 10);
	define("FOE_SUPERIOR_COMPLETO_PEDAGOGIA", 	 5);
	define("FOE_SUPERIOR_COMPLETO_LICENCIATURA", 6);
	define("FOE_SUPERIOR_COMPLETO_OUTRO", 		 7);
	define("FOE_SUPERIOR_INCOMPLETO", 		 	 3);	
	
	//Perfil
	define("PFL_SUPER_USUARIO",	 	1093);
	define("PFL_COORDENADOR_INST",	1098);
	define("PFL_COORDENADOR_CURSO",	1097);
	define("PFL_EQUIPE_MEC", 		1096);
	define("PFL_ADMINISTRADOR",     1094);
	define("PFL_CONSULTAGERAL",		1095);
	//Sistema
	define("SIS_SISFOR", 		   	177);	
	//WORKFLOW Curso
	define("WF_ENVIAR_ANALISE_MEC",	975);	
	//WORKFLOW IES - FASE 01 
	define("WF_PLAN_ANALISE_MEC",  1083);	
	define("WF_PLAN_FECHADO",  	   1151); // O cуdigo antigo й 1151 - O status fechado nгo existe mais. 	
	//WORKFLOW IES - FASE 02
	define("WF_PLAN_ANALISE_MEC2",  1226);	
	define("WF_PLAN_FECHADO2",  	1228);		
	
	//Tipo de Documento WORKFLOW
	define("WF_TPDID_SISFOR", 		   	    153);
	define("WF_TPDID_SISFOR_PLAN", 		    175);
	define("WF_TPDID_SISFOR_PLAN2", 		194);
	define("WF_TPDID_SISFOR_CADASTRAMENTO", 190);
	define("WF_TPDID_SISFOR_AVALIACAO", 	191);

    define("ESD_PROJETO_EMCADASTRAMENTO", 1100);
    define("ESD_PROJETO_VALIDADO", 		  1187);
    define("ESD_PROJETO_AJUSTES", 		  1229);

    define("ESD_EM_CADASTRAMENTO", 1204);
    define("ESD_CADASTRAMENTO_FINALIZADO", 1205);

    define("ESD_AVALIACAO", 1206);
    define("ESD_ENVIADO_PAGAMENTO", 1207);
    define("ESD_ANALISE_MEC", 1224);
    
    define("ESD_VALIDADO_MEC", 1083);
    
    define("ESD_VALIDADO_MEC2", 1227); //Validado pelo MEC - FASE 2
    define("ESD_EM_ELABORACAO2", 1225); //Em Elaboraзгo - FASE 2
} else {
	
	if(strstr($_SERVER['HTTP_HOST'],"simec-local") || strstr($_SERVER['HTTP_HOST'],"simec-d.mec.gov.br") || strstr($_SERVER['HTTP_HOST'],"simec-d")){
		
		// desenvolvimento
		define( 'SISTEMA_SGB',  'SISFOR' );
		define( 'USUARIO_SGB',  'SISFOR' );
		define( 'PROGRAMA_SGB', 'PROFE' );
		define( 'SENHA_SGB',    'SISFOR_HOMOLOG' );
		define( 'WSDL_CAMINHO', 'https://hmg.fnde.gov.br/spba/Servicos?wsdl');
		define( 'WSDL_CAMINHO_CADASTRO', 'http://sgbhmg.fnde.gov.br/sistema/ws/?wsdl');
		/*
		define( 'SISTEMA_SGB',  'SISFOR' );
		define( 'USUARIO_SGB',  'SISFOR' );
		define( 'SENHA_SGB',    'QEE,ZR.UM:U$:GGGPRG@R0Y#-DABM:GD' );
		define( 'WSDL_CAMINHO', 'http://www.fnde.gov.br/spba/Servicos?wsdl');
		define( 'WSDL_CAMINHO_CADASTRO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl');
		*/
	} else {
		// produзгo
		define( 'SISTEMA_SGB',  'SISFOR' );
		define( 'USUARIO_SGB',  'SISFOR' );
		define( 'SENHA_SGB',    'QEE,ZR.UM:U$:GGGPRG@R0Y#-DABM:GD' );
		define( 'WSDL_CAMINHO', 'http://www.fnde.gov.br/spba/Servicos?wsdl');
		define( 'WSDL_CAMINHO_CADASTRO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl');
	}	
	
	
	//Estado Civil
	define("ECI_CASADO", 1);
	define("ECI_UNIAO_ESTAVEL", 7);	
	
	//Formaзгo Escolaridade
	define("FOE_ESPECIALIZACAO", 				 8);
	define("FOE_MESTRADO", 						 9);
	define("FOE_DOUTORADO", 					 10);
	define("FOE_SUPERIOR_COMPLETO_PEDAGOGIA", 	 5);
	define("FOE_SUPERIOR_COMPLETO_LICENCIATURA", 6);
	define("FOE_SUPERIOR_COMPLETO_OUTRO", 		 7);
	define("FOE_SUPERIOR_INCOMPLETO", 		 	 3);		

	//Perfil
	define("PFL_SUPER_USUARIO",	 	1100);
	define("PFL_COORDENADOR_INST",	1103);
	define("PFL_COORDENADOR_CURSO",	1105);
	define("PFL_COORDENADOR_ADJUNTO_IES", 1195);
	define("PFL_SUPERVISOR_IES",    	  1197);
	define("PFL_FORMADOR_IES",    	  	  1198);
	define("PFL_PROFESSOR_PESQUISADOR",	  1196);
	define("PFL_TUTOR",	  				  1199);
	
	
	define("PFL_EQUIPE_MEC", 		1102);
	define("PFL_COORDENADOR_MEC",	1208);
	define("PFL_DIRETOR_MEC",		1209);
	define("PFL_FORUM_ESTADUAL_PERMANENTE", 1228);
	
	define("PFL_ADMINISTRADOR",     1101);
	define("PFL_CONSULTAGERAL",		1104);
	//Sistema
	define("SIS_SISFOR", 		   	177);	
	//WORKFLOW Curso
	define("WF_ENVIAR_ANALISE_MEC",1060);	
	//WORKFLOW IES - FASE 01 
	define("WF_PLAN_ANALISE_MEC",  1082);	
	define("WF_PLAN_FECHADO",  	   1146); // O cуdigo antigo й 1146 - O status fechado nгo existe mais.	
	//WORKFLOW IES - FASE 02
	define("WF_PLAN_ANALISE_MEC2",  1226);
	define("WF_PLAN_FECHADO2",  	1228);		
	
	//Tipo de Documento WORKFLOW
	define("WF_TPDID_SISFOR", 		170);	
	define("WF_TPDID_SISFOR_PLAN", 	175);
	define("WF_TPDID_SISFOR_PLAN2", 194);
	define("WF_TPDID_PROJETO", 		180);
    define("WF_TPDID_SISFOR_CADASTRAMENTO", 190);
    define("WF_TPDID_SISFOR_AVALIACAO", 	191);
    define("WF_TPDID_PAGAMENTOBOLSA",		198);
    define("WF_RELATORIO_MENSAL",   208);
    define("WF_TPDID_AVALIACAOFINAL",		216);
	
	define("ESD_PROJETO_EMCADASTRAMENTO", 1100);
	define("ESD_PROJETO_VALIDADO", 1187);
	define("ESD_PROJETO_AJUSTES", 		  1229);
	
	define("ESD_PROJETO_BLOQUEADO", 1569);

	define("ESD_EM_CADASTRAMENTO", 1204);
	define("ESD_CADASTRAMENTO_FINALIZADO", 1205);

	define("ESD_AVALIACAO", 1206);
	define("ESD_ENVIADO_PAGAMENTO", 1207);
    define("ESD_ANALISE_MEC", 1224);
    define("ESD_ANALISE_COORDENADORINSTITUCIONAL", 1276);
    
    define("ESD_VALIDADO_MEC", 1083);
    
    define("ESD_EM_ELABORACAO2", 1225); //Em Elaboraзгo - FASE 2
	define("ESD_VALIDADO_MEC2", 1227); //Validado pelo MEC - FASE 2    
    
    define("ESD_PG_AGUARDANDO_AUTORIZACAO", 1255);
    define("ESD_PAGAMENTO_AUTORIZADO",		1252);
    define("ESD_PAGAMENTO_RECUSADO", 		1251);
    
    define("ESD_PAGAMENTO_AG_AUTORIZACAO_SGB",   1256);
    define("ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO", 1253);
    define("ESD_PAGAMENTO_EFETIVADO", 			 1258);
    define("ESD_PAGAMENTO_NAO_AUTORIZADO",		 1254);
    define("ESD_PAGAMENTO_ENVIADOBANCO",		 1257);
    
    
    define("ESD_RELATORIOMENSAL_EMELABORACAO", 1333);
    define("ESD_RELATORIOMENSAL_EMANALISE",    1334);
    define("ESD_RELATORIOMENSAL_EMPAGAMENTO",  1335);
    
    define("ESD_RELATORIOFINAL_EMELABORACAO", 1407);
    
    
	// workflow.acaoestadodoc
    define("AED_EQUIPEMEC_APROVAR", 	 2704);
    define("AED_COORDENACAOMEC_APROVAR", 2705);
    define("AED_DIRETORIAMEC_APROVAR",   2712);
    define("AED_PG_AUTORIZAR", 			 2854);
    define("AED_ENVIAR_PAGAMENTO_SGB",  2849);
    define("AED_NAOAUTORIZAR_PAGAMENTO", 2850);
    define("AED_RECUSAR_PAGAMENTO", 2847);
    define("AED_AUTORIZAR_RECUSADO_PAGAMENTO", 2846);
    define("AED_AUTORIZARSGB_PAGAMENTO", 2862);
    define("AED_ENVIARBANCO_PAGAMENTO", 2858);
    
    define("AED_ENVIAR_AVALFINAL_COORDENADORINST", 3250);
    define("AED_ENVIAR_AVALFINAL_EQUIPEMEC",       3251);
    define("AED_ENVIAR_AVALFINAL_COORDEMEC",       3252);
    
    

	//entidade.funcao
	define("FUN_UNIVERSIDADE", 		12);
	define("FUN_INSTITUTO", 		11);
	
	define("APPRAIZ_SISFOR", APPRAIZ."/sisfor/modulos/principal/");
}

//Ano CENSO
define("ANO_CENSO", 2013);

// tabela sisfor.nacionalidade
define("NAC_BRASIL", 10);

// tabela sisfor.subatividades
define("SUA_DEFINIR_NUM_ORIENTADORES", 2);
define("SUA_DEFINIR_NUM_PROFESSOR",    21);


// tabela sisfor.estadocivil
define("ECI_CASADO", 1);
define("ECI_UNIAO_ESTAVEL", 7);

// tabela sisfor.tipodocumento
define("TDO_RG", 2);

// tabela sisfor.formacaoescolaridade
define("FOE_ESPECIALIZACAO", 				 8);
define("FOE_MESTRADO", 						 9);
define("FOE_DOUTORADO", 					 10);
define("FOE_SUPERIOR_COMPLETO_PEDAGOGIA", 	 5);
define("FOE_SUPERIOR_COMPLETO_LICENCIATURA", 6);
define("FOE_SUPERIOR_COMPLETO_OUTRO", 		 7);
define("FOE_SUPERIOR_INCOMPLETO", 		 	 3);

// tabela sisfor.cursoformacao
define("CUF_NAO_TEM_AREA_FORMACAO_ESPECIFICA", 9999);

//Planejamento
define("FASE01", 1);
define("FASE02", 2);

// cуdigos do SGB
define("SGB_ENVIADOBANCO",					 6);
define("SGB_AUTORIZADA",					 1);
define("SGB_HOMOLOGADA",					 2);
define("SGB_PREAPROVADA",					 3);
define("SGB_ENVIADOAOSIGEF",				 4);
define("SGB_CREDITADA",						 7);
define("SGB_SACADA",						 8);
define("SGB_RESTITUIDO",					 9);
		
?>