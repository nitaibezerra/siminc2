<?php

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
								  AND p.sisid = ".SISID_EMI."
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

// INICIO FUNÇÕES DO WORKFLOW

function criaDocumento( $entid, $emiid ) {
	
	global $db;
	
	if(!isset($_SESSION['emiid'])){
		return false;
	}
	
	if(empty($_SESSION['emiid'])){
		return false;
	}
	
	$sql = "select count(*) from em.emiprofissionalenvolvido where emiid = ".$_SESSION['emiid'];
	$rsProfEnvolvido = $db->pegaUm($sql);
	
	$sql = "select count(*) from em.emigap where emiid = ".$_SESSION['emiid'];
	$rsGap = $db->pegaUm($sql);
	
	$sql = "select etjid from em.emiensinomedioinovador where emiid = ".$_SESSION['emiid'];
	$rsJornada = $db->pegaUm($sql);
	
	$sql = "SELECT ent.* FROM entidade.entidade ent 
			LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
			LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			WHERE fen.funid='".ENTIDADE_FUNID_DIRETOR."' AND fea.entid='{$_SESSION['entid']}'";	
	$rsDiretor = $db->pegaLinha($sql);
	
	$sql = "select emiid from em.emiescolaturno where emiid = ".$_SESSION['emiid'];
	$rsTurno = $db->pegaUm($sql);
	
	if($rsProfEnvolvido == 0 && $rsGap == 0 && empty($rsJornada) && !$rsDiretor && !$rsTurno){
		return false;
	}
	
	if(!$entid || !$emiid) return false;
	
	$docid = pegaDocid($entid, $emiid);
	
	if( !$docid ){
				
		$tpdid = WF_TPDID_ENSINO_INOVADOR;
		
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
		
		$docdsc = "Cadastramento ensino inovador - " . $descricao;
		
		/*
		 * cria documento WORKFLOW
		 */
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );		
		
		if($emiid) {
			$sql = "UPDATE em.emiensinomedioinovador SET 
					 docid = ".$docid." 
					WHERE
					 emiid = ".$emiid;

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

function pegaDocid( $entid , $emiid ) {
	
	global $db;
	
	$entid = (integer) $entid;
	$emiid = (integer) $emiid;
	
	$sql = "SELECT
			 docid
			FROM
			 em.emiensinomedioinovador
			WHERE
			 entid  = " . $entid . " AND 
			 emiid = " . $emiid . " AND 
			 emistatus = 'A'";
	
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

function existeAssociacaoEntidde($entid, $funid){ 
	
	global $db;
	
	if(!$entid) {
		echo "<script>
				alert('Entidade não encontrada. Refaça o procedimento.');
				window.location='em.php?modulo=inicio&acao=A';
			  </script>";
		exit;
	}
	
	$sql = "SELECT 
				e.entnumcpfcnpj 
			FROM 
				entidade.entidade e  
			INNER JOIN 
				entidade.funcaoentidade fe on e.entid = fe.entid 
			INNER JOIN 
				entidade.funentassoc fea on fea.fueid = fe.fueid 
			WHERE 
				fea.entid = '".$entid."' 
			AND 
				fe.funid = '".$funid."'";
	
	$cpf = $db->pegaUm($sql);
	
	return $cpf;
}

function cabecalho($entid = null) {

	global $db;
	
	$entid = $entid ? $entid : $_SESSION['entid'];
	
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
			  	ent.tpcid IN (1,3) AND
		    	ent.entid IN ('{$entid}')";

	$dados = $db->carregar($sql);
	
	echo "<script type=\"text/javascript\">
		    function popupMapa(entid){
				window.open('em.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
			}
		 </script>";
	
	$cab = "<table align=\"center\" class=\"Tabela\">
			 <tbody>
			 	<tr>
			 		<td colspan=\"2\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\">
			 			<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"javascript:popupMapa({$entid})\" ><img style=\"vertical-align:middle;\" src=\"/imagens/globo_terrestre.png\" border=\"0\" title=\"Exibir Mapa\"> Georeferenciamento: Itinerário Educativo</a>
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

function montaCabecalhoPRC( $emiid = null, $mcpid = null, $papid = null, $form = true, $critica = false ){
	
	global $db;
	
	if(!$_SESSION['entid']){
		return false;
	}
	
	$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid'] );
	$esdid = pegaEstadoAtual( $docid );
	
	$ativo = 'N';
	if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
		$ativo = 'S';	
	}
	
	$sql = "select 
				sec.entnome 
			from 
				entidade.entidade ent
			inner join 
				entidade.endereco ede on ent.entid = ede.entid
			inner join 
				entidade.endereco sed on sed.estuf = ede.estuf
			inner join 
				entidade.entidade sec on sec.entid = sed.entid
			inner join 
				entidade.funcaoentidade fet ON sec.entid = fet.entid 
					and fet.funid = ".ENTIDADE_FUNID_SEC_ESTADUAL."
			where 
				ent.entid = {$_SESSION['entid']}";
	
	$secretariaEducacao = $db->pegaUm( $sql );
	
	$sql = "SELECT
				ee.entcodent || ' - ' || entnome as nome,
				ed.estuf as uf,
				mundescricao as municipio,
				etjjornada,
				em.etjid
			FROM
				entidade.entidade ee
			INNER JOIN
				em.emiensinomedioinovador em ON em.entid = ee.entid
			LEFT JOIN 
				em.emitipojornada tj ON tj.etjid = em.etjid
			INNER JOIN
				entidade.endereco ed ON ed.entid = ee.entid
			INNER JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			WHERE
				em.emiid = {$emiid}";
	
	$dadosEscola = $db->pegaLinha( $sql );
	
	// Recupera os turnos cadastrados para a escola
	$sql = "SELECT ettid FROM em.emiescolaturno WHERE emiid = ".$_SESSION['emiid'];
	$rsTurnosEscola = $db->carregar($sql);
	
	$arTurnosEscola = array();
	$rsTurnosEscola = $rsTurnosEscola ? $rsTurnosEscola : array();
	foreach($rsTurnosEscola as $turnoEscola){
		$arTurnosEscola[] = $turnoEscola['ettid'];
	}
	
	//Recupera todos os turnos
	$sql = "SELECT * FROM em.emitipoturno ORDER BY ettdescricao";
	$rsTurnos = $db->carregar($sql);
	
	$htmlTurnos = '';
	$rsTurnos = $rsTurnos ? $rsTurnos : array();
	foreach($rsTurnos as $turno){
		$htmlTurnos .= '<input type="checkbox" name="ettid[]" value="'.$turno['ettid'].'" '.(in_array($turno['ettid'], $arTurnosEscola) ? 'checked="checked"' : '').''.($ativo == 'N' ? 'disabled="disabled"' : '').'/>'.$turno['ettdescricao'].'&nbsp;';	
	}
	
	// Recupera todas as jornadas
	$arJornada = array(7=>'7 Horas', 5=>'5 Horas');
	
	$sql = "select 
				* 
			from 
				em.emitipojornada 
			order by 
				etjdescricao";	
	$rsJornada = $db->carregar($sql);
	$htmlComboJornada = '<select name="etjid" class="CampoEstilo" style="width: auto" '.($ativo == 'N' ? 'disabled="disabled"' : '').'><option value="">Selecione...</option>';			
	foreach($rsJornada as $jornada){				
		$htmlComboJornada .= '<option value="'.$jornada['etjid'].'" '.($dadosEscola['etjid'] == $jornada['etjid'] ? 'selected="selected"' : '').'>'.$jornada['etjdescricao'].'</option>';
	}
	$htmlComboJornada .= '</select>';
	
	$sql = "SELECT * FROM em.emicenso WHERE entid = '".$_SESSION['entid']."' order by emcserie asc";
	$rsCenso = $db->carregar($sql);
	
	$totalAluno = 0;
	$rsCenso = $rsCenso ? $rsCenso : array();		
	foreach($rsCenso as $censo){
		if($censo['emcserie'] == 31){
			$arCenso[] = 'Não seriada '.$censo['emcquantidadealunos']." alunos";
		}else 
		if($censo['emcserie'] == 36){
			$arCenso[] = 'Integrado não seriada '.$censo['emcquantidadealunos']." alunos";
		}else{
			$arCenso[] = $censo['emcserie']."ª série ".$censo['emcquantidadealunos']." alunos";			
		}
		$totalAluno += $censo['emcquantidadealunos'];
	}			
	$htmlCenso = count($arCenso) ? implode(', ',$arCenso).", Total de alunos: {$totalAluno}" : 'Sem alunos';
	
	// Repasse total
	if(!empty($dadosEscola['etjjornada'])){
		$sql = "SELECT * FROM em.emireccusteiocapital WHERE emrjornada = {$dadosEscola['etjjornada']} AND {$totalAluno} BETWEEN emrqtdinialunos AND emrqtdfinalunos;";
		$reccusteiocapital = $db->pegaLinha($sql);
	}
	$valorRepasse = !empty($reccusteiocapital['emrcusteio']) && !empty($reccusteiocapital['emrcusteio']) ? $reccusteiocapital['emrcusteio']+$reccusteiocapital['emrcapital'] : 0; 
	
	print '<form id="cabecalho_Form" name="cabecalho_Form" method="post">';
	print "<table class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3' align='center'>"
		. "	   <tr>"
		. "	       <td width='190px' class='subtitulodireita'>Nome da Secretaria:</td>"
		. "	       <td>"
		.  			   $secretariaEducacao
		. "	       </td>"
		. "	   </tr>"
		. "	   <tr>"
		. "	       <td width='190px' class='subtitulodireita'>Nome da Escola:</td>"
		. "	       <td>"
		.  			   $dadosEscola["nome"]
		. "	       </td>"
		. "	   </tr>"
		. "	   <tr>"
		. "	       <td class='subtitulodireita'>Município / UF:</td>"
		. "	       <td>"
		.  			   $dadosEscola["municipio"] . " / " . $dadosEscola["uf"]
		. "	       </td>"
		. "	   </tr>";
		
		if($form){
			print "<tr>"
			. "	       <td class='subtitulodireita'>Censo 2011:</td>"
			. "	       <td>"
			.  			   $htmlCenso
			. "	       </td>"
			. "	   </tr>"
			. "	   <tr>"
			. "	       <td class='subtitulodireita'>Jornada Escolar:</td>"
			. "	       <td>"
			.  			   $htmlComboJornada
			. "	       </td>"
			. "	   </tr>"
			. "	   <tr>"
			. "	       <td class='subtitulodireita'>Turno:</td>"
			. "	       <td>"
			.  			   $htmlTurnos
			. "	       </td>"
			. "	   </tr>"
			. "	   <tr>"
			. "	       <td class='subtitulodireita'>Valor de Repasse (total):</td>"
			. "	       <td id='valor_repasse'>"
			.  			   number_format($valorRepasse, 2, ",", ".")
			. "	       </td>"
			. "	   </tr>";
		}

		// Mostra macrocampo
		if($mcpid){
		
			$sql = "select mcpdsc from em.macrocampo where mcpid = ".$mcpid;
			$rsMacrocampo = $db->pegaUm($sql);
			
			print "<tr>
					<td class='subtitulodireita'>Macrocampo</td>
					<td>{$rsMacrocampo}</td>
				   </tr>";
		}
		
		// Mostra ação / atividade
		if($papid){
			
			$sql = "select * from em.emigap where papid = ".$papid;
			$rsAcao = $db->pegaLinha($sql);
			
			
			
			print "<tr>
					<td class='subtitulodireita'>Ação/Atividade</td>
					<td>{$rsAcao['papcaoatividade']}</td>
				  </tr>
				  <tr>
					<td class='subtitulodireita'>Meta</td>
					<td>{$rsAcao['papmeta']}</td>
				  </tr>";
			
			if($critica){
				
				$sql = "select * from em.emiprofissionalenvolvido  where emiid = {$emiid} and mcpid = {$mcpid}";	
				$rsProfissionais = $db->pegaLinha($sql);
				$htmlProfissionais = 'sem dados';
				if($rsProfissionais){				
					$htmlProfissionais = 'Professor(a): '.$rsProfissionais['preqtdprofessor'].
										', Equipe Direção: '.$rsProfissionais['preqtddirecao'].
										', Outros Profissionais:'.$rsProfissionais['preqtdoutros'].
										', Total: '.($rsProfissionais['preqtdprofessor']+$rsProfissionais['preqtddirecao']+$rsProfissionais['preqtdoutros']);
				}
				
				print "<tr>
						<td class='subtitulodireita'>Profissionais de Educação</td>
						<td>{$htmlProfissionais}</td>
					  </tr>";
				
				$sql = "select b.* from em.emigap g
						inner join em.emibeneficiario b on b.benid = g.benid
						where g.mcpid = {$mcpid}
						and g.emiid = {$emiid}";
				
				$rsBeneficiarios = $db->pegaLinha($sql);
				$htmlBeneficiario = 'sem dados';
				if($rsBeneficiarios){
	
					$htmlBeneficiario = '1º Ano Mat. '.$rsBeneficiarios['benqtd1anomat'].
										' aluno(s), 1º Ano Vesp. '.$rsBeneficiarios['benqtd1anovesp'].
										' aluno(s), 1º Ano Vesp. '.$rsBeneficiarios['benqtd1anonot'].
										' aluno(s), 2º Ano Mat. '.$rsBeneficiarios['benqtd2anomat'].
										' aluno(s), 2º Ano Vesp. '.$rsBeneficiarios['benqtd2anovesp'].
										' aluno(s), 2º Ano Not. '.$rsBeneficiarios['benqtd2anonot'].
										' aluno(s), 3º Ano Mat. '.$rsBeneficiarios['benqtd3anomat'].
										' aluno(s), 3º Ano Vesp. '.$rsBeneficiarios['benqtd3anovesp'].
										' aluno(s), 3º Ano Not. '.$rsBeneficiarios['benqtd3anonot'].
										' aluno(s), 4º Ano Mat. '.$rsBeneficiarios['benqtd4anomat'].
										' aluno(s), 4º Ano Vesp. '.$rsBeneficiarios['benqtd4anovesp'].
										' aluno(s), 4º Ano Not. '.$rsBeneficiarios['benqtd4anonot'].' aluno(s), '.
										'Total: '.($rsBeneficiarios['benqtd1anomat']+$rsBeneficiarios['benqtd1anovesp']+$rsBeneficiarios['benqtd1anonot']
										+$rsBeneficiarios['benqtd2anomat']+$rsBeneficiarios['benqtd2anovesp']+$rsBeneficiarios['benqtd2anonot']
										+$rsBeneficiarios['benqtd3anomat']+$rsBeneficiarios['benqtd3anovesp']+$rsBeneficiarios['benqtd3anonot']
										+$rsBeneficiarios['benqtd4anomat']+$rsBeneficiarios['benqtd4anovesp']+$rsBeneficiarios['benqtd4anonot']).' aluno(s)';
				}
				
				print "<tr>
						<td class='subtitulodireita'>Beneficiários</td>
						<td>{$htmlBeneficiario}</td>
					  </tr>";
			}
		}
		
	print "</table></form>";
	
		
}

function verificaValidacaoCritica( $emiid ) {
	
	global $db;
	
	$retorno = true;
	
	// Dimensões
	$sql = "SELECT 
				emdid as id,
				dimcod as codigo,
				dimdsc as descricao
			FROM 
				em.emidimensao ed
			INNER JOIN 
				cte.dimensao cd ON ed.dimid = cd.dimid";
	$dadosDimensao = $db->carregar( $sql );
	
	if ( $dadosDimensao ) {
		for( $i = 0; $i < count( $dadosDimensao ); $i++ ) {
			// Linhas de Ação
			$sql = "SELECT 
						lacid as id, 
						laccod as codigo, 
						lacdsc as descricao
					FROM 
						em.emilinhaacao
					WHERE
						emdid = {$dadosDimensao[$i]["id"]} AND
						tppid = ".EMI_TIPO_ENTIDADE_ESCOLA;
			$dadosLinhaAcao = $db->carregar( $sql );
			
			if ( $dadosLinhaAcao ){
				for( $j = 0; $j < count( $dadosLinhaAcao ); $j++ ) {
					// Componentes
					$sql = "SELECT
								comid as id,
								comcod as codigo,
								comdsc as descricao
							FROM
								em.emicomponentes
							WHERE
								lacid = {$dadosLinhaAcao[$j]["id"]}
							ORDER BY
								codigo";
					$dadosComponentes = $db->carregar( $sql );
		
					if ( $dadosComponentes ){
						for( $k = 0; $k < count( $dadosComponentes ); $k++ ) {
							// PAPS
							$sql = "SELECT
										papid as id,
										trim(papcaoatividade) as atividade,
										trim(papmeta) as meta
									FROM
										em.emipap
									WHERE
										comid = {$dadosComponentes[$k]["id"]} AND
										emiid = {$emiid} AND
										papstatus = 'A'";
							$dadosPap = $db->carregar( $sql );
							
							if($dadosPap) {
								for( $l = 0; $l < count( $dadosPap ); $l++ ) {
									// Recupera se a crítica realizada validou ou não as Ações/Atividades.
									$crpvalidado = $db->pegaUm("SELECT crpvalidado FROM emi.critricapap WHERE papid = ".$dadosPap[$l]["id"]);
									
									if($crpvalidado == "f") {
										return false;
										exit;
									} else {
										// recupera se foi feita alguma observação
											$crpobs	= $db->pegaUm("SELECT crpobs FROM emi.critricapap WHERE papid = ".$dadosPap[$l]["id"]);
											if($crpobs == "t") $retorno = "observacao";
									} 
									
									$sql = "SELECT
												mdoid
											FROM 
												em.emimatrizdistribuicaoorcamentar em
											INNER JOIN
												cte.unidademedidadetalhamento cu ON cu.unddid = em.unddid
											INNER JOIN
												em.emiitemfinanciavel ei ON ei.itfid = em.itfid 
											WHERE 
												papid = {$dadosPap[$l]["id"]} AND mdostatus = 'A'
											ORDER BY
												itfdsc";
									$dadosMatriz = $db->carregar( $sql );
									
									if( $dadosMatriz[0] ) {
										for( $m = 0; $m < count( $dadosMatriz ); $m++ ) {
											// Recupera se a crítica realizada validou ou não o item da matriz.
											$crmvalidado = $db->pegaUm("SELECT crmvalidado FROM emi.critricamatriz WHERE mdoid = ".$dadosMatriz[$m]["mdoid"]);
											
											if($crmvalidado == "f") {
												return false;
												exit;
											} else {
												// recupera se foi feita alguma observação
												$crmobs = $db->pegaUm("SELECT crmobs FROM emi.critricamatriz WHERE mdoid = ".$dadosMatriz[$m]["mdoid"]);
												if($crmobs == "t") $retorno = "observacao";
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	return $retorno;
}

function verificaEnviarSeduc(){
	global $db; 
	// Não permite enviar para análise da Seduc após 31/08/2012 23:59:59
	// Concatena a data atual e a data limite formando números inteiros, para comparação
	if(date('YmdHis') > DATA_LIMTE){ 
		$mensagem = "
			<script>
				alert('Data para envio do PRC expirou, por favor, faça contato com a Coordenação Geral de Ensino Médio para que a SEDUC informe e solicite novo prazo.');
			</script>
		";
		if(checkPerfil(PERFIL_CADASTRADOR)){
			switch ($_SESSION['estuf']) {
				/*case 'PI':
					if(date('YmdHis') > 20120731235959){
						echo $mensagem;
						return false;
					}
					break;*/
				default:
					echo $mensagem;
					return false;
					break;
			}
		}
		else{
			echo $mensagem;
			return false;
		}
	}
	
	$sql = "SELECT ent.* FROM entidade.entidade ent 
			LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
			LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			WHERE fen.funid='".ENTIDADE_FUNID_DIRETOR."' AND fea.entid='{$_SESSION['entid']}'";
	
	$rsDiretor = $db->pegaLinha($sql);
	
	if(!$rsDiretor){
		return false;
	}
		
	$sql = "SELECT ent.* FROM entidade.entidade ent 
			LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
			LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			WHERE fen.funid='".ENTIDADE_FUNID_COORDENADOR."' AND fea.entid='{$_SESSION['entid']}'";
	
	$rsArticulador = $db->pegaLinha($sql);
		
	if(!$rsArticulador){						
		return false;
	}
	
	$sql = "SELECT 
				m.mcpid, 
				g.papid, 
				t.mdoid, 
				b.benid, 
				p.preid 
			FROM 
				em.macrocampo m
			LEFT JOIN 
				em.emigap g on g.mcpid = m.mcpid 
					and g.emiid = {$_SESSION['emiid']}
			LEFT JOIN 
				em.emimatrizdistribuicaoorcamentargap t on t.papid = g.papid
			LEFT JOIN 
				em.emibeneficiario b on b.benid = g.benid
			LEFT JOIN 
				em.emiprofissionalenvolvido p on p.mcpid = m.mcpid 
					and p.emiid = {$_SESSION['emiid']}
			ORDER BY 
				m.mcpid, 
				g.papid, 
				t.mdoid, 
				b.benid, 
				p.preid";
	
	$rsMacrocampo = $db->carregar($sql);	
	foreach($rsMacrocampo as $dados){
		if(in_array($dados['mcpid'], array(1,2))){
			if(empty($dados['papid']) || empty($dados['mdoid']) || empty($dados['benid']) || empty($dados['preid'])){
				return false;
			}	
		}else{
			if(!empty($dados['papid']) || !empty($dados['mdoid']) || !empty($dados['benid']) || !empty($dados['preid'])){
				if(empty($dados['papid']) || empty($dados['mdoid']) || empty($dados['benid']) || empty($dados['preid'])){
					return false;
				}	
			}
		}		
	}
	
	$sql = "SELECT etjid from em.emiensinomedioinovador WHERE emiid = ".$_SESSION['emiid'];
	$rsJornada = $db->pegaUm($sql);

	if(!$rsJornada){
		return false;
	}
	
	$sql = "SELECT emiid FROM em.emiescolaturno WHERE emiid = ".$_SESSION['emiid'];
	$rsTurno = $db->pegaUm($sql);
	
	if(!$rsTurno){
		return false;
	}
	
//	$sql = "SELECT 
//				emi.emiid,
//				ede.estuf,
//				ent.entnome,	
//				tjo.etjjornada,	
//				est.estdescricao,
//				(SELECT
//					coalesce(sum(cen.emcquantidadealunos),0) as total 
//				FROM 
//					em.emicenso cen
//				WHERE 
//					cen.entid::integer = ent.entid)	as alunos,
//				coalesce((SELECT 
//					SUM(rcc.emrcusteio+rcc.emrcapital) as total
//				FROM 
//					em.emireccusteiocapital rcc
//				WHERE 
//					rcc.emrjornada = tjo.etjjornada 
//				AND 
//					(SELECT
//						coalesce(sum(cen.emcquantidadealunos),0) as total 
//					FROM 
//						em.emicenso cen
//					WHERE 
//						cen.entid::integer = ent.entid) BETWEEN rcc.emrqtdinialunos 
//				AND 
//					rcc.emrqtdfinalunos),0) as recurso,
//				coalesce((select 
//					SUM(mdovalorunitario*mdoqtd) 
//				FROM 
//					em.emimatrizdistribuicaoorcamentargap gt1
//				JOIN 
//					em.emigap ga1 on ga1.papid = gt1.papid 
//				WHERE 
//					ga1.emiid = emi.emiid),0) as itens
//			FROM 
//				em.emiensinomedioinovador emi
//			INNER JOIN 
//				entidade.entidade ent on ent.entid = emi.entid
//			INNER JOIN 
//				entidade.endereco ede on ede.entid = ent.entid
//			INNER JOIN 
//				em.emitipojornada tjo on tjo.etjid = emi.etjid
//			INNER JOIN
//				territorios.estado est on upper(est.estuf) = upper(ede.estuf)
//			WHERE 
//				emi.entid = {$_SESSION['entid']}
//			ORDER BY
//				ede.estuf, ent.entnome";
//	
//		$rsValor = $db->pegaLinha($sql);
//				
//		if($rsValor['itens'] > $rsValor['recurso']){
//			return false;	
//		}

		$sql = "SELECT 
					coalesce(sum(mdovalorunitario*mdoqtd),0) as total 
				FROM 
					em.emimatrizdistribuicaoorcamentargap m
				INNER JOIN 
					em.emigap p ON p.papid = m.papid 
				WHERE 
					emiid = {$_SESSION['emiid']}  
				AND 
					itfid in (8,5,3,2,7,1,4,6,13)";
		
		$totalCusteio = $db->pegaUm($sql);
		
		$sql = "SELECT 
					coalesce(sum(mdovalorunitario*mdoqtd),0) as total 
				FROM 
					em.emimatrizdistribuicaoorcamentargap m
				INNER JOIN 
					em.emigap p ON p.papid = m.papid
				WHERE 
					emiid = {$_SESSION['emiid']} 
				AND 
					itfid in (9,11,10,12)";
		
		$totalCapital = $db->pegaUm($sql);
		
		$saldoCusteio = $totalCusteio;
		$saldoCapital = $totalCapital;
		
		$sql = "
			SELECT 
				emvlrcusteio,
				emvlrcapital,
				emvlrpago
			FROM em.emipagtofnde
			WHERE entcodent = (select entcodent from entidade.entidade where entid = {$_SESSION['entid']})
		";
		
		$dadosMEC = $db->pegaLinha($sql);
		
		$capitalDisponivel = $dadosMEC['emvlrcapital'] - $saldoCapital;
		$custeioDisponivel = $dadosMEC['emvlrcusteio'] - $saldoCusteio;
		//$totalDisponivel   = $dadosMEC['emvlrpago']    - $totalGeralAxB;
		$totalDisponivel   = $dadosMEC['emvlrpago']    - ($saldoCapital + $saldoCusteio);
		
		if($totalDisponivel<0){
			return false;	
		}
		
	return true;	
}

function wfVerificarPendencias(){
	global $db;
	$docid = pegaDocid( $_SESSION['entid'], $_SESSION['emiid'] );
	$esdid = pegaEstadoAtual( $docid );
	
	// Não permite enviar para análise da Seduc após 30/06/2012 23:59:59
	// Concatena a data atual e a data limite formando números inteiros, para comparação
	
	$sql = "SELECT 	m.estuf
	FROM entidade.entidade e
	INNER JOIN entidade.endereco endr ON endr.entid = e.entid
	INNER JOIN territorios.municipio m ON m.muncod = endr.muncod
	INNER JOIN em.emiensinomedioinovador emi ON emi.entid = e.entid						 	
	WHERE e.entid = '{$_SESSION['entid']}'";

	$estado = $db->pegaUm($sql);
	
	$mensagem = "
	<script>
		alert('Data para envio do PRC expirou, por favor, faça contato com a Coordenação Geral de Ensino Médio para que a SEDUC informe e solicite novo prazo.');
		//location.href='em.php?modulo=painel&acao=A';
	</script>";

	if(in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO,WF_ESDID_EM_ANALISE_SEDUC))){
		if( (date('YmdHis') > DATA_LIMTE)){
				echo $mensagem;
				return false;
		}
	}	
	
	if(in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
		echo "<script>
						
				function verificarPendencias(emiid){
					var janela = window.open(\"?modulo=principal/popupPendenciasPRC&acao=A&emiid=\" + emiid, \"pendenciasPRC\", \"menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=600\");
					janela.focus();
				}
				
				jQuery(function(){					
					workflow = jQuery('#td_workflow').find('table tbody');
					botao = '<a href=\"javascript:void(0)\" onclick=\"verificarPendencias({$_SESSION['emiid']})\">Verificar</a>';
					html  = '<tr style=\"background-color: #c9c9c9; text-align:center;\"><td style=\"font-size:7pt; text-align:center;\">Pendências</td></tr>';
					html += '<tr style=\"text-align:center;\"><td style=\"font-size:7pt; border-top: 2px solid #d0d0d0;\">'+botao+'</td></tr>';
					workflow.append(html);	
				});
				
			</script>";
	}
}

function qtRPCPendentes($id = "tabela_1",$sql,$sqlAgrupador = array(),$arrOff = array() ){
	
	 global $db;
	 $dados = $db->carregar($sql);
 
	 if(!$dados){
	 	return 0;
	 }

	 $num_colunas = count($dados[0]);
	 $num_colunas = $num_colunas - (count($arrOff));
	 
	 $i = 0;
	 foreach($dados as $d){
	 	
	 	$sqlAg = $sqlAgrupador['sql'];
	 	if($sqlAgrupador['sql']){
	 		
	 		if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']] || $d[$sqlAgrupador['agrupador']] == "0" || $d[$sqlAgrupador['agrupador']] == "999999"  || $d[$sqlAgrupador['agrupador']] == "888888"){
	 			
	 			if($d[$sqlAgrupador['agrupador']] == "0" && $id == "tabela_1"){
	 				
	 				//executa este sql quando esdid is null  --> Não iniciado --> $d[$sqlAgrupador['agrupador']] == "0"	
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1; 
									
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM entidade.entidade ent
								INNER JOIN em.emiensinomedioinovador emi    ON ent.entid           = emi.entid							
								INNER JOIN entidade.endereco         entEnd on ent.entid           = entEnd.entid
								INNER JOIN territorios.estado        est    ON upper(entEnd.estuf) = upper(est.estuf)
								LEFT  JOIN workflow.documento        d      on d.docid             = emi.docid
								LEFT  JOIN workflow.estadodocumento  ed     on ed.esdid            = d.esdid
							WHERE d.esdid is null
							AND   emi.entcodent not in (select mem.entcodent
							                            from em.emiensinomedioinovador mem
							                            where mem.emianoreferencia = ".$anoAnterior."
							                            and   mem.emistatus        = 'A')
							AND   emi.emianoreferencia = ".$ano."							
							AND   emi.emistatus        = 'A' 
							AND   est.estuf            = '{$_REQUEST['estuf']}'
							AND   ed.esddsc            not in ('Finalizado', 'Concluido')
							GROUP BY ed.esdid, descricao
							ORDER BY descricao";
	 			
	 			}elseif((($d[$sqlAgrupador['agrupador']] == "0") || ($d[$sqlAgrupador['agrupador']] == null)) && ($id == "tabela_2")){	 			
	 				
	 				//executa este sql quando esdid is null  --> Não iniciado --> $d[$sqlAgrupador['agrupador']] == "0"
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
						 				
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM em.emiensinomedioinovador emi 
								INNER JOIN em.emiensinomedioinovador me1    on me1.entid            = emi.entid
								AND                                            me1.emianoreferencia = ".$anoAnterior."
								AND                                            me1.emistatus        = 'A'							
								INNER JOIN entidade.endereco         entEnd on ent.entid            = entEnd.entid
								INNER JOIN territorios.estado        est    ON upper(entEnd.estuf)  = upper(est.estuf)
								LEFT  JOIN workflow.documento        d      on d.docid              = emi.docid
								LEFT  JOIN workflow.estadodocumento  ed     on ed.esdid             = d.esdid
							WHERE d.esdid is null
							AND   emi.emianoreferencia = ".$ano."
							AND   emi.emistatus        = 'A' 
							AND   est.estuf            = '{$_REQUEST['estuf']}'
							AND   ed.esddsc            not in ('Finalizado', 'Concluido')
							GROUP BY ed.esdid, descricao
							ORDER BY descricao";
 						
	 			}else{
	 				
	 				$sqlAg = str_replace("|agrupador|"," = ".$d[$sqlAgrupador['agrupador']],$sqlAg);
	 				
	 			}
	
	 			$dadosAgrupados = $db->carregar($sqlAg);

	 		}
	 	}
	 	
	 	$keys = array_keys($d);
	 	$j = 0;
		while($j < $num_colunas){
			if(!strstr($keys[$j],"ordem") && is_numeric($d[$keys[$j]])  && !in_array($keys[$j],$arrOff)){
		 		$soma[$keys[$j]] += $d[$keys[$j]];
		 		$campo_soma[] = $keys[$j];
		 	}
		 	$j++;
		 	
		}
	 	$i++;
	 }
	 
	$totalRPCPendentes = 0;
 	foreach($keys as $k1 => $k){
 		$totalRPCPendentes =+ $soma[$k];
 	}
	
	return $totalRPCPendentes;
}

?>