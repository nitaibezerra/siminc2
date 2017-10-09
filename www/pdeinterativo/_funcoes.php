<?php

function verificaRespostaAbaFormacao() {
	global $db;
	
	$abasobr = array("diagnostico_4_1_direcao",
					 "diagnostico_5_2_docentes",
					 "primeiros_passos_passo_1",
					 "primeiros_passos_passo_2",
					 "primeiros_passos_passo_3");
	
	$sql = "SELECT COUNT(*) as numero FROM pdeinterativo.abaresposta ar 
			INNER JOIN pdeinterativo.aba ab ON ab.abaid=ar.abaid 
			WHERE ar.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  ab.abacod IN('".implode("','",$abasobr)."')";
	
	$numero = $db->pegaUm($sql);
	
	if($numero==count($abasobr)) return true;
	else return false;
	
}

function apagarCachePdeInterativo() {
	if(CACHE_FILE) {
		/* Início - Cache em arquivo*/
		include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
		$cache = new cache(false);
		$cache->apagarCache("planoestrategico_".$_SESSION['pdeinterativo_vars']['pdeid']);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_FNDE);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_ESTADUAL);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_MUNICIPAL);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_CONSULTA);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_DIRETOR);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_ESTADUAL);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_MUNICIPAL);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_MEC);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_SUPER_USUARIO);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_ESTADUAL);
		$cache->apagarCache("planoestrategico_0_3_visualizarplanoacao_".$_SESSION['pdeinterativo_vars']['pdeid']."_semperfil");
		/* Fim - Cache em arquivo*/
	}
	
}

function apagarCachePdeDiagnotico() {
	global $arrAbasCache;
	include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
	$cache = new cache(false);
	if($arrAbasCache) {
		foreach($arrAbasCache as $aba) {
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_FNDE);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_ESTADUAL);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_MUNICIPAL);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_CONSULTA);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_DIRETOR);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_ESTADUAL);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_MUNICIPAL);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_MEC);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_SUPER_USUARIO);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_ESTADUAL);
			$cache->apagarCache($aba."_".$_SESSION['pdeinterativo_vars']['pdeid']."_semperfil");
		}
	}
	
}



function recuperaMembroPorCPF()
{
	global $db;
	$usucpf = $_POST['usucpf'];
	$sql = "select DISTINCT
				usu.*,
				ususis.suscod as status,
				( select htudsc from seguranca.historicousuario htu where htu.usucpf = usu.usucpf order by htuid desc limit 1) as justificativa
			from 
				seguranca.usuario usu 
			inner join
				seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf and ususis.sisid = ".SISID_PDE_INTERATIVO."
			inner join
				seguranca.statususuario sus ON sus.suscod = usu.suscod
			left join
				pdeinterativo.pessoa pde ON pde.usucpf = usu.usucpf
			where 
				usu.usucpf = '$usucpf'";
	
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		$arrDados = codificaUTF8($arrDados);
		echo simec_json_encode($arrDados);
	}
	die();
}

function filtraMunicipio()
{
	global $db;
	$estuf = $_POST['estuf'];
	$muncod = $_POST['muncod'];
	$sql = "	select
					muncod as codigo,
					mundescricao as descricao
				from
					territorios.municipio
				where
					estuf = '$estuf'
				order by
					mundescricao";
	$db->monta_combo("muncod",$sql,"S","Selecione...","","","","","S","","",$muncod);
}

function filtraOrgao()
{
	global $db;
	$tpocod = $_POST['tpocod'];
	$orgid = $_POST['orgid'];
	$sql = "	select
					orgid as codigo,
					orgcod || ' - ' || orgdsc as descricao
				from
					public.orgao
				where
					tpocod = '$tpocod'
				order by
					orgdsc";
	$db->monta_combo("orgid",$sql,"S","Selecione...","","","","","N","","",$orgid);
}

function salvarMembroComissao()
{
	global $db;
	
	extract($_POST);
	
	if(!$usucpf || !$estuf || !muncod || !$tpocod || !$usuemail || !$usufoneddd || !$usufonenum || !$rdo_status){
		$_SESSION['pdeinterativo']['msg'] = "Favor preencher todos os campos obrigatórios!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroComite&acao=A");
		exit;
	}
	
	if( !preg_match( "/^([a-zA-Z0-9])+([a-zA-Z0-9\._-])*@([a-zA-Z0-9_-])+([a-zA-Z0-9\._-]+)+$/", $usuemail)){
		$_SESSION['pdeinterativo']['msg'] = "E-mail inválido!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroComite&acao=A");
		exit;
	}
	if( $usuemail != $cousuemail ){
		$_SESSION['pdeinterativo']['msg'] = "Confirmação de E-mail inválida!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroComite&acao=A");
		exit;
	}
	//Município
	if($tpocod == "3"){
		$entid = $muncod;
		$pflcod = PDEINT_PERFIL_COMITE_MUNICIPAL;
	}
	//Estadual
	if($tpocod == "2"){
		$entid = $estuf;
		$pflcod = PDEINT_PERFIL_COMITE_ESTADUAL;
	}
	
	if(!ativaUsuarioPDEInterativo($_POST,$pflcod,$entid,$tpocod)){
		$_SESSION['pdeinterativo']['msg'] = "Não foi possível cadastrar o usuário no SIMEC!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroComite&acao=A");
		exit;
	}
	
	$usucpf = "'".substr(str_replace(array(".","-","/"),"",$usucpf),0,11)."'";
	$pesnome = "'$usunome'";
	
	$sql = "select pesid from pdeinterativo.pessoa where usucpf = $usucpf;";
	$existe = $db->pegaUm($sql);
	if(!$existe){
		$sql = "insert into 
					pdeinterativo.pessoa
				(usucpf,pesnome,pesstatus,pflcod)
					values
				($usucpf,$pesnome,'$rdo_status',$pflcod)";
	}else{
		$sql = "update 
					pdeinterativo.pessoa
				set
					pesstatus = '$rdo_status',
					pflcod = $pflcod
				where
					usucpf = $usucpf";
	}
	$db->executar($sql);
	$db->commit();
	
	$_SESSION['pdeinterativo']['msg'] = "Operação realizada com sucesso!";
	header("Location: pdeinterativo.php?modulo=principal/cadastroComite&acao=A");
	exit;
}


function salvarMembroConsulta()
{
	global $db;
	
	extract($_POST);
	
	
	if( !preg_match( "/^([a-zA-Z0-9])+([a-zA-Z0-9\._-])*@([a-zA-Z0-9_-])+([a-zA-Z0-9\._-]+)+$/", $usuemail)){
		$_SESSION['pdeinterativo']['msg'] = "E-mail inválido!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroConsulta&acao=A");
		exit;
	}
	if( $usuemail != $cousuemail ){
		$_SESSION['pdeinterativo']['msg'] = "Confirmação de E-mail inválida!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroConsulta&acao=A");
		exit;
	}
	//Município
	if($tpocod == "3"){
		$entid = $muncod;
		$pflcod = PDEINT_PERFIL_CONSULTA_MUNICIPAL;
		if(!$usucpf || !$estuf || !$muncod || !$tpocod || !$usuemail || !$usufoneddd || !$usufonenum || !$rdo_status){
			$_SESSION['pdeinterativo']['msg'] = "Favor preencher todos os campos obrigatórios!";
			header("Location: pdeinterativo.php?modulo=principal/cadastroConsulta&acao=A");
			exit;
		}
		
	}
	//Estadual
	if($tpocod == "2"){
		$entid = $estuf;
		$pflcod = PDEINT_PERFIL_CONSULTA_ESTADUAL;
		if(!$usucpf || !$estuf  || !$tpocod || !$usuemail || !$usufoneddd || !$usufonenum || !$rdo_status){
			$_SESSION['pdeinterativo']['msg'] = "Favor preencher todos os campos obrigatórios!";
			header("Location: pdeinterativo.php?modulo=principal/cadastroConsulta&acao=A");
			exit;
		}
		
	}
	
	if(!ativaUsuarioPDEInterativo($_POST,$pflcod,$entid,$tpocod)){
		$_SESSION['pdeinterativo']['msg'] = "Não foi possível cadastrar o usuário no SIMEC!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroConsulta&acao=A");
		exit;
	}
	
	$usucpf = "'".substr(str_replace(array(".","-","/"),"",$usucpf),0,11)."'";
	$pesnome = "'$usunome'";
	
	$sql = "select pesid from pdeinterativo.pessoa where usucpf = $usucpf";
	$exite = $db->pegaUm($sql);
	if(!$exite){
		$sql = "insert into 
					pdeinterativo.pessoa
				(usucpf,pesnome,pesstatus,pflcod)
					values
				($usucpf,$pesnome,'$rdo_status',$pflcod)";
	}else{
		$sql = "update 
					pdeinterativo.pessoa
				set
					pesstatus = '$rdo_status',
					pflcod = $pflcod
				where
					usucpf = $usucpf";
	}
	$db->executar($sql);
	$db->commit();
	
	$_SESSION['pdeinterativo']['msg'] = "Operação realizada com sucesso!";
	header("Location: pdeinterativo.php?modulo=principal/cadastroConsulta&acao=A");
	exit;
}


function atualizarUsuariosSeguranca()
{
	global $db;
	
	ini_set( "memory_limit", "1024M" ); // ...
	set_time_limit(0);
	
	$sql = "select 
				usucpf 
			from 
				seguranca.usuario 
			where 
				usunome = 'nome';";
	$arrCPF = $db->carregarColuna($sql);
		
	if($arrCPF){
		include APPRAIZ."www/includes/webservice/cpf.php";
		foreach($arrCPF as $cpf){
			$PF = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
			$dados = $PF->solicitarDadosPessoaFisicaPorCpf(trim($cpf));
			if(!strstr($dados,"WS-ACS - Erro")){
				$dado = explode(trim($cpf),$dados);
				$usunome = str_replace(array("<RESPOSTA><PESSOA><no_pessoa_rf>","</no_pessoa_rf><nu_cpf_rf>"),array("",""),$dado[0]);
				$arrSql.= "update seguranca.usuario set usunome = '$usunome' where usucpf = '".trim($cpf)."';";
			}else{
				$arrSql.= "delete from seguranca.historicousuario where usucpf = '".trim($cpf)."';
						   delete from seguranca.usuario_sistema where usucpf = '".trim($cpf)."';
						   delete from seguranca.perfilusuario where usucpf = '".trim($cpf)."';
						   delete from pdeinterativo.usuarioresponsabilidade where usucpf = '".trim($cpf)."';
						   delete from seguranca.usuario where usucpf = '".trim($cpf)."';";
				
			}
		}
		if($arrSql){
			$db->executar($arrSql);
			$db->commit();
			dbg($arrSql);
		}
	}
	die("Usuários atualizados com sucesso!");
}

function ativaUsuarioPDEInterativo( $dados, $pflcod, $entid = null , $tpocod = null){
	global $db;

	//id modulo sistema
	$sisid = SISID_PDE_INTERATIVO;

	$cpf = str_replace(array(".","-"),"",$dados["usucpf"]);
	$cpf = substr($cpf,0,11);
	$usunome = $dados["usunome"];
	$usuemail = $dados["usuemail"];
	$usufoneddd = $dados["usufoneddd"];
	$usufonenum = $dados["usufonenum"];
	$ususexo = substr($dados["ususexo"],0,1);
	$regcod = $dados["estuf"];
	$orgao = $dados["orgid"];
	$carid = $dados["carid"];
	$tpocod = $dados["tpocod"];
	$reenvio_email = $dados["hdn_reenvia_email"];
	$senhageral = $dados['chk_senha_padrao'] ? "simecdti" : false ;
	
	//variavel ativa usuario
	$suscod = "A";
	
	//envia Email quando Ativa o usuário
	if($dados['rdo_status'] == "A"){
		$sql = "select pesid from pdeinterativo.pessoa where usucpf = '$cpf' and pesstatus != 'A'";
		if($db->pegaUm($sql)){
			$reenvio_email = true;
		}
	}
	
	//Reenvia a senha p/ o usuário
	if($dados['rdo_reeviar_senha'] == "S"){
		$reenvio_email = true;;
	}
	

	//gera senha usuario
	$senhageral = $senhageral ? $senhageral : $db->gerar_senha();

	//usuário responsabilidade
	$regcod     = $dados['estuf'];
	$muncod     = $dados['muncod'];
	if($muncod) $muncod = ltrim($muncod,"0");

	// verifica se o cpf já está cadastrado no simec
	$sql = "SELECT usucpf, ususenha FROM seguranca.usuario WHERE usucpf = '$cpf'";
	$usuario = $db->pegaLinha( $sql );

	$unicod = !$unicod ? "null" : $unicod;
	$regcod = !$regcod ? "null" : "'".$regcod."'";
	$ungcod = !$ungcod ? "null" : $ungcod;
	$orgao = !$orgao ? "null" : $orgao;
	$carid = !$carid ? "null" : $carid;
	$tpocod = !$tpocod ? "null" : "'".$tpocod."'";
	
	if(!$senhageral) return false;
		
	if(!$usuario['usucpf']){
		// insere informações gerais do usuário
		$sql = "INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, carid, entid, unicod, usuchaveativacao, regcod,
					ususexo, ungcod, ususenha, suscod, orgao,
					muncod, tpocod
				) values (
					'".trim($cpf)."', '".str_to_upper( $usunome )."', '".strtolower( $usuemail )."', '$usufoneddd', '$usufonenum',
					'$usufuncao', $carid, null, $unicod, 'f',$regcod,
					'$ususexo', $ungcod, '".md5_encrypt_senha( $senhageral, '' )."', '$suscod', $orgao, 
					'$muncod', $tpocod
				)";
					
		$db->executar( $sql );

	}
	else{
		$sql = "UPDATE seguranca.usuario
				SET  
					usuemail = '$usuemail',
					usufoneddd = '$usufoneddd', 
					usufonenum = '$usufonenum',
					muncod = '$muncod'
					".($dados['chk_senha_padrao'] ? ",ususenha = '".md5_encrypt_senha( "simecdti", '' )."' " : "")."
				WHERE usucpf = '$cpf'";
		$db->executar($sql);
			
		$senhageral = $dados['chk_senha_padrao'] ? "simecdti" : md5_decrypt_senha( $usuario['ususenha'], '' );
	}
	$db->commit();

	// verifica se o usuário já está cadastrado no módulo selecionado
	$sql = sprintf("SELECT usucpf FROM seguranca.usuario_sistema WHERE usucpf = '%s' and sisid = ".SISID_PDE_INTERATIVO." and susstatus = 'A' ",
		$cpf);
	$modulo = $db->pegaLinha( $sql );

	if(!$modulo['usucpf']){
		// vincula o usuário com o módulo
		$sqlu = sprintf(
	    		"INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod , suscod) values ( '%s', %d, %d , 'A')",
		$cpf,
		SISID_PDE_INTERATIVO,
		$pflcod
		);
		$db->executar( $sqlu );

	}else{
		// atualiza o vínculo do usuário com o módulo
		$sqlu = "update
					seguranca.usuario_sistema
				set
					pflcod = $pflcod
				where
					usucpf = '$cpf'
				and
					suscod = 'A'
				and
					sisid = ".SISID_PDE_INTERATIVO."
				and
					susstatus = 'A'";
		$db->executar( $sqlu );
	}
	$db->commit();

	//Ativa o usuário
	$justificativa = !$dados['justificativa'] ? "Ativação automática de usuário pelo Módulo PDE Interativo" : $dados['justificativa'];
	$db->alterar_status_usuario( $cpf, $dados['rdo_status'], $justificativa, $sisid );

	// Verifica se existe o perfil p/ o usuário
	$existe = $db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf = '$cpf' and pflcod=".$pflcod);
	
	if(!$existe){
		$sqlp = sprintf(
			"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
		$cpf,
		$pflcod
		);
		
		$db->executar( $sqlp );
		$db->commit();
	}

	if($entid){
		if($pflcod == PDEINT_PERFIL_COMITE_ESTADUAL || $pflcod == PDEINT_PERFIL_CONSULTA_ESTADUAL){
			$campo = "estuf";
			$NaoDesativaOutros = true;
		}elseif($pflcod == PDEINT_PERFIL_COMITE_MUNICIPAL || $pflcod == PDEINT_PERFIL_CONSULTA_MUNICIPAL){
			$campo = "muncod";
			$NaoDesativaOutros = true;
		}else{
			$campo = "entid";
			$NaoDesativaOutros = false;
		}
		$sql = "SELECT * FROM pdeinterativo.usuarioresponsabilidade WHERE rpustatus = 'A' and usucpf = '$cpf' and $campo = '$entid' and pflcod = $pflcod";
		$existeu = $db->pegaUm($sql);
		if(!$existeu){
			
			if($pflcod == PDEINT_PERFIL_COMITE_MUNICIPAL || $pflcod == PDEINT_PERFIL_COMITE_ESTADUAL){
				$sqlp = sprintf(
				   "INSERT INTO pdeinterativo.usuarioresponsabilidade ( pflcod, usucpf, $campo, rpustatus, rpudata_inc ) 
					VALUES ( %d, '%s', '%s', 'A', now() )",
				$pflcod,
				$cpf,
				$entid
				);
			}else{
				$sqlp = sprintf(
				   (!$NaoDesativaOutros ? "UPDATE pdeinterativo.usuarioresponsabilidade set rpustatus = 'I' where pflcod = $pflcod and $campo = '$entid' ;" : "")."
					INSERT INTO pdeinterativo.usuarioresponsabilidade ( pflcod, usucpf, $campo, rpustatus, rpudata_inc ) 
					VALUES ( %d, '%s', '%s', 'A', now() )",
				$pflcod,
				$cpf,
				$entid
				);
			}
			$db->executar( $sqlp );
			$db->commit();
		}
	}
	

	// envia o email de confirmação da conta aprovada
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO PDE-INTERATIVO","email" => $usuemail);
	$destinatario = $usuemail;
	$assunto = "Cadastro no SIMEC - MÓDULO PDE-INTERATIVO";
	$conteudo = "
		<br/>
		<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
		<br/>
		<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
		<br/><br/>
		";
	if(!$existeu || $reenvio_email){
		$conteudo .= sprintf(
		"%s %s, <p>Você foi pré-cadastrado no SIMEC, módulo PDE-Interativo. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
				<p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
				<p>Sua Senha de acesso é: %s</p>
				<br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.",
		'Prezado(a)',
		$usunome,
		$senhageral	
		);
	}else{
		$conteudo .= sprintf(
		"%s %s, <p>Você foi pré-cadastrado no SIMEC, módulo PDE-Interativo. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
				<p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>",
		'Prezado(a)',
		$usunome	
		);
	}
	
	if(!strstr($_SERVER['HTTP_HOST'],"simec-local") && (!$existeu || $reenvio_email) && $dados['rdo_status'] == "A"  ){
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	}	
	return true;

}

