<?
/**
 * Função utilizada no momento em que é selecionado um funcionário de plantão
 * 
 * @author Alexandre Dourado
 * @return htmlcode Contendo varios parametros separados por ##
 * @param integer $dados[fcoid] ID do funcionário
 * @param integer $dados[mes]   Mês selecionado 
 * @param integer $dados[ano]   Ano selecionado 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 21/09/2009
 */
function selecionarFuncionarioPlantao($dados) {
	global $db;
	// carregando lista de feriados, indice data
	$sql = "SELECT * FROM rehuf.feriado";
	$feriados = $db->carregar($sql);
	
	if($feriados[0]) {
		foreach($feriados as $fer) {
			$listafer[$fer['ferdata']] = $fer['ferid'];
		}
	}
	// FIM - listaferiados 
	
	// carregando dados do funcionário
	if($dados['fcoid']) {
		$sql = "SELECT c.carnome, c.carnivel, f.fcocodigosiape FROM rehuf.funcionarioplantao f
				LEFT JOIN rehuf.cargoplantao c ON c.carid = f.carid 
				WHERE fcoid='".$dados['fcoid']."'";
		$funcionario = $db->pegaLinha($sql);
	}
	
	// FIM - dados do funcionário
	if($funcionario) {
		$HTML .= trim($funcionario['fcocodigosiape'])."##".trim($funcionario['carnome']);
	} else {
		$HTML .= "##";
	}
	
	// carregando o número de dias em determinado mês e ano
	$ndias = cal_days_in_month(CAL_GREGORIAN, $dados['mes'], $dados['ano']);
	
	// verificando se o funcionário é de nível superior, caso "S"(Sim), o funcionário podera fazer plantão
	// presencial e não-presencial, caso contratario, somente presencial
	if($funcionario['carnivel'] == "S") {
		// combo dos dias uteis
		$selectdutil .= "<select  name='epltipo[".$dados['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltiposuperior' onchange='calcularPlantao(this);'>
						 <option value=''>-</option>
						 <option value='PN'>PN</option>
						 <option value='PD'>PD</option>
						 <option value='PF'>PF</option>
						 <option value='SD'>SD</option>
						 <option value='SF'>SF</option>
						 </select>";
		// combo dos fim de semana e feriados
		$selectfsemferiad .= "<select  name='epltipo[".$dados['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltiposuperior' onchange='calcularPlantao(this);'>
						 	  <option value=''>-</option>
						 	  <option value='PN'>PN</option>
						 	  <option value='PF'>PF</option>
						 	  <option value='SF'>SF</option>
						 	  </select>";
		
		
	} else {
		// combo dos dias uteis
		$selectdutil .= "<select  name='epltipo[".$dados['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltipomedio' onchange='calcularPlantao(this);'>
						 <option value=''>-</option>
						 <option value='PN'>PN</option>
						 <option value='PD'>PD</option>
						 <option value='PF'>PF</option>
						 </select>";
		// combo dos fim de semana e feriados
		$selectfsemferiad .= "<select  name='epltipo[".$dados['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltipomedio' onchange='calcularPlantao(this);'>
						 	  <option value=''>-</option>
						 	  <option value='PN'>PN</option>
						 	  <option value='PF'>PF</option>
						 	  </select>";
	}
	// montando as informações para o GRID do Mês (começando do dia 01 ate ultimo dia do mês $ndias)	
	for($i=1;$i<=$ndias;$i++) {
		if($funcionario) {
			// se for feriado, inserir combo de feriado e fim de semana
			if($listafer[$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]) {
				$HTML .= "##".str_replace(array("{dia}"),array($i),$selectfsemferiad);
			// se for fim de semana, inserir combo de feriado e fim de semana
			}elseif(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
			   		date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
				$HTML .= "##".str_replace(array("{dia}"),array($i),$selectfsemferiad);
			// senão é dia util, inserir combo de dia util
			} else {
				$HTML .= "##".str_replace(array("{dia}"),array($i),$selectdutil);			
			}
		} else {
			$HTML .= "##";
		}

	}
	// imprime a saída HTML
	echo $HTML;
}
/**
 * Função utilizada no momento em que se clica para inserir novo funcionário
 * 
 * @author Alexandre Dourado
 * @return htmlcode Contendo varios parametros separados por ##
 * @param integer $dados[setid] ID do setor
 * @param integer $dados[mes]   Mês selecionado 
 * @param integer $dados[ano]   Ano selecionado 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 21/09/2009
 */
function inserirLinhaPlantao($dados) {
	global $db;
	
	if(validaVariaveisSistema()) {
		echo "<p>Problemas nas variáveis do sistema. <a href='rehuf.php?modulo=inicio&acao=C'><b>Clique aqui</b></a> para refazer o procedimento.</p>";
		exit;
	}
	
	// carregando os feriados
	$sql = "SELECT * FROM rehuf.feriado";
	$feriados = $db->carregar($sql);
	
	if($feriados[0]) {
		foreach($feriados as $fer) {
			$listafer[$fer['ferdata']] = $fer['ferid'];
		}
	}
	// carregando o número de dias do mês	
	$ndias = cal_days_in_month(CAL_GREGORIAN, $dados['mes'], $dados['ano']);
	
	// carregando os funcionários ativos vinculados ao hospital, e que não esteja fazendo escala naquele mês, ano e setor
	$sql = "SELECT f.fcoid as codigo, f.fconome as descricao 
			FROM rehuf.funcionarioplantao f 
			LEFT JOIN rehuf.funcionarioplantaohospital fp ON fp.fcoid=f.fcoid
			WHERE f.fcostatus='A' AND fp.entid='".$_SESSION['rehuf_var']['entid']."' AND f.fcoid NOT IN(SELECT fcoid FROM rehuf.escalaplantao WHERE to_char(epldata,'yyyy-MM')='".$dados['ano']."-".$dados['mes']."' AND setid='".$dados['setid']."' AND entid='".$_SESSION['rehuf_var']['entid']."' GROUP BY fcoid) ORDER BY fconome";
	$funcs = $db->carregar($sql);
	
	$listafunc .= "<select  name='funcs[]'  class='CampoEstilo'  style='width: auto' onchange='selecionarFuncionarioPlantao(this)' id='fcoid'>";
	$listafunc .= "<option value=\"\">Selecione</option>";
	if($funcs[0]) {
		foreach($funcs as $func) {
			$listafunc .= "<option value='".$func['codigo']."'>".$func['descricao']."</option>";		
		}
	}
	$listafunc .= "</select>";
	
	$HTML .= "<img src=\"../imagens/excluir.gif\" onclick=\"excluirLinhaPlantao(this);\" >##<input type=\"text\" size=\"9\" onblur=\"selecionarLinhaPlantaoSIAPE(this);\">##".$listafunc."##&nbsp;##";
	
	for($i=1;$i<=$ndias;$i++) {
		
		if($listafer[$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]) {
			
   			$HTML .= "background-color:#CCFFCC;##";
			
		}elseif(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
		   date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
		   	
   			$HTML .= "background-color:#FFFFCC;##";
   			
		} else {
			$HTML .= "&nbsp;##";			
		}
		

	}
	
	$HTML .= "classname-cellsj:SubTituloCentro##classname-cellsj:SubTituloCentro##classname-cellsj:SubTituloCentro##classname-cellsj:SubTituloCentro##classname-cellsj:SubTituloCentro##classname-cellsj:SubTituloCentro";
	
	echo $HTML;
	
}

/**
 * Função utilizada para criar o GRID do plantão (estrutura da tabela)
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param integer $dados[setid] ID do setor
 * @param integer $dados[mes]   Mês selecionado 
 * @param integer $dados[ano]   Ano selecionado 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 21/09/2009
 */
function gridPlantaoRelatorio($dados) {
	global $db, $_TOTALPER;
	
	ini_set('memory_limit','1024M');
	
	// carregando os feriados
	$sql = "SELECT * FROM rehuf.feriado";
	$feriados = $db->carregar($sql);
	
	if($feriados[0]) {
		foreach($feriados as $fer) {
			$listafer[$fer['ferdata']] = $fer['ferid'];
		}
	}
	
	$ndias = cal_days_in_month(CAL_GREGORIAN, $dados['mes'], $dados['ano']);
	
	// verificando cada dia do mês
	for($i=1;$i<=$ndias;$i++) {
		if($listafer[$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]) {
			$tipo = "feriado";
		}elseif(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
	 		    date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
			
			$tipo = "fimsemana";
		   	
		} else {
			
			$tipo = "normal";
			
		}
		$diasGrid[] = array('tipo'  => $tipo,
							'label' => $i);
	}

	$HTML .= "<table style=\"width:100%\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"tablePlantao\">";
	$HTML .= "<thead>";
	// criando cabeçalho
	$HTML .= "<tr>";
	$HTML .= "<td class=\"SubTituloCentro\" colspan=\"3\" rowspan=\"2\">DADOS</td>";
	$HTML .= "<td class=\"SubTituloCentro\" colspan=\"".count($diasGrid)."\" rowspan=\"2\">DIAS</td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\" colspan=\"6\"><strong><font size=1>Total Plantão (Quantidade de plantões)</font></strong></td>";
	$HTML .= "</tr>";
	
	$HTML .= "<tr>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\" colspan=\"4\"><strong><font size=1>Nível Superior</font></strong></td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\" colspan=\"2\"><strong><font size=1>Nível Médio</font></strong></td>";
	$HTML .= "</tr>";
	
	
	$HTML .= "<tr>";
	$HTML .= "<td class=\"SubTituloCentro\"><font size=1>SIAPE</font></td>";
	$HTML .= "<td class=\"SubTituloCentro\"><font size=1>NOME</font></td>";
	$HTML .= "<td class=\"SubTituloCentro\"><font size=1>CARGO</font></td>";

	foreach($diasGrid as $dias) {
		
		switch($dias['tipo']) {
			case "fimsemana":
				$style = "background-color:#FFFFCC;";
				break;
			case "feriado":
				$style = "background-color:#CCFFCC;";
				break;
			default:
				$style = "";
		}
		
		$HTML .= "<td align=\"center\" style=\"".$style."\"><font size=1>".sprintf("%02d", $dias['label'])."</font></td>";
	}
	
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\"><font size=1>PD</font></td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\"><font size=1>PF</font></td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\"><font size=1>SD</font></td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\"><font size=1>SF</font></td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\"><font size=1>PD</font></td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\"><font size=1>PF</font></td>";
	
	$HTML .= "</tr>";
	$HTML .= "</thead>";
	// fim do cabecalho
	
	$sql = "SELECT * FROM rehuf.escalaplantao e 
			LEFT JOIN rehuf.funcionarioplantao f ON e.fcoid=f.fcoid 
			LEFT JOIN rehuf.cargoplantao c ON c.carid=f.carid  
			WHERE e.entid='".$dados['entid']."' AND to_char(epldata,'YYYY-mm')= '".$dados['ano']."-".$dados['mes']."' AND setid='".$dados['setid']."' 
			ORDER BY f.fconome";
	
	$dadosfun = $db->carregar($sql);
	if($dadosfun[0]) {
		foreach($dadosfun as $fun) {
			$dadosfuncon[$fun['fcoid']]['dadosfuncionario'] = array("fcocodigosiape" => $fun['fcocodigosiape'],
																	"fcoid"          => $fun['fcoid'],
																	"carnome"        => $fun['carnome'],
																	"fconome"        => $fun['fconome'],
																	"carnivel"       => $fun['carnivel'],
																	"carid"          => $fun['carid']);
			$dadosfuncon[$fun['fcoid']]['dadosplantao'][$fun['epldata']] = array("epltipo"=> $fun['epltipo']);
		}
	}
	
	$HTML .= "<tbody>";	
	if($dadosfuncon) {
		foreach($dadosfuncon as $funcon) {
			$HTML .= "<tr>";
			$HTML .= "<td><font size=1>".$funcon['dadosfuncionario']['fcocodigosiape']."</font></td>";
			$HTML .= "<td nowrap><font size=1>".abreviar_nome($funcon['dadosfuncionario']['fconome'])."</font></td>";
			$HTML .= "<td align=center><font size=1>".$funcon['dadosfuncionario']['carnivel'].$funcon['dadosfuncionario']['carid']."</font></td>";
			
			unset($total);
			// verificando cada dia do mês
			for($i=1;$i<=$ndias;$i++) {
				if($listafer[$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]) {
					
					if($funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo']) $selected = $funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo'];
					else  $selected ="";
					
					if($selected) $total[$selected]++;
					
					$HTML .= "<td style=\"background-color:#CCFFCC;text-align:center;border: 1px solid black;\"><font size=1>".$selected."</font></td>";
					
				}elseif(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
				   date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
				   	
					if($funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo']) $selected = $funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo'];
					else  $selected ="";
					
					if($selected) $total[$selected]++;
				   	
					$HTML .= "<td style=\"background-color:#FFFFCC;text-align:center;border: 1px solid black;\"><font size=1>".$selected."</font></td>";
				   	
				} else {
					
					if($funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo']) $selected = $funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo'];
					else  $selected ="";
					
					if($selected) $total[$selected]++;
					
					$HTML .= "<td style=\"text-align:center;border: 1px solid black;\"><font size=1>".$selected."</font></td>";
					
				}
			}
			
			if($funcon['dadosfuncionario']['carnivel'] == "S") {
				$Ttotal['S']['PD'][$funcon['dadosfuncionario']['carid']] += $total['PD']; 
				$Ttotal['S']['PF'][$funcon['dadosfuncionario']['carid']] += $total['PF'];
				$Ttotal['S']['SD'][$funcon['dadosfuncionario']['carid']] += $total['SD'];
				$Ttotal['S']['SF'][$funcon['dadosfuncionario']['carid']] += $total['SF'];
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PD'])?$total['PD']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PF'])?$total['PF']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['SD'])?$total['SD']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['SF'])?$total['SF']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
			} elseif($funcon['dadosfuncionario']['carnivel'] == "M") {
				$Ttotal['M']['PD'][$funcon['dadosfuncionario']['carid']] += $total['PD']; 
				$Ttotal['M']['PF'][$funcon['dadosfuncionario']['carid']] += $total['PF'];
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PD'])?$total['PD']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PF'])?$total['PF']:"")."</td>";
			}
			
			$HTML .= "</tr>";
		}
	} else {
		$HTML .= "<tr>
					<td colspan=\"".($ndias+9)."\" style=\"background-color:#FFFFFF;\">Não existem escalas cadastras ate o momento.</td>
				  </tr>";
	}
	$HTML .= "<tbody>";
	$HTML .= "</table>";
	
	// Neste caso, mostrar apenas o resumo dos plantões (formato definido pelo Dr. Celso)
	if($dados['somentetotalizadores'] == "sim") {
		$cargossuperior = $db->carregar("SELECT carid, carnome FROM rehuf.cargoplantao WHERE carnivel='S' AND carstatus='A'");
		$cargosmedio    = $db->carregar("SELECT carid, carnome FROM rehuf.cargoplantao WHERE carnivel='M' AND carstatus='A'");
		unset($HTML);
		$HTML .= "<table style=\"width:100%\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"tablePlantao\">";
		$HTML .= "<tr><td class=SubTituloCentro rowspan=4>Nível Superior</td><td class=SubTituloDireita>Presencial dias úteis</td><td>";
		if($cargossuperior[0]) {
			$HTML .= "<table width=100%>";
			foreach($cargossuperior as $cs) {
				$HTML .= "<tr><td width=70%>".$cs['carnome']."</td><td>".($Ttotal['S']['PD'][$cs['carid']]+0)."</td></tr>";
			}
			$HTML .= "</table>";
		} else {
			$HTML .= "Não existem cargos";
		}
		$HTML .= "</td></tr><tr><td class=SubTituloDireita>Presencial final de semana e feriados</td><td>";
		if($cargossuperior[0]) {
			$HTML .= "<table width=100%>";
			foreach($cargossuperior as $cs) {
				$HTML .= "<tr><td width=70%>".$cs['carnome']."</td><td>".($Ttotal['S']['PF'][$cs['carid']]+0)."</td></tr>";
			}
			$HTML .= "</table>";
		} else {
			$HTML .= "Não existem cargos";
		}
		$HTML .= "</td></tr><tr><td class=SubTituloDireita>Sobreaviso dias úteis</td><td>";
		if($cargossuperior[0]) {
			$HTML .= "<table width=100%>";
			foreach($cargossuperior as $cs) {
				$HTML .= "<tr><td width=70%>".$cs['carnome']."</td><td>".($Ttotal['S']['SD'][$cs['carid']]+0)."</td></tr>";
			}
			$HTML .= "</table>";
		} else {
			$HTML .= "Não existem cargos";
		}
		$HTML .= "</td></tr><tr><td class=SubTituloDireita>Sobreaviso final de semana e feriados</td><td>";
		if($cargossuperior[0]) {
			$HTML .= "<table width=100%>";
			foreach($cargossuperior as $cs) {
				$HTML .= "<tr><td width=70%>".$cs['carnome']."</td><td>".($Ttotal['S']['SF'][$cs['carid']]+0)."</td></tr>";
			}
			$HTML .= "</table>";
		} else {
			$HTML .= "Não existem cargos";
		}
		$HTML .= "</td></tr><tr><td class=SubTituloCentro rowspan=2>Nível Médio</td><td class=SubTituloDireita>Presencial dias úteis</td><td>";
		if($cargosmedio[0]) {
			$HTML .= "<table width=100%>";
			foreach($cargosmedio as $cm) {
				$HTML .= "<tr><td width=70%>".$cm['carnome']."</td><td>".($Ttotal['M']['PD'][$cm['carid']]+0)."</td></tr>";
			}
			$HTML .= "</table>";
		} else {
			$HTML .= "Não existem cargos";
		}
		$HTML .= "</td></tr><tr><td class=SubTituloDireita>Presencial final de semana e feriados</td><td>";
		if($cargosmedio[0]) {
			$HTML .= "<table width=100%>";
			foreach($cargosmedio as $cm) {
				$HTML .= "<tr><td width=70%>".$cm['carnome']."</td><td>".($Ttotal['M']['PF'][$cm['carid']]+0)."</td></tr>";
			}
			$HTML .= "</table>";
		} else {
			$HTML .= "Não existem cargos";
		}
		$HTML .= "</td></tr></table>";
	}
	
	$_TOTALPER = $Ttotal;
	
	return $HTML;
}


