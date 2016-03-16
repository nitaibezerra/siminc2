<?php

include_once APPRAIZ . 'includes/workflow.php';

function verificaEnviarEmbaixada() {
    global $db;
    //return true;

    $docid = criaDocumento($_SESSION['eexid']);
  
	$sqlDocumento =	"SELECT count (*) as total
			
			FROM escolasexterior.tpdocescolaext AS a

			LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
			LEFT JOIN seguranca.usuario AS u ON p.usucpf = u.usucpf
			LEFT JOIN escolasexterior.tipodocumento t ON t.tpdid = a.tpdid
			
			WHERE 
				a.tdestatus = 'A' 
				AND a.eexid = '{$_SESSION['eexid']}' AND a.tpdid IN(' " . TPD_COMPROVACAO . "', '" . TPD_DESCRICAO . "','" . TPD_ORGANIZACAO . "','" . TPD_PROPOSTA . "','" . TPD_REGIMENTO . "') ";
   $resultadoDoc = $db->pegaUm($sqlDocumento);

    $sqlPessoas = "
            SELECT
			COUNT(*) as total 
  		FROM escolasexterior.pessoalescola p
		inner join escolasexterior.pessoalcargo pc2 on p.peeid = pc2.peeid
        	inner join escolasexterior.cargo c2 on pc2.cgoid = c2.cgoid
		WHERE p.eexid = '{$_SESSION['eexid']}'
		and p.peestatus = 'A'
		and pc2.cgoid = '11'";
    $resultadoPes = $db->pegaUm($sqlPessoas);

    $sqlDocPessoa = "	SELECT
	count(*) as total
				
					FROM escolasexterior.pessoalescolaarquivo AS a
					inner join escolasexterior.pessoalescola pee on a.peeid = pee.peeid
				LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
				inner join escolasexterior.pessoalcargo pc2 on pee.peeid = pc2.peeid
				left join escolasexterior.cargo c2 on pc2.cgoid = c2.cgoid
					WHERE a.pesstatus = 'A' and pee.eexid = '{$_SESSION['eexid']}' and pc2.cgoid = '11'";
    $resultadoDocPes = $db->pegaUm($sqlDocPessoa);                               
    if ($resultadoDoc < 5) {
        return "Existem Documentos pendentes.";
    }
    if ($resultadoPes == 0) {
        return "Necessário cadastro do diretor.";
    }
     if ($resultadoDocPes == 0) {
        return "Necessário cadastro de documento do diretor.";
    }
    if ($resultadoDoc >= 5 && $resultadoPes >= 1 && $resultadoDocPes >= 1) {
        return true;
    }
}

/**
 * Recupera o(s) perfil(is) do usuário no módulo
 * 
 * @return array $pflcod
 */
function arrayPerfil()
{
	/*** Variável global de conexão com o bando de dados ***/
	global $db;

	/*** Executa a query para recuperar os perfis no módulo ***/
	$sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN 
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = ".SISID_EE."
			WHERE
				pu.usucpf = '".$_SESSION['usucpf']."'
			ORDER BY
				p.pflnivel";
	$pflcod = $db->carregarColuna($sql);
	
	/*** Retorna o array com o(s) perfil(is) ***/
	return (array)$pflcod;
}

function checkPerfil( $pflcods ){

	global $db;

	//if ($db->testa_superuser()) {

		//return true;

	//}else{

		if ( is_array( $pflcods ) )
		{
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		}
		else
		{
			$pflcods = array( (integer) $pflcods );
		}
		if ( count( $pflcods ) == 0 )
		{
			return false;
		}
		$sql = "
			select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		return $db->pegaUm( $sql ) > 0;

	//}
}



// INICIO FUNÇÕES DO WORKFLOW

