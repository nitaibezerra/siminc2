<?

/* Ambiente de produзгo */

define("COD_PROGRAMA_SGB","PCT");

// tabela seguranca.sistema
define("SIS_SISPACTO", 		   182);
define("SIS_PAR", 		   		23);

// tabela seguranca.perfil
define("PFL_COORDENADORLOCAL", 	1380);
define("PFL_ORIENTADORESTUDO", 	1381);
define("PFL_COORDENADORIES", 	1378);
define("PFL_CONSULTAMUNICIPAL", 1383);
define("PFL_CONSULTAESTADUAL", 	1382);
define("PFL_EQUIPEMUNICIPALAP", 1386);
define("PFL_EQUIPEESTADUALAP", 	1385);
define("PFL_EQUIPEMEC", 		1384);
define("PFL_ADMINISTRADOR",     1391);
define("PFL_CONSULTAMEC",		1377);

define("PFL_FORMADORIES",            1390);
define("PFL_FORMADORIESP",			 1375);
define("PFL_SUPERVISORIES",          1389);
define("PFL_COORDENADORADJUNTOIES",  1388);
define("PFL_PROFESSORALFABETIZADOR", 1379);
define("PFL_SUPERUSUARIO",			 1376);

define("PFL_ORIENTADORESTUDO2014",	 	 1120);
define("PFL_PROFESSORALFABETIZADOR2014", 1118);
define("PFL_COORDENADORIES2014", 		 1117);
define("PFL_FORMADORIES2014",            1131);
define("PFL_SUPERVISORIES2014",          1130);
define("PFL_COORDENADORADJUNTOIES2014",  1129);

define("PFL_EQMUNAP_PAR", 674);
define("PFL_EQESTAP_PAR", 672);



// tabela workflow.tipodocumento
define("TPD_ORIENTADORESTUDO", 228);
define("TPD_PROJETOIES",       226);
define("TPD_PAGAMENTOBOLSA",   227);
define("TPD_FLUXOMENSARIO",    229);
define("TPD_FORMACAOINICIAL",  230);
define("TPD_FLUXOTURMA",  	   231);
define("TPD_FLUXORELATORIOFINAL",   232);

// tabela workflow.estadodocumento
define("ESD_ELABORACAO_COORDENADOR_LOCAL", 			  		  1510);
define("ESD_ANALISE_COORDENADOR_LOCAL", 					  1511);
define("ESD_VALIDADO_COORDENADOR_LOCAL", 			  		  1512);
define("ESD_ELABORACAO_COORDENADOR_IES",   			  		  1515);
define("ESD_ANALISE_COORDENADOR_IES", 	   			  		  1516);
define("ESD_VALIDADO_COORDENADOR_IES", 	   			  		  1517);
define("ESD_FECHADO_TURMA",	  			  		  			  1509);
define("ESD_EM_ABERTO_MENSARIO", 					  		  1521);
define("ESD_ENVIADO_MENSARIO",			  			  		  1529);
define("ESD_INVALIDADO_MENSARIO",		  			  		  1527);
define("ESD_APROVADO_MENSARIO",					  	  		  1513);
define("ESD_ABERTO_FORMACAOINICIAL",	  			  		  1532);
define("ESD_FECHADO_FORMACAOINICIAL",					  	  1533);
define("ESD_PAGAMENTO_APTO", 	 		   			  		  1535);
define("ESD_PAGAMENTO_AUTORIZADO", 	 	   			  		  1520);
define("ESD_PAGAMENTO_ENVIADOBANCO",		  	  		  	  1537);
define("ESD_PAGAMENTO_EFETIVADO",			  	  		  	  1538);
define("ESD_PAGAMENTO_NAO_AUTORIZADO",			  	  		  1531);
define("ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO",		  		  1530);
define("ESD_PAGAMENTO_RECUSADO", 		   			  		  1519);
define("ESD_PAGAMENTO_AG_AUTORIZACAO_SGB",	  	  		  	  1536);

define("ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL", 		  990);
define("ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL", 999);

define("ESD_RELATORIOFINAL_EMELABORACAO",					  1016);
define("ESD_ORCAMENTO_APROVADO",							  893);





// tabela sispacto.identificacaousuario
define("IUS_AVALIADOR_MEC", 1052684);



// tabela workflow.acaoestadodoc
define("AED_APROVAR_EMABERTO_MENSARIO",	 	 3496);
define("AED_APROVAR_MENSARIO", 				 3495);
define("AED_AUTORIZAR_APTO", 	 		     3523);
define("AED_AUTORIZAR_RECUSADO", 		     3526);
define("AED_VALIDAR_COORDENADORLOCAL",       3493);

