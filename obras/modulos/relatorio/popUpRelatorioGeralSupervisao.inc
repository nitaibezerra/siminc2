<?php
ini_set( "memory_limit", "2048M" );
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
if($_REQUEST['esdid'][0] != "" && !in_array("situacaosupervisaoobra",$_REQUEST['agrupador']) ){
	$_REQUEST['agrupador'][] = "situacaosupervisaoobra";
	$_POST['agrupador'][] = "situacaosupervisaoobra";
}

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
				case 'uf':
					array_push($agp['agrupador'], array("campo" => "uf", "label" => "UF"));
					$groupBy[] = "uf";
					$orderBy[] = "uf";
					monta_join('uf');
					break;
				case 'brasil':
					array_push($agp['agrupador'], array("campo" => "brasil", "label" => "Brasil"));
					$groupBy[] = "brasil";
					$orderBy[] = "brasil";
					monta_join('brasil');
					break;
				case 'nomeobra':
					array_push($agp['agrupador'], array("campo" => "nomeobra", "label" => "Nome da Obra"));
					$groupBy[] = "nomeobra";
					$orderBy[] = "nomeobra";
					monta_join('nomeobra');
					break;
				case 'situacaosupervisaoobra':
					array_push($agp['agrupador'], array("campo" => "situacaosupervisaoobra", "label" => "Situação da Supervisão (Obra)"));
					$groupBy[] = "ed.esdid";
					$groupBy[] = "ed.esddsc";
					monta_join('situacaosupervisaoobra');
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
			if( !in_array("LEFT JOIN obras.orgao org1 ON org1.orgid = obr.orgid and org1.orgid = ".ORGID_EDUCACAO_SUPERIOR."", $join) ) { $join[] = "LEFT JOIN obras.orgao org1 ON org1.orgid = obr.orgid and org1.orgid = ".ORGID_EDUCACAO_SUPERIOR.""; }
			break;
		
		case 'educacao_profissional':
			if( !in_array("LEFT JOIN obras.orgao org2 ON org2.orgid = obr.orgid and org2.orgid = ".ORGID_EDUCACAO_PROFISSIONAL."", $join) ) { $join[] = "LEFT JOIN obras.orgao org2 ON org2.orgid = obr.orgid and org2.orgid = ".ORGID_EDUCACAO_PROFISSIONAL.""; }
			break;
			
		case 'educacao_basica':
			if( !in_array("LEFT JOIN obras.orgao org3 ON org3.orgid = obr.orgid and org3.orgid = ".ORGID_EDUCACAO_BASICA."", $join) ) { $join[] = "LEFT JOIN obras.orgao org3 ON org3.orgid = obr.orgid and org3.orgid = ".ORGID_EDUCACAO_BASICA.""; }
			break;
			
		case 'administrativo':
			if( !in_array("LEFT JOIN obras.orgao org4 ON org4.orgid = obr.orgid and org4.orgid = ".ORGID_ADMINSTRATIVO."", $join) ) { $join[] = "LEFT JOIN obras.orgao org4 ON org4.orgid = obr.orgid and org4.orgid = ".ORGID_ADMINSTRATIVO.""; }
			break;
			
		case 'hospitais':
			if( !in_array("LEFT JOIN obras.orgao org5 ON org5.orgid = obr.orgid and org5.orgid = ".ORGID_HOSPITAIS."", $join) ) { $join[] = "LEFT JOIN obras.orgao org5 ON org5.orgid = obr.orgid and org5.orgid = ".ORGID_HOSPITAIS.""; }
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
			//if( !in_array("LEFT JOIN workflow.documento doc3 ON doc3.docid = obr.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc3 ON doc3.docid = obr.docid"; }
			//if( !in_array("LEFT JOIN workflow.historicodocumento hst3 ON hst3.docid = doc3.docid AND (hst3.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc3.docid) OR hst3.htddata IS NULL )", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hst3 ON hst3.docid = doc3.docid AND (hst3.htddata = (SELECT max(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc3.docid) OR hst3.htddata IS NULL )"; }
			break;
		
		case 'repositorio':
			if( !in_array("LEFT JOIN obras.repositorio rep2 ON rep2.obrid = obr.obrid", $join) ) { $join[] = "LEFT JOIN obras.repositorio rep2 ON rep2.obrid = obr.obrid"; }
			break;
		
		case 'situacaosupervisaoobra':
			if( !in_array("INNER JOIN obras.situacaoobra so ON so.stoid = obr.stoid", $join) ) { $join[] = "INNER JOIN obras.situacaoobra so ON so.stoid = obr.stoid"; }
			if( !in_array("INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid", $join) ) { $join[] = "INNER JOIN obras.repositorio rep ON rep.obrid = obr.obrid"; }
			if( !in_array("INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid", $join) ) { $join[] = "INNER JOIN obras.itemgrupo itg ON itg.repid = rep.repid"; }
			if( !in_array("INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid", $join) ) { $join[] = "INNER JOIN obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid"; }
			if( !in_array("LEFT JOIN workflow.documento doc3 ON doc3.docid = obr.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento doc3 ON doc3.docid = obr.docid"; }
			if( !in_array("LEFT JOIN workflow.estadodocumento west ON west.esdid = doc3.esdid", $join) ) { $join[] = "LEFT JOIN workflow.estadodocumento west ON west.esdid = doc3.esdid"; }
			if( !in_array("LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc3.esdid AND ed.esdstatus = 'A'", $join) ) { $join[] = "LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc3.esdid AND ed.esdstatus = 'A'"; }
			if( !in_array("LEFT JOIN workflow.documento wdc ON wdc.docid = gpd.docid", $join) ) { $join[] = "LEFT JOIN workflow.documento wdc ON wdc.docid = gpd.docid"; }
			
			//if( !in_array("LEFT JOIN workflow.historicodocumento hd ON hd.docid = obr.docid", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento hd ON hd.docid = obr.docid"; }			
			if( !in_array("LEFT JOIN (select max(htddata) as htddata, docid from workflow.historicodocumento group by docid) as hd ON hd.docid = obr.docid", $join) ) { $join[] = "LEFT JOIN (select max(htddata) as htddata, docid from workflow.historicodocumento group by docid) as hd ON hd.docid = obr.docid"; }
			
			if( !in_array("LEFT JOIN workflow.estadodocumento wed ON wed.esdid = wdc.esdid AND wed.esdstatus = 'A'", $join) ) { $join[] = "LEFT JOIN workflow.estadodocumento wed ON wed.esdid = wdc.esdid AND wed.esdstatus = 'A'"; }
			
			//if( !in_array("LEFT JOIN workflow.historicodocumento whd ON whd.docid = gpd.docid AND whd.aedid = ". GRUPOLIBERADOPARASUPERVISAO ."", $join) ) { $join[] = "LEFT JOIN workflow.historicodocumento whd ON whd.docid = gpd.docid AND whd.aedid = ". GRUPOLIBERADOPARASUPERVISAO .""; }
			if( !in_array("LEFT JOIN (select max(htddata) as htddata, docid from workflow.historicodocumento WHERE aedid = ". GRUPOLIBERADOPARASUPERVISAO ." group by docid) AS whd ON whd.docid = gpd.docid", $join) ) { $join[] = "LEFT JOIN (select max(htddata) as htddata, docid from workflow.historicodocumento WHERE aedid = ". GRUPOLIBERADOPARASUPERVISAO ." group by docid) AS whd ON whd.docid = gpd.docid"; }
			
			if( !in_array("INNER JOIN obras.unidademedida ou ON ou.umdid = obr.umdidobraconstruida", $join) ) { $join[] = "INNER JOIN obras.unidademedida ou ON ou.umdid = obr.umdidobraconstruida"; }
			
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
				$select[] = 'count(org1.orgid) as educacao_superior';
				break;
			case 'educacao_profissional':
				$select[] = 'count(org2.orgid) as educacao_profissional';
				break;
			case 'educacao_basica':
				$select[] = 'count(org3.orgid) as educacao_basica';
				break;
			case 'administrativo':
				$select[] = 'count(org4.orgid) as administrativo';
				break;
			case 'hospitais':
				$select[] = 'count(org5.orgid) as hospitais';
				break;
		}
	}
	
	foreach($_POST['agrupador'] as $g)
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
			case 'uf':
				$select[] = 'est.estdescricao as uf';
				break;
			case 'brasil':
				$select[] = 'pai.paidescricao as brasil';
				break;
			case 'unidade':
				$select[] = 'ent2.entnome as unidade';
				break;
			case 'nomeobra':
				$select[] = "' <a href=\"#\" onclick=\"window.opener.location.href=\'obras.php?modulo=principal/cadastro&acao=A&obrid=' || obr.obrid || '\'\" > (' || obr.obrid || ') ' || obr.obrdesc || '</a>' as nomeobra";
				break;
			case 'situacaosupervisaoobra':
				$select[] = "'<FONT ' ||
							/* Situação: Em Supervisão */
							CASE WHEN ed.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) <= 20
									THEN 'COLOR=\"#008000\" />'||ed.esddsc
							 	 WHEN ed.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) <= 25
									THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
							     WHEN ed.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) > 25
									THEN 'COLOR=\"#DD0000\" />'||ed.esddsc
								ELSE /* Situação: Em Avaliação de Supervisão(MEC) */
								     CASE WHEN ed.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 15
											THEN 'COLOR=\"#008000\" />'||ed.esddsc
									     WHEN ed.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 15 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 20
											THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
									     WHEN ed.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 20
											THEN 'COLOR=\"#DD0000\" />'||ed.esddsc 
									ELSE /* Situação: Ajuste de Supervisão(Empresa) */
									     CASE WHEN ed.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 5
												THEN 'COLOR=\"#008000\" />'||ed.esddsc
										     WHEN ed.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 5 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 10
												THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
										     WHEN ed.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 10
												THEN 'COLOR=\"#DD0000\" />'||ed.esddsc 
										ELSE /* Situação: Reavaliação da Supervisão(MEC) */
										     CASE WHEN ed.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 7
													THEN 'COLOR=\"#008000\" />'||ed.esddsc
											     WHEN ed.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 7 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 10
													THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
											     WHEN ed.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 10
													THEN 'COLOR=\"#DD0000\" />'||ed.esddsc
											 ELSE /* Situação: Reajuste da supervisão (Empresa) */
											     CASE WHEN ed.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 5
														THEN 'COLOR=\"#008000\" />'||ed.esddsc
												     WHEN ed.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 5 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 10
														THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
												     WHEN ed.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 10
														THEN 'COLOR=\"#DD0000\" />'||ed.esddsc
														ELSE /* As demais Situações do Grupo */
														     CASE WHEN ed.esddsc IS NOT NULL
																THEN 'COLOR=\"#000000\" />'||ed.esddsc
															 WHEN ed.esdid IS NULL
																THEN 'COLOR=\"#000000\" />Em Repositório'
														END
											 END			 
										END			
									END 
								END
							END ||'</FONT>'AS situacaosupervisaoobra";
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
		$where[] = "obr.orgid IN (".implode(",", $orgids).")";
		monta_join('tipoensino');
	}
	
	/*** Situação da Obra ***/
	if( $_REQUEST['esdid_obra'] && !empty($_REQUEST['esdid_obra']) && !empty($_REQUEST['esdid_obra'][0]) && $_REQUEST['esdid_obra'] != '' )
	{
		$where[] = "esd.esdid IN (".implode(",", $_REQUEST['esdid_obra']).")";
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
	
	/*** Situação Documento ***/
	if( $_REQUEST['esdid'][0] != "")
	{
		$where[] = "ed.esdid IN ('".implode("','", $_REQUEST['esdid'])."') AND repstatus = 'A'";
	}
		
	//Repositorio
	switch( $_REQUEST["repositorio"] ){
		case "S":
			$where[] = "rep2.repid is not null";
			monta_join('repositorio');
			break;
		case "N":
			$where[] = "rep2.repid is null";
			monta_join('repositorio');
			break;
	}
	
	//Regra = verifica se o grupo da obra está finalizado
	 
	switch( $_REQUEST["supervisao"] ){
		
		case "S":
			if( !$_REQUEST["subfiltro_inicio"] ){
				$where[] = "( (SELECT 
			                COUNT(ooi.obrid)
			        FROM
			                obras.obrainfraestrutura ooi
			        left JOIN
			                obras.repositorio r ON r.obrid = ooi.obrid 
			        left JOIN
			                obras.itemgrupo oig ON oig.repid = r.repid
			        left JOIN
			                obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid 
			        left JOIN
			                workflow.documento wd ON wd.docid = ogd.docid
			        left JOIN
			                workflow.estadodocumento we ON we.esdid = wd.esdid 
			        WHERE
			                ooi.obrid = obr.obrid
			                AND we.esdid = ".OBRSUPFINALIZADA."
			                AND r.repstatus = 'I'
			                AND ogd.gpdstatus = 'A'
			                ) > 0  )";
			}else{
				if($_REQUEST["tiposupervisao"] == "entre"){
					$where[] = "( (SELECT 
			                COUNT(ooi.obrid)
			        FROM
			                obras.obrainfraestrutura ooi
			        left JOIN
			                obras.repositorio r ON r.obrid = ooi.obrid 
			        left JOIN
			                obras.itemgrupo oig ON oig.repid = r.repid
			        left JOIN
			                obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid 
			        left JOIN
			                workflow.documento wd ON wd.docid = ogd.docid
			        left JOIN
			                workflow.estadodocumento we ON we.esdid = wd.esdid 
			        WHERE
			                ooi.obrid = obr.obrid
			                AND we.esdid = ".OBRSUPFINALIZADA."
			                AND r.repstatus = 'I'
			                AND ogd.gpdstatus = 'A'
			                ) between {$_REQUEST["subfiltro_inicio"]} and {$_REQUEST["subfiltro_fim"]} )";
				}else{
					$where[] = "( (SELECT 
			                COUNT(ooi.obrid)
			        FROM
			                obras.obrainfraestrutura ooi
			        left JOIN
			                obras.repositorio r ON r.obrid = ooi.obrid 
			        left JOIN
			                obras.itemgrupo oig ON oig.repid = r.repid
			        left JOIN
			                obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid 
			        left JOIN
			                workflow.documento wd ON wd.docid = ogd.docid
			        left JOIN
			                workflow.estadodocumento we ON we.esdid = wd.esdid 
			        WHERE
			                ooi.obrid = obr.obrid
			                AND we.esdid = ".OBRSUPFINALIZADA."
			                AND r.repstatus = 'I'
			                AND ogd.gpdstatus = 'A'
			                ) {$_REQUEST["tiposupervisao"]} {$_REQUEST["subfiltro_inicio"]}  )";
				}
			}
		break;
			
		case "N":
			$where[] = "( (SELECT 
		                COUNT(ooi.obrid)
		        FROM
		                obras.obrainfraestrutura ooi
		        left JOIN
		                obras.repositorio r ON r.obrid = ooi.obrid 
		        left JOIN
		                obras.itemgrupo oig ON oig.repid = r.repid
		        left JOIN
		                obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid 
		        left JOIN
		                workflow.documento wd ON wd.docid = ogd.docid
		        left JOIN
		                workflow.estadodocumento we ON we.esdid = wd.esdid 
		        WHERE
		                ooi.obrid = obr.obrid
		                AND we.esdid = ".OBRSUPFINALIZADA."
		                AND r.repstatus = 'I'
		                AND ogd.gpdstatus = 'A'
		                ) = 0  )";
		break;
		
	}
}


function monta_sql()
{
	global $select,$join,$where,$groupBy,$orderBy;
	
	monta_select();
	
	monta_where();
	
	if($orderBy){		
		$stOrderBy = " ORDER BY ".implode(',', $orderBy)." ";
	}
	
	$sql = "SELECT DISTINCT 
				".implode(',', $select)." 
			FROM 
				obras.obrainfraestrutura obr 
			".implode(' ', $join)." 
			WHERE 
				".implode(' AND ', $where)." 
			GROUP BY 
				".implode(',', $groupBy)." ".$stOrderBy;

	return $sql;
}

$agrup = monta_agp($_REQUEST['orgid'], $_REQUEST['agrupador']);
$col   = monta_coluna($_REQUEST['orgid']);
$sql   = monta_sql();
//dbg( simec_htmlentities($sql), 1);die;
$dados = $db->carregar($sql);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados);
$r->setColuna($col);
$r->setTotNivel(true);
$r->setMonstrarTolizadorNivel(false);
$r->setBrasao(true);
echo $r->getRelatorio();
?>