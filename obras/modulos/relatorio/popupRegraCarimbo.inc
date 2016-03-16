<?php
$sql = "SELECT
			t.tpodsc,
			r.rcdqtdhomologacao,
		    r.rcdqtd30exec,
		    r.rcdqtd50exec,
		    r.rcdqtd75exec,
		    r.rcdqtd100exec
		FROM
			obras.regracarimbo r
		    inner join obras.tipologiaobra t on t.tpoid = r.tpoid";

/*$arRegras = $db->carregar($sql);
$arRegras = $arRegras ? $arRegras : array();*/
?>
<title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<?
$cabecalho = array("Tipo de Obra","Homologação (dias)","25% de Execução (dias)","50% de Execução (dias)","75% de Execução (dias)", "100% de Execução (dias)");
$db->monta_lista_simples( $sql, $cabecalho, 100000, 1, 'N', '100%');
?>