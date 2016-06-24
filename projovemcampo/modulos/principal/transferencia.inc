<?php
if( $_POST['arqid'] != '' ){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_POST['arqid']);
    echo"<script>window.location.href = window.location.href;</script>";
    exit;
} 

header('Content-Type: text/html; charset=cp1252');

function testaidade($estid,$aprid){
	
	global $db;
	$aprid = trim($aprid);
	if($aprid!=3 && !empty($aprid)){
	$sql = "SELECT
                apranoreferencia 
            FROM
                projovemcampo.anoprograma
            WHERE
                --apcstatus = 'A'
			--AND 
				aprid = $aprid
            ORDER BY
                apranoreferencia";
	$anodestino = $db->pegaUm($sql);
	$sql = "SELECT
				estcpf as cpf,
				estnome as nome
			FROM
				projovemcampo.estudante
			WHERE
				estid = $estid
			AND 	(($anodestino - extract(year from estdatanascimento)<18) OR ($anodestino - extract(year from estdatanascimento)>29))";
	$dadosaluno = $db->pegaLinha($sql);
	}else{
		$anodestino1 = 2014;
		$anodestino2 = 2015;
		$sql = "SELECT
					estcpf as cpf,
					estnome as nome
				FROM
					projovemcampo.estudante
				WHERE
					estid = $estid
				AND 	(($anodestino1 - extract(year from estdatanascimento)<18) OR ($anodestino1 - extract(year from estdatanascimento)>29))";
		$dadosaluno = $db->pegaLinha($sql);
		if($dadosaluno){
			$sql = "SELECT
						estcpf as cpf,
					estnome as nome
					FROM
						projovemcampo.estudante
					WHERE
						estid = $estid
					AND 	(($anodestino2 - extract(year from estdatanascimento)<18) OR ($anodestino2 - extract(year from estdatanascimento)>29))";
			$dadosaluno = $db->pegaLinha($sql);
		}
	}
	
	return $dadosaluno;
}
function anoDestino(){
    
     global $db;
    
    $sql = "SELECT 
                aprid as codigo,
                apranoreferencia as descricao
            FROM 
                projovemcampo.anoprograma
            ORDER BY
                apranoreferencia";
    $aprid = $_REQUEST['aprid'];
//    ver($_POST['modalidade']);
    if($_POST['modalidade'] == 'M'){
    $db->monta_combo('aprid', $sql, 'S', 'Selecione o Ano', '', '', '', '', 'S', 'aprid');
    }else{
     $db->monta_combo('aprid_outros', $sql, 'S', 'Selecione o Ano', '', '', '', '', 'S', 'aprid_outros');
    }
}

function buscarComboAlunos(){

    global $db;
    
    $where = Array('est.estid NOT IN ( SELECT estid FROM projovemcampo.transferencia tra
                                        --INNER JOIN projovemcampo.historico_transferencia htr ON htr.htrid = tra.htrid_ultimo
                                        --WHERE htr.shtid_status IN (1,2) 
	    							)');
    
    if( $_POST['turid'] != '' && $_POST['turid'] != 'undefined' ){
        $where[] = "tur.turid = {$_POST['turid']}";
    }

    $sqlSelecionaVarios = "
							SELECT DISTINCT
								est.estid as codigo,
								removeacento(est.estcpf||' - '||
 								est.estnome || ': ' ||
                               'Escola '|| ent.entnome || ' Turma: '||
                               turdescricao) as descricao
                           	FROM 
                               projovemcampo.estudante est
							INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
							INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
                           WHERE
                               ".implode(" AND ", $where).(count($where)<2?'LIMIT 100':'');
    mostrarComboPopup( 'Aluno(s):', 'estid_combo',  $sqlSelecionaVarios, '', 'Selecione a(s) Situação(ões)' );
}

function enviar_aluno_salvar(){
	
	global $db;
    extract($_POST);
   
    if(empty($estid_combo)&&empty($estid)){
    	echo "
	          <script>
	            alert('Você não selecionou nenhum aluno.');
	             window.location.href = window.location.href;
	          </script>
	             ";
    	die();
    }
    if( is_array($estid_combo) ){
    	$arrestid = $estid_combo;
    	$turidorigem = $turidm;
    }else{
    	$arrestid = Array($estid);
    }
    
    $sqlPeridOrigem =	"SELECT DISTINCT
	    					MAX(perid)
	    				FROM
	    					projovemcampo.diario 
	    				WHERE
	    					turid =  {$turidorigem}";
    $PeridOrigem = $db->pegaUm($sqlPeridOrigem);
    
    if($PeridOrigem!=''){
    	$periodotransferido = ",periodotransferido";
    	$periodovalor = ",{$PeridOrigem}";
    }
    if($_SESSION['projovemcampo']['modalidade']=='M'){
    	$localdestino1 = 'turid_destino';
    	$localdestino2 = $turiddestino;
    }else{
    	$localdestino1 = 'entid_destino';
    	$localdestino2 = $entiddestino;
    }
    foreach($arrestid as $estid){
	    $idadeforapadaro = testaidade($estid,$aprid);
	    if(empty($idadeforapadaro)){
    	
		    $sqlinsert ="  INSERT INTO projovemcampo.transferencia(
						            turid_origem,
						            $localdestino1, 
						            estid, 
						            cpfresponsavelorigem, 
						            numordemtransferencia
						            $periodotransferido)
						    VALUES (
						    		{$turidorigem},
						    		{$localdestino2},
						    		{$estid},
						    		'{$_SESSION['usucpf']}',
						    		(SELECT count(traid) FROM projovemcampo.transferencia WHERE estid = $estid)+1
						    		$periodovalor)
						    RETURNING
								traid;
						    ";
			$traid  = $db->pegaUm($sqlinsert);
			
			$htrid = atualiza_historico_transferencia($traid,1,$justificativa);
			
			$sqlhistorico .= 	"UPDATE projovemcampo.transferencia SET
									htrid_ultimo = $htrid
								WHERE
									traid = $traid;";
		    if( $arqid != '' ){
		    	$sqlarquivo.= "INSERT INTO projovemurbano.arquivo_tranferencia(arqid, traid)
		    	VALUES($arqid, $traid);";
		    }else{
		    	if($_FILES["arquivo"]['name']!=''){
		    		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		    
		    		$campos = array("traid" => $traid);
		    
		    		$file = new FilesSimec("arquivo_tranferencia", $campos, 'projovemcampo');
		    
		    		$file->setUpload("Documento de Pedido de Transferência CPF: $estcpf");
		    
		    		$sql = "SELECT arqid FROM projovemcampo.arquivo_tranferencia WHERE traid = $traid";
		    		$arqid = $db->pegaUm($sql);
		    	}
		    	
		    }
		    
	    }else{
	    	$dadosalunoidade .= '\n'.'CPF:'.$idadeforapadaro['cpf'].'-NOME:'.$idadeforapadaro['nome'];
	    }
    }
	$db->executar($sqlhistorico);
	
    $db->commit();
    
    echo "
		<script>
			alert('O aluno foi enviado.');
			window.location.href = window.location.href;
		</script>";
}

