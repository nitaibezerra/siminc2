<?php

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){	
	
	#SERVIDOR LOCAL
	
	#TIPO DE DOCUMENTO
	define("WF_TIPOS_DE_DOCUMENTOS", 84);
	
	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENTÁVEL - ALUNOS.  DESENVOLVIMENTO.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL_ALUNOS", 99);
	
	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENTÁVEL. DESENVOLVIMENTO.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL", 100);
	
	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENTÁVEL. DESENVOLVIMENTO.
	define("WF_ES_EM_CADASTRAMENTO", 645);
	define("WF_ES_EM_VALIDACAO_DIRETOR", 646);
	define("WF_ES_ENCAMINHAR_MEC", 647);
	
	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENTÁVEL - POR ALUNOS. DESENVOLVIMENTO.
	define("WF_ES_EM_CADASTRAMENTO_ALUNOS", 639);
	define("WF_ES_EM_VALIDACAO_PROREITOR_ALUNOS", 640);
	define("WF_ES_ENCAMINHAR_MEC_ALUNOS", 641);
	
	### DESCENTRALIZACAO ###
	define("WF_TPDID_DESCENTRALIZACAO", 84);
        
	// Estados workflow
	define("EM_CADASTRAMENTO", 559);
	define("EM_APROVACAO_DA_REITORIA", 560);
	define("EM_ANALISE_DA_SECRETARIA", 562);
	define("EM_ANALISE_OU_PENDENTE", 563);//analise da diretoria
	define("AGUARDANDO_APROVACAO_SECRETARIO", 564);
	define("EM_ANALISE_PELA_CGSO", 565);
	define("EM_ANALISE_PELA_SPO", 566);
	define("EM_DESCENTRALIZACAO", 567);
	define("EM_EMISSAO_NOTA_CREDITO", 1034);	
	define("EM_EXECUCAO", 568);
	define("TERMO_FINALIZADO", 569);
	define("EM_DILIGENCIA", 648);
	define("AGUARDANDO_APROVACAO_DIRETORIA", 642);
	define("ALTERAR_TERMO_COOPERACAO", 643);
	define("EM_ANALISE_COORDENACAO", 649);
	define("TERMO_ARQUIVADO", 654);
	
	define("AGUARDANDO_APROVACAO_GESTOR_PROPONENTE", 631);	
	define("AGUARDANDO_DISPONIBILIDADE_ORCAMENTARIA", 654);
	define("RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR", 652);
	define("RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA", 655);
	define("RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD", 656);
	
	
	//Acao workflow
	define("AEDID_ARQUIVAR_TERMO", 1544);
	### FIM DESCENTRALIZACAO ###
	
	#Perfis
	define("PERFIL_PROREITOR_ADM", 872);
	define("PERFIL_DIRETOR_ADMIM", 876);
	define("PERFIL_COORDENADOR_SEC", 875);
	define("PERFIL_CGSO", 826);
	define("PERFIL_REITOR", 825);
	define("PERFIL_SECRETARIO", 824);
	define("PERFIL_SECRETARIA", 822);
	define("PERFIL_DIRETORIA", 823);
	define("PERFIL_SUBSECRETARIO", 827);
    define('PERFIL_ADMINISTRADOR_PROPOSTAS', 1011);
    define('PERFIL_GABINETE_SECRETARIA_AUTARQUIA', 859);

	/*
	 * Tramitação em Lote
	 * */
	
	define("LOTE_TIPO_DESCENTRALIZACAO", 1);
	
}else{
	
	#SERVIDOR DE PRODUÇÃO
	
	#TIPO DE DOCUMENTO
	define("WF_TIPOS_DE_DOCUMENTOS", 97);
	
	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENTÁVEL - ALUNOS. PRODUÇÃO.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL_ALUNOS", 92);

	#TIPO DE DOCUEMNTO PROJETO ESPANADA SUSTENTÁVEL. PRODUÇÃO.
	define("WF_PROJETO_ESPLANADA_SUSTENTAVEL", 91);

	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENTÁVEL. PRODUÇÃO.
	define("WF_ES_EM_CADASTRAMENTO", 604);
	define("WF_ES_EM_VALIDACAO_DIRETOR", 605);
	define("WF_ES_ENCAMINHAR_MEC", 606);
	
	#ESTADO DOCUMENTO PROJETO ESPANADA SUSTENTÁVEL - POR ALUNOS. PRODUÇÃO.
	define("WF_ES_EM_CADASTRAMENTO_ALUNOS", 607);
	define("WF_ES_EM_VALIDACAO_PROREITOR_ALUNOS", 608);
	define("WF_ES_ENCAMINHAR_MEC_ALUNOS", 609);
	
	### DESCENTRALIZACAO ###
	
	define("WF_TPDID_DESCENTRALIZACAO", 97);

        /** Termo em cadastramento */
	define("EM_CADASTRAMENTO", 631);
        
        /** Termo aguardando aprovação do Representante Legal do Proponente */
	define("EM_APROVACAO_DA_REITORIA", 632);
        
        /** Em distribuição pelo gabinete da secretaria/autarquia */
	define("EM_ANALISE_DA_SECRETARIA", 633);
        
        /** analise da diretoria - \Em analise pela coordenação */
	define("EM_ANALISE_OU_PENDENTE", 634);
        
        /** Termo aguardando aprovação pelo  Representante Legal do Concedente */
	define("AGUARDANDO_APROVACAO_SECRETARIO", 635);
        
        /** Termo em análise pela UG repassadora */
	define("EM_ANALISE_PELA_CGSO", 636); 
        
        /** Termo em Análise pelo Gestor Orçamentário do Concedente */
	define("EM_ANALISE_PELA_SPO", 637);
        
        /** Termo aprovado, aguardando execução da descentralização */
	define("EM_DESCENTRALIZACAO", 638);
        
        /** Termo em Execução */
	define("EM_EXECUCAO", 639);
        
        /** Termo finalizado */
	define("TERMO_FINALIZADO", 640);
        
        /** Termo com solicitação de alteração */
	define("ALTERAR_TERMO_COOPERACAO", 641);
        
        /** Aguardando aprovação pela diretoria */
	define("AGUARDANDO_APROVACAO_DIRETORIA", 642);
        
        /** Em Diligência */
	define("EM_DILIGENCIA", 643);
        
        /** Em análise da coordenação */
	define("EM_ANALISE_COORDENACAO", 644);
        
        /** Arquivado */
	define("TERMO_ARQUIVADO", 647);
	
        /** Termo aguardando disponibilidade orçamentária */
	define("AGUARDANDO_DISPONIBILIDADE_ORCAMENTARIA", 654);
        
        /** Termo em analise pela DIGAP */
	define("RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR", 652);
        
        /** Termo aguardando aprovação do Gestor Orçamentário do Proponente */
	define("TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP", 653);
        
        /** Relatório de cumprimento do objeto aguardando aprovação do Representante Legal do Proponente */
	define("RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA", 655);
        
        /** Relatório de cumprimento do objeto em análise pela coordenação */
	define("RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD", 656);
        
        /** Termo em Diligência do Relatório de cumprimento */
	define("TERMO_EM_DILIGENCIA_RELATORIO", 660);

    /** Termo em análise orçamentária no FNDE **/
    define("TERMO_EM_ANALISE_ORCAMENTARIA_FNDE", 662);

    /** Termo aguardando validação da diretoria no FNDE **/
    define("TERMO_AGUARDANDO_VALIDACAO_DIRETORIA_FNDE", 888);

	//Acao workflow
	define("AEDID_ARQUIVAR_TERMO", 1608);
	
	#Perfis
	define("PERFIL_PROREITOR_ADM", 852);
	define("PERFIL_DIRETOR_ADMIM", 851);
	define("PERFIL_COORDENADOR_SEC", 866);
	define("PERFIL_CGSO", 862); //UG Repassadora
	define("PERFIL_REITOR", 864);
	define("PERFIL_SECRETARIO", 865);
    define("PERFIL_REPRESENTATE_LEGAL_CONCEDENTE", 865);
	define("PERFIL_SECRETARIA", 859);
	define("PERFIL_DIRETORIA", 860);
	define("PERFIL_SUBSECRETARIO", 863);
    define('PERFIL_ADMINISTRADOR_PROPOSTAS', 1011);
    define('PERFIL_DIRETORIA_FNDE', 1052);
    define('PERFIL_AREA_TECNICA_FNDE', 871);
    define('PERFIL_GABINETE_SECRETARIA_AUTARQUIA', 859);
    define('PERFIL_SUPER_USUARIO', 23);

	/*
	 * Tramitação em Lote
	* */
	
	define("LOTE_TIPO_DESCENTRALIZACAO", 1);
}

