<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de adesão ao termo de compromisso</b></p>
<?php

$sql = "
select to_char(foo.data::date,'dd/mm/YYYY') as t, foo.total from (
select to_char(iusdatainclusao,'YYYY-mm-dd') as data, count(i.iusd) as total from sispacto.identificacaousuario i
inner join sispacto.tipoperfil t on t.iusd = i.iusd 
where i.iustermocompromisso=true
group by to_char(iusdatainclusao,'YYYY-mm-dd') 
order by 1 desc) foo";
$cabecalho = array("Data","Total");
$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','',true, false, false, true);

?>