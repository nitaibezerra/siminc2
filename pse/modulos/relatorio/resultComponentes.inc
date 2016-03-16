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
$r->setTotNivel(false);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);

echo $r->getRelatorio();
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;

	extract($_POST);
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND m.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $comp1[0] && ( $comp1_campo_flag ||$comp1_campo_flag == '1' )){
		$where[2] = " OR a.atvid " . (( $comp1_campo_excludente == null || $comp1_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $comp1 ) . "') ";		
	}
	if( $comp2[0] && ( $comp2_campo_flag ||$comp2_campo_flag == '1' )){
		$where[3] = " OR a.atvid " . (( $comp2_campo_excludente == null || $comp2_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $comp2 ) . "') ";		
	}
	if( $comp3[0] && ( $comp3_campo_flag ||$comp3_campo_flag == '1' )){
		$where[4] = " OR a.atvid " . (( $comp3_campo_excludente == null || $comp3_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $comp3 ) . "') ";		
	}
//	if( $comp4[0] && ( $comp4_campo_flag ||$comp4_campo_flag == '1' )){
//		$where[5] = " OR a.atvid " . (( $comp4_campo_excludente == null || $comp4_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $comp4 ) . "') ";		
//	}
	if( $portaria[0] && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[6] = " AND port.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[7] = " AND e.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[8] = " AND e.empano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	if($where[2] || $where[3] || $where[4] || $where[5]){
		$va = '=';
	} else {
		$va = '<>';
	}
	
	$sql = "
			SELECT * FROM (
				SELECT distinct
					a.atvid as pergunta,
					case when e.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera, 
					a.atvdescricao as descricao,
					e.empano as exer,
					q.qtdescola as quantidade,
					m.estuf,
					m.mundescricao as municipio,
					c.copdescricao as tipo,
					case 	
						when a.copid IN (2,3,4) then 'Componente I' 
						when a.copid = 5 		then 'Componente II' 
						when a.copid = 6 		then 'Componente III' 
						when a.copid IN (7,8) 	then 'Componente IV' 
					end as componente
				FROM
					pse.portariapse port ,
					pse.componente c	 ,
					pse.atividade a 	 ,
					pse.quantidadeescola q   ,
					pse.estadomunicipiopse e , 
					territorios.municipio m
					--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
					--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
				WHERE
					port.pormunicipio = e.muncod
					AND
					e.empid = q.empid
					AND
					a.atvid = q.atvid
					AND
					c.copid = a.copid
					AND
					m.muncod = e.muncod
					AND 
					a.copid IN (2,3,4,5,6,7,8)
					".$where[0]."
					".$where[1]."
					".$where[6]."
					".$where[7]."
					".$where[8]."
					AND (a.atvid ".$va." 0 ".$where[2].$where[3].$where[4].$where[5]." )
			
				UNION ALL
			
				SELECT distinct
					a.atvid as pergunta,
					case when e.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera,  
					a.atvdescricao as descricao,
					e.empano as exer,
					0 as quantidade,
					m.estuf,
					m.mundescricao as municipio,
					c.copdescricao as tipo,
					case 	when a.copid IN (2,3,4) then 'Componente I' 
						when a.copid = 5 			then 'Componente II' 
						when a.copid = 6 			then 'Componente III' 
						when a.copid IN (7,8) 		then 'Componente IV' 
					end as componente
				FROM 
					pse.portariapse port ,
					pse.componente c	 ,
					pse.atividade a 	 ,
					pse.estadomunicipiopse e , 
					territorios.municipio m
					--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
					--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
				WHERE
					port.pormunicipio = e.muncod
					AND
					c.copid = a.copid 
					AND
					m.muncod = e.muncod
					AND 
					a.copid IN (2,3,4,5,6,7,8)
					".$where[0]."
					".$where[1]."
					".$where[6]."
					".$where[7]."
					".$where[8]."
					AND (a.atvid ".$va." 0 ".$where[2].$where[3].$where[4].$where[5]." )					

					AND
					a.atvid in (
						SELECT distinct
							a.atvid as pergunta
						FROM
							pse.portariapse port ,
							pse.componente c	 ,
							pse.atividade a 	 ,
							pse.quantidadeescola q   ,
							pse.estadomunicipiopse e , 
							territorios.municipio m
							--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
							--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
						WHERE
							port.pormunicipio = e.muncod
							AND
							c.copid = a.copid 
							AND
							a.atvid = q.atvid  
							AND
							e.empid = q.empid
							AND
							m.muncod = e.muncod
							AND 
							a.copid IN (2,3,4,5,6,7,8)
							".$where[0]."
							".$where[1]."
							".$where[6]."
							".$where[8]."
							AND (a.atvid ".$va." 0 ".$where[2].$where[3].$where[4].$where[5]." )	
						)	
			) AS main
			
			ORDER BY
				estuf, municipio,
				main.pergunta";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("quantidade")	  
				);
	
	$count = 1;
		$i = 0;
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório Componentes PSE- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório Componentes PSE- Secretarias Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório Componentes PSE- secretarias Estaduais e Municipais<br>";	
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
		    case 'componente':
		    	array_push($agp['agrupador'], array(
													"campo" => "componente",
											 		"label" => "$var Componente")										
									   				);	
		    	array_push($agp['agrupador'], array(
													"campo" => "tipo",
											 		"label" => "Subcomponente")										
									   				);
				array_push($agp['agrupador'], array(
													"campo" => "descricao",
											 		"label" => "Ação")										
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
							  "campo" => "quantidade",
					   		  "label" => "Quantitativo de Estudantes <br>Contemplados",
							  "type"  => "numeric"
						) 		
					);
	return $coluna;			  	
	
}
?>