function listaComite()
{
	global $db;
	
	if($_POST){
		extract($_POST);
	}
	
	if($usunome){
		$arrWhere[] = "removeacento(usu.usunome) ilike removeacento(('%$usunome%'))";
	}
	if($usucpf){
		$arrWhere[] = "usu.usucpf = '".str_replace(array("-","."),"",$usucpf)."'";
	}
	if($estuf){
		$arrWhere[] = "usu.regcod = '$estuf'";
	}
	if($muncod){
		$arrWhere[] = "usu.muncod = '$muncod'";
	}
	if($carid){
		$arrWhere[] = "usu.carid = $carid";
	}
	
	if($usu_plfcod){
		switch($usu_plfcod){
			case "Estadual":
				$arrWhere[] = "per.pflcod = ".PDEINT_PERFIL_COMITE_ESTADUAL;
				break;
			case "Municipal":
				$arrWhere[] = "per.pflcod = ".PDEINT_PERFIL_COMITE_MUNICIPAL;
				break;
		}
	}
	if($rdo_status){
		if($rdo_status == "P"){
			$arrWhere2[] = "susdsc = 'Pendente'";
		}elseif($rdo_status == "A"){
			$arrWhere2[] = "susdsc = 'Ativo'";
		}else{
			$arrWhere2[] = "susdsc = 'Bloqueado'";
		}
	}
	
	$sql = "select distinct * from (
(
select DISTINCT
				'<img class=\"link\" onclick=\"editarMembroComite(\'' || usu.usucpf || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"link\" onclick=\"excluirMembroComite(\'' || usu.usucpf || '\')\" src=\"../imagens/excluir.gif\" />' as acao,
				usu.usucpf,
				usu.usunome,
				usu.usuemail,
				mun.mundescricao,
				mun.estuf,
				pes.pesstatus,
				CASE per.pflcod
					WHEN ".PDEINT_PERFIL_COMITE_ESTADUAL." THEN 'Comitê Estadual'
					WHEN ".PDEINT_PERFIL_COMITE_MUNICIPAL." THEN 'Comitê Municipal'
					ELSE 'N/A'
				END as membro,
				CASE (
						CASE ususis.suscod
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END ) WHEN 'Ativo'
					THEN
						(CASE pesstatus
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END )
					ELSE
						( CASE ususis.suscod
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END )
				END as susdsc
			from
				seguranca.usuario usu
			left join
				territorios.municipio mun ON mun.muncod = usu.muncod
			left join
				pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus <> 'I'
			left join
				seguranca.perfilusuario per ON per.usucpf = usu.usucpf
			left join
				seguranca.statususuario sus ON sus.suscod = usu.suscod
			left join
				seguranca.perfil pfl ON pfl.pflcod = per.pflcod
			inner join
				seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf and ususis.susstatus = 'A' and ususis.sisid = ".SISID_PDE_INTERATIVO."
			where
				1=1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			and
				per.pflcod in (".PDEINT_PERFIL_COMITE_ESTADUAL.",".PDEINT_PERFIL_COMITE_MUNICIPAL.")
			and
				pfl.sisid  = ".SISID_PDE_INTERATIVO."

 ) UNION ALL (				

select DISTINCT
				'<img class=\"link\" onclick=\"editarMembroComite(\'' || usu.usucpf || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"link\" onclick=\"excluirMembroComite(\'' || usu.usucpf || '\')\" src=\"../imagens/excluir.gif\" />' as acao,
				usu.usucpf,
				usu.usunome,
				usu.usuemail,
				mun.mundescricao,
				mun.estuf,
				pes.pesstatus,
				CASE ususis.pflcod
					WHEN ".PDEINT_PERFIL_COMITE_ESTADUAL." THEN 'Comitê Estadual'
					WHEN ".PDEINT_PERFIL_COMITE_MUNICIPAL." THEN 'Comitê Municipal'
					ELSE 'N/A'
				END as membro,
				CASE (
						CASE ususis.suscod
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END ) WHEN 'Ativo'
					THEN
						(CASE pesstatus
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END )
					ELSE
						( CASE ususis.suscod
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END )
				END as susdsc
			from
				seguranca.usuario usu
			left join
				territorios.municipio mun ON mun.muncod = usu.muncod
			left join
				pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus <> 'I'
			left join
				seguranca.perfilusuario per ON per.usucpf = usu.usucpf
			left join
				seguranca.statususuario sus ON sus.suscod = usu.suscod
			left join
				seguranca.perfil pfl ON pfl.pflcod = per.pflcod
			inner join
				seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf and ususis.susstatus = 'A'
			where
				1=1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			and
				ususis.pflcod in (".PDEINT_PERFIL_COMITE_ESTADUAL.",".PDEINT_PERFIL_COMITE_MUNICIPAL.") 
			and
				ususis.sisid  = ".SISID_PDE_INTERATIVO." ) ) as t where 1=1 ".($arrWhere2 ? " and ".implode(" and ",$arrWhere2) : "")."";
	
	//dbg($sql);
	
	$arDados = $db->carregar( $sql );
	$arDados = $arDados ? $arDados : array();
	$arRegistro = array();
	
	foreach ($arDados as $key => $v) {
		if( $v['pesstatus'] <> 'I' ){
			array_push($arRegistro, array(
							"acao" => $v['acao'],
							"usucpf" => $v['usucpf'],
							"usunome" => $v['usunome'],
							"usuemail" => $v['usuemail'],
							"mundescricao" => $v['mundescricao'],
							"estuf" => $v['estuf'],
							"membro" => $v['membro'],
							"susdsc" => $v['susdsc']
						));
		}
	}
	
	$cabecalho = array("Ação","CPF","Nome","E-mail","Município","UF","Membro","Status");
	$db->monta_lista_array($arRegistro, $cabecalho, 50, 10, 'N','Center');
	//$db->monta_lista($sql,$cabecalho,50,10,"N","center","N");
	
}

function listaMembroConsulta()
{
	global $db;
	
	if($_POST){
		extract($_POST);
	}
	
	if($usunome){
		$arrWhere[] = "removeacento(usu.usunome) ilike removeacento(('%$usunome%'))";
	}
	if($usucpf){
		$arrWhere[] = "usu.usucpf = '".str_replace(array("-","."),"",$usucpf)."'";
	}
	if($estuf){
		$inner = "inner join pdeinterativo.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf and ur.estuf='$estuf' AND rpustatus='A'";
	}
	if($muncod){
		$inner = "inner join pdeinterativo.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf and ur.muncod='$muncod' AND rpustatus='A'";
	}
	if($carid){
		$arrWhere[] = "usu.carid = $carid";
	}
	
	if($usu_plfcod){
		switch($usu_plfcod){
			case "Estadual":
				$arrWhere11[] = "per.pflcod = ".PDEINT_PERFIL_CONSULTA_ESTADUAL;
				$arrWhere22[] = "ususis.pflcod = ".PDEINT_PERFIL_CONSULTA_ESTADUAL;
				break;
			case "Municipal":
				$arrWhere11[] = "per.pflcod = ".PDEINT_PERFIL_CONSULTA_MUNICIPAL;
				$arrWhere22[] = "ususis.pflcod = ".PDEINT_PERFIL_CONSULTA_MUNICIPAL;
				break;
		}
	}
	if($rdo_status){
		if($rdo_status == "P"){
			$arrWhere2[] = "susdsc = 'Pendente'";
		}elseif($rdo_status == "A"){
			$arrWhere2[] = "susdsc = 'Ativo'";
		}else{
			$arrWhere2[] = "susdsc = 'Bloqueado'";
		}
	}
	
	
	$sql = "select DISTINCT
				'<img class=\"link\" onclick=\"editarMembroConsulta(\'' || usu.usucpf || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"link\" onclick=\"excluirMembroConsulta(\'' || usu.usucpf || '\')\" src=\"../imagens/excluir.gif\" />' as acao,
				usu.usucpf,
				usu.usunome,
				usu.usuemail,
				mun.mundescricao,
				mun.estuf,
				pes.pesstatus,
				CASE per.pflcod
					WHEN ".PDEINT_PERFIL_CONSULTA_ESTADUAL." THEN 'Consulta Estadual'
					WHEN ".PDEINT_PERFIL_CONSULTA_MUNICIPAL." THEN 'Consulta Municipal'
					ELSE 'N/A'
				END as membro,
				CASE (
						CASE ususis.suscod
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END ) WHEN 'Ativo'
					THEN
						(CASE pesstatus
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END )
					ELSE
						( CASE ususis.suscod
							WHEN 'A' THEN 'Ativo'
							WHEN 'B' THEN 'Bloqueado'
							WHEN 'I' THEN 'Inativo'
							ELSE 'Pendente'
						END )
				END as susdsc
			from
				seguranca.usuario usu
			left join
				territorios.municipio mun ON mun.muncod = usu.muncod
			left join
				pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus <> 'I'
			left join
				seguranca.perfilusuario per ON per.usucpf = usu.usucpf
			left join
				seguranca.statususuario sus ON sus.suscod = usu.suscod
			left join
				seguranca.perfil pfl ON pfl.pflcod = per.pflcod
			inner join
				seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf and ususis.susstatus = 'A' and ususis.sisid = ".SISID_PDE_INTERATIVO."
			{$inner}
			where
				1=1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			".($arrWhere11 ? " and ".implode(" and ",$arrWhere11) : "")."
			and
				per.pflcod in (".PDEINT_PERFIL_CONSULTA_ESTADUAL.",".PDEINT_PERFIL_CONSULTA_MUNICIPAL.")
			and
				pfl.sisid  = ".SISID_PDE_INTERATIVO."";
	
	$arDados = $db->carregar( $sql );
	$arDados = $arDados ? $arDados : array();
	$arRegistro = array();
	
	foreach ($arDados as $key => $v) {
		if( $v['pesstatus'] <> 'I' ){
			array_push($arRegistro, array(
							"acao" => $v['acao'],
							"usucpf" => $v['usucpf'],
							"usunome" => $v['usunome'],
							"usuemail" => $v['usuemail'],
							"mundescricao" => $v['mundescricao'],
							"estuf" => $v['estuf'],
							"membro" => $v['membro'],
							"susdsc" => $v['susdsc']
						));
		}
	}
	
	$cabecalho = array("Ação","CPF","Nome","E-mail","Município","UF","Membro","Status");
	$db->monta_lista_array($arRegistro, $cabecalho, 50, 10, 'N','Center');
	//$db->monta_lista($sql,$cabecalho,50,10,"N","center","N");
	
}


function carregarMembroComite()
{
	return array("usucpf" => $_POST['usucpf']);
}

function carregarMembroConsulta()
{
	return array("usucpf" => $_POST['usucpf']);
}


function ativaTodosMembrosComite()
{
	global $db;
	
	ini_set( "memory_limit", "2048M" );
	set_time_limit(0);
	
	$sql = "select distinct * from (
				(
				select DISTINCT
								' ' as acao,
								usu.usucpf,
								usu.usunome,
								usu.usuemail,
								mun.mundescricao,
								mun.muncod,
								mun.estuf,
								per.pflcod,
								CASE per.pflcod
									WHEN 525 THEN 'Comitê Estadual'
									WHEN 540 THEN 'Comitê Municipal'
									ELSE 'N/A'
								END as membro,
								CASE pesstatus
									WHEN 'A' THEN 'Ativo'
									WHEN 'B' THEN 'Bloqueado'
									WHEN 'I' THEN 'Inativo'
									ELSE 'Pendente'
								END as susdsc
							from
								seguranca.usuario usu
							left join
								territorios.municipio mun ON mun.muncod = usu.muncod
							left join
								pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus <> 'I'
							left join
								seguranca.perfilusuario per ON per.usucpf = usu.usucpf
							left join
								seguranca.statususuario sus ON sus.suscod = usu.suscod
							left join
								seguranca.perfil pfl ON pfl.pflcod = per.pflcod
							inner join
								seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf
							where
								1=1
							
							and
								per.pflcod in (".PDEINT_PERFIL_COMITE_ESTADUAL.",".PDEINT_PERFIL_COMITE_MUNICIPAL.")
							and
								pfl.sisid  = ".SISID_PDE_INTERATIVO."
							and
								pesstatus = 'A'
				
				 ) UNION ALL (				
				
				select DISTINCT
								' ' as acao,
								usu.usucpf,
								usu.usunome,
								usu.usuemail,
								mun.mundescricao,
								mun.muncod,
								mun.estuf,
								per.pflcod,
								CASE ususis.pflcod
									WHEN 525 THEN 'Comitê Estadual'
									WHEN 540 THEN 'Comitê Municipal'
									ELSE 'N/A'
								END as membro,
								CASE pesstatus
									WHEN 'A' THEN 'Ativo'
									WHEN 'B' THEN 'Bloqueado'
									WHEN 'I' THEN 'Inativo'
									ELSE 'Pendente'
								END as susdsc
							from
								seguranca.usuario usu
							left join
								territorios.municipio mun ON mun.muncod = usu.muncod
							left join
								pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf AND pesstatus <> 'I'
							left join
								seguranca.perfilusuario per ON per.usucpf = usu.usucpf
							left join
								seguranca.statususuario sus ON sus.suscod = usu.suscod
							left join
								seguranca.perfil pfl ON pfl.pflcod = per.pflcod
							inner join
								seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf
							where
								1=1
							
							and
								ususis.pflcod in (".PDEINT_PERFIL_COMITE_ESTADUAL.",".PDEINT_PERFIL_COMITE_MUNICIPAL.")
							and
								ususis.sisid  = ".SISID_PDE_INTERATIVO." and pesstatus = 'A') ) as t where susdsc = 'Ativo'";
	dbg($sql);
	$arrUsers = $db->carregar($sql);
	
	if($arrUsers){
		foreach($arrUsers as $user){
			if($user['muncod'] || $user['estuf']){
				if($user['pflcod'] == PDEINT_PERFIL_COMITE_ESTADUAL){
					$campo = "estuf";
					$valor = $user['estuf'];
				}elseif($user['pflcod'] == PDEINT_PERFIL_COMITE_MUNICIPAL){
					$campo = "muncod";
					$valor = $user['muncod'];
				}
				
				if($campo && $valor && $user['usucpf'] && $user['pflcod']){
					$sql = "SELECT * FROM pdeinterativo.usuarioresponsabilidade WHERE rpustatus = 'A' and usucpf = '{$user['usucpf']}' and $campo = '$valor' and pflcod = {$user['pflcod']}";
					$exite = $db->pegaUm($sql);
					if(!$exite){
						$sqlp= sprintf(
						   "INSERT INTO pdeinterativo.usuarioresponsabilidade ( pflcod, usucpf, $campo, rpustatus, rpudata_inc ) 
							VALUES ( %d, '%s', '%s', 'A', now() ); ",
						$user['pflcod'],
						$user['usucpf'],
						$valor
						);
					}else{
						$sqlp= sprintf(
						   "INSERT INTO pdeinterativo.usuarioresponsabilidade ( pflcod, usucpf, $campo, rpustatus, rpudata_inc ) 
							VALUES ( %d, '%s', '%s', 'A', now() ); ",
						$user['pflcod'],
						$user['usucpf'],
						$valor
						);
					}
					$db->executar( $sqlp );
					$db->commit();
					
				}
			}
		}
	}
	
}

function excluirMembroConsulta()
{
	global $db;
	$usucpf = $_POST['usucpf'];

	$sql = "select pflcod from pdeinterativo.usuarioresponsabilidade where usucpf = '$usucpf'";
	$pflcod = $db->pegaUm($sql);
	if($pflcod){
		$sqlD = "delete from seguranca.perfilusuario where usucpf = '$usucpf' and pflcod = $pflcod;
				delete from pdeinterativo.usuarioresponsabilidade where usucpf = '$usucpf' and pflcod = '$pflcod';";
	}
	
	$sql = "$sqlD
			delete from seguranca.usuario_sistema where usucpf = '$usucpf' and sisid = ".SISID_PDE_INTERATIVO.";
			update 
				pdeinterativo.pessoa
			set
				pesstatus = 'I'
			where
				usucpf = '$usucpf'";
	
	$db->executar($sql);
	$db->commit();
	$_SESSION['pdeinterativo']['msg'] = "Usuário excluído com sucesso!";
	
	
	// envia o email
	$sql = "select usunome,usuemail from seguranca.usuario where usucpf = '$usucpf'";
	$usu = $db->pegaLinha($sql);
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO PDE-INTERATIVO","email" => $usuemail);
	$destinatario = $usu['usuemail'];
	$assunto = "Cadastro no SIMEC - MÓDULO PDE-INTERATIVO";	
	$conteudo .= sprintf(
	"%s %s, <p>Seu cadastro no módulo PDE Interativo / SIMEC não foi aprovado. Reveja o módulo e o perfil solicitado. Em caso de dúvidas, entre em contato com a sua Secretaria.</p>",
	'Prezado(a)',
	$usu['usunome']);
	if($usu && !strstr($_SERVER['HTTP_HOST'],"simec-local")){
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	}
	
	header("Location: pdeinterativo.php?modulo=principal/listaConsulta&acao=A");
	exit;
}


function excluirMembroComite()
{
	global $db;
	$usucpf = $_POST['usucpf'];

	$sql = "select pflcod from pdeinterativo.usuarioresponsabilidade where usucpf = '$usucpf'";
	$pflcod = $db->pegaUm($sql);
	if($pflcod){
		$sqlD = "delete from seguranca.perfilusuario where usucpf = '$usucpf' and pflcod = $pflcod;
				delete from pdeinterativo.usuarioresponsabilidade where usucpf = '$usucpf' and pflcod = '$pflcod';";
	}
	
	$sql = "$sqlD
			delete from seguranca.usuario_sistema where usucpf = '$usucpf' and sisid = ".SISID_PDE_INTERATIVO.";
			update 
				pdeinterativo.pessoa
			set
				pesstatus = 'I'
			where
				usucpf = '$usucpf'";
	
	$db->executar($sql);
	$db->commit();
	$_SESSION['pdeinterativo']['msg'] = "Membro excluído com sucesso!";
	
	
	// envia o email
	$sql = "select usunome,usuemail from seguranca.usuario where usucpf = '$usucpf'";
	$usu = $db->pegaLinha($sql);
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO PDE-INTERATIVO","email" => $usuemail);
	$destinatario = $usu['usuemail'];
	$assunto = "Cadastro no SIMEC - MÓDULO PDE-INTERATIVO";	
	$conteudo .= sprintf(
	"%s %s, <p>Seu cadastro no módulo PDE Interativo / SIMEC não foi aprovado. Reveja o módulo e o perfil solicitado. Em caso de dúvidas, entre em contato com a sua Secretaria.</p>",
	'Prezado(a)',
	$usu['usunome']);
	if($usu && !strstr($_SERVER['HTTP_HOST'],"simec-local")){
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	}
	
	header("Location: pdeinterativo.php?modulo=principal/listaComite&acao=A");
	exit;
	
}

function importarEscola()
{
	global $db;
	
	if(!$_FILES['arquivo']){
		$_SESSION['pdeinterativo']['msg'] = "Favor selecionar o arquivo!";
		header("Location: pdeinterativo.php?modulo=sistema/importacao/importarEscola&acao=A");
		exit;	
	}
	if(!strstr($_FILES['arquivo']['name'],".csv")){
		$_SESSION['pdeinterativo']['msg'] = "Favor selecionar um arquivo '.csv'!";
		header("Location: pdeinterativo.php?modulo=sistema/importacao/importarEscola&acao=A");
		exit;
	}
	
	$csv = file($_FILES['arquivo']['tmp_name']);
	
	if($csv){
		foreach($csv as $escola){
			$esc = explode(";",$escola);
			if($esc[0] == "D"){
				$arrEscola[] = $esc[5];
			}elseif($esc[2] == "MUNICIPAL" || $esc[2] == "ESTADUAL"){
				$arrEscola[] = $esc[3];
			}
			
		}
		
		if($arrEscola){
			$sql = "select pdicodinep from pdeinterativo.pdinterativo where pdicodinep in('".implode("','",$arrEscola)."') and pdistatus = 'A'";
			$arrEscolasPresentes = $db->carregarColuna($sql);
			$arrEscolasPresentes = !$arrEscolasPresentes ? array() : $arrEscolasPresentes;
			
			$arrEscolaNovo = array_diff($arrEscola,$arrEscolasPresentes);
			
			foreach($arrEscolaNovo as $esc){
				$arrEscolasImportadas[] = $esc;
				$sqlI.= "insert into pdeinterativo.pdinterativo (pdicodinep,pdistatus) values ('$esc','A');";
			}
		}
		
		if($sqlI){
			$db->executar($sqlI);
			$campos	= array("tpdid"				=> 1,
							"anxdtinclusao" 	=> "now()",
							"anxstatus" 		=> "'A'"
						   );	
			
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			
			$file = new FilesSimec("anexo", $campos ,"pdeinterativo");
			
			$arquivoSalvo = $file->setUpload("Arquivo de Importação de Escolas do PDE-Interativo");
			
			if($arquivoSalvo){
				$db->commit();
			}
			
			$_SESSION['pdeinterativo']['msg'] = "Foram importadas ".number_format(count($arrEscolasImportadas),0,".",".")." de ".number_format(count($csv),0,".",".")." escolas presentes no arquivo selecionado!";
			header("Location: pdeinterativo.php?modulo=sistema/importacao/importarEscola&acao=A");
			exit;
		}else{
			$_SESSION['pdeinterativo']['msg'] = "Não existem escolas para importação no arquivo selecionado!";
			header("Location: pdeinterativo.php?modulo=sistema/importacao/importarEscola&acao=A");
			exit;
		}
	}
}

function listaArquivosImportacao()
{
	global $db;
	
	 $sql = "SELECT
                        to_char(arq.arqdata,'DD/MM/YYYY'),
                        tpa.tpddsc,
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'pdeinterativo.php?modulo=sistema/importacao/importarEscola&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        arq.arqdescricao,
                        usu.usunome
                    FROM
                        public.arquivo arq 
                    INNER JOIN 
                   		pdeinterativo.anexo a ON arq.arqid = a.arqid
                    INNER JOIN 
                   		pdeinterativo.tipodocumento tpa ON tpa.tpdid = a.tpdid
                    INNER JOIN
                    	seguranca.usuario usu ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A'
                    AND
                    	tpa.tpdid = 1";
		
        $cabecalho = array( 
                            "Data Inclusão",
                            "Tipo Arquivo",
                            "Nome Arquivo",
                            "Descrição Arquivo",
                            "Responsável"
                            );
        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '' );
	
}

