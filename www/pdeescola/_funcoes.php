<?php

/**
 * Recupera a escola, estado ou município
 * atribuído ao perfil do usuário no PDE Escola
 * 
 * @param string $resp
 * @return mixed
 * @author Felipe Carvalho
 */
function pdeRecuperaResponsabilidadePerfil($resp)
{
	global $db;

	$sql = "SELECT
				".$resp."
			FROM
				pdeescola.usuarioresponsabilidade
			WHERE
				usucpf = '".$_SESSION["usucpf"]."' 
				AND rpustatus = 'A'
				AND pflcod in (".PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL.",
							   ".PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL.",
							   ".PDEESC_PERFIL_COMITE_MUNICIPAL.",
							   ".PDEESC_PERFIL_COMITE_ESTADUAL.",
							   ".PDEESC_PERFIL_MONITORAMENTO_ESTADUAL.",
							   ".PDEESC_PERFIL_MONITORAMENTO_MUNICIPAL.")";
	return $db->pegaUm($sql);
}

/**
 * Verifica se o usuário possui algum perfil do submódulo
 * passado por parâmetro
 * 
 *  @author Felipe Carvalho 
 *  @param string $submodulo
 *  @param array $perfis
 *  @return boolean
 */
function possuiPerfilSubModulo( $submodulo, $perfis )
{
	/*** Inicializa a variável de retorno ***/
	$retorno = false;
	
	/*** PDE ESCOLA ***/
	if($submodulo == 'pdeescola')
	{
		/*** Atribui ao array os perfis existentes no submódulo ***/
		$pdeEscola	=	array(
							PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL,
							PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL,
							PDEESC_PERFIL_COMITE_MUNICIPAL,
							PDEESC_PERFIL_COMITE_ESTADUAL,
							PDEESC_PERFIL_EQUIPE_TECNICA_MEC,
							PDEESC_PERFIL_CONSULTA,
							PDEESC_PERFIL_MONITORAMENTO_ESTADUAL,
							PDEESC_PERFIL_MONITORAMENTO_MUNICIPAL
							);
		/*** Verifica se o usuário possui algum perfil no submódulo ***/
		for($i=0; $i<count($perfis); $i++)
		{
			if( in_array($perfis[$i], $pdeEscola) )
			{
				$retorno = true;
			}
		}
	}
	/*** MAIS EDUCAÇÃO ***/
	if($submodulo == 'maiseducacao')
	{
		/*** Atribui ao array os perfis existentes no submódulo ***/
		$maisEducacao	=	array(
								PDEESC_PERFIL_CAD_MAIS_EDUCACAO,
								PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO,
								PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO,
								PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO,
								PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO
								);
		/*** Verifica se o usuário possui algum perfil no submódulo ***/						
		for($i=0; $i<count($perfis); $i++)
		{
			if( in_array($perfis[$i], $maisEducacao) )
			{
				$retorno = true;
			}
		}
	}
	/*** ESCOLA ACESSÍVEL ***/
	if($submodulo == 'escolaacessivel')
	{
		/*** Atribui ao array os perfis existentes no submódulo ***/
		$escolaAcessivel	=	array(
									PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL,
									PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL,
									PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL,
									PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ACESSIVEL,
									PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL
									);
		/*** Verifica se o usuário possui algum perfil no submódulo ***/				
		for($i=0; $i<count($perfis); $i++)
		{
			if( in_array($perfis[$i], $escolaAcessivel) )
			{
				$retorno = true;
			}
		}
	}
	/*** ESCOLA ABERTA ***/
	if($submodulo == 'escolaaberta')
	{
		/*** Atribui ao array os perfis existentes no submódulo ***/
		$escolaAberta	=	array(
								PDEESC_PERFIL_CAD_ESCOLA_ABERTA,
								PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA,
								PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA,
								PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ABERTA,
								PDEESC_PERFIL_CONSULTA_ESCOLA_ABERTA
								);
		/*** Verifica se o usuário possui algum perfil no submódulo ***/
		for($i=0; $i<count($perfis); $i++)
		{
			if( in_array($perfis[$i], $escolaAberta) )
			{
				$retorno = true;
			}
		}
	}
	/*** QUESTIONÁRIO SEESP ***/
	if($submodulo == 'questionario')
	{
		/*** Atribui ao array os perfis existentes no submódulo ***/
		$questionarioSEESP	=	array(
								PDEESC_PERFIL_SEC_ESTADUAL_QUEST_SEESP,
								PDEESC_PERFIL_SEC_MUNICIPAL_QUEST_SEESP,
								PDEESC_PERFIL_ESCOLA_QUEST_SEESP,
								PDEESC_PERFIL_ADM_QUEST_SEESP
								);
		/*** Verifica se o usuário possui algum perfil no submódulo ***/
		for($i=0; $i<count($perfis); $i++)
		{
			if( in_array($perfis[$i], $questionarioSEESP) )
			{
				$retorno = true;
			}
		}
	}
								
	return $retorno;
}

/*
 * Função que pega os perfis do usuário que possuem acesso à escola
 *
 * @return: (array) contendo o resultado do sql executado
 * @author: FelipeChiavicatti
 */
function arrayAcessoPerfil(){
	global $db;
	$pdeid = $_SESSION['pdeid'];

	$sql = sprintf("SELECT
					 ur.pflcod
					FROM
					 pdeescola.pdeescola pe
					 INNER JOIN entidade.entidade e ON e.entid = pe.entid
					 INNER JOIN entidade.endereco endi ON endi.entid = e.entid
					 INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.usucpf 	 = '%s' AND
																	    ur.rpustatus = 'A' AND
																	    (
																			pe.pdeuf    = ur.estuf OR
																			endi.muncod = ur.muncod OR
																			e.entid     = ur.entid
																	    )
					WHERE
					 pdeid = %d;",
	$_SESSION['usucpf'],
	$pdeid);

	return (array) $db->carregarColuna($sql,'pflcod');

}



/*
 * Retorna a imagem de 'Respondido' ou 'Não Respondido'.
 */

/*
 function carregaImagem($questao) {
 global $db;
 $pdeid = $_SESSION['pdeid'];

 switch($questao) {
 case 'p1':
 $epfid = 1;
 break;
 case 'p7':
 $epfid = 2;
 break;
 case 'p8':
 $epfid = 3;
 break;
 case 'p9_1':
 $epfid = 5;
 break;
 case 'p10_1':
 $epfid = 7;
 break;
 case 'p10_2':
 $epfid = 8;
 break;
 case 'p10_3':
 $epfid = 9;
 break;
 case 'p10_4':
 $epfid = 10;
 break;
 case 'p10_5':
 $epfid = 11;
 break;
 case 'p10_6':
 $epfid = 12;
 break;
 case 'p10_7':
 $epfid = 13;
 break;
 case 'p11_1':
 $epfid = 15;
 break;
 case 'p11_2':
 $epfid = 16;
 break;
 case 'p12_1':
 $epfid = 18;
 break;
 case 'p13_1':
 $epfid = 20;
 break;
 case 'p13_2':
 $epfid = 21;
 break;
 case 'p13_3':
 $epfid = 22;
 break;
 case 'p14':
 $epfid = 23;
 break;
 case 'p15':
 $epfid = 24;
 break;
 case 'p16':
 $epfid = 25;
 break;
 case 'p17':
 $epfid = 26;
 break;
 case 'p18':
 $epfid = 27;
 break;
 case 'p19':
 $epfid = 28;
 break;
 case 'p20':
 $epfid = 29;
 break;
 case 'p21':
 $epfid = 30;
 break;
 case 'p22':
 $epfid = 31;
 break;
 case 'p23':
 $epfid = 32;
 break;
 case 'p24':
 $epfid = 33;
 break;
 case 'p25':
 $epfid = 34;
 break;
 case 'p26':
 $epfid = 35;
 break;
 case 'p27':
 $epfid = 36;
 break;
 case 'p28':
 $epfid = 37;
 break;
 }

 $existe = $db->pegaUm("SELECT
 pepid
 FROM
 pdeescola.pdeepf
 WHERE
 pdeid = ".$pdeid." AND
 epfid = ".$epfid);
 if($existe == NULL)
 $img = "<img src=\"../imagens/atencao.png\" style=\"border:0;\" title=\"Não Respondido\">";
 else
 $img = "<img src=\"../imagens/check_p.gif\" style=\"border:0;\" title=\"Respondido\">";

 return $img;
 }
 */


function verificaComite(){

	$da = explode('-', date('d-m-Y'));
	$dc = explode('-', DATA_LIMITE_COMITE);
	$perfis = arrayPerfil();

	$dataAtual = mktime($day = $da[0], $month = $da[1], $year = $da[2]);
	$dataComite = mktime ($day = $dc[0], $month = $dc[1], $year = $dc[2]);

	$limPrazoComite = ( $dataComite < $dataAtual);
	//dbg($limPrazoComite,1);
	$pTodos = (in_array( PDEESC_PERFIL_CONSULTA, $perfis) || in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfis) || in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis) || in_array( PDEESC_PERFIL_COMITE_ESTADUAL, $perfis ) || in_array( PDEESC_PERFIL_COMITE_MUNICIPAL, $perfis));
	$perfComite = (in_array( PDEESC_PERFIL_COMITE_ESTADUAL, $perfis ) || in_array( PDEESC_PERFIL_COMITE_MUNICIPAL, $perfis ));


	if($perfComite && $limPrazoComite ){
		return true;
	}elseif($perfComite && !$limPrazoComite){
		return false;
	}else{
		return true;
	}

}


function carregaImagem($questao) {
	global $db;
	$pdeid = $_SESSION['pdeid'];

	$existe = $db->pegaUm("SELECT
								pprid
							FROM
								pdeescola.pdepreenchimento
							WHERE
								pdeid = ".$pdeid." AND
	ppritem = '$questao'");
	if($existe == NULL){
		$img = "<img src=\"../imagens/atencao.png\" style=\"border:0;\" title=\"Não Respondido\">";

	}
	else{
		$img = "<img src=\"../imagens/check_p.gif\" style=\"border:0;\" title=\"Respondido\">";
	}
	return $img;
}


function pdeescola_possui_perfil( $pflcods ){

	global $db;

	if ($db->testa_superuser()) {

		return true;

	}else{

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

	}
}

/**
 * 
 */
function pdeescola_pega_escola_atribuida( $usucpf ){

	global $db;

	$sql = "SELECT
	entid
	FROM
	pdeescola.usuarioresponsabilidade
	WHERE
	usucpf = '{$usucpf}' AND
	rpustatus = 'A'";

	$entid = $db->pegaUm( $sql );

	return $entid;

}

function pdeescola_possui_escola_atribuida_cadastrador( $usucpf ){

	global $db;

	$sql = "SELECT
	entid
	FROM
	pdeescola.usuarioresponsabilidade
	WHERE
	usucpf = '{$usucpf}' AND
	rpustatus = 'A' AND
	pflcod = ".PDEESC_PERFIL_CAD_MAIS_EDUCACAO."";

	$entid = $db->pegaUm( $sql );

	return !empty($entid) ? true : false;

}

/**
 *
 */
function pdeescola_possui_escola_ambos( $entid ){

	global $db;

	$sql = "SELECT
	entid
	FROM
	entidade.entidadedetalhe
	WHERE
	entid = {$entid} AND
	entpdeescola = 'true'";

	$existe_pde = $db->pegaUm( $sql );

	$sql = "SELECT
	entid
	FROM
	pdeescola.memaiseducacao
	WHERE
	entid = {$entid} AND
	memanoreferencia = '2009'";

	$existe_me = $db->pegaUm( $sql );

	return ( $existe_me && $existe_pde ) ? true : false;

}

/*
 * Monta Arvore "Proposta Monitoramento"
 */
function montaTreePropostaMonitoramento(){
	global $db;

	$entid = $_SESSION['entid'];
	$pdeid = $_SESSION['pdeid'];
	/*
	 * $tree, recebe script da árvore
	 */
	$tree .= "<div id=\"bloco\" style=\"overflow: hidden;\">
				<p>
					<a href=\"javascript: arvore.openAll();\">Abrir Todos</a>
					&nbsp;|&nbsp;
					<a href=\"javascript: arvore.closeAll();\">Fechar Todos</a>
				</p>
				<div id=\"_arvore\"></div>
			  </div>";

	$tree .= "<script type=\"text/javascript\">
				arvore = new dTree( 'arvore' );
				arvore.config.folderLinks = true;
				arvore.config.useIcons = true;
				arvore.config.useCookies = true;\n";

	if ( $entid ){
		$where = "ent.tpcid IN (1,3) AND
				  ent.entid IN ('".$entid."') ";
		$on    = "(ur.entid  = ent.entid OR
				   ur.muncod = ende.muncod OR
				   ur.estuf  = ende.estuf)";
		$tree  .= "arvore.add( {$entid}, -1, 'PDE ESCOLA - AVALIAÇÃO' );\n";
	}else{
		die('<script>
				history.go(-1);
			 </script>');
	}

	/*
	 * Carrega array com perfis do usuário
	 */
	$perfil = arrayPerfil();

	/*
	 * Caso não tenha acesso global
	 * vê somente o que tiver acesso, atravéz do "usuarioresponsabilidade"
	 */
	if ( !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) && !in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil) ) {
		$from = "LEFT JOIN pdeescola.usuarioresponsabilidade ur ON ".$on." AND
																	rpustatus = 'A' AND
																	ur.usucpf = '".$_SESSION['usucpf']."' AND
																	ur.pflcod IN (".implode(',',$perfil).")";

	}

	/*
	 * Requisita entidades
	 */
	$sql = sprintf("SELECT
					 DISTINCT
					-- est.estuf,
					-- est.estdescricao,
					-- mun.muncod,
					-- mun.mundescricao,
					 ent.entid,
					 ent.entnome -- ,
					 -- pde.pdeid
					FROM
					 entidade.entidade ent
					 INNER JOIN entidade.funcaoentidade as fe ON fe.entid = ent.entid
					 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid
					-- INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
					-- INNER JOIN territorios.estado est ON est.estuf = mun.estuf
					 -- LEFT JOIN pdeescola.pdeescola pde ON pde.entid = ent.entid
					 %s
					WHERE
					 fe.funid = 3 and
					 %s
					-- ORDER BY
					-- mun.mundescricao",
	$from,
	$where);

	$dados = (array) $db->carregar($sql);

	$est = array();
	$mun = array();
	$ent = array();

	/*
	 * Carrega perguntas Monitoramento
	 */
	$sql = "SELECT
	DISTINCT
	qap2.qapid,
	qap2.qapcodigo,
	qap2.qapdescricao,
	qap2.qapidpai,
	ap.aplid,
	ap.aplsugestao
	FROM
	pdeescola.questaoavaliacaoplano qap
	INNER JOIN pdeescola.questaoavaliacaoplano qap2 ON qap2.qapidpai = qap.qapid OR qap2.qapidpai IS NULL
	LEFT JOIN pdeescola.avaliacaoplano ap ON ap.qapid = qap2.qapid AND ap.pdeid = {$pdeid}
	WHERE
	qap2.qapano = ".ANO_EXERCICIO_PDE_ESCOLA."
			ORDER BY
			 qap2.qapid";

	$perg = (array) $db->carregar($sql);

	/*
	 * Carrega Totais 2º nível
	 */
	$sql = "SELECT
			 SUM(ea.eavvalor::integer) AS eavid,
			 qap.qapid
			FROM
			 pdeescola.questaoavaliacaoplano qap
			 INNER JOIN pdeescola.questaoavaliacaoplano qap2 ON qap2.qapidpai = qap.qapid
			 INNER JOIN pdeescola.avaliacaoplano ap ON ap.qapid = qap2.qapid
			 INNER JOIN pdeescola.escalaavaliacao ea ON ea.eavid = ap.eavid
			WHERE
			 qap.qapano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
	qap.qapidpai IS NOT NULL AND
	ap.pdeid = {$pdeid}
	GROUP BY
	qap.qapid";
	$total = (array) $db->carregar($sql);

	/*
	 * Carrega totais 1º nível
	 */

	$sql = "SELECT
			 SUM(ea.eavvalor::integer) AS eavid,
			 qap.qapid
			FROM
			 pdeescola.questaoavaliacaoplano qap
			 INNER JOIN pdeescola.questaoavaliacaoplano qap1 ON qap1.qapidpai = qap.qapid
			 INNER JOIN pdeescola.questaoavaliacaoplano qap2 ON qap2.qapidpai = qap1.qapid
			 INNER JOIN pdeescola.avaliacaoplano ap ON ap.qapid = qap2.qapid
			 INNER JOIN pdeescola.escalaavaliacao ea ON ea.eavid = ap.eavid
			WHERE
			 qap.qapano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
	qap.qapidpai IS NULL AND
	ap.pdeid = {$pdeid}
	GROUP BY
	qap.qapid";
	$total_1 = (array) $db->carregar($sql);


/*
	 * Adiciona os itens da árvore
	 */
	foreach ($dados as $dado){

		// Monta ENTIDADE "Escola"
		if ( !in_array($dado['entid'], $ent) ){
			$ent[] = $dado['entid'];

			/*
			 * Se ouver registro no "monitoramento"
			 * Monta estrutura monitoramento
			 */
			if ($dado['entid']){
				//$tree .= "arvore.add('i_1','{$dado['entid']}','');\n";
				$entidx = $dado['entid'];

				foreach ($perg as $perg):
				$texto = simec_htmlentities(str_replace(array('\n','\r','<br>','</br>',chr(10)),'',trim($perg['qapcodigo'])." - ". (strlen($perg['qapdescricao']) > 110 ? substr($perg['qapdescricao'],0,110).'...' : $perg['qapdescricao'] )),ENT_QUOTES);
				$param_img = 'cadastroMonitoramento_'.$perg['qapid'];

				if ( !$perg['qapidpai']){
					$tree .= "arvore.add('{$perg['qapid']}','". ($perg['qapidpai'] ? $perg['qapidpai'] : $entidx) ."','{$texto}','javascript:void(0);');\n";
				}else if(($perg['qapidpai'] == 1) || ($perg['qapidpai'] == 28)){
					$tree .= "arvore.add('{$perg['qapid']}','". ($perg['qapidpai'] ? $perg['qapidpai'] : $entidx) ."','{$texto}','javascript:void(0);');\n";
				}else{
					$tree .= "arvore.add('{$perg['qapid']}','{$perg['qapidpai']}','".carregaImagem(trim($param_img))." <a href=\'?modulo=principal/instrumento3/cadastroMonitoramento&acao=A&qapid={$perg['qapid']}\'>".$texto."</a>','javascript:void(0)');\n";
				}
				endforeach;
			}
			/*
			 * Adiciona totais 1º nível
			 */
			if ($total_1[0]){
				foreach ($total_1 as $tot_1):
				$tree .= "arvore.add('tot_{$tot_1['qapid']}','{$tot_1['qapid']}','<B>Total:</B> {$tot_1['eavid']}','javascript:void(0);');\n";
				endforeach;
			}
			/*
			 * Adiciona totais 2º nível
			 */
			if ($total[0]){
				foreach ($total as $tot):
				$tree .= "arvore.add('tot_{$tot['qapid']}','{$tot['qapid']}','<B>Total:</B> {$tot['eavid']}','javascript:void(0);');\n";
				endforeach;
			}


		}
	}

	$tree .= "  elemento = document.getElementById( '_arvore' );
			    elemento.innerHTML = arvore;
			  </script>";

	return $tree;
}


/*
 * Monta Arvore "escolas"
 */
