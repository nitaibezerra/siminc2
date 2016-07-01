<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
permissaoPerfilConsultaCockpit('estrategico.php?modulo=principal/cockpit_pronatec&acao=A');

$codper = $_REQUEST['codper'] ? $_REQUEST['codper'] : 2;
if($_REQUEST['codper']){
    $_SESSION['codper'] = $_REQUEST['codper'];
}

if($_SESSION['codper']){
    if($_SESSION['codper']==1){
        $periodoPronatec = "WHERE 1 = 1 ";
        $tituloperiodo = "TOTAL";
    }elseif($_SESSION['codper']==2){
        $periodoPronatec = "WHERE ano BETWEEN '2011' AND '2014' ";
        $tituloperiodo = " (2011 - 2014)";
    }elseif($_SESSION['codper']==3){
        $periodoPronatec = "WHERE ano BETWEEN '2015' AND '2018' ";
        $tituloperiodo = " (2015 - 2018)";
    }
}

if($_REQUEST['geral']){
	$_SESSION['pronatec_uf'] = "";
}else{
	if($_REQUEST['parametro']){
		$_SESSION['pronatec_uf'] = $_REQUEST['parametro'];
	}
}

if ($_REQUEST['limpar_dados']) {
	unset($_SESSION['graficos_pronatec'], $_SESSION['grupo_pronatec']);
	echo "<script>window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A'</script>";
}

if ('gerar_grafico' == $_REQUEST['tipo']) {
	
// 	if ($_REQUEST['verificar_grupo']) {
// 		foreach($_SESSION['grupo_pronatec'] as $seq => $grupo_pronatec){
// 			$next = $seq+1;
// 			if($_REQUEST['seq'] > $next){
// 				unset($_SESSION['grupo_pronatec'][$next]);
// 			}
// 		} 	
// 		foreach($_SESSION['graficos_pronatec'] as $seq => $graficos_pronatec){
// 			$next = $seq+1;
// 			if($_REQUEST['seq'] > $next){
// 				unset($_SESSION['graficos_pronatec'][$next]);
// 			}
// 		}
// 	}
	
	if($_REQUEST['filtro']){
		$_SESSION['grupo_pronatec'][$_REQUEST['seq']] = $_REQUEST['grupo'];
		$_SESSION['graficos_pronatec'][$_REQUEST['seq']] = array($_REQUEST['grupo']=>$_REQUEST['filtro']);
		$_REQUEST['grupo'] = null;
	}
	
	gerarDetalheGrafico($_REQUEST['seq'], $_REQUEST['grupo'], $periodoPronatec);
	die;
}

