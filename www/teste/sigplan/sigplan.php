<?php

/**
 * @param string $nome
 * @param string $sql
 * @return string
 */
function sigplan_criarDocumento( $nome, $sql ){
	global $db;
	$retorno = $db->executar( $sql, false );
	$quantidade = $db->conta_linhas( $retorno );
	if ( $quantidade > 0 ) {
		$documento = new DOMDocument( "1.0", "utf-8" );
		$acoes = $documento->appendChild( new DOMElement( "ArrayOf{$nome}" ) );
		for( $i = 0; $i <= $quantidade; $i++ ){
			$registro = $db->carrega_registro( $retorno, $i );
			$acao = $acoes->appendChild( new DOMElement( $nome ) );
			foreach ( $registro as $atributo => $valor ) {
				if ( empty( $valor ) || is_integer( $atributo ) ) {
					continue;
				}
				$valor = $valor == 't' ? 1 : $valor;
				$valor = $valor == 'f' ? 0 : $valor;
				$valor = str_replace( "'", "&apos;", utf8_encode( simec_htmlspecialchars( $valor ) ) );
				$acao->appendChild( new DOMElement( $atributo, $valor ) );
			}
		}
		return $documento->saveXML();
	}
	return null;
}

?>