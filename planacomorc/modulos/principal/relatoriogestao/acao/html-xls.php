<?php
/**
 * HTML para preenchimento e processamento do XLS.
 * @version $Id: html-xls.php 89593 2014-11-03 18:34:19Z maykelbraz $
 * @see html.php
 */
?>
<div class="quadro-tcu">
    <table>
        <tr>
            <td style="border-color:white!important;" colspan="7">
                <p>Quadro A.5.2.3.1 - Ações de responsabilidade da UJ - OFSS</p>
            </td>
        </tr>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Identificação da Ação</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td class="titulo">Código</td>
                <td colspan="2" style="border:0">%codigo%</td>
                <td style="border:0;font-weight:bold">Tipo:</td>
                <td colspan="3" style="border:0">%rldtipocod%</td>
            </tr>
            <tr>
                <td class="titulo">Título</td>
                <td colspan="6" style="text-align:left;">%rldtitulo%</td>
            </tr>
            <tr>
                <td class="titulo">Iniciativa</td>
                <td colspan="6">%rldiniciativa%</td>
            </tr>
            <tr>
                <td class="titulo">Objetivo</td>
                <td colspan="4" style="border:0">%rldobjetivo%</td>
                <td style="border:0;font-weight:bold">Código:</td>
                <td style="border:0">%rldcodigoobjetivo%</td>
            </tr>
            <tr>
                <td class="titulo">Programa</td>
                <td colspan="2" style="border-left:0;border-right:0">%rldprograma%</td>
                <td style="border-left:0;border-right:0;font-weight:bold">Código:</td>
                <td style="border-left:0;border-right:0">%rldcodigoprograma%</td>
                <td style="border-left:0;border-right:0;font-weight:bold">Tipo:</td>
                <td style="border-left:0">%rldtipoprograma%</td>
            </tr>
            <tr>
                <td class="titulo">Unidade Orçamentária</td>
                <td colspan="6">%unicod%</td>
            </tr>
            <tr>
                <td class="titulo">Ação Prioritária</td>
                <td style="border:0">( %rldacaoprioritaria_t% ) Sim</td>
                <td style="border:0">( %rldacaoprioritaria_f% ) Não</td>
                <td style="border:0;font-weight:bold">Caso positivo:</td>
                <td style="border:0">( %rldacaoprioritariatipo_p% ) PAC</td>
                <td style="border:0">( %rldacaoprioritariatipo_b% ) Brasil sem Miséria</td>
                <td style="border:0">( %rldacaoprioritariatipo_o% ) Outras</td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Lei Orçamentária %exercicio%</th>
            </tr>
            <tr class="level2">
                <th colspan="7">Execução Orçamentária e Financeira</th>
            </tr>
            <tr class="leveln">
                <th colspan="2">Dotação</th>
                <th colspan="3">Despesa</th>
                <th colspan="2">Restos a Pagar<br />inscritos %exercicio%</th>
            </tr>
            <tr class="leveln">
                <th>Inicial</th>
                <th>Final</th>
                <th>Empenhada</th>
                <th>Liquidada</th>
                <th>Paga</th>
                <th>Processados</th>
                <th>Não<br />Processados</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right">R$ %rlddotacaoinicial%</td>
                <td style="text-align:right">R$ %rlddotacaofinal%</td>
                <td style="text-align:right">R$ %rlddespempenhada%</td>
                <td style="text-align:right">R$ %rlddespliquidada%</td>
                <td style="text-align:right">R$ %rlddesppaga%</td>
                <td style="text-align:right">R$ %rldrapinscprocessado%</td>
                <td style="text-align:right">R$ %rldrapinscnaoprocessado%</td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Execução Física</th>
            </tr>
            <tr class="level2">
                <th rowspan="2">Descrição da Meta</th>
                <th rowspan="2">Unidade de Medida</th>
                <th colspan="5">Montante</th>
            </tr>
            <tr class="level2">
                <th colspan="2">Previsto</th>
                <th>Reprogramado</th>
                <th colspan="2">Realizado</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>%rlddescmeta%</td>
                <td>%rldunidademedida%</td>
                <td colspan="2" style="text-align:right">%rldmontanteprevisto%</td>
                <td style="text-align:right">%rldmontantereprogramado%</td>
                <td colspan="2" style="text-align:right">%rldmontanterealizado%</td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Restos a Pagar Não processados - Exercícios Anteriores</th>
            </tr>
            <tr class="level2">
                <th colspan="3">Execução Orçamentária e Financeira</th>
                <th colspan="4">Execução Física - Metas</th>
            </tr>
            <tr class="leveln">
                <th>Valor em 01/01/%exercicio%</th>
                <th>Valor Liquidado</th>
                <th>Valor Cancelado</th>
                <th>Descrição da Meta</th>
                <th>Unidade de medida</th>
                <th colspan="2">Realizada</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right">R$ %rldrapeaem0101%</td>
                <td style="text-align:right">R$ %rldrapeavalorliquidado%</td>
                <td style="text-align:right">R$ %rldrapeavalorcancelado%</td>
                <td>%rldrapeadescricaometa%</td>
                <td>%rldrapeaunidademedida%</td>
                <td colspan="2" style="text-align:right">%rldrapearealizado%</td>
            </tr>
        </tbody>
    </table>
</div>