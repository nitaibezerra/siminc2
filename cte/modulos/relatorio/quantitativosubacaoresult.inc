<?php
$agrupadorFiltroColuna 	= $_REQUEST['agrupadorFiltroColuna'];
$agrupadorFiltro 		= $_REQUEST['agrupadorFiltro'];
$estados				= $_REQUEST['estado'];
$municipio				= $_REQUEST['municipios'];
$dimensao				= $_REQUEST['dimensao'];
$area					= $_REQUEST['areas'];
$indicador				= $_REQUEST['indicador'];
$acao					= $_REQUEST['acao'];
$subacao					= $_REQUEST['subacao'];

//extract($_POST);
//if( $indicador[0] && ( $indicador_campo_flag || $indicador_campo_flag == '1' )){
// Colunas
$select = '';
$selectxls = array ();
$selectInterno = '';
$groupbyIn = '';
$groupbyEx = '';
$innerInterno = '';

if(is_array($agrupadorFiltroColuna)){
	foreach($agrupadorFiltroColuna as $colunas){
		if($colunas == 'qtd2007'){
			$select .= 'sum(qtd2007) as qtd2007,';
			$selectxls[3] = ' sum(qtd2007) as Qtd2007';
			$selectInterno .= "CASE WHEN csa.cosano = '2007' THEN  sum(csa.cosqtd) END AS qtd2007,";
			$selectInternoGlobal .= "CASE WHEN csa.sptano = '2007' THEN  sum(csa.sptunt) END AS qtd2007,";
			if( strpos($groupbyIn, 'csa.cosano, csa.cosqtd,') === false ){
				$groupbyIn .= 'csa.cosano, csa.cosqtd,';
			}
			if( strpos($groupbyInGlobal, "csa.sptano, csa.sptunt,") === false ){
				$groupbyInGlobal .= 'csa.sptano, csa.sptunt,';
			}
		}
		if($colunas == 'qtd2008'){
			$select .= 'sum(qtd2008) as qtd2008,';
			$selectxls[4] = ' sum(qtd2008) as Qtd2008';
			$selectInterno .= "CASE WHEN csa.cosano = '2008' THEN  sum(csa.cosqtd) END AS qtd2008,";
			$selectInternoGlobal .= "CASE WHEN csa.sptano = '2008' THEN  sum(csa.sptunt) END AS qtd2008,";
			if( strpos($groupbyIn, 'csa.cosano, csa.cosqtd,') === false ){
				$groupbyIn .= 'csa.cosano, csa.cosqtd,';
			}
			if( strpos($groupbyInGlobal, "csa.sptano, csa.sptunt,") === false ){
				$groupbyInGlobal .= 'csa.sptano, csa.sptunt,';
			}
		}
		if($colunas == 'qtd2009'){
			$select .= 'sum(qtd2009) as qtd2009,';
			$selectxls[5] = ' sum(qtd2009) as Qtd2009';
			$selectInterno .= "CASE WHEN csa.cosano = '2009' THEN  sum(csa.cosqtd) END AS qtd2009,";
			$selectInternoGlobal .= "CASE WHEN csa.sptano = '2009' THEN  sum(csa.sptunt) END AS qtd2009,";
			if( strpos($groupbyIn, 'csa.cosano, csa.cosqtd,') === false ){
				$groupbyIn .= 'csa.cosano, csa.cosqtd,';
			}
			if( strpos($groupbyInGlobal, "csa.sptano, csa.sptunt,") === false ){
				$groupbyInGlobal .= 'csa.sptano, csa.sptunt,';
			}
		}
		if($colunas == 'qtd2010'){
			$select .= 'sum(qtd2010) as qtd2010,';
			$selectxls[6] = ' sum(qtd2010) as Qtd2010';
			$selectInterno .= "CASE WHEN csa.cosano = '2010' THEN  sum(csa.cosqtd) END AS qtd2010,";
			$selectInternoGlobal .= "CASE WHEN csa.sptano = '2010' THEN  sum(csa.sptunt) END AS qtd2010,";
			if( strpos($groupbyIn, 'csa.cosano, csa.cosqtd,') === false ){
				$groupbyIn .= 'csa.cosano, csa.cosqtd,';
			}
			if( strpos($groupbyInGlobal, "csa.sptano, csa.sptunt,") === false ){
				$groupbyInGlobal .= 'csa.sptano, csa.sptunt,';
			}
		}
		if($colunas == 'qtd2011'){
			$select .= 'sum(qtd2011) as qtd2011,';
			$selectxls[7] = ' sum(qtd2011) as Qtd2011';
			$selectInterno .= "CASE WHEN csa.cosano = '2011' THEN  sum(csa.cosqtd) END AS qtd2011,";
			$selectInternoGlobal .= "CASE WHEN csa.sptano = '2011' THEN  sum(csa.sptunt) END AS qtd2011,";
			if( strpos($groupbyIn, 'csa.cosano, csa.cosqtd,') === false ){
				$groupbyIn .= 'csa.cosano, csa.cosqtd,';
			}
			if( strpos($groupbyInGlobal, "csa.sptano, csa.sptunt,") === false ){
				$groupbyInGlobal .= 'csa.sptano, csa.sptunt,';
			}
		}
		if($colunas == 'qtdtotaldosanos'){
			$select .= 'sum(qtdglobaltodosAnos) as qtdtotaldosanos,';
			$selectxls[8] = ' sum(qtdglobaltodosAnos) as Total';
			$selectInterno .= "sum(cosqtd) AS qtdglobaltodosAnos,";
			$selectInternoGlobal .= "sum(csa.sptunt) AS qtdglobaltodosAnos,";
			//$groupbyIn .= 'csa.cosano, csa.cosqtd';
		}

	}
}

