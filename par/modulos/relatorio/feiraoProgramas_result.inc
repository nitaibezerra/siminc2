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
	
	foreach ($_POST['agrupador'] as $agrup){
		if($agrup == 'secretaria'){
			array_push($arCabecalho, 'Secretaria');
			array_push($colXls, 'secretaria');
		}
		if($agrup == 'unidade'){
			array_push($arCabecalho, 'Unidade');
			array_push($colXls, 'unidade');
		}
		if($agrup == 'requerente'){
			array_push($arCabecalho, 'Requerente');
			array_push($colXls, 'requerente');
		}
		if($agrup == 'data'){
			array_push($arCabecalho, 'Data de inclusão');
			array_push($colXls, 'data');
		}
		if($agrup == 'situacao'){
			array_push($arCabecalho, 'Situação');
			array_push($colXls, 'situacao');
		}
		if($agrup == 'pergunta'){
			array_push($arCabecalho, 'Pergunta');
			array_push($colXls, 'pergunta');
		}
		if($agrup == 'prazo'){
			array_push($arCabecalho, 'Prazo');
			array_push($colXls, 'prazo');
		}					
	}
	
	if( is_array($col) ){
		foreach($col as $cabecalho){
			array_push($arCabecalho, $cabecalho['label']);
			array_push($colXls, $cabecalho['campo']);
		}
	}
	
	$arDados = Array();
	if( is_array($dados) ){
		foreach( $dados as $k => $registro ){
			foreach( $colXls as $campo ){
				$arDados[$k][$campo] = $campo == 'slcnumsic' ? (string) '-'.trim($registro[$campo]).'-' : $registro[$campo];			
			}
		}
	}
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_SIC_RelatorioGeral_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_SIC_RelatorioGeral_".date("Ymdhis").".xls");
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
												"esfera",
												"estuf",
												"mundescricao",		 
												"adpid",											   		
												"estuf",
												"mundescricao",
												"adprespostapacto",
												"adpdatarespostapacto",
												"adpresposta",
												"adpdataresposta",
												"pfcdescricao",
												"pcunome",
												"pcucpf",
												"pcuemail",
												"telefone",
												"tfodsc",
												"tvpdsc",
												"esddsc"
										  )	  
					);

	foreach ($agrupador as $val){ 
		
		switch ($val) {
			
		    case 'esfera':
				array_push($agp['agrupador'], array(
													"campo" => "esfera",
											  		"label" => "Esfera")										
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
		}
	}
	
	array_push($agp['agrupador'], array(
										"campo" => "adpid",
								  		"label" => "Adesão")
						   				);
	
	return $agp;
}

// Monta colunas conforme agrupadores
function monta_coluna()
{
	$agrupador = $_REQUEST['agrupador'];
	
	$coluna = array();
	if(!in_array('esfera', $agrupador)){
		$coluna[] = array(
						  "campo" => "esfera",
				   		  "label" => "Esfera"
					);
	}
	if(!in_array('estuf', $agrupador)){
		$coluna[] = array(
						  "campo" => "estuf",
				   		  "label" => "UF"
					);
	}
	if(!in_array('mundescricao', $agrupador)){
		$coluna[] = array(
						  "campo" => "mundescricao",
				   		  "label" => "Município"
					);
	}
	if(!in_array('adprespostapacto', $agrupador)){
		$coluna[] = array(
						  "campo" => "adprespostapacto",
				   		  "label" => "Adesao ao Pacto"
					);
	}
	if(!in_array('adpdatarespostapacto', $agrupador)){
		$coluna[] = array(
						  "campo" => "adpdatarespostapacto",
				   		  "label" => "Data de Adesão ao Pacto"
					);
	}
	if(!in_array('adpresposta', $agrupador)){
		$coluna[] = array(
						  "campo" => "adpresposta",
				   		  "label" => "Adesão ao Termo"
					);
	}
	if(!in_array('adpdataresposta', $agrupador)){
		$coluna[] = array(
						  "campo" => "adpdataresposta",
				   		  "label" => "Data de Adesão ao Termo"	
					);
	}
	if(!in_array('pfcdescricao', $agrupador)){
		$coluna[] = array(
						  "campo" => "pfcdescricao",
				   		  "label" => "Curso/Aba"
					);
	}
	if(!in_array('pcunome', $agrupador)){
		$coluna[] = array(
						  "campo" => "pcunome",
				   		  "label" => "Nome"
					);
	}
	if(!in_array('pcucpf', $agrupador)){
		$coluna[] = array(
						  "campo" => "pcucpf",
				   		  "label" => "CPF",
						  "type"=>"string"
					);
	}
	if(!in_array('pcuemail', $agrupador)){
		$coluna[] = array(
						  "campo" => "pcuemail",
				   		  "label" => "E-mail"
					);
	}
	if(!in_array('telefone', $agrupador)){
		$coluna[] = array(
						  "campo" => "telefone",
				   		  "label" => "Telefone"
					);
	}
	if(!in_array('tfodsc', $agrupador)){
		$coluna[] = array(
						  "campo" => "tfodsc",
				   		  "label" => "Formação"
					);
	}
	if(!in_array('tvpdsc', $agrupador)){
		$coluna[] = array(
						  "campo" => "tvpdsc",
				   		  "label" => "Vínculo"
					);
	}
	if(!in_array('pffdescricao', $agrupador)){
		$coluna[] = array(
						  "campo" => "pffdescricao",
				   		  "label" => "Função Atual"
					);
	}
	if(!in_array('esddsc', $agrupador)){
		$coluna[] = array(
						  "campo" => "esddsc",
				   		  "label" => "Situação"
					);
	}
	
				
	return $coluna;	
}

