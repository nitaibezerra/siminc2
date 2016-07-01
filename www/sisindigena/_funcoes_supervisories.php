<?
function sqlEquipeSupervisor($dados) {
	global $db;
	
	$sql = "(

			SELECT i.iusd, 
								i.iuscpf, 
								i.iusnome, 
								i.iusemailprincipal, 
								p.pflcod,
								p.pfldsc, 
								to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo, 
								(FLOOR((t.tpeatuacaofim - t.tpeatuacaoinicio)/30)+1) as nmeses, 
								(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
								(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=p.pflcod) as perfil 
						FROM sisindigena.identificacaousuario i
						INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_ORIENTADORESTUDO."'
						INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
						INNER JOIN sisindigena.orientadorturma ot ON ot.iusd = i.iusd 
						INNER JOIN sisindigena.turmas tu ON tu.turid = ot.turid
						WHERE tu.iusd='".$dados['iusd']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
								i.iuscpf, 
								i.iusnome, 
								i.iusemailprincipal, 
								p.pflcod,
								p.pfldsc, 
								to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo, 
								(FLOOR((t.tpeatuacaofim - t.tpeatuacaoinicio)/30)+1) as nmeses, 
								(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
								(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=p.pflcod) as perfil 
						FROM sisindigena.identificacaousuario i
						INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'
						INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
						INNER JOIN sisindigena.orientadorturma ot ON ot.iusd = i.iusd 
						INNER JOIN sisindigena.turmas tu ON tu.turid = ot.turid 
						INNER JOIN sisindigena.identificacaousuario i2 ON i2.iusd = tu.iusd 
						INNER JOIN sisindigena.tipoperfil t2 ON t2.iusd = i2.iusd AND t2.pflcod='".PFL_ORIENTADORESTUDO."' 
						INNER JOIN sisindigena.orientadorturma ot2 ON ot2.iusd = i2.iusd 
						INNER JOIN sisindigena.turmas tu2 ON tu2.turid = ot2.turid 
						WHERE tu2.iusd='".$dados['iusd']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			)
			";

	
	return $sql;
}

function carregarSupervisorIES($dados) {
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
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_SUPERVISORIES."'");
	
	if($infprof['iusd']) {
		
		$arrTurma = $db->pegaLinha("SELECT turid, docid FROM sisindigena.turmas WHERE iusd='".$infprof['iusd']."'");
		
		$dados['turid'] = $arrTurma['turid'];
		$docid          = $arrTurma['docid'];
		
		if(!$dados['turid']) {
			
			$docid = wf_cadastrarDocumento(TPD_COORDENADORIES,"SIS Indigena Supervisor IES ".$arr['picid']);
			
			$sql = "INSERT INTO sisindigena.turmas(
            		iusd, turdesc, turstatus, picid, docid)
    				VALUES ('".$infprof['iusd']."', 'Turma ".$infprof['iusd']."', 'A', '".$arr['picid']."', '".$docid."') RETURNING turid;";
			
			$dados['turid'] = $db->pegaUm($sql);
			
			$db->commit();
		}
	}
	
	
	
	$_SESSION['sisindigena']['supervisories'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
												   "turid" => $dados['turid'],
												   "docid" => $docid,
												   "iusnome" => $infprof['iusnome'],
												   "picid" => $arr['picid'],
												   "curid" => $arr['curid'], 
												   "uncid" => $arr['uncid'], 
												   "reiid" => $arr['reiid'], 
												   "estuf" => $arr['uniuf'], 
												   "iusd" => $infprof['iusd'],
												   "iuscpf" => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sisindigena.php?modulo=principal/supervisories/supervisories&acao=A&aba=principal");
		alertlocation($al);
	}
	
}


