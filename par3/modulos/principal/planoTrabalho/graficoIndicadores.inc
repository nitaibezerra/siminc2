<?php


function indicadorSimec($idIndicador, $idIndicador2, $tipo)
{
    $painel = new Painel_Model_Seriehistorica();
    if ($tipo == "grafico") {

        $dados = $painel->carregarGraficoIndicador($idIndicador);
        $dados2 = $painel->carregarGraficoIndicador($idIndicador2);
        $indicador1 = $painel->detalhamentoIndicadorfunction($idIndicador);
        $indicador2 = $painel->detalhamentoIndicadorfunction($idIndicador2);
        ?>

        <script>
            $(document).ready(function () {
                var data2 = [<?php
                    foreach( $dados as $id=>$key){
                        echo "[".$id.",".$key['total']."]";
                        if(count($dados)-1 != $id){
                                    echo ",";
                        }
                    }
                    ?>];

                var data3 = [
                    <?php
                        foreach( $dados2 as $id=>$key){
                            echo "[".$id.",".$key['total']."]";
                            if(count($dados2)-1 != $id){
                                echo ",";
                            }
                        }
                    ?>];
                var dataset = [
                    {
                        label: "<?php echo $indicador1[0]['indnome']?>",
                        data: data2,
                        color: "#1ab394",
                        bars: {
                            show: true,
                            align: "center",
                            barWidth: 0.2,
                            lineWidth: 0
                        }

                    },

                    {
                        label: "<?php echo $indicador2[0]['indnome']?>",
                        data: data3,
                        color: "#464f88",
                        bars: {
                            show: true,
                            align: "center",
                            barWidth: 0.2,
                            lineWidth: 0
                        }

                    }
                ];


                var ticks = [<?php foreach( $dados as $id=>$key){
                        echo "[".$id.",\"".$key['tipo']."\"]";
                        if(count($dados)-1 != $id){
                                    echo ",";
                        }
                        }?>];
                var options = {
                    xaxis: {
                        // mode: "time",
                        ticks: ticks,
                        //tickSize: [3, "day"],
                        //tickLength: 0,
                        // axisLabel: "Date",
                        axisLabelUseCanvas: true,
                        axisLabelFontSizePixels: 12,
                        axisLabelFontFamily: 'Arial',
                        axisLabelPadding: 10,
                        color: "#d5d5d5"
                    },
                    yaxes: [{

                        axisLabelPadding: 3
                    }
                    ],
                    legend: {
                        noColumns: 1,
                        labelBoxBorderColor: "#000000",
                        position: "nw"
                    },
                    grid: {
                        hoverable: false,
                        borderWidth: 0
                    }
                };


                $.plot($("#flot-dashboard-chart"), dataset, options);


            });
        </script>

    <?
    }
    if ($tipo == "tabela") {
        $dados3 = $painel->carregarTabelaIndicadoresVinculados($idIndicador, $idIndicador2);
        ?>
        <div class="clearfix"></div>
        <div class="row" style="margin-top: 40px;">
            <div class="col-md-12">
                <div class="ibox-title esconde " tipo="integrantes">
                    <h3>Tabela do Indicador</h3>
                </div>
                <div class="ibox float-e-margins" style="margin: 25px;">
                    <div class="ibox-content">

                        <table class="table table-hover dataTable">
                            <thead>
                            <tr>
                                <th>Escola</th>
                                <th>Equipamento</th>
                                <th>Quantidade existente do equipamento</th>
                                <th>Demanda do equipamento</th>
                            </tr>
                            </thead>

                            <?php
                            foreach ($dados3 as $key) {
                                ?>
                                <tr>
                                    <td><?php echo $key['escola'] ?></td>
                                    <td><?php echo $key['tipo'] ?></td>
                                    <td><?php echo $key['totalequipamento'] ?></td>
                                    <td><?php echo $key['totaldemanda'] ?></td>
                                </tr>
                            <?php } ?>
                        </table>
                    </div>
                </div>

            </div>
        </div>



    <?php
    }
}

$iue = new Par3_Model_InstrumentoUnidade ($_REQUEST['inuid']);
function remove_acentos($str)
{
    $str = trim($str);
    $str = strtr($str, "¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ!@#%&*()[]{}+=?",
        "YuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy_______________");
    $str = str_replace("..", "", str_replace("/", "", str_replace("\\", "", str_replace("\$", "", $str))));
    return str_replace(array(' ', ':'), array('-', ''), strtolower($str));
}


?>

<div class="row">
    <div class="col-md-2">
        <div
            style="background: rgb(164, 170, 175);height: 200px;width: 200px;text-align: center;color: #fff;padding: 10px">
            <span style="font-size: 16px"><? echo $iue->inudescricao ?></span>
            <br>
            <?php echo $iue->estuf ?>
            <br>
            <br>

            <?php if($iue->itrid == 2){?>
                <img src="http://www.mbi.com.br/mbi/files/media/image/brasil-municipio/<?php echo strtolower($iue->estuf) ?>-<?php echo strtolower(remove_acentos($iue->inudescricao)) ?>-bandeira.jpg" style="width: 120px;">
             <?php } else{?>
            <img src="..\imagens\bandeiras\bandeira_<?php echo $iue->estuf?>.jpg" style="width: 120px;">
            <?php } ?>

            <br>
            <br>


        </div>
    </div>
    <div class="col-md-10">
        <div class="flot-chart" style="margin: 10px;">
            <h3>
                <?php
                $painel = new Painel_Model_Seriehistorica();
                $indicador1 = $painel->detalhamentoIndicadorfunction(3205);
                //  ver($indicador1,d);
                echo $indicador1[0]['indnome'];
                echo " x ";
                $painel = new Painel_Model_Seriehistorica();
                $indicador2 = $painel->detalhamentoIndicadorfunction(3272);
                echo $indicador2[0]['indnome'];
                ?>
            </h3>

            <div class="flot-chart-content" id="flot-dashboard-chart"></div>
        </div>
    </div>


    <?php
    indicadorSimec(3205, 3272, 'grafico');
    indicadorSimec(3205, 3272, 'tabela');


    ?>
    <style>
        .x1Axis .tickLabel {
            font-size: 11px;
            color: #000;
            width: 100px;
        }
    </style>


