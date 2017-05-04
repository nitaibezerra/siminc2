BEGIN; -- ROLLBACK;

SELECT
	*
FROM monitora.pi_metas_ppa
WHERE
	mppstatus = 'A'
	AND prsano = '2016'
;

--de -- SELECT * FROM planointerno.metappa WHERE prsano = '2017';
-- para -- SELECT * FROM monitora.pi_metas_ppa WHERE prsano = '2016';
-- DELETE FROM monitora.pi_objetivo_ppa;
-- TRUNCATE TABLE planointerno.ppaobjetivo;
BEGIN; -- ROLLBACK;
INSERT INTO monitora.pi_metas_ppa (
	mppdesc,
	mppcod,
	mppnome,
	prsano
)
SELECT
	mppdescricao,
	mppcod,
	mppnome,
	'2016'
FROM planointerno.metappa
WHERE prsano = '2017'
;

UPDATE monitora.pi_metas_ppa SET mppdesc = 'Não se aplica.', mppnome = 'Não se aplica.', mppcod = '0' WHERE mppid IN(13, 14);

-- COMMIT; ROLLBACK;