// Agrupador

if(is_array($agrupadorFiltro)){
	foreach($agrupadorFiltro as $colunas){
		if($colunas == 'estuf'){
			$select .= 'estuf,';
			$selectxls[0] = ' estuf as Estado';
			$selectInterno .= 'iu.estuf,';
			$selectInternoGlobal .= 'iu.estuf,';
			$groupbyIn .= 'iu.estuf,';
			$groupbyInGlobal .= 'iu.estuf,';
			$groupbyEx .= 'estuf,';
		}
		if($colunas == 'muncod'){
			$select .= 'muncod, mundescricao,';
			$selectxls[1] = ' mundescricao as Municipio';
			$selectInterno .= 'iu.muncod, m.mundescricao,';
			$selectInternoGlobal .= 'iu.muncod, m.mundescricao,';
			$groupbyIn .= 'iu.muncod, m.mundescricao,';
			$groupbyInGlobal .= 'iu.muncod, m.mundescricao,';
			$groupbyEx .= 'muncod, mundescricao,';
			$innerInterno .= 'INNER JOIN territorios.municipio m ON m.muncod = iu.muncod';
		}
		if($colunas == 'sbaid'){
			$select .= 'sbaid, descricaosub,';
			$selectxls[2] = ' descricaosub as Subacao';
			$selectInterno .= "s.sbaid AS sbaid, s.sbadsc AS descricaosub,";
			$selectInternoGlobal .= "s.sbaid AS sbaid, s.sbadsc AS descricaosub,";
			$groupbyIn .= 's.sbaid, sbadsc,';
			$groupbyInGlobal .= 's.sbaid, sbadsc,';
			$groupbyEx .= 'sbaid, descricaosub, ';
		}
	}
}

// Condições
$condicaoInterno = array();
$agrupadorMunicipio = false;
//Estado
if(is_array($estados) && $estados[0]){
	foreach($agrupadorFiltro as $colunas){
		if($colunas == 'muncod'){
			$agrupadorMunicipio = true;
		}
	}
	if($agrupadorMunicipio == true){ // muncipios no agurpador mais filtro de estados
		array_push( $condicaoInterno, " iu.mun_estuf in  ( '" . implode( "','", $estados ) . "' ) " );
	}else{
		array_push( $condicaoInterno, " iu.estuf in  ( '" . implode( "','", $estados ) . "' ) " );
	}
}
//Município
if(is_array($municipio) && $municipio[0]){
	array_push( $condicaoInterno, " iu.muncod in  ( '" . implode( "','", $municipio ) . "' ) " );
}

