<?php

if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
    // Perfil
    define("CONTRATO_PERFIL_SUPER_USUARIO", 1233);
    define("CONTRATO_PERFIL_ADMINISTRADOR", 1234);
    define("CONTRATO_PERFIL_GESTOR_CONTRATO", 1235);
    define("CONTRATO_PERFIL_EXECUTOR", 1236);
    define("CONTRATO_PERFIL_VALIDADOR", 1237);
    define("CONTRATO_PERFIL_CERTIFICADOR", 1238);
    define("CONTRATO_PERFIL_CONSULTA", 1239);

//WORKFLOW
    define("WF_CONTRATO_GESTAO", 195);

    define("WF_CONTRATO_GESTAO_EXECUTACAO", 1229);
    define("WF_CONTRATO_GESTAO_VALIDACAO", 1230);
    define("WF_CONTRATO_GESTAO_CERTIFICACAO", 1231);
    define("WF_CONTRATO_GESTAO_FINALIZADO", 1232);

    define("WF_ACAO_EXECUTACAO_FINALIZADO", 2257);
    define("WF_ACAO_EXECUTACAO_EXECUTADO", 2256);

    define("WF_ACAO_VALIDACAO_FINALIZADO", 2260);
    define("WF_ACAO_VALIDACAO_INVALIDADO", 2259);
    define("WF_ACAO_VALIDACAO_VALIDADO", 2258);

    define("WF_ACAO_CERTIFICACAO_EXECUCAO_NAO_CERTIFICADO", 2263);
    define("WF_ACAO_CERTIFICACAO_VALIDACAO_NAO_CERTIFICADO", 2262);
    define("WF_ACAO_CERTIFICACAO_CERTIFICADO", 2261);


//FUN��O
    define("FUNCAO_EXECUTOR", 120);
    define("FUNCAO_VALIDADOR", 121);
    define("FUNCAO_CERTIFICADOR", 122);

//SISTEMA
    define("ID_SISTEMA", 194);
    define("MODULO", 'contratogestao');
} else {
    // Perfil
    define("CONTRATO_PERFIL_SUPER_USUARIO", 1252);
    define("CONTRATO_PERFIL_ADMINISTRADOR", 1253);
    define("CONTRATO_PERFIL_GESTOR_CONTRATO", 1255);
    define("CONTRATO_PERFIL_EXECUTOR", 1254);
    define("CONTRATO_PERFIL_VALIDADOR", 1256);
    define("CONTRATO_PERFIL_CERTIFICADOR", 1257);
    define("CONTRATO_PERFIL_CONSULTA", 1258);

    //WORKFLOW
    define("WF_CONTRATO_GESTAO", 195);
    define("WF_CONTRATO_GESTAO_EXECUTACAO", 1234);
    define("WF_CONTRATO_GESTAO_VALIDACAO", 1233);
    define("WF_CONTRATO_GESTAO_CERTIFICACAO", 1232);
    define("WF_CONTRATO_GESTAO_FINALIZADO", 1231);

    define("WF_ACAO_EXECUTACAO_FINALIZADO", 2813);
    define("WF_ACAO_EXECUTACAO_EXECUTADO", 2812);

    define("WF_ACAO_VALIDACAO_FINALIZADO", 2816);
    define("WF_ACAO_VALIDACAO_INVALIDADO", 2814);
    define("WF_ACAO_VALIDACAO_VALIDADO", 2815);

    define("WF_ACAO_CERTIFICACAO_EXECUCAO_NAO_CERTIFICADO", 2817);
    define("WF_ACAO_CERTIFICACAO_VALIDACAO_NAO_CERTIFICADO", 2818);
    define("WF_ACAO_CERTIFICACAO_CERTIFICADO", 2819);

    //FUN��O
    define("FUNCAO_EXECUTOR", 120);
    define("FUNCAO_VALIDADOR", 121);
    define("FUNCAO_CERTIFICADOR", 122);

    //SISTEMA
    define("ID_SISTEMA", 197);
    define("MODULO", 'contratogestao');
}
?>