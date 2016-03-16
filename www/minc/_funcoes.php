<?php
//copia da função que identifica destinatario para enviar e-mail
require_once APPRAIZ . "includes/Email.php";

class EmailMinc extends Email {
	
	public function enviar( array $destinatarios, $assunto, $conteudo, array $arquivos, $remetenteInformado = array(), $destinoArquivo = null, $condicao = true ) {
		# identifica o remetente
		$remetente = $this->pegarUsuario( $_SESSION['usucpforigem'] );
		if ( !$remetente->usucpf ) {
			return false;
		}	
		
		$this->From     = isset( $remetenteInformado["usuemail"] ) ? $remetenteInformado["usuemail"] : $remetente->usuemail;
		$this->FromName = isset( $remetenteInformado["usunome"]  ) ? $remetenteInformado["usunome"]  : $remetente->usunome;
		
		# identifica os destinatários
		foreach ( $destinatarios as &$destinatario ) {
			$this->AddBCC( $destinatario["usuemail"], $destinatario["usunome"] );
		}
		# anexa os arquivos
		foreach ( $arquivos as $arquivo ) {
			if ( $arquivo["error"] == UPLOAD_ERR_NO_FILE ) {
				continue;
			}
			
			$this->AddAttachment( $destinoArquivo, basename( $destinoArquivo ) );
		}
		
		# formata assunto, conteudo e envia a mensagem
		$this->Subject = self::ASSUNTO . str_replace( "\'", "'", $assunto );
		$this->Body    = str_replace( "\'", "'", $conteudo );
		$this->IsHTML( true );
		set_time_limit( 180 );
		
		
		
		if( !$this->Send() ) {
			return false;
		}
		
		if($condicao){			
			return $this->registrar( $remetente, $destinatarios, $assunto, $conteudo );
		}else{			
			return true;
		}
	}
	
	public static function identificar_destinatarios( $orgao = null, $tipoEnsino = null, array $uo = array(), $ug = 0, array $perfis = array(), $outros = null, $statusUsuario = null, $ideb = null, $arMunicipios = array(), $cargo = null, $tpmid = array(), $esdid = null, $usustatus = null ) {
		global $db;
		
		$statusUsuario = ($statusUsuario ) ? $statusUsuario : 'A';

		//parâmetro para filtrar (query) usuários só ativos (envio de e-mail)
		$usustatus = ($usustatus) ? " and (u.usustatus = '".$usustatus."' or u.usustatus is null) " : '';
		
		$suscod = $statusUsuario ? " us.suscod = '". $statusUsuario ."' and ": "";

		# conjunto de regras de restrição
		$restricao = array();
		# restringe os usuários por órgão
		if ( !empty( $orgao ) ) {
			array_push( $restricao, " u.orgcod = '" . $orgao ."' " );
		}
		# restringe os usuários por tipo de ensino
		if ( !empty( $tipoEnsino ) ) {
			if ($tipoEnsino == 1){
				$funid = '12';	
			}elseif ($tipoEnsino == 2){
				$funid = '11';
			}
			array_push( $restricao, " (o.orgid = '" . $tipoEnsino ."' OR f.funid in ('" . $funid ."')) " );
		}
		
		# restringe os usuários por unidade orçamentária
		if ( !empty( $uo ) && !in_array( "", $uo ) ) {
			array_push( $restricao, " unicod in ( '". implode( "','", $uo ) ."' ) " );
		}
		# restringe os usuários por unidade gestora
		if ( !empty( $ug ) ) {
			array_push( $restricao, " ungcod = '" . $ug ."' " );
		}
		# restringe os usuários por cargo
		if ( !empty( $cargo ) ) {
			array_push( $restricao, " u.carid = '" . $cargo ."' " );
		}
		
		# restringe os usuários por perfil
		$join_perfil = "";
		if ( !empty( $perfis ) && !in_array( "", $perfis ) ) {
			$esquema = $_SESSION["sisdiretorio"];
			$join_perfil = sprintf(
				" inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf " .
				" inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = s.sisid and p.pflstatus = 'A' " .
				" and ( (
					p.pflresponsabilidade is not null and exists ( select pflcod from %s.usuarioresponsabilidade ur where ur.usucpf = u.usucpf and ur.pflcod = p.pflcod )
				) or (
					p.pflresponsabilidade is null
				) or p.pflresponsabilidade = 'N')",
				$esquema,
				$esquema,
				$esquema
			);
			
			$stPerfis = implode( ",", $perfis );
			