/**
 * Função utilizada para criar o GRID do plantão (estrutura da tabela)
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param integer $dados[setid] ID do setor
 * @param integer $dados[mes]   Mês selecionado 
 * @param integer $dados[ano]   Ano selecionado 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 21/09/2009
 */
/**
 * Função utilizada para criar o GRID do plantão (estrutura da tabela)
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param integer $dados[setid] ID do setor
 * @param integer $dados[mes]   Mês selecionado 
 * @param integer $dados[ano]   Ano selecionado 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 21/09/2009
 */
function gridPlantao($dados) {
	global $db;
	
	ini_set('memory_limit','1024M');
	
	// carregando os feriados
	$sql = "SELECT * FROM rehuf.feriado";
	$feriados = $db->carregar($sql);
	
	if($feriados[0]) {
		foreach($feriados as $fer) {
			$listafer[$fer['ferdata']] = $fer['ferid'];
		}
	}
	
	$ndias = cal_days_in_month(CAL_GREGORIAN, $dados['mes'], $dados['ano']);
	
	// verificando cada dia do mês
	for($i=1;$i<=$ndias;$i++) {
		if($listafer[$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]) {
			$tipo = "feriado";
		}elseif(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
	 		    date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
			
			$tipo = "fimsemana";
		   	
		} else {
			
			$tipo = "normal";
			
		}
		$diasGrid[] = array('tipo'  => $tipo,
							'label' => $i);
	}

	$HTML .= "<table class=\"tabela\" style=\"width:2600px\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"tablePlantao\">";
	
	// criando cabeçalho
	$HTML .= "<tr>";
	$HTML .= "<td class=\"SubTituloCentro\" colspan=\"4\" rowspan=\"2\">DADOS</td>";
	$HTML .= "<td class=\"SubTituloCentro\" colspan=\"".count($diasGrid)."\" rowspan=\"2\">DIAS</td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\" colspan=\"6\"><strong>Total Plantão (Quantidade de plantões)</strong></td>";
	$HTML .= "</tr>";
	
	$HTML .= "<tr>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\" colspan=\"4\"><strong>Nível Superior</strong></td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\" colspan=\"2\"><strong>Nível Médio</strong></td>";
	$HTML .= "</tr>";
	
	
	$HTML .= "<tr>";
	$HTML .= "<td style=\"width:10px;\">&nbsp;</td>";
	$HTML .= "<td class=\"SubTituloCentro\" style=\"width:70px;\">SIAPE</td>";
	$HTML .= "<td class=\"SubTituloCentro\" style=\"width:150px;\">NOME</td>";
	$HTML .= "<td class=\"SubTituloCentro\" style=\"width:150px;\">CARGO</td>";

	foreach($diasGrid as $dias) {
		
		switch($dias['tipo']) {
			case "fimsemana":
				$style = "background-color:#FFFFCC;";
				break;
			case "feriado":
				$style = "background-color:#CCFFCC;";
				break;
			default:
				$style = "";
		}
		
		$HTML .= "<td align=\"center\" style=\"width:42px;".$style."\">".$dias['label']."</td>";
	}
	
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial dias úteis</td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial final de semana e feriados</td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Sobreaviso dias úteis</td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Sobreaviso final de semana e feriados</td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial dias úteis</td>";
	$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial final de semana e feriados</td>";
	
	$HTML .= "</tr>";
	// fim do cabecalho
	
	// validando se as variaveis elementares de sessão estão marcadas
	if(validaVariaveisSistema()) {
		echo "<p>Problemas no carregamento das informações. <a href='?modulo=inicio&acao=C'>Clique aqui</a> e refaça o procedimento.</p>";
		exit;
	}
	
	$sql = "SELECT * FROM rehuf.escalaplantao e 
			LEFT JOIN rehuf.funcionarioplantao f ON e.fcoid=f.fcoid 
			LEFT JOIN rehuf.cargoplantao c ON c.carid=f.carid  
			WHERE e.entid='".$_SESSION['rehuf_var']['entid']."' AND to_char(epldata,'yyyy-MM')= '".$dados['ano']."-".$dados['mes']."' AND setid='".$dados['setid']."' 
			ORDER BY f.fconome";
	$dadosfun = $db->carregar($sql);
	if($dadosfun[0]) {
		foreach($dadosfun as $fun) {
			$dadosfuncon[$fun['fcoid']]['dadosfuncionario'] = array("fcocodigosiape" => $fun['fcocodigosiape'],
																	"fcoid"          => $fun['fcoid'],
																	"carnome"        => $fun['carnome'],
																	"fconome"        => $fun['fconome'],
																	"carnivel"       => $fun['carnivel']);
			$dadosfuncon[$fun['fcoid']]['dadosplantao'][$fun['epldata']] = array("epltipo"=> $fun['epltipo']);
		}
	}
	
	if($dadosfuncon) {
		$controlcabecalho = 0;
		foreach($dadosfuncon as $funcon) {
			
			$controlcabecalho++;
			if($controlcabecalho%8 == 0) {

				$HTML .= "<tr>";
				$HTML .= "<td style=\"width:10px;\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\" style=\"width:70px;\">SIAPE</td>";
				$HTML .= "<td class=\"SubTituloCentro\" style=\"width:150px;\">NOME</td>";
				$HTML .= "<td class=\"SubTituloCentro\" style=\"width:150px;\">CARGO</td>";
			
				foreach($diasGrid as $dias) {
					
					switch($dias['tipo']) {
						case "fimsemana":
							$style = "background-color:#FFFFCC;";
							break;
						case "feriado":
							$style = "background-color:#CCFFCC;";
							break;
						default:
							$style = "";
					}
					
					$HTML .= "<td align=\"center\" style=\"width:42px;".$style."\">".$dias['label']."</td>";
				}
				
				$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial dias úteis</td>";
				$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial final de semana e feriados</td>";
				$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Sobreaviso dias úteis</td>";
				$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Sobreaviso final de semana e feriados</td>";
				$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial dias úteis</td>";
				$HTML .= "<td align=\"center\" style=\"background-color:#808080;\">Presencial final de semana e feriados</td>";
				
				$HTML .= "</tr>";
				
				$controlcabecalho = 0;
				
			}
			
			$HTML .= "<tr>";
			$HTML .= "<td><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirLinhaPlantao(this);\"></td>";
			$HTML .= "<td>".$funcon['dadosfuncionario']['fcocodigosiape']."</td>";
			$HTML .= "<td nowrap><input type='hidden' name='funcs[]' value='".$funcon['dadosfuncionario']['fcoid']."'>".$funcon['dadosfuncionario']['fconome']."</td>";
			$HTML .= "<td nowrap>".$funcon['dadosfuncionario']['carnome']."</td>";
			
			if($funcon['dadosfuncionario']['carnivel'] == "S") {
				$selectdutil = "<select  name='epltipo[".$funcon['dadosfuncionario']['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltiposuperior' onchange='calcularPlantao(this);'>
								 <option value=''>-</option>
						 		 <option value='PN' {selected_PN}>PN</option>
								 <option value='PD' {selected_PD}>PD</option>
								 <option value='PF' {selected_PF}>PF</option>
								 <option value='SD' {selected_SD}>SD</option>
								 <option value='SF' {selected_SF}>SF</option>
								 </select>";
				
				$selectfsemferiad = "<select  name='epltipo[".$funcon['dadosfuncionario']['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltiposuperior' onchange='calcularPlantao(this);'>
								 	  <option value=''>-</option>
						 		 	  <option value='PN' {selected_PN}>PN</option>
								 	  <option value='PF' {selected_PF}>PF</option>
								 	  <option value='SF' {selected_SF}>SF</option>
								 	  </select>";
				
				
			} else {
				$selectdutil = "<select  name='epltipo[".$funcon['dadosfuncionario']['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltipomedio' onchange='calcularPlantao(this);'>
								 <option value=''>-</option>
						 		 <option value='PN' {selected_PN}>PN</option>
								 <option value='PD' {selected_PD}>PD</option>
								 <option value='PF' {selected_PF}>PF</option>
								 </select>";
				
				$selectfsemferiad = "<select  name='epltipo[".$funcon['dadosfuncionario']['fcoid']."][{dia}]'  class='CampoEstilo'  style='width: auto' id='epltipomedio' onchange='calcularPlantao(this);'>
								 	  <option value=''>-</option>
						 		 	  <option value='PN' {selected_PN}>PN</option>
								 	  <option value='PF' {selected_PF}>PF</option>
								 	  </select>";
			}			
			
			unset($total);
			
			// verificando cada dia do mês
			for($i=1;$i<=$ndias;$i++) {
				if($listafer[$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]) {
					
					if($funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo']) $selected = $funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo'];
					else  $selected ="";
					
					if($selected) $total[$selected]++;
					
					$HTML .= "<td style=\"background-color:#CCFFCC;\">".str_replace(array("{dia}", "{selected_".$selected."}"),array($i, "selected"),$selectfsemferiad)."</td>";
					
				}elseif(date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 0 || 
				   date("w", mktime(0,0,0,$dados['mes'],$i,$dados['ano'])) == 6) {
				   	
					if($funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo']) $selected = $funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo'];
					else  $selected ="";
					
					if($selected) $total[$selected]++;
				   	
					$HTML .= "<td style=\"background-color:#FFFFCC;\">".str_replace(array("{dia}", "{selected_".$selected."}"),array($i, "selected"),$selectfsemferiad)."</td>";
				   	
				} else {
					
					if($funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo']) $selected = $funcon['dadosplantao'][$dados['ano']."-".sprintf("%02s",$dados['mes'])."-".sprintf("%02s",$i)]['epltipo'];
					else  $selected ="";
					
					if($selected) $total[$selected]++;
					
					$HTML .= "<td>".str_replace(array("{dia}", "{selected_".$selected."}"),array($i, "selected"),$selectdutil)."</td>";
					
				}
			}
			
			if($funcon['dadosfuncionario']['carnivel'] == "S") {
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PD'])?$total['PD']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PF'])?$total['PF']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['SD'])?$total['SD']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['SF'])?$total['SF']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
			} else {
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PD'])?$total['PD']:"")."</td>";
				$HTML .= "<td class=\"SubTituloCentro\">".(($total['PF'])?$total['PF']:"")."</td>";
			}
			
			$HTML .= "</tr>";
		}
	}
	
	$HTML .= "<tr>";
	$HTML .= "<td><img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"inserirLinhaPlantao('requisicao=inserirLinhaPlantao&mes=".$dados['mes']."&ano=".$dados['ano']."&setid=".$dados['setid']."');\"></td>";
	$HTML .= "<td colspan=\"".(count($diasGrid)+12)."\">&nbsp;</td>";
	$HTML .= "</tr>";
	
	$HTML .= "<tr bgcolor=\"#C0C0C0\">";
	$HTML .= "<td colspan=\"".(count($diasGrid)+13)."\"><input type='submit' value='Salvar'></td>";
	$HTML .= "</tr>";
	
	$HTML .= "</table>";
	
	echo $HTML;
}

