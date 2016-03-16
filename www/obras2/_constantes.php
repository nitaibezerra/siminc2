<?php

define('MSG001', 'Operaчуo realizada com sucesso!');
define('MSG002', 'Operaчуo nуo pode ser realizada!');

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	define('WF_TPDID_DEMANDA' ,                                   145);
	/*
	 * INICIO de estados do documento da demanda
	 * _____________________________________________________________________________
	 */
	// Envia para AGUARDANDO_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO" ,          928);
	// Envia para ANALISE_DE_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO" ,    929);
	// Envia para ATENDIMENTO ou EM_DILIGENCIA
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_ANALISE_DE_ATENDIMENTO" , 930);
	// Envia para EM_DILIGENCIA ou EM_PAUSA ou AGUARDANDO_VALIDACAO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_ATENDIMENTO" ,            932);
	// Volta para EM_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_PAUSA" ,                  934);
	// Volta para ANALISE_DE_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_DILIGENCIA" ,             931);
	// Envia para ESTADO_CONCLUIDO
	define("OBRAS2_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO" ,      935);
	// Nao pode mais fazer nada.
	define("OBRAS2_WF_DEMANDA_ESTADO_CONCLUIDO" ,                 933);
}
else{
	define('WF_TPDID_DEMANDA' ,                                   147);
	/*
	 * INICIO de estados do documento da demanda
	* _____________________________________________________________________________
	*/
	// Envia para AGUARDANDO_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO" ,          933);
	// Envia para ANALISE_DE_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_AGUARDANDO_ATENDIMENTO" ,    934);
	// Envia para ATENDIMENTO ou EM_DILIGENCIA
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_ANALISE_DE_ATENDIMENTO" , 935);
	// Volta para ANALISE_DE_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_DILIGENCIA" ,             936);
	// Envia para EM_DILIGENCIA ou EM_PAUSA ou AGUARDANDO_VALIDACAO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_ATENDIMENTO" ,            937);
	// Envia para ESTADO_CONCLUIDO
	define("OBRAS2_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO" ,      938);
	// Nao pode mais fazer nada.
	define("OBRAS2_WF_DEMANDA_ESTADO_CONCLUIDO" ,                 939);
	// Volta para EM_ATENDIMENTO
	define("OBRAS2_WF_DEMANDA_ESTADO_EM_PAUSA" ,                  940);
}
/*
 * FIM de estados do documento da demanda
 * _____________________________________________________________________________
 */


if ($_SESSION['baselogin'] == 'simec_desenvolvimento') {

    // Pefil
    define("PFLCOD_SUPER_USUARIO", 832);
    define("PFLCOD_ADMINISTRADOR", 841);
    define("PFLCOD_CADASTRADOR_INSTITUCIONAL", 842);
    define("PFLCOD_SAA", 843);
    define("PFLCOD_GESTOR_MEC", 844);
    define("PFLCOD_SUPERVISOR_MEC", 845);
    define("PFLCOD_GESTOR_UNIDADE", 846);
    define("PFLCOD_CONSULTA_GERAL", 847);
    define("PFLCOD_SUPERVISOR_UNIDADE", 848);
    define("PFLCOD_AUDITOR_INTERNO", 849);
    define("PFLCOD_CONSULTA_ESTADUAL", 850);
    define("PFLCOD_CONSULTA_TIPO_DE_ENSINO", 851);
    define("PFLCOD_CONSULTA_UNIDADE", 852);
    define("PFLCOD_EMPRESA_CONTRATADA", 853);
    define("PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC", 909);
    define("PFLCOD_EMPRESA_VISTORIADORA_GESTOR", 910);
    define("PFLCOD_EMPRESA_VISTORIADORA_FISCAL", 911);
    define("PFLCOD_EMPRESA_MI_GESTOR", 1008);
    define("PFLCOD_EMPRESA_MI_FISCAL", 1036);
    define("PFLCOD_EMPRESA_MI_ADMINISTRATIVO", 1299);
    define("PFLCOD_CALL_CENTER", 1023);
//	define("PFLCOD_CONSULTA_ESTADUAL",				1015);
    define("PFLCOD_ALERTA_MI_INTERNO", 1066);
    
    define("PFLCOD_CGIMP_GESTOR",  1276);
    define("PFLCOD_CGIMP_TECNICO", 1278);

    // (obras2.tipologiaobra)
//104;"MI - Escola de Educaчуo Infantil Tipo B"
    define("TPOID_MI_TIPO_B", 104);
//105;"MI - Escola de Educaчуo Infantil Tipo C"
    define("TPOID_MI_TIPO_C", 105);

    // (obras2.etapaquestao)
    define("ETQID_SUPERVISAO", 1);
    define("ETQID_VINCULADA", 9);
    define("ETQID_DESEMBOLSO", 10);

    // (obras2.situacaoobra)
    define("SOBID_EXECUCAO", 1);
    define("SOBID_DESACORDO_EXECUCAO", 2);
    define("SOBID_CONCLUIDA", 3);
    define("SOBID_PARALISADA", 4);

    // situaчуo atividade (obras2.situacaoatividade)
    define("STAID_EXECUCAO", 1);
    define("STAID_PARALISADO", 2);
    define("STAID_CONCLUIDO", 3);
    define("STAID_ELABORACAO_PROJETO", 4);
    define("STAID_LICITACAO", 5);
    define("STAID_CONTRATO_CANCELADO", 6);
    define("STAID_CONVENIO_CANCELADO", 7);
    define("STAID_ANALISE_CAIXA", 8);
    define("STAID_REFORMULACAO", 9);
    define("STAID_OBRA_CANCELADA", 10);
    define("STAID_ETAPA_CONCLUIDA", 11);
    define("STAID_REGISTRO_PRECO", 12);
    define("STAID_PREPARATORIA", 99);

    // tipo de situaчуo do registro
    define("STRID_PLANEJAMENTO_PROPONENTE", 1);
    define("STRID_LICITACAO", 2);
    define("STRID_CONTRATACAO", 3);
    define("STRID_EXECUCAO", 4);
    define("STRID_PARALISADA", 5);
    define("STRID_CONCLUIDA", 6);
    define("STRID_INACABADA", 7);
    define("STRID_CANCELADA", 8);

    // tipo de paralisacao (obras2.tipoparalisacao)
    define("TPLID_EMBARGO", 1);
    define("TPLID_ABANDONO_EMPRESA", 2);
    define("TPLID_CONTRATO_RESCINDIDO", 3);
    define("TPLID_OUTROS", 4);
    define("TPLID_OS_PARALISACAO", 5);

    // tipo de detalhamento da execuчуo financeira (obras2.itensexecucaoorcamentaria)
    define("TIPO_DETALHAMENTO_EXECUCAO_EMPENHADO", 'E');
    define("TIPO_DETALHAMENTO_EXECUCAO_LIQUIDADO", 'L');

    // tipo execuчуo orчamentсria (obras2.tipoexecorcamentaria)
    define("TIPO_EXEC_ORCAMENTARIA_OBRA", 1);
    define("TIPO_EXEC_ORCAMENTARIA_EQUIPAMENTO", 2);

    // forma repasse recurso (obras2.tipoformarepasserecursos)
    define("FRPID_CONVENIO", 2);
    define("FRPID_DESCENTRALIZACAO", 3);
    define("FRPID_RECURSO_PROPRIO", 4);
    define("FRPID_TESOURO_UO", 6);

    // Tipo foto obras2.tipoarquivo
//	define("TIPO_OBRA_ARQUIVO_FOTO",		'F');
//	define("TIPO_OBRA_ARQUIVO_DOCUMENTO",	'D');
    define("TIPO_OBRA_ARQUIVO_FOTO_VISTORIA", 23);
    define("TIPO_OBRA_ARQUIVO_HOMOLOGACAO", 24);
    define("TIPO_OBRA_ARQUIVO_OUTROS", 21);

    // Situaчуo da obra obras2.situacaoobra
    define("SITUACAO_OBRA_EM_EXECUCAO", 1);
    define("SITUACAO_OBRA_CONCLUIDA", 3);
    define("SITUACAO_OBRA_PARALISADA", 4);

    // tipo endereчo entidade.endereco
    define("TIPO_ENDERECO_OBRA", 3);
    define("TIPO_ENDERECO_OBJETO", 4); // Estс sendo usado tanto para EMPREENDIMENTO (obra) como para OBRA (objeto)
    // гrgуos do mѓdulo de obras
    define("ORGID_EDUCACAO_SUPERIOR", 1);
    define("ORGID_EDUCACAO_PROFISSIONAL", 2);
    define("ORGID_EDUCACAO_BASICA", 3);
    define("ORGID_ADMINISTRATIVO", 4);
    define("ORGID_HOSPITAIS", 5);

    // Constantes das funчѕes das entidades
    define('ID_UNIVERSIDADE', 12);
    define('ID_HOSPITAL', 16);
    define('ID_ESCOLAS_TECNICAS', 11);
    define('ID_ESCOLAS_AGROTECNICAS', 14);
    define('ID_ADM', 34);

    // ids das unidades
    define("ID_UNIDADEIMPLANTADORA", 44);
    define("ID_CAMPUS", 18);
    define("ID_UNED", 17);
    define("ID_SUPERVISIONADA", 35);
    define("ID_REITORIA", 75);
    define("ID_UNIESTADUAL", 42);

    // IDs Abas
    define("ID_ABA_LISTA_EMP", 57594);
    define("ID_ABA_CADASTRA_EMP", 57597);
    define("ID_ABA_EMP_CADASTRADO", 57598);
    define("ID_ABA_CADASTRA_OBRA", 57599);
    define("ID_ABA_OBRA_CADASTRADA_FNDE", 57603);
    define("ID_ABA_CUMPRIMENTO_OBJETO", 57896);
    define("ID_ABA_OBRA_CADASTRADA", 57602);
    define("ID_ABA_CADASTRA_OBRA_EMP", 57601);
    define("ID_ABA_DETALHAMENTO_ITEM_OBRA", 57605);
    define("ID_ABA_PREVISAO_DETALHAMENTO_OBRA", 57606);
    define("ID_ABA_CADASTRO_VISTORIA_EMP", 57630);
    define("ID_ABA_CADASTRO_VISTORIA_EMP_EDICAO", 57631);
    define("ID_ABA_CADASTRO_VISTORIA_EMPRESA", 57632);
    define("ID_ABA_CADASTRO_VISTORIA_EMPRESA_EDICAO", 57633);
    define("ID_ABA_CADASTRO_VISTORIA", 57609);
    define("ID_ABA_OS_MI", 57790);
    define("ID_ABA_CADASTRO_VISTORIA_FNDE", 57610); // Empreendimento junto da obra
    
    define("ID_ABA_MONITORAMENTO_ESPECIAL", 57837);
    
    // IDs Menus do sistema
    define('ID_MENU_LICITACAO', 12803);
    define('ID_MENU_CONTRATACAO', 12804);
    define('ID_MENU_VISTORIA', 12779);
    define('ID_MENU_CRONOGRAMA', 12736);

    // IDs tipo fase licitaчуo ( obras2.tiposfaseslicitacao ) 
    define('FASE_LIC_PUBLICACAO_EDITAL', 2);
    define('FASE_LIC_RECURSO_INTERPOSTO', 5);
    define('FASE_LIC_ORDEM_SERVICO', 6);
    define('FASE_LIC_ABERTURA_PROPOSTA', 7);
    define('FASE_LIC_HOMOLOGACAO', 9);

//	// Workflow Obras (OBJETO)
//	define("TPDID_OBRA",97);
//	
//	define("ESD_CADASTRAMENTO",623);
////		define("AEDID_OBJ_CADAST_EXECUCAO",	1755);
//	define("ESD_EXECUCAO",624);
//	define("ESD_PARALIZADA",625);
//	define("ESD_CANCELADA",626);
//	define("ESD_CONCLUЭDA",627);
    //ID fundi Obras
    define("ID_RESPONSAVELOBRAS", 45);

    // IDs Workflow Demanda
    define("TPDID_DEMANDA", 98);

    define("ESDID_CADASTRAMENTO", 632);
    define("ESDID_APROVACAO", 633);
    define("ESDID_CONTRATACAO", 634);
    define("ESDID_CONCLUIDO", 635);
    define("ESDID_CANCELADO", 636);

    // IDs Workflow Vistoria
    define("TPDID_VISTORIA", 101);
    define("ESDID_VISTORIA_CADASTRAMENTO", 653);

    // IDs Workflow supervisуo MI
    define("TPDID_SUPERVISAO_MI", 128);
    define("ESDID_MI_CADASTRAMENTO", 837);
    define("ESDID_MI_VALIDACAO"    , 1042);
    define("ESDID_MI_CONCLUIDO"    , 1043);
    define("ESDID_MI_CORRECAO"     , 1044);
    define("AEDID_MI_ENVIAR_VALIDACAO", 2448);
    define("AEDID_MI_ENVIAR_VALIDACAO_POS_CORRECAO", 2451);
    
     // IDs Workflow Cumprimento do objeto
    define("TPDID_CUMPRIMENTO_OBJETO", 200);
    define("ESDID_CUMPRIMENTO_CADASTRAMENTO" , 1266);
    define("ESDID_CUMPRIMENTO_DILIGENCIADO" , 1267);
    define("ESDID_CUMPRIMENTO_VALIDACAO_FNDE" , 1268);
    #define("ESDID_CUMPRIMENTO_APROVADO_TOTALMENTE" , 1269); DESABILITADO
    #define("ESDID_CUMPRIMENTO_REPROVADO_TOTALMENTE" , 1621); DESABILITADO
    #define("ESDID_CUMPRIMENTO_APROVADO_PARCIALMENTE" , 1622); DESABILITADO
    define("ESDID_CUMPRIMENTO_AGUARDANDO_VALIDACAO_PROCESSO" , 1623);
    define("ESDID_CUMPRIMENTO_AGUARDANDO_DEFERIMENTO" , 1624);
    define("ESDID_CUMPRIMENTO_AGUARDANDO_APROVACAO" , 1625);
    define("ESDID_CUMPRIMENTO_APROVADO" , 1626);
    
    define("ESDID_MI_CADASTRAMENTO", 837);
    define("ESDID_MI_VALIDACAO"    , 1042);
    define("ESDID_MI_CONCLUIDO"    , 1043);
    define("ESDID_MI_CORRECAO"     , 1044);
    define("AEDID_MI_ENVIAR_VALIDACAO", 2448);
    define("AEDID_MI_ENVIAR_VALIDACAO_POS_CORRECAO", 2451);
    
    
//    define("ESDID_MI_AGUARDANDO_HOMOLOGACAO", 838);
//    define("ESDID_MI_HOMOLOGADO", 839);

    // IDs Workflow Laudo / Supervisao (EMPRESA)
    define("WF_TPDID_LAUDO_SUPERVISAO_EMPRESA", 116);
    define("WF_ESDID_LAUDO_SUPERVISAO_EM_CADASTRAMENTO", 732);
    define("WF_ESDID_LAUDO_SUPERVISAO_AGUARDANDO_HOMOLOGACAO", 733);
    define("WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO", 734);
    define("WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO", 756);
    define("WF_ESDID_LAUDO_SUPERVISAO_PAGO", 757);
    define("WF_ESDID_LAUDO_SUPERVISAO_CANCELADO", 1188);

    //IDs Aчуo Estado WorkFkow Laudo / Supervisao (EMPRESA)
    define("WF_AEDID_LAUDO_SUPERVISAO_HOMOLOGAR", 1726);
    define("WF_AEDID_LAUDO_SUPERVISAO_SOLICITAR_PAGAMENTO", 1776);
    define("WF_AEDID_LAUDO_SUPERVISAO_REALIZAR_PAGAMENTO", 1777);

    // IDs Workflow Pagamento Supervisуo
    define("WF_TPDID_PAGAMENTO_SUPERVISAO", 120);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_EM_CADASTRAMENTO", 758);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_ENVIADO_FNDE", 759);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_PAGAMENTO_SOLICITADO", 760);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_PAGO", 761);

    define("ESDID_VISTORIA_EMP_CADASTRAMENTO", 732);
    define("ESDID_VISTORIA_EMP_ANALISE_GESTOR", 733);
    define("ESDID_VISTORIA_EMP_LAUDO", 734);

    // IDs Workflow O.S
    define("TPDID_OS", 115);

    define("ESDID_OS_CADASTRAMENTO", 728);
    define("ESDID_OS_CANCELADA", 729);
    define("ESDID_OS_EXECUCAO", 730);
    define("ESDID_OS_CONCLUIDA", 731);
    define("ESDID_OS_ENVIADA_EMPRESA", 765);

    define("AEDID_OS_CONCLUIDA", 1723);

