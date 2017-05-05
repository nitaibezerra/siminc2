
/*

-- DROP TABLE planointerno.ppainiciativa;

CREATE TABLE planointerno.ppainiciativa
(
  iniid serial NOT NULL,
  prsano character(4) NOT NULL,
  inicod character(4) NOT NULL,
  ininome character varying(500) NOT NULL,
  inidescricao character varying(500),
  inistatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_ppainiciativa PRIMARY KEY (iniid),
  CONSTRAINT ckc_inistatus_ppainici CHECK (inistatus = ANY (ARRAY['A'::bpchar, 'I'::bpchar]))
)
WITH (
  OIDS=FALSE
);

*/

SELECT
	*
FROM monitora.pi_iniciativa_ppa
WHERE
	ippstatus = 'A'
	AND prsano = '2016'
ORDER BY
ippcod
;

--de -- SELECT * FROM planointerno.ppainiciativa WHERE prsano = '2017';
-- para -- SELECT * FROM monitora.pi_iniciativa_ppa WHERE prsano = '2016';
-- DELETE FROM monitora.pi_iniciativa_ppa;
-- TRUNCATE TABLE planointerno.ppainiciativa;
BEGIN; -- ROLLBACK;
INSERT INTO monitora.pi_iniciativa_ppa (
	oppid,
	ippdesc,
	ippnome,
	ippcod,
	prsano
)
SELECT DISTINCT
	3, -- SELECT * FROM monitora.pi_objetivo_ppa;
	p_i.inidescricao,
	p_i.ininome,
	p_i.inicod,
	'2016'
FROM planointerno.ppainiciativa p_i -- 159 -- SELECT * FROM planointerno.ppainiciativa
--	JOIN planointerno.ppaobjetivo p_o ON p_i. -- SELECT * FROM planointerno.ppaobjetivo 
WHERE p_i.prsano = '2017'
;

UPDATE monitora.pi_iniciativa_ppa SET ippnome = 'Não se aplica.' WHERE ippid = 135;

-- COMMIT; ROLLBACK;