function listaDiretor()
{
	global $db;
	
	if($_POST){
		extract($_POST);
	}
	if($filtrodiretor){
		$arrWhere[] = $filtrodiretor;
	}
	if($pditempdeescola){
		$arrWhere[] = "pde.pditempdeescola={$pditempdeescola}";
	}
	if($pdenome){
		$arrWhere[] = "removeacento(pdenome) ilike removeacento(('%$pdenome%'))";
	}
	if($pdicodinep){
		$arrWhere[] = "pdicodinep = '$pdicodinep'";
	}
	if($usunome){
		$arrWhere[] = "removeacento(us.usunome) ilike removeacento(('%$usunome%'))";
	}
	if($usucpf){
		$arrWhere[] = "us.usucpf = '".str_replace(array("-","."),"",$usucpf)."'";
	}
	if($tpocod){
		$arrWhere[] = "us.tpocod = '$tpocod'";
	}
	if($pdiesfera){
		$arrWhere[] = "pdiesfera = '".trim($pdiesfera)."'";
	}
	
	$arrPerfil = pegaPerfilGeral();
	
	if(!$db->testa_superuser()) {
		
		if(in_array(PDEESC_PERFIL_CONSULTA,$arrPerfil)){
			$nao_exibir_icones = true;
		}
		
		if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$arrPerfil)){
			$sql = "select mun.muncod
					from pdeinterativo.usuarioresponsabilidade ur
					inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_MUNICIPAL."
					order by rpuid desc";
			
			$muncod = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Municipal' AND mun.muncod='".$muncod."' AND pde.pdigeridapde = TRUE)";
		}
		
		if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$arrPerfil)){
			$sql = "select mun.muncod
					from pdeinterativo.usuarioresponsabilidade ur
					inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL."
					order by rpuid desc";
			
			$muncod = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Municipal' AND mun.muncod='".$muncod."' AND pde.pdigeridapde = FALSE)";
		}
		
		if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$arrPerfil)){
			$sql = "select estuf
					from pdeinterativo.usuarioresponsabilidade ur
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_ESTADUAL."
					order by rpuid desc";
	
			$estuf = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Estadual' AND mun.estuf='".$estuf."' AND pde.pdigeridapde = TRUE)";
			
		}
		
		if(in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL,$arrPerfil)){
			$sql = "select estuf
					from pdeinterativo.usuarioresponsabilidade ur
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_ESTADUAL."
					order by rpuid desc";
	
			$estuf = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Estadual' AND mun.estuf='".$estuf."' AND pde.pdigeridapde = FALSE)";
			
		}
		
		if($paramperfil) {
			
			$arrWhere[] = "(".implode(" OR ",$paramperfil).")";
			
		} else {
			
			if($estuf){
				$arrWhere[] = "mun.estuf = '$estuf'";
			}
			if($muncod){
				$arrWhere[] = "mun.muncod = '$muncod'";
			}
			
		}
	} else {
			
		if($estuf){
			$arrWhere[] = "mun.estuf = '$estuf'";
		}
		if($muncod){
			$arrWhere[] = "mun.muncod = '$muncod'";
		}
		
	}
	
	
	
		
	if($rdo_status){
		if($rdo_status == "P"){
			$arrWhere2[] = "susdsc = 'Pendente'";
		}elseif($rdo_status == "A"){
			$arrWhere2[] = "susdsc = 'Ativo'";
		}else{
			$arrWhere2[] = "susdsc = 'Bloqueado'";
		}
	}
	
	$sql = "SELECT '<span style=\"white-space:nowrap;\" >".(($nao_exibir_icones)?"-":"<img class=\"link\" onclick=\"editarDiretor(\'' || COALESCE(diretor.usucpf,'') || '\',\'' || COALESCE(pdicodinep,'') || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"link\" onclick=\"excluirDiretor(\'' || COALESCE(diretor.usucpf,'') || '\',\'' || COALESCE(pdicodinep,'') || '\')\" src=\"../imagens/excluir.gif\" />")."</span>' as acao,
					pdicodinep,
					pdenome,
					CASE WHEN 
						 pdiesfera IS NULL THEN 'N/A'
					ELSE pdiesfera
					END as pdiesfera,
					mun.mundescricao,
					mun.estuf,
					diretor.usucpf,
					diretor.pesnome,
					diretor.usuemail,
					CASE (
							CASE diretor.suscod
								WHEN 'A' THEN 'Ativo'
								WHEN 'B' THEN 'Bloqueado'
								WHEN 'I' THEN 'Inativo'
								ELSE 'Pendente'
							END ) WHEN 'Ativo'
						THEN
							(CASE diretor.pesstatus
								WHEN 'A' THEN 'Ativo'
								WHEN 'B' THEN 'Bloqueado'
								WHEN 'I' THEN 'Inativo'
								ELSE 'Pendente'
							END )
						ELSE
							( CASE diretor.suscod
								WHEN 'A' THEN 'Ativo'
								WHEN 'B' THEN 'Bloqueado'
								WHEN 'I' THEN 'Inativo'
								ELSE 'Pendente'
							END )
					END as susdsc,
					CASE WHEN pde.pditempdeescola=TRUE THEN '<img src=../imagens/check.jpg border=0>' 
					ELSE '-' END as tempdeescola
					
			FROM 
				pdeinterativo.pdinterativo pde 
			LEFT JOIN (SELECT pes.usucpf, pes.pesnome, ptp.pdeid, usu.usuemail, ususis.suscod, pes.pesstatus from pdeinterativo.pessoa pes 
				 	   INNER JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = pes.pesid 
	   				   INNER JOIN seguranca.usuario usu on usu.usucpf = pes.usucpf 
	   				   INNER JOIN seguranca.usuario_sistema ususis ON usu.usucpf = ususis.usucpf AND ususis.susstatus = 'A' AND ususis.sisid = ".SISID_PDE_INTERATIVO."
	   				   WHERE pes.pesstatus <> 'I' AND ptp.tpeid=".TPE_DIRETOR." AND pes.pflcod=".PDEESC_PERFIL_DIRETOR." AND pdeid is not null) diretor ON diretor.pdeid = pde.pdeid 
			LEFT JOIN seguranca.usuario us on us.usucpf = diretor.usucpf 
			LEFT JOIN territorios.municipio mun on mun.muncod = pde.muncod
			WHERE 1=1
			".($arrWhere ? " AND ".implode(" AND ",$arrWhere) : "")."
			AND pdistatus='A'";
	
	$cabecalho = array("Ação","Código INEP","Nome da Escola", "Esfera da Escola", "Município", "UF", "CPF do Diretor","Nome do Diretor","E-mail","Status","Tem PDE Escola");
	$db->monta_lista($sql,$cabecalho,50,10,"N","center","N","","","");
}

function listarEscolas()
{
	global $db;
	
	if($_POST){
		extract($_POST);
	}

	//$arrWhere[] = "pde.pdistatus='A'";
	if($pdienergiaeletricacenso){
		$arrWhere[] = "pdienergiaeletricacenso=".$pdienergiaeletricacenso;
	}
	if($pdienergiaeletrica){
		$arrWhere[] = "pdienergiaeletrica=".$pdienergiaeletrica;
	}
	if($pdipossuicoordenadasgeograficas){
		$arrWhere[] = "pdipossuicoordenadasgeograficas=".$pdipossuicoordenadasgeograficas;
	}
	if($pdenome){
		$arrWhere[] = "removeacento(pdenome) ilike removeacento(('%$pdenome%'))";
	}
	if($pditempdeescola){
		$arrWhere[] = "pditempdeescola={$pditempdeescola}";
	}
	if($pdicodinep){
		$arrWhere[] = "pdicodinep = '$pdicodinep'";
	}

	if($pdiesfera){
		$arrWhere[] = "pdiesfera = '".trim($pdiesfera)."'";
	}
	if($usunome){
		$arrWhere[] = "removeacento(usunome) ilike removeacento(('%$usunome%'))";
	}
	if($usucpf){
		$arrWhere[] = "usucpf = '".str_replace(array("-","."),"",$usucpf)."'";
	}
	if($entid){
		$arrWhere[] = "pdicodinep in (select pdicodinep from pdeinterativo.pdinterativo where entid in (".implode(",",$entid)."))";
	}
	if($aedid) {
		if(is_numeric($aedid)) $arrWhere[] = "aedid = '".trim($aedid)."'";
		elseif($aedid=="emelaboracao") $arrWhere[] = "(docid IS NOT NULL AND aedid IS NULL AND percent>0)";
		else $arrWhere[] = "(docid IS NOT NULL AND percent=0)";
	}
	if($aedidf) {
		if(is_numeric($aedidf)) $arrWhere[] = "aedidf = '".trim($aedidf)."'";
		else $arrWhere[] = "aedidf IS NULL";
	}
	
	if($tramitacaoinicio && $tramitacaofim) {
		$arrWhere[] = "(htddata>='".formata_data_sql($tramitacaoinicio)." 00:00:00' AND htddata<='".formata_data_sql($tramitacaofim)." 23:59:59')";
	}
	if($tramitacaoiniciof && $tramitacaofimf) {
		$arrWhere[] = "(htddataf>='".formata_data_sql($tramitacaoiniciof)." 00:00:00' AND htddataf<='".formata_data_sql($tramitacaofimf)." 23:59:59')";
	}
	$arrPerfil = pegaPerfilGeral();

	if(!$db->testa_superuser()) {
		
		if(in_array(PDEINT_PERFIL_CONSULTA_ESTADUAL, $arrPerfil)) {
			$paramperfil[] 		  = " (pdiesfera = 'Estadual' AND estuf IS NOT NULL)";
		} else {
			if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil)) {
				$paramperfil[] 		  = " (pdiesfera = 'Estadual' AND estuf='".$estuf."' AND pdigeridapde = TRUE)";
			}
			if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL, $arrPerfil)) {
				$paramperfil[] 		  = " (pdiesfera = 'Municipal' AND muncod='".$muncod."' AND pdigeridapde = TRUE)";
			}
			if(in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL, $arrPerfil)) {
				$paramperfil[] 		  = " (pdiesfera = 'Estadual' AND estuf='".$estuf."' AND pdigeridapde = FALSE)";
			}
			if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL, $arrPerfil)) {
				$paramperfil[] 		  = " (pdiesfera = 'Municipal' AND muncod='".$muncod."' AND pdigeridapde = FALSE)";
			}
			if(in_array(PDEINT_PERFIL_CONSULTA_MUNICIPAL, $arrPerfil)) {
				$paramperfil[] 		  = " (pdiesfera = 'Municipal' AND muncod='".$muncod."')";
			}
		}
		if($estuf){
			$arrWhere[] = "estuf = '$estuf'";
		}
		if($muncod){
			$arrWhere[] = "muncod = '$muncod'";
		}
		if($paramperfil) {
			$arrWhere[] = "(".implode(" OR ",$paramperfil).")";
		}
	} else {
		if($estuf){
			$arrWhere[] = "estuf = '$estuf'";
		}
		if($muncod){
			$arrWhere[] = "muncod = '$muncod'";
		}
	}
	
	if($db->testa_superuser() || in_array(PDEINT_PERFIL_EQUIPE_MEC, $arrPerfil) || in_array(PDEINT_PERFIL_EQUIPE_FNDE, $arrPerfil)) {
		$acao = "(CASE WHEN esdid = ".WF_ESD_VALIDADO_MEC." THEN '<span style=\"white-space:nowrap;\" ><img class=\"link\" onclick=\"visualizarPDE(\'' || usucpf || '\')\" src=\"../imagens/consultar.gif\" /> <img class=\"link\" onclick=\"informarPagamento(\'' || usucpf || '\')\" src=\"../imagens/money.gif\" /> <img src=\"../imagens/editar_nome_vermelho.gif\" style=cursor:pointer; border=0 onclick=\"wf_exibirHistorico( '||docid||' );\" /></span>'
					   WHEN usucpf IS NULL THEN '-'
					   ELSE '<span style=\"white-space:nowrap;\" ><img class=\"link\" onclick=\"visualizarPDE(\'' || usucpf || '\')\" src=\"../imagens/consultar.gif\" /> '|| CASE WHEN docid IS NULL THEN '' ELSE '<img src=\"../imagens/editar_nome_vermelho.gif\" border=0 style=cursor:pointer; onclick=\"wf_exibirHistorico( '||docid||' );\" />' END ||'</span>'
				  END) ";
		$campo = ", pagamento";
	}else{
		$acao = "(CASE WHEN usucpf IS NULL THEN '-'
					  ELSE '<span style=\"white-space:nowrap;\" ><img class=\"link\" onclick=\"visualizarPDE(\'' || usucpf || '\')\" src=\"../imagens/consultar.gif\" /> '|| CASE WHEN docid IS NULL THEN '' ELSE '<img src=\"../imagens/editar_nome_vermelho.gif\" border=0 style=cursor:pointer; onclick=\"wf_exibirHistorico( '||docid||' );\" />' END ||'</span>' END)";
		$campo = ", pagamento";
	}
	
	/* Adaptação do monta lista para ordenar data */
	// coluna 12 é referente a data de tramitação
	if($_POST['ordemlista']==12) $_REQUEST['ordemlista'] = 'htddata';
	if($_POST['ordemlista']==14) $_REQUEST['ordemlista'] = 'htddataf';
	/* FIM Adaptação do monta lista para ordenar data */
	
	$sql = "SELECT 
				$acao as acao,
				foto,
				pdicodinep,
				pdenome,
				pdiesfera,
				mundescricao,
				estuf,
				usucpfdiretor,
				usunome,
				usuemail,
				realizado,
				to_char(datatramitacao,'dd/mm/YYYY HH24:MI') as datatramitacao,
				percent,
				realizadof,
				to_char(datatramitacaof,'dd/mm/YYYY HH24:MI') as datatramitacaof
				$campo,
				tempdeescola
			FROM pdeinterativo.listapdeinterativo 
			".($arrWhere ? " WHERE ".implode(" and ",$arrWhere) : "");
	
	$cabecalho = array("Ação","F","Código INEP","Nome da Escola", "Esfera", "Município", "UF", "CPF do Diretor","Nome do Diretor","E-mail","Situação (PDE)","Data de Tramitação (PDE)","Preenchimento do PDE (%)","Situação (Formação)","Data de Tramitação (Formação)","Situação do Pagamento - Motivo","Tem PDE Escola");
	if(CACHE_MEM) {
		$db->monta_lista($sql,$cabecalho,50,10,"N","center","N","","","",3600);
	} else {
		$db->monta_lista($sql,$cabecalho,50,10,"N","center","N","","","");
	}
}


function alterarEstadoDocumentoListapdeinterativo($pdeid,$estado)
{
	
}

function recuperaDiretorPorCPF()
{
	global $db;
	$usucpf = $_POST['usucpf'];
	$pdicodinep = $_POST['pdicodinep'];
	
	//Testa se o CPF pode ser atribuído para um Diretor
	$sql = "select 
				count(*) 
			from 
				seguranca.usuario usu
			inner join
				pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf 
			inner join 
				pdeinterativo.pessoatipoperfil ptp on ptp.pesid=pes.pesid
			inner join
				seguranca.perfilusuario per ON per.usucpf = usu.usucpf
			inner join
				seguranca.statususuario sus ON sus.suscod = usu.suscod
			where
				pesstatus = 'A'
			and
				usu.usucpf = '$usucpf'
			and
				per.pflcod in (".PDEINT_PERFIL_COMITE_ESTADUAL.",".PDEINT_PERFIL_COMITE_MUNICIPAL.") 
			and 
				ptp.tpeid=".TPE_DIRETOR."";
	
	if($db->pegaUm($sql)>0){
		echo simec_json_encode( array("naopode" => ICONV( "ISO-8859-1", "UTF-8", "O CPF informado faz parte do comitê!" ) ) );
		die;
	}
	
	$sql = "select 
				count(*) 
			from 
				seguranca.usuario usu
			inner join
				pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf 
			inner join 
				pdeinterativo.pessoatipoperfil ptp on ptp.pesid=pes.pesid
			inner join
				seguranca.perfilusuario per ON per.usucpf = usu.usucpf
			inner join
				seguranca.statususuario sus ON sus.suscod = usu.suscod
			where
				pesstatus = 'A'
			and
				usu.usucpf = '$usucpf'
			".($pdicodinep ? " and pdeid != (select pdeid from pdeinterativo.pdinterativo where pdistatus = 'A' and pdicodinep::integer = $pdicodinep limit 1) " : " and pdeid is not null ")."
			and 
				ptp.tpeid=".TPE_DIRETOR."
			and
				per.pflcod in (".PDEESC_PERFIL_DIRETOR.")";
	//dbg($sql,1);
	if($db->pegaUm($sql)>0){
		echo simec_json_encode( array("naopode" => ICONV( "ISO-8859-1", "UTF-8", "O CPF informado já é diretor de uma escola!" ) ) );
		die;
	}
	
	if($pdicodinep){
		$sql = "select 
					count(*) 
				from 
					seguranca.usuario usu
				inner join
					pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf 
				inner join 
					pdeinterativo.pessoatipoperfil ptp on ptp.pesid=pes.pesid
				inner join
					seguranca.perfilusuario per ON per.usucpf = usu.usucpf
				inner join
					seguranca.statususuario sus ON sus.suscod = usu.suscod
				where
					usu.usucpf != '$usucpf'
				and
					pesstatus = 'A'
				and
					pdeid = (select pdeid from pdeinterativo.pdinterativo where pdistatus = 'A' and pdicodinep::integer = $pdicodinep limit 1)
				and
					per.pflcod in (".PDEESC_PERFIL_DIRETOR.") 
				and 
					ptp.tpeid=".TPE_DIRETOR."";
		if($db->pegaUm($sql)>0){
			$confirm = ICONV( "ISO-8859-1", "UTF-8", "Deseja realmente remover o diretor antigo da escola?" );
		}
	}
	
	$sql = "select DISTINCT
				usu.*,
				CASE pesstatus
					WHEN 'A' THEN 'A'
					WHEN 'B' THEN 'B'
					ELSE 'P'
				END as status,
				( select htudsc from seguranca.historicousuario htu where htu.usucpf = usu.usucpf order by htuid desc limit 1) as justificativa
			from 
				seguranca.usuario usu
			inner join
				seguranca.statususuario sus ON sus.suscod = usu.suscod
			left join
				pdeinterativo.pessoa pde ON pde.usucpf = usu.usucpf
			where 
				usu.usucpf = '$usucpf'";
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		$arrDados = codificaUTF8($arrDados);
		if($confirm){
			$arrDados['confirm'] = $confirm;
		}
		echo simec_json_encode($arrDados);
	}else{
		if($confirm){
			$arrDados['confirm'] = $confirm;
		}
		echo simec_json_encode($arrDados);
	}
	die();
}

