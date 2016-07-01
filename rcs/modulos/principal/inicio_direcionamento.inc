<?PHP
    $perfil = pegaPerfilGeral($_SESSION['usucpf']);

    if( !(in_array(PERFIL_RCS_SUPER_USUARIO, $perfil) || in_array(PERFIL_RCS_ADMINISTRADOR, $perfil) || in_array(PERFIL_RCS_ADM_INSTITUCIONAL, $perfil) ) ){

        if( in_array(PERFIL_RCS_PROFESSOR, $perfil) ){
            $link_ins = "rcs.php?modulo=principal/saberes/cad_inscricao_prof&acao=A&acUsProf=S";                    
            $link_sor = "";
        }
        
        if( in_array(PERFIL_RCS_INTERLOCUTOR, $perfil) ){
            $link_ins = "rcs.php?modulo=principal/saberes/lista_grid_isncricao_prof&acao=A";
            $link_sor = "rcs.php?modulo=principal/sorteio_avaliadores/cad_sorteio_avaliadores&acao=A";
        }
        
        
        #PERFIL ACUMULADO.
        if( in_array(PERFIL_RCS_INTERLOCUTOR, $perfil) && in_array(PERFIL_RCS_PROFESSOR, $perfil) ){
            $link_ins = "rcs.php?modulo=principal/saberes/lista_grid_isncricao_prof&acao=A";                    
            $link_sor = "rcs.php?modulo=principal/sorteio_avaliadores/cad_sorteio_avaliadores&acao=A";
        }

    }else{
        $link_ins = "rcs.php?modulo=principal/saberes/lista_grid_isncricao_prof&acao=A";
        $link_sor = "rcs.php?modulo=principal/sorteio_avaliadores/cad_sorteio_avaliadores&acao=A";
    }

    include  APPRAIZ."includes/cabecalho.inc";
    monta_titulo( 'Reconhecimento de Saberes e Competências', 'Processo Seletivo para Comissão Avaliadora' );
?>

<link type="text/css" rel="stylesheet" href="../rcs/css/caixas_pagina_principal.css" media="screen">

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../rcs/javascript/caixas_pagina_principal.js"></script>

<script type="text/javascript">

    $( document ).ready(function() {
        definrCoresCaixas();
    });

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="height:465px;">
    <tr>
        <td width="50%" style="vertical-align: top;">
             <div class="divCaixaPrincipal" id="divCaixa_1">
                <div class="divCaixaTitulo"> PROCESSO SELETIVO - INSCRIÇÃO AVALIADORES </div>
                <div class="btnNormal btnOn" id="btnCaixa_1" data-request="<?=$link_ins;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/rcs/cadastrar_32x32.png"/>
                    <label class="labelTiluloSecundario"> Processo Seletivo - Inscrição Avaliadores </label>
                </div>
            </div>
        </td>
        <td width="50%" style="vertical-align: top;">
             <div class="divCaixaPrincipal" id="divCaixa_2">
                <span class="divCaixaTitulo"> SORTEIO DE AVALIADORES </span>
                <div class="btnNormal btnOn" id="btnCaixa_2" data-request="<?=$link_sor;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/rcs/avaliacao_32x32.png"/>
                    <label class="labelTiluloSecundario"> Sorteio de Avaliadores</label>
                </div>
            </div>
        </td>
    </tr>
</table>