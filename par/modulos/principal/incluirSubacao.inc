<?php

ini_set("memory_limit", "2048M");
set_time_limit(0);
	
if($_POST['requisicaoAjax'] == 'filtraPlanoInterno'){
	global $db;
	
	$sqlPlanos = "SELECT 	DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno as descricao
				FROM
					par.planointerno
				WHERE pliano = '".date('Y')."'  AND prgid = ".$_POST['prgid'];
											
	echo $db->monta_combo( "sbdplanointerno[".date('Y')."]", $sqlPlanos , 'S', 'Selecione', 'filtraPTRES', '', '','','N', 'sbdplanointerno', false, $sbdplanointerno, 'Plano Interno' );
	exit();
}

if($_POST['requisicaoAjax'] == 'filtraPTRES'){
	global $db;

	$sql = "select DISTINCT
				pliptres as codigo,
				pliptres as descricao
			from par.planointerno
			where plinumplanointerno = '{$_POST['sbdplanointerno']}'";
	
	echo $db->monta_combo( "sbdptres[".date('Y')."]", $sql, 'S', 'Selecione...', '',"","","","N","","");
	exit;
}

if($_POST['salvaformulario'] == 1){
	global $db;

	if( $_POST['tpsubacao'] == 1 ){ //Vem do guia!

		if($_POST['tipo'] == 1 ){ //estado
			if( is_array( $_POST['estados'] ) && $_POST['estados'][0] ){
				$estados = implode("', '", $_POST['estados']);
			} else { //todos
				$sql = "select estuf from territorios.estado";
				$arrEstados = $db->carregarColuna($sql);
				$estados = implode("', '", $arrEstados);
			}
			$sql = "SELECT inuid FROM par.instrumentounidade WHERE estuf IN ('{$estados}')";
			$sqlInuid = "SELECT DISTINCT
							iu.inuid
						FROM
							territorios.estado AS e   
						LEFT JOIN par.instrumentounidade iu ON iu.estuf = e.estuf
						LEFT JOIN workflow.documento d on d.docid = iu.docid AND d.tpdid = 44 
						LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
						WHERE
							ed.esdid IN (".WF_ELABORACAO.", ".WF_ANALISE.")";
			
		} else { // municipio
			if( is_array( $_POST['municipios'] ) || is_array( $_POST['municipiosMobEquip'] ) ){
				if( is_array( $_POST['municipiosMobEquip'] ) && !empty($_POST['municipiosMobEquip'][0]) ) $municipios = implode("', '", $_POST['municipiosMobEquip']);
				else $municipios = implode("', '", $_POST['municipios']);
			} else { //todos
				$sql = "select muncod from territorios.municipio";
				$arrMunicipios = $db->carregarColuna($sql);
				$municipios = implode("', '", $arrMunicipios);
			}
			$sql = "select inuid from par.instrumentounidade where muncod in ('{$municipios}')";
			$sqlInuid = "SELECT DISTINCT
							iu.inuid
						FROM
							territorios.municipio AS m   
						LEFT JOIN par.instrumentounidade iu ON iu.muncod = m.muncod
						LEFT JOIN workflow.documento d on d.docid = iu.docid AND d.tpdid = 44 
						LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
						WHERE
							ed.esdid IN (".WF_ELABORACAO.", ".WF_ANALISE.")";
		}
	
		$arrInuid 		 = $db->carregarColuna( $sql );
		$arrInuidValidos = $db->carregarColuna( $sqlInuid );
		$arrInuidPulo = array();
	//	$arrInuidValidos = array();municipios
		
		foreach( $arrInuid as $inuid ){
	
			if( in_array( $inuid, $arrInuidValidos ) ){ //Verifica se está em elaboração ou em análise! //recoloquei essa regra no dia 17/10/2012 a pedido do Julio
		
				$sql = "select
							a.aciid
						from
							par.indicador i
						inner join par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
						inner join par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
						inner join par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
						where 
							i.indid = {$_POST['indid']} AND p.inuid = ".$inuid;
				
				$aciid = $db->pegaUm( $sql );

				if( $aciid ){
					
					// Salva subação	
					$oSubacao = new Subacao();
					
					$estado = $db->pegaUm("SELECT d.esdid FROM par.instrumentounidade iu
										INNER JOIN workflow.documento d ON d.docid = iu.docid
										WHERE iu.inuid = ".$inuid);
			
					if( $estado == WF_ANALISE ){
						//$desc = 'Em Diligência';
						//$esdid = WF_SUBACAO_DILIGENCIA;
						$desc = 'Em Análise';
						$esdid = WF_SUBACAO_ANALISE;
					} else {
						$desc = 'Em Elaboração';
						$esdid = WF_SUBACAO_ELABORACAO;
					}
					
					$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
							VALUES (62, ".$esdid.", '".$desc."', now()) returning docid ";
					$docid = $db->pegaUm($sql);
					
					$ordemMax = $db->pegaUm( "SELECT MAX( sbaordem ) FROM par.subacao WHERE aciid = ".$aciid." AND sbaordem > 2000 AND {$inuid} = ".$inuid );
					
					$ordemFinal = $ordemMax ? $ordemMax + 1 : 2001;
					
					//ver($ordemFinal, d);
					$oSubacao->sbaid 					  = null;
					$oSubacao->sbadsc 					  = $_POST['ppsdsc'];
					$oSubacao->sbaordem 				  = $ordemFinal;
				//	$oSubacao->sbaobra 					  = $propostaSubacao['sbaobra'];
					$oSubacao->sbaestrategiaimplementacao = $_POST['ppsestrategiaimplementacao'];
				//	$oSubacao->sbaptres 				  = $propostaSubacao['ppsptres'];
				//	$oSubacao->sbanaturezadespesa 		  = $propostaSubacao['ppsnaturezadespesa'];
					$oSubacao->sbamonitoratecnico 		  = $_POST['hdn_monitoramento'];
					$oSubacao->frmid 					  = $_POST['hdn_frmid'];
					$oSubacao->prgid 					  = $_POST['hdn_prgid'];
					$oSubacao->ptsid 					  = $_POST['hdn_ptsid'] ? $_POST['hdn_ptsid'] :  NULL;
					$oSubacao->indid 					  = $_POST['indid'];
					if( $_POST['hdn_foaid'] ){
						$oSubacao->foaid 					  = $_POST['hdn_foaid'] ? $_POST['hdn_foaid'] : NULL;
					}
					$oSubacao->sbastatus				  = 'A';
					$oSubacao->undid 					  = $_POST['hdn_undid'] ? $_POST['hdn_undid'] : NULL;
					$oSubacao->docid 					  = $docid;
					$oSubacao->ppsid 					  = $_POST['ppsid'];
					$oSubacao->sbacronograma 			  = $_POST['hdn_cronograma'] ? $_POST['hdn_cronograma'] : NULL;
					$oSubacao->sbaextraordinaria 		  = 20;
					if( $_POST['extraordinaria'] == 'sim' ){
						$oSubacao->sbaextraordinariatramita = 'true';
					} else {
						$oSubacao->sbaextraordinariatramita = 'false';
					}
					$oSubacao->aciid 					  = $aciid;
					$sbaid = $oSubacao->salvar();
					$oSubacao->commit();
		//ver($oSubacao, d);
					
					// Salva detalhe
					$oSbd = new SubacaoDetalhe();
		
					foreach( $_POST['sbdano'] as $ano ){
						
						if( $_POST['sbdparecer'][$ano] || 
							$_POST['sbdparecerdemerito'][$ano] || 
							$_POST['ssuid'][$ano] || 
							$_POST['sbddetalhamento'][$ano] || 
							$_POST['sbdquantidade'][$ano] || 
							$_POST['sbdinicio'][$ano] ){
								
							$entrou = 1;
					
							$parecer = NULL;
							$parecerdemerito = NULL;
							$statusdasubacao = NULL;
							
							if( $estado == WF_ANALISE ){
								$parecer = $_POST['sbdparecer'][$ano] ? $_POST['sbdparecer'][$ano] : NULL;
								$parecerdemerito = $_POST['sbdparecerdemerito'][$ano] ? $_POST['sbdparecerdemerito'][$ano] : NULL;
								$statusdasubacao = $_POST['ssuid'][$ano] ? $_POST['ssuid'][$ano] : NULL;
							}
							
							$oSbd->sbdid 				= null;	
							$oSbd->sbaid 				= $sbaid; //retorno da outra
							$oSbd->sbdparecer 			= $parecer;
							$oSbd->sbdparecerdemerito	= $parecerdemerito;
							$oSbd->sbdquantidade 		= $_POST['sbdquantidade'][$ano] ? str_replace(".","",$_POST['sbdquantidade'][$ano]) : null;
							$oSbd->sbdinicio 			= $_POST['sbdinicio'][$ano] ? $_POST['sbdinicio'][$ano] : NULL;
							$oSbd->sbdfim 				= $_POST['sbdfim'][$ano] ? $_POST['sbdfim'][$ano] : NULL;
							$oSbd->sbdplanointerno 		= $_POST['sbdplanointerno'][$ano] ? $_POST['sbdplanointerno'][$ano] : NULL;
							$oSbd->sbdptres 			= $_POST['sbdptres'][$ano] ? $_POST['sbdptres'][$ano] : NULL;
							$oSbd->sbddetalhamento		= $_POST['sbddetalhamento'][$ano] ? $_POST['sbddetalhamento'][$ano] : NULL;
							$oSbd->ssuid 				= $statusdasubacao;
							$oSbd->sbdanotermino 		= $_POST['sbdanotermino'][$ano] ? $_POST['sbdanotermino'][$ano] : NULL;
							$oSbd->sbdano 				= $ano;
						//	$oSbd->sbdnaturezadespesa 	= $_POST['sbdnaturezadespesa'][$ano];
							$oSbd->salvar();
							$oSbd->commit();
			
							//Insere os itens de composição
							
							if( is_array( $_POST['itemvalor'][$ano] ) ){
			
								$oIC = new SubacaoItensComposicao();
			
								foreach( $_POST['itemvalor'][$ano] as $picid => $it ){
									
									foreach( $it as $gicid => $icoquantidade ){
										
										if( $icoquantidade && $_POST['sbdquantidade'][$ano] ){
											$icoquantidade = $icoquantidade * $_POST['sbdquantidade'][$ano];
										}
			
										$uf = $db->pegaUm("SELECT CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun_estuf END as estuf 
															FROM par.instrumentounidade iu WHERE iu.inuid = ".$inuid);
										
										$dado = $db->pegaLinha("SELECT DISTINCT
																	pic.picdescricao,
																	pic.picdetalhe,
																	umiid,
																	dic.dicvalor as valor
																FROM
																	par.detalheitemcomposicao dic
																INNER JOIN par.propostaitemcomposicao pic ON pic.picid = dic.picid
																INNER JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid and puf.estuf = '{$uf}'
																INNER JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
																INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid AND ufdic.estuf = '{$uf}'
																WHERE
																	dic.picid = ".$picid." and
																	dic.dicstatus = 'A' AND
																	(now()::date between dic.dicdatainicial and dic.dicdatafinal)");
										
										if( !$dado ){
											$dado = $db->pegaLinha("SELECT DISTINCT
																	pic.picdescricao,
																	pic.picdetalhe,
																	umiid,
																	dic.dicvalor as valor
																FROM
																	par.detalheitemcomposicao dic
																INNER JOIN par.propostaitemcomposicao pic ON pic.picid = dic.picid
																LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid and puf.estuf = '{$uf}'
																LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
																INNER JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid AND ufdic.estuf = '{$uf}'
																WHERE
																	dic.picid = ".$picid." and
																	dic.dicstatus = 'A' AND
																	(now()::date between dic.dicdatainicial and dic.dicdatafinal)");
										}
										
										if( !$dado ){
											$dado = $db->pegaLinha("SELECT DISTINCT
																	pic.picdescricao,
																	pic.picdetalhe,
																	umiid,
																	dic.dicvalor as valor
																FROM
																	par.detalheitemcomposicao dic
																INNER JOIN par.propostaitemcomposicao pic ON pic.picid = dic.picid
																LEFT JOIN par.pregaouf puf ON puf.ptpid = dic.ptpid and puf.estuf = '{$uf}'
																LEFT JOIN par.propostatipopregao ptp on ptp.ptpid = puf.ptpid AND ptpstatus = 'A'
																LEFT JOIN par.ufdetalheitemcomposicao ufdic ON ufdic.dicid = dic.dicid AND ufdic.estuf = '{$uf}'
																WHERE
																	dic.picid = ".$picid." and
																	dic.dicstatus = 'A' AND
																	(now()::date between dic.dicdatainicial and dic.dicdatafinal)");
										}
										
		//								if( strpos($dicvalor, ',') ){
		//									$valor = str_replace(",",".", (str_replace(".","",$dicvalor)));
		//								} else {
		//									$valor = $dicvalor;
		//								}
										
										$oIC->icoid 			 = null;
										$oIC->icoano 			 = $ano;
										$oIC->icodescricao 	 	 = $dado['picdescricao'];
										$oIC->icoquantidade		 = $icoquantidade; //$_POST['icoquantidade'][$n] ? str_replace(".","",$_POST['icoquantidade'][$n]) : 0;
										$oIC->icoquantidadetecnico = $icoquantidade;
										$oIC->icovalidatecnico 	 = 'S'; 
										$oIC->icovalortotal		 = 0; //$_POST['valortotalH'][$n] ? str_replace(",",".", (str_replace(".","",$_POST['valortotalH'][$n]))) : 0;
										$oIC->icovalor 			 = $dado['valor'];
										$oIC->icostatus	 		 = "A";
										$oIC->sbaid 			 = $sbaid;
										if( $gicid != 0 ){
											$oIC->gicid 		 = $gicid;
										}
										$oIC->unddid 			 = $dado['umiid'];
										$oIC->icodetalhe 		 = $dado['picdetalhe'];
										$oIC->usucpf 	 		 = $_SESSION['usucpf'];
										$oIC->dtatualizacao 	 = "'now()'";
										$oIC->picid 			 = $picid;
										$oIC->salvar();
										$oIC->commit();
									}
								}
							}
							
							//Insere as Escolas
							if( is_array( $_POST['escola'][$ano] ) ){	
								
								$oSes = new SubacaoEscola();
								
								foreach( $_POST['escola'][$ano] as $entid ){	
			
									$sql = "select escid from par.escolas where entid = $entid";
									$escid = $db->pegaUm($sql);
									if(!$escid){
										$escid = $db->pegaUm("insert into par.escolas (entid) values ($entid) returning escid");
									}
									
									$oSes->sesid 		 = null;
									$oSes->escid 		 = $escid;
									$oSes->sesano 		 = $ano;
									$oSes->sesstatus	 = "A";
									$oSes->sbaid 		 = $sbaid;
									$oSes->salvar();
									$oSes->commit();
								}
							}
							
							//Insere Beneficiarios				
							if( is_array( $_POST['rural'][$ano] ) ){	
								
								$oBen = new SubacaoBeneficiario();
								
								foreach($_POST['rural'][$ano] as $benid => $sabqtdurbano){
									
									$oBen->sabid 		= null;
									$oBen->sbaid 		= $sbaid;
									$oBen->benid 		= $benid;
									$oBen->sabqtdurbano = $sabqtdurbano ? str_replace(".","",$sabqtdurbano) : 0;
									$oBen->sabqtdrural  = $_POST['urbano'][$ano][$benid] ? str_replace(".","",$_POST['urbano'][$ano][$benid]) : 0;
									$oBen->sabano 		= $ano;
									$oBen->salvar();
									$oBen->commit();
									
								}
							
							}
						}
						//ver('teste', d);
					}
				} else { //Não tem a ação!
					$arrInuidPulo[$inuid]['inuid'] = $inuid; 
					$arrInuidPulo[$inuid]['motivo'] = 'Não possui a ação cadastrada em seu Plano'; 
				}
			} else { //Não está entre os inuids validos
				$arrInuidPulo[$inuid]['inuid'] = $inuid; 
				$arrInuidPulo[$inuid]['motivo'] = 'O Plano não está em Elaboração ou Análise'; 
			}
			//Envio o email
			//enviaEmailDiligenciaSubacao( $inuid, 'Carga' );
		}

		if(count($arrInuidPulo) > 0){
			enviaEmailInuidFaltandoCargaSubacao( $arrInuidPulo );
		}

	} else { // Manual
		if($_POST['tpsubacao'] == 2 && $_FILES['arquivo']['name']){ // Se for tipo manual e tiver arquivo
			$dados = file($_FILES['arquivo']['tmp_name']);
			
			$ar = array();
			
			foreach($dados as $numline => $line) {
				unset($n);
				$columns = explode(";", $line);

				if($_POST['tipo'] == 1 ){ //estado
					$inuid = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE estuf = '".$columns[0]."'" );
				} else { //municipio
					$inuid = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$columns[0]."'" );
				}
				
				$ar[$inuid][trim($columns[3])][trim($columns[1])]['quantidade'] = trim($columns[2]);
			}
		}
		
		foreach( $ar as $inuid => $v ){

			$sql = "select
						a.aciid
					from
						par.indicador i
					inner join par.criterio c ON c.indid = i.indid AND c.crtstatus = 'A'
					inner join par.pontuacao p ON p.crtid = c.crtid AND p.ptostatus = 'A'
					inner join par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
					where 
						i.indid = {$_POST['indid']} AND p.inuid = ".$inuid;
			
			$aciid = $db->pegaUm( $sql );
				
			// Salva subação	
			$oSubacao = new Subacao();
			
			$estado = $db->pegaUm("SELECT d.esdid FROM par.instrumentounidade iu
								INNER JOIN workflow.documento d ON d.docid = iu.docid
								WHERE iu.inuid = ".$inuid);
	
			if( $estado == WF_ANALISE ){
				$desc = 'Em Análise';
				$esdid = WF_SUBACAO_ANALISE;
			} else {
				$desc = 'Em Elaboração';
				$esdid = WF_SUBACAO_ELABORACAO;
			}
			
			$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
					VALUES (62, ".$esdid.", '".$desc."', now()) returning docid ";
			$docid = $db->pegaUm($sql);
			
			$ordem = $db->pegaUm( "SELECT MAX(sbaordem) FROM par.subacao WHERE aciid = ".$aciid );
			
			$oSubacao->sbaid 					  = null;
			$oSubacao->sbadsc 					  = $_POST['ppsdsc'];
			$oSubacao->sbaordem					  = $ordem ? ($ordem+1) : 1;
			$oSubacao->sbaestrategiaimplementacao = $_POST['ppsestrategiaimplementacao'];
			$oSubacao->sbamonitoratecnico 		  = $_POST['hdn_monitoramento'];
			$oSubacao->frmid 					  = $_POST['hdn_frmid'];
			$oSubacao->prgid 					  = $_POST['hdn_prgid'];
			$oSubacao->ptsid 					  = $_POST['hdn_ptsid'];
			$oSubacao->indid 					  = $_POST['indid'];
			if( $_POST['hdn_foaid'] ){
				$oSubacao->foaid 					  = $_POST['hdn_foaid'] ? $_POST['hdn_foaid'] : NULL;
			}
			$oSubacao->sbastatus				  = 'A';
			$oSubacao->undid 					  = $_POST['hdn_undid'] ? $_POST['hdn_undid'] : NULL;
			$oSubacao->docid 					  = $docid;
			$oSubacao->ppsid 					  = $_POST['ppsid'];
			$oSubacao->sbacronograma 			  = $_POST['hdn_cronograma'] ? $_POST['hdn_cronograma'] : NULL;
			$oSubacao->aciid 					  = $aciid;
			$sbaid = $oSubacao->salvar();
			$oSubacao->commit();

			
			// Salva detalhe
			$oSbd = new SubacaoDetalhe();

			foreach( $v as $ano => $d ){
						
					$entrou = 1;
			
					$oSbd->sbdid 				= null;	
					$oSbd->sbaid 				= $sbaid; //retorno da outra
					$oSbd->sbdparecer 			= $_POST['sbdparecer'][$ano] ? $_POST['sbdparecer'][$ano] : NULL;
					$oSbd->sbdparecerdemerito	= $_POST['sbdparecerdemerito'][$ano] ? $_POST['sbdparecerdemerito'][$ano] : NULL;
					$oSbd->sbdquantidade 		= $_POST['sbdquantidade'][$ano] ? str_replace(".","",$_POST['sbdquantidade'][$ano]) : null;
					$oSbd->sbdplanointerno 		= $_POST['sbdplanointerno'][$ano] ? $_POST['sbdplanointerno'][$ano] : NULL;
					$oSbd->sbdptres 			= $_POST['sbdptres'][$ano] ? $_POST['sbdptres'][$ano] : NULL;
					$oSbd->sbdinicio 			= $_POST['sbdinicio'][$ano] ? $_POST['sbdinicio'][$ano] : NULL;
					$oSbd->sbdfim 				= $_POST['sbdfim'][$ano] ? $_POST['sbdfim'][$ano] : NULL;
					$oSbd->sbddetalhamento		= $_POST['sbddetalhamento'][$ano] ? $_POST['sbddetalhamento'][$ano] : NULL;
					$oSbd->ssuid 				= $_POST['ssuid'][$ano] ? $_POST['ssuid'][$ano] : NULL;
					$oSbd->sbdanotermino 		= $_POST['sbdanotermino'][$ano] ? $_POST['sbdanotermino'][$ano] : NULL;
					$oSbd->sbdano 				= $ano;
				//	$oSbd->sbdnaturezadespesa 	= $_POST['sbdnaturezadespesa'][$ano];
					$oSbd->salvar();
					$oSbd->commit();
	
					//Insere os itens de composição
					if( is_array( $d ) ){
	
						$oIC = new SubacaoItensComposicao();
	
						foreach( $d as $picid => $quantidade ){
	
							$dado = $db->pegaLinha("select * from par.propostaitemcomposicao pic LEFT JOIN par.detalheitemcomposicao dic ON dic.picid = pic.picid AND dicstatus = 'A' WHERE pic.picid = $picid");

							$oIC->icoid 			 = null;
							$oIC->icoano 			 = $ano;
							$oIC->icodescricao 	 	 = $dado['picdescricao'];
							$oIC->icoquantidade		 = $quantidade['quantidade']; //$_POST['icoquantidade'][$n] ? str_replace(".","",$_POST['icoquantidade'][$n]) : 0;
							$oIC->icovalortotal		 = 0; //$_POST['valortotalH'][$n] ? str_replace(",",".", (str_replace(".","",$_POST['valortotalH'][$n]))) : 0;
							$oIC->icovalor 			 = $dado['dicvalor'];
							$oIC->icostatus	 		 = "A";
							$oIC->sbaid 			 = $sbaid;
							$oIC->unddid 			 = $dado['umiid'];
							$oIC->icodetalhe 		 = $dado['picdetalhe'];
							$oIC->usucpf 	 		 = $_SESSION['usucpf'];
							$oIC->dtatualizacao 	 = "'now()'";
							$oIC->picid 			 = $picid;
							$oIC->salvar();
							$oIC->commit();
							
						}
					}
					
					//Insere as Escolas
					if( is_array( $_POST['escola'][$ano] ) ){	
						
						$oSes = new SubacaoEscola();
						
						foreach( $_POST['escola'][$ano] as $entid ){	
	
							$sql = "select escid from par.escolas where entid = $entid";
							$escid = $db->pegaUm($sql);
							if(!$escid){
								$escid = $db->pegaUm("insert into par.escolas (entid) values ($entid) returning escid");
							}
							
							$oSes->sesid 		 = null;
							$oSes->escid 		 = $escid;
							$oSes->sesano 		 = $ano;
							$oSes->sesstatus	 = "A";
							$oSes->sbaid 		 = $sbaid;
							$oSes->salvar();
							$oSes->commit();
						}
					}
					
				//ver('teste', d);
			}
			//ver('teste', d);
		}
	}
	
	if( $db->commit() ){
		echo "<script>
					alert('Operação realizada com sucesso!');
					window.close();
				</script>";
	} else {
		echo "<script>
					alert('Erro no processo!');
					window.location.href = window.reload;
				</script>";
	}
}

if( $_REQUEST['aciid'] ){

if( $_POST['adicionaSubacao'] && $_POST['ppsid'] && $_POST['aciid'] ){
	
	$sql = "SELECT * FROM par.propostasubacao WHERE ppsid = ".$_POST['ppsid'];
	$propostaSubacao = $db->pegaLinha( $sql );
	
	$oSubacao = new Subacao();
	
	$estado = $db->pegaUm("SELECT d.esdid FROM par.instrumentounidade iu
							INNER JOIN workflow.documento d ON d.docid = iu.docid
							WHERE iu.inuid = ".$_SESSION['par']['inuid']);
			
	if( $estado == WF_ANALISE ){
		$desc = 'Em Análise';
		$esdid = WF_SUBACAO_ANALISE;
	} else {
		$desc = 'Em Elaboração';
		$esdid = WF_SUBACAO_ELABORACAO;
	}
	
	$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
			VALUES (62, ".$esdid.", '".$desc."', now()) returning docid ";
	$docid = $db->pegaUm($sql);
	
	$oSubacao->sbaid 					  = null;
	$oSubacao->sbadsc 					  = $propostaSubacao['ppsdsc'];
	$oSubacao->sbaordem 				  = $propostaSubacao['ppsordem'];
	$oSubacao->sbaobra 					  = $propostaSubacao['sbaobra'];
	$oSubacao->sbaestrategiaimplementacao = $propostaSubacao['ppsestrategiaimplementacao'];
	$oSubacao->sbaptres 				  = $propostaSubacao['ppsptres'];
	$oSubacao->sbanaturezadespesa 		  = $propostaSubacao['ppsnaturezadespesa'];
	$oSubacao->sbamonitoratecnico 		  = $propostaSubacao['ppsmonitora'];
	$oSubacao->frmid 					  = $propostaSubacao['frmid'];
	$oSubacao->ptsid 					  = $propostaSubacao['ptsid'];
	$oSubacao->sbastatus				  = 'A';
	$oSubacao->indid 					  = $propostaSubacao['indid'];
	$oSubacao->foaid 					  = $propostaSubacao['foaid'];
	$oSubacao->undid 					  = $propostaSubacao['undid'];
	$oSubacao->docid 					  = $docid;
	$oSubacao->ppsid 					  = $propostaSubacao['ppsid'];
	$oSubacao->sbacronograma 			  = $propostaSubacao['ppscronograma'];
	$oSubacao->aciid 					  = $_POST['aciid'];
	$oSubacao->salvar();
	
	$oSubacao->commit();
	
}

if( $_POST['montaListadeValores'] ){
	
	$sql = "SELECT
					ppsid,
					ppsordem || '. ' || ppsdsc as descricao
			   FROM
			   		par.propostasubacao
			   WHERE
			   		indid = {$_REQUEST['indid']} AND 
			   		ppsid NOT IN ( select ppsid from par.subacao where sbastatus = 'A' AND aciid = {$_REQUEST['aciid']} AND indid = {$_REQUEST['indid']} )
			   	ORDER BY
			   		ppsordem";
	$dado = $db->carregar( $sql );
	$lista = array();
	
	if( is_array($dado) ){
		foreach( $dado as $dados ){
			$listaInstrumento = listaSubacaoExcusivoParaEntidade($dados['ppsid'], $_SESSION['par']['itrid']);
			if(is_array($listaInstrumento)){
				if( in_array($_SESSION['par']['inuid'],$listaInstrumento) ){
					$lista[] = array('codigo' => "<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"adicionarSubacao(".$dados['ppsid'].")\"><img src=\"/imagens/gif_inclui.gif\" border=0 title=\"Adicionar Subação\"></a></center>",	
												'descricao' => $dados['descricao']
												);
				}
			} else {
				$lista[] = array('codigo' => "<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"adicionarSubacao(".$dados['ppsid'].")\"><img src=\"/imagens/gif_inclui.gif\" border=0 title=\"Adicionar Subação\"></a></center>",	
												'descricao' => $dados['descricao']
												);
			}
		}
	}
	//dbg($lista,1);
	$cabecalho = array("Ação","Descrição");
	die( $db->monta_lista_array($lista,$cabecalho,50,5,'N',"center") ); 
	
}

if($_POST['filtraForm'] == 1){
	$displayTipoSubAcao = 'S';
	$frmid = $_REQUEST['frmid'];
	if(!$frmid || $frmid == '2' || $frmid == '4' ) $displayTipoSubAcao = 'N';
		
		if( $frmid == '12' || $frmid == '4' ){
			$where = "AND ptsid IN (46,42)";			
		}
	
		$arrTipoSubacao = array();
		if($frmid){
			$sql = "SELECT
						ptsid as codigo,
						ptsdescricao as descricao
					FROM
						par.propostatiposubacao
					WHERE
						frmid = $frmid
					{$where}
					AND
						ptsstatus = 'A'
					ORDER BY
						descricao";
			$arrTipoSubacao = $db->carregar($sql);
		}
		if($arrTipoSubacao){
			$diplay = "";
			$displayTipoSubAcao = "S";
		}else{
			$diplay = "none";
			$displayTipoSubAcao = "N";
		}
		
	if($arrTipoSubacao){
		echo '<tr id="tr_combo_tiposubaacao" style="display: <?php echo $diplay;?>;">
			<td class="SubTituloDireita">Tipo da subação:</td>
			<td id="td_combo_tiposubaacao">
					'.$db->monta_combo('ptsid', $arrTipoSubacao, 'S', 'Selecione...', '','', '','',$displayTipoSubAcao,'','',$ptsid).'
			</td>
		</tr>';
	}
	die();
}

function salvarSubacao(){
	global $db;

	$arCamposNulo = array();
	$oAcao = new Subacao();
	
	if($_POST['ptsid']){
		$sql = "select MAX( sbaordem )
					from
						par.subacao
					where
						sbastatus = 'A' AND aciid = ".$_POST['aciid'];
		
		$qnt = $db->pegaUm( $sql ) + 1;

		$estado = $db->pegaUm("SELECT d.esdid FROM par.instrumentounidade iu
							INNER JOIN workflow.documento d ON d.docid = iu.docid
							WHERE iu.inuid = ".$_SESSION['par']['inuid']);
			
		if( $estado == WF_ANALISE ){
			$desc = 'Em Análise';
			$esdid = WF_SUBACAO_ANALISE;
		} else {
			$desc = 'Em Elaboração';
			$esdid = WF_SUBACAO_ELABORACAO;
		}
		
		$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
				VALUES (62, ".$esdid.", '".$desc."', now()) returning docid ";
		$docid = $db->pegaUm($sql);
		
		if( empty($_POST['foaid'])  ) $arCamposNulo = array( 'foaid' ); else $oAcao->foaid = $_POST['foaid'];
		$oAcao->sbadsc = $_POST['sbadsc'];
		$oAcao->sbaordem = $qnt;
		$oAcao->frmid = $_POST['frmid'];
		$oAcao->ptsid = $_POST['ptsid'];
		$oAcao->indid = $_POST['indid'];
		$oAcao->aciid = $_POST['aciid'];
		if( $_POST['undid'] ){
			$oAcao->undid = $_POST['undid'];
		}
		$oAcao->prgid = 2;
		$oAcao->sbacronograma = $_POST['sbacronograma'];
		$oAcao->docid = $docid;
		$oAcao->sbaestrategiaimplementacao = $_POST['sbaestrategiaimplementacao'];
		$oAcao->salvar(true, true, $arCamposNulo);
		$oAcao->commit();
	
		echo "<script>
					alert('Operação realizada com sucesso!');
					window.location.href = '/par/par.php?modulo=principal/subacao&acao=A&sbaid=$oAcao->sbaid';
				</script>";
	} else {
		echo "<script>
					alert('Faltam dados!');
					window.location.href = window.reload;
				</script>";
		
	}
	exit;
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

$sql = "SELECT
			d.dimcod, 
			d.dimdsc, 
			a.arecod, 
			a.aredsc, 
			i.indcod, 
			i.inddsc 
		FROM
			par.dimensao d
		INNER JOIN par.area a ON a.dimid = d.dimid
		INNER JOIN par.indicador i ON i.areid = a.areid 
		WHERE
			i.indid = ".$_REQUEST['indid'];
$dados = $db->pegaLinha($sql);

$sql = "select acidsc from par.acao WHERE acistatus = 'A' AND aciid = ".$_REQUEST['aciid'];
$acao = $db->pegaUm($sql);
?>
<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR 2010 - Plano de Metas - Subação</title>
	
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body>
		<script type="text/javascript">
			function adicionarSubacao( ppsid ){
				if( ppsid ){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'adicionaSubacao=1&ppsid='+ppsid+'&indid='+<?=$_GET['indid']?>+'&aciid='+<?=$_GET['aciid']?>,
						  success	: function(res) {
						  		alert('Subação adicionada com sucesso!');
						  		window.opener.location.href = "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
						  		montaListadeValores();
						  }
					});
				} else {
					alert('Erro: Falta o ppsid!');
				}
			}
			
			function montaListadeValores(){
				jQuery.ajax({
					  type		: 'post',
					  url		: window.location,
					  data		: 'montaListadeValores=1&indid='+<?=$_REQUEST['indid']?>+'&aciid='+<?=$_REQUEST['aciid']?>,
					  success	: function(res) {
					  		jQuery( '#lista' ).html( res );
					  }
				});
			}
		
			function salvarSubacao()
			{
				var erro = 0;
				$("[class~=obrigatorio]").each(function() { 
					if(!this.value || this.value == "Selecione..."){
						if( $("[name=frmid]").val() == 6 ){
							if( !(this.name == 'undid') ){
								erro = 1;
								alert('Favor preencher todos os campos obrigatórios!');
								this.focus();
								return false;
							}
						} else {
							erro = 1;
							alert('Favor preencher todos os campos obrigatórios!');
							this.focus();
							return false;
						}
					}
				});
				if(erro == 0){
					$("#form_subacao").submit();
				}
			}
			function filtraTipoSubacao(frmid)
			{
				if(frmid){
					if( frmid == 6 ){
						jQuery('#tr_combo_unidademedida').hide();
					} else {
						jQuery('#tr_combo_unidademedida').show();
					}
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'filtraForm=1&frmid='+frmid+'&indid='+<?=$_REQUEST['indid']?>,
						  success	: function(res) {
						  		if(res){
									jQuery('#tr_combo_tiposubaacao').show();
									jQuery('[name=ptsid]').attr("class",'CampoEstilo obrigatorio');
									jQuery('#td_combo_tiposubaacao').html(res);
								}else{
									jQuery('[name=ptsid]').attr("class",'CampoEstilo');
									jQuery('#tr_combo_tiposubaacao').hide();
								}
						  }
					});
				}
			}
		</script>
		<form name="form_subacao" id="form_subacao" method="post" action="" >
			<input type="hidden" name="indid" value="<?php echo $_REQUEST['indid'] ?>" />
			<input type="hidden" name="aciid" value="<?php echo $_REQUEST['aciid'] ?>" />
			<input type="hidden" name="requisicao" value="salvarSubacao" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td colspan="2" style="color: blue; font-size: 22px">
						<a href="#">
							<?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
							<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
						</a>
					</td>
				</tr>
				<tr>
						<td class="SubTituloDireita">Dimensão:</td>
						<td><?php echo $dados['dimcod'] , '. ' , $dados['dimdsc'] ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Área:</td>
					<td><?php echo $dados['dimcod'] , '.' , $dados['arecod'] , '. ' , $dados['aredsc']; ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Indicador:</td>
					<td>
						<?php echo $dados['dimcod'] , '.' , $dados['arecod'] , '.' , $dados['indcod']   , '. ' , $dados['inddsc']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Ação:</td>
					<td><?php echo $acao; ?></td>
				</tr>
				<?php if( $_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 2 && recuperaGrandesMunicipios($_SESSION['par']['inuid']) ){ ?>
					<tr style="background-color: #cccccc;">
						<td align="left" colspan="2"><strong>Dados da Subação</strong></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Forma de execução:</td>
						<td>
							<?php $frmid = $oPropostaSubacao->frmid ? $oPropostaSubacao->frmid : $_GET['frmid'] ?>
							<?php
							$arrPerfil = pegaPerfilGeral();
							if($_SESSION['par']['itrid'] == 1){ //estado
								$perf = FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO;
							}elseif( $_SESSION['par']['itrid'] == 2 ){ //municipio
								$perf = FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO;
							}
							if( in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) ){
								$perf = FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO.", ".FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO;
							}
							
							$sql = "SELECT 
										frmid as codigo, 
										frmdsc as descricao 
									FROM par.formaexecucao
									WHERE
										frmid in (".FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA.", ".$perf.")
									ORDER BY frmdsc";

							$db->monta_combo('frmid', $sql, 'S', 'Selecione...', 'filtraTipoSubacao','','','','S');
							?>
						</td>
					</tr>
					<?
						$diplay = "none";
					?>
					<tr id="tr_combo_tiposubaacao" style="display: <?php echo $diplay;?>;">
						<td class="SubTituloDireita">Tipo da subação:</td>
						<td id="td_combo_tiposubaacao">
						</td>
					</tr>
					<tr>
						<td width="25%" class="SubtituloDireita" >Descrição da Subação:</td>
						<td><?php /*$sbadsc = $subacao['sbadsc'];*/ echo campo_textarea('sbadsc',"S","S","",90,5,"","") ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Estratégia de Implementação:</td>
						<td><?php /*$sbaestrategiaimplementacao = $subacao['sbaestrategiaimplementacao'];*/ echo campo_textarea('sbaestrategiaimplementacao',"S","S","",90,5,"","") ?></td>
					</tr>
					<!--<tr>
						<td class="SubTituloDireita">Programa:</td>
						<td>
							<?php $sql = "select prgid as codigo, prgdsc as descricao from par.programa where prgstatus = 'A' order by descricao"; ?>
							<?php /*$prgid = $subacao['prgid'];*/ $db->monta_combo("prgid",$sql,"S","Selecione...","","","","200px","S") ?>
						</td>
					</tr>
					-->
					<tr id="tr_combo_unidademedida">
						<td class="SubTituloDireita">Unidade de Medida:</td>
						<td>
							<?php $sql = "select undid as codigo, unddsc as descricao from par.unidademedida where undstatus = 'A' order by descricao"; ?>
							<?php $db->monta_combo("undid",$sql,"S","Selecione...","","","","","S") ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Cronograma:</td>
						<td>
							<?php $arrCronograma = array( 0 => array("codigo" => 1, "descricao" => "Global"), 1 => array("codigo" => 2, "descricao" => "Escola")  ) ?>
							<?php /*$ppscronograma = $subacao['ppscronograma'];*/ $db->monta_combo("sbacronograma",$arrCronograma,"S","Selecione...","","","","","S") ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita"></td>
						<td class="SubTituloEsquerda">
							<input type="button" value="Salvar" name="btn_salvar" onclick="salvarSubacao()" />
							<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
						</td>
					</tr>
				<?php } ?>
				<tr style="background-color: #cccccc;">
					<td align="center" colspan="2"><strong>Lista de Subações do Guia</strong></td>
				</tr></table><span id="lista">
					<?
					$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
					
					if( in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ){
						$filtro = "";
					} else {
						$filtro = "AND ppscarga = 't'
									AND frmid NOT IN ( 14, 15 )
									AND ppsid NOT IN ( select ppsid from par.subacao where sbastatus = 'A' AND aciid = {$_REQUEST['aciid']} AND indid = {$_REQUEST['indid']}  AND ppsid IS NOT NULL )";
					}
					
					$sql = "SELECT
									ppsid,
									ppsordem || '. ' || ppsdsc as descricao
							   FROM
							   		par.propostasubacao
							   WHERE
							   		indid = {$_REQUEST['indid']}
									$filtro
							   	ORDER BY
							   		ppsordem";

					$dado = $db->carregar( $sql );
					$lista = array();
					
					if( is_array($dado) ){
						foreach( $dado as $dados ){
							$listaInstrumento = listaSubacaoExcusivoParaEntidade($dados['ppsid'], $_SESSION['par']['itrid']);
							if(is_array($listaInstrumento)){
								if( in_array($_SESSION['par']['inuid'],$listaInstrumento) ){
									$lista[] = array('codigo' => "<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"adicionarSubacao(".$dados['ppsid'].")\"><img src=\"/imagens/gif_inclui.gif\" border=0 title=\"Adicionar Subação\"></a></center>",	
																'descricao' => $dados['descricao']
																);
								}
							} else {
								$lista[] = array('codigo' => "<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"adicionarSubacao(".$dados['ppsid'].")\"><img src=\"/imagens/gif_inclui.gif\" border=0 title=\"Adicionar Subação\"></a></center>",	
																'descricao' => $dados['descricao']
																);
							}
						}
					}
					//dbg($lista,1);
					$cabecalho = array("Ação","Descrição");
					$db->monta_lista_array($lista,$cabecalho,50,5,'N',"center"); 
					
					?>
					</span>
		</form>
	</body>
</html>

<?php } else { //SE VIER DO MENU PRINCIPAL

if( $_REQUEST['carregarDimensao'] ){
	
	$_SESSION['par']['cadSubacao']['itrid'] = $_REQUEST['itrid'];
	
	$sql = "SELECT 
				dimid as codigo, 
				substr(dimcod || '.' || dimdsc, 0, 200) as descricao 
			FROM 
				par.dimensao 
			WHERE 
				dimstatus = 'A' AND
				itrid = ".$_REQUEST['itrid']."
			ORDER BY
				dimcod";
	
	echo $db->monta_combo( "dimid", $sql, 'S', 'Selecione a Dimensão', 'carregaArea(this.value);', '', '', 500 );
	exit();
}

if( $_REQUEST['carregarArea'] ){
	$sql = "SELECT
				areid as codigo,
				substr(arecod || '.' || aredsc, 0, 200) as descricao
			FROM 
				par.area 
			WHERE 
				arestatus = 'A' AND
				dimid = ".$_REQUEST['dimid']."
			ORDER BY
				arecod";
	
	echo $db->monta_combo( "areid", $sql, 'S', 'Selecione a Área', 'carregaIndicador(this.value);', '', '', 500 );
	exit();
}

if( $_REQUEST['carregarIndicador'] ){
	$sql = "SELECT
				indid as codigo,
				substr(indcod || '.' || inddsc, 0, 200) as descricao
			FROM 
				par.indicador 
			WHERE 
				indstatus = 'A' AND
				areid = ".$_REQUEST['areid']."
			ORDER BY
				indcod";
	
	echo $db->monta_combo( "indid", $sql, 'S', 'Selecione o Indicador', 'carregaSubacao(this.value);', '', '', 500 );
	exit();
}

if( $_REQUEST['carregarSubacao'] ){
	$sql = "select 
				pps.ppsid as codigo,
				substr(pps.ppsordem || '.' || pps.ppsdsc, 0, 200) as descricao
			from 
				par.propostasubacao pps
			where 
				pps.indid = ".$_REQUEST['indid']."
			AND 
				pps.ppscronograma <> 2 
			ORDER BY
				pps.ppsordem";
	
	echo $db->monta_combo( "ppsid", $sql, 'S', 'Selecione a Subação', 'mostraGrupoEntidade( this.value );', '', '', 500 );
	exit();
}

if( $_REQUEST['carregarDadosSubacao'] ){
	$sql = "select
				ppsdsc,
				frmid, 
				ppsestrategiaimplementacao, 
				prgid, 
				undid, 
				ppscronograma, 
				ppsmonitora, 
				foaid,
				ptsid
			from 
				par.propostasubacao 
			where 
				ppsid = ".$_REQUEST['ppsid'];
	
	$dados = $db->pegaLinha( $sql );
	
	echo $dados['ppsdsc'].'|'.$dados['frmid'].'|'.$dados['ppsestrategiaimplementacao'].'|'.$dados['prgid'].'|'.$dados['undid'].'|'.$dados['ppscronograma'].'|'.$dados['ppsmonitora'].'|'.$dados['foaid'].'|'.$dados['ptsid'];	
	exit();
}

if( $_REQUEST['carregarTipoEstadoMunicipio'] ){
	
	if($_REQUEST['itrid'] == 2){
		
		$sqlComboIDEB = "select
							tpmid as codigo,
							tpmdsc as descricao
						from territorios.tipomunicipio
						where
							gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
							tpmstatus = 'A'";
		$sqlGrupoMunicipios = "select * from (
						(select 
							tpmid as codigo,
							case when tpmid = 140 then 'Municípios de até 10.000 habitantes' when tpmid = 141 then 'Municípios de 10.001 a 20.000 habitantes' else tpmdsc end as descricao
						from  
							territorios.tipomunicipio
						where 
							tpmid in (1, 16, 17, 140, 141, 150, 151, 152, 154)
						)								
						union
						(select 
							gtmid as codigo,
							gtmdsc as descricao 
						from  
							territorios.grupotipomunicipio
						where 
							gtmid = 5)
						order by descricao) as tbl";
		
		$dado = '<table>
					<tr>
						<td bgcolor="#e9e9e9" align="right">Municípios</td>
						<td>
							<select 
								multiple="multiple" 
								size="5" 
								name="municipios[]" 
					        	id="municipios"  
					        	ondblclick="abrepopupMunicipio();"  
					        	class="CampoEstilo" 
					        	style="width:400px;" >
				        		<option value="">Duplo clique para selecionar da lista</option>
					        </select>
						</td>
					</tr>
					<tr>
						<td bgcolor="#e9e9e9" align="right">Classificação IDEB</td>
						<td>
							<select 
								style="width: 400px;" 
								class="CampoEstilo" 
								onkeydown="javascript:combo_popup_remove_selecionados( event, \'ideb\' );" 
								ondblclick="javascript:combo_popup_abre_janela( \'ideb\', 215, 400, false );" 
								onclick="javascript:combo_popup_alterar_campo_busca( this );" 
								id="ideb" name="ideb[]" size="5" 
								multiple="multiple" tipo="combo_popup" maximo="0">
								<option value="">Duplo clique para selecionar da lista</option>
							</select>
						</td>
					</tr>
					<tr>
						<td bgcolor="#e9e9e9" align="right">Grupo de Municípios</td>
						<td>
							<select 
								style="width: 400px;" 
								class="CampoEstilo" 
								onkeydown="javascript:combo_popup_remove_selecionados( event, \'grupo\' );" 
								ondblclick="javascript:combo_popup_abre_janela( \'grupo\', 215, 400, false );" 
								onclick="javascript:combo_popup_alterar_campo_busca( this );" 
								id="grupo" 
								name="grupo[]" 
								size="5" 
								multiple="multiple" 
								tipo="combo_popup" maximo="0">
								<option value="">Duplo clique para selecionar da lista</option>
							</select>
						</td>
					</tr>
					<tr>
						<td bgcolor="#e9e9e9" align="right">Municípios de Mobiliário e Equipamentos</td>
						<td>
							<select 
								multiple="multiple" 
								size="5" 
								name="municipiosMobEquip[]" 
					        	id="municipiosMobEquip"  
					        	ondblclick="abrepopupMunicipioMobEquip();"  
					        	class="CampoEstilo" 
					        	style="width:400px;" >
				        		<option value="">Duplo clique para selecionar da lista</option>
					        </select>
						</td>
					</tr>
			</table>';
				
			echo $dado;
	} else {
		echo '<td bgcolor="#e9e9e9" align="right">Estados</td>
				<td>
				<select 
					multiple="multiple" 
					size="5" 
					name="estados[]" 
		        	id="estados"  
		        	ondblclick="abrepopupEstado();"  
		        	class="CampoEstilo" 
		        	style="width:400px;" >
	        		<option value="">Duplo clique para selecionar da lista</option>
		        </select>
			</td>';
	}
	exit();
}
	
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Incluir Subação - PAR', 'As subações poderão ser inseridas no sistema para um grupo de município a qualquer fase do WorkFlow' );
?>
<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR 2010 - Plano de Metas - Subação</title>
	
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body>
		<script type="text/javascript">
			function salvarSubacao(){
				var formulario = document.formulario;
			
				$('#salvaformulario').val(1);
				
				selectAllOptions( formulario.estados );
				selectAllOptions( formulario.ideb );
				selectAllOptions( formulario.grupo );
				selectAllOptions( formulario.municipiosMobEquip );
				
				$("#formulario").submit();
			}
			
			function carregaEstadoMunicipio( tpsubacao ){
				if( tpsubacao == 1 ){
					jQuery( '#trtipo' ).show();
					jQuery( '#trdimensao' ).show();
					jQuery( '#trarea' ).show();
					jQuery( '#trindicador' ).show();
					jQuery( '#trsubacao' ).show();
				} else {
					jQuery( '#trtipo' ).show();
					jQuery( '#trdimensao' ).show();
					jQuery( '#trarea' ).show();
					jQuery( '#trindicador' ).show();
					jQuery( '#trsubacao' ).show();
				}
				jQuery( '#grupoentidade' ).hide();
				jQuery( '#trexcel' ).hide();
				jQuery( '#tab2' ).hide();
				jQuery( '#tabAbasSelecaoCosano' ).hide();
				jQuery( '#area' ).html( "" );
		  		jQuery( '#indicador' ).html( "" );
		  		jQuery( '#subacao' ).html( "" );
		  		jQuery( '#dimensao' ).html( "" );
			}
			
			function carregaDimensao(itrid){
				if( itrid ){
			  		jQuery( '#itridHidden' ).val( itrid );
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'carregarDimensao=1&itrid='+itrid,
						  success	: function(res) {
						  		jQuery( '#area' ).html( "" );
						  		jQuery( '#indicador' ).html( "" );
						  		jQuery( '#subacao' ).html( "" );
						  		jQuery( '#dimensao' ).html( res );
						  		jQuery( '#grupoentidade' ).hide();
								jQuery( '#tab2' ).hide();
								jQuery( '#tabAbasSelecaoCosano' ).hide();
								jQuery( '#trexcel' ).hide();
						  }
					});
				} else {
					alert('Erro: Falta o itrid!');
				}
			}

			function carregaArea(dimid){
				if( dimid ){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'carregarArea=1&dimid='+dimid,
						  success	: function(res) {
						  		jQuery( '#indicador' ).html( "" );
						  		jQuery( '#subacao' ).html( "" );
						  		jQuery( '#area' ).html( res );
						  		jQuery( '#grupoentidade' ).hide();
						  		jQuery( '#trexcel' ).hide();
						  }
					});
				} else {
					alert('Erro: Falta o dimid!');
				}
			}
			
			function carregaIndicador(areid){
				if( areid ){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'carregarIndicador=1&areid='+areid,
						  success	: function(res) {
						  		jQuery( '#subacao' ).html( "" );
						  		jQuery( '#indicador' ).html( res );
						  		jQuery( '#grupoentidade' ).hide();
						  		jQuery( '#trexcel' ).hide();
						  }
					});
				} else {
					alert('Erro: Falta o areid!');
				}
			}

			function carregaSubacao(indid){
				if( indid ){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'carregarSubacao=1&indid='+indid,
						  success	: function(res) {
						  		jQuery( '#subacao' ).html( res );
						  }
					});
				} else {
					alert('Erro: Falta o indid!');
				}
			}
			
			function mostraGrupoEntidade( ppsid ){
			
				itrid = jQuery( '#itridHidden' ).val();
				jQuery( '#ppsid' ).val( ppsid );
				
				if( jQuery( 'input[name=tpsubacao]:checked' ).val() == 1 ){ // Se for do Guia
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'carregarTipoEstadoMunicipio=1&itrid='+itrid,
						  success	: function(res) {
								jQuery( '#grupoentidade' ).show();
						  		jQuery( '#tipoEstadoMunicipio' ).html( res );
								jQuery( '#tab2' ).show();
								jQuery( '#tabAbasSelecaoCosano' ).show();
						  }
					});
				} else { // Se for Manual
					jQuery( '#trexcel' ).show();
					jQuery( '#tab2' ).show();
					jQuery( '#tabAbasSelecaoCosano' ).show();
				}
				if( ppsid ){
					jQuery.ajax({
						type		: 'post',
						url			: window.location,
						data		: 'carregarDadosSubacao=1&ppsid='+ppsid,
						success		: function(res) {
							dados = res;
		   	    			dados = dados.split('|');
		   	    			
		   	    			jQuery( '#ppsdsc').val( dados[0] );
		   	    			jQuery( '#ppsdsc').attr( 'readonly', 'readonly' );
							jQuery( '[name=frmid]').val( dados[1] ); 
							jQuery( '[name=frmid]').attr('disabled','disabled');
							jQuery( '[name=hdn_frmid]').val( dados[1] ); 
							jQuery( '#ppsestrategiaimplementacao').val( dados[2] );
							jQuery( '#ppsestrategiaimplementacao').attr( 'readonly', 'readonly' );
							jQuery( '[name=prgid]').val( dados[3] );
							jQuery( '[name=prgid]').attr('disabled','disabled');
							jQuery( '[name=hdn_prgid]').val( dados[3] ); 
							jQuery( '[name=undid]').val( dados[4] );
							jQuery( '[name=undid]').attr('disabled','disabled');
							jQuery( '[name=hdn_undid]').val( dados[4] ); 
							jQuery( '[name=cronograma][value=' + dados[5] + ']').attr('checked','checked');
							jQuery( '[name=cronograma]').attr('disabled','disabled');
							jQuery( '[name=hdn_cronograma]' ).val( dados[5] );
							if( jQuery('[name=hdn_cronograma]').val() == 2 ){
								alert('Essa subação é por escola! Por favor selecione uma global!');
								jQuery('[name=ppsid]').focus();
							}
							jQuery( '[name=monitoramento][value=' + dados[6] + ']').attr('checked','checked');
							jQuery( '[name=monitoramento]').attr('disabled','disabled');
							jQuery( '[name=hdn_monitoramento]' ).val( dados[6] );
							jQuery( '[name=foaid]').val( dados[7] ); 
							jQuery( '[name=foaid]').attr('disabled','disabled');
							jQuery( '[name=hdn_foaid]').val( dados[7] ); 
							jQuery( '[name=ptsid]').val( dados[8] ); 
							jQuery( '[name=ptsid]').attr('disabled','disabled');
							jQuery( '[name=hdn_ptsid]').val( dados[8] ); 
							
							jQuery('[name^="sbdptres"]').val('');
							
							if( dados[3] != '' ) filtraPlanoInterno( dados[3] );
		   	    			
						}
					});
				}
			}
			
			function filtraPlanoInterno(prgid) {
				
				jQuery.ajax({
				   type: "POST",
				   url: window.location,
				   data: "requisicaoAjax=filtraPlanoInterno&" + "prgid=" + prgid,
				   success: function(resp){
						jQuery("#td_plano_"+<?php echo date("Y"); ?>).html(resp);
					}
				});
			}
			
			function filtraPTRES(plicod) {
				if(plicod!=''){
					
					jQuery.ajax({
					   type: "POST",
					   url: window.location,
					   data: "requisicaoAjax=filtraPTRES&" + "sbdplanointerno=" + plicod,
					   success: function(resp){
						   jQuery("#td_ptres_"+<?php echo date("Y"); ?>).html(resp);
						}
					});
				} else {
					$('[name^="sbdptres"]').val('');
				}
			}
			
			function abrepopupMunicipio(){
				window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_municipios_bandalarga.php','Municipios','width=400,height=400,scrollbars=1');
			}
			
			function abrepopupMunicipioMobEquip(){
				window.open('http://<?=$_SERVER['SERVER_NAME']?>/par/combo_municipios_mob_equip.php?ppsid='+$('[name="ppsid"]').val(),'Municipios','width=650,height=500,scrollbars=1');
			}

			function abrepopupEstado(){
				window.open('http://<?=$_SERVER['SERVER_NAME']?>/par/par.php?modulo=principal/popupEstados&acao=A','Estados','width=400,height=400,scrollbars=1');
			}
			
			function selecionarAba(cosano)
			{
            	$('[id^="abaAnos"]').attr("style","display:none");
            	$('[id^="aba_"]').attr("style","width:50px;margin: 0;padding: 0 0 0 5px;background: url(\"../../imagens/left_both.gif\") no-repeat left top;border-bottom: 1px solid #765;background-position: 0% 0%");
            	$('[id^="aba_"] a').attr("style","display: block;background: url(\"../../imagens/right_both.gif\") no-repeat right top;padding: 5px 0 4px 0;text-decoration: none;font-weight: bold;color: #765;width: auto;font-size:12px");
            	$('#spanAno').innerHTML = cosano;
        		$('#abaAnos' + cosano).attr("style","display:table-row");
				$('#aba_' + cosano).attr("style","width:50px;background-position:0% -150px;color: #333;");
				$('#aba_' + cosano + ' a:first').attr("style","background-position:100% -150px;color: #333;");
				$('#anoatual').val(cosano);
    		}
    		
    		$(function() {
				if($('#anoatual').val()){
					selecionarAba($('#anoatual').val());
				} else {
					selecionarAba('<?php echo date("Y"); ?>');
					$('#anoatual').val('<?php echo date("Y"); ?>');
				}
			});
			
			function inserirItemCompsicao(ano,sbaid,sbacronograma)
    		{
    			ppsid = jQuery( '#ppsid' ).val();
    			sbacronograma = $("[name=hdn_cronograma]").val();
    			if( sbacronograma == 2 ){
	    			escolas = $('[name=escolas['+ano+']]').val();
	    			if( escolas == 0 ){
	    				alert('Por favor, cadastre pelo menos uma escola!');
		    			//return false;
	    			} else {
	    				url = "par.php?modulo=principal/subacaoItemComposicao&acao=A&ppsid=" + ppsid + "&ano=" + ano + "&sbacronograma=" + sbacronograma;
		    			janela(url,700,550,"Item de Composição");
	    			}
    			} else {
    				url = "par.php?modulo=principal/subacaoItemComposicao&acao=A&ppsid=" + ppsid + "&ano=" + ano + "&sbacronograma=" + sbacronograma;
		    		janela(url,700,550,"Item de Composição");
    			}
    		}
    		function inserirEscolas(ano,sbaid)
    		{
    			itrid = jQuery( '#itridHidden' ).val();
    			ppsid = jQuery( '#ppsid' ).val();
    			url = "par.php?modulo=principal/subacaoEscolas&acao=A&itrid=" + itrid + "&ano=" + ano + "&ppsid=" + ppsid;
    			janela(url,700,550,"Escolas da Subação");
    		}
    		function inserirBeneficiarios(ano)
    		{
    			ppsid = jQuery( '#ppsid' ).val();
    			url = "par.php?modulo=principal/subacaoBeneficiarios&acao=A&ppsid=" + ppsid + "&ano=" + ano;
    			janela(url,700,550,"Beneficiários da Subação");
    		}
			
		/*
			function adicionarSubacao( ppsid ){
				if( ppsid ){
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'adicionaSubacao=1&ppsid='+ppsid+'&indid='+<?=$_REQUEST['indid']?>+'&aciid='+<?=$_REQUEST['aciid']?>,
						  success	: function(res) {
						  		alert('Subação adicionada com sucesso!');
						  		window.opener.location.href = "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
						  		montaListadeValores();
						  }
					});
				} else {
					alert('Erro: Falta o ppsid!');
				}
			}
			
			function montaListadeValores(){
				jQuery.ajax({
					  type		: 'post',
					  url		: window.location,
					  data		: 'montaListadeValores=1&indid='+<?=$_REQUEST['indid']?>+'&aciid='+<?=$_REQUEST['aciid']?>,
					  success	: function(res) {
					  		jQuery( '#lista' ).html( res );
					  }
				});
			}
		
			function salvarSubacao()
			{
				var erro = 0;
				$("[class~=obrigatorio]").each(function() { 
					if(!this.value || this.value == "Selecione..."){
						if( $("[name=frmid]").val() == 6 ){
							if( !(this.name == 'undid') ){
								erro = 1;
								alert('Favor preencher todos os campos obrigatórios!');
								this.focus();
								return false;
							}
						} else {
							erro = 1;
							alert('Favor preencher todos os campos obrigatórios!');
							this.focus();
							return false;
						}
					}
				});
				if(erro == 0){
					$("#form_subacao").submit();
				}
			}
			function filtraTipoSubacao(frmid)
			{
				if(frmid){
					if( frmid == 6 ){
						jQuery('#tr_combo_unidademedida').hide();
					} else {
						jQuery('#tr_combo_unidademedida').show();
					}
					jQuery.ajax({
						  type		: 'post',
						  url		: window.location,
						  data		: 'filtraForm=1&frmid='+frmid+'&indid='+<?=$_REQUEST['indid']?>,
						  success	: function(res) {
						  		if(res){
									jQuery('#tr_combo_tiposubaacao').show();
									jQuery('[name=ptsid]').attr("class",'CampoEstilo obrigatorio');
									jQuery('#td_combo_tiposubaacao').html(res);
								}else{
									jQuery('[name=ptsid]').attr("class",'CampoEstilo');
									jQuery('#tr_combo_tiposubaacao').hide();
								}
						  }
					});
				}
			} */
		</script>
		<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="">
			<input type="hidden" name="indid" value="<?php echo $_REQUEST['indid'] ?>" />
			<input type="hidden" name="aciid" value="<?php echo $_REQUEST['aciid'] ?>" />
			<input type="hidden" name="ppsid" id="ppsid" value="" />
			<input type="hidden" name="itridHidden" id="itridHidden" value="" />
			<input type="hidden" name="requisicao" value="salvarSubacao" />
			<input type="hidden" name="salvaformulario" id="salvaformulario" value="" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<colgroup>
					<col width="25%" />
					<col width="75%" />
				</colgroup>
				<tr>
					<td class="SubTituloDireita">Carga da Subação:</td>
					<td><input type="radio" name="tpsubacao" onclick="javascript:carregaEstadoMunicipio(this.value);" value="1">do Guia<input type="radio" name="tpsubacao" onclick="javascript:carregaEstadoMunicipio(this.value);" value="2">Manual</td>
				</tr>
				<tr id="trtipo" style="display: none;">
					<td class="SubTituloDireita">Tipo de Entidade:</td>
					<td><input type="radio" name="tipo" onclick="javascript:carregaDimensao(this.value);" value="1">Estadual<input type="radio" name="tipo" onclick="javascript:carregaDimensao(this.value);" value="2">Municipal</td>
				</tr>
				<tr id="trdimensao" style="display: none;">
						<td class="SubTituloDireita">Dimensão:</td>
						<td id="dimensao"></td>
				</tr>
				<tr id="trarea" style="display: none;">
					<td class="SubTituloDireita">Área:</td>
					<td id="area"></td>
				</tr>
				<tr id="trindicador" style="display: none;">
					<td class="SubTituloDireita">Indicador:</td>
					<td id="indicador"></td>
				</tr>
				<tr id="trsubacao" style="display: none;">
					<td class="SubTituloDireita">Subação:</td>
					<td id="subacao"></td>
				</tr>
				<tr id="grupoentidade" style="display: none;">
					<td class="SubTituloDireita">Grupo de Entidade:</td>
					<td align="left">
						<table>
							<tr id="tipoEstadoMunicipio"></tr>
						</table>
					</td>
				</tr>
				<tr id="trexcel" style="display: none;">
					<td class="SubTituloDireita">Anexar Excel:<br>Formato do Arquivo: <b>Código IBGE (Municipal) ou UF (Estadual), Código do Item, Quantidade, Ano</b></td>
					<td align="left">
						<input type="file" name="arquivo" id="arquivoEx">
					</td>
				</tr>
				</table>
				<table id="tab2" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="display: none;">
					<tr bgcolor="#e9e9e9" align="center">
						<td colspan="2"><b>Preenchimento da subação</b></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Tipo da Subação:</td>
						<td>
							<?php
							$sql = "SELECT 
										ptsid as codigo, 
										ptsdescricao as descricao 
									FROM
										par.propostatiposubacao 
									WHERE
										ptsstatus = 'A' 
									ORDER BY ptsdescricao";
				 
							$db->monta_combo('ptsid', $sql, 'S', 'Selecione...', '','', '','','N','','',$ptsid);
							?>
							<input type="hidden" id="hdn_ptsid" name="hdn_ptsid" value=""> 
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Descrição da subação:</td>
						<td>
							<?php echo campo_textarea('ppsdsc', 'S', 'S', '', 70, 6, '','', 0, '', false, null, $ppsdsc) ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Forma de execução:</td>
						<td>
							<?php $frmid = $oPropostaSubacao->frmid ? $oPropostaSubacao->frmid : $_GET['frmid'] ?>
							<?php
							$sql = "SELECT 
										frmid as codigo, 
										frmdsc as descricao 
									FROM par.formaexecucao
									WHERE frmid NOT IN (14, 15)
									ORDER BY frmdsc";
	 
							$db->monta_combo('frmid', $sql, 'S', 'Selecione...', 'filtraTipoSubacao','','','','S');
							?>
							<input type="hidden" id="hdn_frmid" name="hdn_frmid" value=""> 
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width:250px;">Estratégia de implementação:</td>
						<td>
							<?php $ppsestrategiaimplementacao = $oPropostaSubacao->ppsestrategiaimplementacao ?>
							<?php echo campo_textarea('ppsestrategiaimplementacao', 'S', 'S', '', 70, 6, '','', 0, '', false, null, $ppsestrategiaimplementacao) ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Programa:</td>
						<td>
							<?php
							$sql = "SELECT 
										prgid AS codigo, 
										prgdsc AS descricao
									FROM par.programa
									WHERE prgstatus = 'A'
									ORDER BY prgdsc";
							$db->monta_combo('prgid', $sql, 'S', 'Selecione...', '','', '','200px','S','','',$prgid);
							?>
							<input type="hidden" id="hdn_prgid" name="hdn_prgid" value=""> 
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Unidade de medida:</td>
						<td>
							<?php
							$sql = "SELECT 
										undid as codigo , 
										unddsc as descricao
									FROM par.unidademedida
									WHERE
										undstatus = 'A'
									ORDER BY unddsc";
							$db->monta_combo('undid', $sql, 'S', 'Selecione...', '','', '','','S','','',$undid);
							?>
							<input type="hidden" id="hdn_undid" name="hdn_undid" value=""> 
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Cronograma:</td>
						<td>
							<input type="radio" id="global" name="cronograma" value="1" <?=($oPropostaSubacao->ppscronograma == 1) ? 'checked' : '' ?>>Global
							<input type="radio" id="porescola" name="cronograma" value="2" <?=($oPropostaSubacao->ppscronograma == 2) ? 'checked' : '' ?>>Por escola 
							<input type="hidden" id="hdn_cronograma" name="hdn_cronograma" value=""> 
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Subação Extraordinária:</td>
						<td>
							<input type="radio" id="ext_sim" name="extraordinaria" value="sim"> Sim
							<input type="radio" id="ext_nao" name="extraordinaria" value="nao" checked> Não 
							<input type="hidden" id="hdn_extraordinaria" name="hdn_extraordinaria" value=""> 
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Passível de monitoramento técnico:</td>
						<td>
							<input type="radio" id="monitoramentosim" name="monitoramento" value="t" >Sim
							<input type="radio" id="monitoramentonao" name="monitoramento" value="f" >Não
							<input type="hidden" id="hdn_monitoramento" name="hdn_monitoramento" value=""> 
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Forma de atendimento:</td>
						<td>
							<?php
							$sql = "SELECT 
										foaid as codigo , 
										foadescricao as descricao
									FROM par.formaatendimento
									WHERE foastatus = 'A'
									ORDER BY foadescricao";
				 
							$db->monta_combo('foaid', $sql, 'S', 'Selecione...', '','', '','','N','','',$foaid);
							?>
							<input type="hidden" id="hdn_foaid" name="hdn_foaid" value=""> 
						</td>
					</tr>
				</table>
				
				
				
				
				
				
				<?php 
					$anos = array( 2011, 2012, 2013, 2014, 2015 );
					$arrMeses = array(
											0 =>  array("codigo" => 1,  "descricao" => "Janeiro"),
											1 =>  array("codigo" => 2,  "descricao" => "Fevereiro"),
											2 =>  array("codigo" => 3,  "descricao" => "Março"),
											3 =>  array("codigo" => 4,  "descricao" => "Abril"),
											4 =>  array("codigo" => 5,  "descricao" => "Maio"),
											5 =>  array("codigo" => 6,  "descricao" => "Junho"),
											6 =>  array("codigo" => 7,  "descricao" => "Julho"),
											7 =>  array("codigo" => 8,  "descricao" => "Agosto"),
											8 =>  array("codigo" => 9,  "descricao" => "Setembro"),
											9 =>  array("codigo" => 10, "descricao" => "Outubro"),
											10 => array("codigo" => 11, "descricao" => "Novembro"),
											11 => array("codigo" => 12, "descricao" => "Dezembro")
											);
				?>
				
				
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano" style="display: none;">
					<tr bgcolor="#e9e9e9" align="center">
						<td colspan="10"><b>Detalhamento por ano que compõem a subação</b></td>
					</tr>
					<tr id="abasSelecaoCosano">
						<?php
						$colspan 		= 5;		
						$tamanhoCelula 	= round(100 / $colspan);
						foreach( $anos as $ano ){
							echo "<td id=\"aba_$ano\" style=\"width: 50px;\" ><a href=\"javascript:selecionarAba('$ano')\">$ano</a></td>";
						}
						?>
					</tr>
					<?php foreach( $anos as $ano ){ ?>
						<tr id="abaAnos<?php echo $ano; ?>" class="abaAnos">
							<td colspan="<?php echo $colspan; ?>" >
								
								<?php //$oSbd = new SubacaoDetalhe(); ?>
								<?php //$sbdid = $oSbd->retornaDetalheSubacao($ano,$sbaid); ?>
								<input type="hidden" name="sbdid[<?php echo $ano ?>]" value="<?php echo $sbdid ?>" />
								<input type="hidden" name="sbaid[<?php echo $ano ?>]" value="<?php echo $sbaid ?>" />
								<input type="hidden" name="sbdano[<?php echo $ano ?>]" value="<?php echo $ano ?>" />
								<?php $sbdid ? $oSbd->carregarPorId($sbdid) : ""; ?>
								
								<table class="tabela tabelaExecucaoAnos" border="0" cellspacing="0" style="width: 100%;">
									<tr bgcolor="#e9e9e9">
										<td colspan="2" class="SubTituloTabela"><b>Quantidades e Cronograma de Execução</b></td>
									</tr>
									<tr id="cronogramaGlobal_<?php echo $ano; ?>">
										<td class="SubTituloDireita">Quantidade:</td>
										<td>
											<?php echo campo_texto("sbdquantidade[$ano]","N","S","Quantidade","20","20","[.###]","","","","","sbdquantidade_$ano","", $oSbd->sbdquantidade ? number_format($oSbd->sbdquantidade,0,2,'.') : "" ) ?>
										</td>
									</tr>
									<? //endif; ?>
									<tr>
										<td class="SubTituloDireita">Cronograma Físico:</td>
										<td>
											<?php $db->monta_combo("sbdinicio[$ano]",$arrMeses,"S","Selecione...","","","","","N","sbdinicio_$ano","",$oSbd->sbdinicio) ?>
											a
											<?php $db->monta_combo("sbdfim[$ano]",$arrMeses,"S","Selecione...","","","","","N","sbdfim_$ano","",$oSbd->sbdfim) ?>
											 <span style="margin-left:50px;font-weight:bold">Ano de Término:</span> 
											<?php echo campo_texto("sbdanotermino[$ano]","N","S","Ano de Término","5","4","[#]","","","","","sbdanotermino_$ano","",$oSbd->sbdanotermino,"verificaAnoValido(this,$ano)") ?>
										</td>
									</tr>
									<tr>
										<td class="SubTituloDireita" width="40%">Plano Interno:</td>
										 <td id="td_plano_<?php echo $ano; ?>">
											<?php //filtraPlanoInterno
											if( $_REQUEST['prgid'] ){
												$sqlPlanos = "
													SELECT 	DISTINCT
															plinumplanointerno as codigo,
															plinumplanointerno as descricao
													FROM
														par.planointerno
													WHERE pliano = '".$ano."'  AND prgid = ".$_REQUEST['prgid'];
											} else {
												$sqlPlanos = array();
											}
											
											$db->monta_combo( "sbdplanointerno[$ano]", $sqlPlanos , 'S', 'Selecione', 'filtraPTRES', '', '','','N', 'sbdplanointerno', false, $sbdplanointerno, 'Plano Interno' ); ?>
										</td>
									</tr>
									<tr>
								        <td class="SubTituloDireita">PTRES:</td>
								        <td id="td_ptres_<?php echo $ano; ?>">
								        	<?php 
								        	$sql = "select
													pliptres as codigo,
													pliptres as descricao
												from par.planointerno
												where plinumplanointerno = '{$sbdplanointerno}'";

								        	$db->monta_combo("sbdptres[$ano]",$sql, 'S',"Selecione...",'',"","","","N","","",$sbdptres,"PTRES"); ?>
								        </td>
									</tr>
									<tr bgcolor="#e9e9e9">
										<td colspan="2" class="SubTituloTabela"><b>Itens de Composição</b></td>
									</tr>
									<tr>
										<td id="itens_<?php echo $ano; ?>" colspan="2" >
										<?php //$oIC = new SubacaoItensComposicao();?>
										<?php //$oIC->montaLista($_GET['sbaid'],$ano, $subacao['sbacronograma'])?>
										<input type="hidden" name="itens[<?php echo $ano ?>]" value="<?php //echo $oIC->conta($_GET['sbaid'],$ano); ?>" />
										</td>
									</tr>
									<tr>
										<td class="SubTituloEsquerda" colspan="2" >
											<img class="middle link" onclick="inserirItemCompsicao('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>','<?php echo $subacao['sbacronograma'] ?>')"  src="../imagens/gif_inclui.gif" /> <a href="javascript:inserirItemCompsicao('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>','<?php echo $subacao['sbacronograma'] ?>')">Editar / Inserir Itens de Composição</a>
										</td>
									</tr>
									<!--<tr bgcolor="#e9e9e9">
										<td colspan="2" class="SubTituloTabela"><b>Beneficiários</b></td>
									</tr>
									<tr>
										<td id="beneficiarios_<?php echo $ano; ?>" colspan="2" >
											<?php //$oBen = new SubacaoBeneficiario();?>
											<?php //$oBen->montaLista($_GET['sbaid'],$ano)?>
											<input type="hidden" name="beneficiarios[<?php echo $ano ?>]" value="<?php //echo $oBen->conta($_GET['sbaid'],$ano); ?>" />
										</td>
									</tr>
									<tr>
										<td class="SubTituloEsquerda" colspan="2" >
											<img class="middle link" onclick="inserirBeneficiarios('<?php echo $ano ?>')"  src="../imagens/gif_inclui.gif" /> <a href="javascript:inserirBeneficiarios('<?php echo $ano ?>')">Editar / Inserir Beneficiários</a>
										</td>
									</tr>-->
									<tr bgcolor="#e9e9e9">
										<td colspan="2" class="SubTituloTabela"><b>Detalhamento</b></td>
									</tr>
									<tr id="parecer_<?php echo $ano; ?>">
										<td class="SubTituloDireita">Detalhamento</td>
										<td><?php echo campo_textarea("sbddetalhamento[$ano]", 'S', '', '', 100, 12, 0 ,'','','','','', $oSbd->sbddetalhamento); ?></td>
									</tr>
										<tr bgcolor="#e9e9e9">
											<td colspan="2" class="SubTituloTabela"><b>Parecer da Equipe Técnica</b></td>
										</tr>
										<tr id="parecer_<?php echo $ano; ?>">
											<td class="SubTituloDireita">Parecer:</td>
											<td><?php echo campo_textarea("sbdparecer[$ano]", 'N', '', '', 100, 12, 0 ,'','','','','',$oSbd->sbdparecer); ?></td>
										</tr>
										<tr id="parecerdemerito_<?php echo $ano; ?>">
											<td class="SubTituloDireita">Parecer de Mérito:</td>
											<td>
												<?php echo campo_textarea("sbdparecerdemerito[$ano]", 'N', 'S', '', 100, 12, 0 ,'','','','','',$oSbd->sbdparecerdemerito); ?>
											</td>
										</tr>
										<tr id="status_<?php echo $ano; ?>">
											<td class="SubTituloDireita">Status da Subação:</td>
											<td>
												<?php 	
														/*** Lista de Status da Subação ***/
														$sqlStatusSubacao = "SELECT 
																				ssuid AS codigo,
																				ssudescricao AS descricao 
																			FROM 
																				par.statussubacao 
																			WHERE 
																				ssutipostatus = 'E'";
			
														$db->monta_combo("ssuid[$ano]", $sqlStatusSubacao,'','Selecione','','','','','','ssuid_' . $ano ,'',$oSbd->ssuid); 
												?>
											</td>
										</tr>
								</table>
							</td>
						</tr>
					<? } // Fim do foreach anos ?>
					<tr>
						<td class="SubTituloDireita"></td>
						<td class="SubTituloEsquerda" colspan="5">
							<input type="button" value="Salvar" name="btn_salvar" onclick="salvarSubacao()" />
						</td>
					</tr>
				</table>
				
				
				
				
		</form>
	</body>
</html>

<?php }?>