<?php
ini_set("memory_limit", "3024M");
set_time_limit(0);

include_once "config.inc";
//include_once "/var/www/simec/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/**** DECLARAÇÃO DE VARIAVEIS ****/
session_start();
$db = new cls_banco();

$dataInicio = date("d/m/Y h:i:s");

$req_processo = $_REQUEST['processo'];

if( $req_processo ){
	$filtroProc = "where nu_processo = '$req_processo'";
} else {
	$filtroProc = "";
}

$sql = "SELECT hpsid, prpid, proidpac, proidpar, empid, nu_processo, nu_parcela, an_exercicio, vl_parcela,
		  	nu_mes, nu_documento_siafi_ne, nu_seq_mov_ne, ds_username_movimento, ds_situacao_doc_siafi,
		  	dt_movimento, nu_seq_mov_pag, dt_emissao, nu_documento_siafi, numero_de_vinculacao,
		    case when prpid is not null then 'PAR'
		       when proidpar is not null then 'OBRA'
		       when proidpac is not null then 'PAC'
		    else '' end tipo
		FROM 
		  	par.historicopagamentosigef
		$filtroProc
		ORDER BY nu_processo";

$arrDadosPag = $db->carregar($sql);
$arrDadosPag = $arrDadosPag ? $arrDadosPag : array();
//ver($arrDadosPag,d);

$db->executar("update par.pagamento set pagstatus = 'I'");
$db->commit();

