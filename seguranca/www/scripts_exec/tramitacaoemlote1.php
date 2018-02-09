<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";

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
end AS Esfera,
me.docid
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
and me.entcodent not in (
                         select entcodent 
                   from pdeescola.memaiseducacao 
                          where memanoreferencia = 2011 
                            and memstatus = 'A'
                            and memclassificacaoescola = 'U'
                                   )
and me.memid not in (
            --ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6
            select me.memid
            from pdeescola.meatividade me
            inner join pdeescola.metipoatividade t using(mtaid)
            inner join pdeescola.memaiseducacao e using(memid)
             left join entidade.entidade et using(entid)
             left join entidade.endereco ed using(entid)
             left join territorios.municipio mu using(muncod)
            where e.memstatus = 'A'
            and e.memclassificacaoescola = 'U'
            and e.memanoreferencia = 2012
            and me.meaano = 2012
            group by me.memid, e.entcodent
            having count(me.meaid) < 5 or count(me.meaid) > 6
UNION
            --ESCOLAS COM PST
            select memid 
            from pdeescola.meatividade me
            inner join pdeescola.metipoatividade t using(mtaid)
            inner join pdeescola.memaiseducacao e using(memid)
            left join entidade.entidade et using(entid)
            left join entidade.endereco ed using(entid)
            left join territorios.municipio mu using(muncod)
            where e.memstatus = 'A'
            and e.memclassificacaoescola = 'U'
            and e.memanoreferencia = 2012
            and t.mtaid in (730) --COM PST
            group by me.memid
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
            and me.memid not in 
            (
            select distinct a.memid
            from pdeescola.meatividade a 
            inner join pdeescola.metipoatividade b 
                        using(mtaid)
            inner join pdeescola.metipoatividade 
                        using(mtmid)
            where a.meaano = 2012
            and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
            order by 1
            )
            and me.memid not in 
            (
            select distinct a.memid
            from pdeescola.meatividade a 
            inner join pdeescola.metipoatividade b 
                        using(mtaid)
            inner join pdeescola.metipoatividade 
                        using(mtmid)
            where a.meaano = 2011
            and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
            order by 1
            )
UNION
            ---ESCOLAS COM MAIS DE 4 MACROCAMPOS
            select memid
            from (
                        select mem.memid, mtp.mtmid, mem.entcodent
                          from pdeescola.memaiseducacao mem
                         left join entidade.entidade et 
                                   using(entid)
                         left join entidade.endereco ed 
                                   using(entid)
                         left join territorios.municipio mu 
                                   using(muncod)
                        inner join pdeescola.meatividade mea
                                   using(memid)
                        inner join pdeescola.metipoatividade met
                                   using(mtaid)
                        inner join pdeescola.metipomacrocampo mtp
                                   using(mtmid)
                        where memanoreferencia = 2012
                          and memstatus = 'A'
                          and memclassificacaoescola = 'U'
                        group by 1,2, 3
                        order by 1
                 ) Macro
            group by Macro.memid
            having count(memid) > 4
UNION
            --ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
            select e.memid
              from pdeescola.memaiseducacao e
            left join entidade.entidade et using(entid)
            left join entidade.endereco ed using(entid)
            left join territorios.municipio mu using(muncod)
            left join ( select memid, sum(coalesce(mapquantidade,0)) as mapquantidade 
                              from pdeescola.mealunoparticipante 
                     where mapano = 2012 
                  group by memid ) p on p.memid = e.memid
            left join ( select entcodent, sum(coalesce(mecquantidadealunos,0)) as mecquantidadealunos 
                              from pdeescola.mecenso 
                             where mecanoreferencia = 2012 
                        group by entcodent ) m on e.entcodent = m.entcodent
            where e.memstatus = 'A'
            and e.memclassificacaoescola = 'U'
            and e.memanoreferencia = 2012
            and p.mapquantidade > m.mecquantidadealunos

UNION
            --NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE
            select memid from (
                        select a.memid, qtd1, qtd2
                          from pdeescola.memaiseducacao m
                        left join entidade.entidade et using(entid)
                        left join entidade.endereco ed using(entid)
                        left join territorios.municipio mu using(muncod)

                        inner join ( select * from pdeescola.meatividade where meaano = 2012 ) a 
                                   on m.memid = a.memid
                        inner join (select memid, mapano, sum(mapquantidade) as qtd1 from pdeescola.mealunoparticipante group by memid, mapano ) b 
                                   on b.memid = m.memid and b.mapano = m.memanoreferencia
                        inner join (select memid, p.meaid, meaano, sum(mpaquantidade) as qtd2
                        from pdeescola.mealunoparticipanteatividade p
                        inner join pdeescola.meatividade a 
                                   on p.meaid = a.meaid
                        where a.meaano = 2012 
                        group by memid, p.meaid, meaano) c 
                                   on c.meaano = a.meaano and c.memid = a.memid and c.meaid = a.meaid
                        where m.memstatus = 'A'
                        and m.memanoreferencia = 2012
                        and m.memclassificacaoescola = 'U'
                        and qtd1 = qtd2
                        order by memid
            ) as x
            group by memid
            having count(memid) < 5

UNION
            --ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADOPARTICIPANTE
            select distinct a.memid
            from pdeescola.meatividade a 
            inner join pdeescola.mealunoparticipanteatividade b using(meaid)
            inner join pdeescola.mealunoparticipante              p 
                        on a.memid = p.memid
            inner join pdeescola.memaiseducacao e
                        on e.memid = a.memid
            left join entidade.entidade et using(entid)
            left join entidade.endereco ed using(entid)
            left join territorios.municipio mu using(muncod)

            where a.meaano = 2012
            and   p.mapano = 2012
            and e.memanoreferencia = 2012
            and e.memstatus = 'A'
            and e.memclassificacaoescola = 'U'
            group by a.memid, p.memid, a.mtaid
            having sum(b.mpaquantidade) <> sum(mapquantidade)

UNION
            --ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES
            Select memid from
            (
            select
                        distinct mme.memid, eaba.eatid
            from
                        pdeescola.meeabatividade eaba
            inner join
                        pdeescola.eabtipoatividade eat on eat.eatid = eaba.eatid
            inner join
                        pdeescola.eabtipooficina eao on eao.eaoid = eaba.eaoid
            inner join
                        pdeescola.eabtipoduracaooficina ead on ead.eadid = eaba.eadid
            inner join
                        pdeescola.eabdiarealizacao edr on edr.edrid = eaba.edrid
            inner join
                        pdeescola.eabtipoespaco eae on eae.eaeid = eaba.eaeid
            inner join 
                        pdeescola.memaiseducacao mme ON eaba.memid = mme.memid
            left join entidade.entidade et using(entid)
            left join entidade.endereco ed using(entid)
            left join territorios.municipio mu using(muncod)

            where   mme.memstatus = 'A'
              and   mme.memclassificacaoescola = 'U'
              and   mme.memanoreferencia = 2012
            group by mme.memid, eaba.eatid
            order by mme.memid
            ) as Total
            group by memid
            having count (memid) < 4

--ESCOLAS QUE POSSUEM ATIVIDADE TECNOLOGIA DA ALFABETIZAÇÃO
UNION
            select a.memid
            from pdeescola.memaiseducacao a
            inner join pdeescola.meatividade b
                         on a.memid = b.memid
                        and a.memanoreferencia = b. meaano
            inner join pdeescola.metipoatividade C
                        on b.mtaid = c.mtaid
            inner join pdeescola.metipomacrocampo d
                        on c.mtmid = d.mtmid
            where a.memanoreferencia = 2012
              and a.memclassificacaoescola = 'U'
              and c.mtasituacao = 'I'
              and c.mtaid = 674 

)

/*
AND me.memid NOT IN
(
--ESCOLAS QUE RECEBERAM RECURSO EM 2011 E NÃO EXECUTARAM ATIVIDADES
--RETIRAR PARA AS VÁLIDAS

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
            and e.memclassificacaoescola = 'U'
            and e2.memanoreferencia = 2011
            and e.memstatus = 'A'
            group by e.memid,  e2.memid
order by e.memid  ) foo 
    where total_atv = total_atv_false 
) */

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