<?php
ini_set("memory_limit", "1024M");
$GLOBALS['idprocesso'] = $_REQUEST['idproc'];
// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

global $db;

$sql   = monta_sql2();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(true);
$r->setBrasao(true);
$r->setEspandir(true);

function monta_agp(){
	//$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											
											"uasg",
											"logradouro",
											"item",
											"quantidade",
											"valorminimo",				
											"valormaximo",
											"totalvalorminimo",				
											"totalvalormaximo",
											"siasg",
											"unidademedida"			   		
										  )	  
				);

	array_push($agp['agrupador'], array(
							"campo" => "uasg",
							"label" => "UASG"));
	array_push($agp['agrupador'], array(
									"campo" => "logradouro",
									"label" => "Logradouro")										
							   		);	

	
						   		
	array_push($agp['agrupador'], array(
								"campo" => "item",
								"label" => "Item")										
						   		);
						   		
		   						 
	return $agp;
}


function monta_coluna(){
		
	$coluna = array();
	
/*	array_push($coluna, array( "campo" => "uasg",
							   "label" => "UASG"	
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "logradouro",
							   "label" => "Logradouro"	
										)										
							   		);	*/			
		/*					   					   		
	array_push($coluna, array( "campo" => "item",
							   "label" => "Item"	
										)										
							   		);	*/
	array_push($coluna, array( "campo" => "quantidade",
						       "label" => "Quantidade",	
						       "type"  => "numeric"	
										)										
							   		);	

	array_push($coluna, array( "campo" => "valorminimo",
						       "label" => "Valor Mínimo (R$)",	
						       "type"  => "decimal"	
										)										
							   		);	

	array_push($coluna, array( "campo" => "valormaximo",
						       "label" => "Valor Máximo (R$)",	
						       "type"  => "decimal"	
										)										
							   		);	

	array_push($coluna, array( "campo" => "totalvalorminimo",
						       "label" => "Valor Total Mínimo (R$)",	
						       "type"  => "decimal"	
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "totalvalormaximo",
						       "label" => "Valor Total Máximo (R$)",	
						       "type"  => "decimal"	
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "siasg",
							   "label" => "Código SIASG",
							   "type"  => "string"	
										)										
							   		);	
							   		
	array_push($coluna, array( "campo" => "unidademedida",
							   "label" => "Unidade de medida"	
										)										
							   		);	

	/*array_push($coluna, array("campo" => "item",
							  "label" => "Item"	
										)										
							   		);	

	array_push($coluna, array("campo" => "instituicao",
							  "label" => "Instituição"	
										)										
							   		);	*/

	return $coluna;			  	
}

function monta_sql2(){

		$sql = "select coe.coeid,
		   		usg.usgcod,
				usg.usgdsc || ' UASG: ' || usg.usgcod as UASG,
				coe.coenddsc || ', ' || coe.coendlog || ', ' || coe.coendnum || ', ' || coe.coendcom || ' ' || coe.coendbai || ', CEP: ' || coe.coendcep || ', ' || mun.mundescricao || '-' || coe.estuf as logradouro,
				coi.coidsc as item,
				coi.coicodsiasg as siasg,
				cdi.cdiqtde as quantidade,
				ume.umedescricao as unidademedida,
                coi.coivlrreferenciamin as valorminimo,
				coi.coivlrreferenciamax as valormaximo,
                coi.coivlrreferenciamin*cdi.cdiqtde as totalvalorminimo,
				coi.coivlrreferenciamax*cdi.cdiqtde as totalvalormaximo
				FROM evento.coenderecoentrega coe
				inner join evento.coadesao coa on coa.coaid = coe.coaid
				inner join workflow.documento doc on coa.docid = doc.docid
				inner join evento.codemandaitem cdi on coe.coeid = cdi.coeid
				inner join evento.coitemprocesso cip on cip.cotid = cdi.cotid
				inner join evento.coitem coi on coi.coiid = cip.coiid
				inner join evento.unidademedida ume on coi.umeid = ume.umeid
				inner join evento.uasg usg on coa.usgid = usg.usgid
				inner join territorios.municipio mun on coe.muncod = mun.muncod
				where coa.copid = {$GLOBALS['idprocesso']}
				and doc.esdid = 46
				and cdi.cdiqtde > 0
				group by
				coe.coeid,
				usg.usgcod,
				uasg,
				logradouro,
				item,
				siasg,
				quantidade,
				unidademedida,
                valorminimo,
                valormaximo,
                totalvalorminimo,
                totalvalormaximo			
				order by usgcod
		";
			
	//ver($sql,d);	 
	//dbg($sql,1);
	return $sql;
}
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

<script>
	function chamaproc(prcid)
	{
		//window.opener.location="conjur.php?modulo=principal/editarprocesso&acao=A&prcid="+prcid;
		window.opener.focus();
		
	}
</script>