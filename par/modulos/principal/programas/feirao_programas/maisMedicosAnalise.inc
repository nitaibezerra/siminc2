<?php 
    include_once '_funcoes_maismedicos.php';

    if($_REQUEST['requisicao'] == 'salvarTermo'){
            salvarAdesao($_POST);
            exit();    
    }
    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';
    echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A');
    monta_titulo($titulo_modulo, recuperaMunicipioEstado());
    $_SESSION['maismedicos'] = true;

    $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

    $aba = !$_GET['aba'] ? "Parecer" : $_GET['aba'];

    $abaAtiva = "par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=$aba";

    if( in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ){
        $menu = array( 
            0 => array("id" => 1, "descricao" => "Parecer", "link" => "par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=Parecer"),
            1 => array("id" => 2, "descricao" => "Ficha de Avaliação", "link" => "par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=Ficha")
        );
    }else{
        $menu = array( 
            0 => array("id" => 1, "descricao" => "Parecer", "link" => "par.php?modulo=principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=Parecer")
        );
    }
    
    echo "<br/>"; 
    echo montarAbasArray($menu, $abaAtiva);	

    include_once "maisMedicos".$aba.".inc";
?>
