<?

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","250M");
set_time_limit(0);
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
	$r->setBrasao(true);
	$r->setTotNivel(false);
	$r->setEspandir(false);
	$r->setMonstrarTolizadorNivel(false);
	$r->setTotalizador(true);
	$r->setTolizadorLinha(false);
	
	echo $r->getRelatorio();
?>
</body>
</html>
<?php 




function monta_sql(){
	global $filtroSession;
	
	extract($_POST);
	
//	ver( $_POST );
	
	$select = array();
	$from	= array();
	
	if( $equipe[0] && ( $equipe_campo_flag || $equipe_campo_flag == '1' )){
		$where[0] = " AND sc.cnecodigocnes " . (( $equipe_campo_excludente == null || $equipe_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $equipe ) . "') ";		
	}
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[1] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[2] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $esfera[0] != '' && ( $esfera_campo_flag ||$esfera_campo_flag == '1' )){
		$where[3] = " AND p.empflagestmun " . (( $esfera_campo_excludente == null || $esfera_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esfera ) . "') ";		
	}
	if( $exer[0] && ( $exer_campo_flag || $exer_campo_flag == '1' )){
		$where[4] = " AND ep.espano " . (( $exer_campo_excludente == null || $exer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $exer ) . "') ";		
	}
	if( $anoportaria[0] && ( $anoportaria_campo_flag || $anoportaria_campo_flag == '1' )){
		if ( in_array('2008',$anoportaria) ) {
			$valores[] = 't';
		}
		if ( in_array('2009',$anoportaria) ) {
			$valores[] = 'f';
		}	
		if($valores) $where_ano_portaria = " AND ep.espparticipapse2009 " . (( $anoportaria_campo_excludente == null || $anoportaria_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $valores ) . "') ";		
	}
	
	$sql = "SELECT  
				case when p.empflagestmun = 'm' then 'Municipal' else 'Estadual' end as esfera,
				entSecretaria.entid_secretaria,
				entSecretaria.nome_secretaria AS secretaria, 
				entEscola.entid,
				entd.entcodent || ' - ' || entEscola.entnome AS escola, 
				ende.estuf, 
				mun.mundescricao, 
				sc.cnenomefantasia as unidadesaude, 
				sc.cneseqequipe as equipe,
				CASE
					WHEN ep.espparticipapse2009 = 't' THEN '2008' 
					ELSE '2009'
				END as anoportaria 
			FROM 
				entidade.entidade entEscola
			LEFT JOIN
				entidade.endereco ende ON ende.entid = entEscola.entid
			INNER JOIN
				entidade.funcaoentidade feEscola ON feEscola.entid = entEscola.entid
			INNER JOIN
				entidade.entidadedetalhe entd ON entd.entid = entEscola.entid		
			INNER JOIN
				territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN
				pse.estadomunicipiopse p ON p.muncod = mun.muncod
			INNER JOIN
				pse.escolapse ep ON ep.entid = entEscola.entid
			LEFT JOIN
				pse.equipesaudefamilia esf ON esf.espid = ep.espid
			LEFT JOIN
				pse.scnes sc ON esf.cneid = sc.cneid

			LEFT JOIN 
				( SELECT
					entSecretaria.entid AS entid_secretaria,
					mun.muncod AS muncod_secretaria, 
					entSecretaria.entnome AS nome_secretaria
				  FROM 	
					entidade.endereco ende 

					inner join entidade.entidade entSecretaria 
					on ende.entid = entSecretaria.entid

					inner join entidade.funcaoentidade feSecretaria 
					on entSecretaria.entid = feSecretaria.entid 
					and 
					feSecretaria.funid in (7) -- Secretaria Municipal de Educação					

					inner join territorios.municipio mun 
					on mun.muncod = ende.muncod

				  WHERE
				  	0=0
					".$where[1]."
					".$where[2]."
					
				) entSecretaria
				  ON entSecretaria.muncod_secretaria = ende.muncod

			WHERE	
				entEscola.entstatus='A' 
				AND
				feEscola.funid in (3) -- Escola
				
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
				".$where[4]."
				".$where_ano_portaria."				
			
			ORDER BY 
				entid_secretaria, ende.estuf, mun.mundescricao";
	
	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array()	  
				);
	
	$count = 1;
		$i = 0;
		foreach( $_POST['esfera'] as $esfera => $valor ){
			if( $valor == 'm' ){
				$vari = "PSE - PSE - Programa Saúde na Escola - Relatório ESF e Secretaria- Secretarias Municipais<br>";
				$i++;
			} elseif( $valor == 'e' ){
				$vari = "PSE - Programa Saúde na Escola - Relatório ESF e Secretaria- Secretarias Estaduais<br>";
				$i++;
			}
		}
		
		if( $i > 1 || $i == 0 ){
			$vari = "PSE - Programa Saúde na Escola - Relatório ESF e Secretaria- Secretarias Estaduais e Municipais<br>";	
		}
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {
			
		    case 'anoportaria':
				array_push($agp['agrupador'], array(
													"campo" => "anoportaria",
											 		"label" => "$var Ano da Portaria")										
									   				);					
		   		continue;			
		    break;			
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
		    case 'equipe':
				array_push($agp['agrupador'], array(
													"campo" => "unidadesaude",
											 		"label" => "$var Equipe de Saúde")										
									   				);					
		    	continue;			
		    break;	
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											 		"label" => "$var INEP - Escola")										
									   				);					
		   		continue;			
		    break;
		    case 'secretaria':
				array_push($agp['agrupador'], array(
													"campo" => "secretaria",
											 		"label" => "$var Secretaria")										
									   				);					
		   		continue;			
		    break;	
		    
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna() {
	//
}
?>
