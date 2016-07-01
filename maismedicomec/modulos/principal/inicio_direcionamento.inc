<?PHP
    #MATA AS SESSÕES DE INDENTIFICAÇÃO DO QUESTIONARIO, MUNICÍPÍO E DEMAIS SESSÕES USADAS NO SISTEMA.
    unset( $_SESSION['maismedicomec'] );

    $perfil = pegaPerfilGeral($_SESSION['usucpf']);

    if( !(in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ) ){

        if( in_array(PERFIL_INST_AVAL_AVALIADOR_MEC, $perfil) || in_array(PERFIL_INST_AVAL_ANALISTA_MEC, $perfil) || in_array(PERFIL_CONSULTA, $perfil)  ){
            $link_aval = "maismedicomec.php?modulo=principal/instrumentoavaliacao/lista_grid_municipio&acao=A&qstid=".QUEST_INSTRUMENTO_AVAL_CURSO_MEDICINA;
            $link_mant = "maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A";
        }else{
            $link_aval = "";
            $link_mant = "";
        }
        
            if( in_array(PERFIL_HOSPITAL, $perfil)) {
                $link_mant = "maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A";                
            }
            
            if( in_array(PERFIL_IES, $perfil)) {
                $link_mant = "maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A";                
            }

            if( in_array(PERFIL_MM_MEC_REVISOR, $perfil)) {
                $link_mant = "maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A";
            }

    }else{
        $link_aval = "maismedicomec.php?modulo=principal/instrumentoavaliacao/lista_grid_municipio&acao=A&qstid=".QUEST_INSTRUMENTO_AVAL_CURSO_MEDICINA;
        $link_mant = "maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A";
    }

    include  APPRAIZ."includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Instrumento de Avaliação <i>IN LOCO</i>' );
?>

<link type="text/css" rel="stylesheet" href="../maismedicomec/css/caixas_pagina_principal.css" media="screen">

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../maismedicomec/javascript/caixas_pagina_principal.js"></script>

<script type="text/javascript">

    $( document ).ready(function() {
        definrCoresCaixas();
    });

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="height:465px;">
    <tr>
        <td width="50%" style="vertical-align: top;">

            <?php
            if( 
                
                (in_array(PERFIL_HOSPITAL, $perfil) != true && in_array(PERFIL_IES, $perfil) != true && in_array(PERFIL_MM_MEC_REVISOR, $perfil) != true) ) { ?>

                 <div class="divCaixaPrincipal" id="divCaixa_1">
                    <div class="divCaixaTitulo"> INSTRUMENTO DE AVALIAÇÃO IN LOCO </div>

                    <div class="btnNormal btnOn" id="btnCaixa_1" data-request="<?=$link_aval;?>">
                        <img class="iconeTituloSecundario" width="22" src="../imagens/mm_avaliacao/Avaliacao_32x32.png"/>
                        <label class="labelTiluloSecundario">Instrumento de Avaliação IN LOCO</label>
                    </div>
                </div>
            <?php } ?>

        </td>
        <!-- Hospital, IES, superususario, administrador -->
         <?php if(in_array(PERFIL_IES, $perfil) === true || in_array(PERFIL_HOSPITAL, $perfil) === true || in_array(PERFIL_SUPER_USUARIO, $perfil) === true || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) === true || in_array(PERFIL_MM_MEC_REVISOR, $perfil) === true || in_array(PERFIL_CONSULTA, $perfil) === true){?>
        <td width="50%" style="vertical-align: top;">
             <div class="divCaixaPrincipal" id="divCaixa_2">
                <span class="divCaixaTitulo"> CADASTRAMENTO DA MANTENEDORA </span>
                <div class="btnNormal btnOn" id="btnCaixa_2" data-request="<?=$link_mant;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/mm_avaliacao/cadastrar_32x32.png"/>
                    <label class="labelTiluloSecundario">Cadastramento da Mantenedora</label>
                </div>
            </div>
        </td>
         <?php }?>
    </tr>
</table>