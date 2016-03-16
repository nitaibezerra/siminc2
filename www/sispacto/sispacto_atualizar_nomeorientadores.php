<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_desenvolvimento";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

include "_constantes.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select * from sispacto.tutoresproletramento where nome='' or nome is null";

$users = $db->carregar($sql);

?>
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<div id="resultado"></div>

<script>
var comp = new dCPF();

<? foreach($users as $us) : ?>	
comp.buscarDados('<?=$us['cpf'] ?>');
document.getElementById('resultado').innerHTML += "UPDATE sispacto.tutoresproletramento SET nome='"+comp.dados.no_pessoa_rf+"' WHERE cpf='<?=$us['cpf'] ?>';";
<? endforeach; ?>
</script>