function gerarDetalheGrafico($seq, $grupo = '', $periodoPronatec)
{
	global $db;

	$aGrupo = array('acao', 'tipo', 'categoriaofertante', 'ofertante', 'categoria', 'situacao', 'ano');
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
		case ('categoriaofertante'):  $agrupamento = 'Ofertante: ' . $valor; break;
		case ('ofertante'):  $agrupamento = 'Ofertante Detalhado: ' . $valor; break;
		case ('categoria'):   $agrupamento = 'Situação: ' . $valor; break;
		case ('situacao'):   $agrupamento = 'Situação Detalhada: ' . $valor; break;
		case ('ano'):   	 $agrupamento = 'Ano de Ingresso: ' . $valor; break;
		default: $agrupamento = 'Geral';
	}
	
	$sql = "SELECT acao, tipo, categoriaofertante, ofertante, categoria, situacao, ano, sum(valor) AS valor
			FROM (
				SELECT
					'Acordo de Gratuidade' AS acao,
					'FIC' AS tipo,
					CASE
					    WHEN tid1.tiddsc IN ('Rede Federal', 'Redes Estaduais', 'Instituições municipais') THEN 'Redes Públicas'
					    WHEN tid1.tiddsc IN ('SENAC', 'SENAI', 'SENAR', 'SENAT') THEN 'Serviços Nacionais de Aprendizagem'
					    WHEN tid1.tiddsc IN ('Escolas técnicas privadas', 'IES privadas') THEN 'Instituições privadas'
					    ELSE tid1.tiddsc
					END AS categoriaofertante,
					tid1.tiddsc AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
					tid2.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2540)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			UNION ALL
				SELECT
					'Acordo de Gratuidade' AS acao,
					'Técnico' AS tipo,
					CASE
					    WHEN tid1.tiddsc IN ('Rede Federal', 'Redes Estaduais', 'Instituições municipais') THEN 'Redes Públicas'
					    WHEN tid1.tiddsc IN ('SENAC', 'SENAI', 'SENAR', 'SENAT') THEN 'Serviços Nacionais de Aprendizagem'
					    WHEN tid1.tiddsc IN ('Escolas técnicas privadas', 'IES privadas') THEN 'Instituições privadas'
					    ELSE tid1.tiddsc
					END AS categoriaofertante,
					tid1.tiddsc AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
					tid2.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2541)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			UNION ALL
				SELECT
					'Bolsa-Formação' AS acao,
					'FIC' AS tipo,
					CASE
					    WHEN tid1.tiddsc IN ('Rede Federal', 'Redes Estaduais', 'Instituições municipais') THEN 'Redes Públicas'
					    WHEN tid1.tiddsc IN ('SENAC', 'SENAI', 'SENAR', 'SENAT') THEN 'Serviços Nacionais de Aprendizagem'
					    WHEN tid1.tiddsc IN ('Escolas técnicas privadas', 'IES privadas') THEN 'Instituições privadas'
					    ELSE tid1.tiddsc
					END AS categoriaofertante,
					tid1.tiddsc AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
					tid2.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2300)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			UNION ALL
				SELECT
					'Bolsa-Formação' AS acao,
					'Técnico' AS tipo,
					CASE
					    WHEN tid1.tiddsc IN ('Rede Federal', 'Redes Estaduais', 'Instituições municipais') THEN 'Redes Públicas'
					    WHEN tid1.tiddsc IN ('SENAC', 'SENAI', 'SENAR', 'SENAT') THEN 'Serviços Nacionais de Aprendizagem'
					    WHEN tid1.tiddsc IN ('Escolas técnicas privadas', 'IES privadas') THEN 'Instituições privadas'
					    ELSE tid1.tiddsc
					END AS categoriaofertante,
					tid1.tiddsc AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
					tid2.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2333)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			UNION ALL
				SELECT
					'Bolsa-Formação' AS acao,
					'Técnico' AS tipo,
					CASE
					    WHEN tid1.tiddsc IN ('Rede Federal', 'Redes Estaduais', 'Instituições municipais') THEN 'Redes Públicas'
					    WHEN tid1.tiddsc IN ('SENAC', 'SENAI', 'SENAR', 'SENAT') THEN 'Serviços Nacionais de Aprendizagem'
					    WHEN tid1.tiddsc IN ('Escolas técnicas privadas', 'IES privadas') THEN 'Instituições privadas'
					    ELSE tid1.tiddsc
					END AS categoriaofertante,
					tid1.tiddsc AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
					tid2.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2302)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			UNION ALL
				SELECT
					'Brasil Profissionalizado' AS acao,
					'Técnico' AS tipo,
					'Redes Públicas' AS categoriaofertante,
					'Redes Estaduais' AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
                    tid1.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2539)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			UNION ALL
				SELECT
					'E-TEC' AS acao,
					'Técnico' AS tipo,
					CASE
					    WHEN tid1.tiddsc IN ('Rede Federal', 'Redes Estaduais', 'Instituições municipais') THEN 'Redes Públicas'
					    WHEN tid1.tiddsc IN ('SENAC', 'SENAI', 'SENAR', 'SENAT') THEN 'Serviços Nacionais de Aprendizagem'
					    WHEN tid1.tiddsc IN ('Escolas técnicas privadas', 'IES privadas') THEN 'Instituições privadas'
					    ELSE tid1.tiddsc
					END AS categoriaofertante,
					tid1.tiddsc AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
                    tid2.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2393)
				AND sh.sehstatus <> 'I'
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
		    UNION ALL
				SELECT
					'E-TEC' AS acao,
					'Técnico' AS tipo,
					'Redes Públicas' AS categoriaofertante,
					'Rede Federal' AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
					tid1.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2347)
				AND sh.sehstatus <> 'I'
				AND dsh.tidid2 IN (5132)
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			UNION ALL
				SELECT
					'Rede Federal EPT' AS acao,
					CASE WHEN dsh.tidid2 IN (5129) THEN 'FIC' ELSE 'Técnico' END AS tipo,
					'Redes Públicas' AS categoriaofertante,
					'Rede Federal' AS ofertante,
					".montaColunaCategoriaPronatec()." AS categoria,
					tid1.tiddsc AS situacao,
					dpe.dpeanoref AS ano,
					SUM(dsh.dshqtde::integer) AS valor,
					dsh.dshuf AS uf
				FROM painel.seriehistorica sh
				INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
				INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
				INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
				WHERE sh.indid IN (2347)
				AND sh.sehstatus <> 'I'
				AND dsh.tidid2 IN (5130,5131,5188,5190,5189,5129)
				GROUP BY tipo, categoriaofertante, ofertante, categoria, situacao, ano, uf
			) AS foo
				";
	
	$sql .= $periodoPronatec;
	$sql .= $_SESSION['pronatec_uf'] ? " AND uf = '{$_SESSION['pronatec_uf']}' " : "";
 	$sql .= count($where) ? ' AND ' . implode(' AND ', $where) : '';
	
	$sql .= " GROUP BY acao, tipo, categoriaofertante, ofertante, categoria, situacao, ano
			  ORDER BY ano, acao, tipo, categoriaofertante, ofertante, categoria, situacao
				";
	$arrDados = $db->carregar($sql);
    $_SESSION['exportar1'] = $arrDados;

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
            	    
                <?php if (in_array('categoriaofertante', $gruposRestantes)) { ?>
                    <span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="categoriaofertante" id="grupo_categoriaofertante_<?php echo $seq; ?>" <?php echo $grupo == 'categoriaofertante' ? 'checked="checked"': ''; ?> /><label for="grupo_categoriaofertante_<?php echo $seq; ?>">Ofertante</label></span>
                <?php } ?>

                <?php if (in_array('ofertante', $gruposRestantes)) { ?>
                    <span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="ofertante" id="grupo_ofertante_<?php echo $seq; ?>" <?php echo $grupo == 'ofertante' ? 'checked="checked"': ''; ?> /><label for="grupo_ofertante_<?php echo $seq; ?>">Ofertante Detalhado</label></span>
                <?php } ?>
				
				<?php if (in_array('categoria', $gruposRestantes)) { ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="categoria" id="grupo_categoria_<?php echo $seq; ?>" <?php echo $grupo == 'categoria' ? 'checked="checked"': ''; ?> /><label for="grupo_categoria_<?php echo $seq; ?>">Situação</label></span>
				<?php } ?>
                <?php if (in_array('situacao', $gruposRestantes)) { ?>
                    <span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="situacao" id="grupo_situacao_<?php echo $seq; ?>" <?php echo $grupo == 'situacao' ? 'checked="checked"': ''; ?> /><label for="grupo_situacao_<?php echo $seq; ?>">Situação Detalhada</label></span>
                <?php } ?>
				<?php if (in_array('ano', $gruposRestantes)) { ?>
                	<span class="span_grupo"><input seq="<?php echo $seq; ?>" type="radio" name="grupo" class="filtro_grupo" value="ano" id="grupo_ano_<?php echo $seq; ?>" <?php echo $grupo == 'ano' ? 'checked="checked"': ''; ?> /><label for="grupo_ano_<?php echo $seq; ?>">Ano de Ingresso</label></span>
				<?php } ?>
            </div>
			
			<?php
			if ($arrDados) {
				if($_SESSION['pronatec_uf']){
					$estuf = $_SESSION['pronatec_uf'];
					if($estuf == 'AL' || $estuf == 'MG' || $estuf == 'PE' || $estuf == 'RO' || $estuf == 'RR' || $estuf == 'SC' || $estuf == 'SE' || $estuf == 'SP'){
						$preposicao = "de";
					}elseif($estuf == 'BA' || $estuf == 'PB'){
						$preposicao = "da";
					}else{
						$preposicao = "do";
					}
					$sql = "SELECT 'Estado {$preposicao} ' || estdescricao AS titulo FROM territorios.estado WHERE estuf = '{$estuf}' ";
					$titulo = $db->pegaUm($sql);
				}
				geraGraficoCallback($arrDados, "graficoPizza{$seq}", $titulo,"<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", 'js-acao', 'montarDetalheGrafico(' . $next . ', "' . $grupo . '"', null, null, true);
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
            	window.location.href = '/pde/estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A&limpar_dados=1';
            });

            jQuery('#div_graficos').on('click', '.detalhar_grafico', function(e) {
                var next = parseInt(jQuery(this).attr('seq')) + 1;
                jQuery('#grafico_' + next).load('/pde/estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A&tipo=gerar_grafico&seq='+next);
            });
            
            jQuery('#div_graficos').on('click', '.filtro_grupo', function(e) {
                var seq = parseInt(jQuery(this).attr('seq'));
                jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A&verificar_grupo=1&tipo=gerar_grafico&seq='+seq+'&grupo='+jQuery(this).val());
            });

        });

        function montarDetalheGrafico(seq, grupo, descricao)
        {
            descricao = str_replace(' ', '___', descricao);
            jQuery('#grafico_' + seq).load('/pde/estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A&tipo=gerar_grafico&seq='+seq+'&filtro='+descricao+'&grupo='+grupo+'&verificar_grupo=1');
        }

        function visualizarXLS(tipo){
            window.location='estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A&tipo='+tipo;
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
		<td class="fundo_titulo" style="text-align:center" colspan="8" ><div style="margin:28px" >Pronatec<br><?=$tituloperiodo?></div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link"  align="middle">
			<div style="text-align: left !important;">
                <input type="button" id="limpar_dados" value="Limpar Filtros" />&nbsp;&nbsp;
                <input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(1);" style="cursor: pointer;"/>
            </div>
		
			<div id="div_graficos">
				<div id='grafico_0'>
					<?php gerarDetalheGrafico(0, 'acao', $periodoPronatec); ?>
				</div>
			</div>
		</td>
	</tr>
</table>
</body>
</html>
<?php
//GERAR DADOS EM EXCEL
$gerarExcel = $_REQUEST['tipo'];
switch($gerarExcel){
    case 1:
        //Evolução de matrículas atendidas
        ob_clean();
        $db->sql_to_excel( $_SESSION['exportar1'], "Pronatec");
        break;
}
?>