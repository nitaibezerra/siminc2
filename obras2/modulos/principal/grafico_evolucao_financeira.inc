<?php
//$atiid = $_REQUEST['filtro'];
//
//$sql = "SELECT
//            ati.atidescricao
//        FROM
//            pde.atividade ati
//        WHERE
//            ati.atiid = {$atiid}";
//$dadosProjeto = $db->pegaLinha($sql);
//
//$sql = "SELECT
//            pornome
//        FROM
//            portos.portoatividade pa
//        JOIN portos.porto p ON p.porid = pa.porid
//        WHERE
//            atiid = {$atiid}";
//$pornome = $db->carregarColuna($sql);
//$dadosProjeto['pornome'] = implode(', ', $pornome);

$obrid = $_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid'];

$obra = new Obras( $obrid );

$vlrObra    = new ValorMensalObra();
$arPrevisto = $vlrObra->getValorMensalByObra($obrid);

$arPrevisto      = is_array($arPrevisto) ? $arPrevisto : array();
$arPrevistoFinal = array();
$arInfo          = array();
$categorias      = array();
$total           = 0;
$totalValor      = 0;
$referencia      = array();

foreach ($arPrevisto as $k => $previsto){
    $mes         = mes_extenso($previsto['mes']);
    $ano         = $previsto['ano'];
    $total      += $previsto['percentualprevistomes'];
    $totalValor += $previsto['valorprevisto'];
    $totalMin    = round($total, 1);

    $arPrevistoFinal["{$previsto['ano']}{$previsto['mes']}"] = $totalMin;

    $arInfo[$k]['Previsto']['mes']         = utf8_encode($mes);
    $arInfo[$k]['Previsto']['ano']         = $ano;
    $arInfo[$k]['Previsto']['porcentagem'] = str_replace('.', ',', $totalMin);
    $arInfo[$k]['Previsto']['valor']       = formata_valor($totalValor, 2);

    $referencia["{$previsto['ano']}{$previsto['mes']}"] = $k;
}

$sql = "SELECT
            ano,
            mes,
            valorexecutado,
            precentualexecutado
        FROM
            obras2.vw_obra_execucao_realizada
        WHERE
            obrid = {$obrid}
        ORDER BY
            ano, mes;";

$arRealizado      = $db->carregar($sql);
$arRealizado      = is_array($arRealizado) ? $arRealizado : array();
$arRealizadoFinal = array();
$total            = 0;
$totalValor       = 0;
$ultIndicePrevisto = array_keys( $referencia );
$ultIndicePrevisto = end( $ultIndicePrevisto );
foreach ($arRealizado as $k => $realizado){
    $mes         = mes_extenso($realizado['mes']);
    $ano         = $realizado['ano'];
    $total       = $realizado['precentualexecutado'];
    $totalValor  = $realizado['valorexecutado'];
    $totalMin    = round($total, 1);

    $arRealizadoFinal["{$realizado['ano']}{$realizado['mes']}"] = $totalMin;

    if ( !isset($referencia["{$realizado['ano']}{$realizado['mes']}"]) ){
		$arPrevistoFinal["{$realizado['ano']}{$realizado['mes']}"] 					= $arPrevistoFinal[$ultIndicePrevisto];    	
    	$referencia["{$realizado['ano']}{$realizado['mes']}"] 						= $referencia[$ultIndicePrevisto] + 1;
    	$arInfo[$referencia["{$realizado['ano']}{$realizado['mes']}"]]['Previsto'] 	= $arInfo[$referencia[$ultIndicePrevisto]]['Previsto'];
    	$arInfo[$referencia["{$realizado['ano']}{$realizado['mes']}"]]['Previsto']['mes'] = $mes;
    	$arInfo[$referencia["{$realizado['ano']}{$realizado['mes']}"]]['Previsto']['ano'] = $ano;
    	$ultIndicePrevisto = "{$realizado['ano']}{$realizado['mes']}";
    }
    $arInfo[$referencia["{$realizado['ano']}{$realizado['mes']}"]]['Realizado']['mes']         = utf8_encode($mes);
    $arInfo[$referencia["{$realizado['ano']}{$realizado['mes']}"]]['Realizado']['ano']         = $ano;
    $arInfo[$referencia["{$realizado['ano']}{$realizado['mes']}"]]['Realizado']['porcentagem'] = str_replace('.', ',', $totalMin);
    $arInfo[$referencia["{$realizado['ano']}{$realizado['mes']}"]]['Realizado']['valor']       = formata_valor($totalValor, 2);
}

