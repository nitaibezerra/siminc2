<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Datas por universidade</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "
			select 
			
			universidade,
			di1||' - '||df1 as per1, 
			di2||' - '||df2 as per2,
			di3||' - '||df3 as per3,
			di4||' - '||df4 as per4,
			di5||' - '||df5 as per5,
			di6||' - '||df6 as per6,
			di7||' - '||df7 as per7,
			di8||' - '||df8 as per8,
			di9||' - '||df9 as per9,
			di10||' - '||df10 as per10,
			di11||' - '||df11 as per11,
			di12||' - '||df12 as per12,
			di13||' - '||df13 as per13
			
			from (
select 
foo.universidade,
case when ini_1 is not null then to_char(ini_1,'dd/mm/YYYY') else to_char(iniprev_1,'dd/mm/YYYY') end as di1,
case when ini_2 is not null then to_char(ini_2,'dd/mm/YYYY') else to_char(iniprev_2,'dd/mm/YYYY') end as di2,
case when ini_3 is not null then to_char(ini_3,'dd/mm/YYYY') else to_char(iniprev_3,'dd/mm/YYYY') end as di3,
case when ini_4 is not null then to_char(ini_4,'dd/mm/YYYY') else to_char(iniprev_4,'dd/mm/YYYY') end as di4,
case when ini_5 is not null then to_char(ini_5,'dd/mm/YYYY') else to_char(iniprev_5,'dd/mm/YYYY') end as di5,
case when ini_6 is not null then to_char(ini_6,'dd/mm/YYYY') else to_char(iniprev_6,'dd/mm/YYYY') end as di6,
case when ini_7 is not null then to_char(ini_7,'dd/mm/YYYY') else to_char(iniprev_7,'dd/mm/YYYY') end as di7,
case when ini_8 is not null then to_char(ini_8,'dd/mm/YYYY') else to_char(iniprev_8,'dd/mm/YYYY') end as di8,
case when ini_9 is not null then to_char(ini_9,'dd/mm/YYYY') else to_char(iniprev_9,'dd/mm/YYYY') end as di9,
case when ini_10 is not null then to_char(ini_10,'dd/mm/YYYY') else to_char(iniprev_10,'dd/mm/YYYY') end as di10,
case when ini_11 is not null then to_char(ini_11,'dd/mm/YYYY') else to_char(iniprev_11,'dd/mm/YYYY') end as di11,
case when ini_12 is not null then to_char(ini_12,'dd/mm/YYYY') else to_char(iniprev_12,'dd/mm/YYYY') end as di12,
case when ini_13 is not null then to_char(ini_13,'dd/mm/YYYY') else to_char(iniprev_13,'dd/mm/YYYY') end as di13,

case when fim_1 is not null then to_char(fim_1,'dd/mm/YYYY') else to_char(fimprev_1,'dd/mm/YYYY') end as df1,
case when fim_2 is not null then to_char(fim_2,'dd/mm/YYYY') else to_char(fimprev_2,'dd/mm/YYYY') end as df2,
case when fim_3 is not null then to_char(fim_3,'dd/mm/YYYY') else to_char(fimprev_3,'dd/mm/YYYY') end as df3,
case when fim_4 is not null then to_char(fim_4,'dd/mm/YYYY') else to_char(fimprev_4,'dd/mm/YYYY') end as df4,
case when fim_5 is not null then to_char(fim_5,'dd/mm/YYYY') else to_char(fimprev_5,'dd/mm/YYYY') end as df5,
case when fim_6 is not null then to_char(fim_6,'dd/mm/YYYY') else to_char(fimprev_6,'dd/mm/YYYY') end as df6,
case when fim_7 is not null then to_char(fim_7,'dd/mm/YYYY') else to_char(fimprev_7,'dd/mm/YYYY') end as df7,
case when fim_8 is not null then to_char(fim_8,'dd/mm/YYYY') else to_char(fimprev_8,'dd/mm/YYYY') end as df8,
case when fim_9 is not null then to_char(fim_9,'dd/mm/YYYY') else to_char(fimprev_9,'dd/mm/YYYY') end as df9,
case when fim_10 is not null then to_char(fim_10,'dd/mm/YYYY') else to_char(fimprev_10,'dd/mm/YYYY') end as df10,
case when fim_11 is not null then to_char(fim_11,'dd/mm/YYYY') else to_char(fimprev_11,'dd/mm/YYYY') end as df11,
case when fim_12 is not null then to_char(fim_12,'dd/mm/YYYY') else to_char(fimprev_12,'dd/mm/YYYY') end as df12,
case when fim_13 is not null then to_char(fim_13,'dd/mm/YYYY') else to_char(fimprev_13,'dd/mm/YYYY') end as df13

from (

select 
uu.unisigla || ' / ' || uu.uninome as universidade,

(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 1 AND es.uncid=u.uncid) as iniprev_1, 
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 2 AND es.uncid=u.uncid) as iniprev_2, 
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 3 AND es.uncid=u.uncid) as iniprev_3, 
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 4 AND es.uncid=u.uncid) as iniprev_4, 
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 5 AND es.uncid=u.uncid) as iniprev_5, 
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 6 AND es.uncid=u.uncid) as iniprev_6, 
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 7 AND es.uncid=u.uncid) as iniprev_7, 
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 8 AND es.uncid=u.uncid) as iniprev_8,
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 9 AND es.uncid=u.uncid) as iniprev_9,
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 10 AND es.uncid=u.uncid) as iniprev_10,
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 11 AND es.uncid=u.uncid) as iniprev_11,
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 12 AND es.uncid=u.uncid) as iniprev_12,
(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 13 AND es.uncid=u.uncid) as iniprev_13,
		
		
		
		
		

 
 
 (SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 1 AND es.uncid=u.uncid) as fimprev_1, 
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 2 AND es.uncid=u.uncid) as fimprev_2, 
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 3 AND es.uncid=u.uncid) as fimprev_3, 
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 4 AND es.uncid=u.uncid) as fimprev_4, 
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 5 AND es.uncid=u.uncid) as fimprev_5, 
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 6 AND es.uncid=u.uncid) as fimprev_6, 
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 7 AND es.uncid=u.uncid) as fimprev_7, 
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 8 AND es.uncid=u.uncid) as fimprev_8,
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 9 AND es.uncid=u.uncid) as fimprev_9,
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 10 AND es.uncid=u.uncid) as fimprev_10,
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 11 AND es.uncid=u.uncid) as fimprev_11,
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 12 AND es.uncid=u.uncid) as fimprev_12,
(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 13 AND es.uncid=u.uncid) as fimprev_13, 
		
		
		
		

 


 (SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 1 AND es.uncid=u.uncid) as ini_1, 
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 2 AND es.uncid=u.uncid) as ini_2, 
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 3 AND es.uncid=u.uncid) as ini_3, 
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 4 AND es.uncid=u.uncid) as ini_4, 
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 5 AND es.uncid=u.uncid) as ini_5, 
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 6 AND es.uncid=u.uncid) as ini_6, 
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 7 AND es.uncid=u.uncid) as ini_7, 
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 8 AND es.uncid=u.uncid) as ini_8,
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 9 AND es.uncid=u.uncid) as ini_9,
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 10 AND es.uncid=u.uncid) as ini_10,
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 11 AND es.uncid=u.uncid) as ini_11,
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 12 AND es.uncid=u.uncid) as ini_12,
(SELECT au.aundatainicio FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 13 AND es.uncid=u.uncid) as ini_13, 
		
		
		
		


 

 (SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 1 AND es.uncid=u.uncid) as fim_1, 
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 2 AND es.uncid=u.uncid) as fim_2, 
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 3 AND es.uncid=u.uncid) as fim_3, 
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 4 AND es.uncid=u.uncid) as fim_4, 
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 5 AND es.uncid=u.uncid) as fim_5, 
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 6 AND es.uncid=u.uncid) as fim_6, 
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 7 AND es.uncid=u.uncid) as fim_7, 
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 8 AND es.uncid=u.uncid) as fim_8,
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 9 AND es.uncid=u.uncid) as fim_9,
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 10 AND es.uncid=u.uncid) as fim_10,
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 11 AND es.uncid=u.uncid) as fim_11,
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 12 AND es.uncid=u.uncid) as fim_12,
(SELECT au.aundatafim FROM sismedio.atividadeuniversidade au 
 INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid 
 WHERE au.suaid = 13 AND es.uncid=u.uncid) as fim_13
		

from sismedio.universidadecadastro u 
inner join sismedio.universidade uu on uu.uniid = u.uniid
) foo ) foo2
			";
			
			$cabecalho = array("Universidade","1.1. I Seminário Estadual - Mobilização",
								"2.1. Formação inicial (24h)",
								"2.2. 1º Encontro com Formadores Regionais (8h) - Aprofundamento",
								"2.3. 2º Encontro com Formadores Regionais (8h) - Acompanhamento",
								"2.4. 3º Encontro com Formadores Regionais (8h) - Avaliação",
								"2.5. Apresentação do relatório parcial sobre a execução da Formação",
								"3.1. II Seminário Estadual - Acompanhamento/Avaliação da 1ª Etapa e preparação da 2ª Etapa da Formação",
								"4.1. Formação inicial dos Formadores Regionais (24h)",
								"4.2. 1º Encontro com Formadores Regionais (8h) - Aprofundamento",
								"4.3. 2º Encontro com Formadores Regionais (8h) - Acompanhamento",
								"4.4. 3º Encontro com Formadores Regionais (8h) - Avaliação",
								"4.5. III Seminário Estadual - Avaliação da Formação no âmbito do Estado",
								"4.6. Apresentação do relatório final sobre a execução da Formação");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		

</table>