//	// IDs workflow Empreendimento (OBRA)
//	define("TPDID_EMPREENDIMENTO", 102);
//	define("ESDID_EMP_CADASTRAMENTO",	656);	
//	define("ESDID_EMP_DIVISAO",			657);	
//	define("ESDID_EMP_LICITACAO",		658);	
//	define("ESDID_EMP_CONTRATACAO",		659);
//		
//	define("ESDID_EMP_EXECUCAO",		660);	
////		define("AEDID_EMP_EXEC_PARALISAR",	1742);	
//	define("ESDID_EMP_PARALISACAO",		661);	
//	define("ESDID_EMP_CANCELADA",		662);	
//	define("ESDID_EMP_CONCLUIDA",		663);	
    // IDs workflow Obra (OBJETO)
    define("TPDID_OBJETO", 105);
    define("ESDID_OBJ_REPASSE", 762);
    define("AEDID_OBJ_LICITACAO_LICITACAO", 2896); // Registra obra vinculada
    define("ESDID_OBJ_PLANEJAMENTO_PROPONENTE", 689);
    define("AEDID_OBJ_PLANEJAMENTO_CANCELADO", 1861);
    define("ESDID_OBJ_LICITACAO", 763);
    define("ESDID_OBJ_AGUARDANDO_1_REPASSE", 762);
    define("ESDID_OBJ_CONTRATACAO", 764);
    define("AEDID_OBJ_CONTRATACAO_EXECUCAO", 1785);
    define("ESDID_OBJ_EXECUCAO", 690);
    define("AEDID_OBJ_EXECUCAO_PARALISADO", 1757);
    define("AEDID_OBJ_EXECUCAO_CONCLUIDO", 1758);
    define("AEDID_OBJ_EXECUCAO_ADITIVO", 1756);
    define("ESDID_OBJ_PARALISADO", 691);
    define("AEDID_OBJ_PARALISADO_EXECUCAO", 1759);
    define("ESDID_OBJ_CONCLUIDO", 693);
    define("ESDID_OBJ_INACABADA", 1084);
    define("AEDID_OBJ_CONCLUIDO_EXECUCAO", 1964); // produчуo 1964);
    define("AEDID_OBJ_CONCLUIDO_PARALISADO", 1965); // produчуo 1965);
    define("ESDID_OBJ_ADITIVO", 692);
    define("ESDID_OBJ_CONTRATO_CANCELADO", 766);
    define("ESDID_OBJ_CONVENIO_CANCELADO", 767);
    define("ESDID_OBJ_REFORMULACAO", 768);
    define("ESDID_OBJ_CANCELADO", 769);
    define("ESDID_OBJ_CONCLUIDA", 770);
    define("ESDID_OBJ_REGISTRO_PRECO", 771);
    define("ESDID_OBJ_AGUARDANDO_EMISSAO_OS", 864);
    define("ESDID_OBJ_AGUARDANDO_ACEITE_OS", 873);
    define("AEDID_OBJ_ACEITE_RECUSADA", 1973);
    define("AEDID_OBJ_ACEITE_EXECUCAO", 1972);
    define("ESDID_OBJ_OS_RECUSADA", 874);

    // IDs workflow Obra MI (OBJETO)
    define("TPDID_OBRAMI", 126);
    define("ESDID_OBJMI_ADESAO_MUNICIPIO", 774);
    define("ESDID_OBJMI_AUTORIZACAO_FNDE", 816);
    define("ESDID_OBJMI_ANUENCIA_FORNECEDOR", 817);
    define("ESDID_OBJMI_TERRAPLANAGEM", 818);
    define("ESDID_OBJMI_ASSINATURA_CONTRATO", 819);
    define("ESDID_OBJMI_EMISSAO_OS", 820);
    define("ESDID_OBJMI_PARALISADA", 821);
    define("ESDID_OBJMI_EXECUCAO", 822);
    define("ESDID_OBJMI_CONCLUIDA", 823);

