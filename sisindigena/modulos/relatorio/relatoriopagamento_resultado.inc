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
$r->setMonstrarTolizadorNivel(true);
$r->setTotNivel(true);
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

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"nome",
												"valor",
												"nucleo",
												"situacao",
												"perfil",
												"parcela",
												"universidade",
												"esfera",
												"municipio",
												"estado",
												"datatramitacao"
												
											  )	  
					);
	if($agrupador):				
		foreach ($agrupador as $val): 
			switch ($val) :		
			    case 'universidade':
					array_push($agp['agrupador'], array(
														"campo" => "universidade",
												 		"label" => "Universidade")										
										   				);					
			    	continue;			
			        break;
		        case 'nucleo':
		        	array_push($agp['agrupador'], array(
											        	"campo" => "nucleo",
											        	"label" => "Núcleo")
											        	);
		        	continue;
		        	break;
			    case 'perfil':
					array_push($agp['agrupador'], array(
													"campo" => "perfil",
													"label" => "Perfil")										
											   		);	
					continue;
					break;
			    case 'esfera':
					array_push($agp['agrupador'], array(
													"campo" => "esfera",
													"label" => "Esfera")										
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
			    case 'estado':
					array_push($agp['agrupador'], array(
													"campo" => "estado",
													"label" => "UF")										
											   		);	
					continue;
					break;
				
			endswitch;
		endforeach;
	endif;
	
	array_push($agp['agrupador'], array(
									"campo" => "parcela",
									"label" => "Parcela")										
							   		);	
	

	array_push($agp['agrupador'], array( "campo" => "nome",
										 "label" => "Nome")										
										);	
	
	return $agp;
}


function monta_coluna(){
	$coluna = array();
	
	array_push($coluna,array( "campo" => "parcela",
					   		  "label" => "Parcela" ) );					
	
	
	array_push($coluna,array( "campo" => "valor",
					   		  "label" => "Valor R$" ) );					
	
	array_push($coluna,array( "campo" => "situacao",
					   		  "label" => "Situação Pagamento" ) );
	
	array_push($coluna,array( "campo" => "datatramitacao",
					   		  "label" => "Data Tramitação" ) );
	

	return $coluna;			  	
}
?>