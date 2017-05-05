BEGIN; -- ROLLBACK;

    SELECT
	oppid AS codigo,
	oppcod || '-' || oppnome AS descricao
    FROM monitora.pi_objetivo_ppa
    WHERE
	oppstatus = 'A'
	AND prsano = '2016'
    ORDER BY
	oppcod

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

UPDATE monitora.pi_objetivo_ppa SET oppnome = 'Não se aplica.', oppcod = '0' WHERE oppid = 11;

--SELECT * FROM monitora.pi_objetivo_ppa WHERE oppcod = '1085';
--SELECT DISTINCT oppid FROM monitora.pi_iniciativa_ppa;

UPDATE monitora.pi_objetivo_ppa SET oppstatus = 'I'
WHERE
	oppid NOT IN(
		SELECT DISTINCT oppid FROM monitora.pi_iniciativa_ppa UNION SELECT 11
	)
	OR oppid = '0'
;

SELECT * FROM monitora.pi_objetivo_ppa
WHERE
	oppid NOT IN(
		SELECT DISTINCT oppid FROM monitora.pi_iniciativa_ppa UNION SELECT 11
	)
;


-- COMMIT; ROLLBACK;