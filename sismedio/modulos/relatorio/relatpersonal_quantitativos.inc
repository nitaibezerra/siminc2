<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de Quantitativos</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */


echo "<p>Por universidade</p>";

$sql = "SELECT uu.unisigla||' - '||uu.uninome as universidade, 
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_COORDENADORIES."') as coordenadorgeral, 
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_COORDENADORADJUNTOIES."') as coordenadoradjunto,
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_SUPERVISORIES."') as supervisores,
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_FORMADORIES."') as formador,
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_FORMADORREGIONAL."') as formadorregional,
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_ORIENTADORESTUDO."') as orientadorestudo,
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_PROFESSORALFABETIZADOR."') as professor,
			   (select count(*) from sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd where i.iusstatus='A' and i.uncid = u.uncid and t.pflcod='".PFL_COORDENADORPEDAGOGICO."') as coordenadorpedagogico
		FROM sismedio.universidadecadastro u 
		INNER JOIN sismedio.universidade uu ON uu.uniid = u.uniid 
		";

$cabecalho = array("Universidade","Qtd Coordenador Geral","Qtd Coordenador Adjunto","Qtd Supervisor","Qtd Formador","Qtd Formador Regional","Qtd Orientador Estudo","Qtd Professor","Qtd Coordenador Pedagógico");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);

echo "<p>Por uf</p>";


$sql = "select foo.lemuf, sum(norientadores) as norientadores, sum(nprofessornaobolsista) as nprofessornaobolsista, sum(nprofessorbolsista) as nprofessorbolsista, sum(ncoordenadorpedagogico) as ncoordenadorpedagogico from (
select 
m.lemuf,
CASE WHEN doe.doeid IS NOT NULL THEN 1 ELSE 0 END as norientadores,
CASE WHEN i.iustipoprofessor='cpflivre' AND t.pflcod=1082 AND doe.doeid IS NULL THEN 1 ELSE 0 END as nprofessornaobolsista,
CASE WHEN i.iustipoprofessor='censo' AND t.pflcod=1082 AND doe.doeid IS NULL THEN 1 ELSE 0 END as nprofessorbolsista,
CASE WHEN t.pflcod=1088 AND doe.doeid IS NULL THEN 1 ELSE 0 END as ncoordenadorpedagogico
from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
inner join sismedio.listaescolasensinomedio m ON m.lemcodigoinep::integer = i.iuscodigoinep 
left join sismedio.definicaoorientadoresestudo doe ON doe.iusd = i.iusd AND doe.doecodigoinep = i.iuscodigoinep 
where i.iusstatus='A' and t.pflcod in(1082,1088)
) foo group by foo.lemuf 
order by foo.lemuf";

$cabecalho = array("UF","Qtd Orientador de Estudo","Qtd Professor (Não bolsista)","Qtd Professor (Bolsista)","Qtd Coordenador Pedagógico");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);


?>