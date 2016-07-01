<?PHP   
    $perfil = pegaPerfilGeral();
//ver($perfil, PERFIL_SUPER_USER);
     if( in_array(PERFIL_SUPER_USER, $perfil) || in_array(PERFIL_FT_ADMINISTRADOR_GERAL, $perfil) ){
         header("Location: gestaopessoa.php?modulo=principal/forca_trabalho/painel_forca_trabalho&acao=A");
         
     }elseif( in_array(PERFIL_SERVIDOR, $perfil) || in_array(PERFIL_TERCEIRIZADO, $perfil) ){
        unset($_SESSION['fdpcpf']);
        $sql = "select fdpcpf from gestaopessoa.ftdadopessoal where fdpcpf = '{$_SESSION['usucpf']}'";
        
        $fdpCpf = $db->pegaUm( $sql );
        
        if ($fdpCpf) {
            include_once( APPRAIZ . "gestaopessoa/classes/FtDadoPessoal.class.inc" );
            $ft = new FtDadoPessoal();
            $ft->carregarPorId("'" . $fdpCpf . "'");
            $sql = "SELECT * FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '" . $fdpCpf . "'";
            $dados = $db->carregar($sql);
            $_SESSION['fdpcpf'] = $fdpCpf;
        }
        header("Location: gestaopessoa.php?modulo=principal/cadDadosPessoais&acao=A");
        
    } 
    
    if (controlaPermissao('administrador') || controlaPermissao('consulta') || controlaPermissao('superuser')) {
        header('Location: gestaopessoa.php?modulo=principal/forca_trabalho/painel_forca_trabalho&acao=A');         
    }else{
        header('Location: gestaopessoa.php?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A');
    }
?>

<script>
    alert('Acesso negado. Usuário sem perfil cadastrado no sistema.'); 
    window.location.href = '?modulo=inicio&acao=C'; 
</script>