define("AED_APROVAR_CADASTRO_ORIENTADORES",  3494);
define("AED_REPROVAR_CADASTRO_ORIENTADORES", 3485);
define("AED_ENVIAR_MENSARIO", 	 		     3531);
define("AED_INVALIDAR_EMANALISE_MENSARIO",	 3530);

define("AED_REALIZAR_PAGAMENTO", 		 	 3570);
define("AED_ENVIAR_PAGAMENTO_SGB", 		     3565);
define("AED_RECUSAR_PAGAMENTO",  		     3520);
define("AED_NAOAUTORIZAR_PAGAMENTO", 		 3557);
define("AED_ENVIARBANCO_PAGAMENTO", 		 3567);
define("AED_REALIZAR_PAGAMENTO_BANCO", 		 3568);
define("AED_EFETIVAR_PAGAMENTO", 		     3554);
define("AED_AUTORIZARSGB_PAGAMENTO", 		 3553);

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
	

define("APPRAIZ_SISPACTO", APPRAIZ."/sispacto3/modulos/principal/");

$_SERIE_TURMA = array("01"  => "1є ano",
					  "02"  => "2є ano/ 1Є sйrie",
					  "03"  => "3є ano/ 2Є sйrie",
					  "MS" => "Multisseriada/ Multietapa"
					  );


$_TIPO_ORIENTADORES = array("orientadorsispacto2014" 	  => "Orientador de Estudo do Pacto 2014 recomendado para certificaзгo",
							"professorsispacto2014" 	  => "Professor Alfabetizador do Pacto 2014 recomendado para certificaзгo",
							"tutoresproletramento" 		  => "Tutores Prу-Letramento",
							"tutoresredesemproletramento" => "Professores da rede que nгo foram Tutores do Prу-Letramento",
							"profissionaismagisterio" 	  => "Profissionais do Magistйrio com experiкncia em formaзгo de professores"
							);

$_PERGUNTA_JUSTIFICATIVA = array("orientadorsispacto2014" 		=> "1.	Por que todas as vagas nгo foram preenchidas com Orientadores de Estudo do Pacto 2014 recomendados para certificaзгo?",
								 "professorsispacto2014" 		=> "2.	Por que todas as vagas nгo foram preenchidas com Professores Alfabetizadores do Pacto 2014 recomendados para certificaзгo?",
								 "tutoresproletramento" 		=> "3.	Por que todas as vagas nгo foram preenchidas com tutores do Pro-Letramento?",
								 "tutoresredesemproletramento" 	=> "4.	Por que nгo foram escolhidos professores da rede para ocupar as vagas remanescentes de Orientadores de Estudo?"
								 );
								 
$OPT_AV = array("frequencia" 		   => array(0=>array("codigo"=>"1.0","descricao"=>"Presenзa integral"),1=>array("codigo"=>"0.5","descricao"=>"Presenзa parcial"),2=>array("codigo"=>"0.0","descricao"=>"Ausкncia")),
				"atividadesrealizadas" => array(0=>array("codigo"=>"1.0","descricao"=>"Realizou as atividades integralmente"),1=>array("codigo"=>"0.7","descricao"=>"Realizou as atividades suficientemente"),2=>array("codigo"=>"0.4","descricao"=>"Realizou as atividades insuficientemente"),3=>array("codigo"=>"0.0","descricao"=>"Nгo realizou as atividades")),
				"avaliacaoexterna" 	   => array(0=>array("codigo"=>"1.0","descricao"=>"Уtimo"),1=>array("codigo"=>"0.8","descricao"=>"Bom"),2=>array("codigo"=>"0.5","descricao"=>"Regular"),3=>array("codigo"=>"0.2","descricao"=>"Ruim"),3=>array("codigo"=>"0.0","descricao"=>"Pйssimo"))

);

define("APRENDIZAGEM_MATEMATICA",       16);
define("APRENDIZAGEM_PORTUGUES",        8);
define("APRENDIZAGEM_MATERIALDIDATICO", 7);

$_HIERARQUIA_PFL = array(PFL_FORMADORIES           => array(PFL_ORIENTADORESTUDO),
						 PFL_SUPERVISORIES 		   => array(PFL_FORMADORIES),
						 PFL_COORDENADORADJUNTOIES => array(PFL_SUPERVISORIES, PFL_COORDENADORLOCAL),
						 PFL_ORIENTADORESTUDO      => array(PFL_PROFESSORALFABETIZADOR)); 

					
?>