<?php
/*******************
 *
 * FUNÇÕES DE SEGURANÇA
 *
 *******************/


include_once APPRAIZ . "includes/classes/dateTime.inc";

/**************
 * Função que retorna os perfi(S) do usuário que lhe permite acessar à demanda.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @return perfil (array)
 *
 **************/
function arrayPerfilDemanda() {
	global $db;

	$dmdid  	= $_SESSION['dmdid'];
	$usucpf 	= $_SESSION['usucpf'];
	$perfilUser	= arrayPerfil();

	if (!$dmdid):
	die('<script>
				alert("Problemas com a sessão.\nAcesse novamente!");
				location.href = "?modulo=principal/lista&acao=A";
			 </script>');
	endif;

	$sql = "(	SELECT
					ur.pflcod
				FROM
					demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
					INNER JOIN demandas.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
																	  (ur.ordid = od.ordid OR ur.sidid = d.sidid)
				WHERE
					ur.usucpf = '".$usucpf."' AND
					d.dmdid = ".$dmdid."
			)UNION ALL(
				SELECT
					'".DEMANDA_PERFIL_DEMANDANTE."' AS pflcod
				FROM
					demandas.demanda d
					INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = d.usucpfdemandante					 
				WHERE
					d.usucpfdemandante = '".$usucpf."' AND
					pu.pflcod = ".DEMANDA_PERFIL_DEMANDANTE." AND
					d.dmdid = ".$dmdid."
					
			)				
				";
	$perfil = (array) $db->carregarColuna($sql);

	// Atribui a lista de perfis, caso exista, o perfil "SUPER USUÁRIO"
	if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_SUPERUSUARIO);
	}

	// Atribui a lista de perfis, caso exista, o perfil "CONSULTA GERAL"
	if ( in_array(DEMANDA_PERFIL_CONSULTA_GERAL, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_CONSULTA_GERAL);
	}

	// Atribui a lista de perfis, caso exista, o perfil "TECNICO 1º NIVEL"
	if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_TECNICO1);
	}

	// Atribui a lista de perfis, caso exista, o perfil "TECNICO 2º NIVEL"
	if ( in_array(DEMANDA_PERFIL_TECNICO, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_TECNICO);
	}

	// Atribui a lista de perfis, caso exista, o perfil "ANALISTA DE SISTEMAS"
	if ( in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_ANALISTA_SISTEMA);
	}

	// Atribui a lista de perfis, caso exista, o perfil "ANALISTA DE TESTE"
	if ( in_array(DEMANDA_PERFIL_ANALISTA_TESTE, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_ANALISTA_TESTE);
	}
	
	// Atribui a lista de perfis, caso exista, o perfil "ANALISTA DE SISTEMAS"
	if ( in_array(DEMANDA_PERFIL_ANALISTA_FNDE, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_ANALISTA_FNDE);
	}
	
	// Atribui a lista de perfis, caso exista, o perfil "GERENTE DE PROJETOS"
	if ( in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_GERENTE_PROJETO);
	}

	// Atribui a lista de perfis, caso exista, o perfil "GERENTE DE PROJETOS"
	if ( in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_PROGRAMADOR);
	}

	// Atribui a lista de perfis, caso exista, o perfil "GERENTE DE PROJETOS"
	if ( in_array(DEMANDA_PERFIL_DEMANDANTE, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_DEMANDANTE);
	}

	if ( in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_DEMANDANTE_AVANCADO);
	}
	
	if ( in_array(DEMANDA_PERFIL_DEPOSITO_DTI, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_DEPOSITO_DTI);
	}
	
	// Atribui a lista de perfis, caso exista, o perfil "GERENTE DE PROJETOS"
	if ( in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_ADMINISTRADOR);
	}

	// Atribui a lista de perfis, caso exista, o perfil "GERENTE DE PROJETOS"
	if ( in_array(DEMANDA_PERFIL_ADM_REDES, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_ADM_REDES);
	}

	// Atribui a lista de perfis, caso exista, o perfil "GERENTE DE PROJETOS"
	if ( in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_GESTOR_REDES);
	}
	
	if ( in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_FISCAL_TECNICO_FSW);
	}
	
	if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_GERENTE_FSW);
	}
	
	if ( in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_ANALISTA_FSW);
	}
	
	if ( in_array(DEMANDA_PERFIL_GESTOR_EQUIPE, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_GESTOR_EQUIPE);
	}
	
	if ( in_array(DEMANDA_PERFIL_EQUIPE, $perfilUser) ) {
		array_push($perfil,(string) DEMANDA_PERFIL_EQUIPE);
	}
	
	if ( !$perfil && !in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfilUser) && !in_array(DEMANDA_PERFIL_CONSULTA_GERAL, $perfilUser) && !in_array(DEMANDA_PERFIL_TECNICO1, $perfilUser) && !in_array(DEMANDA_PERFIL_TECNICO, $perfilUser) && !in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfilUser) && !in_array(DEMANDA_PERFIL_ANALISTA_FNDE, $perfilUser) && !in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfilUser) && !in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfilUser) && !in_array(DEMANDA_PERFIL_DEMANDANTE, $perfilUser) && !in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO, $perfilUser) && !in_array(DEMANDA_PERFIL_DEPOSITO_DTI, $perfilUser) && !in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfilUser) && !in_array(DEMANDA_PERFIL_ADM_REDES, $perfilUser) && !in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfilUser) && !in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfilUser) && !in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfilUser) && !in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfilUser) && !in_array(DEMANDA_PERFIL_ANALISTA_TESTE, $perfilUser) && !in_array(DEMANDA_PERFIL_EQUIPE, $perfilUser) && !in_array(DEMANDA_PERFIL_GESTOR_EQUIPE, $perfilUser) ){
		die('<script>
				alert("Seu perfil não lhe permite acesso!");
				history.go(-1);
			 </script>');
	}

	return $perfil;
}

/**************
 * Função que valida se o estado(s) passado no @param corresponde ao da demanda,
 * se coincidir redireciona para página anterior, caso não, function return.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $estDoc (array) OU (integer)
 * @return Direciona para página anterior ou return;
 *
 **************/
function estadoBloqPag($estDoc = null){
	global $db;

	$estDoc = (array) $estDoc;
	$dmdid  = $_SESSION['dmdid'];

	if (!$estDoc):
	die('<script>
				history.go(-1);
			 </script>');
	endif;

	$sql = "SELECT
				dmdid
			FROM
				demandas.demanda d
				INNER JOIN workflow.documento dc ON dc.docid = d.docid and dc.tpdid in (31,35)
			WHERE
				d.dmdid = {$dmdid} AND
				dc.esdid IN (".implode(',', $estDoc).");";

	if ( $db->pegaUm($sql) ){
		die('<script>
				alert("Não é possível acessar esta funcionalidade!\nDevido ao estado da demanda no fluxo de trabalho.");
				history.go(-1);	
			 </script>');
	}
	return;
}

/*******************
 *
 * FIM FUNÇÕES DE SEGURANÇA
 *
 *******************/

/******************
 * Pega array com perfis
 ******************/
function arrayPerfil(){
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 44
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
	$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

/****************************
 * RECUPERA NOME DO USUÁRIO
 * Caso não seja passado o parametro "$cpf"
 * Carregará o cpf em sessão
 ****************************/
function nomeUser($cpf=null){
	global $db;

	$cpf = $cpf ? $cpf : $_SESSION['usucpf'];
	$sql = sprintf("SELECT
					 usunome
					FROM
					 seguranca.usuario
					WHERE
					 usucpf = '%s'",$cpf);
	return $db->pegaUm($sql);
}


function verificaEditaDemanda(){
	global $db;


	//Pega array com os perfis do usuário que podem acessar a demanda.
	$perfilDem = arrayPerfil();


	if (!$_SESSION['dmdid']){
		if ( in_array(DEMANDA_PERFIL_DEMANDANTE,$perfilDem) || in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO,$perfilDem) ){
			echo "<script>window.location.href=\"demandas.php?modulo=principal/painelDemandante&acao=A\";</script>";
		}
		else{
			echo "<script>window.location.href=\"demandas.php?modulo=principal/lista&acao=A\";</script>";
		}
		exit;
	}


	if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilDem) || in_array(DEMANDA_PERFIL_ADMINISTRADOR,$perfilDem) ){
		return true;
	}



	if ( in_array(DEMANDA_PERFIL_TECNICO1,$perfilDem) || in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfilDem) || in_array(DEMANDA_PERFIL_DBA,$perfilDem) || in_array(DEMANDA_PERFIL_ADM_REDES,$perfilDem) || in_array(DEMANDA_PERFIL_GESTOR_REDES,$perfilDem) || in_array(DEMANDA_PERFIL_GERENTE_PROJETO,$perfilDem) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA,$perfilDem) || in_array(DEMANDA_PERFIL_ANALISTA_TESTE,$perfilDem) || in_array(DEMANDA_PERFIL_ANALISTA_WEB,$perfilDem) || in_array(DEMANDA_PERFIL_GERENTE_FSW,$perfilDem) || in_array(DEMANDA_PERFIL_ANALISTA_FSW,$perfilDem) || in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW,$perfilDem) || in_array(DEMANDA_PERFIL_GESTOR_EQUIPE,$perfilDem) || in_array(DEMANDA_PERFIL_EQUIPE,$perfilDem) ){

		//verifica origem e celula
		$sql = "SELECT t.ordid, 
					   CASE WHEN t.ordid in (2,13,14,23,24) THEN
					    		t.celid
					    	ELSE
					   			s.celid
					   	END as celid
		 		FROM demandas.demanda d
				LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid	 			
				LEFT JOIN demandas.sistemacelula s ON s.sidid = d.sidid
		 		WHERE d.dmdid=".$_SESSION['dmdid'];
		
		$dados = $db->pegaLinha($sql);

		if($dados['celid']) $flagcel = " OR celid = ".$dados['celid'];

		if($dados['ordid']){
			$sql = "SELECT distinct usucpf
					FROM demandas.usuarioresponsabilidade
					WHERE rpustatus = 'A' 
					AND usucpf = '".$_SESSION['usucpf']."'
					AND (ordid = ".$dados['ordid']."
					$flagcel ) ";
					
			$dados2 = $db->pegaUm($sql);

			if($dados2){
				return true;
			}
			else{
				return false;
			}

		}
		else{
			//se origem for null
			return true;
		}
	}

	//verifica se é demandante ou tecnico executor
	$sql = "SELECT dmdid
	 		FROM demandas.demanda 
 			WHERE dmdid=".$_SESSION['dmdid']."
			AND (usucpfdemandante='".$_SESSION['usucpf']."'
	 		OR usucpfanalise='".$_SESSION['usucpf']."'
	 		OR usucpfexecutor='".$_SESSION['usucpf']."'
	 		)";
	$dmdid = $db->pegaUm($sql);

	if($dmdid){
		return true;
	}

	return false;


}

/*****************************
 * MONTA LISTA DE DEMANDAS
 *****************************/
