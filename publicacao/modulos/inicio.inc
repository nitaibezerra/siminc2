<?php
/**
 * Sistema de Publicação.
 * werteralmeida
 */

/**
 * Cabecalho do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";

?>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">$(document).ready(function () {
        inicio();
    });
</script>
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
             <div class="divGraf bg-gn">
                <span class="tituloCaixa">Pedidos de Publicação</span>
                <?php
                $params = array();
                $params['texto'] = 'Cadastrar Solicitação';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'publicacao.php?modulo=principal/solicitacaopublicacao/formSolicitacaopublicacao&acao=A';
                montaBotaoInicio($params);

                $params = array();
                $params['texto'] = 'Listar Solicitações';
                $params['tipo'] = 'listar';
                $params['url'] = 'publicacao.php?modulo=principal/solicitacaopublicacao/listarSolicitacaopublicacao&acao=A';
                montaBotaoInicio($params);

                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                    $params = array();
                    $params['texto'] = 'Geral';
                    $params['tipo'] = 'relatorio';
                    $params['url'] = '';
                    montaBotaoInicio($params);
                    
                    $params['texto'] = 'Relatório Resumido';
                    $params['tipo'] = 'relatorio';
                    $params['url'] = '';
                    montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divGraf">
                <span class="tituloCaixa" >Manuais</span> <br><br><br>
                <?php
                $params = array();
                $params['texto'] = 'Manual de Solicitação';
                $params['tipo'] = 'pdf';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>
            </div>
        </td>
    </tr>
</table>