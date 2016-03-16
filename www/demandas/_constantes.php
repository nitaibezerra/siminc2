<?php 
/************************
 * PERFIS
*************************/
define('DEMANDA_PERFIL_SUPERUSUARIO', 	  214);
define('DEMANDA_PERFIL_ANALISTA_TECNICO', 232);
define('DEMANDA_PERFIL_GESTOR_MEC', 	  232);
define('DEMANDA_PERFIL_TECNICO', 	  	  233);
define('DEMANDA_PERFIL_ANALISTA_WEB', 	  234);
define('DEMANDA_PERFIL_DEMANDANTE',   	  235);
define('DEMANDA_PERFIL_GERENTE_PROJETO',  236);
define('DEMANDA_PERFIL_ANALISTA_SISTEMA', 237);
define('DEMANDA_PERFIL_PROGRAMADOR',   	  238);
define('DEMANDA_PERFIL_ADMINISTRADOR',    239);
define('DEMANDA_PERFIL_CONSULTA_GERAL',   240);
define('DEMANDA_PERFIL_CONSULTA_AREA',    241);
define('DEMANDA_PERFIL_TECNICO1',		  248);
define('DEMANDA_PERFIL_DBA',			  257);
define('DEMANDA_PERFIL_ANALISTA_TESTE',	  266);
define('DEMANDA_PERFIL_DEPOSITO_DTI',	  511);
define('DEMANDA_PERFIL_ANALISTA_FNDE',	  554);
define('DEMANDA_PERFIL_ADM_REDES',		  285);
define('DEMANDA_PERFIL_GESTOR_REDES',	  286);
define('DEMANDA_PERFIL_EMPRESA_TYPE',	  676);
define('DEMANDA_PERFIL_DEMANDANTE_AVANCADO',	  681);
define('DEMANDA_PERFIL_FISCAL_TECNICO_FSW', 	  995);
define('DEMANDA_PERFIL_GERENTE_FSW', 			  996);
define('DEMANDA_PERFIL_ANALISTA_FSW', 			  1024);
define('DEMANDA_PERFIL_EQUIPE',	  		  1191);
define('DEMANDA_PERFIL_GESTOR_EQUIPE',	  1192);

/************************
 * CLASSIFICAÇÃO DA DEMANDA
 ************************/
define('CLASSIFICACAO_RESOLUCAO_PROBLEMA', 'P');
define('CLASSIFICACAO_RESOLUCAO_MUDANCA',  'M');


/************************
 * CLASSIFICAÇÃO DA DEMANDA DE SISTEMA DE INFORMAÇÃO
 ************************/
define('CLASSIFICACAO_SIS_INICIAL', 	  1);
define('CLASSIFICACAO_SIS_CONSULTIVA', 	  2);
define('CLASSIFICACAO_SIS_INVESTIGATIVA', 3);
define('CLASSIFICACAO_SIS_CORRETIVA', 	  4);
define('CLASSIFICACAO_SIS_EVOLUTIVA', 	  5);

/************************
 * ID DEMANDA DE SISTEMA "origem demanda"
 ************************/
define('ORIGEM_DEMANDA_SISTEMA_INFORMACAO', 1);
if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define('ORIGEM_DEMANDA_ESCRITORIO_PROCESSOS', 13);
} else {
	define('ORIGEM_DEMANDA_ESCRITORIO_PROCESSOS', 15);
}

/***********************
 * WORKFLOW(S)
 ***********************/
define('DEMANDA_WORKFLOW_GENERICO', 	  	  35);
define('DEMANDA_WORKFLOW_ATENDIMENTO', 	  	  31);

/************************
 * ESTADO(S) DO(S) DOCUMENTO(S)
 ************************/
define('DEMANDA_ESTADO_EM_ANALISE', 	  	  91);
define('DEMANDA_ESTADO_EM_ATENDIMENTO', 	  92);
define('DEMANDA_ESTADO_AGUARDANDO_VALIDACAO', 93);
define('DEMANDA_ESTADO_AGUARDANDO_AVALIACAO', 94);
define('DEMANDA_ESTADO_FINALIZADO',   	  	  95);
define("DEMANDA_ESTADO_CANCELADO",	 		 100);
define("DEMANDA_ESTADO_INVALIDADA",   		 135);
define("DEMANDA_ESTADO_VALIDADA_FORA_PRAZO", 170);
define('DEMANDA_ESTADO_AUDITADO',   		 890);

define("DEMANDA_GENERICO_ESTADO_EM_ANALISE",	       				107);
define("DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO",				    108);
define("DEMANDA_GENERICO_ESTADO_FINALIZADO",		   				109);
define("DEMANDA_GENERICO_ESTADO_CANCELADO",			   				110);
define("DEMANDA_GENERICO_ESTADO_AGUARDANDO_VALIDACAO", 				111);
define("DEMANDA_GENERICO_ESTADO_AGUARDANDO_VALIDACAO_DEMANDANTE", 	278);
define("DEMANDA_GENERICO_ESTADO_AGUARDANDO_AVALIACAO", 				112);
define("DEMANDA_GENERICO_ESTADO_INVALIDADA", 		   				136);
define('DEMANDA_GENERICO_ESTADO_AUDITADO',   	  	 				772);
define('DEMANDA_GENERICO_ESTADO_AGUARDANDO_PAGAMENTO', 			    1336);
define('DEMANDA_GENERICO_ESTADO_PAGO',   	  	 					1337);

/************************
 * AVALIAÇÃO DA DEMANDA
 ************************/
define('DEMANDA_AVALIACAO_RUIM', 	1);
define('DEMANDA_AVALIACAO_REGULAR', 2);
define('DEMANDA_AVALIACAO_BOM', 	3);
define('DEMANDA_AVALIACAO_OTIMO', 	4);

/**********************
 * REMETENTE WORKFLOW
 **********************/ 
 define('REMETENTE_WORKFLOW_EMAIL', 'demandas@mec.gov.br');
 define('REMETENTE_WORKFLOW_NOME',  'Módulo Demandas');
 
 
 /**********************
 * CADASTRO SISTEMAS
 **********************/ 
 define('DESTAQUE_BANNER_MAX', 5);
 define('DESTAQUE_ABA_MAX',  8);
 define('DESTAQUE_BANNER_MIN', 1);
 define('DESTAQUE_ABA_MIN',  3);
    
 
 /**********************
 * PAINEL DE GRÁFICOS
 **********************/ 
 $arr_cores_painel = array('#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#FFFAFA','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282');
 
 
 /**********************
 * CADASTRO ENTIDADES (FORNECEDORES)
 **********************/ 
 define('FUNID_FORNECEDOR', 80);
 
 
 
 if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define("DEMANDA_QUESTIONARIO", 	111);
} else {
	define("DEMANDA_QUESTIONARIO", 	63);
}
 
 
?>