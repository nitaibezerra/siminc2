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
end AS Esfera, me.docid
from pdeescola.memaiseducacao me
inner join entidade.entidade et using(entid)
inner join entidade.endereco ed using(entid)
inner join territorios.municipio mu using(muncod)
inner join workflow.documento d using (docid)
inner join workflow.estadodocumento esd using (esdid)
where me.memstatus = 'A'
and me.memclassificacaoescola = 'R'
and me.memanoreferencia = 2012
and esd.esdid = 33
and me.memid not in (
            --ESCOLAS COM TOTAL DE ATIVIDADES DIFERENTE DE 4 
            select memid 
            from pdeescola.meatividade me
            inner join pdeescola.metipoatividade t using(mtaid)
            inner join pdeescola.memaiseducacao e using(memid)
            left join entidade.entidade et using(entid)
            left join entidade.endereco ed using(entid)
            left join territorios.municipio mu using(muncod)
            where e.memstatus = 'A'
            and e.memclassificacaoescola = 'R'
            and e.memanoreferencia = 2012
            --and ed.estuf = ' '      ---------------PARAMETRO
            --and mu.muncod = ' '      ---------------PARAMETRO
            group by me.memid
            having count(me.meaid) <> 4

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
            and e.memclassificacaoescola = 'R'
            and e.memanoreferencia = 2012
            --and ed.estuf = ' '      ---------------PARAMETRO
            --and mu.muncod = ' '      ---------------PARAMETRO
            and t.mtaid in (730) --COM PST
            group by me.memid

UNION
            --ESCOLAS SEM CAMPO DO CONHECIMENTO
            select me.memid
            from pdeescola.memaiseducacao me
            inner join entidade.entidade et using(entid)
            inner join entidade.endereco ed using(entid)
            inner join territorios.municipio mu using(muncod)
            where me.memstatus = 'A'
            and me.memclassificacaoescola = 'R'
            and me.memanoreferencia = 2012
            --and ed.estuf = ' '       ---------------PARAMETRO
            --and mu.muncod = ' '      ---------------PARAMETRO
            and me.memid not in 
                        (
                        select distinct a.memid
                        from pdeescola.meatividade a 
                        inner join pdeescola.metipoatividade b 
                                   using(mtaid)
                        where a.meaano = 2012
                        and mtaid = 676 --CAMPOS DO CONHECIMENTO
                        order by 1
                        )
            and me.memid not in 
                        (
                        select distinct a.memid
                        from pdeescola.meatividade a 
                        inner join pdeescola.metipoatividade b 
                                   using(mtaid)
                        where a.meaano = 2011
                        and mtaid = 676 --CAMPOS DO CONHECIMENTO
                        order by 1
                        )

UNION
            --ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
            select e.memid
            from pdeescola.memaiseducacao e
            left join entidade.entidade et using(entid)
            left join entidade.endereco ed using(entid)
            left join territorios.municipio mu using(muncod)
            left join ( 
                        select memid, sum(coalesce(mapquantidade,0)) as mapquantidade 
                          from pdeescola.mealunoparticipante 
                         where mapano = 2012 group by memid ) p ON p.memid = e.memid
            left join ( 
                        select entcodent, sum(coalesce(mecquantidadealunos,0)) as mecquantidadealunos 
                          from pdeescola.mecenso 
                         where mecanoreferencia = 2012 group by entcodent ) m on e.entcodent = m.entcodent
            where e.memstatus = 'A'
            --and ed.estuf = ' '      ---------------PARAMETRO
            --and mu.muncod = ' '      ---------------PARAMETRO
            and e.memclassificacaoescola = 'R'
            and e.memanoreferencia = 2012
            and p.mapquantidade > m.mecquantidadealunos

UNION
            --ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADOPARTICIPANTE
            select distinct a.memid
            from pdeescola.meatividade a 
            inner join pdeescola.mealunoparticipanteatividade b using(meaid)
            inner join pdeescola.mealunoparticipante              p 
                        on a.memid = p.memid
            inner join pdeescola.memaiseducacao e
                        on e.memid = a.memid
            where a.meaano = 2012
            and   p.mapano = 2012
            and e.memanoreferencia = 2012
            and e.memstatus = 'A'
            and e.memclassificacaoescola = 'R'
            --and ed.estuf = ' '      ---------------PARAMETRO
            --and mu.muncod = ' '      ---------------PARAMETRO
            group by a.memid, p.memid, a.mtaid
            having sum(b.mpaquantidade) <> sum(mapquantidade)
UNION
            --ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES
            Select memid from
            (
                        SELECT
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
                                   pdeescola.memaiseducacao mme on eaba.memid = mme.memid
                        left join entidade.entidade et using(entid)
                        left join entidade.endereco ed using(entid)
                        left join territorios.municipio mu using(muncod)
                        where   mme.memstatus = 'A'
                          and   mme.memclassificacaoescola = 'R'
                          and   mme.memanoreferencia = 2012
                          --and ed.estuf = ' '      ---------------PARAMETRO
                          --and mu.muncod = ' '      ---------------PARAMETRO
                        group by mme.memid, eaba.eatid
                        order by mme.memid
            ) AS Total
            GROUP BY memid
            having count (memid) < 4

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