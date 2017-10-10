<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

if ($_REQUEST['limpar_dados']) {
	unset($_SESSION['graficos_pronatec'], $_SESSION['grupo_pronatec']);
	echo "<script>window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesProinfancia&acao=A'</script>";
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

	$aGrupo = array('empresa', 'fluxo', 'situacao', 'motivo', 'ano');
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
		case ('empresa'):  	 $agrupamento = 'Empresa: ' . $valor; 	 	break;
		case ('fluxo'):  	 $agrupamento = 'Fluxo: ' . $valor; 	 	break;
        case ('situacao'):   $agrupamento = 'Situação: ' . $valor; break;
        case ('motivo'):  $agrupamento = 'Motivo: ' . $valor; break;
        default: $agrupamento = 'Total de Adesões MI';
	}
	
	$sql = "SELECT empresa, fluxo, motivo, situacao, SUM(valor) AS valor
			FROM (
				SELECT
					CASE cnpj
						WHEN '' THEN 'CONSÓRCIO PIB'
						WHEN '' THEN 'MVC COMPONENTES PLASTICOS LTDA'
						WHEN '' THEN 'CONSÓRCIO JGA / VENÂNCIO'
						WHEN '' THEN 'CASAALTA CONSTRUCOES LTDA'
					END AS empresa,
					cobfluxo AS fluxo,
					cobmotivo AS motivo,
					esddsc AS situacao,
					count(0) AS valor
				FROM obras2.obras o
				INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
				INNER JOIN workflow.documento d ON d.docid = o.docid
				INNER JOIN
					(SELECT MAX(aosdtalteracao), aoscnpjfornecedor AS cnpj, preid, aosnomefornecedor FROM par.adesaoobraspacsituacao GROUP BY cnpj, preid, aosnomefornecedor) AS f ON f.preid = o.preid
				INNER JOIN obras.preobra pre ON pre.preid = f.preid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
				INNER JOIN pde.cockpit_obrasmi cob ON cob.preid = pre.preid
				WHERE o.obrstatus = 'A'
				AND e.orgid=3
				AND e.prfid=41
				AND o.obridpai is null
				AND d.esdid NOT IN (884, 768)
				AND o.tooid in (1,2,4)
				AND o.tpoid IN (104,105)
				GROUP BY empresa, fluxo, motivo, situacao
			) AS FOO
				";
	

 	$sql .= count($where) ? ' where ' . implode(' AND ', $where) : '';
	
	$sql .= " GROUP BY empresa, fluxo, motivo, situacao
			  ORDER BY empresa, fluxo, motivo, situacao
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
				if (in_array('empresa', $gruposRestantes)){ ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="empresa" id="grupo_empresa_<?php echo $seq; ?>" <?php echo $grupo == 'empresa' ? 'checked="checked"': ''; ?> /><label for="grupo_empresa_<?php echo $seq; ?>">Empresa</label></span>
				<?php } ?>

				<?php if (in_array('fluxo', $gruposRestantes)) { ?>
            	    <span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="fluxo" id="grupo_fluxo_<?php echo $seq; ?>" <?php echo $grupo == 'fluxo' ? 'checked="checked"': ''; ?> /><label for="grupo_fluxo_<?php echo $seq; ?>">Fluxo</label></span>
				<?php } ?>

				<?php if (in_array('situacao', $gruposRestantes)) { ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="situacao" id="grupo_situacao_<?php echo $seq; ?>" <?php echo $grupo == 'situacao' ? 'checked="checked"': ''; ?> /><label for="grupo_situacao_<?php echo $seq; ?>">Situação</label></span>
				<?php } ?>

                <?php if (in_array('motivo', $gruposRestantes)) { ?>
                    <span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="motivo" id="grupo_motivo_<?php echo $seq; ?>" <?php echo $grupo == 'motivo' ? 'checked="checked"': ''; ?> /><label for="grupo_motivo_<?php echo $seq; ?>">Motivo</label></span>
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
        .fundo_titulo{background-image:url('../imagens/fundocreche.jpg');background-repeat:repeat-xt;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}

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
            	window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesProinfancia&acao=A&limpar_dados=1';
            });

            jQuery('#div_graficos').on('click', '.detalhar_grafico', function(e) {
                var next = parseInt(jQuery(this).attr('seq')) + 1;
                jQuery('#grafico_' + next).load('/pde/estrategico.php?modulo=principal/popupDetalhesProinfancia&acao=A&tipo=gerar_grafico&seq='+next);
            });

            jQuery('#div_graficos').on('click', '.filtro_grupo', function(e) {
                var seq = parseInt(jQuery(this).attr('seq'));
                jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesProinfancia&acao=A&verificar_grupo=1&tipo=gerar_grafico&seq='+seq+'&grupo='+jQuery(this).val());
            });

        });

        function montarDetalheGrafico(seq, grupo, descricao)
        {
            descricao = str_replace(' ', '___', descricao);
            jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesProinfancia&acao=A&tipo=gerar_grafico&seq='+seq+'&filtro='+descricao+'&grupo='+grupo+'&verificar_grupo=1');
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
		<td class="fundo_titulo" style="text-align:center" colspan="8" ><div style="margin:28px" >Acompanhamento de Obras</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link"  align="middle">
			<div style="text-align: left !important;"><input type="button" id="limpar_dados" value="Limpar Filtros" /></div>

			<div id="div_graficos">
				<div id='grafico_0'>
					<?php gerarDetalheGrafico(0, 'empresa'); ?>
				</div>
			</div>
		</td>
	</tr>
</table>
</body>
</html>