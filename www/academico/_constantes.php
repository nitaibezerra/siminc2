<?PHP
// Constantes do Orgão a ser filtrado no sistema
define('ORGAO', CODIGO_ORGAO_SISTEMA);

// Constantes de perfis do módulo
define('PERFIL_IFESCADBOLSAS', 279); //277
define('PERFIL_IFESCADCURSOS', 278);
define('PERFIL_IFESCADASTRO', 375);

define('PERFIL_IFES_CADASTRO_PROGRAMA', 766);
	
define('PERFIL_IFESCONBOLSAS', 279);
define('PERFIL_IFESCONCURSOS', 280);
define('PERFIL_IFESCONSULTA', 374);
	
define('PERFIL_MECCADBOLSAS', 282);
define('PERFIL_MECCADCURSOS', 284);
define('PERFIL_MECCADASTRO', 378);
	
define('PERFIL_MECCONBOLSAS', 281);
define('PERFIL_MECCONCURSOS', 283);
define('PERFIL_MECCONSULTAGERAL', 377);
	
define('PERFIL_SUPERUSUARIO', 369);
define('PERFIL_ADMINISTRADOR', 373);

define('PERFIL_CONSULTA_GERAL',	607);
define('PERFIL_COORDENADOR_GERAL', 608);
define('PERFIL_DIRETORIA_SETEC', 606);
	
define('PERFIL_IFESAPROVACAO', 376); //Desativado
	
define('PERFIL_IFESCONTCU', 403);
define('PERFIL_IFESCADTCU', 404);
	
define('PERFIL_CADASTROGERAL', 487);

define('PERFIL_REITOR',	526);

define('PERFIL_ALTA_GESTAO', 528);
define('PERFIL_ASSESSORIA_ALTA_GESTAO',	529);

define('PERFIL_ADM_MULHERES_MIL', 690);

define('PERFIL_GESTAO_CONTRATOS', 769);

define('PERFIL_ASSISTENCIA_ESTUDANTIL',	805);

define('PERFIL_MAIS_MEDICOS_UNIDADE',939);

define('PERFIL_REDE_FEDERAL_MINISTRO', 1336);
define('PERFIL_REDE_FEDERAL_SECR_EXECUTIVO',1337);

define('PERFIL_RF_CONSUTORIA_JURIDICA', 1157);
define('PERFIL_RF_SECRETARIO_EXECUTIVO_DEC_GOV',1366);
define('PERFIL_RF_SESU_DEC_GOV',1367);
define('PERFIL_RF_SETEC_DEC_GOV',1368);
define('PERFIL_RF_MINISTRO_DEC_GOV',1369);

define('PERFIL_INTERLOCUTOR_INSTITUTO',1411);
define('PERFIL_DIRETOR_CAMPUS',1409);
define('PERFIL_INTERLOCUTOR_CAMPUS',1410);
define('PERFIL_PROREITOR',1413);

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
    define('PERFIL_ASSISTENCIA_ESTUDANTIL_IES',	833);
}else{
    define('PERFIL_ASSISTENCIA_ESTUDANTIL_IES',	850);
}

// tipo de perfis
define('ARRAY_PERFIL_IFES', serialize(array( (string) PERFIL_IFESAPROVACAO, (string) PERFIL_IFESCADASTRO, (string) PERFIL_IFESCONSULTA)) );
define('ARRAY_PERFIL_MEC',  serialize(array( (string) PERFIL_MECCADASTRO, (string) PERFIL_MECCONSULTAGERAL, (string) PERFIL_ADMINISTRADOR)) );

// Constantes das funções das entidades
define('ACA_ID_UNIVERSIDADE', 12);
define('ACA_ID_ESCOLAS_TECNICAS', 11);
define('ACA_ID_ESCOLAS_AGROTECNICAS', 14);
define('ACA_ID_UNED', 17);
define('ACA_ID_CAMPUS', 18);
define('ACA_ID_REITORIA', 75);
define('ACA_ID_UNIDADES_VINCULADAS', 102);


