<?
/* Ambiente de produзгo */

define("COD_PROGRAMA_SGB","SINDI");

// tabela seguranca.sistema
define("SIS_INDIGENA", 		   217);

// tabela seguranca.perfil
define("PFL_SUPERUSUARIO",			 1324);
define("PFL_CONTEUDISTA", 			 1325);
define("PFL_ORIENTADORESTUDO", 	1326);
define("PFL_COORDENADORADJUNTOIES",  1327);
define("PFL_SUPERVISORIES",          1328);
define("PFL_FORMADORIES",            1329);
define("PFL_COORDENADORIES", 	1330);
define("PFL_PESQUISADOR",			 1331);
define("PFL_PROFESSORALFABETIZADOR", 1333);
define("PFL_COORDENADORLOCAL", 	1334);
define("PFL_ADMINISTRADOR",			 1335);

define("PFL_COORDENADORLOCAL2014", 	1028);
define("PFL_ORIENTADORESTUDO2014", 	1029);
define("PFL_COORDENADORIES2014", 	1030);
define("PFL_FORMADORIES2014",            1033);
define("PFL_SUPERVISORIES2014",          1032);
define("PFL_COORDENADORADJUNTOIES2014",  1031);
define("PFL_PROFESSORALFABETIZADOR2014", 1027);
define("PFL_CONTEUDISTA2014", 			 1051);
define("PFL_PESQUISADOR2014",			 1050);




/*
define("PFL_CONSULTAMUNICIPAL", 833);
define("PFL_CONSULTAESTADUAL",  834);
define("PFL_EQUIPEMUNICIPALAP", 836);
define("PFL_EQUIPEESTADUALAP",  837);
define("PFL_EQUIPEMEC", 		  831);
*/




	
// tabela workflow.tipodocumento
//define("TPD_ORIENTADORESTUDO", 83);
define("TPD_COORDENADORIES",    210);
define("TPD_PAGAMENTOBOLSA",   212);
define("TPD_FLUXOMENSARIO",    211);
//define("TPD_FORMACAOINICIAL",  93);
//define("TPD_FLUXOTURMA",  	   96);
	
// tabela workflow.estadodocumento

define("ESD_ELABORACAO_COORDENADOR_IES",   			  		  1355);
define("ESD_ANALISE_COORDENADOR_IES", 	   			  		  1354);
define("ESD_VALIDADO_COORDENADOR_IES", 	   			  		  1353);
define("ESD_EM_ABERTO_MENSARIO", 					  		  1357);
define("ESD_ENVIADO_MENSARIO",			  			  		  1362);
define("ESD_APROVADO_MENSARIO",					  	  		  1356);


define("ESD_PAGAMENTO_APTO", 	 		   			  		  1369);
define("ESD_PAGAMENTO_AUTORIZADO", 	 	   			  		  1365);
define("ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO",		  		  1366);
define("ESD_PAGAMENTO_RECUSADO", 		   			  		  1367);
define("ESD_PAGAMENTO_NAO_AUTORIZADO",			  	  		  1368);
define("ESD_PAGAMENTO_EFETIVADO",			  	  		  	  1372);
define("ESD_PAGAMENTO_ENVIADOBANCO",		  	  		  	  1371);
define("ESD_PAGAMENTO_AG_AUTORIZACAO_SGB",	  	  		  	  1370);


/*
 * 
define("ESD_ELABORACAO_COORDENADOR_LOCAL", 			  		  561);
define("ESD_VALIDADO_COORDENADOR_LOCAL", 			  		  563);
define("ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL", 		  592);
define("ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL", 597);
 
define("ESD_INVALIDADO_MENSARIO",		  			  		  602);
define("ESD_ABERTO_FORMACAOINICIAL",	  			  		  610);
define("ESD_FECHADO_TURMA",	  			  		  			  630);
define("ESD_FECHADO_FORMACAOINICIAL",					  	  611);
*/



// tabela sispacto.identificacaousuario
define("IUS_AVALIADOR_MEC", 1339);



// tabela workflow.acaoestadodoc
define("AED_APROVAR_MENSARIO", 				 3144);
define("AED_APROVAR_EMABERTO_MENSARIO",	 	 3141);
define("AED_AUTORIZAR_APTO", 	 		     3162);
define("AED_AUTORIZAR_RECUSADO", 		     3158);
define("AED_AUTORIZARSGB_PAGAMENTO", 		 3166);
define("AED_ENVIARBANCO_PAGAMENTO", 		 3151);
define("AED_ENVIAR_PAGAMENTO_SGB", 		     3146);
define("AED_NAOAUTORIZAR_PAGAMENTO", 		 3147);
define("AED_RECUSAR_PAGAMENTO",  		     3148);
define("AED_EFETIVAR_PAGAMENTO", 		     3149);
define("AED_REABRIR_MENSARIO_APROVADO",      3139);

/*
define("AED_INVALIDAR_MENSARIO", 		     1549);
define("AED_ENVIAR_MENSARIO", 	 		     1553);
define("AED_AUTORIZAR_TROCA_ORIENTADORES",   1539);
define("AED_APROVAR_CADASTRO_ORIENTADORES",  1452);
define("AED_REPROVAR_CADASTRO_ORIENTADORES", 1453);
*/

