<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

include_once "LerParametrosFiltroConsulta.php";
include_once "resultSession.php";

ini_set("memory_limit","500M");
set_time_limit(0);

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

	$parametros = new ParametroFiltroConsulta($_POST);	
	$parametros->addCondicaoWhereConformeFiltro(" AND estado.estuf " 		   			 , "estado" 	);   
	$parametros->addCondicaoWhereConformeFiltro(" AND municipio.muncod   " 	   			 , "municipio"  ); 
	$parametros->addCondicaoWhereConformeFiltro(" AND pseanoescolamun.moeid  " 			 , "modalidade" ); 
	$parametros->addCondicaoWhereConformeFiltro(" AND pseanoescolamun.nieid  "  		 , "nivel" 	  	); 
	$parametros->addCondicaoWhereConformeFiltro(" AND pseanoescolamun.pamanoreferencia " , "ano" 		);
	$parametros->addCondicaoWhereConformeFiltro(" AND p.empflagestmun " 				 , "esfera" 	);
	$parametros->addCondicaoWhereConformeFiltro(" AND p.empano " 						 , "exer" 		);  
	$arraySubstituicao = array( "2008" => "t" ,
							    "2009" => "f"
							  );
	$parametros->addCondicoesWhereConformeArray(" AND escolapse.espparticipapse2009 ", "anoportaria", $arraySubstituicao); 	
	
	$sql = "
			SELECT	distinct 
				estado.estuf as estuf,
				case when p.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera, 
				municipio.mundescricao ,
				pseanoescolamun.pamanoreferencia as ano, 	
				entidade.entcodent|| ' - ' ||entidade.entnome as entnome,
				COALESCE(pseanoescolamun.pamquantprevista,0) as qnt1, 
				COALESCE(pseanoescolamun.pamquantatendida,0) as qnt2,
				modalidadeensino.moedsc as moddescricao, 
				nivelensino.niedsc as nivdescricao, 	
				0 as percent
			
			FROM
				territorios.regiao 
			
				inner join territorios.estado 
				on regiao.regcod = estado.regcod
			
				inner join territorios.municipio
				on estado.estuf = municipio.estuf
			
				inner join pse.estadomunicipiopse p
				on municipio.muncod = p.muncod
			
				inner join pse.pseanoescolamun
				on p.empid = pseanoescolamun.empid
			
					inner join entidade.entidade entidade
					on pseanoescolamun.entid = entidade.entid
			
					left join pse.escolapse
					on entidade.entid = escolapse.entid
			
				inner join pse.modalidadeensino 
				on pseanoescolamun.moeid = modalidadeensino.moeid
				
				inner join pse.nivelensino 
				on pseanoescolamun.nieid = nivelensino.nieid
			WHERE
				1=1
				".$parametros->getCondicoesWhere()."
			ORDER BY 
				estado.estuf, 
				municipio.mundescricao ,
				pseanoescolamun.pamanoreferencia , 	
				entnome
		";
	
//	ver ($sql);
//	die();
	
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
				$vari = "PSE - Programa Saúde na Escola - Relatório Escola PSE- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório Escola PSE- Secretarias Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório Escola PSE- Secretarias Estaduais e Municipais<br>";	
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
	return $coluna;			  	
}
?>