function enviar_aluno_transferir(){
    
    global $db;
    
    extract($_POST);
    
    if($traid[0]!=''){
    	
    	foreach($traid as $idtransferencia){
    		
    		if($_SESSION['projovemcampo']['modalidade']=='M'){
    			
	    			$sqlDadosTrans = 	"SELECT DISTINCT
	    									turid_origem,
											turid_destino,
											estid,
											periodotransferido
										FROM	
											projovemcampo.transferencia
										WHERE
											traid = {$idtransferencia}";
	    			$DadosTrans = $db->pegaLinha($sqlDadosTrans);
	    			
					if($DadosTrans['turid_origem']){
					
		    			$sqlPeriodoscursados =	"SELECT DISTINCT
		    										perid
		    									FROM
		    										projovemcampo.diario 
		    									WHERE
		    										turid = {$DadosTrans['turid_origem']}
		    										";
						$Periodoscursados = $db->pegaLinha($sqlPeriodoscursados);
						
						if($Periodoscursados){
							
							$sqlperiododestino = 	"SELECT
														max(perid)
													FROM 
														projovemcampo.diario
													WHERE 
														turid = {$DadosTrans['turid_destino']}";
									// ver($sqlperiododestino,d);
							$periododestino = $db->pegaUm ( $sqlperiododestino );
						
							$inativaFrequencias .= 	"UPDATE projovemcampo.lancamentodiario
													SET
														status = 'I'
													WHERE
														Estid = {$DadosTrans['estid']}
													AND diaid in(SELECT DISTINCT
																	diaid
																FROM
																	projovemcampo.diario
																WHERE
																	turid = {$DadosTrans['turid_origem']});";
							$sqltestaturma ="SELECT DISTINCT
												lnd.lndid
											FROM
												projovemcampo.lancamentodiario lnd
											INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
											WHERE
												dia.turid = {$DadosTrans['turid_destino']}
											AND lnd.estid = {$DadosTrans['estid']}
											AND lnd.lndstatus = 'I'
											ORDER BY
												lndid";
							$testaturma = $db->pegarColunas( $sqltestaturma );
						
							if (is_array ( $testaturma ) && ! empty ( $testaturma )) {
								
								$sqlfrequenciavolta.= 	"UPDATE projovemcampo.lancamentodiario  SET
															frqstatus = 'A'
														WHERE
															estid = {$DadosTrans['estid']}
														AND lndstatus = 'I'
														AND lndid in ($testaturma);";
								
								
								$sqlVerificaPeriodoVolta ="SELECT
																dia.perid
															FROM 
																projovemcampo.diario dia
															INNER JOIN projovemcampo.lancamentodiario lnd ON lnd.diaid = dia.diaid
															WHERE 
																	lnd.estid = {$DadosTrans['estid']}
															AND lndstatus = 'I'
															AND lndid in ($testaturma)";
								
								$VerificaPeriodoVolta = $db->carregarColuna($sqlVerificaPeriodoVolta);
								
								$sqlinserefrequencianova.= "
														INSERT INTO projovemcampo.lancamentodiario(estid, diaid, lndhorasescola, lndhorascomunidade)
															SELECT
																estid,
																(
																	SELECT DISTINCT
																		diaid
																	FROM
																		projovemcampo.diario dia2
																	WHERE
																		dia2.diaid in(
																						SELECT
																							diaid
																						FROM
																							projovemcampo.diario
																						WHERE
																							turid = {$DadosTrans['turid_destino']}
																						AND perid NOT IN ($VerificaPeriodoVolta)
																					)
																	AND dia2.perid = dia.perid
																) as diaid,
																lndhorasescola,
																lndhorascomunidade
															FROM
																projovemcampo.lancamentodiario lnd
															INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
															INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
															WHERE
																estid = {$DadosTrans['estid']}
															AND turid = {$dadostrasns['turid_origem']}
															AND lndstatus = 'I'
															AND perid NOT IN ($VerificaPeriodoVolta)
															ORDER BY
															3
														;";
							} else {
								if(!$periododestino){
									$sqlinserefrequencianova.= "
													INSERT INTO projovemcampo.lancamentodiario(estid, diaid, lndhorasescola, lndhorascomunidade)
														SELECT
															estid,
															(
																SELECT DISTINCT
																	diaid
																FROM
																	projovemcampo.diario dia2
																WHERE
																	dia2.diaid in(
																				SELECT
																					diaid
																				FROM
																					projovemcampo.diario
																				WHERE
																					turid = {$DadosTrans['turid_destino']}
																				AND perid BETWEEN 1
																				AND(SELECT perid FROM projovemcampo.periodo WHERE perid = ($periododestino - 1))
																				)
																AND dia2.perid = dia.perid
															) as diaid,
															lndhorasescola,
															lndhorascomunidade
														FROM
															projovemcampo.lancamentodiario lnd
														INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
														INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
														WHERE
																estid = {$DadosTrans['estid']}
																AND turid = {$dadostrasns['turid_origem']}
																AND lndstatus = 'I'
													AND dia.perid BETWEEN (SELECT perid FROM projovemcampo.periodo WHERE  perid = 1)
													AND($periododestino - 1)
													ORDER BY
													3
													;";
												// ver($sql12,d);
							}
						}
					}
					
	    			$sqlTransEstudante .= "UPDATE projovemcampo.estudante
										   	SET 
	    										turid = {$DadosTrans['turid_destino']}
										 WHERE 
										 		estid = {$DadosTrans['estid']};";
	    			
	    			$justificativa = "Transferência efetuada!";
	    			
	    			$htrid = atualiza_historico_transferencia($idtransferencia,3,$justificativa);
	    			
	    			$sqlhistorico .= 	"UPDATE projovemcampo.transferencia SET
						    				htrid_ultimo = $htrid
						    			WHERE
						    				traid = $idtransferencia;";
					}
    			}else{
    				$sqlDadosTrans = 	"SELECT DISTINCT
					    					estid,
					    					entid_destino
					    				FROM
					    					projovemcampo.transferencia
					    				WHERE
					    					traid = {$idtransferencia}";
    				$DadosTrans = $db->pegaLinha($sqlDadosTrans);
    				
    				$justificativa = "Pedido de Transfrência Enviado!";
    				
    				$htrid = atualiza_historico_transferencia($idtransferencia,2,$justificativa);
	    			
	    			$sqlhistorico .= 	"UPDATE projovemcampo.transferencia SET
						    				htrid_ultimo = $htrid
						    			WHERE
						    				traid = $idtransferencia;";
	    			
					$sql = "SELECT DISTINCT
    							usu.usuemail
	    					FROM
	    						projovemcampo.secretaria 		seca
	    					INNER JOIN seguranca.usuario        usu ON usu.usucpf = seca.secoordcpf
	    					INNER JOIN projovemcampo.turma 		tur ON tur.secaid = seca.secaid
	    					WHERE
	    						tur.entid = '{$DadosTrans['entid_destino']}'";
    				//                ver($sql,d);
    				$email_resp_destino = $db->pegaUm($sql);
    				
    				$sql = "SELECT estcpf||' - '||estnome FROM projovemcampo.estudante WHERE estid = {$DadosTrans['estid']}";
    				
    				$alunos = $db->carregarColuna($sql);
    				
    				$assunto = "PROJOVEM CAMPO - Pedido de transferência sob sua reponsabilidade.";
    				
    				$conteudo = "Foi feito pedido de tranferência do seguinte aluno para escola de sua reponsabilidade:
                                ".implode('; <br>',$alunos).".";
//     				enviar_email( $_SESSION['email_sistema'], $email_resp_destino, $assunto, $conteudo );
    		}
    	}
    	if($sqlhistorico){
    		$db->executar($sqlhistorico);
    	}
    	if($sqlTransEstudante){
    		$db->executar($sqlTransEstudante);
    	}
    	if($sqlinserefrequencianova){
    		$db->executar($sqlinserefrequencianova);
    	}
    	if($sqlfrequenciavolta){
    		$db->executar($sqlfrequenciavolta);
    	}
    	if($sqlfrequenciavolta){
    		$db->executar($sqlfrequenciavolta);
    	}
    		
    	$db->commit();
    	echo "
			<script>
// 				alert('Tranferência Concluida.');
				window.location.href = window.location.href;
			</script>";
    	 
    }
}

