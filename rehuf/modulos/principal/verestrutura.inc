<?
/*
 * TELA DE EDIÇÃO DE TABELAS (Monta vários tipo de tabelas definidos pelo administrador)
 * Parâmetro obrigatório : $_REQUEST['tabtid'], parâmetro opcional : $_REQUEST['gitid'],$_REQUEST['ano']
 *  
 */

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<style type="text/css">
.stylecolunas {
	background-color: #dcdcdc;
	text-align: center; 
}
.stylelinhas {
	background-color: #dcdcdc;
	text-align: right; 
}
.styleconteudo {
	text-align: left;
}
</style>
<form name='formulario' method='post' id='formulario'>
<input type='hidden' name='alterabd' value='inserirconteudoitem'>
<?
// Pegando dados da TABELA (pai de toda a estrutura)
if($_REQUEST['tabtid']) {
	$tabela = $db->carregar("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid = '". $_REQUEST['tabtid'] ."'");
}
// Verifica se existe a TABELA
if($tabela) {
	// Caso exista, pegar o primeiro registro
	$tabela = current($tabela);
	// Se existir o filtro de 'grupo', buscar somente tal grupo
	if($_REQUEST['gitid']) {
		$filgrupo = "AND gitid = '". $_REQUEST['gitid'] ."'";
	}
	// Pegar um registro (ou registro filtrado, ou primeiro registro da ordem)
	$grupoitem = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ".$filgrupo." ORDER BY gitordem LIMIT 1");
	if($grupoitem) {
		$grupoitem = current($grupoitem);
		// Analisando o tipo de coluna (definido em constantes.php)
		switch($grupoitem['tpgidcoluna']) {
			case TPG_CFIXAS_PA: // Colunas fixas referentes por ano
				$colunapa = (array) $db->carregar("SELECT * FROM rehuf.coluna col
												   LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
												   WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY coldsc");
				break;
			case TPG_CFIXAS_SN: // Colunas fixas sem agrupadores
				$coluna = (array) $db->carregar("SELECT * FROM rehuf.coluna col
												 LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
												 WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY colordem");
				break;
			case TPG_CFIXAS_CN: // Colunas fixas agrupadores agrupadores
				// Carregando os agrupadores de coluna
				$agrupadorescoluna = (array) $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid = '". $grupoitem['gitid'] ."' AND agplinha = false ORDER BY agpordem");
				// Verifica se o agrupador esta vazio (a função carregar retorna o indice 0 vazio: array(0 =>))
				if($agrupadorescoluna[0]) {
					foreach($agrupadorescoluna as $agpcoluna) {
						// Carregando as colunas de cada agrupador
						$coluna[$agpcoluna['agpid']] = (array) $db->carregar("SELECT * FROM rehuf.coluna col
																			  LEFT JOIN rehuf.tipoitem tpi ON tpi.tpiid = col.tpiid 
																			  WHERE gitid = '". $grupoitem['gitid'] ."' AND agpid = '". $agpcoluna['agpid'] ."' ORDER BY colordem");
						// Caso não tenha coluna em um agrupador, criando coluna indicando 'Sem coluna' 
						if(!$coluna[$agpcoluna['agpid']][0]) {
							$coluna[$agpcoluna['agpid']][0] = array("coldsc" => "Sem coluna");
						}
					}
				} else { // Caso tenha apenas o indice 0, limpar o agrupador
					unset($agrupadorescoluna);
				}
				break;
		}
		// Analisando o tipo de linha (definido em constantes.php)
		switch($grupoitem['tpgidlinha']) {
			case TPG_LDINAM_TEXT:
				$linhaDinTx = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY linordem");
				break;
			case TPG_LDINAM_OPCOES:
				$linhaDinOp = (array) $db->carregar("SELECT * FROM rehuf.linha lin
													 LEFT JOIN rehuf.opcoes opc ON opc.opcid = lin.opcid  
													 WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY linordem");
				break;
			case TPG_LFIXAS_SN:
				$linhas = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' ORDER BY linordem");
				break;
			case TPG_LFIXAS_CN:
				$agrupadoreslinha = (array) $db->carregar("SELECT * FROM rehuf.agrupamento WHERE gitid = '". $grupoitem['gitid'] ."' AND agplinha = true ORDER BY agpordem");
				if($agrupadoreslinha[0]) {
					foreach($agrupadoreslinha as $agplinha) {
						$linhas[$agplinha['agpid']] = (array) $db->carregar("SELECT * FROM rehuf.linha WHERE gitid = '". $grupoitem['gitid'] ."' AND agpid = '". $agplinha['agpid'] ."' ORDER BY linordem");
						if(!$linhas[$agplinha['agpid']][0]) {
							$linhas[$agplinha['agpid']][0] = array("lindsc" => "Sem coluna");;
						}
					}
				} else {
					unset($agrupadoreslinha);
				}
				break;
		}
	}
	echo "<div style=\"overflow:auto;\" id=\"controlerolagem\">";
	
	// Criando navegação no cabeçalho (navegando entre os grupos da tabela)
	$grupositem = $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ORDER BY gitordem");
	if(count($grupositem) > 1) {
		// imprimindo cabeçalho (contendo nome do grupo com paginação e nome do hospital)
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
				<tr><td align='center'><span class=\"TituloTela\" style=\"color:#000000;\">".$crtback." ".$tabela['tabtdsc']." ".$crtgrupo ." ".$crtfoward."</span></td></tr>
			  </table>";
	} else {
		// imprimindo cabeçalho (contendo nome do grupo com paginação e nome do hospital)
		echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			  <tr><td align='center'><span class=\"TituloTela\" style=\"color:#000000;\">".$tabela['tabtdsc']."</span></td></tr>
			  </table>";
		
	}
	echo "<br />";
	
	/* Se o tipo de coluna for igual a 'referentes ao ano', não imprime abas.
	 * Tratamento especial para o tipo 'colunas fixas referente por ano', onde
	 * o controle do exercicio é feito descrição da coluna (coldsc) 
	 */
	if($grupoitem['tpgidcoluna'] != TPG_CFIXAS_PA) {
		// Criando Abas de navegação por ano
		for($i = $tabela['tabanoini']; $i <= $tabela['tabanofim']; $i++) {
			$ano[] = array("id" => $i,
						   "descricao" => $i,
						   "link" => "?modulo=principal/editartabela&acao=A&gitid=". $grupoitem['gitid'] ."&tabtid=". $_REQUEST['tabtid'] ."&ano=".$i);	
		}
		$_REQUEST['ano'] = (($_REQUEST['ano'])?$_REQUEST['ano']:$tabela['tabanoini']);
		// Caso o grupo item tenha abas por ano, criar filtro para pegar as respostas apenas por ano
		$fano = "cti.ctiexercicio = '". $_REQUEST['ano'] ."' AND";
	}

?>
	<script>
	// Função utilizada na inserção de linhas com o campo text
	// No momento do clique no ícone de inserir
	function inserirlinhatx(linha, cols) {
		ajaxatualizar('alterabd=inserirlinhadintx&lindsc='+document.getElementById('lindsc_'+linha).value+'&gitid=<? echo $grupoitem['gitid']; ?>','');
		window.location='?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $grupoitem['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; ?>'
	}
	function removerlinhatx(linid) {
		var conf = confirm("Esta ação irá apagar todos os dados referentes a esta linha. Deseja continuar ?");
		if(conf) {
			ajaxatualizar('alterabd=removerlinhadintx&linid='+linid,'');
			window.location='?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $grupoitem['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; ?>'
		}
	}
	
	function removerlinha(opcid) {
		var conf = confirm("Esta ação irá apagar todos os dados referentes a esta linha. Deseja continuar ?");
		if(conf) {
			ajaxatualizar('alterabd=removerlinhadinop&opcid='+opcid+'&gitid=<? echo $grupoitem['gitid'];; ?>','');
			window.location='?modulo=principal/editartabela&acao=A&ano=<? echo $_REQUEST['ano']; ?>&gitid=<? echo $grupoitem['gitid']; ?>&tabtid=<? echo $_REQUEST['tabtid']; ?>'
			alert('Linha removida com sucesso');
		}
	}
	
	</script>
	<input type='hidden' name='gitid' value='<? echo $grupoitem['gitid']; ?>'>
	<input type='hidden' name='esuid' value='<? echo $_SESSION['rehuf_var']['esuid']; ?>'>
	<input type='hidden' name='ctiexercicio' value='<? echo $_REQUEST['ano']; ?>'>
	<script>
	var myWidth;
	if( typeof( window.innerWidth ) == 'number' ) {
    	//Non-IE
    	myWidth = window.innerWidth;
	} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
    	//IE 6+ in 'standards compliant mode'
    	myWidth = document.documentElement.clientWidth;
	} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
    	//IE 4 compatible
    	myWidth = document.body.clientWidth;
	}
	document.getElementById('controlerolagem').style.width = myWidth-20;
	</script>
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
			// Caso o tipo de linha seja dinâmica, inserir uma coluna de ações
			if($linhaDinOp || $linhaDinTx) {
				echo "<td rowspan='2' onmouseover=\"return escape('Ações');\" class='stylecolunas'>Ações</td>";	
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
				
				$coluna[] = array("coldsc" => "<strong>TOTAL</strong>", 
								  "tpitipocampo" => "textpossuitotalcoluna",
								  "colobs" => "Total geral",
								  "tpimascara" => $mask);
			}
			// Imprimindo as colunas (sem agrupadores)
			foreach($coluna as $col) {
				echo "<td class='stylecolunas' onmouseover=\"return escape('". str_replace("\n","<br />",$col['colobs']) ."');\">". $col['coldsc'] ."</td>";
			}
			// Caso o tipo de linha seja dinâmica, inserir uma coluna de ações (para inserir os ícones de inserir e remover)
			if($linhaDinOp || $linhaDinTx) {
				echo "<td class='stylecolunas' onmouseover=\"return escape('Ações');\">Ações</td>";	
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
				
				$colunapa[] = array("coldsc" => "<strong>TOTAL</strong>", 
									"tpitipocampo" => "textpossuitotalcoluna",
									"colobs" => "Total geral",
									"tpimascara" => $mask);
			}
			// imprimindo as colunas
			foreach($colunapa as $col) {
				echo "<td class='stylecolunas'>". $col['coldsc'] ."</td>";
			}
			// Caso o tipo de linha seja dinâmica, inserir uma coluna de ações
			if($linhaDinOp || $linhaDinTx) {
				echo "<td class='stylecolunas' onmouseover=\"return escape('Ações');\">Ações</td>";	
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
								// 	verificando se pode existir obs
								unset($campoobs);
								if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
									$campoobs = definirobservacao($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']);
								}
																
								echo "<td class='styleconteudo'>".definircampo($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano'])." ".$campoobs."</td>";
							}
						}
					}
				} elseif($coluna) { // verifica se o formato de coluna é 'fixas sem subniveis'
					foreach($coluna as $col) {
						// 	verificando se pode existir obs
						unset($campoobs);
						if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']);
						}
												
						echo "<td class='styleconteudo'>". definircampo($col,$linhas[$agplinha['agpid']][0]['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
					}
				} elseif($colunapa) {// verifica se o formato de coluna é 'fixas refentes ao ano'
					foreach($colunapa as $col) {
						// 	verificando se pode existir obs
						unset($campoobs);
						if(($linhas[$agplinha['agpid']][0]['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']);
						}
						
						echo "<td class='styleconteudo'>". definircampo($col,$linhas[$agplinha['agpid']][0]['linid'],$col['coldsc']) ."<input type='hidden' name='anoexercicioitem[". $linhas[$agplinha['agpid']][0]['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'> ".$campoobs."</td>";
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
									// verificando se pode existir obs
									unset($campoobs);
									if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
										$campoobs = definirobservacao($col,$sublinha['linid'],$_REQUEST['ano']);
									}
									
									echo "<td class='styleconteudo'>".definircampo($col,$sublinha['linid'],$_REQUEST['ano'])." ".$campoobs."</td>";
								}
							}
						}
					} elseif($coluna) {
						foreach($coluna as $col) {
							// verificando se pode existir obs
							unset($campoobs);
							if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$campoobs = definirobservacao($col,$sublinha['linid'],$_REQUEST['ano']);
							}
							
							echo "<td class='styleconteudo'>". definircampo($col,$sublinha['linid'],$col['coldsc']) ." ".$campoobs."</td>";
						}	
					} elseif($colunapa) {
						foreach($colunapa as $col) {
							// verificando se pode existir obs
							unset($campoobs);
							if(($sublinha['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$campoobs = definirobservacao($col,$sublinha['linid'],$col['coldsc']);
							}
							
							echo "<td class='styleconteudo'>". definircampo($col,$sublinha['linid'],$col['coldsc']) ."<input type='hidden' name='anoexercicioitem[". $sublinha['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'> ".$campoobs."</td>";
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
		foreach($linhas as $lin) {
			echo "<tr>";
			echo "<td class='stylelinhas' onmouseover=\"return escape('".$lin['linobs']."');\">".$lin['lindsc']."</td>";
			if($agrupadorescoluna) {
				foreach($agrupadorescoluna as $agpcoluna) {
					foreach($coluna[$agpcoluna['agpid']] as $col) {
						// 	verificando se pode existir obs
						unset($campoobs);
						if(($lin['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$lin['linid'],$_REQUEST['ano']);
						}
						
						echo "<td class='styleconteudo'>". definircampo($col,$lin['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
					}
				}
			}elseif($coluna) {
				foreach($coluna as $col) {
					if(!$dadoscombo[$col['gpoid']][0] && $col['gpoid']) {
						$dadoscombo[$col['gpoid']] = $db->carregar("SELECT opcid AS codigo, opcdsc AS descricao FROM rehuf.opcoes WHERE gpoid='".$col['gpoid']."' ORDER BY descricao");
					}
					// verificando se pode existir obs
					unset($campoobs);
					if(($lin['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
						$campoobs = definirobservacao($col,$lin['linid'],$_REQUEST['ano']);
					}
					echo "<td class='styleconteudo'>". definircampo($col,$lin['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
				}
			} elseif($colunapa) {
				foreach($colunapa as $col) {
					// verificando se pode existir obs
					unset($campoobs);
					if(($lin['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
						$campoobs = definirobservacao($col,$lin['linid'],$col['coldsc']);
					}
					
					echo "<td class='styleconteudo'><input type='hidden' name='anoexercicioitem[". $lin['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'>". definircampo($col,$lin['linid'],$col['coldsc']) ." ".$campoobs."</td>";
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
			//imprimindo linhas ja cadastradas
			foreach($linhaDinOp as $lindiop) {
				echo "<tr>";
				echo "<td nowrap>";
				echo $lindiop['opcdsc'];
				echo "</td>";
				// construindo o campo editavel, de acordo com o formato de colunas,
				// o campo é construido por linha
				if($agrupadorescoluna) {
					// varrendo as colunas dentro dos agrupadores
					foreach($agrupadorescoluna as $agpcoluna) {
						foreach($coluna[$agpcoluna['agpid']] as $col) {
							// verificando se pode existir obs
							unset($campoobs);
							if(($lindiop['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$campoobs = definirobservacao($col,$lindiop['linid'],$_REQUEST['ano']);
							}
							
							echo "<td>". definircampo($col,$lindiop['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
						}
					}
				}elseif($coluna) {
					foreach($coluna as $col) {
						// verificando se pode existir obs
						unset($campoobs);
						if(($lindiop['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$lindiop['linid'],$_REQUEST['ano']);
						}
						
						echo "<td>". definircampo($col,$lindiop['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
					}
				}
				echo "<td>". (($permissoes['gravar'])?"<img src=\"../imagens/excluir.gif\" onclick=\"removerlinha(". $lindiop['opcid'] .");\">":"&nbsp") ."</td>";
				echo "</tr>";
			}
		}
		// essa linha sempre será a última, estatica para inserção
		echo "<tr>";
		// definindo o número de colunas para saber o tamanho do colspan 
		if($agrupadorescoluna) {
			$clspan = 1;
			foreach($agrupadorescoluna as $agpcoluna) {
				$clspan += count($coluna[$agpcoluna['agpid']]);
			}
		}elseif($coluna) {
			$clspan = (count($coluna)+1);
		}
		// colocando o ícone de inserir na última coluna
		echo "<td colspan='".($clspan+1)."'><a href=\"javascript:void(0);\"><img src=\"../imagens/gif_inclui.gif\" border=\"0\"> Incluir linha</a></td>";
		unset($coljs);
		echo "</tr>";
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
				$nometx = 'lindsc_'.$linditx['linid'];
				$$nometx = $linditx['lindsc'];
				echo campo_texto($nometx, "N", (($permissoes['gravar'])?"S":"N"), "Descrição da linha", 20, 20, "", "", '', '', 0, 'id="gitdsc_'.$linditx['linid'].'"' );
				echo "</td>";
				if($agrupadorescoluna) {
					$clspan = 1;
					foreach($agrupadorescoluna as $agpcoluna) {
						$clspan += count($coluna[$agpcoluna['agpid']]);
						foreach($coluna[$agpcoluna['agpid']] as $col) {
							// verificando se pode existir obs
							unset($campoobs);
							if(($linditx['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
								$campoobs = definirobservacao($col,$linditx['linid'],$_REQUEST['ano']);
							}
							
							echo "<td>". definircampo($col,$linditx['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
						}
					}
				}elseif($coluna) {
					foreach($coluna as $col) {
						$clspan = (count($coluna)+1);
						
						// verificando se pode existir obs
						unset($campoobs);
						if(($linditx['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$linditx['linid'],$_REQUEST['ano']);
						}
						
						
						echo "<td>". definircampo($col,$linditx['linid'],$_REQUEST['ano']) ." ".$campoobs."</td>";
					}
				} elseif($colunapa) {
					foreach($colunapa as $col) {
						
						// verificando se pode existir obs
						unset($campoobs);
						if(($linditx['linpermiteobs']=="t") || ($col['colpermiteobs']=="t")) {
							$campoobs = definirobservacao($col,$linditx['linid'],$_REQUEST['ano']);
						}
						
						echo "<td class='styleconteudo'>". definircampo($col,$linditx['linid'],$col['coldsc']) ."<input type='hidden' name='anoexercicioitem[". $linditx['linid'] ."][". $col['colid'] ."]' value='". $col['coldsc'] ."'> ".$campoobs."</td>";
					}
				}
				echo "<td>".(($permissoes['gravar'])?"<img src=\"../imagens/excluir.gif\" onclick=\"removerlinhatx('". $linditx['linid'] ."');\">":"&nbsp")."</td>";
				echo "</tr>";
			}
		} 
		echo "<tr style='display:none'>";
		echo "<td>"; 
		echo campo_texto('lindsc_insercao', "N", "S", "Nome do grupo", 20, 20, "", "", '', '', 0, 'id="lindsc_insercao"' );
		echo "</td>";
		if($agrupadorescoluna) {
			foreach($agrupadorescoluna as $agpcoluna) {
				foreach($coluna[$agpcoluna['agpid']] as $col) {
					echo "<td>". definircampo($col,'insercao') ."</td>";
				}
			}
		}elseif($coluna) {
			foreach($coluna as $col) {
				echo "<td>". definircampo($col,'insercao') ."</td>";
			}
		} elseif($colunapa) {
			foreach($colunapa as $col) {
				echo "<td>". definircampo($col,'insercao') ."</td>";
			}
		}
		// referente a ações
		echo "<td>&nbsp;</td>";
		echo "</tr>";
		echo "<tr>";
		echo "<td><a href=\"javascript:void(0);\"><img src=\"../imagens/gif_inclui.gif\" border=\"0\"> Incluir linha</a></td>";
		unset($coljs);
		echo "</tr>";
		
	   /*
	 	* INÍCIO
	 	* FORMATO DE LINHA : LINHAS DINÂMICAS COM OPÇÃO DE ESCREVER  
	 	*/
		
	}
}
?>
</table>

</div>



<? 
} else {
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
		  <tr>
			<td colspan=\"2\">
			<div style=\"float: left;\"><strong>Estrutura não encontrada.</strong></div>
			</td>
		  </tr>
		  </table>"; 
}
?>
</form>
<script>
aplicarmascara();
function aplicarmascara() {
	var form = document.getElementById('formulario');
	for (var j=0; j < form.length; j++){
		if(form[j].value != "" && form[j].type == "text" && form[j].name.substr(0,12) == "conteudoitem") {
			form[j].onkeyup();
		}
	}
}
</script>