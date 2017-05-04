BEGIN; -- ROLLBACK;

SELECT
	*
FROM monitora.pi_iniciativa_ppa
WHERE
	ippstatus = 'A'
	AND prsano = '2016'
ORDER BY
ippcod
;

--de -- SELECT * FROM planointerno.iniciativaestrategica WHERE prsano = '2017';
-- para -- SELECT * FROM monitora.pi_iniciativa_ppa WHERE prsano = '2016';
-- DELETE FROM monitora.pi_iniciativa_ppa;
-- TRUNCATE TABLE planointerno.iniciativaestrategica;
BEGIN; -- ROLLBACK;
INSERT INTO monitora.pi_iniciativa_ppa (
	oppid,
	ippdesc,
	ippnome,
	ippcod,
	prsano
)
SELECT
	3, -- SELECT * FROM monitora.pi_objetivo_ppa;
	inedescricao,
	inenome,
	inecod,
	'2016'
FROM planointerno.iniciativaestrategica
WHERE prsano = '2017'
;

UPDATE monitora.pi_iniciativa_ppa SET ippnome = 'Não se aplica.' WHERE ippid = 135;

-- COMMIT; ROLLBACK;
