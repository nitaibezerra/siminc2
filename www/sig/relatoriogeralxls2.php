<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include "_funcoes.php";
$db = new cls_banco();

header("Content-Type: application/vnd.ms-excel");
header("Content-disposition: attachment; filename=RelatorioGeral.xls");
ini_set( "memory_limit", "1024M" );

function recuperaAgrupadores($dadosagrupados){
	//echo "<pre>";
	//print_r($dadosagrupados);
	//exit;
	foreach($dadosagrupados as $key => $valor) {
		$r .= $key." \n";
		if($valor['sub_itens']){
			$r .= recuperaAgrupadores($valor['sub_itens']);
		}
	}
	return $r;			
}

function agrupar( $lista, $agrupadores ) {
	global $db;
	
	$existeProximo = count( $agrupadores ) > 0; 
	if ( $existeProximo == false ) {
		return array();
	}
	$campo = array_shift( $agrupadores );
	$novo = array();
	foreach ( $lista as $item )	{
		$chave = $item[$campo];
		
		if($chave) {
		if ( array_key_exists( $chave, $novo ) == false ){			
			$novo[$chave] = array(
				"tipoensinoid"   => $item["tipoensinoid"],
				$campo."id"   => $item[$campo."id"],
				"agrupador"   => $campo,
				"sub_itens"   => array()
			);
		}
		
		if ( $existeProximo ) {	
			array_push( $novo[$chave]["sub_itens"], $item );
		}
		
		}
	}
	if ( $existeProximo ) {
		foreach ( $novo as $chave => $dados )
		{
			$novo[$chave]["sub_itens"] = agrupar( $novo[$chave]["sub_itens"], $agrupadores );
		}
	}	
	return $novo;
}
	
	
// Analisando o filtro geral
	if($_POST['tpeid'][0]) {
		$filtro['geral'][] = "teo.tpeid IN('".implode("','",$_POST['tpeid'])."')"; 
	}
	if($_POST['estuf'][0]) {
		$filtro['geral'][] = "cam.estuf IN('".implode("','",$_POST['estuf'])."')"; 
	}
	if($_POST['unidades'][0]) {
		foreach($_POST['unidades'] as $unidade) {
			$detalhesunidade = explode("@@",$unidade);
			$filuni[] = "(uni.unicod = '".$detalhesunidade[0]."' AND uni.unitpocod = '".$detalhesunidade[1]."')";
		}
		$filtro['geral'][] = "(". implode(" OR ", $filuni) .")";
	}
	// Analisando o filtro dos dados campus
	if($_POST['itmid'][0]) {
		$filtro['campus'][] = "itm.itmid IN('".implode("','",$_POST['itmid'])."')"; 
	}
	if($_POST['ano'][0]) {
		$filtro['ano'] = $_POST['ano'];
	}
	$sql = "SELECT est.estuf as uf, est.estuf as ufid, teo.tpedsc as tipoensino, teo.tpeid as tipoensinoid, uni.unidsc as unidade, uni.unicod as unidadeid, cam.cmpdsc as campus, cam.cmpid as campusid FROM sig.campus cam 
			LEFT JOIN public.unidade uni ON cam.unicod = uni.unicod AND cam.unitpocod = uni.unitpocod
			LEFT JOIN sig.tipoensinouo teu ON uni.gunid = teu.gunid
			LEFT JOIN sig.tipoensino teo ON teu.tpeid = teo.tpeid
 			LEFT JOIN territorios.estado est ON est.estuf = cam.estuf 
 			".((count($filtro['geral']) > 0)?"WHERE ".implode(" AND ", $filtro['geral']):"");
	$dados = $db->carregar( $sql );
	$dados = $dados ? $dados : array();		
	
	$dadosagrupados = agrupar($dados, $_POST['agrupador']);
//echo "<pre>";
//print_r($dadosagrupados);
//exit;

//montando saida xls
	
global $db,$filtro,$filtrofinal;
	
