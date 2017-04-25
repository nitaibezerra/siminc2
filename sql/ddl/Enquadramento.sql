BEGIN;
--ROLLBACK;
--COMMIT;

DROP TABLE IF EXISTS monitora.pi_indicador_pnc;
DROP TABLE IF EXISTS monitora.pi_meta_pnc;
DROP TABLE IF EXISTS monitora.pi_indicador_ppa;
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
/* DDL-SIMINC
CREATE TABLE planointerno.ppaobjetivo
(
  objid serial NOT NULL,
  prsano character(4) NOT NULL,
  objcod character(4) NOT NULL,
  objnome character varying(400) NOT NULL,
  objdescricao character varying(500),
  objstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_ppaobjetivo PRIMARY KEY (objid),
  CONSTRAINT fk_ppaobjet_reference_programa FOREIGN KEY (prsano)
      REFERENCES monitora.programacaoexercicio (prsano) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_objstatus_ppaobjet CHECK (objstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)*/

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

/* DDL-SIMINC
CREATE TABLE planointerno.metappa
(
  mppid serial NOT NULL,
  prsano character(4) NOT NULL,
  mppcod character(4) NOT NULL,
  mppnome character varying(400) NOT NULL,
  mppdescricao character varying(4000),
  mppstatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  mppquantificavel boolean NOT NULL DEFAULT false,
  mppregionalizado boolean DEFAULT false,
  mppacumulativo boolean DEFAULT true,
  CONSTRAINT pk_metappa PRIMARY KEY (mppid),
  CONSTRAINT fk_metappa_reference_programa FOREIGN KEY (prsano)
  REFERENCES planointerno.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_mppsigla_metappa CHECK (mppstatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)*/

CREATE TABLE monitora.pi_iniciativa_ppa
(
  ippid SERIAL,
  oppid SERIAL NOT NULL UNIQUE ,
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
/* DDL-SIMINC
CREATE TABLE planointerno.ppainiciativa
(
  iniid serial NOT NULL,
  prsano character(4) NOT NULL,
  inicod character(4) NOT NULL,
  ininome character varying(500) NOT NULL,
  inidescricao character varying(500),
  inistatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_ppainiciativa PRIMARY KEY (iniid),
  CONSTRAINT fk_ppainici_reference_programa FOREIGN KEY (prsano)
  REFERENCES planointerno.programacaoexercicio (prsano) MATCH SIMPLE
  ON UPDATE CASCADE ON DELETE RESTRICT,
  CONSTRAINT ckc_inistatus_ppainici CHECK (inistatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
);*/


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
  ipnid SERIAL NOT NULL UNIQUE ,
  mpnid  SERIAL NOT NULL UNIQUE,
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

GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE monitora.pi_objetivo_ppa,
monitora.pi_iniciativa_ppa,
monitora.pi_indicador_pnc,
monitora.pi_metas_ppa,
monitora.pi_meta_pnc TO usr_simec;