<?php 
/********
 * SISTEMA
 ********/

define("ID_NOTA_TECNICA", 1);
define("ID_PROJETO_BASICO", 2);
define("ID_OUTROS", 3);
define("ID_TERMO_REFERENCIA", 4);
define("ID_PROPOSTA_SERVICO", 5);
define("ID_PORTARIA_FISCAL", 6);
define("ID_EDITAL", 7);
define("ID_CONTRATO", 8);
define("ID_ATA_REGISTRO_PRECO", 9);
define("ID_PARECER_CONJUR", 10);
define("ID_QUESTIONAMENTOS", 11);
define("ID_RESP_QUESTIONAMENTOS", 12);
define("ID_RELATORIO_ADJUDICACAO", 13);
define("ID_RELATORIO_HOMOLOGACAO", 14);
define("ID_TERMO_ADITIVO", 15);
define("ID_GARANTIA_CONTRATO", 23);

if($_SESSION['ambiente'] == "stg-ebserh" || $_SESSION['ambiente'] == "prod-ebserh"){
	define("EVT_SISID", 195);
	
	define("PERFIL_SUPER_USUARIO",  			 getIdConstanteVirtual('seguranca_perfil_pflcod_superusuario1151'));
	define("PERFIL_EQUIPE_TECNICA_UNIDADE", 	 getIdConstanteVirtual('seguranca_perfil_pflcod_equipetecnicadaunidade1154'));
	define("PERFIL_GESTOR_FINANCEIRO_UNIDADE", 	 getIdConstanteVirtual('seguranca_perfil_pflcod_gestorfinanceirodaunidade1152')); // de PERFIL_COORDENADOR_DESPESA_UNIDADE para PERFIL_GESTOR_FINANCEIRO_UNIDADE
	define("PERFIL_ADMINISTRADOR",  			 getIdConstanteVirtual('seguranca_perfil_pflcod_administrador1155'));
	define("PERFIL_GESTOR_UNIDADE", 			 getIdConstanteVirtual('seguranca_perfil_pflcod_gestordaunidade1156'));
	define("PERFIL_CONSULTA_UNIDADE", 			 getIdConstanteVirtual('seguranca_perfil_pflcod_consultadaunidade1150'));
	define("PERFIL_CONSULTA_GERAL", 			 getIdConstanteVirtual('seguranca_perfil_pflcod_consultageral1153'));
	define("PERFIL_FISCAL_CONTRATO", 			 getIdConstanteVirtual('seguranca_perfil_pflcod_fiscaldocontrato1157'));
	define("PERFIL_TRIAGEM", 					 getIdConstanteVirtual('seguranca_perfil_pflcod_Triagem'));
	define("PFLCOD_ADMINISTRADOR_UNIDADE",		 getIdConstanteVirtual('seguranca_perfil_pflcod_adm_unidade_contratos'));
	
	/* Faturamento de Contratos EBSERH*/
	define("TPDID_FATURAMENTO",							141);
	define("ESTADO_WK_FATURAMENTO_EM_CADASTRAMENTO",	988);
	define("ESTADO_WK_FATURAMENTO_AGUARDANDO_APROVACAO",987);
	define("ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO",989);
	define("ESTADO_WK_FATURAMENTO_PAGO",				990);
	
	define("CONTRATANTE_EBSERH", 57);
	
}else {
	define("EVT_SISID", 21);
	
	define("PERFIL_SUPER_USUARIO",  			 255);
	define("PERFIL_EQUIPE_TECNICA_UNIDADE", 	 263);
	define("PERFIL_GESTOR_FINANCEIRO_UNIDADE", 	 354); // de PERFIL_COORDENADOR_DESPESA_UNIDADE para PERFIL_GESTOR_FINANCEIRO_UNIDADE
	define("PERFIL_ADMINISTRADOR",  			 541);
	define("PERFIL_GESTOR_UNIDADE", 			 1118);
	define("PERFIL_CONSULTA_UNIDADE", 			 262);
	define("PERFIL_CONSULTA_GERAL", 			 537);
	define("PERFIL_FISCAL_CONTRATO", 			 1143);
	
	/* Faturamento de Contratos EBSERH*/
	define("TPDID_FATURAMENTO",							127);
	define("ESTADO_WK_FATURAMENTO_EM_CADASTRAMENTO",	817);
	define("ESTADO_WK_FATURAMENTO_AGUARDANDO_APROVACAO",818);
	define("ESTADO_WK_FATURAMENTO_AGUARDANDO_PAGAMENTO",819);
	define("ESTADO_WK_FATURAMENTO_PAGO",				820);
	
	define("CONTRATANTE_EBSERH", 57);
	
}


?>