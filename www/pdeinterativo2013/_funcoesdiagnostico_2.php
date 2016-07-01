<?

function montaGraficoAproveitamentoEstudantes($dados) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$anosDS = array("2010"); 
	
	$colorGraph = array("B" => "#cc1111",
						"E" => "#11cccc",
						"M" => "#1111cc",
						"S" => "#111111");
	
	$arrEns = array("M" => "Ensino Médio",
					"U" => "Ensino Fundamental");
	
	$arrSit = array("A" => "Aprovação",
					"R" => "Reprovação",
					"B" => "Abandono");
	
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	
	foreach($arrEsf as $codesf => $esfera) {
		
		$dadosescensino = $db->carregar("SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
										 LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intensino='".$dados['ensino']."' AND it.intsubmodulo='T' AND it.intano IN('".implode("','",$anosDS)."')");
		
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intesfera']][$dee['intaprrepaba']] = $dee['intvalor'];
			}
		}
	}
	foreach($arrSit as $codsit => $situacao) {	
		foreach($arrEsf as $codesfera => $esfera) {
			$dadosvalores[$codesfera][] = (($arrEscEns[$codesfera][$codsit])?$arrEscEns[$codesfera][$codsit]:0);
		}
		$_x_ax[]  = $situacao;
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	
	// Setup the graph.
	$graph = new Graph(400,120);
	$graph->img->SetMargin(30,95,45,25);
	$graph->SetScale("textlin");
	
	// Set up the title for the graph
	$graph->title->Set("Aproveitamento dos estudantes - ".$arrEns[$dados['ensino']]." (em %)");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,12);
	$graph->title->SetColor("black");
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL,7);
	$graph->xaxis->SetTickLabels($_x_ax);
	$graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
	$graph->legend->SetLineSpacing(5);
	$graph->legend->Pos(0.02,0.3);	
	// Create the bar plots
	foreach($arrEsf as $esfecod => $esfera) {
		$bp = new BarPlot($dadosvalores[$esfecod]);
		$bp->SetColor("white");
		$bp->SetFillColor($colorGraph[$esfecod]);
		$bp->SetLegend($esfera);
		$bp->value->Show();
		$bp->value->SetAngle(90); 
		$bp->value->SetFont(FF_FONT0,FS_NORMAL,7);
		
		$arrPlots[] = $bp; 
	}
	
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot($arrPlots);
	// ...and add it to the graPH
	$graph->Add($gbplot);
	
	// Display the graph
	$graph->Stroke();
	
}

function montaGraficoDistorcao($dados) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$anosDS = array("2012"); 
	
	$colorGraph = array("B" => "#cc1111",
						"E" => "#11cccc",
						"M" => "#1111cc",
						"S" => "#111111");
	
	$arrEns = array("M" => "Ensino Médio",
					"U" => "Ensino Fundamental");
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	
	foreach($arrEsf as $codesf => $esfera) {
		
		$dadosescensino = $db->carregar("SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
										 LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL 
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intensino IN ('".implode("','",array_keys($arrEns))."') AND it.intsubmodulo='D' AND it.intano IN('".implode("','",$anosDS)."')");
		
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intensino']][$dee['intesfera']] = $dee['intvalor'];
			}
		}
	}
	
	foreach($arrEns as $codensino => $ensino) {
		foreach($arrEsf as $esfecod => $esfera) {
			$dadosvalores[$esfecod][] = (($arrEscEns[$codensino][$esfecod])?$arrEscEns[$codensino][$esfecod]:0);
		}
		$_x_ax[]  = $ensino;
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	
	// Setup the graph.
	$graph = new Graph(400,120);
	$graph->img->SetMargin(30,95,10,25);
	$graph->SetScale("textlin");
	
	// Set up the title for the graph
	$graph->title->Set("Distorção Idade-Série (em %)");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,12);
	$graph->title->SetColor("black");
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL,7);
	$graph->xaxis->SetTickLabels($_x_ax);
	$graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
	$graph->legend->SetLineSpacing(5);
	$graph->legend->Pos(0.02,0.3);	
	// Create the bar plots
	foreach($arrEsf as $esfecod => $esfera) {
		//$esfera="tt";
		//echo "<pre>";
		//print_r($dadosvalores[$esfecod]);
		$bp = new BarPlot($dadosvalores[$esfecod]);
		$bp->SetColor("white");
		$bp->SetFillColor($colorGraph[$esfecod]);
		//print_r($esfera);
		$bp->SetLegend($esfera);
		$bp->value->Show();
		$bp->value->SetAngle(90); 
		$bp->value->SetFont(FF_FONT0,FS_NORMAL,7);
		
		$arrPlots[] = $bp; 
	}
	//exit;
	
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot($arrPlots);
	// ...and add it to the graPH
	$graph->Add($gbplot);
	
	// Display the graph
	$graph->Stroke();
	
}



