<?php
	ini_set("memory_limit","500M");
	set_time_limit(0);
	
	if($_REQUEST['requisicaoRelatorio']){
		ob_clean();
		$_REQUEST['requisicaoRelatorio']($_REQUEST);
	}
	
	if($_REQUEST['relatorio'] == 'obrasPorEstadoXls'){	
		include APPRAIZ. 'includes/classes/relatorio.class.inc';
		
		$sql   = monta_sql();
		$dados = $db->carregar($sql);
		$agrup = monta_agp();
		$col   = monta_coluna();
			
		$r = new montaRelatorio();
		$r->setAgrupador($agrup, $dados); 
		$r->setColuna($col);
		//$r->setBrasao(true);
		//$r->setTotNivel(true);
		//$r->setEspandir(false);
		//$r->setMonstrarTolizadorNivel(true);
		$r->setTotalizador(true);
		$r->setTolizadorLinha(true);
		
		ob_clean();
		$nomeDoArquivoXls = "relatorio_supervisao_".date('d-m-Y_H_i');
		echo $r->getRelatorioXls();
	}

	include APPRAIZ . 'includes/cabecalho.inc';
	echo "<br>";
	
	$titulo_modulo = "Relatório - Resumo de Obras Supervisionadas";
	monta_titulo( $titulo_modulo, 'Selecione o período da OS' );

	
function trataArrayObras($arrDados){
	if($arrDados){
		foreach($arrDados as $dados){
			$arrDadosTratados[$dados['estuf']][$dados['orgid']] = $dados['totalobras'];
		}
	}else{
		$arrDadosTratados[][] = null;
	}
	return $arrDadosTratados;
}

function recuperaUF(){
	global $db;
	$sql = "select estuf from territorios.estado order by estuf";
	return $db->carregarColuna($sql);
}

