<?php

set_time_limit(0);
ini_set( "display_error", 1 );
ini_set( "memory_limit", "1024M" );
error_reporting( E_ALL ^ E_NOTICE );

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
$rel->setMonstrarTolizadorNivel( TIPO_TOTALIZADOR_SUB_ITEM );

function recuperarSql(){
	
	
	$arWhere = array();
	$arInner = array();
	
	// Região
	if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] ){
		array_push( $arWhere, " r.regcod::integer in ( " . implode( ",", $_REQUEST['regcod'] ) . " ) " );
	}
	// UF
	if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] ){
		array_push( $arWhere, " es.estuf in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
	}
	// Município
	if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] ){
		array_push( $arWhere, " mu.muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
	}
	// Forma de Execução
	if ( count( $_REQUEST['frmid'] ) && $_REQUEST['frmid'][0] ){
		array_push( $arWhere, " frm.frmid in ( '" . implode( "','", $_REQUEST['frmid'] ) . "' ) " );
	}
	// Período
	if ( count( $_REQUEST['perid'] ) && $_REQUEST['perid'][0] ){
		array_push( $arWhere, " per.perid in ( '" . implode( "','", $_REQUEST['perid'] ) . "' ) " );
	}
	// Estado da subação
	if ( count( $_REQUEST['estid'] ) && $_REQUEST['estid'][0] ){
		array_push( $arWhere, " ess.estid in ( '" . implode( "','", $_REQUEST['estid'] ) . "' ) " );
	}
	// Tipos de Restrição
	if ( count( $_REQUEST['trscod'] ) && $_REQUEST['trscod'][0] ){
		array_push( $arWhere, " re.trscod in ( '" . implode( "','", $_REQUEST['trscod'] ) . "' ) " );
		array_push( $arInner, ' LEFT JOIN cte.restricaoexecucao AS re ON re.mntid = mnt.mntid ' );
		
	}
	// Dimensão
	if ( count( $_REQUEST['dimid'] ) && $_REQUEST['dimid'][0] ){
		array_push( $arWhere, " d.dimid in ( '" . implode( "','", $_REQUEST['dimid'] ) . "' ) " );
	}
	// Área Dimensão
	if ( count( $_REQUEST['ardid'] ) && $_REQUEST['ardid'][0] ){
		array_push( $arWhere, " a.ardid in ( '" . implode( "','", $_REQUEST['ardid'] ) . "' ) " );
	}
	// Indicador
	if ( count( $_REQUEST['indid'] ) && $_REQUEST['indid'][0] ){
		array_push( $arWhere, " i.indid in ( '" . implode( "','", $_REQUEST['indid'] ) . "' ) " );
	}
	
	$stInner = count( $arInner ) ? implode( " ", $arInner ) : "";
	$stWhere = count( $arWhere ) ? ' and ' . implode( "and ", $arWhere ) : "";
	
	$sql = "SELECT  distinct
				d.dimid, d.dimcod, d.dimdsc
				,d.dimcod || ' - ' || d.dimdsc as dimensao
				,a.ardid, a.ardcod, a.arddsc 
				,a.ardcod || ' - ' || a.arddsc  as area
				,i.indid, i.indcod, i.inddsc
				,i.indcod || ' - ' || i.inddsc as indicador
				,s.sbaid, s.sbaordem, s.sbadsc
				,d.dimcod || '.' || a.ardcod || '.' || i.indcod || ' (' || s.sbaordem || ') - ' || s.sbadsc as subacao
				,mnt.mntid
				,prg.prgdsc
				,ume.unddsc 
				,frm.frmdsc 
				,ess.estid, ess.estdsc
				,exs.exeid, coalesce( exs.exeqtd, 0 ) as exeqtd, exs.exeatual
				, substr( exs.exedtexecucao, 5,  2 ) || '/' || substr( exs.exedtexecucao, 1,  4 ) as exedtexecucao
				,per.perid, per.perdsc
				,mu.muncod, mu.mundescricao, mu.estuf, 	r.regdescricao
			FROM cte.indicador i 
				INNER JOIN cte.areadimensao a on a.ardid = i.ardid 
				INNER JOIN cte.dimensao d on d.dimid = a.dimid 
				INNER JOIN cte.pontuacao p on p.indid = i.indid
				INNER JOIN cte.criterio c on c.crtid = p.crtid
				INNER JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid  
				INNER JOIN cte.subacaoindicador s on s.aciid = aim.aciid 
				INNER JOIN cte.subacaoparecertecnico spt on s.sbaid = spt.sbaid
				INNER JOIN cte.programa prg ON prg.prgid = s.prgid
				INNER JOIN cte.formaexecucao frm ON s.frmid = frm.frmid
				INNER JOIN cte.unidademedida ume ON s.undid = ume.undid
				INNER JOIN cte.proposicaosubacao ps ON ps.ppsid = s.ppsid
				INNER JOIN cte.monitoramentosubacao AS mnt ON mnt.sbaid = s.sbaid 
				INNER JOIN cte.execucaosubacao AS exs ON exs.mntid = mnt.mntid AND exs.exeatual = TRUE
				$stInner
				INNER JOIN cte.periodoreferencia AS per ON per.perid = exs.perid
				INNER JOIN cte.estadosubacao AS ess ON ess.estid = exs.estid
				INNER JOIN cte.instrumentounidade AS ins ON ins.inuid = p.inuid
			 	INNER JOIN territorios.municipio AS mu ON mu.muncod = ins.muncod
				INNER JOIN territorios.estado es ON es.estuf = ins.mun_estuf OR es.estuf = ins.estuf
				INNER JOIN territorios.regiao r ON r.regcod = es.regcod	
				INNER JOIN workflow.documento AS doc ON doc.docid = ins.docid 
				INNER JOIN workflow.estadodocumento AS est ON est.esdid = doc.esdid 
			WHERE mnt.mntstatus = 'A' 
			AND i.indstatus = 'A' 
			AND a.ardstatus = 'A' 
			AND d.dimstatus = 'A' 
			AND d.itrid = 2
			AND s.frmid IN ( 1, 2, 4, 5, 6, 7, 8, 9, 10 )
			AND spt.ssuid = 3 
			AND ppsindcobuni = false
			AND est.esdid in ( 14, 15, 11, 13)
			$stWhere
			
			-- Deixar essa linha comentada pois ajuda muito a fazer os testes
			-- and p.inuid = 1648
			 
			and p.ptostatus = 'A'
			and aim.acilocalizador = 'M'
			ORDER BY mu.estuf, mu.mundescricao, d.dimcod, a.ardcod, i.indcod,  s.sbaordem, s.sbadsc";
			
	return $sql;
}

