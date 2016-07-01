<?php

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

$where = array();

$filtro="";
//dbg($_REQUEST,1);
// Filtros
//ver($_REQUEST);

if ($_REQUEST["preesfera"][0] ){
	$preesfera = $_REQUEST['preesfera_campo_excludente'] == '1' ? ' NOT ' : '';
	$esfera = Array();
	if(in_array('M',$_REQUEST["preesfera"])){ array_push($esfera , 'M'); }
	if(in_array('E',$_REQUEST["preesfera"])){ array_push($esfera , 'E'); }
	$filtro .= " AND peo.preesfera ".$preesfera."IN ('".implode("','",$esfera)."') ";
}

if ($_REQUEST["preanoselecao"] && $_REQUEST["preanoselecao"] != '' ){
	$filtro .= " AND peo.preanoselecao = ".$_REQUEST["preanoselecao"];
}

if ($_REQUEST["regcod"] && $_REQUEST["regcod"] != '' ){
	$filtro .= " AND est.regcod = '".$_REQUEST["regcod"]."'";
}

if ($_REQUEST["tpmid"][0]){
	$nottpmid = $_REQUEST['tpmid_campo_excludente'] == '1' ? ' NOT ' : '';
	$filtro .= " AND tmo.tpmid ".$nottpmid."IN ('".implode("','",$_REQUEST["tpmid"])."') ";
}

if ($_REQUEST["esdid_"][0]){
	$notesdid_ = $_REQUEST['esdid__campo_excludente'] == '1' ? ' NOT ' : '';
	$filtro .= " AND esd.esdid ".$notesdid_."IN (".implode(",",$_REQUEST["esdid_"]).") ";
}

if ($_REQUEST["esdid"][0]){
	$notesdid = $_REQUEST['esdid_campo_excludente'] == '1' ? ' NOT ' : '';
	$filtro .= " AND esd.esdid ".$notesdid."IN ('".implode("','",$_REQUEST["esdid"])."') ";
}

if ($_REQUEST["ptoid"][0]){
	$notptoid = $_REQUEST['ptoid_campo_excludente'] == '1' ? ' NOT ' : '';
	$filtro .= " AND peo.ptoid ".$notptoid."IN ('".implode("','",$_REQUEST["ptoid"])."') ";
}

if ($_REQUEST["estuf"][0]){
	$notestuf = $_REQUEST['estuf_campo_excludente'] == '1' ? ' NOT ' : '';
	$filtro .= " AND mun.estuf ".$notestuf."IN ('".implode("','",$_REQUEST["estuf"])."') ";
}

if ($_REQUEST["muncod"][0]){
	$notmuncod = $_REQUEST['[muncod_campo_excludente'] == '1' ? ' NOT ' : '';
	$filtro .= " AND mun.muncod ".$notmuncod."IN ('".implode("','",$_REQUEST["muncod"])."') ";
}