foreach ($arrDadosPag as $key => $v) {
	$sql = "SELECT coalesce(pagid, 0) FROM par.pagamento WHERE parnumseqob = '{$v['nu_seq_mov_pag']}' and empid = {$v['empid']}";
	$pagamento = $db->pegaUm($sql);
	
	$tipo = $v['tipo'];
	
	$nu_parcela 			= $v['nu_parcela']; 
	$an_exercicio 			= $v['an_exercicio']; 
	$vl_parcela 			= $v['vl_parcela'];
	$nu_mes 				= $v['nu_mes'];
	$nu_documento_siafi_ne 	= $v['nu_documento_siafi_ne']; 
	$nu_seq_mov_ne 			= $v['nu_seq_mov_ne'];
	$ds_username_movimento 	= $v['ds_username_movimento']; 
	$ds_situacao_doc_siafi 	= $v['ds_situacao_doc_siafi'];
	$dt_movimento 			= $v['dt_movimento'];
	$nu_seq_mov_pag 		= $v['nu_seq_mov_pag'];
	$dt_emissao 			= $v['dt_emissao'];
	$nu_documento_siafi 	= $v['nu_documento_siafi']; 
	$numero_de_vinculacao	= $v['numero_de_vinculacao'];
	$empid	 				= $v['empid'];
	$nu_processo			= $v['nu_processo'];
	
	$numeroOB 				= $an_exercicio.'OB'.$nu_documento_siafi;
	
	if( $ds_situacao_doc_siafi == 'EFETIVADO' ) $ds_situacao_doc_siafi = '2 - EFETIVADO';
	
	if( $nu_seq_mov_ne ){
		$empnumero = $db->pegaUm("select empnumero from par.empenho where empprotocolo = '$nu_seq_mov_ne'");
	}
	
	if( (int)$pagamento == (int)0 ){
		$sql = "INSERT INTO par.pagamento(pagparcela, paganoexercicio, pagvalorparcela, paganoparcela, pagmes, pagnumeroempenho, empid, usucpf, pagsituacaopagamento,
  					pagdatapagamento, parnumseqob, pagstatus, pagdatapagamentosiafi, pagnumeroob, pagcarga) 
				VALUES (
				  	".($nu_parcela ? "'".$nu_parcela."'" : 'null').",
				  	".($an_exercicio ? "'".$an_exercicio."'" : 'null').",
				  	".($vl_parcela ? "'".$vl_parcela."'" : 'null').",
				  	".($an_exercicio ? "'".$an_exercicio."'" : 'null').",
				  	".($nu_mes ? "'".$nu_mes."'" : 'null').",
				  	".($empnumero ? "'".$empnumero."'" : 'null').",
				  	".($empid ? "'".$empid."'" : 'null').",
				  	'',
				  	".($ds_situacao_doc_siafi ? "'".$ds_situacao_doc_siafi."'" : 'null').",
				  	".($dt_movimento ? "'".$dt_movimento."'" : 'null').",
				  	".($nu_seq_mov_pag ? "'".$nu_seq_mov_pag."'" : 'null').",
				  	'A',
				  	null,
				  	".($numeroOB ? "'".$numeroOB."'" : 'null').",
				 	'S' 
				) returning pagid";
		$pagid = $db->pegaUm($sql);
		
		if( $tipo == 'PAR' ){
			
			$sql = "select count(sd.sbaid) from par.processopar p
						inner join par.processoparcomposicao pp on pp.prpid = p.prpid
					    inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
					where p.prpnumeroprocesso = '{$nu_processo}'";
			$totSub = $db->pegaUm($sql);
			
			if( (int)$totSub == (int)1 ){
				$sql = "select sd.sbaid, sd.sbdano from par.processopar p
							inner join par.processoparcomposicao pp on pp.prpid = p.prpid
							inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
						where p.prpnumeroprocesso = '{$nu_processo}'";
				$arDetalhe = $db->pegaLinha($sql);
				
				$sql = "INSERT INTO par.pagamentosubacao(sbaid, pagid, pobpercentualpag, pobvalorpagamento, pobano, pobstatus) 
						VALUES ( {$arDetalhe['sbaid']}, $pagid, 0, $vl_parcela, '{$arDetalhe['sbdano']}', 'A')";
				$db->executar($sql);
			}
			
		} elseif( $tipo == 'OBRA' ){
			
			$sql = "select pp.preid from par.processoobraspar p
						inner join par.processoobrasparcomposicao pp on pp.proid = p.proid
					where p.pronumeroprocesso = '{$nu_processo}'";
			$totObra = $db->carregarColuna($sql);
			
			if( (int)sizeof($totObra) == (int)1 ){
				$sql = "INSERT INTO par.pagamentoobrapar(preid, pagid, poppercentualpag, popvalorpagamento) 
						VALUES ({$totObra[0]}, $pagid, 0, $vl_parcela)";
				$db->executar($sql);
			}
		} elseif( $tipo == 'PAC' ){
			
			$sql = "select pp.preid from par.processoobra p
						inner join par.processoobraspaccomposicao pp on pp.proid = p.proid
					where p.pronumeroprocesso = '{$nu_processo}'";
			$totObra = $db->carregarColuna($sql);
						
			if( (int)sizeof($totObra) == (int)1 ){
				$sql = "INSERT INTO par.pagamentoobra(preid, pagid, pobpercentualpag, pobvalorpagamento)  
						VALUES ({$totObra[0]}, $pagid, 0, $vl_parcela)";
				$db->executar($sql);
			}
		}
		$db->commit();
	} else {
		$sql = "UPDATE par.pagamento SET
				  	pagparcela 				= ".($nu_parcela ? "'".$nu_parcela."'" : 'null').",
				  	paganoexercicio 		= ".($an_exercicio ? "'".$an_exercicio."'" : 'null').",
				  	pagvalorparcela 		= ".($vl_parcela ? "'".$vl_parcela."'" : 'null').",
				  	paganoparcela 			= ".($an_exercicio ? "'".$an_exercicio."'" : 'null').",
				  	pagmes 					= ".($nu_mes ? "'".$nu_mes."'" : 'null').",
				  	pagsituacaopagamento 	= ".($ds_situacao_doc_siafi ? "'".$ds_situacao_doc_siafi."'" : 'null').",
				  	pagdatapagamento 		= ".($dt_movimento ? "'".$dt_movimento."'" : 'null').",
				  	parnumseqob 			= ".($nu_seq_mov_pag ? "'".$nu_seq_mov_pag."'" : 'null').",
				  	pagnumeroob 			= ".($numeroOB ? "'".$numeroOB."'" : 'null').",
				  	pagstatus 				= 'A'
				WHERE pagid = $pagamento";
		$db->executar($sql);
		
		if( $tipo == 'PAR' ){
			
			$sql = "SELECT count(sbaid) FROM par.pagamentosubacao WHERE pagid = $pagamento and pobstatus = 'A'";
			$totSub = $db->pegaUm($sql);
			/*Caso tenha somente um subação paga*/
			if( (int)$totSub == (int)1 ){				
				$sql = "UPDATE par.pagamentosubacao SET
						  	pobvalorpagamento = {$vl_parcela},
						  	pobstatus = 'A' 
						WHERE pagid = $pagamento";
				$db->executar($sql);
			}
		}else if( $tipo == 'OBRA' ){
			
			$sql = "SELECT count(preid) FROM par.pagamentoobrapar WHERE pagid = $pagamento";
			$totObra = $db->pegaUm($sql);
			
			/*Caso tenha somente um obra paga*/
			if( (int)$totObra == (int)1 ){
				$sql = "UPDATE par.pagamentoobrapar SET
						  	popvalorpagamento = {$vl_parcela}										
						WHERE
						 	pagid = $pagamento";
				$db->executar($sql);
				
			}
		}else if( $tipo == 'PAC' ){
			
			$sql = "SELECT count(preid) FROM par.pagamentoobra WHERE pagid = $pagamento";
			$totObra = $db->pegaUm($sql);
			
			/*Caso tenha somente um obra paga*/
			if( (int)$totObra == (int)1 ){
				$sql = "UPDATE par.pagamentoobra SET
						  	pobvalorpagamento = {$vl_parcela}										
						WHERE
						 	pagid = $pagamento";
				$db->executar($sql);
				
			}
		}
	}
	$db->commit();
}

$dataFim  = date("d/m/Y h:i:s");
$intervalos = intervaloEntreDatas($dataInicio, $dataFim);

ver($intervalos);
?>