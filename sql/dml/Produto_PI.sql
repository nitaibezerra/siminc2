/*
-- DROP TABLE planointerno.produtopi;
CREATE TABLE planointerno.produtopi
(
  priid serial NOT NULL,
  prsano character(4) NOT NULL,
  prinome character varying(200) NOT NULL,
  pridescricao character varying(500),
  pristatus character(1) NOT NULL DEFAULT 'A'::bpchar,
  CONSTRAINT pk_produtopi PRIMARY KEY (priid),
  CONSTRAINT ckc_pristatus_produtop CHECK (pristatus = ANY (ARRAY['I'::bpchar, 'A'::bpchar]))
)
WITH (
  OIDS=FALSE
);

*/


SELECT
	pprid AS codigo,
	pprnome AS descricao
FROM monitora.pi_produto
WHERE
	prsano = '2016'
	AND pprstatus = 'A'
ORDER BY
	descricao
;

SELECT * FROM monitora.pi_produto WHERE prsano = '2016';

--de -- SELECT * FROM planointerno.produtopi WHERE prsano = '2017';
-- para -- SELECT * FROM monitora.pi_produto WHERE prsano = '2016';
-- DELETE FROM monitora.pi_produto;
-- TRUNCATE TABLE planointerno.produtopi;
BEGIN; -- ROLLBACK;

INSERT INTO monitora.pi_produto (
	pprnome,
	pprdescricao,
	prsano
)
SELECT
	prinome,
	pridescricao,
	2016
FROM planointerno.produtopi
WHERE
	prsano = '2017'
;

UPDATE territorios.esfera SET esfdsc ='Federal/Brasil'  WHERE esfdsc = 'Federal';

-- COMMIT; ROLLBACK;
