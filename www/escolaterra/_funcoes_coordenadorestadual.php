<?

function inserirCoordenadorEstadual($dados) {
	global $db;
	
	$sql = "SELECT					  i.iusid,
									  i.ufpid,
	     							  t.pflcod, 
	     							  p.pfldsc, 
    							   	  es.estuf||' - '||es.estdescricao as descricao 
	     						FROM escolaterra.identificacaousuario i 
	     						LEFT JOIN escolaterra.tipoperfil t ON i.iusid = t.iusid 
	     						LEFT JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
	     						LEFT JOIN escolaterra.ufparticipantes c ON c.ufpid = i.ufpid 
	     						LEFT JOIN territorios.estado es ON es.estuf = c.estuf
	     						WHERE i.iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	$iusid     = $identificacaousuario['iusid'];
	$ufpid     = $identificacaousuario['ufpid'];
	$pflcod    = $identificacaousuario['pflcod'];
	$pfldsc    = $identificacaousuario['pfldsc'];
	$descricao = $identificacaousuario['descricao'];
	
	if($iusid) {
		
		if($pflcod) {
			if($ufpid!=$dados['ufpid']) {
	 			$al = array("alert"=>"Este CPF ja possui um perfil (".$pfldsc." , ".$descricao.") no sistema e não pode ser cadastrado","location"=>"escolaterra.php?modulo=principal/coordenadorestadual/gerenciarcoordenadorestadual&acao=A&ufpid=".$dados['ufpid']);
	 			alertlocation($al);
			}
		} else {
			$at_ufp = "ufpid='".$dados['ufpid']."', ";
		}
		
		$sql = "UPDATE escolaterra.identificacaousuario SET {$at_ufp} iusstatus='A', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusid='".$iusid."'";
		$db->executar($sql);
		
	} else {
		
    	$sql = "INSERT INTO escolaterra.identificacaousuario(
	            ufpid, 
	            iuscpf, 
	            iusnome, 
	            iusemailprincipal,  
	            iusdatainclusao, 
	            iusstatus)
			    VALUES ('".$dados['ufpid']."', 
			    		'".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 
			    		'".$dados['iusnome']."', 
			    		'".$dados['iusemailprincipal']."',  
			            NOW(), 
			            'A') returning iusid;";
    	
    	$iusid = $db->pegaUm($sql);
    	
	}
    	
    $existe_usu = $db->pegaUm("SELECT usucpf FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
    	
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
		
	$remetente = array("nome" => "SIMEC - MÓDULO ESCOLA DA TERRA","email" => $dados['iusemailprincipal']);
	$destinatario = $dados['iusemailprincipal'];
	$arrUsr = $db->pegaLinha("SELECT usunome, ususenha FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
	$usunome  = $arrUsr['usunome'];
	$ususenha = $arrUsr['ususenha'];
	$assunto = "Cadastro no SIMEC - MÓDULO ESCOLA DA TERRA";
	$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
	$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo ESCOLA DA TERRA. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
						  <p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
						  <p>Sua Senha de acesso é: %s</p>
						  <br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.",
		'Prezado(a)',
		$usunome,
		md5_decrypt_senha( $ususenha, '' )	
		);
	
	if(!strstr($_SERVER['HTTP_HOST'],"simec-local")){
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	}
	
    	
   	$existe_sis = $db->pegaUm("SELECT usucpf FROM seguranca.usuario_sistema WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND sisid='".SIS_ESCOLATERRA."'");
    	
   	if(!$existe_sis) {
    		
    	$sql = "INSERT INTO seguranca.usuario_sistema(
        	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
    			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', ".SIS_ESCOLATERRA.", 'A', NULL, NOW(), 'A');";
	    	
    	$db->executar($sql);
	    	
   	} else {
   		
   		if($dados['suscod']=="A") {
	    	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A', susdataultacesso=NOW() WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND sisid='".SIS_ESCOLATERRA."'";
	    	$db->executar($sql);
   		}
   		
   	}
    	
   	$existe_pfl = $db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND pflcod='".PFL_COORDENADORESTADUAL."'");
    	
   	if(!$existe_pfl) {
   		$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".PFL_COORDENADORESTADUAL."');";
   		$db->executar($sql);
   	}
   	
    $existe_usr = $db->pegaUm("SELECT usucpf FROM escolaterra.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND pflcod='".PFL_COORDENADORESTADUAL."' AND rpustatus='A' AND  ufpid='".$dados['ufpid']."'");
    
    if(!$existe_usr) {
    	
   		$sql = "INSERT INTO escolaterra.usuarioresponsabilidade(
           		pflcod, usucpf, rpustatus, rpudata_inc, ufpid)
			    VALUES ('".PFL_COORDENADORESTADUAL."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), ".(($adesao['ufpid'])?"'".$adesao['ufpid']."'":"NULL").");";
   		
   		$db->executar($sql);
   		
    }
     
    $existe_tpf = $pflcod;
    
    if(!$existe_tpf) {
    	
		$sql = "INSERT INTO escolaterra.tipoperfil(
 		            iusid, pflcod, tpestatus)
 		    	VALUES ('".$iusid."', '".PFL_COORDENADORESTADUAL."', 'A');";
     	$db->executar($sql);
     	
    } else {
    	
     	if($existe_tpf != PFL_COORDENADORESTADUAL) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$pfldsc." , ".$descricao.") no sistema e não pode ser cadastrado","location"=>"escolaterra.php?modulo=principal/coordenadorestadual/gerenciarcoordenadorestadual&acao=A&ufpid=".$dados['ufpid']);
 			alertlocation($al);
     	}
    	
    }
    

    if($_FILES['arquivo']['error']==0) {
    	
    	$db->executar("DELETE FROM escolaterra.identificacaousuariodocumentos WHERE iusid='".$iusid."' AND iudtipo='".$dados['tipodocumentoselecao']."'");
    	
	   	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	    $campos = array("iusid" => "'".$iusid."'","iudtipo" => "'C'");
	    $file = new FilesSimec( "identificacaousuariodocumentos", $campos, "escolaterra" );
	    $file->setUpload( NULL, "arquivo" );
	    
    }
    
    
    $db->commit();
	
    if(!$dados['naoredirecionar']) {
		$al = array("alert"=>"Coordenador Estadual inserido com sucesso","javascript"=>"window.opener.location='escolaterra.php?modulo=principal/coordenadorestadual/listacoordenadorestadual&acao=A';window.location='escolaterra.php?modulo=principal/coordenadorestadual/gerenciarcoordenadorestadual&acao=A&ufpid=".$dados['ufpid']."';");
		alertlocation($al);
    }	
}

