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

//xx($_POST);

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
//dbg($sql,1);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
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
	
	$exercicio = !is_array($exercicio) ? explode(',', $exercicio) : $exercicio;
	$unidade   = !is_array($unidade)   ? explode(',', $unidade)   : $unidade;
	$campus    = !is_array($campus)    ? explode(',', $campus)    : $campus;
	$programa  = !is_array($programa)  ? explode(',', $programa)  : $programa;
	$classe    = !is_array($classe)    ? explode(',', $classe)    : $classe;

	if( $exercicio[0] && ( $exercicio_campo_flag || $exercicio_campo_flag == '1' )){
		$where[] = " ano " . (( $exercicio_campo_excludente == null || $exercicio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exercicio ) . "') ";
				
		$param .= "&anoexcludente=" . (( $exercicio_campo_excludente == null || $exercicio_campo_excludente == '0') ? 'IN' : 'NOT IN');
		$param .= "&ano=" . implode( ",", $exercicio );			
		
	}
	
	if( $unidade[0] && ( $unidade_campo_flag || $unidade_campo_flag == '1' )){
		$where[] = " idunidade " . (( $unidade_campo_excludente == null || $unidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unidade ) . "') ";
	}elseif ($_SESSION['academico']['entid'] && $filtroSession && $filtroSession == 'unidade'){
		$where[] = " idunidade = " . $_SESSION['academico']['entid'];
	}
		
	if( $campus[0] && ( $campus_campo_flag || $campus_campo_flag == '1' )){
		$where[] = " idcampus " . (($campus_campo_excludente == null || $campus_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $campus ) . "') ";
		
		$param .= "&campusexcludente=" . (($campus_campo_excludente == null || $campus_campo_excludente == '0') ? 'IN' : 'NOT IN');
		$param .= "&idcampus=" . implode( ",", $campus );
	}elseif ($_SESSION['academico']['entidcampus'] && $filtroSession && $filtroSession != 'unidade'){
		$where[] = " idcampus = " . $_SESSION['academico']['entidcampus'];
	}
	
	if( $programa[0] && ( $programa_campo_flag || $programa_campo_flag == '1' )){
		$where[] = " idprograma " . (( $programa_campo_excludente == null || $programa_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $programa ) . "') ";
		
		$param .= "&programaexcludente=" . (( $programa_campo_excludente == null || $programa_campo_excludente == '0') ? 'IN' : 'NOT IN');		
		$param .= "&idprograma=" . implode( ",", $programa );
	}
	
	if( $classe[0] && ( $classe_campo_flag || $classe_campo_flag == '1' )){
		$where[] = " idclasse " . (( $classe_campo_excludente == null || $classe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $classe ) . "') ";
	}
	
	if ($filtroSession == 'unidade'){
		$param .= "&entidunidade={$_SESSION['academico']['entid']}";
		$param .= (strpos($agrupador, "campus") !== false) ? "&entidcampus=' || idcampus || '" : '';
	}elseif ($filtroSession == 'campus'){
		$param .= "&entidcampus={$_SESSION['academico']['entidcampus']}";
	}
	
	$agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador; 
	
	//if (strpos($agrupador, "unidade") !== false){
		$select[] = "COALESCE(unidade , 'Não informado') as unidade";
		$select[] = "idunidade as idunidade";		
	//}

	//if (strpos($agrupador, "campus") !== false){
		$select[] = "COALESCE(campus, 'Não informado') as campus";
		$select[] = "idcampus as idcampus";	
	//}	
	
	//if (strpos($agrupador, "classe") !== false){
	$select[] = "COALESCE('<a href=\"#\" onclick=\"abreDetalhamentoLancamento(' || idclasse || ', \'" . $param . "\')\">' || classe || '</a>', 'Não informado') as classe";
		$select[] = "idclasse as idclasse";	
	//}	

	//if (strpos($agrupador, "tipoensino") !== false || !$agrupador){
		$select[] = "tipoensino as tipoensino";	
	//}	
	
	//if (strpos($agrupador, "programa") !== false){
		$select[] = "programa as programa";
		$select[] = "idprograma as idprograma";	
	//}	

	//if (strpos($agrupador, "ano") !== false){
		$select[] = "ano";
	//}	
	
	//if (strpos($agrupador, "portaria") !== false){
		$select[] = "'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria";
	//}	
			
	$sql = "select
				COALESCE(sum( projetado ),0) as projetado, 
				COALESCE(sum( autorizado ),0) as autorizado,  
				COALESCE(sum( provimentoautorizado ),0) as provimento, 
				COALESCE(sum( publicado ),0) as publicado, 
				COALESCE(sum( homologado ),0) as homologado,
				COALESCE(sum(lepvlrprovefetivados),0) as lepvlrprovefetivados,
				COALESCE((sum( provimentoautorizado ) - sum(lepvlrprovefetivados)),0) as provimentosnaoefetivados,
				--(sum( homologado ) - (sum( provimentoautorizado ) - sum(lepvlrprovefetivados)) ) AS provimentopendencia,
				COALESCE((COALESCE(sum( homologado ), 0) - COALESCE(sum(provimentoautorizado), 0)),0) AS provimentopendencia,
				" . (implode(" , ", $select)) . "
			from
			(
			
				
				SELECT 
				p.prtid as portaria,
				p.prtnumero as numeroportaria,
				p.prtano as ano, 
				0 as projetado, 
				0 as autorizado, 
				0 as provimentoautorizado,
				sum(lepvlrpublicacao) as publicado,
				sum( lepvlrhomologado ) as homologado,
				sum(lepvlrprovefetivados) as lepvlrprovefetivados,
				COALESCE(ent.entnome , 'Não informado') as unidade ,
				ent.entid as idunidade, 
				COALESCE(cp.entnome, 'Não informado') as campus ,
				cp.entid as idcampus, 
				COALESCE(cl.clsdsc, 'Não informado') as classe ,
				cl.clsid as idclasse, 
				ao.orgdesc as tipoensino , 
				ao.orgid,
				pp.prgdsc as programa,
				p.prgid as idprograma 
				FROM 
					academico.portarias p 
				inner join academico.editalportaria ep ON ep.prtid = p.prtid 
				inner join academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
																	 AND lep.lepstatus = 'A'		
				INNER JOIN entidade.entidade cp ON cp.entid = ep.entidcampus AND cp.entstatus = 'A' 
				INNER JOIN entidade.entidade ent ON ent.entid = ep.entidentidade AND ent.entstatus = 'A'
				inner join academico.cargos c ON c.crgid = lep.crgid
				inner join academico.classes cl ON cl.clsid = c.clsid
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid 
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				where 
					p.tprid = " . ACA_TPORTARIA_CONCURSO . " 
					AND p.prtstatus = 'A' 
					AND ep.edpstatus = 'A' 
					AND ep.tpeid in ( " . ACA_TPEDITAL_PUBLICACAO . ", " . ACA_TPEDITAL_HOMOLOGACAO . ", " . ACA_TPEDITAL_NOMEACAO . " )					
				group by 
					p.prtid ,
					p.prtnumero,
					p.prtano, 
					COALESCE(ent.entnome , 'Não informado'),
					ent.entid, 
					COALESCE(cp.entnome, 'Não informado'),
					cp.entid, 
					COALESCE(cl.clsdsc, 'Não informado') ,
					cl.clsid, 
					ao.orgdesc , 
					ao.orgid,
					pp.prgdsc,
					p.prgid 
			
			
			
			union all
			
			
				SELECT 
					p.prtid as portaria,
					p.prtnumero as numeroportaria, 
					p.prtano as ano, 
					ap.acpvalor as projetado, 
					lp.lnpvalor as autorizado, 
					0 as provimentoautorizado,
					0 as publicado ,
					0 as homologado	,
					0 as lepvlrprovefetivados,
					COALESCE(ent.entnome , 'Não informado') as unidade ,
					ent.entid as idunidade, 
					COALESCE(c.entnome, 'Não informado') as campus ,
					c.entid as idcampus, 
					COALESCE(cl.clsdsc, 'Não informado') as classe ,
					cl.clsid as idclasse,  
					ao.orgdesc as tipoensino ,
					ao.orgid, 
					pp.prgdsc as programa,
					p.prgid as idprograma 
				FROM 
					academico.portarias p 
				INNER JOIN 
					( SELECT 
						prtid, entidcampus, entidentidade, clsid, sum(lnpvalor) as lnpvalor 
					  FROM 
						academico.lancamentosportaria a 
					  WHERE
					  	a.lnpstatus = 'A'							
					  GROUP BY 
						prtid, entidcampus, entidentidade, clsid ) lp ON lp.prtid = p.prtid 
																		 --and lp.lnpano = p.prtano
				INNER JOIN ( SELECT 
								prtid, entidcampus, entidentidade, clsid, sum(acpvalor) as acpvalor 
							  FROM 
								academico.acumuladoprojetado a 
							  GROUP BY 
								prtid, entidcampus, entidentidade, clsid ) ap ON ap.prtid = p.prtid
				

				INNER JOIN entidade.entidade ent ON ent.entid = lp.entidentidade and ent.entid = ap.entidentidade AND ent.entstatus = 'A'
				INNER JOIN entidade.entidade c ON c.entid = lp.entidcampus and c.entid = ap.entidcampus AND c.entstatus = 'A' 
			--	INNER JOIN entidade.funcaoentidade fen ON fen.entid = c.entid
				INNER JOIN ( select entid, min(fueid) as fueid from entidade.funcaoentidade where ( funid = 17 or funid = 18 ) group by entid ) fen ON fen.entid = c.entid 
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid and fea.entid = ent.entid		
											  
				INNER JOIN academico.classes cl ON cl.clsid = lp.clsid and cl.clsid = ap.clsid AND cl.clsstatus = 'A' 
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid 
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				where 
					p.tprid = " . ACA_TPORTARIA_CONCURSO . "
					AND p.prtstatus = 'A'
			
			
			union all
			
				SELECT 
					p2.prtid as portaria, 
					p2.prtnumero as numeroportaria,
					p2.prtano as ano, 
					0 as projetado, 
					0 as autorizado, 
					lp.lnpvalor as provimentoautorizado,
					0 as publicado ,
					0 as homologado	,
					0 as lepvlrprovefetivados,
					COALESCE(ent.entnome , 'Não informado') as unidade ,
					ent.entid as idunidade, 
					COALESCE(c.entnome, 'Não informado') as campus ,
					c.entid as idcampus, 
					COALESCE(cl.clsdsc, 'Não informado') as classe ,
					cl.clsid as idclasse,  
					ao.orgdesc as tipoensino , 
					ao.orgid,
					pp.prgdsc as programa,
					p.prgid as idprograma 
				FROM 
					academico.portarias p 
				inner join academico.portarias p2 ON p.prtidautprov = p2.prtid 
													 AND p2.prtstatus = 'A' 
				INNER JOIN 
					( SELECT 
						prtid, entidcampus, entidentidade, clsid, sum(lnpvalor) as lnpvalor 
					  FROM 
						academico.lancamentosportaria a 
					  WHERE
					  	a.lnpstatus = 'A'
					  GROUP BY 
						prtid, entidcampus, entidentidade, clsid ) lp ON lp.prtid = p.prtid --and lp.lnpano = p.prtano
				inner JOIN entidade.entidade ent ON ent.entid = lp.entidentidade AND ent.entstatus = 'A'
				inner JOIN entidade.entidade c ON c.entid = lp.entidcampus AND c.entstatus = 'A' 
				INNER JOIN academico.classes cl ON cl.clsid = lp.clsid AND cl.clsstatus = 'A' 
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid 
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				WHERE
					p.tprid = " . ACA_TPORTARIA_PROVIMENTO . "
					AND p.prtstatus = 'A'
			) as foo 
			" . (is_array($where) ? " WHERE ".implode(' AND ', $where) : '') . "
			group by 
				portaria,
				numeroportaria,
				unidade,
				idunidade,
				campus,
				idcampus,
				classe,
				idclasse,
				tipoensino,
				orgid,
				programa,
				idprograma,
				ano
			order by 
				" . (!empty($agrupador) ? $agrupador : "portaria");
//	dbg($sql,1);
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
											"projetado",	
									   		"autorizado", 
 									   		"provimento",
 									   		"publicado",
 									   		"homologado",
											"lepvlrprovefetivados",
											"provimentosnaoefetivados",
											"provimentopendencia",
										    "homocolor"
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'tipoensino':
				array_push($agp['agrupador'], array(
													"campo" => "tipoensino",
											  		"label" => "Tipo Ensino")										
									   				);				
		    	continue;
		        break;
		    case 'portaria':
				array_push($agp['agrupador'], array(
													"campo" => "portaria",
											  		"label" => "Portaria")										
									   				);					
		    	continue;
		        break;		    	
		    case 'campus':
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											 		"label" => "Campus")										
									   				);					
		    	continue;			
		        break;	
		    case 'classe':
				array_push($agp['agrupador'], array(
												"campo" => "classe",
												"label" => "Classe <span style=\"font-weight: normal; color: red;\">(Clique  na classe para detalhamento das informações)</span>")										
										   		);	
				continue;
				break;	    	
		    case 'programa':
				array_push($agp['agrupador'], array(
												"campo" => "programa",
												"label" => "Programa")										
										   		);	
				continue;
				break;	    	
		    case 'unidade':
				array_push($agp['agrupador'], array(
												"campo" => "unidade",
												"label" => "Unidade")										
										   		);	
				continue;
				break;					
		    case 'ano':
				array_push($agp['agrupador'], array(
												"campo" => "ano",
												"label" => "Ano")										
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
							  "campo" => "projetado",
					   		  "label" => "Projetado <br/>(A)",
							  "type"  => "numeric"
						),
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
						array(
							  "campo" => "autorizado",
					   		  "label" => "Concursos <br/>Autorizados <br/>(B)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "publicado",
					   		  "label" => "Concursos <br/>Publicados <br/>(C)",
							  "type"  => "numeric"
						)
					);
					
	if ( $permissao ){				
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

	}			

	return $coluna;			  	
}
?>
</body>