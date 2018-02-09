<?php
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
$_REQUEST['baselogin'] = 'simec_espelho_producao';

// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';

$db = new cls_banco();

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '72324414104';
$_SESSION['usucpf'] = '72324414104';
$_SESSION['sisid'] = 194;

global $db;

$sql =  "
	--APAGA TODOS OS DADOS DA VIEW
	DELETE FROM carga.vw_avaliacao_simec;

	--ATUALIZAR A VIEW COM OS DADOS DO DBLINK
	    INSERT INTO carga.vw_avaliacao_simec
	      ( \"DISCENTE\",
	      \"CPF_DISCENTE\" ,
	      \"BOLSISTA\",
	      \"INICIO_ATIVIDADE\",
	      \"FIM_ATIVIDADE\",
	      \"CODIGO_IES\",
	      \"UF_IES\", \"NOME_IES\",
	      \"CODIGO_GRUPOPET\",
	      \"NOME_GRUPOPET\",
	      \"ABRANGENCIA\",
	      \"TUTOR\",
	      \"CPF_TUTOR\",
	      \"INICIO TUTORIA\",
	      \"CURSO\")
	    SELECT *
		FROM
		    dblink ( 'dbname= hostaddr= user= password= port=', 'SELECT * FROM consulta.vw_avaliacao_simec;' ) AS rs
		  (
		    \"DISCENTE\" varchar(100),
		    \"CPF_DISCENTE\" varchar(11),
		    \"BOLSISTA\" char(1),
		    \"INICIO_ATIVIDADE\" varchar(10),
		    \"FIM_ATIVIDADE\" varchar(10),
		    \"CODIGO_IES\" integer,
		    \"UF_IES\" varchar(2),
		    \"NOME_IES\" varchar(500),
		    \"CODIGO_GRUPOPET\" integer,
		    \"NOME_GRUPOPET\" varchar(500),
		    \"ABRANGENCIA\" varchar(500),
		    \"TUTOR\" varchar(500),
		    \"CPF_TUTOR\" varchar(100),
		    \"INICIO TUTORIA\" varchar(11),
		    \"CURSO\" varchar(500)
		);


	--APAGA OS REGISTROS DO PET
	DELETE FROM pet.tutor;
	DELETE FROM pet.vigencia;
	DELETE FROM pet.grupopetcurso;
	DELETE FROM pet.curso;
	DELETE FROM pet.discente;
	DELETE FROM pet.grupopet;
	DELETE FROM pet.institutoensinosuperior;

	--*** ATUALIZAR AS TABELAS DO PET UTILIZANDO A vw_avaliacao_simec

	--- GRAVA AS UNIVERSIDADES
	INSERT INTO pet.institutoensinosuperior (iesid,   nome,uf)
		SELECT DISTINCT \"CODIGO_IES\", \"NOME_IES\",\"UF_IES\"  FROM carga.vw_avaliacao_simec ORDER BY \"CODIGO_IES\";


	--- GRAVA OS CURSOS
	INSERT INTO pet.curso ( nome)
		SELECT DISTINCT \"CURSO\"  FROM carga.vw_avaliacao_simec ORDER BY \"CURSO\";


	--- GRAVA OS GRUPOS
	    INSERT INTO pet.grupopet (grpid , iesid, abrangencia, nomegrupo)
		SELECT DISTINCT \"CODIGO_GRUPOPET\",
		     (SELECT iesid FROM pet.institutoensinosuperior WHERE nome = \"NOME_IES\") as iesid,
			       CASE
						      WHEN \"ABRANGENCIA\" = 'CURSO ESPECIFICO' THEN 'C'
						      WHEN \"ABRANGENCIA\" = 'INTERDISCIPLINAR' THEN 'I'
			       END,
		      \"NOME_GRUPOPET\"
		      FROM carga.vw_avaliacao_simec ORDER BY \"CODIGO_GRUPOPET\";

	--- GRAVA OS GRUPO / CURSO
	    INSERT INTO pet.grupopetcurso (grpid, curid)
		SELECT DISTINCT
			       \"CODIGO_GRUPOPET\" AS grpid,
			       (SELECT curid FROM pet.curso WHERE nome = \"CURSO\") AS curid
		FROM carga.vw_avaliacao_simec  WHERE \"CURSO\" IS NOT NULL
		ORDER BY \"CODIGO_GRUPOPET\";

	------------------------------------------------------------
	--- GRAVA OS DISCENTES
	    INSERT INTO pet.discente (cpf, nome )
		SELECT
			       DISTINCT \"CPF_DISCENTE\", \"DISCENTE\"
		FROM carga.vw_avaliacao_simec
		ORDER BY \"DISCENTE\";


	       -- GRAVA A VIGENCIA
	    INSERT INTO pet.vigencia (grpid, disid, datainicioatividade, datafimatividade, bolsista )
		SELECT
			       \"CODIGO_GRUPOPET\",
			       (SELECT disid FROM pet.discente WHERE cpf = \"CPF_DISCENTE\") AS disid,
			       to_date(\"INICIO_ATIVIDADE\", 'DD/MM/YYYY') as datainicioatividade,
			       to_date(\"FIM_ATIVIDADE\", 'DD/MM/YYYY') as datafimatividade,
			       CASE \"BOLSISTA\" WHEN 'S' THEN TRUE ELSE FALSE END
		FROM carga.vw_avaliacao_simec
		ORDER BY \"CODIGO_GRUPOPET\";
	------------------------------------------------------------

	--- GRAVA OS TUTORES
	    INSERT INTO pet.tutor (cpf, nome, grpid, datainiciotutoria)
	    SELECT DISTINCT
			   \"CPF_TUTOR\",
			   \"TUTOR\",
			   \"CODIGO_GRUPOPET\",
			   to_date(\"INICIO TUTORIA\", 'DD/MM/YYYY')  AS datainiciotutoria
	    FROM carga.vw_avaliacao_simec ORDER BY \"TUTOR\"
";


$dados = $db->executar($sql);
exit();

?>
