<?

function condicaoRetornarIES() {
	global $db;
	
	$sql = "SELECT jtoid FROM sispacto2.justificativatrocaorientador WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND jtostatus='I'";
	$jtoids = $db->carregarColuna($sql);
	
	if(count($jtoids) > 0) {
		return "Existem justificativas cadastradas para Orientadores de Estudo, caso queira retornar para análise da IES remova as justificativas";		
	}
	
	return true;
	
}

function removerProfessorAlfabetizador($dados) {
	global $db;
	
	$sql = "SELECT pboid FROM sispacto2.pagamentobolsista WHERE iusd='".$dados['iusd']."'";
	$pboid = $db->pegaUm($sql);
	
	if($pboid) {
		echo "Não é possível remover o Professor Alfabetizador, pois este ja recebeu uma Bolsa de Estudo pelo SISPACTO. Somente será permitido substituições.";
		exit;		 
	}
	
	$sql = "DELETE FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusd']."' AND pflcod='".PFL_PROFESSORALFABETIZADOR."'";
	$db->executar($sql);
	$sql = "DELETE FROM sispacto2.orientadorturma WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$sql = "UPDATE sispacto2.identificacaousuario SET iusstatus='I' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();
	
}


function inserirProfessoresAlfabetizadores($dados) {
	global $db;
	
	// validação da sessão
	if(!$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'])
		die("<script>alert('As informações não foram gravadas. Houve perdas de informações internas. Você será direcionado para a tela principal.');window.location='sispacto2.php?modulo=inicio&acao=C';</script>");
		
	$uncid = $db->pegaUm("SELECT i.uncid FROM sispacto2.turmas t INNER JOIN sispacto2.identificacaousuario i ON i.iusd=t.iusd WHERE t.turid='".$dados['turid']."'");
	
	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
			
			unset($erro);
			if(!$dados['nome'][$cpf]) $erro[] = "Nome em branco"; 
			if(!$dados['email'][$cpf]) $erro[] = "Email em branco";
			if(!$dados['serie'][$cpf]) $erro[] = "Série em branco";
			
			if($erro) {
				$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erro),"location"=>$dados['goto']);
				alertlocation($al);
			}
			
			$palid = $db->pegaUm("SELECT palid FROM sispacto2.professoresalfabetizadores WHERE cpf='".str_replace(array(".","-"),array(""),$cpf)."'");
			if($palid) $dados['tipo'][$cpf] = 'censo';
			
			$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'");
			
			if($iusd) {
				
				$sql = "UPDATE sispacto2.identificacaousuario SET 
						picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."',
						".(($uncid)?"uncid='".$uncid."',":"")."
						iusnome='".$dados['nome'][$cpf]."', 
						iusemailprincipal='".$dados['email'][$cpf]."', 
						iusserieprofessor='".$dados['serie'][$cpf]."',
						iustipoprofessor='".$dados['tipo'][$cpf]."',
						iusstatus='A'
						WHERE iusd='".$iusd."'";
				$db->executar($sql);
				
			} else {
			
				$sql = "INSERT INTO sispacto2.identificacaousuario(
			            picid, iuscpf, iusnome, iusemailprincipal, iusserieprofessor, iusdatainclusao, uncid, iustipoprofessor)
					    VALUES ('".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
					    		'".str_replace(array(".","-"),array(""),$cpf)."', 
					    		'".$dados['nome'][$cpf]."', 
					    		'".$dados['email'][$cpf]."',
					    		'".$dados['serie'][$cpf]."',
					    		NOW(),
					    		".(($uncid)?"'".$uncid."'":"NULL").",
					    		'".$dados['tipo'][$cpf]."') RETURNING iusd;";
				
				$iusd = $db->pegaUm($sql);
			
			}
			
			$sql = "SELECT p.pfldsc, CASE WHEN pa.muncod IS NOT NULL THEN mu.estuf||' / '||mu.mundescricao
										  WHEN pa.estuf IS NOT NULL THEN es.estuf||' / '||es.estdescricao END  as descricao FROM sispacto2.tipoperfil t
					INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
					INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd 
					INNER JOIN sispacto2.pactoidadecerta pa ON pa.picid = i.picid 
					LEFT JOIN territorios.municipio mu ON mu.muncod = pa.muncod 
					LEFT JOIN territorios.estado es ON es.estuf = pa.estuf
					WHERE t.iusd='".$iusd."'";
			$arr = $db->pegaLinha($sql);
			
			$pfldsc    = $arr['pfldsc'];
			$descricao = $arr['descricao'];
			
			if($pfldsc) {
				
				$al = array("alert"=>"Caso queira indicar este CPF(".$dados['nome'][$cpf].") como Professor Alfabetizador, é necessário antes remove-lo do perfil(".$pfldsc.",".$descricao.").","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
				alertlocation($al);
			}
			
			$sql = "INSERT INTO sispacto2.tipoperfil(iusd, pflcod, tpestatus)
    				VALUES ('".$iusd."', '".PFL_PROFESSORALFABETIZADOR."', 'A');";
			$db->executar($sql);
			
			$existe_turma = $db->pegaUm("SELECT otuid FROM sispacto2.orientadorturma WHERE iusd='{$iusd}'");
			
			if($existe_turma) {
				
				$sql = "UPDATE sispacto2.orientadorturma SET turid='".$dados['turid']."' WHERE iusd='{$iusd}'";
				$db->executar($sql);
				
			} else {
			
				$sql = "INSERT INTO sispacto2.orientadorturma(
	            		turid, iusd, otustatus)
	    				VALUES ('".$dados['turid']."', '".$iusd."', 'A');";
				
				$db->executar($sql);
			
			}
			
		}
	}
	
	if($_FILES['anexoportaria']) {
		foreach($_FILES['anexoportaria']['error'] as $cpf => $erro) {
		
			if($erro == 0) {
				
				$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array("",""),$cpf)."'");
				
				$_FILES['anexoportaria_tmp'] = array("name"     => $_FILES['anexoportaria']['name'][$cpf],
													 "type"     => $_FILES['anexoportaria']['type'][$cpf],
													 "tmp_name" => $_FILES['anexoportaria']['tmp_name'][$cpf],
													 "error" 	=> $_FILES['anexoportaria']['error'][$cpf],
													 "size" 	=> $_FILES['anexoportaria']['size'][$cpf]);
		
				
		    	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		        $campos = array("iusd" => "'".$iusd."'");
		
		        $file = new FilesSimec( "portarianomeacao", $campos, "sispacto2" );
		        $file->setUpload( NULL, "anexoportaria_tmp" );
			}
		}
		
	}
	
	// validando o número de professores alfabetizadores não ultrapassou o previsto
	
	$ar = array("estuf" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf'],
				"muncod" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod'],
				"dependencia" => $_SESSION['sispacto2']['esfera']);
	
	$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
	$professoesalfabetizados = carregarDadosIdentificacaoUsuario(array("picid" => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'],"pflcod" => PFL_PROFESSORALFABETIZADOR));
	
	if(count($professoesalfabetizados) > $totalalfabetizadores['total']) {
		$db->rollback();
		
		$al = array("alert"=>"Não foi possível salvar. Professores Alfabetizadores ultrapassaram o número previsto de ".$totalalfabetizadores['total'],"location"=>$dados['goto']);	
	} else {
		$db->commit();
		
		$al = array("alert"=>"Professores Alfabetizadores gravados com sucesso.","location"=>$dados['goto']);
	}
	
	alertlocation($al);
	
}


function inserirOrientadoresEstudo($dados) {
	global $db;
	
	// validação da sessão
	if(!$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'])
		die("<script>alert('As informações não foram gravadas. Houve perdas de informações internas. Você será direcionado para a tela principal.');window.location='sispacto2.php?modulo=inicio&acao=C';</script>");
	
	if($dados['picincluirprofessorrede']=="TRUE") {
		
		$aliid = $db->pegaUm("SELECT aliid FROM sispacto2.alfabetizadoresindicados WHERE alimuncodorigem='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod']."'");
		
		if($aliid) {
			
			$sql = "UPDATE sispacto2.alfabetizadoresindicados SET aliestufdestino='".$dados['aliestufdestino']."' WHERE aliid='".$aliid."'";
			$db->executar($sql);
			
		} else {
			
			$ar = array("estuf" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf'],
						"muncod" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod'],
						"dependencia" => $_SESSION['sispacto2']['esfera']);
			
			$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
		
			$sql = "INSERT INTO sispacto2.alfabetizadoresindicados(
	            	alimuncodorigem, aliestufdestino, aliquantidade, alistatus)
	    			VALUES ('".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod']."', 
	    					'".$dados['aliestufdestino']."', '".$totalalfabetizadores['total']."', 'P');";
			
			$db->executar($sql);
		
		}
		
	} elseif($dados['picincluirprofessorrede']=="FALSE") {
		$db->executar("DELETE FROM sispacto2.alfabetizadoresindicados WHERE alimuncodorigem='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod']."'");
	}
	
	
	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
			
			$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'");
			
			if($iusd) {
				
				$sql = "UPDATE sispacto2.identificacaousuario SET 
						picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
						iusnome='".$dados['nome'][$cpf]."', 
						iusemailprincipal='".$dados['email'][$cpf]."', 
						iustipoorientador='".$dados['tipo'][$cpf]."',
						".(($dados['muncodatuacao'][$cpf])?"muncodatuacao='".$dados['muncodatuacao'][$cpf]."', ":"")."
						iusstatus='A'
						WHERE iusd='".$iusd."'";
				$db->executar($sql);
				
			} else {
			
				$sql = "INSERT INTO sispacto2.identificacaousuario(
			            picid, iuscpf, iusnome, iusemailprincipal, iustipoorientador, iusdatainclusao, muncodatuacao)
					    VALUES ('".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
					    		'".str_replace(array(".","-"),array(""),$cpf)."', 
					    		'".$dados['nome'][$cpf]."', 
					    		'".$dados['email'][$cpf]."',
					    		'".$dados['tipo'][$cpf]."',
					    		NOW(),
					    		'".$dados['muncodatuacao'][$cpf]."') RETURNING iusd;";
				
				$iusd = $db->pegaUm($sql);
			
			}
			
			$sql = "SELECT p.pfldsc, CASE WHEN pa.muncod IS NOT NULL THEN mu.estuf||' / '||mu.mundescricao
										  WHEN pa.estuf IS NOT NULL THEN es.estuf||' / '||es.estdescricao END  as descricao FROM sispacto2.tipoperfil t
					INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
					INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd 
					INNER JOIN sispacto2.pactoidadecerta pa ON pa.picid = i.picid 
					LEFT JOIN territorios.municipio mu ON mu.muncod = pa.muncod 
					LEFT JOIN territorios.estado es ON es.estuf = pa.estuf
					WHERE t.iusd='".$iusd."'";
			$arr = $db->pegaLinha($sql);
			
			$pfldsc    = $arr['pfldsc'];
			$descricao = $arr['descricao'];
			
			if($pfldsc) {
				
				$al = array("alert"=>"Caso queira indicar este CPF(".$dados['nome'][$cpf].") como Orientador de Estudo, é necessário antes remove-lo do perfil(".$pfldsc.",".$descricao.").","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
				alertlocation($al);
			}
			
			$sql = "INSERT INTO sispacto2.tipoperfil(iusd, pflcod, tpestatus)
    				VALUES ('".$iusd."', '".PFL_ORIENTADORESTUDO."', 'A');";
			$db->executar($sql);
		}
	}
	
	if($_FILES['anexoportaria']) {
		foreach($_FILES['anexoportaria']['error'] as $cpf => $erro) {
		
			if($erro == 0) {
				
				$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array("",""),$cpf)."'");
				
				$_FILES['anexoportaria_tmp'] = array("name"     => $_FILES['anexoportaria']['name'][$cpf],
													 "type"     => $_FILES['anexoportaria']['type'][$cpf],
													 "tmp_name" => $_FILES['anexoportaria']['tmp_name'][$cpf],
													 "error" 	=> $_FILES['anexoportaria']['error'][$cpf],
													 "size" 	=> $_FILES['anexoportaria']['size'][$cpf]);
		
				
		    	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		        $campos = array("iusd" => "'".$iusd."'");
		
		        $file = new FilesSimec( "portarianomeacao", $campos, "sispacto2" );
		        $file->setUpload( NULL, "anexoportaria_tmp" );
			}
		}
		
	}
	
	$db->commit();
	
	if($dados['picselecaopublica']=="FALSE") $msg = "Para indicar os Orientadores de Estudo é necessário realizar um processo de seleção interna segundo critérios técnicos e objetivos.";
	else $msg = "Orientadores de Estudo gravados com sucesso.";
	
	$al = array("alert"=>$msg,"location"=>$dados['goto']);
	alertlocation($al);
	
	
}

