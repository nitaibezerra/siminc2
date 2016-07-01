<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Perfil dos participantes do SISIndígena</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

if(!$_REQUEST['dias']) $_REQUEST['dias'] = "45";

echo '<p> Perfil : ';
$sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p 
		INNER JOIN sisindigena.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		WHERE sisid='".SIS_INDIGENA."'";
$db->monta_combo('pflcod', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'pflcod', '', $_REQUEST['pflcod']);
echo '</p>';

echo '<p align="center"><input type="button" value="Aplicar" onclick="window.location=\'sisindigena.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_perfilparticipantes.inc&pflcod=\'+document.getElementById(\'pflcod\').value;"></p>';

if($_REQUEST['pflcod']) {
	$f_pfl = "AND t.pflcod='".$_REQUEST['pflcod']."'";
}

?>
<p align="center">Os participantes que acessaram o sistema e preencheram o TERMO DE COMPROMISSO</p>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td class="SubTituloDireita" valign="top" width="10%">Faixa Etária :</td>
	<td>
	<?
	$sql = "select faixa, count(foo.iusd), round(( (count(foo.iusd)*100)::numeric / (SELECT count(*) FROM sisindigena.identificacaousuario i inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
	from (
	select  i.iusd,
		CASE WHEN extract(year from age(iusdatanascimento)) >= 60 THEN '60+'
		     WHEN extract(year from age(iusdatanascimento)) >= 50 AND extract(year from age(iusdatanascimento)) < 60 THEN '50-60'
		     WHEN extract(year from age(iusdatanascimento)) >= 40 AND extract(year from age(iusdatanascimento)) < 50 THEN '40-50' 
		     WHEN extract(year from age(iusdatanascimento)) >= 30 AND extract(year from age(iusdatanascimento)) < 40 THEN '30-40' 
		     WHEN extract(year from age(iusdatanascimento)) >= 20 AND extract(year from age(iusdatanascimento)) < 30 THEN '20-30'
		     ELSE '19-' END as faixa
	from sisindigena.identificacaousuario i
	inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl}
	where iustermocompromisso=true
	) foo 
	group by faixa 
	order by 2 desc";
	
	$cabecalho = array("Faixa Etária","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	?>
	</td>
	
	<td class="SubTituloDireita" valign="top" width="10%">Sexo :</td>
	<td>
	<?
	$sql = "select i.iussexo, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sisindigena.identificacaousuario i inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sisindigena.identificacaousuario i
inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl}
where iustermocompromisso=true 
group by i.iussexo
order by 2 desc";
	
	$cabecalho = array("Sexo","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" valign="top">Local de nascimento (por UF) :</td>
	<td>
	<div style="height:150px;overflow:auto;">
	<?
	$sql = "select m.estuf, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sisindigena.identificacaousuario i inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
	from sisindigena.identificacaousuario i
	inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl}
	inner join territorios.municipio m on m.muncod = i.muncod
	where iustermocompromisso=true 
	group by m.estuf 
	order by 2 desc";
	
	$cabecalho = array("UF","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	?>
	</div>
	</td>
	<td class="SubTituloDireita" valign="top">Local de nascimento (por Município) :</td>
	<td>
	<div style="height:150px;overflow:auto;">
	<?
	$sql = "select m.estuf, m.mundescricao, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sisindigena.identificacaousuario i inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sisindigena.identificacaousuario i
inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl}
inner join territorios.municipio m on m.muncod = i.muncod
where iustermocompromisso=true 
group by m.estuf, m.mundescricao
order by 3 desc";
	
	$cabecalho = array("UF","Município","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	?>
	</div>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Formação :</td>
	<td>
	<div style="height:150px;overflow:auto;">
	<?
	$sql = "select f.foedesc, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sisindigena.identificacaousuario i inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sisindigena.identificacaousuario i
inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl}
inner join sisindigena.formacaoescolaridade f on f.foeid = i.foeid
where iustermocompromisso=true 
group by f.foedesc
order by 2 desc";
	
	$cabecalho = array("Formação","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	?>
	</div>
	</td>
	<td class="SubTituloDireita" valign="top">Área de Formação :</td>
	<td>
	<div style="height:150px;overflow:auto;">
	<?
	$sql = "select f.cufcursodesc, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sisindigena.identificacaousuario i inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sisindigena.identificacaousuario i
inner join sisindigena.tipoperfil t on t.iusd = i.iusd {$f_pfl}
inner join sisindigena.identiusucursoformacao ic on ic.iusd = i.iusd 
inner join sisindigena.cursoformacao f on f.cufid = ic.cufid
where iustermocompromisso=true 
group by f.cufcursodesc
order by 2 desc";
	
	$cabecalho = array("Formação","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	?>
	</div>
	</td>
</tr>


</table>

<?

?>