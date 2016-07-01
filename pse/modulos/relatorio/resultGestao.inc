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

	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND uf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $frequencia[0] && ( $frequencia_campo_flag ||$frequencia_campo_flag == '1' )){
		$where[2] = " AND i.iteid " . (( $frequencia_campo_excludente == null || $frequencia_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $frequencia ) . "') ";		
	}
	if( $dificuldade[0] && ( $dificuldade_campo_flag ||$dificuldade_campo_flag == '1' )){
		$where[3] = " AND i.iteid " . (( $dificuldade_campo_excludente == null || $dificuldade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dificuldade ) . "') ";		
	}
	if( $acomp[0] && ( $acomp_campo_flag ||$acomp_campo_flag == '1' )){
		$where[4] = " AND i.iteid " . (( $acomp_campo_excludente == null || $acomp_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $acomp ) . "') ";		
	}
	if( $acomp2[0] && ( $acomp2_campo_flag ||$acomp2_campo_flag == '1' )){
		$where[5] = " AND i.iteid " . (( $acomp2_campo_excludente == null || $acomp2_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $acomp2 ) . "') ";		
	}
	if( $portaria[0] && ( $portaria_campo_flag ||$portaria_campo_flag == '1' )){
		$where[6] = " AND porano " . (( $portaria_campo_excludente == null || $portaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $portaria ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[7] = " AND exercicio " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	
	$variavel = "".$where[2].",".$where[3].",".$where[4].",".$where[5]."";
	
	$sql = "SELECT

						descricao, res1, res2, res3, res4, uf, muncod, porano, exercicio
					
					FROM (
					
						SELECT
					
						descricao, sum(res1) AS res1, sum(res2) AS res2, sum(res3) AS res3, sum(res4) AS res4, uf, muncod, porano, exercicio
					
						FROM (
					
						    SELECT
							distinct m.mundescricao as descricao, i.iteid as res1, 0 as res2, 0 as res3, 0 as res4, m.estuf as uf, m.muncod as muncod, port.porano as porano, e.empano as exercicio
							
						    FROM
								pse.portariapse port
	    					INNER JOIN territorios.municipio m ON m.muncod = port.pormunicipio
	    					--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
	    					--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
						    INNER JOIN pse.estadomunicipiopse e ON m.muncod = e.muncod
						    INNER JOIN pse.resposta r ON e.empid = r.empid
						    INNER JOIN pse.itemselecionado i ON i.resid = r.resid
						    INNER JOIN pse.itempergunta p ON i.iteid = p.iteid
						    WHERE
							r.perid IN (10)
							".$where[2]."
									
					
						    UNION ALL
					
						    SELECT
							distinct m.mundescricao, 0 as res1, i.iteid as res2, 0 as res3, 0 as res4, m.estuf as uf, m.muncod as muncod, port.porano as porano, e.empano as exercicio
							
						    FROM
								pse.portariapse port
	    					INNER JOIN territorios.municipio m ON m.muncod = port.pormunicipio
	    					--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
	    					--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
						    INNER JOIN pse.estadomunicipiopse e ON m.muncod = e.muncod
						    INNER JOIN pse.resposta r ON e.empid = r.empid
						    INNER JOIN pse.itemselecionado i ON i.resid = r.resid
						    INNER JOIN pse.itempergunta p ON i.iteid = p.iteid
						    WHERE
							r.perid IN (11)
							".$where[3]."                                
					
						    UNION ALL
					
						    SELECT
							distinct m.mundescricao, 0 as res1, 0 as res2, i.iteid as res3, 0 as res4, m.estuf as uf, m.muncod as muncod, port.porano as porano, e.empano as exercicio
							
						    FROM
								pse.portariapse port
	    					INNER JOIN territorios.municipio m ON m.muncod = port.pormunicipio
	    					--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
	    					--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
						    INNER JOIN pse.estadomunicipiopse e ON m.muncod = e.muncod
						    INNER JOIN pse.resposta r ON e.empid = r.empid
						    INNER JOIN pse.itemselecionado i ON i.resid = r.resid
						    INNER JOIN pse.itempergunta p ON i.iteid = p.iteid
						    WHERE
							r.perid IN (12)
							".$where[4]."                      
					
						    UNION ALL
					
						    SELECT
							distinct m.mundescricao, 0 as res1, 0 as res2, 0 as res3, i.iteid as res4, m.estuf as uf, m.muncod as muncod, port.porano as porano, e.empano as exercicio
							
						    FROM
								pse.portariapse port
	    					INNER JOIN territorios.municipio m ON m.muncod = port.pormunicipio
	    					--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
	    					--INNER JOIN pse.escolapse ep ON ep.entid = ende.entid
						    INNER JOIN pse.estadomunicipiopse e ON m.muncod = e.muncod
						    INNER JOIN pse.resposta r ON e.empid = r.empid
						    INNER JOIN pse.itemselecionado i ON i.resid = r.resid
						    INNER JOIN pse.itempergunta p ON i.iteid = p.iteid
						    WHERE
							r.perid IN (13)
							".$where[5]."                      
					
						) as Principal
						GROUP BY descricao, uf, muncod, porano, exercicio
					) as dois
					WHERE res1 > 0
					AND res2 > 0
					AND res3 > 0
					AND res4 > 0
					".$where[0]."
					".$where[1]."
					".$where[6]."
					".$where[7]."
					ORDER BY
						uf, muncod, descricao";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( )	  
				);
	
	$count = 1;
		$i = 0;
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - Programa Saúde na Escola - Modelo de Gestão de Comunicação- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Modelo de Gestão de Comunicação- Secretarias Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Modelo de Gestão de Comunicação- Secretarias Estaduais e Municipais<br>";	
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
													"campo" => "uf",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "descricao",
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
	

	return $coluna;			  	

}	
?>
