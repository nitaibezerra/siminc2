<?php

header( 'Content-Type: text/plain;' );
set_time_limit( 0 );
ini_set( 'display_errors', E_ALL );

// carrega as bibliotecas
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$_SESSION['usucpforigem'] = '';

function msg( $msg )
{
	echo $msg . "\n";
}

function erro( $erro )
{
	global $db;
	msg( 'erro! ' . $erro );
	msg( 'operação abortada' );
	$db->rollback();
	exit();
}

$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'localhost';
$porta_bd    = '5432';
$usuario_db  = 'postgres';
$senha_bd    = 'postgres01';

$db = new cls_banco();

// esta função deve ser disparada após a atualização da tabela dados fisicos
// o primeiro passo é selecionar os campos que interessam em dados fisicos

// captura o ano de exercício
$sql = "select ano from public.anos where anosnatual = 't' limit 1";
$exercico = $db->pegaUm( $sql );

$sql = "select d.acaid from monitora.dadofisico d inner join monitora.acao a on a.acaid=d.acaid and a.prgano = '" . $exercico . "' and a.acasnrap='f' where d.prgano='" . $exercico . "' order by a.acaid ";
$RSd = $db->record_set( $sql );
$nlinhasd = $db->conta_linhas( $RSd );
for ( $id = 0; $id <= $nlinhasd; $id++ )
{
	$res = $db->carrega_registro( $RSd, $id );
	if( is_array( $res ) ) extract( $res );
	else continue;
	// aqui se seleciona o acaid para buscar na tabela execucaopto
	// com o acaid faço a busca em execucaopto
	$soma = 0;
	$ok = 0;
	$sql3 = "
		select e.expid,e.exprealizado,a.acasnmetanaocumulativa, r.refmes_ref from monitora.execucaopto e
			inner join monitora.referencia r on e.refcod=r.refcod and r.refmes_ref::integer <= 12
			inner join monitora.acao a on a.acaid=e.acaid
		where e.acaid =" . $acaid . " ORDER BY refmes_ref";
	$res = $db->carregaAgrupado( $sql3, "refmes_ref" );
	if ( count( $res ) >= 0 )
	{
		$ok = 1;
		$realizado1 = $realizado2 = $realizado3 = $realizado4 = $realizado5 = $realizado6 = $realizado7 = $realizado8 = $realizado9 = $realizado10 = $realizado11 = $realizado12 = 0;
		foreach( $res as $mes => $dados)
		{
			$dados = $dados[0];
			$j = (int) $mes;
			${'realizado'.$j} = $dados["exprealizado"];
			if ( $dados["acasnmetanaocumulativa"] == 't' )
			{
				// é não cumulativa
				if ( $soma < ${'realizado'.$j} )
				{
					$soma = ${'realizado'.$j};
				}
			}
			else
			{
				$soma += ${'realizado'.$j};  
			}
		}
	}
	if ( $ok )
	{
		$sql = "update dadofisico set FISQtdeRealizado1=$realizado1,FISQtdeRealizado2 =$realizado2,  FISQtdeRealizado3 =$realizado3 ,  FISQtdeRealizado4=$realizado4  ,  FISQtdeRealizado5=$realizado5  ,  FISQtdeRealizado6=$realizado6 ,  FISQtdeRealizado7=$realizado7  ,  FISQtdeRealizado8=$realizado8  ,  FISQtdeRealizado9=$realizado9 ,  FISQtdeRealizado10 =$realizado10 ,  FISQtdeRealizado11=$realizado11  , FISQtdeRealizado12=$realizado12 ,FISQtdeRealizadoano=$soma  where acaid=$acaid";
		$db->executar( $sql );
		//$db->commit();
	}
}

$sql = "select count( prgano ) from monitora.dadofisico where prgano='" . $exercico . "'";
$sql = 'select d.PRGAno as "PRGAno" ,d.PRGCod as "PRGCod",d.ACACod as "ACACod",d.SACCod as "SACCod",d.REGCod as "REGCod",FISQtdeCronInicial1 as "FISQtdeCronInicial1",FISQtdeCronInicial2 as "FISQtdeCronInicial2",FISQtdeCronInicial3 as "FISQtdeCronInicial3",FISQtdeCronInicial4 as "FISQtdeCronInicial4",FISQtdeCronInicial5 as "FISQtdeCronInicial5",FISQtdeCronInicial6 as "FISQtdeCronInicial6",FISQtdeCronInicial7 as "FISQtdeCronInicial7",FISQtdeCronInicial8 as "FISQtdeCronInicial8",FISQtdeCronInicial9 as "FISQtdeCronInicial9",FISQtdeCronInicial10 as "FISQtdeCronInicial10",FISQtdeCronInicial11 as "FISQtdeCronInicial11",FISQtdeCronInicial12 as "FISQtdeCronInicial12",FISQtdeCronogramado1 as "FISQtdeCronogramado1", FISQtdeCronogramado2 as "FISQtdeCronogramado2",FISQtdeCronogramado3 as "FISQtdeCronogramado3",FISQtdeCronogramado4 as "FISQtdeCronogramado4",FISQtdeCronogramado5 as "FISQtdeCronogramado5",FISQtdeCronogramado6 as "FISQtdeCronogramado6",FISQtdeCronogramado7 as "FISQtdeCronogramado7",FISQtdeCronogramado8 as "FISQtdeCronogramado8",FISQtdeCronogramado9 as "FISQtdeCronogramado9",FISQtdeCronogramado10 as "FISQtdeCronogramado10",FISQtdeCronogramado11 as "FISQtdeCronogramado11",FISQtdeCronogramado12 as "FISQtdeCronogramado12",FISQtdeRealizado1 as "FISQtdeRealizado1",FISQtdeRealizado2 as "FISQtdeRealizado2",  FISQtdeRealizado3  as "FISQtdeRealizado3",  FISQtdeRealizado4  as "FISQtdeRealizado4",  FISQtdeRealizado5  as "FISQtdeRealizado5",  FISQtdeRealizado6  as "FISQtdeRealizado6",  FISQtdeRealizado7  as "FISQtdeRealizado7",  FISQtdeRealizado8  as "FISQtdeRealizado8",  FISQtdeRealizado9  as "FISQtdeRealizado9",  FISQtdeRealizado10  as "FISQtdeRealizado10",  FISQtdeRealizado11  as "FISQtdeRealizado11",  FISQtdeRealizado12  as "FISQtdeRealizado12",FISQtdeprevistoano as "FISQtdePrevistoAno",FISQtdeCronInicialano as "FISQtdeCronInicialAno",FISQtdeatualano as "FISQtdeAtualAno",FISQtdeCronogramadoAno as "FISQtdeCronogramadoAno",FISQtdeRealizadoAno as "FISQtdeRealizadoAno",FISDscComentExecucao as "FISDscComentExecucao" from monitora.dadofisico d inner join monitora.acao a on a.acaid = d.acaid and a.acasnrap = \'f\' where d.prgano= \'' .$exercico . '\' ';
if ( !$db->ConvertToXML( $clientedb, $sql, "RetornaDadoFisico.xml", "DadoFisico" ) )
{
	erro( 'falha ao converter dados físicos em xml' );
}

$db->commit();
msg( 'arquivo RetornaDadoFisico.xml criado com sucesso' );
