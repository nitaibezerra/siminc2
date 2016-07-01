<?php

function pegaInformacoesQuadroResumo(){

	global $db;

	$sql = "SELECT DISTINCT
				count(DISTINCT foo.inuid) as qtd_municipios,
				SUM(foo.qtd_obras_aprovadas) as qtd_obras_aprovadas,
				SUM(foo.vlr_obras_aprovadas) as vlr_obras_aprovadas,
				SUM(foo.vlr_repactuado_mi) as vlr_repactuado_mi,
				SUM(foo.vlr_empenho_pendente) as vlr_empenho_pendente
			FROM
				(
				--PAC
				SELECT DISTINCT
					inu.inuid,
					COUNT(DISTINCT pre.preid) as qtd_obras_aprovadas,
					(
					SELECT
						SUM(DISTINCT pre.prevalorobra)
					FROM
						par.processoobra pr
					INNER JOIN par.termocompromissopac 			ter ON ter.proid = pr.proid AND terstatus = 'A'
					INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pr.proid AND poc.pocstatus = 'A'
					INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
					WHERE pr.proid = pro.proid
					) as vlr_obras_aprovadas,
					SUM(pre.prevalorobra) - SUM(pbk.prevalorobra) as vlr_repactuado_mi,
					SUM(pre.prevalorobra) - SUM(veo.saldo) as vlr_empenho_pendente
				FROM
					par.instrumentounidade inu
				INNER JOIN par.processoobra 				pro ON (( pro.muncod = inu.muncod AND pro.muncod IS NOT NULL ) OR ( pro.estuf = inu.estuf AND pro.muncod IS NULL )) AND pro.prostatus = 'A'
				INNER JOIN par.termocompromissopac 			ter ON ter.proid = pro.proid AND terstatus = 'A'
				INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
				INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A' AND pre.ptoid IN (73,74)
				INNER JOIN
					(
					SELECT
						max(bkp.preid) as preid,
						preidpai
					FROM
						obras.preobra bkp
					GROUP BY bkp.preidpai
					) bkp ON bkp.preidpai = pre.preid
				INNER JOIN obras.preobra					pbk ON pbk.preid = bkp.preid
				INNER JOIN par.v_saldo_empenho_por_obra 	veo ON veo.preid = pre.preid
				INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND doc.esdid = 228
				GROUP BY
					inu.inuid,
					pro.proid
				UNION ALL
				--PAC
				SELECT DISTINCT
					inu.inuid,
					COUNT(DISTINCT pre.preid) as qtd_obras_aprovadas,
					(
					SELECT
						SUM(pre.prevalorobra)
					FROM
						par.processoobra pr
					INNER JOIN par.termocompromissopac 			ter ON ter.proid = pr.proid AND terstatus = 'A'
					INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pr.proid AND poc.pocstatus = 'A'
					INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
					WHERE pr.proid = pro.proid
					) as vlr_obras_aprovadas,
					SUM(pre.prevalorobra) - SUM(pbk.prevalorobra) as vlr_repactuado_mi,
					SUM(pre.prevalorobra) - SUM(veo.saldo) as vlr_empenho_pendente
				FROM
					par.instrumentounidade inu
				INNER JOIN par.processoobraspar   			pro ON pro.inuid = inu.inuid AND pro.prostatus = 'A'
				INNER JOIN par.vm_documentopar_ativos		dop ON dop.proid = pro.proid
				INNER JOIN par.processoobrasparcomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
				INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A' AND pre.ptoid IN (73,74)
				INNER JOIN
					(
					SELECT
						max(bkp.preid) as preid,
						preidpai
					FROM
						obras.preobra bkp
					GROUP BY bkp.preidpai
					) bkp ON bkp.preidpai = pre.preid
				INNER JOIN obras.preobra					pbk ON pbk.preid = bkp.preid
				INNER JOIN par.v_saldo_empenho_por_obra 	veo ON veo.preid = pre.preid
				INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND doc.esdid = 337
				GROUP BY
					inu.inuid,
					pro.proid
				) as foo";

	$arrayQuadro =  $db->pegaLinha( $sql );

	$sql2 = "
	 SELECT
			count(distinct pre.preid)
		FROM
			obras.preobra pre
		INNER JOIN workflow.documento doc ON pre.docid = doc.docid
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esdstatus = 'A'
		INNER JOIN obras2.obras o

		on o.preid = pre.preid AND o.obrstatus = 'A'

		WHERE
			esd.esdid in ( 1561, 1579, 1487, 1489, 1564,1568, 1553,  1566 ) -- EM REFORMULAÇÃO -- OBRA APROVADA
		AND
			pre.prestatus = 'A'";


	$totalAnalise =  $db->pegaUm( $sql2 );
	$arrayQuadro['total_analise'] = $totalAnalise;
	$sql3 = "
		  SELECT
			count(distinct pre.preid)
		FROM
			obras.preobra pre
		INNER JOIN workflow.documento doc ON pre.docid = doc.docid
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esdstatus = 'A'
		INNER JOIN obras2.obras o

		on o.preid = pre.preid AND o.obrstatus = 'A'

		WHERE
			pre.ptoid in (43, 42, 44, 45) -- MI
		AND
			esd.esdid in ( 360, 624, 337, 228) -- EM REFORMULAÇÃO, OBRA APROVADA
			--esd.esdid in ( 337, 228 ) -- OBRA APROVADA

		AND
			pre.prestatus = 'A'";


	$totalSemPedido =  $db->pegaUm( $sql3 );

	$arrayQuadro['total_sem_pedido'] = $totalSemPedido;

	return $arrayQuadro;

}