// Constantes dos orgãos
define('ACA_ORGAO_SUPERIOR', 1);
define('ACA_ORGAO_TECNICO', 2);

// Constantes de tipos de portaria
define('ACA_TPORTARIA_CONCURSO', 1);
define('ACA_TPORTARIA_PROVIMENTO', 2);


// Constantes de tipos de edital de portaria
define('ACA_TPEDITAL_CONCURSO',    1);
define('ACA_TPEDITAL_PUBLICACAO',  2);
define('ACA_TPEDITAL_HOMOLOGACAO', 3);
define('ACA_TPEDITAL_PROVIMENTO',  4);
define('ACA_TPEDITAL_NOMEACAO',    5);


// Constantes das classes
define('CLASSE_DOC',  1);
define('CLASSE_E',    2);
define('CLASSE_D',    3);
define('CLASSE_C',    4);
define('CLASSE_B',    5);
define('CLASSE_DOC2', 7);

// Encargo patronal
define('ENCARGO', 1.22);

#ID'S ABAS DOS SISTEMAS 
define('ABA_DADOS_ENTIDADES_CURSOS', 57135);
define('ABA_CURSO_POSGRADUACAO_DEV', 57560);
define('ABA_CURSO_POSGRADUACAO_OLD', 57393);
define('ABAS_DADOS_ADICIONAIS_DIRIGENTES', 57891);

#- ABA DA TABELA DE APOIO CADASTRO DE MÓDULOS E AMBIENTES
define('ABAS_TABELA_DE_APOIO_AMBIENTE_MODULO', 57895);

$_funcoesacademico = array('unidade' => array(21, 40),'campus' => array(24, 40));

define("FUNCAO_CAMPUS", 18);
define("FUNCAO_REITORIA", 75);
define("FUNCAO_UNIVERSIDADE", 12);

define("FUNCAO_ENTIDADE_PROPONENTE", 86);
define("FUNCAO_REP_ENTIDADE_PROPONENTE", 87);
define("FUNCAO_ENTIDADE_CONCEDENTE", 88);
define("FUNCAO_REP_ENTIDADE_CONCEDENTE", 89);


/********************************************
 * 
 * CONSTANTES MIGRADAS DO SIG
 * 
 ********************************************/

define("TIPOENSINO_DEFAULT", 1);
define("SISID", 56);

define("PERFIL_ATUALIZACAO", 202);
define("PERFIL_ATUALIZACAO_UNI", 228);

define("PERFIL_CONSULTA", 203);
define("PERFIL_ADMINISTRADOR", 201);

define("TIPOITEM_QTD", 3);


//Item Previstos
define("ITM_VAGAS_PREV_SUP",2);
define("ITM_DOCENTE_PREV_SUP",3);
define("ITM_TECNICO_PREV_SUP",4);
define("ITM_MAT_PREV_SUP",24);
define("ITM_CURSO_PREV_SUP",10);
define("ITM_INVESTIMENTO_PREV_SUP",5);
define("ITM_BOLSAS_MESTRADO_PREV_SUP",38);
define("ITM_BOLSAS_DOUTORADO_PREV_SUP",40);
define("ITM_BOLSAS_POSDOUTORADO_PREV_SUP",41);

define("ITM_MAT_OFERTATUAL_PROF", 34);
define("ITM_MAT_PREVISTA_PROF", 11);
define("ITM_INVS_PREVISTO_PROF", 14);
define("ITM_INVS_REALIZADO_PROF", 37);

//Item Realizado
define("ITM_VAGAS_REALIZ_SUP",28);
define("ITM_DOCENTE_REALIZ_SUP",25);
define("ITM_TECNICO_REALIZ_SUP",26);
define("ITM_MAT_REALIZ_SUP",27);
define("ITM_CURSO_REALIZ_SUP",29);
define("ITM_INVESTIMENTO_REALIZ_SUP",30);
define("ITM_BOLSAS_MESTRADO_REALIZ_SUP",42);
define("ITM_BOLSAS_DOUTORADO_REALIZ_SUP",43);
define("ITM_BOLSAS_POSDOUTORADO_REALIZ_SUP",44);
define("ITM_MATRICULAS_GRADUACAO_PRESENCIAL_SUP",45);