function enviar_aluno_cancelarTranferencia(){

	global $db;

	extract($_POST);
	 
	if( $traid != '' ){

		$sql = "UPDATE projovemcampo.transferencia SET
					cpfresponsaveldestino = '{$_SESSION['usucpf']}'
				WHERE
					traid = $traid";
					
					
		$htrid = atualiza_historico_transferencia($traid,5,$justificativa);
				 
		$sqlhistorico .= 	"UPDATE projovemcampo.transferencia SET
								htrid_ultimo = $htrid
							WHERE
								traid = $traid";
	    if($sqlhistorico){
	    	$db->executar($sqlhistorico);
		}
		
		$db->commit();
		echo "
			<script>
				alert('Tranferência cancelada.');
				window.location.href = window.location.href;
			</script>";
	}
}
function atualiza_historico_transferencia($traid,$shtid,$justificativa){
	global $db;
	
	if(!$justificativa){
		$justificativa = 'Não Justificou';
	}
	$sql = "INSERT INTO projovemcampo.historico_transferencia(
				traid,
				shtid_status,
				usucpf_fezacao,
				htrdatahoraacao,
				justificativa)
			VALUES (
				$traid, 
				$shtid,
				'{$_SESSION['usucpf']}',
				now(),
				'{$justificativa}')
			RETURNING
				htrid";
	$htrid  = $db->pegaUm($sql);
	 
	return $htrid;
	
}
function receber_aluno_tranferir(){

    global $db;
    extract($_POST);
    $meta = pegameta();
	
    $sqlQtdAlunos = "SELECT DISTINCT
    					count(estid)
    				FROM
    					projovemcampo.estudante est
    				INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
    				INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
    				WHERE
    					apc.apcid = {$_SESSION['projovemcampo']['apcid']}
    				AND est.eststatus = 'A'";
    $QtdAlunos = $db->pegaUm($sqlQtdAlunos);
    
    if( $meta <= $QtdAlunos){
    	die("<script>
    			alert('Meta atingida.');
    			window.location.href = window.location.href;
			</script>");
    }
    
    if(is_array($turid) ){
    	$x=0;
    	$turidAnteiror ='';
		foreach( $turid as $idturma ){
    		
    		if($idturma!=$turidAnteiror){
    			$x=0;
    			$turidAnteiror ='';
    		}
    		if($x!=0||$turidAnteiror==''){
    			$turidAnteiror =$idturma;
    		} 
    		if($idturma==$turidAnteiror){
    			$x++;
    		}
			$sql=" 	SELECT
						turqtdalunosprevistos - ( 	SELECT 
														count(estid)
													FROM 
														projovemcampo.estudante est
													WHERE
														est.turid = {$idturma}
													AND  est.eststatus = 'A' )
					FROM
						projovemcampo.turma 
					WHERE
						turid = {$idturma}";
			$vagasturma = $db->pegaUm( $sql );
			if( $x > $vagasturma ){
	    		echo "
					<script>
		    			alert('Você ultrapassou a lotação da turma.');
			            window.location.href = window.location.href;
					</script>
			             ";
		        die();
	    	}
		}
	}
	
	foreach($traid as $idtransferencia){
	    
		$sqlDadosTrans = 	"SELECT DISTINCT
								turid_origem,
					    		estid,
					    		periodotransferido,
					    		entid_destino
				    		FROM
					    		projovemcampo.transferencia
				    		WHERE
					    		traid = {$idtransferencia}";
		$DadosTrans = $db->pegaLinha($sqlDadosTrans);
	    		 
    		if($DadosTrans['turid_origem']){
    			
	    		$sqlPeriodoscursados =	"SELECT DISTINCT
	    									perid
							    		FROM
							    		projovemcampo.diario
							    			WHERE
							    				turid = {$turid[$idtransferencia]}
		    				";
    			$Periodoscursados = $db->pegaLinha($sqlPeriodoscursados);
	    
    			if($Periodoscursados){
    				$sqlperiododestino = 	"SELECT
						    					max(perid)
						    				FROM
						    					projovemcampo.diario
						    				WHERE
						    				turid = {$turid[$idtransferencia]}";
	    				// ver($sqlperiododestino,d);
    				$periododestino = $db->pegaUm ( $sqlperiododestino );
	    
    				$inativaFrequencias .= 	"UPDATE projovemcampo.lancamentodiario
				    						SET
					    						status = 'I'
					    						WHERE
					    						Estid = {$DadosTrans['estid']}
					    					AND diaid in(SELECT DISTINCT
					    									diaid
									    				FROM
									    					projovemcampo.diario
									    				WHERE
									    					turid = {$DadosTrans['turid_origem']});";
					$sqltestaturma ="SELECT DISTINCT
					    				lnd.lndid
				    				FROM
					    				projovemcampo.lancamentodiario lnd
				    				INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
				    				WHERE
					    				dia.turid = {$turid[$idtransferencia]}
					   				AND lnd.estid = {$DadosTrans['estid']}
					    			AND lnd.lndstatus = 'I'
					    			ORDER BY
						    			lndid";
    				$testaturma = $db->pegarColunas( $sqltestaturma );
	    		
    				if (is_array ( $testaturma ) && ! empty ( $testaturma )) {
	    
    					$sqlfrequenciavolta.= 	"UPDATE projovemcampo.lancamentodiario  SET
	    											frqstatus = 'A'
	    										WHERE
	    											estid = {$DadosTrans['estid']}
	    										AND lndstatus = 'I'
	    										AND lndid in ($testaturma);";
    
	    
    					$sqlVerificaPeriodoVolta =	"SELECT
						    							dia.perid
						    						FROM
						    							projovemcampo.diario dia
						    						INNER JOIN projovemcampo.lancamentodiario lnd ON lnd.diaid = dia.diaid
						    						WHERE
						    							lnd.estid = {$DadosTrans['estid']}
					    							AND lndstatus = 'I'
					    							AND lndid in ($testaturma)";
	    
    					$VerificaPeriodoVolta = $db->carregarColuna($sqlVerificaPeriodoVolta);
	    
						$sqlinserefrequencianova.= "
						    						INSERT INTO projovemcampo.lancamentodiario(estid, diaid, lndhorasescola, lndhorascomunidade)
						    							SELECT
						    								estid,
							    							(
								    						SELECT DISTINCT
								    							diaid
								    						FROM
								    							projovemcampo.diario dia2
								    						WHERE
								    							dia2.diaid in(
								    											SELECT
							    													diaid
								    											FROM
								    												projovemcampo.diario 
								    											WHERE
								    												turid = {$turid[$idtransferencia]}
								    											AND perid NOT IN ($VerificaPeriodoVolta)
							    												)
						    								AND dia2.perid = dia.perid
						    								) as diaid,
						    								lndhorasescola,
						    								lndhorascomunidade
						    							FROM
						    								projovemcampo.lancamentodiario lnd
						    							INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
						    							INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
						    							WHERE
						    								estid = $estid
						    							AND turid = {$dadostrasns['turid_origem']}
						    							AND lndstatus = 'I'
						    							AND perid NOT IN ($VerificaPeriodoVolta)
						    							ORDER BY
						    							3
	    							;";
    				} else {
						if(!$periododestino){
							$periododestino = 1;
						}
						$sqlinserefrequencianova.= "
										INSERT INTO projovemcampo.lancamentodiario(estid, diaid, lndhorasescola, lndhorascomunidade)
											SELECT
												estid,
												(
													SELECT DISTINCT
														diaid
													FROM
														projovemcampo.diario dia2
													WHERE
														dia2.diaid in(
																	SELECT
																		diaid
																	FROM
																		projovemcampo.diario 
																	WHERE
																		turid = {$DadosTrans[$idtransferencia]}
																	AND perid BETWEEN 1
																	AND(SELECT perid FROM projovemcampo.periodo WHERE perid = ($periododestino - 1))
																	)
													AND dia2.perid = dia.perid
												) as diaid,
												lndhorasescola,
												lndhorascomunidade
											FROM
												projovemcampo.lancamentodiario lnd
											INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
											INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
											WHERE
													estid = $caeid
													AND turid = {$dadostrasns['turid_origem']}
													AND lndstatus = 'I'
										AND dia.perid BETWEEN (SELECT perid FROM projovemcampo.periodo WHERE  perid = 1)
										AND($periododestino - 1)
										ORDER BY
										3
										;";
															// ver($sql12,d);
				}
				
			}
			$sqlTransEstudante .= "UPDATE projovemcampo.estudante
									SET
										turid = {$turid[$idtransferencia]}
									WHERE
										estid = {$DadosTrans['estid']};";
			
			$justificativa = "Transferência efetuada!";
			if($turid[$idtransferencia]){
				$turma = ",turid_destino = {$turid[$idtransferencia]}";
			}
			$htrid = atualiza_historico_transferencia($idtransferencia,3,$justificativa);
			
			$sqlhistorico .= 	"UPDATE projovemcampo.transferencia SET
										htrid_ultimo = $htrid
										$turma
									WHERE
										traid = $idtransferencia;";
			
			$sql = "SELECT DISTINCT
						usu.usuemail
					FROM
						projovemcampo.secretaria 		seca
					INNER JOIN seguranca.usuario        usu ON usu.usucpf = seca.secoordcpf
					INNER JOIN projovemcampo.turma 		tur ON tur.secaid = seca.secaid
					WHERE
						tur.entid = '{$DadosTrans['entid_destino']}'";
			//                ver($sql,d);
			$email_resp_destino = $db->pegaUm($sql);
			
			$sql = "SELECT estcpf||' - '||estnome FROM projovemcampo.estudante WHERE estid = {$DadosTrans['estid']}";
			
			$alunos = $db->carregarColuna($sql);
			
			$assunto = "PROJOVEM CAMPO - Pedido de transferência sob sua reponsabilidade.";
			
			$conteudo = "Foi feito pedido de tranferência do seguinte aluno para escola de sua reponsabilidade:
					".implode('; <br>',$alunos).".";
			//     				enviar_email( $_SESSION['email_sistema'], $email_resp_destino, $assunto, $conteudo );
    	}
	}
	if($sqlhistorico){
		$db->executar($sqlhistorico);
	}
	if($sqlTransEstudante){
		$db->executar($sqlTransEstudante);
	}
	if($sqlinserefrequencianova){
		$db->executar($sqlinserefrequencianova);
	}
	if($sqlfrequenciavolta){
		$db->executar($sqlfrequenciavolta);
	}
	if($sqlfrequenciavolta){
		$db->executar($sqlfrequenciavolta);
	}
	
	$db->commit();
	echo "
			<script>
// 				alert('Tranferência Concluida.');
				window.location.href = window.location.href;
			</script>";
}

function receber_aluno_cancelar(){

    global $db;
    
    extract($_POST);
    if( $traid[0] != '' ){
    	foreach( $traid as $traid_ ){
    		
    		$sqlDadosTrans = 	"SELECT DISTINCT
    								turid_origem,
    								estid
					    		FROM
					    			projovemcampo.transferencia
					    		WHERE
					    			traid = {$traid_}";
    		$DadosTrans = $db->pegaLinha($sqlDadosTrans);
    		
    		$sql = "UPDATE projovemcampo.transferencia SET
    					cpfresponsaveldestino = '{$_SESSION['usucpf']}'
    				WHERE
    					traid = $traid_;";
    				 
    				 
    		$htrid = atualiza_historico_transferencia($traid_,4,$justificativa);
    		    		 
			$sqlhistorico .= 	"UPDATE projovemcampo.transferencia SET
		    		    			htrid_ultimo = $htrid
		    		    		WHERE
		    		    			traid = $traid_;";
    		$sql = "SELECT DISTINCT
						usu.usuemail
					FROM
						projovemcampo.secretaria 		seca
					INNER JOIN seguranca.usuario        usu ON usu.usucpf = seca.secoordcpf
					INNER JOIN projovemcampo.turma 		tur ON tur.secaid = seca.secaid
					WHERE
						tur.turid = '{$DadosTrans['turid_origem']}'";
			//                ver($sql,d);
			$email_resp_destino = $db->pegaUm($sql);
			
			$sql = "SELECT estcpf||' - '||estnome FROM projovemcampo.estudante WHERE estid = {$DadosTrans['estid']}";
			
			$alunos = $db->carregarColuna($sql);
			
			$assunto = "PROJOVEM CAMPO - Pedido de transferência sob sua reponsabilidade.";
			
			$conteudo = "Foi feito pedido de tranferência do seguinte aluno para escola de sua reponsabilidade:
					".implode('; <br>',$alunos).".";
			//     				enviar_email( $_SESSION['email_sistema'], $email_resp_destino, $assunto, $conteudo );
    	}
    }
    
    if($sqlhistorico){
    	$db->executar($sqlhistorico);
    }
    
	$db->commit ();
	echo "	<script>
				alert('Cancelamento concluído.');
				window.location.href = window.location.href;
			</script>";
	die;
}
    
function buscarDadosAluno(){
	
	global $db;
	
	extract($_POST);
    $sql="
	    	SELECT 
				apcid as apcid,
				--apcesfera as esfera,
				apccodibge,	 
				aprid as aprid
			FROM 
				projovemcampo.estudante est
			INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
			WHERE
				estcpf ='$estcpf'";
    $dadosaluno = $db->pegalinha( $sql );
   
    $sql = "SELECT 
    			true 
    		FROM
    			 projovemcampo.transferencia tra
            INNER JOIN projovemcampo.estudante est ON est.estid = tra.estid
            INNER JOIN projovemcampo.historico_transferencia htr ON htr.htrid = tra.htrid_ultimo
            WHERE 
    			estcpf = '$estcpf' AND htr.shtid_status NOT IN (3,4,5)";
              
    $temTranferenciaAtiva = $db->pegaUm( $sql );
   
	if( $dadosaluno['apcid'] == $_SESSION['projovemcampo']['apcid']){
//         if($dadosaluno['aprid'] == $_SESSION['projovemcampo']['aprid']){
            if( $temTranferenciaAtiva != 't' ){
				$sql = "SELECT
							est.estid as estid,
							est.turid as turidorigem,
							apc.aprid as anoorigem,
							est.estnome as nome,
							to_char(his.hicdataacao,'DD/MM/YYY') as dataingresso,
							ent.entnome as Escola,
							turdescricao as turma
						FROM 
							projovemcampo.estudante est
						INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
						INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
						INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
						LEFT  JOIN projovemcampo.historicocadastro his ON his.estid = est.estid
						WHERE
							estcpf = '$estcpf'";
            	$dadosAluno = $db->pegaLinha($sql);
            }else{
                $dadosAluno = Array('nome'=>utf8_encode("Este aluno já possui tranferência em andamento."));
            }
//         }else{
// 			$dadosAluno = Array('nome'=>utf8_encode("Aluno não encontrado, o aluno deve ser da mesma coordenação e do mesmo ano de execução do sistema que está operando"));
// 		}
     }else{
            $dadosAluno = Array('nome'=>utf8_encode("Aluno não encontrado, o aluno deve ser da mesma coordenação e do mesmo ano de execução do sistema que está operando"));
     }
	echo simec_json_encode($dadosAluno);
    die;
}

function buscarComboEscola(){

    global $db;
    
    $sql = "(
    		SELECT DISTINCT
    			ent.entid as codigo,
    			ent.entnome as descricao
    		FROM
    			entidade.entidade ent
    		INNER JOIN projovemcampo.turma tur ON tur.entid = ent.entid
    		INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
    		INNER  JOIN projovemcampo.transferencia tra1 ON tra1.turid_origem = tur.turid
    		WHERE
    			apc.apcid = {$_SESSION['projovemcampo']['apcid']}
    		)UNION ALL(
            SELECT DISTINCT
    			ent.entid as codigo,
    			ent.entnome as descricao
    		FROM
    			entidade.entidade ent
    		INNER JOIN projovemcampo.turma tur ON tur.entid = ent.entid
    		INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
    		INNER  JOIN projovemcampo.transferencia tra2 ON tra2.entid_destino = tur.entid
    		WHERE
    			apc.apcid = {$_SESSION['projovemcampo']['apcid']}
            )
    		";
    mostrarComboPopup( 'Escola:', 'entid',  $sql, '', 'Selecione o(s) Escola(s)', array(), 'filtraTurma' );
}

