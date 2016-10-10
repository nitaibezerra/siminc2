<!--<script src="includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="includes/JQuery/jquery-1.9.1/funcoes.js"></script>
<script src="includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<link href="includes/JQuery/jquery-1.9.1/css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<link href="css/estilo.css" rel="stylesheet" type="text/css" />-->

<?php

include_once( APPRAIZ. "pes/modulos/principal/acompanhamento/_funcoes_despesas.php" );

if($_POST['action'] == 'mostrar_restantes'){

    $coEntidade = $_POST['codigo_entidade'];
    $ano = $_POST['ano'];

    mostrarDespesasRstantes($coEntidade, $ano);
    exit;
}



//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";

// Exibindo barra com os dados recursivos da entidade.
$controllerEntidade = new Controller_Usuario();
echo $controllerEntidade->barraEntidadeUsuarioAction();
echo '<br />';

monta_titulo('Níveis de Preenchimento - Geral', '&nbsp;');

$entcodigo = $_SESSION['pes']['codigo_entidade'];


$anoSelecionado = ( isset($_REQUEST['ano']) && !empty( $_REQUEST['ano']))? $_REQUEST['ano']: date('Y');

$anoInicial = $anoSelecionado;
$anoFinal   = $anoSelecionado;
$mesAtual   = date('m') == 1 ? 12 : (date('m') - 1);

$sql = "select  distinct ent.entcodigo, ent.entnome, ent.uorcodigo, coalesce(ano, $anoFinal) as ano,
            total,
            case
            when coalesce(val.status, '') = '' and coalesce(total, 0) = 0 then '1 - Cadastramento não iniciado'
            when coalesce(val.status, '') = '' and coalesce(total, 0) > 0 then '2 - Em cadastramento'
            else val.status
            end as status
        from pes.pesentidade ent
            inner join pes.pesunidadeorcamentaria uor on uor.uorcodigo = ent.uorcodigo
            left  join (
            select entcodigo, ano, sum(total) as total
                from(
                select cont.entcodigo, ceaano as ano, sum(cea.ceavalor) as total
                from pes.pescontrato cont
                    inner join pes.pescelulaacompanhamento  cea on cea.concodigo = cont.concodigo
                    inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                    inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                where ceaano  >= $anoInicial
                and ccototaliza = 'S'
                group by cont.entcodigo, ceaano

                union all

                select cnd.entcodigo, canano as ano, sum(can.canvalor) as total
                from pes.pescelulaacompnatdespesa can
                    inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo
                where canano  >= $anoInicial
                and cantipovalor = 'FN'
                group by cnd.entcodigo, canano
                ) as agrupador
                group by entcodigo, ano
            ) con on con.entcodigo = ent.entcodigo
            left join (
            select entcodigo, -- sum(naoiniciado) as naoiniciado, sum(aprovado) as aprovado, sum(reprovado) as reprovado,
                   -- (select count(*) from pes.pestipodespesa where tidacompanha = 't' and tidpossuimeta = 't') * $mesAtual as totalmaximo,
                   case
                  when sum(aprovado) >= ((select count(*) from pes.pestipodespesa where tidacompanha = 't' and tidpossuimeta = 't') * $mesAtual) then '4 - Aprovado'
                  when (sum(aprovado) + sum(reprovado)) > 0 then '3 - Em validação'
                  else '2 - Em cadastramento'
                   end as status
            from (
                select entcodigo, count(*) as naoiniciado, 0 as aprovado, 0 as reprovado
                from pes.pesvalidacao
                where  valstatus = 'NI'
                and valmes <= $mesAtual
                and valano = $anoFinal
                group by entcodigo, aprovado,reprovado

                union all

                select entcodigo, 0 as naoiniciado, count(*) as aprovado, 0 as reprovado
                from pes.pesvalidacao
                where  valstatus = 'APR'
                and valmes <= $mesAtual
                and valano = $anoFinal
                group by entcodigo, naoiniciado,reprovado

                union all

                select entcodigo, 0 as naoiniciado, 0 as aprovado, count(*) as reprovado
                from pes.pesvalidacao
                where  valstatus = 'REP'
                and valmes <= $mesAtual
                and valano = $anoFinal
                group by entcodigo, naoiniciado,aprovado
            ) as validacao
            group by entcodigo
            ) as val on val.entcodigo = ent.entcodigo
        where orgcodigo = '". CODIGO_ORGAO_SISTEMA. "'
        -- and ent.entcodigo = 454

        ";

