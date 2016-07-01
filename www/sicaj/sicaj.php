<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
/**
 * Autoload de classes dos Módulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$perfis = pegaPerfilGeral();
/* Controle de exibição do de Simular Usuário */
if (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_CGO, $perfis)) {
    $exibirSimular = true;
}

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