function montaTreeEscola(){
	global $db, $docid;

	$entid = $_SESSION['entid'];
	$pdeid = $_SESSION['pdeid'];
	
	$esdid = pegarEstadoAtual($docid);
	$perfil = arrayPerfil();
	
	/*
	 * $tree, recebe script da árvore
	 */
	$tree .= "<div id=\"bloco\" style=\"overflow: hidden;\">
				<p>
					<a href=\"javascript: arvore.openAll();\">Abrir Todos</a>
					&nbsp;|&nbsp;
					<a href=\"javascript: arvore.closeAll();\">Fechar Todos</a>
				</p>
				<div id=\"_arvore\"></div>
			  </div>";

	$tree .= "<script type=\"text/javascript\">
				arvore = new dTree( 'arvore' );
				arvore.config.folderLinks = true;
				arvore.config.useIcons = true;
				arvore.config.useCookies = true;\n";
	/*
	 * Filtro por estado ou municipio ou entidade
	 */
	/*
	 if ( array_key_exists("estuf", $typeGet) ){
		$where = " ent.tpcid = 1 AND
		est.estuf IN ('".implode( "','", explode(';',$typeGet['estuf']) )."') ";
		$on    = "(ur.estuf = ende.estuf OR
		ur.entid = ent.entid)";
		$tree  .= "arvore.add( 0, -1, 'Diagnóstico Estadual' );\n";
		}elseif ( array_key_exists("muncod", $typeGet) ){
		$where = " ent.tpcid = 3 AND
		mun.muncod IN ('".implode( "','", explode(';',$typeGet['muncod']) )."') ";
		$on    = "(ur.muncod = ende.muncod  OR
		ur.entid = ent.entid)";
		$tree  .= "arvore.add( 0, -1, 'Diagnóstico Municipal' );\n";
		}else*/if ( $entid ){
	$where = "ent.tpcid IN (1,3) AND
				  ent.entid IN ('".$entid."') ";
	$on    = "(ur.entid  = ent.entid OR
				   ur.muncod = ende.muncod OR
				   ur.estuf  = ende.estuf)";
	$tree  .= "arvore.add( {$entid}, -1, 'Diagnóstico Escolar' );\n";
		}else{
			die('<script>
				history.go(-1);
			 </script>');
		}

		/*
		 * Carrega array com perfis do usuário
		 */
		$perfil = arrayPerfil();

		/*
		 * Caso não tenha acesso global
		 * vê somente o que tiver acesso, atravéz do "usuarioresponsabilidade"
		 */
		if ( !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) && !in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil) ) {
			$from = "LEFT JOIN pdeescola.usuarioresponsabilidade ur ON ".$on." AND
																	rpustatus = 'A' AND
																	ur.usucpf = '".$_SESSION['usucpf']."' AND
																	ur.pflcod IN (".implode(',',$perfil).")";

		}

		/*
		 * Requisita entidades
		 */
		$sql = sprintf("SELECT
					 DISTINCT
					-- est.estuf,
					-- est.estdescricao,
					-- mun.muncod,
					-- mun.mundescricao,
					 ent.entid,
					 ent.entnome -- ,
					 -- pde.pdeid
					FROM
					 entidade.entidade ent
					 INNER JOIN entidade.funcaoentidade as fe ON fe.entid = ent.entid
					 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid
					-- INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
					-- INNER JOIN territorios.estado est ON est.estuf = mun.estuf
					 -- LEFT JOIN pdeescola.pdeescola pde ON pde.entid = ent.entid
					 %s
					WHERE
					 fe.funid = 3 and
					 %s
					-- ORDER BY
					-- mun.mundescricao",
		$from,
		$where);

		//pre( $sql, 1);

		$dados = (array) $db->carregar($sql);

		//	if (!$dados[0]):
		//		die('<script>
		//				alert(\'Usuário não possui permissão para acessar esta Escola!\');
		//				history.go(-1);
		//			 </script>');
		//	endif;

		$est = array();
		$mun = array();
		$ent = array();

		/*
		 * Carrega perguntas instrumento 2
		 */
		/*
		 $sql = "

		 select  cl.relname as tabela,

		 a.attname as coluna

		 from pg_catalog.pg_attribute a

		 join pg_catalog.pg_class cl on (a.attrelid = cl.oid )

		 join pg_catalog.pg_namespace n on (n.oid = cl.relnamespace)

		 where n.nspname = 'pdeescola'

		 and attstattarget != 0
		 and cl.relname = 'analisecriterioeficacia'

		 and cl.relkind = 'r'
			";

			echo $sql;
			die();
			*/

		//	print_r( $_SESSION['baselogin']);
		//	die();
		$sql = "SELECT
		DISTINCT
		p2.aceid,
		p2.acecodigo,
		p2.acedescricao,
		p2.acedescricaopr,
		p2.aceidpai,
		dace.dacid,
		dace.dacevidencia
		FROM
		pdeescola.analisecriterioeficacia p
		INNER JOIN pdeescola.analisecriterioeficacia p2 ON p2.aceidpai = p.aceid OR p2.aceidpai IS NULL
		LEFT JOIN pdeescola.detalheanalisecriterioeficacia dace ON dace.aceid = p2.aceid AND dace.pdeid = {$pdeid}
		WHERE
		p2.aceano = ".ANO_EXERCICIO_PDE_ESCOLA."
			ORDER BY
			 p2.acecodigo";


		$perg = (array) $db->carregar($sql);

		/*
		 * Carrega Totais 2º nível
		 */
		$sql = "SELECT
			 SUM(esc.esavalor) AS esavalor,
			 p.aceid
			FROM
			 pdeescola.analisecriterioeficacia p
			 INNER JOIN pdeescola.analisecriterioeficacia p2 ON p2.aceidpai = p.aceid
			 INNER JOIN pdeescola.detalheanalisecriterioeficacia dace ON dace.aceid = p2.aceid
			 INNER JOIN pdeescola.escalaace esc ON esc.esaid = dace.esaid
			WHERE
			 p.aceano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
		p.aceidpai IS NOT NULL AND
		p.aceseq IS NULL AND
		dace.pdeid = {$pdeid}
		GROUP BY
		p.aceid";
		$total = (array) $db->carregar($sql);

	 /*
	  * Carrega totais 1º nível
	  */
		$sql = "SELECT
			 SUM(esc.esavalor) AS esavalor,
			 p.aceid
			FROM
			 pdeescola.analisecriterioeficacia p
			 INNER JOIN pdeescola.analisecriterioeficacia p1 ON p1.aceidpai = p.aceid
			 INNER JOIN pdeescola.analisecriterioeficacia p2 ON p2.aceidpai = p1.aceid
			 INNER JOIN pdeescola.detalheanalisecriterioeficacia dace ON dace.aceid = p2.aceid
			 INNER JOIN pdeescola.escalaace esc ON esc.esaid = dace.esaid
			WHERE
			 p.aceano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
		p.aceidpai IS NULL AND
		p.aceseq IS NULL AND
		dace.pdeid = {$pdeid}
		GROUP BY
		p.aceid";
		$total_1 = (array) $db->carregar($sql);

		/*
		 * Adiciona os itens da árvore
		 */
		foreach ($dados as $dado){
			/*
			 // Monta ESTADO
			 if ( !in_array($dado['estuf'], $est) ){
			 $tree .= "arvore.add('{$dado['estuf']}', 0,'<b>".simec_htmlentities($dado['estdescricao'],ENT_QUOTES)."</b>');\n";
			 $est[]  = $dado['estuf'];
			 }

			 // Monta MUNICÍPIO
			 if ( !in_array($dado['muncod'], $mun) ){
			 $tree .= "arvore.add('{$dado['muncod']}','{$dado['estuf']}','".simec_htmlentities($dado['mundescricao'],ENT_QUOTES)."');\n";
			 $mun[]  = $dado['muncod'];
			 }
			 */
			// Monta ENTIDADE "Escola"
			if ( !in_array($dado['entid'], $ent) ){
				//	$tree .= "arvore.add('{$dado['entid']}','{$dado['muncod']}','".simec_htmlentities($dado['entnome'],ENT_QUOTES)."');\n";
				$ent[] = $dado['entid'];

				$tree .= "arvore.add('i_1','{$dado['entid']}','Instrumento 1');\n";

				///// Perguntas instrumento 1 /////
				$arv = $db->carregar("SELECT
									epfid,
									epfcodigo,
									epfdescricao,
									epflinkitem,
									epfnomearquivo,
									epfidpai,
									epfperg
								FROM
									pdeescola.estruturaperfilfuncionamento
								WHERE
									epfano = ".ANO_EXERCICIO_PDE_ESCOLA."
								ORDER BY
									epfordem");
				
				if($arv) {
					foreach($arv as $arvore) {
						
						if(in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfil) ||
						   in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfil)){
						   	
						   	if($esdid == AVALIACAO_COMITE_ME_WF){
							
								$arvore["epflinkitem"] = "javascript:void(0)";
						   	}
						}
						
						if($arvore["epfperg"] == 'f') {
							if($arvore["epfcodigo"] == NULL) {
								if($arvore["epfidpai"] == NULL)
								{
									$tree .= "arvore.add('".$arvore["epfid"]."_i','i_1','".trim($arvore["epfdescricao"])."','javascript:void(0);');\n";
								}
								else
								{
									if( (trim($arvore["epfnomearquivo"]) != 'fr1' ) && (trim($arvore["epfnomearquivo"]) != 'fr2' ))
									{
										$tree .= "arvore.add('".$arvore["epfid"]."_i','".$arvore["epfidpai"]."_i','".carregaImagem(trim($arvore["epfnomearquivo"]))."<a href=\'".trim($arvore["epflinkitem"])."\'>".trim($arvore["epfdescricao"])."</a>','javascript:void(0);');\n";
									}
									else
									{
										$tree .= "arvore.add('".$arvore["epfid"]."_i','".$arvore["epfidpai"]."_i','<a href=\'".trim($arvore["epflinkitem"])."\'>".trim($arvore["epfdescricao"])."</a>','javascript:void(0);');\n";
									}
								}
							} else {
								if($arvore["epfidpai"] == NULL)
								$tree .= "arvore.add('".$arvore["epfid"]."_i','i_1','".trim($arvore["epfcodigo"])." - ".trim($arvore["epfdescricao"])."','javascript:void(0);');\n";
								else
								$tree .= "arvore.add('".$arvore["epfid"]."_i','".$arvore["epfidpai"]."_i','".trim($arvore["epfcodigo"])." - ".trim($arvore["epfdescricao"])."','javascript:void(0);');\n";
							}

						} else {
							if($arvore["epfidpai"] == NULL)
							$tree .= "arvore.add('".$arvore["epfid"]."_i','i_1','".carregaImagem(trim($arvore["epfnomearquivo"]))." <a href=\'".trim($arvore["epflinkitem"])."\'>".trim($arvore["epfcodigo"])." - ".trim($arvore["epfdescricao"])."</a>','javascript:void(0);');\n";
							else
							$tree .= "arvore.add('".$arvore["epfid"]."_i','".$arvore["epfidpai"]."_i','".carregaImagem(trim($arvore["epfnomearquivo"]))." <a href=\'".trim($arvore["epflinkitem"])."\'>".trim($arvore["epfcodigo"])." - ".trim($arvore["epfdescricao"])."</a>','javascript:void(0);');\n";
						}
					}
				}

				/*$tree .= "arvore.add('1_i','i_1','".carregaImagem('p1')." <a href=\'?modulo=principal/instrumento1/p1&acao=A\'>1-6 Dados da Escola</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('7_i','i_1','".carregaImagem('p7')." <a href=\'?modulo=principal/instrumento1/p7&acao=A\'>7 Nível e modalidade de ensino ministrados na escola</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('8_i','i_1','".carregaImagem('p8')." <a href=\'?modulo=principal/instrumento1/p8&acao=A\'>8 - Dependências escolares e condições de uso</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9_i','i_1','9 - Ensino Fundamental','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.1_i','9_i','".carregaImagem('p9_1')." <a href=\'?modulo=principal/instrumento1/p9_1&acao=A\'>9.1 - Matrícula inicial (ano anterior)</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.2_i','9_i','".carregaImagem('p9_2')." <a href=\'?modulo=principal/instrumento1/p9_2&acao=A\'>9.2 - Aproveitamento dos alunos 1ª - 4ª (ano anterior)</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.3_i','9_i','".carregaImagem('p9_3')." <a href=\'?modulo=principal/instrumento1/p9_3&acao=A\'>9.3 - Aproveitamento dos alunos 5ª - 8ª (ano anterior)</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.4_i','9_i','".carregaImagem('p9_4')." <a href=\'?modulo=principal/instrumento1/p9_4&acao=A\'>9.4 - Aproveitamento dos alunos Ciclo/Etapa (ano anterior)</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.5_i','9_i','".carregaImagem('p9_5')." <a href=\'?modulo=principal/instrumento1/p9_5&acao=A\'>9.5 - Disciplinas críticas</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.6_i','9_i','".carregaImagem('p9_6')." <a href=\'?modulo=principal/instrumento1/p9_6&acao=A\'>9.6 - Distorção idade-série - 1ª a 4ª séries (ano anterior)</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.7_i','9_i','".carregaImagem('p9_7')." <a href=\'?modulo=principal/instrumento1/p9_7&acao=A\'>9.7 - Distorção idade-série - 5ª a 8ª séries (ano anterior)</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.8_i','9_i','".carregaImagem('p9_8')." <a href=\'?modulo=principal/instrumento1/p9_8&acao=A\'>9.8 - Recursos Humanos</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.8_a','9.8_i','".carregaImagem('p9_8a')." <a href=\'?modulo=principal/instrumento1/p9_8a&acao=A\'>9.8.a - Pessoal técnico de acordo com a formação</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.8_b','9.8_i','".carregaImagem('p9_8b')." <a href=\'?modulo=principal/instrumento1/p9_8b&acao=A\'>9.8.b - Relação aluno/docente e aluno/não-docente</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('9.8_c','9.8_i','".carregaImagem('p9_8c')." <a href=\'?modulo=principal/instrumento1/p9_8c&acao=A\'>9.8.c - Há turmas ou disciplinas sem professor? Se a resposta for afirmativa, especifique.</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('10_i','i_1','10 - Ensino Médio','javascript:void(0);');\n";
				 $tree .= "arvore.add('10.1_i','10_i','".carregaImagem('p10_1')." <a href=\'?modulo=principal/instrumento1/p10_1&acao=A\'>10.1 - Matrícula inicial</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('10.2_i','10_i','".carregaImagem('p10_2')." <a href=\'?modulo=principal/instrumento1/p10_2&acao=A\'>10.2 - Aproveitamento dos alunos (ano anterior)</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('11_i','i_1','".carregaImagem('p11')." <a href=\'?modulo=principal/instrumento1/p11&acao=A\'>11 - Fontes e destinação dos recursos utilizados pela Escola</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('12_i','i_1','".carregaImagem('p12')." <a href=\'?modulo=principal/instrumento1/p12&acao=A\'>12 - Previsão de recursos da escola para o ano corrente, segundo fontes</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('13_i','i_1','".carregaImagem('p13')." <a href=\'?modulo=principal/instrumento1/p13&acao=A\'>13 - A escola provê para os alunos</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('14_i','i_1','".carregaImagem('p14')." <a href=\'?modulo=principal/instrumento1/p14&acao=A\'>14 - Liste as medidas ou projetos que estão sendo implantados na atual administração</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('15_i','i_1','".carregaImagem('p15')." <a href=\'?modulo=principal/instrumento1/p15&acao=A\'>15 - Como a escola implantou as medidas ou projetos?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('16_i','i_1','".carregaImagem('p16')." <a href=\'?modulo=principal/instrumento1/p16&acao=A\'>16 - O que mudou com a implantação das medidas ou projetos em relação à situação anterior</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('17_i','i_1','".carregaImagem('p17')." <a href=\'?modulo=principal/instrumento1/p17&acao=A\'>17 - Como a Secretaria de Educação trabalhou com a escola?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('18_i','i_1','".carregaImagem('p18')." <a href=\'?modulo=principal/instrumento1/p18&acao=A\'>18 - Qual tem sido a participação dos professores e demais funcionários nas medidas e projetos implementados pela escola?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('19_i','i_1','".carregaImagem('p19')." <a href=\'?modulo=principal/instrumento1/p19&acao=A\'>19 - Qual a participação do Colegiado/Conselho Escolar?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('20_i','i_1','".carregaImagem('p20')." <a href=\'?modulo=principal/instrumento1/p20&acao=A\'>20 - A execução das medidas ou projetos envolveu parceria com outras instituições (ONGs, empresas, sindicatos etc.)?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('21_i','i_1','".carregaImagem('p21')." <a href=\'?modulo=principal/instrumento1/p21&acao=A\'>21 - Como a escola avalia sua relação com a Secretaria de Educação?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('22_i','i_1','".carregaImagem('p22')." <a href=\'?modulo=principal/instrumento1/p22&acao=A\'>22 - Como a escola avalia sua relação com a comunidade?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('23_i','i_1','".carregaImagem('p23')." <a href=\'?modulo=principal/instrumento1/p23&acao=A\'>23 - Qual a forma de seleção do diretor(a) para a escola?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('24_i','i_1','".carregaImagem('p24')." <a href=\'?modulo=principal/instrumento1/p24&acao=A\'>24 - A taxa de rotatividade dos professores e funcionários, nos últimos três anos, tem afetado o desempenho de escola? ...</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('25_i','i_1','".carregaImagem('p25')." <a href=\'?modulo=principal/instrumento1/p25&acao=A\'>25 - Qual o percentual de professores com jornada de trabalho em tempo integral na escola, atualmente?</a>','javascript:void(0);');\n";
				 $tree .= "arvore.add('34_i','i_1','Ficha Resumo','javascript:void(0);');\n";
				 $tree .= "arvore.add('34.1_i','34_i',' <a href=\'?modulo=principal/instrumento1/fr1&acao=A\'>Ficha resumo 1 - Funcionamento da Escola </a>','javascript:void(0);');\n";*/

				//$tree .= "arvore.add('{$dado['entid']}','{$dado['muncod']}','".simec_htmlentities($dado['entnome'],ENT_QUOTES)."');\n";
				/*
				 * Se ouver registro no "pdeescola"
				 * Monta instrumento 2
				 */
				if ($dado['entid']){
					$tree .= "arvore.add('i_2','{$dado['entid']}','Instrumento 2');\n";

					foreach ($perg as $perg):
					/*
					 if($perg['dacid'] == NULL)
						$img = "<img src=\"../imagens/atencao.png\" style=\"border:0;\" title=\"Não Respondido\">";
						else
						$img = "<img src=\"../imagens/check_p.gif\" style=\"border:0;\" title=\"Respondido\">";
						*/
					
					$link = "?modulo=principal/instrumento2/cadastro_instrumento2&acao=A&aceid={$perg['aceid']}";
					$link2 = "?modulo=principal/instrumento2/cadastro_criticidade&acao=A";
					$link3 = "?modulo=principal/instrumento2/cadastro_prioridade&acao=A";
					$link4 = "?modulo=principal/instrumento2/total_pontos&acao=A";
					$link5 = "?modulo=principal/sintese_autoavaliacao/problemas_criterios&acao=A";
					$link6 = "?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A";
					$link7 = "?modulo=principal/sintese_autoavaliacao/previsao_recursos&acao=A";
					$link8 = "?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A"; 
					
					if(in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfil) ||
					   in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfil)){
					   	
					   	if($esdid == AVALIACAO_COMITE_ME_WF){
						
							$link = "javascript:void(0)";
							$link2 = "javascript:void(0)";
							$link3 = "javascript:void(0)";
							$link4 = "javascript:void(0)";
							$link5 = "javascript:void(0)";
							$link6 = "javascript:void(0)";
							$link7 = "javascript:void(0)";
							$link8 = "javascript:void(0)";
					   	}
					}

					if( trim( $_SESSION['estado'] ) != 'PR')
					$texto = simec_htmlentities(str_replace(array('\n','\r','<br>','</br>',chr(10)),'',trim($perg['acecodigo'])." - ". (strlen($perg['acedescricao']) > 110 ? substr($perg['acedescricao'],0,110).'...' : $perg['acedescricao'] )),ENT_QUOTES);
					else
					$texto = simec_htmlentities(str_replace(array('\n','\r','<br>','</br>',chr(10)),'',trim($perg['acecodigo'])." - ". (strlen($perg['acedescricaopr']) > 110 ? substr($perg['acedescricaopr'],0,110).'...' : $perg['acedescricaopr'] )),ENT_QUOTES);

					$param_img = 'cadastro_instrumento2_'.$perg['aceid'];

					if ( !$perg['aceidpai'] || is_numeric(trim($perg['acecodigo'])) ){
						$tree .= "arvore.add('{$perg['aceid']}','". ($perg['aceidpai'] ? $perg['aceidpai'] : 'i_2') ."','{$texto}','javascript:void(0);');\n";
					}else{
						$tree .= "arvore.add('{$perg['aceid']}','{$perg['aceidpai']}','".carregaImagem(trim($param_img))." <a href=\'{$link}\'>".$texto."</a>','javascript:void(0)');\n";
					}
					endforeach;
				}

				/*
				 * Itens staticos, instrumento 2
				 */
				$tree .= "arvore.add('2f','i_2','Ficha resumo');\n";
				$tree .= "arvore.add('2f.2','2f','".carregaImagem(trim('cadastro_criticidade'))."<a href=\'{$link2}\'>Criticidade</a>');\n";
				$tree .= "arvore.add('2f.3','2f','".carregaImagem(trim('cadastro_prioridade'))."<a href=\'{$link3}\'>Prioridade</a>');\n";
				$tree .= "arvore.add('2f.1','2f','<a href=\'{$link4}\'>Total de pontos</a>');\n";
				/*
				 * Adiciona totais 1º nível
				 */
				if ($total_1[0]){
					foreach ($total_1 as $tot_1):
					$tree .= "arvore.add('tot_{$tot_1['aceid']}','{$tot_1['aceid']}','<B>Total:</B> {$tot_1['esavalor']}','javascript:void(0);');\n";
					endforeach;
				}

				/*
				 * Adiciona totais 2º nível
				 */
				if ($total[0]){
					foreach ($total as $tot):
					$tree .= "arvore.add('tot_{$tot['aceid']}','{$tot['aceid']}','<B>Total:</B> {$tot['esavalor']}','javascript:void(0);');\n";
					endforeach;
				}

				/*
				 * Adiciona Instrumento 3
				 */
				$tree .= "arvore.add('i_3','{$dado['entid']}','Síntese da autoavaliação');\n";

				$tree .= "arvore.add('3f.1','i_3','".carregaImagem(trim('popupProblemaCriterio'))."<a href=\'{$link5}\'>Problemas x Critérios de eficácia escolar</a>');\n";
				$tree .= "arvore.add('3f.2','i_3','".carregaImagem(trim('problemas_causas_acoes'))."<a href=\'{$link6}\'>Problemas x Causas x Ações</a>');\n";
				$tree .= "arvore.add('3f.3','i_3','<a href=\'{$link7}\'>Previsão de Recursos</a>');\n";

				$tree .= "arvore.add('i_4','{$dado['entid']}','Plano de Ação');\n";
				$tree .= "arvore.add('3f.4','i_4','".carregaImagem(trim('popupObjetivosMetas'))."<a href=\'{$link8}\'>Objetivos Estratégicos, Estratégias e Metas</a>');\n";
			}

		}


		$tree .= "  elemento = document.getElementById( '_arvore' );
			    elemento.innerHTML = arvore;
			  </script>";

		return $tree;
}