//Ordencao
if($_GET['ordem_nome']){
        $sql .= "order by entnome {$_GET['ordem_nome']}";
}else if($_GET['ordem_valor']){
        $sql .= "order by status {$_GET['ordem_valor']}";
}else {
    $sql .= "order by status, entnome, ano";
}


$result = $db->carregar($sql);
$aEntidades = array();

$resultDivididoPorStatus = array();
$total = count($result);
if ($result) {

    foreach ($result as $dado) {
        $aEntidades[$dado['entnome']][$dado['ano']] = $dado;
        $resultDivididoPorStatus[$dado['status']][] = $dado;
    }

    $totalStatus = array();
    foreach($resultDivididoPorStatus as $key => $resultStatus){
        $totalStatus[$key] = count($resultStatus);//round(count($resultStatus) * 100 / $total, 2) ;
    }
} else {
    $totalStatus['1 - Cadastramento não iniciado'] = 0;
    $totalStatus['2 - Em cadastramento'] = 0;
    $totalStatus['3 - Em validação'] = 0;
    $totalStatus['4 - Aprovado'] = 0;
}

for ($ano=$anoInicial; $ano<=$anoFinal; $ano++) {
    $aAno[] = $ano;
}

?>

<script language="javascript" type="text/javascript" src="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.css" />

<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
            <form method="POST" name="form" id="form">
                <tr class="container_botao">
                    <td colspan="2" class="container_button_save">
                            Ano referência:
                            <?php
                                $sql = "select aexano as codigo, aexano as descricao
                                        from pes.pesanoexercicio
                                        order by aexano";
                                ?>
                            <?php echo $db->monta_combo( "ano", $sql, 'S', "Selecione", 'mudarAno', '', '', '', 'S', 'ano', '',  $anoSelecionado); ?>
                        <br />
                    </td>
                </tr>
            </form>
        </table>
<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
    <tr>
            <td>
                <div id="container" style="height:350px;width:100%; "></div>
            </td>
<!--        <td>
            <div id="chart" style="height:250px;width:800px; "></div>
        </td>-->
    </tr>
