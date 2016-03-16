<?php
	
	set_time_limit( 0 );
	
	function msg( $msg )
	{
		print '[' . date( 'H:i:s' ) . '] ' . $msg . "<br/>\n";
		ob_end_flush();
		flush();
	}
	
	// variáveis auxiliares utilizadas durante os loops
	$i = 0;
	$fp = null;
	//$linha = '';
	$arquivo = '';
	$dados = array();
	$cre = array( 4,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18 );
	$deb = array( 5, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 );
	
	// campos capturados em cada linha do arquivo
	/*
	$uniges = '';	$gestao = '';
	$codcon = '';	$concor = '';
	$deb = array(
		0 => '',	1 => '',	2 => '',	3 => '',	4 => '',
		5 => '',	6 => '',	7 => '',	8 => '',	9 => '',
		10 => '',	11 => '',	12 => '',	13 => '',
	);
	$cre = array(
		0 => '',	1 => '',	2 => '',	3 => '',	4 => '',
		5 => '',	6 => '',	7 => '',	8 => '',	9 => '',
		10 => '',	11 => '',	12 => '',	13 => '',
	);
	$dattra = '';
	$hortra = '';
	*/
	
	// conecta no banco e prepara query pra ser executada
	$conexao = pg_connect( 'host= port= dbname=simec user= password=' );
	pg_prepare( $conexao, 'query',
		' insert into importacao.saldocontabil_teste2 ' .
		' ( ' .
			' "IT_CO_UNIDADE_GESTORA",			"IT_CO_GESTAO",			"GR_CODIGO_CONTA", ' .
			' "IT_CO_CONTA_CORRENTE_CONTABIL",	"IT_VA_DEBITO_MENSAL",	"IT_VA_CREDITO_MENSAL", ' .
			' "IT_DA_TRANSACAO",				"IT_HO_TRANSACAO",		"MES" '.
		' ) ' .
		' values ' .
		' ( ' .
			' $1, $2, $3, ' .
			' $4, $5, $6, ' .
			' $7, $8, $9 ' .
		' )'
	);
	
	// percorre arquivos a serem importados
	foreach ( glob( '../financeiro/arquivos/siafi/saldo/saldocontabil_2005/sc*.txt' ) as $caminho )
	{
		$fp = fopen( $caminho, 'r' );
		if ( !$fp )
		{
			msg( 'ERRO! falha ao abrir o arquivo ' . basename( $caminho ) );
			continue;
		}
		$arquivo = basename( $caminho );
		msg( $arquivo . ' iniciada' );
		while( !feof( $fp ) )
		{
			//$linha = fgets( $fp );
			$dados = sscanf( $fp, "%6s%5s%9s%43s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%8s%6s\n" );
			/*
			sscanf(
				$linha,
				'%6s%5s%9s%43s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%18s%8s%6s',
				$uniges,	$gestao,	$codcon,	$concor,
				$deb[0],	$cre[0],
				$deb[1],	$deb[2],	$deb[3],	$deb[4],
				$deb[5],	$deb[6],	$deb[7],	$deb[8],
				$deb[9],	$deb[10],	$deb[11],	$deb[12],
				$deb[13],
				$cre[1],	$cre[2],	$cre[3],	$cre[4],
				$cre[5],	$cre[6],	$cre[7],	$cre[8],
				$cre[9],	$cre[10],	$cre[11],	$cre[12],
				$cre[13],
				$dattra,	$hortra
			);
			*/
			for ( $i = 0; $i < 14; $i++ )
			{
				if (
					!pg_execute(
						$conexao,
						'query',
						array(
							$dados[0],
							$dados[1],
							$dados[2],
							$dados[3],
							$dados[$cre[$i]],
							$dados[$deb[$i]],
							$dados[32],
							$dados[33],
							$i
						)
					)
				)
				{
					msg( 'ERRO! falha ao executar query para os seguintes dados : ' . serialize( array( $uniges, $gestao, $codcon, $concor, $deb[$i], $cre[$i], $dattra, $hortra, $i ) ) );
					exit();
				}
			}
			/*
			for ( $i = 0; $i < 14; $i++ )
			{
				if ( !pg_execute( $conexao, 'query', array( $uniges, $gestao, $codcon, $concor, $deb[$i], $cre[$i], $dattra, $hortra, $i ) ) )
				{
					msg( 'ERRO! falha ao executar query para os seguintes dados : ' . serialize( array( $uniges, $gestao, $codcon, $concor, $deb[$i], $cre[$i], $dattra, $hortra, $i ) ) );
					exit();
				}
			}
			*/
		}
		fclose( $fp );
		msg( $arquivo . ' finalizada' );
		@pg_close($conexao);
		break;
	}
	
?>