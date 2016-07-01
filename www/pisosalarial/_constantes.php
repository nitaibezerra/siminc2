<?php
// Diretorio do SAP
define('APPPISO', APPRAIZ . 'pisosalarial/');

define('TPDID_PISOSALARIAL', 43);

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' || $_SESSION['baselogin'] == 'simec_desenvolvimento_old' ){

    /*
     * DESENVOLVIMENTO
     */

    // Perfis
    define('PERFIL_ADMINISTRADOR',      544);
    define('PERFIL_CONSULTA_GERAL',     547);
    define('PERFIL_CADASTRO_MUNICIPAL', 545);
    define('PERFIL_CONSULTA_MUNICIPAL', 546);

    // Workflow
    define('WF_EM_CADASTRAMENTO',       264);
    define('WF_EM_ANALISE_MEC',         265);
    define('WF_APROVADO',               266);

    // Abas
    define('ABA_FP_DUPLICAR_MES',       7971);
    define('ABA_FP_IMPORTAR_ARQUIVO',   7972);
    define('ABA_FP_LISTAR_FOLHAS',      7970);
    define('ABA_FP_LISTAR_PROFIS',      7973);
    define('ABA_FP_FORM_PROFISSIONAL',  7975);

}else{

    /*
     * PRODUวรO
     */

    // Perfis
    define('PERFIL_ADMINISTRADOR',      574);
    define('PERFIL_CONSULTA_GERAL',     577);
    define('PERFIL_CADASTRO_MUNICIPAL', 575);
    define('PERFIL_CONSULTA_MUNICIPAL', 576);

    // Workflow
    define('WF_EM_CADASTRAMENTO',       341);
    define('WF_EM_ANALISE_MEC',         342);
    define('WF_APROVADO',               343);

    // Abas
    define('ABA_FP_DUPLICAR_MES',       8071);
    define('ABA_FP_IMPORTAR_ARQUIVO',   8072);
    define('ABA_FP_LISTAR_FOLHAS',      8070);
    define('ABA_FP_LISTAR_PROFIS',      8073);
    define('ABA_FP_FORM_PROFISSIONAL',  8075);

}

// Tabelas analise piso, parametros
define('TB_ANALISE_AP',             1);
define('TB_ANALISE_APAD',           2);
define('TB_ANALISE_APAE',           3);
define('TB_ANALISE_PARM_PAFR',      4);
define('TB_ANALISE_PMDE',           5);
define('TB_ANALISE_IFLEI',          6);

// Tipos cargos piso salarial
define('PISO_CARGO_PROFESSOR',      1);
define('PISO_CARGO_APOIO_DOCENCIA', 2);
define('PISO_CARGO_APOIO_ESCOLAR',  3);

// Cargos Profissional Folha Pagamento
define('CARGO_OUTROS_PROFESSOR',    142);
define('CARGO_OUTROS_DOCENCIA',     143);
define('CARGO_OUTROS_APOIO',        144);

?>