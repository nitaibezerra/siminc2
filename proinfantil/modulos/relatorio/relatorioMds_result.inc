<?php 

ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql();
$dados = $db->carregar($sql);

// Gerar arquivo xls
if( $_POST['req'] == 'geraxls' ){
	
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna($col);
	$r->setTotNivel(true);
//	$r->setMonstrarTolizadorNivel(true);
	$r->setBrasao(true);
	$r->setEspandir( $_REQUEST['expandir'] );
	$r->setTotalizador(true);
	echo $r->getRelatorioXls();
	die;
	
}

// Monta agrupadores
function monta_agp()
{
	$agrupador = $_REQUEST['agrupador'];
	$colunas = $_REQUEST['coluna'];
	
	$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(	
												"entnumcpfcnpjescola",
												"escola",												
												"muncod",
												"entnumcpfcnpj",
												"estuf",
												"mundescricao",
												"esddsc",
												"nomeprefeito",
												"emailprefeito",
												"telefoneprefeito",	
												"totalescolapublica",
												"totalescolasparticulares",
												"qtdparcialprivada",
												"qtdintegralprivada",
												"qtdparcialpublica",
												"qtdintegralpublica",
												"vlrrepasseparcialpublica",
												"vlrrepasseintegralpublica",
												"vlrrepasseparcialprivada",
												"vlrrepasseintegralprivada",
												"total"
										  )	  
					);

	foreach ($agrupador as $val){ 
		
		switch ($val) {
			
		    case 'esddsc':
				array_push($agp['agrupador'], array(
													"campo" => "esddsc",
											  		"label" => "Situação")										
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
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											  		"label" => "Nome da Escola")										
									   				);				
		    	continue;
		        break;
		}
	}
	
	if(in_array('escola', $colunas) || in_array('escola', $agrupador)){
		array_push($agp['agrupador'], array(
											"campo" => "escola",
									  		"label" => "Escola")										
							   				);
	}
	
	return $agp;
}

