<?php 

$MOSTRA_TEMPO_EXECUCAO = $_REQUEST['requisicao']!='exibirxls' ? true : false;

if($MOSTRA_TEMPO_EXECUCAO){
	// Iniciamos o "contador"
	list($usec, $sec) = explode(' ', microtime());
	$script_start = (float) $sec + (float) $usec;
}
?>

<?php 

$TOTAL_COLUNAS_ENVIO 	= $_REQUEST['qtdemeses'] ? $_REQUEST['qtdemeses'] : 3;
$TOTAL_COLUNAS_VISITA 	= $_REQUEST['qtdemeses'] ? $_REQUEST['qtdemeses'] : 3;

if($_REQUEST['requisicao']=='gerarxlserro'){
	$sql = "select
			case when nomunicipioprofissional is null then 'Erro: município não preenchido'
				 when to_char(dtvisita, 'YYYYMM') < '201310' or to_char(dtvisita, 'YYYYMM') > to_char(now(), 'YYYYMM') then 'Erro: data de visita errada'
				 when dtvisita is null then 'Erro: data de visita vazia' end as erro,
				*
			from maismedicos.ws_respostas_formulario
			where wssstatus = 'A'
			and status = 'Finalizado'
			and itens_processado = true
			and idformulario in (35,36,40,43) 
			and (to_char(dtvisita, 'YYYYMM') < '201310' or to_char(dtvisita, 'YYYYMM') > to_char(now(), 'YYYYMM') 
					or dtvisita is null or muncodprofissional is null)
			";
	
	$arTipo = array();
	gerar_excel($sql, null, 'SIMEC_PMMB_FORMULARIOS_COM_ERRO_'.date("Ymdhis"), $arTipo);
	die;
}
                
extract($_REQUEST);

// UF
if($estuf){
	$label .= '<tr><td align="right"><b>UF</b></td><td align="left">'.$estuf.'</td></tr>';
	$arWhere[] = " f.estufprofissional = '{$estuf}' ";
}

// Municipio
if( $muncod[0] && $muncod_campo_flag ){
	$label .= '<tr><td align="right"><b>Município(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mundescricao from territorios.municipio where muncod in ('".implode( "','", $muncod )."')")).'</td></tr>';
	$arWhere[] = " f.muncodprofissional ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
}

// So capitais
if($socapitais == 'on'){
	$label .= '<tr><td align="right"><b>Só capitais</b></td><td align="left">Sim</td></tr>';
	$arWhere[] = " f.muncodprofissional in ('3550308','3304557','2927408','5300108','2304400','3106200','1302603','4106902','2611606','4314902','1501402','5208707',
											'2111300','2704302','2408102','2211001','5002704','2507507','2800308','5103403','1100205','4205407','1600303','1200401',
											'3205309','1400100','1721000')";
}

// Instituicao Supervisora
if( $uniid[0] && $uniid_campo_flag ){
	$label .= '<tr><td align="right"><b>Instituição(ões) Supervisora(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select uninome from maismedicos.universidade where uniid in ('".implode( "','", $uniid )."')")).'</td></tr>';
	$arWhere[] = " f.muncodprofissional in (select muncod from maismedicos.universidademunicipio where uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $uniid )."' )) ";
}

// Periodo
$mesvisitainicio = str_pad($mesvisitainicio, 2, "0", STR_PAD_LEFT);
$mesvisitafim = str_pad($mesvisitafim, 2, "0", STR_PAD_LEFT);

