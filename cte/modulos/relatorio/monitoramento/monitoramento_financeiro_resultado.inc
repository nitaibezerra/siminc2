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
$rel->setTotalizador( false );
//$rel->setMonstrarTolizadorNivel( TIPO_TOTALIZADOR_SUB_ITEM );

function tratarDataMesAno( $data ){
	if( !$data ) return '';
	return substr($data, 3, 4).substr($data, 0, 2);
}

function recuperarSql(){
	
	
	$arCampo = array();
	$arInner = array();
	$arWhere = array();
	
	// Ano do Convênio
	array_push( $arWhere, " cs.cosano = '{$_REQUEST['anoreferencia']}' " );
	
	// Mês/Ano Monitoramento
	array_push( $arWhere, " to_char(hmcdatamonitoramento, 'YYYYMM') = '" . tratarDataMesAno( $_REQUEST['mesano'] ) . "' " );
	
	// Região
	if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] ){
		array_push( $arWhere, " r.regcod::integer in ( " . implode( ",", $_REQUEST['regcod'] ) . " ) " );
	}
	// UF
	if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] ){
		array_push( $arWhere, " es.estuf in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
	}
	// Estado do Item
	if ( count( $_REQUEST['scsid'] ) && $_REQUEST['scsid'][0] ){
		array_push( $arWhere, " scs.scsid in ( '" . implode( "','", $_REQUEST['scsid'] ) . "' ) " );
	}
	// Município
	if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] ){
		array_push( $arWhere, " mu.muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
		array_push( $arInner, ' inner join territorios.municipio as mu on mu.muncod = iu.muncod ' );
		array_push( $arCampo, ' mu.mundescricao ' );
	}
	
	$stCampo = count( $arCampo ) ? ', ' . implode( ", ", $arCampo ) : "";
	$stInner = count( $arInner ) ? implode( " ", $arInner ) : "";
	$stWhere = count( $arWhere ) ? ' and ' . implode( "and ", $arWhere ) : "";
	
	$sql = "select  prsnumconvsape || ' / ' || p.prsano as prsnumconvsape, 
					sbadsc, undddsc, cs.cosid, cs.cosdsc, cs.cosqtd, cs.cosvlruni, (cs.cosqtd * cs.cosvlruni) as totalprogramado
					, es.estuf || ' - ' || es.estdescricao as estado 
					$stCampo
					, umd.undddsc, cs.sbaid, coalesce(h.hmsid,0) as  hmsid
					, coalesce(hi.hciid,0) as hciid,  coalesce(hi.scsid,0) as scsid
					, coalesce(hmsvalorunitario, 0) as hmsvalorunitario, coalesce(hmsquantidadepaga, 0) as hmsquantidadepaga, 0, coalesce(hmsvalortotalpago, 0) as hmsvalortotalpago
					, to_char(hmcdatamonitoramento, 'MM') as mes , hmcstatus
					, scs.scsdesc, scs.scsdesc
					, coalesce(hic.tot_hmsvalortotalpago, 0) AS tot_hmsvalortotalpago
					, coalesce(hic.tot_hmsquantidadepaga, 0) AS tot_hmsquantidadepaga
			from cte.projetosape p
				inner join cte.instrumentounidade iu on iu.inuid = p.inuid
				inner join territorios.estado es on es.estuf = iu.mun_estuf or es.estuf = iu.estuf
				inner join cte.projetosapesubacao ps on p.prsid = ps.prsid
				inner join cte.subacaoindicador s on s.sbaid = ps.sbaid
				inner join cte.composicaosubacao cs on cs.sbaid = s.sbaid
				inner join cte.unidademedidadetalhamento umd on umd.unddid = cs.unddid
				left  join cte.historicomonitoramentoconvsubac h ON h.sbaid = cs.sbaid -- AND h.hmsid = 9642
				left  join cte.historicoconvitemcomposicao hi ON hi.hmsid = h.hmsid AND hi.cosid = cs.cosid -- AND h.hmsid = 9642
				left  join cte.historicomonitoramentoconvenio hmc ON hmc.hmcid = h. hmcid
				left  join cte.statuscomposicaosubacao scs ON scs.scsid = hi.scsid
				left join ( 
				     select 
				    	sum( hmsquantidadepaga ) as tot_hmsquantidadepaga,
						sum( hmsvalortotalpago ) as tot_hmsvalortotalpago, 
						cosid
				    from
						cte.historicoconvitemcomposicao i
						inner join cte.historicomonitoramentoconvsubac s on s.hmsid = i.hmsid
						inner join cte.historicomonitoramentoconvenio hc on hc.hmcid = s.hmcid
				    where
						i.hmsano = '{$_REQUEST['anoreferencia']}'
						-- and  to_char(hmcdatamonitoramento, 'yyyymm') = '" . tratarDataMesAno( $_REQUEST['mesano'] ) . "'
				    group by
						i.cosid
			  	  ) hic on hic.cosid = cs.cosid	
				$stInner
			where prsnumconvsape is not null
			-- and p.prsid = 1447
			$stWhere
			order by sbadsc, cosdsc";
			
//			ver( $sql );
			
	return $sql;
}

