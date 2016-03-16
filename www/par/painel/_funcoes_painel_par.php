<?php

function detalhe_repasse_obras(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Repasse de Obras',
			'cabecalho' => Array('Ações', 'Preid', 'Obrid', 'Nome da obra', 'Tipo da Obra', 'Funcional Programática', 'Valor empenhado', 'Pagamento solicitado', 'Pagamento efetivado', 'Pagamento total',	
								 'Situação obras 2', 'Restrições obras 2', 'Recebeu mobiliário (Apenas Creche)' ),
			'dados'		=> Array()
	);
	
	if( $_POST['inuid'] ){
		$where = " inu.inuid = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " inu.inuid IN (SELECT inu1.inuid
									   FROM par.instrumentounidade inu1
									   INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
									   WHERE inu2.inuid = {$_POST['inuid']})";
		}
	}else{
		$where = "1=1";
	}
	
	if( is_array($_SESSION['par']['painel']['filtro2']) ){
		foreach( $_SESSION['par']['painel']['filtro2'] as $filtro ){
			if( $filtro['campo'] == 'origem' ){
				$sinal = $filtro['valor'] == 'PAC' ? '=' : '<>';
				$where .= " AND tooid $sinal 1";
			}
			if( $filtro['campo'] == 'ptodescricao' ){
				switch($filtro['valor']){
					case 'Quadra':
						$ptoclassificacao = 'Q';
						$where .= " AND ptoclassificacaoobra = '$ptoclassificacao'";
						break;
					case 'Cobertura':
						$ptoclassificacao = 'C';
						$where .= " AND ptoclassificacaoobra = '$ptoclassificacao'";
						break;
					case 'Creche': 
						$ptoclassificacao = 'P';
						$where .= " AND ptoclassificacaoobra = '$ptoclassificacao'";
						break;   
					default:
						$sql = "SELECT ptoid FROM obras.pretipoobra WHERE ptodescricao ilike '%".utf8_decode($filtro['valor'])."%'";
						$ptoid = $db->pegaUm( $sql );
						if( $_POST['inuid'] ){
							$where .= " AND pt.ptoid = $ptoid";
						}else{
							$where .= " AND ptoid = $ptoid";
						}
						break;
				}
			}
		}
	}
	
	if( $_POST['anoprocesso'] ){
		$where .= " and substring(pop.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	$sql = "SELECT 
				'<span style=\"color:#428bca;font-size:12px;cursor:pointer;\" title=\"Ver Pré-Obra\" class=\"preobra glyphicon glyphicon-export\" preid='|| pre.preid ||' tooid='|| pre.tooid ||' > </span>
				<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=\"Ver Obra\" class=\"obra glyphicon glyphicon-export\" obrid='|| o.obrid ||' > </span>' as acoes ,
				'('||pre.preid||')' as preid, '('||pre.obrid||')' as obrid, pre.predescricao, pt.ptodescricao,
				array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod ||'</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empid in (select empid from par.empenhoobrapar where preid = pre.preid)), '<br>') as funcional, 
                SUM(de.saldo) AS valorempenhado,  
                (
                               SELECT sum(popvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobrapar po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
                               AND po.preid = pre.preid
                ) AS pagamentosolicitado, 
                (
                               SELECT sum(popvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobrapar po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('2 - EFETIVADO')
                               AND po.preid = pre.preid
                ) AS pagamentoefetivado,
                (
                               SELECT sum(popvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobrapar po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF','2 - EFETIVADO')
                               AND po.preid = pre.preid
                ) AS pagamentototal,
                (SELECT DISTINCT esddsc FROM workflow.estadodocumento esd
				INNER JOIN workflow.documento doc ON doc.esdid = esd.esdid
				WHERE doc.docid = o.docid limit 1) AS situacaoobras2,
				CASE WHEN (SELECT DISTINCT TRUE FROM obras2.restricao WHERE obrid = o.obrid LIMIT 1)
					THEN 'SIM <span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Restrições class=\"restricoes glyphicon glyphicon-download\" obrid='|| o.obrid ||' > </span>'
					ELSE 'NÃO'
				END as restricoes,
                ' - ' AS recebeumobiliario
			FROM par.v_saldo_empenho_por_obra  de
			INNER JOIN par.processoobraspar 	pop ON pop.pronumeroprocesso = de.processo  
			INNER JOIN par.instrumentounidade 	inu ON inu.inuid = pop.inuid
			INNER JOIN obras.preobra 			pre ON pre.preid = de.preid
			INNER JOIN obras.pretipoobra 		pt  ON pt.ptoid = pre.ptoid
			INNER JOIN obras2.obras 			o   ON o.preid = pre.preid AND o.obridpai is null AND o.obrstatus = 'A'
			WHERE $where  
			GROUP BY acoes , pre.preid, pre.obrid, pre.predescricao, pt.ptodescricao, o.docid, o.obrid, pre.tooid
			HAVING SUM(de.saldo) > 0
			UNION ALL 
			SELECT 
				'<span style=\"color:#428bca;font-size:12px;cursor:pointer;\" title=\"Ver Pré-Obra\" class=\"preobra glyphicon glyphicon-export\" preid='|| pre.preid ||' tooid='|| pre.tooid ||' > </span>
				<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=\"Ver Obra\" class=\"obra glyphicon glyphicon-export\" obrid='|| o.obrid ||' > </span>' as acoes ,
				'('||pre.preid||')', '('||pre.obrid||')', pre.predescricao, pt.ptodescricao,
				array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod ||'</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empid in (select empid from par.empenhoobra where preid = pre.preid)), '<br>') as funcional, 
                SUM(de.saldo) AS valorempenhado,  
                (
                               SELECT sum(pobvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobra po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
                               AND po.preid = pre.preid
                ) AS pagamentosolicitado, 
                (
                               SELECT sum(pobvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobra po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('2 - EFETIVADO')
                               AND po.preid = pre.preid
                ) AS pagamentoefetivado,
                (
                               SELECT sum(pobvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobra  po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF','2 - EFETIVADO')
                               AND po.preid = pre.preid
                ) AS pagamentototal,
                (SELECT DISTINCT esddsc FROM workflow.estadodocumento esd
				INNER JOIN workflow.documento doc ON doc.esdid = esd.esdid
				INNER JOIN obras2.obras obr ON obr.docid = doc.docid
				WHERE pre.preid = obr.preid AND obr.obrstatus = 'A' limit 1) AS situacaoobras2,
				CASE WHEN (SELECT DISTINCT TRUE FROM obras2.restricao WHERE obrid = o.obrid LIMIT 1)
					THEN 'SIM <span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Restrições class=\"restricoes glyphicon glyphicon-download\" obrid='|| o.obrid ||' > </span>'
					ELSE 'NÃO'
				END as restricoes,
                coalesce(            (
                               SELECT DISTINCT 
                               (  CASE WHEN   
                                               (
                                               SELECT CASE WHEN sum(e.saldo) > 0 THEN TRUE ELSE FALSE END
                                               FROM par.subacaoobravinculacao sv
                                               INNER JOIN par.v_saldo_empenho_por_subacao e ON e.sbaid = sv.sbaid AND eobano = sv.sovano 
                                               WHERE sv.preid =  pre2.preid 
                                               GROUP BY sv.preid 
                                               --HAVING sum(e.saldo) > 0
                                               ) = TRUE THEN 'SIM' ELSE 'NÃO' END     
                               ) AS temmobiliario
                               FROM obras.preobra pre2
                               INNER JOIN par.v_saldo_empenho_por_obra e ON e.preid = pre2.preid
                               INNER JOIN par.processoobra p ON p.pronumeroprocesso = e.processo
                               WHERE prestatus = 'A'
                               AND pre2.preid = pre.preid
                               AND prostatus = 'A'
                               AND p.protipo = 'P'
                               GROUP BY pre2.preid
                               HAVING sum(e.saldo) > 0
                               
                               ), 'não se aplica')
                AS temmobiliario
			FROM par.v_saldo_empenho_por_obra  de
			INNER JOIN par.processoobra 				pop ON pop.pronumeroprocesso = de.processo 
			INNER JOIN par.instrumentounidade 			inu ON ( inu.muncod = pop.muncod AND inu.itrid = 2 AND pop.estuf IS NULL ) OR (inu.estuf = pop.estuf AND inu.itrid = 1 AND pop.muncod IS NULL ) OR (inu.estuf =pop.estuf AND inu.itrid = 2 AND pop.estuf = 'DF')
			INNER JOIN obras.preobra 					pre ON pre.preid = de.preid
			INNER JOIN obras.pretipoobra 				pt  ON pt.ptoid = pre.ptoid
			INNER JOIN obras2.obras 					o   ON o.preid = pre.preid AND o.obridpai is null AND o.obrstatus = 'A'
			WHERE pop.prostatus = 'A' AND $where
			GROUP BY acoes , pre.preid, pre.obrid, pre.predescricao, pt.ptodescricao , protipo, o.docid, o.obrid, pre.tooid
			HAVING SUM(de.saldo) > 0";

	if( TRIM($_POST['inuid']) == '' ){
		$sql = "SELECT acoes, preid, obrid, predescricao, ptodescricao, funcional, valorempenhado, pagamentosolicitado, pagamentoefetivado, pagamentototal, situacaoobras2, restricoes, recebeumobiliario
				FROM par.vm_temporiaria_detalhe_repasse_obras 
				WHERE $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();
	
	return $arrDetalhes;
}

function detalhe_repasse_programas_par(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Repasse por Programas PAR',
			'cabecalho' => Array('Ações', 'Processo', 'Termo', 'Funcional Programática', 'Programa', 'Valor empenhado', 'Valor pago', 'Contrato', 'Nota' ),
			'dados'		=> Array(Array('teste', 'teste', 'teste', 'teste'),Array('teste', 'teste', 'teste', 'teste'),Array('teste', 'teste', 'teste', 'teste'))
	);
	
        # Swith invertido para atender o gato na query e poder atender mais programas, os quais não atende no sql.
        $arrPrgid = array();
        switch (utf8_decode($_POST['detalhe_programa_nome'])) {
            case 'Tablet':
                $arrPrgid = array(154, 186); 
                break;
            case 'Equipamentos':
                $arrPrgid = array(104, 236, 161, 103, 208, 75, 235, 210, 245); 
                break;
            case 'BRASIL PRO - Equipamentos':
                $arrPrgid = array(170); 
                break;
            case 'Climatização':
                $arrPrgid = array(106, 105); 
                break;
            case 'Instrumento Musicais':
                $arrPrgid = array(95); 
                break;
            case 'Formacao Continuada Ed. especial':
                $arrPrgid = array(139, 140, 204); 
                break;
            case 'BPC na Escola':
                $arrPrgid = array(146); 
                break;
            case 'Inclusão e Diversidade':
                $arrPrgid = array(68, 206); 
                break;
            case 'Caminho da Escola':
                $arrPrgid = array(50, 81, 159, 51); 
                break;
            case 'Formacao Indígena':
                $arrPrgid = array(147, 148); 
                break;
            case 'Mobiliário':
                $arrPrgid = array(48, 76, 162, 109, 165, 211); 
                break;
            case 'BRASIL PRO - Mobiliário':
                $arrPrgid = array(171); 
                break;
            case 'Projetor':
                $arrPrgid = array(49, 163); 
                break;
            case 'Computador':
                $arrPrgid = array(187); 
                break;
            case 'Uniforme Escolar':
                $arrPrgid = array(160); 
                break;
            case 'Conferência Infanto Juvenil':
                $arrPrgid = array(145); 
                break;
            case 'Brasil Pro - Laboratórios':
                $arrPrgid = array(175); 
                break;
            case 'Ônibus Escolar Acessível':
                $arrPrgid = array(164, 158, 153); 
                break;
            case 'Educação em Prisões':
                $arrPrgid = array(141, 143, 142); 
                break;
            case 'Educação no Campo':
                $arrPrgid = array(207); 
                break;
            case 'Salas Multifuncionais':
                $arrPrgid = array(53); 
                break;
            case 'Livro Acessível':
                $arrPrgid = array(137, 138); 
                break;
            case 'Formacao EJA':
                $arrPrgid = array(205); 
                break;
            case 'Correção de Fluxo Escolar':
                $arrPrgid = array(27); 
                break;
            case 'Material Ditático Especifico Indigena':
                $arrPrgid = array(149); 
                break;
            case 'Apoio Conselhos Municipais Educação':
                $arrPrgid = array(241); 
                break;
            case 'Outros':
                $arrPrgid = array(2); 
                break;
            default:
                $arrPrgid = array(0); 
                break;
        }
        
        if( trim($_POST['inuid']) != '' ){
            $where = " AND iu.inuid = {$_POST['inuid']}";
            if( $_POST['esfera'] == 'EM' ){
                $where = " AND iu.inuid IN (	
                                SELECT 
                                    inu1.inuid
                                FROM par.instrumentounidade inu1
                                INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
                                WHERE inu2.inuid = {$_POST['inuid']})";
            }

            if($_POST['detalhe_programa_nome']){
                if (!in_array(0, $arrPrgid)) {
                    $where .= " AND pro.prgid IN (".implode(',', $arrPrgid).")";
                } else {
                    $where .= " AND prgdsc ILIKE '%".utf8_decode($_POST['detalhe_programa_nome'])."%' ";
                }
            }

            if( $_POST['anoprocesso'] ){
                $where .= " AND substring(p.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
            }
	}else{
            if($_POST['detalhe_programa_nome']){
                if (!in_array(0, $arrPrgid)) {
                    $where .= " AND prgid IN (".implode(',', $arrPrgid).")";
                } else {
                    $where .= " AND prgdsc ILIKE '%".utf8_decode($_POST['detalhe_programa_nome'])."%' ";
                }
            }

            if( $_POST['anoprocesso'] ){
                $where .= " AND ano = '{$_POST['anoprocesso']}'";
            }
	}
	
	$sql = "SELECT  
				'<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Baixar class=\"termo glyphicon glyphicon-download\" title=\"Aderir ao pregão\" dopid='|| d.dopid ||' > </span>' as acao,
           		substring(p.prpnumeroprocesso from 1 for 5)||'.'||
				substring(p.prpnumeroprocesso from 6 for 6)||'/'||
				substring(p.prpnumeroprocesso from 12 for 4)||'-'||
				substring(p.prpnumeroprocesso from 16 for 2) as numeroprocesso,
                'N°: '||d.dopnumerodocumento as termo,
                array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod ||'</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empnumeroprocesso = p.prpnumeroprocesso), '<br>') as funcional,
                pro.prgdsc,
                sum(es.saldo) AS valorempenhado,
                sum(pobvalorpagamento) AS valorpago,
                CASE WHEN (SELECT DISTINCT TRUE FROM par.subacaoitenscomposicaoContratos sic
					   INNER JOIN par.subacaoitenscomposicao		ico ON ico.icoid = sic.icoid AND ico.icostatus = 'A'
					   WHERE ico.sbaid = sd.sbaid AND ico.icoano = sd.sbdano )
					THEN 'SIM'
					ELSE 'Não'
				END as possui_contrato,
				CASE WHEN (SELECT DISTINCT TRUE FROM par.subacaoitenscomposicaonotasfiscais 	snf
					   INNER JOIN par.subacaoitenscomposicao		ico ON ico.icoid = snf.icoid AND ico.icostatus = 'A'
					   WHERE ico.sbaid = sd.sbaid AND ico.icoano = sd.sbdano )
					THEN 'SIM'
					ELSE 'Não'
				END as possui_nota
			FROM par.processopar p
			INNER JOIN par.processoparcomposicao pc ON pc.prpid = p.prpid
			INNER JOIN par.vm_documentopar_ativos d ON d.prpid = p.prpid 
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = pc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid AND s.sbastatus = 'A'
			LEFT JOIN par.v_saldo_empenho_por_subacao   es ON  es.empnumeroprocesso = p.prpnumeroprocesso --es.sbaid = s.sbaid  and sd.sbdano = es.eobano
			LEFT JOIN par.pagamento pag on pag.empid = es.empid AND pag.pagstatus = 'A'
			LEFT JOIN par.pagamentosubacao ps ON pag.pagid = ps.pagid
			INNER JOIN par.programa pro ON pro.prgid = s.prgid
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod AND iu.itrid = 2
			LEFT JOIN territorios.estado e ON e.estuf = iu.estuf AND iu.itrid = 1
			WHERE prpstatus = 'A' $where
			GROUP BY p.prpnumeroprocesso, pro.prgdsc , d.dopnumerodocumento, d.dopid, sd.sbaid, sd.sbdano";
	
	if( trim($_POST['inuid']) == '' ){
		$sql = "SELECT acao, numeroprocesso, termo, funcional, prgdsc, valorempenhado, valorpago, possui_contrato, possui_nota 
				FROM par.vm_temporaria_detalhe_repasse_programas_par
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();
	
	return $arrDetalhes;
}

function detalhes_valor_pactuado(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Valor Pactuado',
			'cabecalho' => Array('Ações', 'Processo', 'N° do Termo', 'Funcional Programática', 'Tipo de Documento', 'Data de Vigência', 'Valor do Termo', 'Valor Empenhado', 'Pagamento Solicitado', 'Pagamento Efetivado' )
	);
	
	$where = " = {$_POST['inuid']}";
	if( $_POST['esfera'] == 'EM' ){
		$where = " IN (	SELECT inu1.inuid
						FROM par.instrumentounidade inu1
						INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
						WHERE inu2.inuid = {$_POST['inuid']})";
	}
	
	if( $_POST['anoprocesso'] ){
		$where1 = " and substring(pro.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
		$where2 = " and substring(pro.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
	}

	$sql = "-- PAC
			SELECT   
				acao,
				substring(processo from 1 for 5)||'.'||
				substring(processo from 6 for 6)||'/'||
				substring(processo from 12 for 4)||'-'||
				substring(processo from 16 for 2) as numeroprocesso,
               	'N°: '||numerotermo as numerotermo,
               	array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod ||'</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empnumeroprocesso = processo), '<br>') as funcional,
               	tipodocumento,
               	datavigencia,
               	valortermo,
               	valorempenhado,
               	pagamentosolicitado,
              	pagamentoefetivado
			FROM (
				SELECT DISTINCT
					'<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Baixar class=\"termo_pac glyphicon glyphicon-download\" title=\"Visualizar Termo\" terid='|| tc.terid ||' > </span>' as acao,
					pro.pronumeroprocesso AS processo,
					'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY')  AS numerotermo,
					'Termo de Compromisso' AS tipodocumento,
					'-' AS datavigencia,
					( select sum( prevalorobra ) from par.termoobra ter inner join obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A' WHERE ter.terid = tc.terid ) AS valortermo,
					v.saldo AS valorempenhado,
					(
						SELECT sum(pagvalorparcela)
					    FROM par.empenho emp 
					    INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
					    WHERE pag.pagstatus = 'A' 	AND emp.empnumeroprocesso = pro.pronumeroprocesso
                    ) AS pagamentosolicitado,
					(
						SELECT valorpago
					    FROM par.vm_valor_pago_por_processo
					    WHERE processo = pro.pronumeroprocesso
					) AS pagamentoefetivado
               	FROM par.processoobra pro 
				INNER JOIN par.instrumentounidade 			inu ON ( inu.muncod = pro.muncod AND inu.itrid = 2 AND pro.estuf IS NULL ) OR (inu.estuf = pro.estuf AND inu.itrid = 1 AND pro.muncod IS NULL ) OR (inu.estuf =pro.estuf AND inu.itrid = 2 AND pro.estuf = 'DF')
               	INNER JOIN par.processoobraspaccomposicao 	p   ON pro.proid = p.proid
               	INNER JOIN par.termocompromissopac  		tc  ON pro.proid = tc.proid and tc.terstatus = 'A'
               	INNER JOIN obras.preobra 					pre ON pre.preid = p.preid AND pre.prestatus = 'A'
               	LEFT JOIN par.vm_saldo_empenho_do_processo  v   ON v.processo = pro.pronumeroprocesso
               	WHERE inu.inuid $where AND pro.prostatus = 'A' $where1
				UNION ALL
				-- PAR OBRAS
               	SELECT DISTINCT
					'<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Baixar class=\"termo glyphicon glyphicon-download\" title=\"Visualizar Termo\" dopid='|| dp.dopid ||' > </span>' as acao,
					pro.pronumeroprocesso AS processo,
                    dp.dopnumerodocumento::text AS numerotermo,
                    dp.mdonome AS tipodocumento,
                    (             
	                    SELECT dopdatafimvigencia FROM par.documentopar  d
	                    INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
	                    WHERE d.proid = pro.proid AND dopstatus <> 'E' AND dpvstatus = 'A' AND mdoid not in (79,65,66,68,76,80,67,73,82)
	                    ORDER BY d.dopid desc
	                    LIMIT 1
                    ) AS datavigencia,
                    dp.dopvalortermo AS valortermo,
                    v.saldo AS valorempenhado,
                   	(
	                   	SELECT sum(pagvalorparcela)
	                   	FROM par.empenho emp 
	                   	INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
	                   	WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.pronumeroprocesso
                   	) AS pagamentosolicitado,
                   	(
	                   	SELECT valorpago
	                   	FROM par.vm_valor_pago_por_processo 
	                   	WHERE processo = pro.pronumeroprocesso
                   	) AS pagamentoefetivado
               	FROM par.processoobraspar pro 
               	INNER JOIN par.processoobrasparcomposicao 	p   ON pro.proid = p.proid
               	LEFT JOIN par.vm_documentopar_ativos 				dp  ON dp.proid = pro.proid
               	LEFT JOIN par.modelosdocumentos   			d   ON d.mdoid = dp.mdoid
               	INNER JOIN obras.preobra 					pre ON pre.preid = p.preid AND pre.prestatus = 'A'
               	LEFT JOIN par.vm_saldo_empenho_do_processo  v   ON v.processo = pro.pronumeroprocesso
               	WHERE pro.inuid $where AND pro.prostatus = 'A' $where1
				UNION ALL
				-- PAR  GENERICO
				SELECT DISTINCT
					'<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Baixar class=\"termo glyphicon glyphicon-download\" title=\"Visualizar Termo\" dopid='|| dp.dopid ||' > </span>' as acao,
              		pro.prpnumeroprocesso AS processo,
               		dp.dopnumerodocumento::text AS numerotermo,
               		dp.mdonome AS tipodocumento,
               		(              
                    	select dopdatafimvigencia from par.documentopar  d
                        inner join par.documentoparvalidacao v ON d.dopid = v.dopid
                        where d.prpid = pro.prpid AND dopstatus <> 'E' AND dpvstatus = 'A' AND mdoid not in (79,65,66,68,76,80,67,73,82)
						order by d.dopid desc
                        LIMIT 1
					) AS datavigencia,
               		-- dp.dopvalortermo AS valortermo,
               		SUM(par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano )) AS valortermo,
               		v.saldo AS valorempenhado,
                    (
                    	SELECT sum(pagvalorparcela)
                        FROM par.empenho emp 
                        INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
                        WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.prpnumeroprocesso
					) AS pagamentosolicitado,
                    (
                   		SELECT valorpago
                        FROM par.vm_valor_pago_por_processo 
                        WHERE processo = pro.prpnumeroprocesso
					) AS pagamentoefetivado
				FROM par.processopar pro 
				INNER JOIN par.processoparcomposicao 		p  ON pro.prpid = p.prpid
				INNER JOIN par.subacaodetalhe 				sd ON sd.sbdid = p.sbdid
				LEFT JOIN par.vm_documentopar_ativos 				dp ON dp.prpid = pro.prpid
				LEFT JOIN par.modelosdocumentos   			d  ON d.mdoid = dp.mdoid
				LEFT JOIN par.vm_saldo_empenho_do_processo  v  ON v.processo = pro.prPnumeroprocesso
				WHERE pro.inuid $where 
				AND pro.prpstatus = 'A' $where2
				GROUP BY 
					dp.dopid,
					pro.prpnumeroprocesso,
					dp.dopnumerodocumento,
					dp.mdonome,
					datavigencia,
					valorempenhado,
					pagamentosolicitado,
					pagamentoefetivado
				) AS foo";
	
	if( TRIM($_POST['inuid']) == '' ){
		$where = '';
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}' ";
		}
		$sql = "SELECT acao, numeroprocesso, numerotermo, funcional, tipodocumento, datavigencia, valortermo, valorempenhado, pagamentosolicitado, pagamentoefetivado 
				FROM par.vm_temporaria_detalhes_valor_pactuado
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();
	
	return $arrDetalhes;
}

function detalhes_valor_empenhado(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Valor Empenhado',
			'cabecalho' => Array('Processo', 'Tipo', 'Funcional Programática', 'Valor empenhado', 'Valor cancelado', 'Valor reforço', 'Saldo' ),
			'dados'		=> Array()
	);
	
	if( trim($_POST['inuid']) != '' ){
		
		$where = " = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = {$_POST['inuid']})";
		}
		
		if( $_POST['anoprocesso'] ){
			$where1 = " AND substring(po.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
			$where2 = " AND substring(pp.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
		}
	}else{
		
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}' ";
		}
	}
	
	
	$sql = "SELECT  
				substring(proce from 1 for 5)||'.'||
				substring(proce from 6 for 6)||'/'||
				substring(proce from 12 for 4)||'-'||
				substring(proce from 16 for 2) as numeroprocesso,
               	tipo,
               	array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod ||'</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empnumeroprocesso = proce), '<br>') as funcional,
               	SUM(valorempenho) AS valorempenho,
               	SUM(valorcancelado) AS valorcancelado,
               	SUM(valorreforco) AS valorreforco,
			    SUM(saldo) AS saldo
			FROM (
			-- processo do par
			SELECT  
				pp.prpnumeroprocesso as proce, 'PAR' as tipo, 
	            SUM(valorempenho) AS valorempenho,
	            SUM(valorcancelado) AS valorcancelado,
	            SUM(valorreforco) AS valorreforco, 
	            SUM(de.saldo) AS saldo
			FROM par.v_saldo_por_empenho    de
			INNER JOIN par.processopar pp ON pp.prpnumeroprocesso = de.processo 
			WHERE pp.inuid $where $where2
			group BY pp.prpnumeroprocesso
			UNION ALL 
			-- processo do pac
			SELECT  
				po.pronumeroprocesso  as proce, 'PAC' as tipo, SUM(valorempenho) AS valorempenho,
                SUM(valorcancelado) AS valorcancelado,
                SUM(valorreforco) AS valorreforco, 
                SUM(de.saldo) AS saldo
			FROM par.v_saldo_por_empenho    de
			INNER JOIN par.processoobra 		po  ON po.pronumeroprocesso = de.processo
			INNER JOIN par.instrumentounidade	inu ON ( inu.muncod = po.muncod AND inu.itrid = 2 AND po.estuf IS NULL ) OR (inu.estuf = po.estuf AND inu.itrid = 1 AND po.muncod IS NULL ) OR (inu.estuf =po.estuf AND inu.itrid = 2 AND po.estuf = 'DF')
			WHERE po.prostatus = 'A' AND inu.inuid = {$_POST['inuid']} $where1
			group by po.pronumeroprocesso
			UNION ALL 
			-- processo obras do par
			SELECT  po.pronumeroprocesso  as proce, 'Obras PAR' as tipo, SUM(valorempenho) AS valorempenho,
			               SUM(valorcancelado) AS valorcancelado,
			               SUM(valorreforco) AS valorreforco, 
			               SUM(de.saldo) AS saldo
			FROM par.v_saldo_por_empenho    de
			INNER JOIN par.processoobraspar po ON po.pronumeroprocesso = de.processo  
			WHERE po.inuid $where  AND po.prostatus = 'A' $where1 AND proid IN ( SELECT DISTINCT proid FROM par.processoobrasparcomposicao  )
			GROUP BY po.pronumeroprocesso
			) AS FOO
			GROUP BY proce, tipo";
	
	if( trim($_POST['inuid']) == '' ){
		
		$sql = "SELECT numeroprocesso, tipo, funcional, valorempenho, valorcancelado, valorreforco, saldo
				FROM par.vm_temporaria_detalhes_valor_empenhado
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();
	
	return $arrDetalhes;
}

