
-- Tabela de importação
/*
CREATE TABLE planointerno.unidadegestora_limite
(
  ungcod character(10),
  lmuvlr numeric(15,2),
  lmudtcadastro timestamp without time zone DEFAULT now(),
  usucpf character(11),
  lmuflgliberado boolean DEFAULT false
)
WITH (
  OIDS=FALSE
);
*/
-- cod_ung, total, lmudtcadastro, usucpf, lmustatus, lmuflgliberado, prsano

/*
-- Consulta pra extrair os dados de importação
SELECT
	suo.suocod,
	COALESCE(lmu.lmuvlrcusteio,0) + COALESCE(lmu.lmuvlrcapital,0) as total,
	lmu.lmudtcadastro,
	lmu.usucpf,
	lmu.lmuflgliberado
    FROM planointerno.limitemomentounidade lmu -- SELECT * FROM planointerno.limitemomentounidade;
	    JOIN planointerno.momento mom ON mom.momid = lmu.momid
	    JOIN planointerno.subunidadeorcamentaria suo ON suo.suoid = lmu.suoid -- SELECT * FROM planointerno.subunidadeorcamentaria;
	    JOIN planointerno.unidadeorcamentaria uno ON uno.unoid = suo.unoid -- SELECT * FROM planointerno.unidadeorcamentaria;
    WHERE
	mom.momid = 21 
	AND lmu.lmustatus = 'A'
	AND mom.prsano = '2016'
;
*/
-- de : SELECT * FROM planointerno.unidadegestora_limite;
-- para : SELECT * FROM planacomorc.unidadegestora_limite;
-- DELETE FROM planacomorc.unidadegestora_limite;

-- ALTER TABLE planacomorc.unidadegestora_limite DROP COLUMN ungid;

ALTER TABLE planacomorc.unidadegestora_limite ADD COLUMN ungcod character(10);
ALTER TABLE planacomorc.unidadegestora_limite ALTER COLUMN ungcod SET NOT NULL;

BEGIN; -- ROLLBACK; COMMIT;

INSERT INTO planacomorc.unidadegestora_limite(
	ungcod,
	lmuvlr,
	lmudtcadastro,
	usucpf,
	lmuflgliberado,
	prsano
)
SELECT DISTINCT
	ul.ungcod,
	ul.lmuvlr,
	ul.lmudtcadastro,
	ul.usucpf,
	ul.lmuflgliberado,
	'2016'
FROM planointerno.unidadegestora_limite ul;


SELECT
	ungcod,
	lmuvlr
--	,*
FROM planacomorc.unidadegestora_limite ul -- SELECT * FROM planacomorc.unidadegestora_limite
WHERE
	ul.lmustatus = 'A'
	AND ul.prsano = '2016'
	AND ul.lmuflgliberado IS TRUE
	AND ul.ungcod = '420041'
;

