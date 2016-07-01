<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de orçamento</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$sql = "select uu.unisigla||' - '||uu.uninome as uni, gdedesc, orcvlrunitario, o.orcvlrexecutado from sispacto2.orcamento o
		inner join sispacto2.universidadecadastro u on u.uncid = o.uncid
		inner join sispacto2.universidade uu on uu.uniid = u.uniid
		inner join sispacto2.grupodespesa gd on gd.gdeid = o.gdeid
		where o.orcstatus='A'
		order by 1,2";

$cabecalho = array("IES","Grupo de Despesa","Valor previsto","Valor executado");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);

		
		
?>