if($mesvisitainicio && $anovisitainicio && empty($mesvisitafim) && empty($anovisitafim)){
	
	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.'</td></tr>';

	$arWhereEnvio[] = " to_char(f.dtenvio, 'YYYYMM') = '{$anovisitainicio}{$mesvisitainicio}' ";
	$arWhereVisita[] = " to_char(f.dtvisita, 'YYYYMM') = '{$anovisitainicio}{$mesvisitainicio}' ";

}elseif($mesvisitainicio && $anovisitainicio && $mesvisitafim && $anovisitafim){

	$label .= '<tr><td align="right"><b>Período</b></td><td align="left">'.$mesvisitainicio.'/'.$anovisitainicio.' até '.$mesvisitafim.'/'.$anovisitafim.'</td></tr>';
	
	$arWhereEnvio[] = " to_char(f.dtenvio, 'YYYYMM') >= '{$anovisitainicio}{$mesvisitainicio}' and to_char(f.dtenvio, 'YYYYMM') <= '{$anovisitafim}{$mesvisitafim}' ";
	$arWhereVisita[] = " to_char(f.dtvisita, 'YYYYMM') >= '{$anovisitainicio}{$mesvisitainicio}' and to_char(f.dtvisita, 'YYYYMM') <= '{$anovisitafim}{$mesvisitafim}' ";

}else{
	
	if($mostrardtvazia == 'on'){
		$label .= '<tr><td align="right"><b>Mostrar data de visita vazia</b></td><td align="left">Sim</td></tr>';
		$dtVisitaFiltro[] = " (f.dtvisita is null) ";
	}
	
	if($mostrardterro == 'on'){
		$label .= '<tr><td align="right"><b>Mostrar data de visita com erro</b></td><td align="left">Sim</td></tr>';
		$dtVisitaFiltro[] = " (to_char(f.dtvisita, 'YYYYMM') < '201310' or to_char(f.dtvisita, 'YYYYMM') > to_char(now(), 'YYYYMM')) ";
	}
	
	$dtVisitaFiltro[] = " (to_char(f.dtvisita, 'YYYYMM') >= '201310' and to_char(f.dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM')) ";
	
	$arWhereEnvio[] = " to_char(f.dtenvio, 'YYYYMM') >= '201310' and to_char(f.dtenvio, 'YYYYMM') <= to_char(now(), 'YYYYMM') ";
// 	$arWhereVisita[] = " to_char(f.dtvisita, 'YYYYMM') >= '201310' and to_char(f.dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM') ";	
	$arWhereVisita[] = " (".implode(' or ', $dtVisitaFiltro).") ";
	
}

// Mostrar DSEI 
if($mostrardsei == 'on'){
	
	$label .= '<tr><td align="right"><b>Mostrar DSEI</b></td><td align="left">Sim</td></tr>';
	
	$whereTotal = " f.idformulario in (".FORM_PRIMEIRA_VISITA.",".
										FORM_SUPERVISAO_PRATICA.",".
										FORM_PRIMEIRA_VISITA_DSEI.",".
										FORM_SUPERVISAO_PRATICA_DSEI.") ";
	
	$arWhere[] = $whereTotal;
	$arWhereTotal[] = $whereTotal;
	
	$arRelatorios = array(
					FORM_PRIMEIRA_VISITA		 => 'Primeira Visita de Supervisão',
					FORM_SUPERVISAO_PRATICA		 => 'Supervisão Prática',
					FORM_PRIMEIRA_VISITA_DSEI	 => 'Primeira Visita de Supervisão "IN LOCO" – DSEI',
					FORM_SUPERVISAO_PRATICA_DSEI => 'Supervisão Prática – DSEI'
				);
}else{
	
	$whereTotal = " f.idformulario in (".FORM_PRIMEIRA_VISITA.",".
										FORM_SUPERVISAO_PRATICA.") ";
	$arWhere[] = $whereTotal;
	$arWhereTotal[] = $whereTotal;
	
	$arRelatorios = array(
					FORM_PRIMEIRA_VISITA		 => 'Primeira Visita de Supervisão',
					FORM_SUPERVISAO_PRATICA		 => 'Supervisão Prática'
				);
}

// Supervisor
if( $supervisor[0] && $supervisor_campo_flag ){
	$label .= '<tr><td align="right"><b>Supervisor(es)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select tutnome from maismedicos.tutor where tutcpf in ('".implode( "','", $supervisor )."')")).'</td></tr>';
	$arWhere[] = " f.cpfsupervisor ". (!$supervisor_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $supervisor )."' ) ";
}

