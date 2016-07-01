-- View: obras2.v_regras_obras_evolucao

-- DROP VIEW obras2.v_regras_obras_evolucao;

CREATE OR REPLACE VIEW obras2.v_regras_obras_evolucao AS 
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid, 'Obra com avanço inferior a 10% nas três últimas vistorias' AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_municipal o
         WHERE o.situacaoobra <> 691 AND o.orgid = 3 AND (o.situacaoobra = ANY (ARRAY[690, 691]))
         AND
          -- Regra do bloqueio: Pega a última vistoria maior que 70 ou 100 dias com < 85% de execução e >= 85% respectivamente
          -- Verifica se a partir da vistoria recuperada em diante existe alguma vistoria paralisada
          -- Se sim, a regra busca a vistoria responsável por tramitar a obra para execução
          CASE WHEN (
            SELECT s.supdata
             FROM obras2.supervisao s
            WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata >= CASE WHEN o.obrpercentultvistoria < 85::numeric THEN (select now() - '70 days'::interval) ELSE (select now() - '100 days'::interval) END AND s.staid = 2
            ORDER BY s.supdata ASC LIMIT 1
          ) IS NOT NULL THEN

            -- A partir da data que a obra entrou em execução a regra calcula a evolução
            -- Obras com percentual >= 85%
              -- Entre 40 e 80 dias o acanço deve ser de no minimo 3%
              -- Entre 80 e 120 dias o acanço deve ser de no minimo 6%
              -- Maior que 120 o avanço deve ser de no minimo 10%
            -- Obras com percentual < 85%
              -- Entre 30 e 60 dias o acanço deve ser de no minimo 3%
              -- Entre 60 e 90 dias o acanço deve ser de no minimo 6%
              -- Maior que 90 o avanço deve ser de no minimo 10%
            ( SELECT CASE
                -- < 85%
                WHEN (media >= 30 and media < 60 and o.obrpercentultvistoria < 85 and (o.obrpercentultvistoria - percentual) <= 3) THEN TRUE
                WHEN (media >= 60 and media < 90 and o.obrpercentultvistoria < 85 and (o.obrpercentultvistoria - percentual) <= 6) THEN TRUE
                WHEN (media >= 90 and o.obrpercentultvistoria < 85 and (o.obrpercentultvistoria - percentual) <= 10) THEN TRUE

                -- >= 85%
                WHEN (media >= 40 and media < 80 and o.obrpercentultvistoria >= 85 and (o.obrpercentultvistoria - percentual) <= 3) THEN TRUE
                WHEN (media >= 80 and media < 120 and o.obrpercentultvistoria >= 85 and (o.obrpercentultvistoria - percentual) <= 6) THEN TRUE
                WHEN (media >= 120 and o.obrpercentultvistoria >= 85 and (o.obrpercentultvistoria - percentual) <= 10) THEN TRUE
              ELSE FALSE
              END AS exibeObra
              FROM
              (
                SELECT date_part('day',now() - s.supdata) as media, ( SELECT
                      CASE
                          WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                          ELSE 0::numeric
                      END AS total
                        FROM obras2.cronograma cro
                          JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid AND i.obrid = o.obrid
                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                       WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o.obrid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid) AS percentual
                 FROM obras2.supervisao s
                WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata >= CASE WHEN o.obrpercentultvistoria < 85::numeric THEN (select now() - '70 days'::interval) ELSE (select now() - '100 days'::interval) END AND s.staid = 1
                AND s.supdata >
                (SELECT s.supdata
                   FROM obras2.supervisao s
                  WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata >= CASE WHEN o.obrpercentultvistoria < 85::numeric THEN (select now() - '70 days'::interval) ELSE (select now() - '100 days'::interval) END AND s.staid = 2
                  ORDER BY s.supdata DESC LIMIT 1)
                ORDER BY s.supdata ASC LIMIT 1
              ) as q2
            )


          ELSE

            -- Quando a obra não possui um supervisão de paralisação, pega o percentual da primeira vistoria menor que 70 ou 100 dias
            (  o.obrpercentultvistoria - ( SELECT sup.percentual
               FROM obras2.supervisao s
                  JOIN ( SELECT DISTINCT s.supdata, s.supid, ( SELECT
                    CASE
                        WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                        ELSE 0::numeric
                    END AS total
                       FROM obras2.cronograma cro
                         JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid AND i.obrid = o.obrid
                      LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                     WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o.obrid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid) AS percentual
                   FROM obras2.supervisao s
                  WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata < CASE WHEN o.obrpercentultvistoria >= 85 THEN (now() - '70 days'::interval) ELSE (now() - '100 days'::interval) END
                  ORDER BY s.supdata DESC
                 LIMIT 1) sup ON sup.supid = s.supid
                 ORDER BY s.supdata DESC) ) < 10::numeric

          END



         AND NOT (o.obrid IN ( SELECT p.obrid
               FROM obras2.desbloqueioobra d
          JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
         WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
                  FROM obras2.desbloqueioobra s
                 WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
                 GROUP BY s.pdoid), 0)))
