<?php 
function possuiPerfil( $pflcods ){

	global $db;
	
	if($db->testa_superuser()){
		return true;
	}
	
	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	} else {
		$pflcods = array( (integer) $pflcods );
	} if ( count( $pflcods ) == 0 ) {
		return false;
	}
	$sql = "SELECT
					count(*)
			FROM seguranca.perfilusuario
			WHERE
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
} 

function validaSession($session){
	if( empty($session) ){
		echo "<script>
				alert('Faltam dados na sessão.');
				window.location.href = 'callcenter.php?modulo=inicio&acao=C';
			  </script>";
		die();
	}
}
?>