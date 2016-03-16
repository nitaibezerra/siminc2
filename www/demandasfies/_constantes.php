<?php
if($_SESSION['baselogin']=='simec_desenvolvimento'){

    //Perfil
    define("PFL_SUPER_USUARIO",	 	1281);
    define("PFL_CONSULTA",	 	    1286);
    define("PFL_ADMINISTRADOR",	 	1287);
    define("PFL_PROCURADOR_FEDERAL",1288);
    define("PFL_NUCLEO_JURIDICO",	1289);
    define("PFL_ADVOGADO",	 	    1290);
    define("PFL_DTI_MEC",	 	    1291);
    define("PFL_ANALISTA_DTI_MEC",	1294);
    define("PFL_4_NIVEL",	 	    1293);
    define("PFL_GESTOR_FIES",	    1292);
    define("PFL_DISTRIBUIDOR",	    1346);
	define("PFL_GERENCIA_DIGEF",	    1339);
//	define("PFL_CGSUP",	    1339);

	//Acao
    define("AC_ENVIAR_DTI_MEC",	    2976);
    define("AC_PROFE_FINALIZAR",	3091);
    define("AC_EXECUCAO_DTI_MEC",	3433);
	define("AC_ENVIA_SUBSIDIO_PROFE",	3173);

    //Sistema
    define("SIS_DEMANDASFIES", 		   	195);

    //Tipo de Documento WORKFLOW
    define("WF_TPDID_DEMANDASFIES_DEMANDA", 156);
    define("WF_TPDID_DEMANDASFIES_ENTREGA", 204);

    // Estados do Documento WORKFLOW
    define("ESD_DEMANDA_EM_CADASTRAMENTO", 1271);
    define("ESD_DEMANDA_EM_INTERVENCAO", 1277);

	define("ESD_GERENCIA_DIGEF",	    1374);
//	define("ESD_DEMANDA_CGSUP",	    1280);
    define("ESD_DEMANDA_NUCLEO_JURIDICO", 1270);
    define("ESD_DEMANDA_ADVOGADO", 1272);
    define("ESD_DEMANDA_DTI_MEC", 1282);
    define("ESD_DEMANDA_EXECUCAO_DTI_MEC", 1283);
    define("ESD_DEMANDA_4_NIVEL", 1274);
    define("ESD_DEMANDA_ANALISTA_DTI_MEC", 1273);
    define("ESD_DEMANDA_GESTOR_FIES", 1284);
    define("ESD_DEMANDA_FINALIZADA", 1275);
    define("ESD_DEMANDA_PROFE", 1296);

    define("ESD_ENTREGA_EM_ELABORACAO", 1297);
    define("ESD_ENTREGA_AGUARDANDO_APROVACAO", 1298);
    define("ESD_ENTREGA_AGUARDANDO_APROVACAO_DIGEF", 1411);
	define("ESD_ENTREGA_APROVADA", 1299);

} else {

	//Acao
	define("AC_ENVIAR_DTI_MEC",	    2976);
	define("AC_PROFE_FINALIZAR",	3091);
	define("AC_EXECUCAO_DTI_MEC",	3433);
	define("AC_ENVIA_SUBSIDIO_PROFE", 3173);

    //Perfil
    define("PFL_SUPER_USUARIO",	 	1281);
    define("PFL_CONSULTA",	 	    1286);
    define("PFL_ADMINISTRADOR",	 	1287);
    define("PFL_PROCURADOR_FEDERAL",1288);
    define("PFL_NUCLEO_JURIDICO",	1289);
    define("PFL_ADVOGADO",	 	    1290);
    define("PFL_DTI_MEC",	 	    1291);
    define("PFL_ANALISTA_DTI_MEC",	1294);
    define("PFL_4_NIVEL",	 	    1293);
    define("PFL_GESTOR_FIES",	    1292);
	define("PFL_DISTRIBUIDOR",	    1346);
	define("PFL_GERENCIA_DIGEF",	1339);

	define("AED_GERENCIA_DIGEF",	    1374);
	define("AED_ADVOGADO_DTI_MEC",	    2921);
	define("AED_DTI_MEC_ANALISTA_DTI_MEC",	    2969);
    //Sistema
    define("SIS_DEMANDASFIES", 		   	198);

    //Tipo de Documento WORKFLOW
    define("WF_TPDID_DEMANDASFIES_DEMANDA", 201);
    define("WF_TPDID_DEMANDASFIES_ENTREGA", 204);

    // Estados do Documento WORKFLOW
    define("ESD_DEMANDA_EM_CADASTRAMENTO", 1271);
    define("ESD_DEMANDA_EM_INTERVENCAO", 1277);
    define("ESD_DEMANDA_NUCLEO_JURIDICO", 1270);
    define("ESD_DEMANDA_ADVOGADO", 1272);
    define("ESD_DEMANDA_DTI_MEC", 1282);
    define("ESD_DEMANDA_EXECUCAO_DTI_MEC", 1283);
    define("ESD_DEMANDA_4_NIVEL", 1274);
    define("ESD_DEMANDA_ANALISTA_DTI_MEC", 1273);
    define("ESD_DEMANDA_GESTOR_FIES", 1284);
    define("ESD_DEMANDA_FINALIZADA", 1275);
    define("ESD_DEMANDA_PROFE", 1296);
	define("ESD_GERENCIA_DIGEF",	 1374);
	define("ESD_DEMANDA_CGSUP",	    1280);

	define("ESD_ENTREGA_EM_ELABORACAO", 1297);
	define("ESD_ENTREGA_AGUARDANDO_APROVACAO", 1298);
	define("ESD_ENTREGA_AGUARDANDO_APROVACAO_DIGEF", 1411);
	define("ESD_ENTREGA_APROVADA", 1299);
}
?>