<?php
// $_REQUEST['filtros'][0] = 'tipo_Escola 12 Salas - Projeto FNDE';
// $_REQUEST['filtros'][1] = 'situacao_Obra Aprovada';

set_time_limit(0);
/* $conf = Array('nome'=>string, 'descricao'=>string, 'unidade'=>string, 'minimo'=>integer, 'maximo'=>integer, 'passo'=>integer, 'passo'=>integer(%));
 * 
 * */
function montaSlider( $conf ){
?>
	<div style="width: <?=$conf['largura'] ?>% !important; margin: 0 auto; text-align: center;">
		<script>
		jQuery(document).ready(function(){
		
			jQuery(".slider-range").slider({
				range: true,
				min: 0,
				max: <?=$conf['maximo'] ?>,
				step: <?=$conf['passo'] ?>,
				values: [<?=$conf['minimo'] ?>, <?=$conf['maximo'] ?>],
				slide: function( event, ui ) {
					jQuery( "#<?=$conf['nome'] ?>" ).html( ui.values[ 0 ] + "<?=$conf['unidade'] ?> - " + ui.values[ 1 ] );
					jQuery( "#vlr_min_<?=$conf['nome'] ?>" ).html( ui.values[ 0 ] );
					jQuery( "#vlr_max_<?=$conf['nome'] ?>" ).html( ui.values[ 1 ] );
		
					if(ui.values[1] <= 45){
						jQuery(this).find('.ui-slider-range').css('background', '#EE3030');
					}else if(ui.values[1] <= 60){
						jQuery(this).find('.ui-slider-range').css('background', '#CCCC00');
					}else{
						jQuery(this).find('.ui-slider-range').css('background', '#669933');
					}
		
				}
			});
		
			jQuery( "#<?=$conf['nome'] ?>" ).html( jQuery( ".slider-range" ).slider( "values", 0 ) + "<?=$conf['unidade'] ?> - " + jQuery( ".slider-range" ).slider( "values", 1 ) );
		});
		</script>
		<div style="margin-bottom: 5px; font-weight: bold;">
			<?=$conf['descricao'] ?>:
			<span id="<?=$conf['nome'] ?>" style="border: 0; color: #fff; font-weight: bold;"></span><?=$conf['unidade'] ?>
			<input type=hidden name=vlr_min_<?=$conf['nome'] ?> id=vlr_min_<?=$conf['nome'] ?> value=<?=$conf['minimo'] ?> />
			<input type=hidden name=vlr_max_<?=$conf['nome'] ?> id=vlr_max_<?=$conf['nome'] ?> value=<?=$conf['maximo'] ?> />
		</div>
		<div class="slider-range" id="slider0"></div>
	</div>
<?php
}

