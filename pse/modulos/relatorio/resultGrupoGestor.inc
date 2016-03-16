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
$r->setMonstrarTolizadorNivel(true);
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
		$where[0] = " AND m.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $gge[0] && ( $gge_campo_flag ||$gge_campo_flag == '1' )){
		$where[2] = " AND (select resflagresposta from pse.resposta where perid = 2 and empid = r.empid ) " . (( $gge_campo_excludente == null || $gge_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $gge ) . "') ";		
	}
	if( $ggm[0] != '' && ( $ggm_campo_flag ||$ggm_campo_flag == '1' )){
		$where[3] = " AND (select resflagresposta from pse.resposta where perid = 3 and empid = r.empid ) " . (( $ggm_campo_excludente == null || $ggm_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ggm ) . "') ";		
	}
	if( $portaria[0] && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[4] = " AND port.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[5] = " AND e.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[6] = " AND e.empano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}

	
	$sql = "SELECT
					distinct(port.pormunicipio),
					case when e.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera,
					m.estuf,
					e.empano,
					m.mundescricao as municipio,
					r.empid,
					(select case when resflagresposta = 's' then 'Sim' else 'Não' end from pse.resposta where perid = 2 and empid = r.empid ) as gge,
					(select case when resflagresposta = 's' then 'Sim' else 'Não' end from pse.resposta where perid = 3 and empid = r.empid ) as ggm 
				FROM 
					pse.portariapse port
				INNER JOIN pse.estadomunicipiopse e ON e.muncod = port.pormunicipio
				INNER JOIN pse.resposta r ON e.empid = r.empid
				INNER JOIN territorios.municipio m ON m.muncod = e.muncod
				WHERE
					m.estuf <> '' AND
					 (select resflagresposta from pse.resposta where perid = 2 and empid = r.empid) <> '' AND
					 (select resflagresposta from pse.resposta where perid = 3 and empid = r.empid) <> ''
					".$where[0]."
					".$where[1]."
					".$where[2]."
					".$where[3]."
					".$where[4]."
					".$where[5]."
					".$where[6]."
					AND  resflagresposta is not null
				ORDER BY
					m.estuf, m.mundescricao";
//	ver ($sql);			
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"gge",
											"ggm"
											/*"projetado",	
									   		"autorizado", 
 									   		"provimento",
 									   		"publicado",
 									   		"homologado",
											"lepvlrprovefetivados",
											"provimentosnaoefetivados",
											"provimentopendencia",
										    "homocolor"*/
										    
										  )	  
				);
	
	$count = 1;
		$i = 0;
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório SPE/PSE- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório SPE/PSE- Secretarias Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório SPE/PSE- Secretarias Estaduais e Municipais<br>";	
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
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
						array(
							  "campo" => "gge",
					   		  "label" => "GGE",
							  "type"  => "string"
						),
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
						array(
							  "campo" => "ggm",
					   		  "label" => "GGM",
							  "type"  => "string"
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

	}		
*/	
	return $coluna;			  	

}	
?>
