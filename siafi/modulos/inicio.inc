<?php
/**
 * Sistema de criação e gestão de propostas orçamentárias.
 * $Id: inicio.inc 102679 2015-09-22 12:57:12Z werteralmeida $
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

$perfis = pegaPerfilGeral();
// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST['exercicio'])) {
    $_SESSION['exercicio'] = $_REQUEST['exercicio'];
}

echo '<br />';
$sql = " SELECT DISTINCT 
            arq.arqid,
            arq.arqnome,
            arq.arqextensao,
            con.angdsc
         FROM
            siafi.anexogeral con
            INNER JOIN public.arquivo arq on con.arqid = arq.arqid
            INNER JOIN seguranca.usuario usu on usu.usucpf = arq.usucpf
         WHERE arqstatus = 'A' AND con.angtipoanexo = 'C'
         ORDER BY
            angdsc
       ";

$listaComunicado = $db->carregar($sql);

include APPRAIZ . "includes/cabecalho.inc";
echo '<br />';
?>
<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        inicio();
    });
</script>
<br />
<table width="95%" class="tabela" bgcolor="#" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
            <div id="divDadosSiop" class="divGraf" style="background-color: #FF6347">
                <span class="tituloCaixa" >Execução
                    <span class="subTituloCaixa" style="width: 50%; border-bottom: none !important; float: right; text-align: right">
                        fonte: SIOP em
                        <?php
                        echo $db->pegaUm("SELECT   TO_CHAR(MAX(dtatualizacao), 'dd/mm/yyyy') AS data FROM  spo.siopexecucao limit 1 ");
                        ?>
                    </span>
                </span>
                <?php
                $params = array();
                $params['texto'] = 'Gerar Relatório';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'siafi.php?modulo=principal/dadossiop/gerarrelatorio&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divDadosSiafi" class="divGraf bg-ds">
                <span class="tituloCaixa">Carga do SIAFI</span>
                <?php
                $params = array();
                $params['texto'] = 'Relatório Financeiro Dinâmico';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'siafi.php?modulo=principal/dadossiafi/gerarrelatorio&acao=A';
                $params['url'] = '';
                montaBotaoInicio($params);

//                $params = array();
//                $params['texto'] = 'PT (Plano de Trabalho)';
//                $params['tipo'] = 'relatorio';
//                $params['url'] = '';
//                montaBotaoInicio($params);
//
//                $params = array();
//                $params['texto'] = 'PTRES (Plano de Trabalho Resumido)';
//                $params['tipo'] = 'relatorio';
//                $params['url'] = '';
//                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'ND (Nota de Dotação)';
                $params['tipo'] = 'relatorio';
                $params['url'] = '';
                montaBotaoInicio($params);

//                $params = array();
//                $params['texto'] = 'PI (Plano Interno)';
//                $params['tipo'] = 'relatorio';
//                $params['url'] = '';
//                montaBotaoInicio($params);


                $params = array();
                $params['texto'] = 'NE (Nota de Empenho)';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'siafi.php?modulo=principal/dadossiafi/listarne&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'NL (Nota de Lançamento)';
                $params['tipo'] = 'relatorio';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'NS (Nota de Sistema)';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'siafi.php?modulo=principal/dadossiafi/listarns&acao=A';
                montaBotaoInicio($params);

//
//                $params = array();
//                $params['texto'] = 'PF(Programação Financeira)';
//                $params['tipo'] = 'relatorio';
//                $params['url'] = '';
//                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'OB (Ordem Bancária)';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'siafi.php?modulo=principal/dadossiafi/listarob&acao=A';
                montaBotaoInicio($params);
                
                $params = array();
                $params['texto'] = 'Gerenciamento Dinâmico de Contas Contábeis';
                $params['tipo'] = 'cadastrar';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>

            </div>
            <div class="divCap" style="background-color: yellowgreen">
                <span class="tituloCaixa">Ações <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                $params['tipo'] = 'snapshot';
                $params['url'] = 'siafi.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Manuais</span>
                <?php
                $params['texto'] = 'Manual';
                $params['tipo'] = 'pdf';
                $params['url'] = '/siafi/manual.pdf';
                $params['target'] = '_blank';
                montaBotaoInicio($params);
                ?>
                <!--                <div class="btnNormal btnNovaJanela" data-request="/siafi/manual.pdf" id="btnDownloadManual">
                                    <img src="/recorc/css/images/book-bookmark.png" /> Manual
                                </div>-->
            </div>

            <div id="divComunicados" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Comunicados</span>
                <?php
                montaComunicados();
                ?>
            </div>
        </td>
    </tr>
</table>