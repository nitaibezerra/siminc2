/***************************************************************************************
Relatório B
Formação Inicial (2007 e 2008; Estados/Municípios total/Municípios prioritários):
    # média de dias;
    # média de carga horária;
    # distribuição em percentual por faixas de duração da Formação Inicial (1 a 5 dias; 6 a 10 dias; + de 10 dias);
    # distribuição em percentual por faixas de carga horária da Formação Inicial (1 a 40 horas; 41 a 60 horas; + de 60 horas).
*****************************************************************************************/
/*
Banco Postgree
*/

-- Quantidade de PPAlfas 2008 que não possui a formação inicial preenchida
SELECT	count(DISTINCT ppa.co_plano_plurianual) 
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	pba.tb_pba_beneficiario ben
ON	ben.co_plano_plurianual = ppa.co_plano_plurianual
JOIN	municipio mun
ON	ben.co_cidade = mun.muncod
WHERE	ppa.dt_fim_formacao_inicial    IS NULL
AND	ppa.dt_inicio_formacao_inicial IS NULL


-- Quantidade de PPAlfas 2007 que não possuem a quantidade da carga horaria preenchida
SELECT	count(DISTINCT ppa.co_plano_plurianual) 
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	pba.tb_pba_beneficiarios ben
ON	ben.co_plano_plurianual = ppa.co_plano_plurianual
JOIN	municipio mun
ON	ben.co_cidade = mun.muncod
WHERE	ppa.qt_carga_horaria_alfabetizacao_hora  IS NULL


-- Média de dias e carga horaria
-- UF

SELECT	estado.regcod,
	avg(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) AS "media de dias", 		
	avg(ppa.qt_carga_horaria_alfabetizacao_hora) AS "media de carga horaria"
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	
(
  SELECT  DISTINCT b.regcod, a.co_plano_plurianual
  FROM	pba.tb_pba_beneficiarios a
	JOIN municipio b
  ON	a.co_cidade = b.muncod
) estado
ON	ppa.co_plano_plurianual = estado.co_plano_plurianual
GROUP BY estado.regcod
ORDER BY estado.regcod

-- Municipio
SELECT	estado.regcod,
	estado.mundsc,
	avg(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) AS "media de dias", 		
	avg(ppa.qt_carga_horaria_alfabetizacao_hora) AS "media de carga horaria"
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	
(
  SELECT  DISTINCT b.regcod, b.mundsc, a.co_plano_plurianual
  FROM	pba.tb_pba_beneficiarios a
  JOIN	municipio b
  ON		a.co_cidade = b.muncod
  order by b.regcod, b.mundsc
) estado
ON	ppa.co_plano_plurianual = estado.co_plano_plurianual
GROUP BY estado.regcod,
	 estado.mundsc
ORDER BY estado.regcod

-- Municipio Prioritario
SELECT	estado.regcod,
	estado.mundsc,
	avg(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) AS "media de dias", 		
	avg(ppa.qt_carga_horaria_alfabetizacao_hora) AS "media de carga horaria"
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	
(
  SELECT  DISTINCT b.regcod, b.mundsc, a.co_plano_plurianual
  FROM	pba.tb_pba_beneficiarios a
  JOIN	municipio b
  ON	a.co_cidade = b.muncod
  JOIN	pba.tb_pba_municipios_prioritarios c
  ON	c.co_municipio = b.muncod
  ORDER BY b.regcod, b.mundsc
) estado
ON	ppa.co_plano_plurianual = estado.co_plano_plurianual
WHERE	ppa.dt_fim_formacao_inicial    IS NOT NULL
AND	ppa.dt_inicio_formacao_inicial IS NOT NULL
GROUP BY estado.regcod,
		 estado.mundsc
ORDER BY estado.regcod




-- distribuição em percentual por faixas de duração da Formação Inicial (1 a 5 dias; 6 a 10 dias; + de 10 dias);

--UF
SELECT	mun.regcod,
        count(DISTINCT ppa.co_plano_plurianual) AS "total",
        sub_prim."total1-5",
        (sub_prim."total1-5"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual 1-5",
        sub_seg."total 6-10",
        (sub_seg."total 6-10"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual 6-10",
        sub_ter."total +10",
        (sub_ter."total +10"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual +10"
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	pba.tb_pba_beneficiarios ben
ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
JOIN	municipio mun
ON		mun.muncod = ben.co_cidade
LEFT JOIN
(-- 1-5
    SELECT	mun.regcod,
			count(DISTINCT ppa.co_plano_plurianual) AS "total1-5"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) < 6
    AND		ppa.dt_fim_formacao_inicial IS NOT NULL 
    AND		ppa.dt_inicio_formacao_inicial IS NOT NULL 
    GROUP BY	mun.regcod
    ORDER BY	mun.regcod
)sub_prim
ON	mun.regcod = sub_prim.regcod
LEFT JOIN
(-- 6-10
    SELECT	mun.regcod,
	count(DISTINCT ppa.co_plano_plurianual)  AS "total 6-10"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) > 5
    AND		(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) < 11
    AND		ppa.dt_fim_formacao_inicial IS NOT NULL 
    AND		ppa.dt_inicio_formacao_inicial IS NOT NULL 
    GROUP BY	mun.regcod
    ORDER BY	mun.regcod
)sub_seg
ON	mun.regcod = sub_seg.regcod
LEFT JOIN
(-- +10
    SELECT	mun.regcod,
			count(DISTINCT ppa.co_plano_plurianual) AS "total +10"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) > 10
    AND		ppa.dt_fim_formacao_inicial IS NOT NULL 
    AND		ppa.dt_inicio_formacao_inicial IS NOT NULL 
    GROUP BY	mun.regcod
    ORDER BY	mun.regcod
)sub_ter
ON	mun.regcod = sub_ter.regcod
WHERE	ppa.dt_fim_formacao_inicial IS NOT NULL 
AND 	ppa.dt_inicio_formacao_inicial IS NOT NULL 
GROUP BY    mun.regcod, 
	    sub_prim."total1-5",
            sub_seg."total 6-10",
	    sub_ter."total +10"



