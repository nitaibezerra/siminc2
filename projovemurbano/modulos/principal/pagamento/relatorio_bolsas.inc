<?php

ini_set("memory_limit", "2048M");
// set_time_limit(120);

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$superuser = $db->testa_superuser();
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql($superuser);
$dados = $db->carregar($sql);

?>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
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
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(false);
$r->setBrasao(true);
$r->setEspandir( $_REQUEST['expandir']);
echo $r->getRelatorio();

function monta_sql( $superuser ){
	
	global $db;
	
	extract($_GET);
// 	ver($_GET,d);	
	// monta o sql
	if($_GET['tipo'] == 'pendente'){ 
		$sql = "
				SELECT DISTINCT
					COALESCE(pju.estuf,mun.estuf) as estuf,
					CASE WHEN pju.muncod IS NULL
						THEN 'Estadual'
						ELSE 'Municipal'
					END as esfera,
					coalesce(mun.mundescricao,'Esfera Estadual') as mundescricao,
					COALESCE('POLO '||cae.polid,'Não Possui') as polo,
					'NÚCLEO '||nuc.nucid as nucleo,
					tur.turdesc as turma,
					dia.diabolsaspendentes as qtd
				FROM projovemurbano.pagamentoestudante pge
				INNER JOIN projovemurbano.cadastroestudante cae        ON pge.caeid = cae.caeid
				INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid 
				INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
				INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
				INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid 
				INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
				LEFT JOIN projovemurbano.municipio muni 	 ON muni.munid = nuc.munid
				LEFT JOIN territorios.municipio mun 		 ON mun.muncod = pju.muncod
				WHERE 
					pge.pgeaptoreceber = 't' 
				AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0 
				AND per.perid = {$perid}
				AND dia.diabolsaspendentes is not null
				AND dia.diabolsaspendentes != 0
				ORDER BY 
					esfera,
					estuf, 
					mundescricao, 
					polo, 
					nucleo,
					turma
				";
	}elseif($_GET['tipo'] == 'autorizado'){
		$sql = "
				SELECT DISTINCT
					COALESCE(pju.estuf,mun.estuf) as estuf,
					CASE WHEN pju.muncod IS NULL
					THEN 'Estadual'
					ELSE 'Municipal'
					END as esfera,
					coalesce(mun.mundescricao,'Esfera Estadual') as mundescricao,
					COALESCE('POLO '||cae.polid,'Não Possui') as polo,
					'NÚCLEO '||nuc.nucid as nucleo,
					tur.turdesc as turma,
					dia.diabolsasautorizadas as qtd
				FROM projovemurbano.pagamentoestudante pge
				INNER JOIN projovemurbano.cadastroestudante cae        ON pge.caeid = cae.caeid
				INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid
				INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
				INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
				INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid
				INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
				LEFT JOIN projovemurbano.municipio muni 	 ON muni.munid = nuc.munid
				LEFT JOIN territorios.municipio mun 		 ON mun.muncod = pju.muncod
				WHERE
					pge.pgeaptoreceber = 't'
				AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
				AND per.perid = {$perid}
				AND dia.diabolsasautorizadas is not null
				AND dia.diabolsasautorizadas != 0
				ORDER BY
					esfera,
					estuf,
					mundescricao,
					polo,
					nucleo,
					turma
			";
	}elseif($_GET['tipo'] == 'enviado'){
		$sql = "
				SELECT DISTINCT
					COALESCE(pju.estuf,mun.estuf) as estuf,
					CASE WHEN pju.muncod IS NULL
					THEN 'Estadual'
					ELSE 'Municipal'
					END as esfera,
					coalesce(mun.mundescricao,'Esfera Estadual') as mundescricao,
					COALESCE('POLO '||cae.polid,'Não Possui') as polo,
					'NÚCLEO '||nuc.nucid as nucleo,
					tur.turdesc as turma,
					dia.diabolsasenviadas as qtd
				FROM projovemurbano.pagamentoestudante pge
				INNER JOIN projovemurbano.cadastroestudante cae        ON pge.caeid = cae.caeid
				INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid
				INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
				INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
				INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid
				INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
				LEFT JOIN projovemurbano.municipio muni 	 ON muni.munid = nuc.munid
				LEFT JOIN territorios.municipio mun 		 ON mun.muncod = pju.muncod
				WHERE
					pge.pgeaptoreceber = 't'
				AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
				AND per.perid = {$perid}
				AND dia.diabolsasenviadas is not null
				AND dia.diabolsasenviadas != 0
				ORDER BY
					esfera,
					estuf,
					mundescricao,
					polo,
					nucleo,
					turma
			";
	}elseif($_GET['tipo'] == 'rejeitado'){
		$sql = "
				SELECT DISTINCT
					COALESCE(pju.estuf,mun.estuf) as estuf,
					CASE WHEN pju.muncod IS NULL
					THEN 'Estadual'
					ELSE 'Municipal'
					END as esfera,
					coalesce(mun.mundescricao,'Esfera Estadual') as mundescricao,
					COALESCE('POLO '||cae.polid,'Não Possui') as polo,
					'NÚCLEO '||nuc.nucid as nucleo,
					tur.turdesc as turma,
					dia.diabolsasrecusadas as qtd
				FROM projovemurbano.pagamentoestudante pge
				INNER JOIN projovemurbano.cadastroestudante cae        ON pge.caeid = cae.caeid
				INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid
				INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
				INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
				INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid
				INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
				LEFT JOIN projovemurbano.municipio muni 	 ON muni.munid = nuc.munid
				LEFT JOIN territorios.municipio mun 		 ON mun.muncod = pju.muncod
				WHERE
					pge.pgeaptoreceber = 't'
				AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
				AND per.perid = {$perid}
				AND dia.diabolsasrecusadas is not null
				AND dia.diabolsasrecusadas != 0
				ORDER BY
					esfera,
					estuf,
					mundescricao,
					polo,
					nucleo,
					turma
			";
	}
// 	ver($sql,d);
	return $sql;
}

function monta_agp(){
	
	$agp = array("agrupador" => array(),
				 "agrupadoColuna" => array( "documento",
											"qtd") );
	
	array_push($agp['agrupador'], array("campo" => "esfera",
								 		"label" => "Esfera") );	
	
	array_push($agp['agrupador'], array("campo" => "estuf",
								 		"label" => "Estado") );	
					
	array_push($agp['agrupador'], array("campo" => "mundescricao",
								 		"label" => "Municipio") );
						
	array_push($agp['agrupador'], array("campo" => "polo",
										"label" => "Polo") );
			
	array_push($agp['agrupador'], array("campo" => "nucleo",
										"label" => "Nucleo") );
	
	array_push($agp['agrupador'], array("campo" => "turma",
										"label" => "Turma") );
	
	return $agp;
}

function monta_coluna(){
	
	$coluna = array(	
					array(
						  "campo" => "qtd",
				   		  "label" => "Total",
					   	  "type"  => "numeric"	
					),		
					
			);
					  	
	return $coluna;			  	
}
?>
</body>
</html>