function buscarComboTurma(){

    global $db;
    
    extract($_POST);
  
    if($entid[0]!=''){
	    $sql = "(
			    SELECT DISTINCT
			    	tur.turid as codigo,
			    	tur.turdescricao as descricao
			    FROM
			    	projovemcampo.turma tur 
			    INNER  JOIN projovemcampo.transferencia tra ON tra.turid_origem = tur.turid
			    WHERE
			    	tur.entid IN ( ".implode(', ',$entid)." )
			    )UNION ALL(
			     SELECT DISTINCT
			    	tur.turid as codigo,
			    	tur.turdescricao as descricao
			    FROM
			    	projovemcampo.turma tur 
			    INNER  JOIN projovemcampo.transferencia tra ON tra.entid_destino = tur.entid
			    WHERE
			    	tur.entid IN ( ".implode(', ',$entid)." )
			    )
			    ";
    }else{
    	 $sql = "SELECT
                    '' as codigo, '' as descricao";
    }
	    mostrarComboPopup( 'Turma:', 'turid',  $sql, '', 'Selecione a(s) Turma(s)' );
}

function buscarEscola(){

	global $db;
	
	$Escolas =  pegarEscolas(true);
	
	$Escolas = ( $Escolas ? $Escolas : array(0=> array("codigo"=>0 , "descricao"=>"Sem Escolas")));
    
    $db->monta_combo('entidM', $Escolas, 'S', 'Selecione', 'buscarTurmasM', '', '', '', 'N', 'entidM');
   
}
function buscarEscolaDestino(){
	global $db;
	
	$perfis = pegaPerfilGeral();
	if($_REQUEST['modalidade']=='M'){
		if($_SESSION['projovemcampo']['estuf']){
			$whereLocalidade = "AND est.estuf::character(2) = '{$_SESSION['projovemcampo']['estuf']}' AND apc.apcesfera = 'E'";
			$innerLocalidade ="INNER JOIN territorios.estado est ON est.estcod = apc.apccodibge::character(2)  ";
		}else{
			$whereLocalidade = "AND mun.muncod::character(7) = '{$_SESSION['projovemcampo']['muncod']}'";
			$innerLocalidade ="INNER JOIN territorios.municipio mun ON mun.muncod = apc.apccodibge::character(7) AND apc.apcesfera = 'M' ";
		}
	}else{
		if($_REQUEST['estuf']){
			$whereLocalidade = "AND est.estuf::character(2) = '{$_REQUEST['estuf']}'";
			$innerLocalidade ="INNER JOIN territorios.estado est ON est.estcod = apc.apccodibge::character(2) AND apc.apcesfera = 'E'  ";
		}else{
			$whereLocalidade = "AND mun.muncod::character(7) = '{$_REQUEST['muncod']}' AND apc.apcesfera = 'M'";
			$innerLocalidade ="INNER JOIN territorios.municipio mun ON mun.muncod = apc.apccodibge::character(7)  ";
		}
	}
	if(in_array(PFL_COORDENADOR_TURMA, $perfis)){
		$inner_diretor = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.turid = tur.turid AND rpustatus='A'";
	}
	if(in_array(PFL_DIRETOR_ESCOLA, $perfis)){
		$inner_diretor= "INNER JOIN	projovemcampo.usuarioresponsabilidade usu	ON usu.entid = tur.entid AND usucpf = '".$_SESSION['usucpf']."' AND rpustatus='A'";
	}
	
	$sql = "WITH entidadeturma as(SELECT DISTINCT
									tur.entid as entid,
									ent.entnome as entnome,
									tur.turqtdalunosprevistos as turqtdalunosprevistos,									
									(SELECT 
										count(estid)
									FROM
										projovemcampo.estudante
									WHERE
										turid =tur.turid) as qtdmatriculado
								FROM
									projovemcampo.adesaoprojovemcampo apc
								INNER JOIN	projovemcampo.turma tur ON tur.secaid = apc.secaid
								INNER JOIN 	entidade.entidade ent	ON ent.entid = tur.entid
								$innerLocalidade
								WHERE
									apc.aprid = {$_REQUEST['aprid']}
								AND tur.turstatus = 'A'	
								$whereLocalidade																
								)
			SELECT DISTINCT
				entid as codigo,
				entnome as descricao							
			FROM
				entidadeturma
			WHERE
				entidadeturma.turqtdalunosprevistos > entidadeturma.qtdmatriculado";
	$Escolas = $db->carregar( $sql );
	
	$Escolas = ( $Escolas ? $Escolas : array(0=> array("codigo"=>0 , "descricao"=>"Sem Escolas")));
	
	$db->monta_combo('entidDestino', $Escolas, 'S', 'Selecione', 'buscarTurmasDestino', '', '', '', 'N', 'entidDestino');
}

