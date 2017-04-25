BEGIN; -- ROLLBACK;

SELECT
	*
FROM monitora.pi_objetivo_ppa
WHERE
	oppstatus = 'A'
	AND prsano = '2016'
;

--de -- SELECT * FROM planointerno.ppaobjetivo WHERE prsano = '2016';
-- para -- SELECT * FROM monitora.pi_objetivo_ppa WHERE prsano = '2016';
-- DELETE FROM monitora.pi_objetivo_ppa;
-- TRUNCATE TABLE planointerno.ppaobjetivo;
BEGIN; -- ROLLBACK;
INSERT INTO monitora.pi_objetivo_ppa (
	prsano,
	oppdesc,
	oppnome,
	oppcod
)
SELECT
	'2016',
	objdescricao,
	objnome,
	objcod
FROM planointerno.ppaobjetivo
WHERE prsano = '2017'
;

-- COMMIT; ROLLBACK;