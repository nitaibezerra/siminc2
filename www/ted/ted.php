<?php
//Carrega parametros iniciais do simec
include_once 'controleInicio.inc';

/**
 * Autoload de classes dos Módulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
include_once '_autoload.inc';

$perfis = pegaPerfilGeral();
/* Controle de exibição do de Simular Usuário */
/* 1285 - CGSO */
if (is_array($perfis) && (in_array(PERFIL_CGSO, $perfis) || in_array(PERFIL_UG_REPASSADORA, $perfis))) {
    $exibirSimular = true;
}

//Carrega as funções de controle de acesso
include_once 'controleAcesso.inc';

$_SESSION['sislayoutbootstrap'] = 't';

if (strpos($_SERVER['REQUEST_URI'], 'modulo=sistema')) : ?>
    <script src="/includes/funcoes.js" ></script>
<?php endif;