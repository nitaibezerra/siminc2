<?php
/**
 * Sistema CGO
 * $Id: inicio.inc 100004 2015-07-10 20:05:01Z werteralmeida $
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
?>
<link href="css/progorc.css" rel="stylesheet" media="screen">
<style>
    .btnNormal:hover{background-color:#C0C0C0}
    #divAcoes{background-color:yellowgreen}
    #divSubacoes{background-color:#00CED1}
    #divPI{background-color:#FF6347}
    #divManuais{background-color:#EEB422}
    #divPTRES{background-color:darksalmon}
    #divPrevRecOrc{background-color:#CC6666}
    #divTabelaApoio{background-color:#18bc9c}
    #divComunicados{background-color:royalblue}
    #divAlteracaoCredito{background-color:PaleVioletRed}
    #divAcoes{background-color:yellowgreen}
    #divSubacoes{background-color:#00CED1}
    #divCap{background-color:#FF6347}
    #divManuais{background-color:#EEB422}
    #divPTRES{background-color:darksalmon}
    #divPrevRecOrc{background-color:#CC6666}
    #divTabelaApoio{background-color:#18bc9c}
    #divComunicados{background-color:royalblue}
</style>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" language="JavaScript">
    $(document).ready(function () {
        inicio();
        $('.btnOn').click(function () {
            var uri = $(this).attr('data-request');
            if (!uri) {
                alert('Botão sem url (data-request) definida.');
                return;
            }
            location.href = uri;
        });
        $('.btnNovaJanela').click(function () {
            var uri = $(this).attr('data-request');
            if (!uri) {
                alert('Botão sem url (data-request) definida.');
                return;
            }
            window.open(uri);
        });

        $('#btnCadastrar').click(function () {
            location.href = 'progorc.php?modulo=principal/comunicado/cadastrar&acao=A';
        });

        $('#btnListar').click(function () {
            location.href = 'progorc.php?modulo=principal/comunicado/listar&acao=A';
        });
    });

    function abrirArquivo(arqid) {
        window.location = 'progorc.php?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid=' + arqid;
    }
</script>
<!--<br>
<br>
Senhor Dirigente,
<br>
<br>
Informamos que o módulo SPO - Limites Orçamentários será desativado do SIMEC - Sistema Integrado de Monitoramento, Execução e Controle.
A desativação deve-se ao fato de que os limites já foram pré-definidos e divulgados nas reuniões realizadas pelo CONIF - Conselho Nacional das Instituições da Rede Federal de Educação Profissional, Científica e Tecnológica e pela ANDIFES - Associação Nacional dos Dirigentes das Instituições Federais de Ensino Superior nos dias 18/06 e 30/06, respectivamente.
Em caso de dúvidas, favor entrar em contato com a Coordenação de Programação Orçamentária ? CPRO/CGO/SPO/MEC, por meio dos telefones (61) 2022-8854 / 88560/ 8918/8843.
<br>
Atenciosamente,
<br>
<br>
SPO-->

<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0"
       cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td >
            <br/>
            <div class="divGraf" style="background-color: #FF6347;">
                <span class="tituloCaixa">Pedidos de Limites</span> <br> <br> <br>
                <?php
                $params['texto'] = 'Acompanhamento dos Pedidos';
                $params['tipo'] = 'acompanhamento';
                $params['url'] = 'progorc.php?modulo=principal/limite/acompanhamento&acao=A';
                montaBotaoInicio($params);

                $params['texto'] = 'Pedidos';
                $params['tipo'] = 'listar';
                $params['url'] = 'progorc.php?modulo=principal/limite/listar&acao=A';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params['texto'] = 'Extrato dos Pedidos';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'progorc.php?modulo=relatorio/limites/extrato&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>

            <div id="divTabelaApoio" class="divCap" data-request="">
                <span class="tituloCaixa">Parâmetros SPO</span>
                <?php
                $params['texto'] = 'Upload Pedidos';
                $params['tipo'] = 'upload';
                $params['url'] = 'progorc.php?modulo=principal/limite/pedidoupload&acao=A';
                montaBotaoInicio($params);

                $params['texto'] = 'Upload das Dotações';
                $params['tipo'] = 'upload';
                $params['url'] = 'progorc.php?modulo=principal/limite/uploaddotacoes&acao=A';
                montaBotaoInicio($params);

                $params['texto'] = 'Upload Projeções';
                $params['tipo'] = 'upload';
                $params['url'] = 'progorc.php?modulo=principal/limite/projecaoupload&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>

            <div id="divManuais" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Manuais</span>
                <?php
                $params['texto'] = 'Manual de Pedidos de Limites';
                $params['tipo'] = 'pdf';
                $params['url'] = 'manual.pdf';
                $params['target'] = '_blank';
                montaBotaoInicio($params);
                ?>
            </div>
            <!--
            <div class="divCap" style="background-color: yellowgreen">
                <span class="tituloCaixa">Ações <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                $params['tipo'] = 'snapshot';
                $params['url'] = 'progorc.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            -->
            <div id="divComunicados" class="divCap">
                <span class="tituloCaixa">Comunicados</span>
                <?php
                montaComunicados();
                ?>
            </div>

        </td>
    </tr>

</table>