// Monta colunas conforme agrupadores
function monta_coluna()
{
	$agrupador = $_REQUEST['agrupador'];
	$colunas = $_REQUEST['coluna'];		
	
	$excel = false;
	if($_POST['req'] == 'geraxls'){
		$excel = true;
	}
	
	$coluna = array();
	if(!in_array('muncod', $agrupador) && in_array('muncod', $colunas) || ($excel && in_array('muncod', $colunas)) ){
		$coluna[] = array(
						  "campo" => "muncod",
				   		  "label" => "Código do Município",
						  "type" => "string"
					);
	}
	if(!in_array('entnumcpfcnpj', $agrupador) && in_array('entnumcpfcnpj', $colunas) || ($excel && in_array('entnumcpfcnpj', $colunas))){
		$coluna[] = array(
						  "campo" => "entnumcpfcnpj",
				   		  "label" => "CNPJ Município",
						  "type" => "string"
					);
	}
	if(!in_array('entnumcpfcnpjescola', $agrupador) && in_array('entnumcpfcnpjescola', $colunas) || ($excel && in_array('entnumcpfcnpjescola', $colunas))){
		$coluna[] = array(
						  "campo" => "entnumcpfcnpjescola",
				   		  "label" => "CNPJ Escola",
						  "type" => "string"
					);
	}
	if(!in_array('escola', $agrupador) && in_array('escola', $colunas) || ($excel && in_array('escola', $colunas))){
		$coluna[] = array(
						  "campo" => "escola",
				   		  "label" => "Nome da Escola"
					);
	}
	if(!in_array('estuf', $agrupador) && in_array('estuf', $colunas) || ($excel && in_array('estuf', $colunas))){
		$coluna[] = array(
						  "campo" => "estuf",
				   		  "label" => "UF"
					);
	}
	if(!in_array('mundescricao', $agrupador) && in_array('mundescricao', $colunas) || ($excel && in_array('mundescricao', $colunas))){
		$coluna[] = array(
						  "campo" => "mundescricao",
				   		  "label" => "Município"
					);
	}
	if(!in_array('esddsc', $agrupador) && in_array('esddsc', $colunas) || ($excel && in_array('esddsc', $colunas))){
		$coluna[] = array(
						  "campo" => "esddsc",
				   		  "label" => "Situação do Município"
					);
	}
	if(!in_array('nomeprefeito', $agrupador) && in_array('nomeprefeito', $colunas) || ($excel && in_array('nomeprefeito', $colunas))){
		$coluna[] = array(
						  "campo" => "nomeprefeito",
				   		  "label" => "Nome Prefeito"
					);
	}
	if(!in_array('emailprefeito', $agrupador) && in_array('emailprefeito', $colunas) || ($excel && in_array('emailprefeito', $colunas))){
		$coluna[] = array(
						  "campo" => "emailprefeito",
				   		  "label" => "E-mail Prefeito"
					);
	}
	if(!in_array('telefoneprefeito', $agrupador) && in_array('telefoneprefeito', $colunas) || ($excel && in_array('telefoneprefeito', $colunas))){
		$coluna[] = array(
						  "campo" => "telefoneprefeito",
				   		  "label" => "Telefone Prefeito"
					);
	}
	if(!in_array('totalescolapublica', $agrupador) && in_array('totalescolapublica', $colunas) || ($excel && in_array('totalescolapublica', $colunas))){
		$coluna[] = array(
						  "campo" => "totalescolapublica",
				   		  "label" => "Total de escola Pública"
					);
	}
	if(!in_array('totalescolasparticulares', $agrupador) && in_array('totalescolasparticulares', $colunas) || ($excel && in_array('totalescolasparticulares', $colunas))){
		$coluna[] = array(
						  "campo" => "totalescolasparticulares",
				   		  "label" => "Total de escola Particulares"
					);
	}
	if(!in_array('qtdparcialprivada', $agrupador) && in_array('qtdparcialprivada', $colunas) || ($excel && in_array('qtdparcialprivada', $colunas))){
		$coluna[] = array(
						  "campo" => "qtdparcialprivada",
				   		  "label" => "QTD parcial privada"	
					);
	}
	if(!in_array('qtdintegralprivada', $agrupador) && in_array('qtdintegralprivada', $colunas) || ($excel && in_array('qtdintegralprivada', $colunas))){
		$coluna[] = array(
						  "campo" => "qtdintegralprivada",
				   		  "label" => "QTD integral privada"
					);
	}
	if(!in_array('qtdparcialpublica', $agrupador) && in_array('qtdparcialpublica', $colunas) || ($excel && in_array('qtdparcialpublica', $colunas))){
		$coluna[] = array(
						  "campo" => "qtdparcialpublica",
				   		  "label" => "QTD parcial publica"
					);
	}
	if(!in_array('qtdintegralpublica', $agrupador) && in_array('qtdintegralpublica', $colunas) || ($excel && in_array('qtdintegralpublica', $colunas))){
		$coluna[] = array(
						  "campo" => "qtdintegralpublica",
				   		  "label" => "QTD integral publica"
					);
	}
	if(!in_array('vlrrepasseparcialpublica', $agrupador) && in_array('vlrrepasseparcialpublica', $colunas) || ($excel && in_array('vlrrepasseparcialpublica', $colunas))){
		$coluna[] = array(
						  "campo" => "vlrrepasseparcialpublica",
				   		  "label" => "VLR de repasse parcial publica",
						  "type"=>"string"
					);
	}
	if(!in_array('vlrrepasseintegralpublica', $agrupador) && in_array('vlrrepasseintegralpublica', $colunas) || ($excel && in_array('vlrrepasseintegralpublica', $colunas))){
		$coluna[] = array(
						  "campo" => "vlrrepasseintegralpublica",
				   		  "label" => "VLR de repasse integral publica",
						  "type"=>"string"
					);
	}	
	if(!in_array('vlrrepasseparcialprivada', $agrupador) && in_array('vlrrepasseparcialprivada', $colunas) || ($excel && in_array('vlrrepasseparcialprivada', $colunas))){
		$coluna[] = array(
						  "campo" => "vlrrepasseparcialprivada",
				   		  "label" => "VLR de repasse parcial privada",
						  "type"=>"string"
					);
	}
	if(!in_array('vlrrepasseintegralprivada', $agrupador) && in_array('vlrrepasseintegralprivada', $colunas) || ($excel && in_array('vlrrepasseintegralprivada', $colunas))){
		$coluna[] = array(
						  "campo" => "vlrrepasseintegralprivada",
				   		  "label" => "VLR de repasse integral privada",
						  //"blockAgp" => array('mundescricao', 'estuf', 'esddsc')
						  "type"=>"string"
					);
	}
	if(!in_array('total', $agrupador) && in_array('total', $colunas) || ($excel && in_array('total', $colunas))){
		$coluna[] = array(
						  "campo" => "total",
				   		  "label" => "Total"
					);
	}
				
	return $coluna;	
}