//	define("TPDID_OBJETO", 105);
//	define("ESDID_OBJ_CADASTRAMENTO",	689);	
////		define("AEDID_OBJ_CADAST_EXECUCAO",	1755);
//	define("ESDID_OBJ_EXECUCAO",		690);
////		define("AEDID_OBJ_EXEC_PARALISAR",	1757);
//	define("ESDID_OBJ_PARALISACAO",		691);	
//	define("ESDID_OBJ_CANCELADA",		692);	
//	define("ESDID_OBJ_CONCLUIDA",		693);	
    // Tipos de Arquivo
    define('TIPO_ARQUIVO_FOTO_VISTORIA', 23);

    // Tipo de Aditivo
    define('TIPO_ADV_PRAZO', 1);
    define('TIPO_ADV_VALOR', 2);
    define('TIPO_ADV_PRAZOVALOR', 3);

    // MacroEtapa (macroitemcomposiчуo)
    define('MACROETAPA_SERVICO_PRELIMINARES', 1);
    define('MACROETAPA_INFRAESTRUTURA', 2);
    define('MACROETAPA_SUPERESTRUTURA', 3);
    define('MACROETAPA_SERVICO_COMPLEMENTARES', 4);

    //Aчуo estado documento do Workflow O.S.
    define('AEDID_WF_EXECUCAO', 1645);

    // Tipo OS MI obras2.tipoosmi
    define('TOMID_EXECUACAO', 1);
    define('TOMID_SONDAGEM', 2);
    define('TOMID_PROJETO', 3);

    // Tecnologia MI obras2.tecnologiami
    define('TMIID_PVC_CONCRETO', 1);
    define('TMIID_PLACA_CIMENTICIA', 2);
    define('TMIID_MATERIAIS_COMPOSITOS', 3);
    
    /**
     * Questionсrios do Checklist da Validaчуo da Obra
     */
    define('QUEID_QUEST_CHKLST_2P' ,      95);
    define('QUEID_QUEST_CHKLST_ADM',      96);
    define('QUEID_QUEST_CHKLST_ADM_SP', 109);
    define('QUEID_QUEST_CHKLST_ADM_2015', 116);
    define('QUEID_QUEST_CHKLST_TEC',      98);
    define('QUEID_QUEST_CHKLST_TEC_2015',      117);
