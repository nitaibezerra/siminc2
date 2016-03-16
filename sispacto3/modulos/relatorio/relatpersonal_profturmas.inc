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
        <tr><td class="SubTituloEsquerda">Informações sobre Turmas de Professores</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "
select 
foo.estuf, 
foo.muncod, 
foo.mundescricao, 
foo.esfera,  
sum(foo.ano1) as ano1,
sum(foo.ano2) as ano2,
sum(foo.ano3) as ano3,
sum(foo.anoms) as anoms

from (

(
select 
m.estuf, 
m.muncod, 
m.mundescricao, 
'Municipal'::text as esfera,  
case when iusserieprofessor ilike '%01;%' then 1 else 0 end as ano1,
case when iusserieprofessor ilike '%02;%' then 1 else 0 end as ano2,
case when iusserieprofessor ilike '%03;%' then 1 else 0 end as ano3,
case when iusserieprofessor ilike '%MS;%' then 1 else 0 end as anoms
from sispacto3.identificacaousuario i 
inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
inner join sispacto3.pactoidadecerta p on p.picid = i.picid 
inner join territorios.municipio m on m.muncod = p.muncod
where iusstatus='A' and t.pflcod=1379
) union all(
select 
m.estuf, 
m.muncod, 
m.mundescricao, 
'Estadual'::text as esfera,  
case when iusserieprofessor ilike '%01;%' then 1 else 0 end as ano1,
case when iusserieprofessor ilike '%02;%' then 1 else 0 end as ano2,
case when iusserieprofessor ilike '%03;%' then 1 else 0 end as ano3,
case when iusserieprofessor ilike '%MS;%' then 1 else 0 end as anoms
from sispacto3.identificacaousuario i 
inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
inner join sispacto3.pactoidadecerta p on p.picid = i.picid and p.estuf is not null
inner join territorios.municipio m on m.muncod = i.muncodatuacao
where iusstatus='A' and t.pflcod=1379
)

) foo

group by foo.estuf, 
foo.muncod, 
foo.mundescricao, 
foo.esfera 
order by foo.estuf, foo.mundescricao";
			
			
			$cabecalho = array("UF","Cód. Município","Município","Esfera","1º Ano","2º Ano","3º Ano","Multisseriada");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		

</table>