/*
 * Montar cabeçalho do sistema
 */
function cabecalhoPDE($orientacoes = NULL){
	global $db, $docid;

	$entid = $_SESSION['entid'];
	//xd($_SESSION);
	pde_verificaSessao();
	$sql = "SELECT
	DISTINCT
	est.estdescricao as est,
	est.estuf,
	mun.mundescricao as mun,
	ent.entnome as esc,
	--ed.entpdevlrpaf as vlrpaf,
	coalesce(pde.pdevlrplanocapital,0) as pdevlrplanocapital,
	coalesce(pde.pdevlrplanocusteio,0) as pdevlrplanocusteio,
	coalesce(pde.pdevlrcomplementarcusteio,0) as pdevlrcomplementarcusteio,
	coalesce(pde.pdevlrcomplementarcapital,0) as pdevlrcomplementarcapital  
	FROM
	entidade.entidade ent
	INNER JOIN entidade.funcaoentidade as fe ON fe.entid = ent.entid
	INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
	INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid
	INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
	INNER JOIN territorios.estado est ON est.estuf = mun.estuf
	INNER JOIN pdeescola.pdeescola pde ON ent.entid = pde.entid
	WHERE
	fe.funid = 3 and
	ent.tpcid IN (1,3) AND
	ent.entid IN ('{$entid}')";

	$dados = $db->carregar($sql);

//	$percent_capital = ($dados[0]['vlrpaf'] * 30)/100;
//	$percent_custeio = ($dados[0]['vlrpaf'] * 70)/100;

	$percent_capital = $dados[0]['pdevlrplanocapital'];
	$percent_custeio = $dados[0]['pdevlrplanocusteio'];
	$totalPAF = $percent_capital + $percent_custeio;
	
	$complementar_capital = $dados[0]['pdevlrcomplementarcapital'];
	$complementar_custeio = $dados[0]['pdevlrcomplementarcusteio'];
	$totalComplementar = $complementar_capital + $complementar_custeio;

	$escpaga = verificaEscolaPagaWs();

	 	if( $escpaga == 'errowebservice' ){
			$textPago = "<a style=\"color: red;\" >( Serviço fora do ar )</a>";
		}elseif( $escpaga == 't' ){
			$textPago = "<a style=\"color: red;\" >( Escola Paga )</a>";
		} else {
			$textPago = "";
		}
	
	$esdid = pegarEstadoAtual($docid);

	//if($esdid == VALIDACAO_PELO_MEC_WF){		
	
		if($totalPAF != 0){
			$valoresPAF = "".number_format($totalPAF,2,',','.')." - ( <b>Custeio: </b> R$".number_format($percent_custeio,2,',','.')."  -  <b>Capital:</b> R$".number_format($percent_capital,2,',','.')." )". '  '. $textPago  ."";
		} elseif($escpaga == 't') {
			$valoresPAF = "<span style=\"color:red;\"><b>Escola já recebeu a Parcela Principal.</b></span>";
		} else {	
			//$valoresPAF = "<span style=\"color:red;\"><b> &nbsp;&nbsp;&nbsp;-- </b></span>";
			$valoresPAF = "".number_format($totalPAF,2,',','.')." - ( <b>Custeio: </b> R$".number_format($percent_custeio,2,',','.')."  -  <b>Capital:</b> R$".number_format($percent_capital,2,',','.')." )". '  '. $textPago  ."";
		}
		
		if($totalComplementar != 0){
			$valoresComplementar = "".number_format($totalComplementar,2,',','.')." - ( <b>Custeio: </b> R$".number_format($complementar_custeio,2,',','.')."  -  <b>Capital:</b> R$".number_format($complementar_capital,2,',','.')." )";
		} else {
			$valoresComplementar = "<span style=\"color:red;\"><b>Não se aplica.</b></span>";
		}
		
//	} else {
//		$valoresPAF = "".number_format($totalPAF,2,',','.')." - ( <b>Custeio: </b> R$".number_format($percent_custeio,2,',','.')."  -  <b>Capital:</b> R$".number_format($percent_capital,2,',','.')." )". '  '. $textPago  ."";
//		$valoresComplementar = "".number_format($totalComplementar,2,',','.')." - ( <b>Custeio: </b> R$".number_format($complementar_custeio,2,',','.')."  -  <b>Capital:</b> R$".number_format($complementar_capital,2,',','.')." )";
//	}
	
	
	
	//$textPago = "Sem conexão no Momento";
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
	<tr>
	<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor PAF (R$): </td>
	<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$valoresPAF}</td>
	</tr>
	<tr>
	<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Valor Parcela Complementar (R$): </td>
	<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$valoresComplementar}</td>
	</tr>	
	<tr>
	<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloEsquerda\">Orientações: </td>
	<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">{$orientacoes}</td>
	</tr>	
	</tbody>
	</table>";
	$_SESSION['estado'] = $dados[0]['estuf'];
	return $cab;
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
								  AND p.sisid = ".SISID_PDE_ESCOLA."
			WHERE
				pu.usucpf = '".$_SESSION['usucpf']."'
			ORDER BY
				p.pflnivel";
	$pflcod = $db->carregarColuna($sql);
	
	/*** Retorna o array com o(s) perfil(is) ***/
	return (array)$pflcod;
}

/*
 * Montar sub-cabeçalho, telas de perguntas
 */
function subCabecalho($texto){
	$subCab = "<table class=\"listagem\" width=\"95%\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
	<thead>
	<tr>
	<td><b>{$texto}</b></td>
	</tr>
	</thead>
	</table>";
	return $subCab;
}

/*
 * Direciona o usuario
 * de acordo com os registros em "usuarioresponsabilidade"
 */
function direcionaUsuario (){
	global $db;

	/*
	 * Carrega array com perfis do usuário
	 */
	$perfil = arrayPerfil();

	if ( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) || in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil) ) {
		return;
	}
	$sql = sprintf("SELECT
					 ur.pflcod,
					 estuf,
					 muncod,
					 entid
					FROM
					 pdeescola.usuarioresponsabilidade ur
					WHERE
					 rpustatus = 'A' AND
					 ur.pflcod IN (".implode(',',$perfil).") AND
					 ur.usucpf = '%s'",
	$_SESSION['usucpf']);

	$resp = $db->carregar($sql);

	if (count($resp) == 1 && $resp){
		if ( $resp[0]['pflcod'] == PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL || $resp[0]['pflcod'] == PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL ){
			$_SESSION['entid'] = $resp[0]['entid'];
			#$param = array("entid" => $resp[0]['entid']);
			if ($_GET['modulo'] != 'principal/estrutura_avaliacao'){
				die ('<script>
						location.href=\'?modulo=principal/estrutura_avaliacao&acao=A\';
					  </script>');
			}
		}
	}

	return;
}

/*
 * Monta lista de Escolas
 * Em conformidade com o filtro
 */
function lista(){
	
	global $db;

	// Filtros	
	if ($_POST['escola'])
		$where[] = " UPPER(p.entnome) LIKE UPPER('".trim(tratarStrBusca($_POST['escola']))."')";

	if ($_POST['entcodent'])
		$where[] = " p.entcodent LIKE '%".trim($_POST['entcodent'])."%'";

	if ($_REQUEST['estuf'])
		$where[] = " p.estuf = '".$_REQUEST['estuf']."'";

	if ($_POST['muncod'])
		$where[] = " p.muncod = '".$_POST['muncod']."'";
	
	if($_REQUEST['esdid'])
		$where[] = " p.esdid = '".$_REQUEST['esdid']."'";
	
	if ($_POST['preenchimento1'] AND $_POST['preenchimento2'])		
		$where[] = " p.total BETWEEN ".$_POST['preenchimento1']." AND ".$_POST['preenchimento2']." ";
	
	if( ( $_POST['preenchimento1'] == '') && ( $_POST['preenchimento2'] != ''))
		$where[] = " p.total BETWEEN 0 AND ".$_POST['preenchimento2']." ";
	
	if( ( $_POST['preenchimento2'] == '') && ( $_POST['preenchimento1'] != ''))
		$where[] = " p.total BETWEEN ".$_POST['preenchimento1']." AND 100  ";
	
	if ($_REQUEST['parcela'] == 'true')
		$where[] = " p.parcela not ilike '%--%'";
	if ($_REQUEST['parcela'] == 'false')
		$where[] = " p.parcela ilike '%--%'";
	
	if ($_REQUEST['pontuacao'])
		$where[] = " p.pontuacao NOT ILIKE '%--%'";
	
	if ($_POST['tpcid'])
		$where[] = " p.tpcid IN (".$_POST['tpcid'].")";
	else
		$where[] = " p.tpcid IN (1,3)";

	if ($_POST['epiclasse']){
		if( $_POST['epiclasse'] == 'A')
			$var = " = 'A'";
		else if($_POST['epiclasse'] == 'B')
			$var = " = 'B'";
		else if($_POST['epiclasse'] == 'C')
			$var = " = 'C'";
		
		$where[] = " p.epiclasse $var";
	}else{
		$var = "= '{$_POST['epiclasse']}'";
	}

	if($_REQUEST['pafpago'] != ''){
		if( $_REQUEST['pafpago'] == 'Pago' )
			$where[] = " pdepafretorno IS NOT NULL";
		else if( $_REQUEST['pafpago'] == '--' )
			$where[] = " pdepafretorno IS NULL";		
	}

	if ($_POST['eavvalor']){
		
		if(($_POST['eavvalor'] == 'A'))		
			$where[] = "  eavvalor >  5  AND eavvalor <=  55 AND qtdap = 55 AND pdepafretorno IS NOT NULL";		
		else if(($_POST['eavvalor'] == 'B'))		
			$where[] = "  eavvalor >  55 AND eavvalor <= 110 AND qtdap = 55 AND pdepafretorno IS NOT NULL";		
		else if(($_POST['eavvalor'] == 'C'))		
			$where[] = "  eavvalor > 110 AND eavvalor <= 165 AND qtdap = 55 AND pdepafretorno IS NOT NULL";		
		else if(($_POST['eavvalor'] == 'D'))		
			$where[] = "  eavvalor > 165 AND eavvalor <= 220 AND qtdap = 55 AND pdepafretorno IS NOT NULL";		
		else if(($_POST['eavvalor'] == 'E'))		
			$where[] = "  eavvalor > 220 AND eavvalor <= 275 AND qtdap = 55 AND pdepafretorno IS NOT NULL";
	}
	
	if($_POST['datatramite_inicio'] && $_POST['datatramite_fim'])
	{
		$inicio = explode("/",$_POST['datatramite_inicio']);
		$inicio = $inicio[2]."-".$inicio[1]."-".$inicio[0];
		$fim = explode("/",$_POST['datatramite_fim']);
		$fim = $fim[2]."-".$fim[1]."-".$fim[0];
		$where[] = " datatramitacao BETWEEN '".$inicio."' AND '".$fim."' ";
	}
	
	// Carrega array com perfis do usuário	
	$perfil = arrayPerfil();

	// Vê somente o que tiver acesso, atravéz do "usuarioresponsabilidade
	if ( !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) && !in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil)  && !in_array(PDEESC_PERFIL_CONSULTA, $perfil) ) {
		$from[] = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND
					ur.pflcod IN (".implode(',',$perfil).") AND
					ur.usucpf = '".$_SESSION['usucpf']."' AND
					(
					 (ur.muncod = p.muncod AND
					  p.tpcid = 3) OR
 					 ur.entid  = p.entid OR
 					 (ur.estuf  = p.estuf AND
 					  p.tpcid = 1)
 					)";
	}	

	// Monta sql
	$sql = sprintf("SELECT
						acao,
						p.entcodent,
						entnome ,
						tipo,
						epiclasse,
						p.estuf,
						mundescricao,
						situacao,
						pp,
						maiseduc,
						datatramite,
						ocupacao,
						pafpago,
						p.parcela,
						pontuacao						
					FROM pdeescola.preenchimento p
						%s
					WHERE p.entcodent is not null
						%s
					GROUP BY
						acao,
						p.entcodent,
						entnome ,
						tipo,
						epiclasse,
						p.estuf,
						mundescricao,
						situacao,
						pp,
						maiseduc,
						datatramite,
						ocupacao,
						p.pdepafretorno,
						p.parcela,
						p.esdid, pafpago,
						pontuacao						
						$having ",
					$from ? implode(' ', $from) : ' ',
					$where ? " AND  ".implode(' AND ', $where) : ' ');
					
//					ver($sql);
//					die();

	$cabecalho = array( "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ação&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", "Cód", "Escola", "Tipo","Classe IDEB", "UF", "Município", "Situação", "Preenchimento", "ME", "Data Tramitação", "Ocupação", "PAF", "PC", "Resultado da autoavaliação");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', '', '', 3600);
}

/*
 * Trata string para busca no SQL
 * ADD '%' nos espaços da string, quando maior que três caracteres
 */
function tratarStrBusca($str){
	$str = explode(" ",$str);
	foreach ($str as $str):
	$text .= strlen($str) >= 3 ? "%".$str : '';
	endforeach;

	return $text."%";
}
/*
 ********************************** FUNÇÕES WORKFLOW ***************************
 */
/*
 * Pegar docid em "pdeescola.pdeescola"
 */
function pegarDocid( $entid ) {
	global $db;
	$entid = (integer) $entid;
	$sql = "SELECT
			 docid
			FROM
			 pdeescola.pdeescola
			WHERE
			 pdeano = " .ANO_EXERCICIO_PDE_ESCOLA. " AND
			 entid  = " . $entid;

	return $db->pegaUm( $sql );

}


/*
 * Pegar pdeid em "pdeescola.pdeescola"
 */
function pegarCodInep( $entid ) {
	global $db;
	$sql = "SELECT
			 entcodent
			FROM
			 entidade.entidade
			WHERE
			  entid  = " . $entid;
	return $db->pegaUm( $sql );
}

/*
 * Pegar pdeid em "pdeescola.pdeescola"
 */
function pegarPdeid( $entid ) {
	global $db;
	$sql = "SELECT
			 pdeid
			FROM
			 pdeescola.pdeescola
			WHERE
			 pdeano = " .ANO_EXERCICIO_PDE_ESCOLA. " AND
			 entid  = " . $entid;

	$pdeid = $db->pegaUm( $sql );
	/*
	 * Verifica se há pdeescola, se sim
	 * Atualiza docid no pdeescola, senão
	 * Insere pdeescola
	 */
	if (!$pdeid){
		$pdecodinep = pegarCodInep($entid);
		
		//pega valores
		$sqlv = "SELECT
			 \"Custeio Plano\" as cup,
			 \"Capital Plano\" as cap,
			 \"Custeio PC\" as cupc,
			 \"Capital PC\" as capc
			FROM
			 pdeescola.\"PDE_2010_2405\"
			WHERE
			  \"COD INEP\"  = '".$pdecodinep."'";
		$valores = $db->pegaLinha( $sqlv );

		if(!$valores['cup']) $valores['cup'] = 0;
		if(!$valores['cap']) $valores['cap'] = 0;
		if(!$valores['cupc']) $valores['cupc'] = 0;
		if(!$valores['capc']) $valores['capc'] = 0;

		$sql = "INSERT INTO pdeescola.pdeescola
				(
				 entid,
				 pdeano,
				 pdecodinep,
				 pdevlrplanocusteio,
				 pdevlrplanocapital,
				 pdevlrcomplementarcusteio,
				 pdevlrcomplementarcapital
				)VALUES(
				 ".$entid.",
				 ".ANO_EXERCICIO_PDE_ESCOLA.",
				'" . $pdecodinep . "',
				".$valores['cup'].",
				".$valores['cap'].",
				".$valores['cupc'].",
				".$valores['capc']."
				) returning pdeid";
		$pdeid = $db->pegaUm($sql);
	}
	return $pdeid;
}
/*
 * Selecionar entidade
 *
 */
function selecionarEntidade ($entid = null){
	global $db;
	if (!$entid)
	return false;

	$sql = "SELECT
	 		 e.entid
	 		FROM
	 		 entidade.entidade e
			 inner join entidade.entidadedetalhe ed on e.entid=ed.entid
	 		WHERE
	 		 ed.entpdeescola='t' and e.entid = ".$entid;
	$entid = $db->pegaUm($sql);
	if ($entid) {$_SESSION['entid'] = $entid; return true;} else {return false;}

}

/*
 * Criar Documento =>
 * "workflow.documento" e "pdeescola.pdeescola"
 */
function criarDocumento( $entid ) {
	global $db;
	if( ! pegarDocid($entid)){
		$pdeid = pegarPdeid($entid);
		$tpdid = TPDID_PDE_ESCOLA;

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

		$docdsc = "PDE Escola(".$pdeid.") - Entidade(" . $entid . ') - ' . $descricao;

		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );


		$sql = "UPDATE pdeescola.pdeescola SET
				 docid = ".$docid."
				WHERE
				 pdeid = ".$pdeid;
		$db->executar( $sql );
		$db->commit();
	}
}
/*
 * função para verificar a quantidade de questões preenchidas do pdeescola;
 * retorna true caso ele tenha preenchido todas as questões
 * retorna false caso nao tenha.
 * author: PedroDantas;
 * date: 25/11/2008
 */
if($_SESSION['pdeid']){
	function verificaPreenchimento()
	{
		global $db;

		$sql = "SELECT count(pprid) FROM pdeescola.pdepreenchimento WHERE pdeid = '{$_SESSION['pdeid']}'";
		$qtdQuestoes = $db->pegaUm( $sql );


		$maxQuestoes = retornaNumeroQuestoes($_SESSION['pdeid']);
		if($maxQuestoes == '')
		{
			$maxQuestoes = 210;
		}

		//limpando a sessão
		$_SESSION['erros'] = null;

		if( $qtdQuestoes < $maxQuestoes )
		{
			$_SESSION['erros']['preenchimento'] = "erro1";
		}

		if(verificaProgramas() == true){
			return true;
		}else{
			return false;
		}
		
		//return true;
	}
}

/*
 * função para liberar link para tramitar "Enviar para monitoramento" ,verificando se todas as questões foram preenchidas da Autoavaliação;
 * retorna true depois de verificar que todas as questoes foram preenchidas.
 * author: Marilúcia Cardozo;
 * date: 15/10/2009
 */
function verificaPreenchimentoMonitoramento()
{
	global $db;
	$sql = " SELECT
				count( q.qapid )
			 FROM
			    pdeescola.questaoavaliacaoplano as q
			 INNER JOIN
			 	pdeescola.avaliacaoplano as a ON a.qapid = q.qapid
			 WHERE
			    a.pdeid = ".$_SESSION['pdeid'];

	$qtdQuestoes = $db->pegaUm( $sql );
	$maxQuestoes = 55;
	if($qtdQuestoes < $maxQuestoes){
		$_SESSION['pendencias_monitoramento'] = true;
		return false;
	}else{
		$_SESSION['pendencias_monitoramento'] = false;
		return true;
	}
}



/*
 * function: verificaProgramas()
 * author:   PedroDantas
 * date:     22/12/2008
 * params:   no
 * desc:     verifica os tipos de programa selecionados no preenchimento do plano de acao.
 * 			 caso não tenha sido preenchido todos os programas será necessário justificar.
 * 			 a função apenas retorna um array com todos os ids de programas não informados
 * 			 para este pdeid.
 * returns:  array();
 */
