<?php

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";

switch ( $_SESSION['sisdiretorio'] ){

	case 'pdeinterativo':
		include_once APPRAIZ . "www/pdeinterativo/_constantes.php";
		include_once APPRAIZ . "www/pdeinterativo/_funcoesplanoestrategico_1.php";
		include_once APPRAIZ . "www/pdeinterativo/_funcoes.php";
	break;
	case 'pdeinterativo2013':
		include_once APPRAIZ . "www/pdeinterativo2013/_constantes.php";
		include_once APPRAIZ . "www/pdeinterativo2013/_funcoesplanoestrategico_1.php";
		include_once APPRAIZ . "www/pdeinterativo2013/_funcoes.php";
	break;
	
}

	
if ( !$db )
{
	$db = new cls_banco();
}

if(!$_REQUEST['docid'] || !$_REQUEST['esdid'] || !$_REQUEST['aedid']) {
	echo "<script>
			alert('Informações não foram passadas corretamente. Refaça o procedimento.');
			window.opener.location='?modulo=inicio&acao=C';
			window.close();
		  </script>";
	exit;
}

$docid = (integer) $_REQUEST['docid'];
$esdid = (integer) $_REQUEST['esdid'];
$aedid = (integer) $_REQUEST['aedid'];
$cmddsc = trim( $_REQUEST['cmddsc'] );
$verificacao = (string) $_REQUEST['verificacao'];
 
// verifica se precisa de comentário e se comentário está preenchido
if ( wf_acaoNecessitaComentario2( $aedid ) && !$cmddsc )
{
	include "alterar_estado_comentario_pdeinterativo.php";
	exit();
}

// trata dado para verificacao externa
$dadosVerificacao = unserialize( stripcslashes( $verificacao ) );
if ( !is_array( $dadosVerificacao ) )
{
	$dadosVerificacao = array();
}

// realiza alteracao de estado
if ( wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao ) )
{
    //var_dump($a);
    //die();
	$mensagem = "Estado alterado com sucesso!";
}
else
{
	$mensagem = wf_pegarMensagem();
	$mensagem = $mensagem ? $mensagem : "Não foi possível alterar estado do documento.";
}

apagarCachePdeInterativo();

?>
<script type="text/javascript">
var winW = 10;
var winH = 10;

if (document.body && document.body.offsetWidth) {
	document.body.offsetWidth=winW;
	document.body.offsetHeight=winH;
}
if (document.compatMode=='CSS1Compat' &&
    document.documentElement &&
    document.documentElement.offsetWidth ) {
    
	document.documentElement.offsetWidth=winW;
	document.documentElement.offsetHeight=winH;
}
if (window.innerWidth && window.innerHeight) {
	window.innerWidth=winW;
	window.innerHeight=winH;
}
	window.opener.wf_atualizarTela( '<?php echo $mensagem ?>', self );
</script>