function criaDocumento( $eexid ) {
	
	global $db;
	
	if(!isset($_SESSION['eexid'])){
		return false;
	}
	

	if(!$eexid) return false;
	
	$docid = pegaDocid($eexid);
	
	if( !$docid ){
				
		$tpdid = WF_TPDID_EE;
		
		/*
		 * Pega nome da entidade
		 */
		$sqlDescricao = "SELECT
						  eexnomeestabelecimento
						 FROM
						  escolasexterior.escolasexterior
						 WHERE
						  eexid = '" . $eexid . "'";
		
		$descricao = $db->pegaUm( $sqlDescricao );
		
		$docdsc = "Cadastramento Escolas no Exterior - " . $descricao;
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
		
		if($eexid) {
			$sql = "UPDATE escolasexterior.escolasexterior SET 
					 docid = ".$docid." 
					WHERE
					 eexid = ".$eexid;

			$db->executar( $sql );		
			$db->commit();
			return $docid;
		}else{
			return false;
		}
	}
	else {
		return $docid;
	}
}

function pegaEstadoAtual( $docid ) {
	
	global $db; 
	
	if($docid) {
		$docid = (integer) $docid;
		 
		$sql = "
			select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
		$estado = $db->pegaUm( $sql );
		 
		return $estado;
	} else {
		return false;
	}
}

function pegaDocid( $eexid ) {
	
	global $db;
	
	$eexid = (integer) $eexid;
	
	$sql = "SELECT
			 docid
			FROM
			 escolasexterior.escolasexterior
			WHERE
			 eexid = " . $eexid . " AND 
			 eexstatus = 'A'";
	
	return (integer) $db->pegaUm( $sql );
}

// FINAL FUNÇÕES DO WORKFLOW



function maxProgramacaoExercicio() {
	
	global $db;
	
	$sql = "SELECT
				max(prsano)
			FROM
				em.programacaoexercicio
			WHERE
				prsstatus = 'A'
				AND prsexerccorrente = 't'";
	
	return (integer)$db->pegaUm($sql);
	
}

function cabecalhoEE($eexid = null) {

	global $db;
	
	$eexid = $eexid ? $eexid : $_SESSION['eexid'];
	
	if($eexid){
		
		$sql = "SELECT DISTINCT
					eexnomeestabelecimento as escola,
					eexcidade as cidade,
					eexendereco as endereco
				FROM
					escolasexterior.escolasexterior	
				WHERE
					eexid = {$eexid}
				  	AND eexstatus = 'A'";
		
		$dadose = $db->pegaLinha($sql);
		
		$cab = "<table align=\"center\" class=\"Tabela\">
				 <tbody>
					<tr>
						<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Escola:</td>
						<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadose['escola']}</td>
					</tr>			 
					<tr>
						<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Cidade:</td>
						<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadose['cidade']}</td>
					</tr>
					<tr>
						<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Endereço:</td>
						<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dadose['endereco']}</td>
					</tr>
				 </tbody>
				</table>";
	}
	
	echo $cab;
}

function montaAbasEscolasExterior(){
	
	if($_SESSION['eexid']){
		$menu = array(0 => array("id" => 1, "descricao" => "Lista", 		"link" => "/escolasexterior/escolasexterior.php?modulo=principal/lista&acao=A"),
			  1 => array("id" => 2, "descricao" => "Dados da Escola", 	"link" => "/escolasexterior/escolasexterior.php?modulo=principal/dadosEscola&acao=A"),
			  2 => array("id" => 3, "descricao" => "Documentos", 		"link" => "/escolasexterior/escolasexterior.php?modulo=principal/cadastroDocumento&acao=A"),
			  3 => array("id" => 4, "descricao" => "Dados do Pessoal", 	"link" => "/escolasexterior/escolasexterior.php?modulo=principal/cadastroPessoal&acao=A"),
			  4 => array("id" => 5, "descricao" => "Trâmite", 			"link" => "/escolasexterior/escolasexterior.php?modulo=principal/tramite&acao=A")
			  // 5 => array("id" => 6, "descricao" => "Etapa de Ensino", 	"link" => "/escolasexterior/escolasexterior.php?modulo=principal/cadastroEtapaEnsino&acao=A")
	  	  );
	}
	else{
		$menu = array(0 => array("id" => 1, "descricao" => "Lista", 	"link" => "/escolasexterior/escolasexterior.php?modulo=principal/lista&acao=A"),
			  1 => array("id" => 2, "descricao" => "Dados da Escola", 	"link" => "/escolasexterior/escolasexterior.php?modulo=principal/cadastroEscola&acao=A")
	  	  );
		
	}
	
	echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
	
}

