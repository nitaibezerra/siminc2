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
$r->setTolizadorLinha(false);

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
	

	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[0] = " AND ep.espano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $portaria[0] && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[] = " AND port.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $pse[0] != '' && ( $pse_campo_flag ||$pse_campo_flag == '1' )){
		$where[] = " AND p.empparticipapse2009 " . (( $pse_campo_excludente == null || $pse_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pse ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[] = " AND p.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
		
	$sql = "SELECT
					distinct(m.mundescricao) as municipio, p.empid, ende.estuf, case when p.empparticipapse2009 = '1' then 'Sim' else 'Não' end as participa,
					case when p.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera
				--	case when ano.pamanoreferencia is null then 'Não respondeu'
				--	when ano.pamanoreferencia = 2009 then '2009'
				--	when ano.pamanoreferencia = 2010 then '2010'
				--	when ano.pamanoreferencia = 2011 then '2011' end as ano
			FROM 
					pse.portariapse port
			INNER JOIN pse.estadomunicipiopse p ON p.muncod = port.pormunicipio
			INNER JOIN territorios.municipio m ON m.muncod = p.muncod
			INNER JOIN entidade.endereco ende ON ende.muncod = p.muncod
			INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
			-- LEFT JOIN pse.pseanoescolamun ano ON ano.empid = p.empid
			WHERE
					ende.estuf <> ''
					".$where[0]."
					".$where[1]."
					".$where[2]."
					".$where[3]."
					".$where[4]."
					".$where[5]."
			ORDER BY
					ende.estuf, m.mundescricao";
//	ver ($sql);
	return $sql;
}
function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"participa",
//											"esfera"
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
	foreach( $_POST['esfera'] as $esfera => $valor ){
		if( $valor == 'm' ){
			$vari = "PSE - Programa Saúde na Escola - Relatório Secretarias Municipais<br>";
			$i++;
		} elseif( $valor == 'e' ){
			$vari = "PSE - Programa Saúde na Escola - Relatório Secretarias Estaduais<br>";
			$i++;
		}
	}
	
	if( $i > 1 || $i == 0 ){
		$vari = "PSE - Programa Saúde na Escola - Relatório Secretarias Estaduais e Municipais<br>";	
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
													"campo" => "municipio",
											  		"label" => "$var Município")										
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
//		    case 'tipo':
//				array_push($agp['agrupador'], array(
//													"campo" => "esfera",
//											 		"label" => "$var Esfera")										
//									   				);					
//		    	continue;			
//		    break;
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
						array(
							  "campo" => "participa",
					   		  "label" => "Participou do PSE",
							  "type"  => "string"
						)
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
//						array(
//							  "campo" => "esfera",
//					   		  "label" => "Esfera",
//							  "type"  => "string"
//						)
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