function buscarTurmasDestino(){
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_DIRETOR_ESCOLA, $perfis)){
		$inner_escolas = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.entid = tur.entid AND rpustatus='A'";
	}
	$sqlQtdMax = "
					SELECT DISTINCT
						SUM(turqtdalunosprevistos)
					FROM	
						projovemcampo.turma
					WHERE
						entid = {$_REQUEST['entid']}";
	
	$qtdMax = $db->pegaUm ($sqlQtdMax);
	
	$sqlQtdAlunos = "SELECT DISTINCT
						count(estid) 
					FROM 
						projovemcampo.estudante est 
					INNER JOIN	projovemcampo.turma tur ON tur.turid = est.turid
					WHERE 
						est.eststatus = 'A'
					AND tur.entid = {$_REQUEST['entid']}";
	$qtdAlunos = $db->pegaUm ( $sqlQtdAlunos );
	
	if($qtdMax > $qtdAlunos){
		$sql = "SELECT DISTINCT
					tur.turid as codigo,
					tur.turdescricao ||', Total de Alunos: '||(
															SELECT 
																count(*) 
															FROM projovemcampo.estudante c 
															WHERE 
																c.turid = tur.turid 
																AND eststatus = 'A'
															) 
					as descricao
				FROM
				projovemcampo.turma tur
				INNER JOIN	projovemcampo.estudante est ON est.turid = tur.turid
				$inner_escolas
				WHERE
					tur.entid = {$_REQUEST['entid']}
				AND tur.turstatus = 'A'
				AND tur.turqtdalunosprevistos > (
												SELECT 
													count(*) 
												FROM projovemcampo.estudante c 
												WHERE 
													c.turid = tur.turid 
													AND eststatus = 'A'
												)
				ORDER BY
					tur.turid";
		$Turmas = $db->carregar ( $sql );
		
		$Turmas = ( $Turmas ? $Turmas : array(0=> array("codigo"=>0 , "descricao"=>"Sem turmas com vaga.")));
	}else{
		$Turmas = array(0=> array("codigo"=>0 , "descricao"=>"Número máximos de alunos atingido."));
	}
	$db->monta_combo('turidDestino', $Turmas, 'S', 'Selecione uma Turma', '', '', '', '', 'N', 'turidDestino');
	
}
function anexarArquivos(){
        monta_titulo('Lista de Arquivos','');

        $acao = "
            <center>
                <img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer\" onclick=\"excluirArquivoInfra('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">
                &nbsp;&nbsp;
                <img src=\"../imagens/indicador-vermelha.png\" title=\"Baixar Arquivo (Download)\" style=\"cursor: pointer\" onclick=\"downloadArquivoInfra('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">            
            </center>
        ";

        $sql = "
            SELECT '{$acao}' as acao,
                    ta.tpadsc as tipo_doc,
                    arqdescricao as descricao,
                    arqnome||'.'||arqextensao as arqnome,
                    arqtamanho as tamanho,
                    to_char(arqdata, 'DD/MM/YYYY') as data
            FROM academico.demandainfra di
            JOIN academico.arquivotipologia AS a ON a.dinid = di.dinid
            JOIN academico.tipoarquivo AS ta ON ta.tpaid = a.tpaid
            JOIN public.arquivo AS arq ON arq.arqid = a.arqid
            WHERE di.dinid  = {$dinid}
        ";
        $cabecalho = array("<center>Ação</center>", "Tido de Documento", "Descrição Arquivo", "Nome Arquivo", "Tamanho (MB)", "Data Inclusão");
        $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
        $tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
}
function mostraDetalheTransferencia() {
	global $db;
	array($_POST);

	$sql = "SELECT DISTINCT
				shtdescricao as status,
				est.estcpf as cpf,
				est.estnome as nome,
				
				CASE 
					WHEN apcO.apcesfera = 'E'
					THEN munO.estuf
					ELSE munO.estuf|| ' - '||munO.mundescricao
				END as localorigem,
				entO.entnome as escolaorigem,
				turO.turdescricao as turmaorigem,
				aprO.apranoreferencia as anoorigem,
				CASE 
					WHEN apcD.apcesfera = 'E' OR apcD2.apcesfera = 'E'
					THEN coalesce(munD.estuf,munD2.estuf)
					ELSE coalesce(munD.estuf|| ' - '||munD.mundescricao,munD2.estuf|| ' - '||munD2.mundescricao)
				END as localdestino,
				coalesce(entD.entnome,entD2.entnome) as escoladestino,
				coalesce(turD.turdescricao,turD2.turdescricao) as turmadestino,
				coalesce(aprD.apranoreferencia,aprD2.apranoreferencia) as anodestino,
				CASE 
					WHEN arqt.arqid IS NULL
					THEN 'Não Inserido'
					ELSE arqt.arqid::text
				END as arquivo,
				usu.usunome as resporigem,
				coalesce(usu2.usunome,usu3.usunome) as respdestino
			FROM
				projovemcampo.transferencia tra
			INNER JOIN seguranca.usuario	usu  ON usu.usucpf  = tra.cpfresponsavelorigem
			LEFT  JOIN projovemcampo.arquivo_tranferencia		arqt ON arqt.traid = tra.traid
			INNER JOIN projovemcampo.estudante est ON est.estid = tra.estid
			LEFT JOIN projovemcampo.historicocadastro htc ON htc.estid =  est.estid
			INNER JOIN projovemcampo.historico_transferencia       hst  ON hst.htrid   = tra.htrid_ultimo
			INNER JOIN projovemcampo.statushistorictransferencia   sht  ON sht.shtid   = hst.shtid_status
			-- Dados Origem
			INNER JOIN projovemcampo.turma turO ON turO.turid = tra.turid_origem
			INNER JOIN projovemcampo.adesaoprojovemcampo apcO ON apcO.secaid = turO.secaid
			INNER JOIN projovemcampo.anoprograma aprO ON aprO.aprid = apcO.aprid
			INNER JOIN entidade.entidade entO ON entO.entid = turO.entid
			INNER JOIN entidade.endereco endeO ON endeO.entid = entO.entid
			INNER JOIN territorios.municipio munO ON munO.muncod = endeO.muncod
			--Dados Destino
			LEFT JOIN entidade.entidade entD ON entD.entid = tra.entid_destino
			LEFT JOIN projovemcampo.turma turD ON turD.entid = entD.entid
			LEFT JOIN projovemcampo.adesaoprojovemcampo apcD ON apcD.secaid = turD.secaid
			LEFT JOIN projovemcampo.secretaria seca ON seca.secaid = turD.secaid
			LEFT JOIN seguranca.usuario	usu2  ON usu2.usucpf  = seca.secoordcpf
			LEFT JOIN entidade.endereco endeD ON endeD.entid = entD.entid
			LEFT JOIN projovemcampo.anoprograma aprD ON aprD.aprid = apcD.aprid
			LEFT JOIN territorios.municipio munD ON munD.muncod = endeD.muncod
			--Dados Destino 2
			LEFT JOIN projovemcampo.turma turD2 ON turD2.turid = tra.turid_destino
			LEFT JOIN entidade.entidade entD2 ON entD2.entid = turD2.entid
			LEFT JOIN projovemcampo.adesaoprojovemcampo apcD2 ON apcD2.secaid = turD2.secaid
			LEFT JOIN entidade.endereco endeD2 ON endeD2.entid = entD2.entid
			LEFT JOIN projovemcampo.anoprograma aprD2 ON aprD2.aprid = apcD2.aprid
			LEFT JOIN territorios.municipio munD2 ON munD2.muncod = endeD2.muncod
			LEFT JOIN projovemcampo.secretaria seca3 ON seca3.secaid = turD2.secaid
			LEFT JOIN seguranca.usuario	usu3  ON usu3.usucpf  = seca3.secoordcpf
			WHERE
				tra.traid = {$_POST['traid'] }
			ORDER BY
				nome";
	$dados = $db->pegaLinha ( $sql );
	extract ( $dados );
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="Center">
	<tr>
		<td class="SubTituloDireita" width="20%">Aluno:</td>
		<td width="30%"><?=$nome?></td>
		<td class="SubTituloDireita" width="20%">Coordenação Destino:</td>
		<td width="30%"><?=$localdestino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CPF:</td>
		<td width="30%"><?=$cpf?></td>
		<td class="SubTituloDireita" width="20%">Escola Destino</td>
		<td width="30%"> <?=$escoladestino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Coordenação de Origem:</td>
		<td width="30%"><?=$localorigem?></td>
		<td class="SubTituloDireita" width="20%">Turma Destino:</td>
		<td width="30%"> <?=$turmadestino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Escola Origem:</td>
		<td width="30%"> <?=$escolaorigem?></td>
		<td class="SubTituloDireita" width="20%">Ano Destino:</td>
		<td width="30%"><?=($anodestino!=''?$anodestino:'Não informado')?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Ano Origem</td>
		<td width="30%"><?=$anoorigem?></td>
		<td class="SubTituloDireita" width="20%">Status Atual:</td>
		<td width="30%"><?=$status?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Turma Origem:</td>
		<td width="30%"> <?=$turmaorigem?></td>
		<td class="SubTituloDireita" width="20%">Coordenador Responsável Destino:</td>
		<td width="30%"><?=$respdestino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Data que Ingressou na Origem</td>
		<td width="30%"><?=$dataingressoorigem?></td>
		
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Coordenador Responsável
			Origem:</td>
		<td width="30%"><?=$resporigem?></td>
	</tr>
</table>
<?
	$sql = "SELECT 
                *
            FROM
            (
            SELECT 
                ( SELECT
                    shtdescricao
                FROM
                    projovemcampo.statushistorictransferencia
                WHERE
                    shtid = (SELECT shtid_status 
                        FROM projovemcampo.historico_transferencia
                        WHERE htrid = (SELECT max(htrid) FROM projovemcampo.historico_transferencia WHERE traid = htr.traid AND htrid < htr.htrid )
                        ) ) as anterior,
                shtdescricao as atual,
                usu.usunome,
                to_char( htrdatahoraacao, 'DD/MM/YYYY HH:MM:SS' ) as data,
                justificativa
            FROM 
                projovemcampo.historico_transferencia htr
            INNER JOIN projovemcampo.statushistorictransferencia   sht ON sht.shtid  = htr.shtid_status
            LEFT  JOIN seguranca.usuario                usu ON usu.usucpf = usucpf_fezacao
            WHERE 
                traid = {$_POST['traid'] }
            ) asfoo
            WHERE
                anterior IS NOT NULL";
	// ver($sql,d);
	$cabecalho = array (
			"Onde estava?",
			"O que aconteceu?",
			"Quem fez?",
			"Quando fez?",
			"Justificativa" 
	);
	$db->monta_lista ( $sql, $cabecalho, 100, 5, 'N', 'center', '', 'form_transferencias' );
}