// Monta sql do relatorio
function monta_sql()
{
	
	extract($_REQUEST);

	$innerLote = "LEFT ";
	
	// UF
	if( $estuf[0] && $estuf_campo_flag ){
		$arWhere[] = " mun.estuf ". (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $estuf )."' ) ";
	}
	
	// Municipio
	if( $muncod[0] && $muncod_campo_flag ){
		$arWhere[] = " mun.muncod ". (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $muncod )."' ) ";
	}
	
	// Situação
	if( $esdid[0] && $esdid_campo_flag ){
		$arWhere[] = " doc.esdid ". (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $esdid )."' ) ";
	}

	// Lote
	if($lotid[0] && $lotid_campo_flag ){
		$innerLote = "INNER ";
		$arWhere[] = " lot.lotid ". (!$lotid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( '".implode( "', '", $lotid )."' ) ";
	}
	
	//Ano
	if($ano == '2013'){
		$select_ano = "'897,49' as vlrrepasseparcialpublica,
                	   '1.458,41' as vlrrepasseintegralpublica,
                	   '897,49' as vlrrepasseparcialprivada,
                	   '1.234,04' as vlrrepasseintegralprivada,
					   (COALESCE(SUM(quantidadeparcialpublica),0)*897.49) + (COALESCE(SUM(quantidadeparcialparticular),0)*897.49) + (COALESCE(SUM(quantidadeintegralpublica),0)*1458.41) + (COALESCE(SUM(quantidadeintegralparticular),0)*1234.04) as total";
	} elseif($ano == '2012'){
		$select_ano = "'419,34' as vlrrepasseparcialpublica,
                	   '681,42' as vlrrepasseintegralpublica,
                	   '419,34' as vlrrepasseparcialprivada,
                	   '576,59' as vlrrepasseintegralprivada,
					   (COALESCE(SUM(quantidadeparcialpublica),0)*419.34) + (COALESCE(SUM(quantidadeparcialparticular),0)*419.34) + (COALESCE(SUM(quantidadeintegralpublica),0)*681.42) + (COALESCE(SUM(quantidadeintegralparticular),0)*576.59) as total";
	}
		
	$sql = "with temp_ent as (select e.entid, e.entnumcpfcnpj, e.tpcid, e.entnome, e.entcodent, pc.muncod as muncod1, ed.muncod as muncod2, mdsquantidadepbfparcial, mdsquantidadepbfintegral 
						       from proinfantil.procenso pc 
						       inner join entidade.entidade e ON pc.entcodent = e.entcodent inner join entidade.endereco ed ON ed.entid = e.entid 
						       INNER JOIN proinfantil.mdssuplementacao sup on sup.entcodent = e.entcodent and sup.prcid = pc.prcid AND sup.mdsstatus = 'A' where e.tpcid in (1,2,3,4) and pc.prcano = ({$ano}-1)
							)
			
			
			SELECT          cnpjmunicipio as entnumcpfcnpj,
			                escola,
			                cnpjescola as entnumcpfcnpjescola,
			                muncod,
			                estuf,
			                estuf || ' - ' || mundescricao as mundescricao,
			                esddsc,
			                nomeprefeito, 
			                emailprefeito,
			                telefoneprefeito,
			                SUM(totalescolapublica) AS totalescolapublica,
			                SUM(totalescolaparticulares) AS totalescolasparticulares,
			                SUM(quantidadeparcialparticular) AS qtdparcialprivada,
			                SUM(quantidadeintegralparticular) AS qtdintegralprivada,
			                SUM(quantidadeparcialpublica) AS qtdparcialpublica,
			                SUM(quantidadeintegralpublica) AS qtdintegralpublica,
							$select_ano
            FROM (
                SELECT      	pre.cnpjmunicipio,
			                    a.entnome as escola,
			                    a.entnumcpfcnpj as cnpjescola,
			                    mun.muncod,
			                    mun.estuf,
			                    mun.mundescricao,
			                    esd.esddsc,
			        	        pre.nomeprefeito, 
			    	            pre.emailprefeito,
				                pre.telefoneprefeito,                   
			                    CASE WHEN a.tpcid IN (4) THEN coalesce(sum( a.mdsquantidadepbfparcial ),0) END AS quantidadeparcialparticular,
			                    CASE WHEN a.tpcid IN (4) THEN coalesce(sum( a.mdsquantidadepbfintegral ),0) END AS quantidadeintegralparticular,
			                    CASE WHEN a.tpcid IN (1,2,3) THEN coalesce(sum( a.mdsquantidadepbfparcial ),0) END AS quantidadeparcialpublica,
			                    CASE WHEN a.tpcid IN (1,2,3) THEN coalesce(sum( a.mdsquantidadepbfintegral ),0) END AS quantidadeintegralpublica,
			                    CASE WHEN a.tpcid IN (1,2,3) THEN coalesce(count( a.mdsquantidadepbfintegral ),0) else 0 END AS totalescolapublica,
			                    CASE WHEN a.tpcid IN (4) THEN coalesce(count( a.mdsquantidadepbfintegral ),0) else 0 END AS totalescolaparticulares
                FROM       		territorios.municipio mun
                INNER JOIN      proinfantil.mdsdadoscriancapormunicipio dcm ON dcm.muncod = mun.muncod AND dcm.cpmano = ({$ano}-1)
                {$innerLote} JOIN proinfantil.lote lot ON dcm.lotid = lot.lotid
                LEFT JOIN       workflow.documento doc ON doc.docid = dcm.docid
                LEFT JOIN       workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN (SELECT 			en.muncod, e2.entnumcpfcnpj as cnpjmunicipio, e.entnome as nomeprefeito, e.entemail as emailprefeito, '(' || e.entnumdddcomercial || ')' || e.entnumcomercial as telefoneprefeito    from entidade.entidade e 
							INNER JOIN 		entidade.funcaoentidade f ON f.entid = e.entid AND f.funid=2 and f.fuestatus = 'A'
							INNER JOIN 		entidade.funentassoc ff ON ff.fueid = f.fueid 
							INNER JOIN 		entidade.entidade e2 ON e2.entid = ff.entid 
							INNER JOIN 		entidade.funcaoentidade f2 ON f2.entid = e2.entid AND f2.funid=1 and f2.fuestatus = 'A'
							INNER JOIN 		entidade.endereco en ON en.entid = e2.entid 
							WHERE 			e.entstatus='A'
							GROUP BY 		en.muncod, e2.entnumcpfcnpj, e.entnome, e.entemail, e.entnumdddcomercial, e.entnumcomercial
               			) AS pre ON pre.muncod = mun.muncod                        
  				/*INNER JOIN ( select e.entid, e.entnumcpfcnpj, e.tpcid, e.entnome, e.entcodent, pc.muncod as muncod1, ed.muncod as muncod2, mdsquantidadepbfparcial, mdsquantidadepbfintegral
						    from proinfantil.procenso pc
						    inner join entidade.entidade e ON pc.entcodent = e.entcodent
						    inner join entidade.endereco ed ON ed.entid = e.entid
						    INNER JOIN proinfantil.mdssuplementacao sup on sup.entcodent = e.entcodent and sup.prcid = pc.prcid AND sup.mdsstatus = 'A' 
						    where e.tpcid in (1,2,3,4) and pc.prcano = ({$ano}-1) ) a ON a.muncod1 = dcm.muncod*/
				
				INNER JOIN ( select entid, entnumcpfcnpj, tpcid, entnome, entcodent, muncod1, muncod2, mdsquantidadepbfparcial, mdsquantidadepbfintegral from temp_ent ) a ON a.muncod1 = dcm.muncod
				
				".( is_array($arWhere) ? ' WHERE ' . implode(' AND ', $arWhere) : '' )." 
                GROUP BY
                    a.entnumcpfcnpj, a.entnome,mun.muncod,mun.estuf,mun.mundescricao,esd.esddsc,a.tpcid,a.entnome, pre.cnpjmunicipio, pre.nomeprefeito, pre.emailprefeito, pre.telefoneprefeito  
            ) AS foo
            GROUP BY
                cnpjescola, cnpjmunicipio, escola, muncod, estuf, mundescricao, esddsc, nomeprefeito, emailprefeito, telefoneprefeito  
            ORDER BY
                estuf,mundescricao,escola ";
	
	$arAgrupadores = monta_agp();
	return $sql;
}
?>
<html>
	<head>
		<title>Relatório Geral - Suplementação de Creches MDS</title>
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
//			$r->setMonstrarTolizadorNivel(true);
			$r->setBrasao(true);
			$r->setEspandir( $_REQUEST['expandir'] );
			$r->setTotalizador(true);
			echo $r->getRelatorio(); ?>
	</body>
</html>