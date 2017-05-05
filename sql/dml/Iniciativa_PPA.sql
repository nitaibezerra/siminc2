
-- SET CLIENT_ENCODING TO 'LATIN5';
-- SET CLIENT_ENCODING TO 'UTF-8';

-- Objetivos
-- SELECT * FROM monitora.pi_objetivo_ppa WHERE oppcod = '1085';

SELECT
	*
FROM monitora.pi_iniciativa_ppa
WHERE
	ippstatus = 'A'
	AND prsano = '2016'
ORDER BY
	ippcod
;

-- COMMIT; ROLLBACK;
