<?php

ini_set( "memory_limit", "512M" );
set_time_limit(0);

include APPRAIZ. 'includes/classes/relatorio.class.inc';

?>

<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
<?php

$groupBy = array();
$orderBy = array();
$colunas = array();
$select  = array();
$join 	 = array();
$where	 = array();

$orgids	 = array();

// Função para montar o array com os agrupadores
function monta_agp( $orgid, $agrupador )
{
	if( $orgid )
	{
		global $groupBy,$orderBy,$colunas,$orgids;
		
		foreach($orgid as $orgao)
		{
			switch( (integer)$orgao )
			{
				case ORGID_EDUCACAO_SUPERIOR:
					$colunas[] = "educacao_superior";
					$orgids[] = ORGID_EDUCACAO_SUPERIOR;
					monta_join('educacao_superior');
					break;
				case ORGID_EDUCACAO_PROFISSIONAL:
					$colunas[] = "educacao_profissional";
					$orgids[] = ORGID_EDUCACAO_PROFISSIONAL;
					monta_join('educacao_profissional');
					break;
				case ORGID_EDUCACAO_BASICA:
					$colunas[] = "educacao_basica";
					$orgids[] = ORGID_EDUCACAO_BASICA;
					monta_join('educacao_basica');
					break;
				case ORGID_ADMINSTRATIVO:
					$colunas[] = "administrativo";
					$orgids[] = ORGID_ADMINSTRATIVO;
					monta_join('administrativo');
					break;
				case ORGID_HOSPITAIS:
					$colunas[] = "hospitais";
					$orgids[] = ORGID_HOSPITAIS;
					monta_join('hospitais');
					break;
			}
		}
		
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => $colunas
					);

		foreach($agrupador as $agrup)
		{
			switch($agrup)
			{
				case 'regiao':
					array_push($agp['agrupador'], array("campo" => "regiao", "label" => "Região"));
					$groupBy[] = "regiao";
					$orderBy[] = "regiao";
					monta_join('regiao');
					break;
				case 'municipio':
					array_push($agp['agrupador'], array("campo" => "municipio", "label" => "Município"));
					$groupBy[] = "municipio";
					$orderBy[] = "municipio";
					monta_join('municipio');
					break;
				case 'situacaoobra':
					array_push($agp['agrupador'], array("campo" => "situacaoobra", "label" => "Situação da Obra"));
					$groupBy[] = "situacaoobra";
					$orderBy[] = "situacaoobra";
					monta_join('situacaoobra');
					break;
				case 'situacaogrupo':
					array_push($agp['agrupador'], array("campo" => "situacaogrupo", "label" => "Situação do Grupo"));
					$groupBy[] = "situacaogrupo";
					$orderBy[] = "situacaogrupo";
					monta_join('situacaogrupo');
					break;
				case 'empresa':
					array_push($agp['agrupador'], array("campo" => "empresa", "label" => "Empresa"));
					$groupBy[] = "empresa";
					$orderBy[] = "empresa";
					monta_join('empresa');
					break;
				case 'uf':
					array_push($agp['agrupador'], array("campo" => "uf", "label" => "UF"));
					$groupBy[] = "uf";
					$orderBy[] = "uf";
					monta_join('uf');
					break;
				case 'mesorregiao':
					array_push($agp['agrupador'], array("campo" => "mesorregiao", "label" => "Mesorregião"));
					$groupBy[] = "mesorregiao";
					$orderBy[] = "mesorregiao";
					monta_join('mesorregiao');
					break;
				case 'tipoensino':
					array_push($agp['agrupador'], array("campo" => "tipoensino", "label" => "Tipo de Ensino"));
					$groupBy[] = "tipoensino";
					$orderBy[] = "tipoensino";
					monta_join('tipoensino');
					break;
				case 'brasil':
					array_push($agp['agrupador'], array("campo" => "brasil", "label" => "Brasil"));
					$groupBy[] = "brasil";
					$orderBy[] = "brasil";
					monta_join('brasil');
					break;
				case 'unidade':
					array_push($agp['agrupador'], array("campo" => "unidade", "label" => "Unidade"));
					$groupBy[] = "unidade";
					$orderBy[] = "unidade";
					monta_join('unidade');
					break;
				case 'campus':
					array_push($agp['agrupador'], array("campo" => "campus", "label" => "Campus"));
					$groupBy[] = "campus";
					$orderBy[] = "campus";
					monta_join('campus');
					break;
				case 'nomeobra':
					array_push($agp['agrupador'], array("campo" => "nomeobra", "label" => "Nome da Obra"));
					$groupBy[] = "nomeobra";
					$orderBy[] = "nomeobra";
					monta_join('nomeobra');
					break;
				case 'grupo':
					array_push($agp['agrupador'], array("campo" => "grupo", "label" => "Grupo"));
					$groupBy[] = "grupo";
					$orderBy[] = "grupo";
					monta_join('grupo');
					break;
				case 'nivelsupervisao':
					array_push($agp['agrupador'], array("campo" => "nivelsupervisao", "label" => "Nível do Estado da Supervisão (Obra)"));
					$groupBy[] = "nivelsupervisao";
					$orderBy[] = "nivelsupervisao";
					monta_join('nivelsupervisao');
					break;
				case 'nivelsupervisaogrupo':
					array_push($agp['agrupador'], array("campo" => "nivelsupervisaogrupo", "label" => "Nível do Estado da Supervisão (Grupo)"));
					$groupBy[] = "nivelsupervisaogrupo";
					$orderBy[] = "nivelsupervisaogrupo";
					monta_join('nivelsupervisaogrupo');
					break;
			}
			
		}
		
		return $agp;
	}
}

