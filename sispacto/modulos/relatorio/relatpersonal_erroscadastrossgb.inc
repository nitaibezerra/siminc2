<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de erros no cadastro de bolsistas no SGB</b></p>
<?php

$sql = "select logresponse, count(*) from sispacto.logsgb where logservico='gravarDadosBolsista' and logerro=true and logdata>= to_char(NOW(),'YYYY-mm-dd')::date - interval '1 day' group by logresponse order by 2 desc";
$cabecalho = array("Erros","Total");
$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','',true, false, false, true);


?>