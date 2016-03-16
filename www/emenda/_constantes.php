<?php
// Diretorio do EMENDA
define('APPEMENDA', APPRAIZ . 'emenda/');

// Funções do Emendas (funid)
define( "FUN_DIRIGENTE_REPRESENTANTE", 	52 );
define( "ADMINISTRADOR_INST", 			273 );
define( "INSTITUICAO_BENEFICIADA", 		274 );
define( "AUTOR_EMENDA", 				295 );
define( "TEMPORARIO_ASSINATURA",		479 );

//Definição do Tipo de Beneficiário Escolas
define( "BENEFICIARIO_ESCOLAS", 20 );
//Definição do Tipo de Beneficiário Alunos
define( "BENEFICIARIO_ALUNOS", 	18 );


#Tipos de Reformulação

define('ALTERACAO_DAS_CLAUSULAS_DO_CONVENIO', 1);
define('ALTERACAO_DO_VALOR', 2);
define('SUPLEMENTACAO_DOS_RECURSOS', 5);
define('PRORROGACAO_DA_VIGENCIA', 3);
define('REFORMULACAO', 4);
define('PRORROGRACAO_DE_OFICIO', 8);
define('APOSTILAMENTO', 9);
define('RENDIMENTO_DE_APLICACAO', 10);
define('ALTERACAO_DE_ITENS_DA_ESPECIFICACAO', 11);


/*if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define( "ANALISTA_MERITO",				310 );
	define( "ANALISTA_FNDE",				323 );
	define( "ANALISTA_TECNICO",				324 );
	define( "ASSESSORIA_PARLAMENTAR", 		308 );
	define( "CONSULTA_GERAL",				307 );
	define( "CONVENIO",				 		390 );
	define( "EMPENHO",				 		382 );
	define( "SUPER_USUARIO", 				271 );
	define( "GESTOR_EMENDAS", 				413 );
	define( "PAGAMENTO", 					421 );
	define( "LIBERAR_SENHA",				902 );
} else {*/
	define( "ANALISTA_MERITO",				301 );
	define( "ANALISTA_FNDE",				392 );
	define( "ANALISTA_TECNICO",				393 );
	define( "ASSESSORIA_PARLAMENTAR", 		299 );
	define( "CONSULTA_GERAL",				298 );
	define( "CONVENIO",				 		396 );
	define( "EMPENHO",				 		399 );
	define( "SUPER_USUARIO", 				389 );
	define( "SUPER_ADMINISTRADOR", 			550 );
	define( "GESTOR_EMENDAS", 				406 );
	define( "PAGAMENTO", 					407 );
	define( "LIBERAR_SENHA",				415 );
	define( "ADMINISTRADOR_MINUTA",			486 );
	define( "ADMINISTRADOR_REFORMULACAO",	475 );
//}

// TIPO DOCUMENTO DO SISTEMA (WORKFLOW)
define( "TPDID_EMENDAS", 8 );

// TIPO DOCUMENTO DO SISTEMA (WORKFLOW DESCENTRALIZACAO)
define( "TPDID_DESCENTRALIZA", 19 );

