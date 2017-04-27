BEGIN; -- ROLLBACK;

-- DROP TABLE planointerno.tipoinstrumento;
/*
CREATE TABLE planointerno.tipoinstrumento
(
  tpiid serial NOT NULL,
  tpidsc character varying(250) NOT NULL,
  tpistatus character(1) DEFAULT 'A'::bpchar,
  prsano character(4),
  CONSTRAINT pk_planointerno_tipoinstrumento PRIMARY KEY (tpiid)
)
WITH (
  OIDS=FALSE
);
*/

SELECT
	*
FROM monitora.pi_categoriaapropriacao
WHERE
	capano = '2016'
;

--de -- SELECT * FROM planointerno.tipoinstrumento WHERE prsano = '2017'
-- para -- SELECT * FROM monitora.pi_categoriaapropriacao
-- DELETE FROM obras.unidadeobrasubacao; -- SELECT * FROM obras.unidadeobrasubacao;
-- DELETE FROM monitora.pi_categoriaapropriacao;
-- TRUNCATE TABLE planointerno.tipoinstrumento;
BEGIN; -- ROLLBACK;
INSERT INTO monitora.pi_categoriaapropriacao (
	capcod,
	capdsc,
	capano
)
SELECT
	tpiid,
	tpidsc,
	'2016'
FROM planointerno.tipoinstrumento
WHERE
	prsano = '2017'
;

DELETE FROM monitora.pi_categoriaapropriacao WHERE capid = 278;

-- COMMIT; ROLLBACK;