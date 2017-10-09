<?php
/**
 * Sistema de criação e gestão de propostas orçamentárias.
 * $Id: inicio.inc 86650 2014-09-11 19:46:35Z werteralmeida $
 */

/**
 * Limpa os filtros que estão em sessão
 */
clearAllStorage();

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
<script type="text/javascript" lang="javascript">
    $(document).ready(function() {
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
                <span class="tituloCaixa">Pedidos de CDO</span>
                <?php
                $params = array();
                $params['texto'] = 'Acompanhamento dos Pedidos';
                $params['tipo'] = 'acompanhamento';
                $params['url'] = '';
                montaBotaoInicio($params);
                
                $params = array();
                $params['texto'] = 'Pedidos';
                $params['tipo'] = 'listar';
                $params['url'] = 'sicaj.php?modulo=principal/listar&acao=A';
                montaBotaoInicio($params);
                ?>                
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params = array();
                $params['texto'] = 'Extrato Completo';
                $params['tipo'] = 'relatorio';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>              
            </div>

            <div id="divManuais" class="divCap">
                <span class="tituloCaixa">Manuais</span>
                <?php
                $params = array();
                $params['texto'] = 'Manual';
                $params['tipo'] = 'pdf';
                $params['url'] = "manual{$_SESSION['exercicio']}.pdf";
                montaBotaoInicio($params);
                
                $params = array();
                $params['texto'] = 'Fluxo de Pedidos <br>de CDO';
                $params['tipo'] = 'processo';
                $params['toggle'] = 'modal';
                $params['target'] = '#fluxo';
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
            <!--Comentado pois o autoload está dando conflito com tela de pedido-->
<!--            <div class="divCap" style="background-color: yellowgreen">
                <span class="tituloCaixa">Ações <?//= $_SESSION['exercicio']; ?></span> <br><br><br>-->
                <?php
//                    $params = array();
//                    $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
//                    $params['tipo'] = 'snapshot';
//                    $params['url'] = 'sicaj.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
//                    montaBotaoInicio($params);
                ?>
            <!--</div>-->
        </td>
    </tr>
</table>     

<div class="modal fade" id="fluxo" tabindex="-1" role="dialog" aria-hidden="true"  >
    <div class="modal-dialog" style="width: 90%; ">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
                <h4 class="modal-title">Fluxo do Pedido de CDO </h4>
            </div>
            <div class="modal-body" style="overflow: auto; text-align: center;">
                <img src="fluxo_pedido_cdo.jpg" alt="Fluxo" />
                <br/><br/>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->