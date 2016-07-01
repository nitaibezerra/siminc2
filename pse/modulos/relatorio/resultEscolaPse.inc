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

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);

//$r->setBrasao(true);
echo $r->getRelatorio();
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
//	if ($f_tipoensino){
//		$where[] = " orgid = '$f_tipoensino'";
//	}
	
//	if (!empty($_SESSION["academico"]["orgid"])){
//		$funid = $_SESSION["academico"]["orgid"] == 1 ? 18 : 17;	
//	}
	/*
	$equipe = !is_array($equipe) ? explode(',', $equipe) : $equipe;
	$estado = !is_array($estado)   ? explode(',', $estado)   : $estado;
	$municipio = !is_array($municipio)    ? explode(',', $municipio)    : $municipio;
	$programa  = !is_array($programa)  ? explode(',', $programa)  : $programa;
	$classe    = !is_array($classe)    ? explode(',', $classe)    : $classe;
*/
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $modalidade[0] && ($modalidade_campo_flag || $modalidade_campo_flag == '1' )){
		$where[2] = " AND ano.moeid " . (( $modalidade_campo_excludente == null || $modalidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $modalidade ) . "') ";		
	}
	if( $nivel[0] && ( $nivel_campo_flag ||$nivel_campo_flag == '1' )){
		$where[3] = " AND ano.nieid " . (( $nivel_campo_excludente == null || $nivel_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $nivel ) . "') ";		
	}
	if( $ano[0] && ( $ano_campo_flag ||$ano_campo_flag == '1' )){
		$where[4] = " AND ano.paeanoreferencia " . (( $ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";		
	}
	if( $pse[0] && ( $pse_campo_flag ||$pse_campo_flag == '1' )){
		$where[5] = " AND es.espparticipapse2009 " . (( $pse_campo_excludente == null || $pse_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pse ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[6] = " AND p.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $portaria[0] != '' && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[7] = " AND pp.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[8] = " AND es.espano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}	
//	
//	if( $unidade[0] && ( $unidade_campo_flag || $unidade_campo_flag == '1' )){
//		$where[] = " idunidade " . (( $unidade_campo_excludente == null || $unidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unidade ) . "') ";
//	}elseif ($_SESSION['academico']['entid'] && $filtroSession && $filtroSession == 'unidade'){
//		$where[] = " idunidade = " . $_SESSION['academico']['entid'];
//	}
//		
//	if( $campus[0] && ( $campus_campo_flag || $campus_campo_flag == '1' )){
//		$where[] = " idcampus " . (($campus_campo_excludente == null || $campus_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $campus ) . "') ";
//	}elseif ($_SESSION['academico']['entidcampus'] && $filtroSession && $filtroSession != 'unidade'){
//		$where[] = " idcampus = " . $_SESSION['academico']['entidcampus'];
//	}
//	
//	if( $programa[0] && ( $programa_campo_flag || $programa_campo_flag == '1' )){
//		$where[] = " idprograma " . (( $programa_campo_excludente == null || $programa_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $programa ) . "') ";
//	}
//	
//	if( $classe[0] && ( $classe_campo_flag || $classe_campo_flag == '1' )){
//		$where[] = " idclasse " . (( $classe_campo_excludente == null || $classe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $classe ) . "') ";
//	}
//	
//	$agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador; 
	
	//if (strpos($agrupador, "unidade") !== false){
//		$select[] = "COALESCE(unidade , 'Não informado') as unidade";
//		$select[] = "idunidade as idunidade";		
//	//}
//
//	//if (strpos($agrupador, "campus") !== false){
//		$select[] = "COALESCE(campus, 'Não informado') as campus";
//		$select[] = "idcampus as idcampus";	
//	//}	
//
//	//if (strpos($agrupador, "classe") !== false){
//		$select[] = "COALESCE(classe, 'Não informado') as classe";
//		$select[] = "idclasse as idclasse";	
//	//}	
//
//	//if (strpos($agrupador, "tipoensino") !== false || !$agrupador){
//		$select[] = "tipoensino as tipoensino";	
//	//}	
//	
//	//if (strpos($agrupador, "programa") !== false){
//		$select[] = "programa as programa";
//		$select[] = "idprograma as idprograma";	
//	//}	
//
//	//if (strpos($agrupador, "ano") !== false){
//		$select[] = "ano";
//	//}	
//	
//	//if (strpos($agrupador, "portaria") !== false){
//		$select[] = "'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria";
//	//}	
	
	$sql = "SELECT	
							e.entcodent || ' - ' || e.entnome AS entnome, 
							ende.estuf as estuf,
							mod.moedsc as moddescricao,
							niv.niedsc as nivdescricao,
							COALESCE(ano.paequantprevista, 0) as qnt1,
							COALESCE(ano.paequantatendida,0) as qnt2,
							0 as percent,
							ano.paeanoreferencia as ano,
							m.mundescricao,
							case when p.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera
					FROM
						pse.equipesaudefamilia esf
					INNER JOIN pse.scnes sc ON esf.cneid = sc.cneid
					INNER JOIN pse.escolapse es ON esf.espid=es.espid
					INNER JOIN entidade.entidade e ON es.entid = e.entid
					INNER JOIN entidade.endereco ende ON es.entid = ende.entid
					INNER JOIN pse.estadomunicipiopse p ON p.muncod = ende.muncod
					INNER JOIN pse.pseanoescola ano ON ano.esfid = esf.esfid
					INNER JOIN pse.modalidadeensino mod ON ano.moeid = mod.moeid
					INNER JOIN pse.nivelensino niv ON niv.nieid = ano.nieid
					INNER JOIN territorios.municipio m ON m.muncod = ende.muncod
					INNER JOIN pse.portariapse pp ON pp.pormunicipio = m.muncod
					RIGHT JOIN pse.usuarioresponsabilidade rsp ON rsp.entid = e.entid
					WHERE
						rsp.pflcod in (320,321) AND rsp.rpustatus = 'A'
						".$where[0]."
						".$where[1]."
						".$where[2]."
						".$where[3]."
						".$where[4]."
						".$where[5]."
						".$where[6]."
						".$where[7]."
						".$where[8]."
					ORDER BY entnome, ano.paeanoreferencia";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"qnt1",
											"qnt2",
											"percent"
										  )	  
				);
	
	$count = 1;
		$i = 0;
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório Escola PSE- Escolas Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório Escola PSE- Escolas Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório Escola PSE- Escolas Estaduais e Municipais<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "$var Município")										
									   				);					
		    	continue;
		    break;  
		    case 'modalidade':
				array_push($agp['agrupador'], array(
													"campo" => "moddescricao",
											  		"label" => "$var Modalidade de Ensino")										
									   				);					
		    	continue;
		    break;		    	
		    case 'nivel':
				array_push($agp['agrupador'], array(
													"campo" => "nivdescricao",
											 		"label" => "$var Nível de Ensino")										
									   				);					
		    	continue;			
		    break;
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "entnome",
											 		"label" => "$var INEP - Escola")										
									   				);					
		   		continue;			
		    break;
		    case 'ano':
				array_push($agp['agrupador'], array(
													"campo" => "ano",
											 		"label" => "$var Ano")										
									   				);					
		   		continue;			
		    break;
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
						array(
							  "campo" => "qnt1",
					   		  "label" => "Quantidade Prevista",
							  "type"  => "numeric"
						),
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
						array(
							  "campo" => "qnt2",
					   		  "label" => "Quantidade Atendida",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "percent",
							  "type"  => "numeric",
					   		  "label" => "Percentual <br>de Defasagem (%)",
							  "php"   => array("expressao" => "({qnt1} == 0) || ((100 -(({qnt2}/{qnt1})*100))<0)",
											   "var" => "percentual",
											   "true" => "0",
							  					"type"  => "numeric",
											   "false" => "round((100 -(({qnt2}/{qnt1})*100)),2)",
											   "html" => "{percentual}%")
						)
					);
		/*			
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

	}		*/	

	return $coluna;			  	
	
}
?>
