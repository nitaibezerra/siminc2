<?
include "config.inc";
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Pragma: no-cache");
header('Content-Type: text/html; charset=iso-8859-1');

include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
	
$db = new cls_banco();

//error_reporting( E_ALL );

function geraFiltroPorTiposdePropostas( $arrTiposdePropostas , $strNomeTabela )
{
	$arrTiposBancoProposta = array();
	
	if( $arrTiposdePropostas[ 'inclusao' ] )
	{
		$arrTiposBancoProposta[] = '\'I\'';
	}
	if( $arrTiposdePropostas[ 'alteracao' ] )
	{
		$arrTiposBancoProposta[] = '\'A\'';
	}
	if( $arrTiposdePropostas[ 'exclusao' ] )
	{
		$arrTiposBancoProposta[] = '\'E\'';
	}
	if( $arrTiposdePropostas[ 'fusao' ] )
	{
		$arrTiposBancoProposta[] = '\'F\'';
	}
	if( $arrTiposdePropostas[ 'migracao' ] )
	{
		$arrTiposBancoProposta[] = '\'M\'';
	}
	
	if( sizeof( $arrTiposBancoProposta ) > 0 )
	{
		$strWhereAppend = '' .
			$strNomeTabela . '.' . 'tipo' . 
			' IN ' .
			' ( ' . 
				implode( ',' , $arrTiposBancoProposta ) . 
			' ) ';
	}
	else
	{
		$strWhereAppend = '';
	}	
	
	return $strWhereAppend;
}

function geraListagemDePropostasDaAcao( $intAcaoId , $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio )
{
	global $db;
	
	$strSqlWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	
	if( $strSqlWhereAppend != '' )
	{
		$strSqlWhereAppend = ' AND ' . $strSqlWhereAppend;
	}

	
	$strSql = '' .
	' SELECT ' .
		'acao_id' .
		' ,  ' .
		'tipo' .
		' , '. 
		'Usuario.usunome' . ' AS ' . 'usuario_nome' . 
	' FROM ' . 
		' ( ' .
	// fusão //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' .
			' , ' .
			'\'F\'' . ' AS ' . 'tipo' .
			' , ' .
	 		'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' .
	 		'elabrev.proposta_fusao_acao' . 
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// migração //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'M\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
 		' FROM ' .
			'elabrev.proposta_migracao_acao' .
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// alteração //
		' SELECT ' .
			'eracod' . ' AS ' . 'acao_id' . 
			' , ' .
	 		'\'A\'' . ' AS ' . 'tipo' .
			' , ' .
	 		'usucpf' . ' AS ' . 'usuario_cpf' . 
 		' FROM ' .
	 		'elabrev.elaboracaorevisao' . 
		' WHERE ' .
	 		'eratabela' . ' = ' . '\'ppaacao_proposta\'' . 
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// criação //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'I\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' .
			'elabrev.ppaacao_proposta' . 
	 	' WHERE ' .
			'acastatus' . ' = ' . '\'' . 'N' . '\'' .
		' GROUP ' . ' BY ' .
			'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' .
	 	' UNION ' . ' ALL ' .
	// exclusão //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'E\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' .
			'elabrev.proposta_exclusao_acao' .
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
	 	' ) ' . ' AS ' . 'Propostas' .
	' JOIN ' .
		'seguranca.usuario' . ' AS ' . 'Usuario' . 
	' ON ' .
		'Usuario.usucpf' . ' = ' . 'usuario_cpf' .
	' LEFT ' . ' JOIN  ' .
		'elabrev.ppaacao_proposta' . ' AS ' . 'Acoes' .
	' ON ' . 
		'Acoes.acaid' . ' = ' . 'Propostas.acao_id' .
	' WHERE ' .
		'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
	' AND '.
	 	'acao_id' . ' = ' . $intAcaoId .
		 $strSqlWhereAppend .
	' ORDER ' . ' BY ' .
		'tipo' .
		' , ' .
		'usuario_nome' .
	'';
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	$arrAbreviacoesDoTipo = array
	( 
		'A' => 'Alteração'	,	 
		'I' => 'Inclusão'	, 
		'E' => 'Exclusão' 	, 
		'F' => 'Fusão' 		,
		'M' => 'Migração'
	);
	
	$arrPropostas = array();
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
		
		$arrProposta = array();
		
		$arrProposta[ 'acao_id' ] 					= $objRegistroAtual[ 'acao_id' ];
		$arrProposta[ 'proposta_tipo' ]				= $arrAbreviacoesDoTipo[ $objRegistroAtual[ 'tipo' ] ];
		$arrProposta[ 'usuario_nome' ]				= ucwords( strtolower( $objRegistroAtual[ 'usuario_nome' ] ) );
		
		$arrProposta[ 'quantidade_alteracao' ]		= 0;
		$arrProposta[ 'quantidade_fusao' ]			= 0;
		$arrProposta[ 'quantidade_migracao' ]		= 0;
		$arrProposta[ 'quantidade_inclusao' ]		= 0;
		$arrProposta[ 'quantidade_exclusao' ]		= 0;
		$arrProposta[ 'quantidade_total' ]			= 1;
		
		switch( $arrProposta[ 'proposta_tipo' ] )
		{
			case 'Fusão':
			{
				$arrProposta[ 'quantidade_fusao' ] = 1;
				break;
			}
			case 'Alteração':
			{
				$arrProposta[ 'quantidade_alteracao' ] = 1;
				break;
			}
			case 'Migração' :
			{
				$arrProposta[ 'quantidade_migracao' ] = 1;
				break;
			}
			case 'Inclusão':
			{
				$arrProposta[ 'quantidade_inclusao' ] = 1;
				break;
			}
			case 'Exclusão':
			{
				$arrProposta[ 'quantidade_exclusao' ] = 1;
				break;
			}
			default:
			{
				$arrProposta[ 'quantidade_total' ] = 0;
			}
		}
		$arrPropostas[] = $arrProposta;
	}
	
	return $arrPropostas;
}