function carregarCoordenadorEstadual($dados) {
	global $db;
	
	$sql = "SELECT i.iusid, i.iusnome, i.ufpid, e.estuf || ' / ' || e.estdescricao as descricao, i.iuscpf 
			FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN escolaterra.ufparticipantes u ON u.ufpid = i.ufpid 
			INNER JOIN territorios.estado e ON e.estuf = u.estuf 
			WHERE i.ufpid = '".$dados['ufpid']."' AND t.pflcod='".PFL_COORDENADORESTADUAL."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	
	$_SESSION['escolaterra']['coordenadorestadual']['iusid']     = $identificacaousuario['iusid'];
	$_SESSION['escolaterra']['coordenadorestadual']['iusnome']   = $identificacaousuario['iusnome'];
	$_SESSION['escolaterra']['coordenadorestadual']['iuscpf']    = $identificacaousuario['iuscpf'];
	$_SESSION['escolaterra']['coordenadorestadual']['ufpid']     = $identificacaousuario['ufpid'];
	$_SESSION['escolaterra']['coordenadorestadual']['descricao'] = $identificacaousuario['descricao'];
	

	if($dados['direcionar']) {
		$al = array("location"=>"escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function sqlEquipeCoordenadorEstadual($dados) {
	global $db;
	
	$sql = "SELECT 	   i.iusid as iusid,
					   i.iuscpf as iuscpf,
					   i.iusnome as iusnome,
					   i.iusemailprincipal as iusemailprincipal,
					   pp.pflcod,
					   pp.pfldsc, 
				   	   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_ESCOLATERRA.") as status,
				   	   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_TUTOR.") as perfil				
		   	   FROM escolaterra.identificacaousuario i 
				INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_TUTOR." 
				INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod  
				INNER JOIN escolaterra.turmaidusuario u ON u.iusid = i.iusid 
				INNER JOIN escolaterra.turmas tu ON tu.turid = u.turid
				WHERE tu.iusid='".$dados['iusid']."'";
	
	return $sql;
}

function carregarListaProfessores($dados) {
	global $db;
	
	echo "<p align=\"center\"><b>Lista de Professores</b></p>";
	
	$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as cpf,
				   i.iusnome as nome,
				   i.iusemailprincipal as email,
				   COALESCE(m.estuf||' - '||m.mundescricao,'Não informado') as municipio
			FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_PROFESSOR." 
			INNER JOIN escolaterra.turmaidusuario u ON u.iusid = i.iusid 
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			WHERE u.turid='".$dados['turid']."'";
	
	$cabecalho = array("CPF","Nome","Email","Município");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	
}

function habilitarAbaValidacaoProfessores($dados) {
	global $db;
	
	$esdid = $db->pegaUm("SELECT d.esdid FROM escolaterra.turmas t 
						 INNER JOIN workflow.documento d ON d.docid = t.docid 
						 WHERE t.iusid='".$_SESSION['escolaterra']['coordenadorestadual']['iusid']."'");
	
	if($esdid == ESD_VALIDADO) return true;
	else return false;
	
}

function downloadDocumentoSelecao($dados) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "identificacaousuariodocumentos", $campos, "escolaterra" );
    $file->getDownloadArquivo( $dados['arqid'] );
	
}

function gravarRelatorioAcompanhamentoCoordenadorEstadual($dados) {
	global $db;
	
	if(!$dados['iusid']) $dados['iusid'] = $_SESSION['escolaterra']['coordenadorestadual']['iusid'];
	
	if($dados['raeparticipouencontro']!='S') {
		
		unset($dados['raedataencontro'],
			  $dados['raeparticipacaoencontro'],
			  $dados['raeatividadesocorridasencontro'],
			  $dados['raesatisfacaoencontro'],
			  $dados['raedestaquespositivosencontro'],
		  	  $dados['raedestaquesnegativosencontro'],
			  $dados['raeaperfeicoarencontro']);
		
	}
	
	$sql = "UPDATE escolaterra.relatorioacompanhamentocoordenadorestadual 
			SET 
			raerealizouacaoescolaterra=".(($dados['raerealizouacaoescolaterra'])?"'".$dados['raerealizouacaoescolaterra']."'":"NULL").", raetempoformacao=".(($dados['raetempoformacao'])?"'".$dados['raetempoformacao']."'":"NULL").", 
       		raeprincipaisatividades=".(($dados['raeprincipaisatividades'])?"'".substr($dados['raeprincipaisatividades'],0,2000)."'":"NULL").", raeparticipouencontro=".(($dados['raeparticipouencontro'])?"'".$dados['raeparticipouencontro']."'":"NULL").", 
       		raedataencontro=".(($dados['raedataencontro'])?"'".$dados['raedataencontro']."'":"NULL").", 
       		raeparticipacaoencontro=".(($dados['raeparticipacaoencontro'])?"'".$dados['raeparticipacaoencontro']."'":"NULL").", raeatividadesocorridasencontro=".(($dados['raeatividadesocorridasencontro'])?"'".substr($dados['raeatividadesocorridasencontro'],0,2000)."'":"NULL").", 
	        raesatisfacaoencontro=".(($dados['raesatisfacaoencontro'])?"'".$dados['raesatisfacaoencontro']."'":"NULL").", raedestaquespositivosencontro=".(($dados['raedestaquespositivosencontro'])?"'".substr($dados['raedestaquespositivosencontro'],0,2000)."'":"NULL").", raedestaquesnegativosencontro=".(($dados['raedestaquesnegativosencontro'])?"'".substr($dados['raedestaquesnegativosencontro'],0,2000)."'":"NULL").", 
    	    raeaperfeicoarencontro=".(($dados['raeaperfeicoarencontro'])?"'".substr(addslashes($dados['raeaperfeicoarencontro']),0,2000)."'":"NULL")."
			WHERE iusid='".$dados['iusid']."' AND fpbid='".$dados['fpbid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Relatório de acompanhamento gravado com sucesso","location"=>"escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadualexecucao&acao=A&aba=relatorioacompanhamento&fpbid=".$dados['fpbid']);
	alertlocation($al);

}

?>