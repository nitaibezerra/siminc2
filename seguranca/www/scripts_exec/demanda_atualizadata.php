<?php
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
$db->executar("update seguranca.sistema SET sisdtalteradados = current_date WHERE sisid=44 AND sisdtalteradados + '60 days'::interval < current_date;");
$db->commit();

//echo "feito";
?>