/**
 * Função utilizada inserir dos dados na escala do plantão
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param integer $dados[setid]     ID do setor
 * @param integer $dados[mes]       Mês selecionado 
 * @param integer $dados[ano]       Ano selecionado 
 * @param array $dados[epltipo]     Tipo de escala 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 21/09/2009
 */
function inserirEscalaPlantao($dados) {
	global $db;
	$dadosp = explode("-", $dados['periodo']);
	$dados['ano'] = $dadosp[0];
	$dados['mes'] = $dadosp[1];
	
	if(validaVariaveisSistema()) {
		echo "<script>
				alert('Problemas nas variáveis do sistema.');
				window.location='?modulo=inicio&acao=A';
			  </script>";
		exit;
	}
	
	$sql = "DELETE FROM rehuf.escalaplantao WHERE setid='".$dados['setid']."' AND entid='".$_SESSION['rehuf_var']['entid']."' AND to_char(epldata,'yyyy-MM')= '".$dados['ano']."-".$dados['mes']."'";
	$db->executar($sql);
	if($dados['epltipo']) {
		foreach($dados['epltipo'] as $fcoid => $fu) {
			foreach($fu as $dia => $epltipo) {
				if($epltipo) {
					$sql = "INSERT INTO rehuf.escalaplantao(
				            fcoid, setid, entid, epldata, epltipo, eplstatus)
				    		VALUES ('".$fcoid."', '".$dados['setid']."', '".$_SESSION['rehuf_var']['entid']."', '".$dados['ano']."-".$dados['mes']."-".sprintf("%02s",$dia)."', '".$epltipo."', 'A');";
					$db->executar($sql);
				}
			}
		}
	}
	$db->commit();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='?modulo=plantao/plantao&acao=A&setid=".$dados['setid']."&periodo=".$dados['periodo']."';
		  </script>";
	exit;
}
function inserirPeriodosPlantao($dados) {
	global $db;
	
	$sql = "INSERT INTO rehuf.periodoplantao(ppldata, pplstatus)
    		VALUES ('".$dados['ano']."-".$dados['mes']."-01', 'A') RETURNING pplid;";
	
	$pplid = $db->pegaUm($sql);
	
	$sql = "SELECT ent.entid FROM entidade.entidade ent 
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
			LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
			WHERE fen.funid = '".HOSPITALUNIV."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true)";

	$_hospitais = $db->carregar($sql);

	if($dados['pplpreenchimentoinicial'] && $dados['pplpreenchimentofinal']) {
		if($_hospitais[0]) {
			foreach($_hospitais as $hosp) {
				
	
				$sql = "INSERT INTO rehuf.periodoplantaodata(
	            		pplid, entid, pplpreenchimentoinicial, pplpreenchimentofinal)
	    				VALUES ('".$pplid."', '".$hosp['entid']."', '".formata_data_sql($dados['pplpreenchimentoinicial'])."', '".formata_data_sql($dados['pplpreenchimentofinal'])."');";
				
				$db->executar($sql);			
			}
		}
	
		$db->commit();
		
		echo "<script>
				alert('Período inserido com sucesso');
				window.location='?modulo=plantao/cadastroperiodo&acao=A';
			  </script>";
		
	} else {
		
		echo "<script>
				alert('Período não pode ser inserido em branco');
				window.location='?modulo=plantao/cadastroperiodo&acao=A';
			  </script>";
		
	}
	
	exit;
}

