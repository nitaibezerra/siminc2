<?
if( ($_SERVER['HTTP_HOST'] == 'simec-local' ||
	$_SERVER['HTTP_HOST'] == 'simec-d.mec.gov.br' || 
	$_SERVER['HTTP_HOST'] == 'simec-d') && $_SESSION['baselogin'] == 'simec_desenvolvimento' ){

	define("TPDID_ANALISE_SITUACAO"	, 79);
	
	define("ESDID_AGUARDANDO_ANALISE"		, 530);
	define("ESDID_ANALISADO"				, 531);
	define("ESDID_REJEITADO"				, 532);
	define("ESDID_CANCELADO_PELO_PROFESSOR"	, 533);
	define("ESDID_ENVIADO_CORRECAO"			, 534);
	
	define("AEDID_FINALIZAR_ANALISE"	, 1307);
	define("AEDID_REJEITAR_ATUACAO"		, 1308);
	define("AEDID_CANCELAR_ATUACAO"		, 1309);
	define("AEDID_CANCELAR_ATUACAO_REJ"	, 1313);
	define("AEDID_CANCELAR_ATUACAO_ANAL", 1312);
	define("AEDID_ENVIAR_CORRECAO"		, 1310);
	define("AEDID_ENVIAR_ANALISE"		, 1311);
	
	define("TPDID"	, 68);
	
	define("PFL_SUPER_USUARIO"				, 742);
	define("PFL_PROFESSOR"					, 743);
	define("PFL_SECRETARIO_ESTADUAL"		, 745);
	define("PFL_SUB_SECRETARIO_ESTADUAL"	, 801);
	define("PFL_SECRETARIO_MUNICIPAL"		, 755);
	define("PFL_SUB_SECRETARIO_MUNICIPAL"	, 800);
	
	define("DT_INICIO_PROFESSOR" , '01-04-2012');
	define("DT_FIM_PROFESSOR"	 , '31-05-2012');
	
	/*
	if( date('Y') == '2014' ){
		define("DT_INICIO_PROGRAMA" , '2010-01-01');
		define("DT_FIM_PROGRAMA"	, '2012-12-31');
	}else{
		define("DT_INICIO_PROGRAMA" , (date('Y')-1).'-01-01');
		define("DT_FIM_PROGRAMA"	, (date('Y')-1).'-12-31');
	}

	
	//arquivo funcoes, altera o valor das datas abaixo.
	define("DT_INICIO_PROGRAMA" , '2010-01-01');
	define("DT_FIM_PROGRAMA"	, '2014-12-31');
	*/

	/*
	 * PARÂMETROS DE SISTEMA
	 * */
	if( isset($db) ){
		define("PRAZO_DECURSO_DE_PRAZO"	, $db->pegaUm("SELECT pdaprazoaprovacao 								FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("PRAZO_REABERTURA"		, $db->pegaUm("SELECT pdaprazoreabertura 								FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_INICIO_SOLICITACAO" 	, $db->pegaUm("SELECT to_char(pdadatainiciosolicitacao,'YYYYMMDD')  	FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_FIM_SOLICITACAO"    	, $db->pegaUm("SELECT to_char(pdadatafimsolicitacao,'YYYYMMDD')  		FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_INICIO_APROVACAO"   	, $db->pegaUm("SELECT to_char(pdadatainicioaprovacao,'YYYYMMDD') 		FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_FIM_APROVACAO"    	, $db->pegaUm("SELECT to_char(pdadatafimaprovacao,'YYYYMMDD') 			FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
	}
	
	
	//WORKFLOW
	define("WF_FIES1_ENVIAR_SOLICITACAO",				1235);
	define("WF_FIES1_ENVIAR_SOLICITACAO_REENV",			1300);
	define("WF_FIES1_ENVIAR_SOLICITACAO_REENV_PRAZO",	1304);
	define("WF_FIES1_CONFIRMAR_SOLICITACAO",			1236);
	define("WF_FIES1_APROVAR_SOLICITACAO",				1237);
	define("WF_FIES1_CANCELAR_ABATIMENTO_PEND",			1298);
	define("WF_FIES1_CANCELAR_ABATIMENTO_APRV",			1293);
	define("WF_FIES1_CANCELAR_ABATIMENTO_REEN",			1301);
	define("WF_FIES1_CANCELAR_ABATIMENTO_REEN_PRAZO",	1306);
	define("WF_FIES1_REABRIR_ABATIMENTO",				1294);
	define("WF_FIES1_REABRIR_ABATIMENTO2",				1295);
	define("WF_FIES1_REJEITAR_ABATIMENTO",				1297);
	define("WF_FIES1_REJEITAR_PRAZO_ABATIMENTO", 		1296);
	define("WF_FIES1_REJEITAR_PRAZO_REENVIO", 			1296);
	define("WF_FIES1_ENVIAR_PROCESSAMENTO_BANCARIO", 	1314);
	
	define("WF_FIES1_EM_SOLICITACAO_PELO_PROFESSOR",									479);
	define("WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL",	481);
	define("WF_FIES1_APROVADA",															482);
	define("WF_FIES1_CANCELADA",														526);
	define("WF_FIES1_REENVIO",															527);
	define("WF_FIES1_REENVIO_PRAZO",													529);
	define("WF_FIES1_REJEITADO",														528);
	define("WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO",									535);
	
}else{
	
	define("TPDID_ANALISE_SITUACAO"	, 101);
	
	define("ESDID_AGUARDANDO_ANALISE"		, 665);
	define("ESDID_ANALISADO"				, 666);
	define("ESDID_REJEITADO"				, 667);
	define("ESDID_CANCELADO_PELO_PROFESSOR"	, 668);
	define("ESDID_ENVIADO_CORRECAO"			, 669);
	
	define("AEDID_FINALIZAR_ANALISE"	, 1672);
	define("AEDID_REJEITAR_ATUACAO"		, 1674);
	define("AEDID_CANCELAR_ATUACAO"		, 1675);
	define("AEDID_CANCELAR_ATUACAO_REJ"	, 1677);
	define("AEDID_CANCELAR_ATUACAO_ANAL", 1676);
	define("AEDID_ENVIAR_CORRECAO"		, 1673);
	define("AEDID_ENVIAR_ANALISE"		, 1678);
	
	define("TPDID"	, 102);
	
	define("WF_FIES1_EM_SOLICITACAO_PELO_PROFESSOR",									670);
	define("WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL",	671);
	define("WF_FIES1_APROVADA",															672);
	define("WF_FIES1_CANCELADA",														673);
	define("WF_FIES1_REENVIO",															674);
	define("WF_FIES1_REENVIO_PRAZO",													676);
	define("WF_FIES1_REJEITADO",														675);
	define("WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO",									677);
	
	define("WF_FIES1_ENVIAR_SOLICITACAO",				1679);
	define("WF_FIES1_ENVIAR_SOLICITACAO_REENV",			1688);
	define("WF_FIES1_ENVIAR_SOLICITACAO_REENV_PRAZO",	1691);
	define("WF_FIES1_CONFIRMAR_SOLICITACAO",			1236);
	define("WF_FIES1_APROVAR_SOLICITACAO",				1683);
	define("WF_FIES1_CANCELAR_ABATIMENTO_PEND",			1684);
	define("WF_FIES1_CANCELAR_ABATIMENTO_APRV",			1685);
	define("WF_FIES1_CANCELAR_ABATIMENTO_REEN",			1690);
	define("WF_FIES1_CANCELAR_ABATIMENTO_REEN_PRAZO",	1693);
	define("WF_FIES1_REABRIR_ABATIMENTO",				1681);
	define("WF_FIES1_REABRIR_ABATIMENTO2",				1687);
	define("WF_FIES1_REJEITAR_ABATIMENTO",				1680);
	define("WF_FIES1_REJEITAR_PRAZO_ABATIMENTO", 		1682);
	define("WF_FIES1_REJEITAR_PRAZO_REENVIO", 			1689);
	define("WF_FIES1_ENVIAR_PROCESSAMENTO_BANCARIO", 	1686);
	
	//PERFIL
	define("PFL_SUPER_USUARIO"				, 872);
	define("PFL_ADMINISTRADOR"				, 929);
	define("PFL_PROFESSOR"					, 873);
	define("PFL_SECRETARIO_ESTADUAL"		, 875);
	define("PFL_SUB_SECRETARIO_ESTADUAL"	, 931);
	define("PFL_SECRETARIO_MUNICIPAL"		, 885);
	define("PFL_SUB_SECRETARIO_MUNICIPAL"	, 930);
	
	define("DT_INICIO_PROFESSOR" , '01-04-2012');
	define("DT_FIM_PROFESSOR"	 , '31-05-2012');
	
	/*
	if( date('Y') == '2014' ){
		define("DT_INICIO_PROGRAMA" , '2010-01-01');
		define("DT_FIM_PROGRAMA"	, '2012-12-31');
	}else{
		define("DT_INICIO_PROGRAMA" , (date('Y')-1).'-01-01');
		define("DT_FIM_PROGRAMA"	, (date('Y')-1).'-12-31');
	}

	
	//arquivo funcoes, altera o valor das datas abaixo.
	define("DT_INICIO_PROGRAMA" , '2010-01-01');
	define("DT_FIM_PROGRAMA"	, '2014-12-31');
	*/

	/*
	 * PARÂMETROS DE SISTEMA
	 * */
	if( isset($db) ){
		define("PRAZO_DECURSO_DE_PRAZO"	, $db->pegaUm("SELECT pdaprazoaprovacao 								FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("PRAZO_REABERTURA"		, $db->pegaUm("SELECT pdaprazoreabertura 								FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_INICIO_SOLICITACAO" 	, $db->pegaUm("SELECT to_char(pdadatainiciosolicitacao,'YYYYMMDD')  	FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_FIM_SOLICITACAO"    	, $db->pegaUm("SELECT to_char(pdadatafimsolicitacao,'YYYYMMDD')  		FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_INICIO_APROVACAO"   	, $db->pegaUm("SELECT to_char(pdadatainicioaprovacao,'YYYYMMDD') 		FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
		define("DT_FIM_APROVACAO"    	, $db->pegaUm("SELECT to_char(pdadatafimaprovacao,'YYYYMMDD') 			FROM fiesabatimento.parametrosabatimento WHERE pdastatus = 'A'"));
	}
	

	
}



//arquivo funcoes, altera o valor das datas abaixo.
/*
define("DT_INICIO_PROGRAMA_VALIDA", 20150318);
if( date('Ymd') > DT_INICIO_PROGRAMA_VALIDA ) {
	define("DT_INICIO_PROGRAMA", '2010-01-01');
	define("DT_FIM_PROGRAMA", '2014-12-31');
}else{
	define("DT_INICIO_PROGRAMA", '2010-01-01');
	define("DT_FIM_PROGRAMA", '2012-12-31');
}

dbg(DT_FIM_PROGRAMA);
*/


if($_SERVER['HTTP_HOST'] == 'simec.mec.gov.br'){

	//PERFIL FANTASMA SIMEC
	define("CPF_PROFESSOR",'');
	define("CPF_DIRETOR_FEDERAL",'');
	define("CPF_DIRETOR_MUNICIPAL",'00000000005');
	define("CPF_DIRETOR_ESTADUAL",'00000000004');
	define("CPF_SECRETARIO",'00000000003');
	
	//URL simec
	define('URL_SISTEMA','simec.mec.gov.br');
    
//	define('URL_SISTEMA','simec-d.mec.gov.br');
	//define('URL_SISTEMA','simec-local');
	
	//Dados WS
	define("WS_USUARIO",'c60e624c3b7ae3fe6c2987ae3bf1f017');
	define("WS_SENHA",'d3ad683319bdbba570f1205cf9a2c965');
	define("WS_CLIENTE",SIGLA_SISTEMA);
	define("WS_WSDL",'http://sisfies.mec.gov.br/service/abatimento?wsdl');
// 	define("WS_WSDL_FIES",'http://freire.mec.gov.br/services');
	define("WS_WSDL_FIES",'http://freire.capes.gov.br/services');
	define("PARAM_DBLINK_FREIRE",'	dbname=
									hostaddr=
									user=
									password=
									port=');
	define("LINK_FREIRE",'http://freire.mec.gov.br/index/principal');
	
}else{
	
	//PERFIL FANTASMA SIMEC
	define("CPF_PROFESSOR",'');
	define("CPF_DIRETOR_FEDERAL",'');
	define("CPF_DIRETOR_MUNICIPAL",'00000000005');
	define("CPF_DIRETOR_ESTADUAL",'00000000004');
	define("CPF_SECRETARIO",'00000000003');
	
	//URL simec
	define('URL_SISTEMA','simec-d.mec.gov.br');
	//define('URL_SISTEMA','simec-local');
	
	//Dados WS
// 	define("WS_USUARIO",'c60e624c3b7ae3fe6c2987ae3bf1f017');
// 	define("WS_SENHA",'d3ad683319bdbba570f1205cf9a2c965');
// 	define("WS_CLIENTE",SIGLA_SISTEMA);
// 	define("WS_WSDL",'http://sisfies.mec.gov.br/service/abatimento?wsdl');
	
	
	
	define("WS_USUARIO",'c60e624c3b7ae3fe6c2987ae3bf1f017');
	define("WS_SENHA",'d3ad683319bdbba570f1205cf9a2c965');
	define("WS_CLIENTE",SIGLA_SISTEMA);
	define("WS_WSDL",'http://sisfieshmg.mec.gov.br/service/abatimento?wsdl');
// 	define("WS_WSDL_FIES",'http://freire.mec.gov.br/services');
	define("WS_WSDL_FIES",'http://freire.capes.gov.br/services');
	define("PARAM_DBLINK_FREIRE",'	dbname=
									hostaddr=
									user=
									password=
									port=');
	define("LINK_FREIRE",'http://sigforhmg.mec.gov.br/ssd/servlet/');
	
}

define('MUNCOD_BRASILIA','5300108');

$cpf_liberados = array('','70481814191','72129794149','');

?>