//Dimensão
if(is_array($dimensao) && $dimensao[0]){
	array_push( $condicaoInterno, " d.dimcod in  ( '" . implode( "','", $dimensao ) . "' ) " );
}

//Área

if(is_array($area) && $area[0]){
	array_push( $condicaoInterno, " ad.ardcod in  ( '" . implode( "','", $area ) . "' ) " );
}

//Indicador
if(is_array($indicador) && $indicador[0]){
	array_push( $condicaoInterno, " i.indcod in  ( '" . implode( "','", $indicador ) . "' ) " );
}

// subacao
if(is_array($subacao) && $subacao[0]){
	array_push( $condicaoInterno, " s.ppsid in  ( '" . implode( "','", $subacao ) . "' ) " );
}

$condicaoInterno = count( $condicaoInterno ) ?  implode( " and ", $condicaoInterno ) : "";

$sql = "SELECT
			".$select."
			dimcod,
			ardcod,
			indcod
		FROM (
		
			SELECT	
				".$selectInternoGlobal."
				d.dimcod,
				ad.ardcod,
				i.indcod
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid and ardstatus = 'A'
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid and indstatus = 'A'
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid and ptostatus = 'A'
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	-- and s.sbasituacaoarvore = 1
				LEFT JOIN cte.subacaoparecertecnico csa ON csa.sbaid = s.sbaid AND sptano in( 2007,2008,2009,2010,2011)
				".$innerInterno."
			WHERE 	
				".$condicaoInterno."
				 AND s.sbaporescola = false -- Subações Globais.
				 and dimstatus = 'A'
				-- and s.sbaid = 728
			GROUP BY
				".$groupbyInGlobal."
				d.dimcod,
				ad.ardcod,
				i.indcod
				
		UNION ALL
				SELECT	
				".$selectInterno."
				d.dimcod,
				ad.ardcod,
				i.indcod
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid and ardstatus = 'A'
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid and indstatus = 'A'
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid and ptostatus = 'A'
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	-- and s.sbasituacaoarvore = 1
				LEFT JOIN cte.composicaosubacao csa ON csa.sbaid = s.sbaid AND cosano in( 2007,2008,2009,2010,2011)
				LEFT JOIN (SELECT cosid, SUM(ecsqtd) as qtdescola FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON csa.cosid = ecs.cosid
				".$innerInterno."
			WHERE 	
				".$condicaoInterno."
				 AND s.sbaporescola = TRUE -- Subações por escola.
				 and dimstatus = 'A'
				 --and s.sbaid = 728
			GROUP BY
				".$groupbyIn."
				d.dimcod,
				ad.ardcod,
				i.indcod
		) AS resultado
		GROUP BY 
			".$groupbyEx."
			dimcod,
			ardcod,
			indcod
		";


if(isset( $_REQUEST['xls'])){
	ksort($selectxls,SORT_NUMERIC);
	
$selectxlsext = implode( ",", $selectxls );
	
		$sqlQddXls = "SELECT
			".$selectxlsext."
		FROM (
		
			SELECT	
				".$selectInternoGlobal."
				d.dimcod,
				ad.ardcod,
				i.indcod
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid and ardstatus = 'A'
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid and indstatus = 'A'
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid and ptostatus = 'A'
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	-- and s.sbasituacaoarvore = 1
				LEFT JOIN cte.subacaoparecertecnico csa ON csa.sbaid = s.sbaid AND sptano in( 2007,2008,2009,2010,2011)
				".$innerInterno."
			WHERE 	
				".$condicaoInterno."
				 AND s.sbaporescola = false -- Subações Globais.
				 and dimstatus = 'A'
				-- and s.sbaid = 728
			GROUP BY
				".$groupbyInGlobal."
				d.dimcod,
				ad.ardcod,
				i.indcod
				
		UNION ALL
				SELECT	
				".$selectInterno."
				d.dimcod,
				ad.ardcod,
				i.indcod
			FROM cte.dimensao d
				INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid and ardstatus = 'A'
				INNER JOIN cte.indicador i ON i.ardid = ad.ardid and indstatus = 'A'
				INNER JOIN cte.criterio c ON c.indid = i.indid
				INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid and ptostatus = 'A'
				INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
				INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	-- and s.sbasituacaoarvore = 1
				LEFT JOIN cte.composicaosubacao csa ON csa.sbaid = s.sbaid AND cosano in( 2007,2008,2009,2010,2011)
				LEFT JOIN (SELECT cosid, SUM(ecsqtd) as qtdescola FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON csa.cosid = ecs.cosid
				".$innerInterno."
			WHERE 	
				".$condicaoInterno."
				 AND s.sbaporescola = TRUE -- Subações por escola.
				 and dimstatus = 'A'
				 --and s.sbaid = 728
			GROUP BY
				".$groupbyIn."
				d.dimcod,
				ad.ardcod,
				i.indcod
		) AS resultado
		GROUP BY 
			".$groupbyEx."
			dimcod,
			ardcod,
			indcod
		";
	$db->sql_to_excel( $sqlQddXls, "RelatorioQauntitativosSubacao" );
	die();
}
$dados = $db->carregar( $sql );

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}


include APPRAIZ. 'includes/classes/relatorio.class.inc';

$_SESSION['par']['postrelat'] = $_POST;


$dados 		= $db->carregar($sql);
$a = array();


$agrup 		= monta_agp();
$col   		= monta_coluna();
$r 			= new montaRelatorio();
$r->setAgrupador($agrup, $dados, $agrupcol); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(true);
$r->setTotalizador(true);
$r->setTolizadorLinha(false);





function monta_agp( $exibe = null ){
	$agrupador = $_POST['agrupadorFiltro'];

	if( !is_array($agrupador) ){
		$agrupador = array();
	}

		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(	"estuf",
												"muncod",
												"dimcod",  
												"ardcod",  
												"indcod",  
												"sbaid",
												"qtd2007",
												"qtd2008",
												"qtd2009",
												"qtd2010",
												"qtd2011",
												"qtdtotaldosanos"
											  )	  
					);
					
		
		$count = 1;
			$i = 0;
			
			if( $i > 1 || $i == 0 ){
				$vari = "";	
			}
			
		$novoAgrup = array(); 

		if( !is_array($agrupador) ){
			$agrupador = array();
		}
		foreach ($agrupador as $val){
			array_push($novoAgrup, $val);
		}
	
		foreach ($novoAgrup as $val):
			if($count == 1){
				$var = $vari;
			} else {
				$var = "";		
			}
												
			switch ($val) {
				case 'estuf':
					array_push($agp['agrupador'], array(
														"campo" => "estuf",
												  		"label" => "Estado")										
										   				);				
			   		continue;
			    break;
				case 'muncod':
					array_push($agp['agrupador'], array(
														"campo" => "mundescricao",
												  		"label" => "Município")										
										   				);				
			   		continue;
			    break;
				case 'sbaid':
					array_push($agp['agrupador'], array(
														"campo" => "descricaosub",
												  		"label" => "Subação")										
										   				);				
			   		continue;
			    break;
				case 'dimcod':
					array_push($agp['agrupador'], array(
														"campo" => "dimcod",
												  		"label" => "Dimensão")										
										   				);				
			   		continue;
			    break;
			    case 'ardcod':
					array_push($agp['agrupador'], array(
														"campo" => "ardcod",
												  		"label" => "Área")										
										   				);				
			   		continue;
			    break;
			    case 'indcod':
					array_push($agp['agrupador'], array(
														"campo" => "indcod",
												  		"label" => "Indicador")										
										   				);				
			   		continue;
			    break;
			}
			$count++;
		endforeach;


	return $agp;
}

