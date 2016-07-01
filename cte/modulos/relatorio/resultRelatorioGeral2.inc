<?

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","250M");
set_time_limit(0);
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$exibe = true;

if( $exibe ){
	
	$sql   = monta_sql( $exibe );
	$dados = $db->carregar($sql);
	$agrupador = monta_agp( $exibe );
	$coluna   = monta_coluna( $exibe );
	$x = new montaRelatorio();
	$x->setAgrupador($agrupador, $dados); 
	$x->setColuna($coluna);
	$x->setBrasao(false);
	$x->setTotNivel(false);
	$x->setEspandir(false);
	$x->setMonstrarTolizadorNivel(true);
	$x->setTotalizador(true);
	$x->setTolizadorLinha(false);
	
	echo $x->getRelatorio();
	
}
?>
</body>
</html>

<?php 
function monta_sql( $exibe = null ){
	global $filtroSession;

	extract($_SESSION['par']['postrelat']);

	$select = array();
	$from	= array();

		
//	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
//		$where[0] = " AND iu.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
//	}
	if( $dimensao[0] && ( $dimensao_campo_flag || $dimensao_campo_flag == '1' )){
//		$where[1] = " AND ee.estuf " . (( $dimensao_campo_excludente == null || $dimensao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dimensao ) . "') ";		
	}
	if( $areas[0] && ( $areas_campo_flag || $areas_campo_flag == '1' )){
//		$where[2] = " AND ee.estuf " . (( $areas_campo_excludente == null || $areas_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $areas ) . "') ";		
	}
	if( $indicador[0] && ( $indicador_campo_flag || $indicador_campo_flag == '1' )){
		$where[1] = " AND i.indid " . (( $indicador_campo_excludente == null || $indicador_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $indicador ) . "') ";		
	}
	if( $plano[0] && ( $plano_campo_flag || $plano_campo_flag == '1' )){
//		$where[4] = " AND ee.estuf " . (( $plano_campo_excludente == null || $plano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $plano ) . "') ";		
	}
	
	$sql = "SELECT
					estuf AS estado,
		            sbadsc AS descricaosubacao,
		            unddsc AS unidademedida,
		            sum(quantidadePorEscolaPlanejado + quantidadeGlobalPlanejado) as planejado, -- valordasubacao
		            sum(quantidadePorEscolaAprovado + quantidadeGlobalAprovado) AS aprovado,
		            sum(quantidadePorEscolaEmanalise + quantidadeGlobalEmanalise) AS emanalise,
		            ' - ' AS qtdmonitoramento,
		            ' - ' AS qtdpainel,
		            ' - ' AS qtdindcadorpainel,
		            ' - ' AS qtdoutrafonte,
		            ' - ' AS qtdfonte,
		            0 AS valor
		FROM (  -- POR ESCOLA --
		            SELECT           
		            			iu.estuf,
		                        s.sbaid,
		                        s.sbadsc,
		                        u.unddsc,
		                        0 as quantidadeGlobalPlanejado,
		                        0 as quantidadeGlobalAprovado,
		                        0 as quantidadeGlobalEmanalise,
		                        sum(coalesce(qf.qfaqtd,0)) as quantidadePorEscolaPlanejado,
		                        CASE WHEN spt.ssuid = 3  THEN sum(coalesce(qf.qfaqtd,0)) ELSE 0 END as quantidadePorEscolaAprovado,
		                        CASE WHEN spt.ssuid = 1  THEN sum(coalesce(qf.qfaqtd,0)) ELSE 0 END as quantidadePorEscolaEmanalise
		            FROM cte.dimensao d
		                        INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
		                        INNER JOIN cte.indicador i ON i.ardid = ad.ardid
		                        INNER JOIN cte.criterio c ON c.indid = i.indid
		                        INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
		                        INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
		                        INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
		                        INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
		                        INNER JOIN cte.unidademedida u on u.undid = s.undid
		                        INNER JOIN cte.qtdfisicoano qf ON qf.sbaid = s.sbaid 
		                        LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid AND qf.qfaano = spt.sptano
		            WHERE
		                      --   iu.inuid = 41
		                         s.sbaporescola = true -- por escola.
		                         AND s.frmid in (4) -- and s.sbaid = 2361
		                         AND iu.estuf = '".$estado."'
		                         ".$where[1]."
		            GROUP BY iu.estuf,
		                         s.sbaid,
		                         s.sbadsc,
		                         u.unddsc,
		                         spt.ssuid  
		
		            UNION ALL
		
		            -- GLOBAL --
		
		            SELECT           iu.estuf,
		                        s.sbaid,
		                        s.sbadsc,
		                        u.unddsc,
		                        sum(coalesce(spt.sptunt,0)) as quantidadeGlobalPlanejado,
		                        CASE WHEN spt.ssuid = 3  THEN sum(coalesce(spt.sptunt,0)) ELSE 0 END as quantidadeGlobalAprovado,
		                        CASE WHEN spt.ssuid = 1  THEN sum(coalesce(spt.sptunt,0)) ELSE 0 END as quantidadeGlobalEmanalise,
		                        0 as quantidadePorEscolaPlanejado,
		                        0 as quantidadePorEscolaAprovado,
		                        0 as quantidadePorEscolaEmanalise
		            FROM cte.dimensao d
		                        INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
		                        INNER JOIN cte.indicador i ON i.ardid = ad.ardid
		                        INNER JOIN cte.criterio c ON c.indid = i.indid
		                        INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
		                        INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
		                        INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
		                        INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid
		                        INNER JOIN cte.unidademedida u on u.undid = s.undid
		                        LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid 
		            WHERE
		                        -- iu.inuid = 41
		                         s.sbaporescola = false -- global.
		                         AND s.frmid in (4)
		                         AND iu.estuf = '".$estado."'
		                         ".$where[1]."
		            GROUP BY iu.estuf,
		                         s.sbaid,
		                         s.sbadsc,
		                         u.unddsc, spt.ssuid
		) AS foo
		GROUP BY      
		            estuf, sbadsc, unddsc
		ORDER BY 
		            estuf, sbadsc";

	
	return $sql;
}

function monta_agp( $exibe = null ){
	$agrupador = $_SESSION['par']['postrelat']['agrupador'];
	if( !is_array($agrupador) ){
		$agrupador = array();
	}
	
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"descricaosubacao",  
												"unidademedida",  
												"planejado",  
												"aprovado",
												"emanalise",
												"qtdmonitoramento",
												"qtdpainel",
												"qtdindcadorpainel",
												"qtdoutrafonte",
												"qtdfonte",
												"valor"
//												"descricaoarea",
//												"numeroconvenio",
//												"valorglobalconvenio",
//												"valorglobalempenhado",
//												"valorglobalpago",
//												"valorglobalapagar",
//												"valorglobalaempenhar",
//												"valorglobalconcedente",
//												"valorglobalproponente",
//												"saldoglobalconta",
//												"saldoglobalaplicacao",
//												"ultimadatadaconta",
//												"programa",
//												"planointerno"
										//		"qnt2",
											//	"percent"
												/*"projetado",	
										   		"autorizado", 
	 									   		"provimento",
	 									   		"publicado",
	 									   		"homologado",
												"lepvlrprovefetivados",
												"provimentosnaoefetivados",
												"provimentopendencia",
											    "homocolor"
											    */
											  )	  
					);
		
		$count = 1;
			$i = 0;
			
			if( $i > 1 || $i == 0 ){
				$vari = "";	
			}
			
		$novoAgrup = array(); 
		array_push($novoAgrup, 'estado');
		array_push($novoAgrup, 'subacao');
	//	array_push($novoAgrup, 'municipio');
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
				case 'estado':
					array_push($agp['agrupador'], array(
														"campo" => "estado",
												  		"label" => "$var Estado")										
										   				);				
			   		continue;
			    break;