function recuperaEscolaPorCodigoINEP()
{
	global $db;
	
	$arrPerfil = pegaPerfilGeral();
	
	if(!$db->testa_superuser() && (in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL,$arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$arrPerfil))){	
		if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$arrPerfil)){
			 $sql = "select 
						mun.estuf,
						mun.muncod
					from 
						pdeinterativo.usuarioresponsabilidade ur
					inner join
						territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
					where 
						usucpf = '{$_SESSION['usucpf']}' 
					and 
						rpustatus = 'A' 
					and 
						pflcod = ".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL;
			$arrUR = $db->pegaLinha($sql);
			$arrTravaConsulta['mun.muncod'] = !$arrUR['muncod'] || $arrUR['muncod'] == "" ? "X" : $arrUR['muncod'];
			$arrTravaConsulta['mun.estuf'] = !$arrUR['estuf'] || $arrUR['estuf'] == "" ? "X" : $arrUR['estuf'];
			$arrTravaConsulta['pdiesfera'] = "Municipal";
			$arrTravaConsulta['pdigeridapde'][] = "FALSE";
		}else{
			$sql = "select 
						estuf 
					from 
						pdeinterativo.usuarioresponsabilidade 
					where 
						usucpf = '{$_SESSION['usucpf']}' 
					and 
						rpustatus = 'A' 
					and 
						pflcod = ".PDEINT_PERFIL_COMITE_PAR_ESTADUAL;
			$arrEstuf = $db->pegaUm($sql);
			$arrTravaConsulta['mun.estuf'] = !$arrEstuf || $arrEstuf == "" ? "X" : $arrEstuf;
			$arrTravaConsulta['pdiesfera'] = "Estadual";
			$arrTravaConsulta['pdigeridapde'][] = "FALSE";
		}
	
	}
	
	if(!$db->testa_superuser() && (in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$arrPerfil) || in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$arrPerfil))){	
		if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$arrPerfil)){
			 $sql = "select 
						mun.estuf,
						mun.muncod
					from 
						pdeinterativo.usuarioresponsabilidade ur
					inner join
						territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
					where 
						usucpf = '{$_SESSION['usucpf']}' 
					and 
						rpustatus = 'A' 
					and 
						pflcod = ".PDEINT_PERFIL_COMITE_MUNICIPAL;
			$arrUR = $db->pegaLinha($sql);
			$arrTravaConsulta['mun.muncod'] = !$arrUR['muncod'] || $arrUR['muncod'] == "" ? "X" : $arrUR['muncod'];
			$arrTravaConsulta['mun.estuf'] = !$arrUR['estuf'] || $arrUR['estuf'] == "" ? "X" : $arrUR['estuf'];
			$arrTravaConsulta['pdiesfera'] = "Municipal";
			$arrTravaConsulta['pdigeridapde'][] = "TRUE";
		}else{
			$sql = "select 
						estuf 
					from 
						pdeinterativo.usuarioresponsabilidade 
					where 
						usucpf = '{$_SESSION['usucpf']}' 
					and 
						rpustatus = 'A' 
					and 
						pflcod = ".PDEINT_PERFIL_COMITE_ESTADUAL;
			$arrEstuf = $db->pegaUm($sql);
			$arrTravaConsulta['mun.estuf'] = !$arrEstuf || $arrEstuf == "" ? "X" : $arrEstuf;
			$arrTravaConsulta['pdiesfera'] = "Estadual";
			$arrTravaConsulta['pdigeridapde'][] = "TRUE";
		}
	
	}

	if($arrTravaConsulta){
		foreach($arrTravaConsulta as $campo => $valor) {
			if(is_array($valor)) $arrWhere[] = "$campo IN(".implode(",",$valor).")";
			else $arrWhere[] = "$campo = '$valor'";
		}
	}
	
	
	$pdicodinep = $_POST['pdicodinep'];
	$sql = "select
				pdi.*,
				mundescricao,
				pes.usucpf
			from
				pdeinterativo.pdinterativo pdi
			left join
				territorios.municipio mun ON mun.muncod = pdi.muncod
			left join
				pdeinterativo.pessoatipoperfil ptp ON ptp.pdeid = pdi.pdeid 
			left join
				pdeinterativo.pessoa pes ON pes.pesid = ptp.pesid 
			where 
				pdicodinep = '$pdicodinep'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			and
				pdistatus = 'A'";
	$arrDados = $db->pegaLinha($sql);
	
	if(!$arrDados['pdenome']){
		
		$sql = "select
					entnome,
					ende.estuf,
					ent.entid,
					ende.muncod,
					endlog,
					endbai,
					endcom,
					endnum,
					tpldesc,
					endcep,
					entnumcomercial,
					entnumdddcomercial,
					CASE WHEN ten.id_dependencia_adm='1' THEN 'Federal' 
						 WHEN ten.id_dependencia_adm='2' THEN 'Estadual' 
						 WHEN ten.id_dependencia_adm='3' THEN 'Municipal'
						 WHEN ten.id_dependencia_adm='4' THEN 'Privada' END as tpcdesc
				from
					entidade.entidade ent
				inner join
					entidade.endereco ende ON ende.entid = ent.entid
				inner join
					pdeinterativo.pdinterativo pdi ON pdi.pdicodinep = ent.entcodent
				left join
					entidade.tipolocalizacao tpl ON ent.tplid = tpl.tplid
				left join
					educacenso_2010.tab_entidade ten ON ten.pk_cod_entidade = pdi.pdicodinep::bigint
				where
					ent.entcodent = '$pdicodinep'
				and
					 pdistatus = 'A'";
		$arrEscola = $db->pegaLinha($sql);
		
		if(!$arrEscola){
			echo simec_json_encode( array("naopode" => ICONV( "ISO-8859-1", "UTF-8", "O código INEP informado não foi priorizado!" ) ) );
			die;
		}
		
		$sql = "update
					pdeinterativo.pdinterativo
				set
					pdenome = '{$arrEscola['entnome']}',
					entid = '{$arrEscola['entid']}',
					estuf = '{$arrEscola['estuf']}',
					muncod = '{$arrEscola['muncod']}',
					pdelogradouro = '{$arrEscola['endlog']}',
					pdebairro = '{$arrEscola['endbai']}',
					pdecomplemento = '{$arrEscola['endcom']}',
					pdilocalizacao = '{$arrEscola['tpldesc']}',
					pdinumero = '{$arrEscola['endnum']}',
					pdecep = '{$arrEscola['endcep']}',
					pdidddtelefone = '{$arrEscola['entnumdddcomercial']}',
					pdinumtelefone = '{$arrEscola['entnumcomercial']}',
					pdiesfera = '{$arrEscola['tpcdesc']}'
				where
					pdicodinep = '$pdicodinep'";
		
		$db->executar($sql);
		$db->commit();
	}
		
	$sql = "select
			pdi.*,
			mundescricao,
			pes.usucpf
		from
			pdeinterativo.pdinterativo pdi
		left join
			territorios.municipio mun ON mun.muncod = pdi.muncod 
		left join
			pdeinterativo.pessoatipoperfil ptp ON ptp.pdeid = pdi.pdeid and ptp.tpeid=".TPE_DIRETOR." 
		left join
			pdeinterativo.pessoa pes ON pes.pesid = ptp.pesid and pes.pflcod = ".PDEESC_PERFIL_DIRETOR."
		where 
			pdicodinep = '$pdicodinep'
		".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
		and
			pdistatus = 'A'";
	
	$arrDados = $db->pegaLinha($sql);
	
	if($arrTravaConsulta && ($arrTravaConsulta['mun.estuf']&& $arrDados['estuf'] != $arrTravaConsulta['mun.estuf']) || ($arrTravaConsulta['mun.muncod'] && $arrDados['muncod'] != $arrTravaConsulta['mun.muncod'])){
		if($arrTravaConsulta['pdiesfera'] == "Estadual"){
			echo simec_json_encode( array("naopode" => ICONV( "ISO-8859-1", "UTF-8", "Você não tem perfil para cadastrar diretores deste Estado!" ) ) );
		}else{
			echo simec_json_encode( array("naopode" => ICONV( "ISO-8859-1", "UTF-8", "Você não tem perfil para cadastrar diretores deste Município!" ) ) );
		}
		die;
	}
		
	
	if($arrDados){
		$arrDados = codificaUTF8($arrDados);
		echo simec_json_encode($arrDados);
	}
	die();
}

function codificaUTF8($array = array())
{
	foreach($array as $key => $texto){
		$arrUTF8[$key] = ICONV( "ISO-8859-1", "UTF-8", $texto );
	}
	return $arrUTF8;
}


function salvarDiretor()
{
	global $db;
	
	
	if(!$_REQUEST['entid'] || $_REQUEST['entid'] == null || $_REQUEST['entid'] == ""){
		$_SESSION['pdeinterativo']['msg'] = "Escola não encontrada!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
	}
	
	//Atribui ao usuário UF e Município igual ao da escola
	$sql = "select estuf, muncod, pdiesfera from pdeinterativo.pdinterativo where entid = {$_REQUEST['entid']} and pdistatus = 'A'";
	$arrD = $db->pegaLinha($sql);
	
	$_POST['muncod'] = $arrD['muncod'];
	$_POST['estuf'] = $arrD['estuf'];
	$_POST['tpocod'] = $arrD['pdiesfera'] == "Estadual" ? 2 : 3;
		
	extract($_POST);
		
	if($naopode){
		$_SESSION['pdeinterativo']['msg'] = $naopode;
		header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
		exit;
	}
	
	if(!$usucpf || !$usuemail || !$usufoneddd || !$usufonenum || !$rdo_status || !$entid){
		$_SESSION['pdeinterativo']['msg'] = "Favor preencher todos os campos obrigatórios!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
		exit;
	}
	
	if( !preg_match( "/^([a-zA-Z0-9])+([a-zA-Z0-9\._-])*@([a-zA-Z0-9_-])+([a-zA-Z0-9\._-]+)+$/", $usuemail)){
		$_SESSION['pdeinterativo']['msg'] = "E-mail inválido!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
		exit;
	}
	if( $usuemail != $cousuemail ){
		$_SESSION['pdeinterativo']['msg'] = "Confirmação de E-mail inválida!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
		exit;
	}
	
	if(!ativaUsuarioPDEInterativo($_POST,PDEESC_PERFIL_DIRETOR,$entid,3)){
		$_SESSION['pdeinterativo']['msg'] = "Não foi possível cadastrar o usuário no SIMEC!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
		exit;
	}
	
	$usucpf = "'".substr(str_replace(array(".","-"),"",$usucpf),0,11)."'";
	$pesnome = "'$usunome'";
	$pflcod = PDEESC_PERFIL_DIRETOR;
	$pdeid = recuperaPdeidPorCodigoINEP($pdicodinep);
	$pdeid = !$pdeid ? "null" : $pdeid;
	
	$sql = "select pesid from pdeinterativo.pessoa where usucpf = $usucpf";
	$pesid = $db->pegaUm($sql);
	
	if($pdeid) {
		$sql = "update pdeinterativo.pessoa set pflcod=NULL where pesid in(select pesid from pdeinterativo.pessoatipoperfil where tpeid=".TPE_DIRETOR." and pdeid=".$pdeid.")";
		$db->executar($sql);
		$db->commit();
		
		$sql = "delete from pdeinterativo.pessoatipoperfil where tpeid=".TPE_DIRETOR." and pdeid=".$pdeid."";
		$db->executar($sql);
		$db->commit();
	} else {
		$_SESSION['pdeinterativo']['msg'] = "Não foi possível identificar o Código INEP da escola selecionada!";
		header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
		exit;
	}
	
	if(!$pesid){
		
		$sql = "insert into 
					pdeinterativo.pessoa
				(usucpf,pesnome,pesstatus,pflcod)
					values
				($usucpf,$pesnome,'$rdo_status',$pflcod) returning pesid";
		$pesid = $db->pegaUm($sql);
	}else{
		$sql = "update 
					pdeinterativo.pessoa
				set
					pesstatus = '$rdo_status',
					pflcod = $pflcod
				where
					pesid = $pesid";
		$db->executar($sql);
	}
	$db->commit();
	
	$sql = "select 
				pesid 
			from 
				pdeinterativo.pessoatipoperfil
			where
				pesid = $pesid
			and
				tpeid = ".TPE_DIRETOR."";
	if(!$db->pegaUm($sql)){
		$sql = "insert into pdeinterativo.pessoatipoperfil (pesid,tpeid,pdeid) values ($pesid,".TPE_DIRETOR.",$pdeid);";
		$db->executar($sql);
		$db->commit();
	}
	
	$sql = "INSERT INTO pdeinterativo.historicocadastrodiretor(
            hcdinep, hcdcpfdiretor, hcddata, hcdcpf, hcdacao)
    		VALUES (".(($pdicodinep)?"'".$pdicodinep."'":"NULL").", 
    				".$usucpf.", 
    				NOW(), 
    				'".$_SESSION['usucpf']."', 
    				'salvarDiretor');";
	
	$db->executar($sql);
	
	$db->commit();
	
	$_SESSION['pdeinterativo']['msg'] = "Operação realizada com sucesso!";
	header("Location: pdeinterativo.php?modulo=principal/cadastroDiretor&acao=A");
	exit;
}

function recuperaPdeidPorCodigoINEP($pdicodinep)
{
	global $db;
	
	if(!$pdicodinep){
		return false;
	}
	$sql = "select pdeid from pdeinterativo.pdinterativo where pdicodinep = '$pdicodinep' and pdistatus = 'A'";
	return $db->pegaUm($sql);
}

function carregarDiretor()
{
	return array("usucpf" => $_POST['usucpf'],"pdicodinep" => $_POST['pdicodinep']);
}

function listaEscolas()
{
	global $db;
	
	if($_POST){
		extract($_POST);
	}
	
	$arrPerfil = pegaPerfilGeral();
	
	if(!$db->testa_superuser()) {
		
		if(in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$arrPerfil)){
			$sql = "select mun.muncod
					from pdeinterativo.usuarioresponsabilidade ur
					inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_MUNICIPAL."
					order by rpuid desc";
			
			$muncod = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Municipal' AND mun.muncod='".$muncod."' AND pde.pdigeridapde = TRUE)";
		}
		
		if(in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL,$arrPerfil)){
			$sql = "select mun.muncod
					from pdeinterativo.usuarioresponsabilidade ur
					inner join territorios.municipio mun ON mun.muncod::integer = ur.muncod::integer
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL."
					order by rpuid desc";
			
			$muncod = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Municipal' AND mun.muncod='".$muncod."' AND pde.pdigeridapde = FALSE)";
		}
		
		if(in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$arrPerfil)){
			$sql = "select estuf
					from pdeinterativo.usuarioresponsabilidade ur
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_ESTADUAL."
					order by rpuid desc";
	
			$estuf = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Estadual' AND mun.estuf='".$estuf."' AND pde.pdigeridapde = TRUE)";
			
		}
		
		if(in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL,$arrPerfil)){
			$sql = "select estuf
					from pdeinterativo.usuarioresponsabilidade ur
					where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = ".PDEINT_PERFIL_COMITE_PAR_ESTADUAL."
					order by rpuid desc";
	
			$estuf = $db->pegaUm($sql);
			$paramperfil[] = "(pde.pdiesfera = 'Estadual' AND mun.estuf='".$estuf."' AND pde.pdigeridapde = FALSE)";
			
		}
		
		if($paramperfil) {
			
			$arrWhere[] = "(".implode(" OR ",$paramperfil).")";
			
		} else {
			
			if($estuf){
				$arrWhere[] = "mun.estuf = '$estuf'";
			}
			if($muncod){
				$arrWhere[] = "mun.muncod = '$muncod'";
			}
			
		}
	} else {
			
		if($estuf){
			$arrWhere[] = "mun.estuf = '$estuf'";
		}
		if($muncod){
			$arrWhere[] = "mun.muncod = '$muncod'";
		}
		
	}
	
	$arrWhere[] = "pdistatus='A'";
	
	if($pdenome){
		$arrWhere[] = "removeacento(pdenome) ilike removeacento(('%$pdenome%'))";
	}
	if($id_dependencia_adm){
		$arrWhere[] = "ten.id_dependencia_adm = '".trim($id_dependencia_adm)."'";
	}
		
	$sql = "select
				'<input type=\"radio\" name=\"rdo_inep\" onclick=\"selecionaINEP(\'' || pde.pdicodinep || '\')\"  />' as acao,
				pde.pdicodinep,
				pde.pdenome,
				CASE WHEN id_dependencia_adm='1' THEN 'FEDERAL'
					 WHEN id_dependencia_adm='2' THEN 'ESTADUAL'
					 WHEN id_dependencia_adm='3' THEN 'MUNICIPAL'
					 WHEN id_dependencia_adm='4' THEN 'PRIVADA' END as tpcdesc,
				mun.mundescricao,
				mun.estuf
			from
				pdeinterativo.pdinterativo pde
			inner join
				territorios.municipio mun ON mun.muncod = pde.muncod
			inner join
				educacenso_2011.tab_entidade ten ON ten.pk_cod_entidade = pde.pdicodinep::bigint 
			where
				1 = 1 
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."";
	
	$cabecalho = array("Ação","Código INEP","Nome da Escola", "Esfera", "Município", "UF");
	$db->monta_lista($sql,$cabecalho,50,10,"N","center","N");
	
}

function excluirDiretor()
{
	global $db;
	
	global $db;
	$usucpf = substr($_POST['usucpf'],0,11);
	$pflcod = PDEESC_PERFIL_DIRETOR;
	if($pflcod){
		$sqlD = "delete from seguranca.perfilusuario where usucpf = '$usucpf' and pflcod = $pflcod;
				 delete from pdeinterativo.usuarioresponsabilidade where usucpf = '$usucpf' and pflcod = '$pflcod';";
		$db->executar($sqlD);
		$db->commit();
	}
	
	$sql = "delete from seguranca.usuario_sistema where usucpf = '$usucpf' and sisid = ".SISID_PDE_INTERATIVO;
	$db->executar($sql);
	$db->commit();
	
	$sql = "delete from pdeinterativo.pessoatipoperfil where pesid in(select pesid from pdeinterativo.pessoa where usucpf='".$usucpf."') AND tpeid=".TPE_DIRETOR;
	$db->executar($sql);
	$db->commit();
	
	$sql = "update pdeinterativo.pessoa	set	pflcod=NULL where usucpf = '$usucpf'";
	$db->executar($sql);
	$db->commit();
	
	$sql = "INSERT INTO pdeinterativo.historicocadastrodiretor(
            hcdinep, hcdcpfdiretor, hcddata, hcdcpf, hcdacao)
    		VALUES (NULL, 
    				".(($usucpf)?"'".$usucpf."'":"NULL").", 
    				NOW(), 
    				'".$_SESSION['usucpf']."', 
    				'excluirDiretor');";
	
	$db->executar($sql);
	$db->commit();
	
	// envia o email
	$sql = "select usunome,usuemail from seguranca.usuario where usucpf = '$usucpf'";
	$usu = $db->pegaLinha($sql);
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO PDE-INTERATIVO","email" => $usuemail);
	$destinatario = $usu['usuemail'];
	$assunto = "Cadastro no SIMEC - MÓDULO PDE-INTERATIVO";	
	$conteudo .= sprintf(
	"%s %s, <p>Seu cadastro no módulo PDE Interativo / SIMEC não foi aprovado. Reveja o módulo e o perfil solicitado. Em caso de dúvidas, entre em contato com a sua Secretaria.</p>",
	'Prezado(a)',
	$usu['usunome']);
	if($usu && !strstr($_SERVER['HTTP_HOST'],"simec-local")){
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	}
	
	//dbg($sql,1);
	$_SESSION['pdeinterativo']['msg'] = "Diretor excluído com sucesso!";
	header("Location: pdeinterativo.php?modulo=principal/listaDiretor&acao=A");
	exit;
}

function verificaReenvioEmail()
{
	global $db;
	$usucpf = trim(str_replace(array("-","."),"",$_POST['usucpf']));
	$usuemail = trim($_POST['usuemail']);
	
	$sql = "select usuemail from seguranca.usuario where usucpf = '$usucpf'";
	$email = $db->pegaUm($sql);
	if(!$email){
		return false;
	}
	if($email && $email != $usuemail){
		echo "diferente";
		return false;
	}else{
		return true;
	}
}

function recuperaDadosDiretor($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select 
				usu.*,
				CASE WHEN ususexo = 'M'
					THEN 'Masculino'
					ELSE 'Feminino'
				END as genero
			from 
				seguranca.usuario usu
			inner join
				seguranca.perfilusuario pfl ON usu.usucpf = pfl.usucpf
			inner join
				pdeinterativo.pessoa pes ON pes.usucpf = usu.usucpf
			inner join 
				pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = pes.pesid and ptp.tpeid=".TPE_DIRETOR." 
			inner join
				pdeinterativo.usuarioresponsabilidade rpu ON usu.usucpf = rpu.usucpf AND pfl.pflcod = rpu.pflcod AND rpustatus = 'A'
			inner join
				pdeinterativo.pdinterativo pde ON pde.pdeid = ptp.pdeid
			where 
				pesstatus = 'A'
			and
				pde.pdeid = '$pdeid'
			and
				pes.pflcod = ".PDEESC_PERFIL_DIRETOR.";";
	
	return $db->pegaLinha($sql);
}


