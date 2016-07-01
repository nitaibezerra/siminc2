	<form name='formulario' method='post' id='formulario<? echo $esuid['entid']."_".$tabela['tabtid']."_".$grupoitem['gitid']."_".$_REQUEST['ano']; ?>'>
	<input type='hidden' name='alterabd' value='inserirconteudoitem'>
	<input type='hidden' name='gitid' value='<? echo $grupoitem['gitid']; ?>'>
	<input type='hidden' name='esuid' value='<? echo $_POST['esuid']; ?>'>
	<input type='hidden' name='ctiexercicio' value='<? echo $_REQUEST['ano']; ?>'>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<?	if($grupoitem) { ?>
	<tr>
		<td width='50%' onmouseover="return escape('<? echo nl2br($grupoitem['gitobs']); ?>')" class='stylecolunas' <? echo (($agrupadorescoluna)?'rowspan="2"':''); echo (($agrupadoreslinha)?'colspan="2"':''); ?> ><strong><font style="font-size:13px;"><? echo $grupoitem['gitdsc']; ?></font></strong></td>
		<?
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
				$coluna["total"][] = array("coldsc" => "<strong>TOTAL</strong>", "tpitipocampo" => "textpossuitotalcoluna", "tpimascara" => $mask, "colobs" => "Total geral");
			}
			foreach($agrupadorescoluna as $agpcoluna) {
				// Verifica se o agrupador possui totalizador, obs: totalizar somente od itens do agrupador, e não todos
				if($agpcoluna['agppossuitotal']=='t') {
					$mask = current($coluna[$agpcoluna['agpid']]);
					$mask = $mask['tpimascara'];
					
					$coluna[$agpcoluna['agpid']][] = array("coldsc" => "<strong>TOTAL</strong>", "tpitipocampo" => "textpossuitotalagrupador", "agpid" => $agpcoluna['agpid'], "tpimascara" => $mask, "colobs" => "Total do agrupador",);
				}
				// imprimindo o primeiro nivel das colunas (agrupadores), em seguida imprimir o segundo nivel contendo as colunas (COD. LINE: +- 261 )
				echo "<td class='stylecolunas' colspan=". (count($coluna[$agpcoluna['agpid']])) .">".$agpcoluna['agpdsc']."</td>";
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
				
				$coluna[] = array("colid" => "totalcol",
								  "coldsc" => "<strong>TOTAL</strong>", 
								  "tpitipocampo" => "textpossuitotalcoluna",
								  "colobs" => "Total geral",
								  "tpimascara" => $mask);
			}
			// Imprimindo as colunas (sem agrupadores)
			foreach($coluna as $col) {
				echo "<td class='stylecolunas' onmouseover=\"return escape('". str_replace("\n","<br />",$col['colobs']) ."');\">". $col['coldsc'] ."</td>";
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
				
				$colunapa[] = array("colid" => "totalcol",
									"coldsc" => "<strong>TOTAL</strong>", 
									"tpitipocampo" => "textpossuitotalcoluna",
									"colobs" => "Total geral",
									"tpimascara" => $mask);
			}
			// imprimindo as colunas
			foreach($colunapa as $col) {
				echo "<td class='stylecolunas'>". $col['coldsc'] ."</td>";
			}
			/*
			 * FIM
			 * FORMATO DE COLUNA : COLUNAS FIXAS REFERENTES AOS ANOS DA TABELA  
			 */
		}
		?>
	</tr>
	<?
	/*
	 * INÍCIO
	 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS (2ºETAPA)
	 * Caso tenha agrupadores de colunas, este laço irá imprimir as colunas do agrupadores
	 * é uma nova linha referente aos dados de coluna (ainda representa o cabeçalho)  
	 */
	if($agrupadorescoluna) {
		echo "<tr>";
		foreach($agrupadorescoluna as $agpcoluna) {
			if($coluna[$agpcoluna['agpid']]) {
				foreach($coluna[$agpcoluna['agpid']] as $col) {
					echo "<td class='stylecolunas' onmouseover=\"return escape('".$col['colobs']."');\">".$col['coldsc']."</td>";
				}
			}
		}
		echo "</tr>";
	}
	/*
	 * FIM
	 * FORMATO DE COLUNA : COLUNAS FIXAS COM SUBNIVEIS (2ºETAPA)
	 */
	

	/*
	 * INÍCIO
	 * FORMATO DE LINHA : LINHAS FIXAS COM SUBNIVEIS
	 * Caso tenha agrupadores por linha
	 */

	if($agrupadoreslinha) {
		// verificando se existe linha totalizador geral
		if($grupoitem['gitpossuitotallinha'] == 't') {
			$agrupadoreslinha[] = array("agpid" => "total");
			$linhas['total'][] = array("linid" => "totallin", "lindsc" => "<strong>TOTAL</strong>", "lintipocampo" => "textpossuitotallinha");
		}
		
		// varrendo as linhas fixas cadastradas
		foreach($agrupadoreslinha as $agplinha) {
			// se o agrupador de linha tiver sublinhas (o normal é ter linhas dentro do agrupador)
			if(count($linhas[$agplinha['agpid']]) > 0) {
				// imprimindo a primeira linha com agrupador(rowspan)
				echo "<tr>";
				echo "<td class='stylecolunas' rowspan='". count($linhas[$agplinha['agpid']]) ."'>".$agplinha['agpdsc']."</td>";
				echo "<td class='stylelinhas' onmouseover=\"return escape('".$linhas[$agplinha['agpid']][0]['linobs']."');\">".$linhas[$agplinha['agpid']][0]['lindsc']." : </td>";
				// definindo campos da linha (varrendo os dados de coluna)
				if($agrupadorescoluna) { // verifica se o formato de coluna é 'fixas com subniveis'
					foreach($agrupadorescoluna as $agpcoluna) {
						if($coluna[$agpcoluna['agpid']]) {
							foreach($coluna[$agpcoluna['agpid']] as $col) {
								echo "<td class='styleconteudo'>".definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano'])." ".$campoobs."</td>";
							}
						}
					}
				} elseif($coluna) { // verifica se o formato de coluna é 'fixas sem subniveis'
					foreach($coluna as $col) {
						/*
						// 	verificando se pode existir obs
						unset($campoobs);
						if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']);
						}*/
						echo "<td class='styleconteudo'>". definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
					}
				} elseif($colunapa) {// verifica se o formato de coluna é 'fixas refentes ao ano'
					foreach($colunapa as $col) {
						if($linhas[$agplinha['agpid']][0]['lintipocampo']) {
							$col['tpitipocampo'] = $linhas[$agplinha['agpid']][0]['lintipocampo'];
						}
						if($linhas[$agplinha['agpid']][0]['linid'] == "totallin" || $col['colid'] == "totalcol") {
							echo "<td class='styleconteudo'><b>". definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']) ."</b><input type='hidden' name='anoexercicioitem[". $linhas[$agplinha['agpid']][0]['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'> ".$campoobs."</td>";
						} else {
							echo "<td class='styleconteudo'>". definircamporelatorio($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']) ."<input type='hidden' name='anoexercicioitem[". $linhas[$agplinha['agpid']][0]['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'> ".$campoobs."</td>";
						}
					}
				}
				unset($linhas[$agplinha['agpid']][0]);				
				echo "</tr>";
				// fim da primeira linha com o agrupador
				
				// imprime as demais linhas contidas no agrupador
				foreach($linhas[$agplinha['agpid']] as $sublinha) {
					echo "<tr>";
					echo "<td class='stylelinhas' onmouseover=\"return escape('".$sublinha['linobs']."');\">".$sublinha['lindsc']." : </td>";
					
					if($agrupadorescoluna) {
						foreach($agrupadorescoluna as $agpcoluna) {
							if($coluna[$agpcoluna['agpid']]) {
								foreach($coluna[$agpcoluna['agpid']] as $col) {
									/*
									// verificando se pode existir obs
									unset($campoobs);
									if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
										$campoobs = definirobservacao($col,$sublinha['linid'],$_REQUEST['ano']);
									}*/
									
									echo "<td class='styleconteudo'>".definircamporelatorio($col,$sublinha['linid'],$_REQUEST['ano'])." ".$campoobs."</td>";
								}
							}
						}
					} elseif($coluna) {
						foreach($coluna as $col) {
							/*
							// verificando se pode existir obs
							unset($campoobs);
							if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$campoobs = definirobservacao($col,$sublinha['linid'],$_REQUEST['ano']);
							}*/
							
							echo "<td class='styleconteudo'>". definircamporelatorio($col,$sublinha['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
						}	
					} elseif($colunapa) {
						foreach($colunapa as $col) {
							if($col['colid'] == "totalcol") {
								echo "<td class='styleconteudo'><b>". definircamporelatorio($col,$sublinha['linid'],$col['coldsc']) ."</b></td>";
							} else {
								echo "<td class='styleconteudo'>". definircamporelatorio($col,$sublinha['linid'],$col['coldsc']) ."</td>";
							}
						}
					}
					echo "</tr>";
				}
				// fim das linhas do agrupador
			}
		}
	/*
	 * FIM
	 * FORMATO DE LINHA : LINHAS FIXAS COM SUBNIVEIS
	 */
		
	} elseif($linhas[0]) { // Caso não tenha agrupadores de linha, imprimir as linhas diretamente
		// verificando se existe linha totalizador geral
		if($grupoitem['gitpossuitotallinha'] == 't') {
			$linhas[] = array("linid" => "totallin","lindsc" => "<strong>TOTAL</strong>", "lintipocampo" => "textpossuitotallinha");
		}
		
		foreach($linhas as $lin) {
			echo "<tr>";
			echo "<td class='stylelinhas' onmouseover=\"return escape('".$lin['linobs']."');\">".$lin['lindsc']."</td>";
			if($agrupadorescoluna) {
				foreach($agrupadorescoluna as $agpcoluna) {
					foreach($coluna[$agpcoluna['agpid']] as $col) {
						// verificando se é uma linha totalizadora
						if($lin['lintipocampo']) {
							$col['tpitipocampo'] = $lin['lintipocampo'];
						}
						if($lin['linid'] == "totallin" || $col['colid'] == "totalcol") {
							echo "<td class='styleconteudo'><b>". definircamporelatorio($col,$lin['linid'],$_REQUEST['ano'])."</b></td>";
						} else {
							echo "<td class='styleconteudo'>". definircamporelatorio($col,$lin['linid'],$_REQUEST['ano'])."</td>";
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
					if($lin['linid'] == "totallin" || $col['colid'] == "totalcol") {					
						echo "<td class='styleconteudo'><b>". definircamporelatorio($col,$lin['linid'],$_REQUEST['ano']) ."</b></td>";
					} else {
						echo "<td class='styleconteudo'>". definircamporelatorio($col,$lin['linid'],$_REQUEST['ano']) ."</td>";
					}
				}
			} elseif($colunapa) {
				foreach($colunapa as $col) {
					// verificando se é uma linha totalizadora
					if($lin['lintipocampo']) {
						$col['tpitipocampo'] = $lin['lintipocampo'];
					}
					if($lin['linid'] == "totallin" || $col['colid'] == "totalcol") {
						echo "<td class='styleconteudo'><b>". definircamporelatorio($col,$lin['linid'],$col['coldsc']) ."</b></td>";
					} else {
						echo "<td class='styleconteudo'>". definircamporelatorio($col,$lin['linid'],$col['coldsc']) ."</td>";
					}
				}
			}
			echo "</tr>";
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
				$linhaDinOp[] = array("linid" => "totallin", "opcdsc" => "<strong>TOTAL</strong>", "lintipocampo" => "textpossuitotallinha");
			}
			
			//imprimindo linhas ja cadastradas
			foreach($linhaDinOp as $lindiop) {
				echo "<tr onmouseover=\"this.bgColor='#ffffcc';\"  onmouseout=\"this.bgColor='';\">";
				echo "<td>";
				echo $lindiop['opcdsc'];
				echo "</td>";
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
							if($lindiop['linid'] == "totallin" || $col['colid'] == "totalcol") {
								echo "<td class='styleconteudo'><b>". definircamporelatorio($col,$lindiop['linid'],$_REQUEST['ano']) ."</b></td>";
							} else {
								echo "<td class='styleconteudo'>". definircamporelatorio($col,$lindiop['linid'],$_REQUEST['ano']) ."</td>";
							}
						}
					}
				}elseif($coluna) {
					foreach($coluna as $col) {
						if($lindiop['lintipocampo']) {
							$col['tpitipocampo'] = $lindiop['lintipocampo'];
						}
						// Verificando se a celular é do total, se for, deixar em negrito
						if($lindiop['linid'] == "totallin" || $col['colid'] == "totalcol") {
							echo "<td class='styleconteudo'><b>". definircamporelatorio($col,$lindiop['linid'],$_REQUEST['ano']) ."</b></td>";
						} else {
							echo "<td class='styleconteudo'>". definircamporelatorio($col,$lindiop['linid'],$_REQUEST['ano']) ."</td>";
						}
					}
				} elseif($colunapa) {
					foreach($colunapa as $col) {
						/*
						// verificando se pode existir obs
						unset($campoobs);
						if(($lindiop['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$lindiop['linid'],$_REQUEST['ano']);
						}*/
						// verificando se é uma linha totalizadora
						if($lindiop['lintipocampo']) {
							$col['tpitipocampo'] = $lindiop['lintipocampo'];
						}
						
						echo "<td class='styleconteudo'>". definircamporelatorio($col,$lindiop['linid'],$col['coldsc']) ."<input type='hidden' name='anoexercicioitem[". $lindiop['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'> ".$campoobs."</td>";
					}
				}
				echo "</tr>";
			}
		} else {
			echo "<tr><td>";
			echo "<table class='tabela' align='center'><tr><td><b>Não existem linhas cadastradas</b></td></tr></table>";
			echo "</td></tr>";	
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
				echo "<tr>";
				echo "<td>";
				//$nometx = 'lindsc_'.$linditx['linid'];
				echo $linditx['lindsc'];
				//echo campo_texto($nometx, "N", (($permissoes['gravar'])?"S":"N"), "Descrição da linha", 20, 20, "", "", '', '', 0, 'id="gitdsc_'.$linditx['linid'].'"' );
				echo "</td>";
				if($agrupadorescoluna) {
					$clspan = 1;
					foreach($agrupadorescoluna as $agpcoluna) {
						$clspan += count($coluna[$agpcoluna['agpid']]);
						foreach($coluna[$agpcoluna['agpid']] as $col) {
							echo "<td class='styleconteudo'>". definircamporelatorio($col,$linditx['linid'],$_REQUEST['ano']) ."</td>";
						}
					}
				}elseif($coluna) {
					foreach($coluna as $col) {
						$clspan = (count($coluna)+1);
						echo "<td class='styleconteudo'>". definircamporelatorio($col,$linditx['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
					}
				} elseif($colunapa) {
					foreach($colunapa as $col) {
						echo "<td class='styleconteudo'>". definircamporelatorio($col,$linditx['linid'],$col['coldsc']) ."</td>";
					}
				}
				echo "</tr>";
			}
		} else {
			echo "<tr><td>";
			echo "<table class='tabela' align='center'><tr><td><b>Não existem linhas cadastradas</b></td></tr></table>";
			echo "</td></tr>";
		}
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÃO DE ESCREVER  
	 	*/
		
	} 
}

?>
</table>
</form>