			//$stWherePerfil = " and (p.pflcod in ( $stPerfis ) or us.pflcod in ( $stPerfis ))";
			$stWherePerfil = " and (p.pflcod in ( $stPerfis ))";
		}
		
		$joinMunTipoMunicipio = "";
		# restringe os usuários por tipo Grupo PAC
		if( !empty( $tpmid ) && !in_array( "", $tpmid ) )
		{
			$tpmid = implode(',', $tpmid);
			$joinMunTipoMunicipio = " inner join territorios.muntipomunicipio mtmn on mtmn.muncod = m.muncod and mtmn.tpmid in (".$tpmid.") ";
		} 
		
		$joinEstadoObra = "";
		# restringe os usuários por Estado da Escola
		if( !empty( $esdid ) )
		{
			$joinEstadoEscola = "--INNER JOIN entidade.endereco ende ON ende.muncod = m.muncod
								 INNER JOIN minc.usuarioresponsabilidade usur ON (u.usucpf = usur.usucpf)
								 INNER JOIN minc.mcemaiscultura mce on mce.entid = usur.entid 
								 INNER JOIN workflow.documento doc on doc.docid = mce.docid and doc.esdid = ".$esdid." ";
		} 
		
		if ( empty( $restricao ) && empty( $join_perfil ) && $outros ){
			$destinatarios = array();
		}
		else {
			if ( empty( $restricao ) ) {
				array_push( $restricao, " 1 = 1 " );
			}
			
			$joinIdeb = $ideb ? " inner join territorios.muntipomunicipio mtm on mtm.muncod = ur.muncod " : "";
			$clausulaIdeb = $ideb ?  " and mtm.tpmid in ( '". implode("', '", $ideb ) ."' ) " : "";
			
			$innerComplemento = "";

			if( $_SESSION['sisarquivo'] == 'cte' ) $innerComplemento = " left join cte.usuarioresponsabilidade ur on ur.usucpf = u.usucpf "; 
			
			if(!empty($tipoEnsino) ){
				$innerComplemento.= " left join academico.usuarioresponsabilidade ur on (u.usucpf = ur.usucpf )
									  left join academico.orgao o on (ur.orgid = o.orgid)
									  left join entidade.entidade e on ur.entid = e.entid
									  left join entidade.funcaoentidade f on f.entid = e.entid ";
			}

			/*
			$whereUF = "";
			$innerUF = "";
			
			if( count( $arUF ) && !in_array( "", $arUF ) ){
				$innerUF = " inner join territorios.municipio mu on mu.muncod = u.muncod ";  
				$whereUF = " and mu.estuf in ( '". implode( "', '", $arUF ) ."' ) " ;  
			}
			*/			
			$whereMunicipios = "";
			
			if( count( $arMunicipios ) && !in_array( "", $arMunicipios ) ){
				$whereMunicipios = " and u.muncod in ( '". implode( "', '", $arMunicipios ) ."' ) " ;  
			}
			
//			if(!empty($_POST['ptiid']) && $_POST['iniusuario'] && !$_POST['inisomente']){
//				
//				$stWhereEmendas = "AND u.usucpf IN (select distinct
//														ed.edecpfresp														
//													from emenda.ptiniciativa pti
//													inner join emenda.iniciativa 			i   on pti.iniid = i.iniid
//													inner join emenda.iniciativadetalheentidade 	ede on i.iniid = ede.iniid
//													inner join emenda.emendadetalheentidade 	ed  on ed.edeid = ede.edeid
//													where i.iniid = {$_REQUEST['ptiid']})";
//				
//			}
			
			if(!$_POST['inisomente']){
			
				$sql = sprintf(
					"select distinct u.usunome, u.usuemail, u.usucpf, u.regcod, m.mundescricao
					from seguranca.usuario u
					inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
					inner join seguranca.sistema s on s.sisid = us.sisid
					inner join territorios.municipio m on (u.muncod = m.muncod)
					$joinMunTipoMunicipio
					$joinEstadoEscola
					$innerUF
					$innerComplemento 
					$joinIdeb 
					%s
					where $suscod s.sisid = %d 
					and s.sisstatus = 'A' 
					$usustatus
					and %s $clausulaIdeb
					$whereMunicipios
					$stWherePerfil									
					group by u.usunome, u.usuemail, u.usucpf, u.regcod, m.mundescricao",
					$join_perfil,
					$_SESSION["sisid"],
					implode( " and ", $restricao )
				);
// 				ver('1',$sql, d);
				$destinatarios = $db->carregar( $sql );
				$destinatarios = $destinatarios ? $destinatarios : array();
				sort($destinatarios);
			}

			
			
			/*
			 * FILTRO POR INICIATÍVA
			 */
			if(!empty($_POST['ptiid'])){
				
				$sql = "select distinct
							ed.edenomerep as usunome,
							ed.edemailresp as usuemail,
							ed.edecpfresp as usucpf,	
							u.regcod as regcod,
							m.mundescricao as mundescricao														
						from emenda.ptiniciativa pti
						inner join emenda.iniciativa 				i   on pti.iniid = i.iniid
						inner join emenda.iniciativadetalheentidade ede on i.iniid = ede.iniid
						inner join emenda.emendadetalheentidade 	ed  on ed.edeid = ede.edeid
						left join seguranca.usuario 				u   on u.usucpf = ed.edecpfresp
						left join territorios.municipio 			m   on u.muncod = m.muncod
						where i.iniid = {$_POST['ptiid']}
						and ed.edestatus = 'A'";
				//ver('2',$sql);
				
				$destinatariosIniciativa = $db->carregar( $sql );
				$destinatarios = $destinatarios ? $destinatarios : array();
				$destinatarios = array_merge($destinatariosIniciativa, $destinatarios);
//				$destinatarios = array_intersect($destinatariosIniciativa, $destinatarios);
				sort($destinatarios);
			}
			
			if ( !$destinatarios ) {
				$destinatarios = array();
			}
		}
		//die;
		return self::identificar_outros_destinatarios( $destinatarios, $outros );
	}
	
	public static function identificar_outros_destinatarios( array $destinatarios, $outros ) {
		global $db;
		foreach ( explode( ",", $outros ) as $item ) {
			if ( empty( $item ) ) {
				continue;
			}
			if ( strpos( $item, "<" ) ) {
				preg_match("/([\w\pL[:space:]]{0,})/", $item, $saida );
				$nome = strtoupper( $saida[0] );
				preg_match("/<(.*)>/", $item, $saida );
				$email = $saida[1];
			} else {
				$nome = "";
				$email = $item;
			}
			if ( empty( $nome ) ) {
				$nome = strtoupper( substr( $email, 0, strpos( $email, "@" ) ) );
			}
			$registro = array(
				"usucpf" => null,
				"usunome" => trim( $nome ),
				"usuemail" => trim( $email )
			);
			foreach ( $destinatarios as $destinatario ) {
				if ( $destinatario["usuemail"] == $registro["usuemail"] ) {
					continue 2;
				}
			}
			$sql = sprintf( "select usucpf from seguranca.usuario where usuemail = '%s'", $registro["usuemail"] );
			$registro["usucpf"] = $db->pegaUm( $sql );
			array_push( $destinatarios, $registro );
		}
		return $destinatarios;
	}
	
}
//fim copia da função que identifica destinatario para enviar e-mail
//Verifica Campos antes de Enviar Avaliação para Secretaria
function enviarAvaliacao(){


	global $db;

	$sqlDadosEscola = "SELECT e.entnumcpfcnpj, e.entcodent, e.entnome, e.entrazaosocial, e.entemail, e.entnumdddcomercial, 
					  e.entnumcomercial,e.tpcid, e.tpsid, a.endcep, a.endlog, a.endnum, a.endbai, a.estuf, a.muncod 
					  FROM entidade.entidade e
					  INNER JOIN minc.mcemaiscultura m 
					  ON e.entid = m.entid  
					  LEFT JOIN entidade.endereco a
					  ON e.entid = a.entid
					  WHERE mceid = {$_SESSION['minc']['mceid']}";
	$rsDadosEscola = $db->pegaLinha($sqlDadosEscola);
	
	if(empty($rsDadosEscola['entcodent']) || empty($rsDadosEscola['entnome'])  
	|| empty($rsDadosEscola['entemail']) || empty($rsDadosEscola['tpcid']) 
	|| empty($rsDadosEscola['tpsid']) || empty($rsDadosEscola['endcep']) || empty($rsDadosEscola['endlog']) || empty($rsDadosEscola['endnum']) || empty($rsDadosEscola['endbai']) 
	|| empty($rsDadosEscola['estuf']) || empty($rsDadosEscola['muncod']) ){
		return 'Verifique o preenchimento dos campos obrigatórios da aba Dados da Escola';
	}

	$sqlDiretor = "SELECT fun.entnumcpfcnpj, fun.entnome, fun.entemail, fun.entnumrg, fun.entorgaoexpedidor, fun.entsexo, fun.entdatanasc, fun.entnumdddresidencial, fun.entnumresidencial
				  FROM entidade.funentassoc f
				  INNER JOIN entidade.funcaoentidade fe using(fueid)
				  INNER JOIN entidade.entidade e ON e.entid = f.entid
				  INNER JOIN entidade.entidade fun ON fun.entid = fe.entid
				  WHERE f.entid = {$_SESSION['minc']['entid']} AND fe.funid = 19";
	$rsDiretor = $db->pegaLinha($sqlDiretor);

	if(empty($rsDiretor['entnumcpfcnpj']) || empty($rsDiretor['entnome']) || empty($rsDiretor['entemail']) || empty($rsDiretor['entorgaoexpedidor']) || empty($rsDiretor['entnumrg']) 
	|| empty($rsDiretor['entsexo']) || empty($rsDiretor['entdatanasc']) || empty($rsDiretor['entnumdddresidencial']) || empty($rsDiretor['entnumresidencial'])){ 
		return 'Verifique o preenchimento dos campos obrigatórios da aba Diretor';
	}	
	
	$sqlCoordenador = "SELECT fun.entnumcpfcnpj, fun.entnome, fun.entemail, fun.entnumrg, fun.entorgaoexpedidor, fun.entsexo, fun.entdatanasc, fun.entnumdddresidencial, fun.entnumresidencial
					  FROM entidade.funentassoc f
					  INNER JOIN entidade.funcaoentidade fe using(fueid)
					  INNER JOIN entidade.entidade e ON e.entid = f.entid
					  INNER JOIN entidade.entidade fun ON fun.entid = fe.entid
					  WHERE f.entid = {$_SESSION['minc']['entid']} AND fe.funid = 109";
	$rsCoordenador = $db->pegaLinha($sqlCoordenador);

	if(empty($rsCoordenador['entnumcpfcnpj']) || empty($rsCoordenador['entnome']) || empty($rsCoordenador['entemail']) || empty($rsCoordenador['entorgaoexpedidor'])  || empty($rsCoordenador['entnumrg']) 
	|| empty($rsCoordenador['entsexo']) || empty($rsCoordenador['entdatanasc']) || empty($rsCoordenador['entnumdddresidencial']) || empty($rsCoordenador['entnumresidencial'])){ 
		return 'Verifique o preenchimento dos campos obrigatórios da aba Coordenador';
	}	
	
	$sqlCultural = "SELECT ent.entnumcpfcnpj, ent.entnome, ent.entemail, ent.entnumrg, ent.entorgaoexpedidor, ent.entsexo, ent.entdatanasc, 
					ent.entnumdddresidencial, ent.entnumresidencial, ent.entnumdddcelular, ent.entnumcelular, mce.parhistoricoatuacao, anx.arqid,
					ent.entrazaosocial, ent.entnumdddcomercial, ent.entnumcomercial, ent.njuid, ent.tpcid
					FROM minc.mceparceiro mce
					INNER JOIN minc.mceanexo anx ON anx.parid = mce.parid
					LEFT JOIN entidade.entidade ent ON ent.entid = mce.entid 
					WHERE mceid = {$_SESSION['minc']['mceid']}
					AND mce.parstatus = 'A'
					AND anx.anestatus = 'A'";
	
	$rsCultural = $db->pegaLinha($sqlCultural);

	if(strlen($rsCultural['entnumcpfcnpj']) == 11){
		if(empty($rsCultural['entnumcpfcnpj']) || empty($rsCultural['entnome']) || empty($rsCultural['entemail']) || empty($rsCultural['entnumrg']) || empty($rsCultural['entorgaoexpedidor']) || empty($rsCultural['entsexo']) || empty($rsCultural['entdatanasc']) 
		|| empty($rsCultural['entnumdddresidencial']) || empty($rsCultural['entnumresidencial']) || empty($rsCultural['entnumdddcelular']) || empty($rsCultural['entnumcelular']) || empty($rsCultural['parhistoricoatuacao']) || empty($rsCultural['arqid'])){ 
			return 'Verifique o preenchimento dos campos obrigatórios da aba Iniciativa Cultural Parceria';
		}
	} else {
		if(empty($rsCultural['entnumcpfcnpj']) || empty($rsCultural['entnome']) || empty($rsCultural['entrazaosocial']) || empty($rsCultural['entemail']) || empty($rsCultural['entnumdddcomercial']) || empty($rsCultural['entnumcomercial']) 
		|| empty($rsCultural['njuid']) || empty($rsCultural['tpcid']) || empty($rsCultural['parhistoricoatuacao']) || empty($rsCultural['arqid'])){ 
			return 'Verifique o preenchimento dos campos obrigatórios da aba Iniciativa Cultural Parceria';
		}
	}

	$sqlPlano = "SELECT ptrid, ptrobjetivogeral, ptrjustificativa, ptrresultadosesperados, ptrquantidadealuno, ptrquantidadeprofessor, ptrquantidadepessoa, ptrdescricaproduto, 
       			ptrquantidadefamiliar, ptrproduto, descdesenvolvido, desccomunidadeescolar, desccomunidadelocal, descrelprojetoescola, descoqueseradesenv, descelemtodialogo 
  				FROM minc.mceplanotrabalho 
  				WHERE mceid = {$_SESSION['minc']['mceid']}";
	
	$rsPlano = $db->pegaLinha($sqlPlano);
	
	if($rsPlano['ptrproduto'] == 'f'){
		if(empty($rsPlano['ptrobjetivogeral']) || empty($rsPlano['ptrjustificativa']) || empty($rsPlano['ptrresultadosesperados']) || empty($rsPlano['ptrquantidadealuno']) || empty($rsPlano['ptrquantidadeprofessor']) || empty($rsPlano['ptrquantidadepessoa']) 	|| empty($rsPlano['ptrquantidadefamiliar']) 
		|| empty($rsPlano['ptrproduto']) || empty($rsPlano['descdesenvolvido']) || empty($rsPlano['desccomunidadeescolar']) || empty($rsPlano['descrelprojetoescola']) || empty($rsPlano['descoqueseradesenv']) ) {
			return 'Verifique o preenchimento dos campos obrigatórios da aba Plano de Atividade Cultural da Escola 2013';
		}
	} else {
		if(empty($rsPlano['ptrobjetivogeral']) || empty($rsPlano['ptrjustificativa']) || empty($rsPlano['ptrresultadosesperados']) || empty($rsPlano['ptrquantidadealuno']) || empty($rsPlano['ptrquantidadeprofessor']) || empty($rsPlano['ptrquantidadepessoa']) 	|| empty($rsPlano['ptrquantidadefamiliar']) 
		|| empty($rsPlano['ptrdescricaproduto']) || empty($rsPlano['ptrproduto']) || empty($rsPlano['descdesenvolvido']) || empty($rsPlano['desccomunidadeescolar']) || empty($rsPlano['descrelprojetoescola']) || empty($rsPlano['descoqueseradesenv']) ) {
			return 'Verifique o preenchimento dos campos obrigatórios da aba Plano de Atividade Cultural da Escola 2013';
		}		
	}

	$sqlPrevisao = "SELECT promaterialconsumo, procontservcultural, procontservdiverso, prolocacao, promaterialpermanente
  				    FROM minc.mceprevisaoorcamentaria 
  				    WHERE ptrid = {$rsPlano['ptrid']}";
	$rsPrevisao = $db->pegaLinha($sqlPrevisao);

	if(empty($rsPrevisao['promaterialconsumo']) || empty($rsPrevisao['procontservcultural']) || empty($rsPrevisao['procontservdiverso']) || empty($rsPrevisao['prolocacao']) || empty($rsPrevisao['promaterialpermanente'])){
		return 'Verifique o preenchimento dos campos obrigatórios da aba Plano de Atividade Cultural da Escola 2013';
	}
	
	$sqlTermo = "SELECT paraceite
  				FROM minc.mceparceiro
  				WHERE mceid = {$_SESSION['minc']['mceid']}";
	$rsTermo = $db->pegaLinha($sqlTermo);
	
	if($rsTermo['paraceite'] != 't'){
		return 'Verifique o preenchimento dos campos obrigatórios da aba Termo de Parceria';
	}
	
	return true;
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
								  AND p.sisid = ".SISID_MAIS_CULTURA."
			WHERE
				pu.usucpf = '".$_SESSION['usucpf']."'
			ORDER BY
				p.pflnivel";
		$pflcod = $db->carregarColuna($sql);
	
	/*** Retorna o array com o(s) perfil(is) ***/
	return (array)$pflcod;
}


