<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de Bolsistas sem tramitação de pagamento</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$sql = "SELECT m.estuf, m.mundescricao, uni.unisigla, uni.uninome, CASE WHEN abr.esfera='M' THEN 'Municipal' WHEN abr.esfera='E' THEN 'Estadual' END as esfera FROM sispacto.universidadecadastro unc 
		INNER JOIN sispacto.universidade uni ON uni.uniid = unc.uniid 
		INNER JOIN sispacto.estruturacurso ecu ON ecu.uncid = unc.uncid 
		INNER JOIN sispacto.abrangencia abr ON abr.ecuid = ecu.ecuid 
		INNER JOIN territorios.municipio m ON m.muncod = abr.muncod
ORDER BY m.estuf, m.mundescricao";

$cabecalho = array("UF","Município","Sigla","Universidade","Esfera");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);

?>