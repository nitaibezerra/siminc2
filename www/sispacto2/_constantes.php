<?

/* Ambiente de produзгo */

define("COD_PROGRAMA_SGB","PCT");

// tabela seguranca.sistema
define("SIS_SISPACTO", 		   181);
define("SIS_PAR", 		   		23);

// tabela seguranca.perfil
define("PFL_COORDENADORLOCAL", 	1119);
define("PFL_ORIENTADORESTUDO", 	1120);
define("PFL_COORDENADORIES", 	1117);
define("PFL_CONSULTAMUNICIPAL", 1123);
define("PFL_CONSULTAESTADUAL", 	1121);
define("PFL_EQUIPEMUNICIPALAP", 1127);
define("PFL_EQUIPEESTADUALAP", 	1126);
define("PFL_EQUIPEMEC", 		1124);
define("PFL_ADMINISTRADOR",     1125);
define("PFL_CONSULTAMEC",		1116);

define("PFL_FORMADORIES",            1131);
define("PFL_FORMADORIESP",			 1168);
define("PFL_SUPERVISORIES",          1130);
define("PFL_COORDENADORADJUNTOIES",  1129);
define("PFL_PROFESSORALFABETIZADOR", 1118);
define("PFL_SUPERUSUARIO",			 1115);

define("PFL_ORIENTADORESTUDO2013",	 	 827);
define("PFL_PROFESSORALFABETIZADOR2013", 849);
define("PFL_COORDENADORIES2013", 		 832);
define("PFL_FORMADORIES2013",            848);
define("PFL_SUPERVISORIES2013",          847);
define("PFL_COORDENADORADJUNTOIES2013",  846);

define("PFL_EQMUNAP_PAR", 674);
define("PFL_EQESTAP_PAR", 672);



// tabela workflow.tipodocumento
define("TPD_ORIENTADORESTUDO", 157);
define("TPD_ORIENTADORIES",    155);
define("TPD_PAGAMENTOBOLSA",   156);
define("TPD_FLUXOMENSARIO",    158);
define("TPD_FORMACAOINICIAL",  159);
define("TPD_FLUXOTURMA",  	   160);
define("TPD_FLUXORELATORIOFINAL",   161);

// tabela workflow.estadodocumento
define("ESD_ELABORACAO_COORDENADOR_LOCAL", 			  		  986);
define("ESD_ANALISE_COORDENADOR_LOCAL", 					  987);
define("ESD_VALIDADO_COORDENADOR_LOCAL", 			  		  988);
define("ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL", 		  990);
define("ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL", 999);
define("ESD_ELABORACAO_COORDENADOR_IES",   			  		  991);
define("ESD_ANALISE_COORDENADOR_IES", 	   			  		  992);
define("ESD_VALIDADO_COORDENADOR_IES", 	   			  		  993);
define("ESD_FECHADO_TURMA",	  			  		  			  985);
define("ESD_ABERTO_FORMACAOINICIAL",	  			  		  1009);
define("ESD_FECHADO_FORMACAOINICIAL",					  	  1010);
define("ESD_EM_ABERTO_MENSARIO", 					  		  998);
define("ESD_ENVIADO_MENSARIO",			  			  		  1006);
define("ESD_INVALIDADO_MENSARIO",		  			  		  1004);
define("ESD_APROVADO_MENSARIO",					  	  		  989);

define("ESD_PAGAMENTO_APTO", 	 		   			  		  1012);
define("ESD_PAGAMENTO_AUTORIZADO", 	 	   			  		  997);
define("ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO",		  		  1007);
define("ESD_PAGAMENTO_RECUSADO", 		   			  		  996);

define("ESD_PAGAMENTO_NAO_AUTORIZADO",			  	  		  1008);
define("ESD_PAGAMENTO_EFETIVADO",			  	  		  	  1015);
define("ESD_PAGAMENTO_ENVIADOBANCO",		  	  		  	  1014);
define("ESD_PAGAMENTO_AG_AUTORIZACAO_SGB",	  	  		  	  1013);

define("ESD_RELATORIOFINAL_EMELABORACAO",					  1016);
define("ESD_ORCAMENTO_APROVADO",							  893);





// tabela sispacto.identificacaousuario
define("IUS_AVALIADOR_MEC", 700128);



// tabela workflow.acaoestadodoc
define("AED_APROVAR_CADASTRO_ORIENTADORES",  2297);
define("AED_REPROVAR_CADASTRO_ORIENTADORES", 2298);
define("AED_APROVAR_MENSARIO", 				 2363);
define("AED_ENVIAR_MENSARIO", 	 		     2356);
define("AED_APROVAR_EMABERTO_MENSARIO",	 	 2357);
define("AED_INVALIDAR_EMANALISE_MENSARIO",	 2361);

define("AED_AUTORIZAR_APTO", 	 		     2404);
define("AED_AUTORIZAR_RECUSADO", 		     2351);
define("AED_REALIZAR_PAGAMENTO", 		 	 2364);
define("AED_ENVIAR_PAGAMENTO_SGB", 		     2352);
define("AED_RECUSAR_PAGAMENTO",  		     2355);
define("AED_NAOAUTORIZAR_PAGAMENTO", 		 2353);
define("AED_ENVIARBANCO_PAGAMENTO", 		 2365);
define("AED_REALIZAR_PAGAMENTO_BANCO", 		 2410);
define("AED_EFETIVAR_PAGAMENTO", 		     2354);
define("AED_AUTORIZARSGB_PAGAMENTO", 		 2408);