</table>
<script language="javascript" type="text/javascript">

    function mudarAno(ano)
    {
        $('#form').submit();
    }

    $(function () {

        // Radialize the colors
        Highcharts.getOptions().colors = Highcharts.map(Highcharts.getOptions().colors, function(color) {
            return {
                radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                stops: [
                    [0, color],
                    [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                ]
            };
        });

        // Build the chart
        $('#container').highcharts({
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: true
            },
            title: {
                text: 'Situação'
            },
            tooltip: {
            headerFormat: '<span style="font-size: 14px"><strong>{point.key}</strong></span><br/>',
            pointFormat: 'Quantidade de Entidades: {point.y} <br />Porcentagem: {point.percentage:.2f}%'
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: false,
                        color: '#000000',
                        distance: 30,
                        formatter: function() {
                            return this.percentage +' %'; //'<b>'+ this.point.name +'</b>: '+
                        },
                        format: '{point.percentage:.2f}%</b>',
                    showInLegend: true
                    },showInLegend: false
                }
            },
            series: [{
                type: 'pie',
                name: 'Situação da despesa',
                data: [
                    ['Cadastramento não iniciado',   <?php echo $totalStatus['1 - Cadastramento não iniciado'] ? $totalStatus['1 - Cadastramento não iniciado'] : 0; ?>],
                    ['Em cadastramento',   <?php echo $totalStatus['2 - Em cadastramento'] ? $totalStatus['2 - Em cadastramento'] : 0; ?>],
                    ['Em validação',   <?php echo $totalStatus['3 - Em validação'] ? $totalStatus['3 - Em validação'] : 0; ?>],
                    ['100% Validado',     <?php echo $totalStatus['4 - Aprovado'] ? $totalStatus['4 - Aprovado'] : 0; ?>]
                ]
            }]
        });
    });

    Highcharts.theme = {
   colors: ['#FF6A6A', '#FFD700', '#00BFFF', '#7CCD7C', "#DDDF0D", "#55BF3B", "#DF5353", "#7798BF", "#aaeeee", "#ff0066", "#eeaaee",
      "#55BF3B", "#DF5353", "#7798BF", "#aaeeee"],
   chart: {
//      backgroundColor: {
//         linearGradient: [0, 0, 250, 500],
//         stops: [
//            [0, 'rgb(48, 96, 48)'],
//            [1, 'rgb(0, 0, 0)']
//         ]
//      },
      borderColor: '#000000',
      borderWidth: 0,
//      className: 'dark-container',
      plotBackgroundColor: 'rgba(255, 255, 255, .1)',
//      plotBorderColor: '#CCCCCC',
      plotBorderWidth: 1
   },
//   title: {
//      style: {
//         color: '#C0C0C0',
//         font: 'bold 16px "Trebuchet MS", Verdana, sans-serif'
//      }
//   },
   subtitle: {
      style: {
         color: '#666666',
         font: 'bold 12px "Trebuchet MS", Verdana, sans-serif'
      }
   },
   xAxis: {
      gridLineColor: '#333333',
      gridLineWidth: 1,
      labels: {
         style: {
            color: '#A0A0A0'
         }
      },
      lineColor: '#A0A0A0',
      tickColor: '#A0A0A0',
      title: {
         style: {
            color: '#CCC',
            fontWeight: 'bold',
            fontSize: '12px',
            fontFamily: 'Trebuchet MS, Verdana, sans-serif'

         }
      }
   },
   yAxis: {
      gridLineColor: '#333333',
      labels: {
         style: {
            color: '#A0A0A0'
         }
      },
      lineColor: '#A0A0A0',
      minorTickInterval: null,
      tickColor: '#A0A0A0',
      tickWidth: 1,
      title: {
         style: {
            color: '#CCC',
            fontWeight: 'bold',
            fontSize: '12px',
            fontFamily: 'Trebuchet MS, Verdana, sans-serif'
         }
      }
   },
//   tooltip: {
//      backgroundColor: 'rgba(0, 0, 0, 0.75)',
//      style: {
//         color: '#ffffff'
//      }
//   },
   toolbar: {
      itemStyle: {
         color: 'silver'
      }
   },
   plotOptions: {
      line: {
         dataLabels: {
            color: '#CCC'
         },
         marker: {
            lineColor: '#333'
         }
      },
      spline: {
         marker: {
            lineColor: '#333'
         }
      },
      scatter: {
         marker: {
            lineColor: '#333'
         }
      },
      candlestick: {
         lineColor: 'white'
      }
   },
//   legend: {
//    layout: 'horizontal',
//    align: 'left',
//    verticalAlign: 'top',
//      itemStyle: {
//         font: '9pt Trebuchet MS, Verdana, sans-serif',
//         color: '#A0A0A0'
//      },
//      itemHoverStyle: {
//         color: '#FFF'
//      },
//      itemHiddenStyle: {
//         color: '#444'
//      }
//   },
   credits: {
      style: {
         color: '#666'
      }
   },
   labels: {
      style: {
         color: '#CCC'
      }
   },


   navigation: {
      buttonOptions: {
         symbolStroke: '#DDDDDD',
         hoverSymbolStroke: '#FFFFFF',
         theme: {
            fill: {
               linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
               stops: [
                  [0.4, '#606060'],
                  [0.6, '#333333']
               ]
            },
            stroke: '#000000'
         }
      }
   },

   // scroll charts
   rangeSelector: {
      buttonTheme: {
         fill: {
            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
            stops: [
               [0.4, '#888'],
               [0.6, '#555']
            ]
         },
         stroke: '#000000',
         style: {
            color: '#CCC',
            fontWeight: 'bold'
         },
         states: {
            hover: {
               fill: {
                  linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                  stops: [
                     [0.4, '#BBB'],
                     [0.6, '#888']
                  ]
               },
               stroke: '#000000',
               style: {
                  color: 'white'
               }
            },
            select: {
               fill: {
                  linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                  stops: [
                     [0.1, '#000'],
                     [0.3, '#333']
                  ]
               },
               stroke: '#000000',
               style: {
                  color: 'yellow'
               }
            }
         }
      },
      inputStyle: {
         backgroundColor: '#333',
         color: 'silver'
      },
      labelStyle: {
         color: 'silver'
      }
   },

   navigator: {
      handles: {
         backgroundColor: '#666',
         borderColor: '#AAA'
      },
      outlineColor: '#CCC',
      maskFill: 'rgba(16, 16, 16, 0.5)',
      series: {
         color: '#7798BF',
         lineColor: '#A6C7ED'
      }
   },

   scrollbar: {
      barBackgroundColor: {
            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
            stops: [
               [0.4, '#888'],
               [0.6, '#555']
            ]
         },
      barBorderColor: '#CCC',
      buttonArrowColor: '#CCC',
      buttonBackgroundColor: {
            linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
            stops: [
               [0.4, '#888'],
               [0.6, '#555']
            ]
         },
      buttonBorderColor: '#CCC',
      rifleColor: '#FFF',
      trackBackgroundColor: {
         linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
         stops: [
            [0, '#000'],
            [1, '#333']
         ]
      },
      trackBorderColor: '#666'
   },

   // special colors for some of the
   legendBackgroundColor: 'rgba(0, 0, 0, 0.5)',
   legendBackgroundColorSolid: 'rgb(35, 35, 70)',
   dataLabelsColor: '#444',
   textColor: '#C0C0C0',
   maskColor: 'rgba(255,255,255,0.3)'
};