function excluirPeriodosPlantao($dados) {
	global $db;
	$sql = "UPDATE rehuf.periodoplantao SET pplins=NOW(), pplstatus='I' WHERE pplid='".$dados['pplid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Período removido com sucesso');
			window.location='?modulo=plantao/cadastroperiodo&acao=A';
		  </script>";
	
	exit;
	
}

function atualizarPeriodosPlantao($dados) {
	global $db;
	
	$sql = "UPDATE rehuf.periodoplantao SET pplins=NOW(), ppldata='".$dados['ano']."-".$dados['mes']."-01' WHERE pplid='".$dados['pplid']."'";
	$db->executar($sql);
	
	$periodoembranco = false;
	if($dados['pplpreenchimentoinicial']) {
		foreach(array_keys($dados['pplpreenchimentoinicial']) as $entid) {
			
			if($dados['pplpreenchimentoinicial'][$entid] && $dados['pplpreenchimentofinal'][$entid]) {
				
				$sql = "UPDATE rehuf.periodoplantaodata
	   					SET pplins=NOW(), pplpreenchimentoinicial='".formata_data_sql($dados['pplpreenchimentoinicial'][$entid])."', pplpreenchimentofinal='".formata_data_sql($dados['pplpreenchimentofinal'][$entid])."'
	 					WHERE pplid='".$dados['pplid']."' AND entid='".$entid."'";
				$db->executar($sql);
				
			} else {
				$periodoembranco = true;
			}
		}
	}

	$db->commit();	
	echo "<script>
			".(($periodoembranco)?"alert('Os períodos em branco não podem ser atualizados');":"")."
			alert('Período atualizado com sucesso');
			window.location='?modulo=plantao/cadastroperiodo&acao=A';
		  </script>";
	exit;
}

