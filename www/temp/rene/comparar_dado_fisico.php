<?php

ini_set( 'display_errors', E_ALL );
set_time_limit( 0 );

// carrega as bibliotecas
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// atribuições requeridas para que a auditoria do sistema funcione
$_SESSION['sisid'] = 1; # seleciona o sistema de segurança
$_SESSION['usucpf'] = '';
$_SESSION['usucpforigem'] = '';
$ano = $_REQUEST['ano'];

// abre conexão com o banco espelho produção
$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'localhost';
$porta_bd    = '5432';
$usuario_db  = 'postgres';
$senha_bd    = 'postgres01';
$db = new cls_banco();

// carrega o xml para a memória
$arquivo = APPRAIZ . 'arquivos/SIGPLAN/importacao/sigplan-20070226.xml';
$documento = simplexml_load_file( $arquivo );
if ( !$documento ) {
	// @TODO: manipular erro
	dbg( 'xml inválido', 1 );
}

$total = 0;
foreach ( $documento->ArrayOfDadoFisico->DadoFisico as $dadofisico ) {
	$dadofisico = (array) $dadofisico;
	if ( $dadofisico['ACACod'] == '4572' ) {
		$total += $dadofisico['FISQtdePrevistoAno'];
	}
}
dbg( $total, 1 );


// importa os dados novos
$db->executar( "delete from monitora.dadofisico2" );
foreach ( $documento->ArrayOfDadoFisico->DadoFisico as $dadofisico ) {
	$dadofisico = (array) $dadofisico;
	$sql_identificador = sprintf(
		"select acaid from monitora.dadofisico where prgano = '%s' and prgcod = '%s' and acacod = '%s' and saccod = '%s' and regcod = '%s'",
		$dadofisico['PRGAno'],
		$dadofisico['PRGCod'],
		$dadofisico['ACACod'],
		$dadofisico['SACCod'],
		$dadofisico['REGCod']
	);
	$acaid = $db->pegaUm( $sql_identificador );
	if ( !$acaid ) {
		continue;
	}
	$sql = sprintf(
		"insert into monitora.dadofisico2 ( %s, acaid ) values ( '%s', %s )",
		implode( ",", array_keys( $dadofisico ) ),
		implode( "','", $dadofisico ),
		$acaid
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		dbg( $dadofisico, 1 );
	}
}
$db->commit();

// identifica as ações do sigplan que não constam no simec
$acoes = array();
foreach ( $documento->ArrayOfAcao->Acao as $acao ) {
	$acao = (array) $acao;
	$sql = sprintf(
		"select count(*) from acao a where prgano = '%s' and prgcod = '%s' and acacod = '%s' and saccod = '%s'",
		$acao['PRGAno'],
		$acao['PRGCod'],
		$acao['ACACod'],
		$acao['SACCod']
	);
	$quantidade = $db->pegaUm( $sql );
	if ( $quantidade == 0 ) {
		$chave = $acao['PRGAno'] . $acao['PRGCod'] . $acao['ACACod'] . $acao['SACCod'];
		$acoes[$chave] = $acao;
		//array_push( $acoes, $acao );
	}
}
ksort( $acoes, SORT_STRING );
$acoes = array_values( $acoes );

?>

<html>
	<head>
		<meta http-equiv="content-type" content="text/html; charset=ISO-8859-1"/>
		<meta http-equiv="Cache-Control" content="no-cache"/>
		<meta http-equiv="Pragma" content="no-cache"/>
		<meta http-equiv="Expires" content="-1"/>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
	</head>
	<body>
		<table class="listagem" style="width: 600px">
			<caption><h2>Ações presentes no SIGPLAN e não incluídas no SIMEC</h2></caption>
			<thead>
				<tr>
					<th width="20px">#</th>
					<th>Identificador</th>
					<th>Descrição</th>
				</tr>
			</thead>
			<tbody>
			<?php foreach ( (array) $acoes as $indice => $acao ): ?>
				<tr align="center">
					<td align="center"><?= sprintf( "%02d", $indice + 1 ) ?></td>
					<td align="center"><?= $acao['PRGCod'] ?>.<?= $acao['ACACod'] ?>.<?= $acao['UNICod'] ?>.<?= $acao['LOCCod'] ?></td>
					<?php
						$descricao = utf8_decode( $acao['ACADsc'] );
					?>
					<td align="left"><?= substr( $descricao, 0, 60 ) ?> <?= strlen( $descricao ) > 60 ? '...' : '' ?></td>
				</tr>
			<?php endforeach; ?>
			</tbody>
		</table>
		<p>&nbsp;</p>
		
		<?php
			// compara os registros importados com os utilizados pelo sistema
			$sql = "select df.prgano, df.prgcod, df.acacod, df.saccod, df.regcod, a.unicod, a.loccod, df.fisqtdeprevistoano as simec, df2.fisqtdeprevistoano as sigplan
					from monitora.dadofisico df
					inner join monitora.dadofisico2 df2 on df2.acaid = df.acaid
					inner join monitora.acao a on a.acaid = df.acaid
					where df2.fisqtdeprevistoano <> df.fisqtdeprevistoano";
			$registros = $db->carregar( $sql );
			/*
			foreach( $registros as $registro ) {
				$sql = sprintf(
					"update monitora.dadofisico set fisqtdeprevistoano = '%s' where prgano = '%s' and prgcod = '%s' and acacod = '%s' and saccod = '%s' and regcod = '%s'",
					$registro['sigplan'],
					$registro['prgano'],
					$registro['prgcod'],
					$registro['acacod'],
					$registro['saccod'],
					$registro['regcod']
				);
				echo "<pre>$sql;</pre>";
			}
			*/
		?>
		<table class="listagem" style="width: 600px">
			<caption><h2>Registros a previsão da LOA inconsistente</h2></caption>
			<thead>
				<tr>
					<th width="20px">#</th>
					<th>identificador</th>
					<th>sigplan</th>
					<th>simec</th>
					<th>diferença</th>
				</tr>
			</thead>
			<tbody>
			<?php foreach( (array) $registros as $indice => $registro ): ?>
				<tr>
					<td align="center"><?= sprintf( "%02d", $indice + 1 ) ?></td>
					<td align="center"><?= $registro['prgcod'] ?>.<?= $registro['acacod'] ?>.<?= $registro['unicod'] ?>.<?= $registro['loccod'] ?></td>
					<td align="right"><?= number_format( $registro['sigplan'], 0, ',', '.' ) ?></td>
					<td align="right"><?= number_format( $registro['simec'], 0, ',', '.' ) ?></td>
					<td align="right"><?= number_format( $registro['simec'] - $registro['sigplan'], 0, ',', '.' ) ?></td>
				</tr>
			<?php endforeach; ?>
			</tbody>
		</table>
	</body>
</html>