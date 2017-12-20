<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

$db          = new cls_banco();

$sem_cabecalho = true;
$_SESSION['sem_cabecalho'] = true;
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?

include APPRAIZ . "painel/modulos/principal/mapas/mapaProInfancia.inc";

//include APPRAIZ . "includes/rodape.inc";

?>