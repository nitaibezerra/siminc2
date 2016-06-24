<?

function carregarProfessorAlfabetizador($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, 
								  re.reiid, 
								  su.uniuf, 
								  u.curid, 
								  u.docid, 
								  su.unisigla||' - '||su.uninome as descricao
						   FROM sispacto3.universidadecadastro u 
					 	   INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto3.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sispacto3.identificacaousuario i 
							   INNER JOIN sispacto3.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'");
	
	$_SESSION['sispacto3']['professoralfabetizador'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
															"curid" 	=> $arr['curid'], 
															"uncid" 	=> $arr['uncid'], 
															"reiid" 	=> $arr['reiid'], 
															"estuf" 	=> $arr['uniuf'], 
															"docid" 	=> $arr['docid'], 
															"iusd" 	   	=> $infprof['iusd'],
															"iuscpf"    => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function gerenciarMateriaisProfessores($dados) {
	global $db;
	
	if(!$_SESSION['sispacto3']['professoralfabetizador']['iusd']) {
		$al = array("alert"=>"Não foi possível gravar as informações sobre materiais. Tente novamente mais tarde.","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
		alertlocation($al);
	}
	
	$sql = "UPDATE sispacto3.materiaisprofessores
	   			SET recebeumaterialpacto='".substr($dados['recebeumaterialpacto'],0,1)."',
	       			criadocantinholeitura='".substr($dados['criadocantinholeitura'],0,1)."'
	 			WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'";
	
	$db->executar($sql);
	
	$sql = "INSERT INTO sispacto3.materiaisprofessores(
            	iusd, recebeumaterialpacto,  
            	criadocantinholeitura, mapstatus)
			 	SELECT '".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', 
			    		'".$dados['recebeumaterialpacto']."', 
			            '".$dados['criadocantinholeitura']."', 'A'
			    WHERE (SELECT mapid FROM sispacto3.materiaisprofessores WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."') IS NULL";
	
	$db->executar($sql);
	
	$db->commit();
			
	$sql = "SELECT mapid as t FROM sispacto3.materiaisprofessores WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'";
	$mapid = $db->pegaUm($sql);
	
	if($_FILES['arquivo']['error']=='0') {
		
		$campos	= array("mapid"	 => $mapid,
						"mpfdsc" => "'".$dados['mpfdsc']."'");	
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file = new FilesSimec("materiaisprofessoresfotos", $campos ,"sispacto3");
				
		$arquivoSalvo = $file->setUpload($dados['mafdsc']);
	}
	
	
	$al = array("alert"=>"Informações sobre materiais salvas com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}

function desvincularTurmaProfessor($dados) {
	global $db;
	
	$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET tpastatus='I', tpajustificativadesvinculacao='".$dados['tpajustificativadesvinculacao']."' WHERE tpaid='".$dados['tpaid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Desvinculação feita com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
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
										FROM sispacto3.turmasprofessoresalfabetizadores t 
										INNER JOIN territorios.municipio m ON m.muncod = t.tpamuncodescola 
										".(($wh)?"WHERE ".implode(" AND ",$wh):"")." ORDER BY t.tpaid");
	
	return $turmasprofessores;
}

function carregarEscolasPorMunicipio($dados) {
	global $db;
	
	$sql = "SELECT pk_cod_entidade as codigo, pk_cod_entidade || ' - ' || no_entidade as descricao FROM educacenso_2014.tab_entidade WHERE fk_cod_municipio='".$dados['muncod']."' ORDER BY no_entidade";
	$combo = $db->monta_combo('tpacodigoescola', $sql, 'S', 'Selecione', 'exibirDadosTurma', '', '', '200', 'S', 'tpacodigoescola', '', '');
	
}

function confirmarRegenciaTurma($dados) {
	global $db;
	if($dados['tpaconfirmaregencia']) {
		foreach($dados['tpaconfirmaregencia'] as $tpaid => $vl) {
			$db->executar("UPDATE sispacto3.turmasprofessoresalfabetizadores SET tpaconfirmaregencia=".$vl." WHERE tpaid='".$tpaid."'");
			$db->commit();			
		}
	}
	
	$al = array("alert"=>"Confirmação de regência feita com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
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
		$al = array("alert"=>"Estão faltando informações para o cadastramento da turma : ".'\n'.implode('\n',$inf_faltando).'\n'."Caso o erro persista (mesmo selecionando as informações necessárias), solicitamos que utilize outra máquina. Sugerimos Sistema Operacional: Window, Linux. Browser: Internet Explorer, Firefox, Google Chrome","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
		alertlocation($al);
	}
	
	$sql = "INSERT INTO sispacto3.turmasprofessoresalfabetizadores(
            tpacodigoescola, tpanomeescola, tpamuncodescola, tpaemailescola, 
            tpanometurma, tpahorarioinicioturma, tpahorariofimturma, 
            iusd, tpastatus, tpaoriginalcenso, tpaetapaturma, tpaconfirmaregencia)
            SELECT pk_cod_entidade, no_entidade, fk_cod_municipio, no_email,
            	   '".$dados['tpanometurma']."', '".$dados['tpahorarioinicioturma_hr'].":".$dados['tpahorarioinicioturma_mi']."', '".$dados['tpahorariofimturma_hr'].":".$dados['tpahorariofimturma_mi']."',
            	   '".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', 'A', false, (SELECT no_etapa_ensino FROM educacenso_2014.tab_etapa_ensino WHERE pk_cod_etapa_ensino='".$dados['pk_cod_etapa_ensino']."'), 
            	   true
            FROM educacenso_2014.tab_entidade WHERE pk_cod_entidade='".$dados['tpacodigoescola']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Turma do professor inserida com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}

function gerenciarInformacoesTurmasProfessor($dados) {
	global $db;
	
	if($dados['tpaid']) {
		foreach($dados['tpaid'] as $tpaid) {
			
			$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET 
					  tpatotalmeninos=".((is_numeric($dados['tpatotalmeninos'][$tpaid]))?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					  tpatotalmeninas=".((is_numeric($dados['tpatotalmeninas'][$tpaid]))?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
					  tpafaixaetariaabaixo6anos=".((is_numeric($dados['tpafaixaetariaabaixo6anos'][$tpaid]))?"'".$dados['tpafaixaetariaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria6anos=".((is_numeric($dados['tpafaixaetaria6anos'][$tpaid]))?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria7anos=".((is_numeric($dados['tpafaixaetaria7anos'][$tpaid]))?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria8anos=".((is_numeric($dados['tpafaixaetaria8anos'][$tpaid]))?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria9anos=".((is_numeric($dados['tpafaixaetaria9anos'][$tpaid]))?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria10anos=".((is_numeric($dados['tpafaixaetaria10anos'][$tpaid]))?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria11anos=".((is_numeric($dados['tpafaixaetaria11anos'][$tpaid]))?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
					  tpafaixaetariaacima11anos=".((is_numeric($dados['tpafaixaetariaacima11anos'][$tpaid]))?"'".$dados['tpafaixaetariaacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpabolsafamiliaabaixo6anos=".((is_numeric($dados['tpabolsafamiliaabaixo6anos'][$tpaid]))?"'".$dados['tpabolsafamiliaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia6anos=".((is_numeric($dados['tpabolsafamilia6anos'][$tpaid]))?"'".$dados['tpabolsafamilia6anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia7anos=".((is_numeric($dados['tpabolsafamilia7anos'][$tpaid]))?"'".$dados['tpabolsafamilia7anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia8anos=".((is_numeric($dados['tpabolsafamilia8anos'][$tpaid]))?"'".$dados['tpabolsafamilia8anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia9anos=".((is_numeric($dados['tpabolsafamilia9anos'][$tpaid]))?"'".$dados['tpabolsafamilia9anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia10anos=".((is_numeric($dados['tpabolsafamilia10anos'][$tpaid]))?"'".$dados['tpabolsafamilia10anos'][$tpaid]."'":"NULL").",
					  tpabolsafamilia11anos=".((is_numeric($dados['tpabolsafamilia11anos'][$tpaid]))?"'".$dados['tpabolsafamilia11anos'][$tpaid]."'":"NULL").",
					  tpabolsafamiliaacima11anos=".((is_numeric($dados['tpabolsafamiliaacima11anos'][$tpaid]))?"'".$dados['tpabolsafamiliaacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpavivemcomunidadeabaixo6anos=".((is_numeric($dados['tpavivemcomunidadeabaixo6anos'][$tpaid]))?"'".$dados['tpavivemcomunidadeabaixo6anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade6anos=".((is_numeric($dados['tpavivemcomunidade6anos'][$tpaid]))?"'".$dados['tpavivemcomunidade6anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade7anos=".((is_numeric($dados['tpavivemcomunidade7anos'][$tpaid]))?"'".$dados['tpavivemcomunidade7anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade8anos=".((is_numeric($dados['tpavivemcomunidade8anos'][$tpaid]))?"'".$dados['tpavivemcomunidade8anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade9anos=".((is_numeric($dados['tpavivemcomunidade9anos'][$tpaid]))?"'".$dados['tpavivemcomunidade9anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade10anos=".((is_numeric($dados['tpavivemcomunidade10anos'][$tpaid]))?"'".$dados['tpavivemcomunidade10anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidade11anos=".((is_numeric($dados['tpavivemcomunidade11anos'][$tpaid]))?"'".$dados['tpavivemcomunidade11anos'][$tpaid]."'":"NULL").",
					  tpavivemcomunidadeacima11anos=".((is_numeric($dados['tpavivemcomunidadeacima11anos'][$tpaid]))?"'".$dados['tpavivemcomunidadeacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpafreqcrecheabaixo6anos=".((is_numeric($dados['tpafreqcrecheabaixo6anos'][$tpaid]))?"'".$dados['tpafreqcrecheabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche6anos=".((is_numeric($dados['tpafreqcreche6anos'][$tpaid]))?"'".$dados['tpafreqcreche6anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche7anos=".((is_numeric($dados['tpafreqcreche7anos'][$tpaid]))?"'".$dados['tpafreqcreche7anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche8anos=".((is_numeric($dados['tpafreqcreche8anos'][$tpaid]))?"'".$dados['tpafreqcreche8anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche9anos=".((is_numeric($dados['tpafreqcreche9anos'][$tpaid]))?"'".$dados['tpafreqcreche9anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche10anos=".((is_numeric($dados['tpafreqcreche10anos'][$tpaid]))?"'".$dados['tpafreqcreche10anos'][$tpaid]."'":"NULL").",
					  tpafreqcreche11anos=".((is_numeric($dados['tpafreqcreche11anos'][$tpaid]))?"'".$dados['tpafreqcreche11anos'][$tpaid]."'":"NULL").",
					  tpafreqcrecheacima11anos=".((is_numeric($dados['tpafreqcrecheacima11anos'][$tpaid]))?"'".$dados['tpafreqcrecheacima11anos'][$tpaid]."'":"NULL").",
					  		
					  tpafreqpreescolaabaixo6anos=".((is_numeric($dados['tpafreqpreescolaabaixo6anos'][$tpaid]))?"'".$dados['tpafreqpreescolaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola6anos=".((is_numeric($dados['tpafreqpreescola6anos'][$tpaid]))?"'".$dados['tpafreqpreescola6anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola7anos=".((is_numeric($dados['tpafreqpreescola7anos'][$tpaid]))?"'".$dados['tpafreqpreescola7anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola8anos=".((is_numeric($dados['tpafreqpreescola8anos'][$tpaid]))?"'".$dados['tpafreqpreescola8anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola9anos=".((is_numeric($dados['tpafreqpreescola9anos'][$tpaid]))?"'".$dados['tpafreqpreescola9anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola10anos=".((is_numeric($dados['tpafreqpreescola10anos'][$tpaid]))?"'".$dados['tpafreqpreescola10anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescola11anos=".((is_numeric($dados['tpafreqpreescola11anos'][$tpaid]))?"'".$dados['tpafreqpreescola11anos'][$tpaid]."'":"NULL").",
					  tpafreqpreescolaacima11anos=".((is_numeric($dados['tpafreqpreescolaacima11anos'][$tpaid]))?"'".$dados['tpafreqpreescolaacima11anos'][$tpaid]."'":"NULL")." 
					WHERE tpaid='".$tpaid."'";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	$al = array("alert"=>"Informações das Turmas gravadas com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
}

function excluirMateriaisProfessoresFoto($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto3.materiaisprofessoresfotos WHERE mpfid='".$dados['mpfid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Foto excluída com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
	alertlocation($al);
	
}

function gravarAprendizagemTurma($dados) {
	global $db;
	
	if(!$_SESSION['sispacto3']['professoralfabetizador']['uncid']) {
		$al = array("alert"=>"Informações não encontradas. Tente novamente","location"=>"sispacto3.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	if($dados['tpaid']) {
		
		$sql = "SELECT rfuparcela FROM sispacto3.folhapagamentouniversidade WHERE fpbid='".$dados['fpbid']."' AND pflcod='".PFL_PROFESSORALFABETIZADOR."' AND uncid='".$_SESSION['sispacto3']['professoralfabetizador']['uncid']."'";
		$rfuparcela = $db->pegaUm($sql);
		
		foreach($dados['tpaid'] as $tpaid) {
			$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET 
					  tpatotalmeninos{$rfuparcela}=".((is_numeric($dados['tpatotalmeninos'][$tpaid]))?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					  tpatotalmeninas{$rfuparcela}=".((is_numeric($dados['tpatotalmeninas'][$tpaid]))?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
					  tpafaixaetariaabaixo6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaabaixo6anos'][$tpaid]))?"'".$dados['tpafaixaetariaabaixo6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria6anos'][$tpaid]))?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria7anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria7anos'][$tpaid]))?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria8anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria8anos'][$tpaid]))?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria9anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria9anos'][$tpaid]))?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria10anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria10anos'][$tpaid]))?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria11anos'][$tpaid]))?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
					  tpafaixaetariaacima11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaacima11anos'][$tpaid]))?"'".$dados['tpafaixaetariaacima11anos'][$tpaid]."'":"NULL")."
					WHERE tpaid='".$tpaid."'";
			
			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	if($dados['catid']) {
		
		foreach($dados['catid'] as $tpaid => $arr) {
			
			foreach($arr as $catid) {
				
				$sql = "SELECT actid as id_aprendizagem FROM sispacto3.aprendizagemconhecimentoturma WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
				$id_aprendizagem = $db->pegaUm($sql);
				
				if($id_aprendizagem) {
					$sql = "UPDATE sispacto3.aprendizagemconhecimentoturma SET actsim=".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").", 
																			   actparcialmente=".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").",
																			   actnao=".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")." 
							WHERE actid='".$id_aprendizagem."'";
					
					$db->executar($sql);
					$db->commit();
					
				} else {
					
					$sql = "SELECT count(*) as qtd FROM sispacto3.aprendizagemconhecimentoturma WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
					$qtd = $db->pegaUm($sql);
					
					if(!$qtd) {
						
						$sql = "INSERT INTO sispacto3.aprendizagemconhecimentoturma(
			            		catid, tpaid, actsim, actparcialmente, actnao) 
								SELECT '".$catid."', 
									   '".$tpaid."', 
									   ".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").", 
									   ".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").", 
									   ".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")." 
								FROM coalesce((SELECT actid::text FROM sispacto3.aprendizagemconhecimentoturma WHERE catid='".$catid."' AND tpaid='".$tpaid."'),NULL) as foo WHERE foo IS NULL";
						
						$db->executar($sql);
						$db->commit();
						
					}
					
					
				}
				
			}
		}
	}
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}





function verificarAprendizagemTurma($dados) {
	global $db;
	
	$tot = $db->pegaUm("SELECT count(actid) as t FROM sispacto3.aprendizagemconhecimentoturma a 
				 	    INNER JOIN sispacto3.turmasprofessoresalfabetizadores t ON a.tpaid = t.tpaid 
				 	    WHERE t.iusd='".$dados['iusd']."'");
	
	if($tot) {
		echo 'TRUE';
	} else {
		echo 'FALSE';
	}
}

function gerenciarUsoMateriaisDidaticos($dados) {
	global $db;
	
	if($dados['usomaterialdidatico']) {
		foreach($dados['usomaterialdidatico'] as $catid => $umdopcao) {
			
			$umdid = $db->pegaUm("SELECT umdid FROM sispacto3.usomateriaisdidaticos WHERE catid='{$catid}' AND iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
			
			if($umdid) {
				
				$sql = "UPDATE sispacto3.usomateriaisdidaticos SET umdopcao='{$umdopcao}' WHERE umdid='{$umdid}'";
				
				$db->executar($sql);
				
			} else {
				
				$sql = "INSERT INTO sispacto3.usomateriaisdidaticos(
            			iusd, catid, umdopcao)
    					VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '{$catid}', '{$umdopcao}');";
				
				$db->executar($sql);
				
			}
			
		}
	}
	
	$db->commit();

	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
}

function gravarRelatoExperiencia($dados) {
	global $db;
	if(!$dados['reeturma']) $err[] = "Preencha as turmas";	
	if(!$dados['reeobjetivo']) $err[] = "Preencha os Objetivos principais da experiência";
	if(!$dados['reetecnicas']) $err[] = "Preencha as Técnicas utilizadas";
	if(!$dados['reedificuldades']) $err[] = "Preencha as Dificuldades na realização da atividade";
	$dt_val = explode("/",$dados['reeperiodoexperienciaini']);
	if(!checkdate((($dt_val[1])?$dt_val[1]:"0"),(($dt_val[0])?$dt_val[0]:"0"),(($dt_val[2])?$dt_val[2]:"0"))) $err[] = "Formato da data início inválida";
	$dt_val = explode("/",$dados['reeperiodoexperienciafim']);
	if(!checkdate((($dt_val[1])?$dt_val[1]:"0"),(($dt_val[0])?$dt_val[0]:"0"),(($dt_val[2])?$dt_val[2]:"0"))) $err[] = "Formato da data fim inválida";
	
	if($err) {
		$al = array("alert"=>"Foram encontrados problemas :".'\n'.implode('\n',$err),"javascript"=>"window.history.back();");
		alertlocation($al);
	}
	
	if(!$_SESSION['sispacto3']['professoralfabetizador']['iusd']) {
		$al = array("alert"=>"Foram encontrados problemas internos. Por favor tente novamente.","location"=>"sispacto3.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	$reeid = $db->pegaUm("SELECT reeid FROM sispacto3.relatoexperiencia WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
	
	$idx = array_keys($dados);
	
	foreach($idx as $ix)
	if(is_array($dados[$ix]))
	foreach($dados[$ix] as $key => $vlr)
		$dados[$ix][$key] = $vlr.(($dados['tx_'.$ix.'_'.$vlr])?"||".$dados['tx_'.$ix.'_'.$vlr]:"");
	else $dados[$ix] = $dados[$ix].(($dados['tx_'.$ix.'_'.$dados[$ix]])?"||".$dados['tx_'.$ix.'_'.$dados[$ix]]:"");
	
	
	if($reeid) {
		
		$sql = "UPDATE sispacto3.relatoexperiencia SET 
						reeareatematica       ='".$dados['reeareatematica']."', 
						reeturma			  ='".implode(";",$dados['reeturma'])."', 
						reeperiodoexperienciaini ='".formata_data_sql($dados['reeperiodoexperienciaini'])."', 
						reeperiodoexperienciafim ='".formata_data_sql($dados['reeperiodoexperienciafim'])."',
			            reeobjetivo			  ='".addslashes(implode(";",$dados['reeobjetivo']))."', 
						reetecnicas			  ='".addslashes(implode(";",$dados['reetecnicas']))."', 
						reetempoduracao		  ='".$dados['reetempoduracao']."', 
						reeorganizacao		  ='".$dados['reeorganizacao']."', 
						reemateriaisutilizados='".$dados['reemateriaisutilizados']."', 
			            reelocal			  ='".$dados['reelocal']."', 
						reedificuldades		  ='".implode(";",$dados['reedificuldades'])."', 
						reeenvolvimento		  ='".$dados['reeenvolvimento']."', 
						reetitulo			  ='".addslashes($dados['tx_reetitulo'])."', 
						reeresumo			  ='".substr(addslashes($dados['tx_reeresumo']),0,1000)."', 
			            reeobjetivosalcancados='".$dados['reeobjetivosalcancados']."', 
			            reerepetirexperiencia ='".$dados['reerepetirexperiencia']."' 
			    WHERE reeid='".$reeid."'";
		
	} else {
		
		$sql = "INSERT INTO sispacto3.relatoexperiencia(
			            iusd, 
						reeareatematica, 
						reeturma, 
						reeperiodoexperienciaini, 
						reeperiodoexperienciafim,
			            reeobjetivo, 
						reetecnicas, 
						reetempoduracao, 
						reeorganizacao, 
						reemateriaisutilizados, 
			            reelocal, 
						reedificuldades, 
						reeenvolvimento, 
						reetitulo, 
						reeresumo, 
			            reeobjetivosalcancados, reerepetirexperiencia)
			    VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', 
			    		'".$dados['reeareatematica']."', 
			    		'".implode(";",$dados['reeturma'])."', 
			    		'".formata_data_sql($dados['reeperiodoexperienciaini'])."', 
			    		'".formata_data_sql($dados['reeperiodoexperienciafim'])."',
			            '".addslashes(implode(";",$dados['reeobjetivo']))."', 
			            '".addslashes(implode(";",$dados['reetecnicas']))."', 
			            '".addslashes($dados['reetempoduracao'])."', 
			            '".addslashes($dados['reeorganizacao'])."', 
			            '".addslashes($dados['reemateriaisutilizados'])."', 
			            '".addslashes($dados['reelocal'])."', 
			            '".implode(";",$dados['reedificuldades'])."', 
			            '".addslashes($dados['reeenvolvimento'])."', 
			            '".addslashes($dados['tx_reetitulo'])."', 
			            '".substr(addslashes($dados['tx_reeresumo']),0,1000)."', 
			            '".addslashes($dados['reeobjetivosalcancados'])."', 
			            '".addslashes($dados['reerepetirexperiencia'])."');";
	}
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
}

function estruturaImpressaoANA($dados) {

	$es['imaparticiparam'] = array('texto' => 'Nesta escola uma ou mais crianças participaram da Avaliação Nacional de Aprendizagem (ANA)?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'S','descricao'=>'Sim'),
					array('valor'=>'N','descricao'=>'Não')
			)
	);
	
	$es['imaacessoresultados'] = array('texto' => 'Você teve acesso aos resultados da ANA de sua escola?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'S','descricao'=>'Sim'),
					array('valor'=>'N','descricao'=>'Não')
			)
	);
	
	$es['imaresultadosescola'] = array('texto' => 'Os resultados da ANA na sua escola:',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Estão dentro de sua expectativa'),
					array('valor'=>'2','descricao'=>'Estão acima de sua expectativa'),
					array('valor'=>'3','descricao'=>'Estão abaixo de sua expectativa'),
					array('valor'=>'4','descricao'=>'Não sabe avaliar'),
			)
	);
	
	
	$es['imaaspectos'] = array('texto' => 'Qual a sua avaliação sobre os seguintes aspectos da ANA:',
			'tipo' => 'gridradio',
			'colunas' => array(
							array('codigo'=>'P','descricao'=>'Péssimo'),
							array('codigo'=>'U','descricao'=>'Ruim'),
							array('codigo'=>'R','descricao'=>'Regular'),
							array('codigo'=>'B','descricao'=>'Bom'),
							array('codigo'=>'O','descricao'=>'Ótimo'),
							array('codigo'=>'N','descricao'=>'Não sei informar')
						 ),
				
			'linhas' => array(
					array('codigo'=>'orientacoes','descricao'=>'Orientações previas a aplicação'),
					array('codigo'=>'tempoaplicacao','descricao'=>'Tempo de aplicação da avaliação'),
					array('codigo'=>'horarioaplicacao','descricao'=>'Horário de aplicação da avaliação'),
					array('codigo'=>'quantidadequestoes','descricao'=>'Quantidade de questões'),
					array('codigo'=>'clarezaquestoes','descricao'=>'Clareza na apresentação das questões'),
					array('codigo'=>'aplicadorexterno','descricao'=>'Necessidade de aplicador externo'),
					array('codigo'=>'localavaliacao','descricao'=>'Local de aplicação da avaliação'),
					array('codigo'=>'apresentacaoavaliacao','descricao'=>'Forma de apresentação da avaliação'),
					array('codigo'=>'apresentacaoresultados','descricao'=>'Forma de apresentação dos resultados da escola')
						
			)
	);
	
	$es['imafatores'] = array('texto' => 'Avalie como os fatores abaixo interferiram nos resultados de alfabetização das crianças:',
			'tipo' => 'gridradio',
			'colunas' => array(
					array('codigo'=>'I','descricao'=>'Não interferiu'),
					array('codigo'=>'P','descricao'=>'Interferiu pouco'),
					array('codigo'=>'M','descricao'=>'Interferiu muito'),
					array('codigo'=>'N','descricao'=>'Não sei informar')
			),
	
			'linhas' => array(
					array('codigo'=>'gestaoescolar','descricao'=>'Gestão escolar'),
					array('codigo'=>'formacaoprofessores','descricao'=>'Formação dos professores'),
					array('codigo'=>'praticaspedagogicas','descricao'=>'Práticas pedagógicas de sala de aula'),
					array('codigo'=>'perfilalunos','descricao'=>'Perfil dos  alunos'),
					array('codigo'=>'recursosdidaticos','descricao'=>'Recursos didáticos'),
					array('codigo'=>'estruturafisica','descricao'=>'Estrutura física da escola'),
					array('codigo'=>'participacaofamilia','descricao'=>'Participação da família na vida escolar da criança'),
					array('codigo'=>'relacoesinterpessoais','descricao'=>'Relações interpessoais da escola')
	
			)
	);
	
	return $es;
	
	
}

function estruturaContribuicaoPacto($dados) {


	$es['cpacontribuicao'] = array('texto' => 'Informe qual foi a contribuição da Formação do Pacto para:',
			'tipo' => 'gridradio',
			'colunas' => array(
					array('codigo'=>'M','descricao'=>'Contribuiu muito'),
					array('codigo'=>'P','descricao'=>'Contribuiu pouco'),
					array('codigo'=>'C','descricao'=>'Não contribuiu')
			),

			'linhas' => array(
					array('codigo'=>'1','descricao'=>'a reflexão sobre a prática pedagógica'),
					array('codigo'=>'2','descricao'=>'o aprofundamento da compreensão sobre o currículo nos anos iniciais do Ensino Fundamental e os direitos de aprendizagem'),
					array('codigo'=>'3','descricao'=>'a ampliação de conhecimentos sobre avaliação no ciclo de alfabetização'),
					array('codigo'=>'4','descricao'=>'a ampliação de estratégias de inclusão de crianças com deficiências'),
					array('codigo'=>'5','descricao'=>'o planejamento de mais estratégias para lidar com a heterogeneidade presente nas salas de aula quanto aos processos de aprendizagem'),
					array('codigo'=>'6','descricao'=>'a análise e criação de propostas de organização de rotinas da alfabetização na perspectiva do letramento'),
					array('codigo'=>'7','descricao'=>'o planejamento de projetos didáticos e sequências didáticas, integrando diferentes componentes curriculares'),
					array('codigo'=>'8','descricao'=>'o planejamento de aulas por meio de situações diferenciadas de ensino'),
					array('codigo'=>'9','descricao'=>'o uso de jogos e recursos didáticos diversificados'),
					array('codigo'=>'10','descricao'=>'o uso de recursos didáticos distribuídos pelo Ministério da Educação (livros didáticos e obras complementares aprovados no PNLD; livros do PNBE e PNBE Especial; jogos didáticos)')

			)
	);
	
	$es['cpadificuldade'] = array('texto' => 'Informe a dificuldade encontrada para:',
			'tipo' => 'gridradio',
			'colunas' => array(
					array('codigo'=>'M','descricao'=>'Muita dificuldade'),
					array('codigo'=>'P','descricao'=>'Pouca dificuldade'),
					array('codigo'=>'N','descricao'=>'Nenhuma dificuldade')
			),
	
			'linhas' => array(
					array('codigo'=>'1','descricao'=>'comunicar-se com o Ministério da Educação pelo e-mail '. $_SESSION['email_sistema']),
					array('codigo'=>'2','descricao'=>'utilizar o Sispacto 2014')
	
			)
	);
	

	return $es;


}

function estruturaRelatoExperiencia($dados) {
	
	$es['reeareatematica'] = array('texto' => '1. Área temática',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'P','descricao'=>'Língua Portuguesa'),
					array('valor'=>'M','descricao'=>'Matemática')
			)
	);
		
	$es['reeturma'] = array('texto' => '2. Turma',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'1º ano'),
					array('valor'=>'2','descricao'=>'2º ano/ 1ª série'),
					array('valor'=>'3','descricao'=>'2º ano/ 1ª série'),
					array('valor'=>'4','descricao'=>'3º ano/ 2ª série'),
					array('valor'=>'5','descricao'=>'3ª série'),
					array('valor'=>'6','descricao'=>'Multisseriada')
			)
	);
		
	$es['reeperiodoexperiencia'] = array('texto' => '3. Período em que a experiência foi realizada',
			'tipo' => 'data',
			'datas' => array(
					array('valor'=>'ini','descricao'=>'Início (dd/mm/aaaa)'),
					array('valor'=>'fim','descricao'=>'Término (dd/mm/aaaa)')
			)
	);
		
		
	$es['reeobjetivo'] = array('texto' => '4. Objetivo principal da experiência',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Apropriar-se do Sistema de Escrita Alfabética (SEA)'),
					array('valor'=>'2','descricao'=>'Reconhecer a função social de um texto'),
					array('valor'=>'3','descricao'=>'Identificar e utilizar diferentes suportes textuais'),
					array('valor'=>'4','descricao'=>'Produzir textos utilizando diversos gêneros'),
					array('valor'=>'5','descricao'=>'Conhecer e fazer uso da norma padrão na escrita de textos'),
					array('valor'=>'6','descricao'=>'Outro objetivo','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reetecnicas'] = array('texto' => '5. Técnicas utilizadas',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Brincadeira'),
					array('valor'=>'2','descricao'=>'Jogo'),
					array('valor'=>'3','descricao'=>'Dramatização'),
					array('valor'=>'4','descricao'=>'Exposição dialogada'),
					array('valor'=>'5','descricao'=>'Exercício escrito'),
					array('valor'=>'6','descricao'=>'Leitura em voz alta'),
					array('valor'=>'7','descricao'=>'Recorte e colagem'),
					array('valor'=>'8','descricao'=>'Outra técnica','complementotexto' => 'Qual?'),
			)
	);
		
		
	$es['reetempoduracao'] = array('texto' => '6. Tempo de duração da experiência',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Menos de 20 minutos'),
					array('valor'=>'2','descricao'=>'Entre 20 e 40 minutos'),
					array('valor'=>'3','descricao'=>'Mais de 40 minutos')
			)
	);
		
	$es['reeorganizacao'] = array('texto' => '7. Organização',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Individual'),
					array('valor'=>'2','descricao'=>'2 pessoas'),
					array('valor'=>'3','descricao'=>'3 pessoas'),
					array('valor'=>'4','descricao'=>'Mais de 3 pessoas')
			)
	);
		
	$es['reemateriaisutilizados'] = array('texto' => '8. Materiais utilizados',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Obras complementares do PNLD'),
					array('valor'=>'2','descricao'=>'Obras literárias do PNBE'),
					array('valor'=>'3','descricao'=>'Outras obras literárias'),
					array('valor'=>'4','descricao'=>'Livros didáticos do PNLD'),
					array('valor'=>'5','descricao'=>'Jogos de alfabetização'),
					array('valor'=>'6','descricao'=>'Jogos de matemática'),
					array('valor'=>'7','descricao'=>'Revistas, jornais, gibis e outros suportes textuais'),
					array('valor'=>'8','descricao'=>'Caixa matemática'),
					array('valor'=>'9','descricao'=>'Outros materiais','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reelocal'] = array('texto' => '9. Local em que a atividade foi realizada',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Em sala de aula (Na escola)'),
					array('valor'=>'2','descricao'=>'No pátio (Na escola)'),
					array('valor'=>'3','descricao'=>'Outro ambiente (Na escola)', 'complementotexto' => 'Qual?'),
					array('valor'=>'4','descricao'=>'Praça, parque ou jardim (Fora da escola)'),
					array('valor'=>'5','descricao'=>'Teatro/ cinema (Fora da escola)'),
					array('valor'=>'6','descricao'=>'Biblioteca (Fora da escola)'),
					array('valor'=>'7','descricao'=>'Quadras esportivas ou similares (Fora da escola)'),
					array('valor'=>'8','descricao'=>'Outro espaço (Fora da escola)','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reedificuldades'] = array('texto' => '10. Dificuldades na realização da atividade',
			'tipo' => 'checkbox',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Não houve dificuldade para realizar a atividade'),
					array('valor'=>'2','descricao'=>'Incompreensão da atividade por parte das crianças'),
					array('valor'=>'3','descricao'=>'Dificuldade das crianças em realizar as atividades propostas'),
					array('valor'=>'4','descricao'=>'Desinteresse da maioria das crianças pela atividade'),
					array('valor'=>'5','descricao'=>'Tempo escasso para concluir a atividade'),
					array('valor'=>'6','descricao'=>'Falta de materiais apropriados para realizar a atividade'),
					array('valor'=>'7','descricao'=>'Espaço inadequado para realizar as atividades'),
					array('valor'=>'8','descricao'=>'Outra dificuldade','complementotexto' => 'Qual?'),
			)
	);
		
	$es['reeenvolvimento'] = array('texto' => '11. Como você avalia o grau de envolvimento das crianças?',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Todas as crianças participaram da atividade'),
					array('valor'=>'2','descricao'=>'Mais da metade das crianças participou da atividade'),
					array('valor'=>'3','descricao'=>'Metade das crianças participou da atividade'),
					array('valor'=>'4','descricao'=>'Menos da metade as crianças participou da atividade'),
					array('valor'=>'5','descricao'=>'Nenhuma criança participou da atividade')
			)
	);
		
	$es['reetitulo'] = array('texto' => '12. Titulo da experiência',
			'text' => array('maxsize'=>'100','rows'=>'4','cols'=>'40')
	);
		
	$es['reeresumo'] = array('texto' => '13. Resumo da experiência',
			'text' => array('maxsize'=>'1000','rows'=>'4','cols'=>'40','dica'=>'Escreva uma síntese objetiva da atividade, considerando que as características gerais já foram indicadas nos itens anteriores. Valorize as informações essenciais que permitam a qualquer leitor entender o que foi feito.')
	);
		
	$es['reeobjetivosalcancados'] = array('texto' => '14. Os objetivos principais foram alcançados?',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Sim'),
					array('valor'=>'2','descricao'=>'Não'),
					array('valor'=>'3','descricao'=>'Parcialmente','complementotexto' => 'Por quê?'),
			)
	);
		
	$es['reerepetirexperiencia'] = array('texto' => '15. Você pretende repetir essa experiência futuramente?',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Sim'),
					array('valor'=>'2','descricao'=>'Não')
			)
	);
	
	return $es;
}

