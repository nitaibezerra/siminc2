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
	//ver ($_POST);
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[0] = " AND ep.espano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[1] = " AND est.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[2] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[3] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $material[0] && ( $material_campo_flag || $material_campo_flag == '1' )){
		$where[4] = " AND ip.iulflagpedcli " . (( $material_campo_excludente == null || $material_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $material ) . "') ";		
	}
	$sql = "SELECT DISTINCT
		        ende.estuf,
		        mun.mundescricao || ' - ' || CASE WHEN est.empflagestmun = 'm' THEN 'Municipal' ELSE 'Estadual' END as municipio,
		        ep.espid,
		        ent.entnome as escola,
		        rr.pulid,
		        CASE WHEN ip.iulflagpedcli = 'p' THEN 'Pedagógico' ELSE 'Clínico' END as tipomaterial,
		        ip.iuldescricao as material,
		        tt.tulquantescola as escolas,
		        tt.tulquantesf as esf,
		        ep.espano,
		        est.empflagestmun
			FROM
			        pse.iulitempergunta ip
			INNER JOIN
			        pse.isuitemselecionado uis ON uis.iulid = ip.iulid
			LEFT JOIN
			        pse.tultextoitemselecionado tt ON tt.isuid = uis.isuid
			INNER JOIN
			        pse.rulresposta rr ON rr.rulid = uis.rulid
			INNER JOIN
			        pse.escolapse ep ON ep.espid = rr.espid
			INNER JOIN
			        entidade.entidade ent ON ent.entid = ep.entid
			INNER JOIN
			        entidade.endereco ende ON ende.entid = ent.entid
			INNER JOIN
				territorios.municipio mun ON mun.muncod = ende.muncod
			LEFT JOIN
				pse.escolamunpse2009 em ON em.entid = ep.entid
			LEFT JOIN
				pse.estadomunicipiopse est ON est.empid = em.empid AND est.empano = ep.espano
			WHERE
			        rr.pulid IN ( 11 )
			        AND ip.iulid NOT IN (300)
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
				".$where[4]."
			ORDER BY
			       	ende.estuf,
			        municipio,
			        ep.espid,
			        ent.entnome,
			        rr.pulid,
			        tipomaterial,
			        ip.iuldescricao";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"escolas",
											"esf"
										  )	  
				);
	
	$count = 1;
		$i = 0;

		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório de Materiais- Escolas Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório de Materiais- Escolas Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório de Materiais- Escolas Estaduais e Municipais<br>";	
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
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											 		"label" => "$var Escola")										
									   				);					
		   		continue;			
		    break;	
		    case 'material':
				array_push($agp['agrupador'], array(
													"campo" => "tipomaterial",
											 		"label" => "$var Tipo do Material")										
									   				);					
				array_push($agp['agrupador'], array(
													"campo" => "material",
											 		"label" => "$var Material")										
									   				);					
		    	continue;			
		    break;	
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	if( $_POST['local'] == 1 ){
		$coluna    = array(
							array(
								  "campo" => "escolas",
						   		  "label" => "Escolas",
								  "type"  => "numeric"
							)
					);
		
	} elseif( $_POST['local'] == 2 ){
		$coluna    = array(
							array(
								  "campo" => "esf",
						   		  "label" => "ESF",
								  "type"  => "numeric"
							)
					);
	} else {
		$coluna    = array(
						array(
							  "campo" => "escolas",
					   		  "label" => "Escolas",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "esf",
					   		  "label" => "ESF",
							  "type"  => "numeric"
						)
					);
	}
	return $coluna;		
}
?>
