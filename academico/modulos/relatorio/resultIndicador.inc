<? 
ini_set("memory_limit","1000M");
set_time_limit(0);

include APPRAIZ. 'includes/classes/relatorio.class.inc';

$dados = preparaDados();
////ver($dados);
$agrup = montaAgp();
$col   = montaColuna();
if( $_POST['req'] == 'geraxls' ){
	$arCabecalho = array();
	foreach ($_POST['agrupador'] as $agrup){
		if($agrup == 'campus'){
			array_push($arCabecalho, 'Campus');
		}
		if($agrup == 'instituicao'){
			array_push($arCabecalho, 'Instituição');
		}
		if($agrup == 'municipio'){
			array_push($arCabecalho, 'Município');
		}
		if($agrup == 'estado'){
			array_push($arCabecalho, 'Estado');
		}
		if($agrup == 'resgiao'){
			array_push($arCabecalho, 'Região');
		}
		if($agrup == 'brasil'){
			array_push($arCabecalho, 'Brasil');
		}
	}
	array_push($arCabecalho, 'Indicadores');
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
	}
	$arDados = Array();
	foreach( $dados as $k => $registro ){
		foreach( $registro as $nome => $linha ){
			$ban = Array('campo','id','itpid','itpformula','itpobservacao');
			if( !in_array($nome,$ban) && substr ( $nome, 0, 13 ) != formula_calc_ ){
				$arDados[$k][$nome] = $linha;
			}
		}
	}
//	ver($dados,d);
//	$arDados = ($dados) ? $dados : array();
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatorioIndicadores".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	die;
}
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
$obRelatorio = new montaRelatorio();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir( true );
ob_flush();
echo $r->getRelatorio();


?>
</body>
<?php