function gravarImpressoesANA($dados) {
	global $db;
	
	if($dados['tpaid']) {
		foreach($dados['tpaid'] as $tpaid) {
			$imaid = $db->pegaUm("SELECT imaid FROM sispacto3.impressoesana WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."' AND tpaid='{$tpaid}'");
			
			if($imaid) $sql = "UPDATE sispacto3.impressoesana SET imaparticiparam='".$dados['imaparticiparam'][$tpaid]."', imaacessoresultados='".$dados['imaacessoresultados'][$tpaid]."', imaresultadosescola='".$dados['imaresultadosescola'][$tpaid]."', 
					            imaaspectosorientacoes='".$dados['imaaspectosorientacoes'][$tpaid]."', imaaspectostempoaplicacao='".$dados['imaaspectostempoaplicacao'][$tpaid]."', imaaspectoshorarioaplicacao='".$dados['imaaspectoshorarioaplicacao'][$tpaid]."', 
					            imaaspectosquantidadequestoes='".$dados['imaaspectosquantidadequestoes'][$tpaid]."', imaaspectosclarezaquestoes='".$dados['imaaspectosclarezaquestoes'][$tpaid]."', imaaspectosaplicadorexterno='".$dados['imaaspectosaplicadorexterno'][$tpaid]."', 
					            imaaspectoslocalavaliacao='".$dados['imaaspectoslocalavaliacao'][$tpaid]."', imaaspectosapresentacaoavaliacao='".$dados['imaaspectosapresentacaoavaliacao'][$tpaid]."', 
					            imaaspectosapresentacaoresultados='".$dados['imaaspectosapresentacaoresultados'][$tpaid]."', imafatoresgestaoescolar='".$dados['imafatoresgestaoescolar'][$tpaid]."', imafatoresformacaoprofessores='".$dados['imafatoresformacaoprofessores'][$tpaid]."', 
					            imafatorespraticaspedagogicas='".$dados['imafatorespraticaspedagogicas'][$tpaid]."', imafatoresperfilalunos='".$dados['imafatoresperfilalunos'][$tpaid]."', imafatoresrecursosdidaticos='".$dados['imafatoresrecursosdidaticos'][$tpaid]."', 
					            imafatoresestruturafisica='".$dados['imafatoresestruturafisica'][$tpaid]."', imafatoresparticipacaofamilia='".$dados['imafatoresparticipacaofamilia'][$tpaid]."', imafatoresrelacoesinterpessoais='".$dados['imafatoresrelacoesinterpessoais'][$tpaid]."' 
					            WHERE imaid='{$imaid}'";
			else $sql = "INSERT INTO sispacto3.impressoesana(
					            iusd, imaparticiparam, imaacessoresultados, imaresultadosescola, 
					            imaaspectosorientacoes, imaaspectostempoaplicacao, imaaspectoshorarioaplicacao, 
					            imaaspectosquantidadequestoes, imaaspectosclarezaquestoes, imaaspectosaplicadorexterno, 
					            imaaspectoslocalavaliacao, imaaspectosapresentacaoavaliacao, 
					            imaaspectosapresentacaoresultados, imafatoresgestaoescolar, imafatoresformacaoprofessores, 
					            imafatorespraticaspedagogicas, imafatoresperfilalunos, imafatoresrecursosdidaticos, 
					            imafatoresestruturafisica, imafatoresparticipacaofamilia, imafatoresrelacoesinterpessoais, 
					            tpaid)
					    VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '".$dados['imaparticiparam'][$tpaid]."', '".$dados['imaacessoresultados'][$tpaid]."', '".$dados['imaresultadosescola'][$tpaid]."', 
					            '".$dados['imaaspectosorientacoes'][$tpaid]."', '".$dados['imaaspectostempoaplicacao'][$tpaid]."', '".$dados['imaaspectoshorarioaplicacao'][$tpaid]."', 
					            '".$dados['imaaspectosquantidadequestoes'][$tpaid]."', '".$dados['imaaspectosclarezaquestoes'][$tpaid]."', '".$dados['imaaspectosaplicadorexterno'][$tpaid]."', 
					            '".$dados['imaaspectoslocalavaliacao'][$tpaid]."', '".$dados['imaaspectosapresentacaoavaliacao'][$tpaid]."', 
					            '".$dados['imaaspectosapresentacaoresultados'][$tpaid]."', '".$dados['imafatoresgestaoescolar'][$tpaid]."', '".$dados['imafatoresformacaoprofessores'][$tpaid]."', 
					            '".$dados['imafatorespraticaspedagogicas'][$tpaid]."', '".$dados['imafatoresperfilalunos'][$tpaid]."', '".$dados['imafatoresrecursosdidaticos'][$tpaid]."', 
					            '".$dados['imafatoresestruturafisica'][$tpaid]."', '".$dados['imafatoresparticipacaofamilia'][$tpaid]."', '".$dados['imafatoresrelacoesinterpessoais'][$tpaid]."', 
					            '{$tpaid}');";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	

}

function estruturaQuestoesDiversas($dados) {
	
	$es['qudformaapoia'] = array('texto' => '1. De que forma a direção da escola apoia os professores alfabetizadores que participam do Pacto Nacional pela Alfabetização na Idade Certa?',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Disponibilizando meios e espaços adequados para os estudos.'),
					array('valor'=>'2','descricao'=>'Incentivando a revisão do Projeto Político-Pedagógico.'),
					array('valor'=>'3','descricao'=>'Promovendo reuniões de pais e mestres e/ou eventos pedagógicos para apresentar o Pacto.'),
					array('valor'=>'4','descricao'=>'Disponibilizando materiais de apoio à formação.'),
					array('valor'=>'5','descricao'=>'A direção da escola não apoia os professores que participam do Pacto.')
			)
	);
	

	$es['qudperiodicidade'] = array('texto' => '2. Com que periodicidade a escola promove atividades visando envolver as famílias dos estudantes no processos de alfabetização e letramento dos filhos?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'S','descricao'=>'Semanal'),
					array('valor'=>'M','descricao'=>'Mensal'),
					array('valor'=>'B','descricao'=>'Bimestral'),
					array('valor'=>'E','descricao'=>'Semestral'),
					array('valor'=>'N','descricao'=>'A escola não promove atividades com as famílias'),
			)
	);
	
	$es['qudformaparticipa'] = array('texto' => '3. De que forma o Conselho Escolar participa das atividades do Pacto Nacional da Alfabetização na Idade Certa?',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Apresenta sugestões e críticas ao programa.'),
					array('valor'=>'2','descricao'=>'Acompanha o processo formativo dos professores alfabetizadores.'),
					array('valor'=>'3','descricao'=>'Propõe alterações no planejamento pedagógico da escola voltado para as turmas de alfabetização.'),
					array('valor'=>'4','descricao'=>'O Conselho Escolar nunca discutiu sobre alfabetização.'),
					array('valor'=>'5','descricao'=>'Outra.','complementotexto' => 'Qual?'),
					array('valor'=>'6','descricao'=>'A escola não possui Conselho Escolar.')
			)
	);
	
	$es['qudmedidaparticipa'] = array('texto' => '4. Em que medida a comunidade escolar participa do Pacto pela Alfabetização na Idade Certa:',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'A','descricao'=>'Participa ativamente'),
					array('valor'=>'M','descricao'=>'Participa moderadamente'),
					array('valor'=>'P','descricao'=>'Participa pouco'),
					array('valor'=>'N','descricao'=>'Não participa')
			)
	);
	
	$es['qudmedidacontribui'] = array('texto' => '5. Em que medida o Pacto contribui para o seu conhecimento acerca do direitos de aprendizagem das crianças, nos três primeiros anos do ensino fundamental?',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'D','descricao'=>'Contribui decisivamente'),
					array('valor'=>'M','descricao'=>'Contribui moderadamente'),
					array('valor'=>'P','descricao'=>'Contribui um pouco'),
					array('valor'=>'N','descricao'=>'Não contribui')
			)
	);
	


	return $es;
}

