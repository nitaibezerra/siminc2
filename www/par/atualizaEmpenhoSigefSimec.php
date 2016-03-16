<?php
ini_set("memory_limit", "1024M");
set_time_limit(3);

include_once "config.inc";
//include_once "/var/www/simec/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/**** DECLARAÇÃO DE VARIAVEIS ****/
session_start();
$db = new cls_banco();

$req_processo = $_REQUEST['processo'];

if( $req_processo ){
	$filtroProc = "'$req_processo'";
} else {
	$filtroProc = "select distinct ems_numero_processo from  par.empenhosigef";
}

$sql = "SELECT emsid, prpid, proidpac, proidpar, empid, teeid, ems_numero_processo, ems_cnpj, ems_programa_fnde, ems_unidade_gestora, ems_numero_do_empenho, ems_numero_do_empenho_pai,
		  	ems_valor_empenho, ems_numero_sequencial_da_ne, ems_nu_seq_mov_ne, ems_data_empenho, ems_cpf, ems_numero_sistema, ems_descricao_do_empenho, ems_ano_do_empenho,
		  	ems_centro_de_gestao, ems_codigo_nat_despesa, ems_fonte_recurso, ems_ptres, ems_esfera, ems_pi, ems_codigo_especie, ems_situacao_do_empenho,
			case when prpid is not null then 'PAR'
	                 when proidpar is not null then 'OBRA'
	                 when proidpac is not null then 'PAC'
            else '' end tipo
		FROM 
		  	par.empenhosigef e 
		where ems_numero_processo in ( 
							$filtroProc
						)
		ORDER BY ems_numero_do_empenho";

/*
 * select distinct empnumeroprocesso from par.empenho 
								where empsituacao <> 'CANCELADO'
								and empcodigoespecie = '01' and empnumeroprocesso not in (select empnumeroprocesso from par.empenho where empcodigoespecie ='03' and empsituacao <> 'CANCELADO')
								and empnumeroprocesso in
								(select ems_numero_processo from par.empenhosigef where ems_codigo_especie ='03' and ems_situacao_do_empenho <> 'CANCELADO')
								and empnumeroprocesso IN (SELECT PRONUMEROPROCESSO FROM par.processoobra)
 * */

//23400003046201134 - 2 cancelamento - PAR
//23400012186201365 - 2 cancelamento - PAC
//23400000428201297 pac
//23400004926201390 pac
//23400004888201375 obras par
//23400008648201340 par