function geraListagemDePropostasDeProgramaDoPrograma( $intProgramaId , $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio )
{
	global $db;
	
	$strSqlWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	
	if( $strSqlWhereAppend != '' )
	{
		$strSqlWhereAppend = ' AND ' . $strSqlWhereAppend;
	}
	
	$strSql = '' .
	' SELECT ' .
		'programa_id' .
		' , ' .
		'Programas.prgcod' . ' AS ' . 'programa_codigo' .
		' , ' .
		'Programas.prgdsc' . ' AS ' . 'prgograma_descricao' .
		' ,  ' .
		'tipo' .
		' , ' .
		'Usuario.usunome' . ' AS ' . 'usuario_nome' .	
	' FROM ' .
		' ( ' .
		// nao existe tabela de fusão de programas //
		// nao existe tabela de migração de programas //
		// alteração //
		' SELECT ' . 
			'eracod' . ' AS ' . 'programa_id' .
			' , ' . 
			'\'A\'' . ' AS ' . 'tipo' .
			' , ' . 
			'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' . 
			'elabrev.elaboracaorevisao' . 
		' WHERE ' .
			'eratabela' . ' = ' . '\'ppaprograma_proposta\'' .
		' GROUP ' . ' BY ' .
			'programa_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' .
		' UNION ' . ' ALL ' .
		// inclusão //
		' SELECT ' .
			'prgid' . ' AS ' . 'programa_id' .
			' , ' . 
			'\'I\'' . ' AS ' . 'tipo' .
			' , ' . 
			'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' .
			'elabrev.ppaprograma_proposta' . 
		' WHERE ' .
			'prgstatus' . ' = ' . '\'' . 'N' . '\'' .
		' GROUP ' . ' BY ' .
			'programa_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' .
		' ) ' .
	' AS ' .
		'Propostas' .
	' LEFT ' . ' JOIN  ' .
		'elabrev.ppaprograma_proposta' . ' AS ' . 'Programas' .
	' ON ' . 
		'Programas.prgid' . ' = ' . 'Propostas.programa_id' .
	' JOIN ' .
		'seguranca.usuario' . ' AS ' . 'Usuario' . 
	' ON ' .
		'Usuario.usucpf' . ' = ' . 'usuario_cpf' .
	' WHERE ' .
		'Programas.prsano' . ' = ' . $intAnoExercicio .
	' AND '.
		'programa_id' . ' = ' . $intProgramaId .
		$strWhereAppend .
	' ORDER ' . ' BY ' .
		'tipo' .
		' , ' .
		'usuario_nome' .
	'';
		
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	$arrAbreviacoesDoTipo = array
	( 
		'A' => 'Alteração'	,	 
		'I' => 'Inclusão'	, 
		'E' => 'Exclusão' 	, 
		'F' => 'Fusão' 		,
		'M' => 'Migração'
	);
	
	$arrPropostas = array();
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
		
		$arrProposta = array();
		
		$arrProposta[ 'programa_id' ] 				= $objRegistroAtual[ 'programa_id' ];
		$arrProposta[ 'proposta_tipo' ]				= $arrAbreviacoesDoTipo[ $objRegistroAtual[ 'tipo' ] ];
		$arrProposta[ 'quantidade_alteracao' ]		= 0;
		$arrProposta[ 'quantidade_fusao' ]			= 0;
		$arrProposta[ 'quantidade_migracao' ]		= 0;
		$arrProposta[ 'quantidade_inclusao' ]		= 0;
		$arrProposta[ 'quantidade_exclusao' ]		= 0;
		$arrProposta[ 'quantidade_total' ]			= 1;
		
		switch( $arrProposta[ 'proposta_tipo' ] )
		{
			case 'Fusão':
			{
				$arrProposta[ 'quantidade_fusao' ] = 1;
				break;
			}
			case 'Alteração':
			{
				$arrProposta[ 'quantidade_alteracao' ] = 1;
				break;
			}
			case 'Migração' :
			{
				$arrProposta[ 'quantidade_migracao' ] = 1;
				break;
			}
			case 'Inclusão':
			{
				$arrProposta[ 'quantidade_inclusao' ] = 1;
				break;
			}
			case 'Exclusão':
			{
				$arrProposta[ 'quantidade_exclusao' ] = 1;
				break;
			}
			default:
			{
				$arrProposta[ 'quantidade_total' ] = 0;
			}
		}
		
		$arrProposta[ 'usuario_nome' ]				= ucwords( strtolower( $objRegistroAtual[ 'usuario_nome' ] ) );
		$arrProposta[ 'proposta_justificativa' ]	= $objRegistroAtual[ 'proposta_justificativa' ];
		
		$arrPropostas[] = $arrProposta;
	}
	
	return $arrPropostas;
}