function geraGraficoCallback(array $dados,$nomeUnico,$titulo,$formatoDica,$formatoValores,$nomeSerie,$mostrapopudetalhes=false,$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $aLegendaConfig = array(), $legendaClique = false)
{
	if($legendaClique){
		$legendaClique = 'true';
		$cursor = 'pointer';
	} else {
		$legendaClique = 'false';
		$cursor = 'default';
	}

	$array_valores;

	/*  Configuração da exibição das legendas
	 *
	*  Alinhamento (align): O alinhamento do box da legenda dentro da área do gráfico.
	*  Valores válidos "left", "center" ou "right".
	*  Valor padrão "center".
	*
	*  Layout (layout): O layout dos itens da legenda.
	*  Valores válidos "horizontal" ou "vertical".
	*  Valor padrão "horizontal".
	*/
	$aLegendaConfig['align']  = $aLegendaConfig['align']  ? $aLegendaConfig['align']  : 'center';
	$aLegendaConfig['layout'] = $aLegendaConfig['layout'] ? $aLegendaConfig['layout'] : 'horizontal';

	$arrCorItem = array(
			"Aguardando solicitação do município"						=> "'#CCCA00'", // Amarelo
			"Análise da solicitação de utilização da ata"				=> "'#EEAAEE'", // Rosa claro
			"Aguardando ciência do fornecedor"							=> "'#00BFFF'", // Azul claro
			"Aguardando autorização do FNDE"							=> "'#848305'", // Amarelo Escuro
			"Aguardando geração do contrato pelo município"				=> "'#FFD700'", // Laranja
			"Contrato em tramitação"									=> "'#AAEEEE'", // Cinza claro
			"Aguardando emissão de OS (com pendência de regularização)"	=> "'#A077BF'", // Roxo
			"Aguardando emissão de OS"									=> "'#7798BF'", // Roxo claro
			"Aguardando aceite da OS pelo fornecedor"					=> "'#A2A9B0'", // Cinza
			"OS Recusada"												=> "'#000000'", // Preto
			"Solicitação cancelada"										=> "'#FF6A6A'", // Vermelho claro
			"Execução até 25%"											=> "'#E5F6DB'", // Verde
			"Execução de 25 a 50%"										=> "'#C5F5AA'", // Verde
			"Execução de 50% a 75%"										=> "'#A4F677'", // Verde
			"Execução acima de 75%"										=> "'#7AC551'", // Verde
			"Concluída"													=> "'#5EFB09'", // Verde
			"Paralisada"												=> "'#FFFF00'", // Amarelo
			"Obra Cancelada"											=> "'#850808'", // Vermelho
			"Execução"													=> "'#E5F6DB'", // Verde
			"Em Reformulação"											=> "'#0000CC'", // Azul Escuro
			"Ensino Médio"												=> "'#FFFF00'", // Amarelo
			"FIC"														=> "'#00BFFF'", // Azul claro
			"Pós-Graduação"												=> "'#FF6A6A'", // Vermelho claro
			"Superior"													=> "'#EEAAEE'", // Rosa claro
			"Técnico"													=> "'#3CA628'", // Verde
	);

	$arrCor = array();
	foreach ($dados as $item)
	{
		$array_valores .= '[\''.$item['descricao'].'\','.$item['valor'].']';
		$strValores .= "'".$item['descricao']."',";
		if(in_array($item['descricao'], array_keys($arrCorItem))){
			$arrCor[] = $arrCorItem[$item['descricao']];
		}
	}

	if(count($arrCor) > 0){
		$cor = implode(', ' , $arrCor);
		$cor .= "
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro

                //    , '#0000FF' // Azul
                    , '#008000' // Verde
                //    , '#FFD700' // Gold
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

	} else {
		$cor = "
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

                //    , '#0000FF' // Azul
                    , '#008000' // Verde
                //    , '#FFD700' // Gold
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

	$strValores = trim($strValores,",");
	$array_valores = str_replace('][', '],[', $array_valores);
	?>
		<script>
		jQuery(document).ready(function() {

                // Radialize the colors
                Highcharts.getOptions().colors = Highcharts.map(
                        [
                           <?php echo $cor ?>
                    ]
                        , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                });


		jQuery('#<?=$nomeUnico?>').highcharts({
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
                plotShadow: true,
                backgroundColor:'rgba(255, 255, 255, 0.0)'
            },
            title: {
                text: '<?=$titulo?>',
                style: {
         			color: '#C7C5C5'
      			}
            },
            tooltip: {
                 pointFormat: '<?=$formatoDica?>'
            },

            //habilitar o botão de salvar como imagem, pdf, etc
            exporting: {
        	 enabled: true
			},
			//estilo legenda
			legend: {
			   layout: '<?php echo $aLegendaConfig['layout']; ?>',
			   align:  '<?php echo $aLegendaConfig['align']; ?>',
			   itemStyle: {
				   paddingBottom: '10px',
				   color: '#C7C5C5'
			   }
		   },
            plotOptions: {
                pie: {
                    point: {
                        events: {
                            legendItemClick: function() {
                                    return <?php echo $legendaClique ?>;
                            }
                        }
                    },
                    cursor: '<?php echo $cursor ?>',
                    borderWidth: 0, // Borda dos pedaços da pizza
                    allowPointSelect: true,
                    dataLabels: {
                        enabled: true,
                        color: '#C7C5C5',
//                        connectorColor: 'white',
						//<b>{point.name}</b>: para colocar o nome na legenda
                        <?php if ($formatoValores) { ?>
                            format: '<?=$formatoValores ?>'
                        <?php } else { ?>
                            formatter: function () { return number_format(this.y, 0, ',', '.') + ' (' + number_format(this.percentage, 2, ',') + ') %'; }
                        <?php } ?>
                    },
					showInLegend: '<?=$mostrarLegenda ?>'
                },
                series: {
                    cursor: 'pointer'
                    ,
                    events: {
//                        legendItemClick:  false
                        <?if ($mostrapopudetalhes): ?>
                            click: function(event){
                                var arrValores = new Array (<?=$strValores?>);
                               	<?php echo $caminhopopupdetalhes; ?>, arrValores[event.point.x]);
                            }
                        <? endif ?>
                    }
    			}

            },
            series: [{
                type: 'pie',
                name: '<?=$nomeSerie?>',
                data: [ <?PHP echo $array_valores; ?> ]

            }]
        });
    });

    </script>
 	<div id="<?=$nomeUnico?>" ></div>
	<?
}