function lista($dmdidorigem = null, $flag_sublista = null){
	//function lista($dmdidorigem = null){
	global $db;

	$codigo   = $_POST['codigo'];
	$situacao = $_POST['esdid'];
	$origem   = $_POST['ordid'];
	$origemarr  = $_POST['ordidarr'];
	$tiposervico  = $_POST['tiposervico'];
	$setor    = $_POST['unaid'];
	$dataini  = $_POST['dataini'];
	$datafim  = $_POST['datafim'];
	$celula   = $_POST['celid'];
	$sistema  = $_POST['sidid'];
	$motid	  = $_POST['motid'];
	$anexo	  = $_POST['anexo'];
	$palavra_chave = $_POST['palavra_chave'];
	$nserie = $_POST['nserie'];
	$npatrimonio = $_POST['npatrimonio'];
	$solicitante = $_POST['solicitante'];
	$responsavel = $_POST['responsavel'];
	$nao_exibir = $_POST['nao_exib_finaliz_cancela_invalid'];
	$somente_avaliadas = $_POST['somente_avaliadas'];
	$somente_pausadas = $_POST['somente_pausadas'];
	$dmdnumdocrastreamento = $_POST['dmdnumdocrastreamento'];
	$dmddatadocrastreamento = $_POST['dmddatadocrastreamento'];
	$dmdunidadedocrastreamento = $_POST['dmdunidadedocrastreamento'];
	$anosAnteriores = $_POST['anosAnteriores'];
	


	//Consulta Rápida
	($_POST['consultaRapida'] == "nova")? $nova = true : '';
	($_POST['consultaRapida'] == "minhademanda")? $minha = true : '';
	($_POST['consultaRapida'] == "todas")? $todas = true : '';
	($_POST['consultaRapida'] == "hoje")? $hoje = true : '';
	($_POST['consultaRapida'] == "ematraso")? $ematraso = true : '';
	($_POST['consultaRapida'] == "vencehoje")? $vencehoje = true : '';
	($_POST['consultaRapida'] == "emanalise")? $emanalise = true : '';
	($_POST['consultaRapida'] == "ematendimento")? $ematendimento = true : '';
	($_POST['consultaRapida'] == "agvalidacao")? $agvalidacao = true : '';
	($_POST['consultaRapida'] == "agvalidacaodem")? $agvalidacaodem = true : '';
	//($_POST['consultaRapida'] == "agavaliacao")? $agavaliacao = true : '';
	($_POST['consultaRapida'] == "finalizada")? $finalizada = true : '';
	($_POST['consultaRapida'] == "auditada")? $auditada = true : '';
	($_POST['consultaRapida'] == "agpago")? $agpago = true : '';
	($_POST['consultaRapida'] == "pago")? $pago = true : '';
	($_POST['consultaRapida'] == "invalidada")? $invalidada = true : '';
	($_POST['consultaRapida'] == "cancelada")? $cancelada = true : '';
	($_POST['consultaRapida'] == "emprocessamento")? $emprocessamento = true : '';
	($_POST['consultaRapida'] == "urgente")? $urgente = true : '';
	($_POST['consultaRapida'] == "alta")? $alta = true : '';
	($_POST['consultaRapida'] == "normal")? $normal = true : '';
	($_POST['consultaRapida'] == "ndas")? $ndas = true : '';



	$from	  = array();
	$where    = array();
	$where1   = array();
	$perfil   = arrayPerfil();


	/******************
	 * Validação por perfil
	 ******************/

	if ( !in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) && !in_array(DEMANDA_PERFIL_CONSULTA_GERAL, $perfil) ){

		if ( in_array(DEMANDA_PERFIL_DEMANDANTE, $perfil) && count($perfil)==1){
			$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
		}
		
		/*
		if ( in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO, $perfil) && count($perfil)==1){
			$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
		}
		*/
		
		if ( in_array(DEMANDA_PERFIL_DEPOSITO_DTI, $perfil)){
			$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
		}		

		if ( in_array(DEMANDA_PERFIL_TECNICO, $perfil) || in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil) || in_array(DEMANDA_PERFIL_ADM_REDES, $perfil) || in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO, $perfil) || in_array(DEMANDA_PERFIL_EQUIPE, $perfil) ){
			
			if ( !in_array(DEMANDA_PERFIL_ADM_REDES, $perfil) ){

				$from[] = "LEFT JOIN demandas.sistemadetalhe sd2 ON sd2.sidid = d.sidid
						   LEFT JOIN demandas.usuarioresponsabilidade ur2 ON (ur2.sidid = sd2.sidid OR ur2.ordid = od.ordid OR ur2.celid = t.celid) AND
					   													  ur2.rpustatus = 'A' AND
					   													  ur2.usucpf = '".$_SESSION['usucpf']."' AND
																		  ur2.pflcod IN (".DEMANDA_PERFIL_TECNICO.",
																		 			     ".DEMANDA_PERFIL_PROGRAMADOR.",
																		 			     ".DEMANDA_PERFIL_EQUIPE.",
																	 			     	 ".DEMANDA_PERFIL_DEMANDANTE_AVANCADO.")";
				
				//if(in_array(DEMANDA_PERFIL_TECNICO, $perfil)){ atende somente uma origem
				//	$where1[] = "(d.usucpfexecutor = '{$_SESSION['usucpf']}' AND ur2.usucpf = d.usucpfexecutor)";
				//}
				//else{
				$where1[] = "d.usucpfexecutor = '{$_SESSION['usucpf']}'";
				//}
					
				$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
			}
			else{
				
				if(!$flag_sublista){
					$from[] = "LEFT JOIN demandas.sistemadetalhe sd2 ON sd2.sidid = d.sidid
							   INNER JOIN demandas.usuarioresponsabilidade ur2 ON (ur2.sidid = sd2.sidid OR ur2.ordid = od.ordid OR ur2.celid = t.celid) AND
					   													  ur2.rpustatus = 'A' AND
					   													  ur2.usucpf = '".$_SESSION['usucpf']."' AND
																		  ur2.pflcod IN (".DEMANDA_PERFIL_ADM_REDES.")";
				}
				else{
					$from[] = "LEFT JOIN demandas.sistemadetalhe sd2 ON sd2.sidid = d.sidid
							   LEFT JOIN demandas.usuarioresponsabilidade ur2 ON (ur2.sidid = sd2.sidid OR ur2.ordid = od.ordid OR ur2.celid = t.celid) AND
					   													  ur2.rpustatus = 'A' AND
					   													  ur2.usucpf = '".$_SESSION['usucpf']."' AND
																		  ur2.pflcod IN (".DEMANDA_PERFIL_ADM_REDES.")";
				}
				
			}
		}

		if ( in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) || in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ){
			if (!$minha){

				$from[] = "INNER JOIN demandas.usuarioresponsabilidade ur ON ( (ur.ordid = od.ordid OR od.ordid IS NULL) AND
																		  ur.rpustatus = 'A' AND	
																		  ur.usucpf = '".$_SESSION['usucpf']."' AND	
																		  ur.pflcod IN (".DEMANDA_PERFIL_ADMINISTRADOR.",
																		 			    ".DEMANDA_PERFIL_TECNICO1."
																		 			    )
																		 ) ";
			}
			else{
				$from[] = "INNER JOIN demandas.usuarioresponsabilidade ur ON ( (ur.rpustatus = 'A' OR od.ordid IS NULL) AND
																		  ur.usucpf = '".$_SESSION['usucpf']."' AND	
																		  ur.pflcod IN (".DEMANDA_PERFIL_ADMINISTRADOR.",
																		 			    ".DEMANDA_PERFIL_TECNICO1."
																		 			    )
																		 ) ";

			}

		}

		if ( in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){
			if (!$minha){

				$from[] = "INNER JOIN demandas.usuarioresponsabilidade ur3 ON ( (ur3.ordid = od.ordid OR od.ordid IS NULL) AND
																		  ur3.rpustatus = 'A' AND	
																		  ur3.usucpf = '".$_SESSION['usucpf']."' AND	
																		  ur3.pflcod IN (".DEMANDA_PERFIL_GESTOR_MEC.")
																		 ) ";
			}
			else{
				$from[] = "INNER JOIN demandas.usuarioresponsabilidade ur3 ON ( (ur3.rpustatus = 'A' OR od.ordid IS NULL) AND
																		  ur3.usucpf = '".$_SESSION['usucpf']."' AND	
																		  ur3.pflcod IN (".DEMANDA_PERFIL_GESTOR_MEC.")
																		 ) ";

			}

		}


		if ( in_array(DEMANDA_PERFIL_GESTOR_REDES, $perfil) ){
			$from[] = "LEFT JOIN demandas.usuarioresponsabilidade ur7 ON ur7.celid = t.celid AND
					   													 ur7.rpustatus = 'A' AND
					   													 ur7.usucpf = '".$_SESSION['usucpf']."' AND
																		 ur7.pflcod IN (".DEMANDA_PERFIL_GESTOR_REDES.")";
				
			$where1[] = "t.celid in (select distinct ur8.celid
									 from demandas.usuarioresponsabilidade ur8 
									 inner join demandas.tiposervico t8 on t8.celid = ur8.celid
									 where t8.ordid=2 and ur8.rpustatus = 'A' and ur8.usucpf='{$_SESSION['usucpf']}'
									)";
				
				
			$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
		}
		
		
		if ( in_array(DEMANDA_PERFIL_GESTOR_EQUIPE, $perfil) ){
			$from[] = "LEFT JOIN demandas.usuarioresponsabilidade ur9 ON ur9.celid = t.celid AND
					   													 ur9.rpustatus = 'A' AND
					   													 ur9.usucpf = '".$_SESSION['usucpf']."' AND
																		 ur9.pflcod IN (".DEMANDA_PERFIL_GESTOR_EQUIPE.")";
				
			$where1[] = "t.celid in (select distinct ur10.celid
									 from demandas.usuarioresponsabilidade ur10 
									 inner join demandas.tiposervico t8 on t8.celid = ur10.celid
									 where t8.ordid=23 and ur10.rpustatus = 'A' and ur10.usucpf='{$_SESSION['usucpf']}'
									)";
				
				
			$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
			
		}

		if ( in_array(DEMANDA_PERFIL_DBA, $perfil)  ){
			$from[] = "INNER JOIN demandas.usuarioresponsabilidade ur ON (ur.ordid = od.ordid AND
																		  ur.rpustatus = 'A' AND	
																		  ur.usucpf = '".$_SESSION['usucpf']."' AND	
																		  ur.pflcod IN (".DEMANDA_PERFIL_DBA.")
																		 ) --or ( d.usucpfdemandante = '{$_SESSION['usucpf']}' )
																		 ";
				
			//$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";

		}

		if ( in_array(DEMANDA_PERFIL_ANALISTA_WEB, $perfil)  ){
			$from[] = "INNER JOIN demandas.usuarioresponsabilidade ur ON (ur.ordid = od.ordid AND
																		  ur.rpustatus = 'A' AND	
																		  ur.usucpf = '".$_SESSION['usucpf']."' AND	
																		  ur.pflcod IN (".DEMANDA_PERFIL_ANALISTA_WEB.")
																		 ) --or ( d.usucpfdemandante = '{$_SESSION['usucpf']}' )
																		 ";
				
			// $where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";

		}


		if ( in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_FNDE, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_TESTE, $perfil) ){
			$from[] = "LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
					   LEFT JOIN demandas.usuarioresponsabilidade ur1 ON ur1.sidid = sd.sidid AND
					   													 ur1.rpustatus = 'A' AND
					   													 ur1.usucpf = '".$_SESSION['usucpf']."' AND
																		 ur1.pflcod IN (".DEMANDA_PERFIL_ANALISTA_SISTEMA.",".DEMANDA_PERFIL_ANALISTA_FNDE.",".DEMANDA_PERFIL_ANALISTA_TESTE.")";
			
			if(!$flag_sublista){
				//$where1[] = "d.sidid IS NOT NULL";
				$where1[] = "d.sidid in (select distinct sc4.sidid
										from demandas.sistemacelula sc4 
										inner JOIN demandas.usuarioresponsabilidade ur5 ON ur5.celid = sc4.celid 
										where ur5.rpustatus = 'A' and ur5.usucpf='{$_SESSION['usucpf']}'
										)";
				
				$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
			}
				
			
		}
		
		
		if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfil) || in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) ){
			
			$ordidx = $db->carregarColuna("SELECT distinct ordid from demandas.usuarioresponsabilidade where rpustatus='A' and ordid is not null and usucpf = '".$_SESSION['usucpf']."'");
			$celidx = $db->carregarColuna("SELECT distinct celid from demandas.usuarioresponsabilidade where rpustatus='A' and celid is not null and usucpf = '".$_SESSION['usucpf']."'");
			$sididx = $db->carregarColuna("SELECT distinct sidid from demandas.usuarioresponsabilidade where rpustatus='A' and sidid is not null and usucpf = '".$_SESSION['usucpf']."'");
			
			if($ordidx[0]){
				$ordidx = implode(',',$ordidx);
				$where[] = "od.ordid IN (".$ordidx.")";
			}
			
			if($celidx[0]){
				$celidx = implode(',',$celidx);
				$where[] = "d.celid IN (".$celidx.")";
			}
			
			if($sididx[0]){
				$sididx = implode(',',$sididx);
				$where[] = "d.sidid IN (".$sididx.")";
			}
			
			/*
			$from[] = "INNER JOIN demandas.usuarioresponsabilidade ur9 ON ur9.rpustatus = 'A' AND
					   													 ur9.usucpf = '".$_SESSION['usucpf']."' AND
																		 ur9.pflcod IN (".DEMANDA_PERFIL_GERENTE_FSW.",".DEMANDA_PERFIL_ANALISTA_FSW.") AND
																		 (ur9.ordid = od.ordid OR ur9.celid = d.celid OR ur9.sidid = d.sidid)";
			*/
			if(!$flag_sublista){
				//$where1[] = "d.sidid IS NOT NULL";
				/*
				$where1[] = "d.sidid in (select distinct sc5.sidid
										from demandas.sistemacelula sc5 
										inner JOIN demandas.usuarioresponsabilidade ur10 ON ur10.celid = sc5.celid 
										where ur10.rpustatus = 'A' and ur10.usucpf='{$_SESSION['usucpf']}'
										)";
				*/
				//$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
			}
				
			
		}

		if ( in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) ){
			$from[] = "LEFT JOIN demandas.sistemacelula sc3 ON sc3.sidid = d.sidid
					   LEFT JOIN demandas.usuarioresponsabilidade ur4 ON ur4.celid = sc3.celid AND
					   													 ur4.rpustatus = 'A' AND
					   													 ur4.usucpf = '".$_SESSION['usucpf']."' AND
																		 ur4.pflcod IN (".DEMANDA_PERFIL_GERENTE_PROJETO.")";
			
			if(!$flag_sublista){
				//$where1[] = "d.sidid IS NOT NULL";
				$where1[] = "d.sidid in (select distinct sc5.sidid
										from demandas.sistemacelula sc5 
										inner JOIN demandas.usuarioresponsabilidade ur6 ON ur6.celid = sc5.celid 
										where ur6.rpustatus = 'A' and ur6.usucpf='{$_SESSION['usucpf']}'
										)";
						
				$where1[] = "d.usucpfdemandante = '{$_SESSION['usucpf']}'";
			}
			
		}


		if ($where1){
			$where[] = "(".implode(' OR ', $where1).")";
		}
		//dbg($from,1);
	}
	/******************
	 * Fim validação por perfil
	 *****************/

	if(!$anosAnteriores && !$codigo){
		$where[] = "to_char(d.dmddatainclusao,'YYYY') in ('2014','2015')";
	}
	
	 
	if ($dmdidorigem){
		$where[] = "d.dmdidorigem = {$dmdidorigem}";
	}
	/*
	 else{
		if ( !in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) && !in_array(DEMANDA_PERFIL_TECNICO, $perfil) && !in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil) && !$codigo ){
		$where[] = "d.dmdidorigem IS NULL";
		}
		}
		*/


	if ($codigo){
		$codigo = (int) str_replace("'","",str_replace(" ","",str_replace("#","",$codigo)));
		$where[] = "d.dmdid = {$codigo}";
	}

	if ($situacao){
		if($situacao=='EP'){
			$where[] = "doc.esdid is null";
		}
		elseif($situacao=='EA'){
			$where[] = " ( doc.esdid is null OR doc.esdid in (91,107,92,108) ) ";
		}
		else{
			$where[] = "doc.esdid in ({$situacao})";
		}
	}

	if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ){

		if($tiposervico[0] != "") {
			if($_REQUEST["tiposervico"] != "1")
			$where[] = "( d.tipid in ('".implode("','", $tiposervico)."') )";
			else
			$where[] = "( d.tipid not in ('".implode("','", $tiposervico)."') )";
		}

	}

	if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_TECNICO, $perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ){

		if($origemarr[0] != "") {
			if($_REQUEST["ordidarr"] != "1" && $_REQUEST["ordidarr"] != "18" && $_REQUEST["ordidarr"] != "19" && $_REQUEST["ordidarr"] != "20" && $_REQUEST["ordidarr"] != "21")
			$where[] = "( od.ordid in ('".implode("','", $origemarr)."') )";
			else
			$where[] = "( od.ordid not in ('".implode("','", $origemarr)."') )";
		}

	}
	else{
		if ($origem){
			$where[] = "od.ordid = {$origem}";
		}
	}


	if ($setor){
		$where[] = "d.unaid = {$setor}";
	}

	if ($motid){
		$where[] = "d.motid = {$motid}";
	}

	if ($dataini){
		$xDatai	= explode("/", $dataini);
		$xDatai2 = $xDatai[2]."-".$xDatai[1]."-".$xDatai[0];
		$where[] = "d.dmddatainclusao >= '{$xDatai2} 00:00:00'";
	}

	if ($datafim){
		$xDataf	= explode("/", $datafim);
		$xDataf2 = $xDataf[2]."-".$xDataf[1]."-".$xDataf[0];
		$where[] = "d.dmddatainclusao <= '{$xDataf2} 23:59:59'";
	}

	if ($celula && !$sistema){
		$where[] = "(d.sidid in (select sidid from demandas.sistemacelula where celid=".$celula.") or d.celid = ".$celula.")";
	}

	if ($sistema){
		$where[] = "d.sidid = {$sistema}";
	}

	if ($palavra_chave){
		$palavra_chave = pg_escape_string($palavra_chave);
		$where[] = " (d.dmdtitulo ILIKE '%{$palavra_chave}%' OR d.dmddsc ILIKE '%{$palavra_chave}%')";
	}

	if($responsavel[0] != "") {
		if($_REQUEST["responsavel_campo_excludente"] != "1")
		$where[] = "( d.usucpfexecutor in ('".implode("','", $responsavel)."') )";
		else
		$where[] = "( d.usucpfexecutor not in ('".implode("','", $responsavel)."') )";
	}

	/*
	if($solicitante[0] != "") {
		if($_REQUEST["solicitante_campo_excludente"] != "1")
		$where[] = "( d.usucpfdemandante in ('".implode("','", $solicitante)."') )";
		else
		$where[] = "( d.usucpfdemandante not in ('".implode("','", $solicitante)."') )";
	}
	*/

	if ($solicitante){
		$solicitante = pg_escape_string($solicitante);
		$where[] = " ( public.removeacento(u.usunome) ILIKE public.removeacento('%{$solicitante}%') ) ";
	}

	if ($dmdnumdocrastreamento){
		$where[] = "d.dmdnumdocrastreamento = '$dmdnumdocrastreamento'";
	}

	if ($dmdunidadedocrastreamento){
		$where[] = "d.dmdunidadedocrastreamento = $dmdunidadedocrastreamento";
	}

	if ($dmddatadocrastreamento){
		$where[] = "to_char(d.dmddatadocrastreamento, 'DD/MM/YYYY') = '$dmddatadocrastreamento'";
	}

	if ($anexo){
		switch($anexo){
			case "true":
				$where[] = " d.dmdid in (
									SELECT dd.dmdid
										FROM demandas.demanda dd
									RIGHT JOIN demandas.anexos danx ON dd.dmdid = danx.dmdid)";
				break;
			case "false":
				$where[] = " d.dmdid not in (
									SELECT dd.dmdid
										FROM demandas.demanda dd
									RIGHT JOIN demandas.anexos danx ON dd.dmdid = danx.dmdid)";
				break;
			default :
		}
	}
	
	if ($nserie){
		$nserie = pg_escape_string($nserie);
		$where[] = " d.dmdid in (
							SELECT di.dmdid
								FROM demandas.demanda di
							RIGHT JOIN demandas.itemhardwaredemanda itw ON di.dmdid = itw.dmdid and itw.ihdnumserie ilike '%$nserie%')";
	}	

	if ($npatrimonio){
		$npatrimonio = pg_escape_string($npatrimonio);
		$where[] = " d.dmdid in (
							SELECT di.dmdid
								FROM demandas.demanda di
							RIGHT JOIN demandas.itemhardwaredemanda itw ON di.dmdid = itw.dmdid and itw.ihdnumpatrimonio ilike '%$npatrimonio%')";
	}	
	
	
	//filtra Consulta Rápida
	if ($nova){
		$where[] = "( d.docid IS NULL or doc.esdid in (".DEMANDA_ESTADO_EM_ANALISE.",".DEMANDA_GENERICO_ESTADO_EM_ANALISE.") )";
	}

	if ($minha){
		//$where[] = "u.usucpf = '{$_SESSION['usucpf']}'";
		//$where[] = "( d.usucpfexecutor = '".$_SESSION['usucpf']."' or d.usucpfdemandante = '".$_SESSION['usucpf']."' or d.usucpfanalise = '".$_SESSION['usucpf']."' )";
		$where[] = "( d.usucpfexecutor = '".$_SESSION['usucpf']."' or d.usucpfdemandante = '".$_SESSION['usucpf']."' )";
	}

	if ($ematraso){
		$where[] = "d.dmddatafimprevatendimento < CURRENT_DATE";
		$where[] = "doc.esdid not in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.")";
	}

	if ($vencehoje){
		$where[] = "to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')";
		$where[] = "doc.esdid not in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.")";
	}

	if ($emanalise){
		$where[] = "doc.esdid in (".DEMANDA_ESTADO_EM_ANALISE.",".DEMANDA_GENERICO_ESTADO_EM_ANALISE.")";
	}
	if ($ematendimento){
		$where[] = "doc.esdid in (".DEMANDA_ESTADO_EM_ATENDIMENTO.",".DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO.")";
	}
	if ($agvalidacao){
		$where[] = "doc.esdid in (".DEMANDA_ESTADO_AGUARDANDO_VALIDACAO.",".DEMANDA_GENERICO_ESTADO_AGUARDANDO_VALIDACAO.")";
	}
	if ($agvalidacaodem){
		$where[] = "doc.esdid in (".DEMANDA_GENERICO_ESTADO_AGUARDANDO_VALIDACAO_DEMANDANTE.")";
	}
	/*
	 if ($agavaliacao){
		$where[] = "doc.esdid in (".DEMANDA_ESTADO_AGUARDANDO_AVALIACAO.",".DEMANDA_GENERICO_ESTADO_AGUARDANDO_AVALIACAO.")";
		}
		*/
	if ($finalizada){
		$where[] = "doc.esdid in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_VALIDADA_FORA_PRAZO.")";
	}
	if ($auditada){
		$where[] = "doc.esdid in (".DEMANDA_GENERICO_ESTADO_AUDITADO.",".DEMANDA_ESTADO_AUDITADO.")";
	}
	if ($agpago){
		$where[] = "doc.esdid in (".DEMANDA_GENERICO_ESTADO_AGUARDANDO_PAGAMENTO.")";
	}
	if ($pago){
		$where[] = "doc.esdid in (".DEMANDA_GENERICO_ESTADO_PAGO.")";
	}
	if ($invalidada){
		$where[] = "doc.esdid in (".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.")";
	}
	if ($cancelada){
		$where[] = "doc.esdid in (".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.")";
	}
	if ($emprocessamento){
		$where[] = "d.docid IS NULL";
	}

	if ($urgente){
		$where[] = "d.priid in (3)";
	}
	if ($alta){
		$where[] = "d.priid in (2)";
	}
	if ($normal){
		$where[] = "d.priid in (1)";
	}
	if ($ndas){
		$where[] = "d.priid IS NULL";
	}

	if($somente_pausadas == "on") {
		//$where[] = " dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) ";
		$where[] = " ( select count(pp.dmdid) 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid) > 0 
					  AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
					  OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) ";
	}

	$flag_avaliacao = "LEFT";
	if($somente_avaliadas == "on") $flag_avaliacao = "INNER";



	$flagtodas = 1;
	if($todas || $minha || $nova || $hoje || $ematraso || $vencehoje
	|| $emanalise || $ematendimento || $agvalidacao || $agvalidacaodem
	|| $finalizada || $auditada || $agpago || $pago || $invalidada || $cancelada || $emprocessamento
	|| $urgente || $alta || $normal || $ndas
	|| $dmdidorigem || $codigo || $nserie || $npatrimonio) $flagtodas = 2;
	if($flagtodas == 1){
		if (!$situacao){
			if($nao_exibir != "on") {
				//checkbox pesquisa avançada
				//$where[] = "ed.esddsc not in ('Finalizada','Cancelada')";
				//if ( !in_array(DEMANDA_PERFIL_ADM_REDES, $perfil) ){
					$where[] = "( doc.esdid not in (".DEMANDA_GENERICO_ESTADO_AUDITADO.",".DEMANDA_ESTADO_AUDITADO.",".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.",".DEMANDA_ESTADO_VALIDADA_FORA_PRAZO.") or doc.esdid is null )";
				//}
				//else{
				//	$where[] = "( doc.esdid not in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.",".DEMANDA_ESTADO_VALIDADA_FORA_PRAZO.") )";
				//}
			}
		}
	}





	/*
	 * Carrega array com perfis do usuário
	 */
	//$perfil = arrayPerfil();

	/*Perfis que são Coordenadores de Sistemas*/
	/*
	 $sql = "select distinct(usucpf) from demandas.usuarioresponsabilidade where rpustatus = 'A' and ordid = 1";
	 $coord_sistemas = array();
	 $coord_sistemas = $db->carregar($sql);
	 */

	$idzeroesquerda = "	lpad(cast(d.dmdid as varchar),
									case when length(cast(d.dmdid as varchar)) > 6 then 
											length(cast(d.dmdid as varchar)) 
								   	else 
								   		6 
								   	end 
									, '0') as id, ";

	/*Colunas específicas para cada Perfil*/
	if(in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil) || in_array(DEMANDA_PERFIL_CONSULTA_GERAL, $perfil)){
		$cabecalho = array("Cód","Prioridade","Assunto","Origem","Situação","Solicitante","Técnico Responsável","Data Abertura","Data prevista de término","Data de conclusão");
		$colspan_filhos = count($cabecalho);
		$busca = "SELECT
					 DISTINCT
					 --'<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdid || '</a>' AS id,
					 --d.dmdid as id,
					 $idzeroesquerda
					(CASE p.priid
					    WHEN 3 THEN '<img src=\'../imagens/pd_urgente.JPG\' />'|| ' ' || p.pridsc
					    WHEN 1 THEN '<img src=\'../imagens/pd_normal.JPG\' />'|| ' ' || p.pridsc
					    WHEN 2 THEN '<img src=\'../imagens/pd_alta.JPG\' />'|| ' ' || p.pridsc
					    ELSE '<div style=\'color:red;\' title=\'Não Atribuído\'>N/A</div>'
					 END) AS prioridade,

					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid) > 0 
	 					AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
	 					OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
	 					THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END
					 ||					 	
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
					 	  	'<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 	ELSE
					 	  	'<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 END) AS tit,
					 
					 orddescricao AS origemdemanda,
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
					  
					  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
					  	ELSE  esddsc
					  END
					  
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 -- t.tipdescricao,
					 CASE 
					  	WHEN d.dmdnomedemandante != '' THEN '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(d.dmdnomedemandante) || '</a>'
					  	ELSE '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(u.usunome) || '</a>'
					  END as usuario,
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS responsavel,
					 to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') AS datainclusao,
					 
			 		(CASE esddsc 
			 			WHEN 'Validada' THEN
				 			'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
			 			WHEN 'Validada Sem Pausa' THEN
				 			'<font title=\"Demanda Validada Sem Pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Finalizada' THEN
				 			'<font title=\"Demanda Finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Auditada' THEN
				 			'<font title=\"Demanda Auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Aguardando Pagamento' THEN
				 			'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Pago' THEN
				 			'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Invalidada' THEN
				 			'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Aguardando validação' THEN
				 			'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 	 ELSE
				 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
							(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
					 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
					 			ELSE 
									(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE THEN 
												'<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							 			  ELSE 
							 			  		'<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							 		END)						 		
					 		END)						 		
				 		END)
				 	 END) AS dataprevisaotermino,
				 	 
					 CASE 
					 	--WHEN dm.contador > 0
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 
					 	THEN
					 		(CASE WHEN esddsc in ('Validada','Aguardando validação','Finalizada','Auditada','Pago','Aguardando Pagamento','Validada sem pausa','Invalidada') THEN
								 	to_char(htddata::timestamp,'DD-MM-YYYY HH24:MI:SS') || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
							ELSE
								'</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
						 	END)
					 ELSE
					 		(CASE WHEN esddsc in ('Aguardando validação','Finalizada','Auditada','Pago','Aguardando Pagamento','Invalidada','Validada','Validada sem pausa') THEN
								 	to_char(htddata::timestamp,'DD-MM-YYYY HH24:MI:SS')
						 	END)
					 END AS dataconclusao
					 --d.dmddatainclusao,
					 --doc.esdid";
	}
	elseif ( in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil)){
		$cabecalho = array("Cód","Prioridade","Assunto","Descrição","Sistema","Origem","Situação","Solicitante","Técnico Responsável","Data de Abertura","Data prevista de início","Data prevista de término","Data de conclusão");
		$colspan_filhos = count($cabecalho);
		$busca = "SELECT
					 DISTINCT
					 --'<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdid || '</a>' AS id,
					 $idzeroesquerda
					(CASE p.priid
					    WHEN 3 THEN '<img src=\'../imagens/pd_urgente.JPG\' />'|| ' ' || p.pridsc
					    WHEN 1 THEN '<img src=\'../imagens/pd_normal.JPG\' />'|| ' ' || p.pridsc
					    WHEN 2 THEN '<img src=\'../imagens/pd_alta.JPG\' />'|| ' ' || p.pridsc
					    ELSE '<div style=\'color:red;\' title=\'Não Atribuído\'>N/A</div>'
					 END) AS prioridade,
					 
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid) > 0 
	 					AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
	 					OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
	 					THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END
					 ||					 	
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN 
					 	  	'<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 	ELSE
					 	  	'<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 END) AS tit,
					 CASE WHEN esddsc in ('Auditada','Pago','Aguardando Pagamento','Finalizada','Validada','Validada sem pausa','Invalidada','Aguardando validação') THEN
					 		dmddsc
					 	ELSE
					 		(CASE WHEN
					 			d.dmddatafimprevatendimento < CURRENT_DATE
					 		 THEN '<font color=\"red\">' || dmddsc || '</font>'
					 		 ELSE '<font color=\"green\">' || dmddsc || '</font>'
					 		 END)
					 END as descricao,
					  sis.siddescricao AS sistema,
					  orddescricao AS origemdemanda,
					  
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
					  
					  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
					  	ELSE  esddsc
					  END
					  
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 -- t.tipdescricao,
					 CASE 
					  	WHEN d.dmdnomedemandante != '' THEN '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(d.dmdnomedemandante) || '</a>'
					  	ELSE '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(u.usunome) || '</a>'
					  END as usuario,
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS responsavel,
					 to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') AS datainclusao,
					 to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dataprevisaoinicio,
			 		(CASE esddsc
			 			WHEN 'Validada' THEN
				 			'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
			 			WHEN 'Validada sem pausa' THEN
			 				'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>' 
			 			WHEN 'Invalidada' THEN
				 			'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
			 			WHEN 'Finalizada' THEN
				 			'<font title=\"Demanda finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Auditada' THEN
				 			'<font title=\"Demanda auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Aguardando Pagamento' THEN
				 			'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Pago' THEN
				 			'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 		WHEN 'Aguardando validação' THEN	
				 			'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
				 	 ELSE
				 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
							(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
					 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
					 			ELSE 
									(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
							 			THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							 			ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							 		END)						 		
					 		END)						 		
				 		END)
				 	 END) AS dataprevisaotermino,
					 CASE 
					 	--WHEN dm.contador > 0
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0
					 	THEN
					 		(CASE WHEN esddsc in ('Aguardando validação','Finalizada','Auditada','Pago','Aguardando Pagamento','Validada','Validada sem pausa','Invalidada') THEN
								 		to_char(htddata::timestamp,'DD-MM-YYYY HH24:MI:SS') || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'					 		 	
							ELSE
								'</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
						 	END)
					 ELSE
					 		(CASE WHEN esddsc in ('Aguardando validação','Validada','Validada sem pausa','Invalidada','Finalizada','Auditada','Pago','Aguardando Pagamento') THEN
										 to_char(htddata::timestamp,'DD-MM-YYYY HH24:MI:SS')
						 	END)
					 END AS dataconclusao
					 --d.dmddatainclusao,
					 --doc.esdid";
	}
	/*
	 elseif ( in_array(DEMANDA_PERFIL_COORDENADOR, $perfil) && in_array($_SESSION['usucpf'],$coord_sistemas[0])){
		$cabecalho = array("Cód","Prioridade","Assunto","Célula","Sistema","Origem","Situação","Solicitante","Ténico Responsável","Data de Abertura","Data prevista de término","Data de conclusão");
		$colspan_filhos = count($cabecalho);
		$busca = "SELECT
		DISTINCT
		--'<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdid || '</a>' AS id,
		$idzeroesquerda
		(CASE p.priid
		WHEN 3 THEN '<img src=\'../imagens/pd_urgente.JPG\' />'|| ' ' || p.pridsc
		WHEN 1 THEN '<img src=\'../imagens/pd_normal.JPG\' />'|| ' ' || p.pridsc
		WHEN 2 THEN '<img src=\'../imagens/pd_alta.JPG\' />'|| ' ' || p.pridsc
		ELSE '<div style=\'color:red;\' title=\'Não Atribuído\'>N/A</div>'
		END) AS prioridade,

		(CASE
		WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
		'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
		ELSE
		''
		END
		||
		CASE
		WHEN dm.contador > 0 THEN
		'<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
		ELSE
		'<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
		END) AS tit,

		CASE
		WHEN celnome IS NULL THEN '<span style=\'color:red;\'>N/A</span>'
		ELSE  celnome
		END as celula,
		sis.siddescricao AS sistema,
		orddescricao AS origemdemanda,
		CASE
		WHEN d.docid IS NOT NULL THEN
			
		CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
		ELSE  esddsc
		END
			
		ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
		END AS situacao,
		-- t.tipdescricao,
		CASE
		WHEN u.usunome != '' THEN '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(u.usunome) || '</a>'
		ELSE '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(d.dmdnomedemandante) || '</a>'
		END as usuario,
		CASE
		WHEN u2.usucpf != '' THEN upper(u2.usunome)
		ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
		END AS responsavel,
		to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') AS datainclusao,
		(CASE esddsc
		WHEN 'Finalizada' THEN
		'<font title=\"Demanda finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
		WHEN 'Aguardando validação' THEN
		'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
		ELSE
		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
		(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
		THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>'
		ELSE
		(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
		THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>'
		ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>'
		END)
		END)
		END)
		END) AS dataprevisaotermino,
		CASE
		WHEN dm.contador > 0
		THEN
		(CASE esddsc
		WHEN 'Aguardando validação' THEN
		datahist || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
		WHEN 'Finalizada' THEN
		datahist || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
		WHEN 'Aguardando avaliação' THEN
		datahist || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
		ELSE
		'</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
		END)
		ELSE
		(CASE esddsc
		WHEN 'Aguardando validação' THEN
		datahist
		WHEN 'Finalizada' THEN
		datahist
		WHEN 'Aguardando avaliação' THEN
		datahist
		END)
		END AS dataconclusao
		--d.dmddatainclusao,
		--doc.esdid";
		}*/
	elseif ( in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_TESTE, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_FNDE, $perfil) || in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil) || in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) || in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfil) ){
		$cabecalho = array("Cód","Prioridade","Assunto","Descrição da Atividade","Sistema","Situação","Solicitante","Técnico Responsável","Data de Abertura","Data prevista de início","Data prevista de término");
		$colspan_filhos = count($cabecalho);
		$busca = "SELECT
					 DISTINCT
					 --'<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdid || '</a>' AS id,
					 --d.dmdid AS id,
					 $idzeroesquerda
					(CASE p.priid
					    WHEN 3 THEN '<img src=\'../imagens/pd_urgente.JPG\' />'|| ' ' || p.pridsc
					    WHEN 1 THEN '<img src=\'../imagens/pd_normal.JPG\' />'|| ' ' || p.pridsc
					    WHEN 2 THEN '<img src=\'../imagens/pd_alta.JPG\' />'|| ' ' || p.pridsc
					    ELSE '<div style=\'color:red;\' title=\'Não Atribuído\'>N/A</div>'
					 END) AS prioridade,
					 
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid) > 0 
	 					AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
	 					OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
	 					THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END
					 ||					 	
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
					 	  	'<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 	ELSE
					 	  	'<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 END) AS tit,
					 
					 CASE WHEN esddsc in ('Auditada','Pago','Aguardando Pagamento','Finalizada','Validada','Validada sem pausa','Invalidada','Aguardando validação') THEN
					 		dmddsc
					 	ELSE
					 		(CASE WHEN
					 			d.dmddatafimprevatendimento < CURRENT_DATE
					 		 THEN '<font color=\"red\">' || dmddsc || '</font>'
					 		 ELSE '<font color=\"green\">' || dmddsc || '</font>'
					 		 END)
					 END as descricao,
					 sis.siddescricao AS sistema,
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
					  
					  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
					  	ELSE  esddsc
					  END
					  
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 CASE 
					  	WHEN d.dmdnomedemandante != '' THEN '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(d.dmdnomedemandante) || '</a>'
					  	ELSE '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(u.usunome) || '</a>'
					  END as usuario,
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS responsavel,					 
					 -- t.tipdescricao,
					 CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS responsavel,
					  '<span style=\"display:none\">' || d.dmddatainclusao || '</span>' ||
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
					 	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') 
					 	ELSE 
					 	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS')
					 END AS datainclusao,
					 to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dataprevisaoinicio,
					 CASE 
					 	--WHEN dm.contador > 0
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 
					 	THEN
					 		(CASE esddsc 
					 			WHEN 'Validada' THEN
					 		 		'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Validada sem pausa' THEN
					 		 		'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Invalidada' THEN
					 		 		'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Finalizada' THEN
					 		 		'<font title=\"Demanda finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Auditada' THEN
					 		 		'<font title=\"Demanda Auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Aguardando Pagamento' THEN
					 		 		'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Pago' THEN
					 		 		'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 			WHEN 'Aguardando validação' THEN
					 		 		'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 ELSE
						 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
									(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
							 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
							 			ELSE 
											(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
									 			THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
									 			ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
									 		END)						 		
							 		END)						 		
						 		ELSE '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
						 		END)
						 	 END)
					 	ELSE
					 		(CASE esddsc 
					 			WHEN 'Validada' THEN
						 			'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Validada sem pausa' THEN
						 			'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Invalidada' THEN
						 			'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Finalizada' THEN
						 			'<font title=\"Demanda Finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Auditada' THEN
						 			'<font title=\"Demanda Auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Aguardando Pagamento' THEN
						 			'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Pago' THEN
						 			'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
					 			WHEN 'Aguardando validação' THEN
						 			'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 	 ELSE
						 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
									(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
							 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							 			ELSE 
											(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
									 			THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
									 			ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
									 		END)						 		
							 		END)						 		
						 		END)
						 	 END)
					 END AS dataprevisaotermino
					 --d.dmddatainclusao,
					 --doc.esdid,";
	}
	elseif ( in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) ){
		$cabecalho = array("Cód","Prioridade","Assunto","Origem","Descrição da Atividade","Situação","Solicitante","Técnico Responsável","Serviço Executado","Avaliação","Histórico Pausa","Nº Série / Nº Patr.","Data de Abertura","Data prevista de início","Data prevista de término");
		$colspan_filhos = count($cabecalho);
		$busca = "SELECT
					 DISTINCT
					 --'<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdid || '</a>' AS id,
					 $idzeroesquerda
					(CASE p.priid
					    WHEN 3 THEN '<img src=\'../imagens/pd_urgente.JPG\' />'|| ' ' || p.pridsc
					    WHEN 1 THEN '<img src=\'../imagens/pd_normal.JPG\' />'|| ' ' || p.pridsc
					    WHEN 2 THEN '<img src=\'../imagens/pd_alta.JPG\' />'|| ' ' || p.pridsc
					    ELSE '<div style=\'color:red;\' title=\'Não Atribuído\'>N/A</div>'
					 END) AS prioridade,
					 
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid) > 0 
	 					AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
	 					OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
	 					THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END
					 ||					 	
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
					 	  	'<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 	ELSE
					 	  	'<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 END) AS tit,
					 
					 orddescricao AS origemdemanda,
					 
					 CASE WHEN esddsc in ('Finalizada','Auditada','Pago','Aguardando Pagamento','Validada','Validada sem pausa','Invalidada','Aguardando validação') THEN
					 		 dmddsc
					 	ELSE
					 		(CASE WHEN
					 			d.dmddatafimprevatendimento < CURRENT_DATE
					 		 THEN '<font color=\"red\">' || dmddsc || '</font>'
					 		 ELSE '<font color=\"green\">' || dmddsc || '</font>'
					 		 END)
					 END as descricao,
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
					  
					  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
					  	ELSE  esddsc
					  END
					  
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 -- t.tipdescricao,
					 CASE 
					  	WHEN d.dmdnomedemandante != '' THEN '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(d.dmdnomedemandante) || '</a>'
					  	ELSE '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(u.usunome) || '</a>'
					  END as usuario,
					 CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					 END AS responsavel,
					 
					 CASE 
					  	--WHEN servicoexec <> '' THEN	'<center><img style=\'cursor:hand;\' src=\'/imagens/report.gif\' title=\'\' onmouseover=\"return escape(\'' || translate(translate(translate(servicoexec, chr(10)||chr(13), ' '), chr(34), ' '), chr(39), ' ') || '\');\" ></center>'				 
					  	WHEN cmddsc <> '' THEN	'<center><a href=\'javascript:void(0);\'><img style=\'cursor:hand;\' src=\'/imagens/report.gif\' border=0 title=\'' || translate(translate(translate(cmddsc, chr(10)||chr(13), ' '), chr(34), ' '), chr(39), ' ') || '\'  ></a></center>'
					 END AS servicoexec2,
					 
					 CASE
					 	WHEN (select count(dmdid) as ctavaliacao from demandas.avaliacaodemanda where dmdid=d.dmdid) > 0 THEN '<CENTER><a href=\'javascript:void(0);\'><img src=\'../imagens/report.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidAvaliacaoAjax='|| d.dmdid ||'\',this);\"></a></CENTER>' 
					 END AS avaliacao,

					 CASE
					 	--WHEN dmdidpausahist > 0 THEN '<CENTER><a href=\'javascript:void(0);\'><img src=\'../imagens/report.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a></CENTER>'
					 	WHEN ( select count(dmdid) from demandas.pausademanda where pdmstatus = 'A' and dmdid = d.dmdid ) > 0 THEN '<CENTER><a href=\'javascript:void(0);\'><img src=\'../imagens/report.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a></CENTER>'
					 END AS histpausa,

					 CASE
					 	--WHEN ctnseriepatr > 0 THEN '<CENTER><a href=\'javascript:void(0);\'><img src=\'../imagens/report.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidNseriepatrAjax='|| d.dmdid ||'\',this);\"></a></CENTER>'
					 	WHEN ( select count(ihdid) from demandas.itemhardwaredemanda where dmdid = d.dmdid ) > 0 THEN '<CENTER><a href=\'javascript:void(0);\'><img src=\'../imagens/report.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidNseriepatrAjax='|| d.dmdid ||'\',this);\"></a></CENTER>'
					 	 
					 END AS nserie,
					 
					  '<span style=\"display:none\">' || d.dmddatainclusao || '</span>' ||
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
					 	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') 
					 	ELSE 
					 	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS')
					 END AS datainclusao,
					 to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dataprevisaoinicio,					 
					 CASE 
					 	--WHEN dm.contador > 0
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 
					 	THEN
					 		(CASE esddsc 
					 			WHEN 'Finalizada' THEN
					 		 		'<font title=\"Demanda Finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Auditada' THEN
					 		 		'<font title=\"Demanda Auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Aguardando Pagamento' THEN
					 		 		'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Pago' THEN
					 		 		'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Validada' THEN
					 		 		'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Validada sem pausa' THEN
					 		 		'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Invalidada' THEN
					 		 		'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 			WHEN 'Aguardando validação' THEN
					 		 		'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 ELSE
						 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
									(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
							 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
							 			ELSE 
											(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
									 			THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
									 			ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
									 		END)						 		
							 		END)						 		
						 		ELSE '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
						 		END)
						 	 END)
					 	ELSE
					 		(CASE esddsc 
					 			WHEN 'Finalizada' THEN
						 			'<font title=\"Demanda Finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Auditada' THEN
						 			'<font title=\"Demanda Auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Aguardando Pagamento' THEN
						 			'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Pago' THEN
						 			'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Validada' THEN
						 			'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Validada sem pausa' THEN
						 			'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Invalidada' THEN
						 			'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
					 			WHEN 'Aguardando validação' THEN
						 			'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 	 ELSE
						 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
									(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
							 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							 			ELSE 
											(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
									 			THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
									 			ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
									 		END)						 		
							 		END)						 		
						 		END)
						 	 END)
					 END AS dataprevisaotermino
					 --d.dmddatainclusao,
					 --doc.esdid";
	}
	else{
		$cabecalho = array("Cód","Prioridade","Assunto","Origem","Célula","Situação","Solicitante","Técnico Responsável","Data Abertura","Data prevista de término");
		$colspan_filhos = count($cabecalho);
		$busca = "SELECT
					 DISTINCT
					 --'<a style=\'color:#0066CC\'  href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdid || '</a>' AS id,
					 $idzeroesquerda
					(CASE p.priid
					    WHEN 3 THEN '<img src=\'../imagens/pd_urgente.JPG\' />'|| ' ' || p.pridsc
					    WHEN 1 THEN '<img src=\'../imagens/pd_normal.JPG\' />'|| ' ' || p.pridsc
					    WHEN 2 THEN '<img src=\'../imagens/pd_alta.JPG\' />'|| ' ' || p.pridsc
					    ELSE '<div style=\'color:red;\' title=\'Não Atribuído\'>N/A</div>'
					 END) AS prioridade,
					 
					 (CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid) > 0 
	 					AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
	 					OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108) and pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
	 					THEN
							'<a href=\'javascript:void(0);\'><img src=\'../imagens/pause.gif\' border=0  title=\' \' align=\'absmiddle\'  onmouseout=\'SuperTitleOff(this);\' onmousemove=\"SuperTitleAjax(\'demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax='|| d.dmdid ||'\',this);\"></a>'
						ELSE
							''	
					 END
					 ||					 	
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
					 	  	'<a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| d.dmdid ||')\'><img id=\'img_mais_'|| d.dmdid ||'\' src=\'../imagens/mais.gif\' border=\'0\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| d.dmdid ||')\'><img id=\'img_menos_'|| d.dmdid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'display:none\'></a> ' || '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 	ELSE
					 	  	'<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/lista&acao=A&dmdid=' || d.dmdid || '\'>' || d.dmdtitulo || '</a>'
					 END) AS tit,
					 					 
					 orddescricao AS origemdemanda,
					 celnome AS celula,
					 CASE
					  WHEN d.docid IS NOT NULL THEN 
					  
					  CASE esddsc WHEN 'Cancelada' THEN '<span style=\'color:red;\'>' || esddsc || '</span>'
					  	ELSE  esddsc
					  END
					  
					  ELSE '<span style=\'color:blue;\' title=\'Em Processamento\'>Em Processamento</span>'
					 END AS situacao,
					 -- t.tipdescricao,
					 CASE 
					  	WHEN d.dmdnomedemandante != '' THEN '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(d.dmdnomedemandante) || '</a>'
					  	ELSE '<a style=\'color:#0066CC\' href=\'demandas.php?modulo=principal/dadosSolicitante&acao=A&dmdid=' || d.dmdid || '\'>' || upper(u.usunome) || '</a>'
					  END as usuario,
					  CASE
					  	WHEN u2.usucpf != '' THEN upper(u2.usunome)
					  	ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>N/A</span>'
					  END AS responsavel,
					'<span style=\"display:none\">' || d.dmddatainclusao || '</span>' ||
					 CASE 
					 	--WHEN dm.contador > 0 THEN
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 THEN
					 	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS') 
					 	ELSE 
					 	  to_char(d.dmddatainclusao, 'DD-MM-YYYY HH24:MI:SS')
					 END AS datainclusao,
					 --to_char(d.dmddatainiprevatendimento, 'DD-MM-YYYY HH24:MI:SS') AS dataprevisaoinicio,
					 CASE 
					 	--WHEN dm.contador > 0
					 	WHEN ( select count(dmdid) from demandas.demanda where dmdidorigem = d.dmdid ) > 0 
					 	THEN
					 		(CASE esddsc 
					 			WHEN 'Finalizada' THEN
					 		 		'<font title=\"Demanda Finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Auditada' THEN
					 		 		'<font title=\"Demanda Auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Aguardando Pagamento' THEN
					 		 		'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Pago' THEN
					 		 		'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Validada' THEN
					 		 		'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Validada sem pausa' THEN
					 		 		'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 	WHEN 'Invalidada' THEN
					 		 		'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 			WHEN 'Aguardando validação' THEN
					 		 		'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font> </tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
					 		 ELSE
						 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
									(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
							 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
							 			ELSE 
											(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
									 			THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
									 			ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
									 		END)						 		
							 		END)						 		
						 		ELSE '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=$colspan_filhos style=\"padding-left:20px;\" id=\"td_' || d.dmdid || '\" ></td></tr>'
						 		END)
						 	 END)
					 	ELSE
					 		(CASE esddsc 
					 			WHEN 'Finalizada' THEN
						 			'<font title=\"Demanda Finalizada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Auditada' THEN
						 			'<font title=\"Demanda Auditada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Aguardando Pagamento' THEN
						 			'<font title=\"Demanda Aguardando Pagamento!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Pago' THEN
						 			'<font title=\"Demanda Pago!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Validada' THEN
						 			'<font title=\"Demanda Validada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Validada sem pausa' THEN
						 			'<font title=\"Demanda Validada sem pausa!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 		WHEN 'Invalidada' THEN
						 			'<font title=\"Demanda Invalidada!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
					 			WHEN 'Aguardando validação' THEN
						 			'<font title=\"Demanda Aguardando validação!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '<font>'
						 	 ELSE
						 		(CASE WHEN d.dmddatafimprevatendimento is not null THEN
									(CASE WHEN to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
							 			THEN '<font color=\"#FBB917\" title=\"Demanda com vencimento hoje!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
							 			ELSE 
											(CASE WHEN d.dmddatafimprevatendimento < CURRENT_DATE
									 			THEN '<font color=\"red\" title=\"Demanda em atraso!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
									 			ELSE '<font color=\"green\" title=\"Demanda em dia!\">' || to_char(d.dmddatafimprevatendimento, 'DD-MM-YYYY HH24:MI:SS') || '</font>' 
									 		END)						 		
							 		END)						 		
						 		END)
						 	 END)
					 END AS dataprevisaotermino
					 --d.dmddatainclusao,
					 --doc.esdid";
	}

	$where = array_unique($where);
	
	if ( in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) || in_array(DEMANDA_PERFIL_TECNICO1, $perfil) || in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) || in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) || in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) || in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfil) ){
		
		$sql = sprintf("$busca
						 
						FROM
						 demandas.demanda d
						 --LEFT JOIN ( select dmdid, count(ihdid) as ctnseriepatr from demandas.itemhardwaredemanda group by dmdid ) nsp ON nsp.dmdid = d.dmdid
						 --LEFT JOIN ( select dmdidorigem,  count(dmdid) as contador from demandas.demanda group by dmdidorigem ) dm ON dm.dmdidorigem = d.dmdid
						 --LEFT JOIN (  select dmdid, count(pdmid) as contapausa from demandas.pausademanda where pdmdatafimpausa is null group by dmdid ) ps ON ps.dmdid = d.dmdid
						 /*
						 --pega subdemandas com pausa
						 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
	 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108)
									 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
						*/
						 --pega historico da demanda
						 --LEFT JOIN ( select dmdid, count(dmdid) as dmdidpausahist from demandas.pausademanda where pdmstatus = 'A' group by dmdid ) phst ON phst.dmdid = d.dmdid			 
						 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
						 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
						 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
						 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sidid = sis.sidid
						 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid or cel.celid = d.celid
						 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
						 LEFT JOIN demandas.anexos anx ON anx.dmdid = d.dmdid
						 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
						 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid --and ed.esdid in (93,95,109,111,135,136,170)
						 LEFT JOIN workflow.historicodocumento a ON a.hstid = doc.hstid --and a.aedid in(146, 191)
						 LEFT JOIN workflow.comentariodocumento b on a.hstid = b.hstid --recupera htddata e cmddsc as servicoexec
						 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
						 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
						 -- $flag_avaliacao JOIN (select dmdid,  count(dmdid) as ctavaliacao from demandas.avaliacaodemanda group by dmdid) avd ON avd.dmdid = d.dmdid
						 /*
						 LEFT JOIN (  (select a.docid, max(b.cmdid) as cmdid, to_char(max(htddata)::timestamp,'DD-MM-YYYY HH24:MI:SS') as datahist
									 		from 	workflow.historicodocumento a
									 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
									 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
									 	where aedid in (146, 191) and c.esdid in (93,95,109,111,135,136,170)
									  group by a.docid ) ) as hst ON hst.docid = d.docid
						 */	
						 /*
						 LEFT JOIN (  (select cmdid, cmddsc as servicoexec
									 		from 	workflow.comentariodocumento 
									   ) ) as hst2 ON hst2.cmdid = hst.cmdid  					
						 */			  
						 %s 					  
						WHERE 
						 dmdstatus = 'A'	
						 %s		 
						ORDER BY id DESC",
						 implode(' ', $from),
						 ($where ? " AND ".implode(' AND ', $where) : ' ') );
						 
	}else{
	
		$sql = sprintf("$busca
					 
					FROM
					 demandas.demanda d
					 --LEFT JOIN ( select dmdid, count(ihdid) as ctnseriepatr from demandas.itemhardwaredemanda group by dmdid ) nsp ON nsp.dmdid = d.dmdid
					 --LEFT JOIN ( select dmdidorigem,  count(dmdid) as contador from demandas.demanda group by dmdidorigem ) dm ON dm.dmdidorigem = d.dmdid
					 --LEFT JOIN (  select dmdid, count(pdmid) as contapausa from demandas.pausademanda where pdmdatafimpausa is null group by dmdid ) ps ON ps.dmdid = d.dmdid
					 /*
					 --pega subdemandas com pausa
					 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid and doc2.tpdid in (31,35)
 								 where pp.pdmstatus = 'A' and doc2.esdid in (91,92,107,108)
								 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid
					 */
					 --pega historico da demanda
					 --LEFT JOIN ( select dmdid, count(dmdid) as dmdidpausahist from demandas.pausademanda where pdmstatus = 'A' group by dmdid ) phst ON phst.dmdid = d.dmdid			 
					 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
					 LEFT JOIN demandas.sistemadetalhe sis ON sis.sidid = d.sidid
					 LEFT JOIN demandas.sistemacelula sis_c ON sis_c.sidid = sis.sidid
					 LEFT JOIN demandas.celula cel ON cel.celid = sis_c.celid or cel.celid = d.celid
					 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
					 LEFT JOIN demandas.anexos anx ON anx.dmdid = d.dmdid
					 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
					 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid --and ed.esdid in (93,95,109,111,135,136,170)
					 LEFT JOIN workflow.historicodocumento a ON a.hstid = doc.hstid --and a.aedid in(146, 191) 
        			 LEFT JOIN workflow.comentariodocumento b on a.hstid = b.hstid --recupera htddata
					 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
					 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
					 -- $flag_avaliacao JOIN (select dmdid,  count(dmdid) as ctavaliacao from demandas.avaliacaodemanda group by dmdid) avd ON avd.dmdid = d.dmdid
					 /*
					 LEFT JOIN (  (select a.docid, max(b.cmdid) as cmdid, to_char(max(htddata)::timestamp,'DD-MM-YYYY HH24:MI:SS') as datahist
								 		from 	workflow.historicodocumento a
								 			inner join workflow.comentariodocumento b on a.hstid = b.hstid
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111,135,136,170)
								  group by a.docid ) ) as hst ON hst.docid = d.docid	
					 */
					 --LEFT JOIN (  (select cmdid, cmddsc as servicoexec
					 --			 		from 	workflow.comentariodocumento 
					 --			   ) ) as hst2 ON hst2.cmdid = hst.cmdid  					
								  
					 %s 					  
					WHERE 
					 dmdstatus = 'A'	
					 %s		 
					ORDER BY id DESC",
					 implode(' ', $from),
					 ($where ? " AND ".implode(' AND ', $where) : ' ') );
					 
	}
	
	//dbg($sql,1);
	return array("sql" => $sql, "cabecalho" => $cabecalho);
	
}