function carregaObrasPorEstado($dados){
	global $db;
		
	extract($dados);

	$join_orderServico = $_SESSION['join_data_ordem_serviço'];
	
	if($tipo && $orgid){
		
		switch($orgid){
			case 1:
			$educacao = "Superior";
			break;
			
			case 2:
			$educacao = "Profissional";
			break;
			
			case 3:
			$educacao = "Básica";
			break;
		}
		
		echo "<input type='hidden' name='tr_obras_ajax_carregado_$estuf' id='tr_obras_ajax_carregado_$estuf' value='tipo_{$tipo}_estuf_{$estuf}_orgid_{$orgid}' />";
		switch($tipo){
			case "repositorio":
				$sql = "select
							'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
							CASE WHEN arquivo IS NOT NULL THEN
							'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
							END as anexo,
							CASE WHEN foto IS NOT NULL THEN
									'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
							END as fotos,
							obr.obrid,
							obr.obrdesc,
							ee.entnome as entdescricao,
							'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
							CASE WHEN max(ta.tradtinclusao) IS NOT NULL
								THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
								ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
							END as termino,
							mun.mundescricao,
							CASE 
								WHEN orgid = 1 THEN 'Educação Superior'
								WHEN orgid = 2 THEN 'Educação Profissional'
								WHEN orgid = 3 THEN 'Educação Básica'
							END as educacao,
							stodesc as situacao,
							-- pog ordena data
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
								 ELSE 
								 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
							END
							|| '<FONT ' ||
							CASE WHEN obr.stoid IN (1, 2) THEN
									CASE WHEN obrdtvistoria IS NOT NULL THEN 
											CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
													'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
												 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
													'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
												 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
													'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
											|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
								 		 ELSE 
								 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
													'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
												 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
													'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
												 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
													'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
											|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
									END
								 WHEN obr.stoid IN (3) THEN
								  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
								 ELSE
								 	'COLOR=\"#000000\" TITLE=\" \">' ||
									CASE WHEN obrdtvistoria IS NOT NULL THEN 
											to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
								 		 ELSE 
								 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
									END 
							END 
							|| '</FONT>' as atualizacao,
							obr.obrpercexec as executado,
							COALESCE(esd.esddsc,'N/A') AS situacaoobra,
							obr.obrvlrrealobra
						from 
							obras.repositorio rep
						inner join
							 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
						inner join
							 entidade.endereco ende ON ende.endid = obr.endid
						inner join
							 territorios.municipio mun ON mun.muncod = ende.muncod
						INNER JOIN
							entidade.entidade ee ON ee.entid = obr.entidunidade
						LEFT JOIN
							obras.termoaditivo ta ON ta.obrid = obr.obrid
						LEFT JOIN
							obras.situacaoobra so ON so.stoid = obr.stoid
						left join
							workflow.documento wk ON wk.docid = obr.docid
						left join
							workflow.estadodocumento esd ON esd.esdid = wk.esdid
						LEFT JOIN
							( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
						LEFT JOIN
							( 
							    SELECT
							 	max(aqoid) as foto,
								max(arq.arqid) as  arqfoto,
								oar.obrid
							     FROM
								obras.arquivosobra oar
							     INNER JOIN
								public.arquivo arq ON arq.arqid = oar.arqid
							     WHERE
								aqostatus = 'A'
								and tpaid = 21
								and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
							     GROUP BY oar.obrid
										) af ON af.obrid = obr.obrid
						LEFT JOIN
							obras.itemgrupo ig ON ig.repid = rep.repid
						LEFT JOIN
							obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid
						LEFT JOIN
							workflow.documento wd ON wd.docid = gd.docid
						where 
							rep.repstatus = 'A'
						and
							wk.esdid is null
						and
							wd.esdid is null
						and
							mun.estuf = '$estuf'
						and
							orgid = $orgid
						and
							obr.obsstatus = 'A'
						GROUP BY
							ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc
						order by
							obr.orgid,obr.obrdesc";
				echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras no Repositório - $estuf - Educação $educacao</td></tr></table>";
				//$arrCabecalho = array("Ação","A","F","ID","Descrição","Município","Unidade Implantadora","Educação","Data de Início","Data de Término","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
				$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
				$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
				echo "<br />";
			break;
			case "distribuidas":
				$sql = "select
					'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
					CASE WHEN arquivo IS NOT NULL THEN
					'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
					END as anexo,
					CASE WHEN foto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,
					obr.obrid,
					obr.obrdesc,
					ee.entnome as entdescricao,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					CASE WHEN max(ta.tradtinclusao) IS NOT NULL
						THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
						ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
					END as termino,
					mun.mundescricao,
					CASE 
						WHEN orgid = 1 THEN 'Educação Superior'
						WHEN orgid = 2 THEN 'Educação Profissional'
						WHEN orgid = 3 THEN 'Educação Básica'
					END as educacao,
					stodesc as situacao,
					-- pog ordena data
					CASE WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE WHEN obr.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN obr.stoid IN (3) THEN
						  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					obr.obrpercexec as executado,
					COALESCE(esd.esddsc,'N/A') AS situacaoobra,
					obr.obrvlrrealobra
				from 
					obras.grupodistribuicao grp
				inner join
					obras.itemgrupo item ON item.gpdid = grp.gpdid
				inner join
					obras.repositorio rep ON rep.repid = item.repid
				inner join
					 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
				inner join
					 entidade.endereco ende ON ende.endid = obr.endid
				inner join
					 territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					entidade.entidade ee ON ee.entid = obr.entidunidade
				LEFT JOIN
					obras.termoaditivo ta ON ta.obrid = obr.obrid
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = obr.stoid
				left join
					workflow.documento wk ON wk.docid = obr.docid
				left join
							workflow.estadodocumento esd ON esd.esdid = wk.esdid
				LEFT JOIN
					( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
				LEFT JOIN
					( 
					    SELECT
					 	max(aqoid) as foto,
						max(arq.arqid) as  arqfoto,
						oar.obrid
					     FROM
						obras.arquivosobra oar
					     INNER JOIN
						public.arquivo arq ON arq.arqid = oar.arqid
					     WHERE
						aqostatus = 'A'
						and tpaid = 21
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
					     GROUP BY oar.obrid
								) af ON af.obrid = obr.obrid
				inner join
					workflow.documento wok ON wok.docid = grp.docid
				$join_orderServico
				where
					rep.repstatus = 'A'
				and
					obr.obsstatus = 'A'
				and
					mun.estuf = '$estuf'
				and
					orgid = $orgid
				and
					wok.esdid in (156,157,158)
				GROUP BY
					ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc
				order by
					obr.orgid,obr.obrdesc";
				echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras Distribuídas - $estuf - Educação $educacao</td></tr></table>";
				$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
				$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
				echo "<br />";
			break;
			case "supervisao":
				$sql = "select
					'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
					CASE WHEN arquivo IS NOT NULL THEN
					'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
					END as anexo,
					CASE WHEN foto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,
					obr.obrid,
					obr.obrdesc,
					ee.entnome as entdescricao,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					CASE WHEN max(ta.tradtinclusao) IS NOT NULL
						THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
						ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
					END as termino,
					mun.mundescricao,
					CASE 
						WHEN orgid = 1 THEN 'Educação Superior'
						WHEN orgid = 2 THEN 'Educação Profissional'
						WHEN orgid = 3 THEN 'Educação Básica'
					END as educacao,
					stodesc as situacao,
					-- pog ordena data
					CASE WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE WHEN obr.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN obr.stoid IN (3) THEN
						  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					obr.obrpercexec as executado,
					COALESCE(esd.esddsc,'N/A') AS situacaoobra,
					obr.obrvlrrealobra
				from 
					obras.grupodistribuicao grp
				inner join
					obras.itemgrupo item ON item.gpdid = grp.gpdid
				inner join
					obras.repositorio rep ON rep.repid = item.repid
				inner join
					 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
				inner join
					 entidade.endereco ende ON ende.endid = obr.endid
				inner join
					 territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					entidade.entidade ee ON ee.entid = obr.entidunidade
				LEFT JOIN
					obras.termoaditivo ta ON ta.obrid = obr.obrid
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = obr.stoid
				left join
					workflow.documento wk ON wk.docid = obr.docid
				left join
					workflow.estadodocumento esd ON esd.esdid = wk.esdid
				LEFT JOIN
					( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
				LEFT JOIN
					( 
					    SELECT
					 	max(aqoid) as foto,
						max(arq.arqid) as  arqfoto,
						oar.obrid
					     FROM
						obras.arquivosobra oar
					     INNER JOIN
						public.arquivo arq ON arq.arqid = oar.arqid
					     WHERE
						aqostatus = 'A'
						and tpaid = 21
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
					     GROUP BY oar.obrid
								) af ON af.obrid = obr.obrid
				inner join
					workflow.documento wok ON wok.docid = grp.docid
				$join_orderServico					
				where
					rep.repstatus = 'A'
				and
					obr.obsstatus = 'A'
				and
					mun.estuf = '$estuf'
				and
					orgid = $orgid
				and
					wok.esdid in (216,297,172)
				GROUP BY
					ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc
				order by
					obr.orgid,obr.obrdesc";
				echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras em Supervisão - $estuf - Educação $educacao</td></tr></table>";
				$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
				$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
				echo "<br />";
			break;
			case "finalizadas":
				$sql = "select
					'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
					CASE WHEN arquivo IS NOT NULL THEN
					'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
					END as anexo,
					CASE WHEN foto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,
					obr.obrid,
					obr.obrdesc,
					ee.entnome as entdescricao,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					CASE WHEN max(ta.tradtinclusao) IS NOT NULL
						THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
						ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
					END as termino,
					mun.mundescricao,
					CASE 
						WHEN orgid = 1 THEN 'Educação Superior'
						WHEN orgid = 2 THEN 'Educação Profissional'
						WHEN orgid = 3 THEN 'Educação Básica'
					END as educacao,
					stodesc as situacao,
					-- pog ordena data
					CASE WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE WHEN obr.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN obr.stoid IN (3) THEN
						  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					obr.obrpercexec as executado,
					COALESCE(esd.esddsc,'N/A') AS situacaoobra,
					obr.obrvlrrealobra
				from 
					obras.grupodistribuicao grp
				inner join
					obras.itemgrupo item ON item.gpdid = grp.gpdid
				inner join
					obras.repositorio rep ON rep.repid = item.repid
				inner join
					 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
				inner join
					 entidade.endereco ende ON ende.endid = obr.endid
				LEFT join
					 territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					entidade.entidade ee ON ee.entid = obr.entidunidade
				LEFT JOIN
					obras.termoaditivo ta ON ta.obrid = obr.obrid
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = obr.stoid
				left join
					workflow.documento wk ON wk.docid = obr.docid
				left join
					workflow.estadodocumento esd ON esd.esdid = wk.esdid
				LEFT JOIN
					( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
				LEFT JOIN
					( 
					    SELECT
					 	max(aqoid) as foto,
						max(arq.arqid) as  arqfoto,
						oar.obrid
					     FROM
						obras.arquivosobra oar
					     INNER JOIN
						public.arquivo arq ON arq.arqid = oar.arqid
					     WHERE
						aqostatus = 'A'
						and tpaid = 21
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
					     GROUP BY oar.obrid
								) af ON af.obrid = obr.obrid
				inner join
					workflow.documento wok ON wok.docid = grp.docid
				$join_orderServico
				where
					obr.obsstatus = 'A'
				and
					mun.estuf = '$estuf'
				and
					orgid = $orgid
				and
					obr.obrsupemp is true
				GROUP BY
					ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc,rep.repsitsupervisao
				order by
					obr.orgid,obr.obrdesc";
				
				echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras com Supervisão Finalizada - $estuf - Educação $educacao</td></tr></table>";
				$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
				$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
				echo "<br />";
			break;
		}
	}else{
		echo "<input type='hidden' name='tr_obras_ajax_carregado_$estuf' id='tr_obras_ajax_carregado_$estuf' value='geral' />";	
		$sql = "select
							'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
							CASE WHEN arquivo IS NOT NULL THEN
							'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
							END as anexo,
							CASE WHEN foto IS NOT NULL THEN
									'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
							END as fotos,
							obr.obrid,
							obr.obrdesc,
							ee.entnome as entdescricao,
							'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
							CASE WHEN max(ta.tradtinclusao) IS NOT NULL
								THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
								ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
							END as termino,
							mun.mundescricao,
							CASE 
								WHEN orgid = 1 THEN 'Educação Superior'
								WHEN orgid = 2 THEN 'Educação Profissional'
								WHEN orgid = 3 THEN 'Educação Básica'
							END as educacao,
							stodesc as situacao,
							-- pog ordena data
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
								 ELSE 
								 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
							END
							|| '<FONT ' ||
							CASE WHEN obr.stoid IN (1, 2) THEN
									CASE WHEN obrdtvistoria IS NOT NULL THEN 
											CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
													'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
												 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
													'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
												 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
													'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
											|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
								 		 ELSE 
								 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
													'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
												 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
													'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
												 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
													'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
											|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
									END
								 WHEN obr.stoid IN (3) THEN
								  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
								 ELSE
								 	'COLOR=\"#000000\" TITLE=\" \">' ||
									CASE WHEN obrdtvistoria IS NOT NULL THEN 
											to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
								 		 ELSE 
								 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
									END 
							END 
							|| '</FONT>' as atualizacao,
							obr.obrpercexec as executado,
							COALESCE(esd.esddsc,'N/A') AS situacaoobra,
							obr.obrvlrrealobra
						from 
							obras.repositorio rep
						inner join
							 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
						inner join
							 entidade.endereco ende ON ende.endid = obr.endid
						inner join
							 territorios.municipio mun ON mun.muncod = ende.muncod
						INNER JOIN
							entidade.entidade ee ON ee.entid = obr.entidunidade
						LEFT JOIN
							obras.termoaditivo ta ON ta.obrid = obr.obrid
						LEFT JOIN
							obras.situacaoobra so ON so.stoid = obr.stoid
						left join
							workflow.documento wk ON wk.docid = obr.docid
						left join
							workflow.estadodocumento esd ON esd.esdid = wk.esdid
						LEFT JOIN
							( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
						LEFT JOIN
							( 
							    SELECT
							 	max(aqoid) as foto,
								max(arq.arqid) as  arqfoto,
								oar.obrid
							     FROM
								obras.arquivosobra oar
							     INNER JOIN
								public.arquivo arq ON arq.arqid = oar.arqid
							     WHERE
								aqostatus = 'A'
								and tpaid = 21
								and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
							     GROUP BY oar.obrid
										) af ON af.obrid = obr.obrid
						where 
							rep.repstatus = 'A'
						and
							wk.esdid is null
						and
							mun.estuf = '$estuf'
						and
							obr.obsstatus = 'A'
						GROUP BY
							ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc
						order by
							obr.orgid,obr.obrdesc";
		echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras no Repositório - $estuf</td></tr></table>";
		$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
		$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
		echo "<br />";
		$sql = "select
					'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
					CASE WHEN arquivo IS NOT NULL THEN
					'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
					END as anexo,
					CASE WHEN foto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,
					obr.obrid,
					obr.obrdesc,
					ee.entnome as entdescricao,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					CASE WHEN max(ta.tradtinclusao) IS NOT NULL
						THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
						ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
					END as termino,
					mun.mundescricao,
					CASE 
						WHEN orgid = 1 THEN 'Educação Superior'
						WHEN orgid = 2 THEN 'Educação Profissional'
						WHEN orgid = 3 THEN 'Educação Básica'
					END as educacao,
					stodesc as situacao,
					-- pog ordena data
					CASE WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE WHEN obr.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN obr.stoid IN (3) THEN
						  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					obr.obrpercexec as executado,
					COALESCE(esd.esddsc,'N/A') AS situacaoobra,
					obr.obrvlrrealobra
				from 
					obras.grupodistribuicao grp
				inner join
					obras.itemgrupo item ON item.gpdid = grp.gpdid
				inner join
					obras.repositorio rep ON rep.repid = item.repid
				inner join
					 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
				inner join
					 entidade.endereco ende ON ende.endid = obr.endid
				inner join
					 territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					entidade.entidade ee ON ee.entid = obr.entidunidade
				LEFT JOIN
					obras.termoaditivo ta ON ta.obrid = obr.obrid
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = obr.stoid
				left join
					workflow.documento wk ON wk.docid = obr.docid
				left join
					workflow.estadodocumento esd ON esd.esdid = wk.esdid
				LEFT JOIN
					( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
				LEFT JOIN
					( 
					    SELECT
					 	max(aqoid) as foto,
						max(arq.arqid) as  arqfoto,
						oar.obrid
					     FROM
						obras.arquivosobra oar
					     INNER JOIN
						public.arquivo arq ON arq.arqid = oar.arqid
					     WHERE
						aqostatus = 'A'
						and tpaid = 21
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
					     GROUP BY oar.obrid
								) af ON af.obrid = obr.obrid
				inner join
					workflow.documento wok ON wok.docid = grp.docid
				$join_orderServico		
				where
					rep.repstatus = 'A'
				and
					obr.obsstatus = 'A'
				and
					mun.estuf = '$estuf'
				and
					wok.esdid in (156,157,158)
				GROUP BY
					ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc
				order by
					obr.orgid,obr.obrdesc";
		echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras Distribuídas - $estuf</td></tr></table>";
		$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
		$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
		echo "<br />";
		$sql = "select
					'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
					CASE WHEN arquivo IS NOT NULL THEN
					'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
					END as anexo,
					CASE WHEN foto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,
					obr.obrid,
					obr.obrdesc,
					ee.entnome as entdescricao,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					CASE WHEN max(ta.tradtinclusao) IS NOT NULL
						THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
						ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
					END as termino,
					mun.mundescricao,
					CASE 
						WHEN orgid = 1 THEN 'Educação Superior'
						WHEN orgid = 2 THEN 'Educação Profissional'
						WHEN orgid = 3 THEN 'Educação Básica'
					END as educacao,
					stodesc as situacao,
					-- pog ordena data
					CASE WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE WHEN obr.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN obr.stoid IN (3) THEN
						  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					obr.obrpercexec as executado,
					COALESCE(esd.esddsc,'N/A') AS situacaoobra,
					obr.obrvlrrealobra
				from 
					obras.grupodistribuicao grp
				inner join
					obras.itemgrupo item ON item.gpdid = grp.gpdid
				inner join
					obras.repositorio rep ON rep.repid = item.repid
				inner join
					 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
				inner join
					 entidade.endereco ende ON ende.endid = obr.endid
				inner join
					 territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					entidade.entidade ee ON ee.entid = obr.entidunidade
				LEFT JOIN
					obras.termoaditivo ta ON ta.obrid = obr.obrid
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = obr.stoid
				left join
					workflow.documento wk ON wk.docid = obr.docid
				left join
					workflow.estadodocumento esd ON esd.esdid = wk.esdid
				LEFT JOIN
					( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
				LEFT JOIN
					( 
					    SELECT
					 	max(aqoid) as foto,
						max(arq.arqid) as  arqfoto,
						oar.obrid
					     FROM
						obras.arquivosobra oar
					     INNER JOIN
						public.arquivo arq ON arq.arqid = oar.arqid
					     WHERE
						aqostatus = 'A'
						and tpaid = 21
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
					     GROUP BY oar.obrid
								) af ON af.obrid = obr.obrid
				inner join
					workflow.documento wok ON wok.docid = grp.docid
				$join_orderServico				
				where
					rep.repstatus = 'A'
				and
					obr.obsstatus = 'A'
				and
					mun.estuf = '$estuf'
				and
					wok.esdid in (216,297,172)
				GROUP BY
					ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc
				order by
					obr.orgid,obr.obrdesc";
		echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras em Supervisão - $estuf</td></tr></table>";
		$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
		$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
		echo "<br />";
		$sql = "select
					'<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"visualizarObras(\'' || obr.obrid || '\')\" />' as acao,
					CASE WHEN arquivo IS NOT NULL THEN
					'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
					END as anexo,
					CASE WHEN foto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || obr.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,
					obr.obrid,
					obr.obrdesc,
					ee.entnome as entdescricao,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					CASE WHEN max(ta.tradtinclusao) IS NOT NULL
						THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
						ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
					END as termino,
					mun.mundescricao,
					CASE 
						WHEN orgid = 1 THEN 'Educação Superior'
						WHEN orgid = 2 THEN 'Educação Profissional'
						WHEN orgid = 3 THEN 'Educação Básica'
					END as educacao,
					stodesc as situacao,
					-- pog ordena data
					CASE WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE WHEN obr.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN obr.stoid IN (3) THEN
						  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					obr.obrpercexec as executado,
					COALESCE(esd.esddsc,'N/A') AS situacaoobra,
					obr.obrvlrrealobra
				from 
					obras.grupodistribuicao grp
				inner join
					obras.itemgrupo item ON item.gpdid = grp.gpdid
				inner join
					obras.repositorio rep ON rep.repid = item.repid
				inner join
					 obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
				inner join
					 entidade.endereco ende ON ende.endid = obr.endid
				inner join
					 territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					entidade.entidade ee ON ee.entid = obr.entidunidade
				LEFT JOIN
					obras.termoaditivo ta ON ta.obrid = obr.obrid
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = obr.stoid
				left join
					workflow.documento wk ON wk.docid = obr.docid
				left join
					workflow.estadodocumento esd ON esd.esdid = wk.esdid
				LEFT JOIN
					( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = obr.obrid
				LEFT JOIN
					( 
					    SELECT
					 	max(aqoid) as foto,
						max(arq.arqid) as  arqfoto,
						oar.obrid
					     FROM
						obras.arquivosobra oar
					     INNER JOIN
						public.arquivo arq ON arq.arqid = oar.arqid
					     WHERE
						aqostatus = 'A'
						and tpaid = 21
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
					     GROUP BY oar.obrid
								) af ON af.obrid = obr.obrid
				inner join
					workflow.documento wok ON wok.docid = grp.docid
				$join_orderServico
				where
					obr.obsstatus = 'A'
				and
					mun.estuf = '$estuf'
				and
					wk.docid is not null
				and				
					obr.obrsupemp is true
				GROUP BY
					ao.arquivo,af.foto,af.arqfoto,obr.obrid,obr.obrdesc,ee.entnome,obr.obrdtinicio,obr.obrdttermino,mun.mundescricao,obr.orgid,obr.stoid,obsdtinclusao,obrdtvistoria,so.stodesc,obr.obrpercexec,obr.obrvlrrealobra,esd.esdid,esddsc
				order by
					obr.orgid,obr.obrdesc";
		echo "<table width='100%' bgcolor='#dcdcdc' ><tr><td class='TituloTela' align='center' >Obras com Supervisão Finalizada - $estuf</td></tr></table>";
		$arrCabecalho = array("Ação","A","F","ID","Descrição","Unidade Implantadora","Data de Início","Data de Término","Município","Educação","Situação de Execução da Obra","Última Atualização","% Executado","Situação da Obra","Valor da Obra");
		$db->monta_lista_simples($sql,$arrCabecalho,100000,10,"N","100%","N",true,'','',true);
		echo "<br/>";			
	}


//	require_once(APPRAIZ."includes/classes/MontaListaAjax.class.inc");
//	$ajax = new MontaListaAjax($db);
//	$arrCabecalho = array("Ação","ID","Descrição","Município");
//	$ajax->montaLista($sql,$arrCabecalho,50,5,"N","center",100);
	
	exit;
}

function monta_sql(){
	global $filtroSession; 
	
	$sql = "
		Select	1 as ord, 
			'Obras no Repositório' as titulo,
			mun.estuf as estado,
			obr.obrid,
			obr.obrdesc,
			ee.entnome as entdescricao,
			to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
			Case When max(ta.tradtinclusao) is not null Then to_char(max(ta.traterminoexec), 'DD/MM/YYYY') Else to_char(obrdttermino, 'DD/MM/YYYY') End as termino,
			mun.mundescricao,
			Case 
			  When orgid = 1 Then 'Educação Superior'
			  When orgid = 2 Then 'Educação Profissional'
			  When orgid = 3 Then 'Educação Básica'
			End as educacao,
			stodesc as situacao,
			Case 
			  When obrdtvistoria is not null Then obrdtvistoria Else obsdtinclusao 
			End ||
			Case 
			  When obr.stoid in (1, 2) 
			    Then 
			      Case 
			        When obrdtvistoria is not null 
			          Then 
				    Case 
				      When DATE_PART('days', NOW() - obrdtvistoria) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 45 and DATE_PART('days', NOW() - obrdtvistoria) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
				  Else
				    Case 
				      When DATE_PART('days', NOW() - obsdtinclusao) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 45 and DATE_PART('days', NOW() - obsdtinclusao) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			   When obr.stoid IN (3) 
			     Then 'Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')) || DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
			     Else '' || 
			       Case 
			         When obrdtvistoria is not null 
			           Then to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
			           Else to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			End as atualizacao,
			obr.obrpercexec as executado,
			COALESCE(esd.esddsc,'N/A') AS situacaoobra,
			obr.obrvlrrealobra
		From obras.repositorio rep
		
		Join obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
		Join entidade.endereco ende ON ende.endid = obr.endid
		Join territorios.municipio mun ON mun.muncod = ende.muncod
		Join entidade.entidade ee ON ee.entid = obr.entidunidade
		
		Left Join obras.termoaditivo ta ON ta.obrid = obr.obrid
		Left Join obras.situacaoobra so ON so.stoid = obr.stoid
		Left Join workflow.documento wk ON wk.docid = obr.docid
		Left Join workflow.estadodocumento esd ON esd.esdid = wk.esdid
		
		Where rep.repstatus = 'A' and wk.esdid is null and obr.obsstatus = 'A'
		
		Group by titulo, obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, obr.orgid, obr.obrpercexec, obr.obrvlrrealobra, obr.stoid,
			 obsdtinclusao, obrdtvistoria, esddsc,
			 esd.esdid, 
			 mun.estuf , mun.mundescricao, 
			 so.stodesc, 
			 ee.entnome
		
		Union
		
		Select	2 as ord,
			'Obras Distribuídas' as titulo, 
			mun.estuf as estado,
			obr.obrid,
			obr.obrdesc,
			ee.entnome as entdescricao,
			to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
			Case When max(ta.tradtinclusao) is not null Then to_char(max(ta.traterminoexec), 'DD/MM/YYYY') Else to_char(obrdttermino, 'DD/MM/YYYY') End as termino,
			mun.mundescricao,
			Case 
			  When orgid = 1 Then 'Educação Superior'
			  When orgid = 2 Then 'Educação Profissional'
			  When orgid = 3 Then 'Educação Básica'
			End as educacao,
			stodesc as situacao,
			Case 
			  When obrdtvistoria is not null Then obrdtvistoria Else obsdtinclusao 
			End ||
			Case 
			  When obr.stoid in (1, 2) 
			    Then 
			      Case 
			        When obrdtvistoria is not null 
			          Then 
				    Case 
				      When DATE_PART('days', NOW() - obrdtvistoria) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 45 and DATE_PART('days', NOW() - obrdtvistoria) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
				  Else
				    Case 
				      When DATE_PART('days', NOW() - obsdtinclusao) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 45 and DATE_PART('days', NOW() - obsdtinclusao) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			   When obr.stoid IN (3) 
			     Then 'Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')) || DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
			     Else '' || 
			       Case 
			         When obrdtvistoria is not null 
			           Then to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
			           Else to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			End as atualizacao,
			obr.obrpercexec as executado,
			COALESCE(esd.esddsc,'N/A') AS situacaoobra,
			obr.obrvlrrealobra
		From obras.grupodistribuicao grp
		
		Join obras.itemgrupo item ON item.gpdid = grp.gpdid
		Join obras.repositorio rep ON rep.repid = item.repid
		Join obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
		Join entidade.endereco ende ON ende.endid = obr.endid
		Join territorios.municipio mun ON mun.muncod = ende.muncod
		Join entidade.entidade ee ON ee.entid = obr.entidunidade
		
		Left Join obras.termoaditivo ta ON ta.obrid = obr.obrid
		Left Join obras.situacaoobra so ON so.stoid = obr.stoid
		Left Join workflow.documento wk ON wk.docid = obr.docid
		Left Join workflow.estadodocumento esd ON esd.esdid = wk.esdid
		Join workflow.documento wok ON wok.docid = grp.docid
		
		$join_orderServico
		
		Where rep.repstatus = 'A' and obr.obsstatus = 'A' and wok.esdid in (156,157,158)
		
		Group by titulo, obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, obr.orgid, obr.obrpercexec, obr.obrvlrrealobra, obr.stoid,
			 obsdtinclusao, obrdtvistoria, esddsc,
			 esd.esdid, 
			 mun.estuf , mun.mundescricao, 
			 so.stodesc, 
			 ee.entnome
		
		Union
		
		Select	3 as ord,
			'Obras em Supervisão' as titulo, 
			mun.estuf as estado,
			obr.obrid,
			obr.obrdesc,
			ee.entnome as entdescricao,
			to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
			Case When max(ta.tradtinclusao) is not null Then to_char(max(ta.traterminoexec), 'DD/MM/YYYY') Else to_char(obrdttermino, 'DD/MM/YYYY') End as termino,
			mun.mundescricao,
			Case 
			  When orgid = 1 Then 'Educação Superior'
			  When orgid = 2 Then 'Educação Profissional'
			  When orgid = 3 Then 'Educação Básica'
			End as educacao,
			stodesc as situacao,
			Case 
			  When obrdtvistoria is not null Then obrdtvistoria Else obsdtinclusao 
			End ||
			Case 
			  When obr.stoid in (1, 2) 
			    Then 
			      Case 
			        When obrdtvistoria is not null 
			          Then 
				    Case 
				      When DATE_PART('days', NOW() - obrdtvistoria) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 45 and DATE_PART('days', NOW() - obrdtvistoria) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
				  Else
				    Case 
				      When DATE_PART('days', NOW() - obsdtinclusao) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 45 and DATE_PART('days', NOW() - obsdtinclusao) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			   When obr.stoid IN (3) 
			     Then 'Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')) || DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
			     Else '' || 
			       Case 
			         When obrdtvistoria is not null 
			           Then to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
			           Else to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			End as atualizacao,
			obr.obrpercexec as executado,
			COALESCE(esd.esddsc,'N/A') AS situacaoobra,
			obr.obrvlrrealobra
		From obras.grupodistribuicao grp
		
		Join obras.itemgrupo item ON item.gpdid = grp.gpdid
		Join obras.repositorio rep ON rep.repid = item.repid
		Join obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
		Join entidade.endereco ende ON ende.endid = obr.endid
		Join territorios.municipio mun ON mun.muncod = ende.muncod
		Join entidade.entidade ee ON ee.entid = obr.entidunidade
		
		Left Join obras.termoaditivo ta ON ta.obrid = obr.obrid
		Left Join obras.situacaoobra so ON so.stoid = obr.stoid
		Left Join workflow.documento wk ON wk.docid = obr.docid
		Left Join workflow.estadodocumento esd ON esd.esdid = wk.esdid
		Join workflow.documento wok ON wok.docid = grp.docid
		
		Where rep.repstatus = 'A' and obr.obsstatus = 'A' and wok.esdid in (216,297,172)
		
		$join_orderServico
		
		Group by titulo, obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, obr.orgid, obr.obrpercexec, obr.obrvlrrealobra, obr.stoid,
			 obsdtinclusao, obrdtvistoria, esddsc,
			 esd.esdid, 
			 mun.estuf , mun.mundescricao, 
			 so.stodesc, 
			 ee.entnome
		
		Union
		
		Select	4 as ord,
			'Obras com Supervisão Finalizada' as titulo, 
			mun.estuf as estado,
			obr.obrid,
			obr.obrdesc,
			ee.entnome as entdescricao,
			to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
			Case When max(ta.tradtinclusao) is not null Then to_char(max(ta.traterminoexec), 'DD/MM/YYYY') Else to_char(obrdttermino, 'DD/MM/YYYY') End as termino,
			mun.mundescricao,
			Case 
			  When orgid = 1 Then 'Educação Superior'
			  When orgid = 2 Then 'Educação Profissional'
			  When orgid = 3 Then 'Educação Básica'
			End as educacao,
			stodesc as situacao,
			Case 
			  When obrdtvistoria is not null Then obrdtvistoria Else obsdtinclusao 
			End ||
			Case 
			  When obr.stoid in (1, 2) 
			    Then 
			      Case 
			        When obrdtvistoria is not null 
			          Then 
				    Case 
				      When DATE_PART('days', NOW() - obrdtvistoria) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 45 and DATE_PART('days', NOW() - obrdtvistoria) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obrdtvistoria) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
				  Else
				    Case 
				      When DATE_PART('days', NOW() - obsdtinclusao) <= 45 Then 'Esta obra foi atualizada em até 45 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 45 and DATE_PART('days', NOW() - obsdtinclusao) <= 60 Then 'Esta obra foi atualizada entre 45 e 60 dias' 
				      When DATE_PART('days', NOW() - obsdtinclusao) > 60 Then 'Esta obra está desatualizada' 
				    End || to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			   When obr.stoid IN (3) 
			     Then 'Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')) || DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
			     Else '' || 
			       Case 
			         When obrdtvistoria is not null 
			           Then to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obrdtvistoria) || ' dia(s) )' 
			           Else to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') || DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
			       End
			End as atualizacao,
			obr.obrpercexec as executado,
			COALESCE(esd.esddsc,'N/A') AS situacaoobra,
			obr.obrvlrrealobra
		From obras.grupodistribuicao grp
		
		Join obras.itemgrupo item ON item.gpdid = grp.gpdid
		Join obras.repositorio rep ON rep.repid = item.repid
		Join obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
		Join entidade.endereco ende ON ende.endid = obr.endid
		Join territorios.municipio mun ON mun.muncod = ende.muncod
		Join entidade.entidade ee ON ee.entid = obr.entidunidade
		
		Left Join obras.termoaditivo ta ON ta.obrid = obr.obrid
		Left Join obras.situacaoobra so ON so.stoid = obr.stoid
		Left Join workflow.documento wk ON wk.docid = obr.docid
		Left Join workflow.estadodocumento esd ON esd.esdid = wk.esdid
		Join workflow.documento wok ON wok.docid = grp.docid
		
		$join_orderServico
		
		Where obr.obsstatus = 'A' and wk.docid is not null and obr.obrsupemp is true
		
		Group by titulo, obr.obrid, obr.obrdesc, obr.obrdtinicio, obr.obrdttermino, obr.orgid, obr.obrpercexec, obr.obrvlrrealobra, obr.stoid,
			 obsdtinclusao, obrdtvistoria, esddsc,
			 esd.esdid, 
			 mun.estuf , mun.mundescricao, 
			 so.stodesc, 
			 ee.entnome
			 
		order by estado, ord, educacao, obrdesc			
	";
	return $sql;
}

