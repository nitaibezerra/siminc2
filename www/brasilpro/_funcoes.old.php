<?php

// funcoes publicas

function selecionar_unidade_feredacao( $estuf ){
	global $db;
	$_SESSION['itrid'] = 1;
	$_SESSION['inuid'] = null;
	$_SESSION['estuf'] = null;
	
	if ( empty( $estuf ) ) {
		return false;
	}
	if ( !verificar_responsabilidade( $estuf ) ) {
		return false;
	}
	$sql = sprintf(
		"select inuid from cte.instrumentounidade where estuf = '%s' and itrid = %d",
		$estuf,
		$_SESSION['itrid']
	);
	$inuid = $db->pegaUm( $sql );
	if ( empty( $inuid ) ) {
		$sql = sprintf(
			"insert into cte.instrumentounidade ( estuf, itrid ) values ( '%s', %d ) returning inuid",
			$estuf,
			$_SESSION['itrid']
		);
		$inuid = $db->pegaUm( $sql );
		if ( !$inuid ) {
			return false;
		}
	}
	$sql = sprintf(
		"select iu.inuid, iu.docid, e.estdescricao from cte.instrumentounidade iu inner join territorios.estado e on e.estuf = iu.estuf where inuid = %d",
		$inuid
	);
	$registro = $db->pegaLinha( $sql );
	if ( !$registro['docid'] ) {
		$registro['docid'] = wf_cadastrarDocumento( WF_TIPO_CTE, "Plano de Metas CTE - " . $registro['estdescricao'] );
		$sql = sprintf(
			"update cte.instrumentounidade set docid = %d where inuid = %d",
			$registro['docid'],
			$registro['inuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	$_SESSION['docid'] = $registro['docid'];
	$_SESSION['inuid'] = $inuid;
	$_SESSION['estuf'] = $estuf;
	return true;
}

function verificar_responsabilidade( $estuf ){
	$arrUfsPermitidas = cte_UfsPermitidas();
	if( in_array( $estuf , $arrUfsPermitidas ) ) return true;
	else return false;
}

function verifica_preenchimento( $inuid )
{
	return pegar_percentagem( $inuid ) > 99;
}

function pegar_percentagem( $inuid )
{
	$inuid = (integer) $inuid;
	global $db;
	$total = $db->pegaUm(
		"select count(*)
		from cte.instrumento i
		inner join cte.dimensao d on d.itrid = i.itrid and d.dimstatus = 'A'
		inner join cte.areadimensao a on a.dimid = d.dimid and a.ardstatus = 'A'
		inner join cte.indicador ind on ind.ardid = a.ardid and ind.indstatus = 'A'
		where i.itrid = 1"
	);
	$pontuados = $db->pegaUm( "
		select
			count(*)
		from cte.instrumento i
			inner join cte.dimensao d on d.itrid = i.itrid and d.dimstatus = 'A'
			inner join cte.areadimensao a on a.dimid = d.dimid and a.ardstatus = 'A'
			inner join cte.indicador ind on ind.ardid = a.ardid and ind.indstatus = 'A'
			inner join cte.pontuacao p on p.indid = ind.indid and p.inuid = " . $inuid . "
		where
			i.itrid = 1
		"
	);
	return $total > 0 ? abs( intval( ( $pontuados / $total ) * 100 ) ) : 0;
}

function verifica_sessao(){
	if ( !$_SESSION['inuid'] ) {
		header( "Location: ?modulo=inicio&acao=A" );
		exit();
	}
}


function cte_podeEditarQuestaoPontual( $estuf )
{
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
			$perfis = array( CTE_PERFIL_EQUIPE_LOCAL, CTE_PERFIL_EQUIPE_LOCAL_APROVACAO );
			break;
		case CTE_ESTADO_ANALISE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
			break;
		case CTE_ESTADO_FINALIZADO:
			$perfis = array();
			break;
		default:
			$perfis = array();
			break;
	}
	if ( $db->testa_superuser() ) {
		return true;
	} else foreach ( $perfis as $perfil ) {
		if ( _cte_possuiPerfil( $perfil ) ) {
			return true;
		}
	}
	return false;
}

function cte_podeEditarIndicador( $estuf )
{
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
			$perfis = array( CTE_PERFIL_EQUIPE_LOCAL, CTE_PERFIL_EQUIPE_LOCAL_APROVACAO );
			break;
		case CTE_ESTADO_ANALISE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
			break;
		case CTE_ESTADO_FINALIZADO:
			$perfis = array();
			break;
		default:
			$perfis = array();
			break;
	}
	if ( $db->testa_superuser() ) {
		return true;
	} else foreach ( $perfis as $perfil ) {
		if ( _cte_possuiPerfil( $perfil ) ) {
			return true;
		}
	}
	return false;
}

function cte_podeVerQuestaoPontual( $estuf )
{
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FINALIZADO:
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA
			);
			break;
		default:
			$perfis = array();
			break;
	}
	if ( $db->testa_superuser() ) {
		return true;
	} else foreach ( $perfis as $perfil ) {
		if ( _cte_possuiPerfil( $perfil ) ) {
			return true;
		}
	}
	return false;
}