function geraListagemDeAcoesComPropostasDoPrograma( $intProgramaId , $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio , $boolIncluiAcoesSemPropostas = false )
{
	global $db;
	
	$strSqlWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	
	if( $strSqlWhereAppend != '' )
	{
		$strSqlWhereAppend = ' AND ' . $strSqlWhereAppend;
	}
	
	$strSql = '';
	
	if( $boolIncluiAcoesSemPropostas )
	{
		$strSql .= '' .  
		' SELECT ' .
			'Acoes.acadsc'	. ' AS ' . 'acao_descricao'	. 
			' , ' . 
			'Acoes.acaid'	. ' AS ' . 'acao_id'		. 
			' , ' .
			'Acoes.acacod'	. ' AS ' . 'acao_codigo'	. 
			' , ' .
			'Acoes.prgid'	. ' AS ' . 'programa_id'	. 
			' , ' .
			'quantidade_fusao' . 
			' , ' .
			'quantidade_migracao' . 
			' , ' .
			'quantidade_alteracao' . 
			' , ' .
			'quantidade_inclusao' .
			' , ' .
			'quantidade_exclusao' .
		' FROM '. 
			'elabrev.ppaacao_proposta' . ' AS  ' . 'Acoes' . 
		' LEFT JOIN ' .
			' ( ' .
		''; 
	}
		
	$strSql .= '' .
	' SELECT ' .
		'acao_id' .
		' , ' .
		'Acoes.acacod' 	. ' AS ' . 'acao_codigo' .
		' , ' .
		'Acoes.acadsc'	. ' AS ' . 'acao_descricao'	. 
		' , ' .
		' SUM( ' . 'quantidade_fusao' 		. ' ) ' . ' AS ' . 'quantidade_fusao' .
		' , ' .
		' SUM( ' . 'quantidade_migracao' 	. ' ) '	. ' AS ' . 'quantidade_migracao' .
		' , ' .
		' SUM( ' . 'quantidade_alteracao'  	. ' ) '	. ' AS ' . 'quantidade_alteracao' .
		' , ' .
		' SUM( ' . 'quantidade_inclusao'  	. ' ) '	. ' AS ' . 'quantidade_inclusao' .
		' , ' .
		' SUM( ' . 'quantidade_exclusao'  	. ' ) '	. ' AS ' . 'quantidade_exclusao' .
	' FROM ' . 
		' ( ' .
	// fusão //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' .
			' , ' .
			'\'F\'' . ' AS ' . 'tipo' .
			' , ' .
	 		'usucpf' . ' AS ' . 'usuario_cpf' .
			' , ' .
	 		'1' . ' AS ' . 'quantidade_fusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_migracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_alteracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_inclusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_exclusao' .	 		
		' FROM ' .
	 		'elabrev.proposta_fusao_acao' . 
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// migração //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'M\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
			' , ' .
	 		'0' . ' AS ' . 'quantidade_fusao' .
	 		' , ' .
	 		'1' . ' AS ' . 'quantidade_migracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_alteracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_inclusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_exclusao' .			
 		' FROM ' .
			'elabrev.proposta_migracao_acao' .
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// alteração //
		' SELECT ' .
			'eracod' . ' AS ' . 'acao_id' . 
			' , ' .
	 		'\'A\'' . ' AS ' . 'tipo' .
			' , ' .
	 		'usucpf' . ' AS ' . 'usuario_cpf' . 
			' , ' .
	 		'0' . ' AS ' . 'quantidade_fusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_migracao' .
	 		' , ' .
	 		'1' . ' AS ' . 'quantidade_alteracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_inclusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_exclusao' .
		' FROM ' .
	 		'elabrev.elaboracaorevisao' . 
		' WHERE ' .
	 		'eratabela' . ' = ' . '\'ppaacao_proposta\'' . 
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// criação //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'I\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
			' , ' .
	 		'0' . ' AS ' . 'quantidade_fusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_migracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_alteracao' .
	 		' , ' .
	 		'1' . ' AS ' . 'quantidade_inclusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_exclusao' .
		' FROM ' .
			'elabrev.ppaacao_proposta' . 
	 	' WHERE ' .
			'acastatus' . ' = ' . '\'' . 'N' . '\'' .
		' GROUP ' . ' BY ' .
			'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' .
	 	' UNION ' . ' ALL ' .
	// exclusão //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'E\'' . ' AS ' . 'tipo' .  
			' , ' .
	 		'usucpf' . ' AS ' . 'usuario_cpf' .
			' , ' .
	 		'0' . ' AS ' . 'quantidade_fusao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_migracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_alteracao' .
	 		' , ' .
	 		'0' . ' AS ' . 'quantidade_inclusao' .
	 		' , ' .
	 		'1' . ' AS ' . 'quantidade_exclusao' .
		' FROM ' .
			'elabrev.proposta_exclusao_acao' .
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
	 	' ) ' . ' AS ' . 'Propostas' .
	' LEFT ' . ' JOIN  ' .
		'elabrev.ppaacao_proposta' . ' AS ' . 'Acoes' .
	' ON ' . 
		'Acoes.acaid' . ' = ' . 'Propostas.acao_id' .
	' WHERE ' .
		'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
	' AND '.
	 	'Acoes.prgid' . ' = ' . $intProgramaId .
		 $strSqlWhereAppend .
	' GROUP BY ' .
		'acao_id' . 
		' , '.
		'Acoes.acacod' .
		' , '.
		'Acoes.acadsc' .
	'';
		
	if( $boolIncluiAcoesSemPropostas )
	{
		$strSql .= '' .
			' ) ' . ' AS '. 'AcoesComPropostas' .
		' ON ' .
			'Acoes.acaid' . ' = ' . 'AcoesComPropostas.acao_id' .
		' WHERE ' .
			'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
		' AND '.
	 		'Acoes.prgid' . ' = ' . $intProgramaId .
		'';
	}

	$_SESSION[ "debugger" ] = $strSql ;
//	dbg( $strSql, 1 );
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	$arrAbreviacoesDoTipo = array
	( 
		'A' => 'Alteração'	,	 
		'I' => 'Inclusão'	, 
		'E' => 'Exclusão' 	, 
		'F' => 'Fusão' 		,
		'M' => 'Migração'
	);
	
	$arrAcoes = array();
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
		
		$intAcaoId = $objRegistroAtual[ 'acao_id' ];
		
		if( !isset( $arrAcoes[ $intAcaoId ] ) )
		{
			$arrAcao = array();
			$arrAcao[ 'acao_id' ] 					= $intAcaoId;
			$arrAcao[ 'acao_descricao' ] 			= $objRegistroAtual[ 'acao_descricao' ];
			$arrAcao[ 'acao_codigo' ]				= $objRegistroAtual[ 'acao_codigo' ];
			$arrAcao[ 'quantidade_alteracao' ]		= (integer)@$objRegistroAtual[ 'quantidade_alteracao' ];
			$arrAcao[ 'quantidade_fusao' ]			= (integer)@$objRegistroAtual[ 'quantidade_fusao' ];
			$arrAcao[ 'quantidade_migracao' ]		= (integer)@$objRegistroAtual[ 'quantidade_migracao' ];
			$arrAcao[ 'quantidade_inclusao' ]		= (integer)@$objRegistroAtual[ 'quantidade_inclusao' ];
			$arrAcao[ 'quantidade_exclusao' ]		= (integer)@$objRegistroAtual[ 'quantidade_exclusao' ];
			$arrAcao[ 'quantidade_total' ]			= $arrAcao[ 'quantidade_alteracao' ] + $arrAcao[ 'quantidade_fusao' ] +	
				$arrAcao[ 'quantidade_migracao' ] +	$arrAcao[ 'quantidade_inclusao' ] +	$arrAcao[ 'quantidade_exclusao' ] ;
			$arrAcao[ 'programa_id' ] 				= $intProgramaId;
			
		}
		$arrAcoes[ $intAcaoId ] = $arrAcao;
	}
	
	/*
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
		
		$intAcaoId = $objRegistroAtual[ 'acao_id' ];
		
		if( !isset( $arrAcoes[ $intAcaoId ] ) )
		{
			$arrAcao = array();
			$arrAcao[ 'acao_id' ] 					= $intAcaoId;
			$arrAcao[ 'acao_descricao' ] 			= $objRegistroAtual[ 'acao_descricao' ];
			$arrAcao[ 'acao_codigo' ]				= $objRegistroAtual[ 'acao_codigo' ];
			$arrAcao[ 'programa_id' ] 				= $intProgramaId;
			$arrAcao[ 'proponentes_nomes' ]			= array();
		}
		else
		{
			$arrAcao = $arrAcoes[ $intAcaoId ];
		}
		$strProponenteNome = $objRegistroAtual[ 'usuario_nome' ];
		$strProponenteCPF = $objRegistroAtual[ 'usuario_cpf' ];
		$arrAcao[ 'proponentes_nomes' ][ $strProponenteCPF ]		= ucwords( strtolower( $strProponenteNome ) );
		
		$arrAcoes[ $intAcaoId ] = $arrAcao;
	}
	*/
	return $arrAcoes;
}

