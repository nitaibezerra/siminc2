<?php
//
// $Id$
//





if ($_REQUEST['relatorio']) {
    if ($_REQUEST['tp_relatorio'] == 'xls') {
        header("Content-Type: application/vnd.ms-excel");
        header("Content-disposition: attachment; filename=Plano_De_Aplicacao_2008_-_Consolidado.xls");
    }

    echo '
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<table width="100%" cellpadding="0" cellspacing="0">
  <tr>
    <td style="padding: 5px 0px 5px 5px; font-weight: bold; font-size: 14px;">Plano de Aplicação - Ano Exercício 2008 - Relatório Consolidado</td>
  </tr>

  <tr>
    <td>
      <table border="1" width="100%" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3" style="font-size: 10px;">
        <tr>
          <td colspan="8" style="font-weight: bold">Orgão do MEC - Secretaria de Educação Superior - CONCEDENTE</td>
          <td colspan="2">U.G. e GESTÃO<br /><br /></td>
        </tr>

        <tr>
          <td colspan="10" style="font-weight: bold; font-size: 11px; text-align: center; background-color: #ddd">Classificação Orçamentária</td>
        </tr>

        <tr>
          <td colspan="8">
            Programa(s) de Trabalho de Título(s) do(s) Projeto(s)/Atividade(s):<br />
            Programa 1073 - Brasil Universitário<br />
            Ação 8282 - Reestruturação e Expansão das Universidades Federais - REUni.
          </td>
          <td colspan="2">
            FONTE DE RECURSOS<br />
            [<span style="width: 25px; color: #000">X</span>] Tesouro<br />
            [<span style="width: 25px; color: #fff">X</span>] Outras Fontes
          </td>
        </tr>

        <tr>
          <td colspan="10" style="font-weight: bold; font-size: 11px; text-align: center; background-color: #ddd">Aplicação</td>
        </tr>

        <tr>
            <td colspan="10" align="left" >Órgão Aplicador: Ministério da Educação</td>
        </tr>

';


    $sql = "select
                unicod,
                unidsc
            from
                public.unidade u
            where
                u.unistatus     = 'A'
                and u.unitpocod = 'U'
                and u.gunid     = 3";

    $uni = (array) $db->carregar($sql);

    while (list(, $unidade) = each ($uni)) {
        $unicod = $unidade['unicod'];
        $unidsc = $unidade['unidsc'];

        $sql_valor_total_investimento = "SELECT
                                            lidvalor
                                        FROM
                                            reuni.limitedesppercent40 as lim
                                        INNER
                                            JOIN reuni.tipodesp as tipo on tipo.tipid = lim.tipid
                                        WHERE
                                            tipo.tipcod = 4 AND
                                            lim.unicod = '".$unicod."' AND
                                            lim.unitpocod = 'U'";

        $sql_valor_lancado_investimento = "
                                            SELECT SUM(lantotal)
                                            FROM reuni.lancamentodesppercent40 as lan
                                            INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lan.tipid
                                            WHERE tipo.tipcod = 4 AND
                                            lan.unicod = '".$unicod."' AND
                                            lan.unitpocod = 'U'";

        $sql_valor_total_despesa = "SELECT lidvalor
                                            FROM reuni.limitedesp as lim
                                            INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lim.tipid
                                            WHERE tipo.tipcod = 5 AND
                                            lim.unicod = '".$unicod."' AND
                                            lim.unitpocod = 'U'";

        $sql_valor_lancado_despesa= "SELECT SUM(lantotal)
                                            FROM reuni.lancamentodesp as lan
                                            INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lan.tipid
                                            WHERE tipo.tipcod = 5 AND
                                            lan.unicod = '".$unicod."' AND
                                            lan.unitpocod = 'U'";

        $sql_despesa_pessoal = "SELECT
                                    substr( lfxcodnatureza, 1, 2 ) || '.' ||
                                    substr( lfxcodnatureza, 3, 2 ) || '.' ||
                                    substr( lfxcodnatureza, 5, 2 )
                                    as codigo,
                                    lfxdscnatureza, lfxvalor
                                    FROM reuni.lancamentofixo
                                    WHERE unicod = '".$unicod."' AND lfxtipo = '1) - Despesas Orçamentárias de Pessoal'
                                    ORDER BY lfxordem";

        $sqp_soma_pessoal = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo WHERE unicod = '".$unicod."' AND lfxtipo = '1) - Despesas Orçamentárias de Pessoal'";

        $sql_despesa_bolsas = "SELECT
                                substr( lfxcodnatureza, 1, 2 ) || '.' ||
                                substr( lfxcodnatureza, 3, 2 ) || '.' ||
                                substr( lfxcodnatureza, 5, 2 )
                                as codigo,
                                lfxdscnatureza, lfxvalor
                                FROM reuni.lancamentofixo 						
                                WHERE unicod = '".$unicod."' AND lfxtipo = '2) - Despesas Orçamentárias (Bolsas/Capes)'
                                ORDER BY lfxordem";

        $sqp_soma_bolsas = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo WHERE unicod = '".$unicod."' AND lfxtipo = '2) - Despesas Orçamentárias (Bolsas/Capes)'";

        $sql_despesa_bolsas_assitencia = "SELECT tipcod, tipdsc FROM reuni.tipodesp WHERE tipcod = 3";

        $sql_despesa_assitencia = "SELECT
                                    substr( lfxcodnatureza, 1, 2 ) || '.' ||
                                    substr( lfxcodnatureza, 3, 2 ) || '.' ||
                                    substr( lfxcodnatureza, 5, 2 )
                                    as codigo,
                                    lfxdscnatureza, lfxvalor
                                    FROM reuni.lancamentofixo 						
                                    WHERE unicod = '".$unicod."' AND lfxtipo = '3) - Despesas Orçamentárias (Assistência Estudantil)'
                                    ORDER BY lfxordem";

        $sqp_soma_assitencia = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo WHERE unicod = '".$unicod."' AND lfxtipo = '3) - Despesas Orçamentárias (Assistência Estudantil)'";

        $sql_investimento = "
            select
                substr( ndpcod, 1, 2 ) || '.' ||
                substr( ndpcod, 3, 2 ) || '.' ||
                substr( ndpcod, 5, 2 ) || '.' ||
                substr( ndpcod, 7, 2 )
                    as codigo,
                ndpdsc as descricao, lan.lantotal as valor
            from naturezadespesa as nat
                inner join reuni.lancamentodesppercent40 as lan on lan.ndpid = nat.ndpid
                inner join reuni.tipodesp as tipo on tipo.tipid = lan.tipid
            where
                substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
                ndpstatus = 'A' and
                lan.unicod = '".$unicod."' and
                lan.unitpocod = 'U' and 
                tipo.tipcod = '4'
            order by ndpcod";

        $sql_despesa = "
            select
                substr( ndpcod, 1, 2 ) || '.' ||
                substr( ndpcod, 3, 2 ) || '.' ||
                substr( ndpcod, 5, 2 ) || '.' ||
                substr( ndpcod, 7, 2 )
                    as codigo,
                ndpdsc as descricao, lan.lantotal as valor
            from naturezadespesa as nat
                inner join reuni.lancamentodesp as lan on lan.ndpid = nat.ndpid
                inner join reuni.tipodesp as tipo on tipo.tipid = lan.tipid
            where
                substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
                ndpstatus = 'A' and
                lan.unicod = '".$unicod."' and
                lan.unitpocod = 'U' and
                tipo.tipcod = '5'
            order by ndpcod";
        
        $sqp_despesa = "SELECT SUM(lantotal) FROM reuni.lancamentodesp as lan WHERE lan.unicod = '".$unicod."' AND lan.tipid = (select tipid from reuni.tipodesp where tipodesp.tipcod = 5)";

    ?>

        <tr>
          <td colspan="10" style="font-weight: bold; font-size: 11px; text-align: center; background-color: #ddd">
            Descrição do Projeto/Atividade dae Aplicação: Plano de Reestruturação e Expansão da Universidade - REUNI
          </td>
        </tr>

        <tr>
          <td colspan="8">
            <strong>Instituição: </strong><?php echo $unicod , " - " , $unidsc ?><br />
            <strong>Unidade aplicadora: </strong><?php echo $unidsc ?><br />
          </td>
          <td colspan="2">U.G e GESTÃO/CNPJ<br /><br /></td>
        </tr>

        <tr>
          <td colspan="2" style="font-weight: bold; text-align: center">Despesas Orçamentárias de Pessoal</strong></td>
          <td colspan="2" style="font-weight: bold; text-align: center">Despesas Orçamentárias (Bolsas/Capes)</strong></td>
          <td colspan="2" style="font-weight: bold; text-align: center">Despesas Orçamentárias (Assistência Estudantil)</strong></td>
          <td colspan="2" style="font-weight: bold; text-align: center">Despesas Orçamentárias (Investimento)</strong></td>
          <td colspan="2" style="font-weight: bold; text-align: center">Despesas Orçamentárias (Outras Despesas)</strong></td>
        </tr>

        <tr>
          <td style="text-align: center;">Código de Despesa</td>
          <td style="text-align: center;">Valor Atual (R$) </td>
          <td style="text-align: center;">Código de Despesa</td>
          <td style="text-align: center;">Valor Atual (R$) </td>
          <td style="text-align: center;">Código de Despesa</td>
          <td style="text-align: center;">Valor Atual (R$) </td>
          <td style="text-align: center;">Código de Despesa</td>
          <td style="text-align: center;">Valor Atual (R$) </td>
          <td style="text-align: center;">Código de Despesa</td>
          <td style="text-align: center;">Valor Atual (R$) </td>
        </tr>

<?php
        $item_financeiro_valor_total_investimento       = $db->pegaUm($sql_valor_total_investimento);
        $item_financeiro_valor_lancado_investimento     = $db->pegaUm($sql_valor_lancado_investimento);

        $item_financeiro_valor_disponivel_investimento  = $item_financeiro_valor_total_investimento - $item_financeiro_valor_lancado_investimento;

        /*!@
        $item_financeiro_valor_total_investimento       = number_format( $item_financeiro_valor_total_investimento, 2, ",", "." );
        $item_financeiro_valor_lancado_investimento     = number_format( $item_financeiro_valor_lancado_investimento, 2, ",", "." );
        $item_financeiro_valor_disponivel_investimento  = number_format( $item_financeiro_valor_disponivel_investimento, 2, ",", "." );
        //                                                                  */

        $item_financeiro_valor_total_despesa            = $db->pegaUm($sql_valor_total_despesa);
        $item_financeiro_valor_lancado_despesa          = $db->pegaUm($sql_valor_lancado_despesa);

        $item_financeiro_valor_disponivel_despesa       = $item_financeiro_valor_total_despesa - $item_financeiro_valor_lancado_despesa;

        /*!@
        $item_financeiro_valor_total_despesa            = number_format( $item_financeiro_valor_total_despesa, 2, ",", "." );
        $item_financeiro_valor_lancado_despesa          = number_format( $item_financeiro_valor_lancado_despesa, 2, ",", "." );
        $item_financeiro_valor_disponivel_despesa       = number_format( $item_financeiro_valor_disponivel_despesa, 2, ",", "." );
        //                                                                  */


        // Totalizadores
        $resultado_pessoal      = (array)   $db->carregar($sql_despesa_pessoal);
        $total_pessoal          = (integer) $db->pegaUm($sqp_soma_pessoal);

        $lenght                 = sizeof($resultado_pessoal);

        $resultado_bolsas       = (array) $db->carregar($sql_despesa_bolsas);
        $total_bolsas           = $db->pegaUm($sqp_soma_bolsas);

        $lenght                 = ($nlenght > sizeof($resultado_bolsas)) > $lenght ? $nlenght : $lenght;

        $resultado_assitencia   = (array) $db->carregar($sql_despesa_assitencia);
        $total_assitencia       = $db->pegaUm($sqp_soma_assitencia);

        $lenght                 = ($nlenght > sizeof($resultado)) > $lenght ? $nlenght : $lenght;

        $resultado_investimento = (array) $db->carregar($sql_investimento);
        /*!@
        TOTAL DISPONÍVEL: $item_financeiro_valor_disponivel_investimento
        TOTAL:            $item_financeiro_valor_total_investimento
        TOTAL LANÇADO:    $item_financeiro_valor_lancado_investimento
        //                                                                  */

        $lenght                 = ($nlenght > sizeof($resultado_investimento)) > $lenght ? $nlenght : $lenght;

        $resultado_despesa      = (array) $db->carregar($sql_despesa);
        $total_despesa          = $db->pegaUm($sqp_despesa);

        $lenght                 = ($nlenght > sizeof($resultado_despesa)) > $lenght ? $nlenght : $lenght;
        /*!@
        TOTAL DISPONÍVEL: $item_financeiro_valor_disponivel_despesa
        TOTAL:            $item_financeiro_valor_total_despesa
        TOTAL LANÇADO:    $item_financeiro_valor_lancado_despesa
        //                                                                  */

        $totais = array($resultado_pessoal,
                        $resultado_bolsas,
                        $resultado_assitencia,
                        $resultado_investimento,
                        $resultado_despesa);

        for ($i = 0; $i < $lenght; $i++) {
            if (is_array($totais[0][$i])) {
                $codigo_0 = $totais[0][$i]['codigo'];
                $valor_0  = number_format($totais[0][$i]['lfxvalor'], 2, ',', '.');
            } else {
                $codigo_0 = '&nbsp';
                $valor_0  = '&nbsp';
            }

            if (is_array($totais[1][$i])) {
                $codigo_1 = $totais[1][$i]['codigo'];
                $valor_1  = number_format($totais[1][$i]['lfxvalor'], 2, ',', '.');
            } else {
                $codigo_1 = '&nbsp';
                $valor_1  = '&nbsp';
            }

            if (is_array($totais[2][$i])) {
                $codigo_2 = $totais[2][$i]['codigo'];
                $valor_2  = number_format($totais[2][$i]['lfxvalor'], 2, ',', '.');
            } else {
                $codigo_2 = '&nbsp';
                $valor_2  = '&nbsp';
            }

            if (is_array($totais[3][$i])) {
                $codigo_3 = $totais[3][$i]['codigo'];
                $valor_3  = number_format($totais[3][$i]['valor'], 2, ',', '.');
            } else {
                $codigo_3 = '&nbsp';
                $valor_3  = '&nbsp';
            }

            if (is_array($totais[4][$i])) {
                $codigo_4 = $totais[4][$i]['codigo'];
                $valor_4  = number_format($totais[4][$i]['valor'], 2, ',', '.');
            } else {
                $codigo_4 = '&nbsp';
                $valor_4  = '&nbsp';
            }

            $cor = isset($cor) || $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5';

            echo '        <tr bgcolor="' , $cor , '" onmouseout="this.style.backgroundColor=\'#f5f5f5\';" onmouseover="this.style.backgroundColor=\'#ffffcc\';">
          <td style="text-align: center">' , $codigo_0 , '</td><td style="text-align: right">' , $valor_0 , '</td>
          <td style="text-align: center">' , $codigo_1 , '</td><td style="text-align: right">' , $valor_1 , '</td>
          <td style="text-align: center">' , $codigo_2 , '</td><td style="text-align: right">' , $valor_2 , '</td>
          <td style="text-align: center">' , $codigo_3 , '</td><td style="text-align: right">' , $valor_3 , '</td>
          <td style="text-align: center">' , $codigo_4 , '</td><td style="text-align: right">' , $valor_4 , '</td>
        </tr>' , "\n";
            //                                                              */
        }

        echo '        </tr>

        <tr>
          <td style="font-weight: bold; text-align: right">Total</td><td style="text-align: right;">'                     , number_format($total_pessoal, 2, ",", ".") , '</td>
          <td style="font-weight: bold; text-align: right">Total</td><td style="text-align: right;">'                     , number_format($total_bolsas, 2, ",", ".") , '</td>
          <td style="font-weight: bold; text-align: right">Total</td><td style="text-align: right;">'                     , number_format($total_assitencia, 2, ",", ".") , '</td>
          <td style="font-weight: bold; text-align: right">Total</td><td style="text-align: right;">'                     , number_format($item_financeiro_valor_lancado_investimento, 2, ",", ".") , '</td>
          <td style="font-weight: bold; text-align: right">Total</td><td style="text-align: right; margin-bottom: 10px">' , number_format($item_financeiro_valor_lancado_despesa, 2, ",", ".") , '</td>
        </tr>' , "\n";

        //break;
    }
?>
      </table>
    </td>
  </tr>
</table>
<?php

} else {

    include APPRAIZ . 'includes/cabecalho.inc';
    include APPRAIZ . 'includes/Agrupador.php';
    echo '<br />';


    monta_titulo( $titulo_modulo, '' );
?>

<form action="" method="post" id="formEmissaoRelatorio">
  <input type="hidden" name="relatorio" value="1" />
  <input type="hidden" name="tp_relatorio" id="tp_relatorio" value="htm" />
  <table align="center" bgcolor="#f5f5f5" cellpadding="10" cellspacing="0" border="0" class="tabela">
    <tr>
      <td style="text-align: center">
        <input type="button" onclick="document.getElementById('tp_relatorio').value = 'xls'; document.getElementById('formEmissaoRelatorio').submit();" value="Emitir XLS" />
        <input type="button" onclick="document.getElementById('tp_relatorio').value = 'htm'; windowOpen('reuni.php?modulo=relatorios/planoaplicacaoconsolidado&acao=A&relatorio=1&tp_relatorio=htm', 'relatorio')" value="Exibir HTML" />
      </td>
    </tr>
  </table>
</form>

<?php
} // if ($_REQUEST['relatorio'])





