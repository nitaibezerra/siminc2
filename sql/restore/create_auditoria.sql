﻿
CREATE SCHEMA auditoria
  AUTHORIZATION postgres;

GRANT ALL ON SCHEMA auditoria TO postgres;
GRANT USAGE ON SCHEMA auditoria TO usr_simec;

CREATE TABLE auditoria.auditoria
(
  audid serial NOT NULL,
  usucpf character(11),
  mnuid integer,
  audsql text,
  audtabela character varying(100),
  audtipo character(1),
  audip character varying(20),
  auddata timestamp without time zone DEFAULT now(),
  audmsg text,
  sisid integer,
  audscript character varying(5000),
  CONSTRAINT pk_auditoria_11_2014 PRIMARY KEY (audid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE auditoria.auditoria
  OWNER TO postgres;
GRANT ALL ON TABLE auditoria.auditoria TO postgres;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE auditoria.auditoria TO usr_simec;

-- Index: auditoria.idx_auddata

-- DROP INDEX auditoria.idx_auddata;

CREATE INDEX idx_auddata
  ON auditoria.auditoria
  USING btree
  (auddata);

-- Index: auditoria.idx_audip_audata

-- DROP INDEX auditoria.idx_audip_audata;

CREATE INDEX idx_audip_audata
  ON auditoria.auditoria
  USING btree
  (audip COLLATE pg_catalog."default", auddata);

-- Index: auditoria.idx_audtabela_auddata

-- DROP INDEX auditoria.idx_audtabela_auddata;

CREATE INDEX idx_audtabela_auddata
  ON auditoria.auditoria
  USING btree
  (audtabela COLLATE pg_catalog."default", auddata);

-- Index: auditoria.idx_audtipo_auddata

-- DROP INDEX auditoria.idx_audtipo_auddata;

CREATE INDEX idx_audtipo_auddata
  ON auditoria.auditoria
  USING btree
  (audtipo COLLATE pg_catalog."default", auddata);

-- Index: auditoria.idx_sisid_auddata

-- DROP INDEX auditoria.idx_sisid_auddata;

CREATE INDEX idx_sisid_auddata
  ON auditoria.auditoria
  USING btree
  (sisid, auddata);

-- Index: auditoria.idx_usucpf_auddata

-- DROP INDEX auditoria.idx_usucpf_auddata;

CREATE INDEX idx_usucpf_auddata
  ON auditoria.auditoria
  USING btree
  (usucpf COLLATE pg_catalog."default", auddata);

-- Atualizando senhas e e-mail por segurança
UPDATE
    seguranca.usuario
SET 
    ususenha = 'o/0m5tlONgaBe9NwzktC4uUvv+26NqEE6YAJmOz4Qn4=', -- 123456
    usuemail = 'teste@teste.com.br'
;