<?

function carregarDadosGestorEscola($dados) {
	global $db;
	
	$sql = "SELECT * FROM sismedio.listaescolasensinomedio l 
			LEFT JOIN seguranca.usuario u ON u.usucpf = l.lemcpfgestor 
			WHERE lemcodigoinep='".$dados['lemcodigoinep']."'";
	
	$listaescolasensinomedio = $db->pegaLinha($sql);
	
	return $listaescolasensinomedio;
}

function removerGestorEscola($dados) {
	global $db;
	
	$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$dados['usucpf']."' AND pflcod='".PFL_GESTORESCOLA."'";
	$db->executar($sql);
	
	$pflsismedio = $db->pegaUm("SELECT COUNT(*) as n 
								 FROM seguranca.perfilusuario pu 
								 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod 
								 WHERE pu.usucpf='".$dados['usucpf']."' AND p.sisid='".SIS_MEDIO."'");
	
	if(!$pflsismedio) {
		$sql = "DELETE FROM seguranca.usuario_sistema WHERE usucpf='".$dados['usucpf']."' AND sisid='".SIS_MEDIO."'";
		$db->executar($sql);
	}
	
	$sql = "UPDATE sismedio.listaescolasensinomedio SET lemcpfgestor=NULL, lemnomegestor=NULL, lememailgestor=NULL WHERE lemcodigoinep='".$dados['lemcodigoinep']."'";
	$db->executar($sql);
	
	$db->commit();
	
	$al = array("alert"=>"Gestor da Escola removido com sucesso","javascript"=>"window.opener.location=window.opener.location");
	alertlocation($al);
	
}

function inserirGestorEscolaGerenciamento($dados) {
	global $db;
	 
	$existe_usu = $db->pegaUm("select usucpf from seguranca.usuario where usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'");
	 
	if(!$existe_usu) {
		 
		$sql = "INSERT INTO seguranca.usuario(
             	usucpf, usunome, usuemail, usustatus, ususenha, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['usucpf'])."', '".$dados['usunome']."', '".$dados['usuemail']."', 'A', '".md5_encrypt_senha("simecdti", '')."', 'A');";
		
		$db->executar($sql);
		 
	} else {
		 
		if($dados['reenviarsenha']=="S") {
			$cl_senha = ", ususenha='".md5_encrypt_senha( "simecdti", '' )."', usuchaveativacao=false";
		}
		
		$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', usuemail='".$dados['usuemail']."' {$cl_senha} WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'";
		$db->executar($sql);
	}
	
	$db->commit();
	
	if($dados['reenviarsenha']=="S") {
			
		$remetente = array("nome" => "SIMEC - MÓDULO SISMÉDIO","email" => $dados['usuemail']);
		$destinatario = $dados['usuemail'];
		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'");
		$assunto = "Cadastro no SIMEC - MÓDULO SISMÉDIO";
		$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo sismedio. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
	 
	$existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."' and sisid='".SIS_MEDIO."'");
	 
	if(!$existe_sis) {
	
		$sql = "INSERT INTO seguranca.usuario_sistema(
         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['usucpf'])."', ".SIS_MEDIO.", 'A', NULL, NOW(), 'A');";
	
		$db->executar($sql);
	
	} else {
		if($dados['suscod']=="A") {
			$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."' AND sisid='".SIS_MEDIO."'";
			$db->executar($sql);
		}
	}
	
	$db->commit();
	 
	$existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."' and pflcod='".PFL_GESTORESCOLA."'");
	 
	if(!$existe_pfl) {
		$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['usucpf'])."', '".PFL_GESTORESCOLA."');";
		$db->executar($sql);
	}
	
	$db->executar("UPDATE sismedio.listaescolasensinomedio SET lemcpfgestor='".str_replace(array(".","-"),array(""),$dados['usucpf'])."', lemnomegestor='".addslashes($dados['usunome'])."', lememailgestor='".$dados['usuemail']."' WHERE lemcodigoinep='".$dados['lemcodigoinep']."'");
	
	$db->commit();

	if(!$dados['naoredirecionar']) {
		$al = array("alert"=>"Gestor da Escola inserido com sucesso","javascript"=>"window.opener.location=window.opener.location");
		alertlocation($al);
	}
	
	
}