function geraListagemDeAcoesComPropostasDaUnidade( $intUnidadeId , $strUnidadeCodigo , $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio )
{
	global $db;
	
	$strSqlWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'Propostas' );
	
	if( $strSqlWhereAppend != '' )
	{
		$strSqlWhereAppend = ' AND ' . $strSqlWhereAppend;
	}
	
	$strSql = '' .
	' SELECT ' .
		'acao_id' .
		' ,  ' .
		'Acoes.acacod' . ' AS ' . 'acao_codigo' .
		' ,  ' .
		'Acoes.acadsc' . ' AS ' . 'acao_descricao' .
		' ,  ' .
		'Usuario.usunome' . ' AS ' . 'usuario_nome' . 
		' ,  ' .
		'usuario_cpf' .
	' FROM ' . 
		' ( ' .
	// fusão //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' .
			' , ' .
			'\'F\'' . ' AS ' . 'tipo' .
			' , ' .
	 		'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' .
	 		'elabrev.proposta_fusao_acao' . 
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// migração //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'M\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
 		' FROM ' .
			'elabrev.proposta_migracao_acao' .
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// alteração //
		' SELECT ' .
			'eracod' . ' AS ' . 'acao_id' . 
			' , ' .
	 		'\'A\'' . ' AS ' . 'tipo' .
			' , ' .
	 		'usucpf' . ' AS ' . 'usuario_cpf' . 
 		' FROM ' .
	 		'elabrev.elaboracaorevisao' . 
		' WHERE ' .
	 		'eratabela' . ' = ' . '\'ppaacao_proposta\'' . 
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
		' UNION ' . ' ALL ' .
	// criação //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'I\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' .
			'elabrev.ppaacao_proposta' . 
	 	' WHERE ' .
			'acastatus' . ' = ' . '\'' . 'N' . '\'' .
		' GROUP ' . ' BY ' .
			'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' .
	 	' UNION ' . ' ALL ' .
	// exclusão //
		' SELECT ' .
			'acaid' . ' AS ' . 'acao_id' . 
			' , '.
			'\'E\'' . ' AS ' . 'tipo' .  
			' , ' .
			'usucpf' . ' AS ' . 'usuario_cpf' .
		' FROM ' .
			'elabrev.proposta_exclusao_acao' .
	 	' GROUP ' . ' BY ' .
	 		'acao_id' .
			' , ' .
			'usuario_cpf' .
			' , ' .
			'tipo' . 
	 	' ) ' . ' AS ' . 'Propostas' .
	' JOIN ' .
		'seguranca.usuario' . ' AS ' . 'Usuario' . 
	' ON ' .
		'Usuario.usucpf' . ' = ' . 'usuario_cpf' .
	' LEFT ' . ' JOIN  ' .
		'elabrev.ppaacao_proposta' . ' AS ' . 'Acoes' .
	' ON ' . 
		'Acoes.acaid' . ' = ' . 'Propostas.acao_id' .
	' LEFT ' . ' JOIN  ' .
		'elabrev.unidade_acao' . ' AS ' . 'UnidadeAcao' .
	' ON ' . 
		'UnidadeAcao.acaid' . ' = ' . 'Propostas.acao_id' .
	' WHERE ' .
		'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
	' AND '.
	 	'UnidadeAcao.unicod' . ' = ' . '\'' . $strUnidadeCodigo . '\'' .
		 $strSqlWhereAppend .
	' ORDER ' . ' BY ' .
		'tipo' .
		' , ' .
		'usuario_nome' .
		' , ' .
		'usuario_cpf' .
	'';
	
	$_SESSION[ "debugger" ] = $strSql ;
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	$arrAbreviacoesDoTipo = array
	( 
		'A' => 'Alteração'	,	 
		'I' => 'Inclusão'	, 
		'E' => 'Exclusão' 	, 
		'F' => 'Fusão' 		,
		'M' => 'Migração'
	);
	
	$arrAcoes = array();
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
		
		$intAcaoId = $objRegistroAtual[ 'acao_id' ];
		
		if( !isset( $arrAcoes[ $intAcaoId ] ) )
		{
			$arrAcao = array();
			$arrAcao[ 'acao_id' ] 					= $intAcaoId;
			$arrAcao[ 'acao_descricao' ] 			= $objRegistroAtual[ 'acao_descricao' ];
			$arrAcao[ 'acao_codigo' ]				= $objRegistroAtual[ 'acao_codigo' ];
			$arrAcao[ 'unidade_id' ] 				= $intUnidadeId;
			$arrAcao[ 'proponentes_nomes' ]			= array();
		}
		else
		{
			$arrAcao = $arrAcoes[ $intAcaoId ];
		}
		$strProponenteNome = $objRegistroAtual[ 'usuario_nome' ];
		$strProponenteCPF = $objRegistroAtual[ 'usuario_cpf' ];
		$arrAcao[ 'proponentes_nomes' ][ $strProponenteCPF ]		= ucwords( strtolower( $strProponenteNome ) );
		
		$arrAcoes[ $intAcaoId ] = $arrAcao;
	}
return $arrAcoes;
}