function verificaSessao(){
	if(!$_SESSION['eexid']){
		?>
		<script>
			alert("Sua sessão expirou. Por favor, entre novamente!");
			location.href='escolasexterior.php?modulo=inicio&acao=C';
		</script>
		<?
		exit;
	}
}


/**
 * Enviador de email para as diferentes condições do Escolas Exterior
 * Proveniente da Pós-ação 'envioEmailGenericoEscolasExterior( eexid, esdid )'
 *
 * @author Sávio Resende
 * @return BOOL
 * @param Integer $eexid
 * @param String $tipo
 */
function envioEmailGenericoEscolasExterior( $eexid = null, $docid = null ){
	global $db;
	require APPRAIZ . '/includes/Email.php';
	
	if(!$_SESSION['eexid']) return false;
	
	$sql = "select docid from escolasexterior.escolasexterior where eexid = ".$_SESSION['eexid'];
	$docid = $db->pegaUm($sql);
	
	if(!$docid) return false;
	
	$comentario = wf_pegarComentarioEstadoAtual( $docid );
	$estadoAnterior = wf_pegarEstadoAnterior( $docid );
	$acao = wf_pegarAcao2( $estadoAnterior['aedid'] );
	// return true;
	// ver($estadoAnterior['aedid'],d);

	$assunto = "Tramitação em Escolas Exterior - SIMEC";
	$conteudo = "Mensagem: <br> ------ <br>" . $comentario."<br> ------- <br>";
	$conteudo .= "<br><br>Ação: ".$acao['aeddscrealizar']." ";
	$destinatarios = array();

	switch ( $estadoAnterior['aedid'] ) {
		// estado iniciao - estado destino
		case WF_AEDID_PARA_VALIDACAO_EMBAIXADA: // para validacao embaixada - validador
			// PERFIL_VALIDADOR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_VALIDADOR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_AJUSTES_NO_CADASTRO: // para ajuste de cadastro ( 975 - 983 ) - cadastrador
			// PERFIL_CADASTRADOR - com pais
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join escolasexterior.escolasexterior e on e.eexid = " . $_SESSION['eexid'] . "
				join escolasexterior.usuarioresponsabilidade ur on ur.usucpf = u.usucpf and ur.pflcod = " . PERFIL_CADASTRADOR . " and ur.paiid = 3
				join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = u.usucpf
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_ANALISE_ASSESSORIA_INTERNACIONAL: // para analise assessoria internacional ( 975 - 976 ) - Administrador
			// PERFIL_ADMINISTRADOR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_ADMINISTRADOR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_DOCUMENTACAO_COMPLEMENTAR: // para documentacao complementar ( 975 - 983 ) - Validador
			// PERFIL_VALIDADOR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_VALIDADOR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_APROVACAO_NA_CEB: // para aprovacao na seb ( 976 - 977 ) - Aprovador SEB
			// PERFIL_APROVADOR_SEB
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_APROVADOR_SEB . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_PARECER_CNE: // para parecer cne ( 976 - 978 ) - Administrador
			// PERFIL_ADMINISTRADOR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_ADMINISTRADOR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_ANALISE_ASSESSORIA_INTERNACIONAL_DE_SEB: // para analise assessoria internacional de seb ( 977 - 976 ) - Administrador
			// PERFIL_ADMINISTRADOR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_ADMINISTRADOR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_CADASTRAMENTO_INEP_NA_DCEI: // para cadastramento inep ( 977 - 981 ) - Aprovador SEB
			// PERFIL_APROVADOR_SEB
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_APROVADOR_SEB . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_PARECER_CONJUR: // para parecer conjur ( 978 - 979 ) - Parecerista CONJUR
			// PERFIL_PARECERISTA_CONJUR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_PARECERISTA_CONJUR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_ANALISE_ASSESSORIA_INTERNACIONAL_DE_CNE: // para anbalise assessoria internacional de cne ( 978 - 976 ) - Administrador
			// PERFIL_ADMINISTRADOR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_ADMINISTRADOR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_HOMOLOGACAO_NO_GABINETE_DO_MINISTRO: // para homologacao gabinete ( 979 - 980 ) - Homologador
			// PERFIL_HOMOLOGADOR
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_HOMOLOGADOR . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_CADASTRAMENTO_INEP_NA_SEB: // para cadastramento inep de gabinete ( 980 - 977 ) - Cadastrador INEP
			// PERFIL_CADASTRADOR_INEP
			$sql = " 
				select u.usuemail as email
				from seguranca.usuario u
				join seguranca.perfilusuario pu on pu.pflcod = " . PERFIL_CADASTRADOR_INEP . " and pu.usucpf = u.usucpf 
			";
			$resultados = $db->carregar( $sql );
			foreach ($resultados as $chave => $valor)
				array_push($destinatarios, $valor['email']);
			break;
		case WF_AEDID_PARA_CRIAR_PROCESSO_CONJUR: // para criar processo conjur ( 979 - 979 ) - Sem email	
		case WF_AEDID_PARA_INSERIR_PARECER_CONJUR: // para inserir parecer conjur ( 979 - 979 ) - Sem email	
		case WF_AEDID_PARA_HOMOLOGAR_PARECER_CNE_PENDENTE: // para homologar parecer cne ( 980 - 980 ) - Sem email	
		case WF_AEDID_PARA_PARECER_CNE: // para parecer cne de cne ( 978 - 978 ) - Sem email	
		case WF_AEDID_PARA_INSERIR_NOVA_NOTA_TECNICA_DESPACHO_REGULARIDADE: // para inserir nota tecnica ( 977 - 977 ) - Sem email	
		default:
			return true;
			break;
	}
	// ver($destinatarios,d);

	$copia_oculta = array();
    $remetente     = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
	$retorno = enviar_email($remetente, $destinatarios, $assunto, $conteudo);
    return $retorno;
}

