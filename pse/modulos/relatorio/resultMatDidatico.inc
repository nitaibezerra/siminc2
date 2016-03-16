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
$r->setTolizadorLinha(true);

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
		$where[0] = " AND e.empano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[] = " AND m.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $recebeu[0] && ( $recebeu_campo_flag ||$recebeu_campo_flag == '1' )){
		$where[] = " AND i.iteid " . (( $recebeu_campo_excludente == null || $recebeu_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $recebeu ) . "') ";		
	}
	if( $item[0] != '' && ( $item_campo_flag ||$item_campo_flag == '1' )){
		$where[] = " AND k.kitid " . (( $item_campo_excludente == null || $item_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $item ) . "') ";		
	}
	if( $portaria[0] && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[] = " AND port.porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[] = " AND e.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}

	$sql = "SELECT 
					distinct(port.pormunicipio),
					case when e.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera,
					e.empid,
					e.empano,
					k.kitid as coditem,
					case when k.kitdescricao <> '' then k.kitdescricao else 'Não recebeu o material' end as material,
					s.skcquantescola as escolas,
					s.skcquantesf as esf,
					m.estuf,
					m.mundescricao as municipio,
					case when kitflagpedcli = 'p' then 'Material Didático' else 'Material Clínico' end as tipo,
					--i.iteid,
					port.porano
				FROM
					pse.portariapse port
				INNER JOIN pse.estadomunicipiopse e ON e.muncod    = port.pormunicipio
				INNER JOIN pse.resposta r 	    	ON r.empid     = e.empid
				INNER JOIN pse.itemselecionado i    ON i.resid     = r.resid
				INNER JOIN territorios.municipio m  ON m.muncod    = e.muncod
				--INNER JOIN entidade.endereco ende   ON ende.muncod = m.muncod
				--INNER JOIN pse.escolapse ep         ON ep.entid    = ende.entid
				LEFT  JOIN pse.secrekitcompleto s   ON e.empid     = s.empid
				LEFT  JOIN pse.kitpedagoclinico k   ON k.kitid     = s.kitid
				WHERE
					r.perid = 19
					AND m.estuf <> ''
					".$where[0]."
					".$where[1]."
					".$where[2]."
					".$where[3]."
					".$where[4]."
					".$where[5]."
					".$where[6]."
				ORDER BY
					m.estuf,
					m.mundescricao,
					k.kitid";
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
			$vari = "PSE - Programa Saúde na Escola - Relatório Material Didático-Pedagógico e Clínico- Secretarias Municipais<br>";
			$i++;
		} elseif( $valor == 'e' ){
			$vari = "PSE - Programa Saúde na Escola - Relatório Material Didático-Pedagógico e Clínico- Secretarias Estaduais<br>";
			$i++;
		}
	}
	
	if( $i > 1 || $i == 0 ){
		$vari = "PSE - Programa Saúde na Escola - Relatório Material Didático-Pedagógico e Clínico- Secretarias Estaduais e Municipais<br>";	
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
		    case 'material':
				array_push($agp['agrupador'], array(
													"campo" => "tipo",
											 		"label" => "Tipo")										
									   				);					
				array_push($agp['agrupador'], array(
													"campo" => "material",
											 		"label" => "$var Material Didático")										
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
							  "campo" => "escolas",
					   		  "label" => "Escolas",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "esf",
					   		  "label" => "Esf",
							  "type"  => "numeric"
						)
					);

	return $coluna;			  	
	
}
?>