function montaTabelaDistorcao() {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$anosDS = array("2012"); 
	
	$colorGraph = array("B" => "#cc1111",
						"E" => "#11cccc",
						"M" => "#1111cc",
						"S" => "#111111");
	
	$arrEns = array("U" => "Ensino Fundamental",
					"M" => "Ensino Médio"
					);
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	
	foreach($arrEsf as $codesf => $esfera) {
		$sql = "SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
				 LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
				 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL
				 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
				 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intensino IN ('".implode("','",array_keys($arrEns))."') AND it.intsubmodulo='D' AND it.intano IN('".implode("','",$anosDS)."')";
		//dbg($sql);
		$dadosescensino = $db->carregar($sql);
		
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intensino']][$dee['intesfera']] = $dee['intvalor'];
				if($dee['intvalor'] && $dee['intesfera'] == "B"){
					$taxaDistorcaoBrasil[$dee['intensino']] = $dee['intvalor'];
				}
			}
		}
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$html .= "<table class=listagem width=100%>";
	
	$html .= "<tr>
				<td class=SubTituloCentro colspan=3>Distorção Idade-Série (média, em %)</td>
			  </tr>";

	$html .= "<tr>
				<td class=SubTituloCentro colspan=3>Ano referência: ".implode(",",$anosDS)."</td>
			  </tr>";
	
	$html .= "<tr>
				<td class=SubTituloCentro>Esfera</td>
				<td class=SubTituloCentro>Ensino Fundamental</td>
				<td class=SubTituloCentro>Ensino Médio</td>
			  </tr>";
	
	$html .= "</tr>";
	
	foreach($arrEsf as $codesfera => $esfera) {
		$html .= "<tr>";
		$html .= "<td>".$esfera."</td>";
		
		foreach($arrEns as $codensino => $ensino) {
			$html .= "<td align=center>".$arrEscEns[$codensino][$codesfera]."</td>";
		}
		
		$html .= "</tr>";
		
	}
	
	$html .= "</table>";
	
	return array('html' => $html, 'taxaDistorcaoBrasil' => array("Ensino Médio" => $taxaDistorcaoBrasil['M'], "Ensino Fundamental" => $taxaDistorcaoBrasil['U']) );
	
}

function montaTabelaAproveitamentoEstudantes($ensino) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$anosDS = array("2011"); 
	
	$colorGraph = array("B" => "#cc1111",
						"E" => "#11cccc",
						"M" => "#1111cc",
						"S" => "#111111");
	
	$arrEns = array("U" => "Ensino Fundamental",
					"M" => "Ensino Médio"
					);

	$arrSit = array("A" => "Aprovação",
					"R" => "Reprovação",
					"B" => "Abandono");
					
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	
	foreach($arrEsf as $codesf => $esfera) {
		$sql = "SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
										 LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL 
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intensino='".$ensino."' AND it.intsubmodulo='T' AND it.intano IN('".implode("','",$anosDS)."')";
		
		$dadosescensino = $db->carregar($sql);
		
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intesfera']][$dee['intaprrepaba']] = $dee['intvalor'];
				if($dee['intvalor'] && $dee['intesfera'] == "B" && $dee['intaprrepaba'] == "R"){
					$taxaBrasil[$dee['intensino']]['reprovacao'] = $dee['intvalor'];
				}
				if($dee['intvalor'] && $dee['intesfera'] == "B" && $dee['intaprrepaba'] == "B"){
					$taxaBrasil[$dee['intensino']]['abandono'] = $dee['intvalor'];
				}
			}
		}
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$html .= "<table class=listagem width=100%>";
	
	$html .= "<tr>
				<td class=SubTituloCentro colspan=4>Aproveitamento do estudantes (%)</td>
			  </tr>";
	
	$html .= "<tr>
				<td class=SubTituloCentro colspan=4>".$arrEns[$ensino]."</td>
			  </tr>";
	
	$html .= "<tr>
				<td class=SubTituloCentro colspan=4>Ano referência: ".implode(",",$anosDS)."</td>
			  </tr>";
	
	$html .= "<tr>
				<td class=SubTituloCentro>Esfera</td>
				<td class=SubTituloCentro>Aprovação</td>
				<td class=SubTituloCentro>Reprovação</td>
				<td class=SubTituloCentro>Abandono</td>
			  </tr>";
	
	$html .= "</tr>";
	
	foreach($arrEsf as $codesfera => $esfera) {
		$html .= "<tr>";
		$html .= "<td>".$esfera."</td>";
		
		foreach($arrSit as $codsit => $situacao) {
			$html .= "<td align=center>".$arrEscEns[$codesfera][$codsit]."</td>";
		}
		
		$html .= "</tr>";
		
	}
	
	$html .= "</table>";
	
	return array(
					'html' => $html,
					'taxaAbandonoBrasil' => array(
												"Ensino Médio" => $taxaBrasil["M"]['abandono'], 
												"Ensino Fundamental" => $taxaBrasil["U"]['abandono']
												),
					'taxaReprovacaoBrasil' => array(
												"Ensino Médio" => $taxaBrasil["M"]['reprovacao'], 
												"Ensino Fundamental" => $taxaBrasil["U"]['reprovacao']
												)
				);
	
}


?>