// Medico
if( $medico[0] && $medico_campo_flag ){
	$label .= '<tr><td align="right"><b>Médico(s)</b></td><td align="left">'.implode('<br/>', $db->carregarColuna("select mdcnome from maismedicos.medico where mdccpf in ('".implode( "','", $medico )."')")).'</td></tr>';
	$arWhere[] = " f.cpfprofissional ". (!$medico_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $medico )."' ) ";
}

$arPerfil = arrayPerfil();
if(in_array(PERFIL_TUTOR, $arPerfil)){
	$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
	$arWhereTotal[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
}else
	if(in_array(PERFIL_SUPERVISOR, arrayPerfil())){
	$arWhere[] = "f.cpfsupervisor = '{$_SESSION['usucpf']}'";
}

$sqlEnvio = "select 	
			f.idformulario,
			to_char(f.dtenvio,'YYYYMM') as dtenvio,
			coalesce(count(distinct f.idpublicarformulario),0) as qtde_visita_mes
		from maismedicos.ws_respostas_formulario f
		left join maismedicos.usuarioresponsabilidade usr on usr.uniid = f.uniid
				and usr.pflcod in (".implode(",",$arPerfil).")
		where f.wssstatus = 'A'
		and f.status = 'Finalizado'
		".($arWhere ? ' and '.implode(' and ', $arWhere) : '')."
		".($arWhereEnvio ? ' and '.implode(' and ', $arWhereEnvio) : '')."
		group by to_char(f.dtenvio,'YYYYMM'), f.idformulario
		order by to_char(f.dtenvio,'YYYYMM')";

// Carrega os dados
$rsEnvio = $db->carregar($sqlEnvio);

// Cria as variaveis para montar o relatorio de envio
if($rsEnvio){
	foreach($rsEnvio as $envio){
		$arEnvio[$envio['idformulario']][$envio['dtenvio']] = $envio['qtde_visita_mes'];
		$arMesesEnvioTmp[$envio['dtenvio']] = $envio['dtenvio'];
	}
}

// Ordena os meses descrecente
if($arMesesEnvioTmp) rsort($arMesesEnvioTmp);

// Separa os meses de envio conforme qtde colunas
$totalMesesEnvio=0;
if($arMesesEnvioTmp){
	foreach($arMesesEnvioTmp as $k=>$v){
		if($totalMesesEnvio<$TOTAL_COLUNAS_ENVIO){
			$arMesesEnvio[$v] = $v;
		}else{
			$arMesesEnvioOutros[$v] = $v;
		}
		$totalMesesEnvio++;
	}
}

$sqlVisita = "select 	
			f.idformulario,
			to_char(f.dtvisita,'YYYYMM') as dtvisita,
			coalesce(count(distinct f.idpublicarformulario),0) as qtde_visita_mes	
		from maismedicos.ws_respostas_formulario f
		left join maismedicos.usuarioresponsabilidade usr on usr.uniid = f.uniid
				and usr.pflcod in (".implode(",",$arPerfil).")
		where f.wssstatus = 'A'
		and f.status = 'Finalizado'		
		".($arWhere ? 'and '.implode(' and ', $arWhere) : '')."
		".($arWhereVisita ? 'and '.implode(' and ', $arWhereVisita) : '')."
		group by to_char(f.dtvisita,'YYYYMM'), f.idformulario
		order by to_char(f.dtvisita,'YYYYMM') desc";
// ver($sqlVisita, d);
// Carrega os dados
$rsVisita = $db->carregar($sqlVisita);

// Cria as variaveis para montar o relatorio de visita
if($rsVisita){
	foreach($rsVisita as $visita){
		$visita['dtvisita'] = empty($visita['dtvisita']) ? '999999' : $visita['dtvisita'];
		$arVisita[$visita['idformulario']][$visita['dtvisita']] = $visita['qtde_visita_mes'];
		$arMesesVisitaTmp[$visita['dtvisita']] = $visita['dtvisita'];
	}
}

// Ordena os meses descrecente
if($arMesesVisitaTmp) rsort($arMesesVisitaTmp);

// Separa os meses de envio conforme qtde colunas
$totalMesesVisita=0;
if($arMesesVisitaTmp){
	foreach($arMesesVisitaTmp as $k=>$v){
		if($totalMesesVisita<$TOTAL_COLUNAS_VISITA){
			$arMesesVisita[$v] = $v;
		}else{
			$arMesesVisitaOutros[$v] = $v;
		}
		$totalMesesVisita++;
	}
}

$sqlTotal = "select 
				f.idformulario,
				coalesce(count(distinct f.idpublicarformulario),0) as total
			from maismedicos.ws_respostas_formulario f
			left join maismedicos.usuarioresponsabilidade usr on usr.uniid = f.uniid
				and usr.pflcod in (".implode(",",$arPerfil).")
			where f.wssstatus = 'A' 
			and f.status = 'Finalizado'
			".($arWhereTotal ? 'and '.implode(' and ', $arWhereTotal) : '')."	
			group by f.idformulario
			order by f.idformulario";

$rsTotal = $db->carregar($sqlTotal);

// Monta as variaveis dos totais
if($rsTotal){
	foreach($rsTotal as $total){
		$arTotal[$total['idformulario']] = $total['total'];
	}
}

$cssColunasQtde = "";

// ver($sqlVisita, $sqlEnvio);

if($_REQUEST['requisicao']=='exibirxls'){

	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PMMB_PANORAMA_POSTAGENS_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PMMB_PANORAMA_POSTAGENS_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );	
	
	$atTabelaListagem = 'border="1"';
	
}else{

	$atTabelaListagem = 'class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width:95%"';
?>

<html>
	<head>
		<title> Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<script src="/includes/JQuery/jquery-1.10.2.min.js"></script>
	    <script src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
		<script src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
	    <script>

    		<?php
    	 
    		if($arMesesVisita){
	    		
	    		// Variaveis do grafico
	    		$grMeses = '';
	    		$grBarrasPriPostagens = '';
	    		$grBarrasSupPostagens = '';
	    		$grBarrasPriPostagensDsei = '';
	    		$grBarrasSupPostagensDsei = '';
	    		
	    		// Contadores
	    		$colunaGrafico = 0;		    		 
	    		$totalColunasGrafico = count($arMesesVisita);
	    		
	    		// Monta variaveis
	    		foreach($arMesesVisita as $mes){
	    		 
	    			// Conta colunas e inicia contagem meses
	    			${$mes} = 0;
	    			++$colunaGrafico;
	    			
	    			// Dos meses
    				$grMeses .= "'".substr($mes,4,2).'/'.substr($mes,0,4)."'"; 
	    			
	    			// Das barras
	    			$grBarrasPriPostagens .= $arVisita[FORM_PRIMEIRA_VISITA][$mes] ? $arVisita[FORM_PRIMEIRA_VISITA][$mes] : 0;
	    			$grBarrasSupPostagens .= $arVisita[FORM_SUPERVISAO_PRATICA][$mes] ? $arVisita[FORM_SUPERVISAO_PRATICA][$mes] : 0;
	    			
	    			$grBarrasPriPostagensDsei .= $arVisita[FORM_PRIMEIRA_VISITA_DSEI][$mes] ? $arVisita[FORM_PRIMEIRA_VISITA_DSEI][$mes] : 0;
	    			$grBarrasSupPostagensDsei .= $arVisita[FORM_SUPERVISAO_PRATICA_DSEI][$mes] ? $arVisita[FORM_SUPERVISAO_PRATICA_DSEI][$mes] : 0;
	    			
	    			// Coloca virgulas 
	    			if($totalColunasGrafico!=$colunaGrafico){
	    				$grMeses .= ',';
	    				$grBarrasPriPostagens .= ',';
	    				$grBarrasSupPostagens .= ',';
	    				$grBarrasPriPostagensDsei .= ',';
	    				$grBarrasSupPostagensDsei .= ',';
	    			}
	    			
	    		}
	    		?>
	    		
	    		var meses = [<?php echo $grMeses; ?>];
	    		
	    		var barras = [{
	                name: 'Primeira Visita de Supervisão',
	                data: [<?php echo $grBarrasPriPostagens; ?>]
	            }, {
	                name: 'Supervisão Prática',
	                data: [<?php echo $grBarrasSupPostagens; ?>]
	            }
	            <?php if($mostrardsei == 'on'): ?>
		            ,{
		                name: 'Primeira Visita de Supervisão - DSEI',
		                data: [<?php echo $grBarrasPriPostagensDsei; ?>]
		            }, {
		                name: 'Supervisão Prática - DSEI',
		                data: [<?php echo $grBarrasSupPostagensDsei; ?>]
		            }
	            <?php endif; ?>
	            ];
	            
    	<?php } ?>
    	
	    $(function () {
	        $('#graficoBars').highcharts({
	            chart: 		 { type: 'column' },
	            title: 		 { text: 'Gráfico de Postagens Data de Visita' },
	            subtitle: 	 { text: 'Relação Mês x Quantidade' },
	            xAxis: 		 { categories: meses },
	            yAxis: 		 { min: 0, title: { text: 'Quantidade (mil)' } },
	            tooltip: 	 { enabled: false },
	            plotOptions: { column: { pointPadding: 0.2, borderWidth: 0, dataLabels: { enabled: true } } },
	            series: barras
	        });

	        $('.btnGerarRelatorioErros').click(function(){
				document.location.href = '/maismedicos/maismedicos.php?modulo=relatorio/panoramaPostagens&acao=A&requisicao=gerarxlserro';
		    });
	    });
	    
    </script>
    
	</head>
	<body>
		
		<?php 
		
		$linha1 = 'Relatório Panorama de Postagens';
		$linha2 = '';
		monta_titulo($linha1, $linha2);
		
		if($label){
			echo '<center>';
			echo '<b>FILTRO(S):</b>';
			echo '<table cellspacing="1" cellpadding="3" align="center">';
			echo $label;
			echo '</table>';
			echo '</center>';
		}
		
		if($arEnvio){
				
			$totalMesEnvio35 	 = 0;
			$totalMesEnvio36 	 = 0;
			$totalMesEnvio40 	 = 0;
			$totalMesEnvio43		 = 0;
			$totalMesesEnvio = 0;
			$totalGeralEnvio = 0;
			?>
					
			<center><h3>DATA DE ENVIO</h3></center>			
			<table <?php echo $atTabelaListagem; ?>>
				<tr>
					<th>Relatório</th>
					<?php foreach($arMesesEnvio as $mes): ?>
						<th style="<?php echo $cssColunasQtde; ?>"><?php echo substr($mes,4,2).'/'.substr($mes,0,4); ${$mes} = 0; ?></th>		
					<?php endforeach; ?>
					<th style="<?php echo $cssColunasQtde; ?>">Total Meses</th>
					<th style="<?php echo $cssColunasQtde; ?>">Total Geral</th>
				</tr>	
				<?php foreach($arRelatorios as $k => $v): ?>
					<tr>
						<td><?php echo $v; ?></td>
						<?php foreach($arMesesEnvio as $mes): ?>
							<td>
								<?php 
								$totalMesEnvio{$mes} += $arEnvio[$k][$mes]; 
								$totalRelatorioEnvio{$k} += $arEnvio[$k][$mes]; 
								echo $arEnvio[$k][$mes] ? $arEnvio[$k][$mes] : 0; 
								?>
							</td>
						<?php endforeach; ?>
						<td><center><b><?php echo $totalRelatorioEnvio{$k} ? $totalRelatorioEnvio{$k} : 0; ?></b></center></td>
						<td><center><b><?php echo $arTotal[$k] ? $arTotal[$k] : 0; ?></b></center></td>
					</tr>
					<?php 
					$totalMesesEnvio += $totalRelatorioEnvio{$k};
					$totalGeralEnvio += $arTotal[$k];
					?>
				<?php endforeach; ?>
				<tr>
					<td><b>Total</b></td>
					<?php foreach($arMesesEnvio as $mes): ?>
						<td><b><?php echo $totalMesEnvio{$mes}; ?></b></td>
					<?php endforeach; ?>
					<td><center><b><?php echo $totalMesesEnvio ? $totalMesesEnvio : 0; ?></b></center></td>
					<td><center><b><?php echo $totalGeralEnvio ? $totalGeralEnvio : 0; ?></b></center></td>
				</tr>
			</table>
			
		<?php
	 
		}else{
 
		?>
	
			<center><b><font color='red'>Sem registros!</font></b></center>
			
		<?php
	
		} 
}	
		