//    define('QUEID_QUEST_CHKLST_TEC', 94);
    define('QUEID_QUEST_CHKLST_OBR_VINC', 99);
    define('QUEID_QUEST_CHKLST_OBR_MI', 107);

    define('QUEID_QUEST_CHKLST_SOLICITACOES', 120);
    define('QUEID_QUEST_CHKLST_CUMPRIMENTO', 121);
    
    //Workflow do Checklist da validaчуo
    define('TPID_CHECKLIST_VALIDACAO', 176);
    //Workflow Estados do Checklist da validaчуo
    define("ESDID_CHKLST_CADASTRAMENTO", 1088);
    define("ESDID_CHKLST_CONCLUIDO"    , 1089);
    define("ESDID_CHKLST_CORRECAO"     , 1090);
    
    // IDs Workflow Restriчуo/Inconformidade
    define("TPDID_RESTRICAO_INCONFORMIDADE", 186);

    define("ESDID_AGUARDANDO_PROVIDENCIA", 1140);
    define("ESDID_AGUARDANDO_ANALISE_FNDE", 1141);
    define("ESDID_SUPERADA", 1142);
    define("ESDID_CANCELADA", 1143);
    define("ESDID_AGUARDANDO_CORRECAO", 1144);
    define("ESDID_JUSTIFICADA", 1503);
    
    define("AEDID_CANCELAR",                             2653);
    define("AEDID_ENCAMINHAR_PARA_ANALISE",              2650);
    define("AEDID_CONFIRMAR_SUPERACAO",                  2651);
    define("AEDID_DEVOLVER_PARA_CORRECAO",               2652);
    define("AEDID_RETORNAR_PARA_ANALISE",                2654);
    define("AEDID_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA", 2655);
    define("AEDID_ENVIAR_PARA_ANALISE_FNDE",             2656);
    
    //Workflow de Atividade de Monitoramento Especial
    define('TPID_ME_ATIVIDADE_MONITORAMENTO', 196);
    //Workflow Estados do Monitoramento Especial
    define("ESDID_ME_ATIVIDADE_CADASTRAMENTO", 1237);
    define("ESDID_ME_ATIVIDADE_ANALISE"      , 1238);
    define("ESDID_ME_ATIVIDADE_CORRECAO"     , 1246);
    define("ESDID_ME_ATIVIDADE_CONCLUIDO"    , 1239);    
    //Workflow das Aчѕes da Atividade de Monitoramento Especial
    define("AEDID_ME_ATIVIDADE_ENVIAR_PARA_ANALISE"              ,2833);
    define("AEDID_ME_ATIVIDADE_ENVIAR_PARA_CORRECAO"             ,2834);
    define("AEDID_ME_ATIVIDADE_CONCLUIR_ANALISE"                 ,2835);
    define("AEDID_ME_ATIVIDADE_ENVIAR_DA_CORRECAO_PARA_ANALISE"  ,2836);
    
    //Workflow de Itens de Monitoramento Especial
    define('TPID_ME_ITEM_MONITORAMENTO', 197);
    //Workflow Estados do Monitoramento Especial
    define("ESDID_ME_ITEM_CADASTRAMENTO", 1247);
    define("ESDID_ME_ITEM_ANALISE"      , 1248);
    define("ESDID_ME_ITEM_CORRECAO"     , 1249);
    define("ESDID_ME_ITEM_CONCLUIDO"    , 1250);    
    //Workflow das Aчѕes dos Itens de Monitoramento Especial
    define("AEDID_ME_ITEM_ENVIAR_PARA_ANALISE"              ,2840);
    define("AEDID_ME_ITEM_ENVIAR_PARA_CORRECAO"             ,2841);
    define("AEDID_ME_ITEM_CONCLUIR_ANALISE"                 ,2842);
    define("AEDID_ME_ITEM_ENVIAR_DA_CORRECAO_PARA_ANALISE"  ,2843);

    // Situaчуo Obra
    define("STRID_OBJ_PLANEJAMENTO_PELO_PROPONENTE"  ,1);
    define("STRID_OBJ_LICITACAO"  ,2);
    define("STRID_OBJ_CONSTRACAO"  ,3);
    define("STRID_OBJ_EXECUCAO"  ,4);
    define("STRID_OBJ_PARALISADO"  ,5);
    define("STRID_OBJ_CONCLUIDO"  ,6);
    define("STRID_OBJ_INACABADO"  ,7);

    define("TPDID_SOLICITACAO_VINCULADA", 218);
    define("ESDID_AGUARDANDO_ANALISE" , 1427);
    define("ESDID_DEFERIDO" , 1428);
    define("ESDID_INDEFERIDO" , 1429);

    // IDs Workflow Solicitacao Desebolso
    define("TPDID_SOLICITACAO_DESEMBOLSO", 236);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO" , 1598);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI" , 1597);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL" , 1575);
    define("ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO" , 1576);
    define("ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO" , 1577);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA" , 1581);

    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_DEFERIDO", 3693);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO", 3805);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO", 3721);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO", 3692);

    define("AEDID_SOLICITACAO_DESEMBOLSO_CORRECAO_PARA_ANALISE", 3807);


    // IDs Workflow Solicitaчѕes
    define("TPDID_SOLICITACOES", 235);
    define("ESDID_SOLICITACOES_CADASTRAMENTO", 1570);
    define("ESDID_SOLICITACOES_AGUARDANDO_ANALISE", 1571);
    define("ESDID_SOLICITACOES_DEFERIDO", 1572);
    define("ESDID_SOLICITACOES_INDEFERIDO", 1573);
    define("ESDID_SOLICITACOES_DILIGENCIA", 1574);
    define("ESDID_SOLICITACOES_RETORNADO", 1592);

    define("AEDID_SOLICITACOES_CADASTRAMENTO_PARA_ANALISE", 3670);
    define("AEDID_SOLICITACOES_ANALISE_PARA_DEFERIDO", 3671);
    define("AEDID_SOLICITACOES_ANALISE_PARA_INDEFERIDO", 3672);
    define("AEDID_SOLICITACOES_ANALISE_PARA_DILIGENCIA", 3673);
    define("AEDID_SOLICITACOES_DILIGENCIA_PARA_DEFERIDO", 3674);
    define("AEDID_SOLICITACOES_DILIGENCIA_PARA_ANALISE", 3690);

}
elseif ($_SESSION['baselogin'] == 'simectreinamento') {
    // Pefil
    define("PFLCOD_SUPER_USUARIO", 832);
    define("PFLCOD_ADMINISTRADOR", 841);
    define("PFLCOD_CADASTRADOR_INSTITUCIONAL", 842);
    define("PFLCOD_SAA", 843);
    define("PFLCOD_GESTOR_MEC", 844);
    define("PFLCOD_SUPERVISOR_MEC", 845);
    define("PFLCOD_GESTOR_UNIDADE", 846);
    define("PFLCOD_CONSULTA_GERAL", 847);
    define("PFLCOD_SUPERVISOR_UNIDADE", 848);
    define("PFLCOD_AUDITOR_INTERNO", 849);
    define("PFLCOD_CONSULTA_ESTADUAL", 850);
    define("PFLCOD_CONSULTA_TIPO_DE_ENSINO", 851);
    define("PFLCOD_CONSULTA_UNIDADE", 852);
    define("PFLCOD_EMPRESA_CONTRATADA", 853);
    define("PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC", 909);
    define("PFLCOD_EMPRESA_VISTORIADORA_GESTOR", 910);
    define("PFLCOD_EMPRESA_VISTORIADORA_FISCAL", 911);
    define("PFLCOD_EMPRESA_MI_GESTOR", 1008);
    define("PFLCOD_EMPRESA_MI_FISCAL", 1036);
    define("PFLCOD_EMPRESA_MI_ADMINISTRATIVO", 1299);
    define("PFLCOD_CALL_CENTER", 1023);
//	define("PFLCOD_CONSULTA_ESTADUAL",				1015);
    define("PFLCOD_ALERTA_MI_INTERNO", 1066);
    
    define("PFLCOD_CGIMP_GESTOR",  1276);
    define("PFLCOD_CGIMP_TECNICO", 1278);


    // (obras2.tipologiaobra)
//104;"MI - Escola de Educaчуo Infantil Tipo B"
    define("TPOID_MI_TIPO_B", 104);
//105;"MI - Escola de Educaчуo Infantil Tipo C"
    define("TPOID_MI_TIPO_C", 105);

    // (obras2.etapaquestao)
    define("ETQID_SUPERVISAO", 1);
    define("ETQID_VINCULADA", 9);
    define("ETQID_DESEMBOLSO", 10);

    // (obras2.situacaoobra)
    define("SOBID_EXECUCAO", 1);
    define("SOBID_DESACORDO_EXECUCAO", 2);
    define("SOBID_CONCLUIDA", 3);
    define("SOBID_PARALISADA", 4);

    // situaчуo atividade (obras2.situacaoatividade)
    define("STAID_EXECUCAO", 1);
    define("STAID_PARALISADO", 2);
    define("STAID_CONCLUIDO", 3);
    define("STAID_ELABORACAO_PROJETO", 4);
    define("STAID_LICITACAO", 5);
    define("STAID_CONTRATO_CANCELADO", 6);
    define("STAID_CONVENIO_CANCELADO", 7);
    define("STAID_ANALISE_CAIXA", 8);
    define("STAID_REFORMULACAO", 9);
    define("STAID_OBRA_CANCELADA", 10);
    define("STAID_ETAPA_CONCLUIDA", 11);
    define("STAID_REGISTRO_PRECO", 12);
    define("STAID_PREPARATORIA", 99);

    // tipo de situaчуo do registro
    define("STRID_PLANEJAMENTO_PROPONENTE", 1);
    define("STRID_LICITACAO", 2);
    define("STRID_CONTRATACAO", 3);
    define("STRID_EXECUCAO", 4);
    define("STRID_PARALISADA", 5);
    define("STRID_CONCLUIDA", 6);
    define("STRID_INACABADA", 7);
    define("STRID_CANCELADA", 8);

    // tipo de paralisacao (obras2.tipoparalisacao)
    define("TPLID_EMBARGO", 1);
    define("TPLID_ABANDONO_EMPRESA", 2);
    define("TPLID_CONTRATO_RESCINDIDO", 3);
    define("TPLID_OUTROS", 4);
    define("TPLID_OS_PARALISACAO", 5);

    // tipo de detalhamento da execuчуo financeira (obras2.itensexecucaoorcamentaria)
    define("TIPO_DETALHAMENTO_EXECUCAO_EMPENHADO", 'E');
    define("TIPO_DETALHAMENTO_EXECUCAO_LIQUIDADO", 'L');

    // tipo execuчуo orчamentсria (obras2.tipoexecorcamentaria)
    define("TIPO_EXEC_ORCAMENTARIA_OBRA", 1);
    define("TIPO_EXEC_ORCAMENTARIA_EQUIPAMENTO", 2);

    // forma repasse recurso (obras2.tipoformarepasserecursos)
    define("FRPID_CONVENIO", 2);
    define("FRPID_DESCENTRALIZACAO", 3);
    define("FRPID_RECURSO_PROPRIO", 4);
    define("FRPID_TESOURO_UO", 6);

    // Tipo foto obras2.tipoarquivo
//	define("TIPO_OBRA_ARQUIVO_FOTO",		'F');
//	define("TIPO_OBRA_ARQUIVO_DOCUMENTO",	'D');
    define("TIPO_OBRA_ARQUIVO_FOTO_VISTORIA", 23);
    define("TIPO_OBRA_ARQUIVO_HOMOLOGACAO", 24);
    define("TIPO_OBRA_ARQUIVO_OUTROS", 21);

    // Situaчуo da obra obras2.situacaoobra
    define("SITUACAO_OBRA_EM_EXECUCAO", 1);
    define("SITUACAO_OBRA_CONCLUIDA", 3);
    define("SITUACAO_OBRA_PARALISADA", 4);

    // tipo endereчo entidade.endereco
    define("TIPO_ENDERECO_OBRA", 3);
    define("TIPO_ENDERECO_OBJETO", 4); // Estс sendo usado tanto para EMPREENDIMENTO (obra) como para OBRA (objeto)
    // гrgуos do mѓdulo de obras
    define("ORGID_EDUCACAO_SUPERIOR", 1);
    define("ORGID_EDUCACAO_PROFISSIONAL", 2);
    define("ORGID_EDUCACAO_BASICA", 3);
    define("ORGID_ADMINISTRATIVO", 4);
    define("ORGID_HOSPITAIS", 5);

    // Constantes das funчѕes das entidades
    define('ID_UNIVERSIDADE', 12);
    define('ID_HOSPITAL', 16);
    define('ID_ESCOLAS_TECNICAS', 11);
    define('ID_ESCOLAS_AGROTECNICAS', 14);
    define('ID_ADM', 34);

    // ids das unidades
    define("ID_UNIDADEIMPLANTADORA", 44);
    define("ID_CAMPUS", 18);
    define("ID_UNED", 17);
    define("ID_SUPERVISIONADA", 35);
    define("ID_REITORIA", 75);
    define("ID_UNIESTADUAL", 42);

    // IDs Abas
    define("ID_ABA_LISTA_EMP", 57594);
    define("ID_ABA_CADASTRA_EMP", 57597);
    define("ID_ABA_EMP_CADASTRADO", 57598);
    define("ID_ABA_CADASTRA_OBRA", 57599);
    define("ID_ABA_OBRA_CADASTRADA_FNDE", 57603);
    define("ID_ABA_CUMPRIMENTO_OBJETO", 57896);
    define("ID_ABA_OBRA_CADASTRADA", 57602);
    define("ID_ABA_CADASTRA_OBRA_EMP", 57601);
    define("ID_ABA_DETALHAMENTO_ITEM_OBRA", 57605);
    define("ID_ABA_PREVISAO_DETALHAMENTO_OBRA", 57606);
    define("ID_ABA_CADASTRO_VISTORIA_EMP", 57630);
    define("ID_ABA_CADASTRO_VISTORIA_EMP_EDICAO", 57631);
    define("ID_ABA_CADASTRO_VISTORIA_EMPRESA", 57632);
    define("ID_ABA_CADASTRO_VISTORIA_EMPRESA_EDICAO", 57633);
    define("ID_ABA_CADASTRO_VISTORIA", 57609);
    define("ID_ABA_OS_MI", 57790);
    define("ID_ABA_CADASTRO_VISTORIA_FNDE", 57610); // Empreendimento junto da obra
    
    define("ID_ABA_MONITORAMENTO_ESPECIAL", 57837);
    
    // IDs Menus do sistema
    define('ID_MENU_LICITACAO', 12803);
    define('ID_MENU_CONTRATACAO', 12804);
    define('ID_MENU_VISTORIA', 12779);
    define('ID_MENU_CRONOGRAMA', 12736);

    // IDs tipo fase licitaчуo ( obras2.tiposfaseslicitacao ) 
    define('FASE_LIC_PUBLICACAO_EDITAL', 2);
    define('FASE_LIC_RECURSO_INTERPOSTO', 5);
    define('FASE_LIC_ORDEM_SERVICO', 6);
    define('FASE_LIC_ABERTURA_PROPOSTA', 7);
    define('FASE_LIC_HOMOLOGACAO', 9);

    // Workflow Obras
//	define("TPDID_OBRA",97);
//	
//	define("ESD_CADASTRAMENTO",623);
//		define("AEDID_OBJ_CADAST_EXECUCAO",	1755);
//			
//	define("ESD_EXECUCAO",624);
//	define("ESD_PARALIZADA",625);
//	define("ESD_CANCELADA",626);
//	define("ESD_CONCLUЭDA",627);
    //ID fundi Obras
    define("ID_RESPONSAVELOBRAS", 45);

    // IDs Workflow Demanda
    define("TPDID_DEMANDA", 98);

    define("ESDID_CADASTRAMENTO", 632);
    define("ESDID_APROVACAO", 633);
    define("ESDID_CONTRATACAO", 634);
    define("ESDID_CONCLUIDO", 635);
    define("ESDID_CANCELADO", 636);

    // IDs Workflow Vistoria
    define("TPDID_VISTORIA", 101);
    define("ESDID_VISTORIA_CADASTRAMENTO", 653);

    // IDs Workflow supervisуo MI
    define("TPDID_SUPERVISAO_MI", 128);
    define("ESDID_MI_CADASTRAMENTO", 837);
    define("ESDID_MI_VALIDACAO"    , 1042);
    define("ESDID_MI_CONCLUIDO"    , 1043);
    define("ESDID_MI_CORRECAO"     , 1044);
    define("AEDID_MI_ENVIAR_VALIDACAO", 2448);
    define("AEDID_MI_ENVIAR_VALIDACAO_POS_CORRECAO", 2451);
    
//    define("ESDID_MI_AGUARDANDO_HOMOLOGACAO", 838);
//    define("ESDID_MI_HOMOLOGADO", 839);

    // IDs Workflow Laudo / Supervisao (EMPRESA)
    define("WF_TPDID_LAUDO_SUPERVISAO_EMPRESA", 116);
    define("WF_ESDID_LAUDO_SUPERVISAO_EM_CADASTRAMENTO", 732);
    define("WF_ESDID_LAUDO_SUPERVISAO_AGUARDANDO_HOMOLOGACAO", 733);
    define("WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO", 734);
    define("WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO", 756);
    define("WF_ESDID_LAUDO_SUPERVISAO_PAGO", 757);
    define("WF_ESDID_LAUDO_SUPERVISAO_CANCELADO", 1188);

    //IDs Aчуo Estado WorkFkow Laudo / Supervisao (EMPRESA)
    define("WF_AEDID_LAUDO_SUPERVISAO_HOMOLOGAR", 1726);
    define("WF_AEDID_LAUDO_SUPERVISAO_SOLICITAR_PAGAMENTO", 1776);
    define("WF_AEDID_LAUDO_SUPERVISAO_REALIZAR_PAGAMENTO", 1777);

    // IDs Workflow Pagamento Supervisуo
    define("WF_TPDID_PAGAMENTO_SUPERVISAO", 120);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_EM_CADASTRAMENTO", 758);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_ENVIADO_FNDE", 759);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_PAGAMENTO_SOLICITADO", 760);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_PAGO", 761);

    define("ESDID_VISTORIA_EMP_CADASTRAMENTO", 732);
    define("ESDID_VISTORIA_EMP_ANALISE_GESTOR", 733);
    define("ESDID_VISTORIA_EMP_LAUDO", 734);


    // IDs Workflow O.S
    define("TPDID_OS", 115);
    define("ESDID_OS_CADASTRAMENTO", 728);
    define("ESDID_OS_CANCELADA", 729);
    define("ESDID_OS_EXECUCAO", 730);
    define("ESDID_OS_CONCLUIDA", 731);
    define("ESDID_OS_ENVIADA_EMPRESA", 765);

