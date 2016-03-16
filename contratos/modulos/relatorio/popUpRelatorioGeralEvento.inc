<?php
ini_set( "memory_limit", "512M" );

set_time_limit(0);

montaBotaoForm();
include APPRAIZ. 'includes/classes/relatorio.class.inc';

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	 <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if($_POST['limpaSession']){
	unset($_SESSION['emenda']['post']);
	$_POST['limpaSession'] = "";
}

if( empty($_SESSION['emenda']['post']) ){
	$_SESSION['emenda']['post'] = $_POST;
}

$col = monta_coluna();

$coluna = array();
$cabecalho = array();
$id = array();

foreach ($col as $c) {
	if( $c['campo'] ){
		$coluna[] = $c['campo'];
	}
	$cabecalho[] = $c['label'];
	$id[] = $c['id'];
}

//print_r($id);

$arSql   = monta_sql($coluna, $id);

//ver( $arSql,d );

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');

	$db->sql_to_excel($arSql, 'relGeralEvento', $cabecalho);
	exit;
	$exporta = "false";
}

$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				"</td>" .
			  "</tr>
			  </table>";

echo $cabecalhoBrasao;

$db->monta_lista_array( $arSql, $cabecalho, 100000, 1, 'N', 'center', '');

montaBotaoForm();

