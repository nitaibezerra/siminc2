<?
/**
 * Função que gerencia a gravação dos dados referente a tela 1.4.Sintese da dimensão
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param $dados => array contendo as informações de todos os checkbox referentes aos problemas criticos
 * @global $db classe que instância o banco de dados 
 * @version v1.0 20/07/2011
 */
function diagnostico_1_4_sintesedimensao1($dados,$salvaAba = true) {
	global $db;
	
	if($dados['respostaideb']) {
		foreach($dados['respostaideb'] as $campo => $ideb) {
			$db->executar("UPDATE pdeinterativo2013.respostaideb SET ".$campo."=".$ideb." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($dados['respostataxarendimento']) {
		foreach($dados['respostataxarendimento'] as $campo => $tx) {
			$db->executar("UPDATE pdeinterativo2013.respostataxarendimento SET ".$campo."=".$tx." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($dados['respostaprovabrasil']) {
		foreach($dados['respostaprovabrasil'] as $campo => $pb) {
			$db->executar("UPDATE pdeinterativo2013.respostaprovabrasil SET ".$campo."=".$pb." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($salvaAba){
		salvarAbaResposta("diagnostico_1_4_sintesedimensao1");
	
		echo "<script>
				alert('Dados gravados com sucesso');
				window.location='".$dados['togo']."';
			  </script>";
	}

}

/**
 * Função que verifica se existe algum programa em determinado submodulo
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param $dados => array contendo a sigla do submodulo
 * @global $db classe que instância o banco de dados 
 * @version v1.0 20/07/2011
 */
function existePrograma($dados) {
	global $db;
	$sql = "SELECT rppresposta FROM pdeinterativo2013.respostaprojetoprograma 
			WHERE rppmodulo='".$dados['rppmodulo']."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rppstatus='A' AND rpptipo='G' LIMIT 1";
	
	$rppresposta = $db->pegaUm($sql);
	
	if($dados['exibirresposta']) {
		echo $rppresposta;
	} else {
		return $rppresposta;		
	}
}

/**
 * Função que verifica se existe algum projeto em determinado submodulo
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param $dados => array contendo a sigla do submodulo
 * @global $db classe que instância o banco de dados 
 * @version v1.0 20/07/2011
 */
function existeProjeto($dados) {
	global $db;
	
	$sql = "SELECT rppresposta FROM pdeinterativo2013.respostaprojetoprograma 
			WHERE rppmodulo='".$dados['rppmodulo']."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rppstatus='A' AND rpptipo='J' LIMIT 1";
	
	$rppresposta = $db->pegaUm($sql);
	
	if($dados['exibirresposta']) {
		echo $rppresposta;
	} else {
		return $rppresposta;		
	}

	
}

/**
 * Função que atualiza a sintese dos programas, esta função é generica e utilizada em todas as sinteses
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param $dados => array contendo todas as informações referentes a sintese do programa (pdeinterativo2013.sinteseprograma)
 * @global $db classe que instância o banco de dados 
 * @version v1.0 20/07/2011
 */
function atualizarPrograma($dados) {
	global $db;
	$sql = "UPDATE pdeinterativo2013.sinteseprograma
		    SET proid='".$dados['proid']."', spodesc=".(($dados['spodesc'])?"'".$dados['spodesc']."'":"NULL").", 
		    	spoobjepro='".$dados['spoobjepro']."', sposituacao='".$dados['sposituacao']."', spoorgao=".(($dados['spoorgao'])?"'".$dados['spoorgao']."'":"NULL").", 
		    	sposite=".(($dados['sposite'])?"'".$dados['sposite']."'":"NULL")."
 			WHERE spoid='".$dados['spoid']."';";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Programa atualizado com sucesso');
			window.opener.carregarProgramas('".$dados['sprmodulo']."');
			window.close();
		  </script>";
}


function inserirPrograma($dados) {
	global $db;
	
	$sql = "INSERT INTO pdeinterativo2013.sinteseprograma(
            pdeid, spostatus, sprmodulo, spodesc, spoobjepro, sposituacao, 
            spoorgao, sposite, proid)
		    VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."', 'A', '".$dados['sprmodulo']."', ".(($dados['spodesc'])?"'".$dados['spodesc']."'":"NULL").", ".(($dados['spoobjepro'])?"'".$dados['spoobjepro']."'":"NULL").", ".(($dados['sposituacao'])?"'".$dados['sposituacao']."'":"NULL").", 
		    		".(($dados['spoorgao'])?"'".$dados['spoorgao']."'":"NULL").", ".(($dados['sposite'])?"'".$dados['sposite']."'":"NULL").", '".$dados['proid']."');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Programa inserido com sucesso');
			window.opener.carregarProgramas('".$dados['sprmodulo']."');
			window.close();
		  </script>";
	
}


function excluirPrograma($dados) {
	global $db;
	$sql = "UPDATE pdeinterativo2013.sinteseprograma SET spostatus='I' WHERE spoid='".$dados['spoid']."'";
	$db->executar($sql);
	$db->commit();

	echo "Programa removido com sucesso";
	
}

function excluirProjeto($dados) {
	global $db;
	
	$sql = "UPDATE pdeinterativo2013.sinteseprojeto SET sprstatus='I' WHERE sprid='".$dados['sprid']."'";
	$db->executar($sql);
	$db->commit();

	echo "Projeto removido com sucesso";
	
}

function atualizarProjeto($dados) {
	global $db;
	$sql = "UPDATE pdeinterativo2013.sinteseprojeto
   			SET sprdesc='".$dados['sprdesc']."', sprobjetivo='".$dados['sprobjetivo']."'
 			WHERE sprid='".$dados['sprid']."';";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Projeto atualizado com sucesso');
			window.opener.carregarProjetos('".$dados['sprmodulo']."');
			window.close();
		  </script>";
}

function inserirProjeto($dados) {
	global $db;
	
	$sql = "INSERT INTO pdeinterativo2013.sinteseprojeto(
            pdeid, sprmodulo, sprdesc, sprobjetivo, sprstatus)
    		VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."', '".$dados['sprmodulo']."', '".$dados['sprdesc']."', '".$dados['sprobjetivo']."', 'A');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Projeto inserido com sucesso');
			window.opener.carregarProjetos('".$dados['sprmodulo']."');
			window.close();
		  </script>";
}

function gerenciarProgramas($dados) {
	global $db;

	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<script language=\"JavaScript\" src=\"./js/pdeinterativo2013.js\"></script>";
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script>";
	
	if(!$_GET['sprmodulo']){
		echo "alert('Aba não encontrada!');window.close();</script>";
		exit;
	}
	
	$programas = $db->carregar("SELECT * 
				   				FROM pdeinterativo2013.programa WHERE prostatus='A' order by prodesc");
	
	if(!$programas){
		echo "alert('Não existem programas cadastrados!');window.close();</script>";
		exit;
	}
	
	if($programas[0]) {
		echo "var resp = new Array();";
		foreach($programas as $prg) {
			$dadosprg[] = array('codigo' => $prg['proid'],'descricao' => $prg['prodesc']);
			echo "resp['".$prg['proid']."'] = new Array();";
			echo "resp['".$prg['proid']."']['proorgaoresp']='".$prg['proorgaoresp']."';";
			echo "resp['".$prg['proid']."']['proorgaoresp']='".$prg['proorgaoresp']."';";
			echo "resp['".$prg['proid']."']['prorepassarec']='".$prg['prorepassarec']."';";
			echo "resp['".$prg['proid']."']['prodesc']='".$prg['prodesc']."';";
			echo "resp['".$prg['proid']."']['proorgaoresp']='".$prg['proorgaoresp']."';";
			echo "resp['".$prg['proid']."']['prosite']='".$prg['prosite']."';";
			//echo "resp['".$prg['proid']."']['proobjetivo']='".$prg['proobjetivo']."';";
			echo "resp['".$prg['proid']."']['proobjetivo']='';";
		}
	}
	
	echo "
		function validarPrograma(){
			if(document.getElementById('proid').value=='') {
				alert('Selecione um programa');
				return false;
			}
			
			if(document.getElementById('spoorgao_S')) {
				if(jQuery(\"[name^='spoorgao']:checked\").length == 0) {
					alert('Clique no Orgão responsável');
					return false;
				}
			}
			
			if(jQuery(\"[name^='sposituacao']:checked\").length == 0) {
				alert('Selecione a situação');
				return false;
			}
			
			if(document.getElementById('sposite').value!='') {
				if (!isUrl(document.getElementById('sposite').value)) {
				 	alert('URL com formato errado. Formato correto: http://www.google.com');
				 	return false;
				} 
			}
			
			if(document.getElementById('proid').value=='1') {
				if(document.getElementById('spodesc').value==''){
					alert('Nome do programa é obrigatório');
					return false;
				}						
			}
			divCarregando();
			document.getElementById('form_programa').submit();
		 }
		  function selecionarPrograma(proid){
		    if(!proid){
		    	return false;
		    }
		  	document.getElementById('td_spoorgao').value='';
		  	document.getElementById('sposite').value='';
		  	document.getElementById('spoobjepro').value='';
			if(proid == '1') {
			document.getElementById('tr_qual').style.display='';
			document.getElementById('tr_spoorgao').style.display='';
			document.getElementById('td_spoorgao').innerHTML='<input type=radio name=spoorgao value=S> Secretaria de Educação <input type=radio name=spoorgao value=E> Entidade Externa';
			} else if(proid == '') {
			document.getElementById('tr_qual').style.display='none';
			document.getElementById('tr_spoorgao').style.display='none';
			document.getElementById('td_spoorgao').innerHTML='';
			} else {
			document.getElementById('tr_qual').style.display='none';
			document.getElementById('tr_spoorgao').style.display='';
			document.getElementById('td_spoorgao').innerHTML=resp[proid]['proorgaoresp'];
			document.getElementById('sposite').value=resp[proid]['prosite'];
			document.getElementById('spoobjepro').value=resp[proid]['proobjetivo'];
			}
		  }
";
	
	echo "</script>";
	
	echo "<form method=post id=form_programa>";
	echo "<input type=hidden name=sprmodulo value=".$dados['sprmodulo'].">";
	
	if($dados['spoid']) {
		echo "<input type=hidden name=requisicao value=atualizarPrograma>";
		echo "<input type=hidden name=spoid value=".$dados['spoid'].">";
		$dadosprograma = $db->pegaLinha("SELECT * FROM pdeinterativo2013.sinteseprograma p
										 LEFT JOIN pdeinterativo2013.programa po ON po.proid = p.proid 
										 WHERE p.spoid='".$dados['spoid']."'");
		if($dadosprograma) extract($dadosprograma);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirPrograma>";
	}
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir programa</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Nome do programa:</td>";
	
	
	echo "<td>".$db->monta_combo('proid', $dadosprg, 'S', 'Selecione', 'selecionarPrograma', '', '', '200', 'S', 'proid', true, $proid)."</td>";
	echo "</tr>";
	echo "<tr id=tr_qual ".(($prodesc=="Outro programa")?"":"style=display:none;").">";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Qual?</td>";
	echo "<td>".campo_texto('spodesc', "S", "S", "Nome do projeto", 46, 150, "", "", '', '', 0,'id="spodesc"','',$spodesc)."</td>";
	echo "</tr>";
	
	if($prodesc=='Outro programa')$orgao_td = "<input type=radio id=spoorgao_S name=spoorgao value=S ".(($spoorgao=="S")?"checked":"")."> Secretaria de Educação <input type=radio name=spoorgao value=E ".(($spoorgao=="E")?"checked":"")."> Entidade Externa";
	elseif($proid=='')$orgao_td = "";
	else $orgao_td = $proorgaoresp;
	
	echo "<tr id=tr_spoorgao ".(($dados['spoid'])?"":"style=display:none;").">";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Orgão responsável:</td>";
	echo "<td id=td_spoorgao>".$orgao_td."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Situação:</td>";
	echo "<td><input type=radio name=sposituacao value=P ".(($sposituacao=="P")?"checked":"").">Já participa <input type=radio name=sposituacao value=G ".(($sposituacao=="G")?"checked":"").">Gostaria de participar</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Site do programa:</td>";
	echo "<td>".campo_texto('sposite', "N", "S", "Site do programa", 46, 150, "", "", '', '', 0,'id="sposite"','',$sposite)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Objetivo do programa:</td>";
	echo "<td>".campo_textarea( 'spoobjepro', 'N', 'S', '', '50', '4', '500','','','','','',$spoobjepro)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=gravar value=Gravar onclick=validarPrograma();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function gravarRespostaProgramasProjetos($dados) {
	global $db;
	
	if($dados['tipo']=="G") {
		
		$sql = "UPDATE pdeinterativo2013.sinteseprograma
   				SET spostatus='I'
 				WHERE sprmodulo='".$dados['smodulo']."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."';";
		$db->executar($sql);
		
	} elseif($dados['tipo']=="J") {
		
		$sql = "UPDATE pdeinterativo2013.sinteseprojeto
   				SET sprstatus='I'
 				WHERE sprmodulo='".$dados['smodulo']."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."';";
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	$rppid = $db->pegaUm("SELECT rppid FROM pdeinterativo2013.respostaprojetoprograma WHERE rppmodulo='".$dados['smodulo']."' AND rpptipo='".$dados['tipo']."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
	
	if($rppid) {
		
		$sql = "UPDATE pdeinterativo2013.respostaprojetoprograma
   				SET rppresposta=".$dados['resposta']." 
 				WHERE rppid='".$rppid."';";
		
	} else {
		
		$sql = "INSERT INTO pdeinterativo2013.respostaprojetoprograma(
	            rppresposta, rppmodulo, pdeid, rpptipo, rppstatus)
	    		VALUES (".$dados['resposta'].", '".$dados['smodulo']."', '".$_SESSION['pdeinterativo2013_vars']['pdeid']."', '".$dados['tipo']."', 'A');";
		
    }
    
    $db->executar($sql);
    $db->commit();

}


function gerenciarProjetos($dados) {
	global $db;
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script>function validarProjeto(){
					if(document.getElementById('sprdesc').value==''){
						alert('Nome do projeto é obrigatório');
						return false;
					}
					if(document.getElementById('sprobjetivo').value==''){
						alert('Objetivo é obrigatório');
						return false;
					}
					divCarregando();
					document.getElementById('form_projeto').submit();}</script>";
	
	echo "<form method=post id=form_projeto>";
	echo "<input type=hidden name=sprmodulo value=".$dados['sprmodulo'].">";
	
	if($dados['sprid']) {
		echo "<input type=hidden name=requisicao value=atualizarProjeto>";
		echo "<input type=hidden name=sprid value=".$dados['sprid'].">";
		$dadosprojeto = $db->pegaLinha("SELECT * FROM pdeinterativo2013.sinteseprojeto WHERE sprid='".$dados['sprid']."'");
		extract($dadosprojeto);
	} else {
		echo "<input type=hidden name=requisicao value=inserirProjeto>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir projeto</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Nome do projeto:</td>";
	echo "<td>".campo_texto('sprdesc', "S", "S", "Nome do projeto", 46, 150, "", "", '', '', 0,'id="sprdesc"','',$sprdesc)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Objetivo do projeto:</td>";
	echo "<td>".campo_textarea( 'sprobjetivo', 'S', 'S', '', '50', '4', '500','','','','','',$sprobjetivo)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=gravar value=Gravar onclick=validarProjeto();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function carregarProjetos($dados) {
	global $db;
	
	echo "<p>Clique no botão ao lado para inserir informações sobre o(s) projeto(s).</p>";
	
	$sql = "SELECT '<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"gerenciarProjetos(\'".$dados['sprmodulo']."\', \'' || sprid || '\');\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirProjeto(\'".$dados['sprmodulo']."\', \'' || sprid || '\');\"></center>' as acoes, sprdesc, sprobjetivo 
			FROM pdeinterativo2013.sinteseprojeto 
			WHERE sprmodulo='".$dados['sprmodulo']."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND sprstatus='A'";
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;","Nome do projeto","Objetivo");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
	echo "<p align=\"right\"><input type=\"button\" value=\"Incluir Projeto\" onclick=\"gerenciarProjetos('".$dados['sprmodulo']."','');\"></p>";
	verificaPermissao(PDEESC_PERFIL_DIRETOR);
		
}

function carregarProgramas($dados) {
	global $db;
	
	echo "<p>Clique no botão ao lado para inserir informações sobre o(s) programa(s).</p>";
	
	$sql = "SELECT '<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"gerenciarProgramas(\'".$dados['sprmodulo']."\', \'' || spoid || '\');\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirPrograma(\'".$dados['sprmodulo']."\', \'' || spoid || '\');\"></center>' as acoes, 
					   CASE WHEN p.proid='1' THEN p.spodesc ELSE po.prodesc END, 
				   spoobjepro,
				   CASE WHEN sposituacao='P' THEN 'Já participa'
						WHEN sposituacao='G' THEN 'Gostaria de participar' END as situacao,
				   CASE WHEN p.proid='1' THEN
					   CASE WHEN spoorgao='S' THEN 'Secretaria de Educação'
					   		WHEN spoorgao='E' THEN 'Entidade Externa' END
				   ELSE po.proorgaoresp END as orgao,
				   sposite
	FROM pdeinterativo2013.sinteseprograma p 
	LEFT JOIN pdeinterativo2013.programa po ON po.proid = p.proid 
	WHERE p.sprmodulo='".$dados['sprmodulo']."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND spostatus='A'";
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;","Nome do programa","Objetivo do programa","Situação","Orgão","Site");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
	echo "<p align=\"right\"><input type=\"button\" value=\"Incluir Programa\" onclick=\"gerenciarProgramas('".$dados['sprmodulo']."','');\"></p>";
	verificaPermissao(PDEESC_PERFIL_DIRETOR);
}

function montaGraficoTaxasRendimento($dados) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$arrTipo = array("A" => "Aprovação",
				     "R" => "Reprovação",
					 "B" => "Abandono");
		
	
	$anosTX = array("2008","2009","2010","2011"); 
	
	$colorGraph = array("2008" => "#cc1111",
						"2009" => "#11cccc",
					    "2010" => "#1D79F9",
						"2011" => "#1111cc");
	
	$arrEns = array("I" => "Anos iniciais do Ensino Fundamental",
				    "F" => "Anos finais do Ensino Fundamental",
					"M" => "Ensino Médio",
					"U" => "Ensino Fundamental");
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND mu.estuf <> 'DF' AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	
	foreach($arrEsf as $codesf => $esfera) {
		
		$dadosescensino = $db->carregar("SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
										 LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intaprrepaba='".$dados['tipo']."' AND it.intensino='".$dados['ensino']."' AND it.intsubmodulo='T'");

		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intesfera']][$dee['intano']] = $dee['intvalor'];
			}
		}
		
	}
	
	
	foreach($arrEsf as $codesfera => $esfera) {
		foreach($anosTX as $anotx) {
			$dadosvalores[$anotx][] = $arrEscEns[$codesfera][$anotx];				
		}
		$_x_ax[]  = $esfera;
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	
	// Setup the graph.
	$graph = new Graph(400,120);
	$graph->img->SetMargin(30,80,45,25);
	$graph->SetScale("textlin");
	
	// Set up the title for the graph
	$graph->title->Set("Gráfico - Taxa de ".$arrTipo[$dados['tipo']]." do ".$arrEns[$dados['ensino']]." (em %)");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,12);
	$graph->title->SetColor("black");
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL,7);
	$graph->xaxis->SetTickLabels($_x_ax);
	$graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
	$graph->legend->SetLineSpacing(5);
	$graph->legend->Pos(0.02,0.3);	
	// Create the bar plots
	foreach($anosTX as $anotx) {
		$bp = new BarPlot($dadosvalores[$anotx]);
		$bp->SetColor("white");
		$bp->SetFillColor($colorGraph[$anotx]);
		$bp->SetLegend($anotx);
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


/**
 * Função que gera t no padrão em barras para 1.1.IDEB
 * 
 * @author Alexandre Dourado
 * @return jpgraph
 * @param $dados => array contendo todas as informações referentes ao questionpario da Prova Brasil
 * @global $db classe que instância o banco de dados 
 * @version v1.0 18/07/2011
 */
function montaTabelaTaxasRendimento($ensino, $tipo) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$anosTX = array("2008","2009","2010","2011"); 
 
	$arrTipo = array("A" => "Aprovação",
				     "R" => "Reprovação",
					 "B" => "Abandono");
	
	$arrEns = array("U" => "Ensino Fundamental",
					"M" => "Ensino Médio");
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola (".$db->pegaUm("SELECT pdenome FROM pdeinterativo2013.pdinterativo WHERe pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'").")");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND mu.estuf <> 'DF' AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	
	foreach($arrEsf as $codesf => $esfera) {
		$sql = "SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
									     LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL 
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intaprrepaba='".$tipo."' AND it.intensino='".$ensino."' AND it.intsubmodulo='T'";
		
		$dadosescensino = $db->carregar($sql);

		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if(!is_null($dee['intvalor'])) $arrEscEns[$dee['intesfera']][$dee['intano']] = $dee['intvalor'];
			}
		}
		
	}

	/* CONSTANTES E ARRAY DE DADOS */
	
	$html .= "<table class=listagem width=100%>";
	
	$html .= "<tr>
				<td class=SubTituloCentro colspan=6>Taxa de ".$arrTipo[$tipo]." do ".$arrEns[$ensino]." (em %)</td>
			  </tr>";
	
	$html .= "<tr>
				<td class=SubTituloCentro>Esfera</td>";
	
	foreach($anosTX as $anotx) {
		$html .= "<td class=SubTituloCentro>".$anotx."</td>";
	}
	
	$html .= "</tr>";
	
	foreach($arrEsf as $codesfera => $esfera) {
		$html .= "<tr>";
		$html .= "<td ".($codesfera == "S" ? "style=\"color:#006400;font-weight:bold\"" : "")." >".$esfera."</td>";
		
		foreach($anosTX as $anotx) {
			if($codesfera == "S") $valores_escola[] = $arrEscEns[$codesfera][$anotx];
			$html .= "<td ".($codesfera == "S" ? "style=\"color:#006400;font-weight:bold\"" : "")." align=center>".((!is_null($arrEscEns[$codesfera][$anotx]))?$arrEscEns[$codesfera][$anotx]:"-")."</td>";
		}
		
		$html .= "</tr>";
		
	}
	
	$html .= "</table>";
	
	$regs=0;
	if($valores_escola)
		foreach($valores_escola as $v)
			if($v)$regs++;
	
	if($tipo == "A") {
			// 	verificando se os valores do IDEB estão melhorando ou piorando (aplicação da regra)
		if($regs < 2) {
			$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Aprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";			
			$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Aprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
		} elseif(($valores_escola[(count($valores_escola)-1)] - $valores_escola[(count($valores_escola)-2)]) > 0) {
			$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Aprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Taxa de Aprovação. Por favor, reveja a sua análise.');this.checked=false;\"";
		} else {
			$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Aprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Taxa de Aprovação. Por favor, reveja a sua análise.');this.checked=false;\"";
		}
	
	} elseif($tipo == "R") {
		// verificando se os valores do IDEB estão melhorando ou piorando (aplicação da regra)
		if($regs < 2) {
			$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Reprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";			
			$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Reprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
		} elseif(($valores_escola[(count($valores_escola)-2)] - $valores_escola[(count($valores_escola)-1)]) > 0) {
			$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Reprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Taxa de Reprovação. Por favor, reveja a sua análise.');this.checked=false;\"";
		} else {
			$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Reprovação diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Taxa de Reprovação. Por favor, reveja a sua análise.');this.checked=false;\"";
		}
		
	} else {
		// verificando se os valores do IDEB estão melhorando ou piorando (aplicação da regra)
		if($regs < 2) {
			$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Abandono diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";			
			$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Abandono diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
		} elseif(($valores_escola[(count($valores_escola)-2)] - $valores_escola[(count($valores_escola)-1)]) > 0) {
			$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Abandono diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Taxa de Abandono. Por favor, reveja a sua análise.');this.checked=false;\"";
		} else {
			$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Taxa de Abandono diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Taxa de Abandono. Por favor, reveja a sua análise.');this.checked=false;\"";
		}
	}
	
	return array('html' => $html,'onclickperg' => $onclickperg);
	
}



function montaGraficoProvaBrasil($dados) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$arrMat = array("M" => "Matemática",
				    "P" => "Língua Portuguesa");
	
	
	$anosPB = array("2005","2007","2009","2011"); 
	
	$colorGraph = array("2005" => "#cc1111",
						"2007" => "#11cccc",
					    "2009" => "#1D79F9",
						"2011" => "#1111cc");
	
	
	$arrEns = array("I" => "Anos iniciais do Ensino Fundamental",
				    "F" => "Anos finais do Ensino Fundamental",
					"M" => "Ensino Médio");
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND mu.estuf <> 'DF' AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	
	foreach($arrEsf as $codesf => $esfera) {
		
		$dadosescensino = $db->carregar("SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
										 LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intpormat='".$dados['materia']."' AND it.intensino='".$dados['ensino']."' AND it.intsubmodulo='P'");		
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intesfera']][$dee['intano']] = $dee['intvalor'];
			}
		}
		
	}
	
	foreach($arrEsf as $codesfera => $esfera) {
		foreach($anosPB as $anopb) {
			$dadosvalores[$anopb][] = $arrEscEns[$codesfera][$anopb];				
		}
		$_x_ax[]  = $esfera;
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	
	// Setup the graph.
	$graph = new Graph(400,120);
	$graph->img->SetMargin(30,80,10,25);
	$graph->SetScale("textlin",0,400);
	
	// Set up the title for the graph
	$graph->title->Set($arrEns[$dados['ensino']]."(".$arrMat[$dados['materia']].")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,12);
	$graph->title->SetColor("black");
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL,7);
	$graph->xaxis->SetTickLabels($_x_ax);
	$graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
	$graph->legend->SetLineSpacing(5);
	$graph->legend->Pos(0.02,0.3);	
	// Create the bar plots
	foreach($anosPB as $anopb) {
		$bp = new BarPlot($dadosvalores[$anopb]);
		$bp->SetColor("white");
		$bp->SetFillColor($colorGraph[$anopb]);
		$bp->SetLegend($anopb);
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


/**
 * Função que gera tabela html para 1.3.Prova Brasil
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param $ensino => tipo de ensino que pode ser Enfino Fundamental, Ensino Fundamental Anos Iniciais, Ensino Fundamental Anos Finais e Ensino Médio
 * @param $submodulo => os submodulos do sistema, podendo ser IDEB, Taxas de rendimento, Prova Brasil e distorções e aproveitamento 
 * @global $db classe que instância o banco de dados 
 * @version v1.0 18/07/2011
 */

function montaTabelaProvaBrasil($ensino, $materia) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$anosPB = array("2005","2007","2009","2011"); 
	
	$arrMat = array("M" => "Matemática",
				    "P" => "Língua Portuguesa");
	
	$arrEns = array("I" => "Anos iniciais do Ensino Fundamental",
				    "F" => "Anos finais do Ensino Fundamental",
					"M" => "Ensino Médio");
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola (".$db->pegaUm("SELECT pdenome FROM pdeinterativo2013.pdinterativo WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'").")");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND mu.estuf <> 'DF' AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	foreach($arrEsf as $codesf => $esfera) {
		
		$dadosescensino = $db->carregar("SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
										 LEFT JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' 
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL 
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intpormat='".$materia."' AND it.intensino='".$ensino."' AND it.intsubmodulo='P'");		
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intesfera']][$dee['intano']] = $dee['intvalor'];
			}
		}
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$html .= "<table class=listagem width=100%>";
	
	$html .= "<tr>
				<td class=SubTituloCentro colspan=5>".$arrEns[$ensino]."</td>
			  </tr>";
	
	$html .= "<tr>
				<td class=SubTituloCentro rowspan=2>Esfera</td>
				<td class=SubTituloCentro colspan=".count($anosPB).">".$arrMat[$materia]."</td>
			  </tr>";
	
	$html .= "<tr>";
	foreach($anosPB as $anopb) {
		$html .= "<td class=SubTituloCentro>".$anopb."</td>";
	}
	
	$html .= "</tr>";
	
	foreach($arrEsf as $codesfera => $esfera) {
		$html .= "<tr>";
		$html .= "<td ".($codesfera == "S" ? "style=\"color:#006400;font-weight:bold\"" : "")." >".$esfera."</td>";
		
		foreach($anosPB as $anopb) {
			if($codesfera == "S") $valores_escola[] = $arrEscEns[$codesfera][$anopb];
			$html .= "<td ".($codesfera == "S" ? "style=\"color:#006400;font-weight:bold\"" : "")." align=center>".(($arrEscEns[$codesfera][$anopb])?$arrEscEns[$codesfera][$anopb]:"-")."</td>";
		}
		
		$html .= "</tr>";
		
	}
	
	$html .= "</table>";
	
	$regs=0;
	if($valores_escola)
		foreach($valores_escola as $v)
			if($v)$regs++;

	// verificando se os valores do IDEB estão melhorando ou piorando (aplicação da regra)	
	if($regs<2) {
		$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Prova Brasil diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
		$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Prova Brasil diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
	} elseif(($valores_escola[(count($valores_escola)-1)] - $valores_escola[(count($valores_escola)-2)]) > 0) {
		$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução da Prova Brasil diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
		$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Prova Brasil. Por favor, reveja a sua análise.');this.checked=false;\"";
	} else {
		$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução da Prova Brasil diverge da resposta apresentada. Por favor, reveja a sua análise.');this.checked=false;\"";
		$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos à Prova Brasil. Por favor, reveja a sua análise.');this.checked=false;\"";
	}
	
	return array('html' => $html,'onclickperg' => $onclickperg);
	
}

/**
 * Função que gera gráfico no padrão em barras para 1.1.IDEB
 * 
 * @author Alexandre Dourado
 * @return jpgraph
 * @param $dados => array contendo todas as informações referentes ao questionpario da Prova Brasil
 * @global $db classe que instância o banco de dados 
 * @version v1.0 18/07/2011
 */



function montaGraficoIDEB($dados) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$anosIDEB = array("2005","2007","2009","2011"); 
	$anosIDEBMeta = array("2013","2015");
	
	$colorGraph = array("2005" => "#cc1111",
						"2007" => "#11cccc",
					    "2009" => "#1D79F9",
						"2011" => "#1111cc",
						"2013" => "#007700",
						"2015" => "#006400");
	
	$arrEns = array("I" => "Anos iniciais do Ensino Fundamental",
				    "F" => "Anos finais do Ensino Fundamental",
					"M" => "Ensino Médio");
	
	$arrEsf = array("B" => "Brasil",
					"E" => "Estado",
					"M" => "Município",
					"S" => "Escola");
	
	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND mu.estuf <> 'DF' AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");
	
	foreach($arrEsf as $codesf => $esfera) {
		
		$dadosescensino = $db->carregar("SELECT it.* FROM pdeinterativo2013.indicadorestaxas it
										 left JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) AND es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL 
										 left JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intensino='".$dados['ensino']."' AND it.intsubmodulo='I'");
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intesfera']][$dee['intano']] = $dee['intvalor'];
				if($dee['intvlrmeta']) $arrEscEnsMeta[$dee['intesfera']][$dee['intano']] = $dee['intvlrmeta'];
			}
		}
	}
	
	foreach($arrEsf as $codesfera => $esfera) {
		foreach($anosIDEB as $anoideb) {
			$dadosvalores[$anoideb][] = $arrEscEns[$codesfera][$anoideb];				
		}
		foreach($anosIDEBMeta as $anoidebmeta) {
			$dadosvalores[$anoidebmeta][] = $arrEscEnsMeta[$codesfera][$anoidebmeta];				
		}
		$_x_ax[]  = $esfera;
	}
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	
	// Setup the graph.
	$graph = new Graph(400,120);
	$graph->img->SetMargin(20,80,10,25);
	$graph->SetScale("textlin");
	
	// Set up the title for the graph
	$graph->title->Set($arrEns[$dados['ensino']]);
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,12);
	$graph->title->SetColor("black");
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL,7);
	$graph->xaxis->SetTickLabels($_x_ax);
	$graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
	$graph->legend->SetLineSpacing(5);
	$graph->legend->Pos(0.02,0.1);	
	// Create the bar plots
	foreach($anosIDEB as $anoideb) {
		$bp = new BarPlot($dadosvalores[$anoideb]);
		$bp->SetColor("white");
		$bp->SetFillColor($colorGraph[$anoideb]);
		$bp->SetLegend($anoideb);
		$bp->value->Show();
		$bp->value->SetAngle(90); 
		$bp->value->SetFont(FF_FONT0,FS_NORMAL,7);
		
		$arrPlots[] = $bp; 
	}
	
	// Create the bar plots
	foreach($anosIDEBMeta as $anoidebmeta) {
		$bp = new BarPlot($dadosvalores[$anoidebmeta]);
		$bp->SetColor("white");
		$bp->SetFillColor($colorGraph[$anoidebmeta]);
		$bp->SetLegend($anoidebmeta);
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

/**
 * Função que gerencia a gravação dos dados referente a tela 1.3.Prova Brasil
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param $dados => array contendo todas as informações referentes ao questionpario da Prova Brasil
 * @global $db classe que instância o banco de dados 
 * @version v1.0 18/07/2011
 */
function diagnostico_1_3_provabrasil($dados) {
	global $db;
	$sql = "SELECT rpbid FROM pdeinterativo2013.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rbdstatus='A'";
	$rpbid = $db->pegaUm($sql);
	
	if($rpbid) {
		
		$sql = "UPDATE pdeinterativo2013.respostaprovabrasil
				   SET rpbinicialport=".(($dados['rpbinicialport'])?"'".$dados['rpbinicialport']."'":"NULL").",  
				       rpbinicialmat=".(($dados['rpbinicialmat'])?"'".$dados['rpbinicialmat']."'":"NULL").", 
				       rpbfinalport=".(($dados['rpbfinalport'])?"'".$dados['rpbfinalport']."'":"NULL").",  
				       rpbfinalmat=".(($dados['rpbfinalmat'])?"'".$dados['rpbfinalmat']."'":"NULL").", 
				       rbpmedioport=".(($dados['rbpmedioport'])?"'".$dados['rbpmedioport']."'":"NULL").",  
				       rbpmediomat=".(($dados['rbpmediomat'])?"'".$dados['rbpmediomat']."'":"NULL")." 
				 WHERE rpbid='".$rpbid."';";

		$db->executar($sql);
	} else {
		$sql = "INSERT INTO pdeinterativo2013.respostaprovabrasil(
			            pdeid, 
			            rpbinicialport, 
			            rpbinicialmat, 
			            rpbfinalport, 
			            rpbfinalmat, 
			            rbpmedioport, 
			            rbpmediomat, 
			            rbdstatus)
			    VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."', 
			    		".(($dados['rpbinicialport'])?"'".$dados['rpbinicialport']."'":"NULL").", 
			    		".(($dados['rpbinicialmat'])?"'".$dados['rpbinicialmat']."'":"NULL").", 
			    		".(($dados['rpbfinalport'])?"'".$dados['rpbfinalport']."'":"NULL").", 
			            ".(($dados['rpbfinalmat'])?"'".$dados['rpbfinalmat']."'":"NULL").", 
			            ".(($dados['rbpmedioport'])?"'".$dados['rbpmedioport']."'":"NULL").", 
			            ".(($dados['rbpmediomat'])?"'".$dados['rbpmediomat']."'":"NULL").", 
			            'A');";
		
		$db->executar($sql);
	}
	
	$db->commit();
	
	salvarAbaResposta("diagnostico_1_3_provabrasil");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
}


/**
 * Função que gerencia a gravação dos dados referente a tela 1.1.IDEB
 * 
 * @author Alexandre Dourado
 * @return javascriptcode
 * @param $dados => array contendo todas as informações referentes ao questionpario do IDEB
 * @global $db classe que instância o banco de dados 
 * @version v1.0 18/07/2011
 */
function diagnostico_1_1_ideb($dados) {
	global $db;
	
	$sql = "SELECT ridid FROM pdeinterativo2013.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND ridsstatus='A'";
	$ridid = $db->pegaUm($sql);
	
	if($ridid) {
		$sql = "UPDATE pdeinterativo2013.respostaideb
   				SET ridinicialum=".(($dados['ridinicialum'])?"'".$dados['ridinicialum']."'":"NULL").", 
   					ridinicialdois=".(($dados['ridinicialdois'])?"'".$dados['ridinicialdois']."'":"NULL").", 
   					riddinicialdesc=".(($dados['riddinicialdesc'])?"'".$dados['riddinicialdesc']."'":"NULL").", 
       			    ridfinalum=".(($dados['ridfinalum'])?"'".$dados['ridfinalum']."'":"NULL").", 
       			    ridfinaldois=".(($dados['ridfinaldois'])?"'".$dados['ridfinaldois']."'":"NULL").", 
       			    ridfinaldesc=".(($dados['ridfinaldesc'])?"'".$dados['ridfinaldesc']."'":"NULL").", 
       			    ridmedioum=".(($dados['ridmedioum'])?"'".$dados['ridmedioum']."'":"NULL").", 
       			    ridmediodois=".(($dados['ridmediodois'])?"'".$dados['ridmediodois']."'":"NULL").", 
       			    ridmediodesc=".(($dados['ridmediodesc'])?"'".$dados['ridmediodesc']."'":"NULL")."  
 				WHERE ridid='".$ridid."';";
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO pdeinterativo2013.respostaideb(
	            ridinicialum, 
	            ridinicialdois, 
	            riddinicialdesc, 
	            ridfinalum, 
	            ridfinaldois, 
	            ridfinaldesc, ridmedioum, ridmediodois, ridmediodesc, 
	            ridsstatus, pdeid)
			    VALUES (".(($dados['ridinicialum'])?"'".$dados['ridinicialum']."'":"NULL").", 
			    		".(($dados['ridinicialdois'])?"'".$dados['ridinicialdois']."'":"NULL").", 
			    		".(($dados['riddinicialdesc'])?"'".$dados['riddinicialdesc']."'":"NULL").", 
			    		".(($dados['ridfinalum'])?"'".$dados['ridfinalum']."'":"NULL").", 
			    		".(($dados['ridfinaldois'])?"'".$dados['ridfinaldois']."'":"NULL").",
			    		".(($dados['ridfinaldesc'])?"'".$dados['ridfinaldesc']."'":"NULL").",
			    		".(($dados['ridmedioum'])?"'".$dados['ridmedioum']."'":"NULL").",
			    		".(($dados['ridmediodois'])?"'".$dados['ridmediodois']."'":"NULL").",
			    		".(($dados['ridmediodesc'])?"'".$dados['ridmediodesc']."'":"NULL").",
	            		'A',
	            		'".$_SESSION['pdeinterativo2013_vars']['pdeid']."');";
		
		$db->executar($sql);
		
	}
	$db->commit();
	
	salvarAbaResposta("diagnostico_1_1_ideb");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
}

function diagnostico_1_2_taxasderendimento($dados) {
	global $db;
	
	$sql = "SELECT rtrid FROM pdeinterativo2013.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rtrstatus='A'";
	$rtrid = $db->pegaUm($sql);
	
	if($rtrid) {
		
		$sql = "UPDATE pdeinterativo2013.respostataxarendimento
   				SET rtrfunaprova=".(($dados['rtrfunaprova'])?"'".$dados['rtrfunaprova']."'":"NULL").", 
				    rtrfunreprova=".(($dados['rtrfunreprova'])?"'".$dados['rtrfunreprova']."'":"NULL").", 
				    rtrfunabandono=".(($dados['rtrfunabandono'])?"'".$dados['rtrfunabandono']."'":"NULL").", 
				    rtrmedaprova=".(($dados['rtrmedaprova'])?"'".$dados['rtrmedaprova']."'":"NULL").", 
				    rtrmedreprova=".(($dados['rtrmedreprova'])?"'".$dados['rtrmedreprova']."'":"NULL").", 
				    rtrmedabandono=".(($dados['rtrmedabandono'])?"'".$dados['rtrmedabandono']."'":"NULL")." 
				 WHERE rtrid='".$rtrid."';";
		
		$db->executar($sql);
		
	} else {
		$sql = "INSERT INTO pdeinterativo2013.respostataxarendimento(
			            pdeid, 
			            rtrfunaprova, 
			            rtrfunreprova, 
			            rtrfunabandono, 
			            rtrmedaprova, 
			            rtrmedreprova, 
			            rtrmedabandono, 
			            rtrstatus)
			    VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."',
			    		".(($dados['rtrfunaprova'])?"'".$dados['rtrfunaprova']."'":"NULL").", 
			    		".(($dados['rtrfunreprova'])?"'".$dados['rtrfunreprova']."'":"NULL").", 
			    		".(($dados['rtrfunabandono'])?"'".$dados['rtrfunabandono']."'":"NULL").", 
			    		".(($dados['rtrmedaprova'])?"'".$dados['rtrmedaprova']."'":"NULL").", 
			            ".(($dados['rtrmedreprova'])?"'".$dados['rtrmedreprova']."'":"NULL").", 
			            ".(($dados['rtrmedabandono'])?"'".$dados['rtrmedabandono']."'":"NULL").", 
			            'A');";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	salvarAbaResposta("diagnostico_1_2_taxasderendimento");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
}

/**
 * Função que verifcia se em determinado submodulo e tipo de ensino, existem informações da escola
 * 
 * @author Alexandre Dourado
 * @return boolean
 * @param $ensino => tipo de ensino que pode ser Enfino Fundamental, Ensino Fundamental Anos Iniciais, Ensino Fundamental Anos Finais e Ensino Médio
 * @param $submodulo => os submodulos do sistema, podendo ser IDEB, Taxas de rendimento, Prova Brasil e distorções e aproveitamento 
 * @global $db classe que instância o banco de dados 
 * @version v1.0 18/07/2011
 */
function possuiEnsino($ensino, $submodulo) {
	global $db;
	$existe = $db->pegaUm("SELECT intid FROM pdeinterativo2013.indicadorestaxas WHERE intinep='".$_SESSION['pdeinterativo2013_vars']['pdicodinep']."' AND intensino='".$ensino."' AND intsubmodulo='".$submodulo."' LIMIT 1");
	return (($existe)?TRUE:FALSE);
}

/**
 * Função que gera tabela html para 1.3.IDEB
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param $ensino => tipo de ensino que pode ser Enfino Fundamental, Ensino Fundamental Anos Iniciais, Ensino Fundamental Anos Finais e Ensino Médio
 * @global $db classe que instância o banco de dados 
 * @version v1.0 18/07/2011
 */
function montaTabelaIDEB($ensino) {
	global $db;
	
	/* CONSTANTES E ARRAY DE DADOS */
	$anosIDEB = array("2005","2007","2009","2011"); 
	$anosIDEBMeta = array("2013","2015");
	
	$arrEns = array("I" => "Anos iniciais do Ensino Fundamental",
				    "F" => "Anos finais do Ensino Fundamental",
					"M" => "Ensino Médio");
	
	$arrEsf = array("B" => "IDEB Brasil",
					"E" => "IDEB Estado",
					"M" => "IDEB Município",
					"S" => "IDEB Escola (".$db->pegaUm("SELECT pdenome FROM pdeinterativo2013.pdinterativo WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'").")");

	$filtros_por_esfera = array("B" => "",
								"E" => "es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL AND",
								"M" => "mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL AND mu.estuf <> 'DF' AND",
								"S" => "it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND");	
	
	foreach($arrEsf as $codesf => $esfera) {
				
		$dadosescensino = $db->carregar("SELECT it.* FROM pdeinterativo2013.indicadorestaxas it 
										 LEFT JOIN pdeinterativo2013.pdinterativo es ON es.estuf = it.intcoduf::character(2) AND es.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcoduf IS NOT NULL 
 										 left JOIN pdeinterativo2013.pdinterativo it2 ON it2.pdicodinep::numeric = it.intinep and it2.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'
 										 LEFT JOIN pdeinterativo2013.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND it.intcodmun IS NOT NULL
										 WHERE ".$filtros_por_esfera[$codesf]." it.intesfera='".$codesf."' AND it.intensino='".$ensino."' AND it.intsubmodulo='I'");		
		if($dadosescensino[0]) {
			foreach($dadosescensino as $dee) {
				if($dee['intvalor']) $arrEscEns[$dee['intesfera']][$dee['intano']] = $dee['intvalor'];
				if($dee['intvlrmeta']) $arrEscEnsMeta[$dee['intesfera']][$dee['intano']] = $dee['intvlrmeta'];
			}
		}
	}
	//ver($sql,d);
	
	/* CONSTANTES E ARRAY DE DADOS */
	
	$html .= "<table class=listagem width=100%>";
	
	$html .= "<tr>
				<td class=SubTituloCentro colspan=6>".$arrEns[$ensino]."</td>
			  </tr>";
	
	$html .= "<tr>
				<td class=SubTituloCentro rowspan=2>Esfera</td>
				<td class=SubTituloCentro width='60' colspan=".count($anosIDEB).">IDEB Observado</td>
				<td class=SubTituloCentro style=\"color:#006400\" width='60' colspan=".count($anosIDEBMeta).">Meta</td>
			  </tr>";
	
	$html .= "<tr>";
	foreach($anosIDEB as $anoideb) {
		$html .= "<td class=SubTituloCentro>".$anoideb."</td>";
	}
	
	foreach($anosIDEBMeta as $anoidebmeta) {
		$html .= "<td style=\"color:#006400;font-weight:bold\" class=SubTituloCentro>".$anoidebmeta."</td>";
	}
	$html .= "</tr>";
	
	foreach($arrEsf as $codesfera => $esfera) {
		$html .= "<tr>";
		$html .= "<td ".($codesfera == "S" ? "style=\"color:#006400;font-weight:bold\" " : "")." >".$esfera."</td>";
		
		foreach($anosIDEB as $key => $anoideb) {
			if($codesfera == "S") $valores_escola[] = $arrEscEns[$codesfera][$anoideb];
				$html .= "<td ".($codesfera == "S" ? "style=\"color:#006400;font-weight:bold\" " : "")." align=center>".(($arrEscEns[$codesfera][$anoideb])?$arrEscEns[$codesfera][$anoideb]:"-")."</td>";
		}
		
		foreach($anosIDEBMeta as $anoidebmeta) {
			$html .= "<td style=\"color:#006400;font-weight:bold\" align=center>".(($arrEscEnsMeta[$codesfera][$anoidebmeta])?$arrEscEnsMeta[$codesfera][$anoidebmeta]:"-")."</td>";
		}
		
		$html .= "</tr>";
		
	}
	
	$html .= "</table>";
		
	// verificando se os valores do IDEB estão melhorando ou piorando (aplicação da regra)
	if(count($valores_escola) == 0 || count($valores_escola) == 1 || (!$valores_escola[(count($valores_escola)-1)] || !$valores_escola[(count($valores_escola)-2)]) || ( $valores_escola[(count($valores_escola)-2)] == $valores_escola[(count($valores_escola)-1)] ) ){
		$onclickperg['S'] = "onclick=\"alert('Não existem dados do IDEB suficientes para selecionar essa resposta. Por favor, reveja a sua análise!');this.checked=false;\"";
		$onclickperg['N'] = "onclick=\"alert('Não existem dados do IDEB suficientes para selecionar essa resposta. Por favor, reveja a sua análise!');this.checked=false;\"";
	}else{
		if(($valores_escola[(count($valores_escola)-1)] - $valores_escola[(count($valores_escola)-2)]) > 0) {
			$onclickperg['N'] = "onclick=\"alert('Os dados mostram que a evolução do IDEB diverge da resposta apresentada. Por favor, reveja a sua análise!');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos ao IDEB. Por favor, subtraia o valor do último resultado do penúltimo e reveja a sua análise.');this.checked=false;\"";
		} else {
			$onclickperg['S'] = "onclick=\"alert('Os dados mostram que a evolução do IDEB diverge da resposta apresentada. Por favor, reveja a sua análise!');this.checked=false;\"";
			$onclickperg['A'] = "onclick=\"alert('Os dados mostram que há mais de dois resultados relativos ao IDEB. Por favor, subtraia o valor do último resultado do penúltimo e reveja a sua análise.');this.checked=false;\"";
		}
	}
	
	return array('html' => $html,'onclickperg' => $onclickperg, 'duasmedicao' => ((!$valores_escola[(count($valores_escola)-1)] || !$valores_escola[(count($valores_escola)-2)])?FALSE:TRUE));
	
}


?>