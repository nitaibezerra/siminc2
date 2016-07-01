BEGIN TRANSACTION;

-- Criacao da tabela parametrossistema
DROP TABLE fabrica.parametrossistema IF EXISTS;
CREATE TABLE fabrica.parametrossistema
(
  psid serial NOT NULL,
  psnome character varying(50) NOT NULL,
  psdsc character varying(250),
  psvalor integer NOT NULL,
  CONSTRAINT pk_parametrossistema PRIMARY KEY (psid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE fabrica.parametrossistema OWNER TO simec;

--Criacao da tabela de glosa
DROP TABLE fabrica.glosa IF EXISTS;
CREATE TABLE fabrica.glosa
(
  glosaid serial NOT NULL,
  glosaqtdepf numeric(12,2) NOT NULL,
  glosajustificativa character varying(2000) NOT NULL,
  glosadatainclusao date NOT NULL,
  glosacpfusuarioresponsavel character varying(11) NOT NULL,
  CONSTRAINT pk_glosa PRIMARY KEY (glosaid)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE fabrica.glosa OWNER TO simec;

--Criacao da tabela de memorando
DROP TABLE fabrica.memorando IF EXISTS;
CREATE TABLE fabrica.memorando
(
  memoid serial NOT NULL,
  memocpfservidorresponsavel character varying(11) NOT NULL,
  memonumero integer NOT NULL,
  memodata date NOT NULL,
  memoidprestadorservico integer NOT NULL,
  memotexto character varying(2000) NOT NULL,
  memostatus character varying(4) NOT NULL,
  CONSTRAINT pk_memorando PRIMARY KEY (memoid),
  CONSTRAINT ck_fabrica_memorando CHECK (memostatus in ('IMPR', 'NIMP'))
)
WITH (
  OIDS=FALSE
);
ALTER TABLE fabrica.memorando OWNER TO simec;

--- Alteracao na tabela de OS
ALTER TABLE fabrica.ordemservico ADD memoid integer;
ALTER TABLE fabrica.ordemservico ADD glosaid integer;
ALTER TABLE fabrica.ordemservico ADD CONSTRAINT fk_ordemservico_memorando FOREIGN KEY (memoid) REFERENCES fabrica.memorando (memoid) MATCH SIMPLE ON UPDATE RESTRICT ON DELETE RESTRICT;
ALTER TABLE fabrica.ordemservico ADD CONSTRAINT fk_ordemservico_glosa FOREIGN KEY (glosaid) REFERENCES fabrica.glosa (glosaid) MATCH SIMPLE ON UPDATE RESTRICT ON DELETE RESTRICT;

--Populando tabela de fabrica.parametrossistema
INSERT INTO fabrica.parametrossistema (psnome, psdsc, psvalor) VALUES ('PERIODO_DETALHAMENTO_AVALIACAO', 'Período em dias de tramitação da situação em detalhamento para em avaliação', 3);

--Alteração na tabela de observação para guardar justificativa, adicionado campo obstp onde O = Observacao e J = Justificativa
ALTER TABLE fabrica.observacoes ADD COLUMN obstp character(1);
ALTER TABLE fabrica.observacoes ADD COLUMN obssituacao integer;
ALTER TABLE fabrica.observacoes ADD CONSTRAINT ck_observacao_obstp CHECK (obstp = 'J'::char OR obstp = 'O'::char);
UPDATE fabrica.observacoes SET obstp = 'O';

COMMIT;

