<?PHP
    unset($_SESSION['cpfavaliado']);
    
    $perfil = pegaPerfilGeral();
        
    if( in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) || in_array( PERFIL_AVAL_EQUIPE_APOIO, $perfil) ){

        header('Location: gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_servidores_avaliacao&acao=A');  

    }elseif( in_array( PERFIL_AVAL_SERV_AVALIACAO, $perfil ) || in_array( PERFIL_AVAL_SERV_SERVIDOR, $perfil )){

        header('Location: gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_pess_avaliados&acao=A');  

    }else{
        header('Location: gestaopessoa.php?modulo=inicio&acao=C');  
    }
        
?>
    