echo '<script>
		$(\'loader-container\').hide();
	  </script>';

function montaBotaoForm(){
	echo '	<html>
			<head>
				<style>
				
				#loader-container,
				#LOADER-CONTAINER{
				    background: transparent;
				    position: absolute;
				    width: 100%;
				    text-align: center;
				    z-index: 8000;
				    height: 100%;
				}
				
				
				#loader {
				    background-color: #fff;
				    color: #000033;
				    width: 300px;
				    border: 2px solid #cccccc;
				    font-size: 12px;
				    padding: 25px;
				    font-weight: bold;
				    margin: 150px auto;
				}
				</style>
				</head>
			<script type="text/javascript" src="/includes/prototype.js"></script>
			<body>
			<style type="">
				@media print {.notprint { display: none } .div_rolagem{display: none} }	
				@media screen {.notscreen { display: none; }
				
				.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
				
			</style>
			<div id="loader-container" style="display: none">
		   		<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
			</div>
			<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
					</td>
				</tr>
			</table>
			</form>
			</body>
			<script type="text/javascript">				
				function exportarExcel(){
					document.getElementById(\'exporta\').value = "true";
					document.getElementById(\'formulario\').submit();
				}
				$(\'loader-container\').show();
			</script>
			</html>';
}

function monta_sql($cols, $id){
	global $db;
	$filtro = array();
	$carregaCPFresp = false;
	
	extract($_SESSION['emenda']['post']);
		
	$eveid 					 = !is_array($eveid) ? explode(',', $eveid) : $eveid;
	$ungcod					 = !is_array($ungcod) ? explode(',', $ungcod) : $ungcod;
	$rcoid					 = !is_array($rcoid) ? explode(',', $rcoid) : $rcoid;
	$tpeid					 = !is_array($tpeid) ? explode(',', $tpeid) : $tpeid;	
	$evequantidadedias 		 = !is_array($evequantidadedias) ? explode(',', $evequantidadedias) : $evequantidadedias;
	$evepublicoestimado 	 = !is_array($evepublicoestimado) ? explode(',', $evepublicoestimado) : $evepublicoestimado;
	$estuf 					 = !is_array($estuf) ? explode(',', $estuf) : $estuf;
	$muncod					 = !is_array($muncod) ? explode(',', $muncod) : $muncod;
	$evecustoprevisto 		 = !is_array($evecustoprevisto) ? explode(',', $evecustoprevisto) : $evecustoprevisto;
	$eveanopi 				 = !is_array($eveanopi) ? explode(',', $eveanopi) : $eveanopi;
	$plicod 				 = !is_array($plicod) ? explode(',', $plicod) : $plicod;
	$evenumeroprocesso		 = !is_array($evenumeroprocesso) ? explode(',', $evenumeroprocesso) : $evenumeroprocesso;
	
	if( $eveid[0] && ( $eveid_campo_flag || $eveid_campo_flag == '1' )){
		$where[] = " ev.eveid " . (( $eveid_campo_excludente == null || $eveid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $eveid ) . "') ";		
	}
	if( $ungcod[0] && ( $ungcod_campo_flag || $ungcod_campo_flag == '1' )){
		$where[] = " u.ungcod " . (( $ungcod_campo_excludente == null || $ungcod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ungcod ) . "') ";		
	}
	if( $rcoid[0] && ( $rcoid_campo_flag || $rcoid_campo_flag == '1' )){
		$where[] = " rc.rcoid " . (( $rcoid_campo_excludente == null || $rcoid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $rcoid ) . "') ";		
	}
	if( $tpeid[0] && ( $tpeid_campo_flag || $tpeid_campo_flag == '1' )){
		$where[] = " te.tpeid " . (( $tpeid_campo_excludente == null || $tpeid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpeid ) . "') ";		
	}
	if( $evequantidadedias[0] && ( $evequantidadedias_campo_flag || $evequantidadedias_campo_flag == '1' )){
		$where[] = " ev.evequantidadedias " . (( $evequantidadedias_campo_excludente == null || $evequantidadedias_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $evequantidadedias ) . "') ";		
	}
	if( $evepublicoestimado[0] && ( $evepublicoestimado_campo_flag || $evepublicoestimado_campo_flag == '1' )){
		$where[] = " ev.evepublicoestimado " . (( $tevepublicoestimado_campo_excludente == null || $evepublicoestimado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $evepublicoestimado ) . "') ";		
	}
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
		$where[] = " ev.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	if( $muncod[0] && ( $muncod_campo_flag || $muncod_campo_flag == '1' )){
		$where[] = " tm.muncod " . (( $muncod_campo_excludente == null || $muncod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') ";		
	}
	if( $evecustoprevisto[0] && ( $evecustoprevisto_campo_flag || $evecustoprevisto_campo_flag == '1' )){
		$where[] = " ev.evecustoprevisto " . (( $evecustoprevisto_campo_excludente == null || $evecustoprevisto_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $evecustoprevisto ) . "') ";		
	}
	if( $eveanopi[0] && ( $eveanopi_campo_flag || $eveanopi_campo_flag == '1' )){
		$where[] = " ev.eveanopi " . (( $eveanopi_campo_excludente == null || $eveanopi_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $eveanopi ) . "') ";		
	}
	if( $plicod[0] && ( $plicod_campo_flag || $plicod_campo_flag == '1' )){
		$where[] = " mp.plicod " . (( $plicod_campo_excludente == null || $plicod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $plicod ) . "') ";		
	}	
	if( $evenumeroprocesso[0] && ( $evenumeroprocesso_campo_flag || $evenumeroprocesso_campo_flag == '1' )){
		$where[] = " ev.evenumeroprocesso " . (( $evenumeroprocesso_campo_excludente == null || $evenumeroprocesso_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $evenumeroprocesso ) . "') ";		
	}		
	
	
	$order = array();
	if($agrupador){
		foreach ($agrupador as $val){
			switch ($val) {
				case 'ungcod':
					$order[] = 'u.ungcod';
				break;
				
				case 'eveid':
					$order[] = 'ev.eveid';
				break;

				case 'rcoid':
					$order[] = 'rc.rcoid';
				break;

				case 'tpeid':
					$order[] = 'te.tpeid';
				break;
					
				case 'evequantidadedias':
					$order[] = 'ev.evequantidadedias';
				break;
				
				case 'evepublicoestimado':
					$order[] = 'ev.evepublicoestimado';
				break;
				
				case 'estuf':
					$order[] = 'ev.estuf';
				break;
				
				case 'muncod':
					$order[] = 'tm.muncod';
				break;
				
				case 'evecustoprevisto':
					$order[] = 'ev.evecustoprevisto';
				break;
				
				case 'eveanopi':
					$order[] = 'ev.eveanopi';
				break;
				
				case 'plicod':
					$order[] = 'mp.plicod';
				break;

				case 'evenumeroprocesso':
					$order[] = 'ev.evenumeroprocesso';
				break;				
			}
		}
	}

	
	#add a coluna para fazer a ordenação, caso na exista
	foreach ($order as $v) {
		if( !in_array( $v, $cols ) ){
			$cols[] = $v; 
		}	
	}
	
	$sql = "SELECT DISTINCT
				".( $cols ? implode(',', $cols) : "*" )."
			
			FROM evento.evento AS ev
			INNER JOIN public.unidadegestora AS u ON ev.ungcod = u.ungcod
			INNER JOIN evento.reuniaocomite AS rc ON rc.rcoid = ev.rcoid
			INNER JOIN evento.tipoevento AS te ON te.tpeid = ev.tpeid
			LEFT JOIN monitora.pi_planointerno AS mp ON mp.ungcod = u.ungcod
			INNER JOIN territorios.municipio AS tm ON tm.muncod = ev.muncod

			".(($where)?" WHERE ".implode(" AND ", $where):"")."
			".($order ? "ORDER BY " . implode(',', $order) : '');

	$arDados = $db->carregar( $sql );
	if( $arDados ){
		$registro = array();
		$arDadosArray = array();
		foreach ($arDados as $key => $value) {

	

			$xx += isset($xx) ? 1 : 0;
			foreach ($id as $key => $c) {
				$registro[$xx][$c] = $value[$c];
			}

		}
		
	}
	return $registro;
}

function monta_coluna(){
	
	$cols = $_SESSION['emenda']['post']['coluna'] ? $_SESSION['emenda']['post']['coluna'] : array();

	$coluna = array("coluna" => array() );
	
	if( is_array($cols) ){
		for($i=0; $i<sizeof($cols); $i++){
			
//		}	
//		foreach ($cols as $val) {
			switch ($cols[$i]) {

				case 'eveid':
					array_push($coluna['coluna'], array(
														"campo" => "ev.evetitulo",
														"id" => "evetitulo",
												  		"label" => "Título do Evento")										
										   				);
				break;
				
				case 'ungcod':
					array_push($coluna['coluna'], array(
														"campo" => "u.ungdsc",
														"id" => "ungdsc",
												  		"label" => "Unidade Gestora")										
										   				);
				break;

				case 'rcoid':
					array_push($coluna['coluna'], array(
														"campo" => "rc.rcodescricao",
														"id" => "rcodescricao",
												  		"label" => "Reunião do Comitê")										
										   				);
				break;				

				case 'tpeid':
					array_push($coluna['coluna'], array(
														"campo" => "te.tpedescricao",
														"id" => "tpedescricao",
												  		"label" => "Tipo")										
										   				);
				break;
								
				case 'evequantidadedias':
					array_push($coluna['coluna'], array(
														"campo" => "ev.evequantidadedias",
														"id" => "evequantidadedias",
												  		"label" => "Nº de dias do Evento")										
										   				);
				break;
				
				case 'evepublicoestimado':
					array_push($coluna['coluna'], array(
														"campo" => "ev.evepublicoestimado",
														"id" => "evepublicoestimado",
												  		"label" => "Público")										
										   				);
				break;
				
				case 'estuf':
					array_push($coluna['coluna'], array(
														"campo" => "ev.estuf",
														"id" => "estuf",
												  		"label" => "UF")										
										   				);
				break;
				
				case 'muncod':
					array_push($coluna['coluna'], array(
														"campo" => "tm.mundescricao",
														"id" => "mundescricao",
												  		"label" => "Município")										
										   				);
				break;
				
				case 'evecustoprevisto':
					array_push($coluna['coluna'], array(
														"campo" => "ev.evecustoprevisto",
														"id" => "evecustoprevisto",
												  		"label" => "Custo(R$)")										
										   				);
				break;
				
				case 'eveanopi':
					array_push($coluna['coluna'], array(
														"campo" => "ev.eveanopi",
														"id" => "eveanopi",
												  		"label" => "Ano do PI")										
										   				);
				break;
						
				case 'plicod':
					array_push($coluna['coluna'], array(
														"campo" => "mp.plicod",
														"id" => "plicod",
												  		"label" => "Nº PI")										
										   				);
				break;

				case 'evenumeroprocesso':
					array_push($coluna['coluna'], array(
														"campo" => "ev.evenumeroprocesso",
														"id" => "evenumeroprocesso",
												  		"label" => "Nº Processo")										
										   				);
				break;				
				
				
			}		
		}
	}
	return $coluna['coluna'];			  	
}
?>