//				case 'aprovado':
//					array_push($agp['agrupador'], array(
//														"campo" => "aprovado",
//												  		"label" => "$var Convenio")										
//										   				);				
//			   		continue;
//			    break;
	//			case 'municipio':
	//				array_push($agp['agrupador'], array(
	//													"campo" => "municipio",
	//											  		"label" => "$var Município")										
	//									   				);				
	//		   		continue;
	//		    break;
//			    case 'dimensao':
//					array_push($agp['agrupador'], array(
//														"campo" => "descricaodimensao",
//												  		"label" => "$var Dimensão")										
//										   				);					
//			    	continue;
//			    break;		    	
//			    case 'area':
//					array_push($agp['agrupador'], array(
//														"campo" => "descricaoarea",
//												 		"label" => "$var Área")										
//										   				);					
//			    	continue;			
//			    break;
//			    case 'indicador':
//					array_push($agp['agrupador'], array(
//														"campo" => "descricaoindicador",
//												 		"label" => "$var Índicador")										
//										   				);					
//			   		continue;			
//			    break;
//			    case 'acao':
//					array_push($agp['agrupador'], array(
//														"campo" => "descricaoacao",
//												 		"label" => "$var Ação")										
//										   				);					
//			   		continue;			
//			    break;
			    case 'subacao':
					array_push($agp['agrupador'], array(
														"campo" => "descricaosubacao",
												 		"label" => "$var Subação")										
										   				);					
			   		continue;			
			    break;