function recuperarArColunas(){

	$coluna 	= array();
	$agrupador 	= $_REQUEST['agrupador'];
	
	$coluna[] = array( "campo" 	  => "undddsc",
					   "type" 	  => "string",
			   		   "label" 	  => "Unidade de Medida" );
	
	$coluna[] = array( "campo" 	  => "cosqtd",
//					   "type" 	  => "string",
			   		   "label" 	  => "Qtd Programada" );
		
	$coluna[] = array( "campo" 	  => "cosvlruni",
//					   "type" 	  => "string",
			   		   "label" 	  => "Unitário Programado" );
		
	$coluna[] = array( "campo" 	  => "totalprogramado",
//					   "type" 	  => "string",
			   		   "label" 	  => "Total Programado" );
	
	$coluna[] = array( "campo" 	  => "hmsquantidadepaga",
//					   "type" 	  => "string",
			   		   "label" 	  => "Qtd Executado" );
		
	$coluna[] = array( "campo" 	  => "hmsvalorunitario",
//					   "type" 	  => "string",
			   		   "label" 	  => "Unitário Executado" );
		
	$coluna[] = array( "campo" 	  => "hmsvalortotalpago",
//					   "type" 	  => "string",
			   		   "label" 	  => "Total Executado" );
	
	$coluna[] = array( "campo" 	  => "tot_hmsquantidadepaga",
//					   "type" 	  => "string",
			   		   "label" 	  => "Qtd Acumulada" );
		
	$coluna[] = array( "campo" 	  => "tot_hmsvalortotalpago",
//					   "type" 	  => "string",
			   		   "label" 	  => "Total Acumulado" );
	
	$coluna[] = array( "campo" 	  => "scsdesc",
//					   "type" 	  => "string",
			   		   "label" 	  => "Status do Item de Composição" );
	
	return $coluna;
	
}

function recuperarArAgrupador(){
	
	if(!$_REQUEST['agrupador']) {
		
		die("<script>
				alert('Selecione um agrupador');
				window.close();
			 </script>");
		
	}
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
										  "undddsc",
										  "cosqtd",
										  "cosvlruni",
										  "totalprogramado",
										  "hmsquantidadepaga",
										  "hmsvalorunitario",
										  "hmsvalortotalpago",
										  "tot_hmsquantidadepaga",
										  "tot_hmsvalortotalpago",
										  "scsdesc",
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
													"campo" => "estado",
											  		"label" => "UF"									
									   				) );
				break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "Município")										
									   				);
				break;
			case "convenio":
				array_push($agp['agrupador'], array(
													"campo" => "prsnumconvsape",
											  		"label" => "Convênio")										
									   				);
				break;
			case "subacao":
				array_push($agp['agrupador'], array(
													"campo" => "sbadsc",
											  		"label" => "Subação")						
									   				);
				break;
			case "item":
				array_push($agp['agrupador'], array(
													"campo" => "cosdsc",
											  		"label" => "Item")						
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