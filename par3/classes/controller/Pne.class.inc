<?php
/**
 * Classe de controle do PNE
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_Pne
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */
class Par3_Controller_Pne extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função desenharTela
     * - desenha a tela da unidade de acordo com o menu a ser preenchido
     *
     * @return escreve a tela.
     *
     */
    public function desenharTela($menu)
    {
        $pastaPne = 'par3/modulos/principal/planoTrabalho/pne/';

        $aba = 'possuiDiagnostico';

        if (0 < $menu && $menu < 21) $aba = 'meta';

        switch($aba){
        	case 'meta':
        	    include APPRAIZ.$pastaPne.'meta.php';
        	    break;
        	case 'possuiDiagnostico':
        	default:
        	    include APPRAIZ.$pastaPne.'possuiDiagnostico.php';
        	    break;
        }

    }//end desenharTela();


    /**
     * Função situacaoPne
     * - retorna situacao PNE
     *
     * @return escreve a tela.
     *
     */
    public function situacaoPne($inuid)
    {
        $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
        $controllerPne   = new Par3_Controller_Pne();

        if ($modelUnidade->itrid === '2') {
            require_once APPRAIZ.'sase/classes/Assessoramento.class.inc';
            $assessoramento = new Assessoramento();
            return $assessoramento->pegaDscSituacao($modelUnidade->muncod);
        } else {
            require_once APPRAIZ.'sase/classes/AssessoramentoEstado.class.inc';
            $assessoramento = new AssessoramentoEstado();
            return $assessoramento->pegaDscSituacao($modelUnidade->estuf);
        }
    }//end situacaoPne();


    /**
     * Função preenchimentoPossuiDiagnostico
     * - retorna icone de preenchimento do menu possui diagnostico
     *
     * @return string.
     *
     */
    public function preenchimentoPossuiDiagnostico($inuid)
    {
        $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
        $controllerPne   = new Par3_Controller_Pne();

        if ($modelUnidade->itrid === '2') {
            require_once APPRAIZ.'sase/classes/Assessoramento.class.inc';
            $assessoramento = new Assessoramento();
            $stacod         = $assessoramento->pegaStacod($modelUnidade->muncod);
        } else {
            require_once APPRAIZ.'sase/classes/AssessoramentoEstado.class.inc';
            $assessoramento = new AssessoramentoEstado();
            $stacod         = $assessoramento->pegaDscSituacao($modelUnidade->estuf);
        }

        $iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
        $iconPreenchido    = '<span class="fa fa-check iconUnidade"'.
                                    'style="color:green !important;'.
                                    'background-color: transparent;"></span>';

        if ($stacod > 3) {
            return $iconPreenchido;
        } else {
            return $iconNaoPreenchido;
        }
    }//end preenchimentoPossuiDiagnostico();


    /**
     * Função retornaArrayMetas
     * - retorna array de metas
     *
     * @return array.
     *
     */
    public function retornaArrayMetas()
    {
        require_once APPRAIZ.'sase/classes/Metas.class.inc';
        $metas = new Metas();

        return $metas->retornaArrayListaGeral();
    }//end retornaArrayMetas();


    /**
     * Função camposComparacaoMetasPNE
     * - retorna array de metas
     *
     * @return array.
     *
     */
    public function camposComparacaoMetasPNE($arrPost)
    {
        global $simec;

        $sql = Territorios_Model_Regiao::pegarSQLSelect($arrPost);
        echo '  <div class="panel panel-default">
                    <div class="panel-heading"><b>Região</b></div>
                    <div class="panel-body">';
        echo $simec->select('regcod[]', '', $arrPost['regcod'], $sql, $arrAttr);
        echo '      </div>
                </div>';

        $sql = Territorios_Model_Estado::pegarSQLSelect($arrPost);
        echo '  <div class="panel panel-default">
                    <div class="panel-heading"><b>UF</b></div>
                    <div class="panel-body">';
        echo $simec->select('estuf[]', '', $arrPost['estuf'], $sql, $arrAttr);
        echo '      </div>
                </div>';

        $sql = Territorios_Model_Mesoregiao::pegarSQLSelect($arrPost);
        echo '  <div class="panel panel-default">
                    <div class="panel-heading"><b>Mesoregião</b></div>
                    <div class="panel-body">';
        echo $simec->select('mescod[]', '', $arrPost['mescod'], $sql, $arrAttr);
        echo '      </div>
                </div>';
    }//end camposComparacaoMetasPNE();


    /**
     * Função geraGraficoPNE
     * - retorna graficoPNE
     *
     * @return Grafico.
     *
     */
    function geraGraficoPNE($nomeUnico, $dados)
    {
        $meta = $dados['metaBrasil'];
        $metaTotal = $dados['metaTotal'];
        $metaFormatada = number_format($meta, 0, ',', '.');

        $complemento = $simbolo = '';

        $casasDecimais = 0;

        switch ($dados['tipo']) {
        	case ('P'):
        	    $complemento = $simbolo = '%';
        	    $metaFormatada = $meta;
        	    $casasDecimais = 1;
        	    break;
        	case ('A'):
        	    $complemento = ' anos';
        	    /**
        	     * Victor Martins Machado
        	     * Incluído para mostrar o valor com uma casa decimal
        	     */
        	    $metaFormatada = $meta;
        	    $casasDecimais = 1;
        	    break;
        	case ('M'):
        	    $complemento = ' matrículas';
        	    break;
        	case ('T'):
        	    $complemento = ' títulos';
        	    break;
        }

        $formatter = "function () { return '<div style=\"text-align:center\"><span style=\"font-size:20px;color:black; font-weight: normal;\">' + number_format(this.y, " . $casasDecimais .", ',', '.') + '" . $simbolo ."</span><br/>' +
                                       '<span style=\"font-size:13px;color:black; font-weight: normal;\">" . $dados['descricao'] . "</span></div>'; }";

        ?>

            <script type="text/javascript">
                jQuery(function () {

                    var gaugeOptions = {

                        chart: {
                            type: 'solidgauge'
                        },

                        title: null,


                        credits: {
                            enabled: false
                        },
                        exporting: {
                            enabled: false
                        },

                        credits: {
                            enabled: false
                        },
                        exporting: {
                            enabled: false
                        },

                        pane: {
                            center: ['50%', '85%'],
                            size: '120%',
                            startAngle: -90,
                            endAngle: 90,
                            background: {
                                backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                                innerRadius: '60%',
                                outerRadius: '100%',
                                shape: 'arc'
                            }
                        },

                        tooltip: {
                            enabled: false
                        },

                        // the value axis
                        yAxis: {
                            stops: [
                                [0, '<?php echo $dados['cor']; ?>']
                            ],
                            lineWidth: 0,
                            minorTickInterval: null,
                            tickPixelInterval: 400,
                            tickWidth: 0,
                            title: {
                                y: -55
                            },
                            labels: {
                                enabled: false
                            },
                            plotBands: [{
                                from: 0,
                                to: parseFloat('<?php echo $meta; ?>'),
                                color: '<?php echo $dados['plotBandsCor'] == '' ? '#726D70' : $dados['plotBandsCor']; ?>',
                                innerRadius: '100%',
                                outerRadius: '<?php echo $dados['plotBandsOuterRadius'] == '' ? '110%' : $dados['plotBandsOuterRadius']; ?>'
                            }]
                        },

                        plotOptions: {
                            solidgauge: {
                                dataLabels: {
                                    y: 5,
                                    borderWidth: 0,
                                    useHTML: true
                                }
                            }
                        }
                    };

                    // The speed gauge
                    jQuery('#<?=$nomeUnico?>').highcharts(Highcharts.merge(gaugeOptions, {
                        yAxis: {
                            min: 0,
                            max: parseFloat('<?php echo $metaTotal; ?>'),
                            title: {
                                text: '<span style="color:black;"><?php echo $dados['title'] == '' ? 'Meta Brasil:' : $dados['title']; ?> <?php echo $metaFormatada . $complemento; ?><?php echo $dados['anoprevisto'] != '' ? ' - '.$dados['anoprevisto'] : ''; ?></span>'
                            }
                        },

                        credits: {
                            enabled: false
                        },

                        series: [{
                            name: 'Meta',
                            data: [0],
                            dataLabels: {
                                y: 25,
                                formatter: <?php echo $formatter; ?>
                            },
                            tooltip: {
                                valueSuffix: ' '
                            }
                        }]

                    }));

                    // Bring life to the dials
                    var chart = $('#<?=$nomeUnico?>').highcharts();
                    if (chart) {
                        var point = chart.series[0].points[0],
                            newVal = parseFloat('<?php echo $dados['valor']; ?>');
                        point.update(newVal);
                    }
                });
            </script>
            <div id="<?=$nomeUnico?>" style="width: 200px; height: 150px; float: left"></div>
<?php
    }//end geraGraficoPNE()


}//end Class
?>