function atualizarQuadroResumo(){

	$arrDadosQuadroResumo = pegaInformacoesQuadroResumo();

?>
	<tr class="dados_resumo" >
		<td class="SubTituloDireita" width="60%">Municípios Aprovados:</td>
		<td style="text-align:center;font-weight:bold;background-color: white"><?=number_format($arrDadosQuadroResumo['qtd_municipios'], 0, ',', '.') ?></td>
	</tr>
	<tr class="dados_resumo">
		<td class="SubTituloDireita">Quantidade de Obras Repactuadas:</td>
		<td style="text-align:center;font-weight:bold;"><?=number_format($arrDadosQuadroResumo['qtd_obras_aprovadas'], 0, ',', '.') ?></td>
	</tr>
	<tr class="dados_resumo">
		<td class="SubTituloDireita">Valor Total Repactuado:</td>
		<td style="text-align:center;font-weight:bold;background-color: white">R$ <?=number_format($arrDadosQuadroResumo['vlr_obras_aprovadas'], 2, ',', '.') ?></td>
	</tr>
	<Tr class="dados_resumo">
		<td class="SubTituloDireita">Novo Aporte FNDE:</td>
		<td style="text-align:center;font-weight:bold;">R$ <?=number_format($arrDadosQuadroResumo['vlr_repactuado_mi'], 2, ',', '.') ?></td>
	</tr>
	<tr class="dados_resumo">
		<td class="SubTituloDireita">Valor a Empenhar:</td>
		<td style="text-align:center;font-weight:bold;background-color: white">R$ <?=number_format($arrDadosQuadroResumo['vlr_empenho_pendente'], 2, ',', '.') ?></td>
	</tr>
	<tr class="dados_resumo">
		<td class="SubTituloDireita">Obras que não solicitaram Reformulação:</td>
		<td style="text-align:center;font-weight:bold;"> <?=number_format($arrDadosQuadroResumo['total_sem_pedido'], 0, ',', '.') ?></td>
	</tr>
	<tr class="dados_resumo">
		<td class="SubTituloDireita">Obras em análise no FNDE:</td>
		<td style="text-align:center;font-weight:bold;background-color: white"> <?=number_format($arrDadosQuadroResumo['total_analise'], 0, ',', '.') ?></td>
	</tr>
<?php
}

function pegaNomeInstrumentoUnidade(){

	global $db;

	$sql = "SELECT
				COALESCE(mun.mundescricao||'/'||mun.estuf, inu.estuf) as nome
			FROM
				par.instrumentounidade inu
			LEFT JOIN territorios.municipio mun ON mun.muncod = inu.muncod
			WHERE
				inu.inuid = {$_REQUEST['inuid']}";

	echo $db->pegaUm( $sql );
}

function carregaMunicipios(){

	global $db;

	extract($_POST);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
			FROM territorios.municipio
			WHERE estuf = '$estuf'
			ORDER BY 2 ASC";

	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}