function verificaProgramas()
{
	global $db;

	confereValorPaf();

	$sqlProgramas =
			"
			SELECT
			distinct tprid
			FROM pdeescola.tipoprograma
			ORDER BY tprid
			";
	$arrayProgramas = $db->carregar( $sqlProgramas );

	for( $i=0; $i< count($arrayProgramas); $i++)
	{
		if( ($arrayProgramas[$i]['tprid'] == 1) || ($arrayProgramas[$i]['tprid'] == 2) )
		{
			$arrProgramas_[$i] = $arrayProgramas[$i]['tprid'];
		}
	}

	for( $l = 0; $l < count( $arrProgramas_ ); $l++)
	{
		$arrProgramas[$l] = $arrProgramas_[$l];
	}

	$sqlProgramasPde = "
	SELECT
	distinct dp.tprid
	FROM pdeescola.detalheplanoacao AS dp
	INNER JOIN pdeescola.planoacao AS pl ON pl.plaid = dp.plaid
	INNER JOIN pdeescola.planosuporteestrategico AS ps ON ps.pseid = pl.pseid
	WHERE ps.pdeid = '{$_SESSION['pdeid']}'
	ORDER BY dp.tprid
			";
	$arrayProgramasPde = $db->carregar( $sqlProgramasPde );

	for( $i=0; $i< count($arrayProgramasPde); $i++)
	{
		if( ($arrayProgramasPde[$i]['tprid'] == 1) || ($arrayProgramasPde[$i]['tprid'] == 2) )
		{
			$arrProgramasPde_[$i] = $arrayProgramasPde[$i]['tprid'];
		}
	}

	for( $l = 0; $l < count( $arrProgramasPde_ ); $l++)
	{
		$arrProgramasPde[$l] = $arrProgramasPde_[$l];
	}

	if( $arrProgramasPde == false )
	{
		$arrProgramasPde = array();
		$arrProgramasPde[0] = 'null';
	}

	$arrayPendentes_ = array_diff( $arrProgramas  , $arrProgramasPde  );
	$arrayPendentes  = array_values( $arrayPendentes_ );

	//verifica se ja não tem algo justificado
	$sqlJust = "select op.tprid from pdeescola.justificativa AS ju
	left join pdeescola.opcaojustificativa AS op on ju.opjid = op.opjid
	where ju.pdeid = '{$_SESSION['pdeid']}'";


	$arrJustificados = $db->carregar( $sqlJust );

	for( $i=0; $i< count($arrJustificados); $i++)
	{
		if( ($arrJustificados[$i]['tprid'] == '1') || ($arrJustificados[$i]['tprid'] == '2') )
		{
			$arrJustificados_[$i] = $arrJustificados[$i]['tprid'];
		}
	}

	for( $l = 0; $l < count( $arrJustificados_ ); $l++)
	{
		$arrJustificados[$l] = $arrJustificados_[$l];
	}

	if(count($arrJustificados) > 0 && is_array($arrJustificados)) //Comentário teste
	{
		$arrayPend = array_diff($arrayPendentes, $arrJustificados);
	}
	else
	{
		$arrayPend = $arrayPendentes;
	}


	if( count( $arrayPend ) > 0 && is_array($arrayPend))
	{
		$_SESSION['erros']['programas_pendentes'] = array();
		$_SESSION['erros']['programas_pendentes'] = $arrayPend;
	}
	if($_SESSION['erros']){
		return false;
	}
	return true;

}

/*
 * function: carregaJustificativas()
 * date:     12/03/2009
 * params:   no
 * desc:     Exibe as justificativas preenchidas PDEESCOLA
 * returns:  array();
 */
function carregaJustificativas()
{
	global $db;

	//verifica se ja não tem algo justificado
	$sqlJust = "select op.tprid from pdeescola.justificativa AS ju
	left join pdeescola.opcaojustificativa AS op on ju.opjid = op.opjid
	where ju.pdeid = '{$_SESSION['pdeid']}'";


	$arrJustificados = $db->carregar( $sqlJust );

	for( $i=0; $i< count($arrJustificados); $i++)
	{
		if( ($arrJustificados[$i]['tprid'] == '1') || ($arrJustificados[$i]['tprid'] == '2') )
		{
			$arrJustificados_[$i] = $arrJustificados[$i]['tprid'];
		}
	}

	for( $l = 0; $l < count( $arrJustificados_ ); $l++)
	{
		$arrJustificados[$l] = $arrJustificados_[$l];
	}

	if( count( $arrJustificados ) > 0 )
	{
		$_SESSION['pendentes']['programas_pendentes'] = array();
		$_SESSION['pendentes']['programas_pendentes'] = $arrJustificados;
	}

}

/*
 * function: pegarEstadoAtual()
 * date:     16/03/2009
 * params:   $docid
 * desc:     Carrega o estado atual do workflow
 * returns:  $estado;
 */
function pegarEstadoAtual( $docid )
{
	global $db;
	$docid = (integer) $docid;

	$sql = "
		select
			ed.esdid,
			ed.esddsc
		from workflow.documento d
			inner join workflow.estadodocumento ed on ed.esdid = d.esdid
		where
			d.docid = " . $docid;
	$estado = $db->pegaUm( $sql );

	return $estado;
}

function retornaNumeroQuestoes( $pdeid )
{
	global $db;
	$sql ="SELECT t.tmeid as id,
		   n.tmeid as selecionado,
		   tmedescricao as descricao
		   FROM pdeescola.tiponivelmodalidadeensino t
		   left join pdeescola.nivelmodalidadeensino n on n.tmeid = t.tmeid AND
		   pdeid = ".$pdeid."
		  -- WHERE N.tmeid IS NOT NULL";

	$rstudo = $db->carregar( $sql );

	$arrModalidades = array();
	for( $i = 0; $i< count( $rstudo ); $i++ )
	{
		if( $rstudo[$i]['selecionado'] == 1)
		{
			array_push($arrModalidades, MAX_QUESTOES_PRE_ESCOLA);
		}
		if( $rstudo[$i]['selecionado'] == 2)
		{
			array_push($arrModalidades, MAX_QUESTOES_ENS_1_A_4);
		}
		if( $rstudo[$i]['selecionado'] == 3)
		{
			array_push($arrModalidades, MAX_QUESTOES_ENS_5_A_8);
		}
		if( $rstudo[$i]['selecionado'] == 6)
		{
			array_push($arrModalidades, MAX_QUESTOES_ENS_MEDIO);
		}
		if( $rstudo[$i]['selecionado'] == 9)
		{
			array_push($arrModalidades, MAX_QUESTOES_CRECHE);
		}
		if( $rstudo[$i]['selecionado'] == 10)
		{
			array_push($arrModalidades, MAX_QUESTOES_EJA);
		}

	}
	rsort( $arrModalidades );
	$maxQ = $arrModalidades[0];

	$possuiProjetos = verificaProjetos( $pdeid );
	if( !$possuiProjetos ){
		$maxQuestoes = ($maxQ - 7);
	}
	return $maxQuestoes;
}


function confereValorPaf()
{
	global $db;


//	$sqlPAF = " SELECT
//	ed.entpdevlrpaf as vlrpaf
//	FROM
//	entidade.entidadedetalhe AS ed
//	WHERE
//	ed.entid = '{$_SESSION['entid']}'";

	$sqlPAF = " SELECT
	coalesce(p.pdevlrplanocusteio,0) as custeio,
	coalesce(p.pdevlrplanocapital,0) as capital
	FROM
	pdeescola.pdeescola AS p
	WHERE
	p.entid = '{$_SESSION['entid']}'";	

	$vlrpaf = $db->carregar( $sqlPAF );

//	$percent_capital = ( $vlrpaf * 30 )/100;
//	$percent_custeio = ( $vlrpaf * 70 )/100;
//	$percent_MAX     = ( $percent_custeio * 15 )/100;
	
	$percent_capital = $vlrpaf[0]['capital']; 
	$percent_custeio = $vlrpaf[0]['custeio'];
	$percent_MAX     = ( $percent_custeio * 15 )/100;

	$sqlvalor = "SELECT
	p.pseid,
	d.dpavalorcusteio,
	d.dpavalorcapital
	FROM
	pdeescola.planoacao AS p
	INNER JOIN
	pdeescola.detalheplanoacao as d ON d.plaid = p.plaid
	INNER JOIN
	pdeescola.planosuporteestrategico AS s ON p.pseid = s.pseid
	INNER JOIN
	pdeescola.fonterecurso AS f ON f.forid = d.forid
	WHERE
	s.pdeid = '{$_SESSION['pdeid']}'
	AND
	f.forid = 5";
	//5 é o id o PAF, no qual devem ser analisados os valores.

	$rsValor = $db->carregar( $sqlvalor );

	for( $v = 0; $v < count( $rsValor); $v++)
	{
		$total_custeio += $rsValor[$v]['dpavalorcusteio'];
		$total_capital += $rsValor[$v]['dpavalorcapital'];
	}

	if( ( number_format($percent_capital,2,',','.') != number_format($total_capital,2,',','.')  ) || (  number_format($percent_custeio,2,',','.')  !=   number_format($total_custeio,2,',','.') ) )
	{
		$_SESSION['erros']['custeio'] = "Os valores de capital e custeio informados para o PAF não conferem com os valores disponíveis para a escola";
		$exp = $percent_capital.'_'.$total_capital.'_'.$percent_custeio.'_'.$total_custeio;		

		$_SESSION['erros']['dados']['paf'] = $exp;
		
		$erroAlert = "Os valores de capital e custeio informados para o PAF não conferem com os valores disponíveis para a escola.";
		
		if(( number_format($percent_capital,2,',','.') != number_format($total_capital,2,',','.')  )){
			$erroAlert .= " Valores de capital: ".( number_format($percent_capital,2,',','.')."(FNDE) diferente de ".number_format($total_capital,2,',','.')  )."(MEC)";
		}
		if((  number_format($percent_custeio,2,',','.')  !=   number_format($total_custeio,2,',','.') )){
			$erroAlert .= " Valores de custeio: ".( number_format($percent_custeio,2,',','.')."(FNDE) diferente de ".number_format($total_custeio,2,',','.')  )."(MEC)";
		}
		
		alert($erroAlert);
		return false;
	}

	$sql = "SELECT
	p.pseid,
	d.dpavalorcusteio,
	d.dpavalorcapital
	FROM
	pdeescola.planoacao AS p
	INNER JOIN
	pdeescola.detalheplanoacao as d ON d.plaid = p.plaid
	INNER JOIN
	pdeescola.planosuporteestrategico AS s ON p.pseid = s.pseid
	INNER JOIN
	pdeescola.fonterecurso AS f ON f.forid = d.forid
	INNER JOIN
	pdeescola.tipocategoria AS tp ON tp.tcaid = d.tcaid
	WHERE
	s.pdeid = '{$_SESSION['pdeid']}'
	AND
	f.forid = 5
	AND
	tp.tcaid = 11
					";
	//5 é o id o PAF, no qual devem ser analisados os valores.
	// tipo categoria cujo a regra diz que o valor dos itens não deve ultrapassar 15% do valor do PAF

	$itens = $db->carregar( $sql );

	for( $v = 0; $v < count( $rsValor); $v++)
	{
		$itens_custeio += $itens[$v]['dpavalorcusteio'];
		$itens_capital += $itens[$v]['dpavalorcapital'];
	}
	$total_usado = $itens_custeio + $itens_capital;

	if( $total_usado > $percent_MAX)
	{
		$_SESSION['erros']['itens'] = "A soma total dos ítens de \"contratação de serviços\" não pode ultrapassar 15% do valor de custeio do PAF. \n O valor maximo permitido é: R$ $percent_MAX , e o declarado pela escola foi: R$ $total_usado ";
	}

	$query = "
	SELECT
	s.pseid
	FROM
	pdeescola.planoacao AS p
	INNER JOIN
	pdeescola.detalheplanoacao as d ON d.plaid = p.plaid
	INNER JOIN
	pdeescola.planosuporteestrategico AS s ON p.pseid = s.pseid

	WHERE
	s.pdeid = '{$_SESSION['pdeid']}'
	AND
	d.forid = 5
	AND ( ( d.tprid IS NULL ) OR ( d.tcaid IS NULL ) )
			";
	$errorPlano = $db->pegaUm( $query );

	if( $errorPlano != '')
	{
		$_SESSION['erros']['naodeclarados'] = "Existem ações no seu Plano de Ação com fonte de recurso \"PAF\" que estão sem um tipo de programa e categoria. \nFavor selecione um destes ítens antes de tramitar.";
	}

}

function pre( $var1, $die = '' )
{
	if( $var1 != '' )
	{
		echo("<pre>");
		print_r( $var1 );
		echo("</pre>");
	}
	if( $die == 1 )
	die();
}


