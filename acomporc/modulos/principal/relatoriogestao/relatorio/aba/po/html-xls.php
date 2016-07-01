<?php
/**
 * HTML para preenchimento e processamento do PDF.
 * @version $Id: html-xls.php 95729 2015-03-24 17:43:21Z lindalbertofilho $
 * @see html.php
 */
?>
<div class="quadro-tcu">
    <table>
        <thead>
            <tr>
                <th colspan="7">Identificação do Plano Orçamentário</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td class="titulo">Código</td>
                <td colspan="6">%codigo%</td>
            </tr>
            <tr>
                <td class="titulo">Título</td>
                <td colspan="6">%rldtitulo%</td>
            </tr>
            <tr>
                <td class="titulo">Programa</td>
                <td colspan="2">%rldprograma%</td>
                <td colspan="2"><b>Código:</b> %rldcodigoprograma%</td>
                <td colspan="2"><b>Tipo:</b> %rldtipoprograma%</td>
            </tr>
            <tr>
                <td class="titulo">Unidade Orçamentária</td>
                <td colspan="6" style="text-align:left">%unicod%</td>
            </tr>
            <tr>
                <td class="titulo">Ação Prioritária</td>
                <td colspan="2">( %rldacaoprioritaria_t% ) Sim &nbsp;( %rldacaoprioritaria_f% ) Não</td>
                <td colspan="4"><b>Caso positivo:</b>&nbsp;&nbsp;&nbsp;&nbsp;
                    ( %rldacaoprioritariatipo_p% ) PAC&nbsp;&nbsp;&nbsp;
                    ( %rldacaoprioritariatipo_b% ) Brasil sem Miséria&nbsp;&nbsp;&nbsp;
                    ( %rldacaoprioritariatipo_o% ) Outras
                </td>
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
                <th rowspan="2" colspan="2">Descrição da Meta</th>
                <th rowspan="2" colspan="2">Unidade de Medida</th>
                <th colspan="3">Montante</th>
            </tr>
            <tr class="level2">
                <th>Previsto</th>
                <th>Reprogramado</th>
                <th>Realizado</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td colspan="2">%rlddescmeta%</td>
                <td colspan="2">%rldunidademedida%</td>
                <td style="text-align:right">%rldmontanteprevisto%</td>
                <td style="text-align:right">%rldmontantereprogramado%</td>
                <td style="text-align:right">%rldmontanterealizado%</td>
            </tr>
        </tbody>
    </table>
    <table>
        <thead>
            <tr>
                <th colspan="7">Restos a Pagar Não processados - Exercícios Anteriores</th>
            </tr>
            <tr class="level2">
                <th colspan="4">Execução Orçamentária e Financeira</th>
                <th colspan="3">Execução Física - Metas</th>
            </tr>
            <tr class="leveln">
                <th colspan="2">Valor em 01/01/%exercicio%</th>
                <th>Valor Liquidado</th>
                <th>Valor Cancelado</th>
                <th>Descrição da Meta</th>
                <th>Unidade de medida</th>
                <th>Realizada</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right" colspan="2">R$ %rldrapeaem0101%</td>
                <td style="text-align:right">R$ %rldrapeavalorliquidado%</td>
                <td style="text-align:right">R$ %rldrapeavalorcancelado%</td>
                <td>%rldrapeadescricaometa%</td>
                <td>%rldrapeaunidademedida%</td>
                <td style="text-align:right">%rldrapearealizado%</td>
            </tr>
        </tbody>
    </table>
</div>