function colunasContexto( $contexto = '' ){
	
	switch ($contexto){
		case 'obras':
			return Array(
						Array('codigo'=>'prenome', 			'descricao'=>'Nome da Obra'),
						Array('codigo'=>'estuf', 			'descricao'=>'UF'),
						Array('codigo'=>'mundescricao', 	'descricao'=>'Município'),
						Array('codigo'=>'ptodescricao', 	'descricao'=>'Tipo de Obra'),
						Array('codigo'=>'esddescricao', 	'descricao'=>'Situação da Obra'),
						Array('codigo'=>'Valor da Obra', 	'descricao'=>'Valor da Obra'),
					);
			break;
	}
}

function gerarDetalheGrafico( $grupo = '' )
{
	global $db;
	
	//Verifica se já possui filtros anteriores
	$filtros = is_array($_REQUEST['filtros']) ? $_REQUEST['filtros'] : Array();
	
	//Verifica se quer voltar o filtro
	if( $_REQUEST['voltar_filtros'] != '' ){
		//Recupera grupo a ser retornado
		$grupo = $filtros[$_REQUEST['voltar_filtros']];
		$grupo = explode('_',$grupo);
		$grupo = $grupo[0];
		//FIM - Recupera grupo a ser retornado

		//Cria temporário para filtros e limpa até filtro anterior ao qual se quer retornar
		$tmpFiltros = $filtros;
		$filtros = Array();
		for($x=0;$x<$_REQUEST['voltar_filtros'];$x++){
			$filtros[] = $tmpFiltros[$x];
		}
		// FIM - Cria temporário para filtros e limpa até filtro anterior ao qual se quer retornar
	}
	
	//Se for para mudar o agrupador e filtrar por ele adiciona o agrupador nos filtros.
	if( $_REQUEST['filtrar'] == 'true' ){
		$filtros[] = $grupo.'_'.str_replace('___', ' ', $_REQUEST['descricao']);
		unset($grupo);
	}
	//FIM - Se for para mudar o agrupador e filtrar por ele adiciona o agrupador nos filtros.
	
	//Inicia tratamento de filtros anteriores para criação do rastro
	$where = Array('predtinclusao::date' > '2014-01-01');
	$filtroAgrupamento = '';
	$AgrupadoresFiltrados = Array();
	if( $filtros ){
		foreach( $filtros as $ordem => $filtro ){
			$filtro = explode('_',$filtro);
			$AgrupadoresFiltrados[] = $filtro[0];
			$where[] = "{$filtro[0]} = '{$filtro[1]}'";
			switch ($filtro[0]) {
				case ('uf'):
					$descricao .= "UF da Obra"; 
					break;
				case ('tipo'):
					$descricao .= "Tipo da Obra"; 
					break;
				case ('situacao'):  
					$descricao .= "Situação da Obra"; 
					break;
			}
			$filtroAgrupamento .= "<label class=voltar_filtro ordem=$ordem >$descricao('{$filtro[1]}') => </label>&nbsp;"; 
			unset($descricao);
		}
	}
	// FIM - Inicia tratamento de filtros anteriores para criação do rastro

	//Define possiveis grupos de gráficos(contextos) Array(contexto::texto => Array(grupo, ...), ...)
	$arrGrupos = Array('obras' => Array('uf', 'tipo', 'situacao'));
	$aGrupo = Array('uf', 'tipo', 'situacao'); //contexto inicial
	$arrLabelGrupo = Array('uf' => 'UF', 'tipo' => 'Tipo', 'situacao' => 'Situação' ); // Colocar aqui todos os labels possiveis
	$contextoGrupo = 'obras';
	
	//verifica qual grupo de gráfico(contexto) deve ser carregado
	foreach( $arrGrupos as $contexto => $grupos ){
		if( in_array($grupo,$grupos ) ){
			$aGrupo = $grupos;
			$contextoGrupo = $contexto;
		}
	}
	//FIM - verifica qual grupo de gráfico deve ser carregado
	
	array_unique( $AgrupadoresFiltrados );
	$gruposRestantes = array_diff($aGrupo, (array) $AgrupadoresFiltrados);
	$qtdGruposRestantes = count($gruposRestantes);

	$grupo = $grupo ? $grupo : $gruposRestantes[0];
	$grupo = $grupo ? $grupo : current($gruposRestantes);

	if(!count($gruposRestantes)){ return ''; }

	$agrupamento = '';
	switch ($contextoGrupo){
		case 'obras':
			switch ($grupo) {
				case ('tipo'):  	 	$agrupamento = 'Tipo da Obra: '; break;
				case ('situacao'):   	$agrupamento = 'Situação da Obra:'; break;
				case ('uf'):   			$agrupamento = 'UF da Obra:'; break;
				default: $agrupamento = 'Geral (Obras):';
			}
			break;
	}
	$agrupamento = $filtroAgrupamento.$agrupamento;
	
	$sql = "SELECT
				1 as valor,
				pre.preid,
				esd.esddsc as situacao,
				pto.ptodescricao as tipo,
                pre.estuf as uf
			FROM
				obras.preobra pre
			INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
			INNER JOIN workflow.estadodocumento 	esd ON esd.esdid = doc.esdid
			INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid
			WHERE 
				predtinclusao::date > '2014-01-01'";

	$arrDados = $db->carregar($sql);

	$dados = array();
	if ($arrDados) {
		foreach ($arrDados as $dado) {
			$dados[$dado[$grupo]] += $dado['valor'];
		}
		if($dados){
			$arrDados = array();
			foreach ($dados as $descricao => $valor) {
				$arrDados[] = array('descricao'=>$descricao, 'valor'=>$valor);
			}
		}
	}
	
	$acaoFiltrar = "";
	$booAcaoFiltrar = false;
	if( $qtdGruposRestantes > 1 ){
		$acaoFiltrar = 'montarDetalheGrafico(1, "' . $grupo . '"';
		$booAcaoFiltrar = 'js-acao';
	}

?>
	<div id="accordion_1" >
		<h3><a id="titulo_1"><?=$agrupamento ?></a></h3>
		<div>
<?php
			if( count($filtros)>0 ){
				foreach( $filtros as $ordem => $filtro ){
?>
				<input type="hidden" name="filtros[<?=$ordem ?>]" value="<?=$filtro ?>"/>
<?php 
				}
			}
?>
			<div style="clear:both;width:100%; margin:20px 0;">
				<?php foreach( $gruposRestantes as $grp ){?>
            	    <span class="span_grupo">
            	    	<input type="radio" name="grupo" class="filtro_grupo" value="<?=$grp ?>" id="grupo_demandante_1" <?=($grupo == $grp ? 'checked="checked"': ''); ?> />
            	    	<label for="grupo_tipo_1"><?=($arrLabelGrupo[$grp]) ?></label>
            	    </span>
				<?php } ?>
            </div>
			<?php
			if ($arrDados) {
				geraGraficoCallback($arrDados, "graficoPizza{$seq}", $titulo,"<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", $booAcaoFiltrar, $acaoFiltrar, null, null, true);
			}
			?>
		</div>
	</div>
<?php 
	$arrColunas = colunasContexto( $contextoGrupo );
	if( $arrColunas[0]['codigo'] != '' ){
?>
	<div id=grafico_detalhe_2 >
		<h3><a id=titulo_xls >Exportar XLS</a></h3>
		<div>
			<div class=container_botao ><div style="float:left;" ><input type=button value="Marcar Todos" class=marcar_todos /></div></div>
			<div class=container_colunas >
				<input type=hidden name=contextoGrupo value=<?=$contextoGrupo ?> />
<?php 	foreach( $arrColunas as $ordem => $coluna ){ ?>
				<div class=coluna_xls coluna=<?=$coluna['codigo'] ?>  >
					<span><?=$coluna['descricao'] ?></span>
					<input type=hidden name=coluna[<?=$ordem ?>] value="" />
				</div>
<?php 	}?>
			</div>
			<div id=grafico_detalhe_2_filtro >
				<h3><a id=filtro_xls >Filtros</a></h3>
				<div>
					<div style="width: 95% !important; margin: 0 auto; text-align: center;">
						<?php 
						$conf = Array(
									'nome'=>'percentual_execucao',
									'descricao'=>'Percentual de execução',
									'unidade'=>'%',
									'minimo'=>'0',
									'maximo'=>'100',
									'largura'=>'20',
									'passo'=>'25'
								);
						echo montaSlider( $conf ) 
						?>
					</div>
				</div>
			</div>
			<div class=container_botao ><input type=button value="Exportar XLS" class=exportar_xls /></div>
		</div>
	</div>
<?php } ?>
	<script>
		jQuery("#accordion_1").accordion({collapsible: true, heightStyle: "content"});		
<?php if( $arrColunas[0]['codigo'] != '' ){ ?>
		jQuery("#grafico_detalhe_2").accordion({collapsible: true, heightStyle: "content"});
		jQuery("#grafico_detalhe_2_filtro").accordion({collapsible: true, heightStyle: "content"});
<?php } ?>
	</script>
<?php 
}

