<?PHP

ini_set('error_reporting', E_ALL);
ini_set('display_errors', 1);


$user = "IECENSOSUP_SIMEC";
$password = "IECENSOSUP_SIMEC";
$dbname = "(DESCRIPTION = (LOAD_BALANCE = YES) (ADDRESS = (PROTOCOL = TCP)(HOST = )(PORT = 1521))(CONNECT_DATA =(SERVER = DEDICATED)(SERVICE_NAME = )))";

$conn = oci_connect($user, $password, $dbname);
if ($conn) {

    $sql = oci_parse($conn, '
SELECT
moda.NO_MODALIDADE_ENSINO  as DESCRICAO,
count(moda.NO_MODALIDADE_ENSINO) as VALOR
FROM
  ODSCENSOSUP_2012.DM_IES ies
  INNER JOIN ODSCENSOSUP_2012.DM_LOCAL_OFERTA_IES loof ON ies.CO_DM_IES = loof.CO_DM_IES
  INNER JOIN ODSCENSOSUP_2012.DM_CURSO curso ON loof.CO_DM_LOCAL_OFERTA_IES = curso.CO_DM_LOCAL_OFERTA_IES
  INNER JOIN ODSCENSOSUP_2012.DM_MODALIDADE_ENSINO moda ON curso.CO_DM_MODALIDADE_ENSINO = moda.CO_DM_MODALIDADE_ENSINO
WHERE ies.CO_CATEGORIA_ADMINISTRATIVA = 211
AND ies.CO_DM_IES = 125
AND curso.CO_DM_NIVEL_ACADEMICO = 1
GROUP BY ies.NO_IES , moda.NO_MODALIDADE_ENSINO
');

        echo "Consulta -1-";
        print "<br>";
        print "<br>";
        oci_execute($sql);
        while ( $row = oci_fetch_array($sql)){
                $desc = $row['DESCRICAO'];
                $val = $row['VALOR'];
         print  $desc .": ". $val . "<br>\n";
        }
        print "<br>";

    $sql2 = oci_parse($conn, '
-- Cor
SELECT CR.NO_COR_RACA AS descricao , COUNT (CR.CO_DM_COR_RACA) AS valor
--*
FROM ODSCENSOSUP_2012.DM_IES I
INNER JOIN ODSCENSOSUP_2012.DM_CURSO C ON (C.CO_DM_IES = I.CO_DM_IES)
INNER JOIN ODSCENSOSUP_2012.FT_MATRICULA M ON (M.CO_DM_CURSO = C.CO_DM_CURSO)
INNER JOIN ODSCENSOSUP_2012.DM_ALUNO A ON (A.CO_DM_ALUNO = M.CO_DM_ALUNO)
INNER JOIN ODSCENSOSUP_2012.DM_COR_RACA CR ON (CR.CO_DM_COR_RACA = A.CO_DM_COR_RACA)
WHERE I.CO_DM_IES = 125
AND I.CO_CATEGORIA_ADMINISTRATIVA = 211
AND C.CO_DM_NIVEL_ACADEMICO = 1
GROUP BY CR.NO_COR_RACA , CR.CO_DM_COR_RACA
');

        echo "Consulta -2-";
        print "<br>";
        print "<br>";
        oci_execute($sql2);
        while ( $row = oci_fetch_array($sql2)){
                $desc = $row['DESCRICAO'];
                $val = $row['VALOR'];
         print  $desc .": ". $val . "<br>\n";
        }
        print "<br>";



    $sql3 = oci_parse($conn, '
-- Situação
SELECT S.NO_ALUNO_SITUACAO as descricao , COUNT (S.CO_DM_ALUNO_SITUACAO) AS valor
--*
FROM ODSCENSOSUP_2012.DM_IES I
INNER JOIN ODSCENSOSUP_2012.DM_CURSO C ON (C.CO_DM_IES = I.CO_DM_IES)
INNER JOIN ODSCENSOSUP_2012.FT_MATRICULA M ON (M.CO_DM_CURSO = C.CO_DM_CURSO)
INNER JOIN ODSCENSOSUP_2012.DM_ALUNO_SITUACAO S ON (S.CO_DM_ALUNO_SITUACAO = M.CO_DM_ALUNO_SITUACAO)
WHERE I.CO_DM_IES = 125
AND I.CO_CATEGORIA_ADMINISTRATIVA = 211
AND C.CO_DM_NIVEL_ACADEMICO = 1
GROUP BY S.NO_ALUNO_SITUACAO , S.CO_DM_ALUNO_SITUACAO
');

        echo "Consulta -3-";
        print "<br>";
        print "<br>";
        oci_execute($sql3);
        while ( $row = oci_fetch_array($sql3)){
                $desc = $row['DESCRICAO'];
                $val = $row['VALOR'];
         print  $desc .": ". $val . "<br>\n";
        }
        print "<br>";



        print "<br>";
        print "<br>";

        echo "conectou usando $dbname";

        print "<br>";
        print "<br>";

} else {
    echo "nao conectou";
}
