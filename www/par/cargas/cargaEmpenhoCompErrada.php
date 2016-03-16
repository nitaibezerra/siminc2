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
			empid_original,
		    empid_cancelado,
		    empnumeroprocesso,
		    empnumero,
		    empnumero_cancelado,
		    empcodigoespecie,
		    vrlempenho,
		    vrlcancelado,
		    par, obra, pac
		from(
		    select
		        e.empid as empid_original,
		        ep.empid as empid_cancelado,
		        e.empnumeroprocesso,
		        ep.empnumero as empnumero_cancelado,
		        e.empnumero,
		        e.empcodigoespecie,
		        sum(e.empvalorempenho) as vrlempenho,
		        sum(ep.vrlcancelado) as vrlcancelado,
		        (select count(prpid) from par.processopar where prpnumeroprocesso = e.empnumeroprocesso and prpstatus = 'A') as par,
		        (select count(proid) from par.processoobraspar where pronumeroprocesso = e.empnumeroprocesso and prostatus = 'A') as obra,
		        (select count(proid) from par.processoobra where pronumeroprocesso = e.empnumeroprocesso and prostatus = 'A') as pac
		    from
		        par.empenho e
		        left join (
		                select empnumeroprocesso, empidpai, empid, sum(empvalorempenho) as vrlcancelado, empnumero 
		                from par.empenho
		                where empcodigoespecie in ('03', '13')
							and empstatus = 'A'
		                group by 
		                    empnumeroprocesso,
		                    empnumero,
		                    empidpai, empid
		        ) as ep on ep.empidpai = e.empid
		    where
		        e.empcodigoespecie = '01'
				and e.empstatus = 'A'
		    group by 
		        e.empid,
		        e.empnumeroprocesso,
		        e.empnumero,
		        e.empcodigoespecie,
		        ep.empid,
		        ep.empnumero
		) as foo
		where
			vrlempenho = vrlcancelado
			--and empnumeroprocesso = '23400010197201220'
			--and pac > 0
		";
$arrEmpenho = $db->carregar($sql);
$arrEmpenho = $arrEmpenho ? $arrEmpenho : array();

$arrAtualizadoPAC = array();
$arrAtualizadoObra = array();