function inserirCargosPlantao($dados) {
	global $db;
	$sql = "INSERT INTO rehuf.cargoplantao(
            carnome, carstatus, carnivel)
		    VALUES ('".$dados['carnome']."', 'A', '".$dados['carnivel']."') RETURNING carid;";
	$dados['carid'] = $db->pegaUm($sql);
	
	if($dados['entid']) {
		foreach($dados['entid'] as $entid) {
			$sql = "INSERT INTO rehuf.cargohospitalplantao(carid, entid) VALUES ('".$dados['carid']."', '".$entid."');";
			$db->executar($sql);
		}
	}
	$db->commit();
	
	echo "<script>
			alert('Cargo inserido com sucesso');
			window.location='?modulo=plantao/cadastrocargos&acao=A';
		  </script>";
	exit;
}

function atualizarCargosPlantao($dados) {
	global $db;
	$sql = "UPDATE rehuf.cargoplantao SET carins=NOW(), carnome='".$dados['carnome']."', carnivel='".$dados['carnivel']."' 
			WHERE carid='".$dados['carid']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM rehuf.cargohospitalplantao WHERE carid='".$dados['carid']."'";
	$db->executar($sql);
	
	if($dados['entid'][0]) {
		
		foreach($dados['entid'] as $entid) {
			$dadosconsolidados[$entid] = $entid;
		}
		
		foreach($dadosconsolidados as $entid) {
			$sql = "INSERT INTO rehuf.cargohospitalplantao(carid, entid) VALUES ('".$dados['carid']."', '".$entid."');";
			$db->executar($sql);
		}
	}
	$db->commit();	
	echo "<script>
			alert('Cargo atualizado com sucesso');
			window.location='?modulo=plantao/cadastrocargos&acao=A';
		  </script>";
	exit;
}