function carregarCoordenadorLocal($dados) {
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	if($dados['muncod']) { // se tiver muncod, simular coordenador municipal
		unset($_SESSION['sispacto2']['coordenadorlocal']['estadual']);
		$arr = $db->pegaLinha("SELECT p.picid, p.docid, m.estuf || ' - ' || m.mundescricao as descricao, p.docidturma FROM sispacto2.pactoidadecerta p INNER JOIN territorios.municipio m ON m.muncod=p.muncod WHERE p.muncod='".$dados['muncod']."'");
		$picid 	    = $arr['picid'];
		$docid 	    = $arr['docid'];
		$docidturma = $arr['docidturma'];
		$descricao  = $arr['descricao'];
		
		if($picid) {
			
			if(in_array(PFL_COORDENADORLOCAL,$perfis)) {
			
				$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.uncid, i.iuscpf, i.iusdesativado 
								   	   FROM sispacto2.identificacaousuario i 
								   	   INNER JOIN sispacto2.tipoperfil t ON t.iusd=i.iusd 
								   	   WHERE i.picid='".$picid."' AND t.pflcod='".PFL_COORDENADORLOCAL."' AND i.iuscpf='".$_SESSION['usucpf']."'");
				
			} else {
				
				if(!$dados['iusd']) $dados['iusd'] = $_SESSION['sispacto2']['coordenadorlocal']['iusd'];

				$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.uncid, i.iuscpf, i.iusdesativado 
								   	   FROM sispacto2.identificacaousuario i 
								   	   INNER JOIN sispacto2.tipoperfil t ON t.iusd=i.iusd 
								   	   WHERE i.picid='".$picid."' AND t.pflcod='".PFL_COORDENADORLOCAL."' ".(($dados['iusd'])?" AND i.iusd='".$dados['iusd']."'":" ORDER BY RANDOM()"));
				
			}
			
			$descricao .= " ( ".$infprof['iusnome']." )";
			
			if(!$docid) {
				$docid = wf_cadastrarDocumento(TPD_ORIENTADORESTUDO,"Sispacto_CoordenadorLocal_estuf_".$dados['estuf']."_muncod_".$dados['muncod']);
				$db->executar("UPDATE sispacto2.pactoidadecerta SET docid='".$docid."' WHERE picid='".$picid."'");
				$db->commit();
			}
			
			if(!$docidturma) {
				$docidturma = wf_cadastrarDocumento(TPD_FLUXOTURMA,"Sispacto_CoordenadorLocal_turma_estuf_".$dados['estuf']."_muncod_".$dados['muncod']);
				$db->executar("UPDATE sispacto2.pactoidadecerta SET docidturma='".$docidturma."' WHERE picid='".$picid."'");
				$db->commit();
			}
			
			unset($_SESSION['sispacto2']['coordenadorlocal']['municipal']);
			$_SESSION['sispacto2']['esfera'] = 'municipal';
			$_SESSION['sispacto2']['coordenadorlocal']['iusd'] = $infprof['iusd'];
			$_SESSION['sispacto2']['coordenadorlocal']['iusdesativado'] = $infprof['iusdesativado'];
			$_SESSION['sispacto2']['coordenadorlocal']['uncid'] = $infprof['uncid'];
			$_SESSION['sispacto2']['coordenadorlocal']['iuscpf'] = $infprof['iuscpf'];
			$_SESSION['sispacto2']['coordenadorlocal']['municipal'] = array("estuf" => $dados['estuf'],"muncod" => $dados['muncod'],"picid" => $picid,"docid" => $docid,"descricao" => $descricao,"iusd" => $infprof['iusd'],"docidturma" => $docidturma);
		}	
	}elseif($dados['estuf']) { // senão se tiver estuf, simular coordenador estadual
		unset($_SESSION['sispacto2']['coordenadorlocal']['municipal']);
		$arr = $db->pegaLinha("SELECT p.picid, p.docid, e.estuf || ' - ' || e.estdescricao as descricao, p.docidturma FROM sispacto2.pactoidadecerta p INNER JOIN territorios.estado e ON e.estuf=p.estuf WHERE p.estuf='".$dados['estuf']."'");
		$picid 	    = $arr['picid'];
		$docid 	    = $arr['docid'];
		$docidturma = $arr['docidturma'];
		$descricao  = $arr['descricao'];
		
		if($picid) {
			
			
			if(in_array(PFL_COORDENADORLOCAL,$perfis)) {
			
				$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.uncid, i.iuscpf, i.iusdesativado 
								   	   FROM sispacto2.identificacaousuario i 
								   	   INNER JOIN sispacto2.tipoperfil t ON t.iusd=i.iusd 
								   	   WHERE i.picid='".$picid."' AND t.pflcod='".PFL_COORDENADORLOCAL."' AND i.iuscpf='".$_SESSION['usucpf']."'");
				
			} else {
				
				if(!$dados['iusd']) $dados['iusd'] = $_SESSION['sispacto2']['coordenadorlocal']['iusd'];

				$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.uncid, i.iuscpf, i.iusdesativado 
									   	   FROM sispacto2.identificacaousuario i 
									   	   INNER JOIN sispacto2.tipoperfil t ON t.iusd=i.iusd 
									   	   WHERE i.picid='".$picid."' AND t.pflcod='".PFL_COORDENADORLOCAL."' ".(($dados['iusd'])?" AND i.iusd='".$dados['iusd']."'":" ORDER BY RANDOM()"));
				
			}
			
			$descricao .= " ( ".$infprof['iusnome']." )";
			
			
			if(!$docid) {
				$docid = wf_cadastrarDocumento(TPD_ORIENTADORESTUDO,"Sispacto_CoordenadorLocal_estuf_".$dados['estuf']);
				$db->executar("UPDATE sispacto2.pactoidadecerta SET docid='".$docid."' WHERE picid='".$picid."'");
				$db->commit();
			}
			
			if(!$docidturma) {
				$docidturma = wf_cadastrarDocumento(TPD_FLUXOTURMA,"Sispacto_CoordenadorLocal_turma_estuf_".$dados['estuf']);
				$db->executar("UPDATE sispacto2.pactoidadecerta SET docidturma='".$docidturma."' WHERE picid='".$picid."'");
				$db->commit();
			}
			
			unset($_SESSION['sispacto2']['coordenadorlocal']['estadual']);
			$_SESSION['sispacto2']['esfera'] = 'estadual';
			$_SESSION['sispacto2']['coordenadorlocal']['iusd']  = $infprof['iusd'];
			$_SESSION['sispacto2']['coordenadorlocal']['iusdesativado'] = $infprof['iusdesativado'];
			$_SESSION['sispacto2']['coordenadorlocal']['uncid'] = $infprof['uncid'];
			$_SESSION['sispacto2']['coordenadorlocal']['iuscpf'] = $infprof['iuscpf'];
			$_SESSION['sispacto2']['coordenadorlocal']['estadual'] = array("estuf" => $dados['estuf'],"picid" => $picid,"docid" => $docid,"descricao" => $descricao,"iusd" => $infprof['iusd'],"docidturma" => $docidturma);
		}	
	}
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=principal");
		alertlocation($al);
	}
	
}


