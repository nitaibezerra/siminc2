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
			<b>Números cadastrados</b><br><br>
			<? 
			$sql = "(
select '2015' as ano, sum(foo.ano1) as ano1, sum(foo.ano2) as ano2, sum(foo.ano3) as ano3, sum(foo.anoms) as anoms

from(

select 
case when iusserieprofessor ilike '%01;%' then 1 else 0 end as ano1,
case when iusserieprofessor ilike '%02;%' then 1 else 0 end as ano2,
case when iusserieprofessor ilike '%03;%' then 1 else 0 end as ano3,
case when iusserieprofessor ilike '%MS;%' then 1 else 0 end as anoms
from sispacto3.identificacaousuario i 
inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
where iusstatus='A' and t.pflcod=1379

) foo

) union all (

select '2014' as ano, sum(foo.ano1) as ano1, sum(foo.ano2) as ano2, sum(foo.ano3) as ano3, sum(foo.anoms) as anoms

from(

select 
case when iusserieprofessor ilike '%01;%' then 1 else 0 end as ano1,
case when iusserieprofessor ilike '%02;%' then 1 else 0 end as ano2,
case when iusserieprofessor ilike '%03;%' then 1 else 0 end as ano3,
case when iusserieprofessor ilike '%MS;%' then 1 else 0 end as anoms
from sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
where iusstatus='A' and t.pflcod=1118

) foo

) union all (

select '2013' as ano, sum(foo.ano1) as ano1, sum(foo.ano2) as ano2, sum(foo.ano3) as ano3, sum(foo.anoms) as anoms

from(

select 
case when iusserieprofessor ilike '%01;%' then 1 else 0 end as ano1,
case when iusserieprofessor ilike '%02;%' then 1 else 0 end as ano2,
case when iusserieprofessor ilike '%03;%' then 1 else 0 end as ano3,
case when iusserieprofessor ilike '%MS;%' then 1 else 0 end as anoms
from sispacto.identificacaousuario i 
inner join sispacto.tipoperfil t on t.iusd = i.iusd 
where iusstatus='A' and t.pflcod=849

) foo

)";
			
			
			$cabecalho = array("Ano","1º Ano","2º Ano","3º Ano","Multisseriada");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		
				<tr>
			<td>
			<b>Números certificados</b><br><br>
			<? 
			$sql = "(

select '2014' as ano, sum(foo.ano1) as ano1, sum(foo.ano2) as ano2, sum(foo.ano3) as ano3, sum(foo.anoms) as anoms

from(

select 
case when iusserieprofessor ilike '%01;%' then 1 else 0 end as ano1,
case when iusserieprofessor ilike '%02;%' then 1 else 0 end as ano2,
case when iusserieprofessor ilike '%03;%' then 1 else 0 end as ano3,
case when iusserieprofessor ilike '%MS;%' then 1 else 0 end as anoms
from sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
inner join sispacto2.certificacao c on c.iusd = i.iusd 
where iusstatus='A' and t.pflcod=1118 and c.cerfrequencia >= 75

) foo

) union all (

select '2013' as ano, sum(foo.ano1) as ano1, sum(foo.ano2) as ano2, sum(foo.ano3) as ano3, sum(foo.anoms) as anoms

from(

select 
case when iusserieprofessor ilike '%01;%' then 1 else 0 end as ano1,
case when iusserieprofessor ilike '%02;%' then 1 else 0 end as ano2,
case when iusserieprofessor ilike '%03;%' then 1 else 0 end as ano3,
case when iusserieprofessor ilike '%MS;%' then 1 else 0 end as anoms
from sispacto.identificacaousuario i 
inner join sispacto.tipoperfil t on t.iusd = i.iusd 
inner join sispacto.certificacao c on c.iusd = i.iusd
where iusstatus='A' and t.pflcod=849 and c.cercertificou = true

) foo

)";
			
			
			$cabecalho = array("Ano","1º Ano","2º Ano","3º Ano","Multisseriada");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		

</table>