function pdeescola_pegaescola( $usucpf ){

	global $db;

	$entid = $db->pegaUm("SELECT
	entid
	FROM
	pdeescola.usuarioresponsabilidade
	WHERE
	usucpf = '{$usucpf}' AND
	rpustatus = 'A'");

	return $entid;

}

/*
 ********************************** FIM FUNÇÕES WORKFLOW *********************************
 */
/*
 * Monta Arvore report Instrumento
 */
function montaTreeReport(){
	global $db;

	$entid = $_SESSION['entid'];
	$pdeid = $_SESSION['pdeid'];
	/*
	 * $tree, recebe script da árvore
	 */
	$tree .= "<div id=\"bloco\" style=\"overflow: hidden;\">
				<p>
					<a href=\"javascript: arvore.openAll();\">Abrir Todos</a>
					&nbsp;|&nbsp;
					<a href=\"javascript: arvore.closeAll();\">Fechar Todos</a>
				</p>
				<div id=\"_arvore\"></div>
			  </div>";

	$tree .= "<script type=\"text/javascript\">
				arvore = new dTree( 'arvore' );
				arvore
				.config.folderLinks = true;
				arvore.config.useIcons = true;
				arvore.config.useCookies = true;
				arvore.config.useLines = false;\n";
	/*
	 * Filtro por estado ou municipio ou entidade
	 */
	/*
	 if ( array_key_exists("estuf", $typeGet) ){
		$where = " ent.tpcid = 1 AND
		est.estuf IN ('".implode( "','", explode(';',$typeGet['estuf']) )."') ";
		$on    = "(ur.estuf = ende.estuf OR
		ur.entid = ent.entid)";
		$tree  .= "arvore.add( 0, -1, 'Diagnóstico Estadual' );\n";
		}elseif ( array_key_exists("muncod", $typeGet) ){
		$where = " ent.tpcid = 3 AND
		mun.muncod IN ('".implode( "','", explode(';',$typeGet['muncod']) )."') ";
		$on    = "(ur.muncod = ende.muncod  OR
		ur.entid = ent.entid)";
		$tree  .= "arvore.add( 0, -1, 'Diagnóstico Municipal' );\n";
		}else*/if ( $entid ) {
	$where = "ent.tpcid IN (1,3) AND
				  ent.entid IN ('".$entid."') ";
	$on    = "(ur.entid  = ent.entid OR
				   ur.muncod = ende.muncod OR
				   ur.estuf  = ende.estuf)";
	$tree  .= "arvore.add( {$entid},-1, 'Diagnóstico Escolar' );\n";
		}else{
			die('<script>
				history.go(-1);
			 </script>');
		}

		/*
		 * Carrega array com perfis do usuário
		 */
		$perfil = arrayPerfil();

		/*
		 * Caso não tenha acesso global
		 * vê somente o que tiver acesso, atravéz do "usuarioresponsabilidade"
		 */
		if ( !in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) && !in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil) ) {
			$from = "INNER JOIN pdeescola.usuarioresponsabilidade ur ON ".$on." AND
																	rpustatus = 'A' AND
																	ur.usucpf = '".$_SESSION['usucpf']."' AND
																	ur.pflcod IN (".implode(',',$perfil).")";
		}

		/*
		 * Requisita entidades
		 */
		$sql = sprintf("SELECT
					 DISTINCT
					-- est.estuf,
					-- est.estdescricao,
					-- mun.muncod,
					-- mun.mundescricao,
					 ent.entid,
					 ent.entnome -- ,
					 -- pde.pdeid
					FROM
					 entidade.entidade ent
					 INNER JOIN entidade.funcaoentidade as fe ON fe.entid = ent.entid
					 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid
					-- INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
					-- INNER JOIN territorios.estado est ON est.estuf = mun.estuf
					 -- LEFT JOIN pdeescola.pdeescola pde ON pde.entid = ent.entid
					 %s
					WHERE
					 fe.funid = 3 and
					 %s
					-- ORDER BY
					-- mun.mundescricao",
		$from,
		$where);

		$dados = (array) $db->carregar($sql);

		if (!$dados[0]):
		die('<script>
				alert(\'Usuário não possui permissão para acessar esta Escola!\');
				history.go(-1);
			 </script>');
		endif;

		$est = array();
		$mun = array();
		$ent = array();

		/*
		 * Carrega perguntas instrumento 2
		 */
		$sql = "SELECT
		DISTINCT
		p2.aceid,
		p2.acecodigo,
		p2.acedescricao,
		p2.aceidpai,
		esc.esadescricao,
		dace.dacevidencia,
		dace.dacid,
		dace.esaid
		FROM
		pdeescola.analisecriterioeficacia p
		INNER JOIN pdeescola.analisecriterioeficacia p2 ON p2.aceidpai = p.aceid OR p2.aceidpai IS NULL
		LEFT JOIN pdeescola.detalheanalisecriterioeficacia dace ON dace.aceid = p2.aceid AND dace.pdeid = {$pdeid}
		LEFT JOIN pdeescola.escalaace esc ON esc.esaid = dace.esaid
		WHERE
		p2.aceano = ".ANO_EXERCICIO_PDE_ESCOLA."
			ORDER BY
			 p2.acecodigo";

		$perg = (array) $db->carregar($sql);


		/*
		 * Carrega Totais 2º nível
		 */
		$sql = "SELECT
			 SUM(esc.esavalor) AS esavalor,
			 p.aceid
			FROM
			 pdeescola.analisecriterioeficacia p
			 INNER JOIN pdeescola.analisecriterioeficacia p2 ON p2.aceidpai = p.aceid
			 INNER JOIN pdeescola.detalheanalisecriterioeficacia dace ON dace.aceid = p2.aceid
			 INNER JOIN pdeescola.escalaace esc ON esc.esaid = dace.esaid
			WHERE
			 p.aceano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
		p.aceidpai IS NOT NULL AND
		p.aceseq IS NULL AND
		dace.pdeid = {$pdeid}
		GROUP BY
		p.aceid";
		$total = (array) $db->carregar($sql);

	 /*
	  * Carrega totais 1º nível
	  */
		$sql = "SELECT
			 SUM(esc.esavalor) AS esavalor,
			 p.aceid
			FROM
			 pdeescola.analisecriterioeficacia p
			 INNER JOIN pdeescola.analisecriterioeficacia p1 ON p1.aceidpai = p.aceid
			 INNER JOIN pdeescola.analisecriterioeficacia p2 ON p2.aceidpai = p1.aceid
			 INNER JOIN pdeescola.detalheanalisecriterioeficacia dace ON dace.aceid = p2.aceid
			 INNER JOIN pdeescola.escalaace esc ON esc.esaid = dace.esaid
			WHERE
			 p.aceano = ".ANO_EXERCICIO_PDE_ESCOLA." AND
		p.aceidpai IS NULL AND
		p.aceseq IS NULL AND
		dace.pdeid = {$pdeid}
		GROUP BY
		p.aceid";
		$total_1 = (array) $db->carregar($sql);
		/*
		 * Adiciona os itens da árvore
		 */
		foreach ($dados as $dado){
			/*
			 // Monta ESTADO
			 if ( !in_array($dado['estuf'], $est) ){
			 $tree .= "arvore.add('{$dado['estuf']}', 0,'<b>".simec_htmlentities($dado['estdescricao'],ENT_QUOTES)."</b>');\n";
			 $est[]  = $dado['estuf'];
			 }

			 // Monta MUNICÍPIO
			 if ( !in_array($dado['muncod'], $mun) ){
			 $tree .= "arvore.add('{$dado['muncod']}','{$dado['estuf']}','".simec_htmlentities($dado['mundescricao'],ENT_QUOTES)."');\n";
			 $mun[]  = $dado['muncod'];
			 }
			 */
			// Monta ENTIDADE "Escola"
			if ( !in_array($dado['entid'], $ent) ){
				//	$tree .= "arvore.add('{$dado['entid']}','{$dado['muncod']}','".simec_htmlentities($dado['entnome'],ENT_QUOTES)."');\n";
				$ent[] = $dado['entid'];

				if ($dado['entid']){
					$tree .= "arvore.add('i_2','{$dado['entid']}','Instrumento 2');\n";

					/*
					 $tree .="arvore.add('1','i_2','1 - Ensino e Aprendizagem','javascript:void(0);');\n";
					 $tree .= "arvore.add('2','1','1.1 - Curr&iacute;culo organizado e articulado','javascript:void(0);')\n";

					 $tree .="arvore.add('13','1','1.2 - Prote&ccedil;&atilde;o do tempo de aprendizagem','javascript:void(0);');\n";

					 $tree .="arvore.add('22','1','1.3 - Pr&aacute;ticas efetivas dentro de sala de aula','javascript:void(0);');\n";

					 $tree .="arvore.add('36','1','1.4 - Estrat&eacute;gias de ensino diferenciadas','javascript:void(0);');\n";

					 $tree .="arvore.add('43','1','1.5 - Deveres de casa freq&uuml;entes e consistentes','javascript:void(0);');\n";

					 $tree .="arvore.add('49','1','1.6 - Disponibilidade e utiliza&ccedil;&atilde;o de recursos did&aacute;ticopedag&oacute;gicos','javascript:void(0);');\n";

					 $tree .="arvore.add('55','1','1.7 - Avalia&ccedil;&atilde;o cont&iacute;nua do rendimento dos alunos','javascript:void(0);');\n";

					 $tree .="arvore.add('66','i_2','2 - Clima Escolar','javascript:void(0);');\n";

					 $tree .="arvore.add('67','66','2.1 - Estabelecimento de altos padr&otilde;es de ensino','javascript:void(0);')\n";

					 $tree .="arvore.add('81','66','2.2 - Altas expectativas em rela&ccedil;&atilde;o &agrave; aprendizagem dos alunos','javascript:void(0);')\n";

					 $tree .="arvore.add('93','66','2.4 - Presen&ccedil;a efetiva do diretor','javascript:void(0);')\n";

					 $tree .="arvore.add('100','66','2.5 - Ambiente escolar bem organizado e agrad&aacute;vel','javascript:void(0);')\n";

					 $tree .="arvore.add('105','66','2.6 - Normas e regulamentos escolares','javascript:void(0);')\n";

					 $tree .="arvore.add('111','66','2.7 - Confian&ccedil;a dos professores no seu trabalho','javascript:void(0);')\n";

					 $tree .="arvore.add('120','66','2.9 - Trabalho em equipe','javascript:void(0);')\n";

					 $tree .="arvore.add('124','i_2','3 - Pais e Comunidade','javascript:void(0);')\n";

					 $tree .="arvore.add('125','124','3.1 - Apoio material da comunidade','javascript:void(0);')\n";

					 $tree .="arvore.add('135','124','3.4 - Envolvimento dos pais na aprendizagem','javascript:void(0);')\n";

					 $tree .="arvore.add('140','i_2','4 - Gest&atilde;o de Pessoas','javascript:void(0);')\n";

					 $tree .="arvore.add('141','140','4.1 - Gest&atilde;o do pessoal docente e n&atilde;o-docente','javascript:void(0);')\n";

					 $tree .="arvore.add('150','140','4.2 - Forma&ccedil;&atilde;o e desenvolvimento','javascript:void(0);')\n";

					 $tree .="arvore.add('159','140','4.3 - Experi&ecirc;ncia apropriada','javascript:void(0);')\n";

					 $tree .="arvore.add('168','i_2','5 - Gest&atilde;o de Processos','javascript:void(0);')\n";

					 $tree .="arvore.add('169','168','5.1 - Conselho/Colegiado Escolar atuante','javascript:void(0);')\n";

					 $tree .="arvore.add('176','168','5.2 - Utiliza&ccedil;&atilde;o e controle dos recursos financeiros','javascript:void(0);')\n";

					 $tree .="arvore.add('181','168','5.3 - Planejamento de a&ccedil;&otilde;es','javascript:void(0);')\n";

					 $tree .="arvore.add('186','168','5.4 - Objetivos claros','javascript:void(0);')\n";

					 $tree .="arvore.add('192','168','5.5 - Rotina Organizada','javascript:void(0);')\n";

					 $tree .="arvore.add('200','i_2','6 - Infra-estrutura','javascript:void(0);')\n";

					 $tree .="arvore.add('201','200','6.1 - Instala&ccedil;&otilde;es adequadas da escola','javascript:void(0);')\n";

					 $tree .="arvore.add('208','i_2','7 - Resultados','javascript:void(0);')\n";

					 $tree .="arvore.add('209','208','7.1 - Desempenho acad&ecirc;mico dos alunos','javascript:void(0);')\n";

					 $tree .="arvore.add('215','208','7.2 - Desempenho geral da escola','javascript:void(0);')\n";

					 $tree .="arvore.add('2f','i_2','Ficha resumo')\n";

					 $tree .="arvore.add('2f.1','2f','<a href=\'?modulo=principal/instrumento2/total_pontos&acao=A\'>Total de pontos</a>')\n";

					 $tree .="arvore.add('tot_1','1','<B>Total:</B> 9','javascript:void(0);')\n";

					 $tree .="arvore.add('tot_2','2','<B>Total:</B> 5','javascript:void(0);')\n";

					 $tree .="arvore.add('tot_13','13','<B>Total:</B> 4','javascript:void(0);')\n";

					 $tree .="arvore.add('i_3','261615','Síntese da auto-Avalição');\n";

					 $tree .="arvore.add('3f.1','i_3','<img src=\"../imagens/atencao.png\" style=\"border:0;\" title=\"Não Respondido\"><a href=\'?modulo=principal/sintese_autoavaliacao/problemas_criterios&acao=A\'>Problemas x Critérios de eficácia escolar</a>');\n";

					 $tree .="arvore.add('3f.2','i_3','<img src=\"../imagens/atencao.png\" style="border:0;\" title=\"Não Respondido\"><a href=\'?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A\'>Problemas x Causas x Ações</a>');\n";

					 $tree .="arvore.add('3f.3','i_3','<a href=\'?modulo=principal/sintese_autoavaliacao/previsao_recursos&acao=A\'>Previsão de Recursos</a>');\n";

					 $tree .="arvore.add('i_4','261615','Plano de Ação');\n";

					 $tree .="arvore.add('3f.4','i_4','<img src=\"../imagens/atencao.png" style="border:0;" title="Não Respondido\"><a href=\'?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A\'>Objetivos Estratégicos, Estratégias e Metas</a>');\n";
					 */


					foreach ($perg as $perg):
					/*
					 if($perg['dacid'] == NULL)
						$img = "<img src=\"../imagens/atencao.png\" style=\"border:0;\" title=\"Não Respondido\">";
						else
						$img = "<img src=\"../imagens/check_p.gif\" style=\"border:0;\" title=\"Respondido\">";
						*/
					$texto = simec_htmlentities(str_replace(array('\n','\r','<br>','</br>',chr(10)),'',trim($perg['acecodigo'])." - ". (strlen($perg['acedescricao']) > 110 ? substr($perg['acedescricao'],0,110).'...' : $perg['acedescricao'] )),ENT_QUOTES);

					$param_img = 'cadastro_instrumento2_'.$perg['aceid'];

					if ( !$perg['aceidpai'] || is_numeric(trim($perg['acecodigo']))){

						$tree .= "arvore.add('{$perg['aceid']}','". ($perg['aceidpai'] ? $perg['aceidpai'] : 'i_2') ."','{$texto}','javascript:void(0);');\n";

					}else{

						if( $perg['esaid'] == '')
						{
							$esaid = '&nbsp;&nbsp;&nbsp;';
						}
						else
						{
							$esaid = $perg['esaid'];
						}
						if( $perg['esadescricao'] == '')
						{
							$esadescricao = 'Não Informada';
						}
						else
						{
							$esadescricao = $perg['esadescricao'];
						}
						if( $perg['dacevidencia'] == '')
						{
							$dacevidencia = 'Não Informada';
						}
						else
						{
							$dacevidencia = str_replace(chr(10),"",str_replace(chr(13)," ",addslashes($perg['dacevidencia'])));
						}


						//$tipo = carregaPreenchimento(trim($param_img));
						//						echo '<pre>';
						//						echo 'Parâmetro:'.$param_img;
						//						echo '<br>Tipo:'.$tipo;
						//						echo' <br><br></pre>';
						//if( ($tipo == 's') || ($perg['aceidpai'] == 1) && ($perg['esadescricao'] != ''))
						//{
						//if($perg['esaid'] != '')
						//{

						$tree .= "arvore.add('{$perg['aceid']}','{$perg['aceidpai']}','".carregaImagem(trim($param_img))." <a href=\'?modulo=principal/instrumento2/cadastro_instrumento2&acao=A&aceid={$perg['aceid']}\'></a>".$texto."?<div id=\"alinha\" style=\"position: relative; left: 95px; width: 500px;border-color: #cccccc; border: 5px;\"><p style=\"border-width: 4px; border-style: none;\"><b>Escala:&nbsp;".$esaid."&nbsp;-&nbsp;</b>".$esadescricao."<br><b>Evidência:&nbsp;&nbsp;</b>".$dacevidencia."</p></div>','javascript:void(0)');\n";

						//}
						//}


					}
					endforeach;
				}

				/*
				 * Itens staticos, instrumento 2
				 */


				$tree .= "arvore.add('2f','i_2','Ficha resumo');\n";
				$tree .= "arvore.add('2f.2','2f','".carregaImagem(trim('cadastro_criticidade'))."<a href=\'?modulo=principal/instrumento2/cadastro_criticidade&acao=A\'>Criticidade</a>');\n";
				$tree .= "arvore.add('2f.3','2f','".carregaImagem(trim('cadastro_prioridade'))."<a href=\'?modulo=principal/instrumento2/cadastro_prioridade&acao=A\'>Prioridade</a>');\n";
				$tree .= "arvore.add('2f.1','2f','<a href=\'?modulo=principal/instrumento2/total_pontos&acao=A\'>Total de pontos</a>');\n";

				if ($total_1[0]){
					foreach ($total_1 as $tot_1):
					$tree .= "arvore.add('tot_{$tot_1['aceid']}','{$tot_1['aceid']}','<B>Total:</B> {$tot_1['esavalor']}','javascript:void(0);');\n";
					endforeach;
				}


				if ($total[0]){
					foreach ($total as $tot):
					$tree .= "arvore.add('tot_{$tot['aceid']}','{$tot['aceid']}','<B>Total:</B> {$tot['esavalor']}','javascript:void(0);');\n";
					endforeach;
				}

				//
				//			$tree .= "arvore.add('i_3','{$dado['entid']}','Síntese da autoavaliação');\n";
				//
				//			$tree .= "arvore.add('3f.1','i_3','".carregaImagem(trim('popupProblemaCriterio'))."<a href=\'?modulo=principal/sintese_autoavaliacao/problemas_criterios&acao=A\'>Problemas x Critérios de eficácia escolar</a>');\n";
				//			$tree .= "arvore.add('3f.2','i_3','".carregaImagem(trim('problemas_causas_acoes'))."<a href=\'?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A\'>Problemas x Causas x Ações</a>');\n";
				//			$tree .= "arvore.add('3f.3','i_3','<a href=\'?modulo=principal/sintese_autoavaliacao/previsao_recursos&acao=A\'>Previsão de Recursos</a>');\n";
				//
				//		$tree .= "arvore.add('i_4','{$dado['entid']}','Plano de Ação');\n";
				//	$tree .= "arvore.add('3f.4','i_4','".carregaImagem(trim('popupObjetivosMetas'))."<a href=\'?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A\'>Objetivos Estratégicos, Estratégias e Metas</a>');\n";
				//
				//

			}

		}


		$tree .= "  elemento = document.getElementById( '_arvore' );
			    elemento.innerHTML = arvore;
			  </script>";


		return $tree;
}

function pde_verificaSessao(){
	if ( !$_SESSION["pdeid"] || !$_SESSION["entid"]){
		echo "<script> window.location = '../logout.php';</script>";
		exit();
	}
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
function verificaProjetos($pdeid , &$db=null){

	if( !$db ){
		global $db;
	}
	$sql = "SELECT
	*
	FROM
	pdeescola.medidaprojetoatual
	WHERE
	pdeid = $pdeid
			";
	$dados = $db->carregar( $sql );

	if( $dados[0]['mpaid'] != ''){
		return true;
	}

	return false;
}

function valorUtilizadoEscolaPAF(){
	global $db;

	$sqlvalor = "SELECT
	p.pseid,
	d.dpavalorcusteio,
	d.dpavalorcapital
	FROM
	pdeescola.planoacao AS p
	INNER JOIN
	pdeescola.detalheplanoacao as d ON d.plaid = p.plaid
	INNER JOIN
	pdeescola.planosuporteestrategico AS s ON p.pseid = s.pseid
	INNER JOIN
	pdeescola.fonterecurso AS f ON f.forid = d.forid
	WHERE
	s.pdeid = '{$_SESSION['pdeid']}'
	AND
	f.forid = 5";
	//5 é o id o PAF, no qual devem ser analisados os valores.

	$rsValor = $db->carregar( $sqlvalor );

	for( $v = 0; $v < count( $rsValor); $v++)
	{
		$total_custeio += $rsValor[$v]['dpavalorcusteio'];
		$total_capital += $rsValor[$v]['dpavalorcapital'];
	}
	$valor = ($total_custeio + $total_capital);

	return $valor;
}

function condicaoFinalizarPde()
{
	global $db;
    
	$sql = "SELECT pdeid FROM pdeescola.pdeescola WHERE pdeid = '{$_SESSION['pdeid']}' AND pdepafretorno IS NOT NULL";
	$pdeidPago = $db->pegaUm( $sql );
	
	if( $pdeidPago )
	{
		alert('A escola já foi paga!');
		return false;
	}
	
	
    $valorPAF                  = round( pegaSomaValoresPlanos(), 2);
   	$valorUtilizadoEscolaPAF   = round( valorUtilizadoEscolaPAF(), 2);
    
	if($valorPAF == $valorUtilizadoEscolaPAF)
	{
		return true;
	}
	else
	{
		alert("Os valores de capital e custeio informados para o PAF não conferem com os valores disponíveis para a escola");
		return false;
	}
}

/*
 * @metodo: finalizarPde - para instanciar e chamar o metodo "magico" da classe pdeWs, que via parametros chama ele mesmo novamente
 * @autor: Pedro Dantas
 *
 */
function finalizarPde()
{
	global $db;
	
	include_once("pdeWs.php");
	$ws = new pdeWs();
	
	$entcodent = pegarEntCodEnt( $_SESSION['entid'] );
	$coProgramaFNDE = 96;
	$anoAtual = date('Y');

	$teste = $ws->pdeEscolaWs('atualizaAnaliseEscola', $anoAtual, $entcodent, $coProgramaFNDE);

	if( $teste && $teste != "errowebservice" )
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

function condicaoEnviarParaPagamento()
{
	global $db;
	
	//carregando valores declarados Parcela Complementar.
	$dadosValorParcela = $db->pegaUm("SELECT 
											sum(pasp.pasquantidade * pasp.pasvalorunitario) as valor
						  			  FROM
											pdeescola.planoacaosegundaparcela pasp 
						  			  WHERE
											pdeid = ".$_SESSION['pdeid']);
	
	$valorComplementar 	= pegaSomaValoresComplementares();

	if($valorComplementar == $dadosValorParcela)
	{
		return true;
	}
	else
	{	
		alert("O valor da Parcela Complementar não está de acordo com os valores declarados");
		return false;
	}

}
/*
 * Condição para as ação do workflow: 'Avaliação MEC Parcela Complementar -> Enviado para pagamento Parcela Complementar'.
 */
function enviarParaPagamento()
{
	global $db;
	
	include_once("pdeWs.php");
	$ws = new pdeWs();

	$entcodent = pegarEntCodEnt( $_SESSION['entid'] );
	$coProgramaFNDE = 96;
	$anoAtual = date('Y');

	$teste = $ws->pdeEscolaWs('atualizaAnaliseEscola', $anoAtual, $entcodent, $coProgramaFNDE);
	
	if( $teste && $teste != "errowebservice" )
	{
		return true;
	}
	else
	{
		return false;
	}
}

function pegaMunicipioAssociado($perfil){
	global $db;

	$sql = "SELECT muncod FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = '{$perfil}' ";
	$municipio = $db->pegaUm($sql);

	if( $perfil == PDEESC_PERFIL_SUPER_USUARIO || $perfil == PDEESC_PERFIL_ADM_QUEST_SEESP ){
		return $_REQUEST['muncod'];
	} else {
		if($municipio[0]){
			return $municipio;
		}
	}

	return false;
}

function pegaEstadoAssociado($perfil){
	global $db;
	
	$sql = "SELECT estuf FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = '{$perfil}' ";
	$estado = $db->pegaUm($sql);

	if( $perfil == PDEESC_PERFIL_SUPER_USUARIO || $perfil == PDEESC_PERFIL_ADM_QUEST_SEESP ){
		return $_REQUEST['estuf'];
	} else {
		if($estado[0]){
			return $estado;
		}
	}

	return false;
}

function pegaEscolaAssociada($perfil){
	global $db;
	
	$sql = "SELECT entid FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = '{$perfil}' ";
	$escola = $db->pegaUm($sql);

	
	if( $perfil == PDEESC_PERFIL_SUPER_USUARIO || $perfil == PDEESC_PERFIL_ADM_QUEST_SEESP ){
		return $_REQUEST['entid'];
	} else {
		if( !$escola ){
			print "<script>"
				. "    alert('Você não tem acesso a esta tela!');"
				. "    history.back(-1);"
				. "</script>";
			
			die;
			return false;
		} elseif($escola[0]){
			return $escola;
		}
	}

	return false;
}

function cabecalhoQuestionario( $acao, $perfil ){
	global $db;
	
	if( $_GET['acao'] == 'C' ){
		if( $perfil == PDEESC_PERFIL_SUPER_USUARIO || $perfil == PDEESC_PERFIL_ADM_QUEST_SEESP ){
			$uf = $_REQUEST['estuf'];
		} else {
			$uf = pegaEstadoAssociado( $perfil );
		}
		$sql = "SELECT
                    estdescricao as descricao
                FROM
                    territorios.estado
                WHERE
                    estuf = '".$uf."'";
		$descricao = $db->pegaUm( $sql );
		$desc = "<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Descrição:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$descricao."</td>
				</tr>";
	} elseif( $_GET['acao'] == 'B' ){
		if( $perfil == PDEESC_PERFIL_SUPER_USUARIO || $perfil == PDEESC_PERFIL_ADM_QUEST_SEESP ){
			$muncod = $_REQUEST['muncod'];
		} else {
			$muncod = pegaMunicipioAssociado( $perfil );
		}
		$sql = "SELECT
					estuf as estado,
                    mundescricao as descricao
                FROM
                    territorios.municipio
                WHERE
                    muncod = '".$muncod."'";

		$descricao = $db->pegaLinha( $sql );
		$uf = $descricao['estado'];
		$desc = "<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Município:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$descricao['descricao']."</td>
				</tr>";
	} elseif( $_GET['acao'] == 'A' ){
		if( $perfil == PDEESC_PERFIL_SUPER_USUARIO || $perfil == PDEESC_PERFIL_ADM_QUEST_SEESP ){
			$entid = $_REQUEST['entid'];
		} else {
			$entid = pegaEscolaAssociada( $perfil );
		}
		$sql = "SELECT 
					ent.entnome as escola,
					m.estuf as estado,
					m.mundescricao as municipio  
				FROM
					entidade.entidade ent				
				INNER JOIN
					entidade.endereco ende ON ende.entid = ent.entid
				INNER JOIN
					entidade.funcaoentidade fe ON fe.entid = ent.entid
				INNER JOIN
					territorios.municipio m ON m.muncod = ende.muncod
				WHERE
					ent.entid = ".$entid;
		$dados = $db->pegaLinha( $sql );
		$uf = $dados['estado'];
		$mun = $dados['municipio'];
		$esc = $dados['escola'];
		$desc = "<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Município:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$mun."</td>
				</tr>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Escola:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$esc."</td>
				</tr>";
	}
	
	echo "		
		<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
			<tbody>
				<tr>
					<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">UF:</td>
					<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$uf."</td>
				</tr>
				{$desc}
			</tbody>
		</table><br />";
}

function pegaQrpid( $acao, $aba, $perfil, $ano ){
	global $db;

	//ver(1,1,1,1,1,1,$acao, $aba, $perfil, $ano, $_REQUEST['estuf'], d);
	
	if( $acao == 'A' ){
		if( $aba == 1 ){
			$queid = QUESTIONARIO_VI;
		}elseif( $aba == 2 ){
			$queid = QUESTIONARIO_VII;
		}else{
			$queid = QUESTIONARIO_VI;
		}
		$valor = pegaEscolaAssociada( $perfil );
		if( $valor ){
			$where = " p.entid = '".$valor."'";
			$tipo = 'entid';
		}else{
			echo '<script>
				 alert("Você não possui permissão para visualizar esse questionário!");
				 history.back(-1);
			  </script>';
			die;
		}
	} elseif( $acao == 'B' ) {
		if( $aba == 1 ){
			$queid = QUESTIONARIO_III;
		} elseif( $aba == 2 ) {
			$queid = QUESTIONARIO_V;
		}elseif( $aba == 3 ){
			$queid = QUESTIONARIO_IV;
		}
		$valor = pegaMunicipioAssociado( $perfil );
		$where = " p.muncod = '".$valor."'";
		$tipo = 'muncod';
	} else {
		if( $aba == 1 ){
			$queid = QUESTIONARIO_I;
		}elseif( $aba == 2 ){
			$queid = QUESTIONARIO_II;
		}elseif( $aba == 3 ){
			$queid = QUESTIONARIO_IV;
		}elseif( $aba == 4 ){
			$queid = QUESTIONARIO_V;
		}else{
			$queid = QUESTIONARIO_I;
		}
		$valor = pegaEstadoAssociado( $perfil );
		$where = " p.estuf = '".$valor."'";
		$tipo = 'estuf';
	}

	$sql = "SELECT
					p.qrpid
			FROM
					pdeescola.pdequestionario p
			INNER JOIN
					questionario.questionarioresposta q ON q.qrpid = p.qrpid
			WHERE
					{$where} AND
					q.queid = {$queid} AND
					p.prsano = '{$ano}'";
	$qrpid = $db->pegaUm( $sql );

	if(!$qrpid){
		if( $tipo == 'muncod' ){
			$sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$valor}'";
		} elseif( $tipo == 'estuf' ){
			$sql = "SELECT estdescricao FROM territorios.estado WHERE estuf = '{$valor}'";
		} elseif( $tipo == 'entid' ){
			$sql = "SELECT entnome FROM entidade.entidade WHERE entid = '{$valor}'";
		}
		$titulo = $db->pegaUm( $sql );
		$arParam = array ( 'queid' => $queid, 'titulo' => 'PDE-Escola ('.$titulo.' - '.$ano.')' );
		$qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
		$sql = "INSERT INTO pdeescola.pdequestionario (qrpid, {$tipo}, prsano) VALUES ({$qrpid}, '{$valor}', '{$ano}')";
		$db->executar( $sql );
		$db->commit();
	}
	return $qrpid;
}

function verificaEscolaPagaWs(){
	global $db;
	include_once("pdeWs.php");

	$sql = "SELECT pdeid FROM pdeescola.pdeescola WHERE pdeid = '{$_SESSION['pdeid']}' AND pdepafretorno IS NOT NULL";
	$pdeidPago = $db->pegaUm( $sql );

	if( !$pdeidPago ){
		return false;
	}else{

		$sql = "SELECT pdepafretorno FROM pdeescola.pdeescola WHERE pdeid = '{$_SESSION['pdeid']}'";
		$pdeidPafDataRetorno = $db->pegaUm( $sql );

		if( $pdeidPafDataRetorno == date("Y-m-d") ){
			$pago  = "t";

			return $pago;
		}else{

			$entcodent 		= pegarEntCodEnt( $_SESSION['entid'] );
			$coProgramaFNDE = 96;

			$ws = new pdeWs();
			$pdeidPago = $ws->pdeEscolaWs('IsEscolaPaga', '2009', $entcodent, $coProgramaFNDE);

			if( $pdeidPago == "errowebservice"){

				return $pdeidPago;

			} else {

				$sql = "UPDATE pdeescola.pdeescola SET pdepafretorno = '".date("Y-m-d")."' WHERE pdeid = {$_SESSION['pdeid']}";
				$db->executar( $sql );
				$db->commit();

				if($pdeidPago == '1'){
					$pago  = "t";
					return $pago;
				} else {
					$pago  = "f";
					return $pago;
				}
			}
		}
	}
}

function verificaEscEnviadaPgto() {
	global $db;

	$entid = $_SESSION['entid'];

	$sql = "SELECT
				ppr.entid
			FROM
				pdeescola.preenchimento as ppr
			INNER JOIN
				pdeescola.pdeescola as ppd ON ppr.entid = ppd.entid AND ppd.pdepafretorno IS NOT NULL
			WHERE
				entcodent is not null
			AND
				ppr.esdid in (61,90)
			AND
				ppr.entid = ".$entid;

	$envPgto = $db->pegaUm( $sql );

	if( $envPgto){
		return true;
	}
	else {
		return false;
	}
}

function carregaAbas() {

	global $db, $docid;

	if( $docid ){
		$sql = ("select count(*)
				from
					workflow.historicodocumento
				where
					docid ={$docid} and aedid = 141");

		$busca = $db->carregar($sql);
		$retorno = $busca[0]["count"];
	}
	
	if($_SESSION['pdeid']){
		$esdid 					= pegarEstadoAtual($docid);	
		$valorPC 				= pegaSomaValoresComplementares();
		$valorPAF 				= pegaSomaValoresPlanos();
	}
	
	if($_SESSION['entid']){
		$rsAutoavaliacao 		= verificaResultadoAltuavaliacao();
	}
	
	$arEstadosWorkflow 		    = array(VALIDACAO_PELO_MEC_WF, AVALIACAO_COMITE_ME_WF, DEVOLVIDO_PARA_ESCOLA_PC_WF, EM_ELABORACAO_PC_WF, DEVOLVIDO_PARA_ESCOLA_PC_WF, DEVOLVIDO_PARA_COMITE_WF, AVALIACAO_MEC_PARCERIA_COMPLEMENTAR_WF, ENVIADO_PARA_PAGAMENTO_WF);
	$verificaPaginaLista 	    = (strpos($_SERVER['REQUEST_URI'], '?modulo=lista&acao=A') > 0) ? false : true;
	$arEstadosValidadoMEC 		= array(VALIDACAO_PELO_MEC_WF, EM_ELABORACAO_PC_WF, DEVOLVIDO_PARA_ESCOLA_PC_WF, AVALIACAO_COMITE_ME_WF,DEVOLVIDO_PARA_COMITE_WF,AVALIACAO_MEC_PARCERIA_COMPLEMENTAR_WF,ENVIADO_PARA_PAGAMENTO_WF);

	$perfil = arrayPerfil();
	if( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) || in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil) || in_array(PDEESC_PERFIL_CONSULTA, $perfil) || 
		in_array( PDEESC_PERFIL_COMITE_MUNICIPAL, $perfil) || in_array(PDEESC_PERFIL_COMITE_ESTADUAL, $perfil)) {

		// Verifica se não existe entid, se existir vai pro else
		if(!$_SESSION['entid']){
			$menu = array(
						0 => array("id" => 0, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=lista&acao=E"),
						);
		}else{
			if(in_array(PDEESC_PERFIL_VISITA_AMOSTRAL, $perfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil)){
				
				$menu = array(
							0 => array("id" => 0, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=lista&acao=E"),
							1 => array("id" => 1, "descricao" => "Plano Atual", "link" => "/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A"),
							//2 => array("id" => 2, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A"),
							//4 => array("id" => 4, "descricao" => "Questionário", "link" => "/pdeescola/pdeescola.php?modulo=principal/questionario1&acao=A"),
							);
							
				if($retorno != 0){
					array_push( $menu,  array("id" => 3, "descricao" => "Avaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A") );
					
					if($verificaPaginaLista){
						array_push( $menu,  array("id" => 4, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );
					}
					if($arEstadosValidadoMEC){
						array_push( $menu,  array("id" => 5, "descricao" => "Execução/Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A") );
					}
				}
							
			}else{
				$menu = array(
							0 => array("id" => 0, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=lista&acao=E"),
							1 => array("id" => 1, "descricao" => "Plano Atual", "link" => "/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A"),
							//2 => array("id" => 2, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A"),
							);
				// Verifica se já foi enviado para pagamento alguma vez, independente do estado do workflow
				if($retorno != 0){
					array_push( $menu,  array("id" => 3, "descricao" => "Avaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A") );
										
					if($verificaPaginaLista){
						array_push( $menu,  array("id" => 4, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );
					}
					if($arEstadosValidadoMEC){
						array_push( $menu,  array("id" => 5, "descricao" => "Execução/Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A") );
					}
				}
			} // Fim else para demais perfis	
					
		} // Fim do if que verifica se existe entid
		
	} elseif( in_array( PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfil ) || in_array( PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL,  $perfil ) ) {
		
		// Se o estado do workflow for diferente de Validado pelo Mec não mostra abas
		if(!in_array($esdid, $arEstadosWorkflow)){		
//			$menu = array(
//						0 => array("id" => 0, "descricao" => "", "link" => "javascript:void(0)"),
//						);

			$menu = array(
						0 => array("id" => 0, "descricao" => "Plano Atual", "link" => "/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A"),
						//1 => array("id" => 1, "descricao" => "Autoavaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A"),				
						);
						
			if($retorno > 0){
				array_push( $menu,  array("id" => 1, "descricao" => "Avaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A") );
				if($verificaPaginaLista){
					array_push( $menu,  array("id" => 4, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );
				}							
			}
			
		}else{
			
			if($_SESSION['pdeid']){
									
				if($valorPC == 0){ 
//					$menu = array(
//								0 => array("id" => 0, "descricao" => "", "link" => "javascript:void(0)"),
//								);

					$menu = array(
						0 => array("id" => 0, "descricao" => "Plano Atual", "link" => "/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A"),
						1 => array("id" => 1, "descricao" => "Avaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A"),
						);
						
					if($arEstadosValidadoMEC){
						array_push( $menu,  array("id" => 5, "descricao" => "Execução/Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A") );
					} 
				}else{ 
					$menu = array(
								 0 => array("id" => 0, "descricao" => "Plano Atual", "link" => "/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A"),
								 );
								
					if($valorPAF == 0 && $valorPC > 0 && in_array($esdid, $arEstadosWorkflow) && $retorno > 0){
						array_push( $menu,  array("id" => 1, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );
						array_push( $menu,  array("id" => 3, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A") );
					} elseif($valorPAF > 0 && $valorPC > 0 && in_array($esdid, $arEstadosWorkflow) && $retorno > 0){
						array_push( $menu,  array("id" => 1, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );						
						array_push( $menu,  array("id" => 3, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A") );
					} elseif($valorPAF > 0 && $valorPC > 0 && in_array($esdid, $arEstadosWorkflow) && $retorno == 0){
						array_push( $menu,  array("id" => 1, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );
						array_push( $menu,  array("id" => 3, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A") );						
					}
					array_push( $menu,  array("id" => 2, "descricao" => "Avaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A") );
					if($arEstadosValidadoMEC){
						array_push( $menu,  array("id" => 5, "descricao" => "Execução/Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A") );
					} 
				}
			}
		}
	}else{
		$menu = array(
					0 => array("id" => 0, "descricao" => "Lista", "link" => "/pdeescola/pdeescola.php?modulo=lista&acao=E"),
					);
					
		array_push( $menu,  array("id" => 1, "descricao" => "Plano Atual", "link" => "/pdeescola/pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A") );
		
		if($valorPAF == 0 && $valorPC > 0 && in_array($esdid, $arEstadosWorkflow) && $retorno > 0){
			array_push( $menu,  array("id" => 2, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );
			array_push( $menu,  array("id" => 4, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A") );
		} elseif($valorPAF > 0 && $valorPC > 0 && in_array($esdid, $arEstadosWorkflow) && $retorno > 0){
			array_push( $menu,  array("id" => 2, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );						
			array_push( $menu,  array("id" => 4, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A") );
		} elseif($valorPAF > 0 && $valorPC > 0 && in_array($esdid, $arEstadosWorkflow) && $retorno == 0){
			array_push( $menu,  array("id" => 2, "descricao" => "Parcela Complementar", "link" => "/pdeescola/pdeescola.php?modulo=principal/segundaParcela/segundaParcela&acao=A") );
			array_push( $menu,  array("id" => 4, "descricao" => "Documentos Anexos", "link" => "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A") );						
		} 
		
		array_push( $menu,  array("id" => 3, "descricao" => "Avaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A") );
		if($arEstadosValidadoMEC){
			array_push( $menu,  array("id" => 5, "descricao" => "Execução/Monitoramento", "link" => "/pdeescola/pdeescola.php?modulo=principal/monitoramento/monitoramentoPlanoAcao&acao=A") );
		}
//		if($retorno > 0){
//			array_push( $menu,  array("id" => 3, "descricao" => "Autoavaliação", "link" => "/pdeescola/pdeescola.php?modulo=principal/propostaMonitoramento&acao=A") );							
//		}
		
	}

	return $menu;
}

//function retornaAvaliacaoMEC(){
//	global $db;
//
//	$sql = "UPDATE pdeescola.pdeescola SET pdepafpago = 'f' WHERE pdeid = {$_SESSION['pdeid']}";
//	$up = $db->executar( $sql );
//	$db->commit();
//	return true;
//}


function tabEscola($entid=null){
	global $db;

	if (!$entid){
		return false;
	}

	$css = '<style type="text/css">
				@charset "iso-8859-1";
				/* CSS Document */
				body{
				margin:0px;
				padding:0px;
				margin-left:10px;
				width:740px;
				}
				th, td{
				border: 1px solid #fff;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				}
				h1{
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:14px;
				color:#6E8D62;
				text-transform: uppercase;
				}
				img{
				border:none;
				}
				caption {
				display:none;
				}
				.upper {
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:8px;
				color:#000000;
				text-transform: uppercase;
				}
				#indicadores_titulo{
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:10px;
				width:721px;
				font-weight:bold;
				margin-top:5px;
				text-align:left;
				}
				#indicadores_titulo img{
				margin-right:0px;
				}
				#texto_indicadores{
				margin-top:10px;
				width:745px;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:11px;
				text-align:justify;
				}
				.back_button a{
				font-size: 12px;
				font-weight: normal;
				color: #2B729D;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				text-decoration:none;
				}
				.back_button avisited{
				font-size: 12px;
				font-weight: normal;
				color: #2B729D;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				text-decoration:none;
				}
				.back_button a:hover{
				font-size: 12px;
				font-weight: normal;
				color: #2B729D;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				text-decoration:underline;
				}
				#print{
				width:80px;
				text-align:right;
				height:20px;
				margin-top:5px;
				margin-bottom:5px;
				float:right;
				}
				#voltar{
					width:200px;
					text-align:right;
					height:20px;
					margin-right:20px;
					margin-top:50px;
					margin-bottom:5px;
					float:right;
					text-decoration:none;
				}

				#voltar a{
				text-decoration:none;
				}

				#voltar a:link{
				text-decoration:none;
				}

				#voltar a:hover{
				text-decoration:none;
				}


				table{
					width:721px;
					font-size:10px;
					float:left;
				}
				.th1{
				font-size:14px;
				color:#FFFFFF;
				text-align: left;
				height:30px;
				padding-left:10px;
				font-weight:bold;
				}

				/* Tabela 1*/
				#tb1{
				border: 3px double #aba476;
				background-color: #ecebd5;
				margin-bottom:14px;
				}
				#tb1 .th1{
				background-color:#aba476;
				}
				#tb1 .th2{
				height:30px;
				font-size:11px;
				text-align: center;
				background-color:#d4ce6e;
				}
				#tb1 .th3{
				font-weight:bold;
				text-align:center;
				height:30px;
				background-color:#DFDCB5;
				font-size:11px;
				}
				#tb1 .th4{
				font-weight:bold;
				text-align:center;
				background-color:#EFEAA4;
				font-size:11px;
				}
				#tb1 .th5{
				text-align:center;
				height:30px;
				}
				#tb1 .th5_blue{
				text-align:center;
				height:30px;
				color:#000099;
				font-weight: bold;
				}
				#tb1 .th5_red{
				text-align:center;
				height:30px;
				color:#CC3300;
				font-weight: bold;
				}
				#tb1 .th5_green{
				text-align:center;
				height:30px;
				color:#003300;
				font-weight: bold;
				}
				.th6{
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:9px;
				color:#000000;
				text-transform: uppercase;
				}
				#tb1 .th6{
				text-align:left;
				height:30px;
				font-weight:bold;
				padding-left:10px;
				background-color:#aba476;
				color:#FFFFFF;
				}
				#tb1 .th7{
				text-align:left;
				height:30px;
				font-weight:bold;
				background-color:#aba476;
				color:#FFFFFF;
				}
				#tb1 .th3_red{
				font-weight:bold;
				text-align:center;
				background-color:#edebbd;
				height:30px;
				font-size:10px;
				color: #CC3300;
				}
				#tb1 .th3_green{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:10px;
				color: #003300;
				}
				/* Fim da Tabela 1*/

				/* Tabela 2*/
				#tb2{
				border: 3px double #6E8D62;
				background-color: #e9f0e3;
				margin-bottom:14px;
				}
				#tb2 .th1{
				background-color:#6E8D62;
				}
				#tb2 .th2{
				height:30px;
				font-size:11px;
				text-align: center;
				background-color:#B3BFA7;
				}
				#tb2 .th3{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:11px;
				}
				#tb2 .th3_red{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:10px;
				color: #CC3300;
				}
				#tb2 .th3_green{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:10px;
				color: #003300;
				}
				#tb2 .th4{
				font-weight:bold;
				text-align:center;
				background-color:#B3CC99;
				height:30px;
				font-size:11px;
				}
				#tb2 .th5{
				text-align:center;
				height:30px;
				}
				#tb2 .th52{
				height:30px;
				}
				#tb2 .th5_blue{
				text-align:center;
				height:30px;
				color:#000099;
				font-weight: bold;
				}
				#tb2 .th5_red{
				text-align:center;
				height:30px;
				color:#CC3300;
				font-weight: bold;
				}
				#tb2 .th5_green{
				text-align:center;
				height:30px;
				color:#003300;
				font-weight: bold;
				}
				#tb2 .th6{
				text-align:left;
				height:30px;
				font-weight:bold;
				padding-left:10px;
				background-color:#6E8D62;
				color:#FFFFFF;
				}
				/* Fim da Tabela 2*/

			</style>';

	$sql = "SELECT
	--edt.*,
	--e.entid,
	edt.entcodent,
	entnome,
	entemail,
	-- entnumdddcelular,
	-- entnumcelular,
	entnumfax,
	--	entnumramalfax,
	entnumdddcomercial,
	entnumcomercial as telefone,
	--	entnumramalcomercial,
	--	entnumdddresidencial,
	--	entnumresidencial,
	endlog,
	endcom,
	endbai,
	endnum,
	to_char(endcep::float,'99999-999') AS endcep,
	mundescricao,
	CASE
	WHEN e.tpcid = 1 THEN 'Estadual'
	ELSE 'Municipal'
	END AS rede,
	tl.tpldesc AS zona
	FROM
	entidade.entidade e
	INNER JOIN entidade.endereco ed ON ed.entid = e.entid
	INNER JOIN territorios.municipio m ON m.muncod = ed.muncod
	INNER JOIN entidade.entidadedetalhe edt ON edt.entid = e.entid
	INNER JOIN entidade.tipolocalizacao tl ON tl.tplid = e.tplid
	WHERE
	e.entid = {$entid}
				".($_REQUEST['entcodent'] ? " AND e.entcodent = '".$_REQUEST['entcodent']."'" : '');

	$col = $db->pegaLinha($sql);

	if (!$col){
		die("<script>
				alert('Não há registro!');
				window.close();
			 </script>");
	}

	$html = '<table cellspacing="0" cellpadding="1" border="1" summary="Número de escolas em áreas específicas - Rede Estadual 2007 - Zona Rural" id="tb2">
				<caption>
				Escolas - Zona Rural - Rede Municipal - 2007</caption>
				<thead>
					<tr>
				    <th id="h1" class="th1" colspan="22">'.$col['entnome'].'</th>
				  </tr>
				  </thead>
				   <tbody>
				   <tr>
				      <th id="h3" class="th2" colspan="2">Código da Escola
				      </th><td headers="h1 h3" class="th7" colspan="8">'.$col['entcodent'].'</td>
				      <th id="h3" class="th2" colspan="2">Município
				      </th><td headers="h1 h3" class="th7" colspan="10">'.strtoupper($col['mundescricao']).'</td>
				    </tr>
				    <tr>
				      <th id="h3" class="th2">Endereço
				      </th><td headers="h1 h3" class="th7" colspan="5">'.strtoupper($col['endlog']).'</td>
				      <th id="h3" class="th2" colspan="1">Complem.
				      </th><td headers="h1 h3" class="th7" colspan="2">'.($col['endcom'] ? strtoupper($col['endcom']) : '&nbsp;').'</td>
				      <th id="h3" class="th2" colspan="1">N°</th><td headers="h1 h3" class="th7">'.($col['endnum'] ? $col['endnum'] : '&nbsp;').'</td>

				      <th headers="h1 h3" id="h32" class="th2">Bairro</th>
				      <td colspan="10" headers="h1 h3 h32" class="th7">'.strtoupper($col['endbai']).'</td>
				    </tr>
				    <tr>
				      <th id="h4" class="th2">Rede
				      </th><td colspan="5" headers="h1 h4" class="th7">'.strtoupper($col['rede']).'</td>
				      <th colspan="1" id="h41" class="th2">Zona</th>
				      <td headers="h1 h4 h41" class="th7" colspan="3">'.strtoupper($col['zona']).'</td>
				      <th id="h42" class="th2">CEP</th>
				      <td colspan="11" headers="h1 h4 h42" class="th7">'.$col['endcep'].'</td>
				    </tr>
				    <tr>
				      <th id="h6" class="th2" rowspan="2">Contato
				      </th><th id="h61" class="th3">DDD</th>
				      <th colspan="2" id="h62" class="th3">Telefone</th>
				      <th colspan="3" id="h63" class="th3"><p>Telefone<br/>
				      Público <br/>
				      1</p></th>
				      <th colspan="3" id="h64" class="th3"><p>Telefone<br/>Público 2</p></th>
				      <th id="h65" class="th3">Fax</th>
				      <th colspan="11" id="h66" class="th3">E-mail</th>
				    </tr>
				    <tr>
				      <td headers="h6 h61" class="th5">'.$col['entnumdddcomercial'].'</td>
				      <td colspan="2" headers="h6 h62" class="th5">'.($col['telefone'] ? $col['telefone'] : '-').'</td>
				      <td colspan="3" headers="h6 h63" class="th5">-</td>
				      <td colspan="3" headers="h6 h64" class="th5">-</td>
				      <td headers="h6 h65" class="th5">'.($col['entnumfax'] ? $col['entnumfax'] : '-').'</td>
				      <td colspan="11" headers="h6 h66" class="th5"> <a href="mailto:'.$col['entemail'].'">'.$col['entemail'].'</a></td>
				    </tr>
				<!--
				    <tr>
				    <th id="h1" class="th4" colspan="22">Matrículas por Nível</th>
				  	</tr>
				    <tr>
				      <th id="h7" class="th3" colspan="3">Creche</th>
				      <th id="h8" class="th3" colspan="3"> Pré-escola </th>
				      <th colspan="3" id="h9" class="th3">Anos Iniciais <br/>
				      Ensino Fundamental</th>
				      <th colspan="2" id="h10" class="th3">Anos Finais<br/>
				      Ensino Fundamental</th>
				      <th id="h11" class="th3" colspan="3">Ensino Médio</th>
				      <th id="h12" class="th3" colspan="3">EJA Ensino Fundamental</th>
				      <th id="h13" class="th3" colspan="5">EJA Ensino Médio</th>
				      </tr>
				    <tr>
				      <td class="th5" colspan="3"> 0</td>
				      <td class="th5" colspan="3"> 30</td>
				      <td class="th5" colspan="3"> 82</td>
				      <td class="th5" colspan="2"> 0</td>
				      <td class="th5" colspan="3"> 0</td>
				      <td class="th5" colspan="3"> 18</td>
				    <td class="th5" colspan="5"> 0</td>
				    </tr>
				    <tr>
				    <th height="38" id="h1" class="th4" colspan="22">IDEB</th>
				  </tr>

				    <tr>
				      <td height="24" id="h7" class="th4" colspan="9">Anos Iniciais do Ensino Fundamental</td>
				      <td id="h7" class="th4" colspan="13">Anos Finais do Ensino Fundamental</td>
				     </tr>
				    <tr>
				      <td id="h14" class="th4" colspan="5">IDEB Observado</td>
				      <td id="h14" class="th4" colspan="4">Metas</td>
				      <td id="h7" class="th4" colspan="7">IDEB Observado</td>
				      <td id="h7" class="th4" colspan="6">Metas</td>
				    </tr>
				    <tr>
				      <td id="h14" class="th4" colspan="2">2005</td>
				      <td id="h14" class="th4" colspan="3">2007</td>
				      <td id="h15" class="th4" colspan="2">2007</td>
				      <td id="h15" class="th4" colspan="2">2021</td>
				      <td id="h7" class="th4" colspan="3">2005</td>
				      <td id="h7" class="th4" colspan="4">2007</td>
				      <td id="h7" class="th4" colspan="3">2007</td>
				      <td id="h7" class="th4" colspan="3">2021</td>
				     </tr>
				    <tr>
				      <td id="h14" class="th5" colspan="2">  - </td>
				      <td id="h14" class="th5" colspan="3">  - </td>
				      <td id="h23" class="th5" colspan="2">  - </td>
				      <td id="h15" class="th5" colspan="2">  - </td>
				      <td id="h7" class="th5" colspan="3">  - </td>
				      <td id="h7" class="th5" colspan="4">  - </td>
				      <td id="h7" class="th5" colspan="3">  - </td>
				      <td id="h7" class="th5" colspan="3">  - </td>
				     </tr>
				-->
				<!--          <tr>
				      <td colspan="2" class="th5" id="h14">-</td>
				      <td colspan="3" class="th5" id="h14">-</td>
				      <td colspan="2" class="th5" id="h23">-</td>
				      <td colspan="2" class="th5" id="h15">-</td>
				      <td colspan="3" class="th5" id="h7">-</td>
				      <td colspan="4" class="th5" id="h7">-</td>
				      <td colspan="3" class="th5" id="h7">-</td>
				      <td colspan="3" class="th5" id="h7">-</td>
				     </tr>
				-->  </tbody>
				</table>';

	return $css.$html;
}

