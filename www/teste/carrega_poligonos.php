<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */
// inicializa sistema
define( 'APPRAIZ', '' );
		$nome_bd     = '';
		$servidor_bd = '';
		$porta_bd    = '5432';
		$usuario_db  = '';
		$senha_bd    = '';

//require_once "config.inc";


include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$sql = "SELECT ST_asGeoJSON(the_geom,2, 0) as poli, muncod, removeacento(mundescricao) as mundescricao, estuf, '#f6ead9' as cor
from municipios_br m
inner join territorios.municipio mun on m.codigo_mun = mun.muncod
where mun.estuf in ('".$_REQUEST['uf']."')";
$dados = $db->carregar($sql); 
echo JSON_encode($dados);
?>
