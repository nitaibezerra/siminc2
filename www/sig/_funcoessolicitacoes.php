<?


function carregardadosmenusigsolicitacoes() {
	global $permissoes;
	// monta menu padrão contendo informações sobre as entidades, personalizado
	if($permissoes['solicitante']) {
		$menu[] = array("id" => 1, "descricao" => "Solicitações", "link" => "/sig/sig.php?modulo=principal/solicitacao&acao=A".(($_REQUEST['solid'])?"&solid=".$_REQUEST['solid']:""));
	}
	
	if($_REQUEST['modulo'] == "principal/encaminhamento") {
		$menu[] = array("id" => 2, "descricao" => "Encaminhamento", "link" => "/sig/sig.php?modulo=principal/encaminhamento&acao=A".(($_REQUEST['solid'])?"&solid=".$_REQUEST['solid']:"").(($_REQUEST['encid'])?"&encid=".$_REQUEST['encid']:""));
	}
	
	if($_REQUEST['modulo'] == "principal/resposta") {
		$menu[] = array("id" => 2, "descricao" => "Resposta", "link" => "/sig/sig.php?modulo=principal/resposta&acao=A&solid=".$_REQUEST['solid']);
	}
	
	
	if($permissoes['atendente']) {
		$menu[] = array("id" => 3, "descricao" => "Atendimento", "link" => "/sig/sig.php?modulo=principal/atendimento&acao=A".(($_REQUEST['ecaid'])?"&ecaid=".$_REQUEST['ecaid']:""));
	}
	return $menu;
}

function inserirsolicitacao($dados) {
	global $db;
	
	$sql = "INSERT INTO sig.solicitacao(tipid, 
										stsid, 
										soldesc, 
										solprazo, 
										usucpfsol, 
										soldatainclusao, 
										solporcentoexec, 
										solstatus)
										 
			VALUES ('".$dados['tipid']."', 
					'".SITSOL_NINICIADO."', 
					'".$dados['soldesc']."', 
					'".formata_data_sql($dados['solprazodata'])." ".$dados['solprazohora']."', 
					'".$_SESSION['usucpf']."', 
					NOW(), 
					0, 
					'A') RETURNING solid;";
	
	$solid = $db->pegaUm($sql);
	
	/*
	 * Enviando SMS
	 */
	$dadosusus[''] = '556181149953';
	$dadosusus[''] = '556178132238';
	
	require_once('../webservice/painel/nusoap.php');
	$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
	$err = $client->getError();
	if ($err) {
	    die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
	}
	$sql = "SELECT * FROM seguranca.perfilusuario pfu 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = pfu.usucpf 
			WHERE pfu.pflcod='".PERFIL_ENCAMINHADOR."'";
	$perfilusuario = $db->carregar($sql);
	if($perfilusuario[0]) {
		foreach($perfilusuario as $pfu) {
			if($dadosusus[$pfu['usucpf']]) {
				$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => SIGLA_SISTEMA, 'target' => $pfu['usucelfone'], 'body' => 'Uma nova solicitação foi inserido no SIMEC, favor encaminhar para providências', 'ID' => substr($pfu['usucpf'],0,6).date("Ymdhis")));
			}
		}
	}
	/*
	 * Enviando SMS
	 */
	
	/*
	 * Enviando email
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA;
	$mensagem->From 		= $_SESSION['email_sistema'];
	
	if($perfilusuario[0]) {
		foreach($perfilusuario as $pfu) {
			$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
		}
	}
	
	$mensagem->Subject = "Nova solicitação";
	$mensagem->Body = "Uma nova solicitação foi criada";
	$mensagem->IsHTML( true );
	$mensagem->Send();
	/*
	 * FIM
	 * Enviando email
	 */
	
	
	
	$db->commit();
	
	redirecionarPagina("Solicitação cadatrada com sucesso", "sig.php?modulo=principal/solicitacao&acao=A");
	
}

function atualizarsolicitacao($dados) {
	global $db;
	
	$sql = "UPDATE sig.solicitacao
   			SET tipid='".$dados['tipid']."', soldesc='".$dados['soldesc']."', solprazo='".formata_data_sql($dados['solprazodata'])." ".$dados['solprazohora']."'  
 			WHERE solid='".$dados['solid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	redirecionarPagina("Solicitação atualizada com sucesso", "sig.php?modulo=principal/solicitacao&acao=A&visetapa=solicitacao");
}

