<?php

/**
 * Verifica se as variáveis de sessão usadas no
 * Escola Aberta estão devidamente setadas.
 * 
 * @return void
 */
function eabVerificaSessao()
{
	/*** Se alguma das variáveis de sessão estiver nula ou vazia ***/ 
	if( empty($_SESSION['eabid']) || is_null($_SESSION['eabid']) || empty($_SESSION['entid']) || is_null($_SESSION['entid']) )
	{
		echo '<script>
		
				/*** Exibe o alerta de erro ***/
				alert("Ocorreu um erro interno.\n
				       O sistema irá redirecioná-lo à página inicial do módulo.");
				       
				/*** Redireciona o usuário ***/
				location.href = "pdeescola.php?modulo=inicio&acao=C";
				
			  </script>';
		die();
	}
}

/**
 * Recupera a escola, estado ou município
 * atribuído ao perfil do usuário no Escola Aberta
 * 
 * @param string $resp
 * @return mixed
 * @author Felipe Carvalho
 */
function eabRecuperaResponsabilidadePerfil($resp)
{
	global $db;

	$sql = "SELECT
				".$resp."
			FROM
				pdeescola.usuarioresponsabilidade
			WHERE
				usucpf = '".$_SESSION["usucpf"]."' 
				AND rpustatus = 'A'
				AND pflcod in (".PDEESC_PERFIL_CAD_ESCOLA_ABERTA.",
							   ".PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA.",
							   ".PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA.")";
	return $db->pegaUm($sql);
}

/**
 * Função que monta as abas do 'Escola Aberta'
 * @return array
 */
function carregaAbasEscolaAberta() {
	global $db;
	
	if(!$_SESSION['entid']){
		$menu = array(
				  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=eablista&acao=E"),
				  );
	} else {
		if($_SESSION['bo_cadastrador_escola_aberta']) {
			$menu = array(
					  0 => array("id" => 1, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/dados_escola&acao=A"),
					  1 => array("id" => 2, "descricao" => "Dados do Diretor", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
					  2 => array("id" => 3, "descricao" => "Dados Programa ".$_SESSION["exercicio"]."", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/dados_programa&acao=A"),
					  3 => array("id" => 4, "descricao" => "Equipe Local", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/equipe_local&acao=A")
					  );
		} else {
			$menu = array(
					  0 => array("id" => 1, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=eablista&acao=E"),
					  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/dados_escola&acao=A"),
					  2 => array("id" => 3, "descricao" => "Dados do Diretor", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
					  3 => array("id" => 4, "descricao" => "Dados Programa ".$_SESSION["exercicio"]."", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/dados_programa&acao=A"),
					  4 => array("id" => 5, "descricao" => "Equipe Local", "link" => "/pdeescola/pdeescola.php?modulo=eabprincipal/equipe_local&acao=A")
					  );
		}
	}
		
	$menu = $menu ? $menu : array();
	  
	return $menu;
}

/**
 * Monta a lista de escolas do Escola Aberta
 * em conformidade com os filtros
 * 
 * @return void
 */
function eablista()
{
	global $db;
	
	$ano = $_SESSION["exercicio"];
	
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
			$naoIniciado = "eab.docid is null and";
	} 

	if ($_POST['tpcid'])
		$where[] = " e.tpcid IN (".$_POST['tpcid'].")";	
	//else
		//$where[] = " e.tpcid IN (1,3)";		
		
	
	if( $_REQUEST['modalidade'] == 'F') {
		$where[] = " eab.eabmodalidadeensino = 'F' ";
	}
	else if( $_REQUEST['modalidade'] == 'M') {
		$where[] = " eab.eabmodalidadeensino = 'M' ";
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
    	if ( (in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA, $perfil)) && in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA, $perfil)){
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
		} elseif ( in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA, $perfil)) { //Perfil PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL só ver na sua escola ESTADUAL
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)";
		} elseif ( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA, $perfil)) { //Perfil PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL só ver na sua escola MUNICIPAL
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
			 				 '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaEAB(\'eabajax.php\', \'tipo=redirecionaeab&entid=' || e.entid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
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
							 CASE WHEN eab.eabmodalidadeensino = 'M' THEN 'Médio' ELSE 'Fundamental' END as ensino
						FROM
							 entidade.entidade e
						INNER JOIN 
							 entidade.endereco endi ON endi.entid = e.entid
						LEFT JOIN 
							 territorios.municipio m ON m.muncod = endi.muncod
							 %s
						INNER JOIN
							 pdeescola.eabescolaaberta eab ON %s eab.entid = e.entid AND eab.eabanoreferencia = ".$_SESSION["exercicio"]." AND eab.eabstatus = 'A'
						LEFT JOIN 
							 workflow.documento d ON d.docid = eab.docid
						LEFT JOIN 
							 workflow.estadodocumento est ON est.esdid = d.esdid
							 %s %s ) as foo 
						%s",
				$from,
				$naoIniciado,
				$where ? " WHERE ".implode(' AND ', $where)." " : ' ',
				$and,
				$where1 ? $where1 : '');
				
				//dbg($sql,1);

	$cabecalho = array( "Ação", "Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Ensino");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
}

function eabMaxProgramacaoExercicio() {
	/*** Temporário, enquanto não existem escolas em 2011... ***/
	return 2011;
	
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
 * Função para montar o cabeçalho usado nas páginas do 'Escola Aberta'
 * 
 * @return string
 */
function cabecalhoEscolaAberta() {

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
			  	--ent.tpcid IN (1,3) AND
		    	ent.entid IN ('{$entid}')";
	//dbg($sql);
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
 * Função para verificar se existe o Diretor ou Coordenador para a entidade escolhida
 * 
 * @return string
 */
function eabExisteDiretorCoordenadorPorCpf($funid){ // Função feita para atender necessidade do Cliente com urgência
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
				window.location='pdeescola.php?modulo=eablista&acao=E';
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

/*** WorkFlow ***/
function eabPegarDocid( $entid , $eabid ) {
	global $db;
	
	$entid = (integer) $entid;
	$eabid = (integer) $eabid;
	
	$sql = "SELECT
			 docid
			FROM
			 pdeescola.eabescolaaberta
			WHERE
			 entid  = " . $entid . " AND 
			 eabid = " . $eabid . " AND 
			 eabstatus = 'A'";
	return (integer) $db->pegaUm( $sql );
}

function eabCriarDocumento( $entid, $eabid ) {
	global $db;
	
	$docid = eabPegarDocid($entid, $eabid);
	
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
		$tpdid = TPDID_ESCOLA_ABERTA;
		
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
		
		$docdsc = "Cadastramento Escola Aberta - " . $descricao;
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );	
		
		//if ($memid = pegarMemid($entid)){
		if($eabid) {
			$sql = "UPDATE 
					pdeescola.eabescolaaberta
					SET 
					 docid = ".$docid." 
					WHERE
					 eabid = ".$eabid;	

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

function eabPegarEstadoAtual( $docid ) {
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

function funcaomestre($eabid) {
	$retorno1 = condicaoCadastramento($eabid);
	$retorno2 = validaAtividadeAtual($eabid);
	
	if($retorno1 && $retorno2) 
	return true;
	else 
	return false;
}


function condicaoCadastramento($eabid)
{
	require_once APPRAIZ . 'includes/classes/entidades.class.inc';
	
	global $db;
	$retorno = false;
	
	$sql = "SELECT
				 count(e.eatid) as ct,
				 e1.ct1,
				 e2.ct2,
				 e3.ct3,
				 e4.ct4
			FROM
				pdeescola.eabatividade e
			LEFT JOIN
				(SELECT	count(eatid) as ct1, eabid FROM pdeescola.eabatividade WHERE eatid = 1 group by eabid) e1 ON e1.eabid = e.eabid
			INNER JOIN
				(SELECT	count(eatid) as ct2, eabid FROM pdeescola.eabatividade WHERE eatid = 2 group by eabid) e2 ON e2.eabid = e.eabid
			INNER JOIN
				(SELECT	count(eatid) as ct3, eabid FROM pdeescola.eabatividade WHERE eatid = 3 group by eabid) e3 ON e3.eabid = e.eabid
			INNER JOIN
				(SELECT	count(eatid) as ct4, eabid FROM pdeescola.eabatividade WHERE eatid = 4 group by eabid) e4 ON e4.eabid = e.eabid	
			WHERE
				e.eabid = ".$_SESSION["eabid"]."
			group by e1.ct1,e2.ct2,e3.ct3,e4.ct4";
	$dados = $db->pegaLinha($sql);	
	
	if($dados['ct1'] > 0 && $dados['ct2'] > 0 && $dados['ct3'] > 0 && $dados['ct4'] > 0 ) 
		$retorno = true;

	
	return $retorno;

}

function validaAtividadeAtual($eabid)
{
	global $db;
	$retorno = false;
	
	//Execução (oficineiro)
	$sql = "SELECT
				eaeid 
			FROM
				pdeescola.eabequipelocal
			WHERE
				eabid = $eabid
				and eaeid = 3 --Execução (oficineiro)
			";
	$eaeid = $db->pegaUm($sql);
	
	
	//"Coordenação" ou "Organização Pedagógica"
	$sql = "SELECT
				eaeid 
			FROM
				pdeescola.eabequipelocal
			WHERE
				eabid = $eabid
				and eaeid in (1,2) --Coordenação ou Organização Pedagógica
			";
	$eaeid2 = $db->pegaUm($sql);
	
	
	if($eaeid && $eaeid2){
		return true;  
	}
	else
	return false;
}

	
?>