function gravarQuestoesDiversas($dados) {
	global $db;
	
	$idx = array_keys($dados);
	
	foreach($idx as $ix)
	if(is_array($dados[$ix]))
	foreach($dados[$ix] as $key => $vlr)
		$dados[$ix][$key] = $vlr.(($dados['tx_'.$ix.'_'.$vlr])?"||".$dados['tx_'.$ix.'_'.$vlr]:"");
	else $dados[$ix] = $dados[$ix].(($dados['tx_'.$ix.'_'.$dados[$ix]])?"||".$dados['tx_'.$ix.'_'.$dados[$ix]]:"");
	
	$qudid = $db->pegaUm("SELECT qudid FROM sispacto3.questoesdiversasatv8 WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
	
	if($qudid) $sql = "UPDATE sispacto3.questoesdiversasatv8 SET qudformaapoia='".implode(";",$dados['qudformaapoia'])."', 
														  qudperiodicidade='".$dados['qudperiodicidade']."', 
														  qudformaparticipa='".implode(";",$dados['qudformaparticipa'])."',
														  qudmedidaparticipa='".$dados['qudmedidaparticipa']."',
														  qudmedidacontribui='".$dados['qudmedidacontribui']."' WHERE qudid='{$qudid}'";
		
	else $sql = "INSERT INTO sispacto3.questoesdiversasatv8(
            iusd, qudformaapoia, qudperiodicidade, qudformaparticipa, 
            qudmedidaparticipa, qudmedidacontribui)
    		VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '".implode(";",$dados['qudformaapoia'])."', '".$dados['qudperiodicidade']."', '".implode(";",$dados['qudformaparticipa'])."', 
            '".$dados['qudmedidaparticipa']."', '".$dados['qudmedidacontribui']."');";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}