// Função utilizada como condição da ação 'Enviar para parecer CONJUR' do workflow.
function verificaEnviarCONJUR(){
	global $db;
	//return true;
	
	$sql = "select docid from escolasexterior.escolasexterior where eexid = ".$_SESSION['eexid'];
	$docid = $db->pegaUm($sql);
		
	$estadoAtual = wf_pegarEstadoAtual($docid);
	
	$msg = '';
	switch ($estadoAtual['esdid']){
		// Código executado quando esta função for executada a partir do estado 'Aguardando parecer CNE'
		case WF_ESDID_AGUARDANDO_PARECER_CNE:
			$sqlDocumento =	"SELECT count (*) as total
				
							FROM escolasexterior.tpdocescolaext AS a
							
							LEFT JOIN public.arquivo AS p ON a.arqid = p.arqid
							LEFT JOIN seguranca.usuario AS u ON p.usucpf = u.usucpf
							LEFT JOIN escolasexterior.tipodocumento t ON t.tpdid = a.tpdid
								
							WHERE
							a.tdestatus = 'A'
							AND a.eexid = '{$_SESSION['eexid']}' AND a.tpdid IN(' " . TPD_PARECER_CNE . "') ";
			
			$resultadoDoc = $db->pegaUm($sqlDocumento);
			
			if ($resultadoDoc < 5) {
				$msg = "Parecer CNE pendente.";
			}
			break;
	}
	
	if ($msg != ''){
		return $msg;
	} else {
		return true;
	}
	
}

// Função utilizada como condição da ação 'Enviar para análise Assessoria Internacional' do workflow.
function verificaEnviarAssessoriaInternacional(){
	return true;
}

?>