function boExisteEntidade( $entid ){
	global $db;
	$entidade = "";
	
	if($entid){
		$entidade = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entid = {$entid}");
	}	
	
	return $entidade;
}

/*
* Função para montar o cabeçalho usado nas páginas do 'Mais Educação'
*/
function cabecalho() {

	global $db;
	
	$entid = $_SESSION['minc']['entid'];
	
	$sql = "SELECT DISTINCT
				est.estdescricao as est,
				est.estuf,
				mun.mundescricao as mun,
				ent.entnome as esc
			FROM
				entidade.entidade ent 
			LEFT JOIN 
				entidade.endereco ende ON ent.entid = ende.entid
			INNER JOIN 
				territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN 
				territorios.estado est ON est.estuf = mun.estuf		
			WHERE
				--ent.funid = 3 and
			  	--ent.tpcid IN (1,3) AND
		    	ent.entid IN ('{$entid}')";
	//xx($sql);
	$dados = $db->carregar($sql);
	
	$cab = "<table align=\"center\" class=\"Tabela\">
			 <tbody>
			 	<tr>
			 		<td colspan=\"2\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\">
			 			<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(".$_SESSION['minc']['entid'].");\" ><img style=\"vertical-align:middle;\" src=\"/imagens/globo_terrestre.png\" border=\"0\" title=\"Exibir Mapa\"> Portal CulturaEduca</a>
			 		</td>
			 	</tr>
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

function tratarStrBusca($str){
	$str = explode(" ",$str);
	foreach ($str as $str):
	$text .= strlen($str) >= 3 ? "%".$str : '';
	endforeach;

	return $text."%";
}

/**
 * Função que monta as abas do 'Mais Educação'
 *
 * @return array
 * 
 * Since: 13/04/2009
 */
function carregaAbas() {
	global $db;

	if(!$_SESSION['minc']['entid']){

		$menu = array(
				  	 0 => array("id" => 1, "descricao" => "Lista", "link" => "/minc/minc.php?modulo=principal/lista&acao=A")
				  	 );
		
		
	} else {
		
		$menu = array(
			  0 => array("id" => 1, "descricao" => "Lista", "link" => "/minc/minc.php?modulo=principal/lista&acao=A"),
			  1 => array("id" => 2, "descricao" => "Dados da Escola", "link" => "/minc/minc.php?modulo=principal/dados_escola&acao=A"),
			  2 => array("id" => 3, "descricao" => "Diretor", "link" => "/minc/minc.php?modulo=principal/cadastro_diretor_coordenador&tipo=diretor&acao=A"),
			  3 => array("id" => 4, "descricao" => "Coordenador", "link" => "/minc/minc.php?modulo=principal/cadastro_diretor_coordenador&tipo=coordenador&acao=A"),
			  4 => array("id" => 5, "descricao" => "Plano de Trabalho " . $_SESSION["exercicio"], "link" => "/minc/minc.php?modulo=principal/plano_trabalho&acao=A")
		);		
		
							  
		//coloca a ultima aba
		$menu2 = array("id" => count($menu)+1, "descricao" => "Parceiros", "link" => "/minc/minc.php?modulo=principal/parceiros&acao=A");
		array_push($menu,$menu2);
							  
	}		
	
	$menu = $menu ? $menu : array();
	
	return $menu;
	
}


function melista(){

	global $db;
	
	$ano = $_SESSION["exercicio"];
	$anoAnterior = $ano -1;
	$anoAnt = $anoAnterior -1;
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
	elseif($_SESSION['maiseducacao']['filtro']['estuf'])
		$where[] = " m.estuf = '".$_SESSION['maiseducacao']['filtro']['estuf']."'";

	if($_POST['muncod'])
		$where[] = " m.muncod = '".$_POST['muncod']."'";
	elseif($_SESSION['maiseducacao']['filtro']['muncod'])
		$where[] = " m.muncod = '".$_SESSION['maiseducacao']['filtro']['muncod']."'";
		
	if ($_REQUEST['esdid'] == '0'){
		$_REQUEST['esdid'] = "naoiniciado";
	}
 	if ($_REQUEST['esdid']) {
		$naoIniciado = "";
		
		if( $_REQUEST['esdid'] != "naoiniciado" && $_REQUEST['esdid'] != "9999999" )
			$where[] = " est.esdid = '".$_REQUEST['esdid']."'";
		else
			$naoIniciado = " maedu.docid is null ";
	} 
/*	if($_POST['escolasPBF']){
// 		ver($_POST['escolasPBF']);
		$where[] = " maedu.mcemaioriapbf = 't' ";
	}*/
	//filtro Maioria PBF? 
    if($_POST['escolasPBF'] == 'T') {
        $where[] = " maedu.mcemaioriapbf = 't' ";
    }
    if( $_POST['escolasPBF'] == "F") {
        $where[] = " maedu.mcemaioriapbf = 'f' ";
    }



	$innerMonitora = '';
	if($_POST['monitoramento']){
		if($_POST['monitoramento'] == 'T') {
			$innerMonitora = "INNER JOIN minc.monitoramento mon ON mon.mceid = maedu.mceid AND mon.moninicioupreenchimento = 't'";
		}
		if( $_POST['monitoramento'] == "F") {
			$innerMonitora = "INNER JOIN minc.monitoramento mon ON mon.mceid = maedu.mceid AND mon.moninicioupreenchimento = 'f'";
		}
	}
	
	/*	
	if ( $_POST['usuativo'] ){
		$where1 = "WHERE ativo = 'Sim'"; 
	}elseif ( isset($_POST['usuativo']) ){
		$where1 = "WHERE ativo = 'Não'"; 
	}
	*/	
    if ($_REQUEST['avaliacao']) {
        switch ($_REQUEST['avaliacao']) {
             case 'a':
                 $where[] = " ( (aval.avainiciativacultural = '1'  AND aval.avaplanoatividade = '2') or (aval.avainiciativacultural = '2'  AND aval.avaplanoatividade = '1')or(aval.avainiciativacultural = '2'  AND aval.avaplanoatividade = '2'))";
                 break;
             case 'b':
                 $where[] = " ( (aval.avainiciativacultural = '1'  AND aval.avaplanoatividade = '1') or (aval.avainiciativacultural = '2'  AND aval.avaplanoatividade = '0')or (aval.avainiciativacultural = '0'  AND aval.avaplanoatividade = '2')) ";
                 break;
             case 'c':
                 $where[] = " ( (aval.avainiciativacultural = '1'  AND aval.avaplanoatividade = '0') or (aval.avainiciativacultural = '0'  AND aval.avaplanoatividade = '1')or(aval.avainiciativacultural = '0'  AND aval.avaplanoatividade = '0')) ";
                 break;
         }
     }

     if ($_REQUEST['avaliacao_pontos']) {
        switch ($_REQUEST['avaliacao_pontos']) {
            case '0x':
                $where[] = "aval.avainiciativacultural = '0'  AND aval.avaplanoatividade = '0'";
                break;
            case '1':
                $where[] = "( (aval.avainiciativacultural = '1'  AND aval.avaplanoatividade = '0') or (aval.avainiciativacultural = '0'  AND aval.avaplanoatividade = '1'))";
                break;
            case '2':
                $where[] = "( (aval.avainiciativacultural = '1'  AND aval.avaplanoatividade = '1') or (aval.avainiciativacultural = '2'  AND aval.avaplanoatividade = '0')or (aval.avainiciativacultural = '0'  AND aval.avaplanoatividade = '2'))";
                break;
            case '3':
                $where[] = "( (aval.avainiciativacultural = '1'  AND aval.avaplanoatividade = '2') or (aval.avainiciativacultural = '2'  AND aval.avaplanoatividade = '1')";
                break;
            case '4':
                $where[] = " aval.avainiciativacultural = '2'  AND aval.avaplanoatividade = '2'";
                break;
        }
    }

    if( $_REQUEST['modalidade'] == 'F') {
		$where[] = " maedu.mcemodalidadeensino = 'F' ";
	}
	else if( $_REQUEST['modalidade'] == 'M') {
		$where[] = " maedu.mcemodalidadeensino = 'M' ";
	}
	
	if( $_REQUEST['classificacao'] == 'U') {
		$where[] = " maedu.mceclassificacaoescola = 'U' ";
	}
	else if( $_REQUEST['classificacao'] == 'R') {
		$where[] = " maedu.mceclassificacaoescola = 'R' ";
	}
	
	if(checkPerfil(PERFIL_MINC_SEC_ESTADUAL, false) && !checkPerfil(array(PERFIL_MINC_SUPER_USUARIO,PERFIL_MINC_ADMINISTRADOR), false)){
		//$where[] = "e.tpcid = 1";
	}else 
	if(checkPerfil(PERFIL_MINC_SEC_MUNICIPAL, false) && !checkPerfil(array(PERFIL_MINC_SUPER_USUARIO,PERFIL_MINC_ADMINISTRADOR), false)){
		//$where[] = "e.tpcid = 2";
	}else 
	if($_POST['tpcid']){
		$where[] = " e.tpcid IN (".$_POST['tpcid'].")";
	}else
	if($_SESSION['maiseducacao']['filtro']['tpcid']){
		$where[] = " e.tpcid IN (".$_SESSION['maiseducacao']['filtro']['tpcid'].")";
	}
	
	if($_REQUEST['tipoescola']){
		switch ($_REQUEST['tipoescola']){
			case 'proemi':
				$where[] = "mceproemi = 't'";
				break;
			case 'pme':
				$where[] = "mcepme = 't'";
				break;
			case 'ambos':
				$where[] = "mceproemi = 't' and mcepme = 't'";
				break;
		}
	}
	if($_POST['avaculturaafro']){
		$where[] = "aval.avaculturaafro = 't'";
	}
	if($_POST['avaculturaindigena']){
		$where[] = "aval.avaculturaindigena = 't'";
	}
	if($_POST['avamusicanaescola']){
		$where[] = "aval.avamusicanaescola = 't'";
	}
    /* Filtro Tipo de Busca */
    $cpfcnpj = "CPF/CNPJ do Parceiro";//inicializando varíavel que define coluna CPF ou CNPJ de acordo com filtro
    if($_REQUEST['cpfcnpj']){
        switch ($_REQUEST['cpfcnpj']){
            case '1':
                $where1 = "WHERE length(foo.numcpfcnpj) = 11";
                $cpfcnpj = "CPF do Parceiro";
                break;
            case '2':
                $where1 = "WHERE length(foo.numcpfcnpj) > 11";
                $cpfcnpj = "CNPJ do Parceiro";
                break;
        }
    }

    $stInner = '';
	if($_REQUEST['eixo']){
		$stInner = '
				JOIN ( 
					select * from minc.mceplanotrabalho a
					join minc.mceplanoeixo b on a.ptrid = b.ptrid
				) ext ON ext.mceid = maedu.mceid AND ext.extid = '.$_REQUEST['eixo'].' 
				';
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
	if (    in_array(PERFIL_MINC_SUPER_USUARIO, $perfil) 
		 || in_array(PERFIL_MINC_ADMINISTRADOR, $perfil)  
		) {
		$from = "";
    } else {
    	if ( in_array(PERFIL_MINC_SEC_ESTADUAL, $perfil) && in_array(PERFIL_MINC_SEC_MUNICIPAL, $perfil)){
			$from = " INNER JOIN minc.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
						 (ur.muncod = m.muncod AND 
						  e.tpcid = 3) OR
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)"; 
		} elseif ( in_array(PERFIL_MINC_SEC_ESTADUAL, $perfil)) { //Perfil PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO só ver na sua escola ESTADUAL
			$from = " INNER JOIN minc.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)";
		} elseif ( in_array(PERFIL_MINC_SEC_MUNICIPAL, $perfil)) { //Perfil PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO só ver na sua escola MUNICIPAL
			$from = " INNER JOIN minc.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
						 (ur.muncod = m.muncod AND 
						  e.tpcid = 3) OR
	 					 ur.entid  = e.entid
	 					)";
		}
    } 
	
    $mcestatus = ( !$_REQUEST['status'] ) ? "AND maedu.mcestatus = 'A'" : "AND maedu.mcestatus = 'I'";    
    $acao = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"minc.php?modulo=principal/informacoes&acao=A&mceid=' || maedu.mceid || '&entid=' || e.entid || '\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>'";
    
    $where[] = " maedu.mceanoreferencia = ".$_SESSION["exercicio"]." {$mcestatus} ";
    if($naoIniciado) $where[] = $naoIniciado;
    
    $sqlSaldoOrcamento = "";
    if($_POST['requisicao'] == 'excel'){
		$acao = "";

		$sqlSaldoOrcamento = "
			,
			(
			CASE WHEN foo.qtd_alunos <= 500 THEN
				to_char(20000::numeric, '999G999G999D99')
			WHEN foo.qtd_alunos >= 501 AND foo.qtd_alunos <= 1000 THEN
				to_char(21000::numeric, '999G999G999D99')
			WHEN foo.qtd_alunos > 1000 THEN
				to_char(22000::numeric, '999G999G999D99')
			ELSE
				to_char(0::numeric, '999G999G999D99')
			END
			) AS saldo_orcamento
		";

		$campoQtdAlunos = " (
								SELECT
									count(qtd_alunos_m.fk_cod_aluno) as qtd_alunos
								FROM
									educacenso_2013.tab_dado_escola qtd_alunos_e
								JOIN educacenso_2013.tab_matricula qtd_alunos_m ON qtd_alunos_e.fk_cod_entidade = qtd_alunos_m.fk_cod_entidade
								JOIN entidade.entidade qtd_alunos_ee on qtd_alunos_ee.entcodent::integer = qtd_alunos_e.fk_cod_entidade
								WHERE
									qtd_alunos_ee.entid = e.entid
								LIMIT 1
							) AS qtd_alunos ";
	} else {
		$acao = " {$acao} as acao, ";

		$campoQtdAlunos = " 0 AS qtd_alunos ";
	}

	$sql = sprintf("
            SELECT
            ".(!empty($acao)?'foo.acao,':'')."
            foo.entcodent,
            foo.entnome,
            foo.numcpfcnpj,
            foo.tipo,
            foo.estuf,
            foo.mundescricao,
            foo.situacao,
            foo.tipo_ensino
            $sqlSaldoOrcamento
            FROM(
						SELECT DISTINCT
							 $acao
							 e.entcodent,
							 e.entnome,
							 (SELECT distinct entnumcpfcnpj FROM minc.mceparceiro mce LEFT JOIN entidade.entidade ent ON ent.entid = mce.entid WHERE parstatus='A' and mceid = maedu.mceid limit 1) as numcpfcnpj,

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
							 CASE WHEN maedu.mcemodalidadeensino = 'M' THEN 'Médio' ELSE 'Fundamental' END AS tipo_ensino,
							 --CASE WHEN maedu.memadesaopst = 'S' THEN 'Sim'
							 --WHEN maedu.memadesaopst = 'N' THEN 'Não'							 
							 --ELSE '-' END AS pst
							 --'' as pst,
							{$campoQtdAlunos}
						FROM
							 minc.mcemaiscultura maedu 
					    INNER JOIN
							 entidade.entidade e ON e.entid = maedu.entid AND e.entstatus = 'A'
                        INNER JOIN
                        	 entidade.funcaoentidade fe ON fe.entid = e.entid AND fe.fuestatus = 'A'
                        INNER JOIN
                             entidade.funcao f ON f.funid = fe.funid AND f.funstatus = 'A'
						INNER JOIN 
							 entidade.endereco endi ON endi.entid = e.entid AND endi.endstatus = 'A'
						LEFT JOIN
							minc.avaliacaoplanoescola aval ON aval.mceid = maedu.mceid
						{$stInner}
						{$innerMonitora}
						LEFT JOIN 
							 territorios.municipio m ON m.muncod = endi.muncod
						LEFT JOIN	
							 minc.usuarioresponsabilidade ur1 ON ur1.entid = e.entid AND ur1.rpustatus = 'A' AND ur1.pflcod = 383
							 %s
						LEFT JOIN 
							 workflow.documento d ON d.docid = maedu.docid
						INNER JOIN 
							 workflow.estadodocumento est ON est.esdid = d.esdid
							 %s %s ) as foo 
						%s",
				$from,
				$where ? " WHERE ".implode(' AND ', $where)." " : ' ',
				$and,
				$where1 ? $where1 : '');
	//ver($sql,d);
	if($_POST['requisicao'] == 'excel'){
		ini_set("memory_limit", "1024M");
		$cabecalho = array("Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Ensino", "Total Orçamento(R$)");
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatME".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatME".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000000,5,'N','100%', 'S');
		exit;
	} else {
// ver($sql,d);
		$cabecalho = array( "Ação", "Cód", "Escola", $cpfcnpj,"Tipo", "UF", "Município", "Situação", "Ensino");
		$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', '', '');
	}
	
	
}