function redirecionarPagina($msg, $link) {
	
	echo "<script>
			alert('".$msg."');
			window.location='".$link."';
		  </script>";
	
	exit;
}

function atualizarencaminhamento($dados) {
	global $db;
	$sql = "UPDATE sig.encaminhamento SET encdestinatario='".$dados['pessoas']."', 
										  encdesc='".$dados['encdesc']."', 
										  encprazo='".formata_data_sql($dados['encprazodata'])." ".$dados['encprazohora']."' 
 			WHERE encid='".$dados['encid']."'";
	$db->executar($sql);
	$db->commit();
	
	redirecionarPagina("Encaminhamento atualizado com sucesso", "sig.php?modulo=principal/encaminhamento&acao=A&encid=".$dados['encid']);
}

function inserirencaminhamento($dados) {
	global $db;
	
	$pessoas = explode(",", strtolower($dados['pessoas']));
	$sql = "SELECT usucpf FROM seguranca.usuario WHERE LOWER(usunome) || ' <' || LOWER(usuemail) || '>' IN ('".implode("','", $pessoas)."')";
	$encaminhados = $db->carregar($sql);
	
	if($encaminhados[0]) {
		foreach($encaminhados as $encam) {
			$sql = "SELECT uss.sisid, pfu.pflcod FROM seguranca.usuario usu 
					LEFT JOIN seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf AND pfu.pflcod='".PERFIL_ATENDENTE."' 
					LEFT JOIN seguranca.usuario_sistema uss ON uss.usucpf = usu.usucpf AND uss.sisid='".$_SESSION['sisid']."' 
					WHERE usu.usucpf='".$encam['usucpf']."'";
			
			$dadosen = $db->pegaLinha($sql);
			
			if(!$dadosen['pflcod']) {
				$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".$encam['usucpf']."', '".PERFIL_ATENDENTE."');";
				$db->executar($sql);
			}
			
			if(!$dadosen['sisid']) {
				$sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod) 
						VALUES ('".$encam['usucpf']."', '".$_SESSION['sisid']."', 'A', '".PERFIL_ATENDENTE."', NULL, 'A');";
				$db->executar($sql);
			}
		}
	}
	
	$sql = "INSERT INTO sig.encaminhamento(solid, encdestinatario, encdesc, encprazo, usucpfenc, encdataenc) 
            VALUES ('".$dados['solid']."', '".$dados['pessoas']."', '".$dados['encdesc']."', '".formata_data_sql($dados['encprazodata'])." ".$dados['encprazohora']."', '".$_SESSION['usucpf']."', NOW()) RETURNING encid;";
	
	$encid = $db->pegaUm($sql);
	
	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];
	
	if($arquivo["name"]) {
	
		// BUG DO IE
		// O type do arquivo vem como image/pjpeg
		if($arquivo["type"] == 'image/pjpeg') {
			$arquivo["type"] = 'image/jpeg';
		}
		
		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
		values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$dados["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
		$arqid = $db->pegaUm($sql);
	
		//Insere o registro na tabela obras.arquivosobra
		$sql = "INSERT INTO sig.anexo(arqid, anxdsc, encid) VALUES ('".$arqid."', '".current(explode(".", $arquivo["name"]))."', '".$encid."');";
		$db->executar($sql);
		
		if(!is_dir('../../arquivos/sig/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/sig/'.floor($arqid/1000), 0777);
		}
		
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
		
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			$db->rollback();
			echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
			exit;
		}
	
	}
	
	$db->executar("UPDATE sig.solicitacao SET stsid='".SITSOL_EMATENDIMENTO."' WHERE solid='".$dados['solid']."'");


	/*
	 * Enviando SMS
	 */
	$dadosusus[''] = '556181149953';
	$dadosusus[''] = '556178132238';
	
	require_once('../webservice/painel/nusoap.php');
	$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
	$err = $client->getError();
	if ($err) {
	    die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
	}
	if($encaminhados[0]) {
		foreach($encaminhados as $encam) {
			$sql = "INSERT INTO sig.encaminhados(encid, usucpfencaminhado)
	    			VALUES ('".$encid."', '".$encam['usucpf']."');";
			$db->executar($sql);
			if($dadosusus[$encam['usucpf']]) {
				$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => SIGLA_SISTEMA,	'target' => $dadosusus[$encam['usucpf']], 'body' => 'Uma solicitação foi encaminhada para você, favor tomar providências', 'ID' => substr($encam['usucpf'],0,6).date("Ymdhis")));
			}
		}
	}
	/*
	 * Enviando SMS
	 */
	
	/*
	 * Enviando email
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA;
	$mensagem->From 		= $_SESSION['email_sistema'];
	
	if($encaminhados[0]) {
		foreach($encaminhados as $encam) {
			if($dadosusus[$pfu['usucpf']]) {
				$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
				$mensagem->AddAddress("priscila.vilaca@mec.gov.br", "Priscila Vilaça");
			}
		}
	}
	
	$mensagem->Subject = "Novo encaminhamento";
	$mensagem->Body = "Um nov encaminhamento foi criado";
	$mensagem->IsHTML( true );
	$mensagem->Send();
	/*
	 * FIM
	 * Enviando email
	 */
	
	$db->commit();

	
	redirecionarPagina("Encaminhamento inserido com sucesso", "sig.php?modulo=principal/encaminhamento&acao=A&solid=".$dados['solid']);
	
}

