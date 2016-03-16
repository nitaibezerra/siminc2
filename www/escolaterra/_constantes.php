<?
// tabela seguranca.perfil
define("PFL_COORDENADORESTADUAL", 1020);
define("PFL_TUTOR", 			  1021);
define("PFL_PROFESSOR", 		  1022);
define("PFL_ADMINISTRADOR", 	  1035);

define("SIS_ESCOLATERRA", 		  161);
define("APPRAIZ_ESCOLATERRA", APPRAIZ."/escolaterra/modulos/principal/");

// tabela catalogocurso2014.curso
define("CUR_ESCOLATERRA_SISFOR", 235);


// tabela escolaterra.estadocivil
define("ECI_CASADO", 1);
define("ECI_UNIAO_ESTAVEL", 7);

// tabela escolaterra.tipodocumento
define("TDO_RG", 2);

// tabela escolaterra.formacaoescolaridade
define("FOE_ESPECIALIZACAO", 				 8);
define("FOE_MESTRADO", 						 9);
define("FOE_DOUTORADO", 					 10);
define("FOE_SUPERIOR_COMPLETO_PEDAGOGIA", 	 5);
define("FOE_SUPERIOR_COMPLETO_LICENCIATURA", 6);
define("FOE_SUPERIOR_COMPLETO_OUTRO", 		 7);
define("FOE_SUPERIOR_INCOMPLETO", 		 	 3);

// tabela escolaterra.cursoformacao
define("CUF_NAO_TEM_AREA_FORMACAO_ESPECIFICA", 9999);

// tabela escolaterra.nacionalidade
define("NAC_BRASIL", 10);

// tabela workflow.tipodocumento
define("TPD_CADASTRAMENTO", 134);
define("TPD_RELATORIO", 	136);
define("TPD_RELATORIOCE", 	213);

define("TPD_PAGAMENTOBOLSA",209);

// tabela workflow.estadodocumento
define("ESD_EM_CADASTRAMENTO", 851);
define("ESD_EM_ANALISE", 	   852);
define("ESD_VALIDADO",		   853);

define("ESD_EM_ELABORACAO",    				 854);
define("ESD_EM_ANALISE_COORDENADORESTADUAL", 855);
define("ESD_LIBERADO_PAGAMENTO",		     856);
define("ESD_LIBERADO_PAGAMENTO_COORDENADORESTADUAL", 1381);

define("ESD_PROJETO_VALIDADO_SISFOR", 1187);

define("ESD_PAGAMENTO_APTO", 1348);
define("ESD_PAGAMENTO_RECUSADO", 1344);
define("ESD_PAGAMENTO_AUTORIZADO", 1345);
define("ESD_PAGAMENTO_AG_AUTORIZACAO_SGB", 1349);
define("ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO", 1346);
define("ESD_PAGAMENTO_ENVIADOBANCO", 1350);
define("ESD_PAGAMENTO_EFETIVADO", 1351);
define("ESD_PAGAMENTO_NAO_AUTORIZADO", 1347);


// tabela workflow.acaoestadodoc
define("AED_ANALISE_PARA_CADASTRAMENTO", 1929);
define("AED_ANALISE_PARA_VALIDACAO",	 1928);

define("AED_LIBERAR_PAGAMENTO", 		 1936);
define("AED_DEVOLVER_ELABORACAO", 		 1935);

define("AED_AUTORIZAR_APTO",             3127);
define("AED_AUTORIZAR_RECUSADO",         3133);

define("AED_ENVIAR_PAGAMENTO_SGB",       3129);
define("AED_NAOAUTORIZAR_PAGAMENTO",     3130);
define("AED_RECUSAR_PAGAMENTO",          3132);
define("AED_AUTORIZARSGB_PAGAMENTO", 	 3237);
define("AED_ENVIARBANCO_PAGAMENTO", 	 3240);


if(strstr($_SERVER['HTTP_HOST'],"simec-local") || strstr($_SERVER['HTTP_HOST'],"simec-d.mec.gov.br") || strstr($_SERVER['HTTP_HOST'],"simec-d")){
	
	// desenvolvimento
	define( 'SISTEMA_SGB',  'ESCTERR' );
	define( 'USUARIO_SGB',  'ESCTERR' );
	define( 'PROGRAMA_SGB', 'TERRA' );
	define( 'SENHA_SGB',    'ESCTERR_HOMOLOG' );
	define( 'WSDL_CAMINHO', 'https://hmg.fnde.gov.br/spba/Servicos?wsdl');
	define( 'WSDL_CAMINHO_CADASTRO', 'http://sgbhmg.fnde.gov.br/sistema/ws/?wsdl');

} else {
	// produчуo
	define( 'SISTEMA_SGB',  'ESCTERR' );
	define( 'USUARIO_SGB',  'ESCTERR' );
	define( 'PROGRAMA_SGB', 'TERRA' );
	define( 'SENHA_SGB',    'ZM3ANKN8WLUFSWAAT0A2TM2P2JNLY9AB' );
	define( 'WSDL_CAMINHO', 'http://www.fnde.gov.br/spba/Servicos?wsdl');
	define( 'WSDL_CAMINHO_CADASTRO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl');
}	

define("SGB_ENVIADOBANCO",					 6);
define("SGB_AUTORIZADA",					 1);
define("SGB_HOMOLOGADA",					 2);
define("SGB_PREAPROVADA",					 3);
define("SGB_ENVIADOAOSIGEF",				 4);
define("SGB_CREDITADA",						 7);
define("SGB_SACADA",						 8);
define("SGB_RESTITUIDO",					 9);


?>