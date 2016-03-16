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
        <tr><td class="SubTituloEsquerda">Quantitativo por universidade</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "select 
uu.unisigla ||' - '|| uu.uninome as universidade,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod=".PFL_COORDENADORIES." and i.uncid= uc.uncid ) as coordenadorgeral,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod=".PFL_COORDENADORADJUNTOIES." and i.uncid= uc.uncid) as coordenadoradjunto,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod=".PFL_SUPERVISORIES." and i.uncid= uc.uncid) as supervisor,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod in(".PFL_FORMADORIES.",".PFL_FORMADORIESP.") and i.uncid= uc.uncid) as formador,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod=".PFL_COORDENADORLOCAL." and i.uncid= uc.uncid) as coordenadorlocal,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod=".PFL_ORIENTADORESTUDO." and i.iusformacaoinicialorientador=true and i.uncid= uc.uncid) as orientador,
(select count(*) from sispacto2.identificacaousuario i inner join sispacto2.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and t.pflcod=".PFL_PROFESSORALFABETIZADOR." and i.uncid= uc.uncid) as professor
from sispacto2.universidadecadastro uc 
inner join sispacto2.universidade uu on uu.uniid = uc.uniid 
order by 1";
			
			$cabecalho = array("Universidade","Qtd Coordenador Geral","Qtd Coordenador Adjunto","Qtd Supervisor","Qtd Formadores(Mat+Port)","Qtd Coordenador Local","Qtd Orientador","Qtd Professor");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
			 
			?>
			</td>
		</tr>
		
        <tr><td class="SubTituloEsquerda">Quantitativo por UF</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "select foo.uf, sum(coordenadorlocal) as coordenadorlocal, sum(orientador) as orientador, sum(professor) as professor from (
select 
case when p.muncod is not null then m.estuf 
else p.estuf end as uf,
case when t.pflcod=".PFL_COORDENADORLOCAL." then 1 else 0 end as coordenadorlocal,
case when t.pflcod=".PFL_ORIENTADORESTUDO." then 1 else 0 end as orientador,
case when t.pflcod=".PFL_PROFESSORALFABETIZADOR." then 1 else 0 end as professor
from sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
inner join sispacto2.pactoidadecerta p on p.picid = i.picid 
left join territorios.municipio m on m.muncod = p.muncod
where t.pflcod in(".PFL_COORDENADORLOCAL.",".PFL_ORIENTADORESTUDO.",".PFL_PROFESSORALFABETIZADOR.") and i.iusstatus='A' and case when t.pflcod=".PFL_ORIENTADORESTUDO." then i.iusformacaoinicialorientador=true else true end
) foo 
group by foo.uf 
order by 1";
			
			$cabecalho = array("UF","Qtd Coordenador Local","Qtd Orientador","Qtd Professor");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
			 
			?>
			</td>
		</tr>
</table>