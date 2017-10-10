<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

if ($_REQUEST['indid']) {
	$_SESSION['indidsuperior'] = $_REQUEST['indid'];
	if($_SESSION['indidsuperior']==439){
		$titulo = "Matrículas no Ensino Superior";
	}else{
		$titulo = "Concluintes no Ensino Superior";
	}
}

if ($_REQUEST['limpar_dados']) {
	unset($_SESSION['graficos_pronatec'], $_SESSION['grupo_pronatec']);
	echo "<script>window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesEnsinoSuperior&acao=A&indid='+".$_SESSION['indidsuperior']."</script>";
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

	$aGrupo = array('dependencia', 'modalidade', 'nivel');
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
		case ('dependencia'):  	 $agrupamento = 'Dependência Administrativa: ' . $valor; 	 	break;
		case ('modalidade'):  	 $agrupamento = 'Modalidade de ensino: ' . $valor; 	 	break;
		case ('nivel'):   $agrupamento = 'Nível Acadêmico: ' . $valor; break;
		default: $agrupamento = 'Geral';
	}
	
	$sql = "SELECT dependencia, modalidade, nivel, SUM(valor) AS valor
			FROM(
				SELECT
					tid2.tiddsc AS dependencia,
					CASE
						WHEN dsh.tidid1 IN (640, 1848, 648, 1842) THEN 'Presencial'
						WHEN dsh.tidid1 IN (641, 1849, 649, 1843) THEN 'Distância'
					END AS modalidade,
					CASE
						WHEN dsh.tidid1 IN (640, 641, 649, 648) THEN 'Gradução'
						WHEN dsh.tidid1 IN (1848, 1849, 650, 1842, 1843) THEN 'Sequencial'
					END AS nivel,
					SUM(dsh.dshqtde::integer) AS valor
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
				WHERE sh.indid IN (".$_SESSION['indidsuperior'].")
				AND sh.sehstatus <> 'I'
				AND dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
				GROUP BY dependencia, modalidade, nivel
			) AS FOO
				";

 	$sql .= count($where) ? ' where ' . implode(' AND ', $where) : '';
	
	$sql .= " GROUP BY dependencia, modalidade, nivel
			  ORDER BY dependencia, modalidade, nivel
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
				if (in_array('dependencia', $gruposRestantes)){ ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="dependencia" id="grupo_dependencia_<?php echo $seq; ?>" <?php echo $grupo == 'dependencia' ? 'checked="checked"': ''; ?> /><label for="grupo_dependencia_<?php echo $seq; ?>">Dependência Administrativa</label></span>
				<?php } ?>
				
				<?php if (in_array('modalidade', $gruposRestantes)) { ?>
            	    <span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="modalidade" id="grupo_demandante_<?php echo $seq; ?>" <?php echo $grupo == 'modalidade' ? 'checked="checked"': ''; ?> /><label for="grupo_modalidade_<?php echo $seq; ?>">Modalidade de ensino</label></span>
				<?php } ?>
				
				<?php if (in_array('nivel', $gruposRestantes)) { ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="nivel" id="grupo_nivel_<?php echo $seq; ?>" <?php echo $grupo == 'nivel' ? 'checked="checked"': ''; ?> /><label for="grupo_nivel_<?php echo $seq; ?>">Nível Acadêmico</label></span>
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
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}

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
            	window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesEnsinoSuperior&acao=A&limpar_dados=1';                
            });

            jQuery('#div_graficos').on('click', '.detalhar_grafico', function(e) {
                var next = parseInt(jQuery(this).attr('seq')) + 1;
                jQuery('#grafico_' + next).load('/pde/estrategico.php?modulo=principal/popupDetalhesEnsinoSuperior&acao=A&tipo=gerar_grafico&seq='+next);
            });
            
            jQuery('#div_graficos').on('click', '.filtro_grupo', function(e) {
                var seq = parseInt(jQuery(this).attr('seq'));
                jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesEnsinoSuperior&acao=A&verificar_grupo=1&tipo=gerar_grafico&seq='+seq+'&grupo='+jQuery(this).val());
            });

        });

        function montarDetalheGrafico(seq, grupo, descricao)
        {
            descricao = str_replace(' ', '___', descricao);
            jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesEnsinoSuperior&acao=A&tipo=gerar_grafico&seq='+seq+'&filtro='+descricao+'&grupo='+grupo+'&verificar_grupo=1');
        }
    </script>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<div style="float:right;cursor:pointer;" onclick="window.close();">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>

<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="8" ><div style="margin:28px" ><?=$titulo?></div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link"  align="middle">
			<div style="text-align: left !important;"><input type="button" id="limpar_dados" value="Limpar Filtros" /></div>
		
			<div id="div_graficos">
				<div id='grafico_0'>
					<?php gerarDetalheGrafico(0, 'dependencia'); ?>
				</div>
			</div>
		</td>
	</tr>	
</table>
</body>
</html>