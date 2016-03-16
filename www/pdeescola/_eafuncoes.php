<?php

/**
 * Verifica se a variável de sessão usada no
 * Escola Acessível está devidamente setada.
 * 
 * @return void
 */
function eaVerificaSessao()
{
	/*** Se alguma das variáveis de sessão estiver nula ou vazia ***/ 
	if( empty($_SESSION['eacid']) || is_null($_SESSION['eacid']) || empty($_SESSION['entid']) || is_null($_SESSION['entid']) )
	{
		echo '<script>
		
				/*** Exibe o alerta de erro ***/
				alert("Ocorreu um erro interno.\n O sistema irá redirecioná-lo à página inicial do módulo.");
				       
				/*** Redireciona o usuário ***/
				location.href = "pdeescola.php?modulo=inicio&acao=C";
				
			  </script>';
		die();
	}
}

/**
 * Recupera a escola, estado ou município
 * atribuído ao perfil do usuário no Escola Acessível
 * 
 * @param string $resp
 * @return mixed
 * @author Felipe Carvalho
 */
function eaRecuperaResponsabilidadePerfil($resp)
{
	global $db;

	$sql = "SELECT
				".$resp."
			FROM
				pdeescola.usuarioresponsabilidade
			WHERE
				usucpf = '".$_SESSION["usucpf"]."' 
				AND rpustatus = 'A'
				AND pflcod in (".PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL.",
							   ".PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL.",
							   ".PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL.")";
	return $db->pegaUm($sql);
}

/**
 * Função para montar o cabeçalho usado nas páginas do 'Escola Acessível'
 * 
 * @return string
 * 
 * Since: 15/04/2009
 */
function cabecalhoEscolaAcessivel() {

	global $db;
	
	$entid = $_SESSION['entid'];
	
	$sql = "SELECT DISTINCT
				est.estdescricao as est,
				est.estuf,
				mun.mundescricao as mun,
				ent.entnome as esc
			FROM
				entidade.entidade ent 
			INNER JOIN 
				entidade.endereco ende ON ent.entid = ende.entid
			INNER JOIN 
				territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN 
				territorios.estado est ON est.estuf = mun.estuf		
			WHERE
				--ent.funid = 3 and
			  	ent.tpcid IN (1,3) AND
		    	ent.entid IN ('{$entid}')";
	//xx($sql);
	$dados = $db->carregar($sql);
	
	$cab = "<table align=\"center\" class=\"Tabela\">
			 <tbody>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Escola</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dados[0]['esc']}</td>
				</tr>			 
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Município</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dados[0]['mun']}</td>
				</tr>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Estado</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dados[0]['est']}</td>
				</tr>
			 </tbody>
			</table>";
	
	return $cab;
}

/**
 * Função que monta as abas do 'Escola Acessível'
 *
 * @return array
 * 
 * Since: 10/03/2010
 */
