<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select
				empenho,
				processo,
			    notaempenho,
			    sequencial,
			    especie,
				vrlempenho,
			    vrlempcomposicao,
			    tipo,
			    diferenca
			from(
			    select
			    	coalesce(e.empvalorempenho, 0) as vrlempenho,
			        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
			        (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
			        e.empid as empenho,
			        e.empnumeroprocesso as processo,
			        e.empnumero as notaempenho,
			        e.empprotocolo as sequencial,
			        e.empcodigoespecie as especie,
			        'PAR' as tipo
			    from
			        par.processopar pp
			        inner join par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso
			        left join par.empenhosubacao es on es.empid = e.empid and es.eobstatus = 'A'
			    where
			        pp.prpstatus = 'A'
			        and e.empstatus = 'A'
			        and e.empsituacao <> 'CANCELADO'
			    group by
			        e.empid,
			        e.empnumeroprocesso,
			        e.empnumero,
			        e.empcodigoespecie,
			        e.empprotocolo,
			        e.empvalorempenho,
			        e.empvalorempenho
			        
			    UNION ALL
			
			    select 
			        coalesce(e.empvalorempenho, 0) as vrlempenho,
			        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
			        (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
			        e.empid as empenho,
			        e.empnumeroprocesso as processo,
			        e.empnumero as notaempenho,
			        e.empprotocolo as sequencial,
			        e.empcodigoespecie as especie,
			        'OBRA' as tipo
			    from
			        par.processoobraspar pp
			        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
			        left join par.empenhoobrapar es on es.empid = e.empid and es.eobstatus = 'A'
			    where
			        pp.prostatus = 'A'
			        and e.empstatus = 'A'
			        and e.empsituacao <> 'CANCELADO'
			    group by
			        e.empid,
			        e.empnumeroprocesso,
			        e.empnumero,
			        e.empcodigoespecie,
			        e.empprotocolo,
			        e.empvalorempenho
			        
			    UNION ALL
			
			    select 
			        coalesce(e.empvalorempenho, 0) as vrlempenho,
			        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
			        (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
			        e.empid as empenho,
			        e.empnumeroprocesso as processo,
			        e.empnumero as notaempenho,
			        e.empprotocolo as sequencial,
			        e.empcodigoespecie as especie,
			        'PAC' as tipo
			    from
			        par.processoobra pp
			        inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
			        left join par.empenhoobra es on es.empid = e.empid and es.eobstatus = 'A'
			    where
			        pp.prostatus = 'A'
			        and e.empstatus = 'A'
			        and e.empsituacao <> 'CANCELADO'
			    group by
			        e.empid,
			        e.empnumeroprocesso,
			        e.empnumero,
			        e.empcodigoespecie,
			        e.empprotocolo,
			        e.empvalorempenho
			) as foo
			where
				vrlempenho > vrlempcomposicao
				--and diferenca > 1
				--and processo = '23400000227201117' 
                and especie in ('03', '04', '13')";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

/**
 * Empenho de SUBAÇÃO DO PAR
 * */
foreach ($arrDados as $v) {	
	if( $v['tipo'] == 'PAR' ){
		$arEmpenhoPai = $db->pegaLinha("select empidpai, count(empid) as totEmpid from par.empenho where empid = ".$v['empenho']." group by empidpai");
		$empidpai = $arEmpenhoPai['empidpai'];
		$totEmpidPai = $arEmpenhoPai['totempid'];
		
		$vrlEmpenhoPai = 0;
		if( $empidpai ) $vrlEmpenhoPai = $db->pegaUm("select empvalorempenho from par.empenho where empid = $empidpai");
		
		if( (int)$totEmpidPai == (int)1 && (float)$vrlEmpenhoPai == (float)$v['vrlempenho'] ){
			$sql = "SELECT 
					  	empid, 
					  	eobano, 
					  	sbaid, 
					  	eobvalorempenho
					FROM 
					  	par.empenhosubacao WHERE empid = $empidpai and eobstatus = 'A'";
			$arrEmpenhoCompPai = $db->carregar($sql);
			$arrEmpenhoCompPai = $arrEmpenhoCompPai ? $arrEmpenhoCompPai : array();
			
			foreach ($arrEmpenhoCompPai as $empPai) {
				$totCompfilho = $db->pegaUm("select count(eobid) from par.empenhosubacao where empid = {$v['empenho']} and sbaid = {$empPai['sbaid']} and eobano = {$empPai['eobano']} and eobstatus = 'A'");
				
				if( (int)$totCompfilho == (int)0 ){
					$sql = "INSERT INTO par.empenhosubacao(eobano, sbaid, empid, eobvalorempenho, eobpercentualemp)
							VALUES ({$empPai['eobano']}, {$empPai['sbaid']}, {$v['empenho']}, {$empPai['eobvalorempenho']}, 0)";						
					$db->executar($sql);
				} else {
					$sql = "UPDATE par.empenhosubacao SET
								eobvalorempenho = {$empPai['eobvalorempenho']}
							WHERE empid = {$v['empenho']} 
								and sbaid = {$empPai['sbaid']} 
								and eobano = {$empPai['eobano']}";
					$db->executar($sql);
				}
				$db->commit();
			}
		}
	}
}

/**
 * Empenho de OBRAS PAR
 * */
foreach ($arrDados as $v) {
	if( $v['tipo'] == 'OBRA' ){
		$arEmpenhoPai = $db->pegaLinha("select empidpai, count(empid) as totEmpid from par.empenho where empid = ".$v['empenho']." group by empidpai");
		$empidpai = $arEmpenhoPai['empidpai'];
		$totEmpidPai = $arEmpenhoPai['totempid'];
		
		$vrlEmpenhoPai = 0;
		if( $empidpai ) $vrlEmpenhoPai = $db->pegaUm("select empvalorempenho from par.empenho where empid = $empidpai");
		
		if( (int)$totEmpidPai == (int)1 && (float)$vrlEmpenhoPai == (float)$v['vrlempenho'] ){
			$sql = "SELECT 
					  	eobid,
					  	preid,
					  	empid,
					  	eobvalorempenho
					FROM 
					  	par.empenhoobrapar WHERE empid = $empidpai and eobstatus = 'A'";
			$arrEmpenhoCompPai = $db->carregar($sql);
			$arrEmpenhoCompPai = $arrEmpenhoCompPai ? $arrEmpenhoCompPai : array();
			
			foreach ($arrEmpenhoCompPai as $empPai) {
				$totCompfilho = $db->pegaUm("select count(eobid) from par.empenhoobrapar where empid = {$v['empenho']} and preid = {$empPai['preid']} and eobstatus = 'A'");
				
				if( (int)$totCompfilho == (int)0 ){
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
							VALUES ({$empPai['preid']}, {$v['empenho']}, 0, {$empPai['eobvalorempenho']}, 0)";						
					$db->executar($sql);
				} else {
					$sql = "UPDATE par.empenhoobrapar SET
								eobvalorempenho = {$empPai['eobvalorempenho']}
							WHERE empid = {$v['empenho']} 
								and preid = {$empPai['preid']}";
					$db->executar($sql);
				}
				$db->commit();
			}
		}
	}
}


/**
 * Empenho de OBRAS PAC
 * */

foreach ($arrDados as $v) {
	if( $v['tipo'] == 'PAC' ){
		$arEmpenhoPai = $db->pegaLinha("select empidpai, count(empid) as totEmpid from par.empenho where empid = ".$v['empenho']." group by empidpai");
		$empidpai = $arEmpenhoPai['empidpai'];
		$totEmpidPai = $arEmpenhoPai['totempid'];
		
		$vrlEmpenhoPai = 0;
		if( $empidpai ) $vrlEmpenhoPai = $db->pegaUm("select empvalorempenho from par.empenho where empid = $empidpai");
		
		if( (int)$totEmpidPai == (int)1 && (float)$vrlEmpenhoPai == (float)$v['vrlempenho'] ){
			$sql = "SELECT 
					  	eobid,
					  	preid,
					  	empid,
					  	eobvalorempenho
					FROM 
					  	par.empenhoobra WHERE empid = $empidpai and eobstatus = 'A'";
			$arrEmpenhoCompPai = $db->carregar($sql);
			$arrEmpenhoCompPai = $arrEmpenhoCompPai ? $arrEmpenhoCompPai : array();
			
			foreach ($arrEmpenhoCompPai as $empPai) {
				$totCompfilho = $db->pegaUm("select count(eobid) from par.empenhoobra where empid = {$v['empenho']} and preid = {$empPai['preid']} and eobstatus = 'A'");
				
				if( (int)$totCompfilho == (int)0 ){
					$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
							VALUES ({$empPai['preid']}, {$v['empenho']}, 0, {$empPai['eobvalorempenho']}, 0)";						
					$db->executar($sql);
				} else {
					$sql = "UPDATE par.empenhoobra SET
								eobvalorempenho = {$empPai['eobvalorempenho']}
							WHERE empid = {$v['empenho']} 
								and preid = {$empPai['preid']}";
					$db->executar($sql);
				}
				$db->commit();
			}
		}
	}
}
























?>