function atualizaGrafico(){
	gerarDetalheGrafico( $_REQUEST['grupo'] );
}

if( $_REQUEST['req'] ){
	ob_clean();
	$_REQUEST['req']();
	die();
}

require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Protótipo Relatório Gráfico', '<b></b>');
?>
<style>

.voltar_filtro:hover{
	color:yellow;
	text-decoration: underline;
}

.ui-icon-triangle-1-e{
	float:left;
}

.ui-icon-triangle-1-s{
	float:left;
}

.container_botao{
	width: 100%;
	height: 20px;
/* 	background-color: yellow;   */
	padding:5px;
}

.container_colunas{
	width: 100%;
	height: 70px;
/*  	background-color: green;  */
}

.coluna_xls{
	background-color: #428BCA;
	border: 3px solid #529BDA;
	color: white;
	width: 90px;
	height: 40px;
	float: left;
	-webkit-border-radius: 10px;
	-moz-border-radius: 10px;
	border-radius: 10px;
	margin: 5px;
	padding:5px;
}

.fundo_padrao{
	-webkit-border-radius: 10px;
	-moz-border-radius: 10px;
	border-radius: 10px;
	width: 98%;
}

.coluna_xls:hover{
	background-color: #529BDA;
}

.coluna_xls_selected{
	background-color: #62ABEA;
	border: 3px solid #429BDA;
	color: black;
	font-weight: bold;
}