function detalhes_valor_repassado(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Valor Repassado',
			'cabecalho' => Array('Processo', 'Tipo', 'Pagamento solicitado', 'Pagamento efetivado', 'Saldo' ),
			'dados'		=> Array()
	);
	
	if( trim($_POST['inuid']) != '' ){
		
		$where = " = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = {$_POST['inuid']})";
		}
		
		if( $_POST['anoprocesso'] ){
			$where1 = " and substring(pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
			$where2 = " and substring(pro.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
		}
	}else{
		
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}' ";
		}
	}
	
	$sql = "SELECT
				substring(processo from 1 for 5)||'.'||
				substring(processo from 6 for 6)||'/'||
				substring(processo from 12 for 4)||'-'||
				substring(processo from 16 for 2) as numeroprocesso,  
               	tipo,
               	pagamentosolicitado,
              	pagamentoefetivado,
               	pagamentototal
			FROM (
				-- processo do par
				SELECT distinct 
	               prpnumeroprocesso AS processo,
	               'PAR' AS tipo,
	               (
					SELECT sum(pagvalorparcela)
                    FROM par.empenho emp 
                    INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
                    WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.prpnumeroprocesso
					) AS pagamentosolicitado,
                    (
                    SELECT valorpago
                    FROM par.vm_valor_pago_por_processo
                    WHERE processo = pro.prpnumeroprocesso
					) AS pagamentoefetivado,
	               	(
                    SELECT sum(pagvalorparcela)
                    FROM par.empenho emp 
                    INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF', '2 - EFETIVADO' )
                    WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.prpnumeroprocesso
                    ) AS pagamentototal
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processopar pro ON pro.prpnumeroprocesso = de.processo 
				INNER JOIN par.processoparcomposicao p ON pro.prpid = p.prpid 
				WHERE pro.inuid $where $where2
				UNION ALL 
				-- processo do pac
               	SELECT DISTINCT 
               		pronumeroprocesso AS processo,
	               	'PAC' AS tipo,
	               	(
                    SELECT sum(pagvalorparcela)
                    FROM par.empenho emp 
                    INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
                    WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = po.pronumeroprocesso
                    ) AS pagamentosolicitado,
                    (
                   	SELECT valorpago
					FROM par.vm_valor_pago_por_processo
                    WHERE processo = po.pronumeroprocesso
					) AS pagamentoefetivado,
               		(
                    SELECT sum(pagvalorparcela)
                    FROM par.empenho emp 
                    INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF', '2 - EFETIVADO' )
                    WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = po.pronumeroprocesso
                    ) AS pagamentototal
               	FROM par.v_saldo_por_empenho    de
               	INNER JOIN par.processoobra 				po  ON po.pronumeroprocesso = de.processo
				INNER JOIN par.instrumentounidade			inu ON ( inu.muncod = po.muncod AND inu.itrid = 2 AND po.estuf IS NULL ) OR (inu.estuf = po.estuf AND inu.itrid = 1 AND po.muncod IS NULL ) OR (inu.estuf =po.estuf AND inu.itrid = 2 AND po.estuf = 'DF')
               	INNER JOIN par.processoobraspaccomposicao 	p 	ON po.proid = p.proid
               	WHERE po.prostatus = 'A' AND inu.inuid $where $where1
				UNION ALL 
				-- processo obras do par
				SELECT distinct 
	               	pronumeroprocesso AS processo,
	               	'OBRAS PAR' AS tipo,
	               	(
                    SELECT sum(pagvalorparcela)
                    FROM par.empenho emp 
                    INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
                    WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.pronumeroprocesso
                    ) AS pagamentosolicitado,
                    (
                    SELECT valorpago
                    FROM par.vm_valor_pago_por_processo
                    WHERE processo = pro.pronumeroprocesso
					) AS pagamentoefetivado,
               		(
                    SELECT sum(pagvalorparcela)
                    FROM par.empenho emp 
                    INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF', '2 - EFETIVADO' )
                    WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.pronumeroprocesso
                    ) AS pagamentototal
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = de.processo 
				WHERE pro.inuid $where AND pro.prostatus = 'A' $where1 AND pro.proid in ( SELECT DISTINCT proid FROM par.processoobrasparcomposicao  )
				GROUP BY pronumeroprocesso
				) AS FOO";
	
	if( trim($_POST['inuid']) == ''){
		
		$sql = "SELECT numeroprocesso, tipo, pagamentosolicitado, pagamentoefetivado, pagamentototal 
				FROM par.vm_temporaria_detalhes_valor_repassado
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();

	return $arrDetalhes;
}