function monta_agp(){
	$agp = 	array(	"agrupador" => array(), 
					"agrupadoColuna" => array("titulo","estado", "obrid", "obrdesc", "entdescricao", "inicio", "termino", "mundescricao", "educacao", "situacao", "atualizacao", "executado", "situacaoobra", "obrvlrrealobra")	  
			);		
	
	#Agrupador padrão, usado pelo componente. 
	array_push($agp['agrupador'], array("campo" => "obrid", "label" => "Código."));
		
	return $agp;
}

function monta_coluna(){
	
	$coluna = array();

	array_push( $coluna, 
					array(	"campo" 	=> "titulo",
				   		   	"label"		=> "Titulo",
				   		   	"type"	  	=> "string"
					) 
	);


	array_push( $coluna, 
					array(	"campo" 	=> "estado",
				   		   	"label"		=> "Estado",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "obrid",
				   		   	"label"		=> "ID",
				   		   	"type"	  	=> "numeric"
					)
	);

	array_push( $coluna, 
					array(	"campo" 	=> "obrdesc",
				   		   	"label"		=> "Descrição",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "entdescricao",
				   		   	"label"		=> "Unidade Implantadora",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "inicio",
				   		   	"label"		=> "Data de Início",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "termino",
				   		   	"label"		=> "Data de Término",
				   		   	"type"	  	=> "numeric"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "mundescricao",
				   		   	"label"		=> "Município",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "educacao",
				   		   	"label"		=> "Educação",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "situacao",
				   		   	"label"		=> "Situação de Execução da Obra",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "atualizacao",
				   		   	"label"		=> "Última Atualização",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "executado",
				   		   	"label"		=> "% Executado",
				   		   	"type"	  	=> "string"
					) 
	);

	array_push( $coluna, 
					array(	"campo" 	=> "situacaoobra",
				   		   	"label"		=> "Situação da Obra",
				   		   	"type"	  	=> "string"
					) 
	);
	
	array_push( $coluna, 
					array(	"campo" 	=> "obrvlrrealobra",
				   		   	"label"		=> "Valor da Obra",
				   		   	"type"	  	=> "numeric"
					) 
	);	

	return $coluna;
}



	
	#Bloco que monta os totalizadores do Grid.
	#Caso exista o post dos filtros Data é incluido a consulta uma clasula join.
	#Variavél global = $join_dataOrderServico foi criada, para que seja usada na função 
	$join_dataOrderServico = "";
	if($_REQUEST['htddataini'] != '' && $_REQUEST['htddatafim']){
		$dataini = formata_data_sql($_POST['htddataini']);
		$datafim = formata_data_sql($_POST['htddatafim']);
		
		$join_dataOrderServico = "Join obras.ordemservico ord ON ord.gpdid = grp.gpdid and cast(ord.orsdtemissao as date) Between cast('$dataini' as date) and cast('$datafim' as date)";
		
		$_SESSION['join_data_ordem_serviço'] = $join_dataOrderServico; 
	}else{
		$join_dataOrderServico = "";
	}
	
	$sql = "
		select  mun.estuf,
				orgid,
				count(distinct obr.obrid) as totalobras
		from obras.repositorio rep
		inner join obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
		inner join entidade.endereco ende ON ende.endid = obr.endid
		left join territorios.municipio mun ON mun.muncod = ende.muncod
		left join workflow.documento wk ON wk.docid = obr.docid
		LEFT JOIN obras.itemgrupo ig ON ig.repid = rep.repid
		LEFT JOIN obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid
		LEFT JOIN workflow.documento wd ON wd.docid = gd.docid
		where  rep.repstatus = 'A' and obr.obsstatus = 'A' AND wk.esdid is null and wd.esdid is null and obr.obsstatus = 'A'
		group by  mun.estuf,orgid
		order by mun.estuf,orgid
	";
	$arrObrasRepositorio = $db->carregar($sql);
	$arrObrasRepositorio = trataArrayObras($arrObrasRepositorio);

	$sql = "
		select	ende.estuf,
				obr.orgid,
				count(distinct obr.obrid) as totalobras
		from obras.grupodistribuicao grp
		inner join obras.itemgrupo item ON item.gpdid = grp.gpdid
		inner join obras.repositorio rep ON rep.repid = item.repid
		inner join obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
		inner join entidade.endereco ende ON ende.endid = obr.endid
		left join territorios.municipio mun ON mun.muncod = ende.muncod
		inner join obras.situacaoobra so ON so.stoid = obr.stoid
		inner join workflow.documento wok ON wok.docid = grp.docid
		LEFT JOIN workflow.documento wdoc ON wdoc.docid = obr.docid
		LEFT JOIN workflow.estadodocumento west ON west.esdid = wdoc.esdid
		$join_dataOrderServico
		where rep.repstatus = 'A' and obr.obsstatus = 'A' and (wok.esdid <> 173 or wok.esdid is null) and wok.esdid in (156,157,158)
		group by ende.estuf,obr.orgid
		order by ende.estuf,obr.orgid
	";
	$arrObrasDistribuidas = $db->carregar($sql);
	$arrObrasDistribuidas = trataArrayObras($arrObrasDistribuidas);

	$sql = "
		select ende.estuf,
				obr.orgid,
				count(distinct obr.obrid) as totalobras
		from obras.grupodistribuicao grp
		inner join obras.itemgrupo item ON item.gpdid = grp.gpdid
		inner join obras.repositorio rep ON rep.repid = item.repid
		inner join obras.obrainfraestrutura obr ON obr.obrid = rep.obrid
		inner join entidade.endereco ende ON ende.endid = obr.endid
		left join territorios.municipio mun ON mun.muncod = ende.muncod
		inner join obras.situacaoobra so ON so.stoid = obr.stoid
		inner join workflow.documento wok ON wok.docid = grp.docid
		LEFT JOIN workflow.documento wdoc ON wdoc.docid = obr.docid
		LEFT JOIN workflow.estadodocumento west ON west.esdid = wdoc.esdid
		$join_dataOrderServico    	
		where rep.repstatus = 'A' and obr.obsstatus = 'A' and wok.esdid in (216,297,172) and (wok.esdid <> 173 or wok.esdid is null)
		group by ende.estuf,obr.orgid
		order by ende.estuf,obr.orgid
	";
	$arrObrasSupervisao = $db->carregar($sql);
	$arrObrasSupervisao = trataArrayObras($arrObrasSupervisao);

	$sql = "
		Select 	tm.estuf,
			oi.orgid,
			count(distinct oi.obrid) as totalobras
		From obras.itemgrupo ig
		Join obras.repositorio ore on ore.repid = ig.repid
		Join obras.obrainfraestrutura oi on oi.obrid = ore.obrid
		Join entidade.entidade ee on ee.entid = oi.entidunidade
		Join entidade.endereco ed on ed.endid = oi.endid
		
		Left Join obras.unidademedida ou on ou.umdid = oi.umdidobraconstruida
		Left Join territorios.municipio tm on tm.muncod = ed.muncod
		Left Join obras.orgao oo on oo.orgid = oi.orgid
		Left Join obras.situacaoobra so on so.stoid = oi.stoid
		Left Join workflow.documento wk on wk.docid = oi.docid
		Left Join workflow.estadodocumento esd on esd.esdid = wk.esdid
		
		Join obras.grupodistribuicao grp on grp.gpdid = ig.gpdid
		$join_dataOrderServico
		Where oi.obsstatus = 'A' and oi.obrsupemp is true 
		
		Group by tm.estuf,oi.orgid
		Order by tm.estuf,oi.orgid
	";
	$arrObrasSupervisaoFinalizadas = $db->carregar($sql);
	$arrObrasSupervisaoFinalizadas = trataArrayObras($arrObrasSupervisaoFinalizadas);


