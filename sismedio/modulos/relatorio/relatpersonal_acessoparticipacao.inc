<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de acesso e participação</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$sql = "select 
foo.lemuf,
sum(foo.total) as total,
sum(foo.totalvalidado) as totalvalidado,
round((sum(foo.totalvalidado)::numeric/sum(foo.total)::numeric)*100,2) as porcentvalidado,
sum(foo.totalemelab) as totalemelab,
round((sum(foo.totalemelab)::numeric/sum(foo.total)::numeric)*100,2) as porcentemelab,
sum(foo.totalemanalise) as totalemanalise,
round((sum(foo.totalemanalise)::numeric/sum(foo.total)::numeric)*100,2) as porcentemanalise,
sum(foo.totalseminteresse) as totalseminteresse,
round((sum(foo.totalseminteresse)::numeric/sum(foo.total)::numeric)*100,2) as porcentseminteresse,
sum(foo.validadasemies) as validadasemies,
round((sum(foo.validadasemies)::numeric/sum(foo.total)::numeric)*100,2) as porcentvalidadasemies,
(sum(foo.totalemelab)+sum(foo.totalemanalise)+sum(foo.totalseminteresse)+sum(foo.validadasemies)) as totalnaoparticipantes,
round(((sum(foo.totalemelab)+sum(foo.totalemanalise)+sum(foo.totalseminteresse)+sum(foo.validadasemies))::numeric/sum(foo.total)::numeric)*100,2) as porcentnaoparticipantes
	

from (

select lemuf, 
       1 as total, 
       case when d.esdid=1091 then 1 else 0 end as totalvalidado,
       case when (d.esdid=972 or d.esdid is null) then 1 else 0 end as totalemelab,
       case when d.esdid=973 then 1 else 0 end as totalemanalise,
       case when d.esdid=1151 then 1 else 0 end as totalseminteresse,
       case when (d.esdid=1091 and a.abrid is null) then 1 else 0 end as validadasemies
       
from sismedio.listaescolasensinomedio e
left join workflow.documento d on e.docid = d.docid 
left join sismedio.abrangencia a on a.lemcodigoinep = e.lemcodigoinep

) foo
group by foo.lemuf
order by foo.lemuf";

		

$cabecalho = array("UF",
				   "Nº de Escolas que ofertam a Etapa do Ensino Médio - Censo 2013",
				   "Nº das Escolas Validadas pelas Secretarias de Educação (Participantes)",
				   "%",
				   "Nº de Escolas - Em elaboração (Não participantes)",
				   "%",
				   "Nº de Escolas - Em Análise (Não participantes)",
				   "%",
				   "Nº de Escolas - que não têm interesse/não têm professores no Ensino Médio (Não participantes)",
				   "%",
				   "Nº de Escolas - Validadas não atendidas por Instituição Pública de Ensino Superior (Não participantes)",
				   "%",
				   "Nº de Escolas não participantes",
				   "%");

$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);

?>