// tabela tipoensino
define("TPENSSUP", 1);
define("TPENSPROF", 2);


// lista do tipo de entidade universidades, ou centros profissionalizantes
$_tipoentidade = array(TPENSSUP => 12, TPENSPROF => array(11,14));


// lista de funcoes (cargos) por tipo de ensino "1"-> Superior, "2"->"Profissional"
$_funcoes = array( TPENSSUP => array('campus' => 24, 'unidade' => 21), TPENSPROF => array('campus' => 24, 'unidade' => 21) );

/*
 * ALTERAÇÃO SOLICITADA POR WESLEY LIRA (19/05/09)
 * EFETUADA POR ALEXANDRE DOURADO
 * MUDANÇA DO CARGO DE DIRIGENTE DA UNIDADE DE DIRETOR GERAL
 * PARA REITOR 
 * 
    $_funcoes = array( TPENSSUP => array('campus' => 24, 'unidade' => 21), TPENSPROF => array('campus' => 23, 'unidade' => 22) );
 */

// lista de funcoes (tipo de entidade, se é universidade ou centro tecnologico) por tipo de ensino "1"-> Superior, "2"->"Profissional"
$_funcoesentidade = array( TPENSSUP => array('campus' => 18, 'unidade' => 12), TPENSPROF => array('campus' => 17, 'unidade' => 11) );

define("INTERLOCUTORINS", 40);

// anos analisados por tipo de ensino
$anosanalisados[TPENSSUP] = array(2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015);
$anosanalisados[TPENSPROF] = array(2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015);
// anos analisados por default
$anosanalisados['default'] = array(2008,2009,2010,2011,2012,2013,2014,2015);

//Títulos dos itens
$tituloitens[TPENSSUP][0] = "REUNI";// tipo ensino: superior
$tituloitens[TPENSSUP][1] = "Total (REUNI/Expansão)";// total
$tituloitens[TPENSPROF][0] = "Expansão";// tipo ensino: profissional
$tituloitens[TPENSPROF][1] = "Total";// tipo ensino: profissional
$tituloitens['default'] = "Previsão";// tipo ensino: default

// Títulos dos cursos
$titulocursos[TPENSSUP] = "Detalhamento de vagas por curso";
$titulocursos[TPENSPROF] = "Detalhamento de matrículas por curso";
$titulocursos['default'] = "Vagas de curso";


/***********************
 * 
 * INICIO - Constantes de Cursos
 * 
 ***********************/
define("TIPOCURSOGRADUACAO", 	1);
define("TIPOCURSOPOSGRADUACAO", 7);
define("DADOSCURSO",			3);

/***********************
 * 
 * FIM - Constantes de Cursos
 * 
 ***********************/

/***********************
 * 
 * INICIO - WORKFLOW
 * 
 ***********************/
#ID DO FLUXO DO WORK FLOW REDE FEDERAL - AUTORIZAÇÃO DE BENS E SERVIÇOS.
define( "WF_FLUXO_AUTORIZACAO_BENS_SERVICOS", 71 );

define("WF_EM_ANALISE_PELA_CONJUR", 1065);
define("WF_EM_AJUSTE_PELO_DEMANDANTE", 1085);

define( "TPDID_EQUIVALENCIA", 17 );

define( "ESDID_CADASTRO", 160 );
define( "ESDID_ANALISE_MEC", 161 );
define( "ESDID_APROVADO", 163 );
define( "ESDID_EQUIVALENCIA_FINALIZADA", 162 );


define( "TC_TIPO_DOCUMENTO", 40 );