function gravarContribuicaoPacto($dados) {
	global $db;
	
	$idx = array_keys($dados);
	
	foreach($idx as $ix)
	if(is_array($dados[$ix]))
	foreach($dados[$ix] as $key => $vlr)
		$dados[$ix][$key] = $vlr.(($dados['tx_'.$ix.'_'.$vlr])?"||".$dados['tx_'.$ix.'_'.$vlr]:"");
	else $dados[$ix] = $dados[$ix].(($dados['tx_'.$ix.'_'.$dados[$ix]])?"||".$dados['tx_'.$ix.'_'.$dados[$ix]]:"");
	
	$cpaid = $db->pegaUm("SELECT cpaid FROM sispacto3.contribuicaopacto WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");

	if($cpaid) $sql = "UPDATE sispacto3.contribuicaopacto SET cpacontribuicao1='".$dados['cpacontribuicao1']."',
															  cpacontribuicao2='".$dados['cpacontribuicao2']."',
															  cpacontribuicao3='".$dados['cpacontribuicao3']."',
															  cpacontribuicao4='".$dados['cpacontribuicao4']."',
															  cpacontribuicao5='".$dados['cpacontribuicao5']."',
															  cpacontribuicao6='".$dados['cpacontribuicao6']."',
															  cpacontribuicao7='".$dados['cpacontribuicao7']."',
															  cpacontribuicao8='".$dados['cpacontribuicao8']."',
															  cpacontribuicao9='".$dados['cpacontribuicao9']."',
															  cpacontribuicao10='".$dados['cpacontribuicao10']."',
															  cpadificuldade1='".$dados['cpadificuldade1']."',
															  cpadificuldade2='".$dados['cpadificuldade2']."' 
													 WHERE cpaid='{$cpaid}'";
	
	else $sql = "INSERT INTO sispacto3.contribuicaopacto(
            iusd, cpacontribuicao1, cpacontribuicao2, cpacontribuicao3, 
            cpacontribuicao4, cpacontribuicao5, cpacontribuicao6, cpacontribuicao7, 
            cpacontribuicao8, cpacontribuicao9, cpacontribuicao10, cpadificuldade1, 
            cpadificuldade2)
    VALUES ('".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."', '".$dados['cpacontribuicao1']."', '".$dados['cpacontribuicao2']."', '".$dados['cpacontribuicao3']."', 
            '".$dados['cpacontribuicao4']."', '".$dados['cpacontribuicao5']."', '".$dados['cpacontribuicao6']."', '".$dados['cpacontribuicao7']."', 
            '".$dados['cpacontribuicao8']."', '".$dados['cpacontribuicao9']."', '".$dados['cpacontribuicao10']."', '".$dados['cpadificuldade1']."', 
            '".$dados['cpadificuldade2']."');";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}