function downloadarquivo($dados) {
	global $db;
	
	$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$dados['arqid'];
	$arquivo = $db->pegaLinha($sql);
	
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
	
	if ( !is_file( $caminho ) ) {
		$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
	}
	$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
	header( 'Content-type: '. $arquivo['arqtipo'] );
	header( 'Content-Disposition: attachment; filename='.$filename);
	readfile( $caminho );
	exit();
}

function inseriratendimento($dados) {
	global $db;
	
	$sql = "INSERT INTO sig.atendimento(atdtxtresposta, ecaid, atddataatend) VALUES ('".$dados['atdtxtresposta']."', '".$dados['ecaid']."', NOW()) RETURNING atdid;";
	$atdid = $db->pegaUm($sql);
	
	if($_FILES['arquivo']['name'][0]) {
		for($i=0;$i<count($_FILES['arquivo']['name']);$i++) {
			if($_FILES['arquivo']['name'][$i]) {
				//Insere o registro do arquivo na tabela public.arquivo
				$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
				values('".current(explode(".", $_FILES['arquivo']['name'][$i]))."','".end(explode(".", $_FILES['arquivo']['name'][$i]))."','".$dados["arqdescricao"]."','".$_FILES['arquivo']['type'][$i]."','".$_FILES['arquivo']['size'][$i]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION['usucpf']."',". $_SESSION['sisid'] .") RETURNING arqid;";
				$arqid = $db->pegaUm($sql);
				
				//Insere o registro na tabela obras.arquivosobra
				$sql = "INSERT INTO sig.anexo(arqid, anxdsc, atdid) VALUES ('".$arqid."', '".(($dados['arquivonome'][$i])?$dados['arquivonome'][$i]:"Nome em branco")."', '".$atdid."');";
				$db->executar($sql);
				
				if(!is_dir('../../arquivos/sig/'.floor($arqid/1000))) {
					mkdir(APPRAIZ.'/arquivos/sig/'.floor($arqid/1000), 0777);
				}
				
				$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
				
				if ( !move_uploaded_file( $_FILES['arquivo']['tmp_name'][$i], $caminho ) ) {
					$db->rollback();
					die("<script>alert(\"Problemas no envio do arquivo.\");</script>");
				}
			}
		}
	}
	
	$db->commit();
	
	$sql = "SELECT usucpfenc FROM sig.encaminhados eca 
			LEFT JOIN sig.encaminhamento enc ON eca.encid = enc.encid 
			WHERE eca.ecaid='".$dados['ecaid']."'";
	
	$usucpfenc = $db->pegaUm($sql);
	
	/*
	 * Enviando SMS
	 */
	$dadosusus[''] = '556181149953';
	$dadosusus[''] = '556178132238';
	
	require_once('../webservice/painel/nusoap.php');
	$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
	$err = $client->getError();
	if ($err) {
	    die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
	}
	if($dadosusus[$encam['usucpf']]) {
		$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => SIGLA_SISTEMA, 'target' => $dadosusus[$usucpfenc], 'body' => 'Solicitação encaminhada foi respondida', 'ID' => substr($usucpfenc,0,6).date("Ymdhis")));
	}
	/*
	 * Enviando SMS
	 */
	
	/*
	 * Enviando email
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA;
	$mensagem->From 		= $_SESSION['email_sistema'];
	$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
	$mensagem->Subject = "Atendimento efetuado";
	$mensagem->Body = "Atendimento foi efetuado";
	$mensagem->IsHTML( true );
	$mensagem->Send();
	/*
	 * FIM
	 * Enviando email
	 */
	
	
	
	redirecionarPagina("Atendimento inserido com sucesso", "sig.php?modulo=principal/atendimento&acao=A");
	
}

