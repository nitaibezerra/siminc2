<?php
/**
 * Sistema de criação e gestão de propostas orçamentárias.
 * $Id: inicio.inc 82784 2014-07-14 12:43:18Z werteralmeida $
 */
/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
//$perfis = pegaPerfilGeral();
// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST['exercicio'])) {
    $_SESSION['exercicio'] = $_REQUEST['exercicio'];
}
/*
 * Componetnes para a criação da página Início
 */
include APPRAIZ . "includes/dashboardbootstrap.inc";
include APPRAIZ . 'includes/cabecalho.inc';

?>
<link href="/includes/spo.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
$(document).ready(function() {
    inicio();
});
</script>
<br />
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">            
            <div id="divLiberacaoFinanceira" class="divGraf">
                <span class="tituloCaixa">Termo de Execução Descentralizada</span>
                <?php
                $params['texto'] = 'Listar Termos';
                $params['tipo'] = 'listar';
                $params['url'] = 'ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A';
                montaBotaoInicio($params);

                if (Ted_Utils_Model::checkUsuarioResponsabilidade()) {
                    $params['texto'] = 'Cadastrar Termo';
                    $params['tipo'] = 'cadastrar';
                    $params['url'] = 'ted.php?modulo=principal/termoexecucaodescentralizada/proponente&acao=A';
                    montaBotaoInicio($params);
                }

                if (Ted_Utils_Model::quemTramitaTermoLote()) {
                    $params['texto'] = 'Tramitação em Lote';
                    $params['tipo'] = 'processo';
                    $params['url'] = 'ted.php?modulo=principal/termoexecucaodescentralizada/tramitaLote&acao=A';
                    montaBotaoInicio($params);
                }
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params['texto'] = 'Exportação Macro';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'ted.php?modulo=relatorio/exportacaoMacro&acao=A';
                montaBotaoInicio($params);

                $params['texto'] = 'Geral';
                $params['tipo'] = 'relatorio';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params['texto'] = 'Relatório por Unidade Gestora (UG)';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'ted.php?modulo=relatorio/relPorUG&acao=A';
                montaBotaoInicio($params);

                $params['texto'] = 'Relatório Cumprimento Execução';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'ted.php?modulo=relatorio/relCumpExec&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divCap" style="cursor: pointer;">
                <span class="tituloCaixa">Manuais</span>
                <?php
                $params['texto'] = 'Manual 2.0';
                $params['tipo'] = 'pdf';
                $params['url'] = '/manualTermoExecucaoDescentralizadoV2.pdf';
                $params['target'] = '_blank';
                montaBotaoInicio($params);

                $params['texto'] = 'Portaria N°8 Descentralizações';
                $params['tipo'] = 'pdf';
                $params['url'] = '/Portaria8Descentralizacoes.pdf';
                $params['target'] = '_blank';
                montaBotaoInicio($params);

                $params['texto'] = 'Portaria Interministerial N°507 2011';
                $params['tipo'] = 'pdf';
                $params['url'] = '/PortariaInterministerialN507D24Novembro2011.pdf';
                $params['target'] = '_blank';
                montaBotaoInicio($params);

                $params['texto'] = 'Portaria N°1529 2014 Descentralização MEC';
                $params['tipo'] = 'pdf';
                $params['url'] = '/Portaria_1529_de_30-12-2014_Regulamenta_Descentralizaoes_no_ambito_do_MEC.pdf';
                $params['target'] = '_blank';
                montaBotaoInicio($params);
                ?>
                <div class="btnNormal"  
                     data-toggle="modal" 
                     data-target="#fluxo">
                    <span class="iconeBotao glyphicon glyphicon-random" style="color: red; font-size: 16px; margin-right: 5px;" ></span> 
                    Fluxo do Termo de Execução Descentralizada
                </div>
            </div>
            <div id="divTabelaApoio" class="divGraf">
                <center>
                    <span class="tituloCaixa" >Tabelas de Apoio</span> <br><br><br>
                </center>
                <?php
                $params['texto'] = 'Representante Legal';
                $params['tipo'] = 'tabelaapoio';
                $params['url'] = 'ted.php?modulo=sistema/tabeladeapoio/representanteLegal&acao=A';
                montaBotaoInicio($params);

                $params['texto'] = 'Unidade Gestora';
                $params['tipo'] = 'tabelaapoio';
                $params['url'] = 'ted.php?modulo=sistema/tabeladeapoio/unidadeGestora&acao=A';
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
<div class="modal fade" id="fluxo" tabindex="-1" role="dialog" aria-hidden="true"  >
    <div class="modal-dialog" style="width: 80%; ">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
                <h4 class="modal-title">Fluxo do Termo de Execução Descentralizada</h4>
            </div>
            <div class="modal-body" style="overflow: auto; text-align: center;">
                <img src="fluxo_ted.jpg" alt="Fluxo" height="80%" />
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

<?php

$sql = "

";

?>