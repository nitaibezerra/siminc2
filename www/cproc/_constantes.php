<?PHP

if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){

    // SISTEMA
    define("CPROC_SISID", 186);

    // WORKFLOW
    define("CPROC_GESTAODOCUMENTOSDISUP_TPDID", 155);
    
    // ESTADO DOCUMENTO - TODO: copiar para outra baselogin
    //define("CPROC_EMCADASTRAMENTO_ESDID", 1000);
    define("CPROC_EMCADASTRAMENTO_ESDID", 1311);
    //define("CPROC_ANALISEPROCEDIMENTOCOORDENADOR_ESDID", 1001);
    define("CPROC_ANALISEPROCEDIMENTOCOORDENADOR_ESDID", 1308);
    //define("CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID", 1007);
    define("CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID", 1305);
    //define("CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID", 1004);
    define("CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID", 1306);
    define("CPROC_AGUARDARMANIFESTACAOINTERESSADO_ESDID", 1313);
    define("CPROC_ARQUIVARPROCEDIMENTO_ESDID", 1309);
    
    // ABAS DO SISTEMA.
    define ("ABA_CAD_PROCESSO", 57794);

    // PERFIS DE USU�RIOS.
    define ("PERFIL_SUPER_USUARIO", 1122);
    define ("PERFIL_APOIO_CPROC", 1143);
    define ("PERFIL_COORDENADOR_CPROC", 1141);
    define ("PERFIL_TECNICO_CPROC", 1142);

}else{

    // SISTEMA
    define("CPROC_SISID", 186);

    // WORKFLOW
    //define("CPROC_GESTAODOCUMENTOSDISUP_TPDID", 155);
    define("CPROC_GESTAODOCUMENTOSDISUP_TPDID", 206);
    
    // WORKFLOW
    define("CPROC_EMCADASTRAMENTO_ESDID", 1311);
    define("CPROC_ANALISEPROCEDIMENTOCOORDENADOR_ESDID", 1308);
    define("CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID", 1305);
    define("CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID", 1306);
    define("CPROC_AGUARDARMANIFESTACAOINTERESSADO_ESDID", 1313);
    define("CPROC_ARQUIVARPROCEDIMENTO_ESDID", 1309);
    
    // PERFIS

    #ABAS DO SISTEMA.
    //define ("ABA_CAD_PROCESSO", 57794);
    define ("ABA_CAD_PROCESSO", 57848);
    

    // PERFIS DE USU�RIOS.
    //define ("PERFIL_SUPER_USUARIO", 1122);
    define ("PERFIL_SUPER_USUARIO", 1306);
    //define ("PERFIL_APOIO_CPROC", 1143);
    define ("PERFIL_APOIO_CPROC", 1310);
    define ("PERFIL_COORDENADOR_CPROC", 1308);
    define ("PERFIL_TECNICO_CPROC", 1309);
    
    // Tipo de verifica��o in loco, que determina o t�tulo do campo vlrnumprocemec
    define ("VRL_REGULACAO_INEP", 1);
    define ("VRL_SUPERVISAO_INEP", 2);
    define ("VRL_SUPERVISAO_INTERNA", 3);
}

?>