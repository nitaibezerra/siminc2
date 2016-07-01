<?php
/**
 * Sistema de criação e gestão de propostas orçamentárias.
 * $Id: inicio.inc 100646 2015-07-30 20:48:15Z maykelbraz $
 */
/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
    });
    function abrirArquivoComunicado(arqid) {
        var uri = window.location.href;
        uri = uri.replace(/\?.+/g, '?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid=' + arqid);
        window.location.href = uri;
    }
</script>
<br />
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
            <div id="divLiberacaoFinanceira" class="divGraf">
                <span class="tituloCaixa">PLOA <?php echo $_SESSION['exercicio'] + 1; ?></span>
                <?php
                $params = array();
                $params['texto'] = 'Gestão da PLOA';
                $params['tipo'] = 'processo';
                $params['url'] = 'proporc.php?modulo=principal/gestaoploa/inicio&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Controle de Envio SIOP';
                $params['tipo'] = 'log';
                $params['url'] = 'proporc.php?modulo=principal/gestaoploa/controleenvio&acao=A';
                montaBotaoInicio($params);
                $params = array();

                $params['texto'] = 'Pré-Limites Pessoal, Benefícios e Pensões';
                $params['tipo'] = 'listar';
                $params['url'] = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params = array();
                $params['texto'] = 'Extrato Completo';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'proporc.php?modulo=relatorio/gestaoploa/extrato&acao=A';
                montaBotaoInicio($params);
                ?>


            </div>
            <div id="divAcoes" class="divGraf" style="cursor: pointer;">
                <span class="tituloCaixa">Ações</span>
                <?php
                $params = array();
                $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                $params['tipo'] = 'snapshot';
                $params['url'] = 'proporc.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params = array();
                $params['texto'] = 'Espelho das Ações';
                $params['tipo'] = 'espelho';
                $params['url'] = 'proporc.php?modulo=relatorio/espelho/espelhoacoes&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divCap" style="cursor:pointer;">
                <span class="tituloCaixa">Manuais</span>
                <?php
                $proximo_ano = $_SESSION['exercicio'] + 1;
                $params = array();
                $params['texto'] = "Manual de Pré-Limites Pessoal, Benefícios e Pensões {$proximo_ano}";
                $params['tipo'] = 'pdf';
                $params['url'] = "manual_prelimites_{$proximo_ano}.pdf";
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divLimites" class="divGraf" style="background-color: #18bc9c">
                <span class="tituloCaixa">Parâmetros SPO para PLOA <?php echo $_SESSION['exercicio'] + 1; ?></span>
                <?php
                $params = array();
                $params['texto'] = 'Configuração de despesas';
                $params['tipo'] = 'tabelaapoio';
                $params['url'] = 'proporc.php?modulo=principal/despesas/despesas&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Definição de Limites';
                $params['tipo'] = 'tabelaapoio';
                $params['url'] = 'proporc.php?modulo=principal/limites/limites&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = "Carga de Despesas";
                $params['tipo'] = 'upload';
                $params['url'] = 'proporc.php?modulo=principal/despesas/carga&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $ploa = $_SESSION['exercicio'] + 1;
                $params['texto'] = "Período de Preenchimento da Proposta (PLOA {$ploa})";
                $params['tipo'] = 'calendario';
                $params['url'] = 'proporc.php?modulo=principal/periodoreferencia/periodoreferencia&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $ploa = $_SESSION['exercicio'] + 1;
                $params['texto'] = "Período de Pré-Limite";
                $params['tipo'] = 'calendario';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params = array();
                $params['texto'] = 'QDD por PO';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'proporc.php?modulo=relatorio/qdds/listarUosQdd&acao=A&tipo=showQDDPO';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'QDD por Subação';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'proporc.php?modulo=relatorio/qdds/listarUosQdd&acao=A&tipo=showQDDSubacao';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Síntese das Despesas';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'proporc.php?modulo=relatorio/qdds/listarUosQdd&acao=A&tipo=showSinteseDespesas';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Limites Orçamentários';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'proporc.php?modulo=relatorio/qdds/listarUosQdd&acao=A&tipo=showLimites';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'UO e Grupo de Colunas';
                $params['tipo'] = 'relatorio';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'QDD por PO / Subação';
                $params['tipo'] = 'relatorio';
                $params['url'] = '';
                montaBotaoInicio($params);
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