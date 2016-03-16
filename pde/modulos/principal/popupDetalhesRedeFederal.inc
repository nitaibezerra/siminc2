<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

if ($_REQUEST['limpar_dados']) {
	unset($_SESSION['graficos_pronatec'], $_SESSION['grupo_pronatec']);
	echo "<script>window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesRedeFederal&acao=A'</script>";
}

if ('gerar_grafico' == $_REQUEST['tipo']) {
	if($_REQUEST['filtro']){
		$_SESSION['grupo_pronatec'][$_REQUEST['seq']] = $_REQUEST['grupo'];
		$_SESSION['graficos_pronatec'][$_REQUEST['seq']] = array($_REQUEST['grupo']=>$_REQUEST['filtro']);
		$_REQUEST['grupo'] = null;
	}
	
	gerarDetalheGrafico($_REQUEST['seq'], $_REQUEST['grupo']);
	die;
}

function gerarDetalheGrafico($seq, $grupo = '')
{
	global $db;

	$aGrupo = array('acao', 'tipo', 'situacao');
	$gruposRestantes = array_diff($aGrupo, (array) $_SESSION['grupo_pronatec']);
	
	$grupo = $grupo ? $grupo : current($gruposRestantes);
	
	if(!count($gruposRestantes)){ return ''; }
	
	$where = array();
	if(is_array($_SESSION['graficos_pronatec'])) {
		foreach ($_SESSION['graficos_pronatec'] as $graficos_pronatec) {
			foreach ($graficos_pronatec as $chave => $valor) {
				if($chave){
					$valor = str_replace('___', ' ', $valor);
					$where[] = " $chave = '$valor'";
				}
			}
		}
	}
	
	$agrupamento = '';
	switch ($chave) {
		case ('acao'):  	 $agrupamento = 'Iniciativa: ' . $valor; 	 	break;
		case ('tipo'):  	 $agrupamento = 'Tipo de Curso: ' . $valor; 	 	break;
		case ('situacao'):   $agrupamento = 'Categoria de Situação: ' . $valor; break;
		default: $agrupamento = 'Geral';
	}
	
	$sql = "SELECT acao, tipo, situacao, sum(valor) AS valor
			FROM (
				SELECT
					'Bolsa-Formação' AS acao,
					'FIC' AS tipo,
					".montaColunaCategoriaPronatec()." AS situacao,
					SUM(dsh.dshqtde::integer) AS valor
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				WHERE sh.indid IN (2300)
				AND dsh.tidid1 IN (4874)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, situacao
			UNION ALL
				SELECT
					'Bolsa-Formação' AS acao,
					'Técnico' AS tipo,
					".montaColunaCategoriaPronatec()." AS situacao,
					SUM(dsh.dshqtde::integer) AS valor
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				WHERE sh.indid IN (2333)
				AND dsh.tidid1 IN (4998)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, situacao
			UNION ALL
				SELECT
					'Bolsa-Formação' AS acao,
					'Técnico' AS tipo,
					".montaColunaCategoriaPronatec()." AS situacao,
					SUM(dsh.dshqtde::integer) AS valor
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				WHERE sh.indid IN (2302)
				AND dsh.tidid1 IN (4893)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, situacao
			UNION ALL
				SELECT
					'E-TEC' AS acao,
					'Técnico' AS tipo,
					".montaColunaCategoriaPronatec()." AS situacao,
					SUM(dsh.dshqtde::integer) AS valor
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2393)
				AND dsh.tidid1 IN (5209)
				AND dpe.dpeanoref >= '2011'
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, situacao
			UNION ALL
				SELECT
					'Regular' AS acao,
					CASE
						WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
						WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
						WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
						WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
						WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
					END AS tipo,
					".montaColunaCategoriaPronatec()." AS situacao,
					SUM(dsh.dshqtde::integer) AS valor
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2347)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, situacao
			) AS foo
				";

 	$sql .= count($where) ? ' where ' . implode(' AND ', $where) : '';
	
	$sql .= " GROUP BY acao, tipo, situacao
			  ORDER BY acao, tipo, situacao
				";	
	
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
	
	$next = $seq+1;
	
	echo '
	<div id="accordion_' . $seq . '">
		<h3><a id="titulo_' . $seq . '">' . $agrupamento . '</a></h3>
		<div>';
			?>
			
			<div style="clear:both;width:100%; margin:20px 0;">
				<?php 
				if (in_array('acao', $gruposRestantes)){ ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="acao" id="grupo_acao_<?php echo $seq; ?>" <?php echo $grupo == 'acao' ? 'checked="checked"': ''; ?> /><label for="grupo_acao_<?php echo $seq; ?>">Iniciativa</label></span>
				<?php } ?>
				
				<?php if (in_array('tipo', $gruposRestantes)) { ?>
            	    <span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="tipo" id="grupo_demandante_<?php echo $seq; ?>" <?php echo $grupo == 'tipo' ? 'checked="checked"': ''; ?> /><label for="grupo_tipo_<?php echo $seq; ?>">Tipo de Curso</label></span>
				<?php } ?>
				
				<?php if (in_array('situacao', $gruposRestantes)) { ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="situacao" id="grupo_situacao_<?php echo $seq; ?>" <?php echo $grupo == 'situacao' ? 'checked="checked"': ''; ?> /><label for="grupo_situacao_<?php echo $seq; ?>">Categoria de Situação</label></span>
				<?php } ?>
            </div>
			
			<?php
			if ($arrDados) { 
				geraGraficoCallback($arrDados, "graficoPizza{$seq}", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", 'js-acao', 'montarDetalheGrafico(' . $next . ', "' . $grupo . '"', null, null, true);
			}
			
		echo '
		</div>
	</div>';
		
	echo "<div id='grafico_{$next}' seq='{$seq}'></div>";
	
	echo '
	<script>
		jQuery("#accordion_' . $seq . '").accordion({collapsible: true, heightStyle: "content"});';		
		if($seq != 0){
		    echo 'jQuery("#accordion_' . ($seq - 1) . '").accordion({ active: 1 });';
		}

	echo 'jQuery("#accordion_' . $seq . '").accordion({ active: 0 });
	</script>
	';
}