function detalhes_valor_saldo_conta(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Saldo em conta',
			'cabecalho' => Array('Ações', 'Processos', 'Tipo', 'Saldo' ),
			'dados'		=> Array()
	);
	
	if( trim($_POST['inuid']) != '' ){
		
		$where = " = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = {$_POST['inuid']})";
		}
		
		if( $_POST['anoprocesso'] ){
			$where1 = " and substring(p.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
			$where2 = " and substring(p.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
		}
	}else{
		
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}' ";
		}
	}

	$sql = "SELECT '<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=\"Ver Obra\" class=\"saldoprocesso glyphicon glyphicon-export\" processo='|| processo ||' > </span>' as acoes , 
				substring(processo from 1 for 5)||'.'||
				substring(processo from 6 for 6)||'/'||
				substring(processo from 12 for 4)||'-'||
				substring(processo from 16 for 2) as numeroprocesso, tipo, saldo 
			FROM
			(
			    -- obras par
			    SELECT dfi.dfiprocesso as processo, 'Obras PAR' as tipo,  max(dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo
			    FROM painel.dadosfinanceirosconvenios dfi 
			    INNER JOIN par.processoobraspar 			p    ON p.pronumeroprocesso = dfi.dfiprocesso
			    INNER JOIN par.processoobrasparcomposicao 	popc ON popc.proid = p.proid
			    WHERE p.inuid $where AND p.prostatus = 'A' $where1 AND TO_CHAR(dfi.dfidatasaldo, 'YYYYMM') = TO_CHAR((NOW() - INTERVAL '1 MONTH'), 'YYYYMM')
			    GROUP BY dfi.dfiprocesso
			UNION ALL
			    -- obras PAC
			    SELECT dfi.dfiprocesso as processo, 'PAC' as tipo, max(dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo
			    FROM painel.dadosfinanceirosconvenios dfi 
			    INNER JOIN par.processoobra 				p    ON p.pronumeroprocesso = dfi.dfiprocesso
				INNER JOIN par.instrumentounidade 			inu  ON ( inu.muncod = p.muncod AND inu.itrid = 2 AND p.estuf IS NULL ) OR (inu.estuf = p.estuf AND inu.itrid = 1 AND p.muncod IS NULL ) OR (inu.estuf =p.estuf AND inu.itrid = 2 AND p.estuf = 'DF')
			    INNER JOIN par.processoobraspaccomposicao 	popc ON popc.proid = p.proid
			    WHERE inu.inuid $where AND p.prostatus = 'A' $where1 AND to_char(dfi.dfidatasaldo, 'YYYYMM') = to_char((now() - INTERVAL '1 MONTH'), 'YYYYMM')
			    group by dfi.dfiprocesso
			UNION ALL
			      -- PAR GENERICO
			    SELECT dfi.dfiprocesso as processo, 'PAR' as tipo, max(dfi.dfisaldoconta + dfi.dfisaldofundo + dfi.dfisaldopoupanca + dfi.dfisaldordbcdb) AS saldo
			    FROM painel.dadosfinanceirosconvenios dfi 
			    INNER JOIN par.processopar 				p ON p.prpnumeroprocesso = dfi.dfiprocesso
			    INNER JOIN par.processoparcomposicao 	ppc on ppc.prpid = p.prpid
			    WHERE p.inuid $where AND p.prpstatus = 'A' $where2 AND TO_CHAR(dfi.dfidatasaldo, 'YYYYMM') = TO_CHAR((NOW() - INTERVAL '1 MONTH'), 'YYYYMM')
			    GROUP BY dfi.dfiprocesso
			) as saldomes";
	
	if( trim($_POST['inuid']) == '' ){
		
		$sql = "SELECT acoes, processo, tipo, saldo
				FROM par.vm_temporaria_detalhes_valor_saldo_conta
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();
	
	return $arrDetalhes;
}

function detalhes_financiadas_par(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Obras Financiadas do PAR',
			'cabecalho' => Array('Ações','Preid', 'Obrid', 'Nome da obra', 'Tipo da Obra', 'Funcional Programática', 'Valor empenhado', 'Pagamento solicitado', 'Pagamento efetivado', 'Pagamento total', 'Situação obras 2', 'Restrições obras 2',
								 'Recebeu mobiliário (Apenas Creche)' ),
			'dados'		=> Array()
	);
	
	if( trim($_POST['inuid']) != '' ){
	
		$where = " = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = {$_POST['inuid']})";
			}
	
		if( $_POST['anoprocesso'] ){
			$where .= " and substring(pop.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		}
	}else{
	
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}'";
		}
	}

	$sql = "SELECT 
				'<span style=\"color:#428bca;font-size:12px;cursor:pointer;\" title=\"Ver Pré-Obra\" class=\"preobra glyphicon glyphicon-export\" preid='|| pre.preid ||' tooid='|| pre.tooid ||' > </span>
				<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=\"Ver Obra\" class=\"obra glyphicon glyphicon-export\" obrid='|| o.obrid ||' > </span>' as acoes ,
				'('||pre.preid||')' as preid, '('||pre.obrid||')' as obrid, pre.predescricao, pt.ptodescricao,
				 array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod ||'</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empid in (select empid from par.empenhoobrapar where preid = pre.preid)), '<br>') as funcional, 
               	SUM(de.saldo) AS valorempenhado,  
               	(
                               SELECT sum(popvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobrapar po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
                               AND po.preid = pre.preid
               	) AS pagamentosolicitado, 
               	(
                               SELECT sum(popvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobrapar po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('2 - EFETIVADO')
                               AND po.preid = pre.preid
               	) AS pagamentoefetivado,
               	(
                               SELECT sum(popvalorpagamento)
                               FROM par.pagamento pag
                               INNER JOIN par.pagamentoobrapar po ON po.pagid = pag.pagid
                               WHERE pag.pagstatus = 'A'
                               AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF','2 - EFETIVADO')
                               AND po.preid = pre.preid
               	) AS pagamentototal,
                (SELECT DISTINCT esddsc FROM workflow.estadodocumento esd
				INNER JOIN workflow.documento doc ON doc.esdid = esd.esdid
				WHERE doc.docid = o.docid ) AS situacaoobras2,
				CASE WHEN (SELECT DISTINCT TRUE FROM obras2.restricao WHERE obrid = o.obrid LIMIT 1)
					THEN 'SIM <span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Restrições class=\"restricoes glyphicon glyphicon-download\" obrid='|| o.obrid ||' > </span>'
					ELSE 'NÃO'
				END as restricoes,
               	' - ' AS recebeumobiliario
			FROM par.v_saldo_empenho_por_obra  de
			INNER JOIN par.processoobraspar pop ON pop.pronumeroprocesso = de.processo  
			INNER JOIN obras.preobra pre ON pre.preid = de.preid
			INNER JOIN obras.pretipoobra pt ON pt.ptoid = pre.ptoid
			LEFT  JOIN obras2.obras o ON o.preid = pre.preid AND o.obridpai is null AND o.obrstatus = 'A'
			WHERE pop.inuid $where 
			GROUP BY acoes , pre.preid, pre.obrid, pre.predescricao, pt.ptodescricao, o.docid, o.obrid, pre.tooid
			HAVING SUM(de.saldo) > 0";
	
	if( trim($_POST['inuid']) == '' ){
		
		$sql = "SELECT acoes, preid, obrid, predescricao, ptodescricao, funcional, valorempenhado, pagamentosolicitado, pagamentoefetivado, pagamentototal, situacaoobras2, restricoes, recebeumobiliario
				FROM par.vm_temporaria_detalhes_financiadas_par
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();
	
	return $arrDetalhes;
}

function detalhes_financiadas_pac(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Obras Financiadas do PAC',
			'cabecalho' => Array('Ações', 'Preid', 'Obrid', 'Nome da obra', 'Tipo da Obra', 'Funcional Programática', 'Valor empenhado', 'Pagamento solicitado', 'Pagamento efetivado', 'Pagamento total', 'Situação obras 2', 'Restrições obras 2', 
								 'Recebeu mobiliário (Apenas Creche)' ),
			'dados'		=> Array()
	);
	
	if( trim($_POST['inuid']) != '' ){
		
		$where = " = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = {$_POST['inuid']})";
		}
		
		if( $_POST['anoprocesso'] ){
			$where .= " and substring(pop.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		}
	}else{
		
		if( $_POST['anoprocesso'] ){
			$where .= " and ano = '{$_POST['anoprocesso']}'";
		}
	}
	
	$sql = "SELECT 
				'<span style=\"color:#428bca;font-size:12px;cursor:pointer;\" title=\"Ver Pré-Obra\" class=\"preobra glyphicon glyphicon-export\" preid='|| pre.preid ||' tooid='|| pre.tooid ||' > </span>
				<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=\"Ver Obra\" class=\"obra glyphicon glyphicon-export\" obrid='|| o.obrid ||' > </span>' as acoes ,
				'('||pre.preid||')' as preid, '('||pre.obrid||')' as obrid, pre.predescricao, pt.ptodescricao, 
				array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod|| '</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empid in (select empid from par.empenhoobra where preid = pre.preid)), '<br>') as funcional, 
		        SUM(de.saldo) AS valorempenhado,  
		        (
		                      SELECT sum(pobvalorpagamento)
		                      FROM par.pagamento pag
		                      INNER JOIN par.pagamentoobra po ON po.pagid = pag.pagid
		                      WHERE pag.pagstatus = 'A'
		                      AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
		                      AND po.preid = pre.preid
		       	) AS pagamentosolicitado,
		       	(
		                      SELECT sum(pobvalorpagamento)
		                      FROM par.pagamento pag
		                      INNER JOIN par.pagamentoobra po ON po.pagid = pag.pagid
		                      WHERE pag.pagstatus = 'A'
		                      AND pag.pagsituacaopagamento IN ('2 - EFETIVADO')
		                      AND po.preid = pre.preid
		       	) AS pagamentoefetivado,
		       	(
		                      SELECT sum(pobvalorpagamento)
		                      FROM par.pagamento pag
		                      INNER JOIN par.pagamentoobra  po ON po.pagid = pag.pagid
		                      WHERE pag.pagstatus = 'A'
		                      AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF','2 - EFETIVADO')
		                      AND po.preid = pre.preid
		       	) AS pagamentototal,
                (SELECT DISTINCT esddsc FROM workflow.estadodocumento esd
				INNER JOIN workflow.documento doc ON doc.esdid = esd.esdid
				WHERE doc.docid = o.docid ) AS situacaoobras2,
				CASE WHEN (SELECT DISTINCT TRUE FROM obras2.restricao WHERE obrid = o.obrid LIMIT 1)
					THEN 'SIM <span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Restrições class=\"restricoes glyphicon glyphicon-download\" obrid='|| o.obrid ||' > </span>'
					ELSE 'NÃO'
				END as restricoes,
       			coalesce(            (
                      SELECT DISTINCT 
                      (  CASE WHEN   
                                      (
                                      SELECT CASE WHEN sum(e.saldo) > 0 THEN TRUE ELSE FALSE END
                                      FROM par.subacaoobravinculacao sv
                                      INNER JOIN par.v_saldo_empenho_por_subacao e ON e.sbaid = sv.sbaid AND eobano = sv.sovano 
                                      WHERE sv.preid =  pre2.preid 
                                      GROUP BY sv.preid 
                                      --HAVING sum(e.saldo) > 0
                                      ) = TRUE THEN 'SIM' ELSE 'NÃO' END     
                      ) AS temmobiliario
                      FROM obras.preobra pre2
                      INNER JOIN par.v_saldo_empenho_por_obra e ON e.preid = pre2.preid
                      INNER JOIN par.processoobra p ON p.pronumeroprocesso = e.processo
                      WHERE prestatus = 'A'
                      AND pre2.preid = pre.preid
                      AND prostatus = 'A'
                      AND p.protipo = 'P'
                      GROUP BY pre2.preid
                      HAVING sum(e.saldo) > 0
                      
                      ), 'não se aplica') AS temmobiliario
			FROM par.v_saldo_empenho_por_obra  de
			INNER JOIN par.processoobra 		pop ON pop.pronumeroprocesso = de.processo  
			INNER JOIN par.instrumentounidade	inu ON ( inu.muncod = pop.muncod AND inu.itrid = 2  ) OR (inu.estuf = pop.estuf AND inu.itrid = 1 AND pop.muncod IS NULL )OR (inu.estuf =pop.estuf AND inu.itrid = 2 AND pop.estuf = 'DF')
			INNER JOIN obras.preobra 			pre ON pre.preid = de.preid
			INNER JOIN obras.pretipoobra 		pt ON pt.ptoid = pre.ptoid
			LEFT  JOIN obras2.obras 			o ON o.preid = pre.preid AND o.obridpai is null AND o.obrstatus = 'A'
			WHERE pop.prostatus = 'A' AND inu.inuid $where
			GROUP BY acoes , pre.preid, pre.obrid, pre.predescricao, pt.ptodescricao , protipo, o.docid, o.obrid, pre.tooid
			HAVING SUM(de.saldo) > 0";
	
	if( trim($_POST['inuid']) == '' ){
		
		$sql = "SELECT acoes, preid, obrid, predescricao, ptodescricao, funcional, valorempenhado, pagamentosolicitado, pagamentoefetivado, pagamentototal, situacaoobras2, restricoes, temmobiliario 
				FROM par.vm_temporaria_detalhes_financiadas_pac
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();

	return $arrDetalhes;
}

function detalhes_termos_par(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Termos do PAR',
			'cabecalho' => Array('Ações', 'Processo', 'Nº do termo', 'Tipo de documento', 'Data de vigência', 'Funcional Programática', 'Valor do termo', 'Valor empenhado', 'Pagamento solicitado', 'Pagamento efetivado' ),
			'dados'		=> Array()
	);
	
	$where = " = {$_POST['inuid']}";
	if( $_POST['esfera'] == 'EM' ){
		$where = " IN (	SELECT inu1.inuid
						FROM par.instrumentounidade inu1
						INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
						WHERE inu2.inuid = {$_POST['inuid']})";
	}
	
	if( $_POST['anoprocesso'] ){
		$where1 = " and substring(pro.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
		$where2 = " and substring(pro.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}' ";
	}
	
	$sql = "-- PAR
			SELECT DISTINCT
				'<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Baixar class=\"termo glyphicon glyphicon-download\" title=\"Aderir ao pregão\" dopid='|| dp.dopid ||' > </span>' as acao,
				substring(pro.pronumeroprocesso from 1 for 5)||'.'||
				substring(pro.pronumeroprocesso from 6 for 6)||'/'||
				substring(pro.pronumeroprocesso from 12 for 4)||'-'||
				substring(pro.pronumeroprocesso from 16 for 2) as numeroprocesso,
				'N°: '||dp.dopnumerodocumento::text AS numerotermo,
				dp.mdonome AS tipodocumento,
				(             
				SELECT dopdatafimvigencia from par.documentopar  d
				INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
				WHERE d.proid = pro.proid AND dopstatus <> 'E' AND dpvstatus = 'A' AND mdoid not in (79,65,66,68,76,80,67,73,82)
				ORDER BY d.dopid desc
				LIMIT 1
				) AS datavigencia,
				array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod|| '</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empnumeroprocesso = pro.pronumeroprocesso), '<br>') as funcional,
				dp.dopvalortermo AS valortermo,
				v.saldo AS valorempenhado,
				(
				SELECT sum(pagvalorparcela)
				FROM par.empenho emp 
				INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
				WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.pronumeroprocesso
				) AS pagamentosolicitado,
				(
				SELECT valorpago
				FROM par.vm_valor_pago_por_processo
				WHERE processo = pro.pronumeroprocesso
				) AS pagamentoefetivado
			FROM par.processoobraspar pro 
			LEFT  JOIN par.processoobrasparcomposicao p ON pro.proid = p.proid
			INNER JOIN par.vm_documentopar_ativos dp ON dp.proid = pro.proid
			LEFT  JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
			LEFT  JOIN obras.preobra pre ON pre.preid = p.preid AND pre.prestatus = 'A'
			LEFT  JOIN par.vm_saldo_empenho_do_processo  v ON v.processo = pro.pronumeroprocesso
			WHERE pro.inuid $where AND pro.prostatus = 'A' $where1
			UNION ALL
			-- PAR  GENERICO
			SELECT DISTINCT
				'<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Baixar class=\"termo glyphicon glyphicon-download\" title=\"Aderir ao pregão\" dopid='|| dp.dopid ||' > </span>' as acao,
				substring(pro.prpnumeroprocesso from 1 for 5)||'.'||
				substring(pro.prpnumeroprocesso from 6 for 6)||'/'||
				substring(pro.prpnumeroprocesso from 12 for 4)||'-'||
				substring(pro.prpnumeroprocesso from 16 for 2) as numeroprocesso,
				'N°: '||dp.dopnumerodocumento::text AS numerotermo,
				dp.mdonome AS tipodocumento,
				(              
				SELECT dopdatafimvigencia from par.documentopar  d
				INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
				WHERE d.prpid = pro.prpid AND dopstatus <> 'E' AND dpvstatus = 'A' AND mdoid not in (79,65,66,68,76,80,67,73,82)
				ORDER BY d.dopid desc
				LIMIT 1
				) AS datavigencia,
				array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod|| '</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empnumeroprocesso = pro.prpnumeroprocesso), '<br>') as funcional,
				dp.dopvalortermo AS valortermo,
				v.saldo AS valorempenhado,
				(
				SELECT sum(pagvalorparcela)
				FROM par.empenho emp 
				INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
				WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.prpnumeroprocesso
				) AS pagamentosolicitado,
				(
				SELECT valorpago
				FROM par.vm_valor_pago_por_processo 
				WHERE processo = pro.prpnumeroprocesso
				) AS pagamentoefetivado
			FROM par.processopar pro 
			LEFT  JOIN par.processoparcomposicao p ON pro.prpid = p.prpid
			LEFT  JOIN par.subacaodetalhe sd ON sd.sbdid = p.sbdid
			INNER JOIN par.vm_documentopar_ativos dp ON dp.prpid = pro.prpid
			LEFT  JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
			LEFT  JOIN par.vm_saldo_empenho_do_processo  v ON v.processo = pro.prPnumeroprocesso
			WHERE pro.inuid $where AND pro.prpstatus = 'A' $where2";
	
	if( TRIM($_POST['inuid']) == '' ){
		$where = '';
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}' ";
		}
		$sql = "SELECT acao, processo, numerotermo, tipodocumento, datavigencia, funcional, valortermo, valorempenhado, pagamentosolicitado, pagamentoefetivado
  				FROM par.vm_temporaria_detalhes_termos_par
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();

	return $arrDetalhes;
}

