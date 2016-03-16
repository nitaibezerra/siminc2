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
		$where[] = "( edu.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') OR
					  edc.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') )";
	}
		
	if( $muncod[0] && ( $muncod_campo_flag || $muncod_campo_flag == '1' )){
		$where[] = "( muu.muncod  " . (($muncod_campo_excludente == null || $muncod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') OR
					  muc.muncod " . (($muncod_campo_excludente == null || $muncod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') )";
	}
		
	if( $universidade[0] && ( $universidade_campo_flag || $universidade_campo_flag == '1' )){
		$where[] = "( uni.entid " . (($universidade_campo_excludente == null || $universidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $universidade ) . "') OR
					  e3.entid " . (($universidade_campo_excludente == null || $universidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $universidade ) . "') )";
	}
	
	if( $campus[0] && ( $campus_campo_flag || $campus_campo_flag == '1' )){
		$where[] = " cam.entid " . (($campus_campo_excludente == null || $campus_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $campus ) . "') ";
	}
		
	if( $pertitulo ){
		$where[] = " per.pertitulo ilike '%$pertitulo%'";
	}
		
	if( $resdescricao != '' ){
		$resdescricao = str_replace('\\','',$resdescricao);
		$where[] = " res.resdescricao $resdescricao";
	}
	
	
	$sql = "SELECT 
				res.entid, 
				coalesce(edu.estuf,edc.estuf) as uf,
				coalesce(muu.mundescricao, muc.mundescricao) as municipio,
				coalesce(uni.entnome,e3.entnome) as universidade,
				coalesce(cam.entnome,'1. Universidade') as campus,
				per1.pernumero||' '||per1.pertitulo as pertitulo1,
				per2.pernumero||' '||per2.pertitulo as pertitulo2,
				per3.pernumero||' '||per3.pertitulo as pertitulo3,
				per4.pernumero||' '||per4.pertitulo as pertitulo4,
				CASE WHEN resdescricao = 'TRUE'
					THEN 1
					ELSE 0
				END as sim,
				CASE WHEN resdescricao = 'FALSE'
					THEN 1
					ELSE 0
				END as nao,
				CASE WHEN resdescricao IS NULL
					THEN 1
					ELSE 0
				END as nao_respondeu
			FROM 
				academico.pergunta per
			INNER JOIN academico.resposta res ON res.perid = per.perid 
			LEFT  JOIN academico.pergunta per1 ON per1.pernumero = substring(per.pernumero,0,3) AND per1.pernivel = 1
			LEFT  JOIN academico.pergunta per2 ON per2.pernumero = substring(per.pernumero,0,5) AND per2.pernivel = 2
			LEFT  JOIN academico.pergunta per3 ON per3.pernumero = substring(per.pernumero,0,7) AND per3.pernivel = 3
			LEFT  JOIN academico.pergunta per4 ON per4.pernumero = substring(per.pernumero,0,9) AND per4.pernivel = 4
			
			LEFT JOIN entidade.funcaoentidade 	efu ON efu.entid  = res.entid AND efu.funid = 12
			LEFT JOIN entidade.entidade 	  	uni ON uni.entid  = efu.entid AND uni.entstatus = 'A'
			LEFT JOIN entidade.endereco 		edu ON edu.entid  = uni.entid 
			LEFT JOIN territorios.municipio 	muu ON muu.muncod = edu.muncod
			
			LEFT JOIN entidade.funcaoentidade 	efc ON efc.entid  = res.entid AND efc.funid = 18
			LEFT JOIN entidade.entidade 	  	cam ON cam.entid  = efc.entid AND cam.entstatus = 'A'
			LEFT JOIN entidade.endereco 		edc ON edc.entid  = cam.entid AND edc.endstatus = 'A' AND edc.estuf IS NOT NULL AND edc.muncod IS NOT NULL
			LEFT JOIN territorios.municipio 	muc ON muc.muncod = edc.muncod
			LEFT JOIN entidade.funentassoc 	  	ea  ON ea.fueid   = efc.fueid 
			LEFT JOIN entidade.entidade	  	e3  ON e3.entid   = ea.entid
			WHERE
				(per.pernumero ilike '1.%' OR per.pernumero ilike '2.%') 
				AND per.tppid = 2 
				" . (is_array($where) ? " 
				AND ".implode(' AND ', $where) : '') . "
			ORDER BY
				uf,municipio,universidade,campus,pertitulo1,pertitulo2,pertitulo3,pertitulo4";
	
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"sim",	
									   		"nao",
											"nao_respondeu"
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
									"campo" => "pertitulo1",
							  		"label" => "Pergunta")										
					   				);	
	array_push($agp['agrupador'], array(
									"campo" => "pertitulo2",
							  		"label" => "Pergunta")										
					   				);	
	array_push($agp['agrupador'], array(
									"campo" => "pertitulo3",
							  		"label" => "Pergunta")										
					   				);	
	array_push($agp['agrupador'], array(
									"campo" => "pertitulo4",
							  		"label" => "Pergunta")										
					   				);	
	return $agp;
}

function monta_coluna(){
	
	$coluna    = array(
						array(
							  "campo" => "sim",
					   		  "label" => "Sim",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "nao",
					   		  "label" => "Não",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "nao_respondeu",
					   		  "label" => "Não Respondeu",
							  "type"  => "numeric"
						)
					);
					
	return $coluna;			  	
}
?>
</body>