function montaSubLista($dmdid){
	global $db;

	$sql = lista($dmdid,$flag_sublista = 1);
	$db->monta_lista_simples( $sql['sql'], $sql['cabecalho'], 50, 10, 'N', '', '' );
}


/**************
 * Função que retorna diferença entre a data do @param e a atual
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $data1 (date) "####-##-## OU ##-##-#### OU ####/##/## OU ##/##/####"
 * @return date (text) "## Dia(s) ## Hora(s) ## Minuto(s) OU ## Hora(s) ## Minuto(s)"
 *
 **************/
/*function dataDecorrida($data1=null){

if (!$data1){
return;
}

$data1 = substr($data1, 0, 16);

$ch1  = strpos($data1,"-") ? '-' : '/';
$loc1 = strrpos($data1, $ch1);

if ($loc1 == 5){
list($dia1, $mes1, $ano1)  = explode($ch1, $data1);
list($ano1, $horMin)	   = explode(" ", $ano1);
list($hora1, $min1)		   = explode(":", $horMin);
}else{
list($ano1, $mes1, $dia1)  = explode($ch1, $data1);
list($dia1, $horMin)	   = explode(" ", $dia1);
list($hora1, $min1)		   = explode(":", $horMin);
}

list($dia,$mes,$ano,$hora,$min) = explode("-",date("d-m-Y-H-i"));

//echo ($dia-$dia1)." | ".($mes-$mes1)." | ".($ano-$ano1)." | ".($hora-$hora1)." | ".($min - $min1)."<BR>";


if ($dia-$dia1 > 0 || $mes-$mes1 > 0 || $ano-$ano1 > 0){
$format = 'd H:i';
}else{
$format = 'H:i';
}

$date = date($format, mktime( ($hora-$hora1), ($min-$min1), 0, ($mes-$mes1), ($dia-$dia1), ($ano1-$ano) ) );

$date = str_replace(':', ' Hora(s) ',str_replace(' ', ' Dia(s) ', $date)).' Minuto(s)';

return $date;


$data_1 = mktime($hora1, $min1, 0, $mes1, $dia1, $ano1);
$data_2 = mktime($hora, $min, 0, $mes, $dia, $ano);
$diferenca = $data_2 - $data_1;
if($diferenca<0) $diferenca = 0;

$arrayd = converte_segundos($diferenca, 'd');

return $arrayd['dias']." Dia(s) ". $arrayd['horas']." Hora(s) ".$arrayd['minutos']." Minuto(s)";

}
*///echo dataDecorrida('26/11/2008 06:01');

/*
 function converte_segundos($total_segundos, $inicio = 'Y') {

 Devido à variação de dias entre os meses (pode ter 28, 29, 30 ou 31), o cálculo com diferenças entre timestamps nunca poderá ser exato, a não ser que o cálculo comece pelo número de dias (ou horas, minutos, segundos). Para minimizar ao máximo essa diferença, eu criei esta constante para utilizar durante o cálculo:

 define('dias_por_mes', ((((365*3)+366)/4)/12) );

 $comecou = false;

 if ($inicio == 'Y')
 {
 $array['anos'] = floor( $total_segundos / (60*60*24* dias_por_mes *12) );
 $total_segundos = ($total_segundos % (60*60*24* dias_por_mes *12));
 $comecou = true;
 }
 if (($inicio == 'm') || ($comecou == true))
 {
 $array['meses'] = floor( $total_segundos / (60*60*24* dias_por_mes ) );
 $total_segundos = ($total_segundos % (60*60*24* dias_por_mes ));
 $comecou = true;
 }
 if (($inicio == 'd') || ($comecou == true))
 {
 $array['dias'] = floor( $total_segundos / (60*60*24) );
 $total_segundos = ($total_segundos % (60*60*24));
 $comecou = true;
 }
 if (($inicio == 'H') || ($comecou == true))
 {
 $array['horas'] = floor( $total_segundos / (60*60) );
 $total_segundos = ($total_segundos % (60*60));
 $comecou = true;
 }
 if (($inicio == 'i') || ($comecou == true))
 {
 $array['minutos'] = floor($total_segundos / 60);
 $total_segundos = ($total_segundos % 60);
 $comecou = true;
 }
 $array['segundos'] = $total_segundos;

 return $array;
 }
 */

/* Função que retorna o pai de todos os níveis da demanda corrente .*/
function dmdPai ($dmdid){
	global $db;
	$sql="select dm.dmdidorigem from demandas.demanda dm where dmdid = $dmdid";
	$pai = $db->pegaUm($sql);
	if($pai){
		$pai2 = dmdPai($pai);
		return $pai2;
	}
	else{
		return $dmdid;
	}
}

/* Função que monta/retorna os filhos das demandas.*/
function dmdFilho ($dmdid,$width,$profundidade = null){
	global $db;

	if($profundidade == 1){
		$sql = "select dm.dmdid, dm.dmdtitulo from demandas.demanda dm where dm.dmdid = $dmdid order by dmdid desc";
		$dadosDemanda = $db->carregar($sql);
		$caminho = $_SERVER ['REQUEST_URI'];
		$caminho = explode("&dmdid=",$caminho);
		$caminho = $caminho[0]."&dmdid={$dadosDemanda[0]['dmdid']}";
		($_SESSION['dmdid'] == $dadosDemanda[0]['dmdid'])? $cor = "font-weight:bold" : $cor="";
		$tr_filhos .= "<div style=\"text-align: left;background: rgb(238, 238, 238);$cor\" ><a href=\"$caminho\"> Cód. # {$dadosDemanda[0]['dmdid']} -  {$dadosDemanda[0]['dmdtitulo']}</a></div>";
	}

	($profundidade)? $profundidade++ : $profundidade = $profundidade;
	$sql="select dm.dmdid, dm.dmdtitulo from demandas.demanda dm where dm.dmdidorigem = $dmdid order by dmdid desc";
	$filhos = $db->carregar($sql);
	if($filhos){
		$nivel = 1;
		foreach($filhos AS $fl){
			$arvore = "1.$x$nivel";
			$caminho = $_SERVER ['REQUEST_URI'];
			$caminho = explode("&dmdid=",$caminho);
			$caminho = $caminho[0]."&dmdid={$fl['dmdid']}";
			($_SESSION['dmdid'] == $fl['dmdid'])? $cor = "font-weight:bold" : $cor="";
			$tr_filhos .= ("
						<div style=\"text-align: left;background: rgb(238, 238, 238);padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\" ><img src='../imagens/seta_filho.gif' ><a href=\"$caminho\" > Cód. # {$fl['dmdid']} - {$fl['dmdtitulo']}</a></div>
						");
			$filho = dmdFilho($fl['dmdid'],$width,$profundidade);
			($filho)? $profundidade = $profundidade : "" ;
			$tr_filhos .= $filho;
			$nivel++;
		}
	}
	return $tr_filhos;
}



/**************
 * Função que monta/retorna o "sub-cabeçalho" padrão, para as telas do demandas.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @return cab (html)
 *
 **************/
function cabecalhoDemanda(){
	global $db;

	$dmdid = $_GET['dmdid'];

	if ($dmdid){
		unset($_SESSION['dmdid']);
		$caminho = $_SERVER ['REQUEST_URI'];
		$caminho = explode("&dmdid=",$caminho);
		$_SESSION['dmdid'] = $_GET['dmdid'];
		echo "<script>window.location.href=\"{$caminho[0]}\";</script>";
		exit;
	}
	else{
		$dmdid = $_SESSION['dmdid'];
	}

	if (!$dmdid){
		echo "<script>window.location.href=\"demandas.php?modulo=principal/lista&acao=A\";</script>";
		exit;
	}

	$sql = "SELECT
			 dmdtitulo,
			 to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI:SS') AS dmddatainclusao,
			 to_char(d.dmddatainclusao, 'YYYY-MM-DD HH24:MI:00') AS dmddatainclusaodif,
			 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
			 d.dmdhorarioatendimento,
			 od.ordid,
			 od.orddescricao ||' - '|| ts.tipnome AS origem,
			 esddsc,
			 CASE 
			  	WHEN d.dmdnomedemandante != '' THEN  upper(d.dmdnomedemandante)
			  	ELSE  upper(u.usunome)
			 END as solicitante	,
			 --dataconc
			 (select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
			  from workflow.historicodocumento a
			  inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
			  where aedid in (146, 191) 
			  and c.esdid in (93,95,109,111,135,136,170)
			  and a.docid = d.docid
			  ) as dataconc		 
			FROM
			 demandas.demanda d
			 LEFT JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 LEFT JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
			 -- LEFT JOIN workflow.tipodocumento tpd ON tpd.tpdid = doc.tpdid
			 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid 
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
			 /*
			 LEFT JOIN (  (select a.docid, to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as dataconc
								 		from 	workflow.historicodocumento a
								 			inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
								 	where aedid in (146, 191) and c.esdid in (93,95,109,111,135,136,170)
								  group by a.docid ) ) as hst ON hst.docid = d.docid
			 */
			WHERE
			 dmdid = {$dmdid}";
	$dados = $db->carregar($sql);
	if($dados[0]){
		extract($dados[0]);
	}
	else{
		echo "<script>window.location.href=\"demandas.php?modulo=principal/lista&acao=A\";</script>";
		exit;
	}

	$dataconclusao = date("Y-m-d H:i:00");
	if($dataconc) $dataconclusao = $dataconc;

	$Pai = dmdPai($dmdid);
	$filhos = dmdFilho($Pai,0,1);
	$sql = "select count(dmdid) from demandas.demanda where dmdidorigem = $Pai";
	$filho = $db->pegaUm($sql);
	($filho)? $vis="" : $vis = "none";


	
	if($dataconclusao){
		
		//ini_set("memory_limit", "1024M");
		
		if(!$dmddatainiprevatendimento) $dmddatainiprevatendimento = $dmddatainclusaodif;
		$ano_ini_c	= substr($dmddatainiprevatendimento,0,4);
		$mes_ini_c	= substr($dmddatainiprevatendimento,5,2);
		$dia_ini_c	= substr($dmddatainiprevatendimento,8,2);
		$hor_ini_c	= substr($dmddatainiprevatendimento,11,2);
		$min_ini_c	= substr($dmddatainiprevatendimento,14,2);
		//$seg_ini_c	= substr($dmddatainiprevatendimento,17,2);
		$seg_ini_c = 0;
		
		
		
		//calcula Duração do atendimento
		$total_minuto_conclusao = calculaTempoMinuto($dmddatainiprevatendimento, $dataconclusao, $dmdhorarioatendimento, $ordid);
		
		$dataFinalConc = mktime($hor_ini_c,$min_ini_c+$total_minuto_conclusao,$seg_ini_c,$mes_ini_c,$dia_ini_c,$ano_ini_c); 
		$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);			
		
		$classdata = new Data();
		$datadiff = $classdata->diferencaEntreDatas(  $dmddatainiprevatendimento, $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
		
		if(!$datadiff) $datadiff = "1 minuto";
	}
	
/*
	$datadf = new Data();
	$retorno = $datadf->diferencaEntreDatas(  $dmddatainclusaodif, $dataconclusao, 'tempoEntreDadas', 'string','dd/mm/yyyy');
	$datadiff = $retorno;
*/

	$cab = "<table align=\"center\" class=\"Tabela\" style='border-bottom:2px solid #000;'>
			 <tbody>
			 	<tr style=\"display:$vis\" >
			 		<td colspan=2>$filhos</td>
			 	</tr>		 
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Solicitante:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$solicitante}</td>
				</tr>
			 	<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Serviço Solicitado:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dmdtitulo}</td>
				</tr>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Origem Demanda:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$origem}</td>
				</tr>								 
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Situação:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$esddsc}</td>
				</tr>			 
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Data de Abertura:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dmddatainclusao}</td>
				</tr>
				<tr>
					<td width=\"20%\" nowrap style=\"text-align: right;\" class=\"SubTituloEsquerda\">Tempo decorrido da demanda:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial; color:blue;\" class=\"SubTituloDireita\"><B>".$datadiff."</b> </td>
				</tr>				
			 </tbody>
			</table>";
	return $cab;
}


/**************
 * Função que monta/retorna o "sub-cabeçalho" padrão, para as telas do sistemas.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @return cab (html)
 *
 **************/
function cabecalhoSistemas(){
	global $db;


	$sidid = $_SESSION['sidid'];


	if (!$sidid){
		return;
	}


	$sql= "SELECT
			 sd.sidabrev ||' - '|| sd.siddescricao AS dscsistema, 
		     ss.ssidsc AS sitsistema, 
		     u.unasigla ||' - '|| u.unadescricao AS orgsistema
		   FROM
		     demandas.sistemadetalhe sd 
		   LEFT JOIN demandas.sistemasituacao ss ON ss.ssiid = sd.ssiid
		   LEFT JOIN demandas.unidadeatendimento u ON u.unaid = sd.unaid
		   WHERE 
		     sd.sidid = {$sidid}";
	//dbg($sql,1);
	$dados = $db->carregar($sql);
	extract($dados[0]);


	$cab = "<table align=\"center\" class=\"Tabela\" style='border-bottom:2px solid #000;'>
			 <tbody>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Sistema:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$dscsistema}</td>
				</tr>
			 	<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Órgão:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$orgsistema}</td>
				</tr>
				<tr>
					<td width=\"20%\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Situação:</td>
					<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$sitsistema}</td>
				</tr>								 
			 </tbody>
			</table>";
	return $cab;
}


/**************
 * Função que retorna o sidid "demandas.sistemadetalhe", vinculado a demanda.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $dmdid (integer)
 * @return sidid (integer)
 *
 **************/
function pegaSidid($dmdid = null){
	global $db;

	if (!$dmdid){
		return;
	}
	$sql = "SELECT
			 sidid
			FROM
			 demandas.demanda
			WHERE
			 dmdid ={$dmdid};";

	return $db->pegaUm($sql);
}
/****************************
 *
 * FUNÇÕES DO WORWFLOW
 *
 ****************************/

/**************
 * Função que retorna o ID do "workflow.documento", vinculado a demanda.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $dmdid (integer)
 * @return docid (integer)
 *
 **************/
function pegarDocid( $dmdid = null ) {
	global $db;

	$dmdid = (integer) $dmdid ? $dmdid : $_SESSION['dmdid'];

	$sql = "SELECT
			 docid
			FROM
			 demandas.demanda
			WHERE
			 dmdid = {$dmdid}";
	return (integer) $db->pegaUm( $sql );
}

/**************
 * Função que cria o "workflow.documento", caso não exista, vinculando-o a "demandas.demanda"
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $dmdid (integer)
 * @return docid (integer)
 *
 **************/
function criarDocumento( $dmdid = null ) {
	global $db;

	$dmdid = (integer) $dmdid ? $dmdid : $_SESSION['dmdid'];
	$docid = pegarDocid($dmdid);

	if(!$docid){

		/**
		 * pega origem da demanda
		 **/
		$sql = "SELECT
				 t.ordid
				FROM
				 demandas.demanda d
				 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
				WHERE
				 dmdid = {$dmdid}";

		$ordid = $db->pegaUm($sql);

		if (!$ordid)
		return false;
		/*
		 * define tipo do documento "WORKFLOW"
		 */
		if($ordid == ORIGEM_DEMANDA_SISTEMA_INFORMACAO || $ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21 || $ordid == 23){
			$tpdid = DEMANDA_WORKFLOW_GENERICO;
		}
		else{
			$tpdid = DEMANDA_WORKFLOW_ATENDIMENTO;
		}
			


		/*
		 * Pega nome da demanda
		 */
		$sqlDescricao = "SELECT
						  REPLACE (dmdtitulo, chr(92), chr(47))
						 FROM
						  demandas.demanda
						 WHERE
						  dmdid = '" . $dmdid . "'";

		$descricao = $db->pegaUm( $sqlDescricao );

		$docdsc = "Cadastramento DEMANDAS - " . $descricao;

		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		/*
		 * Atualiza docid na demanda
		 */
		//if($docid){
		$sql = "UPDATE demandas.demanda SET
				 docid = '".$docid."' 
				WHERE
				 dmdid = ".$dmdid;				

		$db->executar( $sql );
		$db->commit();
		//}
	}
	return $docid;
}

/**************
 * Função que retorna o estado do documento, vinculado a demanda.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @return esdid (integer)
 *
 **************/
function recuperaEstadoDocumento (){
	global $db;

	$dmdid = $_SESSION['dmdid'];

	if ( !$dmdid ){
		return;
	}

	$sql = "SELECT
				dc.esdid
			FROM
				demandas.demanda d
				INNER JOIN workflow.documento dc ON dc.docid = d.docid and dc.tpdid in (31,35)
			WHERE
				dmdid = ".$dmdid;

	return $db->pegaUm($sql);
}

/*******
 * Funções de verifição e pós ação
 ********/

// Funções do estado "Cancelada"
function enviaEmailCanceladaFinalizar(){
	return true;
}



function enviaEmailCadDemanda($dmdid) {
	global $db;

	$emailCopia = '';

	// Seta remetente
	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);

	$sql = "SELECT
				d.dmdid,
				d.dmdtitulo as assunto,
				d.dmddsc as descricao,
				to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
		 		to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
				to_char(dmddatainclusao, 'YYYY') AS ano,
				od.orddescricao ||' / '|| ts.tipnome as origem,
				od.ordid as ordid,
				CASE 
				 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
				 ELSE u.usuemail
				END AS emaildemandante,
				CASE 
				 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
				 ELSE u.usunome
				END AS demandante,				
				'(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
				upper(unasigla)||' - '||unadescricao as setor, 	
				loc.lcadescricao as edificio,
				aa.anddescricao AS andar,
				d.dmdsalaatendimento as sala,
				sd.siddescricao as sistema				  	
			FROM 
				demandas.demanda d
				INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
				INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
				LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
				LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 
				LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
				LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
				LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
				LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
				dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	// seta dados da demanda
	$dadoDemanda = array();


	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
											  "descricao" => '-',
											  "usuario"	=> '-',
											  "data" => '-'	 
											  )
											  );
											  
											  // Seta responsável
											  //$ano  		 = $dado['ano'];
											  //$emailDem	 = $dado['demandante'];
											  $destinatario = $dado['emaildemandante'];
											  if($dado['ordid'] == '8'){
											  	$emailCopia	 = "cgi-dbd@mec.gov.br";
											  }
											  
											  // Seta Assunto
											  $assunto = "Demanda Nº {$dmdid} Cadastrada.";

											  // Seta Conteúdo
											  $conteudo = textMail("Demanda Nº {$dmdid} Cadastrada.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

											  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
											   
											  return true;
}



function enviaEmailCancelamento() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	$update = "UPDATE
	 		   	demandas.demanda
			   SET
			    usucpfanalise = '".$_SESSION['usucpf']."', 
			    usucpfexecutor = '".$_SESSION['usucpf']."'
			   WHERE
			    dmdid = {$dmdid}";

	$db->executar($update);
	//	$db->commit();

	// Seta remetente
	$remetente = array('nome'=>REMETENTE_WORKFLOW_NOME, 'email'=>REMETENTE_WORKFLOW_EMAIL);

	$sql = "SELECT
				d.dmdid,
				d.dmdtitulo as assunto,
				d.dmddsc as descricao,
				to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
		 		to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
				to_char(dmddatainclusao, 'YYYY') AS ano,
				od.orddescricao ||' / '|| ts.tipnome as origem,
				CASE 
				 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
				 ELSE u.usuemail
				END AS emaildemandante,
				CASE 
				 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
				 ELSE u.usunome
				END AS demandante,				
				u2.usunome AS resp,
				'(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
				upper(unasigla)||' - '||unadescricao as setor, 	
				loc.lcadescricao as edificio,
				aa.anddescricao AS andar,
				d.dmdsalaatendimento as sala,
				sd.siddescricao as sistema				  	
			FROM 
				demandas.demanda d
				INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
				INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
				LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
				LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 
				LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
				LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
				LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
				LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
				LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
				dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	// seta dados da demanda
	$dadoDemanda = array();


	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
	  "descricao" => '-',
	  "usuario"	=> '-',
	  "data" => '-'	 
	  )
	  );
	  // Seta ano/responsável
	  $ano  		 = $dado['ano'];
	  $responsavel = $dado['resp'];
	  $emailDem	 = $dado['emaildemandante'];

	  // Seta Assunto
	  $assunto = "Demanda [{$dmdid}] -  Cancelamento do Chamado.";

	  // Seta Conteúdo
	  $conteudo = textMail("Demanda [{$dmdid}] foi cancelada. Para maiores informações, entre em contato com o responsável {$responsavel}.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);
	  //"Sua demanda [{$dmdid}/{$ano}] foi cancelada. Para maiores informações, entre em contato com o responsável {$responsavel}.";
	  //	$emailDem = 'felipe.chiavicatti@mec.gov.br';
	  //	echo "$rementente | $emailDem | $assunto | $conteudo | $emailCopia";
	  //	exit;

	  enviar_email( $remetente, $emailDem, $assunto, $conteudo, $emailCopia );
	  return true;
}