function cte_podeAnalisar()
{
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	return
		(
			_cte_possuiPerfil( CTE_PERFIL_EQUIPE_TECNICA ) ||
			$db->testa_superuser()
		) &&
		$esdid == CTE_ESTADO_ANALISE;
}

function cte_podeVerRelatorioParCopia()
{
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FINALIZADO:
			return true;
		default:
			return $db->testa_superuser();
	}
}

function cte_podeVerRelatorioParAtual()
{
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_FINALIZADO:
			return true;
		default:
			return $db->testa_superuser();
	}
}

function cte_podeVerIndicador( $estuf )
{
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FINALIZADO:
			$perfis = array(
				CTE_PERFIL_ALTA_GESTAO,
				CTE_PERFIL_CONSULTA_GERAL,
				CTE_PERFIL_CONSULTORES,
				CTE_PERFIL_EQUIPE_LOCAL,
				CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
				CTE_PERFIL_EQUIPE_TECNICA
			);
			break;
		default:
			$perfis = array();
			break;
	}
	if ( $db->testa_superuser() ) {
		return true;
	} else foreach ( $perfis as $perfil ) {
		if ( _cte_possuiPerfil( $perfil ) ) {
			return true;
		}
	}
	return false;
}

function cte_MunicipiosPermitidos(){}

function cte_UfsPermitidas()
{
	global $db;
	if ( $db->testa_superuser() || _cte_possuiPerfilSemVinculoUf() ) {
		// pega todos os estados
		$sql = "select estuf from territorios.estado";
	} else {
		// pega estados do perfil equipe local
		$sql =
			"select distinct e.estuf
			from territorios.estado e
			inner join cte.usuarioresponsabilidade ur on ur.estuf = e.estuf
			where ur.usucpf = '" . $_SESSION['usucpf'] . "' and rpustatus = 'A'";
	}
	$dados = $db->carregar( $sql );
	$dados = $dados ? $dados : array();
	$retorno = array();
	foreach ( $dados as $linha ) {
		array_push( $retorno, $linha['estuf'] );
	}
	return $retorno;
}

function cte_podeElaborarPlanoDeAcoes( $docid ){
	global $db;
	$documento = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_DIAGNOSTICO:
			$perfis = array();
			break;
		case CTE_ESTADO_PAR:
			$perfis = array( CTE_PERFIL_EQUIPE_LOCAL, CTE_PERFIL_EQUIPE_LOCAL_APROVACAO );
			break;
		case CTE_ESTADO_ANALISE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
			break;
		case CTE_ESTADO_FINALIZADO:
			$perfis = array();
			break;
		default:
			$perfis = array();
			break;
	}
	if ( $db->testa_superuser() ) {
		return true;
	} else foreach ( $perfis as $perfil ) {
		if ( _cte_possuiPerfil( $perfil ) ) {
			return true;
		}
	}
	return false;
}

function cte_estadoVerParecer(){
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	$estados = array( CTE_ESTADO_PAR, CTE_ESTADO_ANALISE, CTE_ESTADO_FINALIZADO );
	return in_array( $esdid, $estados );
}