function pdeescola_monta_sql_relatorio($tipo = ''){

	$where = array();

	extract($_REQUEST);
/*
	if($_REQUEST['agrupador']){
		for($x=0; $x<count($_REQUEST['agrupador']); $x++){
			if($_REQUEST['agrupador'][$x] == 'atividade' || $_REQUEST['agrupador'][$x] == 'macrocampo'){
				$execcaoAgrupadorCampos = 'mta.mtadescricao as atividade, mtm.mtmdescricao as macrocampo,';
				$execcaoAgrupadorGroupBy = ', atividade, macroCampo';
			}
		}

	}
*/
	// ano de referencia
	if( $anoref ){
		array_push($where, " mem.memanoreferencia = '" . $anoref . "'");
	}

	// modalidade
	if( $modalidade ){
		array_push($where, " mem.memmodalidadeensino IN ('" . implode( "','", $modalidade ) . "') ");
	}

	// região
	if( $regiao[0] && $regiao_campo_flag ){
		array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $regiao ) . "') ");
	}

	// UF
	if( $uf[0] && $uf_campo_flag ){
		array_push($where, " ed.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uf ) . "') ");
	}

	// municipio
	if( $municipio[0]  && $municipio_campo_flag ){
		array_push($where, " ed.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ");
	}

	// atividade
	if( $atividade[0] && $atividade_campo_flag ){
		array_push($where, " mta.mtaid " . (!$atividade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $atividade ) . ") ");
	}

	// macroCampo
	if( $macrocampo[0] && $macrocampo_campo_flag ){
		array_push($where, " mtm.mtmid " . (!$macrocampo_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $macrocampo ) . ") ");
	}

	// Filtro de restricao
	switch ( $possuiPst ) {
		case 'sim' : $stFiltro .= " and (mem.memadesaopst = 'S' and mem.memstatus = 'A')"; break;
		case 'nao' : $stFiltro .= " and (mem.memadesaopst = 'N' and mem.memstatus = 'A')"; break;
		case 'todos' : $stFiltro .= ""; break;
	}

	// Filtro de 'Estado Atual'
	$joinEstado = "";
	$primeiraParte = " INNER JOIN pdeescola.meatividade mea
						ON mem.memid = mea.memid
					INNER JOIN pdeescola.metipoatividade mta
						ON mea.mtaid = mta.mtaid
					INNER JOIN pdeescola.metipomacrocampo mtm
						ON mta.mtmid = mtm.mtmid
					INNER JOIN pdeescola.mealunoparticipante map
						ON mea.memid = map.memid ";
	$wherePrimeiraParte = " AND mta.mtasituacao = 'A' AND mtm.mtmsituacao = 'A' ";
	
	if( !is_null($_REQUEST['estado']) && $_REQUEST['estado'] != "" )
	{
		if($_REQUEST['estado'] == 0)
		{
			array_push($where, " mem.docid is null ");
			$primeiraParte = " LEFT JOIN pdeescola.meatividade mea
									ON mem.memid = mea.memid
								LEFT JOIN pdeescola.metipoatividade mta
									ON mea.mtaid = mta.mtaid
								LEFT JOIN pdeescola.metipomacrocampo mtm
									ON mta.mtmid = mtm.mtmid
								LEFT JOIN pdeescola.mealunoparticipante map
									ON mea.memid = map.memid ";
			$wherePrimeiraParte = "";
		}
		else
		{
			$joinEstado = "INNER JOIN workflow.documento doc ON doc.docid = mem.docid AND doc.esdid = ".$_REQUEST['estado'];
		}
	}
	
	
	
	$campoSerie = ",
					CASE 
						WHEN map.sceid = 1 THEN '1ª Série'
						WHEN map.sceid = 2 THEN '2ª Série'
						WHEN map.sceid = 3 THEN '3ª Série'
						WHEN map.sceid = 4 THEN '4ª Série'
						WHEN map.sceid = 5 THEN '5ª Série'
						WHEN map.sceid = 6 THEN '6ª Série'
						WHEN map.sceid = 7 THEN '7ª Série'
						WHEN map.sceid = 8 THEN '8ª Série'
						WHEN map.sceid = 9 THEN '9ª Série'
						WHEN map.sceid = 20 THEN '1° Ano do Ensino Médio'
						WHEN map.sceid = 21 THEN '2° Ano do Ensino Médio'
						WHEN map.sceid = 22 THEN '3° Ano do Ensino Médio'
					END as serie";
	
	if($tipo == 'xls') $campoSerie = ", '' as serie ";
	
		
		
	// monta o sql
	$sql = "SELECT
				ent.entnome as nomedaescola,
				ent.entnome as nomedaescolaxls,
				CASE WHEN memmodalidadeensino = 'M' THEN 'Médio'
					WHEN memmodalidadeensino = 'F' THEN 'Fundamental'
					ELSE 'Outro' END as modalidade,
				coalesce(x.valor,0) as vlrpago,
				coalesce(x.vlrsuplementar,0) as vlrsuplementar,
				--mem.memvlrsuplementar as vlrsuplementar,
				pa.paidescricao as pais,
				re.regdescricao as regiao,
				CASE WHEN ed.estuf <> '' THEN ed.estuf
					ELSE 'Não Informado' END as uf,
				tm.mundescricao as municipio,
				mta.mtadescricao as atividade,
				mtm.mtmdescricao as macrocampo,
				CASE WHEN mem.memmodalidadeensino = 'M' THEN coalesce(sum(map.mapquantidade), 0) END as medio,
				CASE WHEN mem.memmodalidadeensino = 'F' THEN coalesce(sum(map.mapquantidade), 0) END as fundamental,
				coalesce(sum(map.mapquantidade),0) as total
				$campoSerie
			FROM entidade.entidade ent

				--2ª parte
				INNER JOIN entidade.endereco ed
					ON ent.entid = ed.entid
				LEFT JOIN territorios.estado et
					ON ed.estuf = et.estuf
				LEFT JOIN territorios.regiao re
					ON re.regcod = et.regcod
				LEFT JOIN territorios.municipio tm
					ON tm.muncod = ed.muncod
				LEFT JOIN territorios.mesoregiao me
					ON me.mescod = tm.mescod
				LEFT JOIN territorios.pais pa
					ON pa.paiid = re.paiid

				--1ª parte
				INNER JOIN pdeescola.memaiseducacao mem
					ON ent.entid = mem.entid
				".$primeiraParte."
					
				".$joinEstado."					
					
				left join (
						select
							mem.memid,
							max(mta.mtaid) as mtaid,
							memvlrpago as valor,
							memvlrsuplementar as vlrsuplementar
						from pdeescola.memaiseducacao mem
							INNER JOIN pdeescola.meatividade mea
								ON mem.memid = mea.memid
							INNER JOIN pdeescola.metipoatividade mta
								ON mea.mtaid = mta.mtaid
							INNER JOIN pdeescola.metipomacrocampo mtm
								ON mta.mtmid = mtm.mtmid
							INNER JOIN pdeescola.mealunoparticipante map
								ON mea.memid = map.memid
						where mem.memstatus = 'A'
						and coalesce(memvlrpago,0) <> 0
						or coalesce(memvlrsuplementar,0) <> 0
						group by mem.memid, memvlrpago, memvlrsuplementar
					) x ON x.memid = mem.memid and x.mtaid = mta.mtaid
			WHERE
				mem.memstatus = 'A' ".$wherePrimeiraParte." "
			. ( is_array($where) ? ' AND' . implode(' AND ', $where) : '' )
			. $stFiltro . "

			GROUP BY
				nomedaescola, nomedaescolaxls, vlrpago, vlrsuplementar, pais, regiao, uf, municipio,
				mem.memmodalidadeensino, atividade, macroCampo, modalidade, ent.entid, serie

			ORDER BY
				" . (is_array( $agrupador ) ?  implode(",", $agrupador) : "pais");
	

			//ver($sql);
			//die;

	return $sql;

}

