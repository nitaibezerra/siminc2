-- View: obras2.v_regras_obras_execucao_sem_vistoria

-- DROP VIEW obras2.v_regras_obras_execucao_sem_vistoria;

CREATE OR REPLACE VIEW obras2.v_regras_obras_execucao_sem_vistoria AS
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid,
                'Obra em execução sem vistoria há mais de 60 dias.'::text AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_municipal o
           JOIN obras2.obras oi ON oi.obrid = o.obrid
  WHERE o.situacaoobra = 690 AND o.orgid = 3 AND oi.obrdtultvistoria IS NULL AND (SELECT DATE_PART('days',NOW() - h.htddata) FROM workflow.historicodocumento h
      JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid AND a.esdiddestino = 690
      WHERE h.docid = o.docid
      ORDER BY h.htddata DESC LIMIT 1) >= 60 AND NOT (o.obrid IN ( SELECT p.obrid
         FROM obras2.desbloqueioobra d
    JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
   WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
            FROM obras2.desbloqueioobra s
           WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
           GROUP BY s.pdoid), 0)))
UNION
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid,
                'Obra em execução sem vistoria há mais de 60 dias.'::text AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_estadual o
           JOIN obras2.obras oi ON oi.obrid = o.obrid
           JOIN entidade.entidade e ON e.entid = o.entid
  WHERE o.situacaoobra = 690 AND o.orgid = 3 AND oi.obrdtultvistoria IS NULL
  AND e.entnumcpfcnpj IN ('', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '')
  AND (SELECT DATE_PART('days',NOW() - h.htddata) FROM workflow.historicodocumento h
      JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid AND a.esdiddestino = 690
      WHERE h.docid = o.docid
      ORDER BY h.htddata DESC LIMIT 1) >= 60 AND NOT (o.obrid IN ( SELECT p.obrid
         FROM obras2.desbloqueioobra d
    JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
   WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
            FROM obras2.desbloqueioobra s
           WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
           GROUP BY s.pdoid), 0)));

ALTER TABLE obras2.v_regras_obras_execucao_sem_vistoria
  OWNER TO seguranca;
GRANT ALL ON TABLE obras2.v_regras_obras_execucao_sem_vistoria TO seguranca;
GRANT SELECT ON TABLE obras2.v_regras_obras_execucao_sem_vistoria TO sysdbbackup;
GRANT SELECT ON TABLE obras2.v_regras_obras_execucao_sem_vistoria TO sysdbsimec_consulta;
COMMENT ON VIEW obras2.v_regras_obras_execucao_sem_vistoria
  IS 'Regras:
- Órgão: Educação Básica
- Situação: Obra a mais de 60 dias que esta em execução e sem vistoria
';
