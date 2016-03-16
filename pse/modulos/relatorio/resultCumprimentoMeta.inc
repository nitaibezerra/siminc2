<?
if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","2048M");
set_time_limit(30000);
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

$sql   = monta_sql_testeMun();
$dadosTesteMun = $db->carregar($sql);
$sql   = monta_sql();
$dados = $db->carregar($sql);
$dadosOrigem = $dados; 

//verifica quais municipios atingiu a meta de 70% do alunado avaliado.
if( $_POST['tiporelatorio'] == 1 ){
	
	if( is_array($dados) ){
		
		$limpaMunicipio = array();
		
		foreach($dadosTesteMun as $i => $dado){
			
			//comp 1, ação 3 - não contabiliza
			if($dado['acao'] != "3 - Ação: Aferir a pressão arterial e identificar os educandos com HAS"){
					
				if( (int)$dado['percentualatingido'] < 70 && !in_array($dado['municipio'], $limpaMunicipio) ){
					array_push($limpaMunicipio, $dado['municipio']);
				}
			}
			
		}
		
		foreach($dados as $j => $dado){
			
			if( in_array($dado['municipio'], $limpaMunicipio) ){
				unset($dados[$j]);
			}
		}
		
		//reindexa o array
		if ($dados){
			$dados = array_values( $dados );
		}
		
		
	}
	
}
elseif( $_POST['tiporelatorio'] == 2 ){
	
	if( is_array($dados) ){
		
		$limpaMunicipio = array();
		
		foreach($dadosTesteMun as $i => $dado){
			
			//comp 1, ação 3 - não contabiliza
			if($dado['acao'] != "3 - Ação: Aferir a pressão arterial e identificar os educandos com HAS"){
					
				if( (int)$dado['percentualatingido'] < 70 && !in_array($dado['municipio'], $limpaMunicipio) ){
					array_push($limpaMunicipio, $dado['municipio']);
				}
			}
			
		}
		
		foreach($dados as $j => $dado){
			
			if( !in_array($dado['municipio'], $limpaMunicipio) ){
				unset($dados[$j]);
			}
				
		}
		
		//reindexa o array
		if ($dados){
			$dados = array_values( $dados );
		}
		
		
	}
	
}




//die;

$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(true);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(true); 
$r->setTolizadorLinha(false);
//$r->setBrasao(true);

if($_POST['tipo_relatorio'] == 'xls'){
	ob_clean();
	$nomeDoArquivoXls="relatorio_Cumprimento_Metas_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
}elseif($_POST['tipo_relatorio'] == 'html'){
	echo $r->getRelatorio();
}

?>
</body>
</html>
<?php 

