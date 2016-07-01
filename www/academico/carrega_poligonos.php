<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */
// inicializa sistema
	// carrega as funções gerais
include_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();


if($_REQUEST['uf2']) {
	$sql = "select ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(estpoligono, 2249), 10000),4291),2, 0) as poli, REPLACE(REPLACE(st_astext(st_centroid(estpoligono)),'POINT(',''),')','') as centro, estuf from territoriosgeo.estado where estuf='".$_REQUEST['uf2']."'";
} else {
	$sql = "SELECT ST_asGeoJSON(ST_transform(ST_simplify(ST_transform(munpoligono, 2249), 10000),4291),2, 0) as poli, muncod, removeacento(mundescricao) as mundescricao, estuf, '#f6ead9' as cor
	from territoriosgeo.municipio mun
	where munpoligono is not null
	".(($_REQUEST['uf'])?" and mun.estuf in ('".$_REQUEST['uf']."')":" ")."
	".(($_REQUEST['muncod'])?" and mun.muncod in ('".$_REQUEST['muncod']."') ":" ");
}
$dados = $db->carregar($sql); 
//dbg($dados,1);
echo JSON_encode($dados);
?>