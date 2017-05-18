UPDATE seguranca.sistema SET sisdsc = REPLACE(sisdsc, 'SPO', 'SPOA'), sisabrev = REPLACE(sisabrev, 'SPO', 'SPOA')
WHERE sisstatus = 'A' and sisdsc ILIKE 'SPO%';