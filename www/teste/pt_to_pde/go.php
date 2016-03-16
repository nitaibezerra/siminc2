<?php

header( "Content-Type: text/plain;" );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$db = new cls_banco();

define( "PROJETO", 8384 );

// captura as ações que tem itens a serem inseridos
$sql = "
	select pt.acaid
	from monitora.planotrabalho pt
		inner join monitora.acao a on a.acaid = pt.acaid
	where ptostatus = 'A'
	group by pt.acaid
";
$acaids = $db->carregar( $sql );
$acaids = $acaids ? $acaids : array();

$sqlInsert = "
insert into pde.atividade
(
	atidescricao,     atidetalhamento,     atidatainicio,       atidatafim,
	usucpf,           esaid,               atiporcentoexec,     acaid,
	atiidpai,         atiordem,            _atiprojeto
)
values
(
	'%s',
	'%s',
	%s,
	%s,
	%s, -- usucpf
	%d, -- estado
	%d, -- porcentagem conclusao
	%d, -- acao
	%s, -- pai
	%d, -- ordem
	%d -- projeto
) returning atiid
";

function pegarIdTarefaAcao( $acaid, $nome = "" )
{
	static $atiids = array();
	global $db, $sqlInsert;
	if ( !array_key_exists( $acaid, $atiids ) )
	{
		$sql = sprintf(
			$sqlInsert,
				$nome,
				"",      // detalhamento
				"null",  // data inicio
				"null",  // data fim
				"null",
				1,       // estado
				0,       // conclusao
				$acaid,
				PROJETO,
				1,       // ordem
				PROJETO
		);
		$atiids[$acaid] = $db->pegaUm( $sql );
	}
	return $atiids[$acaid];
}

foreach ( $acaids as $acaid )
{
	$acaid = $acaid['acaid'];
	$sql = "
		select
			pt.ptoid        as _cod,
			pt.ptoid_pai    as _codpai,
			a.unicod,
			a.prgcod,
			a.acacod,
			a.loccod,
			a.acadsc,
			pt.ptodsc       as atidescricao,
			pt.ptodescricao as atidetalhamento,
			pt.ptodata_ini  as atidatainicio,
			pt.ptodata_fim  as atidatafim,
			pt.usucpf       as usucpf,
			pt.acaid        as acaid
		from monitora.planotrabalho pt
			inner join monitora.acao a on a.acaid = pt.acaid
		where
			pt.ptostatus = 'A' and
			pt.acaid = $acaid
		order by
			pt.ptoid, pt.ptoid_pai
	";
	$atividades = $db->carregar( $sql );
	$atiids = array();
	foreach ( $atividades as $atividade )
	{
		// tenta pegar pai da lista de inseridos
		$atividade["atiidpai"] = false;
		if ( array_key_exists( $atividade["_codpai"], $atiids ) )
		{
			$atividade["atiidpai"] = $atiids[$atividade["_codpai"]];
		}
		//dbg( $atividade["atiidpai"] );
		// tenta pegar de novo... dessa vez é filho do raiz
		if ( !$atividade["atiidpai"] )
		{
			// pega id da atividade que representa a ação
			$nomeAcao =
				$atividade["unicod"] . "." . $atividade["prgcod"] . "." . 
				$atividade["acacod"] . "." . $atividade["loccod"] . " " .
				$atividade["acadsc"];
			$atividade["atiidpai"] = pegarIdTarefaAcao(
				$atividade["acaid"],
				$nomeAcao
			);
		}
		//dbg( $atividade["atiidpai"] );
		// formata data inicio
		if ( !$atividade["atidatainicio"] )
		{
			$atividade["atidatainicio"] = "null";
		}
		else
		{
			$atividade["atidatainicio"] = "'" . $atividade["atidatainicio"] . "'";
		}
		// formata data fim
		if ( !$atividade["atidatafim"] )
		{
			$atividade["atidatafim"] = "null";
		}
		else
		{
			$atividade["atidatafim"] = "'" . $atividade["atidatafim"] . "'";
		}
		// previne problema de aspas
		$atividade["atidescricao"] = str_replace( "'", "\\'", $atividade["atidescricao"] );
		$atividade["atidetalhamento"] = str_replace( "'", "\\'", $atividade["atidetalhamento"] );
		$sql = sprintf(
			$sqlInsert,
				$atividade["atidescricao"],
				$atividade["atidetalhamento"],
				$atividade["atidatainicio"],
				$atividade["atidatafim"],
				"'" . $atividade["usucpf"] . "'",
				1, // estado
				0, // porcentagem de execucao
				$atividade["acaid"],
				$atividade["atiidpai"],
				1, // atiordem
				PROJETO
		);
		//dbg( $sql, 1 );
		$atiids[$atividade["_cod"]] = (integer) $db->pegaUm( $sql );
		//echo $sql . "\n\n";
	}
}

function atividade_calcular_dados( $atividade )
{
	if ( !$atividade )
	{
		return;
	}
	global $db;
	// pega dados da atividade
	$atividade = (integer) $atividade;
	$sql = "select _atiordem, _atinumero, _atiprofundidade, _atiprojeto from pde.atividade where atiid = " . $atividade;
	$pai = $db->recuperar( $sql );
	
	// pega filhos
	$sql = "select atiid, atiordem from pde.atividade where atiidpai = " . $atividade . " and atistatus = 'A'";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	$sql = "update pde.atividade set _atifilhos = " . count( $filhos ) . " where atiid = " . $atividade;
	//echo $sql . ";\n";
	$db->executar( $sql, false );
	
	// atualiza filhos
	foreach ( $filhos as $filho )
	{
		$_atinumero  = ( $pai['_atinumero'] ? $pai['_atinumero'] . "." : '' ) . $filho['atiordem'];
		$_atiordem   = ( $pai['_atiordem'] ? $pai['_atiordem'] : '' ) . sprintf( '%04d', $filho['atiordem'] );
		$_atiprojeto = (integer) $pai['_atiprojeto']; 
		$sql = "update pde.atividade set _atinumero = '" . $_atinumero . "', _atiordem = '" . $_atiordem . "', _atiprofundidade = " . ( $pai['_atiprofundidade'] + 1 ) . ", _atiirmaos = " . count( $filhos ) . ", _atiprojeto = " . $_atiprojeto . " where atiid = " . $filho['atiid'];
		//echo $sql . ";\n\n";
		$db->executar( $sql, false );
		atividade_calcular_dados( $filho['atiid'] );
	}
}

function corrige( $atiid )
{
	if ( !$atiid )
	{
		return;
	}
	global $db;
	$sql = "
		select a.atiid
		from pde.atividade a
		where
			a.atistatus = 'A' and
			a.atiidpai = $atiid
		order by
			a.atiordem
	";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	$quantidade = count( $filhos );
	$numero = 1;
	foreach ( $filhos as $filho )
	{
		$atiidfilho = $filho['atiid'];
		$sql = "update pde.atividade set atiordem = $numero where atiid = $atiidfilho";
		$db->executar( $sql, false );
		//echo $sql . ";\n\n";
		corrige( $atiidfilho );
		$numero++;
	}
}
/*
$sql = "update pde.atividade set esaid = 1 where esaid is null";
$db->executar( $sql, false );
$sql = "update pde.atividade set atiporcentoexec = 0 where atiporcentoexec is null";
$db->executar( $sql, false );
*/
corrige( PROJETO );
atividade_calcular_dados( PROJETO );

$db->commit();
echo "\n\nterminou! " . PROJETO . "\n" . date( "d/m/Y H:i:s" );

//$db->rollback();
$db->commit();








