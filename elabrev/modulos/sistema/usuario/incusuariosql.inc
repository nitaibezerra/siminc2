<?php

	// exclui todas as unidades propostas anteriormente
	$db->executar( "DELETE FROM elabrev.unidproposto WHERE usucpf = '$cpf'" );

	// relaciona o usuário com os programas indicados por ele
	$unidades = (array) $_REQUEST["proposto"]['U'];
	foreach ( $unidades as $unidade ) {
		if ( !$unidade ) {
			continue;
		}
		$sql = sprintf(
			"INSERT INTO elabrev.unidproposto ( usucpf, unicod ) VALUES ( '%s', '%s' )",
			$cpf,
			$unidade
		);
		$db->executar( $sql );
	}

	// relaciona o usuário com os programas indicados por ele
	$programas = (array) $_REQUEST["proposto"]['P'];
	foreach ( $programas as $programa ) {
		if ( !$programa ) {
			continue;
		}
		$sql = sprintf(
			"insert into elabrev.progacaoproposto ( usucpf, prgid ) values ( '%s', '%s' )",
			$cpf,
			$programa
		);
		$db->executar( $sql );
	}

	// relaciona o usuário com as ações indicadas por ele
	$acoes = (array) $_REQUEST["proposto"]['A'];
	foreach ( $acoes as $acao ) {
		if ( !$acao ) {
			continue;
		}
		$acao = explode( '.', $acao );
		$sql = sprintf(
			"insert into elabrev.progacaoproposto ( usucpf, prgid, acacod, unicod ) values ( '%s', '%s', '%s', '%s' )",
			$cpf,
			$acao[0],
			$acao[1],
			$acao[2]
		);
		$db->executar( $sql );
	}

?>