// Monta sql do relatorio
function monta_sql()
{
	
	extract($_REQUEST);

	$arWhere = array();
	
	// Programa
	if( $programa[0] && $programa_campo_flag ){
		$arWhere[] = " tap.prgid ". (!$programa_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $programa )." ) ";
	}
	
	// Municipio
	if( $muncod[0] && $muncod_campo_flag ){
		$arWhere[] = " mun.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $muncod )."' ) ";
	}
	
	// Ano de adesão
	if($adpano){
		$arWhere[] = " adp.adpano = '{$adpano}' ";
	}
	
	// periodo resposta termo
	if($dttermoinicio || $dttermofim){		
		if($dttermoinicio && $dttermofim){
			$arWhere[] = " adpdataresposta between '".formata_data_sql($dttermoinicio)."' and '".formata_data_sql($dttermofim)."' ";
		}else if($dttermoinicio){
			$arWhere[] = " adpdataresposta = '".formata_data_sql($dttermoinicio)."' ";
		}else if($dttermofim){
			$arWhere[] = " adpdataresposta = '".formata_data_sql($dttermofim)."' ";
		}
	}
	
	// periodo resposta termo
	if($dtpactoinicio || $dtpactofim){		
		if($dtpactoinicio && $dtpactofim){
			$arWhere[] = " adpdatarespostapacto between '".formata_data_sql($dtpactoinicio)."' and '".formata_data_sql($dtpactofim)."' ";
		}else if($dtpactoinicio){
			$arWhere[] = " adpdatarespostapacto = '".formata_data_sql($dtpactoinicio)."' ";
		}else if($dtpactofim){
			$arWhere[] = " adpdatarespostapacto = '".formata_data_sql($dtpactofim)."' ";
		}
	}
	
	$sql = "select distinct
				'Adesão N.º ' || adp.adpid as adpid,
				case when tprcod = 1 then 'Estadual' else 'Municipal' end as esfera,
				case when inu.estuf is null then inu.mun_estuf else inu.estuf end as estuf,
				mun.mundescricao,
				esd.esddsc,
				adp.adprespostapacto,
				to_char(adp.adpdatarespostapacto, 'dd/mm/yyyy') as adpdatarespostapacto,
				adp.adpresposta,
				to_char(adp.adpdataresposta, 'dd/mm/yyyy') as adpdataresposta,
				pfc.pfcdescricao,
				pcu.pcunome,
				pcu.pcucpf,
				pcu.pcuemail,
				'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone,
				tfo.tfodsc,
				tvp.tvpdsc,
				pff.pffdescricao
			from par.pfadesaoprograma adp
			inner join par.pftermoadesaoprograma tap on adp.tapid = tap.tapid
			inner join par.instrumentounidade inu on inu.inuid = adp.inuid
			left join workflow.documento doc on doc.docid = adp.docid
			left join workflow.estadodocumento esd on esd.esdid = doc.esdid 
			left join territorios.municipio mun on mun.muncod = inu.muncod
			left join par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'
			left join par.pfcursista pcu on pcu.adpid = adp.adpid
			left join public.tipoformacao tfo on tfo.tfoid = pcu.tfoid
			left join public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid
			left join par.pffuncao pff on pff.pffid = pcu.pffid
			where
				tapstatus = 'A'			
			".( !empty($arWhere) ? ' AND' . implode(' AND ', $arWhere) : '' ).
			"order by ".( !empty($agrupador) ? implode(',', $agrupador) : '' );

	return $sql;
}

?>

<html>

	<head>
	
		<title>Relatório Geral - Programas do MEC</title>
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