?>
<style>
	.tituloRelatorio{text-align:center;font-weight:bold}
	.numberRelatorio{text-align:right;color:blue}
	.bold{font-weight:bold}
	.link{cursor:pointer}
	.middle{vertical-align:middle}
	.totalRelatorio{text-align:right;font-weight:bold;color:blue}
</style>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script src="/includes/calendario.js"></script>

<script>
	function exibeObrasRepositorio(estuf,orgid,tipo)
	{
		var linha = document.getElementById('tr_' + estuf).rowIndex;
		
			jQuery("#img_" + estuf).attr("src","../imagens/menos.gif");
			if( jQuery("#tr_obras_" + estuf).html() ){
				if(jQuery("#tr_obras_ajax_carregado_" + estuf).val() == "tipo_" + tipo + "_estuf_" + estuf + "_orgid_" + orgid){
					jQuery("#tr_obras_" + estuf).show();
				}else{
					var parametros = "estuf=" + estuf + "&tipo=" + tipo + "&orgid=" + orgid;
					exibeObras("tr_obras_" + estuf,parametros);
					jQuery("#tr_obras_" + estuf).show();
				}
			}else{
				jQuery('#relatorio_vistoria > tbody > tr').eq( linha ).after("<tr bgcolor='#ffffff' ><td colspan='17' id='tr_obras_" + estuf + "' ></td></tr>");
				var parametros = "estuf=" + estuf + "&tipo=" + tipo + "&orgid=" + orgid;
				exibeObras("tr_obras_" + estuf,parametros);
				jQuery("#tr_obras_" + estuf).show();
			}
	}
	
	function exibeObrasPorEstado(obj,estuf)
	{
		var linha = obj.parentNode.parentNode.parentNode.rowIndex;
		 		
		if(jQuery(obj).attr("src") == "../imagens/mais.gif"){
			jQuery(obj).attr("src","../imagens/menos.gif");
			if( jQuery("#tr_obras_" + estuf).html() ){
				if(jQuery("#tr_obras_ajax_carregado_" + estuf).val() == "geral"){
					jQuery("#tr_obras_" + estuf).show();
				}else{
					jQuery('#relatorio_vistoria > tbody > tr').eq( linha ).after("<tr bgcolor='#ffffff' ><td colspan='17' id='tr_obras_" + estuf + "' ></td></tr>");
					var parametros = "estuf=" + estuf;
					exibeObras("tr_obras_" + estuf,parametros);
				}
				
			}else{
				jQuery('#relatorio_vistoria > tbody > tr').eq( linha ).after("<tr bgcolor='#ffffff' ><td colspan='17' id='tr_obras_" + estuf + "' ></td></tr>");
				var parametros = "estuf=" + estuf;
				exibeObras("tr_obras_" + estuf,parametros);
			}
		}else{
			jQuery(obj).attr("src","../imagens/mais.gif");
			jQuery("#tr_obras_" + estuf).hide();
		}
	}
	
	function exibeObras(destino,parametros){
		jQuery('#' + destino).html( '<center>Carregando... <img src="../imagens/wait.gif" /></center>' );
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoRelatorio=carregaObrasPorEstado&" + parametros,
			   success: function(msg){
			   		jQuery('#' + destino).html( msg );
			   }
		});
	}
	
	function visualizarObras(obrid){
		window.location.href = 'obras.php?modulo=principal/cadastro&acao=A&obrid=' + obrid;
	}

	function obrasPorEstadoXls(){	
		var janela = window.open( 'obras.php?modulo=relatorio/resumoSupervisao&acao=A&relatorio=obrasPorEstadoXls', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	}
	
	function pegarFiltroData(){
		var dataini 	= jQuery("[name='htddataini']").val();
		var datafim 	= jQuery("[name='htddatafim']").val();
		var error = 0;
		
		jQuery(":text").each(function() { 
			if(!this.value || this.value == ""){
				error = 1;
				alert('É necessario o preencher os campos de data INICIAL e FINAL!');
				this.focus();
				return false;
			}
		});
	
		if (new Date(jQuery("[name='htddataini']").val()) > new Date(jQuery("[name='htddatafim']").val())){
			error = 1;
			alert('A data INICIAL é maior que a data FINAL');
		}
			
		if(error == 0){
			var formulario = document.formulario;
			jQuery.ajax({
				type: "POST",
				url	: 'obras.php?modulo=relatorio/resumoSupervisao&acao=A',
				data: "requisicao=filtroData&htddataini=" + dataini + "&htddatafim=" + datafim,
				success: function(msg){
					alert('Filtro aplicado com sucesso, as obras mostradas estão no intervalo indicado!');
					formulario.submit();
			   }
			});
		}
	}

	function clearFiltroData(){
		var formulario = document.formulario;
		jQuery('#htddataini').val('');
		jQuery('#htddatafim').val('');
		alert('Filtro retirado com sucesso, todas as obras estão sendo mostradas!');
		formulario.submit();
	}

</script>

<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
		        <td class="SubTituloDireita" valign="top">Data da OS:</td>
		        <td>
		           <?= campo_data( 'htddataini', 'N','S','','','','' ); ?> (dd/mm/aaaa) 
		           &nbsp;&nbsp;&nbsp;até &nbsp; 
		           <?= campo_data( 'htddatafim', 'N','S','','','','' ); ?> (dd/mm/aaaa)
		        </td>
	    	</tr>
			<tr>
		        <td class="SubTituloDireita" valign="top">OBS:</td>
		        <td>
		           <b style="color: #CD2626;">O filtro por data de OS só terá relação com grupos que estejam em supervisão.</b>
		        </td>
	    	</tr>	    	
			<tr>
		        <td class="SubTituloDireita" valign="top">&nbsp;</td>
		        <td>
		           <input type="button" name="gerarFiltroData" value="Pesquisar" onclick="javascript: pegarFiltroData();"/>
		           <input type="button" name="limparFiltroData" value="Limpar Filtro" onclick="javascript: clearFiltroData();"/>
		        </td>
	    	</tr>	    	
	    </table>
	    
	    <br/>
	       
		<table id="relatorio_vistoria" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr bgcolor="#D5D5D5" >
				<td></td>
				<td colspan="4" class="tituloRelatorio" >Obras no Repositório</td>
				<td colspan="4" class="tituloRelatorio" >Obras Distribuídas</td>
				<td colspan="4" class="tituloRelatorio" >Obras em Supervisão</td>
				<td colspan="4" class="tituloRelatorio" >Obras com Supervisão Finalizada</td>
			</tr>
			<tr bgcolor="#D5D5D5" >
				<td class="tituloRelatorio" >UF</td>
				<?php for($i=0;$i<4;$i++): ?>
					<td class="tituloRelatorio" >Educação Básica</td>
					<td class="tituloRelatorio" >Educação Superior</td>
					<td class="tituloRelatorio" >Educação Profissional</td>
					<td class="tituloRelatorio" >Total</td>
				<?php endfor; ?>
			</tr>
			<?php $arrUF = recuperaUF() ?>
			<?php $n=0;foreach($arrUF as $uf): ?>
				<?php $cor = $n%2==0 ? "#fcfcfc" : "" ?>
				<tr id="tr_<?php echo $uf ?>" bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
					<td>
						<span style="white-space:nowrap" >
							<img title="Exibir Obras" id="img_<?php echo $uf ?>" src="../imagens/mais.gif" class="link middle" onclick="exibeObrasPorEstado(this,'<?php echo $uf ?>')"  /> 
							<?php echo $uf ?>
						</span>
					</td>
					<?php for($i=0;$i<4;$i++): ?>
						<?php 
						switch($i){
							case 0:
								$arrDados = $arrObrasRepositorio;
								$tipo = "repositorio";
							break;
							case 1:
								$arrDados = $arrObrasDistribuidas;
								$tipo = "distribuidas";
							break;
							case 2:
								$arrDados = $arrObrasSupervisao;
								$tipo = "supervisao";
							break;
							case 3:
								$arrDados = $arrObrasSupervisaoFinalizadas;
								$tipo = "finalizadas";
							break;
						}
						?>
		
						<td class="numberRelatorio link" onclick="exibeObrasRepositorio('<?php echo $uf ?>','3','<?php echo $tipo ?>')" ><?php $total[$tipo][3] += $arrDados[$uf][3]; echo simec_number_format($arrDados[$uf][3]) ?></td>
						<td class="numberRelatorio link" onclick="exibeObrasRepositorio('<?php echo $uf ?>','1','<?php echo $tipo ?>')" ><?php $total[$tipo][1] += $arrDados[$uf][1]; echo simec_number_format($arrDados[$uf][1]) ?></td>
						<td class="numberRelatorio link" onclick="exibeObrasRepositorio('<?php echo $uf ?>','2','<?php echo $tipo ?>')" ><?php $total[$tipo][2] += $arrDados[$uf][2]; echo simec_number_format($arrDados[$uf][2]) ?></td>
						<td class="numberRelatorio bold" ><?php echo simec_number_format($arrDados[$uf][1] + $arrDados[$uf][2] + $arrDados[$uf][3]) ?></td>
					<?php endfor; ?>
				</tr>
			<?php $n++;endforeach; ?>
			<tr bgcolor="#D5D5D5" >
				<td class="totalRelatorio" >Total</td>
				<?php $arrTipo = array("repositorio","distribuidas","supervisao","finalizadas") ?>
				<?php foreach($arrTipo as $tipo): ?>
					<td class="totalRelatorio" ><?php echo simec_number_format($total[$tipo][3]) //simec_number_format($total[$tipo][3]) ?></td>
					<td class="totalRelatorio" ><?php echo simec_number_format($total[$tipo][1])//simec_number_format($total[$tipo][1]) ?></td>
					<td class="totalRelatorio" ><?php echo simec_number_format($total[$tipo][2])//simec_number_format($total[$tipo][2]) ?></td>
					<td class="totalRelatorio" ><?php echo simec_number_format($total[$tipo][3] + $total[$tipo][2] + $total[$tipo][1])//simec_number_format($total[$tipo][3] + $total[$tipo][2] + $total[$tipo][1]) ?></td>
				<?php endforeach; ?>
			</tr>
			<tr>
				<td>
					<input type="button" name="GerarRelatorio" value="Visualizar XLS" onclick="javascript:obrasPorEstadoXls();"/>
				</td>
			</tr>
		</table>
</form>