if($_REQUEST['relatorio']=='planilhamunicipios') {

	$sql = "SELECT
				foo.estuf,
				foo.mundescricao,
				foo.muncod,
				foo.tpmdsc,
				(SELECT
					trim(to_char(sum(qtde), '')) as qtde
				 FROM (
					 SELECT
					 	d.dshcodmunicipio,
					 	CASE WHEN d.indcumulativo='S' THEN sum(d.qtde)
					 		 WHEN d.indcumulativo='N' THEN
					 			CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
					 				ELSE 0 END
					 		 WHEN d.indcumulativo='A' THEN
					 			CASE when d.dpeanoref='2010' THEN sum(d.qtde)
					 				ELSE 0 END
				 		END as qtde
					 FROM
					 	painel.v_detalheindicadorsh d
					 WHERE
					 	d.dshcodmunicipio=foo.muncod AND d.indid='543'
					 GROUP BY
					 	d.dshcodmunicipio, d.indcumulativo, d.sehstatus, d.dpeanoref, d.indqtdevalor, d.unmid) as foo2
				 WHERE
				 	foo2.qtde!=0
				 GROUP BY
				 	foo2.dshcodmunicipio) as numero_unidades_conveniadas,

			pop.poptotalproinfancia,
			pop.poptotalquadra,
			SUM(foo.proinf_analise) as p_analise,
			SUM(foo.proinf_cadastramento) as p_cadastramento,
			SUM(foo.proinf_deferida) as p_deferida,
			SUM(foo.proinf_indeferida) as p_indeferida,
			SUM(foo.proinf_diligencia) as p_diligencia,
			SUM(foo.proinf_aprovada) as p_aprovada,
			SUM(foo.proinf_arquivada) as p_arquivada,
			SUM(foo.proinf_total) as p_total,

			SUM(foo.quadra_analise) as q_analise,
			SUM(foo.quadra_cadastramento) as q_cadastramento,
			SUM(foo.quadra_deferida) as q_deferida,
			SUM(foo.quadra_indeferida) as q_indeferida,
			SUM(foo.quadra_diligencia) as q_diligencia,
			SUM(foo.quadra_aprovada) as q_aprovada,
			SUM(foo.quadra_arquivada) as q_arquivada,
			SUM(foo.quadra_total) as q_total

			FROM (

				SELECT
				mun.estuf,
				mun.mundescricao,
				mun.muncod,
				tmo.tpmdsc,
			  	CASE WHEN esd.esdid IN(194,215,218,212,217,211,210) AND pto.ptoclassificacaoobra = 'P' THEN 1 ELSE 0 END as proinf_analise,
			  	CASE WHEN esd.esdid IN(193) 			    AND pto.ptoclassificacaoobra = 'P' THEN 1 ELSE 0 END as proinf_cadastramento,
			  	CASE WHEN esd.esdid IN(214,220) 			AND pto.ptoclassificacaoobra = 'P' THEN 1 ELSE 0 END as proinf_deferida,
				CASE WHEN esd.esdid IN(213,221) 			AND pto.ptoclassificacaoobra = 'P' THEN 1 ELSE 0 END as proinf_indeferida,
				CASE WHEN esd.esdid IN(195) 			    AND pto.ptoclassificacaoobra = 'P' THEN 1 ELSE 0 END as proinf_diligencia,
				CASE WHEN esd.esdid IN(228) 			    AND pto.ptoclassificacaoobra = 'P' THEN 1 ELSE 0 END as proinf_aprovada,
				CASE WHEN esd.esdid IN(229) 			    AND pto.ptoclassificacaoobra = 'P' THEN 1 ELSE 0 END as proinf_arquivada,
				CASE WHEN pto.ptoid!=5 						     THEN 1 ELSE 0 END as proinf_total,

			  	CASE WHEN esd.esdid IN(194,215,218,212,217,211,210) AND pto.ptoid=5 THEN 1 ELSE 0 END as quadra_analise,
			  	CASE WHEN esd.esdid IN(193) 			    AND pto.ptoclassificacaoobra = 'Q' THEN 1 ELSE 0 END as quadra_cadastramento,
			  	CASE WHEN esd.esdid IN(214,220) 			AND pto.ptoclassificacaoobra = 'Q' THEN 1 ELSE 0 END as quadra_deferida,
				CASE WHEN esd.esdid IN(213,221) 			AND pto.ptoclassificacaoobra = 'Q' THEN 1 ELSE 0 END as quadra_indeferida,
				CASE WHEN esd.esdid IN(195) 			    AND pto.ptoclassificacaoobra = 'Q' THEN 1 ELSE 0 END as quadra_diligencia,
				CASE WHEN esd.esdid IN(228) 			    AND pto.ptoclassificacaoobra = 'Q' THEN 1 ELSE 0 END as quadra_aprovada,
				CASE WHEN esd.esdid IN(229) 			    AND pto.ptoclassificacaoobra = 'Q' THEN 1 ELSE 0 END as quadra_arquivada,
			 	CASE WHEN pto.ptoid=5 						    THEN 1 ELSE 0 END as quadra_total


				FROM territorios.municipio mun
				INNER JOIN obras.preobra peo ON mun.muncod=peo.muncod
				LEFT JOIN obras.preobraanalise poa ON peo.preid=poa.preid
				LEFT JOIN obras.pretipoobra pto ON pto.ptoid=peo.ptoid
				LEFT JOIN territorios.estado est ON est.estuf=mun.estuf
				LEFT JOIN territorios.muntipomunicipio mtm ON mtm.estuf=mun.estuf AND mtm.muncod=mun.muncod
				LEFT JOIN territorios.tipomunicipio tmo ON tmo.tpmid=mtm.tpmid
				LEFT JOIN workflow.documento doc ON doc.docid=peo.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid=doc.esdid
				WHERE  peo.docid is not null AND	peo.tooid = 1 AND peo.prestatus = 'A'
						AND gtmid=7 ".(($filtro)?" ".$filtro:"")." ) foo

			LEFT JOIN obras.preobraprevista pop ON pop.muncod=SUBSTR(foo.muncod,1,6)
			GROUP BY foo.estuf, foo.mundescricao, foo.muncod, foo.tpmdsc, pop.poptotalproinfancia, pop.poptotalquadra
			ORDER BY foo.estuf, foo.mundescricao";

//	ver($sql,d);
	$cabecalho = array("UF", "Município Beneficiado", "Cod IBGE", "Grupo PAC", "Número de Unidades (Proinfância) já conveniadas", "Atendimento Previsto - Proinfância", "Atendimento Previsto - Quadras","Proifância - Em análise", "Proifância - Em cadastramento", "Proifância - Obra deferida", "Proifância - Obra indeferida", "Proifância - Em diligência", "Proifância - Obra Aprovada", "Proifância - Obra Arquivada", "Proifância - Total", "Quadra - Em análise", "Quadra - Em cadastramento", "Quadra -  Obra deferida", "Quadra - Obra indeferida", "Quadra - Em diligência", "Quadra - Obra Aprovada", "Quadra - Obra Arquivada", "Quadra - Total");



	if($_REQUEST['formato']=='xls') {
		$db->sql_to_excel($sql,"planilhamunicipio_".date("Ymdhis").".xls",$cabecalho, array("", "", "", "", "n", "n", "n","n", "n", "n", "n", "n", "n", "n", "n", "n", "n", "n", "n", "n","n","n","n"));
	} else {
		echo "<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
			  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%',$par2);
	}
	exit;

}