function geraListagemDePropostasDeAcaoDaUnidade( $intUnidadeId , $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio )
{
	global $db;
	
	$strSqlWhereAppend = geraFiltroPorTiposdePropostas( $arrTiposdePropostas , 'PropostasPorTipo' );
	
	if( $strSqlWhereAppend != '' )
	{
		$strSqlWhereAppend = ' AND ' . $strSqlWhereAppend;
	}
	
	$strSql = '' .
	' SELECT ' .
		'UnidadeAcao.unicod' .	' AS ' . 'unidade_codigo' . 
		' , ' .
		'Unidade.uniid' .		' AS ' . 'unidade_id' . 
		' , ' .
		'Usuario.usunome' . ' AS ' . 'usuario_nome' .	
		' , ' .
		'tipo' .
	' FROM ' . 
		' ( ' .	
			' SELECT ' .
				'acao_id' .
				' , ' .
				'tipo' . 
				' , ' .
				'usuario_cpf' .
			' FROM ' . 
				' ( ' .	
					' SELECT ' . 
						'acaid' . ' AS ' . 'acao_id' .
						' , ' .
						'\'F\'' . ' AS ' . 'tipo' .
						' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' .
					' FROM ' . 
						'elabrev.proposta_fusao_acao' .
					' GROUP ' . ' BY ' . 
						'acao_id' . 
						' , ' .
						'usuario_cpf' . 
						' , ' .
						'tipo' . 
					' UNION ' . ' ALL ' . 
					
					' SELECT ' .
						'acaid' . ' AS ' . 'acao_id' . 
						' , ' .
						'\'M\'' .  ' AS ' . 'tipo' . 
						' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' .
					' FROM ' .
						'elabrev.proposta_migracao_acao' .
					' GROUP ' . ' BY ' .
						'acao_id' .
						' , ' .
						'usuario_cpf' .
						' , ' .
						'tipo' .
					' UNION ' . ' ALL ' .
					
					' SELECT ' .
						'eracod' . ' AS ' . 'acao_id' . 
						' , ' .
						'\'A\'' . ' AS ' . 'tipo' . 
						' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' . 
					' FROM ' . 
						'elabrev.elaboracaorevisao' . 
					' WHERE ' . 
						'eratabela' . ' = ' . '\'ppaacao_proposta\'' .
					' GROUP ' . ' BY ' . 
						'acao_id' . ' , ' .
						'usuario_cpf' . ' , ' .
						'tipo' .
					' UNION ' . ' ALL ' . 
					
					' SELECT ' .
						'acaid' . ' AS ' . 'acao_id' . ' , ' .
						'\'I\'' . ' AS ' . 'tipo' . ' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' .
					' FROM ' . 
						'elabrev.ppaacao_proposta' . 
					' WHERE ' .  
						'acastatus' . ' = ' . '\'N\'' . 
					' GROUP ' . ' BY ' . 
						'acao_id' . 
						' , ' .
						'usuario_cpf' . 
						' , ' .
						'tipo' . 
					' UNION ' . ' ALL ' . 
					
					' SELECT ' .
						'acaid' . ' AS ' . 'acao_id' . 
						' , ' .
						'\'E\''  . ' AS ' .  'tipo' .
						' , ' .
						'usucpf' . ' AS ' . 'usuario_cpf' .
					' FROM ' . 
						'elabrev.proposta_exclusao_acao' . 
					' GROUP ' . ' BY ' . 
						'acao_id' . 
						' , ' .
						'usuario_cpf' . 
						' , ' .
						'tipo' . 
				' ) ' . ' AS ' . 'Propostas' . 
			' LEFT ' . ' JOIN ' .
				'elabrev.ppaacao_proposta' . ' AS ' . 'Acoes' . 
			' ON ' .
				'Acoes.acaid' . ' = ' . 'Propostas.acao_id' .
			' WHERE ' .
				'Acoes.prsano' . ' = ' . "'$intAnoExercicio'" .
			' GROUP ' . ' BY ' .
				'acao_id' . 
				' , ' .
				'tipo' . 
				' , ' .	
				'usuario_cpf' .
		' ) ' . ' AS ' . 'PropostasPorTipo' . 
	' JOIN ' .
		'elabrev.unidade_acao' . ' AS ' . 'UnidadeAcao' . 
	' ON ' .
		'UnidadeAcao.acaid' . ' = ' . 'PropostasPorTipo.acao_id' . 
	' JOIN ' .
		'public.unidade' . ' AS ' . 'Unidade' .
	' ON ' .
		'Unidade.unicod' . ' = ' . 'UnidadeAcao.unicod' . 
	' JOIN ' .
		'seguranca.usuario' . ' AS ' . 'Usuario' .
	' ON ' .
		'Usuario.usucpf' . ' = ' . 'usuario_cpf' .
	' WHERE ' .
		'Unidade.uniid' .  ' = ' . $intUnidadeId .
		$strSqlWhereAppend .
	' ORDER ' . ' BY ' .
		'tipo' .
		' , ' .
		'usuario_nome' .
	'';
	
	$_SESSION[ "debugger" ] = $strSql ;
	
	$objResultSet		= $db->record_set( $strSql );
	$intNumerodeLinhas	= $db->conta_linhas( $objResultSet );
	
	$arrAbreviacoesDoTipo = array
	( 
		'A' => 'Alteração'	,	 
		'I' => 'Inclusão'	, 
		'E' => 'Exclusão' 	, 
		'F' => 'Fusão' 		,
		'M' => 'Migração'
	);
	
	$arrPropostas = array();
	
	for ( $intLinhaAtual = 0 ; $intLinhaAtual <= $intNumerodeLinhas ; $intLinhaAtual++ )
	{
		
		$objRegistroAtual = $db->carrega_registro( $objResultSet , $intLinhaAtual );
		
		$arrProposta = array();
		
		$arrProposta[ 'unidade_id' ] 				= $objRegistroAtual[ 'unidade_id' ];
		$arrProposta[ 'proposta_tipo' ]				= $arrAbreviacoesDoTipo[ $objRegistroAtual[ 'tipo' ] ];
		$arrProposta[ 'quantidade_alteracao' ]		= 0;
		$arrProposta[ 'quantidade_fusao' ]			= 0;
		$arrProposta[ 'quantidade_migracao' ]		= 0;
		$arrProposta[ 'quantidade_inclusao' ]		= 0;
		$arrProposta[ 'quantidade_exclusao' ]		= 0;
		$arrProposta[ 'quantidade_total' ]			= 1;
		
		switch( $arrProposta[ 'proposta_tipo' ] )
		{
			case 'Fusão':
			{
				$arrProposta[ 'quantidade_fusao' ] = 1;
				break;
			}
			case 'Alteração':
			{
				$arrProposta[ 'quantidade_alteracao' ] = 1;
				break;
			}
			case 'Migração' :
			{
				$arrProposta[ 'quantidade_migracao' ] = 1;
				break;
			}
			case 'Inclusão':
			{
				$arrProposta[ 'quantidade_inclusao' ] = 1;
				break;
			}
			case 'Exclusão':
			{
				$arrProposta[ 'quantidade_exclusao' ] = 1;
				break;
			}
			default:
			{
				$arrProposta[ 'quantidade_total' ] = 0;
			}
		}
		
		$arrProposta[ 'usuario_nome' ]				= ucwords( strtolower( $objRegistroAtual[ 'usuario_nome' ] ) );
		
		$arrPropostas[] = $arrProposta;
	}
	
	return $arrPropostas;
	
}

function geraLinkAcao( $strDiretorioSistema , $intAcaoId , $arrTiposdePropostas )
{
	$strLink = $strDiretorioSistema . '.php?modulo=relatorio/acao/geraproposta' .
	'&' . 'acao' 	. '=' . 'A' .
	'&' . 'acaid' 	. '=' . $intAcaoId .
	'&' . 'cbinclusao'		. '=' . (int)$arrTiposdePropostas[ 'inclusao' ] .
	'&' . 'cbexclusao'		. '=' . (int)$arrTiposdePropostas[ 'exclusao' ] .
	'&' . 'cbalteracao'		. '=' . (int)$arrTiposdePropostas[ 'alteracao' ] .
	'&' . 'cbfusao'			. '=' . (int)$arrTiposdePropostas[ 'fusao' ] .
	'&' . 'cbmigracao'		. '=' . (int)$arrTiposdePropostas[ 'migracao' ] .
	'';
	
	return $strLink;
}

function geraLinkPrograma( $strDiretorioSistema , $strCod , $arrTiposdePropostas )
{
	$strLink = $strDiretorioSistema . '.php?modulo=relatorio/acao/geraproposta' .
	'&' . 'acao' 	. '=' . 'A' .
	'&' . 'prgid' 	. '=' . $strCod .
	'&' . 'cbinclusao'		. '=' . (int)$arrTiposdePropostas[ 'inclusao' ] .
	'&' . 'cbexclusao'		. '=' . (int)$arrTiposdePropostas[ 'exclusao' ] .
	'&' . 'cbalteracao'		. '=' . (int)$arrTiposdePropostas[ 'alteracao' ] .
	'&' . 'cbfusao'			. '=' . (int)$arrTiposdePropostas[ 'fusao' ] .
	'&' . 'cbmigracao'		. '=' . (int)$arrTiposdePropostas[ 'migracao' ] .
		'';
	
	return $strLink;
}