function enviaEmailAnaliseFinalizada(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') AS dtini,
	 		 to_char(d.dmddatafimprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') AS dtfim,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
	 		 u2.usuemail as emailtec,
	 		 u2.usunome as nometec,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema 
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$emailTec 			= $dado['emailtec'];
	$nomeTec  			= $dado['nometec'];
	$emailDemandante  	= $dado['emaildemandante'];
	$ano	  			= $dado['ano'];


	// 	seta dados da addlinha
	$addLinha['Previsão de atendimento']  = 'Início: '. ($dado['dtini'] ? $dado['dtini'] : '<B>-</B>').'<br>até<br>Término: '. ($dado['dtfim'] ? $dado['dtfim'] : '<B>-</B>');
	$addLinha['Técnico Responsável'] = $nomeTec;
	$addLinha['Obs'] = "Demanda será atendida o mais rápido possível.";

	//pega id pai
	$descidpai = "";
	$idpai = $db->pegaUm("select dmdidorigem from demandas.demanda where dmdid = {$dmdid}");
	if($idpai) $descidpai = " - <b><font color=red>(Originado da demanda código: $idpai)</font></b>";

	// seta dados da demanda
	$dadoDemanda['Código']			  = $dado['dmdid'] . $descidpai;
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
		  "arquivo" => '-',
		  "data"	=> '-'	 
		  )
		  );

		  // Busca observações
		  $sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

		  $dadoObs = $db->carregar($sql);
		  $dadoObs = $dadoObs ? $dadoObs : array(
		  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
	  	  "data" => '-'	 
	  	  )
	  	  );
	  	   
	  	   

	  	  // Seta assunto
	  	  $assunto  = "Demanda [{$dmdid}] – Envio para atendimento";

	  	  // Seta Conteúdo
	  	  $conteudo = textMail("Demanda [{$dmdid}] foi enviada para atendimento e está sob sua responsabilidade.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

	  	  //$conteudo .= "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";

	  	  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";
	  	  //	$emailTec = 'felipe.chiavicatti@mec.gov.br';
	  	  //	echo $remetente." <==> ".$emailTec." <==> ".$assunto." <==> ".$conteudo." <==> ".$emailCopia;
	  	  //dbg($conteudo,1);
	  	   
	  	  //para o técnico
	  	  enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );
	  	   
	  	   
	  	  // Seta assunto
	  	  $assunto  = "Demanda nº {$dmdid} – Atendimento em andamento";
	  	  // Seta Conteúdo
	  	  $dadoArquivo = null;
	  	  $conteudo = textMail("ATENDIMENTO DEMANDA Nº {$dmdid} EM ANDAMENTO", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);
	  	   
	  	  //para o demandante
	  	  enviar_email( $remetente, $emailDemandante, $assunto, $conteudo, $emailCopia );
	  	   
	  	  return true;

}


function enviaEmailAlertaDemandaAtraso($dmdid){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	//$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
	 		 u2.usuemail as emailtec,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
	 		 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 (CASE WHEN ed.esddsc <> '' THEN
		 	 		ed.esddsc
		 	   ELSE 
		 	   		'Em processamento'
		 	  END) AS situacao 
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 INNER JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
			 INNER JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$emailDem = $dado['emaildemandante'];
	$emailCopia = 'alex.pereira@mec.gov.br';
	$ano	  = $dado['ano'];

	//pega id pai
	$descidpai = "";
	$idpai = $db->pegaUm("select dmdidorigem from demandas.demanda where dmdid = {$dmdid}");
	if($idpai) $descidpai = " - <b><font color=red>(Originado da demanda ID: $idpai)</font></b>";

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'] . $descidpai;
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Situação']		  = $dado['situacao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
	  "descricao" => '-',
	  "usuario"	=> '-',
	  "data" => '-'	 
	  )
	  );

	  // Seta assunto
	  $assunto  = "Demanda [{$dmdid}] – alerta de demanda";

	  // Seta Conteúdo
	  $conteudo = textMail("Demanda [{$dmdid}] está a menos de 1 hora para entrar em atraso", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

	  //$conteudo .= "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";

	  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";
	  //	$emailDem = 'felipe.chiavicatti@mec.gov.br';
	  //	echo $remetente." <==> ".$emailDem." <==> ".$assunto." <==> ".$conteudo." <==> ".$emailCopia;
	  //dbg($conteudo,1);
	   
	  enviar_email( $remetente, $emailDem, $assunto, $conteudo, $emailCopia );
	  return true;

}


function enviaEmailAltera(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
	 		 u2.usuemail as emailtec,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
	 		 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema 
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$emailTec = $dado['emailtec'];
    $emailCopia = $dado['emaildemandante'];


	if($emailTec){
		$ano	  = $dado['ano'];

		// seta dados da demanda
		$dadoDemanda['ID'] 				  = $dado['dmdid'];
		$dadoDemanda['Origem da demanda'] = $dado['origem'];

		if ($dado['sistema'])
		$dadoDemanda['Sistema']		  = $dado['sistema'];

		$dadoDemanda['Assunto']			  = $dado['assunto'];
		$dadoDemanda['Descricão']		  = $dado['descricao'];
		$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

		/*
		 $dadoDemanda = array(
		 "ID"					   => $dado['dmdid'],
		 "Origem da demanda" 	   => $dado['origem'],
		 "Sistema"				   => $dado['sistema'],
		 "Assunto" 			 	   => $dado['assunto'],
		 "Descricão" 		 	   => $dado['descricao'],
		 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
		 );*/

		// seta dados do demandante
		$dadoDemandante = array (
		    						 "Solicitante" => $dado['demandante'],
		    						 "Telefone"    => $dado['fone'],
		     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
		    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
		    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
		    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
		);
		// Busca arquivos
		$sql = "SELECT
						arqnome||'.'||arqextensao AS arquivo,
						to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
					FROM 
						demandas.anexos a
						INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
					WHERE	
						a.dmdid = {$dmdid}"; 

		$dadoArquivo = $db->carregar($sql);
		$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
		array("arquivo" => '-',
																	  "data"	=> '-'	 
																	  )
																	  );

																	  // Busca observações
																	  $sql = "SELECT
						obsdsc AS descricao,
						usunome AS usuario,
						to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
					FROM 
						demandas.observacoes o
						INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
					WHERE	
						o.obsstatus = 'A' AND
						o.obslog is null AND
						dmdid = {$dmdid}
					ORDER BY obsid DESC"; 		

																	  $dadoObs = $db->carregar($sql);
																	  $dadoObs = $dadoObs ? $dadoObs : array(
																	  array(  "descricao" => '-',
																  "usuario"	=> '-',
														  		  "data" => '-'	 
														  		  )
														  		  );

														  		  // Seta assunto
														  		  $assunto  = "Demanda [{$dmdid}] – Reenvio para atendimento";

														  		  // Seta Conteúdo
														  		  $conteudo = textMail("Demanda [{$dmdid}] foi alterada e reenviada para atendimento e está sob sua responsabilidade.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

														  		  //$conteudo .= "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";

														  		  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";
														  		  //	$emailTec = 'felipe.chiavicatti@mec.gov.br';
														  		  //	echo $remetente." <==> ".$emailTec." <==> ".$assunto." <==> ".$conteudo." <==> ".$emailCopia;

														  		  enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );

														  		  return true;
	}
	else{
		return false;
	}
}

function enviaEmailAlteraAtrasoCelula(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
	 		 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 u3.usunome as nomeanalista,
			 u2.usunome as nometecnico
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf
			 LEFT JOIN seguranca.usuario u3 ON d.usucpfanalise = u3.usucpf  
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$nometecnico = $dado['nometecnico'];
	$nomeanalista = $dado['nomeanalista'];
	$emailTec = "daniel.brito@mec.gov.br";
    $emailCopia = "andre.neto@mec.gov.br";


	if($emailTec){
		$ano	  = $dado['ano'];

		// seta dados da demanda
		$dadoDemanda['ID'] 				  = $dado['dmdid'];
		$dadoDemanda['Origem da demanda'] = $dado['origem'];

		if ($dado['sistema'])
		$dadoDemanda['Sistema']		  = $dado['sistema'];

		$dadoDemanda['Assunto']			  = $dado['assunto'];
		$dadoDemanda['Descricão']		  = $dado['descricao'];
		$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');
		$dadoDemanda['Programador']		  = $nometecnico;

		/*
		 $dadoDemanda = array(
		 "ID"					   => $dado['dmdid'],
		 "Origem da demanda" 	   => $dado['origem'],
		 "Sistema"				   => $dado['sistema'],
		 "Assunto" 			 	   => $dado['assunto'],
		 "Descricão" 		 	   => $dado['descricao'],
		 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
		 );*/

		// seta dados do demandante
		$dadoDemandante = array (
		    						 "Solicitante" => $dado['demandante'],
		    						 "Telefone"    => $dado['fone'],
		     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
		    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
		    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
		    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
		);
		// Busca arquivos
		$sql = "SELECT
						arqnome||'.'||arqextensao AS arquivo,
						to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
					FROM 
						demandas.anexos a
						INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
					WHERE	
						a.dmdid = {$dmdid}"; 

		$dadoArquivo = $db->carregar($sql);
		$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
															array("arquivo" => '-', "data"	=> '-')
														  );

	  // Busca observações
	  $sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			--o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC limit 1"; 		

			  $dadoObs = $db->carregar($sql);
			  $dadoObs = $dadoObs ? $dadoObs : array(
			  											array(  "descricao" => '-', "usuario" => '-', "data" => '-')
  		  											);

  		  // Seta assunto
  		  $assunto  = "Demanda [{$dmdid}] – Alteração de demanda atrasada pelo Analista: ".$nomeanalista;

  		  // Seta Conteúdo
  		  $conteudo = textMail("Demanda [{$dmdid}] foi alterada a data de previsão de término pelo Analista: ".$nomeanalista, $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

  		  //$conteudo .= "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";

  		  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";
  		  //	$emailTec = 'felipe.chiavicatti@mec.gov.br';
  		  //	echo $remetente." <==> ".$emailTec." <==> ".$assunto." <==> ".$conteudo." <==> ".$emailCopia;

  		  enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );

  		  return true;
	}
	else{
		return false;
	}
}

function enviaEmailEmAnalise(){
	global $db;

	if ( $_SERVER['HTTP_HOST'] == "localhost" || $_SERVER['HTTP_HOST'] == "simec-local" ){
        return true;
	}
	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
	 		 u2.usuemail as emailtec,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
	 		 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema 
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$emailTec = $dado['emailtec'];
    $emailCopia = "alexpereira@mec.gov.br";


	if($emailTec){
		$ano	  = $dado['ano'];

		// seta dados da demanda
		$dadoDemanda['ID'] 				  = $dado['dmdid'];
		$dadoDemanda['Origem da demanda'] = $dado['origem'];

		if ($dado['sistema'])
		$dadoDemanda['Sistema']		  = $dado['sistema'];

		$dadoDemanda['Assunto']			  = $dado['assunto'];
		$dadoDemanda['Descricão']		  = $dado['descricao'];
		$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

		/*
		 $dadoDemanda = array(
		 "ID"					   => $dado['dmdid'],
		 "Origem da demanda" 	   => $dado['origem'],
		 "Sistema"				   => $dado['sistema'],
		 "Assunto" 			 	   => $dado['assunto'],
		 "Descricão" 		 	   => $dado['descricao'],
		 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
		 );*/

		// seta dados do demandante
		$dadoDemandante = array (
		    						 "Solicitante" => $dado['demandante'],
		    						 "Telefone"    => $dado['fone'],
		     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
		    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
		    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
		    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
		);
		// Busca arquivos
		$sql = "SELECT
						arqnome||'.'||arqextensao AS arquivo,
						to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
					FROM 
						demandas.anexos a
						INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
					WHERE	
						a.dmdid = {$dmdid}"; 

		$dadoArquivo = $db->carregar($sql);
		$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
		array("arquivo" => '-',
																	  "data"	=> '-'	 
																	  )
																	  );

																	  // Busca observações
																	  $sql = "SELECT
						obsdsc AS descricao,
						usunome AS usuario,
						to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
					FROM 
						demandas.observacoes o
						INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
					WHERE	
						o.obsstatus = 'A' AND
						o.obslog is null AND
						dmdid = {$dmdid}
					ORDER BY obsid DESC"; 		

																	  $dadoObs = $db->carregar($sql);
																	  $dadoObs = $dadoObs ? $dadoObs : array(
																	  array(  "descricao" => '-',
																  "usuario"	=> '-',
														  		  "data" => '-'	 
														  		  )
														  		  );

														  		  // Seta assunto
														  		  $assunto  = "Demanda [{$dmdid}] – Em Análise";

														  		  // Seta Conteúdo
														  		  $conteudo = textMail("Demanda [{$dmdid}] foi enviada para Em Análise e está sob sua responsabilidade.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

														  		  //$conteudo .= "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";

														  		  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi enviada para atendimento e está sob sua responsabilidade.";
														  		  //	$emailTec = 'felipe.chiavicatti@mec.gov.br';
														  		  //	echo $remetente." <==> ".$emailTec." <==> ".$assunto." <==> ".$conteudo." <==> ".$emailCopia;

														  		  enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );

														  		  return true;
	}
	else{
		return false;
	}
}

// Funções do estado "Em atendimento"
function enviaEmailAtendimentoFinalizado() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email analista/gerente e ano
	$sql = "SELECT
			 --cmddsc,
			 --dthst,
			 (SELECT (SELECT b.cmddsc FROM workflow.comentariodocumento b WHERE b.hstid = max(a.hstid)) 
              FROM workflow.historicodocumento a 
              WHERE a.aedid in (146, 191,224, 703) and docid=d.docid --atendimento técnico finalizado
             ) as cmddsc,
			 (SELECT to_char(max(a.htddata)::timestamp,'HH24:MI') || ' hr(s) - ' || to_char(max(a.htddata)::timestamp,'DD/MM/YYYY') 
              FROM workflow.historicodocumento a 
              WHERE a.aedid in (146, 191,224, 703) and docid=d.docid --atendimento técnico finalizado
             ) as dthst,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u3.usuemail AS analista,
			 --u2.usuemail AS gerente,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			 -- LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
			 --												   ur.rpustatus = 'A' AND
			 --												   ur.pflcod = ".DEMANDA_PERFIL_COORDENADOR."	
			 --LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf
			 /*  
			 LEFT JOIN (select a.hstid, a.docid, c.cmddsc, to_char(a.htddata::timestamp,'HH24:MI') || ' hr(s) - ' || to_char(a.htddata::timestamp,'DD/MM/YYYY') as dthst  
						from 	workflow.historicodocumento a
							inner join workflow.documento d on d.docid = a.docid and d.tpdid in (31,35)
							inner join workflow.comentariodocumento c on c.hstid = a.hstid
							INNER JOIN workflow.historicodocumento b ON b.hstid = d.hstid
							-- INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento where aedid in (146, 191)  GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (146, 191) -- atendimento técnico finalizado
					group by a.hstid, a.docid, c.cmddsc, a.htddata) as ate ON ate.docid = d.docid	
			 */			 			 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
			 dmdid = {$dmdid}";	

	$dado 		  = (array) $db->pegaLinha($sql);

	// Seta Destinatário = analista | Cópia = gerente | ano
	$destinatario = $dado['analista'];
	$cmddsc = $dado['cmddsc'];
	$dthst = $dado['dthst'];

	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21'){
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}


	$ano		  = $dado['ano'];

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
	  "descricao" => '-',
	  "usuario"	=> '-',
	  "data" => '-'	 
	  )
	  );

	  // Seta assunto
	  $assunto = "Demanda [{$dmdid}] – Atendimento Finalizado";

	  // Seta Conteúdo
	  $conteudo = textMail("Demanda [{$dmdid}] foi finalizada pelo técnico responsável. <br> <font color='black'>Serviço Executado: {$cmddsc} </font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);
	  //	$conteudo = "A demanda [{$dmdid}/{$ano}] foi finalizada pelo responsável pelo atendimento.";

	  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";

	  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
	   
	  //chama função envia email para avaliação da demanda
	  enviaEmailAvaliacao($cmddsc,$dthst);
	   
	  return true;
}

function enviaEmailAtendimentoRetorno() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email analista/gerente | ano | justificativa
	/*
	 $sql = "SELECT
	 cmddsc,
	 to_char(dmddatainclusao, 'YYYY') AS ano,
	 u3.usuemail AS analista,
	 --u2.usuemail AS gerente,
	 d.dmdid,
	 d.dmdtitulo as assunto,
	 d.dmddsc as descricao,
	 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 od.orddescricao ||' / '|| ts.tipnome as origem,
	 od.ordid as codorigem,
	 u.usunome AS demandante,
	 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
	 upper(unasigla)||' - '||unadescricao as setor,
	 loc.lcadescricao as edificio,
	 aa.anddescricao AS andar,
	 d.dmdsalaatendimento as sala,
	 sd.siddescricao as sistema
	 FROM
	 demandas.demanda d
	 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
	 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
	 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
	 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
	 LEFT JOIN (SELECT
	 cmdid,
	 cmddsc,
	 docid
	 FROM
	 workflow.comentariodocumento
	 ORDER BY
	 cmdid DESC
	 LIMIT 1) cd ON cd.docid = d.docid
	 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
	 --LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
	 --												   ur.rpustatus = 'A' AND
	 --												   ur.pflcod = ".DEMANDA_PERFIL_COORDENADOR."
	 --LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf
	 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
	 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
	 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
	 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
	 WHERE
	 dmdid = {$dmdid}";
		*/

	$sql = "SELECT
			 (SELECT
					cmddsc
				 FROM 
					workflow.comentariodocumento
				 WHERE
				 	docid = d.docid
				 ORDER BY
					cmdid DESC
				 LIMIT 1) as cmddsc, 
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u3.usuemail AS analista,
			 --u2.usuemail AS gerente,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
	 		 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			 --LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
			 --												   ur.rpustatus = 'A' AND
			 --												   ur.pflcod = ".DEMANDA_PERFIL_COORDENADOR."	
			 --LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf  			 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid		 
			WHERE
			 dmdid = {$dmdid}";	

	//dbg($sql,1);
	$dado = (array) $db->pegaLinha($sql);

	// Seta Destinatário = analista | ANO
	$destinatario = $dado['analista'];

	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21'){
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod in (".DEMANDA_PERFIL_ADMINISTRADOR.",".DEMANDA_PERFIL_TECNICO1.")
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}

	$ano		  = $dado['ano'];
	$just		  = $dado['cmddsc'];

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
	  "descricao" => '-',
	  "usuario"	=> '-',
	  "data" => '-'	 
	  )
	  );


	  // Seta assunto
	  $assunto = "Demanda –[{$dmdid}] - Retorno para Análise";

	  // Seta Conteúdo
	  $conteudo = textMail("Demanda [{$dmdid}] foi devolvida para análise pelo técnico responsável. <br> <font color='black'>Justificativa: {$just}</font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

	  //	$conteudo .= "A demanda [{$dmdid}/{$ano}] foi devolvida pelo responsável pelo atendimento solicitando. Justificativa: {$just}";

	  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi devolvida pelo responsável pelo atendimento solicitando. Justificativa: {$just}";

	  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
	  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
	  return true;
}

// Funções do estado "Aguardando Validação"

function enviaEmailValidacaoRetorno() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email tecnico | ano	| justificativa
	$sql = "SELECT
			 --cmddsc,
			 (SELECT cmddsc FROM workflow.comentariodocumento
			  WHERE docid = d.docid
			  ORDER BY cmdid DESC
			  LIMIT 1
			 ) as cmddsc,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u3.usuemail AS emailtec,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 
			 /*		 			 
			 LEFT JOIN (SELECT
							cmdid,
							cmddsc,
							docid
						 FROM 
							workflow.comentariodocumento
						 ORDER BY
							cmdid DESC
						 LIMIT 1) cd ON cd.docid = d.docid
			 */
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfexecutor
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid		 	 
			WHERE
			 dmdid = {$dmdid}";		

	$dado = (array) $db->pegaLinha($sql);

	$emailTec = $dado['emailtec'];
	$ano  	  = $dado['ano'];
	$just 	  = $dado['cmddsc'];
	//$nome = nomeUser();

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto  = "Demanda [{$dmdid}] - Retorno para Atendimento";

		  // Seta Conteúdo
		  $conteudo = textMail("Demanda [{$dmdid}] foi devolvida para atendimento pelo gerente/analista solicitando ajuste(s) no(s) problema(s) abaixo relacionado(s):<BR><font color='black'>{$just}</font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

		  // Seta Conteúdo
		  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi devolvida pelo gerente/analista solicitando ajuste(s) no(s) problema(s) abaixo relacionado(s):<BR>
		  //{$just}";

		  //echo "$remetente<BR>$emailTec<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );
		  return true;
}


function enviaEmailRevalidacao() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email tecnico | ano	| justificativa
	$sql = "SELECT
			 --cmddsc as justificativa,
			 --emailgestor as emailgestor,
			 (select c.cmddsc
			  from 	workflow.historicodocumento a
			  inner join workflow.documento d2 on d2.docid = a.docid and d2.tpdid in (31,35)
			  inner join seguranca.usuario u on u.usucpf = a.usucpf
			  inner join workflow.comentariodocumento c on c.hstid = a.hstid
			  where a.aedid in (224,186,165,278,279) and a.docid = d.docid --224,186,165=finalizada/validada -  278,279=invalidada
		          group by c.cmddsc
			 ) as justificativa,
			 (select u.usuemail
			  from 	workflow.historicodocumento a
			  inner join workflow.documento d2 on d2.docid = a.docid and d2.tpdid in (31,35)
			  inner join seguranca.usuario u on u.usucpf = a.usucpf
			  inner join workflow.comentariodocumento c on c.hstid = a.hstid
			  where a.aedid in (224,186,165,278,279) and a.docid = d.docid --224,186,165=finalizada/validada -  278,279=invalidada
			  group by u.usuemail
			 ) as emailgestor,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 --u3.usuemail AS emailtec,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 /*
			 LEFT JOIN (select a.docid, u.usuemail as emailgestor, c.cmddsc  
						from 	workflow.historicodocumento a
							inner join workflow.documento d on d.docid = a.docid and d.tpdid in (31,35)
							inner join seguranca.usuario u on u.usucpf = a.usucpf
							inner join workflow.comentariodocumento c on c.hstid = a.hstid
					where a.aedid in (224,186,165,278,279) --224,186,165=finalizada/validada -  278,279=invalidada
					group by a.docid, u.usuemail, c.cmddsc) as ges ON ges.docid = d.docid
			 */						 
			 --LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfexecutor
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid		 	 
			WHERE
			 dmdid = {$dmdid}";		

	$dado = (array) $db->pegaLinha($sql);

	$emailGestor = $dado['emailgestor'];
	$ano  	  = $dado['ano'];
	$just 	  = $dado['justificativa'];
	//$nome = nomeUser();

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto  = "Demanda [{$dmdid}] - Solicitado para Revaliadação";

		  // Seta Conteúdo
		  $conteudo = textMail("Demanda [{$dmdid}] foi solicitada para Revalidação.<BR><font color='black'><b>Justificativa:</b>{$just}</font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

		  // Seta Conteúdo
		  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi devolvida pelo gerente/analista solicitando ajuste(s) no(s) problema(s) abaixo relacionado(s):<BR>
		  //{$just}";

		  //echo "$remetente<BR>$emailTec<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  enviar_email( $remetente, $emailGestor, $assunto, $conteudo, $emailCopia );
		  return true;
}



function enviaEmailPausarDemanda(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email analista/gerente e ano
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 --u3.usuemail AS analista,
			 --u2.usuemail AS gerente,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 u4.usunome AS nometecnico,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 pad.pdmjustificativa as justificativa,
			 tpa.tpadsc as tipopausa,
			 to_char(pad.pdmdatainiciopausa,'HH24:MI') || ' hr(s) ' || to_char(pad.pdmdatainiciopausa,'DD/MM/YYYY') AS dtinipausa,
			 CASE WHEN pad.pdmdatafimpausa is null THEN 
			 		'Indeterminado'
			 	  ELSE
			 	  	to_char(pad.pdmdatafimpausa,'HH24:MI') || ' hr(s) ' || to_char(pad.pdmdatafimpausa,'DD/MM/YYYY') 
			 END AS dtfimpausa
			 
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN seguranca.usuario u4 ON u4.usucpf = d.usucpfexecutor
			 --LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			-- LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
			 --												   ur.rpustatus = 'A' AND
			 --												   ur.pflcod = ".DEMANDA_PERFIL_COORDENADOR."	
			 --LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf  			 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			 LEFT JOIN demandas.pausademanda AS pad ON pad.dmdid = d.dmdid
			 LEFT JOIN demandas.tipopausademanda AS tpa ON tpa.tpaid = pad.tpaid
			WHERE
			 d.dmdid = {$dmdid}";	

	$dado 		  = (array) $db->pegaLinha($sql);

	// Seta Destinatário = analista | Cópia = gerente | ano
	$destinatario = $dado['emaildemandante'];

	$nometecnico = $dado['nometecnico'];
	$tipopausa = $dado['tipopausa'];
	$justificativa = $dado['justificativa'];

	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21'){
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}


	$ano		  = $dado['ano'];


	// 	seta dados da addlinha
	$addLinha['Justificativa']  = $tipopausa .' - '. $justificativa;
	$addLinha['Início da Pausa']  = ($dado['dtinipausa'] ? $dado['dtinipausa'] : '<B>-</B>');
	$addLinha['Previsão para término da pausa']  = ($dado['dtfimpausa'] ? $dado['dtfimpausa'] : '<B>-</B>');
	$addLinha['Demanda pausada pelo Técnico'] = $nometecnico;



	// seta dados da demanda
	$dadoDemanda['Código'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] 	  = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  	  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data	
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto = "Demanda nº {$dmdid} – Início de Pausa";

		  $textoconteudo = "DEMANDA Nº {$dmdid} - PAUSADA";

		  // Seta Conteúdo
		  $dadoArquivo = null;
		  $conteudo = textMail($textoconteudo, $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);
		  //	$conteudo = "A demanda [{$dmdid}/{$ano}] foi finalizada pelo responsável pelo atendimento.";

		  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";

		  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );

		  return true;


}

function enviaEmailDespausarDemanda(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	if(!$_SESSION['dmdid']) return false;
	
	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email analista/gerente | ano
	$sql = "SELECT
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u3.usuemail AS emailanalista,
			 u4.usuemail AS emailtecnico,
			 --u2.usuemail AS gerente,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') AS dtfim,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
 			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 pad.pdmjustificativa as justificativa,
			 tpa.tpadsc as tipopausa,
			 to_char(pad.pdmdatainiciopausa,'HH24:MI') || ' hr(s) ' || to_char(pad.pdmdatainiciopausa,'DD/MM/YYYY') AS dtinipausa,
			 CASE WHEN pad.pdmdatafimpausa is null THEN 
			 		'Indeterminado'
			 	  ELSE
			 	  	to_char(pad.pdmdatafimpausa,'HH24:MI') || ' hr(s) ' || to_char(pad.pdmdatafimpausa,'DD/MM/YYYY') 
			 END AS dtfimpausa
			 
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			 LEFT JOIN seguranca.usuario u4 ON u4.usucpf = d.usucpfexecutor
			 --LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
			 --												   ur.rpustatus = 'A' AND
			 --												   ur.pflcod = ".DEMANDA_PERFIL_COORDENADOR."	
			 --LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf  			 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid	
			 LEFT JOIN demandas.pausademanda AS pad ON pad.dmdid = d.dmdid
			 LEFT JOIN demandas.tipopausademanda AS tpa ON tpa.tpaid = pad.tpaid
			WHERE
			 d.dmdid = {$dmdid}";	

	//dbg($sql,1);
	$dado = (array) $db->pegaLinha($sql);

	// Seta Destinatário = analista | ANO
	$destinatario = $dado['emaildemandante'];

	$tipopausa = $dado['tipopausa'];
	$justificativa = $dado['justificativa'];


	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21'){
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}

	$emailCopia = $emailCopia . " ; " . $dado['emailanalista'] . " ; " . $dado['emailtecnico'];

	$ano		  = $dado['ano'];


	// 	seta dados da addlinha
	$addLinha['Justificativa']  = $tipopausa .' - '. $justificativa;
	$addLinha['Término da Pausa']  = ($dado['dtfimpausa'] ? $dado['dtfimpausa'] : '<B>-</B>');
	$addLinha['Previsão para término do atendimento']  = 'Até '.($dado['dtfim'] ? $dado['dtfim'] : '<B>-</B>');



	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );


		  // Seta assunto
		  $assunto = "Demanda nº {$dmdid} - Fim da Pausa";

		  // Seta Conteúdo
		  $dadoArquivo = null;
		  $conteudo = textMail("DEMANDA Nº {$dmdid} - FIM DA PAUSA <BR> ATENDIMENTO EM ANDAMENTO.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);

		  //	$conteudo .= "A demanda [{$dmdid}/{$ano}] foi devolvida pelo responsável pelo atendimento solicitando. Justificativa: {$just}";

		  //$conteudo = "A demanda [{$dmdid}/{$ano}] foi devolvida pelo responsável pelo atendimento solicitando. Justificativa: {$just}";

		  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
		  return true;

}

