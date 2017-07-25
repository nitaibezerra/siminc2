<?php
/**
 * Tela inicial do módulo planacomorc com o menu de opções disponíveis no sistema.
 * $Id: inicio.inc 103493 2015-10-08 19:31:03Z werteralmeida $
 */
/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";

$perfis = pegaPerfilGeral();
// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST['exercicio'])) {
    $_SESSION['exercicio'] = $_REQUEST['exercicio'];
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
    });
</script>
<div style="width: 450px; float:right; ">
    <p style="color:tomato; font-weight: bold; margin-bottom: 4px;">
        <span class="glyphicon glyphicon-asterisk"></span> Atenção,
    </p>
    <p>
        Tudo referente a <b>Acompanhamentos</b> está em um módulo <a href="/acomporc/acomporc.php?modulo=inicio&acao=C">Novo</a>.
    </p>
</div>
<div style="clear:both;"></div>
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center" >
                <div style=" background-color: #9370DB;" class="divGraf">
                    <center>
                        <span class="tituloCaixa" >Plano Interno</span> <br><br><br>
                    </center>

                    <?php
                    /*$params = array();
                    $params['texto'] = 'Lista de Subações';
                    $params['tipo'] = 'listar';
                    $params['url'] = 'planacomorc.php?modulo=principal/unidade/listasubacoes&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Cadastrar Subação';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=principal/unidade/cadastro_subacoes&acao=A';
                    montaBotaoInicio($params);*/

                    $params = array();
                    $params['texto'] = 'Lista de PI';
                    $params['tipo'] = 'listar';
                    $params['url'] = 'planacomorc.php?modulo=principal/unidade/listapimanter&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Cadastrar PI';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Criar por Proposta';
                    $params['tipo'] = 'listar';
                    $params['url'] = 'planacomorc.php?modulo=principal/unidade/listar_propostas_pi&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Lista de PTRES';
                    $params['tipo'] = 'listar';
                    $params['url'] = 'planacomorc.php?modulo=principal/unidade/listaptres&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Manual de Cadastro de PI e Subações (Instituições Federais)';
                    $params['tipo'] = 'pdf';
                    $params['url'] = '../planacomorc/manual_simec_subacoes_e_pis_unidades.pdf';
                    montaBotaoInicio($params);
                    ?>
                </div>
                <?php

            if (in_array(PFL_SUPERUSUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) {
                ?>
                <!--                <div style="background-color: #FF8C00;" class="divGraf">
                                    <span class="tituloCaixa" >Relatório de Gestão (TCU/CGU)</span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Preencher Relatório';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'planacomorc.php?modulo=principal/relatoriogestao/cadastro_gestao&acao=A';
                montaBotaoInicio($params);
                ?>
                                    <span class="subTituloCaixa">Questionário por Exercício</span>
                <?php
                $params = array();
                $params['texto'] = 'Questionário de Relatório';
                $params['tipo'] = 'questionario';
                $params['url'] = '/planacomorc/planacomorc.php?modulo=principal/questionariorelatoriogestao/listaquestionario&acao=A';
                montaBotaoInicio($params);
                ?>
                                </div>-->
            <?php } ?>
            <div id="divAcoes" class="divGraf bg-ds">
                <span class="tituloCaixa">Ações <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                $params['tipo'] = 'snapshot';
                $params['url'] = 'planacomorc.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                montaBotaoInicio($params);

//                    $params = array();
//                    $params['texto'] = 'Central de Acompanhamento';
//                    $params['tipo'] = 'central';
//                    $params['url'] = 'planacomorc.php?modulo=centralacompanhamento&acao=A';
//                    montaBotaoInicio($params);
//
//                    $params = array();
//                    $params['texto'] = 'Acompanhamento de Ações';
//                    $params['tipo'] = 'acompanhamento';
//                    $params['url'] = 'planacomorc.php?modulo=principal/acoes/listaunidades&acao=A';
//                    montaBotaoInicio($params);
//
//                    $params = array();
//                    $params['texto'] = 'Questionário de Acompanhamento';
//                    $params['tipo'] = 'questionario';
//                    $params['url'] = 'planacomorc.php?modulo=principal/questionario/listaquestionario&acao=A';
//                    montaBotaoInicio($params);
//
//                    $params = array();
//                    $params['texto'] = 'Períodos de Referência do Acompanhamento';
//                    $params['tipo'] = 'calendario';
//                    $params['url'] = 'planacomorc.php?modulo=principal/periodoreferencia/listaperiodoreferencia&acao=A';
//                    montaBotaoInicio($params);
//
//                    $params = array();
//                    $params['texto'] = 'Metodologia do Acompanhamento';
//                    $params['tipo'] = 'calendario';
//                    $params['url'] = 'planacomorc.php?modulo=principal/produto/listametodologia&acao=A';
//                    montaBotaoInicio($params);
//
//                    $params = array();
//                    $params['texto'] = 'Enviar Acompanhamento para o SIOP em LOTE';
//                    $params['tipo'] = 'enviar';
//                    $params['url'] = '/planacomorc/planacomorc.php?modulo=sistema/comunica/enviaQuantitativos&acao=A';
//                    montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
//                    $params = array();
//                    $params['texto'] = 'Responsáveis';
//                    $params['tipo'] = 'user';
//                    $params['url'] = '/planacomorc/planacomorc.php?modulo=relatorio/responsaveis/acoes&acao=A';
//                    montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Espelho das Ações';
                $params['tipo'] = 'espelho';
                $params['url'] = 'planacomorc.php?modulo=principal/dadosacoes/espelho&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
<!--
            <div class="divGraf bg-ds" id="divSubacoes">
                <span class="tituloCaixa">Subações <?= $_SESSION['exercicio']; ?></span><br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Lista de Subações';
                $params['tipo'] = 'listar';
                $params['url'] = 'planacomorc.php?modulo=principal/subacoes/listasubacoesmanter&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Cadastrar Subação';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'planacomorc.php?modulo=principal/subacoes/cadastro&acao=A';
                montaBotaoInicio($params);

//                    $params = array();
//                    $params['texto'] = 'Acompanhamento';
//                    $params['tipo'] = 'acompanhamento';
//                    $params['url'] = 'planacomorc.php?modulo=principal/subacoes/listasubacoes&acao=A';
//                    montaBotaoInicio($params);
//                    
//                    $params = array();
//                    $params['texto'] = 'Central de Acompanhamento';
//                    $params['tipo'] = 'central';
//                    $params['url'] = 'planacomorc.php?modulo=principal/subacoes/centralacompanhamento&acao=A';
//                    montaBotaoInicio($params);
//                    
//                    $params = array();
//                    $params['texto'] = 'Produtos das Subações';
//                    $params['tipo'] = 'listar';
//                    $params['url'] = 'planacomorc.php?modulo=principal/produto/listaproduto&acao=A';
//                    montaBotaoInicio($params);
//                                        $params = array();
//                    $params['texto'] = 'Questionário de Acompanhamento de Subações';
//                    $params['tipo'] = 'questionario';
//                    $params['url'] = 'planacomorc.php?modulo=principal/questionariosubacoes/listaquestionario&acao=A';
//                    montaBotaoInicio($params);
//                    
                $params = array();
                $params['texto'] = 'Solicitação de Remanejamento de Subação';
                $params['tipo'] = 'processo';
                $params['url'] = 'planacomorc.php?modulo=principal/remanejavalorsubacao/listaptres&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Remanejamento de Subação';
                $params['tipo'] = 'processo';
                $params['url'] = 'planacomorc.php?modulo=principal/remanejavalorsubacao/listaptres&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Transações de Remanejamento de Subação';
                $params['tipo'] = 'listar';
                $params['url'] = 'planacomorc.php?modulo=principal/remanejavalorsubacao/listartransacoes&acao=A';
                montaBotaoInicio($params);
                ?>
                <!--<span class="subTituloCaixa">Relatórios</span>-->
                <?php
//                    $params = array();
//                    $params['texto'] = 'Produtos das Subações';
//                    $params['tipo'] = 'user';
//                    $params['url'] = '/planacomorc/planacomorc.php?modulo=relatorio/responsaveis/subacoes&acao=A';
//                    montaBotaoInicio($params);
                ?>
<!--            </div> -->
<!--
            <div id="divPI" class="divGraf">
                <span class="tituloCaixa" >Plano Interno</span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Lista de PI';
                $params['tipo'] = 'listar';
                $params['url'] = 'planacomorc.php?modulo=principal/planointerno/listapimanter&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Cadastrar PI';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'planacomorc.php?modulo=principal/planointerno/cadastro&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Solicitação de Cadastro de PI';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'planacomorc.php?modulo=principal/planointerno/cadastro&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Listar Transações de Solicitação de Cadastro de PI';
                $params['tipo'] = 'listar';
                $params['url'] = 'planacomorc.php?modulo=principal/planointerno/listatransacoes&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
-->
            <div style="background-color: darksalmon;" class="divGraf">
                <span class="tituloCaixa" >PTRES <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Lista de PTRES';
                $params['tipo'] = 'listar';
                $params['url'] = 'planacomorc.php?modulo=principal/ptres/listaptres&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <?php if (in_array(PFL_SUPERUSUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) { ?>
                <div id="divTabelaApoio" class="divGraf">
                    <span class="tituloCaixa" >Tabelas de Apoio</span> <br><br><br>
                    <?php
                    $params = array();
                    $params['texto'] = 'Enquadramentos de Despesa';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/cadEnquadramentoDespesas&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Área Cultural';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/cadModalidadesEnsino&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Seguimento Cultural';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/cadNivelEtapaEnsino&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Executor Orçamentário e Financeiro';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/cadExecutorOrcamentarioFinanceiro&acao=A';
                    montaBotaoInicio($params);

    /*              $params = array();
                    $params['texto'] = 'Gestor da Subação';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/cadGestorSubacao&acao=A';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = 'Vinculação de Enquadramentos de Despesa a Subação';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/AddEnquadramentoSubacao&acao=A';
                    montaBotaoInicio($params);*/

                    $params = array();
                    $params['texto'] = 'Vinculação Orçamentária da Ação Estratégica.';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A&listar=true';
                    montaBotaoInicio($params);

                    $params = array();
                    $params['texto'] = ' Modalidade de Pactuação.';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'planacomorc.php?modulo=sistema/tabelasapoio/cadCategoriaApropriacao&acao=A&listar=true';
                    montaBotaoInicio($params);
                    ?>
                </div>
            <?php } ?>

            <div id="divManuais" class="divGraf">
                <span class="tituloCaixa" >Manuais</span> <br><br><br>
                <?php
//                $params = array();
//                $params['texto'] = 'Planejamento e Acompanhamento Orçamentário 2013';
//                $params['tipo'] = 'pdf';
//                $params['url'] = '../planacomorc/Manual_SIMEC_Planejamento_e_Acompanhamento_Orcamentario_2013.pdf';
//                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Manual de Cadastro de PI e Subações (Instituições Federais)';
                $params['tipo'] = 'pdf';
                $params['url'] = '../planacomorc/manual_simec_subacoes_e_pis_unidades.pdf';
                montaBotaoInicio($params);
                ?>
                <?php
                if (in_array(PFL_SUPERUSUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) {
                    $params = array();
                    $params['texto'] = 'Calendário da SPO';
                    $params['tipo'] = 'calendario';
                    $params['url'] = '../planacomorc/calendario_spo.pdf';
                    montaBotaoInicio($params);
                }
                ?>
            </div>
            <div id="divComunicados" class="divCap">
                <span class="tituloCaixa">Comunicados</span>
                <?php
                montaComunicados();
                ?>
            </div>
        </td>
    </tr>
</table>
