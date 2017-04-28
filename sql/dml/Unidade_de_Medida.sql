/*
-- DROP TABLE planointerno.unidademedidapi;
CREATE TABLE planointerno.unidademedidapi
(
  umpid serial NOT NULL,
  prsano character(4) NOT NULL,
  umpnome character varying(100) NOT NULL,
  umpdescricao character varying(200),
  umpstatus character(1) NOT NULL,
  CONSTRAINT pk_unidademedidapi PRIMARY KEY (umpid)
)
WITH (
  OIDS=FALSE
);
*/

SELECT
	pumid AS codigo,
	pumdescricao AS descricao
FROM monitora.pi_unidade_medida 
WHERE
	prsano = '2016'
	AND pumstatus = 'A'
ORDER BY
	descricao
;

SELECT * FROM monitora.pi_unidade_medida WHERE prsano = '2016';

--de -- SELECT * FROM planointerno.unidademedidapi WHERE prsano = '2017';
-- para -- SELECT * FROM monitora.pi_unidade_medida WHERE prsano = '2016';
-- DELETE FROM monitora.pi_unidade_medida;
-- TRUNCATE TABLE planointerno.unidademedidapi;
BEGIN; -- ROLLBACK;

INSERT INTO monitora.pi_unidade_medida (
	pumnome,
	pumdescricao,
	prsano
)
SELECT
	umpnome,
	umpdescricao,
	2016
FROM planointerno.unidademedidapi
WHERE
	prsano = '2017'
;

-- COMMIT; ROLLBACK;