$arRealizadoTmp   = $arRealizadoFinal;
$arRealizadoFinal = array();
foreach ($arPrevistoFinal as $k => $v){
    if(is_null($arRealizadoTmp[$k])){
        $arRealizadoFinal[$referencia[$k]] = 0;
        $arInfo[$referencia[$k]]['Realizado']['mes'] = $arInfo[$referencia[$k]]['Previsto']['mes'];
        $arInfo[$referencia[$k]]['Realizado']['ano'] = $arInfo[$referencia[$k]]['Previsto']['ano'];
    }else{
        $arRealizadoFinal[$referencia[$k]] = $arRealizadoTmp[$k];
        //break;
    }
}

//retira de trás para a frente os registros que não tiverem vistoria, até encontrar algum que tenha
//então ele pára
$arRealizadoFinal = array_reverse($arRealizadoFinal);
foreach ($arRealizadoFinal as $k => $v ){
    if($v == 0){
        unset($arRealizadoFinal[$k]);
    }else{
        break;
    }
}
$arRealizadoFinal = array_reverse($arRealizadoFinal);

// Varre o array para fazer com que os meses que não possuem vistoria ficarem com o valor da vistoria
// do mes anterior
foreach ($arRealizadoFinal as $k => $v ){
	// Todos as vistorias que vem do banco bem como float, se for int então o mes nao foi vistoriado e
	// foi adicionado nas logicas acima
    if( is_int($v) ){
    	// Verifica se o registro anterior existe ( se não é o primeiro )
        if( ($k-1) >= 0){
        	// Atribui o valor anterior ao registro
		    $arRealizadoFinal[$k] = !empty($arRealizadoFinal[$k-1]) ? $arRealizadoFinal[$k-1] : 0;
        }
    }
}

?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>

	<link type="text/css" href="/includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css" rel="stylesheet" />


