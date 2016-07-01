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
	
	
		echo '<br>
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
				<tr>
					<td  class="subtituloesquerda">
					<center>
					Assistência Técnica
					</center>
					</td>
				</tr>
			</table>
			';
	echo '<iframe id="carregaIframe2" onmouseover="document.getElementById(\'carregaIframe2\').style.height = document.getElementById(\'carregaIframe2\').contentWindow.document.body.scrollHeight + \'px\';" marginheight="0" marginwidth="0" name="Localtermo2" 
			src="/cte/cte.php?modulo=relatorio/resultRelatorioGeral2&acao=A" 
			width="100%" frameborder="0" allowtransparency="true" scrolling="no"></iframe>';
	
}
?>
</body>
</html>
<script>
	function resizeFrame(f) {
		
	}
</script>	
<?php 
function monta_sql( $exibe = null ){
	global $filtroSession;

	extract($_SESSION['par']['postrelat']);

	$select = array();
	$from	= array();
		
//	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
//		$where[0] = " AND dshuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
//	}
	if( $dimensao[0] && ( $dimensao_campo_flag || $dimensao_campo_flag == '1' )){
//		$where[1] = " AND ee.estuf " . (( $dimensao_campo_excludente == null || $dimensao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dimensao ) . "') ";		
	}
	if( $areas[0] && ( $areas_campo_flag || $areas_campo_flag == '1' )){
//		$where[2] = " AND ee.estuf " . (( $areas_campo_excludente == null || $areas_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $areas ) . "') ";		
	}
	if( $indicador[0] && ( $indicador_campo_flag || $indicador_campo_flag == '1' )){
		$where[1] = " AND codindicador " . (( $indicador_campo_excludente == null || $indicador_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $indicador ) . "') ";		
	}
	if( $plano[0] && ( $plano_campo_flag || $plano_campo_flag == '1' )){
//		$where[4] = " AND ee.estuf " . (( $plano_campo_excludente == null || $plano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $plano ) . "') ";		
	}
	
	$sql = "SELECT
				*
			FROM ( SELECT
				i.indid as codindicador,
				i.indnome as indicador,
				dpeanoref as ano,
				dpeordem as ordem,
				dpe.dpedsc as periodo,
				dshuf as estado,
			--	est.estdescricao,
				sec.secdsc as responsavel,
				aca.acadsc as acao,
				ume.umedesc as unidademedida,
				tiddsc as detalhe,
				sum(dshqtde) as valor
			FROM
				painel.indicador i
			INNER JOIN painel.seriehistorica sh on sh.indid=i.indid
			INNER JOIN painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			INNER JOIN painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
			INNER JOIN territorios.estado est on est.estuf = dsh.dshuf
			INNER JOIN painel.secretaria sec on i.secid = sec.secid
			INNER JOIN painel.acao aca on i.acaid = aca.acaid
			INNER JOIN painel.unidademeta ume on i.umeid = ume.umeid
			LEFT JOIN painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
			WHERE
				i.indid in (471) 
				AND sehstatus <> 'I'
				AND i.indpublicado is true
				AND dpeanoref >= '2008'
				AND dshuf = '".$estado."'
			GROUP BY
				i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
			
				
				
			UNION ALL
			
			SELECT 
				i.indid as codindicador,
				i.indnome as indicador,
				dpeanoref as ano,
				dpeordem as ordem,
				dpe.dpedsc as periodo,
				dshuf as estado,
			--	est.estdescricao,
				sec.secdsc as responsavel,
				aca.acadsc as acao,
				ume.umedesc as unidademedida,
				tiddsc as detalhe,
				sum(dshqtde) as valor
			FROM
				painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
			inner join territorios.estado est on est.estuf = dsh.dshuf
			inner join painel.secretaria sec on i.secid = sec.secid
			inner join painel.acao aca on i.acaid = aca.acaid
			inner join painel.unidademeta ume on i.umeid = ume.umeid
			left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
			WHERE
				i.indid in (297)
				AND i.indpublicado is true
				AND sehstatus <> 'I'
				AND dpeanoref >= '2008'
				AND tidid = 474
				AND dshuf = '".$estado."'
			GROUP BY
				i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
			
			
			UNION ALL
			
			
			SELECT 
				i.indid as codindicador,
				i.indnome as indicador,
				dpeanoref as ano,
				dpeordem as ordem,
				dpe.dpedsc as periodo,
				dshuf as estado,
			--	est.estdescricao,
				sec.secdsc as responsavel,
				aca.acadsc as acao,
				ume.umedesc as unidademedida,
				tiddsc as detalhe,
				sum(dshqtde) as valor
			FROM
				painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
			inner join territorios.estado est on est.estuf = dsh.dshuf
			inner join painel.secretaria sec on i.secid = sec.secid
			inner join painel.acao aca on i.acaid = aca.acaid
			inner join painel.unidademeta ume on i.umeid = ume.umeid
			left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
			WHERE
				i.indid in (519)
				AND i.indpublicado is true
				AND sehstatus <> 'I'
				AND dpeanoref >= '2008'
				AND tidid = 1678
				AND dshuf = '".$estado."'
			GROUP BY
				i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc
			
			
			UNION ALL 
			
			
			SELECT
				i.indid as codindicador,
				i.indnome as indicador,
				dpeanoref as ano,
				dpeordem as ordem,
				dpe.dpedsc as periodo,
				dshuf as estado,
			--	est.estdescricao,
				sec.secdsc as responsavel,
				aca.acadsc as acao,
				ume.umedesc as unidademedida,
				tiddsc as detalhe,
				sum(dshqtde) as valor
			FROM
				painel.indicador i
			inner join painel.seriehistorica sh on sh.indid=i.indid
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
			inner join territorios.estado est on est.estuf = dsh.dshuf
			inner join painel.secretaria sec on i.secid = sec.secid
			inner join painel.acao aca on i.acaid = aca.acaid
			inner join painel.unidademeta ume on i.umeid = ume.umeid
			left join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
			WHERE
				i.indid in (520)
				AND i.indpublicado is true
				AND sehstatus <> 'I'
				AND dpeanoref >= '2008'
				AND dshuf = '".$estado."'
			GROUP BY
				i.indid, i.indnome, dpeanoref, dpeordem, dpedsc, dshuf, est.estdescricao, sec.secdsc, aca.acadsc, ume.umedesc, tiddsc ) as foo
			WHERE
				1=1
				".$where[1]."
			ORDER BY
				estado, codindicador, indicador, ano, ordem, periodo, responsavel, acao, unidademedida, detalhe";
			
	//	ver($sql);

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
												"indicador",  
												"ano",  
												"ordem",  
												"periodo",
												"estado",
												"responsavel",
												"acao",
												"unidademedida",
												"detalhe",
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
		array_push($novoAgrup, 'indicador');
		array_push($novoAgrup, 'periodo');
		array_push($novoAgrup, 'detalhe');
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
				case 'indicador':
					array_push($agp['agrupador'], array(
														"campo" => "indicador",
												  		"label" => "$var Indicador")										
										   				);				
			   		continue;
			    break;
				case 'periodo':
					array_push($agp['agrupador'], array(
														"campo" => "periodo",
												  		"label" => "$var Período")										
										   				);				
			   		continue;
			    break;
				case 'detalhe':
					array_push($agp['agrupador'], array(
														"campo" => "detalhe",
												  		"label" => "$var Detalhe")										
										   				);				
			   		continue;
			    break;
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
														"campo" => "descricaosubacao1",
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
	//ver($agrupador, d);
	
												
	array_push( $agrupador, "indicador" );
	array_push( $agrupador, "ano" );
	array_push( $agrupador, "periodo" );
	array_push( $agrupador, "responsavel" );
	array_push( $agrupador, "acao" );
	array_push( $agrupador, "unidademedida" );
	array_push( $agrupador, "detalhe" );
	array_push( $agrupador, "valor" );
	
	
	foreach ($agrupador as $val){
		switch ($val) {
			case 'valor':
				array_push($coluna, array(
						  "campo" => "valor",
				   		  "label" => "Valor",
						  "html"  => "<b>{valor}</b>")
									);				
		   		continue;
		    break;
			case 'detalhe':
				array_push($coluna, array(
						  "campo" => "detalhe",
				   		  "label" => "Detalhe",
						  "type"  => "string")
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
			case 'acao':
				array_push($coluna, array(
						  "campo" => "acao",
				   		  "label" => "Ação",
						  "type"  => "string")
									);				
		   		continue;
		    break;
			case 'responsavel':
				array_push($coluna, array(
						  "campo" => "responsavel",
				   		  "label" => "Responsável",
						  "type"  => "string")
									);				
		   		continue;
		    break;
			case 'periodo':
				array_push($coluna, array(
						  "campo" => "periodo",
				   		  "label" => "Período",
						  "type"  => "string")
									);				
		   		continue;
		    break;
			case 'ordem':
				array_push($coluna, array(
						  "campo" => "ordem",
				   		  "label" => "Ordem",
						  "type"  => "string")
									);				
		   		continue;
		    break;
			case 'indicador':
				array_push($coluna, array(
						  "campo" => "indicador",
				   		  "label" => "Indicador",
						  "type"  => "string")
									);				
		   		continue;
		    break;
			case 'aprovado':
				array_push($coluna, array(
						  "campo" => "ano",
				   		  "label" => "Ano",
						  "type"  => "string")
									);				
		   		continue;
		    break;
		}
	}
	
	return $coluna;	
}
?>
