<?php

if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
    // PERFIL
    define("SOLUCAO_PERFIL_SUPER_USUARIO", 1302);
    define("SOLUCAO_PERFIL_ADMINISTRADOR", 1303);
    define("SOLUCAO_PERFIL_CONSULTA", 1305);
    define("SOLUCAO_PERFIL_EXECUTOR", 1304);

    //WORKFLOW
    define("WF_PTO", 205);

    //WORKFLOW - ACAO
    define("WF_ESTADO_EM_EXECUCAO", 1301);
    define("WF_ESTADO_FINALIZADO", 1300);

    //SISTEMA
    define("ID_SISTEMA", 201);
    define("MODULO", 'pto');
} else {
    // PERFIL
    define("SOLUCAO_PERFIL_SUPER_USUARIO", 1302);
    define("SOLUCAO_PERFIL_ADMINISTRADOR", 1303);
    define("SOLUCAO_PERFIL_CONSULTA", 1305);
    define("SOLUCAO_PERFIL_EXECUTOR", 1304);

    //WORKFLOW
    define("WF_PTO", 205);

    //WORKFLOW - ACAO
    define("WF_ESTADO_EM_EXECUCAO", 1301);
    define("WF_ESTADO_FINALIZADO", 1300);

    //SISTEMA
    define("ID_SISTEMA", 201);
    define("MODULO", 'pto');
}
?>