// Apply the theme
var highchartsOptions = Highcharts.setOptions(Highcharts.theme);
</script>

<style>
    td .red:hover{
                    cursor: pointer;
                    background-color: #FF6A6A;
    }

    td .green:hover{
        cursor: pointer;
        background-color: #7CCD7C;
    }

    td .red{
        background-color: #FFC1C1;
    }

    td .green{
        background-color:#90EE90;
    }

    td .yellow{
        background-color: #FFEC8B;
    }

    td .yellow:hover{
        cursor: pointer;
        background-color: #FFD700;
    }

    td .blue{
        background-color: #ADD8E6;
    }

    td .blue:hover{
        cursor: pointer;
        background-color: #00BFFF;
    }


</style>
<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem" style="margin-top: 10px;">
    <?php if ($aEntidades): ?>
        <thead>
            <tr align="center">
                <td onclick='javascript:ordenarNome("<?php echo ($_GET['ordem_nome'] == 'DESC')? 'ASC' : 'DESC' ?>")' onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="font-weight: bold; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                    <?php if($_GET['ordem_nome']): ?>
                        <img width="11" height="13" align="middle" src="../imagens/seta_ordem<?php echo $_GET['ordem_nome'] ?>.gif">
                    <?php endif ?>
                    Entidade
                </td>
                <?php foreach ($aAno as $ano): ?>
                    <td onclick='javascript:ordenarValor("<?php echo ($_GET['ordem_valor'] == 'DESC')? 'ASC' : 'DESC' ?>")' onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="font-weight: bold; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; width: 15%;" class="title">
                    <?php if($_GET['ordem_valor']): ?>
                        <img width="11" height="13" align="middle" src="../imagens/seta_ordem<?php echo $_GET['ordem_valor'] ?>.gif">
                    <?php endif ?>
                        <?php echo $ano; ?>
                    </td>
                <?php endforeach ?>
            </tr>
        </thead>
        <tbody>
            <?php
            $count = 0;
            $aPreenchimento = getPreenchimentoFisico($anoSelecionado);

            foreach ($aEntidades as $entnome => $aDados):
                $count++;
                $complemento = ($count%2) ? 'bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';"' : 'bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';"'; ?>
                <tr class="list" <?php echo $complemento; ?>>
                    <td><?php echo $entnome; ?></td>
                    <?php foreach ($aAno as $ano):
                        $class = '';
                        switch (substr($aDados[$ano]['status'], 0, 1)) {
                            case (1): $class = 'red';       break;
                            case (2): $class = 'yellow';    break;
                            case (3): $class = 'blue';      break;
                            case (4): $class = 'green';     break;
                        }

                        $classPreenchimento = '';
                        $titlePreenchimento = '';

                        $percentual = 0;
                        if(!isset($aPreenchimento[$aDados[$ano]['entcodigo']])){
                            $percentual = '-';
                        } elseif ($aPreenchimento[$aDados[$ano]['entcodigo']] && $aPreenchimento[$aDados[$ano]['entcodigo']][1] &&  $aPreenchimento[$aDados[$ano]['entcodigo']]['qtd']){
                            $percentual = ($aPreenchimento[$aDados[$ano]['entcodigo']][1]*100)/$aPreenchimento[$aDados[$ano]['entcodigo']]['qtd'];
                        }


                        if(!$percentual){
                            $classPreenchimento = 'red';
                            $titlePreenchimento = 'Nenhum dado físico preenchido';
                        } elseif ($percentual == '100'){
                            $classPreenchimento = 'green';
                            $titlePreenchimento = 'Dados físicos totalmente preenchidos';
                        } elseif ($percentual == '-'){
                            $classPreenchimento = '';
                            $titlePreenchimento = 'Cadastramento não iniciado';
                        } else {
                            $classPreenchimento = 'yellow';
                            $titlePreenchimento = 'Dados físicos parcialmente preenchidos';
                        }

                        if($percentual == '100' || $percentual == '-'){
                            $onclick = "onclick=\"javascript:msg('', 'Não existe nenhuma pendência de preenchimento físico nesta entidade!')\"";
                        } else {
                            $onclick = "onclick=\"javascript:mostrarRestantes({$aDados[$ano]['entcodigo']}, {$aDados[$ano]['ano']});\"";
                        }
                        ?>

                        <td align="right" class="<?php echo $class; ?>">
                            <div <?php echo $onclick ?> style="width: 29%; float:left;" class="<?php echo $classPreenchimento; ?> div-preenchimento" entcodigo="<?php echo $aDados[$ano]['entcodigo']; ?>" title="<?php echo $titlePreenchimento;?>">
                                <?php echo is_numeric($percentual) ? round($percentual, 2) . ' %' : $percentual;?>
                            </div>
                            <div style="width: 70%" onclick="javascript:consultar(<?php echo $aDados[$ano]['entcodigo']?>, <?php echo $aDados[$ano]['ano']?>);"><?php echo array_key_exists($ano, $aDados) ? number_format($aDados[$ano]['total'], 2, ',', '.') : ' - '; ?></div>
                            <div style="clear: both;"></div>
                        </td>
                    <?php endforeach ?>
                </tr>
            <?php endforeach ?>
        </tbody>
    <?php else: ?>
        <tr>
            <td style="background-color: #ffffcc; text-align: center; color: red;" >Não possui dados!</td>
        </tr>
    <?php endif ?>
