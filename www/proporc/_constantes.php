<?php
/**
 * Constantes do sistema
 * $Id: _constantes.php 97332 2015-05-14 19:03:59Z lindalbertofilho $
 */

/**
 * Identifica o nome do sistema. Utilizado para armazenar dados na sessão.
 */
define('MODULO', $_SESSION['sisdiretorio']);

/**
 * Código do tipo de documento usado no workflow da proposta orçamentária.
 */
define('TPDOC_PROPOSTA_ORCAMENTARIA', 188);

/**
 * Código do tipo de documento usado no workflow da proposta orçamentária / prelimites pessoal.
 */
define('TPDOC_PRELIMITES_PESSOAL', 224);

/**
 * Estado da proposta assim que ela é criada e ainda não foi tramitado.
 */
define('ESDOC_EM_PREENCHIMENTO', 1195);
/**
 * Estado assumido pela documentação qdo a proposta foi enviado para análise SPO.
 */
define('ESDOC_ANALISE_SPO', 1196);
/**
 * Estado assumido qdo a proposta precisa de correções.
 */
define('ESDOC_ACERTOS_UO', 1197);
/**
 * Estado assumido qdo a proposta foi enviado para a SOF através do webservice.
 */
define('ESDOC_ENVIADO_SOF', 1198);

/**
 * Perfis utilizado pelas UOs
 */
define('PFL_UO_EQUIPE_TECNICA', 1222);
/**
 * Perfis utilizados pelas UGs da UO 26101
 */
define('PFL_AD_EQUIPE_TECNICA', 1230);
/**
 * Perfil utilizado internamente no mec.
 */
define('PFL_CGO_EQUIPE_ORCAMENTARIA', 1221);
/**
 * Perfil de superusuário
 */
define('PFL_ADMINISTRADOR', 1213);
/**
 * Transição de Análise SPO para Envio à SOF.
 */
define('AESDID_SPO_PARA_SIOP', 2732);
/**
 * Id do Módulo.
 */
define('SISID', 191);
/**
 * Estado do Workflow Pré-Limites: Em Preenchimento.
 */
define('ESTADO_PRELIMITE_EM_PREENCHIMENTO', 1500);
/**
 * Estado do Workflow Pré-Limites: Análise SPO.
 */
define('ESTADO_PRELIMITE_ANALISE_SPO', 1501);
/**
 * Estado do Workflow Pré-Limites: Ajustes SPO.
 */
define('ESTADO_PRELIMITE_AJUSTES_UO', 1502);
/**
 * Estado do Workflow Pré-Limites: Concluído.
 */
define('ESTADO_PRELIMITE_CONCLUIDO', 1503);
