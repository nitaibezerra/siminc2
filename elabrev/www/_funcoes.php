<?php
function possui_perfil($perfil){

	global $db;

	if( !is_array($perfil) ) $perfil = Array($perfil);

	$sql = "SELECT
				count(1)
			FROM
				seguranca.perfilusuario
			WHERE
				usucpf = '{$_SESSION['usucpf']}'
				AND pflcod in ('".implode(',',$perfil)."')";

	return (boolean) $db->pegaUm( $sql );
}

function enviar_email_a($docid){

	global $db;

	$sql = "SELECT tcpid FROM monitora.termocooperacao WHERE docid = ".$docid;
	$tcpid = $db->pegaUm($sql);

    $strSQL = "
        select
            u.usuemail --, ur.usucpf, ur.pflcod
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where
          ur.pflcod = ".PERFIL_PROREITOR_ADM."
          and ur.rpustatus = 'A'
          and ur.prsano = '{$_SESSION['exercicio']}'
          and ur.ungcod = (SELECT ungcodproponente FROM monitora.termocooperacao WHERE tcpid = {$tcpid});
    ";
	
	$email2 = $db->carregarColuna($strSQL);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
	$atual = $db->pegaUm($sql);

	// Adicionar $_SESSION['baselogin'] no rodapé dos e-mails para ver de qual ambiente está buscando.
	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid foi cadastrado. Aguardando aprovação da reitoria.</p>";
	} else {
        $email = $atual;
        $cc = array($_SESSION['email_sistema']);
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid foi cadastrado. Aguardando aprovação da reitoria.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=> $_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid foi cadastrado. Aguardando aprovação da reitoria.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

//retornar para correção
function enviar_email_tec($docid) {
	global $db;

	$sql = "SELECT tcpid FROM monitora.termocooperacao WHERE docid = ".$docid;
	$tcpid = $db->pegaUm($sql);

	$strSQL = "
	    select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".UO_EQUIPE_TECNICA."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodproponente from monitora.termocooperacao where tcpid = {$tcpid});
	";
	$email2 = $db->carregarColuna($strSQL);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'";
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=> $_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid necessita de ajustes.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

//retornar para aprovação reitor
function enviar_email_rei($docid) {
	global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        SELECT email
        FROM elabrev.representantelegal
        WHERE ug = (SELECT ungcodproponente FROM monitora.termocooperacao WHERE tcpid = {$tcpid})
    ";
    $email2 = $db->carregarColuna($strSQL);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'";
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid necessita de ajustes.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

//Enviar email para gabinete da secretaria autarquia,
//quando termo em analise pela Coordenação
//estado: 'Aguardando aprovação pela Diretoria'
//destino: 'Enviar para aprovação do Secretário'
function enviar_email_sec($docid) {
	global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".PERFIL_GABINETE_SECRETARIA_AUTARQUIA."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";
    $email2 = $db->carregarColuna($strSQL);

	$sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid necessita de ajustes.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}
function enviar_email_dir($docid){

	global $db;

	$sql = "SELECT tcpid FROM monitora.termocooperacao WHERE docid = ".$docid;
	$tcpid = $db->pegaUm($sql);

	$sql = "select distinct usu.usuemail --, ung.ungcod, ung.ungdsc, dir.dircod, dir.dirdsc, usu.usucpf, usu.usunome, pfl.pfldsc
			from monitora.termocooperacao tcp
			join elabrev.diretoria dir on dir.ungcod = tcp.ungcodconcedente
			join unidadegestora ung on ung.ungcod = dir.ungcod
			join elabrev.usuarioresponsabilidade usr on usr.dircod = dir.dircod
			join seguranca.perfil pfl on pfl.pflcod = usr.pflcod
			join seguranca.usuario usu on usu.usucpf = usr.usucpf
			where usr.pflcod in (".PERFIL_DIRETORIA.") and tcp.tcpid = $tcpid and usr.rpustatus = 'A'";
	$email2 = $db->carregarColuna($sql);

	$sql = "SELECT usuemail	FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
	$atual = $db->pegaUm($sql);

	if(strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')){
			//ver("teste 1",d);
			$email = $email2;
			if(!$email) $email = $atual;
			//$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p> O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	}else{
			//ver("teste 2", d);
			//$email = $email2;
			$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p> O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	}


	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);

	$assunto  = "O termo de execução descentralizada nº $tcpid necessita de ajustes.";

	//$conteudo = "<p>$email2</p><p>$teste O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";

	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

/**
 * estado: "Termo em Análise pelo Gestor Orçamentário do Concedente"
 * destino: "Retornar para Análise da UG Repassadora"
 * @param $docid
 * @return bool
 */
function enviar_email_cgso($docid){
	global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = %d", (int) $docid);

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".PERFIL_CGSO."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = '152734';
    ";
	$email2 = $db->carregarColuna($strSQL);

	$sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>  O termo de execução descentralizada nº $tcpid necessita de ajustes.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid necessita de ajustes.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

//Envia email para perfil gabinete secretaria autarquia
function enviar_email_c($docid) {
	global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        select
            u.usuemail --,ur.usucpf, ur.pflcod
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".PERFIL_SECRETARIA."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid});
    ";
	$email2 = $db->carregarColuna($strSQL);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'";
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid foi aprovado pela reitoria. Aguarda posicionamento da secretaria.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p> O termo de execução descentralizada nº $tcpid foi aprovado pela reitoria. Aguarda posicionamento da secretaria.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado pela reitoria. Aguarda posicionamento da secretaria.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}



function _pegarDocid($tcpid) {
    global $db;

    if($tcpid){
        $sql = "Select	docid
				From monitora.termocooperacao
				Where tcpid = $tcpid
		";
        return $db->pegaUm($sql);
    }
    return false;
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $lbrid
 * @return integer
 */
function _pegaEstadoAtual($tcpid, $retornarDescricao = false) {
    global $db;

    $docid = _pegarDocid($tcpid);

    if ($docid) {
        $sql = <<<DML
SELECT ed.esdid,
       ed.esddsc
  FROM workflow.documento d
    INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
  WHERE d.docid = {$docid}
DML;
        $esddoc = $db->carregar($sql);
        if ($esddoc) {
            if (!$retornarDescricao) {
                return (integer)$esddoc[0]['esdid'];
            }
            return array((int)$esddoc[0]['esdid'], $esddoc[0]['esddsc']);
        }
    }
    return false;
}

//Envia email para o Coordenador da Secretaria Autarquia
function enviar_email_d($docid) {
	global $db;

    $linha = $db->pegaLinha("SELECT tcpid, cooid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        SELECT
            u.usuemail
        FROM elabrev.usuarioresponsabilidade ur
        JOIN seguranca.usuario u ON (u.usucpf = ur.usucpf)
        WHERE ur.pflcod = ".PERFIL_COORDENADOR_SEC."
            AND ur.rpustatus = 'A'
            AND ur.prsano = '{$_SESSION['exercicio']}'
            AND ur.ungcod = (SELECT ungcodconcedente FROM monitora.termocooperacao WHERE tcpid = {$linha['tcpid']})
            and ur.cooid = (SELECT cooid FROM monitora.termocooperacao WEHRE tcpid = {$linha['tcpid']})
    ";

	$sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid aguarda análise e parecer da coordenação.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid aguarda análise e parecer da coordenação.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid aguarda análise e parecer da coordenação.";

    /**
     * Envia email para gabinete, caso o termo ja tenha passado pelo estado Diligencia
     */
    $estadoAtual = _pegaEstadoAtual($tcpid);
    if ($estadoAtual == EM_ANALISE_OU_PENDENTE && jaFoiParaDiligencia($tcpid)) {
        enviar_email_gabinete($docid);
    }

	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
	return true;
}

//Envia email para diretoria
function enviar_email_e($docid) {
	global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        select
            u.usuemail --, ur.usucpf, ur.pflcod, ur.cooid
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".PERFIL_DIRETORIA."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";
    $email2 = $db->carregarColuna($strSQL);

	$sql = sprintf("SELECT usuemail	FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação pela diretoria.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação pela diretoria.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação pela diretoria.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

//Envia email quando o termo vai para diligencia
function enviar_email_f($docid) {
	global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        select
            u.usuemail --, ur.usucpf, ur.pflcod, ur.cooid
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".UO_EQUIPE_TECNICA."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";
    $email2 = $db->carregarColuna($strSQL);

    $sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi devolvido para ajustes (em diligência). Aguardando posicionamento da unidade técnica.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi devolvido para ajustes (em diligência). Aguardando posicionamento da unidade técnica.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid foi devolvido para ajustes (em diligência). Aguardando posicionamento da unidade técnica.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

function enviar_email_g($docid) {

	global $db;

	$sql = "SELECT tcpid, ungcodconcedente FROM monitora.termocooperacao WHERE docid = ".$docid;
	$rsTC = $db->pegaLinha($sql);

	if (!termoConcedenteFndeInep()) {
		$where[] = "usr.ungcod = '".UG_CGSO."'";
	} else {
		$where[] = "usr.ungcod = '{$rsTC['ungcodconcedente']}'";
	}

	$tcpid = $rsTC['tcpid'];

	$sql = "select distinct usu.usuemail
			from seguranca.perfilusuario upf
			join seguranca.usuario usu on usu.usucpf = upf.usucpf
			join seguranca.perfil pfl on pfl.pflcod = upf.pflcod
			join elabrev.usuarioresponsabilidade usr on usr.usucpf = usu.usucpf and usr.rpustatus='A'
			where usr.pflcod in (".PERFIL_CGSO.")
			".(is_array($where) ? " AND ".implode(" AND ", $where) : "")."
			and usr.prsano = '{$_SESSION['exercicio']}'";
    //ver($sql, d);
	$email2 = $db->carregarColuna($sql);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
	$atual = $db->pegaUm($sql);

	if(strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')){
			//ver("teste 1",d);
			$email = $email2;
			if(!$email) $email = $atual;
			//$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado pela secretaria. Aguardando posicionamento da UG repassadora.</p>";
	}else{
			//ver("teste 2", d);
			//$email = $email2;
// 			$email = array($_SESSION['email_sistema']);
			$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado pela secretaria. Aguardando posicionamento da UG repassadora.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);

	$assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado pela secretaria. Aguardando posicionamento da UG repassadora.";

	//$conteudo = "<p>$email2</p><p>O termo de execução descentralizada nº $tcpid foi aprovado pela secretaria. Aguardando posicionamento da SPO para descentralização.</p>";


	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

/**
 * estado: "Termo em análise pela UG repassadora"
 * destino: "Enviar para Aprovação do Gestor Orçamentário do Concedente"
 * @param $docid
 * @return bool
 */
function enviar_email_h($docid) {
	global $db;

    $sql = sprintf("SELECT tcpid, ungcodconcedente FROM monitora.termocooperacao WHERE docid = %d", (int) $docid);
    $rsTC = $db->pegaLinha($sql);

    $ungCod = (!termoConcedenteFndeInep()) ? UG_CGSO : $rsTC['ungcodconcedente'];

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod in (".PERFIL_SUBSECRETARIO.")
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = '{$ungCod}'
    ";
	$email2 = $db->carregarColuna($strSQL);

	$sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid aguarda autorização para descentralização.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid aguarda autorização para descentralização.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid aguarda autorização para descentralização.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

function enviar_email_i($docid){

	global $db;
	
	$sql = "SELECT tcpid FROM monitora.termocooperacao WHERE docid = ".$docid;
	$tcpid = $db->pegaUm($sql);

	$sql = "select distinct usu.usuemail --, usu.usucpf, usu.usunome, pfl.pfldsc
			from seguranca.perfilusuario upf
			join seguranca.usuario usu on usu.usucpf = upf.usucpf
			join seguranca.perfil pfl on pfl.pflcod = upf.pflcod
			join elabrev.usuarioresponsabilidade usr on usr.usucpf = usu.usucpf and usr.rpustatus='A'
			join monitora.termocooperacao tcp on tcp.ungcodconcedente = usr.ungcod and tcp.tcpid = {$tcpid}
			where upf.pflcod in (".PERFIL_CGSO.")";
	$email2 = $db->carregarColuna($sql);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
	$atual = $db->pegaUm($sql);

	if(strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')){
			//ver("teste 1",d);
			$email = $email2;
			if(!$email) $email = $atual;
			//$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid pode ser enviado para execução. Descentralizar os recursos.</p>";
	}else{
			//ver("teste 2", d);
			//$email = $email2;
			$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid pode ser enviado para execução. Descentralizar os recursos.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);

	$assunto  = "O termo de execução descentralizada nº $tcpid pode ser enviado para execução. Descentralizar os recursos.";

	//$conteudo = "<p>O termo de execução descentralizada nº $tcpid pode ser enviado para execução. Descentralizar os recursos.</p>";


	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}
function enviar_email_j($docid){

	global $db;
	
	$sql = "SELECT tcpid, ungcodproponente FROM monitora.termocooperacao WHERE docid = ".$docid;
	$dados = $db->pegaLinha($sql);
	$tcpid = $dados['tcpid'];
	$ungcod = $dados['ungcodproponente'];

	if($ungcod){
		$sql = "select distinct usu.usuemail --, usu.usucpf, usu.usunome, pfl.pfldsc
				from seguranca.perfilusuario upf
				join seguranca.usuario usu on usu.usucpf = upf.usucpf
				join seguranca.perfil pfl on pfl.pflcod = upf.pflcod
				join elabrev.usuarioresponsabilidade ur on ur.usucpf = usu.usucpf and ur.rpustatus = 'A'
				where ur.rpustatus='A'
				and ur.pflcod in (".PERFIL_REITOR.")
				and ur.ungcod = '$ungcod'";
		$email2 = $db->carregarColuna($sql);
	}

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
	$atual = $db->pegaUm($sql);

	if(strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')){
			//ver("teste 1",d);
			$email = $email2;
			if(!$email) $email = $atual;
			//$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi alterado. Aguardando aprovação da reitoria.</p>";
	}else{
			//ver("teste 2", d);
			//$email = $email2;
			$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi alterado. Aguardando aprovação da reitoria.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);

	$assunto  = "O termo de execução descentralizada nº $tcpid foi alterado. Aguardando aprovação da reitoria.";

//	$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi alterado. Aguardando aprovação da reitoria.</p>";


	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}
function enviar_email_k($docid){

	global $db;

	$sql = "SELECT tcpid FROM monitora.termocooperacao WHERE docid = ".$docid;
	$tcpid = $db->pegaUm($sql);

	if($tcpid){
		$sql = "select distinct usu.usuemail --, usu.usucpf, usu.usunome, pfl.pfldsc
				from seguranca.perfilusuario upf
				join seguranca.usuario usu on usu.usucpf = upf.usucpf
				join seguranca.perfil pfl on pfl.pflcod = upf.pflcod
				join elabrev.usuarioresponsabilidade ur on ur.usucpf = usu.usucpf and ur.rpustatus='A'
				join monitora.termocooperacao tc on tc.cooid = ur.cooid
				where upf.pflcod in (".PERFIL_COORDENADOR_SEC.") and tc.tcpid = ".$tcpid;
		$email2 = $db->carregarColuna($sql);
	}
	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
	$atual = $db->pegaUm($sql);

	if(strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')){
			//ver("teste 1",d);
			$email = $email2;
			if(!$email) $email = $atual;
			//$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do secretário.</p>";
	}else{
			//ver("teste 2", d);
			//$email = $email2;
			$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do secretário.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);

	$assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do secretário.";

	//$conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do secretário.</p>";


	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

/**
 * Estado: "Aguardando aprovação pela Diretoria"
 * Destino: "Enviar para aprovação do Representante Legal do Concedente"
 * @param $docid
 * @return bool
 */
function enviar_email_l($docid) {
	global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".PERFIL_SECRETARIO."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
        union all
        select email from elabrev.representantelegal where ug = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";

	$sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if(!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Concedente.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Concedente.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Concedente.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

function enviar_email_m($docid) {
	global $db;

	$tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        SELECT email
        FROM elabrev.representantelegal
        WHERE ug = (SELECT ungcodproponente FROM monitora.termocooperacao WHERE tcpid = {$tcpid})
    ";
    $email2 = $db->carregarColuna($strSQL);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'";
	$atual = $db->pegaUm($sql);

	if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email){
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado.Aguardando aprovação do Representante Legal do Proponente.</p>";
	} else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Proponente.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
	$assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Proponente.";
	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
	return true;
}

function enviar_email_n($docid) {
	global $db;

	$sql = "SELECT tcpid, ungcodproponente FROM monitora.termocooperacao WHERE docid = {$docid}";
	$dados = $db->pegaLinha($sql);
	$tcpid = $dados['tcpid'];
	$ungcod = $dados['ungcodproponente'];

	if($ungcod){
		$sql = "select distinct usu.usuemail --, usu.usucpf, usu.usunome, pfl.pfldsc
				from seguranca.perfilusuario upf
				join seguranca.usuario usu on usu.usucpf = upf.usucpf
				join seguranca.perfil pfl on pfl.pflcod = upf.pflcod
				join elabrev.usuarioresponsabilidade ur on ur.usucpf = usu.usucpf and ur.rpustatus = 'A'
				where ur.rpustatus='A'
				and ur.pflcod in (".PERFIL_REITOR.")
				and ur.ungcod = '$ungcod'";
		$email2 = $db->carregarColuna($sql);
	}

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
	$atual = $db->pegaUm($sql);

	if(strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')){
			//ver("teste 1",d);
			$email = $email2;
			if(!$email) $email = $atual;
			//$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O relatório de cumprimento do objeto referente ao termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Proponente.</p>";
	}else{
			//ver("teste 2", d);
			//$email = $email2;
			$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O relatório de cumprimento do objeto referente ao termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Proponente.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);

	$assunto  = "O relatório de cumprimento do objeto referente ao termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação do Representante Legal do Proponente.";

	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

function enviar_email_o($docid) {
	global $db;

	$sql = "SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}";
	$tcpid = $db->pegaUm($sql);

    $strSQL = "
        select u.usuemail
        from elabrev.usuarioresponsabilidade ur
        join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where rpustatus = 'A'
            and ur.pflcod = (".PERFIL_COORDENADOR_SEC.")
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";

	$email2 = $db->carregarColuna($sql);

	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'";
	$atual = $db->pegaUm($sql);

	if(strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')){
			$email = $email2;

			if (!$email) {
                $email = $atual;
            }

			$cc = $atual;
			$conteudo = "<p>O relatório de cumprimento do objeto referente ao termo de execução descentralizada nº $tcpid foi aprovado. Aguardando análise da coordenação da Secretaria, com parecer e observações (optativos), para finalização do termo.</p>";
	}else{
			$email = array($_SESSION['email_sistema']);
			$cc = $atual;
			$conteudo = "<p>O relatório de cumprimento do objeto referente ao termo de execução descentralizada nº $tcpid foi aprovado. Aguardando análise da coordenação da Secretaria, com parecer e observações (optativos), para finalização do termo.</p>";
	}

	$remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);

	$assunto  = "O relatório de cumprimento do objeto referente ao termo de execução descentralizada nº $tcpid foi aprovado. Aguardando análise da coordenação da Secretaria, com parecer e observações (optativos), para finalização do termo.";

	enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
	return true;
}

function enviar_email_gabinete($docid) {
    global $db;

    $sql = "SELECT tcpid FROM monitora.termocooperacao WHERE docid = ".$docid;
    $tcpid = $db->pegaUm($sql);

    $strSQL = "
        select distinct
            usu.usuemail
        from
            monitora.termocooperacao tcp
        join elabrev.usuarioresponsabilidade usr on (usr.ungcod = ungcodconcedente)
        join seguranca.perfil pfl on (pfl.pflcod = usr.pflcod)
        join seguranca.usuario usu on (usu.usucpf = usr.usucpf)
        join unidadegestora ung on (ung.ungcod = usr.ungcod)
        where
            usr.pflcod in(".PERFIL_GABINETE_SECRETARIA_AUTARQUIA.") and
            tcp.tcpid = {$tcpid} and
            usr.rpustatus = 'A'";
    $email2 = $db->carregarColuna($strSQL);

    $sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '".$_SESSION['usucpf']."'";
    $atual = $db->pegaUm($sql);

    if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;

        if (!$email)
            $email = $atual;

        $cc = $atual;
    } else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
    }

    $conteudo = "<p>O termo de execução descentralizada nº {$tcpid} estava em diligência e foi enviado para coordenação responsável.</p>";

    $remetente = array('nome'=>'Programação Orçamentária - Termo de Execução Descentralizada', 'email'=>$_SESSION['email_sistema']);

    $assunto  = "O termo de execução descentralizada nº {$tcpid} estava em diligência e foi enviado para coordenação responsável.";

    enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
    return true;
}

/**
 * Verifica se um determinado termo de cooperação já passou pelo estado de Diligencia
 * @return bool
 */
function jaFoiParaDiligencia($tcpid) {
    global $db;

    $sql = "
        select
        tcp.tcpid, tcp.docid, hd.hstid, hd.htddata
    from
        monitora.termocooperacao tcp
    join workflow.historicodocumento hd ON (hd.docid = tcp.docid)
    where
        aedid = 1607 and
        tcpid = {$tcpid}
    ";

    $isDiligencia = $db->carregar($sql);

    return ($isDiligencia) ? true : false;
}

/**
 *
 * @return bool
 */
function termoConcedenteFndeInep() {
	global $db;

	$sql = "select
				tcpid
			from monitora.termocooperacao
			where ungcodconcedente in ('".UG_INEP."', '".UG_FNDE."', '".UG_CAPES."')
			and tcpid = '{$_SESSION['elabrev']['tcpid']}'";
	$rs = $db->pegaUm($sql);

    return ($rs) ? true : false;
}

function combo_popup_uo( $nome, $sql, $titulo, $tamanho_janela = '400x400', $maximo_itens = 0,
$codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
$campo_flag_contem = false, $size = 10, $width = 400 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null)
{

    global ${$nome};
    unset($dados_sessao);
    // prepara parametros
    $maximo_itens = abs( (integer) $maximo_itens );
    $codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
    // prepara sessão
    $dados_sessao = array(
    'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
    'titulo' => $titulo,
    'indice' => $indice_visivel,
    'maximo' => $maximo_itens,
    'codigos_fixos' => $codigos_fixos,
    'mensagem_fixo' => $mensagem_fixo,
    'param_conexao' => $param_conexao,
    'where'         => $where,
    'mostraPesquisa'=> $mostraPesquisa,
    'intervalo'     => $intervalo,
    'arrVisivel'    => $arrVisivel,
    'arrOrdem'     => $arrOrdem
    );

    if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
    {
        $_SESSION['indice_sessao_combo_popup'] = array();
    }
    unset($_SESSION['indice_sessao_combo_popup'][$nome]);
    $_SESSION['indice_sessao_combo_popup'][$nome] = $dados_sessao;

    // monta html para formulario
    $tamanho    = explode( 'x', $tamanho_janela );
    $onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca_uo( this );" ';

    /*** Adiciona a função Javascript ***/
    $funcaoJS = (is_null($funcaoJS)) ? 'false' : "'" . $funcaoJS . "'";

    $ondblclick = ' ondblclick="javascript:combo_popup_abre_janela_uo( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );" ';
    $ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados_uo( event, \'' . $nome . '\' );" ';
    $onpop      = ( $onpop == null ) ? $onpop = '' : ' onpop="' . $onpop . '"';
    $onpush     = ( $onpush == null ) ? $onpush = '' : ' onpush="' . $onpush . '"';
    $habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;
    $select =
    '<select ' .
    'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
    'multiple="multiple" size="' . $size . '" ' .
    'name="' . $nome . '[]" id="' . $nome . '" '.
    $onclick . $ondblclick . $ondelete . $onpop . $onpush  .
    'class="CampoEstilo" style="width:' . $width . 'px;" ' .
    $habilitado_select .
    '>';

    if($value && count( $value ) > 0){
        $itens_criados = 0;
        foreach ( $value as $item )
        {
            $select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
            $itens_criados++;
            if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
            {
                break;
            }
        }
    } elseif ( ${$nome} && count( ${$nome} ) > 0 ) {
        $itens_criados = 0;
        if( is_array(${$nome}) ){
            foreach ( ${$nome} as $item )
            {
                $select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
                $itens_criados++;
                if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
                {
                    break;
                }
            }
        }
    }
    else if ( $habilitado == 'S' )
    {
        $select .= '<option value="">Duplo clique para selecionar da lista</option>';
    }
    else
    {
        $select .= '<option value="">Nenhum</option>';
    }
    $select .= '</select>';
    $buscaCodigo = '';

    #Alteração feita por wesley romualdo
    #caso a consulta não seja por descrição e sim por codigo, não permitir digitar string no campo de consulta.
    if($campo_busca_descricao == true ){
        $paramentro = "";
        $complOnblur = "";
    } else {
        $paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
        $complOnblur = "this.value=mascaraglobal('[#]',this.value);";
    }

    if ( $campo_busca_codigo == true && $habilitado == 'S' )
    {
        $buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $nome . '" onkeypress="combo_popup_keypress_buscar_codigo_uo( event, \'' . $nome . '\', this.value );" '.$paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
        $buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo_uo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
        $buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item_uo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
        $buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela_uo( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
        $buscaCodigo .= '<br/>';
    }
    #Fim da alteração realizada por wesley romualdo

    $flagContem = '';
    if ( $campo_flag_contem == true )
    {
        $nomeFlagContemGlobal = $nome . '_campo_excludente';
        global ${$nomeFlagContemGlobal};
        $flagContem .= '<input type="checkbox" id="' . $nome . '_campo_excludente" name="' . $nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
        $flagContem .= '&nbsp;<label for="' . $nome . '_campo_excludente">Não contém</label>';
    }
    $cabecalho = '';
    if ( $buscaCodigo != '' || $flagContem != '' )
    {
        $cabecalho .= '<table width="' . $width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
        $cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
        $cabecalho .= '<td align="right">' . $flagContem . '</td>';
        $cabecalho .= '</tr></table>';
    }
    print $cabecalho . $select  . ' <img src="../imagens/pop_p.gif" style="cursor:pointer;" align="absmiddle" onclick="javascript:combo_popup_abre_janela_uo( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );">';
}

// -- Funções utilizadas na geração dos relatórios
function retornaColunasELabels() {
    return array(
        'descricao' => 'Descrição',
        'unidadegestorap' => 'UG do Proponente',
        'unidadegestorac' => 'UG do Cedente',
        'resp_proponente' => 'Representante do Proponente',
        'resp_concedente' => 'Representante do Cedente',
        'esddsc' => 'Estado da Documentação',
        'coodsc' => 'Coordenação',
        'valor' => 'Valor da Proposta',
        'ptres_desc' => 'Programa de Trabalho',
        'plicod' => 'Plano Interno',
        'ntddsc' => 'Natureza da Despesa',
    );
}

function monta_sql()
{
    $where = array();

    // -- Filtros do relatório
    // -- Ano de referência
    if (!empty($_REQUEST['proanoreferencia'])) {
        $where[] = "pro.proanoreferencia = '{$_REQUEST['proanoreferencia']}'";
    }
    // -- Natureza da Despesa
    if (!empty($_REQUEST['naturezadespesa'][0])) {
        if (1 == count($_REQUEST['naturezadespesa'])) {
            $where[] = "ndp.ndpid = '{$_REQUEST['naturezadespesa'][0]}'";
        } else {
            array_walk($_REQUEST['naturezadespesa'], 'quote');
            $where[] = 'ndp.ndpid IN(' . implode(', ', $_REQUEST['naturezadespesa']) . ')';
        }
    }
    // -- Plano interno
    if (!empty($_REQUEST['planointerno'][0])) {
        if (1 == count($_REQUEST['planointerno'])) {
            $where[] = "pli.pliid = '{$_REQUEST['planointerno'][0]}'";
        } else {
            array_walk($_REQUEST['planointerno'], 'quote');
            $where[] = 'pli.pliid IN(' . implode(', ', $_REQUEST['planointerno']) . ')';
        }
    }
    // -- Plano interno
    if (!empty($_REQUEST['planotrabalho'][0])) {
        if (1 == count($_REQUEST['planotrabalho'])) {
            $where[] = "ptr.ptrid = '{$_REQUEST['planotrabalho'][0]}'";
        } else {
            array_walk($_REQUEST['planotrabalho'], 'quote');
            $where[] = 'ptr.ptrid IN(' . implode(', ', $_REQUEST['planotrabalho']) . ')';
        }
    }
    // -- UG Proponente
    if (!empty($_REQUEST['unidadegestorap'][0])) {
        if (1 == count($_REQUEST['unidadegestorap'])) {
            $where[] = "unp.ungcod = '{$_REQUEST['unidadegestorap'][0]}'";
        } else {
            array_walk($_REQUEST['unidadegestorap'], 'quote');
            $where[] = 'unp.ungcod IN(' . implode(', ', $_REQUEST['unidadegestorap']) . ')';
        }
    }
    // -- UG Cedente
    if (!empty($_REQUEST['unidadegestorac'][0])) {
        if (1 == count($_REQUEST['unidadegestorac'])) {
            $where[] = "unc.ungcod = '{$_REQUEST['unidadegestorac'][0]}'";
        } else {
            array_walk($_REQUEST['unidadegestorac'], 'quote');
            $where[] = 'unc.ungcod IN(' . implode(', ', $_REQUEST['unidadegestorac']) . ')';
        }
    }
    // -- Estado da Documentação
    if (!empty($_REQUEST['esdid'][0])) {
        if (1 == count($_REQUEST['esdid'])) {
            $where[] = "esd.esdid = '{$_REQUEST['esdid'][0]}'";
        } else {
            array_walk($_REQUEST['esdid'], 'quote');
            $where[] = 'esd.esdid IN(' . implode(', ', $_REQUEST['esdid']) . ')';
        }
    }
    // -- Coordenação
    if (!empty($_REQUEST['cooid'][0])) {
        if (1 == count($_REQUEST['cooid'])) {
            $where[] = "tcp.cooid = '{$_REQUEST['cooid'][0]}'";
        } else {
            array_walk($_REQUEST['cooid'], 'quote');
            $where[] = 'tcp.cooid IN(' . implode(', ', $_REQUEST['cooid']) . ')';
        }
    }

    // -- Processando os filtros escolhidos para incluir na query
    if (!empty($where)) {
        $where = 'AND ' . implode(' AND ', $where);
    } else {
        $where = '';
    }

    $sql = <<<DML
SELECT 'Termo Nº '||tcpid|| ' ' AS descricao,
       unp.ungcod || ' / ' || unp.ungdsc || ' - ' || unp.ungabrev AS unidadegestorap,
       unc.ungcod || ' / ' || unc.ungdsc || ' - ' || unc.ungabrev AS unidadegestorac,
       coalesce(rpp.nome,' - ') AS resp_proponente,
       coalesce(rpc.nome,' - ') AS resp_concedente,
       esd.esddsc AS esddsc,
       coalesce(cdn.coodsc, '-') AS coodsc,
       pro.proid,
       pro.provalor AS valor,
       ndp.ndpcod || ' - ' || ndp.ndpdsc AS ntddsc,
       COALESCE(pli.plicod, ' - ') AS plicod,
       COALESCE(ptr.ptres || ' - '
                    || ptr.funcod || '.'
                    || ptr.sfucod || '.'
                    || ptr.prgcod || '.'
                    || ptr.acacod || '.'
                    || ptr.unicod || '.'
                    || ptr.loccod, '-') AS ptres_desc
  FROM monitora.termocooperacao tcp
    LEFT JOIN elabrev.coordenacao cdn ON cdn.cooid = tcp.cooid
    LEFT JOIN public.unidadegestora unp ON unp.ungcod = tcp.ungcodproponente			
    LEFT JOIN public.unidadegestora unc ON unc.ungcod = tcp.ungcodconcedente
    LEFT JOIN elabrev.representantelegal rpc ON rpc.ug = tcp.ungcodconcedente
    LEFT JOIN elabrev.representantelegal rpp ON rpp.ug = tcp.ungcodproponente
    LEFT JOIN workflow.documento doc ON doc.docid = tcp.docid
    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid

    LEFT JOIN monitora.previsaoorcamentaria pro USING(tcpid)
    LEFT JOIN monitora.pi_planointerno pli ON pro.pliid = pli.pliid
    LEFT JOIN public.naturezadespesa ndp USING(ndpid)
    LEFT JOIN monitora.ptres ptr USING(ptrid)
  WHERE tcpstatus = 'A' {$where}
  ORDER BY tcpid DESC
DML;
//  ver($sql);
  return $sql;
}

function monta_coluna()
{
    $arLabels = retornaColunasELabels();
    $colunaArr = (array)$_REQUEST['coluna'];
    $coluna = array();
    foreach ($colunaArr as $col) {
        switch ($col) {
            case 'descricao':
                array_push($coluna, array('campo' => 'descricao', 'label' => 'Descrição'));
                continue;
            case 'unidadegestorap':
                array_push($coluna, array('campo' => 'unidadegestorap', 'label' => 'UG do Proponente'));
                continue;
            case 'unidadegestorac':
                array_push($coluna, array('campo' => 'unidadegestorac', 'label' => 'UG do Cedente'));
                continue;
            case 'resp_proponente':
                array_push($coluna, array('campo' => 'resp_proponente', 'label' => 'Representante do Proponente'));
                continue;
            case 'resp_concedente':
                array_push($coluna, array('campo' => 'resp_concedente', 'label' => 'Representante do Cedente'));
                continue;
            case 'esddsc':
                array_push($coluna, array('campo' => 'esddsc', 'label' => 'Estado da Documentação'));
                continue;
            case 'coodsc':
                array_push($coluna, array('campo' => 'coodsc', 'label' => 'Coordenação'));
                continue;
            case 'valor':
                array_push($coluna, array('campo' => 'valor', 'label' => $arLabels['valor']));
                continue;
            case 'ptres_desc':
                array_push($coluna, array('campo' => 'ptres_desc', 'label' => $arLabels['ptres_desc']));
                continue;
            case 'plicod':
                array_push($coluna, array('campo' => 'plicod', 'label' => $arLabels['plicod']));
                continue;
            case 'ntddsc':
                array_push($coluna, array('campo' => 'ntddsc', 'label' => $arLabels['ntddsc']));
                continue;
        }
    }
    return $coluna;
}

/**
 * Callback para array_walk. Coloca todos os itens do array entre aspas.
 * @param string $var Item do array.
 */
function quote(&$var)
{
    $var = "'{$var}'";
}

function criarPlanoTrabalhoEmenda( $docid  ){
	global $db;
	
		
	$sql = "SELECT tcpid, tcptipoemenda FROM monitora.termocooperacao WHERE docid = ".$docid;
	$arrTermo = $db->pegaLinha($sql);
	
	if( $arrTermo['tcptipoemenda'] == 'S' ){
		$emeid = $db->pegaUm("select emeid from emenda.emendatermocooperacao where tcpid = {$arrTermo['tcpid']}");
		
		$sql = "select 
					e.emeid,
				    e.emecod,
		            e.resid,
		            ve.entid,
		            ve.edevalor,
		            ve.edeid
				from emenda.emenda e
					inner join emenda.v_emendadetalheentidade ve on ve.emeid = e.emeid
					inner join emenda.entidadebeneficiada en on en.enbid = ve.entid
				where
					e.emeid = $emeid
				    and e.etoid = 2
				    and e.emeano = '".date('Y')."'
				    and ve.edestatus = 'A'";
		
		$arEmenda = $db->pegaLinha($sql);
		
		$ptridpai = 'null';
		$ptrcod = ($db->pegaUm ( "SELECT max(ptrcod) + 1 FROM emenda.planotrabalho" ));
		
		$resid = $arEmenda['resid'];
		$enbid = $arEmenda['entid'];
		$edeid = $arEmenda['edeid'];
		$edevalor = $arEmenda['edevalor'];
		
		if ($resid == 3) {
			$mdeid = 3;
		} else {
			$mdeid = $db->pegaUm ( "SELECT mdeid FROM emenda.modalidadeensino where resid = $resid" );
		}
		
		$sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrstatus, resid, mdeid, ptridpai, ptrcod, ptrsituacao, sisid )
				VALUES ( {$enbid}, " . date ( 'Y' ) . ", 'A', {$resid}, {$mdeid}, " . $ptridpai . ", " . $ptrcod . ", 'E', 2) RETURNING ptrid";
		$ptrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO emenda.ptemendadetalheentidade(ptrid, edeid, pedvalor) 
				VALUES ($ptrid, $edeid, $edevalor)";
		$db->executar ( $sql );
		
		ob_clean ();
		include_once APPRAIZ . 'includes/workflow.php';
		// cria o docid (workflow) do PTA
		$tpdid = 8;
		$docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;
		$docidPTA = wf_cadastrarDocumento ( $tpdid, $docdsc );
		
		$sql = "UPDATE emenda.planotrabalho SET docid = " . $docidPTA . " WHERE ptrid = " . $ptrid;
		$db->executar ( $sql );
		$db->commit ();
		
		$pta = $ptrcod . '/' . date('Y');
	}
	
	enviar_email_g( $docid );
	
	return true;
}

function getCelulaOrcamentaria($tcpid) {
    global $db;
    $sql = <<<DML
SELECT DISTINCT
       pro.proid,
       pro.tcpid,
       ptres || ' - ' || p.funcod || '.' || p.sfucod ||'.' || p.prgcod || '.'
             || p.acacod || '.' || p.unicod || '.' || p.loccod AS ptrid_descricao,
       SUBSTR(pi.plicod || ' - ' || pi.plidsc, 1, 45) ||'...' AS pliid_descricao,
       SUBSTR(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc AS ndp_descricao,
       pro.ptrid,
       a.acacod,
       pro.pliid,
       CASE WHEN a.acatitulo IS NOT NULL THEN substr(a.acatitulo, 1, 70) || '...'
            ELSE SUBSTR(a.acadsc, 1, 70) || '...'
         END AS acatitulo,
       pro.ndpid,
       TO_CHAR(pro.provalor, '999G999G999G999G999D99') AS provalor,
       COALESCE(pro.provalor, 0) AS valor,
       crdmesliberacao,
       crdmesexecucao,
       pro.proid,
       pro.proanoreferencia,
       pro.prodata,
       (SELECT ppa2.codncsiafi
          FROM elabrev.previsaoparcela ppa2
          WHERE ppa2.ppaid = (SELECT MAX(ppa1.ppaid)
                                FROM elabrev.previsaoparcela ppa1
                                WHERE ppa1.proid = pro.proid)
            AND ppa2.ppacancelarnc = 'f') AS lote,
       pp.codsigefnc,
       pp.codncsiafi
  FROM monitora.previsaoorcamentaria pro
    LEFT JOIN monitora.pi_planointerno pi           ON pi.pliid = pro.pliid
    LEFT JOIN monitora.pi_planointernoptres pts     ON pts.pliid = pi.pliid
    LEFT JOIN public.naturezadespesa ndp            ON ndp.ndpid = pro.ndpid
    LEFT JOIN monitora.ptres p                      ON p.ptrid = pro.ptrid
    LEFT JOIN monitora.acao a                       ON a.acaid = p.acaid

    LEFT JOIN public.unidadegestora u 			    ON u.unicod = p.unicod
	LEFT JOIN monitora.pi_planointernoptres pt 	    ON pt.ptrid = p.ptrid

    LEFT JOIN elabrev.previsaoparcela pp            ON (pp.proid = pro.proid)

    LEFT JOIN monitora.termocooperacao tc           ON (tc.tcpid = pro.tcpid)
	LEFT JOIN public.unidadegestora unc             ON (unc.ungcod = tc.ungcodconcedente)

  WHERE pro.prostatus = 'A'
    AND pro.tcpid = {$tcpid}
  ORDER BY lote,
           pro.proanoreferencia DESC,
           crdmesliberacao
DML;
//    ver($sql, d);
    $dados = !empty($tcpid) ? $db->carregar($sql) : array();
    return $dados;
}

function celulaOrcamentariaTable($tcpid, $loteid = null, $xls = null) {
    global $db;

    $dados = getCelulaOrcamentaria($tcpid);
//    ver($dados);

    if (null !== $loteid) {

        $strSQL = "select proid from elabrev.lotemacroitens where lotid = {$loteid} AND loistatus = 'A'";
        $itensSelected = $db->carregar($strSQL);
        $proIds = array();
        if ($itensSelected) {
            foreach ($itensSelected as $linha) {
                array_push($proIds, $linha['proid']);
            }
        }
    } else {

        $strSQL = "select proid from elabrev.lotemacroitens where tcpid = {$tcpid} AND loistatus = 'A'";
        $itensSelected = $db->carregar($strSQL);
        $proIds = array();
        if ($itensSelected) {
            foreach ($itensSelected as $linha) {
                array_push($proIds, $linha['proid']);
            }
        }
    }

    if ($dados[0] != '') {

        //retira as celulas orçamentárias que ja foram descentralizadas via lote
        foreach ($dados as $k => $dado) {
            if (is_array($proIds) && in_array($dado['proid'], $proIds)) {
                unset($dados[$k]);
            }

            //Retira também as celulas orçamentárias que já foram descentralizadas
            $existePrev = $db->pegaLinha("select * from elabrev.previsaoparcela where proid = {$dado['proid']}");
            if ($existePrev) {
                unset($dados[$k]);
            }
        }

        $arAnosPrevisao = array();
        $arLote = array();
        $totalPrevisao = count($dados)-1;

        if (!$xls)
            echo '<div class="celula_container">';

        echo monta_cabecalho_celula_orcamentaria();

        foreach ($dados as $k => $dado) {

            if ($xls && is_array($proIds) && !in_array($dado['proid'], $proIds))
                continue;

            if (!in_array($dado['lote'], $arLote)) {
                if ($subTotalPorLote>0) {
                    echo '<tr bgcolor="#f0f0f0" id="tr_'.$k.'">
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                            <td align="right"><img align="absmiddle" src="../imagens/icone_lupa.png" border="0" class="linkLote" codigo="'.$loteAnterior.'" alt="Visualizar lote" title="Visualizar lote"/>&nbsp;<span class="linkLote" codigo="'.$loteAnterior.'"><b>Subtotal ('.($loteAnterior ? $loteAnterior : 'lote não encontrado').')</b></span>&nbsp;</td>
                            <td align="right" id="td_subtotalano_'.($loteAnterior ? $loteAnterior : '0000').'" style="font-weight:bold;">R$ '.formata_valor($subTotalPorLote).'</td>
                            <td>&nbsp;</td>
                            <td>&nbsp;</td>
                          </tr>';
                }
                array_push($arLote, $dado['lote']);
                $subTotalPorLote = 0;
                $loteAnterior = $dado['lote'];
            }

            ?>
            <tr id="tr_<?=$dado['proid']?>">
                <td align="center">
                    <?php
                    if (!$xls) {
                        echo "<input name='proid[]' {$checked} id='checkEnvio_{$dado['proid']}' value='{$dado['tcpid']}-{$dado['proid']}' type='checkbox'/>";
                    }
                    ?>
                </td>

                <td align="center" id="td_anoref_<?=$dado['proid']; //$k ?>" width="14%">
                    <?php if($habilita_Natur == 'S'){?>
                        <?php
                        for($z=0;$z<=10;$z++){
                            $arAnosRef[$z]['codigo']	= 2013+$z;
                            $arAnosRef[$z]['descricao']	= 2013+$z;
                        }

                        $db->monta_combo('proanoreferencia[]',$arAnosRef, 'S', 'Selecione...','',$opc,'','','S', 'proanoreferencia_'.$dado['proid'], '', $dado['proanoreferencia'], $title= null);
                        ?>
                    <?php }else{ ?>
                        <?php echo $dado['proanoreferencia'] ? $dado['proanoreferencia'] : '-'; ?>
                        <input type="hidden" name="proanoreferencia[]" id="proanoreferencia_<?php echo $dado['proid']; ?>" value="<?=$dado['proanoreferencia'] ?>"/>
                    <?php } ?>
                </td>

                <td align="center" id="td_acao_<?=$k ?>">
                    <?=$dado['acacod'] ?>
                </td>

                <?php if($habilita_Plano == 'S'){?>
                    <td align="center" id="td_prg_<?=$dado['proid']; //$k ?>"  width="10%" >
                        <?php
                        $sql = "SELECT DISTINCT
                                        p.ptrid as codigo,
                                        ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
                                    FROM monitora.ptres p
                                    JOIN public.unidadegestora u
                                        ON u.unicod = p.unicod
                                    WHERE p.ptrano = '2013'
                                    AND p.ptrstatus = 'A'
                                    AND u.unicod IN ( '26101','26298','26291','26290' )";

                        //$db->monta_combo('ptrid[]',$sql, 'S', 'Selecione...','',$opc,'','200','S', 'combobox', '', $dado['ptrid'], $title= null);
                        ?>
                        <?=campo_texto('ptrid_temp[]','S','S','',20,27,'','', 'left', '', '', 'id="ptridtemp_'.$dado['proid'].'"', '', $dado['ptrid_descricao']);?>
                        <input type="hidden" name="ptrid[]" id="ptrid_<?php echo $dado['proid']; ?>" value="<?php echo $dado['ptrid']; ?>" />
                    </td>
                <?php }else{ ?>
                    <td align="center" id="td_prg_<?=$k ?>">
                        <input type="hidden" name="ptrid[]" value="<?=$dado['ptrid']?>">
                        <?=$dado['ptrid_descricao']?$dado['ptrid_descricao']:'-'?>
                    </td>
                <?php }?>

                <?php if ($habilita_Plano == 'S') { ?>
                    <td align="center" id="td_pi_<?=$dado['proid']; //$k ?>"  width="10%">
                        <?php
                        if( $dado['ptrid'] != '' ){
                            $sql = "SELECT p.pliid as codigo,
                                            plicod||' - '||plidsc as descricao
                                    FROM monitora.pi_planointerno p
                                    INNER JOIN monitora.pi_planointernoptres pt on pt.pliid = p.pliid
                                    WHERE pt.ptrid = ".$dado['ptrid']."
                                    ORDER by 2";

                            echo $db->monta_combo('pliid[]',$sql, 'S', 'Selecione...','',$opc,'','85','S', 'pliid', '', $dado['pliid'], $title= null);
                        }
                        ?>
                    </td>
                <?php } else { ?>
                    <td align="center" id="td_pi_<?=$dado['proid']; //$k ?>">
                        <input type="hidden" name="pliid[]" value="<?=$dado['pliid']?>">
                        <?=$dado['pliid_descricao']?$dado['pliid_descricao']:'-'?>
                    </td>
                <?php } ?>

                <td align="center" id="td_acaodsc_<?=$dado['proid']; //$k ?>">
                    <?=$dado['acatitulo']?$dado['acatitulo']:'-'?>
                </td>

                <?php if($habilita_Natur == 'S'){?>
                    <td align="center" width="23%">
                        <?php
                        $sql = "SELECT 	DISTINCT ndpid as codigo,
                                     substr(ndpcod, 1, 6) || ' - ' || ndpdsc as descricao
                                FROM public.naturezadespesa
                                WHERE ndpstatus = 'A' and sbecod = '00' and edpcod != '00' and substr(ndpcod,1,2) not in ( '31', '32', '46', '34' )
                                AND ( substr(ndpcod, 3, 2) in ('80', '90', '91','40') or substr(ndpcod, 1, 6) in ('335041','339147','335039', '445041', '333041') )
                                order by 2";
                        //ver($sql);
                        $db->monta_combo('ndpid[]',$sql,'S','Selecione...','',$opc,'','250','S', 'ndpid', '', $dado['ndpid'], $title= null);
                        ?>
                    </td>
                <?php }else{?>
                    <td align="center">
                        <input type="hidden" name="ndpid[]" value="<?=$dado['ndpid']?>">
                        <?=$dado['ndp_descricao']?$dado['ndp_descricao']:'-'?>
                    </td>
                <?php }?>

                <?php if($habilita_Natur == 'S'){?>
                    <td align="center" width="16%" >
                        <?=campo_texto('provalor[]','S','S','',17,17,'[.###],##','', 'right', '', '', 'id="provalor_'.$dado['proid'].'"', '', $dado['provalor']);?>
                    </td>
                <?php }else{?>
                    <td align="center">
                        <input type="hidden" name="provalor[]" value="<?=$dado['provalor']?>">
                        <?=$dado['provalor']?>
                    </td>
                <?php }?>

                <?php if($habilita_Meslib == 'S'){ ?>
                    <td align="center" width="10%">
                        <?php
                        $sql = Array(Array('codigo'=>1,'descricao'=>'Janeiro'),
                            Array('codigo'=>2,'descricao'=>'Fevereiro'),
                            Array('codigo'=>3,'descricao'=>'Março'),
                            Array('codigo'=>4,'descricao'=>'Abril'),
                            Array('codigo'=>5,'descricao'=>'Maio'),
                            Array('codigo'=>6,'descricao'=>'Junho'),
                            Array('codigo'=>7,'descricao'=>'Julho'),
                            Array('codigo'=>8,'descricao'=>'Agosto'),
                            Array('codigo'=>9,'descricao'=>'Setembro'),
                            Array('codigo'=>10,'descricao'=>'Outubro'),
                            Array('codigo'=>11,'descricao'=>'Novembro'),
                            Array('codigo'=>12,'descricao'=>'Dezembro')
                        );
                        $db->monta_combo('crdmesliberacao[]',$sql,'S','Selecione...','',$opc,'','85','S', 'crdmesliberacao', '', $dado['crdmesliberacao'], $title= null);
                        ?>
                    </td>
                <?php }else{ ?>
                    <td align="center"  width="50%">
                        <input type="hidden" name="crdmesliberacao[]" id="'crdmesliberacao[]" value="<?=$dado['crdmesliberacao']?>">
                        <?php echo $dado['crdmesliberacao'] ? mes_extenso($dado['crdmesliberacao']) : '-';?>
                    </td>
                <?php }?>

                <?php if ($habilita_Parc == 'S') { ?>
                    <td align="center"  width="50%">
                        <?php
                        $sql = array();
                        for($i = 1; $i <= 50; $i++){
                            $sql[$i-1]['codigo'] = $i;
                            $sql[$i-1]['descricao'] = $i.' Mês(s)';
                        }
                        array_push($sql, $sql);
                        $db->monta_combo('crdmesexecucao[]', $sql,'S','Selecione...','',$opc,'','100','S', 'crdmesexecucao', '', $dado['crdmesexecucao'], $title= null);
                        ?>
                    </td>
                <?php }  else{ ?>
                    <td align="center">
                        <input type="hidden" name="crdmesexecucao[]" id="'crdmesexecucao[]" value="<?=$dado['crdmesexecucao']?>">
                        <?php echo $dado['crdmesexecucao'].' Mês(s)' ?>
                    </td>
                <?php }?>
            </tr>
            <?php

            // TODO: realizar a somatória baseada no mês
            if( $dado['lote'] != null )
                $subTotalPorLote = $subTotalPorLote+$dado['valor'];
        }
    }
    echo '</table>';

    if (!$xls) {
        echo '</div>';
    }
}

/**
 *
 */
function monta_cabecalho_celula_orcamentaria() {
    return '
    <table id="previsao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
        <tr id="tr_titulo">
			<th class="subtitulocentro">&nbsp;</th>
			<th class="subtitulocentro"  width="10%">Ano</th>
			<th class="subtitulocentro" width="">Ação</th>
			<th class="subtitulocentro" width="14%">Programa de Trabalho</th>
			<th class="subtitulocentro" width="10%">Plano Interno</th>
			<th class="subtitulocentro" width="">Descrição da Ação Constante da LOA</th>
			<th class="subtitulocentro" width="20%">Nat.da Despesa</th>
			<th class="subtitulocentro" width="13%">Valor (em R$ 1,00)</th>
			<th class="subtitulocentro" width="8%">Mês da Liberação</th>
			<th class="subtitulocentro" width="20%">Prazo para o cumprimento do objeto</th>
		</tr>';
}

/**
 * Envia log de erro
 */
function enviar_email_dev($titulo_assunto, $content) {

    if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = array("Lucas.Gomes@mec.gov.br");
        $conteudo = '<p>Log:</p>';
        $conteudo.= '<p>'.$content.'</p>';

        $remetente = array('nome'=>'Programação Orçamentária - Log de Erro', 'email'=>$_SESSION['email_sistema']);
        $assunto  = $titulo_assunto;

        enviar_email($remetente, $email, $assunto, $conteudo);
        return true;
    }
}

//Enviado para os responsaveis da SPO
//estado: Termo em análise pela Coordenação
//para: Enviar para arquivamento
function enviar_email_arq($docid) {
    global $db;

    $sql = sprintf("SELECT usuemail	FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
    $atual = $db->pegaUm($sql);

    $email2 = array($_SESSION['email_sistema']);

    if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando analise da SPO.</p>";
    } else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando analise da SPO.</p>";
    }

    $remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
    $assunto  = "O termo de execução descentralizada nº $tcpid foi enviado para Arquivado. Aguardando analise da SPO.";
    enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
    return true;
}

/**
 * Enviar para termo em execução (sem nova descentralização)
 * Estado: "Termo em análise pela Coordenação"
 * Destino: "Enviar para Termo em Execução (sem nova descentralização)"
 * @param $docid
 * @return bool
 */
function enviar_email_tec_prop($docid) {
    global $db;

    $stmt = sprintf("SELECT tcpid FROM monitora.termocooperacao WHERE docid = %d", (int) $docid);
    $tcpid = $db->pegaUm($stmt);

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".UO_EQUIPE_TECNICA."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodproponente from monitora.termocooperacao where tcpid = {$tcpid})
    ";
    $email2 = $db->carregarColuna($strSQL);

    if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando análise da Equipe Técnica.</p>";
    } else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando análise da Equipe Técnica.</p>";
    }

    $remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
    $assunto  = "O termo de execução descentralizada nº $tcpid foi enviado para Execução (sem nova descentralização). Aguardando análise da Equipe Técnica.";
    enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
    return true;
}


/**
 * Estado: "Termo em análise pela Coordenação"
 * Destino: "Enviar para aguardando disponibilidade orçamentária"
 * @param $docid
 * @return bool
 */
function enviar_email_dispo_orca($docid) {
    global $db;

    $stmt = sprintf("SELECT tcpid FROM monitora.termocooperacao WHERE docid = %d", (int) $docid);
    $tcpid = $db->pegaUm($stmt);

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".PERFIL_COORDENADOR_SEC."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";
    $email2 = $db->carregarColuna($strSQL);

    if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if (!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando análise da Equipe Técnica.</p>";
    } else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando análise da Equipe Técnica.</p>";
    }

    $remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
    $assunto  = "O termo de execução descentralizada nº $tcpid foi enviado para Execução (sem nova descentralização). Aguardando análise da Equipe Técnica.";
    enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
    return true;
}

/**
 * Estado: "Aguardando aprovação pela Diretoria"
 * Destino: "Termo em análise orçamentária no FNDE"
 * @param $docid
 * @return bool
 */
function enviar_email_area_tec_fnde($docid) {
    global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod = ".PERFIL_AREA_TECNICA_FNDE."
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";

    $sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
    $atual = $db->pegaUm($sql);

    if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if(!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação da Área Tecnica do FNDE.</p>";
    } else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação da Área Tecnica do FNDE.</p>";
    }

    $remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
    $assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação da Área Tecnica do FNDE.";
    enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
    return true;
}

/**
 * Estado: "Termo em análise orçamentária no FNDE"
 * Destino: "Encaminhar para validação da diretoria no FNDE"
 * @param $docid
 * @return bool
 */
function enviar_email_dir_fnde($docid) {
    global $db;

    $tcpid = $db->pegaUm("SELECT tcpid FROM monitora.termocooperacao WHERE docid = {$docid}");

    $strSQL = "
        select
            u.usuemail
        from elabrev.usuarioresponsabilidade ur
        inner join seguranca.usuario u on (u.usucpf = ur.usucpf)
        where ur.pflcod in (".PERFIL_DIRETORIA_FNDE.", ".PERFIL_AREA_TECNICA_FNDE.")
            and ur.rpustatus = 'A'
            and ur.prsano = '{$_SESSION['exercicio']}'
            and ur.ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
    ";

    $sql = sprintf("SELECT usuemail FROM seguranca.usuario WHERE usucpf = '%s'", (string) $_SESSION['usucpf']);
    $atual = $db->pegaUm($sql);

    if (strstr($_SERVER['HTTP_HOST'], 'simec.mec.gov.br')) {
        $email = $email2;
        if(!$email) {
            $email = $atual;
        }
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação da diretoria do FNDE.</p>";
    } else {
        $email = array($_SESSION['email_sistema']);
        $cc = $atual;
        $conteudo = "<p>O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação da diretoria do FNDE.</p>";
    }

    $remetente = array('nome'=>'Programação Orçamentária - Descentralização de Crédito', 'email'=>$_SESSION['email_sistema']);
    $assunto  = "O termo de execução descentralizada nº $tcpid foi aprovado. Aguardando aprovação da diretoria do FNDE.";
    enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco);
    return true;
}