//function enviaEmailValidacaoFinalizada() {
function enviaEmailAvaliacao($servicoExec, $datafim) {
	global $db;


	$dmdid = $_SESSION['dmdid'];


	//grava codigo de segurança para enviar por email ao solicitante
	$letters = '1234567890qwertyuiopasdfghjklzxcvbnm';
	$length = 10;
	$s = '';
	$lettersLength = strlen($letters)-1;
	for($i = 0 ; $i < $length ; $i++)	$s .= $letters[rand(0,$lettersLength)];
	$codseg = $s;

	$sql = "UPDATE demandas.demanda SET dmdcodseg='$codseg' WHERE dmdid = {$dmdid}";
	$db->executar($sql);
	$db->commit();



	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';


	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email DEMANDANTE | ano
	$sql = "SELECT
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 CASE 
			  WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			  ELSE u.usuemail
			 END AS emaildemandante,
			 CASE 
			  WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			  ELSE u.usunome
 			 END AS demandante,				
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 u2.usunome as nometecnico,
			 od.ordid,
			 d.tipid
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
			 LEFT JOIN seguranca.usuario u ON d.usucpfdemandante = u.usucpf
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid		 	 			 
			WHERE
			 dmdid = {$dmdid}";

	$dado = $db->pegaLinha($sql);

	// Seta remetente/ano
	$emailDem = $dado['emaildemandante'];
	$ano	  = $dado['ano'];
	$tipid	  = $dado['tipid'];


	if($dado['ordid'] == '3') $txtsuporte = "Demanda executada pelo técnico: " .$dado['nometecnico'] . " do setor: SUPORTE DE ATENDIMENTO.<BR>";


	// 	seta dados da addlinha
	$addLinha['Serviço Executado']  = $servicoExec;
	$addLinha['Hora da finalização do serviço']  = $datafim;
	//$addLinha['Tempo total de atendimento']  = '';
	$addLinha['Finalizada pelo Técnico']  = $dado['nometecnico'];


	// seta dados da demanda
	$dadoDemanda['Código'] 			  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto = "Demanda Nº {$dmdid} Finalizada – Faça a avaliação do atendimento";

		  // Seta Conteúdo
		  $dadoArquivo = null;
		  $conteudo = textMail("DEMANDA Nº {$dmdid} FINALIZADA <BR> $txtsuporte <font color='#FFFF99'><b>Sua avaliação é muito importante para melhoria do processo. <BR> >> <a target='_blank' href='http://simec.mec.gov.br/demandas/popCadAvaliacao.php?dmdid={$dmdid}&codseg={$codseg}'>CLIQUE AQUI</a> para avaliar.</b></font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);

		  // Seta conteúdo
		  //	$conteudo = "O atendimento de sua demanda [{$dmdid}/{$ano}] precisa ser avaliada.";
		  //dbg($conteudo,1);
		  //echo "$remetente<BR>$emailDem<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  
		  //catalogo de serviço
		  //688 = 39 - Registro e classificação de demandas (suporte de atendimento)
		  if($tipid != 688){
		  	enviar_email( $remetente, $emailDem, $assunto, $conteudo, $emailCopia );
		  }
		  
		  //return true;
}

function enviaEmailValidacaoFinalizar() {

	/*
	 global $db;

	 //$emailCopia = 'wesleylira@mec.gov.br';
	 $emailCopia = '';

	 $dmdid = $_SESSION['dmdid'];

	 // Seta remetente
	 $remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	 // Pega email DEMANDANTE
	 $sql = "SELECT
	 (SELECT
	 cmddsc
	 FROM
	 workflow.comentariodocumento
	 WHERE
	 docid = d.docid
	 ORDER BY
	 cmdid DESC
	 LIMIT 1) as cmddsc,
	 to_char(dmddatainclusao, 'YYYY') AS ano,
	 CASE
	 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
	 ELSE u.usuemail
	 END AS emaildemandante,
	 CASE
	 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
	 ELSE u.usunome
	 END AS demandante,
	 d.dmdid,
	 d.dmdtitulo as assunto,
	 d.dmddsc as descricao,
	 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 od.orddescricao ||' / '|| ts.tipnome as origem,
	 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
	 upper(unasigla)||' - '||unadescricao as setor,
	 loc.lcadescricao as edificio,
	 aa.anddescricao AS andar,
	 d.dmdsalaatendimento as sala,
	 sd.siddescricao as sistema
	 FROM
	 demandas.demanda d
	 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
	 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
	 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
	 LEFT JOIN seguranca.usuario u ON d.usucpfdemandante = u.usucpf
	 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
	 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
	 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
	 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
	 WHERE
	 dmdid = {$dmdid}";

	 $dado = $db->pegaLinha($sql);

	 // Seta remetente/ano
	 $emailDem = $dado['emaildemandante'];
	 $ano	  = $dado['ano'];

	 $cmddsc = $dado['cmddsc'];


	 // seta dados da demanda
	 $dadoDemanda['ID'] 				  = $dado['dmdid'];
	 $dadoDemanda['Origem da demanda'] = $dado['origem'];

	 if ($dado['sistema'])
	 $dadoDemanda['Sistema']		  = $dado['sistema'];

	 $dadoDemanda['Assunto']			  = $dado['assunto'];
	 $dadoDemanda['Descricão']		  = $dado['descricao'];
	 $dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');


	 // $dadoDemanda = array(
	 // "ID"					   => $dado['dmdid'],
	 // "Origem da demanda" 	   => $dado['origem'],
	 // "Sistema"				   => $dado['sistema'],
	 // "Assunto" 			 	   => $dado['assunto'],
	 // "Descricão" 		 	   => $dado['descricao'],
	 // "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 // );

	 // seta dados do demandante
	 $dadoDemandante = array (
	 "Solicitante" => $dado['demandante'],
	 "Telefone"    => $dado['fone'],
	 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
	 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
	 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
	 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')
	 );
	 // Busca arquivos
	 $sql = "SELECT
	 arqnome||'.'||arqextensao AS arquivo,
	 to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
	 FROM
	 demandas.anexos a
	 INNER JOIN public.arquivo ar ON ar.arqid = a.arqid
	 WHERE
	 a.dmdid = {$dmdid}";

	 $dadoArquivo = $db->carregar($sql);
	 $dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	 array(
	 "arquivo" => '-',
	 "data"	=> '-'
	 )
	 );
	 // Busca observações
	 $sql = "SELECT
	 obsdsc AS descricao,
	 usunome AS usuario
	 FROM
	 demandas.observacoes o
	 INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf
	 WHERE
	 o.obsstatus = 'A' AND
	 dmdid = {$dmdid}";

	 $dadoObs = $db->carregar($sql);
	 $dadoObs = $dadoObs ? $dadoObs : array(
	 array(
	 "descricao" => '-',
	 "usuario"	=> '-'
	 )
	 );

	 // Seta assunto
	 $assunto = "Demanda – [{$dmdid}] – Finalizada e Validada pelo Gestor/Analista MEC";

	 // Seta Conteúdo
	 $conteudo = textMail("Demanda [{$dmdid}] finalizada e validada pelo Gestor/Analista MEC. <br> <font color='black'>Serviço Executado pelo Técnico: {$cmddsc} </font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

	 // Seta conteúdo
	 //	$conteudo = "A sua demanda [{$dmdid}/{$ano}] foi atendida com sucesso. Demanda finalizada.";

	 //	echo "$remetente<BR>$emailDem<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
	 //	exit;
	 enviar_email( $remetente, $emailDem, $assunto, $conteudo, $emailCopia );
	 return true;

	 */
	return true;

}


function enviaEmailInvalidacaoFinalizar() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email DEMANDANTE
	$sql = "SELECT
				(SELECT
					cmddsc
				 FROM 
					workflow.comentariodocumento
				 WHERE
				 	docid = d.docid
				 ORDER BY
					cmdid DESC
				 LIMIT 1) as cmddsc,	
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 CASE 
			  WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			  ELSE u.usuemail
			 END AS emaildemandante,
			 CASE 
			  WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			  ELSE u.usunome
 			 END AS demandante,				
			 u2.usuemail as emailtec, 
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
			 LEFT JOIN seguranca.usuario u ON d.usucpfdemandante = u.usucpf 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
			 dmdid = {$dmdid}";

	$dado = $db->pegaLinha($sql);

	// Seta remetente/ano
	$emailTec = $dado['emailtec'];
	$ano	  = $dado['ano'];

	$cmddsc = $dado['cmddsc'];


	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto = "Demanda – [{$dmdid}] – Finalizada e Invalidada pelo Gestor/Analista MEC";

		  // Seta Conteúdo
		  $conteudo = textMail("Demanda [{$dmdid}] finalizada e invalidada pelo Gestor/Analista MEC. <br> <font color='black'>Justificativa: {$cmddsc} </font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

		  // Seta conteúdo
		  //	$conteudo = "A sua demanda [{$dmdid}/{$ano}] foi atendida com sucesso. Demanda finalizada.";

		  //	echo "$remetente<BR>$emailTec<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  //	exit;
		  enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );
		  return true;
}


function enviaEmailValidacaoForaPrazo() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email DEMANDANTE
	$sql = "SELECT
				(SELECT
					cmddsc
				 FROM 
					workflow.comentariodocumento
				 WHERE
				 	docid = d.docid
				 ORDER BY
					cmdid DESC
				 LIMIT 1) as cmddsc,	
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 CASE 
			  WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			  ELSE u.usuemail
			 END AS emaildemandante,
			 CASE 
			  WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			  ELSE u.usunome
 			 END AS demandante,				
			 u2.usuemail as emailtec, 
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
			 LEFT JOIN seguranca.usuario u ON d.usucpfdemandante = u.usucpf 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
			 dmdid = {$dmdid}";

	$dado = $db->pegaLinha($sql);

	// Seta remetente/ano
	$emailTec 	= $dado['emailtec'];
	$emailCopia = $dado['emaildemandante'];
	$ano	  	= $dado['ano'];

	$cmddsc 	= $dado['cmddsc'];


	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto = "Demanda – [{$dmdid}] – Finalizada e Validada Fora do Prazo pelo Gestor/Analista MEC";

		  // Seta Conteúdo
		  $conteudo = textMail("Demanda [{$dmdid}] finalizada e validada fora do prazo pelo Gestor/Analista MEC. <br> <font color='black'>Justificativa: {$cmddsc} </font>", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

		  // Seta conteúdo
		  //	$conteudo = "A sua demanda [{$dmdid}/{$ano}] foi atendida com sucesso. Demanda finalizada.";

		  //	echo "$remetente<BR>$emailTec<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  //	exit;
		  enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );
		  return true;
}


function enviaEmailAvaliacaoRertorno() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email analista/gerente | ano | justificativa
	$sql = "SELECT
			 --cmddsc,
			 (SELECT cmddsc FROM workflow.comentariodocumento
			  WHERE docid = d.docid
			  ORDER BY cmdid DESC
			  LIMIT 1
			 ) as cmddsc,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u3.usuemail AS analista,
			 --u2.usuemail AS gerente,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
	 		 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid 
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid
			 /*
			 LEFT JOIN (SELECT
							cmdid,
							cmddsc,
							docid
						 FROM 
							workflow.comentariodocumento
						 ORDER BY
							cmdid DESC
						 LIMIT 1) cd ON cd.docid = d.docid
			 */
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			 --LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
			 --												   ur.rpustatus = 'A' AND
			 --												   ur.pflcod = ".DEMANDA_PERFIL_COORDENADOR."	
			 --LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf  
			 LEFT JOIN seguranca.usuario u ON d.usucpfdemandante = u.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			 WHERE
			 dmdid = {$dmdid}";	

	$dado = (array) $db->pegaLinha($sql);

	// Seta Destinatário = analista | Cópia = gerente
	$ano		  = $dado['ano'];
	$destinatario = $dado['analista'];


	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21'){
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}


	$just		  = $dado['cmddsc'];

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto = "Demanda [{$dmdid}] – Retorno para validação";

		  // Seta Conteúdo
		  $conteudo = textMail("Demanda [{$dmdid}] foi devolvida para validação pelo demandante solicitando a sua validação.<BR>Segue abaixo a justificativa:<BR>{$just}", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);

		  //	// Seta conteúdo
		  //	$conteudo = "A demanda [{$dmdid}/{$ano}] foi devolvida pelo demandante solicitando a sua validação.<BR>
		  //				 Segue abaixo a justificativa:<BR>{$just}";

		  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
		  return true;
}


function enviaEmailObservacao(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') AS dtini,
	 		 to_char(d.dmddatafimprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') AS dtfim,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
	 		 u2.usuemail as emailtec,
	 		 u2.usunome as nometec,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema 
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$emailTec 			= $dado['emailtec'];
	$nomeTec  			= $dado['nometec'];
	$emailDemandante  	= $dado['emaildemandante'];
	$ano	  			= $dado['ano'];

	if($emailTec) $emailCopia = $emailTec;


	// 	seta dados da addlinha
	//$addLinha['Previsão de atendimento']  = 'Início: '. ($dado['dtini'] ? $dado['dtini'] : '<B>-</B>').'<br>até<br>Término: '. ($dado['dtfim'] ? $dado['dtfim'] : '<B>-</B>');
	//$addLinha['Técnico Responsável'] = $nomeTec;
		

	// seta dados da demanda
	$dadoDemanda['Código']			  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);

	// Busca observações
	$sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

	$dadoObs = $db->carregar($sql);
	$dadoObs = $dadoObs ? $dadoObs : array(
	array(
		  "descricao" => '-',
		  "usuario"	=> '-',
	  	  "data" => '-'	 
	  	  )
	  	  );
	  	   
	  	   

	  	  // Seta assunto
	  	  $assunto  = "Demanda nº {$dmdid} – foi adicionada uma observação";
	  	  // Seta Conteúdo
	  	  $dadoArquivo = null;
	  	  $conteudo = textMail("DEMANDA Nº {$dmdid} <br> Foi adicionada uma observação. Veja no campo Observações abaixo.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);
	  	   
	  	  //para o demandante
	  	  enviar_email( $remetente, $emailDemandante, $assunto, $conteudo, $emailCopia );
	  	   
	  	  return true;

}


function enviaEmailRespostaAvaliacao($avdid){
	global $db;

	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);
	
	
	
	$sql = "SELECT 	(CASE avdgeral
						 	WHEN '1' THEN '<center>RUIM</center>'
						 	WHEN '2' THEN '<center>REGULAR</center>'
						 	WHEN '3' THEN '<center>BOM</center>'
						 	WHEN '4' THEN '<center>ÓTIMO</center>'
						END) AS avdgeral,
						(CASE WHEN avsobs is not null THEN
						 	 	' - ' || avsobs
						END) AS avsobs,
				 		u.usunome as nometec,
		 				avdresposta AS resposta
				FROM demandas.avaliacaodemanda a
				left join seguranca.usuario u on u.usucpf = a.usucpftecnico
				where avdid = {$avdid}";	
	$dado2 = (array) $db->pegaLinha($sql);

	$nomeTec  			= $dado2['nometec'];
	
	// 	seta dados da addlinha
	$addLinha['Sua avaliação']  = $dado2['avdgeral'] . $dado2['avsobs'];
	$addLinha['Resposta do Técnico'] = $dado2['resposta'] . "<br><b>Técnico: {$nomeTec}</b>";

	
	
	

	// Pega email TÉCNICO e DEMANDANTE
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') AS dtini,
	 		 to_char(d.dmddatafimprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') AS dtfim,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema 
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$emailDemandante  	= $dado['emaildemandante'];
	$ano	  			= $dado['ano'];
	
	
	// seta dados da demanda
	$dadoDemanda['Código']			  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  		= $dado['sistema'];

	$dadoDemanda['Assunto']			  	= $dado['assunto'];
	$dadoDemanda['Descricão']		  	= $dado['descricao'];


	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);

	// Busca observações
	$sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

	$dadoObs = $db->carregar($sql);
	$dadoObs = $dadoObs ? $dadoObs : array(
	array(
		  "descricao" => '-',
		  "usuario"	=> '-',
	  	  "data" => '-'	 
	  	  )
	  	  );
	  	   
	  	   

	  	  // Seta assunto
	  	  $assunto  = "Demanda nº {$dmdid} – foi adicionada uma resposta para sua avaliação";
	  	  // Seta Conteúdo
	  	  $dadoArquivo = null;
	  	  $conteudo = textMail("DEMANDA Nº {$dmdid} <br> foi adicionada uma resposta para sua avaliação.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);
	  	   
	  	  //para o demandante
	  	  enviar_email( $remetente, $emailDemandante, $assunto, $conteudo, $emailCopia );
	  	   
	  	  return true;

}



function enviaEmailTempoAdicional(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') AS dtini,
	 		 to_char(d.dmddatafimprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') AS dtfim,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
	 		 u2.usuemail as emailtec,
	 		 u2.usunome as nometec,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 to_char(d.dmdtempoadicional,'HH24:MI') as dmdtempoadicional,
			 d.dmdobstempoadicional 
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	$emailTec 			= $dado['emailtec'];
	$nomeTec  			= $dado['nometec'];
	$emailDemandante  	= $dado['emaildemandante'];
	$ano	  			= $dado['ano'];

	if($emailTec) $emailCopia = $emailTec;

	$addTempo = $dado['dmdtempoadicional'];


	// 	seta dados da addlinha
	$addLinha['Tempo Adicional']  = $addTempo . ' hr(s)';
	$addLinha['Justificativa'] = $dado['dmdobstempoadicional'];
	$addLinha['Previsão de atendimento']  = 'Início: '. ($dado['dtini'] ? $dado['dtini'] : '<B>-</B>').'<br>até<br>Término: '. ($dado['dtfim'] ? $dado['dtfim'] : '<B>-</B>');
	$addLinha['Técnico Responsável'] = $nomeTec;
		

	// seta dados da demanda
	$dadoDemanda['Código']			  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);

	// Busca observações
	$sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

	$dadoObs = $db->carregar($sql);
	$dadoObs = $dadoObs ? $dadoObs : array(
	array(
		  "descricao" => '-',
		  "usuario"	=> '-',
	  	  "data" => '-'	 
	  	  )
	  	  );
	  	   
	  	   

	  	  // Seta assunto
	  	  $assunto  = "Demanda nº {$dmdid} – foi adicionada $addTempo hr(s) na previsão final de atendimento";
	  	  // Seta Conteúdo
	  	  $dadoArquivo = null;
	  	  $conteudo = textMail("DEMANDA Nº {$dmdid} <br> Foi adicionada $addTempo hr(s) na previsão final de atendimento.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);
	  	   
	  	  //para o demandante
	  	  enviar_email( $remetente, $emailDemandante, $assunto, $conteudo, $emailCopia );
	  	   
	  	  return true;

}




function enviaEmailMaterial(){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email analista/gerente e ano
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 --u3.usuemail AS analista,
			 --u2.usuemail AS gerente,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 u4.usunome AS nometecnico,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 m.mtrdsc || ' - ' || tm.tpmdsc as material,
			 to_char(cm.ctmdatahora,'DD/MM/YYYY HH24:MI') AS datamaterial,
			 d.dmdqtde as qtd
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN seguranca.usuario u4 ON u4.usucpf = d.usucpfexecutor
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			 LEFT JOIN demandas.controlematerial AS cm ON cm.dmdid = d.dmdid and cm.ctmstatus='A'
			 LEFT JOIN demandas.tipomaterial AS tm ON tm.tpmid = cm.tpmid
			 LEFT JOIN demandas.material AS m ON m.mtrid = tm.mtrid
			WHERE
			 d.dmdid = {$dmdid}";	

	$dado 		  = (array) $db->pegaLinha($sql);


	$nometecnico = $dado['nometecnico'];

	//pega o email dos perfis DEMANDA_PERFIL_DEPOSITO_DTI
	//origem diferente de sistema
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21'){
		$sqlx = "select DISTINCT u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					INNER JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GESTOR_MEC."
				 	INNER JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf
				 	INNER join seguranca.usuario_sistema us on u2.usucpf = us.usucpf and us.susstatus = 'A' AND us.suscod = 'A'		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		//$gerenteMec = implode("; ", $dadox);
			
		$destinatario   = $dadox;
	}
	/*
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	*/


	$ano		  = $dado['ano'];


	// 	seta dados da addlinha
	$addLinha['Técnico Responsável'] = $nometecnico;
	$addLinha['Material Solicitado']  = ($dado['material'] ? $dado['material'] : '<B>-</B>');
	$addLinha['Qtd. Solicitado']  = ($dado['qtd'] ? $dado['qtd'] : '<B>-</B>');
	$addLinha['Data/Hora Solicitada']  = ($dado['datamaterial'] ? $dado['datamaterial'] : '<B>-</B>');
	



	// seta dados da demanda
	$dadoDemanda['Código'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] 	  = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  	  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
				obsdsc AS descricao,
				usunome AS usuario,
				to_char(obsdata,'DD/MM/YYYY HH24:MI') as data	
			FROM 
				demandas.observacoes o
				INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
			WHERE	
				o.obsstatus = 'A' AND
				o.obslog is null AND
				dmdid = {$dmdid}
			ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
		  "data" => '-'	 
		  )
		  );

		  // Seta assunto
		  $assunto = "Demanda Nº {$dmdid} – Solicitação de Material";

		  $textoconteudo = "DEMANDA Nº {$dmdid} - Solicitação de Material";

		  // Seta Conteúdo
		  $dadoArquivo = null;
		  $conteudo = textMail($textoconteudo, $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);
		  //	$conteudo = "A demanda [{$dmdid}/{$ano}] foi finalizada pelo responsável pelo atendimento.";

		  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
		  if($destinatario){
		  	enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
		  }
		  
		  return true;


}



function enviaEmailValidacaoDemandante() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email demandante
	$sql = "SELECT
			 --cmddsc,
			 --dthst,
			 (SELECT (SELECT b.cmddsc FROM workflow.comentariodocumento b WHERE b.hstid = max(a.hstid)) 
              FROM workflow.historicodocumento a 
              WHERE a.aedid in (146, 191,224, 703) and docid=d.docid --atendimento técnico finalizado
             ) as cmddsc,
			 (SELECT to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') 
              FROM workflow.historicodocumento a 
              WHERE a.aedid in (146, 191,224, 703) and docid=d.docid --atendimento técnico finalizado
             ) as dthst,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u3.usuemail AS analista,
			 --u2.usuemail AS gerente,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			 /*
			 LEFT JOIN (select a.hstid, a.docid, c.cmddsc, to_char(a.htddata::timestamp,'DD/MM/YYYY HH24:MI') as dthst  
						from 	workflow.historicodocumento a
							inner join workflow.documento d on d.docid = a.docid and d.tpdid in (31,35)
							inner join workflow.comentariodocumento c on c.hstid = a.hstid
							INNER JOIN workflow.historicodocumento b ON b.hstid = d.hstid
							-- INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento where aedid in (146, 191)  GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (146, 191) -- atendimento técnico finalizado
					group by a.hstid, a.docid, c.cmddsc, a.htddata) as ate ON ate.docid = d.docid	
			 */			 			 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
			 dmdid = {$dmdid}";	

	$dado 		  = (array) $db->pegaLinha($sql);

	// Seta Destinatário
	$destinatario = $dado['emaildemandante'];
	//$destinatario = "alexpereira@mec.gov.br";

	
	/*
	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1'){
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	*/

	$ano		  = $dado['ano'];

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Data do atendimento finalizado'] = $dado['dthst'];
	$dadoDemanda['Serviço executado pelo técnico'] = $dado['cmddsc'];
	$dadoDemanda['Data do envio para validação'] = date("d/m/Y H:i");

	

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
	  "descricao" => '-',
	  "usuario"	=> '-',
	  "data" => '-'	 
	  )
	  );

	  // Seta assunto
	  $assunto = "Demanda – [{$dmdid}] – Enviado para validação do demandante";

	  // Seta Conteúdo
	  $conteudo = textMail("Demanda [{$dmdid}] foi enviada para sua validação. <br> Favor, entrar no sistema e validar esta demanda.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);
	  //	$conteudo = "A demanda [{$dmdid}/{$ano}] foi finalizada pelo responsável pelo atendimento.";

	  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";

	  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
	   
	  return true;
}


function enviaEmailRetornoAguardValidacao() {
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email analista/gerente e ano
	$sql = "SELECT
			 --cmddsc,
			 --dthst,
			 (SELECT (SELECT b.cmddsc FROM workflow.comentariodocumento b WHERE b.hstid = max(a.hstid)) 
              FROM workflow.historicodocumento a 
              WHERE a.aedid in (704) and docid=d.docid --atendimento técnico finalizado
             ) as cmddsc,
			 (SELECT to_char(max(a.htddata)::timestamp,'HH24:MI') || ' hr(s) - ' || to_char(max(a.htddata)::timestamp,'DD/MM/YYYY') 
              FROM workflow.historicodocumento a 
              WHERE a.aedid in (704) and docid=d.docid --atendimento técnico finalizado
             ) as dthst,
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI') AS iniprevatendimento,
	 		 to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI') AS fimprevatendimento,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
	 		 od.ordid as codorigem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
			 u3.usuemail AS analista,
			 --u2.usuemail AS gerente,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema
			FROM
			 demandas.demanda d 
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 			 
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			 /*
			 LEFT JOIN (select a.hstid, a.docid, c.cmddsc, to_char(a.htddata::timestamp,'DD/MM/YYYY HH24:MI') as dthst  
						from 	workflow.historicodocumento a
							inner join workflow.documento d on d.docid = a.docid and d.tpdid in (31,35)
							inner join workflow.comentariodocumento c on c.hstid = a.hstid
							INNER JOIN workflow.historicodocumento b ON b.hstid = d.hstid
							-- INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento where aedid in (704)  GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (704) -- Retornar para aguardando validação
					group by a.hstid, a.docid, c.cmddsc, a.htddata) as ate ON ate.docid = d.docid
			 */				 			 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			WHERE
			 dmdid = {$dmdid}";	

	$dado 		  = (array) $db->pegaLinha($sql);

	// Seta Destinatário
	$destinatario = $dado['analista'];
	//$destinatario = "alexpereira@mec.gov.br";

	
	/*
	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1'){
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	else{ //origem = sistema
		$sqlx = "select u2.usuemail from demandas.demanda d
					INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
					LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
				 												   ur.rpustatus = 'A' AND
				 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
				 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
				 	WHERE
				 		d.dmdid = {$dmdid}";
		$dadox = (array) $db->carregarColuna($sqlx);
		$gerente = implode("; ", $dadox);
			
		$emailCopia   = $gerente;
	}
	*/

	$ano		  = $dado['ano'];

	// seta dados da demanda
	$dadoDemanda['ID'] 				  = $dado['dmdid'];
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	$dadoDemanda['Data do retorno'] = $dado['dthst'];
	$dadoDemanda['Justificativa do demandante'] = $dado['cmddsc'];
	
	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
															  "arquivo" => '-',
															  "data"	=> '-'	 
															  )
															  );
															  // Busca observações
															  $sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			o.obslog is null AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

															  $dadoObs = $db->carregar($sql);
															  $dadoObs = $dadoObs ? $dadoObs : array(
															  array(
	  "descricao" => '-',
	  "usuario"	=> '-',
	  "data" => '-'	 
	  )
	  );

	  // Seta assunto
	  $assunto = "Demanda – [{$dmdid}] – Retornado para aguardando validação";

	  // Seta Conteúdo
	  $conteudo = textMail("Demanda [{$dmdid}] foi retornada para sua revalidação. <br> Favor, entrar no sistema e revalidar esta demanda.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs);
	  //	$conteudo = "A demanda [{$dmdid}/{$ano}] foi finalizada pelo responsável pelo atendimento.";

	  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";

	  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
	   
	  return true;
}