function buscaDadosIndicador(){
	global $db;

	$arWhere = array();
	extract( $_POST );
	
	if( $itpid[0] && ( $itpid_campo_flag || $itpid_campo_flag == '1' )){
		$arWhere[] = " itpid " . (( $itpid_campo_excludente == null || $itpid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $itpid ) . "') ";
	}
	
	$sql = "SELECT 
				itpid, 
				itpdsc, 
				itpformula, 
				itpobservacao
  			FROM 
  				academico.indicadorestcuprofissional
  			WHERE
  				itpstatus = 'A'
  				" . (count($arWhere) ? " AND " . implode(' AND ', $arWhere) : '');
	$arDados = $db->carregar( $sql );
//	ver($arDados);
	return $arDados;
}

function retornaCamposSQL(){
	
	$campos = Array(Array(),Array());
	
	switch( $_POST['agrupador'][count($_POST['agrupador'])-1] ){
		case "brasil":
		break;
		case "regiao":
			array_push($campos[0], '\'r.regcod\' as campo, \'\\\'\' || r.regcod || \'\\\'\' as id' );
			array_push($campos[1], '\'r.regcod\' as campo, \'\\\'\' || r.regcod || \'\\\'\' as id' );
		break;
		case "estado":
			array_push($campos[0], '\'est.estuf\' as campo, \'\\\'\' || est.estuf || \'\\\'\' as id' );
			array_push($campos[1], '\'est.estuf\' as campo, \'\\\'\' || est.estuf || \'\\\'\' as id' );
		break;
		case "municipio":
			array_push($campos[0], '\'m.muncod\' as campo, \'\\\'\' || m.muncod || \'\\\'\' as id' );
			array_push($campos[1], '\'m.muncod\' as campo, \'\\\'\' || m.muncod || \'\\\'\' as id' );
		break;
		case "instituicao":
			array_push($campos[0], '\'e2.entid\' as campo, e2.entid as id' );
			array_push($campos[1], '\'e2.entid\' as campo, e2.entid as id' );
		break;
		case "campus":
			array_push($campos[0], '\'ci.cmpid\'  as campo, c.cmpid  as id' );
			array_push($campos[1], '\'ri.entid\'  as campo, e.entid as id' );
		break;
	}	
	
	if( is_array($_POST['agrupador']) ){			
		foreach ( $_POST['agrupador'] as $val ){
			switch( $val ){
				case "brasil":
					array_push($campos[0], '\'Brasil\' as brasil' );
					array_push($campos[1], '\'Brasil\' as brasil' );
				break;
				case "regiao":
					array_push($campos[0], 'r.regdescricao as regiao' );
					array_push($campos[1], 'r.regdescricao as regiao' );
				break;
				case "estado":
					array_push($campos[0], 'ed.estuf as estado' );
					array_push($campos[1], 'ed.estuf as estado' );
				break;
				case "municipio":
					array_push($campos[0], 'm.mundescricao as municipio' );
					array_push($campos[1], 'm.mundescricao as municipio' );
				break;
				case "instituicao":
					array_push($campos[0], 'e2.entnome as instituicao' );
					array_push($campos[1], 'e2.entnome as instituicao' );
				break;
				case "campus":
					array_push($campos[0], 'e.entnome as campus' );
					array_push($campos[1], 'e.entnome as campus' );
				break;
			}	
		}
	}
	
	return $campos;
}

function buscaDadosCampus(){
	global $db;
	
	$arWhere = array();
	extract($_POST);
	
	if( $regcod[0] && ( $regcod_campo_flag || $regcod_campo_flag == '1' )){
		$arWhere[] = " r.regcod " . (( $regcod_campo_excludente == null || $regcod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $regcod ) . "') ";
	}
	
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
		$arWhere[] = " est.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";
	}
	
	if( $muncod[0] && ( $muncod_campo_flag || $muncod_campo_flag == '1' )){
		$arWhere[] = " m.muncod " . (( $muncod_campo_excludente == null || $muncod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') ";
	}
	
	if( $entidunidade[0] && ( $entidunidade_campo_flag || $entidunidade_campo_flag == '1' )){
		$arWhere[] = " e2.entid " . (( $entidunidade_campo_excludente == null || $entidunidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $entidunidade ) . "') ";
	}
	
	$arWhereCamp = '';
	$arWhereReit = '';
	if( $entidcampus[0] && ( $entidcampus_campo_flag || $entidcampus_campo_flag == '1' )){
		$arWhereCamp = "AND e.entid " . (( $entidcampus_campo_excludente == null || $entidcampus_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $entidcampus ) . "') ";
		if( $entidreitoria[0] == '' ){
			$arWhereReit = " AND e.entid is null";
		}
	}
	
	if( $entidreitoria[0] && ( $entidreitoria_campo_flag || $entidreitoria_campo_flag == '1' )){
		$arWhereReit = "AND e.entid " . (( $entidreitoria_campo_excludente == null || $entidreitoria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $entidreitoria ) . "') ";
		if( !($entidcampus[0]) ){
			$arWhereCamp = " AND e.entid is null";
		}
	}
	
	$camposSQL = retornaCamposSQL();
	
	$sql = "SELECT DISTINCT
				".implode(",",$camposSQL[0])."
			FROM
				entidade.entidade e 
			
			INNER JOIN academico.campus 	   		   c ON c.entid  = e.entid
			JOIN academico.campusitem    	  ci ON ci.cmpid = c.cmpid
			
			JOIN academico.item            	   i ON i.itmid       = ci.itmid 
			INNER JOIN entidade.endereco 			  ed ON ed.entid      = e.entid 
			INNER JOIN entidade.funcaoentidade 	 fen ON fen.entid     = e.entid
												    AND fen.funid = 17
			INNER JOIN entidade.funentassoc 		 fea ON fea.fueid      = fen.fueid 
			INNER JOIN entidade.entidade 			  e2 ON e2.entid       = fea.entid 
			INNER JOIN entidade.funcaoentidade 	fen2 ON fen2.entid     = e2.entid
													AND fen2.funid = 11 
			INNER JOIN territorios.estado 		 est ON est.estuf = ed.estuf
			INNER JOIN territorios.regiao 		   r ON r.regcod  = est.regcod
			INNER JOIN territorios.municipio 		   m ON m.muncod  = ed.muncod 
			WHERE
				1=1 
			" . (count($arWhere) ? " AND ".implode(' AND ', $arWhere) : '')." {$arWhereCamp}
			
			UNION ALL
			
			SELECT DISTINCT
				".implode(",",$camposSQL[1])."
			FROM
				entidade.entidade e 
			
			JOIN academico.reitoriasitem ri ON ri.entid= e.entid 
			
			JOIN academico.item                i ON i.itmid       = ri.itmid
			INNER JOIN entidade.endereco 			  ed ON ed.entid      = e.entid 
			INNER JOIN entidade.funcaoentidade 	 fen ON fen.entid 	  = e.entid
												    AND fen.funid = 75
			INNER JOIN entidade.funentassoc 		 fea ON fea.fueid 	   = fen.fueid 
			INNER JOIN entidade.entidade 			  e2 ON e2.entid 	   = fea.entid 
			INNER JOIN entidade.funcaoentidade 	fen2 ON fen2.entid 	   = e2.entid
													AND fen2.funid = 11 
			INNER JOIN territorios.estado 		 est ON est.estuf = ed.estuf
			INNER JOIN territorios.regiao 		   r ON r.regcod  = est.regcod
			INNER JOIN territorios.municipio 		   m ON m.muncod  = ed.muncod 
			WHERE
				1=1 
			" . (count($arWhere) ? " AND ".implode(' AND ', $arWhere) : '')."{$arWhereReit}";
	
//	dbg($sql, 1);
	$arDados = $db->carregar( $sql );
	
	return $arDados;
}


//function buscaVlrItem($cmpid = false, $retid = false, $itmid, $ano, $tipoRetorno = false){
function buscaVlrItem($nomeCampo = '' , $valorCampo = '' , $itmid, $ano, $tipoRetorno = false){
	global $db;
	
	$sql = "SELECT DISTINCT
				itmdsc 
			FROM 
				academico.item 
			WHERE 
				itmid = {$itmid}";
	
	$item = $db->pegaUm($sql);
	
//	$coluna = !$tipoRetorno ? "sum(foo2.vlr2) as vlr" : "coalesce(foo2.vlr2,0) as vlr";
//	$coluna = !$tipoRetorno ? "sum(foo2.vlr2) as vlr" : "foo2.item || ' - ' || coalesce(foo2.vlr2,0) as vlr";
//	$group  = !$tipoRetorno ? ""                      : "";
	
	if( $nomeCampo != '' ){
		if( $_POST['agrupador'][count($_POST['agrupador'])-1] == 'campus' ){
			if( $nomeCampo == 'ci.cmpid' ){
				$whereA = " AND $nomeCampo = {$valorCampo}";	
				$whereB = " AND 1=0";	
				$where = "";	
			}elseif( $nomeCampo == 'ri.entid' ){
				$whereA = " AND 1=0";	
				$whereB = " AND $nomeCampo = {$valorCampo}";	
				$where = "";	
			}
		}else{
			$whereA = " ";	
			$whereB = " ";	
			$where = " AND {$nomeCampo} = {$valorCampo}";	
		}
	}
	
	$sql = "SELECT
					sum(foo.vlr) as vlr
			FROM 
			(
				(
				SELECT DISTINCT
					coalesce(ci.cpivalor,0) as vlr,
					ci.itmid
				FROM
					academico.item i
				INNER JOIN academico.campusitem      ci ON ci.itmid = i.itmid {$whereA} AND cpiano = '{$ano}'	
			
				INNER JOIN academico.campus 	      c ON c.cmpid    = ci.cmpid 
				INNER JOIN entidade.entidade          e ON e.entid    = c.entid 
				INNER JOIN entidade.endereco 	     ed ON ed.entid   = e.entid 
				INNER JOIN entidade.funcaoentidade  fen ON fen.entid  = e.entid   AND fen.funid = 17
				INNER JOIN entidade.funentassoc     fea ON fea.fueid  = fen.fueid 
				INNER JOIN entidade.entidade 	     e2 ON e2.entid   = fea.entid 
				INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = e2.entid  AND fen2.funid = 11 
				INNER JOIN territorios.estado 	    est ON est.estuf  = ed.estuf
				INNER JOIN territorios.regiao 	      r ON r.regcod   = est.regcod
				INNER JOIN territorios.municipio      m ON m.muncod   = ed.muncod
				WHERE
					i.itmid = {$itmid} {$where}
				)
			UNION ALL
				(
				SELECT DISTINCT
					coalesce(ri.retvalor,0) as vlr,
					ri.itmid
				FROM
					academico.item i
				INNER JOIN academico.reitoriasitem   ri ON ri.itmid = i.itmid {$whereB} AND retano = '{$ano}'
				
				INNER JOIN entidade.entidade          e ON e.entid    = ri.entid 
				INNER JOIN entidade.endereco 	     ed ON ed.entid   = e.entid 
				INNER JOIN entidade.funcaoentidade  fen ON fen.entid  = e.entid   AND fen.funid = 75
				INNER JOIN entidade.funentassoc     fea ON fea.fueid  = fen.fueid 
				INNER JOIN entidade.entidade 	     e2 ON e2.entid   = fea.entid 
				INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = e2.entid  AND fen2.funid = 11 
				INNER JOIN territorios.estado 	    est ON est.estuf  = ed.estuf
				INNER JOIN territorios.regiao 	      r ON r.regcod   = est.regcod
				INNER JOIN territorios.municipio       m ON m.muncod   = ed.muncod
					
				WHERE
					i.itmid = {$itmid} {$where}
				)
			) as foo
			";
	
//	ver($sql);
//	ver($where);
	$vlr = $db->pegaUm($sql);
	$vlr = !$tipoRetorno ? ($vlr != '' ? $vlr : '0') : $item." - ".($vlr != '' ? $vlr : '0');
	
	return $vlr;
}

function preparaDados(){
	$anoPost			= (array) $_POST['ano'];
	$arDadosCampus   	= buscaDadosCampus();
//	ver('Acabou',$arDadosCampus,d);
	$arDadosIndicador   = buscaDadosIndicador();
	$arResultDados 		= array();
	$arCalculoItem 		= array();
//	$arAno 		   		= array(2009, 2010); 
	$arAno 		   		= array(2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012);
	
	$arAno = array_intersect($arAno, $anoPost);
//	ver($arDadosCampus,$arDadosIndicador);
	$i = 0;
	if( is_array($arDadosCampus) ){
		foreach ( $arDadosCampus as $dadosCampus ){
			$arLinhaResultDados = array();
//			ver($arDadosIndicador);
			foreach ( $arDadosIndicador as $dadosIndicador ){
				array_push($arResultDados, array_merge($dadosCampus, $dadosIndicador));
//				$formulaDif = strpos($dadosIndicador['itpformula'], ";") == false ? false : true;
//				$formulaDif = $dadosIndicador['itpid'] == 12 ? true : false;
//				ver($dadosIndicador);
				preg_match_all('({ano.[0-9]}[[][0-9]*[]]|[[][0-9]*[]])', $dadosIndicador['itpformula'], $arResultIndicador);
//				ver($dadosIndicador['itpformula'],$arResultIndicador);
				foreach ( $arAno as $ano ){
					$arResultDados[$i]["calc_$ano"] = str_replace(";", "<br>", $arResultDados[$i]['itpformula']);
					$arCalculoItem 		= array();
					foreach( current($arResultIndicador) as $resultIndicador ){
						preg_match_all('({(ano.[0-9])}|[[]([0-9]*)[]])', $resultIndicador, $arResultLinhaIndicador);
						
						$anoBusca = (int) ($arResultLinhaIndicador[1][0] ? eval(str_replace("ano", $ano, "return (" . $arResultLinhaIndicador[1][0] . ");")) : $ano);//$ano;
						$idItem   = $arResultLinhaIndicador[2][0] ? $arResultLinhaIndicador[2][0] : $arResultLinhaIndicador[2][1];
						
						$indiceCalculo = str_replace("ano", $ano, $resultIndicador);
						if ( !isset($arCalculoItem[$anoBusca][$indiceCalculo]) ){
							if( $dadosCampus['id'] != '' ){
								$formulaDif = in_array( $idItem, Array( 75,76,77,78,79,80) ) ? true : false;
								
								$vlrItem = buscaVlrItem($dadosCampus['campo'], $dadosCampus['id'], $idItem, $anoBusca, $formulaDif);
							}else{
								$formulaDif = in_array( $idItem, Array( 75,76,77,78,79,80) ) ? true : false;
								
								$vlrItem = buscaVlrItem('', '', $idItem, $anoBusca, $formulaDif);
							}
							$arCalculoItem[$anoBusca][$indiceCalculo] = ( !$formulaDif ? (float) $vlrItem : $vlrItem);
						}	
							
						$arResultDados[$i]["formula_calc_$ano"] = str_replace($resultIndicador, $arCalculoItem[$anoBusca][$indiceCalculo], $arResultDados[$i]["calc_$ano"]);
						$arResultDados[$i]["calc_$ano"] 		= str_replace($resultIndicador, $arCalculoItem[$anoBusca][$indiceCalculo], $arResultDados[$i]["calc_$ano"]);
					}
//					ver($arResultDados);
					if( $dadosIndicador['itpid'] != 12 ){
						$arResultDados[$i]["calc_$ano"] = number_format(calcular( $arResultDados[$i]["calc_$ano"] ),2,',','.');
					}else{
						$arResultDados[$i]["calc_$ano"] = calcular( $arResultDados[$i]["calc_$ano"] );
					}
						
				}
				$i++;
			}
		}
	}
	return  $arResultDados;
}

function calcular( $calc ){
	if(ereg('/([\(][^/]*)|/([^/\(\)]*)', $calc, $arFragmentoCalculo)){
		$divVal = eval("return(" . ( !is_bool( $arFragmentoCalculo[1] ) ? $arFragmentoCalculo[1] : $arFragmentoCalculo[2] ) . ");");
		$return = ($divVal > 0 ? eval("return(" . $calc . ");") : 0);
	}else{
		$return = $calc;
	}
//	ver($calc);
	
	return $return;
}

function montaAgp(){
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"formula_calc_2005",	
									   		"formula_calc_2006", 
 									   		"formula_calc_2007",
 									   		"formula_calc_2008",
 									   		"formula_calc_2009",
											"formula_calc_2010",
											"formula_calc_2011",
											"formula_calc_2012",
											"calc_2005",
											"calc_2006",
											"calc_2007",
											"calc_2008",
											"calc_2009",
											"calc_2010",
											"calc_2011",
											"calc_2012",
										  )	  
				);
//	ver($_POST['agrupador'],d);
	if( is_array($_POST['agrupador']) ){			
		foreach ( $_POST['agrupador'] as $val ){
			switch( $val ){
				case "instituicao":
					array_push($agp['agrupador'], array(
														"campo" => "instituicao",
												  		"label" => "Instuição")										
										   				);
				break;
				case "brasil":
					array_push($agp['agrupador'], array(
														"campo" => "brasil",
												  		"label" => "Brasil")										
										   				);
				break;
				case "regiao":
					array_push($agp['agrupador'], array(
														"campo" => "regiao",
												  		"label" => "Região")										
										   				);
				break;
				case "estado":
					array_push($agp['agrupador'], array(
														"campo" => "estado",
												  		"label" => "Estado")										
										   				);
				break;
				case "municipio":
					array_push($agp['agrupador'], array(
														"campo" => "municipio",
												  		"label" => "Município")										
										   				);
				break;
				case "campus":
					array_push($agp['agrupador'], array(
														"campo" => "campus",
												  		"label" => "Campus/Reitoria"));
				break;
			}	
		}
	}
	array_push($agp['agrupador'], array("campo"  => "itpdsc",
									 	"label" => "Indicador")										
						   				);
	
	return $agp;
}

function montaColuna(){
	$arColuna		= array();
	$anoPost	= (array) $_POST['ano'];
	$arAno		= array(2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012);
	$arAno 		= array_intersect($arAno, $anoPost);

	foreach($arAno as $ano){
	
		array_push($arColuna, array(
								  "campo" => "calc_{$ano}",
						   		  "label" => $ano,
								  "type"  => "string",
								  "html"  => "<div style='text-align:right; color: #0066CC;'>{calc_{$ano}}</div>"
									));
	}				
	
	return $arColuna;
}
?>