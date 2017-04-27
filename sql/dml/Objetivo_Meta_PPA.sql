BEGIN; -- ROLLBACK;

SELECT
	*
FROM monitora.pi_objetivoppa_metappa 
;

--de -- SELECT * FROM planointerno.metappaobjetivo
-- para -- SELECT * FROM monitora.pi_objetivoppa_metappa
-- DELETE FROM monitora.pi_objetivoppa_metappa;
-- TRUNCATE TABLE planointerno.metappaobjetivo;
BEGIN; -- ROLLBACK;
INSERT INTO monitora.pi_objetivoppa_metappa (
	oppid,
	mppid
)
SELECT -- DISTINCT
--	*
--	p_mo.objid,
	m_o.oppid,

--	p_m.mppid,
	m_m.mppid
	
/*
	oppid
*/
FROM planointerno.metappaobjetivo p_mo
	JOIN planointerno.ppaobjetivo p_o ON p_mo.objid = p_o.objid -- SELECT * FROM planointerno.ppaobjetivo
	JOIN monitora.pi_objetivo_ppa m_o ON p_o.objcod = m_o.oppcod -- SELECT * FROM monitora.pi_objetivo_ppa 
	
	JOIN planointerno.metappa p_m ON p_mo.mppid = p_m.mppid -- SELECT * FROM planointerno.metappa 
	JOIN monitora.pi_metas_ppa m_m ON p_m.mppcod = m_m.mppcod -- SELECT * FROM monitora.pi_metas_ppa 
WHERE
	p_o.prsano = '2017' -- 43 registros de 2017
	AND m_o.prsano = '2016' -- Rodar pra 2017 quando for carga 2017 - deixar os dois filtros com o mesmo ano
	
	AND p_m.prsano = '2017' -- 43 registros de 2017
	AND m_m.prsano = '2016' -- Rodar pra 2017 quando for carga 2017 - deixar os dois filtros com o mesmo ano
;

-- COMMIT; ROLLBACK;