function cte_podeEditarParecer(){
	global $db;
	$documento = wf_pegarEstadoAtual( $_SESSION['docid'] );
	$esdid = (integer) $documento['esdid'];
	switch ( $esdid ) {
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
			$perfis = array();
			break;
		case CTE_ESTADO_ANALISE:
			$perfis = array( CTE_PERFIL_EQUIPE_TECNICA );
			break;
		case CTE_ESTADO_FINALIZADO:
			$perfis = array();
			break;
		default:
			$perfis = array();
			break;
	}
	if ( $db->testa_superuser() ) {
		return true;
	} else foreach ( $perfis as $perfil ) {
		if ( _cte_possuiPerfil( $perfil ) ) {
			return true;
		}
	}
	return false;
}


// funcoes privadas

function _cte_possuiPerfil( $pflcod )
{
	global $db;
	$pflcod = (integer) $pflcod;
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod = " . $pflcod;
	return $db->pegaUm( $sql ) > 0;
}

function _cte_possuiPerfilComVinculoUf( $pflcod, $estuf )
{
	global $db;
	$pflcod = (integer) $pflcod;
	$estuf = str_replace( "'", "\\'", trim( $estuf ) );
	$sql = "
		select
			count(*)
		from cte.usuarioresponsabilidade ur
			inner join seguranca.perfil p on p.pflcod = ur.pflcod
		where
			ur.pflcod = " . $pflcod . " and
			ur.estuf = '" . $estuf . "' and
			ur.usucpf = '" . $_SESSION['usucpf'] . "' and
			ur.rpustatus = 'A' and
			p.pflstatus = 'A'
	";
	return $db->pegaUm( $sql ) > 0;
}

function _cte_possuiPerfilSemVinculoUf()
{
	global $db;
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
			inner join seguranca.perfil using ( pflcod )
			left join cte.tprperfil using ( pflcod )
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			tprcod is null and
			sisid = " . CTE_SISTEMA;
	return $db->pegaUm( $sql ) > 0;
}

// funções de relatorio

function cte_estadoPosCopia( $docid )
{
	global $db;
	$docid = (integer) $docid;
	$estado = wf_pegarEstadoAtual( $docid );
	$esdid = (integer) $estado['esdid'];
	switch ( $esdid )
	{
		case CTE_ESTADO_ANALISE:
		case CTE_ESTADO_FINALIZADO:
			return true;
		case CTE_ESTADO_DIAGNOSTICO:
		case CTE_ESTADO_PAR:
		default:
			return false;
	}
}