// Função para montar as colunas do relatório
function monta_coluna($orgid)
{
	if( $orgid )
	{
		$coluna = array();
		
		foreach($orgid as $orgao)
		{
			switch( (integer)$orgao )
			{
				case ORGID_EDUCACAO_SUPERIOR:
					$coluna[] = array("campo" => "educacao_superior", "label" => "Educação Superior", "type"=> "numeric");
					break;
				case ORGID_EDUCACAO_PROFISSIONAL:
					$coluna[] = array("campo" => "educacao_profissional", "label" => "Educação Profissional", "type"=> "numeric");
					break;
				case ORGID_EDUCACAO_BASICA:
					$coluna[] = array("campo" => "educacao_basica", "label" => "Educação Básica", "type"=> "numeric");
					break;
				case ORGID_ADMINSTRATIVO:
					$coluna[] = array("campo" => "administrativo", "label" => "Administrativo", "type"=> "numeric");
					break;
				case ORGID_HOSPITAIS:
					$coluna[] = array("campo" => "hospitais", "label" => "Hospitais", "type"=> "numeric");
					break;
			}
		}
		
		return $coluna;
	}
}

// Função para montar o array com os JOIN's necessários
function monta_join($tipo)
{
	global $join;
	
	switch( $tipo )
	{
		case 'educacao_superior':
			if( !in_array("LEFT JOIN obras.obrainfraestrutura obr2 ON obr2.obrid = obr.obrid AND obr2.orgid = ".ORGID_EDUCACAO_SUPERIOR."", $join) ) { $join[] = "LEFT JOIN obras.obrainfraestrutura obr2 ON obr2.obrid = obr.obrid AND obr2.orgid = ".ORGID_EDUCACAO_SUPERIOR.""; }
			break;
		
		case 'educacao_profissional':
			if( !in_array("LEFT JOIN obras.obrainfraestrutura obr3 ON obr3.obrid = obr.obrid AND obr3.orgid = ".ORGID_EDUCACAO_PROFISSIONAL."", $join) ) { $join[] = "LEFT JOIN obras.obrainfraestrutura obr3 ON obr3.obrid = obr.obrid AND obr3.orgid = ".ORGID_EDUCACAO_PROFISSIONAL.""; }
			break;
			
		case 'educacao_basica':
			if( !in_array("LEFT JOIN obras.obrainfraestrutura obr4 ON obr4.obrid = obr.obrid AND obr4.orgid = ".ORGID_EDUCACAO_BASICA."", $join) ) { $join[] = "LEFT JOIN obras.obrainfraestrutura obr4 ON obr4.obrid = obr.obrid AND obr4.orgid = ".ORGID_EDUCACAO_BASICA.""; }
			break;
			
		case 'administrativo':
			if( !in_array("LEFT JOIN obras.obrainfraestrutura obr5 ON obr5.obrid = obr.obrid AND obr5.orgid = ".ORGID_ADMINSTRATIVO."", $join) ) { $join[] = "LEFT JOIN obras.obrainfraestrutura obr5 ON obr5.obrid = obr.obrid AND obr5.orgid = ".ORGID_ADMINSTRATIVO.""; }
			break;
			
		case 'hospitais':
			if( !in_array("LEFT JOIN obras.obrainfraestrutura obr6 ON obr6.obrid = obr.obrid AND obr6.orgid = ".ORGID_HOSPITAIS."", $join) ) { $join[] = "LEFT JOIN obras.obrainfraestrutura obr6 ON obr6.obrid = obr.obrid AND obr6.orgid = ".ORGID_HOSPITAIS.""; }
			break;
		
		case 'regiao':
			if( !in_array("INNER JOIN entidade.endereco ende ON ende.endid = obr.endid", $join) ) { $join[] = "INNER JOIN entidade.endereco ende ON ende.endid = obr.endid"; }
			if( !in_array("INNER JOIN territorios.estado est ON est.estuf = ende.estuf", $join) ) { $join[] = "INNER JOIN territorios.estado est ON est.estuf = ende.estuf"; }
			if( !in_array("INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod", $join) ) { $join[] = "INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod"; }
			break;
			
		case 'municipio':
			if( !in_array("INNER JOIN entidade.endereco ende ON ende.endid = obr.endid", $join) ) { $join[] = "INNER JOIN entidade.endereco ende ON ende.endid = obr.endid"; }
			if( !in_array("INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod", $join) ) { $join[] = "INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod"; }
			break;
			
		case 'situacaoobra':
			if( !in_array("LEFT JOIN workflow.documento doc ON doc.docid = obr.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc ON doc.docid = obr.docid"; }
			if( !in_array("LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid", $join) ) { $join[] = "LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid"; }
			break;
			
		case 'situacaogrupo':
			if( !in_array("INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid", $join) ) { $join[] = "INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid"; }
			if( !in_array("INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid", $join) ) { $join[] = "INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid"; }
			if( !in_array("INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid", $join) ) { $join[] = "INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid"; }
			if( !in_array("INNER JOIN workflow.documento doc2 ON doc2.docid = gpd.docid", $join) ) { $join[] = "INNER JOIN workflow.documento doc2 ON doc2.docid = gpd.docid"; }
			if( !in_array("INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid", $join) ) { $join[] = "INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid"; }
			break;
			
		case 'empresa':
			if( !in_array("INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid", $join) ) { $join[] = "INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid"; }
			if( !in_array("INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid", $join) ) { $join[] = "INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid"; }
			if( !in_array("INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid", $join) ) { $join[] = "INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid"; }
			if( !in_array("INNER JOIN obras.empresacontratada epc ON epc.epcid = gpd.epcid", $join) ) { $join[] = "INNER JOIN obras.empresacontratada epc ON epc.epcid = gpd.epcid"; }
			if( !in_array("INNER JOIN entidade.entidade ent ON ent.entid = epc.entid", $join) ) { $join[] = "INNER JOIN entidade.entidade ent ON ent.entid = epc.entid"; }
			break;
			
		case 'uf':
			if( !in_array("INNER JOIN entidade.endereco ende ON ende.endid = obr.endid", $join) ) { $join[] = "INNER JOIN entidade.endereco ende ON ende.endid = obr.endid"; }
			if( !in_array("INNER JOIN territorios.estado est ON est.estuf = ende.estuf", $join) ) { $join[] = "INNER JOIN territorios.estado est ON est.estuf = ende.estuf"; }
			break;
			
		case 'mesorregiao':
			if( !in_array("INNER JOIN entidade.endereco ende ON ende.endid = obr.endid", $join) ) { $join[] = "INNER JOIN entidade.endereco ende ON ende.endid = obr.endid"; }
			if( !in_array("INNER JOIN territorios.mesoregiao mes ON mes.estuf = ende.estuf", $join) ) { $join[] = "INNER JOIN territorios.mesoregiao mes ON mes.estuf = ende.estuf"; }
			break;
			
		case 'tipoensino':
			if( !in_array("INNER JOIN obras.orgao org ON org.orgid = obr.orgid", $join) ) { $join[] = "INNER JOIN obras.orgao org ON org.orgid = obr.orgid"; }
			break;
			
		case 'brasil':
			if( !in_array("INNER JOIN entidade.endereco ende ON ende.endid = obr.endid", $join) ) { $join[] = "INNER JOIN entidade.endereco ende ON ende.endid = obr.endid"; }
			if( !in_array("INNER JOIN territorios.estado est ON est.estuf = ende.estuf", $join) ) { $join[] = "INNER JOIN territorios.estado est ON est.estuf = ende.estuf"; }
			if( !in_array("INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod", $join) ) { $join[] = "INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod"; }
			if( !in_array("INNER JOIN territorios.pais pai ON pai.paiid = reg.paiid", $join) ) { $join[] = "INNER JOIN territorios.pais pai ON pai.paiid = reg.paiid"; }
			break;
			
		case 'unidade':
			if( !in_array("INNER JOIN entidade.entidade ent2 ON ent2.entid = obr.entidunidade", $join) ) { $join[] = "INNER JOIN entidade.entidade ent2 ON ent2.entid = obr.entidunidade"; }
			break;
			
		case 'campus':
			if( !in_array("INNER JOIN entidade.entidade ent3 ON ent3.entid = obr.entidcampus", $join) ) { $join[] = "INNER JOIN entidade.entidade ent3 ON ent3.entid = obr.entidcampus"; }
			break;
			
		case 'nomeobra':
			if( !in_array("LEFT JOIN workflow.documento doc3 ON doc3.docid = obr.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc3 ON doc3.docid = obr.docid"; }
			if( !in_array("LEFT JOIN workflow.historicodocumento hst3 ON hst3.docid = doc3.docid AND (hst3.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc3.docid) OR hst3.htddata IS NULL )", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hst3 ON hst3.docid = doc3.docid AND (hst3.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc3.docid) OR hst3.htddata IS NULL )"; }
			break;
			
		case 'grupo':
			if( !in_array("INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid", $join) ) { $join[] = "INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid"; }
			if( !in_array("INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid", $join) ) { $join[] = "INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid"; }
			if( !in_array("INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid", $join) ) { $join[] = "INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid"; }
			if( !in_array("LEFT JOIN workflow.documento doc4 ON doc4.docid = gpd.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc4 ON doc4.docid = gpd.docid"; }
			if( !in_array("LEFT JOIN workflow.historicodocumento hst4 ON hst4.docid = doc4.docid AND (hst4.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc4.docid) OR hst4.htddata IS NULL )", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hst4 ON hst4.docid = doc4.docid AND (hst4.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc4.docid) OR hst4.htddata IS NULL )"; }
			if( !in_array("LEFT JOIN workflow.estadodocumento esd4 ON esd4.esdid = doc4.esdid", $join) ) { $join[] = "LEFT JOIN workflow.estadodocumento esd4 ON esd4.esdid = doc4.esdid"; }
			if( !in_array("LEFT JOIN workflow.documento doc5 ON doc5.docid = gpd.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc5 ON doc5.docid = gpd.docid"; }
			if( !in_array("LEFT JOIN workflow.historicodocumento hst5 ON hst5.docid = doc5.docid AND hst5.aedid = ".GRUPOLIBERADOPARASUPERVISAO." AND (hst5.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc5.docid AND h.aedid = ".GRUPOLIBERADOPARASUPERVISAO.") OR hst5.htddata IS NULL )", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hst5 ON hst5.docid = doc5.docid AND hst5.aedid = ".GRUPOLIBERADOPARASUPERVISAO." AND (hst5.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc5.docid AND h.aedid = ".GRUPOLIBERADOPARASUPERVISAO.") OR hst5.htddata IS NULL )"; }
			break;
		
		case 'nivelsupervisao':
			if( !in_array("LEFT JOIN workflow.documento doc ON doc.docid = obr.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc ON doc.docid = obr.docid"; }
			if( !in_array("LEFT JOIN workflow.historicodocumento hst ON hst.docid = doc.docid AND hst.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid)", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hst ON hst.docid = doc.docid AND hst.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid)"; }
			if( !in_array("LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid", $join) ) { $join[] = "LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid"; }
			break;
			
		case 'nivelsupervisaogrupo':
			if( !in_array("INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid", $join) ) { $join[] = "INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid"; }
			if( !in_array("INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid", $join) ) { $join[] = "INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid"; }
			if( !in_array("INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid", $join) ) { $join[] = "INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid"; }
			if( !in_array("LEFT JOIN workflow.documento doc6 ON doc6.docid = gpd.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc6 ON doc6.docid = gpd.docid"; }
			if( !in_array("LEFT JOIN workflow.historicodocumento hst6 ON hst6.docid = doc6.docid AND hst6.htddata = (SELECT max(h6.htddata) FROM workflow.historicodocumento h6 WHERE h6.docid = doc6.docid)", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hst6 ON hst6.docid = doc6.docid AND hst6.htddata = (SELECT max(h6.htddata) FROM workflow.historicodocumento h6 WHERE h6.docid = doc6.docid)"; }
			if( !in_array("LEFT JOIN workflow.estadodocumento esd6 ON esd6.esdid = doc6.esdid", $join) ) { $join[] = "LEFT JOIN workflow.estadodocumento esd6 ON esd6.esdid = doc6.esdid"; }
			if( !in_array("LEFT JOIN workflow.documento doc7 ON doc7.docid = gpd.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc7 ON doc7.docid = gpd.docid"; }
			if( !in_array("LEFT JOIN workflow.historicodocumento hst7 ON hst7.docid = doc7.docid AND hst7.aedid = ".GRUPOLIBERADOPARASUPERVISAO." AND (hst7.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc7.docid AND h.aedid = ".GRUPOLIBERADOPARASUPERVISAO.") OR hst7.htddata IS NULL )", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hst7 ON hst7.docid = doc7.docid AND hst7.aedid = ".GRUPOLIBERADOPARASUPERVISAO." AND (hst7.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc7.docid AND h.aedid = ".GRUPOLIBERADOPARASUPERVISAO.") OR hst7.htddata IS NULL )"; }
			break;
			
		case 'tramitacao_grupo':
			if( !in_array("INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid", $join) ) { $join[] = "INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid"; }
			if( !in_array("INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid", $join) ) { $join[] = "INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid"; }
			if( !in_array("INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid", $join) ) { $join[] = "INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid"; }
			if( !in_array("INNER JOIN workflow.documento doc2 ON doc2.docid = gpd.docid", $join) ) { $join[] = "INNER JOIN workflow.documento doc2 ON doc2.docid = gpd.docid"; }
			if( !in_array("INNER JOIN workflow.historicodocumento hst2 ON hst2.docid = doc2.docid AND hst2.htddata = (SELECT max(h2.htddata) FROM workflow.historicodocumento h2 WHERE h2.docid = doc2.docid)", $join) ) { $join[] = "INNER JOIN workflow.historicodocumento hst2 ON hst2.docid = doc2.docid AND hst2.htddata = (SELECT max(h2.htddata) FROM workflow.historicodocumento h2 WHERE h2.docid = doc2.docid)"; }
			break;
			
		case 'os_grupo':
			if( !in_array("INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid", $join) ) { $join[] = "INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid"; }
			if( !in_array("INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid", $join) ) { $join[] = "INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid"; }
			if( !in_array("INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid", $join) ) { $join[] = "INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid"; }
			if( !in_array("INNER JOIN obras.ordemservico os ON gpd.gpdid = os.gpdid AND os.orsstatus = 'A'", $join) ) { $join[] = "INNER JOIN obras.ordemservico os ON gpd.gpdid = os.gpdid AND os.orsstatus = 'A'"; }
			break;
			
		case 'tramitacao_obra':
			if( !in_array("INNER JOIN workflow.documento doc ON doc.docid = obr.docid", $join) ) { $join[] = "INNER JOIN workflow.documento doc ON doc.docid = obr.docid"; }
			if( !in_array("INNER JOIN workflow.historicodocumento hst ON hst.docid = doc.docid AND hst.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid)", $join) ) { $join[] = "INNER JOIN workflow.historicodocumento hst ON hst.docid = doc.docid AND hst.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc.docid)"; }
			break;
			
		case 'checklist':
			if( !in_array("INNER JOIN obras.checklistvistoria chk ON chk.obrid = obr.obrid AND chk.chkstatus = 'A'", $join) ) { $join[] = "INNER JOIN obras.checklistvistoria chk ON chk.obrid = obr.obrid AND chk.chkstatus = 'A'"; }
			break;
			
		case 'parecer':
			if( !in_array("INNER JOIN obras.checklistvistoria chk ON chk.obrid = obr.obrid AND chk.chkstatus = 'A'", $join) ) { $join[] = "INNER JOIN obras.checklistvistoria chk ON chk.obrid = obr.obrid AND chk.chkstatus = 'A'"; }
			if( !in_array("INNER JOIN obras.movparecercklist mpc ON mpc.chkid = chk.chkid AND mpc.mpcdtinclusao = (SELECT max(mpc2.mpcdtinclusao) FROM obras.movparecercklist mpc2 WHERE mpc2.chkid = chk.chkid AND mpc2.mpcstatus = 'A') AND mpc.mpcstatus = 'A'", $join) ) { $join[] = "INNER JOIN obras.movparecercklist mpc ON mpc.chkid = chk.chkid AND mpc.mpcdtinclusao = (SELECT max(mpc2.mpcdtinclusao) FROM obras.movparecercklist mpc2 WHERE mpc2.chkid = chk.chkid AND mpc2.mpcstatus = 'A') AND mpc.mpcstatus = 'A'"; }
			break;
	}
}

function monta_select()
{
	global $colunas,$select,$groupBy;
	
	foreach($colunas as $c)
	{
		switch($c)
		{
			case 'educacao_superior':
				$select[] = 'count(obr2.obrid) as educacao_superior';
				break;
			case 'educacao_profissional':
				$select[] = 'count(obr3.obrid) as educacao_profissional';
				break;
			case 'educacao_basica':
				$select[] = 'count(obr4.obrid) as educacao_basica';
				break;
			case 'administrativo':
				$select[] = 'count(obr5.obrid) as administrativo';
				break;
			case 'hospitais':
				$select[] = 'count(obr6.obrid) as hospitais';
				break;
		}
	}
	
	foreach($groupBy as $g)
	{
		switch($g)
		{
			case 'regiao':
				$select[] = 'reg.regdescricao as regiao';
				break;
			case 'municipio':
				$select[] = 'mun.mundescricao as municipio';
				break;
			case 'situacaoobra':
				$select[] = 'CASE WHEN esd.esddsc IS NOT NULL
								  THEN  cast(esd.esddsc AS varchar)
								  ELSE \' Aguardando Início de Tramitação\'
						     END AS situacaoobra';
				break;
			case 'situacaogrupo':
				$select[] = 'esd2.esddsc as situacaogrupo';
				break;
			case 'empresa':
				$select[] = 'ent.entnome as empresa';
				break;
			case 'uf':
				$select[] = 'est.estdescricao as uf';
				break;
			case 'mesorregiao':
				$select[] = 'mes.mesdsc as mesorregiao';
				break;
			case 'tipoensino':
				$select[] = 'org.orgdesc as tipoensino';
				break;
			case 'brasil':
				$select[] = 'pai.paidescricao as brasil';
				break;
			case 'unidade':
				$select[] = 'ent2.entnome as unidade';
				break;
			case 'campus':
				$select[] = 'ent3.entnome as campus';
				break;
			case 'nomeobra':
				$select[] = "CASE WHEN doc3.docid IS NULL 
							           THEN '(' || obr.obrid || ') ' || obr.obrdesc 
							      WHEN hst3.htddata IS NOT NULL
							           THEN '(' || obr.obrid || ') ' || obr.obrdesc || ' (n° dias ' || DATE_PART('days', NOW() - (to_char(hst3.htddata, 'YYYY-mm-dd'))::timestamp) || ')' 
							      ELSE '(' || obr.obrid || ') ' || obr.obrdesc 
			     			 END as nomeobra";
				break;
			case 'grupo':
				$select[] = "CASE WHEN doc4.docid is null THEN gpd.gpdid::varchar
							 ELSE
							 (
							'<FONT '||

                             /*Situação: Grupo em Supervisão*/

                             CASE WHEN esd4.esdid = ".GRUPOEMSUPERVISAO."  AND DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) > 90

                                           THEN 'COLOR=\"#DD0000\" />'||cast(gpd.gpdid as varchar) || ' (n° dias ' || DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) || ' / ' || DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) || ')'

                                    /* Situação: Aguardando Início de Supervisão pela Empresa */

                                    WHEN esd4.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) <= 20

                                           THEN 'COLOR=\"#008000\" />'||cast(gpd.gpdid as varchar) || ' (n° dias ' || DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) || ' / ' || DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) || ')'

                                    WHEN esd4.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) <= 24

                                           THEN 'COLOR=\"#BB9900\" />'||cast(gpd.gpdid as varchar) || ' (n° dias ' || DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) || ' / ' || DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) || ')'

                                    WHEN esd4.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) > 24

                                           THEN 'COLOR=\"#DD0000\" />'||cast(gpd.gpdid as varchar) || ' (n° dias ' || DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) || ' / ' || DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) || ')'

                                    ELSE/* As demais Situações do Grupo */

                                               CASE WHEN esd4.esddsc IS NOT NULL AND hst5.htddata IS NOT NULL AND hst4.htddata IS NOT NULL

													     THEN 'COLOR=\"#000000\" />'||cast(gpd.gpdid as varchar) || ' (n° dias ' || DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) || ' / ' || DATE_PART('days', NOW() - (to_char(hst5.htddata, 'YYYY-mm-dd'))::timestamp) || ')'
								
												    WHEN esd4.esddsc IS NOT NULL AND hst4.htddata IS NOT NULL
								
									                     THEN 'COLOR=\"#000000\" />'||cast(gpd.gpdid as varchar) || ' (n° dias ' || DATE_PART('days', NOW() - (to_char(hst4.htddata, 'YYYY-mm-dd'))::timestamp) || ' / - )'
													 	
									                ELSE 'COLOR=\"#000000\" />'||cast(gpd.gpdid as varchar) || ' ( n° dias - / -  )'	
											
									           END
									           
                             END ||'</FONT>') END as grupo";
				break;
			case 'nivelsupervisao':
				$select[] = "'<FONT ' ||

                             /* Situação: Em Supervisão */

                             CASE WHEN esd.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp)<= 20

                                         THEN 'COLOR=\"#008000\" />'||esd.esddsc

                              WHEN esd.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 30

                                         THEN 'COLOR=\"#BB9900\" />'||esd.esddsc

                                  WHEN esd.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 30

                                         THEN 'COLOR=\"#DD0000\" />'||esd.esddsc

                                   ELSE /* Situação: Em Avaliação de Supervisão(MEC) */

                                        CASE WHEN esd.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 15

                                                     THEN 'COLOR=\"#008000\" />'||esd.esddsc

                                              WHEN esd.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 15 AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 20

                                                     THEN 'COLOR=\"#BB9900\" />'||esd.esddsc

                                              WHEN esd.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 20

                                                     THEN 'COLOR=\"#DD0000\" />'||esd.esddsc

                                         ELSE /* Situação: Ajuste de Supervisão(Empresa) */

                                              CASE WHEN esd.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 5

                                                           THEN 'COLOR=\"#008000\" />'||esd.esddsc

                                                    WHEN esd.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 5 AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 10

                                                           THEN 'COLOR=\"#BB9900\" />'||esd.esddsc

                                                    WHEN esd.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 10

                                                           THEN 'COLOR=\"#DD0000\" />'||esd.esddsc

                                               ELSE /* Situação: Reavaliação da Supervisão(MEC) */

                                                    CASE WHEN esd.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 7

                                                                 THEN 'COLOR=\"#008000\" />'||esd.esddsc

                                                          WHEN esd.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 7 AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 10

                                                                 THEN 'COLOR=\"#BB9900\" />'||esd.esddsc

                                                          WHEN esd.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 10

                                                                 THEN 'COLOR=\"#DD0000\" />'||esd.esddsc

                                                      ELSE /* Situação: Reajuste da supervisão (Empresa) */

                                                          CASE WHEN esd.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 5

                                                                       THEN 'COLOR=\"#008000\" />'||esd.esddsc

                                                                WHEN esd.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 5 AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) <= 10

                                                                       THEN 'COLOR=\"#BB9900\" />'||esd.esddsc

                                                                WHEN esd.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(hst.htddata, 'YYYY-mm-dd'))::timestamp) > 10

                                                                       THEN 'COLOR=\"#DD0000\" />'||esd.esddsc

                                                                       ELSE /* As demais Situações do Grupo */

                                                                            CASE WHEN esd.esddsc IS NOT NULL

                                                                                  THEN 'COLOR=\"#000000\" />'||esd.esddsc

                                                                                  ELSE 'COLOR=\"#000000\" /> Aguardando Início de Tramitação'
                                                                       END

                                                      END             

                                               END              

                                         END

                                   END

                             END || '</FONT>' as nivelsupervisao";
				break;
			case 'nivelsupervisaogrupo':
				$select[] = "'<FONT '||

                             /*Situação: Grupo em Supervisão*/

                             CASE WHEN esd6.esdid = ".GRUPOEMSUPERVISAO."  AND DATE_PART('days', NOW() - (to_char(hst7.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst6.htddata, 'YYYY-mm-dd'))::timestamp) > 90

                                       THEN 'COLOR=\"#DD0000\" />'||esd6.esddsc

                                    /* Situação: Aguardando Início de Supervisão pela Empresa */

                                    WHEN esd6.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(hst7.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst6.htddata, 'YYYY-mm-dd'))::timestamp) <= 20

                                           THEN 'COLOR=\"#008000\" />'||esd6.esddsc

                                    WHEN esd6.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(hst7.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst6.htddata, 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(hst7.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst6.htddata, 'YYYY-mm-dd'))::timestamp) <= 24

                                           THEN 'COLOR=\"#BB9900\" />'||esd6.esddsc

                                    WHEN esd6.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(hst7.htddata, 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(hst6.htddata, 'YYYY-mm-dd'))::timestamp) > 24

                                           THEN 'COLOR=\"#DD0000\" />'||esd6.esddsc

                                    ELSE/* As demais Situações do Grupo */

                                               CASE WHEN esd6.esddsc IS NOT NULL

                                                             THEN 'COLOR=\"#000000\" />'||esd6.esddsc

                                               END

                             END ||'</FONT>' as nivelsupervisaogrupo";
				break;
		}
	}
}