function geraLinkUnidade( $strDiretorioSistema , $strCod , $arrTiposdePropostas )
{
	$strLink = $strDiretorioSistema . '.php?modulo=relatorio/acao/geraproposta' .
	'&' . 'acao' 	. '=' . 'A' .
	'&' . 'unicod' 	. '=' . $strCod .
	'&' . 'cbinclusao'		. '=' . (int)$arrTiposdePropostas[ 'inclusao' ] .
	'&' . 'cbexclusao'		. '=' . (int)$arrTiposdePropostas[ 'exclusao' ] .
	'&' . 'cbalteracao'		. '=' . (int)$arrTiposdePropostas[ 'alteracao' ] .
	'&' . 'cbfusao'			. '=' . (int)$arrTiposdePropostas[ 'fusao' ] .
	'&' . 'cbmigracao'		. '=' . (int)$arrTiposdePropostas[ 'migracao' ] .
		'';
	
	return $strLink;
}

function geraLinkPropostasDaAcao( $strDiretorioSistema , $intAcaoId , $strCodIdUnique , $arrTiposdePropostas )
{
	$strLink = 
	'\'' .
	'geral/reltipos_ajax.php?' . 
	'&' . 'intAcaoId'						. '=' . $intAcaoId .
	'&' . 'strAgrupadoPor'					. '=' . 'PropostasDasAcoes' .
	'&' . 'arrTiposPropostas[inclusao]'		. '=' . $arrTiposdePropostas[ 'inclusao' ] .
	'&' . 'arrTiposPropostas[exclusao]'		. '=' . $arrTiposdePropostas[ 'exclusao' ] .
	'&' . 'arrTiposPropostas[alteracao]'	. '=' . $arrTiposdePropostas[ 'alteracao' ] .
	'&' . 'arrTiposPropostas[fusao]'		. '=' . $arrTiposdePropostas[ 'fusao' ] .
	'&' . 'arrTiposPropostas[migracao]'		. '=' . $arrTiposdePropostas[ 'migracao' ] .
	'\'' . ',' . '\'' . $strCodIdUnique . '\'';

	return $strLink;
}

function geraHtmlDePropostasDaAcao( $arrPropostas , $strDiretorioSistema , $arrTiposdePropostas )
{
	
	?>
		<div style="width:100%; text-align: left"> 
			<table border="0" cellspacing="0" cellpadding="0" style="color:#003F7E;" width="100%" class="listagem_ajax">
				<thead>
					<tr>
				  		<th nowrap="true" class="propostaContador" width="15px" style=" height: 15px; font-size: 11px; text-align: left width: 15px"> 
				  			&nbsp;
			  			</th>
						<th valign="top" class="propostaProponente" style=" height: 15px;font-size: 11px;  text-align: left">
							Proponente
						</th>
						<th valign="top" class="propostaTipo" style=" height: 15px;font-size: 11px;  text-align: left">
							Tipo
						</th>
						<th width="21px" title="Inclusões">
							<div style="color: brow; height: 15px; width: 21px; font-size:11px;" class="listagem_ajax_div">
								I
							</div>							
						</th>
						<th width="21px" title="Alterações">
							<div style="color: blue; height: 15px; width: 21px; font-size:11px;" class="listagem_ajax_div"> 
								A
							</div>							
						</th>
						<th width="21px" title="Migrações">
							<div style="color: black; height: 15px; width: 21px; font-size:11px;" class="listagem_ajax_div" >
								M
							</div>							
						</th>
						<th width="21px" title="Fusões">
							<div style="color: green; height: 15px; width: 21px; font-size:11px;" class="listagem_ajax_div" >
								F
							</div>							
						</th>
						<th width="21px" title="Exclusões"> 
							<div style="color: red; height: 15px; width: 21px; font-size:11px;" class="listagem_ajax_div" >
								E
							</div>							
						</th>
						<th width="21px" title="Quantidade">
							<div style="color: black; height: 15px; width: 21px; font-size:11px;" class="listagem_ajax_div">
								Q
							</div>
						</th>
					</tr>
				</thead>
				<tbody>
		<!-- fim cabecalho -->
					<?
						foreach( $arrPropostas as $intCount => $arrProposta )
						{
							$strLink = geraLinkAcao( $strDiretorioSistema , $arrProposta[ 'acao_id' ] , $arrTiposdePropostas );
							?>
		<!-- proposta elemento -->
								<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td valign="top" style="text-align: center">
										<?= $intCount + 1 ?>
									</td>
									<td>
										<a href="<?= $strLink ?>" target="_blank">
											<?= $arrProposta[ 'usuario_nome' ] ?>
										</a>
									</td>
									<td>
										<a href="<?= $strLink ?>" target="_blank">
											<?= $arrProposta[ 'proposta_tipo' ] ?>
										</a>
									</td>
									<td align="center" title="Inclusões">
										<div style="color: brow; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											<?= $arrProposta[ 'quantidade_inclusao' ] ?>
										</div>							
									</th>
									<td align="center" title="Alterações">
										<div style="color: blue; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" > 
											<?= $arrProposta[ 'quantidade_alteracao' ] ?>
										</div>							
									</td>
									<td align="center" title="Migrações">
										<div style="color: black; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											<?= $arrProposta[ 'quantidade_migracao' ] ?>
										</div>							
									</td>
									<td align="center" title="Fusões">
										<div style="color: green; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div">
											<?= $arrProposta[ 'quantidade_fusao' ] ?>
										</div>							
									</td>
									<td align="center" title="Exclusões">
										<div style="color: red; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											<?= $arrProposta[ 'quantidade_exclusao' ] ?>
										</div>							
									</td>
									<td align="center" title="Quantidade">
										<div style="color: black; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											(<?= $arrProposta[ 'quantidade_total' ] ?>)
										</div>							
									</td>
								</tr>
		<!-- fim proposta elemento -->
							<?					
						}
					?>
		<!-- rodapé -->
				</tbody>
			</table>
		</div>
		<!-- fim rodapé -->
	<?
}

function geraHtmlDePropostasDoPrograma( $arrPropostas , $strDiretorioSistema , $arrTiposdePropostas )
{
	?>
	<!-- cabecalho -->
		<div style="width:100%; text-align: left"> 
			<table border="0" cellspacing="0" cellpadding="0" style="color:#003F7E;">
				<thead>
					<tr>
				  		<th nowrap="true" class="propostaContador"> 
				  			&nbsp;
			  			</th>
						<th valign="top" class="propostaProponente">
							Proponente Z
						</th>
						<th valign="top" class="propostaTipo">
							Tipo
						</th>
					</tr>
				</thead>
				<tbody>
		<!-- fim cabecalho -->
					<?
						foreach( $arrPropostas as $intCount => $arrProposta )
						{
							$strLink = geraLinkPrograma( $strDiretorioSistema , $arrProposta[ 'programa_id' ] , $arrTiposdePropostas );
							?>
		<!-- proposta elemento -->
								<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td valign="top">
										<?= $intCount + 1 ?>
									</td>
									<td>
										<a href="<?= $strLink ?>" target="_blank">
											<?= $arrProposta[ 'usuario_nome' ] ?>
										</a>
									</td>
									<td>
										<a href="<?= $strLink ?>" target="_blank">
											<?= $arrProposta[ 'proposta_tipo' ] ?>
										</a>
									</td>
								</tr>
		<!-- fim proposta elemento -->
							<?					
						}
					?>
		<!-- rodapé -->
				</tbody>
			</table>
		</div>
	<!-- fim rodapé -->
	<?
}

