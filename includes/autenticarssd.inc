<?

include_once("connector.php");
header("Content-Type: text/html; charset=utf-8");
ob_start();

function getErroSSD($erro) {
	$f1 = strpos($erro,"(");
	return substr($erro,($f1+1),11);
	
}

function erro(){
	global $db;
	$db->commit();
	$_SESSION = array();
	$_SESSION['MSG_AVISO'] = func_get_args();
	header( "Location: ". $_SERVER['PHP_SELF'] );
	exit();
}


$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);

if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
	$SSDWs->useProductionSSDServices();
} else {
	$SSDWs->useHomologationSSDServices();
}

$cpfOrCnpj = str_replace(array(".","-"),array("",""), $_POST['usucpf']);
$resposta = $SSDWs->getUserInfoByCPFOrCNPJ($cpfOrCnpj);

/* Se o usuário não existe no SSD, verificar se ele existe no simec
 * caso existir, sincronizar com SSD
 */
define("PER_SIMEC", 154);
if(!$resposta->cpf) {
	$errossd = getErroSSD($resposta["erro"]);
	switch($errossd) {
		case "ErrorId=001":
			$cpfOrCnpj = str_replace(array(".","-"),array("",""), $_POST['usucpf']);
			$sql = "SELECT * FROM seguranca.usuario WHERE usucpf='".$cpfOrCnpj."'";
			$dadosusuario = $db->pegaLinha($sql);
			if($dadosusuario) {
				
				/*
				 * Exportando os dados do simec para o ssd (transparente para o usuário)
				 */
				$SSD_senha = @utf8_encode(base64_encode(md5_decrypt_senha( $dadosusuario['ususenha'], '' )));
				$SSD_tipo_pessoa = @utf8_encode("F");
				$SSD_nome = @utf8_encode($dadosusuario['usunome']);
				$SSD_cpf = @utf8_encode($cpfOrCnpj);
				$SSD_data_nascimento = @utf8_encode((($dadosusuario['usudatanascimento'])?$dadosusuario['usudatanascimento']:"0000-00-00"));
				$SSD_email = @utf8_encode($dadosusuario['usuemail']);
				$SSD_ddd_telefone = @utf8_encode($dadosusuario['usufoneddd']);
				$SSD_telefone = @utf8_encode($dadosusuario['usufonenum']);
				// Variavel para inserir os dados no SSD
				$userInfo = "$SSD_senha||$SSD_tipo_pessoa||$SSD_nome||$nome_mae||$SSD_cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" .
							"$SSD_data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$SSD_email||$email_alternativo||" .
							"$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$SSD_ddd_telefone||$SSD_telefone||" .
							"$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||ssd";
				// Inserindo usuario no SSD
				$respostanovousu = $SSDWs->signUpUser($userInfo);
				if($respostanovousu != "true") {
					switch($respostanovousu['erro']) {
						case @utf8_encode("SSD : A senha informada deve possuir no mínimo 4 caracteres não numéricos e 2 númericos."):
							// Se a autenticação para o SIMEC estiver correta, enviar para mudança de senha (para o padrão SSD)
							if (md5_decrypt_senha($dadosusuario['ususenha'],'') == $_POST['ususenha']) {
								$_SESSION['altsenhassd_cpf'] = $cpfOrCnpj;
								$_SESSION['altsenhassd_baselogin'] = $_POST['baselogin'];
								header("location: altsenhassd.php");
								exit;
							}
							
						default:
							session_unset();
							$_SESSION['MSG_AVISO'] = $respostanovousu['erro'];
							header('location: login.php');
							exit;
							
					}
				}
				/*
				 * Inserindo permissão ao novo usuário
				 */
				$permissionId = PER_SIMEC;
				// $responsibleForChangeCpfOrCnpj deve ser vazio
				$respostaperm = $SSDWs->includeUserPermissionByCPFOrCNPJ($cpfOrCnpj, $permissionId, $responsibleForChangeCpfOrCnpj);
				if($respostaperm != "true") {
					session_unset();
					$_SESSION['MSG_AVISO'] = $respostanovousu['erro'];
					header('location: login.php');
					exit;
				}
				
			}
			break;
			
		case "ErrorId=004":
			$permissionId = PER_SIMEC;
			$cpfOrCnpj = str_replace(array(".","-"),array("",""),$_POST["usucpf"]);
			// $responsibleForChangeCpfOrCnpj deve ser vazio
			$respostaperm = $SSDWs->includeUserPermissionByCPFOrCNPJ($cpfOrCnpj, $permissionId, $responsibleForChangeCpfOrCnpj);
			if($respostaperm != "true") {
				session_unset();
				$_SESSION['MSG_AVISO'] = $respostaperm['erro'];
				header('location: login.php');
				exit;
			}
			break;
	}
}
/* FIM
 * Se o usuário não existe no SSD, verificar se ele existe no simec
 * caso existir, sincronizar com SSD
 */