function pdeescola_monta_coluna_relatorio(){

	$coluna = array();

	/*foreach ( $_REQUEST['modalidade'] as $valor ){

		switch( $valor ){

			case 'M':
				array_push( $coluna, array("campo" 	  => "medio",
								   		   "label" 	  => "Ensino Médio",
								   		   "blockAgp" => "nomedaescola",
								   		   "type"	  => "character") );
			break;
			case 'F':
				array_push( $coluna, array("campo" 	  => "fundamental",
								   		   "label" 	  => "Ensino Fundamental",
								   		   "blockAgp" => "nomedaescola",
								   		   "type"	  => "character") );
			break;
		}

	}*/
	//ver($escola, $_REQUEST['escola'], $dados);
	//die();

	/*array_push( $coluna, array(		"campo" 	=> "escola",
								   		"label" 	=> "Escola",
								   		"blockAgp" 	=> "nomedaescola",
								   		"type"	 	=> "character") );

	array_push( $coluna, array(			"campo" 	=> "vlrpago",
								   		"label" 	=> "Valor Pago",
								   		"blockAgp" 	=> "nomedaescola",
								   		"type"	 	=> "character") );

	array_push( $coluna, array(			"campo" 	=> "vlrsuplementar",
								   		"label" 	=> "Valor Suplementar",
								   		"blockAgp" 	=> "nomedaescola",
								   		"type"	 	=> "character") );
	
	array_push( $coluna, array(			"campo" 	=> "vlrsuplementar",
								   		"label" 	=> "Valor Suplementar",
								   		"blockAgp" 	=> array('atividade','macrocampo'),
								   		"type"	 	=> "character") );
	
	array_push( $coluna, array(			"campo" 	=> "total",
								   		"label" 	=> "Total de Alunos",
								   		"blockAgp" 	=> "nomedaescola",
								   		"type"	 	=> "numeric") );*/
	
	array_push( $coluna, array(			"campo" 	=> "total",
								   		"label" 	=> "Quantidade de Alunos",
								   		"type"	 	=> "numeric") );

	return $coluna;

}

function pdeescola_monta_agp_relatorio(){

	$agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];

			
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("nomedaescola",
										  "vlrpago",
										  "vlrsuplementar",
										  "medio",
										  "fundamental",
										  "total"),
				"agrupadorDetalhamento" => array(
													array(
															"campo" => "macrocampo",
															"label" => "Macro Campo"
														  ),
													array(
															"campo" => "atividade",
															"label" => "Atividade"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Município"
														  ),
													array(
															"campo" => "pais",
													  		"label" => "País"
											   			  ),
											   		array(
															"campo" => "regiao",
													  		"label" => "Região"
											   			  ),
											   		array(
															"campo" => "nomedaescola",
													  		"label" => "Nome da Escola"
											   			  ),
											   		array(
															"campo" => "serie",
													  		"label" => "Série"
											   			  )
												)
				);
				
	

	foreach ( $agrupador as $val ){
		switch( $val ){
			case "macrocampo":
				array_push($agp['agrupador'], array(
													"campo" => "macrocampo",
											  		"label" => "Macro Campo")
									   				);
			break;
			case "atividade":
				array_push($agp['agrupador'], array(
													"campo" => "atividade",
											  		"label" => "Atividade")
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")
									   				);
			break;
			case "pais":
				array_push($agp['agrupador'], array(
													"campo" => "pais",
											  		"label" => "País")
									   				);
			break;
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Região")
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")
									   				);
			break;
//************************************************************************
			case "modalidade":
				array_push($agp['agrupador'], array(
													"campo" => "modalidade",
											  		"label" => "Modalidade")
									   				);
			break;
//************************************************************************
			case "nomedaescola":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaescola",
											  		"label" => "Nome da Escola")
									   				);
			break;
			case "nomedaescolaxls":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaescolaxls",
											  		"label" => "Nome da Escola")
									   				);
			break;
			case "nivelpreenchimento":
				array_push($agp['agrupador'], array(
													"campo" => "nivelpreenchimento",
											  		"label" => "Nível de Preenchimento")
									   				);
			break;
			
			case "serie":
				array_push($agp['agrupador'], array(
													"campo" => "serie",
											  		"label" => "Série")
									   				);
			break;
		}
	}

	array_push($agp['agrupador'], array(
										"campo" => "nomedaescola",
								  		"label" => "Nome da Escola")
						   				);


	return $agp;

}

/******************************************
 * RELATÓRIO SITUACIONAL DO PDE ESCOLA
 * Funções para utilização no relatório.xls
 */

function pdeescola_situacional_monta_sql_relatorio(){

	$where = array();

	extract($_REQUEST);
	
	// Regiao
	if ($f_regiao[0] && $regiao_campo_flag){
		$where[] = " tr.regcod ".(!$f_regiao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_regiao)."') ";
	}
	// Estado
	if ($f_estuf[0] && $estuf_campo_flag){
		$where[] = " tm.estuf ".(!$f_estuf_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_estuf)."') ";
	}
	// Município
	if ($f_municipio[0] && $municipio_campo_flag){
		$where[] = " tm.muncod ".(!$f_municipio_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_municipio)."') ";
	}

	// Codigo INEP
	if ($f_inep[0] && $inep_campo_flag){
		$where[] = " e.entcodent ".(!$f_inep_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_inep)."') ";
	}
	
	if(!$agrupador[0]) {
		$agrupador = array();
	}
 
	$count = count($_POST['qapid']);
	$order = implode(",", $_POST['agrupador']);
	
					$sql = "SELECT DISTINCT 
							e.entid, 
							tm.estuf as ESTADO,
							regdescricao as REGIAO,
							tm.mundescricao as MUNICIPIO,
							e.entcodent as INEP, 
							e.entnome as NOME_ESCOLA, 
							epd.epiclasse as IDEB, 
							CASE
							    WHEN SUM(pde.pdevlrplanocusteio + pde.pdevlrplanocapital)  IS NOT NULL THEN SUM(pde.pdevlrplanocusteio + pde.pdevlrplanocapital)
							    ELSE 0.00
							END as PLANO,
							CASE
							    WHEN SUM(pde.pdevlrcomplementarcusteio + pde.pdevlrcomplementarcapital)  IS NOT NULL THEN SUM(pde.pdevlrcomplementarcusteio + pde.pdevlrcomplementarcapital)
							    ELSE 0.00
							END as PARCELA,
							CASE
							    WHEN est.esddsc IS NOT NULL THEN est.esddsc
							    ELSE 'Aguardando iniciação'::character varying
							END AS status, 
							CASE
							    WHEN hd.htddata IS NULL THEN ' -- '::text
							    ELSE to_char(hd.htddata, 'dd/mm/yyyy'::text)
							END AS data_ultimo_tramite
						FROM 
							entidade.entidade e
						JOIN 
							entidade.endereco endi ON endi.entid = e.entid
						JOIN 	
							territorios.municipio tm ON tm.muncod = endi.muncod::bpchar
						JOIN 
							territorios.estado te ON te.estuf = tm.estuf
						JOIN 
							territorios.regiao tr ON tr.regcod = te.regcod	
						JOIN 
							entidade.entidadedetalhe ed ON ed.entid = e.entid AND ed.entpdeescola = true	
						JOIN 
							pdeescola.entpdeideb epd ON epd.epientcodent = ed.entcodent::bpchar
						LEFT JOIN 
							pdeescola.pdeescola pde ON pde.entid = e.entid AND pde.pdeano = 2008::numeric
						LEFT JOIN 
							workflow.documento d ON d.docid = pde.docid
						LEFT JOIN 
							( SELECT historicodocumento.docid, max(historicodocumento.htddata) AS htddata FROM workflow.historicodocumento GROUP BY historicodocumento.docid) hd ON hd.docid = d.docid
						LEFT JOIN 
							workflow.estadodocumento est ON est.esdid = d.esdid
						WHERE 
							(e.tpcid = ANY (ARRAY[1, 3]))
							".$where = (!empty($where) ? "AND ".implode(" AND ", $where) : "")."
						GROUP BY
							e.entid, 
							tm.estuf, 
							tr.regdescricao,
							tm.mundescricao, 
							e.entcodent, 
							e.entnome, 
							epd.epiclasse, 
							pde.pdevlrplanocusteio, 
							pde.pdevlrplanocapital, 
							pde.pdevlrcomplementarcapital, 
							pde.pdevlrcomplementarcusteio,
							d.esdid, 
							est.esddsc, 
							hd.htddata
						ORDER BY
							$order";

	return $sql;
}

function pdeescola_situacional_monta_agp_relatorio(){

	$agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];

		$agrupador = $_POST['agrupador'];
	
	$agp = array(
			"agrupador" => array(),
			"agrupadoColuna" => array(
										"nome_escola",
										"ideb",
										"plano",
										"parcela",
										"status",
										"data_ultimo_tramite"  		
									  )	  
			);
	
	foreach ($agrupador as $val): 
		switch ($val) {
			case 'regiao':
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Regiao")										
									   				);				
		    	continue;
		        break;
		    case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado")										
									   				);				
		    	continue;
		        break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);					
		    	continue;
		        break;	
		    case 'inep':
				array_push($agp['agrupador'], array(
													"campo" => "inep",
											  		"label" => "Codigo INEP")										
									   				);					
		    	continue;
		        break;		
		}
	endforeach;
	
	return $agp;

}

function pdeescola_situacional_monta_coluna_relatorio(){

	global $_REQUEST;
	global $db;
	
	$arDados = $_REQUEST['qapid'];
	
	$coluna    = array(
					array(
						  "campo" => "nome_escola",
				   		  "label" => "Nome Escola",
						  "blockAgp" => array("estado", "municipio"),
					),		
					array(
						  "campo" => "ideb",
				   		  "label" => "IDEB"	
					),
					array(
						  "campo" => "plano",
				   		  "label" => "Plano"	
					),
					array(
						  "campo" 	 => "parcela",
				   		  "label" 	 => "Parcela"
					),	
					array(
						  "campo" 	 => "status",
				   		  "label" 	 => "Status"
					),	
					array(
						  "campo" => "data_ultimo_tramite",
				   		  "label" => "Data Tramitação"		  	
					)				
				  );
	return $coluna;			  	

}

/*
 * ******************FIM DO RELATÓRIO SITUACIONAL**********************
 */

/******************************************
 * RELATÓRIO DE AUTOAVALIAÇÃO DO PDE ESCOLA
 * Funções para utilização no relatório
 */

