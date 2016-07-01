<?
set_time_limit(0);

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}

function verifica( $total, $total2 ) {
	if( $total == 0 && $total2 <> 0){
		return "<div style='background-color:#1E90FF; color:#FFFFFF;'>100%</div>";
	} elseif( $total == 0 && $total2 == 0 ){
		return "<div style='background-color:#FF0000; color:#000000;'>0%</div>";
	} else {
		$valor = round(($total2/$total)*100,2);
		if( $valor >= 0 && $valor <= 49 ){
			return "<div style='background-color:#FF0000; color:#000000;'>".$valor."%</div>";
		}elseif($valor > 49 && $valor <= 74){
			return "<div style='background-color:#FFFF00; color:#000000;'>".$valor."%</div>";
		}elseif($valor > 74 && $valor <= 99){
			return "<div style='background-color:#00EE00; color:#FFFFFF;'>".$valor."%</div>";
		}elseif($valor >= 100){
			return "<div style='background-color:#1E90FF; color:#FFFFFF;'>".$valor."%</div>";
		}
	}
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
include APPRAIZ. 'includes/classes/relatorio.class.inc';

//xx($_POST);

$sql    = monta_sql();
$dados  = $db->carregar($sql);
$agrup  = monta_agp();
$col    = monta_coluna();
$coltop = array( 0 => array( "label" => "2007", "colspan" => "3" ),
				 1 => array( "label" => "2008", "colspan" => "3" ),
				 2 => array( "label" => "2009", "colspan" => "3" ),
				 3 => array( "label" => "2010", "colspan" => "3" ),
				 4 => array( "label" => "2011", "colspan" => "3" ),
				 5 => array( "label" => "2012", "colspan" => "3" ),
				 6 => array( "label" => "Total", "colspan" => "3" )
				);
//dbg($sql,1);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setColunaTop($coltop);
$r->setBrasao($true ? true : false);
$r->setTotNivel(true);
$r->setEspandir(true);
//$r->setBrasao(true);
echo $r->getRelatorio();

function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if ($f_tipoensino){
		$where[] = " orgid = '$f_tipoensino'";
	}

	if ($dtini && $dtfim){
		$where[] = " ( dtinclusao BETWEEN '".formata_data_sql($dtini)." 00:00:00' AND '".formata_data_sql($dtfim)." 23:59:59' AND  dtinclusao <> date('1900-01-01') ) ";
	}
	
	if (!empty($_SESSION["academico"]["orgid"])){
		$funid = $_SESSION["academico"]["orgid"] == 1 ? 18 : 17;	
	}
	
//	$exercicio = !is_array($exercicio) ? explode(',', $exercicio) : $exercicio;
	$unidade   = !is_array($unidade)   ? explode(',', $unidade)   : $unidade;
	$campus    = !is_array($campus)    ? explode(',', $campus)    : $campus;
	$programa  = !is_array($programa)  ? explode(',', $programa)  : $programa;
//	$classe    = !is_array($classe)    ? explode(',', $classe)    : $classe;

/*	if( $exercicio[0] && ( $exercicio_campo_flag || $exercicio_campo_flag == '1' )){
		$where[] = " ano " . (( $exercicio_campo_excludente == null || $exercicio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exercicio ) . "') ";		
	}*/
	
	if( $instituicao[0] && ( $instituicao_campo_flag || $instituicao_campo_flag == '1' )){
		$where[0] = " AND e.entid " . (( $instituicao_campo_excludente == null || $instituicao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $instituicao ) . "') ";
	}
		
	if( $campus[0] && ( $campus_campo_flag || $campus_campo_flag == '1' )){
		$where[1] = " AND campus.codigo " . (($campus_campo_excludente == null || $campus_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $campus ) . "') ";
	}
	
	if( $programa[0] && ( $programa_campo_flag || $programa_campo_flag == '1' )){
		$where[2] = " AND campus.pgcid " . (( $programa_campo_excludente == null || $programa_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $programa ) . "') ";
	}
	
/*	if( $classe[0] && ( $classe_campo_flag || $classe_campo_flag == '1' )){
		$where[] = " idclasse " . (( $classe_campo_excludente == null || $classe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $classe ) . "') ";
	}*/
	
	$agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador; 
	
	//if (strpos($agrupador, "unidade") !== false){
//		$select[] = "COALESCE(unidade , 'Não informado') as unidade";
//		$select[] = "idunidade as idunidade";		
	//}

	//if (strpos($agrupador, "campus") !== false){
//		$select[] = "COALESCE(campus, 'Não informado') as campus";
//		$select[] = "idcampus as idcampus";	
	//}	

	//if (strpos($agrupador, "classe") !== false){
//		$select[] = "COALESCE(classe, 'Não informado') as classe";
//		$select[] = "idclasse as idclasse";	
	//}	

	//if (strpos($agrupador, "tipoensino") !== false || !$agrupador){
//		$select[] = "tipoensino as tipoensino";	
	//}	
	
	//if (strpos($agrupador, "programa") !== false){
//		$select[] = "programa as programa";
//		$select[] = "idprograma as idprograma";	
	//}	

	//if (strpos($agrupador, "ano") !== false){
//		$select[] = "ano";
	//}	
	
	//if (strpos($agrupador, "portaria") !== false){
//		$select[] = "'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria";
	//}	

	//SQL ANTIGO
	/**/
		
		
		$sql = "SELECT DISTINCT
					COALESCE('' || UPPER(e.entsig) ||  ' - ' || UPPER(e.entnome) || '' , 'Não informado') as nome,
					campus.descricao,
					sum(COALESCE(campus.prev2007,0)) as prev2007, 
					sum(COALESCE(campus.prev2008,0)) as prev2008,
					sum(COALESCE(campus.prev2009,0)) as prev2009,
					sum(COALESCE(campus.prev2010,0)) as prev2010,
					sum(COALESCE(campus.prev2011,0)) as prev2011,
					sum(COALESCE(campus.prev2012,0)) as prev2012,
					COALESCE(campus.prev2007,0) + 
					COALESCE(campus.prev2008,0) +
					COALESCE(campus.prev2009,0) +
					COALESCE(campus.prev2010,0) +
					COALESCE(campus.prev2011,0) +
					COALESCE(campus.prev2012,0) as total,
					e.entsig,
					e.entnome as curso,
					COALESCE(campus.pgcdsc, 'Não informado') as prog,
					campus.curso ||  ' - ' || campus.turprev as cursoturno,
					COALESCE(equivalencia.cdtvgexecano2007,0) as cdtvgexecano2007,
					COALESCE(equivalencia.cdtvgexecano2008,0) as cdtvgexecano2008,
					COALESCE(equivalencia.cdtvgexecano2009,0) as cdtvgexecano2009,
					COALESCE(equivalencia.cdtvgexecano2010,0) as cdtvgexecano2010,
					COALESCE(equivalencia.cdtvgexecano2011,0) as cdtvgexecano2011,
					COALESCE(equivalencia.cdtvgexecano2012,0) as cdtvgexecano2012,
					COALESCE(equivalencia.cdtvgexecano2007,0) +
					COALESCE(equivalencia.cdtvgexecano2008,0) +
					COALESCE(equivalencia.cdtvgexecano2009,0) +
					COALESCE(equivalencia.cdtvgexecano2010,0) +
					COALESCE(equivalencia.cdtvgexecano2011,0) +
					COALESCE(equivalencia.cdtvgexecano2012,0) as total2,
					0 as percent
				FROM
					entidade.entidade e 
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				LEFT JOIN 
					entidade.endereco ed ON e.entid = ed.entid
				LEFT JOIN
					( SELECT
							e.entid AS codigo,
							e.entnome AS descricao,
							ea.entid AS instituicao,
							curd.cdtvgprevano2007 as prev2007,
							curd.cdtvgprevano2008 as prev2008,
							curd.cdtvgprevano2009 as prev2009,
							curd.cdtvgprevano2010 as prev2010,
							curd.cdtvgprevano2011 as prev2011,
							curd.cdtvgprevano2012 as prev2012,
							prog.pgcid as pgcid,
							prog.pgcdsc as pgcdsc,
							curso.curdsc as curso,
							curso.curid,
							te.turdsc AS turprev
						FROM
							entidade.entidade e2
						INNER JOIN 
							entidade.entidade e ON e2.entid = e.entid
						INNER JOIN 
							entidade.funcaoentidade ef ON e.entid = ef.entid
						INNER JOIN 
							entidade.funentassoc ea ON ef.fueid = ea.fueid 
						LEFT JOIN
							academico.cursodetalhe curd ON curd.entid = e.entid
						LEFT JOIN 
							academico.turno te ON te.turid = curd.turidexecutado
						LEFT JOIN
							public.curso curso ON curd.curid = curso.curid
						LEFT JOIN
							academico.programacurso prog ON prog.pgcid = curd.pgcid
						WHERE 
							e.entstatus = 'A' 
							AND ef.funid = 18
						) as campus ON campus.instituicao = e.entid
				LEFT JOIN
					( SELECT DISTINCT
							cdtcodigoemec as codemec,
							curdsc as curso,
							curidpactuado,
							entnome as campus,
							cd.entid as entid,
							pgcdsc as programa,
							arcdsc as area,
							cdtduracao as duracao,
							te.turdsc AS turprev,
							cdtid,
							cdtvgexecano2007, cdtvgexecano2008, cdtvgexecano2009, cdtvgexecano2010, cdtvgexecano2011, cdtvgexecano2012	
						FROM 
							academico.cursoequivalencia eq
						INNER JOIN
							academico.cursodetalhe cd ON eq.curidexecutado = cd.curid
						INNER JOIN 
							public.curso c ON c.curid = cd.curid
						INNER JOIN
							entidade.entidade e ON e.entid = cd.entid
						INNER JOIN
							academico.areacurso ar ON ar.arcid = cd.arcid
						LEFT JOIN 
							academico.programacurso pc ON pc.pgcid = cd.pgcid
						LEFT JOIN
							academico.turno tp ON tp.turid = cd.turidprevisto
						LEFT JOIN 
							academico.turno te ON te.turid = cd.turidexecutado
						WHERE
							cdtpactuacao = 'E'
							AND cdtstatus = 'A'
						ORDER BY
							curdsc
					) as equivalencia ON campus.curid = equivalencia.curidpactuado
				
				WHERE
					e.entstatus = 'A'
					AND ef.funid  in ('12')
					{$where[0]}
					{$where[1]}
					{$where[2]}
				GROUP BY
					e.entsig, e.entnome, campus.descricao, campus.prev2007, campus.prev2008, campus.prev2009, campus.prev2010, campus.prev2011, campus.prev2012,
					campus.pgcdsc, campus.curso, campus.turprev, equivalencia.cdtvgexecano2007, equivalencia.cdtvgexecano2008, equivalencia.cdtvgexecano2009,
					equivalencia.cdtvgexecano2010, equivalencia.cdtvgexecano2011, equivalencia.cdtvgexecano2012
				ORDER BY
					 e.entsig, e.entnome";
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	if (!$agrupador){
		$agrupador = array(
							'tipoensino',
/*							'ano',
							'portaria',
							'unidade',
							'campus',
							'programa',
							'classe'*/
						 );
	}elseif ( !is_array($agrupador) ){
		$agrupador = explode(",", $agrupador);
	}	
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"prev2007",	
											"prev2008",	
											"prev2009",	
											"prev2010",	
											"prev2011",	
											"prev2012",	
											"total",	
									   		"cdtvgexecano2007", 
 									   		"cdtvgexecano2008",
 									   		"cdtvgexecano2009",
 									   		"cdtvgexecano2010",
											"cdtvgexecano2011",
											"cdtvgexecano2012",
											"total2",
											"percent2007",	
											"percent2008",	
											"percent2009",	
											"percent2010",	
											"percent2011",	
											"percent2012",
											"percent"
										)	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'campus':
				array_push($agp['agrupador'], array(
													"campo" => "descricao",
											 		"label" => "Campus")										
									   				);					
		    	continue;			
		        break;	
		    case 'programa':
				array_push($agp['agrupador'], array(
												"campo" => "prog",
												"label" => "Programa")										
										   		);	
				continue;
				break;	    	
		    case 'instituicao':
				array_push($agp['agrupador'], array(
												"campo" => "nome",
												"label" => "Instituicao")										
										   		);	
				continue;
				break;					
		    case 'cursoturno':
				array_push($agp['agrupador'], array(
												"campo" => "cursoturno",
												"label" => "Curso")										
										   		);	
				continue;
				break;					
		}
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	$arrPerfil = array(
						PERFIL_SUPERUSUARIO,
						PERFIL_ADMINISTRADOR
					  );
	
	$permissao = academico_possui_perfil($arrPerfil);
	
	$coluna    = array(
						array(
							  "campo" => "prev2007",
					   		  "label" => "P",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "cdtvgexecano2007",
					   		  "label" => "E",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "prev2007",
					   		  "label" => "%",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({cdtvgexecano2007} == -1)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
//											   "false" => "round((({cdtvgexecano2007}/{percent2007})*100),2)",
											   "false" => "verifica( {prev2007}, {cdtvgexecano2007} )",
											   "html" => "{percentual}"
												)
						),
						array(
							  "campo" => "prev2008",
					   		  "label" => "P",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "cdtvgexecano2008",
					   		  "label" => "E",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "prev2008",
					   		  "label" => "%",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({cdtvgexecano2008} == -1)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
//											   "false" => "round((({cdtvgexecano2008}/{percent2008})*100),2)",
											   "false" => "verifica( {prev2008}, {cdtvgexecano2008} )",
											   "html" => "{percentual}")
						),
						array(
							  "campo" => "prev2009",
					   		  "label" => "P",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "cdtvgexecano2009",
					   		  "label" => "E",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "prev2009",
					   		  "label" => "%",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({cdtvgexecano2009} == -1)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
//											   "false" => "round((({cdtvgexecano2009}/{percent2009})*100),2)",
											   "false" => "verifica( {prev2009}, {cdtvgexecano2009} )",
											   "html" => "{percentual}")
						),
						array(
							  "campo" => "prev2010",
					   		  "label" => "P",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "cdtvgexecano2010",
					   		  "label" => "E",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "prev2010",
					   		  "label" => "%",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({cdtvgexecano2010} == -1)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
//											   "false" => "round((({cdtvgexecano2010}/{percent2010})*100),2)",
											   "false" => "verifica( {prev2010}, {cdtvgexecano2010} )",
											   "html" => "{percentual}")
						),
						array(
							  "campo" => "prev2011",
					   		  "label" => "P",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "cdtvgexecano2011",
					   		  "label" => "E",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "prev2011",
					   		  "label" => "%",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({cdtvgexecano2011} == -1)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
//											   "false" => "round((({cdtvgexecano2011}/{percent2011})*100),2)",
											   "false" => "verifica( {prev2011}, {cdtvgexecano2011} )",
											   "html" => "{percentual}")
						),
						array(
							  "campo" => "prev2012",
					   		  "label" => "P",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "cdtvgexecano2012",
					   		  "label" => "E",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "prev2012",
					   		  "label" => "%",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({cdtvgexecano2012} == -1)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
//											   "false" => "round((({cdtvgexecano2012}/{percent2012})*100),2)",
											   "false" => "verifica( {prev2012}, {cdtvgexecano2012} )",
											   "html" => "{percentual}")
						),
						array(
							  "campo" => "total",
					   		  "label" => "P",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "total2",
					   		  "label" => "E",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "percent",
					   		  "label" => "%",
							  "type"  => "numeric",
							  "php"   => array("expressao" => "({total2} == -1)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
											 //  "false" => "round((({total2}/{total})*100),2)",
											   "false" => "verifica( {total}, {total2} )",
											   "html" => "{percentual}")
//											   "html" => "verificaCor( {percentual} )")
						)
					);
					
	/*if ( $permissao ){				
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric",
								  "html"  	 => "<span style='color:{color}'>{homologado}</span>",
								  "php" 	 => array(
									  					"expressao" => "{homologado} > {autorizado}",
														"var" 		=> "color",
														"true"	    => "red",
														"false"     => "#0066CC",
									  				 )	
							)
				  );			
	}else{
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric"
								 )
				  );			

	}
	
	array_push($coluna,array(
							  "campo" => "provimento",
					   		  "label" => "Provimentos <br/>Autorizados <br/>(E)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "lepvlrprovefetivados",
					   		  "label" => "Provimentos <br/>Efetivados <br/>(F)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "provimentosnaoefetivados",
					   		  "label" => "Provimentos <br/>Não Efetivados <br/>(G)=(E)-(F)",
							  "type"  => "numeric"
						)
				);	

				
	if ( $permissao ){				
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric",
									  "html"  => "{valorcampo}",
									  "php"   => array(
									  					"expressao" => "({homologado} - {provimento} >= 0) || ({provimentopendencia} > 0)",
														"type"		=> "string",
									  					"var"       => "valorcampo",
														"true"		=> "{provimentopendencia}",
														"false"		=> "<span style='color:red;'>-</span>"					
									  				   )
								)
				  );			
	}else{
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric"		
								  )
				  );			

	}	*/		

	return $coluna;			  	
}
?>
</body>