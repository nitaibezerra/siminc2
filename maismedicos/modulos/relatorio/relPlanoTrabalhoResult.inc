<?php 

ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql();
$dados = $db->carregar($sql);

// Gerar arquivo xls
if( $_POST['req'] == 'geraxls' ){
	
	$arCabecalho = array();
	$colXls = array();
	
	$_POST['agrupador'] = $_POST['agrupador'] ? $_POST['agrupador'] : array();
	foreach ($_POST['agrupador'] as $agrup){
		if($agrup == 'dirnome'){
			array_push($arCabecalho, 'Diretoria');
			array_push($colXls, 'dirnome');
		}
		if($agrup == 'coonome'){
			array_push($arCabecalho, 'Coordenação');
			array_push($colXls, 'coonome');
		}
		if($agrup == 'chenome'){
			array_push($arCabecalho, 'Chefia');
			array_push($colXls, 'chenome');
		}
		if($agrup == 'pronome'){
			array_push($arCabecalho, 'Processo');
			array_push($colXls, 'pronome');
		}
		if($agrup == 'situacao'){
			array_push($arCabecalho, 'Situação');
			array_push($colXls, 'situacao');
		}						
	}
	
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
	$dados = $dados ? $dados : array();
	foreach( $dados as $k => $registro ){
		foreach( $colXls as $campo ){
			$arDados[$k][$campo] = $registro[$campo];			
		}
	}
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_SIC_RelatorioGeral_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_DIMENCARGO_RELATORIO_GERAL_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%','');
	die;
}

// Monta agrupadores
function monta_agp()
{
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(												 
											"ptrinstituicao",
											"ptrano",
											"ptrmes",
											"cpfprofissional",
											"nomeprofissional",
											"situacao",											
											"ibge",
											"municipio",
											"uf",
											"cpftutor",
											"nometutor",
											"cpfsupervisor",
											"nomesupervisor",
											"justificativaprofissional",
											"observacao"
										  )	  
					);
	
	foreach ($agrupador as $val){ 
		
		switch ($val) {
			
		    case 'ptrinstituicao':
				array_push($agp['agrupador'], array(
													"campo" => "ptrinstituicao",
											  		"label" => "Instituição")										
									   				);				
		    	continue;
		        break;		    	
		    case 'ptrano':
				array_push($agp['agrupador'], array(
													"campo" => "ptrano",
											  		"label" => "Ano")										
									   				);				
		    	continue;
		        break;
		    case 'ptrmes':
				array_push($agp['agrupador'], array(
													"campo" => "ptrmes",
											 		"label" => "Mês")										
									   				);					
		    	continue;			
		        break;
		   
		}
	}
	
	// Adiciona ultimo nivel, dados da solicitacao
	array_push($agp['agrupador'], array(
										"campo" => "ptiid",
								  		"label" => "Item")
						   				);	
	
	return $agp;
}

// Monta colunas conforme agrupadores
function monta_coluna()
{
	$agrupador = $_REQUEST['agrupador'];
	
	$coluna = array();
	if(!in_array('ptrinstituicao', $agrupador) || $_POST['req'] == 'geraxls'){
		$coluna[] = array(
				"campo" => "ptrinstituicao",
				"label" => "Instituição"
		);
	}
	if(!in_array('ptrano', $agrupador) || $_POST['req'] == 'geraxls'){
		$coluna[] = array(
				"campo" => "ptrano",
				"label" => "Ano",
				"type"	=> "string"
		);
	}	
	if(!in_array('ptrmes', $agrupador) || $_POST['req'] == 'geraxls'){
		$coluna[] = array(
				"campo" => "ptrmes",
				"label" => "Mês",
				"type"	=> "string"
		);
	}	
	$coluna[] = array(
			"campo" => "cpfprofissional",
			"label" => "CPF do Profissional"
	);	
	$coluna[] = array(
			"campo" => "nomeprofissional",
			"label" => "Nome do Profissional"
	);
	$coluna[] = array(
			"campo" => "situacao",
			"label" => "Situação",
			"type"	=> "string"
	);
	$coluna[] = array(
			"campo" => "ibge",
			"label" => "IBGE",
			"type"	=> "string"
	);	
	$coluna[] = array(
			"campo" => "municipio",
			"label" => "Município"
	);
	$coluna[] = array(
			"campo" => "uf",
			"label" => "UF"
	);
	$coluna[] = array(
			"campo" => "cpftutor",
			"label" => "CPF do Tutor"
	);
	$coluna[] = array(
			"campo" => "nometutor",
			"label" => "Nome do Tutor"
	);
	$coluna[] = array(
			"campo" => "cpfsupervisor",
			"label" => "CPF do Supervisor"
	);
	$coluna[] = array(
			"campo" => "nomesupervisor",
			"label" => "Nome do Supervisor"
	);
	$coluna[] = array(
			"campo" => "justificativaprofissional",
			"label" => "Justificativa Profissional"
	);
	$coluna[] = array(
			"campo" => "observacao",
			"label" => "Observação"
	);
				
	return $coluna;	
}

