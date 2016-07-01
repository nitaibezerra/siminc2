<html>
<head>
<style>
 .tp_grafico{background-color: #FCFFCC; border: solid 1px #FCCFFF;width: 100%;height: 100%;text-align: center;cursor: pointer;}
    
    @media print {
      .noprint { display: none; }
    }
	.grafico{padding-left:5px;float:left;width:420px;magin-left:5px;margin-bottom:5px;text-align:center;font-weight:bold;margin-left:5px;text-align:center}
	.titulo_grafico{font-weight:bold;font-size:14px;}
	.titulo_sub_indicador{font-weight:bold;font-size:18px;text-align: center;margin-bottom: 5px}

</style>
<title>Gráficos</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body>
<div id="exibe_graficos">
<?
/*
 * Verificando se existe a estrutura de diretorios
 * Caso não, cria-los
 */
if(!is_dir(APPRAIZ."www/graficos")) {
	mkdir(APPRAIZ."www/graficos", 0777);
}
if(!is_dir(APPRAIZ."www/graficos/rehuf")) {
	mkdir(APPRAIZ."www/graficos/rehuf", 0777);
	mkdir(APPRAIZ."www/graficos/rehuf/xml", 0777);
}
if(!is_dir(APPRAIZ."www/graficos/rehuf/xml")) {
	mkdir(APPRAIZ."www/graficos/rehuf/xml", 0777);
}
$caminho = APPRAIZ."www/graficos/rehuf/xml/";

// carregando os valores
$sql = "SELECT * FROM rehuf.indicadorvalor";
$indicadorvalores = $db->carregar($sql);
if($indicadorvalores) {
	foreach($indicadorvalores as $valores) {
		$indvalores["{valor".$valores['invid']."}"] = $valores['invselect'];
	}
}


$sql = "SELECT * FROM rehuf.indicadorsubitem stm 
		LEFT JOIN rehuf.indicador ind ON stm.indid = ind.indid
		WHERE sinid='".$_REQUEST['sinid']."' ORDER BY sinordem";

$dadosindicadores = $db->pegaLinha($sql);
//print_r($dadosindicadores);
if($dadosindicadores['sincores']) {
	$sincores = explode(";", $dadosindicadores['sincores']);   	
} else {
	echo "<script>
			document.getElementById('exibe_graficos').innerHTML='Não existem faixas de valores cadastradas';
		  </script>";
	unset($dadosindicadores);
}

if($dadosindicadores) {
	echo "<script type=\"text/javascript\" src=\"/includes/FusionCharts/FusionCharts.js\"></script>";
	for($i=$dadosindicadores['indanoini'];$i<$dadosindicadores['indanofim'];$i++) {
		$indices = array_keys($indvalores);
		$sql_formula = str_replace($indices, $indvalores, $dadosindicadores['sinformula']);
		$sql_formula = str_replace(array("{esuid}","{ano}"), array($_SESSION['rehuf_var']['esuid'], $i), $sql_formula);
		if($sql_formula) {
			$resultado = $db->pegaUm("SELECT (".$sql_formula.")");
			/*
			 * Criando arquivo XML
			 */
			$arquivo_xml = $_SESSION['rehuf_var']['entid']."_".$dadosindicadores['indid']."_".$dadosindicadores['sinid']."_".$i.".xml";
			$min = current($sincores);
			$min = explode(",",$min);
			$max = $sincores[(count($sincores)-2)];
			$max = explode(",",$max);
			$conteudo_xml = "<Chart bgColor='FFFFFF' fillAngle='45' upperLimit='".$max[1]."' lowerLimit='".$min[0]."' majorTMNumber='10' majorTMHeight='8' showGaugeBorder='1' gaugeOuterRadius='140' gaugeOriginX='205' gaugeOriginY='206' gaugeInnerRadius='2' formatNumberScale='4' numberPrefix='' displayValueDistance='30' decimalPrecision='2' tickMarkDecimalPrecision='2' pivotRadius='17' showPivotBorder='1' pivotBorderColor='000000' pivotBorderThickness='5' pivotFillMix='FFFFFF,000000'>
							 	<colorRange>";
			foreach($sincores as $dadosc) {
				if($dadosc) {
					$dc = explode(",",$dadosc);
					$conteudo_xml .= "<color minValue='".$dc[0]."' maxValue='".$dc[1]."' code='".$dc[2]."'/>";
				}
			}					
			$conteudo_xml .=	"</colorRange>
								<dials>
								<dial value='".round($resultado,2)."' borderAlpha='0' bgColor='000000' baseWidth='28' topWidth='1' radius='130'/>
								</dials>
								<annotations>
									<annotationGroup xPos='205' yPos='207.5'>
									<annotation type='circle' xPos='0' yPos='2.5' radius='150' startAngle='0' endAngle='180' fillPattern='linear' fillAsGradient='1' fillColor='dddddd,666666' fillAlpha='100,100'  fillRatio='50,50' fillDegree='0' showBorder='1' borderColor='444444' borderThickness='2'/>
									<annotation type='circle' xPos='0' yPos='0' radius='145' startAngle='0' endAngle='180' fillPattern='linear' fillAsGradient='1' fillColor='666666,ffffff' fillAlpha='100,100'  fillRatio='50,50' fillDegree='0' />
									</annotationGroup>
								</annotations>
							 </Chart>";
			$xml = fopen($caminho.$arquivo_xml, 'w');
			fwrite($xml, $conteudo_xml);
			fclose($xml);
			/*
			 * FIM
			 * Criando arquivo XML
			 */
			$javascript .= "<script type=\"text/javascript\">
				   		   	var grafico_".$dadosindicadores['sinid']."_".$i." = new FusionCharts(\"/includes/FusionCharts/AngularGauge.swf\", \"graf_".$dadosindicadores['sinid']."_".$i."\", \"400\", \"250\", \"0\", \"0\");
							grafico_".$dadosindicadores['sinid']."_".$i.".setDataURL('../graficos/rehuf/xml/".$arquivo_xml."');
							grafico_".$dadosindicadores['sinid']."_".$i.".render(\"graf_".$dadosindicadores['sinid']."_".$i."\");
   				  		    </script>";
			$div.= "<div class=\"grafico\"><span class=\"titulo_grafico\">$i</span><div class=\"grafico_flash\" id=\"graf_".$dadosindicadores['sinid']."_".$i."\" ></div></div>";
		}
	}
	echo "<script>
			document.getElementById('exibe_graficos').innerHTML += '<div class=\"titulo_sub_indicador\">".$dadosindicadores['sindsc']."</div>".$div."';
		  </script>";
	echo $javascript;	
	
}

?>
</div>
</body>
</html>