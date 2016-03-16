<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
$mic = "|--Todos--";
if(isset($_REQUEST['uf'])){
	$db = new cls_banco();
	$sql = "select mic.micdsc as descricao, mic.miccod as codigo, mes.estuf 
			from territorios.microregiao as mic 
			inner join territorios.mesoregiao as mes
			on mic.mescod = mes.mescod ";
	$sql .= " where mes.estuf = '" . $_REQUEST['uf'] . "'";
	$arMicroregiao = $db->carregar($sql);
	foreach($arMicroregiao as $microregiao){
		if($microregiao != ""){
			$mic .= "#%" . $microregiao['codigo'] . "|" . $microregiao['descricao'];
		}
	}
}
echo $mic;
?>