function carregaAbasEscolaAcessivel() {
	global $db;
	
	if(!$_SESSION['entid']){
		$menu = array(
				  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=ealista&acao=E"),
				  );
	} else {
		$perfis = arrayPerfil();
		
		//if($_SESSION['bo_cadastrador_escola_acessivel']) {
		if( in_array(PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL, $perfis) ) {
			$menu = array(
					  0 => array("id" => 1, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/dados_escola&acao=A"),
					  1 => array("id" => 2, "descricao" => "Dados do Diretor", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
					  2 => array("id" => 3, "descricao" => "Plano de Atendimento", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/plano_acao&acao=A")
					  );
		} else {
			/*
			$menu = array(
					  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=ealista&acao=E"),
					  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/dados_escola&acao=A"),
					  2 => array("id" => 3, "descricao" => "Dados do Diretor", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
					  3 => array("id" => 4, "descricao" => "Parceria", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/diagnostico&acao=A"),
					  4 => array("id" => 5, "descricao" => "Plano de Atendimento", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/plano_acao&acao=A")
					  );
			*/
			$menu = array(
					  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=ealista&acao=E"),
					  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/dados_escola&acao=A"),
					  2 => array("id" => 3, "descricao" => "Dados do Diretor", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
					  3 => array("id" => 4, "descricao" => "Plano de Atendimento", "link" => "/pdeescola/pdeescola.php?modulo=eaprincipal/plano_acao&acao=A")
					  );
		}
	}
		
	$menu = $menu ? $menu : array();
	  
	return $menu;
}


/*
 * Monta lista de escolas em conformidade com os filtros
 */
function ealista() {
	global $db;
	
	$ano = $_SESSION["exercicio"];
	if(empty($ano)){
		echo '<script>

				/*** Exibe o alerta de erro ***/
				alert("Ocorreu um erro interno.\n O sistema irá redirecioná-lo à página inicial do módulo.");

				/*** Redireciona o usuário ***/
				location.href = "pdeescola.php?modulo=inicio&acao=C";

			  </script>';
		die();
	}
	/*
	 * Filtro
	 * Escola, Código, Estado, Municipio, Situação, Tipo
	 */
	if ($_POST['escola'])
		$where[] = " UPPER(e.entnome) LIKE UPPER('".tratarStrBusca($_POST['escola'])."')";

	if ($_POST['entcodent'])
		$where[] = " e.entcodent LIKE '%".$_POST['entcodent']."%'";	
		
	if ($_REQUEST['estuf'])
		$where[] = " m.estuf = '".$_REQUEST['estuf']."'";

	if ($_POST['muncod'])
		$where[] = " m.muncod = '".$_POST['muncod']."'";

	if ($_REQUEST['esdid'] == '0'){
		$_REQUEST['esdid'] = "naoiniciado";
	}
 	if ($_REQUEST['esdid']) {
		$naoIniciado = "";
		
		if($_REQUEST['esdid'] != "naoiniciado")
			$where[] = " est.esdid = '".$_REQUEST['esdid']."'";
		else
			$naoIniciado = "eac.docid is null and";
	} 

	if ($_POST['tpcid'])
		$where[] = " e.tpcid IN (".$_POST['tpcid'].")";	
	//else
		//$where[] = " e.tpcid IN (1,3)";		
		
	
	if( $_REQUEST['modalidade'] == 'F') {
		$where[] = " eac.eacmodalidadeensino = 'F' ";
	}
	else if( $_REQUEST['modalidade'] == 'M') {
		$where[] = " eac.eacmodalidadeensino = 'M' ";
	}
	
	/*
	 * Carrega array com perfis do usuário
	 */	
	$perfil = arrayPerfil();
	
	/*
	 * Caso não tenha acesso global
	 * vê somente o que tiver acesso, atravéz do "usuarioresponsabilidade"
	 */
	$from = "";
	if (    in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) 
		 || in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil)  
		 || in_array(PDEESC_PERFIL_CONSULTA, $perfil) 
		) {
		$from = "";
    } else {
    	if ( (in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $perfil)) && in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $perfil)){
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
						 (ur.muncod = m.muncod AND 
						  e.tpcid = 3) OR
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)"; 
		} elseif ( in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $perfil)) { //Perfil PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL só ver na sua escola ESTADUAL
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)";
		} elseif ( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $perfil)) { //Perfil PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL só ver na sua escola MUNICIPAL
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
						 (ur.muncod = m.muncod AND 
						  e.tpcid = 3) OR
	 					 ur.entid  = e.entid
	 					)";
		}
    }
    
	$sql = sprintf("SELECT * FROM(
						SELECT DISTINCT
			 				 '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaEA(\'eaajax.php\', \'tipo=redirecionaea&entid=' || e.entid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
			 				 &nbsp;&nbsp;
							 ' as acao,
							 e.entcodent,
							 e.entnome,
							 CASE
							  WHEN e.tpcid = 1 THEN 'Estadual'
							  ELSE 'Municipal'
							 END AS tipo,
							 m.estuf, 
							 m.mundescricao,
							 CASE
						  	  WHEN est.esdid IS NOT NULL THEN est.esddsc
						  	  ELSE 'Não Iniciado'
						 	 END AS situacao,
							 CASE WHEN eac.eacmodalidadeensino = 'M' THEN 'Médio' ELSE 'Fundamental' END as ensino
						FROM
							 entidade.entidade e
						INNER JOIN 
							 entidade.endereco endi ON endi.entid = e.entid
						LEFT JOIN 
							 territorios.municipio m ON m.muncod = endi.muncod
							 %s
						INNER JOIN
							 pdeescola.eacescolaacessivel eac ON %s eac.entid = e.entid AND eac.eacanoreferencia = ".$_SESSION["exercicio"]." AND eac.eacstatus = 'A'
						LEFT JOIN 
							 workflow.documento d ON d.docid = eac.docid
						LEFT JOIN 
							 workflow.estadodocumento est ON est.esdid = d.esdid
							 %s %s ) as foo 
						%s",
				$from,
				$naoIniciado,
				$where ? " WHERE ".implode(' AND ', $where)." " : ' ',
				$and,
				$where1 ? $where1 : '');
				
// 				dbg($sql,1);

	$cabecalho = array( "Ação", "Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Ensino");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
}

function eaMaxProgramacaoExercicio()
{
	/*** Temporário, enquanto não existem escolas em 2011... ***/
	//return 2011;
	
	//correção para retirar o codigo fixo "return 2011;"
	global $db;
	
	if($_SESSION['eacid']){
		$eacanoreferencia = $db->pegaUm("select eacanoreferencia from pdeescola.eacescolaacessivel where eacid = ".$_SESSION['eacid']);
	}
	else{
		$eacanoreferencia = 2011;
	}
	
	return $eacanoreferencia;
	
	/*global $db;
	
	$sql = "SELECT
				max(prsano)
			FROM
				pdeescola.programacaoexercicio
			WHERE
				prsstatus = 'A'
				AND prsexerccorrente = 't'";
	return (integer)$db->pegaUm($sql);*/
}

/**
 * Função para Verificar se existe a Diretor ou Coordenador para Entidade escolhida
 * 
 * @return string
 * 
 * Since: 29/04/2009
 */
