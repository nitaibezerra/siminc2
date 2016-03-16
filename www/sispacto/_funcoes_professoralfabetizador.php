<?

function carregarProfessorAlfabetizador($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, 
								  re.reiid, 
								  su.uniuf, 
								  u.curid, 
								  u.docid, 
								  su.unisigla||' - '||su.uninome as descricao
						   FROM sispacto.universidadecadastro u 
					 	   INNER JOIN sispacto.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf, i.iusdesativado 
							   FROM sispacto.identificacaousuario i 
							   INNER JOIN sispacto.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'");
	
	$_SESSION['sispacto']['professoralfabetizador'] = array("descricao" 	=> $arr['descricao']." ( ".$infprof['iusnome']." )",
															"curid" 		=> $arr['curid'], 
															"uncid" 		=> $arr['uncid'], 
															"reiid" 		=> $arr['reiid'], 
															"estuf" 		=> $arr['uniuf'], 
															"docid" 		=> $arr['docid'], 
															"iusd" 	   		=> $infprof['iusd'],
															"iuscpf"    	=> $infprof['iuscpf'],
															"iusdesativado" => $infprof['iusdesativado']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function gerenciarMateriaisProfessores($dados) {
	global $db;
	
	if(!$_SESSION['sispacto']['professoralfabetizador']['iusd']) {
		$al = array("alert"=>"Não foi possível gravar as informações sobre materiais. Tente novamente mais tarde.","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
		alertlocation($al);
	}
	
	$sql = "UPDATE sispacto.materiaisprofessores
	   			SET recebeumaterialpacto='".$dados['recebeumaterialpacto']."',
	       			recebeumaterialpnld='".$dados['recebeumaterialpnld']."',
	       			recebeulivrospnld='".$dados['recebeulivrospnld']."',
	       			recebeumaterialpnbe='".$dados['recebeumaterialpnbe']."',
	       			criadocantinholeitura='".$dados['criadocantinholeitura']."'
	 			WHERE iusd='".$_SESSION['sispacto']['professoralfabetizador']['iusd']."'";
	
	$db->executar($sql);
	
	$sql = "INSERT INTO sispacto.materiaisprofessores(
            	iusd, recebeumaterialpacto,  
            	recebeumaterialpnld, recebeulivrospnld, recebeumaterialpnbe, 
            	criadocantinholeitura, mapstatus)
			 	SELECT '".$_SESSION['sispacto']['professoralfabetizador']['iusd']."', 
			    		'".$dados['recebeumaterialpacto']."', 
			    		'".$dados['recebeumaterialpnld']."', 
			    		'".$dados['recebeulivrospnld']."', 
			    		'".$dados['recebeumaterialpnbe']."', 
			            '".$dados['criadocantinholeitura']."', 'A'
			    WHERE (SELECT mapid FROM sispacto.materiaisprofessores WHERE iusd='".$_SESSION['sispacto']['professoralfabetizador']['iusd']."') IS NULL";
	
	$db->executar($sql);
	
	$db->commit();
			
	$sql = "SELECT mapid as t FROM sispacto.materiaisprofessores WHERE iusd='".$_SESSION['sispacto']['professoralfabetizador']['iusd']."'";
	$mapid = $db->pegaUm($sql);
	
	if($_FILES['arquivo']['error']=='0') {
		$campos	= array("mapid"	 => $mapid,
						"mpfdsc" => "'".$dados['mpfdsc']."'");	
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file = new FilesSimec("materiaisprofessoresfotos", $campos ,"sispacto");
				
		$arquivoSalvo = $file->setUpload($dados['mafdsc']);
	}
	
	
	$al = array("alert"=>"Informações sobre materiais salvas com sucesso","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
	alertlocation($al);
	
}

function desvincularTurmaProfessor($dados) {
	global $db;
	
	$sql = "UPDATE sispacto.turmasprofessoresalfabetizadores SET tpastatus='I', tpajustificativadesvinculacao='".$dados['tpajustificativadesvinculacao']."' WHERE tpaid='".$dados['tpaid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Desvinculação feita com sucesso","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
}


function pegarTurmasProfessores($dados) {
	global $db;
	
	if($dados['iusd']) $wh[] = "t.iusd='".$dados['iusd']."'";
	if($dados['tpaid']) $wh[] = "t.tpaid='".$dados['tpaid']."'";
	if($dados['tpastatus']) $wh[] = "t.tpastatus='".$dados['tpastatus']."'";
	if($dados['tpaconfirmaregencianulo']) $wh[] = "t.tpaconfirmaregencia IS NULL";
	if($dados['tpaconfirmaregencia']) $wh[] = "t.tpaconfirmaregencia=".$dados['tpaconfirmaregencia'];
	
	$turmasprofessores = $db->carregar("SELECT * 
										FROM sispacto.turmasprofessoresalfabetizadores t 
										INNER JOIN territorios.municipio m ON m.muncod = t.tpamuncodescola 
										".(($wh)?"WHERE ".implode(" AND ",$wh):""));
	
	return $turmasprofessores;
}

function carregarEscolasPorMunicipio($dados) {
	global $db;
	
	$sql = "SELECT pk_cod_entidade as codigo, pk_cod_entidade || ' - ' || no_entidade as descricao FROM educacenso_2013.tab_entidade WHERE fk_cod_municipio='".$dados['muncod']."' ORDER BY no_entidade";
	$combo = $db->monta_combo('tpacodigoescola', $sql, 'S', 'Selecione', 'exibirDadosTurma', '', '', '200', 'S', 'tpacodigoescola', '', '');
	
}

function confirmarRegenciaTurma($dados) {
	global $db;
	if($dados['tpaconfirmaregencia']) {
		foreach($dados['tpaconfirmaregencia'] as $tpaid => $vl) {
			$db->executar("UPDATE sispacto.turmasprofessoresalfabetizadores SET tpaconfirmaregencia=".$vl." WHERE tpaid='".$tpaid."'");
			$db->commit();			
		}
	}
	
	$al = array("alert"=>"Confirmação de regência feita com sucesso","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
	
}

function inserirTurmaProfessor($dados) {
	global $db;
	
	if(!$dados['tpahorarioinicioturma_hr']) $inf_faltando[] = "Hora (Inicío) em branco";
	if(!$dados['tpahorarioinicioturma_mi']) $inf_faltando[] = "Minuto (Inicío) em branco";
	if(!$dados['tpahorariofimturma_hr']) $inf_faltando[] = "Hora (Fim) em branco";
	if(!$dados['tpahorariofimturma_mi']) $inf_faltando[] = "Minuto (Fim) em branco";
	if(!$dados['pk_cod_etapa_ensino']) $inf_faltando[] = "Etapa em branco";
	if(!$dados['tpacodigoescola']) $inf_faltando[] = "Escola em branco";
	
	if($inf_faltando) {
		$al = array("alert"=>"Estão faltando informações para o cadastramento da turma : ".'\n'.implode('\n',$inf_faltando).'\n'."Caso o erro persista (mesmo selecionando as informações necessárias), solicitamos que utilize outra máquina. Sugerimos Sistema Operacional: Window, Linux. Browser: Internet Explorer, Firefox, Google Chrome","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
		alertlocation($al);
	}
	
	$sql = "INSERT INTO sispacto.turmasprofessoresalfabetizadores(
            tpacodigoescola, tpanomeescola, tpamuncodescola, tpaemailescola, 
            tpanometurma, tpahorarioinicioturma, tpahorariofimturma, 
            iusd, tpastatus, tpaoriginalcenso, tpaetapaturma, tpaconfirmaregencia)
            SELECT pk_cod_entidade, no_entidade, fk_cod_municipio, no_email,
            	   '".$dados['tpanometurma']."', '".$dados['tpahorarioinicioturma_hr'].":".$dados['tpahorarioinicioturma_mi']."', '".$dados['tpahorariofimturma_hr'].":".$dados['tpahorariofimturma_mi']."',
            	   '".$_SESSION['sispacto']['professoralfabetizador']['iusd']."', 'A', false, (SELECT no_etapa_ensino FROM educacenso_2013.tab_etapa_ensino WHERE pk_cod_etapa_ensino='".$dados['pk_cod_etapa_ensino']."'), 
            	   true
            FROM educacenso_2013.tab_entidade WHERE pk_cod_entidade='".$dados['tpacodigoescola']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Turma do professor inserida com sucesso","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
}

function gerenciarInformacoesTurmasProfessor($dados) {
	global $db;
	
	if($dados['tpaid']) {
		foreach($dados['tpaid'] as $tpaid) {

			$sql = "DELETE FROM sispacto.aprendizagemconhecimentoturma WHERE tpaid='".$tpaid."'";
			$db->executar($sql);
			
			$sql = "UPDATE sispacto.turmasprofessoresalfabetizadores SET 
					  tpatotalmeninos=".((is_numeric($dados['tpatotalmeninos'][$tpaid]))?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					  tpatotalmeninas=".((is_numeric($dados['tpatotalmeninas'][$tpaid]))?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
					  tpafaixaetaria5anos=".((is_numeric($dados['tpafaixaetaria5anos'][$tpaid]))?"'".$dados['tpafaixaetaria5anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria6anos=".((is_numeric($dados['tpafaixaetaria6anos'][$tpaid]))?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria7anos=".((is_numeric($dados['tpafaixaetaria7anos'][$tpaid]))?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria8anos=".((is_numeric($dados['tpafaixaetaria8anos'][$tpaid]))?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria9anos=".((is_numeric($dados['tpafaixaetaria9anos'][$tpaid]))?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria10anos=".((is_numeric($dados['tpafaixaetaria10anos'][$tpaid]))?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria11anos=".((is_numeric($dados['tpafaixaetaria11anos'][$tpaid]))?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
					  tpafaixaetariaacima11anos=".((is_numeric($dados['tpafaixaetariaacima11anos'][$tpaid]))?"'".$dados['tpafaixaetariaacima11anos'][$tpaid]."'":"NULL").",
					  tpatotalfreqeducinfantil=".((is_numeric($dados['tpatotalfreqeducinfantil'][$tpaid]))?"'".$dados['tpatotalfreqeducinfantil'][$tpaid]."'":"NULL").",
					  tpatotalbolsafamilia=".((is_numeric($dados['tpatotalbolsafamilia'][$tpaid]))?"'".$dados['tpatotalbolsafamilia'][$tpaid]."'":"NULL").",
					  tpatotalvivemcomunidade=".((is_numeric($dados['tpatotalvivemcomunidade'][$tpaid]))?"'".$dados['tpatotalvivemcomunidade'][$tpaid]."'":"NULL").",
					  tpatotalfreqcreche=".((is_numeric($dados['tpatotalfreqcreche'][$tpaid]))?"'".$dados['tpatotalfreqcreche'][$tpaid]."'":"NULL").",
					  tpatotalfreqpreescola=".((is_numeric($dados['tpatotalfreqpreescola'][$tpaid]))?"'".$dados['tpatotalfreqpreescola'][$tpaid]."'":"NULL")." 
					WHERE tpaid='".$tpaid."'";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	$al = array("alert"=>"Informações das Turmas gravadas com sucesso","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
	
}

function excluirMateriaisProfessoresFoto($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto.materiaisprofessoresfotos WHERE mpfid='".$dados['mpfid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Foto excluída com sucesso","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
	alertlocation($al);
	
}

function gravarAprendizagemTurma($dados) {
	global $db;
	
	if($dados['catid']) {
		
		foreach(array_keys($dados['catid']) as $tpaid) {
			$sql = "DELETE FROM sispacto.aprendizagemconhecimentoturma WHERE tpaid='".$tpaid."'";
			$db->executar($sql);
		}
		
		foreach($dados['catid'] as $tpaid => $arr) {
			
			foreach($arr as $catid) {
				
				unset($actid);
				
				if($catid) $actid = $db->pegaUm("SELECT actid FROM sispacto.aprendizagemconhecimentoturma WHERE catid='".$catid."' AND tpaid='".$tpaid."'");
			
				if(!$actid) {
					
					$sql = "INSERT INTO sispacto.aprendizagemconhecimentoturma(
		            		catid, tpaid, actsim, actparcialmente, actnao)
		    				VALUES ('".$catid."', 
		    						'".$tpaid."', 
		    						".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").", 
		    						".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").", 
		    						".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL").");";
					
					$db->executar($sql);
					
				}
			
			}
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Conhecimentos gravados com sucesso","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=aprendizagemturma&tpaid=".$dados['tpaid']);
	alertlocation($al);
	
}

function verificarAprendizagemTurma($dados) {
	global $db;
	
	$tot = $db->pegaUm("SELECT count(actid) as t FROM sispacto.aprendizagemconhecimentoturma a 
				 	    INNER JOIN sispacto.turmasprofessoresalfabetizadores t ON a.tpaid = t.tpaid 
				 	    WHERE t.iusd='".$dados['iusd']."'");
	
	if($tot) {
		echo 'TRUE';
	} else {
		echo 'FALSE';
	}
}
?>