UNION 
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid, 'Obra com avanço inferior a 10% nas três últimas vistorias' AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_estadual o
           JOIN entidade.entidade e ON e.entid = o.entid
         WHERE e.entnumcpfcnpj IN ('', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '') AND o.situacaoobra <> 691 AND o.orgid = 3  AND (o.situacaoobra = ANY (ARRAY[690, 691]))
         AND

          -- Regra do bloqueio: Pega a última vistoria maior que 70 ou 100 dias com < 85% de execução e >= 85% respectivamente
          -- Verifica se a partir da vistoria recuperada em diante existe alguma vistoria paralisada
          -- Se sim, a regra busca a vistoria responsável por tramitar a obra para execução
          CASE WHEN (
            SELECT s.supdata
             FROM obras2.supervisao s
            WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata >= CASE WHEN o.obrpercentultvistoria < 85::numeric THEN (select now() - '70 days'::interval) ELSE (select now() - '100 days'::interval) END AND s.staid = 2
            ORDER BY s.supdata ASC LIMIT 1
          ) IS NOT NULL THEN

            -- A partir da data que a obra entrou em execução a regra calcula a evolução
            -- Obras com percentual >= 85%
              -- Entre 40 e 80 dias o acanço deve ser de no minimo 3%
              -- Entre 80 e 120 dias o acanço deve ser de no minimo 6%
              -- Maior que 120 o avanço deve ser de no minimo 10%
            -- Obras com percentual < 85%
              -- Entre 30 e 60 dias o acanço deve ser de no minimo 3%
              -- Entre 60 e 90 dias o acanço deve ser de no minimo 6%
              -- Maior que 90 o avanço deve ser de no minimo 10%
            ( SELECT CASE
                -- < 85%
                WHEN (media >= 30 and media < 60 and o.obrpercentultvistoria < 85 and (o.obrpercentultvistoria - percentual) <= 3) THEN TRUE
                WHEN (media >= 60 and media < 90 and o.obrpercentultvistoria < 85 and (o.obrpercentultvistoria - percentual) <= 6) THEN TRUE
                WHEN (media >= 90 and o.obrpercentultvistoria < 85 and (o.obrpercentultvistoria - percentual) <= 10) THEN TRUE

                -- >= 85%
                WHEN (media >= 40 and media < 80 and o.obrpercentultvistoria >= 85 and (o.obrpercentultvistoria - percentual) <= 3) THEN TRUE
                WHEN (media >= 80 and media < 120 and o.obrpercentultvistoria >= 85 and (o.obrpercentultvistoria - percentual) <= 6) THEN TRUE
                WHEN (media >= 120 and o.obrpercentultvistoria >= 85 and (o.obrpercentultvistoria - percentual) <= 10) THEN TRUE
              ELSE FALSE
              END AS exibeObra
              FROM
              (
                SELECT date_part('day',now() - s.supdata) as media, ( SELECT
                      CASE
                          WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                          ELSE 0::numeric
                      END AS total
                        FROM obras2.cronograma cro
                          JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid AND i.obrid = o.obrid
                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                       WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o.obrid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid) AS percentual
                 FROM obras2.supervisao s
                WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata >= CASE WHEN o.obrpercentultvistoria < 85::numeric THEN (select now() - '70 days'::interval) ELSE (select now() - '100 days'::interval) END AND s.staid = 1
                AND s.supdata >
                (SELECT s.supdata
                   FROM obras2.supervisao s
                  WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata >= CASE WHEN o.obrpercentultvistoria < 85::numeric THEN (select now() - '70 days'::interval) ELSE (select now() - '100 days'::interval) END AND s.staid = 2
                  ORDER BY s.supdata DESC LIMIT 1)
                ORDER BY s.supdata ASC LIMIT 1
              ) as q2
            )


          ELSE

            -- Quando a obra não possui um supervisão de paralisação, pega o percentual da primeira vistoria menor que 70 ou 100 dias
            (  o.obrpercentultvistoria - ( SELECT sup.percentual
             FROM obras2.supervisao s
               JOIN ( SELECT DISTINCT s.supdata, s.supid, ( SELECT
                                                          CASE
                                                          WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                                          ELSE 0::numeric
                                                          END AS total
                                                            FROM obras2.cronograma cro
                                                              JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid AND i.obrid = o.obrid
                                                              LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                                            WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND cro.obrid = o.obrid AND cro.crostatus IN ('A','H') AND cro.croid = s.croid) AS percentual
                      FROM obras2.supervisao s
                      WHERE s.obrid = o.obrid AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.rsuid = 1 AND s.supdata < CASE WHEN o.obrpercentultvistoria >= 85 THEN (now() - '70 days'::interval) ELSE (now() - '100 days'::interval) END
                      ORDER BY s.supdata DESC
                      LIMIT 1) sup ON sup.supid = s.supid
             ORDER BY s.supdata DESC) ) < 10::numeric

          END

         AND NOT (o.obrid IN ( SELECT p.obrid
               FROM obras2.desbloqueioobra d
          JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
         WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
                  FROM obras2.desbloqueioobra s
                 WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
                 GROUP BY s.pdoid), 0)));

ALTER TABLE obras2.v_regras_obras_evolucao
  OWNER TO seguranca;
GRANT ALL ON TABLE obras2.v_regras_obras_evolucao TO seguranca;
GRANT SELECT ON TABLE obras2.v_regras_obras_evolucao TO sysdbbackup;
GRANT SELECT ON TABLE obras2.v_regras_obras_evolucao TO sysdbsimec_consulta;
COMMENT ON VIEW obras2.v_regras_obras_evolucao
  IS 'Regras:
- Órgão: Educação Básica
- Tipo Obra Origem: PAC2, Convênio e Emendas
- Situação: Situação: Em execução e Paralisada
- Obra com avanço inferior a 10% nas três últimas vistorias
';
COMMENT ON COLUMN obras2.v_regras_obras_evolucao.tooid IS 'Tipo Obra Origem: PAC2, Convênio e Emendas';
COMMENT ON COLUMN obras2.v_regras_obras_evolucao.situacaoobra IS 'Situação: Em execução e Paralisada';
COMMENT ON COLUMN obras2.v_regras_obras_evolucao.orgid IS 'Órgão: Educação Básica';

