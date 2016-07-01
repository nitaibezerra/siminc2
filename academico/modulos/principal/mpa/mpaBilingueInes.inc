<?PHP

    if ($_REQUEST['req']) {
        $_REQUEST['req']();
        die();
    }

    $perfis = pegaPerfilGeral();

    $habilita = 'N';
    if (in_array(PERFIL_ADMINISTRADOR, $perfis) || $db->testa_superuser()) {
        $habilita = 'S';
    };

    function buscaDadosGrafico(){
        global $db;
        
        $sql = "
            SELECT  estuf as uf_descricao,
                    inbnumvagas as numeros_vaga
            FROM academico.inesbilingue
            ORDER BY inbid, estuf
        ";
        $dados = $db->carregar($sql);
        foreach ($dados as $i => $item){
            if( $i == 0){
                $array_valores .= "{ name: '".$item['uf_descricao']."', y: ".$item['numeros_vaga'].", sliced: true, selected: true },";
            }else{
                $array_valores .= '[\''.$item['uf_descricao'].'\','.$item['numeros_vaga'].']';
            }
        }
        $array_valores = str_replace('][', '],[', $array_valores);
        return  $array_valores;
        die;
    }

    function salvar() {
        global $db;

        if (is_array($_POST['vaga'])) {
            $sql = '';
            foreach ($_POST['vaga'] as $uf => $valor) {
                $valor = $valor ? $valor : 'NULL';
                $sql .= "
                    UPDATE academico.inesbilingue 
                        SET inbnumvagas = $valor
                    WHERE estuf = '$uf';
                ";
            }

            if ($sql != '') {
                $db->executar($sql);
                $db->commit();
            }
        }
        echo "
            <script>
                    alert('Dados Salvos com sucesso!');
                    window.location.href = window.location.href; 
            </script>
        ";
    }

    monta_titulo('Bilingues-INES', '');
    echo"<br>";
    
?>

<style>
    .imgbot{
        cursor:pointer;
    }
</style>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>


<script>
    
    $(document).ready(function() {
        $('.salvar').click(function() {
            $('#req').val('salvar');
            $('#formulario').submit();
        });
    });
    
    $(function () {
        
        <? $array_valores = buscaDadosGrafico(); ?>
      
        //Radialize the colors
        Highcharts.getOptions().colors = Highcharts.map(Highcharts.getOptions().colors, function(color) {
            return {
                radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                stops: [
                    [0, color],
                    [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                ]
            };
        });
        
        //Build the chart
        $('#container').highcharts({ 
            lang: {
                printChart: 'Imprimir',
                downloadPDF: 'Exportar em PDF',
                downloadJPEG: 'Exportar em JPG',
                downloadPNG: 'Exportar em PNG',
                downloadSVG: 'Exportar em SVG',
                decimalPoint: ',',
                thousandsSep: '.'
            },
            credits: {
		enabled: false
            },
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: false
            },
            title: {
                text: 'Números de vagas por Estados.'
            },
            tooltip: {
                 pointFormat: '<b>{series.name}<b>: <b>{point.percentage:.2f}%</b>'
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: true,
                        color: '#000000',
                        connectorColor: '#000000',
                        format: '<b>{point.name}</b>: {point.percentage:.2f} %'
                    }
                }
            },
            series: [{
                type: 'pie',
                name: 'Percentual de Vagas',
                data: [ <?PHP echo $array_valores; ?> ]
                //data: [ ['Firefox', 45], ['IE', 26], { name: 'Chrome', y: 12, sliced: true, selected: true }, ['Safari', 8], ['Opera', 6], ['Others', 1] ]
            }]
        });
    });
    
    
</script>

<form id="formulario" name="formulario" method="post" action="">
    <input type="hidden" value="" name="req" id="req" />
    <input type="hidden" value="" name="idaid" id="idaid" />
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <tr>
            <td width="40%">
               <table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem" style="color:333333;"><tr>
                        <? $cabecalho = Array('INSTITUIÇÃO FORMADORA', 'CURSOS', 'MODALIDADE', 'ESTADO', 'VAGAS'); ?>
                    <tr>
                        <? foreach ($cabecalho as $cab) { ?>
                            <td align="center" valign="top" class="TituloTela" style="font-size: 11px; color:#000000;">
                                <?= $cab ?>
                            </td>
                        <? } ?>
                    </tr>
                    <?PHP
                        switch ($_SESSION['academico']['orgid']) {
                            case '1':
                                $funid = ACA_ID_UNIVERSIDADE;
                                break;
                        }

                        $sql = "
                            SELECT  inbid as codigo,
                                    estuf as uf,
                                    inbnumvagas as vaga
                            FROM academico.inesbilingue
                            ORDER BY inbid, estuf
                        ";
                        $ufs = $db->carregar($sql);
                    ?>
                    <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                        <td rowspan="<?= count($ufs); ?>" align="center" valign="center" class="TituloTela" style="font-size: 11px; color:#000000;">INES</td>
                        <td rowspan="<?= count($ufs); ?>" align="center" valign="center" class="TituloTela" style="font-size: 11px; color:#000000;">Pedagogia na <br/> Perspectiva <br/> Bilíngue</td>
                        <td rowspan="<?= count($ufs); ?>" align="center" valign="center" class="TituloTela" style="font-size: 11px; color:#000000;">Distância</td>

                    <?PHP
                        if (is_array($ufs)) {
                            foreach ($ufs as $uf) {
                    ?>
                        <td align="center" style="font-weight:bold;">
                            <?= $uf['uf']; ?>
                        </td>
                        <td align="center">
                            <?= campo_texto("vaga[{$uf['uf']}]", 'N', $habilita, '', 3, 3, '###', '', '', '', '', 'id="vaga' . $uf['uf'] . '"', '', $uf['vaga']); ?>
                        </td>
                    </tr>
                    <?PHP
                        }
                    }
                    ?>
                </table>
            </td>
            <td align="right" >
                <div id="container" style="min-width: 350px; height: 350px; margin: 0 auto;"></div>
            </td>
        </tr>
        <tr>
            <td  colspan="2" align="right" >Fonte:DIFES/SESU</td>
        </tr>
        <tr>
            <td align="left" colspan="2">
                <input type="button" class="salvar" style="cursor:pointer" value="Salvar">
            </td>
        </tr>
    </table>
</form>