function cte_agruparDadosRelatorio( array $agrupadores, array $itens )
{
	if ( count( $agrupadores ) == 0 || count( $itens ) == 0 )
	{
		return array();
	}
	
	// captura agrupador atual
	$agrupadorAtual = array_shift( $agrupadores );
	
	// inicia variavel resultante
	$resultado = array();
	
	// percorre itens (realiza agrupamento)
	foreach ( $itens as $item )
	{
		
		$chave = $item[$agrupadorAtual];
		if ( !array_key_exists( $chave, $resultado ) )
		{
			$resultado[$chave] = $item;
			
			unset( $resultado[$chave]['fis_1_original'] );
			unset( $resultado[$chave]['fis_2_original'] );
			unset( $resultado[$chave]['fis_3_original'] );
			unset( $resultado[$chave]['fis_4_original'] );
			unset( $resultado[$chave]['fis_1_copia'] );
			unset( $resultado[$chave]['fis_2_copia'] );
			unset( $resultado[$chave]['fis_3_copia'] );
			unset( $resultado[$chave]['fis_4_copia'] );
			unset( $resultado[$chave]['fin_1_original'] );
			unset( $resultado[$chave]['fin_2_original'] );
			unset( $resultado[$chave]['fin_3_original'] );
			unset( $resultado[$chave]['fin_4_original'] );
			unset( $resultado[$chave]['fin_1_copia'] );
			unset( $resultado[$chave]['fin_2_copia'] );
			unset( $resultado[$chave]['fin_3_copia'] );
			unset( $resultado[$chave]['fin_4_copia'] );
			
			// financeiro
				$resultado[$chave]['fin_sol'] = array();
					$resultado[$chave]['fin_sol'][1] = 0;
					$resultado[$chave]['fin_sol'][2] = 0;
					$resultado[$chave]['fin_sol'][3] = 0;
					$resultado[$chave]['fin_sol'][4] = 0;
				$resultado[$chave]['fin_ate'] = array();
					$resultado[$chave]['fin_ate'][1] = 0;
					$resultado[$chave]['fin_ate'][2] = 0;
					$resultado[$chave]['fin_ate'][3] = 0;
					$resultado[$chave]['fin_ate'][4] = 0;
			// fisico
				$resultado[$chave]['fis_sol'] = array();
					$resultado[$chave]['fis_sol'][1] = 0;
					$resultado[$chave]['fis_sol'][2] = 0;
					$resultado[$chave]['fis_sol'][3] = 0;
					$resultado[$chave]['fis_sol'][4] = 0;
				$resultado[$chave]['fis_ate'] = array();
					$resultado[$chave]['fis_ate'][1] = 0;
					$resultado[$chave]['fis_ate'][2] = 0;
					$resultado[$chave]['fis_ate'][3] = 0;
					$resultado[$chave]['fis_ate'][4] = 0;
			// total
				$resultado[$chave]['tot_sol'] = array();
					$resultado[$chave]['tot_sol'][1] = 0;
					$resultado[$chave]['tot_sol'][2] = 0;
					$resultado[$chave]['tot_sol'][3] = 0;
					$resultado[$chave]['tot_sol'][4] = 0;
				$resultado[$chave]['tot_ate'] = array();
					$resultado[$chave]['tot_ate'][1] = 0;
					$resultado[$chave]['tot_ate'][2] = 0;
					$resultado[$chave]['tot_ate'][3] = 0;
					$resultado[$chave]['tot_ate'][4] = 0;
			
			// filhos
			$resultado[$chave]['sub'] = array();
		}
		
		// detecta os posfixo dos campos de valores
		if ( cte_estadoPosCopia( $item['docid'] ) )
		{
			$campoSolicitacao = 'copia';
			$campoAtendimento = 'original';
		}
		else
		{
			$campoSolicitacao = 'original';
			$campoAtendimento = 'copia';
		}
		
		// adiciona valores do item ao agrupador
		
		$resultado[$chave]['fin_sol'][1] += $item['fin_1_' . $campoSolicitacao];
		$resultado[$chave]['fin_sol'][2] += $item['fin_2_' . $campoSolicitacao];
		$resultado[$chave]['fin_sol'][3] += $item['fin_3_' . $campoSolicitacao];
		$resultado[$chave]['fin_sol'][4] += $item['fin_4_' . $campoSolicitacao];
		
		$resultado[$chave]['fin_ate'][1] += $item['fin_1_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][2] += $item['fin_2_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][3] += $item['fin_3_' . $campoAtendimento];
		$resultado[$chave]['fin_ate'][4] += $item['fin_4_' . $campoAtendimento];
		
		$resultado[$chave]['fis_sol'][1] += $item['fis_1_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][2] += $item['fis_2_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][3] += $item['fis_3_' . $campoSolicitacao];
		$resultado[$chave]['fis_sol'][4] += $item['fis_4_' . $campoSolicitacao];
		
		$resultado[$chave]['fis_ate'][1] += $item['fis_1_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][2] += $item['fis_2_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][3] += $item['fis_3_' . $campoAtendimento];
		$resultado[$chave]['fis_ate'][4] += $item['fis_4_' . $campoAtendimento];
		
		// adicionar o item como filho do agrupador caso haja mais agrupadores
		
		if ( count( $agrupadores ) > 0 )
		{
			array_push( $resultado[$chave]['sub'], $item );
		}
		
	}
	
	// agrupa filhos dos filhos do agrupador atual caso haja mais agrupadores
	
	reset( $agrupadores );
	if ( count( $agrupadores ) > 0 )
	{
		reset( $resultado );
		foreach ( $resultado as &$item )
		{
			$item['sub'] = cte_agrupar( $agrupadores, $item['sub'] );
		}
	}
	
	ksort( $resultado );
	reset( $resultado );
	return $resultado;
}