function monta_where()
{
	global $where,$orgids;
	
	$where[] = "obr.obsstatus = 'A'";
	
	/*** Inclue somente obras com os tipos de ensino selecionados para as colunas ***/
	if( !empty($orgids) )
	{
		$where[] = "org.orgid IN (".implode(",", $orgids).")";
		monta_join('tipoensino');
	}
	
	/*** Situação do Grupo ***/
	if( $_REQUEST['esdid_grupo'] && !empty($_REQUEST['esdid_grupo']) && !empty($_REQUEST['esdid_grupo'][0]) && $_REQUEST['esdid_grupo'] != '' )
	{
		$where[] = "esd2.esdid IN (".implode(",", $_REQUEST['esdid_grupo']).")";
		monta_join('situacaogrupo');
	}
	/*** Situação da Obra ***/
	if( $_REQUEST['esdid_obra'] && !empty($_REQUEST['esdid_obra']) && !empty($_REQUEST['esdid_obra'][0]) && $_REQUEST['esdid_obra'] != '' )
	{
		$where[] = "esd.esdid IN (".implode(",", $_REQUEST['esdid_obra']).")";
		monta_join('situacaoobra');
	}
	/*** Unidade ***/
	if( $_REQUEST['entid_unidade'] && !empty($_REQUEST['entid_unidade']) && !empty($_REQUEST['entid_unidade'][0]) && $_REQUEST['entid_unidade'] != '' )
	{
		$where[] = "ent2.entid IN (".implode(",", $_REQUEST['entid_unidade']).")";
		monta_join('unidade');
	}
	/*** Campus ***/
	if( $_REQUEST['entid_campus'] && !empty($_REQUEST['entid_campus']) && !empty($_REQUEST['entid_campus'][0]) && $_REQUEST['entid_campus'] != '' )
	{
		$where[] = "ent3.entid IN (".implode(",", $_REQUEST['entid_campus']).")";
		monta_join('campus');
	}
	/*** Empresa ***/
	if( $_REQUEST['epcid_empresa'] && !empty($_REQUEST['epcid_empresa']) && !empty($_REQUEST['epcid_empresa'][0]) && $_REQUEST['epcid_empresa'] != '' )
	{
		$where[] = "ent.entid IN (".implode(",", $_REQUEST['epcid_empresa']).")";
		monta_join('empresa');
	}
	/*** UF ***/
	if( $_REQUEST['uf'] && !empty($_REQUEST['uf']) && !empty($_REQUEST['uf'][0]) && $_REQUEST['uf'] != '' )
	{
		$where[] = "est.estuf IN ('".implode("','", $_REQUEST['uf'])."')";
		monta_join('uf');
	}
	/*** Município ***/
	if( $_REQUEST['muncod'] && !empty($_REQUEST['muncod']) && !empty($_REQUEST['muncod'][0]) && $_REQUEST['muncod'] != '' )
	{
		$where[] = "mun.muncod IN ('".implode("','", $_REQUEST['muncod'])."')";
		monta_join('municipio');
	}
	/*** Região ***/
	if( $_REQUEST['regcod'] && !empty($_REQUEST['regcod']) && !empty($_REQUEST['regcod'][0]) && $_REQUEST['regcod'] != '' )
	{
		$where[] = "reg.regcod IN ('".implode("','", $_REQUEST['regcod'])."')";
		monta_join('regiao');
	}
	/*** Mesorregião ***/
	if( $_REQUEST['mescod'] && !empty($_REQUEST['mescod']) && !empty($_REQUEST['mescod'][0]) && $_REQUEST['mescod'] != '' )
	{
		$where[] = "mes.mescod IN ('".implode("','", $_REQUEST['mescod'])."')";
		monta_join('mesorregiao');
	}
	/*** Tipo de Ensino ***/
	/*if( $_REQUEST['orgid_tipoensino'] && !empty($_REQUEST['orgid_tipoensino']) && !empty($_REQUEST['orgid_tipoensino'][0]) && $_REQUEST['orgid_tipoensino'] != '' )
	{
		$where[] = "org.orgid IN (".implode(",", $_REQUEST['orgid_tipoensino']).")";
		monta_join('tipoensino');
	}*/
	/*** Grupo ***/
	if( $_REQUEST['gpdid'] && !empty($_REQUEST['gpdid']) && !empty($_REQUEST['gpdid'][0]) && $_REQUEST['gpdid'] != '' )
	{
		$where[] = "gpd.gpdid IN (".implode(",", $_REQUEST['gpdid']).")";
		monta_join('grupo');
	}
	/*** Intervalo de Data da Última Tramitação do Grupo ***/
	if( $_REQUEST['dt_tramitacao_grupo_ini'] && $_REQUEST['dt_tramitacao_grupo_ini'] != '' && $_REQUEST['dt_tramitacao_grupo_fim'] && $_REQUEST['dt_tramitacao_grupo_fim'] != '' )
	{
		$where[] = "to_char(hst2.htddata,'DD/MM/YYYY') BETWEEN '".$_REQUEST['dt_tramitacao_grupo_ini']."' AND '".$_REQUEST['dt_tramitacao_grupo_fim']."'";
		monta_join('tramitacao_grupo');
	}
	/*** Intervalo de Data da Última Tramitação da Obra ***/
	if( $_REQUEST['dt_tramitacao_obra_ini'] && $_REQUEST['dt_tramitacao_obra_ini'] != '' && $_REQUEST['dt_tramitacao_obra_fim'] && $_REQUEST['dt_tramitacao_obra_fim'] != '' )
	{
		$where[] = "to_char(hst.htddata,'DD/MM/YYYY') BETWEEN '".$_REQUEST['dt_tramitacao_obra_ini']."' AND '".$_REQUEST['dt_tramitacao_obra_fim']."'";
		monta_join('tramitacao_obra');
	}
	/*** Intervalo de Datas do Parecer ***/
	if( $_REQUEST['dt_parecer_ini'] && $_REQUEST['dt_parecer_ini'] != '' && $_REQUEST['dt_parecer_fim'] && $_REQUEST['dt_parecer_fim'] != '' )
	{
		$where[] = "TO_CHAR(mpc.mpcdtinclusao,'YYYY-mm-dd') BETWEEN '". formata_data_sql($_REQUEST['dt_parecer_ini']) ."' AND '". formata_data_sql($_REQUEST['dt_parecer_fim']) ."'";
		monta_join('parecer');
	}
	/*** Intervalo de Datas da OS ***/
	if( $_REQUEST['dt_os_ini'] && $_REQUEST['dt_os_ini'] != '' && $_REQUEST['dt_os_fim'] && $_REQUEST['dt_os_fim'] != '' )
	{
		$where[] = "TO_CHAR(os.orsdtemissao,'YYYY-mm-dd') BETWEEN '". formata_data_sql($_REQUEST['dt_os_ini']) ."' AND '". formata_data_sql($_REQUEST['dt_os_fim']) ."'";
		monta_join('os_grupo');
	}
	/*** Preenchimento do Checklist ***/
	if( $_REQUEST['checklist'] && $_REQUEST['checklist'] != 'T' )
	{
		if( $_REQUEST['checklist'] == 'S' )
			$where[] = "(SELECT count(per.perid) FROM questionario.questionarioresposta qrp INNER JOIN questionario.pergunta per ON per.queid = qrp.queid WHERE qrp.qrpid = chk.qrpid) = (SELECT count(res.resid) FROM questionario.resposta res WHERE res.qrpid = chk.qrpid AND res.itpid is not null)";
		else
			$where[] = "(SELECT count(per.perid) FROM questionario.questionarioresposta qrp INNER JOIN questionario.pergunta per ON per.queid = qrp.queid WHERE qrp.qrpid = chk.qrpid) <> (SELECT count(res.resid) FROM questionario.resposta res WHERE res.qrpid = chk.qrpid AND res.itpid is not null)";
			
		monta_join('checklist');
	}
	/*** Situação do Parecer ***/
	if( $_REQUEST['parecer'] && $_REQUEST['parecer'] != 'T' )
	{
		if( $_REQUEST['parecer'] == 'S' ){
			$where[] = "mpc.mpcsituacao = 't'";
		}else if ( $_REQUEST['parecer'] == 'N' ){
			$where[] = "mpc.mpcsituacao = 'f'";
		}else if ( $_REQUEST['parecer'] == 'NI' ){
			$where[] = "mpc.mpcsituacao IS NULL";
		}	
		monta_join('parecer');
	}
}

function monta_sql()
{
	global $select,$join,$where,$groupBy,$orderBy;
	
	monta_select();
	
	monta_where();
	
	$sql = "SELECT DISTINCT 
				".implode(',', $select)." 
			FROM 
				obras.obrainfraestrutura obr 
			".implode(' ', $join)." 
			INNER JOIN obras.repositorio r on obr.obrid = r.obrid
			WHERE 
				".implode(' AND ', $where)." 
			GROUP BY 
				".implode(',', $groupBy)." 
			ORDER BY 
				".implode(',', $orderBy)."";

	return $sql;
}

$agrup = monta_agp($_REQUEST['orgid'], $_REQUEST['agrupador']);
$col   = monta_coluna($_REQUEST['orgid']);
$sql   = monta_sql();
//dbg($sql,1);
$dados = $db->carregar($sql);

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados);
$r->setColuna($col);
$r->setTotNivel(true);
$r->setMonstrarTolizadorNivel(false);
$r->setBrasao(true);
//$r->setEspandir( $_REQUEST['expandir'] );
echo $r->getRelatorio();

?>