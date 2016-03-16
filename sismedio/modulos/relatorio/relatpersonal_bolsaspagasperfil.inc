<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Número de Bolsas pagas por Perfil</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$sql = "
select 
foo.universidade,
sum(foo.coordenadorgeral) as coordenadorgeral,
sum(foo.coordenadoradjunto) as coordenadoradjunto,
sum(foo.supervisories) as supervisories,
sum(foo.formadories) as formadories,
sum(foo.formadorregional) as formadorregional,
sum(foo.orientadorestudo) as orientadorestudo,
sum(foo.professor) as professor,
sum(foo.coordenadorpedagogico) as coordenadorpedagogico
		
from (

select 
u.unisigla||' - '||u.uninome as universidade, 
case when p.pflcod=".PFL_COORDENADORIES." then 1 else 0 end as coordenadorgeral, 
case when p.pflcod=".PFL_COORDENADORADJUNTOIES." then 1 else 0 end as coordenadoradjunto,
case when p.pflcod=".PFL_SUPERVISORIES." then 1 else 0 end as supervisories,
case when p.pflcod=".PFL_FORMADORIES." then 1 else 0 end as formadories,
case when p.pflcod=".PFL_FORMADORREGIONAL." then 1 else 0 end as formadorregional,
case when p.pflcod=".PFL_ORIENTADORESTUDO." then 1 else 0 end as orientadorestudo,
case when p.pflcod=".PFL_PROFESSORALFABETIZADOR." then 1 else 0 end as professor,
case when p.pflcod=".PFL_COORDENADORPEDAGOGICO." then 1 else 0 end as coordenadorpedagogico

from sismedio.pagamentobolsista p 
inner join workflow.documento d on d.docid = p.docid 
inner join sismedio.universidade u on u.uniid = p.uniid 
where d.esdid=".ESD_PAGAMENTO_EFETIVADO."
		
) foo
group by foo.universidade";

							


$cabecalho = array("IES",
				   "Coordenador-Geral",
				   "Coordenador adjunto",
				   "Supervisor",
				   "Formador da IES",
				   "Formador Regional",
				   "Orientador de Estudo",
				   "Professor",
				   "Coordenador Pedagógico");

$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);

?>