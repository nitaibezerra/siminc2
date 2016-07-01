<?php

/*
 * Classe Grafico
 * Classe para gerar gráficos
 * @author Orion Teles de Mesquita
 * @since 10/04/2014
 */
class GraficoPAR {

    /**************************
     *        CONSTANTES       *
     **************************/
    // Tipo de Gráfico
    const K_TIPO_PIZZA  = 'P';
    const K_TIPO_AREA   = 'A';
    const K_TIPO_LINHA  = 'L';
    const K_TIPO_BARRA  = 'B';
    const K_TIPO_COLUNA = 'C';

    // Formatos
    const K_DECIMAL_0 = "function () { return number_format(this.value, 0, ',', '.'); }";
    const K_DECIMAL_2 = "function () { return number_format(this.value, 2, ',', '.'); }";
    const K_TOOLTIP_DECIMAL_0 = "function() { return '<span>' + this.x + '</b><br /><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }";
    const K_TOOLTIP_DECIMAL_2 = "function() { return '<span>' + this.x + '</b><br /><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }";
    const K_TOOLTIP_PIE_DECIMAL_0 = "function() { return '<b>' + this.point.name + '</b><br />Valor: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + this.point.percentage.toFixed(2) + '%</b>'; }";
    const K_TOOLTIP_PIE_DECIMAL_2 = "function() { return '<b>' + this.point.name + '</b><br />Valor: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + number_format(this.point.percentage, 2, ',', '.') + '%</b>'; }";

    /**************************
     *        ATRIBUTOS        *
     **************************/
    private $tipo;
    private $id;
    private $dados;
    private $height;
    private $titulo;
    private $subtitulo;
    private $agrupamentoManual;
    private $label_xAxis;
    private $arrLabel_yAxis;
    private $xAxis;
    private $yAxis;
    private $formatoX;
    private $formatoY;
    private $formatoPieLabel = ' R$ {y:,.2f} / {point.percentage:.2f}%</b>';
    private $formatoTooltip;
    private $labelX;
    private $agrupadores;
    private $legenda;
    private $colors;
    private $event;

    /**************************
     *        MÉTODOS          *
     **************************/
    public function __construct($tipo = self::K_TIPO_PIZZA , $incluirBiblioteca = false)
    {
        $this->resetValores();
        $this->tipo = $tipo;

        if($incluirBiblioteca){
            $this->incluirBiblioteca();
        }
    }


    /**************************
     *    GETTERS E SETTERS    *
     **************************/
    /**
     * @param boolean $agrupamentoManual
     */
    public function setAgrupamentoManual($agrupamentoManual)
    {
        $this->agrupamentoManual = $agrupamentoManual;
        return $this;
    }

    /**
     * @return boolean
     */
    public function getAgrupamentoManual()
    {
        return $this->agrupamentoManual;
    }

