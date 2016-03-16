<?php 

require_once '_funcoes_termoCooperacao.php';

$aba = $_REQUEST['aba'] ? $_REQUEST['aba'] : 'macroListaLotes';
include_once APPRAIZ.'elabrev/modulos/relatorio/termoCooperacao/'.$aba.'.inc';

?>