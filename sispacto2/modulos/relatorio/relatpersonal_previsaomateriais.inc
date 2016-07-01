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
        <tr><td class="SubTituloEsquerda">Planilha de distribuição de material do Pacto da Alfabetização 2015 - Censo 2014 homologado</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "select 
case when p.muncod is not null then m.estuf
     when p.estuf is not null then e.estuf end as uf,
case when p.muncod is not null then 'Municipal'
     when p.estuf is not null then 'Estadual' end as esfera,
m.mundescricao as municipio,
m.muncod,
case when p.muncod is not null then secmunicipal.usunome
     when p.estuf is not null then secestadual.usunome end as nomesec,
case when p.muncod is not null then secmunicipal.usucpf
     when p.estuf is not null then secestadual.usucpf end as cpfsec,
case when p.muncod is not null then secmunicipal.usuemail
     when p.estuf is not null then secestadual.usuemail end as emailsec,
case when p.muncod is not null then secmunicipal.telefone
     when p.estuf is not null then secestadual.telefone end as telefonesec,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod=1119 and i.picid=p.picid) as participantes_CL,
case when p.muncod is not null then totmun.total_orientadores_a_serem_cadastrados
     when p.estuf is not null then totest.total_orientadores_a_serem_cadastrados end as totorientadores,
case when p.muncod is not null then totmun.total
     when p.estuf is not null then totest.total end as totprofessores


from sispacto2.pactoidadecerta p 
left join (
select sigla, total, total_orientadores_a_serem_cadastrados from sispacto2.totalalfabetizadores where dependencia='ESTADUAL'
) totest on totest.sigla = p.estuf
left join (
select cod_municipio, total, total_orientadores_a_serem_cadastrados from sispacto2.totalalfabetizadores where dependencia='MUNICIPAL'
) totmun on totmun.cod_municipio = p.muncod
left join territorios.municipio m on m.muncod = p.muncod 
left join territorios.estado e on e.estuf = p.estuf 
left join (

select u.usucpf, u.usunome, u.usuemail, '('||u.usufoneddd||')'||u.usufonenum as telefone, foo.muncod from sispacto2.usuarioresponsabilidade r 
inner join seguranca.usuario u on u.usucpf = r.usucpf
inner join (
select muncod, max(rpudata_inc) as dataa from sispacto2.usuarioresponsabilidade where pflcod in(1127,1126) and rpustatus='A' and muncod is not null group by muncod
) foo on foo.muncod = r.muncod and foo.dataa = r.rpudata_inc 
where rpustatus='A'

) as secmunicipal on secmunicipal.muncod = p.muncod
left join (

select u.usucpf, u.usunome, u.usuemail, '('||u.usufoneddd||')'||u.usufonenum as telefone, foo.estuf from sispacto2.usuarioresponsabilidade r 
inner join seguranca.usuario u on u.usucpf = r.usucpf
inner join (
select estuf, max(rpudata_inc) as dataa from sispacto2.usuarioresponsabilidade where pflcod in(1127,1126) and rpustatus='A' and estuf is not null group by estuf
) foo on foo.estuf = r.estuf and foo.dataa = r.rpudata_inc 
where rpustatus='A'

) as secestadual on secestadual.estuf = p.estuf
order by 2,1,3";
			
			
				
			$cabecalho = array("UF","Esfera","Município","Código IBGE", "Nome do Secretário de Educação", "CPF do Secretário de Educação", "E-mail do Secretário de Educação", "Telefone do Secretário", "Número de Participantes Coordenadores Locais", "Número de Vagas para Orientadores de Estudo", "Número de Vagas para Professores Alfabetizadores");

			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		

</table>