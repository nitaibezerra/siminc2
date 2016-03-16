<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

restore_error_handler();
restore_exception_handler();
error_reporting( E_ALL );

# abre conexão com o banco
$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
$db          = new cls_banco();

# carrega o documento
$diferenca = array();
$documento = simplexml_load_file( APPRAIZ . "arquivos/SIGPLAN/importacao/2007/apoio/CargaProduto.xml" );
foreach( $documento as $produto ) {
	
	# aplica o filtro usado na importação
	$produto = (array) $produto;
	$produto = array_combine( array_map( 'strtolower', array_keys( $produto ) ), array_values( $produto ) );
	$produto = array_map( 'utf8_decode', $produto );
	$produto = array_map( 'trim', $produto );
	$produto = array_map( 'mb_strtolower', $produto );
	
	# captura os dados do simec
	$sql = sprintf( "select prodsc from public.produto where procod = %d", $produto['procod'] );
	$prodsc = mb_strtolower( trim( $db->pegaUm( $sql ) ) );
	
	# monta o script par atualização do banco
	if ( strcmp( $prodsc, $produto['prodsc'] ) != 0 ) {
		$registro = array(
			'procod' => $produto['procod'],
			'sigplan' => $produto['prodsc'],
			'simec' => $prodsc,
		);
		array_push( $diferenca, $registro );
	}
}

?>

<?php if( !empty( $diferenca ) ): ?>
	<table cellspacing="0" cellpadding="5">
		<caption>Relatório de Produtos<br/><?= date( "d/m/Y H:i" ) ?></caption>
		<colgroup>
			<col style="width: 10%;"/>
			<col style="width: 45%;"/>
			<col style="width: 45%;"/>
		</colgroup>
		<thead>
			<tr style="background-color: #cfcfcf;">
				<th>Código</th>
				<th>SIGPLAN</th>
				<th>SIMEC</th>
			</tr>
		</thead>
		<tbody>
			<?php foreach( $diferenca as $indice => $registro ): ?>
				<tr style="background-color: <?= $indice % 2 ? '#ededed' : '#ffffff' ?>;">
					<td style="text-align: center;"><?= $registro['procod'] ?></td>
					<td><?= $registro['sigplan'] ?></td>
					<td><?= $registro['simec'] ?></td>
				</tr>
			<?php endforeach; ?>
		</tbody>
	</table>
<?php endif; ?>