if($_REQUEST['relatorio']=='planilhaobras') {

	$sql = "SELECT DISTINCT
				CASE
					WHEN pto.ptoclassificacaoobra = 'Q' THEN 'Quadra'
					WHEN pto.ptoclassificacaoobra = 'P' THEN 'Proinfância' END as tipo_obra,
				peo.preid,
				peo.estuf,
				'Município' as proponente,
				mun.mundescricao,
				mun.muncod,
				tmo.tpmdsc,
				pto.ptodescricao,
				(SELECT SUM(ppovalorunitario*itcquantidade) FROM obras.preplanilhaorcamentaria INNER JOIN obras.preitenscomposicao ON preplanilhaorcamentaria.itcid=preitenscomposicao.itcid WHERE preid=peo.preid) as valor_proposto,

				CASE WHEN peo.qrpid IS NOT NULL THEN
					(SELECT itptitulo FROM questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1037 and r.qrpid=peo.qrpid) ELSE '-' END as terreno_pac,

				CASE WHEN peo.qrpid IS NOT NULL THEN
					(SELECT itptitulo FROM questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1038 AND r.qrpid=peo.qrpid) ELSE '-' END as terreno_minhacasavida,

				CASE WHEN esd.esdid IN(194,215,218,212,217,211,210) THEN 'Em análise' ELSE esd.esddsc END as analisefnde,
				COALESCE(CASE WHEN esd.esdid IN(213,221) THEN
					CASE WHEN length(TRIM(poa.poajustificativa)) > 0 THEN COALESCE(TRIM(poa.poajustificativa),'')
					ELSE

					CASE WHEN poa.qrpid IS NOT NULL THEN (select COALESCE(r1.resdsc,'') ||
																 COALESCE(r2.resdsc,'') ||
																 COALESCE(r3.resdsc,'') ||
																 COALESCE(r4.resdsc,'') ||
																 COALESCE(r5.resdsc,'') ||
																 COALESCE(r6.resdsc,'') ||
																 COALESCE(r7.resdsc,'') ||
																 COALESCE(r8.resdsc,'') ||
																 COALESCE(r9.resdsc,'') ||
																 COALESCE(r10.resdsc,'') ||
																 COALESCE(r11.resdsc,'') ||
																 COALESCE(r12.resdsc,'') ||
																 COALESCE(r13.resdsc,'') as resdsc
															from questionario.questionarioresposta g
															left join questionario.resposta r1 on r1.perid=1300 and r1.qrpid=g.qrpid
															left join questionario.resposta r2 on r2.perid=1302 and r2.qrpid=g.qrpid
															left join questionario.resposta r3 on r3.perid=1304 and r3.qrpid=g.qrpid
															left join questionario.resposta r4 on r4.perid=1306 and r4.qrpid=g.qrpid
															left join questionario.resposta r5 on r5.perid=1308 and r5.qrpid=g.qrpid
															left join questionario.resposta r6 on r6.perid=1310 and r6.qrpid=g.qrpid
															left join questionario.resposta r7 on r7.perid=1312 and r7.qrpid=g.qrpid
															left join questionario.resposta r8 on r8.perid=1314 and r8.qrpid=g.qrpid
															left join questionario.resposta r9 on r9.perid=1316 and r9.qrpid=g.qrpid
															left join questionario.resposta r10 on r10.perid=1318 and r10.qrpid=g.qrpid
															left join questionario.resposta r11 on r11.perid=1320 and r11.qrpid=g.qrpid
															left join questionario.resposta r12 on r12.perid=1324 and r12.qrpid=g.qrpid
															left join questionario.resposta r13 on r13.perid=1323 and r13.qrpid=g.qrpid
														   where g.qrpid=poa.qrpid) END

					END
				END, '-') as motivoindeferido,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
								 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
								 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1299 AND r.qrpid=poa.qrpid) ELSE '-' END as relatorio_de_vistoria_terreno,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
								 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
								 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1303 AND r.qrpid=poa.qrpid) ELSE '-' END as fotos_do_terreno,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1305 AND r.qrpid=poa.qrpid) ELSE '-' END as planilha_orcamentária,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1307 AND r.qrpid=poa.qrpid) ELSE '-' END as cronograma_fisico_financeiro,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1309 AND r.qrpid=poa.qrpid) ELSE '-' END as estudo_de_demanda,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1311 AND r.qrpid=poa.qrpid) ELSE '-' END as planta_de_localizacao_do_terreno,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1313 AND r.qrpid=poa.qrpid) ELSE '-' END as planta_de_situacao_do_terreno,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1315 AND r.qrpid=poa.qrpid) ELSE '-' END as planta_de_locacao_da_obra,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1317 AND r.qrpid=poa.qrpid) ELSE '-' END as levantamento_planialtimetrico,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
						 WHEN poa.qrpid IS NULL THEN '-'
						 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1319 AND r.qrpid=poa.qrpid) ELSE '-' END as dominialidade_do_terreno,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1321 AND r.qrpid=poa.qrpid) ELSE '-' END as declaracao_de_fornecimento_de_infraestrutura,

				CASE WHEN poa.qrpid IS NOT NULL THEN
					(SELECT CASE WHEN itptitulo='Sim.' THEN 'OK'
							 	 WHEN itptitulo='Não.' THEN 'DILIGÊNCIA'
							 	 ELSE '-'
							END from questionario.resposta r
					LEFT JOIN questionario.itempergunta ip ON r.itpid=ip.itpid
					WHERE r.perid=1322 AND r.qrpid=poa.qrpid) ELSE '-' END as declaracao_de_compatibilidade_de_fundacao,

				CASE WHEN pto.ptoclassificacaoobra = 'P' THEN '-' ELSE CASE WHEN memid IS NULL THEN 'Não' ELSE 'Sim' END END as escola_com_mais_educacao,
				CASE WHEN peo.preesfera = 'M' THEN 'Municipal' WHEN peo.preesfera = 'E' THEN 'Estadual' END as esfera,
				peo.preanoselecao

				FROM obras.preobra peo
				LEFT JOIN obras.preobraanalise 			poa ON peo.preid     = poa.preid
				LEFT JOIN obras.pretipoobra 			pto ON pto.ptoid     = peo.ptoid
				LEFT JOIN territorios.municipio 		mun ON mun.muncod    = peo.muncod
				LEFT JOIN territorios.estado 			est ON est.estuf     = mun.estuf
				LEFT JOIN territorios.muntipomunicipio 	mtm ON mtm.estuf     = peo.estuf AND mtm.muncod=mun.muncod
				LEFT JOIN territorios.tipomunicipio 	tmo ON tmo.tpmid     = mtm.tpmid
				LEFT JOIN workflow.documento 			doc ON doc.docid     = peo.docid
				LEFT JOIN workflow.estadodocumento 		esd ON esd.esdid     = doc.esdid
				LEFT JOIN pdeescola.memaiseducacao 		mme ON mme.entcodent = peo.entcodent AND memstatus='A' AND memanoreferencia='2010'
				WHERE peo.docid is not null AND peo.tooid = 1 AND
					gtmid=7
					AND peo.prestatus='A'
					AND peo.docid IS NOT NULL
					".$filtro."
				ORDER BY
					peo.estuf, mun.mundescricao";