define("AED_REALIZAR_PAGAMENTO", 		 	 3150);
define("AED_REALIZAR_PAGAMENTO_BANCO", 		 3168);


//define("AED_INVALIDAR_EMANALISE_MENSARIO",	 1709);



define("SGB_ENVIADOBANCO",					 6);
define("SGB_AUTORIZADA",					 1);
define("SGB_HOMOLOGADA",					 2);
define("SGB_PREAPROVADA",					 3);
define("SGB_ENVIADOAOSIGEF",				 4);
define("SGB_CREDITADA",						 7);
define("SGB_SACADA",						 8);
define("SGB_RESTITUIDO",					 9);




// tabela sispacto.nacionalidade
define("NAC_BRASIL", 10);

// tabela sispacto.subatividades
define("SUA_DEFINIR_NUM_ORIENTADORES", 2);
define("SUA_DEFINIR_NUM_PROFESSOR",    21);


// tabela sispacto.estadocivil
define("ECI_CASADO", 1);
define("ECI_UNIAO_ESTAVEL", 7);

// tabela sispacto.tipodocumento
define("TDO_RG", 2);

// tabela sispacto.formacaoescolaridade
define("FOE_ESPECIALIZACAO", 				 8);
define("FOE_MESTRADO", 						 9);
define("FOE_DOUTORADO", 					 10);
define("FOE_SUPERIOR_COMPLETO_PEDAGOGIA", 	 5);
define("FOE_SUPERIOR_COMPLETO_LICENCIATURA", 6);
define("FOE_SUPERIOR_COMPLETO_OUTRO", 		 7);
define("FOE_SUPERIOR_INCOMPLETO", 		 	 3);

// tabela sispacto.cursoformacao
define("CUF_NAO_TEM_AREA_FORMACAO_ESPECIFICA", 9999);
	
	if(strstr($_SERVER['HTTP_HOST'],"simec-local") || strstr($_SERVER['HTTP_HOST'],"simec-d.mec.gov.br") || strstr($_SERVER['HTTP_HOST'],"simec-d")){
		// desenvolvimento
		define( 'SISTEMA_SGB',  'SINDI' );
		define( 'USUARIO_SGB',  'SINDI' );
		define( 'PROGRAMA_SGB', 'SINDI' );
		define( 'SENHA_SGB',    'SINDI_HOMOLOG' );
		define( 'WSDL_CAMINHO', 'http://dev.fnde.gov.br/spba/Servicos?wsdl');
		define( 'WSDL_CAMINHO_CADASTRO', 'http://sgbhmg.fnde.gov.br/sistema/ws?wsdl');

	} else {
		// produзгo
		define( 'SISTEMA_SGB',  'SINDI' );
		define( 'USUARIO_SGB',  'SINDI' );
		define( 'PROGRAMA_SGB', 'SINDI' );
		define( 'SENHA_SGB',    '$YC,#QRQ9-C|YLOR#OLZOEUM,,I1R#DI' );
		define( 'WSDL_CAMINHO', 'http://www.fnde.gov.br/spba/Servicos?wsdl');
		define( 'WSDL_CAMINHO_CADASTRO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl');
	}
	

define("APPRAIZ_SISINDIGENA", APPRAIZ."/sisindigena2/modulos/principal/");

$_SERIE_TURMA = array("01"  => "1є ano",
					  "02"  => "2є ano/ 1Є sйrie",
					  "03"  => "3є ano/ 2Є sйrie",
					  "MS" => "Multisseriada/ Multietapa"
					  );


$_TIPO_ORIENTADORES = array("tutoresproletramento" 		  => "Tutores Prу-Letramento",
							"tutoresredesemproletramento" => "Professores da rede que nгo foram Tutores do Prу-Letramento",
							"profissionaismagisterio" 	  => "Profissionais do Magistйrio com experiкncia em formaзгo de professores"
							);

$_PERGUNTA_JUSTIFICATIVA = array("tutoresproletramento" 		=> "1.	Por que todas as vagas nгo foram preenchidas com tutores do Pro-Letramento?",
								 "tutoresredesemproletramento" 	=> "2.	Por que nгo foram escolhidos professores da rede para ocupar as vagas remanescentes de Orientadores de Estudo?"
								 );
								 
$OPT_AV = array("frequencia" 		   => array(0=>array("codigo"=>"1.0","descricao"=>"Presenзa integral"),1=>array("codigo"=>"0.5","descricao"=>"Presenзa parcial"),2=>array("codigo"=>"0.0","descricao"=>"Ausкncia")),
				"atividadesrealizadas" => array(0=>array("codigo"=>"1.0","descricao"=>"Realizou as atividades integralmente"),1=>array("codigo"=>"0.7","descricao"=>"Realizou as atividades suficientemente"),2=>array("codigo"=>"0.4","descricao"=>"Realizou as atividades insuficientemente"),3=>array("codigo"=>"0.0","descricao"=>"Nгo realizou as atividades")),
				"avaliacaoexterna" 	   => array(0=>array("codigo"=>"1.0","descricao"=>"Уtimo"),1=>array("codigo"=>"0.8","descricao"=>"Bom"),2=>array("codigo"=>"0.5","descricao"=>"Regular"),3=>array("codigo"=>"0.2","descricao"=>"Ruim"),3=>array("codigo"=>"0.0","descricao"=>"Pйssimo"))

);

					
?>