define("AED_INVALIDAR_MENSARIO", 		     1549);
define("AED_AUTORIZAR_TROCA_ORIENTADORES",   1539);


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
define("FOE_FUNDAMENTAL_INC", 				 11);
define("FOE_FUNDAMENTAL_COM", 				 12);
define("FOE_MEDIO_INC", 					 1);
define("FOE_MEDIO_COM", 				 	 2);

// tabela sispacto.cursoformacao
define("CUF_NAO_TEM_AREA_FORMACAO_ESPECIFICA", 9999);

if(strstr($_SERVER['HTTP_HOST'],"simec-local") || strstr($_SERVER['HTTP_HOST'],"simec-d.mec.gov.br") || strstr($_SERVER['HTTP_HOST'],"simec-d")){
	// desenvolvimento
	define( 'SISTEMA_SGB',  'PACTO' );
	define( 'USUARIO_SGB',  'PCT' );
	define( 'PROGRAMA_SGB', 'PCT' );
	define( 'SENHA_SGB',    'PCT_HOMOLOG' );
	define( 'WSDL_CAMINHO', 'https://hmg.fnde.gov.br/spba/Servicos?wsdl');
	define( 'WSDL_CAMINHO_CADASTRO', 'http://sgbhmg.fnde.gov.br/sistema/ws/?wsdl');
	
} else {
	// produзгo
	define( 'SISTEMA_SGB',  'PACTO' );
	define( 'USUARIO_SGB',  'PCT' );
	define( 'PROGRAMA_SGB', 'PCT' );
	define( 'SENHA_SGB',    'AXD*0MI!4WBY1GI:LC+YQF@JHUN3|TMA' );
	define( 'WSDL_CAMINHO', 'http://www.fnde.gov.br/spba/Servicos?wsdl');
	define( 'WSDL_CAMINHO_CADASTRO', 'http://sgb.fnde.gov.br/sistema/ws/?wsdl');
}
	

define("APPRAIZ_SISPACTO", APPRAIZ."/sispacto2/modulos/principal/");

$_SERIE_TURMA = array("01"  => "1є ano",
					  "02"  => "2є ano/ 1Є sйrie",
					  "03"  => "3є ano/ 2Є sйrie",
					  "MS" => "Multisseriada/ Multietapa"
					  );


$_TIPO_ORIENTADORES = array("orientadorsispacto2013" 	  => "Orientador de Estudo do Pacto 2013 recomendado para certificaзгo",
							"professorsispacto2013" 	  => "Professor Alfabetizador do Pacto 2013 recomendado para certificaзгo",
							"tutoresproletramento" 		  => "Tutores Prу-Letramento",
							"tutoresredesemproletramento" => "Professores da rede que nгo foram Tutores do Prу-Letramento",
							"profissionaismagisterio" 	  => "Profissionais do Magistйrio com experiкncia em formaзгo de professores"
							);

$_PERGUNTA_JUSTIFICATIVA = array("orientadorsispacto2013" 		=> "1.	Por que todas as vagas nгo foram preenchidas com Orientadores de Estudo do Pacto 2013 recomendados para certificaзгo?",
								 "professorsispacto2013" 		=> "2.	Por que todas as vagas nгo foram preenchidas com Professores Alfabetizadores do Pacto 2013 recomendados para certificaзгo?",
								 "tutoresproletramento" 		=> "3.	Por que todas as vagas nгo foram preenchidas com tutores do Pro-Letramento?",
								 "tutoresredesemproletramento" 	=> "4.	Por que nгo foram escolhidos professores da rede para ocupar as vagas remanescentes de Orientadores de Estudo?"
								 );
								 
$OPT_AV = array("frequencia" 		   => array(0=>array("codigo"=>"1.0","descricao"=>"Presenзa integral"),1=>array("codigo"=>"0.5","descricao"=>"Presenзa parcial"),2=>array("codigo"=>"0.0","descricao"=>"Ausкncia")),
				"atividadesrealizadas" => array(0=>array("codigo"=>"1.0","descricao"=>"Realizou as atividades integralmente"),1=>array("codigo"=>"0.7","descricao"=>"Realizou as atividades suficientemente"),2=>array("codigo"=>"0.4","descricao"=>"Realizou as atividades insuficientemente"),3=>array("codigo"=>"0.0","descricao"=>"Nгo realizou as atividades")),
				"avaliacaoexterna" 	   => array(0=>array("codigo"=>"1.0","descricao"=>"Уtimo"),1=>array("codigo"=>"0.8","descricao"=>"Bom"),2=>array("codigo"=>"0.5","descricao"=>"Regular"),3=>array("codigo"=>"0.2","descricao"=>"Ruim"),3=>array("codigo"=>"0.0","descricao"=>"Pйssimo"))

);

define("APRENDIZAGEM_MATEMATICA",       17);
define("APRENDIZAGEM_PORTUGUES",        11);
define("APRENDIZAGEM_MATERIALDIDATICO", 7);

					
?>