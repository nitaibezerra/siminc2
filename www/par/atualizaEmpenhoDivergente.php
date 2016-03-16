<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
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
			    tipo
			from(			
			    select 
			        coalesce(e.empvalorempenho, 0) as vrlempenho,
			        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
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
                vrlempenho = '0.01'
                and vrlempcomposicao = 0";

	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	foreach ($arrDados as $v) {
		$sql = "select eobid from par.empenhoobra where empid = {$v['empenho']} and eobstatus = 'A'";
		$eobid = $db->pegaUm($sql);
		
		if( !empty($eobid) ){
			$sql = "UPDATE par.empenhoobra SET
  						eobpercentualemp2 = 0,
						eobvalorempenho = {$v['vrlempenho']},
						eobpercentualemp = 0
					WHERE eobid = $eobid";
			$db->executar($sql);
		} else {
			$sql = "select preid from par.empenhoobra e where empid in (select empidpai from par.empenho where empid = {$v['empenho']})";
			$preid = $db->pegaUm($sql);
			$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp) 
					VALUES ($preid, {$v['empenho']}, 0, {$v['vrlempenho']}, 0)";
			$db->executar($sql);
		}
		$db->commit();
	}

?>