<!--	<link rel="stylesheet"  href="/includes/jquery-mobile/jquery.mobile-1.0.1.min.css" />-->
<!--	<link rel="stylesheet"  href="/includes/jquery-mobile/jqm-docs.css" />-->
<!--	<script src="/includes/jquery-mobile/jquery.js"></script>-->
<!--	<script src="/includes/jquery-mobile/jquery.mobile-1.0.1.min.js"></script>-->

	<script src="/includes/JQuery/jquery-1.4.2.js"></script>
	<script src="/includes/jquery-highcharts/js/highcharts.js"></script>
	<script src="/includes/jquery-highcharts/js/modules/exporting.js"></script>
	<script type="text/javascript" src="/includes/estouvivo.js"></script>
    <script type="text/javascript">
    var info = <?= simec_json_encode($arInfo); ?>;

    $(function(){
    	var chart;
        $(document).ready(function() {
            chart = new Highcharts.Chart({
                chart: {
                    renderTo: 'container',
                    type: 'line',
                    marginRight: 130,
                    marginBottom: 25
                },
                exporting: { enabled: false },
                credits: {
                    enabled: false
                },
                title: {
                    text: 'Evolução Física',
                    x: -20 //center
                },
                subtitle: {
                    text: '',
                    x: -20
                },
                xAxis: {
                    categories: []
                },
                yAxis: {
                    title: {
                        text: 'Porcentagem'
                    },
                    plotLines: [{
                        value: 0,
                        width: 1,
                        color: '#808080'

                    }]
                },
                tooltip: {
                    formatter: function() {
                              var pPorcentagem = '-';
                              var pValor       = '-';
                              var rPorcentagem = '-';
                              var rValor       = '-';

                              if(info[this.x]['Previsto']){
                            	   pPorcentagem = info[this.x]['Previsto'].porcentagem + '%';
                            	   pValor       = info[this.x]['Previsto'].valor;
                              }

                              if(info[this.x]['Realizado']){
                            	   if(info[this.x]['Realizado'].porcentagem){
                                       rPorcentagem = info[this.x]['Realizado'].porcentagem + '%';
                            	   }

                            	   if(info[this.x]['Realizado'].valor){
                                	   rValor       = info[this.x]['Realizado'].valor;
                            	   }
                              }

                              var out = '<b>'+ this.series.name + '</b>';
                              out     += "<br><b>Mês:</b>"                   + info[this.x][this.series.name].mes;
                              out     += "<br><b>Ano:</b>"                   + info[this.x][this.series.name].ano;
                              out     += "<br><b>Porcentagem Prevista:</b>"  + pPorcentagem;
                              out     += "<br><b>Porcentagem Realizada:</b>" + rPorcentagem;
                              out     += "<br><b>Valor Previsto:</b> R$ "    + pValor;
                              out     += "<br><b>Valor Realizado:</b> R$ "   + rValor;
                              return out;
                    }
                },
                legend: {
                    layout: 'vertical',
                    align: 'right',
                    verticalAlign: 'top',
                    x: -10,
                    y: 100,
                    borderWidth: 0
                },
                series: [{
                    name: 'Previsto',
                    data: [<?= implode(',', $arPrevistoFinal); ?>]
                }, {
                    name: 'Realizado',
                    data: [<?= implode(',', $arRealizadoFinal) ?>]
                }]
            });
        });
    });

    </script>
        <style type="text/css">

		#pagina_1{
			min-height:100%;
	    	position:relative;
		}
		body {
		    -webkit-background-size: cover;
		    -moz-background-size: cover;
		    -o-background-size: cover;
		    background-size: cover;
		    margin: 0;

		}

        .content-primary{
            float:left;
            margin-left: 20px;
            width: 60%;
            margin-left: 390px;
            position: absolute;
            margin-top: 10px;
        }

        .content-secondary{
            margin-top: 10px;
            width: 340px;
            border:2px solid #CCCCCC;
            margin-left: 10px;
            padding-right: 0%;
            position: absolute;
            padding: 0px;
            height: 560px;

        }

        .contentInfo{
            padding:2px;
            margin:5px 10px;
            width:320px;
            height:200px;
            float:left
        }

        body, html {
            background-color: #EDEDED;
            padding: 0px;
            margin: 0px;
            font-family: Helvetica,Arial,sans-serif;
            height: 100%;
        }

        [data-role=header]{
            display: block;
            font-size: 16px;
            min-height: 1.1em;
            outline: 0 none !important;
            overflow: hidden;
            padding: 0;
            text-align: center;
            text-overflow: ellipsis;
            white-space: nowrap;
            border: 1px solid #456F9A;
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #254F7A;
            width: 99.8%;
            background: #81A8CE; /* for non-css3 browsers */
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#81A8CE', endColorstr='#5E87B0'); /* for IE */
            background: -webkit-gradient(linear, left top, left bottom, from(#81A8CE), to(#5E87B0)); /* for webkit browsers */
            background: -moz-linear-gradient(#81A8CE, #5E87B0) repeat scroll 0 0 #5E87B0;
        }
        [data-role=header] h1{
            font-size: 16px;
            margin-top: 10px;
        }

        [data-role=page]{
        }

        [data-role=content]{
            height: auto;
            min-height: 100%;
            /* height: 550px; */
            padding-bottom: 40px;
        }

        [data-icon=home]{
            display: inline-block;
            font-size: 13px;
            margin: 0;
            position: absolute;
            left: 10px;
            top: 12px;
            height: 24px;
            width: 24px;
            cursor: pointer;
            text-align: center;
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #145072;
        }

        [data-role=footer]{
            border-left-width: 0;
            border-right-width: 0;
            overflow: hidden;
            display: block;
            font-family: Helvetica,Arial,sans-serif;
            border: 1px solid #456F9A;
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #254F7A;
            /* position: relative; */
            height: 40px;
            clear:both;
            background: #81A8CE; /* for non-css3 browsers */
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#81A8CE', endColorstr='#5E87B0'); /* for IE */
            background: -webkit-gradient(linear, left top, left bottom, from(#81A8CE), to(#5E87B0)); /* for webkit browsers */
            background: -moz-linear-gradient(#81A8CE, #5E87B0) repeat scroll 0 0 #5E87B0;

            position:absolute;
		    bottom:0;
            width: 99.8%;
        }


        [data-role=button], .ui-btn-up-b{
            border: 1px solid #456F9A;
            background: #81A8CE; /* for non-css3 browsers */
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#81A8CE', endColorstr='#5E87B0');
            background: -webkit-gradient(linear, left top, left bottom, from(#81A8CE), to(#5E87B0));
            background: -moz-linear-gradient(#81A8CE, #5E87B0) repeat scroll 0 0 #5E87B0;
            text-align: center;
            padding-bottom: 0px;
            width: 99.8%;
            height: 35px;
            padding-top: 10px;
            cursor: pointer;
        }

        [data-role=button]:hover{
            background: #5E9BC4; /* for non-css3 browsers */
            background: -moz-linear-gradient(#5E9BC4, #3A6C9F) repeat scroll 0 0 #5E9BC4;
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#5E9BC4', endColorstr='#3A6C9F');
            background: -webkit-gradient(linear, left top, left bottom, from(#5E9BC4), to(#3A6C9F));
        }

        [data-role=button] a{
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #254F7A;
        }

        .ui-btn-up-b{
            color: #FFFFFF;
            font-weight: bold;
            text-shadow: 0 -1px 1px #145072;
            padding-top: 0px;
            width: 100px;
            cursor: auto;
        }

        #div_imagem a, #div_imagem  img{
            text-decoration: none;
            border: 0px;
        }

		.linkInicio{
			background-image:url(/imagens/inicio_mobile.png);
			background-repeat:repeat-x;
			height: 26px;
			width: 26px;
		}

		.linkInicio:hover{
			background-image:url(/imagens/inicio_hover_mobile.png);
			background-repeat:repeat-x;
			height: 27px;
			width: 27px;
		}


        .linkVoltar{
            background-image:url(/imagens/voltar_mobile.png);
            background-repeat:repeat-x;
            height: 26px;
            width: 26px;
        }

        .linkVoltar:hover{
            background-image:url(/imagens/voltar_hover_mobile.png);
            background-repeat:repeat-x;
            height: 26px;
            width: 26px;
        }

        #rodape {
            height: 30px;
            width: 100%;
            text-align: center;
            padding-top: 5px;
            background: -moz-linear-gradient(#81A8CE, #5E87B0) repeat scroll 0 0 #5E87B0;
            filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#81A8CE', endColorstr='#5E87B0');
            background: -webkit-gradient(linear, left top, left bottom, from(#81A8CE), to(#5E87B0));
        }

		a:link {
		    text-decoration: none;
		}

        #rodape a{
            cursor: pointer;
            left: 10px;
            position: absolute;

        }
    </style>
<?php 
if ( $_GET['acao'] == 'V' ):
?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php 
	$habilitado = false;
	$habilita 	= 'N';
else:
	$habilitado = true;
	$habilita 	= 'S';
endif;
?>	
    
</head>
<body>
<?php 
if ( $habilitado == false ){
	$db->cria_aba($abacod_tela,$url,$parametros);
	echo cabecalhoObra($obrid);
	echo '<center>
			<div style="width:95%;">';
}
?>
	<div data-role="page" id="pagina_1">
		<div data-role="header" data-theme="b">
			<h1><?=$obra->obrnome ?></h1>
	<!--		<a href="projetos.php?modulo=relatorio/mobile&acao=A" style="text-decoration: none;" data-icon="home" data-iconpos="notext" data-direction="reverse" class="ui-btn-right jqm-home">Início</a>-->
		</div><!-- /header -->
	
		<div data-role="content" >
		    <div id="container" style="min-width: 400px; height: 400px; margin: 0 auto"></div>
		</div><!-- /content -->
	
	     <div id="rodape">
	     </div>
	
	<!--	<div data-role="footer" data-theme="b" data-position="fixed">-->
	<!--		<div data-role="controlgroup" data-type="horizontal">-->
	<!--			<a href="projetos.php?modulo=relatorio/mobile_gerencial_infraestrutura&acao=C&atiid=<?php echo $atiid; ?>" data-icon="home" class="inicio-rodape" style="text-decoration: none;">Voltar</a>-->
	<!--		</div>-->
	<!--	</div> /footer -->
	</div><!-- /page -->
<?php 
if ( $habilitado == false ){
	echo '	</div>
		  </center>';
}
?>

