<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
error_reporting(-1);
session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '50166794015';
$_SESSION['usucpf'] = '50166794015';

$db = new cls_banco();

$sql = "select et.entcodent, et.entnome, mu.estuf, mu.mundescricao, 
case
            when et.tpcid = 1 then 'Estadual'
            when et.tpcid = 2 then 'Federal'
            when et.tpcid = 3 then 'Municipal'
end AS Esfera, me.docid
from pdeescola.memaiseducacao me
inner join entidade.entidade et using(entid)
inner join entidade.endereco ed using(entid)
inner join territorios.municipio mu using(muncod)
inner join workflow.documento d using (docid)
inner join workflow.estadodocumento esd using (esdid)
where me.memstatus = 'A'
and me.memanoreferencia = 2012
and esd.esdid = 33
and me.memid in (

             select memid 
              from (
            select  e.memid AS memid, e2.memid as memid2,
                                   (select count(*) from pdeescola.meatividade x where e2.memid=x.memid) as total_atv,
                                   (select count(*) from pdeescola.meatividade xx where e2.memid=xx.memid and  xx.meacomecounoano = false) as total_atv_false
                        from pdeescola.memaiseducacao e  
                        inner join pdeescola.memaiseducacao e2
                                   on e.entcodent = e2.entcodent
                        inner join pdeescola.meatividade me
                                   on me.memid = e2.memid 
                        where e2.mempagofnde = 't'
                        and e.memanoreferencia = 2012
                        and e2.memanoreferencia = 2011
                        and e.memstatus = 'A'
                        group by e.memid,  e2.memid
            order by e.memid  ) foo 
                where total_atv = total_atv_false 
            and memid in (  Select memid from
                        (
                        
                                   select  mme.memid, count(eaba.eatid) AS total
                                     from pdeescola.memaiseducacao mme
                                     left join pdeescola.meeabatividade eaba 
                                               using (memid)
                                     left join pdeescola.eabtipoatividade eat 
                                               using (eatid)
                                     left join pdeescola.eabtipooficina eao 
                                               using (eaoid)
                                     left join pdeescola.eabtipoduracaooficina ead 
                                               using (eadid)
                                     left join pdeescola.eabdiarealizacao edr 
                                                 on edr.edrid = eaba.edrid
                                     left join pdeescola.eabtipoespaco eae 
                                               using (eaeid)
                                    where  mme.memstatus = 'A'
                                      and   mme.memanoreferencia = 2012
                                      and   mme.mamescolaaberta <> 't'
                                   --   and mme.entcodent= '31002208'
                                   group by mme.memid--, eaba.eatid 
                        )
                        as Total
                                   group by memid, total
                                   --having total = 0
             )
)";
$lista = $db->carregar($sql);
if($lista[0]) {
	foreach($lista as $l) {
		$docid = $l['docid'];
		$aedid = 68;
		$dados = array();
		$result = wf_alterarEstado( $docid, $aedid, $cmddsc = 'Tramitação feita em lote', $dados);
	
	}
}

$sql = "select et.entcodent, et.entnome, mu.estuf, mu.mundescricao, 
case
            when et.tpcid = 1 then 'Estadual'
            when et.tpcid = 2 then 'Federal'
            when et.tpcid = 3 then 'Municipal'
end AS Esfera, me.docid
from pdeescola.memaiseducacao me
inner join entidade.entidade et using(entid)
inner join entidade.endereco ed using(entid)
inner join territorios.municipio mu using(muncod)
inner join workflow.documento d using (docid)
inner join workflow.estadodocumento esd using (esdid)
where me.memstatus = 'A'
and me.memanoreferencia = 2012
and esd.esdid = 33
and me.memid in (

             select memid 
              from (
            select  e.memid AS memid, e2.memid as memid2,
                                   (select count(*) from pdeescola.meatividade x where e2.memid=x.memid) as total_atv,
                                   (select count(*) from pdeescola.meatividade xx where e2.memid=xx.memid and  xx.meacomecounoano = false) as total_atv_false
                        from pdeescola.memaiseducacao e  
                        inner join pdeescola.memaiseducacao e2
                                   on e.entcodent = e2.entcodent
                        inner join pdeescola.meatividade me
                                   on me.memid = e2.memid 
                        where e2.mempagofnde = 't'
                        and e.memanoreferencia = 2012
                        and e2.memanoreferencia = 2011
                        and e.memstatus = 'A'
                        group by e.memid,  e2.memid
            order by e.memid  ) foo 
                where total_atv = total_atv_false 
            and memid in (  Select memid from
                        (
                        
                                   select  mme.memid, count(eaba.eatid) AS total
                                     from pdeescola.memaiseducacao mme
                                     left join pdeescola.meeabatividade eaba 
                                               using (memid)
                                     left join pdeescola.eabtipoatividade eat 
                                               using (eatid)
                                     left join pdeescola.eabtipooficina eao 
                                               using (eaoid)
                                     left join pdeescola.eabtipoduracaooficina ead 
                                               using (eadid)
                                     left join pdeescola.eabdiarealizacao edr 
                                                 on edr.edrid = eaba.edrid
                                     left join pdeescola.eabtipoespaco eae 
                                               using (eaeid)
                                    where  mme.memstatus = 'A'
                                      and   mme.memanoreferencia = 2012
                                      and   mme.mamescolaaberta = 't'
                                   --   and mme.entcodent= '31002208'
                                   group by mme.memid--, eaba.eatid 
                        )
                        as Total
                                   group by memid, total
                                   having total <> 0
             )

)";
$lista = $db->carregar($sql);
if($lista[0]) {
	foreach($lista as $l) {
		$docid = $l['docid'];
		$aedid = 68;
		$dados = array();
		$result = wf_alterarEstado( $docid, $aedid, $cmddsc = 'Tramitação feita em lote', $dados);
	
	}
}


echo "fim";
?>