function confirmaDadosDiretor()
{
	global $db;
	
	extract($_POST);
	
	if(!$usucpf || !$usuemail || !$usufoneddd || !$usufonenum){
		$_SESSION['pdeinterativo']['msg'] = "Favor preencher todos os campos obrigatórios!";
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A");
		exit;
	}
	
	if( !preg_match( "/^([a-zA-Z0-9])+([a-zA-Z0-9\._-])*@([a-zA-Z0-9_-])+([a-zA-Z0-9\._-]+)+$/", $usuemail)){
		$_SESSION['pdeinterativo']['msg'] = "E-mail inválido!";
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A");
		exit;
	}
	
	if( $usuemail != $cousuemail ){
		$_SESSION['pdeinterativo']['msg'] = "Confirmação de E-mail inválida!";
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A");
		exit;
	}

	$usucpf = "'".str_replace(array(".","-"),"",$usucpf)."'";
	
//	if($hdn_reenvia_email){
//		$senhageral = $db->gerar_senha();
//		$ususenha = md5_encrypt_senha( $senhageral, '' );
//	}
	
	if($usucelddd && $usucelnum) {
		
		$uscid = $db->pegaUm("SELECT uscid FROM pdeinterativo.usuariocelular WHERE usucpf = {$usucpf}");
		
		if($uscid) {
			
			$sql = "UPDATE pdeinterativo.usuariocelular
					SET usucelddd='".$usucelddd."', usucelnum='".$usucelnum."'
					WHERE  usucpf={$usucpf}";
			
		} else {
			
			$sql = "INSERT INTO pdeinterativo.usuariocelular(
	            	usucelddd, usucelnum, usucpf)
	    			VALUES ('".$usucelddd."', '".$usucelnum."', {$usucpf});";
		}
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	/* Comentado pelo Alexandre - não permitir alteração de senha nesta tela
	$sql = "update 
				seguranca.usuario 
			set
				usuemail = '$usuemail',
				usufoneddd = '$usufoneddd',
				usufonenum = '$usufonenum',
				ususenha = '$ususenha'
			where 
				usucpf = $usucpf;
		   select usunome from seguranca.usuario where usucpf = $usucpf;";
	*/
	
	$sql = "update 
				pdeinterativo.detalhepessoa 
			set
				dpeemail = '$usuemail',
				dpetelefone = '".str_replace("-","",$usufoneddd.$usufonenum)."'
			where 
				pesid in(select pesid from pdeinterativo.pessoa where usucpf=$usucpf);";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update 
				seguranca.usuario 
			set
				usuemail = '$usuemail',
				usufoneddd = '$usufoneddd',
				usufonenum = '$usufonenum'
			where 
				usucpf = $usucpf;
		   select usunome from seguranca.usuario where usucpf = $usucpf;";

	$usunome = $db->pegaUm($sql);
	$db->commit();
	
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO PDE-INTERATIVO","email" => $usuemail);
	$destinatario = $usuemail;
	$assunto = "Cadastro no SIMEC - MÓDULO PDE-INTERATIVO";
	$conteudo = "
		<br/>
		<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
		<br/>
		<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
		<br/><br/>
		";
	if($hdn_reenvia_email){
		$conteudo .= sprintf(
		"%s %s, <p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
		'Prezado(a)',
		$usunome,
		$senhageral
		);
		$conteudo .= "<br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.";
	}
	
	if($hdn_reenvia_email && !strstr($_SERVER['HTTP_HOST'],"simec-local")){
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	}
	
	salvarAbaResposta("identificacao_diretor");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Diretor");
	}
	
	exit;
	
}

function recuperaDadosEscolaPorCPFDiretor($usucpf = null)
{
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$sql = "select 
				pdi.*,
				usucpf,
				mun.mundescricao,
				mun.muncod,
				CASE WHEN (select distinct id_ens_fundamental_ciclos from educacenso_2010.tab_dado_escola where fk_cod_entidade::bigint = pdicodinep::bigint) = 1
					THEN 'Sim'
					ELSE 'Não'
				END as ciclo
			from 
				pdeinterativo.pdinterativo pdi 
			inner join
				pdeinterativo.pessoatipoperfil ptp ON ptp.pdeid = pdi.pdeid and ptp.tpeid=".TPE_DIRETOR."  
			inner join
				pdeinterativo.pessoa pes ON pes.pesid = ptp.pesid
			inner join
				territorios.municipio mun ON pdi.muncod = mun.muncod 
			where
				usucpf = '$usucpf'
			and
				pesstatus = 'A'
			and
				pdistatus = 'A'
			order by
				pdeid";
	//dbg($sql);
	return $db->pegaLinha($sql);
	
}

function gerenciarCargaCapitalCusteio($dados) {
	global $db;
	
	$cccid = $db->pegaUm("SELECT cccid FROM pdeinterativo.cargacapitalcusteio WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	
	if($cccid) {
		
		$sql = "UPDATE pdeinterativo.cargacapitalcusteio
				SET ccccapitalprimeira=".(($dados['ccccapitalprimeira'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccapitalprimeira'])."'":"0").", 
					ccccapitalsegunda=".(($dados['ccccapitalsegunda'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccapitalsegunda'])."'":"0").", 
					ccccusteioprimeira=".(($dados['ccccusteioprimeira'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccusteioprimeira'])."'":"0").", 
	            	ccccusteiosegunda=".(($dados['ccccusteiosegunda'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccusteiosegunda'])."'":"0").", 
	            	ccccapitalvlrtotal=".(($dados['ccccapitalvlrtotal'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccapitalvlrtotal'])."'":"0").", 
	            	ccccusteiovlrtotal=".(($dados['ccccusteiovlrtotal'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccusteiovlrtotal'])."'":"0")."
				WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
		
	} else {
	
		$sql = "INSERT INTO pdeinterativo.cargacapitalcusteio(
	            pdeid, ccccapitalprimeira, ccccapitalsegunda, ccccusteioprimeira, 
	            ccccusteiosegunda, ccccapitalvlrtotal, ccccusteiovlrtotal, cccstatus, 
	            cccanoprimeira, cccanosegunda, codinep)
	    		VALUES (".$_SESSION['pdeinterativo_vars']['pdeid'].", 
	    				".(($dados['ccccapitalprimeira'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccapitalprimeira'])."'":"0").", 
	    				".(($dados['ccccapitalsegunda'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccapitalsegunda'])."'":"0").", 
	    				".(($dados['ccccusteioprimeira'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccusteioprimeira'])."'":"0").", 
	            		".(($dados['ccccusteiosegunda'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccusteiosegunda'])."'":"0").", 
	            		".(($dados['ccccapitalvlrtotal'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccapitalvlrtotal'])."'":"0").", 
	            		".(($dados['ccccusteiovlrtotal'])?"'".str_replace(array(".",","," "),array("",".",""),$dados['ccccusteiovlrtotal'])."'":"0").", 
	            		'A', 
	            		'2011', 
	            		'2012', 
	            		'".$_SESSION['pdeinterativo_vars']['pdicodinep']."');";
	
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Dados atualizados com sucesso');
			window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=".$_REQUEST['aba']."&aba1=".$_REQUEST['aba1']."';
		  </script>";
	
}



function arrayPerfil(){
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid=".SISID_PDE_INTERATIVO."
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
			$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}


function confirmaDadosEscola()
{
	global $db;
	extract($_POST);
	
	if(!$pdecep || !$pdicodinep || !$pdelogradouro || !$pdidddtelefone || !$pdinumtelefone){
		$_SESSION['pdeinterativo']['msg'] = "Favor preencher todos os campos obrigatórios!";
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola");
		exit;
	}
	
	if($pdeemail){
		if( !preg_match( "/^([a-zA-Z0-9])+([a-zA-Z0-9\._-])*@([a-zA-Z0-9_-])+([a-zA-Z0-9\._-]+)+$/", $pdeemail)){
			$_SESSION['pdeinterativo']['msg'] = "E-mail inválido!";
			header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola");
			exit;
		}
		$pdeemail = "'$pdeemail'";
	}else{
		$pdeemail = "NULL";
	}
	
	if($graulatitude && $minlatitude && $seglatitude && $pololatitude){
		$medlatitude = "$graulatitude.$minlatitude.$seglatitude.$pololatitude";
	}
	
	if($graulongitude && $minlongitude && $seglongitude){
		$medlongitude = "$graulongitude.$minlongitude.$seglongitude.W";
	}
	
	
	$sql = "update
					pdeinterativo.pdinterativo
				set
					pdelogradouro = '$pdelogradouro',
					pdebairro = '$pdebairro',
					pdecomplemento = '$pdecomplemento',
					pdinumero = '$pdinumero',
					pdecep = '$pdecep',
					pdeemail = $pdeemail,
					pdidddtelefone = '$pdidddtelefone',
					pdinumtelefone = '$pdinumtelefone',
					medlatitude = ".($medlatitude ? "'$medlatitude'" : "null").",
					medlongitude = ".($medlongitude ? "'$medlongitude'" : "null").",
					pdienergiaeletrica = ".(($pdienergiaeletrica)?$pdienergiaeletrica:"NULL")." 
				where
					pdicodinep = '$pdicodinep'";
		
	$db->executar($sql);
	$db->commit();
	
	salvarAbaResposta("identificacao_escola");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Galeria");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A&aba=Escola");
	}
	
	exit;
}

function recuperaModalidadesEnsinoPorCodigoINEP($inep)
{
	global $db;
	
	$sql = "select distinct
				modalidade.no_mod_ensino 
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_mod_ensino modalidade ON turma.fk_cod_mod_ensino = modalidade.pk_cod_mod_ensino
			where 
				turma.fk_cod_entidade = $inep
			order by
				modalidade.no_mod_ensino;";
	return $db->carregarColuna($sql);
}

function recuperaNiveisEnsinoPorCodigoINEP($inep)
{
	global $db;
	
	$sql = "select 
				no_etapa_ensino as nivel,
				count(fk_cod_aluno) as qtde_alunos
			from 
				educacenso_2010.tab_matricula matricula
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = matricula.fk_cod_etapa_ensino
			where 
				matricula.fk_cod_entidade = $inep
			and 
				matricula.id_status = 1
			group by
				no_etapa_ensino
			order by
				no_etapa_ensino;";
	return $db->carregar($sql);
}

function recuperaParametroCNE()
{
	global $db;
	
	$sql = "select 
				cneid,
				cnedesc,
				cnenumestturm
			from 
				pdeinterativo.parametrocne
			where 
				cnestatus = 'A'
			order by
				cneposicao,
				cnedesc;";
	return $db->carregar($sql);
}

function recuperaTurmasPorEscola($inep = null)
{
	global $db;
	
	$inep = !$inep ? $_SESSION['pdeinterativo_vars']['pdicodinep'] : $inep;
	
	$sql = "select 
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where pk_cod_turma = fk_cod_turma and t.id_status = 1) as matricula,
				ten.id_localizacao as localizacao
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			left join
				educacenso_2010.tab_entidade ten ON ten.pk_cod_entidade::bigint = turma.fk_cod_entidade::bigint
			where 
				fk_cod_entidade = $inep
			order by
				no_etapa_ensino";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			if($dado['matricula'] != 0){
				if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
					$inicio = strpos($dado['no_etapa_ensino'],"-");
					$fim = strlen($dado['no_etapa_ensino']);
					$arrEF["Ensino Fundamental"][] = array(
									"ensino" => "Ensino Fundamental",
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"turma" => $dado['no_turma'],
									"hrinicio" => $dado['hrinicio'],
									"hrfim" => $dado['hrfim'],
									"nummatricula" => $dado['matricula'],
									"localizacao" => $dado['localizacao'],
									"pk_cod_turma" => $dado['pk_cod_turma']
									);
				}
				elseif(strstr($dado['no_etapa_ensino'],"Educação Infantil")){
					$inicio = strpos($dado['no_etapa_ensino'],"-");
					$fim = strlen($dado['no_etapa_ensino']);
					$arrEF["Educação Infantil"][] = array(
									"ensino" => "Educação Infantil",
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"turma" => $dado['no_turma'],
									"hrinicio" => $dado['hrinicio'],
									"hrfim" => $dado['hrfim'],
									"nummatricula" => $dado['matricula'],
									"pk_cod_turma" => $dado['pk_cod_turma']
									);
				}
				elseif(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
					$inicio = strpos($dado['no_etapa_ensino'],"-");
					$fim = strlen($dado['no_etapa_ensino']);
					$arrEF["Ensino Médio"][] = array(
									"ensino" => "Ensino Médio",
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"turma" => $dado['no_turma'],
									"hrinicio" => $dado['hrinicio'],
									"hrfim" => $dado['hrfim'],
									"nummatricula" => $dado['matricula'],
									"pk_cod_turma" => $dado['pk_cod_turma']
									);
				}
			}
		}
	}
	return $arrEF;
}

function retornaparametroCNE($ensino,$serie = null,$numMatricula,$escola = null)
{
	global $db;
	
	if($ensino == "Ensino Médio"){
		$sql = "select cnenumestturm from pdeinterativo.parametrocne where cnedesc = '$ensino'";
		$cnenumestturm = $db->pegaUm($sql);
		if($numMatricula > $cnenumestturm){
			return true;
		}else{
			return false;
		}
	}elseif($ensino == "Ensino Fundamental"){
		$ano = substr($serie,0,1);
		$tipo = substr($serie,1,1);
		//dbg($tipo);
		if(is_numeric($ano)){
			$escola = $escola == "1" ? "$ensino - " : "$ensino - Escola do campo - ";
			if($tipo == "º"){
				if($ano <= 5){
					$sql = "select cnenumestturm from pdeinterativo.parametrocne where cnedesc ilike ('%{$escola}Anos iniciais%')";
					$cnenumestturm = $db->pegaUm($sql);
					if($numMatricula > $cnenumestturm){
						return true;
					}else{
						return false;
					}
				}elseif($ano > 5){
					$sql = "select cnenumestturm from pdeinterativo.parametrocne where cnedesc ilike ('%{$escola}Anos finais%')";
					$cnenumestturm = $db->pegaUm($sql);
					if($numMatricula > $cnenumestturm){
						return true;
					}else{
						return false;
					}	
				}else{
					return false;
				}
			}else{
				if($ano < 5){
					$sql = "select cnenumestturm from pdeinterativo.parametrocne where cnedesc ilike ('%{$escola}Anos iniciais%')";
					$cnenumestturm = $db->pegaUm($sql);
					if($numMatricula > $cnenumestturm){
						return true;
					}else{
						return false;
					}
				}elseif($ano >= 5){
					$sql = "select cnenumestturm from pdeinterativo.parametrocne where cnedesc ilike ('%{$escola}Anos finais%')";
					$cnenumestturm = $db->pegaUm($sql);
					if($numMatricula > $cnenumestturm){
						return true;
					}else{
						return false;
					}	
				}else{
					return false;
				}	
			}
		}else{
			return "Não existe parâmetro do CNE para esta turma.";
		}
	}elseif($ensino == "Educação Infantil"){
		$sql = "select cnenumestturm from pdeinterativo.parametrocne where cnedesc ilike ('%{$serie}%')";
		$cnenumestturm = $db->pegaUm($sql);
		if($numMatricula > $cnenumestturm){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
	
}

function salvarDistorcaoDiagnosticoMatricula()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "update pdeinterativo.distorcaoaproveitamento set diastatus = 'I' where pdeid = $pdeid and diasubmodulo = 'M';";
	$db->executar($sql);
	$db->commit();
	
	
	if($chk_turma){
		foreach($chk_turma as $turma){
			$sql = "insert into
				pdeinterativo.distorcaoaproveitamento
			(pdeid,fk_cod_turma,diasubmodulo,diastatus)
				values
			($pdeid,$turma,'M','A');";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	salvarAbaResposta("diagnostico_2_1_matriculas");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_2_distorcaoidadeserie");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_1_matriculas");
	}
	
	exit;
	
}

function carregaDistorcaoDiagnosticoMatricula($pdeid = null,$diasubmodulo = "M",$marcado = "M")
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select
				fk_cod_turma 
			from
				pdeinterativo.distorcaoaproveitamento
			where
				pdeid = $pdeid
			and
				diasubmodulo = '$diasubmodulo'
			".($marcado ? " and diamarcado = '$marcado' " : "")."
			and
				diastatus = 'A'";
	
	
	/*$sql = "select
	fk_cod_turma
	from
	pdeinterativo.distorcaoaproveitamento
	where
	pdeid = $pdeid and diastatus = 'A'";
	*/
	
	//dbg($sql);
	return $db->carregarColuna($sql);
}

function recuperaCodigoINEPPorCPFDiretor($usucpf = null)
{
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$sql = "select 
				pdi.pdicodinep
			from 
				pdeinterativo.pdinterativo pdi 
			inner join
				pdeinterativo.pessoatipoperfil ptp ON ptp.pdeid = pdi.pdeid and ptp.tpeid='".TPE_DIRETOR."'
			inner join
				pdeinterativo.pessoa pes ON pes.pesid = ptp.pesid 
			inner join
				territorios.municipio mun ON pdi.muncod = mun.muncod 
			where
				usucpf = '$usucpf'
			and
				pesstatus = 'A'
			and
				pflcod = ".PDEESC_PERFIL_DIRETOR."
			and
				pdistatus = 'A'
			order by
				pdeid";
	
	return $db->pegaUm($sql);
}

function quadroPerguntas($sql)
{
	global $db;
	
	if(!strstr($sql,"pdeinterativo.pergunta")){
		echo "SQL de perguntas inválido!";
		return false;
	}
	
	$arrPerguntas = $db->carregar($sql);
	
	if($arrPerguntas){
		
		foreach($arrPerguntas as $p){
			$arrPrgid[] = $p['prgid'];
		}
		
		$sql = "select 
					opc.*,
					pro.prgid
				from 
					pdeinterativo.opcaopergunta opc
				inner join
					pdeinterativo.perguntaopcao pro ON opc.oppid = pro.oppid 
				where 
					prgid in (".implode(",",$arrPrgid).") 
				and 
					oppstatus = 'A' 
				order 
					by prgid,
					oppid";
		
		$arrOpcoes = $db->carregar($sql);
		if($arrOpcoes){
			foreach($arrOpcoes as $opc){
				$arrOpc[$opc['prgid']]["oppid"][] = $opc['oppid'];
				$arrOpc[$opc['prgid']]["oppdesc"][] = $opc['oppdesc'];
			}
			
			foreach($arrOpc as $p){
				$arrTam[] = count($p['oppdesc']);
			}
			$tamanho = max($arrTam);
			foreach($arrOpc as $key => $p){
				if($tamanho == count($p['oppdesc'])){
					$arrOpp = $p['oppdesc'];
					continue;
				}
			}
			
		}
		
		$sql = "select * from pdeinterativo.respostapergunta where prgid in (".implode(",",$arrPrgid).") AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
		$arrRespostas = $db->carregar($sql);
		if($arrRespostas){
			foreach($arrRespostas as $resp){
				$arrResp[$resp['prgid']][$resp['oppid']] = true;
			}
		}
		
		$arrOpp = !$arrOpp ? array() : $arrOpp;
		?>
		<table class="tabela" bgcolor="#DCDCDC"  cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#c5c5c5" >
				<td class="bold center" >PERGUNTA(S)</td>
				<?php foreach($arrOpp as $opp): ?>
					<td class="bold center" style="width:1%" nowrap ><?php echo $opp ?></td>
				<?php endforeach; ?>
			</tr>
			<?php $n=0; ?>
			<?php foreach($arrPerguntas as $perg):?>
				<?php $cor = $n%2 == 0 ? "#FFFFFF" : "" ?>
				<tr bgcolor="<?php echo $cor ?>" >
					<td id="prgid_<?php echo $perg['prgid'] ?>" >
						<?php echo $perg['prgdesc'] ?>
						<input type="hidden" name="hdn_prgid_<?php echo $perg['prgid'] ?>" value=""  />
					</td>
					<?php for($i=0;$i<$tamanho;$i++): ?>
						<td class="center" >
							<?php if($arrOpc[$perg['prgid']]['oppid'][$i]): ?>
								<input type="radio" onclick="respondePergunta('<?php echo $perg['prgid'] ?>','<?php echo strtolower($arrOpc[$perg['prgid']]['oppdesc'][$i]) ?>')" name="perg[<?php echo $perg['prgid'] ?>]" value="<?php echo $arrOpc[$perg['prgid']]['oppid'][$i] ?>" <?php echo $arrResp[$perg['prgid']][$arrOpc[$perg['prgid']]['oppid'][$i]] ? "checked='checked'" : "" ?>  id="perg_<?php echo $perg['prgid'] ?>_<?php echo $arrOpc[$perg['prgid']]['oppid'][$i] ?>" />
								<?php if ($arrResp[$perg['prgid']][$arrOpc[$perg['prgid']]['oppid'][$i]]): ?>
									<?php $arrExecJS[] = "respondePergunta('{$perg['prgid']}','".strtolower($arrOpc[$perg['prgid']]['oppdesc'][$i])."')" ?>
								<?php endif; ?>
							<?php else: ?>
								- 
							<?php endif; ?>
						</td>
					<?php endfor; ?>
				</tr>
				<?php $n++; ?>
			<?php endforeach ?>
		</table>
		<script>
			jQuery(function() {
				<?php if($arrExecJS): ?>
					<?php foreach($arrExecJS as $js): ?>
						<?php echo $js ?>;
					<?php endforeach; ?>
				<?php endif; ?>
			});
		</script>
	<?php }
	
}

function salvarDistorcaoIdadeSerie()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "update pdeinterativo.distorcaoaproveitamento set diastatus = 'I' where pdeid = $pdeid and diasubmodulo = 'D';";
	$db->executar($sql);
	$db->commit();
	
	if($chk_turma){
		foreach($chk_turma as $turma){
			$sql= "insert into
				pdeinterativo.distorcaoaproveitamento
			(pdeid,fk_cod_turma,diasubmodulo,diastatus,diamarcado)
				values
			($pdeid,$turma,'D','A','D');";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($num_distorcao){
		foreach($num_distorcao as $turma => $num){
			if($num != ""){
				$num = (integer) $num;
				$sql= "insert into
					pdeinterativo.distorcaoaproveitamento
				(pdeid,fk_cod_turma,diasubmodulo,diastatus,dianumdistorcao)
					values
				($pdeid,$turma,'D','A','$num');";
				$db->executar($sql);
				$db->commit();
				
			}
		}
	}
	
	salvarRespostasPorEscola();
	
	salvarAbaResposta("diagnostico_2_2_distorcaoidadeserie");
	
	$db->commit($sql);
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_3_aproveitamentoescolar");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_2_distorcaoidadeserie");
	}
	exit;
	
}

