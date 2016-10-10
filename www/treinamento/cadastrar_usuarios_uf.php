<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

header( 'Content-Type: text/plain; charset=utf-8' );

// simula autenticação no sistema
$_SESSION['sisid'] = 4;
$_SESSION['usucpf'] = '';
$_SESSION['usucpforigem'] = '';

$senha = md5_encrypt_senha( '12345', '' );
//$sistemas = array( 1, 2, 5, 6, 7 );
//$perfis = array( 6, 23, 62, 56, 60 );
//$sistemas = array( 1, 2, 3, 5, 6, 7, 10, 11 );
$sistemas = array( 32 );
$perfis = array( 219 );
//$sistemas = array( 7 );
//$perfis = array( 60 );

// ----- TREINAMENTO


$servidor_bd = '200.130.3.109';
$porta_bd    = '5432';
$nome_bd     = 'simectreinamento';
$usuario_db  = 'simec';
$senha_bd    = 'phpsimecalt';


$db = new cls_banco();

$arquivo = fopen( 'usuarios_uf.csv', 'r' );
$colunas = fgetcsv( $arquivo );


while( $linha = fgetcsv( $arquivo ) ) {
	$usuario = array_combine( $colunas, $linha );
	$usuario['usunome']  = str_replace( "'", "\\'", $usuario['usunome'] );
	$usuario['usucpf']   = $usuario['usucpf'] ;
	$usuario['orgcod']   = CODIGO_ORGAO_SISTEMA;
	$usuario['ususenha'] = $senha;
	$usuario['usuchaveativacao'] = 't';
	$usuario['suscod']   = 'A';
	$usuario['regcod']   = $usuario['regcod'];

	
	$usuario['usuemail'] = 'email@padrao.org';
	
	$sql = sprintf( "select count(*) from seguranca.usuario where usucpf = '%s'", $usuario['usucpf'] );
	if ( $db->pegaUm( $sql ) == 1 ) {
		$sql = sprintf(
			"update seguranca.usuario set suscod = '%s', usuchaveativacao = '%s', ususenha = '%s' where usucpf = '%s'",
			$usuario['suscod'],
			$usuario['usuchaveativacao'],
			$usuario['ususenha'],
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

		$sql = sprintf(
			"insert into parindigena.usuarioresponsabilidade ( pflcod, usucpf, estuf ) values ( '%s', '%s', '%s' )",
			219,
			$usuario['usucpf'],
			$usuario['regcod']
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
		//echo $sql . ";\n";
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
		//echo $sql . ";\n";
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
//$db->commit();

?>