function detalhes_termos_pac(){

	global $db;

	$arrDetalhes = Array(
			'titulo' 	=> 'Termos do PAC',
			'cabecalho' => Array('Ações', 'Processo', 'Nº do termo', 'Tipo de documento', 'Data de vigência', 'Funcional Programática', 'Valor do termo', 'Valor empenhado', 'Pagamento solicitado', 'Pagamento efetivado' ),
			'dados'		=> Array()
	);
	
	$where = " = {$_POST['inuid']}";
	if( $_POST['esfera'] == 'EM' ){
		$where = " IN (	SELECT inu1.inuid
						FROM par.instrumentounidade inu1
						INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
						WHERE inu2.inuid = {$_POST['inuid']})";
	}
	
	if( $_POST['anoprocesso'] ){
		$where .= " and substring(pro.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}

	$sql = "SELECT DISTINCT
				'<span style=\"color:#228B22;font-size:12px;cursor:pointer;\" title=Baixar class=\"termo_pac glyphicon glyphicon-download\" title=\"Aderir ao pregão\" terid='|| tc.terid ||' > </span>' as acao,
				substring(pro.pronumeroprocesso from 1 for 5)||'.'||
				substring(pro.pronumeroprocesso from 6 for 6)||'/'||
				substring(pro.pronumeroprocesso from 12 for 4)||'-'||
				substring(pro.pronumeroprocesso from 16 for 2) as numeroprocesso,
                'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY')  AS numerotermo,
               	'Termo de Compromisso' AS tipodocumento,
               	'-' AS datavigencia,
               	array_to_string(array(SELECT distinct '<span id='||pt.ptres||' class=funcionalprogramatica_detalhe>'||
									    a.esfcod || '.' || 
									    a.unicod || '.' || 
									    a.funcod || '.' || 
									    a.sfucod || '.' || 
									    a.prgcod || '.' || 
									    a.acacod || '.' || 
									    a.loccod|| '</span>' as funcional
									FROM monitora.acao a
									    inner join monitora.ptres pt on pt.acaid = a.acaid
									    left join monitora.planoorcamentario po ON po.acaid = pt.acaid AND po.plocodigo = pt.plocod 
									    inner join monitora.pi_planointernoptres pip ON pt.ptrid = pip.ptrid
									    inner join monitora.pi_planointerno pi ON pip.pliid = pi.pliid 
									    inner join par.empenho e ON e.empcodigoptres = pt.ptres AND empcodigopi =  pi.plicod
									WHERE
										e.empnumeroprocesso = pro.pronumeroprocesso), '<br>') as funcional,
               	( select sum( prevalorobra ) from par.termoobra ter inner join obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A' WHERE ter.terid = tc.terid ) AS valortermo,
               	v.saldo AS valorempenhado,
               	(
               	SELECT sum(pagvalorparcela)
               	FROM par.empenho emp 
               	INNER JOIN  par.pagamento pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento IN ('ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF' )
               	WHERE pag.pagstatus = 'A' AND emp.empnumeroprocesso = pro.pronumeroprocesso
				) AS pagamentosolicitado,
                (
                SELECT valorpago
                FROM par.vm_valor_pago_por_processo 
                WHERE processo = pro.pronumeroprocesso
				) AS pagamentoefetivado
			FROM par.processoobra pro 
			INNER JOIN par.instrumentounidade			inu ON ( inu.muncod = pro.muncod AND inu.itrid = 2 ) OR (inu.estuf = pro.estuf AND inu.itrid = 1 AND pro.muncod IS NULL ) OR (inu.estuf = pro.estuf AND inu.itrid = 2 AND pro.estuf = 'DF')
            INNER JOIN par.termocompromissopac  		tc ON pro.proid = tc.proid and pro.prostatus = 'A'
            LEFT  JOIN par.processoobraspaccomposicao 	p ON pro.proid = p.proid
            LEFT  JOIN obras.preobra 					pre ON pre.preid = p.preid AND pre.prestatus = 'A'
            LEFT  JOIN par.vm_saldo_empenho_do_processo  v ON v.processo = pro.pronumeroprocesso
            WHERE inu.inuid $where AND pro.prostatus = 'A' AND tc.terstatus = 'A'";
	
	if( TRIM($_POST['inuid']) == '' ){
		$where = '';
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}' ";
		}
		$sql = "SELECT acao, processo, numerotermo, tipodocumento, datavigencia, funcional, valortermo, valorempenhado, pagamentosolicitado, pagamentoefetivado
  				FROM par.vm_temporaria_detalhes_termos_pac
				WHERE 1=1 $where";
	}
	
	$arrDetalhes['quantidade'] 	= $db->pegaUm( "SELECT COUNT(*) FROM ($sql) as conta" );
	
	$arrDetalhes['dados'] = $db->carregar( $sql." LIMIT {$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']} OFFSET ".($_POST['pagina'] > 0 ? $_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina']*($_POST['pagina']-1) : 0) );
	$arrDetalhes['dados'] = is_array($arrDetalhes['dados']) ? $arrDetalhes['dados'] : Array();
	
	return $arrDetalhes;
}

function excell(){
	
	global $db;
	
	$_POST = $_REQUEST;
	
	$arrDados = $_POST['tipo']();
	
	header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
	header("Pragma: no-cache");
	header("Content-type: application/xls; name=SIMEC_RelatDocente" . date("Ymdhis") . ".xls");
	header("Content-Disposition: attachment; filename=SIMEC_ValidacaoExecucaoFinanceira" . date("Ymdhis") . ".xls");
	$cabecalho = Array( 'UF', 'Município', 'Esfera', 'N° Termo', 'N° Processo', 'Contrato', 'Nota' );
	$db->monta_lista_tabulado($arrDados['dados'], $arrDados['cabecalho'], 1000000, 5, 'N', '100%');
}

function atualizaDetalhe(){

	global $db;
	
	$_SESSION['paginacaoAjax']['atualizaDetalhe']['container'] 	= 'td_detalhe';
	$_SESSION['paginacaoAjax']['atualizaDetalhe']['por_pagina'] = 50;
	
	$arrDados 	= $_POST['tipo']();
	
	$_SESSION['paginacaoAjax']['atualizaDetalhe']['quantidade'] = $arrDados['quantidade'];
	?>
	<script type="text/javascript">
	$(document).ready(function(){
		$('.repasse_prog_par tr td:nth-child(4)').css('height', Math.max.apply(null, $('.repasse_prog_par tr td:nth-child(4)').map(function ()
			{
			    return $(this).toArray()[0].offsetHeight;
			}).get()));

		$('.tdlistaTitulo').css('height', '25px');
		$('.repasse_prog_par tr td').css('vertical-align', 'middle');
	});
	</script>	
	<table border="0" align="left" width="99%" cellspacing="0" cellpadding="0" class="quadros tabela_painel repasse_prog_par" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px; margin-left: 11px;  ">
		<tr>
			<td colspan="<?=count($arrDados['cabecalho']) ?>">	
				<div style="background-color: #FFFFFF; opacity:0.20; height: 30px; position: relative;"  ></div>
				<div style="position: relative; top: -22px; font-weight: bold; font-size: 13px; height:0px;">
					<?=$arrDados['titulo'] ?> - <?=pegaNomeInstrumentoUnidade( $_POST['inuid'] ) ?>
				</div>
				<img src="../../imagens/excel2013.png" class="excell" funcao="<?=$_POST['tipo'] ?>" style="float: middle; position: relative; margin-top: -25px; margin-left: 550px; cursor: pointer;" width="20px;">
			</td>
		</tr> 
		<tr >
<?php 
	foreach( $arrDados['cabecalho'] as $cabecalho ){ ?>
			<td class="tdlistaTitulo" valign="middle"><?=$cabecalho ?></td>
<?php 
	} ?>
		</tr>
<?php 
	foreach( $arrDados['dados'] as $k => $linha ){

		$resto = $k%2; ?>
		<tr>
<?php 
		foreach( $linha as $coluna ){ 
			
			$coluna = is_numeric($coluna) ? simec_number_format($coluna, 2, ',', '.') : $coluna;
?>
			<td class="tdlista<?=$resto ?>" ><?=$coluna ?></td>
<?php  } ?>
		</tr>
<?php 
	} ?>
		<tr>
			<td colspan="<?=count($arrDados['cabecalho']) ?>" class="tdlistaTitulo" style="text-align:right;padding: 3px;" >
			<? paginacaoAjax(); ?>
			Total de Registros: <?=count($arrDados['dados']) ?> de <?=$arrDados['quantidade'] ?>	
			</td>
		</tr> 
</table> 
<?php 
}

function atualizarGrafico1(){

	global $db;

	switch($_POST['descricao']){
		case 'programa':
			$descricao = "sbd.sbdano as descricao";
			$agrupador = "sbd.sbdano";
			break;
		default:
			$descricao = "pro.prgdsc as descricao";
			$agrupador = "pro.prgdsc";
			break;
	}

	switch($_POST['valor']){
		case 'empenho':
			$valor = "coalesce(sum(ses.saldo),0) as valor";
			break;
		default:
			$valor = "coalesce(sum(pas.pobvalorpagamento),0) as valor";
			break;
	}

	if( TRIM($_POST['inuid']) != '' ){
		
		$where = " = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = {$_POST['inuid']})";
		}
		
		if( $_POST['anoprocesso'] ){
			$where .= " and substring(p.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		}
		
		if( $_POST['numeroprocesso'] ){
			$where .= " AND p.prpnumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		}
		
		if( $_POST['dopnumerodocumento'] ){
			$where .= "p.prpid IN ( SELECT prpid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
		}
	}else{
		
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}'";
		}
	}

	$sql = "SELECT DISTINCT descricao, SUM(valorpago) AS valor
			FROM (
			SELECT  
                            CASE 
                                WHEN pro.prgid in (154, 186) THEN 'Tablet' 
			        WHEN pro.prgid = 104 THEN 'Equipamentos' 
			        WHEN pro.prgid IN (106, 105) THEN 'Climatização' 
			        WHEN pro.prgid = 95 THEN 'Instrumento Musicais' 
			        WHEN pro.prgid in (139,140,204) THEN 'Formacao Continuada Ed. especial' 
			        WHEN pro.prgid = 146 THEN 'BPC na Escola' 
			        WHEN pro.prgid = 206 THEN 'Inclusão e Diversidade' 
			        WHEN pro.prgid in (50,81) THEN 'Caminho da Escola' 
			        WHEN pro.prgid in (147,148) THEN 'Formacao Indígena' 
			        WHEN pro.prgid in (48,76,162,109,165,211) THEN 'Mobiliário' 
			        WHEN pro.prgid = 49 THEN 'Projetor' 
			        WHEN pro.prgid in( 104,103) THEN 'Equipamentos' 
			        WHEN pro.prgid = 145 THEN 'Conferência Infanto Juvenil' 
			        WHEN pro.prgid = 175 THEN 'Brasil Pro - Laboratórios' 
			        WHEN pro.prgid IN (75, 235, 210) THEN 'Equipamentos' 
			        WHEN pro.prgid IN (164, 158, 153) THEN 'Ônibus Escolar Acessível' 
			        WHEN pro.prgid IN (141, 143, 142) THEN 'Educação em Prisões' 
			        WHEN pro.prgid IN (207) THEN 'Educação no Campo' 
			        WHEN pro.prgid IN (53) THEN 'Salas Multifuncionais' 
			        WHEN pro.prgid IN (137, 138) THEN 'Livro Acessível' 
			        WHEN pro.prgid IN (205) THEN 'Formacao EJA' 
			        WHEN pro.prgid IN (27) THEN 'Correção de Fluxo Escolar' 
			        WHEN pro.prgid IN (149) THEN 'Material Ditático Especifico Indigena' 
			        WHEN pro.prgid IN (241) THEN 'Apoio Conselhos Municipais Educação' 
			        WHEN pro.prgid IN (2) THEN 'Outros' 
			        ELSE pro.prgid||' - '||pro.prgdsc  
                END AS descricao,
                pro.prgid,
               	pro.prgdsc,
                sum(pobvalorpagamento) AS valorpago
			FROM par.processopar p
			INNER JOIN par.processoparcomposicao pc ON pc.prpid = p.prpid
			INNER JOIN par.vm_documentopar_ativos d ON d.prpid = p.prpid
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = pc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			LEFT JOIN par.v_saldo_empenho_por_subacao   es ON es.sbaid = s.sbaid 
			LEFT JOIN par.pagamento pag on pag.empid = es.empid AND pag.pagstatus = 'A'
			LEFT JOIN par.pagamentosubacao ps ON pag.pagid = ps.pagid
			INNER JOIN par.programa pro ON pro.prgid = s.prgid
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod AND iu.itrid = 2
			LEFT JOIN territorios.estado e ON e.estuf = iu.estuf AND iu.itrid = 1
			WHERE prpstatus = 'A' AND iu.inuid $where
			GROUP BY p.prpnumeroprocesso, pro.prgid, pro.prgdsc , d.dopnumerodocumento
			HAVING sum(pobvalorpagamento) > 0
			) AS foo
			GROUP BY descricao";

	if( TRIM($_POST['inuid']) == '' ){
		$sql = "SELECT descricao, SUM(valor) as valor 
                FROM par.vm_temporario_atualizarGrafico1 
                WHERE 1=1 $where
				GROUP BY descricao
				ORDER BY descricao";
	}
//	ver($sql, d);
	$arrDados = $db->carregar( $sql );
?>
<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px;">
	<tr>
		<td class="subtitulo">
			<div style="background-color: #FFFFFF; opacity: 0.20; height: 20px; position: relative;"></div>
			<div style="position: relative; top: -17px;"> Repasse por Programas PAR</div>
			<div align="right" style="margin-right: 10px;">
				<input type="button" class="mostraDetalhe" funcao="detalhe_repasse_programas_par" value="Detalhar">
				<input type="hidden" name="detalhe_programa_nome" id="detalhe_programa_nome" value="">
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<?php  
			$grafico1 = new GraficoPAR();
			$grafico1->width = '100%';			
			$extra['onclickArea'] = "registrarSessaoPainel(event.point.name";			
			$grafico1->gerarGrafico( $arrDados, $extra );
			?>
		</td>
	</tr>
</table>
<?php 
}