function enviaEmailAvaliacaoRuim($dmdid, $tipoAvaliacao, $avaliacao){
	global $db;

	//$emailCopia = 'wesleylira@mec.gov.br';
	$emailCopia = '';

	$dmdid = $_SESSION['dmdid'];

	// Seta remetente
	$remetente = array("nome"=>REMETENTE_WORKFLOW_NOME, "email"=>REMETENTE_WORKFLOW_EMAIL);

	// Pega email TÉCNICO e ANO
	$sql = "SELECT
			 d.dmdid,
			 d.dmdtitulo as assunto,
			 d.dmddsc as descricao,
			 to_char(d.dmddatainiprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY') AS dtini,
	 		 to_char(d.dmddatafimprevatendimento,'HH24:MI') || ' hr(s) - ' || to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY') AS dtfim,
	 		 od.orddescricao ||' / '|| ts.tipnome as origem,
			 to_char(dmddatainclusao, 'YYYY') AS ano,
	 		 u2.usuemail as emailtec,
	 		 u2.usunome as nometec,
			CASE 
			 WHEN d.dmdemaildemandante != '' THEN d.dmdemaildemandante
			 ELSE u.usuemail
			END AS emaildemandante,
			CASE 
			 WHEN d.dmdnomedemandante	!= '' THEN d.dmdnomedemandante
			 ELSE u.usunome
			END AS demandante,
			 '(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
			 upper(unasigla)||' - '||unadescricao as setor, 	
			 loc.lcadescricao as edificio,
			 aa.anddescricao AS andar,
			 d.dmdsalaatendimento as sala,
			 sd.siddescricao as sistema,
			 od.ordid as codorigem
			FROM
			 demandas.demanda d
			 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
			 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
			 LEFT JOIN demandas.sistemadetalhe sd ON sd.sidid = d.sidid	
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante 		 
			 LEFT JOIN seguranca.usuario u2 ON d.usucpfexecutor = u2.usucpf 
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid  
			 LEFT JOIN demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN demandas.localatendimento AS loc ON loc.lcaid = l.lcaid			 
			WHERE
			 d.dmdid = {$dmdid}";

	$dado = (array) $db->pegaLinha($sql);

	//$emailTec 			= $dado['emailtec'];
	$nomeTec  			= $dado['nometec'];
	//$emailDemandante  	= $dado['emaildemandante'];
	$ano	  			= $dado['ano'];

	// 	seta dados da addlinha
	$addLinha['Avaliação do Atendimento'] = $tipoAvaliacao;
	$addLinha['Justificativa da Avaliação'] = str_replace(chr(13), '<br>', $avaliacao);
	$addLinha['Previsão de atendimento']  = 'Início: '. ($dado['dtini'] ? $dado['dtini'] : '<B>-</B>').'<br>até<br>Término: '. ($dado['dtfim'] ? $dado['dtfim'] : '<B>-</B>');
	$addLinha['Técnico Responsável'] = $nomeTec;
	
	
	
	//pega o email do gerente
	//origem diferente de sistema
	if($dado['codorigem'] != '1' && $dado['codorigem'] != '18' && $dado['codorigem'] != '19' && $dado['codorigem'] != '20' && $dado['codorigem'] != '21' && $dado['codorigem'] != '23'){
		
		$sqlSuporteAtend = " UNION ALL
			 	             SELECT 'avaliacaosimec@mec.gov.br' ";
		//4=Logistica e 8=banco de dados
		if($dado['codorigem'] == '4' || $dado['codorigem'] == '8') $sqlSuporteAtend = "";
		
		/*
		$sqlx = "select distinct u2.usuemail from demandas.demanda d
				INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
				LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.ordid = ts.ordid AND
			 												   ur.rpustatus = 'A' AND
			 												   ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR."
			 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
			 	WHERE
			 		d.dmdid = {$_SESSION['dmdid']}
			 	$sqlSuporteAtend 	
			 	";
		*/
		if($dado['codorigem'] == '3'){
			$sqlx = "select distinct u.usuemail from seguranca.usuario u
					 inner JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf AND 
	                                ur.rpustatus = 'A' AND 
	                                ur.pflcod = ".DEMANDA_PERFIL_ADMINISTRADOR." AND
	                                ur.ordid='{$dado['codorigem']}' AND u.usucpf != '' AND u.usucpf != ''";	 	
	        $destinatario = (array) $db->carregarColuna($sqlx);
		}
		//$gerente = implode("; ", $dadox);
		
		//$emailCopia = "servicedesk@mec.gov.br";
	}
	else{ //origem = sistema
		$sqlx = "select distinct u2.usuemail from demandas.demanda d
				INNER JOIN demandas.sistemacelula sc ON sc.sidid = d.sidid	
				LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = sc.celid AND
			 												   ur.rpustatus = 'A' AND
			 												   ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO."
			 	LEFT JOIN seguranca.usuario u2 ON u2.usucpf = ur.usucpf		
			 	WHERE
			 		d.dmdid = {$_SESSION['dmdid']} AND u2.usucpf != '' AND u2.usucpf != ''";
		$destinatario = (array) $db->carregarColuna($sqlx);
		//$gerente = implode(";", $dadox);
		
		//$emailCopia = $gerente;
	}		
	



	//pega id pai
	$descidpai = "";
	$idpai = $db->pegaUm("select dmdidorigem from demandas.demanda where dmdid = {$dmdid}");
	if($idpai) $descidpai = " - <b><font color=red>(Originado da demanda código: $idpai)</font></b>";

	// seta dados da demanda
	$dadoDemanda['Código']			  = $dado['dmdid'] . $descidpai;
	$dadoDemanda['Origem da demanda'] = $dado['origem'];

	if ($dado['sistema'])
	$dadoDemanda['Sistema']		  = $dado['sistema'];

	$dadoDemanda['Assunto']			  = $dado['assunto'];
	$dadoDemanda['Descricão']		  = $dado['descricao'];
	//$dadoDemanda['Previsão de atendimento'] = ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>');

	/*
	 $dadoDemanda = array(
	 "ID"					   => $dado['dmdid'],
	 "Origem da demanda" 	   => $dado['origem'],
	 "Sistema"				   => $dado['sistema'],
	 "Assunto" 			 	   => $dado['assunto'],
	 "Descricão" 		 	   => $dado['descricao'],
	 "Previsão de atendimento" => ($dado['iniprevatendimento'] ? $dado['iniprevatendimento'] : '<B>-</B>').' à '.($dado['fimprevatendimento'] ? $dado['fimprevatendimento'] : '<B>-</B>')
	 );*/

	// seta dados do demandante
	$dadoDemandante = array (
    						 "Solicitante" => $dado['demandante'],
    						 "Telefone"    => $dado['fone'],
     						 "Setor" 	   => ($dado['setor'] ? $dado['setor'] : '<B>-</B>'),
    						 "Edifício"    => ($dado['edificio'] ? $dado['edificio'] : '<B>-</B>'),
    						 "Andar" 	   => ($dado['andar'] ? $dado['andar'] : '<B>-</B>'),
    						 "Sala" 	   => ($dado['sala'] ? $dado['sala'] : '<B>-</B>')	 			
	);
	// Busca arquivos
	$sql = "SELECT
				arqnome||'.'||arqextensao AS arquivo,
				to_char((arqdata || ' ' || arqhora::time)::timestamp,'DD/MM/YYYY HH24:MI') AS data
			FROM 
				demandas.anexos a
				INNER JOIN public.arquivo ar ON ar.arqid = a.arqid 
			WHERE	
				a.dmdid = {$dmdid}"; 

	$dadoArquivo = $db->carregar($sql);
	$dadoArquivo = $dadoArquivo ? $dadoArquivo : array(
	array(
		  "arquivo" => '-',
		  "data"	=> '-'	 
		  )
		  );

		  // Busca observações
		  $sql = "SELECT
			obsdsc AS descricao,
			usunome AS usuario,
			to_char(obsdata,'DD/MM/YYYY HH24:MI') as data
		FROM 
			demandas.observacoes o
			INNER JOIN seguranca.usuario u ON u.usucpf = o.usucpf 
		WHERE	
			o.obsstatus = 'A' AND
			dmdid = {$dmdid}
		ORDER BY obsid DESC"; 		

		  $dadoObs = $db->carregar($sql);
		  $dadoObs = $dadoObs ? $dadoObs : array(
		  array(
		  "descricao" => '-',
		  "usuario"	=> '-',
	  	  "data" => '-'	 
	  	  )
	  	  );
	  	   
	  	   

	  	  // Seta assunto
	  	  
	  	  $assunto  = "Demanda [{$dmdid}] foi avaliada como {$tipoAvaliacao}.";

	  	  // Seta Conteúdo
	  	  $conteudo = textMail("Demanda [{$dmdid}] foi avaliada como {$tipoAvaliacao}.", $dadoDemanda, $dadoDemandante, $dadoArquivo, $dadoObs, $addLinha);

	  	  if($destinatario){
	  	  	enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
	  	  }
	  	   
	  	  return true;

}



// Funções do estado "Em Análise"
function regraAnaliseFinalizar() {
	global $db;

	$dmdid = $_SESSION['dmdid'];

	if (!$dmdid){
		return false;
	}

	$where = pegaSidid($dmdid) ? " dmdclassificacaosistema IS NOT NULL AND " : "";

	$sql = "SELECT
			 COUNT(dmdid)
			FROM
			 demandas.demanda
			WHERE
			 dmddatainiprevatendimento IS NOT NULL AND
			 dmddatafimprevatendimento IS NOT NULL AND
			 priid IS NOT NULL AND
			 usucpfexecutor IS NOT NULL AND
			 dmdclassificacao IS NOT NULL AND
			 {$where}
			 dmdid = {$dmdid};";
			 return (bool) $db->pegaUm($sql);
}


// Funções do estado "Aguardando valiação"
function regraAvaliacaoFinalizar() {
	global $db;

	if($_SESSION['flagfinalizaembloco']){
		$_SESSION['flagfinalizaembloco'] = "";
		return true;
	}

	$dmdid = $_SESSION['dmdid'];
	$perfil   = arrayPerfil();

	$varaux = 0;

	//executa comandos abaixo quando perfil for demandante
	/*
	 if ( in_array(DEMANDA_PERFIL_DEMANDANTE, $perfil) && count($perfil) == 1){

		if ( recuperaEstadoDocumento() != DEMANDA_ESTADO_AGUARDANDO_AVALIACAO && recuperaEstadoDocumento() != DEMANDA_GENERICO_ESTADO_AGUARDANDO_AVALIACAO ){
		$varaux = 0;
		return (bool) $varaux;
		}

		//verifica se avaliação esta preenchida
		$sql = "SELECT
		count(d.dmdid)
		FROM
		demandas.demanda d
		INNER JOIN demandas.avaliacaodemanda avd ON avd.dmdid = d.dmdid
		WHERE
		d.dmdid = {$dmdid} AND
		avd.avsobs IS NOT NULL";
		$varaux = $db->pegaUm($sql);
		return (bool) $varaux;
		}
		*/

	//verifica o setor do gestor ou analista pode validar
	if (  in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO,$perfil) || in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfil) || in_array(DEMANDA_PERFIL_DBA,$perfil) || in_array(DEMANDA_PERFIL_GESTOR_REDES,$perfil) || in_array(DEMANDA_PERFIL_GERENTE_PROJETO,$perfil) || in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA,$perfil) || in_array(DEMANDA_PERFIL_ANALISTA_FNDE,$perfil) || in_array(DEMANDA_PERFIL_ANALISTA_WEB,$perfil) || in_array(DEMANDA_PERFIL_GESTOR_EQUIPE,$perfil) ){

		//verifica origem e celula
		$sql = "SELECT t.ordid, 
					   case when s.celid is null then
								d.celid
							else
								s.celid
						end as celid
		 		FROM demandas.demanda d
				LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid	 			
				LEFT JOIN demandas.sistemacelula s ON s.sidid = d.sidid
		 		WHERE d.dmdid=".$_SESSION['dmdid'];
		$dados = $db->pegaLinha($sql);

		if($dados['celid']) $flagcel = " OR celid = ".$dados['celid'];

		if($dados['ordid']){
			$sql = "SELECT distinct usucpf
					FROM demandas.usuarioresponsabilidade
					WHERE rpustatus = 'A'
					AND pflcod in (".DEMANDA_PERFIL_GESTOR_MEC.",
									".DEMANDA_PERFIL_DBA.",
									".DEMANDA_PERFIL_GESTOR_REDES.",
									".DEMANDA_PERFIL_GESTOR_EQUIPE.",
									".DEMANDA_PERFIL_GERENTE_PROJETO.",
									".DEMANDA_PERFIL_ANALISTA_SISTEMA.",
									".DEMANDA_PERFIL_ANALISTA_FNDE.",
									".DEMANDA_PERFIL_ANALISTA_WEB.",
									".DEMANDA_PERFIL_DEMANDANTE_AVANCADO.") 
					AND usucpf = '".$_SESSION['usucpf']."'
					AND (ordid = ".$dados['ordid']."
					$flagcel ) ";
					$dados2 = $db->pegaUm($sql);
						
					if($dados2){
						$varaux = 1;
					}
					else{
						$varaux = 0;
					}

		}
		else{
			//se origem for null
			$varaux = 0;
		}
	}

	//super usuário
	if (in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfil)){
		$varaux = 1;
	}

	//verifica se existem demandas relacionadas que não foram finalizadas e canceladas
	if($varaux == 1){

		$sql = "SELECT count(d.dmdid)
				FROM demandas.demanda d 
				left JOIN workflow.documento dc ON dc.docid = d.docid and dc.tpdid in (31,35) 
				WHERE d.dmdidorigem = ".$dmdid." 
				AND (dc.esdid not in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.",".DEMANDA_ESTADO_VALIDADA_FORA_PRAZO.") or dc.esdid is null)";
		$qtd = $db->pegaUm($sql);

		if($qtd > 0) $varaux = 0;
		else $varaux = 1;

	}

	return (bool) $varaux;

}


// Funções do estado "Aguardando valiação"
function regraFinalizaDemandante() {
	global $db;


	$dmdid = $_SESSION['dmdid'];
	$usucpf = $_SESSION['usucpf'];
	$perfil   = arrayPerfil();
	
	if(!$dmdid || !$usucpf) {
		return false;
	}

	$varaux = 0;


	//verifica se o proprio demandante
	$sql = "SELECT usucpfdemandante FROM demandas.demanda WHERE dmdid = ".$dmdid." and usucpfdemandante = '".$usucpf."'";
	$usucpfdemandante = $db->pegaUm($sql);

	if($usucpfdemandante) $varaux = 1;
	else $varaux = 0;
		

	//super usuário
	if (in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfil)){
		$varaux = 1;
	}
	

	//verifica se existem demandas relacionadas que não foram finalizadas e canceladas
	if($varaux == 1){

		$sql = "SELECT count(d.dmdid)
				FROM demandas.demanda d 
				left JOIN workflow.documento dc ON dc.docid = d.docid and dc.tpdid in (31,35) 
				WHERE d.dmdidorigem = ".$dmdid." 
				AND (dc.esdid not in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.",".DEMANDA_ESTADO_VALIDADA_FORA_PRAZO.",".DEMANDA_ESTADO_AUDITADO.",".DEMANDA_GENERICO_ESTADO_AUDITADO.") or dc.esdid is null)";
		$qtd = $db->pegaUm($sql);

		if($qtd > 0) $varaux = 0;
		else $varaux = 1;

	}

	return (bool) $varaux;

}


function regraAuditarDemanda() {
	global $db;

	return true;

	$dmdid = $_SESSION['dmdid'];
	$usucpf = $_SESSION['usucpf'];
	$perfil   = arrayPerfil();
	
	if(!$dmdid || !$usucpf) {
		return false;
	}

	$varaux = 0;

	//verifica se  éobrigatorio preencher nº SS do módulo fabrica
	$sql = "select scsid from demandas.demanda where dmdid = $dmdid";
	$exite = $db->pegaUm($sql);

	if($exite){
		$varaux = 1;
	}else{
		$varaux = 0;
	}

	return (bool) $varaux;

}


function regraRetornaAguardValidDemandante() {
	global $db;


	$dmdid = $_SESSION['dmdid'];
	$usucpf = $_SESSION['usucpf'];
	$perfil   = arrayPerfil();
	
	if(!$dmdid || !$usucpf) {
		return false;
	}

	$varaux = 0;


	//verifica se o proprio demandante
	$sql = "SELECT usucpfdemandante FROM demandas.demanda WHERE dmdid = ".$dmdid." and usucpfdemandante = '".$usucpf."'";
	$usucpfdemandante = $db->pegaUm($sql);

	if($usucpfdemandante) $varaux = 1;
	else $varaux = 0;
		

	//super usuário
	if (in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfil)){
		$varaux = 1;
	}

	return (bool) $varaux;

}


function regraCancelarDemanda(){
	global $db;

	$dmdid = $_SESSION['dmdid'];

	$varaux = 0;

	//verifica se existem demandas relacionadas que não foram finalizadas e canceladas
	$sql = "SELECT count(d.dmdid)
			FROM demandas.demanda d 
			left JOIN workflow.documento dc ON dc.docid = d.docid and dc.tpdid in (31,35) 
			WHERE d.dmdidorigem = ".$dmdid." 
			AND (dc.esdid not in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.",".DEMANDA_ESTADO_VALIDADA_FORA_PRAZO.") or dc.esdid is null)";
	$qtd = $db->pegaUm($sql);

	if($qtd > 0) $varaux = 0;
	else $varaux = 1;

	return (bool) $varaux;

}


function regraFinalizaAtendimento(){
	global $db;

	$dmdid = $_SESSION['dmdid'];
	
	if(!$dmdid) {
		echo '<script>alert("Sessão expirou! Acesse novamente a demanda.");
					  location.href="demandas.php?modulo=principal/lista&acao=A";
			  </script>';
		die;
	}

	$varaux = 0;

	//verifica se exite pausa
	$sql = "SELECT count(dmdid)
			FROM demandas.pausademanda  
			WHERE dmdid = ".$dmdid." 
			AND pdmstatus = 'A' 
			AND pdmdatafimpausa is null";
	$qtd = $db->pegaUm($sql);

	if($qtd > 0){
		$varaux = 0;
		return (bool) $varaux;
	}
	else{
		$varaux = 1;
	}


	//verifica se obrigatorio preencher nº serie / patrimonio
	$sql = "select t.tipnumserie as nserie, t.tipnumpatr as npatrimonio
			from demandas.demanda d
			LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
			where dmdid = $dmdid
			";
	$exite = $db->carregar($sql);

	if($exite[0]['nserie'] == 't' || $exite[0]['npatrimonio'] == 't'){
		$sql = "SELECT count(dmdid)
				FROM demandas.itemhardwaredemanda  
				WHERE dmdid = ".$dmdid; 
		$qtd = $db->pegaUm($sql);

		if($qtd > 0) {
			$varaux = 1;
		}
		else{
			$varaux = 0;
		}
	}


	return (bool) $varaux;
}

function regraReabrirDemanda(){
	global $db;

	$dmdid = $_SESSION['dmdid'];

	$varaux = 0;

	//verifica se existem demandas relacionadas que não foram finalizadas e canceladas
	$sql = "SELECT count(d.dmdid)
			FROM demandas.demanda d 
			left JOIN workflow.documento dc ON dc.docid = d.docid and dc.tpdid in (31,35) 
			WHERE d.dmdidorigem = ".$dmdid." 
			AND (dc.esdid not in (".DEMANDA_ESTADO_FINALIZADO.",".DEMANDA_ESTADO_CANCELADO.",".DEMANDA_GENERICO_ESTADO_FINALIZADO.",".DEMANDA_GENERICO_ESTADO_CANCELADO.",".DEMANDA_ESTADO_INVALIDADA.",".DEMANDA_GENERICO_ESTADO_INVALIDADA.",".DEMANDA_ESTADO_VALIDADA_FORA_PRAZO.") or dc.esdid is null)";
	$qtd = $db->pegaUm($sql);

	if($qtd > 0) $varaux = 0;
	else $varaux = 1;

	return (bool) $varaux;
}


function regraPagarMemorando(){
	global $db;
	
	if($_SESSION['flagpagamentoembloco']){
		return true;
	}else{
		return false;
	}
	
}


/*******************
 * FIM funções de verificação e pós-ação
 ******************/

/****************************
 *
 * FIM FUNÇÕES DO WORWFLOW
 *
 ****************************/

/**************
 * Função que monta texto do email, no formato HTML.
 *
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @param  $msg (text) Texto principal do email
 * @param  $dadoDemanda (array) o índice será o label e o valor será texto que ficará à frente do label.
 * @param  $dadoDemandante (array) o índice será o label e o valor será texto que ficará à frente do label.
 * @param  $dadoArquivo (array) Será um array de array, onde os índices do array interno serão "arquivo" e "data".
 * @param  $dadoObs (array) Será um array de array, onde os índices do array interno serão "descricao" e "usuario".
 * @return (text) Texto no formato HTML;
 * @example textMail(
 * 					  'A demanda [15-2009] foi atribuida a você',
 *                    array("Data:"=>"12-10-2009"),
 *                    array("Solicitante:"=>"Felipe..."),
 *                    array(
 *                    		array("arquivo" => "arquivo.doc",
 *                    		"data" => "12/10/2009")
 *                    		),
 *                    array(
 *                    		array("descricao" => "Observações feitas...",
 *                    		"usuario" => "Felipe...")
 *                    		)
 *                   );
 *
 **************/
// textMail('A demanda [15-2009] foi atribuida a você', array("Data:"=>"12-10-2009"));
function textMail($msg=null, $dadoDemanda = array("" => ""), $dadoDemandante = array("-" => "-"), $dadoArquivo = array(array("arquivo" => "-", "data" => "-")), $dadoObs = array(array("descricao" => "-", "usuario" => "-", "data" => "-")), $addLinha=null ){
	$text = '<html xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:w="urn:schemas-microsoft-com:office:word" xmlns:m="http://schemas.microsoft.com/office/2004/12/omml" xmlns="http://www.w3.org/TR/REC-html40">
			<head>
			<style>
			.table_mail{
			    width: 80%;
			    border:outset #0099CC 2px;
			}
			
			.tit_td{
				background:#6699CC; 
				font-size:10.0pt;
				font-family:"Arial","sans-serif";
				color:white;
				font-weight: bold;
				text-align: center;
				border-bottom: 1px solid white;
				margin:2px;
			}
			
			.item_1{			
				font-size:9.0pt;
				text-align: right;
				font-weight: bold;
				font-family:"Arial","sans-serif";
				padding: 3px;
				padding-right: 5px;
				border-right: 1px solid white;
			}
			
			.item_2{
				font-size:9.0pt;			
				text-align: left;
				font-family:"Arial","sans-serif";
				padding-left: 3px;
				border-right: 1px solid white;	
			}
			
			</style>
			</head>
			<body lang=PT-BR link=blue vlink=purple>
			    <table cellpadding="1" cellspacing="0" class="table_mail">
			    	<tr>
			    		<td class="tit_td" colspan="4">'.$msg.'</td>
			    	</tr>';
	if($addLinha){
		$a = 0;
		foreach($addLinha as $ind=>$val){
			$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
			    		<td class="item_1" colspan="2" nowrap width="40%">'.$ind.':</td>
			    		<td class="item_2" colspan="2">'.$val.'</td>
			    	  </tr>'; 
			$a++;
		}
	}

	$text .= '     	<tr>
			    		<td class="tit_td" colspan="4">Dados da Demanda</td>
			    	</tr>';
	$a = 0;
	foreach($dadoDemanda as $ind=>$val){
		$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
		    		<td class="item_1" colspan="2" nowrap width="40%">'.$ind.':</td>
		    		<td class="item_2" colspan="2">'.$val.'</td>
		    	  </tr>'; 
		$a++;
	}

	$text .= ' <tr>
		    		<td class="tit_td" colspan="4">Dados do Solicitante</td>
		    	</tr>
		    	<TR>
		    		<TD colspan="4">
		    			<table width="100%" border="0" cellpadding="0" cellspacing="0">';    	
	$a = 0;
	while (list($key, $val) = each($dadoDemandante)) {
		$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
		    		<td class="item_1" width="20%">'.$key.'</td>
		    		<td class="item_2" width="30%">'.$val.'</td>';

		list($key, $val) = each($dadoDemandante);

		$text .='
		    		<td class="item_1" width="20%">'.($key ? $key : '-').'</td>
		    		<td class="item_2" width="30%">'.($val ? $val : '-').'</td>
		    	  </tr>'; 
		$a++;
	}

	$text .= '
					    </table>    	   
		    		</td>
		    	</tr>';

	if($dadoArquivo){
		$text .= '
			      	<tr>
			    		<td class="tit_td" colspan="4">Dados do(s) Anexo(s)</td>
			    	</tr>
			    	<TR>
			    		<TD colspan="4">
			    			<table width="100%" border="0" cellpadding="0" cellspacing="0">    	
						    	<tr>
						    		<td class="tit_td" colspan="2">ARQUIVO</td>
						    		<td class="tit_td" colspan="2">
										DATA/HORA<BR>
										(xx-xx-xxxx xx:xx)
									</td>
						    	</tr>';
		$a = 0;
		foreach($dadoArquivo as $val){
			$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
			    		<td class="item_1" colspan="2">'.$val['arquivo'].'</td>
			    		<td class="item_2" colspan="2">'.$val['data'].'</td>
			    	  </tr>'; 
			$a++;
		}

		$text .= '
					    </table>    	   
		    		</td>
		    	</tr>';
	}

	if($dadoObs){
		$text .= '
			    	<tr>
			    		<td class="tit_td" colspan="4">Observações</td>
			    	</tr>';
		$a = 0;
		foreach($dadoObs as $val){
			$textColor = "";
			if($a == 0) $textColor = "blue";
			$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
			    		<td class="item_2" colspan="4"><font color="'.$textColor.'">'.$val['descricao'].'</font></td>
			    	  </tr>
					  <tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
			    		<td class="item_2" colspan="4"><B><font color="'.$textColor.'">AUTOR: '.$val['usuario'].'<br>DATA: '.$val['data'].'</font></B></td>
			    	  </tr>		    	  '; 
			$a++;
		}
	}

	$text .= '
					<tr>
			    		<td class="tit_td" colspan="4"> >> <a target="_blank" href="http://simec.mec.gov.br">CLIQUE AQUI</a> PARA ACOMPANHAR A DEMANDA</a> </td>
			    	</tr>
		    </table>
		</body>
		</html>';

	return $text;
}


function textMailAud($msg=null, $dadoReserva = array("" => ""), $dadoSolicitante = array("-" => "-"), $dadoAgenda = array(array("data" => "-", "turno" => "-")) ){
	$text = '<html xmlns:v="urn:schemas-microsoft-com:vml" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:w="urn:schemas-microsoft-com:office:word" xmlns:m="http://schemas.microsoft.com/office/2004/12/omml" xmlns="http://www.w3.org/TR/REC-html40">
			<head>
			<style>
			.table_mail{
			    width: 80%;
			    border:outset #0099CC 2px;
			}
			
			.tit_td{
				background:#6699CC; 
				font-size:10.0pt;
				font-family:"Arial","sans-serif";
				color:white;
				font-weight: bold;
				text-align: center;
				border-bottom: 1px solid white;
				margin:2px;
			}
			
			.item_1{			
				font-size:9.0pt;
				text-align: right;
				font-weight: bold;
				font-family:"Arial","sans-serif";
				padding: 3px;
				padding-right: 5px;
				border-right: 1px solid white;
			}
			
			.item_2{
				font-size:9.0pt;			
				text-align: left;
				font-family:"Arial","sans-serif";
				padding-left: 3px;
				border-right: 1px solid white;	
			}
			
			</style>
			</head>
			<body lang=PT-BR link=blue vlink=purple>
			    <table cellpadding="1" cellspacing="0" class="table_mail">
			    	<tr>
			    		<td class="tit_td" colspan="6">RESERVA DE AUDITÓRIO <br> <font size=1>'.$msg.'</font></td>
			    	</tr>
			      	<tr>
			    		<td class="tit_td" colspan="4">Dados da Reserva</td>
			    	</tr>';
	$a = 0;
	foreach($dadoReserva as $ind=>$val){
		$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
		    		<td class="item_1" colspan="2" nowrap width="40%">'.$ind.':</td>
		    		<td class="item_2" colspan="2">'.$val.'</td>
		    	  </tr>'; 
		$a++;
	}
	$text .= ' <tr>
		    		<td class="tit_td" colspan="4">Dados do Solicitante</td>
		    	</tr>
		    	<TR>
		    		<TD colspan="4">
		    			<table width="100%" border="0" cellpadding="0" cellspacing="0">';    	
	$a = 0;
	foreach($dadoSolicitante as $ind=>$val){
		$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
		    		<td class="item_1" colspan="2" nowrap width="40%">'.$ind.':</td>
		    		<td class="item_2" colspan="2">'.$val.'</td>
		    	  </tr>'; 
		$a++;
	}

	$text .= '
					    </table>    	   
		    		</td>
		    	</tr>				    	
		      	<tr>
		    		<td class="tit_td" colspan="4">Dados do Agendamento</td>
		    	</tr>
		    	<TR>
		    		<TD colspan="4">
		    			<table width="100%" border="0" cellpadding="0" cellspacing="0">    	
					    	<tr>
					    		<td class="tit_td" colspan="2">DATA</td>
					    		<td class="tit_td" colspan="2">TURNO</td>
					    	</tr>';
	$a = 0;
	$find = array("1","2","3");
	foreach($dadoAgenda as $val){
		$text .= '<tr bgcolor="'.(is_int($a/2) ? '#EFEFEF' : '#DFDFDF').'">
		    		<td class="item_1" colspan="2" width="50%"><div align="center">'.$val['data'].'</div></td>
		    		<td class="item_2" colspan="2" ><div align="center"><b>'.str_replace($find,"",$val['turno']).'</b></div></td>
		    	  </tr>'; 
		$a++;
	}

	$text .= '
					    </table>    	   
		    		</td>
		    	</tr>';
	$text .= '
		    </table>
		</body>
		</html>';

	return $text;
}


function enviaEmailReservaAud($evaid,$acao) {
	global $db;


	$sql = "SELECT  e.evaid, e.evaevento,
					(CASE e.evalocal
					    WHEN 'S' THEN 'SEDE (110 cadeiras fixas)'
					    WHEN 'A' THEN 'ANEXO (109 cadeiras fixas)' 
					 END) AS local, 
					 e.evanumpart, e.ungcod, e.evastatus, e.usucpf, 
		            e.evaprojetor, e.evacomput, e.evatelao, e.evamicrofone, e.evainternet, 
		            e.evaobs,
					u.usunome AS solicitante,
					'(' || u.usufoneddd || ') ' || u.usufonenum AS fone,
					u.usuemail as usuemail,
					upper(ug.ungabrev)||' - '||ug.ungdsc as orgao		            
			FROM  demandas.eventoauditorio e 
		    LEFT JOIN seguranca.usuario u ON u.usucpf = e.usucpf 
		    LEFT JOIN public.unidadegestora ug ON ug.ungcod = e.ungcod
			WHERE e.evaid = $evaid";

	$dado = (array) $db->pegaLinha($sql);

	$remetente = array("nome"=>"SIMEC - MÓDULO DE RESERVA DE AUDITÓRIO", "email"=>"auditorio@mec.gov.br");
	//$remetente = $dado['usuemail'];
	$destinatario = $dado['usuemail'];
	$emailCopia = 'auditorio@mec.gov.br';


	if($dado['evaprojetor'] == 'S') $equip .= " - Projetor &nbsp;&nbsp;&nbsp;";
	if($dado['evatelao'] == 'S') $equip .= " - Telão &nbsp;&nbsp;&nbsp;";
	if($dado['evacomput'] == 'S') $equip .= " - Computador para Projeção &nbsp;&nbsp;&nbsp;";
	if($dado['evamicrofone'] == 'S') $equip .= " - Microfones &nbsp;&nbsp;&nbsp;";
	if($dado['evainternet'] == 'S') $equip .= " - Acesso a Internet &nbsp;&nbsp;&nbsp;";



	// seta dados da demanda
	$dadoReserva['ID'] 				  	= $dado['evaid'];
	$dadoReserva['LOCAL']			  	= $dado['local'];
	$dadoReserva['Nome do Evento'] 	  	= $dado['evaevento'];
	$dadoReserva['Órgão / Secretaria']	 = $dado['orgao'];
	$dadoReserva['Nº de Participantes']	  = $dado['evanumpart'];
	$dadoReserva['Equipamentos Necessários'] = $equip;


	// seta dados do demandante
	$dadoSolicitante = array (
    						 "Solicitante" => $dado['solicitante'],
    						 "Telefone"    => $dado['fone'],
     						 "Email" 	   => ($dado['usuemail'] ? $dado['usuemail'] : '<B>-</B>') 			
	);

	// Busca agenda
	$sql = "SELECT to_char(agadata::timestamp,'DD/MM/YYYY') AS data, '1Manhã' as turno
			FROM 
				demandas.agendaauditorio 
			WHERE
				agastatus = 'A'
				and aga_evaid_manha = {$evaid}

			UNION				

				SELECT to_char(agadata::timestamp,'DD/MM/YYYY') AS data, '2Almoço' as turno
				FROM 
					demandas.agendaauditorio 
				WHERE
					agastatus = 'A'
					and aga_evaid_almoco = {$evaid}
			
			UNION				
			
				SELECT to_char(agadata::timestamp,'DD/MM/YYYY') AS data, '3Tarde' as turno
				FROM 
					demandas.agendaauditorio 
				WHERE
					agastatus = 'A'
					and aga_evaid_tarde = {$evaid}
			
			order by 1,2	
							"; 

	$dadoAgenda = $db->carregar($sql);
	$dadoAgenda = $dadoAgenda ? $dadoAgenda : array(
	array(
															  "data" => '-',
															  "turno"	=> '-'	 
															  )
															  );

															  if($acao == 'I') $msg = "Inclusão de Evento";
															  if($acao == 'A') $msg = "Alteração de Evento";
															  if($acao == 'C') $msg = "Cancelamento de Evento";
															  	
															  // Seta assunto
															  $assunto = "Reserva de Auditório Nº {$evaid} - ({$msg})";

															  // Seta Conteúdo
															  $conteudo = textMailAud($msg, $dadoReserva, $dadoSolicitante, $dadoAgenda);

															  //	// Seta conteúdo
															  //	$conteudo = "A demanda [{$dmdid}/{$ano}] foi devolvida pelo demandante solicitando a sua validação.<BR>
															  //				 Segue abaixo a justificativa:<BR>{$just}";
															   
															  //echo $conteudo;
															  //exit;
															   
															  //echo "$remetente<BR>$destinatario<BR>$assunto<br>$conteudo<br>$emailCopia<BR>";
															  enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
															  return true;
}


/**************
 * Função que retorna o link para criar novas demandas.
 **************/
function criarNovaDemanda (){
	global $db;

	$dmdid  = $_SESSION['dmdid'];
	$perfil = arrayPerfil();

	if ( !$dmdid ){
		return;
	}

	if ( in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfil)    ||
	in_array(DEMANDA_PERFIL_COORDENADOR, $perfil)     ||
	in_array(DEMANDA_PERFIL_GERENTE_PROJETO, $perfil) ||
	in_array(DEMANDA_PERFIL_TECNICO1, $perfil) ||
	in_array(DEMANDA_PERFIL_TECNICO, $perfil) ||
	in_array(DEMANDA_PERFIL_GESTOR_MEC, $perfil) ||
	in_array(DEMANDA_PERFIL_GESTOR_EQUIPE, $perfil) ||
	in_array(DEMANDA_PERFIL_ANALISTA_SISTEMA, $perfil) ||
	in_array(DEMANDA_PERFIL_ANALISTA_FNDE, $perfil) ||
	in_array(DEMANDA_PERFIL_PROGRAMADOR, $perfil) ||
	in_array(DEMANDA_PERFIL_ADMINISTRADOR, $perfil) ||
	in_array(DEMANDA_PERFIL_GERENTE_FSW, $perfil) ||
	in_array(DEMANDA_PERFIL_ANALISTA_FSW, $perfil) ||
	in_array(DEMANDA_PERFIL_FISCAL_TECNICO_FSW, $perfil) ){




		//pega tipid para download de tutoriais
		$sql = "select tipid
				from demandas.demanda 
				where dmdid = $dmdid
				";
		$tipid = $db->pegaUm($sql);


		//pega tipnumserie / tipnumpatr para obrigar o tecnico digitar o nº de serie ou patrimonio
		if($tipid){
			$sql = "select tipnumserie, tipnumpatr, ordid
					from demandas.tiposervico 
					where tipid = $tipid
					";
				
			$dados = $db->carregar($sql);
			$ordid = $dados[0]['ordid'];
			$tipnumserie = $dados[0]['tipnumserie'];
			$tipnumpatr = $dados[0]['tipnumpatr'];
		}


		//detalhes da pausa

		//verifica pausa da demanda
		$sql = "select t.tpafimautomatico, p.pdmid, p.pdmdatainiciopausa, p.pdmdatafimpausa
				from demandas.pausademanda p 
				inner join demandas.tipopausademanda t ON t.tpaid=p.tpaid
				where p.pdmstatus = 'A' and p.dmdid = $dmdid
				ORDER BY pdmid DESC
				LIMIT 1
				";

		$dadosp = $db->pegaLinha($sql);

		if($dadosp){
			$pdmid = $dadosp['pdmid'];
			$pdmdatainiciopausa = $dadosp['pdmdatainiciopausa'];
			$pdmdatafimpausa = $dadosp['pdmdatafimpausa'];
			$tpafimautomatico = $dadosp['tpafimautomatico'];
				
			if($pdmdatainiciopausa && $pdmdatafimpausa){
				$dt_ini  = strtotime($pdmdatainiciopausa);
				$dt_fim  = strtotime($pdmdatafimpausa);
				$dt_hoje = strtotime(date('Y-m-d H:i:s'));

				if(($dt_fim > $dt_hoje) && $tpafimautomatico == 'T') $flag_datapause = 1;
			}
		}

		//demanda pausada
		if((!$pdmdatafimpausa && $pdmid) || $flag_datapause) {
			//echo "<table border='0' cellpadding='3' cellspacing='0' style='width: 80px;'><tr><td align='center'><FONT COLOR=RED size=2><B><div id='pauseid' onmouseover=\"return escape('Listar Demandas que ainda não foram iniciadas.');\">PAUSADO</div></B></FONT></td></tr></table>";
			echo "<table border='0' cellpadding='3' cellspacing='0' style='width: 80px;'><tr><td align='center'><FONT COLOR=RED size=2><B><div title=' ' id='pauseid' onmouseout=\"SuperTitleOff(this);\" onmousemove=\"SuperTitleAjax('demandas.php?modulo=principal/lista&acao=A&dmdidPausaAjax=".$dmdid."',this)\" >PAUSADO</div></B></FONT></td></tr></table>";
			?>
<style type="text/css">
.titulo1 {
	background-color: rgb(227, 227, 227);
	text-align: center;
	font-weight: bold;
}

.linha1 {
	background-color: #f5f5f5;
}

.linha2 {
	background-color: #fdfdfd;
}
</style>
<link
	rel="stylesheet" type="text/css" href="../includes/superTitle.css" />
<link
	rel='stylesheet' type='text/css' href='../includes/listagem.css' />
<script
	src="../includes/calendario.js"></script>
<script
	src="./js/ajax.js" type="text/javascript"></script>
<script
	src="./js/demandas.js" type="text/javascript"></script>
<script
	type="text/javascript" src="../includes/prototype.js"></script>
<script
	type="text/javascript" src="../includes/funcoes.js"></script>
<script
	type="text/javascript" src="../includes/remedial.js"></script>
<script
	type="text/javascript" src="../includes/superTitle.js"></script>
<script
	language="JavaScript" src="../includes/wz_tooltip.js"></script>
			<?
		}
		?>
<table border="0" cellpadding="3" cellspacing="0"
	style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
	<tr style="background-color: #c9c9c9; text-align: center;">
		<td style="font-size: 7pt; text-align: center;"><span
			title="estado atual"> <b>outras ações</b></span></td>
	</tr>
	<?
	// Pega o "estado do documento", vinculado à demanda
	$esdid = recuperaEstadoDocumento();
	//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
	if( in_array($esdid, array(DEMANDA_ESTADO_EM_ATENDIMENTO,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO)) ){
		?>
	<tr>
		<td style="font-size: 7pt; text-align: center; border-bottom: 2px solid #c9c9c9;" onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
			<?if($pdmdatafimpausa && $pdmid && !$flag_datapause){?>
				<a href="javascript:popPausaDemanda('');">Pausar Demanda</a> 
			<?}elseif(!$pdmdatafimpausa && !$pdmid){?>
				<a href="javascript:popPausaDemanda('');">Pausar Demanda</a> 
			<?}else{?>
				<div id="divRetomarAtend">
					<a href="javascript:popPausaDemanda('<?=$pdmid?>');">Retomar Atendimento</a>
				</div> 
			<?}?>
		</td>
	</tr>
	<?}?>
	<?
	if( in_array($esdid, array(DEMANDA_ESTADO_EM_ANALISE,DEMANDA_GENERICO_ESTADO_EM_ANALISE,DEMANDA_ESTADO_EM_ATENDIMENTO,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO)) ){
		?>
	<tr>
		<td style="font-size: 7pt; text-align: center;"
			onmouseover="this.style.backgroundColor='#ffffdd';"
			onmouseout="this.style.backgroundColor='';"><a
			href="javascript:popCadDemanda();">Criar Novas Demandas a partir
		desta</a></td>
	</tr>
	<?
	}

	//obriga o tecnico digitar o nº de serie / patrimonio
	if( ($tipnumserie == 't' || $tipnumpatr  == 't') && in_array($esdid, array(DEMANDA_ESTADO_EM_ATENDIMENTO,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO)) ){
		?>
	<tr>
		<td
			style="font-size: 7pt; text-align: center; border-top: 2px solid #c9c9c9;"
			onmouseover="this.style.backgroundColor='#ffffdd';"
			onmouseout="this.style.backgroundColor='';"><a
			href="javascript:popCadnSeriePatr();">Cadastrar <br>
		Nº Serie / Patrimonio</a></td>
	</tr>
	<?
	}
	?>
	<?if($pdmid){?>
	<tr>
		<td
			style="font-size: 7pt; text-align: center; border-top: 2px solid #c9c9c9;"
			onmouseover="this.style.backgroundColor='#ffffdd';"
			onmouseout="this.style.backgroundColor='';"><a
			href="javascript:popHistPausa('<?=$pdmid?>');">Histórico <br>da Pausa</a></td>
	</tr>
	<?}?>
	<tr>
		<td
			style="font-size: 7pt; text-align: center; border-top: 2px solid #c9c9c9;"
			onmouseover="this.style.backgroundColor='#ffffdd';"
			onmouseout="this.style.backgroundColor='';"><a
			href="javascript:popDownTut('<?=$tipid?>','<?=$ordid?>');">Download <br>
		(Tutoriais de Atendimento)</a></td>
	</tr>
</table>

<script>
				var dmdid = "<?=$dmdid?>";
		
				function popCadDemanda()
				{
					
					var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/demandas/demandas.php' +
						'?modulo=principal/popCadDemanda' +
						'&acao=A' +
						'&dmdid=' + dmdid;
					window.open(
						url,
						'cadDemanda',
						'width=700,height=460,scrollbars=yes,scrolling=no,resizebled=no'
					);
				}

				function popPausaDemanda(pdmid)
				{

					if(pdmid) document.getElementById("divRetomarAtend").innerHTML = "Aguarde...";
										
					var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/demandas/demandas.php' +
						'?modulo=principal/popPausaDemanda' +
						'&acao=A' +
						'&dmdid=' + dmdid +
						'&pdmid=' + pdmid;
					window.open(
						url,
						'pausaDemanda',
						'width=550,height=496,scrollbars=yes,scrolling=no,resizebled=no'
					);
					
				}

				function popHistPausa(pdmid)
				{
					
					var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/demandas/demandas.php' +
						'?modulo=principal/popHistoricoPausa' +
						'&acao=A' +
						'&dmdid=' + dmdid +
						'&pdmid=' + pdmid;
					window.open(
						url,
						'historicoPausa',
						'width=550,height=496,scrollbars=yes,scrolling=no,resizebled=no'
					);
				}
				
				function popCadnSeriePatr()
				{
					
					var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/demandas/demandas.php' +
						'?modulo=principal/nSeriePatrimonio' +
						'&acao=A' +
						'&dmdid=' + dmdid;
					window.open(
						url,
						'Cadnseriepatr',
						'width=600,height=400,scrollbars=yes,scrolling=no,resizebled=no'
					);
				}
				
				
				function popDownTut(tipid, ordid)
				{
					
					var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/demandas/demandas.php' +
						'?modulo=sistema/apoio/catalogoAnexo' +
						'&acao=A' +
						'&tipo=P' +
						'&tipid=' + tipid +
						'&ordid=' + ordid;
					window.open(
						url,
						'tutoriais',
						'width=700,height=400,scrollbars=yes,scrolling=no,resizebled=yes'
					);
				}

			</script>
	<?
	}

}




