<?php

$sbaid = (integer) $_REQUEST['sbaid'];
$aciid = (integer) $_REQUEST['aciid'];

// verifica se formulario foi submetido
if ( isset( $_REQUEST['formulario'] ) )
{
	
	// trata entradas
	$sbadsc = trim( str_replace( "'", "\\'", $_REQUEST['sbadsc'] ) );
	$sbastgmpl = trim( str_replace( "'", "\\'", $_REQUEST['sbastgmpl'] ) );
	$sbaprm = trim( str_replace( "'", "\\'", $_REQUEST['sbaprm'] ) );
	$sbapcr = trim( str_replace( "'", "\\'", $_REQUEST['sbapcr'] ) );
	$sbauntdsc = trim( str_replace( "'", "\\'", $_REQUEST['sbauntdsc'] ) );
	$sba1ini = trim( str_replace( "'", "\\'", $_REQUEST['sba1ini'] ) );
	$sba1fim = trim( str_replace( "'", "\\'", $_REQUEST['sba1fim'] ) );
	$sba2ini = trim( str_replace( "'", "\\'", $_REQUEST['sba2ini'] ) );
	$sba2fim = trim( str_replace( "'", "\\'", $_REQUEST['sba2fim'] ) );
	$sba3ini = trim( str_replace( "'", "\\'", $_REQUEST['sba3ini'] ) );
	$sba3fim = trim( str_replace( "'", "\\'", $_REQUEST['sba3fim'] ) );
	$sba4ini = trim( str_replace( "'", "\\'", $_REQUEST['sba4ini'] ) );
	$sba4fim = trim( str_replace( "'", "\\'", $_REQUEST['sba4fim'] ) );
	
	$undid = $_REQUEST['undid'];
	$frmid = $_REQUEST['frmid'];
	$sba1ano = $_REQUEST['sba1ano'];
	$sba2ano = $_REQUEST['sba2ano'];
	$sba3ano = $_REQUEST['sba3ano'];
	$sba4ano = $_REQUEST['sba4ano'];
	$sbaunt = $_REQUEST['sbaunt'];
	$sbauntdsc = $_REQUEST['sbauntdsc'];
	
	// ----- INSERT ------------------------------------------------------------
	if ( !$sbaid )
	{
		$sql = "
			insert into cte.subacaoindicador
			(
				aciid,
				undid,				frmid,				sbadsc,
				sbastgmpl,			sbaprm,				sbapcr,
				sba1ano,			sba2ano,			sba3ano,
				sba4ano,			sbaunt,				sbauntdsc,
				sba1ini,			sba1fim,			sba2ini,
				sba2fim,			sba3ini,			sba3fim,
				sba4ini,			sba4fim
			)
			values
			(
				%d,
				%d,					%d,					'%s',
				'%s',				'%s',				'%s',
				%d,					%d,					%d,
				%d,					%f,					'%s',
				'%s',				'%s',				'%s',
				'%s',				'%s',				'%s',
				'%s',				'%s'
			)
			return sbaid
		";
		$sql = sprintf(
			$sql,
				$aciid,
				$undid,				$frmid,				$sbadsc,
				$sbastgmpl,			$sbaprm,			$sbapcr,
				$sba1ano,			$sba2ano,			$sba3ano,
				$sba4ano,			$sbaunt,			$sbauntdsc,
				$sba1ini,			$sba1fim,			$sba2ini,
				$sba2fim,			$sba3ini,			$sba3fim,
				$sba4ini,			$sba4fim
		);
		
		$sbaid = (integer) $db->executar( $sql );
		$sucesso = (boolean) $sbaid;
	}
	
	// ----- UPDATE ------------------------------------------------------------
	else
	{
		
		$sql = "
			update cte.subacaoindicador set
				undid = %d,				frmid = %d,
				sbadsc = '%s',			sbastgmpl = '%s',
				sbaprm = '%s',			sbapcr = '%s',
				sba1ano = %d,			sba2ano = %d,
				sba3ano = %d,			sba4ano = %d,
				sbaunt = %f,			sbauntdsc = '%s',
				sba1ini = '%s',			sba1fim = '%s',
				sba2ini = '%s',			sba2fim = '%s',
				sba3ini = '%s',			sba3fim = '%s',
				sba4ini = '%s',			sba4fim = '%s'
			where
				sbaid = %d
		";
		$sql = sprintf(
			$sql,
				$undid,					$frmid,
				$sbadsc,				$sbastgmpl,
				$sbaprm,				$sbapcr,
				$sba1ano,				$sba2ano,
				$sba3ano,				$sba4ano,
				$sbaunt,				$sbauntdsc,
				$sba1ini,				$sba1fim,
				$sba2ini,				$sba2fim,
				$sba3ini,				$sba3fim,
				$sba4ini,				$sba4fim,
				$_REQUEST['sbaid']
		);
		
		$sucesso = (boolean) $db->executar( $sql );
	}
	
	// executa aчуo
	if ( $sucesso )
	{
		$mensagem = "Sucesso!";
		$db->commit();
	}
	else
	{
		$mensagem = "Insucesso!";
		$db->rollback();
	}
}


?>