foreach ($arrEmpenho as $key => $v) {
	$arrObras = array();
	
	if( (int)$v['par'] > 0 ){
		/* $sql = "SELECT sbaid, empid, eobpercentualemp, eobvalorempenho, eobano FROM par.empenhosubacao WHERE empid = {$v['empid_original']}";
		$arrCompOriginal = $db->carregar($sql);
		$arrCompOriginal = $arrCompOriginal ? $arrCompOriginal : array();
		
		foreach ($arrCompOriginal as $original) {
			$sql = "SELECT eobid, sbaid, empid, eobpercentualemp, eobvalorempenho, eobano FROM par.empenhosubacao 
						WHERE empid = {$v['empid_cancelado']} and sbaid = {$original['sbaid']} and eobano = {$original['eobano']}";
			$arrCompCancelado = $db->pegaLinha($sql);
			$arrCompCancelado = $arrCompCancelado ? $arrCompCancelado : array();
			
			if( $arrCompCancelado['eobid'] ){
				$sql = "UPDATE par.empenhosubacao SET 
						  	sbaid 				= {$original['sbaid']},
						  	eobpercentualemp 	= {$original['eobpercentualemp']},
						  	eobvalorempenho 	= {$original['eobvalorempenho']},
						  	eobano 				= {$original['eobano']}						 
						WHERE eobid 			= {$original['eobid']}";
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano) 
						VALUES ({$original['sbaid']}, {$original['empid']}, {$original['eobpercentualemp']}, {$original['eobvalorempenho']}, {$original['eobano']})";
				$db->executar($sql);
				$db->commit();
			}
		} */
	} elseif( (int)$v['obra'] > 0 ){ 
		$sql = "select o.eobpercentualemp, o.preid, o.eobvalorempenho, eobpercentualemp2 from par.empenhoobrapar o where o.empid = {$v['empid_original']} and eobstatus = 'A'";
		$arrCompOriginal = $db->carregar($sql);
		$arrCompOriginal = $arrCompOriginal ? $arrCompOriginal : array();
		
		foreach ($arrCompOriginal as $original) {
			$sql = "select o.eobid, o.eobpercentualemp, o.preid, o.eobvalorempenho from par.empenhoobrapar o where o.empid = {$v['empid_cancelado']} and preid = {$original['preid']} and eobstatus = 'A'";
			$arrCompCancelado = $db->pegaLinha($sql);
			$arrCompCancelado = $arrCompCancelado ? $arrCompCancelado : array();
			
			if( (float)$original['eobvalorempenho'] <> (float)$arrCompCancelado['eobvalorempenho'] ){
				
				array_push($arrObras, $original['preid']);
				
				if( $arrCompCancelado['eobid'] ){
					$sql = "UPDATE par.empenhoobrapar SET
		  						eobpercentualemp2 	= ".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null').",
		  						eobvalorempenho 	= {$original['eobvalorempenho']},
		  						eobpercentualemp 	= ".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null')." 
							WHERE eobid 			= {$arrCompCancelado['eobid']}";
					$db->executar($sql);
					$db->commit();
				} else {
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp) 
							VALUES ({$original['preid']}, {$v['empid_cancelado']}, ".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null').", {$original['eobvalorempenho']}, 
								".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null').")";
					$db->executar($sql);
					$db->commit();
				}
			}
		}
		if( $arrObras[0] ){
			array_push($arrAtualizadoObra, array(
											'processo' => $v['empnumeroprocesso'],
											'empenho_original' => $v['empnumero'],
											'empenho_cancelado' => $v['empnumero_cancelado'],
											'preid' => $arrObras,
											)
					);
		}
		
	} elseif( (int)$v['pac'] > 0 ){
		$sql = "select o.eobpercentualemp, o.preid, o.eobvalorempenho, eobpercentualemp2 from par.empenhoobra o where o.empid = {$v['empid_original']} and eobstatus = 'A'";
		$arrCompOriginal = $db->carregar($sql);
		$arrCompOriginal = $arrCompOriginal ? $arrCompOriginal : array();
		
		foreach ($arrCompOriginal as $original) {
			$sql = "select eobid, o.eobpercentualemp, o.preid, o.eobvalorempenho from par.empenhoobra o where o.empid = {$v['empid_cancelado']} and preid = {$original['preid']} and eobstatus = 'A'";
			$arrCompCancelado = $db->pegaLinha($sql);
			$arrCompCancelado = $arrCompCancelado ? $arrCompCancelado : array();
			//ver($arrCompCancelado, $v['empid_cancelado'], $original['preid']);
			if( (float)$original['eobvalorempenho'] <> (float)$arrCompCancelado['eobvalorempenho'] ){
				
				array_push($arrObras, $original['preid']);
				
				if( $arrCompCancelado['eobid'] ){
					$sql = "UPDATE par.empenhoobra SET
		  						eobpercentualemp2 	= ".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null').",
		  						eobvalorempenho 	= {$original['eobvalorempenho']},
		  						eobpercentualemp 	= ".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null')." 
							WHERE eobid 			= {$arrCompCancelado['eobid']}";
					$db->executar($sql);
					$db->commit();
				} else {
					$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp) 
							VALUES ({$original['preid']}, {$v['empid_cancelado']}, ".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null').", 
										{$original['eobvalorempenho']}, ".($original['eobpercentualemp'] ? $original['eobpercentualemp'] : 'null').")";
					$db->executar($sql);
					$db->commit();
				}
			}
		}
		if( $arrObras[0] ){
			array_push($arrAtualizadoPAC, array(
											'processo' => $v['empnumeroprocesso'],
											'empenho_original' => $v['empnumero'],
											'empenho_cancelado' => $v['empnumero_cancelado'],
											'preid' => $arrObras,
											)
					);
		}
	}
}

ver($arrAtualizadoObra, $arrAtualizadoPAC,d);
























?>