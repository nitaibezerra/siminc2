<?php
    $perfil = pegaPerfilGeral( $_SESSION['usucpf'] );
    
    $link_aval = "";
    $link_forc = "";
    $link_cess = "";

    if( !( in_array(PERFIL_SUPER_USER, $perfil) || in_array(PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil) || in_array(PERFIL_AVAL_EQUIPE_APOIO, $perfil) || in_array(PERFIL_GESTOR, $perfil) ) ){

        #SISTEMA - AVALIAÇÃO SERVIDOR
        if( in_array(PERFIL_AVAL_SERV_AVALIACAO, $perfil) || in_array(PERFIL_AVAL_SERV_SERVIDOR, $perfil) || in_array(PERFIL_AVAL_SERV_CONSULTA, $perfil) || in_array(PERFIL_SERVIDOR, $perfil) ){
            $link_aval = "gestaopessoa.php?modulo=principal/inicio_avaliacao_servidores&acao=A";
        }

        #SISTEMA - FORÇA DE TRABALHO
        if( in_array(PERFIL_FT_CONSULTA_GERAL, $perfil) || in_array(PERFIL_FT_ADMINISTRADOR_CONTRATO, $perfil) || in_array(PERFIL_FT_ADMINISTRADOR_PESSOAL, $perfil) || in_array(PERFIL_FT_ADMINISTRADOR_PROJETO, $perfil) || in_array(PERFIL_FT_FISCAL_CONTRATO, $perfil) || in_array(PERFIL_FT_FISCAL_PESSOAL, $perfil) || in_array(PERFIL_FT_FISCAL_PROJETO, $perfil) || in_array(PERFIL_SERVIDOR, $perfil) || in_array(PERFIL_CONSULTOR, $perfil) || in_array(PERFIL_TERCEIRIZADO, $perfil) ){
            $link_forc = "gestaopessoa.php?modulo=principal/inicio_forca_trabalho&acao=A";
        }

        #SISTEMA - CESSÃO PRORROGAÇÃO
        if( in_array(PERFIL_CESSAO_MOVIMENTACAO, $perfil) || in_array(PERFIL_CESSAO_CAP, $perfil) || in_array(PERFIL_CESSAO_CGGP, $perfil) || in_array(PERFIL_CESSAO_SAA_ADM, $perfil) || in_array(PERFIL_CESSAO_GABINTE_MIN_ADM, $perfil) || in_array(PERFIL_CESSAO_GABINTE_MIN_ADM, $perfil) || in_array(PERFIL_CESSAO_CGGA_ADM, $perfil) || in_array(PERFIL_CESSAO_CGGP_ADM, $perfil) || in_array(PERFIL_CESSAO_CONJUR_ADM, $perfil) || in_array(PERFIL_CESSAO_CONJUR_ADVOGADO, $perfil) || in_array(PERFIL_CESSAO_CONJUR_COORDENACAO, $perfil) ){
            $link_cess = "gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_grid_cessao&acao=A";
        }

    }else{
        $link_aval = "gestaopessoa.php?modulo=principal/inicio_avaliacao_servidores&acao=A";
        $link_forc = "gestaopessoa.php?modulo=principal/inicio_forca_trabalho&acao=A";
        $link_cess = "gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_grid_cessao&acao=A";
        
        #SUB LINKS
        $link_definir_chefe  = "gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_monta_equipe&acao=A";
        $link_chefes_equipes = "gestaopessoa.php?modulo=principal/avaliacao_servidor/lista_grid_feche_equipe&acao=A";
    }

    include  APPRAIZ."includes/cabecalho.inc";
    monta_titulo( 'Gestão de Pessoas', 'Avaliação de Servidores/ Força de Trabalho/ Cessão-Prorrogação' );
?>

<link type="text/css" rel="stylesheet" href="../gestaopessoa/css/caixas_pagina_principal.css" media="screen">

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../gestaopessoa/js/caixas_pagina_principal.js"></script>

<style type="text/css"> 
    .ajusteCaixaPrincipal{
        height:150px !important;
    }
    .ajustebtnCaixa_1{
        height:30px !important;
    }
    .ajustebtnCaixa_1_1{
        height:25px !important;
        width: 175px !important;
    }
</style>
    
<script type="text/javascript">
    $( document ).ready(function() {
        definrCoresCaixas();
    });
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0" style="height:465px;">
    <tr>
        <td width="33%" style="vertical-align: top;">
            <div class="divCaixaPrincipal ajusteCaixaPrincipal" id="divCaixa_1">
                <div class="divCaixaTitulo"> AVALIAÇÃO DE SERVIDORES </div>
                <div class="btnNormal btnOn ajustebtnCaixa_1" id="btnCaixa_1" data-request="<?=$link_aval;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/gestaopessoas/avaliacao_32x32.png"/>
                    <label class="labelTiluloSecundario"> Processo de Avaliação - Formulários de Avaliação </label>
                </div>
                <div class="btnNormal btnOn ajustebtnCaixa_1_1" id="btnCaixa_1_1" data-request="<?=$link_chefes_equipes;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/gestaopessoas/chefes_equipes_32x32.png"/>
                    <label class="labelTiluloSecundario"> Chefes/Equipes </label>
                </div>
                <div class="btnNormal btnOn ajustebtnCaixa_1_1" id="btnCaixa_1_2" data-request="<?=$link_definir_chefe;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/gestaopessoas/definir_chefe_32x32.png"/>
                    <label class="labelTiluloSecundario"> Definir chefes de Equipes </label>
                </div>
            </div>
        </td>
        <td width="33%" style="vertical-align: top;">
             <div class="divCaixaPrincipal" id="divCaixa_2">
                <span class="divCaixaTitulo"> FORÇA DE TRABALHO </span>
                <div class="btnNormal btnOn" id="btnCaixa_2" data-request="<?=$link_forc;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/gestaopessoas/cadastrar_32x32.png"/>
                    <label class="labelTiluloSecundario"> Cadastro dos Servidores </label>
                </div>
            </div>
        </td>
        <td width="33%" style="vertical-align: top;">
             <div class="divCaixaPrincipal" id="divCaixa_3">
                <span class="divCaixaTitulo"> CESSÃO/PRORROGAÇÃO </span>
                <div class="btnNormal btnOn" id="btnCaixa_3" data-request="<?=$link_cess;?>">
                    <img class="iconeTituloSecundario" width="22" src="../imagens/gestaopessoas/cessao_32x32.png"/>
                    <label class="labelTiluloSecundario"> Cessão/Prorrogação </label>
                </div>
            </div>
        </td>
    </tr>
</table>