//	// IDs workflow Empreendimento
//	define("TPDID_EMPREENDIMENTO", 102);	
//	define("ESDID_EMP_CADASTRAMENTO",	656);	
//	define("ESDID_EMP_DIVISAO",			657);	
//	define("ESDID_EMP_LICITACAO",		658);	
//	define("ESDID_EMP_CONTRATACAO",		659);	
//	define("ESDID_EMP_EXECUCAO",		660);
//		define("AEDID_EMP_EXEC_PARALISAR",	1742);	
//	define("ESDID_EMP_PARALISACAO",		661);	
//	define("ESDID_EMP_CANCELADA",		662);	
//	define("ESDID_EMP_CONCLUIDA",		663);	
    // IDs workflow Obra (OBJETO)
    define("TPDID_OBJETO", 105);
    define("ESDID_OBJ_REPASSE", 762);
    define("AEDID_OBJ_LICITACAO_LICITACAO", 2896); // Registra obra vinculada
    define("ESDID_OBJ_PLANEJAMENTO_PROPONENTE", 689);
    define("AEDID_OBJ_PLANEJAMENTO_CANCELADO", 1861);
    define("ESDID_OBJ_LICITACAO", 763);
    define("ESDID_OBJ_AGUARDANDO_1_REPASSE", 762);
    define("ESDID_OBJ_CONTRATACAO", 764);
    define("AEDID_OBJ_CONTRATACAO_EXECUCAO", 1785);
    define("ESDID_OBJ_EXECUCAO", 690);
    define("AEDID_OBJ_EXECUCAO_PARALISADO", 1757);
    define("AEDID_OBJ_EXECUCAO_CONCLUIDO", 1758);
    define("AEDID_OBJ_EXECUCAO_ADITIVO", 1756);
    define("ESDID_OBJ_PARALISADO", 691);
    define("AEDID_OBJ_PARALISADO_EXECUCAO", 1759);
    define("ESDID_OBJ_CONCLUIDO", 693);
    define("ESDID_OBJ_INACABADA", 1084);
    define("AEDID_OBJ_CONCLUIDO_EXECUCAO", 1964); // produчуo 1964);
    define("AEDID_OBJ_CONCLUIDO_PARALISADO", 1965); // produчуo 1965);
    define("ESDID_OBJ_ADITIVO", 692);
    define("ESDID_OBJ_CONTRATO_CANCELADO", 766);
    define("ESDID_OBJ_CONVENIO_CANCELADO", 767);
    define("ESDID_OBJ_REFORMULACAO", 768);
    define("ESDID_OBJ_CANCELADO", 769);
    define("ESDID_OBJ_CONCLUIDA", 770);
    define("ESDID_OBJ_REGISTRO_PRECO", 771);
    define("ESDID_OBJ_AGUARDANDO_EMISSAO_OS", 864);
    define("ESDID_OBJ_AGUARDANDO_EMISSAO_OS_COM_PENDENCIA", 872);
    define("ESDID_OBJ_AGUARDANDO_ACEITE_OS", 873);
    define("AEDID_OBJ_ACEITE_RECUSADA", 1973);
    define("AEDID_OBJ_ACEITE_EXECUCAO", 1972);
    define("ESDID_OBJ_OS_RECUSADA", 874);

    // IDs workflow Obra MI (OBJETO)
    define("TPDID_OBRAMI", 126);
    define("ESDID_OBJMI_ADESAO_MUNICIPIO", 774);
    define("ESDID_OBJMI_AUTORIZACAO_FNDE", 816);
    define("ESDID_OBJMI_ANUENCIA_FORNECEDOR", 817);
    define("ESDID_OBJMI_TERRAPLANAGEM", 818);
    define("ESDID_OBJMI_ASSINATURA_CONTRATO", 819);
    define("ESDID_OBJMI_EMISSAO_OS", 820);
    define("ESDID_OBJMI_PARALISADA", 821);
    define("ESDID_OBJMI_EXECUCAO", 822);
    define("ESDID_OBJMI_CONCLUIDA", 823);

//	define("TPDID_OBJETO", 105);
//	define("ESDID_OBJ_CADASTRAMENTO",	689);	
//		define("AEDID_OBJ_CADAST_EXECUCAO",	1755);
//	define("ESDID_OBJ_EXECUCAO",		690);
//		define("AEDID_OBJ_EXEC_PARALISAR",	1757);
//	define("ESDID_OBJ_PARALISACAO",		691);	
//	define("ESDID_OBJ_CANCELADA",		692);	
//	define("ESDID_OBJ_CONCLUIDA",		693);	
    // Tipos de Arquivo
    define('TIPO_ARQUIVO_FOTO_VISTORIA', 23);

    // Tipo de Aditivo
    define('TIPO_ADV_PRAZO', 1);
    define('TIPO_ADV_VALOR', 2);
    define('TIPO_ADV_PRAZOVALOR', 3);

    // MacroEtapa (macroitemcomposiчуo)
    define('MACROETAPA_SERVICO_PRELIMINARES', 1);
    define('MACROETAPA_INFRAESTRUTURA', 2);
    define('MACROETAPA_SUPERESTRUTURA', 3);
    define('MACROETAPA_SERVICO_COMPLEMENTARES', 4);

    //Aчуo estado documento do Workflow O.S.
    define('AEDID_WF_EXECUCAO', 1722); // Falta pegar o ID certo no banco de treinamento ------------------------------------------------------
    // Tipo OS MI obras2.tipoosmi
    define('TOMID_EXECUACAO', 1);
    define('TOMID_SONDAGEM', 2);
    define('TOMID_PROJETO', 3);

    // Tecnologia MI obras2.tecnologiami
    define('TMIID_PVC_CONCRETO', 1);
    define('TMIID_PLACA_CIMENTICIA', 2);
    define('TMIID_MATERIAIS_COMPOSITOS', 3);
    
    /**
     * Questionсrios do Checklist da Validaчуo da Obra
     */
    define('QUEID_QUEST_CHKLST_2P' ,      95);
    define('QUEID_QUEST_CHKLST_ADM',      96);
    define('QUEID_QUEST_CHKLST_ADM_SP', 109);
    define('QUEID_QUEST_CHKLST_ADM_2015', 116);
    define('QUEID_QUEST_CHKLST_TEC',      98);
    define('QUEID_QUEST_CHKLST_TEC_2015',      117);
