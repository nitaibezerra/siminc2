<?php 
if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define("PERFIL_SUPER_USUARIO",946);
	define("PERFIL_REITOR",947);
	define("PERFIL_ADMINISTRADOR",1064);
	define("PERFIL_CONSULTA",1065);
	define("PFLCOD_CONSULTA_EXTERNO",1093);
	define("PERFIL_APOIADOR",1134);
	define("PERFIL_TUTOR",1135);
	define("PERFIL_SUPERVISOR",1343);
	
	define("TRGID_CABECALHO",1);
	define("TRGID_DETALHE",2);
	define("TRGID_TRAILER",3);
	
	define("TPLID_CREDITO",1);
	define("TPLID_CADASTRO",2);
	
	define("TPUID_UNIVERSIDADE",1);
	define("TPUID_INSTITUICAO",2);
	define("TPUID_ESCOLA",3);
	define("TPUID_PROGRAMA",4);
	define("TPUID_COMISSAO",5);
	
	define("STRID_REGISTRO_OK",1);
	
	define("BNCID_BANCO_BRASIL",1);
	
	define("TFPID_PAGAMENTO",1);
	
	// Categorias uploads e links
	define("CAT_UPLOADAS_IMAGENS",11);
	define("CAT_UPLOADAS_VIDEOS",12);
		
}else{
	define("PERFIL_SUPER_USUARIO",1040);
	define("PERFIL_REITOR",1041);
	define("PERFIL_ADMINISTRADOR",1064);
	define("PERFIL_CONSULTA",1065);
	define("PFLCOD_CONSULTA_EXTERNO",1093);
	define("PERFIL_APOIADOR",1219);
	define("PERFIL_TUTOR",1220);
	define("PERFIL_SUPERVISOR",1343);
	
	define("TRGID_CABECALHO",1);
	define("TRGID_DETALHE",2);
	define("TRGID_TRAILER",3);
	
	define("TPLID_CREDITO",1);
	define("TPLID_CADASTRO",2);
	
	define("TPUID_UNIVERSIDADE",1);
	define("TPUID_INSTITUICAO",2);
	define("TPUID_ESCOLA",3);
	define("TPUID_PROGRAMA",4);
	define("TPUID_COMISSAO",5);
	
	define("STRID_REGISTRO_OK",1);

	define("BNCID_BANCO_BRASIL",1);
	
	define("TFPID_PAGAMENTO",1);
	
	// Categorias uploads e links
	define("CAT_UPLOADAS_IMAGENS",11);
	define("CAT_UPLOADAS_VIDEOS",12);
}

// CONSTANTES DOS RELATORIOS DE SUPERVISAO
define("FORM_PRIMEIRAS_IMPRESSOES",		33);
define("FORM_PRIMEIRA_VISITA",			35);
define("FORM_SUPERVISAO_PRATICA",		36);
define("FORM_PRIMEIRAS_IMPRESSOES_DSEI",39);
define("FORM_PRIMEIRA_VISITA_DSEI",		40);
define("FORM_SUPERVISAO_PRATICA_DSEI",	43);

// Novos Perfis
define("PERFIL_APOIO_GESTAO",1295);
define("PERFIL_APOIO_DESENVOLVIMENTO",1296);
define("PERFIL_APOIO_DESCENTRALIZADO",1219);

// CAMEM
define("PERFIL_AVALIADOR_CAMEM",1345);
define("PERFIL_COORDENADOR_CAMEM",1344);
define("PERFIL_DIRETOR_CAMEM",1347);

define("FUNCAO_COORDENADOR_CAMEM",1);
define("FUNCAO_AVALIADOR_CAMEM",2);

/*
1;1344;"Preencher execuчуo do plano de trabalho do Coordenador";"";"A"
2;1345;"Preencher execuчуo do plano de trabalho do Avaliador";"";"A"
3;1344;"Preencher plano de trabalho do Coordenador";"";"A"
4;1345;"Preencher plano de trabalho do Avaliador";"";"A"
5;1344;"Validar plano(s) de trabalho do(s) Avaliador(es)";"";"A"
6;1347;"Validar plano(s) de trabalho do(s) Coordenador(es)";"";"A"
*/

define("PRAZO_CAD_PT_AVAL_CAMEM",4);
define("PRAZO_CAD_PT_COOR_CAMEM",3);
define("PRAZO_PTEXEC_AVAL_CAMEM",2);
define("PRAZO_PTEXEC_COOR_CAMEM",1);
define("PRAZO_VALIDACAO_DIRE_CAMEM",6);
define("PRAZO_VALIDACAO_COOR_CAMEM",5);
?>