define("WF_EM_CORRECAO", 251);
define("WF_EM_ANALISE_ORCAMENTARIA", 245);
define("WF_EM_CANCELADO", 250);
define("WF_EM_FINALIZADO", 249);
define("WF_EM_ANALISE_PRESTACAO", 248);
define("WF_EM_PRESTACAO_CONTAS", 247);
define("WF_EM_MONITORAMENTO", 246);
define("WF_EM_AVALIACAO", 244);
define("WF_EM_ELABORACAO", 243);
define("WF_EM_ANALISE_DIRETORIA", 284);
define("WF_EM_ANALISE_COORDENACAO", 283);


define("WF_SOLICITACAO_VIAGEM", 40);
define("WF_SOLICITACAO_VIAGEM_EM_CADASTRAMENTO", 285);
define("WF_SOLICITACAO_VIAGEM_AGUARDANDO_AUTORIZACAO_SECRETARIA_EXECUTIVA", 287);
define("WF_SOLICITACAO_VIAGEM_AGUARDANDO_AUTORIZACAO_REITOR", 286);
define("WF_SOLICITACAO_VIAGEM_AUTORIZADO", 288);

#WORKFLOW - ESTADOS DA AUTORIZAÇÃO DE DECRETO
define("WF_BENS_SERVICOS_EM_CADASTRAMENTO", 491);
define("WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_REITOR", 492);
define("WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_MINISTRO", 494);
define("WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_SECRETARIO", 500);
define("WF_BENS_SERVICOS_AUTORIZADO_SECRETARIO", 495);
define("WF_BENS_SERVICOS_AUTORIZADO_MINISTRO", 496);
define("WF_BENS_SERVICOS_EM_ANALISE_SETEC", 1064);
define("WF_BENS_SERVICOS_EM_ANALISE_CONJUR", 1065);

define( "TPDID_CONSOLIDACAO_IFES", 74 );

define("WF_CONSOLIDACAO_IFES_EM_PREENCHIMENTO", 508);
define("WF_CONSOLIDACAO_IFES_EM_ANALISE_MEC", 509);
define("WF_CONSOLIDACAO_IFES_APROVADO_MEC", 510);

define("WF_AEDID_CONSOLIDACAO_IFES_ENVIAR_ANALISE", 1351);
define("WF_AEDID_CONSOLIDACAO_IFES_APROVAR", 1352);

#WORKFLOW - ACÕES PARA ENVIO PARA ANALISE DA CONJUR
define("WF_CADASTRAMENTO_ENVIAR_ANALISE_CONJUR", 2490);
define("WF_AGUARD_AUT_DIRIGEN_ENVIAR_ANALISE_CONJUR", 2738);
define("WF_AGUARD_AUT_SECRETA_ENVIAR_ANALISE_CONJUR", 2743);
define("WF_EM_ANALISE_SESU_ANALISE_CONJUR", 2491);
define("WF_EM_ANALISE_SETEC_ANALISE_CONJUR", 2492);
define("WF_AJUSTE_DEMANDANTE_ANALISE_CONJUR", 2531);

#WORKFLOW - ACÕES PARA ENVIO PARA ANALISE DA SESU
define("WF_CADASTRAMENTO_ENVIAR_ANALISE_SESU", 2488);
define("WF_AGUARD_AUT_DIRIGEN_ENVIAR_ANALISE_SESU", 2737);

#WORKFLOW - ACÕES PARA ENVIO PARA ANALISE DA SETEC
define("WF_CADASTRAMENTO_ENVIAR_ANALISE_SETEC", 2489);
define("WF_AGUARD_AUT_DIRIGEN_ENVIAR_ANALISE_SETEC", 2741);

#WORKFLOW - ACÕES PARA ENVIO PARA ANALISE DA MINISTRO
define("WF_AGUARD_AUT_DIRIGEN_ENVIAR_AUTORIZACAO_MINISTERIAL", 1326);
define("WF_AUTORIZ_MINISTRO_RETORNA_AUTORIZACAO_MINISTERIAL", 2805);
define("WF_EM_ANALISE_CONJUR_ENVIAR_AUTORIZACAO_MINISTERIAL", 2740);

