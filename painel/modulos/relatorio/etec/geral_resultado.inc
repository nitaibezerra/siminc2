<?php

set_time_limit(0);
ini_set("memory_limit", "1024M");

function etec_monta_sql_relatorio(){
	
	$where = array();
	
	extract($_REQUEST);
	
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
	
	// ofertante
	if( $ofertante[0]  && $ofertante_campo_flag ){
		array_push($where, " co.coeofertante " . (!$ofertante_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ofertante ) . "') ");
	}
	
	// nome curso
	if( $nomecurso[0]  && $nomecurso_campo_flag ){
		array_push($where, " co.coenomecurso " . (!$nomecurso_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $nomecurso ) . "') ");
	}
	
	// polo
	if( $polo[0]  && $polo_campo_flag ){
		array_push($where, " co.coepolo " . (!$polo_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $polo ) . "') ");
	}
	
	// monta o sql 
	$sql = "SELECT 
				co.coepolo       	   as polo,
				lower(co.coenomecurso)    	   as nomecurso,
				co.coeofertante        as ofertante,
				mu.mundescricao 	   as municipio,
				co.coeqtdaluno		   as coeqtdaluno,
				co.coeqtdpolo		   as coeqtdpolo,
				es.estuf               as uf,
				re.regdescricao        as regiao 
			FROM painel.consultaofertasetec co
			INNER JOIN territorios.municipio mu ON mu.muncod = co.muncod
			INNER JOIN territorios.estado es ON es.estuf = mu.estuf 
			INNER JOIN territorios.regiao re ON re.regcod = es.regcod  
			".(($where)?"WHERE ".implode(" AND ", $where):"")."
			ORDER BY " . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' ) ." ";
//dbg($sql);
	return $sql;
	
}


function etec_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("coeqtdpolo",
										  "coeqtdaluno"),
				"agrupadorDetalhamento" => array(
													array(
														'codigo'    => 'polo',
														'descricao' => 'Polo'
														  ),
													array(
														'codigo'    => 'nomecurso',
														'descricao' => 'Nome Curso'
														  ),
													array(
														'codigo'    => 'ofertante',
														'descricao' => 'Ofertante'										
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
			case "polo":
				array_push($agp['agrupador'], array(
													"campo" => "polo",
											  		"label" => "Polo")										
									   				);
			break;
			case "nomecurso":
				array_push($agp['agrupador'], array(
													"campo" => "nomecurso",
											  		"label" => "Nome Curso")										
									   				);
			break;
			case "ofertante":
				array_push($agp['agrupador'], array(
													"campo" => "ofertante",
											  		"label" => "Ofertante")										
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
$sql       = etec_monta_sql_relatorio();
$agrupador = etec_monta_agp_relatorio();

$coluna = array();

array_push( $coluna, array("campo" 	  => "coeqtdpolo",
				   		   "label" 	  => "Quantidade de Polos",
				   		   "type"	  => "numeric") );

array_push( $coluna, array("campo" 	  => "coeqtdaluno",
				   		   "label" 	  => "Quantidade de Alunos",
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