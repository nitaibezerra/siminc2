<?php
/**
 * Sistema CGO
 * $Id: inicio.inc 99229 2015-06-25 18:40:56Z werteralmeida $
 */
// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST ['exercicio'])) {
    $_SESSION ['exercicio'] = $_REQUEST ['exercicio'];
}
/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";
include APPRAIZ . "includes/cabecalho.inc";
$sql = " SELECT DISTINCT 
            arq.arqid,
            arq.arqnome,
            arq.arqextensao,
            con.angdsc
         FROM
            progfin.anexogeral con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
         WHERE arqstatus = 'A' 
         ORDER BY angdsc
       ";

$listaComunicado = $db->carregar($sql);
?>
<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" language="JavaScript">
    $(document).ready(function () {
        /* Alerta a pedido o Subsecretário Wagner */
        // $("#aviso").dialog({position: 'top', width: 780, top: '20px'});
        inicio();
    });
</script>
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
            <div id="divLiberacaoFinanceira" class="divGraf">
                <span class="tituloCaixa">Liberações Financeiras</span>
                <?php
                $params = array();
                $params['texto'] = 'Solicitação de Recurso Financeiro';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'progfin.php?modulo=principal/solicitacaorecurso/solicitacoes&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Gestão de lotes';
                $params['tipo'] = 'processo';
                $params['url'] = 'progfin.php?modulo=principal/liberacoesfinanceiras/carga&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Retornar pedidos em lote';
                $params['tipo'] = 'processar';
                $params['url'] = 'progfin.php?modulo=principal/solicitacaorecurso/solicitacoes/tramitacaoLote&acao=A';
                montaBotaoInicio($params);

                echo '<span class="subTituloCaixa">Relatórios</span>';

                $params = array();
                $params['texto'] = 'Extrato Solicitação de Recurso Financeiro';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'progfin.php?modulo=principal/solicitacaorecurso/extrato&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divFluxoFinanceiro" class="divGraf">
                <span class="tituloCaixa">Informações Públicas</span>
                <?php
                $params = array();
                $params['texto'] = 'Apuração de Limites de Saque do MEC';
                $params['tipo'] = 'listar';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Apuração de Limites de Saque a SPO';
                $params['tipo'] = 'listar';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Apuração de Limites de Saque da Unidade Orçamentária (UO)';
                $params['tipo'] = 'listar';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>

            </div>
            <div class="divGraf" style="background-color: #374e0c">
                <span class="tituloCaixa">Programação Financeira</span>
                <?php
                $params = array();
                $params['texto'] = 'Programação mensal da Unidade';
                $params['tipo'] = 'listar';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divExtracaoFolha" class="divGraf">
                <span class="tituloCaixa">Extração de Custeio da Folha de Pagamento </span>
                <?php
                $params = array();
                $params['texto'] = 'Upload do Arquivo de Folha de Pagamento';
                $params['tipo'] = 'upload';
                $params['url'] = 'progfin.php?modulo=principal/custeiofolhapagamento/carga&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Regras para processamento da carga';
                $params['tipo'] = 'listar';
                $params['url'] = 'progfin.php?modulo=principal/custeiofolhapagamento/regra/inicio&acao=A';
                montaBotaoInicio($params);

                echo '<span class="subTituloCaixa">Relatórios</span>';

                $params = array();
                $params['texto'] = 'Extrato Completo da Folha de Pagamento';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'progfin.php?modulo=relatorio/custeiofolhapagamento/extratocompleto&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Relatório resumo da Folha Mensal';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'progfin.php?modulo=relatorio/custeiofolhapagamento/resumomensal&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div class="divCap" style="background-color: yellowgreen">
                <span class="tituloCaixa">Ações <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                $params['tipo'] = 'snapshot';
                $params['url'] = 'progfin.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Manuais</span>
                <?php
                $params = array();
                $params['texto'] = 'Manual de Solicitação de Recurso Financeiro';
                $params['tipo'] = 'pdf';
                #$params['target'] = '_blank';
                $params['url'] = 'manual_solicitacao_recurso_financeiro.pdf';
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