// ESTADOS DO WORFLOW (EM ORDEM)
/*if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define( "EM_ELABORACAO", 					134 );
	define( "EM_ANALISE_DE_DADOS", 				135 );
	define( "EM_ANALISE_DE_MERITO", 			136 );
	define( "EM_ANALISE_TECNICA", 				137 );
	define( "EM_EMPENHO", 						186 );
	define( "EM_ANALISE_DO_FNDE", 				139 );
	define( "EM_GERACAO_DA_MINUTA_DE_CONVENIO", 140 );
	define( "EM_ANALISE_PROFE",					141 );
	define( "EM_CRIACAO_DO_COVENIO", 			142 );
	define( "ASSINATURAS", 						156 );
	define( "EM_PUBLICACAO_NO_DOU", 			157 );
	define( "EM_PRE_CONVENIO", 					158 );
	define( "EM_ANALISE_JURIDICA",				177 );
	define( "EM_PAGAMENTO", 					159 );
	define( "RECURSO_LIBERADO",					178 );
	define( "EM_SOLICITACAO_REFORMULACAO",		179 );
	define( "EM_REFORMULACAO",					181 );
	define( "EM_ANALISE_TECNICA_REFORMULACAO",	182 );
	define( "EM_EMPENHO_REFORMULACAO",			183 );
	define( "EM_GERACAO_TERMO_ADITIVO",			184 );
	define( "EM_REPUBLICACAO",					185 );
	
} else {*/
	define( "EM_ELABORACAO", 					52 );
	define( "EM_ANALISE_DE_DADOS", 				53 );
	define( "EM_ANALISE_DE_MERITO", 			54 );
	define( "EM_ANALISE_TECNICA", 				56 );
	define( "EM_EMPENHO", 						57 );
	define( "EM_ANALISE_DO_FNDE", 				55 );
	define( "EM_GERACAO_DA_MINUTA_DE_CONVENIO", 58 );
	define( "EM_ANALISE_PROFE",					59 );
	define( "EM_CRIACAO_DO_COVENIO", 			60 );
	define( "EM_ASSINATURA_CONVENENTE", 		60 );
	define( "EM_ASSINATURA_CONCEDENTE", 		67 );
	define( "EM_PUBLICACAO_NO_DOU", 			68 );
	define( "EM_PRE_CONVENIO", 					69 );
	define( "EM_LIBERACAO_RECURSO",				70 );
	define( "EM_ANALISE_JURIDICA",				119 );
	define( "RECURSO_LIBERADO",					120 );
	define( "EM_SOLICITACAO_REFORMULACAO",		121 );
	define( "EM_REFORMULACAO_PROCESSO",			123 );
	define( "EM_ANALISE_TECNICA_REFORMULACAO",	124 );
	define( "EM_EMPENHO_REFORMULACAO",			125 );
	define( "EM_GERACAO_TERMO_ADITIVO",			137 );
	define( "EM_REPUBLICACAO",					138 );
	define( "EM_ANALISE_PROCESSO_REF",			155 );
	define( "EM_DILIGENCIA",					167 );
	define( "EM_PTA_INDEFERIDO",				197 );
	define( "EM_IDENTIFICACAO_PROCESSO_REFORMULACAO",	198 );
	define( "EM_TERMO_ADITIVO_PUBLICADO",		200 );
	define( "EM_PUBLICACAO_REFORMULACAO",		201 );
	define( "EM_LIBERACAO_RECURSO_REFORMULACAO",202 );
	define( "EM_ASSINATURA_REFORMULACAO",		204 );
	define( "EM_ACEITACAO_REFORMULACAO",		206 );
	define( "EM_PROCESSO_REFORMULADO",			207 );
	define( "EM_ANALISE_JURIDICA_REFORMULACAO",	208 );
	define( "EM_CORREÇÃO_DA_REFORMULAÇÃO_DO_PROCESSO",	219 );
	define( "CONVENIO_CANCELADO",				245 );
	define( "CONVENIO_EM_VIGENCIA_EXPIRADA",	344 );
	define( "EM_PRESTACAO_CONTAS",				199 );
	define( "EM_ORCAMENTO_IMPOSITIVO",			955 );//1025
	define( "EM_PUBLICADO",						1434 );
//}

#TIPOS DE REFORMULAÇÃO PLANO DE TRABALHO
define( "ALTERACAO_DE_CLAUSULA_DO_CONVENIO", 1 );
define( "ALTERACAO_DE_VALOR", 2 );
define( "PRORROGACAO_DA_VIGENCIA", 3 );
define( "REFORMULACAO", 4 );
define( "SUPLEMENTACAO_DOS_RECURSOS", 5 );

/**
 * TIPO DOCUMENTO DO SISTEMA (WORKFLOW EMENDA IMPOSITIVO)
 */ 
define( "TPDID_EMENDA_IMPOSITIVO", 163 );

define( "EM_ELABORACAO_IMPOSITIVO", 				1026 );
define( "EM_IDENTIFICACAO_PROCESSO_IMPOSITIVO", 	1027 );
define( "VINCULACAO_UNIDADES_GESTORAS_IMPOSITIVO", 	1028 );
define( "EM_ANALISE_DE_MERITO_IMPOSITIVO", 			1029 );
define( "EM_ANALISE_TECNICA_IMPOSITIVO", 			1030 );
define( "EM_DILIGENCIA_IMPOSITIVO", 				1031 );
define( "EM_EMPENHO_IMPOSITIVO", 					1033 );
define( "EM_PROPOSTA_SICONV", 						1032 );

/*
 * FIM WORKFLOW EMENDA IMPOSITIVO
 */

define( "DATA_INDICACAO_EMENDA", 	$_SESSION['exercicio'].'-02-18' );
define( "DATA_ELABORACAO_PTA", 		$_SESSION['exercicio'].'-03-31' );
define( "DATA_ANALISE_PTA", 		$_SESSION['exercicio'].'-04-19' );
define( "DATA_DILIGENCIA_PTA", 		$_SESSION['exercicio'].'-04-18' );

/*define( "DATA_INDICACAO_EMENDA", 	$_SESSION['exercicio'].'-02-02' );
define( "DATA_ELABORACAO_PTA", 		$_SESSION['exercicio'].'-02-02' );
define( "DATA_ANALISE_PTA", 		$_SESSION['exercicio'].'-02-02' );
define( "DATA_DILIGENCIA_PTA", 		$_SESSION['exercicio'].'-02-02' );*/
?>