function pdeescola_autoavaliacao_monta_sql_relatorio(){

	$where = array();

	extract($_REQUEST);

	if ($tipo){
		$where[] = "e.tpcid IN($tipo)";
	}
	// Regiao
	if ($f_regiao[0] && $regiao_campo_flag){
		$where[] = " re.regcod ".(!$f_regiao_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_regiao)."') ";
	}
	// Estado
	if ($f_estuf[0] && $estuf_campo_flag){
		$where[] = " m.estuf ".(!$f_estuf_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_estuf)."') ";
	}
	// Município
	if ($f_municipio[0] && $municipio_campo_flag){
		$where[] = " m.muncod ".(!$f_municipio_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_municipio)."') ";
	}
	// Dependência administrativa
	if(isset($_POST["tpcid"]) && $_POST["tpcid"] != "") {
		$where[] = " e.tpcid IN ('".$_POST["tpcid"]."') ";
		//array_push($where, "esd.esdid in (".implode(",",$_REQUEST["esdid"]).")");
	}
	// Questoes	
	if (isset($_POST["qapid"]) && $_POST["qapid"] != ""){
		$where[] =  " qap.qapid IN ('".implode("','",$_POST["qapid"])."')"; 
	}	
	// Codigo INEP
	if ($f_inep[0] && $inep_campo_flag){
		$where[] = " e.entcodent ".(!$f_inep_campo_excludente ? ' IN ' : ' NOT IN ')."('".implode("','",$f_inep)."') ";
	}
	
	if(!$agrupador[0]) {
		$agrupador = array();
	}
 
	$count = count($_POST['qapid']);

	
	$sql = "SELECT 
					inep,
					nome,
					regiao,
					estado,
					municipio,
					--qapid,
					--qapcodigo,
					tipo,
					SUM(Q_1_1) AS Q_1_1,
					SUM(Q_1_2) AS Q_1_2,
					SUM(Q_1_3) AS Q_1_3,
					SUM(Q_1_4) AS Q_1_4,
					SUM(Q_1_5) AS Q_1_5,
					SUM(Q_2_1) AS Q_2_1,
					SUM(Q_2_2) AS Q_2_2,
					SUM(Q_2_3) AS Q_2_3,
					SUM(Q_2_4) AS Q_2_4,
					SUM(Q_2_5) AS Q_2_5,
					SUM(Q_3_1) AS Q_3_1,
					SUM(Q_3_2) AS Q_3_2,
					SUM(Q_3_3) AS Q_3_3,
					SUM(Q_3_4) AS Q_3_4,
					SUM(Q_3_5) AS Q_3_5,
					SUM(Q_3_6) AS Q_3_6,
					SUM(Q_4_1) AS Q_4_1,
					SUM(Q_4_2) AS Q_4_2,
					SUM(Q_4_3) AS Q_4_3,
					SUM(Q_4_4) AS Q_4_4,
					SUM(Q_4_5) AS Q_4_5,
					SUM(Q_4_6) AS Q_4_6,
					SUM(Q_5_1) AS Q_5_1,
					SUM(Q_5_2) AS Q_5_2,
					SUM(Q_5_3) AS Q_5_3,
					SUM(Q_5_4) AS Q_5_4,
					SUM(Q_5_5) AS Q_5_5,
					SUM(Q_5_6) AS Q_5_6,
					SUM(Q_5_7) AS Q_5_7,
					SUM(Q_6_1) AS Q_6_1,
					SUM(Q_6_2) AS Q_6_2,
					SUM(Q_6_3) AS Q_6_3,
					SUM(Q_6_4) AS Q_6_4,
					SUM(Q_6_5) AS Q_6_5,
					SUM(Q_6_6) AS Q_6_6,
					SUM(Q_7_1) AS Q_7_1,
					SUM(Q_7_2) AS Q_7_2,
					SUM(Q_7_3) AS Q_7_3,
					SUM(Q_7_4) AS Q_7_4,
					SUM(Q_7_5) AS Q_7_5,
					SUM(Q_7_6) AS Q_7_6,
					SUM(Q_7_7) AS Q_7_7,
					SUM(Q_7_8) AS Q_7_8,
					SUM(Q_7_9) AS Q_7_9,
					SUM(Q_7_10) AS Q_7_10,
					SUM(Q_7_11) AS Q_7_11,
					SUM(Q_8_1) AS Q_8_1,
					SUM(Q_8_2) AS Q_8_2,
					SUM(Q_8_3) AS Q_8_3,
					SUM(Q_8_4) AS Q_8_4,
					SUM(Q_8_5) AS Q_8_5,
					SUM(Q_8_6) AS Q_8_6,
					SUM(Q_8_7) AS Q_8_7,
					SUM(Q_8_8) AS Q_8_8,
					SUM(Q_8_9) AS Q_8_9,					
					SUM (total) as total,
					maximo,
					(((((total/maximo::numeric)*100)) :: numeric(12,0))|| ' % ' ) as  perc
			FROM (
				SELECT
					DISTINCT
					e.entcodent as inep,
					'' || p.pdenome || '' AS nome,
					re.regdescricao as regiao,
					m.estuf AS estado,
					m.mundescricao AS municipio,
					qap.qapid,
					qap.qapcodigo,
					CASE e.tpcid WHEN 1 THEN 'Escolas Estaduais' ELSE 'Escolas Municipais' END AS tipo,
					CASE WHEN qapcodigo = '1.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_1,
					CASE WHEN qapcodigo = '1.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_2,
					CASE WHEN qapcodigo = '1.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_3,
					CASE WHEN qapcodigo = '1.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_1_4,
					CASE WHEN qapcodigo = '1.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_1_5,
					CASE WHEN qapcodigo = '2.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_1,
					CASE WHEN qapcodigo = '2.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_2,
					CASE WHEN qapcodigo = '2.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_3,
					CASE WHEN qapcodigo = '2.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_2_4,
					CASE WHEN qapcodigo = '2.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_2_5,
					CASE WHEN qapcodigo = '3.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_1,
					CASE WHEN qapcodigo = '3.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_2,
					CASE WHEN qapcodigo = '3.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_3,
					CASE WHEN qapcodigo = '3.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_3_4,
					CASE WHEN qapcodigo = '3.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_3_5,
					CASE WHEN qapcodigo = '3.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_3_6,
					CASE WHEN qapcodigo = '4.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_1,
					CASE WHEN qapcodigo = '4.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_2,
					CASE WHEN qapcodigo = '4.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_3,
					CASE WHEN qapcodigo = '4.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_4_4,
					CASE WHEN qapcodigo = '4.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_4_5,
					CASE WHEN qapcodigo = '4.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_4_6,
					CASE WHEN qapcodigo = '5.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_1,
					CASE WHEN qapcodigo = '5.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_2,
					CASE WHEN qapcodigo = '5.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_3,
					CASE WHEN qapcodigo = '5.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_5_4,
					CASE WHEN qapcodigo = '5.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_5_5,
					CASE WHEN qapcodigo = '5.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_5_6,
					CASE WHEN qapcodigo = '5.7'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_5_7,
					CASE WHEN qapcodigo = '6.1'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_1,
					CASE WHEN qapcodigo = '6.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_2,
					CASE WHEN qapcodigo = '6.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_3,
					CASE WHEN qapcodigo = '6.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_6_4,
					CASE WHEN qapcodigo = '6.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_6_5,
					CASE WHEN qapcodigo = '6.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_6_6,
					CASE WHEN qapcodigo = '7.1'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_1,
					CASE WHEN qapcodigo = '7.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_2,
					CASE WHEN qapcodigo = '7.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_3,
					CASE WHEN qapcodigo = '7.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_4,
					CASE WHEN qapcodigo = '7.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_5,
					CASE WHEN qapcodigo = '7.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_6,
					CASE WHEN qapcodigo = '7.7'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_7,
					CASE WHEN qapcodigo = '7.8'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_8,
					CASE WHEN qapcodigo = '7.9'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_9,
					CASE WHEN qapcodigo = '7.10'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_7_10,
					CASE WHEN qapcodigo = '7.11'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_7_11,
					CASE WHEN qapcodigo = '8.1'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_1,
					CASE WHEN qapcodigo = '8.2'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_2,
					CASE WHEN qapcodigo = '8.3'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_3,
					CASE WHEN qapcodigo = '8.4'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_4,
					CASE WHEN qapcodigo = '8.5'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_5,
					CASE WHEN qapcodigo = '8.6'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_6,
					CASE WHEN qapcodigo = '8.7'
						THEN ea.eavvalor
						ELSE 0	
					END AS Q_8_7,
					CASE WHEN qapcodigo = '8.8'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_8,
					CASE WHEN qapcodigo = '8.9'
						THEN ea.eavvalor
						ELSE 0
					END AS Q_8_9,
					ea.eavvalor as total,
					".$count." * 5 AS maximo
										
				FROM
					entidade.entidade e
				INNER JOIN 
					pdeescola.pdeescola p ON p.entid = e.entid	
				INNER JOIN 
					pdeescola.entpdeideb ei ON ei.epientcodent = p.pdecodinep
				INNER JOIN 
					entidade.endereco e1 ON e1.entid = e.entid
				INNER JOIN 
					territorios.municipio m ON m.muncod = e1.muncod
				INNER JOIN 
					territorios.estado est ON est.estuf = m.estuf
				INNER JOIN 
					territorios.regiao re ON re.regcod = est.regcod				
				INNER JOIN 
					pdeescola.avaliacaoplano ap ON ap.pdeid = p.pdeid
				INNER JOIN 
					pdeescola.questaoavaliacaoplano qap ON qap.qapid = ap.qapid AND ap.aplid IS NOT NULL AND qap.qapidpai IS NOT NULL
				INNER JOIN 
					pdeescola.escalaavaliacao ea ON ea.eavid = ap.eavid 
				".$where = (!empty($where) ? "WHERE ".implode(" AND ", $where) : "")."
				ORDER BY 
					inep, qapcodigo
				) AS f
			GROUP BY
				inep,
				nome,
				regiao,
				estado,
				municipio,
				tipo,
				f.total,
				f.maximo
				";
	return $sql;
}

function pdeescola_autoavaliacao_monta_agp_relatorio(){

	$agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];

		$agrupador = $_POST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
							   				"q_1_1",
											"q_1_2",
											"q_1_3",
											"q_1_4",
											"q_1_5",
											"q_2_1",
											"q_2_2",
											"q_2_3",
											"q_2_4",
											"q_2_5",
											"q_3_1",
											"q_3_2",
											"q_3_3",
											"q_3_4",
											"q_3_5",
											"q_3_6",
											"q_4_1",
											"q_4_2",
											"q_4_3",
											"q_4_4",
											"q_4_5",
											"q_4_6",
											"q_5_1",
											"q_5_2",
											"q_5_3",
											"q_5_4",
											"q_5_5",
											"q_5_6",
											"q_5_7",
											"q_6_1",
											"q_6_2",
											"q_6_3",
											"q_6_4",
											"q_6_5",
											"q_6_6",
											"q_7_1",
											"q_7_2",
											"q_7_3",
											"q_7_4",
											"q_7_5",
											"q_7_6",
											"q_7_7",
											"q_7_8",
											"q_7_9",
											"q_7_10",
											"q_7_11",
											"q_8_1",
											"q_8_2",
											"q_8_3",
											"q_8_4",
											"q_8_5",
											"q_8_6",
											"q_8_7",
											"q_8_8",
											"q_8_9",
											"total",
											"maximo",
											"perc"
	   		
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
			case 'regiao':
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Regiao")										
									   				);				
		    	continue;
		        break;
		    case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "Estado")										
									   				);				
		    	continue;
		        break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);					
		    	continue;
		        break;	
		    case 'inep':
				array_push($agp['agrupador'], array(
													"campo" => "inep",
											  		"label" => "Codigo INEP")										
									   				);					
		    	continue;
		        break;		
		}
	endforeach;
	
	return $agp;

}

function pdeescola_autoavaliacao_monta_coluna_relatorio(){

	global $_REQUEST;
	global $db;
	
	$arDados = $_REQUEST['qapid'];
	
	$coluna = array();
	foreach ($arDados as $dados){

		$sql = "SELECT  
				'q_' || replace(qapcodigo, '.', '_') as campo,
				'Questão ' || qapcodigo as label
			FROM 
				pdeescola.questaoavaliacaoplano 
			WHERE 
				qapid = " .  $dados;

		$dado = $db->pegaLinha($sql);
		array_push($coluna, array(
					  				"campo" 	 => $dado['campo'],
			   		  				"label" 	 => $dado['label'],
					  				"type"	 	=> "numeric"					
								  ));
	}
		array_push($coluna,array(
									"campo" 	 => "total",
						   		  	"label" 	 => "Totais",
								  	"type"	 	=> "numeric"
								  ));									
		array_push($coluna,array(
								  	"campo" 	 => "maximo",
						   		  	"label" 	 => "Total Máximo",
								  	"type"	 	=> "numeric"
								  )	);
		array_push($coluna,array(
								  	"campo" 	 => "perc",
						   		  	"label" 	 => "%",
								  	"type"	 	=> "string"
								  )	);
					  	
	return $coluna;			  	

}

/*
 * ******************FIM DO RELATÓRIO**********************
 */

function verificaAvaliacaoMec() {

	global $db, $docid;

	$retorno = $db->carregar("SELECT
									to_char(htddata,'YYYY-MM-DD') as data
								FROM
									workflow.historicodocumento
								WHERE
									docid = {$docid} and
									aedid in (139,210)
								ORDER BY
									htddata asc");

	// A escola já esteve na situação de 'Avaliação MEC'

	if( $retorno ){

		$dataRetorno = strtotime( $retorno[0]["data"] );
		$dataLimite  = strtotime( "2011-12-18" );

		if( $dataRetorno <= $dataLimite ){
			return true;
		}else{
			return false;
		}
	}else if( strtotime( date("Y-m-d") ) <= strtotime( '2011-12-18' ) ){
		return true;
	}else{
		return false;
	}

}

function verificaValorParcelaComplementarComentario($cmddsc){
	
	global $db;
	
	$docid = pegarDocid($_SESSION['entid']);
	$aedid = $_REQUEST['aedid'];
	
	$dados = array();
	
	wf_alterarEstado($docid, $aedid, $cmddsc, $dados);
	
}

function verificaEdicaoParcelaComplementar(){
	
	$docid = pegarDocid($_SESSION['entid']);	
	$esdid = pegarEstadoAtual($docid);
	
	if($esdid == VALIDACAO_PELO_MEC_WF){
		
		return true;
		
	} else {
		
		return false;
	}
}

function pegaSomaValoresComplementares(){
	
	global $db;
	
	// Pega a soma dos valores complementares
	$sql = "select 
				(pdevlrcomplementarcusteio+pdevlrcomplementarcapital) as valor 
			from pdeescola.pdeescola 
			where pdeid = {$_SESSION['pdeid']}";
			
	return $db->pegaUm($sql);
}

function pegaSomaValoresPlanos(){
	
	global $db;
	
	// Pega a soma dos valores dos planos
	$sql = "select 
				(pdevlrplanocusteio+pdevlrplanocapital) as valor 
			from pdeescola.pdeescola 
			where pdeid = {$_SESSION['pdeid']}";
			
	return $db->pegaUm($sql);
}

function verificaResultadoAltuavaliacao(){
	
	global $db,  $entid;
	
	$entid = $_SESSION['entid'] ? $_SESSION['entid'] : $entid; 
	
	$sql = "SELECT
				entid,
				qtdap
			FROM pdeescola.preenchimento
			WHERE entid ='".$entid."'";
			
	$dados = $db->carregar($sql);
	
	return $dados[0]["qtdap"];
}

function verificaParcelaComplementar(){
	
	global $db;
	
	$sql = "SELECT 
				pdeid		
			FROM (
				SELECT 
					pde.pdeid,		
					COALESCE(sum((psp.pasquantidade * psp.pasvalorunitario)),0) as totalmec,		
					COALESCE(pdevlrcomplementarcusteio + pdevlrcomplementarcapital,0) as totalfnde
				FROM pdeescola.pdeescola pde
				INNER JOIN pdeescola.planoacaosegundaparcela psp ON psp.pdeid = pde.pdeid
				GROUP BY pde.pdeid, pdevlrcomplementarcusteio, pdevlrcomplementarcapital				
			) as compara
			WHERE totalmec = totalfnde
			AND pdeid = '{$_SESSION['pdeid']}'";
			
	if($db->pegaUm($sql)){
		return true;
	}else{
		return false;
	}	
}

function verificaPreenchimentoAutoavaliacao(){
	
	global $db;
	
	$sql = "select entcodent from entidade.entidade where entid = '{$_SESSION['entid']}'";
	$entcodent = $db->pegaUm($sql);
	
	$sql = "SELECT distinct a.pdeid
		   FROM  pdeescola.avaliacaoplano as a
		   LEFT JOIN  pdeescola.pdeescola as p on p.pdeid = a.pdeid
		   LEFT JOIN  pdeescola.preenchimento as pp on pp.entid = p.entid --AND pp.esdid = 90
		   WHERE pp.entcodent LIKE '%{$entcodent}%'
		   GROUP BY a.pdeid
		   HAVING count(a.qapid) >= 55";
									   
	$verificaAutoavaliacao = $db->pegaUm($sql);
	
	return $verificaAutoavaliacao;
}

function montaListaPainelEscola($sql, $idArvore, $arParemetros = array()){
	
		global $db;		
		
		$rsDados = $db->carregar($sql);
		$rsDados = $rsDados ? $rsDados : array();

		$html = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">';

		$indice = 0;
		$stParamentros = null;
		foreach($rsDados as $dados){
			
			$cor = ($indice % 2) ? "#F7F7F7" : "#FCFCFC";
			$idConteudo = $idArvore."_".$indice;

			if($arParemetros){
				foreach($arParemetros as $paramentro){
					
					$stParamentros .= "&{$paramentro}={$dados[$paramentro]}";					
				}
			}
			
			$html .= '<tr bgcolor="'.$cor.'" onmouseover="this.bgColor=\'#ffffcc\'" onmouseout="this.bgColor=\''.$cor.'\'" >
						<td width="80%">							
							<img onclick="carregaDadosAjax(\''.$idConteudo.'\', \''.$idArvore.'\', \''.$stParamentros.'\')" style="float:left;margin-top:2px;cursor:pointer; display:\'\';" id="imgMais_'.$idConteudo.'" align="abdmiddle" src="../imagens/mais.gif" title="Abrir" /> 
							<img onclick="ocultarDadosAjax(\''.$idConteudo.'\')" style="float:left;margin-top:2px;cursor:pointer; display:none;" id="imgMenos_'.$idConteudo.'" align="abdmiddle" src="../imagens/menos.gif" title="Fechar" />
							&nbsp;
							<a href="pdeescola.php?modulo=lista&acao=E'.$stParamentros.'">'.$dados['descricao'].'</a>
						</td>
						<td width="20%" style="color:rgb(0, 102, 204);text-align:right">
							'.str_replace(",",".",number_format($dados['total'])).'
						</td>
					</tr>
					<tr id="tr_'.$idConteudo.'" style="display:none;">
						<td colspan="2"><div id="'.$idConteudo.'"></div></td>
					</tr>';
			
			$total += $dados['total'];
			unset($stParamentros);
			$indice++;
		}
					
		$html .= '<tr style="background:#DCDCDC;"><td align="left"><b>Total</b></td><td align="right"><b>'.str_replace(",",".",number_format($total)).'</b></td></tr></table>';
		
		echo $html;
}

function montaListaSubitensPainelEscola($sql, $arParemetros = array()){
	
		global $db;		
		
		$rsDados = $db->carregar($sql, null, 3600);
		$rsDados = $rsDados ? $rsDados : array();		

		$html = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem">';

		$indice = 0;
		foreach($rsDados as $dados){
			
			if($arParemetros){
				foreach($arParemetros as $paramentro){
					
					$stParamentros .= "&{$paramentro}={$dados[$paramentro]}";					
				}
			}

			$cor = ($indice % 2) ? "#F7F7F7" : "#FCFCFC";			
			$html .= '<tr bgcolor="'.$cor.'" onmouseover="this.bgColor=\'#ffffcc\'" onmouseout="this.bgColor=\''.$cor.'\'" >
						<td width="80%">
							<img  src="../imagens/seta_filho.gif" />							 
							<a href="pdeescola.php?modulo=lista&acao=E'.$stParamentros.'">'.$dados['descricao'].'</a>							
						</td>
						<td width="20%" style="color:rgb(0, 102, 204);text-align:right">
							'.str_replace(",",".",number_format($dados['total'])).'
						</td>
					</tr>';
			
			$indice++;
			unset($stParamentros);
		}
					
		$html .= '</table>';
		
		echo $html;
}

function bloquearWorkflowPrazo()
{
	if(date('YmdHis') >= '20101030000000'){
		return false;
	}else{
		return true;
	}
}

function verificaQuestionario( $entid ){
	global $db;
	
	$sql = "SELECT
				ea.*
			FROM
				pdeescola.eacescolaquestionario ea
			INNER JOIN entidade.entidade e ON e.entcodent = ea.entcodent
			WHERE
				entid = ".$entid;
	
	return $db->pegaLinha( $sql );
}

function questRecuperaResponsabilidadePerfil($resp)
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
							   ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO.",
							   ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO.",
							   ".PDEESC_PERFIL_ESCOLA_QUEST_SEESP.")";

	return $db->pegaUm($sql);
}
?>