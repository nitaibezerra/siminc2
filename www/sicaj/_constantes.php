<?php
// --Equipe responsável
define('EMAIL_SIMEC_ANALISTA', $_SESSION['email_sistema']);
define('EMAIL_SIMEC_DESENVOLVEDOR', $_SESSION['email_sistema']);
define('EMAIL_SIMEC_DESENVOLVEDOR_2', $_SESSION['email_sistema']);

// -- tipo de documento workflow
define('WF_TPDID_SICAJ', 202);

// -- tipo de documento upload de arquivo
define('DECISAO_JUDICIAL', 'DJ');
define('PARECER_EXECUTORIO', 'PE');
define('PLANILHA_FINANCEIRA', 'PF');
define('OUTROS_DOCUMENTOS', 'OD');
define('HOMOLOGACAO_SICAJ', 'HS');
define('MENSAGENS_SEGEP', 'MS');

// -- estados do documento
define('NAO_ENVIADO', 1287);
define('ANALISE_SPO', 1288);
define('ACERTOS_UO', 1290);
define('HOMOLOGADO', 1291);
define('AJUSTES_UO', 1341);
define('PEDIDO_CANCELADO', 1338);
define('HOMOLOGACAO_REFEITA', 1342);
define('HOMOLOGACAO_ANULADA', 1343);
define('ANALISE_SOF', 1399);
define('ANALISE_COORDENACAO', 1496);

// -- perfis
define('PERFIL_UO_EQUIPE_TECNICA', 1301);
define('PERFIL_SUPER_USUARIO', 1298);
define('PERFIL_CGO', 1300);

/**
 * Identifica o nome do sistema. Utilizado para armazenar dados na sessão.
 */
define('MODULO', $_SESSION['sisdiretorio']);