<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Cursos da Instituição<br><span style="font-size: 12px;">Selecione um CAMPUS para monitorar seus cursos<span>', '' );

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);


$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entidunidade'] ;

$autoriazacaoconcursos = new autoriazacaoconcursos();
// cria o cabeçalho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entid);

// lista os campi da unidade
$autoriazacaoconcursos->listacampusedital( $entid );

?>
<script>
	function abredadoscampus( entid ){
		return window.location = '?modulo=principal/cursosevagas/listaCurso&acao=A&entidcampus=' + entid;
	}
</script>