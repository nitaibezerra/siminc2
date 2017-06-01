<?php
/**
 * Constantes do sistema planacomorc.
 * $Id: _constantes.php 98005 2015-05-29 20:02:53Z werteralmeida $
 */
define('MODULO', $_SESSION['sisdiretorio']);
define("SIS_NAME", "Planejamento e Acompanhamento Orçamentário");
define("APPRAIZ_SISOP", APPRAIZ."/planacomorc/modulos/principal/");

define("FLUXO_MONITORAMENTOACAO", 119);

define("PFL_ADMINISTRADOR", 954);
define('PFL_SUPERUSUARIO', 955);
define("PFL_PLANEJAMENTO", 956);
define("PFL_ASPAR", 957);
define("PFL_SUBUNIDADE", 994);

// -- Constantes utilizadas em: monitora/modulos/principal/planotrabalhoUN/popuphistoricoplanointernoUN.inc
define("PFL_CGSO", 1044);
define("PFL_GESTAO_ORCAMENTARIA", PFL_CGSO);
define("PFL_GESTAO_ORCAMENTARIA_IFS", 1207);
define('PFL_APOIO_GESTAO', 1063);
define('PFL_GABINETE', PFL_APOIO_GESTAO);
define('PFL_GESTOR_TRANSACAO', 1007);
define('PFL_RELATORIO_TCU', 1284);

define("ESD_EMELABORACAO", 749);
define("ESD_EMVALIDACAO", 750);
define("ESD_EMAPROVACAO", 751);
define("ESD_ENVIADOSIOP", 753); // -- tah errado, não corrigir
define("ESD_FINALIZADO", 752);

//--constantes workflow Fluxo de monitoramento da subação ##select * from workflow.estadodocumento where tpdid = 151#
//Tipo de Documento WORKFLOW
define("WF_TPDID_PLANEJAMENTO_PI", 265);

define("ESD_PI_CADASTRAMENTO", 1769);
define("ESD_PI_AGUARDANDO_APROVACAO", 1770);
define("ESD_PI_APROVADO", 1771);
define("ESD_PI_CANCELADO", 1772);

define("PREFIX_MINISTERIO_EDUCACAO", 26);
/* Banco de dados do FINANCEIRO */
define("PARAM_DBLINK_FINANCEIRO","dbname=dbsimecfinanceiro hostaddr= user= password= port=");

// -- Unidades orçamentárias associadas ao MEC
define("AD", 26101); // -- Administração Direta
define("CAPES", 26291);
define("INEP", 26290);
define("FNDE", 26298);
define("EBSERH", 26443);
define("FIES", 74902);
define("SUPERVISAOMEC", 73107);

// -- E-mail de recebimento de notificações sobre
define('EMAIL_NOTIFICACAO_SUBACAO', $_SESSION['email_sistema']);

// -- Indica uma transação de criação de PI
define('TRANSACAO_CRIACAO_PI', 'C');
// -- Indica uma transação de remanejamento de PI
define('TRANSACAO_REMANEAMENTO_PI', 'R');

define('WF_TPDID_PLANACOMORC_SUBACAO', '151');

define('TPDID_RELATORIO_TCU', 203);
define('ESDID_TCU_EM_PREENCHIMENTO', 1292);
define('ESDID_TCU_ANALISE_SPO', 1293);
define('ESDID_TCU_ACERTOS_UO', 1294);
define('ESDID_TCU_CONCLUIDO', 1295);

define('PERIODO_ATUAL', 5);

define('ENQUADRAMENTO_FINALISTICO', 354);

// ESFERA DA AÇÃO
define( 'ESFERA_FEDERAL_BRASIL', 1 );
define( 'ESFERA_ESTADUAL_DISTRITO_FEDERAL', 2 );
define( 'ESFERA_MUNICIPAL', 3 );
define( 'ESFERA_EXTERIOR', 4 );