    /**
     * @param mixed $dados
     */
    public function setDados($dados)
    {
        $this->dados = $dados;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getDados()
    {
        return $this->dados;
    }

    /**
     * @param string $formatoTooltip
     */
    public function setFormatoTooltip($formatoTooltip)
    {
        $this->formatoTooltip = $formatoTooltip;
        return $this;
    }

    /**
     * @return string
     */
    public function getFormatoTooltip()
    {
        return $this->formatoTooltip;
    }

    /**
     * @param mixed $formatoX
     */
    public function setFormatoX($formatoX)
    {
        $this->formatoX = $formatoX;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getFormatoX()
    {
        return $this->formatoX;
    }

    /**
     * @param string $formatoY
     */
    public function setFormatoPieLabel($formato)
    {
        $this->formatoPieLabel = $formato;
        return $this;
    }

    /**
     * @return string
     */
    public function getFormatoPieLabel()
    {
        return $this->formatoPieLabel;
    }

    /**
     * @param string $formatoY
     */
    public function setFormatoY($formatoY)
    {
        $this->formatoY = $formatoY;
        return $this;
    }

    /**
     * @return string
     */
    public function getFormatoY()
    {
        return $this->formatoY;
    }

    /**
     * @param string $height
     */
    public function setHeight($height)
    {
        $this->height = $height;
        return $this;
    }

    /**
     * @return string
     */
    public function getHeight()
    {
        return $this->height;
    }

    /**
     * @param string $width
     */
    public function setWidth($width)
    {
        $this->width = $width;
        return $this;
    }

    /**
     * @return string
     */
    public function getWidth()
    {
        return $this->width;
    }

    /**
     * @param mixed $id
     */
    public function setId($id)
    {
        $this->id = $id;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * @param array $labelX
     */
    public function setLabelX($labelX)
    {
        $this->labelX = $labelX;
        return $this;
    }

    /**
     * @return array
     */
    public function getLabelX()
    {
        return $this->labelX;
    }

    /**
     * @param string $subtitulo
     */
    public function setSubtitulo($subtitulo)
    {
        $this->subtitulo = $subtitulo;
        return $this;
    }

    /**
     * @return string
     */
    public function getSubtitulo()
    {
        return $this->subtitulo;
    }

    /**
     * @param mixed $tipo
     *
     * Setar o tooltip sempre após o tipo, caso queira outro tooltip sem ser o padrão para cada tipo.
     */
    public function setTipo($tipo)
    {
        if($tipo != self::K_TIPO_PIZZA){
            $this->formatoTooltip = self::K_TOOLTIP_DECIMAL_2;
        }

        $this->tipo = $tipo;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getTipo()
    {
        return $this->tipo;
    }

    /**
     * @param string $titulo
     */
    public function setTitulo($titulo)
    {
        $this->titulo =  $titulo ;
        return $this;
    }

    /**
     * @return string
     */
    public function getTitulo()
    {
        return $this->titulo;
    }

    /**
     * @param array $agrupadores
     */
    public function setAgrupadores($agrupadores)
    {
        $this->agrupadores = $agrupadores;
        return $this;
    }

    /**
     * @return array
     */
    public function getAgrupadores()
    {
        return $this->agrupadores;
    }

    public function setConfigs($configs)
    {
        foreach($configs as $atributo => $valor){
            if (property_exists( get_class( $this ),$atributo )) {
                $this->{$atributo} = $valor;
            }
        }
        return $this;
    }

    /**
     * @param mixed $legenda
     */
    public function setLegenda($legenda)
    {
        $this->legenda = $legenda;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getLegenda()
    {
        return $this->legenda;
    }

    /**
     * @param mixed $colors
     */
    public function setColors($colors)
    {
        $this->colors = $colors;
        return $this;
    }

    /**
     * @return mixed
     */
    public function getColors()
    {
        return $this->colors;
    }

    /**
     * @return array
     */
    public function getEvent() {
        return $this->event;
    }

    /**
     * @param array $event
     * @return \Grafico
     */
    public function setEvent($event) {
        $this->event = $event;
        return $this;
    }

        
    public function gerarGrafico($dados, $extra = Array() )
    {
    	switch ($extra['tipo']){
    		case '3barras':
				$this->montar3Barras($dados, $extra);
				break;
    		default:
				$this->montarGraficoPizza($dados, $extra);
    			break;
    	}
    }

    public function montarGraficoPizza($dados, $extra = Array() )
    {
        $this->dados = $this->agrupamentoManual ? $dados : $this->agruparDadosGrafico($dados);
        if(!$this->id){
            $this->id = $this->gerarIdGrafico();
        }
        
        $cor = "	'#00BFFF' // Azul claro
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
                    , '#FFA500' // Amarelo quemado ";
        ?>
        <div style=" <?php echo ($this->width)? "width: {$this->width};": ''; ?>; height: <?php echo $this->height; ?> " id="<?php echo $this->id; ?>" ></div>
        <script type="text/javascript">
            jQuery(function () {

                // Radialize the colors
                Highcharts.getOptions().colors = Highcharts.map([ <?php echo $this->colors; ?> ] , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                });
                
                Highcharts.setOptions({
                    lang: {
                        decimalPoint: ',',
                        thousandsSep: '.'
                    }
                });
                
                var title     = '<?php echo $this->titulo; ?>';
                var subtitle  = '<?php echo $this->subtitulo; ?>';
                jQuery('#<?php echo $this->id; ?>').highcharts({
                    chart: {
                        type: '<?php echo $this->getTipoGrafico($this->tipo); ?>',
                        plotBackgroundColor: null,
                        plotBorderWidth: null,
                        plotShadow: true,
                        backgroundColor:'rgba(0, 0, 0, 0.0)'
                    },
                    credits: {
                        enabled: false
                    },
                    exporting: {
                        enabled: false
                    },
                    title: {
                        text: title
                    },
                    subtitle: {
                        text: subtitle
                    },
                    xAxis: {
                        categories: <?php echo json_encode($this->dados['categories']); ?>,

                        <?php if($this->labelX){ ?>
                        labels: <?php echo json_encode($this->labelX); ?>,
                        <?php } ?>

                        <?php if($this->formatoX){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoX; ?>,
                        },
                        <?php } ?>
                    },
                    yAxis: {
                        title: {
                            text: ''
                        },
                        <?php if($this->formatoY){ ?>
                        labels: {
                            formatter: <?php echo $this->formatoY; ?>
                        },
                        <?php } ?>
                        plotLines: [{
                            value: 0,
                            width: 1,
                            color: '#FFFFFF'
                           
                        }]
                    },
                    <?php if( $this->formatoTooltip ){?>
                    tooltip: {
                        formatter: <?php echo $this->formatoTooltip; ?>, 
						hideDelay: 1,
						followPointer: false
                    },
                    <?php }?>
					legend: {
		                itemStyle:{
		                	color: '#fff'
		                },
	                	itemHoverStyle:{
							color: '#FFFFCC'
		                }, 
		                layout:'horizontal',
		                align: 'center',
		                verticalAlign: 'bottom',
		                //backgroundColor: 'green',
		                borderColor: 'white',
		                borderRadius: 5,
		                borderWidth: 1
		            },
                    plotOptions: {
                        pie: {
                            allowPointSelect: true,
                            cursor: 'pointer',
                            dataLabels: {
                                enabled: true,
                                color: '#FFFFFF',
                                distance: 30,
                                formatter: function () { return this.point.name + ' (' + number_format(this.y, 0, ',', '.') + '/' + this.percentage.toFixed(2) + '%)'; },
                               	format: '<?=$this->formatoPieLabel ?>',
                                showInLegend: true
                            },
                            showInLegend: true
                            <?php if( $extra['onclickArea'] ){?>
                            ,events: {
                            	click: function(event){
								<?php 
                                	$strValores = '';
                                    if( is_array($dados) ){
    	                            	foreach($dados as $dado){
    	                                	$strValores .= "'".$dado['descricao']."',";
    	                            	}
                                    }
                                    $strValores = trim($strValores,",");
                                    ?>
                                	var arrValores = new Array (<?=$strValores?>); 
                                	<?=$extra['onclickArea']; ?>, arrValores[event.point.x]);
                                    }
                                }
                           <?php }?>
                        }
                    },
                    series: <?php echo json_encode($this->dados['series']); ?>
                });
            });
        </script>
        <?php
        // Apagando o valor do id em caso de inserir outro gráfico com id gerado automaticamente
        $this->resetValores();
    }

    public function montar3Barras($dados, $extra = Array() )
    {
    	$this->dados = $this->agrupamentoManual ? $dados : $this->agruparDadosGrafico($dados);
    	if(!$this->id){
    		$this->id = $this->gerarIdGrafico();
    	}
    	
    	$this->preparaIndicesDados($extra);
        $this->preparaDados($dados);
        
        $cor = "	'#00BFFF' // Azul claro
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
                    , '#FFA500' // Amarelo quemado ";
        
        ?>
        <div style=" <?php echo ($this->width)? "width: {$this->width};": ''; ?>; height: <?php echo $this->height; ?> " id="<?php echo $this->id; ?>" ></div>
        <script type="text/javascript">
	        
	        Highcharts.setOptions({
	            lang: {
	                decimalPoint: ',',
	                thousandsSep: '.',
					numericSymbols: ['Mil', 'Mi', 'Bi', 'Tri']
	            }
	        });
	        
	        var title     = '<?php echo $this->titulo; ?>';
	        var subtitle  = '<?php echo $this->subtitulo; ?>';
        
	        jQuery(function () {
	            jQuery('#<?php echo $this->id; ?>').highcharts({
	                chart: {
	                    type: 'column',
                        plotBackgroundColor: null,
                        plotBorderWidth: null,
                        plotShadow: true,
                        backgroundColor:'rgba(0, 0, 0, 0.0)'
	                },
                    title: {
                        text: title
                    },
                    subtitle: {
                        text: subtitle
                    },
                    credits: {
                        enabled: false
                    },
                    exporting: {
                        enabled: false
                    },
	                xAxis: {
	                    categories: [<?=$this->xAxis ?>],
	                    crosshair: true,
	                    labels: {
							style: {"color":"white"}
			            }
	                },
	                yAxis: {
	                    min: 0,
	                    title: {
	                        text: 'R $',
	                        style: {"color":"white"}
	                    },
	                    labels: {
							style: {"color":"white"}
			            }
	                },
	                tooltip: {
	                    headerFormat: '<span style="font-size:12px" ><b>{point.key}</b></span><table width=120px>',
	                    pointFormat: '<tr>'+
		                    				'<td style="color:{series.color};padding:0;">{series.name}: </td>' +
		                    			'</tr>' +
		                    			'<tr>' +
		                        			'<td style="padding:0"><b>R$ {point.y:,.2f}</b></td>'+
	                        			'</tr>',
	                    footerFormat: '</table>',
	                    shared: true,
	                    useHTML: true
	                },
					legend: {
		                itemStyle:{
		                	color: '#fff'
		                },
	                	itemHoverStyle:{
							color: '#FFFFCC'
		                }, 
		                layout:'horizontal',
		                align: 'center',
		                verticalAlign: 'bottom',
		                //backgroundColor: 'green',
		                borderColor: 'white',
		                borderRadius: 5,
		                borderWidth: 1
		            },
	                plotOptions: {
	                    column: {
	                        pointPadding: 0.2,
	                        borderWidth: 0
	                    }
	                },
	                series: [<?=$this->yAxis ?>]
	            });
	        });
        </script>
        <?php
        // Apagando o valor do id em caso de inserir outro gráfico com id gerado automaticamente
        $this->resetValores();
    }
    
    public function preparaIndicesDados( $extra ){

		$this->label_xAxis = $extra['xAxis'];
		$this->arrLabel_yAxis = $extra['yAxis'];
    }
    
    public function preparaDados( $dados ){
    	
		$this->montaXAxis($dados);
		$this->montaYAxis($dados);
    }
    
    public function montaXAxis( $dados ){
    	
		$indices = Array();
		if( is_array($dados) ){
			foreach( $dados as $dado ){
				$indices[] = $dado[$this->label_xAxis];
			}
		}
		$this->xAxis = "'".implode("','", $indices)."'";
    }
    
    public function montaYAxis( $dados ){
    	
		$indices = Array();
		$yAxis = Array();
		if( is_array( $this->arrLabel_yAxis ) ){
			foreach( $this->arrLabel_yAxis as $legenda ){
				$valores = Array();
				if( is_array($dados) ){
					foreach( $dados as $dado ){
						$valores[] = number_format($dado[$legenda['campo']],2,".","");
					}
				}
				$yAxis[] = "name: '{$legenda['descricao']}',
    					    data: [".implode(',', $valores)."]";
				unset($valores);
			}
		}

		$this->yAxis = "{".implode("},{", $yAxis)."}";
    }

    public function utf8Encode(&$dados)
    {
        if(is_array($dados)){
            foreach($dados as &$dado ){
                if(is_array($dado)){
                    $this->utf8Encode($dado);
                } else {
                    $dado = utf8_encode($dado);
                }
            }
        }
    }


    /*
     * Função que agrupa os dados de uma consulta para serem usados em gráficos agrupados.
     * O agrupamento obedece ao formato exigido pelo componente de gráfico.
     *
     * @param array $dados - Dados carregados do banco
     * @param string $categoria - Nome do campo que representará as divisões (eixo x)
     * @param string $name - Nome do campo que representará os grupos dentro da divisão
     * @param string $valor - Nome do campo que representará o valor de cada grupo
     *
     * @return array - Retorna um array com duas informações: series e categories, sendo o primeiro com os dados agrupados e o último com todas as divisões únicas
     * @author Orion Teles de Mesquita
     */
    public function agruparDadosGrafico($dados)
    {
        $categoria = $this->agrupadores['categoria'];
        $name = $this->agrupadores['name'];
        $valor = $this->agrupadores['valor'];

        if(is_string($dados)){
            global $db;
            $dados = $db->carregar($dados);
        }

        $categories = array();
        $grupos = array();
        $dadosAgrupados = array();

        if(is_array($dados)){

            $this->utf8Encode($dados);

            if ($this->tipo == self::K_TIPO_PIZZA) {

                $series[0]['name'] = '';
                foreach($dados as $dado){
                    $series[0]['data'][] = array($dado['descricao'], (float)$dado['valor']);
                }

            } else {

                foreach ($dados as $dado) {
//                ver($dados, d);
                    $dadosAgrupados[$dado[$categoria]][$dado[$name]] = (float) $dado[$valor];
                    $categories[$dado[$categoria]] = $dado[$categoria];
                    $grupos[$dado[$name]] = $dado[$name];
                }

                $dadosFinais = array();
                foreach ($grupos as $grupo) {
                    foreach ($categories as $divisao) {
                        if(!isset($dadosAgrupados[$divisao][$grupo])){
                            $dadosFinais[$grupo][] = 0;
                        } else {
                            $dadosFinais[$grupo][] = $dadosAgrupados[$divisao][$grupo];
                        }
                    }
                }
                $series = array();
                foreach ($dadosFinais as $divisao => $aDado) {
                    $series[] = array('name' => $divisao, 'data'=>$aDado);
                }
            }
        }
        return array('series'=>$series, 'categories'=>array_values($categories));
    }

    private function gerarIdGrafico()
    {
        return 'grafico_' . rand() . '_' . rand();
    }

    private function getTipoGrafico($tipo)
    {
        $tipos = array(
            self::K_TIPO_PIZZA  => 'pie',
            self::K_TIPO_AREA   => 'area',
            self::K_TIPO_LINHA  => 'line',
            self::K_TIPO_BARRA  => 'bar',
            self::K_TIPO_COLUNA => 'column',
        );

        return $tipos[$tipo];
    }

    private function resetValores()
    {
        $this->id = null;
        $this->height = '300px';
        $this->titulo = '';
        $this->subtitulo = '';
        $this->agrupamentoManual = false;
        $this->formatoX = null;
        $this->formatoY = self::K_DECIMAL_0;
        $this->labelX = array('rotation'=>-45, 'align'=>'right');
        $this->agrupadores = array('categoria' => 'categoria', 'name' => 'descricao', 'valor' => 'valor');

        // Exemplo de Legenda
        // $this->legenda = array('layout' => 'vertical', 'align' => 'right', 'verticalAlign' => 'middle', 'borderWidth'=>'0', 'itemStyle:'=>simec_json_encode(array('color'=>'#fff')));

        $this->formatoTooltip = self::K_TOOLTIP_PIE_DECIMAL_2;

        $this->colors =  "
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
                        ";

    }

    private function incluirBiblioteca()
    {
        echo '
            <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
            <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
            <script language="javascript" src="/estrutura/js/funcoes.js"></script>

        ';
    }
}
