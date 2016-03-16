<?php

include_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/ChecklistFnde.class.inc";

function montarPainelBaseNacional(){
    global $db;

    $sql1 = "select count(*) as valor,
                    case
                        when bncperg1 = 'a' then 'a - Sim'
                        when bncperg1 = 'b' then 'b - Não'
                        when bncperg1 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg1 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg1
                    end as descricao,
                    'Educação Infantil' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
            group by descricao, categoria
            order by descricao";

    $sql2 = "select count(*) as valor,
                    case
                        when bncperg2 = 'a' then 'a - Sim'
                        when bncperg2 = 'b' then 'b - Não'
                        when bncperg2 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg2 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg2
                    end as descricao,
                    'Ensino Fundamental' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
            group by descricao, categoria
            order by descricao";

    $sql3 = "select count(*) as valor,
                    case
                        when bncperg3 = 'a' then 'a - Sim'
                        when bncperg3 = 'b' then 'b - Não'
                        when bncperg3 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg3 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg3
                    end as descricao,
                    'Ensino Médio' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
            group by descricao, categoria
            order by descricao";

    $sql1Mun = "select count(*) as valor,
                    case
                        when bncperg1 = 'a' then 'a - Sim'
                        when bncperg1 = 'b' then 'b - Não'
                        when bncperg1 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg1 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg1
                    end as descricao,
                    'Educação Infantil' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                left join territorios.municipio mun on mun.muncod = iu.muncod
            where iu.itrid = 2
            group by descricao, categoria
            order by descricao";

    $sql2Mun = "select count(*) as valor,
                    case
                        when bncperg2 = 'a' then 'a - Sim'
                        when bncperg2 = 'b' then 'b - Não'
                        when bncperg2 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg2 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg2
                    end as descricao,
                    'Ensino Fundamental' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                left join territorios.municipio mun on mun.muncod = iu.muncod
            where iu.itrid = 2
            group by descricao, categoria
            order by descricao";

    $sql3Mun = "select count(*) as valor,
                    case
                        when bncperg3 = 'a' then 'a - Sim'
                        when bncperg3 = 'b' then 'b - Não'
                        when bncperg3 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg3 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg3
                    end as descricao,
                    'Ensino Médio' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                left join territorios.municipio mun on mun.muncod = iu.muncod
            where iu.itrid = 2
            group by descricao, categoria
            order by descricao";

    $sql1Uf = "select count(*) as valor,
                    case
                        when bncperg1 = 'a' then 'a - Sim'
                        when bncperg1 = 'b' then 'b - Não'
                        when bncperg1 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg1 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg1
                    end as descricao,
                    'Educação Infantil' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
            where iu.itrid = 1
            group by descricao, categoria
            order by descricao";

    $sql2Uf = "select count(*) as valor,
                    case
                        when bncperg2 = 'a' then 'a - Sim'
                        when bncperg2 = 'b' then 'b - Não'
                        when bncperg2 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg2 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg2
                    end as descricao,
                    'Ensino Fundamental' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
            where iu.itrid = 1
            group by descricao, categoria
            order by descricao";

    $sql3Uf = "select count(*) as valor,
                    case
                        when bncperg3 = 'a' then 'a - Sim'
                        when bncperg3 = 'b' then 'b - Não'
                        when bncperg3 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg3 = 'd' then 'd - Não possuo informações sobre o assunto'
                        else bncperg3
                    end as descricao,
                    'Ensino Médio' as categoria
            from par.basenacionalcomum bn
                inner join par.instrumentounidade iu on iu.inuid = bn.inuid
            where iu.itrid = 1
            group by descricao, categoria
            order by descricao";

    $sql = "select valor,
                  case
                    when descricao = 'a' then 'a - Sim'
                    when descricao = 'b' then 'b - Não'
                    when descricao = 'c' then 'c - Não, mas está em elaboração'
                    when descricao = 'd' then 'd - Não possuo informações sobre o assunto'
                    else descricao
                  end as descricao, categoria
            from (
                    select  count(*) as valor, bncperg1 as descricao, 'Educação Infantil' as categoria
                    from par.basenacionalcomum bn
                        inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                        left join territorios.municipio mun on mun.muncod = iu.muncod
                    -- where bncperg1 != 'a'
                    group by bncperg1


                    union

                    select  count(*), bncperg2 as categoria, 'Ensino Fundamental' as descricao
                    from par.basenacionalcomum bn
                        inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                        left join territorios.municipio mun on mun.muncod = iu.muncod
                    -- where bncperg1 != 'a'
                    group by bncperg2

                    union

                    select  count(*), bncperg3 as categoria, 'Ensino Médio' as descricao
                    from par.basenacionalcomum bn
                        inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                        left join territorios.municipio mun on mun.muncod = iu.muncod
                    -- where bncperg1 != 'a'
                    group by bncperg3
            ) as foo
            order by categoria, descricao";

    $sqlUf = "  select count(*) as valor, 'Respostas' as descricao, mun.estuf as categoria
                from par.basenacionalcomum bn
                        inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                        left join territorios.municipio mun on mun.muncod = iu.muncod
                group by descricao, categoria
                order by descricao";

    $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
    ?>

    <div>
        <div style="width: 50%; float: left;">
            <?php $grafico->setTitulo('Comparação por Nível de Ensino')->setLabelX(array('align'=>'center'))->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($sql); ?>
        </div>
        <div style="width: 50%; float: left;">
            <?php $grafico->setTitulo('Respostas por UF')->setLabelX(array('align'=>'center'))->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($sqlUf); ?>
        </div>

        <div style="clear: both;"></div>

        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Educação Infantil (Total)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql1); ?>
        </div>
        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Ensino Fundamental (Total)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql2); ?>
        </div>
        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Ensino Médio (Total)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql3); ?>
        </div>

        <div style="clear: both;"></div>

        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Educação Infantil (Municipal)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql1Mun); ?>
        </div>
        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Ensino Fundamental (Municipal)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql2Mun); ?>
        </div>
        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Ensino Médio (Municipal)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql3Mun); ?>
        </div>

        <div style="clear: both;"></div>

        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Educação Infantil (Estadual)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql1Uf); ?>
        </div>
        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Ensino Fundamental (Estadual)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql2Uf); ?>
        </div>
        <div style="width: 30%; float: left;">
            <?php $grafico->setTitulo('Ensino Médio (Estadual)')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql3Uf); ?>
        </div>

        <div style="clear: both;"></div>
    </div>

    <?php
}

?>