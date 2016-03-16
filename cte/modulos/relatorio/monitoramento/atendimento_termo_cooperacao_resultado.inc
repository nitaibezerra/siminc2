<?php
// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = recuperarSql();
$coluna    = recuperarArColunas();
$agrupador = recuperarArAgrupador();
$dados 	   = $db->carregar( $sql );

$rel->setColuna( $coluna );
$rel->setAgrupador( $agrupador, $dados ); 

$rel->setEspandir( true );
$rel->setTotNivel( true );
$rel->setTotalizador( false );
//$rel->setMonstrarTolizadorNivel( TIPO_TOTALIZADOR_SUB_ITEM );

function tratarDataMesAno( $data ){
	if( !$data ) return '';
	return substr($data, 3, 4).substr($data, 0, 2);
}

function recuperarSql(){
	
	$arWhere = array();
	if ( count( $_REQUEST['analista'] ) && $_REQUEST['analista'][0] ){
		array_push( $arWhere, " \"ANALISTA\" in ( '" . implode( "','", $_REQUEST['analista'] ) . "' ) " );
	}
	if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] ){
		array_push( $arWhere, "  \"UF\" in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
	}
	if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] ){
		array_push( $arWhere, " \"MUNICÍPIO\" in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
	}
	if ( count( $_REQUEST['ideb'] ) && $_REQUEST['ideb'][0] ){
		array_push( $arWhere, " \"CLASSIFICAÇÃO IDEB\"  in ( '" . implode( "','", $_REQUEST['ideb'] ) . "' ) " );
	}
	if ( count( $_REQUEST['dimcod'] ) && $_REQUEST['dimcod'][0] ){
		array_push( $arWhere, " \"DIMENSÃO\"  in ( '" . implode( "','", $_REQUEST['dimcod'] ) . "' ) " );
	}
	if ( count( $_REQUEST['sbaid'] ) && $_REQUEST['sbaid'][0] ){
		array_push( $arWhere, " \"SUBAÇÃO\"  in ( '" . implode( "','", $_REQUEST['sbaid'] ) . "' ) " );
	}
	if ( count( $_REQUEST['secretaria'] ) && $_REQUEST['secretaria'][0] ){
		array_push( $arWhere, " \"SECRETARIA/ÓRGÃO\"   in ( '" . implode( "','", $_REQUEST['secretaria'] ) . "' ) " );
	}
	if ( count( $_REQUEST['undid'] ) && $_REQUEST['undid'][0] ){
		array_push( $arWhere, " \"F10\" in ( '" . implode( "','", $_REQUEST['undid'] ) . "' ) " );
	}
	if ( count( $_REQUEST['prgid'] ) && $_REQUEST['prgid'][0] ){
		array_push( $arWhere, " \"PROGRAMA\"  in ( '" . implode( "','", $_REQUEST['prgid'] ) . "' ) " );
	}
	if ( count( $_REQUEST['indicador'] ) && $_REQUEST['indicador'][0] ){
		array_push( $arWhere, " \"INDICADOR PAINEL\"  in ( '" . implode( "','", $_REQUEST['indicador'] ) . "' ) " );
	}
	if ( count( $_REQUEST['fonte'] ) && $_REQUEST['fonte'][0] ){
		array_push( $arWhere, " \"Fonte\" in ( '" . implode( "','", $_REQUEST['fonte'] ) . "' ) " );
	}
	
	$stWhere = count( $arWhere ) ? ' where ' . implode( "and ", $arWhere ) : "";
	
	$sql = "select \"ANALISTA\", 
				   \"UF\", 
				   \"MUNICÍPIO\", 
				   \"CLASSIFICAÇÃO IDEB\", 
				   \"DIMENSÃO\",
				   \"SUBAÇÃO\" || ' - ' || \"DESCRIÇÃO DA SUBAÇÃO\" as subacao,  
				   \"SUBAÇÃO\", 
				   \"SECRETARIA/ÓRGÃO\", 
				   \"DESCRIÇÃO DA SUBAÇÃO\", 
				   \"F10\" as unidade, 
				   \"PROGRAMA\", 
				   \"INDICADOR PAINEL\",
				   COALESCE(\"QTD TERMO DE COOPERAÇÃO\", '0') AS \"QTD TERMO DE COOPERAÇÃO\", 
				   COALESCE(\"QTD MONITORAMENTO\", '0') AS \"QTD MONITORAMENTO\", 
				   COALESCE(\"QTD PAINEL\", '0') AS \"QTD PAINEL\",
				   COALESCE(\"QTD OUTRA FONTE (especificar na observação)\", '0') AS \"QTD OUTRA FONTE (especificar na observação)\",
				   \"Fonte\"
			from cte.ctemonitoramentointerno mi
			$stWhere
			order by \"UF\", \"MUNICÍPIO\", \"DIMENSÃO\", \"SUBAÇÃO\" ";
			
//			ver( $sql, d );
			
	return $sql;
}

function recuperarArColunas(){

	$coluna 	= array();
	$agrupador 	= $_REQUEST['agrupador'];
	
	if(in_array('subacao', $agrupador)){
		$coluna[] = array( "campo" 	  => "unidade",
//						   "type" 	  => "string",
				   		   "label" 	  => "Unidade de Medida" );
	}
	
	$coluna[] = array( "campo" 	  => "QTD TERMO DE COOPERAÇÃO",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Qtde. TCT" );
	
	$coluna[] = array( "campo" 	  => "QTD MONITORAMENTO",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Qtd. Monit.",
					   "php"	  => array("expressao" => "is_numeric({QTD MONITORAMENTO}) && {QTD MONITORAMENTO} > 0",
										   "var"	   => "percent",
										   "true"	   => "round( {QTD MONITORAMENTO} / {QTD TERMO DE COOPERAÇÃO} * 100, 2)",	
										   "false"	   => "0",
										   "type"	   => "numeric",			
										   "html"	   => "{QTD MONITORAMENTO} ({percent}%)") 
					 );
	
	$coluna[] = array( "campo" 	  => "QTD PAINEL",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Qtd Painel", 
					   "php"	  => array("expressao" => "is_numeric({QTD PAINEL}) && {QTD PAINEL} > 0",
										   "var"	   => "percent",
										   "true"	   => "round( {QTD PAINEL} / {QTD TERMO DE COOPERAÇÃO} * 100, 2)",	
										   "false"	   => "0",
										   "type"	   => "numeric",			
										   "html"	   => "{QTD PAINEL} ({percent}%)") 
					);
	
	if(in_array('subacao', $agrupador)){
		$coluna[] = array( "campo" 	  => "INDICADOR PAINEL",
						   "type" 	  => "string",
				   		   "label" 	  => "Ind. Painel" );
	}
		
	$coluna[] = array( "campo" 	  => "QTD OUTRA FONTE (especificar na observação)",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Qtd Outra Fonte", 
					   "php"	  => array("expressao" => "is_numeric({QTD OUTRA FONTE (especificar na observação)}) && {QTD OUTRA FONTE (especificar na observação)} > 0",
										   "var"	   => "percent",
										   "true"	   => "round( {QTD OUTRA FONTE (especificar na observação)} / {QTD TERMO DE COOPERAÇÃO} * 100, 2)",	
										   "false"	   => "0",
										   "type"	   => "numeric",			
										   "html"	   => "{QTD OUTRA FONTE (especificar na observação)} ({percent}%)")	
					);
	
	if(in_array('subacao', $agrupador)){
		$coluna[] = array( "campo" 	  => "Fonte",
//						   "type" 	  => "string",
				   		   "label" 	  => "Fonte" );
	}	
	
	return $coluna;
	
}

function recuperarArAgrupador(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
										 "QTD TERMO DE COOPERAÇÃO",
										 "QTD MONITORAMENTO",
										 "QTD PAINEL",
										 "QTD OUTRA FONTE (especificar na observação)"
										  )
				);
				
	if(in_array('subacao', $agrupador)){
		$agp['agrupadoColuna'][] = 'unidade';
		$agp['agrupadoColuna'][] = 'INDICADOR PAINEL';
		$agp['agrupadoColuna'][] = 'Fonte';
	}
				
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "estado":
				array_push($agp['agrupador'], array(
													"campo" => "UF",
											  		"label" => "UF")									
									   				);
				break;
			case "muncod":
				array_push($agp['agrupador'], array(
													"campo" => "MUNICÍPIO",
											  		"label" => "Município"									
									   				) );
				break;
			case "ideb":
				array_push($agp['agrupador'], array(
													"campo" => "CLASSIFICAÇÃO IDEB",
											  		"label" => "Classificação IDEB")										
									   				);
				break;
			case "dimensao":
				array_push($agp['agrupador'], array(
													"campo" => "DIMENSÃO",
											  		"label" => "Dimensão")										
									   				);
				break;
			case "subacao":
				array_push($agp['agrupador'], array(
													"campo" => "subacao",
											  		"label" => "Subação")						
									   				);
				break;
			case "secretaria":
				array_push($agp['agrupador'], array(
													"campo" => "SECRETARIA/ÓRGÃO",
											  		"label" => "Secretaria/Órgão")						
									   				);
				break;
			case "unidade":
				array_push($agp['agrupador'], array(
													"campo" => "unidade",
											  		"label" => "Unidade de Medida")						
									   				);
				break;
			case "programa":
				array_push($agp['agrupador'], array(
													"campo" => "PROGRAMA",
											  		"label" => "Programa")						
									   				);
				break;
			case "indicador":
				array_push($agp['agrupador'], array(
													"campo" => "INDICADOR PAINEL",
											  		"label" => "Indicador Painel")						
									   				);
				break;
			case "fonte":
				array_push($agp['agrupador'], array(
													"campo" => "Fonte",
											  		"label" => "Fonte")		
									   				);
				break;
			case "analista":
				array_push($agp['agrupador'], array(
													"campo" => "ANALISTA",
											  		"label" => "Analista")						
									   				);
				break;
		}
	}

	return $agp;
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>