function removerOrientadorEstudo($dados) {
	global $db;
	
	$sql = "SELECT tpeid FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$tpeid = $db->pegaUm($sql);
	
	
	$sql = "SELECT pboid FROM sispacto2.pagamentobolsista WHERE iusd='".$dados['iusd']."'";
	$pboid = $db->pegaUm($sql);
	
	if($pboid) {
		echo "Não é possível remover o Orientador de Estudo, pois este ja recebeu uma Bolsa de Estudo pelo SISPACTO. Somente será permitido substituições.";
		exit;		 
	}
	
	$sql = "SELECT i.iusnome || '( R$ ' || p.pbovlrpagamento || ' )' as pagamentovaga FROM sispacto2.pagamentobolsista p 
	 		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd 
	 		WHERE tpeid='".$tpeid."'";
	
	$pagamentovaga = $db->carregarColuna($sql);
	
	if($pagamentovaga) {
		echo "Não é possível remover o Orientador de Estudo. Para essa vaga ja foi realizado pagamentos : ".implode('\n',$pagamentovaga);
		exit;		 
	}
	
	// removendo professores 
	
	$iusds = $db->carregarColuna("SELECT i2.iusd FROM sispacto2.identificacaousuario i 
								 INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO."
								 INNER JOIN sispacto2.turmas tu ON tu.iusd = i.iusd 
								 INNER JOIN sispacto2.orientadorturma ot ON ot.turid = tu.turid 
								 INNER JOIN sispacto2.identificacaousuario i2 ON i2.iusd = ot.iusd 
								 INNER JOIN sispacto2.tipoperfil t2 ON t2.iusd = i2.iusd AND t2.pflcod=".PFL_PROFESSORALFABETIZADOR."
								 WHERE i.iusd='".$dados['iusd']."'");
	
	if($iusds) {
		$sql = "UPDATE sispacto2.identificacaousuario SET iusstatus='I' WHERE iusd IN('".implode("','",$iusds)."')";
		$db->executar($sql);
		
		$sql = "DELETE FROM sispacto2.tipoperfil WHERE iusd IN('".implode("','",$iusds)."')";
		$db->executar($sql);
		
		$sql = "DELETE FROM sispacto2.orientadorturma WHERE iusd IN('".implode("','",$iusds)."')";
		$db->executar($sql);
	}
	
	// removendo professores
	
	$sql = "DELETE FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusd']."' AND pflcod='".PFL_ORIENTADORESTUDO."'";
	$db->executar($sql);
	
	$usucpf = $db->pegaUm("SELECT iuscpf FROM sispacto2.identificacaousuario WHERE iusd='".$dados['iusd']."'");
	
	if($usucpf) {
		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$usucpf."' AND pflcod='".PFL_ORIENTADORESTUDO."'";
		$db->executar($sql);
	}
	
	$sql = "UPDATE sispacto2.identificacaousuario SET iusstatus='I' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM sispacto2.orientadorturma WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$db->commit();
	
	

	
	echo "TRUE";
	
}

function calculaPorcentagemCadastroOrientadores($dados) {
	global $db;
	
	$orientadores = carregarDadosIdentificacaoUsuario(array("picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));
	
	$ar = array("estuf" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf'],
				"muncod" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod'],
				"dependencia" => $_SESSION['sispacto2']['esfera']);
	
	$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
	
	$numero_orientadores  = count($orientadores);
	$numero_total_a_serem = $totalalfabetizadores['total_orientadores_a_serem_cadastrados'];
	
	if($numero_total_a_serem) $apassituacao = round(($numero_orientadores/$numero_total_a_serem)*100);
	else $apassituacao = 0;
	$apadatainicio = $orientadores[0]['iusdatainclusao'];
	$apadatafim = $orientadores[count($orientadores)-1]['iusdatainclusao'];
	
	gerenciarAtividadePacto(array('apadatafim'=>$apadatafim,'apadatainicio'=>$apadatainicio,'apassituacao'=>$apassituacao,'suaid'=>$dados['suaid'],'picid'=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']));
	
}

function calculaPorcentagemCadastroProfessores($dados) {
	global $db;
	
	$professores = carregarDadosIdentificacaoUsuario(array("picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'],"pflcod"=>PFL_PROFESSORALFABETIZADOR));
	
	$ar = array("estuf" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf'],
				"muncod" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod'],
				"dependencia" => $_SESSION['sispacto2']['esfera']);
	
	$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
	
	$numero_professores  = count($professores);
	$numero_total_a_serem = $totalalfabetizadores['total'];
	
	if($numero_total_a_serem) $apassituacao = round(($numero_professores/$numero_total_a_serem)*100);
	else $apassituacao = 0;
	
	$apadatainicio = $professores[0]['iusdatainclusao'];
	$apadatafim = $professores[count($orientadores)-1]['iusdatainclusao'];
	
	$sql = "SELECT apaid FROM sispacto2.atividadepacto WHERE suaid='".$dados['suaid']."' AND picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";
	$apaid = $db->pegaUm($sql);
	
	if($apaid) {
		
		$sql = "UPDATE sispacto2.atividadepacto SET 
				apassituacao=".(($apassituacao)?"'".$apassituacao."'":"NULL").", 
				apadatainicio=".(($apadatainicio)?"'".$apadatainicio."'":"NULL").",
				apadatafim=".(($apadatafim)?"'".$apadatafim."'":"NULL")."
			    WHERE apaid='".$apaid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.atividadepacto(
	            suaid, picid, apassituacao, apadatainicio, apadatafim, 
	            apastatus)
			    VALUES ('".$dados['suaid']."', '".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
			    		".(($apassituacao)?"'".$apassituacao."'":"NULL").", 
			    		".(($apadatainicio)?"'".$apadatainicio."'":"NULL").", 
			    		".(($apadatafim)?"'".$apadatafim."'":"NULL").", 'A');";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	
}

function calculaPorcentagemAutorizarSubstitutos($dados) {
	global $db;
	if($_SESSION['sispacto2']['esfera']=='municipal') {
		$f = "fioesfera='M' AND muncod='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod']."'";
	} elseif($_SESSION['sispacto2']['esfera']=='estadual') {
		$f = "fioesfera='E' AND estuf='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf']."'";
	}
	
	$sql = "SELECT COUNT(*) as tot, MAX(fiodata) as apadatafim, MIN(fiodata) as apadatainicio FROM sispacto2.formacaoinicialouvintes WHERE ".$f;
	$arrfio = $db->pegaLinha($sql);
	
	$total 		   = $arrfio['tot'];
	$apadatainicio = $arrfio['apadatainicio'];
	$apadatafim    = $arrfio['apadatafim'];
	
	$sql = "SELECT COUNT(*) as tot FROM sispacto2.formacaoinicialouvintes WHERE fiostatus='I' AND ".$f;
	$total_autorizados = $db->pegaUm($sql);
	
	if($total > 0) $apassituacao = round(($total_autorizados/$total)*100);
	else $apassituacao = 100;
	
	$sql = "SELECT apaid FROM sispacto2.atividadepacto WHERE suaid='".$dados['suaid']."' AND picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";
	$apaid = $db->pegaUm($sql);
	
	if($apaid) {
		
		$sql = "UPDATE sispacto2.atividadepacto SET 
				apassituacao=".(($apassituacao)?"'".$apassituacao."'":"NULL").", 
				apadatainicio=".(($apadatainicio)?"'".$apadatainicio."'":"NULL").",
				apadatafim=".(($apadatafim)?"'".$apadatafim."'":"NULL")."
			    WHERE apaid='".$apaid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.atividadepacto(
	            suaid, picid, apassituacao, apadatainicio, apadatafim, 
	            apastatus)
			    VALUES ('".$dados['suaid']."', '".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
			    		".(($apassituacao)?"'".$apassituacao."'":"NULL").", 
			    		".(($apadatainicio)?"'".$apadatainicio."'":"NULL").", 
			    		".(($apadatafim)?"'".$apadatafim."'":"NULL").", 'A');";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
}

function carregarTotalAlfabetizadores($dados) {
	global $db;
	
	if($dados['estuf']) {
		$f[] = "sigla='".$dados['estuf']."'";
		if($dados['dependencia']=='estadual') {
			
			$total_indicados = $db->pegaUm("SELECT SUM(aliquantidade) as o 
											FROM sispacto2.alfabetizadoresindicados 
											WHERE aliestufdestino='".$dados['estuf']."' AND alistatus='A'");
		} 
		
	}
	if($dados['muncod']) {
		$f[] = "cod_municipio='".$dados['muncod']."'";
	}
	if($dados['dependencia']) $f[] = "dependencia ilike '".$dados['dependencia']."'";
	
	$sql = "SELECT total, total_orientadores_a_serem_cadastrados FROM sispacto2.totalalfabetizadores 
			WHERE ".(($f)?implode(" AND ", $f):"");
	
	$totalalfabetizadores = $db->pegaLinha($sql);
	
	$totalalfabetizadores['total'] += $total_indicados;
	
	if(!$totalalfabetizadores['total']) $totalalfabetizadores['total']="0";
	if(!$totalalfabetizadores['total_orientadores_a_serem_cadastrados']) $totalalfabetizadores['total_orientadores_a_serem_cadastrados']="0";
	
	return $totalalfabetizadores;
	
}

function inserirJustificativas($dados) {
	global $db;
	
	if($dados['joecomentario']) {
		foreach($dados['joecomentario'] as $tipo => $comentario) {
			
			$sql = "SELECT joeid FROM sispacto2.justificativaorientadorestudo WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND joestatus='A' AND joetipo='".$tipo."'";
			$joeid = $db->pegaUm($sql);
			
			if($joeid) {
				
				$sql = "UPDATE sispacto2.justificativaorientadorestudo
   						SET joecomentario='".substr($comentario,0,250)."'
 						WHERE joeid='".$joeid."'";
				
				$db->executar($sql);
				
			} else {
				
				$sql = "INSERT INTO sispacto2.justificativaorientadorestudo(
            			picid, joecomentario, joestatus, joetipo)
    					VALUES ('".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
    							'".substr($comentario,0,250)."', 'A', '".$tipo."') RETURNING joeid;";
				
				$joeid = $db->pegaUm($sql);
				
			}
			
			$db->executar("DELETE FROM sispacto2.justicativaorientadortipo WHERE joeid='".$joeid."'");
			
			if($dados['tjuid'][$tipo]) {
				foreach($dados['tjuid'][$tipo] as $tjuid) {
					$sql = "INSERT INTO sispacto2.justicativaorientadortipo(
            				joeid, tjuid, juostatus)
    						VALUES ('".$joeid."', '".$tjuid."', 'A');";
					
					$db->executar($sql);
				}
			}
			
			$db->commit();
			
		}
	}
	
	$al = array("alert"=>"Justificativas gravadas com sucesso","location"=>$dados['goto']);
	alertlocation($al);
	
}

function carregarJustificativasRespostas($dados) {
	global $db;
	
	$sql = "SELECT joeid, joetipo, joecomentario FROM sispacto2.justificativaorientadorestudo WHERE picid='".$dados['picid']."' AND joestatus='A'";
	$justificativaorientadorestudo = $db->carregar($sql);
	
	if($justificativaorientadorestudo[0]) {
		foreach($justificativaorientadorestudo as $jo) {
			$sql = "SELECT tjuid FROM sispacto2.justicativaorientadortipo WHERE joeid='".$jo['joeid']."'";
			$tjuids = $db->carregarColuna($sql);
			$justificativaoriest[$jo['joetipo']]= array("joecomentario"=>$jo['joecomentario'],"tjuids"=>(($tjuids)?$tjuids:array()));
		}
	}
	
	return $justificativaoriest;
	
}



function inserirAnexoPrincipalCoordenadorLocal($dados) {
	global $db;
	
    if ( $_FILES['arquivo']['error'] == 0 ) {
    	
    	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $campos = array("apaid" => "'".$dados['apaid']."'",
        				"tdaid" => "'".$dados['tdaid']."'");

        $file = new FilesSimec( "documentoatividade", $campos, "sispacto2" );
        $file->setUpload( NULL, "arquivo" );

		$al = array("alert"=>"Documento gravado com sucesso","location"=>$dados['goto']);
		alertlocation($al);
        
    } else {
		$al = array("alert"=>"Documento não foi gravado com sucesso","location"=>$dados['goto']);
		alertlocation($al);
    }
	
}





function excluirDocumento($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto2.documentoatividade WHERE doaid='".$dados['doaid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Documento excluído com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/inseriranexos&acao=A&apaid=".$dados['apaid']);
	alertlocation($al);
	
}

function atualizarSelecaoPublica($dados) {
	global $db;
	
	$sql = "UPDATE sispacto2.pactoidadecerta SET picselecaopublica=".$dados['picselecaopublica']." WHERE picid='".$dados['picid']."'";
	$db->executar($sql);
	
	if($dados['picselecaopublica']=="FALSE") {
		$iusds = $db->carregarColuna("SELECT i.iusd FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid='".$dados['picid']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."'");
		if($iusds) {
			$sql = "DELETE FROM sispacto2.tipoperfil WHERE iusd in('".implode("','",$iusds)."')";
			$db->executar($sql);
			$sql = "UPDATE sispacto2.identificacaousuario SET iusstatus='I' WHERE iusd in('".implode("','",$iusds)."')";
			$db->executar($sql);
		}
		$sql = "DELETE FROM sispacto2.justicativaorientadortipo WHERE joeid IN(SELECT joeid FROM sispacto2.justificativaorientadorestudo WHERE picid='".$dados['picid']."')";
		$db->executar($sql);
		$sql = "DELETE FROM sispacto2.justificativaorientadorestudo WHERE picid='".$dados['picid']."'";
		$db->executar($sql);
		
	}
	
	$db->commit();
}

function atualizarInclusaoProfessorRede($dados) {
	global $db;
	
	$sql = "UPDATE sispacto2.pactoidadecerta SET picincluirprofessorrede=".$dados['picincluirprofessorrede']." WHERE picid='".$dados['picid']."'";
	$db->executar($sql);
	
	$db->commit();
}


function verificarPerfilOutrosMunicipios($dados) {
	global $db;
	$sql = "SELECT pp.pfldsc||' cadastrado em : '||m.estuf||' - '||m.mundescricao as texto FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			WHERE i.iuscpf='".str_replace(array(".","-"),array(""),$dados['cpf'])."' AND i.iusstatus='A' AND i.picid!='".$dados['picid']."'";
	$texto = $db->pegaUm($sql);
	echo (($texto)?$texto:"");
}

function validarEnvioAnaliseIES() {
	global $db;
	
	if(!$_SESSION['sispacto2']['coordenadorlocal']['naoValidarEnvioAnaliseIES']) {
		
		$perfis = pegaPerfilGeral();
		
		if(in_array(PFL_EQUIPEMEC,$perfis) || $db->testa_superuser()) {
			return true;
		}
		
		$orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));
		
		$ar = array("estuf" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf'],
					"muncod" 	  => $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod'],
					"dependencia" => $_SESSION['sispacto2']['esfera']);
		
		$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
		
		if(count($orientadoresestudo) == 0) {
			$erro .= ' - É necessário inserir pelo menos 1(um) orientador de estudo;<br>\n';
		}
		
		if(count($orientadoresestudo) > $totalalfabetizadores['total_orientadores_a_serem_cadastrados']) {
			$erro .= ' - Número de orientadores cadastrados ultrapassou o número máximo;<br>\n';
		}
		
		if($orientadoresestudo) {
			foreach($orientadoresestudo as $oe) {
				if($oe['iustipoorientador']=='profissionaismagisterio' && substr($oe['iuscpf'],0,3)!='SIS') {
					$possuidoc = $db->pegaUm("SELECT COUNT(*) FROM sispacto2.portarianomeacao WHERE iusd='".$oe['iusd']."'");
					if($possuidoc==0) {
						$erro .= ' - Orientador de Estudo : '.$oe['iusnome'].' não enviou o documento comprobatório<br>\n';
					}
				}
				$total[$oe['iustipoorientador']]++;
			}
		}
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] != $total['orientadorsispacto2013']) {
			$arr = $db->pegaLinha("SELECT joeid, joecomentario FROM sispacto2.justificativaorientadorestudo WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND joetipo='orientadorsispacto2013' AND joestatus='A'");
			if($arr) $arr2 = $db->carregarColuna("SELECT jotid FROM sispacto2.justicativaorientadortipo WHERE joeid='".$arr['joeid']."' AND juostatus='A'");
			if(count($arr2)==0) {
				$erro .= ' - É necessário marcar o Tipo Justificativa (Orientadores de Estudo do Pacto 2013 recomendados para certificaçãoo);<br>\n';
			}
			if(!$arr['joecomentario']) {
				$erro .= ' - É necessário preencher o comentários adicionais (Orientadores de Estudo do Pacto 2013 recomendados para certificação);<br>\n';
			}
		}
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] != ($total['orientadorsispacto2013']+$total['professorsispacto2013'])) {
			$arr = $db->pegaLinha("SELECT joeid, joecomentario FROM sispacto2.justificativaorientadorestudo WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND joetipo='professorsispacto2013' AND joestatus='A'");
			if($arr) $arr2 = $db->carregarColuna("SELECT jotid FROM sispacto2.justicativaorientadortipo WHERE joeid='".$arr['joeid']."' AND juostatus='A'");
			if(count($arr2)==0) {
				$erro .= ' - É necessário marcar o Tipo Justificativa (Professores Alfabetizadores do Pacto 2013 recomendados para certificação);<br>\n';
			}
			if(!$arr['joecomentario']) {
				$erro .= ' - É necessário preencher o comentários adicionais (Professores Alfabetizadores do Pacto 2013 recomendados para certificação);<br>\n';
			}
		}
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] != ($total['tutoresproletramento']+$total['orientadorsispacto2013']+$total['professorsispacto2013'])) {
			$arr = $db->pegaLinha("SELECT joeid, joecomentario FROM sispacto2.justificativaorientadorestudo WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND joetipo='tutoresproletramento' AND joestatus='A'");
			if($arr) $arr2 = $db->carregarColuna("SELECT jotid FROM sispacto2.justicativaorientadortipo WHERE joeid='".$arr['joeid']."' AND juostatus='A'");
			if(count($arr2)==0) {
				$erro .= ' - É necessário marcar o Tipo Justificativa (Tutores Pró-Letramento);<br>\n';
			}
			if(!$arr['joecomentario']) {
				$erro .= ' - É necessário preencher o comentários adicionais (Tutores Pró-Letramento);<br>\n';
			}
		}
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] != ($total['tutoresproletramento']+$total['tutoresredesemproletramento']+$total['orientadorsispacto2013']+$total['professorsispacto2013'])) {
			$arr = $db->pegaLinha("SELECT joeid, joecomentario FROM sispacto2.justificativaorientadorestudo WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND joetipo='tutoresredesemproletramento' AND joestatus='A'");
			if($arr) $arr2 = $db->carregarColuna("SELECT jotid FROM sispacto2.justicativaorientadortipo WHERE joeid='".$arr['joeid']."' AND juostatus='A'");
			if(count($arr2)==0) {
				$erro .= ' - É necessário marcar o Tipo Justificativa (Professores da rede que não foram Tutores do Pró-Letramento);<br>\n';
			}
			if(!$arr['joecomentario']) {
				$erro .= ' - É necessário preencher o comentários adicionais (Professores da rede que não foram Tutores do Pró-Letramento);<br>\n';
			}
		}
		
		if($_SESSION['sispacto2']['esfera']=='estadual') {
			$sql = "SELECT COUNT(*) FROM sispacto2.alfabetizadoresindicados WHERE alistatus='P' AND aliestufdestino='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf']."'";
			$nummun = $db->pegaUm($sql);
			if($nummun) {
				$erro .= ' - Existem municípios que desejam incluir professores na sua rede que estão com a situação Pendente.<br>\n';
			}
			
		}
		/*
		if(date('Ymd') > '20121231') {
			$erro .= ' - O prazo de envio das adesões esta encerrado (31/12/2012).<br>\n';
		}
		*/
	
	}
	
	return (($erro)?$erro:true);
}

