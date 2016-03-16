<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// simula autenticação no sistema
$_SESSION['sisid'] = 4;
$_SESSION['usucpf'] = '';
$_SESSION['usucpforigem'] = '';

$senha = md5_encrypt_senha( '12345', '' );
$sistemas = array( 1, 2, 5, 6, 7, 10, 11 );
$perfis = array( 6, 23, 56, 60, 62, 80, 100 );

// ----- TREINAMENTO

$servidor_bd = 'mecsrv85';
$porta_bd    = '5432';
$nome_bd     = 'simectreinamento';
$usuario_db  = 'simec';
$senha_bd    = 'phpsimecalt';

$db = new cls_banco();

$arquivo = fopen( 'usuarios.csv', 'r' );
$colunas = fgetcsv( $arquivo );

header( 'Content-Type: text/plain; charset=iso-8859-1' );
while( $linha = fgetcsv( $arquivo ) ) {
	
	$usuario = array();
	foreach( $colunas as $indice => $coluna ){
		$usuario[$coluna] = $linha[$indice];
	}
	$usuario['usucpf'] = sprintf( "%011s", $usuario['usucpf'] );
	$usuario['orgcod']   = 26000;
	$usuario['ususenha'] = $senha;
	$usuario['usuchaveativacao'] = 't';
	$usuario['suscod']   = 'A';
	
	$sql = sprintf( "select count(*) from seguranca.usuario where usucpf = '%s'", $usuario['usucpf'] );
	if ( $db->pegaUm( $sql ) == 1 ) {
		$sql = sprintf(
"update seguranca.usuario set
usunome = '%s',
usuemail = '%s',
usufoneddd = '%s',
usufonenum = '%s',
orgcod = '%s',
ususenha = '%s',
usuchaveativacao = '%s',
suscod = '%s'
where usucpf = '%s'",
			addslashes( $usuario['usunome'] ),
			$usuario['usuemail'],
			$usuario['usufoneddd'],
			$usuario['usufonenum'],
			$usuario['orgcod'],
			$usuario['ususenha'],
			$usuario['usuchaveativacao'],
			$usuario['suscod'],
			$usuario['usucpf']
		);
		echo $sql . ";\n\n";
		$db->executar( $sql );
	} else {

		$sql = sprintf(
			"insert into seguranca.usuario ( %s ) values ( '%s' )",
			implode( ",", array_keys( $usuario ) ),
			implode( "','", $usuario )
		);

		echo $sql . ";\n\n";
		$db->executar( $sql );
	}
	
	foreach ( $sistemas as $sistema ) {
		$sql = sprintf(
			"select count(*) from seguranca.usuario_sistema where sisid = %d and usucpf = '%s'",
			$sistema,
			$usuario['usucpf']
		);
		if ( $db->pegaUm( $sql ) == 1 ) {
			$sql = sprintf(
				"update seguranca.usuario_sistema set suscod = 'A' where sisid = %d and usucpf = '%s'",
				$sistema,
				$usuario['usucpf']
			);
			echo $sql . ";\n";
			$db->executar( $sql );
		} else {
			$sql = sprintf(
				"insert into seguranca.usuario_sistema ( sisid, usucpf, suscod ) values ( %d, '%s', 'A' )",
				$sistema,
				$usuario['usucpf']				
			);
			echo $sql . ";\n";
			$db->executar( $sql );
		}
	}
	
	echo "\n\n";
	
	foreach ( $perfis as $perfil ) {
		$sql = sprintf(
			"select count(*) from seguranca.perfilusuario where pflcod = %d and usucpf = '%s'",
			$perfil,
			$usuario['usucpf']
		);
		if ( $db->pegaUm( $sql ) != 1 ) {
			$sql = sprintf(
				"insert into seguranca.perfilusuario ( pflcod, usucpf ) values ( %d, '%s' )",
				$perfil,
				$usuario['usucpf']				
			);
			echo $sql . ";\n";
			$db->executar( $sql );
		}
	}
	
	echo "\n\n\n\n";
}
fclose( $arquivo );
$db->rollback();