/*
 * Efetuando a autenticação
 */
$SSDWs = new SSDWsAuth($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
	$SSDWs->useProductionSSDServices();
} else {
	$SSDWs->useHomologationSSDServices();
}

$cpfOrCnpj = base64_encode(str_replace(array(".","-"),array("",""), $_POST['usucpf']));
$password = base64_encode($_POST['ususenha']);
$respostaUsrSSD = $SSDWs->loginUserIntoSystemByCPFOrCNPJAndPassword($cpfOrCnpj, $password);
if($respostaUsrSSD instanceof stdClass) {
	/*
	 * Autenticação com SSD realizada com sucesso
	 */
	
	if($respostaUsrSSD->identifier->field != "CPF" || $respostaUsrSSD->identifier->value != str_replace(array(".","-"),array("",""), $_POST['usucpf'])) {
		// 	Efetuando logout
		session_unset();
		$_SESSION['MSG_AVISO'] = "SSD : Problemas no identificador de autenticação";
		header('location: login.php');
    	exit;
	}
	
	// quantidade limite de tentativas de acesso com senha inválida
	define( 'LIMITE_DE_TENTATIVAS', 4 );

	// lista de status de usuário
	define( 'STATUS_ATIVO', 'A' );
	define( 'STATUS_PENDENTE', 'P' );
	define( 'STATUS_BLOQUEADO', 'B' );

	// mensagens de alera do sistema
	$alerta = array(
		'cpf' => "O cpf informado não está cadastrado.",
		'status_pendente' => "Sua conta está pendente para aprovação, aguarde a avaliação dos administradores.",
		'status_bloqueado' => "Sua conta está bloqueada, clique <a href='solicitar_ativacao_de_conta.php' title='solicitar ativação de conta'>aqui</a> para solicitar a ativação.",
		'senha' => "A senha informada não é válida. Tentativas restantes: %s",
		'tentativas' => "Você excedeu a quantidade de tentativas.",
		'permissao' => "Você não possui permissão de acesso em nenhum dos módulos.",
	);
	
	// carrega os dados da conta do usuário a partir do cpf informado
	$sql = sprintf(
		"SELECT 
			u.usucpf, 
			u.ususenha, 
			u.suscod, 
			u.usutentativas, 
			u.usunome, 
			u.usuemail 
		 FROM 
			seguranca.usuario u
		 WHERE 
			u.usucpf = '%s'",
		str_replace(array(".","-"),array("",""), $_POST['usucpf'])
	);

	$usuario = $db->pegaLinha( $sql );
	
	// verifica se o cpf existe
	if ( !$usuario['usucpf'] ) {
		erro( $alerta['cpf'] );
	}
	
	// Verifica o status do usuário
	switch ( $usuario['suscod'] ) {
		case STATUS_ATIVO:
			break;
		case STATUS_PENDENTE:
			erro( $alerta['status_pendente'] );
		case STATUS_BLOQUEADO:
		default:
			erro( $alerta['status_bloqueado'] );
	}
	
	// atribuições requeridas para que a auditoria do sistema funcione
	$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
	$_SESSION['usucpf'] = $usuario['usucpf'];
	$_SESSION['usucpforigem'] = $usuario['usucpf'];
	
	
	// carrega os dados do usuário para a sessão
	unset( $usuario['ususenha'] );
	foreach ( $usuario as $attribute => $value ) {
		$_SESSION[$attribute] = $value;
	}
	$_SESSION['usucpforigem'] = $usuario['usucpf'];
	
	// verifica permissão de acesso aos módulos
	$sql = sprintf(
		"SELECT
		s.sisid, s.sisdiretorio, s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel AS usunivel, us.susdataultacesso
		FROM seguranca.sistema s
		INNER JOIN seguranca.usuario_sistema us USING ( sisid )
		INNER JOIN seguranca.usuario u USING ( usucpf )
		INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
		INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod AND p.sisid = s.sisid
		WHERE
		us.suscod = 'A' AND
		u.usucpf = '%s' AND
		u.suscod = 'A' AND
		p.pflstatus = 'A'
		GROUP BY s.sisid, s.sisdiretorio,  s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel, us.susdataultacesso
		ORDER BY us.susdataultacesso DESC
		LIMIT 1",
		$usuario['usucpf']
	);

	$sistema = $db->pegaLinha( $sql );
	if ( !$sistema['sisid'] ) {
		erro( $alerta['permissao'] );
	}
	// carrega os dados do módulo para a sessão
	foreach ( $sistema as $attribute => $value ) {
		$_SESSION[$attribute] = $value;
	}
	
	// atribui o ano atual para o exercício das tarefas
	$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
	$_SESSION['exercicio'] = $db->pega_ano_atual();

	unset($_SESSION['superuser']);
	$_SESSION['superuser'] = $db->testa_superuser( $usuario['usucpf'] );

	// atualiza a data de último acesso no módulo selecionado
	$_SESSION['usuacesso'] = date( 'Y/m/d H:i:s' );
	$sql = sprintf(
		$sql = "UPDATE seguranca.usuario_sistema SET susdataultacesso = '%s' where usucpf = '%s' and sisid = %d",
		$_SESSION['usuacesso'],
		$usuario['usucpf'],
		$sistema['sisid']
	);
	$db->executar( $sql );
	
	// TO DO: verificar se há mensagens na caixa de entrada (aprovar e implementar fluxo)
	/*	o sistema deverá disparar o processo automático.
	Para tanto o sistema verfica se algume já disparou o processo para o dia escolhido. se sim, não faz nada, se não então atualiza a tabela e dispara o processo.
	*/
	$sql = "select ctrdata from controleprocesso where ctrdata=current_date";
	if  (! $db->pegaUm($sql) )//or $_SESSION['usucpf']=='')
	{
		// dispara os processos
		include APPRAIZ."includes/dispara_processo.inc";
	}

	$totmsg=0;
	unset($_SESSION['mostramsg']);
	$sql = "select count(msgid) as totmsg from mensagemusuario where usucpf = '".$_SESSION['usucpf']."' and msulida ='f' and msuexcluida='f'";
	$_SESSION['mostramsg']=$db->pegaUm($sql);

	/*
	 * O contador de tempo online na tela deve ser atualizado toda vez que o
	 * usuário carregar uma tela do sistema. Ele é utilizado pelo "estou vivo"
	 * de acordo com a constante MAXONLINETIME, definido no config.inc.
	 */
	$_SESSION["evHoraUltimoAcesso"] = time();
	
	// leva o usuário para a tela inicial do módulo selecionado
	$db->commit();

	if(!$_SESSION['login_demandas']){
		$header = sprintf(
			"Location: ../%s/%s.php?modulo=%s",
			$sistema['sisdiretorio'],
			$sistema['sisarquivo'],
			$sistema['paginainicial']
		);
	}
	else{
		$_SESSION['login_demandas'] = "";
		$header = sprintf(
			"Location: ../%s/%s.php?modulo=%s",
			'demandas',
			'demandas',
			'inicio&acao=C'
		);
	}
	
	header( $header );
	exit;
	
} else {
	// Efetuando logout
	session_unset();
	$_SESSION['MSG_AVISO'] = $respostaUsrSSD['erro'];
	header('location: login.php');
    exit;
}
?>