//			    case 'planointerno':
//					array_push($agp['agrupador'], array(
//														"campo" => "planointerno",
//												 		"label" => "$var Plano Interno")										
//										   				);					
//			   		continue;			
//			    break;
//			    case 'programa':
//					array_push($agp['agrupador'], array(
//														"campo" => "programa",
//												 		"label" => "$var Programa")										
//										   				);					
//			   		continue;			
//			    break;
			}
			$count++;
		endforeach;
		
	
	return $agp;
}

function monta_coluna( $exibe = null ){
	$agrupador = $_SESSION['par']['postrelat']['agrupador'];
	if( !is_array($agrupador) ){
		$agrupador = array();
	}
	$coluna = array();

	array_push( $agrupador, "descricaosubacao" );
	array_push( $agrupador, "unidademedida" );
	array_push( $agrupador, "planejado" );
	array_push( $agrupador, "aprovado" );
	array_push( $agrupador, "emanalise" );
	array_push( $agrupador, "qtdmonitoramento" );
	array_push( $agrupador, "qtdpainel" );
	array_push( $agrupador, "qtdindcadorpainel" );
	array_push( $agrupador, "qtdoutrafonte" );
	array_push( $agrupador, "qtdfonte" );
	array_push( $agrupador, "valor" );
	
	
		foreach ($agrupador as $val){
			switch ($val) {
				case 'valor':
					array_push($coluna, array(
							  "campo" => "valor",
					   		  "label" => "Valor",
							  "type"  => "<b>{valor}</b>")
										);				
			   		continue;
			    break;
				case 'qtdfonte':
					array_push($coluna, array(
							  "campo" => "qtdfonte",
					   		  "label" => "Fonte",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'qtdoutrafonte':
					array_push($coluna, array(
							  "campo" => "qtdoutrafonte",
					   		  "label" => "Qtd. Outra Fonte",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'qtdindcadorpainel':
					array_push($coluna, array(
							  "campo" => "qtdindcadorpainel",
					   		  "label" => "Qtd. Indicador",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'qtdpainel':
					array_push($coluna, array(
							  "campo" => "qtdpainel",
					   		  "label" => "Qtd. Painel",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'qtdmonitoramento':
					array_push($coluna, array(
							  "campo" => "qtdmonitoramento",
					   		  "label" => "Qtd. Monit.",
							  "type"  => "string")
										);				
			   		continue;
			    break;
				case 'emanalise':
					array_push($coluna, array(
							  "campo" => "emanalise",
					   		  "label" => "Em Análise",
							  "type"  => "numeric")
										);				
			   		continue;
			    break;
				case 'aprovado':
					array_push($coluna, array(
							  "campo" => "aprovado",
					   		  "label" => "Aprovado",
							  "type"  => "numeric")
										);				
			   		continue;
			    break;
				case 'planejado':
					array_push($coluna, array(
							  "campo" => "planejado",
					   		  "label" => "Planejado",
							  "type"  => "numeric")
										);				
			   		continue;
			    break;
				case 'unidademedida':
					array_push($coluna, array(
							  "campo" => "unidademedida",
					   		  "label" => "Unidade de Medida",
							  "type"  => "string")
										);				
			   		continue;
			    break;
			}
		
	}
	return $coluna;	
}
?>