function existeDiretorCoordenadorPorCpf($funid){ // Função feita para atender necessidade do Cliente com urgência
	global $db;
	
	# Comentado por causa das modificações da entidade
	/*$sql = "SELECT mep.entid FROM pdeescola.memaiseducacao mee
			  inner join pdeescola.mepessoal mep on mee.memid = mep.memid
		      inner join entidade.entidade e on mep.entid = e.entid
		      inner join entidade.funcaoentidade fe on e.entid = fe.entid
			where mee.entid = $entid and fe.funid = $funid and mep.mepstatus = 'A' and fe.fuestatus = 'A' ";*/
	
	$entid = $_SESSION['minc']['entid'];
	
	/*
	 * Correção por Alexandre Dourado 17/11/09
	 */
	if(!$entid) {
		echo "<script>
				alert('Entidade não encontrada. Refaça o procedimento.');
				window.location='minc.php?modulo=principal/lista&acao=A';
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

// INICIO FUNÇÕES DO WORKFLOW

function criaDocumento( $slcid ) {
	
	global $db;
	
	if(empty($slcid)) return false;
	
	$docid = pegaDocid( $slcid );
	
	if( !$docid ){
				
		$tpdid = WF_TPDID_SIC;
		
		$docdsc = "Cadastramento sistema de informação ao cidadão";
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
		
		if($slcid) {
			$sql = "UPDATE sic.solicitacao SET 
					 docid = ".$docid." 
					WHERE
					 slcid = ".$slcid;

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

function pegaDocid( $slcid ) {
	
	global $db;
	
	$slcid = (integer) $slcid;	
	
	$sql = "SELECT
			 docid
			FROM
			 sic.solicitacao
			WHERE
			 slcid  = " . $slcid;
	
	return (integer) $db->pegaUm( $sql );
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

// INICIO FUNÇÕES DE PERFIL

function checkPerfil( $pflcods, $superuser = true){

	global $db;

	if ($db->testa_superuser() && $superuser) {

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

function listaEscolas()
{
	global $db;
	$ano = $_SESSION["exercicio"];
	$anoAnterior = $ano -1;
	$anoAnt = $anoAnterior -1;

	$anoMaximo = $db->pegaUm("select max(prsano) from pdeescola.programacaoexercicio");
	if(!$_GET['memanoreferencia']) $_GET['memanoreferencia'] = $ano;
	
	if ($_POST['escola'])
		$where[] = " UPPER(e.entnome) LIKE UPPER('".tratarStrBusca($_POST['escola'])."')";

	if ($_POST['entcodent'])
		$where[] = " e.entcodent LIKE '%".$_POST['entcodent']."%'";

	if ($_REQUEST['estuf'])
		$where[] = " m.estuf = '".$_REQUEST['estuf']."'";
	elseif($_SESSION['maiseducacao']['filtro']['estuf'])
	$where[] = " m.estuf = '".$_SESSION['maiseducacao']['filtro']['estuf']."'";

	if($_POST['muncod'])
		$where[] = " m.muncod = '".$_POST['muncod']."'";
	elseif($_SESSION['maiseducacao']['filtro']['muncod'])
	$where[] = " m.muncod = '".$_SESSION['maiseducacao']['filtro']['muncod']."'";

	if ($_REQUEST['esdid'] == '0'){
		$_REQUEST['esdid'] = "naoiniciado";
	}
	if ($_REQUEST['esdid']) {
		$naoIniciado = "";

		if($_REQUEST['esdid'] != "naoiniciado")
			$where[] = " est.esdid = '".$_REQUEST['esdid']."'";
		else
			$naoIniciado = "maedu.docid is null and";
	}

	if($_POST['tpcid'])
		$where[] = " e.tpcid IN (".$_POST['tpcid'].")";
	elseif($_SESSION['maiseducacao']['filtro']['tpcid'])
	$where[] = " e.tpcid IN (".$_SESSION['maiseducacao']['filtro']['tpcid'].")";
	
	if ( $_POST['usuativo'] ){
		$where1 = "WHERE ativo = 'Sim'";
	}elseif ( isset($_POST['usuativo']) ){
		$where1 = "WHERE ativo = 'Não'";
	}

	if( $_REQUEST['modalidade'] == 'F') {
		$where[] = " maedu.memmodalidadeensino = 'F' ";
	}
	else if( $_REQUEST['modalidade'] == 'M') {
		$where[] = " maedu.memmodalidadeensino = 'M' ";
	}

	if( $_REQUEST['classificacao'] == 'U') {
		$where[] = " maedu.memclassificacaoescola = 'U' ";
	}
	else if( $_REQUEST['classificacao'] == 'R') {
		$where[] = " maedu.memclassificacaoescola = 'R' ";
	}
	else if( $_REQUEST['classificacao'] == 'A') {
		$where[] = " maedu.mamescolaaberta = 't' ";
	}else if( $_REQUEST['classificacao'] == 'J') {
		$where[] = " maedu.memjovem1517 = 't' ";
	}

	if( $_REQUEST['aderiupst'] == 'S' ) {

		$where[] = " maedu.memadesaopst = 'S' ";
	}
	elseif( $_REQUEST['aderiupst'] == 'N' ) {

		$where[] = " maedu.memadesaopst = 'N' ";
	}
	elseif( $_REQUEST['aderiupst'] == 'null' ) {

		$where[] = " maedu.memadesaopst is null ";
	}

	if ( $_REQUEST['anoanterior'] == 1){
		$where[] = " maedu.entcodent in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')";
	}	
	if( $_REQUEST['escolasAnexo'] == 1) {
		$where[] = " aqb.arqid is not null ";
	}
	if( $_REQUEST['escolasPBF'] == 1) {
		$where[] = " maedu.memmaioriapbf = 't' ";
	}

	$perfil = arrayPerfil();

	$from = "";
	if (    in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil)
	|| in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfil)
	|| in_array(PDEESC_PERFIL_CONSULTA, $perfil)
	) {
		$from = "";
	} else {
		if ( in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $perfil) && in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $perfil)){
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
		} elseif ( in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $perfil)) { //Perfil PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO só ver na sua escola ESTADUAL
			$from = " INNER JOIN pdeescola.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND
						ur.pflcod IN (".implode(',',$perfil).") AND
						ur.usucpf = '".$_SESSION['usucpf']."' AND
						(
	 					 ur.entid  = e.entid OR
	 					 (ur.estuf  = m.estuf AND
	 					  e.tpcid = 1)
	 					)";
		} elseif ( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $perfil)) { //Perfil PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO só ver na sua escola MUNICIPAL
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


	$memStatus = ( !$_REQUEST['status'] ) ? "AND maedu.memstatus = 'A'" : "AND maedu.memstatus = 'I'";

	if( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfil) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $perfil) )
	{
		$acao = "CASE WHEN maedu.memstatus = 'A'
    			 THEN '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaME(\'meajax.php\', \'tipo=redirecioname&entid=' || e.entid || '&memid=' || maedu.memid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"inativarEscola(' || maedu.memid || ')\"><img src=\"/imagens/valida6.gif\" border=0 title=\"Inativar Escola\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(' || e.entid || ');\" ><img src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'
    			 ELSE
    			 	  '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaME(\'meajax.php\', \'tipo=redirecioname&entid=' || e.entid || '&memid=' || maedu.memid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"ativarEscola(' || maedu.memid || ')\"><img src=\"/imagens/valida1.gif\" border=0 title=\"Ativar Escola\"></a>
    			 	   <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(' || e.entid || ');\"><img src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'
    			 END";
	}
	else
	{
		$acao = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"redirecionaME(\'meajax.php\', \'tipo=redirecioname&entid=' || e.entid || '&memid=' || maedu.memid || '\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:popupMapa(' || e.entid || ');\" ><img src=\"/imagens/globo_terrestre.png\" border=0 title=\"Exibir Mapa\"></a>'";
	}

	if($_POST['requisicao'] == 'excel'){
		$acoes = "";
	} else {
		$acoes = " {$acao} as acao, ";
	}

	$sql = sprintf("SELECT * FROM(
						SELECT DISTINCT
							$acoes
							e.entcodent,
							e.entnome,
							CASE WHEN e.tpcid = 1 THEN 'Estadual'
							ELSE 'Municipal' END AS tipo,
							m.estuf,
							m.mundescricao,
							CASE WHEN est.esdid IS NOT NULL THEN est.esddsc
							ELSE 'Não Iniciado' END AS situacao,
							CASE WHEN ur1.entid is not null THEN 'Sim' ELSE 'Não' END as ativo,							
							CASE WHEN maedu.memmodalidadeensino = 'M' THEN 'Médio' ELSE 'Fundamental' END,
							CASE WHEN maedu.memadesaopst = 'S' THEN 'Sim'
							WHEN maedu.memadesaopst = 'N' THEN 'Não'
							ELSE '-' END AS pst
						FROM
							entidade.entidade e
						INNER JOIN
							entidade.endereco endi ON endi.entid = e.entid
						LEFT JOIN
							territorios.municipio m ON m.muncod = endi.muncod
						LEFT JOIN
							pdeescola.usuarioresponsabilidade ur1 ON ur1.entid = e.entid AND ur1.rpustatus = 'A' AND ur1.pflcod = 383
							%s
						INNER JOIN
							pdeescola.memaiseducacao maedu ON %s maedu.entid = e.entid AND maedu.memanoreferencia = ".$_GET['memanoreferencia']." {$memStatus}
						LEFT JOIN
							pdeescola.mearquivos aqb ON aqb.memid = maedu.memid
						LEFT JOIN
							public.arquivo arq ON arq.arqid = aqb.arqid
						LEFT JOIN
							workflow.documento d ON d.docid = maedu.docid
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
	if($_POST['requisicao'] == 'excel'){
		$cabecalho = array( "Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Ensino", "Aderiu PST");
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatME".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatME".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000000,5,'N','100%', 'S');
		exit;
	} else {
		$cabecalho = array( "Ação", "Cód", "Escola", "Tipo", "UF", "Município", "Situação", "Usuário Ativo", "Ensino", "Aderiu PST");
		$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', '', '', 3600);
	}
}

function verificaEncerramentoPlanoEscola()
{
	global $db;
	
	/*
	
	--Novos
	'24024902',	'31146111',	'23108932',	'23110600',	'23110287',	'23108932',	'24024902',	'31165760',
	'24055808',	'35278212',	'43094066',	'26076772',	'42015812',	'15114260',	'15114287',	'15519171',
	'15150437',	'15115313',	'15114465',	'15114295',	'15039250',	'33145873',	'41011155',	'35065997',
	'43299229',	'17006201',	'21260885',	'26101357'
	
	--Antigos
	'52036774','23166100','26126044','52116808','51063972','43069428','41020731','24050660',			
	'24024902','31146111','23108932','23110600','23110287','23108932','24024902','31165760',
	'24055808','35278212','29043816','43094066','15008487','29043816','26076772','15011356',
	'22048472','42015812','23155990','15114260','15114287','15519171','15150437','15115313',
	'15114465','15114295','43104886','15118037','15039250','31272051','15044378','35017346',
	'33145873','15558215','41011155','35065997','43299229','17006201','52036774','23166100',
	'26126044','52116808','51063972','43069428','41020731','24050660'
	
	*/	
	
	$arEscolasExcecao = array(
						
			'24024902','31146111','23108932','23110600','23110287','23108932','24024902','31165760',
			'24055808','35278212','29043816','43094066','15008487','29043816','26076772','15011356',
			'22048472','42015812','23155990','15114260','15114287','15519171','15150437','15115313',
			'15114465','15114295','43104886','15118037','15039250','31272051','15044378','35017346',
			'33145873','15558215','41011155','35065997','43299229','17006201','52036774','23166100',
			'26126044','52116808','51063972','43069428','41020731','24050660','24049441','33061335',
			'15044181','29173027','29173108','22006559','22006362','22006753','22124780','22006648',
			'33094136','24058840','33019886','35207068','33019886','35054800','15548902','31087262',
			'35900761','21068445','21068577','21260885','21067937','25072439','26111357','31044377',
			'23169001','22078703','35054800','25072110','26154366','51054566','35088365','35191942',
			'35089904','32057032','32055803','32057288','42076234','31044369','29041333','21053804',
			'22048030','33150982','33052123','33009317','13022210','22117717','22078258','26101357',
			'15040500'
	);
	
	$sql = "select 
				e.entcodent,
				d.docid, 
				d.esdid,
				d.hstid
			from minc.mcemaiscultura m
			left join entidade.entidade e on e.entid = m.entid
			left join workflow.documento d on m.docid = d.docid
			where m.mceid = {$_SESSION['minc']['mceid']}";

	$rs = $db->pegaLinha($sql);
	
	if( $rs['esdid'] == ESTADO_DOCUMENTO_CADASTRAMENTO || empty($rs['docid']) ){
		
		if( in_array($rs['entcodent'], $arEscolasExcecao) || !empty($rs['hstid']) ){
			return false;
		}
		
		return true;
		
	}
	
	return false;
}

function listaTramitaEscolas()
{
	global $db;
	
	/*
	 * Filtro
	* Escola, Código, Estado, Municipio, Tipo Avaliação
	*/
	$end = array();
	//filtro Nome da Escola
	if ($_POST['escola'])
		$end[] = " UPPER(e.entnome) LIKE UPPER('".tratarStrBusca(trim($_POST['escola']))."')";
	
	//filtro Código da Escola
	if ($_POST['entcodent'])
		$end[] = " e.entcodent LIKE '%".$_POST['entcodent']."%'";
		
	//filtro PBF
	if($_POST['escolasPBF']){
		$end[] = " maedu.mcemaioriapbf = 't' ";
	}
	
	//filtro Estado
	if ($_REQUEST['estuf'])
		$end[] = " m.estuf = '".$_REQUEST['estuf']."'";
	elseif($_SESSION['maiseducacao']['filtro']['estuf'])
	$end[] = " m.estuf = '".$_SESSION['maiseducacao']['filtro']['estuf']."'";
	
	//filtro Município
	if($_POST['muncod'])
		$end[] = " m.muncod = '".$_POST['muncod']."'";
	elseif($_SESSION['maiseducacao']['filtro']['muncod'])
		$end[] = " m.muncod = '".$_SESSION['maiseducacao']['filtro']['muncod']."'";
	
	//Filtro Avaliação
    if ($_REQUEST['avaliacao']) {
        switch ($_REQUEST['avaliacao']) {
             case 'a':
                 $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) = 4 ";
                 break;
             case 'b':
                 $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) BETWEEN 2 AND 3 ";
                 break;
             case 'c':
                 $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) BETWEEN 0 AND 1 ";
                 break;
         }
     }
     
    //filtro Avaliação por Pontos
    if ($_REQUEST['avaliacao_pontos']!= '') {
        switch ($_REQUEST['avaliacao_pontos']) {
            case '0':
                $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) = 0 ";
                break;
            case '1':
                $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) = 1 ";
                break;
            case '2':
                $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) = 2 ";
                break;
            case '3':
                $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) = 3 ";
                break;
            case '4':
                $end[] = " (aval.avainiciativacultural::int + avaplanoatividade::int) = 4 ";
                break;
        }
    }
	
	//sql monta a lista
	$sql = "SELECT DISTINCT	  
				         '<input type=\"checkbox\" class=\"docid\" name=\"docid[]\" id=\"docid\" value=\"' || maedu.docid || '\" />' as acao,
						 e.entcodent,
						 e.entnome,
						 CASE
						  WHEN e.tpcid = 1 THEN 'Estadual'
						  ELSE 'Municipal'
						 END AS tipo,
						 --Coluna PONTOS
						 --'<div align=\"left\">'|| aval.avainiciativacultural::int + avaplanoatividade::int ||'</div>',
						 --Coluna AVALIAÇÃO
						 --CASE
				         --  WHEN ((aval.avainiciativacultural::int + avaplanoatividade::int) = 4) THEN (
				         --     'a'
				         --  )
				         --  WHEN ((aval.avainiciativacultural::int + avaplanoatividade::int) BETWEEN 2 AND 3) THEN (
				         --     'b'
				         --  )
				         --  WHEN ((aval.avainiciativacultural::int + avaplanoatividade::int) BETWEEN 0 AND 1) THEN (
				         --  	  'c'
				         --  )
				         --Coluna PBF
				         --END as avaliacao,
				         --CASE
						 -- WHEN maedu.mcemaioriapbf = false THEN 'Não'
						 -- ELSE 'Sim'
						 --END AS mcemaioriapbf,
						 m.estuf, 
						 m.mundescricao,
						CASE WHEN maedu.mcemodalidadeensino = 'M' THEN 'Médio' ELSE 'Fundamental' END as modalidade	 
					FROM
						 minc.mcemaiscultura maedu
					INNER JOIN
						 entidade.entidade e ON e.entid = maedu.entid 
					INNER JOIN 
						 entidade.endereco endi ON endi.entid = e.entid 
					INNER JOIN
						minc.avaliacaoplanoescola aval ON aval.mceid = maedu.mceid 
					LEFT JOIN 
						territorios.municipio m ON m.muncod = endi.muncod 
					INNER JOIN 
						workflow.documento d ON d.docid = maedu.docid 
					WHERE  
						maedu.mceanoreferencia = {$_SESSION['exercicio']} 
					AND 
						maedu.mcestatus = 'A'
					AND
						d.esdid = ".ESTADO_DOCUMENTO_AVALIACAO." ".($end ? " 
					AND 
						".implode('	
					AND 
						', $end)." " : ' ')."
					ORDER BY
						m.estuf, m.mundescricao, entnome";
	
	$marcaTodos = "<input type=\"checkbox\" onclick=\"marcaTodos(); \" name=\"todos\" id=\"todos\" /> <input type=\"hidden\" name=\"requisicaotramit\" id=\"requisicaotramit\" value=\"tramitar\" >";
	$cabecalho = array( $marcaTodos, "Cód", "Escola", "Tipo", "UF", "Município","Ensino");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', 'formtramita', '', '', 3600);
}
function enviaemailparacorrecao(){
	
	global $db;
	
	$sqllocalidade = "
						SELECT DISTINCT
							usu.muncod as muncod,
							usu.regcod as estuf,
							e.tpcid as tipo,
							e.entnome as escola
						FROM seguranca.usuario usu
						INNER JOIN minc.usuarioresponsabilidade urp ON urp.usucpf = usu.usucpf
						INNER JOIN entidade.entidade e ON e.entid = urp.entid AND e.entstatus = 'A'
						WHERE
							urp.entid = {$_SESSION['minc']['entid']}
						AND 	urp.rpustatus = 'A'
						";
	$localidade = $db->pegaLinha($sqllocalidade);
	
	$sqldiretores = "select
						usu.usuemail as email_diretor,
					FROM seguranca.usuario usu
					INNER JOIN minc.usuarioresponsabilidade urp ON urp.usucpf = usu.usucpf
					INNER JOIN entidade.entidade e ON e.entid = urp.entid AND e.entstatus = 'A'
					WHERE
						urp.entid = {$_SESSION['minc']['entid']}
					AND 	urp.rpustatus = 'A'";
	$diretores = $db->carregarColuna($sqldiretores);
	
	$assunto = "MAIS CULTURA - X encaminhado para correção.";
	
	$conteudo = "Foi enviado para correção o X da escola {$localidade['escola']} que esta sobe sua reponsabilidade.";
	
	enviar_email( 'simec@mec.gov.br', $diretores, $assunto, $conteudo );
	               
	if($localidade['tipo'] == 3){
		$filtrolocalidade = "urp.muncod = {$localidade['muncod']}";
		$filroperfil = "pfu.pflcod = 781";
	}else{
		$filtro = "urp.estuf = {$localidade['estuf']}";
		$filroperfil = "pfu.pflcod = 782";
	}
	
	$sqlsecretarios = "
						SELECT
							usu.usuemail as email_secretario
						FROM
							seguranca.usuario usu
						INNER JOIN 	seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf
						inner join minc.usuarioresponsabilidade urp ON urp.usucpf = usu.usucpf
						WHERE
							$filtro
						AND $filroperfil
						AND urp.rpustatus = 'A'";
	$secretarios = $db->carregarColuna($sqlsecretarios);
	
	$assunto = "MAIS CULTURA - Plano de Atividade Cultural encaminhado para correção.";
	
	$conteudo = "Prezado(s) Dirigente(s) e Técnico(a)s da Secretaria Estadual/ Municipal de Educação:
					O Plano de Atividade Cultural enviado à Avaliação MinC/MEC pela Unidade Escolar {$localidade['escola']}, foi devolvido aos dirigentes e/ou técnicos da mesma para  alterações. 
					O apoio pedagógico e operacional da Secretaria Estadual/ Municipal na reelaboração é indispensável.
					Os Planos de Atividades Culturais reelaborados deverão ser devolvidos à Avaliação MinC/ MEC o mais breve possível.";
	
	enviar_email( 'simec@mec.gov.br', $secretarios, $assunto, $conteudo );
	
}

