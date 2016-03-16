-- View: obras2.v_regras_obras_paralisadas

-- DROP VIEW obras2.v_regras_obras_paralisadas;

CREATE OR REPLACE VIEW obras2.v_regras_obras_paralisadas AS 
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid, 'Obra Paralisada' AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_municipal o
          WHERE o.orgid = 3 AND o.situacaoobra = 691 AND NOT (o.obrid IN ( SELECT p.obrid
                   FROM obras2.desbloqueioobra d
              JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
             WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
                      FROM obras2.desbloqueioobra s
                     WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
                     GROUP BY s.pdoid), 0)))
UNION 
         SELECT o.obrid, o.obrnome, o.empesfera, o.tobid, o.tpoid, o.cloid, o.tooid, o.mundescricao, o.muncod, o.estuf, o.entid, o.docid, o.situacaoobra, o.esddsc, o.dataultimaalteracao, o.diasultimaalteracao, o.usuarioultimaalteracao, o.inuid, o.obrdtinclusao, o.diasinclusao, o.empdtprimeiropagto, o.diasprimeiropagamento, o.qtdpedidosdesbloqueio, o.qtddeferidos, o.qtdindeferidos, o.qtdnaoanalisados, o.desterminodeferido, o.versaosistema, o.empid, o.htddata, o.docdatainclusao, o.orgid, o.prfid, o.preid, 'Obra Paralisada' AS pendencia, o.obrpercentultvistoria
           FROM obras2.v_obras_situacao_estadual o
           JOIN entidade.entidade e ON e.entid = o.entid
          WHERE o.orgid = 3 AND e.entnumcpfcnpj IN ('', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '')
           AND o.situacaoobra = 691 AND NOT (o.obrid IN ( SELECT p.obrid
                   FROM obras2.desbloqueioobra d
              JOIN obras2.pedidodesbloqueioobra p ON p.pdoid = d.pdoid AND p.pdostatus = 'A'::bpchar
             WHERE d.destipodesbloqueio = 'D'::bpchar AND now() >= d.desdatainicio AND now() <= d.destermino AND d.desid > COALESCE(( SELECT max(s.desid) AS max
                      FROM obras2.desbloqueioobra s
                     WHERE d.pdoid = s.pdoid AND s.destipodesbloqueio = 'I'::bpchar AND now() >= s.desdatainicio AND now() <= s.destermino
                     GROUP BY s.pdoid), 0)));

ALTER TABLE obras2.v_regras_obras_paralisadas
  OWNER TO seguranca;
GRANT ALL ON TABLE obras2.v_regras_obras_paralisadas TO seguranca;
GRANT SELECT ON TABLE obras2.v_regras_obras_paralisadas TO sysdbbackup;
GRANT SELECT ON TABLE obras2.v_regras_obras_paralisadas TO sysdbsimec_consulta;
COMMENT ON VIEW obras2.v_regras_obras_paralisadas
  IS 'Regras:
- Órgão: Educação Básica
- Situação: Situação: Obra Paralisada

';

COMMENT ON COLUMN obras2.v_regras_obras_paralisadas.situacaoobra IS 'Situação: Obra Paralisada';
COMMENT ON COLUMN obras2.v_regras_obras_paralisadas.orgid IS 'Órgão: Educação Básica';