//	ver($sql,$filtro,d);
	$cabecalho = array("Tipo de Obra","Nº Protocolo SIMEC","UF","Proponente","Município Beneficiado","Cod IBGE","Grupo PAC","Tipo de Projeto","Valor Proposto (R$)","Terreno próximo Obras do PAC","Terreno próximo Minha Casa Minha Vida","Situação da Análise do FNDE","Motivo do indeferimento","Relatorio de Vistoria Terreno","Fotos do terreno","Planilha Orçamentária","Cronograma Físico-Financeiro","Estudo de Demanda","Planta de Localização do Terreno","Planta de Situação do Terreno","Planta de Locação da Obra","Levantamento Planialtimétrico","Dominialidade do Terreno","Declaração de Fornecimento de Infraestrutura","Declaração de Compatibilidade de Fundação","Escola com Mais Educação (Quadra)","Esfera","Ano de Seleção");
	$dados = $db->carregar($sql);

	if($dados[0]) {
		foreach($dados as $d) {

			$d['valor_proposto']    = (($d['valor_proposto'])?number_format($d['valor_proposto'], 2, ',', '.'):'0');
			$d['relatorio_de_vistoria_terreno']    = (($d['relatorio_de_vistoria_terreno'])?$d['relatorio_de_vistoria_terreno']:'-');
			$d['fotos_do_terreno'] 				   = (($d['fotos_do_terreno'])?$d['fotos_do_terreno']:'-');
			$d['planilha_orcamentária'] 		   = (($d['planilha_orcamentária'])?$d['planilha_orcamentária']:'-');
			$d['cronograma_fisico_financeiro']     = (($d['cronograma_fisico_financeiro'])?$d['cronograma_fisico_financeiro']:'-');
			$d['estudo_de_demanda'] 			   = (($d['estudo_de_demanda'])?$d['estudo_de_demanda']:'-');
			$d['planta_de_localizacao_do_terreno'] = (($d['planta_de_localizacao_do_terreno'])?$d['planta_de_localizacao_do_terreno']:'-');
			$d['planta_de_situacao_do_terreno']    = (($d['planta_de_situacao_do_terreno'])?$d['planta_de_situacao_do_terreno']:'-');
			$d['planta_de_locacao_da_obra'] 	   = (($d['planta_de_locacao_da_obra'])?$d['planta_de_locacao_da_obra']:'-');
			$d['levantamento_planialtimetrico']    = (($d['levantamento_planialtimetrico'])?$d['levantamento_planialtimetrico']:'-');
			$d['dominialidade_do_terreno'] 		   = (($d['dominialidade_do_terreno'])?$d['dominialidade_do_terreno']:'-');
			$d['declaracao_de_fornecimento_de_infraestrutura'] = (($d['declaracao_de_fornecimento_de_infraestrutura'])?$d['declaracao_de_fornecimento_de_infraestrutura']:'-');
			$d['declaracao_de_compatibilidade_de_fundacao'] = (($d['declaracao_de_compatibilidade_de_fundacao'])?$d['declaracao_de_compatibilidade_de_fundacao']:'-');

			$d['motivoindeferido'] = htmlspecialchars_decode(str_replace(array(chr(10),chr(13)),"",trim(strip_tags($d['motivoindeferido']))));
			$arr[] = $d;
		}
	}

	$arr = is_array($arr) ? $arr : Array();

	if($_REQUEST['formato']!='xls') {
		echo "<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
			  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";

		$db->monta_lista_simples($arr,$cabecalho,100000,5,'N','100%',$par2);
		exit();
	} else {

		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=planilhaobra_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=planilhaobra_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($arr,$cabecalho,100000,5,'N','100%',$par2);
		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Planilha de obras - PAR", "&nbsp;" );


?>
<script>
function onOffCampo( campo ) {
	var div_on  = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input   = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function obras_exibePlanilha(tipo){

	document.getElementById('relatorio').value=tipo;

	selectAllOptions(document.getElementById('preesfera'));
	selectAllOptions(document.getElementById('tpmid'));
	selectAllOptions(document.getElementById('esdid'));
	selectAllOptions(document.getElementById('esdid_'));
	selectAllOptions(document.getElementById('ptoid'));
	selectAllOptions(document.getElementById('estuf'));
	selectAllOptions(document.getElementById('muncod'));

	if(document.getElementById('formatohtml').checked) {
		formulario.target = 'rr';
		var janela = window.open( '', 'rr', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
		document.getElementById('formulario').submit();
		janela.focus();
	} else {
		document.getElementById('formulario').submit();
	}

}

</script>
<form action="" method="post" name="formulario" id="formulario">
<input type="hidden" name="relatorio" id="relatorio" value="1"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
		<tr>
			<td class="subtitulocentro" colspan="2">
				<input type=radio name=formato value=xls checked> XLS <input type=radio name=formato value=html id=formatohtml> HTML
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Região:</td>
			<td>
				<?php
				// Regiões
				$stSql = " SELECT
								regcod as codigo,
								regdescricao as descricao
							FROM
								territorios.regiao
							ORDER BY
								descricao ";

				$db->monta_combo( 'regcod', $stSql, 'S', 'Todos', '', '','','243','N','regcod' );
				?>
			</td>
		</tr>

			<?php

				// Situação
				$stSql = "  SELECT
								tpmid as codigo,
								tpmdsc as descricao
							FROM
								territorios.tipomunicipio
							WHERE
								tpmstatus = 'A'
								AND gtmid = 7
							ORDER BY
								descricao  ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Grupo de Municípios', 'tpmid',  $stSql, '', 'Selecione o(s) Grupo(s)' );

				// Situação
				$stSql = " SELECT DISTINCT
								esdid as codigo,
								esddsc as descricao
							FROM
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37
							ORDER BY
								descricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação', 'esdid',  $stSql, '', 'Selecione a(s) Situação(ões)' );

				// Situação
				$stSql = " SELECT DISTINCT
								CASE WHEN esdid IN(194,215,218,212,217,211,210) THEN '194,215,218,212,217,211,210' ELSE esdid::varchar END as codigo,
								CASE WHEN esdid IN(194,215,218,212,217,211,210) THEN 'Em análise' ELSE esddsc END as descricao
							FROM
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37
							ORDER BY
								descricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação FNDE', 'esdid_',  $stSql, '', 'Selecione a(s) Situação(ões)' );

				// Tipo
				$tpSql = " SELECT
								ptoid as codigo,
								ptodescricao  as descricao
							FROM
								obras.pretipoobra
							WHERE
								ptostatus = 'A'
							ORDER BY
								ptoid";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipo', 'ptoid',  $tpSql, '', 'Selecione o(s) Tipo(s)' );

				// UF
				$ufSql = " SELECT
								po.estuf AS codigo,
								estdescricao AS descricao
							FROM
								territorios.estado est
							INNER JOIN
								obras.preobra po ON po.estuf = est.estuf
							WHERE
							 	po.prestatus = 'A'
							ORDER BY
								estdescricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );

				// Municípios
				$munSql = " SELECT
								tm.muncod AS codigo,
								tm.estuf || ' - ' || tm.mundescricao AS descricao
							FROM
								territorios.municipio tm
							INNER JOIN
								obras.preobra po ON po.muncod = tm.muncod
							WHERE
							 	po.prestatus = 'A'
							ORDER BY
								mundescricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Município', 'muncod',  $munSql, '', 'Selecione o(s) Município(s)' );

				// Esfera
				$munSql = " SELECT DISTINCT
								preesfera as codigo,
								CASE
									WHEN preesfera = 'E' THEN 'Estadual'
									WHEN preesfera = 'M' THEN 'Municipal'
								END as descricao
							FROM
								obras.preobra
							ORDER BY
								descricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Esfera', 'preesfera',  $munSql, '', 'Selecione a(s) Esfera(s)' );

			?>
			<tr>
			<td class="subtitulodireita">
				Ano de Seleção
			</td>
			<td>
				<?php
					$sql = "SELECT DISTINCT
								preanoselecao as codigo,
								preanoselecao as descricao
							FROM
								obras.preobra
							WHERE
								prestatus = 'A'
								AND preanoselecao > 0 ";

					$preanoselecao = $_POST['preanoselecao'];
					$db->monta_combo( "preanoselecao", $sql, 'S', 'Selecione...', '', '' );
				?>
			</td>
		</tr>
</table>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloCentro"><input type="button" name="gerar_relatorio" value="Planilha de Obras" onclick="obras_exibePlanilha('planilhaobras');"> <input type="button" name="gerar_relatorio" value="Planilha de Municípios" onclick="obras_exibePlanilha('planilhamunicipios');"></td>
</tr>
</table>
</form>