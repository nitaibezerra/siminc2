<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Previsão de envio de pagamento</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$sql = "select m.estuf, m.mundescricao, i.iuscpf, i.iusnome, pp.pfldsc||case when pp.pflcod=1021 then '( '||i.iusrede||' )' else '' end as perfil, lpad(f.fpbmesreferencia::text, 2, '0')||'/'||f.fpbanoreferencia as periodoreferencia, pbovlrpagamento
from escolaterra.pagamentobolsista p 
inner join workflow.documento d on d.docid = p.docid 
inner join seguranca.perfil pp on pp.pflcod = p.pflcod
inner join escolaterra.identificacaousuario i on i.iusid = p.iusid 
inner join territorios.municipio m on m.muncod = i.muncodatuacao 
inner join escolaterra.periodoreferencia f on f.fpbid = p.fpbid 
where d.esdid=1348 and case when pp.pflcod=1021 then i.iusrede is not null else true end ORDER BY i.iusnome, f.fpbid
		";

$cabecalho = array("UF","Município","CPF","Nome","Perfil","Período de referência de bolsa","Valor(R$)");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);

?>