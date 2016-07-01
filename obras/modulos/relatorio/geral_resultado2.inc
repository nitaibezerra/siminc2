<?php
if($_REQUEST['status'] != ""){
	$_REQUEST['colunas'][] = "obrdtexclusao";
	$_REQUEST['colunas'][] = "usucpfexclusao";
	$_REQUEST['colunas'][] = "obrobsexclusao";
}
	
function obras_monta_agp_relatorio2(){
	
	$agrupador = array('nomedaobra');
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array()
				);
	
	foreach ( $_REQUEST['colunas'] as $valor ) {
		$agp['agrupadoColuna'][] = $valor;				
	}
				
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "nomedaobra":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "nomedaobra2":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra2",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "nomedaobraxls":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobraxls",
											  		"label" => "Nome da Obra")										
									   				);
			break;
		}	
	}
	
	return $agp;
	
}

/**
 * Funçao que monta o sql para trazer o relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return string
 */
function obras_monta_sql_relatorio2(){
	
	$where = array();
	
	extract($_REQUEST);
	
	$selectTerritorios = "territorios.municipio ";
	
	// tipo de ensino
	if( $orgid ){
		array_push($where, " oi.orgid in (" . implode( ',', $orgid ) . ") ");
	}
	
	// região
	if( $regiao[0] && $regiao_campo_flag ){
		array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $regiao ) . "') ");
	}
	
	// Mesorregião
	if( $mesoregiao[0] && $mesoregiao_campo_flag ){
		array_push($where, " me.mescod " . (!$mesoregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mesoregiao ) . "') ");
	}
	
	// Microrregião
	if( $microregiao[0] && $microregiao_campo_flag ){
		array_push($where, " mic.miccod " . (!$microregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $microregiao ) . "') ");
	}
	
	// UF
	if( $uf[0] && $uf_campo_flag ){
		array_push($where, " ed.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uf ) . "') ");
	}
	
	// grupo municipio
	if( $grupomun[0]  && $grupomun_campo_flag ){
		
		$selectTerritorios = "(SELECT 
									tm.muncod, tm.mundescricao, gt.gtmid, gt.gtmdsc, tpm.tpmdsc
								FROM
									territorios.municipio tm 
								INNER JOIN
									territorios.muntipomunicipio mtm ON mtm.muncod = tm.muncod
								INNER JOIN
									territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
								INNER JOIN
									territorios.grupotipomunicipio gt ON gt.gtmid = tpm.gtmid 
								WHERE 
									tpm.gtmid = 5 AND gt.gtmid = 5 )";
		
		$selectGrupoMun  = "CASE WHEN tm.gtmid is not null THEN tm.gtmdsc ELSE 'Outros' END as grupomun, ";
		$dadosGrupoMun   = "grupomun, ";
		$groupByGrupoMun = "tm.gtmdsc, ";
		$groupByGtmid    = "tm.gtmid, ";
		array_push($where, " tm.gtmid " . (!$grupomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $grupomun ) . "') ");
	}
	
	// tipo municipio
	if( $tipomun[0]  && $tipomun_campo_flag ){
		array_push($where, " tpm.tpmid " . (!$tipomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tipomun ) . "') ");
	}
	
	// municipio
	if( $municipio[0]  && $municipio_campo_flag ){
		array_push($where, " ed.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ");
	}
	
	// unidade
	if( $unidade[0] && $unidade_campo_flag ){
		array_push($where, " oi.entidunidade " . (!$unidade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $unidade ) . ") ");
	}
	
	// entidcampus
	if( $entidcampus[0] && $entidcampus_campo_flag ){
		array_push($where, " oi.entidcampus " . (!$entidcampus_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $entidcampus ) . ") ");
	}
	
	// programa
	if( $prfid[0] && $prfid_campo_flag ){
		if ( !$prfid_campo_excludente ){
			array_push($where, " oi.prfid  IN (" . implode( ',', $prfid ) . ") ");	
		}else{
			array_push($where, " ( oi.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR oi.prfid is null ) ");
		}
		
	}
	
	// metragem da obra
	if( $metragem_inicio ){		
		if($metragem_operador == 'entre'){
			array_push($where, " oi.obrqtdconstruida >= '$metragem_inicio' AND oi.obrqtdconstruida <= '$metragem_fim' ");	
		}else{
			array_push($where, " oi.obrqtdconstruida $metragem_operador '$metragem_inicio' ");		
		}
	}
	
	// fonte
	if( $tooid[0] && $tooid_campo_flag ){
		if ( !$tooid_campo_excludente ){
			array_push($where, " oi.tooid  IN (" . implode( ',', $tooid ) . ") ");	
		}else{
			array_push($where, " ( oi.tooid  NOT IN (" . implode( ',', $tooid ) . ") OR oi.tooid is null ) ");
		}
		
	}
	
	// tipologia da obra
	if( $tpoid[0] && $tpoid_campo_flag ){
		array_push($where, " oi.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $tpoid ) . ") ");
	}
	
	// classificação da obra
	if( $cloid[0] && $cloid_campo_flag ){
		array_push($where, " oi.cloid " . (!$cloid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $cloid ) . ") ");
	}
	
	// situação da obra
	array_push($where, " oi.stoid not in (11) ");
	if( $stoid[0] && $stoid_campo_flag ){
		array_push($where, " oi.stoid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $stoid ) . ") ");
	}
	
	// percentual da obra
	if( $percentualinicial ){
		array_push($where, " oi.obrpercexec BETWEEN {$percentualinicial} AND {$percentualfinal}");
	}
	
	// percentual da obra
	if( $latitudeElongitude ){
		array_push($where, " (TRIM(ed.medlatitude)<>'' AND TRIM(ed.medlongitude)<>'')");
	}
	
	// possui foto
	switch ( $foto ) {
		case 'sim' : $stFiltro .= " and (ao.obrid is not null and ao.aqostatus = 'A') "; break;
		case 'nao' : $stFiltro .= " and ao.obrid is null  "; break;
	}

	// Filtro de vistoria
	switch ( $_REQUEST["vistoria"] ) {
		case 'sim' : $stFiltro .= " and oi.obrdtvistoria is not null "; break;
		case 'nao' : $stFiltro .= " and oi.obrdtvistoria is null "; break;
	}
	
	// Filtro de responsável pela vistoria
	switch ( $_REQUEST["responsavel"] ) {
		case ''  : $stFiltro .= " "; break;
		case '0' : $stFiltro .= " and COALESCE(s.supvid,0) = 0 "; break;
		case '1' : $stFiltro .= " and s.rsuid = 1 "; break;
		case '2' : $stFiltro .= " and s.rsuid = 2 "; break;
		case '3' : $stFiltro .= " and s.rsuid = 3 "; break;
		case '4' : $stFiltro .= " and s.rsuid = 4 "; break;
	}
	
	// Filtro de restricao
	switch ( $restricao ) {
		case 'sim' : $stFiltro .= " and (r.obrid is not null and r.rststatus = 'A')"; break;
		case 'nao' : $stFiltro .= " and r.obrid is null "; break;
	}
	
	// valor da obra
	if ( $_REQUEST["vlrmenor"] && $_REQUEST["vlrmaior"] ) {
		$vlrmenor = str_replace(array(".",","),array("","."),$_REQUEST["vlrmenor"]);
		$vlrmaior = str_replace(array(".",","),array("","."),$_REQUEST["vlrmaior"]);
		$stFiltro .= " AND oi.obrvlrrealobra BETWEEN ".$vlrmenor." AND ".$vlrmaior." ";
	}
	
	// Valor Contratado da Obra (R$):
	if( $obrcustocontrato_inicio != "" &&  $obrcustocontrato_fim != "" ){
		$obrcustocontrato_inicio = str_replace(array(".",","),array("","."),$obrcustocontrato_inicio);
		$obrcustocontrato_fim = str_replace(array(".",","),array("","."),$obrcustocontrato_fim);
		array_push($where, " oi.obrcustocontrato between $obrcustocontrato_inicio and $obrcustocontrato_fim ");
	}
	
	// Está em funcionamento?
	switch ( $_REQUEST["funundfuncionamento"] ) {
		case 'sim' : $stFiltro .= " and funundfuncionamento is true "; break;
		case 'nao' : $stFiltro .= " and (funundfuncionamento is false )"; break;
	}
	if( $_REQUEST["funundfuncionamento"] == 'sim' || $_REQUEST["funundfuncionamento"] == 'nao' ){
		$innerFuncionamento = "INNER JOIN (SELECT 
												f1.obrid, 
												funundfuncionamento
											FROM
												obras.funcunidade f1
											INNER JOIN 
											(
											SELECT obrid, max(funid) as funid FROM obras.funcunidade GROUP BY obrid ORDER BY obrid
											) f2 ON f1.funid = f2.funid ) fun ON oi.obrid = fun.obrid";
	}
	
	//Status da Obra
	if ( $_REQUEST["status"] == "inativo" ) {
		$stFiltro .= " AND oi.obsstatus = 'I' and usucpfexclusao is not null ";
	}elseif($_REQUEST["status"] == "todas"){
		$stFiltro .= "  ";
	}else{
		$stFiltro .= " AND oi.obsstatus = 'A' ";
	}
	
	if( !empty( $_REQUEST["obrtipoesfera"] ) ){
		$stFiltro .= " AND oi.obrtipoesfera = '{$_REQUEST["obrtipoesfera"]}' ";			
	}
	
	//Regra = verifica se o grupo da obra está finalizado 
	switch( $_REQUEST["supervisao"] ){
		
		case "S":
			if( !$_REQUEST["subfiltro_inicio"] ){
				$stFiltro .= " AND ( (SELECT 
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
			                ooi.obrid = oi.obrid
			                AND we.esdid = ".OBRSUPFINALIZADA."
			                AND r.repstatus = 'I'
			                AND ogd.gpdstatus = 'A'
			                ) > 0  )";
			}else{
				if($_REQUEST["tiposupervisao"] == "entre"){
					$stFiltro .= " AND ( (SELECT 
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
			                ooi.obrid = oi.obrid
			                AND we.esdid = ".OBRSUPFINALIZADA."
			                AND r.repstatus = 'I'
			                AND ogd.gpdstatus = 'A'
			                ) between {$_REQUEST["subfiltro_inicio"]} and {$_REQUEST["subfiltro_fim"]} )";
				}else{
					$stFiltro .= " AND ( (SELECT 
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
			                ooi.obrid = oi.obrid
			                AND we.esdid = ".OBRSUPFINALIZADA."
			                AND r.repstatus = 'I'
			                AND ogd.gpdstatus = 'A'
			                ) {$_REQUEST["tiposupervisao"]} {$_REQUEST["subfiltro_inicio"]}  )";
				}
			}
		break;
			
		case "N":
			$stFiltro .= " AND ( (SELECT 
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
		                ooi.obrid = oi.obrid
		                AND we.esdid = ".OBRSUPFINALIZADA."
		                AND r.repstatus = 'I'
		                AND ogd.gpdstatus = 'A'
		                ) = 0  )";
		break;
		
	}
	
	// SQL passada pelo Mario dia 15/06/2012
	$sql_obrsupervisoes = "SELECT DISTINCT count(obr22.obrid) as supervisoes--,
							       --obr22.obrid
							FROM
							   obras.obrainfraestrutura obr22
							INNER JOIN
							   obras.repositorio ore22 ON ore22.obrid = obr22.obrid
							INNER JOIN
							   obras.itemgrupo ig22 ON ig22.repid = ore22.repid
							INNER JOIN
							   obras.grupodistribuicao gd22 ON gd22.gpdid = ig22.gpdid
							INNER JOIN
							   workflow.documento wd22 ON wd22.docid = gd22.docid
							INNER JOIN
							   workflow.estadodocumento we22 ON we22.esdid = wd22.esdid
							WHERE 
								we22.esdid = 173
								AND obr22.obrid = oi.obrid
							GROUP BY
							obr22.obrid";
	//FIM SQL passada pelo Mario dia 15/06/2012
	
	// monta o sql 
	$sql = "SELECT DISTINCT
				oi.obrid || ' ' as obridid,
				'(' || oi.obrid || ') ' || oi.obrdesc as nomedaobra,
				oi.obrnumprocesso,
				oi.obrqtdconstruida || ' ' || COALESCE(und.umdeesc,'') as metragem,
				CASE WHEN oi.obrvalorprevisto is null OR oi.obrvalorprevisto = 0
					THEN ' - '
					ELSE to_char(oi.obrvalorprevisto, '999G999G999G999D99')
				END as obrvalorprevisto,
				CASE WHEN oi.obrcustocontrato is null OR oi.obrcustocontrato = 0
					THEN ' - '
					ELSE to_char(oi.obrcustocontrato, '999G999G999G999D99')
				END as obrcustocontrato,
				oi.obrpercexec||'%' as porcexecucao,
				to_char(date(oi.obrdtassinaturacontrato), 'DD/MM/YYYY') as obrdtassinaturacontrato,
				to_char(date(oi.dtterminocontrato), 'DD/MM/YYYY') as dtterminocontrato,
				to_char(date(oi.obrdtordemservico), 'DD/MM/YYYY') as obrdtordemservico,
				to_char(date(oi.obrdtinicio), 'DD/MM/YYYY') as obrdtinicio,
				to_char(date(oi.obrdttermino), 'DD/MM/YYYY') as obrdttermino,
				to_char(oi.obrdtinicio, 'DD/MM/YYYY') as datainicio,
				to_char(oi.obrdttermino, 'DD/MM/YYYY') as datafim,
				CASE WHEN oi.obrdtvistoria IS NOT NULL THEN 
						to_char(oi.obrdtvistoria, 'DD/MM/YYYY HH24:MI')
					 ELSE 
						to_char(oi.obsdtinclusao, 'DD/MM/YYYY HH24:MI')
				END as ultatualizacao,
				CASE WHEN ( SELECT COUNT(rstoid) FROM obras.restricaoobra WHERE obrid = oi.obrid ) = 0 
					THEN 'Não' 
					ELSE 'Sim' 
				END as qtdrestricoes,
				( SELECT COUNT(DISTINCT supv.supvid) FROM obras.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A') as qtdvistorias,
				( SELECT to_char (min(supvdt),'dd/mm/yyyy') FROM obras.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A' and stoid = 3) as supvdt,
				tai.aqidsc as tipoaquisicao,
				st.stodesc as situacaoobra,
				tm2.muncod as muncod,
				tm2.mundescricao as municipio,
				et.estuf as estado,
				ee.entnome as unidade,
				ee.entsig as sigla,
				ee.entemail as unidade_email,
				'('||ee.entnumdddcomercial||') '||ee.entnumcomercial as unidade_numcomercial,
				eed.endlog as unidade_endlog, 
				eed.endcom as unidade_endcom, 
				eed.endbai as unidade_endbai, 
				eed.endnum as unidade_endnum, 
				eed.endcomunidade as unidade_endcomunidade,
				eed.endcep as unidade_endcep,
				
				ee2.entnome as campus,
				ee3.entnumcpfcnpj || ' ' as cnpjempresacontratada,
				ee3.entnome as nomeempresacontratada,
				tpo.tobadesc as tipoobra,
				pf.prfdesc as subacao,
				too.toodescricao as fonte,
				org.orgdesc as orgao,
				CASE WHEN mol.moldsc is null
					THEN 'Não informado'
					ELSE moldsc
				END as moldsc,
				CASE WHEN licitacaouasg is null
					THEN 'Não informado'
					ELSE licitacaouasg
				END as licitacaouasg,
				CASE WHEN numlicitacao is null
					THEN 'Não informado'
					ELSE numlicitacao
				END as numlicitacao,
				CASE WHEN frpdesc is null
					THEN 'Não informado'
					ELSE frpdesc
				END  as frpdesc,
				to_char((SELECT 
							date(fl.flcpubleditaldtprev)
						 FROM 
						 	obras.faselicitacao fl
						 LEFT JOIN obras.tiposfaseslicitacao tfl ON tfl.tflid = fl.tflid
						 WHERE 
						 	-- fl.tflid = '2'AND fl.obrid = oi.obrid AND fl.flcstatus='A' ORDER BY flcid DESC LIMIT 1
						 	fl.obrid = oi.obrid AND fl.flcstatus = 'A' AND tfl.tflordem = 1 ORDER BY flcid DESC LIMIT 1
				), 'DD/MM/YYYY') as dtpublicacao,
				to_char((SELECT 
							date(fl.flchomlicdtprev)
						 FROM obras.faselicitacao fl
						 LEFT JOIN obras.tiposfaseslicitacao tfl ON tfl.tflid = fl.tflid
						 WHERE --fl.tflid = '9'AND fl.obrid = oi.obrid AND fl.flcstatus='A' ORDER BY flcid DESC LIMIT 1
						 	fl.obrid = oi.obrid AND fl.flcstatus = 'A' AND tfl.tflordem = 4 ORDER BY flcid DESC LIMIT 1	
				), 'DD/MM/YYYY') as dthomologacao,
				CASE WHEN of.covid is not null 
					THEN oc.covnumero || ' ' 
					ELSE oi.numconvenio || ' ' 
				END as nr_convenio,
	            to_char(oc.covdtinicio, 'DD/MM/YYYY') as dt_inicio_conv,
	            to_char(oc.covdtfinal, 'DD/MM/YYYY') as dt_final_conv,
	            SUM(CASE WHEN ieo.eocvlrliquidado is null OR ieo.eocvlrliquidado = 0
					THEN 0.00
					ELSE ieo.eocvlrliquidado
				END) as vlrliquidado,
				CASE WHEN ed.medlatitude = '' and ed.medlongitude = '' 
					THEN ''
					ELSE 'Lat.: ' || ed.medlatitude || '<br>Long.: ' || ed.medlongitude 
				END as coordenadas_geograficas,
				SUBSTRING(ed.endcep, 1, 5) || '-' || SUBSTRING(ed.endcep, 6, 8) as cep, 
				ed.endlog, 
				ed.endcom, 
				ed.endbai, 
				ed.endnum, 
				ed.endcomunidade,
				COALESCE(to_char(obrdtexclusao,'DD/MM/YYYY'),'N/A') as obrdtexclusao,
				COALESCE(obrobsexclusao,'N/A') as obrobsexclusao,
				COALESCE((select usunome from seguranca.usuario where usucpf = usucpfexclusao),'N/A') as usucpfexclusao,
				me.mesdsc as mesoregiao,
				mic.micdsc as microregiao,
				--Alterações Pedidas pelo Mario 15/06/2012
				
				(
				
				$sql_obrsupervisoes
				--SELECT DISTINCT
				--	count(os.orsid) as qtd
				--FROM
				--	obras.obrainfraestrutura obr
				--INNER JOIN
				--	obras.repositorio ore ON ore.obrid = obr.obrid
				--INNER JOIN
				--	obras.itemgrupo ig ON ig.repid = ore.repid
				--INNER JOIN
				--	obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid
				--INNER JOIN
				--	obras.ordemservico os ON os.gpdid = gd.gpdid --ordem de serviço
				--INNER JOIN
				--	workflow.documento wd ON wd.docid = gd.docid
				--INNER JOIN
				--	workflow.estadodocumento we ON we.esdid = wd.esdid
				--WHERE
				--	os.orsstatus = 'A' 
				--	AND obr.obsstatus = 'A' 
				--	AND obr.obrid = oi.obrid 
				--	AND ore.repstatus = 'A'
				--	AND we.esdid = ". OBRSUPFINALIZADA ."
				) 
				--FIM Alterações Pedidas pelo Mario 15/06/2012
				as obrsupervisoes,
				CASE WHEN oi.tpoid is not null THEN tp.tpodsc ELSE 'Não informado' END as tipologia,
				dcoano,
				oi.preid
			FROM
				obras.obrainfraestrutura oi
			$innerFuncionamento
			LEFT JOIN painel.dadosconvenios dc ON dc.dcoprocesso = Replace(Replace(Replace(oi.obrnumprocessoconv,'.',''),'/',''),'-','')
			INNER JOIN 
				entidade.endereco ed 				ON oi.endid = ed.endid
			LEFT JOIN 
				obras.faselicitacao fl 				ON fl.obrid = oi.obrid
			LEFT JOIN 
				obras.tiposfaseslicitacao tfl 		ON tfl.tflid = fl.tflid
			LEFT JOIN 
				obras.infraestrutura inf 			ON inf.iexid = oi.iexid
			LEFT JOIN 
				obras.formarepasserecursos frr 		ON frr.obrid = oi.obrid
			LEFT JOIN 
				obras.tipoformarepasserecursos tfrr ON tfrr.frpid = frr.frpid
			LEFT JOIN 
				obras.modalidadelicitacao mol 		ON mol.molid = oi.molid
			LEFT JOIN 
				obras.tipoaquisicaoimovel tai 		ON tai.aqiid = inf.aqiid
			LEFT JOIN
				obras.situacaoobra st 	   			ON oi.stoid = st.stoid
			LEFT JOIN
				obras.tipoobra tpo 		   			ON tpo.tobaid = oi.tobraid
			LEFT JOIN
				obras.orgao org 		   			ON org.orgid = oi.orgid 
			LEFT JOIN 
				obras.unidademedida und     		ON und.umdid = oi.umdidobraconstruida
			LEFT JOIN 
				obras.execucaoorcamentaria eo       ON eo.obrid = oi.obrid
			LEFT JOIN 
				obras.itensexecucaoorcamentaria ieo ON ieo.eorid = eo.eorid
			LEFT JOIN 
				territorios.estado et 	   			ON ed.estuf = et.estuf
			LEFT JOIN 
				territorios.regiao re 	   			ON re.regcod = et.regcod
			LEFT JOIN 
				territorios.municipio tm2  			ON tm2.muncod = ed.muncod
			LEFT JOIN 
				territorios.mesoregiao me  			ON me.mescod = tm2.mescod
			LEFT JOIN 
				territorios.microregiao mic			ON mic.miccod = tm2.miccod
			LEFT JOIN 
				{$selectTerritorios} tm    			ON tm.muncod = ed.muncod
			INNER JOIN 
				entidade.entidade ee 	   			ON oi.entidunidade = ee.entid
			LEFT  JOIN 
				entidade.endereco eed 				ON eed.entid = ee.entid
			LEFT JOIN 
				territorios.pais pa 	   			ON pa.paiid = re.paiid
			LEFT JOIN 
				entidade.entidade ee2 	   			ON oi.entidcampus = ee2.entid
			LEFT JOIN 
				entidade.funcaoentidade ef 			ON ee2.entid = ef.entid AND ef.funid IN( 17,18 )
			LEFT JOIN
				entidade.entidade ee3 	   			ON oi.entidempresaconstrutora = ee3.entid
			LEFT JOIN 
				obras.programafonte pf     			ON oi.prfid = pf.prfid
			LEFT JOIN
				obras.tipoorigemobra too			ON oi.tooid = too.tooid
			LEFT JOIN
				obras.classificacaoobra cl 			ON oi.cloid = cl.cloid
			LEFT JOIN
				obras.tipologiaobra tp    			ON oi.tpoid = tp.tpoid
			LEFT JOIN
					(SELECT
						rsuid,obrid,supvid
					FROM
						obras.supervisao s
					WHERE
						supvid = (SELECT supvid FROM obras.supervisao ss WHERE ss.obrid = s.obrid order by supvdt desc, supvid desc limit 1) ) AS s ON s.obrid = oi.obrid
			LEFT JOIN 
				( SELECT DISTINCT obrid, aqostatus FROM obras.arquivosobra WHERE tpaid = 21 AND aqostatus = 'A' ) as ao ON ao.obrid = oi.obrid 
			LEFT JOIN 
				( SELECT DISTINCT obrid, rststatus FROM obras.restricaoobra WHERE rststatus = 'A' ) as r ON r.obrid = oi.obrid 
			LEFT JOIN
	            obras.formarepasserecursos of ON of.obrid = oi.obrid
			LEFT JOIN
	            obras.conveniosobra  oc ON oc.covid = of.covid
			WHERE
			--	oi.obrid = 1484 AND 
				1=1 " . ( is_array($where) ? ' AND' . implode(' AND ', $where) : '' ) 
				. $stFiltro."
			GROUP BY
				oi.obrid,oi.obrdesc,oi.obrqtdconstruida,und.umdeesc,oi.obrvalorprevisto,oi.obrcustocontrato,oi.obrpercexec,
				oi.obrdtassinaturacontrato,oi.dtterminocontrato,oi.obrdtordemservico,oi.obrdtinicio,oi.obrdttermino,ultatualizacao,
				tai.aqidsc,st.stodesc,tm2.muncod,tm2.mundescricao,et.estuf,ee.entsig,ee.entnome,ee2.entnome,ee3.entnumcpfcnpj,ee3.entnome,
				tpo.tobadesc,pf.prfdesc,too.toodescricao,org.orgdesc,mol.moldsc,oi.licitacaouasg,oi.numlicitacao,tfrr.frpdesc,of.covid,oc.covnumero,
				oi.numconvenio,oc.covdtinicio,oc.covdtfinal,ed.medlatitude,ed.medlongitude,ed.endcep,ed.endlog,ed.endcom,ed.endbai,
				ed.endnum,ed.endcomunidade,obrdtexclusao,obrobsexclusao,usucpfexclusao,ee.entemail,
				unidade_numcomercial,unidade_endlog,unidade_endcom,unidade_endbai,unidade_endnum, 
				unidade_endcomunidade,unidade_endcep,mesoregiao,microregiao,tipologia,dcoano,oi.preid,oi.obrnumprocesso";
			//ver($sql, d);
	return $sql;
	
}


ini_set("memory_limit", "1024M");

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = '?modulo=<?= $modulo ?>&acao=A';
	</script>
	<?
	die;
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql_relatorio2();
$agrupador = obras_monta_agp_relatorio2();
$coluna    = obras_monta_coluna_relatorio2();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(false);

// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == '2' ){
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
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>