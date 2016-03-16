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
	
	if( $IFES[0] && ( $IFES_campo_flag || $IFES_campo_flag == '1' )){
		$where[0] = " AND ent.entid " . (( $IFES_campo_excludente == null || $IFES_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $IFES ) . "') ";
	}
	if( $indicador[0] && ($indicador_campo_flag || $indicador_campo_flag == '1' )){
		$where[1] = " AND tcu.tcuid " . (( $indicador_campo_excludente == null || $indicador_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $indicador ) . "') ";
	}
	if( $componente[0] && ( $componente_campo_flag ||$componente_campo_flag == '1' )){
		$where[2] = " AND tcu.id " . (( $componente_campo_excludente == null || $componente_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $componente ) . "') ";
	}
	
/*	$sql = "SELECT distinct
					q.atvid as pergunta, 
					a.atvdescricao as descricao,
					q.qtdescola as quantidade,
					ende.estuf,
					m.mundescricao as municipio,
					c.copdescricao as tipo,
					case when a.copid IN (2,3,4) then 'Componente I' when a.copid = 5 then 'Componente II' when a.copid = 6 then 'Componente III' when a.copid IN (7,8) then 'Componente IV' end as componente
				FROM 
					pse.quantidadeescola q
				INNER JOIN pse.atividade a ON q.atvid = a.atvid
				INNER JOIN pse.estadomunicipiopse e ON e.empid = q.empid
				INNER JOIN territorios.municipio m ON m.muncod = e.muncod
				INNER JOIN entidade.endereco ende ON ende.muncod = e.muncod
				INNER JOIN pse.componente c ON a.copid = c.copid
				WHERE
					a.copid IN (2,3,4,5,6,7,8)
					".$where[0]."
					".$where[1]."
					AND (a.atvid ".$va." 0 ".$where[2].$where[3].$where[4].$where[5]." ) 
				ORDER BY
					q.atvid";*/
	
	$sql = "SELECT ent.entid, ent.entsig, tcu.tcucodigo, tcu.tcudsc, tcuindvlr,tcutipo,lci.lcivalor 
			FROM academico.indicadorestcu tcu
			LEFT JOIN
			academico.lancamentoindicador lci ON lci.tcuid = tcu.tcuid
			INNER JOIN
			academico.movimentoindicador mov ON mov.mviid = lci.mviid
			INNER JOIN
			entidade.entidade ent ON ent.entid = mov.entid
			WHERE 0=0 $where[0] $where[1] $where[2]";
	
	return $sql;
	
	
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"quantidade"
										  )	  
				);
	
	$count = 1;
	foreach ($agrupador as $val):
		if($count == 1){
			$var = "Indicadores do TCU<br>";
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'IFES':
				array_push($agp['agrupador'], array(
													"campo" => "entsig",
											  		"label" => "$var IFES")										
									   				);				
		   		continue;
		    break;
		    case 'indicador':
				array_push($agp['agrupador'], array(
													"campo" => "tcucodigo",
											  		"label" => "$var Indicador")										
									   				);					
		    	continue;
		    break;		    	
		  /*  case 'componente':
				array_push($agp['agrupador'], array(
													"campo" => "componente",
											 		"label" => "Componente")										
									   				);	
		    	continue;			
		    break;*/
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
						array(
							  "campo" => "lcivalor",
					   		  "label" => "Valor",
							  "type"  => "numeric"
						)
					);
	return $coluna;			  	
	
}
?>