function carregarGestorEscola($dados) {
	global $db;
	
	$infprof = $db->pegaLinha("SELECT 
							   		lemuf||'/'||lemmundsc||' - '||lemnomeescola as descricao,
									lemdoctotal,
									u.usucpf,
									u.usunome,
									lemcodigoinep,
									l.docid  
							   FROM seguranca.usuario u 
							   INNER JOIN sismedio.listaescolasensinomedio l ON u.usucpf = l.lemcpfgestor 
							   WHERE l.lemcpfgestor='".$dados['cpf']."' AND l.lemcodigoinep='".$dados['codigoinep']."'");
	
	if(!$infprof['docid']) {
		$infprof['docid'] = wf_cadastrarDocumento(TPD_FLUXOESCOLA,"Gestor da Escola ( ".$infprof['descricao']." )");
		
		$db->executar("UPDATE sismedio.listaescolasensinomedio SET docid='".$infprof['docid']."' WHERE lemcpfgestor='".$dados['cpf']."' AND lemcodigoinep='".$dados['codigoinep']."'");
		$db->commit();
	}
	
	$arrEscolas = $db->carregar("SELECT
									u.usucpf,
									lemuf||'/'||lemmundsc||' - '||lemnomeescola as descricao,
									lemcodigoinep
							   	FROM seguranca.usuario u
							   	INNER JOIN sismedio.listaescolasensinomedio l ON u.usucpf = l.lemcpfgestor
							   	WHERE l.lemcpfgestor='".$dados['cpf']."' ORDER BY lemcodigoinep");
	
	if($arrEscolas[0] && count($arrEscolas) > 1) {
		$html  = "<select class=\"CampoEstilo\" style=\"width: auto\" onchange=\"acessarDiretor('".$dados['cpf']."',this.value)\">";
		foreach($arrEscolas as $arrE) {
			$html .= "<option value=\"".$arrE['lemcodigoinep']."\" ".(($arrE['lemcodigoinep']==$dados['codigoinep'])?"selected":"").">".$arrE['descricao']."</option>";
		}
		$html .= "</select>";
		
		$infprof['descricao'] = $html;
	}
	
	

	$_SESSION['sismedio']['gestorescola'] = array("descricao"      => $infprof['descricao']." ( ".$infprof['usunome']." )",
												   "lemdoctotal"   => $infprof['lemdoctotal'], 
												   "lemcodigoinep" => $infprof['lemcodigoinep'],
												   "usucpf"        => $infprof['usucpf'],
												   "docid"         => $infprof['docid']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sismedio.php?modulo=principal/escola/escola&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function condicaoEscolaEnviarAnalise() {
	global $db;
	
	$professores = carregarDadosIdentificacaoUsuario(array("iuscodigoinep" => $_SESSION['sismedio']['gestorescola']['lemcodigoinep'],"pflcod" => array(PFL_PROFESSORALFABETIZADOR,PFL_COORDENADORPEDAGOGICO)));
	
	if(count($professores)==0) {
		return 'Nenhum professor foi cadastrado.';
	}
	
	return true;
}


function inserirProfessoresAlfabetizadores($dados) {
	global $db;

	// validação da sessão
	if(!$_SESSION['sismedio']['gestorescola']['lemcodigoinep']) {
		
		die("<script>
				alert('As informações não foram gravadas. Houve perdas de informações internas. Você será direcionado para a tela principal.');
				window.location='sismedio.php?modulo=inicio&acao=C';
			</script>");
		
	}
	
	//$uncid = $db->pegaUm("SELECT i.uncid FROM sispacto.turmas t INNER JOIN sispacto.identificacaousuario i ON i.iusd=t.iusd WHERE t.turid='".$dados['turid']."'");
	
	$db->executar("DELETE FROM sismedio.definicaoorientadoresestudo WHERE doecodigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."'");

	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
				
			unset($erro);
			if(!$dados['nome'][$cpf]) $erro[] = "Nome em branco";
			if(!$dados['email'][$cpf]) $erro[] = "Email em branco";
				
			if($erro) {
				$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erro),"location"=>$dados['goto']);
				alertlocation($al);
			}
			
			if($dados['pflcod'][$cpf] == PFL_COORDENADORPEDAGOGICO) {
				$dados['tipo'][$cpf] = 'censo';
			} else {
				$palid = $db->pegaUm("SELECT palid FROM sismedio.professoresalfabetizadores WHERE cpf='".str_replace(array(".","-"),array(""),$cpf)."'");
				if($palid) $dados['tipo'][$cpf] = 'censo';
			}
				
			$iusd = $db->pegaUm("SELECT iusd FROM sismedio.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'");
				
			if($iusd) {

				$sql = "UPDATE sismedio.identificacaousuario SET
						iuscodigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."',
						iusnome='".$dados['nome'][$cpf]."',
						iusemailprincipal='".$dados['email'][$cpf]."',
						iustipoprofessor='".$dados['tipo'][$cpf]."',
						iusstatus='A'
						WHERE iusd='".$iusd."'";
				
				$db->executar($sql);

			} else {
					
				$sql = "INSERT INTO sismedio.identificacaousuario(
			            iuscodigoinep, iuscpf, iusnome, iusemailprincipal, iusdatainclusao, iustipoprofessor)
					    VALUES ('".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."',
					    		'".str_replace(array(".","-"),array(""),$cpf)."',
					    		'".$dados['nome'][$cpf]."',
					    		'".$dados['email'][$cpf]."',
					    		NOW(),
					    		'".$dados['tipo'][$cpf]."') RETURNING iusd;";

				$iusd = $db->pegaUm($sql);
					
			}
				
			$sql = "SELECT p.pfldsc, p.pflcod, i.iuscodigoinep, lemnomeescola ||'( '||lemuf||' - '||lemmundsc||' )' as descricao FROM sismedio.tipoperfil t
					INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
					INNER JOIN sismedio.identificacaousuario i ON i.iusd = t.iusd
					INNER JOIN sismedio.listaescolasensinomedio le ON le.lemcodigoinep::bigint = i.iuscodigoinep
					WHERE t.iusd='".$iusd."'";
			$arr = $db->pegaLinha($sql);
				
			$pfldsc     = $arr['pfldsc'];
			$descricao  = $arr['descricao'];
			$codigoinep = $arr['iuscodigoinep'];
			$codperfil  = $arr['pflcod'];
				
			if($pfldsc && ($codigoinep != $_SESSION['sismedio']['gestorescola']['lemcodigoinep'])) {
				$db->rollback();
				$al = array("alert"=>"Caso queira indicar este CPF(".$dados['nome'][$cpf].") como Professor, é necessário antes remove-lo do perfil(".$pfldsc.",".$descricao.").","location"=>"sismedio.php?modulo=principal/escola/escola&acao=A&aba=definirprofessores");
				alertlocation($al);
			}
			
			$tpeid = $db->pegaUm("SELECT tpeid FROM sismedio.tipoperfil WHERE iusd='".$iusd."'");
			
			if($codperfil != $dados['pflcod'][$cpf] && !$tpeid) {
				
				$sql = "INSERT INTO sismedio.tipoperfil(iusd, pflcod, tpestatus)
	    				VALUES ('".$iusd."', '".$dados['pflcod'][$cpf]."', 'A');";
				$db->executar($sql);
				
			}
				
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Professores e Coordenadores Pedagógicos gravados com sucesso.","location"=>$dados['goto']);
	alertlocation($al);

}

function definirOrientadoresEstudo($dados) {
	global $db;
	
	$db->executar("DELETE FROM sismedio.definicaoorientadoresestudo WHERE doecodigoinep='".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."'");
	
	if($dados['orientador']) {
		foreach($dados['orientador'] as $iusd) {
			
			$db->executar("DELETE FROM sismedio.definicaoorientadoresestudo WHERE iusd='".$iusd."'");
			
			$sql = "INSERT INTO sismedio.definicaoorientadoresestudo(
            		iusd, doedtinsercao, doecpf, doecodigoinep)
    				VALUES ('".$iusd."', NOW(), '".$_SESSION['usucpf']."', '".$_SESSION['sismedio']['gestorescola']['lemcodigoinep']."');";
			
			$db->executar($sql);
			
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Orientadores de Estudo definidos com sucesso.","location"=>$dados['goto']);
	alertlocation($al);
	
}

function calcularOrientadoresEstudo($dados) {
	global $db;

	return ceil($dados['numprofessores']/35);

}

function removerProfessorAlfabetizador($dados) {
	global $db;

	$sql = "SELECT pboid FROM sismedio.pagamentobolsista WHERE iusd='".$dados['iusd']."'";
	$pboid = $db->pegaUm($sql);

	if($pboid) {
		echo "Não é possível remover o Professor Alfabetizador, pois este ja recebeu uma Bolsa de Estudo pelo SISMédio. Somente será permitido substituições.";
		exit;
	}

	$sql = "DELETE FROM sismedio.tipoperfil WHERE iusd='".$dados['iusd']."' AND pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."')";
	$db->executar($sql);
	$sql = "DELETE FROM sismedio.orientadorturma WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$sql = "UPDATE sismedio.identificacaousuario SET iusstatus='I' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();

}


function atualizarDadosGestorEscola($dados) {
	global $db;
	
	$sql = "UPDATE seguranca.usuario SET 
			usudatanascimento='".formata_data_sql($dados['usudatanascimento'])."',
			muncod='".$dados['muncod']."',
			ususexo='".$dados['ususexo']."',
			usufoneddd='".$dados['usufoneddd']."',
			usufonenum='".$dados['usufonenum']."',
			usuemail='".$dados['usuemail']."' 
			WHERE usucpf='".str_replace(array(".","-"),array("",""),$dados['usucpf'])."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Dados gravados com sucesso.","location"=>$dados['goto']);
	alertlocation($al);
	
	
}

function carregarCadastramentoEscola($dados) {
	global $db;
	
	echo "<p align=center><b>Professores/ Coordenadores Pedagógicos</b></p>";
	
	$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, i.iusemailprincipal, p.pfldsc FROM sismedio.identificacaousuario i
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
				WHERE i.iusstatus='A' AND t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."') AND i.iuscodigoinep='".$dados['lemcodigoinep']."'";
	
	$cabecalho = array("CPF","Nome","Email","Perfil");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	echo "<p align=center><b>Orientadores de Estudo</b></p>";
	
	$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, i.iusemailprincipal, p.pfldsc 
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sismedio.definicaoorientadoresestudo d ON d.iusd = i.iusd 
			WHERE i.iusstatus='A' AND t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."') AND i.iuscodigoinep='".$dados['lemcodigoinep']."' AND d.doecodigoinep='".$dados['lemcodigoinep']."'";
	
	$cabecalho = array("CPF","Nome","Email","Perfil");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	$docid = $db->pegaUm("SELECT docid FROM sismedio.listaescolasensinomedio WHERE lemcodigoinep='{$dados['lemcodigoinep']}'");
	
	echo "<br>
		  <table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" width=\"100%\">
		  <tr>
		  	<td><font style=font-size:x-small;><b>Termo de validação</b></font></td>
		  </tr>
		  <tr>
		  	<td>
			
			<p><font style=font-size:x-small;>Atesto que TODOS os membros cadastrados nesta escola estão aptos a participar do Pacto Nacional pelo Fortalecimento do Ensino Médio, pertencem à rede estadual de ensino público e atendem aos critérios estabelecidos na Resolução CD/FNDE Nº 51, de 11 de dezembro de 2013, a saber:</font></p>

			<p><font style=font-size:x-small;>No caso do(s) Orientador(es) de Estudo:<br/><br/>
				I – é (são) professor(es) do ensino médio, coordenador(es) pedagógico(s) do ensino médio ou equivalente na rede pública de ensino;<br/>
				II – é (são) formado(s) em Pedagogia ou possui(em) Licenciatura;<br/>
				III – atua(m) há, no mínimo, dois anos no ensino médio, como professor(es) ou coordenador(es) pedagógico(s) ou possui(em) experiência comprovada na formação de professores de ensino médio;<br/>
				IV – tem disponibilidade para dedicar-se ao curso de formação e encontros com o formador regional e ao trabalho de formação na escola, correspondente a 20 horas semanais; e<br/>
				V – consta(m) do Censo Escolar de 2013, exceto aqueles já registrados no SisMédio, sobre os quais asseguramos que pertencem à rede de ensino e cumpre(m) os demais critérios.</font></p>
			<br/>
			<p><font style=font-size:x-small;>No caso dos Professores e Coordenadores Pedagógicos:<br/><br/>
			I - atuam como docentes em sala de aula no ensino médio ou coordenadores pedagógicos no ensino médio, em escola da rede estadual, em efetivo exercício em 2014;<br/>
			II - constam no Censo Escolar de 2013, exceto aqueles já registrados no SisMédio, sobre os quais asseguramos que pertencem à rede de ensino e cumprem os demais critérios.</font></p>
			
			</td>
		  </tr>
		  <tr>
		  	<td><input type=button value=\"Validar Cadastramento\" onclick=\"validarCadastramentoEscola(".$docid.");\"></td>
		  </tr>
		  </table>";
	
	
}

function validarCadastramentoEscola($dados) {
	global $db;
	
	wf_alterarEstado( $dados['docid'], AED_VALIDAR_CADASTRAMENTO_ESCOLA, '', array());
}


?>