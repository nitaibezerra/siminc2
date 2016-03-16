<?	
if($grupoitem) {
	
		// Caso o grupo tenha agrupadores de de coluna, imprime os agrupadores
		if($agrupadorescoluna) {
			/*
			 * INÍCIO
			 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS  
			 */
			
			// Verifica se o grupo item possui uma coluna totalizadora
			if($grupoitem['gitpossuitotalcoluna']=='t') {
				// a mascara do totalizador será igual a mascara da primeira coluna, do primeiro agrupador
				$mask = current($coluna[$agrupadorescoluna[0]['agpid']]);
				$mask = $mask['tpimascara'];
				
				$agrupadorescoluna[] = array("agpid" => "total");
				$coluna["total"][] = array("coldsc" => "TOTAL", "tpitipocampo" => "textpossuitotalcoluna", "tpimascara" => $mask, "colobs" => "Total geral");
			}
			foreach($agrupadorescoluna as $agpcoluna) {
				// Verifica se o agrupador possui totalizador, obs: totalizar somente od itens do agrupador, e não todos
				if($agpcoluna['agppossuitotal']=='t') {
					$mask = current($coluna[$agpcoluna['agpid']]);
					$mask = $mask['tpimascara'];
					
					$coluna[$agpcoluna['agpid']][] = array("coldsc" => "TOTAL", "tpitipocampo" => "textpossuitotalagrupador", "agpid" => $agpcoluna['agpid'], "tpimascara" => $mask, "colobs" => "Total do agrupador",);
				}
			}
			
			/*
			 * FIM
			 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS  
			 */
			
		} elseif($coluna) { // Caso não tenha agrupadores, imprimir as colunas diretamente
			/*
			 * INÍCIO
			 * FORMATO DE COLUNA : COLUNAS FIXAS SEM SUBNIVEIS  
			 */
			
			// Verifica se o grupo item possui uma coluna totalizadora (coluna no final com o total de todas as colunas)
			if($grupoitem['gitpossuitotalcoluna']=='t') {
				// a mascara do totalizador será igual a mascara da primeira coluna
				$mask = current($coluna);
				$mask = $mask['tpimascara'];
				
				$coluna[] = array("coldsc" => "TOTAL", 
								  "tpitipocampo" => "textpossuitotalcoluna",
								  "colobs" => "Total geral",
								  "tpimascara" => $mask);
			}
			/*
			 * FIM
			 * FORMATO DE COLUNA : COLUNAS FIXAS SEM SUBNIVEIS  
			 */
		} elseif($colunapa) {
			/*
			 * INÍCIO
			 * FORMATO DE COLUNA : COLUNAS FIXAS REFERENTES AOS ANOS DA TABELA  
			 */
			
			// Verifica se o grupo item possui uma coluna totalizadora
			if($grupoitem['gitpossuitotalcoluna']=='t') {
				// a mascara do totalizador será igual a mascara da primeira coluna
				$mask = current($colunapa);
				$mask = $mask['tpimascara'];
				
				$colunapa[] = array("coldsc" => "TOTAL", 
									"tpitipocampo" => "textpossuitotalcoluna",
									"colobs" => "Total geral",
									"tpimascara" => $mask);
			}
			/*
			 * FIM
			 * FORMATO DE COLUNA : COLUNAS FIXAS REFERENTES AOS ANOS DA TABELA  
			 */
		}


	/*
	 * INÍCIO
	 * FORMATO DE LINHA : LINHAS FIXAS COM SUBNIVEIS
	 * Caso tenha agrupadores por linha
	 */

	if($agrupadoreslinha) {
		
		// varrendo as linhas fixas cadastradas
		foreach($agrupadoreslinha as $agplinha) {
			// se o agrupador de linha tiver sublinhas (o normal é ter linhas dentro do agrupador)
			if(count($linhas[$agplinha['agpid']]) > 0) {
				// definindo campos da linha (varrendo os dados de coluna)
				if($agrupadorescoluna) { // verifica se o formato de coluna é 'fixas com subniveis'
					foreach($agrupadorescoluna as $agpcoluna) {
						if($coluna[$agpcoluna['agpid']]) {
							foreach($coluna[$agpcoluna['agpid']] as $col) {
								/*
								// 	verificando se pode existir obs
								unset($campoobs);
								if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
									$campoobs = definirobservacao($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']);
								}
								*/
								echo "<td class='styleconteudo'>".definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano'])." ".$campoobs."</td>";
							}
						}
					}
				} elseif($coluna) { // verifica se o formato de coluna é 'fixas sem subniveis'
					foreach($coluna as $col) {
						
						if($periodopa[$_REQUEST['ano']]) {
							foreach($periodopa[$_REQUEST['ano']] as $perdados) {
								unset($dadosperiodopa[$_REQUEST['ano']]);
								$dadosperiodopa[$_REQUEST['ano']][] = $perdados;
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
								$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
								$xls->MontaConteudoString($linexls, 7, $agplinha['agpdsc']);
								$xls->MontaConteudoString($linexls, 8, $linhas[$agplinha['agpid']][0]['lindsc']);
								$xls->MontaConteudoString($linexls, 9, "");
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano']));
								$linexls++;
							}
						} else {
							
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
								$xls->MontaConteudoString($linexls, 6, "");
								$xls->MontaConteudoString($linexls, 7, $agplinha['agpdsc']);
								$xls->MontaConteudoString($linexls, 8, $linhas[$agplinha['agpid']][0]['lindsc']);
								$xls->MontaConteudoString($linexls, 9, "");
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano']));
								$linexls++;
						}
					}
				} elseif($colunapa) {// verifica se o formato de coluna é 'fixas refentes ao ano'
					foreach($colunapa as $col) {
						if($linhas[$agplinha['agpid']][0]['lintipocampo']) {
							$col['tpitipocampo'] = $linhas[$agplinha['agpid']][0]['lintipocampo'];
						}
						
						
						
						if($periodopa[$col['coldsc']]) {
							foreach($periodopa[$col['coldsc']] as $perdados) {
								unset($dadosperiodopa[$col['coldsc']]);
								$dadosperiodopa[$col['coldsc']][] = $perdados;
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
								$xls->MontaConteudoString($linexls, 7, strtoupper($agplinha['agpdsc']));
								$xls->MontaConteudoString($linexls, 8, $linhas[$agplinha['agpid']][0]['lindsc']);
								$xls->MontaConteudoString($linexls, 9, "");
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']));
								$linexls++;
							}
						} else {
							
							$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
							$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
							$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
							$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
							$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
							$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 6, "");
							$xls->MontaConteudoString($linexls, 7, strtoupper($agplinha['agpdsc']));
							$xls->MontaConteudoString($linexls, 8, $linhas[$agplinha['agpid']][0]['lindsc']);
							$xls->MontaConteudoString($linexls, 9, "");
							$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']));
							$linexls++;
						}

					}
				}
				unset($linhas[$agplinha['agpid']][0]);				
				// fim da primeira linha com o agrupador
				
				// imprime as demais linhas contidas no agrupador
				foreach($linhas[$agplinha['agpid']] as $sublinha) {
					if($agrupadorescoluna) {
						foreach($agrupadorescoluna as $agpcoluna) {
							if($coluna[$agpcoluna['agpid']]) {
								foreach($coluna[$agpcoluna['agpid']] as $col) {
									echo "<td class='styleconteudo'>".definircamporelatorio($col,$sublinha['linid'],$_REQUEST['ano'])." ".$campoobs."</td>";
								}
							}
						}
					} elseif($coluna) {
						foreach($coluna as $col) {
							
							if($periodopa[$_REQUEST['ano']]) {
								foreach($periodopa[$_REQUEST['ano']] as $perdados) {
									unset($dadosperiodopa[$_REQUEST['ano']]);
									$dadosperiodopa[$_REQUEST['ano']][] = $perdados;
									$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
									$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
									$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
									$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
									$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
									$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
									$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
									$xls->MontaConteudoString($linexls, 7, strtoupper($agplinha['agpdsc']));
									$xls->MontaConteudoString($linexls, 8, $sublinha['lindsc']);
									$xls->MontaConteudoString($linexls, 9, "");
									$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
									$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$sublinha['linid'],$_REQUEST['ano']));
									$linexls++;
								}
							} else {
								
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
								$xls->MontaConteudoString($linexls, 6, "");
								$xls->MontaConteudoString($linexls, 7, strtoupper($agplinha['agpdsc']));
								$xls->MontaConteudoString($linexls, 8, $sublinha['lindsc']);
								$xls->MontaConteudoString($linexls, 9, "");
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$sublinha['linid'],$_REQUEST['ano']));
								$linexls++;
							}

						}	
					} elseif($colunapa) {
						foreach($colunapa as $col) {
							
							if($periodopa[$col['coldsc']]) {
								foreach($periodopa[$col['coldsc']] as $perdados) {
									unset($dadosperiodopa[$col['coldsc']]);
									$dadosperiodopa[$col['coldsc']][] = $perdados;
									$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
									$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
									$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
									$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
									$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
									$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
									$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
									$xls->MontaConteudoString($linexls, 7, strtoupper($agplinha['agpdsc']));
									$xls->MontaConteudoString($linexls, 8, $sublinha['lindsc']);
									$xls->MontaConteudoString($linexls, 9, "");
									$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
									$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$sublinha['linid'],$col['coldsc']));
									$linexls++;
								}
							} else {
								
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 6, "");
								$xls->MontaConteudoString($linexls, 7, strtoupper($agplinha['agpdsc']));
								$xls->MontaConteudoString($linexls, 8, $sublinha['lindsc']);
								$xls->MontaConteudoString($linexls, 9, "");
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$sublinha['linid'],$col['coldsc']));
								$linexls++;
							}
						}
					}
				}
				// fim das linhas do agrupador
			}
		}
	/*
	 * FIM
	 * FORMATO DE LINHA : LINHAS FIXAS COM SUBNIVEIS
	 */
		
	} elseif($linhas[0]) { // Caso não tenha agrupadores de linha, imprimir as linhas diretamente
		
		foreach($linhas as $lin) {
			if($agrupadorescoluna) {
				foreach($agrupadorescoluna as $agpcoluna) {
					foreach($coluna[$agpcoluna['agpid']] as $col) {
						// verificando se é uma linha totalizadora
						if($lin['lintipocampo']) {
							$col['tpitipocampo'] = $lin['lintipocampo'];
						}
						
						if($periodopa[$_REQUEST['ano']]) {
							foreach($periodopa[$_REQUEST['ano']] as $perdados) {
								unset($dadosperiodopa[$_REQUEST['ano']]);
								$dadosperiodopa[$_REQUEST['ano']][] = $perdados;
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
								$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
								$xls->MontaConteudoString($linexls, 7, "");
								$xls->MontaConteudoString($linexls, 8, $lin['lindsc']);
								$xls->MontaConteudoString($linexls, 9, strtoupper($agpcoluna['agpdsc']));
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lin['linid'],$_REQUEST['ano']));
								$linexls++;
							}
						} else {
							
							$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
							$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
							$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
							$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
							$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
							$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
							$xls->MontaConteudoString($linexls, 6, "");
							$xls->MontaConteudoString($linexls, 7, "");
							$xls->MontaConteudoString($linexls, 8, $lin['lindsc']);
							$xls->MontaConteudoString($linexls, 9, strtoupper($agpcoluna['agpdsc']));
							$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lin['linid'],$_REQUEST['ano']));
							$linexls++;
						}
						

					}
				}
			}elseif($coluna) {
				foreach($coluna as $col) {
					if(!$dadoscombo[$col['gpoid']][0] && $col['gpoid']) {
						$dadoscombo[$col['gpoid']] = $db->carregar("SELECT opcid AS codigo, opcdsc AS descricao FROM rehuf.opcoes WHERE gpoid='".$col['gpoid']."' ORDER BY descricao");
					}
					// verificando se é uma linha totalizadora
					if($lin['lintipocampo']) {
						$col['tpitipocampo'] = $lin['lintipocampo'];
					}
					
					if($periodopa[$_REQUEST['ano']]) {
						foreach($periodopa[$_REQUEST['ano']] as $perdados) {
							unset($dadosperiodopa[$_REQUEST['ano']]);
							$dadosperiodopa[$_REQUEST['ano']][] = $perdados;
							$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
							$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
							$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
							$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
							$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
							$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
							$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
							$xls->MontaConteudoString($linexls, 7, "");
							$xls->MontaConteudoString($linexls, 8, $lin['lindsc']);
							$xls->MontaConteudoString($linexls, 9, "");
							$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lin['linid'],$_REQUEST['ano']));
							$linexls++;
						}
					} else {
						
						$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
						$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
						$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
						$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
						$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
						$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
						$xls->MontaConteudoString($linexls, 6, "");
						$xls->MontaConteudoString($linexls, 7, "");
						$xls->MontaConteudoString($linexls, 8, $lin['lindsc']);
						$xls->MontaConteudoString($linexls, 9, "");
						$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
						$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lin['linid'],$_REQUEST['ano']));
						$linexls++;
					}

					
				}
			} elseif($colunapa) {
				$colum=1;
				foreach($colunapa as $col) {
					// verificando se é uma linha totalizadora
					if($lin['lintipocampo']) {
						$col['tpitipocampo'] = $lin['lintipocampo'];
					}
					if($periodopa[$col['coldsc']]) {
						foreach($periodopa[$col['coldsc']] as $perdados) {
							unset($dadosperiodopa[$col['coldsc']]);
							$dadosperiodopa[$col['coldsc']][] = $perdados;
							$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
							$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
							$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
							$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
							$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
							$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
							$xls->MontaConteudoString($linexls, 7, "");
							$xls->MontaConteudoString($linexls, 8, $lin['lindsc']);
							$xls->MontaConteudoString($linexls, 9, "");
							$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lin['linid'],$col['coldsc']));
							$linexls++;
						}
					} else {
						$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
						$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
						$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
						$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
						$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
						$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
						$xls->MontaConteudoString($linexls, 6, "");
						$xls->MontaConteudoString($linexls, 7, "");
						$xls->MontaConteudoString($linexls, 8, $lin['lindsc']);
						$xls->MontaConteudoString($linexls, 9, "");
						$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
						$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lin['linid'],$col['coldsc']));
						$linexls++;
					}
				}
			}
			
		}
		
	} elseif($linhaDinOp) { // Carregar as linhas que foram criadas dinamicamente
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÕES(COMBOBOX)  
	 	*/
		// verficar se existe linhas dinâmicas com opções cadastradas para tal entidade
		if($linhaDinOp[0]) {
			// verificando se existe linha totalizador geral
			if($grupoitem['gitpossuitotallinha'] == 't') {
				$linhaDinOp[] = array("opcdsc" => "TOTAL", "lintipocampo" => "textpossuitotallinha");
			}
			
			//imprimindo linhas ja cadastradas
			foreach($linhaDinOp as $lindiop) {
				// construindo o campo editavel, de acordo com o formato de colunas,
				// o campo é construido por linha
				if($agrupadorescoluna) {
					// varrendo as colunas dentro dos agrupadores
					foreach($agrupadorescoluna as $agpcoluna) {
						foreach($coluna[$agpcoluna['agpid']] as $col) {
							// verificando se é uma linha totalizadora
							if($lindiop['lintipocampo']) {
								$col['tpitipocampo'] = $lindiop['lintipocampo'];
							}
							
							if($periodopa[$_REQUEST['ano']]) {
								foreach($periodopa[$_REQUEST['ano']] as $perdados) {
									if($lindiop['perid'] == $perdados['codigo']) {
										unset($dadosperiodopa[$_REQUEST['ano']]);
										$dadosperiodopa[$_REQUEST['ano']][] = $perdados;
										$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
										$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
										$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
										$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
										$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
										$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
										$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
										$xls->MontaConteudoString($linexls, 7, "");
										$xls->MontaConteudoString($linexls, 8, $lindiop['opcdsc']);
										$xls->MontaConteudoString($linexls, 9, $agpcoluna['agpdsc']);
										$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
										$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lindiop['linid'],$_REQUEST['ano']));
										$linexls++;
									}
								}
							} else {
							
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
								$xls->MontaConteudoString($linexls, 6, "");
								$xls->MontaConteudoString($linexls, 7, $lindiop['opcdsc']);
								$xls->MontaConteudoString($linexls, 8, "");
								$xls->MontaConteudoString($linexls, 9, strtoupper($agpcoluna['agpdsc']));
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$lindiop['linid'],$_REQUEST['ano']));
								$linexls++;
							}

						}
					}
				}elseif($coluna) {
					foreach($coluna as $col) {

						if($lindiop['lintipocampo']) {
							$col['tpitipocampo'] = $lindiop['lintipocampo'];
						}
						$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
						$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
						$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
						$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
						$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
						$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
						$xls->MontaConteudoString($linexls, 6, "");
						$xls->MontaConteudoString($linexls, 7, $lindiop['opcdsc']);
						$xls->MontaConteudoString($linexls, 8, "");
						$xls->MontaConteudoString($linexls, 9, $col['coldsc']);
						$xls->MontaConteudoString($linexls, 10, definircamporelatorio($col,$lindiop['linid'],$_REQUEST['ano']));
						$linexls++;

					}
				} elseif($colunapa) {
					$colum=1;
					foreach($colunapa as $col) {
						// verificando se é uma linha totalizadora
						if($lindiop['lintipocampo']) {
							$col['tpitipocampo'] = $lindiop['lintipocampo'];
						}
						$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
						$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
						$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
						$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
						$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
						$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
						$xls->MontaConteudoString($linexls, 6, "");
						$xls->MontaConteudoString($linexls, 7, $lindiop['opcdsc']);
						$xls->MontaConteudoString($linexls, 8, "");
						$xls->MontaConteudoString($linexls, 9, $col['coldsc']);
						$xls->MontaConteudoString($linexls, 10, definircamporelatorio($col,$lindiop['linid'],$col['coldsc']));
						$linexls++;
					}
				}
			}
		} else {
			$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
			$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
			$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
			$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
			$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
			$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
			$xls->MontaConteudoString($linexls, 6, "N/A");
			$xls->MontaConteudoString($linexls, 7, "N/A");
			$xls->MontaConteudoString($linexls, 8, "N/A");
			$xls->MontaConteudoString($linexls, 9, "N/A");
			$xls->MontaConteudoString($linexls, 10, "N/A");
			$xls->MontaConteudoString($linexls, 11, "N/A");
			$linexls++;
		}
		
	   /*
	 	* FIM
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÕES(COMBOBOX)  
	 	*/
		
	} elseif($linhaDinTx) {
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÃO DE ESCREVER  
	 	*/
		
		if($linhaDinTx[0]) {
			foreach($linhaDinTx as $linditx) {
				if($agrupadorescoluna) {
					$clspan = 1;
					foreach($agrupadorescoluna as $agpcoluna) {
						$clspan += count($coluna[$agpcoluna['agpid']]);
						foreach($coluna[$agpcoluna['agpid']] as $col) {
							
							echo "<td>". definircamporelatorio($col,$linditx['linid'],$_REQUEST['ano']) ."</td>";
						}
					}
				}elseif($coluna) {
					
					foreach($coluna as $col) {
						
						if($periodopa[$_REQUEST['ano']]) {

							foreach($periodopa[$_REQUEST['ano']] as $perdados) {
								unset($dadosperiodopa[$_REQUEST['ano']]);
								$dadosperiodopa[$_REQUEST['ano']][] = $perdados;
								$perid = $perdados['codigo'];
								if($rspgit[$linditx['linid']][$col['colid']][$_REQUEST['ano']][$perid]) {
									$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
									$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
									$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
									$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
									$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
									$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
									$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
									$xls->MontaConteudoString($linexls, 7, "");
									$xls->MontaConteudoString($linexls, 8, $linditx['lindsc']);
									$xls->MontaConteudoString($linexls, 9, "");
									$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
									$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linditx['linid'],$_REQUEST['ano']));
									$linexls++;
								}
							}
							
						} else {
						
							$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
							$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
							$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
							$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
							$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
							$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
							$xls->MontaConteudoString($linexls, 6, "");
							$xls->MontaConteudoString($linexls, 7, "");
							$xls->MontaConteudoString($linexls, 8, $linditx['lindsc']);
							$xls->MontaConteudoString($linexls, 9, "");
							$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linditx['linid'],$_REQUEST['ano']));
							$linexls++;
						}

					}

					
				} elseif($colunapa) {
					foreach($colunapa as $col) {
						
						if($periodopa[$col['coldsc']]) {
							foreach($periodopa[$col['coldsc']] as $perdados) {
								unset($dadosperiodopa[$col['coldsc']]);
								$dadosperiodopa[$col['coldsc']][] = $perdados;
								$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
								$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
								$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
								$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
								$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
								$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 6, $perdados['descricao']);
								$xls->MontaConteudoString($linexls, 7, "");
								$xls->MontaConteudoString($linexls, 8, $linditx['lindsc']);
								$xls->MontaConteudoString($linexls, 9, "");
								$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
								$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linditx['linid'],$col['coldsc']));
								$linexls++;
							}
						} else {
						
							$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
							$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
							$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
							$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
							$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
							$xls->MontaConteudoString($linexls, 5, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 6, "");
							$xls->MontaConteudoString($linexls, 7, "");
							$xls->MontaConteudoString($linexls, 8, $linditx['lindsc']);
							$xls->MontaConteudoString($linexls, 9, "");
							$xls->MontaConteudoString($linexls, 10, $col['coldsc']);
							$xls->MontaConteudoString($linexls, 11, definircamporelatorio($col,$linditx['linid'],$col['coldsc']));
							$linexls++;
						}
						
					}
				}
			}
		} else {
			$xls->MontaConteudoString($linexls, 0, $esuid['entsig']);
			$xls->MontaConteudoString($linexls, 1, $esuid['entnome']);
			$xls->MontaConteudoString($linexls, 2, $esuid['estuf']);
			$xls->MontaConteudoString($linexls, 3, $tabela['tabtdsc']);
			$xls->MontaConteudoString($linexls, 4, $grupoitem['gitdsc']);
			$xls->MontaConteudoString($linexls, 5, $_REQUEST['ano']);
			$xls->MontaConteudoString($linexls, 6, "");
			$xls->MontaConteudoString($linexls, 7, "N/A");
			$xls->MontaConteudoString($linexls, 8, "");
			$xls->MontaConteudoString($linexls, 9, "N/A");
			$xls->MontaConteudoString($linexls, 10, "N/A");
			$linexls++;
		}
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÃO DE ESCREVER  
	 	*/
		
	} 
}

?>