//função para calcular tempo no relatorio geral
function calculaTempoMinuto($dtini, $dtfim, $tipohorario = null, $ordid = null){
		
	//echo $dtini."<br>";
	//echo $dtfim."<br>";
	//echo "<br>";
		
	//pega o tempo total da demanda
	$ano_ini	= substr($dtini,0,4);
	$mes_ini	= substr($dtini,5,2);
	$dia_ini	= substr($dtini,8,2);
	$hor_ini	= substr($dtini,11,2);
	$min_ini	= substr($dtini,14,2);
	//$seg_ini	= substr($dtini,17,2);

	$ano_fim	= substr($dtfim,0,4);
	$mes_fim	= substr($dtfim,5,2);
	$dia_fim	= substr($dtfim,8,2);
	$hor_fim	= substr($dtfim,11,2);
	$min_fim	= substr($dtfim,14,2);
	//$seg_fim	= substr($dtfim,17,2);
		
	$dini = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data inicial
	$dfim = mktime($hor_fim,$min_fim,0,$mes_fim,$dia_fim,$ano_fim); // timestamp da data final
	//$tempototaldemanda = ($dfim - $dini);

	//pega total e o limite de horas por dia
	//$horapordia = 8;
	$minutopordia = 8*60;
	$limitehoraini = 8;
	$limitehorafim = 18;
	if($tipohorario == "T"){
		//$horapordia = 14;
		$minutopordia = 14*60;
		$limitehorafim = 22;
	}elseif($tipohorario == "A"){
		//$horapordia = 10;
		$minutopordia = 10*60;
	}elseif($tipohorario == "N"){
		//$horapordia = 12;
		$minutopordia = 12*60;
		$limitehorafim = 22;
	}
		
		
	// redes - flag atendimento para redes 24h
	if($ordid == '2'){
		$limitehoraini = 0;
		$limitehorafim = 24;
		$minutopordia = 24*60;
	}
		

	/*
	 if( (int)$hor_ini < 8){
	 $hor_fim = "08";
	 $min_fim = "00";
	 //$seg_fim = "00";
	 }
	 if( (int)$hor_fim > 18){
	 $hor_fim = "18";
	 $min_fim = "00";
	 }
	 */
		

	//calculo de tempo decorrido
	$dini = mktime(0,0,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data inicial
	$dfim = mktime(0,0,0,$mes_fim,$dia_fim,$ano_fim); // timestamp da data final
		
	$total_minuto = 0;
	$ct=0;
	$min_inix = 0;
		
	while($dini <= $dfim){//enquanto uma data for inferior a outra {

		$dt = date("d/m/Y",$dini);//convertendo a data no formato dia/mes/ano


		if(date("N",$dini) != 6 && date("N",$dini) != 7){ // sabado e domingo nao entra no loop
			 
			if($ct == 0 && $dini == $dfim){ // calculo 1 -> 1º registro e único registro - data inicio é a mesma da data fim

				 
				if( (int)$hor_ini < $limitehoraini){
					$hor_ini = $limitehoraini;
					$min_ini = 0;
				}
				if( (int)$hor_fim >= $limitehorafim){
					$hor_fim = $limitehorafim;
					$min_fim = 0;
				}

				//echo "<br>hor_ini = ".$hor_ini;
				//echo "<br>hor_fim = ".$hor_fim;
				//echo "<br>min_ini = ".$min_ini;
				//echo "<br>min_fim = ".$min_fim;
					
					
				//obs: hor_fim vai ser sempre maior que hor_ini
				if( (int)$min_ini > (int)$min_fim ) { //ex: 12:30 --- 13:10 = 40m
					$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
					$total_minuto = ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
				}else{
					$total_minuto = ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini) );
				}
					
				//echo "<br>tempo = ".$total_minuto;
					
				//hora_almoco
				if( ($tipohorario == "C" || !$tipohorario) ){

					if( (int)$hor_ini < 12 && (int)$hor_fim > 14){
						$total_minuto = $total_minuto - (2*60);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim > 14){ // ex: 12:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (1*60);
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
							
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 14){ //ex: 11:08 --- 14:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (2*60);
						//$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 13 && (int)$hor_fim > 14){ // ex: 13:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini < 12 && (int)$hor_fim == 12){ // ex: 11:00 --- 12:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 13){ //ex: 11:00 --- 13:15
						//echo $total_minuto." = okkkk1<br>";
							
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							//echo "resto=".$resto_min."<br>";
							$total_minuto = $total_minuto - $resto_min - $min_ini;
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto -  ((int)$min_ini + (int)$min_fim) + (int)$min_ini;
						}
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 12){
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini == 13 && (int)$hor_fim == 13){
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 13){
						//ex: 12:17 --- 13:16
						//echo $total_minuto." = okkkk1<br>";
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							$total_minuto = $total_minuto - ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto - ((int)$min_fim - (int)$min_ini) ;
						}
						//echo $total_minuto." = okkkk2<br>";
					}

				}


					
			}elseif($dini != $dfim){ // calculo 2 -> registros intermediarios - data inicio é diferente da data fim
				 

				if($ct == 0){
					 
					 
					if( (int)$hor_ini < $limitehoraini){
						$hor_ini = $limitehoraini;
						$min_ini = 0;
					}
					if( (int)$hor_ini >= $limitehorafim){
						$hor_fim = $limitehorafim;
						$min_fim = 0;
						$hor_ini = $limitehoraini;
						$min_ini = 0;
					}

					$hor_fim = $limitehorafim;
					$min_fim = 0;

					//echo "<br>hini = ".$hor_ini;
					//echo "<br>hfim = ".$hor_fim;
					//echo "<br>mini = ".$min_ini;
					//echo "<br>mfim = ".$min_fim;

					//obs: hor_fim vai ser sempre maior que hor_ini
					if( (int)$min_ini > (int)$min_fim ) { //ex: 12:30 --- 13:10 = 40m
						$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
						$total_minuto = ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
					}else{
						$total_minuto = ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini) );
					}
						

					//hora_almoco
					if( ($tipohorario == "C" || !$tipohorario) ){
							
						if( (int)$hor_ini < 12 && (int)$hor_fim > 14){
							$total_minuto = $total_minuto - (2*60);
						}
						elseif( (int)$hor_ini == 12 && (int)$hor_fim > 14){ // ex: 12:17 --- 15:16
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - (1*60);
							$total_minuto = $total_minuto - (60-$min_ini);
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";

						}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 14){ //ex: 11:08 --- 14:15
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - (2*60);
							//$total_minuto = $total_minuto - $min_fim;
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}
						elseif( (int)$hor_ini == 13 && (int)$hor_fim > 14){ // ex: 13:17 --- 15:16
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - (60-$min_ini);
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}
						elseif( (int)$hor_ini < 12 && (int)$hor_fim == 12){ // ex: 11:00 --- 12:15
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - $min_fim;
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 13){ //ex: 11:00 --- 13:15
							//echo $total_minuto." = okkkk1<br>";

							if( (int)$min_ini > (int)$min_fim ) {
								$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
								//echo "resto=".$resto_min."<br>";
								$total_minuto = $total_minuto - $resto_min - $min_ini;
							}else{ // ex: 12:00 --- 13:00
								$total_minuto = $total_minuto - (1*60);
								$total_minuto =  $total_minuto -  ((int)$min_ini + (int)$min_fim) + (int)$min_ini;
							}
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}
						elseif( (int)$hor_ini == 12 && (int)$hor_fim == 12){
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - ($min_fim-$min_ini);
							//echo $total_minuto." = okkkk2<br>";
						}elseif( (int)$hor_ini == 13 && (int)$hor_fim == 13){
							$total_minuto = $total_minuto - ($min_fim-$min_ini);
						}
						elseif( (int)$hor_ini == 12 && (int)$hor_fim == 13){
							//ex: 12:17 --- 13:16
							//echo $total_minuto." = okkkk1<br>";
							if( (int)$min_ini > (int)$min_fim ) {
								$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
								$total_minuto = $total_minuto - ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
							}else{ // ex: 12:00 --- 13:00
								$total_minuto = $total_minuto - (1*60);
								$total_minuto =  $total_minuto - ((int)$min_fim - (int)$min_ini) ;
							}
							//echo $total_minuto." = okkkk2<br>";
						}
							
					}

					//echo $dt." - ";
					//echo "tempo1 = ".$total_minuto."<br>";

				}else{
					 
					//verifica fim de semana
					//if(date("N",$dini) != 6 && date("N",$dini) != 7)
					$total_minuto = $total_minuto + $minutopordia;

					//echo $dt." - ";
					//echo "tempo2 = ".$total_minuto."<br>";
					 
				}
				//echo "<br>total_minuto = total_minuto + (hor_fim*60  - hor_ini*60  + (min_fim - min_ini)) - minutopordia = ";
				//echo "<br>total_minuto = $total_minuto + (". (int)$hor_fim*60 ." - ". (int)$hor_ini*60 ." + (".(int)$min_fim." - ".(int)$min_ini.")) - $minutopordia = ";
				//$total_minuto = $total_minuto + ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini)) - $minutopordia;
				//echo $total_minuto."<br>";
				 
			}elseif($dini == $dfim){ // calculo 3 -> último registro - data inicio é a mesma da data fim

				$hor_fim	= substr($dtfim,11,2);
				$min_fim	= substr($dtfim,14,2);
					

				if( (int)$hor_fim <= $limitehoraini){
					$hor_fim = $limitehoraini;
					$min_fim = 0;
				}
				if( (int)$hor_fim >= $limitehorafim){
					$hor_fim = $limitehorafim;
					$min_fim = 0;
					//$hor_ini = $limitehoraini;
					//$min_ini = 0;

				}
					
				$hor_ini = $limitehoraini;
				$min_ini = 0;

				//obs1: hor_fim vai ser sempre maior que hor_ini
				//obs2: min_fim vai ser sempre maior que min_ini

				//obs: hor_fim vai ser sempre maior que hor_ini
				if( (int)$min_ini > (int)$min_fim ) { //ex: 12:30 --- 13:10 = 40m
					$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
					$total_minuto = $total_minuto + ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
				}else{
					$total_minuto = $total_minuto + ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini) );
				}
					
					
				//hora_almoco
				if( ($tipohorario == "C" || !$tipohorario) ){

					if( (int)$hor_ini < 12 && (int)$hor_fim > 14){
						$total_minuto = $total_minuto - (2*60);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim > 14){ // ex: 12:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (1*60);
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
							
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 14){ //ex: 11:08 --- 14:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (2*60);
						//$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 13 && (int)$hor_fim > 14){ // ex: 13:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini < 12 && (int)$hor_fim == 12){ // ex: 11:00 --- 12:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 13){ //ex: 11:00 --- 13:15
						//echo $total_minuto." = okkkk1<br>";
							
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							//echo "resto=".$resto_min."<br>";
							$total_minuto = $total_minuto - $resto_min - $min_ini;
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto -  ((int)$min_ini + (int)$min_fim) + (int)$min_ini;
						}
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 12){
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini == 13 && (int)$hor_fim == 13){
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 13){
						//ex: 12:17 --- 13:16
						//echo $total_minuto." = okkkk1<br>";
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							$total_minuto = $total_minuto - ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto - ((int)$min_fim - (int)$min_ini) ;
						}
						//echo $total_minuto." = okkkk2<br>";
					}

				}

				//echo $dt." - ";
				//echo "tempo3 = ".$total_minuto."<br>";

			}



		}

		//echo $dt." - "; //exibindo a data
		//echo "week=".date("N",$dini)." - ";
		//echo "totminuto=".$total_minuto."<br>";
		 
		$dini += 86400; // adicionando mais 1 dia (em segundos) na data inicial
		$ct++;
	}
		
	return $total_minuto;
}