function eaExisteDiretorCoordenadorPorCpf($funid){ // Função feita para atender necessidade do Cliente com urgência
	global $db;
	
	# Comentado por causa das modificações da entidade
	/*$sql = "SELECT mep.entid FROM pdeescola.memaiseducacao mee
			  inner join pdeescola.mepessoal mep on mee.memid = mep.memid
		      inner join entidade.entidade e on mep.entid = e.entid
		      inner join entidade.funcaoentidade fe on e.entid = fe.entid
			where mee.entid = $entid and fe.funid = $funid and mep.mepstatus = 'A' and fe.fuestatus = 'A' ";*/
	
	$entid = $_SESSION['entid'];
	
	/*
	 * Correção por Alexandre Dourado 17/11/09
	 */
	if(!$entid) {
		echo "<script>
				alert('Entidade não encontrada. Refaça o procedimento.');
				window.location='pdeescola.php?modulo=ealista&acao=E';
			  </script>";
		exit;
	}
	
	$sql = "SELECT e.entnumcpfcnpj FROM entidade.entidade e  
			INNER JOIN entidade.funcaoentidade fe on e.entid = fe.entid 
			INNER JOIN entidade.funentassoc fea on fea.fueid = fe.fueid 
			WHERE fea.entid = '".$entid."' AND fe.funid = '".$funid."'";
	$cpfDiretorCoord = $db->pegaUm($sql);
	return $cpfDiretorCoord;
}

/******* FUNÇÕES DO WORKFLOW **********/
function eaVerificaPendencias( $eacid ) { /*** incompleto ***/
	if (!$memid){
		$memid = $_SESSION["memid"];
	}
	
	global $db;
	
	$controlePendencias = true;
	
	if(!existeDiretorCoordenadorPorCpf(19)) $controlePendencias = false;
	if(!existeDiretorCoordenadorPorCpf(41)) $controlePendencias = false;
	
	$sql = "SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$_SESSION['meentid']." AND memstatus = 'A' AND memanoreferencia = ".((integer)$_SESSION["exercicio"] - 1);
	$possuiAnoAnterior = $db->carregar($sql);
	
	if($possuiAnoAnterior) {
		if(!existeAtividadesAnoAnterior($memid)) $controlePendencias = false;
	}
	
	if(!existeAtividadesAnoAtual($memid)) {
		if($possuiAnoAnterior) {
			$memvlrpago = $db->pegaUm("SELECT memvlrpago FROM pdeescola.memaiseducacao WHERE memid = ".$memid);
			$atividadesAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$possuiAnoAnterior[0]["memid"]." AND meaano = ".((integer)$_SESSION["exercicio"] - 1)." AND meacomecounoano = 't'");
			
			if($memvlrpago || (integer)$atividadesAnoAnterior > 0) {
				$controlePendencias = false;
			}
		} else {
			$controlePendencias = false;
		}
	}
	
	//if(!existeParceiro()) $controlePendencias = false;
	
	return (boolean) $controlePendencias;
}

function eaPegarDocid( $entid , $eacid ) {
	global $db;
	
	$entid = (integer) $entid;
	$eacid = (integer) $eacid;
	
	$sql = "SELECT
			 docid
			FROM
			 pdeescola.eacescolaacessivel
			WHERE
			 entid  = " . $entid . " AND 
			 eacid = " . $eacid . " AND 
			 eacstatus = 'A'";
	return (integer) $db->pegaUm( $sql );
}

function eaCriarDocumento( $entid, $eacid ) {
	global $db;
	
	$docid = eaPegarDocid($entid, $eacid);
	
	if( ! $docid ){
		
		/*
		 * Pega tipo do documento "WORKFLOW"
		 */
		/*$sqlTpdid = "SELECT
					  t.tpdid 
					 FROM 
					  seguranca.sistema s					
					  INNER JOIN workflow.tipodocumento t ON s.sisid = t.sisid					
					 WHERE
					  s.sisid = '".$_SESSION['sisid']."'";
		$tpdid = $db->pegaUm( $sqlTpdid );*/
		$tpdid = TPDID_ESCOLA_ACESSIVEL;
		
		/*
		 * Pega nome da entidade
		 */
		$sqlDescricao = "SELECT
						  entnome
						 FROM
						  entidade.entidade
						 WHERE
						  entid = '" . $entid . "'";
		
		$descricao = $db->pegaUm( $sqlDescricao );
		
		$docdsc = "Cadastramento Escola Acessivel - " . $descricao;
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );	
		
		//if ($memid = pegarMemid($entid)){
		if($eacid) {
			$sql = "UPDATE 
					pdeescola.eacescolaacessivel 
					SET 
					 docid = ".$docid." 
					WHERE
					 eacid = ".$eacid;	

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

function eaPegarEstadoAtual( $docid ) {
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

function acessoCadEsc( $entid ){
	global $db; 
	
	if( $entid ){
		$sql = "SELECT
					e.*
				FROM
					pdeescola.eacescolaquestionario e
				INNER JOIN entidade.entidade ent on ent.entcodent = e.entcodent
				WHERE
					ent.entid = ".$entid;
		
		$dados = $db->pegaLinha( $sql );
		return $dados;
		
	} else {
		return false;
	}
}
?>