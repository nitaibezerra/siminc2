BEGIN;
--ROLLBACK;
--COMMIT;

DROP TABLE IF EXISTS monitora.pi_indicador_pnc;
DROP TABLE IF EXISTS monitora.pi_meta_pnc;
DROP TABLE IF EXISTS monitora.pi_indicador_ppa;
DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
DROP TABLE IF EXISTS monitora.pi_metas_ppa;
DROP TABLE IF EXISTS monitora.pi_iniciativa_ppa; -- Renomeada pra pi_indicador_ppa
DROP TABLE IF EXISTS monitora.pi_objetivo_ppa;

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

DROP TABLE IF EXISTS monitora.pi_objetivoppa_metappa;
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

DROP TABLE IF EXISTS monitora.pi_iniciativa_ppa; -- Renomeada pra pi_indicador_ppa
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

-- ALTER TABLE monitora.pi_iniciativa_ppa,monitora.pi_objetivo_ppa,pi_metas_ppa,pi_iniciativa_ppa,pi_meta_pnc,pi_indicador_pnc OWNER TO postgres;

GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE 
monitora.pi_iniciativa_ppa,
monitora.pi_indicador_pnc,
monitora.pi_objetivo_ppa,
monitora.pi_metas_ppa,
monitora.pi_objetivoppa_metappa,
monitora.pi_meta_pnc TO usr_simec;