define("EM_EMISSAO_NOTA_CREDITO", 1034);
define("WF_ACAO_SOL_ALTERACAO", 1620);

#PERFIL
define("PERFIL_CONSULTA_ESPLANADA_SUSTENTAVEL", 867);
define("PERFIL_ANALISTA_DIGAP", 871);
define("PERFIL_SUPER_USUARIO", 23);
define("CGO_COORDENADOR_ORCAMENTO", 50);
define("CGO_EQUIPE_ORCAMENTARIA", 52);
define("UO_COORDENADOR_EQUIPE_TECNICA", 53);
define("UO_EQUIPE_TECNICA", 54);
define("MEC_CONSULTA_ORCAMENTO_GERAL", 55);
define("UO_CONSULTA_ORCAMENTO", 57);
define("AUDITOR_EXTERNO", 76);
define("AUDITOR_INTERNO", 388);

#Hints
define("OBJETIVO_HINT", "Favor Preencher com a descrição do objeto a ser executado, indicando, inclusive, o campus em que se localizará o objeto. O objeto é o que deve ser fisicamente entregue à sociedade ao final da execução do Plano de Trabalho.");
define("JUSTIFICATIVA_HINT", "Favor registrar: Contextualização da obra no campus em que o projeto será executado; Motivação da obra, isto é, qual o problema que a obra busca sanar e qual a demanda para o projeto. Caso a proposta tenha recursos a serem descentralizados em mais de um exercício, o proponente deverá inserir no campo da justificativa o comentário de como o recurso deverá ser distribuído ao longo dos exercícios. Ex.: A construção em questão deverá ter aporte de recursos distribuídos em mais de um exercício. Sendo a parcela para 2013 de R$ XX, para 2014 de R$ YY e para 2015 de R$ ZZ.");

#UGs
define("UG_FNDE", 		153173);
define("UG_CGSO", 		152734);
define("UG_CAPES", 		154003);
define("UG_INEP", 		153978);
define("UG_SECADI", 	150028);
define("UG_SETEC", 		150016);
define("UG_SEB", 		150019);

#UOs
define("UO_FNDE", 		26298);

define('MODULO_NAME', 'Termo de Execução Descentralizada');

?>