function enviaemailCorrigido(){

	global $db;

	$sqllocalidade = "
					SELECT DISTINCT
						usu.muncod as muncod,
						usu.regcod as estuf,
						e.tpcid as tipo,
						e.entnome as escola
					FROM seguranca.usuario usu
					INNER JOIN minc.usuarioresponsabilidade urp ON urp.usucpf = usu.usucpf
					INNER JOIN entidade.entidade e ON e.entid = urp.entid AND e.entstatus = 'A'
					WHERE
						urp.entid = {$_SESSION['minc']['entid']}
					AND 	urp.rpustatus = 'A'
					";
	$localidade = $db->pegaLinha($sqllocalidade);

	if($localidade['tipo'] == 3){
		$filtrolocalidade = "urp.muncod = {$localidade['muncod']}";
		$filroperfil = "pfu.pflcod = 781";
	}else{
		$filtro = "urp.estuf = {$localidade['estuf']}";
		$filroperfil = "pfu.pflcod = 782";
	}

	$sqlsecretarios = "
						SELECT
							usu.usuemail as email_secretario
						FROM
							seguranca.usuario usu
						INNER JOIN 	seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf
						inner join minc.usuarioresponsabilidade urp ON urp.usucpf = usu.usucpf
						WHERE
							$filtro
						AND $filroperfil
						AND urp.rpustatus = 'A'";
	$secretarios = $db->carregarColuna($sqlsecretarios);

	$assunto = "MAIS CULTURA - Plano de Atividade Cultural encaminhado corrigido.";

	$conteudo = "Prezados Dirigentes e Técnicos da Secretaria Estadual/ Municipal de Educação:
			     O Plano de Atividade Cultural da Unidade Escolar  {$localidade['escola']} foi reenviado à Avaliação MinC/MEC.";

	enviar_email( 'simec@mec.gov.br', $secretarios, $assunto, $conteudo );

}

function mascaraglobal($value, $mask) {
    $casasdec = explode(",", $mask);
    // Se possui casas decimais
    if ($casasdec[1])
        $value = sprintf("%01." . strlen($casasdec[1]) . "f", $value);

    $value = str_replace(array("."), array(""), $value);
    if (strlen($mask) > 0) {
        $masklen = -1;
        $valuelen = -1;
        while ($masklen >= -strlen($mask)) {
            if (-strlen($value) <= $valuelen) {
                if (substr($mask, $masklen, 1) == "#") {
                    $valueformatado = trim(substr($value, $valuelen, 1)) . $valueformatado;
                    $valuelen--;
                } else {
                    if (trim(substr($value, $valuelen, 1)) != "") {
                        $valueformatado = trim(substr($mask, $masklen, 1)) . $valueformatado;
                    }
                }
            }
            $masklen--;
        }
    }
    return $valueformatado;
}
?>