#WORKFLOW - ACÕES PARA ENVIO PARA ANALISE DO SECRETARIO EXECUTIVO
define("WF_AGUARD_AUT_DIRIGEN_ENVIAR_AUTORIZACAO_SECRETARIO", 1335);
define("WF_EM_ANALISE_CONJUR_ENVIAR_AUTORIZACAO_SECRETARIO_EXEC", 2742);

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){	
    define("WF_ASSISTENCIA_ESTUDANTIL_EM_PREENCHIMENTO", 584);
    define("WF_ASSISTENCIA_ESTUDANTIL_EM_ANALISE_MEC", 585);
    define("WF_ASSISTENCIA_ESTUDANTIL_APROVADO", 586);
    define("WF_ASSISTENCIA_ESTUDANTIL_DEVOLVIDO_PARA_AJUSTE", 587);
}else{
    define("WF_ASSISTENCIA_ESTUDANTIL_EM_PREENCHIMENTO", 593);
    define("WF_ASSISTENCIA_ESTUDANTIL_EM_ANALISE_MEC", 594);
    define("WF_ASSISTENCIA_ESTUDANTIL_APROVADO", 595);
    define("WF_ASSISTENCIA_ESTUDANTIL_DEVOLVIDO_PARA_AJUSTE", 596);
}

#CONSTANTES USADAS NO ARQUIVO "autorizacaoSolicitacaoDecreto.inc" (PROVAAVEL QUE EXISTAS CONTANTES COM NOMES DIFERENTES MAS COM O MESMO ID)
if($_SESSION['baselogin']=="simec_desenvolvimento") {
    define("ESD_AGUARDANDO_MINISTRO", 494);
    define("ESD_AGUARDANDO_SECRETARIO", 500);
    define("EPC_PRORROGACAO", 1);
    define("TPC_LOCACAO", 2);
    define("TPC_LOCACAOADM", 3);
    define("ESD_AUTORIZADO_MINISTRO", 496);
    define("ESD_AUTORIZADO_SECRETARIO", 495);
    define("PFL_REITOR", 526);
    define("AED_AUTORIZAR_MINISTRO", 1260);
    define("AED_AUTORIZAR_SECRETARIO", 1264);
}else{
    define("ESD_AGUARDANDO_MINISTRO", 494);
    define("ESD_AGUARDANDO_SECRETARIO", 500);
    define("EPC_PRORROGACAO", 1);
    define("TPC_LOCACAO", 2);
    define("TPC_LOCACAOADM", 3);
    define("ESD_AUTORIZADO_MINISTRO", 496);
    define("ESD_AUTORIZADO_SECRETARIO", 495);
    define("PFL_REITOR", 526);
    define("AED_AUTORIZAR_MINISTRO", 1330);
    define("AED_AUTORIZAR_SECRETARIO", 1334);
}

define("DATA_ASSINATURA_AUTORIZACAO_DG", '20120727');


#ID DO FLUXO DO WORK FLOW REDE FEDERAL - AUTORIZAÇÃO DE INAUGURAÇÕES - INSTITUTOS
define( "WF_FLUXO_INAUGURACOES_INSTITUTOS", 234 );

/***********************
 * 
 * FIM - WORKFLOW
 * 
 ***********************/


/*** QUESTIONÁRIO ***/
define("QUEID_ACADEMICO", 103);

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){	
    define("QUEID_PROGRAMA_INCLUIR", 87);
    define("ENT_SAA", "742767");	
} else {
    define("QUEID_PROGRAMA_INCLUIR", 88);
     define("ENT_SAA", "742798");
}

define("FUNCAO_ENTIDADE_POLO_FORMACAO_DOCENTE", 94);
define("FUNCAO_ENTIDADE_TUTOR", 116);
define("FUNCAO_ENTIDADE_SUPERVISOR", 117);

?>