if($dadosagrupados){
	
foreach($dadosagrupados as $nome => $dadoagrupado) {
		
		echo(" \n");
		echo($nome." \n");
		print_r(recuperaAgrupadores($dadoagrupado["sub_itens"]));
		$cabecalho[] = "Itens";
		
		$filtrofinal[$dadoagrupado['agrupador']] = $dadoagrupado[$dadoagrupado['agrupador'].'id'];
		
		if($dadoagrupado['agrupador'] == 'campus') {	
			unset($paramselects);
			unset($cabecalho);	
			$cabecalho[] = "Itens";
			
			if($filtro['ano']) {
				foreach($filtro['ano'] as $ano) {
					$paramselects[] = "CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM sig.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $dadoagrupado[$dadoagrupado['agrupador'].'id'] ."') is null THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM sig.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $dadoagrupado[$dadoagrupado['agrupador'].'id'] ."') END AS ano_".$ano;
					$cabecalho[] = $ano;		
				}
				$paramselects = implode(",",$paramselects);
		
				$sql = "SELECT itm.itmdsc as descricao,
					   ". $paramselects ."
					   FROM sig.item itm 
					   LEFT JOIN sig.tipoitem tpi ON tpi.tpiid = itm.tpiid 
					   LEFT JOIN sig.tipoensinoitem tei ON tei.itmid = itm.itmid 
					   WHERE tei.tpeid = '". $dadoagrupado['tipoensinoid'] ."' ".((count($filtro['campus']) > 0)?"AND ".implode(" AND ", $filtro['campus']):"")."
					   ORDER BY tei.teiordem";
				
				$dados_itens= $db->carregar($sql);
				foreach ($cabecalho as $dados) {
					echo($dados."\t");
				}
				echo("\n");					
				
				foreach($dados_itens as $itens) {
					foreach ($itens as $chave =>$dados) {
						if($chave != "descricao"){
							echo(number_format($dados,2,',','.')."\t");
						}else echo($dados."\t");						
					}
					echo("\n");
				}
				echo("\n");
					
			} else {
				echo "Não foi selecionado 'ano' para a pesquisa";
			}
			
		} else {			
			
			unset($cabecalho);
			unset($cmpids);
			
			$cabecalho[] = "Itens";
						
			$dadositenssomados = array();
			$filtrofinal[$dadoagrupado['agrupador']] = $dadoagrupado[$dadoagrupado['agrupador'].'id'];
			
			$cmpids = filtrarcampus($filtrofinal);			
			
			foreach($cmpids as $cmpid) {
				if($filtro['ano']) {
					unset($paramselects);
					foreach($filtro['ano'] as $ano) {
						$paramselects[] = "CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM sig.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."') is null THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM sig.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."') END AS ano_".$ano;
					}
					$paramselects = implode(",",$paramselects);
				
					$sql = "SELECT itm.itmid,
						   itm.itmdsc as descricaoitem,
						   ". $paramselects ."
						   FROM sig.item itm 
						   LEFT JOIN sig.tipoitem tpi ON tpi.tpiid = itm.tpiid 
						   LEFT JOIN sig.tipoensinoitem tei ON tei.itmid = itm.itmid 
						   WHERE tei.tpeid = '". $dadoagrupado['tipoensinoid'] ."' ".((count($filtro['campus']) > 0)?"AND ".implode(" AND ", $filtro['campus']):"")."
						   ORDER BY tei.teiordem";
					$dadositens = $db->carregar($sql);
					
					if($dadositens) {
						foreach($dadositens as $din) {
							$dadositenssomados[$din['itmid']]['descricaoitem'] = $din['descricaoitem'];
							foreach($filtro['ano'] as $ano) {
								$dadositenssomados[$din['itmid']]['ano_'.$ano] += $din['ano_'.$ano];
							}
						}
					}
				} else {
					echo "Não foi selecionado 'ano' para a pesquisa";
				}
			}
			
			if(count($dadositenssomados) > 0) {
				//cabeçalho
				echo("Itens \t"); 
				foreach($filtro['ano'] as $ano) {					
					echo($ano." \t"); 
				}
				echo(" \n");				
				//itens
				foreach($dadositenssomados as $d) {
					echo($d['descricaoitem']." \t" );
					foreach($filtro['ano'] as $ano) {						
						echo(number_format($d['ano_'.$ano],2,',','.')."\t");							
					}
					echo(" \n"); 
				}
				echo(" \n"); 
			}		
		}
	}		
	
}else{
	
	echo("Nehum registro encontrado.");
	
}
	
	
	

?>