//    define('QUEID_QUEST_CHKLST_TEC', 94);
    define('QUEID_QUEST_CHKLST_OBR_VINC', 99);
    define('QUEID_QUEST_CHKLST_SOLICITACOES', 120);
    define('QUEID_QUEST_CHKLST_CUMPRIMENTO', 121);
    define('QUEID_QUEST_CHKLST_OBR_MI', 107);
    //Workflow do Checklist da validaчуo
    define('TPID_CHECKLIST_VALIDACAO', 176);
    //Workflow Estados do Checklist da validaчуo
    define("ESDID_CHKLST_CADASTRAMENTO", 1088);
    define("ESDID_CHKLST_CONCLUIDO"    , 1089);
    define("ESDID_CHKLST_CORRECAO"     , 1090);
    
    // IDs Workflow Restriчуo/Inconformidade
    define("TPDID_RESTRICAO_INCONFORMIDADE", 186);

    define("ESDID_AGUARDANDO_PROVIDENCIA", 1140);
    define("ESDID_AGUARDANDO_ANALISE_FNDE", 1141);
    define("ESDID_SUPERADA", 1142);
    define("ESDID_CANCELADA", 1143);
    define("ESDID_AGUARDANDO_CORRECAO", 1144);
    define("ESDID_JUSTIFICADA", 1503);

    define("AEDID_CANCELAR",                             2653);
    define("AEDID_ENCAMINHAR_PARA_ANALISE",              2650);
    define("AEDID_CONFIRMAR_SUPERACAO",                  2651);
    define("AEDID_DEVOLVER_PARA_CORRECAO",               2652);
    define("AEDID_RETORNAR_PARA_ANALISE",                2654);
    define("AEDID_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA", 2655);
    define("AEDID_ENVIAR_PARA_ANALISE_FNDE",             2656);
    
    //Workflow de Atividade de Monitoramento Especial
    define('TPID_ME_ATIVIDADE_MONITORAMENTO', 196);
    //Workflow Estados do Monitoramento Especial
    define("ESDID_ME_ATIVIDADE_CADASTRAMENTO", 1237);
    define("ESDID_ME_ATIVIDADE_ANALISE"      , 1238);
    define("ESDID_ME_ATIVIDADE_CORRECAO"     , 1246);
    define("ESDID_ME_ATIVIDADE_CONCLUIDO"    , 1239);    
    //Workflow das Aчѕes da Atividade de Monitoramento Especial
    define("AEDID_ME_ATIVIDADE_ENVIAR_PARA_ANALISE"              ,2833);
    define("AEDID_ME_ATIVIDADE_ENVIAR_PARA_CORRECAO"             ,2834);
    define("AEDID_ME_ATIVIDADE_CONCLUIR_ANALISE"                 ,2835);
    define("AEDID_ME_ATIVIDADE_ENVIAR_DA_CORRECAO_PARA_ANALISE"  ,2836);
    
    //Workflow de Itens de Monitoramento Especial
    define('TPID_ME_ITEM_MONITORAMENTO', 197);
    //Workflow Estados do Monitoramento Especial
    define("ESDID_ME_ITEM_CADASTRAMENTO", 1247);
    define("ESDID_ME_ITEM_ANALISE"      , 1248);
    define("ESDID_ME_ITEM_CORRECAO"     , 1249);
    define("ESDID_ME_ITEM_CONCLUIDO"    , 1250);    
    //Workflow das Aчѕes dos Itens de Monitoramento Especial
    define("AEDID_ME_ITEM_ENVIAR_PARA_ANALISE"              ,2840);
    define("AEDID_ME_ITEM_ENVIAR_PARA_CORRECAO"             ,2841);
    define("AEDID_ME_ITEM_CONCLUIR_ANALISE"                 ,2842);
    define("AEDID_ME_ITEM_ENVIAR_DA_CORRECAO_PARA_ANALISE"  ,2843);
    
         // IDs Workflow Cumprimento do objeto
    define("TPDID_CUMPRIMENTO_OBJETO", 200);
    define("ESDID_CUMPRIMENTO_CADASTRAMENTO" , 1266);
    define("ESDID_CUMPRIMENTO_DILIGENCIADO" , 1267);
    define("ESDID_CUMPRIMENTO_VALIDACAO_FNDE" , 1268);
    #define("ESDID_CUMPRIMENTO_APROVADO_TOTALMENTE" , 1269); DESABILITADO
    #define("ESDID_CUMPRIMENTO_REPROVADO_TOTALMENTE" , 1621); DESABILITADO
    #define("ESDID_CUMPRIMENTO_APROVADO_PARCIALMENTE" , 1622); DESABILITADO
    define("ESDID_CUMPRIMENTO_AGUARDANDO_VALIDACAO_PROCESSO" , 1623);
    define("ESDID_CUMPRIMENTO_AGUARDANDO_DEFERIMENTO" , 1624);
    define("ESDID_CUMPRIMENTO_AGUARDANDO_APROVACAO" , 1625);
    define("ESDID_CUMPRIMENTO_APROVADO" , 1626);

    // Situaчуo Obra
    define("STRID_OBJ_PLANEJAMENTO_PELO_PROPONENTE"  ,1);
    define("STRID_OBJ_LICITACAO"  ,2);
    define("STRID_OBJ_CONSTRACAO"  ,3);
    define("STRID_OBJ_EXECUCAO"  ,4);
    define("STRID_OBJ_PARALISADO"  ,5);
    define("STRID_OBJ_CONCLUIDO"  ,6);
    define("STRID_OBJ_INACABADO"  ,7);


    define("TPDID_SOLICITACAO_VINCULADA", 218);
    define("ESDID_AGUARDANDO_ANALISE" , 1427);
    define("ESDID_DEFERIDO" , 1428);
    define("ESDID_INDEFERIDO" , 1429);

    // IDs Workflow Solicitacao Desebolso
    define("TPDID_SOLICITACAO_DESEMBOLSO", 236);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO" , 1598);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI" , 1597);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL" , 1575);
    define("ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO" , 1576);
    define("ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO" , 1577);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA" , 1581);

    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_DEFERIDO", 3693);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO", 3805);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO", 3721);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO", 3692);

    define("AEDID_SOLICITACAO_DESEMBOLSO_CORRECAO_PARA_ANALISE", 3807);


    // IDs Workflow Solicitaчѕes
    define("TPDID_SOLICITACOES", 235);
    define("ESDID_SOLICITACOES_CADASTRAMENTO", 1570);
    define("ESDID_SOLICITACOES_AGUARDANDO_ANALISE", 1571);
    define("ESDID_SOLICITACOES_DEFERIDO", 1572);
    define("ESDID_SOLICITACOES_INDEFERIDO", 1573);
    define("ESDID_SOLICITACOES_DILIGENCIA", 1574);
    define("ESDID_SOLICITACOES_RETORNADO", 1592);

    define("AEDID_SOLICITACOES_CADASTRAMENTO_PARA_ANALISE", 3670);
    define("AEDID_SOLICITACOES_ANALISE_PARA_DEFERIDO", 3671);
    define("AEDID_SOLICITACOES_ANALISE_PARA_INDEFERIDO", 3672);
    define("AEDID_SOLICITACOES_ANALISE_PARA_DILIGENCIA", 3673);
    define("AEDID_SOLICITACOES_DILIGENCIA_PARA_DEFERIDO", 3674);
    define("AEDID_SOLICITACOES_DILIGENCIA_PARA_ANALISE", 3690);

} 
else {

    // Pefil
    define("PFLCOD_SUPER_USUARIO", 932); //ok
    define("PFLCOD_ADMINISTRADOR", 941); //ok
    define("PFLCOD_CADASTRADOR_INSTITUCIONAL", 942); //ok
    define("PFLCOD_SAA", 943); //ok
    define("PFLCOD_GESTOR_MEC", 944); //ok
    define("PFLCOD_SUPERVISOR_MEC", 945); //ok
    define("PFLCOD_GESTOR_UNIDADE", 946); //ok
    define("PFLCOD_CONSULTA_GERAL", 947); //ok
    define("PFLCOD_SUPERVISOR_UNIDADE", 948); //ok
    define("PFLCOD_AUDITOR_INTERNO", 949); //ok
    define("PFLCOD_CONSULTA_ESTADUAL", 950); //ok
    define("PFLCOD_CONSULTA_TIPO_DE_ENSINO", 951); //ok
    define("PFLCOD_CONSULTA_UNIDADE", 952); //ok
    define("PFLCOD_EMPRESA_CONTRATADA", 953); //ok
    define("PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC", 909);
    define("PFLCOD_EMPRESA_VISTORIADORA_GESTOR", 910);
    define("PFLCOD_EMPRESA_VISTORIADORA_FISCAL", 911);
    define("PFLCOD_EMPRESA_MI_GESTOR", 1008);
    define("PFLCOD_EMPRESA_MI_FISCAL", 1036);
    define("PFLCOD_EMPRESA_MI_ADMINISTRATIVO", 1299);
    define("PFLCOD_CALL_CENTER", 1023);
//	define("PFLCOD_CONSULTA_ESTADUAL",				1015);
    define("PFLCOD_ALERTA_MI_INTERNO", 1066);

    define("PFLCOD_CGIMP_GESTOR", 1276);
    define("PFLCOD_CGIMP_TECNICO", 1278);

    // (obras2.tipologiaobra)
//104;"MI - Escola de Educaчуo Infantil Tipo B"
    define("TPOID_MI_TIPO_B", 104);
//105;"MI - Escola de Educaчуo Infantil Tipo C"
    define("TPOID_MI_TIPO_C", 105);

    // (obras2.etapaquestao)
    define("ETQID_SUPERVISAO", 1);
    define("ETQID_VINCULADA", 9);
    define("ETQID_DESEMBOLSO", 10);

    // (obras2.situacaoobra)
    define("SOBID_EXECUCAO", 1);
    define("SOBID_DESACORDO_EXECUCAO", 2);
    define("SOBID_CONCLUIDA", 3);
    define("SOBID_PARALISADA", 4);

    // situaчуo atividade (obras2.situacaoatividade)
    define("STAID_EXECUCAO", 1);
    define("STAID_PARALISADO", 2);
    define("STAID_CONCLUIDO", 3);
    define("STAID_ELABORACAO_PROJETO", 4);
    define("STAID_LICITACAO", 5);
    define("STAID_CONTRATO_CANCELADO", 6);
    define("STAID_CONVENIO_CANCELADO", 7);
    define("STAID_ANALISE_CAIXA", 8);
    define("STAID_REFORMULACAO", 9);
    define("STAID_OBRA_CANCELADA", 10);
    define("STAID_ETAPA_CONCLUIDA", 11);
    define("STAID_REGISTRO_PRECO", 12);
    define("STAID_PREPARATORIA", 99);

    // tipo de situaчуo do registro
    define("STRID_PLANEJAMENTO_PROPONENTE", 1);
    define("STRID_LICITACAO", 2);
    define("STRID_CONTRATACAO", 3);
    define("STRID_EXECUCAO", 4);
    define("STRID_PARALISADA", 5);
    define("STRID_CONCLUIDA", 6);
    define("STRID_INACABADA", 7);
    define("STRID_CANCELADA", 8);

    // tipo de paralisacao (obras2.tipoparalisacao)
    define("TPLID_EMBARGO", 1);
    define("TPLID_ABANDONO_EMPRESA", 2);
    define("TPLID_CONTRATO_RESCINDIDO", 3);
    define("TPLID_OUTROS", 4);
    define("TPLID_OS_PARALISACAO", 5);

    // tipo de detalhamento da execuчуo financeira (obras2.itensexecucaoorcamentaria)
    define("TIPO_DETALHAMENTO_EXECUCAO_EMPENHADO", 'E');
    define("TIPO_DETALHAMENTO_EXECUCAO_LIQUIDADO", 'L');

    // tipo execuчуo orчamentсria (obras2.tipoexecorcamentaria)
    define("TIPO_EXEC_ORCAMENTARIA_OBRA", 1);
    define("TIPO_EXEC_ORCAMENTARIA_EQUIPAMENTO", 2);

    // forma repasse recurso (obras2.tipoformarepasserecursos)
    define("FRPID_CONVENIO", 2);
    define("FRPID_DESCENTRALIZACAO", 3);
    define("FRPID_RECURSO_PROPRIO", 4);
    define("FRPID_TESOURO_UO", 6);

    // Tipo foto obras2.tipoarquivo
//	define("TIPO_OBRA_ARQUIVO_FOTO",		'F');
//	define("TIPO_OBRA_ARQUIVO_DOCUMENTO",	'D');
    define("TIPO_OBRA_ARQUIVO_FOTO_VISTORIA", 23);
    define("TIPO_OBRA_ARQUIVO_HOMOLOGACAO", 24);
    define("TIPO_OBRA_ARQUIVO_OUTROS", 21);

    // Situaчуo da obra obras2.situacaoobra
    define("SITUACAO_OBRA_EM_EXECUCAO", 1);
    define("SITUACAO_OBRA_CONCLUIDA", 3);
    define("SITUACAO_OBRA_PARALISADA", 4);

    // tipo endereчo entidade.endereco
    define("TIPO_ENDERECO_OBRA", 3);
    define("TIPO_ENDERECO_OBJETO", 4); // Estс sendo usado tanto para EMPREENDIMENTO (obra) como para OBRA (objeto)
    // гrgуos do mѓdulo de obras
    define("ORGID_EDUCACAO_SUPERIOR", 1);
    define("ORGID_EDUCACAO_PROFISSIONAL", 2);
    define("ORGID_EDUCACAO_BASICA", 3);
    define("ORGID_ADMINISTRATIVO", 4);
    define("ORGID_HOSPITAIS", 5);

    // Constantes das funчѕes das entidades
    define('ID_UNIVERSIDADE', 12);
    define('ID_HOSPITAL', 16);
    define('ID_ESCOLAS_TECNICAS', 11);
    define('ID_ESCOLAS_AGROTECNICAS', 14);
    define('ID_ADM', 34);

    // ids das unidades
    define("ID_UNIDADEIMPLANTADORA", 44);
    define("ID_CAMPUS", 18);
    define("ID_UNED", 17);
    define("ID_SUPERVISIONADA", 35);
    define("ID_REITORIA", 75);
    define("ID_UNIESTADUAL", 42);

    // IDs Abas
    define("ID_ABA_LISTA_EMP", 57644); //ok
    define("ID_ABA_CADASTRA_EMP", 57647); //ok
    define("ID_ABA_EMP_CADASTRADO", 57648); //ok
    define("ID_ABA_CADASTRA_OBRA", 57649); //ok
    define("ID_ABA_OBRA_CADASTRADA_FNDE", 57653); //ok
    define("ID_ABA_CUMPRIMENTO_OBJETO", 57896);
    define("ID_ABA_OBRA_CADASTRADA", 57652); //ok
    define("ID_ABA_CADASTRA_OBRA_EMP", 57651); //ok
    define("ID_ABA_DETALHAMENTO_ITEM_OBRA", 57655); //ok
    define("ID_ABA_PREVISAO_DETALHAMENTO_OBRA", 57656); //ok
    define("ID_ABA_CADASTRO_VISTORIA_EMP", 57680); //ok
    define("ID_ABA_CADASTRO_VISTORIA_EMP_EDICAO", 57681); //ok
    define("ID_ABA_CADASTRO_VISTORIA_EMPRESA", 57682); //ok
    define("ID_ABA_CADASTRO_VISTORIA_EMPRESA_EDICAO", 57683); //ok
    define("ID_ABA_CADASTRO_VISTORIA", 57659); //ok
    define("ID_ABA_OS_MI", 57790); //ok
    define("ID_ABA_CADASTRO_VISTORIA_FNDE", 57660); //ok // Empreendimento junto da obra

    define("ID_ABA_MONITORAMENTO_ESPECIAL", 57837);

    // IDs Menus do sistema
    define('ID_MENU_LICITACAO', 12803);
    define('ID_MENU_CONTRATACAO', 12804);
    define('ID_MENU_VISTORIA', 12779);
    define('ID_MENU_CRONOGRAMA', 12736);

    // IDs tipo fase licitaчуo ( obras2.tiposfaseslicitacao ) 
    define('FASE_LIC_PUBLICACAO_EDITAL', 2);
    define('FASE_LIC_RECURSO_INTERPOSTO', 5);
    define('FASE_LIC_ORDEM_SERVICO', 6);
    define('FASE_LIC_ABERTURA_PROPOSTA', 7);
    define('FASE_LIC_HOMOLOGACAO', 9);

    // Workflow Obras (OBJETO)
//	define("TPDID_OBRA", 105);
//	
//	define("ESD_CADASTRAMENTO", 689);
//		define("AEDID_OBJ_CADAST_EXECUCAO",	1755);
//	
//	define("ESD_EXECUCAO",		690);
//	define("ESD_PARALIZADA",	691);
//	define("ESD_CANCELADA",		692);
//	define("ESD_CONCLUЭDA",		693);
    //ID fundi Obras
    define("ID_RESPONSAVELOBRAS", 45);

    // IDs Workflow Demanda
    define("TPDID_DEMANDA", 98);

    define("ESDID_CADASTRAMENTO", 744);
    define("ESDID_APROVACAO", 745);
    define("ESDID_CONTRATACAO", 746);
    define("ESDID_CONCLUIDO", 747);
    define("ESDID_CANCELADO", 748);

    // IDs Workflow Vistoria
    define("TPDID_VISTORIA", 118);
    define("ESDID_VISTORIA_CADASTRAMENTO", 743);

    // IDs Workflow supervisуo MI
    define("TPDID_SUPERVISAO_MI", 128);
    define("ESDID_MI_CADASTRAMENTO", 837);
    define("ESDID_MI_VALIDACAO", 1042);
    define("ESDID_MI_CONCLUIDO", 1043);
    define("ESDID_MI_CORRECAO", 1044);
    define("AEDID_MI_ENVIAR_VALIDACAO", 2448);
    define("AEDID_MI_ENVIAR_VALIDACAO_POS_CORRECAO", 2451);

//    define("ESDID_MI_AGUARDANDO_HOMOLOGACAO", 838);
//    define("ESDID_MI_HOMOLOGADO", 839);

    // IDs Workflow Laudo / Supervisao (EMPRESA)
    define("WF_TPDID_LAUDO_SUPERVISAO_EMPRESA", 116);
    define("WF_ESDID_LAUDO_SUPERVISAO_EM_CADASTRAMENTO", 732);
    define("WF_ESDID_LAUDO_SUPERVISAO_AGUARDANDO_HOMOLOGACAO", 733);
    define("WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO", 734);
    define("WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO", 756);
    define("WF_ESDID_LAUDO_SUPERVISAO_PAGO", 757);
    define("WF_ESDID_LAUDO_SUPERVISAO_CANCELADO", 1188);

    //IDs Aчуo Estado WorkFkow Laudo / Supervisao (EMPRESA)
    define("WF_AEDID_LAUDO_SUPERVISAO_HOMOLOGAR", 1726);
    define("WF_AEDID_LAUDO_SUPERVISAO_SOLICITAR_PAGAMENTO", 1776);
    define("WF_AEDID_LAUDO_SUPERVISAO_REALIZAR_PAGAMENTO", 1777);

    //IDs Aчуo Estado 'Concluido' WorkFkow OS
    define("AEDID_OS_CONCLUIDA", 1723);

    // IDs Workflow Pagamento Supervisуo
    define("WF_TPDID_PAGAMENTO_SUPERVISAO", 120);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_EM_CADASTRAMENTO", 758);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_ENVIADO_FNDE", 759);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_PAGAMENTO_SOLICITADO", 760);
    define("WF_ESDID_PAGAMENTO_SUPERVISAO_PAGO", 761);

    define("ESDID_VISTORIA_EMP_CADASTRAMENTO", 732);
    define("ESDID_VISTORIA_EMP_ANALISE_GESTOR", 733);
    define("ESDID_VISTORIA_EMP_LAUDO", 734);

    // IDs Workflow O.S
    define("TPDID_OS", 115);
    define("ESDID_OS_CADASTRAMENTO", 728);
    define("ESDID_OS_CANCELADA", 729);
    define("ESDID_OS_EXECUCAO", 730);
    define("ESDID_OS_CONCLUIDA", 731);
    define("ESDID_OS_ENVIADA_EMPRESA", 765);

    // IDs Workflow O.S
    define("TPDID_OS_MI", 142);
    define("ESDID_OS_MI_CADASTRAMENTO", 903);
    define("ESDID_OS_MI_AGUARDANDO_ACEITE", 904);
    define("ESDID_OS_MI_EXECUCAO", 905);
    define("ESDID_OS_MI_VALIDACAO", 906);
    define("ESDID_OS_MI_CORRECAO", 907);
    define("ESDID_OS_MI_CONCLUIDA", 908);
    define("ESDID_OS_MI_CANCELADA", 909);
    define("ESDID_OS_MI_REPROVADA", 910);

    // IDs Workflow O.S MI