function monta_sql(){
	global $db;
	global $filtroSession;
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $componente[0] && ( $componente_campo_flag || $componente_campo_flag == '1' )){
		$where[2] = " AND co.comid " . (( $componente_campo_excludente == null || $componente_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $componente ) . "') ";		
	}
	if( $acao[0] && ( $acao_campo_flag || $acao_campo_flag == '1' )){
		$where[3] = " AND ac.acaoid " . (( $acao_campo_excludente == null || $acao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $acao ) . "') ";		
	}
	
	//monta query monstra
	//lista as ações para montar a query
	$sql2 = "select co.comdescricao as componente, ac.acaodescricao as acao, ac.acaoid, co.comid 
			from pse.acao ac 
			INNER JOIN pse.componenteavalia co ON co.comid = ac.comid
			WHERE co.comid in (1,2)
			and acaoid not in(6)
			order by 1,2";
	$dadosAC = $db->carregar($sql2);
	
	foreach($dadosAC as $a){
		
		if($a['comid'] == 1){ // componente 1
			
			if($a['acaoid'] == 6){ // Ação 7
				
				$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								mp.mpaacao06comp1/totalescola as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when (mp.mpaacao06comp1/totalescola) > 0 then
										(COALESCE(mo2.qtdrealizada,0) * 100) / (mp.mpaacao06comp1/totalescola)
								      else
										0
								end) as percentualatingido,
								(case when mo2.qtdalterada is not null then
									mo2.qtdalterada
								      else
									0
								end) as qtdalterada
							FROM pse.planoacaoc1 pac
							INNER JOIN
								pse.contratualizacao con ON con.conid = pac.conid and con.arqid is not null
							INNER JOIN
								pse.metapactuada mp ON mp.conid = pac.conid
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(SELECT count(pamid) as totalescola, conid from pse.planoacaoc1 group by conid) te on te.conid = pac.conid
							LEFT JOIN
								(select sum(a.avleducandoavaliado) AS qtdrealizada, sum(a.avlidentifica1) AS qtdalterada, mo.entid, ac.acaodescricao as acao
								from pse.avaliacao a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE 
									mo.moestatus = 'A' 
									AND mo.moeidentificasse in ('N','S') 
									--AND a.avlvalidar = 'S' 
									AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								GROUP BY mo.entid, ac.acaodescricao
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								mp.mpaacao06comp1,
								mo2.acao,
								mo2.qtdrealizada,
								mo2.qtdalterada,
								totalescola
							";
				//dbg($sql,1);
					
			}else{
					$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								pac.pamquanteduseratend as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when pac.pamquanteduseratend > 0 then
										(COALESCE(mo2.qtdrealizada,0) * 100) / pac.pamquanteduseratend
								      else
										0
								end) as percentualatingido,
								(case when mo2.qtdalterada is not null then
									mo2.qtdalterada
								      else
									0
								end) as qtdalterada
							FROM pse.planoacaoc1 pac
							INNER JOIN
								pse.contratualizacao con ON con.conid = pac.conid and con.arqid is not null
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(select sum(a.avleducandoavaliado) AS qtdrealizada, sum(a.avlidentifica1) AS qtdalterada, mo.entid, ac.acaodescricao as acao
								from pse.avaliacao a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE 
									mo.moestatus = 'A' 
									AND mo.moeidentificasse in ('N','S') 
									--AND a.avlvalidar = 'S' 
									AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								GROUP BY mo.entid, ac.acaodescricao
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								pac.pamquanteduseratend,
								mo2.acao,
								mo2.qtdrealizada,
								mo2.qtdalterada
								
							";
				}
		}
		else{ // componente 2
			
			/*
			if($a['acaoid'] == 11){ // Ação 3
				
				$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								mp.mpaacao04comp3/totalescola as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when (mp.mpaacao04comp3/totalescola) > 0 then
									(case when COALESCE(mo2.qtdrealizada,0) > (mp.mpaacao04comp3/totalescola) then
											100.00
									      else
											(COALESCE(mo2.qtdrealizada,0) * 100) / (mp.mpaacao04comp3/totalescola)
									end)
								else
									0
								end) as percentualatingido,
								0 as qtdalterada
							FROM pse.planoacaoc2 pac
							INNER JOIN
								pse.contratualizacao con ON con.conid = pac.conid and con.arqid is not null
							INNER JOIN
								pse.metapactuada mp ON mp.conid = pac.conid
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(SELECT count(pacid) as totalescola, conid from pse.planoacaoc2 group by conid) te on te.conid = pac.conid
							LEFT JOIN
								(select sum(a.avcquantidade) AS qtdrealizada, mo.entid, ac.acaodescricao as acao
								from pse.avaliacomp2 a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE 
									mo.moestatus = 'A' 
									AND mo.moeidentificasse in ('N','S') 
									--AND a.avcvalidar = 'S' 
									AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								GROUP BY mo.entid, ac.acaodescricao
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								mp.mpaacao04comp3,
								mo2.acao,
								mo2.qtdrealizada,
								totalescola
								
							";
				
				
					
			}else{
			*/	
				$sql .= "UNION
					
						SELECT  mun.muncod,
								ende.estuf as estado, 
								mun.mundescricao as municipio,
								'".$a['componente']."' as componente,
								'".$a['acao']."' as acao, 
								ent.entnome as escola,
								pac.pacquanteduseratend as qtdpactuada,
								(case when mo2.qtdrealizada is not null then
									mo2.qtdrealizada
								      else
									0
								end) as qtdrealizada,
								(case when pac.pacquanteduseratend > 0 then
									(case when COALESCE(mo2.qtdrealizada,0) > pac.pacquanteduseratend then
											100.00
									      else
											(COALESCE(mo2.qtdrealizada,0) * 100) / pac.pacquanteduseratend
									end)
								else
									0
								end) as percentualatingido,
								0 as qtdalterada
							FROM pse.planoacaoc2 pac
							INNER JOIN
								pse.contratualizacao con ON con.conid = pac.conid and con.arqid is not null
							INNER JOIN
								entidade.entidade ent ON ent.entid = pac.entid
							INNER JOIN
								entidade.endereco ende ON ende.entid = ent.entid
							INNER JOIN
								territorios.municipio mun ON mun.muncod = ende.muncod
							LEFT JOIN
								(select sum(a.avcquantidade) AS qtdrealizada, mo.entid, ac.acaodescricao as acao
								from pse.avaliacomp2 a
								INNER JOIN
									pse.monitoraescola mo ON mo.moeid = a.moeid 
								INNER JOIN
									pse.acao ac ON ac.acaoid = mo.acaoid
								INNER JOIN
									pse.componenteavalia co ON co.comid = ac.comid
								WHERE 
									mo.moestatus = 'A' 
									AND mo.moeidentificasse in ('N','S') 
									AND a.avctipo = '1'
									--AND a.avcvalidar = 'S' 
									AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
								GROUP BY mo.entid, ac.acaodescricao
								) mo2 ON mo2.entid = ent.entid 
							WHERE
								ent.entstatus='A'
								".$where[0]."
								".$where[1]."
							GROUP BY 
								mun.muncod,
								ende.estuf,
								mun.mundescricao,
								ent.entcodent, ent.entnome,
								pac.pacquanteduseratend,
								mo2.acao,
								mo2.qtdrealizada
								
							";
			/*	
			}
			*/
		}
		
		
	}
	
	$sql = substr($sql, 5);
	$sql = $sql . " ORDER BY 2, 3, 4, 5, 6 ";
	//dbg($sql,1);
	return $sql;
}


function monta_sql_testeMun(){
	global $db;
	global $filtroSession;

	extract($_POST);

	$select = array();
	$from	= array();

	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND mun.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";
	}
	if( $municipio[0] && ( $municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND mun.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";
	}
	if( $componente[0] && ( $componente_campo_flag || $componente_campo_flag == '1' )){
		$where[2] = " AND co.comid " . (( $componente_campo_excludente == null || $componente_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $componente ) . "') ";
	}
	if( $acao[0] && ( $acao_campo_flag || $acao_campo_flag == '1' )){
		$where[3] = " AND ac.acaoid " . (( $acao_campo_excludente == null || $acao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $acao ) . "') ";
	}

	//monta query monstra
	//lista as ações para montar a query
	$sql2 = "SELECT 
				co.comdescricao as componente, 
				ac.acaodescricao as acao, 
				ac.acaoid, 
				co.comid
			FROM 
				pse.acao ac
			INNER JOIN pse.componenteavalia co ON co.comid = ac.comid
			WHERE 
				co.comid in (1,2)
				AND acaoid not in(6)
			ORDER BY 1,2";
	$dadosAC = $db->carregar($sql2);

	foreach($dadosAC as $a){

		if($a['comid'] == 1){ // componente 1
				
			if($a['acaoid'] == 6){ // Ação 7

				$sql .= "UNION
					
				SELECT  
					mun.muncod,
					mun.estuf as estado,
					mun.mundescricao as municipio,
					'".$a['componente']."' as componente,
					'".$a['acao']."' as acao,
					coalesce(sum(mp.mpaacao06comp1/totalescola),0) as qtdpactuada,
					coalesce(sum(mo2.qtdrealizada,0),0) as qtdrealizada,
					(coalesce(sum(mo2.qtdrealizada),0) * 100) / coalesce(sum(mp.mpaacao06comp1/totalescola),0) as percentualatingido,
					coalesce(sum(mo2.qtdalterada),0) as qtdalterada
				FROM 
					pse.planoacaoc1 pac
				INNER JOIN pse.contratualizacao 	con ON con.conid = pac.conid and con.arqid is not null	
				INNER JOIN pse.metapactuada 		 mp ON mp.conid   = pac.conid
				INNER JOIN entidade.entidade 		ent ON ent.entid  = pac.entid
				INNER JOIN entidade.endereco 	   ende ON ende.entid = ent.entid
				INNER JOIN territorios.municipio 	mun ON mun.muncod = ende.muncod
				LEFT  JOIN (SELECT count(pamid) as totalescola, conid 
							FROM pse.planoacaoc1 group by conid) te ON te.conid = pac.conid
				LEFT  JOIN (SELECT sum(a.avleducandoavaliado) AS qtdrealizada, sum(a.avlidentifica1) AS qtdalterada, mo.entid, ac.acaodescricao as acao
							FROM pse.avaliacao a
							INNER JOIN pse.monitoraescola mo ON mo.moeid = a.moeid
							INNER JOIN pse.acao ac ON ac.acaoid = mo.acaoid
							INNER JOIN pse.componenteavalia co ON co.comid = ac.comid
							WHERE 
								mo.moestatus = 'A' 
								AND mo.moeidentificasse in ('N','S') 
								--AND a.avlvalidar = 'S' 
								AND ac.acaoid = ".$a['acaoid']."
							".$where[2]."
							".$where[3]."
							GROUP BY mo.entid, ac.acaodescricao
							) mo2 ON mo2.entid = ent.entid
				WHERE
					ent.entstatus='A'
					".$where[0]."
					".$where[1]."
				GROUP BY
					mun.muncod,
					mun.estuf,
					mun.mundescricao
			";
			}else{
				$sql .= "UNION
					
				SELECT  
					mun.muncod,
					mun.estuf as estado,
					mun.mundescricao as municipio,
					'".$a['componente']."' as componente,
					'".$a['acao']."' as acao,
					coalesce(sum(pac.pamquanteduseratend),0) as qtdpactuada,
					coalesce(sum(mo2.qtdrealizada),0) as qtdrealizada,
					(coalesce(sum(mo2.qtdrealizada),0) * 100) / coalesce(sum(pac.pamquanteduseratend),0) as percentualatingido,
					coalesce(sum(mo2.qtdalterada),0) as qtdalterada
				FROM 
					pse.planoacaoc1 pac
				INNER JOIN pse.contratualizacao 	con ON con.conid = pac.conid and con.arqid is not null
				INNER JOIN entidade.entidade 		ent ON ent.entid = pac.entid
				INNER JOIN entidade.endereco 	   ende ON ende.entid = ent.entid
				INNER JOIN territorios.municipio 	mun ON mun.muncod = ende.muncod
				LEFT  JOIN (SELECT sum(a.avleducandoavaliado) AS qtdrealizada, sum(a.avlidentifica1) AS qtdalterada, mo.entid, ac.acaodescricao as acao
							FROM pse.avaliacao a
							INNER JOIN pse.monitoraescola mo ON mo.moeid = a.moeid
							INNER JOIN pse.acao ac ON ac.acaoid = mo.acaoid
							INNER JOIN pse.componenteavalia co ON co.comid = ac.comid
							WHERE 
								mo.moestatus = 'A' 
								AND mo.moeidentificasse in ('N','S') 
								--AND a.avlvalidar = 'S' 
								AND ac.acaoid = ".$a['acaoid']."
							".$where[2]."
							".$where[3]."
							GROUP BY mo.entid, ac.acaodescricao
							) mo2 ON mo2.entid = ent.entid
				WHERE
					ent.entstatus='A'
					".$where[0]."
					".$where[1]."
				GROUP BY
					mun.muncod,
					mun.estuf,
					mun.mundescricao
				";
			}
		}
		else{ // componente 2
				
			/*	
			if($a['acaoid'] == 11){ // Ação 3

				$sql .= "UNION
					
				SELECT  
					mun.muncod,
					mun.estuf as estado,
					mun.mundescricao as municipio,
					'".$a['componente']."' as componente,
					'".$a['acao']."' as acao,
					coalesce(sum(mp.mpaacao04comp3/totalescola),0) as qtdpactuada,
					coalesce(sum(mo2.qtdrealizada),0) as qtdrealizada,
					(case when coalesce(sum(mo2.qtdrealizada),0) > coalesce(sum(mp.mpaacao04comp3/totalescola),0) then
							100.00
						  else
						  	(coalesce(sum(mo2.qtdrealizada),0) * 100) / coalesce(sum(mp.mpaacao04comp3/totalescola),0)
					end) as percentualatingido,
					0 as qtdalterada
				FROM 
					pse.planoacaoc2 pac
				INNER JOIN pse.contratualizacao 	con ON con.conid = pac.conid and con.arqid is not null
				INNER JOIN pse.metapactuada 		 mp ON mp.conid   = pac.conid
				INNER JOIN entidade.entidade 		ent ON ent.entid  = pac.entid
				INNER JOIN entidade.endereco 	   ende ON ende.entid = ent.entid
				INNER JOIN territorios.municipio 	mun ON mun.muncod = ende.muncod
				LEFT  JOIN (SELECT count(pacid) as totalescola, conid 
							FROM pse.planoacaoc2 group by conid) te ON te.conid = pac.conid
				LEFT  JOIN (SELECT sum(a.avcquantidade) AS qtdrealizada, mo.entid, ac.acaodescricao as acao
							FROM pse.avaliacomp2 a
							INNER JOIN pse.monitoraescola 	mo ON mo.moeid = a.moeid
							INNER JOIN pse.acao 			ac ON ac.acaoid = mo.acaoid
							INNER JOIN pse.componenteavalia co ON co.comid = ac.comid
							WHERE mo.moestatus = 'A' 
							AND mo.moeidentificasse in ('N','S') 
							--AND a.avcvalidar = 'S' 
							AND ac.acaoid = ".$a['acaoid']."
							".$where[2]."
							".$where[3]."
							GROUP BY mo.entid, ac.acaodescricao
							) mo2 ON mo2.entid = ent.entid
				WHERE
					ent.entstatus='A'
					".$where[0]."
					".$where[1]."
				GROUP BY
					mun.muncod,
					mun.estuf,
					mun.mundescricao
			";


					
			}else{
			*/
			
				$sql .= "UNION
					
				SELECT  
					mun.muncod,
					mun.estuf as estado,
					mun.mundescricao as municipio,
					'".$a['componente']."' as componente,
					'".$a['acao']."' as acao,
					coalesce(sum(pac.pacquanteduseratend),0) as qtdpactuada,
					coalesce(sum(mo2.qtdrealizada),0) as qtdrealizada,
					(case when coalesce(sum(mo2.qtdrealizada),0) > coalesce(sum(pac.pacquanteduseratend),0) then
							100.00
						  else
						  	(coalesce(sum(mo2.qtdrealizada),0) * 100) / coalesce(sum(pac.pacquanteduseratend),0)
					end) as percentualatingido,
					0 as qtdalterada
				FROM 
					pse.planoacaoc2 pac
				INNER JOIN pse.contratualizacao 	con ON con.conid = pac.conid and con.arqid is not null
				INNER JOIN entidade.entidade 		ent ON ent.entid = pac.entid
				INNER JOIN entidade.endereco 	   ende ON ende.entid = ent.entid
				INNER JOIN territorios.municipio 	mun ON mun.muncod = ende.muncod
				LEFT  JOIN (SELECT sum(a.avcquantidade) AS qtdrealizada, mo.entid, ac.acaodescricao as acao
							FROM pse.avaliacomp2 a
							INNER JOIN pse.monitoraescola mo ON mo.moeid = a.moeid
							INNER JOIN pse.acao ac ON ac.acaoid = mo.acaoid
							INNER JOIN pse.componenteavalia co ON co.comid = ac.comid
							WHERE 
								mo.moestatus = 'A' 
								AND mo.moeidentificasse in ('N','S')
								AND a.avctipo = '1' 
								--AND a.avcvalidar = 'S' 
								AND ac.acaoid = ".$a['acaoid']."
								".$where[2]."
								".$where[3]."
							GROUP BY mo.entid, ac.acaodescricao
							) mo2 ON mo2.entid = ent.entid
				WHERE
					ent.entstatus='A'
					".$where[0]."
					".$where[1]."
				GROUP BY
					mun.muncod,
					mun.estuf,
					mun.mundescricao
				";
			/*	
			}
			*/
		}
	}

	$sql = substr($sql, 5);
	$sql = $sql . " ORDER BY 2, 3, 4, 5, 6 ";
// ver($sql,d);
	return $sql;
}


function monta_agp(){
	$agrupador = $_POST['agrupador'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"qtdpactuada",
											"qtdrealizada",
											"percentualatingido",
											"qtdalterada"		
										  )	  
				);
				
	foreach ($agrupador as $val):
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);					
		    	continue;
		    break;
		    case 'componente':
				array_push($agp['agrupador'], array(
													"campo" => "componente",
											  		"label" => "Componente")										
									   				);					
		    	continue;
		    break;
		    case 'acao':
				array_push($agp['agrupador'], array(
													"campo" => "acao",
											  		"label" => "Acao")										
									   				);					
		    	continue;
		    break;
		    case 'escola':
				array_push($agp['agrupador'], array(
													"campo" => "escola",
											  		"label" => "Escola")										
									   				);					
		    	continue;
		    break;
		}
		$count++;
	endforeach;
	
	return $agp;
}


