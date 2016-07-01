<?php
/**
 * Constantes do sistema de Receita Orчamentсria.
 * $Id: _constantes.php 94417 2015-02-24 20:57:37Z lindalbertofilho $
 */

/**
 * Esquema do banco de dados usado pelo sistema.
 */
define('DB_ESQUEMA', 'recorc');

/**
 * Estados da documentaчуo
 */
define('TPDID_RECORC_1', 168);
define('STDOC_EM_PREENCHIMENTO', 1045);

define('STDOC_ANALISE_SPO', 1046);
define('STDOC_ACERTOS_UO', 1047);
define('STDOC_APROVADO', 1051);
define('STDOC_ENVIADO_SOF', 1048);
define('STDOC_APROVADO_SOF', 1049);
define('STDOC_REPROVADO_SOF', 1050);
define('STDOC_DE_ACORDO', 1278);
define('STDOC_ALTERADO', 1279);
// -- Transiчуo de documentos
define('AEDID_ENVIADO_APROVADO_SOF', 2460);
define('AEDID_ENVIADO_REPROVADO_SOF', 2459);
define('AEDID_APROVADO_SOF_REPROVADO_SOF', 2462);
define('AEDID_REPROVADO_SOF_APROVADO_SOF', 2463);
define('AEDID_APROVADO_SOF_ENVIADO_SOF', 2464);
define('AEDID_REPROVADO_SOF_ENVIADO_SOF', 2465);
define('AEDID_CONCORDAR_EM_PREENCHIMENTO', 2454);
define('AEDID_APROVAR_CAPTACAO', 2455);
define('AEDID_ENVIAR_ACERTOS', 2456);
define('AEDID_CONCORDAR_ACERTOS_UO', 2943);
define('AEDID_RETORNAR_PARA_ANALISE', 2458);
define('AEDID_RETORNAR_ANALISE_SPO', 2647);
define('AEDID_CADASTRO_SIOP', 2457);
define('AEDID_RETORNAR_SPO', 2670);
define('AEDID_ENVIAR_EM_PREENCHIMENTO', 2942);

define('AEDID_EM_PREENCHIMENTO_PARA_ALTERADO', 2948);
define('AEDID_EM_PREENCHIMENTO_PARA_DE_ACORDO', 2454);
define('AEDID_ALTERADO_PARA_DE_ACORDO', 2950);
define('AEDID_DE_ACORDO_PARA_ALTERADO', 2942);
define('AEDID_ACERTOS_UO_PARA_DE_ACORDO', 2943);

define('AEDID_ANALISE_SPO_PARA_CADASTRADO_SIOP', 2953);

// -- Perfis
define('PFL_SUPER_USUARIO', 1138);
define('PFL_CGO_EQUIPE_ORCAMENTARIA', 1139);
define('PFL_UO_EQUIPE_TECNICA', 1140);

/**
 * Identifica o nome do sistema. Utilizado para armazenar dados na sessуo.
 */
define('MODULO', $_SESSION['sisdiretorio']);