function geraHtmlDePropostasDaUnidade( $arrPropostas , $strDiretorioSistema , $arrTiposdePropostas )
{
	?>
	<!-- cabecalho -->
		<div style="width:100%; text-align: left">
			<table border="0" cellspacing="0" cellpadding="0" style="color:#003F7E;" class="listagem_ajax">
				<thead>
					<tr>
				  		<th nowrap="true" class="propostaContador"  style="font-size: 11px"> 
				  			&nbsp;
			  			</th>
						<th valign="top" class="propostaProponente"  style="font-size: 11px">
							Proponente X
						</th>
						<th valign="top" class="propostaTipo"  style="font-size: 11px">
							Tipo
						</th>
					</tr>
				</thead>
				<tbody>
		<!-- fim cabecalho -->
					<?
						foreach( $arrPropostas as $intCount => $arrProposta )
						{
							$strLink = geraLinkUnidade( $strDiretorioSistema , $arrProposta[ 'unidade_id' ] , $arrTiposdePropostas );
							?>
		<!-- proposta elemento -->
								<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td valign="top">
										<?= $intCount + 1 ?>
									</td>
									<td>
										<a href="<?= $strLink ?>" target="_blank">
											<?= $arrProposta[ 'usuario_nome' ] ?>
										</a>
									</td>
									<td>
										<a href="<?= $strLink ?>" target="_blank">
											<?= $arrProposta[ 'proposta_tipo' ] ?>
										</a>
									</td>
								</tr>
		<!-- fim proposta elemento -->
							<?					
						}
					?>
		<!-- rodapé -->
				</tbody>
			</table>
		</div>
	<!-- fim rodapé -->
	<?
}

function geraHtmlDeAcoesDoPrograma( $arrAcoes , $strDiretorioSistema, $arrTiposdePropostas )
{
	?>
	<!-- cabecalho -->
		<div style="width:100%; text-align: left"> 
			<table border="0" cellspacing="0" cellpadding="0" style="color:#003F7E; float:left" width="100%" >
				<thead>
					<tr>
				  		<th nowrap="true" class="acaoContador"  style=" height: 15px;font-size: 11px;  text-align: left"> 
				  			&nbsp;
			  			</th>
						<th valign="top" class="acaoCodigo" style=" height: 15px; font-size: 11px;  text-align: left">
			  				Código
			  			</th>
						<th valign="top" class="acaoNome"  style=" height: 15px; font-size: 11px; text-align: left">
							Nome da Ação
						</th>
						<th width="21px" title="Inclusões">
							<div style="color: brow; height: 15px; width: 19px; font-size:11px;" class="listagem_ajax_div">
								I
							</div>							
						</th>
						<th width="21px" title="Alterações">
							<div style="color: blue; height: 15px; width: 19px; font-size:11px;" class="listagem_ajax_div"> 
								A
							</div>							
						</th>
						<th width="21px" title="Migrações">
							<div style="color: black; height: 15px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
								M
							</div>							
						</th>
						<th width="21px" title="Fusões">
							<div style="color: green; height: 15px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
								F
							</div>							
						</th>
						<th width="21px" title="Exclusões">
							<div style="color: red; height: 15px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
								E
							</div>							
						</th>
						<th width="21px" title="Quantidade">
							<div style="color: black; height: 15px; width: 19px; font-size:11px;" class="listagem_ajax_div">
								Q
							</div>							
						</th>
					</tr>
				</thead>
				<tbody>
	<!-- fim cabecalho -->
					<?
						$intCount = 0;
						foreach( $arrAcoes as &$arrAcao )
						{
							++$intCount;
							$strUniqueCode = 'Ajax' . $intCount . '-' . $arrAcao[ 'acao_id'];
							$strLink = geraLinkPropostasDaAcao( $strDiretorioSistema , $arrAcao[ 'acao_id'] ,  $strUniqueCode , $arrTiposdePropostas );
							$strImgId = 'img' .  $strUniqueCode;
							$strTdId = 'td' . $strUniqueCode;
							?>
	<!-- acao elemento -->
								<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td valign="top">
										<?= $intCount ?>
	<!-- expansor -->
										<? if ( $arrAcao[ 'quantidade_total' ] > 0 ) : ?>
											<img src="../imagens/mais.gif" name="+" border="0" 
											id="<?= $strImgId ?>" onclick="abreconteudo( <?= $strLink ?> )" />
										<? endif ?>
									</td>
									<td>
										<div style="height: 19px; font-size:11px;" class="listagem_ajax_div" >
											<?= $arrAcao[ 'acao_codigo' ] ?>
										</div>
									</td>
									<td>
										<?= delimitaTexto( $arrAcao[ 'acao_descricao' ] , 100 ) ?>
									</td>
									<td align="center" title="Inclusões">
										<div style="color: brow; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											<?= $arrAcao[ 'quantidade_inclusao' ] ?>
										</div>							
									</th>
									<td align="center" title="Alterações">
										<div style="color: blue; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" > 
											<?= $arrAcao[ 'quantidade_alteracao' ] ?>
										</div>							
									</td>
									<td align="center" title="Migrações">
										<div style="color: black; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											<?= $arrAcao[ 'quantidade_migracao' ] ?>
										</div>							
									</td>
									<td align="center" title="Fusões">
										<div style="color: green; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div">
											<?= $arrAcao[ 'quantidade_fusao' ] ?>
										</div>							
									</td>
									<td align="center" title="Exclusões">
										<div style="color: red; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											<?= $arrAcao[ 'quantidade_exclusao' ] ?>
										</div>							
									</td>
									<td align="center" title="Quantidade">
										<div style="color: black; height: 19px; width: 19px; font-size:11px;" class="listagem_ajax_div" >
											(<?= $arrAcao[ 'quantidade_total' ] ?>)
										</div>							
									</td>
								</tr>
								<tr>
									<td>
									</td>
									<td id="<?= $strTdId ?>" colspan="8"></td>
								</tr>
	<!-- fim acao elemento -->
							<?					
						}
					?>
				</tbody>
			</table>			
		</div>
	<!-- fim rodapé -->
	<?	
}

