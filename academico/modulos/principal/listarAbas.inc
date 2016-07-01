<?php
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas
#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Concursos da Instituição<br><span style="font-size: 12px;">Selecione um CAMPUS para monitorar seus concursos<span>', '' );

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio($_SESSION['academico']['orgid']);


$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entidunidade'] ;

/*
 * VALIDANDO SE EXISTE O ENTID
 * FEITO POR ALEXANDRE DOURADO 16/11/2009
 */
if(!$entid) {
	echo "<script>
			alert('Problemas nas variáveis globais. Acesse novamente');
			window.location='academico.php?modulo=inicio&acao=C';
		  </script>";
	exit;
}


$autoriazacaoconcursos = new autoriazacaoconcursos();
?>
<div class=''>
    <div class='col-md-2' style='position:static;'>
        <?=cria_aba_2($abacod_tela, $url, $parametros);?>
    </div>
    <div class='col-md-9' style='position:static;'>
        <?php
        // cria o cabeçalho padrão do módulo
        echo $autoriazacaoconcursos->cabecalho_entidade($entid);

        // lista os campi da unidade
        $autoriazacaoconcursos->listacampusedital( $entid );
        ?>
    </div>
</div>

<script>
	function abredadoscampus( entid ){
		return window.location = '?modulo=principal/listaPortaria&acao=A&entid=' + entid;
	}
</script>