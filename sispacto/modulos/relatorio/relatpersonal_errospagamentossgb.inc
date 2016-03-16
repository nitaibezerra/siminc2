<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de erros no envio de pagamentos no SGB</b></p>
<?php

$sql = "select cmddsc, count(*) as total from sispacto.pagamentobolsista p 
		inner join workflow.documento d on d.docid = p.docid 
		inner join workflow.comentariodocumento c on c.hstid = d.hstid 
		where d.esdid=".ESD_PAGAMENTO_RECUSADO." group by cmddsc order by 2 desc";
$cabecalho = array("Erros","Total");
$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);


?>