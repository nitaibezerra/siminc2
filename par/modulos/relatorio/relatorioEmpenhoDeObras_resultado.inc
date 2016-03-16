<?php
function obras_monta_agp(){

	$agrupador = $_REQUEST["colunas"];

	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "id_obra", "fonte", "tipo_preobra", "processo", "valor_obra", "perc_empenhado", "valor_empenhado", "valor_pagamento", "nu_conta_corrente", "probanco", "proagencia", "data_inicio_termo", "data_fim_termo")
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "numero_termo":
				array_push($agp['agrupador'], array(
													"campo" => "numero_termo",
											  		"label" => "N° do Termo de Compomisso")										
									   				);
			break;
			case "id_preobra":
				array_push($agp['agrupador'], array(
													"campo" => "id_preobra",
											  		"label" => "ID da Pré-Obra")										
									   				);
			break;
		}	
	}
	
	return $agp;
	
}

function obras_monta_sql(){
	
	global $db;
	
	$where = array('1=1');
	
	// Filtros
	if ( $_REQUEST["numero_termo"] ){
		$where[] = " numero_termo ilike '%{$_REQUEST["numero_termo"]}%'";
	}
	if ( $_REQUEST["preid"] ){
		$where[] = " id_preobra in  ({$_REQUEST["preid"]})";
	}
	if ( $_REQUEST["obrid"] ){
		$where[] = " id_obra in  ({$_REQUEST["obrid"]})";
	}
	$sqlPAC = "SELECT DISTINCT
					tcp.terid||'/'||to_char(tcp.terdatainclusao,'YYYY') as numero_termo,
					pre.preid as id_preobra,
					pre.obrid as id_obra,
					--SUM(emo.eobvalorempenho) as valor_empenhado,
					--coalesce(SUM(emo.eobpercentualemp)::text,'0')||' %' as perc_empenhado,
					dfo.saldo - coalesce(vrlcancelado,0) as valor_empenhado,
					coalesce(ROUND(dfo.saldo*100/pre.prevalorobra)::text,'0')||' %' as perc_empenhado,
					SUM((SELECT SUM(pao.pobvalorpagamento) FROM par.pagamento pag INNER JOIN par.pagamentoobra pao ON pao.pagid = pag.pagid AND pao.preid = pre.preid WHERE pag.pagsituacaopagamento not ilike '%CANCELADO%' AND pag.pagstatus = 'A' AND pag.empid = emp.empid)) as valor_pagamento,
					probanco,
					proagencia,
					nu_conta_corrente,
					pre.prevalorobra as valor_obra,
					emp.empnumeroprocesso as processo,
					'n/a' as data_inicio_termo,
					'n/a' as data_fim_termo,
					'PAC' as fonte,
					pto.ptodescricao as tipo_preobra
				FROM 
					obras.preobra pre
					INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid 
				--	INNER JOIN par.empenhoobra 			emo ON emo.preid = pre.preid and eobstatus = 'A'
					INNER JOIN par.v_dadosfinanceiroporobra dfo ON dfo.preid = pre.preid
					INNER JOIN par.empenho 				emp ON emp.empnumeroprocesso = dfo.empnumeroprocesso AND emp.empstatus = 'A' and empcodigoespecie not in ('03', '13', '04') --emp.empid = emo.empid
					INNER JOIN par.processoobra 		pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
					INNER JOIN par.termocompromissopac 	tcp ON tcp.proid = pro.proid AND tcp.terstatus = 'A'
					left join (select sum(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.preid
								from par.empenhoobrapar eb
									inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
								where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
								group by e1.empidpai, eb.preid
						) as emc on emc.empidpai = emp.empid and emc.preid = pre.preid
                                WHERE pre.prevalorobra > 0
				GROUP BY
					tcp.terdatainclusao,
					tcp.terid,
					probanco,
					proagencia,
					nu_conta_corrente,
					pre.preid,
					pre.obrid,
					pre.prevalorobra,
					processo,
					pto.ptodescricao,
					dfo.saldo,
					emc.vrlcancelado";
	$sqlPAR = "SELECT DISTINCT
					dop.dopnumerodocumento||'' as numero_termo,
					pre.preid as id_preobra,
					pre.obrid as id_obra,
					--SUM(emo.eobvalorempenho) as valor_empenhado,
					(dfo.saldo - coalesce(vrlcancelado,0)) as valor_empenhado,
					--coalesce(SUM(emo.eobpercentualemp)::text,'0')||' %' as perc_empenhado,
					coalesce((dfo.saldo*100/pre.prevalorobra)::text,'0')||' %' as perc_empenhado,
					SUM((SELECT SUM(pao.popvalorpagamento) FROM par.pagamento pag INNER JOIN par.pagamentoobrapar pao ON pao.pagid = pag.pagid AND pao.preid = pre.preid WHERE pag.pagsituacaopagamento not ilike '%CANCELADO%' AND pag.pagstatus = 'A' AND pag.empid = emp.empid)) as valor_pagamento,
					probanco,
					proagencia,
					nu_conta_corrente,
					pre.prevalorobra as valor_obra,
					emp.empnumeroprocesso as processo,
					dopdatainiciovigencia as data_inicio_termo,
					dopdatafimvigencia as data_fim_termo,
					'PAR' as fonte,
					pto.ptodescricao as tipo_preobra
				FROM 
					obras.preobra pre
					INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid 
				--	INNER JOIN par.empenhoobrapar 		emo ON emo.preid = pre.preid and eobstatus = 'A'
					INNER JOIN par.v_dadosfinanceiroporobra dfo ON dfo.preid = pre.preid
					INNER JOIN par.empenho 				emp ON emp.empnumeroprocesso = dfo.empnumeroprocesso AND emp.empstatus = 'A' and empcodigoespecie not in ('03', '13', '04') --emp.empid = emo.empid 
					INNER JOIN par.processoobraspar 	pop ON pop.pronumeroprocesso = emp.empnumeroprocesso and pop.prostatus = 'A'
					INNER JOIN par.documentopar 		dop ON dop.proid = pop.proid
					INNER JOIN par.modelosdocumentos 	mdo ON mdo.mdoid = dop.mdoid AND tpdcod in (21,102)
					left join (select sum(eobvalorempenho) as vrlcancelado, e1.empidpai, eb.preid
								from par.empenhoobrapar eb
									inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
								where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
								group by e1.empidpai, eb.preid
						) as emc on emc.empidpai = emp.empid and emc.preid = pre.preid
                                WHERE pre.prevalorobra > 0
				GROUP BY
					dop.dopnumerodocumento,
					pre.preid,
					pre.obrid,
					probanco,
					proagencia,
					nu_conta_corrente,
					pre.prevalorobra,
					processo,
					dopdatainiciovigencia,
					dopdatafimvigencia,
					pto.ptodescricao,
					dfo.saldo,
					emc.vrlcancelado";
	if( $_REQUEST['fonte_preobra'] == 'PAC' ){
		$sqlPAR = '';
	}
	if( $_REQUEST['fonte_preobra'] == 'PAR' ){
		$sqlPAC = '';
	}
	// monta o sql 
	$sql = "SELECT DISTINCT
				*
			FROM
				(
				$sqlPAC
				".( $sqlPAC != '' && $sqlPAR != '' ? 'UNION ALL' : '')."
				$sqlPAR
				) as foo
			WHERE
				".implode(' AND ', $where)."
			ORDER BY
				".implode( ',' , $_REQUEST['colunas'] ) ;
// 	ver($sql,d);
	return $sql;
	
}

function obras_monta_coluna(){
	
	$coluna = array();

	array_push( $coluna, array("campo" 	  => "id_obra",
					   		   "label" 	  => "ID da Obra",
							   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "valor_obra",
					   		   "label" 	  => "Valor da Obra",
							   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "valor_empenhado",
					   		   "label" 	  => "Valor Empenhado",
							   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "perc_empenhado",
					   		   "label" 	  => "% Empenhado",
							   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "valor_pagamento",
					   		   "label" 	  => "Valor Pago",
							   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "processo",
					   		   "label" 	  => "Nº do Processo",
							   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "probanco",
					   		   "label" 	  => "N° do Banco",
							   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "proagencia",
					   		   "label" 	  => "N° da Agência",
							   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "nu_conta_corrente",
					   		   "label" 	  => "N° Conta Correte",
							   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "data_inicio_termo",
					   		   "label" 	  => "Data Início do Termo",
							   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "data_fim_termo",
					   		   "label" 	  => "Data Fim do Termo",
							   "type"	  => "string") ); 
	array_push( $coluna, array("campo" 	  => "fonte",
					   		   "label" 	  => "Fonte",
							   "type"	  => "string") ); 
	array_push( $coluna, array("campo" 	  => "tipo_preobra",
					   		   "label" 	  => "Tipo da Pré-Obra",
							   "type"	  => "string") ); 
	
	return $coluna;
	
}


/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

if( $_REQUEST['xls'] == '1' ){
	$sql = obras_monta_sql();
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	
	$db->sql_to_excel($sql, 'relRelatorioValidacoes', $cabecalho, '');
	exit;
}

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql(); 
$agrupador = obras_monta_agp();
$coluna    = obras_monta_coluna();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setEspandir(false);
$rel->setTotNivel(true);

echo $rel->getRelatorio(); 
?>