function geraHtmlDeAcoesDaUnidade( $arrAcoes , $strDiretorioSistema, $arrTiposdePropostas )
{
	?>
	<!-- cabecalho -->
		<div style="width:100%; text-align: left"> 
			<table border="0" cellspacing="0" cellpadding="0" style="color:#003F7E;">
				<thead>
					<tr>
				  		<td nowrap="true" class="acaoContador"> 
				  			&nbsp;
			  			</td>
						<td valign="top" class="acaoCodigo">
			  				Código
			  			</td>
						<td valign="top" class="acaoNome">
							Nome da Ação
						</td>
						<td valign="top" class="acaoProponentes">
							Proponentes
						</td>
					</tr>
				</thead>
				<tbody>
	<!-- fim cabecalho -->
					<?
						$intCount = 0;
						foreach( $arrAcoes as &$arrAcao )
						{
							++$intCount;
							$strUniqueCode = 'Ajax' . $intCount . '-' . $arrAcao[ 'acao_id'];
							$strLink = geraLinkPropostasDaAcao( $strDiretorioSistema , $arrAcao[ 'acao_id'] ,  $strUniqueCode , $arrTiposdePropostas );
							$strImgId = 'img' .  $strUniqueCode;
							$strTdId = 'td' . $strUniqueCode;
							?>
	<!-- acao elemento -->
								<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td valign="top">
										<?= $intCount ?>
	<!-- expansor -->
										<img src="../imagens/mais.gif" name="+" border="0" 
										id="<?= $strImgId ?>" onclick="abreconteudo( <?= $strLink ?> )" />
									</td>
									<td>
										<?= $arrAcao[ 'acao_codigo' ] ?>
									</td>
									<td>
										<?= delimitaTexto( $arrAcao[ 'acao_descricao' ] , 50 ) ?>
									</td>
									<td>
										<?= delimitaTexto( montaListagem( $arrAcao[ 'proponentes_nomes' ] ) , 50 ) ?>
									</td>
								</tr>
								<tr>
									<td>
									</td>
									<td id="<?= $strTdId ?>" colspan="3"></td>
								</tr>
	<!-- fim acao elemento -->
							<?					
						}
					?>
	<!-- rodapé -->
				</tbody>
			</table>
		</div>
	<!-- fim rodapé -->
	<?	
}

function delimitaTexto( $strTexto , $intTamanhoMaximo )
{
	if( strlen( $strTexto ) > $intTamanhoMaximo )
	{
		$strTextoCortado = substr( $strTexto , 0 , $intTamanhoMaximo );
		$strOnMouseOver = 'SuperTitleOn( this , \'' . $strTexto . '\' )';
		$strOnMouseOut = 'SuperTitleOff( this )';
		$strTag = '<font onmouseover="' . $strOnMouseOver . '" onmouseout="' . $strOnMouseOut . '" >' . "\n";
		$strTag .= $strTextoCortado . '...' . "\n";
		$strTag .= '</font>';
	}
	else
	{
		$strTag = $strTexto;
	}
	return $strTag;
}

function montaListagem( $arrElementos , $strSeparator =  ', ' , $strLastSeparator = ' e ' )
{
	switch( sizeof( $arrElementos ) )
	{
		case 0:
		{
			$strResult = '';
			break;
		}
		case 1:
		{
			$strResult = array_pop( $arrElementos );
			break;
		}
		default:
		{
			$objLast = array_pop( $arrElementos );
			$strResult = implode( $strSeparator , $arrElementos ) . $strLastSeparator . $objLast ;
			break;	
		}
	}

	return $strResult;
}

function geraListagem( $intAcaoId , $intProgramaId , $intUnidadeId, $strCodigoUnidade, $strAgrupadoPor , $arrTiposdePropostas , $arrColunasOrdenacao , $strDiretorioSistema , $intAnoExercicio , $boolIncluiAgrupadoresVazios )
{
	?>
	<style>
		.listagem_ajax
		{
			font-size:  11px;
		}
		.listagem_ajax_div
		{
			width: 19px;
		}
		
		.propostaContador
		{
			width: 5px;
		}
		.propostaProponente
		{
			width: 260px;
		}
		.propostaTipo
		{
			width: 280px;
		}
	</style>
	<?
	switch( $strAgrupadoPor )
	{
		case 'PropostasDasAcoes':
		{
			$arrPropostas = geraListagemDePropostasDaAcao( $intAcaoId , $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio , $boolIncluiAgrupadoresVazios );
			geraHtmlDePropostasDaAcao( $arrPropostas , $strDiretorioSistema , $arrTiposdePropostas );
			break;
		}
		case 'AcoesDosProgramas':
		{
			$arrAcoes = geraListagemDeAcoesComPropostasDoPrograma( $intProgramaId , $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio , $boolIncluiAgrupadoresVazios );
			geraHtmlDeAcoesDoPrograma( $arrAcoes , $strDiretorioSistema , $arrTiposdePropostas );
			break;
		}
		case 'PropostasDasAcoesDasUnidades':
		{
			$arrPropostas = geraListagemDePropostasDeAcaoDaUnidade( $intUnidadeId , $strCodigoUnidade, $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio , $boolIncluiAgrupadoresVazios );
			geraHtmlDePropostasDaUnidade( $arrPropostas , $strDiretorioSistema , $arrTiposdePropostas );
			break;
		}
		case 'AcoesDasUnidades':
		{
			$arrAcoes = geraListagemDeAcoesComPropostasDaUnidade( $intUnidadeId , $strCodigoUnidade, $arrTiposdePropostas , $arrColunasOrdenacao , $intAnoExercicio , $boolIncluiAgrupadoresVazios );
			geraHtmlDeAcoesDaUnidade( $arrAcoes , $strDiretorioSistema , $arrTiposdePropostas );
			break;
		}
	}
}

$arrTiposdePropostas			= @$_REQUEST[ 'arrTiposPropostas' ];
$arrColunasOrdenacao			= @$_REQUEST[ 'arrColunasOrdenacao' ]; 
$strAgrupadoPor					= @$_REQUEST[ 'strAgrupadoPor' ]; 
$strDirecaoDaOrdenacao			= @$_REQUEST[ 'strDirecaoDaOrdenacao' ]; 
$intAcaoId						= @$_REQUEST[ 'intAcaoId' ]; 
$intProgramaId					= @$_REQUEST[ 'intProgramaId' ]; 
$intUnidadeId					= @$_REQUEST[ 'intUnidadeId' ];
$strCodigoUnidade				= @$_REQUEST[ 'strCodigoUnidade' ];
$boolIncluiAgrupadoresVazios	= @$_REQUEST[ 'boolIncluiAgrupadoresVazios' ];

if( $strDirecaoDaOrdenacao == null )	 	$strDirecaoDaOrdenacao = 'ASC';
if( $strAgrupadoPor == null ) 				$strAgrupadoPor = 'Acoes';
if( $arrTiposdePropostas == null ) 			$arrTiposdePropostas = array();
if( $arrColunasOrdenacao == null ) 			$arrColunasOrdenacao = array();
if( $boolIncluiAgrupadoresVazios == null )	$boolIncluiAgrupadoresVazios = false;

$strDiretorioSistema = 					$_SESSION['sisdiretorio'];
$intAnoExercicio =						$_SESSION[ 'exercicio' ];

geraListagem( $intAcaoId , $intProgramaId , $intUnidadeId , $strCodigoUnidade , $strAgrupadoPor , $arrTiposdePropostas , $arrColunasOrdenacao , $strDiretorioSistema , $intAnoExercicio , $boolIncluiAgrupadoresVazios );
?>