function recuperarArColunas(){

	$coluna 	= array();
	$agrupador 	= $_REQUEST['agrupador'];
	
	$coluna[] = array( "campo" 	  => "prgdsc",
					   "type" 	  => "string",
			   		   "label" 	  => "Programa" );
	
	$coluna[] = array( "campo" 	  => "frmdsc",
					   "type" 	  => "string",
			   		   "label" 	  => "Forma de Execução" );
		
	$coluna[] = array( "campo" 	  => "unddsc",
					   "type" 	  => "string",
			   		   "label" 	  => "Unidade de Medida" );
		
	if( in_array('periodo', $agrupador) ){
		$coluna[] = array( "campo" 	  => "estdsc",
						   "type" 	  => "string",
				   		   "label" 	  => "Situação" );
		
		$coluna[] = array( "campo" 	  => "exedtexecucao",
						   "type" 	  => "string",
				   		   "label" 	  => "Data de Execução" );
	}
	
	$coluna[] = array( "campo" 	  => "exeqtd",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Quantidade" );
	
	
	return $coluna;
	
}

function recuperarArAgrupador(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
										  "prgdsc",
										  "frmdsc",
										  "unddsc",
										  "estdsc",
										  "exedtexecucao",
										  "exeqtd",
										  )
				);
							   				
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regdescricao",
											  		"label" => "Região")										
									   				);
				break;
			case "estado":
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "UF"									
									   				) );
				break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "Município")										
									   				);
				break;
			case "dimensao":
				array_push($agp['agrupador'], array(
													"campo" => "dimensao",
											  		"label" => "Dimensão")										
									   				);
				break;
			case "area":
				array_push($agp['agrupador'], array(
													"campo" => "area",
											  		"label" => "Área")										
									   				);
				break;
			case "indicador":
				array_push($agp['agrupador'], array(
													"campo" => "indicador",
											  		"label" => "Indicador")										
									   				);
				break;
			case "subacao":
				array_push($agp['agrupador'], array(
													"campo" => "subacao",
											  		"label" => "Subação")						
									   				);
				break;
			case "periodo":
				array_push($agp['agrupador'], array(
													"campo" => "perdsc",
											  		"label" => "Período")						
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