.coluna_xls_selected:hover{
	background-color: #72BBFA;
}

#slider0 .ui-slider-range { background: #669933; }
#slider0 { background: #429BDA; }
#grafico_detalhe_2 { overflow: hidden; }

</style>
<link rel="stylesheet" type="text/css" href="/library/jquery/jquery-ui-1.10.3/themes/dark-hive/jquery-ui-1.10.3.custom.min.css"/>
    
<!-- <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script> -->
<script language="javascript" type="text/javascript" src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>    
<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
<script language="javascript" type="text/javascript" src="/estrutura/js/funcoes.js"></script>
<!-- <script language="javascript" type="text/javascript" src="js/estrategico.js"></script> -->

<link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
<script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
<script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

<link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
<script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

<!-- <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script> -->
<!-- <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script> -->

<link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
<script>

jQuery(document).ready(function(){

	jQuery('.filtro_grupo').live('click',function(){
	    jQuery('#grafico').load( 'par.php?modulo=relatorio/prototipo/formRelatorioGrafico&acao=A&req=atualizaGrafico&grupo='+jQuery(this).val()+'&'+jQuery('#formGrafico').serialize() );
	});

	jQuery('.voltar_filtro').live('click', function(){
	    jQuery('#grafico').load( 'par.php?modulo=relatorio/prototipo/formRelatorioGrafico&acao=A&req=atualizaGrafico&voltar_filtros='+jQuery(this).attr('ordem')+'&'+jQuery('#formGrafico').serialize() );
	});

	jQuery('.coluna_xls').live('click', function(){
		var classe = jQuery(this).attr('class');
		if( classe == 'coluna_xls' ){
			jQuery(this).addClass('coluna_xls_selected');
			jQuery(this).find('input').val(jQuery(this).attr('coluna'));
		}else{
			jQuery(this).removeClass('coluna_xls_selected');
			jQuery(this).find('input').val('');
		}
	});

	jQuery('.marcar_todos').live('click',function(){
		if( jQuery('.coluna_xls_selected').size() != jQuery('.coluna_xls').size() ){
			jQuery('.coluna_xls').each(function(){
				jQuery(this).attr('class', 'coluna_xls coluna_xls_selected');
				jQuery(this).find('input').val(jQuery(this).attr('coluna'));
			});
		}else{
			jQuery('.coluna_xls').each(function(){
				jQuery(this).removeClass('coluna_xls_selected');
				jQuery(this).find('input').val('');
			});
		}
	});
	
});

function montarDetalheGrafico(seq, grupo, descricao)
{
    var descricao = str_replace(' ', '___', descricao);
    jQuery('#grafico').load( 'par.php?modulo=relatorio/prototipo/formRelatorioGrafico&acao=A&req=atualizaGrafico&filtrar=true&descricao='+descricao+'&'+jQuery('#formGrafico').serialize(), 
    	    function(){
	    		jQuery('#fundo_padrao').css('width','800px;');
			});
}

</script>
<form id="formGrafico" method="POST" >
	<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="fundo_padrao link"  align="middle">
				<div id="div_graficos">
					<div id='grafico'>
						<?php gerarDetalheGrafico(''); ?>
					</div>
				</div>
			</td>
		</tr>	
	</table>
</form>