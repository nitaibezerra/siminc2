<?php
$ano = $_REQUEST ['ano'];
if ($ano != date('Y')) {
	$visivel = 'disabled="disabled"';
}

ini_set ( "memory_limit", "1000M" );
set_time_limit ( 0 );
// Busca perfil
$perfil = pegaPerfilGeral ();
// CAso seja um dos perfis abaixo poderar aceitar a emenda
if (in_array ( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_FINANCEIRA, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_TECNICA, $perfil )) 

{
	$boolAceitar = true;
} else {
	$boolAceitar = false;
}

if( date('Y-m-d') >= date('Y').'04-01' ){
	$boolAceitar = false;
}

$inuid = $_SESSION ['par'] ['inuid'];

$arrayPerfil = pegaArrayPerfil ( $_SESSION ['usucpf'] );

if ($_REQUEST ['requisicao'] == 'desvincular') {
	
	$sql = "update par.subacaoemendapta set sepstatus = 'I' where sepid = {$_REQUEST['sepid']}";
	$db->executar($sql);
	echo $db->commit();
	
	exit();
}
if ($_REQUEST ['carregaEmendaDetalhePTA']) {
	header ( 'content-type: text/html; charset=ISO-8859-1' );
	
	$ano = $_REQUEST ['ano'];
	if ($ano != date('Y')) {
		$visivel = 'disabled="disabled"';
	}
	
	$sql = "SELECT DISTINCT
				ppsdsc,
				ppsid,
				foo.sbaid,
				foo.itrid,
				sbd.sbdid,
				sep.sepvalor,
				sep.sepid
                       FROM(
                               SELECT DISTINCT
                                      ps.ppsdsc, ps.ppsid, dim.itrid,
                                      (
                                              SELECT s.sbaid
                               FROM par.pontuacao p
                               INNER JOIN par.acao   a ON a.ptoid = p.ptoid AND a.acistatus = 'A'
                               INNER JOIN par.subacao        s ON s.aciid = a.aciid AND s.sbastatus = 'A'
                               WHERE p.inuid = $inuid AND  s.ppsid = ps.ppsid LIMIT 1
                                      ) AS sbaid
                               FROM par.propostasubacao ps
                               INNER JOIN par.propostasubacaoiniciativaemenda        pse  ON pse.ppsid  = ps.ppsid
                               INNER JOIN emenda.iniciativadetalheentidade   ide  ON ide.iniid  = pse.iniid  AND ide.idestatus = 'A'
                               INNER JOIN emenda.v_emendadetalheentidade             vede ON vede.edeid = ide.edeid
                               INNER JOIN par.indicador                                             ind  ON ind.indid  = ps.indid   AND ind.indstatus = 'A'
                           INNER JOIN par.area                                                      ar   ON ar.areid   = ind.areid  AND ar.arestatus = 'A'
                           INNER JOIN par.dimensao                                          dim  ON dim.dimid  = ar.dimid   AND dim.dimstatus = 'A'
                           INNER JOIN par.criterio c on c.indid = ind.indid 
                           INNER JOIN par.propostaacao pa ON pa.crtid = c.crtid
                           INNER JOIN emenda.emendapariniciativa                    epi  ON epi.ppsid  = ps.ppsid   AND epi.edeid = vede.edeid AND vede.edestatus = 'A'
                               WHERE
                                      ps.frmid in (14, 15)
                                      AND vede.emdid = {$_REQUEST['emdid']}
                                      AND dim.itrid  " . ($_SESSION ['par'] ['itrid'] == 1 ? " in ( 1, 3 ) " : " = {$_SESSION['par']['itrid']}") . "
                                      AND vede.emetipo = 'E'
                               GROUP BY ps.ppsdsc, ps.ppsid, dim.itrid, sbaid
                       ) as foo
                       LEFT JOIN par.subacaodetalhe sbd
                               LEFT JOIN par.subacaoemendapta sep ON sep.sbdid = sbd.sbdid AND sep.emdid = {$_REQUEST['emdid']} AND ptrid = {$_REQUEST['ptrid']} and sep.sepstatus = 'A'
                               ON sbd.sbaid = foo.sbaid and sbdano = '" . $ano . "'";
	
	$arrSubAcao = $db->carregar ( $sql );
	$arrSubAcao = $arrSubAcao ? $arrSubAcao : array ();
	$arrRegistro = array ();
	foreach ( $arrSubAcao as $key => $v ) {
		
		$arrSubExc = listaSubacaoExcusivoParaEntidade ( $v ['ppsid'], $_SESSION ['par'] ['itrid'] );
		$boHabilita = false;
		
		if ($arrSubExc) {
			if (in_array ( $_SESSION ['par'] ['inuid'], $arrSubExc )) {
				$boHabilita = true;
			} else {
				$boHabilita = false;
			}
		} else {
			$boHabilita = true;
		}
		
		$checkBox = '<center><input type="checkbox" name="chk[]" value="" disabled/></center>';
		
		if ($boHabilita) {
			$display = 'display:none;';
			if (( int ) $v['sbdid']) {
				$boEmendaPTA = $db->pegaUm ( "SELECT count(sepid) FROM par.subacaoemendapta 
												WHERE sbdid = {$v['sbdid']} and emdid = {$_REQUEST['emdid']} and ptrid = {$_REQUEST['ptrid']} and sepstatus = 'A'" );
				if (( int ) $boEmendaPTA > 0) {
					$display = '';
					$checkBox = '<center>
								<input type="hidden" id="sepid_' . $_REQUEST ['emdid'] . '_' . $_REQUEST ['ptrid'] . '_' . $v ['ppsid'] . '" name="sepid[' . $_REQUEST ['ptrid'] . '][' . $_REQUEST ['emdid'] . '][' . $v ['ppsid'] . ']" value="' . $v ['sepid'] . '">
								<input type="checkbox" "' . $visivel . '" class="chk" checked disabled id="chk_' . $_REQUEST ['emdid'] . '_' . $_REQUEST ['ptrid'] . '_' . $v ['ppsid'] . '" name="chk[' . $_REQUEST ['ptrid'] . '][' . $_REQUEST ['emdid'] . '][' . $v ['ppsid'] . ']" value="' . $v ['sbaid'] . '""/></center>';
					// Trecho comentado de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
					/*if (number_format ( $v ['sepvalor'], 2, ',', '.' ) == '0,00') {
						$checkBox .= ' <input type="hidden" id="chk_' . $_REQUEST ['emdid'] . '_' . $_REQUEST ['ptrid'] . '_' . $v ['ppsid'] . '" name="chk[' . $_REQUEST ['ptrid'] . '][' . $_REQUEST ['emdid'] . '][' . $v ['ppsid'] . ']" value="' . $v ['sbaid'] . '"/>';
					}*/
				} else {
					$checkBox = '<center><input type="checkbox"  ' . $visivel . ' class="chk" onclick="verificaCHK();" id="chk_' . $_REQUEST ['emdid'] . '_' . $_REQUEST ['ptrid'] . '_' . $v ['ppsid'] . '" name="chk[' . $_REQUEST ['ptrid'] . '][' . $_REQUEST ['emdid'] . '][' . $v ['ppsid'] . ']" value="' . $v ['sbaid'] . '"/></center>';
				}
			} else {
				$checkBox = '<center><input type="checkbox" ' . $visivel . ' class="chk" onclick="verificaCHK();" id="chk_' . $_REQUEST ['emdid'] . '_' . $_REQUEST ['ptrid'] . '_' . $v ['ppsid'] . '" name="chk[' . $_REQUEST ['ptrid'] . '][' . $_REQUEST ['emdid'] . '][' . $v ['ppsid'] . '_' . $v ['itrid'] . ']" value="' . $v ['sbaid'] . '"/></center>';
			}
			// Trecho comentado de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
			/*if (number_format ( $v ['sepvalor'], 2, ',', '.' ) == '0,00') {
				if (count ( $arrSubAcao ) < 2 && $v ['sepvalor'] == '') {
					
					$sql = "SELECT
								sum(vede.edevalor) as valor
							FROM
								emenda.v_emendadetalheentidade vede
							INNER JOIN emenda.autor 					a   ON a.autid    = vede.autid
							INNER JOIN emenda.entidadebeneficiada 		enb ON enb.enbid  = vede.entid
							INNER JOIN emenda.ptemendadetalheentidade 	pte ON vede.edeid = pte.edeid
							INNER JOIN emenda.planotrabalho 			ptr ON ptr.ptrid  = pte.ptrid
							WHERE
								vede.edestatus = 'A'
								AND vede.ededisponivelpta = 'S'
								AND vede.emeano = '".$ano."'
								AND vede.emetipo = 'E'
								AND vede.emdid = '{$_REQUEST['emdid']}'
								AND ptr.ptrstatus = 'A'
								AND ptr.ptrexercicio = '".$ano."'
								AND ptr.sisid = 23";
					
					$v ['sepvalor'] = $db->pegaUm ( $sql );
				}
				$campoTexto = campo_texto ( "sepvalor[{$_REQUEST['emdid']}][{$_REQUEST['ptrid']}][{$v['ppsid']}]", "N", "S", "", 16, 18, "[.###],##", "", '', '', 0, "id=\"sepvalor_{$_REQUEST['emdid']}_{$_REQUEST['ptrid']}_{$v['ppsid']}\"", '', number_format ( $v ['sepvalor'], 2, ',', '.' ), '', '' );
			} else {
				$campoTexto = campo_texto ( "sepvalor[{$_REQUEST['emdid']}][{$_REQUEST['ptrid']}][{$v['ppsid']}]", "N", "N", "", 16, 18, "[.###],##", "", '', '', 0, "id=\"sepvalor_{$_REQUEST['emdid']}_{$_REQUEST['ptrid']}_{$v['ppsid']}\"", '', number_format ( $v ['sepvalor'], 2, ',', '.' ), '', '' );
			}*/
			$hidden = '<input type="hidden" name="emdid[]" value="' . $_REQUEST ['emdid'] . '"/>
					   <input type="hidden" name="ptrid[' . $_REQUEST ['emdid'] . ']" value="' . $_REQUEST ['ptrid'] . '"/>';
			
			//Trecho comentado de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
			//$divCampo = "<div style=\"$display float:right\" id=\"divValor_{$_REQUEST['emdid']}_{$_REQUEST['ptrid']}_{$v['ppsid']}\">$campoTexto$hidden</div>";
			$descricao = $v ['ppsdsc'] . $hidden;
		} else {
			$descricao = '<span style="color: red;"><b>A Suba��o est� vinculada a um grupo de munic�pios que este munic�pio n�o faz parte.</b></span>';
		}
		// Demanda 2201 PAR que diz para retirar o campo de valor
		/*
			$arrRegistro [$key] = array (
				'acao' => $checkBox,
				'ppsdsc' => $descricao,
				'valor' => $divCampo
			);
		
		*/
		$arrRegistro [$key] = array (
				'acao' => $checkBox,
				'ppsdsc' => $descricao
		);
	}
	
	echo '<table cellspacing="0" class="tabela" cellpadding="0" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none; width: 100%">
				<tr>
					<td bgcolor="#e9e9e9" align="center" style=""><b>Recurso(s) Vinculado(s) ao Plano de Trabalho</b></td>
				</tr>
			</table>';
//	Demanda 2201 PAR que diz para retirar o campo de valor
	//$cabecalho = array ("A��o", "Descri��o", "Valor");
	$cabecalho = array ("A��o", "Descri��o");
	echo $db->monta_lista_simples ( $arrRegistro, $cabecalho, 500000, 5 );
	exit ();
}

if (! $inuid || $inuid == '') {
	echo "<script>alert('Entidade n�o encontrada!');window.close();</script>";
	exit ();
}

if ($_REQUEST ['requisicao'] == 'salvar') {
	// ver($_POST, d);
	
	if ($_POST ['chk']) {
		//ver($_POST ['chk'], $_POST ['excluiremenda']);
		/*if ($_POST ['excluiremenda']) {
			$sql = "UPDATE par.subacaoemendapta SET sepstatus = 'I' WHERE sbdid IN (SELECT sbdid FROM par.subacaodetalhe WHERE sbaid IN (" . $_POST ['excluiremenda'] . ") AND sbdano = '" . $ano . "' )";
			$db->executar ( $sql );
			$sql = "DELETE FROM par.subacaodetalhe WHERE sbaid IN (" . $_POST ['excluiremenda'] . ") AND sbdano = '" . $ano . "'";
			$db->executar ( $sql );
		}*/
		// ver($_POST['chk'],$_POST['sepvalor'],d);
		foreach ( $_POST ['chk'] as $pta => $arrEmendas ) {
			foreach ( $arrEmendas as $emendas => $arrPpsid ) {
				$valor = 0;
				// Trecho comentado de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
				/*if (is_array ( $_POST ['sepvalor'] [$emendas] [$pta] )) {
					foreach ( $_POST ['sepvalor'] [$emendas] [$pta] as $valorSub ) {
						$valorSub = str_replace ( '.', '', $valorSub );
						$valorSub = str_replace ( ',', '.', $valorSub );
						$valor += $valorSub;
					}
				}
				if ($valor > $_POST ['valorMax'] [$emendas] [$pta]) {
					echo "
						<script>
							alert('A soma dos valores propostos para as suba��es ultrapassa o valor da emenda associada.');
							window.location.href = 'par.php?modulo=principal/propostaEmendaSubacao&acao=A';
						</script>
					";
					die ();
				}
				*/
				// erifica quais suba��es foram desmarcadas.
				$sql = "SELECT sd.sbaid FROM par.subacaoemendapta se
						INNER JOIN par.subacaodetalhe sd ON sd.sbdid = se.sbdid
						WHERE /*se.emdid = $emendas AND*/ se.ptrid = $pta and se.sepstatus = 'A'";
				
				$arrSubChk = $db->carregarColuna ( $sql );
				
				foreach ( $arrPpsid as $ppsid => $sbaid ) {
					
					$ppsid = explode ( '_', $ppsid );
					$itrid = $ppsid [1];
					$ppsid = $ppsid [0];
					
					//$valor = str_replace ( '.', '', $_POST ['sepvalor'] [$emendas] [$pta] [$ppsid] );
					//$valor = str_replace ( ',', '.', $valor );
					// O valor abaixo est� zerado e sua declara��o comentada de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
					$valor = 0;
					
					$ssuid = (($_POST ['tipoAcao'] == 'C') ? 22 : 7);
					
					if (! empty ( $sbaid )) {
						$sql = "SELECT sbdid FROM par.subacaodetalhe WHERE sbaid = $sbaid and sbdano = '" . $ano . "'";
						$sbdid = $db->pegaUm ( $sql );
						
						if (empty ( $sbdid )) {
							$sql = "INSERT INTO par.subacaodetalhe(sbaid, sbdano, ssuid)
									VALUES ($sbaid, '" . $ano . "', $ssuid) returning sbdid";
							$sbdid = $db->pegaUm ( $sql );
							//sepvalor inserido como 0 de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
							$sql = "INSERT INTO par.subacaoemendapta(sbdid, emdid, ptrid, sepvalor)
									VALUES ($sbdid, $emendas, $pta, $valor)";
							$db->executar ( $sql );
						} else {
							$sepid = $_POST ['sepid'] [$pta] [$emendas] [$ppsid];
							
							$sql = "UPDATE par.subacaodetalhe SET ssuid = $ssuid
									WHERE sbdid = $sbdid";
							$db->executar ( $sql );
							
							if ($sepid) {
								$sql = "UPDATE par.subacaoemendapta SET sepstatus = 'I' WHERE sepid = $sepid";
								$db->executar ( $sql );
							}
							//sepvalor inserido como 0 de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
							$sql = "INSERT INTO par.subacaoemendapta(sbdid, emdid, ptrid, sepvalor)
									VALUES ($sbdid, $emendas, $pta, $valor)";
							$db->executar ( $sql );
						}
						
						// caso a suba��o ja esteja inserida n�o realiza��o a tramita��o
						if (! in_array ( $sbaid, $arrSubChk )) {
							require_once (APPRAIZ . 'includes/workflow.php');
							
							$docid = $db->pegaUm ( "select docid from par.subacao where sbaid = $sbaid" );
							$estadoAtual = wf_pegarEstadoAtual ( $docid );
							$esdiddestino = (($_POST ['tipoAcao'] == 'C') ? WF_SUBACAO_DILIGENCIA_CONDICIONAL : WF_SUBACAO_DILIGENCIA);
							$aedid = $db->pegaUm ( "select a.aedid from workflow.acaoestadodoc a
													where a.esdidorigem = {$estadoAtual['esdid']}
														and a.esdiddestino = " . $esdiddestino . "
													    and a.aedstatus = 'A'" );
							
							if (empty ( $aedid )) {
								$sql = "SELECT aeddscrealizar, aeddscrealizada FROM workflow.acaoestadodoc WHERE aedstatus = 'A' and esdiddestino = $esdiddestino limit 1";
								$arAcoes = $db->pegaLinha ( $sql );
								
								$sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada,
								  					aedcondicao, aedobs, aedposacao, aedvisivel, aedicone, aedcodicaonegativa)
											VALUES ({$estadoAtual['esdid']}, $esdiddestino, '{$arAcoes['aeddscrealizar']}', '{$arAcoes['aeddscrealizada']}',
								  					'', '', '', false, '', false) RETURNING aedid";
								
								$aedid = $db->pegaUm ( $sql );
							}
							if (( int ) $estadoAtual ['esdid'] != ( int ) $esdiddestino) {
								$sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, htddata)
												VALUES ($aedid, $docid, '{$_SESSION['usucpf']}', now()) returning hstid";
								$hstid = $db->pegaUm ( $sql );
								
								$sql = "update workflow.documento set esdid = $esdiddestino where docid = $docid";
								$db->executar ( $sql );
								
								$cmddsc = trim ( 'Tramita��o por Proposta Suba��o Emendas' );
								$cmddsc = str_replace ( "'", "\\'", $cmddsc );
								$sql = "insert into workflow.comentariodocumento(docid, hstid, cmddsc, cmddata, cmdstatus)
										values(" . $docid . "," . $hstid . ", '" . addslashes ( $cmddsc ) . "', now(), 'A' )";
								$db->executar ( $sql );
							}
							
							array_push ( $arrSubChk, $sbaid );
						}
					} else {
						$indid = $db->pegaUm ( "SELECT indid FROM par.propostasubacao WHERE ppsid = " . $ppsid );
						
						$sql = "select a.aciid from
									par.indicador i
									inner join par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
									inner join par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
									inner join par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
								where
									i.indid = {$indid} AND p.inuid = " . $inuid;
						// ver($sql,d);
						$aciid = $db->pegaUm ( $sql );
						$erro = false;
						if (! $aciid) {
							// Adiciono a a��o
							// $sql = "SELECT ppa.ppaid, p.ptoid
							// FROM
							// par.propostasubacao pps
							// INNER JOIN par.criterio c ON c.indid = pps.indid
							// INNER JOIN par.propostaacao ppa ON ppa.crtid = c.crtid
							// INNER JOIN par.pontuacao p ON p.crtid = ppa.crtid AND p.inuid = {$inuid} AND p.ptostatus = 'A'
							// WHERE
							// pps.ppsid = ".$ppsid;
							
							$sql = "SELECT pa.ppaid, p.ptoid
									FROM par.dimensao d
									INNER JOIN par.area 			a  ON a.dimid = d.dimid
									INNER JOIN par.indicador 		i  ON i.areid = a.areid
									INNER JOIN par.criterio  		c  ON c.indid = i.indid
									INNER JOIN par.propostaacao 	pa ON pa.crtid = c.crtid
									INNER JOIN par.pontuacao 		p  ON p.crtid = c.crtid AND p.ptostatus = 'A' AND p.inuid = $inuid 
									INNER JOIN par.propostasubacao 	ps ON ps.indid = i.indid
									WHERE 
										ps.ppsid = $ppsid 
										AND itrid = $itrid";
							
							$dadosAcao = $db->pegaLinha ( $sql );
							
							if ($dadosAcao ['ptoid'] == '') {
								$arvore = $itrid == 3 ? 'Brasil-Pr�' : 'PAR';
								echo "
									<script>
										alert('Seu plano do $arvore n�o foi elaborado. Necess�rio pontuar os indicadores.');
										window.location.href = window.location.href;
									</script>";
								die ();
							}
							// ver($sql,d);
							if ($dadosAcao ['ppaid']) {
								$dadosPropostaAcao = $db->pegaLinha ( "select * from par.propostaacao WHERE ppaid = " . $dadosAcao ['ppaid'] );
								
								$oAcao = new Acao ();
								
								$oAcao->ptoid = $dadosAcao ['ptoid'];
								$oAcao->acidsc = $dadosPropostaAcao ['ppadsc'];
								$oAcao->acistatus = 'A';
								$oAcao->acidata = 'NOW()';
								$oAcao->usucpf = $_SESSION ['usucpf'];
								$aciid = $oAcao->salvar ();
								$oAcao->commit ();
								$erro = false;
							} else {
								$erro = true;
							}
						}
						
						if (! $erro) {
							$desc = (($_POST ['tipoAcao'] == 'C') ? 'Em Dilig�ncia Condicional' : 'Em Dilig�ncia');
							$esdid = (($_POST ['tipoAcao'] == 'C') ? WF_SUBACAO_DILIGENCIA_CONDICIONAL : WF_SUBACAO_DILIGENCIA);
							$ssuid = (($_POST ['tipoAcao'] == 'C') ? 22 : 7);
							
							$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
									VALUES (62, " . $esdid . ", '" . $desc . "', now()) returning docid ";
							$docid = $db->pegaUm ( $sql );
							
							$sql = "SELECT ppsid, ppsdsc, ppsordem, frmid, prgid, indid, ppsestrategiaimplementacao, undid, ppspeso, foaid, ppsobra,
										ppsptres, ppsnaturezadespesa, ppscronograma, ppsmonitora, ptsid, ppsobjetivo, ppstexto, ppscobertura, ppscarga
									FROM par.propostasubacao WHERE ppsid = " . $ppsid;
							$propostaSubacao = $db->pegaLinha ( $sql );
							
							$sql = "INSERT INTO par.subacao(sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao, sbaptres, sbanaturezadespesa, sbamonitoratecnico,
										docid, frmid, indid, foaid, undid, ppsid, aciid, ptsid, sbacronograma, usucpf, sbapropostaemenda)
									VALUES ('{$propostaSubacao['ppsdsc']}',
											'{$propostaSubacao['ppsordem']}',
											'{$propostaSubacao['ppsobra']}',
											'{$propostaSubacao['ppsestrategiaimplementacao']}',
											'{$propostaSubacao['ppsptres']}',
											'{$propostaSubacao['ppsnaturezadespesa']}',
											'{$propostaSubacao['ppsmonitora']}',
											{$docid},
											" . ($propostaSubacao ['frmid'] ? $propostaSubacao ['frmid'] : 'null') . ",
											" . ($propostaSubacao ['indid'] ? $propostaSubacao ['indid'] : 'null') . ",
											" . ($propostaSubacao ['foaid'] ? $propostaSubacao ['foaid'] : 'null') . ",
											" . ($propostaSubacao ['undid'] ? $propostaSubacao ['undid'] : 'null') . ",
											$ppsid,
											{$aciid},
											" . ($propostaSubacao ['ptsid'] ? $propostaSubacao ['ptsid'] : 'null') . ",
											'{$propostaSubacao['ppscronograma']}',
											'{$_SESSION['usucpf']}',
											'S'
											) returning sbaid";
							$sbaid = $db->pegaUm ( $sql );
							
							$sql = "INSERT INTO par.subacaodetalhe(sbaid, sbdano, ssuid)
									VALUES ($sbaid, '" . $ano . "', $ssuid) returning sbdid";
 							$sbdid = $db->pegaUm ( $sql );
							
							//sepvalor inserido como 0 de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor
							$valor = 0; 
							$sql = "INSERT INTO par.subacaoemendapta(sbdid, emdid, ptrid, sepvalor)
									VALUES ($sbdid, $emendas, $pta, $valor)";
							$db->executar ( $sql );
						}
					}
				}
			}
		}
		$db->commit ();
	}
	$db->sucesso ( 'principal/propostaEmendaSubacao', '&aba=subacao&ano='.$ano.'&sbaid='.$sbaid );
	exit ();
}
function pegaCnpj($inuid) {
	global $db;
	
	return $db->pegaUm ( "SELECT iue.iuecnpj
	                     FROM par.instrumentounidade iu
	                     inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                     WHERE
	                     	iu.inuid = {$inuid}
	                     	and iue.iuestatus = 'A'
	                        and iue.iuedefault = true" );
}

$sql = "SELECT
			vede.emecod,
			enb.enbnome,
			enb.enbid,
			(CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda,
			a.autnome,
			vfun.fupfuncionalprogramatica,
			vfun.fupdsc,
			sum(vede.edevalor) as valor,
			vede.emdid,
			vede.emedescentraliza,
			ptr.ptrid,
			ptr.ptrvalorproponente
		FROM
			emenda.v_emendadetalheentidade vede
		inner join emenda.autor a on a.autid = vede.autid
		left  join emenda.v_funcionalprogramatica vfun on vfun.acaid = vede.acaid and vfun.prgano = '$ano' and vfun.acastatus = 'A'
		inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
		inner join emenda.ptemendadetalheentidade pte on vede.edeid = pte.edeid
		inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid
		WHERE
			vede.edestatus = 'A'
			and vede.ededisponivelpta = 'S'
			and vede.emeano = '$ano'
			and enb.enbcnpj = '" . pegaCnpj ( $inuid ) . "'
			and ptr.ptrstatus = 'A'
			and ptr.ptrexercicio = '$ano'
			and ptr.sisid = 23
			and vede.emetipo = 'E'
		GROUP BY
			vede.emecod,
			vede.emetipo,
			a.autnome,
			vfun.fupfuncionalprogramatica,
			vfun.fupdsc,
			vede.emdid,
			vede.emedescentraliza,
			enb.enbnome,
			enb.enbid,
			vede.edeid,
			ptr.ptrid,
			ptr.ptrvalorproponente";
//ver($sql);
$arEmenda = $db->carregar ( $sql );
$arEmenda = $arEmenda ? $arEmenda : array ();

$entidadePar = ($_SESSION ['par'] ['itrid'] == 1) ? $_SESSION ['par'] ['estuf'] : $_SESSION ['par'] ['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar ( $entidadePar, $_SESSION ['par'] ['itrid'] );
?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
<title>PAR 2010 - Plano de Metas - Suba��o</title>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
</head>
<body>
	<?
	$abaAtiva = 'par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=' . $_REQUEST ['ano'];
	
	/* Array com os itens da aba de identifica��o */
	$menu = array (
			0 => array (
					"id" => 0,
					"descricao" => "2013",
					"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=2013" 
			),
			1 => array (
					"id" => 1,
					"descricao" => "2014",
					"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=2014" 
			), 
			2 => array (
					"id" => 2,
					"descricao" => "2015",
					"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=2015" 
			) 
	);
	echo montarAbasArray ( $menu, $abaAtiva );
	
	monta_titulo ( $nmEntidadePar, "Emendas Propostas" );
	
	echo '<br>';
	
	if ($_REQUEST ['aba'] == 'emenda') {
		$abaAtivaGeral = 'par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=' . $_REQUEST ['ano'];
	} elseif ($_REQUEST ['aba'] == 'subacao') {
		$abaAtivaGeral = 'par.php?modulo=principal/propostaEmendaSubacao&acao=A&aba=subacao&ano=' . $_REQUEST ['ano'];
	} elseif ($_REQUEST ['aba'] == 'orcamento') {
		$abaAtivaGeral = 'par.php?modulo=principal/orcamentoImpositivoPar&acao=A&aba=orcamento&ano=' . $_REQUEST ['ano'];
	}
	$boImpositivo = verificaEmendaImpositiva($inuid, $_REQUEST ['ano']);
	/* Array com os itens da aba de identifica��o */
	if( $boImpositivo < 0 ){
		$menuGeral = array (
				0 => array (
						"id" => 0,
						"descricao" => "Proposta Emenda",
						"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=$ano"
				),
				1 => array (
						"id" => 1,
						"descricao" => "Proposta Suba��o",
						"link" => "par.php?modulo=principal/propostaEmendaSubacao&acao=A&aba=subacao&ano=$ano"
				) 		
		);	
	} else {
		$menuGeral = array (
				0 => array (
						"id" => 0,
						"descricao" => "Proposta Emenda",
						"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=$ano"
				),
				1 => array (
						"id" => 1,
						"descricao" => "Proposta Suba��o",
						"link" => "par.php?modulo=principal/propostaEmendaSubacao&acao=A&aba=subacao&ano=$ano"
				),
				2 => array (
						"id" => 2,
						"descricao" => "Or�amento Impositivo",
						"link" => "par.php?modulo=principal/orcamentoImpositivoPar&acao=A&aba=orcamento&ano=$ano" 
				) 		
		);
	}
	echo montarAbasArray ( $menuGeral, $abaAtivaGeral );
	
	// $db->cria_aba( $abacod_tela, $url, '' );
	?>
	<form method="post" name="formulario" id="formulario" action="">
		<input type="hidden" name="requisicao" id="requisicao" value=""> 
		<input type="hidden" name="tipoAcao" id="tipoAcao" value=""> 
		<input type="hidden" name="excluiremenda" id="excluiremenda" value="">
	<?if($arEmenda){ ?>
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Recursos Dispon�veis</b></td>
			</tr>
			<tr>
				<td>
					<table id="tblform" class="listagem" width="100%" bgcolor="#f5f5f5"
						cellSpacing="1" cellPadding="3" align="center">
						<thead>
							<tr>
								<td align="Center" class="title" width="08%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>C�digo</strong></td>
								<td align="Center" class="title" width="05%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
								<td align="Center" class="title" width="20%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Autor</strong></td>
								<td align="Center" class="title" width="10%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Funcional Program�tica</strong></td>
								<td align="Center" class="title" width="50%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>SubT�tulo</strong></td>
								<td align="Center" class="title" width="10%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Valor</strong></td>
							</tr>
						</thead>
		<?
		
		foreach ( $arEmenda as $key => $valor ) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			$emedescentraliza = $valor ['emedescentraliza'];
			
			$sql = "select
						count(v.emdid)
					from 
						emenda.ptemendadetalheentidade pt
					    inner join emenda.v_emendadetalheentidade v on v.edeid = pt.edeid
					where
						pt.ptrid = {$valor['ptrid']}
						and v.emdid = {$valor['emdid']}
						and v.edestatus = 'A'
					    and v.emdimpositiva = '6'";
			$boImpositivo = $db->pegaUm($sql);
			
			if( $boImpositivo > 0 ){
				
				if( $_SESSION['exercicio'] > 2014 )	$filtroImp = ' and edi.edeid = ve.edeid';
			
				#Valor impositivo de impedimento da emenda
				$valorImpositivo = $db->pegaUm("select
														coalesce(sum(edi.edivalor), 0) as valor
													from 
														emenda.ptemendadetalheentidade pte
														inner join emenda.v_emendadetalheentidade ve on ve.edeid = pte.edeid
													    inner join emenda.emendadetalheimpositivo edi on edi.emdid = ve.emdid and edi.edistatus = 'A' $filtroImp
													where
														pte.ptrid = {$valor['ptrid']}
														and ve.emdid = {$valor['emdid']}
													    and ve.edestatus = 'A'");
				
				$pedvalor = $db->pegaUm("select sum(pedvalor) from emenda.ptemendadetalheentidade pt
												inner join emenda.v_emendadetalheentidade ve on ve.edeid = pt.edeid  where pt.ptrid = {$valor['ptrid']} and ve.emdid = {$valor['emdid']} and ve.edestatus = 'A'");
				$ptrvalorconcedente = $pedvalor + $val['ptrvalorproponente'];
				$valor['valor'] = ($ptrvalorconcedente - $valorImpositivo);
			}
			
			$valorMax = '<input type="hidden" value="' . $valor ['valor'] . '" name="valorMax[' . $valor ['emdid'] . '][' . $valor ['ptrid'] . ']" id="valorMax_' . $valor ['emdid'] . '_' . $valor ['ptrid'] . '" />';
			$html .= '
				  <tr bgcolor="' . $cor . '" id="tr_' . $key . '" onmouseout="this.bgColor=\'' . $cor . '\';" onmouseover="this.bgColor=\'#ffffcc\';">
				  	<td><img src="/imagens/mais.gif" style="cursor:pointer" border="0" id="img_dimensao_' . $valor ['emdid'] . '" onClick="carregaEmendaDetalhePTA(this.id, ' . $valor ['emdid'] . ', ' . $valor ['ptrid'] . ');"> ' . $valor ['emecod'] . '</td>
				  	<td>' . $valor ['tipoemenda'] . $valorMax . '</td>
				  	<td>' . $valor ['autnome'] . '</td>
				  	<td style="text-align: center;">' . $valor ['fupfuncionalprogramatica'] . '</td>
				  	<td>' . $valor ['fupdsc'] . '</td>
					<td style="text-align: right;">R$ ' . number_format ( $valor ['valor'], 2, ',', '.' ) . '</td>
				  </tr>
				<tr id="trV_' . $valor ['emdid'] . $valor ['ptrid'] . '" style="display: none">
			  	   <td colspan="6" id="listaEmendaDetalhe_' . $valor ['emdid'] . $valor ['ptrid'] . '"></td>
			  	</tr>';
		}
		$html .= '<tr bgcolor="#dedfde">
						<td colspan="7" align="center" valign="bottom">';
		
		if ($boolAceitar) {
			if ($ano == date('Y')) {
				$html .= '<input type="button" name="btnSalvar" id="btnSalvar" value="Aceitar" onclick="salvarPropostaEmenda(\'N\');">&nbsp;';
				if (in_array ( PAR_PERFIL_ADMINISTRADOR, $arrayPerfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $arrayPerfil ) || in_array ( PAR_PERFIL_EQUIPE_FINANCEIRA, $arrayPerfil ) || in_array ( PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil )) 

				{
					$html .= '<input type="button" name="btnSalvar" id="btnSalvar" value="Aceitar Condicional" onclick="salvarPropostaEmenda(\'C\');">&nbsp;';
				}
			}
		}
		$html .= '<input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="fecharPopUp();">
							</td>
					</tr>';
		$html .= '</table>
		</td>
		</tr>
		</table>';
	} else {
		$html .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2">
						<tr><td>
							<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
								<tr>
									<td align="center" style="color:#cc0000;">N�o foram encontrados registros.</td>
								</tr>
							</table>
						</td></tr>
					</table>';
	}
	
	echo $html;
	?>
		
	
	
	
	</form>
	<br>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1"
		cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Emendas Vinculados as Suba��es do PAR</b></td>
		</tr>
		<tr>
			<td colspan="2"><?
			
			
			if( in_array($_SESSION['usucpf'], array('', '', '')) ){
				$imgExcluir = "<img src=\"/imagens/excluir.gif\" title=\"Subacao\" id=\"'||sp.sepid||'\" class=\"desvincularSubacao\">&nbsp;";
			}
			$img = "'$imgExcluir<img src=\"/imagens/alterar.gif\" title=\"Subacao\" onclick=\"w = window.open(\'par.php?modulo=principal/subacao&acao=A&sbaid='||s.sbaid||'&anoatual='||sd.sbdano||'\',\'Subacao\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=600\'); w.focus();\">&nbsp;'";
			//
			$sql = "SELECT distinct
								'<span style=\"color:#0066cc;\">'||vede.emecod||'</span>' as emecod,
								$img  ||
								d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || sbaordem as codigo,
								s.sbadsc
								--  retirar o sepvalor Demanda 2201 PAR que diz para retirar o campo de valor
								--,sp.sepvalor
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
							    and enb.enbcnpj = '" . pegaCnpj ( $_SESSION ['par'] ['inuid'] ) . "'
							    and pont.inuid = {$_SESSION['par']['inuid']}
							    and ptr.ptrstatus = 'A'
							    and ptr.ptrexercicio = '$ano'
							    and ptr.sisid = 23
							    and vede.emetipo = 'E'
							order by 1";
			/*
				retirar o sepvalor Demanda 2201 PAR que diz para retirar o campo de valor
				$cabecalho = array (
					"Localizador",
					"Descri��o",
					"Valor"
				);
			*/ 
			$cabecalho = array (
					"Localizador",
					"Descri��o"
			);
			
			echo $db->monta_lista_grupo ( $sql, $cabecalho, 20, 5, '', '', '', '', 'emecod', '', true );
			?></td>
		</tr>
	</table>
</body>
<script>

	function number_format( number, decimals, dec_point, thousands_sep ) {

	    var n = number, prec = decimals;
	    n = !isFinite(+n) ? 0 : +n;
	    prec = !isFinite(+prec) ? 0 : Math.abs(prec);
	    var sep = (typeof thousands_sep == "undefined") ? ',' : thousands_sep;
	    var dec = (typeof dec_point == "undefined") ? '.' : dec_point;

	    var s = (prec > 0) ? n.toFixed(prec) : Math.round(n).toFixed(prec); //fix for IE parseFloat(0.55).toFixed(0) = 0;

	    var abs = Math.abs(n).toFixed(prec);
	    var _, i;

	    if (abs >= 1000) {
	        _ = abs.split(/\D/);
	        i = _[0].length % 3 || 3;

	        _[0] = s.slice(0,i + (n < 0)) +
	              _[0].slice(i).replace(/(\d{3})/g, sep+'$1');

	        s = _.join(dec);
	    } else {
	        s = s.replace('.', dec);
	    }

	    return s;
	}

	$('.listagem').css('width', '100%');

	function verificaCHK(){
		var excluiremenda = '';
		$('.chk').each(function(){
			if( $(this).attr('checked') == false ){
				if($(this).val() != ''){
					if( excluiremenda == '' ){
						excluiremenda = $(this).val();

					} else {
						excluiremenda = excluiremenda+','+$(this).val();
					}
				}
			}
		});

		$('#excluiremenda').val( excluiremenda );
	}

	$(document).ready(function(){

		$('.desvincularSubacao').live('click',function(){
			var sepid = $(this).attr('id');
			if( confirm('Deseja realmente excluir a vincula��o?') ){
				$.ajax({
					type: "POST",
					url: 'par.php?modulo=principal/propostaEmendaSubacao&acao=A&requisicao=desvincular&sepid='+sepid,
					async: false,
					success: function(retornoajax) {
						//$('#debug').html(retornoajax);
						if( retornoajax == 1 ){
							alert('Opera��o Realizada com Sucesso.');
							window.location.href = window.location;
						}
					}
				});
			}
			
		});
			
		$('.chk').live('click',function(){
			var id = $(this).attr('id').replace('chk','');
			var arrVariaveis = id.split('_');
			var emdid = arrVariaveis[1];
			var ptrid = arrVariaveis[2];
			var ppsid = arrVariaveis[3];
			var valorMax = $('#valorMax_'+emdid+'_'+ptrid).val();
			var qtdSub = $('[id*="sepvalor_'+emdid+'_'+ptrid+'"]').length;

			if( $(this).attr('checked') ){
				// de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor sepvalor
				/*if( qtdSub < 2 ){
					$('#sepvalor'+id).val(number_format(parseFloat(valorMax), 2, ',', '.'));
				}*/
				$('#divValor'+id).show();
			}else{
				$('#divValor'+id).hide();
				//$('#sepvalor'+id).val('0,00');
			}
		});
		// de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor sepvalor
		/*
		$('[id*="sepvalor"]').live('blur',function(){

			var valor = 0;
			valor = replaceAll($(this).val(),'.','');
			valor = valor.replace(',','.');
			if( isNaN(valor) || valor == '' ){
				$(this).val('0,00');
				return false;
			}

			var arrVariaveis = $(this).attr('id').split('_');
			var emdid = arrVariaveis[1];
			var ptrid = arrVariaveis[2];
			var ppsid = arrVariaveis[3];
				valor = 0;
			var valorSoma = 0;
			var valorMax = $('#valorMax_'+emdid+'_'+ptrid).val();

			$('[id*="sepvalor_'+emdid+'_'+ptrid+'"]').each(function(){
				valor = replaceAll($(this).val(),'.','');
				valor = valor.replace(',','.');
				valorSoma = parseFloat(valorSoma)+parseFloat(valor);
			});

			valor = replaceAll($(this).val(),'.','');
			valor = valor.replace(',','.');

			if( valorSoma > valorMax ){
				valor = parseFloat(valorMax) - ( parseFloat( valorSoma ) - parseFloat( valor ) );
				alert('A soma dos valores propostos para as suba��es ultrapassa o valor da emenda associada.');
				$(this).val(number_format(parseFloat(valor), 2, ',', '.'));
			}
		});
		*/
	});

	function salvarPropostaEmenda(tipo){

		if( $('[type="checkbox"]:checked').length ){

			var emdid = 0;
			var ptrid = 0;
			var valor = 0;
			var erro = false;
			var chk = false;
			var nome = '';

			$('[name="emdid[]"]').each(function(){
				emdid = $(this).val();
				ptrid = $('[name="ptrid['+emdid+']"]').val();
				// de acordo com a Demanda 2201 PAR que diz para retirar o campo de valor sepvalor
				/*
				$('[name*="sepvalor['+emdid+']['+ptrid+']"]').each(function(){
					valor = replaceAll($(this).val(),'.','');
					valor = parseFloat(valor.replace(',','.'));
					nome = $(this).attr('id');
					nome = nome.replace('sepvalor','chk')
					chk = $('[id*="'+nome+'"]').attr('checked');
					if( valor <= 0 && chk ){
						alert('O valor da suba��oe escolhida deve ser maior que 0,00.');
						erro = true;
						return false;
					}
				});
				*/
				if( erro ){
					return false;
				}
			});

			if( erro ){
				return false;
			}

			$('#tipoAcao').val(tipo);
			$('#requisicao').val('salvar');
			$('#formulario').submit();
		} else {
			alert('Selecione pelo menos uma proposta!');
			return false;
		}
	}

	function fecharPopUp(){
		window.opener.location.href = window.opener.location;
		window.close();
	}

	function carregaEmendaDetalhePTA(idImg, emdid, ptrid){
		var img 	 = $('#'+idImg );
		var tr_nome = 'trV_'+ emdid + ptrid;
		var td_nome  = 'listaEmendaDetalhe_'+ emdid + ptrid;

		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ''){
			$('#'+td_nome).html('Carregando...');
			$('#'+idImg ).attr('src', '../imagens/menos.gif');
			carregaEmendaDetalhe(emdid, ptrid, td_nome);
		}if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
			$('#'+tr_nome).css('display', '');
			$('#'+idImg ).attr('src', '../imagens/menos.gif');
		} else {
			$('#'+tr_nome).css('display', 'none');
			$('#'+idImg ).attr('src', '../imagens/mais.gif');
		}
	}

	function carregaEmendaDetalhe(emdid, ptrid, td_nome){

		$.ajax({
				type: "POST",
				url: 'par.php?modulo=principal/propostaEmendaSubacao&acao=A&carregaEmendaDetalhePTA=true&emdid='+emdid+'&ptrid='+ptrid+'&ano=<?=$ano ?>',
				async: false,
				success: function(retornoajax) {
					$('#'+td_nome).html(retornoajax);
				}
			});
	}
</script>
<?
function carregaSubacoesVinculadas( $ano ) {
	global $db;
	
	$sql = "SELECT
				vede.emecod,
				enb.enbnome,
				enb.enbid,
				(CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda,
				a.autnome,
				vfun.fupfuncionalprogramatica,
				vfun.fupdsc,
				sum(vede.edevalor) as valor,
				vede.emdid,
				vede.emedescentraliza,
				ptr.ptrid
			FROM
				emenda.v_emendadetalheentidade vede
				inner join emenda.autor a on a.autid = vede.autid
				left join emenda.v_funcionalprogramatica vfun on vfun.acaid = vede.acaid and vfun.prgano = '$ano' and vfun.acastatus = 'A'
				inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
				inner join emenda.ptemendadetalheentidade pte on vede.edeid = pte.edeid
				inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid
	            inner join par.subacaoemendapta sep on sep.ptrid = ptr.ptrid and vede.emdid = sep.emdid and sep.sepstatus = 'A'
			WHERE
				vede.edestatus = 'A'
				and vede.ededisponivelpta = 'S'
				and vede.emeano = '".$ano."'
				and enb.enbcnpj = '" . pegaCnpj ( $_SESSION ['par'] ['inuid'] ) . "'
				and ptr.ptrstatus = 'A'
				and ptr.ptrexercicio = '" . (( int ) $ano) . "'
				and ptr.sisid = 23
			GROUP BY
				vede.emecod,
				vede.emetipo,
				a.autnome,
				vfun.fupfuncionalprogramatica,
				vfun.fupdsc,
				vede.emdid,
				vede.emedescentraliza,
				enb.enbnome,
				enb.enbid,
				vede.edeid,
				ptr.ptrid";
	
	$arEmenda = $db->carregar ( $sql );
	$arEmenda = $arEmenda ? $arEmenda : array ();
	
	return $arEmenda;
}
?>
<div id="debug"></div>