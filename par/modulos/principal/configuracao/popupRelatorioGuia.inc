<?php
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);

$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
//$r->setMonstrarTolizadorNivel(true);
//$r->setTotNivel(true);
$r->setBrasao(true);
$r->setEspandir(false);

?>


<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>

	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

		<!--  Monta o Relatório -->
		<? echo $r->getRelatorio(); ?>

	</body>
</html>


<?

function monta_sql(){
	global $db;
	extract($_REQUEST);

	$where = array();
	
	if($itrid) {
		array_push($where, " d.itrid = ".$itrid);
	}

	$sql = "select par.retornacodigopropostasubacao(pps.ppsid)||' - '||pps.ppsdsc as subacao, pic.picdescricao as item 
			from par.propostasubacao pps
			inner join par.propostasubacaoitem ps on ps.ppsid = pps.ppsid 
			inner join par.propostaitemcomposicao pic on pic.picid = ps.picid
			INNER JOIN par.indicador i  ON i.indid  = pps.indid
			INNER JOIN par.area      ar ON ar.areid = i.areid
			INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid
			".(($where)?"where ".implode(" and ",$where):"")."
			order by par.retornacodigopropostasubacao(pps.ppsid), pps.ppsdsc, pic.picdescricao";

	return $sql;
}

function monta_agp(){
	$agp = array(	"agrupador" => array(),
					"agrupadoColuna" => array(
												"subacao",
												"item"
											  )	  
					);

	array_push($agp['agrupador'], array(
										"campo" => "subacao",
								 		"label" => "Subação")										
						   				);					
					
					
	array_push($agp['agrupador'], array(
									"campo" => "item",
									"label" => "Item")										
							   		);	
	

	
	return $agp;
}


function monta_coluna(){
	$coluna = array();

	return $coluna;			  	
}
?>