?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=7" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <link rel="stylesheet" type="text/css" href="/library/jquery/jquery-ui-1.10.3/themes/dark-hive/jquery-ui-1.10.3.custom.min.css"/>
    
    <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
	<script language="javascript" type="text/javascript" src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>    
    <script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
    <script language="javascript" type="text/javascript" src="/estrutura/js/funcoes.js"></script>
    <script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
    <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

    <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
    <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

    <script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

    <script language="javascript" src="/estrutura/js/funcoes.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>

    <script>
        jQuery.noConflict();

        atualizaUsuario();
    </script>

    <style type="text/css">
        #div-ciclos{height: 900px;}
        #div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')};

        .tabela_listagem {
            background-color: #FFFFFF;
            color: #000000;
        }

        .filtro_listagem{
           width: 70%;
        }
        
        .span_grupo{
        	margin-right: 20px;
        }
    </style>

    <script>
    	jQuery.noConflict();
    	jQuery('.div_fotos_interno').cycle({
    		fx: 'scrollDown'
    	});

        function submeterFormularioListagem(){
            jQuery('#formulario_filtro_listagem').attr('action', window.location.href).submit();
        }

        jQuery(function(){

            jQuery('.ordenador_listagem').click(function(){
                jQuery('#ordenador_listagem').val(jQuery(this).attr('ordenar'));
                submeterFormularioListagem();
            });

            jQuery('.img_filtro_listagem').click(function(){
                submeterFormularioListagem();
            });

            jQuery('.filtro_listagem').keypress(function(e) {
                if(e.which == 13) {
                    submeterFormularioListagem();
                }
            });

            jQuery('#limpar_dados').click(function(){
            	window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesRedeFederal&acao=A&limpar_dados=1';                
            });

            jQuery('#div_graficos').on('click', '.detalhar_grafico', function(e) {
                var next = parseInt(jQuery(this).attr('seq')) + 1;
                jQuery('#grafico_' + next).load('/pde/estrategico.php?modulo=principal/popupDetalhesRedeFederal&acao=A&tipo=gerar_grafico&seq='+next);
            });
            
            jQuery('#div_graficos').on('click', '.filtro_grupo', function(e) {
                var seq = parseInt(jQuery(this).attr('seq'));
                jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesRedeFederal&acao=A&verificar_grupo=1&tipo=gerar_grafico&seq='+seq+'&grupo='+jQuery(this).val());
            });

        });

        function montarDetalheGrafico(seq, grupo, descricao)
        {
            descricao = str_replace(' ', '___', descricao);
            jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesRedeFederal&acao=A&tipo=gerar_grafico&seq='+seq+'&filtro='+descricao+'&grupo='+grupo+'&verificar_grupo=1');
        }
    </script>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<div style="float:right;cursor:pointer;" onclick="window.close();">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>

<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="8" ><div style="margin:28px" >Rede Federal</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link"  align="middle">
			<div style="text-align: left !important;"><input type="button" id="limpar_dados" value="Limpar Filtros" /></div>
		
			<div id="div_graficos">
				<div id='grafico_0'>
					<?php gerarDetalheGrafico(0, 'acao'); ?>
				</div>
			</div>
		</td>
	</tr>	
</table>
</body>
</html>