<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include "_funcoes.php";
$db = new cls_banco();

header("Content-Type: application/vnd.ms-excel");
header("Content-disposition: attachment; filename=RelatorioGeral.xls");
ini_set( "memory_limit", "1024M" );

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
	
	function exibir($dados, $nivel = 0, $nivelid = 'nivel') {
		global $db,$filtro,$filtrofinal;
		
		echo "<table width='100%' cellpadding='0' cellspacing='0' border='0'>";
		;
		
		for($i = 0;$i < $nivel; $i++) {
			$espacamento .= '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
		}
		
		$i = 0;
		foreach($dados as $nome => $dados) {
			
			
			$formato = (($i%2)?"bgcolor=\"#F7F7F7\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#F7F7F7';\"":"bgcolor=\"\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='';\"");
			echo "<tr ". $formato .">";
			echo "<td>". $espacamento  ." ". $nome ."</td>";
			echo "</tr>";
			$filtrofinal[$dados['agrupador']] = $dados[$dados['agrupador'].'id'];
			if($dados['sub_itens']) {
				echo "<tr id='". $pref ."'><td>";
				exibir($dados['sub_itens'], $nivel+1, $pref);
				echo "</td></tr>";
			} else {
				echo "<tr  id='". $pref ."'><td>";
				
				unset($cabecalho);
				unset($cmpids);
				$dadositenssomados = array();
				$cmpids = filtrarcampus($filtrofinal);
				foreach($cmpids as $cmpid) {
				if($filtro['ano']) {
					unset($paramselects);
					foreach($filtro['ano'] as $ano) {
						$paramselects[] = "CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM sig.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."') is null THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM sig.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."') END AS ano_".$ano;
					}
					$paramselects = implode(",",$paramselects);
				
					$sql = "SELECT itm.itmid,
						   '<strong>'||itm.itmdsc||'</strong>' as descricaoitem,
						   ". $paramselects ."
						   FROM sig.item itm 
						   LEFT JOIN sig.tipoitem tpi ON tpi.tpiid = itm.tpiid 
						   LEFT JOIN sig.tipoensinoitem tei ON tei.itmid = itm.itmid 
						   WHERE tei.tpeid = '". $dados['tipoensinoid'] ."' ".((count($filtro['campus']) > 0)?"AND ".implode(" AND ", $filtro['campus']):"")."
						   ORDER BY tei.teiordem";
					$dadositens = $db->carregar($sql);
					//echo "<pre>";
					//print_r($dadositens);
					if($dadositens) {
						foreach($dadositens as $din) {
							$dadositenssomados[$din['itmid']]['descricaoitem'] = $din['descricaoitem'];
							foreach($filtro['ano'] as $ano) {
								$dadositenssomados[$din['itmid']]['ano_'.$ano] += $din['ano_'.$ano];
							}
						}
					}
				} else {
					echo "Não foi selecionado 'ano' para a pesquisa <br/>";
				}
				}
				if(count($dadositenssomados) > 0) {
				print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
				
				print '<tr>';
				print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">Item</td>';
					foreach($filtro['ano'] as $ano) {
						print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">'.$ano.'</td>';
					}
				print '</tr>';
				
				foreach($dadositenssomados as $d) {
					print '<tr>';
					print '<td>'. $d['descricaoitem'] .'</td>';
					foreach($filtro['ano'] as $ano) {						
						$valor_item = number_format($d['ano_'.$ano],2,',','.');		
						print '<td>'.$valor_item.'</td>';
					}
					print '</tr>';
				}
				print '</table>';
				}
				
				echo "</td></tr>";
			}
		}
		echo("</form>");
		echo "</table>";		
	}
	
	
//	array_push($_REQUEST['agrupador'],'campus');
	
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
	$dadosagrupados = agrupar($dados, $_REQUEST['agrupador']);
	
	$titulo_modulo = "Sistema de Informações Gerenciais";
	monta_titulo( $titulo_modulo,'Relatório Geral');
	
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script>
	function abrearvore(pref) {
		var img = document.getElementById("btn_"+pref);
		if(img.title == 'abrir') {
			img.title = 'fechar';
    		img.src = '/imagens/menos.gif';
	    	var acao = '';
    	} else {
    		img.title = 'abrir';
	    	img.src = '/imagens/mais.gif';
    		var acao = 'none';
	    }
   		document.getElementById(pref).style.display = acao;
	}
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<table class="tabela" align="center" cellspacing="1" cellpadding="3">		
<tr>
<td>
	<?
	exibir($dadosagrupados);
	?>
</td>
</tr>
</table>