function atualizarGrafico2(){
	
	global $db;
	
	$grafico = new GraficoPAR();
	$grafico->width = '100%';

	$wherePAR = Array(); 
	$wherePAC = Array(); 
	if( TRIM($_POST['inuid']) != '' ){
		
		$where = " = {$_POST['inuid']}";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = {$_POST['inuid']})";
		}	
		
		if( $where != '' ){
			
			$wherePAR[] = "pop.inuid $where"; 
			$wherePAC[] = "inu.inuid $where"; 
		}
	}
	
	$filtro = '';
	if( in_array($_POST['filtro'], Array('PAR', 'PAC') ) ){
		$grafico->setFormatoPieLabel(' {y} / {point.percentage:.2f}%</b>');

		if( $_POST['filtro'] == 'PAR' ){

			$wherePAR[] = "true = ( SELECT DISTINCT CASE WHEN saldo > 0 THEN TRUE ELSE false END FROM par.v_saldo_empenho_por_obra  WHERE preid = pre.preid AND saldo > 0 )";
			if( $_POST['anoprocesso'] ){
				$wherePAR[] = " substring(pop.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
			}
	
			if( $_POST['numeroprocesso'] ){
				$wherePAR[] = " pop.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
			}
			
			if( $_POST['dopnumerodocumento'] ){
				$wherePAR[] = "pop.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
			}
			
			$sql = "SELECT distinct pt.ptodescricao as descricao, count(po.preid) as valor
					FROM  par.processoobraspar pop -- ON pop.pronumeroprocesso = de.processo  
					INNER JOIN par.processoobrasparcomposicao 	po  ON po.proid = pop.proid
					INNER JOIN obras.preobra 					pre ON pre.preid = po.preid and pre.prestatus = 'A'
					INNER JOIN obras.pretipoobra   				pt  ON pt.ptoid = pre.ptoid
					WHERE ".implode(" AND ", $wherePAR)." 
					GROUP BY pre.ptoid, pt.ptodescricao";
		}

		if( $_POST['filtro'] == 'PAC' ){

			$wherePAC[] = "true = ( SELECT DISTINCT CASE WHEN saldo > 0 THEN true ELSE false END FROM par.v_saldo_empenho_por_obra WHERE preid = pre.preid AND saldo > 0 )";
			$wherePAC[] = "po.prostatus = 'A'";
			if( $_POST['anoprocesso'] ){
				$wherePAC[] = " substring(po.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
			}
	
			if( $_POST['numeroprocesso'] ){
				$wherePAC[] = " po.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
			}
			
			if( $_POST['dopnumerodocumento'] ){
				$wherePAC[] = "po.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
			}
			
			$sql = "SELECT DISTINCT 
						CASE 
							WHEN pt.ptoclassificacaoobra = 'Q' then 'Quadra'
					        when pt.ptoclassificacaoobra = 'C' then 'Cobertura'   
					        when pt.ptoclassificacaoobra = 'P' then 'Creche'          
						end as descricao, count(pre.preid) as valor
					FROM par.processoobra                     po 
					INNER JOIN par.processoobraspaccomposicao  	ppc ON ppc.proid = po.proid 
					INNER JOIN obras.preobra 					pre ON pre.preid = ppc.preid and pre.prestatus = 'A'
					INNER JOIN obras.pretipoobra   				pt  ON pt.ptoid = pre.ptoid
					INNER JOIN par.instrumentounidade 			inu ON ( inu.muncod = po.muncod AND inu.itrid = 2 ) OR (inu.estuf = po.estuf AND inu.itrid = 1 AND po.muncod IS NULL ) OR (inu.estuf =po.estuf AND inu.itrid = 2 AND po.estuf = 'DF')
					WHERE ".implode(" AND ", $wherePAC)." 
					GROUP BY pt.ptoclassificacaoobra";
		}
		
		$btnVoltar = $_POST['inuid'];
		$_SESSION['par']['painel']['filtro2'][count($_SESSION['par']['painel']['filtro2'])] = Array( 'campo' => 'origem', 'valor' =>$_POST['filtro'] );
		$funcao = "atualizarFiltroGrafico( 2, 'ptodescricao' ";
	}else{

		$wherePAC[] = "pop.prostatus = 'A'";
		$wherePAR[] = "p.pagstatus = 'A'";
		$wherePAR[] = "p.pagsituacaopagamento = '2 - EFETIVADO'";
		if( $_POST['anoprocesso'] ){
			$wherePAC[] = "substring(pop.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
			$wherePAR[] = "substring(pop.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		}
	
		if( $_POST['numeroprocesso'] ){
			$wherePAC[] = "pop.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
			$wherePAR[] = "pop.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		}
		
		if( $_POST['dopnumerodocumento'] ){
			$wherePAC[] = "pop.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
			$wherePAR[] = "pop.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
		}
		
		$sql = "SELECT * FROM (
				-- repasse por obras PAC
				SELECT 'PAC' AS descricao, coalesce(sum(po.pobvalorpagamento),0) as valor 
				FROM par.pagamento p
				INNER JOIN par.pagamentoobra 		po  ON p.pagid = po.pagid 
				INNER JOIN par.empenho 				e   ON e.empid = p.empid AND e.empstatus = 'A'
				INNER JOIN par.processoobra 		pop ON pop.pronumeroprocesso = e.empnumeroprocesso
				INNER JOIN par.instrumentounidade 	inu ON ( inu.muncod = pop.muncod AND inu.itrid = 2 AND pop.estuf IS NULL ) OR (inu.estuf = pop.estuf AND inu.itrid = 1 AND pop.muncod IS NULL ) OR (inu.estuf = pop.estuf AND inu.itrid = 2 AND pop.estuf = 'DF')
				WHERE ".implode(" AND ", $wherePAC)."
				UNION ALL				
				-- repasse por obras PAR
				SELECT 'PAR' AS descricao, coalesce(sum(po.popvalorpagamento),0) as valor
				FROM par.pagamento p
				INNER JOIN par.pagamentoobrapar po  ON p.pagid = po.pagid 
				INNER JOIN par.empenho 			e   ON e.empid = p.empid AND e.empstatus = 'A'
				INNER JOIN par.processoobraspar pop ON pop.pronumeroprocesso = e.empnumeroprocesso
				WHERE ".implode(" AND ", $wherePAR)."
				) as foo";
		
		unset($_SESSION['par']['painel']['filtro2']);
	}

	$arrDados = $db->carregar( $sql );
?>
<table border="0" align="left" width="98%" cellspacing="0" cellpadding="0" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px; margin-left: 11px; float:right;">
	<tr>
		<td class="subtitulo">
			<div style="background-color: #FFFFFF; opacity: 0.20; height: 20px; position: relative;"></div>
			<div style="position: relative; top: -17px;">Repasse de Obras</div>
			<div align="right" style="width:15%; float:left; position: relative">
				<?php if( $_POST['filtro'] != '' ){ ?>
				<input type="button" value="Voltar" onclick="atualizarGrafico2(<?=$btnVoltar ?>)" >
				<?php }else{?>
				&nbsp;
				<?php }?>
			</div>
			<div style="position: relative; top: px; width:69%; text-align:center; float: left">&nbsp;
				<input type="hidden" id="filtro2" value="<?=json_encode($_SESSION['par']['painel']['filtro']) ?>" />
				<?=$_POST['filtro'] ?>
			</div>
			<div align="right" style="margin-right: 10px; width:14%; float:left; position: relative">
				<input type="button" value="Detalhar" funcao="detalhe_repasse_obras" class="mostraDetalhe">
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<?php  
			if( $_POST['filtro'] == '' ){
				$extra['onclickArea'] = "atualizarGrafico2( '{$_POST['inuid']}'";
			}else{
				$extra['onclickArea'] = $funcao;
			}
			$grafico->gerarGrafico($arrDados, $extra);
			?>
		</td>
	</tr>
</table>
<?php 
}