function verificarCoordenadorLocalTermoCompromisso($dados) {
	global $db;
	// verificando se coordenador local aceitou o termo de compromisso
	$coordlocal = carregarDadosIdentificacaoUsuario(array("picid"=>$dados['picid'],"pflcod"=>PFL_COORDENADORLOCAL));
	
	if($coordlocal) {
		$coordlocal = current($coordlocal);
	}
	
	if($coordlocal['iustermocompromisso']!="t") {
		$al = array("alert"=>"Antes de cadastrar os Orientadores de Estudo, preencha todos os campos obrigatórios da tela “Dados do Coordenador”.","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=dados");
		alertlocation($al);
	}
}

function atualizarBotoesOrientadoresEstudo($dados) {
	global $db;
	$sql = "UPDATE sispacto2.pactoidadecerta SET picmostrarbotao".$dados['tipo']."=TRUE WHERE picid='".$dados['picid']."'";
	$db->executar($sql);
	$db->commit();
}



function inserirCoordenadorLocalGerenciamento($dados) {
	global $db;
	
	$sql = "SELECT					  i.iusd,
									  i.picid,
	     							  t.pflcod, 
	     							  p.pfldsc, 
	     							  CASE WHEN c.muncod IS NOT NULL THEN m.estuf||' - '||m.mundescricao 
	     							   	   WHEN c.estuf IS NOT NULL THEN es.estuf||' - '||es.estdescricao
	     							  END as descricao 
	     						FROM sispacto2.identificacaousuario i 
	     						LEFT JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd 
	     						LEFT JOIN seguranca.perfil p ON p.pflcod=t.pflcod 
	     						LEFT JOIN sispacto2.pactoidadecerta c ON c.picid=i.picid 
	     						LEFT JOIN territorios.municipio m ON m.muncod=c.muncod 
	     						LEFT JOIN territorios.estado es ON es.estuf=c.estuf
	     						WHERE i.iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	$iusd  = $identificacaousuario['iusd'];
	$picid = $identificacaousuario['picid'];
	$pflcod = $identificacaousuario['pflcod'];
	
	if($iusd) {
		
		if($pflcod) {
			if($picid) {
				if($picid!=$dados['picid']) {
		 			$al = array("alert"=>"Este CPF ja possui um perfil (".$identificacaousuario['pfldsc'].",".$identificacaousuario['descricao'].") no sistema e não pode ser cadastrado","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
		 			alertlocation($al);
				}
			} else {
				$at_pic = "picid='".$dados['picid']."', ";
			}
		} else {
			$at_pic = "picid='".$dados['picid']."', ";
		}
		
		$sql = "UPDATE sispacto2.identificacaousuario SET {$at_pic} iusstatus='A', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$iusd."'";
		$db->executar($sql);
	} else {
    	$sql = "INSERT INTO sispacto2.identificacaousuario(
	            picid, iuscpf, iusnome, iusemailprincipal,  
	            iusdatainclusao, iusstatus)
			    VALUES ('".$dados['picid']."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."',  
			            NOW(), 'A') returning iusd;";
    	$iusd = $db->pegaUm($sql);
	}
    	
    $existe_usu = $db->pegaUm("select usucpf from seguranca.usuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
    	
   	if(!$existe_usu) {
    	
    	$sql = "INSERT INTO seguranca.usuario(
            	usucpf, usunome, usuemail, usustatus, ususenha, suscod)
    			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 'A', '".md5_encrypt_senha("simecdti","")."', 'A');";
    	$db->executar($sql);
    	
   	} else {
   		if($dados['reenviarsenha']=="S") {
   			$cl_senha = ", ususenha='".md5_encrypt_senha( "simecdti", '' )."', usuchaveativacao=false";
   		}
   		$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', usuemail='".$dados['iusemailprincipal']."' {$cl_senha} WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
   		$db->executar($sql);
   	}
   	
	if($dados['reenviarsenha']=="S") {
		$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO SISPACTO 2014","email" => $dados['iusemailprincipal']);
		$destinatario = $dados['iusemailprincipal'];
		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
		$assunto = "Cadastro no SIMEC - MÓDULO SISPACTO 2014";
		$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISPACTO. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
							  <p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
							  <p>Sua Senha de acesso é: %s</p>
							  <br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.",
			'Prezado(a)',
			$usunome,
			"simecdti"	
			);
		
		if(!strstr($_SERVER['HTTP_HOST'],"simec-local")){
			enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		}
	}	
    	
   	$existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and sisid='".SIS_SISPACTO."'");
    	
   	if(!$existe_sis) {
    		
    	$sql = "INSERT INTO seguranca.usuario_sistema(
        	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
    			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', ".SIS_SISPACTO.", 'A', NULL, NOW(), 'A');";
	    	
    	$db->executar($sql);
	    	
   	} else {
   		if($dados['suscod']=="A") {
	    	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A', susdataultacesso=NOW() WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND sisid='".SIS_SISPACTO."'";
	    	$db->executar($sql);
   		}
   	}
    	
   	$existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORLOCAL."'");
    	
   	if(!$existe_pfl) {
    		$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".PFL_COORDENADORLOCAL."');";
    		$db->executar($sql);
   	}
   	
   	$adesao = $db->pegaLinha("SELECT * FROM sispacto2.pactoidadecerta WHERE picid='".$dados['picid']."'");
   	
    $existe_usr = $db->pegaUm("select usucpf from sispacto2.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORLOCAL."' AND rpustatus='A' ".(($adesao['muncod'])?"AND muncod='".$adesao['muncod']."'":"").(($adesao['estuf'])?"AND estuf='".$adesao['estuf']."'":""));
    
    if(!$existe_usr) {
   		$sql = "INSERT INTO sispacto2.usuarioresponsabilidade(
           		pflcod, usucpf, rpustatus, rpudata_inc, muncod, estuf)
			    VALUES ('".PFL_COORDENADORLOCAL."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), ".(($adesao['muncod'])?"'".$adesao['muncod']."'":"NULL").", ".(($adesao['estuf'])?"'".$adesao['estuf']."'":"NULL").");";
   		$db->executar($sql);
    }
     
    $existe_tpf = $identificacaousuario['pflcod'];
    
    if(!$existe_tpf) {
		$sql = "INSERT INTO sispacto2.tipoperfil(
 		            iusd, pflcod, tpestatus)
 		    	VALUES ('".$iusd."', '".PFL_COORDENADORLOCAL."', 'A');";
     	$db->executar($sql);
    } else {

     	if($existe_tpf!=PFL_COORDENADORLOCAL) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$identificacaousuario['pfldsc'].",".$identificacaousuario['descricao'].") no sistema e não pode ser cadastrado","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
 			alertlocation($al);
     	}
    	
    }
    
	$sql = "INSERT INTO sispacto2.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$iusd."', NOW(), '".$_SESSION['usucpf']."', '".str_replace(array("'"),array(""),simec_json_encode($dados))."', 'A', 'inserirCoordenadorLocalGerenciamento');";
	$db->executar($sql);
    
    
    $db->commit();
	
    if(!$dados['naoredirecionar']) {
		$al = array("alert"=>"Coordenador Local inserido com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
		alertlocation($al);
    }
	
}

