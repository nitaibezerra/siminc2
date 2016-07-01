<?php
/**
 * Constantes do sistema de Receita Orчamentсria.
 * $Id: _constantes.php 74942 2014-02-06 19:19:23Z maykelbraz $
 */

/**
 * Esquema do banco de dados usado pelo sistema.
 */
define('DB_ESQUEMA', 'recorc');

/**
 * Estados da documentaчуo
 */
define('TPDID_PROGORC_1', 171);
define('STDOC_NAO_ENVIADO', 1066);
define('STDOC_ANALISE_SPO', 1067);
define('STDOC_ACERTOS_SPO', 1072);
define('STDOC_ANALISE_SECRETARIA', 1068);
define('STDOC_ACERTOS_SECRETARIA', 1069);
define('STDOC_ATENDIDO', 1070);
define('STDOC_RECUSADO', 1071);
define('STDOC_JUNTA_ORCAMENTARIA', 1102);

define('AEDID_ANALISE_APROVADO', 2498);
define('AEDID_ANALISE_RECUSADO', 2499);
define('AEDID_NAO_ENVIADO_ANALISE', 2497);
define('AEDID_NAO_ENVIADO_ATENDIDO_CARGA', 2552);
define('AEDID_SECRETARIA_SPO', 2505);
define('AEDID_ACERTOS_UO', 2511);


// -- Transiчуo de documentos

// -- Perfis
define('PFL_SUPER_USUARIO', 1155);
define('PFL_CGO_EQUIPE_ORCAMENTARIA', 1162);
define('PFL_UO_EQUIPE_TECNICA', 1163);
define('PFL_SECRETARIA', 1164);
define('PFL_JUNTA_ORCAMENTARIA', 1165);

define('MODULO', $_SESSION['sisdiretorio']);




