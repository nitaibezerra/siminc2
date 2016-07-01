<?php

/*

CREATE TABLE carga.produto (
	procod int4 NOT NULL,
	prodsc varchar(80),
	CONSTRAINT pk_produto PRIMARY KEY (procod)
) WITH OIDS;

CREATE TABLE carga.acao (
	prgano char(4),
	prgcod char(4),
	acacod char(4),
	saccod char(4),
	acasnrap bool,
	procod int4,
	prodsc varchar(80),
	CONSTRAINT pk_acao PRIMARY KEY (prgano, prgcod, acacod, saccod, acasnrap),
	CONSTRAINT fk_acao_produto FOREIGN KEY (procod) REFERENCES carga.produto (procod) MATCH SIMPLE
) WITH OIDS;

*/

set_time_limit( 0 );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

restore_error_handler();
restore_exception_handler();
error_reporting( E_ALL );

$booleano = array(
	'acasnmedireta',
	'acasnmedesc',
	'acasnmelincred',
	'acasnmetanaocumulativa',
	'acasnrap',
	'acasnfiscalseguridade',
	'acasninvestatais',
	'acasnoutrasfontes'
);

# abre conexão com o banco
$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
$db          = new cls_banco();

$sql = sprintf(
	"select a.*, p.*
	from carga.acao a
	inner join carga.produto p on p.procod = a.procod"
);
$diferenca = array();
foreach( $db->carregar( $sql ) as $acao ) {
	$sql = sprintf(
		"select a.acaid, a.unicod, a.prgcod, a.acacod, a.loccod, p.procod, p.prodsc
		from monitora.acao a
		inner join public.produto p on p.procod = a.procod
		where a.prgano = '%s' and a.prgcod = '%s' and a.acacod = '%s' and a.saccod = '%s' and a.acasnrap = '%s'",
		$acao['prgano'],
		$acao['prgcod'],
		$acao['acacod'],
		$acao['saccod'],
		$acao['acasnrap']
	);
	$acao_simec = $db->pegaLinha( $sql );
	if ( !$acao_simec ) {
		continue;
	}
	$acao_simec = array_map( 'trim', $acao_simec );
	$acao_simec = array_map( 'mb_strtolower', $acao_simec );
	if ( $acao_simec['prodsc'] != $acao['prodsc'] ) {
		$registro = array(
			'simec' => $acao_simec,
			'sigplan' => $acao
		);
		array_push( $diferenca, $registro );
	}
}

?>

<?php if( !empty( $diferenca ) ): ?>
	<table cellspacing="0" cellpadding="5">
		<caption>Relatório de Diferença<br/><?= date( "d/m/Y H:i" ) ?></caption>
		<thead>
			<tr style="background-color: #cfcfcf;">
				<th>&nbsp;</th>
				<th>Ação</th>
				<th colspan="2">SIMEC</th>
				<th colspan="2">SIGPLAN</th>
			</tr>
		</thead>
		<tbody>
			<?php foreach( $diferenca as $indice => $registro ): ?>
				<tr style="background-color: <?= $indice % 2 ? '#ededed' : '#ffffff' ?>;">
					<td><?= $registro['simec']['acaid'] ?></td>
					<td><?= $registro['simec']['unicod'] .'.'. $registro['simec']['prgcod'] .'.'. $registro['simec']['acacod'] .'.'. $registro['simec']['loccod'] ?></td>
					<td><?= $registro['simec']['procod'] ?></td>
					<td><?= $registro['simec']['prodsc'] ?></td>
					<td><?= $registro['sigplan']['procod'] ?></td>
					<td><?= $registro['sigplan']['prodsc'] ?></td>
				</tr>
			<?php endforeach; ?>
		</tbody>
	</table>
<?php endif; ?>