$arrDadosEmp = $db->carregar($sql);
$arrDadosEmp = $arrDadosEmp ? $arrDadosEmp : array();
//ver($arrDadosEmp,d);
foreach ($arrDadosEmp as $key => $v) {
	$sql = "select coalesce(empid, 0) from par.empenho e where e.empnumeroprocesso = '{$v['ems_numero_processo']}' and e.empprotocolo ilike '%{$v['ems_numero_sequencial_da_ne']}%'";
	$empenho = $db->pegaUm($sql);
	$empenho = $empenho ? $empenho : '0';
	
	$tipo = $v['tipo'];
	 
	$empnumeroFilho = $v['ems_ano_do_empenho'].'NE'.$v['ems_numero_do_empenho'];
	$ems_situacao_do_empenho = trim($v['ems_situacao_do_empenho']);
	if( $ems_situacao_do_empenho == 'EFETIVADO' ) $ems_situacao_do_empenho = '2 - EFETIVADO';
	
	if( (int)$empenho == (int)0 ){
		
		if( $v['ems_codigo_especie'] == '03' || $v['ems_codigo_especie'] == '13' || $v['ems_codigo_especie'] == '04' ){
			
			$sql = "SELECT empnumeroprocesso, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empvalorempenho, empcodigoespecie, 
						empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente, 
					    ds_problema, valor_saldo_pagamento, tp_especializacao, co_diretoria, empid, usucpf, empid
					FROM par.empenho e
					WHERE empnumeroprocesso = '{$v['ems_numero_processo']}'
						and empprotocolo = '{$v['ems_nu_seq_mov_ne']}' 
						and empstatus = 'A' 
					";
			$arEmpPai = $db->pegaLinha($sql);
			$arEmpPai = $arEmpPai ? $arEmpPai : array();
			
			#Verifica se tem empenho pai, caso tenha, insere todas as informações de empenho que estão cadastrada no SIGEF e não estão no SIMEC
			if( !empty($arEmpPai) ){
				$empcentrogestaosolic 	= $arEmpPai['empcentrogestaosolic'];
				$empanoconvenio 		= $arEmpPai['empanoconvenio'];
				$empvalorempenho 		= $arEmpPai['empvalorempenho'];
				$empnumeroconvenio 		= $arEmpPai['empnumeroconvenio'];
				$empcodigoobs 			= $arEmpPai['empcodigoobs'];
				$empcodigotipo 			= $arEmpPai['empcodigotipo'];
				$empdescricao 			= $arEmpPai['empdescricao'];
				$empgestaoeminente 		= $arEmpPai['empgestaoeminente'];
				$empunidgestoraeminente = $arEmpPai['empunidgestoraeminente'];
				$empanoconvenio 		= $arEmpPai['empanoconvenio'];
				$empnumeroconvenio		= $arEmpPai['empnumeroconvenio'];
				$valor_saldo_pagamento	= $arEmpPai['valor_saldo_pagamento'];
				$empnumero				= $arEmpPai['empnumero'];
				$usucpf					= $arEmpPai['usucpf'];
				$tp_especializacao		= $arEmpPai['tp_especializacao'];
				$co_diretoria			= $arEmpPai['co_diretoria'];
				$empid					= $arEmpPai['empid'];
				
				$empanoconvenio = $empanoconvenio ? $empanoconvenio : 'null';
				$empnumeroconvenio = $empnumeroconvenio ? $empnumeroconvenio : 'null';
				$valor_saldo_pagamento = $valor_saldo_pagamento ? $valor_saldo_pagamento : 'null';
				
				#insere empenho filho
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empcodigopi, 
							empcodigoesfera, empcodigoptres, empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, 
							empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
		  					empprogramafnde, empnumerosistema, empsituacao, usucpf, empprotocolo, empnumero, 
		  					empvalorempenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento,
		  					empdata, tp_especializacao, co_diretoria, empidpai, teeid, empcarga)				
						VALUES('{$v['ems_cnpj']}', '{$v['ems_numero_do_empenho']}', '{$v['ems_ano_do_empenho']}', '{$v['ems_numero_processo']}', '{$v['ems_codigo_especie']}', '{$v['ems_pi']}', 
							'{$v['ems_esfera']}', '{$v['ems_ptres']}', '{$v['ems_fonte_recurso']}', '{$v['ems_codigo_nat_despesa']}', '$empcentrogestaosolic', $empanoconvenio, $empnumeroconvenio, 
							'$empcodigoobs', '$empcodigotipo', '$empdescricao', '$empgestaoeminente', '$empunidgestoraeminente',
		  					'{$v['ems_programa_fnde']}', '{$v['ems_numero_sistema']}', '{$ems_situacao_do_empenho}', '$usucpf', '{$v['ems_numero_sequencial_da_ne']}', '{$empnumeroFilho}', 
		  					'{$v['ems_valor_empenho']}', '$ds_problema', '{$v['ems_valor_empenho']}', $valor_saldo_pagamento,
		  					'{$v['ems_data_empenho']}', '$tp_especializacao', '$co_diretoria', '$empid', {$v['teeid']}, 'S') returning empid";
				
				$empidNovo = $db->pegaUm($sql);
				
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho)
	    				VALUES ('', $empidNovo, '{$v['ems_data_empenho']}', '2 - EFETIVADO', '{$v['ems_codigo_especie']}');";
				$db->executar($sql);
				
				/*
				 * Insere as vunculações do empenho filho*/
				if( $tipo == 'PAR' ){
					$totSbaid = $db->pegaUm("select count(sbaid) from par.empenhosubacao where empid = $empid and eobstatus = 'A'");
					
					if( (int)$totSbaid == (int)1 ){
						/*Caso tenha somente uma subação vinculada ao empenho*/
						
						$arEmpenho = $db->pegaLinha("select eobvalorempenho, par.recuperavalorvalidadossubacaoporano(sbaid, eobano) as vrlsubacao, eobpercentualemp, eobano, sbaid,
															par.recuperavalorplanejadossubacaoporano(sbaid, eobano) as vrlsubacaopla
														from par.empenhosubacao where empid = $empid and eobstatus = 'A'");
						
						$vrlsubacao = ($arEmpenho['vrlsubacao'] == 0 ? $arEmpenho['vrlsubacaopla'] : $arEmpenho['vrlsubacao']);
						$eobano = $arEmpenho['eobano'];
						$sbaid = $arEmpenho['sbaid'];
						$eobpercentualemp = $arEmpenho['eobpercentualemp'];
						$eobvalorempenho = $empvalorempenho;
						$ems_valor_empenho = $v['ems_valor_empenho'];
						
						$percent1 = 1; //number_format($ems_valor_empenho*100/$eobvalorempenho, 2);
						
						$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano) 
								VALUES ($sbaid, $empidNovo, {$percent1}, {$ems_valor_empenho}, '{$eobano}')";
						$db->executar($sql);
												
						$percent = 0; //number_format($eobvalorempenho*100/$vrlsubacao, 2); 
						
						$sql = "UPDATE par.empenhosubacao SET
									eobpercentualemp = $eobpercentualemp,
								  	eobvalorempenho = $eobvalorempenho
								WHERE
									empid = $empid";
						$db->executar($sql);
					} else { 
						/*Caso tenha mais de uma subação vinculada ao empenho*/
						
						$sql = "SELECT eop.eobid, eop.sbaid, eop.empid, eop.eobano, 
								    sum(eop.eobvalorempenho) as vrlempenho, 
								    sum(esr.esrvalorreduzido) as vrlreduzido,
								    par.recuperavalorvalidadossubacaoporano(eop.sbaid, eop.eobano) as vrlsubacao
								FROM 
								    par.empenhosubacao eop
								    inner join par.empenhosubacaoreducao esr on esr.eobid = eop.eobid and eobstatus = 'A'
								WHERE eop.empid = $empid
								GROUP BY
								    eop.eobid, eop.empid, eop.eobano, eop.sbaid";
						
						$arSubEmp = $db->carregar($sql);
						$arSubEmp = $arSubEmp ? $arSubEmp : array();
						
						/*Se o valor do empenho pai e igual o valor do empenho de cancelamento*/
						if( $empvalorempenho == $v['ems_valor_empenho'] ){
							foreach ($arSubEmp as $emp) {
								$vrlsubacao = $emp['vrlsubacao'];
								$sbaid 		= $emp['sbaid'];
								$eobano 	= $emp['eobano'];
								$vrlempenho	= $emp['vrlempenho'];
									
								$percent = 100;
									
								$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
										VALUES ($sbaid, $empidNovo, $percent, ".($empvalorempenho / sizeof($arSubEmp)).", '$eobano')";
								$db->executar($sql);
									
								$percent1 = number_format($empvalorempenho*100/$vrlsubacao, 2);
									
								$sql = "update par.empenhosubacao set
											eobvalorempenho = ".($empvalorempenho / sizeof($arSubEmp)).",
											eobpercentualemp = {$percent1}
										where eobid = {$emp['eobid']}";
								$db->executar($sql);
							}
						} else {
							/*Se o valor do empenho pai e diferente do valor do empenho de cancelamento, busca informações na tabela de redução*/
							foreach ($arSubEmp as $emp) {
								$vrlsubacao = $emp['vrlsubacao'];
								$sbaid 		= $emp['sbaid'];
																			$vrlempenho	= $emp['vrlempenho'];
																				
								$sql = "SELECT eobid, esrvalororiginal, esrvalorreduzido, esrdata
										FROM par.empenhosubacaoreducao WHERE eobid = {$emp['eobid']} and eobstatus = 'A'";
								$arrReducao = $db->carregar($sql);
								$arrReducao = $arrReducao ? $arrReducao : array();
								foreach ($arrReducao as $reducao) {
	
									$eobvalorempenho 	= $reducao['esrvalorreduzido'];
									$erpvalororiginal 	= $reducao['esrvalororiginal'];
	
									$percent = 0; //number_format($eobvalorempenho*100/$vrlempenho, 2);
	
									$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano) 
											VALUES ($sbaid, $empidNovo, $percent, $eobvalorempenho, '$eobano')";
									$db->executar($sql);
	
									$percent1 = number_format($erpvalororiginal*100/$vrlsubacao, 2);
	
									$sql = "update par.empenhosubacao set
												eobvalorempenho = {$erpvalororiginal},
												eobpercentualemp = {$percent1}
											where eobid = {$emp['eobid']}";
									$db->executar($sql);
								}
							}
						}
					}
				} else if( $tipo == 'OBRA' ){
					$totPreid = $db->pegaUm("select count(preid) from par.empenhoobrapar where empid = $empid and eobstatus = 'A'");
					
					/*Caso tenha somente uma obra vinculada ao empenho*/
					if( (int)$totPreid == (int)1 ){
						$arEmpenho = $db->pegaLinha("select e.eobpercentualemp, e.eobvalorempenho, pe.preid, pe.prevalorobra as vrlobra 
														from par.empenhoobrapar e 
															inner join obras.preobra pe on pe.preid = e.preid and eobstatus = 'A'
														where e.empid = $empid");
						
						$vrlobra = $arEmpenho['vrlobra'];
						$preid = $arEmpenho['preid'];
						$eobvalorempenho = $empvalorempenho;
						$ems_valor_empenho = $v['ems_valor_empenho'];
						
						//$percent1 = number_format($ems_valor_empenho*100/$eobvalorempenho, 2);
						$percent1 = 0;
						
						$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp) 
								VALUES ($preid, $empidNovo, $percent1, $ems_valor_empenho, $percent1)";						
						$db->executar($sql);
												
						$percent = 0; //number_format($eobvalorempenho*100/$vrlobra, 2);
						/*
						 * Retirar esse update apos novas implementações e regra de sistema*/
						$sql = "UPDATE par.empenhoobrapar SET
									eobpercentualemp = $percent,
								  	eobvalorempenho = $eobvalorempenho
								WHERE
									empid = $empid";						
						$db->executar($sql);
					} else{
						
						/*Caso tenha mais de uma obra vinculada ao empenho*/
						
						$sql = "SELECT eop.eobid, eop.preid, eop.empid, 
									sum(eop.eobvalorempenho) as vrlempenho, 
								    sum(esr.erpvalorreduzido) as vrlreduzido,
								    pe.prevalorobra as vrlobra 
							    FROM 
							        par.empenhoobrapar eop
							        inner join obras.preobra pe on pe.preid = eop.preid and eobstatus = 'A' 
							        inner join par.empenhosubacaoobrasparreducao esr on esr.eobid = eop.eobid
							    WHERE eop.empid = $empid
							    GROUP BY
							        eop.eobid, eop.preid, pe.prevalorobra, eop.empid";
						
						$arObraEmp = $db->carregar($sql);
						$arObraEmp = $arObraEmp ? $arObraEmp : array();
						
						/*Se o valor do empenho pai e igual o valor do empenho de cancelamento*/
						
						if( $empvalorempenho == $v['ems_valor_empenho'] ){
							foreach ($arObraEmp as $emp) {
								$vrlobra 	= $emp['vrlobra'];
								$preid 		= $emp['preid'];
								$vrlempenho	= $emp['vrlempenho'];
									
								$percent = 100;
									
								$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
										VALUES ($preid, $empidNovo, $percent, ".($empvalorempenho / sizeof($arObraEmp)).", $percent)";
								$db->executar($sql);
																
								$percent1 = 0;
									
								$sql = "update par.empenhoobrapar set
											eobvalorempenho = ".($empvalorempenho / sizeof($arObraEmp)).",
											eobpercentualemp = {$percent1},
											eobpercentualemp2 = {$percent1}
										where eobid = {$emp['eobid']}";
								$db->executar($sql);
							}
						}else{
							/*Se o valor do empenho pai e diferente do valor do empenho de cancelamento, busca informações na tabela de redução*/
							
							foreach ($arObraEmp as $emp) {
								$vrlobra 	= $emp['vrlobra'];
								$preid 		= $emp['preid'];
								$vrlempenho	= $emp['vrlempenho'];
								
								$sql = "SELECT eobid, erpvalororiginal, erpvalorreduzido, erpdata, erpespecie 
										FROM par.empenhosubacaoobrasparreducao WHERE eobid = {$emp['eobid']}";
								$arrReducao = $db->carregar($sql);
								$arrReducao = $arrReducao ? $arrReducao : array();
								foreach ($arrReducao as $reducao) {
									
									$eobvalorempenho 	= $reducao['erpvalorreduzido'];
									$erpvalororiginal 	= $reducao['erpvalororiginal'];
									
									$percent = 0; // number_format($eobvalorempenho*100/$vrlempenho, 2);
									
									$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
											VALUES ($preid, $empidNovo, $percent, $eobvalorempenho, $percent)";
									$db->executar($sql);
									
									//$percent1 = number_format($erpvalororiginal*100/$vrlobra, 2);
									$percent1 = 0;
									
									$sql = "update par.empenhoobrapar set 
												eobvalorempenho = {$erpvalororiginal}, 
												eobpercentualemp = {$percent1}, 
												eobpercentualemp2 = {$percent1} 
											where eobid = {$emp['eobid']}";
									$db->executar($sql);
								}
							}
						}
 						$db->commit();
					}
				} else if( $tipo == 'PAC' ){
					$totPreid = $db->pegaUm("select count(preid) from par.empenhoobra where empid = $empid and eobstatus = 'A'");
					
					/*Caso tenha somente uma obra vinculada ao empenho*/
					
					if( (int)$totPreid == (int)1 ){
						
						$sql = "SELECT preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp FROM par.empenhoobra WHERE empid = $empid and eobstatus = 'A'";
						$arEmpenhoObra = $db->pegaLinha($sql);
						
						$arEmpenho = $db->pegaLinha("select e.eobpercentualemp, e.eobvalorempenho, pe.preid, pe.prevalorobra as vrlobra 
														from par.empenhoobra e 
															inner join obras.preobra pe on pe.preid = e.preid and eobstatus = 'A'
														where e.empid = $empid");
						
						$vrlobra 			= $arEmpenho['vrlobra'];
						$preid 				= $arEmpenho['preid'];
						$eobvalorempenho 	= $empvalorempenho;
						$ems_valor_empenho 	= $v['ems_valor_empenho'];
						
						//$percent1 = number_format($ems_valor_empenho*100/$eobvalorempenho, 2);
						$percent1 = 1;
						
						$sql = "INSERT INTO par.empenhoobra(preid, empid, eobvalorempenho, eobpercentualemp2, eobpercentualemp) 
								VALUES ($preid, $empidNovo, '{$ems_valor_empenho}', '{$percent1}', '{$percent1}')";
						$db->executar($sql);
						
						//$eobvalorempenho = (float)$eobvalorempenho - (float)$ems_valor_empenho;
						//$percent = number_format($ems_valor_empenho*100/$vrlobra, 2);
						$percent = 0;
						/*
						 * Retirar esse update apos novas implementações e regra de sistema*/
						$sql = "UPDATE par.empenhoobra SET
									eobpercentualemp = $percent,
								  	eobvalorempenho = $eobvalorempenho
								WHERE
									empid = $empid";
						
						$db->executar($sql);
					} else {
						/*
						 * Caso tenha mais uma obra no empenho
						 */
						$sql = "SELECT eop.eobid, eop.preid, eop.empid, 
								    sum(eop.eobvalorempenho) as vrlempenho, 
								    sum(esr.eprvalorreduzido) as vrlreduzido,
								    pe.prevalorobra as vrlobra 
								FROM 
								    par.empenhoobra eop
								    inner join obras.preobra pe on pe.preid = eop.preid and eobstatus = 'A' 
								    inner join par.empenhoobraspacreducao esr on esr.eobid = eop.eobid
							    WHERE eop.empid = $empid
							    GROUP BY
							        eop.eobid, eop.preid, pe.prevalorobra, eop.empid";
						
						$arPACEmp = $db->carregar($sql);
						$arPACEmp = $arPACEmp ? $arPACEmp : array();
						
						/*Se o valor do empenho pai e igual o valor do empenho de cancelamento*/
						
						if( $empvalorempenho == $v['ems_valor_empenho'] ){
							foreach ($arPACEmp as $emp) {
								$vrlobra 	= $emp['vrlobra'];
								$preid 		= $emp['preid'];
								$vrlempenho	= $emp['vrlempenho'];
									
								$percent = 100;
									
								$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
										VALUES ($preid, $empidNovo, $percent, ".($empvalorempenho / sizeof($arPACEmp)).", $percent)";
								$db->executar($sql);
								
								/* if( $vrlobra <  $empvalorempenho){
									$percent1 = number_format($vrlobra*100/$empvalorempenho, 2);
								} else {
									$percent1 = number_format($empvalorempenho*100/$vrlobra, 2);
								} */
									$percent1 = 1;
								
								$sql = "update par.empenhoobra set
											eobvalorempenho = ".($empvalorempenho / sizeof($arPACEmp)).",
											eobpercentualemp = {$percent1},
											eobpercentualemp2 = {$percent1}
										where eobid = {$emp['eobid']}";
								
								$db->executar($sql);
							}
						} else {
							
							/*Se o valor do empenho pai e diferente do valor do empenho de cancelamento, busca informações na tabela de redução*/
							
							foreach ($arPACEmp as $emp) {
								$vrlobra 	= $emp['vrlobra'];
								$preid 		= $emp['preid'];
								$vrlempenho	= $emp['vrlempenho'];
							
								$sql = "SELECT eobid, eprvalororiginal, eprvalorreduzido, eprdata, eprespecie
										FROM par.empenhoobraspacreducao WHERE eobid = {$emp['eobid']} and eobstatus = 'A'";
								$arrReducao = $db->carregar($sql);
								$arrReducao = $arrReducao ? $arrReducao : array();
								foreach ($arrReducao as $reducao) {
										
									$eobvalorempenho 	= $reducao['eprvalorreduzido'];
									$erpvalororiginal 	= $reducao['eprvalororiginal'];
										
									//$percent = number_format($eobvalorempenho*100/$vrlempenho, 2);
									$percent = 0;
										
									$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp) 
											VALUES ($preid, $empidNovo, $percent, $eobvalorempenho, $percent)";
									$db->executar($sql);
										
									//$percent1 = number_format($erpvalororiginal*100/$vrlobra, 2);
									$percent1 = 1;
									$sql = "update par.empenhoobra set
												eobvalorempenho = {$erpvalororiginal},
												eobpercentualemp = {$percent1},
												eobpercentualemp2 = {$percent1}
											where eobid = {$emp['eobid']}";
									$db->executar($sql);
								}
							}
						}
					}
				}
 				$db->commit();
			}
		} else if( $v['ems_codigo_especie'] == '02' ){
			
			$sql = "SELECT empnumeroprocesso, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empvalorempenho, empcodigoespecie, 
						empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente, 
					    ds_problema, valor_saldo_pagamento, tp_especializacao, co_diretoria, empid, usucpf, empid
					FROM par.empenho e
					WHERE empnumeroprocesso = '{$v['ems_numero_processo']}' 
						and empprotocolo = '{$v['ems_nu_seq_mov_ne']}'";
			
			$arEmpPai = $db->pegaLinha($sql);
			$arEmpPai = $arEmpPai ? $arEmpPai : array();
			
			if( !empty($arEmpPai) ){ 
				$empcentrogestaosolic 	= $arEmpPai['empcentrogestaosolic'];
				$empanoconvenio 		= $arEmpPai['empanoconvenio'];
				$empvalorempenho 		= $arEmpPai['empvalorempenho'];
				$empnumeroconvenio 		= $arEmpPai['empnumeroconvenio'];
				$empcodigoobs 			= $arEmpPai['empcodigoobs'];
				$empcodigotipo 			= $arEmpPai['empcodigotipo'];
				$empdescricao 			= $arEmpPai['empdescricao'];
				$empgestaoeminente 		= $arEmpPai['empgestaoeminente'];
				$empunidgestoraeminente = $arEmpPai['empunidgestoraeminente'];
				$empanoconvenio 		= $arEmpPai['empanoconvenio'];
				$empnumeroconvenio		= $arEmpPai['empnumeroconvenio'];
				$valor_saldo_pagamento	= $arEmpPai['valor_saldo_pagamento'];
				$empnumero				= $arEmpPai['empnumero'];
				$usucpf					= $arEmpPai['usucpf'];
				$tp_especializacao		= $arEmpPai['tp_especializacao'];
				$co_diretoria			= $arEmpPai['co_diretoria'];
				$empidPai				= $arEmpPai['empid'];
				
				$empanoconvenio = $empanoconvenio ? $empanoconvenio : 'null';
				$empnumeroconvenio = $empnumeroconvenio ? $empnumeroconvenio : 'null';
				$valor_saldo_pagamento = $valor_saldo_pagamento ? $valor_saldo_pagamento : 'null';
								
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empcodigopi, 
							empcodigoesfera, empcodigoptres, empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, 
							empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
		  					empprogramafnde, empnumerosistema, empsituacao, usucpf, empprotocolo, empnumero, 
		  					empvalorempenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento,
		  					empdata, tp_especializacao, co_diretoria, empidpai, teeid, empcarga)				
						VALUES('{$v['ems_cnpj']}', '{$v['ems_numero_do_empenho']}', '{$v['ems_ano_do_empenho']}', '{$v['ems_numero_processo']}', '{$v['ems_codigo_especie']}', '{$v['ems_pi']}', 
							'{$v['ems_esfera']}', '{$v['ems_ptres']}', '{$v['ems_fonte_recurso']}', '{$v['ems_codigo_nat_despesa']}', '$empcentrogestaosolic', $empanoconvenio, $empnumeroconvenio, 
							'$empcodigoobs', '$empcodigotipo', '$empdescricao', '$empgestaoeminente', '$empunidgestoraeminente',
		  					'{$v['ems_programa_fnde']}', '{$v['ems_numero_sistema']}', '{$ems_situacao_do_empenho}', '$usucpf', '{$v['ems_numero_sequencial_da_ne']}', '{$empnumeroFilho}', 
		  					'{$v['ems_valor_empenho']}', '$ds_problema', '{$v['ems_valor_empenho']}', $valor_saldo_pagamento,
		  					'{$v['ems_data_empenho']}', '$tp_especializacao', '$co_diretoria', '$empidPai', {$v['teeid']}, 'S') returning empid";
				
				$empidNovo = $db->pegaUm($sql);
				
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho)
	    				VALUES ('', $empidNovo, '{$v['ems_data_empenho']}', '2 - EFETIVADO', '{$v['ems_codigo_especie']}');";
				$db->executar($sql);
				
				if( $tipo == 'PAR' ){
					$totSbaid = $db->pegaUm("select count(sbaid) from par.empenhosubacao where empid = $empidPai and eobstatus = 'A'");
					if( (int)$totSbaid == (int)1 ){
						$arEmpenho = $db->pegaLinha("select eobvalorempenho, par.recuperavalorvalidadossubacaoporano(sbaid, eobano) as vrlsubacao, eobpercentualemp, eobano, sbaid,
														par.recuperavalorplanejadossubacaoporano(sbaid, eobano) as vrlsubacaopla
													from par.empenhosubacao where empid = $empidPai and eobstatus = 'A'");
							
						$vrlsubacao = ($arEmpenho['vrlsubacao'] == 0 ? $arEmpenho['vrlsubacaopla'] : $arEmpenho['vrlsubacao']);
						$eobano = $arEmpenho['eobano'];
						$sbaid = $arEmpenho['sbaid'];
						$eobpercentualemp = $arEmpenho['eobpercentualemp'];
						$eobvalorempenho = $empvalorempenho;
						$ems_valor_empenho = $v['ems_valor_empenho'];
						
						$percent1 = 1; //number_format($ems_valor_empenho*100/$vrlsubacao, 2);
							
						$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
								VALUES ($sbaid, $empidNovo, {$percent1}, {$ems_valor_empenho}, '{$eobano}')";
						$db->executar($sql);
							
						// $eobvalorempenho = (float)$eobvalorempenho - (float)$vrlsubacao;
						//$percent = number_format($eobvalorempenho*100/$vrlsubacao, 2);
							
						$sql = "UPDATE par.empenhosubacao SET
						 			eobpercentualemp = $eobpercentualemp,
									eobvalorempenho = $eobvalorempenho
								WHERE
									empid = $empidPai";
						$db->executar($sql);
					}
				}
			}
 			$db->commit();
		} else if( $v['ems_codigo_especie'] == '01' ){
			/*
			 * Insere Empenhos que não existem no simec mais exitem no sigef
			 * */
			
			$empcentrogestaosolic 	= $v['ems_centro_de_gestao'];
			$empvalorempenho 		= $v['ems_valor_empenho'];
			$empcodigoobs 			= '2';
			$empcodigotipo 			= '3';
			$empdescricao 			= '0010';
			$empgestaoeminente 		= '15253';
			$empunidgestoraeminente = $v['ems_unidade_gestora'];
			$valor_saldo_pagamento	= '0.00';
			$empnumero				= $v['ems_ano_do_empenho'].'NE'.$v['ems_numero_do_empenho'];
			$usucpf					= '';
			
			$valor_saldo_pagamento = $valor_saldo_pagamento ? $valor_saldo_pagamento : 'null';
							
			$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empcodigopi, 
						empcodigoesfera, empcodigoptres, empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, 
						empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
	  					empprogramafnde, empnumerosistema, empsituacao, usucpf, empprotocolo, empnumero, 
	  					empvalorempenho, valor_total_empenhado, valor_saldo_pagamento,
	  					empdata, empidpai, teeid, empstatus, empcarga)				
					VALUES('{$v['ems_cnpj']}', '{$v['ems_numero_do_empenho']}', '{$v['ems_ano_do_empenho']}', '{$v['ems_numero_processo']}', '{$v['ems_codigo_especie']}', '{$v['ems_pi']}', 
						'{$v['ems_esfera']}', '{$v['ems_ptres']}', '{$v['ems_fonte_recurso']}', '{$v['ems_codigo_nat_despesa']}', '$empcentrogestaosolic', 
						'$empcodigoobs', '$empcodigotipo', '$empdescricao', '$empgestaoeminente', '$empunidgestoraeminente',
	  					'{$v['ems_programa_fnde']}', '{$v['ems_numero_sistema']}', '{$ems_situacao_do_empenho}', '$usucpf', '{$v['ems_numero_sequencial_da_ne']}', '{$empnumero}', 
	  					'{$v['ems_valor_empenho']}', '{$v['ems_valor_empenho']}', $valor_saldo_pagamento,
	  					'{$v['ems_data_empenho']}', null, {$v['teeid']}, 'A', 'S') returning empid";
			
			$empidNovo = $db->pegaUm($sql);
			
			$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho)
    				VALUES ('', $empidNovo, '{$v['ems_data_empenho']}', '2 - EFETIVADO', '{$v['ems_codigo_especie']}');";
			$db->executar($sql);
			
			if( $tipo == 'PAR' ){
				$sql = "select count(sd.sbaid) from par.processopar p
							inner join par.processoparcomposicao pp on pp.prpid = p.prpid
						    inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
						where p.prpnumeroprocesso = '{$v['ems_numero_processo']}'";
				$totSub = $db->pegaUm($sql);
				
				if( (int)$totSub == (int)1 ){
					$sql = "select sd.sbaid, sd.sbdano from 
							par.processopar p
								inner join par.processoparcomposicao pp on pp.prpid = p.prpid
								inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
							where p.prpnumeroprocesso = '{$v['ems_numero_processo']}'";
					$arDetalhe = $db->pegaLinha($sql);
					
					$sbaid 				= $arDetalhe['sbaid'];
					$sbdano 			= $arDetalhe['sbdano'];						
					$ems_valor_empenho 	= $v['ems_valor_empenho'];						
					$percent1 			= 1;
					
					$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano) 
							VALUES ({$sbaid}, {$empidNovo}, {$percent1}, {$ems_valor_empenho}, '{$sbdano}')";						
					$db->executar($sql);
				}
			}
			if( $tipo == 'OBRA' ){
				$sql = "select pp.preid from par.processoobraspar p
							inner join par.processoobrasparcomposicao pp on pp.proid = p.proid
						where p.pronumeroprocesso = '{$v['ems_numero_processo']}'";
				$totObra = $db->carregarColuna($sql);
				
				if( (int)sizeof($totObra) == (int)1 ){
					
					$ems_valor_empenho 	= $v['ems_valor_empenho'];						
					$percent1 			= 0;
					
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
							VALUES ({$totObra[0]}, $empidNovo, $percent, $ems_valor_empenho, $percent)";						
					$db->executar($sql);
				}
			}
			if( $tipo == 'PAC' ){
				$sql = "select pp.preid from par.processoobra p
							inner join par.processoobraspaccomposicao pp on pp.proid = p.proid
						where p.pronumeroprocesso = '{$v['ems_numero_processo']}'";
				$totObra = $db->carregarColuna($sql);
				
				if( (int)sizeof($totObra) == (int)1 ){
					
					$preid 				= $totObra[0];
					$ems_valor_empenho 	= $v['ems_valor_empenho'];
					$percent1 			= 0;
					
					$sql = "INSERT INTO par.empenhoobra(preid, empid, eobvalorempenho, eobpercentualemp2, eobpercentualemp)
							VALUES ($preid, $empidNovo, '{$ems_valor_empenho}', '{$percent1}', '{$percent1}')";						
					$db->executar($sql);
				}
			}
			$db->commit();
		}
	} else {
		if( $v['ems_codigo_especie'] == '01' ){
			$sql = "UPDATE par.empenho SET
						empcnpj 				= '{$v['ems_cnpj']}',
						empnumerooriginal 		= '{$v['ems_numero_do_empenho']}',
						empanooriginal 			= '{$v['ems_ano_do_empenho']}',
						empcodigoespecie 		= '{$v['ems_codigo_especie']}',
						empcodigopi 			= '{$v['ems_pi']}',
						empcodigoesfera 		= '{$v['ems_esfera']}',
						empcodigoptres 			= '{$v['ems_ptres']}',
						empfonterecurso 		= '{$v['ems_fonte_recurso']}',
						empcodigonatdespesa 	= '{$v['ems_codigo_nat_despesa']}',
						empprogramafnde 		= '{$v['ems_programa_fnde']}',
						empnumerosistema 		= '{$v['ems_numero_sistema']}',
						empsituacao 			= '{$ems_situacao_do_empenho}',
						empprotocolo 			= '{$v['ems_numero_sequencial_da_ne']}',
						empnumero 				= '".$v['ems_ano_do_empenho'].'NE'.$v['ems_numero_do_empenho']."',
						empvalorempenho 		= '{$v['ems_valor_empenho']}',
						valor_total_empenhado 	= '{$v['ems_valor_empenho']}',
						empdata 				= ".($v['ems_data_empenho'] ? "'".$v['ems_data_empenho']."'" : 'null').",
						teeid 					= {$v['teeid']},
						empcarga 				= 'U',
						empstatus 				= 'A'
					WHERE
						empid = $empenho"; 
			//ver($sql);
			$db->executar($sql);			
 			$db->commit();
		}elseif( $v['ems_codigo_especie'] == '03' || $v['ems_codigo_especie'] == '13' ){
			
			$sql = "select empid from par.empenho where empprotocolo = '{$v['ems_nu_seq_mov_ne']}' and empnumeroprocesso = '{$v['ems_numero_processo']}' and empstatus = 'A'";
			$empidPai = $db->pegaUm($sql);
			
			$sql = "UPDATE par.empenho SET
						empcnpj 				= '{$v['ems_cnpj']}',
						empnumerooriginal 		= '{$v['ems_numero_do_empenho']}',
						empanooriginal 			= '{$v['ems_ano_do_empenho']}',
						empcodigoespecie 		= '{$v['ems_codigo_especie']}',
						empcodigopi 			= '{$v['ems_pi']}',
						empcodigoesfera 		= '{$v['ems_esfera']}',
						empcodigoptres 			= '{$v['ems_ptres']}',
						empfonterecurso 		= '{$v['ems_fonte_recurso']}',
						empcodigonatdespesa 	= '{$v['ems_codigo_nat_despesa']}',
						empprogramafnde 		= '{$v['ems_programa_fnde']}',
						empnumerosistema 		= '{$v['ems_numero_sistema']}',
						empsituacao 			= '{$ems_situacao_do_empenho}',
						empprotocolo 			= '{$v['ems_numero_sequencial_da_ne']}',
						empnumero 				= '".$v['ems_ano_do_empenho'].'NE'.$v['ems_numero_do_empenho']."',
						empvalorempenho 		= '{$v['ems_valor_empenho']}',
						valor_total_empenhado 	= '{$v['ems_valor_empenho']}',
						empdata 				= ".($v['ems_data_empenho'] ? "'".$v['ems_data_empenho']."'" : 'null').",
						teeid 					= {$v['teeid']},
						empidpai				= {$empidPai},
						empcarga 				= 'U',					 
						empstatus 				= 'A'					 
					WHERE
						empid = $empenho"; 
			
			if( !empty($empidPai) ){
				$db->executar($sql);
	 			$db->commit();
			}
		}
		
		if( $tipo == 'PAR' ){
			$totSbaid = $db->pegaUm("select count(sbaid) from par.empenhosubacao where empid = $empenho and eobstatus = 'A'");
		
			if( (int)$totSbaid == (int)1 ){
				$sql = "UPDATE par.empenhosubacao SET
							eobpercentualemp = 0,
							eobvalorempenho = {$v['ems_valor_empenho']}
						WHERE
							empid = $empenho and eobstatus = 'A'";
				$db->executar($sql);
			}
		} else if( $tipo == 'OBRA' ){
			$totPreid = $db->pegaUm("select count(preid) from par.empenhoobrapar where empid = $empenho ");
			
			/*Caso tenha somente uma obra vinculada ao empenho*/
			if( (int)$totPreid == (int)1 ){				
				$sql = "UPDATE par.empenhoobrapar SET
							eobpercentualemp = 0,
						  	eobvalorempenho = {$v['ems_valor_empenho']}
						WHERE
							empid = $empenho and eobstatus = 'A'";						
				$db->executar($sql);
			}
		}else if( $tipo == 'PAC' ){
			$totPreid = $db->pegaUm("select count(preid) from par.empenhoobra where empid = $empenho and eobstatus = 'A'");
						
			if( (int)$totPreid == (int)1 ){
				$sql = "UPDATE par.empenhoobra SET
							eobpercentualemp = 0,
						  	eobvalorempenho = {$v['ems_valor_empenho']}
						WHERE
							empid = $empenho and eobstatus = 'A'";
				
				$db->executar($sql);
			}
		}
		$db->commit();
	}
}