function salvarDistorcaoAproveitamentoEscolar()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "update pdeinterativo.distorcaoaproveitamento set diastatus = 'I' where pdeid = $pdeid and diasubmodulo = 'A';";
	$db->executar($sql);
	$db->commit();
	
	
	if($chk_turma_abandono){
		foreach($chk_turma_abandono as $turma){
			$sql= "insert into
				pdeinterativo.distorcaoaproveitamento
			(pdeid,fk_cod_turma,diasubmodulo,diastatus,diamarcado)
				values
			($pdeid,$turma,'A','A','A');";
			$db->executar($sql);
			$db->commit();
		}
	}
	if($chk_turma_reprovacao){
		foreach($chk_turma_reprovacao as $turma){
			$sql= "insert into
				pdeinterativo.distorcaoaproveitamento
			(pdeid,fk_cod_turma,diasubmodulo,diastatus,diamarcado)
				values
			($pdeid,$turma,'A','A','R');";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($num_reprovacao){
		foreach($num_reprovacao as $turma => $num){
			if($num != ""){
				$sql= "insert into
					pdeinterativo.distorcaoaproveitamento
				(pdeid,fk_cod_turma,diasubmodulo,diastatus,dianumreprovado)
					values
				($pdeid,$turma,'A','A',".str_replace(".","",$num).");";
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	if($num_abandono){
		foreach($num_abandono as $turma => $num){
			if($num != ""){
				$sql= "insert into
					pdeinterativo.distorcaoaproveitamento
				(pdeid,fk_cod_turma,diasubmodulo,diastatus,dianumabandono)
					values
				($pdeid,$turma,'A','A',".str_replace(".","",$num).");";
				$db->executar($sql);
				$db->commit();
				
			}
		}
	}
	
	salvarRespostasPorEscola();
	
	salvarAbaResposta("diagnostico_2_3_aproveitamentoescolar");

	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_4_areasdeconhecimento");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_3_aproveitamentoescolar");
	}

	exit;
	
}

function salvarRespostasPorEscola($pdeid = null)
{
	global $db;
	
	extract($_POST);
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	if(!$pdeid){
		return false;
	}
	
	if($perg){
		foreach($perg as $prgid => $oppid){
			$sqlE= "delete from pdeinterativo.respostapergunta where pdeid = $pdeid and prgid = $prgid;";
			$db->executar($sqlE);
			$db->commit();
			if($oppid) {
				$sqlI= "insert into pdeinterativo.respostapergunta (pdeid,oppid,prgid) values ($pdeid,$oppid,$prgid);";
				$db->executar($sqlI);
				$db->commit();
			}
		}

		return true;

	}
}

function carregaDistorcaoDiagnosticoTaxa($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select
				fk_cod_turma,
				dianumdistorcao,
				CASE 
					WHEN dianumdistorcao > 0
						THEN round(( (dianumdistorcao::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = p.fk_cod_turma  and t.id_status = 1)::numeric)*100))
					when dianumdistorcao = 0
						THEN 0
					ELSE null
				END as taxa
			from
				pdeinterativo.distorcaoaproveitamento p
			where
				pdeid = $pdeid
			and
				diasubmodulo = 'D'
			and
				diastatus = 'A'";

	$arrTaxa = $db->carregar($sql);
	
	if($arrTaxa){
		foreach($arrTaxa as $taxa){
			$arrT[$taxa['fk_cod_turma']]['distorcao'] = $taxa['dianumdistorcao'];
			$arrT[$taxa['fk_cod_turma']]['taxa'] = $taxa['taxa'];
		}
		return $arrT;
	}else{
		return array();
	}
}


function carregaDistorcaoDiagnosticoTaxaEscolar($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select
				fk_cod_turma,
				dianumabandono,
				dianumreprovado,
				CASE 
					WHEN dianumabandono > 0
						THEN round(( (dianumabandono::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = p.fk_cod_turma and t.id_status = 1)::numeric)*100))
					when dianumabandono = 0
						THEN 0
					ELSE null
				END as taxaabandono,
				CASE 
					WHEN dianumreprovado > 0
						THEN round(( (dianumreprovado::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = p.fk_cod_turma and t.id_status = 1)::numeric)*100))
					when dianumreprovado = 0
						THEN 0
					ELSE null
				END as taxareprovacao
			from
				pdeinterativo.distorcaoaproveitamento p
			where
				pdeid = $pdeid
			and
				diasubmodulo = 'A'
			and
				diastatus = 'A'
			and
				diamarcado is null";
	
	//dbg($sql);
	
	$arrTaxa = $db->carregar($sql);
	
	if($arrTaxa){
		foreach($arrTaxa as $taxa){
			if($taxa['dianumabandono'] != ""){
				$arrT["abandono"][$taxa['fk_cod_turma']] = $taxa['dianumabandono'];
				$arrT["taxa"]['abandono'][$taxa['fk_cod_turma']] = $taxa['taxaabandono'];	
			}elseif($taxa['dianumreprovado'] != ""){
				$arrT["reprovacao"][$taxa['fk_cod_turma']] = $taxa['dianumreprovado'];
				$arrT["taxa"]['reprovacao'][$taxa['fk_cod_turma']] = $taxa['taxareprovacao'];	
			}
			
		}
		return $arrT;
	}else{
		return array();
	}
}

function recuperaTurmasCriticasPorEscola($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select distinct
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where pk_cod_turma = fk_cod_turma and t.id_status = 1) as matricula
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			inner join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			where 
				dia.pdeid = $pdeid
			and
				dia.diamarcado = 'R'
			and
				dia.diastatus = 'A'
			order by
				no_etapa_ensino";
	
	//dbg($sql);
	
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			if($dado['matricula'] != 0){
				if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
					$inicio = strpos($dado['no_etapa_ensino'],"-");
					$fim = strlen($dado['no_etapa_ensino']);
					$arrEF["Ensino Fundamental"][] = array(
									"ensino" => "Ensino Fundamental",
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"turma" => $dado['no_turma'],
									"hrinicio" => $dado['hrinicio'],
									"hrfim" => $dado['hrfim'],
									"nummatricula" => $dado['matricula'],
									"localizacao" => $dado['localizacao'],
									"pk_cod_turma" => $dado['pk_cod_turma']
									);
				}
				elseif(strstr($dado['no_etapa_ensino'],"Educação Infantil")){
					$inicio = strpos($dado['no_etapa_ensino'],"-");
					$fim = strlen($dado['no_etapa_ensino']);
					$arrEF["Educação Infantil"][] = array(
									"ensino" => "Educação Infantil",
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"turma" => $dado['no_turma'],
									"hrinicio" => $dado['hrinicio'],
									"hrfim" => $dado['hrfim'],
									"nummatricula" => $dado['matricula'],
									"pk_cod_turma" => $dado['pk_cod_turma']
									);
				}
				elseif(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
					$inicio = strpos($dado['no_etapa_ensino'],"-");
					$fim = strlen($dado['no_etapa_ensino']);
					$arrEF["Ensino Médio"][] = array(
									"ensino" => "Ensino Médio",
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"turma" => $dado['no_turma'],
									"hrinicio" => $dado['hrinicio'],
									"hrfim" => $dado['hrfim'],
									"nummatricula" => $dado['matricula'],
									"pk_cod_turma" => $dado['pk_cod_turma']
									);
				}
			}
		}
	}
	
	return $arrEF;
	
}
function retornaDisciplinasTurma($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select distinct
				pk_cod_turma,
				pk_cod_disciplina,
				no_disciplina
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			inner join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			inner join
				educacenso_2010.tab_disciplina_turma disturma ON disturma.fk_cod_turma = turma.pk_cod_turma
			inner join
				educacenso_2010.tab_disciplina disc ON disc.pk_cod_disciplina = disturma.fk_cod_disciplina
			where 
				dia.pdeid = $pdeid
			and
				dia.diamarcado = 'R'
			and
				dia.diastatus = 'A'
			order by
				pk_cod_turma,
				no_disciplina";
	//dbg($sql);
	$arrDados = $db->carregar($sql);
	
	if($arrDados){
		foreach($arrDados as $dado){
			$arrDisciplina[$dado['pk_cod_turma']][] = array(
															"pk_cod_disciplina" => $dado['pk_cod_disciplina'],
															"no_disciplina" 	=> $dado['no_disciplina']
															);
		}
		return $arrDisciplina;
	}else{
		return array();
	}
}

function salvarDistorcaoAreasConhecimento()
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
		
	extract($_POST);
	
	$sql = "update pdeinterativo.distorcaodisciplina set dtdstatus = 'I' where pdeid = $pdeid;";
	$db->executar($sql);
	$db->commit();
	
	if($num_reprovacao_disciplina){
		foreach($num_reprovacao_disciplina as $turma => $arrDisc){
			if($arrDisc){
				foreach($arrDisc as $disc => $rep){
					if($rep || $rep == "0"){
						$sql= "insert into pdeinterativo.distorcaodisciplina (fk_cod_disciplina,fk_cod_turma,dtdnumreprovado,dtdstatus,pdeid) values ($disc,$turma,$rep,'A',$pdeid);";
						$db->executar($sql);
						$db->commit();
					}
				}
			}
		}
	}

	salvarRespostasPorEscola();
	salvarAbaResposta("diagnostico_2_4_areasdeconhecimento");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_5_sintesedimensao2");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_4_areasdeconhecimento");
	}
	exit;
	
}

function carregaDistorcaoTaxaReprovacaoDisciplina($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select 
					* 
			from 
				pdeinterativo.distorcaodisciplina 
			where 
				pdeid = $pdeid 
			and 
				dtdstatus = 'A'";
	
	//dbg($sql);
	
	$arrDados = $db->carregar($sql);
	
	if($arrDados){
		foreach($arrDados as $dado){
			$arrDisciplina[$dado['fk_cod_turma']][$dado['fk_cod_disciplina']] = $dado['dtdnumreprovado'];
		}
		return $arrDisciplina;
	}else{
		return array();
	}
	
}

function recuperaProgramas()
{
	return array();
}

function recuperaProjetos()
{
	return array();
}

