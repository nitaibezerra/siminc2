<?php
if($_SESSION['baselogin']=='simec_desenvolvimento'){

    //Perfil
    define("PFL_SUPER_USUARIO",	 	0);

    //Sistema
    define("SIS_DEMANDASSE", 		   	195);

    //Tipo de Documento WORKFLOW
    define("WF_TPDID_DEMANDASSE_DEMANDA", 157);

    // Estados do Documento WORKFLOW
    define("ESD_DEMANDA_EM_CADASTRAMENTO", 1204);
    define("ESDID_ARQUIVADO", 1317);

    define("K_TIPO_DOCUMENTO_OFICIO"   , 1);
    define("K_TIPO_DOCUMENTO_MEMO"     , 2);
    define("K_TIPO_DOCUMENTO_PORTARIA" , 3);
    define("K_TIPO_DOCUMENTO_DESPACHO" , 4);

} else {

    //Perfil
    define("PFL_SUPER_USUARIO",	 	1312);

    //Sistema
    define("SIS_DEMANDASSE", 		   	200);

    //Tipo de Documento WORKFLOW
    define("WF_TPDID_DEMANDASSE_DEMANDA", 207);

    // Estados do Documento WORKFLOW
    define("ESD_DEMANDA_EM_CADASTRAMENTO", 1314);
    define("ESD_DEMANDA_EM_ATENDIMENTO", 1315);
    define("ESD_DEMANDA_EM_DILIGENCIA", 1316);
    define("ESDID_ARQUIVADO", 1317);
    
    define("K_TIPO_DOCUMENTO_OFICIO"   , 1);
    define("K_TIPO_DOCUMENTO_MEMO"     , 2);
    define("K_TIPO_DOCUMENTO_PORTARIA" , 3);
    define("K_TIPO_DOCUMENTO_DESPACHO" , 4);

}
?>