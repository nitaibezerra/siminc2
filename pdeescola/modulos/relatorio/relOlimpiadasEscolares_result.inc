<?php 

ini_set("memory_limit", "2048M");

if(isset($_REQUEST['agrupadores'])){
	if(is_array($_REQUEST['agrupadores'])){
		foreach($_REQUEST['agrupadores'] as $agrupador){
			$_REQUEST['agrupador'][] = strtolower($agrupador);
		}
		
	}
}

if(isset($_REQUEST['ponto'])){
	if(is_array($_REQUEST['ponto'])){
		foreach($_REQUEST['ponto'] as $ponto){
			switch ($ponto){
				case 'atletismo'	: $_REQUEST['tesid'][] = 1; break;
				case 'sesi'			: $_REQUEST['tesid'][] = 2; break;
				case 'aabb'			: $_REQUEST['tesid'][] = 3; break;
				case 'abrigos'		: $_REQUEST['tesid'][] = 4; break;
				case 'institutos'	: $_REQUEST['tesid'][] = 5; break; 
			}		
		}
	}
}

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql();
$dados = $db->carregar($sql);

// Gerar arquivo xls
if( $_POST['req'] == 'geraxls' ){
	
	$arCabecalho = array();
	$colXls = array();
	
	foreach ($_POST['agrupador'] as $agrup){
		if($agrup == 'secretaria'){
			array_push($arCabecalho, 'Secretaria');
			array_push($colXls, 'secretaria');
		}			
	}
	
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
	foreach( $dados as $k => $registro ){
		foreach( $colXls as $campo ){
			$arDados[$k][$campo] = $campo == 'slcnumsic' ? (string) '-'.trim($registro[$campo]).'-' : $registro[$campo];			
		}
	}
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_PDE_OlimpiadasEscolares_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_PDE_OlimpiadasEscolares_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%','');
	die;
}

// Monta agrupadores
function monta_agp()
{
	$agrupador = $_REQUEST['agrupador'] ? $_REQUEST['agrupador'] : array();
	
	$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(												 
											"distancia",
											"estid",
											"instituicao",
											"entcodent",
											"entnome",
											"estuf",											
											"endbai",
											"endlog",
											"endnum",
											"mundescricao",
											"entid",
											"lat",
											"lng",
											"tipo"												   		
										  )	  
					);
					
foreach ($agrupador as $val){ 
	
		switch ($val) {
			
		    case 'tipo':
				array_push($agp['agrupador'], array(
										"campo" => "tipo",
								  		"label" => "Tipo")
						   				);			
		    	continue;
		        break;		    	
		    case 'estuf':
				array_push($agp['agrupador'], array(
										"campo" => "estuf",
								  		"label" => "UF")
						   				);			
		    	continue;
		        break;
		    case 'mundescricao':
				array_push($agp['agrupador'], array(
										"campo" => "mundescricao",
								  		"label" => "Município")
						   				);				
		    	continue;			
		        break;	
		    case 'instituicao':
				array_push($agp['agrupador'], array(
										"campo" => "instituicao",
								  		"label" => "Instituição")
						   				);
				continue;
				break;				 
		}
	}
						   										   				
	// Adiciona ultimo nivel, dados da solicitacao
	array_push($agp['agrupador'], array(
										"campo" => "entnome",
								  		"label" => "Escola")
						   				);	
	
	return $agp;
}

// Monta colunas conforme agrupadores
function monta_coluna()
{
	$agrupador = $_REQUEST['agrupador'] ? $_REQUEST['agrupador'] : array();
	
	$coluna = array();
	if(!in_array('distancia', $agrupador)){
		$coluna[] = array(
						  "campo" => "distancia",
				   		  "label" => "Distância (km)",
						  "type"  => "string"
					);
	}
	if(!in_array('instituicao', $agrupador)){
		$coluna[] = array(
						  "campo" => "instituicao",
				   		  "label" => "Instituição"
					);
	}
	if(!in_array('mundescricao', $agrupador)){
		$coluna[] = array(
						  "campo" => "mundescricao",
				   		  "label" => "Município"
					);
	}
	if(!in_array('estuf', $agrupador)){
		$coluna[] = array(
						  "campo" => "estuf",
				   		  "label" => "UF"
					);
	}
	if(!in_array('tipo', $agrupador)){
		$coluna[] = array(
						  "campo" => "tipo",
				   		  "label" => "Tipo"
					);
	}
	
	return $coluna;	
}

// Monta sql do relatorio
function monta_sql()
{
	
	extract($_REQUEST);
	
	$arWhere = array();
	
	// tipo
	if( $tesid[0] ){
		$arWhere[] = "atl.tesid ". (!$tesid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $tesid )." ) ";
	}
	
	if( $muncod[0] ){
		$arWhere[] = "atl.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $muncod )."' ) ";
	}
	
	if( $estuf[0] ){
		$arWhere[] = "atl.estuf ". (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "','", $estuf )."' ) ";
	}
	
	if($raio){
		$arWhere[] = "(ST_Distance_Sphere(ST_Transform(eng.point,4291), ST_Transform(atl.estlatlong,4291))/1000) <= {$raio}";	
	}
	
	$sql = "
			SELECT				
				round((ST_Distance_Sphere(ST_Transform(eng.point,4291), ST_Transform(atl.estlatlong,4291))/1000)::numeric,2) as distancia,
				atl.estid,
				atl.estnome as instituicao,
				ent.entcodent, 
				ent.entnome, 
				ede.estuf, 
				ede.endbai, 
				ede.endlog, 
				ede.endnum, 
				mun.mundescricao, 
				eng.entid, 
				eng.latitudedecimal as lat, 
				eng.longitudedecimal as lng,
				eng.entid,
				(select tesdesc from entidade.tipoestrutura tes where tes.tesid = atl.tesid ) as tipo
			FROM entidade.estrutura atl,  entidade.geo eng
			left join entidade.endereco ede on ede.entid = eng.entid
			left join territorios.municipio mun on mun.muncod = ede.muncod
			join entidade.entidade ent on ent.entid = eng.entid			
			".( !empty($arWhere) ? ' WHERE ' . implode(' AND ', $arWhere) : '' )."
			order by atl.estnome";
	
//	ver($_REQUEST, $sql, d);

	return $sql;
}

?>

<html>

	<head>
	
		<title>Relatório Olimpíadas Escolares</title>
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