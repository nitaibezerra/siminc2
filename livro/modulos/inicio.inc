<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */
    include  APPRAIZ."includes/cabecalho.inc";

    /*** Recupera os perfis do usuário ***/
    $sql = "
        SELECT  edtid,
                colid,
                comid,
                ediid,
                cedid
        FROM livro.usuarioresponsabilidade

        WHERE usucpf = '{$_SESSION['usucpf']}'AND rpustatus = 'A'
    ";
    $rsRpu = $db->carregar($sql);
    $rsRpu = $rsRpu ? $rsRpu : array();

    unset($_SESSION['pnld']);

    foreach($rsRpu as $rpu){
        //editora
        if($rpu['edtid']){
                $_SESSION['pnld']['edtid'][] = $rpu['edtid'];
        }
        if($rpu['colid']){
                $_SESSION['pnld']['colid'][] = $rpu['colid'];
        }
        if($rpu['comid']){
                $_SESSION['pnld']['comid'][] = $rpu['comid'];
        }

        //editora eja
        if($rpu['ediid']){
            $_SESSION['pnld']['ediid'][] = $rpu['ediid'];
        }
        //editora CAMPO
        if($rpu['cedid']){
            $_SESSION['pnld']['cedid'][] = $rpu['cedid'];
        }
    }

    $perfis = arrayPerfil();

    if( !$db->testa_superuser() ){
        if( in_array( PNLD_EDITORA_EJA, $perfis ) || in_array( PNLD_PERFIL_GESTOR_EJA, $perfis ) ){
            header( "location: livro.php?modulo=principal/pnld_editora_eja&acao=A" );
        }elseif( in_array(PNLD_PERFIL_GESTOR, $perfis) || in_array(PNLD_PERFIL_TECNICO, $perfis) || in_array(PNLD_PERFIL_COORDENADOR, $perfis) || in_array(PNLD_PERFIL_PARECERISTA, $perfis) || in_array(PNLD_PERFIL_COMISSAO_TECNICA, $perfis) ){
            header( "location: livro.php?modulo=principal/pnld_editora&acao=A" );
        }elseif( in_array(PNLD_PERFIL_GESTOR_PNIAC, $perfis) || in_array(PNLD_PERFIL_EDITORA_PNIAC, $perfis) ){
            header( "location: livro.php?modulo=principal/pnld_editora_pnaic&acao=A" );
        }elseif( in_array(PNLD_PERFIL_GESTOR_CAMPO, $perfis) || in_array(PNLD_PERFIL_EDITORA_CAMPO, $perfis) ){
            header( "location: livro.php?modulo=principal/pnld_editora_campo&acao=A" );
        }elseif( in_array( PNLD_EDITORA, $perfis ) ){
            header( "location: livro.php?modulo=principal/pnld_editora_analise&acao=A" );
        }
    }else{
        header( "location: livro.php?modulo=principal/pnld_editora&acao=A" );
    }

?>