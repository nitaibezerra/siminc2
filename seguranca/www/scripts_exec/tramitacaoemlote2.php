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
and me.memclassificacaoescola = 'U'
and me.memanoreferencia = 2012
and esd.esdid = 33
and me.entcodent in (select entcodent 
                               from pdeescola.memaiseducacao   
                               where memanoreferencia = 2011 
                                    and memstatus = 'A' 
                                    and memclassificacaoescola = 'U')
and me.memid NOT in (
            --ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6
            select me.memid
            from pdeescola.meatividade me
            inner join pdeescola.metipoatividade t using(mtaid)
            inner join pdeescola.memaiseducacao e using(memid)
            left join entidade.entidade et using(entid)
            left join entidade.endereco ed using(entid)
            left join territorios.municipio mu using(muncod)
            where e.memstatus = 'A'
--          and ed.estuf = ''      ---------------PARAMETRO
--          and mu.muncod = ''     ---------------PARAMETRO  
            and e.memclassificacaoescola = 'U'
            and e.memanoreferencia = 2012
            and me.meaano = 2012
            group by me.memid
            having count(me.meaid) < 5 or count(me.meaid) > 6

UNION

            --ESCOLAS SEM MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
            select me.memid
            from pdeescola.memaiseducacao me
            inner join entidade.entidade et using(entid)
            inner join entidade.endereco ed using(entid)
            inner join territorios.municipio mu using(muncod)
            where me.memstatus = 'A'
            and me.memclassificacaoescola = 'U'
            and me.memanoreferencia = 2012
--          and ed.estuf = ' '      ---------------PARAMETRO
--          and mu.muncod = ' '      ---------------PARAMETRO
            and me.memid not in     (

                        select distinct a.memid
                        from pdeescola.meatividade a 
                        inner join pdeescola.metipoatividade b 
                                   using(mtaid)
                        inner join pdeescola.metipoatividade 
                                   using(mtmid)
                        where a.meaano = 2012
                        and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
                                               )
            and me.memid not in     (
                        select distinct a.memid
                        from pdeescola.meatividade a 
                        inner join pdeescola.metipoatividade b 
                                   using(mtaid)
                        inner join pdeescola.metipoatividade 
                                   using(mtmid)
                        where a.meaano = 2011
                        and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
                                               )

UNION
            ---ESCOLAS COM MAIS DE 4 MACROCAMPOS
            select memid
            from (
                        select mem.memid, mtp.mtmid, mem.entcodent
                          from pdeescola.memaiseducacao mem
                          inner join pdeescola.meatividade mea
                                   using(memid)
                          inner join pdeescola.metipoatividade met
                                   using(mtaid)
                          inner join pdeescola.metipomacrocampo mtp
                                   using(mtmid)
                          left join entidade.entidade et 
                                   using(entid)
                          left join entidade.endereco ed 
                                   using(entid)
                          left join territorios.municipio mu 
                                   using(muncod)
                          where memanoreferencia = 2012
--                          and ed.estuf = ' '      ---------------PARAMETRO
--                          and mu.muncod = ' '     ---------------PARAMETRO
                            and memstatus = 'A'
                            and memclassificacaoescola = 'U'
                            group by 1,2, 3
            ) Macro
            group by Macro.memid
            having count(memid) > 4
UNION
            --NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE
            select memid 
              from (
                        select a.memid, qtd1, qtd2
                          from pdeescola.memaiseducacao m
                          left join entidade.entidade et 
                                   using(entid)
                          left join entidade.endereco ed 
                                   using(entid)
                          left join territorios.municipio mu 
                                   using(muncod)
                        inner join ( select * from pdeescola.meatividade where meaano = 2012 ) a
                                   on m.memid = a.memid
                        inner join (select memid, mapano, sum(mapquantidade) as qtd1 from pdeescola.mealunoparticipante group by memid, mapano ) b 
                                   on b.memid = m.memid and b.mapano = m.memanoreferencia
                        inner join (select memid, p.meaid, meaano, sum(mpaquantidade) as qtd2
                              from pdeescola.mealunoparticipanteatividade p
                        inner join pdeescola.meatividade a 
                                   on p.meaid = a.meaid
                        where a.meaano = 2012 
                        group by memid, p.meaid, meaano
                        ) c on c.meaano = a.meaano 
                        and c.memid = a.memid 
                        and c.meaid = a.meaid
                        where m.memstatus = 'A'
--                        and ed.estuf = ' '      ---------------PARAMETRO
--                        and mu.muncod = ' '     ---------------PARAMETRO
                        and m.memanoreferencia = 2012
                        and m.memclassificacaoescola = 'U'
                        and qtd1 = qtd2
                        order by memid
                        ) as x
            group by memid
            having count(memid) < 5

UNION
            --ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
            select e.memid
            from pdeescola.memaiseducacao e
            inner join entidade.entidade et using(entid)
            inner join entidade.endereco ed using(entid)
            inner join territorios.municipio mu using(muncod)

            left join ( 
                           select memid, sum(coalesce(mapquantidade,0)) as mapquantidade 
                             from pdeescola.mealunoparticipante where mapano = 2012 
                          group by memid ) p ON p.memid = e.memid
            left join ( select entcodent, sum(coalesce(mecquantidadealunos,0)) as mecquantidadealunos 
                              from pdeescola.mecenso where mecanoreferencia = 2012 
                          group by entcodent ) m on e.entcodent = m.entcodent
            where e.memstatus = 'A'
            and e.memclassificacaoescola = 'U'
            and e.memanoreferencia = 2012
--          and ed.estuf = ' '      ---------------PARAMETRO
--          and mu.muncod = ' '      ---------------PARAMETRO
            and p.mapquantidade > m.mecquantidadealunos
                        )

order by estuf, entnome


";
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