function atualizarGrafico3(){
	
	global $db;
	
	$grafico = new GraficoPAR();
	$grafico->width = '100%';

	$where = Array('1=1');
	if( $_POST['anoprocesso'] ){
		$where[] = "eno = '{$_POST['anoprocesso']}'";
	}
	
	$sql = "SELECT estuf, SUM(valorpactuado) as vlr_pactuado, SUM(valorempenhado) as vlr_empenhado, SUM(valorpago) as vlr_pago
			FROM par.vm_temporaria_grafico_por_estado 
			WHERE ".implode(" AND ", $where)."
			GROUP BY estuf
			ORDER BY estuf";
	
	$arrDados = $db->carregar( $sql );
	
?>
<table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px; margin-left: 11px; float:right;">
	<tr>
		<td class="subtitulo">
			<div style="background-color: #FFFFFF; opacity: 0.20; height: 20px; position: relative;"></div>
			<div style="position: relative; top: -17px;">Grafico Financeiro por Estado</div>
		</td>
	</tr>
	<tr>
		<td>
			<?php  
			$extra['tipo'] = '3barras';
			$extra['xAxis'] = 'estuf';
			$extra['yAxis'] = Array(
									Array('campo' => 'vlr_empenhado','descricao' => 'Valor Empenho'), 
									Array('campo' => 'vlr_pactuado','descricao' => 'Valor Pactuado') , 
									Array('campo' => 'vlr_pago','descricao' => 'Valor Pago')
								);
			$grafico->gerarGrafico($arrDados, $extra);
			?>
		</td>
	</tr>
</table>
<?php 
}