function monta_coluna( $exibe = null ){
	$agrupador = $_POST['agrupadorFiltroColuna'];
	if( !is_array($agrupador) ){
		$agrupador = array();
	}

	$coluna = array();

		foreach ($agrupador as $val){
			switch ($val) {
				case 'estuf':
					array_push($coluna, array(
							  "campo" => "estado",
					   		  "label" => "Estado",
							  "type"  => "text"
//							  "mostraNivel" => "numeroconvenio")
							 // "html"  => "<b>{valor}</b>"
									)	);				
			   		continue;
			    break;
			     case 'muncod':
					array_push($coluna, array(
							  "campo" => "muncod",
					   		  "label" => "município",
							  "type"  => "text"
//							  "mostraNivel" => "numeroconvenio")
							 // "html"  => "<b>{valor}</b>"
									)	);				
			   		continue;
			    break;
			    case 'sbaid':
					array_push($coluna, array(
							  "campo" => "descricaosub",
					   		  "label" => "Subação",
							  "type"  => "numeric"
//							  "mostraNivel" => "numeroconvenio")
							 // "html"  => "<b>{valor}</b>"
									)	);				
			   		continue;
			    break;
				case 'qtdtotaldosanos':
					array_push($coluna, array(
							  "campo" => "qtdtotaldosanos",
					   		  "label" => "Quantidade",
							  "type"  => "<b>{qtdtotaldosanos}</b>"
								)
										);				
			   		continue;
			    break;
			    case 'qtd2007':
					array_push($coluna, array(
							  "campo" => "qtd2007",
					   		  "label" => "QTD 2007",
							  "type"  => "<b>{qtd2007}</b>"
								)
										);				
			   		continue;
			    break;
			    case 'qtd2008':
					array_push($coluna, array(
							  "campo" => "qtd2008",
					   		  "label" => "QTD 2008",
							  "type"  => "<b>{qtd2008}</b>"
								)
										);				
			   		continue;
			    break;
			    case 'qtd2009':
					array_push($coluna, array(
							  "campo" => "qtd2009",
					   		  "label" => "QTD 2009",
							  "type"  => "<b>{qtd2009}</b>"
								)
										);				
			   		continue;
			    break;
			    case 'qtd2010':
					array_push($coluna, array(
							  "campo" => "qtd2010",
					   		  "label" => "QTD 2010",
							  "type"  => "<b>{qtd2010}</b>"
								)
										);				
			   		continue;
			    break;
			    case 'qtd2011':
					array_push($coluna, array(
							  "campo" => "qtd2011",
					   		  "label" => "QTD 2011",
							  "type"  => "<b>{qtd2011}</b>"
								)
										);				
			   		continue;
			    break;
			    case 'dimcod':
					array_push($coluna, array(
							  "campo" => "dimcod",
					   		  "label" => "Dimensão",
							  "type"  => "numeric"
								)
										);				
			   		continue;
			    break;
			    case 'ardcod':
					array_push($coluna, array(
							  "campo" => "ardcod",
					   		  "label" => "Área",
							  "type"  => "numeric"
								)
										);				
			   		continue;
			    break;
			    case 'indcod':
					array_push($coluna, array(
							  "campo" => "indcod",
					   		  "label" => "Indicador",
							  "type"  => "numeric"
								)
										);				
			   		continue;
			    break;
			}
		}
	return $coluna;	
}

?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<!-- Includes do SuperTitleAjax -->
		<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
		<script type="text/javascript" src="../includes/remedial.js"></script>
		<script type="text/javascript" src="../includes/superTitle.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script>

			function abreRelatorio( cod, rel ){
			
				var indNivel = 0;
				
				d = cod.retornaElemento();
				abrirTodos(d[indNivel],'', cod, rel);
			} 
		
			function abrirTodos(elemento, idPai, cod, rel){
				if( elemento['elemento'].length == 0 ) return;
				var id = elemento['id'];

				window.setTimeout("d" + rel + ".controle('" + id + "', '" + (idPai ? idPai : id) + "', '" + id + "_img')", 2);
				idPai = (idPai ? idPai + ":" + id : id);

				for( var i = 0; i < elemento['elemento'].length; i++ ){
					abrirTodos(elemento['elemento'][i], idPai, cod, rel);
				}
				
			}
		</script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
	
	<?
		echo $r->getRelatorio();
	?>