</table>
<div id="dialog-preenchimento-fisico"></div>
<br />
<table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <th colspan="2" style="text-align: left;">Notas:</th>
    </tr>
    <tr>
        <td align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;width:50%">
            <?php echo textoUltimaCargaSIAFI(); ?>
        </td>
        <td>&nbsp;</td>
    </tr>
</table>

<script lang="javascript">

    function mostrarRestantes(codigoEntidade, ano)
    {
        var data = {action: 'mostrar_restantes', codigo_entidade: codigoEntidade, ano : ano};
        modalAjax( 'Listagem de pendências de preenchimento dos dados físicos: ', data, '' , 'true');
    }

    function ordenarNome(ordem)
    {
        window.location.href = "/pes/pes.php?modulo=principal/acompanhamento/painelgerencial&acao=A&ordem_nome=" + ordem;
    }

    function ordenarValor(ordem)
    {
        window.location.href = "/pes/pes.php?modulo=principal/acompanhamento/painelgerencial&acao=A&ordem_valor=" + ordem;
    }

//    jQuery(function(){
//    	jQuery(".div-preenchimento").click(function(){
//        	jQuery("#dialog-preenchimento-fisico").dialog({
//        	      resizable: false,
//        	      height:140,
//        	      modal: true,
//        	      buttons: {
//        	        Fechar: function() {
//        	          jQuery('#dialog-preenchimento-fisico').dialog( "close" );
//        	        }
//        	      }
//    	    });
//	    });
//    });

    function consultar(codigoEntidade, ano){
        window.location.href = '/pes/pes.php?modulo=principal/acompanhamento/consultar&acao=A&codigo_entidade=' + codigoEntidade + '&ano=' + ano;
    }
</script>