-- Municipio VAI FICAR FORA


SELECT	mun.regcod,
   		mun.muncod,
		mun.mundsc,
       -- count(DISTINCT ppa.co_plano_plurianual) AS "total",
       sub_prim."total1-5",
       -- (sub_prim."total1-5"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual 1-5",
       sub_seg."total 6-10",
       -- (sub_seg."total 6-10"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual 6-10",
       sub_ter."total +10"
       -- (sub_ter."total +10"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual +10"
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	pba.tb_pba_beneficiarios ben
ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
JOIN	municipio mun
ON		mun.muncod = ben.co_cidade
LEFT JOIN
(-- 1-5
    SELECT	mun.regcod,
    		mun.muncod,
    		mun.mundsc,
            count(DISTINCT ppa.co_plano_plurianual) AS "total1-5"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) < 6
    GROUP BY	mun.regcod,
	    		mun.muncod,
    			mun.mundsc
    ORDER BY	mun.regcod
)sub_prim
ON	mun.muncod = sub_prim.muncod
LEFT JOIN
(-- 6-10
    SELECT	mun.regcod,
    		mun.muncod,
    		mun.mundsc,
            count(DISTINCT ppa.co_plano_plurianual)  AS "total 6-10"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) > 5
    AND		(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) < 11
    GROUP BY	mun.regcod,
	    		mun.muncod,
    			mun.mundsc
    ORDER BY	mun.regcod
)sub_seg
ON	mun.muncod = sub_seg.muncod
LEFT JOIN
(-- +10
    SELECT	mun.regcod,
    		mun.muncod,
    		mun.mundsc,
            count(DISTINCT ppa.co_plano_plurianual) AS "total +10"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) > 10
    GROUP BY	mun.regcod,
	    		mun.muncod,
    			mun.mundsc
    ORDER BY	mun.regcod
)sub_ter
ON	mun.muncod = sub_ter.muncod 
WHERE	ppa.dt_fim_formacao_inicial IS NOT NULL 
AND 	ppa.dt_inicio_formacao_inicial IS NOT NULL 
GROUP BY	mun.regcod, 
    		mun.muncod,
			mun.mundsc,           
	       	sub_prim."total1-5",
            sub_seg."total 6-10",
			sub_ter."total +10"



--# distribuição em percentual por faixas de carga horária da Formação Inicial (1 a 40 horas; 41 a 60 horas; + de 60 horas).

--UF
SELECT	mun.regcod,
        count(DISTINCT ppa.co_plano_plurianual) AS "total",
        sub_prim."total 1-40",
        sub_prim."total 1-40"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual 1-40",
        sub_seg."total 41-60",
        sub_seg."total 41-60"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual 41-60",
        sub_ter."total +60",
        sub_ter."total +60"*100.0)/count(DISTINCT ppa.co_plano_plurianual) AS "percentual +60"
FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
JOIN	pba.tb_pba_beneficiarios ben
ON	ppa.co_plano_plurianual = ben.co_plano_plurianual
JOIN	municipio mun
ON	mun.muncod = ben.co_cidade
LEFT JOIN
(-- 1-5
    SELECT	mun.regcod,
		count(DISTINCT ppa.co_plano_plurianual) AS "total 1-40"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	ppa.qt_carga_horaria_alfabetizacao_hora < 41
    AND		ppa.qt_carga_horaria_alfabetizacao_hora IS NOT NULL 
    GROUP BY	mun.regcod
    ORDER BY	mun.regcod
)sub_prim
ON	mun.regcod = sub_prim.regcod
LEFT JOIN
(-- 6-10
    SELECT	mun.regcod,
		count(DISTINCT ppa.co_plano_plurianual)  AS "total 41-60"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	ppa.qt_carga_alfabetizacao_hora > 40
    AND		ppa.qt_carga_alfabetizacao_hora < 61
    AND		ppa.qt_carga_alfabetizacao_hora IS NOT NULL 
    GROUP BY	mun.regcod
    ORDER BY	mun.regcod
)sub_seg
ON	mun.regcod = sub_seg.regcod
LEFT JOIN
(-- +10
    SELECT	mun.regcod,
		count(DISTINCT ppa.co_plano_plurianual) AS "total +60"
    FROM	pba.tb_pba_plano_plurianual_alfabetizacao ppa
    JOIN	pba.tb_pba_beneficiarios ben
    ON		ppa.co_plano_plurianual = ben.co_plano_plurianual
    JOIN	municipio mun
    ON		mun.muncod = ben.co_cidade
    WHERE	(ppa.dt_fim_formacao_inicial - ppa.dt_inicio_formacao_inicial) > 60
    AND		ppa.qt_carga_alfabetizacao_hora IS NOT NULL 
    GROUP BY	mun.regcod
    ORDER BY	mun.regcod
)sub_ter
ON              mun.regcod = sub_ter.regcod
WHERE		ppa.qt_carga_alfabetizacao_hora IS NOT NULL 
GROUP BY	mun.regcod, 
		sub_prim."total 1-40",
		sub_seg."total 41-60",
		sub_ter."total +60"


/*
