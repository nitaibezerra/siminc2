<?

function carregarProfessorAlfabetizador($dados) {
	global $db;
	
	$arr = $db->pegaLinha("SELECT n.docid, n.picsede, n.picid, u.uncid, su.uniuf, su.unisigla||' - '||su.uninome||' >> '||su2.unisigla||' - '||su2.uninome as descricao 
						   FROM sisindigena.nucleouniversidade n  
						   INNER JOIN sisindigena.universidadecadastro u ON u.uncid = n.uncid  
					 	   INNER JOIN sisindigena.universidade su 		 ON su.uniid = u.uniid 
					 	   INNER JOIN sisindigena.universidade su2       ON su2.uniid = n.uniid 
					 	   INNER JOIN sisindigena.identificacaousuario i ON i.picid = n.picid 
					 	   WHERE i.iusd = '".$dados['iusd']."'");
		
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sisindigena.identificacaousuario i 
							   INNER JOIN sisindigena.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'");
	$_SESSION['sisindigena']['professoralfabetizador'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
															"curid" 	=> $arr['curid'], 
															"uncid" 	=> $arr['uncid'], 
															"reiid" 	=> $arr['reiid'], 
															"estuf" 	=> $arr['uniuf'], 
															"picid" 	=> $arr['picid'],
															"docid" 	=> $arr['docid'], 
															"iusd" 	   	=> $infprof['iusd'],
															"iuscpf"    => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sisindigena.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function gerenciarMateriaisProfessores($dados) {
	global $db;
	
	$sql = "SELECT mapid FROM sisindigena.materiaisprofessores WHERE iusd='".$_SESSION['sisindigena']['professoralfabetizador']['iusd']."'";
	$mapid = $db->pegaUm($sql);
	
	if($mapid) {
		
		$sql = "UPDATE sisindigena.materiaisprofessores
   			SET recebeumaterialpacto='".$dados['recebeumaterialpacto']."', 
       			recebeumaterialpnld='".$dados['recebeumaterialpnld']."', 
       			recebeulivrospnld='".$dados['recebeulivrospnld']."', 
       			recebeumaterialpnbe='".$dados['recebeumaterialpnbe']."', 
       			criadocantinholeitura='".$dados['criadocantinholeitura']."'
 			WHERE mapid='".$mapid."'";
		
		$db->executar($sql);
		
		
	} else {
		$sql = "INSERT INTO sisindigena.materiaisprofessores(
	            iusd, recebeumaterialpacto,  
	            recebeumaterialpnld, recebeulivrospnld, recebeumaterialpnbe, 
	            criadocantinholeitura, mapstatus)
	    VALUES ('".$_SESSION['sisindigena']['professoralfabetizador']['iusd']."', 
	    		'".$dados['recebeumaterialpacto']."', 
	    		'".$dados['recebeumaterialpnld']."', 
	    		'".$dados['recebeulivrospnld']."', 
	    		'".$dados['recebeumaterialpnbe']."', 
	            '".$dados['criadocantinholeitura']."', 'A') RETURNING mapid;";
		
		$mapid = $db->pegaUm($sql);
		
	}
	
	$db->commit();
	
	if($_FILES['arquivo']['error']=='0') {
		$campos	= array("mapid"	 => $mapid,
						"mpfdsc" => "'".$dados['mpfdsc']."'");	
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file = new FilesSimec("materiaisprofessoresfotos", $campos ,"sisindigena");
				
		$arquivoSalvo = $file->setUpload($dados['mafdsc']);
	}
	
	
	$al = array("alert"=>"Informaчѕes sobre materiais salvas com sucesso","location"=>"sisindigena.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=materiais");
	alertlocation($al);
	
}

function desvincularTurmaProfessor($dados) {
	global $db;
	
	$sql = "UPDATE sisindigena.turmasprofessoresalfabetizadores SET tpastatus='I', tpajustificativadesvinculacao='".$dados['tpajustificativadesvinculacao']."' WHERE tpaid='".$dados['tpaid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Desvinculaчуo feita com sucesso","location"=>"sisindigena.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
}


function pegarTurmasProfessores($dados) {
	global $db;
	
	if($dados['iusd']) $wh[] = "t.iusd='".$dados['iusd']."'";
	if($dados['tpastatus']) $wh[] = "t.tpastatus='".$dados['tpastatus']."'";
	if($dados['tpaconfirmaregencianulo']) $wh[] = "t.tpaconfirmaregencia IS NULL";
	if($dados['tpaconfirmaregencia']) $wh[] = "t.tpaconfirmaregencia=".$dados['tpaconfirmaregencia'];
	
	$turmasprofessores = $db->carregar("SELECT * 
										FROM sisindigena.turmasprofessoresalfabetizadores t 
										INNER JOIN territorios.municipio m ON m.muncod = t.tpamuncodescola 
										".(($wh)?"WHERE ".implode(" AND ",$wh):""));
	
	return $turmasprofessores;
}

function carregarEscolasPorMunicipio($dados) {
	global $db;
	
	$sql = "SELECT pk_cod_entidade as codigo, pk_cod_entidade || ' - ' || no_entidade as descricao FROM educacenso_2012.tab_entidade WHERE fk_cod_municipio='".$dados['muncod']."' ORDER BY no_entidade";
	$combo = $db->monta_combo('tpacodigoescola', $sql, 'S', 'Selecione', 'exibirDadosTurma', '', '', '200', 'S', 'tpacodigoescola', '', '');
	
}

function confirmarRegenciaTurma($dados) {
	global $db;
	if($dados['tpaconfirmaregencia']) {
		foreach($dados['tpaconfirmaregencia'] as $tpaid => $vl) {
			$db->executar("UPDATE sisindigena.turmasprofessoresalfabetizadores SET tpaconfirmaregencia=".$vl." WHERE tpaid='".$tpaid."'");
			$db->commit();			
		}
	}
	
	$al = array("alert"=>"Confirmaчуo de regъncia feita com sucesso","location"=>"sisindigena.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
	
}

function inserirTurmaProfessor($dados) {
	global $db;
	
	$sql = "INSERT INTO sisindigena.turmasprofessoresalfabetizadores(
            tpacodigoescola, tpanomeescola, tpamuncodescola, tpaemailescola, 
            tpanometurma, tpahorarioinicioturma, tpahorariofimturma, 
            iusd, tpastatus, tpaoriginalcenso, tpaetapaturma, tpaconfirmaregencia)
            SELECT pk_cod_entidade, no_entidade, fk_cod_municipio, no_email,
            	   '".$dados['tpanometurma']."', '".$dados['tpahorarioinicioturma_hr'].":".$dados['tpahorarioinicioturma_mi']."', '".$dados['tpahorariofimturma_hr'].":".$dados['tpahorariofimturma_mi']."',
            	   '".$_SESSION['sisindigena']['professoralfabetizador']['iusd']."', 'A', false, (SELECT no_etapa_ensino FROM educacenso_2012.tab_etapa_ensino WHERE pk_cod_etapa_ensino='".$dados['pk_cod_etapa_ensino']."'), 
            	   true
            FROM educacenso_2012.tab_entidade WHERE pk_cod_entidade='".$dados['tpacodigoescola']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Turma do professor inserida com sucesso","location"=>"sisindigena.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
}

function gerenciarInformacoesTurmasProfessor($dados) {
	global $db;
	
	if($dados['tpaid']) {
		foreach($dados['tpaid'] as $tpaid) { 
			$sql = "UPDATE sisindigena.turmasprofessoresalfabetizadores SET 
					  tpatotalmeninos=".(($dados['tpatotalmeninos'][$tpaid])?"'".$dados['tpatotalmeninos'][$tpaid]."'":"NULL").",
					  tpatotalmeninas=".(($dados['tpatotalmeninas'][$tpaid])?"'".$dados['tpatotalmeninas'][$tpaid]."'":"NULL").",
					  tpafaixaetaria6anos=".(($dados['tpafaixaetaria6anos'][$tpaid])?"'".$dados['tpafaixaetaria6anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria7anos=".(($dados['tpafaixaetaria7anos'][$tpaid])?"'".$dados['tpafaixaetaria7anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria8anos=".(($dados['tpafaixaetaria8anos'][$tpaid])?"'".$dados['tpafaixaetaria8anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria9anos=".(($dados['tpafaixaetaria9anos'][$tpaid])?"'".$dados['tpafaixaetaria9anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria10anos=".(($dados['tpafaixaetaria10anos'][$tpaid])?"'".$dados['tpafaixaetaria10anos'][$tpaid]."'":"NULL").",
					  tpafaixaetaria11anos=".(($dados['tpafaixaetaria11anos'][$tpaid])?"'".$dados['tpafaixaetaria11anos'][$tpaid]."'":"NULL").",
					  tpatotalfreqeducinfantil=".(($dados['tpatotalfreqeducinfantil'][$tpaid])?"'".$dados['tpatotalfreqeducinfantil'][$tpaid]."'":"NULL").",
					  tpatotalbolsafamilia=".(($dados['tpatotalbolsafamilia'][$tpaid])?"'".$dados['tpatotalbolsafamilia'][$tpaid]."'":"NULL").",
					  tpatotalvivemcomunidade=".(($dados['tpatotalvivemcomunidade'][$tpaid])?"'".$dados['tpatotalvivemcomunidade'][$tpaid]."'":"NULL").",
					  tpatotalfreqcreche=".(($dados['tpatotalfreqcreche'][$tpaid])?"'".$dados['tpatotalfreqcreche'][$tpaid]."'":"NULL").",
					  tpatotalfreqpreescola=".(($dados['tpatotalfreqpreescola'][$tpaid])?"'".$dados['tpatotalfreqpreescola'][$tpaid]."'":"NULL")." 
					WHERE tpaid='".$tpaid."'";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	$al = array("alert"=>"Informaчѕes das Turmas gravadas com sucesso","location"=>"sisindigena.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
	
	
}
?>