<?php

ini_set("memory_limit","500M");
set_time_limit(0);

include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QImpressao.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";


header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

$usucpf = $_SESSION['usucpf'];

?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

</script>
<center>
<?php 
	$qrpid = $_REQUEST['qrpid'];
	$obImprime = new QImpressao( array('tema' => 5) );
	echo $obImprime->montaArvore();
?>
</center>