function atualizaratendimento($dados) {
	global $db;
	
	$sql = "UPDATE sig.atendimento SET atdtxtresposta='".$dados['atdtxtresposta']."' WHERE atdid='".$dados['atdid']."'";
	$db->executar($sql);
	$db->commit();
	
	redirecionarPagina("Atendimento atualizado com sucesso", "sig.php?modulo=principal/atendimento&acao=A&ecaid=".$_REQUEST['ecaid']);
	
}


function inserirresposta($dados) {
	global $db;
	
	$sql = "INSERT INTO sig.resposta(solid, rsptxtresposta, usucpfresposta, rspdataresposta)
    		VALUES ('".$dados['solid']."', '".$dados['rsptxtresposta']."', '".$_SESSION['usucpf']."', NOW()) RETURNING rspid;";
	
	$rspid = $db->pegaUm($sql);
	
	
	for($i=0;$i<count($_FILES['arquivo']['name']);$i++) {
		
		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
		values('".current(explode(".", $_FILES['arquivo']['name'][$i]))."','".end(explode(".", $_FILES['arquivo']['name'][$i]))."','".$dados["arqdescricao"]."','".$_FILES['arquivo']['type'][$i]."','".$_FILES['arquivo']['size'][$i]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION['usucpf']."',". $_SESSION['sisid'] .") RETURNING arqid;";
		$arqid = $db->pegaUm($sql);
		
		//Insere o registro na tabela obras.arquivosobra
		$sql = "INSERT INTO sig.anexo(arqid, anxdsc, rspid) VALUES ('".$arqid."', '".(($dados['arquivonome'][$i])?$dados['arquivonome'][$i]:"Nome em branco")."', '".$rspid."');";
		$db->executar($sql);
		
		if(!is_dir('../../arquivos/sig/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/sig/'.floor($arqid/1000), 0777);
		}
		
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
		
		if ( !move_uploaded_file( $_FILES['arquivo']['tmp_name'][$i], $caminho ) ) {
			$db->rollback();
			echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
			exit;
		}
	}
	
	
	if($dados['arqatendentes']) {
		
		foreach($dados['arqatendentes'] as $anxid) {
			$sql = "UPDATE sig.anexo SET rspid = '".$rspid."' WHERE anxid='".$anxid."'";
			$db->executar($sql);
		}
		
	}
	
	$db->executar("UPDATE sig.solicitacao SET stsid='".SITSOL_FINALIZADO."' WHERE solid='".$dados['solid']."'");
	
	$db->commit();
	
	$sql = "SELECT usucpfsol FROM sig.solicitacao sol 
			WHERE sol.solid='".$dados['solid']."'";
	
	$usucpfsol = $db->pegaUm($sql);
	
	/*
	 * Enviando SMS
	 */
	$dadosusus[''] = '556181149953';
	$dadosusus[''] = '556178132238';
	
	require_once('../webservice/painel/nusoap.php');
	$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
	$err = $client->getError();
	if ($err) {
	    die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
	}
	if($dadosusus[$usucpfsol]) {
		$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => SIGLA_SISTEMA, 'target' => $dadosusus[$usucpfsol], 'body' => 'Solicitação finalizada', 'ID' => substr($usucpfsol,0,6).date("Ymdhis")));
	}
	/*
	 * Enviando SMS
	 */
	
	
	/*
	 * Enviando email
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= SIGLA_SISTEMA;
	$mensagem->From 		= $_SESSION['email_sistema'];
	$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
	$mensagem->Subject = "Solicitação atendida";
	$mensagem->Body = "Solicitação respondida com sucesso";
	$mensagem->IsHTML( true );
	$mensagem->Send();
	/*
	 * FIM
	 * Enviando email
	 */
	
	redirecionarPagina("Resposta inserida com sucesso", "sig.php?modulo=principal/solicitacao&acao=A");
}
?>