function excluirCargosPlantao($dados) {
	global $db;
	$sql = "UPDATE rehuf.cargoplantao SET carins=NOW(), carstatus='I' WHERE carid='".$dados['carid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Cargo removido com sucesso');
			window.location='?modulo=plantao/cadastrocargos&acao=A';
		  </script>";
	exit;
}

function inserirSetoresPlantao($dados) {
	global $db;
	$sql = "INSERT INTO rehuf.setorplantao(
            setnome, setstatus)
    		VALUES ('".$dados['setnome']."', 'A') RETURNING setid;";
	$dados['setid'] = $db->pegaUm($sql);
	
	if(trim($dados['entid'][0])) {
		foreach($dados['entid'] as $entid) {
			$sql = "INSERT INTO rehuf.setorhospitalplantao(setid, entid) VALUES ('".$dados['setid']."', '".$entid."');";
			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	echo "<script>
			alert('Setor inserido com sucesso');
			window.location='?modulo=plantao/cadastrosetor&acao=A';
		  </script>";
	exit;
}

function atualizarSetoresPlantao($dados) {
	global $db;
	$sql = "UPDATE rehuf.setorplantao SET setins=NOW(), setnome='".$dados['setnome']."' 
			WHERE setid='".$dados['setid']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM rehuf.setorhospitalplantao WHERE setid='".$dados['setid']."'";
	$db->executar($sql);
	
	if($dados['entid'][0]) {
		foreach($dados['entid'] as $entid) {
			$sql = "INSERT INTO rehuf.setorhospitalplantao(setid, entid) VALUES ('".$dados['setid']."', '".$entid."');";
			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	echo "<script>
			alert('Setor atualizado com sucesso');
			window.location='?modulo=plantao/cadastrosetor&acao=A';
		  </script>";
	exit;

}

function excluirSetoresPlantao($dados) {
	global $db;
	$sql = "UPDATE rehuf.setorplantao SET setins=NOW(), setstatus='I' 
			WHERE setid='".$dados['setid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Setor removido com sucesso');
			window.location='?modulo=plantao/cadastrosetor&acao=A';
		  </script>";
	exit;
}

function inserirFeriadosPlantao($dados) {
	global $db;
	$sql = "INSERT INTO rehuf.feriado(
            entid, ferdata)
    		VALUES (".(($dados['entid'])?"'".$dados['entid']."'":"NULL").", '".formata_data_sql($dados['ferdata'])."');";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Feriado inserido com sucesso');
			window.location='?modulo=plantao/cadastroferiados&acao=A';
		  </script>";
	exit;
}

function atualizarFeriadosPlantao($dados) {
	global $db;
	$sql = "UPDATE rehuf.feriado SET ferins=NOW(), ferdata='".formata_data_sql($dados['ferdata'])."', entid=".(($dados['entid'])?"'".$dados['entid']."'":"NULL")."  
			WHERE ferid='".$dados['ferid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Feriado atualizado com sucesso');
			window.location='?modulo=plantao/cadastroferiados&acao=A';
		  </script>";
	exit;

}

function excluirFeriadosPlantao($dados) {
	global $db;
	$sql = "DELETE FROM rehuf.feriado WHERE ferid='".$dados['ferid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Feriado removido com sucesso');
			window.location='?modulo=plantao/cadastroferiados&acao=A';
		  </script>";
	exit;
}

function verificarSIAPE($codsiape, $fcoid = false) {
	global $db;
	$sql = "SELECT fcoid FROM rehuf.funcionarioplantao WHERE TRIM(fcocodigosiape)='".trim($codsiape)."'".(($fcoid)?" AND fcoid!='".$fcoid."'":"");
	$fcoid = $db->pegaUm($sql);
	return $fcoid; 
}

function inserirFuncionariosPlantao($dados) {
	global $db;
	
	if(!$_SESSION['rehuf_var']['entid']) {
		echo "<script>alert('Hospital não identificado. Problemas de navegação, acesse novamente.');window.opener.location.href='rehuf.php?modulo=inicio&acao=C';window.close();</script>";
		exit;
	}
	
	$fcoid = verificarSIAPE($dados['fcocodigosiape']);
	if($fcoid) {

		$sql = "UPDATE rehuf.funcionarioplantao SET fcoins=NOW(), fcostatus='A' WHERE fcoid='".$fcoid."'";
		$db->executar($sql);
		
		$sql = "DELETE FROM rehuf.funcionarioplantaohospital WHERE entid='".$_SESSION['rehuf_var']['entid']."' AND fcoid='".$fcoid."'";
		$db->executar($sql);
		
		$sql = "INSERT INTO rehuf.funcionarioplantaohospital(fcoid, entid) VALUES ('".$fcoid."', '".$_SESSION['rehuf_var']['entid']."');";
		$db->executar($sql);
		
		$db->commit();
		
		echo "<script>
				alert('Código do SIAPE existente. Funcionário ativado neste hospital com sucesso');
				window.opener.location.href='?modulo=plantao/cadastrofuncionarios&acao=A';
				window.close();
			  </script>";
		
	} else {
		
		$sql = "INSERT INTO rehuf.funcionarioplantao(
	            carid, fcocodigosiape, fconome, fcostatus)
	    		VALUES ('".$dados['carid']."', '".$dados['fcocodigosiape']."', '".$dados['fconome']."', 'A') RETURNING fcoid;";
		
		$fcoid = $db->pegaUm($sql);
		
		$sql = "INSERT INTO rehuf.funcionarioplantaohospital(fcoid, entid) VALUES ('".$fcoid."', '".$_SESSION['rehuf_var']['entid']."');";
		$db->executar($sql);
		
		$db->commit();
		echo "<script>
				alert('Funcionário inserido com sucesso');
				window.opener.location.href='?modulo=plantao/cadastrofuncionarios&acao=A';
				window.close();
			  </script>";
		
	}
	exit;
}

function atualizarFuncionariosPlantao($dados) {
	global $db;
	$fcoid = verificarSIAPE($dados['fcocodigosiape'], $dados['fcoid']);
	if($fcoid) {
		echo "<script>alert('Código do SIAPE ja existe');window.close();</script>";
	} else {
		$sql = "UPDATE rehuf.funcionarioplantao SET fcoins=NOW(), fcocodigosiape='".trim($dados['fcocodigosiape'])."', fconome='".$dados['fconome']."', carid='".$dados['carid']."' WHERE fcoid='".$dados['fcoid']."'";
		$db->executar($sql);
		$db->commit();
		echo "<script>alert('Funcionário atualizado com sucesso');window.opener.location.href='?modulo=plantao/cadastrofuncionarios&acao=A';window.close();</script>";
	}
}

function excluirFuncionariosPlantao($dados) {
	global $db;
	$sql = "UPDATE rehuf.funcionarioplantao SET fcoins=NOW(), fcostatus='I' WHERE fcoid='".$dados['fcoid']."'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Funcionário removido com sucesso');
			window.location='?modulo=plantao/cadastrofuncionarios&acao=A';
		  </script>";
	exit;
}

function selecionarLinhaPlantaoSIAPE($dados) {
	global $db;
	if($_SESSION['rehuf_var']['entid']) {
		
		$sql = "SELECT fcoid FROM rehuf.funcionarioplantao WHERE fcostatus='A' AND TRIM(fcocodigosiape)=TRIM('".$dados['fcocodigosiape']."')";
		$funcionario = $db->pegaLinha($sql);
		
		if($funcionario['fcoid']) {
			
			$sql = "SELECT f.fcoid FROM rehuf.funcionarioplantao f 
					LEFT JOIN rehuf.funcionarioplantaohospital fp ON fp.fcoid=f.fcoid 
					WHERE f.fcoid='".$funcionario['fcoid']."' AND fp.entid='".$_SESSION['rehuf_var']['entid']."'";
			$funent = $db->pegaLinha($sql);
			
			if($funent['fcoid'])
				echo $funent['fcoid'];
			else echo "outrohospital";
			
		} else echo "naoexiste";
		
	} else echo "entidadenaoexiste";

}

function carregardadosmenuplantao() {
	// monta menu padrão contendo informações sobre as entidades
	$menu = array(0 => array("id" => 1, "descricao" => "Lista de hospitais",   "link" => "/rehuf/rehuf.php?modulo=inicio&acao=C"),
				  1 => array("id" => 2, "descricao" => "Plantões", 			   "link" => "/rehuf/rehuf.php?modulo=plantao/plantao&acao=A"),
				  2 => array("id" => 3, "descricao" => "Lista de Funcionário", "link" => "/rehuf/rehuf.php?modulo=plantao/cadastrofuncionarios&acao=A")
			  	  );
	return $menu;
}

function abreviar_nome ($nome){
	// divide o nome pelo espaço entre os mesmos
	$partes_nome     = explode (" ",trim($nome));// pega o total de palavras do nome
	$total           = count($partes_nome);
	$vetor_ignora     = array('de', 'da', 'das', 'do', 'dos');
	
	foreach ($partes_nome as $indice =>$palavras){
		// nao permite que seja abreviado o primeiro, nem o ultimo nome  
		if($indice!=0 && $indice!=($total-1)) // verifica se 'de', 'do' ou otras ligações estão presentes no nome      
			if(in_array($palavras,$vetor_ignora)) {          
				$nome_abrv .= " ".$palavras;                
			} else {                    
				$nome_abrv.= " ". strtoupper(substr($palavras,0,1)).".";               
			}
	}

	$abreviado = ucfirst($partes_nome[0])." ".$nome_abrv." ".ucfirst($partes_nome[$total-1]);

	return $abreviado;
}

function gerarmodeloimportacao($dados) {
	global $db;
	
	if($dados['downloadcsv']) {
		
		header("Content-Type: text/html; charset=ISO-8859-1");
		header("Content-type: application/octet-stream");
		header("Content-Disposition: attachment; filename=\"set_".$dados['setid']."_per_".$dados['periodo'].".csv\"");
		
		$dt = $db->pegaUm("SELECT ppldata FROM rehuf.periodoplantao WHERE pplid='".$dados['periodo']."'");
		$dat_parts = explode("-",$dt);
		
		$num = cal_days_in_month(CAL_GREGORIAN, $dat_parts[1], $dat_parts[0]);
		
		$baseper = $db->pegaUm("SELECT to_char(ppldata,'YYYY-mm') as per FROM rehuf.periodoplantao WHERE pplid='".$dados['periodo']."'");
		
		$sql = "SELECT * FROM rehuf.escalaplantao e
				LEFT JOIN rehuf.funcionarioplantao f ON f.fcoid = e.fcoid 
				WHERE e.entid='".$_SESSION['rehuf_var']['entid']."' AND setid='".$dados['setid']."' AND to_char(epldata,'YYYY-mm')='".$baseper."'
				ORDER BY f.fconome";
		
		$plantoes = $db->carregar($sql);
		
		if($plantoes[0]) {
			foreach($plantoes as $pl) {
				$registropl[$pl['fcocodigosiape']]['fconome'] = $pl['fconome'];
				$registropl[$pl['fcocodigosiape']][$pl['epldata']] = $pl['epltipo'];
			}
		}
		
		for($i=1;$i<=$num;$i++) {
			$tit[] = $i;
		}
		
		echo "Período;Setor;SIAPE;".implode(";",$tit)."\n";
				
		if($registropl) {

			foreach($registropl as $codigosiape => $arr) {
				unset($dad);
								
				for($i=1;$i<=$num;$i++) {
					$dad[] = $arr[$baseper."-".sprintf("%02s",$i)];
				}
				echo $dados['periodo'].";".$dados['setid'].";".$codigosiape.";".implode(";",$dad).";*".$arr['fconome']."\n";
			}
			
		} else {
			
			$sql = "SELECT * FROM rehuf.funcionarioplantao f 
					INNER JOIN rehuf.funcionarioplantaohospital p ON f.fcoid = p.fcoid 
					WHERE p.entid='".$_SESSION['rehuf_var']['entid']."' 
					ORDER BY f.fconome";
			
			$funcionarios = $db->carregar($sql);
			
			if($funcionarios[0]) {
				foreach($funcionarios as $func) {
					
					unset($dad);
									
					for($i=1;$i<=$num;$i++) {
						$dad[] = "-";
					}
					echo $dados['periodo'].";".$dados['setid'].";".$func['fcocodigosiape'].";".implode(";",$dad).";*".$func['fconome']."\n";
					
				}
			}
		
			
		}	
		
		
		
	} else {
	
		echo "<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>";
		echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">';
		echo '<tr><td class="SubTituloCentro">Manual de importação de dados no plantão</td></tr>';
		echo '<tr><td>';
		
		echo "<p><b>Período e Setor:</b> Estes dados são carregados automaticamente.</p>";
		echo "<p><b>SIAPE:</b> Código do SIAPE referente ao funcionário. CF deverá ser trocado pelo código do SIAPE.</p>";
		echo "<p>
				<b>Dias do mês:</b> CP deve ser trocado pelo código do plantão. Os códigos são:<br><br>
				<fieldset style=width:220px;text-align:left;><legend>Legenda</legend>PN - Plantão normal<br />PD - Plantão presencial dias úteis<br />PF - Plantão presencial final de semana e feriados<br />SD - Plantão sobreaviso dias úteis<br />SF - Plantão sobreaviso final de semana e feriados</fieldset>
			  </p>";
		
		$dt = $db->pegaUm("SELECT ppldata FROM rehuf.periodoplantao WHERE pplid='".$dados['periodo']."'");
		$dat_parts = explode("-",$dt);
		
		$num = cal_days_in_month(CAL_GREGORIAN, $dat_parts[1], $dat_parts[0]);

		for($i=1;$i<=$num;$i++) {
			$tit[] = $i;
			$dad[] = "CP";
		}
		
		echo "<b>Modelo:</b><br><br>";
		
		echo "Período;Setor;SIAPE;".implode(";",$tit)."<br>";
		echo $dados['periodo'].";".$dados['setid'].";CF;".implode(";",$dad)."<br>";	
		echo $dados['periodo'].";".$dados['setid'].";CF;".implode(";",$dad)."<br>";
		echo $dados['periodo'].";".$dados['setid'].";CF;".implode(";",$dad)."<br>";
		echo "...<br>";
		echo "..<br>";
		echo ".<br>";
		echo "<br>";
		echo "<b>Exemplo:</b><br><br>";		
		echo "Período;Setor;SIAPE;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23;24;25;26;27;28;29;30;31<br>";
		echo "6;20;1421119;PN;;;PN;;;PN;;;;;PD;;;;PD;;;;;;;;;;;;;;;<br>";
		echo "6;20;0335616;;;;PD;;;;;;;;;PD;;;;;;;;PD;;;;;PD;;;;;<br>";
		echo "<br>";
		echo "<b>Observação:</b> Manter o cabeçalho no arquivo CSV";
		echo '</td></tr>';
		echo '<tr><td class="SubTituloCentro"><input type=button name=xls value="Donwload CSV" onclick="window.location=\'rehuf.php?modulo=plantao/importarplantao&acao=A&requisicao=gerarmodeloimportacao&downloadcsv=true&setid='.$dados['setid'].'&periodo='.$dados['periodo'].'\';"> <input type=button value="Voltar" onclick="window.location=\'rehuf.php?modulo=plantao/importarplantao&acao=A\';"></td></tr>';
		echo '</table>';
	
	}
	
}


function carregarmodeloimportacao($dados) {
	global $db;
	
	$f = file($_FILES['arquivo']['tmp_name']);
	
	for($i=1;$i<count($f);$i++) {
		$line = $f[$i];
		$cols = explode(";",$line);
		$_dados[$cols[0]][$cols[1]][] = $cols;
	}
	
	// carregando os feriados
	$sql = "SELECT * FROM rehuf.feriado";
	$feriados = $db->carregar($sql);
	
	if($feriados[0]) {
		foreach($feriados as $fer) {
			$listafer[$fer['ferdata']] = $fer['ferid'];
		}
	}
	
	foreach($_dados as $pplid => $d1) {
		$ppldata = $db->pegaUm("SELECT to_char(ppldata,'yyyy-MM') as ppldata FROM rehuf.periodoplantao WHERE pplid='".$pplid."'");
		foreach($d1 as $setid => $d2) {
			
			$sql = "DELETE FROM rehuf.escalaplantao WHERE setid='".$setid."' AND entid='".$_SESSION['rehuf_var']['entid']."' AND to_char(epldata,'yyyy-MM')= '".$ppldata."'";
			$db->executar($sql);
			
			foreach($d2 as $d3) {
				
				
				$dat_parts = explode("-",$ppldata);
				$num = cal_days_in_month(CAL_GREGORIAN, $dat_parts[1], $dat_parts[0]);
				
				$sql = "select fp.fcoid, cp.carnivel from rehuf.funcionarioplantao fp 
						inner join rehuf.funcionarioplantaohospital fh on fp.fcoid = fh.fcoid 
						inner join rehuf.cargoplantao cp on cp.carid = fp.carid 
						where fh.entid='".$_SESSION['rehuf_var']['entid']."' and fp.fcocodigosiape='".$d3[2]."'";
				
				$arrfun = $db->pegaLinha($sql);
				
				if(!$arrfun) $erro[] = "SIAPE ".$d3[2]." não esta cadastrado para este hospital";
				
				for($i=1;$i<=$num;$i++) {
					
					if(trim($d3[$i+2])) {
						
						if(date("w", mktime(0,0,0,$dat_parts[1],$i,$dat_parts[0])) == 0 || 
	 		    		   date("w", mktime(0,0,0,$dat_parts[1],$i,$dat_parts[0])) == 6 ||
	 		    		   $listafer[$dat_parts[0]."-".sprintf("%02s",$dat_parts[1])."-".sprintf("%02s",$i)]) {
	 		    		   	
	 		    		   	switch($arrfun['carnivel']) {
	 		    		   		case 'S':
	 		    		   			if(trim($d3[$i+2])!='PN' && trim($d3[$i+2])!='PF' && trim($d3[$i+2])!='SF') $erro[] = "SIAPE ".$d3[2].", Data ".sprintf("%01s",$i)."/".$dat_parts[1]."/".$dat_parts[0]." : Plantão (".$d3[$i+2].") é inválido (Nível Superior) para o fim de semana/feriado";
	 		    		   			break;
	 		    		   		case 'M':
	 		    		   			if(trim($d3[$i+2])!='PN' && trim($d3[$i+2])!='PF') $erro[] = "SIAPE ".$d3[2].", Data ".sprintf("%01s",$i)."/".$dat_parts[1]."/".$dat_parts[0]." : Plantão (".$d3[$i+2].") é inválido (Nível Médio) para o fim de semana/feriado";
	 		    		   			break;
	 		    		   	}
	 		    		   	
	 		    		}
						
	 		    		if(trim($d3[$i+2])!='PN' && trim($d3[$i+2])!='PF' && trim($d3[$i+2])!='SF' && trim($d3[$i+2])!='PD' && trim($d3[$i+2])!='SD') $erro[] = "SIAPE ".$d3[2]." possui um plantão desconhecido (Data ".sprintf("%01s",$i)."/".$dat_parts[1]."/".$dat_parts[0]." : ".trim($d3[$i+2]).")";
						
						$sql_i .= "INSERT INTO rehuf.escalaplantao(
					            fcoid, setid, entid, epldata, epltipo, eplstatus)
					    		VALUES ('".$arrfun['fcoid']."', '".$setid."', '".$_SESSION['rehuf_var']['entid']."', '".$ppldata."-".sprintf("%02s",$i)."', '".trim($d3[$i+2])."', 'A');";
						
					}
					
				}
			}
			
		}
	}
	
	if(!$erro) {
		$db->executar($sql_i);
		$db->commit();
		
		echo "<script>
				alert('Importação fetia com sucesso');
				window.close();
		  	  </script>";
		
	} else {
		echo "<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>";
		echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		
		echo "ERROS;<br>";
		echo "<pre>";
		print_r($erro);
	}
	
	
	
	
	
}

?>