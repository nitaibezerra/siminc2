<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Avaliação complementar</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

?>

<script>
function selecionarTipo(v) {
	
	if(v=='1') {
		document.getElementById('pflcodavaliador').disabled=false;
		document.getElementById('pflcodavaliado').disabled=false;
	}

	if(v=='2') {
		document.getElementById('pflcodavaliador').disabled=true;
		document.getElementById('pflcodavaliado').disabled=true;
	}
	
}
</script>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloDireita">Tipo avaliação complementar : </td>
	<td>
	<? 
	$t[] = array("codigo" => "1", "descricao" => "Avaliação Complementar de bolsitas");
	$t[] = array("codigo" => "2", "descricao" => "Avaliação Complementar do conteúdo");
	$db->monta_combo('tipoavaliacaocomplementar', $t, 'S', '', 'selecionarTipo', '', '', '', 'N', 'tipoavaliacaocomplementar', '', $_REQUEST['tipoavaliacaocomplementar']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Perfil do avaliador : </td>
	<td>
	<?
	$sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p
		INNER JOIN sispacto.pagamentoperfil pp ON pp.pflcod = p.pflcod
		WHERE sisid='".SIS_SISPACTO."'";
	$db->monta_combo('pflcodavaliador', $sql, (($_REQUEST['tipoavaliacaocomplementar']=='2')?'N':'S'), 'TODOS', '', '', '', '', 'N', 'pflcodavaliador', '', $_REQUEST['pflcodavaliador']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Perfil do avaliado : </td>
	<td>
	<?
	$sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p
			INNER JOIN sispacto.pagamentoperfil pp ON pp.pflcod = p.pflcod
			WHERE sisid='".SIS_SISPACTO."'";
	$db->monta_combo('pflcodavaliado', $sql, (($_REQUEST['tipoavaliacaocomplementar']=='2')?'N':'S'), 'TODOS', '', '', '', '', 'N', 'pflcodavaliado', '', $_REQUEST['pflcodavaliado']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Universidade : </td>
	<td>
	<?
	$sql = "SELECT u.uncid as codigo, uu.unisigla||' / '||uu.uninome as descricao FROM sispacto.universidadecadastro u 
			inner join sispacto.universidade uu on uu.uniid = u.uniid";
	$db->monta_combo('uncid', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'uncid', '', $_REQUEST['uncid']);
	?>
	</td>
</tr>
</table>
<?

echo '<p align="center"><input type="button" value="Aplicar" onclick="window.location=\'sispacto.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_notacomplementar.inc&tipoavaliacaocomplementar=\'+document.getElementById(\'tipoavaliacaocomplementar\').value+\'&pflcodavaliador=\'+document.getElementById(\'pflcodavaliador\').value+\'&pflcodavaliado=\'+document.getElementById(\'pflcodavaliado\').value+\'&uncid=\'+document.getElementById(\'uncid\').value;this.disabled=true;"> <input type=button value="Limpar" onclick="window.location=\'sispacto.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_notacomplementar.inc\';"></p>';



if($_REQUEST['tipoavaliacaocomplementar']=='1') :


if($_REQUEST['pflcodavaliador'] && $_REQUEST['pflcodavaliado'] && $_REQUEST['uncid']) :
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td>
	<?
	$sql = "select  i.iuscpf, i.iusnome, 
	case when p.picid is null then 'Equipe IES'
	     when p.muncod is not null then m.estuf||' / '||m.mundescricao||'( Municipal )'
	     when p.estuf is not null then e.estuf||' / '||e.estdescricao||'( Estadual )' end as esfera,
	count(distinct i2.iusd) as avaliadores,
	avg(a.iccvalor) as nota 
from sispacto.respostasavaliacaocomplementar r 
inner join sispacto.itensavaliacaocomplementarcriterio a on a.iccid = r.iccid 
inner join sispacto.identificacaousuario i on i.iusd = r.iusdavaliado 
inner join sispacto.tipoperfil t on t.iusd = i.iusd 
left join sispacto.pactoidadecerta p on p.picid = i.picid 
left join territorios.municipio m on m.muncod = p.muncod 
left join territorios.estado e on e.estuf = p.estuf
inner join sispacto.identificacaousuario i2 on i2.iusd = r.iusdavaliador 
inner join sispacto.tipoperfil t2 on t2.iusd = i2.iusd 
where t2.pflcod=".$_REQUEST['pflcodavaliador']." and t.pflcod=".$_REQUEST['pflcodavaliado']." and i.iusstatus='A' and i.uncid='".$_REQUEST['uncid']."'
group by i.iuscpf, i.iusnome, p.picid, p.muncod, p.estuf, m.estuf, e.estuf, m.mundescricao, e.estdescricao
order by 5";
	
	$cabecalho = array("CPF","Nome","Esfera","Número de avaliadores","Nota");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	?>
	</td>

</tr>

</table>
<?
else :
 
echo '<p>Selecione o perfil do avaliador, o perfil avaliado e a universidade</p>';

endif;


elseif($_REQUEST['tipoavaliacaocomplementar']=='2') :


if($_REQUEST['uncid']) :
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td>
	<?
	$sql = "select  pp.pfldsc,
	i.iacdsc,
	count(distinct r.iusdavaliador) as avaliadores,
	avg(a.iccvalor) as nota 
from sispacto.respostasavaliacaocomplementar r 
inner join sispacto.identificacaousuario ii on ii.iusd = r.iusdavaliador 
inner join sispacto.tipoperfil t on t.iusd = ii.iusd 
inner join sispacto.itensavaliacaocomplementarcriterio a on a.iccid = r.iccid 
inner join sispacto.itensavaliacaocomplementar i on i.iacid = r.iacid 
inner join seguranca.perfil pp on pp.pflcod = t.pflcod 
where i.gicid=1 and ii.uncid='".$_REQUEST['uncid']."' and ii.iusstatus='A'
group by i.iacdsc, pp.pfldsc
order by 4";
	
	$cabecalho = array("Perfil avaliador","Critério","Número de avaliadores","Nota");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	?>
	</td>

</tr>

</table>
<?
else :
 
echo '<p>Selecione a universidade</p>';

endif;

endif;
?>