//	// IDs workflow Empreendimento (OBRA)
//	define("TPDID_EMPREENDIMENTO",		117);
//	define("ESDID_EMP_CADASTRAMENTO",	735);	
//	define("ESDID_EMP_DIVISAO",			736);	
//	define("ESDID_EMP_LICITACAO",		737);	
//	define("ESDID_EMP_CONTRATACAO",		738);
//		define("AEDID_EMP_CONTRAT_EXECUCAO",1737);	
//	define("ESDID_EMP_EXECUCAO",		739);
//		define("AEDID_EMP_EXEC_PARALISAR",	1742);	
//	define("ESDID_EMP_PARALISACAO",		740);	
//	define("ESDID_EMP_CANCELADA",		741);	
//	define("ESDID_EMP_CONCLUIDA",		742);	
    // IDs workflow Obra (OBJETO)
    define("TPDID_OBJETO", 105);
    define("ESDID_OBJ_REPASSE", 762);
    define("AEDID_OBJ_LICITACAO_LICITACAO", 2896); // Registra obra vinculada
    define("ESDID_OBJ_PLANEJAMENTO_PROPONENTE", 689);
    define("AEDID_OBJ_PLANEJAMENTO_CANCELADO", 1861);
    define("ESDID_OBJ_LICITACAO", 763);
    define("ESDID_OBJ_AGUARDANDO_1_REPASSE", 762);
    define("ESDID_OBJ_CONTRATACAO", 764);
    define("AEDID_OBJ_CONTRATACAO_EXECUCAO", 1785);
    define("ESDID_OBJ_EXECUCAO", 690);
    define("AEDID_OBJ_EXECUCAO_PARALISADO", 1757);
    define("AEDID_OBJ_EXECUCAO_CONCLUIDO", 1758);
    define("AEDID_OBJ_EXECUCAO_ADITIVO", 1756);
    define("ESDID_OBJ_PARALISADO", 691);
    define("AEDID_OBJ_PARALISADO_EXECUCAO", 1759);
    define("ESDID_OBJ_CONCLUIDO", 693);
    define("ESDID_OBJ_INACABADA", 1084);
    define("AEDID_OBJ_CONCLUIDO_EXECUCAO", 1964); // produчуo 1964);
    define("AEDID_OBJ_CONCLUIDO_PARALISADO", 1965); // produчуo 1965);
    define("AEDID_INACABADA_PARA_CONCLUIDA", 3865); // produчуo 3865
    define("ESDID_OBJ_ADITIVO", 692);
    define("ESDID_OBJ_CONTRATO_CANCELADO", 766);
    define("ESDID_OBJ_CONVENIO_CANCELADO", 767);
    define("ESDID_OBJ_REFORMULACAO", 768);
    define("ESDID_OBJ_CANCELADO", 769);
    define("ESDID_OBJ_CONCLUIDA", 770);
    define("ESDID_OBJ_REGISTRO_PRECO", 771);
    define("ESDID_OBJ_AGUARDANDO_EMISSAO_OS", 864);
    define("ESDID_OBJ_AGUARDANDO_EMISSAO_OS_COM_PENDENCIA", 872);
    define("AEDID_OBJ_AGUARDANDO_ACEITE", 1971);
    define("ESDID_OBJ_AGUARDANDO_ACEITE_OS", 873);
    define("AEDID_OBJ_ACEITE_RECUSADA", 1973);
    define("AEDID_OBJ_ACEITE_EXECUCAO", 1972);
    define("ESDID_OBJ_OS_RECUSADA", 874);

    // IDs workflow Obra MI (OBJETO)
    define("TPDID_OBRAMI", 126);
    define("ESDID_OBJMI_ADESAO_MUNICIPIO", 774);
    define("ESDID_OBJMI_AUTORIZACAO_FNDE", 816);
    define("ESDID_OBJMI_ANUENCIA_FORNECEDOR", 817);
    define("ESDID_OBJMI_TERRAPLANAGEM", 818);
    define("ESDID_OBJMI_ASSINATURA_CONTRATO", 819);
    define("ESDID_OBJMI_EMISSAO_OS", 820);
    define("ESDID_OBJMI_PARALISADA", 821);
    define("ESDID_OBJMI_EXECUCAO", 822);
    define("ESDID_OBJMI_CONCLUIDA", 823);