if (!$_SESSION['projovemcampo']['apcid']) {
	die("<script>
    		alert('Problema encontrado no carregamento. Inicie novamente a navegação.');
            window.location='projovemcampo.php?modulo=inicio&acao=C';
		</script>");
}

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

if( $_POST['traid'][0] != '' ){
    enviar_aluno_transferir();
    exit;
}

if ($_REQUEST['estuf']){
    $_SESSION['projovemcampo']['estuf'] = $_REQUEST['estuf'];
} else if ($_REQUEST['muncod']){
    $_SESSION['projovemcampo']['muncod'] = $_REQUEST['muncod'];
}

if (!$_SESSION['projovemcampo']['apcid']) {
    carregarprojovemcampo();
    //encaminharUltimoAcesso();
}
if (!$_SESSION['projovemcampo']['apcid']) {
    die("<script>
			alert('Problema encontrado no carregamento. Inicie novamente a navegação.');
					window.location='projovemcampo.php?modulo=inicio&acao=C';
		</script>");
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
echo montarAbasArray(montaMenuprojovemcampo(),"/projovemcampo/projovemcampo.php?modulo=principal/transferencia&acao=A");
monta_titulo('Transferência de Alunos',montaTituloEstMun());

$db->cria_aba($abacod_tela,$url,$parametros);
$aba = !$_GET['aba'] ? "enviar_aluno" : $_GET['aba'];

$abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/transferencia&acao=A&aba=$aba";

/*** Array com os itens da aba de identificação ***/							
$menu = array( 0 => array("id" => 1, "descricao" => "Enviar Alunos", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/transferencia&acao=A&aba=enviar_aluno")
			  ,1 => array("id" => 2, "descricao" => "Receber Aluno", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/transferencia&acao=A&aba=receber_aluno")
			  ,2 => array("id" => 3, "descricao" => "Painel de Transferência", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/transferencia&acao=A&aba=paineldetransferencia")
			  );
echo "<br/>"; 
$perfis = pegaPerfilGeral();
if($db->testa_superuser()||in_array(PFL_ADMINISTRADOR, $perfis)||in_array(PFL_COORDENADOR_ESTADUAL, $perfis)||in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_CONSULTA, $perfis)){

	echo montarAbasArray($menu, $abaAtiva);			  
	
	include_once "transferenciaAluno/".$aba.".inc";
}


 ?>
<? registarUltimoAcesso(); ?>
