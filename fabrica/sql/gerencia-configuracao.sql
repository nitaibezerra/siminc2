ALTER TABLE fabrica.auditoria ADD COLUMN docid integer;
ALTER TABLE fabrica.auditoria ALTER COLUMN usucpf DROP NOT NULL;
ALTER TABLE fabrica.auditoria
ADD CONSTRAINT docid FOREIGN KEY (docid) REFERENCES workflow.documento(docid) ON UPDATE RESTRICT ON DELETE RESTRICT;

CREATE TABLE fabrica.historicoauditoria (
	haid serial NOT NULL,
	audid bigint NOT NULL,
	usucpf character(11) NOT NULL,
	hadata timestamp without time zone NOT NULL DEFAULT now(),
	CONSTRAINT pk_haid PRIMARY KEY(haid),
	CONSTRAINT fk_audid FOREIGN KEY (audid) 
		REFERENCES fabrica.auditoria (audid) MATCH SIMPLE
		ON UPDATE RESTRICT ON DELETE RESTRICT
);


create table fabrica.historicoitemauditoria
(
 hiaid serial NOT NULL,
 haid integer NOT NULL,
 itemid integer NOT NULL,
 CONSTRAINT pk_hiaid PRIMARY KEY (hiaid),
 CONSTRAINT fk_historicoauditoria_historicoitemauditoria FOREIGN KEY (haid)
      REFERENCES fabrica.historicoauditoria (haid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT,
 CONSTRAINT fk_itemauditoria_historicoitemauditoria FOREIGN KEY (itemid)
      REFERENCES fabrica.itemauditoria (itemid) MATCH SIMPLE
      ON UPDATE RESTRICT ON DELETE RESTRICT
);
 
alter table fabrica.historicoauditoria ADD COLUMN dtamotivohistorico character varying(2000);
alter table fabrica.historicoauditoria ADD COLUMN dtaobservacao character varying(2000);
alter table fabrica.historicoauditoria ADD COLUMN dtaresultado integer;
