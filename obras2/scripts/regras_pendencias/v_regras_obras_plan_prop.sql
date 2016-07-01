-- View: obras2.v_regras_obras_plan_prop

-- DROP VIEW obras2.v_regras_obras_plan_prop;

CREATE OR REPLACE VIEW obras2.v_regras_obras_plan_prop AS
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid, 'Obra já recebeu recurso e está em planejamento pelo proponente a mais de 365 dias' AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_municipal o
      LEFT JOIN par.pagamentoobra po ON po.preid = o.preid
   LEFT JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar
  WHERE o.orgid = 3 AND (o.situacaoobra = ANY (ARRAY[763, 689])) AND
      CASE
          WHEN o.htddata IS NOT NULL THEN
          CASE
              WHEN (now() - o.htddata::timestamp with time zone) < '24:00:00'::interval THEN '1'::text
              ELSE "substring"(((now() - o.htddata::timestamp with time zone)::character varying)::text, 0, strpos(((now() - o.htddata::timestamp with time zone)::character varying)::text, 'd'::text))
          END
          ELSE
          CASE
              WHEN (now() - o.docdatainclusao::timestamp with time zone) < '24:00:00'::interval THEN '1'::text
              ELSE "substring"(((now() - o.docdatainclusao::timestamp with time zone)::character varying)::text, 0, strpos(((now() - o.docdatainclusao::timestamp with time zone)::character varying)::text, 'd'::text))
          END
      END::integer >= 365 AND p.pagvalorparcela > 0::numeric
UNION
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid, 'Obra já recebeu recurso e está em planejamento pelo proponente a mais de 365 dias' AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_estadual o
      LEFT JOIN par.pagamentoobra po ON po.preid = o.preid
   LEFT JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar
  WHERE o.orgid = 3 AND (o.situacaoobra = ANY (ARRAY[763, 689])) AND
      CASE
          WHEN o.htddata IS NOT NULL THEN
          CASE
              WHEN (now() - o.htddata::timestamp with time zone) < '24:00:00'::interval THEN '1'::text
              ELSE "substring"(((now() - o.htddata::timestamp with time zone)::character varying)::text, 0, strpos(((now() - o.htddata::timestamp with time zone)::character varying)::text, 'd'::text))
          END
          ELSE
          CASE
              WHEN (now() - o.docdatainclusao::timestamp with time zone) < '24:00:00'::interval THEN '1'::text
              ELSE "substring"(((now() - o.docdatainclusao::timestamp with time zone)::character varying)::text, 0, strpos(((now() - o.docdatainclusao::timestamp with time zone)::character varying)::text, 'd'::text))
          END
      END::integer >= 365 AND p.pagvalorparcela > 0::numeric;

ALTER TABLE obras2.v_regras_obras_plan_prop
  OWNER TO seguranca;
GRANT ALL ON TABLE obras2.v_regras_obras_plan_prop TO seguranca;
GRANT SELECT ON TABLE obras2.v_regras_obras_plan_prop TO sysdbbackup;
GRANT SELECT ON TABLE obras2.v_regras_obras_plan_prop TO sysdbsimec_consulta;
COMMENT ON VIEW obras2.v_regras_obras_plan_prop
  IS 'Regras:
- Órgão: Educação Básica
- Situação: Em planejamento pelo proponente há mais de 365 dias e já recebeu recurso
';