?>
	
	<?php
	 
		if($arVisita){
			
			$totalMesVisita35 = 0;
			$totalMesVisita36 = 0;
			$totalMesVisita40 = 0;
			$totalMesVisita43 = 0;
			$totalMesesVisita = 0;
			$totalGeralVisita = 0;	

			if($_REQUEST['requisicao']=='exibirxls'){
				sort($arMesesVisita);
			}else{
				echo '<br/><br/>
					  <center><h3>DATA DE VISITA</h3></center>';
			}
			
			?>
			<table <?php echo $atTabelaListagem; ?>>
				<tr>
					<th>Relatório</th>
					<?php foreach($arMesesVisita as $mes): ?>
						<th><?php echo $mes == '999999' ? '(Vazio)' : substr($mes,4,2).'/'.substr($mes,0,4); ${$mes} = 0; ?></th>
					<?php endforeach; ?>
					<th>Total Meses</th>
					<th>Total Geral</th>
				</tr>	
				<?php foreach($arRelatorios as $k => $v): ?>
					<tr>
						<td><?php echo $v; ?></td>
						<?php foreach($arMesesVisita as $mes): ?>
							<td>
								<?php 
								$totalMesVisita{$mes} += $arVisita[$k][$mes]; 
								$totalRelatorioVisita{$k} += $arVisita[$k][$mes]; 
								echo $arVisita[$k][$mes] ? $arVisita[$k][$mes] : 0; 
								?>
							</td>
						<?php endforeach; ?>
						<td><center><b><?php echo $totalRelatorioVisita{$k} ? $totalRelatorioVisita{$k} : 0; ?></b></center></td>
						<td><center><b><?php echo $arTotal[$k] ? $arTotal[$k] : 0; ?></b></center></td>
					</tr>
					<?php 
					$totalMesesVisita += $totalRelatorioVisita{$k};
					$totalGeralVisita += $arTotal[$k];
					?>
				<?php endforeach; ?>							
				<tr>
					<td><b>Total</b></td>
					<?php foreach($arMesesVisita as $mes): ?>
						<td><b><?php echo $totalMesVisita{$mes}; ?></b></td>
					<?php endforeach; ?>
					<td><center><b><?php echo $totalMesesVisita ? $totalMesesVisita : 0; ?></b></center></td>
					<td><center><b><?php echo $totalGeralVisita ? $totalGeralVisita : 0; ?></b></center></td>
				</tr>
			</table>
			<?php if(in_array(PERFIL_ADMINISTRADOR, $arPerfil) || $db->testa_superuser()): ?>
				<p>&nbsp;</p>
				<center><input type="button" value="Gerar Relatório de Erros" class="btnGerarRelatorioErros" /></center>
			<?php endif; ?>
			<p>&nbsp;</p>
			<div id="graficoBars" style="min-width: 400px; height: 400px; margin: 0 auto"></div>
			
	<?php }else{ ?>
	
		<center><b><font color='red'>Sem registros!</font></b></center>
			
	<?php } ?>
	
<?php if($_REQUEST['requisicao']!='exibirxls') : ?>	
		</body>
	</html>
<?php endif; ?>

<?php 
if($MOSTRA_TEMPO_EXECUCAO){
	// Terminamos o "contador" e exibimos
	list($usec, $sec) = explode(' ', microtime());
	$script_end = (float) $sec + (float) $usec;
	$elapsed_time = round($script_end - $script_start, 5);
	echo '<p>&nbsp;</p><center>Tempo decorrido: ', $elapsed_time, ' secs. Memória usada: ', round(((memory_get_peak_usage(true) / 1024) / 1024), 2), 'Mb</center>';
}
?>