function cte_desenhaRelatorio( array $itens, $exibeSol, $exibeAte, $exibeFis, $exibeFin, $profundidade = 0 )
{
	if ( count( $itens ) == 0 )
	{
		return;
	}
	
	// verifica quais campos de valores devem aparecer
	$exibeFis = (boolean) $exibeFis;
	$exibeFin = (boolean) $exibeFin;
	
	$exibeSol = (boolean) $exibeSol;
	$exibeAte = (boolean) $exibeAte;
	
	$rowspan = 0;
	$rowspan += $exibeSol ? 1 : 0;
	$rowspan += $exibeAte ? 1 : 0;
	
	$padding = $profundidade * 25;
	
	foreach ( $itens as $agrupador => $item )
	{
		?>
		<tr>
			<td style="padding-left: <?= $padding ?>px; background-color: #efefef;" colspan="7">
				<?php if ( $profundidade > 0 ) : ?>
					<img src="/imagens/seta_filho.gif" align="absmiddle"/>
				<?php endif; ?>
				<b><?= $agrupador ?></b>
			</td>
		</tr>
		<?php if ( $exibeFis ) : ?>
			<tr>
				<td style="padding-left: <?= $padding ?>px; color: #606060;" rowspan="<?= $rowspan ?>">
					Físico
				</td>
				<?php if ( $exibeSol ) : ?>
					<? cte_desenhaValores( 'Solicitado', $item['fis_sol'], 0 ) ?>
				<?php elseif ( $exibeAte ) : ?>
					<? cte_desenhaValores( 'Atendido', $item['fis_ate'], 0 ) ?>
				<?php endif; ?>
			</tr>
			<?php if ( $exibeSol && $exibeAte ) : ?>
				<tr>
					<? cte_desenhaValores( 'Atendido', $item['fis_ate'], 0 ) ?>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
		<?php if ( $exibeFin ) : ?>
			<tr>
				<td style="padding-left: <?= $padding ?>px; color: #606060;" rowspan="<?= $rowspan ?>">
					Financeiro
				</td>
				<?php if ( $exibeSol ) : ?>
					<? cte_desenhaValores( 'Solicitado', $item['fin_sol'] ) ?>
				<?php elseif ( $exibeAte ) : ?>
					<? cte_desenhaValores( 'Atendido', $item['fin_ate'] ) ?>
				<?php endif; ?>
			</tr>
			<?php if ( $exibeSol && $exibeAte ) : ?>
				<tr>
					<? cte_desenhaValores( 'Atendido', $item['fin_ate'] ) ?>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
		<?php
		cte_desenhaRelatorio( $item['sub'], $exibeSol, $exibeAte, $exibeFis, $exibeFin, $profundidade + 1 );
	}
}

function cte_desenhaValores( $label, $valores, $casas = 2 )
{
	$casas = (integer) $casas;
	?>
	<td align="center" style="color: #606060;">
		<?= $label ?>
	</td>
	<td align="right" style="color: #508050;">
		<?= number_format( $valores[1], $casas, ",", "." ) ?>
	</td>
	<td align="right" style="color: #508050;">
		<?= number_format( $valores[2], $casas, ",", "." ) ?>
	</td>
	<td align="right" style="color: #508050;">
		<?= number_format( $valores[3], $casas, ",", "." ) ?>
	</td>
	<td align="right" style="color: #508050;">
		<?= number_format( $valores[4], $casas, ",", "." ) ?>
	</td>
	<td align="right" style="color: #105010;">
		<?= number_format( $valores[1] + $valores[2] + $valores[3] + $valores[4], $casas, ",", "." ) ?>
	</td>
	<?php
}

