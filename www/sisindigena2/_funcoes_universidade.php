<?

function inserirCoordenadorIESGerenciamento($dados) {
	global $db;
	
	$sql = "SELECT iusd FROM sisindigena2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
 	$iusd = $db->pegaUm($sql);
	
 	if($iusd) {
 		$sql = "UPDATE sisindigena2.identificacaousuario SET iusstatus='A', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$iusd."'";
 		$db->executar($sql);
 	} else {
     	$sql = "INSERT INTO sisindigena2.identificacaousuario(
 	            uncid, iuscpf, iusnome, iusemailprincipal,  
 	            iusdatainclusao, iusstatus)
 			    VALUES ('".$dados['uncid']."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."',  
 			            NOW(), 'A') returning iusd;";
     	$iusd = $db->pegaUm($sql);
 	}
    	
    $existe_usu = $db->pegaUm("select usucpf from seguranca.usuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
    	
   	if(!$existe_usu) {
    	
	   	$sql = "INSERT INTO seguranca.usuario(
             	usucpf, usunome, usuemail, usustatus, ususenha, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 'A', '".md5_encrypt_senha("simecdti", '')."', 'A');";
     	$db->executar($sql);
    	
    } else {
    	
    	if($dados['reenviarsenha']=="S") {
    		$cl_senha = ", ususenha='".md5_encrypt_senha( "simecdti", '' )."', usuchaveativacao=false";
    	}
    	$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', usuemail='".$dados['iusemailprincipal']."' {$cl_senha} WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
    	$db->executar($sql);
    }
   	
 	if($dados['reenviarsenha']=="S") {
 		
 		$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO SISPACTO","email" => $dados['iusemailprincipal']);
 		$destinatario = $dados['iusemailprincipal'];
 		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
 		$assunto = "Cadastro no SIMEC - MÓDULO SISPACTO";
 		$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISIndígena 2015. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
    	
    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and sisid='".SIS_INDIGENA."'");
    	
    if(!$existe_sis) {
    		
    	$sql = "INSERT INTO seguranca.usuario_sistema(
         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', ".SIS_INDIGENA.", 'A', NULL, NOW(), 'A');";
	    	
     	$db->executar($sql);
	    	
    } else {
    	if($dados['suscod']=="A") {
 	    	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND sisid='".SIS_INDIGENA."'";
 	    	$db->executar($sql);
    	}
    }
    	
    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORIES."'");
    	
    if(!$existe_pfl) {
    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".PFL_COORDENADORIES."');";
     	$db->executar($sql);
    }
   	
    $existe_usr = $db->pegaUm("select usucpf from sisindigena2.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORIES."' AND rpustatus='A' AND uncid='".$dados['uncid']."'");
    
     if(!$existe_usr) {
    		$sql = "INSERT INTO sisindigena2.usuarioresponsabilidade(
            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
 			    VALUES ('".PFL_COORDENADORIES."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."');";
    		$db->executar($sql);
     }
    
     $arrTp = $db->pegaLinha("SELECT
     							  t.pflcod, 
     							  p.pfldsc, 
     							  uni.unisigla || '/' || uni.uninome as descricao 
     						FROM sisindigena2.tipoperfil t 
     						INNER JOIN seguranca.perfil p ON p.pflcod=t.pflcod 
     						INNER JOIN sisindigena2.identificacaousuario i ON i.iusd=t.iusd 
     						INNER JOIN sisindigena2.universidadecadastro unc ON unc.uncid=i.uncid 
     						LEFT JOIN sisindigena2.universidade uni ON uni.uniid=unc.uniid 
     						WHERE t.iusd='".$iusd."'");
     
     $existe_tpf = $arrTp['pflcod'];
    
     if(!$existe_tpf) {
 		$sql = "INSERT INTO sisindigena2.tipoperfil(
 		            iusd, pflcod, tpestatus)
 		    	VALUES ('".$iusd."', '".PFL_COORDENADORIES."', 'A');";
     	$db->executar($sql);
     } else {
    	
     	if($existe_tpf!=PFL_COORDENADORIES) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$arrTp['pfldsc'].",".$arrTp['descricao'].") no sistema e não pode ser cadastrado","location"=>"sisindigena2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
 			alertlocation($al);
     	}
    	
     }
    
     $db->commit();
			
 	$al = array("alert"=>"Coordenador-Geral da IES inserido com sucesso","location"=>"sisindigena2.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
 	alertlocation($al);
	
}

