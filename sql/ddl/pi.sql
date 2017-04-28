BEGIN; --ROLLBACK; COMMIT;

ALTER TABLE monitora.pi_niveletapaensino
   ADD COLUMN mdeid integer NOT NULL DEFAULT NULL;
COMMENT ON COLUMN monitora.pi_niveletapaensino.mdeid
  IS 'Relação com a tabela de Área Cultural. monitora.pi_modalidadeensino';

ALTER TABLE monitora.pi_niveletapaensino
   ADD CONSTRAINT fk_pi_niveletapaensino_reference_pi_modalidadeensino FOREIGN KEY(mdeid)
  REFERENCES monitora.pi_modalidadeensino(mdeid) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT;

-- DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
CREATE TABLE monitora.pi_objetivo_ppa
(
  oppid SERIAL PRIMARY KEY,
  prsano CHAR(4),
  oppdesc VARCHAR(1000),
  oppnome VARCHAR(200),
  oppcod VARCHAR(4),
  oppstatus char default 'A'::character varying,
  CONSTRAINT fk_objppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_oppstatus_objppa CHECK (oppstatus= ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_objetivo_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_metas_ppa;
CREATE TABLE monitora.pi_metas_ppa
(
  mppid SERIAL PRIMARY KEY,
  mppdesc VARCHAR(1000),
  mppcod CHARACTER(4),
  mppnome CHARACTER VARYING(400),
  mppstatus char default 'A'::character varying,
  prsano CHAR(4),
  CONSTRAINT fk_metppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_mppstatus_metppa CHECK (mppstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_metas_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
CREATE TABLE monitora.pi_objetivoppa_metappa
(
  opmid serial NOT NULL,
  oppid integer NOT NULL,
  mppid integer NOT NULL,
  mpodata timestamp without time zone NOT NULL DEFAULT now(),
  CONSTRAINT pk_pi_objetivoppa_metappa PRIMARY KEY (opmid),
  CONSTRAINT fk_objetivoppa_metappa_reference_pi_objetivo_ppa FOREIGN KEY (oppid)
      REFERENCES monitora.pi_objetivo_ppa (oppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_objetivoppa_metappa_reference_metas_ppa FOREIGN KEY (mppid)
      REFERENCES monitora.pi_metas_ppa (mppid) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_objetivoppa_metappa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_iniciativa_ppa; -- Renomeada pra pi_indicador_ppa
CREATE TABLE monitora.pi_iniciativa_ppa
(
  ippid serial NOT NULL,
  oppid integer NOT NULL,
  ippdesc VARCHAR(1000),
  ippnome CHARACTER VARYING(500),
  ippcod VARCHAR(4),
  ippstatus char default 'A'::character varying,
  prsano CHAR(4),
  PRIMARY KEY (ippid,oppid),
  CONSTRAINT fk_inippa_reference_obj FOREIGN KEY (oppid)
  REFERENCES monitora.pi_objetivo_ppa (oppid) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_indppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ippstatus_indppa CHECK (ippstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_iniciativa_ppa TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_meta_pnc;
CREATE TABLE monitora.pi_meta_pnc
(
  mpnid SERIAL PRIMARY KEY,
  mpndesc VARCHAR(1000),
  mpnstatus char default 'A'::character varying,
  mpncod character(4) NOT NULL,
  mpnnome character varying(400) NOT NULL,
  prsano CHAR(4),
  CONSTRAINT fk_metppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_mpnstatus_metpnc CHECK (mpnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_meta_pnc TO usr_simec;

-- DROP TABLE IF EXISTS monitora.pi_indicador_pnc;
CREATE TABLE monitora.pi_indicador_pnc
(
  ipnid SERIAL NOT NULL,
  mpnid integer NOT NULL,
  ipndesc VARCHAR(1000),
  ipnstatus char default 'A'::character varying,
  prsano CHAR(4),
  PRIMARY KEY (ipnid,mpnid),
  CONSTRAINT fk_indpnc_reference_meta FOREIGN KEY (mpnid)
  REFERENCES monitora.pi_meta_pnc (mpnid) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT fk_indppa_reference_programa FOREIGN KEY (prsano)
  REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_ipnstatus_indpnc CHECK (ipnstatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_indicador_pnc TO usr_simec;

CREATE TABLE monitora.pi_produto
(
  pprid serial NOT NULL,
  prsano character(4) NOT NULL,
  pprnome character varying(200) NOT NULL,
  pprdescricao character varying(500),
  pprstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_pi_produto PRIMARY KEY(pprid),
  CONSTRAINT ckc_pprstatus_pi_produto CHECK(pprstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_produto TO usr_simec;

-- DROP TABLE monitora.pi_unidade_medida;
CREATE TABLE monitora.pi_unidade_medida
(
  pumid serial NOT NULL,
  prsano character(4) NOT NULL,
  pumnome character varying(100) NOT NULL,
  pumdescricao character varying(200),
  pumstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_pi_unidade_medida PRIMARY KEY(pumid),
  CONSTRAINT ckc_pi_unidade_medida CHECK(pumstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_unidade_medida TO usr_simec;