function gravarAprendizagemTurma2($dados) {
	global $db;

	if($dados['tpaid']) {

		$sql = "SELECT rfuparcela FROM sispacto3.folhapagamentouniversidade WHERE fpbid='".$dados['fpbid']."' AND pflcod='".PFL_PROFESSORALFABETIZADOR."' AND uncid='".$_SESSION['sispacto3']['professoralfabetizador']['uncid']."'";
		$rfuparcela = $db->pegaUm($sql);

		foreach($dados['tpaid'] as $tpaid) {
			$sql = "UPDATE sispacto3.turmasprofessoresalfabetizadores SET
					tpatotalmeninos{$rfuparcela}=".((is_numeric($dados['tpatotalmeninos'][$tpaid]))?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					tpatotalmeninas{$rfuparcela}=".((is_numeric($dados['tpatotalmeninas'][$tpaid]))?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
							tpafaixaetariaabaixo6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaabaixo6anos'][$tpaid]))?"'".$dados['tpafaixaetariaabaixo6anos'][$tpaid]."'":"NULL").",
							  tpafaixaetaria6anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria6anos'][$tpaid]))?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
							  		tpafaixaetaria7anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria7anos'][$tpaid]))?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
							  				tpafaixaetaria8anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria8anos'][$tpaid]))?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
							  				tpafaixaetaria9anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria9anos'][$tpaid]))?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
							  						tpafaixaetaria10anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria10anos'][$tpaid]))?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
							  								tpafaixaetaria11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetaria11anos'][$tpaid]))?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
							  										tpafaixaetariaacima11anos{$rfuparcela}=".((is_numeric($dados['tpafaixaetariaacima11anos'][$tpaid]))?"'".$dados['tpafaixaetariaacima11anos'][$tpaid]."'":"NULL")."
							  										WHERE tpaid='".$tpaid."'";
							  											
							  										$db->executar($sql);
		}
	}

	$db->commit();

	if($dados['catid']) {

		foreach($dados['catid'] as $tpaid => $arr) {
			
			foreach($arr as $catid) {
		
				$sql = "SELECT actid as id_aprendizagem FROM sispacto3.aprendizagemconhecimentoturma2 WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
				$id_aprendizagem = $db->pegaUm($sql);

				if($id_aprendizagem) {
					$sql_e[] = "UPDATE sispacto3.aprendizagemconhecimentoturma2 SET actsim=".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").",
							actparcialmente=".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").",
																			   actnao=".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")."
							WHERE actid='".$id_aprendizagem."';";
					
				} else {
						
					$sql = "SELECT count(*) as qtd FROM sispacto3.aprendizagemconhecimentoturma2 WHERE tpaid='".$tpaid."' AND catid='".$catid."'";
					$qtd = $db->pegaUm($sql);
						
					if(!$qtd) {

						$sql_e[] = "INSERT INTO sispacto3.aprendizagemconhecimentoturma2(
			            		catid, tpaid, actsim, actparcialmente, actnao)
								SELECT '".$catid."',
									   '".$tpaid."',
									   ".(($dados['actsim'][$tpaid][$catid])?"'".$dados['actsim'][$tpaid][$catid]."'":"NULL").",
									   ".(($dados['actparcialmente'][$tpaid][$catid])?"'".$dados['actparcialmente'][$tpaid][$catid]."'":"NULL").",
									   ".(($dados['actnao'][$tpaid][$catid])?"'".$dados['actnao'][$tpaid][$catid]."'":"NULL")."
									   		FROM coalesce((SELECT actid::text FROM sispacto3.aprendizagemconhecimentoturma2 WHERE catid='".$catid."' AND tpaid='".$tpaid."'),NULL) as foo WHERE foo IS NULL;";


					}
						
						
				}
		
			}
		}
		
		if($sql_e) {
				
			$db->executar(implode("",$sql_e));
			$db->commit();
			
		}
		
	}

	$al = array("alert"=>"Dados gravados com sucesso","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$dados['fpbid']."&cattipo=".$dados['cattipo']);
	alertlocation($al);

}

?>