function verificarSituacaoAdesao($dados) {
	global $db;
	$sql = "SELECT esdid FROM sispacto2.pactoidadecerta p 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			WHERE muncod='".$dados['muncod']."'";
	
	$esdid = $db->pegaUm($sql);
	
	if($esdid != ESD_ELABORACAO_COORDENADOR_LOCAL) {
		return 'FALSE';
	} else {
		return 'TRUE';
	}
}

function alterarStatusIndicacao($dados) {
	global $db;
	
	$sql = "UPDATE sispacto2.alfabetizadoresindicados SET alistatus='".$dados['status']."' WHERE aliid='".$dados['aliid']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sispacto2.totalalfabetizadores totl SET total_orientadores_a_serem_cadastrados = foo2.total_orientadores 
			FROM (
			       SELECT 
			       CASE 
			       WHEN total = 0 THEN 0
			       WHEN total <= 10 THEN 1
			       WHEN MOD(total,25) >= 10 THEN num+1 ELSE num END as total_orientadores, 
			       total, 
			       MOD(total,25),
			       talid
				   FROM (
					       SELECT 
					       floor((total+coalesce((SELECT SUM(aliquantidade) FROM sispacto2.alfabetizadoresindicados WHERE alistatus='A' AND aliestufdestino=sigla::character(2)),0))/25) as num,
					       total+coalesce((SELECT SUM(aliquantidade) FROM sispacto2.alfabetizadoresindicados WHERE alistatus='A' AND aliestufdestino=sigla::character(2)),0) as total,
					       talid 
					       FROM sispacto2.totalalfabetizadores
					) foo
			) foo2 WHERE foo2.talid = totl.talid AND totl.sigla::character(2) IN(SELECT aliestufdestino FROM sispacto2.alfabetizadoresindicados WHERE aliid='".$dados['aliid']."') AND totl.dependencia ilike 'ESTADUAL'";
	
	$db->executar($sql);
	
	if($dados['status']=='I') {
		$sql = "DELETE FROM sispacto2.alfabetizadoresindicados WHERE aliid='".$dados['aliid']."'";
		$db->executar($sql);
	}
	
	$db->commit();
	
	$al = array("alert"=>"Situação alterada com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
	alertlocation($al);
	
	
}

function enviarAnaliseIES() {
	global $db;
	
	return true;
	
}

function carregarExecucaoAtividade($dados) {
	global $db;
	
	$execucao_atividade = $db->pegaLinha("SELECT ROUND(AVG(apassituacao)) as apassituacao, 
											  to_char(MIN(apadatainicio),'dd/mm/YYYY') as apadatainicio, 
											  to_char(MAX(apadatafim),'dd/mm/YYYY') as apadatafim 
											  FROM sispacto2.atividadepacto ap 
										  	  INNER JOIN sispacto2.subatividades su ON su.suaid = ap.suaid 
										   	  WHERE su.atiid='".$dados['atiid']."' AND picid='".$dados['picid']."'".(($dados['iusd'])?" AND ap.iusd='".$dados['iusd']."'":""));
		
	return $execucao_atividade;
	
}

function carregarExecucaoSubAcao($dados) {
	global $db;
		
	$atividadepacto = $db->pegaLinha("SELECT apaid,
											 apassituacao, 
										     to_char(apadatainicio,'dd/mm/YYYY') as apadatainicio,
											 to_char(apadatafim,'dd/mm/YYYY') as apadatafim
									  FROM sispacto2.atividadepacto 
									  WHERE suaid='".$dados['suaid']."' AND picid='".$dados['picid']."'".(($dados['iusd'])?" AND iusd='".$dados['iusd']."'":""));
	
	return $atividadepacto;
	
	
}

function mostrarAbaTurma($dados) {
	global $db;
		
	$sql = "SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['docid']."'";
	$esdid = $db->pegaUm($sql);
	
	if($esdid==ESD_VALIDADO_COORDENADOR_LOCAL || $esdid==ESD_ANALISE_COORDENADOR_LOCAL) return true;
	else return false;
}

function inserirTurmaCoordenadorLocal($dados) {
	global $db;
	$sql = "SELECT turid FROM sispacto2.turmas WHERE iusd='".$dados['iusd']."' AND picid='".$dados['picid']."'";
	$turid = $db->pegaUm($sql);
	
	if(!$turid) {
		$sql = "INSERT INTO sispacto2.turmas(
	            iusd, turdesc, turstatus, picid)
	    		VALUES ('".$dados['iusd']."', 'Turma OE - #".$dados['iusd']."', 'A', '".$dados['picid']."');";
		$db->executar($sql);
	}
}

function carregarTurmasOrientadores($dados) {
	global $db;
	$orientadoresestudo = carregarDadosIdentificacaoUsuario($dados);
	
	if($orientadoresestudo) {
		foreach($orientadoresestudo as $oe) {
			inserirTurmaCoordenadorLocal(array("iusd"=>$oe['iusd'],"picid"=>$dados['picid']));  
		}
	}
	
	$db->commit();
	
	$sql = "SELECT 
			    '<center><img src=../imagens/salvar.png style=\"cursor:pointer;\" onclick=\"comporTurma(\''||turid||'\')\"></center>' as acao, 
				t.turdesc as turdesc,
				i.iuscpf,
				i.iusnome,
				i.iusemailprincipal,
				(SELECT '(' || itedddtel || ') '|| itenumtel FROM sispacto2.identificacaotelefone WHERE iusd=t.iusd AND itetipo='T') as telefone,
				(SELECT COUNT(*) FROM sispacto2.orientadorturma WHERE turid=t.turid) as nalunos
			FROM sispacto2.turmas t 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd 
			INNER JOIN sispacto2.tipoperfil tt ON tt.iusd = i.iusd AND tt.pflcod=".PFL_ORIENTADORESTUDO."
			WHERE i.picid='".$dados['picid']."'";
	
	$cabecalho = array("&nbsp;","Turma","CPF","Nome","Email","Telefone","Número de professores");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
}

function gravarJustificativasTroca($dados) {
	global $db;
	
   	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
   	
	if($dados['resp1']) {
		foreach($dados['resp1'] as $iusd => $resp) {
			
			$sql = "SELECT jtoid FROM sispacto2.justificativatrocaorientador WHERE iusd='".$iusd."' AND picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";
			$jtoid = $db->pegaUm($sql);
			
			if(!$jtoid) {
				
				$sql = "INSERT INTO sispacto2.justificativatrocaorientador(picid, iusd) 
						VALUES ('".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
    							'".$iusd."') RETURNING jtoid;";
				
				$jtoid = $db->pegaUm($sql);
				
			}
			
			foreach($resp as $vl1) {
				
				$arqid_resp1=null;
				
				if($_FILES['arq1']['name'][$iusd][$vl1]) {
					
					$_FILES['just_tmp'] = array("name"      => $_FILES['arq1']['name'][$iusd][$vl1],
												"type"      => $_FILES['arq1']['type'][$iusd][$vl1],
												"tmp_name"  => $_FILES['arq1']['tmp_name'][$iusd][$vl1],
												"error" 	=> $_FILES['arq1']['error'][$iusd][$vl1],
												"size" 		=> $_FILES['arq1']['size'][$iusd][$vl1]);
		
			        $file = new FilesSimec( "", array(), "" );
			        $file->setUpload( NULL, "just_tmp", false );
			        $arqid_resp1 = $file->getIdArquivo();
			        
				} elseif($dados['arq1'][$iusd][$vl1]) {
					$arqid_resp1 = $dados['arq1'][$iusd][$vl1];
				}
				
				$up_resp1[] = $vl1."|".$arqid_resp1;
				
				if($vl1 == '2') {
					if($dados['resp2'][$iusd]) {
						foreach($dados['resp2'][$iusd] as $vl2) {

							$arqid_resp2=null;
							
							if($_FILES['arq2']['name'][$iusd][$vl2]) {
								
								$_FILES['just_tmp'] = array("name"      => $_FILES['arq2']['name'][$iusd][$vl2],
															"type"      => $_FILES['arq2']['type'][$iusd][$vl2],
															"tmp_name"  => $_FILES['arq2']['tmp_name'][$iusd][$vl2],
															"error" 	=> $_FILES['arq2']['error'][$iusd][$vl2],
															"size" 		=> $_FILES['arq2']['size'][$iusd][$vl2]);
					
						        $file = new FilesSimec( "", array(), "" );
						        $file->setUpload( NULL, "just_tmp", false );
						        $arqid_resp2 = $file->getIdArquivo();
						        
							} elseif($dados['arq2'][$iusd][$vl2]) {
								$arqid_resp2 = $dados['arq2'][$iusd][$vl2];
							}
							
							$up_resp2[] = $vl2."|".$arqid_resp2;
							
						}
					}
				}
			} // fim foreach
			
			$sql = "UPDATE sispacto2.justificativatrocaorientador SET jtooutrosdsc=".(($dados['jtooutrosdsc'.$iusd])?"'".$dados['jtooutrosdsc'.$iusd]."'":"NULL").", resp1=".(($up_resp1)?"'".implode(";",$up_resp1)."'":"NULL").", resp2=".(($up_resp2)?"'".implode(";",$up_resp2)."'":"NULL")." WHERE jtoid='".$jtoid."'";
			$db->executar($sql);
			
		}
	}
	
	$db->commit();
	$al = array("alert"=>"Justificativas para trocar os Orientadores de Estudo foram salvas com sucesso. Para submeter ao MEC, acesse a aba Resumo Orientadores de Estudo e clique em Enviar para análise da substituição do(s) Orientador(es) pelo MEC.","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
	alertlocation($al);
	
	
}


function carregarJustificativaTroca($dados) {
	global $db;
	
	if($dados['jtoid']) {
		$justificativatrocaorientador = $db->pegaLinha("SELECT * FROM sispacto2.justificativatrocaorientador WHERE jtoid='".$dados['jtoid']."'");
	} else {
		$justificativatrocaorientador = $db->pegaLinha("SELECT * FROM sispacto2.justificativatrocaorientador WHERE iusd='".$dados['iusd']."' AND picid='".$dados['picid']."'");		
	}
	 
	
	if($justificativatrocaorientador) {
		
		$cadastrojustificativa = true;
		
		$cr_resp1 = $justificativatrocaorientador['resp1'];
		$cr_resp1 = explode(";",$cr_resp1);
		if($cr_resp1) {
			foreach($cr_resp1 as $v) {
				$vv = explode("|", $v);
				$resp1[] = $vv[0];
				$arq1[$vv[0]]  = $vv[1];
			}
		} else {
			$resp1 = array();
			$arq1  = array();
		}
		$cr_resp2 = $justificativatrocaorientador['resp2'];
		$cr_resp2 = explode(";",$cr_resp2);
		if($cr_resp2) {
			foreach($cr_resp2 as $v) {
				$vv = explode("|", $v);
				$resp2[] = $vv[0];
				$arq2[$vv[0]]  = $vv[1];
			}
		} else {
			$resp2 = array();
			$arq2  = array();
		}
	} else {
		$resp1 = array();
		$resp2 = array();
		$arq1  = array();
		$cadastrojustificativa = false;
	}
	
	return array("resp1" => $resp1, "arq1" => $arq1, "resp2" => $resp2, "arq2" => $arq2, "cadastrojustificativa" => (($justificativatrocaorientador)?true:false), "jtooutrosdsc" => $justificativatrocaorientador['jtooutrosdsc'] );
	
}

function removerJustificativaTroca($dados) {
	global $db;
	$sql = "DELETE FROM sispacto2.justificativatrocaorientador WHERE picid='".$dados['picid']."' AND iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Justificativa para substituição do(s) Orientadores de Estudo removida com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
	alertlocation($al);
	
}

function autorizarTrocaOrientadores($picid) {
	global $db;
	
	$sql = "SELECT iusd FROM sispacto2.justificativatrocaorientador WHERE picid='".$picid."'";
	$iusds = $db->carregarColuna($sql);
	
	if($iusds) {
		foreach($iusds as $iusd) {
			removerOrientadorEstudo(array("iusd"=>$iusd));
			$sql = "UPDATE sispacto2.justificativatrocaorientador SET jtostatus='A' WHERE picid='".$picid."' AND iusd='".$iusd."'";
			$db->executar($sql);
		}
	}
	
	$sql = "DELETE FROM sispacto2.justicativaorientadortipo WHERE joeid IN(SELECT joeid FROM sispacto2.justificativaorientadorestudo WHERE picid='".$picid."')";
	$db->executar($sql);
	
	$sql = "DELETE FROM sispacto2.justificativaorientadorestudo WHERE picid='".$picid."'";
	$db->executar($sql);
	
	$db->commit();
	
	return true;
	
}

function naoAutorizarTrocaOrientadores() {
	global $db;
	
	$sql = "DELETE FROM sispacto2.justificativatrocaorientador WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND jtostatus='I'";
	$db->executar($sql);
	
	$db->commit();
	
	return true;
	
}

function condicaoAnaliseTrocaMEC() {
	global $db;
	
	$sql = "SELECT jtoid FROM sispacto2.justificativatrocaorientador WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";
	$jtoids = $db->carregarColuna($sql);
	
	if(count($jtoids) == 0) {
		return "É necessário cadastrar as justificativas para a substituição do(s) Orientador(es) de estudo";		
	}
	
	return true;
	
}

function posEfetuarTrocaOrientadores() {
	

	$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['redirecionartroca'] = true;
	
	return true;
	
}

function condicaoEfetuarTrocaOrientadores() {
	if(date("Y-m-d h:i:s") > '2013-02-16 00:00:00') {
		return false;
	} else {
		return true;
	}
}

function trocarOrientadorEstudoMunicipio($dados) {
	global $db;
	
	include_once '_funcoes_universidade.php';
	
	$sql = "SELECT * FROM sispacto2.formacaoinicialouvintes WHERE fioid='".$dados['fioid']."'";
	$fioarr = $db->pegaLinha($sql);
	
	if(!$_SESSION['sispacto2']['coordenadorlocal']['uncid']) {
	 	$al = array("alert"=>"Coordenador Local não vinculado a nenhuma Universidade","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
	 	alertlocation($al);
	}
	
	carregarCoordenadorIES(array('uncid'=>$_SESSION['sispacto2']['coordenadorlocal']['uncid']));
	$retorno = efetuarTrocaUsuarioPerfil(array('iusdantigo'=>$dados['iusd_antigo'],
										        'iuscpf_'=>$fioarr['fiocpf'],
												'iusnome_'=>$fioarr['fionome'],
												'iusemailprincipal_'=>$fioarr['fioemail'],
												'uncid'=>$fioarr['uncid'],
												'pflcod_'=>PFL_ORIENTADORESTUDO,
												'noredirect' => true));
	
	unset($_SESSION['sispacto2']['universidade']);
	
	if($retorno) {
		$sql = "UPDATE sispacto2.identificacaousuario SET iusformacaoinicialorientador=true WHERE iuscpf='".$fioarr['fiocpf']."'";
		$db->executar($sql);
		
		$sql = "UPDATE sispacto2.formacaoinicialouvintes SET fiostatus='I', fiodata=NOW() WHERE fioid='".$dados['fioid']."'";
		$db->executar($sql);
		
		$sql = "DELETE FROM sispacto2.justicativaorientadortipo WHERE joeid IN(SELECT joeid FROM sispacto2.justificativaorientadorestudo WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."')";
		$db->executar($sql);
		
		$sql = "DELETE FROM sispacto2.justificativaorientadorestudo WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";
		$db->executar($sql);
		
		$db->commit();
		
	 	$al = array("alert"=>"Troca efetuada com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
	 	alertlocation($al);
	} else {
	 	$al = array("alert"=>"Novo Usuário ja possui atribuções no SISPACTO, por isso não pode ser inserido","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo");
	 	alertlocation($al);
	}

	
}

function sqlEquipeCoordenadorLocal($dados) {
	global $db;
	
	$sql = "(
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil,
				   (SELECT usucpf FROM sispacto2.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN pi.picid IS NOT NULL THEN 
														CASE WHEN pi.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN pi.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
				   
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.pactoidadecerta pi ON pi.picid = i.picid  
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pi.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND pi.picid='".$dados['picid']."' AND i.iusstatus='A'
			) UNION ALL (
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_PROFESSORALFABETIZADOR.") as perfil,
				   (SELECT usucpf FROM sispacto2.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN pi.picid IS NOT NULL THEN 
														CASE WHEN pi.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN pi.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
				   
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.pactoidadecerta pi ON pi.picid = i.picid  
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pi.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND pi.picid='".$dados['picid']."' AND i.iusstatus='A'
			)";
	
	return $sql;
}

function verificarCenso2012($dados) {
	global $db;
	$sql = "SELECT m.estuf||'/'||m.mundescricao FROM sispacto2.professoresalfabetizadores p INNER JOIN territorios.municipio m ON m.muncod = p.muncod WHERE cpf='".$dados['cpf']."'";
	$mundescricao = $db->pegaUm($sql);
	
	echo $mundescricao;
	
	
}

function gerenciarMateriais($dados) {
	global $db;
	
	$sql = "SELECT matid FROM sispacto2.materiais WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";
	$matid = $db->pegaUm($sql);
	
	if($matid) {
		
		$sql = "UPDATE sispacto2.materiais
   			SET recebeumaterialpacto='".$dados['recebeumaterialpacto']."', 
   				distribuiumaterialpacto='".$dados['distribuiumaterialpacto']."', 
       			recebeumaterialpnld='".$dados['recebeumaterialpnld']."', 
       			recebeulivrospnld='".$dados['recebeulivrospnld']."', 
       			recebeumaterialpnbe='".$dados['recebeumaterialpnbe']."', 
       			criadocantinholeitura='".$dados['criadocantinholeitura']."'
 			WHERE matid='".$matid."'";
		
		$db->executar($sql);
		
		
	} else {
		$sql = "INSERT INTO sispacto2.materiais(
	            picid, recebeumaterialpacto, distribuiumaterialpacto, 
	            recebeumaterialpnld, recebeulivrospnld, recebeumaterialpnbe, 
	            criadocantinholeitura, matstatus)
	    VALUES ('".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."', 
	    		'".$dados['recebeumaterialpacto']."', 
	    		'".$dados['distribuiumaterialpacto']."', 
	    		'".$dados['recebeumaterialpnld']."', 
	    		'".$dados['recebeulivrospnld']."', 
	    		'".$dados['recebeumaterialpnbe']."', 
	            '".$dados['criadocantinholeitura']."', 'A') RETURNING matid;";
		
		$matid = $db->pegaUm($sql);
		
	}
	
	$db->commit();
	
	if($_FILES['arquivo']['error']=='0') {
		$campos	= array("matid"	 => $matid,
						"mafdsc" => "'".$dados['mafdsc']."'");	
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file = new FilesSimec("materiaisfotos", $campos ,"sispacto2");
				
		$arquivoSalvo = $file->setUpload($dados['mafdsc']);
	}
	
	
	$al = array("alert"=>"Informações sobre materiais salvas com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=materiais");
	alertlocation($al);

	
}

function validarAtividadeCoordenadorLocal($dados) {
	
	// validando informações
	if(!$dados['gmccargahoraria']) {
		$erros[] = "Carga Horária não pode ser vazia";
	}
	
	if(!$dados['gmcqtdparticipantes']) {
		$erros[] = "quantidade de participantes não pode ser vazia";
	}
	
	if(!$dados['gmcpublicoalvo']) {
		$erros[] = "Nenhum público alvo foi selecionado";
	}
	
	if(!$dados['gmcinicio']) {
		$erros[] = "Início não pode ser vazio";
	}
	
	if(!$dados['gmcfim']) {
		$erros[] = "Fim não pode ser vazio";
	}
	
	return $erros;
	
}


function salvarAtividadeCoordenadorLocal($dados) {
	global $db;
	
	$erros = validarAtividadeCoordenadorLocal($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&aba=gestaomobilizacao");
		alertlocation($al);
	}
	
	if($dados['gmcid']) {
		
		$gmcid = $dados['gmcid'];
		
		$sql = "UPDATE sispacto2.gestaomobilizacaocoordenadorlocal SET 
				gmcatividade = '".$dados['gmcatividade']."', 
	    		gmcatividadeoutra = ".(($dados['gmcatividadeoutra'])?"'".$dados['gmcatividadeoutra']."'":"NULL").", 
	    		gmccargahoraria = '".$dados['gmccargahoraria']."', 
	            gmcqtdparticipantes = '".$dados['gmcqtdparticipantes']."', 
	            gmcpublicoalvo = '".implode(";",$dados['gmcpublicoalvo']).";', 
	            gmcpublicoalvooutros = ".(($dados['gmcpublicoalvooutros'])?"'".$dados['gmcpublicoalvooutros']."'":"NULL").", 
	            gmcinicio = '".formata_data_sql($dados['gmcinicio'])."', 
	            gmcfim = '".formata_data_sql($dados['gmcfim'])."' 
	            WHERE gmcid='".$gmcid."'";
		
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO sispacto2.gestaomobilizacaocoordenadorlocal(
	            iusd, gmcatividade, gmcatividadeoutra, gmccargahoraria, 
	            gmcqtdparticipantes, gmcpublicoalvo, gmcpublicoalvooutros, gmcinicio, 
	            gmcfim, gmcstatus)
	    		VALUES ('".$_SESSION['sispacto2']['coordenadorlocal']['iusd']."', 
	    				'".$dados['gmcatividade']."', 
	    				".(($dados['gmcatividadeoutra'])?"'".$dados['gmcatividadeoutra']."'":"NULL").", 
	    				'".$dados['gmccargahoraria']."', 
	            		'".$dados['gmcqtdparticipantes']."', 
	            		'".implode(";",$dados['gmcpublicoalvo']).";', 
	            		".(($dados['gmcpublicoalvooutros'])?"'".$dados['gmcpublicoalvooutros']."'":"NULL").", 
	            		'".formata_data_sql($dados['gmcinicio'])."', 
	            		'".formata_data_sql($dados['gmcfim'])."', 
	            		'A') RETURNING gmcid;";
		
		$gmcid = $db->pegaUm($sql);
	
	}
	
	$db->executar("UPDATE sispacto2.gestaomobilizacaoperguntas SET gmpnaoatividades=FALSE WHERE iusd='".$_SESSION['sispacto2']['coordenadorlocal']['iusd']."'");
	
	if($_FILES['arquivo1']['error']=='0') {
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file = new FilesSimec("gestaomobilizacaocoordenadorlocal", array() ,"sispacto2");
				
		$arquivoSalvo = $file->setUpload(null,'arquivo1',false);
		
		$db->executar("UPDATE sispacto2.gestaomobilizacaocoordenadorlocal SET arqid1='".$file->getIdArquivo()."' WHERE gmcid='".$gmcid."'");
		
	}
	
	if($_FILES['arquivo2']['error']=='0') {
				
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$file2 = new FilesSimec("gestaomobilizacaocoordenadorlocal", array() ,"sispacto2");
				
		$arquivoSalvo = $file2->setUpload(null,'arquivo2',false);
		
		$db->executar("UPDATE sispacto2.gestaomobilizacaocoordenadorlocal SET arqid2='".$file2->getIdArquivo()."' WHERE gmcid='".$gmcid."'");
		
	}
	
	$db->commit();

	$al = array("alert"=>"Atividade salva com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&aba=gestaomobilizacao");
	alertlocation($al);
	
	
}

function excluirAtividadeCoordenadorLocal($dados) {
	global $db;
	
	$sql = "UPDATE sispacto2.gestaomobilizacaocoordenadorlocal SET gmcstatus='I' WHERE gmcid='".$dados['gmcid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Atividade excluída com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&aba=gestaomobilizacao");
	alertlocation($al);
	
}

function pegarAtividadeCoordenadorLocal($dados) {
	global $db;
	
	$sql = "SELECT gmcid,
				   gmcatividade,
  				   gmcatividadeoutra,
  				   gmccargahoraria,
  				   gmcqtdparticipantes,
  				   gmcpublicoalvo,
  				   gmcpublicoalvooutros,
  				   to_char(gmcinicio,'dd/mm/YYYY') as gmcinicio,
  				   to_char(gmcfim, 'dd/mm/YYYY') as gmcfim,
  				   '<img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=\'sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&requisicao=downloadDocumento&arqid='||p.arqid||'\'\"> '||p.arqnome||'.'||p.arqextensao as anexo1,
  				   '<img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=\'sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&requisicao=downloadDocumento&arqid='||p2.arqid||'\'\"> '||p2.arqnome||'.'||p2.arqextensao as anexo2
  				     
  			FROM sispacto2.gestaomobilizacaocoordenadorlocal g 
  			LEFT JOIN public.arquivo p ON p.arqid = g.arqid1 
  			LEFT JOIN public.arquivo p2 ON p2.arqid = g.arqid2
  			WHERE gmcid='".$dados['gmcid']."'";
	$gestaomobilizacaocoordenadorlocal = $db->pegaLinha($sql);
	
	echo simec_json_encode($gestaomobilizacaocoordenadorlocal);
	
}

function gerenciarPerguntasCoordenadorLocal($dados) {
	global $db;
	
	$sql = "SELECT gmpid FROM sispacto2.gestaomobilizacaoperguntas WHERE iusd='".$_SESSION['sispacto2']['coordenadorlocal']['iusd']."'";
	$gmpid = $db->pegaUm($sql);
	
	if($gmpid) {
		
		$sql = "UPDATE sispacto2.gestaomobilizacaoperguntas SET
	            gmppergunta1='".$dados['gmppergunta1']."', 
	            gmppergunta1_comentario='".$dados['gmppergunta1_comentario']."', 
	            gmppergunta2=".(($dados['gmppergunta2'])?"'".implode(";",$dados['gmppergunta2']).";'":"NULL").", 
	            gmppergunta2_outros=".(($dados['gmppergunta2_outros'])?"'".$dados['gmppergunta2_outros']."'":"NULL").",
	            gmppergunta3='".$dados['gmppergunta3']."',
	            gmpnaoatividades=".(($dados['gmpnaoatividades'])?"TRUE":"FALSE")." 
	            WHERE gmpid='".$gmpid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.gestaomobilizacaoperguntas(
	            iusd, 
	            gmppergunta1, 
	            gmppergunta1_comentario, 
	            gmppergunta2, 
	            gmppergunta2_outros,
	            gmppergunta3,
	            gmpnaoatividades)
	    		VALUES (
	    		'".$_SESSION['sispacto2']['coordenadorlocal']['iusd']."', 
	    		'".$dados['gmppergunta1']."', 
	    		'".$dados['gmppergunta1_comentario']."', 
	    		".(($dados['gmppergunta2'])?"'".implode(";",$dados['gmppergunta2']).";'":"NULL").", 
	            ".(($dados['gmppergunta2_outros'])?"'".$dados['gmppergunta2_outros']."'":"NULL").",
	            '".$dados['gmppergunta3']."',
	            ".(($dados['gmpnaoatividades'])?"TRUE":"FALSE").");";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	$al = array("alert"=>"Perguntas salvadas com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocalexecucao&acao=A&aba=gestaomobilizacao");
	alertlocation($al);
	
	
}

function calcularNumeroDias($dados) {
	global $db;
	
	$sql = "SELECT date '".formata_data_sql($dados['fim'])."' - date '".formata_data_sql($dados['inicio'])."' as dias";
	echo ($db->pegaUm($sql)+1);
	
}

function verificarNumeroAtividades($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as natividades 
			FROM sispacto2.gestaomobilizacaocoordenadorlocal 
			WHERE iusd='".$_SESSION['sispacto2']['coordenadorlocal']['iusd']."' AND gmcstatus='A'";
	
	echo $db->pegaUm($sql);
	
}

function carregarOrientadoresestudoSispacto2013($dados) {
	global $db;
	
	echo "<script>function marcarTodos(obj) {jQuery(\"[name^='iuscpf[']\").attr('checked',obj.checked);}</script>";
	
	echo "<form method=\"post\" id=\"formulario_importacao\">";
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"efetuarImportacaoSispacto2013\">";
	echo "<input type=\"hidden\" name=\"pflcod\" value=\"".PFL_ORIENTADORESTUDO."\">";
	
	echo "<p align=center><b>Orientadores de Estudo SISPACTO 2013</b></p>";
	echo "<br>";
	
	$sql = "SELECT '<center>'||CASE WHEN avg(ma.mavtotal) >= 7 THEN '<input type=\"checkbox\" name=\"iuscpf[]\" value=\"'||i.iuscpf||'\">' ELSE '' END||'</center>' as chk, replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, count(distinct m.menid) as naval, CASE WHEN avg(ma.mavtotal) >= 7 THEN '<font color=blue;>'||round(avg(ma.mavtotal),2)||'</font>' ELSE '<font color=red;>'||round(avg(ma.mavtotal),2)||'</font>' END as mavtotal FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO2013." 
			INNER JOIN sispacto.mensario m ON m.iusd = i.iusd 
			INNER JOIN sispacto.mensarioavaliacoes ma ON ma.menid = m.menid 
			LEFT JOIN sispacto2.identificacaousuario i2 ON i2.iuscpf = i.iuscpf 
			LEFT JOIN sispacto2.tipoperfil t2 ON t2.iusd = i2.iusd 
			WHERE i.picid=".$dados['picid']." AND i.iusstatus='A' AND i.iuscpf NOT ILIKE 'SIS%' AND (t2.tpeid IS NULL OR i2.iusstatus='I') AND ma.mavrecomendadocertificacao IS NULL
			GROUP BY i.iuscpf, i.iusnome 
			ORDER BY i.iusnome";
	
	echo "<p><input type=\"checkbox\" id=\"marcartodos\" onclick=\"marcarTodos(this);\"> <b>Marcar todos</b></p>";
	$cabecalho = array("&nbsp;","CPF","Nome","Número de avaliações","Nota Final");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	echo "<br>";
	echo "<p align=center><input type=\"button\" name=\"importar\" value=\"Importar\" onclick=\"jQuery('#formulario_importacao').submit();\"></p>";
	
	echo "</form>";
}

function carregarProfessoresalfabetizadoresSispacto2013($dados) {
	global $db;

	echo "<script>function marcarTodos(obj) {jQuery(\"[name^='iuscpf[']\").attr('checked',obj.checked);}</script>";

	echo "<form method=\"post\" id=\"formulario_importacao\">";
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"efetuarImportacaoSispacto2013\">";
	echo "<input type=\"hidden\" name=\"pflcod\" value=\"".PFL_PROFESSORALFABETIZADOR."\">";
	echo "<input type=\"hidden\" name=\"turid\" value=\"".$dados['turid']."\">";

	echo "<p align=center><b>Professores Alfabetizadores SISPACTO 2013</b></p>";
	echo "<br>";

	$sql = "SELECT '<center>'||CASE WHEN avg(ma.mavtotal) >= 7 THEN '<input type=\"checkbox\" name=\"iuscpf[]\" value=\"'||i.iuscpf||'\">' ELSE '' END||'</center>' as chk, replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, count(distinct m.menid) as naval, CASE WHEN avg(ma.mavtotal) >= 7 THEN '<font color=blue;>'||round(avg(ma.mavtotal),2)||'</font>' ELSE '<font color=red;>'||round(avg(ma.mavtotal),2)||'</font>' END as mavtotal FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_PROFESSORALFABETIZADOR2013." 
			INNER JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto.turmas tu ON tu.turid = ot.turid 
			INNER JOIN sispacto.identificacaousuario i3 ON i3.iusd = tu.iusd 
			INNER JOIN sispacto.mensario m ON m.iusd = i.iusd
			INNER JOIN sispacto.mensarioavaliacoes ma ON ma.menid = m.menid
			LEFT JOIN sispacto2.identificacaousuario i2 ON i2.iuscpf = i.iuscpf
			LEFT JOIN sispacto2.tipoperfil t2 ON t2.iusd = i2.iusd
			WHERE i3.iuscpf='".$dados['iuscpf']."' AND i.iusstatus='A' AND i.iuscpf NOT ILIKE 'SIS%' AND (t2.tpeid IS NULL OR i2.iusstatus='I') AND ma.mavrecomendadocertificacao IS NULL
			GROUP BY i.iuscpf, i.iusnome
			ORDER BY i.iusnome";

	echo "<p><input type=\"checkbox\" id=\"marcartodos\" onclick=\"marcarTodos(this);\"> <b>Marcar todos</b></p>";
	$cabecalho = array("&nbsp;","CPF","Nome","Número de avaliações","Nota Final");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);

	echo "<br>";
	echo "<p align=center><input type=\"button\" name=\"importar\" value=\"Importar\" onclick=\"jQuery('#formulario_importacao').submit();\"></p>";

	echo "</form>";
}

function efetuarImportacaoSispacto2013($dados) {
	global $db;
	
	if($dados['iuscpf']) {
		
		if($_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']) {
			$db->executar("UPDATE sispacto2.pactoidadecerta SET picselecaopublica=true WHERE picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'");
		}
		
		foreach($dados['iuscpf'] as $cpf) {
			
			$sql = "INSERT INTO sispacto2.identificacaousuario(
		            picid, muncod, eciid, nacid, fk_cod_docente, iuscpf, iusnome,
		            iussexo, iusdatanascimento, iusnomemae, iusnomeconjuge, iusagenciasugerida,
		            iusemailprincipal, iusemailopcional, iusdatainclusao, iuscadastrovalidadoies,
		            iussituacao, iusstatus, funid, iusagenciaend, iustipoorientador,
		            foeid, iustermocompromisso, tvpid, muncodatuacao, uncid, iusserieprofessor,
		            iusformacaoinicialorientador, cadastradosgb, iustipoprofessor,
		            iusdocumento, iusnaodesejosubstituirbolsa)
					SELECT i.picid, i.muncod, i.eciid, i.nacid, i.fk_cod_docente, i.iuscpf, i.iusnome,
		            i.iussexo, i.iusdatanascimento, i.iusnomemae, i.iusnomeconjuge, i.iusagenciasugerida,
		            i.iusemailprincipal, i.iusemailopcional, i.iusdatainclusao, i.iuscadastrovalidadoies,
		            i.iussituacao, 'A' as iusstatus, i.funid, i.iusagenciaend, 'orientadorsispacto2013' as iustipoorientador,
		            i.foeid, i.iustermocompromisso, i.tvpid, i.muncodatuacao, null, i.iusserieprofessor,
		            false, false, 'cpflivre' as iustipoprofessor,
		            i.iusdocumento, i.iusnaodesejosubstituirbolsa FROM sispacto.identificacaousuario i 
					LEFT JOIN sispacto2.identificacaousuario i2 ON i.iuscpf = i2.iuscpf
					WHERE i.iuscpf='".$cpf."' AND i2.iusd IS NULL
		            RETURNING iusd";
			
			$iusd = $db->pegaUm($sql);
			
			if(!$iusd) {
				$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".$cpf."'");
				if($dados['pflcod']==PFL_ORIENTADORESTUDO) 
					$db->executar("UPDATE sispacto2.identificacaousuario SET iusstatus='A', iustipoorientador='orientadorsispacto2013' WHERE iusd='".$iusd."'");
				
				if($dados['pflcod']==PFL_PROFESSORALFABETIZADOR) {
					$sql = "SELECT cpf FROM sispacto2.professoresalfabetizadores WHERE cpf='".$cpf."'";
					$cpf_existe = $db->pegaUm($sql);
					$db->executar("UPDATE sispacto2.identificacaousuario SET iusstatus='A', iustipoprofessor='".(($cpf_existe)?"censo":"cpflivre")."' WHERE iusd='".$iusd."'");
				}
				
			} else {
				
				if($dados['pflcod']==PFL_PROFESSORALFABETIZADOR) {
					$sql = "SELECT cpf FROM sispacto2.professoresalfabetizadores WHERE cpf='".$cpf."'";
					$cpf_existe = $db->pegaUm($sql);
					$db->executar("UPDATE sispacto2.identificacaousuario SET iusstatus='A', iustipoprofessor='".(($cpf_existe)?"censo":"cpflivre")."' WHERE iusd='".$iusd."'");
				}
				
			}

			$db->executar("DELETE FROM sispacto2.tipoperfil WHERE iusd='".$iusd."'");
		
			$sql = "INSERT INTO sispacto2.tipoperfil(
		            iusd, pflcod, tpestatus)
				    VALUES (
					'".$iusd."', '".$dados['pflcod']."', 'A');";
			
			$db->executar($sql);
			
			$db->executar("DELETE FROM sispacto2.portarianomeacao WHERE iusd='".$iusd."'");
				
			$sql = "INSERT INTO sispacto2.portarianomeacao(
            			arqid, iusd, ponstatus)
    					SELECT arqid, '".$iusd."', ponstatus FROM sispacto.portarianomeacao WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";
				
			$db->executar($sql);
			
			$db->executar("DELETE FROM sispacto2.identificacaotelefone WHERE iusd='".$iusd."'");
				
			$sql = "INSERT INTO sispacto2.identificacaotelefone(
            		iusd, itedddtel, itenumtel, itetipo, itestatus)
    				SELECT '".$iusd."', itedddtel, itenumtel, itetipo, itestatus FROM sispacto.identificacaotelefone WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";
			
			$db->executar($sql);
			
			$db->executar("DELETE FROM sispacto2.identificaoendereco WHERE iusd='".$iusd."'");
				
			$sql = "INSERT INTO sispacto2.identificaoendereco(
		            muncod, iusd, ientipo, iencep, iencomplemento, iennumero, 
		            iensatatus, ienlogradouro, ienbairro)
					SELECT muncod, '".$iusd."', ientipo, iencep, iencomplemento, iennumero, 
		            iensatatus, ienlogradouro, ienbairro FROM sispacto.identificaoendereco WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";
			
			$db->executar($sql);
			
			$db->executar("DELETE FROM sispacto2.identiusucursoformacao WHERE iusd='".$iusd."'");
				
			$sql = "INSERT INTO sispacto2.identiusucursoformacao(
		            iusd, cufid, iufdatainiformacao, iufdatafimformacao, iufsituacaoformacao, 
		            iufstatus)
					SELECT '".$iusd."', cufid, iufdatainiformacao, iufdatafimformacao, iufsituacaoformacao, 
		            iufstatus FROM sispacto.identiusucursoformacao WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";

			$db->executar($sql);
			
			$db->executar("DELETE FROM sispacto2.identusutipodocumento WHERE iusd='".$iusd."'");
				
			$sql = "INSERT INTO sispacto2.identusutipodocumento(
		            iusd, tdoid, itdufdoc, itdnumdoc, itddataexp, itdnoorgaoexp, 
		            itdstatus)
					SELECT '".$iusd."', tdoid, itdufdoc, itdnumdoc, itddataexp, itdnoorgaoexp, 
		            itdstatus FROM sispacto.identusutipodocumento WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";
			
			$db->executar($sql);
			
			if($dados['turid']) {
				
				$db->executar("DELETE FROM sispacto2.orientadorturma WHERE iusd='".$iusd."'");
				
				$sql = "INSERT INTO sispacto2.orientadorturma(
					            turid, iusd, otustatus, otudata)
					    VALUES ('".$dados['turid']."', '".$iusd."', 'A', NOW());";
				
				$db->executar($sql);
				
			}
			
			
			$db->commit();
		}
	}
	
	$al = array("alert"=>"Importação efetuada com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=".$dados['aba']);
	alertlocation($al);
	
}

function atualizarSerieProfessorAlfabetizador($dados) {
	global $db;
	
	if($dados['sercod']) {
		$iusserieprofessor = implode(";",$dados['sercod']);
	}
	
	$db->executar("UPDATE sispacto2.identificacaousuario SET iusserieprofessor=".(($iusserieprofessor)?"'".$iusserieprofessor."'":"NULL")." WHERE iusd='".$dados['iusd']."'");
	$db->commit();
	
	$al = array("alert"=>"Série atualizada com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas&turid=".$dados['turid']);
	alertlocation($al);
	
	
}


?>