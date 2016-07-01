<?php

header("Content-Type: text/html; charset=ISO-8859-1",true);

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "
select count(*) valor, substring(s.sisabrev, 1, 25) as descricao
from seguranca.usuariosonline u
        inner join seguranca.sistema s on s.sisid = u.sisid
group by descricao
order by valor desc
";
$dados = $db->carregar( $sql, null, 3200 );
geraGraficoColuna($dados, 'graficoColunaUsuariosOnline');



function geraGraficoColuna(array $dados,$nomeUnico,$titulo = '',$formatoDica = '',$formatoValores = array(),$nomeSerie = '',$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $altura = '600')
{
    foreach ($dados as $dado)
    {
        $data      .= $dado['valor'].", ";
        $categoria .= "'".$dado['descricao']."',";
    }
    $strValores = trim($categoria,",");

    ?>
    <script>

        jQuery(function () {


            Highcharts.getOptions().colors = Highcharts.map(
                [
                      '#00BFFF' // Azul claro
                    , '#55BF3B' // Verde
                    , '#FFD700' // Amarelo
                    , '#FF6A6A' // Vermelho claro
                    , '#eeaaee' // Rosa claro
                    , '#aaeeee' // Cinza claro
                    , '#7798BF' // Roxo claro
                    , '#DDDF0D' // Verde claro
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro
                    , '#008000' // Verde
                    , '#CD0000' // Vermelho
                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                ]
                , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                })


            jQuery('#<?php echo $nomeUnico; ?>').highcharts({
                chart: {
                    type: 'bar',
                    plotBackgroundColor: null,
                    // margin: [ 50, 50, 100, 80]
                    plotBorderWidth: null,
                    plotShadow: true,
                    backgroundColor:'rgba(255, 255, 255, 0.0)',
                },

//        chart: {
//        },

                title: {
                    text: '',
                },
                //habilitar o botão de salvar como imagem, pdf, etc
                exporting: {
                    enabled: false
                },
                credits: {
                    enabled: false
                },
                xAxis: {
                    categories: [<?php echo $categoria; ?>],
                    labels: {
//                        rotation: -45,
//                        align: 'right',
                        style: {
                            color: '#000',
                            fontSize: '10px',
                        }
                        <?php if ($formatoValores['x']) {
                            echo $formatoValores['x'];
                        } ?>
                    }
                },
                yAxis: {
                    title: {
                        text: ''
                    },
                    labels: {
                        style: {
                            color: '#fff',
                        }
                        <?php if ($formatoValores['y']) {
                            echo $formatoValores['y'];
                        } ?>
                    }
                },
                legend: {
                    enabled: false
                },
                tooltip: {
                    <?php if ($formatoValores['tooltip']) {
                        echo $formatoValores['tooltip'];
                    } else {
                        echo "pointFormat: 'Ocorrências: <b>{point.y:.2f}</b>',";
                    } ?>
                },
                series: [{

                    <?php if($caminhopopupdetalhes){ ?>
                    events: {
                        click: function(event) {
                            var arrValores = new Array (<?=$strValores?>);

                            var x = screen.width/2 - 700/2;
                            var y = screen.height/2 - 450/2;

                            var janela = window.open('<?php echo $caminhopopupdetalhes; ?>&parametro='+arrValores[event.point.x],'winpreobra','menubar=0,scrollbars=yes,width=<?=$largurapopupdetalhes?>,height=<?=$alturapopupdetalhes?>,left='+x+',top='+y);
                            janela.focus();


                        }
                    },
                    <?php } ?>

                    borderWidth: 0,
                    name: 'Population',
                    format: '{point.percentage:.2f} %',
                    colorByPoint: true,
                    data: [<?php echo $data; ?>],
                    dataLabels: {
                        enabled: true,
//                rotation: -90,
//                        color: '#FFFFFF',
                        align: 'right',
                        x: 15,
//                        y: 15,
                        style: {
                            fontSize: '9px',
                            fontFamily: 'Verdana, sans-serif',
                            textShadow: '0 0 3px black'
                        }
                    }
                }]
            });
        });

    </script>
    <div style="height: 600px" id="<?=$nomeUnico?>" ></div>
<?
}