function inserirSupervisorIESGerenciamento($dados) {
	global $db;
	
	$sql = "SELECT iusd FROM sisindigena.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
 	$iusd = $db->pegaUm($sql);
	
 	if($iusd) {
 		$sql = "UPDATE sisindigena.identificacaousuario SET uncid='".$dados['uncid']."', picid='".$dados['picid']."', iusstatus='A', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$iusd."'";
 		$db->executar($sql);
 	} else {
     	$sql = "INSERT INTO sisindigena.identificacaousuario(
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
 		
 		$remetente = array("nome" => "SIMEC - MÓDULO SISINDÍGENA","email" => $dados['iusemailprincipal']);
 		$destinatario = $dados['iusemailprincipal'];
 		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
 		$assunto = "Cadastro no SIMEC - MÓDULO SISINDÍGENA";
 		$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISINDÍGENA. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
    	
    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_SUPERVISORIES."'");
    	
    if(!$existe_pfl) {
    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".PFL_SUPERVISORIES."');";
     	$db->executar($sql);
    }
   	
    $existe_usr = $db->pegaUm("select usucpf from sisindigena.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_SUPERVISORIES."' AND rpustatus='A' AND uncid='".$dados['uncid']."'".(($dados['picid'])?" AND picid='".$dados['picid']."'":""));
    
     if(!$existe_usr) {
     	
     		$sql = "DELETE FROM sisindigena.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND pflcod='".PFL_SUPERVISORIES."'";
     		$db->executar($sql);
     		
    		$sql = "INSERT INTO sisindigena.usuarioresponsabilidade(
            		pflcod, usucpf, rpustatus, rpudata_inc, uncid ".(($dados['picid'])?", picid":"").")
 			    VALUES ('".PFL_SUPERVISORIES."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."' ".(($dados['picid'])?",'".$dados['picid']."'":"").");";
    		$db->executar($sql);
     }
    
     $arrTp = $db->pegaLinha("SELECT
     							  t.pflcod, 
     							  p.pfldsc, 
     							  uni.unisigla || '/' || uni.uninome as descricao 
     						FROM sisindigena.tipoperfil t 
     						INNER JOIN seguranca.perfil p ON p.pflcod=t.pflcod 
     						INNER JOIN sisindigena.identificacaousuario i ON i.iusd=t.iusd 
     						INNER JOIN sisindigena.universidadecadastro unc ON unc.uncid=i.uncid 
     						LEFT JOIN sisindigena.universidade uni ON uni.uniid=unc.uniid 
     						WHERE t.iusd='".$iusd."'");
     
     $existe_tpf = $arrTp['pflcod'];
    
     if(!$existe_tpf) {
 		$sql = "INSERT INTO sisindigena.tipoperfil(
 		            iusd, pflcod, tpestatus)
 		    	VALUES ('".$iusd."', '".PFL_SUPERVISORIES."', 'A');";
     	$db->executar($sql);
     } else {
    	
     	if($existe_tpf!=PFL_SUPERVISORIES) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$arrTp['pfldsc'].",".$arrTp['descricao'].") no sistema e não pode ser cadastrado","location"=>"sisindigena.php?modulo=principal/supervisories/gerenciarsupervisories&acao=A&iusd=".$iusd."&picid=".$dados['picid']);
 			alertlocation($al);
     	}
    	
     }
    
     $db->commit();
			
 	$al = array("alert"=>"Supervisor IES inserido com sucesso","location"=>"sisindigena.php?modulo=principal/supervisories/gerenciarsupervisories&acao=A&iusd=".$iusd."&picid=".$dados['picid']);
 	alertlocation($al);
	
 }
 
 function certificarEquipe($dados) {
 	global $db;
 	
 	if($dados['certificacao']) {
 		foreach($dados['certificacao'] as $iusd => $opcao) {
 			
 			$cerid = $db->pegaUm("SELECT cerid FROM sisindigena.certificados WHERE iusd='{$iusd}'");
 			
 			if($cerid) {
 				
 				if($opcao) {
 					$sql = "UPDATE sisindigena.certificados SET cernota='".$dados['total'][$iusd]."', ceropcao='{$opcao}', cerdata=NOW(), cerusucpfres='".$_SESSION['usucpf']."' WHERE cerid='{$cerid}'";
 					$db->executar($sql);
 					$db->commit();
 				} else {
 					$sql = "DELETE FROM sisindigena.certificados WHERE cerid='{$cerid}'";
 					$db->executar($sql);
 					$db->commit();
 				}
 				
 			} else {
 				
 				if($opcao) {
 					
		 			$sql = "INSERT INTO sisindigena.certificados(iuscpf, cernota, ceropcao, cerdata, cerusucpfres, iusd)
				    		VALUES ((SELECT iuscpf FROM sisindigena.identificacaousuario WHERE iusd={$iusd}), '".$dados['total'][$iusd]."', '".$opcao."', NOW(), '".$_SESSION['usucpf']."', {$iusd});";
		 			
		 			$db->executar($sql);
		 			$db->commit();
	 			
 				}
 			
 			}
 			
 		}
 	}
 	
 	$al = array("alert"=>"Dados inseridos com sucesso","location"=>"sisindigena.php?modulo=principal/supervisories/supervisories&acao=A&aba=certificarequipe");
 	alertlocation($al);
 	
 	
 }



?>