function listaValidacao(){

	global $db;

	$wherePAC = Array('1=1');
	$wherePAR = Array('1=1');
	if( trim($_REQUEST['numeroprocesso']) ){
		$wherePAC[] = "pro.pronumeroprocesso = '".str_replace(Array('-', '/', '.'), '', $_REQUEST['numeroprocesso'])."'";
		$wherePAR[] = "pro.pronumeroprocesso = '".str_replace(Array('-', '/', '.'), '', $_REQUEST['numeroprocesso'])."'";
	}

	if( trim($_REQUEST['dopid']) ){
		$wherePAC[] = "ter.ternumero = '{$_REQUEST['dopid']}'";
		$wherePAR[] = "dop.dopnumerodocumento = '{$_REQUEST['dopid']}'";
	}

	if( trim($_REQUEST['esfera'] )){
		$wherePAC[] = "inu.itrid = {$_REQUEST['esfera']}";
		$wherePAR[] = "inu.itrid = {$_REQUEST['esfera']}";
	}

	if( trim($_REQUEST['estuf']) ){
		$wherePAC[] = "(inu.estuf = '{$_REQUEST['estuf']}' OR inu.mun_estuf = '{$_REQUEST['estuf']}')";
		$wherePAR[] = "(inu.estuf = '{$_REQUEST['estuf']}' OR inu.mun_estuf = '{$_REQUEST['estuf']}')";
	}

	if( trim($_REQUEST['muncod']) ){
		$wherePAC[] = "inu.muncod = '{$_REQUEST['muncod']}'";
		$wherePAR[] = "inu.muncod = '{$_REQUEST['muncod']}'";
	}

	$sql = "SELECT DISTINCT
				acao,
				municipio,
				estuf,
				SUM(qtd_mi_atual_anterior) as qtd_mi_atual_anterior,
				SUM(qtd_criancas_atendidas_antes) as qtd_criancas_atendidas_antes,
				SUM(vlr_pactuado_mi) as vlr_pactuado_mi,
				SUM(vlr_empenhado_mi) as vlr_empenhado_mi,
				SUM(vlr_pagamento_mi) as vlr_pagamento_mi,
				SUM(DISTINCT saldo) as saldo,
				SUM(vlr_repactuado_mi) as vlr_repactuado_mi,
				SUM(qtd_mi_reformulado) as qtd_mi_reformulado,
				SUM(vlr_mi_repactuacao) as vlr_mi_repactuacao,
				SUM(vlr_total) as vlr_total,
				SUM(qtd_obr_repactuacao) as qtd_obr_repactuacao,
				SUM(vlr_diferenca_mi) as vlr_diferenca_mi,
				SUM(vlr_diferenca_empenho_mi) as vlr_diferenca_empenho_mi,
				SUM(qtd_total_obras) as qtd_total_obras,
				SUM(qtd_criancas_atendidas) as qtd_criancas_atendidas,
				aprovar
			FROM
			(
			--PAC
			SELECT DISTINCT
				'<img src=../imagens/mais.gif title=abrir style=cursor:pointer; class=processos inuid='||inu.inuid||' >' as acao,
				coalesce((SELECT mundescricao FROM territorios.municipio mun WHERE mun.muncod = inu.muncod), '-') as municipio,
				coalesce(inu.estuf, inu.mun_estuf) as estuf,
				--pre.preid,
				(
					(SELECT count(poc.preid)
					FROM par.processoobraspaccomposicao poc
					INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
					WHERE poc.proid = pro.proid)+
					(SELECT count(poc.preid)
					FROM par.processoobraspaccomposicao poc
					INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
					INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
					INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
					INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
					WHERE poc.proid = pro.proid)
				) as qtd_mi_atual_anterior,
				(
					SELECT
						SUM(qtd)
					FROM
					(
						(SELECT poc.preid, CASE WHEN ptoid IN (43,44) THEN 120 ELSE 60 END as qtd
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, CASE WHEN pre.ptoid IN (43,44) THEN 120 ELSE 60 END as qtd
						FROM par.processoobraspaccomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) as qtd_criancas_atendidas_antes,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_pactuado_mi,
				(
					SELECT
						SUM(saldo)
					FROM
					(
						(SELECT poc.preid, saldo
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN par.v_saldo_empenho_por_obra eob ON eob.preid = pre.preid
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, saldo
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preidpai = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN obras.preobra 				pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 			doc ON doc.docid = pr2.docid --AND esdid in (1550, 1551)
						INNER JOIN par.vm_saldo_empenho_por_obra	eob ON eob.preid = poc.preid
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_empenhado_mi,
				(
					SELECT
						SUM(valor_pagamento)
					FROM
					(
						(SELECT poc.preid, pob.pobvalorpagamento as valor_pagamento
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN par.pagamentoobra pob ON pob.preid = pre.preid
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pob.pobvalorpagamento as valor_pagamento
						FROM par.processoobraspaccomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						INNER JOIN par.pagamentoobra 	pob ON pob.preid = poc.preid
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_pagamento_mi,
				(
					SELECT
						saldo
					FROM
						(SELECT dfidatasaldo, (dfisaldoconta+dfisaldofundo+dfisaldopoupanca+dfisaldordbcdb) as saldo
						FROM painel.dadosfinanceirosconvenios dfi
						WHERE dfi.dfiprocesso = pro.pronumeroprocesso
						ORDER BY dfidatasaldo DESC LIMIT 1) as foo
				) as saldo,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 228
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_repactuado_mi,
				(
					SELECT
						count(preid)
					FROM
					(
						(SELECT poc.preid
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 228
						WHERE poc.proid = pro.proid)
					) as foo
				) as qtd_mi_reformulado,
				SUM(pre.prevalorobra) as vlr_mi_repactuacao,
				(
					SELECT
						COALESCE(SUM(prevalorobra), 0)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 228
						WHERE poc.proid = pro.proid)
					) as foo
				)+SUM(pre.prevalorobra) as vlr_total,
				count(pre.preid) as qtd_obr_repactuacao,
				(
					SELECT
						COALESCE(SUM(prevalorobra), 0) as valor
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				)-
				(
					(
						SELECT
							COALESCE(SUM(prevalorobra), 0)
						FROM
						(
							(SELECT poc.preid, pre.prevalorobra
							FROM par.processoobraspaccomposicao poc
							INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
							INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 228
							WHERE poc.proid = pro.proid)
						) as foo
					)+SUM(pre.prevalorobra)
				) as vlr_diferenca_mi,
				(
					(
					SELECT
						COALESCE(SUM(prevalorobra), 0)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 228
						WHERE poc.proid = pro.proid)
					) as foo
					)+SUM(pre.prevalorobra)
				)-
				(
					SELECT
						SUM(saldo)
					FROM
					(
						(SELECT poc.preid, saldo
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN par.v_saldo_empenho_por_obra eob ON eob.preid = pre.preid
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, saldo
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preidpai = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN obras.preobra 				pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 			doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						INNER JOIN par.vm_saldo_empenho_por_obra	eob ON eob.preid = poc.preid
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_diferenca_empenho_mi,
				(
					SELECT count(poc.preid)
					FROM par.processoobraspaccomposicao poc
					WHERE poc.proid = pro.proid AND poc.pocstatus = 'A'
				) as qtd_total_obras,
				SUM( 	CASE
						WHEN pre.ptoid = 73 		THEN 188
						WHEN pre.ptoid = 74 		THEN 94
						WHEN pre.ptoid IN (43,44) 	THEN 120
						WHEN pre.ptoid IN (42,45) 	THEN 60
					END) as qtd_criancas_atendidas,
				'<center><input type=button value=Aprovar title=Aprovar style=cursor:pointer; class=aprovar inuid='||inu.inuid||' >&nbsp;
				<input type=button value=Reprovar title=Reprovar style=cursor:pointer; class=reprovar inuid='||inu.inuid||' ></center>' as aprovar
			FROM
				par.instrumentounidade inu
			INNER JOIN par.processoobra 				pro ON (( pro.muncod = inu.muncod AND pro.muncod IS NOT NULL ) OR ( pro.estuf = inu.estuf AND pro.muncod IS NULL )) AND pro.prostatus = 'A'
			--INNER JOIN par.termocompromissopac 			ter ON ter.proid = pro.proid AND terstatus = 'A'
			INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				".implode(' AND ', $wherePAC)."
			GROUP BY
				inu.inuid,
				pro.proid
			UNION
			--PAR
			SELECT DISTINCT
				'<img src=../imagens/mais.gif title=abrir style=cursor:pointer; class=processos inuid='||inu.inuid||' >' as acao,
				coalesce((SELECT mundescricao FROM territorios.municipio mun WHERE mun.muncod = inu.muncod), '-') as municipio,
				coalesce(inu.estuf, inu.mun_estuf) as estuf,
				--pre.preid,
				(
					(SELECT count(poc.preid)
					FROM par.processoobrasparcomposicao poc
					INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
					WHERE poc.proid = pro.proid)+
					(SELECT count(poc.preid)
					FROM par.processoobrasparcomposicao poc
					INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
					INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
					INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
					INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
					WHERE poc.proid = pro.proid)
				) as qtd_mi_atual_anterior,
				(
					SELECT
						SUM(qtd)
					FROM
					(
						(SELECT poc.preid, CASE WHEN ptoid IN (43,44) THEN 120 ELSE 60 END as qtd
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, CASE WHEN pre.ptoid IN (43,44) THEN 120 ELSE 60 END as qtd
						FROM par.processoobrasparcomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) as qtd_criancas_atendidas_antes,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_pactuado_mi,
				(
					SELECT
						SUM(saldo)
					FROM
					(
						(SELECT poc.preid, saldo
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN par.v_saldo_empenho_por_obra eob ON eob.preid = pre.preid
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, saldo
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preidpai = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN obras.preobra 				pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 			doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						INNER JOIN par.vm_saldo_empenho_por_obra eob ON eob.preid = poc.preid
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_empenhado_mi,
				(
					SELECT
						SUM(valor_pagamento)
					FROM
					(
						(SELECT poc.preid, pop.popvalorpagamento as valor_pagamento
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN par.pagamentoobrapar pop ON pop.preid = poc.preid
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pop.popvalorpagamento as valor_pagamento
						FROM par.processoobrasparcomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						INNER JOIN par.pagamentoobrapar pop ON pop.preid = poc.preid
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_pagamento_mi,
				(
					SELECT
						saldo
					FROM
						(SELECT dfidatasaldo, (dfisaldoconta+dfisaldofundo+dfisaldopoupanca+dfisaldordbcdb) as saldo
						FROM painel.dadosfinanceirosconvenios dfi
						WHERE dfi.dfiprocesso = pro.pronumeroprocesso
						ORDER BY dfidatasaldo DESC LIMIT 1) as foo
				) as saldo,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 337
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_repactuado_mi,
				(
					SELECT
						count(preid)
					FROM
					(
						(SELECT poc.preid
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 337
						WHERE poc.proid = pro.proid)
					) as foo
				) as qtd_mi_reformulado,
				SUM(pre.prevalorobra) as vlr_mi_repactuacao,
				(
					SELECT
						COALESCE(SUM(prevalorobra), 0)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 337
						WHERE poc.proid = pro.proid)
					) as foo
				)+SUM(pre.prevalorobra) as vlr_total,
				count(pre.preid) as qtd_obr_repactuacao,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				)-
				(
					(
						SELECT
							COALESCE(SUM(prevalorobra), 0)
						FROM
						(
							(SELECT poc.preid, pre.prevalorobra
							FROM par.processoobrasparcomposicao poc
							INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
							INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 337
							WHERE poc.proid = pro.proid)
						) as foo
					)+SUM(pre.prevalorobra)
				) as vlr_diferenca_mi,
				(
					(
					SELECT
						COALESCE(SUM(prevalorobra), 0)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45, 73, 74)
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND esdid = 337
						WHERE poc.proid = pro.proid)
					) as foo
					)+SUM(pre.prevalorobra)
				)-
				(
					SELECT
						SUM(saldo)
					FROM
					(
						(SELECT poc.preid, saldo
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN par.v_saldo_empenho_por_obra eob ON eob.preid = pre.preid
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, saldo
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra 				pre ON pre.preidpai = poc.preid AND ptoid IN (43, 42, 44, 45)
						INNER JOIN obras.preobra 				pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 			doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						INNER JOIN par.vm_saldo_empenho_por_obra eob ON eob.preid = poc.preid
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_diferenca_empenho_mi,
				(
					SELECT count(poc.preid)
					FROM par.processoobrasparcomposicao poc
					WHERE poc.proid = pro.proid AND poc.pocstatus = 'A'
				) as qtd_total_obras,
				SUM( 	CASE
						WHEN pre.ptoid = 73 		THEN 188
						WHEN pre.ptoid = 74 		THEN 94
						WHEN pre.ptoid IN (43,44) 	THEN 120
						WHEN pre.ptoid IN (42,45) 	THEN 60
					END) as qtd_criancas_atendidas,
				'<center><input type=button value=Aprovar title=Aprovar style=cursor:pointer; class=aprovar inuid='||inu.inuid||' >&nbsp;
				<input type=button value=Reprovar title=Reprovar style=cursor:pointer; class=reprovar inuid='||inu.inuid||' ></center>' as aprovar
			FROM
				par.instrumentounidade inu
			INNER JOIN par.processoobraspar pro ON pro.inuid = inu.inuid AND pro.prostatus = 'A'
			--INNER JOIN par.vm_documentopar_ativos dop ON dop.proid = pro.proid
			INNER JOIN par.processoobrasparcomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				".implode(' AND ', $wherePAR)."
			GROUP BY
				inu.inuid,
				pro.proid
			) as foo
			GROUP BY
				acao,
				municipio,
				estuf,
				aprovar
			ORDER BY 2";
	//Removido necessidade de documento conforme demanda par 2070: Disponibilizar validação Obra PAR Emendas 129882
// 	ver(simec_htmlentities($sql), d);
	unset($_POST['req']); // Limpa request pra n dar merda... (Em homenagem ao grande McBenzi)
	$cabecalho = Array(	"&nbsp;","Município", "UF", "Total de<br> obras MI -<br> Anterior / Atual", "Crianças<br> atendidas inicial", "Valor pactuado<br> de obras MI", "Valor<br> empenhado<br> obras MI",
						"Valor pago<br> obras mi", "Saldo em conta", "Valor já<br>repactuado", "Qtd de obras já<br> reformuladas", "Valor desta<br> repactuação", "Total", "Qtd de obras<br>desta repactuação",
						"Diferença", "Diferenca de<br> empenho", "Total de<br>obras", "Crianças<br> atendidas final", "Aprovação<br> gestor");
	$db->monta_lista($sql,$cabecalho, 2000, 1,'S','95%', 'N', false,false,false,false);

}

function listaProcessoMunicipios(){

	global $db;

	$sql = "SELECT
				'<img src=../imagens/mais.gif title=abrir style=cursor:pointer; class=obras proid='||pro.proid||' >' as acao,
				pronumeroprocesso,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_pactuado_mi,
				SUM(pre.prevalorobra) as vlr_mi_repactuacao,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobraspaccomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) -
				SUM(pre.prevalorobra) as vlr_diferenca
			FROM
				par.instrumentounidade inu
			INNER JOIN par.processoobra 				pro ON (( pro.muncod = inu.muncod AND pro.muncod IS NOT NULL ) OR ( pro.estuf = inu.estuf AND pro.muncod IS NULL )) AND pro.prostatus = 'A'
			INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				inu.inuid = {$_REQUEST['inuid']}
			GROUP BY
				pro.proid
			UNION
			SELECT
				'<img src=../imagens/mais.gif title=abrir style=cursor:pointer; class=obras proid='||pro.proid||' >' as acao,
				pronumeroprocesso,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) as vlr_pactuado_mi,
				SUM(pre.prevalorobra) as vlr_mi_repactuacao,
				(
					SELECT
						SUM(prevalorobra)
					FROM
					(
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND ptoid IN (43, 42, 44, 45)
						WHERE poc.proid = pro.proid)
						UNION
						(SELECT poc.preid, pre.prevalorobra
						FROM par.processoobrasparcomposicao poc
						INNER JOIN (SELECT max(preid) as preid, preidpai FROM obras.preobra WHERE ptoid IN (43, 42, 44, 45) GROUP BY preidpai)	pm ON pm.preidpai = poc.preid
						INNER JOIN obras.preobra 		pre ON pre.preid = pm.preid
						INNER JOIN obras.preobra 		pr2 ON pr2.preid = poc.preid
						INNER JOIN workflow.documento 	doc ON doc.docid = pr2.docid AND esdid in (1550, 1551)
						WHERE poc.proid = pro.proid)
					) as foo
				) -
				SUM(pre.prevalorobra) as vlr_diferenca
			FROM
				par.processoobraspar pro
			INNER JOIN par.processoobrasparcomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				pro.prostatus = 'A'
				AND pro.inuid = {$_REQUEST['inuid']}
			GROUP BY
				pro.proid";

	unset($_POST['req']); // Limpa request pra n dar merda... (Em homenagem ao grande McBenzi)
	$cabecalho = Array(	"&nbsp;","Processo", "Valor Pactuado Antes da Reformulação", "Valor Pactuado Atual", "Diferença");
	$db->monta_lista_simples($sql,$cabecalho, 2000, 1,'N','95%', 'N', false,false,false,false);
}

function listaObrasProcesso(){

	global $db;

	$sql = "SELECT DISTINCT
				pre.preid,
				pre.predescricao,
				(SELECT ptodescricao FROM obras.pretipoobra pto
				INNER JOIN obras.preobra po ON po.ptoid = pto.ptoid
				WHERE po.preid = bkp.id ) as tipo_bkp,
				(SELECT prevalorobra
				FROM obras.preobra
				WHERE preid = bkp.id) as vlr_bkp,
				pto.ptodescricao,
				pre.prevalorobra,
				esd.esddsc
			FROM
				 par.processoobraspaccomposicao poc
			INNER JOIN obras.preobra 		pre ON pre.preid = poc.preid AND poc.pocstatus = 'A'
			INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid
			INNER JOIN (SELECT
							max(preid) as id,
							preidpai
						FROM
							obras.preobra
						GROUP BY
							preidpai) bkp ON bkp.preidpai = poc.preid
			INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
			INNER JOIN workflow.estadodocumento 	esd ON esd.esdid = doc.esdid
			WHERE
				poc.proid = {$_REQUEST['proid']}
			UNION
			SELECT DISTINCT
				pre.preid,
				pre.predescricao,
				(SELECT ptodescricao FROM obras.pretipoobra pto
				INNER JOIN obras.preobra po ON po.ptoid = pto.ptoid
				WHERE po.preid = bkp.id ) as tipo_bkp,
				(SELECT prevalorobra
				FROM obras.preobra
				WHERE preid = bkp.id) as vlr_bkp,
				pto.ptodescricao,
				pre.prevalorobra,
				esd.esddsc
			FROM
				 par.processoobrasparcomposicao poc
			INNER JOIN obras.preobra 			pre ON pre.preid = poc.preid AND poc.pocstatus = 'A'
			INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid
			INNER JOIN (SELECT
							max(preid) as id,
							preidpai
						FROM
							obras.preobra
						GROUP BY
							preidpai) bkp ON bkp.preidpai = poc.preid
			INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE
				poc.proid = {$_REQUEST['proid']}";

	unset($_POST['req']); // Limpa request pra n dar merda... (Em homenagem ao grande McBenzi)
	$cabecalho = Array(	"Preid","Nome da Obra", "Tipo Antigo da Obra", "Valor Antigo da Obra", "Tipo da Obra", "Valor da Obra", "Situação da Obra");
	$db->monta_lista_simples($sql,$cabecalho, 2000, 1,'N','95%', 'N', false,false,false,false);
}

function aprovarObrasInstrumentoUnidade(){

	global $db;

	$sql = "SELECT *
			FROM
			(
			--PAC
			SELECT DISTINCT
				pre.preid,
				pre.tooid,
				pre.docid,
				doc.esdid
			FROM
				par.instrumentounidade inu
			INNER JOIN par.processoobra 				pro ON (( pro.muncod = inu.muncod AND pro.muncod IS NOT NULL ) OR ( pro.estuf = inu.estuf AND pro.muncod IS NULL )) AND pro.prostatus = 'A'
			INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				inu.inuid = {$_REQUEST['inuid']}
			UNION
			--PAR
			SELECT DISTINCT
				pre.preid,
				pre.tooid,
				pre.docid,
				doc.esdid
			FROM
				par.instrumentounidade inu
			INNER JOIN par.processoobraspar   			pro ON pro.inuid = inu.inuid AND pro.prostatus = 'A'
			INNER JOIN par.processoobrasparcomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				inu.inuid = {$_REQUEST['inuid']}
			) as foo
			ORDER BY 2";

	$arrObras = $db->carregar( $sql );
	$arrObras = is_array($arrObras) ? $arrObras : Array();

// 	ver($arrObras, d);
	if( count($arrObras) > 0 ){

		include_once APPRAIZ . 'includes/workflow.php';

		foreach( $arrObras as $obra ){

			if( $obra['tooid'] == 1 ){
				$esdidDestino = WF_TIPO_OBRA_APROVADA;
			}else{
				$esdidDestino = WF_PAR_OBRA_APROVADA;
			}

			if( $obra['esdid'] !=  $esdidDestino && $obra['esdid'] != '' ){

				$sql = "SELECT
							aedid
						FROM workflow.acaoestadodoc
						WHERE
							esdiddestino = $esdidDestino
							AND esdidorigem = {$obra['esdid']}";

				$aedid = $db->pegaUm($sql);

				$sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}';";

				$nomeGestor = $db->pegaUm( $sql );

				if( $aedid == '' ){
					$sql = "INSERT INTO workflow.acaoestadodoc
								(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
								esdsncomentario, aedvisivel, aedcodicaonegativa, aedposacao)
							VALUES(
								{$obra['esdid']}, $esdidDestino, 'Encaminhar para obra aprovada', 'A', 'Obra aprovada', true, false, false, 'atualizaObrasReformulacaoProponentePAC( preid )' )
							RETURNING
								aedid";

					$aedid = $db->pegaUm($sql);
				}

				$teste = wf_alterarEstado( $obra['docid'], $aedid, "Obra n° {$obra['preid']} foi aprovada pelo gestor $nomeGestor, cpf {$_SESSION['usucpf']}.", array( 'docid' => $obra['docid'], 'preid' => $obra['preid'] ) );
				$db->commit();
			}
		}
		echo "sucesso";
	}else{
		echo "Obras não encontradas.";
	}
}

function reprovarObrasInstrumentoUnidade(){

	global $db;

	$sql = "SELECT *
			FROM
			(
			--PAC
			SELECT DISTINCT
				pre.preid,
				pre.tooid,
				pre.docid,
				doc.esdid
			FROM
				par.instrumentounidade inu
			INNER JOIN par.processoobra 				pro ON (( pro.muncod = inu.muncod AND pro.muncod IS NOT NULL ) OR ( pro.estuf = inu.estuf AND pro.muncod IS NULL )) AND pro.prostatus = 'A'
			INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				inu.inuid = {$_REQUEST['inuid']}
			UNION
			--PAR
			SELECT DISTINCT
				pre.preid,
				pre.tooid,
				pre.docid,
				doc.esdid
			FROM
				par.instrumentounidade inu
			INNER JOIN par.processoobraspar   			pro ON pro.inuid = inu.inuid AND pro.prostatus = 'A'
			INNER JOIN par.processoobrasparcomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
			INNER JOIN obras.preobra 					pre ON pre.preid = poc.preid AND pre.preidpai IS NULL AND pre.prestatus = 'A'
			INNER JOIN workflow.documento 				doc ON doc.docid = pre.docid AND esdid in (1550, 1551)
			WHERE
				inu.inuid = {$_REQUEST['inuid']}
			) as foo
			ORDER BY 2";

	$arrObras = $db->carregar( $sql );
	$arrObras = is_array($arrObras) ? $arrObras : Array();


	if( count($arrObras) > 0 ){

		include_once APPRAIZ . 'includes/workflow.php';

		foreach( $arrObras as $obra ){

			if( $obra['tooid'] == 1 ){
				$esdidDestino = WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO_MI_PARA_CONVENCIONAL;
			}else{
				$esdidDestino = WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR;
			}

			if( $obra['esdid'] !=  $esdidDestino ){

				$sql = "SELECT
							aedid
						FROM workflow.acaoestadodoc
						WHERE
							esdiddestino = $esdidDestino
							AND esdidorigem = {$obra['esdid']}";

				$aedid = $db->pegaUm($sql);

				$sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}';";

				$nomeGestor = $db->pegaUm( $sql );

				if( $aedid == '' ){
					$sql = "INSERT INTO workflow.acaoestadodoc
								(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
								esdsncomentario, aedvisivel, aedcodicaonegativa, aedposacao)
							VALUES(
								{$obra['esdid']}, $esdidDestino, 'Retornar para Validação de Deferimento MI para Convencional', 'A', 'Retornado para Validação de Deferimento MI para Convencional', true, false, false, 'wf_pos_refurmulaPreObra_miparaconvencional( preid )' )
							RETURNING
								aedid";

					$aedid = $db->pegaUm($sql);
				}

				$teste = wf_alterarEstado( $obra['docid'], $aedid, "Obra n° {$obra['preid']} foi reprovada pelo gestor $nomeGestor, cpf {$_SESSION['usucpf']}.", array( 'docid' => $obra['docid'], 'preid' => $obra['preid'] ) );
				$db->commit();
			}
		}
		echo "sucesso";
	}else{
		echo "Obras não encontradas.";
	}
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

$arrAbas = array(
		0 => array("descricao" => "Validação de MI para convencional", "link" 	=> "par.php?modulo=principal/validacaoReprogramacaoMIConvencional&acao=A"),
		1 => array("descricao" => "Controle da Reformulação MI para convencional", "link" 	=> "par.php?modulo=principal/controleReprogramacaoMIConvencional&acao=A")
);


echo montarAbasArray($arrAbas, "validacaoReprogramacaoMIConvencional&acao=A");

monta_titulo( $titulo_modulo, '' );
?>
<!-- Div de aguardando -->
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:300%; width:100%; opacity:0.4; filter:alpha(opacity=40); " >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<!-- Div de aguardando - FIM -->
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<!-- Bibliotecas para funcionar a Modal do Jquery -->
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<!-- Bibliotecas para funcionar a Modal do Jquery - FIM -->
<script type="text/javascript">

$(document).ready(function(){


	$('#aguardando').show();
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "req=listaValidacao&"+$('#form_filtro').serialize(),
   		async: false,
   		success: function(resp){
   			$('#lista').html(resp);
   			$('#aguardando').hide();
   		}
 	});

	/* Carrega Municpios de acordo com o estado
	*/
	$('[name="estuf"]').change(function(){
		if( $('[name="esfera"]').val() != 'E' ){
			if( $(this).val() != '' ){
				$('#aguardando').show();
				$.ajax({
			   		type: "POST",
			   		url: window.location.href,
			   		data: "req=carregaMunicipios&estuf="+$(this).val(),
			   		async: false,
			   		success: function(resp){
			   			$('#td_muncod').html(resp);
			   			$('#aguardando').hide();
			   		}
			 	});
			}else{
				$('#td_muncod').html('Escolha uma UF.');
			}
		}
	});

	/* Carrega lista de validacao */
	$('.pesquisar').click(function(){
		$('#aguardando').show();
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=listaValidacao&"+$('#form_filtro').serialize(),
	   		async: false,
	   		success: function(resp){
	   			$('#lista').html(resp);
	   			$('#aguardando').hide();
	   		}
	 	});
	});

	/* Carrega lista de processos */
	$('.processos').live('click',function(){
		$('#aguardando').show();
		var mais = $(this);
		var inuid = $(this).attr('inuid');
		if( $(this).attr('src') == '../imagens/mais.gif' ){
			$(this).attr('src', '../imagens/menos.gif');
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=listaProcessoMunicipios&inuid="+inuid,
		   		async: false,
		   		success: function(resp){
		   			$(mais).parent().parent().after('<tr id=linha_'+inuid+' ><td colspan=19 >'+resp+'</td></tr>');
		   			$('#aguardando').hide();
		   		}
		 	});
		}else{
			$(this).attr('src', '../imagens/mais.gif');
			$('#linha_'+inuid).remove();
   			$('#aguardando').hide();
		}
	});

	/* Carrega lista de processos */
	$('.obras').live('click',function(){
		$('#aguardando').show();
		var mais = $(this);
		var proid = $(this).attr('proid');
		if( $(this).attr('src') == '../imagens/mais.gif' ){
			$(this).attr('src', '../imagens/menos.gif');
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=listaObrasProcesso&proid="+proid,
		   		async: false,
		   		success: function(resp){
		   			$(mais).parent().parent().after('<tr id=linhaP_'+proid+' ><td colspan=5 >'+resp+'</td></tr>');
		   			$('#aguardando').hide();
		   		}
		 	});
		}else{
			$(this).attr('src', '../imagens/mais.gif');
			$('#linhaP_'+proid).remove();
   			$('#aguardando').hide();
		}
	});

	/* Aprova obras do municipio */
	$('.aprovar').live('click',function(){

		$('#aguardando').show();

		var inuid = $(this).attr('inuid');
		var nome_municipio = '';

		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=pegaNomeInstrumentoUnidade&inuid="+inuid,
	   		async: false,
	   		success: function(resp){
	   			nome_municipio = resp;
	   		}
	 	});

		if( confirm('Deseja aprovar as obras de '+nome_municipio+'?') ){
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=aprovarObrasInstrumentoUnidade&inuid="+inuid,
		   		async: false,
		   		success: function(resp){
		   			if( resp == 'sucesso' ){
			   			alert('Obras aprovadas com sucesso!');
			   			window.location.href = window.location.href;
			   		}else{
			   			alert(resp);
				   	}
		   			$('#aguardando').hide();
		   		}
		 	});
		}
		$('#aguardando').hide();
	});

	/* Aprova obras do municipio */
	$('.reprovar').live('click',function(){

		$('#aguardando').show();

		var inuid = $(this).attr('inuid');
		var nome_municipio = '';

		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=pegaNomeInstrumentoUnidade&inuid="+inuid,
	   		async: false,
	   		success: function(resp){
	   			nome_municipio = resp;
	   		}
	 	});

		if( confirm('Deseja reprovar as obras de '+nome_municipio+'?') ){
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=reprovarObrasInstrumentoUnidade&inuid="+inuid,
		   		async: false,
		   		success: function(resp){
		   			if( resp == 'sucesso' ){
			   			alert('Obras reprovadas com sucesso!');
			   			window.location.href = window.location.href;
			   		}else{
			   			alert(resp);
				   	}
		   			$('#aguardando').hide();
		   		}
		 	});
		}
		$('#aguardando').hide();
	});

	$('.mostrar').click(function(){

		$('#aguardando').show();
		$('.atualizar').show();
		$('.dados_resumo').remove();

		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=atualizarQuadroResumo",
	   		async: false,
	   		success: function(resp){
		   		$('.titulo').after(resp);
	   			$('#aguardando').hide();
	   		}
	 	});
	});

});
</script>
<div id="dialog" title="Solicitações do Processo" style="display:none;">
</div>
<form name="form_filtro" id="form_filtro" method="post" action="" >
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="30%" >Número de Processo:</td>
			<td colspan="3" width="40%" >
			<?php
				$filtro = simec_htmlentities( $_REQUEST['numeroprocesso'] );
				$numeroprocesso = $filtro;
				echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', '');
			?>
			</td>
			<td rowspan="5" width="30%" >
				<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<tr class="titulo" >
						<td colspan="2" style="font-size:14px;font-weight:bold;text-align:center;background-color: #dcdcdc" >
							<img src="../imagens/refresh.gif" title="abrir" style="cursor:pointer;display:none" class="mostrar atualizar" inuid="6">
							Resumo de Reformulações MI para Convencional
						</td>
					</tr>
					<tr class="dados_resumo" >
						<td colspan="2" style="font-size:14px;font-weight:bold;text-align:center;background-color: #dcdcdc" ><a class="mostrar" >Mostrar</a></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número do Termo:</td>
			<td colspan="3">
			<?php
				$dopid = $_REQUEST['dopid'];
				echo campo_texto( 'dopid', 'N', 'S', '', 20, 50, '', '');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> Esfera</td>
			<td>
				<?php
				$sql = Array( Array('codigo'=>2, 'descricao'=>'Municipal'), Array('codigo'=>1, 'descricao'=>'Estadual') );
				$db->monta_combo( "esfera", $sql, 'S', 'Todas as esferas', '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> UF</td>
			<td>
				<?php
					$estuf = $_POST['estuf'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao
							FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita" width="30%"> Município</td>
			<td id="td_muncod">
				<?php
					if( $estuf ){
						$muncod = $_POST['muncod'];
						$sql = "SELECT muncod as codigo, mundescricao as descricao
								FROM territorios.municipio
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> </td>
			<td>
				<input type="button" class="pesquisar" value="Pesquisar"/>
			</td>
		</tr>
	</table>
</form>
<div id="lista">
</div>