<?php

$entid 		  = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entidunidade'];
$existecampus = academico_existeentidade( $entid );

if( !$existecampus ){
	echo "<script>
			alert('A Unidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

// objeto da classe do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Lista de campus", "");

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio( $_SESSION['academico']['orgid']);

echo $autoriazacaoconcursos->cabecalho_entidade($entid);

$autoriazacaoconcursos->listacampusedital( $entid );
?>
<script>
function abredadoscampus( entid ){
	return window.location = '?modulo=principal/dadoscampus&acao=A&entid=' + entid;
}
</script>