<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>
<p align="center" style="font-size:16px;"><b>Perfil dos participantes do SISMédio</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

if(!$_REQUEST['dias']) $_REQUEST['dias'] = "45";

echo '<p> Perfil : ';
$sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p 
		INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		WHERE sisid='".SIS_MEDIO."'";
$db->monta_combo('pflcod', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'pflcod');
echo '</p>';

echo '<p align="center"><input type="button" value="Aplicar" onclick="window.location=\'sismedio.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_perfilparticipantes.inc&pflcod=\'+document.getElementById(\'pflcod\').value;"></p>';

if($_REQUEST['pflcod']) {
	$f_pfl = "AND t.pflcod='".$_REQUEST['pflcod']."'";
}

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sismedio']['universidade']['uncid']) {
	$f_pfl .= " AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
}

?>
<script>

function detalharAreaFormaca(cufid, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 4;
		ncol.id      = 'tur_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=detalharAreaFormacao<?=(($_REQUEST['pflcod'])?"&pflcod=".$_REQUEST['pflcod']:"")?>&cufid='+cufid,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
	
}

</script>
<p align="center">Os participantes que acessaram o sistema e preencheram o TERMO DE COMPROMISSO</p>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td class="SubTituloDireita" valign="top" width="10%">Faixa Etária :</td>
	<td>
	<?
	$sql = "select faixa, count(foo.iusd), round(( (count(foo.iusd)*100)::numeric / (SELECT count(*) FROM sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
	from (
	select  i.iusd,
		CASE WHEN extract(year from age(iusdatanascimento)) >= 60 THEN '60+'
		     WHEN extract(year from age(iusdatanascimento)) >= 50 AND extract(year from age(iusdatanascimento)) < 60 THEN '50-60'
		     WHEN extract(year from age(iusdatanascimento)) >= 40 AND extract(year from age(iusdatanascimento)) < 50 THEN '40-50' 
		     WHEN extract(year from age(iusdatanascimento)) >= 30 AND extract(year from age(iusdatanascimento)) < 40 THEN '30-40' 
		     WHEN extract(year from age(iusdatanascimento)) >= 20 AND extract(year from age(iusdatanascimento)) < 30 THEN '20-30'
		     ELSE '19-' END as faixa
	from sismedio.identificacaousuario i
	inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl}
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
	$sql = "select i.iussexo, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl}
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
	$sql = "select m.estuf, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
	from sismedio.identificacaousuario i
	inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl}
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
	$sql = "select m.estuf, m.mundescricao, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl}
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
	$sql = "select f.foedesc, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl}
inner join sismedio.formacaoescolaridade f on f.foeid = i.foeid
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
	$sql = "select '<img src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=\"detalharAreaFormaca('||f.cufid||',this);\">' as acao, f.cufcursodesc, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} where iustermocompromisso=true)::numeric ),2) as porcent
from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl}
inner join sismedio.identiusucursoformacao ic on ic.iusd = i.iusd 
inner join sismedio.cursoformacao f on f.cufid = ic.cufid
where iustermocompromisso=true 
group by f.cufid, f.cufcursodesc
order by 3 desc";
	
	$cabecalho = array("&nbsp;","Formação","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	?>
	</div>
	</td>
</tr>


</table>

<?

?>