function inserirCoordenadorAdjuntoGerenciamento($dados) {
	global $db;
	
	$sql = "SELECT iusd FROM sisindigena2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
 	$iusd = $db->pegaUm($sql);
	
 	if($iusd) {
 		$sql = "UPDATE sisindigena2.identificacaousuario SET uncid='".$dados['uncid']."', picid='".$dados['picid']."', iusstatus='A', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$iusd."'";
 		$db->executar($sql);
 	} else {
     	$sql = "INSERT INTO sisindigena2.identificacaousuario(
 	            uncid, picid, iuscpf, iusnome, iusemailprincipal,  
 	            iusdatainclusao, iusstatus)
 			    VALUES ('".$dados['uncid']."', '".$dados['picid']."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."',  
 			            NOW(), 'A') returning iusd;";
     	$iusd = $db->pegaUm($sql);
 	}
    	
    $existe_usu = $db->pegaUm("select usucpf from seguranca.usuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
    	
   	if(!$existe_usu) {
    	
	   	$sql = "INSERT INTO seguranca.usuario(
             	usucpf, usunome, usuemail, usustatus, ususenha, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 'A', '".md5_encrypt_senha("simecdti", '')."', 'A');";
     	$db->executar($sql);
    	
    } else {
    	
    	if($dados['reenviarsenha']=="S") {
    		$cl_senha = ", ususenha='".md5_encrypt_senha( "simecdti", '' )."', usuchaveativacao=false";
    	}
    	$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', usuemail='".$dados['iusemailprincipal']."' {$cl_senha} WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
    	$db->executar($sql);
    }
   	
 	if($dados['reenviarsenha']=="S") {
 		
 		$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO SISPACTO","email" => $dados['iusemailprincipal']);
 		$destinatario = $dados['iusemailprincipal'];
 		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
 		$assunto = "Cadastro no SIMEC - MÓDULO SISPACTO";
 		$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISIndígena 2015. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
    	
    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and sisid='".SIS_INDIGENA."'");
    	
    if(!$existe_sis) {
    		
    	$sql = "INSERT INTO seguranca.usuario_sistema(
         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', ".SIS_INDIGENA.", 'A', NULL, NOW(), 'A');";
	    	
     	$db->executar($sql);
	    	
    } else {
    	if($dados['suscod']=="A") {
 	    	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND sisid='".SIS_INDIGENA."'";
 	    	$db->executar($sql);
    	}
    }
    	
    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORADJUNTOIES."'");
    	
    if(!$existe_pfl) {
    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".PFL_COORDENADORADJUNTOIES."');";
     	$db->executar($sql);
    }
   	
    $existe_usr = $db->pegaUm("select usucpf from sisindigena2.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORADJUNTOIES."' AND rpustatus='A' AND uncid='".$dados['uncid']."'");
    
     if(!$existe_usr) {
    		$sql = "INSERT INTO sisindigena2.usuarioresponsabilidade(
            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
 			    VALUES ('".PFL_COORDENADORADJUNTOIES."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."');";
    		$db->executar($sql);
     }
    
     $arrTp = $db->pegaLinha("SELECT
     							  t.pflcod, 
     							  p.pfldsc, 
     							  uni.unisigla || '/' || uni.uninome as descricao 
     						FROM sisindigena2.tipoperfil t 
     						INNER JOIN seguranca.perfil p ON p.pflcod=t.pflcod 
     						INNER JOIN sisindigena2.identificacaousuario i ON i.iusd=t.iusd 
     						INNER JOIN sisindigena2.universidadecadastro unc ON unc.uncid=i.uncid 
     						LEFT JOIN sisindigena2.universidade uni ON uni.uniid=unc.uniid 
     						WHERE t.iusd='".$iusd."'");
     
     $existe_tpf = $arrTp['pflcod'];
    
     if(!$existe_tpf) {
 		$sql = "INSERT INTO sisindigena2.tipoperfil(
 		            iusd, pflcod, tpestatus)
 		    	VALUES ('".$iusd."', '".PFL_COORDENADORADJUNTOIES."', 'A');";
     	$db->executar($sql);
     } else {
    	
     	if($existe_tpf!=PFL_COORDENADORADJUNTOIES) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$arrTp['pfldsc'].",".$arrTp['descricao'].") no sistema e não pode ser cadastrado","location"=>"sisindigena2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadoradjunto&acao=A&picid=".$dados['picid']);
 			alertlocation($al);
     	}
    	
     }
    
     $db->commit();
			
 	$al = array("alert"=>"Coordenador Adjunto inserido com sucesso","location"=>"sisindigena2.php?modulo=principal/universidade/gerenciarcoordenadoradjunto&acao=A&picid=".$dados['picid']);
 	alertlocation($al);
	
 }





function carregarCoordenadorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT d.esdid, u.uncid, re.reiid, su.uniuf, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sisindigena2.universidadecadastro u 
					 	   INNER JOIN sisindigena2.universidade su ON su.uniid = u.uniid
						   INNER JOIN sisindigena2.reitor re on re.uniid = su.uniid 
						   LEFT JOIN workflow.documento d ON d.docid = u.docid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$docid = $arr['docid'];
	
	if(!$docid) {
		$docid = wf_cadastrarDocumento(TPD_COORDENADORIES,"SIS Indigena Coordenador IES ".$dados['uncid']);
		$db->executar("UPDATE sisindigena2.universidadecadastro SET docid='".$docid."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sisindigena2.identificacaousuario i 
							   INNER JOIN sisindigena2.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_COORDENADORIES."'");
	
	
	$_SESSION['sisindigena2']['universidade'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
												  "uncid" => $arr['uncid'], 
												  "reiid" => $arr['reiid'], 
												  "estuf" => $arr['uniuf'], 
												  "docid" => $docid,
												  "iusd" => $infprof['iusd'],
												  "iuscpf" => $infprof['iuscpf']);
	
	if($dados['direcionar']) {
		$al = array("location"=>"sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function carregarSubAtividades($dados) {
	global $db;
	$sql = "SELECT suaid as codigo, suadesc as descricao FROM sisindigena2.subatividades WHERE atiid='".$dados['atiid']."'";
	$db->monta_combo('suaid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'suaid', '');
	
}

function carregarUniversidadesPorUF($dados) {
	global $db;
	$sql = "SELECT u.uncid as codigo, su.uninome as descricao FROM sisindigena2.universidadecadastro u 
	 	    INNER JOIN sisindigena2.universidade su ON su.uniid = u.uniid
			WHERE su.uniuf='".$dados['estuf']."'";
	
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function carregarEstruturaCurso($dados) {
	global $db;
	$sql = "SELECT * FROM sisindigena2.estruturacurso e
			LEFT JOIN territorios.municipio m ON m.muncod = e.muncod 
			WHERE e.ecuid='".$dados['ecuid']."'";
	$estruturacurso = $db->pegaLinha($sql);
	return $estruturacurso;
}

function atualizarEstruturaCurso($dados) {
	global $db;
	$sql = "UPDATE sisindigena2.estruturacurso SET muncod='".$dados['muncod_endereco']."', ecuobsplanoatividades=".(($dados['ecuobsplanoatividades'])?"'".$dados['ecuobsplanoatividades']."'":"NULL")." WHERE ecuid='".$dados['ecuid']."'";
	$db->executar($sql);
	
	$suaids = array_keys($dados['aundatainicioprev']);
	
	if($suaids) {
		foreach($suaids as $suaid) {
			$aunid = $db->pegaUm("SELECT aunid FROM sisindigena2.atividadeuniversidade au WHERE au.suaid = '".$suaid."' AND au.ecuid = '".$dados['ecuid']."'");
			
			if($aunid) {
				$sql = "UPDATE sisindigena2.atividadeuniversidade SET aundatainicioprev=".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", aundatafimprev=".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL")." WHERE aunid='".$aunid."'";
				$db->executar($sql);
			} else {
				$sql = "INSERT INTO sisindigena2.atividadeuniversidade(
            			suaid, aundatainicioprev, aundatafimprev, aunstatus, ecuid)
    					VALUES ('".$suaid."', ".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", ".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL").", 'A', '".$dados['ecuid']."');";
				$db->executar($sql);
			}
		}
	}
	
	$ainid = $db->pegaUm("SELECT ainid FROM sisindigena2.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'");
	
	if($ainid) {
		$sql = "UPDATE sisindigena2.articulacaoinstitucional
   				SET ainseduc=".$dados['ainseduc'].", 
   					ainseducjustificativa='".$dados['ainseducjustificativa']."', 
   					ainundime=".$dados['ainundime'].", 
				    ainundimejustificativa='".$dados['ainundimejustificativa']."', 
				    ainuncme=".$dados['ainuncme'].", 
				    ainuncmejustificativa='".$dados['ainuncmejustificativa']."'
				 WHERE ainid='".$ainid."';";
		
		$db->executar($sql);
		
	} else {
		$sql = "INSERT INTO sisindigena2.articulacaoinstitucional(
	            ecuid, ainseduc, ainseducjustificativa, ainundime, ainundimejustificativa, 
	            ainuncme, ainuncmejustificativa, ainstatus)
	    		VALUES ('".$dados['ecuid']."', ".$dados['ainseduc'].", '".$dados['ainseducjustificativa']."', ".$dados['ainundime'].", '".$dados['ainundimejustificativa']."', 
	            ".$dados['ainuncme'].", '".$dados['ainuncmejustificativa']."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	$al = array("alert"=>"Estrutura do curso atualizada com sucesso","location"=>$dados['goto']);
	alertlocation($al);
	
}

function carregarArticulacaoInstitucional($dados) {
	global $db;
	$sql = "SELECT * FROM sisindigena2.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'";
	$articulacaoinstitucional = $db->pegaLinha($sql);
	return $articulacaoinstitucional;
	
}


function carregarPlanoAtividades($dados) {
	global $db;
	
	$sql = "SELECT a.atiid, a.atidesc, s.suaid, s.suadesc, 
			".(($dados['ecuid'])?"(SELECT au.aundatainicioprev FROM sisindigena2.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatainicioprev, 
			".(($dados['ecuid'])?"(SELECT au.aundatafimprev FROM sisindigena2.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatafimprev 
			FROM sisindigena2.subatividades s 
			INNER JOIN sisindigena2.atividades a ON a.atiid = s.atiid 
			WHERE a.attitipo IN('U','E') AND s.suavisivel=true ORDER BY a.atidesc, s.suadesc";
	
	$subatividades = $db->carregar($sql);
	
	if($subatividades[0]) {
		foreach($subatividades as $sub) {
			$arrRsAgrupado[$sub['atidesc']]['atidesc'] = $sub['atidesc'];
			$arrRsAgrupado[$sub['atidesc']]['subatividades'][] = array("suaid"=>$sub['suaid'],
																	 "suadesc"=>$sub['suadesc'],
																	 "aundatainicioprev"=>$sub['aundatainicioprev'],
																	 "aundatafimprev"=>$sub['aundatafimprev']);
		}
	}
	
	echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" width=\"100%\">";
	echo "<tr>";
	echo "<td width=\"50%\" class=\"SubTituloCentro\">ATIVIDADES / SUBATIVIDADES</td><td class=\"SubTituloCentro\" colspan=\"2\">PERÍODO DE EXECUÇÃO</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td width=\"50%\" align=\"center\">&nbsp;</td><td align=\"center\">Início</td><td align=\"center\">Término</td>";
	echo "</tr>";
	
	
	if($arrRsAgrupado) {
		foreach($arrRsAgrupado as $atidesc => $at) {
			
			echo "<tr>";
			echo "<td width=\"50%\"><b>".$at['atidesc']."</b></td><td>&nbsp;</td><td>&nbsp;</td>";
			echo "</tr>";
			
			if($at['subatividades']) {
				
				foreach($at['subatividades'] as $su) {
					echo "<tr>";
					echo "<td width=\"50%\">".$su['suadesc']."</td>";
					if($dados['consulta']) echo "<td align=\"center\">".formata_data($su['aundatainicioprev'])."</td>";
					else echo "<td>".campo_data2('aundatainicioprev['.$su['suaid'].']','S', 'S', 'Inicío', 'S', '', '', $su['aundatainicioprev'], '', '', 'aundatainicioprev_'.$su['suaid'])."</td>";
					if($dados['consulta']) echo "<td align=\"center\">".formata_data($su['aundatafimprev'])."</td>";
					else echo "<td>".campo_data2('aundatafimprev['.$su['suaid'].']','S', 'S', 'Término', 'S', '', '', $su['aundatafimprev'], '', '', 'aundatafimprev_'.$su['suaid'])."</td>";
					echo "</tr>";
				}
			}
		}
	} else {
		echo "<tr>";
		echo "<td class=\"SubTituloEsquerda\">Nenhuma Subatividade foi cadastrada</td>";
		echo "</tr>";
	}
	
	echo "</table>";
	
}



function carregarDetalhamentoAbrangencia($dados) {
	global $db;
	
	if($dados['esfera']=='M') {
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc  FROM sisindigena2.identificacaousuario i 
				INNER JOIN sisindigena2.pactoidadecerta p ON p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sisindigena2.abrangencia a ON p.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='M'";
		
		$cabecalho = array("CPF","Nome","Email","Situação cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	}
	
	if($dados['esfera']=='E') {
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc FROM sisindigena2.identificacaousuario i 
				INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
				INNER JOIN sisindigena2.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sisindigena2.abrangencia a ON mm.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='E'";
		
		$cabecalho = array("CPF","Nome","Email","Situação cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	}
	
}

function carregarEquipeRecursosHumanos($dados) {
	global $db;
	$sql = "(SELECT '&nbsp' as acao, '<center>'||iuscpf||'</center>' as iuscpf, iusnome, iusemailprincipal, p.pfldsc, to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo  
			FROM sisindigena2.identificacaousuario i 
			LEFT JOIN sisindigena2.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sisindigena2.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_COORDENADORIES."') AND i.uncid='".$dados['uncid']."')
			UNION ALL (
			SELECT ".((!$dados['consulta'])?"'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"inserirEquipe(\''||i.iusd||'\');\" > <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirEquipeRecursosHumanos(\''||i.iusd||'\');\">' || CASE WHEN t.tpejustificativaformadories IS NULL THEN '' ELSE ' <img src=\"../imagens/valida2.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"jAlert(\''||t.tpejustificativaformadories||'\', \'Justificativa\');\">' END||'</center>'":"'&nbsp;'")." as acao, '<center>'||iuscpf||'</center>', iusnome, iusemailprincipal, p.pfldsc, to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo 
			FROM sisindigena2.identificacaousuario i
			LEFT JOIN sisindigena2.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sisindigena2.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."') AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome)";
	
	$equiperh = $db->carregar($sql);
	
	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Perfil","Período de atuação");
	$db->monta_lista_simples($equiperh,$cabecalho,1000,5,'N','100%','N');

}

function numeroMaximoCoordenadorAjuntoIES($dados) {
	global $db;
	$sql = "SELECT m.estuf FROM sisindigena2.estruturacurso e 
	 		INNER JOIN sisindigena2.abrangencia a ON a.ecuid = e.ecuid 
	 		INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
	 		WHERE e.uncid='".$dados['uncid']."' 
	 		GROUP BY m.estuf";
	
	$maxCoordenadorAjunto = $db->carregarColuna($sql);
	return count($maxCoordenadorAjunto);
	
}

function numeroCoordenadorAdjuntoIES($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."'";
	
	$numCoordenadorAjunto = $db->pegaUm($sql);
	
	return $numCoordenadorAjunto;
	
}


function validarNumeroCoordenadorAdjuntoIES($dados) {
	global $db;
	
	$maxCoordenadorAjunto = numeroMaximoCoordenadorAjuntoIES($dados);
	
	$numCoordenadorAjunto = numeroCoordenadorAdjuntoIES($dados);
	
	if($maxCoordenadorAjunto>$numCoordenadorAjunto) return true;
	else return false;
	
}

function numeroFormadorIES($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_FORMADORIES."'";
	
	$numFormadorIes = $db->pegaUm($sql);
	
	return $numFormadorIes;
}


function validarNumeroFormadorIES($dados) {
	global $db;
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$dados['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total)); 

	$numFormadorIes = numeroFormadorIES($dados);
	
	if($nturmas>$numFormadorIes) return true;
	else return false;
	
}

function pegarJustificativaFormadorIES($dados) {
	global $db;
	$sql = "SELECT tpejustificativaformadories FROM sisindigena2.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$tpejustificativaformadories = $db->pegaUm($sql);
	return $tpejustificativaformadories;
}


function inserirEquipeRecursosHumanos($dados) {
	global $db;
	
	if($dados['iusd']) {
		$sql = "DELETE FROM sisindigena2.tipoperfil WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
		
		if($dados['pflcod']!=PFL_FORMADORIES) {
			$sql = "SELECT turid FROM sisindigena2.turmas WHERE iusd='".$dados['iusd']."'";
			$existe = $db->pegaUm($sql);
			if($existe) {
				$al = array("alert"=>"Não é possível trocar o perfil. Este usuário possui turma associada. Remova a turma para altera o perfil","location"=>"sisindigena2.php?modulo=principal/universidade/inserirequipe&acao=A&iusd=".$dados['iusd']);
				alertlocation($al);
			}
		}
		
	}
	
	if($dados['pflcod']==PFL_COORDENADORADJUNTOIES) {
		if(!validarNumeroCoordenadorAdjuntoIES(array("uncid"=>$dados['uncid']))) {
			$al = array("alert"=>"Número de Coordenador Adjunto da IES está no máximo. Não é possível inserir o CPF","location"=>"sisindigena2.php?modulo=principal/universidade/inserirequipe&acao=A");
			alertlocation($al);
		}
	}
	
	$iusd = $db->pegaUm("SELECT iusd FROM sisindigena2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
	
	if(!$iusd) {
		
		$sql = "INSERT INTO sisindigena2.identificacaousuario(
	            iuscpf, iusnome, iusemailprincipal, foeid, uncid, iusdatainclusao)
	    		VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 
	    				'".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 
	    				'".$dados['foeid']."', '".$dados['uncid']."', NOW()) RETURNING iusd;";
		
		
		$iusd = $db->pegaUm($sql);
		
	} else {
		
		$sql = "SELECT 'Nome : '||i.iusnome||', Perfil : '||p.pfldsc||', CPF não pode ser cadastrado. É necessário remove-lo do perfil indicado.' as msg FROM sisindigena2.identificacaousuario i 
				INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
				WHERE i.iusd='".$iusd."'";
		
		$msg = $db->pegaUm($sql);
		
		if($msg) {
			$al = array("alert"=>$msg,"location"=>"sisindigena2.php?modulo=principal/universidade/inserirequipe&acao=A");
			alertlocation($al);
		}
		
		$sql = "UPDATE sisindigena2.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."', 
														 foeid='".$dados['foeid']."', 
														 uncid='".$dados['uncid']."',
														 iusstatus='A'
				WHERE iusd='".$iusd."'";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM sisindigena2.identificacaotelefone WHERE iusd='".$iusd."' AND itetipo='T'");
	
	$sql = "INSERT INTO sisindigena2.identificacaotelefone(
           	iusd, itedddtel, itenumtel, itetipo, itestatus)
   			VALUES ('".$iusd."','".$dados['itedddtel']['T']."', '".$dados['itenumtel']['T']."', 'T', 'A');";
		
	$db->executar($sql);

	$sql = "INSERT INTO sisindigena2.tipoperfil(
            iusd, pflcod, tpestatus, tpeatuacaoinicio, tpeatuacaofim, tpejustificativaformadories)
    		VALUES ('".$iusd."', '".$dados['pflcod']."', 'A', '".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01', '".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01',".(($dados['tpejustificativaformadories'])?"'".$dados['tpejustificativaformadories']."'":"NULL").");";
	
	$db->executar($sql);
	
	$db->commit();
	
	$al = array("alert"=>"Equipe gravada com sucesso","javascript"=>"window.opener.carregarEquipeRecursosHumanos();window.close();");
	alertlocation($al);
	
}

function excluirEquipeRecursosHumanos($dados) {
	global $db;
	
	$sql = "DELETE FROM sisindigena2.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM sisindigena2.identificacaotelefone WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);

	$db->commit();
	
	$sql = "SELECT turid FROM sisindigena2.turmas WHERE iusd='".$dados['iusd']."'";
	$turids = $db->carregarColuna($sql);
	
	if($turids) {
		foreach($turids as $turid) {
			excluirTurma(array("turid" => $turid));			
		}
	}
	
	$al = array("alert"=>"Equipe removida com sucesso","location"=>"sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=recursos_humanos");
	alertlocation($al);
	
}

function numeroMaximoTurmas($dados) {
	global $db;
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$dados['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total));
	$formadoressolicitados = carregarDadosIdentificacaoUsuario(array("uncid"=>$dados['uncid'],"pflcod"=>PFL_FORMADORIES,"tpejustificativaformadories"=>true));
	$numeroMaximoTurmas = $nturmas + count($formadoressolicitados);
	return $numeroMaximoTurmas;
}

function numeroTurmas($dados) {
	global $db;
	$total_turmas = $db->pegaUm("SELECT COUNT(*) FROM sisindigena2.turmas WHERE uncid='".$dados['uncid']."' AND turstatus='A'");
	return $total_turmas;
}


function atualizarTurma($dados) {
	global $db;
	
	$sql = "UPDATE sisindigena2.turmas SET iusd='".$dados['iusd']."', turdesc='".$dados['turdesc']."', muncod='".$dados['muncod_endereco']."' WHERE turid='".$dados['turid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	echo "Turma atualizada com sucesso";
}

function validarCadastroTurma($dados) {
	global $db;
	
	$numeroMaximoTurmas = numeroMaximoTurmas($dados);
	$total_turmas = numeroTurmas($dados);
	if($numeroMaximoTurmas>$total_turmas) return true;
	else return false;
	
}

function inserirTurma($dados) {
	global $db;
	
	
	if(validarCadastroTurma($dados)) {
	
		$sql = "INSERT INTO sisindigena2.turmas(
		        uncid, iusd, turdesc, turstatus, muncod)
		    	VALUES ('".$dados['uncid']."', '".$dados['iusd']."', '".$dados['turdesc']."', 'A', '".$dados['muncod_endereco']."');";
		
		$db->executar($sql);
		$db->commit();
		
		echo "Turma inserida com sucesso";
	
	} else {
		
		echo "Não foi possível cadastrar a turma. Número de turmas está no limite.";
		
	}
}

function carregarTurmas($dados) {
	global $db;
	$sql = "SELECT 
				'<img src=\"../imagens/mais.gif\" title=\"mais\" id=\"btn_turma_'||t.turid||'\" style=\"cursor:pointer;\" onclick=\"abrirTurma('||t.turid||',this)\"> ".((!$dados['consulta'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer\" onclick=\"excluirTurma('||t.turid||');\"> <img src=../imagens/alterar.gif style=\"cursor:pointer;\"  onclick=\"abrirCadastroTurma(\''||turid||'\')\"> <img src=../imagens/salvar.png style=\"cursor:pointer;\" onclick=\"comporTurma(\''||turid||'\')\">":"")."' as acao,
				m.estuf|| ' / ' || m.mundescricao as polo, 
				t.turdesc,
				'<center>'||i.iuscpf||'</center>' as iuscpf,
				i.iusnome,
				i.iusemailprincipal,
				(SELECT '(' || itedddtel || ') '|| itenumtel FROM sisindigena2.identificacaotelefone WHERE iusd=t.iusd AND itetipo='T') as telefone,
				(SELECT COUNT(*) FROM sisindigena2.orientadorturma ot INNER JOIN sisindigena2.tipoperfil tt ON tt.iusd = ot.iusd and tt.pflcod=".PFL_ORIENTADORESTUDO." WHERE turid=t.turid) as nalunos
			FROM sisindigena2.turmas t 
			INNER JOIN sisindigena2.identificacaousuario i ON i.iusd = t.iusd 
			LEFT JOIN territorios.municipio m ON m.muncod = t.muncod
			WHERE t.uncid='".$dados['uncid']."'";
	
	if($dados['formacaoinicial']) {
		echo "<p><b>Marcar Todos : </b><input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"TRUE\"> Presente <input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"FALSE\"> Ausente</p>";
	}

	
	$cabecalho = array("&nbsp;","Polo","Turma","CPF","Nome","Email","Telefone","Número de orientadores");
	
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function validarFormadoresTurmas($dados) {
	global $db;
	$sql = "SELECT 'Formador ('||i.iusnome||') não foi vinculado a nenhuma turma.' as t FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_FORMADORIES."
			LEFT JOIN sisindigena2.turmas tt ON tt.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND tt.turid IS NULL";
	$mensagens = $db->carregarColuna($sql);
	
	return $mensagens;
	
}


function inserirAlunoTurma($dados) {
	global $db;
	$sql = "INSERT INTO sisindigena2.orientadorturma(
            turid, iusd, otustatus)
    		VALUES ('".$dados['turid']."', '".$dados['iusd']."', 'A');";
	$db->executar($sql);
	
	if($dados['uncid']) {
		$sql = "UPDATE sisindigena2.identificacaousuario SET uncid='".$dados['uncid']."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	} else {
		$sql = "UPDATE sisindigena2.identificacaousuario SET uncid=NULL WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	}
	
	$db->commit();
}

function excluirAlunoTurma($dados) {
	global $db;
	if($dados['iusd']) $wh = "iusd='".$dados['iusd']."'";
	elseif($dados['turid']) $wh = "turid='".$dados['turid']."'";
	
	$sql = "DELETE FROM sisindigena2.orientadorturma WHERE ".$wh;
	$db->executar($sql);
	$db->commit();
}

function excluirTurma($dados) {
	global $db;
	
	excluirAlunoTurma($dados);
	
	$sql = "DELETE FROM sisindigena2.turmas WHERE turid='".$dados['turid']."'";
	$db->executar($sql);
	$db->commit();
	
}

function carregarFiltrosMunicipios($dados) {
	global $db;
	if($dados['estuf']) {
		$sql = "SELECT m.muncod as codigo, REPLACE(m.mundescricao,'\'',' ') as descricao 
				FROM territorios.municipio m 
				".(($dados['esfera']=="M")?"INNER JOIN sisindigena2.pactoidadecerta p ON p.muncod = m.muncod":"")." 
				WHERE m.estuf='".$dados['estuf']."' AND m.muncod NOT IN(SELECT muncod FROM sisindigena2.abrangencia WHERE esfera='".$dados['esfera']."') ORDER BY mundescricao";
	} else {
		$sql = "SELECT muncod as codigo, muncod as descricao FROM sisindigena2.abrangencia WHERE 1=2";
	}
	
	$_SESSION['indice_sessao_combo_popup']['muncod_abrangencia']['sql'] = $sql;
}

function verificarCoordenadorIESTermoCompromisso($dados) {
	global $db;
	// verificando se coordenador local aceitou o termo de compromisso
	$coordies = carregarDadosIdentificacaoUsuario(array("uncid"=>$dados['uncid'],"pflcod"=>PFL_COORDENADORIES));
	
	if($coordies) {
		$coordies = current($coordies);
	}
	
	if($db->testa_superuser()) {
		return true;
	}
	
	if($coordies['iustermocompromisso']!="t") {
		$al = array("alert"=>"Por favor preencha todos os campos obrigatórios da tela “Dados Coordenador IES”.","location"=>"sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=dados");
		alertlocation($al);
	}
}

function numeroAlunosTurma($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sisindigena2.orientadorturma WHERE turid='".$dados['turid']."'";
	$numturmas = $db->pegaUm($sql);
	
	echo $numturmas;
	
}

function validarEnvioAnaliseMEC() {
	global $db;
	
	$mensagens = validarFormadoresTurmas(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	if($mensagens) $erro .= implode('<br>\n',$mensagens).'<br><br>\n\n Há '.count($mensagens).' Orientadores de Estudo que não foram vinculados a nenhuma turma. Retorne para a tela Turmas e vincule todos os nomes.<br><br>\n\n';
	
	$pp = carregarNumeroOrientadoresPendencia(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	if($pp['nummunpendencias']>0) $erro .= 'Há '.$pp['nummunpendencias'].' municípios que não concluíram o cadastramento dos seus Orientadores de Estudo<br>\n';
	if($pp['numestpendencias']>0) $erro .= 'Há '.$pp['numestpendencias'].' estados que não concluíram o cadastramento dos seus Orientadores de Estudo<br>\n';
	
	$numeroMaximoTurmas = numeroMaximoTurmas(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	$total_turmas = numeroTurmas(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	if($total_turmas>$numeroMaximoTurmas) {
		$erro .= '<br>\nNúmero máximo de turmas (na tela Turmas) deve ser igual ao Número máximo de Formadores (na tela Equipe IES) + Formadores Justificados<br><br>\n\n';
	}
	
	$tso = validarTurmasSemOrientadores(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	if($tso) {
		$erro .= '<br>\nExistem '.count($tso).' turma(s) sem orientadores cadastrados.';
	}
	
	if($erro) return $erro;
	else return true;

}

function validarTurmasSemOrientadores($dados) {
	global $db;
	
	$sql = "SELECT foo.turdesc FROM (
			SELECT t.turdesc, (SELECT COUNT(*) FROM sisindigena2.orientadorturma WHERE turid=t.turid) as nalunos FROM sisindigena2.turmas t 
			WHERE t.uncid='".$dados['uncid']."') foo WHERE foo.nalunos=0";
	
	$turmassemorientadores = $db->carregar($sql);
	
	return $turmassemorientadores;
	
}

function validarOrientadoresCadastradosTurma($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*)			
			FROM sisindigena2.turmas t 
			INNER JOIN sisindigena2.orientadorturma ot ON ot.turid = t.turid
			INNER JOIN sisindigena2.tipoperfil tt ON tt.iusd = ot.iusd and tt.pflcod=".PFL_ORIENTADORESTUDO." 
			INNER JOIN sisindigena2.identificacaousuario i ON i.iusd = tt.iusd 
			WHERE t.uncid='".$dados['uncid']."' AND i.iusstatus='A'";
	
	$numOrientadoresTurmas = $db->pegaUm($sql);
	
	$sql = "SELECT SUM(foo.t) FROM (
			(SELECT COUNT(*) as t  FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.pactoidadecerta p ON p.picid = i.picid 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena2.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sisindigena2.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='M' AND i.iusstatus='A') 
			UNION ALL (
			SELECT COUNT(*) as t FROM sisindigena2.identificacaousuario i 
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			INNER JOIN sisindigena2.pactoidadecerta p ON p.estuf = m.estuf AND p.picid = i.picid 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena2.abrangencia a ON m.muncod=a.muncod 
			INNER JOIN sisindigena2.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='E' AND i.iusstatus='A')
			) foo";
	
	$numTotalOrientadores = $db->pegaUm($sql);
	
	if($numOrientadoresTurmas==$numTotalOrientadores) return true;
	else return ($numTotalOrientadores-$numOrientadoresTurmas);
	
}



function calculaPorcentagemTramitacaoUniversidade($dados) {
	global $db;
	
	
	$sql = "SELECT d.docid, d.esdid, to_char(h.htddata,'YYYY-mm-dd') as htddata 
			FROM sisindigena2.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
	
	$documento = $db->pegaLinha($sql);
	
	$sql = "SELECT to_char(htddata,'YYYY-mm-dd') as htddata FROM workflow.historicodocumento WHERE docid='".$documento['docid']."' ORDER BY htddata ASC LIMIT 1";
	$aundatainicio = $db->pegaUm($sql);
	
	if($documento['esdid']==ESD_ANALISE_COORDENADOR_IES) {
		$aunsituacao = '50';
	}
	
	if($documento['esdid']==ESD_VALIDADO_COORDENADOR_IES) {
		$aunsituacao = '100';
		$aundatafim = $documento['htddata'];;
	}
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	
}

function calculaPorcentagemFormacaoInicial($dados) {
	global $db;
	
	
	$sql = "SELECT d.docid, d.esdid, to_char(h.htddata,'YYYY-mm-dd') as htddata 
			FROM sisindigena2.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docidformacaoinicial 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
	
	$documento = $db->pegaLinha($sql);
	
	$sql = "SELECT to_char(htddata,'YYYY-mm-dd') as htddata FROM workflow.historicodocumento WHERE docid='".$documento['docid']."' ORDER BY htddata ASC LIMIT 1";
	$aundatainicio = $db->pegaUm($sql);
	
	if($documento['esdid']==ESD_FECHADO_FORMACAOINICIAL) {
		$aunsituacao = '100';
		$aundatafim = $documento['htddata'];;
	}
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	
}


function sqlEquipeCoordenadorIES($dados) {
	global $db;
	
	$sql = "SELECT  DISTINCT 
					i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal,
					i.iusformacaoinicialorientador, 
					p.pflcod,
					p.pfldsc, 
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod = t.pflcod) as perfil
			FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.nucleouniversidade n ON n.uncid = i.uncid 
			INNER JOIN workflow.documento d ON d.docid = n.docid 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE (t.pflcod IN('".PFL_FORMADORIES."',
							   '".PFL_SUPERVISORIES."',
							   '".PFL_COORDENADORADJUNTOIES."',
							   '".PFL_COORDENADORLOCAL."',
							   '".PFL_PROFESSORALFABETIZADOR."',
							   '".PFL_ORIENTADORESTUDO."',
							   '".PFL_CONTEUDISTA."',
							   '".PFL_PESQUISADOR."')) AND i.uncid='".$dados['uncid']."' AND d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome";
	
	return $sql;
}


function mostrarAbaFormacaoInicial($dados) {
	global $db;
	$estado = wf_pegarEstadoAtual( $_SESSION['sispacto']['universidade']['docid'] );
	if($estado['esdid'] == ESD_VALIDADO_COORDENADOR_IES) {
		return true;
	} else {
		return false;
	}
	
}

function salvarFormacaoInicial($dados) {
	global $db;
	if($dados['iusd']) {
		foreach($dados['iusd'] as $iusd => $fi) {
			$sql = "UPDATE sisindigena2.identificacaousuario SET iusformacaoinicialorientador=".$fi." WHERE iusd='".$iusd."'";
			$db->executar($sql);
		}
		$db->commit();
	}
	
	$al = array("alert"=>"Formação Inicial dos Orientadores foram salvas com sucesso. Não esqueça de preencher os alunos Presentes na Formação Inicial que não constam na lista, ao final do processo CLICAR no botão de Concluir os Registros de Frequência","location"=>"sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=formacaoinicial");
	alertlocation($al);

}

function condicaoFormacaoInicial() {
	global $db;
	
	$sql = "SELECT COUNT(i.iusd) as total FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER join sisindigena2.pactoidadecerta p on i.picid = p.picid 
			INNER JOIN sisindigena2.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sisindigena2.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE SUBSTR(i.iuscpf,1,3)!='SIS' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND c.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='A' AND iusformacaoinicialorientador IS NULL";
	
	$total_n_gravados = $db->pegaUm($sql);
	
	if($total_n_gravados) return "É necessário gravar os registros selecionando os orientadores que obtiveram a Formação Inicial";
	else return true;
	
}

function verificarValidacaoEstruturaFormacao($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as total FROM sisindigena2.abrangencia a 
			INNER JOIN sisindigena2.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE e.uncid='".$dados['uncid']."'";
	
	$tot_abrangencia = $db->pegaUm($sql);
	
	$sql = "SELECT muncod FROM sisindigena2.estruturacurso e WHERE e.uncid='".$dados['uncid']."'";
	$existe_sede = $db->pegaUm($sql);
	
	if($tot_abrangencia == 0 || !$existe_sede) {
		$al = array("alert"=>"É necessário cadastrar as informações na aba Estrutura da Formação","location"=>"sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=estrutura_curso");
		alertlocation($al);
	}
	
}


function devolverProjetoUniversidade($uncid) {
	global $aed_devolver;
	$aed_devolver=true;
	return aprovarProjetoUniversidade($uncid);
}

function aprovarProjetoUniversidade($uncid) {
	global $db, $aed_devolver;
	
	if($aed_devolver) $aedid=AED_REPROVAR_CADASTRO_ORIENTADORES;
	else $aedid=AED_APROVAR_CADASTRO_ORIENTADORES;
	
	$sql = "SELECT DISTINCT pp.docid
			FROM sisindigena2.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sisindigena2.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN sisindigena2.pactoidadecerta pp ON pp.muncod = m.muncod 
			LEFT JOIN workflow.documento dd ON dd.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid 
			WHERE e.uncid='".$uncid."' AND a.esfera='M'";
	
	$docids = $db->carregarColuna($sql);
	
	if($docids) {
		foreach($docids as $docid) {
			wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
		}
	}
	
	$sql = "SELECT pp.docid
			FROM sisindigena2.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sisindigena2.estruturacurso e ON e.ecuid = a.ecuid
			LEFT JOIN sisindigena2.pactoidadecerta pp ON pp.estuf = m.estuf 
			LEFT JOIN workflow.documento dd ON dd.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid  
			WHERE e.uncid='".$uncid."' AND a.esfera='E'";
	
	$docids = $db->carregarColuna($sql);
	
	if($docids) {
		foreach($docids as $docid) {
			wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
		}
	}
	
	if(!$aed_devolver) {
		
		$sql = "UPDATE sisindigena2.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				u.uncid,
				i.iusd
				FROM sisindigena2.universidadecadastro u 
				INNER JOIN sisindigena2.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sisindigena2.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN sisindigena2.pactoidadecerta p ON p.muncod = a.muncod 
				INNER JOIN sisindigena2.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sisindigena2.tipoperfil t ON i.iusd = t.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
				WHERE u.uncid='".$uncid."' AND esfera='M' ) foo WHERE foo.iusd = id.iusd";
		
		$db->executar($sql);
		
		$sql = "UPDATE sisindigena2.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				distinct 
				u.uncid,
				i.iusd
				FROM sisindigena2.universidadecadastro u 
				INNER JOIN sisindigena2.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sisindigena2.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
				INNER JOIN sisindigena2.pactoidadecerta p ON p.estuf = m.estuf 
				INNER JOIN sisindigena2.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sisindigena2.tipoperfil t ON i.iusd = t.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
				WHERE u.uncid='".$uncid."' AND esfera='E' ) foo WHERE foo.iusd = id.iusd";
		
		$db->executar($sql);
		
		$db->commit();
		
		gerarVersaoProjetoUniversidade(array('uncid'=>$uncid));
	}
	
	return true;
}

function carregarVersoes($dados) {
	global $db;
	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"visualizarVersao(\''||v.vpnid||'\');\">' as acao,
				   u.usunome,
				   to_char(v.vpndata,'dd/mm/YYYY HH24:MI') as data
			FROM sisindigena2.versoesprojetouniversidade v 
			INNER JOIN seguranca.usuario u ON u.usucpf = v.usucpf
			WHERE uncid='".$dados['uncid']."' ORDER BY v.vpndata DESC";
	$cabecalho = array("&nbsp","Usuário que inseriu versão","Data da inserção");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function visualizarVersao($dados) {
	global $db;
	$sql = "SELECT vpnhtml FROM sisindigena2.versoesprojetouniversidade WHERE vpnid='".$dados['vpnid']."'";
	$html = $db->pegaUm($sql);
	
	echo $html;
	
}

function carregarOuvintesFormacaoInicial($dados) {
	global $db;
	
	$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"inserirOuvinte(\''||f.fioid||'\');\" > <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirOuvinte(\''||fioid||'\');\">' as acao, fiocpf, fionome, fioemail, m.estuf||' / '||m.mundescricao as municipio,
				   CASE WHEN f.fioesfera='M' THEN 'Municipal'
				   		WHEN f.fioesfera='E' THEN 'Estadual' END as esfera,
				   	i.iusnome as substituido 
			FROM sisindigena2.formacaoinicialouvintes f 
			LEFT JOIN territorios.municipio m ON m.muncod = f.muncod 
			LEFT JOIN sisindigena2.identificacaousuario i ON i.iusd = f.iusd
			WHERE f.uncid='".$dados['uncid']."' AND f.fiostatus='A'";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","UF / Município","Esfera","Irá substituir");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
}

function carregarUsuariosHabiliatadosTroca($dados) {
	global $db;
	$wh[] = "i.uncid='".$dados['uncid']."'";
	$wh[] = "(i.iusformacaoinicialorientador=false OR i.iusformacaoinicialorientador IS NULL)";
	if($dados['esfera']=='M') $wh[] = "i.picid IN(SELECT picid FROM sisindigena2.pactoidadecerta WHERE muncod='".$dados['muncod']."')";
	elseif($dados['esfera']=='E') $wh[] = "i.picid IN(SELECT picid FROM sisindigena2.pactoidadecerta WHERE estuf='".$dados['estuf']."')";
	
	$sql = "SELECT i.iusd as codigo, i.iusnome as descricao FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_ORIENTADORESTUDO."' 
			INNER JOIN sisindigena2.orientadorturma o ON o.iusd = i.iusd 
			WHERE ".(($wh)?implode(" AND ",$wh):"")." ORDER BY i.iusnome";
	
	$db->monta_combo('iusd', $sql, 'S', 'NÃO É SUBSTITUTO, SOMENTE OUVINTE', '', '', '', '', 'S', 'iusd', '', $dados['iusd']);
}


function atualizarOuvinte($dados) {
	global $db;
	$sql = "UPDATE sisindigena2.formacaoinicialouvintes SET  fiocpf='".str_replace(array(".","-"),array("",""),$dados['fiocpf'])."', 
            fionome='".$dados['fionome']."', 
            fioemail='".$dados['fioemail']."', 
            fioesfera='".$dados['fioesfera']."', 
            estuf='".$dados['estuf_endereco']."', 
            muncod='".$dados['muncod_endereco']."', 
            iusd=".(($dados['iusd'])?"'".$dados['iusd']."'":"NULL")." 
            WHERE fioid='".$dados['fioid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Ouvinte atualizado com sucesso.","javascript"=>"window.opener.carregarOuvintes();window.close();");
	alertlocation($al);
	
}


function inserirNucleoUniversidade($dados) {
	global $db;
	
	$picid = $db->pegaUm("SELECT picid FROM sisindigena2.nucleouniversidade WHERE uniid='".$dados['uniid']."' AND uncid='".$dados['uncid']."'");
	
	if(!$picid) {

		$sql = "INSERT INTO sisindigena2.nucleouniversidade(
	            picstatus, uniid, uncid)
	    		VALUES ('A', '".$dados['uniid']."', '".$dados['uncid']."');";
	
	} else {
		
		$sql = "UPDATE sisindigena2.nucleouniversidade SET picstatus='A', uniid='".$dados['uniid']."', uncid='".$dados['uncid']."' WHERE picid='".$picid."'";
		
	}
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Núcleo inserido com sucesso.","location"=>"sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=nucleos");
	alertlocation($al);
	
}

function excluirNucleoUniversidade($dados) {
	global $db;
	$sql = "UPDATE sisindigena2.nucleouniversidade SET picstatus='I' WHERE picid='".$dados['picid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Núcleo removido com sucesso","location"=>"sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=nucleos");
	alertlocation($al);
	
	
}






?>