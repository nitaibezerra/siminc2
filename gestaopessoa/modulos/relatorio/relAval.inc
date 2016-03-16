
<!-- Relatório Média final -->

<?PHP
    $sql = "
        --AVALIAÇÃO COMPLETA e COM EQUIPE
        SELECT  sersiape, 
                sernome, 
                ROUND(SUM(nota)) AS nota,
                CASE
                    WHEN ROUND(SUM(nota)) <= 30 THEN '6'
                    WHEN ROUND(SUM(nota)) >= 31 AND ROUND(SUM(nota)) <= 40 THEN '8'
                    WHEN ROUND(SUM(nota)) >= 41 AND ROUND(SUM(nota)) <= 50 THEN '10'
                    WHEN ROUND(SUM(nota)) >= 51 AND ROUND(SUM(nota)) <= 60 THEN '12'
                    WHEN ROUND(SUM(nota)) >= 61 AND ROUND(SUM(nota)) <= 70 THEN '14'
                    WHEN ROUND(SUM(nota)) >= 71 AND ROUND(SUM(nota)) <= 80 THEN '16'
                    WHEN ROUND(SUM(nota)) >= 81 AND ROUND(SUM(nota)) <= 90 THEN '18'
                    WHEN ROUND(SUM(nota)) >= 91 AND ROUND(SUM(nota)) <= 100 THEN '20'
                END AS pontos,
                'Avaliação Completa' AS avaliacao
        FROM (		
            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.15) AS nota 
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']}
            ---Autoavaliação
            AND r.tavid = 1  
            ---Tem Avaliação por Equipe
            AND s.sercomequipe = 't'
            ---Avaliação Completa
            AND r.resavaliacaopendente = 'f'
            --- Retira todos os CPF's COM avaliação pendente
            AND s.sercpf NOT IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resavaliacaopendente = 't' AND resano = {$_SESSION['exercicio']})
            GROUP BY sersiape, sernome

            UNION ALL 

            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.60) AS nota
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            WHERE seranoreferencia = {$_SESSION['exercicio']}  AND r.resano = {$_SESSION['exercicio']} 
            ---Avaliação Chefe
            AND r.tavid = 2 
            ---Tem Avaliação por Equipe
            AND s.sercomequipe = 't'
            ---Avaliação Completa
            AND r.resavaliacaopendente = 'f'
            --- Retira todos os CPF's COM avaliação pendente
            AND s.sercpf NOT IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resavaliacaopendente = 't' AND resano = {$_SESSION['exercicio']} ) 
            GROUP BY sersiape, sernome

            UNION ALL 

            SELECT  sersiape,
                    sernome, 
                    SUM(nota)*0.25 as nota 
            FROM (
                SELECT  s.sersiape, 
                        s.sernome, 
                        d.defid, 
                        ROUND(AVG(d.defpeso*r.resnota)) AS nota
                FROM gestaopessoa.servidor s
                INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                WHERE seranoreferencia = {$_SESSION['exercicio']}  AND r.resano = {$_SESSION['exercicio']} 
                ---Avaliação Equipe
                AND tavid = 3 
                ---Tem Avaliação por Equipe
                AND s.sercomequipe = 't'
                ---Avaliação Completa
                AND r.resavaliacaopendente = 'f'
                --- Retira todos os CPF's COM avaliação pendente
                AND s.sercpf NOT IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resavaliacaopendente = 't' AND resano = {$_SESSION['exercicio']} ) 
                GROUP BY s.sersiape, s.sernome, d.defid  ) AS f   
            GROUP BY sersiape, sernome
        ) as foo
        GROUP BY sersiape, sernome

        UNION ALL

        --AVALIAÇÃO INCOMPLETA e COM EQUIPE
        SELECT  sersiape, 
                sernome, 
                ROUND(SUM(nota)) AS nota,
                CASE
                    WHEN ROUND(SUM(nota)) <= 30 THEN '6'
                    WHEN ROUND(SUM(nota)) >= 31 AND ROUND(SUM(nota)) <= 40 THEN '8'
                    WHEN ROUND(SUM(nota)) >= 41 AND ROUND(SUM(nota)) <= 50 THEN '10'
                    WHEN ROUND(SUM(nota)) >= 51 AND ROUND(SUM(nota)) <= 60 THEN '12'
                    WHEN ROUND(SUM(nota)) >= 61 AND ROUND(SUM(nota)) <= 70 THEN '14'
                    WHEN ROUND(SUM(nota)) >= 71 AND ROUND(SUM(nota)) <= 80 THEN '16'
                    WHEN ROUND(SUM(nota)) >= 81 AND ROUND(SUM(nota)) <= 90 THEN '18'
                    WHEN ROUND(SUM(nota)) >= 91 AND ROUND(SUM(nota)) <= 100 THEN '20'
                END AS pontos,
                'Avaliação Incompleta' AS avaliacao
        FROM (		
            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.15) AS nota 
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            WHERE seranoreferencia = {$_SESSION['exercicio']}  AND r.resano = {$_SESSION['exercicio']} 
            ---Autoavaliação
            AND r.tavid = 1  
            ---Avaliação por Equipe
            AND s.sercomequipe = 't'
            ---Avaliação Incompleta
            AND r.resavaliacaopendente = 't'
            --- Retira todos os CPF's SEM avaliação pendente
            AND s.sercpf NOT IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resavaliacaopendente = 'f' AND resano = 2012)  
            GROUP BY sersiape,sernome

            UNION ALL 

            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.60) AS nota
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} 
            ---Avaliação Chefe
            AND r.tavid = 2 
            ---Avaliação por Equipe
            AND s.sercomequipe = 't'
            ---Avaliação Incompleta
            AND r.resavaliacaopendente = 't'
            --- Retira todos os CPF's SEM avaliação pendente
            AND s.sercpf NOT IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resavaliacaopendente = 'f' AND resano = 2012) 
            GROUP BY sersiape,sernome

            UNION ALL 

            SELECT  sersiape,
                    sernome, 
                    SUM(nota)*0.25 as nota 
            FROM(
                SELECT  s.sersiape, 
                        s.sernome, 
                        d.defid, 
                        ROUND(AVG(d.defpeso*r.resnota)) AS nota
                FROM gestaopessoa.servidor s
                INNER JOIN gestaopessoa.respostaavaliacao r ON r.sercpf = s.sercpf
                INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
                WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} 
                ---Equipe
                AND tavid = 3
                ---Avaliação por Equipe
                AND s.sercomequipe = 't'
                ---Avaliação Incompleta
                AND r.resavaliacaopendente = 't'
                --- Retira todos os CPF's SEM avaliação pendente
                AND s.sercpf NOT IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resavaliacaopendente = 'f' AND resano = 2012) 
                ---AND s.sercpf = ''
                GROUP BY s.sersiape,s.sernome,d.defid
            ) AS f   
            GROUP BY sersiape,sernome

        ) as foo
        GROUP BY sersiape,sernome

        UNION ALL

        --AVALIAÇÃO COMPLETA e SEM EQUIPE
        SELECT  sersiape,
                sernome, 
                ROUND(SUM(nota)) as nota,CASE
                    WHEN ROUND(SUM(nota)) <= 30 THEN '6'
                    WHEN ROUND(SUM(nota)) >= 31 AND ROUND(SUM(nota)) <= 40 THEN '8'
                    WHEN ROUND(SUM(nota)) >= 41 AND ROUND(SUM(nota)) <= 50 THEN '10'
                    WHEN ROUND(SUM(nota)) >= 51 AND ROUND(SUM(nota)) <= 60 THEN '12'
                    WHEN ROUND(SUM(nota)) >= 61 AND ROUND(SUM(nota)) <= 70 THEN '14'
                    WHEN ROUND(SUM(nota)) >= 71 AND ROUND(SUM(nota)) <= 80 THEN '16'
                    WHEN ROUND(SUM(nota)) >= 81 AND ROUND(SUM(nota)) <= 90 THEN '18'
                    WHEN ROUND(SUM(nota)) >= 91 AND ROUND(SUM(nota)) <= 100 THEN '20'
                END AS pontos,                
                'Avaliação Completa' AS avaliacao
        FROM (
            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.275) AS nota 
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            WHERE seranoreferencia = {$_SESSION['exercicio']}  AND r.resano = {$_SESSION['exercicio']} 
            ---Autoavaliação
            AND r.tavid = 1  
            ---Não tem Avaliação por Equipe
            AND s.sercomequipe = 'f'
            ---SEM avaliação pendente
            AND r.resavaliacaopendente = 'f'
            AND s.sercpf IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resano = 2012 AND tavid IN (1,2) AND resavaliacaopendente = 'f') 
            GROUP BY sersiape,sernome, r.resavaliacaopendente

            UNION ALL 

            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.725) AS nota
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']} 
            ---Avaliação Chefe
            AND r.tavid = 2 
            ---Não tem Avaliação por Equipe
            AND s.sercomequipe = 'f'
            ---SEM avaliação pendente
            AND r.resavaliacaopendente = 'f'
            AND s.sercpf IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resano = 2012 AND tavid IN (1,2) AND resavaliacaopendente = 'f') 
            GROUP BY sersiape,sernome
        ) as foo
        GROUP BY sersiape,sernome

        UNION ALL 

        --AVALIAÇÃO INCOMPLETA e SEM EQUIPE
        SELECT  sersiape,
                sernome, 
                ROUND(SUM(nota)), 
                CASE
                    WHEN ROUND(SUM(nota)) <= 30 THEN '6'
                    WHEN ROUND(SUM(nota)) >= 31 AND ROUND(SUM(nota)) <= 40 THEN '8'
                    WHEN ROUND(SUM(nota)) >= 41 AND ROUND(SUM(nota)) <= 50 THEN '10'
                    WHEN ROUND(SUM(nota)) >= 51 AND ROUND(SUM(nota)) <= 60 THEN '12'
                    WHEN ROUND(SUM(nota)) >= 61 AND ROUND(SUM(nota)) <= 70 THEN '14'
                    WHEN ROUND(SUM(nota)) >= 71 AND ROUND(SUM(nota)) <= 80 THEN '16'
                    WHEN ROUND(SUM(nota)) >= 81 AND ROUND(SUM(nota)) <= 90 THEN '18'
                    WHEN ROUND(SUM(nota)) >= 91 AND ROUND(SUM(nota)) <= 100 THEN '20'
                END AS pontos,
                'Avaliação Incompleta' AS avaliacao
        FROM (
            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.275) AS nota 
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            WHERE seranoreferencia = {$_SESSION['exercicio']} AND r.resano = {$_SESSION['exercicio']}
            ---Autoavaliação
            AND r.tavid = 1
            ---Não tem Avaliação por Equipe
            AND s.sercomequipe = 'f'
            ---COM avaliação pendente
            AND resavaliacaopendente = 't' 
            AND s.sercpf IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resano = 2012 AND tavid IN (1,2) AND resavaliacaopendente = 't') 
            GROUP BY sersiape,sernome

            UNION ALL 

            SELECT  sersiape,
                    sernome, 
                    SUM(ROUND(d.defpeso*r.resnota)*0.725) AS nota
            FROM gestaopessoa.servidor s
            INNER JOIN gestaopessoa.respostaavaliacao r on r.sercpf = s.sercpf
            INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
            ---Avaliação Chefe
            WHERE seranoreferencia = 2012
            AND r.resano = 2012
            ---Avaliação Chefe
            AND tavid = 2 
            ---Não tem Avaliação por Equipe
            AND s.sercomequipe = 'f'
            --COM avaliação pendente
            AND resavaliacaopendente = 't' 
            AND s.sercpf IN (SELECT DISTINCT sercpf FROM gestaopessoa.respostaavaliacao WHERE resano = 2012 AND tavid IN (1,2) AND resavaliacaopendente = 't')                 
            GROUP BY sersiape,sernome 
        ) as foo
        GROUP BY sersiape, sernome
        ORDER BY sernome
    ";

$cabecalho = array("SIAPE", "Nome", "Avaliação Final", "Pontos GDACE", "Status Avaliação");
$dadosExcel = $db->carregar($sql);

if(empty($dadosExcel)){
	echo '
		<script>
			alert("Nenhum registro encontrado!");
			window.close(); 
		</script>';
	exit;
    
} else{
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$db->sql_to_excel($dadosExcel, 'Relatório_Avaliacao_Final', $cabecalho);
}?>	