// Monta sql do relatorio
function monta_sql()
{
	
	extract($_REQUEST);

	$arWhere[] = "pti.wssstatus = 'A'";
	$arWhere[] = "ptr.wssstatus = 'A'";
	
	// Instituição
	if( $ptrinstituicao[0] && $ptrinstituicao_campo_flag ){
		$arWhere[] = " ptr.ptrinstituicao ". (!$ptrinstituicao_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $ptrinstituicao )."' ) ";
	}
	
	if( $muncod[0] && $muncod_campo_flag ){
		$arWhere[] = " pti.ibge ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	}
	
	if($ptrmes){
		$arWhere[] = " ptr.ptrmes = '{$ptrmes}' ";
	}
	
	if($ptrano){
		$arWhere[] = " ptr.ptrano = '{$ptrano}' ";
	}
	
	if($situacao){
		$arWhere[] = " pti.situacao = '{$situacao}' ";
	}
	
	if($uf){
		$arWhere[] = " pti.uf = '{$uf}' ";
	}
	
	$sql = "select 
				ptr.ptrinstituicao,
				ptr.ptrano,
			
				case when ptr.ptrmes = 1 then 'Janeiro'
					 when ptr.ptrmes = 2 then 'Fevereiro'
					 when ptr.ptrmes = 3 then 'Março'
					 when ptr.ptrmes = 4 then 'Abril'
					 when ptr.ptrmes = 5 then 'Maio'
					 when ptr.ptrmes = 6 then 'Junho'
					 when ptr.ptrmes = 7 then 'Julho'
					 when ptr.ptrmes = 8 then 'Agosto'
					 when ptr.ptrmes = 9 then 'Setembro'
					 when ptr.ptrmes = 10 then 'Outubro'
					 when ptr.ptrmes = 11 then 'Novembro'
					 when ptr.ptrmes = 12 then 'Dezembro'
				end as ptrmes,
			
				'Item nº ' || pti.ptiid as ptiid,
				SUBSTR(pti.cpfprofissional, 1, 3)||'.'||SUBSTR(pti.cpfprofissional, 4, 3)||'.'||SUBSTR(pti.cpfprofissional, 7, 3)||'-'||SUBSTR(pti.cpfprofissional, 10) AS cpfprofissional,
				pti.nomeprofissional,
				pti.situacao,
				pti.ibge,
				pti.municipio,
				pti.uf,
				SUBSTR(pti.cpftutor, 1, 3)||'.'||SUBSTR(pti.cpftutor, 4, 3)||'.'||SUBSTR(pti.cpftutor, 7, 3)||'-'||SUBSTR(pti.cpftutor, 10) AS cpftutor,
				pti.nometutor,
				SUBSTR(pti.cpfsupervisor, 1, 3)||'.'||SUBSTR(pti.cpfsupervisor, 4, 3)||'.'||SUBSTR(pti.cpfsupervisor, 7, 3)||'-'||SUBSTR(pti.cpfsupervisor, 10) AS cpfsupervisor,
				pti.nomesupervisor,
				pti.justificativaprofissional,
				ptr.observacao
			from maismedicos.ws_planotrabalho ptr
			join maismedicos.ws_planotrabalho_itens pti on ptr.ptrid = pti.ptrid
			where pti.ptiid in (				
				select max(ptiid) from maismedicos.ws_planotrabalho_itens group by cpfprofissional
			)
				".( !empty($arWhere) ? ' AND ' . implode(' AND ', $arWhere) : '' )."
			order by 
				".( !empty($agrupador) ? implode(',', $agrupador) : '' );

	return $sql;
}

?>

<html>

	<head>
	
		<title>Relatório de Plano de Trabalho</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

		<?php
		
			$r = new montaRelatorio();
			$r->setAgrupador($agrup, $dados); 
			$r->setColuna($col);
			$r->setTotNivel(true);
			$r->setMonstrarTolizadorNivel(true);
			$r->setBrasao(true);
			$r->setEspandir( $_REQUEST['expandir'] );
			$r->setTotalizador(false);
			echo $r->getRelatorio();
			
		?>
		
	</body>
	
</html>