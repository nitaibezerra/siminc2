-- View: obras2.v_regras_obras_saldo_sem_execucao

-- DROP VIEW obras2.v_regras_obras_saldo_sem_execucao;

CREATE OR REPLACE VIEW obras2.v_regras_obras_saldo_sem_execucao AS
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid,
                CASE
                    WHEN o.tpoid = ANY (ARRAY[104, 105]) THEN 'Obra não entrou em execução (percentual 0%) após 730 dias do primeiro pagamento.'::text
                    ELSE 'Obra não entrou em execução (percentual 0%) após 365 dias do primeiro pagamento.'::text
                END AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_municipal o
           JOIN obras2.obras oi ON oi.obrid = o.obrid
      LEFT JOIN par.pagamentoobra po ON po.preid = o.preid
   LEFT JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) <> 'CANCELADO'::text AND p.pagparcela = 1::numeric
  WHERE ((o.obrpercentultvistoria IS NULL OR o.obrpercentultvistoria <= 0::numeric) AND (oi.obrperccontratoanterior IS NULL OR oi.obrperccontratoanterior <= 0::numeric) ) AND (o.situacaoobra <> ALL (ARRAY[693, 768, 769, 874, 1084])) AND o.orgid = 3 AND date_part('days'::text, now() - p.pagdatapagamento::timestamp with time zone)::integer >=
      CASE
          WHEN o.tpoid = ANY (ARRAY[104, 105]) THEN 730
          ELSE 365
      END AND p.pagvalorparcela > 0::numeric AND NOT (o.obrid IN ( SELECT p.obrid
         FROM obras2.desbloqueioobra d
    JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
   WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
            FROM obras2.desbloqueioobra s
           WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
           GROUP BY s.pdoid), 0))) AND o.tpoid NOT IN(104, 105)
UNION
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid,
                CASE
                    WHEN o.tpoid = ANY (ARRAY[104, 105]) THEN 'Obra não entrou em execução (percentual 0%) após 730 dias do primeiro pagamento.'::text
                    ELSE 'Obra não entrou em execução (percentual 0%) após 365 dias do primeiro pagamento.'::text
                END AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_estadual o
           JOIN obras2.obras oi ON oi.obrid = o.obrid
           JOIN entidade.entidade e ON e.entid = o.entid
      LEFT JOIN par.pagamentoobra po ON po.preid = o.preid
   LEFT JOIN par.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) <> 'CANCELADO'::text AND p.pagparcela = 1::numeric
  WHERE e.entnumcpfcnpj IN ('', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '') AND ((o.obrpercentultvistoria IS NULL OR o.obrpercentultvistoria <= 0::numeric) AND (oi.obrperccontratoanterior IS NULL OR oi.obrperccontratoanterior <= 0::numeric) ) AND (o.situacaoobra <> ALL (ARRAY[693, 768, 769, 874, 1084])) AND o.orgid = 3 AND date_part('days'::text, now() - p.pagdatapagamento::timestamp with time zone)::integer >=
      CASE
          WHEN o.tpoid = ANY (ARRAY[104, 105]) THEN 730
          ELSE 365
      END AND p.pagvalorparcela > 0::numeric AND NOT (o.obrid IN ( SELECT p.obrid
         FROM obras2.desbloqueioobra d
    JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
   WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
            FROM obras2.desbloqueioobra s
           WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
           GROUP BY s.pdoid), 0))) AND o.tpoid NOT IN(104, 105);

ALTER TABLE obras2.v_regras_obras_saldo_sem_execucao
  OWNER TO seguranca;
GRANT ALL ON TABLE obras2.v_regras_obras_saldo_sem_execucao TO seguranca;
GRANT SELECT ON TABLE obras2.v_regras_obras_saldo_sem_execucao TO sysdbbackup;
GRANT SELECT ON TABLE obras2.v_regras_obras_saldo_sem_execucao TO sysdbsimec_consulta;
COMMENT ON VIEW obras2.v_regras_obras_saldo_sem_execucao
  IS 'Regras:
- Órgão: Educação Básica
- Situação: Sem execução e recebeu a primeira parcela a mais de 365 dias
';