function monta_coluna(){

	$coluna    = array(
						array(
							  "campo" => "qtdpactuada",
					   		  "label" => "Quantidade Pactuada",
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente') 
						),
						array(
							  "campo" => "qtdrealizada",
					   		  "label" => "Quantidade Realizada",
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente')
						),
						array(
							  "campo" => "percentualatingido",
					   		  "label" => "(%) Percentual Atingido",
							  "php"	  => array("expressao" => "{qtdpactuada} > 0",
											   "type"	   => "numeric",
											   "html"	   => "{percentual} %",
											   "var" 	   => "percentual",
											   "true"	   => "number_format( (({qtdrealizada} * 100) / {qtdpactuada} > 100 ? 100 : ({qtdrealizada} * 100) / {qtdpactuada}),2,',','.')",
											   "false"	   => "0"),
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente')
						),
						array(
							  "campo" => "qtdalterada",
					   		  "label" => "Quantidade Educandos com Alteração",
							  "type"  => "numeric",
							  "blockAgp" => array('estado','municipio','componente')
						)
					);
	return $coluna;	
/*	
	$arrPerfil = array(
						PERFIL_SUPERUSUARIO,
						PERFIL_ADMINISTRADOR
					  );
	
	$permissao = academico_possui_perfil($arrPerfil);
	
	$coluna    = array(
						array(
							  "campo" => "projetado",
					   		  "label" => "Projetado <br/>(A)",
							  "type"  => "numeric"
						),
	//					array(
	//						  "campo" => "autorizado",
	//				   		  "label" => "Autorizado",
	//						  "type"  => "numeric"
	//					),
						array(
							  "campo" => "autorizado",
					   		  "label" => "Concursos <br/>Autorizados <br/>(B)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "publicado",
					   		  "label" => "Concursos <br/>Publicados <br/>(C)",
							  "type"  => "numeric"
						)
					);
					
	if ( $permissao ){				
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric",
								  "html"  	 => "<span style='color:{color}'>{homologado}</span>",
								  "php" 	 => array(
									  					"expressao" => "{homologado} > {autorizado}",
														"var" 		=> "color",
														"true"	    => "red",
														"false"     => "#0066CC",
									  				 )	
							)
				  );			
	}else{
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric"
								 )
				  );			

	}
	
	array_push($coluna,array(
							  "campo" => "provimento",
					   		  "label" => "Provimentos <br/>Autorizados <br/>(E)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "lepvlrprovefetivados",
					   		  "label" => "Provimentos <br/>Efetivados <br/>(F)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "provimentosnaoefetivados",
					   		  "label" => "Provimentos <br/>Não Efetivados <br/>(G)=(E)-(F)",
							  "type"  => "numeric"
						)
				);	

				
	if ( $permissao ){				
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric",
									  "html"  => "{valorcampo}",
									  "php"   => array(
									  					"expressao" => "({homologado} - {provimento} >= 0) || ({provimentopendencia} > 0)",
														"type"		=> "string",
									  					"var"       => "valorcampo",
														"true"		=> "{provimentopendencia}",
														"false"		=> "<span style='color:red;'>-</span>"					
									  				   )
								)
				  );			
	}else{
		array_push($coluna, array(
									  "campo" => "provimentopendencia",
							   		  "label" => "Aut. Provimento <br/>Pendente <br/>(H)=(D)-(E)",
									  "type"  => "numeric"		
								  )
				  );			

	}			

	return $coluna;			  	
*/	
}
?>


<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
/*
$(function() {

	 //esconde todas as ações 7 do componete 1
	 $('[id*=componente_1_1acao_6_1_img]').hide();

	 //esconde todas as ações 3 do componete 2
	 $('[id*=componente_2_1acao_3_1_img]').hide();
	 
});
*/
</script>
