<?
if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","1024M");
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

echo $r->getRelatorio();
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;

	extract($_POST);
	
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[0] = " AND e.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[1] = " AND ep.espano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	if( $portaria[0] && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[2] = " AND port.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[3] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[4] = " AND ende.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $comp1[0] && ( $comp1_campo_flag ||$comp1_campo_flag == '1' )){
		$where[5] = " OR item.iulid " . (( $comp1_campo_excludente == null || $comp1_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $comp1 ) . "') ";		
	}
	if( $comp2[0] && ( $comp2_campo_flag ||$comp2_campo_flag == '1' )){
		$where[6] = " OR item.iulid " . (( $comp2_campo_excludente == null || $comp2_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $comp2 ) . "') ";		
	}
	if( $comp3[0] && ( $comp3_campo_flag ||$comp3_campo_flag == '1' )){
		$where[7] = " OR item.iulid " . (( $comp3_campo_excludente == null || $comp3_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $comp3 ) . "') ";		
	}
	if($where[5] || $where[6] || $where[7]){
		$va = '=';
	} else {
		$va = '=';
	}
	
	$sql = "SELECT
				ende.estuf,
				muni.mundescricao as municipio,
				port.porano as portaria,
				ep.espano as exerc,
				count(distinct ep.espid) as quantidade,
				case 	
					when perg.pulid IN (12,13,14,15) then 'Componente I' 
					when perg.pulid IN (19,20,22,23) then 'Componente II' 
					when perg.pulid = 24 		 then 'Componente III'  
				end as componente,
				item.iulid,
				perg.iuldescricao as descricao
			
			FROM
				pse.escolapse ep
			
			INNER JOIN
				pse.rulresposta resp ON ep.espid = resp.espid
			INNER JOIN 
				pse.isuitemselecionado item ON item.rulid = resp.rulid
			INNER JOIN
				pse.iulitempergunta perg ON resp.pulid = perg.pulid
			INNER JOIN
				entidade.endereco ende ON ende.entid = ep.entid
			INNER JOIN
				pse.estadomunicipiopse e ON e.muncod = ende.muncod
			INNER JOIN
				territorios.municipio muni ON muni.muncod = ende.muncod
			INNER JOIN
				pse.portariapse port ON port.pormunicipio = ende.muncod
			
			WHERE
			 perg.pulid IN (12,13,14,15,19,20,22,23,24)
			 AND perg.iulid = item.iulid
			 ".$where[0]."
			 ".$where[1]."
			 ".$where[2]."
			 ".$where[3]."
			 ".$where[4]."
			 AND (perg.iulid ".$va." 0 ".$where[5].$where[6].$where[7]." )
			 GROUP BY
				ende.estuf,
				muni.mundescricao,
				port.porano,
				perg.pulid,
				item.iulid,
				descricao,
				ep.espano
				
			ORDER BY ende.estuf, exerc
			";
//	ver ($sql);
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("quantidade")	  
				);
	

		

			$vari = "PSE - Programa Saúde na Escola - Relatório Componentes PSE- ULI<br>";	

			foreach ($agrupador as $val):
			$var = $vari;
			
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
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
		    case 'exerc':
				array_push($agp['agrupador'], array(
													"campo" => "exerc",
											  		"label" => "Exercicio")										
									   				);					
		    	continue;
		    break;		    	
		    case 'componente':
		    	array_push($agp['agrupador'], array(
													"campo" => "componente",
											 		"label" => "Componente")										
									   				);
				array_push($agp['agrupador'], array(
													"campo" => "descricao",
											 		"label" => "Ação"
													)										
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
					   		  "label" => "Quantidade de Escolas<br> que Executaram a Ação",
							  "type"  => "numeric",
							  "blockAgp" =>  "componente",
							  "mostraNivel" => array("descricao", "estado"),
							  "html"  => "<center>{quantidade}</center>"
						) 		
					);
	return $coluna;			  	
	
}
?>