//	define("ESDID_OBJ_CADASTRAMENTO",	689);	
//		define("AEDID_OBJ_CADAST_EXECUCAO",	1755);
//	define("ESDID_OBJ_EXECUCAO",		690);
//		define("AEDID_OBJ_EXEC_PARALISAR",	1757);
//	define("ESDID_OBJ_PARALISACAO",		691);	
//	define("ESDID_OBJ_CANCELADA",		692);	
//	define("ESDID_OBJ_CONCLUIDA",		693);	
    // Tipos de Arquivo
    define('TIPO_ARQUIVO_FOTO_VISTORIA', 23);

    // Tipo de Aditivo
    define('TIPO_ADV_PRAZO', 1);
    define('TIPO_ADV_VALOR', 2);
    define('TIPO_ADV_PRAZOVALOR', 3);

    // MacroEtapa (macroitemcomposiчуo)
    define('MACROETAPA_SERVICO_PRELIMINARES', 1);
    define('MACROETAPA_INFRAESTRUTURA', 2);
    define('MACROETAPA_SUPERESTRUTURA', 3);
    define('MACROETAPA_SERVICO_COMPLEMENTARES', 4);

    //Aчуo estado documento do Workflow O.S.
    define('AEDID_WF_EXECUCAO', 1722);

    // Tipo OS MI obras2.tipoosmi
    define('TOMID_EXECUACAO', 1);
    define('TOMID_SONDAGEM', 2);
    define('TOMID_PROJETO', 3);

    // Tecnologia MI obras2.tecnologiami
    define('TMIID_PVC_CONCRETO', 1);
    define('TMIID_PLACA_CIMENTICIA', 2);
    define('TMIID_MATERIAIS_COMPOSITOS', 3);


    /**
     * Questionсrios do Checklist da Validaчуo da Obra
     */
    define('QUEID_QUEST_CHKLST_2P', 95);
    define('QUEID_QUEST_CHKLST_ADM', 96);
    define('QUEID_QUEST_CHKLST_ADM_SP', 109);
    define('QUEID_QUEST_CHKLST_ADM_2015', 116);
    define('QUEID_QUEST_CHKLST_TEC', 98);
    define('QUEID_QUEST_CHKLST_TEC_2015',      117);
    //define('QUEID_QUEST_CHKLST_TEC', 94);
    define('QUEID_QUEST_CHKLST_OBR_VINC', 99);

    define('QUEID_QUEST_CHKLST_SOLICITACOES', 120);
    define('QUEID_QUEST_CHKLST_CUMPRIMENTO', 121);

    define('QUEID_DOCUMENTOS_INSERIDOS', 101);
    define('QUEID_DADOS_LICITACAO', 102);
    define('QUEID_NOVA_LICITACAO', 103);
    define('QUEID_REFORMULACOES_IMPLEMENTADAS', 104);
    define('QUEID_REFORMULACAO_RECURSO', 105);
    define('QUEID_DOCUMENTOS_FINAIS', 106);
    define('QUEID_QUEST_CHKLST_OBR_MI', 107);


    //Workflow do Checklist da validaчуo
    define('TPID_CHECKLIST_VALIDACAO', 176);
    //Workflow Estados do Checklist da validaчуo
    define("ESDID_CHKLST_CADASTRAMENTO", 1088);
    define("ESDID_CHKLST_CONCLUIDO", 1089);
    define("ESDID_CHKLST_CORRECAO", 1090);


    // IDs Workflow Restriчуo/Inconformidade
    define("TPDID_RESTRICAO_INCONFORMIDADE", 186);

    define("ESDID_AGUARDANDO_PROVIDENCIA", 1140);
    define("ESDID_AGUARDANDO_ANALISE_FNDE", 1141);
    define("ESDID_SUPERADA", 1142);
    define("ESDID_CANCELADA", 1143);
    define("ESDID_AGUARDANDO_CORRECAO", 1144);
    define("ESDID_JUSTIFICADA", 1503);

    define("AEDID_CANCELAR", 2653);
    define("AEDID_ENCAMINHAR_PARA_ANALISE", 2650);
    define("AEDID_CONFIRMAR_SUPERACAO", 2651);
    define("AEDID_DEVOLVER_PARA_CORRECAO", 2652);
    define("AEDID_RETORNAR_PARA_ANALISE", 2654);
    define("AEDID_RETORNAR_PARA_AGUARDANDO_PROVIDENCIA", 2655);
    define("AEDID_ENVIAR_PARA_ANALISE_FNDE", 2656);

    //Workflow de Atividade de Monitoramento Especial
    define('TPID_ME_ATIVIDADE_MONITORAMENTO', 196);
    //Workflow Estados do Monitoramento Especial
    define("ESDID_ME_ATIVIDADE_CADASTRAMENTO", 1237);
    define("ESDID_ME_ATIVIDADE_ANALISE", 1238);
    define("ESDID_ME_ATIVIDADE_CORRECAO", 1246);
    define("ESDID_ME_ATIVIDADE_CONCLUIDO", 1239);
    //Workflow das Aчѕes da Atividade de Monitoramento Especial
    define("AEDID_ME_ATIVIDADE_ENVIAR_PARA_ANALISE", 2833);
    define("AEDID_ME_ATIVIDADE_ENVIAR_PARA_CORRECAO", 2834);
    define("AEDID_ME_ATIVIDADE_CONCLUIR_ANALISE", 2835);
    define("AEDID_ME_ATIVIDADE_ENVIAR_DA_CORRECAO_PARA_ANALISE", 2836);

    //Workflow de Itens de Monitoramento Especial
    define('TPID_ME_ITEM_MONITORAMENTO', 197);
    //Workflow Estados do Monitoramento Especial
    define("ESDID_ME_ITEM_CADASTRAMENTO", 1247);
    define("ESDID_ME_ITEM_ANALISE", 1248);
    define("ESDID_ME_ITEM_CORRECAO", 1249);
    define("ESDID_ME_ITEM_CONCLUIDO", 1250);
    //Workflow das Aчѕes dos Itens de Monitoramento Especial
    define("AEDID_ME_ITEM_ENVIAR_PARA_ANALISE", 2840);
    define("AEDID_ME_ITEM_ENVIAR_PARA_CORRECAO", 2841);
    define("AEDID_ME_ITEM_CONCLUIR_ANALISE", 2842);
    define("AEDID_ME_ITEM_ENVIAR_DA_CORRECAO_PARA_ANALISE", 2843);

    // IDs Workflow Cumprimento do objeto
    define("TPDID_CUMPRIMENTO_OBJETO", 200);
    define("ESDID_CUMPRIMENTO_CADASTRAMENTO" , 1266);
    define("ESDID_CUMPRIMENTO_DILIGENCIADO" , 1267);
    define("ESDID_CUMPRIMENTO_VALIDACAO_FNDE" , 1268);
    #define("ESDID_CUMPRIMENTO_APROVADO_TOTALMENTE" , 1269); DESABILITADO
    #define("ESDID_CUMPRIMENTO_REPROVADO_TOTALMENTE" , 1621); DESABILITADO
    #define("ESDID_CUMPRIMENTO_APROVADO_PARCIALMENTE" , 1622); DESABILITADO
    define("ESDID_CUMPRIMENTO_AGUARDANDO_VALIDACAO_PROCESSO" , 1623);
    define("ESDID_CUMPRIMENTO_AGUARDANDO_DEFERIMENTO" , 1624);
    define("ESDID_CUMPRIMENTO_AGUARDANDO_APROVACAO" , 1625);
    define("ESDID_CUMPRIMENTO_APROVADO" , 1626);

    // Situaчуo Obra
    define("STRID_OBJ_PLANEJAMENTO_PELO_PROPONENTE", 1);
    define("STRID_OBJ_LICITACAO", 2);
    define("STRID_OBJ_CONSTRACAO", 3);
    define("STRID_OBJ_EXECUCAO", 4);
    define("STRID_OBJ_PARALISADO", 5);
    define("STRID_OBJ_CONCLUIDO", 6);
    define("STRID_OBJ_INACABADO", 7);

    define("TPDID_SOLICITACAO_VINCULADA", 218);
    define("ESDID_AGUARDANDO_ANALISE" , 1427);
    define("ESDID_DEFERIDO" , 1428);
    define("ESDID_INDEFERIDO" , 1429);

    // IDs Workflow Solicitacao Desebolso
    define("TPDID_SOLICITACAO_DESEMBOLSO", 236);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO" , 1598);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI" , 1597);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL" , 1575);
    define("ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO" , 1576);
    define("ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO" , 1577);
    define("ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA" , 1581);

    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_DEFERIDO", 3693);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO", 3805);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO", 3721);
    define("AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO", 3692);

    define("AEDID_SOLICITACAO_DESEMBOLSO_CORRECAO_PARA_ANALISE", 3807);

    // IDs Workflow Solicitaчѕes
    define("TPDID_SOLICITACOES", 235);
    define("ESDID_SOLICITACOES_CADASTRAMENTO", 1570);
    define("ESDID_SOLICITACOES_AGUARDANDO_ANALISE", 1571);
    define("ESDID_SOLICITACOES_DEFERIDO", 1572);
    define("ESDID_SOLICITACOES_INDEFERIDO", 1573);
    define("ESDID_SOLICITACOES_DILIGENCIA", 1574);
    define("ESDID_SOLICITACOES_RETORNADO", 1592);

    define("AEDID_SOLICITACOES_CADASTRAMENTO_PARA_ANALISE", 3670);
    define("AEDID_SOLICITACOES_ANALISE_PARA_DEFERIDO", 3671);
    define("AEDID_SOLICITACOES_ANALISE_PARA_INDEFERIDO", 3672);
    define("AEDID_SOLICITACOES_ANALISE_PARA_DILIGENCIA", 3673);
    define("AEDID_SOLICITACOES_DILIGENCIA_PARA_DEFERIDO", 3674);
    define("AEDID_SOLICITACOES_DILIGENCIA_PARA_ANALISE", 3690);
}
?>