function cte_apagaCopiaPlanoAcao ( $inuid )
{
	global $db;
	
	if ( $inuid == '' || empty( $inuid ) ) return false;

	$db->executar("delete from cte.subacaoindicador where aciid in
				(select aciid from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ))");
	
	$db->executar("delete from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid )");
	
	$db->executar("delete from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ");
	
	return true;
}

function cte_copiarPlanoAcao ( $inuid )
{
	global $db;
	
	if ( $inuid == '' || empty( $inuid ) ) return false;
	
	$db->executar("delete from cte.subacaoindicador where aciid in
				(select aciid from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ))");
	
	$db->executar("delete from cte.acaoindicador where ptoid in
				(select ptoid from cte.pontuacao where ptostatus = 'C' and inuid = $inuid )");
	
	$db->executar("delete from cte.pontuacao where ptostatus = 'C' and inuid = $inuid ");
	
	
	$pontuacao = $db->carregar("select ptoid from cte.pontuacao where ptostatus = 'A' and inuid = $inuid ");

	foreach ( $pontuacao as $pontuacao1 ):

		$idpontuacao = $pontuacao1['ptoid'];
		$sql = " insert into cte.pontuacao 
					(  crtid,
					  ptojustificativa,
					  ptodemandamunicipal,
					  ptodemandaestadual,
					  ptodata,
					  usucpf,
					  inuid,
					  indid,
					  ptostatus,
					  ptoparecertecnico)
					select 
					  crtid,
					  ptojustificativa,
					  ptodemandamunicipal,
					  ptodemandaestadual,
					  ptodata,
					  usucpf,
					  inuid,
					  indid,
					  'C',
					  ptoparecertecnico
					from cte.pontuacao
					where ptoid = $idpontuacao returning ptoid; ";
		$novoidpontuacao = $db->pegaUm($sql);

		$acao = $db->carregar("select aciid from cte.acaoindicador where ptoid = ".$idpontuacao);				
		
		if ( $acao != '' )
		{

			foreach ( $acao as $acao1 ):
	
				$idacao = $acao1['aciid'];
				$sql1 = " insert into cte.acaoindicador 
							(    ptoid,
								  parid,
								  acidsc,
								  acirpns,
								  acicrg,
								  acidtinicial,
								  acidtfinal,
								  acirstd,
								  acilocalizador,
								  acidata,
								  usucpf)
							select 
							  	  $novoidpontuacao,
								  parid,
								  acidsc,
								  acirpns,
								  acicrg,
								  acidtinicial,
								  acidtfinal,
								  acirstd,
								  acilocalizador,
								  acidata,
								  usucpf
							from cte.acaoindicador
							where aciid = $idacao returning aciid; ";
				$novoidacao = $db->pegaUm($sql1);			
	
				$subacao = $db->carregar("select sbaid from cte.subacaoindicador where aciid =".$idacao);				
				
				if ( $subacao != '' ) 
				{
					foreach ( $subacao as $subacao1 ):
			
						$idsubacao = $subacao1['sbaid'];
						$sql2 = " insert into cte.subacaoindicador 
									(    aciid ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sba1ano,
										  sba2ano,
										  sba3ano,
										  sba4ano,
										  sbaunt,
										  sbauntdsc,
										  sba1ini,
										  sba1fim,
										  sba2ini,
										  sba2fim,
										  sba3ini,
										  sba3fim,
										  sba4ini,
										  sba4fim,
										  sbadata,
										  usucpf
										)
									select 
										  $novoidacao ,
										  undid,
										  frmid,
										  sbadsc,
										  sbastgmpl,
										  sbaprm,
										  sbapcr,
										  sba1ano,
										  sba2ano,
										  sba3ano,
										  sba4ano,
										  sbaunt,
										  sbauntdsc,
										  sba1ini,
										  sba1fim,
										  sba2ini,
										  sba2fim,
										  sba3ini,
										  sba3fim,
										  sba4ini,
										  sba4fim,
										  sbadata,
										  usucpf
									from cte.subacaoindicador
									where sbaid = $idsubacao returning aciid; ";
						$novoidsubacao = $db->pegaUm($sql2);			
						
					endforeach;
				}
				
			endforeach;
		}
	endforeach;
	
	return true;
}

?>