function pega_valor_pactuado( $inuid ){ 

	global $db;

	if( trim($_POST['inuid']) != '' ){
		
		$where = " = $inuid";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = $inuid)";
		}
		
		if( $_POST['anoprocesso'] ){
			$where1 = " AND substring(pro.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
			$where2 = " AND substring(pro.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
			$where3 = " AND substring(pro.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		}
		
		if( $_POST['numeroprocesso'] ){
			$where1 .= " AND pro.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
			$where2 .= " AND pro.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
			$where3 .= " AND pro.prpnumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		}
		
		if( $_POST['dopnumerodocumento'] ){
			$where1 .= "pro.proid IN ( SELECT proid FROM par.termocompromissopac WHERE par.retornanumerotermopac(proid) = '{$_POST['numeroprocesso']}' )";
			$where2 .= "pro.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
			$where3 .= "pro.prpid IN ( SELECT prpid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
		}
	}else{
		
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}'";
		}
	}
	
	$sql = "SELECT SUM(valor) AS valorpactuado
			FROM (
				-- PAC
				SELECT SUM ( pre.prevalorobra ) AS valor
				FROM par.processoobra pro 
				INNER JOIN par.processoobraspaccomposicao 	p   ON pro.proid = p.proid
				INNER JOIN obras.preobra 					pre ON pre.preid = p.preid AND pre.prestatus = 'A'
				INNER JOIN par.instrumentounidade 			inu ON ( inu.muncod = pro.muncod AND inu.itrid = 2 ) OR (inu.estuf = pro.estuf AND inu.itrid = 1 AND pro.muncod IS NULL ) OR (inu.estuf =pro.estuf AND inu.itrid = 2 AND pro.estuf = 'DF')
				WHERE pro.prostatus = 'A' AND inu.inuid $where $where1
				UNION ALL
				-- PAR
				SELECT SUM ( pre.prevalorobra ) AS valor
				FROM par.processoobraspar pro 
				INNER JOIN par.processoobrasparcomposicao 	p   ON pro.proid = p.proid
				INNER JOIN obras.preobra 					pre ON pre.preid = p.preid AND pre.prestatus = 'A'
				WHERE pro.inuid $where AND pro.prostatus = 'A' $where2
				UNION ALL
				-- PAR  GENERICO
				SELECT SUM(par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano )) AS valor
				FROM par.processopar pro 
				INNER JOIN par.processoparcomposicao 	p  ON pro.prpid = p.prpid
				INNER JOIN par.subacaodetalhe 			sd ON sd.sbdid = p.sbdid
				WHERE pro.inuid $where  $where3
				AND pro.prpstatus = 'A'
			) AS foo";

	if( trim($_POST['inuid']) == '' ){
		
		$sql = "SELECT SUM(valorpactuado) as valorpactuado FROM par.vm_temporario_pega_valor_pactuado WHERE 1=1 $where";
	}
	
	$valor_pactuado = $db->pegaUm( $sql );

	return $valor_pactuado ? 'R$ '.simec_number_format($valor_pactuado, 2, ',', '.') : 'R$ 0,00';
}

function pega_valor_empenhado( $inuid ){

	global $db;

	$where = '';
	if( trim($inuid) != '' ){
		
		$where = " = $inuid";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = $inuid)";
		}
	}
	
	$where1 = Array("po.prostatus = 'A'");
	$where2 = Array("po.prostatus = 'A'", "proid IN ( SELECT DISTINCT proid FROM par.processoobrasparcomposicao  )");
	$where3 = Array("1=1");
	
	if( $where != '' ){
		
		$where1[] = "inu.inuid $where";
		$where2[] = "po.inuid $where";
		$where3[] = "pp.inuid $where";
	}
	
	if( $_POST['anoprocesso'] ){
		$where1[] = "substring(po.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		$where2[] = "substring(po.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		$where3[] = "substring(pp.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	if( $_POST['numeroprocesso'] ){
		$where1[] = " po.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		$where2[] = " po.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		$where3[] = " pp.prpnumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
	}
	
	if( $_POST['dopnumerodocumento'] ){
		$where1[] = "po.proid IN ( SELECT proid FROM par.termocompromissopac WHERE par.retornanumerotermopac(proid) = '{$_POST['numeroprocesso']}' )";
		$where2[] = "po.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
		$where3[] = "pp.prpid IN ( SELECT prpid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
	}

	$sql = "SELECT  SUM(valorempenhado) AS valorempenho
			FROM (
				-- processo do pac
				SELECT  po.pronumeroprocesso  as proce, SUM(de.saldo) AS valorempenhado
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processoobra 		po ON po.pronumeroprocesso = de.processo
				INNER JOIN par.instrumentounidade 	inu ON ( inu.muncod = po.muncod AND inu.itrid = 2 AND po.estuf IS NULL ) OR (inu.estuf = po.estuf AND inu.itrid = 1 AND po.muncod IS NULL ) OR (inu.estuf =po.estuf AND inu.itrid = 2 AND po.estuf = 'DF')
				WHERE ".implode(" AND ", $where1)."
				GROUP BY po.pronumeroprocesso
				UNION ALL 
				-- processo obras do par
				SELECT  po.pronumeroprocesso  as proce, SUM(de.saldo) AS valorempenhado
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processoobraspar po ON po.pronumeroprocesso = de.processo  
				WHERE ".implode(" AND ", $where2)."
				GROUP BY po.pronumeroprocesso
				UNION ALL 
				-- processo do par
				SELECT pp.prpnumeroprocesso as proce, SUM(de.saldo) AS valorempenhado
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processopar pp ON pp.prpnumeroprocesso = de.processo 
				WHERE ".implode(" AND ", $where3)."
				GROUP BY pp.prpnumeroprocesso
			) AS FOO";

	$valor_empenhado = $db->pegaUm( $sql );

	return $valor_empenhado ? 'R$ '.simec_number_format($valor_empenhado, 2, ',', '.') : 'R$ 0,00';
}

function pega_valor_repassado( $inuid ){ 
	
	global $db;

	if( trim($inuid) != '' ){
		
		$where = " = $inuid";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = $inuid)";
		}
	}
	
	$where1 = Array("po.prostatus = 'A'");
	$where2 = Array("po.prostatus = 'A'", "po.proid in ( SELECT DISTINCT proid FROM par.processoobrasparcomposicao  )");
	$where3 = Array("1=1");
	
	if( $where != '' ){
		$where1[] = "inu.inuid $where";
		$where2[] = "po.inuid $where";
		$where3[] = "pp.inuid $where";
	}
	
	if( $_POST['anoprocesso'] ){
		$where1[] = "substring(po.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		$where2[] = "substring(po.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		$where3[] = "substring(pp.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	if( $_POST['numeroprocesso'] ){
		$where1[] = "po.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		$where2[] = "po.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		$where3[] = "pp.prpnumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
	}
	
	if( $_POST['dopnumerodocumento'] ){
		$where1[] = "po.proid IN ( SELECT proid FROM par.termocompromissopac WHERE par.retornanumerotermopac(proid) = '{$_POST['numeroprocesso']}' )";
		$where2[] = "po.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
		$where3[] = "pp.prpid IN ( SELECT prpid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
	}
	
	$sql = "SELECT SUM(valorpago) AS valorpago
			FROM (
				-- processo do pac
				SELECT SUM(pa.pagvalorparcela) AS valorpago
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processoobra 		po  ON po.pronumeroprocesso = de.processo
				INNER JOIN par.instrumentounidade 	inu ON ( inu.muncod = po.muncod AND inu.itrid = 2 AND po.estuf IS NULL ) OR (inu.estuf = po.estuf AND inu.itrid = 1 AND po.muncod IS NULL ) OR (inu.estuf =po.estuf AND inu.itrid = 2 AND po.estuf = 'DF')
				INNER JOIN par.pagamento 			pa 	ON pa.empid = de.empid AND pa.pagstatus = 'A' AND pa.pagsituacaopagamento = '2 - EFETIVADO'
				WHERE ".implode(" AND ", $where1)."
				UNION ALL 
				-- processo obras do par
				SELECT SUM(pa.pagvalorparcela) AS valorpago
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processoobraspar po ON po.pronumeroprocesso = de.processo 
				INNER JOIN par.pagamento pa ON pa.empid = de.empid AND pa.pagstatus = 'A' AND pa.pagsituacaopagamento = '2 - EFETIVADO' 
				WHERE ".implode(" AND ", $where2)."
				GROUP BY pronumeroprocesso
				UNION ALL 
				-- processo do par
				SELECT  SUM(pa.pagvalorparcela) AS valorpago
				FROM par.v_saldo_por_empenho    de
				INNER JOIN par.processopar pp ON pp.prpnumeroprocesso = de.processo 
				INNER JOIN par.pagamento pa ON pa.empid = de.empid AND pa.pagstatus = 'A' AND pa.pagsituacaopagamento = '2 - EFETIVADO'
				WHERE ".implode(" AND ", $where3)."
			) AS FOO";

	$valor_repassado = $db->pegaUm( $sql );

	return $valor_repassado ? 'R$ '.simec_number_format($valor_repassado, 2, ',', '.') : 'R$ 0,00';
}

function pega_valor_saldo_conta( $inuid ){ 

	global $db; 

	$where = " = $inuid";
	if( $_POST['esfera'] == 'EM' ){
		$where = " IN (	SELECT inu1.inuid
						FROM par.instrumentounidade inu1
						INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
						WHERE inu2.inuid = $inuid)";
	}
	
	if( $_POST['anoprocesso'] ){
		$where1 = " and substring(p.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		$where2 = " and substring(p.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	if( $_POST['numeroprocesso'] ){
		$where1 .= " AND p.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		$where2 .= " AND p.prpnumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
	}
	
	if( $_POST['dopnumerodocumento'] ){
		$where1 .= "p.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
		$where2 .= "p.prpid IN ( SELECT prpid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
	}

	$sql = "-- SALDO EM CONTA
			SELECT COALESCE(SUM(saldo)::text, 'Não Informado') as saldo 
			FROM
			(
			    -- OBRAS PAR
				SELECT  DISTINCT p.pronumeroprocesso, par.retornasaldoprocesso(p.pronumeroprocesso)::numeric AS saldo
				FROM par.processoobraspar 					p    
				INNER JOIN par.processoobrasparcomposicao 	popc ON popc.proid = p.proid
				WHERE p.inuid $where AND p.prostatus = 'A' $where1
			UNION ALL
				-- OBRAS PAC
				SELECT DISTINCT p.pronumeroprocesso, par.retornasaldoprocesso(p.pronumeroprocesso)::numeric AS saldo
				FROM par.processoobra 						p
				INNER JOIN par.processoobraspaccomposicao 	popc ON popc.proid = p.proid
				INNER JOIN par.instrumentounidade 			inu  ON ( inu.muncod = p.muncod AND inu.itrid = 2 AND p.estuf IS NULL ) 
																	OR (inu.estuf = p.estuf AND inu.itrid = 1 AND p.muncod IS NULL ) 
																	OR (inu.estuf =p.estuf AND inu.itrid = 2 AND p.estuf = 'DF')
				WHERE inu.inuid $where AND p.prostatus = 'A' $where1
			UNION ALL
				-- PAR GENERICO
				SELECT DISTINCT p.prpnumeroprocesso, par.retornasaldoprocesso(p.prpnumeroprocesso)::numeric AS saldo
				FROM par.processopar 					p
				INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = p.prpid
				WHERE p.inuid $where AND p.prpstatus = 'A' $where2
			) as saldomes";
	
	if( TRIM($inuid) == '' ){
		$sql = "SELECT saldo FROM par.vm_temporario_pega_valor_saldo_conta;";
	}
	
	$valor_saldo_conta = $db->pegaUm( $sql );
	
	return $valor_saldo_conta ? 'R$ '.simec_number_format($valor_saldo_conta, 2, ',', '.') : 'R$ 0,00';
}

function pega_qtd_financiadas_par( $inuid ){ 

	global $db;

	$where = " = $inuid";
	if( $_POST['esfera'] == 'EM' ){
		$where = " IN (	SELECT inu1.inuid
						FROM par.instrumentounidade inu1
						INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
						WHERE inu2.inuid = $inuid)";
	}
	
	if( $_POST['anoprocesso'] ){
		$where .= " and substring(pop.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	if( $_POST['numeroprocesso'] ){
		$where .= " AND pop.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
	}
	
	if( $_POST['dopnumerodocumento'] ){
		$where .= "pop.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
	}
	
	$sql = "SELECT count(qtd)
			FROM (
			              SELECT COUNT(de.preid) AS qtd
			              FROM par.v_saldo_empenho_por_obra  de
			              INNER JOIN par.processoobraspar pop ON pop.pronumeroprocesso = de.processo  
			              WHERE pop.inuid $where  
			              GROUP BY de.preid
			              HAVING SUM(de.saldo) > 0
			) AS foo";
	
	if( TRIM($inuid) == '' ){
		$where = '';
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}'";
		}
		$sql = "SELECT SUM(qtd) as qtd FROM par.vm_temporario_pega_qtd_financiadas_par WHERE 1=1 $where;";
	}

	$qtd_financiadas_par = $db->pegaUm( $sql );
	
	return 'PAR: '.($qtd_financiadas_par ? $qtd_financiadas_par : 0); 
}

