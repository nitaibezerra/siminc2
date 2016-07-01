<?php

set_time_limit(0);
ini_set("memory_limit", "1024M");

function uab_monta_sql_relatorio(){
	
	$where = array();
	
	extract($_REQUEST);
	
	
	// mantenedor
	if( $idmantenedor[0] && $idmantenedor_campo_flag ){
		array_push($where, " co.idmantenedor " . (!$idmantenedor_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $idmantenedor ) . "') ");
	}
	
	// região
	if( $regiao[0] && $regiao_campo_flag ){
		array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $regiao ) . "') ");
	}
	
	// UF
	if( $uf[0] && $uf_campo_flag ){
		array_push($where, " es.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uf ) . "') ");
	}
	
	// municipio
	if( $municipio[0]  && $municipio_campo_flag ){
		array_push($where, " mu.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ");
	}
	
	// ies
	if( $iesid[0]  && $iesid_campo_flag ){
		array_push($where, " ie.iesid " . (!$iesid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $iesid ) . "') ");
	}
	
	// nome curso
	if( $idcurso[0]  && $iesid_campo_flag ){
		array_push($where, " co.idcurso " . (!$idcurso_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $idcurso ) . "') ");
	}
	
	// tipo curso
	if( $tipocurso[0]  && $tipocurso_campo_flag ){
		array_push($where, " co.tipocurso " . (!$tipocurso_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tipocurso ) . "') ");
	}
	
	// polo
	if( $polid[0]  && $polid_campo_flag ){
		array_push($where, " pl.polid " . (!$polid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $polid ) . "') ");
	}
	
	// monta o sql 
	$sql = "SELECT 
				co.mantenedor   	   as mantenedor,
				pl.poldsc       	   as polo,
				co.nomecurso    	   as nomecurso,
				co.tipocurso    	   as tipocurso,
				ie.iesdsc       	   as ies,
				mu.mundescricao 	   as municipio,
				co.nummatriculasativas as nummatriculasativas,
				co.turmasativas 	   as turmasativas,
				es.estuf               as uf,
				re.regdescricao        as regiao 
			FROM painel.consultaofertas co 
			LEFT JOIN painel.polo pl ON pl.polid = trim(co.idpolo)::integer 
			LEFT JOIN painel.ies ie ON ie.iesid = trim(co.idies)::integer 
			LEFT JOIN territorios.municipio mu ON mu.muncod = trim(co.muncod) 
			LEFT JOIN territorios.estado es ON es.estuf = mu.estuf 
			LEFT JOIN territorios.regiao re ON re.regcod = es.regcod 
			".(($where)?"WHERE ".implode(" AND ", $where):"")." ";

	return $sql;
	
}


function uab_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("nummatriculasativas",
										  "turmasativas"),
				"agrupadorDetalhamento" => array(
													array(
														'codigo'    => 'mantenedor',
														'descricao' => 'Mantenedor'
														  ),
													array(
														'codigo'    => 'polo',
														'descricao' => 'Polo'
														  ),
													array(
														'codigo'    => 'nomecurso',
														'descricao' => 'Nome Curso'
														  ),
													array(
														'codigo'    => 'ies',
														'descricao' => 'IES'										
											   			  ),
													array(
														'codigo'    => 'tipocurso',
														'descricao' => 'Tipo Curso'										
											   			  ),
											   		array(
														'codigo'    => 'municipio',
														'descricao' => 'Município'										
											   			  ),					  
											   		array(
														'codigo'    => 'uf',
														'descricao' => 'UF'										
											   			  ),
											   		array(
														'codigo'    => 'regiao',
														'descricao' => 'Região'										
											   			  ),
												)	  
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "mantenedor":
				array_push($agp['agrupador'], array(
													"campo" => "mantenedor",
											  		"label" => "Mantenedor")										
									   				);
			break;
			case "polo":
				array_push($agp['agrupador'], array(
													"campo" => "polo",
											  		"label" => "Polo")										
									   				);
			break;
			
			case "tipocurso":
				array_push($agp['agrupador'], array(
													"campo" => "tipocurso",
											  		"label" => "Tipo Curso")										
									   				);
			break;
			
			case "nomecurso":
				array_push($agp['agrupador'], array(
													"campo" => "nomecurso",
											  		"label" => "Nome Curso")										
									   				);
			break;
			case "ies":
				array_push($agp['agrupador'], array(
													"campo" => "ies",
											  		"label" => "IES")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Região")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = uab_monta_sql_relatorio();
$agrupador = uab_monta_agp_relatorio();

$coluna = array();

array_push( $coluna, array("campo" 	  => "nummatriculasativas",
				   		   "label" 	  => "Número Matriculas Ativas",
				   		   "type"	  => "numeric") );

array_push( $coluna, array("campo" 	  => "turmasativas",
				   		   "label" 	  => "Turmas Ativas",
				   		   "type"	  => "numeric") );

$dados 	   = $db->carregar( $sql );


	
$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);
$rel->setTotalizador(false);

// Gera o XLS do relatório
if ( $_REQUEST['tipoRelatorio'] == 'xls' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
	<!--  Monta o Relatório -->
	<? echo $rel->getRelatorio(); ?>
	</body>
</html>