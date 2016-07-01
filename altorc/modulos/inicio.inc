<?php
/**
 * Sistema CGO
 *
 * $Id: inicio.inc 97206 2015-05-12 21:07:31Z mariluciaqueiroz $
 */

/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";

// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST ['exercicio'])) {
    $_SESSION ['exercicio'] = $_REQUEST ['exercicio'];
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" language="JavaScript">
    $(document).ready(function () {
        inicio();
    });
    function abrirArquivo(arqid) {
        window.location = 'altorc.php?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid=' + arqid;
    }
</script>
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0"
       cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
            <div id="divAlteracaoCredito" class="divGraf" style="background-color: #FF4040">
                <span class="tituloCaixa">Alteração de crédito</span>

                <?php
                $params = array();
                $params['texto'] = 'Pedidos';
                $params['tipo'] = 'processar';
                $params['url'] = 'altorc.php?modulo=principal/pedido&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Acompanhamento dos Pedidos';
                $params['tipo'] = 'acompanhamento';
                $params['url'] = 'altorc.php?modulo=principal/pedido/acompanhamento&acao=A';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params = array();
                $params['texto'] = 'Extrato dos Pedidos';
                $params['tipo'] = 'listar';
                $params['url'] = 'altorc.php?modulo=relatorio/momentocredito/extrato&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divAcoes" class="divGraf" style="cursor: pointer;">
                <span class="tituloCaixa">Ações</span>
                <?php
                $params = array();
                $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                $params['tipo'] = 'snapshot';
                $params['url'] = 'altorc.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divTabelaApoio" class="divCap"
                 data-request="">
                <span class="tituloCaixa">Parâmetros SPO</span>
                <?php
                $params = array();
                $params['texto'] = 'Upload Pedidos';
                $params['tipo'] = 'upload';
                $params['url'] = 'altorc.php?modulo=principal/previsaoupload&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Momentos de Alteração Crédito';
                $params['tipo'] = 'calendario';
                $params['url'] = 'altorc.php?modulo=principal/momentocredito/inicio&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Manuais</span>
                 <?php
                $params = array();
                $params['texto'] = 'Manual de Pedido de Alteração de Créditos '.$_SESSION['exercicio'];
                $params['tipo'] = 'pdf';
                $params['url'] = 'manual.pdf';
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
<script>
</script>