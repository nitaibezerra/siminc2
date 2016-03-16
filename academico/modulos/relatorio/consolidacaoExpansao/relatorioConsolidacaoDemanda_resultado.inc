<?
set_time_limit(0);

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
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
$r->setBrasao($true ? true : false);
$r->setTotNivel(true);
$r->setEspandir($_POST['expandir']);
if( $_POST['req'] == 2 ){
	ob_clean();
	$nomeDoArquivoXls="Relatório_Decreto_7446_de_1º_de_março_de_2011_-_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
}else{
	echo $r->getRelatorio();
}

function monta_sql(){
	
	extract($_POST);
	
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
		$where[] = " edc.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";
	}
		
	if( $muncod[0] && ( $muncod_campo_flag || $muncod_campo_flag == '1' )){
		$where[] = " muc.muncod " . (($muncod_campo_excludente == null || $muncod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') ";
	}
		
	if( $universidade[0] && ( $universidade_campo_flag || $universidade_campo_flag == '1' )){
		$where[] = " e3.entid " . (($universidade_campo_excludente == null || $universidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $universidade ) . "') ";
	}
	
	if( $campus[0] && ( $campus_campo_flag || $campus_campo_flag == '1' )){
		$where[] = " res.entid " . (($campus_campo_excludente == null || $campus_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $campus ) . "') ";
	}
	
	if( $tbtid[0] && ( $tbtid_campo_flag || $tbtid_campo_flag == '1' )){
		$where[] = " res.tbtid " . (($tbtid_campo_excludente == null || $tbtid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tbtid ) . "') ";
	}
		
	if( $tbrdemanda != '' ){
		$where[] = " res.tbrdemanda ilike '%$tbrdemanda%'";
	}
	
	$sql = "SELECT 
				res.entid,  
				edc.estuf as uf,
				muc.mundescricao as municipio,
				e3.entnome as universidade,
				cam.entnome as campus,
				'4. Demanda' as pergunta1,
				pernumero||' - '||pertitulo as pergunta2, 
				tbttipo as tipo,
				'<input type=\"hidden\" value=\"'||res.tbrid||'\" />'||tbrdemanda as tbrdemanda,
				tbrarea,
				tbraluno,
				tv1.tbvcusteio as custeio2013,
				tv1.tbvcapital as capital2013,
				tv2.tbvcusteio as custeio2014,
				tv2.tbvcapital as capital2014,
				tv3.tbvcusteio as custeio2015,
				tv3.tbvcapital as capital2015,
				CASE WHEN tbrprojeto
					THEN 1
					ELSE 0
				END as possui_projeto,
				CASE WHEN tbrprojeto
					THEN 0
					ELSE 1
				END as nao_possui_projeto
			FROM 
				academico.pergunta per
			LEFT JOIN academico.tabelaresposta res ON res.perid = per.perid
			LEFT JOIN academico.tabelatipo tbt ON tbt.tbtid = res.tbtid
			LEFT JOIN academico.tabelavalor tv1 ON tv1.tbrid = res.tbrid AND tv1.tbvano = '2013'
			LEFT JOIN academico.tabelavalor tv2 ON tv2.tbrid = res.tbrid AND tv2.tbvano = '2014'
			LEFT JOIN academico.tabelavalor tv3 ON tv3.tbrid = res.tbrid AND tv3.tbvano = '2015'
			LEFT JOIN entidade.funcaoentidade 	efc ON efc.entid  = res.entid AND efc.funid = 18
			LEFT JOIN entidade.entidade 	  	cam ON cam.entid  = efc.entid AND cam.entstatus = 'A'
			LEFT JOIN entidade.endereco 		edc ON edc.entid  = cam.entid AND edc.endstatus = 'A' AND edc.estuf IS NOT NULL AND edc.muncod IS NOT NULL
			LEFT JOIN territorios.municipio 	muc ON muc.muncod = edc.muncod
			LEFT JOIN entidade.funentassoc 	  	ea  ON ea.fueid   = efc.fueid 
			LEFT JOIN entidade.entidade	  	e3  ON e3.entid   = ea.entid
			WHERE
				per.pernumero ilike '4.%' AND per.perid != 43
				" . (is_array($where) ? " 
				AND ".implode(' AND ', $where) : '') . "
			ORDER BY
				uf,municipio,universidade,campus,pergunta1,pergunta2,tipo,tbrdemanda";
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											//"tbrdemanda",
											"tbrarea",
											"tbraluno",
											"custeio2013",
											"capital2013",
											"custeio2014",
											"capital2014",
											"custeio2015",
											"capital2015",
											"possui_projeto",	
									   		"nao_possui_projeto"
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'estuf':
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "UF")										
									   				);					
		    	continue;
		        break;		        	
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);			
		    case 'universidade':
		    	array_push($agp['agrupador'], array(
													"campo" => "universidade",
											  		"label" => "Universidade")										
									   				);
   								
		    	continue;
		        break;
			case 'campus':
				array_push($agp['agrupador'], array(
												"campo" => "campus",
										  		"label" => "Campus")										
								   				);				
		    	continue;
		        break;		        		    	
		}
	endforeach;
	array_push($agp['agrupador'], array(
									"campo" => "pergunta1",
							  		"label" => "Pergunta")										
					   				);	
	array_push($agp['agrupador'], array(
									"campo" => "pergunta2",
							  		"label" => "Pergunta")										
					   				);	
	array_push($agp['agrupador'], array(
									"campo" => "tipo",
							  		"label" => "Tipo")										
					   				);		
	array_push($agp['agrupador'], array(
									"campo" => "tbrdemanda",
							  		"label" => "Demanda")										
					   				);	
	return $agp;
}

function monta_coluna(){
											
	$coluna    = array(
						array(
							  "campo" => "tbrdemanda",
					   		  "label" => "Demanda"
						),
						array(
							  "campo" => "tbrarea",
					   		  "label" => "Area",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "tbraluno",
					   		  "label" => "Alunos",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "custeio2013",
					   		  "label" => "Custeio 2013",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "capital2013",
					   		  "label" => "Capital 2013",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "custeio2014",
					   		  "label" => "Custeio 2014",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "capital2014",
					   		  "label" => "Capital 2014",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "custeio2015",
					   		  "label" => "Custeio 2015",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "capital2015",
					   		  "label" => "Capital 2015",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "possui_projeto",
					   		  "label" => "Possui projeto",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "nao_possui_projeto",
					   		  "label" => "Não Possui Projeto",
							  "type"  => "numeric"
						)
					);
					
	return $coluna;			  	
}
?>
</body>