function exibeTurmasCriticas()
{
	global $db;
	
	monta_titulo( "Distorção idade-série - Distorção e aproveitamento", "&nbsp;");
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select distinct
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				CASE 
					WHEN dianumdistorcao > 0
						THEN round(( (dianumdistorcao::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = turma.pk_cod_turma and t.id_status = 1)::numeric)*100))
					when dianumdistorcao = 0
						THEN 0
					ELSE null
				END || ' %' as taxa
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			left join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			where 
				dia.pdeid = $pdeid
			and
				dia.diasubmodulo = 'D'
			and
				diamarcado = 'D'
			and
				dia.diastatus = 'A'
			order by
				no_etapa_ensino";
	
	$arrDados = $db->carregar($sql);
		
	if($arrDados){
		foreach($arrDados as $dado){
			if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEF[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => $db->pegaUm("SELECT intvalor FROM pdeinterativo.indicadorestaxas it where intesfera = 'B' and intsubmodulo = 'D' and intano = 2010 and intensino = 'U'")."%"
								);
			}
			if(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEM[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => $db->pegaUm("SELECT intvalor FROM pdeinterativo.indicadorestaxas it where intesfera = 'B' and intsubmodulo = 'D' and intano = 2010 and intensino = 'M'")."%"
								);
			}
			if(strstr($dado['no_etapa_ensino'],"Educação Infantil")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEI[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => "0%"
								);
			}
		}
		$cabecalho = array("Série","Horário","Turma(s)","Taxa de distorção idade-série (em %)","Taxa de distorção idade-série do Brasil (em %)");
		echo "<br />";
		if($arrEF){
			echo "<center><b>Ensino Fundamental</b></center><br />";
			$db->monta_lista_simples($arrEF,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEM){
			echo "<center><b>Ensino Médio</b></center><br />";
			$db->monta_lista_simples($arrEM,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEI){
			echo "<center><b>Educação Infantil</b></center><br />";
			$db->monta_lista_simples($arrEI,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		
	}
	
}


function exibeTurmasCriticasReprovacao()
{
	global $db;
	
	monta_titulo( "Taxa de Reprovação - Distorção e aproveitamento", "&nbsp;");
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select distinct
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				CASE 
					WHEN dianumreprovado > 0
						THEN round(( (dianumreprovado::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = turma.pk_cod_turma)::numeric)*100))
					when dianumreprovado = 0
						THEN 0
					ELSE null
				END || ' %' as taxa
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			left join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			where 
				dia.pdeid = $pdeid
			and
				dia.diasubmodulo = 'A'
			and
				dia.diastatus = 'A'
			and
				dia.dianumreprovado > 0
			order by
				no_etapa_ensino";
	
	$arrDados = $db->carregar($sql);
	
	if($arrDados){
		foreach($arrDados as $dado){
			if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEF[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => $db->pegaUm("SELECT intvalor FROM pdeinterativo.indicadorestaxas it where intesfera = 'B' and intaprrepaba = 'R' and intano = 2010 and intensino = 'U'")."%"
								);
			}
			if(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEM[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => $db->pegaUm("SELECT intvalor FROM pdeinterativo.indicadorestaxas it where intesfera = 'B' and intaprrepaba = 'R' and intano = 2010 and intensino = 'M'")."%"
								);
			}
			if(strstr($dado['no_etapa_ensino'],"Educação Infantil")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEI[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => "0%"
								);
			}
		}
		$cabecalho = array("Série","Horário","Turma(s)","Taxa de reprovação (em %)","Taxa Brasil (em %)");
		echo "<br />";
		if($arrEF){
			echo "<center><b>Ensino Fundamental</b></center><br />";
			$db->monta_lista_simples($arrEF,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEM){
			echo "<center><b>Ensino Médio</b></center><br />";
			$db->monta_lista_simples($arrEM,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEI){
			echo "<center><b>Educação Infantil</b></center><br />";
			$db->monta_lista_simples($arrEI,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		
	}
	
}


function exibeTurmasCriticasAbandono()
{
	global $db;
	
	monta_titulo( "Taxa de Abandono - Distorção e aproveitamento", "&nbsp;");
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select distinct
				pk_cod_turma,
				no_etapa_ensino,
				no_turma,
				hr_inicial || ':' || hr_inicial_minuto as hrinicio,
				hr_final || ':' || hr_final_minuto as hrfim,
				CASE 
					WHEN dianumabandono > 0
						THEN round(( (dianumabandono::numeric/(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where t.fk_cod_turma = turma.pk_cod_turma and t.id_status = 1)::numeric)*100))
					when dianumabandono = 0
						THEN 0
					ELSE null
				END || ' %' as taxa
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			left join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			where 
				dia.pdeid = $pdeid
			and
				dia.diasubmodulo = 'A'
			and
				dia.diastatus = 'A'
			and
				dia.dianumabandono is not null
			order by
				no_etapa_ensino";
	
	$arrDados = $db->carregar($sql);
	
	if($arrDados){
		foreach($arrDados as $dado){
			if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEF[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => $db->pegaUm("SELECT intvalor FROM pdeinterativo.indicadorestaxas it where intesfera = 'B' and intaprrepaba = 'B' and intano = 2010 and intensino = 'U'")."%"
								);
			}
			if(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEM[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => $db->pegaUm("SELECT intvalor FROM pdeinterativo.indicadorestaxas it where intesfera = 'B' and intaprrepaba = 'B' and intano = 2010 and intensino = 'M'")."%"
								);
			}
			if(strstr($dado['no_etapa_ensino'],"Educação Infantil")){
				$inicio = strpos($dado['no_etapa_ensino'],"-");
				$fim = strlen($dado['no_etapa_ensino']);
				$arrEI[] = array( 
									"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
									"horario" => $dado['hrinicio']." - ".$dado['hrfim'],
									"turma" => $dado['no_turma'],
									"taxa" => $dado['taxa'],
									"taxaBrasil" => "0%"
								);
			}
		}
		$cabecalho = array("Série","Horário","Turma(s)","Taxa de abandono (em %)","Taxa de abandono do Brasil (em %)");
		echo "<br />";
		if($arrEF){
			echo "<center><b>Ensino Fundamental</b></center><br />";
			$db->monta_lista_simples($arrEF,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEM){
			echo "<center><b>Ensino Médio</b></center><br />";
			$db->monta_lista_simples($arrEM,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEI){
			echo "<center><b>Educação Infantil</b></center><br />";
			$db->monta_lista_simples($arrEI,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		
	}
	
}

function recuperaRespostasEscola($pdeid = null,$modulo = "D", $submodulo = "D", $detalhe = null, $arrWhere = array() )
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select
				repid,
				pdeid,
				rp.prgid,
				pe.prgdesc,
				op.oppid,
				op.oppdesc,
				prgmodulo,
				prgsubmodulo,
				prgdetalhe,
				critico
			from
				pdeinterativo.respostapergunta rp
			inner join
				pdeinterativo.pergunta pe ON pe.prgid = rp.prgid AND prgstatus = 'A'
			inner join
				pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid  AND oppstatus = 'A'
			where
				pdeid = $pdeid
			and
				prgmodulo = '$modulo'
			and
				prgsubmodulo = '$submodulo'
			".($prgdetalhe ? " and prgdetalhe = '$prgdetalhe' " : "")."
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			order by
				pe.prgdesc";
	//dbg($sql);
	return $db->carregar($sql);
	
}

function removeDiretoresAntigos()
{
	global $db;
	
	$pdicodinep = $_POST['pdicodinep'];
	$usucpf = $_POST['usucpf'];
	
	$pflcod = PDEESC_PERFIL_DIRETOR;
	
	$pdeid = $db->pegaUm("SELECT pdeid FROM pdeinterativo.pdinterativo WHERE pdicodinep='".$pdicodinep."' AND pdistatus='A'");
	$arrDados = recuperaDadosDiretor($pdeid);
	
	$sql = "delete from pdeinterativo.pessoatipoperfil where tpeid='".TPE_DIRETOR."' and pesid=(select pesid from pdeinterativo.pessoa where usucpf='".$arrDados['usucpf']."')";
	$db->executar($sql);
	$db->commit();
	

	$sql = "delete from seguranca.perfilusuario where pflcod=$pflcod and usucpf='".$arrDados['usucpf']."'";
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "delete from seguranca.usuario_sistema where sisid=".SISID_PDE_INTERATIVO." and usucpf='".$arrDados['usucpf']."'";
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "
			update pdeinterativo.usuarioresponsabilidade set rpustatus = 'I' where entid = '$pdicodinep' and pflcod = '$pflcod';
			update 
				pdeinterativo.pessoa
			set
				pflcod = null
			where
				usucpf = '$usucpf';
			delete 
			from 
				pdeinterativo.pessoatipoperfil 
			where 
				tpeid = ".TPE_DIRETOR."	and  
				pdeid = ( select pdeid from pdeinterativo.pdinterativo where pdistatus = 'A' and pdicodinep::integer = $pdicodinep limit 1);";
	
	$db->executar($sql);
	
	$db->commit();
	
}

function carregaTaxa($taxa = "R", $esfera = "B", $submodulo = "T")
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "SELECT it.* FROM pdeinterativo.indicadorestaxas it 
			 LEFT JOIN pdeinterativo.pdinterativo es ON es.estuf = it.intcoduf::character(2) and es.pdistatus = 'A' 
			 LEFT JOIN pdeinterativo.pdinterativo mu ON mu.muncod = it.intcodmun::character(7) and mu.pdistatus = 'A'
			 WHERE  it.intesfera='$esfera' AND it.intsubmodulo='$submodulo' AND it.intano IN('2010') AND intaprrepaba = '$taxa'
			 ORDER BY intensino";
	
	//dbg($sql);
	
	$arrDados = $db->carregar($sql);
	
	if($arrDados){
		foreach($arrDados as $dado){
			$arrTaxa[$dado['intensino']] = $dado['intvalor'];
		}
		return $arrTaxa;
	}else{
		return false;
	}
	
}

function salvarSinsteseDimensao2()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	//Opções das perguntas indicadas como Raramente ou Nunca
	if($arrRepid){
		$sql = "update pdeinterativo.respostapergunta set critico = false where repid in (".implode(",",$arrRepid).");";
		$db->executar($sql);
		$db->commit();
	}
	if($chk_problemas['opcao']){
		foreach($chk_problemas['opcao'] as $repid => $valor){
			$sql = "update pdeinterativo.respostapergunta set critico = true where repid = $repid;";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	//Taxas de distorção, reprovação e abandono abaixo da média do Brasil
	if($arrTurmasTaxa){
		$sql="update pdeinterativo.distorcaoaproveitamento set diacritico = false where fk_cod_turma in (".implode(",",$arrTurmasTaxa).") and pdeid = '$pdeid' and diastatus = 'A' and diamarcado in ('D','R','A');";
		$db->executar($sql);
		$db->commit();
		
	}
	
	
	$sql="update pdeinterativo.distorcaoaproveitamento set diacritico = false where diasubmodulo = 'M' and pdeid = '$pdeid' and diastatus = 'A';";
	$db->executar($sql);
	$db->commit();

	if($chk_problemas['matricula'] && $arrTurmasMatricula[0]){
		$sql="update pdeinterativo.distorcaoaproveitamento set diacritico = true where fk_cod_turma in (".$arrTurmasMatricula[0].") and pdeid = '$pdeid' and diastatus = 'A' and diasubmodulo = 'M';";
		$db->executar($sql);
		$db->commit();
		
	}
	
	if($chk_problemas['taxa']){
		foreach($chk_problemas['taxa'] as $taxa => $arrTurmas){
			switch($taxa){
				case "distorcao":
					$sql = "update 
								pdeinterativo.distorcaoaproveitamento 
							set 
								diacritico = true 
							where 
								fk_cod_turma in (".implode(",",$arrTurmas).") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'D' 
							and 
								pdeid = '$pdeid';";
					$db->executar($sql);
					$db->commit();
					
				break;
				
				case "reprovacao":
					$sql = "update 
								pdeinterativo.distorcaoaproveitamento 
							set 
								diacritico = true 
							where 
								fk_cod_turma in (".implode(",",$arrTurmas).") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'R' 
							and 
								pdeid = '$pdeid';";
					
					$db->executar($sql);
					$db->commit();
					
				break;
				
				case "abandono":
					$sql = "update 
								pdeinterativo.distorcaoaproveitamento 
							set 
								diacritico = true 
							where 
								fk_cod_turma in (".implode(",",$arrTurmas).") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'A' 
							and 
								pdeid = '$pdeid';";
					
					$db->executar($sql);
					$db->commit();
					
				break;
				
			}
			
		}
	}
	
	//Taxas de reprovação por disciplina
	if($arrTurmasDisciplina){
		$sql="update pdeinterativo.distorcaodisciplina set dtdcritico = false where fk_cod_turma in (".implode(",",$arrTurmasDisciplina).") and pdeid = '$pdeid' and dtdstatus = 'A';";
		$db->executar($sql);
		$db->commit();
	}
	if($chk_problemas['disciplina']){
		foreach($chk_problemas['disciplina'] as $disciplina => $arrTurmas){
			$sql = "update 
						pdeinterativo.distorcaodisciplina 
					set 
						dtdcritico = true 
					where 
						fk_cod_disciplina = $disciplina
					and
						fk_cod_turma in (".implode(",",$arrTurmas).") 
					and 
						dtdstatus = 'A'
					and 
						pdeid = '$pdeid' 
					and 
						dtdnumreprovado is not null;";
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	salvarAbaResposta("diagnostico_2_5_sintesedimensao2");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_0_orientacoes");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_5_sintesedimensao2");
	}
	
	exit;
	
}

function verificaCheckBoxTaxa($taxa,$arrTurmas)
{
	global $db;
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	
	
	if($arrTurmas && $taxa){
		switch($taxa){
			case "distorcao":
				$sql = "select distinct
							count(fk_cod_turma)
						from 
							pdeinterativo.distorcaoaproveitamento 
						where
							diacritico = true 
						and 
							fk_cod_turma in (".implode(",",$arrTurmas).") 
						and 
							diastatus = 'A'
						and
							diamarcado = 'D' 
						and 
							pdeid = '$pdeid';";
				
				break;
			
			case "reprovacao":
				$sql = "select distinct
							count(fk_cod_turma)
						from 
							pdeinterativo.distorcaoaproveitamento 
						where
							diacritico = true 
						and 
							fk_cod_turma in (".implode(",",$arrTurmas).") 
						and 
							diastatus = 'A'
						and
							diamarcado = 'R' 
						and 
							pdeid = '$pdeid';";
			break;
			
			case "abandono":
				$sql = "select distinct
							count(fk_cod_turma)
						from 
							pdeinterativo.distorcaoaproveitamento 
						where
							diacritico = true 
						and 
							fk_cod_turma in (".implode(",",$arrTurmas).") 
						and 
							diastatus = 'A'
						and
							diamarcado = 'A' 
						and 
							pdeid = '$pdeid';";
			break;
			
			default:
				return false;
			break;
	
		}
		$numTurmas = $db->pegaUm($sql);
		if($numTurmas == count($arrTurmas)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

function verificaTurmasCNE($arrTurmas)
{
	global $db;
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "select distinct
							count(fk_cod_turma)
						from 
							pdeinterativo.distorcaoaproveitamento 
						where
							diacritico = true 
						and 
							fk_cod_turma in (".implode(",",$arrTurmas).") 
						and 
							diastatus = 'A'
						and
							diacritico is true 
						and
							diasubmodulo = 'M'
						and 
							pdeid = '$pdeid';";
	$numTurmas = $db->pegaUm($sql);
	if($numTurmas == count($arrTurmas)){
		return true;
	}else{
		return false;
	}
	
}

function exibeTurmasCNE()
{
	global $db;
	
	monta_titulo( "Matrículas - Distorção e aproveitamento", "&nbsp;");
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select 
					pk_cod_turma,
					no_etapa_ensino,
					no_turma,
					hr_inicial || ':' || hr_inicial_minuto as hrinicio,
					hr_final || ':' || hr_final_minuto as hrfim,
					(select distinct count(pk_cod_matricula) from educacenso_2010.tab_matricula t where pk_cod_turma = fk_cod_turma and t.id_status = 1) as matricula,
					ten.id_localizacao as localizacao
				from 
					educacenso_2010.tab_turma turma
				inner join
					educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
				left join
					educacenso_2010.tab_entidade ten ON ten.pk_cod_entidade::bigint = turma.fk_cod_entidade::bigint
				where 
					pk_cod_turma in ( select
											fk_cod_turma 
										from
											pdeinterativo.distorcaoaproveitamento
										where
											pdeid = $pdeid
										and
											diasubmodulo = 'M'
										and
											diastatus = 'A' )
				order by
					no_etapa_ensino";
		$arrDados = $db->carregar($sql);
		if($arrDados){
			foreach($arrDados as $dado){
				if($dado['matricula'] != 0){
					if(strstr($dado['no_etapa_ensino'],"Ensino Fundamental")){
						$inicio = strpos($dado['no_etapa_ensino'],"-");
						$fim = strlen($dado['no_etapa_ensino']);
						$arrEF[] = array(
										//"ensino" => "Ensino Fundamental",
										"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
										"turma" => $dado['no_turma'],
										"hrinicio" => $dado['hrinicio']." - ".$dado['hrfim'],
										//"hrfim" => $dado['hrfim'],
										"nummatricula" => $dado['matricula'],
										//"localizacao" => $dado['localizacao'],
										//"pk_cod_turma" => $dado['pk_cod_turma']
										);
					}
					elseif(strstr($dado['no_etapa_ensino'],"Educação Infantil")){
						$inicio = strpos($dado['no_etapa_ensino'],"-");
						$fim = strlen($dado['no_etapa_ensino']);
						$arrEI[] = array(
										//"ensino" => "Educação Infantil",
										"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
										"turma" => $dado['no_turma'],
										"hrinicio" => $dado['hrinicio']." - ".$dado['hrfim'],
										//"hrfim" => $dado['hrfim'],
										"nummatricula" => $dado['matricula'],
										//"pk_cod_turma" => $dado['pk_cod_turma']
										);
					}
					elseif(strstr($dado['no_etapa_ensino'],"Ensino Médio")){
						$inicio = strpos($dado['no_etapa_ensino'],"-");
						$fim = strlen($dado['no_etapa_ensino']);
						$arrEM[] = array(
										//"ensino" => "Ensino Médio",
										"serie" => substr($dado['no_etapa_ensino'],$inicio + 2,$fim - $inicio),
										"turma" => $dado['no_turma'],
										"hrinicio" => $dado['hrinicio']." - ".$dado['hrfim'],
										//"hrfim" => $dado['hrfim'],
										"nummatricula" => $dado['matricula'],
										//"pk_cod_turma" => $dado['pk_cod_turma']
										);
					}
				}
			}
		}
		
		$cabecalho = array("Série","Turma","Horário","Matrículas");
		echo "<br />";
		if($arrEF){
			echo "<center><b>Ensino Fundamental</b></center><br />";
			$db->monta_lista_simples($arrEF,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEM){
			echo "<center><b>Ensino Médio</b></center><br />";
			$db->monta_lista_simples($arrEM,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		if($arrEI){
			echo "<center><b>Educação Infantil</b></center><br />";
			$db->monta_lista_simples($arrEI,$cabecalho,150,10,"N","","N");
			echo "<br /><br />";
		}
		
	
}



function verificaCheckBoxPergunta($repid)
{
	global $db;
	
	$sql = "select repid from pdeinterativo.respostapergunta where critico = true and repid = $repid;";
	if($db->pegaUm($sql)){
		return true;	
	}else{
		return false;
	}
}

function verificaCheckBoxDisciplina($disciplina,$arrTurmas)
{
	global $db;
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "	select
					count(fk_cod_turma)
				from 
						pdeinterativo.distorcaodisciplina 
					where 
						dtdcritico = true 
					and 
						fk_cod_disciplina = $disciplina
					and
						fk_cod_turma in (".implode(",",$arrTurmas).")
					and 
						dtdstatus = 'A'
					and 
						pdeid = '$pdeid' 
					and 
						dtdnumreprovado is not null;";

	$numTurmas = $db->pegaUm($sql);
	
	if($numTurmas == count($arrTurmas)){
		return true;
	}else{
		return false;
	}
	
}

function visualizarPDE()
{
	$_SESSION['pdeinterativo_vars']['usucpfdiretor'] = $_POST['usucpf'];
	header("Location: pdeinterativo.php?modulo=principal/identificacao&acao=A");
	exit;
}

function cabecalhoPDEInterativo($arrDados = array())
{ global $db; ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita bold" >Código INEP:</td>
			<td><?php echo $arrDados['pdicodinep'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita bold" >Escola:</td>
			<td><?php echo $arrDados['pdenome']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita bold" >Dados da escola:</td>
			<td><?php echo "<b>Município :</b> ".$db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$arrDados['muncod']."'").", <b>Unidade Federativa :</b> ".$arrDados['estuf'].", <b>Rede :</b> ".$arrDados['pdiesfera']; ?></td>
		</tr>
		<? if($_REQUEST['modulo'] == 'principal/planoestrategico') : ?>
		<tr>
			<td class="SubTituloDireita bold" >Recursos PDE Escola / Recursos previstos:</td>
			<td><?php 
			$sql = "SELECT * FROM pdeinterativo.cargacapitalcusteio WHERE codinep='".$_SESSION['pdeinterativo_vars']['pdicodinep']."' AND cccstatus='A'";
			$cargacapitalcusteio = $db->pegaLinha($sql);
			
			if($cargacapitalcusteio && !$_REQUEST['cccid']):
			
				$sql = "SELECT SUM(pabvalorcapital) as pabvalorcapital, SUM(pabvalorcusteiro) as pabvalorcusteiro, pabparcela 
						FROM pdeinterativo.planoacaobemservico pab 
						INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paaid = pab.paaid
						INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.paeid = paa.paeid 
						INNER JOIN pdeinterativo.planoacaoproblema pap ON pap.papid = pae.papid 
						WHERE pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pabstatus='A' AND papstatus='A' AND paestatus='A' AND paastatus='A' and ((pabfonte='O') or (pabfonte='P' and pabparcela in('S','P'))) 
						GROUP BY pabparcela";
				
				$valoregastos = $db->carregar($sql);
				
				if($valoregastos[0]) {
					foreach($valoregastos as $vlg) {
						$valorcusteio[$vlg['pabparcela']]=$vlg['pabvalorcusteiro'];
						$valorcapital[$vlg['pabparcela']]=$vlg['pabvalorcapital'];
					}
				}
			
			?>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
				<tr>
					<td class="SubTituloCentro">Parcela <?=(($cargacapitalcusteio['pdeid'])?"<img style=cursor:pointer; src=../imagens/alterar.gif onclick=\"window.location='".$_SERVER['REQUEST_URI']."&cccid=".$cargacapitalcusteio['cccid']."';\">":"") ?></td>
					<td class="SubTituloCentro">Recursos disponíveis PDE</td>
					<td class="SubTituloCentro">Gastos do PDE</td>
					<td class="SubTituloCentro">Restante PDE</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">1ª Parcela:</td>
					<td>R$ <?=number_format(($cargacapitalcusteio['ccccapitalprimeira']+$cargacapitalcusteio['ccccusteioprimeira']),2,",",".")."<br>( Capital: R$ ".number_format($cargacapitalcusteio['ccccapitalprimeira'],2,",",".").", Custeio: R$ ".number_format($cargacapitalcusteio['ccccusteioprimeira'],2,",",".").")" ?></td>
					<td>R$ <?=number_format(($valorcapital['P']+$valorcusteio['P']),2,",",".")."<br>( Capital: R$ ".number_format($valorcapital['P'],2,",",".").", Custeio: R$ ".number_format($valorcusteio['P'],2,",",".").")" ?></td>
					<td>R$ <?=number_format(($cargacapitalcusteio['ccccapitalprimeira']+$cargacapitalcusteio['ccccusteioprimeira']-$valorcapital['P']-$valorcusteio['P']),2,",",".")."<br>( Capital: R$ ".number_format($cargacapitalcusteio['ccccapitalprimeira']-$valorcapital['P'],2,",",".").", Custeio: R$ ".number_format($cargacapitalcusteio['ccccusteioprimeira']-$valorcusteio['P'],2,",",".").")" ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">2ª Parcela:</td>
					<td>R$ <?=number_format(($cargacapitalcusteio['ccccapitalsegunda']+$cargacapitalcusteio['ccccusteiosegunda']),2,",",".")."<br>( Capital: R$ ".number_format($cargacapitalcusteio['ccccapitalsegunda'],2,",",".").", Custeio: R$ ".number_format($cargacapitalcusteio['ccccusteiosegunda'],2,",",".").")" ?></td>
					<td>R$ <?=number_format(($valorcapital['S']+$valorcusteio['S']),2,",",".")."<br>( Capital: R$ ".number_format($valorcapital['S'],2,",",".").", Custeio: R$ ".number_format($valorcusteio['S'],2,",",".").")" ?></td>
					<td>R$ <?=number_format(($cargacapitalcusteio['ccccapitalsegunda']+$cargacapitalcusteio['ccccusteiosegunda']-$valorcapital['S']-$valorcusteio['S']),2,",",".")."<br>( Capital: R$ ".number_format($cargacapitalcusteio['ccccapitalsegunda']-$valorcapital['S'],2,",",".").", Custeio: R$ ".number_format($cargacapitalcusteio['ccccusteiosegunda']-$valorcusteio['S'],2,",",".").")" ?></td>
				</tr>
			</table>
			<? else : ?>
			
			<script>
			function somarParcelas(pref) {
				var primeira=0;
				var segunda=0;
				var total=0;
				if(document.getElementById(pref+'primeira').value!='') {
					primeira  = parseFloat(replaceAll(replaceAll(document.getElementById(pref+'primeira').value,".",""),",","."));
				}
				if(document.getElementById(pref+'segunda').value!='') {
					segunda  = parseFloat(replaceAll(replaceAll(document.getElementById(pref+'segunda').value,".",""),",","."));
				}
				
				total = primeira+segunda;
				document.getElementById(pref+'vlrtotal').value = mascaraglobal('###.###.###,##',total.toFixed(2));
			}
			
			function enviarCapitalCusteio() {
				if(document.getElementById('ccccapitalprimeira').value=='') {
					alert('1ª Parcela - Capital em branco');
					return false;
				}
				if(document.getElementById('ccccusteioprimeira').value=='') {
					alert('1ª Parcela - Custeio em branco');
					return false;
				}
				if(document.getElementById('ccccapitalsegunda').value=='') {
					alert('2ª Parcela - Capital em branco');
					return false;
				}
				if(document.getElementById('ccccusteiosegunda').value=='') {
					alert('2ª Parcela - Custeio em branco');
					return false;
				}
				document.getElementById('formulario').submit();
			}
			</script>
			<?
			if($_REQUEST['cccid']) {
				$sql = "SELECT * FROM pdeinterativo.cargacapitalcusteio WHERE cccid='".$_REQUEST['cccid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
				$arrcargacapitalcusteio = $db->pegaLinha($sql);
			}
			?>
			
			<form method="post" id="formulario" name="formulario">
			<input type="hidden" name="requisicao" value="gerenciarCargaCapitalCusteio">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
				<tr>
					<td class="SubTituloCentro">Parcela</td>
					<td class="SubTituloCentro">Capital</td>
					<td class="SubTituloCentro">Custeio</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">1ª Parcela:</td>
					<td><? echo campo_texto('ccccapitalprimeira', "S", "S", "1ª Parcela:", 16, 14, "###.###.###,##", "", '', '', 0, 'id="ccccapitalprimeira"', "somarParcelas('ccccapital');", number_format($arrcargacapitalcusteio['ccccapitalprimeira'],2,",",".") ); ?></td>
					<td><? echo campo_texto('ccccusteioprimeira', "S", "S", "1ª Parcela:", 16, 14, "###.###.###,##", "", '', '', 0, 'id="ccccusteioprimeira"', "somarParcelas('ccccusteio');", number_format($arrcargacapitalcusteio['ccccusteioprimeira'],2,",",".") ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">2ª Parcela:</td>
					<td><? echo campo_texto('ccccapitalsegunda', "S", "S", "2ª Parcela:", 16, 14, "###.###.###,##", "", '', '', 0, 'id="ccccapitalsegunda"', "somarParcelas('ccccapital');", number_format($arrcargacapitalcusteio['ccccapitalsegunda'],2,",",".") ); ?></td>
					<td><? echo campo_texto('ccccusteiosegunda', "S", "S", "2ª Parcela:", 16, 14, "###.###.###,##", "", '', '', 0, 'id="ccccusteiosegunda"', "somarParcelas('ccccusteio');", number_format($arrcargacapitalcusteio['ccccusteiosegunda'],2,",",".") ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita"><b>TOTAL</b></td>
					<td><? echo campo_texto('ccccapitalvlrtotal', "N", "N", "Total Capital", 16, 14, "###.###.###,##", "", '', '', 0, 'id="ccccapitalvlrtotal"', '', number_format($arrcargacapitalcusteio['ccccapitalvlrtotal'],2,",",".") ); ?></td>
					<td><? echo campo_texto('ccccusteiovlrtotal', "N", "N", "Total Custeio", 16, 14, "###.###.###,##", "", '', '', 0, 'id="ccccusteiovlrtotal"', '', number_format($arrcargacapitalcusteio['ccccusteiovlrtotal'],2,",",".") ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloCentro" colspan="3"><input type="button" value="Salvar" name="salvarcc" onclick="enviarCapitalCusteio();"></td>
				</tr>
			</table>
			</form>
			<? endif; ?>
			</td>
		</tr>
		<? endif; ?>
	</table>
<?php }

function cabecalhoFormacao($arrDados = array())
{ global $db; ?>
	<script>
		function enviarAnalise( docid ){
			window.location = window.location.href+'&requisicao=tramitaFormacao&pdeid=<?=$_SESSION['pdeinterativo_vars']['pdeid'] ?>&docid='+docid+'&aedid=1146';
		}
		
		function wf_exibirHistorico( docid )
		{
			var url = 'http://simec.mec.gov.br/geral/workflow/historico.php' +
				'?modulo=principal/tramitacao' +
				'&acao=C' +
				'&docid=' + docid;
			window.open(
				url,
				'alterarEstado',
				'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
			);
		}
	</script>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita bold" >Código INEP:</td>
			<td><?php echo $arrDados['pdicodinep'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita bold" >Escola:</td>
			<td><?php echo $arrDados['pdenome']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita bold" >Dados da escola:</td>
			<td><?php echo "<b>Município :</b> ".$db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$arrDados['muncod']."'").", <b>Unidade Federativa :</b> ".$arrDados['estuf'].", <b>Rede :</b> ".$arrDados['pdiesfera']; ?></td>
		</tr>
		<!--<tr>
			<td class="SubTituloDireita bold" >Tramitação:</td>
			<td rowspan="6">
				<?php 
				
				$docid = pegaDocid( $_SESSION['pdeinterativo_vars']['pdeid'] );
				
				wf_desenhaBotoesNavegacao( $docid , array( 'pdeid' => $_SESSION['pdeinterativo_vars']['pdeid']));
				
				?>
			</td>
		</tr>
	--></table>
<?php }

function barraProgressoPDEInterativo($pdeid = null)
{ 

	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center" >
		<tr>
			<td class="SubTituloDireita bold direita" >
				<table cellSpacing="1" cellPadding="0" align="right" border="0" width="240">
					<tr>
						<td class=" center bold" >Progresso de Preenchimento do PDE:</td>
					</tr>
					<tr>
						<td><?php progressBar($pdeid); ?></td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
<?php }

function progressBar($pdeid = null) {
	
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select count(distinct abaid) from pdeinterativo.aba where (abatipo != 'O' or abatipo is null) and abaidpai is not null and abaid not in (2,3,4,5,6,7,8,54);";
	$totalAbas = $db->pegaUm($sql);
	
	if(!$pdeid || $pdeid == ""){
		echo "<script>alert('Escola não encontrada!');window.location.href='pdeinterativo.php?modulo=principal/principal&acao=A'</script>";
		exit;
	}
	
	$sql = "select count(distinct abaid) from pdeinterativo.abaresposta where pdeid = $pdeid;";
	$totalAbasPreenchidas = $db->pegaUm($sql);
	
	if($totalAbasPreenchidas && $totalAbas){
		$percentage = (((int)$totalAbasPreenchidas / (int)$totalAbas)*100);
	}else{
		$percentage = 0;
	}
	$percentage = round($percentage,0);
	
	print "<style>.all-rounded {
			    -webkit-border-radius: 5px;
			    -moz-border-radius: 5px;
			    border-radius: 5px;
			}
			 
			.spacer {
				display: block;
			}
			
			.percent{
				position:absolute;
				color:#3063A5;
				margin-top:-14px;
				margin-left:".($percentage+10)."px;
			}
			 
			#progress-bar {
				width: 200px;
				margin: 0 auto;
				background: #cccccc;
				border: 3px solid #f2f2f2;
			}
			 
			#progress-bar-percentage {
				background: #3063A5;
				padding: 1px 0px;
			 	color: #FFF;
			 	font-weight: bold;
			 	text-align: center;
			}</style>";
	$percentage = $percentage > 100 ? 100 : $percentage;
	print "<div id=\"progress-bar\" class=\"all-rounded\">\n";
	print "<div id=\"progress-bar-percentage\" class=\"all-rounded\" style=\"width: $percentage%\">";
		if ($percentage > 10) {
			print "&nbsp;$percentage%";
			print "</div></div>";
		} else {
			print "<div class=\"spacer\">&nbsp;</div><div class=\"percent\" >$percentage%</div>";
			print "</div></div>";
		}
	
}

function salvarAbaResposta($abacod,$pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	if(!$pdeid) die("<script>alert('Problemas na identificação de variáveis. Tente novamente!');window.location='pdeinterativo.php?modulo=inicio&acao=C';</script>");
	
	if(CACHE_FILE) {
		/* Início - Cache em arquivo*/
		$arrAbasCache[] = "diagnostico_1_1_ideb";
		$arrAbasCache[] = "diagnostico_1_2_taxasderendimento";
		$arrAbasCache[] = "diagnostico_1_3_provabrasil";
		$arrAbasCache[] = "diagnostico_2_1_matriculas";
		$arrAbasCache[] = "diagnostico_2_2_distorcaoidadeserie";
		$arrAbasCache[] = "diagnostico_2_3_aproveitamentoescolar";
		$arrAbasCache[] = "diagnostico_2_4_areasdeconhecimento";
		$arrAbasCache[] = "diagnostico_5_2_docentes";
		
		if(in_array($abacod,$arrAbasCache)){
			include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
			$cache = new cache(false);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_FNDE);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_ESTADUAL);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_MUNICIPAL);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_CONSULTA);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_DIRETOR);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_ESTADUAL);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_MUNICIPAL);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_MEC);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_SUPER_USUARIO);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_ESTADUAL);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL);
			$cache->apagarCache($abacod."_".$_SESSION['pdeinterativo_vars']['pdeid']."_semperfil");
		}
		/* Fim - Cache em arquivo*/
	}
	
	
	if(strstr($abacod,"diagnostico_") && !strstr($abacod,"_orientacoes")){
		
		apagarCachePdeInterativo();
		
		$sql = "select flaid from pdeinterativo.flag where pdeid = $pdeid";
		$flaid = $db->pegaUm($sql);
		if($flaid){
			$sql = "update pdeinterativo.flag set atualizaplano = true where flaid = $flaid";
		}else{
			$sql = "insert into pdeinterativo.flag (pdeid,atualizaplano) values ($pdeid,true)";
		}
		$db->executar($sql);
		$db->commit();

	}
	
	if($abacod != "planoestrategico_0_2_planoacao") {
		$sql = "delete from pdeinterativo.abaresposta where pdeid = $pdeid and abaid = (select abaid from pdeinterativo.aba where abacod = 'planoestrategico_0_2_planoacao' limit 1)";
		$db->executar($sql);
		$db->commit();
	}
	
	$arrInfoPde = $db->pegaLinha("SELECT docid, pditempdeescola, pdicodinep, pdeano, pdeid, pdiesfera FROM pdeinterativo.pdinterativo WHERE pdeid=$pdeid");

	if(!$arrInfoPde['docid']) {
		if($arrInfoPde['pdiesfera']=="FEDERAL") {
			$esdid = $db->pegaUm("SELECT esdid FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO_FEDERAL."' AND esdordem='1'");
			$docid = $db->pegaUm("INSERT INTO workflow.documento(tpdid, esdid, docdsc, docdatainclusao) VALUES ('".TPD_WF_FLUXO_FEDERAL."', '".$esdid."', 'PDE Interativo (Escola Federal) ".$arrInfoPde['pdicodinep']."/".(($arrInfoPde['pdeano'])?$arrInfoPde['pdeano']:"XXXX")." ".$arrInfoPde['pdeid']."', NOW()) RETURNING docid;");
		} elseif($arrInfoPde['pditempdeescola']=="t") {
			$esdid = $db->pegaUm("SELECT esdid FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO."' AND esdordem='1'");
			$docid = $db->pegaUm("INSERT INTO workflow.documento(tpdid, esdid, docdsc, docdatainclusao) VALUES ('".TPD_WF_FLUXO."', '".$esdid."', 'PDE Interativo ".$arrInfoPde['pdicodinep']."/".(($arrInfoPde['pdeano'])?$arrInfoPde['pdeano']:"XXXX")." ".$arrInfoPde['pdeid']."', NOW()) RETURNING docid;");
		} elseif($arrInfoPde['pditempdeescola']=="f") {
			$esdid = $db->pegaUm("SELECT esdid FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO_SEMPDE."' AND esdordem='1'");
			$docid = $db->pegaUm("INSERT INTO workflow.documento(tpdid, esdid, docdsc, docdatainclusao) VALUES ('".TPD_WF_FLUXO_SEMPDE."', '".$esdid."', 'PDE Interativo (sem PDE) ".$arrInfoPde['pdicodinep']."/".(($arrInfoPde['pdeano'])?$arrInfoPde['pdeano']:"XXXX")." ".$arrInfoPde['pdeid']."', NOW()) RETURNING docid;");
		}
		$db->executar("UPDATE pdeinterativo.pdinterativo SET docid='".$docid."' WHERE pdeid='$pdeid'");
		$db->commit();
	}
	 	
	$sql = "delete from pdeinterativo.abaresposta where pdeid = $pdeid and abaid = (select abaid from pdeinterativo.aba where abacod = '$abacod' limit 1);";
	$db->executar($sql);
	$db->commit();
	$sql = "insert into pdeinterativo.abaresposta (abaid,pdeid) values ( (select abaid from pdeinterativo.aba where abacod = '$abacod' limit 1) , $pdeid)";
	$db->executar($sql);
	$db->commit();

	return true;

	
}

function apagarTodoCache($dados)
{
	include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
	$cache = new cache(false,null,$dados['diretorioEspecial']);
	$cache->apagarTodoCache(true);
}


function verificaFlagPDEInterativo($flag,$pdeid = null)
{
	global $db;
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	$sql = "select $flag from pdeinterativo.flag where pdeid = $pdeid";
	$valorFlag = $db->pegaUm($sql);
	if($valorFlag == "t"){
		return true;
	}else{
		return false;
	}
}

if($_GET['limparCache']){
	include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
	$cache = new cache(false);
	$cache->apagarCache($_GET['aba1']."_".$_SESSION['pdeinterativo_vars']['pdeid']);
	die("Cache excluído!");
}

function removerAbaResposta($abacod,$pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "delete from pdeinterativo.abaresposta where pdeid = $pdeid and abaid = (select abaid from pdeinterativo.aba where abacod = '$abacod' limit 1);";
	$db->executar($sql);
	if($db->commit($sql)){
		return true;
	}else{
		return false;
	}
	
}

function recuperaTelasPendentes($abacod,$pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select 
				abadescricao 
			from 
				pdeinterativo.aba 
			where 
				abaidpai = (select abaid from pdeinterativo.aba where abacod = '$abacod' limit 1)
			and
				abaid not in (select abaid from pdeinterativo.abaresposta where pdeid = $pdeid)
			and
				abatipo is null 
			order by 
				abadescricao";
	
	$arrDados = $db->carregarColuna($sql);
	
	if($arrDados){
		return $arrDados;
	}else{
		return false;
	}
}

function recuperaTelasSintesesPendentes($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "(select 
				abadescricao 
			from 
				pdeinterativo.aba 
			where 
				abaid not in (select abaid from pdeinterativo.abaresposta where pdeid = $pdeid)
			and
				abaidpai = 44
			and
				 abaid not in (45) 
			order by 
				abadescricao)
			union all
			(select 
				abadescricao 
			from 
				pdeinterativo.aba 
			where 
				abaid not in (select abaid from pdeinterativo.abaresposta where pdeid = $pdeid)
			and
				abatipo='S'
			and
				 abaid not in (9) 
			order by 
				abadescricao)";
	
	$arrDados = $db->carregarColuna($sql);
	
	if($arrDados){
		return $arrDados;
	}else{
		return false;
	}
}

function verificaQtdMaxima()
{
	global $db;
	
	$sql = "SELECT
				count(1) AS qtdfotos
			FROM
				pdeinterativo.galeriafoto
			WHERE
				aefid = ".$_POST['aefid']."
				AND pdeid = ".$_POST['pdeid']."
				AND gfostatus = 'A'";
	$qtdFotos = $db->pegaUm($sql);
	
	$maxFotos = $db->pegaUm("SELECT coalesce(aefqtdfoto, 0) FROM pdeinterativo.ambienteescolafoto WHERE aefid = ".$_POST['aefid']);
	
	if( $qtdFotos == $maxFotos )
		die("maximo");
	else
		die("ok");
}

function excluirFoto()
{
	global $db;

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("galeriafoto", null, "pdeinterativo");

	$db->executar("UPDATE pdeinterativo.galeriafoto SET gfostatus = 'I' WHERE arqid = ".$_POST['arqid']);
	$db->commit();
	
	$db->executar("UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$_POST['arqid']);
	$db->commit();
	
	$file->excluiArquivoFisico($_POST['arqid']);

	//salvarAbaResposta("primeiros_passos_passo_1");
	
	die('ok');
}

function verificaQtdMinima()
{
	global $db;
	
	$ret = array();
	
	$sql = "SELECT aefid,aefdesc,coalesce(aefqtdfotoobrigatoria,0) as aefqtdfotoobrigatoria FROM pdeinterativo.ambienteescolafoto WHERE aefstatus = 'A'";
	$ambienteescolafoto = $db->carregar($sql);
	
	if($ambienteescolafoto[0]) {
		foreach($ambienteescolafoto as $ambiente)
		{
			$sql = "SELECT count(1) FROM pdeinterativo.galeriafoto WHERE aefid = ".$ambiente['aefid']." AND pdeid = ".$_POST['pdeid']." AND gfostatus = 'A'";
			$qtdFotos = $db->pegaUm($sql);
			
			if( (integer)$qtdFotos < (integer)$ambiente['aefqtdfotoobrigatoria'] )
			{
				$ret[] = $ambiente['aefdesc'];
			}
		}
	}
	
	die( simec_json_encode($ret) );
}

function verificaPermissao($arrPflcod,$usucpf = null)
{
	global $db;
	
	if(is_array($arrPflcod))
	{
		$arrWhere[] = "pu.pflcod in ('".implode("','",$arrPflcod)."')";
	}else{
		$arrWhere[] = "pu.pflcod = '$arrPflcod'";
	}
	$arrWhere[] = "p.sisid = ".SISID_PDE_INTERATIVO;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$arrWhere[] = "pu.usucpf = '$usucpf'";
	
	$sql = "select 
				pu.pflcod
			from 
				seguranca.perfilusuario pu 
			inner join 
				seguranca.perfil p on p.pflcod = pu.pflcod
			".($arrWhere ? " where ".implode(" and ",$arrWhere) : " ")."
			and
				pflstatus = 'A'";
	
	// se não estiver em elaboração, não poderá se alterada
	$estado_documento = $db->pegaUm("SELECT d.esdid FROM pdeinterativo.pdinterativo p 
									 INNER JOIN workflow.documento d ON d.docid=p.docid 
									 WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	
	if($estado_documento && $estado_documento != WF_ESD_ELABORACAO && $estado_documento != WF_ESD_ELABORACAO_SEMPDE && $estado_documento != WF_ESD_ELABORACAO_FEDERAL) {
		$arrJS[] = "jQuery(\"[name^='btn_salvar'],[value='Inserir ação'],[value='Enviar para o comitê'],[value='Inserir estratégia'],[value='Inserir outros desafios'],[value='Inserir Membros do Conselho'],[id='btSalvar'],[value='Inserir Demais Profissionais'],[name^='fontesprogramas'],[src='../imagens/alterar.gif'],[src='../imagens/gif_inclui.gif'],[name^='salvar'],[value='Incluir Projeto'],[value='Incluir Programa'],[name^='continuar'], [name^='btn_continuar'],[class='btNovaFoto'],[id='btContinuar'],[src^='/imagens/exclui_p'],[src^='../imagens/exclui'],[src^='../imagens/alterar.gif'],[src^='/imagens/check_p'],[id='arquivo'],[id='btIncluirMembro']\").remove();";
		$arrJS[] = "jQuery(\"[name^='ridfinal'],[name^='pesid_estrategias['],[name^='aoaid['],[name^='metas2['], [name^='metas['],[name^='resposta'],[name='rpcperiodicidade'],[name='rpcunidadeexecutora'],[name^='pessoas['],[name^='pgtcoordenador'],[name^='porque'],[name^='rpb'], [name^='rid'], [name^='opcaocoordenador'],[name^='opcaogrupo'], [name^='atende'],[name^='necessaria'],[name^='justificativa'],[name^='rpcpossuiconcelho'],[name^='fus'],[name^='rdo'],[name^='rdovinculo'],[name^='critico'],[name^='qtd'],[name^='atv'],[name^='rta'],[name^='rtacaso'],[name^='num_'],[name^='perg'],[type='checkbox'][name^='chk_'],[type='checkbox'][name^='resposta'],[name^='exibeprograma'],[name^='rtrfun'],[name^='rpbfinal'],[name^='exibeprojeto']\").attr('disabled','disabled');";
	}
	
	if(!$db->carregar($sql)) {
		$arrJS[] = "jQuery(\"[name^='btn_salvar'],[value='Inserir ação'],[value='Enviar para o comitê'],[value='Inserir estratégia'],[value='Inserir outros desafios'],[value='Inserir Membros do Conselho'],[id='btSalvar'],[value='Inserir Demais Profissionais'],[name^='fontesprogramas'],[src='../imagens/alterar.gif'],[src='../imagens/gif_inclui.gif'],[name^='salvar'],[value='Incluir Projeto'],[value='Incluir Programa'],[name^='continuar'], [name^='btn_continuar'],[class='btNovaFoto'],[id='btContinuar'],[src^='/imagens/exclui_p'],[src^='../imagens/exclui'],[src^='../imagens/alterar.gif'],[src^='/imagens/check_p'],[id='arquivo'],[id='btIncluirMembro']\").remove();";
		$arrJS[] = "jQuery(\"[name^='ridfinal'],[name^='pesid_estrategias['],[name^='aoaid['],[name^='metas2['], [name^='metas['],[name^='resposta'],[name='rpcperiodicidade'],[name='rpcunidadeexecutora'],[name^='pessoas['],[name^='pgtcoordenador'],[name^='porque'],[name^='rpb'], [name^='rid'], [name^='opcaocoordenador'],[name^='opcaogrupo'], [name^='atende'],[name^='necessaria'],[name^='justificativa'],[name^='rpcpossuiconcelho'],[name^='fus'],[name^='rdo'],[name^='rdovinculo'],[name^='critico'],[name^='qtd'],[name^='atv'],[name^='rta'],[name^='rtacaso'],[name^='num_'],[name^='perg'],[type='checkbox'][name^='chk_'],[type='checkbox'][name^='resposta'],[name^='exibeprograma'],[name^='rtrfun'],[name^='rpbfinal'],[name^='exibeprojeto']\").attr('disabled','disabled');";
	}

	//if($arrJS)
	if($arrJS && !$db->testa_superuser())
	{
		echo "<script>";
		echo "jQuery(function() {";
		foreach($arrJS as $js)
		{
			echo $js;
		}
		echo "});";
		echo "</script>";
	}
	
}

function salvarInformePagamento()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	$mopid = !$mopid ? "null" : $mopid;
	$spadatapagamento = !$spadatapagamento ? "null" : "'".formata_data_sql($spadatapagamento)."'";
	$spasituacao = $spasituacao == "t" ? "true" : "false";
	
	$sql = "update pdeinterativo.situacaopagamento set spastatus = 'I' where pdeid = $pdeid;
			insert into 
				pdeinterativo.situacaopagamento 
			(pdeid,mopid,spasituacao,spadatapagamento,spastatus) 
				values 
				($pdeid,$mopid,$spasituacao,$spadatapagamento,'A');";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "select ( case when s.spasituacao then 'Pago' else  'Pendente - ' || mopdesc end)  as result from pdeinterativo.motivopagamento m
	right join pdeinterativo.situacaopagamento s
	on m.mopid = s.mopid 
	where spastatus = 'A' and pdeid = $pdeid";
	
	$situacao = $db->pegaUm($sql);
	$db->commit();
	
	$sql = "update pdeinterativo.listapdeinterativo set pagamento = '{$situacao}' where pdeid = $pdeid";
	$db->executar($sql);
	$db->commit();
	
	$_SESSION['pdeinterativo']['msg'] = "Operação realizada com sucesso!";
}

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function pegarEntCodEnt($entid = false){
	global $db;

	if( !$entid ){
		$entid = $_SESSION['entid'];
	}

	$sql =" SELECT
	trim(ee.entcodent)
	FROM
	entidade.entidadedetalhe AS ee
	INNER JOIN pdeescola.entpdeideb AS pde ON pde.epientcodent = ee.entcodent
	WHERE
	ee.entid = '$entid'";

	$entcodent = $db->pegaUm( $sql);
	return $entcodent;
}

function salvarJustificativaEvidencias($dados) {
	global $db;
	
	$sql = "UPDATE pdeinterativo.justificativaevidencias SET juedescricao=NULL WHERE abacod='".$dados['abacod']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	
	if($dados['juedescricao']) {	
		
		$jueid = $db->pegaUm("SELECT jueid FROM pdeinterativo.justificativaevidencias 
							  WHERE abacod='".$dados['abacod']."' AND 
							  		pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		
		if($jueid) {
			$sql = "UPDATE pdeinterativo.justificativaevidencias SET juedescricao='".substr(trim($dados['juedescricao']),0,1200)."' WHERE jueid='".$jueid."'";
		} else {
			$sql = "INSERT INTO pdeinterativo.justificativaevidencias(abacod, pdeid, juedescricao) VALUES ('".$dados['abacod']."', '".$_SESSION['pdeinterativo_vars']['pdeid']."', '".substr(trim($dados['juedescricao']),0,1200)."');";
		}
	}
	
	$db->executar($sql);
	$db->commit();
	
}

/*
 ********************************** FUNÇÕES WORKFLOW ***************************
 */
/*
 * Pegar docid em "pdeescola.pdeescola"
 */
function pegarDocid($entid){
	global $db;
	$entid = (integer) $entid;
	$sql = "
		SELECT docid
		FROM pdeinterativo.pdinterativo
		WHERE pdeano = ".ANO_EXERCICIO_PDE_INTERATIVO."
		AND   entid  = ".$entid;
	return $db->pegaUm($sql);
}