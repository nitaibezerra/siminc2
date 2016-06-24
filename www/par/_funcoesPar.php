<?php
function testaProcessoTermoFinalizadoPAR( $dopid ){
	
	global $db;
	
	$sql = "SELECT coalesce(prpnumeroprocesso, pronumeroprocesso) as id 
			FROM par.documentopar dop
			LEFT JOIN par.processoobraspar 	pro ON pro.proid = dop.proid 
			LEFT JOIN par.processopar 	prp ON prp.prpid = dop.prpid
			WHERE dopid = $dopid";
	
	$numeroProcesso = $db->pegaUm( $sql );
	
	$sql = "SELECT DISTINCT finalizado
			FROM
			(
				SELECT profinalizado as finalizado FROM par.processoobraspar WHERE pronumeroprocesso = '$numeroProcesso'
				UNION ALL
				SELECT prpfinalizado as finalizado FROM par.processopar WHERE prpnumeroprocesso = '$numeroProcesso'
			) as foo
			WHERE finalizado IS TRUE";
	
	$finalizado = $db->pegaUm( $sql );
	
	return $finalizado == 't';
}

/* inativaObras2SemSaldoEmpenho
 * aliemnta a tabela de histórico de status de uma obra com a data, o responssável pela alteração, qual o novo status e o poorquede trocar o status.
 * */
function inativaObras2SemSaldoEmpenho( $empid, $preids = Array() ){
	
	global $db;
	# A pedido do Leo do FNDE dia 10_02_2015
	return true;
	
	/* if( $preids[0] != '' ){
		$obras = implode(', ', $preids);
	}else{
		$obras = "	SELECT preid FROM par.empenhoobra WHERE empid = $empid
					UNION ALL
					SELECT preid FROM par.empenhoobrapar WHERE empid = $empid";
	}
	
	$sql = "UPDATE obras2.obras SET obrstatus = 'I' 
			WHERE 
				obrid IN (
					SELECT 
						pre.obrid
					FROM obras.preobra pre
					LEFT  JOIN par.vm_saldo_empenho_por_obra vem ON vem.preid = pre.preid
					WHERE
						pre.preid IN ( $obras )
					GROUP BY
						pre.preid, pre.obrid, vem.saldo
					HAVING
						saldo <= 0 OR saldo IS NULL
				)";

	$db->executar($sql);
	$db->commit(); */
}

/* insereHistoricoStatusObra
 * aliemnta a tabela de histórico de status de uma obra com a data, o responssável pela alteração, qual o novo status e o poorquede trocar o status.
 * */
function insereHistoricoStatusObra( $empid, $preids = Array(), $status, $justificativa ){
	
	global $db;
	
	return true;
	
	/* if( $preids[0] != '' ){
		$obras = implode(', ', $preids);
	}else{
		$obras = "	SELECT preid FROM par.empenhoobra WHERE empid = $empid
					UNION ALL
					SELECT preid FROM par.empenhoobrapar WHERE empid = $empid";
	}
	
	$having = '';
	if( $status == 'I' ){
		$having = "HAVING
					saldo <= 0 OR saldo IS NULL";
	}
	
	$notStatus = $status == 'A' ? "( obrstatus = 'I' OR obrstatus IS NULL )" : "obrstatus = 'A'";
	
	$sql = "INSERT INTO par.historico_status_obras2(hsojustificativa, status, usucpf, empid, preid, obrid)
			SELECT 
				'$justificativa' as hsojustificativa, 
				'$status' as status,
				'{$_SESSION['usucpf']}' as usucpf,
				$empid as empid,
				pre.preid,
				pre.obrid
			FROM obras.preobra pre
			LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid
			LEFT  JOIN par.vm_saldo_empenho_por_obra vem ON vem.preid = pre.preid
			WHERE
				pre.preid IN ( $obras ) AND $notStatus
			GROUP BY
				pre.preid, pre.obrid, vem.saldo
			$having";
// 	ver($sql,d);
	$db->executar($sql);
	$db->commit(); */
}

/* Finaliza reprogramação
 * */
function finalizaReprogramacaoSubacao(){
	
	global $db;
	
	extract($_POST);
	
	$sql = "UPDATE par.subacao SET
				sbareformulacao = false
			WHERE sbaid in (SELECT s.sbaid FROM
				par.processoparcomposicao ppc
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid
			WHERE
				dop.dopid = {$dopid} AND s.sbareformulacao = true AND ppc.ppcstatus = 'A'
				AND sd.sbdid NOT IN (SELECT sbdid FROM par.documentoparreprogramacaosubacao WHERE dpsstatus <> 'I') )";
	$db->executar( $sql );
        
	$sql = "UPDATE par.subacaodetalhe SET
				sbdreformulacao = false
			WHERE sbdid in (
				SELECT sd.sbdid FROM
				par.processoparcomposicao ppc
				INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
				INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid
				WHERE dop.dopid = {$dopid} and sd.sbdreformulacao = true and ppc.ppcstatus = 'A')";
	$db->executar( $sql );
	
	$sql = "SELECT dopidoriginal FROM par.reprogramacao WHERE dopidreprogramado = ".$dopid;
	$dopidOriginal = $db->pegaUm($sql);
	
	$sql = "UPDATE par.documentopar SET dopreformulacao = false WHERE dopid = ".$dopidOriginal;
	$db->executar( $sql );
	
	$sql = "UPDATE par.reprogramacao SET repstatus = 'F', repdtfim = 'NOW()' WHERE dopidreprogramado = ".$dopid;
	$db->executar( $sql );
	
	$db->commit();
	
	if($dopid)
	{
		enviaEmailNovoTermo($dopid, 'reformulacao');
	}
	
	echo 'Reformulação finalizada com sucesso!';
}

/* FIM - Finaliza reprogramação
 * */
/* pegaTermoprocesso
 * retorna o termo ativo do processo
 * */
function pegaTermoprocesso( $numeroprocesso ){
	
	global $db;
	
	$sql = "SELECT proid FROM par.processoobraspar  WHERE pronumeroprocesso = '$numeroprocesso'";
	
	$proid = $db->pegaUm($sql);
	
	if( $proid ){
		
		$sql = "SELECT max(dopid) FROM par.documentopar WHERE proid = $proid AND dopstatus = 'A'";
		
	}else{
		
		$sql = "SELECT prpid FROM par.processopar 		WHERE prpnumeroprocesso = '$numeroprocesso'";
		$prpid = $db->pegaUm($sql);
		
		$sql = "SELECT max(dopid) FROM par.documentopar WHERE prpid = $prpid AND dopstatus = 'A'";
	}
	
	return $db->pegaUm($sql);
}
/* verificaTermoVencidoPAR
 * verifica se o termo do par ou obrasPar está vencido
 * */
function verificaTermoVencidoPAR( $numeroprocesso ){
	
	$dopid = pegaTermoprocesso( $numeroprocesso );

	if( !$dopid ){
		return true;
	}
	
	$dtFim = pegaDataFimDocumentoPar( $dopid );
	$dtFim = explode('/', $dtFim);
	
	return Date('Ym') >  (int) $dtFim[1].$dtFim[0];
}
/* pegaQtdMonitorada
 * pega a quantidade monitorada do item de subação
 * */
function pegaQtdMonitorada( $icoid ){
	
	global $db;
	
	$sql = "SELECT
				icoquantidaderecebida
			FROM
				par.subacaoitenscomposicao
			WHERE
				icoid = $icoid";
	
	return $db->pegaUm($sql);
}

function pegaDataFimDocumentoParObra( $numeroprocesso ){	
	global $db;	
	$dopid = pegaTermoprocesso( $numeroprocesso );
	
	if( !$dopid ){
		return false;
	}
	
	$sql = "SELECT
				coalesce(dop.dopdatafimvigencia, to_char((prp.prodatainclusao + INTERVAL '365 DAY'),'MM/YYYY')) as data
			FROM
				par.documentopar dop
				LEFT JOIN par.processoobraspar 	prp  ON prp.proid = dop.proid and prp.prostatus = 'A'
			WHERE
				dop.dopid = $dopid";
	
	$dtFim = $db->pegaUm( $sql );
	$dtFim = explode('/', $dtFim);
    return (Date('Ym') >  (int) $dtFim[1].$dtFim[0]) ? 'S' : 'N';
}

/* pegaDataFimDocumentoPar
 * pega a data inicial do documento par
 */
function pegaDataFimDocumentoPar( $dopid ){
	
	global $db;
	
	$sql = "SELECT
				coalesce(dop.dopdatafimvigencia, to_char((prp.prpdatainclusao + INTERVAL '365 DAY'),'MM/YYYY')) as data
			FROM
				par.documentopar dop
			LEFT JOIN par.processopar 	prp  ON prp.prpid = dop.prpid and prp.prpstatus = 'A'
			WHERE
				dop.dopid = $dopid AND dop.dopstatus = 'A'";
	
	return $db->pegaUm( $sql );
}

/* pegaDataInicioDocumentoPar
 * pega a data inicial do documento par
 */
function pegaDataInicioDocumentoPar( $dopid ){
	
	global $db;
	
	$sql = "SELECT
				CASE WHEN dop.dopidpai IS NULL
					THEN coalesce(dop.dopdatainiciovigencia, to_char(prp.prpdatainclusao,'MM/YYYY'))
					ELSE coalesce(dop2.dopdatainiciovigencia, to_char(prp2.prpdatainclusao,'MM/YYYY'))
				END as data
			FROM
				par.documentopar dop
			LEFT JOIN par.processopar 	prp  ON prp.prpid = dop.prpid and prp.prpstatus = 'A'
			LEFT JOIN par.documentopar 	dop2 ON dop2.dopid = par.retornadopidoriginal(dop.dopidpai)
			LEFT JOIN par.processopar 	prp2  ON prp2.prpid = dop2.prpid and prp2.prpstatus = 'A'
			WHERE
				dop.dopid = $dopid";
	
	return $db->pegaUm( $sql );
}

/* verificaPendenciaMaiseducacao
 * Função testa se a unidade possui pendências no Mais Educação
 * Deve ser passado um Array() da seguinte maneira:
 * Ex.: Array( 'campo' => 'estuf', 'valor' => 'AC' );
 * */
function verificaPendenciaMaiseducacao( $arr = Array() ){
	
	global $db;
	
	$sql = "SELECT 
				*
			FROM
				dblink(
					'".DBLINK_PARAM_PDEINTERATIVO."',
					'
					SELECT DISTINCT 
					case when ( ( doc.esdid is null ) OR ( doc.esdid = 32 ) ) then TRUE else FALSE end as boo
					FROM pddeinterativo.listapdeinterativo lp
					INNER JOIN pdeescola.pddemepriorizadas as prio on prio.entcodent = lp.pdicodinep 
					INNER JOIN pdeescola.memaiseducacao me on lp.pdicodinep = me.entcodent AND me.memanoreferencia = 2014 and memstatus = ''A''
					LEFT JOIN workflow.documento as doc on doc.docid = me.docid
					WHERE 
						lp.{$arr['campo']} = ''{$arr['valor']}''
						AND ( ( doc.esdid is null ) OR ( doc.esdid = 32 ) )'
				) as rs (
					teste boolean
				)";
// 	ver($sql, d);
	return $db->pegaUm( $sql ) == 't';
}

/* pegaArrEsferaInuid
 * Função pega dados esfera por INUID
 * Se for estadual retorna a no campo $arr['esfera'] e o campo e o valor nos respectivos $arr['campo'] e $arr['valor']
 * Ex.: $arr=>['esfera'] = 'E', ['campo'] = 'estuf', ['valor'] = 'AC';
 * */
function pegaArrEsferaInuid( $inuid ){
	
	global $db;
	
	$sql = "SELECT 
				CASE 
					WHEN itrid = 1 THEN 'E'
					WHEN itrid = 2 THEN 'M'
					WHEN itrid = 3 THEN 'P'
				END as esfera,
				CASE 
					WHEN itrid = 1 THEN 'estuf'
					WHEN itrid = 2 THEN 'muncod'
				END as campo,
				CASE 
					WHEN itrid = 1 THEN estuf
					WHEN itrid = 2 THEN muncod
				END as valor
			FROM 
				par.instrumentounidade 
			WHERE 
				inuid = $inuid";
	
	return $db->pegaLinha($sql);
}

/*
 *
Copia as pontuações, ações e subações do PAR, com todas as suas tabelas relacionadas, colocando o status 'C' de cópia.
*/
function copiarPlanoDeAcoesPar($inuid, $itrid = null){
	global $db;

	$itrid = $itrid ? $itrid : $_SESSION['par']['itrid'];

	// verifico antes se ja tem alguma cópia
	if( deletaCopia( $inuid, $itrid ) ){

		$pontuacao = $db->carregar("SELECT
										p.ptoid
									FROM
										par.dimensao                             d
									INNER JOIN par.area                 a  ON a.dimid  = d.dimid AND arestatus = 'A'
									INNER JOIN par.indicador          i  ON i.areid  = a.areid AND indstatus = 'A'
									INNER JOIN par.criterio             c  ON i.indid  = c.indid AND c.crtstatus = 'A'
									INNER JOIN par.pontuacao        p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
									INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
									WHERE
										d.itrid = ".$itrid." AND p.inuid = ".$inuid );

		if( is_array($pontuacao) ){
			foreach ( $pontuacao as $pontuacao1 ) {
				$idpontuacao = $pontuacao1['ptoid'];
				$sql = "INSERT INTO par.pontuacao (
						crtid, ptodata, usucpf, inuid, ptostatus, ptojustificativa, ptodemandamunicipal, ptodemandaestadual
						) SELECT
							crtid, ptodata, usucpf, inuid, 'C' as ptostatus, ptojustificativa, ptodemandamunicipal, ptodemandaestadual
						FROM
							par.pontuacao
						WHERE
							ptoid = " . $idpontuacao . '
						RETURNING
							ptoid';

				$novoidpontuacao = $db->pegaUm($sql);
				$acao            = $db->carregar('SELECT aciid FROM par.acao WHERE ptoid = ' . $idpontuacao);

				if ( $acao != '' ) {
					foreach ( $acao as $acao1 ):
						$idacao = $acao1['aciid'];
						$sqlInsertAcao = "INSERT INTO par.acao
										( ptoid, ppaid, acidsc, acinomeresponsavel, acicargoresponsavel, acidemandapotencial, aciresultadoesperado, acidata, usucpf, acistatus )
										SELECT
											$novoidpontuacao, ppaid, acidsc, acinomeresponsavel, acicargoresponsavel, acidemandapotencial, aciresultadoesperado, acidata, usucpf, 'C' as acistatus
										FROM
											par.acao
										WHERE
											acistatus = 'A' AND
											aciid = $idacao
										RETURNING
											aciid";
						$novoidacao = $db->pegaUm($sqlInsertAcao);

						$subacao = $db->carregar("SELECT s.*
												FROM
													par.subacao s
												LEFT JOIN par.propostasubacao p ON s.ppsid = p.ppsid
												WHERE
													s.sbastatus = 'A' AND
													s.aciid = ".$idacao);
						if ( $subacao != '' ){
							foreach ( $subacao as $subacao1 ):
								$idsubacao = $subacao1['sbaid'];
								$sqlInsertSubacao = " INSERT INTO par.subacao
		                        						( aciid, sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao,
														sbaptres, sbanaturezadespesa, sbamonitoratecnico, docid, frmid,
														indid, foaid, undid, ppsid, prgid, ptsid, sbacronograma, sbappspeso,
														sbaobjetivo, sbatexto, sbacobertura, usucpf, sbadataalteracao,
														sbastatus )
														SELECT
															$novoidacao, sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao,
															sbaptres, sbanaturezadespesa, sbamonitoratecnico, docid, frmid,
															indid, foaid, undid, ppsid, prgid, ptsid, sbacronograma, sbappspeso,
															sbaobjetivo, sbatexto, sbacobertura, usucpf, sbadataalteracao,
															'C' as sbastatus
														FROM
															par.subacao
														WHERE
															sbastatus = 'A' AND
															sbaid = $idsubacao
														RETURNING
															sbaid";

								$novoidsubacao = $db->pegaUm($sqlInsertSubacao);

								//DETALHE
								$sqlInsertSubacaoDetalhe = "INSERT INTO par.subacaodetalhe
															(sbaid, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
															ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento)
															SELECT
																$novoidsubacao, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
																ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento
															FROM
																par.subacaodetalhe
															WHERE
																sbaid = ".$idsubacao;
								$db->carregar($sqlInsertSubacaoDetalhe);

								//ITENS
								$sqlInsertItens ="INSERT INTO par.subacaoitenscomposicao
													(sbaid, icoano, icoordem, icodescricao, icoquantidade, icovalor,
													icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid )
													SELECT
														$novoidsubacao, icoano, icoordem, icodescricao, icoquantidade, icovalor,
														icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid
													FROM
														par.subacaoitenscomposicao
													WHERE
														sbaid = ".$idsubacao;
								$db->carregar($sqlInsertItens);

								//BENEFICIARIOS
								$sqlBeneficiario="INSERT INTO par.subacaobeneficiario
													( sbaid, benid, sabqtdurbano, sabqtdrural, sabano )
													SELECT
														$novoidsubacao, benid, sabqtdurbano, sabqtdrural, sabano
													FROM
														par.subacaobeneficiario
													WHERE
														sbaid = ".$idsubacao;
								$db->carregar($sqlBeneficiario);

								//ESCOLAS
								$sqlEscolasAntigas = "SELECT
														sesid
													FROM
														par.subacaoescolas
													WHERE
														sbaid=".$idsubacao;
								$arrSesid = $db->carregar( $sqlEscolasAntigas );

								if( is_array( $arrSesid ) ){
									foreach( $arrSesid as $sesid ){
										$sqlEscolas = "INSERT INTO par.subacaoescolas
														( sbaid, sesano, escid, sesquantidade, sesstatus )
														SELECT
															$novoidsubacao, sesano, escid, sesquantidade, sesstatus
														FROM
															par.subacaoescolas
														WHERE
															sesid = {$sesid['sesid']}
														RETURNING
															sesid";
										$sesidNovo = $db->pegaUm($sqlEscolas);

										//ITENS POR ESCOLA
										$sqlSubEsc = "INSERT INTO par.subescolas_subitenscomposicao
													( sesid, icoid, seiqtd )
													SELECT
														$sesidNovo, icoid, seiqtd
													FROM
														par.subescolas_subitenscomposicao
													WHERE
														sesid = ".$sesid['sesid'];

										$db->carregar($sqlSubEsc);
									}
								}

								//OBRAS
								$sqlObras = "INSERT INTO par.subacaoobra
											( sbaid, preid, sobano )
											SELECT
												$novoidsubacao, preid, sobano
											FROM
												par.subacaoobra
											WHERE
												sbaid = ".$idsubacao;
								$db->carregar($sqlObras);

							endforeach;
						}
					endforeach;
				}
			}
		}
		//$db->commit(); // Só vai dar o commit no final de todas as operações!
		return true;
	} else {
		return false;
	}
}

// Função que deleta a cópia do PAR
function deletaCopia( $inuid, $itrid = null ){
	global $db;

	$itrid = $itrid ? $itrid : $_SESSION['par']['itrid'];

	$pontuacaoCopia = $db->carregar("SELECT
										p.ptoid
									FROM
										par.dimensao                             d
									INNER JOIN par.area                 a  ON a.dimid  = d.dimid AND arestatus = 'A'
									INNER JOIN par.indicador          i  ON i.areid  = a.areid AND indstatus = 'A'
									INNER JOIN par.criterio             c  ON i.indid  = c.indid AND c.crtstatus = 'A'
									INNER JOIN par.pontuacao        p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
									INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
									WHERE
										d.itrid = ".$itrid." AND p.ptostatus = 'C' AND p.inuid = ".$inuid );
	if( is_array($pontuacaoCopia) ){
		foreach( $pontuacaoCopia as $ptcopia){
			$acaoCopia = $db->carregar("SELECT aciid FROM par.acao WHERE ptoid = ".$ptcopia['ptoid']);
			if( is_array($acaoCopia) ){
				foreach( $acaoCopia as $accopia){
					$subacaoCopia = $db->carregar("SELECT sbaid FROM par.subacao WHERE aciid = ".$accopia['aciid']);
					if( is_array($subacaoCopia) ){
						foreach( $subacaoCopia as $sbcopia){
							$sql = "";
							$sql .= "DELETE FROM par.subacaoobra WHERE sbaid = ".$sbcopia['sbaid']."; ";

							$sqlEscolas = "SELECT
													sesid
												FROM
													par.subacaoescolas
												WHERE
													sbaid=".$sbcopia['sbaid'];
							$arrSesid = $db->carregarColuna( $sqlEscolas );

							if( is_array( $arrSesid ) ){
								foreach( $arrSesid as $sesid ){
									$sql .= "DELETE FROM par.subescolas_subitenscomposicao WHERE sesid = ".$sesid."; ";
								}
							}

							$sql .= "DELETE FROM par.subacaoescolas WHERE sbaid = ".$sbcopia['sbaid']."; ";
							$sql .= "DELETE FROM par.subacaobeneficiario WHERE sbaid = ".$sbcopia['sbaid']."; ";
							$sql .= "DELETE FROM par.subacaoitenscomposicao WHERE sbaid = ".$sbcopia['sbaid']."; ";
							$sql .= "DELETE FROM par.subacaodetalhe WHERE sbaid = ".$sbcopia['sbaid']."; ";
							$sql .= "DELETE FROM par.subacao WHERE sbaid = ".$sbcopia['sbaid']."; ";
							$db->executar( $sql );
						}
					}
					$sql = "DELETE FROM par.acao WHERE aciid = ".$accopia['aciid'];
					$db->executar( $sql );
				}
			}
			$sql = "DELETE FROM par.pontuacao WHERE ptoid = ".$ptcopia['ptoid'];
			$db->executar( $sql );
		}
		if( $db->commit() ){
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
}

function alteraStatusObrasSubacao( $inuid, $oPreObra = null, $oSubacaoControle = null, $itrid = null ){

	global $db;

	$itrid = $itrid ? $itrid : $_SESSION['par']['itrid'];

	$pontuacao = $db->carregar("SELECT
										p.ptoid
									FROM
										par.dimensao                             d
									INNER JOIN par.area                 a  ON a.dimid  = d.dimid AND arestatus = 'A'
									INNER JOIN par.indicador          i  ON i.areid  = a.areid AND indstatus = 'A'
									INNER JOIN par.criterio             c  ON i.indid  = c.indid AND c.crtstatus = 'A'
									INNER JOIN par.pontuacao        p  ON p.crtid  = c.crtid AND p.ptostatus = 'A'
									INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
									WHERE
										d.itrid = ".$itrid." AND p.ptostatus = 'A' AND p.inuid = ".$inuid );
	$pontuacao = $pontuacao ? $pontuacao : array();

	foreach ( $pontuacao as $pontuacao1 ) {
		$idpontuacao = $pontuacao1['ptoid'];
		$acao = $db->carregar('SELECT aciid FROM par.acao WHERE ptoid = ' . $idpontuacao);

		if ( $acao != '' ) {
			foreach ( $acao as $acao1 ){
				$idacao = $acao1['aciid'];
				$subacao = $db->carregar("SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid = ".$idacao);

				if ( $subacao != '' ){
					foreach ( $subacao as $subacao1 ){
						$idsubacao = $subacao1['sbaid'];

						//ALTERA O STATUS DA SUBACAO
						$docid = parPegarDocidParaSubacao($idsubacao, 'analise');
					//	$arDados = array();
					//	wf_alterarEstado( $docid, WF_SUBACAO_ANALISE, '', $arDados );


						//Verifico se a ação já foi empenhada
						$empenhoSubacao = $db->pegaUm( "SELECT sbaid FROM par.empenhosubacao WHERE sbaid = ".$idsubacao ." and eobstatus = 'A'");
						if( $empenhoSubacao ){ // Está empenhada
							$verificaStatus = $db->pegaUm( "SELECT esdid FROM workflow.documento WHERE docid = ".$docid );
							if( $verificaStatus == WF_SUBACAO_ANALISE || $verificaStatus == WF_SUBACAO_ELABORACAO ){ // Se estiver em analise ou elaboração eu altero para empenho
								$status = WF_SUBACAO_EMPENHO;
								$acao = WF_SUBACAO_BTN_ENVIAR_PARA_EMPENHO;
							} else { // Senão eu mantenho o status que estava
								$status = $verificaStatus;
								$acao = $db->pegaUm( "SELECT aedid FROM workflow.historicodocumento WHERE docid = ".$docid." ORDER BY htddata DESC LIMIT 1 " );
								if( !$acao ){ // Se não consegui recuperar nenhuma ação eu pego uma ação que o estado fim seja o estado que ele está!
									$acao = $db->pegaUm( "SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = ".$status." ORDER BY aedid DESC LIMIT 1 " );
								}
							}
						} else { // Não está empenhada
							$status = WF_SUBACAO_ANALISE;
							$acao = WF_SUBACAO_BTN_ENVIAR_PARA_ANALISE;
						}

						// cria log no histórico
						$sqlHistoricoSub = "insert into workflow.historicodocumento
											( aedid, docid, usucpf, htddata )
											values ( " . $acao . ", " . $docid . ", '" . $_SESSION['usucpf'] . "', now() )";

						$db->executar($sqlHistoricoSub);

						// atualiza documento
						$sqlDocumentoSub = "update workflow.documento
										set esdid = " . $status . ", docdsc = 'Em Análise'
										where docid = " . $docid;

						$db->executar($sqlDocumentoSub);


						//ALTERA STATUS DA OBRA

						$arrDocid = parPegarArrDocidParaSubacaoObraEnvioAnalise($idsubacao, $oPreObra, $oSubacaoControle);

						if( is_array($arrDocid) && $arrDocid[0] ){
							foreach( $arrDocid as $docidObra ){
								// cria log no histórico
								$sqlHistoricoObr = "insert into workflow.historicodocumento
													( aedid, docid, usucpf, htddata )
													values ( 865, " . $docidObra . ", '" . $_SESSION['usucpf'] . "', now() )
													returning hstid";

								$db->executar($sqlHistoricoObr);

								// atualiza documento
								$sqlDocumentoObr = "update workflow.documento
												set esdid = " . WF_PAR_EM_ANALISE_FNDE . "
												where docid = " . $docidObra;

								$db->executar($sqlDocumentoObr);

							}
						}
					}
				}
			}
		}
	}
	return true;
}

function verificaLatLongEntid(){

	global $db;

	if( $_SESSION['par']['itrid'] == 1 ){
		$funids = array( 1 => 6 );
	} else {
		$funids = array( 1 => 1, 2 => 7 );
	}

	$pendenciaDadoUnidadeEntid = 0;

	$html = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border: none;">
    				<tr>
    					<td colspan="2" class="TituloTela"  style="background-color:#6F8F20" >Existem Dados da Unidade com Pendência</td>
    				</tr>';

	foreach( $funids as $funid ){

		if($funid == FUNID_PREFEITURA || $funid == FUNID_PREFEITO){
			$funid1 = FUNID_PREFEITO;
			$funid2 = FUNID_PREFEITURA;
		}elseif($funid == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO || $funid == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO){
			$funid1 = FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO;
			$funid2 = FUNID_SECRETARIA_MUNICIPAL_EDUCACAO;
		}elseif($funid == FUNID_SECRETARIA_ESTADUAL_EDUCACAO || $funid == FUNID_SECRETARIO_ESTADUAL_EDUCACAO){
			$funid1 = FUNID_SECRETARIO_ESTADUAL_EDUCACAO;
			$funid2 = FUNID_SECRETARIA_ESTADUAL_EDUCACAO;
		}

		$stWhere .= $_SESSION['par']['muncod'] ? "and eed2.muncod = '{$_SESSION['par']['muncod']}'" : "";
		$stWhere .= $_SESSION['par']['estuf'] ? "and eed2.estuf = '{$_SESSION['par']['estuf']}'" : "";

		if($funid == FUNID_PREFEITURA || $funid == FUNID_SECRETARIA_ESTADUAL_EDUCACAO || $funid == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO){

			$sql = "SELECT
						max(ent.entid) as entid1
					FROM entidade.entidade ent
						INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid AND eed2.tpeid = 1
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid2} AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
					{$stWhere}";
			$entid = $db->pegaUm($sql);

			if( $entid ){

				$sql = "SELECT
							medlatitude, medlongitude
						FROM
							entidade.endereco
						WHERE
							tpeid = 1 AND
							entid = ".$entid;

				$dados = $db->pegaLinha( $sql );

				if( $dados['medlatitude'] == '' || $dados['medlongitude'] == '' ){
					$pendenciaDadoUnidadeEntid = 1;
					if($funid == FUNID_PREFEITURA){
						$entidade = 'Prefeitura';
					} else if($funid == FUNID_SECRETARIA_ESTADUAL_EDUCACAO){
						$entidade = 'Secretaria Estadual de Educação';
					} else {
						$entidade = 'Secretaria Municipal de Educação';
					}
					$html .='<tr>
			    				<td colspan="2" style="background-color:#E9E9E9" >Falta preencher a Latitude / Longitude da '.$entidade.'.</td>
							 </tr>';
				}

			}

		}else{

			$sql = "SELECT
						max(ent.entid) as entid1
					FROM entidade.entidade ent
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid1} AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
						LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
						LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid
						LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid
						LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = {$funid2} AND fue2.fuestatus = 'A'
						LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
					{$stWhere}";
			$entid = $db->pegaUm($sql);
			//ver($entid);
			$sql = "SELECT
						max(ent.entid) as entid1
					FROM entidade.entidade ent
						INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid2} AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
					{$stWhere}";
			 $funentassoc = $db->pegaUm($sql);



			 if(!$funentassoc){
			 	if($_SESSION['par']['itrid'] == 2){
				 	if($funid == FUNID_PREFEITO){
				 		$funidTemp = FUNID_PREFEITURA;
				 		$pendenciaDadoUnidadeEntid = 1;
				 		$html .='<tr>
				    				<td colspan="2" style="background-color:#E9E9E9" >Prefeitura não cadastrada.</td>
								 </tr>';
				 	} elseif($funid == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO){
				 		$funidTemp = FUNID_SECRETARIA_MUNICIPAL_EDUCACAO;
				 		$pendenciaDadoUnidadeEntid = 1;
				 		$html .='<tr>
				    				<td colspan="2" style="background-color:#E9E9E9" >Secretaria não cadastrada.</td>
								 </tr>';
				 	}
			 	} else {
			 		if($funid == FUNID_SECRETARIO_ESTADUAL_EDUCACAO){
				 		$funidTemp = FUNID_SECRETARIA_ESTADUAL_EDUCACAO;
				 		$pendenciaDadoUnidadeEntid = 1;
				 		$html .='<tr>
				    				<td colspan="2" style="background-color:#E9E9E9" >Secretaria não cadastrada.</td>
								 </tr>';
					}
				}
			}
		}
	}

	if( $pendenciaDadoUnidadeEntid == 1 ){
		$html .= '</table>';
		return $html;
	}
}


function retornaEsfera( $preid ){

	global $db;

	$sql = "SELECT
				CASE WHEN muncodpar is null
					THEN 'E'
					ELSE 'M'
				END as esfera
			FROM
				obras.preobra
			WHERE
				(muncodpar is not null OR estufpar is not null) AND
				preid = ".$preid;

	$esfera = $db->pegaUm($sql);

	if( $esfera ){
		return $esfera;
	} elseif( $_SESSION['par']['itrid'] ){
		return $_SESSION['par']['itrid'] == 1 ? 'E' : 'M';
	} else {
		return false;
	}

}

function enviarParaAnalisePar($itrid, $inuid, $oPreObra = null, $oSubacaoControle = null ){
	global $db;

	if(!$itrid){
		$itrid 			= $_SESSION['par']['itrid'];
	}
	$tipoRetorno 	= 'boleano';

	// verifica se tem pendências
	if( verificaPendenciaDeAnalise( $itrid, $inuid, $tipoRetorno ) == true ){
		//deleta subações vazias
		$arrAcoes = apagaSubacoesQuandoEnvioAnalise( $inuid, $itrid );

		//deleta ações vazias
		if( is_array( $arrAcoes ) && $arrAcoes[0] ){
			$acoes = implode(",", $arrAcoes);
			deletaAcao( $acoes, 'ANALISE' );
		}

		// copia plano
		$copia = copiarPlanoDeAcoesPar( $inuid, $itrid );

		// altera os status das subações e das obras
		$altera = alteraStatusObrasSubacao( $inuid, $oPreObra, $oSubacaoControle, $itrid );

		if( $copia && $altera ){

			$db->commit();

			// Gera protocolo
			ob_clean();

			$sql = "SELECT
						CASE WHEN itrid = 2 THEN
							(SELECT m.mundescricao || ' - ' || m.estuf FROM territorios.municipio m WHERE m.muncod = iu.muncod)
						ELSE
							iu.estuf
						END as entidade
					FROM
						par.instrumentounidade iu
					WHERE
						inuid = ".$_SESSION['par']['inuid'];
			$dados = $db->pegaUm( $sql );

			if( $itrid == 3 ){
				$tipo = 'ENVIO PARA ANÁLISE DO BRASIL PROFISSIONALIZADO';
			} else {
				$tipo = 'ENVIO PARA ANÁLISE';
			}

			$conteudo = '<html>
							<head>
							<style type="">
								.fot{
									text-transform: uppercase;
									font-family: arial black;
									font-size: 20px;
									text-align: center;
									}
								.lista{
									font-size: 11px;
									padding: 3px;
									border-top: 2px solid #000;
									border-collapse: collapse;
								}
								.lista1{
									font-size: 11px;
									padding: 3px;
									border-top: 2px solid #000;
									border-collapse: collapse;
								}
								table.lista td{
									border-style: solid;
									border-width: 1px;
									border-color: #000;
									border-collapse: collapse;
								}
								.folha {
							    	page-break-after: always;
								}
								@media print {.notprint { display: none } .div_rolagem{display: none} }
								@media screen {.notscreen { display: none; }

								.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}

							</style>
							</head>
							<body>
								<table width="100%" align="center" cellspacing="0" cellpadding="0">
								<tr>
									<td>
										<table width="100%" class="lista" align="center" bgcolor="silver" cellspacing="1" cellpadding="4">
											<tr>
												<td valign="top" style="text-align: center;"><span class="fot">PAR - PLANO DE METAS</span> <br>
												<b><span style="text-align: center; font-size: 9px">'.$tipo.'</span></b></td>
											</tr>
										</table>
										<table width="100%" class="lista" align="center" cellspacing="1" cellpadding="4">
											<tr>
												<td><b>1 - EXERCÍCIO</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.date('Y').'</td>
												<td colspan="4"><b>2 - ENTIDADE</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dados.'</td>
											</tr>
										</table>
										<table width="100%" class="lista" align="center" cellspacing="1" cellpadding="4">
											<tr>
												<td valign="top" style="text-align: center;"><span class="fot">PROTOCOLO '.date('Y').'-'.$inuid.' GERADO COM SUCESSO NO DIA '.date('d/m/Y').'.</span></td>
											</tr>
										</table>
									</td>
								</tr>
							</table>
						</body>
					</html>';

			if( $itrid == 3 ){
				$campos	= array("inuid"  => $inuid,
								"terid" => "1",
								"usucpf" => "'{$_SESSION['usucpf']}'",
								"prtdatainclusao" => "NOW()"
								);
			} else {
				$campos	= array("inuid"  => $inuid,
								"terid" => "3",
								"usucpf" => "'{$_SESSION['usucpf']}'",
								"prtdatainclusao" => "NOW()"
								);
			}

			$arParam = array( 'tabela' => 'protocolo',
							  'campo' => $campos,
							  'extensao' => 'pdf',
							  'nome' => 'protocolo_'.date('Y').'-'.$inuid
						 );

			$http = new RequestHttp();
			$http->toPdfSave( iconv('ISO-8859-1', 'UTF-8', $conteudo), $arParam );

			//altera o estado do WF
			$arDados = array();
			$docid   = parPegarDocidParaEntidade($inuid);
			wf_alterarEstado( $docid, WF_PAR_AEDID_ELABORACAO_ENVIAR_ANALISE, '', $arDados );

			if( $itrid == 3 ){
				$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = 1 AND inuid = ".$inuid );
			} else {
				$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = 3 AND inuid = ".$inuid );
			}
			if( $arqid ){
			    return true;
			} else {
				return false;
			}
		} else {
			$db->rollback();
			return false;
		}
	} else {
		return 'pendencia';
	}
}

function validaSessaoPar( $session ){
	if( empty($session) ){
		echo "<script>
				alert('Falta dados na sessão!');
				history.back(-1);
			  </script>";
		die;
	}
}

function salvarMinuta( $post, $mintexto = '', $dopid = '' ){
	global $db;
	$retorno = salvarDadosMinuta($post, $mintexto);

	if( $retorno ){
		$dopid = $dopid ? $dopid : $post['dopid'];
		$db->sucesso('principal/minuta', "&prpid=".$_GET['prpid']."&dopid=$dopid");
	} else {
		echo "<script>
				alert('Falha na operação');
				window.location.href = 'par.php?modulo=principal/minuta&acao=A&prpid={$post['prpid']}';
			</script>";
		exit();
	}
}

function salvarDadosMinuta( $post, $mintexto = '', $dopidRef = null ){
	global $db;
	
	extract( $post );
	$texto = $mintexto ? $mintexto : $texto;
        
	$dtIni = explode('-', $dopdatainicio);
	$dtFim = explode('-', $dopdatafim);
	if( strlen($dtIni[0]) == 4 ){
		$dopdatainicio 			= "'".$dtIni[0].'-'.$dtIni[1].'-'.trim(substr($dtIni[2],0,2))."'";
		$dopdatafim 			= "'".$dtFim[0].'-'.$dtFim[1].'-'.trim(substr($dtFim[2],0,2))."'";
	}else{
		$dopdatainicio 			= $dopdatainicio 	? "'".formata_data_sql( $dopdatainicio )."'" 	: 'null';
		$dopdatafim 			= $dopdatafim 		? "'".formata_data_sql( $dopdatafim )."'" 		: 'null';
	}
	$dopdatapublicacao		= $dopdatapublicacao ? "'".formata_data_sql( $dopdatapublicacao )."'" : 'null';
	$dopdiasvigencia 		= formata_string_sql( $dopdiasvigencia );
	$dopjustificativa 		= formata_string_sql( $dopjustificativa );
	$doppaginadou	 		= formata_string_sql( $doppaginadou );
	$dopnumportaria	 		= formata_string_sql( $dopnumportaria );
	$dopdatainiciovigencia	= formata_string_sql( $dopdatainiciovigencia );
	$dopdatafimvigencia		= formata_string_sql( $dopdatafimvigencia );
	$dopvalor				= $dopvalor ? $dopvalor : 'null';
	$iueid					= $iueid ? $iueid : 'null';
	    
	if( $prpid && $mdoid){
		
		$sql = "SELECT 
					dopid 
				FROM 
					par.vm_documentopar_ativos 
				WHERE 
					prpid = $prpid 
					AND mdoid = $mdoid 
					AND dopreformulacao = true";
		$dopidpai = $db->pegaUm( $sql );
		$dopidpai = $dopidpai ? $dopidpai : 'null';
	} else {
		$dopidpai = 'null';
	}
//        $doptexto = simec_htmlentities($doptexto, ENT_QUOTES, 'ISO-8859-1');
        
	if( $dopid ){
		$sql = "UPDATE par.documentopar SET
                            ".( $iueid 								? "iueid = $iueid," : "" )."
                            ".( $mdoid 								? "mdoid = $mdoid," : "" )."
                            ".( $dopdiasvigencia != 'null' 			? "dopdiasvigencia = $dopdiasvigencia," : "" )."
                            ".( $dopdatainicio != 'null' 			? "dopdatainicio = $dopdatainicio," : "" )."
                            ".( $dopdatafim != 'null' 				? "dopdatafim = $dopdatafim," : "" )."
                            ".( $dopjustificativa != 'null' 		? "dopjustificativa = $dopjustificativa," : "" )."
                            ".( $dopdatapublicacao != 'null' 		? "dopdatapublicacao = $dopdatapublicacao," : "" )."
                            ".( $doppaginadou != 'null' 			? "doppaginadou = $doppaginadou," : "" )."
                            ".( $dopnumportaria != 'null' 			? "dopnumportaria = $dopnumportaria," : "" )."
                            ".( $dopdatafimvigencia != 'null' 		? "dopdatafimvigencia = $dopdatafimvigencia," : "" )."
                            ".( $dopdatainiciovigencia != 'null' 	? "dopdatainiciovigencia = $dopdatainiciovigencia," : "" )."
                            ".( $dopvalor != 'null' 				? "dopvalortermo = $dopvalor," : "" )."
                            dopdataalteracao = now(),
                            dopacompanhamento = 't',
                            usucpfalteracao = '".$_SESSION['usucpf']."'
				WHERE dopid = $dopid";
		$db->executar( $sql );
	} else {
		$dado1 = "";
		$dado2 = "";

		if( $dopidRef ){
			$dado1 = ", dopnumerodocumento, dopano";
            
			$sql = "select dopnumerodocumento, dopano from par.documentopar where prpid = {$prpid} and dopstatus <> 'E' order by dopid limit 1";
			$dopnumerodocumentoref = $db->pegaLinha( $sql );
			$dado2 = ", ".$dopnumerodocumentoref['dopnumerodocumento'].", ".$dopnumerodocumentoref['dopano'];
			$dopidpai = $dopidRef;
		}
// 		ver($dopdatainiciovigencia, $dopdatafimvigencia, d	);
		$dopdatainiciovigencia 	= date('m').'/'.date('Y');
		$dopdatafimvigencia		= date('m').'/'.(date('Y')+1);

		$sql = "INSERT INTO par.documentopar(
					prpid,
					proid,
					iueid,
					dopdiasvigencia,
					dopdatainicio,
					dopdatafim,
					mdoid,
				  	dopdatainclusao,
				  	usucpfinclusao,
				  	dopdatainiciovigencia,
				  	dopdatafimvigencia,
				  	dopjustificativa,
				  	dopvalortermo,
				  	dopidpai {$dado1} --, dopidaditivo
				  	,dopdataalteracao
				  	,usucpfalteracao 
                                        ,dopacompanhamento
				  	)
				VALUES (
					$prpid,
					null,
					$iueid,
					$dopdiasvigencia,
					$dopdatainicio,
					$dopdatafim,
					$mdoid,
				  	now(),
				  	'".$_SESSION['usucpf']."',
				  	'$dopdatainiciovigencia',
				  	'$dopdatafimvigencia',
				  	$dopjustificativa,
				  	$dopvalor,
				  	$dopidpai {$dado2} --,$dopidaditivo
				  	,now()
				  	,'".$_SESSION['usucpf']."'
                                        ,'t'
                                            
				  	) RETURNING dopid";
		$dopid = $db->pegaUm( $sql );
		
		// inativa a minuta existente
		if($dopidRef){
			$sql = "UPDATE par.documentopar SET dopstatus = 'I' WHERE dopid = $dopidRef and dopstatus = 'A'";
		} else {
			$anoProcesso = $db->pegaUm("SELECT substring(prpnumeroprocesso,12,4) as anoprocesso FROM par.processopar WHERE prpstatus = 'A' and prpid = ".$prpid);
			if( $anoProcesso < 2013 ){
				$sql = "UPDATE par.documentopar SET dopnumerodocumento = {$dopid} WHERE dopid = {$dopid};";
			} else {
				$max = $db->pegaUm( "SELECT MAX(dopnumerodocumento) FROM par.documentopar WHERE substring(cast(dopnumerodocumento as varchar),1,4) = '".date('Y')."'  AND char_length(cast(dopnumerodocumento as varchar)) > 4" );
				$dopNumero = $max ? ($max + 1) : (date('Y') . '00001');
				$sql = "UPDATE par.documentopar SET dopnumerodocumento = {$dopNumero} WHERE dopid = {$dopid};";
			}
			$sql .= "UPDATE par.documentopar SET dopstatus = 'I' WHERE prpid = ".$prpid." and mdoid = $mdoid and dopid <> $dopid and dopstatus = 'A';";
		}
		$db->executar($sql);
	}	

	if( $dopid ){
		if( !empty($_POST['chk'][0]) ){
			$sql = "DELETE FROM par.termocomposicao WHERE dopid = ".$dopid;
			$db->executar( $sql );

			foreach ($_POST['chk'] as $sbdid) {
				$sql = "INSERT INTO par.termocomposicao( sbdid, dopid )
						VALUES ( $sbdid, $dopid)";
				$db->executar( $sql );
			}
		}
	}

	if( $dopid ){
		if( !empty($_POST['objid'][0]) ){
			$sql = "DELETE FROM par.objetodocumentopar WHERE dopid = ".$dopid;
			$db->executar( $sql );
			foreach ($_POST['objid'] as $objid) {
				$sql = "INSERT INTO par.objetodocumentopar( objid, dopid)
						VALUES ( $objid, $dopid)";
				$db->executar( $sql );
			}
		}
	}
	
	if( $mintexto ){
		gravaHtmlDocumento( $mintexto, $dopid, $prpid, 'PAR' );
	}
	
	buscaDadosTermo($dopid);
	
	salvarTermoComposicaoItens( $prpid, $dopid );
	
//	$db->commit();
	return $dopid;
}

function salvarTermoComposicaoItens( $prpid, $dopid ){
	global $db;
	
	$sql = "SELECT
			    sd.sbaid as subacao,
			    case when pic.picpregao = true then 'S' else 'N' end as pregao,
			    sic.icoid
			FROM
			    par.processopar prp
			    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
			    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
			    inner join par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
			    inner join par.propostaitemcomposicao pic ON pic.picid = sic.picid
			WHERE
			    prp.prpid = {$prpid}
			    and prp.prpstatus = 'A'
			    and sic.icostatus = 'A'
			    and sic.icovalidatecnico = 'S'
			    and sd.sbdid in (select sbdid from par.termocomposicao where dopid = {$dopid} and sbdid is not null)
			GROUP BY
			    sd.sbaid,
			    pic.picpregao,
			    sic.icoid";
	
	$arItens = $db->carregar($sql);
	$arItens = $arItens ? $arItens : array();
		
	if( is_array($arItens) && !empty($arItens[0]) ){
		
		$db->executar("delete from par.termocomposicaoitens where dopid = $dopid");
		
		foreach ($arItens as $item) {			
			$sql = "INSERT INTO par.termocomposicaoitens(dopid, icoid, prpid, idppregao, idpstatus)
					VALUES ({$dopid},
							{$item['icoid']},
							{$prpid},
							'{$item['pregao']}',
							'A')";			
			$db->executar($sql);
		}
		$db->commit();
	}
	return true;
}

function enviaEmailDocGeradoPar($email, $esfera, $prpid, $mdoid){
	global $db;

	if($email){
		if($esfera == 'M'){
			$numTermo = $db->pegaUm( "select dopnumerodocumento from par.vm_documentopar_ativos where prpid = $prpid and mdoid = $mdoid" );

			$conteudo = "Está disponível para a validação eletrônica do (a) prefeito (a) municipal o documento referente ao <b>Termo de Compromisso nº $numTermo</b>.<br>
Somente o(a) PREFEITO(A) MUNICIPAL poderá fazer a validação do Termo de Compromisso, para isso:<br>
- acessar ao SIMEC <a href='http://simec.mec.gov.br'>(http://simec.mec.gov.br/)</a> com o seu login (que sempre é o CPF do usuário) e senha;<br>
-módulo PAR >árvore>documentos;<br>
-após conferir os dados, clicar em <b>aceitar</b>.";
		} else {
			$numTermo = $db->pegaUm( "select dopnumerodocumento from par.vm_documentopar_ativos where prpid = $prpid and mdoid = $mdoid" );

			$conteudo = "Está disponível para a validação eletrônica do (a) prefeito (a) municipal o documento referente ao <b>Termo de Compromisso nº $numTermo</b>.<br>
Somente o(a) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO poderá fazer a validação do Termo de Compromisso, para isso:<br>
- acessar ao SIMEC <a href='http://simec.mec.gov.br'>(http://simec.mec.gov.br/)</a> com o seu login (que sempre é o CPF do usuário) e senha;<br>
-módulo PAR >árvore>documentos;<br>
-após conferir os dados, clicar em <b>aceitar</b>.";
		}
		$assunto = 'MEC/FNDE - Documento(s) para validação ';

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $conteudo, $cc, $cco );
		} else {
			enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $conteudo, $cc, $cco );
		}
	}
}

function enviaEmailDocGeradoObras($email, $esfera, $proid, $mdoid){
	global $db;

	if($email){
		if($esfera == 'M'){
			$numTermo = $db->pegaUm( "select dopnumerodocumento from par.vm_documentopar_ativos where proid = $proid and mdoid = $mdoid" );

			$conteudo = "Está disponível para a validação eletrônica do (a) prefeito (a) municipal o documento referente ao <b>Termo de Compromisso nº $numTermo</b>.<br>
Somente o(a) PREFEITO(A) MUNICIPAL poderá fazer a validação do Termo de Compromisso, para isso:<br>
- acessar ao SIMEC <a href='http://simec.mec.gov.br'>(http://simec.mec.gov.br/)</a> com o seu login (que sempre é o CPF do usuário) e senha;<br>
-módulo PAR >árvore>documentos;<br>
-após conferir os dados, clicar em <b>aceitar</b>.";
		} else {
			$numTermo = $db->pegaUm( "select dopnumerodocumento from par.vm_documentopar_ativos where proid = $proid and mdoid = $mdoid" );

			$conteudo = "Está disponível para a validação eletrônica do (a) prefeito (a) municipal o documento referente ao <b>Termo de Compromisso nº $numTermo</b>.<br>
Somente o(a) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO poderá fazer a validação do Termo de Compromisso, para isso:<br>
- acessar ao SIMEC <a href='http://simec.mec.gov.br'>(http://simec.mec.gov.br/)</a> com o seu login (que sempre é o CPF do usuário) e senha;<br>
-módulo PAR >árvore>documentos;<br>
-após conferir os dados, clicar em <b>aceitar</b>.";
		}
		$assunto = 'MEC/FNDE - Documento(s) para validação ';

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $conteudo, $cc, $cco );
		} else {
			enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $conteudo, $cc, $cco );
		}
	}
}

function salvarDadosMinutaObras( $post, $mintexto = '', $dopidRef = null ){
	global $db;
	extract( $post );
	$texto = $mintexto ? $mintexto : $texto;
	
	if(strpos($dopdatainicio,'-')){
		$dtIni = explode('-', $dopdatainicio);
		$dtFim = explode('-', $dopdatafim);
	} else {
		$dtIni = explode('/', $dopdatainicio);
		$dtFim = explode('/', $dopdatafim);
	}
	
	if( strlen($dtIni[0]) == 4 ){
		$dopdatainicio 			= "'".$dtIni[0].'-'.$dtIni[1].'-'.trim(substr($dtIni[2],0,2))."'";
		$dopdatafim 			= "'".$dtFim[0].'-'.$dtFim[1].'-'.trim(substr($dtFim[2],0,2))."'";
	}else{
		$dopdatainicio 			= $dopdatainicio 	? "'".formata_data_sql( $dopdatainicio )."'" 	: 'null';
		$dopdatafim 			= $dopdatafim 		? "'".formata_data_sql( $dopdatafim )."'" 		: 'null';
	}
        
	$dopdatapublicacao	= $dopdatapublicacao ? "'".formata_data_sql( $dopdatapublicacao )."'" : 'null';
	$iueid				= $iueid ? $iueid : 'null';
	$dopdiasvigencia 	= formata_string_sql( $dopdiasvigencia );
	$dopjustificativa 	= formata_string_sql( $dopjustificativa );
	$doppaginadou	 	= formata_string_sql( $doppaginadou );
	$dopnumportaria	 	= formata_string_sql( $dopnumportaria );
        
        if ($dopdatafimvigencia == null) {
            $ano = date('Y');
            $anoFinal = $ano + 2;
            $mes = date('m');

            $dopdatafimvigencia = $_SESSION['par']['cronogramaFinal'] ? $_SESSION['par']['cronogramaFinal'] : $mes.'/'.$anoFinal;
        }
	$dopvalor = $dopvalor ? $dopvalor : 'null';
	
	//$doptexto = $texto ? "'".simec_htmlspecialchars($texto, ENT_QUOTES, 'ISO-8859-1')."'" : 'null';

	/* Testa se está gerando um termo filho
	 * */
	if( $proid && $mdoid ){
		
		$sql = "SELECT 
					dopid 
				FROM 
					par.vm_documentopar_ativos 
				WHERE 
					proid = $proid 
					AND mdoid = $mdoid 
					".($regerar?"":"AND dopreformulacao = TRUE"); //Testa se a funcionalidade é a Regarar termo ou Reformulação
		
		$dopidpai = $db->pegaUm( $sql );
		$dopidpai = $dopidpai ? $dopidpai : 'null';
		
	}else{
		$dopidpai = 'null';
	}

	if( $dopid && $tipoRegera != 'reformular'){
		$sql = "UPDATE par.documentopar SET
				  	iueid = $iueid,
				  	dopdiasvigencia = $dopdiasvigencia,
				  	dopdatainicio = $dopdatainicio,
				  	dopdatafim = $dopdatafim,
				  	mdoid = $mdoid,
				  	dopdataalteracao = now(),
				  	usucpfalteracao = '".$_SESSION['usucpf']."',
				  	dopjustificativa = ".$dopjustificativa.",
				  	dopdatapublicacao = ".$dopdatapublicacao.",
				  	doppaginadou = ".$doppaginadou.",
				  	dopdatafimvigencia = '".$dopdatafimvigencia."',
				  	dopprorrogacaovigenciaobra = 'false',
				  	dopnumportaria = ".$dopnumportaria.",
				  	".( $dopvalor != 'null' 				? "dopvalortermo = $dopvalor," : "" )."
				  	proid = $proid
				WHERE dopid = $dopid";

		$db->executar( $sql );
	} else {
	
		if( $dopidRef ){
                        
                        $dado1 = ", dopnumerodocumento, dopano";	
                        
			$sql = "SELECT dopnumerodocumento, dopano FROM par.documentopar WHERE dopid = ".$dopidRef;
			$dopnumerodocumentoref = $db->pegaLinha( $sql );
                        
			$dado2 = ", ".$dopnumerodocumentoref['dopnumerodocumento'].", ".$dopnumerodocumentoref['dopano'];
			$dopidpai = $dopidRef;
		}

		$sql = "INSERT INTO par.documentopar(
					prpid,
					iueid,
					proid,
					dopdiasvigencia,
					dopdatainicio,
					dopdatafim,
					mdoid,
				  	dopdatainclusao,
				  	usucpfinclusao,
				  	dopdatainiciovigencia,
				  	dopdatafimvigencia,
				  	dopjustificativa,
				  	dopvalortermo,
				  	dopidpai {$dado1}
				  	,dopdataalteracao
				  	,usucpfalteracao
				  	)
				VALUES (
					null,
					$iueid,
					$proid,
					$dopdiasvigencia,
					$dopdatainicio,
				  	$dopdatafim,
					$mdoid,
				  	now(),
				  	'".$_SESSION['usucpf']."',
				  	'$dopdatainiciovigencia',
				  	'$dopdatafimvigencia',
				  	$dopjustificativa,
				  	$dopvalor,
				  	$dopidpai {$dado2}
				  	,now()
				  	,'".$_SESSION['usucpf']."'
				  	) 
				RETURNING 
					dopid";

		$dopid = $db->pegaUm( $sql );
		// inativa a inuta existente
		if($dopidRef){
			$sql = "UPDATE par.documentopar SET dopstatus = 'I' WHERE dopid = $dopidRef and dopstatus = 'A'";
		} else {
			$sql = "UPDATE par.documentopar SET dopnumerodocumento = {$dopid} WHERE dopid = {$dopid};";
			$db->executar($sql);
			$sql = "UPDATE par.documentopar SET dopstatus = 'I' WHERE proid = ".$proid." and mdoid = $mdoid and dopid <> $dopid and dopstatus = 'A'";
		}
		$db->executar($sql);
	}

	if( $dopid ){
		if( !empty($_POST['chk'][0]) ){
			$sql = "DELETE FROM par.termocomposicao WHERE dopid = ".$dopid;
			$db->executar( $sql );

			foreach ($_POST['chk'] as $preid) {
				$sql = "INSERT INTO par.termocomposicao( preid, dopid)
						VALUES ( $preid, $dopid)";
				$db->executar( $sql );
			}
		}

		if( !empty($_POST['objid'][0]) ){
			$sql = "DELETE FROM par.objetodocumentopar WHERE dopid = ".$dopid;
			$db->executar( $sql );
			foreach ($_POST['objid'] as $objid) {
				$sql = "INSERT INTO par.objetodocumentopar( objid, dopid)
						VALUES ( $objid, $dopid)";
				$db->executar( $sql );
			}
		}
	}
	
	if( $mintexto ){
		gravaHtmlDocumento( $mintexto, $dopid, $proid, 'OBRA' );
	}
	
	$db->commit();
	return $dopid;
}

function alteraMacrosMinuta( $imitexto, $prpid, $post = array()) {
    global $db;

    $cpfpresidentefnde = '';
    $mdoid = $post['mdoid'];
    $tpdcod = $post['tpdcod'];

    $arrSub = $post['chk'];

    $arrSub = $arrSub ? $arrSub : array();

    $sql = "SELECT
                teccampo,
                tectabela,
                teccoluna,
                tecschema,
                tecdescricao,
                tecsql
            FROM
                par.termocampos
            WHERE
                tecstatus = 'A'";

    $dados = $db->carregar($sql);
    $dados = $dados ? $dados : array();

    foreach ($dados as $key => $v) {
        $alias = '';
        if(strpos($imitexto, $v["teccampo"])) {
            /*caso contrario substitui os dados pelo o retorno do sql*/
            // monta o 'select' para realizar a substituição
            $varAlteracaoMacro = '';

            if( !empty($v['tectabela']) ){

                if(strtolower($v['teccampo']) == strtolower('#Objeto_Convenio#')){

                    $sql1 = montaSQLMacro($v['tectabela'], $v['teccampo'], $v['teccoluna'], $prpid);
                    $varAlteracao = $db->carregarColuna($sql1);
                    $varMacro = array();
                    foreach ($varAlteracao as $objeto) {
                            array_push( $varMacro, $objeto );
                    }
                    $varAlteracaoMacro = implode(', ', $varMacro);
                } else if( strtolower($v['teccampo']) == strtolower('#Nome_da_Entidade_Executora#') ){
                    $sql = "SELECT
                                iue.iuenome as universidade,
                                iue.iuecnpj as cnpjuniversidade
                            FROM par.instrumentounidadeentidade iue
                            WHERE 
                                iue.iueid = {$post['iueid']}";
                    if( !empty($post['iueid']) ){
                        $varAlteracaoMacro = $db->pegaUm($sql);
                    } else {
                        $varAlteracaoMacro = '';
                    }					
                } else if( strtolower($v['teccampo']) == strtolower('#Nome_do_Responsavel_Entidade_Executora#') ){

                    if( !empty($post['iueid']) ){
                        $arExecutor = carregarExecutor( $post['iueid'] );
                        $varAlteracaoMacro = $arExecutor['reitor'];
                    } else {
                        $varAlteracaoMacro = '';
                    }					
                } else if( strpos( strtolower($v['teccampo']), strtolower('Dirigente') ) ){
                    $varAlteracaoMacro = $db->pegaUm("SELECT DISTINCT
                                                            u.usunome
                                                        FROM
                                                            par.usuarioresponsabilidade ur
                                                        inner join seguranca.usuario u on u.usucpf = ur.usucpf
                                                        inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
                                                        WHERE
                                                            ur.pflcod = ".PAR_PERFIL_PREFEITO."
                                                            and us.sisid = 23
                                                            and ur.muncod = (select muncod from par.processopar where prpid = $prpid and prpstatus = 'A')
                                                            and ur.rpustatus = 'A'
                                                            and us.suscod = 'A'");

                } else {
                    $sql1 = montaSQLMacro($v['tectabela'], $v['teccampo'], $v['teccoluna'], $prpid);
                    $varAlteracaoMacro = $db->pegaUm($sql1);
                }
            } else {

                if( $v['tecsql'] ){
                    $sql = $v['tecsql'];
                    $sql.= " WHERE pro.prpid = ".$prpid;

                    if( strtolower($v['teccampo']) == strtolower('#Emenda_Parlamentar#') ){
                        $sql.= " AND sep.sepstatus = 'A'";
                        $varAlteracaoMacro = $db->carregarColuna($sql);
                        $varAlteracaoMacro = implode('<br>', $varAlteracaoMacro);
                    } elseif( strtolower($v['teccampo']) == strtolower('#Sequencial_Termo#') ){
                        $dopnumerodocumento = $db->pegaUm("select dopnumerodocumento from par.documentopar where prpid = {$prpid} and dopstatus <> 'E' order by dopid asc LIMIT 1");
                        $varAlteracaoMacro = $dopnumerodocumento;
                    } else {
                        $varAlteracaoMacro = $db->pegaUm($sql);
                    }
                } else {

                    if( strtolower($v['teccampo']) == strtolower('#Valor_Contrapartida#') || strtolower($v['teccampo']) == strtolower('#Valor_Concedente#')
                                    || strtolower($v['teccampo']) == strtolower('#Valor_Total_Convenio#') ||
                                    strtolower($v['teccampo']) == strtolower('#Valor_Contrapartida_Extenso#') || strtolower($v['teccampo']) == strtolower('#Valor_Concedente_Extenso#')
                                    || strtolower($v['teccampo']) == strtolower('#Valor_Total_Convenio_Extenso#') ){

                        $sqlValor = "SELECT  ".$v['teccoluna']."
                                        FROM (
                                        SELECT
                                            CASE WHEN s.sbacronograma = 1
                                            THEN ( SELECT sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
                                                FROM par.subacaoitenscomposicao sic
                                                WHERE sic.sbaid = s.sbaid
                                                AND sic.icoano = sd.sbdano
                                                GROUP BY sic.sbaid )
                                            ELSE ( SELECT sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
                                                FROM par.subacaoitenscomposicao sic
                                                INNER JOIN par.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
                                                WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
                                                GROUP BY sic.sbaid )
                                            END AS totalprojeto
                                        FROM par.processopar p
                                        INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
                                        INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
                                        INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
                                        INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
                                        WHERE p.prpid = ".$prpid."
                                        and p.prpstatus = 'A'
                                        ) AS dados";

                            $varAlteracaoMacro = $db->pegaUm($sqlValor);
                    } else if( strpos( strtolower($v['teccampo']), strtolower('Concedente') ) ){
                        if( !empty($v['teccoluna']) ){
                            $sql = "SELECT DISTINCT ".$v['teccoluna']." FROM entidade.entidade ent
                                                    INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
                                                INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod
                                            WHERE ent.entstatus = 'A' AND ent.entnumcpfcnpj = '$cpfpresidentefnde'";

                            $varAlteracaoMacro = $db->pegaUm($sql);
                        }
                    }
                    else if( strtolower($v['teccampo']) == strtolower('#Nome_Estado#') ){
                        $varAlteracaoMacro = $db->pegaUm("select es.estdescricao from par.processopar p
                                                            inner join par.instrumentounidade iu on iu.inuid = p.inuid
                                                        inner join territorios.estado es on (es.estuf = iu.estuf or es.estuf = iu.mun_estuf)
                                                        where p.prpid = $prpid and p.prpstatus = 'A'");
                    }
                    else if( strtolower($v['teccampo']) == strtolower('#Ano_Exercicio#') ){
                        if( $_SESSION['par']['dopid'] ){
                            $sqlAno = "SELECT dopano FROM par.documentopar WHERE prpid = (SELECT prpid FROM par.documentopar WHERE dopid = {$_SESSION['par']['dopid']}) ORDER BY dopid ASC LIMIT 1";
                        }else{
                            $sqlAno = "SELECT to_char(now(), 'YYYY') as data";
                        }
                        $varAlteracaoMacro = $db->pegaUm( $sqlAno );
                        if( !$varAlteracaoMacro ){
                            $sqlAno = "SELECT to_char(now(), 'YYYY') as data";
                            $varAlteracaoMacro = $db->pegaUm( $sqlAno );
                        }
                    }
                    else if( strtolower($v["teccampo"]) == strtolower('#Tabela_Cronograma_Desembolso#') ){
                        //monta a tabela cronograma desembolso
                        $varAlteracaoMacro = montaTabelaParcelaDesembolso( $prpid );
                    }
                    else if( strtolower($v["teccampo"]) == strtolower('#Tabela_raf_complemento_subacao#') ){
                        //monta a tabela de execução financeira
                        $tabela = montaTabelaRafComplementoSubacao( $prpid );
                        $varAlteracaoMacro = $tabela['tabela'];
                    }
                    else if( strtolower($v["teccampo"]) == strtolower('#Tabela_Classe_Financeira#') ){
                        //monta a tabela de execução financeira
                        $varAlteracaoMacro = montaTabelaClasseFinanceira( $prpid );
                    }
                    else if( strtolower($v["teccampo"]) == strtolower('#Termo_Compromisso#') ){
                        $varAlteracaoMacro = montaTermoCompromisso( $prpid, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#Talela_Identificacao_Ente_Federado#')){
                        $varAlteracaoMacro = montaTabelaSubAcaoItens( $prpid, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_municipios#')){
                        $varAlteracaoMacro = montaTabelaDelimitacaoAcoesFinanceiraMunicipio( $prpid, $arrSub, $mdoid, $tpdcod, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_estados#')){
                        $varAlteracaoMacro = montaTabelaDelimitacaoAcoesFinanceiraEstados( $prpid, $arrSub, $mdoid, $tpdcod, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#tabela_subacao_empenho_estado#')){
                        $varAlteracaoMacro = montaTabelaSubacaoEmpenhoEstado( $prpid, $arrSub, $mdoid, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#tabela_subacao_empenho_municipio#')){
                        $varAlteracaoMacro = montaTabelaSubacaoEmpenhoMunicipio( $prpid, $arrSub, $mdoid, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_BP_Executora#')){
                        $varAlteracaoMacro = montaTabelaTermoCompromisso_BP_Executora( $prpid, $arrSub, $mdoid, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Universidades_Pacto#')){
                        $varAlteracaoMacro = montaTabelaTermoCompromissoUniversidadesPacto( $prpid, $arrSub, $mdoid, $post['iueid'] );
                    }
                    else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Universidades_Brasil_Pro#')){
                        $varAlteracaoMacro = montaTabelaTermoCompromissoUniversidadesBrasilPro( $prpid, $arrSub, $mdoid, $post['iueid'] );
                    }
//                    else if(strtolower($v['teccampo']) == strtolower('#Termo_Compromisso_Brasil_Pro#')){
//                            $varAlteracaoMacro = montaTabelaTermo_Compromisso_Brasil_Pro( $prpid, $arrSub, $mdoid, $arrSub );
//                    }
                    else if( strtolower($v['teccampo']) == strtolower('#Nome_da_Secretaria_Educacao#') || strtolower($v['teccampo']) == strtolower('#Nome_do_Secretario_Educacao#') ){

                        $arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid from	par.processopar p
                                                            inner join par.instrumentounidade iu on iu.inuid = p.inuid
                                                        where p.prpid = $prpid and p.prpstatus = 'A'" );

                        if( $arDadosProcesso['itrid'] == 2 && $arDadosProcesso['inuid'] != 1 ){
                            // CNPJ da prefeitura
                            $arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
                                                                                                      ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
                                                                                            FROM  par.entidade ent
                                                                                            INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_MUNICIPAL."
                                                                                            INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
                                                                                            where 
                                                                                                    ent.dutid = ".DUTID_DIRIGENTE." AND
                                                                                                    ent.entstatus='A' AND
                                                                                                    ent2.entstatus='A' AND 
                                                                                                    ent.inuid = ".$arDadosProcesso['inuid']);
                        } else {
                            //CNPJ do municipio
                            $arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.estdescricao as mundescricao, mun.estuf,
                                                                    ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
                                                            FROM  par.entidade ent
                                                            INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
                                                            INNER JOIN territorios.estado mun on mun.estuf = ent2.estuf
                                                            where 
                                                                    ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
                                                                    ent.entstatus='A' AND
                                                                    ent2.entstatus='A' AND 
                                                                    ent.inuid = ".$arDadosProcesso['inuid']);
                        }

                        if( strtolower($v['teccampo']) == strtolower('#Nome_do_Secretario_Educacao#') ){
                                $varAlteracaoMacro = $arrSecretaria['secretario'];
                        } else {
                                $varAlteracaoMacro = $arrSecretaria['entnome'];
                        }
                    }
                }
            }

            if(strtolower($v['teccampo']) == strtolower('#Ano_Processo#')){
                $varAlteracaoMacro = $db->pegaUm("select substring(prpnumeroprocesso, 12, 4) from par.processopar WHERE prpstatus = 'A' and prpid = ".$prpid);
            }

            if( strpos( strtolower($v["teccampo"]), 'cnpj') || strpos( strtolower($v["teccampo"]), 'cpf') ){
                $varAlteracaoMacro = formatar_cpf_cnpj( $varAlteracaoMacro );
            }
            if( strpos( strtolower($v["teccampo"]), 'cep') ){
                if($varAlteracaoMacro){
                    $varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "-" . substr($varAlteracaoMacro,5,3);
                }
            }
            if( $v["teccampo"] == '#Numero_Processo#' ){ //formata o numero do processo empenho
                    if($varAlteracaoMacro){
                        $varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "." .
                                            substr($varAlteracaoMacro,5,6) . "/" .
                                            substr($varAlteracaoMacro,11,4) . "-" .
                                            substr($varAlteracaoMacro,15,2);
                    }
            }

            if( strpos( strtolower($v["teccampo"]), 'valor') || strpos( strtolower($v["teccampo"]), 'soma') ){
                if( strpos( strtolower($v["teccampo"]), strtolower('Extenso') ) )
                    $varAlteracaoMacro = valorMonetarioExtenso($varAlteracaoMacro);
                else
                    $varAlteracaoMacro = number_format( $varAlteracaoMacro, 2, ',', '.' );
            }

            $imitexto = str_replace($v["teccampo"], strtoupper($varAlteracaoMacro), $imitexto);
        }
    }
    
    return $imitexto;
}

function montaSQLMacro($mictabela, $miccampo, $miccoluna, $prpid){

	switch($mictabela) {
		case 'entidade':
			if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
				$join =	"INNER JOIN par.instrumentounidade iu ON iu.muncod = pro.muncod
						 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO." ";
			}else{
				$join = "INNER JOIN par.entidade ent ON ent.muncod = pro.muncod and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITURA." ";
			}
			$alias = "ent";
			break;

		case 'endereco':
			if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
				$join = "INNER JOIN par.instrumentounidade iu ON iu.muncod = pro.muncod
						 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO." ";
			}else{
				$join = "INNER JOIN par.entidade ent ON ent.muncod = pro.muncod and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITURA." ";
			}
			$alias = "ent";
			break;

		case 'municipio':
			if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
				$join =	"INNER JOIN par.instrumentounidade iu ON iu.muncod = pro.muncod
						 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO."
						 INNER JOIN territorios.municipio mun on mun.muncod = iu.muncod ";
			}else{
				$join =	"INNER JOIN par.entidade ent ON ent.muncod = pro.muncod and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITURA."
						 INNER JOIN territorios.municipio mun on mun.muncod = ent.muncod ";
			}
			$alias = "mun";
			break;

		case 'documentopar':
			$join = "INNER JOIN par.vm_documentopar_ativos pmc on pmc.prpid = pro.prpid";
			$alias = "pmc";
			break;

		case 'processopar':
			$join = "";
			$alias = "pro";
			break;
		case 'objeto':
			$join = "INNER JOIN par.documentopar min on min.prpid = pro.prpid
					 INNER JOIN par.objetodocumentopar omc on omc.dopid = min.dopid and omc.dopid = {$_SESSION['par']['dopid']}
					 INNER JOIN par.objeto obc on obc.objid = omc.objid ";
			$alias = "obc";
			break;
		case 'resolucao':
			$join = "";
			$alias = "res";
			break;

		case 'funcao':
			$join = "INNER JOIN par.instrumentounidade iu ON iu.muncod = pro.muncod
					 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO."
					 INNER JOIN par.dadosunidadetipo dut ON dut.dutid = ent.dutid  ";

			$alias = 'dut';
			$miccoluna = 'dutdescricao';
			break;
		case 'empenho':
			$join = "inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso and empstatus <> 'I' ";
			$alias = "emp";
			break;
		case 'banco':
			$join = "inner join financeiro.banco ban on ban.bcocod = pro.prpbanco ";
			$alias = "ban";
			break;
		case 'bancoagencia':
			$join = "inner join financeiro.bancoagencia bco on pro.prpbanco = bco.codbanco and bco.codagencia = pro.prpagencia ";
			$alias = "bco";
			break;
		case 'instrumentounidadeentidade':
			if( strpos( strtolower($miccampo), strtolower('Responsavel_Entidade_Executora') ) ){
				$join = "inner join par.instrumentounidadeentidade iue on iue.iuecnpj = pro.prpcnpj and pro.inuid = iue.inuid and iuedefault = false
						 inner join par.usuarioresponsabilidade ur on ur.entid = iue.entid
						 inner join entidade.entidade e on iue.entid = e.entid
						 inner join seguranca.usuario u on u.usucpf = ur.usucpf and rpustatus = 'A' and ur.pflcod = 853";
				$alias = "u";
			} else if( strpos( strtolower($miccampo), strtolower('Entidade_Executora') ) ){
				$join = "inner join par.instrumentounidadeentidade iue on iue.iuecnpj = pro.prpcnpj and pro.inuid = iue.inuid and iuedefault = false
						 inner join par.usuarioresponsabilidade ur on ur.entid = iue.entid
						 inner join entidade.entidade e on iue.entid = e.entid
						 inner join seguranca.usuario u on u.usucpf = ur.usucpf and rpustatus = 'A' and ur.pflcod = 853 ";
				$alias = "e";
			}
		break;
	}

	if ( strtolower($miccoluna) == 'dopdatainicio' ||  strtolower($miccoluna) == 'dopdatafim' || strpos( strtolower($miccoluna), 'data' ) ){//formata campo data
		if( $alias == 'pmc' ) $filtro = " and pmc.dopid = {$_SESSION['par']['dopid']} ";
		$sql 	= 	"SELECT to_char(".$alias.".".$miccoluna.", 'DD/MM/YYYY') FROM par.processopar pro ";
		$sql 	.= 	$join;
		$sql 	.= 	" WHERE pro.prpstatus = 'A' and pro.prpid = ".$prpid." $filtro ";
	} else if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
		$sql = "select ent.entnome
				FROM par.entidade ent
				inner join par.instrumentounidade iu ON iu.inuid = ent.inuid
				inner join par.processopar 			pro  on pro.muncod = iu.muncod and pro.prpstatus = 'A'
				where ent.entstatus='A' and dutid = ".DUTID_PREFEITO." and pro.prpid = $prpid";
	} else {
		$sql 	= 	"SELECT ".$distinct." ".$alias.".".$miccoluna." FROM par.processopar pro ";
		$sql 	.= 	$join;
		$sql 	.= 	" WHERE pro.prpstatus = 'A' and pro.prpid = ".$prpid;
	}
	return $sql;
}

function alteraMacrosMinutaObrasBP(&$imitexto, $proid, $post) {
	global $db;
	$cpfpresidentefnde = '';
	$mdoid = $post['mdoid'];
	$tpdcod = $post['tpdcod'];
	$iueid = $post['iueid'];
	$arrObra = $post['chk'];
	$arrObra = $arrObra ? $arrObra : array();
	
	$sql = "SELECT
			  	teccampo,
			  	tectabela,
			  	teccoluna,
			  	tecschema,
			  	tecdescricao,
			  	tecsql
			FROM
			  	par.termocampos
			WHERE
				tecstatus = 'A'";

	$dados = $db->carregar($sql);
	$dados = $dados ? $dados : array();

	foreach ($dados as $key => $v) {
		$alias = '';

		if(strpos($imitexto, $v["teccampo"])) {
			/*caso contrario substitui os dados pelo o retorno do sql*/
			// monta o 'select' para realizar a substituição
			$varAlteracaoMacro = '';

			if( !empty($v['tectabela']) ){
				
				/*if(strtolower($v['teccampo']) == strtolower('#Nome_da_Entidade_Executora#') || strtolower($v['teccampo']) == strtolower('#Nome_do_Responsavel_Entidade_Executora#')){
				
				
				if( $v['teccoluna'] == 'entnome' ){
				$alias = "ent.entnome";
				}else{
				$alias = "usu.usunome";
				}
					
				$sql = "SELECT DISTINCT $alias
				FROM par.processoobraspar pro  -- Entidade
				INNER JOIN par.instrumentounidadeentidade 		iu  ON iu.inuid = pro.inuid AND itrid = 3
				LEFT  JOIN entidade.entidade 		  			ent ON ent.entid = iu.entid AND ent.entstatus = 'A'
				LEFT  JOIN entidade.endereco		  			ed  ON ed.entid = ent.entid
				LEFT  JOIN territorios.estado 		  			est ON est.estuf = ed.estuf
				LEFT  JOIN territorios.municipio 	  			mun ON mun.muncod = est.muncodcapital
				--responsavel
				LEFT  JOIN par.usuarioresponsabilidade 	  		urs ON urs.entid = iu.entid AND pflcod = 1141 AND rpustatus = 'A'
				LEFT  JOIN seguranca.usuario		 			usu ON usu.usucpf = urs.usucpf
				WHERE pro.prostatus = 'A' and pro.proid = $proid";
				
				$varAlteracaoMacro = $db->pegaUm($sql);
				}*/
				
				if( strtolower($v['teccampo']) == strtolower('#Nome_da_Entidade_Executora#') ){
					$sql = "SELECT
								iue.iuenome as universidade,
								iue.iuecnpj as cnpjuniversidade
							FROM par.instrumentounidadeentidade iue
							WHERE iue.iueid = {$post['iueid']}";
					if( !empty($post['iueid']) ){
						$varAlteracaoMacro = $db->pegaUm($sql);
					} else {
						$varAlteracaoMacro = '';
					}
				} else if( strtolower($v['teccampo']) == strtolower('#Nome_do_Responsavel_Entidade_Executora#') ){
			
					if( !empty($post['iueid']) ){
							$arExecutor = carregarExecutor( $post['iueid'] );
							$varAlteracaoMacro = $arExecutor['reitor'];
					} else {
					$varAlteracaoMacro = '';
					}
				}elseif(strtolower($v['teccampo']) == strtolower('#Objeto_Convenio#')){
					$sql1 = montaSQLMacroObras($v['tectabela'], $v['teccampo'], $v['teccoluna'], $proid);
					$varAlteracao = $db->carregarColuna($sql1);
					$varMacro = array();
					foreach ($varAlteracao as $objeto) {
						array_push( $varMacro, $objeto );
					}
					$varAlteracaoMacro = implode(', ', $varMacro);
				} elseif( strtolower($v['teccampo']) == strtolower('#Sequencial_Termo#') ){
					$dopnumerodocumento = $db->pegaUm("select dopnumerodocumento from par.documentopar where proid = {$proid} and dopstatus <> 'E' order by dopid asc LIMIT 1");
					$varAlteracaoMacro = $dopnumerodocumento;
				} else {
					$sql1 = montaSQLMacroObras($v['tectabela'], $v['teccampo'], $v['teccoluna'], $proid);
					$varAlteracaoMacro = $db->pegaUm($sql1);
				}
			} else {

				if( $v['tecsql'] ){
					$sql = $v['tecsql'];
					$sql.= " WHERE pro.prpid = ".$proid;

					if( strtolower($v['teccampo']) == strtolower('#Emenda_Parlamentar#') ){
						$varAlteracaoMacro = $db->carregarColuna($sql);
						$varAlteracaoMacro = implode('<br>', $varAlteracaoMacro);
					} else if( strtolower($v['teccampo']) == strtolower('#Sequencial_Termo#') ){
						$sql = "select dopnumerodocumento from par.documentopar where proid = {$proid} and dopstatus <> 'E' order by dopid asc LIMIT 1";
						$varAlteracaoMacro = $db->pegaUm($sql);
					} else {
						$varAlteracaoMacro = $db->pegaUm($sql);
					}
				} else {
					if( strtolower($v['teccampo']) == strtolower('#Valor_Contrapartida#') || strtolower($v['teccampo']) == strtolower('#Valor_Concedente#')
							|| strtolower($v['teccampo']) == strtolower('#Valor_Total_Convenio#') ||
							strtolower($v['teccampo']) == strtolower('#Valor_Contrapartida_Extenso#') || strtolower($v['teccampo']) == strtolower('#Valor_Concedente_Extenso#')
							|| strtolower($v['teccampo']) == strtolower('#Valor_Total_Convenio_Extenso#') ){

						$sqlValor = "SELECT  ".$v['teccoluna']."
								FROM (
								SELECT
								            CASE WHEN s.sbacronograma = 1
								            THEN ( SELECT sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
								                        FROM par.subacaoitenscomposicao sic
								                        WHERE sic.sbaid = s.sbaid
								                        AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
								            ELSE ( SELECT sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
								                        FROM par.subacaoitenscomposicao sic
								                        INNER JOIN par.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
								                        WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
								            END AS totalprojeto
								FROM par.processopar p
								INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso
								INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
								INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
								INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
								WHERE e.empstatus <> 'I' and p.prpid = ".$prpid."
								and p.prpstatus = 'A'
								) AS dados";

						$varAlteracaoMacro = $db->pegaUm($sqlValor);
					} else if( strpos( strtolower($v['teccampo']), strtolower('Concedente') ) ){
						if( !empty($v['teccoluna']) ){
							$sql = "SELECT DISTINCT ".$v['teccoluna']." FROM entidade.entidade ent
										INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
									    INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod
									WHERE ent.entstatus = 'A' AND ent.entnumcpfcnpj = '$cpfpresidentefnde'";

							$varAlteracaoMacro = $db->pegaUm($sql);
						}
					}else if( strtolower($v['teccampo']) == strtolower('#Ano_Exercicio#') ){
						if( $_SESSION['par']['dopid'] ){
							$sqlAno = "SELECT dopano FROM par.documentopar WHERE prpid = (SELECT prpid FROM par.documentopar WHERE dopid = {$_SESSION['par']['dopid']}) ORDER BY dopid ASC LIMIT 1";
						}else{
							$sqlAno = "SELECT to_char(now(), 'YYYY') as data";
						}
						$varAlteracaoMacro = $db->pegaUm( $sqlAno );
						if( !$varAlteracaoMacro ){
							$sqlAno = "SELECT to_char(now(), 'YYYY') as data";
							$varAlteracaoMacro = $db->pegaUm( $sqlAno );
						}
					}
					/*else if( strtolower($v["teccampo"]) == strtolower('#Tabela_Cronograma_Desembolso#') ){
						//monta a tabela cronograma desembolso
						//$varAlteracaoMacro = montaTabelaParcelaDesembolso( $prpid );
					}else if( strtolower($v["teccampo"]) == strtolower('#Tabela_Classe_Financeira#') ){
						//monta a tabela de execução financeira
//						$varAlteracaoMacro = montaTabelaClasseFinanceira( $prpid );
					}else if( strtolower($v["teccampo"]) == strtolower('#Termo_Compromisso#') ){
//						$varAlteracaoMacro = montaTermoCompromisso( $prpid );
					}else if(strtolower($v['teccampo']) == strtolower('#Talela_Identificacao_Ente_Federado#')){
//						$varAlteracaoMacro = montaTabelaSubAcaoItens( $prpid );
					}else if(strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_municipios#')){
//						$varAlteracaoMacro = montaTabelaDelimitacaoAcoesFinanceiraMunicipio( $prpid );
					}else if(strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_estados#')){
//						$varAlteracaoMacro = montaTabelaDelimitacaoAcoesFinanceiraEstados( $prpid );
					}*/else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Obras_Municipios#')){
						$varAlteracaoMacro = montaTabelaTermoCompromissoMunicipiosBP( $proid, $mdoid, $tpdcod, $arrObra, $iueid );
					}else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Obras_estados#')){
						$varAlteracaoMacro = montaTabelaTermoCompromissoEstadoBP( $proid, $mdoid, $tpdcod, $arrObra, $iueid );
					}else if(strtolower($v['teccampo']) == strtolower('#Termo_Compromisso_Brasil_Pro#')){
						$varAlteracaoMacro = montaTabelaTermo_Compromisso_Brasil_Pro( $proid, $mdoid, $arrObra, $iueid );
					}else if( strtolower($v['teccampo']) == strtolower('#Nome_da_Secretaria_Educacao#') || strtolower($v['teccampo']) == strtolower('#Nome_do_Secretario_Educacao#') ){

						$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid from	par.processoobraspar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prostatus = 'A' and p.proid = $proid" );

// 						INNER JOIN par.instrumentounidadeentidade 	iu  ON iu.inuid = pro.inuid AND itrid = 3
						if( $arDadosProcesso['itrid'] == 2 && $arDadosProcesso['inuid'] != 1 ){
							// CNPJ da Secretaria Municipal
							$sql = "SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
										  ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
									FROM  par.entidade ent
									INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_MUNICIPAL."
									INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
									where 
										ent.dutid = ".DUTID_DIRIGENTE." AND
										ent.entstatus='A' AND
										ent2.entstatus='A' AND 
										ent.inuid = ".$arDadosProcesso['inuid'];

						} else {
							//CNPJ do municipio
							$sql = "SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.estdescricao as mundescricao, mun.estuf,
										ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
									FROM  par.entidade ent
									INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
									INNER JOIN territorios.estado mun on mun.estuf = ent2.estuf
									where 
										ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
										ent.entstatus='A' AND
										ent2.entstatus='A' AND 
										ent.inuid = ".$arDadosProcesso['inuid'];
						}
// 						ver($sql,d);
						$arrSecretaria = $db->pegaLinha( $sql );

						if( strtolower($v['teccampo']) == strtolower('#Nome_do_Secretario_Educacao#') ){
							$varAlteracaoMacro = $arrSecretaria['secretario'];
						} else {
							$varAlteracaoMacro = $arrSecretaria['entnome'];
						}
					}
				}
			}
			
			if(strtolower($v['teccampo']) == strtolower('#Ano_Processo#')){
				$varAlteracaoMacro = $db->pegaUm("select substring(pronumeroprocesso, 12, 4) from par.processoobraspar WHERE prostatus = 'A' and proid = ".$proid);
			}

			if( strpos( strtolower($v["teccampo"]), 'cnpj') || strpos( strtolower($v["teccampo"]), 'cpf') ){
				$varAlteracaoMacro = formatar_cpf_cnpj( $varAlteracaoMacro );
			}
			if( strpos( strtolower($v["teccampo"]), 'cep') ){
				if($varAlteracaoMacro){
					$varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "-" .
										 substr($varAlteracaoMacro,5,3);
				}
			}
			if( $v["teccampo"] == '#Numero_Processo#' ){ //formata o numero do processo empenho
				if($varAlteracaoMacro){
					$varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "." .
										 substr($varAlteracaoMacro,5,6) . "/" .
										 substr($varAlteracaoMacro,11,4) . "-" .
										 substr($varAlteracaoMacro,15,2);
				}
			}

			if( strpos( strtolower($v["teccampo"]), 'valor') || strpos( strtolower($v["teccampo"]), 'soma') ){

				if( strpos( strtolower($v["teccampo"]), strtolower('Extenso') ) )
					$varAlteracaoMacro = valorMonetarioExtenso($varAlteracaoMacro);
				else
					$varAlteracaoMacro = number_format( $varAlteracaoMacro, 2, ',', '.' );
			}

			$imitexto = str_replace($v["teccampo"], strtoupper($varAlteracaoMacro), $imitexto);
		}
	}
	
	return $imitexto;
}

function alteraMacrosMinutaObras(&$imitexto, $proid, $post) {
	global $db;

// 	return $imitexto;
	
	$cpfpresidentefnde = '';
	$mdoid = $post['mdoid'];
	$tpdcod = $post['tpdcod'];
	$iueid = $post['iueid'];
	$arrObra = $post['chk'];
	$arrObra = $arrObra ? $arrObra : array();

	$sql = "SELECT
			  	teccampo,
			  	tectabela,
			  	teccoluna,
			  	tecschema,
			  	tecdescricao,
			  	tecsql
			FROM
			  	par.termocampos
			WHERE
				tecstatus = 'A'";

	$dados = $db->carregar($sql);
	$dados = $dados ? $dados : array();

	foreach ($dados as $key => $v) {
		$alias = '';

		if(strpos($imitexto, $v["teccampo"])) {
			/*caso contrario substitui os dados pelo o retorno do sql*/
			// monta o 'select' para realizar a substituição
			$varAlteracaoMacro = '';

			if( !empty($v['tectabela']) ){

				if(strtolower($v['teccampo']) == strtolower('#Objeto_Convenio#')){
					$sql1 = montaSQLMacroObras($v['tectabela'], $v['teccampo'], $v['teccoluna'], $proid);
					$varAlteracao = $db->carregarColuna($sql1);
					$varMacro = array();
					foreach ($varAlteracao as $objeto) {
						array_push( $varMacro, $objeto );
					}
					$varAlteracaoMacro = implode(', ', $varMacro);
				} elseif ( strtolower($v['teccampo']) == strtolower('#Nome_da_Entidade_Executora#') ){
					$sql = "SELECT
								iue.iuenome as universidade,
								iue.iuecnpj as cnpjuniversidade
							FROM par.instrumentounidadeentidade iue
							WHERE iue.iueid = {$post['iueid']}";
					if( !empty($post['iueid']) ){
						$varAlteracaoMacro = $db->pegaUm($sql);
					} else {
						$varAlteracaoMacro = '';
					}
				} else if( strtolower($v['teccampo']) == strtolower('#Nome_do_Responsavel_Entidade_Executora#') ){
			
					if( !empty($post['iueid']) ){
							$arExecutor = carregarExecutor( $post['iueid'] );
							$varAlteracaoMacro = $arExecutor['reitor'];
					} else {
					$varAlteracaoMacro = '';
					}
				} elseif( strtolower($v['teccampo']) == strtolower('#Valor_Total_Empenho#') ){
					/*$sql = "SELECT sum(po.saldo)
								FROM
									par.termocomposicao tc
								    inner join par.v_saldo_empenho_por_obra po on po.preid = tc.preid
								WHERE
									tc.dopid = {$post['dopid']}";
					$varAlteracaoMacro = $db->pegaUm($sql);*/
                                    $sql = "SELECT DISTINCT
                                                                sepvalor::NUMERIC(15,2)
                                                        FROM par.processoobraspar pro
                                                           inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso and emp.empstatus = 'A'
                                                           inner join par.empenhoobrapar ems ON ems.empid = emp.empid
                                                           inner join par.subacaoobra so on so.preid = ems.preid
                                                           inner join par.subacaodetalhe sd on sd.sbaid = so.sbaid and sd.sbdano = so.sobano
                                                           inner join par.subacaoemendapta sep ON sep.sbdid = sd.sbdid and sep.sepstatus = 'A'
                                                           inner join emenda.emendadetalhe ed ON ed.emdid = sep.emdid
                                                           inner join emenda.emenda eme on eme.emeid = ed.emeid
                                                           inner join par.documentopar dop ON dop.proid = pro.proid
                                                        WHERE dop.dopid = {$post['dopid']}";
                                            $varAlteracaoMacro = $db->pegaUm($sql);
					
				} elseif( strtolower($v['teccampo']) == strtolower('#Sequencial_Termo#') ){
					$dopnumerodocumento = $db->pegaUm("select dopnumerodocumento from par.documentopar where proid = {$proid} and dopstatus <> 'E' order by dopid asc LIMIT 1");
					$varAlteracaoMacro = $dopnumerodocumento;
				} else {
					$sql1 = montaSQLMacroObras($v['tectabela'], $v['teccampo'], $v['teccoluna'], $proid);					
					
					if( strtolower($v['teccampo']) == strtolower('#Nome_Dirigente#') ){
						$sql1 = str_replace(Array('processopar','pro.prpid'),Array('processoobraspar','pro.proid'),$sql1);
						$sql1 = str_replace('pro.prpstatus', 'pro.prostatus', $sql1);						
					}
					$varAlteracaoMacro = $db->pegaUm($sql1);
				}
			} else {
				
				if( $v['tecsql'] ){
					$sql = $v['tecsql'];
					$sql.= " WHERE pro.proid = ".$proid;
					$sql = str_ireplace('par.processopar', 'par.processoobraspar', $sql);
					$sql = str_ireplace('prpid', 'proid', $sql);
					
					if( strtolower($v['teccampo']) == strtolower('#Emenda_Parlamentar#') ){
						$sql = "SELECT DISTINCT
									eme.emecod || '/' || eme.emeano AS emenda
								FROM
									par.processoobraspar pro
								    inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso and emp.empstatus = 'A'
								    inner join par.empenhoobrapar ems ON ems.empid = emp.empid
								    inner join par.subacaoobra so on so.preid = ems.preid
								    inner join par.subacaodetalhe sd on sd.sbaid = so.sbaid and sd.sbdano = so.sobano
								    inner join par.subacaoemendapta sep ON sep.sbdid = sd.sbdid and sep.sepstatus = 'A'
								    inner join emenda.emendadetalhe ed ON ed.emdid = sep.emdid
								    inner join emenda.emenda eme on eme.emeid = ed.emeid
								WHERE pro.proid = $proid";
						//ver($sql,d);
						$varAlteracaoMacro = $db->carregarColuna($sql);
						$varAlteracaoMacro = implode(', ', $varAlteracaoMacro);
					} else if( strtolower($v['teccampo']) == strtolower('#Sequencial_Termo#') ){
                        $sql = "select dopnumerodocumento from par.documentopar where proid = {$proid} and dopstatus <> 'E' order by dopid asc LIMIT 1";
						$varAlteracaoMacro = $db->pegaUm($sql);
					} else {
						$varAlteracaoMacro = $db->pegaUm($sql);
					}
				} else {
					if( strtolower($v['teccampo']) == strtolower('#cronograma_inicial#') ) {
						$varAlteracaoMacro = $db->pegaUm("select to_char(now(), 'MM/YYYY')");
						
					}elseif( strtolower($v['teccampo']) == strtolower('#cronograma_final#') ) {
						$varAlteracaoMacro = $db->pegaUm("select to_char((now() + INTERVAL '720 day'), 'MM/YYYY')");
						
					}elseif( strtolower($v['teccampo']) == strtolower('#sequencial_reformulacao#') ) {
						$sql = "select count(repid) from par.reprogramacao p where p.dopidoriginal in (
										select dopid from par.documentopar where proid = $proid and dopstatus <> 'E'
									)";
						$totTermo = $db->pegaUm($sql);
						$totTermo = ( (int) $totTermo == 0  ? 1 : (int) $totTermo);
						
						$varAlteracaoMacro = str_to_upper(numeroCardinalOrdinal($totTermo, false, 'ordinal', 'fem'));
						
					}elseif( strtolower($v['teccampo']) == strtolower('#mostrar_obras_termo#') ) {
						$sql = "SELECT
									po.preid || ' - ' || predescricao as obra,
									prelogradouro as logradouro,
									precomplemento as complemento,
									ptodescricao as tipoobra,
									pto.ptoid,
									prevalorobra as vlrobra
								FROM
								    par.termocomposicao tc
								    inner join obras.preobra po on po.preid = tc.preid and po.prestatus = 'A'
								    inner join obras.pretipoobra pto ON pto.ptoid = po.ptoid
								WHERE
								    tc.dopid = {$post['dopid']}";
						$arObras = $db->carregar($sql);
						$arObras = $arObras ? $arObras : array();
					
						$tabela = '';
						 foreach ($arObras as $key => $dado) {
						 	$obra = $dado['obra'] != '' ? "" . wordwrap($dado['obra'], 50, '<br />') . "<br />" : "";
						 	$logradouro = $dado['logradouro'] != '' ? "" . wordwrap($dado['logradouro'], 50, '<br />') . "<br />" : "";
						 	$complemento = $dado['complemento'] != '' ? "" . wordwrap($dado['complemento'], 50, '<br />') . "<br />" : "";
						 	
							$tabela .= "<div style=\"display: table;\">
									<div class=\"P_2\">".($key + 1).")</div>
									<div class=\"P_1\">" . $obra . "
		                       " . $logradouro . "
		                       " . $complemento . "
		                       " . $dado['tipoobra'] . "<br> 
		                       	R$ " . number_format($dado['vlrobra'], 2, ',', '.') . "</div>
		                       </div><br />";
						} 
						$html .= '
						<ol><div>
						' . $tabela . '
						</div></ol>';
						$varAlteracaoMacro = ($html);
						
					}elseif( strtolower($v['teccampo']) == strtolower('#valor_total_termo#') ) {
						$sql = "SELECT 
									cast(sum(po.prevalorobra) as numeric(15,2)) as valor
                                FROM obras.preobra po
                                WHERE po.preid IN (SELECT DISTINCT 
                                                   		po.preid
                                                   FROM obras.preobra po
                                                   		inner join par.termocomposicao tc on tc.preid = po.preid and po.prestatus = 'A'
                                                    WHERE tc.dopid = {$post['dopid']})";
						
						$valorTermo = $db->pegaUm($sql);
						
						$varAlteracaoMacro = (float)$valorTermo;
					}elseif( strtolower($v['teccampo']) == strtolower('#valor_fnde#') ) {
						
						$sql = "SELECT DISTINCT 
								    sum(oe.prevalorcomplementarfnde)
								FROM par.termocomposicao tc
								    inner join obras.preobra oe on oe.preid = tc.preid and oe.prestatus = 'A'
								WHERE tc.dopid = {$post['dopid']}";
						$valor_fnde = $db->pegaUm($sql);
						$varAlteracaoMacro = ($valor_fnde ? $valor_fnde : '0,00');
						
					}elseif( strtolower($v['teccampo']) == strtolower('#valor_total_emendas#') ) {
						
						$sql = "SELECT DISTINCT 
								    sum(oe.obpvaloremenda)
								FROM par.termocomposicao tc
								    inner join par.obraemenda oe on oe.preid = tc.preid and oe.obpstatus = 'A'
								WHERE tc.dopid = {$post['dopid']}";
						$valor_total_emendas = $db->pegaUm($sql);
						$varAlteracaoMacro = ($valor_total_emendas ? $valor_total_emendas : '0,00');
						
					}elseif( strtolower($v['teccampo']) == strtolower('#Valor_Contrapartida#') || strtolower($v['teccampo']) == strtolower('#Valor_Concedente#')
							|| strtolower($v['teccampo']) == strtolower('#Valor_Total_Convenio#') ||
							strtolower($v['teccampo']) == strtolower('#Valor_Contrapartida_Extenso#') || strtolower($v['teccampo']) == strtolower('#Valor_Concedente_Extenso#')
							|| strtolower($v['teccampo']) == strtolower('#Valor_Total_Convenio_Extenso#') ){

						$sqlValor = "SELECT  ".$v['teccoluna']."
								FROM (
								SELECT
								            CASE WHEN s.sbacronograma = 1
								            THEN ( SELECT sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
								                        FROM par.subacaoitenscomposicao sic
								                        WHERE sic.sbaid = s.sbaid
								                        AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
								            ELSE ( SELECT sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
								                        FROM par.subacaoitenscomposicao sic
								                        INNER JOIN par.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
								                        WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
								            END AS totalprojeto
								FROM par.processopar p
								INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
								INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
								INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
								INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
								WHERE p.prpid = ".$prpid."
								and p.prpstatus = 'A'
								) AS dados";

						$varAlteracaoMacro = $db->pegaUm($sqlValor);
					} else if( strpos( strtolower($v['teccampo']), strtolower('Concedente') ) ){
						if( !empty($v['teccoluna']) ){
							$sql = "SELECT DISTINCT ".$v['teccoluna']." FROM entidade.entidade ent
										INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
									    INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod
									WHERE ent.entstatus = 'A' AND ent.entnumcpfcnpj = '$cpfpresidentefnde'";

							$varAlteracaoMacro = $db->pegaUm($sql);
						}
					}else if( strtolower($v['teccampo']) == strtolower('#Ano_Exercicio#') ){
						if( $_SESSION['par']['dopid'] ){
							$sqlAno = "SELECT dopano FROM par.documentopar WHERE prpid = (SELECT prpid FROM par.documentopar WHERE dopid = {$_SESSION['par']['dopid']}) ORDER BY dopid ASC LIMIT 1";
						}else{
							$sqlAno = "SELECT to_char(now(), 'YYYY') as data";
						}
						$varAlteracaoMacro = $db->pegaUm( $sqlAno );
						if( !$varAlteracaoMacro ){
							$sqlAno = "SELECT to_char(now(), 'YYYY') as data";
							$varAlteracaoMacro = $db->pegaUm( $sqlAno );
						}
					}
					/*else if( strtolower($v["teccampo"]) == strtolower('#Tabela_Cronograma_Desembolso#') ){
						//monta a tabela cronograma desembolso
						//$varAlteracaoMacro = montaTabelaParcelaDesembolso( $prpid );
					}else if( strtolower($v["teccampo"]) == strtolower('#Tabela_Classe_Financeira#') ){
						//monta a tabela de execução financeira
//						$varAlteracaoMacro = montaTabelaClasseFinanceira( $prpid );
					}else if( strtolower($v["teccampo"]) == strtolower('#Termo_Compromisso#') ){
//						$varAlteracaoMacro = montaTermoCompromisso( $prpid );
					}else if(strtolower($v['teccampo']) == strtolower('#Talela_Identificacao_Ente_Federado#')){
//						$varAlteracaoMacro = montaTabelaSubAcaoItens( $prpid );
					}else if(strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_municipios#')){
//						$varAlteracaoMacro = montaTabelaDelimitacaoAcoesFinanceiraMunicipio( $prpid );
					}else if(strtolower($v['teccampo']) == strtolower('#Tabela_Extrato_execucao_plano_acoes_articuladas_estados#')){
//						$varAlteracaoMacro = montaTabelaDelimitacaoAcoesFinanceiraEstados( $prpid );
					}*/else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Obras_Municipios#')){
						$varAlteracaoMacro = montaTabelaTermoCompromissoMunicipios( $proid, $mdoid, $tpdcod, $arrObra, $iueid );
					}else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Obras_estados#')){
						$varAlteracaoMacro = montaTabelaTermoCompromissoEstado( $proid, $mdoid, $tpdcod, $arrObra, $iueid );
					}else if(strtolower($v['teccampo']) == strtolower('#Tabela_Termo_Compromisso_Obras_estados_RDC#')){
						$varAlteracaoMacro = montaTabelaTermoCompromissoEstadoRDC( $proid, $mdoid, $tpdcod, $arrObra, $iueid );
					}else if( strtolower($v['teccampo']) == strtolower('#Nome_da_Secretaria_Educacao#') || strtolower($v['teccampo']) == strtolower('#Nome_do_Secretario_Educacao#') ){

						$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid from	par.processoobraspar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prostatus = 'A' and p.proid = $proid" );

						if( $arDadosProcesso['itrid'] == 2 && $arDadosProcesso['inuid'] != 1 ){
							// CNPJ da prefeitura
							$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
																  ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
															FROM  par.entidade ent
															INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_MUNICIPAL."
															INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
															where 
																ent.dutid = ".DUTID_DIRIGENTE." AND
																ent.entstatus='A' AND
																ent2.entstatus='A' AND 
																ent.inuid = ".$arDadosProcesso['inuid']);
						} else {
								//CNPJ do municipio
								$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.estdescricao as mundescricao, mun.estuf,
																	ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
																FROM  par.entidade ent
																INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
																INNER JOIN territorios.estado mun on mun.estuf = ent2.estuf
																where 
																	ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
																	ent.entstatus='A' AND
																	ent2.entstatus='A' AND 
																	ent.inuid = ".$arDadosProcesso['inuid']);
						}

						if( strtolower($v['teccampo']) == strtolower('#Nome_do_Secretario_Educacao#') ){
							$varAlteracaoMacro = $arrSecretaria['secretario'];
						} else {
							$varAlteracaoMacro = $arrSecretaria['entnome'];
						}
					}
				}
			}
			
			if(strtolower($v['teccampo']) == strtolower('#Ano_Processo#')){
				$varAlteracaoMacro = $db->pegaUm("select substring(pronumeroprocesso, 12, 4) from par.processoobraspar WHERE prostatus = 'A' and proid = ".$proid);
			}

			if( strpos( strtolower($v["teccampo"]), 'cnpj') || strpos( strtolower($v["teccampo"]), 'cpf') ){
				$varAlteracaoMacro = formatar_cpf_cnpj( $varAlteracaoMacro );
			}
			if( strpos( strtolower($v["teccampo"]), 'cep') ){
				if($varAlteracaoMacro){
					$varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "-" .
										 substr($varAlteracaoMacro,5,3);
				}
			}
			if( $v["teccampo"] == '#Numero_Processo#' ){ //formata o numero do processo empenho
				if($varAlteracaoMacro){
					$varAlteracaoMacro = substr($varAlteracaoMacro,0,5) . "." .
										 substr($varAlteracaoMacro,5,6) . "/" .
										 substr($varAlteracaoMacro,11,4) . "-" .
										 substr($varAlteracaoMacro,15,2);
				}
			}

			if( strpos( strtolower($v["teccampo"]), 'valor') || strpos( strtolower($v["teccampo"]), 'soma') ){

				if( strpos( strtolower($v["teccampo"]), strtolower('Extenso') ) )
					$varAlteracaoMacro = valorMonetarioExtenso($varAlteracaoMacro);
				else
					$varAlteracaoMacro = simec_number_format( $varAlteracaoMacro, 2, ',', '.' );
			}

			$imitexto = str_replace($v["teccampo"], strtoupper($varAlteracaoMacro), $imitexto);
		}
	}
	
	return $imitexto;
}

function montaSQLMacroObras($mictabela, $miccampo, $miccoluna, $proid){

	switch($mictabela) {
		case 'instrumentounidadeentidade':

			$join = "-- Entidade
					INNER JOIN par.instrumentounidadeentidade 	iu  ON iu.inuid = pro.inuid AND itrid = 3
					INNER JOIN entidade.entidade 		  		ent ON ent.entid = iu.entid
					LEFT  JOIN entidade.endereco		  		ed  ON ed.entid = ent.entid
					LEFT  JOIN territorios.estado 		  		est ON est.estuf = ed.estuf
					LEFT  JOIN territorios.municipio 	  		mun ON mun.muncod = est.muncodcapital
					--responsavel
					INNER JOIN par.instrumentounidade 	  		iu2 ON iu2.inuid = pro.inuid
					LEFT  JOIN par.usuarioresponsabilidade 	  	urs ON urs.estuf = iu2.estuf AND pflcod = 839 AND rpustatus = 'A'
					LEFT  JOIN seguranca.usuario		 		usu ON usu.usucpf = urs.usucpf";

			if( $miccoluna == 'entnome' ){
// 				$join =	"INNER JOIN par.instrumentounidadeentidade 	iu  ON iu.inuid = pro.inuid
// 						 INNER JOIN entidade.entidade 				ent ON e.entid = iu.entid OR e.entnumcpfcnpj = iu.iuecnpj";
				$alias = "ent";
			}else{
// 				$join =	"INNER JOIN par.instrumentounidade 		iu  ON iu.inuid = pro.inuid
// 						 INNER JOIN par.usuarioresponsabilidade urs ON urs.estuf = iu.estuf AND pflcod = 839 AND rpustatus = 'A'
// 						 INNER JOIN seguranca.usuario			usu ON usu.usucpf = urs.usucpf --AND usu.usustatus = 'A'";
				$alias = "usu";
			}
			break;
		case 'entidade':
			/*
			if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
				$join =	"INNER JOIN par.instrumentounidade iu on iu.inuid = pro.inuid
						 INNER JOIN par.instrumentounidadeentidade	iue  ON ent2.entid = iue.entid AND iue.iuedefault = TRUE
						 INNER JOIN entidade.endereco ende ON ende.muncod = iu.muncod
						 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid and ent.entstatus = 'A'
						 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=2 and fen.fuestatus = 'A'
						 INNER JOIN entidade.funentassoc fue on fue.fueid = fen.fueid ";
			}else{
				$join = "INNER JOIN par.instrumentounidade iu on iu.inuid = pro.inuid
						 INNER JOIN par.instrumentounidadeentidade	iue  ON ent2.entid = iue.entid AND iue.iuedefault = TRUE
						 INNER JOIN entidade.endereco ende ON ende.muncod = iu.muncod
						 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
						 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=1 and fen.fuestatus = 'A' ";
			}
			$alias = "ent";
			*/
			if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
				$join =	"INNER JOIN par.instrumentounidade iu ON iu.muncod = pro.muncod
						 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO;
				
// 					--	 INNER JOIN entidade.endereco ende ON ende.muncod = pro.muncod
// 					--	 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
// 					--	 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=2 and fen.fuestatus = 'A'
// 					--	 inner join entidade.funentassoc fue on fue.fueid = fen.fueid ";
			}else{
				$join =	"INNER JOIN par.entidade ent ON ent.muncod = pro.muncod and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITURA;
				
// 					--	 INNER JOIN entidade.endereco ende ON ende.muncod = pro.muncod
// 					--	 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
// 					--	 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=1 and fen.fuestatus = 'A' ";
			}
			
			$alias = "ent";
			break;

		case 'endereco':
			
			if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
				$join = "INNER JOIN par.instrumentounidade iu ON iu.inuid = pro.inuid
						 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO;
				
// 					--	 inner join par.instrumentounidade iu on iu.inuid = pro.inuid
// 					--	 INNER JOIN entidade.endereco ende ON ende.muncod = iu.muncod
// 					--	 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
// 					--	 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=2 and fen.fuestatus = 'A'
// 					--	 inner join entidade.funentassoc fue on fue.fueid = fen.fueid ";
			}else{
				$join = "INNER JOIN par.entidade ent ON ent.inuid = pro.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITURA;
				
// 					--	 inner join par.instrumentounidade iu on iu.inuid = pro.inuid
// 					--	 INNER JOIN entidade.endereco ende ON ende.muncod = iu.muncod
// 					--	 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
// 					--	 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=1 and fen.fuestatus = 'A' ";
			}
			$alias = "ent";
			break;

		case 'municipio':
			
			if( strpos( strtolower($miccampo), strtolower('Dirigente') ) ){
				$join =	"INNER JOIN par.instrumentounidade iu ON iu.inuid = pro.inuid
						 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO."
						 INNER JOIN territorios.municipio mun on mun.muncod = iu.muncod";
				
// 					--	 inner join par.instrumentounidade iu on iu.inuid = pro.inuid
// 					--	 INNER JOIN entidade.endereco ende ON ende.muncod = iu.muncod
// 					--	 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
// 					--	 INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod
// 					--	 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=2 and fen.fuestatus = 'A'
// 					--	 inner join entidade.funentassoc fue on fue.fueid = fen.fueid ";
			}else{
				$join =	"INNER JOIN par.entidade ent ON ent.inuid = pro.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITURA."
						 INNER JOIN territorios.municipio mun on mun.muncod = ent.muncod";
				
// 					--	 inner join par.instrumentounidade iu on iu.inuid = pro.inuid
// 					--	 INNER JOIN entidade.endereco ende ON ende.muncod = iu.muncod
// 					--	 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
// 					--	 INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod
// 					--	 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=1 and fen.fuestatus = 'A' ";
			}
			$alias = "mun";
			break;

		case 'documentopar':
			$join = "INNER JOIN par.vm_documentopar_ativos pmc on pmc.prpid = pro.proid";
			$alias = "pmc";
			break;

		case 'processopar':
			$join = "";
			$alias = "pro";
			break;
		case 'objeto':
			$join = "INNER JOIN par.documentopar min on min.prpid = pro.proid
					 INNER JOIN par.objetodocumentopar omc on omc.dopid = min.dopid and omc.dopid = {$_SESSION['par']['dopid']}
					 INNER JOIN par.objeto obc on obc.objid = omc.objid ";
			$alias = "obc";
			break;
		case 'resolucao':
			$join = "";
			$alias = "res";
			break;

		case 'funcao':
			$join = "INNER JOIN par.instrumentounidade iu ON iu.muncod = pro.muncod
					 INNER JOIN par.entidade ent ON ent.inuid = iu.inuid and ent.entstatus = 'A' AND ent.dutid = ".DUTID_PREFEITO."
					 INNER JOIN par.dadosunidadetipo dut ON dut.dutid = ent.dutid"; 
			
// 				--	 INNER JOIN entidade.endereco ende ON ende.muncod = pro.muncod
// 				--	 INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
// 				--	 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid and fen.funid=2 and fen.fuestatus = 'A'
// 				--	 inner join entidade.funcao fuc on fuc.funid = fen.funid and fuc.funstatus = 'A' ";

			$alias = 'dut';
			$miccoluna = 'dutdescricao';
			break;
		case 'empenho':
			$join = "inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso and empstatus <> 'I' ";
			$alias = "emp";
			break;
		case 'banco':
			$join = "inner join financeiro.banco ban on ban.bcocod = pro.prpbanco ";
			$alias = "ban";
			break;
		case 'bancoagencia':
			$join = "inner join financeiro.bancoagencia bco on pro.prpbanco = bco.codbanco and bco.codagencia = pro.prpagencia ";
			$alias = "bco";
			break;
	}
// ver($mictabela, $miccampo, $miccoluna, d);
	if ( strtolower($miccoluna) == 'dopdatainicio' ||  strtolower($miccoluna) == 'dopdatafim' || strpos( strtolower($miccoluna), 'data' ) ){//formata campo data
		if( $alias == 'pmc' ) $filtro = " and pmc.dopid = {$_SESSION['par']['dopid']} ";
		$sql 	= 	"SELECT to_char(".$alias.".".$miccoluna.", 'DD/MM/YYYY') FROM par.processoobraspar pro ";
		$sql 	.= 	$join;
		$sql 	.= 	" WHERE pro.proid = ".$proid." and pro.prostatus = 'A' $filtro ";
	} else if( strpos( strtolower($miccampo), strtolower('Nome_Dirigente') ) ){
		$sql = "SELECT 
					ent.entnome
				FROM 
					par.entidade ent
				INNER JOIN par.instrumentounidade 	iu  ON iu.inuid = ent.inuid
				INNER JOIN par.processoobraspar		pro ON pro.muncod = iu.muncod and  pro.prostatus = 'A'
				WHERE 
					ent.entstatus='A' 
					AND dutid = ".DUTID_PREFEITO." 
					AND pro.proid = $proid";
	} else {
		$sql 	= 	"SELECT ".$distinct." ".$alias.".".$miccoluna." FROM par.processoobraspar pro ";
		$sql 	.= 	$join;
		$sql 	.= 	" WHERE pro.proid = ".$proid." and pro.prostatus = 'A'";
	}

// 	if( $mictabela == 'instrumentounidadeentidade' ){

// 	ver($sql,d);
// 	}
	return $sql;
}

function montaTabelaSubAcaoItens( $prpid, $iueid = '' ){
	global $db;

	$sql = "SELECT  empid as idempenho,
	            sbaid as idsubacao,
	            sbadsc as descricaosubacao,
	            icoid as iditem,
	            icodescricao as descricaoitem,
	            quantidade as quantidadedeitens,
	            valor as valorunitario,
	            (quantidade * valor) as valortotalporitem
			FROM (
			            SELECT  e.empid,
			                        es.sbaid,
			                        s.sbadsc,
			                        sic.icoid,
			                        sic.icodescricao,
			                        CASE WHEN s.sbacronograma = 1 --global
			                        THEN
			                                   CASE WHEN sic.icovalidatecnico IS NULL -- antigos
			                                   THEN coalesce(sic.icoquantidade,0)
			                                   ELSE -- novos
			                                               CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
			                                                           sum(coalesce(sic.icoquantidadetecnico,0))
			                                               END
			                                   END
			                        ELSE -- escolas
			                                    CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
			                                   THEN -- escolas sem itens
			                                               CASE WHEN se.sesvalidatecnico IS NULL -- antigos
			                                                           THEN
			                                                                       sum(coalesce(se.sesquantidade,0))
			                                                           ELSE -- novos
			                                                                       sum(coalesce(se.sesquantidadetecnico,0))
			                                                           END
			                                               ELSE -- escolas com itens
			                                                           CASE WHEN sic.icovalidatecnico IS NULL -- antigos
			                                                           THEN
			                                                                       sum(coalesce(ssi.seiqtd,0))
			                                                           ELSE -- novos
			                                                                       CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
			                                                                                  sum(coalesce(ssi.seiqtdtecnico,0))
			                                                                       END
			                                                           END
			                                               END
			                        END as quantidade,
			                        sic.icovalor as valor
			            FROM par.empenho e
			            INNER JOIN par.empenhosubacao es  ON e.empid =  es.empid and eobstatus = 'A'
			            INNER JOIN par.processopar pp ON pp.prpnumeroprocesso = e.empnumeroprocesso and pp.prpstatus = 'A'
		            INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
		            INNER JOIN par.subacao s ON s.sbaid = es.sbaid
		            INNER JOIN par.subacaodetalhe sd  ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
		            LEFT  JOIN par.subacaoitenscomposicao sic ON sic.sbaid         = s.sbaid   AND sic.icoano = sd.sbdano
		            LEFT  JOIN par.subacaoescolas se  ON se.sbaid           = sic.sbaid AND se.sesano = sic.icoano
		            LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid          = se.sesid
		            WHERE empstatus <> 'I' and
		                pp.prpid = $prpid
		            GROUP BY e.empid,
		                         es.sbaid,
		                         s.sbadsc,
		                         sic.icoid,
		                         sic.icodescricao,
		                         s.sbacronograma,
		                         sic.icovalidatecnico,
		                         sic.icoquantidade,
		                         s.frmid,
		                         s.ptsid,
		                         se.sesvalidatecnico,
		                         sic.icovalor
		) AS dados";

	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	$tabela = '<table align="left" style="border-style: dotted;" border="1" cellspacing="0" cellpadding="2">';

	$sql = "SELECT DISTINCT prg.prgdsc
			FROM par.instrumentounidade 	iu
				INNER JOIN par.pontuacao 	po on po.inuid   = iu.inuid AND ptostatus <> 'I'
				INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid
				INNER JOIN par.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'
				INNER JOIN par.processopar 	p  ON p.inuid = iu.inuid and p.prpstatus = 'A'
			    INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid
			    --INNER JOIN par.empenhoprocessoplanointerno epp ON epp.plicod = sd.sbdplanointerno AND epp.prpnumeroprocesso = p.prpnumeroprocesso
			    inner join par.programa prg on prg.prgid = s.prgid
			WHERE p.prpid = $prpid";

	$arPrograma = $db->carregarColuna($sql);
	$arPrograma = $arPrograma ? $arPrograma : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso from	par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpstatus = 'A' and p.prpid = $prpid" );

	if( $arDadosProcesso['itrid'] == 2 ){
		// CNPJ da prefeitura
		$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
												  ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
											FROM  par.entidade ent
											INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_MUNICIPAL."
											INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
											where 
												ent.dutid = ".DUTID_DIRIGENTE." AND
												ent.entstatus='A' AND
												ent2.entstatus='A' AND 
												ent.inuid = ".$arDadosProcesso['inuid']);
	} else {
			//CNPJ do municipio
			$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.estdescricao as mundescricao, mun.estuf,
													ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
												FROM  par.entidade ent
												INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
												INNER JOIN territorios.estado mun on mun.estuf = ent2.estuf
												where 
													ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
													ent.entstatus='A' AND
													ent2.entstatus='A' AND 
													ent.inuid = ".$arDadosProcesso['inuid']);
	}

	$sql = "SELECT           s.sbadsc as finalidade,
			           	'01/' || sd.sbdinicio::text || '/' || sd.sbdano::text
			            AS cronogramainicial,
			            CASE   WHEN sd.sbdanotermino IS NOT NULL
			                        THEN '01/' || sd.sbdfim::text || '/' || sd.sbdanotermino::text
			                        ELSE '01/' || sd.sbdfim::text || '/' || sd.sbdano::text
			            END AS cronogramafinal
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpstatus = 'A' and p.prpid = $prpid";

	$arCronograma = $db->pegaLinha( $sql );
	
	$cont = 1;
		
		$tabela .= '<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>
							'.($arPrograma ? '-'.implode('<br>-', $arPrograma) : '').'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$_SESSION['exercicio'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA DE EDUCAÇÃO DO ESTADO)</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).'-ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
				if( !empty($iueid) ){
					$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
					$tabela .= $tabelaEntidade['tabela'];
					$cont = $tabelaEntidade['cont'];
				}
				$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>DETALHAMENTO DO OBJETO</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="left" style="border-style: dotted;" width="95%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td><b>Tipo</b></td>
									<td><b>Quantidade</b></td>
									<td><b>Preço Unitário</b></td>
									<td style="text-align: right;"><b>Total</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;
		foreach ($arDadosItem as $key => $valor) {
			$totalQTD += (float)$valor['quantidadedeitens'];
			$totalVLR += (float)$valor['valorunitario'];
			$totalTOT += (float)$valor['valortotalporitem'];
			$tabela .= '	<tr>
											<td>'.$valor['descricaoitem'].'</td>
											<td>'.$valor['quantidadedeitens'].'</td>
											<td>R$ '.($valor['valorunitario'] ? number_format($valor['valorunitario'],2,",",".") : '').'</td>
											<td>R$ '.($valor['valortotalporitem'] ? number_format($valor['valortotalporitem'],2,",",".") : '').'</td>
										</tr>';
		}
			$tabela .= '
										<tr>
											<td align="center"><b>Total Geral</b></td>
											<td>'.$totalQTD.'</td>
											<td>R$ '.number_format($totalVLR,2,",",".").'</td>
											<td>R$ '.number_format($totalTOT,2,",",".").'</td>
										</tr>
									</table>
								</td>
							</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
									<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
								</tr>
							</table>
						</td>
					</tr>
					';
	$tabela .= '</table><br><br>';


	return $tabela;
}

function montaTabelaDelimitacaoAcoesFinanceiraMunicipio( $prpid, $arrSub, $mdoid, $tpdcod, $iueid = '' ){

	global $db;

	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "SELECT
				foo.codigo,
                foo.picdescricao,
                foo.ptsdescricao,
                foo.picid,
                foo.sbdano,
                foo.sbaid,
                foo.quantidade,
                foo.valor,
                ( foo.quantidade * foo.valor ) as total
			FROM (
				SELECT 
					pic.picdescricao,
					pts.ptsdescricao,
					pic.picid,
					sd.sbdano as sbdano,
					s.sbaid,
					par.recuperaquantidadeitemvalidado( sic.icoid )	AS quantidade,
					sic.icovalor as valor,
					d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo	
				FROM par.processopar pp
					INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
					INNER JOIN par.subacaodetalhe 		sd  ON sd.sbdid = ppc.sbdid
					INNER JOIN par.subacao       		s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
					INNER  JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND icostatus = 'A'
					INNER JOIN par.propostaitemcomposicao   pic ON pic.picid = sic.picid
					LEFT JOIN par.propostasubacao 		pps ON pps.ppsid = s.ppsid		
					LEFT JOIN par.propostatiposubacao	pts ON pts.ptsid = pps.ptsid
					INNER JOIN par.acao 					a   ON a.aciid = s.aciid
					INNER JOIN par.pontuacao 				pon ON pon.ptoid = a.ptoid
					INNER JOIN par.criterio 				c   ON c.crtid = pon.crtid
					INNER JOIN par.indicador 				i   ON i.indid = c.indid
					INNER JOIN par.area 					are ON are.areid = i.areid
					INNER JOIN par.dimensao 				d   ON d.dimid = are.dimid
				WHERE 	
					{$whereSubacao}
					pp.prpid = $prpid 
					AND s.sbastatus = 'A' 
					AND pp.prpstatus = 'A'
			) as foo ORDER BY foo.codigo";
                                        
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso,
											case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
										from	par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpid = $prpid and p.prpstatus = 'A'" );

	$sql = "SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
			          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
			FROM  par.entidade ent
			INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_PREFEITURA."
			INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
			where 
				ent.dutid = ".DUTID_PREFEITO." AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent.inuid = ".$arDadosProcesso['inuid'];

	$arrSecretaria = $db->pegaLinha($sql);

	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
				INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpstatus = 'A' and p.prpid = $prpid";

	$arCronograma = $db->pegaLinha( $sql );

	if( $_SESSION['par']['cronogramainicial'] ){
		$arCronograma['cronogramainicial'] = $_SESSION['par']['cronogramainicial'];
	}
	if( $_SESSION['par']['cronogramaFinal'] ){
		$arCronograma['cronogramafinal'] = $_SESSION['par']['cronogramaFinal'];
	}
        
	#Buscar primeira data de vigencia
	$sqlDataInicioVigencia = "SELECT dopdatainiciovigencia FROM par.documentopar WHERE prpid = $prpid AND dopstatus <> 'E' ORDER BY dopid asc LIMIT 1";  
	$arCronograma['cronogramainicial'] = $db->pegaUm($sqlDataInicioVigencia);  
	
	#Recupera Empenhos somado aos reforco  
	$arrEmpenho = Array();
	if( is_array($arrSub) && $arrSub[0] ){
		$arrEmpenho = getEmpenhoAgrupadoSubacao($arrSub);
	}   

	$sql = "SELECT DISTINCT
				u.usunome as prefeito, u.usucpf as cpf, u.usuemail
			FROM
				par.usuarioresponsabilidade ur
			    inner join seguranca.usuario u on u.usucpf = ur.usucpf
			    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
			WHERE
				ur.pflcod = ".PAR_PERFIL_PREFEITO."
                and us.sisid = 23
			    and ur.muncod = (select muncod from par.processopar where prpid = $prpid and prpstatus = 'A')
			    and ur.rpustatus = 'A'
			    and us.suscod = 'A'";
	$arPrefeito = $db->pegaLinha( $sql );

	$cont = 1;
	$exerc = substr($arDadosProcesso['prpnumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['prpnumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA PREFEITURA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
		if( !empty($iueid) ){
			$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
			$tabela .= $tabelaEntidade['tabela'];
			$cont = $tabelaEntidade['cont'];
		}
								
		$tabela .= '<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) PREFEITO(A)</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arPrefeito['prefeito'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arPrefeito['cpf']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	
	$colspan = 4;
	
	$arrModid = Array(29,51,64,45,35,69,81);
	
	if( in_array($mdoid, $arrModid) ){ #PAR_Termo de Compromisso_Municipios_PROINFÂNCIA_Mob_e_Equip
		$colspan = 3;

		$sql = "SELECT DISTINCT
					sov.preid,
					oi.obrid,
					oi.numconvenio,
					oi.obranoconvenio,
					p.pronumeroprocesso as processopar,
					pp.pronumeroprocesso as processopac,
					obrnumprocessoconv as processooutro,
					d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || sbaordem as codigo
				FROM
					par.subacaoobravinculacao sov
				INNER JOIN obras2.obras 					oi   ON oi.obrid = sov.obrid
				INNER JOIN par.subacaodetalhe 				sd   ON sd.sbaid = sov.sbaid and sd.sbdano = sov.sovano
				INNER JOIN par.subacao 						s    ON s.sbaid = sd.sbaid
				INNER JOIN par.acao 	 					a    ON a.aciid  = s.aciid AND a.acistatus = 'A'
				INNER JOIN par.pontuacao 					pont ON pont.ptoid  = a.ptoid AND pont.ptostatus = 'A'
				INNER JOIN par.criterio  					c    ON c.crtid  = pont.crtid AND c.crtstatus = 'A'
				INNER JOIN par.indicador 					i    ON i.indid  = c.indid AND i.indstatus = 'A'
				INNER JOIN par.area 	 					ar   ON ar.areid = i.areid AND ar.arestatus = 'A'
				INNER JOIN par.dimensao  					d    ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
				LEFT  JOIN par.processoobrasparcomposicao 	pop  ON pop.preid = oi.preid  and pop.pocstatus = 'A'
				LEFT  JOIN par.processoobraspar 			p    ON p.proid = pop.proid and p.prostatus = 'A'
				LEFT  JOIN par.processoobraspaccomposicao 	poc  ON poc.preid = oi.preid and poc.pocstatus = 'A' 
				LEFT  JOIN par.processoobra 				pp   ON pp.proid = poc.proid and pp.prostatus = 'A' 
				WHERE 
					{$whereSubacao}
					oi.obrstatus = 'A'
				ORDER BY
					codigo";
// ver($sql,d);
		$arrSubVinc = $db->carregar($sql);
		$arrSubVinc = $arrSubVinc ? $arrSubVinc : array();
		$obrpar = false;
		$obrpac = false;
		$t = 0;

			foreach ($arrSubVinc as $key => $valor) {

				$obrnumprocessoconv = trim($valor['processooutro']);
				$obrnumprocesso = $valor['processopar'] ? $valor['processopar'] : $valor['processopac'];
				$obrpar = $valor['processopar'] ? true : false;
				$obrpac = $valor['processopac'] ? true : false;
				if( $key == 0 && !empty( $obrnumprocessoconv ) ){
					$t++;
					$tabela.= '<tr style="text-align: center;">
									<td colspan="3"><b>Dados da Obra Atendida</b></td>
								</tr>
								<tr>
									<td colspan="3" valign="top">
									<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
											<tr align="center" valign="top">
												<td width="10%"><b>ID OBRA ATENDIDA</b></td>
												<td width="40%"><b>Nº PROCESSO DA OBRA ATENDIDA</b></td>
												<td width="40%"><b>Nº CONVÊNIO/ANO DA OBRA ATENDIDA</b></td>
												<td width="10%"><b>SUBAÇÃO</b></td>
											</tr>';
				} elseif( $key == 0 && ($obrpar || $obrpac) ){
					$t++;
					$tabela.= '<tr style="text-align: center;">
									<td colspan="3"><b>Dados da Obra Atendida</b></td>
								</tr>
								<tr>
									<td colspan="3" valign="top">
									<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
											<tr align="center" valign="top">
												<td width="20%"><b>ID OBRA ATENDIDA</b></td>
												<td width="40%"><b>Nº PROCESSO DA OBRA ATENDIDA</b></td>
												<td width="10%"><b>SUBAÇÃO</b></td>
											</tr>';

				}
				if($obrnumprocessoconv){
					$tabela .= '	<tr>
										<td>'.$valor['obrid'].'</td>
										<td>'.$obrnumprocessoconv.'</td>
										<td>'.$valor['numconvenio'].'/'.$valor['obranoconvenio'].'</td>
										<td>'.$valor['codigo'].'</td>
									</tr>';
				} elseif( ($obrpar || $obrpac) ){
					$tabela .= '	<tr>
										<td>'.$valor['obrid'].'</td>
										<td>'.$obrnumprocesso.'</td>
										<td>'.$valor['codigo'].'</td>
									</tr>';
				}

		}
		if( $t > 0 ){
			$tabela.= '		</table>
					</td>
				</tr>';
		}
	}
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="50%"><b>Tipo</b></td>
									<td width="10%"><b>Tipo de Subação</b></td>';
		if($mdoid <> 29 && $mdoid <> 69) $tabela.= '<td width="10%"><b>ARP*</b></td>';
						$tabela .= '<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="15%"><b>Preço Unitário</b></td>
									<td width="15%"><b>Total</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		foreach ($arDadosItem as $key => $valor) {

			if( $valor['quantidade'] > 0 && $valor['valor'] > 0 ){

				$sql = "SELECT
							case when count(pic.picid) > 0 then 'Sim' else 'Não' end as arp
						FROM
							par.propostaitemcomposicao pic
							INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
							INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$arDadosProcesso['estuf']}'
						WHERE
							pic.picstatus = 'A' AND pic.picid = {$valor['picid']}";
				$arrARP = $db->pegaUm( $sql );

				$totalQTD += (float)$valor['quantidade'];
				$totalVLR += (float)$valor['valor'];
				$totalTOT += (float)$valor['total'];
				$tabela .= '	<tr>
									<td>'.$valor['codigo'].'</td>
									<td>'.$valor['picdescricao'].'</td>
									<td>'.$valor['ptsdescricao'].'</td>';
		if($mdoid <> 29 && $mdoid <> 69) $tabela.= '<td>'.$arrARP.'</td>';
						$tabela .= '<td style="text-align: center;">'.$valor['quantidade'].'</td>
									<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
									<td style="text-align: right;"><b>R$ '.($valor['total'] ? number_format($valor['total'],2,",",".") : '').'</b></td>
								</tr>';
			}
		}

			$_SESSION['par']['totalVLR'] = $totalTOT;
			$tabela .= '
							<tr>
								<td align="center" colspan="'.$colspan.'"><b>Total Geral</b></td>
								<td style="text-align: center;">'.$totalQTD.'</td>
								<td style="text-align: right;">R$ '.number_format($totalVLR,2,",",".").'</td>
								<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
							</tr>
						</table>
					</td>
				</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
									<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
								</tr>
							</table>
						</td>
					</tr>';
	//				if( $mdoid != 41 ){
						$tabela .= '
					<tr style="text-align: left;">
						<td colspan="3">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: center;">
									<td colspan="3"><b>EMPENHOS</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="10%"><b>Subação</b></td>
									<td width="45%"><b>Número</b></td>
									<td width="45%"><b>Valor</b></td>
								</tr>';
					$totalEmp = 0;
					foreach ($arrEmpenho as $v) {
						$totalEmp += (float)$v['valor'];
						$tabela.='<tr valign="top">
									<td width="10%" align="right">'.$v['codigo'].'</td>
									<td width="45%" align="center">'.$v['empnumero'].'</td>
									<td width="45%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
								</tr>';
					}

					$tabela.= '<tr>
									<td align="center" colspan=2><b>Total Empenho</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
									<td></td>
								</tr>
							</table>
						</td>
					</tr>';
		//			}
					$tabela .= '
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – ETAPAS OU FASES (SE HOUVER)</b></td>
					</tr>
					</table>';
					$retorno = montaTabelaRafComplementoSubacao( $prpid );
//ver($retorno, d);
					$tabela .= $retorno['tabela'];
					$total = $totalEmp+$retorno['total'];

	if($mdoid <> 29 && $mdoid <> 69) $tabela .= '(*) Item a ser adquirido por adesão à ata de registro de preços do FNDE<br><br>';

	if( $tpdcod == 102 || $tpdcod == 21 ) enviaEmailDocGeradoPar($arPrefeito['usuemail'], 'M', $prpid, $mdoid);
	return $tabela;
}

function montaTabelaDelimitacaoAcoesFinanceiraEstados( $prpid, $arrSub, $mdoid, $tpdcod, $iueid = '' ){
	global $db;
	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}
	
	$sql = "SELECT
				foo.codigo,
                foo.picdescricao,
                foo.picid,
                foo.sbdano,
                foo.sbaid,
                foo.quantidade,
                foo.valor,
                ( foo.quantidade * foo.valor ) as total
			FROM (
				SELECT 
					pic.picdescricao,
					pts.ptsdescricao,
					pic.picid,
					sd.sbdano as sbdano,
					s.sbaid,
					par.recuperaquantidadeitemvalidado( sic.icoid )	AS quantidade,
					sic.icovalor as valor,
					d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo	
				FROM par.processopar pp
					INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
					INNER JOIN par.subacaodetalhe 		sd  ON sd.sbdid = ppc.sbdid
					INNER JOIN par.subacao       		s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
					INNER  JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND icostatus = 'A'
					INNER JOIN par.propostaitemcomposicao   pic ON pic.picid = sic.picid
					LEFT JOIN par.propostasubacao 		pps ON pps.ppsid = s.ppsid		
					LEFT JOIN par.propostatiposubacao	pts ON pts.ptsid = pps.ptsid
					INNER JOIN par.acao 					a   ON a.aciid = s.aciid
					INNER JOIN par.pontuacao 				pon ON pon.ptoid = a.ptoid
					INNER JOIN par.criterio 				c   ON c.crtid = pon.crtid
					INNER JOIN par.indicador 				i   ON i.indid = c.indid
					INNER JOIN par.area 					are ON are.areid = i.areid
					INNER JOIN par.dimensao 				d   ON d.dimid = are.dimid
				WHERE 	
					{$whereSubacao}
					pp.prpid = $prpid 
					AND s.sbastatus = 'A' 
					AND pp.prpstatus = 'A'
			) as foo ORDER BY foo.codigo";
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	
	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso,
											case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
										from par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpstatus = 'A' and p.prpid = $prpid" );

	$sql = "SELECT 
			ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
			          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario, ent.entemail
			FROM  par.entidade ent
			INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
			INNER JOIN territorios.estado est on est.estuf = ent2.estuf
			LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
			where 
				ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent.inuid = ".$arDadosProcesso['inuid'];
	
	$arrSecretaria = $db->pegaLinha( $sql );

	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
				INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = $prpid and p.prpstatus = 'A'";

	$arCronograma = $db->pegaLinha( $sql );

	if( $_SESSION['par']['cronogramainicial'] ){
		$arCronograma['cronogramainicial'] = $_SESSION['par']['cronogramainicial'];
	}
	if( $_SESSION['par']['cronogramaFinal'] ){
		$arCronograma['cronogramafinal'] = $_SESSION['par']['cronogramaFinal'];
	}
        
        #Buscar primeira data de vigencia
        $sqlDataInicioVigencia = "SELECT dopdatainiciovigencia FROM par.documentopar WHERE prpid = $prpid AND dopstatus <> 'E' ORDER BY dopid asc LIMIT 1";  
        $arCronograma['cronogramainicial'] = $db->pegaUm($sqlDataInicioVigencia);  

	#Recupera Empenhos somado aos reforco
	$arrEmpenho = Array();
	if( is_array($arrSub) && $arrSub[0] ){
		$arrEmpenho = getEmpenhoAgrupadoSubacao($arrSub);
	}  
	
	$cont = 1;
	$exerc = substr($arDadosProcesso['prpnumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
	$tabela .= 		'<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO <br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['prpnumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA DE EDUCAÇÃO DO ESTADO</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 		'<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	
	if($mdoid == 29 || $mdoid == 69){ #PAR_Termo de Compromisso_Municipios_PROINFÂNCIA_Mob_e_Equip
//		$sql = "select
//					sov.obrid,
//					oi.obrnumprocessoconv,
//				    oi.numconvenio,
//				    oi.obranoconvenio
//				from
//					par.subacaoobravinculacao sov
//				    inner join obr as.ob rainfraestrutura oi on oi.obrid = sov.obrid
//				    inner join par.subacaodetalhe sd on sd.sbaid = sov.sbaid and sd.sbdano = sov.sovano
//				where
//					{$whereSubacao}
//					oi.obsstatus = 'A'";
		$sql = "SELECT
					sov.obrid,
					oi.obrnumprocessoconv,
				    oi.numconvenio,
				    oi.obranoconvenio
				FROM
					par.subacaoobravinculacao sov
			    INNER JOIN obras2.obras 		oi ON oi.obrid = sov.obrid
			    INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = sov.sbaid AND sd.sbdano = sov.sovano
				WHERE
					{$whereSubacao}
					oi.obrstatus = 'A'";
		$arrSubVinc = $db->carregar($sql);
		$arrSubVinc = $arrSubVinc ? $arrSubVinc : array();

		foreach ($arrSubVinc as $key => $valor) {

			$obrnumprocessoconv = trim($valor['obrnumprocessoconv']);
			if( $key == 0 && !empty( $obrnumprocessoconv ) ){
				$tabela.= '<tr style="text-align: center;">
								<td colspan="3"><b>Dados da Obra Atendida</b></td>
							</tr>
							<tr>
								<td colspan="3" valign="top">
								<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
										<tr align="center" valign="top">
											<td width="20%"><b>ID OBRA ATENDIDA</b></td>
											<td width="40%"><b>Nº PROCESSO DA OBRA ATENDIDA</b></td>
											<td width="40%"><b>Nº CONVÊNIO/ANO DA OBRA ATENDIDA</b></td>
										</tr>';
			}
			if($obrnumprocessoconv){
				$tabela .= '	<tr>
									<td>'.$valor['obrid'].'</td>
									<td>'.$obrnumprocessoconv.'</td>
									<td>'.$valor['numconvenio'].'/'.$valor['obranoconvenio'].'</td>
								</tr>';
			}
		}
		if( $arrSubVinc ){
			$tabela.= '		</table>
					</td>
				</tr>';
		}
	}
	$tabela .= '<tr style="text-align: center;">
					<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
				</tr>';

	$tabela.=	'<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
				</tr>
				<tr style="text-align: left;" valign="top">
					<td colspan="3">
						<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr style="text-align: left;" valign="top">
								<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
								<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
							</tr>
						</table>
					</td>
				</tr>';

	$totalQTD = 0;
	$totalVLR = 0;
	$totalTOT = 0;

	$tabelaBuffer .= '<tr>
						<td colspan="3" valign="top">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%" colspan="6"><b>Itens</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="55%"><b>Tipo</b></td>
									<td width="10%"><b>ARP*</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="10%"><b>Preço Unitário</b></td>
									<td width="10%"><b>Total</b></td>
								</tr>';

	foreach ($arDadosItem as $key => $valor) {

		if( $valor['quantidade'] > 0 && $valor['valor'] > 0 ){
			$sql = "SELECT
						case when count(pic.picid) > 0 then 'Sim' else 'Não' end as arp
					FROM
						par.propostaitemcomposicao pic
						INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
						INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$arDadosProcesso['estuf']}'
					WHERE
						pic.picstatus = 'A' AND pic.picid = {$valor['picid']}";
			$arrARP = $db->pegaUm( $sql );

			$totalQTD += (float)$valor['quantidade'];
			$totalVLR += (float)$valor['valor'];
			$totalTOT += (float)$valor['total'];
			$tabelaBuffer .= '	<tr>
								<td>'.$valor['codigo'].'</td>
								<td>'.$valor['picdescricao'].'</td>
								<td>'.$arrARP.'</td>
								<td style="text-align: center;">'.$valor['quantidade'].'</td>
								<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
								<td style="text-align: right;"><b>R$ '.($valor['total'] ? number_format($valor['total'],2,",",".") : '').'</b></td>
							</tr>';
		}
	}
	$_SESSION['par']['totalVLR'] = $totalTOT;
	$tabelaBuffer .= '
					<tr>
						<td align="center" colspan="3"><b>Total Itens</b></td>
						<td style="text-align: center;">'.$totalQTD.'</td>
						<td style="text-align: right;">R$ '.number_format($totalVLR,2,",",".").'</td>
						<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
					</tr>
				</table>';

	if( $totalVLR > 0 ){
		$tabela = $tabela.$tabelaBuffer;
	}
//	if($mdoid != 42){
		$tabela .= '
				<tr style="text-align: left;">
					<td colspan="3">
					<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
						<tr style="text-align: center;">
							<td colspan="3"><b>EMPENHOS</b></td>
						</tr>
						<tr align="center" valign="top">
							<td width="05%"><b>Número</b></td>
							<td width="60%"><b>Valor</b></td>
						</tr>';
		$totalEmp = 0;
		foreach ($arrEmpenho as $v) {
			$totalEmp += (float)$v['valor'];
			$tabela.='	<tr valign="top">
							<td width="50%" align="center">'.$v['empnumero'].'</td>
							<td width="50%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
						</tr>';
		}

		$tabela.= '		<tr>
							<td align="center"><b>Total Empenho</b></td>
							<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
						</tr>
					</table>
				</td>
			</tr>';
//	}
	$retorno = montaTabelaRafComplementoSubacao( $prpid );
// 	ver($retorno,d);
	$tabela .= $retorno['tabela'];
// 	$total = $totalTOT+$retorno['total'];
// 	$total = $retorno['total'];
	$total = $totalEmp+$retorno['total'];
// 	$tabela.=	'
// 			<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
// 				<tr>
// 					<td align="center" ><b>Total</b></td>
// 					<td style="text-align: right;" width="10%"><b>R$ '.number_format($total,2,",",".").'</b></td>
// 				</tr>
// 			</table>';
// 	$tabela .= '
// 			<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
// 				<tr style="text-align: left;">
// 					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – ETAPAS OU FASES (SE HOUVER)</b></td>
// 				</tr>
// 			</table>
// 			';
	$tabela .= '</td></tr></table>(*) Item a ser adquirido por adesão à ata de registro de preços do FNDE<br><br>';
	if( $tpdcod == 102 || $tpdcod == 21 ) enviaEmailDocGeradoPar($arrSecretaria['entemail'], 'E', $prpid, $mdoid);
	return $tabela;
}

function carregaTabelaEntidadeExecutora( $iueid, $cont = 1 ){
	global $db;
	
	$sql = "SELECT
				iue.iuenome as universidade,
				iue.iuecnpj as cnpjuniversidade,
                iuelog, 
                iuebai, 
                iuenum,
                m.mundescricao,
                iue.iueendestuf as estuf
			FROM par.instrumentounidadeentidade iue
			LEFT JOIN territorios.municipio m on m.muncod = iue.iueendmuncod
			WHERE 
				iue.iueid = ".$iueid;
	
	$arrUniversidade = $db->pegaLinha($sql);
	
	$tabela = '<tr>
				<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA EXECUTORA</b><br>'.$arrUniversidade['universidade'].'</td>
				<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrUniversidade['cnpjuniversidade']).'</td>
			</tr>
			<tr>
				<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO EXECUTORA</b><br>'.$arrUniversidade['iuelog'].' '.$arrUniversidade['iuenum'].' - '.$arrUniversidade['iuebai'].'</td>
				<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO EXECUTORA</b><br>'.$arrUniversidade['mundescricao'].'</td>
				<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF EXECUTORA</b><br>'.$arrUniversidade['estuf'].'</td>
			</tr>';
	
	return array('tabela' => $tabela, 'cont' => $cont);
}

function carregaTabelaExecutor( $iueid, $cont = 1 ){
	global $db;
	
	$arrReitor = carregarExecutor( $iueid );
	
	$tabela = '<tr style="text-align: left;" valign="top">
					<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DO REITOR(A)</b><br>'.$arrReitor['reitor'].'</td>
					<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrReitor['cpfreitor']).'</td>
			   </tr>';
	
	return array('tabela' => $tabela, 'cont' => $cont);
}

function carregarExecutor( $iueid ){
	global $db;
	
	$sql = "select u.usucpf as cpfreitor, 
				u.usunome as reitor
		    from par.usuarioresponsabilidade ur
		        inner join seguranca.usuario 			u   on u.usucpf = ur.usucpf
		        inner join seguranca.usuario_sistema 	us on us.usucpf = u.usucpf and us.sisid = 23
		    where
		    	ur.iueid = $iueid
		        and us.suscod = 'A'
		    	and ur.rpustatus = 'A'
		    	and ur.pflcod = ".PAR_PERFIL_ENTIDADE_EXECUTORA;
	
	$arrReitor = $db->pegaLinha($sql);
	
	return $arrReitor;
}

function montaTabelaTermo_Compromisso_Brasil_Pro( $proid, $mdoid, $arrObra, $iueid = '' ){

	global $db;

	if( is_array($arrObra) && $arrObra[0] ){
		$whereObras = " AND p.preid in (".implode(',', $arrObra).") ";
	}

	$sql = "SELECT DISTINCT
				iu.itrid, iu.inuid, p.pronumeroprocesso,
				case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
			FROM par.processoobraspar p
			INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
			WHERE
				p.prostatus = 'A' and
				p.proid = $proid";

	$arDadosProcesso = $db->pegaLinha( $sql );
/*
	$sql = "SELECT
                iu2.estuf,
		        u.usucpf as cpfreitor,
		        iue.entid,
		        u.usunome as reitor,
		        iue.iuenome,
		        iue.iuecnpj,
		        ent2.entnome as universidade,
		        ent2.entnumcpfcnpj as cnpjuniversidade,
		        end2.endlog,
		        end2.endcep,
		        end2.endnum,
		        end2.endbai,
                mun.mundescricao,
                mun.estuf,
                mun.muncod
			FROM
			-- Entidade
				entidade.entidade ent
			INNER JOIN entidade.funcaoentidade 			fen  ON fen.entid = ent.entid AND fen.funid = 25
			INNER JOIN entidade.funentassoc         	fea  ON fea.fueid = fen.fueid
			INNER JOIN entidade.entidade                ent2 ON ent2.entid = fea.entid
			INNER JOIN entidade.funcaoentidade 			fen2 ON fen2.entid = ent2.entid and fen2.funid = 6
			INNER JOIN entidade.endereco                end2 ON end2.entid = ent2.entid AND end2.tpeid = 1
																AND end2.endid in ( SELECT max(endid)
																					FROM entidade.endereco
																					WHERE entid = ent2.entid AND endstatus = 'A')
			INNER JOIN par.instrumentounidade    		iu   ON iu.estuf = end2.estuf
			INNER JOIN par.instrumentounidadeentidade	iue  ON ent2.entid = iue.entid AND iue.itrid = 3
			INNER JOIN par.processoobraspar 			pp   ON iue.inuid = pp.inuid and pp.prostatus = 'A'
			INNER JOIN territorios.estado               est  ON est.estuf = iu.estuf
			LEFT  JOIN territorios.municipio    		mun  ON mun.muncod = est.muncodcapital
			--responsavel
			INNER JOIN par.instrumentounidade        	iu2  ON iu2.inuid = pp.inuid
			LEFT  JOIN par.usuarioresponsabilidade    	urs  ON urs.estuf = iu2.estuf AND pflcod = 839 AND rpustatus = 'A'
			LEFT  JOIN seguranca.usuario            	u    ON u.usucpf = urs.usucpf
			WHERE
                ent.entstatus='A' AND
                ent2.entstatus='A' AND
                pp.proid = ".$proid;
*/
	$sql = "SELECT u.usucpf as cpfreitor, iue.entid, u.usunome as reitor, iue.iuenome, iue.iuecnpj,
				e.entnome as universidade, e.entnumcpfcnpj as cnpjuniversidade, ed.endlog, ed.endcep, ed.endnum,
				ed.endbai, mun.mundescricao, mun.estuf, mun.muncod
			FROM par.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario 				u   on u.usucpf = ur.usucpf
			INNER JOIN par.instrumentounidadeentidade 	iue on iue.entid = ur.entid and iuedefault = false
			INNER JOIN entidade.entidade 				e   on iue.entid = e.entid
			INNER JOIN entidade.endereco 				ed 	on ed.entid = e.entid and ed.endstatus = 'A'
			INNER JOIN territorios.estado 				est on est.estuf = ed.estuf
			LEFT  JOIN territorios.municipio 			mun on mun.muncod = est.muncodcapital
			--INNER JOIN par.processopar 					pp  on pp.prpcnpj = iue.iuecnpj and pp.inuid = iue.inuid and pp.prpstatus = 'A'
			INNER JOIN par.processoobraspar 			pp   ON iue.inuid = pp.inuid and pp.prostatus = 'A'
			WHERE
				rpustatus = 'A'
				AND ur.pflcod = 1141
				AND pp.proid = ".$proid;
	
	$arrUniversidade = $db->pegaLinha( $sql );
 	//ver($sql,d);
	$sql = "SELECT
				ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum,
				ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod, ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
			FROM  par.entidade ent
			INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
			INNER JOIN territorios.estado est on est.estuf = ent2.estuf
			LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
			where 
				ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent.inuid = ".$arDadosProcesso['inuid'];
	$arrSecretaria = $db->pegaLinha( $sql );

	$sql = "SELECT DISTINCT
				emp.empnumero,
				sum(p.eobvalorempenho) as valor
			FROM par.empenhoobrapar  p
			INNER JOIN par.empenho emp on emp.empid = p.empid  and eobstatus = 'A' and empstatus <> 'I'
			INNER JOIN par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			group by emp.empnumero";
                                
	if( $whereObras ){
		$arrEmpenho = $db->carregar($sql);
	}
	$cont = 1;
	$exerc = substr($arDadosProcesso['pronumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
	$tabela .= '	<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO <br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS - BRASIL PROFISSIONALIZADO</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['pronumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO SECRETARIA</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO SECRETARIA</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF SECRETARIA</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DOS DIRIGENTES</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DO SECRETÁRIO(A)</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= '	<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="30%"><b>Ação(Nome da Obra)</b></td>
									<td width="30%"><b>Tipo Obra</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="15%"><b>Valor(R$)</b></td>
								</tr>';

	if( is_array($arrObra) && $arrObra[0] ){
		$whereObras = " AND po.preid in (".implode(',', $arrObra).") ";
	}

	$sql = "(SELECT DISTINCT
				po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
				po.prebairro, po.precep,
				po.prelogradouro, mun.mundescricao, est.estdescricao, s.sbaid, d.dimcod || '.' || are.ardcod || '.' || i.indcod || '.' || sbaordem as codigo
			FROM par.empenhoobrapar  p
			INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
			INNER JOIN cte.subacaoobra so ON so.preid = po.preid
			INNER JOIN cte.subacaoindicador s ON s.sbaid = so.sbaid
			INNER JOIN cte.acaoindicador a ON a.aciid = s.aciid
			INNER JOIN cte.pontuacao pon ON pon.ptoid = a.ptoid
			INNER JOIN cte.criterio c on c.crtid = pon.crtid
			INNER JOIN cte.indicador i on i.indid = c.indid
			INNER JOIN cte.areadimensao are on are.ardid = i.ardid
			INNER JOIN cte.dimensao d on d.dimid = are.dimid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
			INNER JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			LEFT  JOIN territorios.municipio mun on mun.muncod = po.muncod
			LEFT  JOIN territorios.estado est on est.estuf = po.estuf
			INNER JOIN par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
			INNER JOIN par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			GROUP BY
				po.preid,
				p.eobvalorempenho,
				po.muncod,
				po.estuf,
				po.predescricao,
				po.prebairro,
				po.precep,
				po.prelogradouro,
				mun.mundescricao,
				est.estdescricao,
				tpo.ptodescricao,
				d.dimcod,
				are.ardcod,
				i.indcod,
				sbaordem,
				s.sbaid
			ORDER BY
				codigo
			)
			UNION ALL
			(
			SELECT DISTINCT
				po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
				po.prebairro, po.precep,
				po.prelogradouro, mun.mundescricao, est.estdescricao, so.sbaid, par.retornacodigosubacao(so.sbaid) as codigo
			FROM par.empenhoobrapar  p
			INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
			INNER JOIN par.subacaoobra so ON so.preid = po.preid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
			INNER JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			LEFT  JOIN territorios.municipio mun on mun.muncod = po.muncod
			LEFT  JOIN territorios.estado est on est.estuf = po.estuf
			INNER JOIN par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
			INNER JOIN par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			GROUP BY
				po.preid,
				p.eobvalorempenho,
				po.muncod,
				po.estuf,
				po.predescricao,
				po.prebairro,
				po.precep,
				po.prelogradouro,
				mun.mundescricao,
				est.estdescricao,
				tpo.ptodescricao,
				--sbaordem,
				so.sbaid
			ORDER BY
				codigo
			)";
// 	ver($sql,d);
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$totalQTD = 0;
	$totalVLR = 0;
	$totalTOT = 0;
	$arrLocalizacao = array();
	$arrPreid = array();
	foreach ($arDadosItem as $key => $valor) {
		//$totalQTD += (float)$valor['quantidade'];
		//$totalVLR += (float)$valor['valor'];
		if(!in_array($valor['preid'], $arrPreid)){
			$arrPreid[] = $valor['preid'];


			if( $valor['valor'] > 0 ){
				$sql = "SELECT
							d.dimcod || '.' || are.ardcod || '.' || i.indcod || '.' || sbaordem
						FROM
							cte.subacaoindicador s
						INNER JOIN cte.acaoindicador a ON a.aciid = s.aciid
						INNER JOIN cte.pontuacao pon ON pon.ptoid = a.ptoid
						INNER JOIN cte.criterio c on c.crtid = pon.crtid
						INNER JOIN cte.indicador i on i.indid = c.indid
						INNER JOIN cte.areadimensao are on are.ardid = i.ardid
						INNER JOIN cte.dimensao d on d.dimid = are.dimid
						WHERE
							s.sbaid = {$valor['sbaid']}";
				$subacao = $db->pegaUm($sql);
				$localizacao = 'Bairro: '.$valor['prebairro'].', '.'Logradouro: '.$valor['prelogradouro'].', '.'Cidade: '.$valor['mundescricao'].'.<br>';

				$arrLocalizacao[] = array(
						'obras' => $valor['predescricao'],
						'localizacao' => $localizacao
						);
				$totalTOT += (float)$valor['valor'];
				$tabela .= '	<tr>
									<td>'.$subacao.'</td>
									<td>'.$valor['predescricao'].'</td>
									<td>'.$valor['ptodescricao'].'</td>
									<td style="text-align: center;">1</td>
									<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
								</tr>';
			}
			}
			}
			$_SESSION['par']['totalVLR'] = $totalTOT;
			$tabela .= '
								<tr>
									<td align="center" colspan=4><b>Total Geral</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
								</tr>
							</table>
						</td>
					</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – LOCALIZAÇÃO</b><br>
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="left" valign="top">
									<td><b>Nome da Obra</b></td>
									<td><b>Endereço</b></td>
								</tr>';
						foreach ($arrLocalizacao as $ende) {
							$tabela.= '<tr align="left" valign="top">
											<td>'.$ende['obras'].'</td>
											<td>'.$ende['localizacao'].'</td>
										</tr>';
						}

			$tabela .= '	</table>
						</td>
					</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.date('m/Y').'</td>
									<td width="50%"><b>Mês Final:</b><br>06/2014</td>
								</tr>
							</table>
						</td>
					</tr>';
			$tabela .= '
					<tr style="text-align: left;">
						<td colspan="3">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: center;">
									<td colspan="3"><b>EMPENHOS</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="05%"><b>Número</b></td>
									<td width="60%"><b>Valor</b></td>
								</tr>';
			$totalEmp = 0;
			foreach ($arrEmpenho as $v) {
				$totalEmp += (float)$v['valor'];
				$tabela.='		<tr valign="top">
									<td width="50%" align="center">'.$v['empnumero'].'</td>
									<td width="50%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
								</tr>';
			}

			$tabela.= '			<tr>
									<td align="center"><b>Total Empenho</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
								</tr>
							</table>
						</td>
					</tr>';
	$tabela .= '</table><br><br>';
// 	ver(simec_htmlentities($tabela),d);
	return $tabela;
}

function montaTabelaTermoCompromissoUniversidadesPacto( $prpid, $arrSub, $mdoid, $iueid = '' ){
	global $db;

	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "SELECT

			    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
			    sub.sbadsc as subacao,
			    (SELECT cast(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid , sd.sbdano) as numeric(20,2) ) ) AS valorsubacao,
				(select array_to_string(array(select es1.empnumero from par.empenho es1  
                            								inner join par.empenhosubacao es2 on es2.empid = es1.empid
                            							  where es2.sbaid = sd.sbaid
                            							    and es1.empstatus <> 'I'
                                                          	and es2.eobano = sd.sbdano
                                                            and es2.eobstatus = 'A'
                                                            and es1.empsituacao ilike '%efetivado%'), ', ') ) as empenho,
                                                            prg.prgdsc
                                                            
						FROM
							par.processopar prp
						    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
						    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
						    inner join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
						    --inner join par.empenho emp on emp.empid = ems.empid
						    inner join par.subacao sub on sub.sbaid = ems.sbaid
						    inner join par.acao a on a.aciid = sub.aciid
						    inner join par.pontuacao pon on pon.ptoid = a.ptoid
						    inner join par.criterio c on c.crtid = pon.crtid
						    inner join par.indicador i on i.indid = c.indid
						    inner join par.area are on are.areid = i.areid
						    inner join par.dimensao d on d.dimid = are.dimid
                                                    inner join par.programa prg on prg.prgid = sub.prgid
						WHERE
							$whereSubacao
							prp.prpid = $prpid
							and prp.prpstatus = 'A'
                            and sub.sbastatus = 'A'
                        GROUP BY
                        	d.dimcod, are.arecod, i.indcod, sub.sbaordem, sub.sbadsc, sd.sbaid , sd.sbdano, prg.prgdsc
						ORDER BY d.dimcod, are.arecod, i.indcod, sub.sbaordem";
//ver($sql, d);
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso,
											case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
										from par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpstatus = 'A' and p.prpid = $prpid" );

	$arrUniversidade = $db->pegaLinha("SELECT u.usucpf as cpfreitor, iue.entid, u.usunome as reitor, iue.iuenome, iue.iuecnpj,
											e.entnome as universidade, e.entnumcpfcnpj as cnpjuniversidade, ed.endlog, ed.endcep, ed.endnum,
											ed.endbai, mun.mundescricao, mun.estuf, mun.muncod
										FROM par.usuarioresponsabilidade ur
										INNER JOIN seguranca.usuario 				u   on u.usucpf = ur.usucpf
										INNER JOIN par.instrumentounidadeentidade 	iue on iue.entid = ur.entid and iuedefault = false
										INNER JOIN entidade.entidade 				e   on iue.entid = e.entid
										INNER JOIN entidade.endereco 				ed 	on ed.entid = e.entid and ed.endstatus = 'A'
										INNER JOIN territorios.estado 				est on est.estuf = ed.estuf
										LEFT  JOIN territorios.municipio 			mun on mun.muncod = est.muncodcapital
										INNER JOIN par.processopar 					pp  on pp.prpcnpj = iue.iuecnpj and pp.inuid = iue.inuid and pp.prpstatus = 'A'
										WHERE
											rpustatus = 'A'
											AND ur.pflcod = 853
											AND pp.prpid = $prpid");
// 	ver($sql,d);
	$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum,
											ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod, ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
										FROM  par.entidade ent
										INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
										INNER JOIN territorios.estado est on est.estuf = ent2.estuf
										LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
										where 
											ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
											ent.entstatus='A' AND
											ent2.entstatus='A' AND 
											ent.inuid = ".$arDadosProcesso['inuid']);

	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
				INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = $prpid and p.prpstatus = 'A'";

	$arCronograma = $db->pegaLinha( $sql );
        
        # Caso possua em sessao a data inicial e final essas serao carregadas no termo de compromisso
        if( $_SESSION['par']['cronogramainicial'] ){
		$arCronograma['cronogramainicial'] = $_SESSION['par']['cronogramainicial'];
	}
	if( $_SESSION['par']['cronogramaFinal'] ){
		$arCronograma['cronogramafinal'] = $_SESSION['par']['cronogramaFinal'];
	}
        
        #Buscar primeira data de vigencia
        $sqlDataInicioVigencia = "SELECT dopdatainiciovigencia FROM par.documentopar WHERE prpid = $prpid AND dopstatus <> 'E' ORDER BY dopid asc LIMIT 1";  
        $arCronograma['cronogramainicial'] = $db->pegaUm($sqlDataInicioVigencia);  

	#Recupera Empenhos somado aos reforco
	$arrEmpenho = Array();
	if( is_array($arrSub) && $arrSub[0] ){
		$arrEmpenho = getEmpenhoAgrupadoSubacao($arrSub);
	}  
        
        
        /**
         * Concatenar nomes dos programas
         */
        $strNomePrograma = '';
        foreach ($arDadosItem as $value) {
            $strNomePrograma .= $value['prgdsc'].'<br/>' ;
        }

	$cont = 1;
	$exerc = substr($arDadosProcesso['prpnumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO <br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>'.$strNomePrograma.'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['prpnumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO SECRETARIA</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO SECRETARIA</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF SECRETARIA</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DOS DIRIGENTES</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DO SECRETÁRIO(A)</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}

		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="10%"><b>Localização</b></td>
									<td width="60%"><b>Subação</b></td>
									<td width="15%"><b>Valor da Subação</b></td>
									<td width="15%"><b>Empenhos</b></td>
								</tr>';
		$totalVLR = 0;

		foreach ($arDadosItem as $key => $valor) {
			$totalVLR += (float) $valor['valorsubacao'];
			$tabela .= '<tr>
							<td>'.$valor['codigo'].'</td>
							<td>'.$valor['subacao'].'</td>
							<td style="text-align: right;">R$ '.($valor['valorsubacao'] ? number_format($valor['valorsubacao'],2,",",".") : '').'</td>
							<td>'.$valor['empenho'].'</td>
						</tr>';
		}
			$_SESSION['par']['totalVLR'] = $totalVLR;
			$tabela .= '
							<tr>
								<td align="center" colspan="2"><b>Total Geral</b></td>
								<td style="text-align: right;">R$ '.number_format($totalVLR,2,",",".").'</td>
								<td> </td>
							</tr>
						</table>
					</td>
				</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
									<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
								</tr>
							</table>
						</td>
					</tr>';
		$tabela .= '
					<tr style="text-align: left;">
						<td colspan="3">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: center;">
									<td colspan="2"><b>EMPENHOS</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="50%"><b>Número</b></td>
									<td width="50%"><b>Valor</b></td>
								</tr>';
					$totalEmp = 0;
					foreach ($arrEmpenho as $v) {
						$totalEmp += (float)$v['valor'];
						$tabela.='<tr valign="top">
									<td width="50%" align="center">'.$v['empnumero'].'</td>
									<td width="50%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
								</tr>';
					}

					$tabela.= '<tr>
									<td align="center"><b>Total Empenho</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
									<td></td>
								</tr>
							</table>
						</td>
					</tr>';
					// Adicionanado complemento para este tipo de documento (demanda 881 par)
					$complementos = montaTabelaRafComplementoSubacao( $prpid );
						
					$complementos = (is_array($complementos)) ? $complementos : Array();
					
					if((count($complementos > 0 ) ) && ($complementos['tabela']))
					{
						$tabela .= $complementos['tabela'];
					}
					$tabela .= '<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – ETAPAS OU FASES (SE HOUVER)</b></td>
					</tr>';
	$tabela .= '</table><br><br>';

	return $tabela;
}

function montaTabelaTermoCompromisso_BP_Executora( $prpid, $arrSub, $mdoid, $iueid = '' ){
	global $db;

	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "SELECT
				foo.codigo,
                foo.picdescricao,
                foo.picid,
                foo.sbdano,
                foo.sbaid,
                foo.quantidade,
                foo.valor,
                ( foo.quantidade * foo.valor ) as total
			FROM (
				SELECT 
					pic.picdescricao,
					pts.ptsdescricao,
					pic.picid,
					sd.sbdano as sbdano,
					s.sbaid,
					par.recuperaquantidadeitemvalidado( sic.icoid )	AS quantidade,
					sic.icovalor as valor,
					d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo	
				FROM par.processopar pp
					INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
					INNER JOIN par.subacaodetalhe 		sd  ON sd.sbdid = ppc.sbdid
					INNER JOIN par.subacao       		s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
					INNER  JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND icostatus = 'A'
					INNER JOIN par.propostaitemcomposicao   pic ON pic.picid = sic.picid
					LEFT JOIN par.propostasubacao 		pps ON pps.ppsid = s.ppsid		
					LEFT JOIN par.propostatiposubacao	pts ON pts.ptsid = pps.ptsid
					INNER JOIN par.acao 					a   ON a.aciid = s.aciid
					INNER JOIN par.pontuacao 				pon ON pon.ptoid = a.ptoid
					INNER JOIN par.criterio 				c   ON c.crtid = pon.crtid
					INNER JOIN par.indicador 				i   ON i.indid = c.indid
					INNER JOIN par.area 					are ON are.areid = i.areid
					INNER JOIN par.dimensao 				d   ON d.dimid = are.dimid
				WHERE 	
					{$whereSubacao}
					pp.prpid = $prpid 
					AND s.sbastatus = 'A' 
					AND pp.prpstatus = 'A'
			) as foo ORDER BY foo.codigo";
                                        
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso,
											case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
										from par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpid = $prpid and p.prpstatus = 'A'" );

	$arrUniversidade = $db->pegaLinha("SELECT u.usucpf as cpfreitor, iue.entid, u.usunome as reitor, iue.iuenome, iue.iuecnpj,
											e.entnome as universidade, e.entnumcpfcnpj as cnpjuniversidade, ed.endlog, ed.endcep, ed.endnum,
											ed.endbai, mun.mundescricao, mun.estuf, mun.muncod
										FROM par.usuarioresponsabilidade ur
										INNER JOIN seguranca.usuario 				u   on u.usucpf = ur.usucpf
										INNER JOIN par.instrumentounidadeentidade 	iue on iue.entid = ur.entid and iuedefault = false
										INNER JOIN entidade.entidade 				e   on iue.entid = e.entid
										INNER JOIN entidade.endereco 				ed 	on ed.entid = e.entid and ed.endstatus = 'A'
										INNER JOIN territorios.estado 				est on est.estuf = ed.estuf
										LEFT  JOIN territorios.municipio 			mun on mun.muncod = est.muncodcapital
										INNER JOIN par.processopar 					pp  on pp.prpcnpj = iue.iuecnpj and pp.inuid = iue.inuid and pp.prpstatus = 'A'
										WHERE
											rpustatus = 'A'
											AND ur.pflcod = 853
											AND pp.prpid = $prpid");
// 	ver($sql,d);
	$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum,
											ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod, ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
										FROM  par.entidade ent
										INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
										INNER JOIN territorios.estado est on est.estuf = ent2.estuf
										LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
										where 
											ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
											ent.entstatus='A' AND
											ent2.entstatus='A' AND 
											ent.inuid = ".$arDadosProcesso['inuid']);

	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
				INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = $prpid
			and p.prpstatus = 'A' ";

	$arCronograma = $db->pegaLinha( $sql );
        
        # Caso possua em sessao a data inicial e final essas serao carregadas no termo de compromisso
        if( $_SESSION['par']['cronogramainicial'] ){
		$arCronograma['cronogramainicial'] = $_SESSION['par']['cronogramainicial'];
	}
	if( $_SESSION['par']['cronogramaFinal'] ){
		$arCronograma['cronogramafinal'] = $_SESSION['par']['cronogramaFinal'];
	}

        #Buscar primeira data de vigencia
        $sqlDataInicioVigencia = "SELECT dopdatainiciovigencia FROM par.documentopar WHERE prpid = $prpid AND dopstatus <> 'E' ORDER BY dopid asc LIMIT 1";  
        $arCronograma['cronogramainicial'] = $db->pegaUm($sqlDataInicioVigencia);  
        
	#Recupera Empenhos somado aos reforco
	$arrEmpenho = Array();
	if( is_array($arrSub) && $arrSub[0] ){
		$arrEmpenho = getEmpenhoAgrupadoSubacao($arrSub);
	}  
        
	$cont = 1;
	$exerc = substr($arDadosProcesso['prpnumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO <br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS – ALFABETIZAÇÃO NA IDADE CERTA</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['prpnumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO SECRETARIA</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO SECRETARIA</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF SECRETARIA</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DOS DIRIGENTES</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DO SECRETÁRIO(A)</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}

		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="60%"><b>Tipo</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="10%"><b>Preço Unitário</b></td>
									<td width="20%"><b>Total</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		foreach ($arDadosItem as $key => $valor) {

			if( $valor['quantidade'] > 0 && $valor['valor'] > 0 ){
				$sql = "SELECT
							case when count(pic.picid) > 0 then 'Sim' else 'Não' end as arp
						FROM
							par.propostaitemcomposicao pic
							INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
							INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$arDadosProcesso['estuf']}'
						WHERE
							pic.picstatus = 'A' AND pic.picid = {$valor['picid']}";
				$arrARP = $db->pegaUm( $sql );

				$totalQTD += (float)$valor['quantidade'];
				$totalVLR += (float)$valor['valor'];
				$totalTOT += (float)$valor['total'];
				$tabela .= '	<tr>
									<td>'.$valor['codigo'].'</td>
									<td>'.$valor['picdescricao'].'</td>
									<td style="text-align: center;">'.$valor['quantidade'].'</td>
									<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
									<td style="text-align: right;"><b>R$ '.($valor['total'] ? number_format($valor['total'],2,",",".") : '').'</b></td>
								</tr>';
			}
		}
			$tabela .= '
							<tr>
								<td align="center" colspan="2"><b>Total Geral</b></td>
								<td style="text-align: center;">'.$totalQTD.'</td>
								<td style="text-align: right;">R$ '.number_format($totalVLR,2,",",".").'</td>
								<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
							</tr>
						</table>
					</td>
				</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
									<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
								</tr>
							</table>
						</td>
					</tr>';
		$tabela .= '
					<tr style="text-align: left;">
						<td colspan="3">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: center;">
									<td colspan="2"><b>EMPENHOS</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="50%"><b>Número</b></td>
									<td width="50%"><b>Valor</b></td>
								</tr>';
					$totalEmp = 0;
					foreach ($arrEmpenho as $v) {
						$totalEmp += (float)$v['valor'];
						$tabela.='<tr valign="top">
									<td width="50%" align="center">'.$v['empnumero'].'</td>
									<td width="50%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
								</tr>';
					}

					$tabela.= '<tr>
									<td align="center"><b>Total Empenho</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
									<td></td>
								</tr>
							</table>
						</td>
					</tr>';
					$tabela .= '<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – ETAPAS OU FASES (SE HOUVER)</b></td>
					</tr>';
	$tabela .= '</table><br><br>';

	return $tabela;
}

function montaTabelaTermoCompromissoUniversidadesBrasilPro( $prpid, $arrSub, $mdoid, $iueid = '' ){
	global $db;

	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "SELECT
				foo.codigo,
                foo.picdescricao,
                foo.picid,
                foo.sbdano,
                foo.sbaid,
                foo.quantidade,
                foo.valor,
                ( foo.quantidade * foo.valor ) as total
			FROM (
				SELECT 
					pic.picdescricao,
					pts.ptsdescricao,
					pic.picid,
					sd.sbdano as sbdano,
					s.sbaid,
					par.recuperaquantidadeitemvalidado( sic.icoid )	AS quantidade,
					sic.icovalor as valor,
					d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo	
				FROM par.processopar pp
					INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
					INNER JOIN par.subacaodetalhe 		sd  ON sd.sbdid = ppc.sbdid
					INNER JOIN par.subacao       		s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
					INNER  JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND icostatus = 'A'
					INNER JOIN par.propostaitemcomposicao   pic ON pic.picid = sic.picid
					LEFT JOIN par.propostasubacao 		pps ON pps.ppsid = s.ppsid		
					LEFT JOIN par.propostatiposubacao	pts ON pts.ptsid = pps.ptsid
					INNER JOIN par.acao 					a   ON a.aciid = s.aciid
					INNER JOIN par.pontuacao 				pon ON pon.ptoid = a.ptoid
					INNER JOIN par.criterio 				c   ON c.crtid = pon.crtid
					INNER JOIN par.indicador 				i   ON i.indid = c.indid
					INNER JOIN par.area 					are ON are.areid = i.areid
					INNER JOIN par.dimensao 				d   ON d.dimid = are.dimid
				WHERE 	
					{$whereSubacao}
					pp.prpid = $prpid 
					AND s.sbastatus = 'A' 
					AND pp.prpstatus = 'A'
			) as foo ORDER BY foo.codigo";
                                        
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso,
											case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
										from par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpid = $prpid and p.prpstatus = 'A'" );

	$sql = "SELECT u.usucpf as cpfreitor, iue.entid, u.usunome as reitor, iue.iuenome, iue.iuecnpj,
				e.entnome as universidade, e.entnumcpfcnpj as cnpjuniversidade, ed.endlog, ed.endcep, ed.endnum,
				ed.endbai, mun.mundescricao, mun.estuf, mun.muncod
			FROM par.usuarioresponsabilidade ur
			INNER JOIN seguranca.usuario 				u   on u.usucpf = ur.usucpf
			INNER JOIN par.instrumentounidadeentidade 	iue on iue.entid = ur.entid and iuedefault = false
			INNER JOIN entidade.entidade 				e   on iue.entid = e.entid
			INNER JOIN entidade.endereco 				ed 	on ed.entid = e.entid and ed.endstatus = 'A'
			INNER JOIN territorios.estado 				est on est.estuf = ed.estuf
			LEFT  JOIN territorios.municipio 			mun on mun.muncod = est.muncodcapital
			INNER JOIN par.processopar 					pp  on pp.prpcnpj = iue.iuecnpj and pp.inuid = iue.inuid and pp.prpstatus = 'A'
			WHERE
				rpustatus = 'A'
				AND ur.pflcod = 1141
				AND pp.prpid = $prpid";
	$arrUniversidade = $db->pegaLinha($sql);
// 	ver($sql,d);
	$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum,
											ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod, ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
										FROM  par.entidade ent
										INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
										INNER JOIN territorios.estado est on est.estuf = ent2.estuf
										LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
										where 
											ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
											ent.entstatus='A' AND
											ent2.entstatus='A' AND 
											ent.inuid = ".$arDadosProcesso['inuid']);

	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
				INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = $prpid and p.prpstatus = 'A'";

	$arCronograma = $db->pegaLinha( $sql );
        
        # Caso possua em sessao a data inicial e final essas serao carregadas no termo de compromisso
        if( $_SESSION['par']['cronogramainicial'] ){
		$arCronograma['cronogramainicial'] = $_SESSION['par']['cronogramainicial'];
	}
	if( $_SESSION['par']['cronogramaFinal'] ){
		$arCronograma['cronogramafinal'] = $_SESSION['par']['cronogramaFinal'];
	}
        
        #Buscar primeira data de vigencia
        $sqlDataInicioVigencia = "SELECT dopdatainiciovigencia FROM par.documentopar WHERE prpid = $prpid AND dopstatus <> 'E' ORDER BY dopid asc LIMIT 1";  
        $arCronograma['cronogramainicial'] = $db->pegaUm($sqlDataInicioVigencia);  

	#Recupera Empenhos somado aos reforco
	$arrEmpenho = Array();
	if( is_array($arrSub) && $arrSub[0] ){
		$arrEmpenho = getEmpenhoAgrupadoSubacao($arrSub);
	}  

	$cont = 1;
	$exerc = substr($arDadosProcesso['prpnumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO <br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS – ALFABETIZAÇÃO NA IDADE CERTA</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['prpnumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO SECRETARIA</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO SECRETARIA</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF SECRETARIA</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DOS DIRIGENTES</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DO SECRETÁRIO(A)</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}

		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="60%"><b>Tipo</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="10%"><b>Preço Unitário</b></td>
									<td width="20%"><b>Total</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		foreach ($arDadosItem as $key => $valor) {

			if( $valor['quantidade'] > 0 && $valor['valor'] > 0 ){
				$sql = "SELECT
							case when count(pic.picid) > 0 then 'Sim' else 'Não' end as arp
						FROM
							par.propostaitemcomposicao pic
							INNER JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' AND (now()::date between dic.dicdatainicial and dic.dicdatafinal)
							INNER JOIN par.ufdetalheitemcomposicao udi ON udi.dicid = dic.dicid AND udi.estuf = '{$arDadosProcesso['estuf']}'
						WHERE
							pic.picstatus = 'A' AND pic.picid = {$valor['picid']}";
				$arrARP = $db->pegaUm( $sql );

				$totalQTD += (float)$valor['quantidade'];
				$totalVLR += (float)$valor['valor'];
				$totalTOT += (float)$valor['total'];
				$tabela .= '	<tr>
									<td>'.$valor['codigo'].'</td>
									<td>'.$valor['picdescricao'].'</td>
									<td style="text-align: center;">'.$valor['quantidade'].'</td>
									<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
									<td style="text-align: right;"><b>R$ '.($valor['total'] ? number_format($valor['total'],2,",",".") : '').'</b></td>
								</tr>';
			}
		}
			$tabela .= '
							<tr>
								<td align="center" colspan="2"><b>Total Geral</b></td>
								<td style="text-align: center;">'.$totalQTD.'</td>
								<td style="text-align: right;">R$ '.number_format($totalVLR,2,",",".").'</td>
								<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
							</tr>
						</table>
					</td>
				</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
									<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
								</tr>
							</table>
						</td>
					</tr>';
			$tabela .= '
					<tr style="text-align: left;">
						<td colspan="3">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: center;">
									<td colspan="3"><b>EMPENHOS</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="50%"><b>Número</b></td>
									<td width="50%"><b>Valor</b></td>
								</tr>';
					$totalEmp = 0;
					foreach ($arrEmpenho as $v) {
						$totalEmp += (float)$v['valor'];
						$tabela.='<tr valign="top">
									<td width="50%" align="center">'.$v['empnumero'].'</td>
									<td width="50%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
								</tr>';
					}

					$tabela.= '<tr>
									<td align="center" colspan=2><b>Total Empenho</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
									<td></td>
								</tr>
							</table>
						</td>
					</tr>';
					$tabela .= '<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – ETAPAS OU FASES (SE HOUVER)</b></td>
					</tr>';
	$tabela .= '</table><br><br>';

	return $tabela;
}

function montaTabelaTermoCompromissoMunicipiosBP( $proid, $mdoid, $tpdcod, $arrObra, $iueid = '' ){
	global $db;

	if( is_array($arrObra) && $arrObra[0] ){
		$whereObras = " and po.preid in (".implode(',', $arrObra).") ";
	}

	$sql = "SELECT DISTINCT po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
			    --p.eobvalorempenho as vlr_empenhado,
			    po.prebairro, po.precep,
			    po.prelogradouro, mun.mundescricao, est.estdescricao, s.sbaid, d.dimcod || '.' || are.ardcod || '.' || i.indcod || '.' || sbaordem as codigo
			FROM par.empenhoobrapar  p
			INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
		--	INNER JOIN par.subacaoobra so ON so.preid = po.preid
			INNER JOIN cte.subacaoobra so ON so.preid = po.preid
			INNER JOIN cte.subacaoindicador s ON s.sbaid = so.sbaid
			INNER JOIN cte.acaoindicador a ON a.aciid = s.aciid
			INNER JOIN cte.pontuacao pon ON pon.ptoid = a.ptoid
			INNER JOIN cte.criterio c on c.crtid = pon.crtid
		    INNER JOIN cte.indicador i on i.indid = c.indid
		    INNER JOIN cte.areadimensao are on are.ardid = i.ardid
		    INNER JOIN cte.dimensao d on d.dimid = are.dimid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
			INNER JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			LEFT JOIN territorios.municipio mun on mun.muncod = po.muncod
			LEFT JOIN territorios.estado est on est.estuf = po.estuf
			INNER JOIN par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
			INNER JOIN par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			group by
				po.preid,
			    p.eobvalorempenho,
				po.muncod,
			    po.estuf,
			    po.predescricao,
			    po.prebairro,
			    po.precep,
			    po.prelogradouro,
			    mun.mundescricao,
			    est.estdescricao,
			    tpo.ptodescricao,
			    d.dimcod,
			    are.ardcod,
			    i.indcod,
			    sbaordem,
			    s.sbaid
			ORDER BY
				codigo";

	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.pronumeroprocesso, procnpj from	par.processoobraspar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prostatus = 'A' and p.proid = $proid" );

	$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, end2.endlog, end2.endcep, end2.endnum, end2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
									          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario, ent.entemail
									FROM  par.entidade ent
									INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_PREFEITURA."
									INNER JOIN territorios.estado est on est.estuf = ent2.estuf
									LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
									where 
										ent.dutid = ".DUTID_PREFEITO." AND
										ent.entstatus='A' AND
										ent2.entstatus='A' AND 
										ent.entnumcpfcnpj = '".$arDadosProcesso['procnpj']."'");

	$sql = "SELECT DISTINCT
				u.usunome as prefeito, u.usucpf as cpf
			FROM
				par.usuarioresponsabilidade ur
			    inner join seguranca.usuario u on u.usucpf = ur.usucpf
			    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
			WHERE
				ur.pflcod = ".PAR_PERFIL_PREFEITO."
                and us.sisid = 23
			    and ur.muncod = (select muncod from par.processoobraspar where prostatus = 'A' and proid = $proid)
			    and ur.rpustatus = 'A'
			    and us.suscod = 'A'";
	$arPrefeito = $db->pegaLinha( $sql );
	$cont = 1;
	$exerc = substr($arDadosProcesso['pronumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE BENEFICIÁRIO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['pronumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA PREFEITURA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) PREFEITO(A)</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arPrefeito['prefeito'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arPrefeito['cpf']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="60%"><b>Ações(Nome da Obra)</b></td>
									<td width="30%"><b>Tipo Obra</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="15%"><b>Valor(R$)</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		$arrLocalizacao = array();
		foreach ($arDadosItem as $key => $valor) {
			//$totalQTD += (float)$valor['quantidade'];
			//$totalVLR += (float)$valor['valor'];
			if( $valor['valor'] > 0 ){

				$localizacao = 'Bairro: '.$valor['prebairro'].', '.'Logradouro: '.$valor['prelogradouro'].', '.'Cidade: '.$valor['mundescricao'].'.<br>';

				$arrLocalizacao[] = array(
										'obras' => $valor['predescricao'],
										'localizacao' => $localizacao
									);

				$totalTOT += (float)$valor['valor'];
				$tabela .= '	<tr>
									<td>'.$valor['codigo'].'</td>
									<td>'.$valor['predescricao'].'</td>
									<td>'.$valor['ptodescricao'].'</td>
									<td style="text-align: center;">1</td>
									<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
								</tr>';
			}
		}
			$_SESSION['par']['totalVLR'] = $totalTOT;
			$tabela .= '
							<tr>
								<td align="center" colspan=3><b>Total Geral</b></td>
								<td style="text-align: center;"></td>
								<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
							</tr>
						</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – LOCALIZAÇÃO</b><br>
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr align="left" valign="top">
								<td><b>Nome da Obra</b></td>
								<td><b>Endereço</b></td>
							</tr>';
						foreach ($arrLocalizacao as $ende) {
							$tabela.= '<tr align="left" valign="top">
											<td>'.$ende['obras'].'</td>
											<td>'.$ende['localizacao'].'</td>
										</tr>';
						}
						
			$ano = date('Y');
			$anoFinal = $ano + 2;
			$mes = date('m');

			$dtFim = $_SESSION['par']['cronogramaFinal'] ? $_SESSION['par']['cronogramaFinal'] : $mes.'/'.$anoFinal;
			
			$tabela .= '</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
				</tr>
				<tr style="text-align: left;" valign="top">
					<td colspan="3">
						<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr style="text-align: left;" valign="top">
								<td width="50%"><b>Mês Inicial:</b><br>'.date('m/Y').'</td>
								<td width="50%"><b>Mês Final:</b><br>'.$dtFim.'</td>
							</tr>
						</table>
					</td>
				</tr>';
	$tabela .= '</table><br><br>';
	if( $tpdcod == 102 || $tpdcod == 21 ) enviaEmailDocGeradoObras($arrSecretaria['entemail'], 'M', $proid, $mdoid);
	return $tabela;
}

function montaTabelaTermoCompromissoMunicipios( $proid, $mdoid, $tpdcod, $arrObra, $iueid = '' ){
	global $db;

	if( $_REQUEST['dopid'] ){
		
		$sql = "SELECT coalesce(dopdatainiciovigencia, '".date('m/Y')."') FROM par.documentopar WHERE dopid = {$_REQUEST['dopid']}";
		
		$dataInicio = $db->pegaUm($sql);
		
	}else{
		
		$dataInicio = date('m/Y');
	}
	
	if( is_array($arrObra) && $arrObra[0] ){
		$whereObras = " and po.preid in (".implode(',', $arrObra).") ";
	}

	$sql = "SELECT DISTINCT 
				po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				po.prevalorobra::numeric(15,2) as valor,
			    po.prebairro, po.precep,
			    po.prelogradouro, mun.mundescricao, est.estdescricao, s.sbaid, d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo
			FROM par.empenhoobrapar  p
			INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
			INNER JOIN par.subacaoobra so ON so.preid = po.preid
            inner join par.subacao s on s.sbaid = so.sbaid and s.sbastatus = 'A'
            inner join par.acao a on a.aciid = s.aciid
			inner join par.pontuacao pon on pon.ptoid = a.ptoid
			inner join par.criterio c on c.crtid = pon.crtid
			inner join par.indicador i on i.indid = c.indid
			inner join par.area are on are.areid = i.areid
			inner join par.dimensao d on d.dimid = are.dimid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid 
			LEFT JOIN territorios.municipio mun on mun.muncod = po.muncod
			LEFT JOIN territorios.estado est on est.estuf = po.estuf
			INNER JOIN par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
			INNER JOIN par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			group by
				po.ptoid,
				tpo.ptocategoria,
				po.preid,
			    p.eobvalorempenho,
				po.muncod,
			    po.estuf,
			    po.predescricao,
			    po.prebairro,
			    po.precep,
			    po.prelogradouro,
			    mun.mundescricao,
			    est.estdescricao,
			    tpo.ptodescricao,
			    d.dimcod,
			    are.arecod,
			    i.indcod,
			    sbaordem,
			    s.sbaid
			ORDER BY
				codigo";
// ver($sql, d);
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.pronumeroprocesso from	par.processoobraspar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prostatus = 'A' and p.proid = $proid" );

	$sql = "SELECT 
				ent.entid, 
				ent2.entid, 
				ent2.entnome, 
				ent2.entnumcpfcnpj, 
				ent2.endlog, 
				ent2.endcep, 
				ent2.endnum, 
				ent2.endbai, 
				mun.mundescricao, 
				mun.estuf, 
				mun.muncod,
				ent.entnumcpfcnpj as cpfsecretario, 
				ent.entnome as secretario, 
				ent.entemail
			FROM  
				par.entidade ent
			INNER JOIN par.entidade 			ent2 ON ent.inuid  = ent2.inuid AND ent2.dutid = ".DUTID_PREFEITURA."
			INNER JOIN territorios.estado 		est  ON est.estuf  = ent2.estuf
			LEFT  JOIN territorios.municipio 	mun  ON mun.muncod = ent2.muncod
			WHERE 
				ent.dutid = ".DUTID_PREFEITO." AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent.inuid = ".$arDadosProcesso['inuid'];
// 	ver($sql,d);
	$arrSecretaria = $db->pegaLinha( $sql );

	$sql = "SELECT DISTINCT
				u.usunome as prefeito, u.usucpf as cpf
			FROM
				par.usuarioresponsabilidade ur
			    inner join seguranca.usuario u on u.usucpf = ur.usucpf
			    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
			WHERE
				ur.pflcod = ".PAR_PERFIL_PREFEITO."
                and us.sisid = 23
			    and ur.muncod = (select muncod from par.processoobraspar where proid = $proid and prostatus = 'A')
			    and ur.rpustatus = 'A'
			    and us.suscod = 'A'";
	$arPrefeito = $db->pegaLinha( $sql );
	$cont = 1;
	$exerc = substr($arDadosProcesso['pronumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE BENEFICIÁRIO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['pronumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA PREFEITURA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) PREFEITO(A)</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arPrefeito['prefeito'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arPrefeito['cpf']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="60%"><b>Ações(Nome da Obra)</b></td>
									<td width="30%"><b>Tipo Obra</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="15%"><b>Valor(R$)</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;

		$arrLocalizacao = array();
		
		foreach ($arDadosItem as $key => $valor) {
			//$totalQTD += (float)$valor['quantidade'];
			//$totalVLR += (float)$valor['valor'];
			if( $valor['valor'] > 0 ){

				$localizacao = 'Bairro: '.$valor['prebairro'].', '.'Logradouro: '.$valor['prelogradouro'].', '.'Cidade: '.$valor['mundescricao'].'.<br>';

				$arrLocalizacao[] = array(
										'obras' => $valor['predescricao'],
										'localizacao' => $localizacao
									);

				$totalTOT += $valor['valor'];
				$tabela .= '	<tr>
									<td>'.$valor['codigo'].'</td>
									<td>'.$valor['predescricao'].'</td>
									<td>'.$valor['ptodescricao'].'</td>
									<td style="text-align: center;">1</td>
									<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
								</tr>';
			}
		}
			$_SESSION['par']['totalVLR'] = $totalTOT;
			$tabela .= '
							<tr>
								<td align="center" colspan=3><b>Total Geral</b></td>
								<td style="text-align: center;"></td>
								<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
							</tr>
						</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – LOCALIZAÇÃO</b><br>
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr align="left" valign="top">
								<td><b>Nome da Obra</b></td>
								<td><b>Endereço</b></td>
							</tr>';
						foreach ($arrLocalizacao as $ende) {
							$tabela.= '<tr align="left" valign="top">
											<td>'.$ende['obras'].'</td>
											<td>'.$ende['localizacao'].'</td>
										</tr>';
						}
						
			$ano = date('Y');
			$anoFinal = $ano + 2;
			$mes = date('m');

			$dtFim = $_SESSION['par']['cronogramaFinal'] ? $_SESSION['par']['cronogramaFinal'] : $mes.'/'.$anoFinal;
		
			$tabela .= '</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
				</tr>
				<tr style="text-align: left;" valign="top">
					<td colspan="3">
						<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr style="text-align: left;" valign="top">
								<td width="50%"><b>Mês Inicial:</b><br>'.$dataInicio.'</td>
								<td width="50%"><b>Mês Final:</b><br>'.$dtFim.'</td>
							</tr>
						</table>
					</td>
				</tr>';
	$tabela .= '</table><br><br>';
	if( $tpdcod == 102 || $tpdcod == 21 ) enviaEmailDocGeradoObras($arrSecretaria['entemail'], 'M', $proid, $mdoid);
	return $tabela;
}


function montaTabelaTermoCompromissoEstadoBP( $proid, $mdoid, $tpdcod, $arrObra, $iueid = '' ){
	global $db;

	if( is_array($arrObra) && $arrObra[0] ){
		$whereObras = " and po.preid in (".implode(',', $arrObra).") ";
	}

	$sql = "SELECT DISTINCT po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
			    --p.eobvalorempenho as vlr_empenhado,
			    po.prebairro, po.precep,
			    po.prelogradouro, mun.mundescricao, est.estdescricao, s.sbaid, d.dimcod || '.' || are.ardcod || '.' || i.indcod || '.' || sbaordem as codigo
			FROM par.empenhoobrapar  p
			INNER JOIN obras.preobra po ON p.preid = po.preid  AND po.prestatus = 'A' and eobstatus = 'A'
		--	INNER JOIN par.subacaoobra so ON so.preid = po.preid
			INNER JOIN cte.subacaoobra so ON so.preid = po.preid
			INNER JOIN cte.subacaoindicador s ON s.sbaid = so.sbaid
			INNER JOIN cte.acaoindicador a ON a.aciid = s.aciid
			INNER JOIN cte.pontuacao pon ON pon.ptoid = a.ptoid
			INNER JOIN cte.criterio c on c.crtid = pon.crtid
		    INNER JOIN cte.indicador i on i.indid = c.indid
		    INNER JOIN cte.areadimensao are on are.ardid = i.ardid
		    INNER JOIN cte.dimensao d on d.dimid = are.dimid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
			INNER JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			left join territorios.municipio mun on mun.muncod = po.muncod
			left join territorios.estado est on est.estuf = po.estuf
			inner join par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
			inner join par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			group by
				po.preid,
			    p.eobvalorempenho,
				po.muncod,
			    po.estuf,
			    po.predescricao,
			    po.prebairro,
			    po.precep,
			    po.prelogradouro,
			    mun.mundescricao,
			    est.estdescricao,
			    tpo.ptodescricao,
			    d.dimcod,
			    are.ardcod,
			    i.indcod,
			    sbaordem,
			    s.sbaid
			 ORDER BY
			   	codigo";

	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.pronumeroprocesso, p.procnpj from	par.processoobraspar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prostatus = 'A' and p.proid = $proid" );

			//CNPJ do municipio
	$sql = "SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
			          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario, ent.entemail
			FROM  par.entidade ent
			INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
			INNER JOIN territorios.estado est on est.estuf = ent2.estuf
			LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
			where 
				ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent2.entnumcpfcnpj = '".$arDadosProcesso['procnpj']."'";
	$arrSecretaria = $db->pegaLinha($sql);
	//ver($sql,d);
	$cont = 1;
	$exerc = substr($arDadosProcesso['pronumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['pronumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA DE EDUCAÇÃO DO ESTADO</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="30%"><b>Ação(Nome da Obra)</b></td>
									<td width="30%"><b>Tipo Obra</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="15%"><b>Valor(R$)</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;
		$arrLocalizacao = array();
		$arrPreid = array();
		foreach ($arDadosItem as $key => $valor) {
        	//$totalQTD += (float)$valor['quantidade'];
            //$totalVLR += (float)$valor['valor'];
            if(!in_array($valor['preid'], $arrPreid)){
            	$arrPreid[] = $valor['preid'];


				if( $valor['valor'] > 0 ){
					$sql = "select
								d.dimcod || '.' || are.ardcod || '.' || i.indcod || '.' || sbaordem
							from
								cte.subacaoindicador s
							INNER JOIN cte.acaoindicador a ON a.aciid = s.aciid
							INNER JOIN cte.pontuacao pon ON pon.ptoid = a.ptoid
							INNER JOIN cte.criterio c on c.crtid = pon.crtid
						    INNER JOIN cte.indicador i on i.indid = c.indid
						    INNER JOIN cte.areadimensao are on are.ardid = i.ardid
						    INNER JOIN cte.dimensao d on d.dimid = are.dimid
							where
								s.sbaid = {$valor['sbaid']}";
					$subacao = $db->pegaUm($sql);
					$localizacao = 'Bairro: '.$valor['prebairro'].', '.'Logradouro: '.$valor['prelogradouro'].', '.'Cidade: '.$valor['mundescricao'].'.<br>';

					$arrLocalizacao[] = array(
											'obras' => $valor['predescricao'],
											'localizacao' => $localizacao
										);
					$totalTOT += (float)$valor['valor'];
					$tabela .= '	<tr>
										<td>'.$subacao.'</td>
										<td>'.$valor['predescricao'].'</td>
										<td>'.$valor['ptodescricao'].'</td>
										<td style="text-align: center;">1</td>
										<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
									</tr>';
				}
			}
		}
			$_SESSION['par']['totalVLR'] = $totalTOT;
			$tabela .= '
							<tr>
								<td align="center" colspan=4><b>Total Geral</b></td>
								<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
							</tr>
						</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – LOCALIZAÇÃO</b><br>
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr align="left" valign="top">
								<td><b>Nome da Obra</b></td>
								<td><b>Endereço</b></td>
							</tr>';
						foreach ($arrLocalizacao as $ende) {
							$tabela.= '<tr align="left" valign="top">
											<td>'.$ende['obras'].'</td>
											<td>'.$ende['localizacao'].'</td>
										</tr>';
						}
						
			$ano = date('Y');
			$anoFinal = $ano + 2;
			$mes = date('m');

			$dtFim = $_SESSION['par']['cronogramaFinal'] ? $_SESSION['par']['cronogramaFinal'] : $mes.'/'.$anoFinal;

			$tabela .= '</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
				</tr>
				<tr style="text-align: left;" valign="top">
					<td colspan="3">
						<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr style="text-align: left;" valign="top">
								<td width="50%"><b>Mês Inicial:</b><br>'.date('m/Y').'</td>
								<td width="50%"><b>Mês Final:</b><br>'.$dtFim.'</td>
							</tr>
						</table>
					</td>
				</tr>';
	$tabela .= '</table><br><br>';

	if( $tpdcod == 102 || $tpdcod == 21 ) enviaEmailDocGeradoObras($arrSecretaria['entemail'], 'E', $proid, $mdoid);
	return $tabela;
}

function montaTabelaTermoCompromissoEstadoRDC( $proid, $mdoid, $tpdcod, $arrObra, $iueid = '' ){
	global $db;

	if( is_array($arrObra) && $arrObra[0] ){
		$whereObras = " and po.preid in (".implode(',', $arrObra).") ";
	}

	$sql = "SELECT DISTINCT po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
			    --p.eobvalorempenho as vlr_empenhado,
			    po.prebairro, po.precep,
			    po.prelogradouro, mun.mundescricao, est.estdescricao, s.sbaid, d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo
			FROM par.empenhoobrapar  p
			INNER JOIN obras.preobra po ON p.preid = po.preid and eobstatus = 'A'
			INNER JOIN par.subacaoobra so ON so.preid = po.preid
            inner join par.subacao s on s.sbaid = so.sbaid and s.sbastatus = 'A'
            inner join par.acao a on a.aciid = s.aciid
			inner join par.pontuacao pon on pon.ptoid = a.ptoid
			inner join par.criterio c on c.crtid = pon.crtid
			inner join par.indicador i on i.indid = c.indid
			inner join par.area are on are.areid = i.areid
			inner join par.dimensao d on d.dimid = are.dimid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
			INNER JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			left join territorios.municipio mun on mun.muncod = po.muncod
			left join territorios.estado est on est.estuf = po.estuf
			inner join par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
			inner join par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and  pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			group by
				po.preid,
			    p.eobvalorempenho,
				po.muncod,
			    po.estuf,
			    po.predescricao,
			    po.prebairro,
			    po.precep,
			    po.prelogradouro,
			    mun.mundescricao,
			    est.estdescricao,
			    tpo.ptodescricao,
			    d.dimcod,
			    are.arecod,
			    i.indcod,
			    sbaordem,
			    s.sbaid
			 ORDER BY
			   	codigo";

	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.pronumeroprocesso from	par.processoobraspar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prostatus = 'A' and p.proid = $proid" );

			//CNPJ do municipio
	$sql = "SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
			          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario, ent.entemail
			FROM  par.entidade ent
			INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
			INNER JOIN territorios.estado est on est.estuf = ent2.estuf
			LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
			where 
				ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent.inuid = ".$arDadosProcesso['inuid'];
		$arrSecretaria = $db->pegaLinha($sql);
		//ver($sql,d);
	$cont = 1;
	$exerc = substr($arDadosProcesso['pronumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['pronumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA DE EDUCAÇÃO DO ESTADO</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="30%"><b>Ação(Nome da Obra)</b></td>
									<td width="30%"><b>Tipo Obra</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;
		$arrLocalizacao = array();
		$arrPreid = array();
		foreach ($arDadosItem as $key => $valor) {
        	//$totalQTD += (float)$valor['quantidade'];
            //$totalVLR += (float)$valor['valor'];
            if(!in_array($valor['preid'], $arrPreid)){
            	$arrPreid[] = $valor['preid'];


				if( $valor['valor'] > 0 ){
					$sql = "select
								d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem
							from
								par.subacao s
							inner join par.acao a on a.aciid = s.aciid
							inner join par.pontuacao p on p.ptoid = a.ptoid
							inner join par.criterio c on c.crtid = p.crtid
							inner join par.indicador i on i.indid = c.indid
							inner join par.area are on are.areid = i.areid
							inner join par.dimensao d on d.dimid = are.dimid
							where
								s.sbaid = {$valor['sbaid']}";
					$subacao = $db->pegaUm($sql);
					$localizacao = 'Bairro: '.$valor['prebairro'].', '.'Logradouro: '.$valor['prelogradouro'].', '.'Cidade: '.$valor['mundescricao'].'.<br>';

					$arrLocalizacao[] = array(
											'obras' => $valor['predescricao'],
											'localizacao' => $localizacao
										);
					$totalTOT += (float)$valor['valor'];
					$tabela .= '	<tr>
										<td>'.$subacao.'</td>
										<td>'.$valor['predescricao'].'</td>
										<td>'.$valor['ptodescricao'].'</td>
										<td style="text-align: center;">1</td>
									</tr>';
				}
			}
		}
		$tabela .= '	<tr>
							<td align="center" colspan=3><b>Total Geral</b></td>
							<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
						</tr>';
		
			$_SESSION['par']['totalVLR'] = $totalTOT;
			$tabela .= '
						</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – LOCALIZAÇÃO</b><br>
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr align="left" valign="top">
								<td><b>Nome da Obra</b></td>
								<td><b>Endereço</b></td>
							</tr>';
						foreach ($arrLocalizacao as $ende) {
							$tabela.= '<tr align="left" valign="top">
											<td>'.$ende['obras'].'</td>
											<td>'.$ende['localizacao'].'</td>
										</tr>';
						}
						
			$ano = date('Y');
			$anoFinal = $ano + 2;
			$mes = date('m');

			$dtFim = $_SESSION['par']['cronogramaFinal'] ? $_SESSION['par']['cronogramaFinal'] : $mes.'/'.$anoFinal;

			$tabela .= '</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
				</tr>
				<tr style="text-align: left;" valign="top">
					<td colspan="3">
						<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr style="text-align: left;" valign="top">
								<td width="50%"><b>Mês Inicial:</b><br>'.date('m/Y').'</td>
								<td width="50%"><b>Mês Final:</b><br>'.$dtFim.'</td>
							</tr>
						</table>
					</td>
				</tr>';
	$tabela .= '</table><br><br>';

	if( $tpdcod == 102 || $tpdcod == 21 ) enviaEmailDocGeradoObras($arrSecretaria['entemail'], 'E', $proid, $mdoid);
	return $tabela;
}

function montaTabelaTermoCompromissoEstado( $proid, $mdoid, $tpdcod, $arrObra, $iueid = '' ){
	global $db;
	
	if( is_array($arrObra) && $arrObra[0] ){
		$whereObras = " and po.preid in (".implode(',', $arrObra).") ";
	}

	$sql = "SELECT DISTINCT po.preid, tpo.ptodescricao, po.predescricao, po.muncod, po.estuf,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
			    --p.eobvalorempenho as vlr_empenhado,
			    po.prebairro, po.precep,
			    po.prelogradouro, mun.mundescricao, est.estdescricao, s.sbaid, d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo
			FROM par.empenhoobrapar  p
			INNER JOIN obras.preobra po ON p.preid = po.preid and eobstatus = 'A'
			INNER JOIN par.subacaoobra so ON so.preid = po.preid
            inner join par.subacao s on s.sbaid = so.sbaid and s.sbastatus = 'A'
            inner join par.acao a on a.aciid = s.aciid
			inner join par.pontuacao pon on pon.ptoid = a.ptoid
			inner join par.criterio c on c.crtid = pon.crtid
			inner join par.indicador i on i.indid = c.indid
			inner join par.area are on are.areid = i.areid
			inner join par.dimensao d on d.dimid = are.dimid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0 and itc.itcstatus = 'A'
			INNER JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			left join territorios.municipio mun on mun.muncod = po.muncod
			left join territorios.estado est on est.estuf = po.estuf
			inner join par.empenho emp on emp.empid = p.empid and empstatus <> 'I'
			inner join par.processoobraspar pro on pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			WHERE
				pro.proid = $proid
				{$whereObras}
			group by
				po.preid,
			    p.eobvalorempenho,
				po.muncod,
			    po.estuf,
			    po.predescricao,
			    po.prebairro,
			    po.precep,
			    po.prelogradouro,
			    mun.mundescricao,
			    est.estdescricao,
			    tpo.ptodescricao,
			    d.dimcod,
			    are.arecod,
			    i.indcod,
			    sbaordem,
			    s.sbaid
			 ORDER BY
			   	codigo";
	
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.pronumeroprocesso from	par.processoobraspar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prostatus = 'A' and  p.proid = $proid" );

			//CNPJ do municipio
	$sql = "SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
			          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario, ent.entemail
			FROM  par.entidade ent
			INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
			INNER JOIN territorios.estado est on est.estuf = ent2.estuf
			LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
			where 
				ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent.inuid = ".$arDadosProcesso['inuid'];
		$arrSecretaria = $db->pegaLinha($sql);
		//ver($sql,d);
	$cont = 1;
	$exerc = substr($arDadosProcesso['pronumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['pronumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA DE EDUCAÇÃO DO ESTADO</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="05%"><b>Subação</b></td>
									<td width="30%"><b>Ação(Nome da Obra)</b></td>
									<td width="30%"><b>Tipo Obra</b></td>
									<td width="10%"><b>Metas Quantitativas</b></td>
									<td width="15%"><b>Valor(R$)</b></td>
								</tr>';
		$totalQTD = 0;
		$totalVLR = 0;
		$totalTOT = 0;
		$arrLocalizacao = array();
		$arrPreid = array();
		foreach ($arDadosItem as $key => $valor) {
        	//$totalQTD += (float)$valor['quantidade'];
            //$totalVLR += (float)$valor['valor'];
            if(!in_array($valor['preid'], $arrPreid)){
            	$arrPreid[] = $valor['preid'];


				if( $valor['valor'] > 0 ){
					$sql = "select
								d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem
							from
								par.subacao s
							inner join par.acao a on a.aciid = s.aciid
							inner join par.pontuacao p on p.ptoid = a.ptoid
							inner join par.criterio c on c.crtid = p.crtid
							inner join par.indicador i on i.indid = c.indid
							inner join par.area are on are.areid = i.areid
							inner join par.dimensao d on d.dimid = are.dimid
							where
								s.sbaid = {$valor['sbaid']}";
					$subacao = $db->pegaUm($sql);
					$localizacao = 'Bairro: '.$valor['prebairro'].', '.'Logradouro: '.$valor['prelogradouro'].', '.'Cidade: '.$valor['mundescricao'].'.<br>';

					$arrLocalizacao[] = array(
											'obras' => $valor['predescricao'],
											'localizacao' => $localizacao
										);
					$totalTOT += (float)$valor['valor'];
					$tabela .= '	<tr>
										<td>'.$subacao.'</td>
										<td>'.$valor['predescricao'].'</td>
										<td>'.$valor['ptodescricao'].'</td>
										<td style="text-align: center;">1</td>
										<td style="text-align: right;">R$ '.($valor['valor'] ? number_format($valor['valor'],2,",",".") : '').'</td>
									</tr>';
				}
			}
		}
			$_SESSION['par']['totalVLR'] = $totalTOT;
			$tabela .= '
							<tr>
								<td align="center" colspan=4><b>Total Geral</b></td>
								<td style="text-align: right;"><b>R$ '.number_format($totalTOT,2,",",".").'</b></td>
							</tr>
						</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – LOCALIZAÇÃO</b><br>
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr align="left" valign="top">
								<td><b>Nome da Obra</b></td>
								<td><b>Endereço</b></td>
							</tr>';
						foreach ($arrLocalizacao as $ende) {
							$tabela.= '<tr align="left" valign="top">
											<td>'.$ende['obras'].'</td>
											<td>'.$ende['localizacao'].'</td>
										</tr>';
						}
						
			$ano = date('Y');
			$anoFinal = $ano + 2;
			$mes = date('m');

			$dtIni = $_SESSION['par']['cronogramainicial'] ? $_SESSION['par']['cronogramainicial'] : date('m/Y');
			$dtFim = $_SESSION['par']['cronogramaFinal'] ? $_SESSION['par']['cronogramaFinal'] : $mes.'/'.$anoFinal;

			$tabela .= '</table>
					</td>
				</tr>
				<tr style="text-align: left;">
					<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
				</tr>
				<tr style="text-align: left;" valign="top">
					<td colspan="3">
						<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
							<tr style="text-align: left;" valign="top">
								<td width="50%"><b>Mês Inicial:</b><br>'.$dtIni.'</td>
								<td width="50%"><b>Mês Final:</b><br>'.$dtFim.'</td>
							</tr>
						</table>
					</td>
				</tr>';
	$tabela .= '</table><br><br>';

	if( $tpdcod == 102 || $tpdcod == 21 ) enviaEmailDocGeradoObras($arrSecretaria['entemail'], 'E', $proid, $mdoid);
	return $tabela;
}

function montaTabelaClasseFinanceira( $prpid ){
	global $db;

	$sql = "SELECT pg.prgdsc, e.empfonterecurso, e.empcodigonatdespesa,
				e.empnumero, e.empdata, vve.vrlempenhocancelado as empvalorempenho
			FROM par.empenho e
				inner join par.empenhosubacao es on es.empid = e.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and eobstatus = 'A'
				inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
			    inner join par.subacao s on s.sbaid = es.sbaid
			    inner join par.programa pg on pg.prgid = s.prgid
			    inner join par.processopar pp on pp.prpnumeroprocesso = e.empnumeroprocesso and pp.prpstatus = 'A'
			WHERE empstatus <> 'I' and pp.prpid = $prpid";

	$arDados = $db->carregar($sql);
	$arDados = $arDados ? $arDados : array();
	$tabela = '';

	$tabela = '<table align="left" style="border-style: dotted;" border="1" cellspacing="0" cellpadding="2">
				<tr style="text-align: center;">
					<td rowspan="2"><b>Programa de Trabalho</b></td>
					<td rowspan="2"><b>Fonte de Recurso</b></td>
					<td rowspan="2"><b>Natureza da Despesa</b></td>
					<td colspan="3"><b>Nota de Empenho</b></td>
				</tr>
				<tr style="text-align: center;">
					<td><b>Número</b></td>
					<td><b>Data</b></td>
					<td><b>Valor(es) em R$</b></td>
				</tr>';

	foreach ($arDados as $key => $valor) {
		$data = $valor['empdata'] ? formata_data($valor['empdata']) : '';
		$valorEmpenho = $valor['empvalorempenho'] ? number_format($valor['empvalorempenho'],2,",",".") : '';

		$tabela .= '<tr>';
		$tabela .= '<td>'.$valor['prgdsc'].'</td>';
		$tabela .= '<td>'.$valor['empfonterecurso'].'</td>';
		$tabela .= '<td>'.$valor['empcodigonatdespesa'].'</td>';
		$tabela .= '<td>'.$valor['empnumero'].'</td>';
		$tabela .= '<td>'.$data.'</td>';
		$tabela .= '<td style="text-align: right;">'.$valorEmpenho.'</td>';
		$tabela .= '</tr>';
	}
	$tabela .= '</table><br><br>';

	return $tabela;
}

function montaTabelaRafComplementoSubacao( $prpid ){

	global $db;

	//Complemento
	$sql = "SELECT
				par.retornacodigosubacao(s.sbaid) as local,
				sbadsc as nome_sub,
				sbdano as ano,
				sbdrepassevlrcomplementaraprovado as complemento
			FROM
				par.subacao  s
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
			LEFT JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
			left JOIN par.documentopar dop ON dop.dopid = tc.dopid
			left JOIN par.processopar prp ON dop.prpid = prp.prpid
			WHERE
            	s.sbastatus = 'A'
				AND prp.prpid = $prpid
				AND sbdrepassevlrcomplementaraprovado > 0
			GROUP BY 
			local, nome_sub, ano, complemento
	";

	$arDados = $db->carregar($sql);
	$arDados = $arDados ? $arDados : array();

	$arIninome = array();
	foreach ($arDados as $v) {
		$arIninome[$v['sbaid']][] = $v['sbaid'];
	}

	$ininomeAnt = "";
	$tabela .= '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
					<tr style="text-align: center;">
						<td colspan="4"><b>Complementos Orçamentários</b></td>
					</tr>
					<tr style="text-align: center;">
						<td><b>Local</b></td>
						<td><b>Nome da Subação</b></td>
						<td><b>Ano</b></td>
						<td width="10%"><b>Complemento <br>Orçamentário</b></td>
					</tr>';

	if( count($arDados) < 1 ){
		$tabela .= '<tr>';
		$tabela .= '	<td colspan="4"> Não possui</td>';
		$tabela .= '</tr>';
	}else{
		$totalComplemento = 0;
		foreach ($arDados as $key => $valor) {
			$valorComplemento 	= $valor['complemento'] ? number_format($valor['complemento'],2,",",".") : '0,00';

			$totalComplemento = $totalComplemento+TotalComplemento+$valor['complemento'];
			$tabela .= '<tr>';
			$tabela .= '	<td>'.$valor['local'].'</td>';
			$tabela .= '	<td>'.$valor['nome_sub'].'</td>';
			$tabela .= '	<td style="text-align: center;">'.$valor['ano'].'</td>';
			$tabela .= '	<td style="text-align: right;">'.$valorComplemento.'</td>';
			$tabela .= '</tr>';
		}
	}
	$tabela .= '<tr>
					<td align="center" colspan="3"><b>Total Complementos Orçamentários</b></td>
					<td style="text-align: right;"><b>R$ '.number_format($totalComplemento,2,",",".").'</b></td>
				</tr>
			</table>';

	if( !($totalComplemento > 0) ){
		$tabela = '';
	}

	//RAF
	$sql = "SELECT
            	par.retornacodigosubacao(s.sbaid) as local,
				sbadsc as  nome_sub,
				sbdano as ano,
				sbdrepassevlrrafaprovado as raf
			FROM
				par.subacao s
			INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid
			LEFT JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
			left JOIN par.documentopar dop ON dop.dopid = tc.dopid
			left JOIN par.processopar prp ON dop.prpid = prp.prpid 
			WHERE
            	s.sbastatus = 'A' AND
            	prp.prpid = $prpid AND 
				sd.sbdrepassevlrrafaprovado > 0
			GROUP BY 
				local, nome_sub, ano, raf";

	$arDados = $db->carregar($sql);
	$arDados = $arDados ? $arDados : array();

	$arIninome = array();
	foreach ($arDados as $v) {
		$arIninome[$v['sbaid']][] = $v['sbaid'];
	}

	$ininomeAnt = "";
	$tabelaBuffer .= '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
					<tr style="text-align: center;">
						<td colspan="4"><b>Complementos RAF</b></td>
					</tr>
					<tr style="text-align: center;">
						<td><b>Local</b></td>
						<td><b>Nome da Subação</b></td>
						<td><b>Ano</b></td>
						<td width="10%"><b>RAF</b></td>
					</tr>';


	if( count($arDados) < 1 ){
		$tabelaBuffer .= '<tr>';
		$tabelaBuffer .= '	<td colspan="4"> Não possui</td>';
		$tabelaBuffer .= '</tr>';
	}else{
		$totalRAF = 0;
		foreach ($arDados as $key => $valor) {

			$valorRAF 			= $valor['raf'] ? number_format($valor['raf'],2,",",".") : '0,00';

			$totalRAF = $totalRAF+$valor['raf'];
			$tabelaBuffer .= '<tr>';
			$tabelaBuffer .= '	<td>'.$valor['local'].'</td>';
			$tabelaBuffer .= '	<td>'.$valor['nome_sub'].'</td>';
			$tabelaBuffer .= '	<td style="text-align: center;">'.$valor['ano'].'</td>';
			$tabelaBuffer .= '	<td style="text-align: right;">'.$valorRAF.'</td>';
			$tabelaBuffer .= '</tr>';
		}
	}
	$tabelaBuffer .= '<tr>
					<td align="center" colspan="3"><b>Total RAF</b></td>
					<td style="text-align: right;"><b>R$ '.number_format($totalRAF,2,",",".").'</b></td>
				</tr>
			</table>';

	if( $valorRAF > 0 ){
		$tabela .= $tabelaBuffer;
	}

	$retorno['tabela'] = $tabela;
	$retorno['total']  = $totalComplemento+$totalRAF;

	return $retorno;

}

function montaTabelaParcelaDesembolso( $prpid ){
	global $db;

	$sql = "SELECT           s.sbadsc as finalidade,
			           	sd.sbdinicio::text || '/' || sd.sbdano::text
			            AS cronogramainicial,
			            CASE   WHEN sd.sbdanotermino IS NOT NULL
			                        THEN sd.sbdfim::text || '/' || sd.sbdanotermino::text
			                        ELSE sd.sbdfim::text || '/' || sd.sbdano::text
			            END AS cronogramafinal, vve.vrlempenhocancelado as empvalorempenho
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04')
			inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE empstatus <> 'I' and p.prpid = $prpid and p.prpstatus = 'A'";

	$arDados = $db->carregar($sql);
	$arDados = $arDados ? $arDados : array();

	$arIninome = array();
	foreach ($arDados as $v) {
		$arIninome[$v['sbaid']][] = $v['sbaid'];
	}

	$ininomeAnt = "";
	$tabela .= '<table align="left" border="1" cellspacing="0" cellpadding="2">
				<tbody>
				<tr style="text-align: center;">
					<td><b>Finalidade</b></td>
					<td><b>Parcela</b></td>
					<td><b>Mês/Ano</b></td>
					<td><b>Valor(es) em R$</b></td>
				</tr>';

	$parcela = 0;
	foreach ($arDados as $key => $valor) {
		$valorEmpenho = $valor['empvalorempenho'] ? number_format($valor['empvalorempenho'],2,",",".") : '';

		$parcela ++;
		$tabela .= '<tr>';
		if( $valor['finalidade'] != $ininomeAnt ){
			$ininomeAnt = $valor['finalidade'];
			$tabela .= '<td rowspan="'.count($arIninome[$valor['sbaid']]).'" valign="middle">'.$valor['finalidade'].'</td>';
			$parcela = 1;
		}
		$tabela .= '<td style="text-align: center;">'.($parcela).'</td>';
		$tabela .= '<td>'.$valor['cronogramainicial'].'</td>';
		$tabela .= '<td style="text-align: right;">'.$valorEmpenho.'</td>';
		$tabela .= '</tr>';
	}
	$tabela .= '</tbody></table><br><br>';

	return $tabela;
}

function montaTermoCompromisso( $prpid, $iueid = '' ){
	global $db;

	echo '<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>';

	$sql = "SELECT iu.inuid, iu.muncod, ent.entnumcpfcnpj, ent.entnome, ent.entemail, ent.estuf, ent.endcep, ent.endlog, ent.endnum, ent.endbai, mu.mundescricao, pp.*
			FROM par.entidade ent
			INNER JOIN par.instrumentounidade      iu ON iu.inuid = ent.inuid
			INNER JOIN par.processopar   pp ON pp.inuid = iu.inuid and pp.prpstatus = 'A'
			inner join territorios.municipio mu on mu.muncod = iu.muncod
			where ent.dutid = ".DUTID_PREFEITURA." AND pp.prpid = $prpid";

	$arDados = $db->pegaLinha( $sql );

	$sql = "SELECT prgdsc as programa, to_char(now(), 'YYYY') as anoexercicio
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.programa pr ON pr.prgid = s.prgid
			WHERE p.prpid = $prpid and p.prpstatus = 'A'";

	$arPrograma = $db->pegaLinha( $sql );

	$sql = "SELECT
					distinct ent.entid, ent.entnome, ent.entemail, ent.entnumcpfcnpj
				FROM par.entidade ent
				INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_PREFEITURA."
				INNER JOIN territorios.estado est on est.estuf = ent2.estuf
				LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
                INNER JOIN par.instrumentounidade      iu ON iu.inuid = ent.inuid
                INNER JOIN par.processopar   pp ON pp.inuid = iu.inuid and pp.prpstatus = 'A'
				WHERE
					ent.dutid = ".DUTID_PREFEITO." AND
					and pp.prpid = $prpid";
	$arrSecretaria = $db->pegaLinha($sql);

	$sql = "SELECT DISTINCT
				u.usunome as entnome, u.usucpf as entnumcpfcnpj
			FROM
				par.usuarioresponsabilidade ur
			    inner join seguranca.usuario u on u.usucpf = ur.usucpf
			    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
			WHERE
				ur.pflcod = ".PAR_PERFIL_PREFEITO."
                and us.sisid = 23
			    and ur.muncod = (select muncod from par.processopar where prpid = $prpid and prpstatus = 'A')
			    and ur.rpustatus = 'A'
			    and us.suscod = 'A'";

	$arPrefeito = $db->pegaLinha( $sql );

	$sql = "SELECT           s.sbadsc as finalidade,
			           	'01/' || sd.sbdinicio::text || '/' || sd.sbdano::text
			            AS cronogramainicial,
			            CASE   WHEN sd.sbdanotermino IS NOT NULL
			                        THEN '01/' || sd.sbdfim::text || '/' || sd.sbdanotermino::text
			                        ELSE '01/' || sd.sbdfim::text || '/' || sd.sbdano::text
			            END AS cronogramafinal
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = $prpid and p.prpstatus = 'A'";

	$arCronograma = $db->pegaLinha( $sql );

	//$espaco = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
	$cont = 1;
	$tabela = '
	<table align="center" class="tabela" border=1 cellspacing="0" cellpadding="3">
		<tr>
			<td colspan=3 style="font-weight: bold; font-size: 8pt; text-align: center">IDENTIFICAÇÃO</td>
		</tr>
		<tr>
			<td colspan=2><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Programa</b><br> '.$espaco.$arPrograma['programa'].'</td>
			<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Exercício</b><br>'.$espaco.$arPrograma['anoexercicio'].'</td>
		</tr>

		<tr>
			<td colspan=2><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nome da Prefeitura</b><br> '.$espaco.$arDados['entnome'].'</td>
			<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' -  N.º do CNPJ</b><br>'.$espaco.formatar_cnpj($arDados['entnumcpfcnpj']).'</td>
		</tr>
		<tr>
			<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Endereço</b><br> '.$espaco.$arDados['endlog'].' '.$arDados['endnum'].' - '.$arDados['endbai'].'</td>
			<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Município</b><br>'.$espaco.$arDados['mundescricao'].'</td>
			<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$espaco.$arDados['estuf'].'</td>
		</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= '
		<tr>
			<td colspan=3 style="font-weight: bold; font-size: 8pt; text-align: center">IDENTIFICAÇÃO DO(A) PREFEITO(A)</td>
		</tr>
		<tr>
			<td colspan=2><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nome</b><br> '.$espaco.$arPrefeito['entnome'].'</td>
			<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.$espaco.formatar_cpf($arPrefeito['entnumcpfcnpj']).'</td>
		</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
		<tr>
			<td colspan=3 style="font-weight: bold; font-size: 8pt; text-align: center">OBJETO: VEÍCULO PADRONIZADO PARA O TRANSPORTE ESCOLAR</td>
		</tr>
		<tr>
			<td colspan=3><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – Detalhamento do Objeto</b><br>
					<table align="center" width="75%" border="1" cellspacing="0" cellpadding="3">
					<tr>
						<td style="text-align: center;"><b>Tipo de Veículo</b></td>
						<td style="text-align: center;"><b>Quantidade</b></td>
						<td style="text-align: center;"><b>Preço Unitário</b></td>
						<td style="text-align: right;"><b>Total</b></td>
					</tr>';

					$sql = "SELECT sic.icodescricao as descricao,
								sic.icovalor as valor,
								sic.icoquantidade as quantidade,
								(sic.icovalor * sic.icoquantidade) as total
							FROM par.processopar p
							INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
							INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
							INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
							INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
							INNER JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = es.eobano
							WHERE p.prpid = $prpid AND icostatus = 'A' and p.prpstatus = 'A'";

					$arItens = $db->carregar( $sql );
					$arItens = $arItens ? $arItens : array();
					$totalQtd 		 = 0;
					$totalValorUnit  = 0;
					$totalValorTotal = 0;
					foreach ($arItens as $v) {
						$totalQtd 		 = (int)$totalQtd + (int)$v['quantidade'];
						$totalValorUnit  = (float) $totalValorUnit + (float)$v['valor'];
						$totalValorTotal = (float) $totalValorTotal + (float)$v['total'];
					$tabela .= '<tr>
							<td>'.$v['descricao'].'</td>
							<td style="text-align: center;">'.$v['quantidade'].'</td>
							<td style="text-align: right;">'.($v['valor'] ? number_format($v['valor'], 2, ',', '.') : '') .'</td>
							<td style="text-align: right;">'.($v['total'] ? number_format($v['total'], 2, ',', '.') : '').'</td>
						</tr>';
					}
				$_SESSION['par']['totalVLR'] = $totalValorTotal;
				$tabela .= '<tr>
								<td>Total Geral</td>
								<td style="text-align: center;"></td>
								<td style="text-align: right;"></td>
								<td style="text-align: right;">'.number_format($totalValorTotal, 2, ',', '.').'</td>
							</tr></table>
			</td>
		</tr>
		<tr>
			<td colspan=3><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – Cronograma de Execução</b><br>
		</tr>
		<tr>
			<td colspan=2>'.$espaco.'Mês Inicial: '.$arCronograma['cronogramainicial'].'</td>
			<td>Mês Final: '.$arCronograma['cronogramafinal'].'</td>
		</tr>
		<tr>
			<td colspan=3 style="font-weight: bold; font-size: 8pt; text-align: center">COMPROMISSOS PARA EXECUÇÃO DO OBJETO</td>
		</tr>
		<tr>
			<td colspan=3><br> I – Comprometo executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação (FNDE) na(s) aquisição(ões) do(s) bem(ns) descrito(s) acima em estrita observância às normas editadas pelo Conselho Deliberativo do FNDE  por meio de resolução específica.

 <br><br><br>

II – Declaro, sob as penas da lei, que o Município encontra-se dentro dos limites de despesa com pessoal, visando ao atendimento das exigências  legais contidas no artigo 169 da Constituição Federal e no artigo 25, §1º, IV, “c” da Lei Complementar nº 101, de 2000, para fins de assistência financeira junto ao FNDE.
<br><br>
			</td>
		</tr>
		</table>';
	return $tabela;
}

function montaTabelaSubacaoEmpenhoEstado( $prpid, $arrSub, $mdoid, $iueid = '' ){
	global $db;

	// A pedido do Julio Viana no dia 28/12/2012 (reta final).
	if( $prpid == 3389 ){

		$sql = "SELECT

						    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
						    sub.sbadsc as subacao,
						    (SELECT cast(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid , sd.sbdano) as numeric(20,2) ) ) AS valorsubacao,
							(select array_to_string(array(select es1.empnumero from par.empenho es1
                            								inner join par.empenhosubacao es2 on es2.empid = es1.empid
                            							  where empstatus <> 'I' and es2.sbaid = sd.sbaid
                                                          	and es2.eobano = sd.sbdano
                                                            and es2.eobstatus = 'A'
                                                            and es1.empsituacao ilike '%efetivado%'), ', ') ) as empenho
						FROM
							par.processopar prp
						    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
						    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
						    left join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
						    --left join par.empenho emp on emp.empid = ems.empid
						    inner join par.subacao sub on sub.sbaid = sd.sbaid
						    inner join par.acao a on a.aciid = sub.aciid
						    inner join par.pontuacao pon on pon.ptoid = a.ptoid
						    inner join par.criterio c on c.crtid = pon.crtid
						    inner join par.indicador i on i.indid = c.indid
						    inner join par.area are on are.areid = i.areid
						    inner join par.dimensao d on d.dimid = are.dimid
						WHERE
							prp.prpid = $prpid
							and prp.prpstatus = 'A'
							and sd.sbdid in (".implode(',', $arrSub).")
                            and sub.sbastatus = 'A'
                        GROUP BY
                        	d.dimcod, are.arecod, i.indcod, sub.sbaordem, sub.sbadsc, sd.sbaid , sd.sbdano
						ORDER BY d.dimcod, are.arecod, i.indcod, sub.sbaordem";

	} else {

		$sql = "SELECT

						    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
						    sub.sbadsc as subacao,
						    (SELECT cast(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid , sd.sbdano) as numeric(20,2) ) ) AS valorsubacao,
							(select array_to_string(array(select es1.empnumero from par.empenho es1
                            								inner join par.empenhosubacao es2 on es2.empid = es1.empid
                            							  where empstatus <> 'I' and es2.sbaid = sd.sbaid and empcodigoespecie <> '03'
                                                          	and es2.eobano = sd.sbdano
                                                            and es2.eobstatus = 'A'
                                                            and es1.empsituacao ilike '%efetivado%'), ', ') ) as empenho
						FROM
							par.processopar prp
						    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
						    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
						    inner join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
						    --inner join par.empenho emp on emp.empid = ems.empid
						    inner join par.subacao sub on sub.sbaid = ems.sbaid
						    inner join par.acao a on a.aciid = sub.aciid
						    inner join par.pontuacao pon on pon.ptoid = a.ptoid
						    inner join par.criterio c on c.crtid = pon.crtid
						    inner join par.indicador i on i.indid = c.indid
						    inner join par.area are on are.areid = i.areid
						    inner join par.dimensao d on d.dimid = are.dimid
						WHERE
							prp.prpid = $prpid
							and prp.prpstatus = 'A'
							and sd.sbdid in (".implode(',', $arrSub).")
                            and sub.sbastatus = 'A'
                        GROUP BY
                        	d.dimcod, are.arecod, i.indcod, sub.sbaordem, sub.sbadsc, sd.sbaid , sd.sbdano
						ORDER BY d.dimcod, are.arecod, i.indcod, sub.sbaordem";
	}
//ver($sql, d);
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso,
											case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
										from par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpstatus = 'A' and p.prpid = $prpid" );

			$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
											          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
											FROM  par.entidade ent
											INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
											INNER JOIN territorios.estado est on est.estuf = ent2.estuf
											LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
											where 
												ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
												ent.entstatus='A' AND
												ent2.entstatus='A' AND 
												ent.inuid = ".$arDadosProcesso['inuid']);

	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
				INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpstatus = 'A' and p.prpid = $prpid";

	$arCronograma = $db->pegaLinha( $sql );
        
        # Caso possua em sessao a data inicial e final essas serao carregadas no termo de compromisso
        if( $_SESSION['par']['cronogramainicial'] ){
		$arCronograma['cronogramainicial'] = $_SESSION['par']['cronogramainicial'];
	}
	if( $_SESSION['par']['cronogramaFinal'] ){
		$arCronograma['cronogramafinal'] = $_SESSION['par']['cronogramaFinal'];
	}
        
        #Buscar primeira data de vigencia
        $sqlDataInicioVigencia = "SELECT dopdatainiciovigencia FROM par.documentopar WHERE prpid = $prpid AND dopstatus <> 'E' ORDER BY dopid asc LIMIT 1";  
        $arCronograma['cronogramainicial'] = $db->pegaUm($sqlDataInicioVigencia);  

	#Recupera Empenhos somado aos reforco
	$arrEmpenho = Array();
	if( is_array($arrSub) && $arrSub[0] ){
		$arrEmpenho = getEmpenhoAgrupadoSubacao($arrSub);
	}  
        
	$cont = 1;
	$exerc = substr($arDadosProcesso['prpnumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO <br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['prpnumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA SECRETARIA DE EDUCAÇÃO DO ESTADO</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 		'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arrSecretaria['secretario'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arrSecretaria['cpfsecretario']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	
	/*if($mdoid == 29){ #PAR_Termo de Compromisso_Municipios_PROINFÂNCIA_Mob_e_Equip
		$sql = "select
					sov.preid,
					oi.obrnumprocessoconv,
				    oi.numconvenio,
				    oi.obranoconvenio
				from
					par.subacaoobravinculacao sov
				    inner join obr as.obr ainfraestrutura oi on oi.preid = sov.preid
				    inner join par.subacaodetalhe sd on sd.sbaid = sov.sbaid and sd.sbdano = sov.sovano
				where
					oi.obsstatus = 'A'
				    and sd.sbdid in (".implode(',', $arrSub).")";
		$arrSubVinc = $db->carregar($sql);
		$arrSubVinc = $arrSubVinc ? $arrSubVinc : array();

			foreach ($arrSubVinc as $key => $valor) {

				$obrnumprocessoconv = trim($valor['obrnumprocessoconv']);
				if( $key == 0 && !empty( $obrnumprocessoconv ) ){
					$tabela.= '<tr style="text-align: center;">
									<td colspan="3"><b>Dados da Obra Atendida</b></td>
								</tr>
								<tr>
									<td colspan="3" valign="top">
									<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
											<tr align="center" valign="top">
												<td width="20%"><b>ID OBRA ATENDIDA</b></td>
												<td width="40%"><b>Nº PROCESSO DA OBRA ATENDIDA</b></td>
												<td width="40%"><b>Nº CONVÊNIO/ANO DA OBRA ATENDIDA</b></td>
											</tr>';
				}
				if($obrnumprocessoconv){
					$tabela .= '	<tr>
										<td>'.$valor['preid'].'</td>
										<td>'.$obrnumprocessoconv.'</td>
										<td>'.$valor['numconvenio'].'/'.$valor['obranoconvenio'].'</td>
									</tr>';
				}

				if( $key == 0 && !empty( $obrnumprocessoconv ) ){
					$tabela.= '		</table>
							</td>
						</tr>';
				}
		}
	}*/
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="10%"><b>Localização</b></td>
									<td width="60%"><b>Subação</b></td>
									<td width="15%"><b>Valor da Subação</b></td>
									<td width="15%"><b>Empenhos</b></td>
								</tr>';

		$totalVLR = 0;

		foreach ($arDadosItem as $key => $valor) {
			$totalVLR += (float) $valor['valorsubacao'];
			$tabela .= '<tr>
							<td>'.$valor['codigo'].'</td>
							<td>'.$valor['subacao'].'</td>
							<td style="text-align: right;">R$ '.($valor['valorsubacao'] ? number_format($valor['valorsubacao'],2,",",".") : '').'</td>
							<td>'.$valor['empenho'].'</td>
						</tr>';
		}
			$_SESSION['par']['totalVLR'] = $totalVLR;
			$tabela .= '
							<tr>
								<td align="center" colspan="2"><b>Total Geral</b></td>
								<td style="text-align: right;">R$ '.number_format($totalVLR,2,",",".").'</td>
								<td></td>
							</tr>
						</table>
					</td>
				</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
									<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr style="text-align: left;">
						<td colspan="3">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: center;">
									<td colspan="3"><b>EMPENHOS</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="05%"><b>Número</b></td>
									<td width="60%"><b>Valor</b></td>
								</tr>';
					$totalEmp = 0;
					foreach ($arrEmpenho as $v) {
						$totalEmp += (float)$v['valor'];
						$tabela.='<tr valign="top">
									<td width="50%" align="center">'.$v['empnumero'].'</td>
									<td width="50%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
								</tr>';
					}

					$tabela.= '<tr>
									<td align="center"><b>Total Empenho</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
								</tr>
							</table>
						</td>
					</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – ETAPAS OU FASES (SE HOUVER)</b></td>
					</tr>
					';
                                        $arrRafComplemento = montaTabelaRafComplementoSubacao($prpid);
                                        $tabela .= $arrRafComplemento['tabela'];
	$tabela .= '</table><br><br>';

	return $tabela;
}

function montaTabelaSubacaoEmpenhoMunicipio( $prpid, $arrSub, $mdoid, $iueid = '' ){
	global $db;

	$sql = "SELECT

						    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
						    sub.sbadsc as subacao,
						    (SELECT cast(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid , sd.sbdano) as numeric(20,2) ) ) AS valorsubacao,
							(select array_to_string(array(select es1.empnumero from par.empenho es1
                            								inner join par.empenhosubacao es2 on es2.empid = es1.empid
                            							  where empstatus <> 'I' and es2.sbaid = sd.sbaid
                                                          	and es2.eobano = sd.sbdano
                                                            and es2.eobstatus = 'A'
                                                            and es1.empsituacao ilike '%efetivado%'), ', ') ) as empenho
						FROM
							par.processopar prp
						    inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
						    inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
						    inner join par.empenhosubacao ems on ems.sbaid = sd.sbaid and ems.eobano = sd.sbdano and eobstatus = 'A'
						    --inner join par.empenho emp on emp.empid = ems.empid
						    inner join par.subacao sub on sub.sbaid = ems.sbaid
						    inner join par.acao a on a.aciid = sub.aciid
						    inner join par.pontuacao pon on pon.ptoid = a.ptoid
						    inner join par.criterio c on c.crtid = pon.crtid
						    inner join par.indicador i on i.indid = c.indid
						    inner join par.area are on are.areid = i.areid
						    inner join par.dimensao d on d.dimid = are.dimid
						WHERE
							prp.prpid = $prpid
							and prp.prpstatus = 'A'
							and sd.sbdid in (".implode(',', $arrSub).")
                            and sub.sbastatus = 'A'
                        GROUP BY
                        	d.dimcod, are.arecod, i.indcod, sub.sbaordem, sub.sbadsc, sd.sbaid , sd.sbdano
						ORDER BY d.dimcod, are.arecod, i.indcod, sub.sbaordem";

	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();

	$arDadosProcesso = $db->pegaLinha( "select distinct iu.itrid, iu.inuid, p.prpnumeroprocesso,
											case when iu.estuf is null then iu.mun_estuf else iu.estuf end as estuf
										from	par.processopar p
										    inner join par.instrumentounidade iu on iu.inuid = p.inuid
										where p.prpstatus = 'A' and p.prpid = $prpid" );

	$arrSecretaria = $db->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
									          ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
									FROM  par.entidade ent
									INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_PREFEITURA."
									INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
									where 
										ent.dutid = ".DUTID_PREFEITO." AND
										ent.entstatus='A' AND
										ent2.entstatus='A' AND 
										ent.inuid = ".$arDadosProcesso['inuid']);


	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
				INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpstatus = 'A' and p.prpid = $prpid";

	$arCronograma = $db->pegaLinha( $sql );
        
        # Caso possua em sessao a data inicial e final essas serao carregadas no termo de compromisso
        if( $_SESSION['par']['cronogramainicial'] ){
		$arCronograma['cronogramainicial'] = $_SESSION['par']['cronogramainicial'];
	}
	if( $_SESSION['par']['cronogramaFinal'] ){
		$arCronograma['cronogramafinal'] = $_SESSION['par']['cronogramaFinal'];
	}
        
        #Buscar primeira data de vigencia
        $sqlDataInicioVigencia = "SELECT dopdatainiciovigencia FROM par.documentopar WHERE prpid = $prpid AND dopstatus <> 'E' ORDER BY dopid asc LIMIT 1";  
        $arCronograma['cronogramainicial'] = $db->pegaUm($sqlDataInicioVigencia);  

	#Recupera Empenhos somado aos reforco
	$arrEmpenho = Array();
	if( is_array($arrSub) && $arrSub[0] ){
		$arrEmpenho = getEmpenhoAgrupadoSubacao($arrSub);
	}  

	$sql = "SELECT DISTINCT
				u.usunome as prefeito, u.usucpf as cpf
			FROM
				par.usuarioresponsabilidade ur
			    inner join seguranca.usuario u on u.usucpf = ur.usucpf
			    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
			WHERE
				ur.pflcod = ".PAR_PERFIL_PREFEITO."
	            and us.sisid = 23
			    and ur.muncod = (select muncod from par.processopar where prpid = $prpid and prpstatus = 'A')
			    and ur.rpustatus = 'A'
			    and us.suscod = 'A'";
	$arPrefeito = $db->pegaLinha( $sql );

	$cont = 1;
	$exerc = substr($arDadosProcesso['prpnumeroprocesso'], 11, 4);
	$tabela = '<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">';
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>EXTRATO DE EXECUÇÃO DO<br>PLANO DE AÇÕES ARTICULADAS – PAR</b></td>
					</tr>
					<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO ENTE FEDERADO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - PROGRAMA(S)</b><br>PLANO DE AÇÕES ARTICULADAS</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - EXERCÍCIO</b><br>'.$exerc.'</td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - Nº PROCESSO</b><br>'.$arDadosProcesso['prpnumeroprocesso'].'</td>
					</tr>
					<tr>
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME DA PREFEITURA</b><br>'.$arrSecretaria['entnome'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - N.º DO CNPJ</b><br>'.formatar_cpf_cnpj($arrSecretaria['entnumcpfcnpj']).'</td>
					</tr>
					<tr>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - ENDEREÇO</b><br>'.$arrSecretaria['endlog'].' '.$arrSecretaria['endnum'].' - '.$arrSecretaria['endbai'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - MUNICÍPIO</b><br>'.$arrSecretaria['mundescricao'].'</td>
						<td><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - UF</b><br>'.$arrSecretaria['estuf'].'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaEntidadeExecutora($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	$tabela .= 	'
					<tr style="text-align: left;">
						<td colspan="3"><b>IDENTIFICAÇÃO DO(A) PREFEITO(A)</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="2"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - NOME</b><br>'.$arPrefeito['prefeito'].'</td>
						<td valign="top"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' - CPF</b><br>'.formatar_cpf_cnpj($arPrefeito['cpf']).'</td>
					</tr>';
	if( !empty($iueid) ){
		$tabelaEntidade = carregaTabelaExecutor($iueid, $cont);
		$tabela .= $tabelaEntidade['tabela'];
		$cont = $tabelaEntidade['cont'];
	}
	/*if($mdoid == 29){ #PAR_Termo de Compromisso_Municipios_PROINFÂNCIA_Mob_e_Equip
		$sql = "select
					sov.preid,
					oi.obrnumprocessoconv,
				    oi.numconvenio,
				    oi.obranoconvenio
				from
					par.subacaoobravinculacao sov
				    inner join obr as.ob rainfraestrutura oi on oi.preid = sov.preid
				    inner join par.subacaodetalhe sd on sd.sbaid = sov.sbaid and sd.sbdano = sov.sovano
				where
					oi.obsstatus = 'A'
				    and sd.sbdid in (".implode(',', $arrSub).")";
		$arrSubVinc = $db->carregar($sql);
		$arrSubVinc = $arrSubVinc ? $arrSubVinc : array();

			foreach ($arrSubVinc as $key => $valor) {

				$obrnumprocessoconv = trim($valor['obrnumprocessoconv']);
				if( $key == 0 && !empty( $obrnumprocessoconv ) ){
					$tabela.= '<tr style="text-align: center;">
									<td colspan="3"><b>Dados da Obra Atendida</b></td>
								</tr>
								<tr>
									<td colspan="3" valign="top">
									<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
											<tr align="center" valign="top">
												<td width="20%"><b>ID OBRA ATENDIDA</b></td>
												<td width="40%"><b>Nº PROCESSO DA OBRA ATENDIDA</b></td>
												<td width="40%"><b>Nº CONVÊNIO/ANO DA OBRA ATENDIDA</b></td>
											</tr>';
				}
				if($obrnumprocessoconv){
					$tabela .= '	<tr>
										<td>'.$valor['preid'].'</td>
										<td>'.$obrnumprocessoconv.'</td>
										<td>'.$valor['numconvenio'].'/'.$valor['obranoconvenio'].'</td>
									</tr>';
				}

				if( $key == 0 && !empty( $obrnumprocessoconv ) ){
					$tabela.= '		</table>
							</td>
						</tr>';
				}
		}
	}*/
		$tabela .= '<tr style="text-align: center;">
						<td colspan="3"><b>IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS</b></td>
					</tr>
					<tr>
						<td colspan="3" valign="top">
						<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr align="center" valign="top">
									<td width="10%"><b>Localização</b></td>
									<td width="60%"><b>Subação</b></td>
									<td width="15%"><b>Valor da Subação</b></td>
									<td width="15%"><b>Empenhos</b></td>
								</tr>';
		$totalVLR = 0;

		foreach ($arDadosItem as $key => $valor) {
			$totalVLR += (float) $valor['valorsubacao'];
			$tabela .= '<tr>
							<td>'.$valor['codigo'].'</td>
							<td>'.$valor['subacao'].'</td>
							<td style="text-align: right;">R$ '.($valor['valorsubacao'] ? number_format($valor['valorsubacao'],2,",",".") : '').'</td>
							<td>'.$valor['empenho'].'</td>
						</tr>';
		}
			$_SESSION['par']['totalVLR'] = $totalVLR;
			$tabela .= '
							<tr>
								<td align="center" colspan="2"><b>Total Geral</b></td>
								<td style="text-align: right;">R$ '.number_format($totalVLR,2,",",".").'</td>
								<td></td>
							</tr>
						</table>
					</td>
				</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – CRONOGRAMA DE EXECUÇÃO FÍSICO-FINANCEIRO</b></td>
					</tr>
					<tr style="text-align: left;" valign="top">
						<td colspan="3">
							<table align="left" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: left;" valign="top">
									<td width="50%"><b>Mês Inicial:</b><br>'.$arCronograma['cronogramainicial'].'</td>
									<td width="50%"><b>Mês Final:</b><br>'.$arCronograma['cronogramafinal'].'</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr style="text-align: left;">
						<td colspan="3">
							<table align="center" style="border-style: dotted;" width="100%" border="1" cellspacing="0" cellpadding="2">
								<tr style="text-align: center;">
									<td colspan="3"><b>EMPENHOS</b></td>
								</tr>
								<tr align="center" valign="top">
									<td width="05%"><b>Número</b></td>
									<td width="60%"><b>Valor</b></td>
								</tr>';
					$totalEmp = 0;
					foreach ($arrEmpenho as $v) {
						$totalEmp += (float)$v['valor'];
						$tabela.='<tr valign="top">
									<td width="50%" align="center">'.$v['empnumero'].'</td>
									<td width="50%" align="right">R$ '.number_format($v['valor'], 2, ",", ".").'</td>
								</tr>';
					}

					$tabela.= '<tr>
									<td align="center"><b>Total Empenho</b></td>
									<td style="text-align: right;"><b>R$ '.number_format($totalEmp,2,",",".").'</b></td>
								</tr>
							</table>
						</td>
					</tr>
					<tr style="text-align: left;">
						<td colspan="3"><b>'.str_pad($cont++, 2, 0, STR_PAD_LEFT).' – ETAPAS OU FASES (SE HOUVER)</b></td>
					</tr>
					';
	$tabela .= '</table><br><br>';

	return $tabela;
}

/**
 * Retorna empenhos somados por subacao
 * @author Jair F Foro Santos <jairsantos@mec.gov.br>
 * @global type $db
 * @param array $arrSub
 * @return array $arrEmpenho 
 */
function getEmpenhoAgrupadoSubacao($arrSub){
    global $db;
    
    $sql = "SELECT 
                par.retornacodigosubacao(sd.sbaid) as codigo,
                s.empnumero, 
                s.saldo as valor 
            FROM par.v_dadosempenhosubacao s 
           -- INNER JOIN par.empenhosubacao es ON es.empid = s.empid 
            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = s.sbdid
            WHERE sd.sbdid IN (".implode(',', $arrSub).") AND s.saldo > 0";

    $arrEmpenho = $db->carregar($sql);    
    return ($arrEmpenho) ? $arrEmpenho : array();
}

function listaMinuta( $prpid ){
	global $db;

	$acoes = "'<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaTermoAditivo('||mi.dopid||');\" border=\"0\"> </center>'";

	$sql = "SELECT
				$acoes as acao,
				to_char(mi.dopdatainicio, 'DD/MM/YYYY') as dopdatainicio,
			    mi.dopdiasvigencia,
			    to_char(mi.dopdatafim, 'DD/MM/YYYY') as dopdatafim,
			    to_char(mi.dopdatainclusao, 'DD/MM/YYYY') as dopdatainclusao,
			    us.usunome as usuinclusao,
			    mo.mdonome,
			    mi.dopstatus
			FROM
				par.documentopar mi
			    left join par.modelosdocumentos mo on mo.mdoid = mi.mdoid
			    inner join seguranca.usuario us on us.usucpf = mi.usucpfinclusao
			WHERE
				mi.prpid = $prpid";
	monta_titulo( '', 'Lista de minuta cadastrada' );
	$cabecalho = array("Comando", "Data Inicio", "Dias", "Data Fim", "Data Inclusão", "Usuário Inclusão", "Documento", "Status");
	$db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center');
}

function importarDadosSubacao( $dados ){
	global $db;

	$inuid = $_SESSION['par']['inuid'];
	$anoPara = $dados['anopara'];
	$anoDe = $dados['anode'];
	$sbaid = $dados['sbaid'];

	if (empty($anoPara)) {
		break;
	}

	if (empty($anoDe)) {
		break;
	}
	
	if( $anoPara == $anoDe ){
		echo "O ano a ser imporrtado deve ser diferente do ano a receber a importação.";
		break;
	}
	
	$sql = "SELECT TRUE FROM par.subacaodetalhe WHERE sbaid = $sbaid AND sbdano = $anoDe";
	$possuiDetalhe = $db->pegaUm($sql);
	if( $possuiDetalhe != 't' ){
		echo "O ano a ser imporrtado deve ser diferente do ano a receber a importação.";
		break;
	}
	
	$sql = "SELECT TRUE FROM par.subacaodetalhe WHERE sbaid = $sbaid AND sbdano = $anoPara";
	$possuiDetalhe = $db->pegaUm($sql);
	if( $possuiDetalhe != 't' ){
		$sql = "INSERT INTO par.subacaodetalhe(sbdparecerdemerito, sbaid, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
	  				ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid, sbdplanointerno)
				(SELECT sbdparecerdemerito, sbaid, sbdparecer, sbdquantidade, $anoPara, sbdinicio, sbdfim,
					ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid, sbdplanointerno
				FROM par.subacaodetalhe WHERE sbaid = $sbaid and sbdano = '$anoDe')";
		$db->executar( $sql );
	}
	
	$sql = "SELECT par.importarDadosSubacao($sbaid, $anoDe, $anoPara);";
// 	ver($sql,d);
	$db->executar($sql);
	echo $db->commit();

// 	$db->executar("DELETE FROM par.subacaodetalhe WHERE sbaid = $sbaid and sbdano = '$anoPara'");
// 	$db->executar("DELETE FROM par.subescolas_subitenscomposicao WHERE icoid IN ( SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid = $sbaid and icoano = '$anoPara' )");
// 	$db->executar("DELETE FROM par.subescolas_subitenscomposicao WHERE sesid IN ( SELECT sesid FROM par.subacaoescolas WHERE sbaid = $sbaid and sesano = '$anoPara' )");
// 	$db->executar("DELETE FROM par.subacaoitenscomposicao WHERE sbaid = $sbaid and icoano = '$anoPara'");
// 	$db->executar("DELETE FROM par.subacaoescolas WHERE sbaid = $sbaid and sesano = '$anoPara'");
	
// 	#PAR.SUBACAODETALHE
// 	$sql = "INSERT INTO par.subacaodetalhe(sbdparecerdemerito, sbaid, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
//   				ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid, sbdplanointerno)
// 			(SELECT sbdparecerdemerito, sbaid, sbdparecer, sbdquantidade, $anoPara, sbdinicio, sbdfim,
// 				ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid, sbdplanointerno
// 			FROM par.subacaodetalhe WHERE sbaid = $sbaid and sbdano = '$anoDe')";
// 	$db->executar( $sql );


// 	#PAR.SUBACAOOBRA
// 	$sql = "UPDATE par.subacaoobra SET
// 			 	sobano = $anoPara
// 			WHERE
// 			  	sbaid = $sbaid
// 			  	and sobano = '$anoDe'";
// 	$db->executar( $sql );


// 	/*INSERT INTO par.subacaoobra(sbaid, preid, sobano)
// 			(SELECT sbaid, preid, $anoPara FROM par.subacaoobra WHERE sbaid = $sbaid and sobano = '$anoDe')*/

// 	#PAR.SUBESCOLAS_SUBITENSCOMPOSICAO
// 	$sql = "INSERT INTO par.subescolas_subitenscomposicao(sesid, icoid, seiqtd, seiimportacao, seiqtdtecnico)
// 			(SELECT sesid, icoid, seiqtd, 'S', seiqtdtecnico
// 			FROM par.subescolas_subitenscomposicao
// 			WHERE sesid in ( select sesid from par.subacaoescolas where sbaid = $sbaid and sesano = '$anoDe' )
// 				and icoid in ( select icoid from par.subacaoitenscomposicao where sbaid = $sbaid and icoano = '$anoDe' ))";
// 	$db->executar( $sql );

// 	#PAR.SUBACAOITENSCOMPOSICAO
// 	$sql = "SELECT icoid, icoquantidadetecnico, icovalidatecnico, icoano, icoordem, icodescricao, icoquantidade, coalesce(icovalor, 0) as icovalor, icoquantidadetecnico,
// 			  	coalesce(icovalortotal, 0) as icovalortotal, icostatus, sbaid, unddid, icodetalhe, usucpf, dtatualizacao, picid, icovalidatecnico
// 			FROM par.subacaoitenscomposicao WHERE sbaid = $sbaid and icoano = '$anoDe'";

// 	$arrItens = $db->carregar( $sql );
// 	$arrItens = $arrItens ? $arrItens : array();

// 	foreach ($arrItens as $value) {
// 		extract($value);

// 		$icoordem 				= $icoordem 			? "'".$icoordem."'" 			: 'null';
// 		$icodescricao 			= $icodescricao 		? "'".$icodescricao."'" 		: 'null';
// 		$icoquantidade 			= $icoquantidade 		? "'".$icoquantidade."'" 		: 'null';
// 		$icovalidatecnico 		= $icovalidatecnico 	? "'".$icovalidatecnico."'" : 'null';
// 		$icoquantidadetecnico 	= $icoquantidadetecnico ? "'".$icoquantidadetecnico."'" 	: 'null';
// 		$unddid 				= $unddid 				? "'".$unddid."'" 				: 'null';
// 		$icodetalhe 			= $icodetalhe 			? "'".$icodetalhe."'" 			: 'null';
		
// 		$sql = "INSERT INTO par.subacaoitenscomposicao(icoano, icoordem, icodescricao, icoquantidade, icovalor,
// 	  				icovalortotal, icostatus, sbaid, unddid, icodetalhe, usucpf, dtatualizacao, picid, icovalidatecnico, icoquantidadetecnico)
// 				VALUES($anoPara, $icoordem, $icodescricao, $icoquantidade, $icovalor,
// 				  	$icovalortotal, '$icostatus', '$sbaid', $unddid, $icodetalhe, '$usucpf', '$dtatualizacao', '$picid', $icovalidatecnico, $icoquantidadetecnico) returning icoid";
		
// 		$icoidNovo = $db->pegaUm( $sql );
	
// 		#PAR.SUBESCOLAS_SUBITENSCOMPOSICAO
// 		$sql = "UPDATE par.subescolas_subitenscomposicao SET
// 				  icoid = $icoidNovo
// 				WHERE icoid = $icoid and seiimportacao = 'S'";
// 		$db->executar( $sql );
// 	}

// 	#PAR.SUBACAOESCOLAS
// 	$sql = "SELECT sesid, sesano, escid, sesquantidade, sesstatus, sbaid
// 			FROM par.subacaoescolas WHERE sbaid = $sbaid and sesano = '$anoDe'";
// 	$arrEscolas = $db->carregar( $sql );
// 	$arrEscolas = $arrEscolas ? $arrEscolas : array();

// 	foreach ($arrEscolas as $value){
// 		extract( $value );

// 		$escid 			= $escid 		 ? "'".$escid."'" 			: 'null';
// 		$sesquantidade	= $sesquantidade ? "'".$sesquantidade."'" 	: 'null';

// 		$sql = "INSERT INTO par.subacaoescolas(sesano, escid, sesquantidade, sesstatus, sbaid)
// 				VALUES('$anoPara', $escid, $sesquantidade, '$sesstatus', '$sbaid') RETURNING sesid";
// 		$sesidNovo = $db->pegaUm( $sql );

// 		#PAR.SUBESCOLAS_SUBITENSCOMPOSICAO
// 		$sql = "UPDATE par.subescolas_subitenscomposicao SET
// 				  sesid = $sesidNovo
// 				WHERE sesid = $sesid and seiimportacao = 'S'";
// 		$db->executar( $sql );
// 	}

// 	#PAR.SUBESCOLAS_SUBITENSCOMPOSICAO
// 	/*
// 	 * Altera o campo da tabela seiimportacao para 'N', para que não aconteça erro de importação para os proximos anos
// 	 */
// 	$sql = "UPDATE par.subescolas_subitenscomposicao SET
// 				seiimportacao = 'N'
// 			WHERE sesid in ( select sesid from par.subacaoescolas where sbaid = $sbaid and sesano = '$anoPara' )
// 				and icoid in ( select icoid from par.subacaoitenscomposicao where sbaid = $sbaid and icoano = '$anoPara' )";
// 	$db->executar( $sql );

// 	echo $db->commit();
}

function verificaExisteObra( $dados ){
	global $db;

	$anoPara = $dados['anopara'];
	$anoDe = $dados['anode'];
	$sbaid = $dados['sbaid'];

	$total = $db->pegaUm("select count(sobid) from par.subacaoobra where sbaid = $sbaid and sobano = '$anoDe'");
	echo $total;
}

function retornaHTMLTermo( $terid ){

	global $db;

	$sql = "SELECT proid FROM par.termocompromissopac WHERE terid = ".$terid;
	$proid = $db->pegaUm( $sql );

	if( $proid ){

		if( $_SESSION['par']['muncod'] ){
			$filtro = " muncod = '".$_SESSION['par']['muncod']."'";
			$esfera = 'M';
		} else {
			$estuf = $_SESSION['par']['estuf'];
			$filtro = " estuf = '".$estuf."'";
			$esfera = 'E';
		}

		if( $esfera == 'M' ){
			$sql = "SELECT
						upper(mun.mundescricao) as municipio,
						est.estuf,
						ent2.entnome as prefeitura,
						coalesce(ent2.endlog,'#') || '/' || coalesce(ent2.endbai,'#') as prefendereco,
						ent2.entnumcpfcnpj as cnpjprefeitura,
						ent.entnome as prefeito,
						ent.entnumrg as rg,
						ent.entnumcpfcnpj as cpf,
						est.estdescricao as estado
					FROM  par.entidade ent
					INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_PREFEITURA."
					INNER JOIN territorios.estado est on est.estuf = ent2.estuf
					LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
					where 
						ent.dutid = ".DUTID_PREFEITO." AND
						ent.entstatus='A' AND
						ent2.entstatus='A' AND 
						ent2.muncod = '".$_SESSION['par']['muncod']."'";

			$munDados = $db->carregar($sql);


			$sql = "SELECT
					pre.preid
				FROM
					obras.preobra pre
				INNER JOIN par.empenhoobra   emp ON emp.preid = pre.preid and eobstatus = 'A'
				INNER JOIN par.empenho   emn ON emn.empid = emp.empid and empstatus <> 'I'
				INNER JOIN par.processoobra   pro ON pro.pronumeroprocesso = emn.empnumeroprocesso and pro.prostatus = 'A' 
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptoclassificacaoobra = pro.protipo
				WHERE
					pro.proid = '".$proid."' AND pre.muncod = '{$_SESSION['par']['muncod']}' AND pre.preesfera = 'M'
				GROUP BY
					pre.preid, pre.predescricao, pto.ptodescricao, prevalorobra
				ORDER BY
					pto.ptodescricao,pre.predescricao";

			$preids = $db->carregarColuna( $sql );

		} else {
			$sql = "SELECT
						upper(est.estdescricao) as municipio,
						est.estuf,
						ent2.entnome as prefeitura,
						coalesce(ent2.endlog,'#') || '/' || coalesce(ent2.endbai,'#') as prefendereco,
						ent2.entnumcpfcnpj as cnpjprefeitura,
						ent.entnome as prefeito,
						ent.entnumrg as rg,
						ent.entnumcpfcnpj as cpf,
						est.estdescricao as estado
					FROM  par.entidade ent
					INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = ".DUTID_SECRETARIA_ESTADUAL."
					INNER JOIN territorios.estado est on est.estuf = ent2.estuf
					where 
						ent.dutid = ".DUTID_SECRETARIO_ESTADUAL." AND
						ent.entstatus='A' AND
						ent2.entstatus='A' AND 
						ent2.estuf = '$estuf'";
			
			$munDados = $db->carregar($sql);

			$sql = "SELECT
					pre.preid
				FROM
					obras.preobra pre
				INNER JOIN par.empenhoobra   emp ON emp.preid = pre.preid and eobstatus = 'A'
				INNER JOIN par.empenho   emn ON emn.empid = emp.empid and empstatus <> 'I'
				INNER JOIN par.processoobra   pro ON pro.pronumeroprocesso = emn.empnumeroprocesso and pro.prostatus = 'A' 
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptoclassificacaoobra = pro.protipo
				WHERE
					pro.proid = '".$proid."' AND pre.estuf = '{$estuf}' AND pre.preesfera = 'E'
				GROUP BY
					pre.preid, pre.predescricao, pto.ptodescricao, prevalorobra
				ORDER BY
					pto.ptodescricao,pre.predescricao";

			$preids = $db->carregarColuna( $sql );
		}

		$termo = $terid;

		$ano   = $db->pegaUm("SELECT to_char(terdatainclusao,'YYYY') FROM par.termocompromissopac WHERE terid = {$termo}");

		//$preids = explode(',',$_REQUEST['arPreid']);
		$tabela = "<div>";
		$x=1;
		foreach( $preids as $preid ){

			$sql = "SELECT
						preid || ' - ' || predescricao as obra,
						prelogradouro as logradouro,
						precomplemento as complemento,
						ptodescricao as tipoobra,
						prevalorobra as vlrobra
					FROM
						obras.preobra pre
					INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
					WHERE
						preid = ".$preid;
			$dado   = $db->pegaLinha($sql);
			$obra 		 = $dado['obra'] 		!= '' ? "".wordwrap ( $dado['obra'], 50, '<br />')."<br />" : "";
			$logradouro  = $dado['logradouro']  != '' ? "".wordwrap ( $dado['logradouro'], 50, '<br />')."<br />" : "";
			$complemento = $dado['complemento'] != '' ? "".wordwrap ( $dado['complemento'], 50, '<br />')."<br />" : "";
			$tabela .= "<div style=\"display: table;\">
							<div class=\"P_2\">{$x} )</div>
							<div class=\"P_4\">".$obra."
											   ".$logradouro."
											   ".$complemento."
											   ".$dado['tipoobra']."R$ ".number_format($dado['vlrobra'],2,',','.')."</div>
						</div><br />";
			$x++;
		}
		$tabela .= "</div>";

	}

	$tipoObra = ($_REQUEST['tipoobra'] == 'Q') ? 'à Quadras' : 'ao Pró-Infância';
	$tipoObra2 = ($_REQUEST['tipoobra'] == 'Q') ? 'quadra(s) esportiva(s) escolar(es) coberta(s)' : 'unidade(s) de educação infantil';

	if( $esfera == 'M' ){

	return '<html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8" />
			<title>Termo de Compomisso</title>
			<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
			<meta name="author" content="Neuza" />
			<meta name="created" content="2008-01-31T17:54:00" />
			<meta name="changedby" content="mec" />
			<meta name="changed" content="2008-02-07T18:09:00" />
			<style type="text/css">
				@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
				.divT_1{ margin-left: 2.5598in; text-align: center; }
				.P_1{ font-family: Arial; font-size: 12pt; margin-left: 1.5598in; margin-right: 0in; text-align: justify ! important; clear: both; }
				.P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 2in; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
				.P_3{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
				.P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
				.Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
				.T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.N{ font-weight: bold; }
				.I{ font-style: italic; }
				.S{ text-decoration: underline; }
				p.quebra    { page-break-before: always }
			</style>
			<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
		</head>
		<body>
			<div>
				<p class="T_1">TERMO DE COMPROMISSO<br />PAC2'.str_pad($termo,5,0, STR_PAD_LEFT).'/'.$ano.'</p>
				<p class="P_1">
					A Prefeitura Municipal de <strong>'.$munDados[0]['municipio'].'('.$munDados[0]['estuf'].')</strong>, com sede na <strong>'.$munDados[0]['prefendereco'].'</strong>,	inscrita no CNPJ/MF sob o n° <strong>'.$munDados[0]['cnpjprefeitura'].'</strong>,
					representada pelo(a) prefeito(a) <strong>'.$munDados[0]['prefeito'].'</strong>, brasileiro(a), portador(a) da carteira de identidade n° <strong>'.$munDados[0]['rg'].'</strong> e do
					CPF n° <strong>'.$munDados[0]['cpf'].'</strong>, residente e domiciliado(a) no estado de <strong>'.$munDados[0]['estado'].'</strong>, considerando o que dispõe a Lei n° 11.578,
					de 26 de novembro de 2007, compromete-se a executar as ações relativas a '.$tipoObra.',
					no âmbito do PAC 2, de acordo com as especificações do(s) projeto(s) fornecido(s) ou aprovado(s) pelo Fundo Nacional de
					Desenvolvimento da Educação – FNDE e em conformidade com os requisitos da lei supramencionada e demais condicionantes, a seguir descritas:
				</p>
				<p class="P_1">
					<br />
					I – Executar todas as atividades inerentes à construção de '.count($preids).' ('.strtolower(extenso(count($preids),true,false)).') '.$tipoObra2.', situada(s) em:
				</p>
					'.$tabela.'
				<p class="P_1">
					<br />
					II - Executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação no âmbito do PAC 2
					em estrito acordo com os projetos executivos fornecidos ou aprovados pelo FNDE/MEC (desenhos técnicos, memoriais descritivos e especificações),
					observando os critérios de qualidade técnica que atendam as determinações da Associação Brasileira de Normas Técnicas (ABNT),
					bem como os prazos e os custos previstos;
				</p>
				<p class="P_1">
					<br />
					III - Utilizar os recursos financeiros transferidos pelo FNDE/MEC exclusivamente no cumprimento do objeto pactuado;
					responsabilizando-se para que a movimentação dos recursos ocorra somente para o pagamento das despesas previstas neste
					Termo de Compromisso ou para aplicação financeira, devendo a movimentação realizar-se, exclusivamente, mediante cheque
					nominativo ao credor ou ordem bancária, Transferência Eletrônica de Disponibilidade (TED) ou outra modalidade de saque
					autorizada pelo Banco Central do Brasil em que fique identificada a destinação e, no caso de pagamento, o credor;
				</p>
				<p class="P_1">
					<br />
					IV - Nomear profissional devidamente habilitado, da área de engenharia civil ou arquitetura, para exercer as funções de
					fiscalização da(s) obra(s), com emissão da respectiva Anotação de Responsabilidade Técnica (ART/CREA);
				</p>
				<p class="P_1">
					<br />
					V - Responsabilizar-se, com recursos próprios, por obras e serviços de terraplenagem e contenções, infraestrutura de
					redes (água potável, esgotamento sanitário, energia elétrica e telefonia), bem como por todos os serviços
					necessários à implantação do(s) empreendimento(s) no(s) terreno(s) tecnicamente aprovado(s), uma vez que os
					valores a serem repassados pelo FNDE/MEC referem-se exclusivamente aos serviços de engenharia constantes nas planilhas
					orçamentárias do(s) projeto(s) pactuado(s) e aprovado(s);
				</p>
				<p class="P_1">
					<br />
					VI - Garantir, com recursos próprios, a conclusão da(s) obra(s) acima pactuada(s) e sua entrega à população,
					no caso de os valores transferidos se revelarem insuficientes para cobrir todas as despesas relativas à implantação;
				</p>
				<p class="P_1">
					<br />
					VII - Indicar agência do Banco do Brasil S/A onde deverão ser depositados os recursos referentes à construção da(s)
					obra(s) pactuada(s) neste Termo de Compromisso, visando à abertura de conta corrente específica pelo FNDE/MEC,
					a qual estará isenta do pagamento de taxas e tarifas bancárias, em conformidade com o Acordo de Cooperação Mútua celebrado com o FNDE,
					disponível no <a href=\"www.fnde.gov.br\" style=\"color: blue\">sítio: www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					VIII - Providenciar a regularização da referida conta corrente na agência indicada, procedendo à entrega e à chancela dos documentos
					necessários à sua movimentação, de acordo com as normas bancárias vigentes, outorgando ao FNDE/MEC a condição de, sempre que necessário,
					obter junto ao banco os saldos e extratos da referida conta, inclusive os das aplicações financeiras, bem como o direito de solicitar
					seu encerramento, bloqueio, estorno ou transferência de valores, nos casos estipulados na Resolução CD/FNDE Nº 69/2011, de que este
					Termo de Compromisso constitui anexo;
				</p>
				<p class="P_1">
					<br />
					IX - Responsabilizar-se pelo acompanhamento das transferências financeiras efetuadas pelo FNDE,
					de forma a garantir a aplicação tempestiva dos recursos creditados a seu favor.
				</p>
				<p class="P_1">
					<br />
					X - Aplicar os recursos recebidos, enquanto não forem utilizados em sua finalidade, obrigatoriamente em caderneta de poupança,
					aberta especificamente para o Programa, quando a previsão do seu uso for igual ou superior a um mês; ou aplicá-los em fundo de
					aplicação financeira de curto prazo ou operação de mercado aberto lastreada em títulos da dívida pública, se a sua utilização
					ocorrer em prazo inferior a um mês. Responsabilizar-se ainda por efetivar a aplicação financeira vinculada à mesma conta corrente
					na qual os recursos financeiros foram creditados pelo FNDE/MEC, inclusive quando se tratar de caderneta de poupança,
					cuja aplicação poderá se dar mediante vinculação do correspondente número de operação à conta já existente.
				</p>
				<p class="P_1">
					<br />
					XI - Destinar os rendimentos das aplicações financeiras exclusivamente às ações do presente Termo de Compromisso, incluindo-os nas mesmas condições
					de prestação de contas exigidas para os recursos transferidos, devendo tais rendimentos ser obrigatoriamente computados a crédito da conta corrente específica;
				</p>
				<p class="P_1">
					<br />
					XII - Realizar licitações para as contratações necessárias à execução da(s) obra(s) acima pactuadas, obedecendo à Lei n° 8.666, de 21 de
					junho de 1993, e observar que os preços unitários de materiais e serviços utilizados não sejam superiores à mediana daqueles constantes
					do Sistema Nacional de Pesquisa de Custos e Índices da Construção Civil – SINAPI, mantido pela Caixa Econômica Federal. Em condições
					especiais, devidamente justificadas em Relatório Técnico circunstanciado, aprovado pela Diretoria de Programas e Projetos Educacionais
					(DIRPE/FNDE), exclusivamente para itens não disponíveis no SINAPI poderão ser praticados preços específicos,
					sem prejuízo da avaliação dos órgãos de controle internos e externos;
				</p>
				<p class="P_1">
					<br />
					XIII - Cientificar mensalmente o FNDE/MEC sobre a aplicação dos recursos e a consecução do objeto conforme o previsto,
					por meio do preenchimento dos dados e informações sobre a(s) obra(s) no Módulo de Monitoramento de Obras do SIMEC
					(Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação), no endereço eletrônico <a href=\"http://simec.mec.gov.br\" style=\"color: blue\">http://simec.mec.gov.br</a>,
					utilizando para tanto a senha do Plano de Ações Articuladas (PAR), fornecida pela Secretaria de Educação Básica (SEB/MEC);
				</p>
				<p class="P_1">
					<br />
					XIV - Assegurar e destacar obrigatoriamente a participação do Governo Federal e do FNDE em toda e qualquer ação, promocional ou não,
					relacionada com a execução do objeto pactuado acima, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do Governo Federal
					em placas, cartazes, faixas e painéis de identificação da(s) obra(s) custeada(s) com os recursos transferidos à conta do Programa,
					obedecendo ao que está disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo e
					Gestão Estratégica da Presidência da República;
				</p>
				<p class="P_1">
					<br />
					XV - Manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução deste Termo de Compromisso,
					para fins de fiscalização, de acompanhamento e de avaliação dos resultados obtidos;
				</p>
				<p class="P_1">
					<br />
					XVI - Facilitar a supervisão e a fiscalização do FNDE/MEC, permitindo-lhe efetuar acompanhamento no local e fornecendo, sempre que solicitado, as informações e os
					documentos relacionados com a execução do objeto deste Instrumento, especialmente no que se refere ao exame da documentação relativa à licitação e aos contratos;
				</p>
				<p class="P_1">
					<br />
					XVII - Permitir o livre acesso de servidores do Sistema de Controle Interno do Poder Executivo Federal (Secretaria Federal de Controle
					– SFC/MF, Delegacia Federal de Controle – DFC ou sua representação no Estado, Secretaria de Controle Interno – CISET) e da Auditoria do
					FNDE, a qualquer tempo e lugar, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente com o
					objeto pactuado no Termo de Compromisso (Anexo I), bem como às obras e serviços a ele referidas, colaborando na obtenção de dados e de
					informações junto à comunidade local sobre os benefícios advindos da implantação do(s) projeto(s), quando em missão de fiscalização e auditoria;
				</p>
				<p class="P_1">
					<br />
					XVIII - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento comprobatório
					de despesa efetuada à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério daquela Autarquia Federal;
				</p>
				<p class="P_1">
					<br />
					XIX - Prestar todo e qualquer esclarecimento sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por órgão do Sistema
					de Controle Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão ou entidade com delegação para esse fim;
				</p>
				<p class="P_1">
					<br />
					XX - Incluir no orçamento anual do Município, ou do estado, os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos estabelecidos no
					§ 1º, do art. 6º, da Lei nº 4.320, de 17 de março de 1964;
				</p>
				<p class="P_1">
					<br />
					XXI - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e ao
					desenvolvimento do ensino, por força do disposto no art. 212 da Constituição Federal;
				</p>
				<p class="P_1">
					<br />
					XXII - Emitir o(s) termo(s) de aceitação definitiva da(s) obra(s), ao final da execução dos recursos, remetendo cópia autenticada do(s) mesmo(s)
					à DIRPE/FNDE para a emissão do(s) termo(s) de conclusão da(s) obra(s) e consolidação deste Termo de Compromisso;
				</p>
				<p class="P_1">
					<br />
					XXIII - Prestar contas ao FNDE/MEC dos recursos recebidos, no prazo e nas condições estipuladas nos artigos 29 e 30
					da Resolução CD/FNDE Nº 13/2011;
				</p>
				<p class="P_1">
					<br />
					XXIV - Manter em seu poder, à disposição do FNDE/MEC, da SEB/MEC, dos órgãos de controle interno e externo e do
					Ministério Público, os comprovantes das despesas efetuadas à conta do Programa, pelo prazo de 10 (dez) anos, contados
					da data da aprovação da prestação de contas anual do FNDE/MEC pelo Tribunal de Contas da União (TCU) a que se refere o exercício do repasse dos recursos,
					a qual será divulgada no sítio eletrônico <a href=\"www.fnde.gov.br\" style=\"color: blue\">www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					XXV - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais
					demandas judiciais relativas a recursos humanos utilizados na execução do objeto deste Termo de Compromisso, bem
					como por todos os ônus tributários ou extraordinários que incidam sobre o presente Instrumento, ressalvados aqueles de natureza compulsória,
					lançados automaticamente pela rede bancária arrecadadora;
				</p>
				<p class="P_1">
					<br />
					XXVI - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso.
				</p>
				<p class="quebra">&nbsp;</p>
				<p class="P_3">
					<br />
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que o município
					cumpre com as exigências do art. 169 da Constituição Federal que trata dos limites de despesa com pessoal e,
					que os recursos próprios de responsabilidade do Município estão assegurados, conforme a
					Lei Orgânica Municipal.
				</p>
				<p class="Data">
					<br />
					Brasília/DF, ___ de ____________________ de ______.
				</p>
				<p class="T_1">
					<br />
					<br />
					<br />
					____________________________________________
					<br />
					<br />
					<strong>'.$munDados[0]['prefeito'].'</strong>
					<br />
					PREFEITO(A) MUNICIPAL DE <strong>'.strtoupper($munDados[0]['municipio']).'/'.$munDados[0]['estuf'].'</strong>
				</p>
			</div>
		</body>
	</html>';
	} else {

	return '<html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8" />
			<title>Termo de Compomisso</title>
			<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
			<meta name="author" content="Neuza" />
			<meta name="created" content="2008-01-31T17:54:00" />
			<meta name="changedby" content="mec" />
			<meta name="changed" content="2008-02-07T18:09:00" />
			<style type="text/css">
				@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
				.divT_1{ margin-left: 2.5598in; text-align: center; }
				.P_1{ font-family: Arial; font-size: 12pt; margin-left: 1.5598in; margin-right: 0in; text-align: justify ! important; clear: both; }
				.P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 2in; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
				.P_3{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
				.P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
				.Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
				.T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.N{ font-weight: bold; }
				.I{ font-style: italic; }
				.S{ text-decoration: underline; }
				p.quebra    { page-break-before: always }
			</style>
			<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
		</head>
		<body>
			<div>
				<p class="T_1">TERMO DE COMPROMISSO<br />PAC2'.str_pad($termo,5,0, STR_PAD_LEFT).'/'.$ano.'</p>
				<p class="P_1">
					A Secretaria de Educação do Estado de <strong>'.$munDados[0]['municipio'].'</strong>, com sede na <strong>'.$munDados[0]['prefendereco'].'</strong>, inscrita no CNPJ/MF sob o n° <strong>'.$munDados[0]['cnpjprefeitura'].'</strong>,
					representada pelo(a) Secretário(a) <strong>'.$munDados[0]['prefeito'].'</strong>, brasileiro(a), portador(a) da carteira de identidade n° <strong>'.$munDados[0]['rg'].'</strong> e do
					CPF n° <strong>'.$munDados[0]['cpf'].'</strong>, residente e domiciliado(a) no estado de <strong>'.$munDados[0]['estado'].'</strong>, considerando o que dispõe a Lei n° 11.578,
					de 26 de novembro de 2007, compromete-se a executar as ações relativas a construção de quadra(s) poliesportiva(s) escolar(es),
					no âmbito do PAC 2, de acordo com as especificações do(s) projeto(s) fornecido(s) ou aprovado(s) pelo Fundo Nacional de
					Desenvolvimento da Educação – FNDE e em conformidade com os requisitos da lei supramencionada e demais condicionantes, a seguir descritas:
				</p>
				<p class="P_1">
					<br />
					I – Executar todas as atividades inerentes à construção de '.count($preids).' ('.strtolower(extenso(count($preids),true,false)).') quadra(s) poliesportiva(s) escolar(es) coberta(as), situada(s) em:
				</p>
					'.$tabela.'
				<p class="P_1">
					<br />
					II - Executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação no âmbito do PAC 2 em estrito acordo
					com os projetos executivos fornecidos ou aprovados pelo FNDE/MEC (desenhos técnicos, memoriais descritivos e especificações),
					observando os critérios de qualidade técnica que atendam as determinações da Associação Brasileira de Normas Técnicas (ABNT),
					bem como os prazos e os custos previstos;
				</p>
				<p class="P_1">
					<br />
					III - Utilizar os recursos financeiros transferidos pelo FNDE/MEC exclusivamente no cumprimento do objeto pactuado;
					responsabilizando-se para que a movimentação dos recursos ocorra somente para o pagamento das despesas previstas neste
					Termo de Compromisso ou para aplicação financeira, devendo a movimentação realizar-se, exclusivamente, mediante cheque
					nominativo ao credor ou ordem bancária, Transferência Eletrônica de Disponibilidade (TED) ou outra modalidade de saque
					autorizada pelo Banco Central do Brasil em que fique identificada a destinação e, no caso de pagamento, o credor;
				</p>
				<p class="P_1">
					<br />
					IV - Nomear profissional devidamente habilitado, da área de engenharia civil ou arquitetura, para exercer as funções de
					fiscalização da(s) obra(s), com emissão da respectiva Anotação de Responsabilidade Técnica (ART/CREA);
				</p>
				<p class="P_1">
					<br />
					V - Responsabilizar-se, com recursos próprios, por obras e serviços de terraplenagem e contenções, infraestrutura de
					redes (água potável, esgotamento sanitário, energia elétrica e telefonia), bem como por todos os serviços
					necessários à implantação do(s) empreendimento(s) no(s) terreno(s) tecnicamente aprovado(s), uma vez que os
					valores a serem repassados pelo FNDE/MEC referem-se exclusivamente aos serviços de engenharia constantes nas planilhas
					orçamentárias do(s) projeto(s) pactuado(s) e aprovado(s);
				</p>
				<p class="P_1">
					<br />
					VI - Garantir, com recursos próprios, a conclusão da(s) obra(s) acima pactuada(s) e sua entrega à população,
					no caso de os valores transferidos se revelarem insuficientes para cobrir todas as despesas relativas à implantação;
				</p>
				<p class="P_1">
					<br />
					VII - Indicar agência do Banco do Brasil S/A onde deverão ser depositados os recursos referentes à construção da(s)
					obra(s) pactuada(s) neste Termo de Compromisso, visando à abertura de conta corrente específica pelo FNDE/MEC,
					a qual estará isenta do pagamento de taxas e tarifas bancárias, em conformidade com o Acordo de Cooperação Mútua celebrado com o FNDE,
					disponível no <a href=\"www.fnde.gov.br\" style=\"color: blue\">sítio: www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					VIII - Providenciar a regularização da referida conta corrente na agência indicada, procedendo à entrega e à chancela dos documentos
					necessários à sua movimentação, de acordo com as normas bancárias vigentes, outorgando ao FNDE/MEC a condição de, sempre que necessário,
					obter junto ao banco os saldos e extratos da referida conta, inclusive os das aplicações financeiras, bem como o direito de solicitar
					seu encerramento, bloqueio, estorno ou transferência de valores, nos casos estipulados na Resolução CD/FNDE Nº 69/2011, de que este
					Termo de Compromisso constitui anexo;
				</p>
				<p class="P_1">
					<br />
					IX - Responsabilizar-se pelo acompanhamento das transferências financeiras efetuadas pelo FNDE,
					de forma a garantir a aplicação tempestiva dos recursos creditados a seu favor.
				</p>
				<p class="P_1">
					<br />
					X - Aplicar os recursos recebidos, enquanto não forem utilizados em sua finalidade, obrigatoriamente em caderneta de poupança,
					aberta especificamente para o Programa, quando a previsão do seu uso for igual ou superior a um mês; ou aplicá-los em fundo de
					aplicação financeira de curto prazo ou operação de mercado aberto lastreada em títulos da dívida pública, se a sua utilização
					ocorrer em prazo inferior a um mês. Responsabilizar-se ainda por efetivar a aplicação financeira vinculada à mesma conta corrente
					na qual os recursos financeiros foram creditados pelo FNDE/MEC, inclusive quando se tratar de caderneta de poupança,
					cuja aplicação poderá se dar mediante vinculação do correspondente número de operação à conta já existente.
				</p>
				<p class="P_1">
					<br />
					XI - Destinar os rendimentos das aplicações financeiras exclusivamente às ações do presente Termo de Compromisso, incluindo-os nas mesmas condições
					de prestação de contas exigidas para os recursos transferidos, devendo tais rendimentos ser obrigatoriamente computados a crédito da conta corrente específica;
				</p>
				<p class="P_1">
					<br />
					XII - Realizar licitações para as contratações necessárias à execução da(s) obra(s) acima pactuadas, obedecendo à Lei n° 8.666, de 21 de
					junho de 1993, e observar que os preços unitários de materiais e serviços utilizados não sejam superiores à mediana daqueles constantes
					do Sistema Nacional de Pesquisa de Custos e Índices da Construção Civil – SINAPI, mantido pela Caixa Econômica Federal. Em condições
					especiais, devidamente justificadas em Relatório Técnico circunstanciado, aprovado pela Diretoria de Programas e Projetos Educacionais
					(DIRPE/FNDE), exclusivamente para itens não disponíveis no SINAPI poderão ser praticados preços específicos,
					sem prejuízo da avaliação dos órgãos de controle internos e externos;
				</p>
				<p class="P_1">
					<br />
					XIII - Cientificar mensalmente o FNDE/MEC sobre a aplicação dos recursos e a consecução do objeto conforme o previsto,
					por meio do preenchimento dos dados e informações sobre a(s) obra(s) no Módulo de Monitoramento de Obras do SIMEC
					(Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação), no endereço eletrônico <a href=\"http://simec.mec.gov.br\" style=\"color: blue\">http://simec.mec.gov.br</a>,
					utilizando para tanto a senha do Plano de Ações Articuladas (PAR), fornecida pela Secretaria de Educação Básica (SEB/MEC);
				</p>
				<p class="P_1">
					<br />
					XIV - Assegurar e destacar obrigatoriamente a participação do Governo Federal e do FNDE em toda e qualquer ação, promocional ou não,
					relacionada com a execução do objeto pactuado acima, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do Governo Federal
					em placas, cartazes, faixas e painéis de identificação da(s) obra(s) custeada(s) com os recursos transferidos à conta do Programa,
					obedecendo ao que está disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo e
					Gestão Estratégica da Presidência da República;
				</p>
				<p class="P_1">
					<br />
					XV - Manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução deste Termo de Compromisso,
					para fins de fiscalização, de acompanhamento e de avaliação dos resultados obtidos;
				</p>
				<p class="P_1">
					<br />
					XVI - Facilitar a supervisão e a fiscalização do FNDE/MEC, permitindo-lhe efetuar acompanhamento no local e fornecendo, sempre que solicitado, as informações e os
					documentos relacionados com a execução do objeto deste Instrumento, especialmente no que se refere ao exame da documentação relativa à licitação e aos contratos;
				</p>
				<p class="P_1">
					<br />
					XVII - Permitir o livre acesso de servidores do Sistema de Controle Interno do Poder Executivo Federal (Secretaria Federal de Controle
					– SFC/MF, Delegacia Federal de Controle – DFC ou sua representação no Estado, Secretaria de Controle Interno – CISET) e da Auditoria do
					FNDE, a qualquer tempo e lugar, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente com o
					objeto pactuado no Termo de Compromisso (Anexo I), bem como às obras e serviços a ele referidas, colaborando na obtenção de dados e de
					informações junto à comunidade local sobre os benefícios advindos da implantação do(s) projeto(s), quando em missão de fiscalização e auditoria;
				</p>
				<p class="P_1">
					<br />
					XVIII - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento comprobatório
					de despesa efetuada à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério daquela Autarquia Federal;
				</p>
				<p class="P_1">
					<br />
					XIX - Prestar todo e qualquer esclarecimento sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por órgão do Sistema
					de Controle Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão ou entidade com delegação para esse fim;
				</p>
				<p class="P_1">
					<br />
					XX - Incluir no orçamento anual do Município, ou do estado, os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos estabelecidos no
					§ 1º, do art. 6º, da Lei nº 4.320, de 17 de março de 1964;
				</p>
				<p class="P_1">
					<br />
					XXI - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e ao
					desenvolvimento do ensino, por força do disposto no art. 212 da Constituição Federal;
				</p>
				<p class="P_1">
					<br />
					XXII - Emitir o(s) termo(s) de aceitação definitiva da(s) obra(s), ao final da execução dos recursos, remetendo cópia autenticada do(s) mesmo(s)
					à DIRPE/FNDE para a emissão do(s) termo(s) de conclusão da(s) obra(s) e consolidação deste Termo de Compromisso;
				</p>
				<p class="P_1">
					<br />
					XXIII - Prestar contas ao FNDE/MEC dos recursos recebidos, no prazo e nas condições estipuladas nos artigos 29 e 30
					da Resolução CD/FNDE Nº 13/2011;
				</p>
				<p class="P_1">
					<br />
					XXIV - Manter em seu poder, à disposição do FNDE/MEC, da SEB/MEC, dos órgãos de controle interno e externo e do
					Ministério Público, os comprovantes das despesas efetuadas à conta do Programa, pelo prazo de 10 (dez) anos, contados
					da data da aprovação da prestação de contas anual do FNDE/MEC pelo Tribunal de Contas da União (TCU) a que se refere o exercício do repasse dos recursos,
					a qual será divulgada no sítio eletrônico <a href=\"www.fnde.gov.br\" style=\"color: blue\">www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					XXV - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais
					demandas judiciais relativas a recursos humanos utilizados na execução do objeto deste Termo de Compromisso, bem
					como por todos os ônus tributários ou extraordinários que incidam sobre o presente Instrumento, ressalvados aqueles de natureza compulsória,
					lançados automaticamente pela rede bancária arrecadadora;
				</p>
				<p class="P_1">
					<br />
					XXVI - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso.
				</p>
				<p class="quebra">&nbsp;</p>
				<p class="P_3">
					<br />
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que a Secretaria de Educação
					cumpre com as exigências do art. 169 da Constituição Federal que trata dos limites de despesa com pessoal e,
					que os recursos próprios de responsabilidade do Secretaria estão assegurados, conforme a Lei Orgânica Municipal.
				</p>
				<p class="Data">
					<br />
					Brasília/DF, ___ de ____________________ de ______.
				</p>
				<p class="T_1">
					<br />
					<br />
					<br />
					____________________________________________
					<br />
					<br />
					<strong>'.$munDados[0]['prefeito'].'</strong>
					<br />
					NOME DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO <strong>'.strtoupper($munDados[0]['municipio']).'/'.$munDados[0]['estuf'].'</strong>
				</p>
			</div>
		</body>
	</html>';
	}
}

function verificaRespostasQuestPAC($preid){
	global $db;

	if( $preid ){
		$qrpid = pegaQrpidAnalisePAC( $preid, 49 );

		$sql = "SELECT
					count(r.resid) as quantidade
				FROM
					questionario.grupopergunta g
				INNER JOIN questionario.pergunta p on p.grpid = g.grpid
				INNER JOIN questionario.itempergunta ip on ip.perid = p.perid
				INNER JOIN questionario.resposta r on r.itpid = ip.itpid
				WHERE
					ip.itpid IN (2698,2700,2702,2705,2707,2711,2713,2715,2717,2719,2721,2723,2725) AND
					r.qrpid = ".$qrpid;

		$quantidade = $db->pegaUm( $sql );

		if( $quantidade == 12 ){
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

}

function verificaPreenchimentoProgramaTransporte(){
	global $db;

	if( $_SESSION['par']['itrid'] == 1 ){ //estadual
		$w = " AND ppaid in (405, 413, 414, 415)";
		$crtid = "(450, 451, 452, 453)";
		$ppsid = 1075;
		if( $_SESSION['par']['estuf'] == 'DF' ){
			$sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.mun_estuf = map.estuf WHERE iu.mun_estuf = '".$_SESSION['par']['estuf']."'";
		} else {
			$sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.estuf = map.estuf WHERE iu.estuf = '".$_SESSION['par']['estuf']."' AND iu.itrid = 1";
		}
		$qntPermitida = $db->pegaUm( $sql );
	} elseif( $_SESSION['par']['itrid'] == 2 ){ //municipal
		$w = " AND ppaid in (379, 380, 381, 382)";
		$crtid = "(381, 382, 383, 384)";
		$ppsid = 1074;
		$sql = "SELECT map.mprqtd FROM par.municipioadesaoprograma map INNER JOIN par.instrumentounidade iu ON iu.muncod = map.muncod WHERE iu.muncod = '".$_SESSION['par']['muncod']."' AND iu.itrid = 2";
		$qntPermitida = $db->pegaUm( $sql );
	}


	$sql = "SELECT s.sbaid
			FROM par.subacao s
			INNER JOIN par.acao a ON a.aciid = s.aciid
			INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.inuid = {$_SESSION['par']['inuid']} AND s.sbastatus = 'A'
			WHERE s.ppsid = {$ppsid}";

	$sbaid = $db->pegaUm($sql);

	if(!empty($sbaid)){
		$sql_cotaaderido = "SELECT  	CASE WHEN f.sbaid IS NULL THEN 0 ELSE (COALESCE(SUM(icoquantidadetecnico),0)) END AS qtdAderido
							FROM 		par.subacaoitenscomposicao i
							LEFT JOIN 	( SELECT 		DISTINCT sbaid
										  FROM 			par.empenhosubacao es
	                               		  INNER JOIN 	par.empenho e ON e.empid = es.empid and eobstatus = 'A' and empstatus <> 'I'
	                               		  WHERE 		eobstatus = 'A' AND e.empsituacao = '2 - EFETIVADO' AND eobano = '".((int)date('Y')-1)."') AS f ON f.sbaid = i.sbaid
							WHERE 		i.sbaid = {$sbaid} AND i.icoano = ".((int)date('Y')-1)." AND i.icovalidatecnico = 'S'
							GROUP BY  	f.sbaid";
		$cota_aderido = $db->pegaUm($sql_cotaaderido);
	} else {
		$cota_aderido = 0;
	}

	$nova_cota = $qntPermitida - $cota_aderido;

	if(!empty($sbaid)){
		$sql_qtd = "SELECT 	SUM(icoquantidade) AS qtd_cadastrado
					FROM 	par.subacaoitenscomposicao
					WHERE 	sbaid = {$sbaid} AND icoano = '".date('Y')."'";
		$qtd = $db->pegaUm($sql_qtd);

		if($nova_cota <= 0){
			return "Não existe cota para aquisição de ônibus, para este município!";
		}

		if($qtd > $nova_cota){
			return "A quantidade de ônibus informada é maior que a cota para este município!";
		}

		$sql_preenchimento = "SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} AND icoano = '".date('Y')."'";
		$prenchimento =  $db->carregar($sql_preenchimento);

		if(empty($prenchimento)){
			return "Informe a quantidade de ônibus!";
		} else {
			return true;
		}
	} else {
		return "Informe a quantidade de ônibus!";
	}
}

function wf_pre_reformulaPreObra( $preid ){
        
	return true;
}

function getBrasao(){
	
	$tempocache = 86400; // 1 dia

	$key = md5('obras2_brasao_email');

	//        if($_SERVER['HTTP_HOST']=='simec-local'){
	//            return base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif'));
	//        }

	try {
		/* pegando informações do memcached server, key igual ao md5 do SQL */
		if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch($key);}

		if ($cache_result) { /* se existir cache, carregar com o resultado do memcached server */
			$res = $cache_result;
		} else { /* senão executa o SQL e guarda o resultado no memcached server */
			$res = base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif'));
			/* Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds */
			if (function_exists('zend_shm_cache_store')) {
				if(zend_shm_cache_store($key, $res, $tempocache) === false) echo '[ZEND CACHE FALHOU]';
			}
				
			
		}
	} catch (Exception $e){
		if($_SESSION['usucpf'] = ''){
			echo $e->getMessage(); exit;
		} else {
			return base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif'));
		}

	}
	return $res;
}

function wf_pos_diligenciaRefurmulaPreObra_miparaconvencional( $preid ){
	
	global $db;
	
	$sql = "SELECT
				pre.predescricao,
				pto.ptodescricao,
				ent.entemail,
				obr.obrid
			FROM
				obras.preobra pre
			INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid
			INNER JOIN obras2.obras						obr ON obr.preid = pre.preid
			INNER JOIN par.instrumentounidade 			inu ON (inu.muncod = pre.muncodpar AND pre.tooid = 1) OR (inu.estuf = pre.estufpar AND pre.tooid <> 1)
			INNER JOIN par.instrumentounidadeentidade	iue ON iue.inuid = inu.inuid
			LEFT  JOIN entidade.entidade				ent ON ent.entnumcpfcnpj = iue.iuecnpj AND ent.entemail IS NOT NULL
			WHERE
				pre.preid = $preid";
	
	$arrDados = $db->pegaLinha( $sql );
	
	$conteudo = '
			<html>
				<head>
					<title></title>
				</head>
				<body>
					<table style="width: 100%;">
						<thead>
							<tr>
								<td style="text-align: center;">
									<p><img  src="http://simec.mec.gov.br/imagens/brasao.gif" width="70"/><br/>
									<b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
									FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO<br/>
									DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS<br/>
									COORDENAÇÃO GERAL DE INFRAESTRUTURA EDUCACIONAL<br/> 
									SBS Quadra 02 - Bloco F - 14º andar - Edifício FNDE - CEP -70070-929<br/>
								</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td style="line-height: 15px;">
								</td>
							</tr>
							<tr>
								<td style="line-height: 15px; text-align:justify">
									<p>Nome da Obra: '.$arrDados['predescricao'].'
									Tipo da Obra: Escola '.$arrDados['ptodescricao'].'
									Nº identificação: '.$arrDados['preid'].'</p>
									<br>
									<p>Após a análise do FNDE nº '.$arrDados['preid'].', verificamos que a proposta(s) está na situação "em diligência de reformulação".</p>
									<br>		
									<p>Solicitamos que a equipe municipal acesse o SIMEC-módulo PAR, clique na obra que está em diligência de reformulação e, depois, 
									na aba "Análise de Engenharia" (no ícone "abrir todos").</p>
									<br>
									<p>Nos itens da análise de engenharia em que a resposta é "NAO", deve-se ler atentamente a "Observação", ajustar o que é solicitado 
									em cada aba correspondente, conforme os itens negativos da análise de engenharia. (ex: Planta de Locação: o novo projeto selecionado não 
									foi implantando no terreno disponibilizado)  enviar novamente a ação para análise da Equipe do FNDE até às 23 horas e 59 minutos do dia ... de .........de 2015.</b>
								</td>
							</tr>
							<tr>
								<td style="padding: 10px 0 0 0;">
									Atenciosamente,
								</td>
							</tr>
							<tr>
								<td style="text-align: center; padding: 10px 0 0 0;">
									<img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="http://simec.mec.gov.br/imagens/obras/assinatura-fabio.png" />
									<br />
									FÁBIO LÚCIO DE A. CARDOSO<br>
									Coordenador-Geral de Infraestrutura Educacional - CGEST<br>
									Diretoria de Gestão, Articulação e Projetos Educacionais - DIGAP<br>
									Fundo Nacional de Desenvolvimento da Educação-FNDE<br>
								</td>
							</tr>
						</tbody>
					</table>
				</body>
			</html>';
	
	$assunto  = "Reformulação da obra ({$arrDados['obrid']}) Construção de Creche Metodologias Inovadoras";
	
	$email = $arrDados['entemail'];
	
	enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $conteudo, $cc, $cco );
	
	return true;
}


function testaObraMIDestrato( $preid )
{
	global $db;
		
	$sql = "
			SELECT 
						distinct pre.preid
					FROM 
						obras.preobra pre
					INNER JOIN workflow.documento doc ON pre.docid = doc.docid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esdstatus = 'A'
					INNER JOIN obras2.obras o 
						INNER JOIN obras2.obras_arquivos oa ON oa.obrid = o.obrid AND tpaid = 30 AND oarstatus = 'A'
					on o.preid = pre.preid AND o.obrstatus = 'A'
					
					WHERE 
						pre.ptoid in (43, 42, 44, 45) -- MI
					AND 
						esd.esdid in ( 360, 624, 337, 228,  1561, 1563 ) -- EM REFORMULAÇÃO -- OBRA APROVADA
					AND
						pre.preid  not IN  
						( 
							SELECT distinct o3.preid from obras2.obras o3 
							inner join workflow.documento doc3 ON o3.docid = doc3.docid 
							inner join workflow.estadodocumento esd3 ON esd3.esdid = doc3.esdid AND esd3.esdstatus = 'A'	
					
							WHERE 
							esd3.esdid in ( 
								691
								,690 
								,769
							)  and preid is not null
							 AND o3.obrstatus = 'A'
						)
					AND
						pre.prestatus = 'A'
				
				and pre.preid = {$preid} 
			
			
			";
	
	if($db->pegaUm($sql))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}


function wf_pos_refurmulaPreObra_miparaconvencional( $preid ){
	
	global $db;
	
	$sql = "SELECT
				pre.predescricao,
				pto.ptodescricao,
				ent.entemail,
				obr.obrid
			FROM
				obras.preobra pre
			INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid
			INNER JOIN obras2.obras						obr ON obr.preid = pre.preid
			INNER JOIN par.instrumentounidade 			inu ON (inu.muncod = pre.muncodpar AND pre.tooid = 1) OR (inu.estuf = pre.estufpar AND pre.tooid <> 1)
			INNER JOIN par.instrumentounidadeentidade	iue ON iue.inuid = inu.inuid
			LEFT  JOIN entidade.entidade				ent ON ent.entnumcpfcnpj = iue.iuecnpj AND ent.entemail IS NOT NULL
			WHERE
				pre.preid = $preid";
	
	$arrDados = $db->pegaLinha( $sql );

    include_once APPRAIZ . "par/classes/modelo/PreObra.class.inc";
	$objPreObra = new PreObra( $preid );
	$novoPreid = $objPreObra->criarBkp();
	
	$ptoid = $db->pegaUm("select ptoidsolicitado from par.solicitacaoreformulacaoobras where preid = $preid");
		
	$sql = "UPDATE obras.preobra SET ptoid = ".($ptoid ? $ptoid : 'null')." WHERE preid = $preid;";
	$db->executar( $sql );
	$db->commit();
	
	$sql = "SELECT
				obrid,
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";
	
	$arObra = $db->pegaLinha( $sql );
	
	if( $arObra['obrid'] && $arObra['esdid'] != 768 ){
	
		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = 768
					AND esdidorigem = {$arObra['esdid']}";
		
		$aedid = $db->pegaUm($sql);
		
		if( $aedid == '' ){
		$sql = "INSERT INTO workflow.acaoestadodoc
					(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
					esdsncomentario, aedvisivel, aedcodicaonegativa)
				VALUES
					({$arObra['esdid']}, 768, 'Enviar para reformulação', 'A', 'Enviada para reformulação',
					true, false, false )
				RETURNING
					aedid";
		
			$aedid = $db->pegaUm($sql);
		}
		
		include_once APPRAIZ . 'includes/workflow.php';
		
		$teste = wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por wf_pos_refurmulaPreObra_miparaconvencional preid = '.$preid, array( 'docid' => $arObra['docid'] ) );
		$db->commit();
	}
	
	$texto = '
			<html>
				<head>
					<title></title>
				</head>
				<body>
					<table style="width: 100%;">
						<thead>
							<tr>
								<td style="text-align: center;">
									<p><img  src="http://simec.mec.gov.br/imagens/brasao.gif" width="70"/><br/>
									<b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
									FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO<br/>
									DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS<br/>
									COORDENAÇÃO GERAL DE INFRAESTRUTURA EDUCACIONAL<br/> 
									SBS Quadra 02 - Bloco F - 14º andar - Edifício FNDE - CEP -70070-929<br/>
								</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td style="line-height: 15px;">
								</td>
							</tr>
							<tr>
								<td style="line-height: 15px; text-align:justify">
									<p>Sr(a). Gestor,<br> a obra ('.$arrDados['obrid'].') Construção de Creche '.$arrDados['predescricao'].' está aberta para reformulação, possibilitando a alteração do projeto para construção em metodologia convencional e posterior realização do processo licitatório para execução da mesma.</p>
									                                                        
									<p>Para obter informações de como preencher o sistema, acesse o manual disponibilizado na página inicial do SIMEC-módulo PAR.</b>
								</td>
							</tr>
							<tr>
								<td style="padding: 10px 0 0 0;">
									Atenciosamente,
								</td>
							</tr>
							<tr>
								<td style="text-align: center; padding: 10px 0 0 0;">
									<img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="http://simec.mec.gov.br/imagens/obras/assinatura-fabio.png" />
									<br />
									FÁBIO LÚCIO DE A. CARDOSO<br>
									Coordenador-Geral de Infraestrutura Educacional - CGEST<br>
									Diretoria de Gestão, Articulação e Projetos Educacionais - DIGAP<br>
									Fundo Nacional de Desenvolvimento da Educação-FNDE<br>
								</td>
							</tr>
						</tbody>
					</table>
				</body>
			</html>';
	
	
	$assunto  = "Reformulação da obra ({$arrDados['obrid']}) Construção de Creche Metodologias Inovadoras";
	
	$email = Array($arrDados['entemail']);
	
	if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
		$email = array($_SESSION['email_sistema']);
	}
	enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $texto, $cc, $cco );
	
	return true;
}

function wf_pos_refurmulaPreObra( $preid ){

	global $db;

	$sql = "UPDATE obras.preobra SET predatareformulacao=NOW(), preusucpfreformulacao='".$_SESSION['usucpf']."' WHERE preid='".$preid."'";
	$db->executar($sql);
	
	$objPreObra = new PreObra( $preid );
	$novoPreid = $objPreObra->criarBkp();

	$sql = "SELECT
				obrid,
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";

	$arObra = $db->pegaLinha( $sql );


	if( $arObra['obrid'] ){

		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = ".OBR_ESDID_EM_REFORMULACAO."
					AND esdidorigem = {$arObra['esdid']}";

		$aedid = $db->pegaUm($sql);

		if( $aedid == '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa)
					VALUES(
						{$arObra['esdid']}, ".OBR_ESDID_EM_REFORMULACAO.", 'Enviar para reformulação', 'A', 'Enviada para reformulação',
						true, false, false )
					RETURNING
						aedid";

			$aedid = $db->pegaUm($sql);
		}
		
		$teste = wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por reformularObra preid = '.$preid, array( 'docid' => $arObra['docid'] ) );

		if( !$teste ){
			return false;
		}
	}
	
	/* Regra somente para PAC*/
	$sql = "SELECT tooid FROM obras.preobra WHERE preid = $preid";
	
	$tooid = $db->pegaUm( $sql );
	
	if( $tooid == '1' ){
	
		$sql = "SELECT max(terid) FROM par.processoobraspaccomposicao ppc
				INNER JOIN par.termocompromissopac ter ON ter.proid = ppc.proid AND terstatus = 'A'
				WHERE preid = $preid";
	
		$terid = $db->pegaUm( $sql );
	
		$sql = "UPDATE obras.preobra SET prereformulacao = TRUE WHERE preid = $preid;
				UPDATE par.termocompromissopac SET terreformulacao = TRUE WHERE terid = $terid";
			
		$db->executar($sql);
		$db->commit();
	}

	return true;
}

function cancelarReformulacaoPreObra( $preid, $booMI = false )
{
	global $db;

	$sql = "SELECT
				COALESCE(obrpercentultvistoria, 0) as percexec,
				obrid,
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";

	$arObra = $db->pegaLinha($sql);

	$sql = "SELECT
				*
			FROM
				obras.preobra
			WHERE
				preidpai = ".$preid."
				AND preusucpfreformulacao IS NOT NULL
				AND predatareformulacao IS NOT NULL
			ORDER BY
				preid DESC
			LIMIT 1";

	$arReformulacao = $db->pegaLinha($sql);

	if($arReformulacao['preid']){

		$sql = "SELECT
					COALESCE(obrpercentultvistoria, 0) as percexec,
					obrid,
					doc.esdid,
					doc.docid
				FROM
					obras2.obras obr
				INNER JOIN workflow.documento doc ON doc.docid = obr.docid
				WHERE
					preid = {$arReformulacao['preid']}
					AND obrstatus = 'A'";

		$arObra2 = $db->pegaLinha($sql);

		$sql = "UPDATE
					obras.preobra
				SET
					predtcancelreformulacao=NOW(),
					precpfcancelreformulacao='".$_SESSION['usucpf']."'
				WHERE
					preid='".$preid."'";

		$db->executar($sql);

		// criar novo preid (replicar obras.preobra)
		$sql = "INSERT INTO
					obras.preobra
				(
	            	docid,
	            	presistema,
	            	preidsistema,
	            	ptoid,
	            	preobservacao,
		            prelogradouro,
		            precomplemento,
		            estuf,
		            muncod,
		            precep,
		            prelatitude,
		            prelongitude,
		            predtinclusao,
		            prebairro,
		            preano,
		            qrpid,
		            predescricao,
		            prenumero,
		            pretipofundacao,
		            prestatus,
		            entcodent,
		            preprioridade,
		            terid,
		            resid,
		            prevalorobra,
		            tooid,
		            muncodpar,
		            estufpar,
		            premcmv,
		            preidpai,
		            predtcancelreformulacao,
		            precpfcancelreformulacao
	            )
	            (
	            	SELECT
	            		NULL,
	            		presistema,
	            		preidsistema,
	            		ptoid,
	            		preobservacao,
			            prelogradouro,
			            precomplemento,
			            estuf,
			            muncod,
			            precep,
			            prelatitude,
			            prelongitude,
			            predtinclusao,
			            prebairro,
			            preano,
			            qrpid,
			            predescricao,
			            prenumero,
			            pretipofundacao,
			            prestatus,
			            entcodent,
			            preprioridade,
			            terid,
			            resid,
			            prevalorobra,
			            tooid,
			            muncodpar,
			            estufpar,
			            premcmv,
			            ".$preid.",
			            predtcancelreformulacao,
			            precpfcancelreformulacao
	            	FROM
	            		obras.preobra
	            	WHERE
	            		preid=".$preid."
	            	AND
	            		preusucpfreformulacao IS NOT NULL
	            	AND
	            		predatareformulacao IS NOT NULL
	            	ORDER BY
	            		preid DESC
	            	LIMIT 1
	            ) RETURNING preid";

		$novopreid = $db->pegaUm($sql);
		
		if( !$novopreid ){
			return false;
		}

		// buscar o qrpid da preobraanalise antiga
		$sql = "SELECT
					qrpid
				FROM
					obras.preobraanalise
				WHERE
					preid='".$preid."'";

		$antigoqrpid = $db->pegaUm($sql);

		// buscar o qrpid da preobra2 antiga
		$sql = "SELECT
					qrpid
				FROM
					obras.preobra
				WHERE
					preid='".$preid."'";

		$antigoqrpid2 = $db->pegaUm($sql);

		// ##### CANCELA #####
		$sql = "SELECT
					qrpid
				FROM
					obras.preobraanalise
				WHERE
					preid='".$arReformulacao['preid']."'";

		$restauraantigoqrpid = $db->pegaUm($sql);

		$sql = "SELECT
					qrpid
				FROM
					obras.preobra
				WHERE
					preid='".$arReformulacao['preid']."'";

		$restauraantigoqrpid2 = $db->pegaUm($sql);
		// ##### FIM #####

		// criar novo qrpid (replicar questionario.questionarioresposta)
		$sql = "INSERT INTO questionario.questionarioresposta
				( queid, qrptitulo, qrpdata )
	            (
	            	SELECT
	            		queid,
	            		'OBRAS (".$novopreid.")',
	            		qrpdata
	            	FROM
	            		questionario.questionarioresposta
	            	WHERE
	            		qrpid='".$antigoqrpid."'

	            ) RETURNING qrpid";

		$novoqrpid = $db->pegaUm($sql);

		// ######### VOLTA QEUSTIONARIO ORIGINAL ANALISE #########
		$sql = "INSERT INTO questionario.questionarioresposta
				( queid, qrptitulo, qrpdata )
	            (
	            	SELECT
	            		queid,
	            		'OBRAS (".$preid.")',
	            		qrpdata
	            	FROM
	            		questionario.questionarioresposta
	            	WHERE
	            		qrpid=( SELECT
									qrpid
								FROM
									obras.preobraanalise
								WHERE
									preid='".$arReformulacao['preid']."' )
	            ) RETURNING qrpid";

		$restauraqrpid = $db->pegaUm($sql);
		// ######### FIM QEUSTIONARIO ORIGINAL ANALISE #########

		// pegando descricao o municipio
		$sql = "SELECT
					m.mundescricao
				FROM
					obras.preobra p
			    LEFT JOIN
			    	territorios.municipio m ON m.muncod = p.muncod
			     WHERE
			     	preid='".$preid."'";

		$mundescricao = $db->pegaUm($sql);
		$mundescricao = str_ireplace("'", '', $mundescricao);

		// criar novo qrpid (replicar questionario.questionarioresposta)
		$sql = "INSERT INTO questionario.questionarioresposta
				( queid, qrptitulo, qrpdata )
	            (
	            	SELECT
	            		queid,
	            		'OBRAS (".$novopreid." - ".$mundescricao.")',
	            		qrpdata
	            	FROM
	            		questionario.questionarioresposta
	            	WHERE
	            		qrpid='".$antigoqrpid2."'

	            ) RETURNING qrpid";

		$novoqrpid2 = $db->pegaUm($sql);

		$sql = "UPDATE obras.preobra SET qrpid='".$novoqrpid2."' WHERE preid='".$novopreid."'";

		$db->executar($sql);

		// ######### VOLTA QUESTIONARIO ORIGINAL PREOBRA #########
		$sql = "SELECT
					m.mundescricao
				FROM
					obras.preobra p
			    LEFT JOIN
			    	territorios.municipio m ON m.muncod = p.muncod
			     WHERE
			     	preid='".$arReformulacao['preid']."'";

		$mundescricao = $db->pegaUm($sql);
		$mundescricao = str_ireplace("'", '', $mundescricao);

		$sql = "INSERT INTO questionario.questionarioresposta
				( queid, qrptitulo, qrpdata )
	            (
	            	SELECT
	            		queid,
	            		'OBRAS (".$preid." - ".$mundescricao.")',
	            		qrpdata
	            	FROM
	            		questionario.questionarioresposta
	            	WHERE
	            		qrpid='".$restauraantigoqrpid2."'

	            ) RETURNING qrpid";

		$restauraqrpid2 = $db->pegaUm($sql);

		$sql = "UPDATE obras.preobra SET qrpid='".$restauraqrpid2."' WHERE preid='".$preid."'";

		$db->executar($sql);
		// ######### FIM QUESTIONARIO ORIGINAL PREOBRA #########

		// criar novo panid (replicar)
		$sql = "INSERT INTO obras.preobraanalise(
	            preid, poadataanalise, poastatus, poausucpfinclusao, qrpid, poaindeferido,
	            poajustificativa)
	            (SELECT '".$novopreid."', poadataanalise, poastatus, poausucpfinclusao, ".(($novoqrpid)?"'".$novoqrpid."'":"NULL").", poaindeferido,
	            poajustificativa FROM obras.preobraanalise WHERE preid='".$preid."')";
		$db->executar($sql);

		// ##### #####
		$sql = "DELETE FROM obras.preobraanalise WHERE preid = {$preid};
				INSERT INTO obras.preobraanalise(
	            preid, poadataanalise, poastatus, poausucpfinclusao, qrpid, poaindeferido,
	            poajustificativa)
	            (SELECT '".$preid."', poadataanalise, poastatus, poausucpfinclusao, ".(($restauraqrpid)?"'".$restauraqrpid."'":"NULL").", poaindeferido,
	            poajustificativa FROM obras.preobraanalise WHERE preid='".$arReformulacao['preid']."')";
		$db->executar($sql);
		// ##### #####

		$sql = "INSERT INTO questionario.resposta(
	            perid, qrpid, usucpf, itpid, resdsc)
	            (SELECT perid, '".$novoqrpid."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$antigoqrpid."')";
		$db->executar($sql);

		// ##### CANCELA #####
		$sql = "INSERT INTO questionario.resposta(
	            perid, qrpid, usucpf, itpid, resdsc)
	            (SELECT perid, '".$restauraqrpid."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$restauraantigoqrpid."')";
		$db->executar($sql);
		// ##### FIM #####

		$sql = "INSERT INTO questionario.resposta(
	            perid, qrpid, usucpf, itpid, resdsc)
	            (SELECT perid, '".$novoqrpid2."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$antigoqrpid2."')";
		$db->executar($sql);

		// ##### CANCELA #####
		$sql = "INSERT INTO questionario.resposta(
	            perid, qrpid, usucpf, itpid, resdsc)
	            (SELECT perid, '".$restauraqrpid2."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$restauraantigoqrpid2."')";
		$db->executar($sql);
		// ##### FIM #####

		$sql = "INSERT INTO obras.preobrafotos(
	            pofdescricao, preid, arqid)
	            (SELECT pofdescricao, '".$novopreid."', arqid FROM obras.preobrafotos WHERE preid='".$preid."')";
		$db->executar($sql);

		// ##### CANCELA #####
		$sql = "DELETE FROM obras.preobrafotos WHERE preid = {$preid};
				INSERT INTO obras.preobrafotos(
	            pofdescricao, preid, arqid)
	            (SELECT pofdescricao, '".$preid."', arqid FROM obras.preobrafotos WHERE preid='".$arReformulacao['preid']."')";
		$db->executar($sql);
		// ##### FIM #####

		$sql = "INSERT INTO obras.preobraanexo(
	            preid, poadescricao, arqid, podid, datainclusao, usucpf,
	            poasituacao)
	            (SELECT '".$novopreid."', poadescricao, arqid, podid, datainclusao, usucpf,
	            poasituacao FROM obras.preobraanexo WHERE preid='".$preid."')";
		$db->executar($sql);

		// ##### CANCELA #####
		$sql = "DELETE FROM obras.preobraanexo WHERE preid = {$preid};
				INSERT INTO obras.preobraanexo(
	            preid, poadescricao, arqid, podid, datainclusao, usucpf,
	            poasituacao)
	            (SELECT '".$preid."', poadescricao, arqid, podid, datainclusao, usucpf,
	            poasituacao FROM obras.preobraanexo WHERE preid='".$arReformulacao['preid']."')";
		$db->executar($sql);
		// ##### FIM #####

		$sql = "INSERT INTO obras.preplanilhaorcamentaria(
	            preid, itcid, ppovalorunitario)
	            (SELECT '".$novopreid."', itcid, ppovalorunitario FROM obras.preplanilhaorcamentaria WHERE preid='".$preid."')";
		$db->executar($sql);

		// ##### CANCELAR #####
		$sql = "DELETE FROM obras.preplanilhaorcamentaria WHERE preid = {$preid};
				INSERT INTO obras.preplanilhaorcamentaria(
	            preid, itcid, ppovalorunitario)
	            (SELECT '".$preid."', itcid, ppovalorunitario FROM obras.preplanilhaorcamentaria WHERE preid='".$arReformulacao['preid']."')";
		$db->executar($sql);
		// ##### FIM #####

		if( $arObra['obrid'] ){

			$sql = "SELECT
						aed.esdidorigem as id,
						esd.esddsc as descricao
					FROM
						workflow.historicodocumento hst
					INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem
					WHERE
						hstid = (SELECT max(hstid) FROM workflow.historicodocumento WHERE docid = {$arObra['docid']})";

			$esdiddestino = $db->pegaLinha($sql);

			$sql = "SELECT
						aedid
					FROM workflow.acaoestadodoc
					WHERE
						esdiddestino = {$esdiddestino['id']}
						AND esdidorigem = {$arObra['esdid']}";

			$aedid = $db->pegaUm($sql);

			if( $aedid == '' ){
				$sql = "INSERT INTO workflow.acaoestadodoc
							(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
							esdsncomentario, aedvisivel, aedcodicaonegativa)
						VALUES(
							{$arObra['esdid']}, {$esdiddestino['id']}, 'Enviar para {$esdiddestino['descricao']}', 'A',
							'Enviada para {$esdiddestino['descricao']}',
							true, false, false )
						RETURNING
							aedid";

				$aedid = $db->pegaUm($sql);
			}

			$teste = wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por cancelarReformulacaoObra preid = '.$preid,
							  array( 'docid' => $arObra['docid'] ) );
		}

		// ##### CANCELAR #####
		if( $arObra2['obrid'] ){

			$sql = "SELECT
						aed.esdidorigem as id,
						esd.esddsc as descricao
					FROM
						workflow.historicodocumento hst
					INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem
					WHERE
						hstid = (SELECT max(hstid) FROM workflow.historicodocumento WHERE docid = {$arObra2['docid']})";

			$esdiddestino = $db->pegaLinha($sql);

			$sql = "SELECT
						aedid
					FROM workflow.acaoestadodoc
					WHERE
						esdiddestino = {$esdiddestino['id']}
						AND esdidorigem = {$arObra2['esdid']}";

			$aedid = $db->pegaUm($sql);

			if( $aedid == '' ){
				$sql = "INSERT INTO workflow.acaoestadodoc
							(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
							esdsncomentario, aedvisivel, aedcodicaonegativa)
						VALUES(
							{$arObra2['esdid']}, {$esdiddestino['id']}, 'Enviar para {$esdiddestino['descricao']}', 'A',
							'Enviada para {$esdiddestino['descricao']}',
							true, false, false )
						RETURNING
							aedid";

				$aedid = $db->pegaUm($sql);
			}

			$teste = wf_alterarEstado( $arObra2['docid'], $aedid, 'Tramitado por cancelarReformulacaoObra preid = '.$preid,
							  array( 'docid' => $arObra2['docid'] ) );
		}
		// ##### FIM #####

		//volta com os dados da obra original
		$sql = "update
					obras.preobra
				set
					presistema 		= ".($arReformulacao['presistema'] ? $arReformulacao['presistema'] : 'null').",
					preidsistema 	= ".($arReformulacao['preidsistema'] ? $arReformulacao['preidsistema'] : 'null').",
					ptoid			= ".($arReformulacao['ptoid'] ? $arReformulacao['ptoid'] : 'null').",
					preobservacao	= '{$arReformulacao['preobservacao']}',
		            prelogradouro	= '{$arReformulacao['prelogradouro']}',
		            precomplemento	= '{$arReformulacao['precomplemento']}',
		            estuf			= '{$arReformulacao['estuf']}',
		            muncod			= ".($arReformulacao['muncod'] ? "'".$arReformulacao['muncod']."'" : 'null').",
		            precep			= '{$arReformulacao['precep']}',
		            prelatitude		= '{$arReformulacao['prelatitude']}',
		            prelongitude	= '{$arReformulacao['prelongitude']}',
		            predtinclusao	= '{$arReformulacao['predtinclusao']}',
		            prebairro		= '{$arReformulacao['prebairro']}',
		            preano			= ".($arReformulacao['preano'] ? $arReformulacao['preano'] : 'null').",
		            predescricao	= '{$arReformulacao['predescricao']}',
		            prenumero		= '{$arReformulacao['prenumero']}',
		            pretipofundacao = '{$arReformulacao['pretipofundacao']}',
		            prestatus		= '{$arReformulacao['prestatus']}',
		            entcodent		= '{$arReformulacao['entcodent']}',
		            preprioridade	= ".($arReformulacao['preprioridade'] ? $arReformulacao['preprioridade'] : 'null').",
		            terid			= ".($arReformulacao['terid'] ? $arReformulacao['terid'] : 'null').",
		            resid			= ".($arReformulacao['resid'] ? $arReformulacao['resid'] : 'null').",
		            prevalorobra	= ".($arReformulacao['prevalorobra'] ? $arReformulacao['prevalorobra'] : 'null').",
		            tooid			= ".($arReformulacao['tooid'] ? $arReformulacao['tooid'] : 'null').",
		            muncodpar		= ".($arReformulacao['muncodpar'] ? "'".$arReformulacao['muncodpar']."'" : 'null').",
		            estufpar		= ".($arReformulacao['estufpar'] ? "'".$arReformulacao['estufpar']."'" : 'null').",
		            premcmv			= '{$arReformulacao['premcmv']}'
		        where
		        	preid = {$preid}; ";

		$db->executar($sql);

		if($db->commit()){
			return true;
		}
	}
	return false;
}

function verificaQuestionarioAnaliseEngenhariaTerreno($preid)
{
	global $db;

	$sql = "SELECT
				po.preid,
				ip.itptitulo
			FROM
				obras.preobraanalise po
			INNER JOIN questionario.questionarioresposta q ON q.qrpid = po.qrpid
			INNER JOIN questionario.grupopergunta gp ON gp.queid = q.queid
			INNER JOIN questionario.pergunta p on p.grpid = gp.grpid AND p.perid = 1301
			INNER JOIN questionario.itempergunta ip on ip.perid = p.perid AND ip.itpid = 2700
			INNER JOIN questionario.resposta r on r.itpid = ip.itpid AND r.qrpid = q.qrpid
			WHERE
				po.preid = {$preid} AND
				q.queid = 49";

	return $db->pegaLinha($sql);
}

function subacaoPodeTramitarParaEmDiligencia( $sbaid ){
	global $db;

	$sql = "SELECT DISTINCT ssuid FROM par.subacaodetalhe WHERE ssuid IS NOT NULL AND sbaid = ".$sbaid;
	$ssuid = $db->carregarColuna( $sql );
	$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

	if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil)  ){
		if(in_array( 7, $ssuid ) || in_array( 10, $ssuid )){
			$sql = "SELECT DISTINCT ssuid, sbdano, sbdparecer, sbdparecerdemerito FROM par.subacaodetalhe WHERE sbaid = $sbaid AND ssuid in (7, 10)";
			$pareceres = $db->carregar($sql);
			foreach( $pareceres as $parecer ){
				if( $parecer['sbdparecer'] == '' && $parecer['sbdparecerdemerito'] == ''){
					return false;
				}
				$sql = "SELECT icoano, icovalidatecnico FROM par.subacaoitenscomposicao WHERE sbaid = $sbaid AND icostatus = 'A' AND icoano = '{$parecer['sbdano']}'";
				$itens = $db->carregar($sql);
				if( is_array($itens) ){
					foreach( $itens as $iten ){
						if( $iten['icovalidatecnico'] == '' ){
							return false;
						}
					}
				}
			}
			return true;
		}
	}

	if( in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrayPerfil) && in_array( 10, $ssuid ) ){ //analista de meritos no status de EM DILIGÊNCIA - ANÁLISE DE MÉRITO
		$sql = "SELECT DISTINCT ssuid, sbdano, sbdparecer, sbdparecerdemerito FROM par.subacaodetalhe WHERE sbaid = $sbaid AND ssuid = 10";
		$pareceres = $db->carregar($sql);
		foreach( $pareceres as $parecer ){
			if( $parecer['sbdparecer'] == '' && $parecer['sbdparecerdemerito'] == '' ){
				return false;
			}
		}
		return true;
	} else if( in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil) ){
		if( in_array( 7, $ssuid ) ){
			$sql = "SELECT DISTINCT ssuid, sbdano, sbdparecer, sbdparecerdemerito FROM par.subacaodetalhe WHERE sbaid = $sbaid AND ssuid = 7";
			$pareceres = $db->carregar($sql);
			foreach( $pareceres as $parecer ){
				if( $parecer['sbdparecer'] == '' && $parecer['sbdparecerdemerito'] == '' ){
					return false;
				}
				$sql = "SELECT icoano, icovalidatecnico FROM par.subacaoitenscomposicao WHERE sbaid = $sbaid AND icostatus = 'A' AND icoano = '{$parecer['sbdano']}'";
				$itens = $db->carregar($sql);
				if( is_array($itens) ){
					foreach( $itens as $iten ){
						if( $iten['icovalidatecnico'] == '' ){
							return false;
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}

function subacaoPodeTramitarParaEmDiligenciaCondicioal( $sbaid ){

	global $db;
	$sql = "SELECT DISTINCT ssuid FROM par.subacaodetalhe WHERE ssuid IS NOT NULL AND sbaid = ".$sbaid;
	$ssuid = $db->carregarColuna( $sql );
	$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

	if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ||
		in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) ||
		in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) ||
		in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil)
	){
		if( in_array( 22, $ssuid ) ){
			$sql = "SELECT DISTINCT ssuid, sbdano, sbdparecer, sbdparecerdemerito FROM par.subacaodetalhe WHERE sbaid = $sbaid AND ssuid = 22";
			$pareceres = $db->carregar($sql);
			foreach( $pareceres as $parecer ){
				if( $parecer['sbdparecer'] == '' && $parecer['sbdparecerdemerito'] == '' ){
					return false;
				}
				$sql = "SELECT icoano, icovalidatecnico FROM par.subacaoitenscomposicao WHERE sbaid = $sbaid AND icostatus = 'A' AND icoano = '{$parecer['sbdano']}'";
				$itens = $db->carregar($sql);
				if( is_array($itens) ){
					foreach( $itens as $iten ){
						if( $iten['icovalidatecnico'] == '' ){
							return false;
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}

function testaDiligencia_RetornarAnalise( $sbaid ){

	global $db;

	$sql = "SELECT docid FROM par.subacao WHERE sbaid = $sbaid";

	$docid = $db->pegaUm($sql);

	$sql = "SELECT
				true
			FROM
				workflow.historicodocumento hst
			INNER JOIN workflow.documento doc ON doc.docid = hst.docid AND doc.tpdid = ".WF_TPDID_SUBACOES_PAR."
			INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem AND esd.esdid = ".WF_SUBACAO_DILIGENCIA_CONDICIONAL."
			WHERE doc.docid = $docid";

	$testaDiligenciaCondicional = $db->pegaUm($sql);

	if( $testaDiligenciaCondicional == 't' ){

		$sql = "SELECT DISTINCT sbdano FROM par.subacaodetalhe WHERE sbaid = $sbaid AND ssuid = 22";
		$anos = $db->carregarColuna($sql);

		if( is_array($anos) ){
			foreach($anos as $ano){
				$sql = "SELECT
							par.recuperavalorplanejadossubacaoporano($sbaid, '$ano')
							- (coalesce(sep.sepvalor,0)+(coalesce(sbdrepassevlrcomplementar,0))+(coalesce(sbdrepassevlrempenho,0))+(coalesce(sbdrepassevlrraf,0)))  as dif
						FROM
							par.subacaoemendapta sep
						INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";

				$testaValores = $db->pegaUm($sql);
				if( $testaValores > 0 ){
					return "Valor planejado ultrapassa o valor empenhado para esta subação em ".number_format($testaValores,2,',','.').".\\n Favor distribui-lo no campo de complementação.";
				}else{
					return true;
				}
			}
		}else{
			return true;
		}
	}else{
		return true;
	}
}


function enviaEmailFimReformulacao( $sbaid ){

	global $db;

	$sql = "SELECT DISTINCT
				par.retornacodigopropostasubacao(ppsid) as codigo,
				CASE
					WHEN iu.itrid = 1 THEN iu.estuf
					ELSE m.mundescricao || '/' || m.estuf
				END as local,
				itrid
			FROM
			       par.subacao s
			INNER JOIN par.acao 			a ON a.aciid = s.aciid AND a.acistatus = 'A'
			INNER JOIN par.pontuacao 		p ON p.ptoid = a.ptoid
			INNER JOIN par.instrumentounidade 	iu ON iu.inuid = p.inuid
			LEFT  JOIN territorios.municipio 	m ON m.muncod = iu.muncod
			WHERE
				sbaid = $sbaid";

	$dados = $db->pegaLinha( $sql );

	extract($dados);

	$enti = $itrid == 1 ? 'estado' : 'município';

	$assunto  = "MEC/FNDE - Finalização de Reformulação no módulo PAR ";

	$conteudo = "<p>A reformulação da subação $codigo do $enti $local foi finalizada.</p>";

	$email = 'par@fnde.gov.br';

	enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $conteudo, $cc, $cco );

	return true;
}

function subacaoPodeTramitarParaIndeferida( $sbaid ){
	global $db;

	$sql = "SELECT DISTINCT ssuid FROM par.subacaodetalhe WHERE ssuid IS NOT NULL AND sbaid = ".$sbaid;
	$ssuid = $db->carregarColuna( $sql );

	$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
	if( in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrayPerfil) && in_array( 11, $ssuid ) ){ //analista de meritos no status de INDEFERIDA - ANÁLISE DE MÉRITO
		return true;
	}

	if( !in_array(PAR_PERFIL_ANALISTA_MERITOS,$arrayPerfil) &&
		( in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil) ) &&
		in_array( 4, $ssuid ) ){ //qualquer um sem ser analista de meritos
		return true;
	}

	return false;
}


function subacaoPodeTramitarParaAguardandoParecerFinanceiro( $sbaid ){
	global $db;

	if( $db->testa_superuser() ){
		return true;
	}

	$sql = "SELECT DISTINCT ssuid FROM par.subacaodetalhe WHERE ssuid IS NOT NULL AND sbaid = ".$sbaid;
	$ssuid = $db->carregarColuna( $sql );

	if( in_array( 2, $ssuid ) ){
		return true;
	}

	if( in_array( 10, $ssuid ) ){ // Status de EM DILIGÊNCIA - ANÁLISE DE MÉRITO
		return false;
	}

	return false;
}

function enviaEmailDiligenciaSubacao( $inuid, $sbaid ){
	global $db;

	$sql = "SELECT
				itrid, estuf, muncod
			FROM
				par.instrumentounidade where inuid = ".$inuid;

	$dadosEntidade = $db->pegaLinha( $sql );

	if( $dadosEntidade['itrid'] == 1 ){ //Estado

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				INNER JOIN seguranca.usuario_sistema us ON us.usucpf = usuario.usucpf AND us.sisid = 23 AND us.susstatus = 'A' AND us.suscod = 'A'
				WHERE
					ur.pflcod IN (".PAR_PERFIL_EQUIPE_ESTADUAL.", ".PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO.") AND
					ur.rpustatus = 'A' AND
					ur.estuf = '".$dadosEntidade['estuf']."'";

	} else { //Município

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				INNER JOIN seguranca.usuario_sistema us ON us.usucpf = usuario.usucpf AND us.sisid = 23 AND us.susstatus = 'A' AND us.suscod = 'A'
				WHERE
					ur.pflcod IN (".PAR_PERFIL_EQUIPE_MUNICIPAL.", ".PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO.", ".PAR_PERFIL_PREFEITO.") AND
					ur.rpustatus = 'A' AND
					ur.muncod = '".$dadosEntidade['muncod']."'";

	}

	$arDadosUsuarios = $db->carregar( $sql );
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

	$cc  = "";
	$cco = "";

	$assunto  = "MEC/FNDE - PAR com subação(ões) em diligência - ".$sbaid;

	$conteudo = '<p>Prezado(a) Senhor(a),</p>

				<p>O Plano de Ações Articuladas (PAR) possui subação(ões) na situação "em diligência".</p>

				<p>Por favor, acesse o PAR, consulte o ícone "Diligências - Abrir", solucione a(s) pendência(s) indicada(s) e encaminhe novamente a(s) subação(ões) para análise do MEC.</p>

				<p>Atenciosamente,</p>


				Equipe Técnica do PAR – Plano de Ações Articuladas<br/>
				Secretaria de Educação Básica<br/>
				Ministério da Educação<br/>
				Telefones: (61) 2022-8335 / 8336 / 8337 / 8338.<br/>
				Endereço eletrônico: planodemetas@mec.gov.br';

	foreach($arDadosUsuarios as $dados){
		$remetente = array('nome'=>$dados['usunome'], 'email'=>$_SESSION['email_sistema']);
		enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
	}
	return true;
}

function getNumDoc( $dopid )
{
	$sql = "SELECT
	CASE WHEN dp2.dopano::boolean THEN
		dp.dopnumerodocumento::text || '/' || dp2.dopano::text
	ELSE
		dp.dopnumerodocumento::text
	END
		as ndocumento
	FROM par.documentopar dp
		
	LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
	WHERE 
		dp.dopid = {$dopid}";
	global $db;
	 
	$result = $db->carregar( $sql );
	if(is_array($result) && count($result))
	{
		
			return $result[0]['ndocumento'];
	}
	else
	{
		return 'erro';
	}


}

/*
 * @desc Envia email caso o FNDE cancele ou diligencie a reprogramacao
 * @param string $tipo = 'D' para diligencia e 'C' para cancelamento
 * */

function enviaEmailDiligCancelReprogramacao($motivo,$sbaid, $ano, $tipo)
{
	global $db;
	
	if( (!empty($sbaid)) && (!empty($ano)) && (!empty($motivo)) )
	{
			
		$sql = "SELECT 
					inu.itrid,
						substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
						substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
						substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
						substring(prp.prpnumeroprocesso from 16 for 2) || ''
					as numeroprocesso, 
					par.retornacodigosubacao(sd.sbaid) codigo_subacao,
				CASE WHEN inu.itrid = 2 THEN
					inu.muncod
				WHEN inu.itrid = 1 THEN
					inu.estuf
				END as filtro
				
				FROM
					 par.subacaodetalhe sd
				INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid
				INNER JOIN par.processopar prp ON prp.prpid = ppc.prpid
				INNER JOIN par.instrumentounidade inu ON inu.inuid = prp.inuid 
				
				where sd.sbaid = {$sbaid}  AND sd.sbdano = {$ano}
		";
		$result = $db->pegaLinha($sql);
		
		$itrid 			= $result['itrid'];
		$filtro 		= $result['filtro'];
		$numeroProcesso = $result['numeroprocesso'];
		$codigoSubacao	= $result['codigo_subacao'];
			
		if( ($itrid == 2) && ($filtro)  )
		{
			$sqlEmail = "
				SELECT
					ent.entemail as email
				FROM
					par.entidade ent
				INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
				INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
				WHERE
					ent.dutid =  7
					and ent.entstatus = 'A'
				AND
					mun.muncod in ( '{$filtro}' )
				";
			
		}
		else if( ($itrid == 1) && ($filtro))
		{
			$sqlEmail = "
				SELECT
					ent.entemail as email
				FROM
					par.entidade ent
				INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
				INNER JOIN territorios.estado est on est.estuf = ent2.estuf
		
				WHERE
					ent.entstatus='A'
				AND
					ent.dutid =  10
				AND
					ent2.estuf in ( '{$filtro}' )";
		}
		
		$resultEmail = $db->pegalinha($sqlEmail);
		
		$emailTo =  $resultEmail['email'];
		
		if( ! $emailTo )
		{
			if($tipoReq == 'reformulacao')
			{
				return false;
			}	
			$contato = ($itrid == 2) ?  "Prefeito(a)" : "Secretário(a) Estadual";
		
			return false;
		}
		
		
		if( $tipo == 'D' )
		{
			$servico 		= "foi enviada para Diligência de Reprogramação.";
			$servicoTitulo	= "Diligência de Reprogramação, subação {$codigoSubacao}";
		} 
		else
		{
			$servico 		= "teve a Reprogramação cancelada.";
			$servicoTitulo 	= "Cancelamento da Reprogramação, subação {$codigoSubacao}.";
		}
		
	$strMensagem = "<pre> 
Prezado(a) dirigente,
	Informamos que a subação {$codigoSubacao}, do processo {$numeroProcesso}, {$servico} Com o seguinte parecer:
	\"$motivo\"
	<br>
Atenciosamente,
<br>
Equipe PAR 
</pre>
";
		
	
		$strAssunto = $servicoTitulo;
		$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
		$strMensagem = html_entity_decode($strMensagem);
			
		if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
			return true;
		}
		else if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
		{
			$strEmailTo = array($_SESSION['email_sistema']);
			$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			return $retorno;
		}
		else
		{
			$strEmailTo = $emailTo;
			$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			return $retorno;
		}
		exit();
	}
}

function enviaEmailRecusaReprogramacao( $dopid , $tipo = 'prazo', $parecer = '', $arrSbd = array()) 
{
	global $db;
	
	if($tipo == 'prazo')
	{
		$sql = "SELECT
					usu.usunome,
					usu.usuemail
				FROM
					seguranca.usuario usu
				INNER JOIN par.documentoparreprogramacao dpr ON dpr.usucpf = usu.usucpf
				WHERE
					dpr.dopid = ".$dopid." AND
					dpr.dprstatus = 'P'";
	}
	else 
	{
		$sql ="SELECT
			usu.usunome,
			usu.usuemail
		FROM
			seguranca.usuario usu
		INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.usucpf = usu.usucpf
		WHERE
			dpr.dopid = ".$dopid." AND
			dpr.dpsstatus = 'P'
		GROUP BY usunome, usuemail";
	}
	
	$arDadosUsuarios = $db->carregar( $sql );
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();
	
	$cc  = "";
	$cco = $_SESSION['email_sistema'];
	
	$strTipo = ( $tipo =='prazo' ) ? 'Prazo' : 'Subação';
	$ndop  	 = getNumDoc($dopid);
	$assunto  = "Sua solicitação de reprogramação de '{$strTipo}' do Termo de Compromisso {$ndop} foi indeferida";
	
	$conteudo = '<p>Prezado (a) Dirigente,</p>

				<p>Após análise técnica do pedido de reprogramação do termo de compromisso, informamos que a sua solicitação foi indeferida por um dos motivos apresentados abaixo:</p>

				1)	Não apresentou argumentos suficientes para justificar o pedido ora analisado;<br/>
				2)	O objeto foi totalmente executado;<br/>
				3)	Não existe saldo/rendimento de aplicação financeira;<br/>
				4)	Ente federativo não apresenta necessidade de reformulação de itens de composição.';
	
	if( $parecer != '' )
	{
		$conteudo .= "
				<br/><br>
				Parecer do Técnico:<br/>
				{$parecer}
				";
	}
	
	if( ($tipo == 'subacao') && ( (is_array($arrSbd)) && (count($arrSbd) > 0 ) ) )
	{
		$conteudo .= "
		<br/><br>
		Subações indeferidas:<br/>
		";
		foreach($arrSbd as $k => $sbdid )
		{
			$sqlSbd = 
			"select sbadsc as descricao, par.retornacodigosubacao(sba.sbaid) as codigo from par.subacao sba
			INNER JOIN par.subacaodetalhe sbd ON sba.sbaid = sbd.sbaid
			WHERE sbd.sbdid = {$sbdid}
			limit 1";
			$descSbd = $db->pegaLinha($sqlSbd);
			$descricaoSba = $descSbd['descricao'];
			$codigoSba = $descSbd['codigo'];
			$conteudo .="	{$codigoSba} - {$descricaoSba}<br>";
		}
	}
	
	$remetente = array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br');
	
	
	if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
	
		if($tipoReq == 'reformulacao')
		{
				
			return true;
		}
		else
		{
			$retorno = true;
		}
	
	}
	elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
	{
		$strEmailTo = array($_SESSION['email_sistema'],$cco);
		$retorno = enviar_email($remetente, $strEmailTo, $assunto, $conteudo, $cc, $cco );
	} 
	else 
	{	
		foreach($arDadosUsuarios as $dados)
		{
			$strEmailTo[] = $dados['usuemail'];
		}
		$retorno = enviar_email($remetente, $strEmailTo, $assunto, $conteudo, $cc, $cco );
	}
	 
	return true;
}

function enviaEmailInuidFaltandoCargaSubacao( $arrInuid ){
	global $db;

	foreach( $arrInuid as $v => $inuid ){
		$w[] = $v;
	}
	$sql = "SELECT
				CASE WHEN itrid = 1 THEN iu.estuf ELSE m.mundescricao || ' - ' || m.estuf END AS local, inuid
			FROM
				par.instrumentounidade iu
			LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
			where iu.inuid IN (".implode(", ",$w).")";

	$dadosEntidade = $db->carregar( $sql );

	$sql = "SELECT
				usunome,
				usuemail
			FROM
				seguranca.usuario
			WHERE
				usucpf = '".$_SESSION['usucpf']."'";


	$dados = $db->pegaLinha( $sql );

	$cc  = "";
	$cco = "";
	$assunto  = "MEC/FNDE - PAR - Erros com Carga de Subações";

	$conteudo = '<p>Prezado(a) Senhor(a),</p>

				<p>Ocorreram falhas no momento da Carga de Subações.</p>

				<p>O(s) local(is) com erro foi(ram):</p>

				';
	if(is_array( $dadosEntidade )){
		foreach( $dadosEntidade as $entidade ){
			$conteudo .= $entidade['local'].' (Motivo: '.$arrInuid[$entidade['inuid']]['motivo'].')<br>';
		}
	}

	$conteudo .= '<p>Atenciosamente,</p>


				Equipe Técnica do PAR – Plano de Ações Articuladas<br/>
				Secretaria de Educação Básica<br/>
				Ministério da Educação<br/>
				Telefones: (61) 2022-8335 / 8336 / 8337 / 8338.<br/>
				Endereço eletrônico: planodemetas@mec.gov.br';

	$remetente = array('nome'=>$dados['usunome'], 'email'=>$_SESSION['email_sistema']);
	enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
	return true;
}

function enviaEmailInuidPiPtres( $arrInuid ){
	global $db;

	$sql = "SELECT
				usunome,
				usuemail
			FROM
				seguranca.usuario
			WHERE
				usucpf = '".$_SESSION['usucpf']."'";


	$dados = $db->pegaLinha( $sql );

	$cc  = "";
	$cco = "";

	$assunto  = "MEC/FNDE - PAR - Erros com Carga de PI e PTRES";

	$conteudo = '<p>Prezado(a) Senhor(a),</p>

				<p>Ocorreram falhas no momento da Carga de PI E PTRES das Subações.</p>

				<p>O(s) local(is) com erro foi(ram):</p>

				';
	if(is_array( $arrInuid )){
		foreach( $arrInuid as $inuid ){
			$conteudo .= $inuid['local'].' (Motivo: '.$inuid['motivo'].')<br>';
		}
	}

	$conteudo .= '<p>Atenciosamente,</p>


				Equipe Técnica do PAR – Plano de Ações Articuladas<br/>
				Secretaria de Educação Básica<br/>
				Ministério da Educação<br/>
				Telefones: (61) 2022-8335 / 8336 / 8337 / 8338.<br/>
				Endereço eletrônico: planodemetas@mec.gov.br';

	$remetente = array('nome'=>$dados['usunome'], 'email'=>$_SESSION['email_sistema']);
	enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
	return true;
}

function enviaEmailPlanodeMetasSubacaoReformulacao( $inuid, $sbaid ){
	global $db;

	$sql = "SELECT
				itrid, estuf, muncod
			FROM
				par.instrumentounidade where inuid = ".$inuid;

	$dadosEntidade = $db->pegaLinha( $sql );

	if( $dadosEntidade['itrid'] == 1 ){ //Estado

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				WHERE
					ur.pflcod IN (".PAR_PERFIL_EQUIPE_ESTADUAL.", ".PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO.") AND
					ur.rpustatus = 'A' AND
					ur.estuf = '".$dadosEntidade['estuf']."'";

	} else { //Município

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				WHERE
					ur.pflcod IN (".PAR_PERFIL_EQUIPE_MUNICIPAL.", ".PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO.", ".PAR_PERFIL_PREFEITO.") AND
					ur.rpustatus = 'A' AND
					ur.muncod = '".$dadosEntidade['muncod']."'";

	}

	$arDadosUsuarios = $db->carregar( $sql );
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

	$cc  = "";
	$cco = "";

	$assunto = 'MEC/FNDE - PAR com subação(ões) em Reformulação - '.$sbaid;

	$conteudo = '<p>Prezado(a) Senhor(a),</p>

				<p>O Plano de Ações Articuladas (PAR) possui subação(ões) na situação "em reformulação".</p>

				<p>Atenciosamente,</p>


				Equipe Técnica do PAR – Plano de Ações Articuladas<br/>
				Secretaria de Educação Básica<br/>
				Ministério da Educação<br/>
				Telefones: (61) 2022-8335 / 8336 / 8337 / 8338.<br/>
				Endereço eletrônico: planodemetas@mec.gov.br';

	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$remetente = array('nome'=>$dados['usunome'], 'email'=>$_SESSION['email_sistema']);
		enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $conteudo, $cc, $cco );
	} else {
		foreach($arDadosUsuarios as $dados){
			$remetente = array('nome'=>$dados['usunome'], 'email'=>$_SESSION['email_sistema']);
			enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
		}
	}
	return true;
}

function enviaEmailPlanodeMetasObrasReformulacao( $inuid, $preid ){
	global $db;

	$sql = "SELECT
				itrid, estuf, muncod
			FROM
				par.instrumentounidade where inuid = ".$inuid;

	$dadosEntidade = $db->pegaLinha( $sql );

	if( $dadosEntidade['itrid'] == 1 ){ //Estado

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				WHERE
					ur.pflcod IN (".PAR_PERFIL_COORDENADOR_GERAL.") AND
					ur.rpustatus = 'A' AND
					ur.estuf = '".$dadosEntidade['estuf']."'";

	} else { //Município

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				WHERE
					ur.pflcod IN (".PAR_PERFIL_COORDENADOR_GERAL.") AND
					ur.rpustatus = 'A' AND
					ur.muncod = '".$dadosEntidade['muncod']."'";

	}

	$arDadosUsuarios = $db->carregar( $sql );
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

	$cc  = "";
	$cco = "";

	$assunto = 'MEC/FNDE - PAR com obra(s) em Reformulação - '.$preid;

	$conteudo = '<p>Prezado(a) Senhor(a),</p>

				<p>O Plano de Ações Articuladas (PAR) possui obra(s) na situação "em reformulação".</p>

				<p>Atenciosamente,</p>


				Equipe Técnica do PAR – Plano de Ações Articuladas<br/>
				Secretaria de Educação Básica<br/>
				Ministério da Educação<br/>
				Telefones: (61) 2022-8335 / 8336 / 8337 / 8338.<br/>
				Endereço eletrônico: planodemetas@mec.gov.br';

	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$remetente = array('nome'=>$dados['usunome'], 'email'=>$_SESSION['email_sistema']);
		enviar_email(array('nome'=>'SIMEC - OBRAS PAR', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $conteudo, $cc, $cco );
	} else {
		if( is_array($arDadosUsuarios) ){
			foreach($arDadosUsuarios as $dados){
				$remetente = array('nome'=>$dados['usunome'], 'email'=>$_SESSION['email_sistema']);
				enviar_email(array('nome'=>'SIMEC - OBRAS PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
			}
		}
	}
	return true;
}

function regraPosPreAnalisePnaic()
{
	global $db;

	regraPosPreAnalise();

	if($_SESSION['par']['muncod']){
		/*
		$funid1 = '15';
		$funid2 = '7';
		$stWhere = " AND eed2.muncod = '{$_SESSION['par']['muncod']}'";
		*/
		$dutid1 = DUTID_SECRETARIA_MUNICIPAL;
		$dutid2 = DUTID_DIRIGENTE;
		$stWhere = " AND ent2.muncod = '{$_SESSION['par']['muncod']}'";
	}else{
		/*
		$funid1 = '25';
		$funid2 = '6';
		*/
		$dutid1 = DUTID_SECRETARIA_ESTADUAL;
		$dutid2 = DUTID_SECRETARIO_ESTADUAL;
		$stWhere = "";
	}

	$sql = "SELECT
				ent.entemail
			FROM
				par.entidade ent
			INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = {$dutid1}
			INNER JOIN territorios.estado est on est.estuf = ent2.estuf
			where 
				ent.dutid = {$dutid2} AND
				ent.entstatus='A' AND
				ent2.entstatus='A' AND 
				ent2.estuf = '{$_SESSION['par']['estuf']}' {$stWhere}";

	$rs = $db->pegaLinha($sql);

	$arEmail = array($rs['entemail']);

	$remetente 	= '';
	$assunto	= 'Conclusão de Adesão ao Pacto';

	$conteudo	= '
					<p>Prezado(a) Secretário(a),</p>

					A sua Secretaria de Educação acaba de concluir a adesão ao Pacto Nacional pela Alfabetização na Idade Certa e às
					Ações do Pacto. Em breve, o Ministério da Educação entrará em contato para informar as próximas etapas. Para saber
					mais, acesse o portal do MEC: http://www.mec.gov.br.
				  ';

	$cc			= array();
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	$sql = "select
				pcuemail
			from
				par.pfcursista
			where
				pfcid = 12
			and
				adpid = ".$_SESSION['par']['adpid'];

	$rs = $db->pegaLinha($sql);

	$arEmail = array($rs['pcuemail']);

	$remetente 	= '';
	$assunto	= 'Conclusão de Adesão ao Pacto';

	$conteudo	= '
					<p>Prezado(a) senhor(a),</p>

					Você foi indicado(a) por seu(sua) Secretário(a) de Educação como Coordenador(a)
					das Ações do Pacto Nacional pela Alfabetização na Idade Certa. Em breve,
					o Ministério da Educação entrará em contato para informar as próximas etapas.
					Para saber mais, acesse o portal do MEC: http://www.mec.gov.br.
				  ';

	$cc			= array();
	$cco		= '';
	$arquivos 	= array();

	enviar_email( $remetente, $arEmail, $assunto, $conteudo, $cc, $cco, $arquivos );

	echo "<script>

				var url = location.href; 	//pega endereço que esta no navegador
				url = url.split('/'); 		//quebra o endeço de acordo com a / (barra)
				var dominio = url[2]; 		// retorna a parte www.endereco.com.br

				alert('Sua Secretaria de Educação concluiu a adesão ao Pacto Nacional pela Alfabetização na Idade Certa e às Ações do Pacto. Em breve, o Ministério da Educação entrará em contato. Para saber mais, acesse o portal do MEC: http://www.mec.gov.br.');
				window.opener.location.href = 'http://'+dominio+'/par/par.php?modulo=inicio&acao=C';

		  </script>";

	return true;
}

function regraEnviaPreAnalisePnaic(){

	global $db;

	$sql = "select
				pfaid
			from
				par.pfadesao
			where
				now() between pfadatainicial and pfadatafinal
			and
				prgid = {$_SESSION['par']['prgid']}";

	$validade = $db->pegaUm($sql);

	if(!$validade && $_SESSION['par']['prgid'] != PROG_PAR_ALFABETIZACAO_IDADE_CERTA){
		return 'Prazo expirado!';
	}

	$msg = 'É necessário cadastrar o(a) coordenador(a) do Pacto!';

	//pega total alunos
	$sql = "select count(pcuid) as tot from par.pfcursista
		WHERE adpid = ". $_SESSION['par']['adpid'];
	$tot_aluno = $db->pegaUm($sql);

	if($tot_aluno > 0){
		return true;
	}
	else{
		return $msg;
	}

}


function regraEnviarAnaliseAguaEscola(){

	global $db;

	if($_SESSION['par']['itrid'] == 2){
		$stWhere = "AND eed.muncod = '{$_SESSION['par']['muncod']}'";
	}else{
		$stWhere = "AND eed.estuf = '{$_SESSION['par']['estuf']}'";
	}

	//verifica se tem escolas no estado ou municipio
	$sql = "SELECT
					count(pfa.paeid)
			FROM par.pfaguaescola pfa
			INNER JOIN entidade.entidade ent ON ent.entcodent = pfa.entcodent
			INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
			INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
			WHERE ent.entstatus = 'A'
			AND ent.entcodent IS NOT NULL
			AND ent.entnome IS NOT NULL
			AND ent.tpcid = ".($_SESSION['par']['itrid'] == 2 ? 3 : 1);
	$totalEscolas = $db->pegaUm($sql);

	if($totalEscolas == 0){
		return 'Não exitem escolas para este Estado ou Município!';
	}

	//verifica se tem escolas no estado ou municipio
	$sql = "SELECT
					count(pfa.paeid)
			FROM par.pfaguaescola pfa
			INNER JOIN entidade.entidade ent ON ent.entcodent = pfa.entcodent
			INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
			INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
			WHERE ent.entstatus = 'A'
			AND (pfa.paeparticipa = 'S' or pfa.paeparticipa is null)
			AND ent.entcodent IS NOT NULL
			AND ent.entnome IS NOT NULL
			AND ent.tpcid = ".($_SESSION['par']['itrid'] == 2 ? 3 : 1);
	$totalEscolasSim = $db->pegaUm($sql);


	//verifica se tem no minimo e no maximo 4 fotos em cada escola
	$sql = "SELECT
					count(f.paeid)
			FROM par.pfaguaescola pfa
			INNER JOIN entidade.entidade ent ON ent.entcodent = pfa.entcodent
			INNER JOIN entidade.endereco eed ON ent.entid = eed.entid {$stWhere}
			INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid AND fue.funid = ".FUNID_ESCOLA."
			INNER JOIN (select paeid, count(pfoid) as totalfotos from par.pffotosaguaescola group by paeid) f on f.paeid = pfa.paeid
			WHERE	ent.entstatus = 'A'
			AND (pfa.paeparticipa = 'S' or pfa.paeparticipa is null)
			AND ent.entcodent IS NOT NULL
			AND ent.entnome IS NOT NULL
			AND ent.tpcid = ".($_SESSION['par']['itrid'] == 2 ? 3 : 1)."
			and totalfotos > 2";
	$totalEscolasFotos = $db->pegaUm($sql);

	if($totalEscolasSim == $totalEscolasFotos){
		return true;
	}
	else{
		return 'É necessário anexar no mínimo 3 fotos ou informar a justificativa para cada escola!';
	}

}

function verificarPreenchimentoEscolaTerra( $esaid = null ){
	global $db;

	$sql = "SELECT * FROM escolaterra.adesaoprogramaescola WHERE adpid='".$_SESSION['par']['adpid']."'";
	$adesaoprogramaescola = $db->carregar($sql);

	if($adesaoprogramaescola[0]) {
		foreach($adesaoprogramaescola as $ades) {
			if(!$ades['apeprofessoressolicitados']) {
				return 'É obrigatório informar o número de participantes por escolas';
			}
		}
	} else {
		return 'Selecione as escolas que irão participar do programa';
	}

	return true;
}

function alteraSituacaoObra( $preid ){

	global $db;

	$sql = "SELECT
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";

	$arObra = $db->pegaLinha( $sql );

	$sql = "SELECT
				aedid
			FROM workflow.acaoestadodoc
			WHERE
				esdiddestino = ".OBR_ESDID_OBRA_CANCELADA."
				AND esdidorigem = {$arObra['esdid']}";

	$aedid = $db->pegaUm($sql);

	if( $aedid == '' ){
		$sql = "INSERT INTO workflow.acaoestadodoc
					(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
					esdsncomentario, aedvisivel, aedcodicaonegativa)
				VALUES(
					{$arObra['esdid']}, ".OBR_ESDID_OBRA_CANCELADA.", 'Enviar para Obras Cancelada',
					'A', 'Enviada para Obras Cancelada',
					true, false, false )
				RETURNING
					aedid";

		$aedid = $db->pegaUm($sql);
	}

	wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por alteraSituacaoObra preid = '.$preid,
					  array( 'docid' => $arObra['docid'] ) );

	// necessita depara stoid = esdid
//	$sql = "UPDATE obr as.ob rainfraestrutura SET stoid = 10 WHERE preid = $preid";
//	$db->executar( $sql );

//	return $db->commit();
}

function alterarDadosObras( $preid ){
	global $db;
	/*** INICIO - Importação dos dados para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra exista ***/
	$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid is not null";
	$existeObra = $db->pegaUm($sql);

	if( (integer)$existeObra <> 0 ){
		/*** Recupera dados da Pre Obra ***/
//		$sql = "SELECT
//					oi.obrid, oi.endid, oi.tpoid, p.ptoid,
//					p.predescricao as nome_obra,
//					mun.mundescricao,
//					ent.entid as unidade_implantadora,
//					p.precep,
//					p.prelogradouro,
//					p.precomplemento,
//					p.prebairro,
//					p.muncod,
//					p.estuf,
//					p.prenumero,
//					p.prelatitude,
//					p.prelongitude,
//					p.preesfera
//				FROM
//					obras.preobra p
//					INNER JOIN territorios.municipio mun on p.muncod = mun.muncod
//					INNER JOIN entidade.endereco ende ON ende.muncod = p.muncod
//					INNER JOIN entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
//					INNER JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid IN (1)
//					left join obra s.ob rainfraestrutura oi on oi.preid = p.preid and oi.obrid = p.obrid
//				WHERE
//					p.preid = ".$preid;

		$sql = "SELECT
					oi.obrid, oi.docid, doc.esdid, oi.empid, oi.endid, oi.tpoid, p.ptoid,
					p.predescricao as nome_obra,
					mun.mundescricao,
					ent.entid as unidade_implantadora,
					p.precep,
					p.prelogradouro,
					p.precomplemento,
					p.prebairro,
					p.muncod,
					p.estuf,
					p.prenumero,
					p.prelatitude,
					p.prelongitude,
					p.preesfera
				FROM
					obras.preobra p
				INNER JOIN territorios.municipio 	mun  ON p.muncod = mun.muncod
				INNER JOIN entidade.endereco 		ende ON ende.muncod = p.muncod
				INNER JOIN entidade.entidade 		ent  ON ent.entid = ende.entid AND ent.entstatus = 'A'
				INNER JOIN entidade.funcaoentidade 	fen  ON ent.entid = fen.entid  AND fen.funid IN (1)
				LEFT  JOIN obras2.obras 			oi   ON oi.preid = p.preid 	   AND oi.obrid = p.obrid
				LEFT  JOIN workflow.documento		doc  ON doc.docid = oi.docid
				WHERE
					p.preid = $preid";
		$dadosPreObra = $db->carregar($sql);

		if( $dadosPreObra[0]['endid'] ){
			$sql = "UPDATE entidade.endereco SET
						endcep = '".$dadosPreObra[0]['precep']."',
  						endlog = '".$dadosPreObra[0]['prelogradouro']."',
					  	endcom = '".$dadosPreObra[0]['precomplemento']."',
					  	endbai = '".$dadosPreObra[0]['prebairro']."',
					  	muncod = '".$dadosPreObra[0]['muncod']."',
					  	estuf = '".$dadosPreObra[0]['estuf']."',
					  	endnum = '".$dadosPreObra[0]['prenumero']."',
  						endstatus = 'A',
  						medlatitude = '".$dadosPreObra[0]['prelatitude']."',
  						medlongitude = '".$dadosPreObra[0]['prelongitude']."'
					WHERE
  						endid = ".$dadosPreObra[0]['endid'];

			$db->executar( $sql );
			$endid = $dadosPreObra[0]['endid'];
		} else {
			/*** Insere novo endereço da obra ***/
			$sql = "INSERT INTO
						entidade.endereco (endcep,
										   endlog,
										   endcom,
										   endbai,
										   muncod,
										   estuf,
										   endnum,
										   medlatitude,
										   medlongitude,
										   endstatus)

					VALUES
						( '".$dadosPreObra[0]['precep']."',
						  '".$dadosPreObra[0]['prelogradouro']."',
						  '".$dadosPreObra[0]['precomplemento']."',
						  '".$dadosPreObra[0]['prebairro']."',
						  '".$dadosPreObra[0]['muncod']."',
						  '".$dadosPreObra[0]['estuf']."',
						  '".$dadosPreObra[0]['prenumero']."',
						  '".$dadosPreObra[0]['prelatitude']."',
						  '".$dadosPreObra[0]['prelongitude']."',
						  'A' ) RETURNING endid";

			$endid = $db->pegaUm($sql);
		}

		/*** Atualiza a nova obra ***/
		if( $dadosPreObra[0]['obrid'] ){
			//ptoid
			if( $dadosPreObra[0]['ptoid'] ) $tpoid = $db->pegaUm( "select tpoid from obras.pretipoobra where ptoid = ".$dadosPreObra[0]['ptoid'] );

	  		$sql = "UPDATE obras2.obras SET
						tobid = 1,
						endid = $endid,
						obrnome = '{$dadosPreObra[0]['nome_obra']}',
						entid = {$dadosPreObra[0]['unidade_implantadora']},
						tpoid = ".($tpoid ? $tpoid : 'null')."
					WHERE
						obrid = {$dadosPreObra[0]['obrid']};
					UPDATE obras2.empreendimento SET
						tobid = 1,
						endid = $endid,
						empdsc = '{$dadosPreObra[0]['nome_obra']}',
						entidunidade = ".($dadosPreObra[0]['unidade_implantadora'] ? $dadosPreObra[0]['unidade_implantadora'] : 'null').",
						tpoid = ".($tpoid ? $tpoid : 'null').",
						empesfera = '{$dadosPreObra[0]['preesfera']}'
					WHERE
						empid = {$dadosPreObra[0]['empid']}";

			$db->executar( $sql );

			$sql = "SELECT
						aedid
					FROM workflow.acaoestadodoc
					WHERE
						esdiddestino = ".OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE."
						AND esdidorigem = {$dadosPreObra[0]['esdid']}";

			$aedid = $db->pegaUm($sql);

			if( $aedid == '' ){
				$sql = "INSERT INTO workflow.acaoestadodoc
							(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
							esdsncomentario, aedvisivel, aedcodicaonegativa)
						VALUES(
							{$dadosPreObra[0]['esdid']}, ".OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE.",
							'Enviar para em planejamento pelo proponente', 'A', 'Enviada para em planejamento pelo proponente',
							true, false, false )
						RETURNING
							aedid";

				$aedid = $db->pegaUm($sql);
			}

			wf_alterarEstado( $dadosPreObra[0]['docid'], $aedid, 'Tramitado por alterarDadosObras preid = '.$preid,
							   array( 'docid' => $dadosPreObra[0]['docid'] ) );

//			$sql = "DELETE FROM obr as.ar quivosobra WHERE obrid = ".$dadosPreObra[0]['obrid']." and tpaid = 21";
			$sql = "DELETE FROM obras2.arquivosobra WHERE obrid = ".$dadosPreObra[0]['obrid']." AND tpaid = 21";
			$db->executar( $sql );

			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						public.arquivo arq
					INNER JOIN obras.preobrafotos pof ON arq.arqid = pof.arqid
					INNER JOIN obras.preobra pre ON pre.preid = pof.preid
					WHERE
						pre.preid = ".$preid."
						AND (substring(arqtipo,1,5) = 'image')";
			$fotosTerreno = $db->carregar($sql);

			if( $fotosTerreno ){
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto){
//					INSERT INTO
//							obr as.ar quivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
					$sql = "INSERT INTO
							obras2.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
							VALUES
							(".$dadosPreObra[0]['obrid'].", 21, ".$foto['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
					$db->executar($sql);
				}
			}

			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						obras.preobraanexo p
					INNER JOIN public.arquivo arq ON arq.arqid = p.arqid
					WHERE
						p.preid = ".$preid;
			$anexos = $db->carregar($sql);

			if( $anexos )
			{
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
//					INSERT INTO
//							obr as.ar quivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
					$sql = "INSERT INTO
							obras2.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
							VALUES
							(".$dadosPreObra[0]['obrid'].", 21, ".$anexo['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
					$db->executar($sql);
				}
			}

			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid = ".$dadosPreObra[0]['obrid']." WHERE preid = ".$preid;
			$db->executar($sql);
		}
	}
	return $db->commit();
	/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
}

function pegaItridSubacao( $sbaid ){
	
	global $db;
	
	$sql = "SELECT 
				itrid
			FROM 
				par.subacao sba
			INNER JOIN par.acao aca ON aca.aciid = sba.aciid
			INNER JOIN par.pontuacao pto ON pto.ptoid = aca.ptoid
			INNER JOIN par.instrumentounidade inu ON inu.inuid = pto.inuid
			WHERE sbaid = $sbaid";
	
	return $db->pegaUm($sql);
}


#Função que busca os programas que estão em data hábil para adesão, o seu resultado é um array que tras os id dos programas.
#A busca é feita na tabela par.pfadesao.
function pegarProgramaDisponivel(){
	global $db;

	$sql = "Select 	pf.pfaid
			From par.programa p
			Join par.pfadesao pf on pf.prgid = p.prgid
			Where '".date('Y-m-d')."' between  pf.pfadatainicial and pf.pfadatafinal
			Order by 1";

	$sis_pfaid = $db->carregar($sql);
	$pfaid = array();
	if($sis_pfaid){
		foreach ($sis_pfaid as $val){
			$pfaid[] = $val['pfaid'];
		}
	}
	return $pfaid;
}


// FUNCOES DO FINANCIAMENTO DE NOVAS TURMAS DE EJA
function verificaPreenchimentoQuestionarioEJA(){
    global $db;
    
    $sql ="
        SELECT qejid FROM eja.questionarioeja WHERE inuid = {$_SESSION['par']['inuid']}
    ";
    $qejid = $db->pegaUm($sql);
    
    $sql = "
        SELECT ejaid FROM par.pfescolaeja WHERE adpid = {$_SESSION['par']['adpid']}
    ";
    $ejaid = $db->pegaUm($sql);
    
    if($qejid > 0 && $ejaid > 0){
        return true;
    }else{
        return false;
    }
}

function verificaAnoMeta( $preid ){
	
	global $db;
	
	$sql = "SELECT
				pre.preanometa
			FROM
				obras.preobra pre
			INNER JOIN
				obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptoclassificacaoobra IN ('P','Q','C')
			WHERE
				pre.preid = $preid";

	$peanometa = $db->pegaUm( $sql );
	
	if( !$peanometa ){
// 		$sql = "UPDATE obras.preobra SET
// 					preanometa = 2014
// 				WHERE
// 					preid = $preid";
// 		$db->executar($sql);
// 		return $db->commit();
		return false;
	}
	return true;
}

function importarObrasAposDeferimento($preid) {
	global $db;
	$sql = "SELECT p.preid
			FROM obras.preobra p 
			INNER JOIN workflow.documento d ON d.docid = p.docid
			INNER JOIN workflow.historicodocumento hd ON hd.docid = d.docid
			INNER JOIN  workflow.acaoestadodoc   a ON a.aedid = hd.aedid
			WHERE a.esdiddestino = ".WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL." AND p.preid = {$preid} ";
	
	$preid = $db->pegaUm($sql);
	
	$sql = "SELECT 
				pro.proid 
			FROM 
				par.processoobrasparcomposicao poc
			INNER JOIN par.processoobraspar pro ON pro.proid = poc.proid AND pro.prostatus = 'A'
			WHERE 
				poc.preid = '$preid' 
			AND 
				poc.pocstatus = 'A'";
	$proid = $db->pegaUm($sql);
		
	//Inclusão de regra para importart obra Demanda 1964
	if( $esdid && $proid ) {
		$preObra = new PreObra();
		$preObra->importarPreobraParaObras2($preid);
	}
}

function atualizaObrasReformulacaoProponente( $preid ){

	global $db;

	if( alterarDadosObrasReformulada( $preid ) ){

		$arrTipoObrasMI = Array(42,43,44,45);
		
		$sql = "SELECT ptoid FROM obras.preobra WHERE preid = $preid";
		
		$ptoid = $db->pegaUm($sql);
		
		if( in_array($ptoid, $arrTipoObrasMI) ){
			$esdidDestino = OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO;
		}elseif($_POST['esdid']){
			$esdidDestino = $_POST['esdid'];
		}else{
			/* Regra definida por Fabio dia 22/06/2015 às 10:26 */
			$esdidDestino = OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE;
		}

		$sql = "SELECT
					doc.esdid,
					doc.docid
				FROM
					obras2.obras obr
				INNER JOIN workflow.documento doc ON doc.docid = obr.docid
				WHERE
					preid = $preid
					AND obrstatus = 'A'";

		$obra = $db->pegaLinha( $sql );

		$obra = is_array($obra) ? $obra : Array();
		
		extract($obra);

		if( $esdid ){
			$sql = "SELECT
						aedid
					FROM workflow.acaoestadodoc
					WHERE
						esdiddestino = $esdidDestino
						AND esdidorigem = $esdid";
	
			$aedid = $db->pegaUm($sql);
	
			if( $aedid == '' ){
				$sql = "INSERT INTO workflow.acaoestadodoc
							(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
							esdsncomentario, aedvisivel, aedcodicaonegativa)
						VALUES(
							$esdid, $esdidDestino,
							'Enviar para em planejamento pelo proponente', 'A', 'Enviada para em planejamento pelo proponente',
							true, false, false )
						RETURNING
							aedid";
	
				$aedid = $db->pegaUm($sql);
			}
	
			wf_alterarEstado( $docid, $aedid, 'Tramitado por atualizaObrasReformulacaoProponente preid = '.$preid,
							  array( 'docid' => $docid ) );
		}

	} else {
		$return2 = false;
	}

	if( alterarDadosObras2Reformulada( $preid ) ){

		$sql = "UPDATE obras2.obras SET staid = 99 WHERE preid = $preid";
		$db->executar( $sql );

		$sql = "UPDATE obras.preobra SET prereformulacao = false WHERE preid = $preid";
		$db->executar( $sql );
		$return2 = $db->commit();
	} else {
		$return2 = false;
	}
}

function atualizaObrasReformulacaoProponentePAC( $preid ){

	global $db;
	
	if( !verificaAnoMeta( $preid ) ){
		return false;
	}

	if( alterarDadosObrasReformuladaPAC( $preid ) ){

		$arrTipoObrasMI = Array(42,43,44,45);
		
		$sql = "SELECT ptoid FROM obras.preobra WHERE preid = $preid";
		
		$ptoid = $db->pegaUm($sql);
		
		if( in_array($ptoid, $arrTipoObrasMI) ){
			$esdidDestino = OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO;
		}elseif($_POST['esdid']){
			$esdidDestino = $_POST['esdid'];
		}else{
			/* Regra definida por Fabio dia 22/06/2015 às 10:26 */
			$esdidDestino = OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE;
		}
		
		$sql = "SELECT
					doc.esdid,
					doc.docid
				FROM
					obras2.obras obr
				INNER JOIN workflow.documento doc ON doc.docid = obr.docid
				WHERE
					preid = $preid
					AND obrstatus = 'A'";

		$obra = $db->pegaLinha( $sql );
		$obra = is_array($obra) ? $obra : Array();
		
		extract($obra);

		if( $esdid != '' ){
			$sql = "SELECT
						aedid
					FROM workflow.acaoestadodoc
					WHERE
						esdiddestino = $esdidDestino
						AND esdidorigem = $esdid";
	
			$aedid = $db->pegaUm($sql);
		}

		if( $aedid == '' && $esdid != '' && $esdidDestino != '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa)
					VALUES(
						$esdid, $esdidDestino,
						'Enviar para em planejamento pelo proponente', 'A', 'Enviada para em planejamento pelo proponente',
						true, false, false )
					RETURNING
						aedid";

			$aedid = $db->pegaUm($sql);
		}
		
		if( $aedid != '' ){
			wf_alterarEstado( $docid, $aedid, 'Tramitado por atualizaObrasReformulacaoProponentePAC preid = '.$preid,
							  array( 'docid' => $docid ) );
		}

	} else {
		$return = false;
	}

	if( alterarDadosObras2ReformuladaPAC( $preid ) ){

		$sql = "UPDATE obras2.obras SET staid = 99 WHERE preid = $preid";
		$db->executar( $sql );

		$sql = "UPDATE obras.preobra SET prereformulacao = false WHERE preid = $preid";
		$db->executar( $sql );
		$return2 = $db->commit();
	} else {
		$return2 = false;
	}

	return (($return2 && $return) ? true : false);
}

function criaBKPObraMIObras( $preid ){

	global $db;

	$sql = "SELECT
				p.predescricao as nome_obra,
				--ent.entid as unidade_implantadora,
				p.precep,
				p.prelogradouro,
				p.precomplemento,
				p.prebairro,
				p.muncod,
				p.estuf,
				p.prenumero,
				p.prelatitude,
				p.prelongitude,
				UPPER(p.preesfera) AS preesfera,
				CASE
					WHEN pt.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
					WHEN pt.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
					WHEN pt.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
					ELSE 54 --OUTROS
                END as programa,
                CASE WHEN pt.ptoclassificacaoobra = 'Q' THEN 3 ELSE 1 END as modalidadeensino, -- MODALIDADE DE ENSINO
				CASE
					WHEN pt.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
					WHEN pt.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
					ELSE 1 --CONSTRUÇÃO
				END AS tipodeobra,
				CASE
					WHEN REPLACE(UPPER(p.predescricao), 'Í', 'I') ILIKE '%INDIGENA%' THEN 4 -- INDÍGENA
					WHEN UPPER(p.predescricao) ILIKE '%RURAL%' THEN 1 -- RURAL
					WHEN UPPER(p.predescricao) ILIKE '%QUILOMBO%' THEN 3 -- QUILOMBO
				ELSE 2 --URBANO
				END AS classificacaoobra,
				p.prevalorobra as valorobra,
				pt.tpoid,
				obr.obrid,
				obr.empid
			FROM
				obras.preobra p
			INNER JOIN obras2.obras obr ON obr.obrid = p.obrid
			INNER JOIN territorios.municipio mun on p.muncod = mun.muncod
			LEFT  JOIN obras.pretipoobra pt ON pt.ptoid = p.ptoid
			WHERE
				p.preid = ".$preid;

	$dadosPreObra = $db->carregar($sql);

	//DEFINDO A ENTIDADE
	if($dadosPreObra[0]['preesfera']=='M'){
		$sql = "SELECT ent.entid
				FROM entidade.entidade ent
				INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
				INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid
				WHERE ent.entstatus = 'A'
				AND fue.funid IN (1)
				AND fue.fuestatus = 'A'
				AND ed.muncod = '".$dadosPreObra[0]['muncod']."'";
		$unidade_implantadora = $db->pegaUm($sql);
	}else{
		$sql = "SELECT ent.entid
				FROM entidade.entidade ent
				INNER JOIN entidade.endereco ed ON ed.entid = ent.entid
				INNER JOIN entidade.funcaoentidade fue ON ent.entid = fue.entid
				WHERE ent.entstatus = 'A'
				AND fue.funid IN (6)
				AND fue.fuestatus = 'A'
				AND ed.estuf = '".$dadosPreObra[0]['estuf']."'";
		$unidade_implantadora = $db->pegaUm($sql);
	}
	
	/*** Insere novo endereço da obra ***/
	$sql = "INSERT INTO entidade.endereco (
				   tpeid,
				   endcep,
				   endlog,
				   endcom,
				   endbai,
				   muncod,
				   estuf,
				   endnum,
				   medlatitude,
				   medlongitude,
				   endstatus
				)VALUES(
				  4,
				  '".substr($dadosPreObra[0]['precep'],0,8)."',
				  '".substr($dadosPreObra[0]['prelogradouro'],0,300)."',
				  '".substr($dadosPreObra[0]['precomplemento'],0,300)."',
				  '".substr($dadosPreObra[0]['prebairro'],0,100)."',
				  '".$dadosPreObra[0]['muncod']."',
				  '".$dadosPreObra[0]['estuf']."',
				  '".substr($dadosPreObra[0]['prenumero'],0,10)."',
				  '".$dadosPreObra[0]['prelatitude']."',
				  '".$dadosPreObra[0]['prelongitude']."',
				  'A' ) RETURNING endid";

	$endid = $db->pegaUm($sql);
	
	/*** Insere a nova obra ***/
	$sql = "INSERT INTO obras2.empreendimento(
		            orgid,
		            empesfera,
		            tpoid,
		            prfid,
		            tobid,
		            tooid,
		            cloid,
		            moeid,
		            entidunidade,
		            empdsc,
		            empvalorprevisto,
		            endid,
		            preid
			) VALUES (
					3,
					'".$dadosPreObra[0]['preesfera']."',
					" . ($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'NULL') . ",
					".$dadosPreObra[0]['programa'].",
					'".$dadosPreObra[0]['tipodeobra']."',
					1,
					'".$dadosPreObra[0]['classificacaoobra']."',
					".$dadosPreObra[0]['modalidadeensino'].",
		            ".$unidade_implantadora.",
		            '".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
		            '".$dadosPreObra[0]['valorobra']."',
		            $endid,
		            '".$preid."') RETURNING empid;";

	$empidBKP = $db->pegaUm( $sql );

	require_once APPRAIZ ."includes/workflow.php";

	$docid = wf_cadastrarDocumento( TPDID_OBJETO, "Obra Reformulada MI - preid $preid" );

	/*** Insere a nova obra ***/
	$sql = "INSERT INTO obras2.obras(
				obrnome,
				entid,
				tooid,
				preid,
				endid,
				tpoid,
				tobid,
				cloid,
				obrvalorprevisto,
				empid,
				docid)
			VALUES('".str_ireplace( "'", "", $dadosPreObra[0]['nome_obra'])."',
					".$unidade_implantadora.",
					1,
					'".$preid."',
					'".$endid."',
					" . ($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'NULL') .",
					'".$dadosPreObra[0]['tipodeobra']."',
					'".$dadosPreObra[0]['classificacaoobra']."',
					'".$dadosPreObra[0]['valorobra']."',
					'" . $empidBKP . "',
					$docid)
			RETURNING obrid";
	$obridBKP = $db->pegaUm($sql);

	$obrid = $dadosPreObra[0]['obrid'];
	$empid = $dadosPreObra[0]['empid'];

	$sql = "UPDATE obras2.empreendimento 	SET empid = 9$empidBKP 	WHERE empid = $empidBKP;
			UPDATE obras2.obras 			SET obrid = 9$obridBKP 	WHERE obrid = $obridBKP;
			UPDATE obras2.empreendimento 	SET empid = $empidBKP 	WHERE empid = $empid;
			UPDATE obras2.obras 			SET obrid = $obridBKP 	WHERE obrid = $obrid;
			INSERT INTO obras2.usuarioresponsabilidade(usucpf, pflcod, empid)
            SELECT usucpf, pflcod, $empid FROM obras2.usuarioresponsabilidade WHERE empid = $empidBKP;";
	$db->executar($sql);
	$db->commit();

	$sql = "UPDATE obras2.empreendimento 	SET empid = $empid 		WHERE empid = 9$empidBKP;
			UPDATE obras2.obras 			SET obrid = $obrid 		WHERE obrid = 9$obridBKP;
			UPDATE obras2.empreendimento 	SET empidpai = $empid 	WHERE empid = $empidBKP;
			UPDATE obras2.obras 			SET obridpai = $obrid 	WHERE obrid = $obridBKP;
			INSERT INTO obras2.usuarioresponsabilidade(usucpf, pflcod, empid)
			SELECT usucpf, pflcod, $empid FROM obras2.usuarioresponsabilidade WHERE empid = $empidBKP;";
	$db->executar($sql);
	$db->commit();
	
	$sql = "SELECT
				aedid
			FROM workflow.acaoestadodoc
			WHERE
				esdiddestino = ".OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO."
				AND esdidorigem = ".OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE;

	$aedid = $db->pegaUm($sql);

	if( $aedid == '' ){
		$sql = "INSERT INTO workflow.acaoestadodoc
					(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
					esdsncomentario, aedvisivel, aedcodicaonegativa)
				VALUES(
					".OBR_ESDID_EM_PLANEJAMENTO_PELO_PROPONENTE.",
					".OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO.", 'Enviar para estado de inicio MI', 'A', 'Enviada para de inicio MI',
					true, false, false )
				RETURNING
					aedid";

		$aedid = $db->pegaUm($sql);
	}

	$teste = wf_alterarEstado( $docid, $aedid, 'Tramitado por criaBKPObraMIObras preid = '.$preid, array( 'docid' => $arObra['docid'] ) );
}

function atualizaObrasReformulacaoMIPAC( $preid ){

	global $db;
	
	if( !verificaAnoMeta( $preid ) ){
		return false;
	}
	
	$sql = "SELECT obrid FROM obras2.obras WHERE preid = $preid AND obridpai IS NULL";
	
	$obrid = $db->pegaUm($sql);

	if( $obrid ){
		criaBKPObraMIObras( $preid );
	}
	return true;
}

function atualizaObrasReformulacaoCancelada( $preid ){

	global $db;

	$sql = "SELECT
				doc.esdid,
				doc.docid
			FROM
				obras2.obras obr
			INNER JOIN workflow.documento doc ON doc.docid = obr.docid
			WHERE
				preid = $preid
				AND obrstatus = 'A'";

	$obra = $db->pegaLinha( $sql );

	if( $obra ){
		extract($obra);
	
		$sql = "SELECT
					aedid
				FROM workflow.acaoestadodoc
				WHERE
					esdiddestino = ".OBR_ESDID_OBRA_CANCELADA."
					AND esdidorigem = $esdid";
	
		$aedid = $db->pegaUm($sql);
	
		if( $aedid == '' ){
			$sql = "INSERT INTO workflow.acaoestadodoc
						(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada,
						esdsncomentario, aedvisivel, aedcodicaonegativa)
					VALUES(
						$esdid, ".OBR_ESDID_OBRA_CANCELADA.",
						'Enviar para obra cancelada', 'A', 'Enviada para obra cancelada',
						true, false, false )
					RETURNING
						aedid";
	
			$aedid = $db->pegaUm($sql);
		}
	
		wf_alterarEstado( $docid, $aedid, 'Tramitado por atualizaObrasReformulacaoCancelada preid = '.$preid,
						  array( 'docid' => $docid ) );	
	}
//	// necessitadepara stoid = esdid
//	$sql = "UPDATE obra s.o brainfraestrutura SET stoid = 10 WHERE preid = $preid";
//	$db->executar( $sql );
//
//	$sql = "UPDATE obras.preobra SET prereformulacao = false WHERE preid = $preid";
//	$db->executar( $sql );
//	return $db->commit();
}

function retornaValorSubacao( $sbaid, $ano ){
	global $db;

			$arrIcoAntigoNovo[$icoid['icoid']] = $icoidNovo;

	$itrid = $db->pegaUm( "SELECT iu.itrid FROM par.subacao s  INNER JOIN par.acao a ON a.aciid = s.aciid INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid WHERE s.sbaid = ".$sbaid );

	$tpcid = $itrid == 1 ? 1 : 3;

	$sql = "SELECT
				CASE WHEN sbacronograma = 1
				THEN ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
								CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(sic.icoquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
								END
							as vlrsubacao
					FROM par.subacaoitenscomposicao sic
					WHERE sic.sbaid = s.sbaid
					AND sic.icoano = sd.sbdano
					GROUP BY sic.sbaid, sic.icovalidatecnico ) as foo )
				ELSE ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
						CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
							THEN -- escolas sem itens
								sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
							ELSE -- escolas com itens
								CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
									sum(coalesce(ssi.seiqtdtecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
								END
							END
						as vlrsubacao
					FROM entidade.entidade t
					inner join entidade.funcaoentidade f on f.entid = t.entid
					left join entidade.entidadedetalhe ed on t.entid = ed.entid
					inner join entidade.endereco d on t.entid = d.entid
					left join territorios.municipio m on m.muncod = d.muncod
					left join par.escolas e on e.entid = t.entid
					INNER JOIN par.subacaoescolas se ON se.escid = e.escid
					INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano
					LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
					WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
					and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 and t.tpcid = {$tpcid}
					GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico ) as foo )
			END AS valorsubacao
		FROM
			par.subacao s
		INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sd.sbdano = {$ano}
		WHERE
			s.sbaid IN ( ".$sbaid." )";

	return $db->pegaUm( $sql );
}

#FUNÇÃO PARA VERIFICAR QUAIS OS MUNICÍPIOS OU ESTADOS ATRIBUIDOS AO USUÁRIO.
#-Como certos perfis pode visualizar todos os municípios e todos os estados independente de estarem atribudos ao usuário. Mas, so podendo editar os atribuidos ao mesmo.
#-Essa função verifica se o minicípio ou estado que o usuário esta trabalhando no momento é também atribuido a ele, par que seja possivél a edição.
#-Sendo possível a edição do formulário (de acordo com as regras estabelecidas).
function verifcaMunEstUsusario($pf){

	$perfil = $pf;

	if($perfil != ''){
		if($_SESSION['par']['muncod']){
			$arrMuncod = pegaMunicipioAssociado($perfil);
			if(!empty($arrMuncod)){
				foreach($arrMuncod as $key_mun => $muncod){
					$arrMun[] = $muncod['muncod'];
				}
				if(in_array($_SESSION['par']['muncod'], $arrMun)){
					$permitido = 'S';
				}else{
					$permitido = 'N';
				}
			}
		}elseif($_SESSION['par']['estuf'] != '' && $_SESSION['par']['muncod'] == ''){
			$arrUf = pegaEstadoAssociado($perfil);
			if(!empty($arrUf)){
				foreach($arrUf as $key_uf => $uf){
					$arrUf[] = $uf['estuf'];
				}
				if(in_array($_SESSION['par']['estuf'], $arrUf)){
					$permitido = 'S';
				}else{
					$permitido = 'N';
				}
			}
		}else{
			$permitido = 'N';
		}
	}
	return $permitido;
}

function cancelaReformulacao($sbaid, $ano, $motivo = null){

	global $db;
	
	$sbdid = $db->pegaUm("SELECT sbdid FROM par.subacaodetalhe WHERE sbaid = ".$sbaid." AND sbdano = ".$ano);
	
	$dado = deletaReformulacao( $sbaid, $ano );
	
	if( !$dado ){
		die();
	}
	
	$sql = "SELECT dopid FROM par.documentoparreprogramacaosubacao WHERE dpsstatus = 'A' AND sbdid = ".$sbdid;
	$dopid = $db->pegaUm($sql);
	
	if( $motivo != '' ){
		$sql = "UPDATE par.documentoparreprogramacaosubacao SET dpsstatus = 'I', dpsjustificativacancelamento = '".substr($motivo, 0, 1200)."' WHERE dpsstatus = 'A' AND sbdid = ".$sbdid;
		$db->executar($sql);
	}
	
	if( $dopid ){
		$sql = "SELECT TRUE FROM par.reprogramacao WHERE repstatus = 'A' AND dopidoriginal = $dopid";
		$reformulacaoPendente = $db->pegaUm($sql);
	}

	if( $reformulacaoPendente != 't' && $dopid ){
		$sql = "UPDATE par.reprogramacao SET repstatus = 'I' WHERE dopidoriginal = $dopid";
		$db->executar($sql);
	}
	$db->commit();
	
	return true;
}
	
function deletaReformulacao($sbaid, $ano){
	
	global $db;

	//Recupero os dados da cópia
	$sbaidCopia = $db->pegaUm("SELECT MAX(sba.sbaid) FROM par.subacao sba INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sba.sbaid AND sd.sbdano = ".$ano." WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = ".$sbaid);

// 	//Deleto os dados da Reformulação
// 	$db->executar("DELETE FROM par.subescolas_subitenscomposicao 	WHERE icoid IN ( SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} and icoano = '{$ano}' )");
// 	$db->executar("DELETE FROM par.subescolas_subitenscomposicao 	WHERE sesid IN ( SELECT sesid FROM par.subacaoescolas WHERE sbaid = {$sbaid} and sesano = '{$ano}' )");
// 	$db->executar("DELETE FROM par.subacaoitenscomposicao 			WHERE sbaid = {$sbaid} and icoano = '{$ano}'");
// 	$db->executar("DELETE FROM par.subacaoescolas 					WHERE sbaid = {$sbaid} and sesano = '{$ano}'");

// 	//Atualizo os dados no subacaodetalhe
// 	$sql = "UPDATE par.subacaodetalhe s
// 			   SET sbdparecer=foo.sbdparecer, sbdquantidade=foo.sbdquantidade, sbdano=foo.sbdano, sbdinicio=foo.sbdinicio,
// 			       sbdfim=foo.sbdfim, ssuid=foo.ssuid, sbdanotermino=foo.sbdanotermino, sbdnaturezadespesa=foo.sbdnaturezadespesa, sbddetalhamento=foo.sbddetalhamento,
// 			       prpid=foo.prpid, sbdplanointerno=foo.sbdplanointerno, sbdparecerdemerito=foo.sbdparecerdemerito, sbdplicod=foo.sbdplicod,
// 			       sbdptres=foo.sbdptres, sbdrepassecomplementar=foo.sbdrepassecomplementar, sbdrepasseempenho=foo.sbdrepasseempenho, sbdrepasseraf=foo.sbdrepasseraf,
// 			       sbdrepassevlrcomplementar=foo.sbdrepassevlrcomplementar, sbdrepassevlrempenho=foo.sbdrepassevlrempenho, sbdrepassevlrraf=foo.sbdrepassevlrraf,
// 			       sbdrepassecomplementaraprovado=foo.sbdrepassecomplementaraprovado, sbdrepasseempenhoaprovado=foo.sbdrepasseempenhoaprovado,
// 			       sbdrepasserafaprovado=foo.sbdrepasserafaprovado, sbdrepassevlrcomplementaraprovado=foo.sbdrepassevlrcomplementaraprovado,
// 			       sbdrepassevlrempenhoaprovado=foo.sbdrepassevlrempenhoaprovado, sbdrepassevlrrafaprovado=foo.sbdrepassevlrrafaprovado, sbdreforco=foo.sbdreforco
// 			FROM (
// 				SELECT * FROM par.subacaodetalhe sd WHERE sd.sbaid = {$sbaidCopia} AND sd.sbdano = {$ano} ) foo
// 			WHERE s.sbaid = {$sbaid} AND s.sbdano = {$ano}";
// 	$db->executar( $sql );


// 	//Insiro os dados das Obras
// 	$sql = "INSERT INTO par.subacaoobra (sbaid, preid, sobano)
// 			( SELECT {$sbaid}, preid, sobano
// 			FROM par.subacaoobra WHERE sbaid = {$sbaidCopia} AND sobano = {$ano})";
// 	$db->executar( $sql );


// 	//Seleciono os Itens de Composição
// 	$sqlItensAntigos = "SELECT
// 							icoid
// 						FROM
// 							par.subacaoitenscomposicao
// 						WHERE
// 							sbaid = ".$sbaidCopia." AND icoano = ".$ano;
// 	$arrIcoid = $db->carregar( $sqlItensAntigos );

// 	$arrIcoAntigoNovo = array();
// 	if( is_array( $arrIcoid ) ){
// 		foreach( $arrIcoid as $icoid ){
// 			//Insiro os Itens de Composição
// 			$sqlItens = "INSERT INTO par.subacaoitenscomposicao
// 							(sbaid, icoano, icoordem, icodescricao, icoquantidade, icovalor,
// 							icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid,
// 							icoquantidadetecnico, icovalidatecnico, dicid )
// 							SELECT
// 								{$sbaid}, icoano, icoordem, icodescricao, icoquantidade, icovalor,
// 								icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid,
// 								icoquantidadetecnico, icovalidatecnico, dicid
// 							FROM
// 								par.subacaoitenscomposicao
// 							WHERE
// 								icoid = ".$icoid['icoid']." AND icoano = ".$ano."
// 							RETURNING
// 								icoid";
// 			$icoidNovo = $db->pegaUm($sqlItens);

// 			$arrIcoAntigoNovo[$icoid['icoid']] = $icoidNovo;

// 		}
// 	}

// 	//Seleciono as Escolas
// 	$sqlEscolasAntigas = "SELECT
// 							sesid
// 						FROM
// 							par.subacaoescolas
// 						WHERE
// 							sbaid=".$sbaidCopia." AND sesano = ".$ano;
// 	$arrSesid = $db->carregar( $sqlEscolasAntigas );

// 	$arrSesAntigoNovo = array();
// 	if( is_array( $arrSesid ) ){
// 		foreach( $arrSesid as $sesid ){
// 			// Insiro as Escolas
// 			$sqlEscolas = "INSERT INTO par.subacaoescolas
// 							( sbaid, sesano, escid, sesquantidade, sesstatus, sesquantidadetecnico, sesvalidatecnico )
// 							SELECT
// 								{$sbaid}, sesano, escid, sesquantidade, sesstatus, sesquantidadetecnico, sesvalidatecnico
// 							FROM
// 								par.subacaoescolas
// 							WHERE
// 								sesid = {$sesid['sesid']} AND sesano = {$ano}
// 							RETURNING
// 								sesid";
// 			$sesidNovo = $db->pegaUm($sqlEscolas);

// 			// Pego todos os itens relacionados a escola antiga
// 			$sqlEscIt = "SELECT
// 							icoid
// 						FROM
// 							par.subescolas_subitenscomposicao
// 						WHERE
// 							sesid = ".$sesid['sesid'];
// 			$itensVelhos = $db->carregar( $sqlEscIt );

// 			if( is_array( $itensVelhos ) ){
// 				foreach( $itensVelhos as $i => $it ){
// 					if( $arrIcoAntigoNovo[$it['icoid']] ){
// 						//Insiro os Itens por Escola
// 						$sqlSubEsc = "INSERT INTO par.subescolas_subitenscomposicao
// 									( sesid, icoid, seiqtd, seiqtdtecnico )
// 									SELECT
// 										$sesidNovo, {$arrIcoAntigoNovo[$it['icoid']]}, seiqtd, seiqtdtecnico
// 									FROM
// 										par.subescolas_subitenscomposicao
// 									WHERE
// 										sesid = ".$sesid['sesid']." AND icoid = ".$it['icoid'];
// 						$db->carregar($sqlSubEsc);
// 					}
// 				}
// 			}
// 		}
// 	}

	if( $sbaidCopia != '' ){
		
		//Recupera Backup
		$sql = "SELECT par.recuperabkpsubacaodetalhe($sbaid, $sbaidCopia, $ano)";
		$atualizou = $db->pegaUm( $sql );
	
		if( $atualizou == 1 ){
			//Atualizo o status da Subação
			$db->executar("UPDATE par.subacao SET sbareformulacao = FALSE WHERE sbaid = {$sbaid}");
			$db->executar("UPDATE par.subacao SET sbastatus = 'I' WHERE sbaid = {$sbaidCopia}");
		
			//Verifico se tem algum outro ano preenchido com essa cópia
			$verificacao = $db->pegaUm("SELECT sbaid FROM par.subacaodetalhe WHERE sbaid = ".$sbaidCopia);
		
			$db->commit();
			
			$sql = "SELECT MAX(sba.sbaid) FROM par.subacao sba INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sba.sbaid AND sd.sbdano = ".$ano." WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = ".$sbaid;
			
			$foiReformulada = $db->pegaUm($sql);
			
			if( !$foiReformulada ){
				
				$sql = "UPDATE par.subacaodetalhe SET sbdreformulacao = NULL WHERE sbaid = $sbaid";
				$db->executar($sql);
			}
			return $db->commit();
		}
	}else{
		echo "
				<script>
					alert('Esta reformulação não pôde ser cancelada por apresentar problemas em seu histórico.');
				</script>";
	}
	return false;
}

function alterarDadosObrasReformulada( $preid ){
	global $db;
	/*** INICIO - Importação dos dados da obrasPar para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra exista ***/
	$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid_1 is not null";
	$existeObra = $db->pegaUm($sql);

	if( (integer)$existeObra <> 0 ){
		/*** Recupera dados da Pre Obra ***/

		$sql = "SELECT DISTINCT
					CASE WHEN iu.itrid = 1 THEN 'E' ELSE 'M' END as esfera,
					po.predescricao as nome_obra,
					pto.tpoid, -- TIPOLOGIA DE OBRA
					/*CASE
						WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
						WHEN pop.sisid = 23 THEN 39 --PAR
						ELSE 42 --Emenda Parlamentar
					END AS programa,*/
					CASE
						WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
						WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
						WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
						ELSE 54 --OUTROS
	                END as programa,
					CASE
						WHEN i.indcod in (3,4,7,8) THEN 1
						WHEN i.indcod in (5,6,10,9) THEN 2
					END AS modalidadedeensino,
					CASE
						WHEN pto.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
						WHEN pto.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
						ELSE 1 --CONSTRUÇÃO
					END AS tipodeobra,
					CASE
						WHEN sisid = 23	THEN 11 -- TD
						WHEN sisid = 57	THEN 4 -- EMENDAS
						ELSE 11 -- TD
					END AS fonte,
					CASE
						WHEN s.ppsid in (652,695,810,896,897,965,966,971,972,977,983,987,989,1015,1016,1091,1093,1099,1104,1105,1115,1116,1119,1120,1122,1124) THEN 1 --RURAL
						WHEN s.ppsid in (494,495,521,555,568,577,624,633,671,676,698,718,783,802,867,882,957,958,961,962,963,964,981,982,1013,1014,1088,1089,1090,1098,1111,1112,1117,1118,1121,1123,1153,1154,1158,1159) THEN 2 --URBANA
						WHEN s.ppsid in (605,655,854,900,901,969,970,975,976,980,986,988,990,1094) THEN 3 --QUILOMBOLA
						WHEN s.ppsid in (542,710,768,801,898,899,967,968,973,974,978,979,984,985) THEN 4 --INDÍGENA
					ELSE 2 --URBANA
					END AS classificacaoobra,
					prevalorobra, -- VALOR PREVISTO
					mun.mundescricao,
					po.precep,
					po.prelogradouro,
					po.precomplemento,
					substring(po.prebairro, 0, 100) as prebairro,
					po.muncod,
					po.estuf,
					po.prenumero,
					po.prelatitude,
					po.prelongitude,
					oi.entid,
					oi.obrid,
					oi.endid,
					po.ptoid
				FROM
					par.dimensao d
				INNER JOIN par.area                   		a 	 ON a.dimid = d.dimid
				INNER JOIN par.indicador           			i 	 ON i.areid = a.areid
				INNER JOIN par.criterio               		c 	 ON c.indid = i.indid
				INNER JOIN par.pontuacao 					p    ON c.crtid = p.crtid AND ptostatus = 'A'
				INNER JOIN par.instrumentounidade 			iu   ON iu.inuid = p.inuid --  iu.inuid = pop.inuid
				INNER JOIN par.acao                   		ac   ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
				INNER JOIN par.subacao                		s 	 ON s.aciid = ac.aciid AND s.sbastatus = 'A'
				INNER JOIN par.subacaoobra    				so   ON so.sbaid = s.sbaid
				INNER JOIN obras.preobra         			po   ON so.preid = po.preid
				INNER JOIN territorios.municipio 			mun  ON po.muncod = mun.muncod
				INNER JOIN entidade.endereco 				ende ON ende.muncod = po.muncod
				INNER JOIN entidade.entidade 				ent  ON ent.entid = ende.entid AND ent.entstatus = 'A'
				INNER JOIN entidade.funcaoentidade 			fen  ON ent.entid = fen.entid AND fen.funid IN (1)
				INNER JOIN par.processoobraspar 			pop  ON pop.inuid = iu.inuid and pop.prostatus = 'A'
				INNER JOIN par.processoobrasparcomposicao 	poc  ON poc.proid = pop.proid and poc.preid = po.preid and poc.pocstatus = 'A'
				INNER JOIN par.empenho                 		e    ON pop.pronumeroprocesso =  e.empnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhoobrapar          		eop  ON e.empid = eop.empid AND eop.preid = po.preid and eobstatus = 'A'
				LEFT  JOIN obras2.obras						oi   ON oi.preid = po.preid
				INNER JOIN obras.pretipoobra 				pto  ON pto.ptoid = po.ptoid
				WHERE
					po.preid = $preid
					AND d.dimcod = 4
					AND a.arecod = 2";

		$dadosPreObra = $db->carregar($sql);

		if( $dadosPreObra[0]['endid'] ){
			$sql = "UPDATE entidade.endereco SET
						endcep = '".$dadosPreObra[0]['precep']."',
  						endlog = '".$dadosPreObra[0]['prelogradouro']."',
					  	endcom = '".$dadosPreObra[0]['precomplemento']."',
					  	endbai = '".$dadosPreObra[0]['prebairro']."',
					  	muncod = '".$dadosPreObra[0]['muncod']."',
					  	estuf = '".$dadosPreObra[0]['estuf']."',
					  	endnum = '".$dadosPreObra[0]['prenumero']."',
  						endstatus = 'A',
  						medlatitude = '".$dadosPreObra[0]['prelatitude']."',
  						medlongitude = '".$dadosPreObra[0]['prelongitude']."'
					WHERE
  						endid = ".$dadosPreObra[0]['endid'];

			$db->executar( $sql );
			$endid = $dadosPreObra[0]['endid'];
		} else {
			/*** Insere novo endereço da obra ***/
			$sql = "INSERT INTO
						entidade.endereco (endcep,
										   endlog,
										   endcom,
										   endbai,
										   muncod,
										   estuf,
										   endnum,
										   medlatitude,
										   medlongitude,
										   endstatus)

					VALUES
						( '".$dadosPreObra[0]['precep']."',
						  '".$dadosPreObra[0]['prelogradouro']."',
						  '".$dadosPreObra[0]['precomplemento']."',
						  '".$dadosPreObra[0]['prebairro']."',
						  '".$dadosPreObra[0]['muncod']."',
						  '".$dadosPreObra[0]['estuf']."',
						  '".$dadosPreObra[0]['prenumero']."',
						  '".$dadosPreObra[0]['prelatitude']."',
						  '".$dadosPreObra[0]['prelongitude']."',
						  'A' ) RETURNING endid";

			$endid = $db->pegaUm($sql);
		}

		/*** Atualiza a nova obra ***/
		if( $dadosPreObra[0]['obrid'] ){

			$sql = "UPDATE obras2.obras SET
						tobid = ".($dadosPreObra[0]['tipoobra'] ? $dadosPreObra[0]['tipoobra'] : 'null').",
						endid = $endid,
						tooid = {$dadosPreObra[0]['fonte']},
						cloid = {$dadosPreObra[0]['classificacaoobra']},
						obrvalorprevisto = {$dadosPreObra[0]['prevalorobra']},
						entid = ".($dadosPreObra[0]['entid'] ? $dadosPreObra[0]['entid'] : 'null').",
						obrnome = '{$dadosPreObra[0]['nome_obra']}',
						tpoid = ".($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'null')."
					WHERE
						obrid = {$dadosPreObra[0]['obrid']}
					RETURNING
						empid";

			$empid = $db->pegaUm( $sql );

			$sql = "UPDATE obras2.empreendimento SET
						tobid = ".($dadosPreObra[0]['tipoobra'] ? $dadosPreObra[0]['tipoobra'] : 'null').",
						endid = $endid,
						prfid = {$dadosPreObra[0]['programa']},
						tooid = {$dadosPreObra[0]['fonte']},
						cloid = {$dadosPreObra[0]['classificacaoobra']},
						empvalorprevisto = ".($dadosPreObra[0]['prevalorobra'] ? $dadosPreObra[0]['prevalorobra'] : 'null').",
						moeid = ".($dadosPreObra[0]['modalidadeensino'] ? $dadosPreObra[0]['modalidadeensino'] : 'null').",
						entidunidade = ".($dadosPreObra[0]['entid'] ? $dadosPreObra[0]['entid'] : 'null').",
						empdsc = '{$dadosPreObra[0]['nome_obra']}',
						tpoid = ".($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'null').",
						empesfera = '{$dadosPreObra[0]['esfera']}'
					WHERE
						empid = $empid";

			$db->executar( $sql );

			/*$sql = "DELETE FROM obras.arquivosobra WHERE obrid = ".$dadosPreObra[0]['obrid']." and tpaid = 21";
			$db->executar( $sql );*/

			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						public.arquivo arq
					INNER JOIN
						obras.preobrafotos pof ON arq.arqid = pof.arqid
					INNER JOIN
						obras.preobra pre ON pre.preid = pof.preid
					WHERE
						pre.preid = ".$preid."
					AND
						(substring(arqtipo,1,5) = 'image')";
			$fotosTerreno = $db->carregar($sql);
			//ver($sql, $sql,d);
			if( $fotosTerreno ){
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto){
					$boArquivo = $db->pegaUm("SELECT COUNT(aqoid) FROM obras2.arquivosobra WHERE tpaid = 21 AND obrid = {$dadosPreObra[0]['obrid']} AND arqid = {$foto['arqid']} AND aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO
									obras2.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
								VALUES
									(".$dadosPreObra[0]['obrid'].", 21, ".$foto['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
						$db->executar($sql);
					}
				}
			}

			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						obras.preobraanexo p
					INNER JOIN
						public.arquivo arq ON arq.arqid = p.arqid
					WHERE
						p.preid = ".$preid;
			$anexos = $db->carregar($sql);

			if( $anexos )
			{
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
					$boArquivo = $db->pegaUm("SELECT COUNT(aqoid) FROM obras2.arquivosobra WHERE tpaid = 21 AND obrid = {$dadosPreObra[0]['obrid']} AND arqid = {$anexo['arqid']} AND aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO obras2.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
								VALUES (".$dadosPreObra[0]['obrid'].", 21, ".$anexo['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
						$db->executar($sql);
					}
				}
			}

			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid_1 = ".$dadosPreObra[0]['obrid']." WHERE preid = ".$preid;
			$db->executar($sql);
		}
	}
	return $db->commit();
	/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
}

function alterarDadosObras2Reformulada( $preid ){
	global $db;
	/*** INICIO - Importação dos dados da obrasPar para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra exista ***/
	$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid IS NOT NULL";
	$existeObra = $db->pegaUm($sql);

	if( (integer)$existeObra <> 0 ){
		/*** Recupera dados da Pre Obra ***/
		$sql = "SELECT DISTINCT
				CASE WHEN iu.itrid = 1 THEN 'E' ELSE 'M' END as esfera,
				po.predescricao as nome_obra,
                pto.tpoid, -- TIPOLOGIA DE OBRA
                /*CASE
					WHEN pop.sisid = 23 THEN 39 --PAR
					ELSE 42 --Emenda Parlamentar
				END AS programa,*/
				CASE
					WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
					WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
					WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
					ELSE 54 --OUTROS
                END as programa,
				CASE
					WHEN i.indcod in (3,4,7,8) THEN 1
					WHEN i.indcod in (5,6,10,9) THEN 2
				END AS modalidadedeensino,
				CASE
					WHEN pto.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
					WHEN pto.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
					ELSE 1 --CONSTRUÇÃO
				END AS tipodeobra,
				CASE
					WHEN sisid = 23 THEN 11 -- TD
					WHEN sisid = 57 THEN 4 -- EMENDAS
					ELSE 11 -- TD
				END AS fonte,
				CASE
					WHEN s.ppsid in (652,695,810,896,897,965,966,971,972,977,983,987,989,1015,1016,1091,1093,1099,1104,1105,1115,1116,1119,1120,1122,1124) THEN 1 --RURAL
					WHEN s.ppsid in (494,495,521,555,568,577,624,633,671,676,698,718,783,802,867,882,957,958,961,962,963,964,981,982,1013,1014,1088,1089,1090,1098,1111,1112,1117,1118,1121,1123,1153,1154,1158,1159) THEN 2 --URBANA
					WHEN s.ppsid in (605,655,854,900,901,969,970,975,976,980,986,988,990,1094) THEN 3 --QUILOMBOLA
					WHEN s.ppsid in (542,710,768,801,898,899,967,968,973,974,978,979,984,985) THEN 4 --INDÍGENA
				ELSE 2 --URBANA
				END AS classificacaoobra,
                prevalorobra, -- VALOR PREVISTO
                mun.mundescricao,
                po.precep,
                po.prelogradouro,
                po.precomplemento,
                po.prebairro,
                po.muncod,
                po.estuf,
                po.prenumero,
                po.prelatitude,
                po.prelongitude,
                oi.obrid,
                oi.empid,
                oi.endid,
                po.ptoid
			FROM par.dimensao d
			INNER JOIN par.area                  		a   ON a.dimid = d.dimid
			INNER JOIN par.indicador           			i   ON i.areid = a.areid
			INNER JOIN par.criterio               		c   ON c.indid = i.indid
			INNER JOIN par.pontuacao 					p   ON c.crtid = p.crtid AND ptostatus = 'A'
			INNER JOIN par.instrumentounidade 			iu  ON iu.inuid = p.inuid --  iu.inuid = pop.inuid
			INNER JOIN par.acao                   		ac  ON ac.ptoid = p.ptoid AND ac.acistatus = 'A'
			INNER JOIN par.subacao                		s   ON s.aciid = ac.aciid AND s.sbastatus = 'A'
			INNER JOIN par.subacaoobra    				so  ON so.sbaid = s.sbaid
			INNER JOIN obras.preobra         			po  ON so.preid = po.preid
			INNER JOIN territorios.municipio 			mun ON po.muncod = mun.muncod
			INNER JOIN entidade.endereco 				ende ON ende.muncod = po.muncod
			INNER JOIN entidade.entidade 				ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
			INNER JOIN entidade.funcaoentidade 			fen ON ent.entid = fen.entid AND fen.funid IN (1)
			INNER JOIN par.processoobraspar 			pop ON pop.inuid = iu.inuid and pop.prostatus = 'A'
			INNER JOIN par.processoobrasparcomposicao 	poc ON poc.proid = pop.proid and poc.preid = po.preid and poc.pocstatus = 'A'
			INNER JOIN par.empenho                 		e   ON pop.pronumeroprocesso =  e.empnumeroprocesso and empstatus <> 'I'
			INNER JOIN par.empenhoobrapar          		eop ON e.empid = eop.empid AND eop.preid = po.preid and eobstatus = 'A'
			LEFT  JOIN obras2.obras						oi  ON oi.preid = po.preid
			INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = po.ptoid
			WHERE
				po.preid = $preid
                and d.dimcod = 4
              	AND a.arecod = 2";
		$dadosPreObra = $db->carregar($sql);

		//ATUALIZA O ENDEREÇO
		if( $dadosPreObra[0]['endid'] ){
			$sql = "UPDATE entidade.endereco SET
						endcep = '".$dadosPreObra[0]['precep']."',
  						endlog = '".$dadosPreObra[0]['prelogradouro']."',
					  	endcom = '".$dadosPreObra[0]['precomplemento']."',
					  	endbai = '".$dadosPreObra[0]['prebairro']."',
					  	muncod = '".$dadosPreObra[0]['muncod']."',
					  	estuf = '".$dadosPreObra[0]['estuf']."',
					  	endnum = '".$dadosPreObra[0]['prenumero']."',
  						endstatus = 'A',
  						medlatitude = '".$dadosPreObra[0]['prelatitude']."',
  						medlongitude = '".$dadosPreObra[0]['prelongitude']."'
					WHERE
  						endid = ".$dadosPreObra[0]['endid'];

			$db->executar( $sql );
			$endid = $dadosPreObra[0]['endid'];
		} else {
			/*** Insere novo endereço da obra ***/
			$sql = "INSERT INTO
						entidade.endereco (
										   tpeid,
										   endcep,
										   endlog,
										   endcom,
										   endbai,
										   muncod,
										   estuf,
										   endnum,
										   medlatitude,
										   medlongitude,
										   endstatus)

					VALUES
						( 4,
						  '".$dadosPreObra[0]['precep']."',
						  '".$dadosPreObra[0]['prelogradouro']."',
						  '".$dadosPreObra[0]['precomplemento']."',
						  '".$dadosPreObra[0]['prebairro']."',
						  '".$dadosPreObra[0]['muncod']."',
						  '".$dadosPreObra[0]['estuf']."',
						  '".$dadosPreObra[0]['prenumero']."',
						  '".$dadosPreObra[0]['prelatitude']."',
						  '".$dadosPreObra[0]['prelongitude']."',
						  'A' ) RETURNING endid";

			$endid = $db->pegaUm($sql);
		}

		// ATUALIZA EMPREENDIMENTO
		if ( $dadosPreObra[0]['empid'] ){
			$sql = "UPDATE obras2.empreendimento
					   SET
					   		endid = {$endid},
					   		tobid = {$dadosPreObra[0]['tipoobra']},
					   		prfid = {$dadosPreObra[0]['programa']},
					   		tooid = {$dadosPreObra[0]['fonte']},
					   		cloid = {$dadosPreObra[0]['classificacaoobra']},
					   		empvalorprevisto = {$dadosPreObra[0]['prevalorobra']},
					   		moeid = ".($dadosPreObra[0]['modalidadeensino'] ? $dadosPreObra[0]['modalidadeensino'] : 'null').",
					   		empdsc='{$dadosPreObra[0]['nome_obra']}',
					   		tpoid = ".($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'null').",
					   		empesfera='{$dadosPreObra[0]['esfera']}'
					WHERE
							empid = {$dadosPreObra['empid']}";
		}

		/*** Atualiza a nova obra ***/
		if( $dadosPreObra[0]['obrid'] ){
			$sql = "UPDATE obras2.obras
					   SET 	endid={$endid},
					   		tobid=" . ($dadosPreObra[0]['tipoobra'] ? $dadosPreObra[0]['tipoobra'] : 'NULL') . ",
					   		tpoid=" . ($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'NULL') . ",
					       	cloid={$dadosPreObra[0]['classificacaoobra']},
					       	tooid={$dadosPreObra[0]['fonte']},
					       	obrnome='{$dadosPreObra[0]['nome_obra']}',
					       	obrvalorprevisto=".($dadosPreObra[0]['prevalorobra'] ? $dadosPreObra[0]['prevalorobra'] : 'null')."
					 WHERE
					 		obrid = ".$dadosPreObra[0]['obrid'];
			$db->executar( $sql );

			/*$sql = "DELETE FROM obras.arquivosobra WHERE obrid = ".$dadosPreObra[0]['obrid']." and tpaid = 21";
			$db->executar( $sql );*/

			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						public.arquivo arq
					INNER JOIN
						obras.preobrafotos pof ON arq.arqid = pof.arqid
					INNER JOIN
						obras.preobra pre ON pre.preid = pof.preid
					WHERE
						pre.preid = ".$preid."
					AND
						(substring(arqtipo,1,5) = 'image')";
			$fotosTerreno = $db->carregar($sql);
			//ver($sql, $sql,d);
			if( $fotosTerreno ){
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto){
					$boArquivo = $db->pegaUm("select count(aqoid) from obras.arquivosobra where tpaid = 21 and obrid = {$dadosPreObra[0]['obrid']} and arqid = {$foto['arqid']} and aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO obras2.obras_arquivos(
						            obrid,
						            tpaid,
						            arqid,
						            oardata,
						            oardtinclusao
								)VALUES (
									". $dadosPreObra[0]['obrid'] .",
									21,
									".$foto['arqid'].",
									NOW(),
									NOW()
								);";

						$db->executar($sql);
					}
				}
			}

			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						obras.preobraanexo p
					INNER JOIN
						public.arquivo arq ON arq.arqid = p.arqid
					WHERE
						p.preid = ".$preid;
			$anexos = $db->carregar($sql);

			if( $anexos )
			{
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
					$boArquivo = $db->pegaUm("select count(aqoid) from obras.arquivosobra where tpaid = 21 and obrid = {$dadosPreObra[0]['obrid']} and arqid = {$anexo['arqid']} and aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO obras2.obras_arquivos(
						            obrid,
						            tpaid,
						            arqid,
						            oardata,
						            oardtinclusao
								)VALUES (
									". $dadosPreObra[0]['obrid'] .",
									21,
									".$anexo['arqid'].",
									NOW(),
									NOW()
								);";

						$db->executar($sql);
					}
				}
			}

			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid = ".$dadosPreObra[0]['obrid']." WHERE preid = ".$preid;
			$db->executar($sql);
		}
	}
	return $db->commit();
	/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
}

function alterarDadosObrasReformuladaPAC( $preid ){
	global $db;
	/*** INICIO - Importação dos dados da obrasPar para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra exista ***/
	$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid_1 is not null";
	$existeObra = $db->pegaUm($sql);

	if( (integer)$existeObra <> 0 ){
		/*** Recupera dados da Pre Obra ***/

		$sql = "SELECT DISTINCT
					po.preesfera as esfera,
					po.predescricao as nome_obra,
					pto.tpoid, -- TIPOLOGIA DE OBRA
					CASE
								WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
								WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
								WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
								ELSE 54 --OUTROS
					END as programa,
					CASE WHEN pto.ptoclassificacaoobra = 'Q' THEN 3  ELSE 1 END as modalidadeensino, -- MODALIDADE DE ENSINO
					CASE
						WHEN pto.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
						WHEN pto.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
						ELSE 1 --CONSTRUÇÃO
					END AS tipodeobra,
					1 as fonte, -- FONTE PAC
					CASE
						WHEN REPLACE(UPPER(po.predescricao), 'Í', 'I') ILIKE '%INDIGENA%' THEN 4 -- INDÍGENA
						WHEN UPPER(po.predescricao) ILIKE '%RURAL%' THEN 1 -- RURAL
						WHEN UPPER(po.predescricao) ILIKE '%QUILOMBO%' THEN 3 -- QUILOMBO
						ELSE 2 --URBANO
					END AS classificacaoobra, -- CLASSIFICAÇÃO DA OBRA
					prevalorobra, -- VALOR PREVISTO
					mun.mundescricao,
					po.precep,
					po.prelogradouro,
					po.precomplemento,
					po.prebairro,
					po.muncod,
					po.estuf,
					po.prenumero,
					po.prelatitude,
					po.prelongitude,
					oi.entid as entidunidade,
					oi.obrid,
					oi.endid,
					oi.empid,
					po.ptoid
				FROM
					obras.preobra po
				INNER JOIN territorios.municipio 			mun on po.muncod = mun.muncod
				INNER JOIN par.processoobraspaccomposicao  	poc ON poc.preid = po.preid and poc.pocstatus = 'A'
				INNER JOIN par.processoobra 				pop ON pop.proid = poc.proid and pop.prostatus = 'A' 
				INNER JOIN par.empenho                 		e   ON pop.pronumeroprocesso =  e.empnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhoobra          		eop ON e.empid = eop.empid AND eop.preid = po.preid and eobstatus = 'A'
				LEFT  JOIN obras2.obras						oi ON oi.preid = po.preid
				INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = po.ptoid
				WHERE
					po.preid = $preid";

		$dadosPreObra = $db->carregar($sql);

		/*** Atualiza a nova obra ***/
		if( $dadosPreObra[0]['obrid'] ){
	
			if( $dadosPreObra[0]['endid'] ){
				$sql = "UPDATE entidade.endereco SET
							endcep = '".$dadosPreObra[0]['precep']."',
	  						endlog = '".$dadosPreObra[0]['prelogradouro']."',
						  	endcom = '".$dadosPreObra[0]['precomplemento']."',
						  	endbai = '".$dadosPreObra[0]['prebairro']."',
						  	muncod = '".$dadosPreObra[0]['muncod']."',
						  	estuf = '".$dadosPreObra[0]['estuf']."',
						  	endnum = '".$dadosPreObra[0]['prenumero']."',
	  						endstatus = 'A',
	  						medlatitude = '".$dadosPreObra[0]['prelatitude']."',
	  						medlongitude = '".$dadosPreObra[0]['prelongitude']."'
						WHERE
	  						endid = ".$dadosPreObra[0]['endid'];
	
				$db->executar( $sql );
				$endid = $dadosPreObra[0]['endid'];
			} else {
				/*** Insere novo endereço da obra ***/
				$sql = "INSERT INTO
							entidade.endereco (endcep,
											   endlog,
											   endcom,
											   endbai,
											   muncod,
											   estuf,
											   endnum,
											   medlatitude,
											   medlongitude,
											   endstatus)
	
						VALUES
							( '".$dadosPreObra[0]['precep']."',
							  '".$dadosPreObra[0]['prelogradouro']."',
							  '".$dadosPreObra[0]['precomplemento']."',
							  '".$dadosPreObra[0]['prebairro']."',
							  '".$dadosPreObra[0]['muncod']."',
							  '".$dadosPreObra[0]['estuf']."',
							  '".$dadosPreObra[0]['prenumero']."',
							  '".$dadosPreObra[0]['prelatitude']."',
							  '".$dadosPreObra[0]['prelongitude']."',
							  'A' ) RETURNING endid";
	
				$endid = $db->pegaUm($sql);
			}

			$sql = "UPDATE obras2.empreendimento SET
						tobid = {$dadosPreObra[0]['tipodeobra']},
						endid = $endid,
						prfid = {$dadosPreObra[0]['programa']},
						tooid = {$dadosPreObra[0]['fonte']},
						cloid = {$dadosPreObra[0]['classificacaoobra']},
						empvalorprevisto = ".($dadosPreObra[0]['prevalorobra'] ? $dadosPreObra[0]['prevalorobra'] : 'null').",
						moeid = {$dadosPreObra[0]['modalidadeensino']},
						entidunidade = {$dadosPreObra[0]['entidunidade']},
						empdsc = '{$dadosPreObra[0]['nome_obra']}',
						tpoid = ".($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'null').",
						empesfera = '{$dadosPreObra[0]['esfera']}'
					WHERE
						empid = {$dadosPreObra[0]['empid']}
					RETURNING
						empid;";

			$empid = $db->pegaUm($sql);

			$sql = "UPDATE obras2.obras SET
						empid = $empid,
						tobid = {$dadosPreObra[0]['tipodeobra']},
						endid = $endid,
						tooid = {$dadosPreObra[0]['fonte']},
						cloid = {$dadosPreObra[0]['classificacaoobra']},
						obrvalorprevisto = ".($dadosPreObra[0]['prevalorobra'] ? $dadosPreObra[0]['prevalorobra'] : 'null').",
						entid = ".($dadosPreObra[0]['entidunidade'] ? $dadosPreObra[0]['entidunidade'] : 'null').",
						obrnome = '{$dadosPreObra[0]['nome_obra']}',
						tpoid = ".($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'null')."
					WHERE
						obrid = {$dadosPreObra[0]['obrid']}";

			$db->executar( $sql );

			/*$sql = "DELETE FROM obras.arquivosobra WHERE obrid = ".$dadosPreObra[0]['obrid']." and tpaid = 21";
			$db->executar( $sql );*/

			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						public.arquivo arq
					INNER JOIN
						obras.preobrafotos pof ON arq.arqid = pof.arqid
					INNER JOIN
						obras.preobra pre ON pre.preid = pof.preid
					WHERE
						pre.preid = ".$preid."
					AND
						(substring(arqtipo,1,5) = 'image')";
			$fotosTerreno = $db->carregar($sql);
			//ver($sql, $sql,d);
			if( $fotosTerreno ){
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto){
					$boArquivo = $db->pegaUm("select count(aqoid) from obras.arquivosobra where tpaid = 21 and obrid = {$dadosPreObra[0]['obrid']} and arqid = {$foto['arqid']} and aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO
								obras2.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
								VALUES
								(".$dadosPreObra[0]['obrid'].", 21, ".$foto['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
						$db->executar($sql);
					}
				}
			}

			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						obras.preobraanexo p
					INNER JOIN
						public.arquivo arq ON arq.arqid = p.arqid
					WHERE
						p.preid = ".$preid;
			$anexos = $db->carregar($sql);

			if( $anexos )
			{
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
					$boArquivo = $db->pegaUm("select count(aqoid) from obras2.arquivosobra where tpaid = 21 and obrid = {$dadosPreObra[0]['obrid']} and arqid = {$anexo['arqid']} and aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO obras2.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
								VALUES (".$dadosPreObra[0]['obrid'].", 21, ".$anexo['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
						$db->executar($sql);
					}
				}
			}

			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid_1 = ".$dadosPreObra[0]['obrid']." WHERE preid = ".$preid;
			$db->executar($sql);
		}
	}
	return $db->commit();
	/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
}

function alterarDadosObras2ReformuladaPAC( $preid ){
	global $db;
	/*** INICIO - Importação dos dados da obrasPar para o sistema de Obras - INICIO ***/

	/*** Só executa a importação caso a obra exista ***/
	$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid IS NOT NULL";
	$existeObra = $db->pegaUm($sql);

	if( (integer)$existeObra <> 0 ){
		/*** Recupera dados da Pre Obra ***/
		$sql = "SELECT DISTINCT
					po.preesfera as esfera,
					po.predescricao as nome_obra,
	                pto.tpoid, -- TIPOLOGIA DE OBRA
	                CASE
						WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
						WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
						WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
						ELSE 54 --OUTROS
	                END as programa,
	                CASE WHEN pto.ptoclassificacaoobra = 'Q' THEN 3  ELSE 1 END as modalidadeensino, -- MODALIDADE DE ENSINO
					CASE
						WHEN pto.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
						WHEN pto.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIAÇÃO
						ELSE 1 --CONSTRUÇÃO
					END AS tipodeobra,
					1 as fonte, -- FONTE PAC
	                CASE
						WHEN REPLACE(UPPER(po.predescricao), 'Í', 'I') ILIKE '%INDIGENA%' THEN 4 -- INDÍGENA
						WHEN UPPER(po.predescricao) ILIKE '%RURAL%' THEN 1 -- RURAL
						WHEN UPPER(po.predescricao) ILIKE '%QUILOMBO%' THEN 3 -- QUILOMBO
					ELSE 2 --URBANO
					END AS classificacaoobra, -- CLASSIFICAÇÃO DA OBRA
	                prevalorobra, -- VALOR PREVISTO
	                mun.mundescricao,
	                po.precep,
	                po.prelogradouro,
	                po.precomplemento,
	                po.prebairro,
	                po.muncod,
	                po.estuf,
	                po.prenumero,
	                po.prelatitude,
	                po.prelongitude,
	                oi.entid as entidunidade,
	                oi.obrid,
	                oi.empid,
	                oi.endid,
	                po.ptoid
				FROM
					obras.preobra po
				INNER JOIN territorios.municipio 		mun on po.muncod = mun.muncod
				--INNER JOIN entidade.endereco 			ende ON ende.muncod = po.muncod
				--INNER JOIN entidade.entidade 			ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
				--INNER JOIN entidade.funcaoentidade 	fen ON ent.entid = fen.entid AND fen.funid IN (1)
				INNER JOIN par.processoobraspaccomposicao  poc ON poc.preid = po.preid and poc.pocstatus = 'A'
				INNER JOIN par.processoobra 		pop ON pop.proid = poc.proid and pop.prostatus = 'A' 
				INNER JOIN par.empenho                 e   ON pop.pronumeroprocesso =  e.empnumeroprocesso and empstatus <> 'I'
				INNER JOIN par.empenhoobra          eop ON e.empid = eop.empid AND eop.preid = po.preid and eobstatus = 'A'
				LEFT JOIN obras2.obras				oi ON oi.preid = po.preid
				INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = po.ptoid
				WHERE
					po.preid = ".$preid;
		$dadosPreObra = $db->carregar($sql);

		//ATUALIZA ENDEREÇO
		if( $dadosPreObra[0]['endid'] ){
			$sql = "UPDATE entidade.endereco SET
						endcep = '".$dadosPreObra[0]['precep']."',
  						endlog = '".$dadosPreObra[0]['prelogradouro']."',
					  	endcom = '".$dadosPreObra[0]['precomplemento']."',
					  	endbai = '".$dadosPreObra[0]['prebairro']."',
					  	muncod = '".$dadosPreObra[0]['muncod']."',
					  	estuf = '".$dadosPreObra[0]['estuf']."',
					  	endnum = '".$dadosPreObra[0]['prenumero']."',
  						endstatus = 'A',
  						medlatitude = '".$dadosPreObra[0]['prelatitude']."',
  						medlongitude = '".$dadosPreObra[0]['prelongitude']."'
					WHERE
  						endid = ".$dadosPreObra[0]['endid'];

			$db->executar( $sql );
			$endid = $dadosPreObra[0]['endid'];
		} else {
			/*** Insere novo endereço da obra ***/
			$sql = "INSERT INTO
						entidade.endereco (tpeid,
										   endcep,
										   endlog,
										   endcom,
										   endbai,
										   muncod,
										   estuf,
										   endnum,
										   medlatitude,
										   medlongitude,
										   endstatus)

					VALUES
						( 4,
						  '".$dadosPreObra[0]['precep']."',
						  '".$dadosPreObra[0]['prelogradouro']."',
						  '".$dadosPreObra[0]['precomplemento']."',
						  '".$dadosPreObra[0]['prebairro']."',
						  '".$dadosPreObra[0]['muncod']."',
						  '".$dadosPreObra[0]['estuf']."',
						  '".$dadosPreObra[0]['prenumero']."',
						  '".$dadosPreObra[0]['prelatitude']."',
						  '".$dadosPreObra[0]['prelongitude']."',
						  'A' ) RETURNING endid";

			$endid = $db->pegaUm($sql);
		}

		// ATUALIZA EMPREENDIMENTO
		if ( $dadosPreObra[0]['empid'] ){
			$sql = "UPDATE obras2.empreendimento
					   SET
					   		endid = {$endid},
					   		tobid = {$dadosPreObra[0]['tipoobra']},
					   		prfid = {$dadosPreObra[0]['programa']},
					   		tooid = {$dadosPreObra[0]['fonte']},
					   		cloid = {$dadosPreObra[0]['classificacaoobra']},
					   		empvalorprevisto = {$dadosPreObra[0]['prevalorobra']},
					   		moeid = ".($dadosPreObra[0]['modalidadeensino'] ? $dadosPreObra[0]['modalidadeensino'] : 'null').",
					   		entidunidade = ".($dadosPreObra[0]['entidunidade'] ? $dadosPreObra[0]['entidunidade'] : 'null').",
					   		empdsc='{$dadosPreObra[0]['nome_obra']}',
					   		tpoid = ".($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'null').",
					   		empesfera='{$dadosPreObra[0]['esfera']}'
					WHERE
							empid = {$dadosPreObra['empid']}";
		}

		/*** Atualiza a nova OBRA ***/
		if( $dadosPreObra[0]['obrid'] ){

			$sql = "UPDATE obras2.obras
					   SET 	endid={$endid},
					   		entid=".($dadosPreObra[0]['entidunidade'] ? $dadosPreObra[0]['entidunidade'] : 'null').",
					   		tobid=".($dadosPreObra[0]['tipoobra'] ? $dadosPreObra[0]['tipoobra'] : '1').",
					   		tpoid=".($dadosPreObra[0]['tpoid'] ? $dadosPreObra[0]['tpoid'] : 'NULL').",
					       	cloid={$dadosPreObra[0]['classificacaoobra']},
					       	tooid={$dadosPreObra[0]['fonte']},
					       	obrnome='{$dadosPreObra[0]['nome_obra']}',
					       	obrvalorprevisto=".($dadosPreObra[0]['prevalorobra'] ? $dadosPreObra[0]['prevalorobra'] : 'null')."
					 WHERE
					 		obrid = ".$dadosPreObra[0]['obrid'];
			$db->executar( $sql );

			/*$sql = "DELETE FROM obras.arquivosobra WHERE obrid = ".$dadosPreObra[0]['obrid']." and tpaid = 21";
			$db->executar( $sql );*/

			/*** Recupera as fotos do terreno no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						public.arquivo arq
					INNER JOIN
						obras.preobrafotos pof ON arq.arqid = pof.arqid
					INNER JOIN
						obras.preobra pre ON pre.preid = pof.preid
					WHERE
						pre.preid = ".$preid."
					AND
						(substring(arqtipo,1,5) = 'image')";
			$fotosTerreno = $db->carregar($sql);
			//ver($sql, $sql,d);
			if( $fotosTerreno ){
				/*** Insere as fotos para galeria de fotos da obra ***/
				foreach($fotosTerreno as $foto){
					$boArquivo = $db->pegaUm("select count(aqoid) from obras.arquivosobra where tpaid = 21 and obrid = {$dadosPreObra[0]['obrid']} and arqid = {$foto['arqid']} and aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO obras2.obras_arquivos(
						            obrid,
						            tpaid,
						            arqid,
						            oardata,
						            oardtinclusao
								)VALUES (
									". $dadosPreObra[0]['obrid'] .",
									21,
									".$foto['arqid'].",
									NOW(),
									NOW()
								);";

						$db->executar($sql);
					}
				}
			}

			/*** Recupera os documentos anexos no Pré Obra ***/
			$sql = "SELECT DISTINCT
						arq.arqid
					FROM
						obras.preobraanexo p
					INNER JOIN
						public.arquivo arq ON arq.arqid = p.arqid
					WHERE
						p.preid = ".$preid;
			$anexos = $db->carregar($sql);

			if( $anexos )
			{
				/*** Insere os documentos nos arquivos da obra ***/
				foreach($anexos as $anexo)
				{
					$boArquivo = $db->pegaUm("select count(aqoid) from obras.arquivosobra where tpaid = 21 and obrid = {$dadosPreObra[0]['obrid']} and arqid = {$anexo['arqid']} and aqostatus ='A'");
					if( $boArquivo == 0 ){
						$sql = "INSERT INTO obras2.obras_arquivos(
						            obrid,
						            tpaid,
						            arqid,
						            oardata,
						            oardtinclusao
								)VALUES (
									".$dadosPreObra[0]['obrid'].",
									21,
									".$anexo['arqid'].",
									NOW(),
									NOW()
								);";

						$db->executar($sql);
					}
				}
			}

			/*** Inclue o ID da nova obra na tabela do pre obra ***/
			$sql = "UPDATE obras.preobra SET obrid = ".$dadosPreObra[0]['obrid']." WHERE preid = ".$preid;
			$db->executar($sql);
		}
	}
	return $db->commit();
	/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
}

function verificaPreenchimentoDadosUnidadePorData($dataAtualizacao = '2013-02-12', $tipo = 'html'){

	global $db;
	
	if( !$_SESSION['par']['inuid'] ){
		
		echo "<script>
				alert('Sessão expirada.');
				window.location='par.php?modulo=inicio&acao=C';
			</script>";
		die();
	}

	$arrPendencia = array();
	if( $_SESSION['par']['itrid'] == 2 ){ // MUNICIPIO
		// prefeitura
		$sql = "SELECT true FROM par.entidade WHERE entstatus = 'A' AND inuid = {$_SESSION['par']['inuid']} AND dutid = ".DUTID_PREFEITURA." AND coalesce(entdataalteracao, entdatainclusao)  > '{$dataAtualizacao}';";
// 		$sql = "SELECT usucpf FROM par.historicodadospar WHERE hdplink ilike '%orgaoEducacao&acao=A&funid=1&%' AND hpddatainsercao > '{$dataAtualizacao}' AND muncod = '{$_SESSION['par']['muncod']}'";
		$prefeitura = $db->pegaUm($sql);
// 		ver($sql,d);
		if( $prefeitura != 't' ){
			$arrPendencia[] = 'Confirme os dados da Prefeitura.';
		}

		// prefeito
		$sql = "SELECT true FROM par.entidade WHERE entstatus = 'A' AND inuid = {$_SESSION['par']['inuid']} AND dutid = ".DUTID_PREFEITO." AND coalesce(entdataalteracao, entdatainclusao)  > '{$dataAtualizacao}';";
// 		$sql = "SELECT usucpf FROM par.historicodadospar WHERE hdplink ilike '%orgaoEducacao&acao=A&funid=2&%' AND hpddatainsercao > '{$dataAtualizacao}' AND muncod = '{$_SESSION['par']['muncod']}'";
		$prefeito = $db->pegaUm($sql);
		if( $prefeito != 't' ){
			$arrPendencia[] = 'Confirme os dados do Prefeito.';
		}

		// secretaria municipal
		$sql = "SELECT true FROM par.entidade WHERE entstatus = 'A' AND inuid = {$_SESSION['par']['inuid']} AND dutid = ".DUTID_SECRETARIA_MUNICIPAL." AND coalesce(entdataalteracao, entdatainclusao)  > '{$dataAtualizacao}';";
// 		$sql = "SELECT usucpf FROM par.historicodadospar WHERE hdplink ilike '%orgaoEducacao&acao=A&funid=7&%' AND hpddatainsercao > '{$dataAtualizacao}' AND muncod = '{$_SESSION['par']['muncod']}'";
		$secMun = $db->pegaUm($sql);
		if( $secMun != 't' ){
			$arrPendencia[] = 'Confirme os dados da Secretaria Municipal de Educação.';
		}

		/// dirigente
		$sql = "SELECT true FROM par.entidade WHERE entstatus = 'A' AND inuid = {$_SESSION['par']['inuid']} AND dutid = ".DUTID_DIRIGENTE." AND coalesce(entdataalteracao, entdatainclusao)  > '{$dataAtualizacao}';";
// 		$sql = "SELECT usucpf FROM par.historicodadospar WHERE hdplink ilike '%orgaoEducacao&acao=A&funid=15&%' AND hpddatainsercao > '{$dataAtualizacao}' AND muncod = '{$_SESSION['par']['muncod']}'";
		$dirigente = $db->pegaUm($sql);
		if( $dirigente != 't' ){
			$arrPendencia[] = 'Confirme os dados do Dirigente Municipal de Educação.';
		}

		// Equipe Local
                $strAnd = "";
                if ($_SESSION['par']['estuf'] == 'DF') {
                    $strAnd = "estuf = '{$_SESSION['par']['estuf']}'";
                }else {
                    $strAnd = "muncod = '{$_SESSION['par']['muncod']}'";
                }
                
                $sql = "SELECT true FROM par.historicodadospar WHERE hdplink ilike '%equipeLocal%' AND hpddatainsercao > '{$dataAtualizacao}' AND {$strAnd}";
		$equipeLocal = $db->pegaUm($sql);
		if( $equipeLocal != 't' ){
			$arrPendencia[] = 'Confirme os dados da Equipe Local.';
		}

		// Comite Local
		$sql = "SELECT true FROM par.historicodadospar WHERE hdplink ilike '%comiteLocal%' AND hpddatainsercao > '{$dataAtualizacao}' AND {$strAnd}";
		$comiteLocal = $db->pegaUm($sql);
		if( $comiteLocal != 't' ){
			$arrPendencia[] = 'Confirme os dados da Comitê Local.';
		}

	} elseif( $_SESSION['par']['itrid'] == 1 ) { // ESTADO

		// secretaria estadual
		$sql = "SELECT true FROM par.entidade WHERE entstatus = 'A' AND inuid = {$_SESSION['par']['inuid']} AND dutid = ".DUTID_SECRETARIA_ESTADUAL." AND coalesce(entdataalteracao, entdatainclusao)  > '{$dataAtualizacao}';";
// 		$sql = "SELECT usucpf FROM par.historicodadospar WHERE hdplink ilike '%orgaoEducacao&acao=A&funid=6&%' AND hpddatainsercao > '{$dataAtualizacao}' AND estuf = '{$_SESSION['par']['estuf']}'";
		$secEst = $db->pegaUm($sql);
		if( $secEst != 't' ){
			$arrPendencia[] = 'Confirme os dados da Secretaria Estadual de Educação.';
		}

		// secretario estadual
		$sql = "SELECT true FROM par.entidade WHERE entstatus = 'A' AND inuid = {$_SESSION['par']['inuid']} AND dutid = ".DUTID_SECRETARIO_ESTADUAL." AND coalesce(entdataalteracao, entdatainclusao)  > '{$dataAtualizacao}';";
// 		$sql = "SELECT usucpf FROM par.historicodadospar WHERE hdplink ilike '%orgaoEducacao&acao=A&funid=25&%' AND hpddatainsercao > '{$dataAtualizacao}' AND estuf = '{$_SESSION['par']['estuf']}'";
		$secretario = $db->pegaUm($sql);
		if( $secretario != 't' ){
			$arrPendencia[] = 'Confirme os dados do Secretário Estadual de Educação.';
		}

		// Equipe Local
		$sql = "SELECT true FROM par.historicodadospar WHERE hdplink ilike '%equipeLocal%' AND hpddatainsercao > '{$dataAtualizacao}' AND estuf = '{$_SESSION['par']['estuf']}'";
		$equipeLocal = $db->pegaUm($sql);
		if( $equipeLocal != 't' ){
			$arrPendencia[] = 'Confirme os dados da Equipe Local.';
		}

		// Comite Local
		$sql = "SELECT true FROM par.historicodadospar WHERE hdplink ilike '%comiteLocal%' AND hpddatainsercao > '{$dataAtualizacao}' AND estuf = '{$_SESSION['par']['estuf']}'";
		$comiteLocal = $db->pegaUm($sql);
		if( $comiteLocal != 't' ){
			$arrPendencia[] = 'Confirme os dados da Comitê Local.';
		}
	}

	if( $tipo == 'boolean' ){
		if( $arrPendencia[0] ){
			return false;
		} else {
			return true;
		}
	}

	if( is_array($arrPendencia) && $arrPendencia[0] ){ // Tem pendencia
		$perfil = pegaArrayPerfil($_SESSION['usucpf']);

		if( (in_array(PAR_PERFIL_SUPER_USUARIO,$perfil) ||
			in_array(PAR_PERFIL_ADMINISTRADOR,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil) ||
			in_array(PAR_PERFIL_PREFEITO,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
			in_array($_SESSION['usucpf'], array('', '')) )
		){
			$msgs = implode('\n', $arrPendencia);
			echo "<script>
				alert('{$msgs}');
				window.location='par.php?modulo=principal/orgaoEducacao&acao=A';
			  </script>";
			die;
		} else {
			return 'Aguardando';
		}
	}

	return true;
}

function verificaPendenciaAtualizacaoPAR($tipoVisualizacao){

	global $db;

	$sql = "SELECT DISTINCT
				dimcod, arecod, indcod, sbaordem,
				sbaid,
				esdid,
				aciid,
				acinomeresponsavel,
				acicargoresponsavel,
				indid
			FROM (

				-- Subações em atualização
				SELECT DISTINCT
					dim.dimcod, ar.arecod, i.indcod, s.sbaordem, s.sbaid, d.esdid, a.aciid, a.acinomeresponsavel, a.acicargoresponsavel, i.indid
				FROM
					par.subacao s
				INNER JOIN workflow.documento d ON d.docid = s.docid AND d.esdid = ".WF_SUBACAO_ATUALIZACAO."
				INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
				INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A'
				INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
				INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
				INNER JOIN par.dimensao  dim  ON dim.dimid  = ar.dimid AND dim.dimstatus = 'A'
				WHERE
					s.sbastatus = 'A' AND
					p.inuid = ".$_SESSION['par']['inuid']."


				UNION ALL

				-- Ações sem preenchimento
				SELECT DISTINCT
					dim.dimcod, ar.arecod, i.indcod, s.sbaordem, s.sbaid, d.esdid, a.aciid, a.acinomeresponsavel, a.acicargoresponsavel, i.indid
				FROM
					par.subacao s
				INNER JOIN workflow.documento d ON d.docid = s.docid
				INNER JOIN workflow.historicodocumento hd ON hd.docid = d.docid
				INNER join workflow.acaoestadodoc  aed ON aed.aedid = hd.aedid AND (aed.esdidorigem = ".WF_SUBACAO_ATUALIZACAO." OR aed.esdiddestino = ".WF_SUBACAO_ATUALIZACAO." )
				INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
				INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A'
				INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
				INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
				INNER JOIN par.dimensao  dim  ON dim.dimid  = ar.dimid AND dim.dimstatus = 'A'
				WHERE
					s.sbastatus = 'A' AND
					p.inuid = ".$_SESSION['par']['inuid']."

			) as foo
			ORDER BY
				dimcod, arecod, indcod, sbaordem, indid";

	$pendencias = $db->carregar($sql);

	$arrAcao = array();
	if( $tipoVisualizacao == 'HTML' ){
		if( is_array($pendencias) && $pendencias[0] ){
			foreach( $pendencias as $pendencia ){
				if( $pendencia['esdid'] == WF_SUBACAO_ATUALIZACAO ){
					$dado = '';
					$dado = $db->pegaUm("SELECT d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || s.sbaordem || ' - ' || sbadsc as sbadsc FROM par.subacao s INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
							INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
							INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
							INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A'
							INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
							INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
							INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A' WHERE s.sbaid = ".$pendencia['sbaid']);
					echo '<tr><td>
							<a href="#">
								<img border="0" src=\'/imagens/consultar.gif\' onclick=\'javascript:janela("par.php?modulo=principal/subacao&acao=A&sbaid='.$pendencia['sbaid'].'",800,600,"Subação")\'>
							</a>
							</td>
							<td>
								<b>'.$dado.'</b>
							</td></tr>';
				}
				if( $pendencia['acinomeresponsavel'] == '' || $pendencia['acicargoresponsavel'] == '' ){
					$dado = '';
					$dado = $db->pegaUm("SELECT acidsc FROM par.acao WHERE aciid = ".$pendencia['aciid']);
					if( !in_array($pendencia['aciid'], $arrAcao) ){
						$arrAcao[] = $pendencia['aciid'];
						echo '<tr><td>
								<a href="#">
									<img border="0" src=\'/imagens/consultar.gif\' onclick=\'javascript:janela("par.php?modulo=principal/parAcao&acao=A&tipo=atualizacao&aciid='.$pendencia['aciid'].'&indid='.$pendencia['indid'].'",800,600,"Ação")\'>
								</a>
								</td>
								<td>
									<b>'.$dado.'</b>
								</td></tr>';
					}
				}
			}
		} else {
			echo '<tr>
					<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
						<b>Não existem pendências</b>
					</td>
				</tr>';
		}
	}
}

function verificaExtratoAtualizacaoPAR($tipoVisualizacao){

	global $db;

	$sql = "SELECT DISTINCT
					dim.dimcod, ar.arecod, i.indcod, s.sbaordem, s.sbaid, a.aciid, a.acinomeresponsavel, a.acicargoresponsavel, i.indid
				FROM
					par.subacao s
				INNER JOIN workflow.historicodocumento h ON h.docid = s.docid AND h.aedid = 1633
				INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
				INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A'
				INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
				INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
				INNER JOIN par.dimensao  dim  ON dim.dimid  = ar.dimid AND dim.dimstatus = 'A'
				WHERE
					p.inuid = ".$_SESSION['par']['inuid']."
				ORDER BY
					dim.dimcod, ar.arecod, i.indcod, s.sbaordem";

	$extrato = $db->carregar($sql);

	$arrAcao = array();
	if( $tipoVisualizacao == 'HTML' ){
		if( is_array($extrato) && $extrato[0] ){
			foreach( $extrato as $ext ){
					$dado = '';
					$dado = $db->pegaUm("SELECT
											d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || s.sbaordem || ' - ' || sbadsc as sbadsc
										FROM
											par.subacao s INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
										INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
										INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
										INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A'
										INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
										INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
										INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A' WHERE s.sbaid = ".$ext['sbaid']."
										ORDER BY
												d.dimcod, ar.arecod, i.indcod, s.sbaordem");
					echo '<tr><td>
							<a href="#">
								<img border="0" src=\'/imagens/consultar.gif\' onclick=\'javascript:janela("par.php?modulo=principal/subacao&acao=A&sbaid='.$ext['sbaid'].'",800,600,"Subação")\'>
							</a>
							</td>
							<td>
								<b>'.$dado.'</b>
							</td></tr>';
			}
		} else {
			echo '<tr>
					<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
						<b>Não existem subações atualizadas</b>
					</td>
				</tr>';
		}
	}
}

function verificaValidacaoQuestoesPontuais( $inuid = null ){
	global $db;

	$inuid = $inuid ? $inuid : $_SESSION['par']['inuid'];

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);

	$sql = "SELECT atpquestoespontuais FROM par.atualizacaoPAR WHERE inuid = ".$inuid;
	$testeQuestoes = $db->pegaUm( $sql );

	if( !$testeQuestoes ){
		if( (in_array(PAR_PERFIL_SUPER_USUARIO,$perfil) ||
			in_array(PAR_PERFIL_ADMINISTRADOR,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil) ||
			in_array(PAR_PERFIL_PREFEITO,$perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
			in_array($_SESSION['usucpf'], array('', '')) )
		){
			echo "<script>
				alert('Você precisa validar as Questões Pontuais');
				window.location='par.php?modulo=principal/questoesPontuais&acao=A&atualizacaopar=true';
			  </script>";
			die;
		} else {
			$testeQuestoes = 'Aguardando';
		}
	}

	return $testeQuestoes;
}

function verificaAbasAtualizacao($inuid){
	global $db;

	$mnuid = array();

	$sql = "SELECT atpjustificativa, atpquestoespontuais, atpalteracoesdiscutidas, atpconhecimentoideb, atpjustificativaideb FROM par.atualizacaopar WHERE inuid = ".$inuid;
	$dados = $db->pegaLinha($sql);

	//ver(situacaoIndicadoresAtualizacaoPAR($inuid), d);
	if( situacaoIndicadoresAtualizacaoPAR($inuid) == false ){
		$mnuid = array( //1 => 12032,
						//2 => 12033,
						//3 => 12034,
						//4 => 12035
						//5 => 12038,
						6 => 12039,
						7 => 12040,
						8 => 12059
						);
	}
	if( !$dados['atpjustificativaideb'] ){
		$mnuid = array( //1 => 12032,
						//2 => 12033,
						//3 => 12034,
						//4 => 12035
						5 => 12038,
						6 => 12039,
						7 => 12040,
						8 => 12059
						);
	}
	if( !$dados['atpalteracoesdiscutidas'] ){
		$mnuid = array( //1 => 12032,
						//2 => 12033,
						//3 => 12034,
						4 => 12035,
						5 => 12038,
						6 => 12039,
						7 => 12040,
						8 => 12059
						);
	}
	if( !$dados['atpquestoespontuais'] ){
		$mnuid = array( //1 => 12032,
						//2 => 12033,
						3 => 12034,
						4 => 12035,
						5 => 12038,
						6 => 12039,
						7 => 12040,
						8 => 12059
						);
	}
	if( verificaPreenchimentoDadosUnidadePorData('2013-02-12', 'boolean') == false ){
		$mnuid = array( //1 => 12032,
						2 => 12033,
						3 => 12034,
						4 => 12035,
						5 => 12038,
						6 => 12039,
						7 => 12040,
						8 => 12059
						);
	}
	if( !$dados['atpjustificativa'] ){
		$mnuid = array( 1 => 12032,
						2 => 12033,
						3 => 12034,
						4 => 12035,
						5 => 12038,
						6 => 12039,
						7 => 12040,
						8 => 12059
						);
	}

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);

	/*
	if( !in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ){
		$abaExtrato = array( 0 => 12059 );
		$mnuid = array_merge($mnuid, $abaExtrato);
	}
	*/

	return $mnuid;
}

function situacaoIndicadoresAtualizacaoPAR($inuid){
	global $db;

	$entidade = $db->pegaLinha("SELECT itrid, CASE WHEN itrid = 1 THEN estuf ELSE muncod END as entidade, CASE WHEN itrid = 1 THEN 'estuf' ELSE 'muncod' END as tipoentidade FROM par.instrumentounidade WHERE inuid = ".$inuid);

	$dadosTermo = $db->pegaUm("SELECT DISTINCT
										COALESCE(count(dop.dopid),0) as conta
									FROM
										par.vm_documentopar_ativos dop
									INNER JOIN par.processopar prp ON prp.prpid = dop.prpid and prp.prpstatus = 'A'
									INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
									where
										dop.mdoid = 20 AND
										iu.".$entidade['tipoentidade']." = '".$entidade['entidade']."' AND
										dop.dopid NOT IN ( SELECT dopid FROM par.documentoparvalidacao dpv )");

	$dadosHabilita = $db->pegaUm("SELECT iuesituacaohabilita FROM par.instrumentounidadeentidade WHERE iuedefault = 't' AND inuid = ".$inuid);

    $obrasPendentes = null;
    if($inuid){
        $dadoEntidade = $db->pegaLinha("SELECT iu.itrid, iu.muncod, iu.estuf FROM par.instrumentounidade iu WHERE iu.inuid = ".$inuid);
        $esfera = $dadoEntidade['itrid'] == 2 ? 'M' : 'E';
        $muncod = $dadoEntidade['muncod'];
        $estuf  = $dadoEntidade['estuf'];
        if($esfera == 'M' && $muncod){
            $obrasPendentes = getObrasPendentesPAR($muncod);
        } elseif($esfera == 'E' && $estuf) {
            $obrasPendentes = getObrasPendentesPAR(null, $estuf);
        }
    }

	if( $dadosTermo > 0 || $dadosHabilita != 'Habilitado' || $obrasPendentes ){
		// pendente
		return false;
	} else {
		// sem pendencias
		return true;
	}
}

function atualizaHabilitaPar($cnpj){
	global $db;
	include_once APPRAIZ . "par/classes/Habilita.class.inc";

	$obHabilita = new Habilita();

	$habilitado = $obHabilita->consultaHabilitaEntidade($cnpj, true);
	$habilitado = json_decode($habilitado);
	$habilitado = utf8_decode($habilitado->descricao);

	$sql = "UPDATE par.instrumentounidadeentidade SET
  				iuesituacaohabilita = '{$habilitado}',
  				iueenviado = true
  			WHERE
  				iuecnpj = '{$cnpj}'";

  	$db->executar($sql);
  	$db->commit();
}

function retiraPontosBD($valor){
	$valor = str_replace(".","", $valor);
	$valor = str_replace(",",".", $valor);

	return $valor;
}

function pegaCnpjInuid($inuid){
	global $db;

	$sql = "SELECT iue.iuecnpj
			FROM par.instrumentounidade iu
				inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
			WHERE
				iu.inuid = {$inuid}
                and iue.iuestatus = 'A'
                and iue.iuedefault = true";
	$cnpj = $db->pegaUm($sql);

	return $cnpj;
}

function verificaPreenchimentoMI( $preid ){
	global $db;

	$reformulaMI = verificaMi( $preid );

	$oPreObra = new PreObra();
	$oSubacaoControle = new SubacaoControle();
	$pacFNDE  = $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
	$arDados  = $oSubacaoControle->recuperarPreObra($preid);
	$qrpid = pegaQrpidPAC( $preid, 43 );
	$pacDados = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
	$pacFotos = $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
	$pacDocumentos = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
	if($pacFNDE == 'f'){
		$pacDocumentosTipoA = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados, true);
	}
	$pacQuestionario = $oPreObra->verificaQuestionario($qrpid);
	$boPlanilhaOrcamentaria = $oSubacaoControle->verificaPlanilhaOrcamentaria($preid, SIS_OBRAS, $preid);
	$pacCronograma = $oPreObra->verificaCronograma($preid);
	$pacLatitude   = $oPreObra->verificaLatitudePreObra($preid);
	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);

	$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];
	$arPendencias = array('Dados do terreno' => 'Falta o preenchimento dos dados.',
					  'Latitude e Longitude dos Dados do Terreno' => 'Falta o preenchimento da Latitude e Longitude.',
					  'Relatório de vistoria' => 'Falta o preenchimento dos dados.',
					  'Cadastro de fotos do terreno' => 'Deve conter no mínimo 3 fotos do terreno.',
					  'Cronograma físico-financeiro' => 'Falta o preenchimento dos dados.',
					  'Documentos anexos' => 'Falta anexar os arquivos.',
					  'Projetos - Tipo A' => 'Falta anexar os arquivos.',
					  'Itens Planilha orçamentária' => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
					  'Planilha orçamentária' => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' item(s) a ser(em) preenchido(s) na planilha orçamentaria.',
					  'Valor da planilha orçamentária' => 'O valor R$ '.formata_valor($boPlanilhaOrcamentaria['valor']).' não confere, deve estar entre R$ '.formata_valor($boPlanilhaOrcamentaria['minimo']).' e R$ '.formata_valor($boPlanilhaOrcamentaria['maximo']).'.');
	$testa = true;

	foreach($arPendencias as $k => $v){
		if(  ( !$pacDados && $k == 'Dados do terreno' ) ||
			 ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) ||
			 ( $k == 'Latitude e Longitude dos Dados do Terreno' && !$pacLatitude ) ||
			 ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
			 ( $k == 'Itens Planilha orçamentária' && $boPlanilhaOrcamentaria['faltam'] > 0 ) ||
			 ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $arDados['ptoprojetofnde'] == 't' && !($reformulaMI)) ||
			 ( $k == 'Valor da planilha orçamentária' && ( str_replace(',','',number_format($boPlanilhaOrcamentaria['valor'],2)) < $boPlanilhaOrcamentaria['minimo'] || str_replace(',','',number_format($boPlanilhaOrcamentaria['valor'],2)) > $boPlanilhaOrcamentaria['maximo']) && $pacFNDE == 't' && !($reformulaMI) ) ||
			 ( $k == 'Cronograma físico-financeiro' && !$pacCronograma && $arDados['ptoprojetofnde'] == 't' && !($reformulaMI) ) ||
			 ( ($pacDocumentosTipoA['arqid'] != $pacDocumentosTipoA['podid'] || !$pacDocumentosTipoA) && $k == 'Projetos - Tipo A' && $arDados['ptoprojetofnde'] == 'f' ) ||
			 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' )
			 ){
			 	if( $testa == true ){
				 	$str = "Preencha os seguintes dados: ". $k;
			 	} else {
				 	$str = $str . ", " . $k;
			 	}
			 	$testa = false;
			 }
	}

	if($testa == false){
		return $str;
	} else {
		return true;
	}

}

function verificaEnvioMI( $preid ){
	global $db;

	$sql = "SELECT
				pre.estuf,
				pre.preesfera,
				COALESCE(oi.obrpercentultvistoria, 0) as percexec
			FROM
				obras.preobra pre
			INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = pre.ptoid
			LEFT  JOIN territorios.municipio 	mun ON mun.muncod = pre.muncod
			LEFT  JOIN obras2.obras 			oi  ON oi.preid = pre.preid AND oi.obrstatus = 'A'
			WHERE
				pre.preid = $preid
				AND pre.prestatus = 'A'";
	$dados = $db->pegaLinha($sql);

//	$sql = "SELECT aoscodsituacao FROM par.adesaoobraspacsituacao WHERE preid = ".$preid." AND aoscodsituacao = 110";
//	$situacaoObra = $db->pegaUm($sql);
//
//	if( $situacaoObra ){ // Já possui contrato assinado
//		return 'Você não pode tramitar para este estado pois a obra já possui contrato assinado.';
//	}

	// se tiver mais de 0% de execução da obra não pode tramitar
	if( (float)$dados['percexec'] > 4 ){
		return 'Verifique o percentual de execução da obra';
	} else {
		if( $dados['estuf'] == 'DF' ){
			return true;
		} elseif( $dados['preesfera'] == 'M' ){
			return true;
		} else {
			return 'Você não pode tramitar para este estado';
		}
	}
}

function enviaMensagemMI( $preid ){
	echo '<script>alert("Senhor(a) Gestor(a),\n\n\tAntes de aderir à ata de Registro de Preços verifique a situação do local onde a obra será edificada. Caso haja necessidade na alteração do local da obra o proponente deve formalizar essa solicitação ao FNDE mediante Oficio assinado pelo(a) Prefeito(a) Municipal, encaminhar para o e-mail: reformulacao_cgest@fnde.gov.br e aguardar análise. \n\n\tRessaltamos que após adesão à ata de Registro de Preços para construção de Creche/Escola – Proinfância é vedado ao município efetuar alterações no local já aprovado para a obra. \n\n\tA partir da validação de adesão à Ata de Registro de Preços o proponente deverá providenciar à adequação do terreno disponibilizado para a construção da obra conforme exigências do FNDE.");</script>';
	return true;
}

function verificaProrrogacao( $preid ){
	global $db;
    $sql = "SELECT popvalidacao FROM obras.preobraprorrogacao WHERE popstatus = 'P' AND preid = ".$preid;
    $validacao = $db->pegaUm( $sql );

    if( $validacao == 'f' ){
        return true;
    } else {
        return false;
    }
}

function gerarDocumentoProrrogacao( $preid, $popparecer, $data, $dias, $tipo = 1 ){
	global $db;

	ob_clean();

	$termo = $db->pegaUm("select
                                terid || '/' || TO_CHAR( terdatainclusao, 'YYYY' ) as termo
                        from
                                par.processoobraspaccomposicao ppc
                        inner join par.termocompromissopac ter on ter.proid = ppc.proid AND ter.terstatus = 'A'
                        where
                                ppc.pocstatus = 'A' and
                                ppc.preid = ".$preid);

	$sql = "SELECT
				pre.predescricao as nome,
				mun.mundescricao || '/' || mun.estuf as entidade ,
				COALESCE(oi.obrpercentultvistoria, 0) as percexec,
				esd2.esddsc as situacao,
				CASE
					WHEN ov.supdtinclusao IS NOT NULL THEN
						ov.supdtinclusao
					WHEN oi.obrdtvistoria IS NOT NULL THEN
						oi.obrdtvistoria
					 ELSE
						oi.obrdtinclusao
				END as ultatualizacao,
                                (
                                    select distinct
                                        to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')  as pronumeroprocesso
                                    from 
                                        par.processoobraspaccomposicao ppc
                                    inner join par.processoobra poc on poc.proid = ppc.proid and poc.prostatus = 'A'
                                    left join obras2.pagamentopac po on poc.pronumeroprocesso = po.ppaprocesso
                                    where 
                                        ppc.pocstatus = 'A' and
                                        preid = $preid
                                ) as processo
			FROM
				obras.preobra pre
			INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
			LEFT  JOIN obras2.obras oi
				INNER JOIN workflow.documento doc ON doc.docid = oi.docid
				INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc.esdid
			ON oi.preid = pre.preid
			INNER JOIN workflow.documento d ON d.docid = pre.docid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
			LEFT JOIN
				(SELECT
					supvid as supervisao, rsuid,obrid, supdtinclusao
				FROM
					obras.supervisao s
				WHERE
					supvid = (SELECT supvid FROM obras.supervisao ss WHERE ss.supstatus = 'A' AND ss.obrid = s.obrid ORDER BY supvdt desc, supvid DESC LIMIT 1)) AS ov
			ON ov.obrid = oi.obrid
			WHERE
				pre.prestatus = 'A'
				AND pre.preid = $preid";

        
	$dadosObra = $db->pegaLinha( $sql );
	$nomedaObra = $dadosObra['nome'];
	$entidade = $dadosObra['entidade'];
	$processo = $dadosObra['processo'];
        
	if( $tipo == 1 ){
		$html = obterHtmlProrrogacaoPrazo($preid);
	} else {
		$html =  '<html>
						<head>
						<style type="">
							.fot{
								font-family: arial black;
								font-size: 12px;
								text-align: center;
								}
							.lista{
								font-size: 11px;
								padding: 3px;
								border-collapse: collapse;
							}
							.lista1{
								font-size: 11px;
								padding: 3px;
								border-collapse: collapse;
							}
							.folha {
						    	page-break-after: always;
							}
	
						</style>
						</head>
						<body>
							<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
								<tr bgcolor="#ffffff">
									<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0">			
										<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
										FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />
									</td>
								</tr>				
							</table><br><br>
							<table width="100%" align="center" cellspacing="0" cellpadding="0">
							<tr>
								<td>
									<table width="100%" class="lista" align="center" cellspacing="1" cellpadding="4">
										<tr>
											<td valign="top" style="text-align: left;"><span class="fot">
											Obra: '.$nomedaObra.'<br>
											ID nº: '.$preid.'<br>
											Final da vigência: '.formata_data($data).'<br>
											Termo de Compromisso: '.$termo.'<br>
											Município: '.$entidade.'<br>
                                                                                        Processo: '.$processo.'<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
											</span></td>
										</tr>
										<tr>
											<td valign="top" style="text-align: center;"><span class="fot">
											<b>Reprogramação (prazo) EX OFFÍCIO em virtude da necessidade de adequação do cronograma de execução da obra.<br>
											As demais cláusulas permanecem inalteradas<br>
											VALIDAÇÃO ELETRÔNICA DO DOCUMENTO</b>
											</span></td>
										</tr>
									</table>
								</td>
							</tr>
						</table>
					</body>
				</html>';
	}
	include_once APPRAIZ . "www/par/prefeitos/dompdf/dompdf_config.inc.php";
	
	ob_start();
	ob_clean();
	
	$dompdf = new DOMPDF();
	$dompdf->load_html($html);
	$dompdf->render();
	
	$response = $dompdf->output();

	$fp = fopen(APPRAIZ . 'arquivos/obras/protocolo_prorrogacao_'.date('Y').'-'.$preid . '.pdf', "w");
	if ($fp) {
	  stream_set_write_buffer($fp, 0);
	  fwrite($fp, $response);
	  fclose($fp);
	}

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arrCampos = array();
	$file = new FilesSimec("preobraprorrogacao", $arrCampos, "obras");
	$file->setMover(APPRAIZ . 'arquivos/obras/protocolo_prorrogacao_'.date('Y').'-'.$preid . '.pdf', 'pdf', false);
	$arqid = $file->getIdArquivo();

	$sql = "UPDATE obras.preobraprorrogacao SET arqid = ".$arqid." WHERE popstatus = 'A' AND preid = ".$preid;
	$db->executar($sql);
        
	if($db->commit()){
		return 'sucesso';
	} else {
		return 'erro';
	}

}

function gerarDocumentoRecusarProrrogacao($preid){
    global $db;

    $html = obterHtmlProrrogacaoPrazo($preid, 'INDEFERIMENTO');

    include_once APPRAIZ . "www/par/prefeitos/dompdf/dompdf_config.inc.php";

    ob_start();
    ob_clean();
    

    $dompdf = new DOMPDF();
    $dompdf->load_html($html);
    $dompdf->render();

    $response = $dompdf->output();

    $fp = fopen(APPRAIZ . 'arquivos/obras/protocolo_prorrogacao_'.date('Y').'-'.$preid . '.pdf', "w");
    if ($fp) {
        stream_set_write_buffer($fp, 0);
        fwrite($fp, $response);
        fclose($fp);
    }

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arrCampos = array();
    $file = new FilesSimec("preobraprorrogacao", $arrCampos, "obras");
    $file->setMover(APPRAIZ . 'arquivos/obras/protocolo_prorrogacao_'.date('Y').'-'.$preid . '.pdf', 'pdf', false);
    $arqid = $file->getIdArquivo();

    $sql = "UPDATE obras.preobraprorrogacao SET popvalidacao = 't', arqid = ".$arqid.", popdatavalidacao = 'NOW()' WHERE popstatus = 'F' AND preid = ".$preid."; ";
    $db->executar($sql);

    if($db->commit()){
            return 'sucesso';
    } else {
            return 'erro';
    }
}

function obterHtmlProrrogacaoPrazo($preid, $parecer = 'DEFERIMENTO') {
    global $db;
    
    # Retorna os dados do validador.
    $dadosUsuario = $db->pegaLinha( "SELECT usunome, usucpf FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'" );
    
    # Verifica se PAR ou PAC e Gera Termo
    $sql = "SELECT tooid FROM obras.preobra WHERE preid = {$preid}";
    $tooid = $db->pegaUm($sql);
    
    $termo = '';
    $tipoObra = '';
    if ($tooid == 1) { # PAC
        $termo = $db->pegaUm("SELECT
                            'PAC ' || terid || '/' || TO_CHAR( terdatainclusao, 'YYYY' ) as termo
                        FROM
                            par.processoobraspaccomposicao ppc
                        INNER JOIN par.termocompromissopac ter on ter.proid = ppc.proid AND ter.terstatus = 'A'
                        WHERE
                            ppc.pocstatus = 'A' 
                            AND ppc.preid = {$preid}");
    } else {
        $termo = $db->pegaUm("SELECT 
                                dp.dopnumerodocumento 
                            FROM par.termocomposicao tc
                            INNER JOIN par.documentopar dp ON dp.dopid=tc.dopid
                            WHERE tc.preid = {$preid}");
        $tipoObra = 'PAR ';           
    }
    
    # Retorna dados da Obra
    $sql = "SELECT
                pre.predescricao as nome,
                mun.mundescricao || '/' || mun.estuf as entidade ,
                COALESCE(oi.obrpercentultvistoria, 0) as percexec,
                esd2.esddsc as situacao,
                CASE
                  WHEN oi.obrdtvistoria IS NOT NULL THEN
                    oi.obrdtvistoria
                  WHEN ov.supdtinclusao IS NOT NULL THEN
                    ov.supdtinclusao
                  ELSE
                    oi.obrdtinclusao
                END as ultatualizacao,
                (
                    select distinct
                        to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')  as pronumeroprocesso
                    from 
                        par.processoobraspaccomposicao ppc
                    inner join par.processoobra poc on poc.proid = ppc.proid and poc.prostatus = 'A'
                    left join obras2.pagamentopac po on poc.pronumeroprocesso = po.ppaprocesso
                    where 
                        ppc.pocstatus = 'A' and
                        preid = $preid
                ) as processo
            FROM
                obras.preobra pre
            INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
            LEFT  JOIN obras2.obras oi
            INNER JOIN workflow.documento doc ON doc.docid = oi.docid
            INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc.esdid
            ON oi.preid = pre.preid
            INNER JOIN workflow.documento d ON d.docid = pre.docid
            INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
            LEFT JOIN
                (SELECT
                    supvid as supervisao, rsuid,obrid, supdtinclusao
                FROM
                    obras.supervisao s
                WHERE
                    supvid = (SELECT supvid FROM obras.supervisao ss WHERE ss.supstatus = 'A' AND ss.obrid = s.obrid ORDER BY supvdt desc, supvid DESC LIMIT 1)) AS ov
            ON ov.obrid = oi.obrid
            WHERE
                pre.prestatus = 'A'
                AND pre.preid = $preid ORDER BY ultatualizacao DESC";
        
    $dadosObra = $db->pegaLinha( $sql );
        
    # Retorna Dados da prorrogacao
    $popstatus = ($parecer == 'DEFERIMENTO') ? 'A' : 'F';
    $preobraprorrogacao = $db->pegaLinha("SELECT * FROM obras.preobraprorrogacao WHERE popstatus = '$popstatus' AND preid = ".$preid." ORDER BY popid DESC LIMIT 1");
    $strHtml = '';
    if ($preid) {
        $qtdDia = ($parecer == 'DEFERIMENTO') ? $preobraprorrogacao['popqtddiasaprovado'] : $preobraprorrogacao['popqtddiassolicitado'];
        $dtAprovado = ($parecer == 'DEFERIMENTO') ? $preobraprorrogacao['popdataprazoaprovado'] : $preobraprorrogacao['popdataprazo'];
        $strHtml = '<html>
                        <head>
                            <style>
                                p {text-align : justify;}
                                h3 {text-align : center; text-decoration: underline}
                                .cabecalh_pdf { text-align: center; width:100%}
                                .folha {
                                    page-break-after: always;
                                }
                            </style>
                        </head>
                        <body>
                            <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                                <tr bgcolor="#ffffff">
                                    <td valign="top" align="center">
                                        <img src="../imagens/brasao.gif" width="45" height="45" border="0">			
                                        <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />
                                    </td>
                                </tr>				
                            </table>
                            <div class="body_pdf">
                                <h3>ANÁLISE DE SOLICITAÇÃO DE REPROGRAMAÇÃO DE PRAZO</h3>

                                <p>EMENTA: Solicitação de reprogramação do prazo da obra <strong>'.$dadosObra['nome'].'</strong>, ID <strong>'.$preid.'</strong>, referente ao Termo de Compromisso <strong>'.$tipoObra.''.$termo.'</strong>, de <strong>'.$dadosObra['entidade'].'</strong> pelo período de <strong>'.$qtdDia.'</strong> dias, finalizando em <strong>'.formata_data($dtAprovado).'</strong>, com base na seguinte justificativa:</p>

                                <p>'.$preobraprorrogacao['popjustificativa'].'</p>

                                <p>De acordo com a última atualização constante no módulo de monitoramento de obras do SIMEC inserida em <strong>'.formata_data($dadosObra['ultatualizacao']).'</strong>, a obra está na situação <strong>'.$dadosObra['situacao'].'</strong>, com <strong>'.$dadosObra['percexec'].'% de execução</strong>.</p>

                                <p>'.$preobraprorrogacao['popparecer'].'</p>';
        
                                if ($parecer == 'DEFERIMENTO') {
                                    $strHtml .='<p>Considerando o interesse público em concluir a ação pactuada, somos pelo <strong>DEFERIMENTO</strong> da reprogramação do prazo.</p>';
                                }else{
                                    $strHtml .='<p>Considerando o parecer acima, somos pelo <strong>INDEFERIMENTO</strong> da reprogramação do prazo.</p>';
                                }
                            $strHtml .='<p>Havendo necessidade de orientações e esclarecimentos complementares, estamos à disposição através do e-mail: grupo.prorrogacao@fnde.gov.br.</p>

                                <p>Protocolo gerado no dia <strong>'.date('d/m/Y G:i:s').'</strong> e validado por <strong>'.$dadosUsuario['usunome'].'</strong>, CPF: <strong>'.$dadosUsuario['usucpf'].'</strong>.</p>
                            </div>
                        </body>
                    </html>';
    }
    
    return $strHtml;
}

function validaPossibilidadeProrrogacao($preid){
	global $db;

	//necessita depara stoid = esdid
//	$sql = "SELECT
//				CASE
//					WHEN ov.supdtinclusao IS NOT NULL AND (oi.stoid IN (3,6,7) OR (oi.stoid IN (2) AND tpl.tplid IN (3))) THEN
//						CASE WHEN DATE_PART('days', NOW() - ov.supdtinclusao) <= 45 THEN
//								CASE WHEN oi.obrpercexec >= 100.00 THEN
//									'preto'
//								ELSE
//									'preto'
//								END
//							 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 45 AND DATE_PART('days', NOW() - ov.supdtinclusao) <= 60 THEN
//								CASE WHEN oi.obrpercexec >= 100.00 THEN
//									'preto'
//								ELSE
//									'preto'
//								END
//							 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 60 THEN
//								CASE WHEN oi.obrpercexec >= 100.00 THEN
//									'preto'
//								ELSE
//									'preto'
//								END
//							 ELSE
//								'preto'
//						END
//					WHEN ov.supdtinclusao IS NOT NULL AND oi.stoid NOT IN (3,4,5,6,7,99) THEN
//						CASE WHEN DATE_PART('days', NOW() - ov.supdtinclusao) <= 45 THEN
//								CASE WHEN oi.obrpercexec >= 100.00 THEN
//									'azul'
//								ELSE
//									'verde'
//								END
//							 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 45 AND DATE_PART('days', NOW() - ov.supdtinclusao) <= 60 THEN
//								CASE WHEN oi.obrpercexec >= 100.00 THEN
//									'azul'
//								ELSE
//									'amarelo'
//								END
//							 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 60 THEN
//								CASE WHEN oi.obrpercexec >= 100.00 THEN
//									'azul'
//								ELSE
//									'vermelho'
//								END
//							 ELSE
//								'preto'
//						END
//					WHEN oi.stoid IN (1, 2) THEN
//						CASE WHEN oi.obrdtvistoria IS NOT NULL THEN
//								CASE WHEN DATE_PART('days', NOW() - oi.obrdtvistoria) <= 45 THEN
//										CASE WHEN oi.obrpercexec >= 100.00 THEN
//											'azul'
//										ELSE
//											'verde'
//										END
//									 WHEN DATE_PART('days', NOW() - oi.obrdtvistoria) > 45 AND DATE_PART('days', NOW() - oi.obrdtvistoria) <= 60 THEN
//										CASE WHEN oi.obrpercexec >= 100.00 THEN
//											'azul'
//										ELSE
//											'amarelo'
//										END
//									 WHEN DATE_PART('days', NOW() - oi.obrdtvistoria) > 60 THEN
//										CASE WHEN oi.obrpercexec >= 100.00 THEN
//											'azul'
//										ELSE
//											'vermelho'
//										END
//								END
//
//							 ELSE
//								CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
//										CASE WHEN oi.obrpercexec >= 100.00 THEN
//											'azul'
//										ELSE
//											'verde'
//										END
//									 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
//										CASE WHEN oi.obrpercexec >= 100.00 THEN
//											'azul'
//										ELSE
//											'amarelo'
//										END
//									 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
//										CASE WHEN oi.obrpercexec >= 100.00 THEN
//											'azul'
//										ELSE
//											'vermelho'
//										END
//								END
//						END
//					 WHEN oi.stoid IN (3) THEN
//						'preto'
//					 ELSE
//						'preto'
//				END
//				as atualizacao
//			from
//				obras.preobra po
//			INNER JOIN obr as.ob rainfraestrutura oi ON oi.preid = po.preid
//			LEFT JOIN
//				(SELECT
//					supvid as supervisao, rsuid,obrid, supdtinclusao
//				FROM
//					obras.supervisao s
//				WHERE
//					supvid = (select supvid from obras.supervisao ss where ss.supstatus = 'A' and ss.obrid = s.obrid order by supvdt desc, supvid desc limit 1)) AS ov ON ov.obrid = oi.obrid
//
//			LEFT JOIN
//				obras.historicoparalisacao hpr ON hpr.supvidparalisacao = ov.supervisao AND hpr.hprdtstatus = 'A'
//			LEFT JOIN
//				obras.tipoparalisacao tpl ON tpl.tplid = hpr.tplid
//			where
//				po.preid = ".$preid;

	$sql = "SELECT
				CASE
					WHEN ov.supdtinclusao IS NOT NULL AND (doc.esdid IN (".OBR_ESDID_CONCLUIDA.",".OBR_ESDID_CONTRATO_CANCELADO.",".OBR_ESDID_CONVENIO_CANCELADO.") OR (doc.esdid IN (".OBR_ESDID_PARALISADA.") AND tpl.tplid IN (3))) THEN
						CASE WHEN DATE_PART('days', NOW() - ov.supdtinclusao) <= 45 THEN
								CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'preto'
									ELSE 'preto'
								END
							WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 45 AND DATE_PART('days', NOW() - ov.supdtinclusao) <= 60 THEN
								CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'preto'
									ELSE 'preto'
								END
							WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 60 THEN
								CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'preto'
									ELSE 'preto'
								END
							ELSE 'preto'
						END
					WHEN ov.supdtinclusao IS NOT NULL AND doc.esdid NOT IN (".OBR_ESDID_CONCLUIDA.",".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.",".OBR_ESDID_EM_LICITACAO.",".OBR_ESDID_CONTRATO_CANCELADO.",".OBR_ESDID_CONVENIO_CANCELADO.",".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") THEN
						CASE
							WHEN DATE_PART('days', NOW() - ov.supdtinclusao) <= 45 THEN
								CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
								ELSE 'verde'
								END
							 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 45 AND DATE_PART('days', NOW() - ov.supdtinclusao) <= 60 THEN
								CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
								ELSE 'amarelo'
								END
							 WHEN DATE_PART('days', NOW() - ov.supdtinclusao) > 60 THEN
								CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
								ELSE 'vermelho'
								END
							 ELSE 'preto'
						END
					WHEN doc.esdid IN (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") THEN
						CASE
							WHEN oi.obrdtvistoria IS NOT NULL THEN
								CASE
									WHEN DATE_PART('days', NOW() - oi.obrdtvistoria) <= 45 THEN
										CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
										ELSE 'verde'
										END
									 WHEN DATE_PART('days', NOW() - oi.obrdtvistoria) > 45 AND DATE_PART('days', NOW() - oi.obrdtvistoria) <= 60 THEN
										CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
										ELSE 'amarelo'
										END
									 WHEN DATE_PART('days', NOW() - oi.obrdtvistoria) > 60 THEN
										CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
										ELSE 'vermelho'
										END
								END

							 ELSE
								CASE
									WHEN DATE_PART('days', NOW() - obrdtinclusao) <= 45 THEN
										CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
										ELSE 'verde'
										END
									 WHEN DATE_PART('days', NOW() - obrdtinclusao) > 45 AND DATE_PART('days', NOW() - obrdtinclusao) <= 60 THEN
										CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
										ELSE 'amarelo'
										END
									 WHEN DATE_PART('days', NOW() - obrdtinclusao) > 60 THEN
										CASE WHEN oi.obrpercentultvistoria >= 100.00 THEN 'azul'
										ELSE 'vermelho'
										END
								END
						END
					 WHEN doc.esdid IN (".OBR_ESDID_CONCLUIDA.") THEN 'preto'
					 ELSE 'preto'
				END
				as atualizacao
			from
				obras.preobra po
			INNER JOIN obras2.obras 		oi  ON oi.preid  = po.preid
			INNER JOIN workflow.documento 		doc ON doc.docid = oi.docid
			LEFT  JOIN
				(
				SELECT
					supid as supervisao, rsuid,obrid, supdtinclusao
				FROM
					obras2.supervisao s
				WHERE
					supid = (SELECT supid FROM obras2.supervisao ss
						  WHERE ss.supstatus = 'A' AND ss.obrid = s.obrid
						  ORDER BY supdata DESC, supid DESC LIMIT 1)
				) AS ov ON ov.obrid = oi.obrid
			LEFT  JOIN obras2.historicoparalisacao 	hpr ON hpr.supidparalisacao = ov.supervisao AND hpr.hprdtstatus = 'A'
			LEFT  JOIN obras2.tipoparalisacao 	tpl ON tpl.tplid = hpr.tplid
			where
				po.preid = $preid";

	return $db->pegaUm($sql);

}

function geraAnexoEscolas($dados, $dopid){
	global $db;

	ob_clean();

	$html =  '<html>
					<head>
					<style type="">
						.fot{
							font-family: arial black;
							font-size: 17px;
							text-align: center;
							}
						.lista{
							font-size: 11px;
							padding: 3px;
							border-top: 2px solid #000;
							border-collapse: collapse;
						}
						.lista1{
							font-size: 11px;
							padding: 3px;
							border-top: 2px solid #000;
							border-collapse: collapse;
						}
						table.lista td{
							border-style: solid;
							border-width: 1px;
							border-color: #000;
							border-collapse: collapse;
						}
						.folha {
					    	page-break-after: always;
						}
						@media print {.notprint { display: none } .div_rolagem{display: none} }
						@media screen {.notscreen { display: none; }

						.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}

					</style>
					</head>
					<body>
						<table width="100%" align="center" cellspacing="0" cellpadding="0">
							<tr>
								<td>
									<table width="100%" class="lista" align="center" cellspacing="1" cellpadding="4">
										<tr>
											<th style="text-align: center">UF</th>
											<th style="text-align: center">MUNICÍPIO</th>
											<th style="text-align: center">ESCOLA</th>
											<th style="text-align: center">CÓDIGO INEP</th>
											<th style="text-align: center">SUBAÇÃO</th>
											<th style="text-align: center">ITEM</th>
											<th style="text-align: center">QUANTIDADE</th>
										</tr>';
										foreach( $dados as $dado ){
											$html .= '<tr>
														<td>'.$dado['uf'].'</td>
														<td>'.$dado['entidade'].'</td>
														<td>'.$dado['escola'].'</td>
														<td>'.$dado['codinep'].'</td>
														<td>'.$dado['subacao'].'</td>
														<td>'.$dado['item'].'</td>
														<td>'.$dado['quantidade'].'</td>
													</tr>';
										}
									$html .= '</table>
								</td>
							</tr>
						</table>
					</body>
				</html>';

	$http = new RequestHttp();
	$response = $http->toPdf( utf8_encode($html) );

	$fp = fopen(APPRAIZ . 'arquivos/par/anexo_escolas_'.date('Y').'-'.$dopid . '.pdf', "w");
	if ($fp) {
	  stream_set_write_buffer($fp, 0);
	  fwrite($fp, $response);
	  fclose($fp);
	}

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arrCampos = array();
	$file = new FilesSimec("documentopar", $arrCampos, "par");
	$file->setMover(APPRAIZ . 'arquivos/par/anexo_escolas_'.date('Y').'-'.$dopid . '.pdf', 'pdf', false);
	$arqid = $file->getIdArquivo();

	$sql = "UPDATE par.documentopar SET arqid = ".$arqid." WHERE dopid = ".$dopid."; ";
	$db->executar($sql);

	if($arqid){
		$db->commit();
		return 'sucesso';
	} else {
		$db->rollback();
		return 'erro';
	}
}

function verificaFormaExecucaoDiligenciaCondicional($sbaid) {
	global $db;

	$sql = "SELECT frmid FROM par.subacao s WHERE s.frmid IN (14, 15) AND s.sbaid = ".$sbaid;
	$teste = $db->pegaUm($sql);

	if( $teste ){
		return true;
	} else {
		return false;
	}
}

function enviaEmailMI( $terid, $arrObras ){
	global $db;


	$arDadosUsuarios = array(0 => array('nome'=>'SIMEC', 'usuemail'=>$_SESSION['email_sistema']));
	//$arDadosUsuarios = $db->carregar( $sql );
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

	$cc  = "";
	$cco = "";

	$assunto  = "MEC/FNDE - PAR com subação(ões) em diligência - ";

	$conteudo = '<p>Tendo em vista à validação do Termo de Compromisso por parte do(a) Prefeito(a) para a à implantação de creches em metodologia inovadora (MI) o município disponibiliza o(s) terreno(s) previamente aprovado(s) ou adequado(s) conforme exigências do FNDE, descrito na Resolução CD/FNDE nº 25 de 14 de Junho de 2013:</p>';

	$conteudo .= '<p>';
	foreach( $arrObras as $obra ){
		$conteudo .= 'Município - UF: '.$obra['local'].'<br/>
					  Obra:	('.$obra['obrid'].') '.$obra['nome'].'<br/><br/>';
	}
	$conteudo .= '</p>';

	$conteudo .= '<p>Por favor acesse o menu “Principal_>Metodologias Inovadoras-> Aprovação de Terrenos” no SIMEC(simec.mec.gov.br) e cadastre a aprovação dos terrenos disponíveis para construção.</p>

				<p>Caso não possua acesso ao sistema SIMEC, entre em contato com o FNDE no fone (61)9999-9999.</p>';

	foreach($arDadosUsuarios as $dados){
		$resultado = enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
	}
	return $resultado;
}

function enviaEmailProtocoloReprogramacao( $hsrid = false, $tipo = 'S', $dopid = null){
	
	$db = new cls_banco ();
	$inuid 			= $_SESSION['par']['inuid'];

	if( ! $hsrid)
	{
		
		return false;
	}
	
	$sqlDados = " SELECT  = $hsrid";

	if( !empty($inuid) ){

		$sql = "SELECT
					iu.itrid,
				CASE WHEN iu.itrid = 2 THEN
					iu.muncod
				WHEN
					iu.itrid = 1 THEN
					iu.estuf
				END as filtro
				FROM
					par.instrumentounidade iu
				WHERE
					inuid = {$inuid}
		";
		$result = $db->pegaLinha($sql);
		$itrid = $result['itrid'];
		$filtro = $result['filtro'];


		if( ($itrid == 2) && ($filtro)  )
		{
			
			$sqlEmail = "SELECT
					distinct ent.entemail as email
				FROM
					par.entidade ent
				INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
				INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
				WHERE
					ent.dutid in( 7,2)
				and ent.entstatus = 'A'
				AND
					mun.muncod in ( '{$filtro}' )
				AND
					ent.entemail <> ''
				AND
					ent.entemail is not null
				";
		}
		else if( ($itrid == 1) && ($filtro))
		{
			$sqlEmail = "
			SELECT
				distinct ent.entemail as email
			FROM
				par.entidade ent
			INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
			INNER JOIN territorios.estado est on est.estuf = ent2.estuf

			WHERE
			ent.entstatus='A'
			AND
			ent.dutid in( 9,10)
			AND
			ent2.estuf in ( '{$filtro}' )
			AND
			ent.entemail <> ''
			AND
			ent.entemail is not null
			";
		}
	
		$resultEmail = $db->carregar($sqlEmail);

		$resultEmail = ($resultEmail ) ? $resultEmail  : Array();

		if(count($resultEmail) > 0)
		{
			foreach($resultEmail as $k => $v)
			{
				$arrEmail[] = $v['email'];
			}
		}
		else
		{
			return false;
		}
		
		$sql = "SELECT 
					hsrprotocolo,
					to_char(hsrdata, 'DD/MM/YYYY HH24:MI') as data_solicitacao,
					usucpf || ' - ' || usunome as nome_usuario,
					CASE WHEN hsrtipo = 'S' THEN
						array_to_string( 
							array(SELECT par.retornacodigosubacao(s.sbaid) || ' - ' ||  sbadsc  FROM par.historicosolicitacaoreprogramacaosubacao hrs
								INNER JOIN par.subacao s ON s.sbaid = hrs.sbaid where hsr.hsrid = hrs.hsrid )
								, '</br>' 
						) 
					ELSE
						''
					END
					as subacoes,
					hsrjustificativa as justificativa
				
				FROM par.historicosolicitacaoreprogramacao hsr
				INNER JOIN seguranca.usuario u ON hsr.hsrcpf = u.usucpf where hsr.hsrid = {$hsrid}
		";
		
		$dadosProtocolo = $db->carregar($sql);
		
		$dadosProtocolo = (is_array($dadosProtocolo[0])) ? $dadosProtocolo[0] : Array();
		
		if( ! count($dadosProtocolo))
		{
			return false;
		}
		
		if($tipo == 'P')
		{
			$sql = "
				SELECT
					CASE WHEN dp2.dopano::boolean THEN
					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
				ELSE
					dp.dopnumerodocumento::text
				END
					as ndocumento
				FROM 
					par.documentopar dp
	
				LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
				WHERE dp.dopid = {$dopid}";
				
			$numTermo = $db->pegaUm($sql);
			
			
			
			$strMensagem = "<pre><p style=\"text-align: justify;\"> 
<b><center>Protocolo {$dadosProtocolo['hsrprotocolo']}</center></b><br>
<b><center> Solicitação de Prorrogação de Prazo </center></b><center>
<table border=\"1\">
					<tr>
						<td>
							<b>Data/Hora</b>
						</td>
						<td>
							<b>Usuário</b>
						</td>
						<td>
							<b>Termo</b>
						</td>
						<td>
							<b>Justificativa</b>
						</td>
					</tr>
					<tr>
						<td>
							 {$dadosProtocolo['data_solicitacao']}
						</td>
						<td>
							 {$dadosProtocolo['nome_usuario']}
						</td>
						<td>
							{$numTermo}
						</td>
						<td>
							 {$dadosProtocolo['justificativa']}
						</td>
					</tr>
				</table>
			</center>
Atenciosamente,<br>
Equipe do PAR
			</p></pre>";
		}
		else
		{
			$strMensagem = "<pre><p style=\"text-align: justify;\"> 
<b><center>Protocolo {$dadosProtocolo['hsrprotocolo']}</center></b><br>
<b><center>Solicitação de Reprogramação</center></b><center>
<table border=\"1\" cellspacing=\"0\" cellpadding=\"1\" >
					<tr>
						<td>
							<b>Data/Hora</b>
						</td>
						<td>
							<b>Usuário</b>
						</td>
						<td>
							<b>Subações</b>
						</td>
						<td>
							<b>Justificativa</b>
						</td>
					</tr>
					<tr>
						<td>
							 {$dadosProtocolo['data_solicitacao']}
						</td>
						<td>
							 {$dadosProtocolo['nome_usuario']}
						</td>
						<td>
							 {$dadosProtocolo['subacoes']}
						</td>
						<td>
							 {$dadosProtocolo['justificativa']}
						</td>
					</tr>
				</table>
			</center>
Atenciosamente,<br>
Equipe do PAR
			</p></pre>";
			
		}	
		
			
		$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
			
		$strMensagem = html_entity_decode($strMensagem);

		$strAssunto = "Protocolo {$dadosProtocolo['hsrprotocolo']} -solicitação de reprogramação";
		
		if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" )
		{
			return true;
		}
		elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
		{
			$strEmailTo = array($_SESSION['email_sistema']);
			enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			return true;
		}
		else
		{
			$strEmailTo = $arrEmail;
			enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			return true;
		}


		} else {
		
			return false;
		}

	global $db;
	die();
}

function enviaEmailReprogramacao( $tipo, $dopid, $prazo = '' , $subacoes = array() ){
	global $db;

	$cc  = "";
	$cco = "";

	$sql = "SELECT
				CASE WHEN iu.itrid = 1 THEN 'estado' ELSE 'município' END as entidade,
				CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE m.mundescricao || '/' || m.estuf END as local,
				dp.dopnumerodocumento
			FROM
				par.documentopar dp
			INNER JOIN par.processopar prp ON prp.prpid = dp.prpid and prp.prpstatus = 'A'
			INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
			LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
			WHERE
				dopid = ".$dopid;

	$dados = $db->pegaLinha($sql);

	extract($dados);

	$assunto  = "Solicitação de Reprogramação";

	$conteudo = '<p>O '.$entidade.' '.$local.' solicitou reprogramação do termo '.$dopnumerodocumento.'.</p>';

	if( $tipo == 'subacao' ){
		$conteudo .= '<p>';
		foreach( $subacoes as $sub ){
			$conteudo .= 'Subação: '.$sub['codigo'].' - '.$sub['nome'].' (ano '.$sub['ano'].')<br/>';
		}
		$conteudo .= '</p>';
	} else {
		$conteudo .= '<p>Prazo solicitado: '.$prazo.'</p>';
	}

	$email = 'par@fnde.gov.br';

	$resultado = enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $email, $assunto, $conteudo, $cc, $cco );
	return $resultado;
}

function enviaEmailLiberacaoReprogramacao( $inuid, $subacoes ){
	global $db;

	$sql = "SELECT
				itrid, estuf, muncod
			FROM
				par.instrumentounidade where inuid = ".$inuid;

	$dadosEntidade = $db->pegaLinha( $sql );

	if( $dadosEntidade['itrid'] == 1 ){ //Estado

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				WHERE
					ur.pflcod IN (".PAR_PERFIL_EQUIPE_ESTADUAL.", ".PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO.") AND
					ur.rpustatus = 'A' AND
					ur.estuf = '".$dadosEntidade['estuf']."'";

	} else { //Município

		$sql = "SELECT
					usunome,
					usuemail
				FROM
					par.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				WHERE
					ur.pflcod IN (".PAR_PERFIL_EQUIPE_MUNICIPAL.", ".PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO.", ".PAR_PERFIL_PREFEITO.") AND
					ur.rpustatus = 'A' AND
					ur.muncod = '".$dadosEntidade['muncod']."'";

	}

	$arDadosUsuarios = $db->carregar( $sql );
	$arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

	$cc  = "";
	$cco = "";

	$assunto = 'Liberação de Reprogramação';

	$conteudo = '<p>A(s) seguinte(s) subação(ões) foi(ram) liberada(s) para reprogramação:</p>';

	$conteudo .= '<p>';
	foreach( $subacoes as $sub ){
		$conteudo .= 'Subação: '.$sub['codigo'].' - '.$sub['nome'].' (ano '.$sub['ano'].')<br/>';
	}
	$conteudo .= '</p>';

	foreach($arDadosUsuarios as $dados){
		enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco );
	}
	return true;
}

function verificaBloqueioObras( $inuid, $tooid = 1 ){
	global $db;

	$arrRetorno = array();
	$entidade = $db->pegaLinha("SELECT itrid, CASE WHEN itrid = 1 THEN estuf ELSE muncod END as entidade, CASE WHEN itrid = 1 THEN 'estuf' ELSE 'muncod' END as tipoentidade FROM par.instrumentounidade WHERE inuid = ".$inuid);
	$esfera = $entidade['tipoentidade'] == 'estuf' ? 'E' : 'M';
	if( $esfera == 'E' || $_SESSION['par']['estuf'] == 'DF' ){
		$sql = "SELECT estado,stoid, count(1) as qtdbloqueio FROM (
						SELECT
							o.obrid,
							o.estuf AS estado,
							o.situacaoobra as stoid,
							case when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and desterminodeferido is not null and desterminodeferido >= now() then 0
								when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 then 1
								else 0 end as bloqueiolicitacao,
							case when o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") and o.diasultimaalteracao > 60 then 1 else 0 end as bloqueioexecucaoparalisada
							--count(o.obrid) as qtdobras

						FROM
							obras2.vm_obras_situacao_estadual o
						WHERE
							o.inuid = {$inuid}
							AND o.tooid = $tooid
							AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
					) t where bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1
					group by estado, stoid";

		$dadosObra = $db->pegaLinha($sql);
	} else {
		$sql = "SELECT estado,stoid, count(1) as qtdbloqueio FROM (
						SELECT
							o.obrid,
							o.estuf AS estado,
							o.situacaoobra as stoid,
							case when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and desterminodeferido is not null and desterminodeferido >= now() then 0
								when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 then 1
								else 0 end as bloqueiolicitacao,
							case when o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") and o.diasultimaalteracao > 60 then 1 else 0 end as bloqueioexecucaoparalisada,
							obrnome as obrdesc,
							muncod,
							estuf
						FROM
							obras2.vm_obras_situacao_estadual o
						WHERE
							o.inuid = {$inuid}
							AND o.tooid = $tooid
							AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
					) t where bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1
					group by estado, stoid";
		$dadosObra = $db->carregar($sql);
	}

	$bloqueioObra = false;
	$arrTexto = array();
	$texto = '';
	$obra = '';
	if( $esfera == 'E' || $_SESSION['par']['estuf'] == 'DF' ){
		if( $dadosObra['qtdbloqueio'] > 0 ){
			$sql = "SELECT * FROM (
							SELECT
								o.*,
								o.estuf AS estado,
								o.situacaoobra as stoid,
								case when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and desterminodeferido is not null and desterminodeferido >= now() then 0
									when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 then 1
									else 0 end as bloqueiolicitacao,
								case when o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") and o.diasultimaalteracao > 60 then 1 else 0 end as bloqueioexecucaoparalisada
								--count(o.obrid) as qtdobras

							FROM
								obras2.vm_obras_situacao_estadual o
							WHERE
								o.inuid = {$inuid}
								AND o.tooid = $tooid
								AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
						) t where bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1
						order by bloqueiolicitacao desc, bloqueioexecucaoparalisada desc
			";
			$obras = $db->carregar($sql);
			foreach( $obras as $obra ) {
				if ($obra['bloqueiolicitacao'])
				{
					$texto =  '<font color="red">Obra ID ' . $obra['obrid'] . ' com licitação não concluída após ' . $obra['diasinclusao'] . ' dia(s) - ' . $obra['obrnome'] . '</font><br/>';
					array_push($arrTexto, $texto);
				}
				elseif ($obra['bloqueioexecucaoparalisada'])
				{
					$texto = '<font color="red">Obra ID ' . $obra['obrid'] . ' com ' . $obra['diasultimaalteracao'] . ' dia(s) sem atualização - ' . $obra['obrnome'] . '</font><br/>';
					array_push($arrTexto, $texto);
				}
			}
			$obra = 'Monitoramento de obras 2.0';
			$bloqueioObra = true;
		} else {
			$texto = '<font color="green">Sem pendências.</font>';
			array_push($arrTexto, $texto);
			$bloqueioObra = false;
			$obra = 'Monitoramento de obras 2.0';
		}
	} else {
		if( sizeof($dadosObra) > 0 && is_array($dadosObra) ){
			foreach( $dadosObra as $obra ) {
				if ($obra['diaslicitacao'])
				{
					$texto = '<font color="red">Obra ID ' . $obra['obrid'] . ' com licitação não concluída após ' . $obra['diaslicitacao'] . ' dia(s) - ' . $obra['obrdesc'] . '</font><br/>';
					array_push($arrTexto, $texto);
				}
				elseif ($obra['diasatualizacao'])
				{
					$texto = '<font color="red">Obra ID ' . $obra['obrid'] . ' com ' . $obra['diasatualizacao'] . ' dia(s) sem atualização - ' . $obra['obrdesc'] . '</font><br/>';
					array_push($arrTexto, $texto);
				}
			}
			$bloqueioObra = true;
			$obra = 'Monitoramento de obras 1.0';
		} else {
			$texto = '<font color="green">Sem pendências no Monitoramento de obras 1.</font><br/>';
			array_push($arrTexto, $texto);
			$bloqueioObra = false;
			$obra = 'Monitoramento de obras 1.0';
		}

		$sql = "SELECT * FROM (
					SELECT
						o.*,
						o.estuf AS estado,
						o.situacaoobra as stoid,
						case when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and desterminodeferido is not null and desterminodeferido >= now() then 0
							when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and coalesce(o.diasprimeiropagamento, o.diasinclusao) > 540 then 1
							else 0 end as bloqueiolicitacao,
						case when o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.") and o.diasultimaalteracao > 60 then 1 else 0 end as bloqueioexecucaoparalisada,
						obrnome,
						muncod,
						estuf
					FROM
						obras2.vm_obras_situacao_municipal o
					WHERE
						o.muncod = '{$entidade['entidade']}'
						AND o.tooid = $tooid
						AND o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_PARALISADA.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.")
				) t where bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1
				order by bloqueiolicitacao desc, bloqueioexecucaoparalisada desc";
		$dadosObra = $db->carregar($sql);
		if( sizeof($dadosObra) > 0 && is_array($dadosObra) ){
			foreach( $dadosObra as $obra ) {
				if ($obra['bloqueiolicitacao'])
				{
					$texto = '<font color="red">Obra ID ' . $obra['obrid'] . ' com licitação não concluída após ' . $obra['diasinclusao'] . ' dia(s) - ' . $obra['obrnome'] . '</font><br/>';
					array_push($arrTexto, $texto);
				}
				elseif ($obra['bloqueioexecucaoparalisada'])
				{
					$texto = '<font color="red">Obra ID ' . $obra['obrid'] . ' com ' . $obra['diasultimaalteracao'] . ' dia(s) sem atualização - ' . $obra['obrnome'] . '</font><br/>';
					array_push($arrTexto, $texto);
				}
			}
			$obra = 'Monitoramento de obras 2.0';
			$bloqueioObra = true;
		} else {
			$texto = '<font color="green">Sem pendências no Monitoramento de obras 2.0.</font>';
			array_push($arrTexto, $texto);
			$bloqueioObra = false;
			$obra = 'Monitoramento de obras 2.0';
		}
	}

	$arrRetorno = array('pendencia' => $arrTexto, 'boBloqueia' => $bloqueioObra, 'sistema' => $obra);
	return $arrRetorno;
}

function cabecalhoProcesso($arProcesso){
	global $db;
	
	$processo = substr($arProcesso['processo'],0,5) . ".".substr($arProcesso['processo'],5,6)."/".substr($arProcesso['processo'],11,4) . "-".substr($arProcesso['processo'],15,2);
	
	$html = '
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="SubTituloDireita" colspan="4"><center><b>Dados do Processo</b></center></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Processo:</b></td>
			<td width="25%">'.$processo.'</td>
			<td class="SubTituloDireita" width="25%"><b>Ano:</b></td>
			<td width="25%">'.$arProcesso['ano'].'</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"><b>Tipo:</b></td>
			<td width="25%">'.$arProcesso['tipo'].'</td>
		</tr>
	</table>';
	
	return $html;
}

/*
 * Funções de pré-ação de rowklfow para envio para diligência e envio para reformulação
 * */

function form_sugestaoTrocaPtoid(){
	
	global $db;
	extract( $_POST );
	
	$sql = "SELECT ptoid FROM obras.preobra WHERE preid = $preid";
	
	$ptoObra = $db->pegaUm( $sql );
	
	$sql = "SELECT ptodescricao FROM obras.pretipoobra WHERE ptoid = $ptoObra";
	
	$ptoObraDsc = $db->pegaUm( $sql );
	
	$arrAedidMi = Array(OBRA_AEDID_ENVIAR_PARA_REFORMULACAO_MI,
						OBRA_AEDID_RETORNAR_PARA_EM_REFORMULACAO_MI);
	
	$inOrNotInMi = "";
	
	if( in_array($aedid, $arrAedidMi) ){
		$inOrNotInMi = "AND pto.ptoid IN (".OBRA_TIPO_ESCOLA_PROINFANCIA_B_MI.",".
                                                    OBRA_TIPO_ESCOLA_PROINFANCIA_C_MI.",".
                                                    OBRA_TIPO_ESCOLA_PROINFANCIA_B_MI_EMENDA.",".
                                                    OBRA_TIPO_ESCOLA_PROINFANCIA_C_MI_EMENDA.")";
	}
	
	$sql = "SELECT
                    pto.ptoid as codigo,
                    pto.ptodescricao as descricao
                FROM
                    obras.pretipoobra pto
                INNER JOIN obras.pretipoobra pto2 ON pto2.ptostatus = pto.ptostatus AND pto2.ptoclassificacaoobra = pto.ptoclassificacaoobra AND ( pto2.tooid = pto.tooid OR pto.tooid = 11 )
                INNER JOIN obras.preobra pre ON pre.ptoid = pto2.ptoid
                WHERE
                        pre.preid = $preid
                        $inOrNotInMi ";
        
        #Pendecia
        $arrPendencia = array();
        #Verifica se possui pendencia de vistoria
        $sqlVistoria = "SELECT
                            count(supid)
			FROM
                            obras2.obras o
			INNER JOIN obras2.supervisao s ON o.obrid = s.obrid
			WHERE
                            o.preid = $preid
                            AND  s.supstatus = 'A'";
	$num_vistorias = $db->pegaUm($sqlVistoria);
        if($num_vistorias > 0) {
            $arrPendencia[] = 'Esta obra não pode ser reformulada, pois existem vistorias cadastradas.<br />';
	}
        
        #Verifica percentual
        $sqlPercentual = "SELECT
                    COALESCE(obrpercentultvistoria, 0) as percexec,
                    obrid,
                    doc.esdid,
                    doc.docid
                FROM
                    obras2.obras obr
                INNER JOIN workflow.documento doc ON doc.docid = obr.docid
                WHERE
                    preid = $preid
                    AND obrstatus = 'A'";
	$arPercentual = $db->pegaLinha($sqlPercentual);
        if($arPercentual['percexec'] > 0) {
            $arrPendencia[] =   'Esta obra não pode ser reformulada, pois a obra esta com '.$arPercentual['percexec'].'% de execução.<br />';
	}
?>
	<input type="hidden" name="preid" value="<?=$preid ?>"/>
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
            <?php
            if (count($arrPendencia)> 0) { 
            ?>
            <tr>
                <td>
                <strong>Está obra possui pendência(s):</strong><br />
                <?php
                    foreach ($arrPendencia as $pendencia) {
                        echo " - {$pendencia}";
                    }
                ?>
                <strong>Deseja continuar reformulação?</strong><br />
                <input type="checkbox" class="required" name="continuar" value="S" /> Sim, de acordo com a justificativa abaixo:
                </td>
            </tr>  
            <tr>
                <td><strong>Justificativa:</strong><br />
                    <textarea name="perjustificativa" id="perjustificativa" class="required" cols="70" rows="5"></textarea>
                </td>
            </tr>  
            <?php   
            }
            ?>
            <tr>
                <td class="SubTituloDireita" colspan="2"><center><b>Escolha o(s) possivel(eis) tipo(s) de obra para a reformulação desta obra:</b></center></td>
            </tr>
            <tr>
                <td>
                <?php 
                    combo_popup( "ptoids", $sql, "Selecione o(s) Tipos de obra(s)", "400x500", 0, array(), "", "S", false, false, 5, 300, '', '', '', '', array(array('codigo' => $ptoObra, 'descricao' => $ptoObraDsc)));
                ?>
                </td>
            </tr>
	</table>
	<script>
            jQuery(document).ready(function(){
                jQuery("input[name=continuar]").change(function(){
                    if (jQuery(this).val()==="N") {
                        jQuery('#perjustificativa').removeClass("required");
                    }else{
                        jQuery('#perjustificativa').addClass("required");
                    }
                });
                
                jQuery("#ptoids").addClass("required");

                jQuery("#ptoids").attr("required","required");

                jQuery("#formsugestaoTrocaPtoid").validate();

                <?php if( $ptoObra ){?>
                jQuery('#ptoids').find('[value=""]').html('<?=$ptoObraDsc ?>');
                jQuery('#ptoids').find('[value=""]').val('<?=$ptoObra ?>');
                <?php }else{?>
                jQuery('#ptoids').find('[value=""]').remove();
                <?php }?>
            });
	</script>
<?php 
}

function sugestaoTrocaPtoid(){
	
	global $db;
	
	extract($_POST);
	
	if( $preid ){
            
		$perjustificativa = substr($perjustificativa, 0, 255);

        # Insere a justificativa de reformulacao
        $sql = "INSERT INTO obras.preobrareformulacao(perjustificativa, preid, usucpf)
                	VALUES ('{$perjustificativa}', $preid, '{$_SESSION['usucpf']}');";
            
        if( $ptoids[0] != '' ){
			$sql .= "UPDATE obras.pretipoobradiligencia SET todstatus = 'I', usucpfinativa = '{$_SESSION['usucpf']}' WHERE preid = $preid;";
			foreach( $ptoids as $ptoid ){
				$sql .= "INSERT INTO obras.pretipoobradiligencia( preid, ptoid, usucpf )
                         VALUES( $preid, $ptoid, '{$_SESSION['usucpf']}');";
			}
            $db->executar( $sql );
            $db->commit();
		}		
            
		$retorno = Array('boo' => true, 	'msg' => '');
	}else{
		$retorno = Array('boo' => false, 	'msg' => 'Operação não pôde ser realizada.');
	}
	
	$retorno = simec_json_encode($retorno);
	echo $retorno;
}

/*
 * Fim fuções pré-ação
 * */

/* verificaItemEmReformulacao();
 * Retorna true se o item estiver em reformulação/reprogramação
 * */
function verificaItemEmReformulacao( $icoid ){
	
	global $db;
	
	$sql = "SELECT 
				TRUE
			FROM
				par.subacaoitenscomposicao ico
			INNER JOIN par.subacaodetalhe sbd ON sbd.sbaid = ico.sbaid AND sbd.sbdano = ico.icoano
			INNER JOIN par.documentoparreprogramacaosubacao dps ON dps.sbdid = sbd.sbdid AND dps.dpsstatus = 'A'
			INNER JOIN par.reprogramacao rep ON rep.repid = dps.repid AND rep.repstatus = 'A'
			WHERE 
				icoid = $icoid";
	
	return $db->pegaUm($sql) == 't';
}
/* FIM - verificaItemEmReformulacao();
 * Retorna true se o item estiver em reformulação/reprogramação
 * */

/* verificaTermoEmReformulacao();
 * Retorna true se o item estiver em reformulação/reprogramação
 * */
function verificaTermoEmReformulacao( $dopid ){
	
	global $db;
	
	$sql = "SELECT 
				TRUE
			FROM
				par.documentoparreprogramacaosubacao dps  
			INNER JOIN par.reprogramacao rep ON rep.repid = dps.repid AND rep.repstatus = 'A'
			WHERE 
				rep.dopidoriginal = $dopid
				AND dps.dpsstatus = 'A'";
	
	return $db->pegaUm($sql) == 't';
}
/* FIM - verificaTermoEmReformulacao();
 * Retorna true se o item estiver em reformulação/reprogramação
 * */

function verificaEmendaImpositiva($inuid, $ano){
	global $db;
	
	$sql = "SELECT
			count(vede.emdid) as total
		FROM
			emenda.v_emendadetalheentidade vede
			inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
		WHERE
			vede.edestatus = 'A'
			and vede.ededisponivelpta = 'S'
			and vede.emeano >= '{$ano}'
			and enb.enbcnpj = '".pegaCnpjInuid($inuid)."'
			and vede.emetipo = 'E'
            and vede.emdimpositiva = '6'
			and vede.edeid in ( select edeid from emenda.emendapariniciativa )";
	
	$total = $db->pegaUm($sql);
	return $total;
}

function carregarPTA($inuid, $ano) {
	global $db;
	
	$acoes = "'<center><img style=\"cursor:pointer\" id=\"img_dimensao_' || ptr.ptrid || '\" src=\"/imagens/mais.gif\" 
			onclick=\"carregaEmendaDetalhePTA(this.id,\'' || ptr.ptrid || '\');\" 
			border=\"0\" /></center>'";
	
	$sql = "SELECT DISTINCT
					$acoes as acoes,
					ptr.ptrid,
					ptr.ptrexercicio,
				    enb.enbnome,
				    enb.enbcnpj,
				    enb.estuf,
				    mun.mundescricao,
				    mod.mdedescricao as resassunto,
				    doc.esdid,
				    sum(pte.pedvalor) as valorTotal,
		            esd.esddsc,
		            ptr.ptrvalorproponente, 
		            '</td></tr>
		            	<tr style=\"display:none\" id=\"listaEmendaDetalhePTA_' || ptr.ptrid || '\" >
		            		<td id=\"trV_' || ptr.ptrid || '\" colspan=8 ></td>
		            </tr>' as linha,
		            ptr.ptridpai,
		            ptr.ptrcod
				FROM 
				    emenda.planotrabalho ptr 
				    inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
					left join emenda.v_ptiniciativa pti1 on ptr.ptrid = pti1.ptrid
				    inner join emenda.entidadebeneficiada enb on ptr.enbid = enb.enbid
				    left join territorios.municipio mun on mun.muncod = enb.muncod
					inner join emenda.modalidadeensino mod on mod.mdeid = ptr.mdeid and mod.resid = ptr.resid
				    left join workflow.documento doc on ptr.docid = doc.docid 
				    left join workflow.estadodocumento esd on esd.esdid = doc.esdid
                    
				WHERE
				    ptr.ptrstatus = 'A' 
				    AND ptr.sisid = 23
				    AND ptr.ptrexercicio = " . $ano . "
				    and enb.enbcnpj = '" . pegaCnpj ( $inuid ) . "'
				    AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')
				group by
                	ptr.ptrid,
				    enb.enbnome,
				    enb.enbcnpj,
				    enb.estuf,
				    mun.mundescricao,
				    mod.mdedescricao,
				    doc.esdid,
		            esd.esddsc, 
		            ptr.ptridpai,
		            ptr.ptrcod,
		            ptr.ptrexercicio,
		            ptr.ptrvalorproponente
				ORDER BY 
					ptr.ptrcod";
	
	$arDados = $db->carregar ( $sql );
	$arDados = $arDados ? $arDados : array ();
	
	$dados_array = array ();
	foreach ( $arDados as $chave => $val ) {
		
		$sql = "select
					count(v.emdid)
				from 
					emenda.ptemendadetalheentidade pt
				    inner join emenda.v_emendadetalheentidade v on v.edeid = pt.edeid
				where
					pt.ptrid = {$val['ptrid']}
				    and v.emdimpositiva = '6'";
		$boImpositivo = $db->pegaUm($sql);
		
		if( $boImpositivo > 0 ){
			if( $ano > 2014 )	$filtroImp = ' and edi.edeid = ve.edeid';
			
			#Valor impositivo de impedimento da emenda
			$valorImpositivo = $db->pegaUm("select
													coalesce(sum(edi.edivalor), 0) as valor
												from 
													emenda.ptemendadetalheentidade pte
													inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid
												    inner join emenda.emendadetalheimpositivo edi on edi.emdid = ve.emdid and edi.edistatus = 'A' $filtroImp
												where
													pte.ptrid = {$val['ptrid']}
												    and ve.edestatus = 'A'");
			
			$pedvalor = $db->pegaUm("select sum(pedvalor) from emenda.ptemendadetalheentidade where ptrid = {$val['ptrid']}");
			$ptrvalorconcedente = (float)$pedvalor + (float)$val['ptrvalorproponente'];
			
			$val['valortotal'] = ((float)$ptrvalorconcedente - (float)$valorImpositivo);
			$val['valortotal'] = number_format($val['valortotal'], '2', ',', '.');
		}
		
		$dados_array [$chave] = array (
				"acoes" => $val ['acoes'],
				"ptrid" => $val ['ptrcod'] . "/" . $val ['ptrexercicio'],
				"enbcnpj" => $val ['enbcnpj'],
				"enbnome" => $val ['enbnome'],
				"estuf" => $val ['estuf'],
				"mundescricao" => $val ['mundescricao'],
				"resassunto" => $val ['resassunto'],
				"valorTotal" => $val ['valortotal'],
				"esddsc" => $val ['esddsc'] . $val ['linha'] 
		);
	}
	
	if ($arDados) {
		echo '<br>';
		monta_titulo ( 'Emenda(s) Aceita(s) pelo seu Município', '' );
		$cabecalho = array (
				"&nbsp;Mostrar Emendas&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",
				"Número do PTA",
				"CNPJ",
				"Órgão ou Entidade",
				"UF",
				"Município",
				"Nível de Ensino",
				"Valor Total",
				"Situação" 
		);
		
		$db->monta_lista_array ( $dados_array, $cabecalho, 500000, 20, '', 'center', '' );
	}
}

function carregaDetalheEmendaAjax($emeid, $enbid, $visivel, $ano) {
	global $db;
	
	if( $ano > 2014 )	$filtroImp = ' and edi.edeid = vede.edeid';
	
	$sql = "SELECT distinct
			    vede.emdid,
			    vede.emeid,
			    vede.gndcod||' - '||gn.gnddsc as gndcod1, 
			    vede.mapcod||' - '||map.mapdsc as modalidade,
			    vede.foncod||' - '||fon.fondsc as fonte,
			    vede.edevalor as edevalor,
			    edi.ediid,
			    ptede.edeid,
			    vede.edeid as coddetalhe,
			    vede.resid,
			    vede.entid as enbid
			FROM
				emenda.v_emendadetalheentidade vede
			    left join emenda.v_funcionalprogramatica funcp on funcp.acaid = vede.acaid and funcp.prgano = '" . $ano . "'
			    left join (Select ped.pedid, ped.edeid, ped.ptrid
			                                        From emenda.ptemendadetalheentidade ped
			                                            inner join emenda.planotrabalho ptr
			                                                ON (ptr.ptrid = ped.ptrid)
			                                        Where ptr.ptrstatus = 'A') ptede
			    	on vede.edeid = ptede.edeid
				left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
				left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
				left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A' 
				left join emenda.emendadetalheimpositivo edi on edi.emdid = vede.emdid and edi.edistatus = 'A' $filtroImp  
			WHERE
			  vede.emeid = $emeid
			  and vede.entid = $enbid
			  and vede.edestatus = 'A'
			  and vede.ededisponivelpta = 'S'
			/*  and vede.edeid not in (select pte1.edeid from emenda.planotrabalho ptr1
			    						inner join emenda.ptemendadetalheentidade pte1 on pte1.ptrid = ptr1.ptrid
			    					where ptr1.ptrstatus = 'A') */";
	//ver($sql, d);
	$arEmendaD = $db->carregar ( $sql );
	$arDados = array ();
	$html = '';
	if ($arEmendaD) {
		$html .= '<table id="tblform" class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="right">
		<thead>
		<tr>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Seleciona</strong></td>
			<td align="Center" class="title" width="20%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>GND</strong></td>
			<td align="Center" class="title" width="15%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Mod</strong></td>
			<td align="Center" class="title" width="45%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Fonte</strong></td>
			<td align="Center" class="title" width="11%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor</strong></td>
		</tr>
		</thead>';
		$total = 0;
		foreach ( $arEmendaD as $key => $valor ) {
			
			$key % 2 ? $cor = "#FFFFFF" : $cor = "";
			if( $valor['edevalor'] == '0.00' ) $visivel = 'disabled="disabled"';
			$html .= '
			  <tr bgcolor="' . $cor . '" id="tr_' . $key . '" onmouseout="this.bgColor=\'' . $cor . '\';" onmouseover="this.bgColor=\'#ffffcc\';">
			  	<td style="text-align: center;">
			  			<input type="hidden" name="resid[]" id="resid" value="' . $valor ['resid'] . '">
			  			<input type="hidden" name="enbid[]" id="enbid" value="' . $valor ['enbid'] . '">
			  			<input type="hidden" name="ediid[]" id="enbid" value="' . $valor ['ediid'] . '">
			  			<input type="hidden" name="edeid[' . $valor ['emdid'] . ']" id="edeid" value="' . $valor ['coddetalhe'] . '">
			  			<input id="emdid_' . $valor ['emdid'] . '" type="checkbox" ' .$visivel.' name="emdid[]" value="' . $valor ['emdid'] . '" onclick="somaEmendaDetalhe(this,\'' . $valor ['edevalor'] . '\', \'' . $emeid . '\');"/></td>
			  	<td style="text-align: center;">' . $valor ['gndcod1'] . '</td>
			  	<td style="text-align: center;">' . $valor ['modalidade'] . '</td>
			  	<td style="text-align: center;">' . $valor ['fonte'] . '</td>
				<td style="text-align: right;">R$ ' . number_format ( $valor ['edevalor'], 2, ',', '.' ) . '</td>
			  </tr>';
		}
	} else {
		$html .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		$html .= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	}
	$html .= '</table>';
	
	return $html;
}

/*
 * 
 * param $opc enviará todos os estados/municipios que possuem pendencia. O $opc deve ter esfera no seu parametro ($opc == 'Municipal' || $opc == 'Estadual');
 * 
 */
function bloqueioObra( $muncod = null, $estuf = null, $detalhe = null, $opc = null ){

	global $db;

    if($muncod){
        $view   = ' obras2.vm_obras_situacao_municipal o ';
        $where  = " AND o.muncod = '{$muncod}' ";
    } elseif($estuf) {
        $view   = ' obras2.vm_obras_situacao_estadual o ';
        $where  = " AND o.estuf = '{$estuf}' ";
    } elseif($opc){
    	if( $opc == 'Municipal' ){
	        $view   = ' obras2.vm_obras_situacao_municipal o ';
	        $where  = "";
    	} else {
	        $view   = ' obras2.vm_obras_situacao_estadual o ';
	        $where  = "";
    	}
    } else {
        return 'Não foi informado uma esfera. Favor verificar.';
    }

    $sql = "SELECT distinct * FROM (
                SELECT
                    o.obrid,
                    o.estuf AS estado,
                    o.situacaoobra as stoid,
                    case when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and desterminodeferido is not null and desterminodeferido >= now() then 0
                        when o.situacaoobra in (".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.") and coalesce(o.diasprimeiropagamento, o.diasinclusao) > 365 then 1
                        else 0 end as bloqueiolicitacao,
                    case when o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_EM_REFORMULACAO.") and o.diasultimaalteracao > 60 then 1 else 0 end as bloqueioexecucaoparalisada,
                    case when o.situacaoobra in (".OBR_ESDID_PARALISADA.") then 1 else 0 end as bloqueioparalisada,
                    obrnome as obrdesc,
                    muncod,
                    estuf
                FROM $view
                WHERE o.situacaoobra in (".OBR_ESDID_EM_EXECUCAO.", ".OBR_ESDID_EM_REFORMULACAO.", ".OBR_ESDID_EM_LICITACAO.", ".OBR_ESDID_EM_ELABORACAO_DE_PROJETOS.", ".OBR_ESDID_PARALISADA.")
                $where
            ) t
            where bloqueiolicitacao = 1 or bloqueioexecucaoparalisada = 1 or bloqueioparalisada = 1
            order by bloqueiolicitacao desc, bloqueioexecucaoparalisada desc, bloqueioparalisada desc";

	$dadosObra = $db->carregar($sql);
	
	if( $opc ){
		$arrObrsPendentes = array();
    	if( $opc == 'Municipal' ){
    		 if( is_array($dadosObra) ){
            	foreach( $dadosObra as $obra ) {
            		if( !in_array($obra['muncod'], $arrObrsPendentes) ){
	            		$arrObrsPendentes[] = $obra['muncod'];
            		}
            	}
    		 }
    	} else {
    		 if( is_array($dadosObra) ){
            	foreach( $dadosObra as $obra ) {
            		if( !in_array($obra['estuf'], $arrObrsPendentes) ){
	            		$arrObrsPendentes[] = $obra['estuf'];
            		}
            	}
    		 }
    	}
    	return $arrObrsPendentes;
	}
    if ($detalhe) {
        $bloqueioObra = false;
        $arrTexto = array();
        $texto = '';
        $obra = '';
        if( is_array($dadosObra) ){

            foreach( $dadosObra as $obra ) {
                if ($obra['bloqueiolicitacao'])
                {
                    $texto =  '<font color="red">Obra ID ' . $obra['obrid'] . ' - ' . $obra['obrdesc'] . ' - Motivo: Obra não entrou em execução após 365 dias do primeiro pagamento.</font><br/>';
                    array_push($arrTexto, $texto);
                }
                elseif ($obra['bloqueioexecucaoparalisada'])
                {
                    $texto =  '<font color="red">Obra ID ' . $obra['obrid'] . ' - ' . $obra['obrdesc'] . ' - Motivo: Obra sem vistoria ou em reformulação a mais de 60 dias.</font><br/>';
                    array_push($arrTexto, $texto);
                }
                elseif ($obra['bloqueioparalisada'])
                {
                    $texto =  '<font color="red">Obra ID ' . $obra['obrid'] . ' - ' . $obra['obrdesc'] . ' - Motivo: Obra paralisada.</font><br/>';
                    array_push($arrTexto, $texto);
                }
            }
            $bloqueioObra = true;
        }
        return $arrTexto;
    } else {
		if( is_array($dadosObra) ){
			return true;
		}
    }

    return false;
}

function buscaDadosTermo( $dopid = null ){
	
	set_time_limit(0);
	ini_set("memory_limit", "12000M");
	
	global $db;
	
	$str = "";
	if( $dopid ){
		$str = " AND d.dopid = ".$dopid." ";
	}
	
	$sql = "SELECT 
				d.dopid, 
				d.mdoid 
			FROM 
				par.documentopar d 
				INNER JOIN par.modelosdocumentos md ON md.mdoid = d.mdoid
			WHERE 
				md.tpdcod IN (102,21)
				{$str}
			ORDER BY 
				d.mdoid, d.dopid";
	$dados = $db->carregar($sql);
	
	$sql = "";
	
	foreach( $dados as $documento ){
	
		$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&l');
		
		$doptexto = pegaTermoCompromissoArquivo($documento['dopid'], '');
		
		$texto = str_ireplace($procurar, '', $doptexto);
		
		if( $documento['mdoid'] == 3 || $documento['mdoid'] == 16 ){
			$texto = trim(substr($texto, stripos($texto, 'S INICIAL:') + 10, 11));
		} else {
			$texto = trim(substr($texto, stripos($texto, 'S INICIAL:') + 10, 7));
		}
	
		if (is_numeric(substr($texto, 0, 2))) {
			$sql .= "UPDATE par.documentopar SET dopdatainiciovigencia = '".$texto."' WHERE dopid = ".$documento['dopid']."; ";
		}
		
	}
	
	
	foreach( $dados as $documento ){
	
		$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&l');
		$doptexto = pegaTermoCompromissoArquivo($documento['dopid'], '');
		$texto = str_ireplace($procurar, '', $doptexto);
		
		if( $documento['mdoid'] == 3 || $documento['mdoid'] == 16 ){
			$texto = trim(substr($texto, stripos($texto, 'S FINAL') + 8, 11));
		} else {
			$texto = trim(substr($texto, stripos($texto, 'S FINAL') + 8, 7));
		}
		
		if (is_numeric(substr($texto, 0, 2))) {
			$vig = "";
			$vig = $db->pegaUm("SELECT dopdatafimvigencia FROM par.documentopar WHERE dopid = ".$documento['dopid']);
			
			if( $vig != $texto ){
				$sql .= "UPDATE par.documentopar SET dopdatafimvigencia = '".$texto."' WHERE dopid = ".$documento['dopid']."; ";
			}
			
	///		if(!$vig){
	///			$sql .= "UPDATE par.documentopar SET dopdatafimvigencia = '".$texto."' WHERE dopid = ".$documento['dopid']."; ";
	///		}
		}
		
	}
	
	
	
	foreach( $dados as $documento ){
		$valor = "";
		
		$doptexto = pegaTermoCompromissoArquivo($documento['dopid'], '');
		$texto = $doptexto;
	
		if( $documento['mdoid'] == 28 || $documento['mdoid'] == 32 ){
			// A partir de TOTAL GERAL
			$texto = trim(substr($texto, stripos($texto, 'TOTAL GERAL') + 4, 500));
		
			// Até a próxima </tr>
			$texto = trim(substr($texto, 0, stripos($texto, '/tr&gt;')));
	
			// Entre R$ e </tr>
			//$texto = trim(substr($texto, strripos($texto, '&lt;td')+4, -5));
			$texto = trim(substr($texto, stripos($texto, 'R$')+3, 20));
			
			$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&lt;tr&gt;', '&lt;/tr&gt;', '&lt;TD&gt;', '&lt;/TD&gt;', '&lt;TD', '/TD&gt;', 'TEXT-ALIGN: CENTER', 'TEXT-ALIGN: RIGHT', 'TEXT-ALIGN: LEFT', '&gt;', '&lt;', 'style=', '&quot;;&quot;', '&amp;nbsp;', 'td', 'R$', '/span', 'span', '/&', 'gt', '/');
		
			$texto = trim(str_ireplace($procurar, '', $texto));
	
			$valor = str_replace('.','',$texto);
			$valor = str_replace(',','.',$valor);
			
		} else {
			// A partir de TOTAL GERAL
			$texto = trim(substr($texto, stripos($texto, 'TOTAL GERAL') + 4, 500));
		
			// Até a próxima </tr>
			$texto = trim(substr($texto, 0, stripos($texto, '/tr&gt;')));
			
			// Entre TOTAL GERAL e </tr>
			$texto = trim(substr($texto, strripos($texto, '&lt;td')+4, -5));
			
			$procurar   = array('&lt;br&gt;', '&lt;br/&gt;', '&lt;br /&gt;', '&lt;strong&gt;', '&lt;/strong&gt;', '&lt;b&gt;', '&lt;/b&gt;', '&lt;td&gt;', '&lt;/td&gt;', '&lt;tr&gt;', '&lt;/tr&gt;', '&lt;TD&gt;', '&lt;/TD&gt;', '&lt;TD', '/TD&gt;', 'TEXT-ALIGN: CENTER', 'TEXT-ALIGN: RIGHT', 'TEXT-ALIGN: LEFT', '&gt;', '&lt;', 'style=', '&quot;;&quot;', '&amp;nbsp;', 'td', 'R$', '/span', 'span');
		
			$texto = trim(str_ireplace($procurar, '', $texto));
			
			$valor = str_replace('.','',$texto);
			$valor = str_replace(',','.',$valor);
		}	
		
		if( is_numeric($valor) ){
			$vl = "";
			$vl = $db->pegaUm("SELECT dopvalortermo FROM par.documentopar WHERE dopid = ".$documento['dopid']);
			
			if(!$vl){
				$sql .= "UPDATE par.documentopar SET dopvalortermo = ".$valor." WHERE dopid = ".$documento['dopid']."; ";
				//	ver($documento['dopid'], $valor);
			}
		}
	}
	
	
	if( $sql ){
		$db->executar($sql);
		$db->commit();
	}
	
	if( $dopid ){
		return true;
	} else {
		echo "FIM";
	}
}

function enviaEmailNovoTermo($dopid, $tipoReq = 'novo')
{

		global $db;
		
		if( !empty($dopid) ){
			
			$sql = "SELECT iu.itrid, 
			CASE WHEN iu.itrid = 2 THEN
				iu.muncod
			WHEN iu.itrid = 1 THEN
				iu.estuf
			END as filtro
			FROM 
				par.documentopar dp
			INNER JOIN par.processopar prp ON dp.prpid = prp.prpid and prp.prpstatus = 'A'
			INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
			WHERE 
				dopid = {$dopid}
			";
			$result = $db->pegaLinha($sql);
			$itrid = $result['itrid'];
			$filtro = $result['filtro'];
			 
			
			if( ($itrid == 2) && ($filtro)  )
			{
				$sqlEmail = "SELECT
					ent.entemail as email
				FROM
					par.entidade ent
				INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
				INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
				WHERE
					ent.dutid =  7 
					and ent.entstatus = 'A'
				AND
					mun.muncod in ( '{$filtro}' )
				";
			}
			else if( ($itrid == 1) && ($filtro)) 
			{
				$sqlEmail = "SELECT
					 ent.entemail as email
					FROM
						par.entidade ent
					INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
					INNER JOIN territorios.estado est on est.estuf = ent2.estuf
					
					WHERE
					ent.entstatus='A' 
					AND 
						ent.dutid =  10
					AND
						ent2.estuf in ( '{$filtro}' )";
			}
			else
			{
				if($tipoReq == 'reformulacao') 
				{
					echo 'erro_email';
					exit;	
				}
				echo "<script>alert('Não foi possível enviar o e-mail!');
						window.location.href = window.location;
				  </script>";
				
			}
			
			$resultEmail = $db->pegalinha($sqlEmail);
			$emailTo =  $resultEmail['email'];
			if( ! $emailTo )
			{
				if($tipoReq == 'reformulacao') 
				{
					echo 'erro_email';
					exit;	
				}
				$contato = ($itrid == 2) ?  "Prefeito(a)" : "Secretário(a) Estadual";
				
				echo "<script>alert('Email do(a) {$contato} não encontrado!');
						window.location.href = window.location;
				  </script>";	
			}
			
			$sql = "SELECT  
				CASE WHEN dp2.dopano::boolean THEN
					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
				ELSE
					dp.dopnumerodocumento::text
				END
					as ndocumento				
			FROM par.documentopar dp
			
			LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
			WHERE dp.dopid = {$dopid}";
			
			$restulnum = $db->pegalinha($sql);
			
			
			$doptexto = pegaTermoCompromissoArquivo($dopid, '');
			
			$numDoc 	= $restulnum['ndocumento'];
 			
			$strMensagem = "Prezado(a) dirigente,<br>
			
			Informamos que foi disponibilizado no SIMEC, na aba “Documentos”, o Termo de Compromisso nº {$numDoc} que deverá ser validado por Vossa Senhoria o mais breve possível. Esclarecemos que somente após a validação do Termo, os itens constantes em Atas de Registro de Preços do FNDE que estejam vigentes serão disponibilizados no SIGARP.
			<br>
			Atenciosamente,
			<br>
			Equipe PAR "; 
			// . $dopTexto
			$strAssunto = "Termo de compromisso nº {$numDoc} disponível para validação no SIMEC";	
			$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
			$strMensagem = html_entity_decode($strMensagem);
			
			if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
				
				if($tipoReq == 'reformulacao') 
				{
					
					return true;
				}
				else 
				{
					$retorno = true;
				}
				
			} elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
				$strEmailTo = array($_SESSION['email_sistema']);
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);		
			} else {
				$strEmailTo = $emailTo;
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			}
			
			if($tipoReq == 'novo')
			{
				if( $retorno ){
					return true;
				} else {
					echo "<script>alert('Não foi possível enviar o e-mail!');
							window.location.href = window.location;
					  </script>";
				}
			}
			else if($tipoReq == 'reformulacao') 
			{
				if( $retorno ){
					return true;
				} else {
					echo 'erro_email';
					exit;
				}
			}
		} else {
			
			if($tipoReq == 'reformulacao') 
			{
				echo 'erro_email';
				exit;	
			}
			
			echo "<script>alert('Não foi possível enviar o e-mail, é necessário cadastrar uma publicação!');
						window.location.href = window.location;
				  </script>";
		}
		exit();
}

function enviaEmailNovoTermoObrasPAR($dopid, $tipoReq = 'novo')
{

		global $db;
		
		if( !empty($dopid) ){
			
			$sql = "SELECT inu.itrid, 
						CASE WHEN inu.itrid = 2 THEN
							inu.muncod
						WHEN inu.itrid = 1 THEN
							inu.estuf
						END as filtro
					FROM 
						par.documentopar dop
					INNER JOIN par.processoobraspar pro ON pro.proid = dop.proid and pro.prostatus = 'A'
					INNER JOIN par.instrumentounidade inu ON inu.inuid = pro.inuid
					WHERE 
						dopid =  $dopid";
			
			$result = $db->pegaLinha($sql);
			$itrid = $result['itrid'];
			$filtro = $result['filtro'];
			 
			
			if( ($itrid == 2) && ($filtro)  )
			{
				$sqlEmail = "SELECT
								ent.entemail as email
							FROM
								par.entidade ent
							INNER JOIN par.entidade 			ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
							INNER JOIN territorios.municipio 	mun ON mun.muncod = ent2.muncod
							WHERE
								ent.dutid =  7 
								AND ent.entstatus = 'A'
								AND mun.muncod IN ( '$filtro' )";
			}
			else if( ($itrid == 1) && ($filtro)) 
			{
				$sqlEmail = "SELECT
							 	ent.entemail as email
							FROM
								par.entidade ent
							INNER JOIN par.entidade 		ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
							INNER JOIN territorios.estado 	est ON est.estuf = ent2.estuf
							WHERE
								ent.entstatus='A' 
								AND ent.dutid =  10
								AND ent2.estuf in ( '{$filtro}' )";
			}
			else
			{
				if($tipoReq == 'reformulacao') 
				{
					echo 'erro_email';
					exit;	
				}
				echo "<script>alert('Não foi possível enviar o e-mail!');
						window.location.href = window.location;
				  </script>";
				
			}
			
			$resultEmail = $db->pegalinha($sqlEmail);
			$emailTo =  $resultEmail['email'];
			if( ! $emailTo )
			{
				if($tipoReq == 'reformulacao') 
				{
					echo 'erro_email';
					exit;	
				}
				$contato = ($itrid == 2) ?  "Prefeito(a)" : "Secretário(a) Estadual";
				
				echo "<script>alert('Email do(a) {$contato} não encontrado!');
						window.location.href = window.location;
				  </script>";	
			}
			
			$sql = "SELECT  
				CASE WHEN dp2.dopano::boolean THEN
					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
				ELSE
					dp.dopnumerodocumento::text
				END
					as ndocumento
				
			FROM par.documentopar dp
			
			LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
			WHERE dp.dopid = {$dopid}";
			
			$restulnum = $db->pegalinha($sql);
			
			$numDoc 	= $restulnum['ndocumento'];
			
			$doptexto = pegaTermoCompromissoArquivo($dopid, '');
 			
			$strMensagem = "Prezado(a) dirigente,<br>
			
			Informamos que foi disponibilizado no SIMEC, na aba “Documentos”, o Termo de Compromisso nº {$numDoc} que deverá ser validado por Vossa Senhoria o mais breve possível. Esclarecemos que somente após a validação do Termo, os itens constantes em Atas de Registro de Preços do FNDE que estejam vigentes serão disponibilizados no SIGARP.
			<br>
			Atenciosamente,
			<br>
			Equipe PAR "; 
			// . $dopTexto
			$strAssunto = "Termo de compromisso nº {$numDoc} disponível para validação no SIMEC";	
			$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
			$strMensagem = html_entity_decode($strMensagem);
			
			if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
				
				if($tipoReq == 'reformulacao') 
				{
					echo '1';
					exit;
				}
				else 
				{
					$retorno = true;
				}
				
			} elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
				$strEmailTo = array($_SESSION['email_sistema']);
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);		
			} else {
				$strEmailTo = $emailTo;
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			}
			
			if($tipoReq == 'novo')
			{
				if( $retorno ){
					return true;
				} else {
					echo "<script>alert('Não foi possível enviar o e-mail!');
							window.location.href = window.location;
					  </script>";
				}
			}
			else if($tipoReq == 'reformulacao') 
			{
				if( $retorno ){
					echo '1';
					exit;
				} else {
					echo 'erro_email';
					exit;
				}
			}
		} else {
			
			if($tipoReq == 'reformulacao') 
			{
				echo 'erro_email';
				exit;	
			}
			
			echo "<script>alert('Não foi possível enviar o e-mail, é necessário cadastrar uma publicação!');
						window.location.href = window.location;
				  </script>";
		}
                return;
}

function enviaEmailNovoTermoPAC($terid, $tipoReq = 'novo')
{

		global $db;
		
		if( !empty($terid) ){
			
			$sql = "SELECT 
						CASE WHEN muncod IS NOT NULL THEN
							2
						ELSE
							1
						END as itrid,
						CASE WHEN muncod IS NOT NULL THEN
							muncod
						ELSE
							estuf
						END as filtro
					FROM 
						par.termocompromissopac 
					WHERE 
						terid = $terid";
			
			$result = $db->pegaLinha($sql);
			$itrid 	= $result['itrid'];
			$filtro = $result['filtro'];
			 
			
			if( ($itrid == 2) && ($filtro)  )
			{
				$sqlEmail = "SELECT
								ent.entemail as email
							FROM
								par.entidade ent
							INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
							INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
							WHERE
								ent.dutid =  7 
								and ent.entstatus = 'A'
							AND
								mun.muncod in ( '{$filtro}' )
							";
			}
			else if( ($itrid == 1) && ($filtro)) 
			{
				$sqlEmail = "SELECT
							 	ent.entemail as email
							FROM
								par.entidade ent
							INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
							INNER JOIN territorios.estado est on est.estuf = ent2.estuf
							
							WHERE
								ent.entstatus='A' 
								AND ent.dutid =  10
								AND ent2.estuf in ( '{$filtro}' )";
			}
			else
			{
				if($tipoReq == 'reformulacao') 
				{
					echo 'erro_email';
					exit;	
				}
				echo "<script>alert('Não foi possível enviar o e-mail!');
						window.location.href = window.location;
				  </script>";
				
			}
			
			$resultEmail = $db->pegalinha($sqlEmail);
			$emailTo =  $resultEmail['email'];
			if( ! $emailTo )
			{
				if($tipoReq == 'reformulacao') 
				{
					echo 'erro_email';
					exit;	
				}
				$contato = ($itrid == 2) ?  "Prefeito(a)" : "Secretário(a) Estadual";
				
				echo "<script>alert('Email do(a) {$contato} não encontrado!');
						window.location.href = window.location;
				  </script>";	
			}
			
			$sql = "SELECT 
						CASE WHEN t.teridpai IS NOT NULL THEN
							'PAC2'||to_char(t.teridpai,'00000')||'/'||to_char(tc2.terdatainclusao, 'YYYY')
						ELSE
							'PAC2'||to_char(t.terid,'00000')||'/'||to_char(t.terdatainclusao,'YYYY') 
						END as ndocumento
					FROM 
						par.termocompromissopac t
					LEFT JOIN par.termocompromissopac tc2 ON tc2.terid = t.teridpai
					WHERE 
						t.terid = $terid";
			
			$numDoc = $db->pegaUm($sql);
			
			$strMensagem = "Prezado(a) dirigente,<br>
							Informamos que foi disponibilizado no SIMEC, na aba “Documentos”, o Termo de Compromisso nº $numDoc que deverá ser validado por Vossa Senhoria o mais breve possível. 
							Esclarecemos que somente após a validação do Termo, os itens constantes em Atas de Registro de Preços do FNDE que estejam vigentes serão disponibilizados no SIGARP.
							<br>
							Atenciosamente,
							<br>
							Equipe PAR "; 
	
			$strAssunto = "Termo de compromisso nº $numDoc disponível para validação no SIMEC";	
			$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
			$strMensagem = html_entity_decode($strMensagem);
			
			if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
				
				if($tipoReq == 'reformulacao') 
				{
					echo '1';
					exit;
				}
				else 
				{
					$retorno = true;
				}
				
			} elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
				$strEmailTo = array($_SESSION['email_sistema']);
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);		
			} else {
				$strEmailTo = $emailTo;
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			}
			
			if($tipoReq == 'novo')
			{
				if( $retorno ){
					return true;
				} else {
					echo "<script>alert('Não foi possível enviar o e-mail!');
							window.location.href = window.location;
					  </script>";
				}
			}
			else if($tipoReq == 'reformulacao') 
			{
				if( $retorno ){
					echo '1';
					exit;
				} else {
					echo 'erro_email';
					exit;
				}
			}
		} else {
			
			if($tipoReq == 'reformulacao') 
			{
				echo 'erro_email';
				exit;	
			}
			
			echo "<script>alert('Não foi possível enviar o e-mail, é necessário cadastrar uma publicação!');
						window.location.href = window.location;
				  </script>";
		}
		return;
}

function logWsRequisicao( $arrParam = array(), $chave = '', $tabela = '', $action = 'insert' ) {
	global $db;

	$codigo = '';
	if( $action == 'insert' ){
		$arCampos = array();
		$arValor = array();
		foreach ($arrParam as $campo => $valor){
			if( $valor !== null ){
				$arCampos[]  = $campo;
				$valor = str_replace($troca, "", $valor);
				$arValor[] = trim( pg_escape_string( $valor ) );
			}
		}
		$sql = "insert into ".$tabela." (".implode(', ', $arCampos) ." ) values( '". implode("', '", $arValor )."') returning ".$chave."";
		$codigo = $db->pegaUm($sql);
	} else {
		$campos = "";
		foreach ($arrParam as $campo => $valor){
			if( !empty($valor) || is_bool($valor) ){

				if( $campo == $chave ){
					$codigo = $valor;
				}elseif( is_bool($valor) ){
					$campos .= $campo." = ".($valor ? 'true' : 'false').", ";
				}else {
					$valor = pg_escape_string( $valor );
					$campos .= $campo." = '".$valor."', ";
				}
			}
		}
		$campos = substr( $campos, 0, -2 );

		$sql = " UPDATE ".$tabela." SET $campos WHERE ".$chave." = $codigo";
		$db->executar($sql);
	}
	$db->commit();
	return $codigo;
}

function formatarProcesso( $numero ){
	if( !empty( $numero ) ) $processo = substr($numero,0,5) . ".".substr($numero,5,6)."/".substr($numero,11,4) . "-".substr($numero,15,2);
	return $processo;
}

function historicoCancelamento( $dados ){
	global $db;
	
	$tipo = $dados['tipo'];
	$empid = $dados['empid'];
	
	$filtro = "e.empid = '$empid'";
	
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT DISTINCT
			       e.empcnpj,  e.empprotocolo, e.empnumero, vve.vrlempenhocancelado as empvalorempenho, u.usunome,
			       e.empsituacao,
					(select sum(empvalorempenho) from par.empenho where empidpai = e.empid and empcodigoespecie in ('03', '13', '04') and empstatus = 'A') as vrlcancelado
			FROM par.empenho e
				inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid and empstatus = 'A'
			    LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
			WHERE $filtro and empcodigoespecie not in ('03', '13', '02', '04') ";
	$arrEmpenho = $db->pegaLinha($sql);
	?>
		<table border="0" class="tabela" style="width: 100%" align="center"  cellspacing="1" cellpadding="3">
			<tr>
				<th colspan="6">Dados do Empenho</th>
			</tr>
			<tr>
				<td class="subtitulodireita" width="20%"><b>CNPJ:</b></td>
				<td width="30%"><?=formatar_cpf_cnpj($arrEmpenho['empcnpj']) ?></td>
				<td class="subtitulodireita" width="20%"><b>Nº Protocolo:</b></td>
				<td width="30%"><?=$arrEmpenho['empprotocolo'] ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita"><b>Valor Empenho(R$):</b></td>
				<td><?=number_format($arrEmpenho['empvalorempenho'], 2, ',', '.') ?></td>
				<td class="subtitulodireita"><b>Nota Empenho:</b></td>
				<td><?=$arrEmpenho['empnumero'] ?></td>
			</tr>
			<tr>
				<td class="subtitulodireita"><b>Situação:</b></td>
				<td><?=$arrEmpenho['empsituacao'] ?></td>
				<td class="subtitulodireita"><b>Usuário:</b></td>
				<td><?=$arrEmpenho['usunome'] ?></td>
			</tr>
		</table>
		<table border="0" class="tabela" style="width: 100%" align="center"  bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<th colspan="6">Histórico de Fase do Empenho</th>
			</tr>
			<tr>
				<td>
		<?php
		
		$sql = "SELECT
				    u.usunome,
				    to_char(hepdata, 'dd/mm/YYYY HH24:MI') as data,
				    te.teedescricao as especie,
				    h.empsituacao,
				    CASE WHEN h.valor_total_empenhado IS NOT NULL THEN h.valor_total_empenhado ELSE e.empvalorempenho END as valor_total_empenhado,
				    h.valor_saldo_pagamento
				FROM par.historicoempenho h
				    left join seguranca.usuario u ON u.usucpf=h.usucpf
				    inner join par.empenho e ON e.empid=h.empid and empstatus = 'A'
				    left join execucaofinanceira.tipoespecieempenho te on te.teecodigo = h.co_especie_empenho and te.teestatus = 'A'
				WHERE $filtro
					and h.hepdata in (select max(h1.hepdata) as data
                                      from par.historicoempenho h1 
                                      where 
                                          h1.empid = e.empid
                                      group by h1.usucpf, h1.empsituacao)
					order by h.hepdata desc";
		
		$cabecalho = array("Usuário atualização", "Data", "Espécie Empenho", "Situação", "Valor empenhado(R$)", "Valor pagamento(R$)");
		$db->monta_lista_simples($sql, $cabecalho, 60000, 1, 'N', '100%', 'S', true, false, false, true);
		?>
				</td>
			</tr>
		</table>
		<?php 
}


function criarBackupComplemento($sbaid,$sbdid){
    global $db;
    $sqlInsertSubacao = " INSERT INTO par.subacao
                        		( aciid, sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao,
								sbaptres, sbanaturezadespesa, sbamonitoratecnico, docid, frmid,
								indid, foaid, undid, ppsid, prgid, ptsid, sbacronograma, sbappspeso,
								sbaobjetivo, sbatexto, sbacobertura, usucpf, sbadataalteracao,
								sbastatus, sbaidpai, sbadatareformulacao )
								SELECT
									aciid, sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao,
									sbaptres, sbanaturezadespesa, sbamonitoratecnico, docid, frmid,
									indid, foaid, undid, ppsid, prgid, ptsid, sbacronograma, sbappspeso,
									sbaobjetivo, sbatexto, sbacobertura, usucpf, sbadataalteracao,
									'R' as sbastatus, sbaid, NOW()
								FROM
									par.subacao
								WHERE
									sbastatus = 'A' AND
									sbaid = {$sbaid}
								RETURNING
									sbaid";

			$novoidsubacao = $db->pegaUm($sqlInsertSubacao);

			//DETALHE
			$sqlInsertSubacaoDetalhe = "INSERT INTO par.subacaodetalhe
										(sbaid, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
										ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid,
            							sbdplanointerno, sbdparecerdemerito, sbdplicod, sbdptres)
										SELECT
											$novoidsubacao, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
											ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid,
            								sbdplanointerno, sbdparecerdemerito, sbdplicod, sbdptres
										FROM
											par.subacaodetalhe
										WHERE
											sbdid = ".$sbdid;
			$db->carregar($sqlInsertSubacaoDetalhe);
    
}

function carregaEmpenhoPorProcesso( $arPost ){
	global $db;
	
	$processo 	= $arPost['processo'];
	$empidpai 	= $arPost['empidpai'];
	$action 	= $arPost['action'];
	
	$acaoConsultar		= "''";
	$acaoAnularEmpenho 	= "''";
	$acaoReduzirEmpenho = "''";
	$acaoHistorico 		= "''";
	
	$perfil = pegaPerfilGeral();
	//regras de acesso passada por Thiago em 24/05/2012
	if(	in_array(PAR_PERFIL_EMPENHADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ){
		$acaoConsultar 			= "'<center><img src=../imagens/refresh2.gif style=cursor:pointer; title=\"Consultar Empenho\" onclick=consultarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\');></center>'";
		$acaoAnularEmpenho 		= "'<center><img src=../imagens/money_cancel.gif align=absmiddle style=cursor:pointer; title=\"Anular Empenho\" onclick=\"cancelarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\', ''anular'');\"></center>'";
		$acaoReduzirEmpenho 	= "'<center><img src=../imagens/money_ico.png align=absmiddle style=cursor:pointer; title=\"Reduzir Empenho\" onclick=\"reduzirEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\', ''reduzir'');\"></center>'";
		$acaoHistorico 			= "'&nbsp;<img src=../imagens/historico.png align=absmiddle style=cursor:pointer; title=\"Histórico de Movimentação da NE\" onclick=\"historicoCancelamento('||empid||');\">'";
	}
	
	if( $action == 'lista' ){
		$acaoAnularEmpenho = "'<div></div>'";
		$acaoReduzirEmpenho = "'<div></div>'";
	}
	
	$acaoEmpenhoFilho = "'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||empid||'\', \'' || trim(empnumeroprocesso) || '\', this);\">'";
	$especie = " e.empcodigoespecie not in ('03', '13', '02', '04') ";
	$imgFilho = '';
	$boQtd = true;
	$tam = '100';
	$boTotal = 'N';
	$filtroEmpid = 'e.empid';
	$vrlCancelado = "vrlcancelado,";
	$tituloTabela = 'Lista de Empenhos Efetivados';
	$acaoReduzirCancelar = "CASE WHEN empvalorempenho > vrlcancelado 
							   		THEN $acaoReduzirEmpenho 
							   		ELSE '&nbsp;' 
							   END as acao_reduzir,
							   CASE WHEN empvalorempenho > vrlcancelado 
							   		THEN $acaoAnularEmpenho 
							   		ELSE '&nbsp;' 
							   END as acao_cancelar,";
	$colspan = '4';
	
	if( !empty($empidpai) ){
		$acaoAnularEmpenho = "''";
		$acaoReduzirEmpenho = "''";
		$acaoEmpenhoFilho = "''";
		$especie = " e.empidpai = $empidpai ";
		$imgFilho = '<tr><td width="30px" rowspan="30"><img align=absmiddle src="../imagens/seta_filho.gif"></td></tr>';
		$boQtd = false;
		$tam = '80';
		$boTotal = 'S';
		$tituloTabela = 'Lista de Empenhos Cancelado\Reduzido\Reforço';
		$acaoReduzirCancelar = "";
		$vrlCancelado = "vrlcancelado,";
		$colspan = '2';
		$filtroEmpid = 'e.empidpai';
	}
		
	$sql = "select 
				DISTINCT
					case when vrlcancelado = 0 and vrlreforco = 0 then '&nbsp;&nbsp;&nbsp;' else  $acaoEmpenhoFilho end || $acaoHistorico as acao,
				   CASE WHEN empsituacao != 'CANCELADO' THEN 
				   		$acaoConsultar 
				   ELSE '&nbsp;' END as acao_consultar,
				   $acaoReduzirCancelar
				  	formata_cpf_cnpj(empcnpj) as cnpj, empprotocolo||'&nbsp;' as protocolo, empnumero, empvalorempenho, 
				  	$vrlCancelado 
				  	usunome,
				  	teedescricao, 
				  	especie,
				  	CASE WHEN (vrlcancelado >= empvalorempenho) 
						THEN 'CANCELADO' 
						ELSE empsituacao
				 	END AS empsituacao,
				 	vrlreforco,
				   vrlpagamento 
			from(
				SELECT  
					   e.empid, e.empnumeroprocesso, e.empcnpj, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao,
					   case when (select count(empid) from par.empenho where empstatus <> 'I' and empidpai = e.empid and empcodigoespecie in ('03', '13', '04')) > 0 then 
					   		'CANCELADO' 
					   else e.empsituacao end as situacao,
					   e.empcodigoespecie as especie,
					   te.teedescricao,
					   sum(pag.vrlpagamento) as vrlpagamento,
					   (select coalesce(sum(empvalorempenho), 0.00) from par.empenho where empstatus <> 'I' and empidpai = $filtroEmpid and empcodigoespecie in ('02')) as vrlreforco,
					   (select coalesce(sum(empvalorempenho), 0.00) from par.empenho where empstatus <> 'I' and empidpai = $filtroEmpid and empcodigoespecie in ('03', '13', '04')) as vrlcancelado
				FROM par.empenho e
					left join seguranca.usuario u ON u.usucpf=e.usucpf 
					left join execucaofinanceira.tipoespecieempenho te on te.teecodigo = e.empcodigoespecie
					left join (
                    	select p.empid, p.pagsituacaopagamento, sum(p.pagvalorparcela) as vrlpagamento
                        from par.pagamento p 
                        where p.pagsituacaopagamento not ilike '%CANCELADO%' AND p.pagstatus = 'A'
                        group by p.empid, p.pagsituacaopagamento
                    ) pag on pag.empid = e.empid
				WHERE
					$especie
					and empnumeroprocesso='".$processo."' and empstatus <> 'I'
				group by
                	e.empid, e.empnumeroprocesso, e.empcnpj, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao,
                    e.empcodigoespecie, te.teedescricao, e.empidpai
			) as foo";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	?>
	<br>
	<table align="left" border="0"  style="width: <?php echo $tam;?>%" cellpadding="3" cellspacing="1">
			<?php echo $imgFilho; ?>
		<tr>
			<td class="subtitulodireita" style="text-align: center;" colspan="2"><?php echo $tituloTabela; ?></td>
		</tr>
<?php if( $arrDados ){ ?>
		<tr>
			<td>
				<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem" style="color:333333;">
				<thead>
					<tr>
						<td valing="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" colspan="<?php echo $colspan;?>" width="5%">Ações</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">CNPJ</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="06%">Nº protocolo</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Nº empenho</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Espécie</td>
						<?php if( empty($empidpai) ){?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor empenho(R$)</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor Cancelado\Reduzido</td>
						<?php } else {?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor Cancelado\Reduzido</td>
						<?php }?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor Reforço</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="30%">Usuário criação</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Situação empenho</td>
						<?php if( empty($empidpai) ){?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Valor Pago</td>
						<?php } ?>
					</tr>
				</thead>
				<tbody>
			<?php
				$totEmpenho = 0 ;
				$totCancelado = 0 ;
				$totReforco = 0 ;
				foreach ($arrDados as $key => $v ) {
					
					$key % 2 ? $cor = "#dedfde" : $cor = "";
					
					//$totEmpenho 	+= (float)$v['empvalorempenho'];
					$totReforco 	+= (float)$v['vrlreforco'];
					$totCancelado 	+= (float)$v['empvalorempenho'];
					
					$boEfetivado 	= stristr(trim($v['empsituacao']), 'EFETIVADO');
					$boEfetivado 	= ( !empty($boEfetivado) ? true : false );
					
					$boAutorizado 	= stristr(trim($v['empsituacao']), 'AUTORIZADO');
					$boAutorizado 	= ( !empty($boAutorizado) ? true : false );
					
					$boEnvioSiaf 	= stristr(trim($v['empsituacao']), 'ENVIADO AO SIAFI');
					$boEnvioSiaf 	= ( !empty($boEnvioSiaf) ? true : false );
					
					$boSemEnvio 	= stristr(trim($v['empsituacao']), 'SEM ENVIO');
					$boSemEnvio 	= ( !empty($boSemEnvio) ? true : false );
					
					$boRejeitado 	= stristr(trim($v['empsituacao']), 'REJEITADO');
					$boRejeitado 	= ( !empty($boRejeitado) ? true : false );
					
					$boSolicitacao 	= stristr(trim($v['empsituacao']), 'SOLICITAÇÃO');
					$boSolicitacao 	= ( !empty($boSolicitacao) ? true : false );
					
					$boHabilitaCancelamento = true;
					$boHabilitaAnulacao 	= true;
					
					if( $boSolicitacao ){
						$boHabilitaCancelamento = true;
						$boHabilitaAnulacao 	= true;
					}
					if( $boAutorizado || $boEnvioSiaf || $boSemEnvio || $boRejeitado ){
						$boHabilitaCancelamento = false;
						$boHabilitaAnulacao 	= false;
					}
					if( $boEfetivado ){
						$boHabilitaCancelamento = true;
						$boHabilitaAnulacao 	= true;
					}
					
					?>
					<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td valign="top"><?php echo $v['acao'];?></td>
						<td valign="top"><?php echo $v['acao_consultar'];?></td>
						<?php if( $v['acao_reduzir'] && $boHabilitaCancelamento ){?><td valign="top"><?php echo $v['acao_reduzir'];?></td><?php }/*else{?><td>&nbsp;</td><?php }*/?>
						<?php if( $v['acao_cancelar'] && $boHabilitaAnulacao ){?><td valign="top"><?php echo $v['acao_cancelar'];?></td><?php }/*else{?><td>&nbsp;</td><?php }*/?>
						<td valign="top" title="CNPJ"><?php echo $v['cnpj'];?></td>
						<td valign="top" title="Nº protocolo"><?php echo $v['protocolo'];?></td>
						<td valign="top" title="Nº empenho"><?php echo $v['empnumero'];?></td>
						<td valign="top" align="right"><?php echo $v['teedescricao'];?></td>
						<?php if( empty($empidpai) ) {?><td valign="top" align="right" title="Valor empenho(R$)" style="color:#999999;"><?php echo number_format( $v['empvalorempenho'], 2, ',', '.' );?></td><?php }?>
						<?php if( isset($v['vrlcancelado']) && empty($empidpai)  ){?>
							<td valign="top" align="right" title="Valor Cancelado(R$)" style="color:#999999;"><div id="vrlCancelReforco"><?php echo number_format( $v['vrlcancelado'], 2, ',', '.' );?></div></td>
						<?php }else{?>
							<td valign="top" align="right" title="Valor Cancelado(R$)" style="color:#999999;"><div id="vrlCancelReforco"><?php echo number_format( $v['empvalorempenho'], 2, ',', '.' );?></div></td>
						<?php }?>
						<td valign="top" align="right" title="Valor Reforço(R$)" style="color:#999999;"><?php echo number_format( $v['vrlreforco'], 2, ',', '.' );?></td>
						<td valign="top" title="Usuário criação"><?php echo $v['usunome'];?></td>
						<td valign="top" title="Situação empenho"><?php echo $v['empsituacao'];?></td>
						<?php if( empty($empidpai) ) {?><td valign="top" title="Situação empenho"><?php echo number_format($v['vrlpagamento'], 2, ',', '.' ); ?></td><?php }?>
					</tr>
			<?php }?>
				</tbody>
			<?php if( !empty($empidpai) ){?>
				<tfoot>
					<tr>
						<td align="right" title="&nbsp;">Totais:</td>
						<td align="right" title="&nbsp;"></td>
						<td align="right" title="CNPJ"></td>
						<td align="right" title="Nº protocolo"></td>
						<td align="right" title="Nº empenho"></td>
						<td align="right" title="Espécie"></td>
						<td align="right" title="Valor Total Cancelado(R$)"><?php echo number_format( $totCancelado, 2, ',', '.' );?></td>
						<td align="right" title="Valor Total Reforço(R$)"><?php echo number_format( $totReforco, 2, ',', '.' );?></td>
						<td align="right" title="Usuário criação"></td>
						<td align="right" title="Situação empenho"></td>
					</tr>
				</tfoot>
			<?php }?>
			</table>
			</td>
		</tr>
		<tr>
			<td>
			<?php if( empty($empidpai) ){?>
				<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem">
				<tbody>
					<tr bgcolor="#ffffff">
						<td><b>Total de Registros: <?php echo sizeof($arrDados); ?></b></td></tr><tr>
					</tr>
				</tbody>
				</table>
			<?php }?>
			</td>
		</tr>
<?php } else {
	?>
	<tr>
		<td>
		<?php if( empty($empidpai) ){?>
			<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem">
			<tbody>
				<tr bgcolor="#ffffff">
					<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
				</tr>
			</tbody>
			</table>
		<?php }?>
		</td>
	</tr>
<?php } ?>
	</table>
	<?php 
}

function carregaEmpenhoPorProcessoTelaDivergente( $arPost ){
	global $db;
	
	$processo 	= $arPost['processo'];
	$empidpai 	= $arPost['empidpai'];
	$action 	= $arPost['action'];
	
	$acaoAnularEmpenho 	= "''";
	$acaoReduzirEmpenho = "''";
	
	$perfil = pegaPerfilGeral();
	//regras de acesso passada por Thiago em 24/05/2012
	if(	in_array(PAR_PERFIL_EMPENHADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ){
		$acaoAnularEmpenho 		= "'<center><img src=../imagens/money_cancel.gif align=absmiddle style=cursor:pointer; title=\"Anular Empenho\" onclick=\"cancelarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\', ''anular'');\"></center>'";
		$acaoReduzirEmpenho 	= "'<center><img src=../imagens/money_ico.png align=absmiddle style=cursor:pointer; title=\"Reduzir Empenho\" onclick=\"reduzirEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\', ''reduzir'');\"></center>'";
	}
	
	if( $action == 'lista' ){
		$acaoAnularEmpenho = "'<div></div>'";
		$acaoReduzirEmpenho = "'<div></div>'";
	}
	
	$acaoEmpenhoFilho = "'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenhoTelaDivergente(\''||empid||'\', \'' || trim(empnumeroprocesso) || '\', this);\">'";
	$especie = " e.empcodigoespecie not in ('03', '13', '02', '04') ";
	$imgFilho = '';
	$boQtd = true;
	$tam = '100';
	$boTotal = 'N';
	$filtroEmpid = 'e.empid';
	$vrlCancelado = "vrlcancelado,";
	$tituloTabela = 'Lista de Empenhos Efetivados';
	$acaoReduzirCancelar = "CASE WHEN empvalorempenho > vrlcancelado 
							   		THEN $acaoReduzirEmpenho 
							   		ELSE '&nbsp;' 
							   END as acao_reduzir,
							   CASE WHEN empvalorempenho > vrlcancelado 
							   		THEN $acaoAnularEmpenho 
							   		ELSE '&nbsp;' 
							   END as acao_cancelar,";
	$colspan = '3';
	$display = "display:none;";
	
	if( !empty($empidpai) ){
		$display = "";
		$acaoAnularEmpenho = "''";
		$acaoReduzirEmpenho = "''";
		$acaoEmpenhoFilho = "''";
		$especie = " e.empidpai = $empidpai ";
		$imgFilho = '<tr><td width="30px" rowspan="30"><img align=absmiddle src="../imagens/seta_filho.gif"></td></tr>';
		$boQtd = false;
		$tam = '80';
		$boTotal = 'S';
		$tituloTabela = 'Lista de Empenhos Cancelado\Reduzido\Reforço';
		$acaoReduzirCancelar = "";
		$vrlCancelado = "vrlcancelado,";
		$colspan = '3';
		$filtroEmpid = 'e.empidpai';
	}
		
	$sql = "select 
				DISTINCT
					case when vrlcancelado = 0 and vrlreforco = 0 then '&nbsp;&nbsp;&nbsp;' else  $acaoEmpenhoFilho end as acao,
				   	$acaoReduzirCancelar
				  	empnumero, empvalorempenho, 
				  	$vrlCancelado 
				  	teedescricao, 
				  	especie,
				  	CASE WHEN (vrlcancelado >= empvalorempenho) 
						THEN 'CANCELADO' 
						ELSE empsituacao
				 	END AS empsituacao,
				 	vrlreforco,
				   vrlpagamento 
			from(
				SELECT  
					   e.empid, e.empnumeroprocesso, e.empcnpj, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao,
					   case when (select count(empid) from par.empenho where empstatus <> 'I' and empidpai = e.empid and empcodigoespecie in ('03', '13', '04')) > 0 then 
					   		'CANCELADO' 
					   else e.empsituacao end as situacao,
					   e.empcodigoespecie as especie,
					   te.teedescricao,
					   sum(pag.vrlpagamento) as vrlpagamento,
					   (select coalesce(sum(empvalorempenho), 0.00) from par.empenho where empstatus <> 'I' and empidpai = $filtroEmpid and empcodigoespecie in ('02')) as vrlreforco,
					   (select coalesce(sum(empvalorempenho), 0.00) from par.empenho where empstatus <> 'I' and empidpai = $filtroEmpid and empcodigoespecie in ('03', '13', '04')) as vrlcancelado
				FROM par.empenho e
					left join seguranca.usuario u ON u.usucpf=e.usucpf 
					left join execucaofinanceira.tipoespecieempenho te on te.teecodigo = e.empcodigoespecie
					left join (
                    	select p.empid, p.pagsituacaopagamento, sum(p.pagvalorparcela) as vrlpagamento
                        from par.pagamento p 
                        where p.pagsituacaopagamento not ilike '%CANCELADO%' AND p.pagstatus = 'A'
                        group by p.empid, p.pagsituacaopagamento
                    ) pag on pag.empid = e.empid
				WHERE
					$especie
					and empnumeroprocesso='".$processo."' and empstatus <> 'I'
				group by
                	e.empid, e.empnumeroprocesso, e.empcnpj, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao,
                    e.empcodigoespecie, te.teedescricao, e.empidpai
			) as foo";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	?>
	<br>
	<script>
	jQuery(document).ready(function(){
		jQuery('#btn_mostra_empenhoEfetivado').live('click',function(){
			if( jQuery(this).attr('src') == '../imagens/menos.gif' ){
				jQuery('.empenhoEfetivado').hide();
				jQuery(this).attr('src','../imagens/mais.gif');
				jQuery(this).attr('title','Mostrar');
			}else{
				jQuery('.empenhoEfetivado').show();
				jQuery(this).attr('src','../imagens/menos.gif');
				jQuery(this).attr('title','Esconder');
			}
		});
	});
	</script>
	<div style="text-align: left;font-size:12px;" class="TituloTela">
		<?php if( empty($empidpai) ){?>
		<img align="absmiddle" id="btn_mostra_empenhoEfetivado" style="cursor:pointer;" title="Mostrar" src="../imagens/mais.gif">
		<?php }?>
		<?=$tituloTabela?>
	</div>
	<table align="left" border="0" class="empenhoEfetivado" style="width: <?php echo $tam;?>%; <?=$display ?>" cellpadding="3" cellspacing="1">
		<?php echo $imgFilho; ?>
<?php if( $arrDados ){ ?>
		<tr>
			<td>
				<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem" style="color:333333;">
				<thead>
					<tr>
						<?php if( empty($empidpai) ){?>
						<td valing="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" colspan="<?php echo $colspan;?>" width="5%">Ações</td>
						<?php }?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Nº empenho</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Espécie</td>
						<?php if( empty($empidpai) ){?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor empenho(R$)</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor Cancelado\Reduzido</td>
						<?php } else {?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor Cancelado\Reduzido</td>
						<?php }?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="08%">Valor Reforço</td>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Situação empenho</td>
						<?php if( empty($empidpai) ){?>
						<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Valor Pago</td>
						<?php } ?>
					</tr>
				</thead>
				<tbody>
			<?php
				$totEmpenho = 0 ;
				$totCancelado = 0 ;
				$totReforco = 0 ;
				foreach ($arrDados as $key => $v ) {
					
					$key % 2 ? $cor = "#dedfde" : $cor = "";
					
					//$totEmpenho 	+= (float)$v['empvalorempenho'];
					$totReforco 	+= (float)$v['vrlreforco'];
					$totCancelado 	+= (float)$v['empvalorempenho'];
					
					$boEfetivado 	= stristr(trim($v['empsituacao']), 'EFETIVADO');
					$boEfetivado 	= ( !empty($boEfetivado) ? true : false );
					
					$boAutorizado 	= stristr(trim($v['empsituacao']), 'AUTORIZADO');
					$boAutorizado 	= ( !empty($boAutorizado) ? true : false );
					
					$boEnvioSiaf 	= stristr(trim($v['empsituacao']), 'ENVIADO AO SIAFI');
					$boEnvioSiaf 	= ( !empty($boEnvioSiaf) ? true : false );
					
					$boSemEnvio 	= stristr(trim($v['empsituacao']), 'SEM ENVIO');
					$boSemEnvio 	= ( !empty($boSemEnvio) ? true : false );
					
					$boRejeitado 	= stristr(trim($v['empsituacao']), 'REJEITADO');
					$boRejeitado 	= ( !empty($boRejeitado) ? true : false );
					
					$boSolicitacao 	= stristr(trim($v['empsituacao']), 'SOLICITAÇÃO');
					$boSolicitacao 	= ( !empty($boSolicitacao) ? true : false );
					
					$boHabilitaAnulacao 	= true;
					
					if( $boSolicitacao ){
						$boHabilitaAnulacao 	= true;
					}
					if( $boAutorizado || $boEnvioSiaf || $boSemEnvio || $boRejeitado ){
						$boHabilitaAnulacao 	= false;
					}
					if( $boEfetivado ){
						$boHabilitaAnulacao 	= true;
					}
					
					?>
					<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<?php if( empty($empidpai) ){?>
						<td valign="top"><?php echo $v['acao'];?></td>
						<?php }?>
						<?php if( $v['acao_reduzir'] && $boHabilitaAnulacao ){?><td valign="top"><?php echo $v['acao_reduzir'];?></td><?php }/*else{?><td>&nbsp;</td><?php }*/?>
						<?php if( $v['acao_cancelar'] && $boHabilitaAnulacao ){?><td valign="top"><?php echo $v['acao_cancelar'];?></td><?php }/*else{?><td>&nbsp;</td><?php }*/?>
						<td valign="top" title="Nº empenho"><?php echo $v['empnumero'];?></td>
						<td valign="top" align="right"><?php echo $v['teedescricao'];?></td>
						<?php if( empty($empidpai) ) {?><td valign="top" align="right" title="Valor empenho(R$)" style="color:#999999;"><?php echo number_format( $v['empvalorempenho'], 2, ',', '.' );?></td><?php }?>
						<?php if( isset($v['vrlcancelado']) && empty($empidpai)  ){?>
							<td valign="top" align="right" title="Valor Cancelado(R$)" style="color:#999999;"><div id="vrlCancelReforco"><?php echo number_format( $v['vrlcancelado'], 2, ',', '.' );?></div></td>
						<?php }else{?>
							<td valign="top" align="right" title="Valor Cancelado(R$)" style="color:#999999;"><div id="vrlCancelReforco"><?php echo number_format( $v['empvalorempenho'], 2, ',', '.' );?></div></td>
						<?php }?>
						<td valign="top" align="right" title="Valor Reforço(R$)" style="color:#999999;"><?php echo number_format( $v['vrlreforco'], 2, ',', '.' );?></td>
						<td valign="top" title="Situação empenho"><?php echo $v['empsituacao'];?></td>
						<?php if( empty($empidpai) ) {?><td valign="top" title="Situação empenho"><?php echo number_format($v['vrlpagamento'], 2, ',', '.' ); ?></td><?php }?>
					</tr>
			<?php }?>
				</tbody>
			<?php if( !empty($empidpai) ){?>
				<tfoot>
					<tr>
						<td align="right" title="&nbsp;"></td>
						<td align="right" title="&nbsp;"></td>
						<td align="right" title="&nbsp;">Totais:</td>
						<td align="right" title="Valor Total Cancelado(R$)"><?php echo number_format( $totCancelado, 2, ',', '.' );?></td>
						<td align="right" title="Valor Total Reforço(R$)"><?php echo number_format( $totReforco, 2, ',', '.' );?></td>
						<td align="right"></td>
					</tr>
				</tfoot>
			<?php }?>
			</table>
			</td>
		</tr>
		<tr>
			<td>
			<?php if( empty($empidpai) ){?>
				<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem">
				<tbody>
					<tr bgcolor="#ffffff">
						<td><b>Total de Registros: <?php echo sizeof($arrDados); ?></b></td></tr><tr>
					</tr>
				</tbody>
				</table>
			<?php }?>
			</td>
		</tr>
<?php } else {
	?>
	<tr>
		<td>
		<?php if( empty($empidpai) ){?>
			<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem">
			<tbody>
				<tr bgcolor="#ffffff">
					<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
				</tr>
			</tbody>
			</table>
		<?php }?>
		</td>
	</tr>
<?php } ?>
	</table>
	<?php 
}

function montaXMLHistoricoProcessoSIGEF( $arrParam = array() ){

	$wsusuario 		= $arrParam['wsusuario'];
	$wssenha 		= $arrParam['wssenha'];
	$nu_processo 	= $arrParam['nu_processo'];
	$sistema	 	= $arrParam['sistema'];
	$method 		= $arrParam['method'];
	$data_created 	= date("c");
	$aParametros 	= array();
	
	if( $method == 'historicopagamento' ){
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	}
	
	try {
	
		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
			<nu_processo>$nu_processo</nu_processo>
		</params>
	</body>
</request>
XML;
	
		$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => $method) )
		->execute();
		
		$xmlRetorno = $xml;
		$xml = simplexml_load_string( stripslashes($xml));
		
		$aParamValor = array();
		if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
			$arrXML = $xml->body->children();
			
			foreach($arrXML as $chaves => $dados ){
				foreach($dados as $key => $dado){
					if($key == 'data_do_empenho' || $key == 'dt_movimento' || $key == 'dt_emissao' ){
						if( $dados->{$key} ){
							$dados->{$key} = formata_data_sql($dados->{$key});
						}
					}
					$aParamValor[$key] = trim((string) utf8_decode($dados->{$key}) );
				}
				array_push($aParametros, $aParamValor);
			}
		} else {
			/* $html = "<span style='color: red;'><b>Detalhes da Execução - Histórico SIGEF:</b><br/><br/>".$xml->status->message->text."<br/><br/>
									<b>Processo:</b> ".$nu_processo."<br/>
									<b>Histórico:</b> ".$method."<br/>
									<b>Sistema:</b> ".$sistema."</span>";
			$assunto  = "SIMEC - Histórico SIGEF";
			enviar_email(array('nome'=>'SIMEC - Histórico', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $html ); */
		}
	} catch (Exception $e) {
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		$erro = "Erro-WS Histórico SIGEF: $erroMSG";
		$html = "<span style='color: red;'><b>Detalhes da Execução - Histórico SIGEF:</b><br/><br/>".$erro."<br/><br/>
					<b>Processo:</b> ".$nu_processo."<br/>
					<b>Histórico:</b> ".$method."<br/>
					<b>Sistema:</b> ".$sistema."</span>";
		
		$assunto  = "SIMEC - Histórico SIGEF";
		enviar_email(array('nome'=>'SIMEC - Histórico', 'email'=>'noreply@mec.gov.br'), $_SESSION['email_sistema'], $assunto, $html );
	}
	
	return $aParametros;
}

function insereCargaEmpenhoSIGEF( $arrRetorno, $arrParam ){
	global $db;
	
	$nu_processo = $arrParam["nu_processo"];
	$codigo  	 = $arrParam["codigo"];
	$sistema 	 = $arrParam["sistema"];
	
	foreach($arrRetorno as $chaves => $dados ){
		
		$dados['data_do_empenho'] = ($dados['data_do_empenho'] == '--' ? '' : $dados['data_do_empenho']);
		
		$cnpj 						= trim((string)$dados['cnpj']);
		$programa_fnde 				= trim((string)$dados['programa_fnde']);
		$unidade_gestora 			= trim((string)$dados['unidade_gestora']);
		$numero_da_proposta_siconv 	= trim((string)$dados['numero_da_proposta_siconv']);
		$numero_da_ne 				= trim((string)$dados['numero_da_ne']);
		$numero_de_vinculacao_ne 	= trim((string)$dados['numero_de_vinculacao_ne']);
		$valor_da_ne 				= trim((string)$dados['valor_da_ne']);
		$numero_sequencial_da_ne 	= trim((string)$dados['numero_sequencial_da_ne']);
		$nu_seq_mov_ne 				= trim((string)$dados['nu_seq_mov_ne']);
		$data_do_empenho 			= trim((string)$dados['data_do_empenho']);
		$cpf 						= trim((string)$dados['cpf']);
		$nu_id_sistema 				= trim((string)$dados['nu_id_sistema']);
		$descricao_do_empenho 		= trim((string)$dados['descricao_do_empenho']);
		$ano_do_empenho 			= trim((string)$dados['ano_do_empenho']);
		$centro_de_gestao 			= trim((string)$dados['centro_de_gestao']);		
		$natureza_de_despesa 		= trim((string)$dados['natureza_de_despesa']);
		$fonte_de_recurso 			= trim((string)$dados['fonte_de_recurso']);
		$ptres 						= trim((string)$dados['ptres']);
		$esfera 					= trim((string)$dados['esfera']);
		$pi 						= trim((string)$dados['pi']);
		$cod_especie 				= trim((string)$dados['cod_especie']);
		$numero_do_processo 		= trim((string)$dados['numero_do_processo']);
		$situacao_do_empenho 		= trim((string)$dados['situacao_do_empenho']);
		
		$teeid = 'null';
		if($cod_especie == '01'){
			$teeid = 1;
		}elseif($cod_especie == '02'){
			$teeid = 2;
		}elseif($cod_especie == '03' || $cod_especie == '13'){
			$teeid = 3;
		}elseif($cod_especie == '04'){
			$teeid = 4;
		}

		$sql = "SELECT empid, empnumero FROM par.empenho WHERE empprotocolo  = '{$numero_sequencial_da_ne}' and empstatus = 'A'";
		$dadosEmpenhoSIMEC = $db->pegaLinha($sql);
		if($dadosEmpenhoSIMEC['empid']){
			$empid = $dadosEmpenhoSIMEC['empid'];
		}else{
			$empid = 'NULL';
		}				
		
		$numero_de_vinculacao_ne 	= $numero_de_vinculacao_ne ? "'".$numero_de_vinculacao_ne."'" : 'NULL';
		$nu_seq_mov_ne 				= $nu_seq_mov_ne ? $nu_seq_mov_ne : 'NULL';				
		$nu_id_sistema 				= $nu_id_sistema ? "'".$nu_id_sistema."'" : 'NULL';
		$data_do_empenho 			= $data_do_empenho ? "'".$data_do_empenho."'" : 'NULL';

		if( $sistema == 'PAR' ){
			$prpid = $codigo;
			$proidpac = 'null';
			$proidpar = 'null';
		} elseif( $sistema == 'ObrasPAR' ){
			$prpid = 'null';
			$proidpac = 'null';
			$proidpar = $codigo;
		} else {
			$prpid = 'null';
			$proidpac = $codigo;
			$proidpar = 'null';
		}
		
		$boTem = $db->pegaUm("select count(ems_numero_sequencial_da_ne) from par.empenhosigef where ems_numero_sequencial_da_ne = '$numero_sequencial_da_ne' and ems_numero_processo = '$numero_do_processo'");
		
		$numero_da_ne 				= $numero_da_ne ? "'".$numero_da_ne."'" : 'NULL';
		$valor_da_ne 				= $valor_da_ne ? "'".$valor_da_ne."'" : 'NULL';
		$numero_sequencial_da_ne 	= $numero_sequencial_da_ne ? "'".$numero_sequencial_da_ne."'" : 'NULL';
		
		if( (int)$boTem == 0 ){

			$sql = "INSERT INTO par.empenhosigef(prpid, proidpac, proidpar, empid, teeid, 
							ems_numero_processo, ems_cnpj, ems_programa_fnde, ems_unidade_gestora,
							ems_numero_do_empenho, ems_numero_do_empenho_pai, ems_valor_empenho, ems_numero_sequencial_da_ne, ems_nu_seq_mov_ne, ems_data_empenho,
							ems_cpf, ems_numero_sistema, ems_descricao_do_empenho, ems_ano_do_empenho, ems_centro_de_gestao, ems_codigo_nat_despesa, ems_fonte_recurso, 
							ems_ptres,ems_esfera,ems_pi,ems_codigo_especie,ems_situacao_do_empenho, numero_da_proposta_siconv, ems_data_atualizacao_rotina
						) VALUES ( {$prpid}, {$proidpac}, {$proidpar}, {$empid}, {$teeid}, 
						'{$numero_do_processo}', '{$cnpj}', '{$programa_fnde}', '{$unidade_gestora}',
						{$numero_da_ne}, {$numero_de_vinculacao_ne}, {$valor_da_ne}, {$numero_sequencial_da_ne}, {$nu_seq_mov_ne}, {$data_do_empenho},
						'{$cpf}', {$nu_id_sistema}, '{$descricao_do_empenho}', '{$ano_do_empenho}', '{$centro_de_gestao}', '{$natureza_de_despesa}', '{$fonte_de_recurso}', 
						'{$ptres}','{$esfera}','{$pi}','{$cod_especie}','{$situacao_do_empenho}', '{$numero_da_proposta_siconv}', now());";
			
			$db->executar($sql);
		} else {
			$sql = "UPDATE par.empenhosigef SET 
					  prpid 						= $prpid,
					  proidpac 						= $proidpac,
					  proidpar 						= $proidpar,
					  empid 						= $empid,
					  teeid 						= $teeid,
					  ems_numero_processo 			= '$numero_do_processo',
					  ems_cnpj 						= '$cnpj',
					  ems_programa_fnde 			= '$programa_fnde',
					  ems_unidade_gestora 			= '$unidade_gestora',
					  ems_numero_do_empenho 		= $numero_da_ne,
					  ems_numero_do_empenho_pai 	= $numero_de_vinculacao_ne,
					  ems_valor_empenho 			= $valor_da_ne,
					  ems_numero_sequencial_da_ne 	= $numero_sequencial_da_ne,
					  ems_nu_seq_mov_ne 			= $nu_seq_mov_ne,
					  ems_data_empenho 				= $data_do_empenho,
					  ems_cpf 						= '$cpf',
					  ems_numero_sistema 			= $nu_id_sistema,
					  ems_descricao_do_empenho 		= '$descricao_do_empenho',
					  ems_ano_do_empenho 			= '$ano_do_empenho',
					  ems_centro_de_gestao 			= '$centro_de_gestao',
					  ems_codigo_nat_despesa 		= '$natureza_de_despesa',
					  ems_fonte_recurso 			= '$fonte_de_recurso',
					  ems_ptres 					= '$ptres',
					  ems_esfera 					= '$esfera',
					  ems_pi 						= '$pi',
					  ems_codigo_especie 			= '$cod_especie',
					  ems_situacao_do_empenho 		= '$situacao_do_empenho',
					  numero_da_proposta_siconv		= '{$numero_da_proposta_siconv}',
					  ems_data_atualizacao_rotina	= now()
					 
					WHERE 
					  	ems_numero_processo = '$numero_do_processo'
						and ems_numero_sequencial_da_ne = $numero_sequencial_da_ne";
			
			$db->executar($sql);
		}
		$db->commit();
	}
	return true;
}

function insereCargaPagamentoSIGEF($arrRetorno, $arrParam){
	global $db;
	
	$nu_processo = $arrParam["nu_processo"];
	$codigo  	 = $arrParam["codigo"];
	$sistema 	 = $arrParam["sistema"];
	
	foreach($arrRetorno as $chaves => $dados ){
				
		$sql = "SELECT empid, empnumero FROM par.empenho WHERE empprotocolo  = '{$dados['nu_seq_mov_ne']}'";
		$dadosEmpenhoSIMEC = $db->pegaLinha($sql);
		if($dadosEmpenhoSIMEC['empid']){
			$empid = $dadosEmpenhoSIMEC['empid'];
		}else{
			$empid = 'NULL';
		}
		
		$sql = "select pagid, pagnumeroob from par.pagamento where parnumseqob = '{$dados['nu_seq_mov_pag']}' and pagstatus = 'A'";
		$dadosPagamentoSIMEC = $db->pegaLinha($sql);
		if($dadosPagamentoSIMEC['pagid']){
			$pagid = $dadosPagamentoSIMEC['pagid'];
		}else{
			$pagid = 'NULL';
		}

		if( $sistema == 'PAR' ){
			$prpid = $codigo;
			$proidpac = 'null';
			$proidpar = 'null';
		} elseif( $sistema == 'ObrasPAR' ){
			$prpid = 'null';
			$proidpac = 'null';
			$proidpar = $codigo;
		} else {
			$prpid = 'null';
			$proidpac = $codigo;
			$proidpar = 'null';
		}
		
		$boTem = $db->pegaUm("select count(nu_seq_mov_ne) from par.historicopagamentosigef where nu_seq_mov_pag = '{$dados['nu_seq_mov_pag']}' and nu_processo = '$nu_processo'");
		
		$dados['dt_emissao'] = ($dados['dt_emissao'] == '--' ? '' : $dados['dt_emissao']);
			
		$nu_parcela 			= $dados['nu_parcela'] 				? "'".$dados['nu_parcela']."'" 				: 'null';
		$an_exercicio 			= $dados['an_exercicio'] 			? "'".$dados['an_exercicio']."'" 			: 'null';
		$processo	 			= $nu_processo 						? "'".$nu_processo."'" 						: 'null';
		$vl_parcela 			= $dados['vl_parcela'] 				? "'".$dados['vl_parcela']."'" 				: 'null';
		$nu_mes 				= $dados['nu_mes'] 					? "'".$dados['nu_mes']."'" 					: 'null';
		$nu_documento_siafi_ne 	= $dados['nu_documento_siafi_ne'] 	? "'".$dados['nu_documento_siafi_ne']."'" 	: 'null';
		$nu_seq_mov_ne 			= $dados['nu_seq_mov_ne'] 			? "'".$dados['nu_seq_mov_ne']."'" 			: 'null';
		$ds_username_movimento 	= $dados['ds_username_movimento'] 	? "'".$dados['ds_username_movimento']."'" 	: 'null';
		$ds_situacao_doc_siafi 	= $dados['ds_situacao_doc_siafi'] 	? "'".$dados['ds_situacao_doc_siafi']."'" 	: 'null';
		$dt_movimento 			= $dados['dt_movimento'] 			? "'".$dados['dt_movimento']."'" 			: 'null';
		$nu_seq_mov_pag 		= $dados['nu_seq_mov_pag'] 			? "'".$dados['nu_seq_mov_pag']."'" 			: 'null';
		$dt_emissao 			= $dados['dt_emissao'] 				? "'".$dados['dt_emissao']."'" 				: 'null';
		$nu_documento_siafi 	= $dados['nu_documento_siafi'] 		? "'".$dados['nu_documento_siafi']."'" 		: 'null';
		$numero_de_vinculacao 	= $dados['numero_de_vinculacao'] 	? "'".$dados['numero_de_vinculacao']."'" 	: 'null';
			
						
		if( (int)$boTem == 0 ){

				$sql = "INSERT INTO par.historicopagamentosigef(prpid, proidpac, proidpar, empid, pagid, nu_processo, nu_parcela, an_exercicio, vl_parcela, nu_mes, nu_documento_siafi_ne, nu_seq_mov_ne,
  							ds_username_movimento, ds_situacao_doc_siafi, dt_movimento, nu_seq_mov_pag, dt_emissao, nu_documento_siafi, numero_de_vinculacao, data_atualizacao_rotina)
						VALUES (
						  $prpid,
						  $proidpac,
						  $proidpar,
  						  $empid,
  						  $pagid,
						  $processo,
						  $nu_parcela,
						  $an_exercicio,
						  $vl_parcela,
						  $nu_mes,
						  $nu_documento_siafi_ne,
						  $nu_seq_mov_ne,
						  $ds_username_movimento,
						  $ds_situacao_doc_siafi,
						  $dt_movimento,
						  $nu_seq_mov_pag,
						  $dt_emissao,
						  $nu_documento_siafi,
						  $numero_de_vinculacao, now())";
			
			$db->executar($sql);
		} else {
			$sql = "UPDATE par.historicopagamentosigef SET 
					  	prpid 					= $prpid,
					  	proidpac 				= $proidpac,
					  	proidpar 				= $proidpar,
					  	empid 					= $empid,
					  	pagid 					= $pagid,
					  	nu_processo 			= $processo,
					  	nu_parcela 				= $nu_parcela,
					  	an_exercicio 			= $an_exercicio,
					  	vl_parcela 				= $vl_parcela,
					  	nu_mes 					= $nu_mes,
					  	nu_documento_siafi_ne 	= $nu_documento_siafi_ne,
					  	nu_seq_mov_ne 			= $nu_seq_mov_ne,
					  	ds_username_movimento 	= $ds_username_movimento,
					  	ds_situacao_doc_siafi 	= $ds_situacao_doc_siafi,
					  	dt_movimento 			= $dt_movimento,
					  	nu_seq_mov_pag 			= $nu_seq_mov_pag,
					  	dt_emissao 				= $dt_emissao,
					  	nu_documento_siafi 		= $nu_documento_siafi,
					  	numero_de_vinculacao 	= $numero_de_vinculacao,
					  	data_atualizacao_rotina = now()					 
					WHERE 
					  	nu_seq_mov_pag = $nu_seq_mov_pag";		
			
			$db->executar($sql);
		}
		$db->commit();
	}
	return true;
}

function verificaEmpenhoValorDivergente( $processo, $empenho = '', $sistema = '' ){
	global $db;
	
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
				vrlempenho <> vrlempcomposicao
				and diferenca > 1
				and processo = '$processo'
				".($empenho ? " and empenho = $empenho" : '')."
				".($sistema ? " and tipo = '$sistema'" : '')."
			order by especie asc limit 1";
// 	ver($sql,d);
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	return $arrDados;
}

function carregaTelaEmpenhoDivergente( $dados ){
	global $db;
	$processo = $dados['processo'];
	$sistema = $dados['sistema'];
	///23400018036201365
	$arrDados = verificaEmpenhoValorDivergente($processo, '', $sistema);
	
	$vrlDivergente = 0;
	?>
	<form action="" method="post" id="formDivergente" name="formDivergente">
			<input type="hidden" name="requisicao" id="requisicao" value="">
	<?php 
	foreach ($arrDados as $key => $v) {
		
		$vrlDivergente = ((float)$v['vrlempenho'] - (float)$v['vrlempcomposicao']);
		
		if( $v['especie'] == '03' || $v['especie'] == '04' || $v['especie'] == '13' ){
			$filtro = "es.empid in (select empidpai from par.empenho where empid = {$v['empenho']})";
			$filtroJoin = ' and eja.empid = es.empid ';
		} else {
			$filtro = "es.empid = {$v['empenho']}";
		}
		
		if( $v['tipo'] == 'PAR' ){
			$label = 'Valor da Subação';
			$buttonLabel = "Adcionar Subação ao Empenho";
			
			$sql = "SELECT distinct
						s.sbaid as codigo,
						s.sbadsc as descricao,
						par.retornacodigosubacao(s.sbaid) as local,
						es.eobvalorempenho as valordistibuir,
						par.recuperavalorvalidadossubacaoporano( s.sbaid, es.eobano ) as vrlitem,
                        ((coalesce(par.recuperavalorvalidadossubacaoporano( s.sbaid, es.eobano ), 0.00) - coalesce(sum(eja.vlrempenhado), 0)) ) as valordisponivel,
					    sum(eja.vlrempenhado) as vlrempenhado,
						es.eobano as ano,
						sum(eja.vrlcancelado) as vrlcancelado
					FROM par.empenhosubacao es
					    inner join par.subacao s on s.sbaid = es.sbaid and es.eobstatus = 'A'				    
					   	left join (select
										(sum(es1.eobvalorempenho) + coalesce(ref.vrlreforco, 0)) - coalesce(emc.vrlcancelado, 0) as vlrempenhado,
										coalesce(emc.vrlcancelado, 0) as vrlcancelado, es1.sbaid, es1.eobano, e1.empid
									from par.empenhosubacao es1
										inner join par.empenho e1 on e1.empid = es1.empid                                        
                                        left join (select sum(eb.eobvalorempenho) as vrlcancelado, e1.empidpai, eb.sbaid, eb.eobano
                                                from par.empenhosubacao eb
                                                    inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eb.eobstatus = 'A'
                                                where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
                                                group by e1.empidpai, eb.sbaid, eb.eobano
                                        ) as emc on emc.empidpai = e1.empid and emc.sbaid = es1.sbaid and emc.eobano = es1.eobano                                        
                                        left join (select sum(eb.eobvalorempenho) as vrlreforco, e1.empidpai, eb.sbaid, eb.eobano
                                                from par.empenhosubacao eb
                                                    inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eb.eobstatus = 'A'
                                                where e1.empcodigoespecie in ('02') and empidpai is not null
                                                group by e1.empidpai, eb.sbaid, eb.eobano
                                        ) as ref on ref.empidpai = e1.empid and ref.sbaid = es1.sbaid and ref.eobano = es1.eobano                                        
									where 
										es1.eobstatus = 'A'
									    and e1.empstatus = 'A'
									    and e1.empcodigoespecie not in ('03', '13', '04')
									group by es1.sbaid, es1.eobano, e1.empid , 
                                    emc.vrlcancelado, ref.vrlreforco) eja on eja.sbaid = es.sbaid and eja.eobano = es.eobano $filtroJoin					    
					WHERE $filtro and es.eobstatus = 'A'
					GROUP BY
                        s.sbaid,
                        s.sbadsc,
                        es.eobano,
					    es.eobvalorempenho";
		}
		if( $v['tipo'] == 'OBRA' ){
			$label = 'Valor da Obra';
			$buttonLabel = "Adcionar Obras ao Empenho";
			$sql = "SELECT DISTINCT
						p.predescricao as descricao, 
						p.preid as local, 
						p.preid as codigo, 
						es.eobvalorempenho as valordistibuir,
						cast(coalesce(p.prevalorobra, 0.00) as numeric(20,2)) as vrlitem,
						cast(coalesce(p.prevalorobra, 0.00) as numeric(20,2))-veo.saldo as valordisponivel,
						veo.saldo as vlrempenhado,
						p.preano as ano,
						veo.valorcancelado
					FROM par.empenhoobrapar es
					INNER JOIN obras.preobra 				p   ON p.preid = es.preid AND es.eobstatus = 'A'
					LEFT  JOIN par.vm_saldo_empenho_por_obra veo ON veo.preid = p.preid
					WHERE $filtro";
		}
		if( $v['tipo'] == 'PAC' ){
			$label = 'Valor da Obra';
			$buttonLabel = "Adcionar Obras ao Empenho";
			$sql = "SELECT DISTINCT
						p.predescricao as descricao, 
						p.preid as local, 
						p.preid as codigo, 
						es.eobvalorempenho as valordistibuir,
						cast(coalesce(p.prevalorobra, 0.00) as numeric(20,2)) as vrlitem,
						cast(coalesce(p.prevalorobra, 0.00) as numeric(20,2))-veo.saldo as valordisponivel,
						veo.saldo as vlrempenhado,
						p.preano as ano,
						veo.valorcancelado
					FROM par.empenhoobra es
					INNER JOIN obras.preobra 				p   ON p.preid = es.preid AND es.eobstatus = 'A'
					LEFT  JOIN par.vm_saldo_empenho_por_obra veo ON veo.preid = p.preid
					WHERE $filtro";
		}
// 		ver($sql,d);
		$arrEmpenho = $db->carregar($sql);
		$arrEmpenho = $arrEmpenho ? $arrEmpenho : array();
		
		$vrlDivergente = number_format( $vrlDivergente, 2, ',', '.' );
		?>
<table align="center" border="0" class="tabela"  style="width: 100%" cellpadding="3" cellspacing="1">
	<tr>
		<td>
			<table align="center" border="0" class="tabela"  style="width: 100%" cellpadding="3" cellspacing="1">
				<tr>
					<td class="subtitulodireita">Processo:</td>
					<td><?php echo $v['processo']; ?></td>
					<td class="subtitulodireita">Valor Empenhado:</td>
					<td><?php echo number_format( $v['vrlempenho'], 2, ',', '.' ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Nota de Empenho:</td>
					<td><?php echo $v['notaempenho']; ?></td>
					<td class="subtitulodireita">Valor Composição:</td>
					<td><?php echo number_format( $v['vrlempcomposicao'], 2, ',', '.' ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Sequencial NE:</td>
					<td><?php echo $v['sequencial']; ?></td>
					<td class="subtitulodireita">Valor Divergente:</td>
					<td><?php echo $vrlDivergente; ?>
						<input type="hidden" name="empenho[]" id="empenho" value="<?php echo $v['empenho']; ?>">
						<input type="hidden" name="tipo" id="tipo" value="<?php echo $v['tipo']; ?>">
						<input type="hidden" name="notaempenho[<?=$v['empenho']?>]" id="notaempenho" value="<?php echo $v['notaempenho']; ?>">
						<input type="hidden" name="especieempenho[<?=$v['empenho']?>]" id="especieempenho" value="<?php echo $v['especie']; ?>">
						<input type="hidden" name="vrlempenhado[<?=$v['empenho']?>]" id="vrlempenhado" value="<?php echo $v['vrlempenho']; ?>">
						<input type="hidden" name="vrlcomposicao[<?=$v['empenho']?>]" id="vrlcomposicao" value="<?php echo $v['vrlempcomposicao']; ?>">
						<input type="hidden" name="vrldivergente[<?=$v['empenho']?>]" id="vrldivergente" value="<?php echo $vrlDivergente; ?>">
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Espécie NE:</td>
					<td colspan="3"><?php echo $v['especie']; ?></td>
				</tr>
			</table>
			<?exibirHistoricoSigef($processo); ?>
			<table style="margin-top: 10px;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td>
						<?php 
						
						$sql = "SELECT TRUE FROM par.processopar WHERE prpnumeroprocesso = '$processo' AND prpstatus = 'A'";
						
						$testaProcessoSub = $db->pegaUm($sql);
						
						if( $testaProcessoSub != 't' ){
						?>
						<script>
						jQuery(document).ready(function(){
							jQuery('#btn_mostra_obras').live('click',function(){
								if( jQuery(this).attr('src') == '../imagens/menos.gif' ){
									jQuery('.obras').hide();
									jQuery(this).attr('src','../imagens/mais.gif');
									jQuery(this).attr('title','Mostrar');
								}else{
									jQuery('.obras').show();
									jQuery(this).attr('src','../imagens/menos.gif');
									jQuery(this).attr('title','Esconder');
								}
							});
						});
						</script>
						<div style="text-align: left;font-size:12px;" class="TituloTela">
							<img align="absmiddle" id="btn_mostra_obras" style="cursor:pointer;" title="Mostrar" src="../imagens/mais.gif">
							Lista de Obras
						</div>
						<div class="obras" style="display:none">
							<?php
							$sql = "SELECT 
										soe.ne,
										pre.predescricao,
										to_char(pre.prevalorobra,'L9G999G990D99') as prevalorobra,
										'% '||to_char((soe.saldo/pre.prevalorobra*100)::numeric(50,2),'999D99') as perc,
										soe.saldo
									FROM  
										par.v_saldo_obra_por_empenho  soe
									INNER JOIN obras.preobra pre ON pre.preid = soe.preid
									WHERE 
										processo = '$processo'
									ORDER BY
										soe.ne,pre.predescricao";
	
							$cabeçalho = Array('NE', 'Nome da Obra', 'Valor da Obra', '% Empenho', 'Valor Empenhado');
	    					$db->monta_lista_simples($sql, $cabeçalho,1000000,5,'S','100%', 'S', '', '', '', true);
	    					?>
						</div>
						<?php 
						}
						carregaEmpenhoPorProcessoTelaDivergente( $dados );
						?>
					</td>
				</tr>
			</table>
			<table align="center" border="0" class="tabela"  style="width: 100%" cellpadding="3" cellspacing="1">
<?php 	if( empty($arrEmpenho[0]) ){ ?>
				<tr>
					<td class="subtitulodireita" style="text-align: left;">
						<input type="button" name="btnAdd" id="btnAdd" value="<?php echo $buttonLabel;?>" style="font-size: 14px;" onclick="adcionaSubacaoaoEmpenho('<?php echo $v['empenho'];?>', '<?php echo $v['processo'];?>', '<?php echo $v['tipo']; ?>')"></td>
				</tr>
<?php 	}?>
				<tr>
					<td>
						<table width="100%" align="left" cellspacing="0" cellpadding="2" border="0" class="listagem" style="color:333333;">
							<thead>
								<tr>
									<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Código</td>
									<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="45%">Descrição</td>
									<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="05%">Ano</td>
									<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%"><?php echo $label;?></td>
									<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Valor Atual Empenhado</td>
									<!--  <td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Valor Disponível</td> -->
									<td valign="middle" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" width="10%">Valor Distribuir</td>
								</tr>
							</thead>
							<tbody>
<?php		if( $arrEmpenho ){
				$vrlTotalItem = 0;
				$vrlTotalEmpenho = 0;
				
				foreach ($arrEmpenho as $chave => $emp ) {
					
					$vrlTotalItem = (float)$vrlTotalItem + (float)$emp['vrlitem'];
					$vrlTotalEmpenho = (float)$vrlTotalEmpenho + (float)$emp['vlrempenhado'];
					
					//$valordistibuir = ( round((float)$emp['valordistibuir'], 2) + round((float)$emp['valordisponivel'], 2));
					if( $v['especie'] == '03' || $v['especie'] == '04' || $v['especie'] == '13' ){
						$valordistibuir = (float)$emp['valordistibuir'];
					} else {
						$valordistibuir = ( (float)$emp['valordistibuir'] + (float)$emp['valordisponivel']);
					}
					
					if( $v['especie'] == '03' || $v['especie'] == '04' || $v['especie'] == '13' ){
						$valordisponivel = $emp['valordistibuir'];
					} else {
						$valordisponivel = $emp['valordisponivel'];						
					}
					
					$chave % 2 ? $cor = "#dedfde" : $cor = ""; ?>
								<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';" id="tr_<?=$v['empenho']?>_<?=$emp['codigo']?>">
									<td><?php echo $emp['local'];?></td>
									<td><?php echo $emp['descricao'];?></td>
									<td align="right"><?php echo $emp['ano'];?></td>
									<td><?php echo number_format( $emp['vrlitem'], 2, ',', '.' );?></td>
									<td><?php echo number_format( $emp['vlrempenhado'], 2, ',', '.' );?></td>
									<!-- <td><?php echo number_format( $valordistibuir, 2, ',', '.' );?></td> -->
									<td align="right">
										<input class="normal focuscampo maskMoney" type="text" title="Valor do Empenho" onblur="MouseBlur(this);validaValorInformado(<?php echo $v['empenho']?>, <?php echo $emp['codigo']?>)" 
										onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
										value="<?php echo number_format(0, 2, ',', '.' ); ?>" maxlength="30" size="11" name="empvalor[<?php echo $v['empenho']?>][<?php echo $emp['codigo']?>]" style="text-align: right; font-size: 14px;">							
										
										<input type="hidden" name="ano[<?php echo $v['empenho']?>][<?php echo $emp['codigo']?>]" id="ano" value="<?php echo $emp['ano']; ?>">
										<input type="hidden" name="codigo[<?=$v['empenho']?>][]" id="codigo" value="<?php echo $emp['codigo']; ?>">
										<input type="hidden" name="valordisponivel[<?=$v['empenho']?>][<?=$emp['codigo']?>]" id="valordisponivel" value="<?php echo $valordisponivel; ?>">
										<input type="hidden" name="valordistibuir[<?=$v['empenho']?>][<?=$emp['codigo']?>]" id="valordistibuir" value="<?php echo $emp['valordistibuir']; ?>">
										<input type="hidden" name="vrlcancelado[<?=$v['empenho']?>][<?=$emp['codigo']?>]" id="vrlcancelado" value="<?php echo $emp['vrlcancelado']; ?>">
										<input type="hidden" name="descricao[<?=$v['empenho']?>][<?=$emp['codigo']?>]" id="descricao" value="<?php echo $emp['descricao']; ?>">
									</td>
								</tr>
<?php 			} ?>
								<tr>
									<td class="subtitulodireita" colspan="3">Valor Total:</td>
									<td style="text-align: left;" bgcolor="#DCDCDC"><?php echo number_format($vrlTotalItem, 2, ',', '.' );?></td>
									<td style="text-align: left;" bgcolor="#DCDCDC"><?php echo number_format($vrlTotalEmpenho, 2, ',', '.' );?></td>
									<td style="text-align: right;" bgcolor="#DCDCDC"><input type="text" id="vrltotalDistribuido" name="vrltotalDistribuido[<?=$v['empenho']?>]" size="11" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
								</tr>
								<tr>
									<td class="subtitulodireita" colspan="5">Valor Total Restante:</td>
									<td style="text-align: right;" bgcolor="#DCDCDC"><input type="text" id="vrltotalRestante" name="vrltotalRestante[<?=$v['empenho']?>]" size="11" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
								</tr>
<?php 		} else { ?>
								<tr><td align="center" style="color:#cc0000;" colspan="10">Não foram encontrados Registros de Empenho.</td>
									<tr>
									<td><input type="hidden" id="vrltotalDistribuido" name="vrltotalDistribuido[<?=$v['empenho']?>]" size="11" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td></tr>
									<td><input type="hidden" id="vrltotalRestante" name="vrltotalRestante[<?=$v['empenho']?>]" size="11" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
								</tr>
<?php 		}?>
							</tbody>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<?php 
	}
	echo '</form>'; 
}

function salvarEmpenhoDivergente( $dados ){
	global $db;
	//23400018036201365
	$tipo = $dados['tipo'];
	$coluna = '';
	
	if( $tipo == 'PAR' ){
		$tabela = 'par.empenhosubacao';
		$chave = 'sbaid';
		$coluna = ', eobano';
	}
	if( $tipo == 'OBRA' ){
		$tabela = 'par.empenhoobrapar';
		$chave = 'preid';
	}
	if( $tipo == 'PAC' ){
		$tabela = 'par.empenhoobra';
		$chave = 'preid';
	}
	//ver($dados,d);
	if( is_array($dados['empenho']) && !empty($dados['empenho'][0]) ){
		foreach ($dados['empenho'] as $empenho) {
			$arrCodigo 	= $dados['codigo'][$empenho];
			$especie 	= $dados['especieempenho'][$empenho];
			
			$arrCodigo 	= $arrCodigo ? $arrCodigo : array();
			
			foreach ($arrCodigo as $codigo) {
				$empvalor 	 	= $dados['empvalor'][$empenho][$codigo];
				$empvrlAtual 	= $dados['valordistibuir'][$empenho][$codigo];
				$vrlcancelado 	= $dados['vrlcancelado'][$empenho][$codigo];
				$ano 		 	= $dados['ano'][$empenho][$codigo];
				
				$empvalor 	 	= retiraPontosBD($empvalor);
				
				if( $especie == '01' ){
					$empvalor = ((float)$empvalor + (float)$empvrlAtual);
				}elseif( in_array($especie, array('03', '04', '13')) ){
					$eobvalorempenho = $db->pegaUm("select sum(eobvalorempenho) from $tabela where $chave = $codigo and empid = $empenho and eobstatus = 'A'");
					$empvalor = ((float)$empvalor + $eobvalorempenho + (float)$vrlcancelado);
					
				}
				
				$boTem = $db->pegaUm("select count(eobid) from $tabela where $chave = $codigo and empid = $empenho and eobstatus = 'A'");
				
				if( (int)$boTem > (int)0 ){
					$sql = "UPDATE $tabela SET eobvalorempenho = ".$empvalor." WHERE $chave = $codigo and empid = $empenho ";
					$db->executar($sql);
				} else {
					if( (float)$empvalor > (float)0 ){
						$sql = "INSERT INTO $tabela($chave, empid, eobpercentualemp, eobvalorempenho $coluna)
								VALUES ($codigo, $empenho, 0, {$empvalor} ".($coluna ? ", '{$ano}'" : '').")";
						$db->executar($sql);
					}
				}
			}
		}
		
		if($db->commit()){
			if( $tipo == 'PAR' ){
				$db->sucesso('principal/solicitacaoEmpenhoPar', '&processo='.$dados['processo'].'&prpid='.$dados['prpid'].'&inuid='.$dados['inuid']);
			}
			if( $tipo == 'OBRA' ){
				$db->sucesso('principal/solicitacaoEmpenhoObrasPar', '&processo='.$dados['processo'].'&proid='.$dados['proid'].'&inuid='.$dados['inuid']);
			}
			if( $tipo == 'PAC' ){
				$db->sucesso('principal/solicitacaoEmpenho', '&processo='.$dados['processo'].'&proid='.$dados['proid']);
			}
		}
	}
}

function botaoGerarTermoEX($processo, $tipo = 1){

	global $db;
	
	if( $tipo == 1 ){ //prpid
		$dado = "AND dp.prpid IS NOT NULL AND dp.prpid = ".$processo;
	} else { //proid
		$dado = "AND dp.proid IS NOT NULL AND dp.proid = ".$processo;
	}
	
	$sql = "SELECT 
				dp.dopid,
				mdo.mdoid as mdoid_original,
				mdo.tpdcod as tpdcod_original,
				mdo2.mdoid as mdoid_ex,
				mdo2.tpdcod as tpdcod_ex,
				mdo.mdodocumentoex
			FROM 
				par.vm_documentopar_ativos dp
			INNER JOIN par.modelosdocumentos mdo  ON mdo.mdoid = dp.mdoid AND mdo.mdostatus = 'A'
			LEFT  JOIN par.modelosdocumentos mdo2 ON mdo2.mdotipovinculado = mdo.mdoid AND mdo2.mdodocumentoex = 't' AND mdo2.mdostatus = 'A'
			WHERE
				dp.dopdatainiciovigencia IS NOT NULL
				AND dp.dopdatafimvigencia IS NOT NULL
				AND LENGTH( dopdatafimvigencia ) = 7
				AND dp.dopnumerodocumento IS NOT NULL
				AND dp.dopid NOT IN (select dopidoriginal from par.reprogramacao where repstatus = 'A')
				AND dp.dopid NOT IN (select dopid from par.documentoparreprogramacao where dprstatus IN ('P','A') AND dprvalidacao NOT IN ('f'))
				{$dado}";

	$dadosDocumento = $db->pegaLinha( $sql );
//	ver($sql, $dadosDocumento);
	if( $dadosDocumento['mdodocumentoex'] == 't' ){
		$dadosDocumento['mdoid_ex']  = $dadosDocumento['mdoid_original']; 
		$dadosDocumento['tpdcod_ex'] = $dadosDocumento['tpdcod_original']; 
	}
	
	return $dadosDocumento;
	
}

function geraTermoEx($dopidEX, $vigenciaEX, $tipo = 1){
	global $db;
	
	if( $tipo == 1 ){ //prpid
		$processo = "prpid";
	} else { //proid
		$processo = "proid";
	}
	
	$sql = "SELECT 
				dp.dopid,
				mdo.mdoid as mdoid,
				mdo.tpdcod as tpdcod,
				mdo2.mdoid as mdoid_ex,
				mdo2.tpdcod as tpdcod_ex,
				dp.dopvalortermo, 
				dp.{$processo} as processo,
				dp.dopnumerodocumento,
				mdo.mdodocumentoex
			FROM 
				par.vm_documentopar_ativos dp
			INNER JOIN par.modelosdocumentos mdo  ON mdo.mdoid = dp.mdoid
			LEFT  JOIN par.modelosdocumentos mdo2 ON mdo2.mdotipovinculado = mdo.mdoid AND mdo2.mdodocumentoex = 't'
			WHERE
				dp.{$processo} is not null
				AND dp.dopdatainiciovigencia IS NOT NULL
				AND dp.dopdatafimvigencia IS NOT NULL
				AND LENGTH( dopdatafimvigencia ) = 7
				AND dp.dopnumerodocumento IS NOT NULL
				AND dp.dopid not in (select dopidoriginal from par.reprogramacao where repstatus = 'A')
				AND dp.dopid = ".$dopidEX;
	$dado = $db->pegaLinha($sql);

	if( $dado['mdodocumentoex'] == 't' ){
		$dado['mdoid_ex'] = $dado['mdoid'];
		$dado['tpdcod_ex'] = $dado['tpdcod'];
	}
	
	if( !empty($dado['processo']) ){
		if( $tipo == 1 ){ //prpid
			$processo = $db->pegaUm("select prpnumeroprocesso from par.processopar where prpid = {$dado['processo']}");
			verificaEmpenhoSigef( $processo );
		} else { //proid
			$processo = $db->pegaUm("select pronumeroprocesso from par.processoobraspar where proid = {$dado['processo']}");
			verificaEmpenhoSigef( $processo );
		}
	}
	
	$sql = "INSERT INTO par.historicotermosex ( dopidoriginal, htedata, usucpf, htevigencia ) VALUES (".$dopidEX.", 'NOW()', '".$_SESSION['usucpf']."', '".$vigenciaEX."')";
	$db->executar($sql);
	
	$sql = "SELECT
				 prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
			  mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
			  dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
			  dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
			  dopreformulacao, dopidpai, dopdatainiciovigencia
			FROM par.documentopar WHERE dopid = {$dado['dopid']} and dopstatus = 'A'";
		  
	$arrDadosDoc = $db->pegaLinha( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
	
	unset($vigencia);
	if(strlen($arrDadosDoc['dopdatainiciovigencia']) == 7){
		$mes = substr($arrDadosDoc['dopdatainiciovigencia'], 0, 2);
		$ano = substr($arrDadosDoc['dopdatainiciovigencia'], 3, 4);
		$vigencia = "01/".$mes."/".$ano;
	} else {
		$mes = substr($arrDadosDoc['dopdatainiciovigencia'], 3, 2);
		$ano = substr($arrDadosDoc['dopdatainiciovigencia'], 6, 4);
		$vigencia = "01/".$mes."/".$ano;
	}
	$retorno = true;
	if( $vigencia ){
		$retorno = validaData(formata_data_sql($vigencia));
	}
	
	if( $retorno == false ){
		return false;
		exit();
	}
	
	$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$dado['mdoid_ex'];
	$arrModelo = $db->pegaLinha($sql);
	
	$subacoes = array();
	
	if( $tipo == 1 ){
		$sql = "SELECT DISTINCT sbdid as chk FROM par.termocomposicao tc INNER JOIN par.documentopar dp ON dp.dopnumerodocumento = tc.dopid WHERE dp.dopid = ".$dado['dopid'];
		$subacoes = $db->carregarColuna($sql);
		$chk = $subacoes; 
	} else {
		$sql = "SELECT DISTINCT preid as chk FROM par.termocomposicao tc INNER JOIN par.documentopar dp ON dp.dopnumerodocumento = tc.dopid WHERE dp.dopid = ".$dado['dopid'];
		$preids = $db->carregarColuna($sql);
		$chk = $preids; 
	}
	
	if( !$subacoes[0] ){
		$sql = "SELECT 
					sd.sbdid 
				FROM 
					par.processopar prp
				INNER JOIN par.processoparcomposicao ppc on ppc.prpid = prp.prpid
				INNER JOIN par.documentopar dp on dp.prpid = prp.prpid
				INNER JOIN par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
				WHERE
					dp.dopid = ".$dado['dopid'];
		
		$subacoes = $db->carregarColuna($sql);
		$chk = $subacoes; 
		
		$sqlSub = "";
		if( is_array($subacoes) ){
			foreach( $subacoes as $sbdid ){
				$sqlSub .= "INSERT INTO par.termocomposicao (dopid, sbdid) VALUES (".$dado['dopid'].",".$sbdid."); ";
			}
		}
		if($sqlSub){
			$db->executar($sqlSub);
		}
	}
	
	$mdoconteudo = $arrModelo['mdoconteudo'];
	
	$mdoid = $dado['mdoid_ex'];
	$tpdcod = $dado['tpdcod_ex'];
	
	$post = array('mdoid' => $mdoid, 'dopid' => $dopidEX, 'tpdcod' => $tpdcod, 'chk' => $chk);
	
	$_SESSION['par']['cronogramaFinal'] = $vigenciaEX;
	$_SESSION['par']['cronogramainicial'] = $arrDadosDoc['dopdatainiciovigencia'];

	if( $tipo == 1 ){
		$doptexto = alteraMacrosMinuta($mdoconteudo, $dado['processo'], $post);
	} else {
		if(strpos($mdoconteudo, '#Justificativa#')) $boJustificativa = true;
		if(strpos($mdoconteudo, '#Objeto_Convenio#')) $boObjeto = true;
		if(strpos($mdoconteudo, '#Numero_Dias#') || strpos($mdoconteudo, '#Data_Fim_Vigencia') || strpos($mdoconteudo, '#Numero_Dias_Prorrogados#')
			|| strpos($mdoconteudo, '#Nova_Vigencia_Inicio#') || strpos($mdoconteudo, '#Nova_Vigencia_Fim#') || strpos($mdoconteudo, '#Data_Celebracao#')) $boDatas = true;
		$doptexto = alteraMacrosMinutaObras($mdoconteudo, $dado['processo'], $post);
	}
	
	$arrDadosDoc['mdoid'] = $mdoid;
	$arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
	$arrDadosDoc['dopdatainicio'] = $vigencia;
	$arrDadosDoc['dopdatafim'] = '28/'.$vigenciaEX;
	$arrDadosDoc['dopdatafimvigencia'] = $vigenciaEX;
	
	if( $tipo == 1 ){
		$arrDadosDoc['prpid'] = $dado['processo'];
		$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $dado['dopid']);
	} else {
		$arrDadosDoc['proid'] = $dado['processo'];
		$dopid = salvarDadosMinutaObras($arrDadosDoc, $doptexto, $dado['dopid']);
	}

	$repid = "";
	$sqlR  = "";
	$sqlRU = "";

	$sqlR = "SELECT repid FROM par.reprogramacao WHERE dopidoriginal = ".$dado['dopid'];
	$repid = $db->pegaUm($sqlR);
	
	if( $repid ){
		$sqlRU = "UPDATE par.reprogramacao SET dopidoriginal = ".$dopid." WHERE repid = ".$repid."; 
				  UPDATE par.documentoparreprogramacaosubacao SET dopid = ".$dopid." WHERE repid = ".$repid."; ";
		
		$db->executar( $sqlRU );
	}
	
	if( is_array($preids) && $preids[0] ){
		$sqlP = "";
		foreach($preids as $pre){
			$sqlP .= "INSERT INTO par.termocomposicao (dopid, preid) VALUES (".$dopid.", ".$pre."); ";
		}
		$db->executar($sqlP);
	}
	
	if( is_array($subacoes) && $subacoes[0] ){
		$sqlS = "";
		foreach($subacoes as $sub){
			$sqlS .= "INSERT INTO par.termocomposicao (dopid, sbdid) VALUES (".$dopid.", ".$sub."); ";
		}
		$db->executar($sqlS);
		
		$tipoEscola = $db->pegaUm("SELECT count(sbacronograma) FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid WHERE s.sbacronograma = 2 AND sd.sbdid IN (".implode(",", $subacoes).")");
		if( $tipoEscola > 0 ){
			$relatorio = $db->carregar("SELECT * FROM (
											SELECT
												CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.estuf END as uf,
												CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao END as entidade,
												ent.entnome as escola,
												ent.entcodent as codinep,
												(par.retornacodigosubacao(sd.sbaid)) as subacao,
												pic.picdescricao as item,
												CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
													THEN -- escolas sem itens
														sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
													ELSE -- escolas com itens
														CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
															sum(ssi.seiqtdtecnico)
														END
												END as quantidade 
											FROM 
												par.subacaodetalhe sd 
											INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
											INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano
											INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
											INNER JOIN par.subacaoescolas se ON se.sbaid = sd.sbaid AND se.sesano = sd.sbdano
											INNER JOIN par.escolas esc on esc.escid = se.escid
											INNER JOIN entidade.entidade ent ON ent.entid = esc.entid
											INNER JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid AND ssi.sesid = se.sesid
											INNER JOIN par.acao a ON a.aciid = s.aciid
											INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
											INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
											LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
											WHERE 
												sd.sbdid IN (".implode(",", $subacoes).")
											GROUP BY
												ent.entnome, ent.entcodent, sd.sbaid, pic.picdescricao, s.frmid, s.ptsid, sic.icovalidatecnico, iu.itrid, iu.estuf, mun.estuf, mun.mundescricao
											ORDER BY
												escola, subacao, item
										) foo
										WHERE
											foo.quantidade > 0");
			
			$teste = geraAnexoEscolas($relatorio, $dopid);
		}
	}
	
	$sql = "UPDATE par.historicotermosex SET dopidex = $dopid WHERE dopidoriginal = $dopidEX;";
	
	$db->executar($sql);
	
	$db->commit();
	
	unset($_SESSION['par']['totalVLR']);
	return '1';
		/*
	} else {
		$sql = "DELETE FROM par.historicotermosex WHERE dopidoriginal = ".$dopidEX;
		$db->executar($sql);
		
		unset($_SESSION['par']['totalVLR']);
		return '2';
*/	

}

function verificaEmpenhoSigef( $processo, $sistema = '' ){
	
	global $db;
	
	$arrEspecie = array('01', '02', '03', '04', '13');
	sort($arrEspecie);
	
	$arrParam = array(
					'wsusuario' 		=> 'USAP_WS_SIGARP',
	        		'wssenha' 			=> '03422625',
					'nu_processo'       => $processo,
					'method'            => 'historicoempenho',
					);
	                    
	$arrRetorno = montaXMLHistoricoProcessoSIGEF( $arrParam );
	$arrRetorno = $arrRetorno ? $arrRetorno : array();
	
	$arrRetorno2 = array();
	foreach($arrRetorno as $dado){
		$arrRetorno2[$dado['cod_especie']][] = $dado;
	}
	
	foreach ($arrEspecie as $especie) {
		$arrRetornoWS = ($arrRetorno2[$especie] ? $arrRetorno2[$especie] : array());
		
		foreach($arrRetornoWS as $key => $v){
			
			$sql = "select coalesce(empid, 0) from par.empenho e where e.empnumeroprocesso = '{$v['numero_do_processo']}' and e.empprotocolo ilike '%{$v['numero_sequencial_da_ne']}%' and empstatus = 'A'";
			$empenho = $db->pegaUm($sql);
			$empenho = $empenho ? $empenho : '0';
			
			$teeid = $db->pegaUm("SELECT teeid FROM execucaofinanceira.tipoespecieempenho WHERE teecodigo = '".$v['cod_especie']."'");
			if( $v['ano_do_empenho'] && $v['numero_da_ne'] ){
				$empnumeroFilho	= "'".$v['ano_do_empenho'].'NE'.$v['numero_da_ne']."'";
			} else {
				$empnumeroFilho = 'null';
			}
			
			$ems_situacao_do_empenho = trim($v['situacao_do_empenho']);
			if( $ems_situacao_do_empenho == 'EFETIVADO' ) $ems_situacao_do_empenho = '2 - EFETIVADO';
		
			
			if( (int)$empenho == (int)0 ){ // Se ele não existe na base
			
				$sql = "SELECT empnumeroprocesso, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empvalorempenho, empcodigoespecie, 
							empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente, 
						    ds_problema, valor_saldo_pagamento, tp_especializacao, co_diretoria, empid, usucpf, empid
						FROM par.empenho e
						WHERE empnumeroprocesso = '{$v['numero_do_processo']}'
							and empprotocolo = '{$v['nu_seq_mov_ne']}' 
							and empstatus = 'A' 
						";
				$arEmpPai = $db->pegaLinha($sql);
				$arEmpPai = $arEmpPai ? $arEmpPai : array();
				
				if( !empty($arEmpPai) ){ // Se ele é filho de alguém
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
					$v['numero_da_ne'] = $v['numero_da_ne'] ? $v['numero_da_ne'] : 'null';
					$v['ano_do_empenho'] = $v['ano_do_empenho'] ? $v['ano_do_empenho'] : 'null';
					$v['valor_da_ne'] = $v['valor_da_ne'] ? $v['valor_da_ne'] : 'null';
					$v['data_do_empenho'] = $v['data_do_empenho'] == trim('--') ? date('Y-m-d') : $v['data_do_empenho'];
					
					// insere empenho filho
					$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empcodigopi, 
								empcodigoesfera, empcodigoptres, empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, 
								empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
			  					empprogramafnde, empnumerosistema, empsituacao, usucpf, empprotocolo, empnumero, 
			  					empvalorempenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento,
			  					empdata, tp_especializacao, co_diretoria, empidpai, teeid, empcarga)				
							VALUES('{$v['cnpj']}', {$v['numero_da_ne']}, {$v['ano_do_empenho']}, '{$v['numero_do_processo']}', '{$v['cod_especie']}', '{$v['pi']}', 
								'{$v['esfera']}', '{$v['ptres']}', '{$v['fonte_de_recurso']}', '{$v['natureza_de_despesa']}', '$empcentrogestaosolic', $empanoconvenio, $empnumeroconvenio, 
								'$empcodigoobs', '$empcodigotipo', '$empdescricao', '$empgestaoeminente', '$empunidgestoraeminente',
			  					'{$v['programa_fnde']}', '{$v['nu_id_sistema']}', '{$ems_situacao_do_empenho}', '$usucpf', '{$v['numero_sequencial_da_ne']}', {$empnumeroFilho}, 
			  					{$v['valor_da_ne']}, '$ds_problema', {$v['valor_da_ne']}, $valor_saldo_pagamento,
			  					'{$v['data_do_empenho']}', '$tp_especializacao', '$co_diretoria', '$empid', {$teeid}, 'S') returning empid";
			  					
					$empidNovo = $db->pegaUm($sql);
					
					$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho)
		    				VALUES ('', $empidNovo, '{$v['data_do_empenho']}', '2 - EFETIVADO', '{$v['cod_especie']}');";
					$db->executar($sql);
					
					insereComposicaoEmpenho($v, $empidNovo, $sistema);
					
				} else { // Se não é filho
					$empcentrogestaosolic 	= $v['centro_de_gestao'];
					$empvalorempenho 		= $v['valor_da_ne'];
					$empcodigoobs 			= '2';
					$empcodigotipo 			= '3';
					$empdescricao 			= '0010';
					$empgestaoeminente 		= '15253';
					$empunidgestoraeminente = $v['unidade_gestora'];
					$valor_saldo_pagamento	= '0.00';
					if( $v['ano_do_empenho'] && $v['numero_da_ne'] ){
						$empnumero	= "'".$v['ano_do_empenho'].'NE'.$v['numero_da_ne']."'";
					} else {
						$empnumero = 'null';
					}
					$usucpf					= '';
		
					$valor_saldo_pagamento = $valor_saldo_pagamento ? $valor_saldo_pagamento : 'null';
					$v['numero_da_ne'] = $v['numero_da_ne'] ? $v['numero_da_ne'] : 'null';
					$v['ano_do_empenho'] = $v['ano_do_empenho'] ? $v['ano_do_empenho'] : 'null';
					$v['valor_da_ne'] = $v['valor_da_ne'] ? $v['valor_da_ne'] : 'null';
					$v['data_do_empenho'] = ($v['data_do_empenho'] == trim('--')) ? date('Y-m-d') : $v['data_do_empenho'];
		
					$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empnumeroprocesso, empcodigoespecie, empcodigopi, 
								empcodigoesfera, empcodigoptres, empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, 
								empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
			  					empprogramafnde, empnumerosistema, empsituacao, usucpf, empprotocolo, empnumero, 
			  					empvalorempenho, valor_total_empenhado, valor_saldo_pagamento,
			  					empdata, empidpai, teeid, empstatus, empcarga)				
							VALUES('{$v['cnpj']}', {$v['numero_da_ne']}, {$v['ano_do_empenho']}, '{$v['numero_do_processo']}', '{$v['cod_especie']}', '{$v['pi']}', 
								'{$v['esfera']}', '{$v['ptres']}', '{$v['fonte_de_recurso']}', '{$v['natureza_de_despesa']}', '$empcentrogestaosolic', 
								'$empcodigoobs', '$empcodigotipo', '$empdescricao', '$empgestaoeminente', '$empunidgestoraeminente',
			  					'{$v['programa_fnde']}', '{$v['nu_id_sistema']}', '{$ems_situacao_do_empenho}', '$usucpf', '{$v['numero_sequencial_da_ne']}', {$empnumero}, 
			  					{$v['valor_da_ne']}, {$v['valor_da_ne']}, $valor_saldo_pagamento,
			  					'{$v['data_do_empenho']}', null, {$teeid}, 'A', 'S') returning empid";
					
					$empidNovo = $db->pegaUm($sql);
					
					$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho)
		    				VALUES ('', $empidNovo, '{$v['data_do_empenho']}', '2 - EFETIVADO', '{$v['cod_especie']}');";
					$db->executar($sql);
					
					insereComposicaoEmpenho($v, $empidNovo, $sistema);
				}
					
			} else { // Já existe na base. Atualizo.
				
				if( $v['ano_do_empenho'] && $v['numero_da_ne'] ){
					$empnumero	= "'".$v['ano_do_empenho'].'NE'.$v['numero_da_ne']."'";
				} else {
					$empnumero = 'null';
				}
				
				$empnumerooriginal = ($v['numero_da_ne'] ? "'".$v['numero_da_ne']."'" : 'null');
				$empanooriginal = ($v['ano_do_empenho'] ? "'".$v['ano_do_empenho']."'" : 'null');
				
				$sql = "UPDATE par.empenho SET
					 		empsituacao 			= '{$ems_situacao_do_empenho}',
							empnumero			 	= {$empnumero},
							empprotocolo		 	= {$v['numero_sequencial_da_ne']},
							empnumerooriginal		= {$empnumerooriginal},
							empanooriginal			= {$empanooriginal},
							".($v['valor_da_ne'] ? "empvalorempenho = '".$v['valor_da_ne']."'," : '')."
							".($v['valor_da_ne'] ? "valor_total_empenhado = '".$v['valor_da_ne']."'," : '')."
							empstatus 				= 'A'
						WHERE
							empid = $empenho"; 
				
				$db->executar($sql);
				$db->commit();
				
				AtualizaComposicaoEmpenho( $v, $empenho, $sistema );
			}
		}
	}
	return true;
}

function insereComposicaoEmpenho( $arrParam = array(), $empidNovo, $sistema = '' ){
	global $db;
	
	if( empty($sistema) ){
		$sql = "select
					tipo
				from(
				    select prpnumeroprocesso as processo, 'PAR' as tipo
				    from par.processopar where prpstatus = 'A'
				    union
				    select pronumeroprocesso as processo, 'OBRAS' as tipo
				    from par.processoobraspar where prostatus = 'A'
				    union
				    select pronumeroprocesso as processo, 'PAC' as tipo
				    from par.processoobra where prostatus = 'A'
				) as foo
				where
					processo = '{$arrParam['numero_do_processo']}'";
		$tipo = $db->pegaUm($sql);
	} else {
		$tipo = $sistema;
	}
	
	if( $tipo == 'PAR' ){
		if( in_array( trim($arrParam['cod_especie']), array('03', '04', '13')) ){
			$sql = "select count(o.sbaid) from par.empenho e
						inner join par.empenhosubacao o on o.empid = e.empid and o.eobstatus = 'A'
					where
						e.empnumeroprocesso = '{$arrParam['numero_do_processo']}'
						and e.empprotocolo = '{$arrParam['nu_seq_mov_ne']}'";
			$totSub = $db->pegaUm($sql);
		} else {
			$sql = "select count(sd.sbaid) from par.processopar p
						inner join par.processoparcomposicao pp on pp.prpid = p.prpid
					    inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
					where p.prpnumeroprocesso = '{$arrParam['numero_do_processo']}'";
			$totSub = $db->pegaUm($sql);
		}
		
		if( (int)$totSub == (int)1 ){
			$sql = "select sd.sbaid, sd.sbdano from 
					par.processopar p
						inner join par.processoparcomposicao pp on pp.prpid = p.prpid
						inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
					where p.prpnumeroprocesso = '{$arrParam['numero_do_processo']}'";
			$arDetalhe = $db->pegaLinha($sql);
			
			$sbaid 				= $arDetalhe['sbaid'];
			$sbdano 			= $arDetalhe['sbdano'];						
			$valor_da_ne 		= $arrParam['valor_da_ne'];						
			$percent1 			= 1;
			
			$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano) 
					VALUES ({$sbaid}, {$empidNovo}, 0, {$valor_da_ne}, '{$sbdano}')";						
			$db->executar($sql);
		}
	} elseif( $tipo == 'OBRAS' ){
		if( in_array( trim($arrParam['cod_especie']), array('03', '04', '13')) ){
			$sql = "select o.preid from par.empenho e
					inner join par.empenhoobrapar o on o.empid = e.empid and o.eobstatus = 'A'
					where
						e.empnumeroprocesso = '{$arrParam['numero_do_processo']}'
						and e.empprotocolo = '{$arrParam['nu_seq_mov_ne']}'";
			$totObra = $db->carregarColuna($sql);
		} else {
			$sql = "select pp.preid from par.processoobraspar p
						inner join par.processoobrasparcomposicao pp on pp.proid = p.proid
					where p.pronumeroprocesso = '{$arrParam['numero_do_processo']}'";
			$totObra = $db->carregarColuna($sql);
		}
		
		if( (int)sizeof($totObra) == (int)1 ){
			
			$valor_da_ne 	= $arrParam['valor_da_ne'];
			
			$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
					VALUES ({$totObra[0]}, $empidNovo, 0, $valor_da_ne, 0)";						
			$db->executar($sql);
		}
	} elseif( $tipo == 'PAC' ){
		if( in_array( trim($arrParam['cod_especie']), array('03', '04', '13')) ){
			$sql = "select o.preid from par.empenho e
						inner join par.empenhoobra o on o.empid = e.empid and o.eobstatus = 'A'
					where
						e.empnumeroprocesso = '{$arrParam['numero_do_processo']}'
						and e.empprotocolo = '{$arrParam['nu_seq_mov_ne']}'";
			$totObra = $db->carregarColuna($sql);
		} else {
			$sql = "select pp.preid from par.processoobra p
						inner join par.processoobraspaccomposicao pp on pp.proid = p.proid
					where p.pronumeroprocesso = '{$arrParam['numero_do_processo']}'";
			$totObra = $db->carregarColuna($sql);
		}
		
		if( (int)sizeof($totObra) == (int)1 ){
			
			$preid 			= $totObra[0];
			$valor_da_ne 	= $arrParam['valor_da_ne'];
			
			$sql = "INSERT INTO par.empenhoobra(preid, empid, eobvalorempenho, eobpercentualemp2, eobpercentualemp)
					VALUES ($preid, $empidNovo, {$valor_da_ne}, '0', '0')";
			$db->executar($sql);
		}
	}
	$db->commit();
}

function AtualizaComposicaoEmpenho( $arrParam = array(), $empid, $sistema = '' ){
	global $db;
	
	if( empty($sistema) ){
		$sql = "select
					tipo
				from(
				    select prpnumeroprocesso as processo, 'PAR' as tipo
				    from par.processopar where prpstatus = 'A'
				    union
				    select pronumeroprocesso as processo, 'OBRAS' as tipo
				    from par.processoobraspar where prostatus = 'A'
				    union
				    select pronumeroprocesso as processo, 'PAC' as tipo
				    from par.processoobra where prostatus = 'A'
				) as foo
				where
					processo = '{$arrParam['numero_do_processo']}'";
		$tipo = $db->pegaUm($sql);
	} else {
		$tipo = $sistema;
	}
	
	$valor_da_ne = $arrParam['valor_da_ne'];
	
	if( $valor_da_ne ){
		if( $tipo == 'PAR' ){
			if( in_array( trim($arrParam['cod_especie']), array('03', '04', '13')) ){
				$sql = "select o.sbaid, o.eobano as sbdano from par.empenho e
							inner join par.empenhosubacao o on o.empid = e.empid and o.eobstatus = 'A'
						where
							e.empnumeroprocesso = '{$arrParam['numero_do_processo']}'
							and e.empprotocolo = '{$arrParam['nu_seq_mov_ne']}'";
				$totSub = $db->carregar($sql);
			} else {
				$sql = "select sd.sbaid, sd.sbdano from par.processopar p
							inner join par.processoparcomposicao pp on pp.prpid = p.prpid
						    inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
						where p.prpnumeroprocesso = '{$arrParam['numero_do_processo']}'";
				$totSub = $db->carregar($sql);
				$totSub = $totSub ? $totSub : array();
			}
			
			if( (int)sizeof($totSub) == (int)1 && $totSub[0]['sbaid'] != '' ){
				$totComSub = $db->pegaUm("select count(eobid) from par.empenhosubacao where empid = $empid and sbaid = {$totSub[0]['sbaid']} and eobano = '{$totSub[0]['sbdano']}' and eobstatus = 'A'");
				
				if( (int)$totComSub == (int)0 ){
					$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
							VALUES ({$totSub[0]['sbaid']}, {$empid}, 0, {$valor_da_ne}, '{$totSub[0]['sbdano']}')";
					$db->executar($sql);
				}else{					
					$sql = "UPDATE par.empenhosubacao SET eobvalorempenho = {$valor_da_ne}, eobstatus = 'A' WHERE empid = $empid and sbaid = {$totSub[0]['sbaid']} and eobano = '{$totSub[0]['sbdano']}'";
					$db->executar($sql);
				}
			}
		}
		$totObra = array();
		if( $tipo == 'OBRAS' ){			
			if( in_array( trim($arrParam['cod_especie']), array('03', '04', '13')) ){
				$sql = "select o.preid from par.empenho e
							inner join par.empenhoobrapar o on o.empid = e.empid and o.eobstatus = 'A'
						where
							e.empnumeroprocesso = '{$arrParam['numero_do_processo']}'
							and e.empprotocolo = '{$arrParam['nu_seq_mov_ne']}'";
				$totObra = $db->carregarColuna($sql);
			} else {
				$sql = "select pp.preid from par.processoobraspar p
							inner join par.processoobrasparcomposicao pp on pp.proid = p.proid
						where p.pronumeroprocesso = '{$arrParam['numero_do_processo']}'";
				$totObra = $db->carregarColuna($sql);
			}
			
			if( (int)sizeof($totObra) == (int)1 ){
				$totComObra = $db->pegaUm("select count(eobid) from par.empenhoobrapar e where empid = $empid and preid = {$totObra[0]} and eobstatus = 'A'");
				
				if( (int)$totComObra == (int)0 ){						
					$sql = "INSERT INTO par.empenhoobrapar(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
							VALUES ({$totObra[0]}, $empid, 0, $valor_da_ne, 0)";
					$db->executar($sql);
				}else{					
					$sql = "UPDATE par.empenhoobrapar SET eobvalorempenho = {$valor_da_ne}, eobstatus = 'A' WHERE empid = $empid and preid = {$totObra[0]}";
					$db->executar($sql);
				}
			}
		}
		if( $tipo == 'PAC' ){
			
			if( in_array( trim($arrParam['cod_especie']), array('03', '04', '13')) ){
				$sql = "select o.preid from par.empenho e
							inner join par.empenhoobra o on o.empid = e.empid and o.eobstatus = 'A'
						where
							e.empnumeroprocesso = '{$arrParam['numero_do_processo']}'
							and e.empprotocolo = '{$arrParam['nu_seq_mov_ne']}'";
				$totObra = $db->carregarColuna($sql);
			} else {
				$sql = "select pp.preid from par.processoobra p
							inner join par.processoobraspaccomposicao pp on pp.proid = p.proid
						where p.pronumeroprocesso = '{$arrParam['numero_do_processo']}'";
				$totObra = $db->carregarColuna($sql);
			}
			
			if( (int)sizeof($totObra) == (int)1 ){
				$sql = "select count(eobid) from par.empenhoobra e where empid = $empid and preid = {$totObra[0]} and eobstatus = 'A'";
				$totComObra = $db->pegaUm($sql);
				
				if( (int)$totComObra == (int)0 ){
					$sql = "INSERT INTO par.empenhoobra(preid, empid, eobvalorempenho, eobpercentualemp2, eobpercentualemp)
							VALUES ({$totObra[0]}, $empid, {$valor_da_ne}, '0', '0')";
					$db->executar($sql);
				}else{					
					$sql = "UPDATE par.empenhoobra SET eobvalorempenho = {$valor_da_ne}, eobstatus = 'A' WHERE empid = $empid and preid = {$totObra[0]}";			
					$db->executar($sql);
				}
			}
		}
	}
	$db->commit();
}

function atualizaBasePagamentoSigef( $retornoHistorico, $processo, $id = '' ){
	global $db;
	
	$arrRetorno = $retornoHistorico['pagamento'];
	$arrRetorno = $arrRetorno ? $arrRetorno : array();
	
	if( !empty($id) ){
		$filtro = " AND id = $id ";
	}
	
	$sql = "select
				tipo
			from(
			    select prpnumeroprocesso as processo, 'PAR' as tipo, prpid as id
			    from par.processopar where prpstatus = 'A'
			    union
			    select pronumeroprocesso as processo, 'OBRAS' as tipo, proid as id
			    from par.processoobraspar where prostatus = 'A'
			    union
			    select pronumeroprocesso as processo, 'PAC' as tipo, proid as id
			    from par.processoobra where prostatus = 'A'
			) as foo
			where
				processo = '{$processo}'
				$filtro";
	
	$tipo = $db->pegaUm($sql);
	
	foreach ($arrRetorno as $v) {
		
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
		$empid	 				= $db->pegaUm("select empid from par.empenho where empprotocolo = '{$v['nu_seq_mov_ne']}'");
		$nu_processo			= $v['nu_processo'];
		$numeroOB 				= $an_exercicio.'OB'.$nu_documento_siafi;

		$sql = "select
					e.empid,
				    e.empnumero,
				    e.empvalorempenho
				from
					par.empenho e where e.empprotocolo = '{$nu_seq_mov_ne}' and empstatus = 'A'";
		$arrEmpenho = $db->pegaLinha($sql);
		$empenho 	= $arrEmpenho['empid'];
		$numEmpenho = $arrEmpenho['empnumero'];
		
		$sql = "SELECT pagid, pagparcela, paganoexercicio, pagvalorparcela, paganoparcela, pagmes, pagnumeroempenho,
				  	pagsituacaopagamento, pagdatapagamento, parnumseqob, pagdatapagamentosiafi, pagnumeroob
				FROM par.pagamento p
				WHERE
					p.pagstatus = 'A'
				    and p.empid = $empenho and p.parnumseqob = '{$nu_seq_mov_pag}'";
		$arrPagamento = $db->pegaLinha($sql);
				
		$pagid = $arrPagamento['pagid'];
		
		if( $ds_situacao_doc_siafi == 'EFETIVADO' ){
			$ds_situacao_doc_siafi = '2 - EFETIVADO'; 
		}
		
		if( !empty($pagid) ){
			$sql = "UPDATE par.pagamento SET
					  	pagparcela 				= ".($nu_parcela ? "'".$nu_parcela."'" : 'null').",
					  	paganoexercicio 		= ".($an_exercicio ? "'".$an_exercicio."'" : 'null').",
					  	pagvalorparcela 		= ".($vl_parcela ? "'".$vl_parcela."'" : 'null').",
					  	pagmes 					= ".($nu_mes ? "'".$nu_mes."'" : 'null').",
					  	pagsituacaopagamento 	= ".($ds_situacao_doc_siafi ? "'".$ds_situacao_doc_siafi."'" : 'null').",
					  	pagdatapagamento 		= ".($dt_movimento ? "'".$dt_movimento."'" : 'null').",
					  	parnumseqob 			= ".($nu_seq_mov_pag ? "'".$nu_seq_mov_pag."'" : 'null').",
					  	pagnumeroob 			= ".($numeroOB ? "'".$numeroOB."'" : 'null').",
					  	pagstatus 				= 'A'
					WHERE pagid = {$pagid}";
			$db->executar($sql);
		} else {
			$sql = "INSERT INTO par.pagamento(pagparcela, paganoexercicio, pagvalorparcela, paganoparcela, pagmes, pagnumeroempenho, empid, usucpf, pagsituacaopagamento,
	  					pagdatapagamento, parnumseqob, pagstatus, pagdatapagamentosiafi, pagnumeroob)
					VALUES (
					  	".($nu_parcela ? "'".$nu_parcela."'" : 'null').",
					  	".($an_exercicio ? "'".$an_exercicio."'" : 'null').",
					  	".($vl_parcela ? "'".$vl_parcela."'" : 'null').",
					  	".(substr($dt_movimento, 0, 4) ? "'".substr($dt_movimento, 0, 4)."'" : 'null').",
					  	".($nu_mes ? "'".$nu_mes."'" : 'null').",
					  	".($numEmpenho ? "'".$numEmpenho."'" : 'null').",
					  	".($empid ? "'".$empid."'" : 'null').",
					  	'',
					  	".($ds_situacao_doc_siafi ? "'".$ds_situacao_doc_siafi."'" : 'null').",
					  	".($dt_movimento ? "'".$dt_movimento."'" : 'null').",
					  	".($nu_seq_mov_pag ? "'".$nu_seq_mov_pag."'" : 'null').",
					  	'A',
					  	null,
					  	".($numeroOB ? "'".$numeroOB."'" : 'null')."
					) returning pagid";
			$pagid = $db->pegaUm($sql);
			
			$sql = "INSERT INTO par.historicopagamento(pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
					VALUES ({$pagid}, ".($dt_movimento ? "'".$dt_movimento."'" : 'null').", '', 
							".($nu_parcela ? "'".$nu_parcela."'" : 'null').", 
							".($vl_parcela ? "'".$vl_parcela."'" : 'null').", 
							".($ds_situacao_doc_siafi ? "'".$ds_situacao_doc_siafi."'" : 'null').")";
			$db->executar($sql);
		}
		
		if( $tipo == 'PAR' ){
			$sql = "select sd.sbaid, sd.sbdano from par.processopar p
						inner join par.processoparcomposicao pp on pp.prpid = p.prpid
						inner join par.subacaodetalhe sd on sd.sbdid = pp.sbdid
					where p.prpnumeroprocesso = '{$processo}'";
			$totSub = $db->carregar($sql);
			$totSub = $totSub ? $totSub : array();
			
			if( (int)sizeof($totSub) == (int)1 ){
				$boComp = $db->pegaUm("SELECT count(pobid) FROM par.pagamentosubacao WHERE pagid = $pagid");
				
				if( (int)$boComp > (int)0 ){
					$sql = "UPDATE par.pagamentosubacao SET pobvalorpagamento = {$vl_parcela} WHERE sbaid = {$totSub[0]['sbaid']} and pobano = {$totSub[0]['sbdano']} and pagid = $pagid";
					$db->executar($sql);
				}else{
					$sql = "INSERT INTO par.pagamentosubacao(sbaid, pagid, pobpercentualpag, pobvalorpagamento, pobano, pobstatus) 
							VALUES ({$totSub[0]['sbaid']}, $pagid, 0, {$vl_parcela}, {$totSub[0]['sbdano']}, 'A')";
					$db->executar($sql);
				}
			}
		}elseif( $tipo == 'OBRAS' ){
			$sql = "select pp.preid from par.processoobraspar p
						inner join par.processoobrasparcomposicao pp on pp.proid = p.proid
					where p.pronumeroprocesso = '{$processo}'";
			$totObra = $db->carregarColuna($sql);
			$totObra = $totObra ? $totObra : array();
			
			if( (int)sizeof($totObra) == (int)1 ){
				$boComp = $db->pegaUm("SELECT count(popid) FROM par.pagamentoobrapar WHERE pagid = $pagid");

				if( (int)$boComp > (int)0 ){
					$sql = "UPDATE par.pagamentoobrapar SET popvalorpagamento = {$vl_parcela} WHERE pagid = $pagid and preid = {$totObra['0']}";
					$db->executar($sql);
				} else {
					$sql = "INSERT INTO par.pagamentoobrapar(preid, pagid, poppercentualpag, popvalorpagamento)
							VALUES ({$totObra[0]}, $pagid, 0, $vl_parcela)";
					$db->executar($sql);
				}
			}
		}elseif( $tipo == 'PAC' ){
			$sql = "select pp.preid from par.processoobra p
						inner join par.processoobraspaccomposicao pp on pp.proid = p.proid
					where p.pronumeroprocesso = '{$processo}'";
			$totObra = $db->carregarColuna($sql);
			$totObra = $totObra ? $totObra : array();
						
			if( (int)sizeof($totObra) == (int)1 ){
				$boComp = $db->pegaUm("SELECT count(pobid) FROM par.pagamentoobra WHERE pagid = $pagid");
				
				if( (int)$boComp > (int)0 ){
					$sql = "UPDATE par.pagamentoobra SET pobvalorpagamento = {$vl_parcela} WHERE pagid = $pagid and preid = {$totObra['0']}";
					$db->executar($sql);
				} else {
					$sql = "INSERT INTO par.pagamentoobra(preid, pagid, pobpercentualpag, pobvalorpagamento) 
							VALUES({$totObra['0']}, $pagid, 0, {$vl_parcela})";
					$db->executar($sql);
				}
			}
		}
		$db->commit();
	}
	
	return true;
}

function sql_verificaPagamentoMaiorQueEmpenhoSubacao( ){

	$sql = "SELECT
				emp.empid, ems.sbaid, ems.eobano, ems.eobvalorempenho, sum(pas.pobvalorpagamento) as valor_pago
			FROM
				par.empenho emp
			INNER JOIN par.empenhosubacao 	ems ON ems.empid = emp.empid and empstatus = 'A' and eobstatus = 'A'
			INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A'
			INNER JOIN par.pagamentosubacao pas ON pas.pagid = pag.pagid AND pas.sbaid = ems.sbaid AND pas.pobano = ems.eobano
			WHERE
				emp.empnumeroprocesso = %processo%
				AND emp.empsituacao <> 'CANCELADO'
				AND emp.empstatus = 'A'
				AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
				%filtro%
			GROUP BY
				emp.empid, ems.sbaid, ems.eobano, ems.eobvalorempenho
			HAVING
				sum(pas.pobvalorpagamento) > ems.eobvalorempenho";

	return $sql;
}

function sql_verificaPagamentoMaiorQueEmpenhoObra( ){

	$tblPar = '';
	$colPar = 'pob';
	if( $_REQUEST['tooid'] != 1 ){
		$tblPar = 'par';
		$colPar = 'pop';
	}
	
	$sql = "SELECT emp.empid,  pob.preid,  eob.eobvalorempenho, sum( pob.{$colPar}valorpagamento )
			FROM
				par.empenho emp
			INNER JOIN par.empenhoobra$tblPar 	eob ON eob.empid = emp.empid and eobstatus = 'A'
			INNER JOIN par.pagamento	 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' and parnumseqob is not null
			INNER JOIN par.pagamentoobra$tblPar pob ON pob.preid = eob.preid AND pag.pagid = pob.pagid
			WHERE
				emp.empnumeroprocesso = %processo%
				AND emp.empsituacao <> 'CANCELADO'
				AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
				AND emp.empstatus = 'A'
				%filtro%
			GROUP BY
				emp.empid,
				pob.preid,
				eob.eobvalorempenho
			HAVING
				sum( pob.{$colPar}valorpagamento ) > (eob.eobvalorempenho+0.01)";
	
	return $sql;
}

function wf_cond_deferir_obra_aprovada( $preid ){
	
	global $db;
	
	$sql = "SELECT TRUE FROM obras2.obras WHERE preid = $preid";
	
	return !($db->pegaUm($sql) == 't') ? TRUE : 'Esta obra só pode ser reformulada.';
}



function salvarRedistribuirEmpenho($post)
{
	
	global $db;
	
	// Proid do processo
	$proid  =  $_POST['proid'];
	// Array vindo do ajax a ser tratado
	$arrStrCheck = $_POST['arraycheck'];
	// Explodo o array pelo delimitador |
	$arrCheck = explode("|", $arrStrCheck);
	//zereando array que serão populados
	$arrCheckResult = Array();
	$arrInseridos   = Array();
	//Montando o array dos checkboxes
	foreach($arrCheck as $k => $v)
	{
		if ( ! in_array($v, $arrInseridos)) 
		{
			$arrInseridos[] = $v;
			if($v != '')
			{
				$arrCampo = explode("=", $v);
				$preidTemp = str_replace('check_', '', $arrCampo[0]);
				$arrCheckResult[$preidTemp]['preid'] = $preidTemp;
				$arrCheckResult[$preidTemp]['value'] = $arrCampo[1];
			}
		}	
	}
	
	//Total a ser redistribuido
	$total = number_format($post['total'],2,'.', '');
	
	// Array vindo do ajax a ser tratado
	$arrayStrInputs  =  $_POST['arrayinputs'];
	$arrInput = explode("|", $arrayStrInputs);
	//Montando o array dos inputs
	$arrInputResult = Array();
	$arrInseridos   = Array();
	
	foreach($arrInput as $k => $v)
	{
		if ( ! in_array($v, $arrInseridos))
		{
			$arrInseridos[] = $v;
			if($v != '')
			{
				$arrCampo = explode("=", $v);
				$preidTemp = str_replace('input_', '', $arrCampo[0]);
				$arrInputResult[$preidTemp]['preid'] = $preidTemp;
				$arrInputResult[$preidTemp]['value'] = str_replace(',' , '.' , str_replace('.','', $arrCampo[1]));
			}
		}
	}
	
	// variaveis do post
	$tipo 				= $post['tipo'];
	$preidDistribuir 	= $post['preidDistribuir'];
	$empIdDistrubir     = $post['empidDistribuir'];
	
	// separa por tipo
	if($tipo == 'PAC')
	{
		$tabela = 'par.empenhoobra';
		
	}	
	elseif($tipo == 'PAR')
	{
		$tabela = ' par.empenhoobrapar';
	}
	
	$valorInserido = 0;
	$arrInsert = Array();
	// Verificar quais foram marcados para serem distribuidos
	foreach($arrCheckResult as $preid => $arrVal )
	{
		if($arrVal['value'] != 'false')
		{
			// quando seleciona e não informar valor retorna erro
			if( ! $arrInputResult[$preid]['value'] )
			{
				echo "ERRO|A obra {$preid} foi selecionada, porém, o valor não foi informado.";
				die();
			}
			// Valor total do valor inseriro (incrementando)
			$valorInserido = $valorInserido + $arrInputResult[$preid]['value'];
			// Popula array para inserir no banco posteriormente
			$arrInsert[$preid]['valor'] =  $arrInputResult[$preid]['value'];
			$arrInsert[$preid]['preid'] =  $arrInputResult[$preid]['preid'];
		}	
	}
	
	// Verificando se houve algo para inserir
	$arrInsert = (is_array($arrInsert)) ? $arrInsert : Array();
	// Caso o valor todo não seja distribuido, a mais ou a menos retorna erro
	/* if($valorInserido != $valorDistribuir )
	{
		echo "ERRO|É necessário distribuir todo o valor do empenho selecionado.";
		die();
		
	}
	else
	{ */
		// Caso tenha o que inserir:
		if(count($arrInsert) > 0)
		{
			// Insere linha alinha
			foreach($arrInsert as $preid => $valInsert )
			{
				// pega os empenhos daquela obra
				$sql = "SELECT
						    cast(p.prevalorobra as numeric(20,2)) as prevalorobra,
						    v.saldo as valorempenhadoobra
						FROM 
						    obras.preobra p
						    left join par.vm_saldo_empenho_por_obra v on v.preid = p.preid
						where p.preid = {$preid}";
				$resultInsert = $db->pegalinha($sql);
				$resultInsert = (is_array($resultInsert)) ? $resultInsert : false;
				 
				if($resultInsert)
				{
					$valorObra 			= $resultInsert['prevalorobra'];
					$valorEmpenhado	    = $resultInsert['valorempenhadoobra'];
					$valorInserir 		= $valInsert['valor'];
					
					$valorTotalEmpenhado = $valorInserir + $valorEmpenhado;
					
					$vrlDistribuir = (float)$valorObra - (float)$valorEmpenhado;
					
// 					Valida se o valor a inserir não é maior do que o valor total da obra
					if( (float)trim($valorTotalEmpenhado) > (float)trim($valorObra) )
					{
						echo "ERRO| O valor do empenho ultrapassa o valor da obra: \n Valor da Obra: R$ ".number_format($valorObra, 2, ',', '.')." \n Valor do Empenhado: R$ ".number_format($valorEmpenhado, 2, ',', '.')." \n Valor Restante: R$ ".number_format($vrlDistribuir, 2, ',', '.');
						die();
					}
					else
					{
						// VAlida se já existe este empenho para esta obra
						$sqlExiste = "select eobid,eobvalorempenho from {$tabela} where empid = {$empIdDistrubir}  and preid = {$preid} and eobstatus = 'A'";
						$resultExiste = $db->pegaLinha($sqlExiste);
						$eobId = ($resultExiste['eobid']) ? $resultExiste['eobid'] : false;
						$valorEmpenhoBD = ($resultExiste['eobvalorempenho']) ? $resultExiste['eobvalorempenho'] : 0 ;
						// VAlor a inserir mais o valor que já existe (caso exista) seão soma com 0
						$valorInserir  = $valorInserir + $valorEmpenhoBD;
						// Caso já exista da update, senão insert
						if($eobId)
						{
							$sqlInserir = "UPDATE {$tabela} SET eobvalorempenho = {$valorInserir} WHERE eobid = {$eobId}";
						}
						else
						{
							$sqlInserir = "INSERT INTO {$tabela} 
												( preid, empid, eobvalorempenho, eobstatus)
											VALUES
												( {$preid}, {$empIdDistrubir}, {$valorInserir}, 'A')";
						}
						
						$sql = "SELECT obrstatus FROM obras2.obras WHERE obrid = (SELECT obrid FROM obras.preobra WHERE preid = $preid)";
						$obrstatus = $db->pegaUm($sql);
						if( $obrstatus == 'I' || $obrstatus == '' ){
							
							$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empIdDistrubir";
							$empnumero = $db->pegaUm($sql);
							
							insereHistoricoStatusObra( $empIdDistrubir, Array(), 'A', "Obra ativada pela redistribuição do empenho $empnumero" ); 
							
							$sql = "SELECT tooid FROM obras.preobra WHERE preid = $preid";
							$tooid = $db->pegaUm($sql);
							
							if( $tooid = '1' ){
								require_once '_funcoes_empenho.php';
								$obrid_1 = importarObrasPac( $preid );
// 								importarObras2Pac( $preid, $obrid_1 );
								$preObra = new PreObra( $preid );
								$preObra->importarPreobraParaObras2( $preid );
							}else{
								require_once '_funcoes_empenho_par_obras.php';
								$obrid_1 = importarObrasPar( $preid );
// 								importarObras2Par( $preid, $obrid_1 );
								$preObra = new PreObra( $preid );
								$preObra->importarPreobraParaObras2( $preid );
							}
							
						}
						//Executa a query
						$db->executar($sqlInserir);
					}
				}
			}
			// Busca o eobid do item que está sendo distribuido para o inativar
			$sqlInativar = "select eobid,eobvalorempenho from {$tabela} where empid = {$empIdDistrubir}  and preid = {$preidDistribuir} and eobstatus = 'A'";
			
			$arrComposicao 		= $db->pegaLinha($sqlInativar);
			$eobInativar 		= $arrComposicao['eobid']; 
			$eobvalorempenho 	= $arrComposicao['eobvalorempenho'];
			$totaldistribuido 	= $post['totaldistribuido'];
			
			if($eobInativar){
				// Caso a atransação ocorra normalmente segue para inativar
				if($db->commit()){
					if( (float)$totaldistribuido == (float)$eobvalorempenho){
// 						ver(12,d);
						// Inativa o empenho que estava sendo distribuido
						$sqlInativa = "UPDATE {$tabela} SET eobstatus = 'I' WHERE eobid = {$eobInativar}";
						$db->executar($sqlInativa);
						
						if($db->commit()){
							
							$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empIdDistrubir";
							$empnumero = $db->pegaUm($sql);
								
							insereHistoricoStatusObra( $empIdDistrubir, Array(), 'I', "Obra inativada pela redistribuição do empenho $empnumero" );
							
							inativaObras2SemSaldoEmpenho( $empIdDistrubir, Array() );
							//retorna sucesso
							die();
						}else{
							//retorna erro
							echo "ERRO| Erro ao executar a modificação na base de dados.";
							die();
						}
					} else {
						$eobvalorempenho = (float)$eobvalorempenho - (float)$post['totaldistribuido'];
						$sqlInativa = "UPDATE {$tabela} SET eobvalorempenho = '$eobvalorempenho' WHERE eobid = {$eobInativar}";
						
						$db->executar($sqlInativa);
						$db->commit();
						
						$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empIdDistrubir";
						$empnumero = $db->pegaUm($sql);
							
						insereHistoricoStatusObra( $empIdDistrubir, Array(), 'I', "Obra inativada pela redistribuição do empenho $empnumero" );
							
						inativaObras2SemSaldoEmpenho( $empIdDistrubir, Array() );
						
						return true;
						exit();
					}
				}
				else
				{
					//retorna erro
					echo "ERRO| Erro ao executar a modificação na base de dados.";
						die();
				}
			}
// 			ver(13,d);
			
		}
	//}
	die();
}

function CarregaModalredistribuirEmpenhoObra($post)
{
	global $db;
	$preid = $_POST['preid'];
	$tipo  =  $_POST['tipo'];
	
	$sqlObra = "SELECT predescricao, prevalorobra from obras.preobra WHERE preid = {$preid}";	
	$resultObra = $db->pegaLinha($sqlObra);
	
	$nomeObra	= $resultObra['predescricao'];
	$valorObra	= number_format($resultObra['prevalorobra'], 2, ',', '.');
	
	
	if($tipo == 'PAR')
	{
		$sqLEmpenho = "SELECT  
						   e.ne, 
						   e.empid,
						   e.saldo as valor_empenho,
						   count(p.pagid) as totpag,
						   count(e.valorcancelado) as totcancelado,
						   coalesce(sum(po.popvalorpagamento), 0) as vrlpagamento,
						   (e.saldo - coalesce(sum(po.popvalorpagamento), 0)) as vrldisponivel
						FROM 
						    par.v_saldo_obra_por_empenho e                            
						LEFT JOIN par.pagamento 		p  ON p.empid = e.empid AND pagstatus = 'A'
						LEFT JOIN par.pagamentoobrapar 	po ON po.preid = e.preid and po.pagid = p.pagid
						WHERE
						    e.preid = {$preid}
						group by e.ne, e.empid, e.saldo
						ORDER BY e.ne";
	}
	else if($tipo == 'PAC')
	{
		//@todo
		$sqLEmpenho = "SELECT  
						   e.ne, 
						   e.empid,
						   e.saldo as valor_empenho,
						   count(p.pagid) as totpag,
						   count(e.valorcancelado) as totcancelado,
						   coalesce(sum(po.pobvalorpagamento), 0) as vrlpagamento,
						   (e.saldo - coalesce(sum(po.pobvalorpagamento), 0)) as vrldisponivel
						FROM 
						    par.v_saldo_obra_por_empenho e
						LEFT JOIN par.pagamento 	p  ON p.empid = e.empid AND pagstatus = 'A'
						LEFT JOIN par.pagamentoobra po ON po.preid = e.preid AND po.pagid = p.pagid
						WHERE
						    e.preid = {$preid}
						group by e.ne, e.empid, e.saldo
						ORDER BY e.ne";
	}
	$resultEmpenho = $db->carregar($sqLEmpenho);
	$resultEmpenho = ($resultEmpenho) ? $resultEmpenho : Array();
	
	$html = '
		<form name="formdistribuir" id="formdistribuir" method="post">   
		<table align="center" border="0" width="100%"  cellpadding="3" cellspacing="2">
			<tr>
				<th>Dados de Empenho por Obra</th>
			</tr>
		</table>
		<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
		<thead>
			<tr>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Obra</td>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor da Obra</td>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ação</td>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Número empenho</td>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Empenho</td>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Pagamento</td>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Disponível</td>
				<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Observação</td>
			</tr>
		</thead>
		<tbody>
		<tr onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">
			<td valign="middle" rowspan="'.(sizeof($resultEmpenho)+1).'" align="center"><span style="color: rgb(0, 102, 204);">'.$preid.'</span> - '.$nomeObra.'</td>
			<td valign="middle" rowspan="'.(sizeof($resultEmpenho)+1).'" align="right" style="color: rgb(0, 102, 204);">'.$valorObra.'</td></tr>
		';
	$totalGeral = 0;
	$totalPag = 0;
	$totalDisp = 0;
	foreach ( $resultEmpenho as $key => $v ){
		$key % 2 ? $cor = "#dedfde" : $cor = "";
		
		$empid = $v['empid'];
		$totalGeral = (float)$totalGeral + (float)$v['valor_empenho'];
		$totalPag = (float)$totalPag + ( ((float)$v['vrlpagamento'] < 0) ? 0 : (float)$v['vrlpagamento']);
		$totalDisp = (float)$totalDisp + ( ((float)$v['vrldisponivel'] < 0) ? 0 : (float)$v['vrldisponivel']);
		
		$texto = '<span style="color: blue">Saldo Disponível</span>';
		
		if( (float)$v['totcancelado'] > (int)0 && (float)$v['vrldisponivel'] == 0){
			$radio = "<img align=\"absmiddle\" style=\"cursor:pointer;\" src=\"../imagens/excluir_01.gif\">";
			$texto = '<span style="color: red">Não existe saldo disponível para este empenho</span>';
		}else{
			if( (float)$v['valor_empenho'] > (float)$v['vrlpagamento'] ){
				$radio = "<input type=radio name='radioempenhoatual' id='radioempenhoatual' value='{$empid}' onclick=\"atualizaValoresTotais('');\">";
			} else {
				if( (float)$v['valor_empenho'] < (int)0 ){
					$radio = "-";
					$texto = '<span style="color: red">Não é possivel distribuir o empenho, pois existe Pagamento.</span>';
				} else {
					$radio = "<span title='Não é possivel distribuir o empenho, pois existe Pagamento.'><a style=\"color: blue\">PAG</a></span>";
					$texto = '<span style="color: red">Não é possivel distribuir o empenho, pois existe Pagamento.</span>';
				}
			}
		}
		
		if( (float)$v['vrldisponivel'] < 0 ){
			$color = 'red';
		}else{
			$color = 'rgb(0, 102, 204)';			
		}
		
		//if( $key > 0 ) 
			$html.='<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		$html.= '
					<td valign="middle" align="center">'.$radio.'</td>
					<td valign="middle">'.$v['ne'].'</td>
					<td valign="middle" align="right" title="Valor Empenho" style="color: rgb(0, 102, 204);">'.number_format($v['valor_empenho'], '2', ',', '.').'</td>
					<td valign="middle" align="right" title="Valor Pagamento" style="color: rgb(0, 102, 204);">'.number_format($v['vrlpagamento'], '2', ',', '.').'</td>
					<td valign="middle" align="right" title="Valor Disponível" style="color: '.$color.';">'.number_format(($v['vrldisponivel']<0 ? 0 : $v['vrldisponivel']), '2', ',', '.').'
						<input type="hidden" value="'.((float)$v['vrldisponivel'] < 0 ? '0' : $v['vrldisponivel']).'" id="vrlempenhoatual_'.$empid.'" name="vrlempenhoatual_'.$empid.'">	
						</td>
                    <td valign="middle" align="center">'.$texto.'</td>
				</tr>
			';
	}
	if( (float)$totalDisp < 0 ){
		$color = 'red';
	}else{
		$color = 'rgb(0, 102, 204)';
	}
	$html.= '</tbody>
				<tfoot>
					<tr>
						<td align="right" colspan="4">Totais: </td>
						<td align="right" title="Valor Empenho" style="color: rgb(0, 102, 204);"><b>'.number_format($totalGeral, '2', ',', '.').'</b></td>
						<td align="right" title="Valor Empenho" style="color: rgb(0, 102, 204);"><b>'.number_format($totalPag, '2', ',', '.').'</b></td>
						<td align="right" title="Valor Empenho" style="color: '.$color.';"><b>'.number_format($totalDisp, '2', ',', '.').'</b>
							<input type="hidden" value="'.$tipo.'" id="tipo_distribuir" name="tipo_distribuir">
							<input type="hidden" value="'.$preid.'" id="preid_distribuir" name="preid_distribuir">
							<input type="hidden" value="'.$totalDisp.'" id="total_distribuir" name="total_distribuir">
						</td>
					</tr>
				</tfoot>
			</table>
		</form>';
	echo $html;
	exit();
}

function CarregaModalredistribuirEmpenho($post)
{
	
	
	global $db;
	$preid = $_POST['preid'];
	$tipo  =  $_POST['tipo'];
	$proid  =  $_POST['proid'];

	if($tipo == 'PAR')
	{
		$sqLEmpenho = "SELECT DISTINCT
						case when v.saldo = cast(pre.prevalorobra as numeric(20,2)) then
							'<input type=\"checkbox\" id=\"check_'|| pre.preid ||'\"  value=\"'|| pre.preid ||'\" name=\"check_'|| pre.preid ||'\" disabled>'
						else
							'<input type=\"checkbox\" id=\"check_'|| pre.preid ||'\"  value=\"'|| pre.preid ||'\" name=\"check_'|| pre.preid ||'\" onclick=\"habilitaDistribuicao('|| pre.preid ||')\">'
						end as acao,
					    pre.preid as obra,
					    pre.predescricao as descricao_obra,
					    cast(pre.prevalorobra as numeric(20,2)) as valor_obra,
					    coalesce(v.saldo,0.00) as valor_empenho,
					    '	
					    	<input type=\"text\" disabled	id=\"input_'|| pre.preid ||'\" name=\"input_'|| pre.preid ||'\"  name=\"input_'|| pre.preid ||'\" class=\"clsMouseFocus campoSoma\" size=\"15\" maxlength=\"20\"  onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" 
					onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" style=\"text-align:left;\">  
					    	<input type=\"hidden\" 	id=\"indice_obra[]\" name=\"indice_'|| pre.preid ||'\" value=\"'|| pre.preid ||'\" >
					    	<input type=\"hidden\" 	id=\"valorobra[]\" name=\"valorobra_'|| pre.preid ||'\" value=\"'|| coalesce(cast(pre.prevalorobra as numeric(20,2)),0.00) ||'\" >
					    	<input type=\"hidden\" 	id=\"valorempenhoobra[]\" name=\"valorempenhoobra_'|| pre.preid ||'\" value=\"'|| coalesce(par.valorempenhadoobra(pre.preid),0.00) ||'\" >
					    ' as distribuir
					
					FROM 
						par.processoobraspar pro 
					INNER JOIN par.processoobrasparcomposicao 	c   ON c.proid 	  = pro.proid  
                    LEFT  JOIN par.v_saldo_empenho_por_obra 	v   ON v.processo = pro.pronumeroprocesso AND v.preid = c.preid and v.tipo = 'PAR'
                    INNER JOIN obras.preobra 					pre ON pre.preid  = c.preid
					WHERE
						pro.proid = {$proid}
						and pre.preid <> {$preid}
					order by pre.predescricao";		
	}
	else if($tipo == 'PAC')
	{		
		$sqLEmpenho = "SELECT DISTINCT
							case when vi.saldo = cast(pre.prevalorobra as numeric(20,2)) then
								'<input type=\"checkbox\" id=\"check_'|| pre.preid ||'\"  value=\"'|| pre.preid ||'\" name=\"check_'|| pre.preid ||'\" disabled>'
							else
								'<input type=\"checkbox\" id=\"check_'|| pre.preid ||'\"  value=\"'|| pre.preid ||'\" name=\"check_'|| pre.preid ||'\" onclick=\"habilitaDistribuicao('|| pre.preid ||')\">'
							end as acao,
						   	pre.preid || '<input type =\'hidden\'>  ' as obra,
						    pre.predescricao as descricao_obra,
						    cast(pre.prevalorobra as numeric(20,2)) as valor_obra,
						    coalesce(vi.saldo,0.00) as valor_empenho,
						    '	
						    	<input type=\"text\" disabled	id=\"input_'|| pre.preid ||'\" name=\"input_'|| pre.preid ||'\"  name=\"input_'|| pre.preid ||'\" class=\"clsMouseFocus campoSoma\" size=\"15\" maxlength=\"20\"  onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" 
						onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" style=\"text-align:left;\" >  
						    	<input type=\"hidden\" 	id=\"indice_obra[]\" name=\"indice_'|| pre.preid ||'\" value=\"'|| pre.preid ||'\" >
						    	<input type=\"hidden\" 	id=\"valorobra[]\" name=\"valorobra_'|| pre.preid ||'\" value=\"'|| coalesce(cast(pre.prevalorobra as numeric(20,2)),0.00) ||'\" >
						    	<input type=\"hidden\" 	id=\"valorempenhoobra[]\" name=\"valorempenhoobra_'|| pre.preid ||'\" value=\"'|| coalesce(par.valorempenhadoobra(pre.preid),0.00) ||'\" >
						    ' as distribuir
			  
					FROM 
						par.processoobra			pro
					    inner join par.processoobraspaccomposicao ppa on ppa.proid = pro.proid and ppa.pocstatus = 'A'
					    inner join obras.preobra 	pre on pre.preid = ppa.preid and pre.tooid = 1
					    /*LEFT JOIN  par.empenho 		emp	ON pro.pronumeroprocesso = emp.empnumeroprocesso 
					    LEFT JOIN  par.empenhoobra 	ems  ON emp.empid = ems.empid and pre.preid = ems.preid and ems.eobstatus = 'A'				
					    left join par.vm_saldo_empenho_por_obra vi on vi.preid = pre.preid*/
					    LEFT  JOIN par.v_saldo_empenho_por_obra 	vi   ON vi.processo = pro.pronumeroprocesso AND vi.preid = pre.preid and vi.tipo = 'PAC'
					WHERE
						pro.proid = {$proid}				
						and pre.preid <> {$preid}";	
	}	
 	//ver(simec_htmlentities($sqLEmpenho),d);
	$cabecalho = array('Ação', 'Obra', 'Nome da Obra', 'Valor da Obra', 'Valor Empenhado da Obra', 'Distribuir');
	
	//include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');
	
	/* $obMontaListaAjax = new MontaListaAjax($db, true);
	$registrosPorPagina = 1000000; */
	
	//$obMontaListaAjax->montaLista($sqLEmpenho, $cabecalho,$registrosPorPagina, 50, 'N', '', '', '', '', '', '', '' );
	$db->monta_lista_simples($sqLEmpenho,$cabecalho, 5000000, 5, 'N', '100%', 'S', true, false, false, true);
	
	echo '<table class="listagem" width="100%" align="center" cellspacing="0" cellpadding="2" border="0" style="color:333333;">
			<tr>
				<td style="width: 50%; text-align: right" class="subtitulodireita">Total Distribuido:</td>
				<td><input id="totaldistribuido" class="clsMouseFocus" type="text" style="text-align: left; font-size: 14px;" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'###.###.###,##\',this.value);" 
					maxlength="15" size="15" name="totaldistribuido" readonly></td>
			</tr>
			<tr>
				<td style="width: 50%; text-align: right" class="subtitulodireita">Total Restante:</td>
				<td><input id="totalrestante" class="clsMouseFocus" type="text" style="text-align: left; font-size: 14px;" onmouseout="MouseOut(this);" 
					onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'###.###.###,##\',this.value);" 
					maxlength="15" size="15" name="totalrestante" readonly></td>
			</tr>
		 </table>';
	exit();
}

function criaAnexosItemComposicao( $prpid, $arrSub, $dopid ){
	global $db;

	if( is_array($arrSub) && $arrSub[0] ){
		$whereSubacao = " sd.sbdid in (".implode(',', $arrSub).") and ";
	}

	$sql = "SELECT
				foo.codigo,
                foo.picdescricao,
                foo.ptsdescricao,
                foo.picid,
                foo.icoid,
                foo.sbdano,
                foo.sbaid,
                foo.quantidade,
                foo.valor,
                ( foo.quantidade * foo.valor ) as total
			FROM (
				SELECT 
					pic.picdescricao,
					pts.ptsdescricao,
					pic.picid,
					sic.icoid,
					sd.sbdano as sbdano,
					s.sbaid,
					par.recuperaquantidadeitemvalidado( sic.icoid )	AS quantidade,
					sic.icovalor as valor,
					d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sbaordem as codigo	
				FROM par.processopar pp
					INNER JOIN par.processoparcomposicao 	ppc ON ppc.prpid = pp.prpid and ppc.ppcstatus = 'A'
					INNER JOIN par.subacaodetalhe 		sd  ON sd.sbdid = ppc.sbdid
					INNER JOIN par.subacao       		s   ON sd.sbaid = s.sbaid AND s.sbastatus = 'A'
					INNER  JOIN par.subacaoitenscomposicao   sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND icostatus = 'A'
					INNER JOIN par.propostaitemcomposicao   pic ON pic.picid = sic.picid
					LEFT JOIN par.propostasubacao 		pps ON pps.ppsid = s.ppsid		
					LEFT JOIN par.propostatiposubacao	pts ON pts.ptsid = pps.ptsid
					INNER JOIN par.acao 					a   ON a.aciid = s.aciid
					INNER JOIN par.pontuacao 				pon ON pon.ptoid = a.ptoid
					INNER JOIN par.criterio 				c   ON c.crtid = pon.crtid
					INNER JOIN par.indicador 				i   ON i.indid = c.indid
					INNER JOIN par.area 					are ON are.areid = i.areid
					INNER JOIN par.dimensao 				d   ON d.dimid = are.dimid
				WHERE 	
					{$whereSubacao}
					pp.prpid = $prpid 
					AND s.sbastatus = 'A' 
					AND pp.prpstatus = 'A'
			) as foo ORDER BY foo.codigo";
                                        
	$arDadosItem = $db->carregar($sql);
	$arDadosItem = $arDadosItem ? $arDadosItem : array();
	
	$html = '
		<table align="center" class="listagem" border="1" width="100%" cellSpacing="1" cellPadding=3 >
			<tr>
				<th colspan="8" style="text-align: center;">ANEXO</th>
			</tr>
			<tr>
				<th width="10%"><b>ID</b></th>
				<th width="60%" style="text-align: center;"><b>Descrição</b></th>
				<th width="10%" style="text-align: center;"><b>Qtd</b></th>
				<th width="10%" style="text-align: center;"><b>Valor</b></th>
				<th width="10%" style="text-align: center;"><b>Valor Total</b></th>
			</tr>';
	
	$totalQtd 		= 0;
	$totalValor 	= 0;
	$totalVrlTotal 	= 0;	
	foreach ($arDadosItem as $v) {
		
		$valorTotal = ($v['total'] ? number_format($v['total'],2,",",".") : '0,00');
		$valor = ($v['valor'] ? number_format($v['valor'],2,",",".") : '0,00');
		
		$totalQtd 		+= $v['quantidade'];
		$totalValor 	+= $v['valor'];
		$totalVrlTotal 	+= $v['total'];	
				
		$html.='
		<tr>
			<td>'.$v['icoid'].'</td>
			<td style="text-align: left;">'.$v['picdescricao'].'</td>
			<td style="text-align: center;">'.$v['quantidade'].'</td>
			<td style="text-align: center;">'.$valor.'</td>
			<td style="text-align: center;">'.$valorTotal.'</td>
		</tr>';
		
		$sql = "INSERT INTO par.itens_documentopar(dopid, icoid, prpid, idppregao) 
				VALUES ($dopid, {$v['icoid']}, $prpid, idppregao)";
		//$db->executar($sql);
	}		
	$html.= '<tr>
				<td colspan="2" style="text-align: right;">Total:</td>
				<td style="text-align: center;">'.number_format($totalQtd,2,",",".").'</td>
				<td style="text-align: center;">'.number_format($totalValor,2,",",".").'</td>
				<td style="text-align: center;">'.number_format($totalVrlTotal,2,",",".").'</td>
			</tr>
		</table>';
	ver($html,d);
	include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
	ob_clean();
		
	$nomeArquivo 		= 'minuta_repasse_'.date('Y-m-d').'_lote_'.$lote;
	$diretorio		 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil';
	$diretorioArquivo 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil/'.$nomeArquivo.'.pdf';
	
	if( !is_dir($diretorio) ){
		mkdir($diretorio, 0777);
	}
	
	$http = new RequestHttp();
	$html = utf8_encode($html);
	$response = $http->toPdf( $html );

	$fp = fopen($diretorioArquivo, "w");
	if ($fp) {
	  stream_set_write_buffer($fp, 0);
	  fwrite($fp, $response);
	  fclose($fp);
	}
		
		$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
				VALUES( '".$nomeArquivo."',
						'pdf',
						'".$nomeArquivo."',
						'application/pdf',
						'".filesize($diretorioArquivo)."',
						'".date('Y-m-d')."',
						'".date('H:i:s')."',
						'".$_SESSION["usucpf"]."',
						{$_SESSION['sisid']},
						'A') RETURNING arqid";
		
		$arqid = $db->pegaUm($sql);
}

function salvarDetalharRecursoObra( $post ){
	global $db;
	
	$emdid 				= $post['emdid'];
	$emeid 				= $post['emeid'];
	$preid 				= $post['preid'];
	$ano 				= $post['ano'];
	$obpid 				= $post['obpid'];
	$obpvaloremenda 	= ($post['obpvaloremenda'] ? retiraPontosBD($post['obpvaloremenda']) : 'null');
	/* $obpvalorentidade 	= ($post['obpvalorentidade'] ? retiraPontosBD($post['obpvalorentidade']) : 'null');
	$obpvalorfnde 		= ($post['obpvalorfnde'] ? retiraPontosBD($post['obpvalorfnde']) : 'null'); */
	
	if( $obpid ){
		$sql = "UPDATE par.obraemenda SET obpvaloremenda = $obpvaloremenda WHERE obpid = $obpid";
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO par.obraemenda(preid, emeid, emdid, obpvaloremenda, obpstatus, obpcpfinclusao) 
				VALUES ($preid, $emeid, $emdid, $obpvaloremenda, 'A', '".$_SESSION['usucpf']."')";
		$db->executar($sql);
	}
	return $db->commit();
}
function detalharDistribuicaoRecurso( $post ){
	global $db;
	
	$emdid 	= $post['emdid'];
	$emeid 	= $post['emeid'];
	$preid 	= $post['preid'];
	$sbaid 	= $post['sbaid'];
	$obpid 	= ($post['obpid'] > 0 ? $post['obpid'] : '' );
	$ano 	= $post['ano'];
	$cnpjEntidade = $post['cnpjEntidade'];
	
	$filtro = '';
	if( $obpid ) $filtro = " and obpid = $obpid ";
	
	$sql = "SELECT distinct
			    ede.emecod,
			    sum(ede.emdvalor) as emdvalor,
    			sum(ede.edevalor) as edevalor,
    			sum(sep.sepvalor) as sepvalor,
			    ede.entid,
			    au.autnome,
			    sum(ede.edevalor) as vrldisponivel,
			    (sum(ede.edevalor) - coalesce(sum(oe.obpvaloremenda),0) ) as obpvaloremenda
			FROM emenda.v_emendadetalheentidade ede
			    inner join emenda.entidadebeneficiada enb on enb.enbid = ede.entid
			    inner join par.subacaoemendapta sep on sep.emdid = ede.emdid and sep.sepstatus = 'A'
			    inner join emenda.autor au on au.autid = ede.autid
			    inner join par.subacaodetalhe sd on sd.sbdid = sep.sbdid
			    left join par.obraemenda oe on oe.emdid = ede.emdid and oe.obpstatus = 'A'
			WHERE 
			    ede.emdid = $emdid
			    and enb.enbcnpj = '".$cnpjEntidade."'
			    and ede.edestatus = 'A'
				and sd.sbaid = $sbaid
				and sd.sbdano = $ano
			group by 
			    ede.emecod,
			    ede.entid,
				au.autnome";
	//ver($sql,d);
	$arEmenda = $db->pegaLinha($sql);
	
	$sqlSub = "SELECT distinct
				case when (select count(sbaid) from par.subacaoobra where sbaid = s.sbaid) > 0 then
					'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaObras(\''||s.sbaid||'\', this);\"></center>' 
				else '' end as acoes,				
			    d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || sbaordem||'&nbsp;' as codigo,
			    s.sbadsc,
			    sp.sepvalor
			FROM
			    par.subacao s
			    inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid
			    inner join par.subacaoemendapta sp on sp.sbdid = sd.sbdid and sp.sepstatus = 'A'
			    inner join par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
			    inner join par.pontuacao pont  ON pont.ptoid  = a.ptoid AND pont.ptostatus = 'A'
			    inner join par.criterio  c  ON c.crtid  = pont.crtid AND c.crtstatus = 'A'
			    inner join par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
			    inner join par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
			    inner join par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
			    inner join emenda.v_emendadetalheentidade vede on vede.emdid = sp.emdid
			    inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
			    inner join emenda.ptemendadetalheentidade pte on vede.edeid = pte.edeid
			    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid
			    inner join par.subacaoemendapta sep on sep.ptrid = ptr.ptrid and vede.emdid = sep.emdid and sep.sepstatus = 'A'
			WHERE
			    vede.edestatus = 'A'
			    and s.sbastatus = 'A'
			    and vede.ededisponivelpta = 'S'
			    and vede.emeano = '$ano'
			    and enb.enbcnpj = '".$cnpjEntidade."'
			    and pont.inuid = {$_SESSION['par']['inuid']}
			    and ptr.ptrstatus = 'A'
			    and ptr.ptrexercicio = '$ano'
			    and ptr.sisid = 23
			    and vede.emetipo = 'E'
			order by 1";
	
	$vrlobra = $db->pegaUm("select cast(prevalorobra as numeric(20,2)) from obras.preobra where preid = $preid");
	$vrEmpenho = $db->pegaUm("select saldo from par.vm_saldo_empenho_por_obra where preid = $preid");
	
	if( $obpid ){
		$sql = "SELECT preid, emeid, emdid, obpvaloremenda, obpvalorfnde, obpvalorentidade
				FROM par.obraemenda
				WHERE obpid = $obpid";
		$arrObraEmenda = $db->pegaLinha($sql);
		$obpvaloremenda 	= simec_number_format($arrObraEmenda['obpvaloremenda'], 2, ',', '.');
		/* $obpvalorfnde 		= simec_number_format($arrObraEmenda['obpvalorfnde'], 2, ',', '.');
		$obpvalorentidade	= simec_number_format($arrObraEmenda['obpvalorentidade'], 2, ',', '.'); */
	}
	
	?>
	<table class="tabela" style="width: 100%" align="center" border="0" cellspacing="1" cellpadding="2">
		<tr>
			<th class="subtitulodireita" colspan="4" style="text-align: center;">Dados da Emenda</th>
		</tr>
		<tr>
			<td class="subtitulodireita" width="25%">Numero da Emenda:</td>
			<td style="color:#0066cc;"><?php echo $arEmenda['emecod'];?></td>
			<td class="subtitulodireita" width="25%">Valor Distribuido para Subação:</td>
			<td style="color:#0066cc;"><?php echo number_format($arEmenda['sepvalor'], 2, ',', '.');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="25%">Autor da Emenda:</td>
			<td><?php echo $arEmenda['autnome']; ?></td>
			<td class="subtitulodireita" width="25%">Valor Indicado para Entidade:</td>
			<td style="color:#0066cc;"><?php echo number_format($arEmenda['edevalor'], 2, ',', '.');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="25%">Valor da Emenda:</td>
			<td style="color:#0066cc;"><?php echo number_format($arEmenda['emdvalor'], 2, ',', '.');?></td>
			<td class="subtitulodireita" width="25%">Valor Disponível:</td>
			<td style="color:#0066cc;"><?php echo number_format($arEmenda['obpvaloremenda'], 2, ',', '.');?>
				<input type="hidden" name="vrldisponivelemenda" id="vrldisponivelemenda" value="<?php echo $arEmenda['vrldisponivel'] ?>" />
			</td>
		</tr>
	</table>
	<table class="tabela" style="width: 100%" align="center" border="0" cellspacing="1" cellpadding="2">
		<tr>
			<th class="subtitulodireita" colspan="2" style="text-align: center;">Dados de Distribuição do Recurso feita pelo Municípios</th>
		</tr>
		<tr>
			<td colspan="2">
			<input type="hidden" name="obpid" value="<?php echo $obpid; ?>">
			<?php 
			$cabecalho = array("Ações", "Codigo","Subação","Valor");
			$db->monta_lista_simples($sqlSub,$cabecalho,500,5,'N','100%','S');
			?></td>
		</tr>
	</table>
	<table class="tabela" style="width: 100%" align="center" border="0" cellspacing="1" cellpadding="2">
		<tr>
			<th class="subtitulodireita" colspan="2" style="text-align: center;">Detalhar Distribuição do Recurso</th>
		</tr>
		<tr>
			<td class="subtitulodireita" width="45%">Valor da Obra (Planilha Orçamentária):</td>
			<td><input type="text" id="vrlobra" name="vrlobra" class="clsMouseFocus" size="15" maxlength="20" 
				onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" disabled="disabled" onmouseover="MouseOver(this);" value="<?php echo number_format($vrlobra, 2, ',', '.');?>"></td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="45%">Valor Empenhado da Obra:</td>
			<td><input type="text" id="vrlempenhoobra" name="vrlempenhoobra" class="clsMouseFocus" size="15" maxlength="20" 
				onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" disabled="disabled" onmouseover="MouseOver(this);" value="<?php echo number_format($vrEmpenho, 2, ',', '.');?>">
				</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Valor da Emenda para a Obra:</td>
			<td><input type="text" id="obpvaloremenda" name="obpvaloremenda" class="clsMouseFocus" size="15" maxlength="20"  value="<?php echo $obpvaloremenda; ?>"
				onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onmouseover="MouseOver(this);" onblur="calculaTotalRestanteEmenda();"></td>
		</tr>
		<!--  <tr>
			<td class="subtitulodireita">Valor Suplementar de <?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
																echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']);?>:</td>
			<td><input type="text" id="obpvalorentidade" name="obpvalorentidade" class="clsMouseFocus" size="15" maxlength="20" value="<?php echo $obpvalorentidade; ?>"  
				onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onmouseover="MouseOver(this);" onblur="calculaTotalRestante(this);"></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Valor Complementar do FNDE:</td>
			<td><input type="text" id="obpvalorfnde" name="obpvalorfnde" class="clsMouseFocus" size="15" maxlength="20"  value="<?php echo $obpvalorfnde; ?>"
				onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onmouseover="MouseOver(this);" onblur="calculaTotalRestante(this);"></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Valor Restante a Distribuir:</td>
			<td><input type="text" id="vrlrestante" name="vrlrestante" class="clsMouseFocus" disabled="disabled" size="15" maxlength="20"  
				onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onmouseover="MouseOver(this);"></td>
		</tr>-->
	</table>
	<?php 
}
/* Esta função irá atualizar os campos sbdvalorplanejado e sbdvaloraprovado
 * Parametros: Sbaid e ano da subação
 * 
 */
function atualizaValorSubacaoDetalhe($sbaid, $sbdano)
{
	
	global $db;
	// Busca valores
	$sqlValores = " 
		SELECT 
			par.recuperavalorplanejadossubacaoporano({$sbaid}, {$sbdano}) as valor_planejado,
			par.recuperavalorvalidadossubacaoporano({$sbaid}, {$sbdano}) as valor_validado 
	";
	// Carrega valores
	$valores = $db->pegaLinha($sqlValores);
	$valorPlanejado = ($valores['valor_planejado']) ? $valores['valor_planejado'] : 0;
	$valorValidado 	= ($valores['valor_validado'])  ? $valores['valor_validado']  : 0;
	// Atualiza na subacaodetalhe
	$sqlUpdateValor = "
		UPDATE 
			par.subacaodetalhe SET sbdvalorplanejado = $valorPlanejado , sbdvaloraprovado = $valorValidado
		WHERE
			sbaid = {$sbaid} AND sbdano = {$sbdano} ";
	
	$db->executar($sqlUpdateValor);
	
	if($db->commit()){
		return true;
	}
}

function verificaPendenciaSubacaoEscola($dopid) {
        global $db;
        $sqlProcesso = "SELECT 
                            prp.prpid,
                            prp.inuid
                        FROM par.documentopar dp
                        INNER JOIN par.processopar prp ON dp.prpid = prp.prpid
                        WHERE dp.dopid={$dopid}";
        $arrProcesso = $db->pegaLinha($sqlProcesso);
        $inuid = $arrProcesso['inuid'];
        $prpid = $arrProcesso['prpid'];
        
        $buscaDf = false;
                                                	
        $infoLocalidade = $db->pegaLinha("select * from par.instrumentounidade where inuid = {$inuid}");
        $itrid =  $infoLocalidade['itrid'];
        
        $sqlSubacao = "SELECT
                            prp.prpid,
                            sd.sbaid,	
                            sd.sbdano as ano
                        FROM
                            par.processopar prp
                            inner join par.processoparcomposicao ppc on ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                            inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid 
                        WHERE prp.prpid ={$prpid}";
        $arrSubacao = $db->pegaLinha($sqlSubacao);
        $sbaid = $arrSubacao['sbaid'];
        $ano = $arrSubacao['ano'];

        //Estadual
        if( $itrid == 1) {
            
            $esferaCerta = 'Estadual';
            $esferaErrada = 'Municipal';
            $uf = $infoLocalidade['estuf'];
            $muncod = $infoLocalidade['muncod'];
            $localidade = "m.estuf = '{$uf}'";

            // tcpid Municipal, já que é estadual vou buscar por municipal
            $tcpid = '(3 , 2)';
            if ($muncod == '5300108' || $uf =='DF') {
                $buscaDf = true;
            }
        } else {
            $esferaCerta = 'Municipal';
            $esferaErrada = 'Estadual';
            $uf = $infoLocalidade['mun_estuf'];
            $muncod = $infoLocalidade['muncod'];

            $localidade = "d.muncod = '{$muncod}'";
            // tcpid estadual, já que é municipal vou buscar por estadual

             $tcpid = '(1 , 2)';
            if ($muncod == '5300108' || $uf=='DF') {
                $buscaDf = true;
            }
        }

        if(! $buscaDf) {
            $sql = " SELECT DISTINCT
                            m.mundescricao,
                            t.entnome,
                            t.entcodent,
                            e.esctipolocalizacao as escola_localizacao                                                                                      
                    FROM
                        entidade.entidade t
                    INNER JOIN entidade.funcaoentidade f ON f.entid = t.entid
                    LEFT JOIN entidade.entidadedetalhe ed ON t.entid = ed.entid
                    INNER JOIN entidade.endereco d ON t.entid = d.entid
                    LEFT JOIN territorios.municipio m ON m.muncod = d.muncod
                    LEFT JOIN par.escolas e ON e.entid = t.entid
                    INNER JOIN par.subacaoescolas ses ON ses.escid = e.escid 
                        AND ses.sbaid = '{$sbaid}' 
                        AND ses.sesstatus = 'A' 
                        AND ses.sesano = '{$ano}'
                    LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = ses.sesid 

                        AND ses.sesano = '{$ano}'
                        AND ses.sbaid = '{$sbaid}'
                    WHERE (t.entescolanova = false or t.entescolanova is null)
                    AND t.entstatus = 'A'
                    AND f.funid = 3
                    AND t.tpcid in {$tcpid}
                    AND {$localidade}
                    AND  ssi.seiqtdtecnico > 0
                    ORDER BY
                        m.mundescricao,
                        t.entnome"; 
                  $result = $db->carregar($sql);
                  $result = (is_array($result)) ? $result : Array();

                  $totalErradas = count($result);
                if(count($result) > 0){
                    $strHtml = "<br>
                    <table cellspacing=\"1\" cellpadding=\"3\" border=\"0\" align=\"center\" style=\"border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 98%;\" id=\"listaRestricoes\">
                        <thead>
                            <tr >
                                <td colspan=\"4\"> Pendências de composição </td>
                            </tr>
                        </thead>
                        <tbody>
                            <tr style=\"color:red;\">
                                <td style=\"width:40%;\">
                                    Existe(m) subação(ões) com erro na composição de escolas. 
                                    A(s) escola(s) deve(m) ser removida(s) na tela de Subação.
                                </td>
                            </tr>
                        </tbody>
                    </table>
                    <br>";
                return $strHtml;
            }
        }
    }
function recusarProrrogacaoObrasPAC( $dados ){
	global $db;
	
	# Recusar prorrogacao
	$sql = "UPDATE obras.preobraprorrogacao SET popstatus = 'F', popparecer = '{$dados['popparecer']}' WHERE popstatus = 'P' AND preid = " . $dados['preid'];
	$db->executar($sql);
	
	# Verifica se PAR ou PAC
	$sql = "SELECT tooid FROM obras.preobra WHERE preid = {$dados['preid']}";
	$tooid = $db->pegaUm($sql);
	
	# Se a obra for PAC
	$docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='" . $dados['preid'] . "'");
	include_once APPRAIZ . 'includes/workflow.php';
	if ($tooid == 1) {
		$esdidorigem = WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO;
	
		$esdiddestino = $db->pegaUm("SELECT esdidorigem FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid=" . $dados['preid']);
		$esdiddestino = $esdiddestino != '' ? $esdiddestino : WF_TIPO_OBRA_APROVADA;
	
		$aedid = $db->pegaUm("SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = " . $esdidorigem . " AND esdiddestino = " . $esdiddestino);
		wf_alterarEstado($docid, $aedid, 'Prorrogação de prazo cancelada.', array('preid' => $dados['preid']));
	
	} else {
		wf_alterarEstado($docid, WF_AEDID_PAR_AGUARDANDO_PRORROGACAO_ENVIAR_PARA_OBRA_RECUSADA, 'Prorrogação de prazo cancelada.', array('preid' => $dados['preid']));
	}
	
	# Gerar documento para recusar prorrogacao
	$retorno = gerarDocumentoRecusarProrrogacao($dados['preid']);
	
	if ($retorno) {

		if($dados['envia_email'])
		{
			$preid =  $dados['preid'];
			$parecer = $dados['popparecer'];
			$sql = "SELECT
						pre.predescricao,
					CASE WHEN muncod IS NULL THEN
						( select inuid from  par.instrumentounidade iu where iu.estuf = pre.estuf )
					ELSE
						( select inuid from  par.instrumentounidade iu where iu.muncod = pre.muncod )
					END AS inuid
					 
					FROM
						obras.preobra pre
					WHERE
						pre.preid = {$preid}";
				
			$dadosObra = $db->pegaLinha($sql);
			$inuid 		= $dadosObra['inuid'];
			$nomeObra	= $dadosObra['predescricao'];
					 
			if( !empty($inuid) )
			{
				$db = new cls_banco ();
				$sql = "SELECT
						iu.itrid,
						CASE WHEN iu.itrid = 2 THEN
							iu.muncod
						WHEN iu.itrid = 1 THEN
							iu.estuf
						END as filtro
						FROM
							par.instrumentounidade iu
						WHERE
							inuid = {$inuid}
				";
				$result = $db->pegaLinha($sql);
				$itrid = $result['itrid'];
				$filtro = $result['filtro'];
					    		 
					
				if( ($itrid == 2) && ($filtro)  )
				{
					$sqlEmail = "
						SELECT
							ent.entemail as email
						FROM
							par.entidade ent
						INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
						INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
						WHERE
							ent.dutid =  7
						AND
							ent.entstatus = 'A'
						AND
							mun.muncod in ( '{$filtro}' )
					";
				}
				else if( ($itrid == 1) && ($filtro))
				{
					$sqlEmail = "
						SELECT
							ent.entemail as email
						FROM
							par.entidade ent
						INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
						INNER JOIN territorios.estado est on est.estuf = ent2.estuf
						
						WHERE
							ent.entstatus='A'
						AND
							ent.dutid =  10
						AND
							ent2.estuf in ( '{$filtro}' )";
				}
					
				$resultEmail = $db->pegalinha($sqlEmail);
					
				$emailTo =  $resultEmail['email'];
				if($emailTo )
				{
					
					$texto = "
<pre><p style=\"text-align: justify;\">
Prezados Senhores,
Informamos que a obra {$nomeObra} teve sua prorrogação recusada. Justificativa: {$parecer}
	    								
Atenciosamente,
Equipe do PAR
</p></pre>
					";
					
					$strMensagem = $texto;
					// . $dopTexto
					$strAssunto = "Prorrogação da obra '{$nomeObra}' recusada ";
					$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
					$strMensagem = html_entity_decode($strMensagem);
						
					if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" )
					{
						
						print_r(
							'Assunto:' . $strAssunto
							."texto" . $texto
						);
						die();
					}
					elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
					{
						$strEmailTo = array($_SESSION['email_sistema']);
						$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
						
					}
					else
					{
						$strEmailTo = $emailTo;
						$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
						
					}
				}
			}
		}
		if( $_REQUEST['acompanhamento'] == 'S' ){
        	echo "<script>alert('O documento foi anexado a lista de obras.');
	        		window.opener.carregarListaDivObras('".$_REQUEST['processo']."', '');
	        		window.close();</script>";
        } else {
			echo '<script>alert("O documento foi anexado a lista de obras.");
			        		window.opener.location.reload();
			        		window.close();</script>';
		}
	} else {
		$db->rollback();
		ob_clean();
		if( $_REQUEST['acompanhamento'] == 'S' ){
			echo "<script>alert('Erro.');
	        		window.opener.carregarListaDivObras('".$_REQUEST['processo']."', '');
	        		window.close();</script>";
		} else {
			echo '<script>alert("Erro.");
						window.opener.location.reload();
					window.close();</script>';
		}
	}
}

function hasPlanoEducacao($arrData) {
    global $db;

    $strWhere = "";
    if (array_key_exists('muncod', $arrData)) {
        $strWhere .="AND a.muncod = '{$arrData['muncod']}'";
        $sql = "SELECT 
                    a.assid
                FROM sase.assessoramento a 
                JOIN territorios.estado e ON e.estuf = a.estuf
                JOIN territorios.municipio m ON m.muncod = a.muncod 
                JOIN workflow.documento d ON d.docid = a.docid
                JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                JOIN sase.situacaoassessoramento s ON s.esdid = ed.esdid  
                WHERE s.stacod = '".SASE_COM_LEI_SANCIONADA."' $strWhere";
    }
    if (array_key_exists('estuf', $arrData)) {
        $strWhere .="AND a.estuf = '{$arrData['estuf']}'";
        $sql = "SELECT 
                    a.aseid
                FROM sase.assessoramentoestado a  
                INNER JOIN sase.situacaoassessoramento sa on(sa.stacod = a.stacod)  
                WHERE a.stacod = '".SASE_COM_LEI_SANCIONADA."' $strWhere";
    }
    $arrResultado = $db->carregar($sql);
    $booResultado = ($arrResultado) ? true : false;
    return (bool)$booResultado;
}

function montaListaEscolaSubacao($arrParam = array(), $booErro = true, $boolReprogramacao = true ) {
    global $db;

    $arrObrigatorio = array('inuid', 'sbaid', 'ano');
    foreach ($arrObrigatorio as $value) {
        if (!key_exists($value, $arrParam)){
            return false;
        }
    }
    
    $strSbaid = '';
    if (is_array($arrParam['sbaid'])) {
        if (key_exists($arrParam['ano'], $arrParam['sbaid'])) {
            $strSbaid = $arrParam['sbaid'][$arrParam['ano']];
        }else {
            return false;
        }
    } else {
        $strSbaid = $arrParam['sbaid'];
    }
    
    $infoLocalidade = $db->pegaLinha("select * from par.instrumentounidade  where inuid = {$arrParam['inuid']}");
    $itrid =  $infoLocalidade['itrid'];
    //Estadual
    if( $itrid == 1) {
        $esferaCerta = 'Estadual';
        $esferaErrada = 'Municipal';
        $uf = $infoLocalidade['estuf'];
        $muncod = $infoLocalidade['muncod'];
        $localidade = "m.estuf = '{$uf}'";

        $tcpid = ($booErro == true) ? '1' : '2,3,4,5'; //2;"Federal"3;"Municipal"4;"Privada"5;"Conveniada"
    } else {
        $esferaCerta = 'Municipal';
        $esferaErrada = 'Estadual';
        $uf = $infoLocalidade['mun_estuf'];
        $muncod = $infoLocalidade['muncod'];

        $localidade = "d.muncod = '{$muncod}'";
        $tcpid = ($booErro == true) ? '3' : '1,2,4,5'; 
        if ($muncod == '5300108' ||$uf=='DF'){
            $tcpid = ($booErro == true) ? '1' : '2,3,4,5'; 
        }
    }

        $montarTR = " || '</td></tr>
    <tr style=\"display:none\" class=\"listaItem_' || ses.sesid || '\" >
    <td class=\"listaItemTd_' || ses.sesid || '\" colspan=7 align=center ></td>
    </tr>' ";
        $strBotaoExcluir = ($booErro == false) ? "CASE WHEN (t.tpcid in ($tcpid)) THEN '<img src=\"../imagens/excluir.gif\" onclick=\"retiraQuantidadeEscola(' || ses.sesid || ',' ||ses.sbaid || ',' ||ses.sesano || ' )\" title=\"Excluir Item\" class=\"middle link\">' ELSE '' END ||" : "";
        $strBotaoExcluir = ( ! $boolReprogramacao ) ? "" : $strBotaoExcluir;
              $sql = " 
                    SELECT 
                        foo.acao,
                        foo.mundescricao,
                        foo.entnome,
                        foo.entcodent,
                        foo.escola_localizacao
                    FROM (
                        SELECT DISTINCT
                            '<center>' || $strBotaoExcluir
                                ' <img src=\"../imagens/mais.gif\" id=\"img_item_' || ses.sesid ||'\" onclick=\"carregarItem($(this), ' || ses.sesid || ')\" title=\"Carregar Item\" class=\"middle link\">
                            </center>' as acao,
                                    m.mundescricao,
                                    t.entnome,
                                    t.entcodent,
                                    e.esctipolocalizacao $montarTR as escola_localizacao                                                                                      
                                FROM
                                    entidade.entidade t
                                INNER JOIN entidade.funcaoentidade f ON f.entid = t.entid
                                LEFT JOIN entidade.entidadedetalhe ed ON t.entid = ed.entid
                                INNER JOIN entidade.endereco d ON t.entid = d.entid
                                LEFT JOIN territorios.municipio m ON m.muncod = d.muncod
                                LEFT JOIN par.escolas e ON e.entid = t.entid
                                INNER JOIN par.subacaoescolas ses ON ses.escid = e.escid 
                                    AND ses.sbaid = '{$strSbaid}' 
                                    AND ses.sesstatus = 'A' 
                                    AND ses.sesano = '{$arrParam['ano']}'
                                LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = ses.sesid 
                                    AND ses.sesano = '{$arrParam['ano']}'
                                    AND ses.sbaid = '{$strSbaid}'
                                WHERE (t.entescolanova = false or t.entescolanova is null)
                                AND t.entstatus = 'A'
                                AND f.funid = 3
                                AND t.tpcid in ($tcpid)
                                AND {$localidade}
                                ORDER BY 
                                    m.mundescricao,
                                    t.entnome
                            ) foo"; 
                                
            $result = $db->carregar($sql);
            return $result;
}

function validaEnvioObraArquivada( $preid ){
	global $db;
	
	$total = $db->pegaUm("select count(eobid) from par.empenhoobrapar where preid = $preid") ;
	
	if( (int)$total > 0 ){
		return false;
	} else {
		return true;
	}
}

function verificaExecucaoFisicaObras( $dados ){
	global $db;

	if($dados['preid']) {
		foreach($dados['preid'] as $preid) {
			$sql = "SELECT DISTINCT
					    oi.obrid as id,
					    oi.preid as idpreobra,
					    oi.preid||' - '||oi.obrnome as descricao,
					    vse.saldo as vrlempenho,
						cast(pre.prevalorobra as numeric(20,2)) as prevalorobra,
					    vlr.vrlpagamento,
					    (coalesce(vse.saldo, 0) - coalesce(vlr.vrlpagamento, 0)) as vrlrestante,
					    CASE WHEN (va.vldstatushomologacao = 'N' OR va.vldstatushomologacao IS NULL) THEN 'nao' ELSE 'sim' END as homologacao,
					    CASE WHEN (va.vldstatus25exec = 'N' OR va.vldstatus25exec IS NULL OR va.vldstatus25exec = '') THEN 'nao' ELSE 'sim' END as execucao25,
					    CASE WHEN (va.vldstatus50exec = 'N' OR va.vldstatus50exec IS NULL OR va.vldstatus50exec = '') THEN 'nao' ELSE 'sim' END as execucao50,
					    CASE WHEN oi.obrpercentultvistoria IS NULL THEN '0.00 %' ELSE oi.obrpercentultvistoria||' %' END as percexec,
					    ptoid
					FROM
					    obras2.obras oi
					    INNER JOIN obras.preobra				pre ON pre.preid = oi.preid
					    INNER JOIN obras2.empreendimento 		emp ON emp.empid = oi.empid
					    inner join par.v_saldo_empenho_por_obra vse on vse.preid = pre.preid
					    LEFT  JOIN obras2.arquivosobra 			ao  ON ao.obrid = oi.obrid AND ao.tpaid = 24 AND ao.aqostatus = 'A'
					    LEFT  JOIN obras2.validacao 			va  on va.obrid = oi.obrid
					    left join(
					    	select sum(po.pobvalorpagamento) as vrlpagamento, po.preid from
					            par.pagamento p
					            inner join par.pagamentoobra po on po.pagid = p.pagid
					        where p.pagstatus = 'A'
					            and p.pagsituacaopagamento not ilike '%cancelado%'
					        group by po.preid
					        union all
					        select sum(po.popvalorpagamento) as vrlpagamento, po.preid from
					            par.pagamento p
					            inner join par.pagamentoobrapar po on po.pagid = p.pagid
					        where p.pagstatus = 'A'
					            and p.pagsituacaopagamento not ilike '%cancelado%'
					        group by po.preid
					    ) vlr on vlr.preid = pre.preid
					WHERE
					    emp.orgid = 3
					    AND oi.obrstatus = 'A'
					    AND obridpai IS NULL
						AND oi.preid =".$preid;
			$dadospre = $db->pegaLinha($sql);
			/*
			 * Alterar a regra para verificar o valor pago e não os números de parcelas.
			 1 = 20%
			 2 = 50%
			 3 = 75%   MI = 90 %
			 4 = 100%  MI = 100 %
			* */
			$arrPtoidMI = Array(43, 42, 44, 45);

			$vlrInformado = retiraPontosBD($dados['valorpagamentoobra'][$preid]);
			$valorpagamentoobra = (float)$vlrInformado + (float)$dadospre['vrlpagamento'];

			if( $dadospre['homologacao'] == 'sim' ){
				$vrlDisponivel = ((50 / 100) * (float)$dadospre['prevalorobra']);
			}
			if( $dadospre['execucao25'] == 'sim' ){
				if( in_array($dadospre['ptoid'], $arrPtoidMI) ){
					$vrlDisponivel = ((90 / 100) * (float)$dadospre['prevalorobra']);
				} else {
					$vrlDisponivel = ((75 / 100) * (float)$dadospre['prevalorobra']);
				}
			}
			if( $dadospre['execucao50'] == 'sim' ){
				$vrlDisponivel = ((100 / 100) * (float)$dadospre['prevalorobra']);
			}

			if( (float)$vrlDisponivel < 1 ){
				echo "SIMEC INFORMA : ".$dadospre['descricao']." não foi homologada";
			} else {
				if( (float)$vlrInformado > (float)$vrlDisponivel ){
					echo "SIMEC INFORMA : O valor informado <b>R$ ".simec_number_format( $vlrInformado, 2, ',', '.' )."</b> para a obra: <b>".$dadospre['descricao']."</b>, utrapassa o valor disponivel: <b>R$ ".simec_number_format( $vrlDisponivel, 2, ',', '.' )."</b>";
				}
			}
		}
	}
}

function pegaValorProjeto( $ptoid, $estuf ){
	global $db;
	 
	$sql = "select sum(p.pirvalor * p.pirqtd) from obras.preitencomposicao_regiao p 
				inner join obras.preitenscomposicao pic on pic.itcid = p.itcid
			where pic.ptoid = {$ptoid} and p.estuf = '{$estuf}'";
	$valor = $db->pegaUm($sql);
	
	return $valor;
}

function formata_Processo( $numero ){
	if( !empty( $numero ) ) $processo = substr($numero,0,5) . ".".substr($numero,5,6)."/".substr($numero,11,4) . "-".substr($numero,15,2);
	return $processo;
}

function verificaVinculacaoNutricionista($cpfNut)
{
	global $db;
	// SQL PRINCIPAL
	$sql = "SELECT 
	vnstatus,
	vn.vndatavinculacao as vndatavinculacao,
	vn.vncargahorariasemanal as vncargahorariasemanal,
	vn.dutid as dutid,
	snaceito,
	vn.snid,
	du.dunid,
	vn.dntvid,
	vn.vnatuacaoexclusivaei
				
	
	FROM
		par.vinculacaonutricionista  vn
	INNER JOIN par.instrumentounidade inu ON inu.inuid = vn.inuid
	LEFT JOIN par.dadosunidade  	du ON du.duncpf = vn.vncpf AND vn.inuid = du.inuid
	LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod
	LEFT JOIN territorios.estado e ON e.estuf = inu.estuf
	
	WHERE
		vncpf =  '{$cpfNut}'
	
	AND
		CASE WHEN du.dunid IS NOT NULL
		THEN
			du.dutid in (11,12)
		ELSE
			true
		END
		";
	
	$result = $db->carregar($sql);
	// foreach
	$retorno = true;
	$result = ($result) ? $result : Array();
	
	if(count($result) > 0 )
	{
		foreach( $result as $k => $vUni )
		{
			
			if( $vUni['vnstatus'] == 'I' )
			{
				continue;
			}
			elseif ($vUni['snid'] == 3)
			{
				$retorno = false;
				return false;
				break;
			}
			else
			{
				
				if(($vUni['vndatavinculacao'] != '') && ($vUni['vncargahorariasemanal'] != '')  && ($vUni['dntvid'] != '') && ( $vUni['vnatuacaoexclusivaei'] != ''))
				{
					if(($vUni['dutid'] == 11))
					{
						if($vUni['snaceito'] != '')
						{
							continue;
						}
						else
						{
							$retorno = false;
							return false;
							break;
						}
					}
					else
					{
						continue;
					}
				}
				else 
				{
					$retorno = false;
					return false;
					break;
				}
			}
		}
	}else
	{
		return false;
	}
	
	return $retorno;
	
	// verifica campos principais 
		// 
	// Inativo pela entidade -- OK
	//if( ! ($vUni['vnstatus'] == 'I' && $vUni['snid'] == 3 ) )
}

function mostraHistoricoWorkflowObraMI( $arrParam = array() ){
	global $db;
		
	include_once APPRAIZ . 'includes/workflow.php';
	
	$docid = (integer) $arrParam['docid'];
	$documento = wf_pegarDocumento( $docid );
	$atual = wf_pegarEstadoAtual( $docid );
	
	$sql = "select
			    ed.esddsc,
			    ac.aeddscrealizada,
			    us.usunome,
			    to_char(hd.htddata, 'DD/MM/YYYY HH24:MI:SS') as htddata,
			    cd.cmddsc
			from workflow.historicodocumento hd
			    inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
			    inner join workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
			    inner join seguranca.usuario us on us.usucpf = hd.usucpf
			    left join workflow.comentariodocumento cd on cd.hstid = hd.hstid
			where
				hd.docid = " . $docid . "
				and ed.esdid in (228, 683, 754, 1486, 1548, 1578, 1487, 1564, 1553, 1561, 1579, 1550, 1561, 1488, 1489, 1568, 1770, 1488, 1549, 1489, 1566, 1551)
			order by
				hd.htddata asc, hd.hstid asc";
	
	$historico = $db->carregar( $sql );
	$historico = $historico ? $historico : array();
	
	//$historico = wf_pegarHistorico( $docid );
	?>
	<script type="text/javascript">
			
		IE = !!document.all;
		
		function exebirOcultarComentario( docid )
		{
			id = 'comentario' + docid;
			div = document.getElementById( id );
			if ( !div )
			{
				return;
			}
			var display = div.style.display != 'none' ? 'none' : 'table-row';
			if ( display == 'table-row' && IE == true )
			{
				display = 'block';
			}
			div.style.display = display;
		}
		
	</script>
	<table class="listagem" cellspacing="1" cellpadding="3" align="center" style="width: 100%;">
		<tr>
				<th style="text-align: center; background-color: #e0e0e0;" colspan="6" height="25px;"><div><b><?php echo $documento['docdsc']; ?></b></div></th>
			</tr>
	</table>
	<table class="listagem" cellspacing="1" cellpadding="3" align="center" style="width: 100%;">
		<thead>
			<?php if ( count( $historico ) ) : ?>
				<tr>
					<td style="width: 20px;" height="25px;"><b>Seq.</b></td>
					<td style="width: 200px;"><b>Onde Estava</b></td>
					<td style="width: 200px;"><b>O que aconteceu</b></td>
					<td style="width: 90px;"><b>Quem fez</b></td>
					<td style="width: 120px;"><b>Quando fez</b></td>
					<td style="width: 17px;">&nbsp;</td>
				</tr>
			<?php endif; ?>
		</thead>
		<?php $i = 1; ?>
		<?php foreach ( $historico as $item ) : ?>
			<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
			<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td align="right"><?=$i?>.</td>
				<td style="color:#008000;">
					<?php echo $item['esddsc']; ?>
				</td>
				<td valign="middle" style="color:#133368">
					<?php echo $item['aeddscrealizada']; ?>
				</td>
				<td style="font-size: 6pt;">
					<?php echo $item['usunome']; ?>
				</td>
				<td style="color:#133368">
					<?php echo $item['htddata']; ?>
				</td>
				<td style="color:#133368; text-align: center;">
					<?php if( $item['cmddsc'] ) : ?>
						<img
							align="middle"
							style="cursor: pointer;"
							src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/restricao.png"
							onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
						/>
					<?php endif; ?>
				</td>
			</tr>
			<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td colspan="6">
					<div >
						<?php echo simec_htmlentities( $item['cmddsc'] ); ?>
					</div>
				</td>
			</tr>
			<?php $i++; ?>
		<?php endforeach; ?>
		<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
		<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
			<td style="text-align: right;" colspan="6" height="25px;">
				<b>Estado atual:</b><span style="color:#008000;"> <?php echo $atual['esddsc']; ?></span>
			</td>
		</tr>
	</table>
	<?php 
}

function validaAnaliseObrasMIConvencional( $post ){
	global $db;
	
	$proid = $post['proid'];
	$prmparecer = $post['prmparecer_'.$proid];
	$esdiddestino = $post['esdiddestino_'.$proid];
	$prmanalisecontrapartida = ($post['prmanalisecontrapartida_'.$proid] ? $post['prmanalisecontrapartida_'.$proid] : 'N');
	$cmddsc = $prmparecer;
	$prmparecer	= ($prmparecer ? "'".str_replace("'", "", $prmparecer)."'" : 'null');
	
	$sql = "select pprid from par.processopedidoreformulacao where proid = ".$post['proid']." and pprstatus = 'A' order by pprid desc";
	$pprid = $db->pegaUm($sql);
	
	if( !empty($pprid) ){
		$sql = "UPDATE par.parecerreformulacaomi SET prmstatus = 'I' WHERE pprid in (select pprid from par.processopedidoreformulacao where proid = ".$post['proid']." and pprstatus = 'A')";
		$db->executar($sql);
			
		$sql = "INSERT INTO par.parecerreformulacaomi(pprid, prmtipo, esdid, prmparecer, usucpf, prmdata, prmanalisecontrapartida, prmstatus)
				VALUES ($pprid, null, {$esdiddestino}, ".$prmparecer.", '".$_SESSION['usucpf']."', now(), '{$prmanalisecontrapartida}', 'A')";
		$db->executar($sql);
		$db->commit();
	
		$sql = "select distinct sr.preid from par.processopedidoreformulacao pp
				    inner join par.solicitacaoreformulacaoobras sr on sr.pprid = pp.pprid
				where
					pp.proid = {$post['proid']}
				    and sr.sfoacaosolicitada = 'RE'
				    and pp.pprstatus = 'A'";
		$arPreid = $db->carregarColuna($sql);
		$arPreid = $arPreid ? $arPreid : array();
		
		foreach ($arPreid as $preid) {
				
			$docid = $db->pegaUm("select docid from obras.preobra where preid = $preid and prestatus = 'A'");
			$sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";
			$esdid = $db->pegaUm( $sql );
	
			$sql = "select aedid from workflow.acaoestadodoc where aedid not in (3835) and esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
			$aedid = $db->pegaUm( $sql );
 			//ver($sql, $aedid, $docid, $cmddsc, d);
			if( $aedid && $docid ){
				$arDados = array( 'preid' => $preid, 'lote' => true );
				$retorno = wf_alterarEstado( $docid, $aedid, $cmddsc, $arDados );
			}
		}
		if( $esdiddestino == WF_TIPO_EM_DILIGENCIA_SOLICITACAO_REFORMULACAO_MI_PARA_CONVENCIONAL ){
			$arProcesso = $db->pegaLinha("select muncod, pronumeroprocesso from par.processoobra where proid = {$post['proid']}");
			$sql = "SELECT
						ent.entemail
					FROM
						par.entidade ent
                    	inner join par.entidade ent2 ON ent2.inuid = ent.inuid and ent2.dutid = 6 and ent2.entstatus = 'A'
                        inner join territorios.municipio mun on mun.muncod = ent2.muncod
					WHERE
						ent.dutid =  7
						and ent.entstatus = 'A'
						and mun.muncod = '{$arProcesso['muncod']}'";
			$strEmailTo = (array)$db->pegaUm($sql);
			$strMensagem = '
			<html>
				<head>
					<title></title>
				</head>
				<body>
					<table style="width: 100%;">
						<thead>
							<tr>
								<td style="text-align: center;">
									<p><img  src="http://simec.mec.gov.br/imagens/brasao.gif" width="70"/><br/>
									<b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
									FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO<br/>
									DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS<br/>
									COORDENAÇÃO GERAL DE INFRAESTRUTURA EDUCACIONAL<br/> 
									SBS Quadra 02 - Bloco F - 14º andar - Edifício FNDE - CEP -70070-929<br/>
								</td>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td style="line-height: 15px;">
								</td>
							</tr>
							<tr>
								<td style="line-height: 15px; text-align:justify">
									<p>Prezado Gestor,<br> A sua solicitação de reformulação MI para Convencional foi diligenciada. Favor verificar o conteúdo da diligência - SIMEC, módulo PAR - ícone Acompanhamento da Solicitação MI. 
										Após correção, reencaminhar solicitação para análise da equipe técnica do FNDE..</p>
									                                                        
									<p>Para obter informações de como preencher o sistema, acesse o manual disponibilizado na página inicial do SIMEC-módulo PAR.</b>
								</td>
							</tr>
							<tr>
								<td style="padding: 10px 0 0 0;">
									Atenciosamente,
								</td>
							</tr>
							<tr>
								<td style="text-align: center; padding: 10px 0 0 0;">
									<img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="http://simec.mec.gov.br/imagens/obras/assinatura-fabio.png" />
									<br />
									FÁBIO LÚCIO DE A. CARDOSO<br>
									Coordenador-Geral de Infraestrutura Educacional - CGEST<br>
									Diretoria de Gestão, Articulação e Projetos Educacionais - DIGAP<br>
									Fundo Nacional de Desenvolvimento da Educação-FNDE<br>
								</td>
							</tr>
						</tbody>
					</table>
				</body>
			</html>';
			$strAssunto = "Processo {$arProcesso['pronumeroprocesso']} em Diligência";
			$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
			
			if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
				$strEmailTo = array($_SESSION['email_sistema']);
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			} else {
				$strEmailTo = $emailTo;
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
			}
		}
		return true;
	} else {
		return false;
	}
}

function carregarListaObrasMiConvencional( $post, $tipoOrigem = '' ){
	global $db;
	
	if( $tipoOrigem == 'T' ){
		$join = "inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'";
	} else {
		$join = "left join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'";		
	}
	
	$distrato = "'<a style=\"cursor: pointer; color: blue;\" onclick=\"donwloadDistrato('||dis.obrid||', \'\', '||dis.total||');\">Sim</a>'";
	
	$sql = "select distinct
				p.preid,
			    p.predescricao,
			    pt.ptodescricao,
			    pt.ptoid,
			    p.docid,
			    es1.esdid as situacaopreobra,
			    es.esddsc, o.obrid, 
			    cast(((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0)) as numeric(20,2)) || '%' as percentual_execucao,
			    case when dis.total > 0 then $distrato
			    else 'Não' end as distrato,
			    cast(p.prevalorobra as numeric(20,2)) as vlrobra,
			    sr.ptoidsolicitado,
    			sr.sfoacaosolicitada,
    			sr.sfojustificativa,
    			sr.sfocontrapartida,
    			sr.sfocontrapartidainformada,
    			(select sum(pir.pirvalor * pir.pirqtd) from obras.preitencomposicao_regiao pir
                      inner join obras.preitenscomposicao pic on pic.itcid = pir.itcid
                  where pic.ptoid = sr.ptoidsolicitado and pir.estuf = p.estuf limit 1) as vlrprojeto
			from
				par.processoobra po
			    inner join par.processoobraspaccomposicao pc on pc.proid = po.proid and pc.pocstatus = 'A'
			    inner join obras.preobra p on p.preid = pc.preid and p.prestatus = 'A'
			    inner join obras.pretipoobra pt on pt.ptoid = p.ptoid			    
			    left join obras2.obras o 
				    inner join workflow.documento d on d.docid = o.docid
				    inner join workflow.estadodocumento es on es.esdid = d.esdid
			    on o.obrid = p.obrid and o.obrstatus = 'A' and o.obridpai IS NULL			    
			    inner join workflow.documento d1 on d1.docid = p.docid
    			inner join workflow.estadodocumento es1 on es1.esdid = d1.esdid
    			left join (
    				SELECT distinct
				        count(oa.oarid) as total,
				        oa.obrid
				    FROM
				        obras2.obras_arquivos oa
				        JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
				        JOIN public.arquivo      a ON a.arqid = oa.arqid
				        JOIN seguranca.usuario usu ON usu.usucpf = a.usucpf
				    WHERE
				        oarstatus = 'A' 
				        AND (arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')
				        and oa.tpaid = 30 
				        and oa.oarstatus = 'A'
				    group by oa.obrid					
    			) dis on dis.obrid = o.obrid
			   $join
			where
				po.proid = {$post['proid']}
			order by p.predescricao";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$sql = "select pprjustificativa from par.processopedidoreformulacao where proid = ".$post['proid']." and pprstatus = 'A'";
	$pprjustificativa = $db->pegaUm($sql);
	
	$sql = "select distinct pf.prmparecer, coalesce(pf.prmanalisecontrapartida, 'N') as prmanalisecontrapartida, pf.esdid
			from par.processopedidoreformulacao pp
				inner join par.parecerreformulacaomi pf on pf.pprid = pp.pprid and pf.prmstatus = 'A'
			where
				pp.proid = {$post['proid']}
			    and pp.pprstatus = 'A'";
	$arrParecer = $db->pegaLinha($sql);
	$prmanalisecontrapartida = $arrParecer['prmanalisecontrapartida'];
	if( empty($prmanalisecontrapartida) ) $prmanalisecontrapartida = 'N';
	?>
	<script type="text/javascript">	  
		$1_11(function() {		
			$1_11( '.glyphicon-question-sign' ).tooltip({
	
				content: function() {
			          var text = jQuery(this).prev().val();
			          return text;
			      }
		      
			});
		  });
	</script>
	<style>
	  .ui-tooltip {
	    border: 2px solid white;
	    color: black;
	    border-radius: 10px;
	    /*font: 12px "Helvetica Neue", Sans-Serif;*/
	    box-shadow: 0 0 7px black;
	    z-index: 9999;
	    max-width: 400px;
	    position: absolute;
	    -webkit-box-shadow: 0 0 8px #aaa;
	  }
	  .quadro_resumo th{
	  	/*background-color: #00A5E0;*/
	  }
	  .titulo_resumo{
	  	background-color: rgb(31, 73, 125); 
	  	color: rgb(250, 125, 0); 	
	  }
  </style>
	<form name="formulario_<?php echo $post['proid']?>" id="formulario_<?php echo $post['proid']?>" method="post">   
    	<input type="hidden" name="requisicao" id="requisicao" value="">
    	<input type="hidden" name="proid" id="proid" value="<?php echo $post['proid']; ?>">
    	<input type="hidden" name="prmanalisecontrapartidainformada_<?php echo $post['proid']; ?>" value="<?php echo $prmanalisecontrapartida; ?>">
    	<input type="hidden" name="estuf_post" id="estuf_post" value="<?php echo $post['estuf']; ?>">
	<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
	<tr>
		<td>
	<table  width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
	<thead>
		<tr>
			<td width="20%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ações</td>
			<td width="02%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">ID</td>
			<td width="20%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Nome da Obra</td>
			<td width="10%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Tipo de Obra</td>
			<td width="10%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação do Obras2</td>
			<td width="05%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Percentual de Execução</td>
			<td width="05%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Tem Distrato</td>
			<td width="05%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor da Obra</td>
			<td width="10%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Reformular para:</td>
			<td width="05%" class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Projeto</td>
			<td class="title cab_projeto_analise_<?php echo $post['proid']; ?>" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Contrapartida</td>
			<td class="title cab_projeto_analise_<?php echo $post['proid']; ?>" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Obra Terá Contrapartida</td>
		</tr>
	</thead>
	<tbody>	
	<?php
	
	if( $arrDados ){
		$valorObra = 0;
		$valorProjeto = 0;
		$valorTotalContraPartidaInformada = 0;
		$valorTotalContraPartida = 0;
		$docid = '';
		foreach ($arrDados as $key => $v) {
			
			if( $v['sfoacaosolicitada'] == 'RE' ) $docid = $v['docid'];
			
			$valorObra += $v['vlrobra'];
			$valorTotalContraPartidaInformada += $v['sfocontrapartidainformada'];
			$valorTotalContraPartida += $v['sfocontrapartida'];
			$key % 2 ? $cor = "#dedfde" : $cor = ""; 
			
			$disabled = '';
			
			if( $v['ptoid'] == 73 ){//Projeto 1 Convencional
				$qtd = 188;
			}
			if( $v['ptoid'] == 74 ){//Projeto 2 Convencional
				$qtd = 94;
			}
			if( in_array($v['ptoid'], array(43, 44) ) ){ //Escola Proinfância B
				$qtd = 120;
			}
			if( in_array($v['ptoid'], array(42, 45)) ){ //Escola Proinfância C
				$qtd = 60;
			}			
			$v['sfocontrapartida'] = ($v['sfocontrapartida'] ? simec_number_format($v['sfocontrapartida'], '2', ',', '.') : '');
			?>
			<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td valign="middle">			
					<input type="hidden" name="preid[<?php echo $post['proid']?>][]" value="<?php echo $v['preid']; ?>">
					<input type="hidden" name="predescricao[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['predescricao']; ?>">
					<input type="hidden" name="ptodescricao[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['ptodescricao']; ?>">
					<input type="hidden" name="alunosatendido[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $qtd; ?>">
					<input type="hidden" name="ptoid_atual[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['ptoid']; ?>">
					<input type="hidden" name="situacaopreobra[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['situacaopreobra']; ?>">					
					<input type="hidden" name="sfocontrapartida[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['sfocontrapartida']; ?>">
					<input type="hidden" name="estadoAtual[<?php echo $_POST['proid']?>][<?php echo $v['preid']; ?>]" value="<?php echo $v['situacaopreobra']; ?>">
					
					<input type="hidden" value="" class="pendeciaobra_<?php echo $v['preid']; ?>">
					
					<span id="span_<?=$v['preid']; ?>" style="font-size:14px; cursor:pointer; color: red;" title="" class="glyphicon glyphicon-question-sign"></span>
					<input type="radio" name="sfoacaosolicitada[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" <?php echo ($v['sfoacaosolicitada'] == 'RE' ? 'checked="checked"' : '' ); /*echo $disabled;*/ ?> id="sfoacaosolicitada_re_<?php echo $post['proid']?>_<?php echo $v['preid']; ?>" value="RE"> Reformular
					<input type="radio" name="sfoacaosolicitada[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" <?php echo ($v['sfoacaosolicitada'] == 'CO' ? 'checked="checked"' : '' ); /*echo $disabled;*/ ?> id="sfoacaosolicitada_co_<?php echo $post['proid']?>_<?php echo $v['preid']; ?>" value="CO"> Cancelar Obra
					<input type="radio" name="sfoacaosolicitada[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" <?php echo ( ($v['sfoacaosolicitada'] == 'SA' /*|| !empty($disabled)*/ ) ? 'checked="checked"' : '' ); ?> id="sfoacaosolicitada_sa_<?php echo $post['proid']?>_<?php echo $v['preid']; ?>" value="SA"> Sem alteração
					</td>
				<td valign="middle" align="right" style="color:#999999;"><?=$v['preid']; ?></td>
				<td valign="middle">
						<img src="../imagens/alterar.gif" onclick="abreObraAnalise( <?php echo $v['preid']; ?> );" style="cursor:pointer"/>&nbsp;
						<a onclick="abreObraAnalise( <?php echo $v['preid']; ?> );" style="cursor:pointer"><?=$v['predescricao']; ?></a>
						</td>
				<td valign="middle"><?=$v['ptodescricao']; ?></td>
				<td valign="middle"><?=$v['esddsc']; ?></td>
				<td valign="middle"><?=$v['percentual_execucao']; ?></td>
				<td valign="middle"><?=$v['distrato']; ?></td>
				<td valign="middle" align="right" style="color:#999999;"><div id="div_valorobra_<?php echo $v['preid']; ?>"><?=simec_number_format($v['vlrobra'], '2', ',', '.'); ?></div></td>
				<td valign="middle" style="text-align: left">
					<div id="ptoid_combo_<?php echo $post['proid']?>_<?php echo $v['preid']; ?>" style="display: <?php echo ($v['sfoacaosolicitada'] == 'RE' ? '' : 'none' ); ?>">
					<?
					$sql = "select 
								p.ptoid as codigo,
							    p.ptodescricao as descricao
							from
								obras.pretipoobra p
							where
								p.ptostatus = 'A'
							    and p.ptoid in (73, 74)
							order by p.ptodescricao";
					?>
					<select name="ptoid[<?php echo $post['proid']?>][<?php echo $v['preid']; ?>]" class="CampoEstilo" style="width:190px;" onchange="pegaValorProjetoAjax(this.value, <?php echo $v['preid']; ?>, '<?php echo $post['estuf']; ?>'); AddTableRow(<?php echo $post['proid']?>);">
						<option value="">Selecione</option>
						<option value="73" <?php echo ($v['ptoidsolicitado'] == '73' ? 'selected="selected"' : '' )?> >Projeto 1 Convencional</option>
						<option value="74" <?php echo ($v['ptoidsolicitado'] == '74' ? 'selected="selected"' : '' )?> >Projeto 2 Convencional</option>
					</select>
					</div>
				</td>
				<td valign="middle" style="color:#999999;"><div id="div_valorprojeto_<?php echo $v['preid']; ?>"><?php
				echo simec_number_format($v['vlrprojeto'], '2', ',', '.');
				?></div></td>
				<td valign="middle" style="color:#999999;" class="td_projeto_analise_<?php echo $post['proid']?>"><div id="div_projeto_analise_<?php echo $post['proid']?>_<?php echo $v['preid']; ?>"><?php echo $v['sfocontrapartida']; ?></div></td>
				<td class="td_projeto_analise_<?php echo $post['proid']?>">
					<div id="div_projeto_contrapartida_<?php echo $post['proid']?>_<?php echo $v['preid']; ?>">
						<?php echo ($v['sfocontrapartidainformada'] ? simec_number_format($v['sfocontrapartidainformada'], 2, ',', '.' ) : '0,00');?>
					</div>
				</td>
			</tr>
		<?} ?>
	</tbody>
	<tfoot>
		<tr bgcolor="#D0D0D0">
			<td align="right"><b>Totais:</b></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"></td>
			<td align="right"><b><?=simec_number_format($valorObra, '2', ',', '.'); ?></b></td>
			<td align="right"></td>
			<td align="right"><b><div id="div_total_vrlprojeto_<?php echo $post['proid']?>"><?=simec_number_format($valorProjeto, '2', ',', '.'); ?></div></b></td>
			<td align="right" class="title cab_projeto_analise_<?php echo $post['proid']; ?>"><b><div id="div_total_vrlcontrapartida_<?php echo $post['proid']?>"><?=simec_number_format($valorTotalContraPartida, '2', ',', '.'); ?></div></b></td>
			<td align="right" class="title cab_projeto_analise_<?php echo $post['proid']; ?>"><b><div id="div_total_vrlcontrapartidainformada_<?php echo $post['proid']?>"><?=simec_number_format($valorTotalContraPartidaInformada, '2', ',', '.'); ?></div></b></td>
		</tr>
		<tr bgcolor="#D0D0D0" height="30px;" id="tr_label_just_<?php echo $post['proid']?>">
			<td colspan="10" align="center"><b>Parecer de Solicitação</b></td>
		</tr>
		<tr id="tr_campo_just_<?php echo $post['proid']?>">
			<td colspan="10" align="center">
				<textarea id="pprjustificativa[<?php echo $post['proid']?>]" name="pprjustificativa[<?php echo $post['proid']?>]" 
						cols="80" rows="5" title="Justificativa" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this );" 
						class="obrigatorio txareanormal"><?php echo $pprjustificativa; ?></textarea>
			</td>
		</tr>
		<tr>
			<td colspan="10" id="td_products_<?php echo $post['proid'];?>">
				<table id="products-table_<?php echo $post['proid'];?>" style="width: 95%" align="center" cellspacing="1" cellpadding="1" class="quadro_resumo">
				<tbody>
				<tr>
					<th colspan="9" height="20px" class="titulo_resumo" style="color: white;">Quadro Resumo</th>
				</tr>
				<tr>
					<th colspan="5" height="20px" width="50%" class="titulo_resumo" style="color: white;">Dados da Obra Atual</th>
					<th valign="middle" class="titulo_resumo" width="10%" id="td_row_<?php echo $post['proid'];?>" style="text-align: center;">
Resultado após<br>reformulações para<br>Atendimento as crianças</th>
					<th colspan="3" height="20px" width="40%" class="titulo_resumo" style="color: white;">Dados da Obra Alterado</th>
				</tr>
				<tr>
					<th class="titulo_resumo" height="20px" width="02%">ID</th>
					<th class="titulo_resumo" width="20%">Obras</th>
					<th class="titulo_resumo" width="20%">Tipo de obra Atual</th>
					<th class="titulo_resumo" width="10%">Valor Atual</th>
					<th class="titulo_resumo" width="10%">QTD de alunos atendidos</th>
					<th class="titulo_resumo" width="10%">Alteração do Tipo de obra</th>
					<th class="titulo_resumo" width="10%">Novo Valor</th>
	  				<th class="titulo_resumo" width="10%">QTD de alunos atendidos</th>
				</tr>
				</tbody>							
		  		</table>		  
			</td>
		</tr>
	</tfoot>
	<?} else { ?>
		<tr><td align="center" style="color:#cc0000;" colspan="10">Não foram encontrados registros de Obras.</td></tr>
	<?} ?>
	</table>
		</td>
		<td valign="top" align="center">
<?php 	

if( $docid ){	
	$estadoAtual = wf_pegarEstadoAtual( $docid );
	$esdidorigem = (integer) $estadoAtual['esdid'];
	if($esdidorigem == '755'){
		$filtroWork = ' and aedid in (1775, 3713, 3714, 3715)';
	}
	if( $esdidorigem != 228){
		
		$pos = strpos(str_to_upper($estadoAtual['esddsc']), str_to_upper('solicitação'));
		
		$sql = "select
					a.aedid,
		            a.aeddscrealizar,
		            a.esdidorigem,
					ed.esdid as esdiddestino,
					ed.esddsc
				from workflow.acaoestadodoc a
					inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
				where
					esdidorigem = $esdidorigem
					and aedstatus = 'A'
		            and ed.esdid not in (755)
		            and a.aedid not in (3835)
		            $filtroWork
				order by a.aedordem asc";
		if ($pos !== false) {
			$arrEstados = $db->carregar( $sql );
		}
		$arrEstados = $arrEstados ? $arrEstados : array();
	}
}
 ?>
	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="estado atual"><b>estado atual</b></span>
			</td>
		</tr>
		<tr style="text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="estado atual"><b><?php echo $estadoAtual['esddsc']; ?></b></span>
			</td>
		</tr>
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="estado atual"><b>ações</b></span>
			</td>
		</tr>
	<?php
if( $arrEstados[0] ){
	if( $tipoOrigem == 'T' ){
		foreach ($arrEstados as $arEstado) { ?>		
			<tr style="text-align: center;">
				<td style="font-size: 7pt; text-align: center;">
					<a style="cursor: pointer" onclick="tipoAnaliseParecerMI(<?php echo $arEstado['esdiddestino']?>, <?php echo $post['proid']?>);" title="<?php echo $arEstado['aeddscrealizar']?>"><?php echo $arEstado['aeddscrealizar']?></a>
				</td>
			</tr>
	<?php
		}
	} else { ?>
		<tr>
			<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
				nenhuma ação disponível para o documento
			</td>
		</tr>
<?php } ?>	
	<?php
} else {?>
	<tr>
		<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
			nenhuma ação disponível para o documento
		</td>
	</tr>
<?php 
}?>				
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="estado atual"><b>histórico</b></span>
			</td>
		</tr>
		<tr style="text-align:center;">
			<td style="font-size:7pt; border-top: 2px solid #d0d0d0;">
				<img style="cursor: pointer;" src="../imagens/fluxodoc.gif" title="" onclick="mostraHistoricoWorkflow( <?php echo $docid;?>, <?php echo $post['proid']?> );">
			</td>
		</tr>
	</table>
			
			</td>
	</tr>
	</table>
	</form>
	<div id="dialog_acoes_<?php echo $post['proid']?>" title="" style="display: none" >
		<div style="padding:5px;text-align:justify;" id="mostraRetorno_<?php echo $post['proid']?>"></div>
	</div>
	<div id="dialog_workflow_<?php echo $post['proid']?>" title="" style="display: none" >
		<div style="padding:5px;text-align:justify;" id="mostraWorkflow_<?php echo $post['proid']?>"></div>
	</div>
	<?php 
}

function verificarProcessoObraMI( $inuid ){
	global $db;
	
	$sqlObrasMiAviso = "SELECT
						TRUE
					FROM
					(
					SELECT
						pro.proid,
						COUNT(pre.preid) as qtd_obras,
						SUM(
							CASE WHEN crt.crtid IS NOT NULL 
								THEN 1
								ELSE 0
							END
						) as qtd_contratos
					FROM
						par.processoobra pro
						INNER JOIN par.instrumentounidade		inu ON ( inu.muncod = pro.muncod OR ( inu.estuf = pro.estuf AND pro.muncod IS NULL ) )
						INNER JOIN par.processoobraspaccomposicao 	poc ON poc.proid = pro.proid AND poc.pocstatus = 'A'
						INNER JOIN obras.preobra 			pre ON pre.preid = poc.preid AND pre.prestatus = 'A' 
													AND pre.preidpai IS NULL AND pre.ptoid IN (43, 42, 44, 45) 
													AND pre.preid NOT IN (SELECT preid FROM carga.par_obras_reformulacao_mi_convencional)
						INNER JOIN obras2.obras 			obr ON obr.preid = pre.preid
						LEFT  JOIN obras2.obrascontrato    		ocr ON ocr.obrid = obr.obrid AND ocr.ocrstatus = 'A'
						LEFT  JOIN obras2.contrato			crt ON crt.crtid = ocr.crtid AND crt.crtstatus = 'A'
					WHERE
						pro.prostatus = 'A'
						AND inu.inuid = {$inuid}
					GROUP BY
						pro.proid
					) as foo
					WHERE
						foo.qtd_obras >= 1";
	
	$dadosObrasAvisoMI = $db->pegaUm($sqlObrasMiAviso);
	return $dadosObrasAvisoMI;
}

function carregarListaProcessoObrasMI( $post, $tipoOrigem = '' ){
	global $db;
	
	if( $post['qtdobraprocesso'] ) $filtro = " having count(p.preid) = {$post['qtdobraprocesso']} ";
		
	$arWere = array();
		
	array_push($arWere, "po.proid in (select distinct
				                        po.proid
				                    from
				                        par.processoobra po
				                        inner join par.processoobraspaccomposicao pc on pc.proid = po.proid and pc.pocstatus = 'A'
										inner join par.instrumentounidade iu on ( iu.muncod = po.muncod and iu.mun_estuf is not null ) OR  ( po.estuf = iu.estuf AND po.muncod is null )
				                        inner join obras.preobra p on p.preid = pc.preid and p.prestatus = 'A'
				                        inner join obras.pretipoobra pt on pt.ptoid = p.ptoid
				                        inner join workflow.documento d on d.docid = p.docid
				                        inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
									where iu.inuid = '{$post['inuid']}'
										and pt.ptoid in (42, 43, 44, 45, 73, 74)
		                                --and sr.sfoacaosolicitada = 'RE'
									group by po.proid
									$filtro
								)");
		
	$imgMais = "<span style=\"font-size:16px; cursor:pointer;\" title=\"mais\" id=\"image_'||po.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||po.pronumeroprocesso||'\', '||po.proid||', \'".$post['estuf']."\', this);\"></span>";
	
	$sql = "select distinct
				'<center>
					$imgMais
				</center>' as mais,
				po.pronumeroprocesso||'&nbsp;' as processo,
			    ter.termonumero,
			    ter.vrltermo,
			    (select sum(se.saldo) from par.vm_saldo_empenho_por_obra se where se.processo = po.pronumeroprocesso) as vrlempenhado,
			    pag.vrlpagamento,
			    est.estado,
    			est.dataanalise,
    			vlrprojeto,
    			vlrcontrapartida,
    			con.qtdalunosatendido,
                con.qtdalunosatendidonovos
			from par.processoobra po
				inner join par.instrumentounidade iu on ( iu.muncod = po.muncod and iu.mun_estuf is not null ) OR  ( po.estuf = iu.estuf AND po.muncod is null )
			    inner join(
			    	select sum(p.prevalorobra) as vrltermo, 
			        	(select par.retornanumerotermopac(tc.proid)) as termonumero, tc.proid 
			        from par.termocompromissopac tc
			        	inner join par.termoobraspaccomposicao toc on toc.terid = tc.terid
			            inner join obras.preobra p on p.preid = toc.preid and p.prestatus = 'A'
			        where
			        	tc.terstatus = 'A'
			        	and p.ptoid IN (43, 42, 44, 45, 73, 74)
			        group by tc.proid
			    ) ter on ter.proid = po.proid
			    left join(
			    	select sum(pgo.pobvalorpagamento) as vrlpagamento, e.empnumeroprocesso from par.empenho e
			        	inner join par.pagamento pg on pg.empid = e.empid and pg.pagstatus = 'A'
			            inner join par.pagamentoobra pgo on pgo.pagid = pg.pagid
			            inner join obras.preobra p on p.preid = pgo.preid and p.prestatus = 'A'
			        where
			        	e.empstatus = 'A'
			            and pg.pagsituacaopagamento not ilike '%cancelado%'
			            and p.ptoid IN (43, 42, 44, 45, 73, 74)
			        group by e.empnumeroprocesso
			    ) pag on pag.empnumeroprocesso = po.pronumeroprocesso
			    left join(
			    	SELECT es1.esddsc as estado, to_char(max(hd.htddata), 'DD/MM/YYYY HH24:MI:SS') as dataanalise, pc.proid 
			        FROM par.processoobraspaccomposicao pc
			            inner join obras.preobra p1 on p1.preid = pc.preid and pc.pocstatus = 'A'
			            inner join par.solicitacaoreformulacaoobras sr on sr.preid = p1.preid and sr.sfostatus = 'A' and sr.sfoacaosolicitada = 'RE'
			            inner join workflow.documento d1 on d1.docid = p1.docid and d1.esdid not in (228)
			            inner join workflow.estadodocumento es1 on es1.esdid = d1.esdid
			            inner join workflow.historicodocumento hd on hd.docid = d1.docid and hd.aedid in (3597, 3699)  
			            inner join par.processoobra po on po.proid = pc.proid and po.prostatus = 'A' 
                        inner join par.instrumentounidade iu on ( iu.muncod = po.muncod and iu.mun_estuf is not null ) OR  ( po.estuf = iu.estuf AND po.muncod is null )
			        WHERE p1.ptoid IN (43, 42, 44, 45, 73, 74) 
			        	and iu.inuid = {$post['inuid']}
			    	GROUP BY es1.esddsc, pc.proid
			    ) est on est.proid = po.proid
			    left join(
			    	SELECT
					     sum(sr.sfocontrapartida) as vlrcontrapartida, pc.proid, 
					     sum(pto.ptoqtdalunosatendidos) as qtdalunosatendido, 
					     
					     coalesce(sum(pir.valorprojeto), 0) as vlrprojeto,
					     
					     /*sum(coalesce((select sum(pr.pirvalor) from obras.preitencomposicao_regiao pr
					    	inner join obras.preitenscomposicao pic on pic.itcid = pr.itcid 
					      where pr.estuf = p.estuf and pr.pirstatus = 'A' and pic.ptoid = sr.ptoidsolicitado and pic.itcstatus = 'A'), 0)) as vlrprojeto,*/
					      
					     sum((case when sr.sfoacaosolicitada = 'SA' then 
					     	pto.ptoqtdalunosatendidos 
					     else ptor.ptoqtdalunosatendidos end)) as qtdalunosatendidonovos     
					FROM par.processoobraspaccomposicao pc
					    inner join obras.preobra p on p.preid = pc.preid and pc.pocstatus = 'A' and p.prestatus = 'A'
					    inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
					    inner join par.processoobra po on po.proid = pc.proid and po.prostatus = 'A' 
                        inner join par.instrumentounidade iu on ( iu.muncod = po.muncod and iu.mun_estuf is not null ) OR  ( po.estuf = iu.estuf AND po.muncod is null ) 
					    left join obras.pretipoobra ptor on ptor.ptoid = sr.ptoidsolicitado and ptor.ptostatus = 'A'
					    left join obras.pretipoobra pto on pto.ptoid = p.ptoid and pto.ptostatus = 'A'
					    left join(
					    	select sum(pr.pirvalor * pr.pirqtd) as valorprojeto, pic.ptoid, pr.estuf 
					    	from obras.preitencomposicao_regiao pr
					            inner join obras.preitenscomposicao pic on pic.itcid = pr.itcid
					        where pic.itcstatus = 'A' and pr.pirstatus = 'A'
					        group by pic.ptoid, pr.estuf
					    ) pir on pir.ptoid = sr.ptoidsolicitado and pir.estuf = p.estuf
					WHERE
					    p.ptoid IN (43, 42, 44, 45, 73, 74) 
					    and iu.inuid = {$post['inuid']}
					GROUP BY pc.proid
			    ) con on con.proid = po.proid
			where
				po.prostatus = 'A'
				and iu.inuid = '{$post['inuid']}'
				".($arWere ? ' and '.implode(' and ', $arWere) : '');
	
	$cabecalho = array('Abrir/Fechar', 'Processo', 'Nº Termo', 'Valor Pactuado', 'Valor empenhado', 'Valor pago', 'Situação do Fluxo (Workflow)', 'Envio para Análise', 'Novo Valor Proposto', 'Previsão de Contrapartida',
	'QTD de Alunos Atendidos', 'Novo QTD de Alunos Atendidos');
	$db->monta_lista_simples( $sql, $cabecalho,100,5,'S','100%','S', true, false, false, false);
}

function retornaObraAprovadaRMC($preid, $lote = false ){
	
	global $db;
		
	$sfoacaosolicitada = $db->pegaUm("select s.sfoacaosolicitada from par.solicitacaoreformulacaoobras s where preid = $preid and s.sfostatus = 'A'");
	
	if( in_array($sfoacaosolicitada, array('SA', 'CO')) ){
		return true;
	} else {
		return false;
	}
}

function gravaHtmlDocumento($doptexto, $idDocumento, $processo, $tipo){
	global $db;
	
	ob_clean();
	
	if( $doptexto ){
		if( strpos($doptexto, '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
			$doptexto = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
		} else {
			$doptexto = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
		}
	}
	$doptexto = $doptexto ? simec_htmlspecialchars($doptexto) : 'null';
	
	$nomeArquivo 		= 'Minuta_Documento_'.$tipo.'_'.$idDocumento.'_'.$processo.'_'.date('YmdHis');
	$diretorio		 	= APPRAIZ . 'arquivos/par/documentoTermo';
	$diretorioArquivo 	= APPRAIZ . 'arquivos/par/documentoTermo/'.$nomeArquivo.'.txt';
	
	if( !is_dir($diretorio) ){
		mkdir($diretorio, 0777);
	}
		 		
	$fp = fopen($diretorioArquivo, "w");
	if ($fp) {
		stream_set_write_buffer($fp, 0);
		fwrite($fp, $doptexto);
		fclose($fp);
	}
	
	if( $tipo == 'PAC' ){		
		$dopid = 'null';
		$terid = $idDocumento;
		//$db->executar("update par.termocompromissopac set terdocumento = null where terid = $terid");
		$db->executar("update par.documentotermoarquivo set dtastatus = 'I' where terid = $terid");
	} else {
		$dopid = $idDocumento;
		$terid = 'null';
		//$db->executar("update par.documentopar set doptexto = null where dopid = $dopid");
		$db->executar("update par.documentotermoarquivo set dtastatus = 'I' where dopid = $dopid");
	}
	
	$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
			VALUES( '".$nomeArquivo."',
					'txt',
					'".$nomeArquivo."',
					'text/plain',
					'".filesize($diretorioArquivo)."',
					'".date('Y-m-d')."',
					'".date('H:i:s')."',
					'".$_SESSION["usucpf"]."',
					".($_SESSION['sisid'] ? $_SESSION['sisid'] : 'null').",
					'A') RETURNING arqid";
	$arqid = $db->pegaUm($sql);
		
	$sql = "insert into par.documentotermoarquivo(dopid, terid, arqid, dtaprocesso, dtatipo, dtanomearquivo, dtastatus)
			values($dopid, $terid, $arqid, $processo, '$tipo', '".$nomeArquivo.'.txt'."', 'A')";
	$db->executar($sql);
	$db->commit();
	return true;
}

function pegaTermoCompromissoArquivo( $dopid, $terid ){
	global $db;
	
	if( $dopid || $terid ){
		
		if( $terid ){
			$dtanomearquivo = $db->pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.terid = {$terid} order by d.dtaid desc");
		} else {
			$dtanomearquivo = $db->pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.dopid = {$dopid} order by d.dtaid desc");
		}
		
		if( $dtanomearquivo ){
			$diretorio = APPRAIZ . 'arquivos/par/documentoTermo/'.$dtanomearquivo;
			if( is_file($diretorio) ){
				$doptexto = file_get_contents($diretorio);
			}
		}
	}
	return $doptexto;
}

?>