//função para calcular tempo no relatorio CGD
function calculaTempoMinutoCgd($dtini, $dtfim, $tipohorario = null, $ordid = null){
		
	//echo $dtini."<br>";
	//echo $dtfim."<br>";
	//echo "<br>";
		
	//pega o tempo total da demanda
	$ano_ini	= substr($dtini,6,4);
	$mes_ini	= substr($dtini,3,2);
	$dia_ini	= substr($dtini,0,2);
	$hor_ini	= substr($dtini,11,2);
	$min_ini	= substr($dtini,14,2);
	//$seg_ini	= substr($dtini,17,2);
	
	$ano_fim	= substr($dtfim,6,4);
	$mes_fim	= substr($dtfim,3,2);
	$dia_fim	= substr($dtfim,0,2);
	$hor_fim	= substr($dtfim,11,2);
	$min_fim	= substr($dtfim,14,2);
	//$seg_fim	= substr($dtfim,17,2);
		
	$dini = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data inicial
	$dfim = mktime($hor_fim,$min_fim,0,$mes_fim,$dia_fim,$ano_fim); // timestamp da data final
	//$tempototaldemanda = ($dfim - $dini);

	//pega total e o limite de horas por dia
	//$horapordia = 8;
	$minutopordia = 8*60;
	$limitehoraini = 8;
	$limitehorafim = 18;
	$existehoraalmoco = true;
	/*
	if($tipohorario == "T"){
		//$horapordia = 14;
		$minutopordia = 14*60;
		$limitehorafim = 22;
	}elseif($tipohorario == "A"){
		//$horapordia = 10;
		$minutopordia = 10*60;
	}elseif($tipohorario == "N"){
		//$horapordia = 12;
		$minutopordia = 12*60;
		$limitehorafim = 22;
	}
	*/
		
		
	// redes - flag atendimento para redes 24h
	if($ordid == '24'){
		$limitehoraini = 0;
		$limitehorafim = 24;
		$minutopordia = 24*60;
		$existehoraalmoco = false;
	}
		

	/*
	 if( (int)$hor_ini < 8){
	 $hor_fim = "08";
	 $min_fim = "00";
	 //$seg_fim = "00";
	 }
	 if( (int)$hor_fim > 18){
	 $hor_fim = "18";
	 $min_fim = "00";
	 }
	 */
		

	//calculo de tempo decorrido
	$dini = mktime(0,0,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data inicial
	$dfim = mktime(0,0,0,$mes_fim,$dia_fim,$ano_fim); // timestamp da data final
		
	$total_minuto = 0;
	$ct=0;
	$min_inix = 0;
		
	while($dini <= $dfim){//enquanto uma data for inferior a outra {

		$dt = date("d/m/Y",$dini);//convertendo a data no formato dia/mes/ano


		//if(date("N",$dini) != 6 && date("N",$dini) != 7){ // sabado e domingo nao entra no loop
			 
			if($ct == 0 && $dini == $dfim){ // calculo 1 -> 1º registro e único registro - data inicio é a mesma da data fim

				//echo $hor_ini .'<'. $limitehoraini.'<br>';
				//echo $hor_fim .'>='. $limitehorafim.'<br>';
				
				if( (int)$hor_ini < $limitehoraini){
					$hor_ini = $limitehoraini;
					$min_ini = 0;
				}
				if( (int)$hor_fim >= $limitehorafim){
					$hor_fim = $limitehorafim;
					$min_fim = 0;
				}
				
			
					
				//echo "<br>hor_ini = ".$hor_ini;
				//echo "<br>hor_fim = ".$hor_fim;
				//echo "<br>min_ini = ".$min_ini;
				//echo "<br>min_fim = ".$min_fim.'<br>';
					
					
				//obs: hor_fim vai ser sempre maior que hor_ini
				if( (int)$min_ini > (int)$min_fim ) { //ex: 12:30 --- 13:10 = 40m
					$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
					$total_minuto = ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
				}else{
					$total_minuto = ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini) );
				}
					
				//echo "<br>tempo = ".$total_minuto;
					
				//hora_almoco
				if( $existehoraalmoco && ($tipohorario == "C" || !$tipohorario) ){

					if( (int)$hor_ini < 12 && (int)$hor_fim > 14){
						$total_minuto = $total_minuto - (2*60);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim > 14){ // ex: 12:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (1*60);
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
							
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 14){ //ex: 11:08 --- 14:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (2*60);
						//$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 13 && (int)$hor_fim > 14){ // ex: 13:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini < 12 && (int)$hor_fim == 12){ // ex: 11:00 --- 12:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 13){ //ex: 11:00 --- 13:15
						//echo $total_minuto." = okkkk1<br>";
							
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							//echo "resto=".$resto_min."<br>";
							$total_minuto = $total_minuto - $resto_min - $min_ini;
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto -  ((int)$min_ini + (int)$min_fim) + (int)$min_ini;
						}
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 12){
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini == 13 && (int)$hor_fim == 13){
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 13){
						//ex: 12:17 --- 13:16
						//echo $total_minuto." = okkkk1<br>";
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							$total_minuto = $total_minuto - ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto - ((int)$min_fim - (int)$min_ini) ;
						}
						//echo $total_minuto." = okkkk2<br>";
					}

				}


					
			}elseif($dini != $dfim){ // calculo 2 -> registros intermediarios - data inicio é diferente da data fim
				 

				if($ct == 0){
					 
					//echo $hor_ini .'<'. $limitehoraini.'<br>';
					//echo $hor_ini .'>='. $limitehorafim.'<br>';
				
					if( (int)$hor_ini < $limitehoraini){
						$hor_ini = $limitehoraini;
						$min_ini = 0;
					}
					
					if( (int)$hor_ini >= $limitehorafim){
						$hor_fim = $limitehorafim;
						$min_fim = 0;
						$hor_ini = $limitehorafim;
						$min_ini = 0;
					}
					
					if( $existehoraalmoco ){
						if( (int)$hor_ini >= 12 && (int)$hor_ini<14){
							$hor_ini = 14;
							$min_ini = 0;
						}
					}
					
					$hor_fim = $limitehorafim;
					$min_fim = 0;
					/*
					$hor_fim = $limitehorafim;
					if( $existehoraalmoco ){
						$min_fim = $min_ini;
						$hor_ini = $limitehorafim;
					}else{
						$min_fim = 0;
					}
					*/
						
					//echo "<br>hini = ".$hor_ini;
					//echo "<br>hfim = ".$hor_fim;
					//echo "<br>mini = ".$min_ini;
					//echo "<br>mfim = ".$min_fim.'<br>';

					//obs: hor_fim vai ser sempre maior que hor_ini
					if( (int)$min_ini > (int)$min_fim ) { //ex: 12:30 --- 13:10 = 40m
						$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
						$total_minuto = ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
					}else{
						$total_minuto = ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini) );
					}
						

					//hora_almoco
					if( $existehoraalmoco && ($tipohorario == "C" || !$tipohorario) ){
							
						if( (int)$hor_ini < 12 && (int)$hor_fim > 14){
							$total_minuto = $total_minuto - (2*60);
						}
						elseif( (int)$hor_ini == 12 && (int)$hor_fim > 14){ // ex: 12:17 --- 15:16
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - (1*60);
							$total_minuto = $total_minuto - (60-$min_ini);
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";

						}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 14){ //ex: 11:08 --- 14:15
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - (2*60);
							//$total_minuto = $total_minuto - $min_fim;
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}
						elseif( (int)$hor_ini == 13 && (int)$hor_fim > 14){ // ex: 13:17 --- 15:16
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - (60-$min_ini);
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}
						elseif( (int)$hor_ini < 12 && (int)$hor_fim == 12){ // ex: 11:00 --- 12:15
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - $min_fim;
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 13){ //ex: 11:00 --- 13:15
							//echo $total_minuto." = okkkk1<br>";

							if( (int)$min_ini > (int)$min_fim ) {
								$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
								//echo "resto=".$resto_min."<br>";
								$total_minuto = $total_minuto - $resto_min - $min_ini;
							}else{ // ex: 12:00 --- 13:00
								$total_minuto = $total_minuto - (1*60);
								$total_minuto =  $total_minuto -  ((int)$min_ini + (int)$min_fim) + (int)$min_ini;
							}
							//$total_minuto = $total_minuto + $min_ini;
							//echo $total_minuto." = okkkk2<br>";
						}
						elseif( (int)$hor_ini == 12 && (int)$hor_fim == 12){
							//echo $total_minuto." = okkkk1<br>";
							$total_minuto = $total_minuto - ($min_fim-$min_ini);
							//echo $total_minuto." = okkkk2<br>";
						}elseif( (int)$hor_ini == 13 && (int)$hor_fim == 13){
							$total_minuto = $total_minuto - ($min_fim-$min_ini);
						}
						elseif( (int)$hor_ini == 12 && (int)$hor_fim == 13){
							//ex: 12:17 --- 13:16
							//echo $total_minuto." = okkkk1<br>";
							if( (int)$min_ini > (int)$min_fim ) {
								$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
								$total_minuto = $total_minuto - ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
							}else{ // ex: 12:00 --- 13:00
								$total_minuto = $total_minuto - (1*60);
								$total_minuto =  $total_minuto - ((int)$min_fim - (int)$min_ini) ;
							}
							//echo $total_minuto." = okkkk2<br>";
						}
							
					}

					//echo $dt." - ";
					//echo "tempo1 = ".$total_minuto."<br>";
					

				}else{
					 
					//verifica fim de semana
					//if(date("N",$dini) != 6 && date("N",$dini) != 7)
					$total_minuto = $total_minuto + $minutopordia;

					//echo $dt." - ";
					//echo "tempo2 = ".$total_minuto."<br>";
					 
				}
				//echo "<br>total_minuto = total_minuto + (hor_fim*60  - hor_ini*60  + (min_fim - min_ini)) - minutopordia = ";
				//echo "<br>total_minuto = $total_minuto + (". (int)$hor_fim*60 ." - ". (int)$hor_ini*60 ." + (".(int)$min_fim." - ".(int)$min_ini.")) - $minutopordia = ";
				//$total_minuto = $total_minuto + ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini)) - $minutopordia;
				//echo $total_minuto."<br>";
				 
			}elseif($dini == $dfim){ // calculo 3 -> último registro - data inicio é a mesma da data fim

				$hor_fim	= substr($dtfim,11,2);
				$min_fim	= substr($dtfim,14,2);
					

				if( (int)$hor_fim <= $limitehoraini){
					$hor_fim = $limitehoraini;
					$min_fim = 0;
				}
				if( (int)$hor_fim >= $limitehorafim){
					$hor_fim = $limitehorafim;
					$min_fim = 0;
					//$hor_ini = $limitehoraini;
					//$min_ini = 0;

				}
					
				$hor_ini = $limitehoraini;
				$min_ini = 0;

				//obs1: hor_fim vai ser sempre maior que hor_ini
				//obs2: min_fim vai ser sempre maior que min_ini

				//obs: hor_fim vai ser sempre maior que hor_ini
				if( (int)$min_ini > (int)$min_fim ) { //ex: 12:30 --- 13:10 = 40m
					$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
					$total_minuto = $total_minuto + ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
				}else{
					$total_minuto = $total_minuto + ( (int)$hor_fim*60 - (int)$hor_ini*60 + ((int)$min_fim - (int)$min_ini) );
				}
					
					
				//hora_almoco
				if( $existehoraalmoco && ($tipohorario == "C" || !$tipohorario) ){

					if( (int)$hor_ini < 12 && (int)$hor_fim > 14){
						$total_minuto = $total_minuto - (2*60);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim > 14){ // ex: 12:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (1*60);
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
							
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 14){ //ex: 11:08 --- 14:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (2*60);
						//$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 13 && (int)$hor_fim > 14){ // ex: 13:17 --- 15:16
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - (60-$min_ini);
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini < 12 && (int)$hor_fim == 12){ // ex: 11:00 --- 12:15
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - $min_fim;
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini < 12 && (int)$hor_fim == 13){ //ex: 11:00 --- 13:15
						//echo $total_minuto." = okkkk1<br>";
							
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							//echo "resto=".$resto_min."<br>";
							$total_minuto = $total_minuto - $resto_min - $min_ini;
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto -  ((int)$min_ini + (int)$min_fim) + (int)$min_ini;
						}
						//$total_minuto = $total_minuto + $min_ini;
						//echo $total_minuto." = okkkk2<br>";
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 12){
						//echo $total_minuto." = okkkk1<br>";
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
						//echo $total_minuto." = okkkk2<br>";
					}elseif( (int)$hor_ini == 13 && (int)$hor_fim == 13){
						$total_minuto = $total_minuto - ($min_fim-$min_ini);
					}
					elseif( (int)$hor_ini == 12 && (int)$hor_fim == 13){
						//ex: 12:17 --- 13:16
						//echo $total_minuto." = okkkk1<br>";
						if( (int)$min_ini > (int)$min_fim ) {
							$resto_min = (60 - (int)$min_ini) + (int)$min_fim;
							$total_minuto = $total_minuto - ( ((int)$hor_fim*60 - (int)$hor_ini*60)-60 + $resto_min );
						}else{ // ex: 12:00 --- 13:00
							$total_minuto = $total_minuto - (1*60);
							$total_minuto =  $total_minuto - ((int)$min_fim - (int)$min_ini) ;
						}
						//echo $total_minuto." = okkkk2<br>";
					}

				}

				//echo $dt." - ";
				//echo "tempo3 = ".$total_minuto."<br>";

			}



		//}

		//echo $dt." - "; //exibindo a data
		//echo "week=".date("N",$dini)." - ";
		//echo "totminuto=".$total_minuto."<br>";
		 
		$dini += 86400; // adicionando mais 1 dia (em segundos) na data inicial
		$ct++;
	}
		
	if($total_minuto<0) $total_minuto=0;
	
	return $total_minuto;
}



function verificaCalculoTempoPrioridade($priidx, $dtinix, $tempoadicional){
	global $db;
	
	$sql = sprintf("SELECT 
					 ( (case when d.dmdqtde > 0 then 
					 		case when char_length(((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar) = 1 then
								'0' || ((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar
							else
								((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar
						    end
						    || ':' ||
						    lpad(mod((d.dmdqtde*tspminuto),60)::varchar,2,'0') 
					 	else 
					 		(case when t.ordid in (18,19,20,21) then
							 	lpad(tsphora::varchar,3,'0') || ':' || lpad(tspminuto::varchar,2,'0') 
							 else
							 	lpad(tsphora::varchar,2,'0') || ':' || lpad(tspminuto::varchar,2,'0')
							 end)
					 	end) 
					 ) as tsptempo, 
					 d.dmdhorarioatendimento
					FROM 
					 	demandas.demanda d
					 	inner join demandas.tiposervicoprioridade p ON p.tipid = d.tipid
					 	LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					WHERE
					 d.dmdid = %d
					 AND p.priid = %d 
					 and p.tspstatus = 'A'
					", 
					$_SESSION['dmdid'],
					$priidx);
	$dados = $db->PegaLinha($sql);
	
	//verifica se existe severidade
	if(!$dados['tsptempo']){
		//echo "";
		//exit;
		$dados['tsptempo'] = "00:00";	
	}


	if(!$tempoadicional) $tempoadicional = "00:00";
	$hord = (int) substr($tempoadicional,0,2);
	$mind = (int) substr($tempoadicional,3,2);
	
	$tempoadd = explode(":",$dados['tsptempo']);
	$horb = (int) $tempoadd[0];
	$minb = (int) $tempoadd[1];
	//$tempoadd = $dados['tsptempo'];
	//$horb = (int) substr($tempoadd,0,2);
	//$minb = (int) substr($tempoadd,3,2);

	$hort = $horb + $hord;
	$mint = $minb + $mind;

	if($mint>=60){
		$hort = $hort + 1;
		$mint = $mint - 60;
	}

	if(strlen($hort) == 1) $hort = "0".$hort;
	if(strlen($mint) == 1) $mint = "0".$mint;
		
	$tempoadd = $hort.":".$mint.":00";
	$tipohorario = $dados['dmdhorarioatendimento'];
	
	$limitehoraini = 8;
	$limitehorafim = 18;
	$minutopordia = 8*60;
	$existehoraalmoco = false;
	if($tipohorario == "T"){
		$limitehorafim = 22;
		$existehoraalmoco = true;
		$minutopordia = 14*60;
	}elseif($tipohorario == "A"){
		$existehoraalmoco = true;
		$minutopordia = 10*60;
	}elseif($tipohorario == "N"){
		$limitehorafim = 22;
		$minutopordia = 12*60;
	}
	
	
	//flag atendimento para redes 24h
	$sql = "SELECT
				t.ordid
	 		FROM
	 			demandas.demanda d
			LEFT JOIN 
				demandas.tiposervico t ON t.tipid = d.tipid	 			
	 		WHERE 
	 			d.dmdid=".$_SESSION['dmdid'];
	$ordid = $db->PegaUm($sql);	

	if($ordid == '2'){ // redes
		$limitehoraini = 0;
		$limitehorafim = 24;
		$minutopordia = 24*60;
		$existehoraalmoco = true;
	}
	
	
	
	
	//pega data inicio
	if($dtinix){
		$dia_ini = substr($dtinix,0,2);
		$mes_ini = substr($dtinix,3,2);
		$ano_ini = substr($dtinix,6,4); 
		$hor_ini = substr($dtinix,11,2);
		$min_ini = substr($dtinix,14,2);
	}
	else{
		$ano_ini = date("Y");
		$mes_ini = date("m");
		$dia_ini = date("d");	
		$hor_ini = date("H");
		$min_ini = date("i");
	}
	
	
	$diaaux = 0;
	if($hor_ini < $limitehoraini){
		$hor_ini = $limitehoraini;
		$min_ini = 0;
	}
	if( ($hor_ini == 12 || $hor_ini == 13) && $existehoraalmoco == false){
		$hor_ini = 14;
		$min_ini = 0;
	}	
	//if( ($hor_ini > $limitehorafim) || ($hor_ini == $limitehorafim && $min_ini > 0) ){
	if($hor_ini >= $limitehorafim){
		$hor_ini = 8;
		$min_ini = 0;
		$diaaux = 1;
	}
	
	
	//CALCULA DATA INICIO
	$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux,$ano_ini); // timestamp da data final
	
	
	//verifica se é sabado, domingo ou feriado
	if($ordid != '2'){ // diferente de redes
		
		//verifica se é feriado
		$datainiferiado = strftime("%d-%m-%Y %H:%M:%S", $dataini2);
		$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($datainiferiado)."'";
		$feriado = $db->PegaUm($sql);
		if($feriado){
			$hor_ini = 8;
			$min_ini = 0;
			$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux+1,$ano_ini);
		}	
		
		//verifica se é sabado dtini
		if(date("N",$dataini2) == 6){
			$hor_ini = 8;
			$min_ini = 0;
			$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux+2,$ano_ini);
		}
		//verifica se é domingo dtini
		if(date("N",$dataini2) == 7){
			$hor_ini = 8;
			$min_ini = 0;
			$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux+1,$ano_ini);
		}
		
	}
		
	$dataini = strftime("%Y-%m-%d %H:%M:%S", $dataini2);			
	
	
	
	
	//pega o tempo para adicionar na data fim
	$tempoadd2 = explode(":",$tempoadd);
	$horaadd = (int) $tempoadd2[0];
	$minutoadd = (int) $tempoadd2[1];
	//$horaadd = (int) substr($tempoadd, 0, 2);
	//$minutoadd = (int) substr($tempoadd, 3, 2);
		
		
	$min_d = $minutoadd + $min_ini;
	

	$ano_ini = substr($dataini,0,4); 
	$mes_ini = substr($dataini,5,2); 
	$dia_ini = substr($dataini,8,2); 
	$dini = mktime(0,0,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data inicial
	 
	$hor_d=$hor_ini;
  
  // faz hora decorrer
  if($horaadd > 0){
  
    for($hor_d=$hor_ini; $hor_d<=$limitehorafim; $hor_d++){
    
        //exit for
        if($horaadd == 0) break;  
        
        //hora almoço
        if($existehoraalmoco == false){
        
          if($hor_d == 12 || $hor_d == 13){
            $hor_d = 14;
          }
          
        }        
        
        //echo $hor_d ." = ". $limitehorafim . "<br>";
        
        //add dia
        if($hor_d == $limitehorafim){
          $hor_d = $limitehoraini;
          
          //add 1 dia
          $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial 
          
          if($ordid != '2'){ // diferente de redes
        	  
          	  //verifica se é feriado
			  $diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
			  $sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
			  $f = $db->PegaUm($sql);
			  if($f) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
          	
	          //verifica se é sabado 
	          if(date("N",$dini) == 6) $dini = $dini + (2*86400); // adicionando mais 2 dia (em segundos) na data inicial
	          
	          //verifica se é domingo 
	          if(date("N",$dini) == 7) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
          }
        }
        
        
        $horaadd = $horaadd - 1;
        
    
    }
  
  }
  

 
  
      
  
  // faz calculo minuto 
  if($min_d > 0){
   // echo $min_d ; 
    if($min_d >= 60){
    
      $hor_d = $hor_d + 1;
      $min_d = $min_d - 60;
      
      //hora almoço
      if($existehoraalmoco == false){
      
        if($hor_d == 12 || $hor_d == 13){
          $hor_d = $hor_d + 2;
        }
        
      }    
      
      
      //add dia
      if( $hor_d > $limitehorafim || ($hor_d == $limitehorafim && $min_d > 0) ) {
        $hor_d = $limitehoraini + ($hor_d - $limitehorafim);
        
        //add 1 dia
        $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial 
        
        if($ordid != '2'){ // diferente de redes
			
        	//verifica se é feriado
			$diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
			$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
			$f1 = $db->PegaUm($sql);
			if($f1) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
        	
	        //verifica se é sabado 
	        if(date("N",$dini) == 6) $dini = $dini + (2*86400); // adicionando mais 2 dia (em segundos) na data inicial
	        
	        //verifica se é domingo 
	        if(date("N",$dini) == 7) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
        }
      }
    
      
    }else{
    
      //hora almoço
      if($existehoraalmoco == false){
      
        if($hor_d == 12 || $hor_d == 13){
          $hor_d = $hor_d + 2;
        }
        
      }    
      

      
      
      //add dia
      if( $hor_d > $limitehorafim || ($hor_d == $limitehorafim && $min_d > 0) ) {
        $hor_d = $limitehoraini + ($hor_d - $limitehorafim);
        
        //add 1 dia
        $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial 
        
        if($ordid != '2'){ // diferente de redes
        	
			//verifica se é feriado
			$diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
			$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
			$f2 = $db->PegaUm($sql);
			if($f2) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
        	
	        //verifica se é sabado 
	        if(date("N",$dini) == 6) $dini = $dini + (2*86400); // adicionando mais 2 dia (em segundos) na data inicial

	        //verifica se é domingo 
	        if(date("N",$dini) == 7) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
        }
      }
    
    
    }   
    
  }
  
  //add dia para redes
  if($hor_d == 24){
      $hor_d = $limitehoraini;
         
      //add 1 dia
      $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial 
  }

  
  	//fazer função recursiva
	//verifica se é feriado1
	$diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
	$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
	$f2 = $db->PegaUm($sql);
	if($f2) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial			
  
	//verifica se é feriado2
	$diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
	$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
	$f2 = $db->PegaUm($sql);
	if($f2) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial			
	//fim fazer função recursiva
  
  
  
	if(strlen($hor_d) == 1) $hor_d = "0".$hor_d;
	if(strlen($min_d) == 1) $min_d = "0".$min_d;
	$datafim = strftime("%Y-%m-%d", $dini) ." ". $hor_d . ":" . $min_d; 

	//formata para colocar no input do html
	$ano_ini = substr($dataini,0,4);
	$mes_ini = substr($dataini,5,2);
	$dia_ini = substr($dataini,8,2);
	$hor_ini = substr($dataini,11,2);
	$min_ini = substr($dataini,14,2);	

	$ano_fim = substr($datafim,0,4);
	$mes_fim = substr($datafim,5,2);
	$dia_fim = substr($datafim,8,2);
	$hor_fim = substr($datafim,11,2);
	$min_fim = substr($datafim,14,2);	
	
	$retorno = $dia_ini."/".$mes_ini."/".$ano_ini ."|". $hor_ini.":".$min_ini ."|". $dia_fim."/".$mes_fim."/".$ano_fim ."|". $hor_fim.":".$min_fim;
	return $retorno; 
}







function verificaCalculoTempoDtfim($dtinix, $tempoadicional, $tipohorario, $dataconc, $ordid = null){
	global $db;

	if(!$tempoadicional) $tempoadicional = "00:00";
	$tempoadicional2 = explode(":", $tempoadicional);

	$hort = (int) $tempoadicional2[0];
	$mint = (int) $tempoadicional2[1];


	if($mint>=60){
		$hort = $hort + 1;
		$mint = $mint - 60;
	}

	if(strlen($hort) == 1) $hort = "0".$hort;
	if(strlen($mint) == 1) $mint = "0".$mint;

	$tempoadd = $hort.":".$mint.":00";

	$limitehoraini = 8;
	$limitehorafim = 18;
	$minutopordia = 8*60;
	$existehoraalmoco = false;
	if($tipohorario == "T"){
		$limitehorafim = 22;
		$existehoraalmoco = true;
		$minutopordia = 14*60;
	}elseif($tipohorario == "A"){
		$existehoraalmoco = true;
		$minutopordia = 10*60;
	}elseif($tipohorario == "N"){
		$limitehorafim = 22;
		$minutopordia = 12*60;
	}


	if($ordid == '2'){ // redes - flag atendimento para redes 24h
		$limitehoraini = 0;
		$limitehorafim = 24;
		$minutopordia = 24*60;
		$existehoraalmoco = true;
	}


	//pega data inicio
	if($dtinix){
		$ano_ini = substr($dtinix,6,4);
		$mes_ini = substr($dtinix,3,2);
		$dia_ini = substr($dtinix,0,2);
		$hor_ini = substr($dtinix,11,2);
		$min_ini = substr($dtinix,14,2);
	}
	else{
		$ano_ini = date("Y");
		$mes_ini = date("m");
		$dia_ini = date("d");
		$hor_ini = date("H");
		$min_ini = date("i");
	}


	$diaaux = 0;
	if($hor_ini < $limitehoraini){
		$hor_ini = $limitehoraini;
		$min_ini = 0;
	}
	if( ($hor_ini == 12 || $hor_ini == 13) && $existehoraalmoco == false){
		$hor_ini = 14;
		$min_ini = 0;
	}
	//if( ($hor_ini > $limitehorafim) || ($hor_ini == $limitehorafim && $min_ini > 0) ){
	if($hor_ini >= $limitehorafim){
		$hor_ini = 8;
		$min_ini = 0;
		$diaaux = 1;
	}


	//CALCULA DATA INICIO
	$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux,$ano_ini); // timestamp da data final


	//verifica se é sabado, domingo ou feriado
	if($ordid != '2'){ // diferente de redes
		
		//verifica se é feriado
		$datainiferiado = strftime("%d-%m-%Y %H:%M:%S", $dataini2);
		$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($datainiferiado)."'";
		$feriado = $db->PegaUm($sql);
		if($feriado){
			$hor_ini = 8;
			$min_ini = 0;
			$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux+1,$ano_ini);
		}	
			
		if(date("N",$dataini2) == 6){
			$hor_ini = 8;
			$min_ini = 0;
			$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux+2,$ano_ini);
		}
		//verifica se é domingo dtini
		if(date("N",$dataini2) == 7){
			$hor_ini = 8;
			$min_ini = 0;
			$dataini2 = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini+$diaaux+1,$ano_ini);
		}
	}

	$dataini = strftime("%Y-%m-%d %H:%M:%S", $dataini2);





	//pega o tempo para adicionar na data fim
	$tempoadd2 = explode(":", $tempoadd);
	$horaadd = (int) $tempoadd2[0];
	$minutoadd = (int) $tempoadd2[1];


	$min_d = $minutoadd + $min_ini;


	$ano_ini = substr($dataini,0,4);
	$mes_ini = substr($dataini,5,2);
	$dia_ini = substr($dataini,8,2);
	$dini = mktime(0,0,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data inicial

	$hor_d=$hor_ini;

	// faz hora decorrer
	if($horaadd > 0){

		for($hor_d=$hor_ini; $hor_d<=$limitehorafim; $hor_d++){

			//exit for
			if($horaadd == 0) break;

			//hora almoço
			if($existehoraalmoco == false){

				if($hor_d == 12 || $hor_d == 13){
					$hor_d = 14;
				}

			}

			//echo $hor_d ." = ". $limitehorafim . "<br>";

			//add dia
			if($hor_d == $limitehorafim){
				$hor_d = $limitehoraini;

				//add 1 dia
				$dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial

				if($ordid != '2'){ // diferente de redes
					
					//verifica se é feriado
					$diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
					$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
					$f = $db->PegaUm($sql);
					if($f) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
					
					//verifica se é sabado
					if(date("N",$dini) == 6) $dini = $dini + (2*86400); // adicionando mais 2 dia (em segundos) na data inicial
					//verifica se é domingo
					if(date("N",$dini) == 7) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
				}
			}


			$horaadd = $horaadd - 1;


		}

	}



	// faz calculo minuto
	if($min_d > 0){
		// echo $min_d ;
		if($min_d >= 60){

			$hor_d = $hor_d + 1;
			$min_d = $min_d - 60;

			//hora almoço
			if($existehoraalmoco == false){

				if($hor_d == 12 || $hor_d == 13){
					$hor_d = $hor_d + 2;
				}

			}


			//add dia
			if( $hor_d > $limitehorafim || ($hor_d == $limitehorafim && $min_d > 0) ) {
				$hor_d = $limitehoraini + ($hor_d - $limitehorafim);

				//add 1 dia
				$dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial

				if($ordid != '2'){ // diferente de redes
					
					//verifica se é feriado
					$diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
					$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
					$f1 = $db->PegaUm($sql);
					if($f1) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
					
					//verifica se é sabado
					if(date("N",$dini) == 6) $dini = $dini + (2*86400); // adicionando mais 2 dia (em segundos) na data inicial
					//verifica se é domingo
					if(date("N",$dini) == 7) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
				}
			}


		}else{

			//hora almoço
			if($existehoraalmoco == false){

				if($hor_d == 12 || $hor_d == 13){
					$hor_d = $hor_d + 2;
				}

			}


			//add dia
			if( $hor_d > $limitehorafim || ($hor_d == $limitehorafim && $min_d > 0) ) {
				$hor_d = $limitehoraini + ($hor_d - $limitehorafim);

				//add 1 dia
				$dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial

				if($ordid != '2'){ // diferente de redes
					
					//verifica se é feriado
					$diniferiado = strftime("%d-%m-%Y %H:%M:%S", $dini);
					$sql = "select frddata from demandas.feriado where frdstatus='A' and frddata = '".formata_data_sql($diniferiado)."'";
					$f2 = $db->PegaUm($sql);
					if($f2) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
					
					//verifica se é sabado
					if(date("N",$dini) == 6) $dini = $dini + (2*86400); // adicionando mais 2 dia (em segundos) na data inicial
					//verifica se é domingo
					if(date("N",$dini) == 7) $dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
				}
			}


		}

	}


	//add dia para redes
	if($hor_d == 24){
		$hor_d = $limitehoraini;
		 
		//add 1 dia
		$dini = $dini + 86400; // adicionando mais 1 dia (em segundos) na data inicial
	}


	if(strlen($hor_d) == 1) $hor_d = "0".$hor_d;
	if(strlen($min_d) == 1) $min_d = "0".$min_d;
	$datafim = strftime("%Y-%m-%d", $dini) ." ". $hor_d . ":" . $min_d;

	//formata para colocar no input do html
	$ano_ini = substr($dataini,0,4);
	$mes_ini = substr($dataini,5,2);
	$dia_ini = substr($dataini,8,2);
	$hor_ini = substr($dataini,11,2);
	$min_ini = substr($dataini,14,2);

	$ano_fim = substr($datafim,0,4);
	$mes_fim = substr($datafim,5,2);
	$dia_fim = substr($datafim,8,2);
	$hor_fim = substr($datafim,11,2);
	$min_fim = substr($datafim,14,2);




	if($dataconc){
		$ano_conc = substr($dataconc,6,4);
		$mes_conc = substr($dataconc,3,2);
		$dia_conc = substr($dataconc,0,2);
		$hor_conc = substr($dataconc,11,2);
		$min_conc = substr($dataconc,14,2);
	}
	else{
		$ano_conc = date("Y");
		$mes_conc = date("m");
		$dia_conc = date("d");
		$hor_conc = date("H");
		$min_conc = date("i");
	}


	if($ano_conc.$mes_conc.$dia_conc.$hor_conc.$min_conc > $ano_fim.$mes_fim.$dia_fim.$hor_fim.$min_fim){
		$dtcalcfim = "<b><font color=red>".$dia_fim."/".$mes_fim."/".$ano_fim ." ". $hor_fim.":".$min_fim."</font></b>";
	}
	else{
		$dtcalcfim = "<b><font color=blue>".$dia_fim."/".$mes_fim."/".$ano_fim ." ". $hor_fim.":".$min_fim."</font></b>";
	}


	return $dtcalcfim;

}

function pegaQrpid( $queid, $dmdid ){
	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	
    global $db;
   
    $sql = "SELECT
            	ep.qrpid as qrpid
            FROM
            	demandas.escritorioprocessos ep
            LEFT JOIN questionario.questionarioresposta q ON q.qrpid = ep.qrpid
            WHERE
            	ep.dmdid = {$dmdid}	AND 
            	q.queid = {$queid}";

    $qrpid = $db->pegaUm( $sql );
    
    if( empty( $qrpid ) ){
    	$sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$cpf}'";
    	$nome = $db->pegaUm( $sql );
    	$sql = "SELECT dmdtitulo FROM demandas.demanda WHERE dmdid = {$dmdid}";
    	$titulo = $db->pegaUm( $sql );
    	$arParam = array ( "queid" => $queid, "titulo" => "DEMANDAS(".$nome." - ".$titulo.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "INSERT INTO demandas.escritorioprocessos ( qrpid, dmdid ) VALUES ( {$qrpid}, {$dmdid})";
    	$db->executar( $sql );
    	$db->commit();
    }
    
    return $qrpid;
}

function formata_valor_sql($valor){
		
	$valor = str_replace('.', '', $valor);
	$valor = str_replace(',', '.', $valor);
	
	return $valor;
}
?>