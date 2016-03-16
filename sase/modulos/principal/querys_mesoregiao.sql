SELECT 
	s.stacor as cor, 
	CASE WHEN s.stacod IS NULL THEN 'NÃ£o Assessorado' ELSE s.stadsc END as situacao,
	a.muncod, a.estuf
FROM sase.assessoramento a
JOIN sase.situacaoassessoramento s ON s.stacod = a.stacod
WHERE 1=1
AND a.estuf IN ('AM') 

select * from territorios.municipio where 