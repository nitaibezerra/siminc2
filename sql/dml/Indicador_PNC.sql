BEGIN; -- ROLLBACK;

SELECT
    ipnid AS codigo,
    ipncod || '-' || ipndesc AS descricao
FROM monitora.pi_indicador_pnc
WHERE
    ipnstatus = 'A'
ORDER BY ipncod;

SELECT
	ipndesc,
	TRIM(SUBSTR(ipndesc, 0, STRPOS(ipndesc, ':'))) AS codigo,
	TRIM(SUBSTR(ipndesc, STRPOS(ipndesc, ':')+1, LENGTH(ipndesc))) AS descricao
FROM monitora.pi_indicador_pnc WHERE ipnstatus = 'A';

-- ALTER TABLE monitora.pi_indicador_pnc DROP COLUMN ipncod;
-- ALTER TABLE monitora.pi_indicador_pnc ADD COLUMN ipncod character(3) DEFAULT NULL;
UPDATE monitora.pi_indicador_pnc SET ipndesc = REPLACE(ipndesc, 'Indicador ', '');
UPDATE monitora.pi_indicador_pnc SET ipncod = TRIM(SUBSTR(ipndesc, 0, STRPOS(ipndesc, ':'))), ipndesc = TRIM(SUBSTR(ipndesc, STRPOS(ipndesc, ':')+1, LENGTH(ipndesc))) ;

-- SELECT * FROM monitora.pi_indicador_pnc WHERE ipnstatus = 'A';
-- TRUNCATE TABLE monitora.pi_indicador_pnc;

UPDATE monitora.pi_indicador_pnc SET ipncod = '0' WHERE ipnid = 434;

-- COMMIT; ROLLBACK;