function pega_qtd_financiadas_pac( $inuid ){ 

	global $db;

	$where = " = $inuid";
	if( $_POST['esfera'] == 'EM' ){
		$where = " IN (	SELECT inu1.inuid
						FROM par.instrumentounidade inu1
						INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
						WHERE inu2.inuid = $inuid)";
	}
	
	if( $_POST['anoprocesso'] ){
		$where .= " and substring(po.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	if( $_POST['numeroprocesso'] ){
		$where .= " AND po.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
	}
	
	if( $_POST['dopnumerodocumento'] ){
		$where .= "po.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
	}
	
	$sql = "-- processo do pac
			SELECT count(qtd)
			FROM (
			        SELECT COUNT(de.preid) AS qtd
			        FROM par.v_saldo_empenho_por_obra de
			        INNER JOIN par.processoobra 		po  ON po.pronumeroprocesso = de.processo
					INNER JOIN par.instrumentounidade	inu ON ( inu.muncod = po.muncod AND inu.itrid = 2 ) OR (inu.estuf = po.estuf AND inu.itrid = 1 AND po.muncod IS NULL ) OR (inu.estuf =po.estuf AND inu.itrid = 2 AND po.estuf = 'DF')
			        WHERE po.prostatus = 'A' AND inu.inuid $where
			    	GROUP BY de.preid
			     	HAVING SUM(de.saldo) > 0
			)
			AS foo";
	
	if( TRIM($inuid) == '' ){
		$where = '';
		if( $_POST['anoprocesso'] ){
			$where = " AND ano = '{$_POST['anoprocesso']}'";
		}
		$sql = "SELECT SUM(qtd) as qtd FROM par.vm_temporario_pega_qtd_financiadas_pac WHERE 1=1 $where;";
	}
	
	$qtd_financiadas_par = $db->pegaUm( $sql );
	
	return 'PAC: '.($qtd_financiadas_par ? $qtd_financiadas_par : 0); 
}

function pega_qtd_termos_par( $inuid ){ 

	global $db;

	$where = " = $inuid";
	if( $_POST['esfera'] == 'EM' ){
		$where = " IN (	SELECT inu1.inuid
						FROM par.instrumentounidade inu1
						INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
						WHERE inu2.inuid = $inuid)";
	}
	
	if( $_POST['anoprocesso'] ){
		$where1 = " and substring(p.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
		$where2 = " and substring(p.prpnumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	if( $_POST['numeroprocesso'] ){
		$where1 .= " AND p.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
		$where2 .= " AND p.prpnumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
	}
	
	if( $_POST['dopnumerodocumento'] ){
		$where1 .= "p.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
		$where2 .= "p.prpid IN ( SELECT prpid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
	}
	
	$sql = "-- QTD DE TERMOS PAR
			SELECT
				SUM(qtd)
			FROM
			(
			SELECT count(d.dopid) as qtd
			FROM par.processopar  p
			INNER JOIN par.vm_documentopar_ativos d ON d.prpid = p.prpid 
			WHERE 
				p.prpstatus = 'A'
				AND p.inuid $where $where2
			UNION ALL
			SELECT count(d.dopid) as qtd 
			FROM par.processoobraspar  p
			INNER JOIN par.vm_documentopar_ativos d ON d.proid = p.proid 
			WHERE 
				p.prostatus = 'A'
				AND p.inuid $where $where1
			) as foo";
	
	if( TRIM($inuid) == '' ){
		$sql = "SELECT
					SUM(qtd) as qtd
				FROM
				(
				SELECT 
					count(d.dopid) as qtd,
					substring(p.prpnumeroprocesso,12,4) as ano
				FROM par.processopar  p
				INNER JOIN par.documentopar d ON d.prpid = p.prpid AND d.dopstatus = 'A'
				WHERE 
					p.prpstatus = 'A' $where2
				GROUP BY p.prpnumeroprocesso
				UNION ALL
				SELECT 
					count(d.dopid) as qtd ,
					substring(p.pronumeroprocesso,12,4) as ano
				FROM par.processoobraspar  p
				INNER JOIN par.documentopar d ON d.proid = p.proid AND d.dopstatus = 'A'
				WHERE 
					p.prostatus = 'A' $where1
				GROUP BY p.pronumeroprocesso
				) as foo";
	}
	
	$qtd_termos_par = $db->pegaUm( $sql );
	
	return "PAR: ".($qtd_termos_par ? $qtd_termos_par : 0);
}

function pega_qtd_termos_pac( $inuid ){

	global $db;

	if( TRIM($inuid) != '' ){
		$where = " = $inuid";
		if( $_POST['esfera'] == 'EM' ){
			$where = " IN (	SELECT inu1.inuid
							FROM par.instrumentounidade inu1
							INNER JOIN par.instrumentounidade inu2 ON inu2.estuf = inu1.mun_estuf
							WHERE inu2.inuid = $inuid)";
		}
		if( $where != '' ){
			$where = " AND inu.inuid $where";
		}
	}
	
	if( $_POST['anoprocesso'] ){
		$where .= " AND substring(p.pronumeroprocesso,12,4) = '{$_POST['anoprocesso']}'";
	}
	
	if( $_POST['numeroprocesso'] ){
		$where .= " AND p.pronumeroprocesso = '".str_replace(Array('.','/','-'), '', $_POST['numeroprocesso'])."'";
	}
	
	if( $_POST['dopnumerodocumento'] ){
		$where .= "p.proid IN ( SELECT proid FROM par.documentopar WHERE dopnumerodocumento = '{$_POST['numeroprocesso']}' OR ( dopid = '{$_POST['numeroprocesso']}' AND dopnumerodocumento IS NULL ) )";
	}
	
	$sql = "-- QTD DE TERMOS PAC
			SELECT count(d.terid) 
			FROM par.processoobra p
			INNER JOIN par.termocompromissopac 	d   ON d.proid = p.proid 
			INNER JOIN par.instrumentounidade 	inu ON ( inu.muncod = p.muncod AND inu.itrid = 2 ) OR (inu.estuf = p.estuf AND inu.itrid = 1 AND p.muncod IS NULL )OR (inu.estuf =p.estuf AND inu.itrid = 2 AND p.estuf = 'DF')
			WHERE p.prostatus = 'A' AND terstatus = 'A' $where";
	
	$qtd_termos_par = $db->pegaUm( $sql );
	
	return "PAC: ".($qtd_termos_par ? $qtd_termos_par : 0);
}

function atualizarTabelas(){

	global $db;

	$retorno = Array();

	$retorno['valor_pactuado'] 		= pega_valor_pactuado( $_POST['inuid'] );
	$retorno['valor_empenhado'] 	= pega_valor_empenhado( $_POST['inuid'] );
	$retorno['valor_repassado'] 	= pega_valor_repassado( $_POST['inuid'] );
	$retorno['valor_saldo_conta'] 	= pega_valor_saldo_conta( $_POST['inuid'] );

	$retorno['qtd_financiadas_par'] = pega_qtd_financiadas_par( $_POST['inuid'] );
	$retorno['qtd_financiadas_pac'] = pega_qtd_financiadas_pac( $_POST['inuid'] );

	$retorno['qtd_termos_par'] 		= pega_qtd_termos_par( $_POST['inuid'] );
	$retorno['qtd_termos_pac'] 		= pega_qtd_termos_pac( $_POST['inuid'] );

	echo simec_json_encode($retorno);
}

function recuperaInuid(){

	global $db;

	if( $_POST['estuf'] == 'DF' ){
		echo '1';
		die();
	}
	
	$where 	= $_POST['muncod'] 	? "muncod = '{$_POST['muncod']}' AND estuf IS NULL" : "estuf = '{$_POST['estuf']}' AND muncod IS NULL";

	$sql = "SELECT inuid FROM par.instrumentounidade WHERE $where";
	
	$inuid = $db->pegaUm( $sql );

	echo $inuid ? $inuid : '';
}

function atualizaMunicipios(){

	global $db;

	echo "Município: <br />";

	if( $_POST['estuf'] != '' ){
		$sql = "SELECT muncod as codigo, mundescricao as descricao
				FROM territorios.municipio
				WHERE estuf = '{$_POST['estuf']}'
				ORDER BY mundescricao ASC";
		$db->monta_combo( "muncod", $sql, 'S', 'Município', '', '','',165, '', 'muncod' );
	}else{
		$sql = "SELECT 0 as codigo, 'Favor escolher uma Unidade Federativa' as descricao";
		$db->monta_combo( "estuf", $sql, 'S', 'Favor escolher uma Unidade Federativa', '', '','',165 );
	}
}

function pegaNomeInstrumentoUnidade( $inuid ){
	
	global $db;
	
	if( trim($inuid) == '' ){
		return 'Brasil';
	}

	$sql = "SELECT
				coalesce(est.estdescricao, mun.mundescricao||' - '||mun.estuf) as descricao
			FROM
				par.instrumentounidade inu
			LEFT JOIN territorios.municipio mun ON mun.muncod = inu.muncod
			LEFT JOIN territorios.estado	est ON est.estuf = inu.estuf
			WHERE
				inuid = '$inuid'";
	if( $inuid ) return $db->pegaUm( $sql ); 
}

function atualizaTitulo(){

	
	echo pegaNomeInstrumentoUnidade( $_POST['inuid'] );
}

function listaRestricoes(){
	
	global $db;
	
	$sql = "SELECT 
				rstdsc,
				rstdtinclusao
				rstdtprevisoaregularizacao,
				rstdtsuperacao,
				rstdscprovidencia,
				usunome,
				rstobsressalva
			FROM obras2.restricao rst
			INNER JOIN seguranca.usuario usu ON usu.usucpf = rst.usucpf
			WHERE
				rst.obrid = {$_POST['obrid']}";
	$cabecalho = Array('Restrição', 'DT criação', 'DT prevista regularização', 'DT superação', 'DT providência', 'Criador', 'Ressalva');
	$db->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2,'formlistaestado',$tamanho,'', '');
}

function graficoSaldoProcesso(){
	
	include_once APPRAIZ."www/obras2/ajax.php";
}

function atualizarFiltroGrafico(){
	
	if($_SESSION['par']['painel']['filtro'.$_POST['numero']][ count($_SESSION['par']['painel']['filtro'.$_POST['numero']])-1 ]['valor'] == $_POST['valor']){
		unset($_SESSION['par']['painel']['filtro'.$_POST['numero']][ count($_SESSION['par']['painel']['filtro'.$_POST['numero']])-1 ]);
	}elseif($_SESSION['par']['painel']['filtro'.$_POST['numero']][ count($_SESSION['par']['painel']['filtro'.$_POST['numero']])-1 ]['campo'] == $_POST['campo']){
		$_SESSION['par']['painel']['filtro'.$_POST['numero']][ count($_SESSION['par']['painel']['filtro'.$_POST['numero']])-1 ]['valor'] = $_POST['valor'];
	}else{
		$_SESSION['par']['painel']['filtro'.$_POST['numero']][ count($_SESSION['par']['painel']['filtro'.$_POST['numero']]) ] = Array('campo'=>$_POST['campo'],'valor'=>$_POST['valor']);
	}
	
	echo json_encode($_SESSION['par']['painel']['filtro'.$_POST['numero']]);
}

?>