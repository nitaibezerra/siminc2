<?php

/*** SISID do módulo ***/
define("SISID_MAIS_CULTURA", 143);

/************************/
/*** Perfis do módulo ***/
/************************/
define( "PERFIL_MINC_ADMINISTRADOR",	779);
define( "PERFIL_MINC_MEC",				783);
define( "PERFIL_MINC_SEC_ESTADUAL",		780);
define( "PERFIL_MINC_SEC_MUNICIPAL",	781);
define( "PERFIL_MINC_CADASTRADOR",		782);
define( "PERFIL_MINC_SUPER_USUARIO",	778);

/*** SUPER USUÁRIO ***/
define( "PERFIL_SUPER_USUARIO",	778);

/*** Funções(funid) MAIS EDUCAÇÃO ***/
define( "FUN_DIRETOR_MC", 	   19 );
define( "FUN_COORDENADOR_MC",  109 );
define( "FUN_PARCEIRO_MC_PF",  110 );
define( "FUN_PARCEIRO_MC_PJ",  111 );

define( "MNUID_AVALIACAO",13559);
define( "MNUID_MONITORAMENTO",15550);


// DESENVOLVIMENTO
if($_SERVER['SERVER_NAME'] == "simec-local" || $_SERVER['SERVER_NAME'] == "simec-d.mec.gov.br"){
	/*** tpdid dos submódulos do módulo "Escola" ***/
	switch($_SESSION['sisbaselogin']){
	    case "simec_desenvolvimento_old":
                        define( "MNUID_AVALIACAO",13559);
                        define( "MNUID_MONITORAMENTO",15550);
                        define( "MNUID_MONITORAMENTO_2",16227);
			define( "TPDID_MAIS_CULTURA",75);
	        break;
	    case "simec_desenvolvimento":
                
                    define( "MNUID_AVALIACAO",13562);
                    define( "MNUID_MONITORAMENTO",15550);
                    define( "MNUID_MONITORAMENTO_2",16227);
                    // Workflow estado de documento
                    define( "ESTADO_DOCUMENTO_CADASTRAMENTO",545);
                    define( "ESTADO_DOCUMENTO_SECRETARIA_MUN_EST",546);
                    define( "ESTADO_DOCUMENTO_AVALIACAO",547);
                    define( "ESTADO_DOCUMENTO_FINALIZADO",548);
                    define( "ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR",1103);
                    define( "ESTADO_DOCUMENTO_AVALIADO",1104);
                    define( "ESTADO_DOCUMENTO_ENVIADO_FNDE",1105);
                    
                    define( "TPDID_MAIS_CULTURA",81);
	        break;
	    case "simec_espelho_producao":
                
                    define( "MNUID_AVALIACAO",13559);
                    define( "MNUID_MONITORAMENTO",15550);
                    define( "MNUID_MONITORAMENTO2",16227);
                    // Workflow estado de documento
                    define( "ESTADO_DOCUMENTO_CADASTRAMENTO",545);
                    define( "ESTADO_DOCUMENTO_SECRETARIA_MUN_EST",546);
                    define( "ESTADO_DOCUMENTO_AVALIACAO",547);
                    define( "ESTADO_DOCUMENTO_FINALIZADO",548);
                    define( "ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR",1103);
                    define( "ESTADO_DOCUMENTO_AVALIADO",1104);
                    define( "ESTADO_DOCUMENTO_ENVIADO_FNDE",1105);
                    
                    define( "TPDID_MAIS_CULTURA",80);
	        break;
	    default:
                
                define( "MNUID_AVALIACAO",13559);
                define( "MNUID_MONITORAMENTO",15550);
                define( "MNUID_MONITORAMENTO2",16227);

            // Workflow estado de documento
            define( "ESTADO_DOCUMENTO_CADASTRAMENTO",545);
            define( "ESTADO_DOCUMENTO_SECRETARIA_MUN_EST",546);
            define( "ESTADO_DOCUMENTO_AVALIACAO",547);
			define( "ESTADO_DOCUMENTO_FINALIZADO",548);
			define( "ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR",1103);
			define( "ESTADO_DOCUMENTO_AVALIADO",1104);
			define( "ESTADO_DOCUMENTO_ENVIADO_FNDE",1105);
	    	
	    	define( "TPDID_MAIS_CULTURA",80);
	    	break;
	}
} else {
	/*** tpdid dos submódulos do módulo "Escola" ***/
	switch($_SESSION['sisbaselogin']){
	    case "simec_desenvolvimento_old":
                         define( "MNUID_AVALIACAO",13559);
                         define( "MNUID_MONITORAMENTO",15550);
                         define( "MNUID_MONITORAMENTO_2",16227);
			define( "TPDID_MAIS_CULTURA",75);
	        break;
	    case "simec_desenvolvimento":
                    
                    define( "MNUID_AVALIACAO",13559);
                    define( "MNUID_MONITORAMENTO",15550);
                    define( "MNUID_MONITORAMENTO_2",16227);
                
                    // Workflow estado de documento
                    define( "ESTADO_DOCUMENTO_CADASTRAMENTO",545);
                    define( "ESTADO_DOCUMENTO_SECRETARIA_MUN_EST",546);
                    define( "ESTADO_DOCUMENTO_AVALIACAO",547);
                    define( "ESTADO_DOCUMENTO_FINALIZADO",548);
                    define( "ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR",1103);
                    define( "ESTADO_DOCUMENTO_AVALIADO",1104);
                    define( "ESTADO_DOCUMENTO_ENVIADO_FNDE",1105);
                    
                    define( "TPDID_MAIS_CULTURA",81);
                
	        break;
            
            // ESPELHO
	    case "simec_espelho_producao":
                    define( "MNUID_AVALIACAO",13559);
                    define( "MNUID_MONITORAMENTO",15550);
                    define( "MNUID_MONITORAMENTO_2",16227);
                
                    // Workflow estado de documento
                    define( "ESTADO_DOCUMENTO_CADASTRAMENTO",545);
                    define( "ESTADO_DOCUMENTO_SECRETARIA_MUN_EST",546);
                    define( "ESTADO_DOCUMENTO_AVALIACAO",547);
                    define( "ESTADO_DOCUMENTO_FINALIZADO",548);
                    define( "ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR",1103);
                    define( "ESTADO_DOCUMENTO_AVALIADO",1104);
                    define( "ESTADO_DOCUMENTO_ENVIADO_FNDE",1105);
					
                    define( "TPDID_MAIS_CULTURA",80);
                break;
            
            // PRODUÇÃO
	    default:
                    define( "MNUID_AVALIACAO",13559);
                    define( "MNUID_MONITORAMENTO",15550);
                    define( "MNUID_MONITORAMENTO_2",16227);
                    // Workflow estado de documento
                    define( "ESTADO_DOCUMENTO_CADASTRAMENTO",545);
                    define( "ESTADO_DOCUMENTO_SECRETARIA_MUN_EST",546);
                    define( "ESTADO_DOCUMENTO_AVALIACAO",547);
                    define( "ESTADO_DOCUMENTO_FINALIZADO",548);
                    define( "ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR",1103);
                    define( "ESTADO_DOCUMENTO_AVALIADO",1104);
                    define( "ESTADO_DOCUMENTO_ENVIADO_FNDE",1105);

                    define( "TPDID_MAIS_CULTURA",80);
                break;
	}
}  

?>