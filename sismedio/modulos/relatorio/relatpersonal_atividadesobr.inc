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
	<td class="SubTituloDireita">Universidade : </td>
	<td>
	<?
	$sql = "SELECT u.uncid as codigo, uu.unisigla||' / '||uu.uninome as descricao FROM sismedio.universidadecadastro u 
			inner join sismedio.universidade uu on uu.uniid = u.uniid";
	$db->monta_combo('uncid', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'uncid', '', $_REQUEST['uncid']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Período de referência : </td>
	<td>
	<?
	$sql = "SELECT f.fpbid as codigo, 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as descricao 
			FROM sismedio.folhapagamento f 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE f.fpbstatus='A'";
	$db->monta_combo('fpbid', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'fpbid', '', $_REQUEST['fpbid']);
	?>
	</td>
</tr>
</table>
<?

echo '<p align="center"><input type="button" value="Aplicar" onclick="window.location=\'sismedio.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_atividadesobr.inc&uncid=\'+document.getElementById(\'uncid\').value+\'&fpbid=\'+document.getElementById(\'fpbid\').value;this.disabled=true;;this.disabled=true;"> <input type=button value="Limpar" onclick="window.location=\'sismedio.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_atividadesobr.inc\';"></p>';

if($_REQUEST['uncid']) {
	$wh[] = "i.uncid='".$_REQUEST['uncid']."'";
}

if($_REQUEST['fpbid']) {
	$wh[] = "r.fpbid='".$_REQUEST['fpbid']."'";
}

if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sismedio']['universidade']['uncid']) {
	$wh[] = "i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
}



echo '<p>O seu Orientador de Estudos propôs que realizasse atividade(s) do(s) Caderno(s) de Formação?</p>';

$sql = "select foo.periodo, 
       sum(foo.caroeproposatividadecadernoformacao1) as caroeproposatividadecadernoformacao1, 
       round((sum(foo.caroeproposatividadecadernoformacao1)::numeric/sum(foo.total)::numeric)*100,2) as porc_caroeproposatividadecadernoformacao1,
       sum(foo.caroeproposatividadecadernoformacao2) as caroeproposatividadecadernoformacao2, 
       round((sum(foo.caroeproposatividadecadernoformacao2)::numeric/sum(foo.total)::numeric)*100,2) as porc_caroeproposatividadecadernoformacao2,
       sum(foo.caroeproposatividadecadernoformacao3) as caroeproposatividadecadernoformacao3,
       round((sum(foo.caroeproposatividadecadernoformacao3)::numeric/sum(foo.total)::numeric)*100,2) as porc_caroeproposatividadecadernoformacao3,
       sum(foo.total) as total
from (
select 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as periodo, 
	case when r.caroeproposatividadecadernoformacao='1' then 1 else 0 end as caroeproposatividadecadernoformacao1,
	case when r.caroeproposatividadecadernoformacao='2' then 1 else 0 end as caroeproposatividadecadernoformacao2,
	case when r.caroeproposatividadecadernoformacao='3' then 1 else 0 end as caroeproposatividadecadernoformacao3,
	1 as total,
	f.fpbid
from sismedio.cadernoatividadesrespostas r 
inner join sismedio.identificacaousuario i on i.iusd = r.iusd 
inner join sismedio.folhapagamento f on f.fpbid = r.fpbid 
inner join public.meses m on m.mescod::integer = f.fpbmesreferencia
where caroeproposatividadecadernoformacao is not null ".(($wh)?"and ".implode(" and ",$wh):"")."
) foo 
group by foo.periodo, foo.fpbid
order by foo.fpbid";
	
$cabecalho = array("Período",
				   "Sim, o Orientador de Estudo propôs uma ou mais atividades previstas nos Cadernos",
				   "%",
				   "Não, o Orientador de Estudo não propôs uma ou mais atividades previstas nos Cadernos",
				   "%",
				   "Não utilizamos o(s) Caderno(s) de Formação fornecidos digitalmente pelo MEC",
				   "%",
				   "Total de professores");

$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');



$campos = array("carconteudocadernoformacao","carclarezainstrucoesatividades","carclarezaobjetivosatividades","carvinculacaocontexto","cararticulacaoteoriapratica");

foreach($campos as $campo) {

	$sql = "select c.caddsc as caderno, a.caddsc as atividade, coalesce(r.{$campo},'Em branco') as {$campo}, count(*) as total 
			from sismedio.cadernoatividades a 
			inner join sismedio.cadernoatividades c on c.cadid = a.cadidpai 
			inner join sismedio.cadernoatividadesrespostas r on r.cadid = a.cadid 
			inner join sismedio.identificacaousuario i on i.iusd = r.iusd 
			where a.cadtipo='A' ".(($wh)?"and ".implode(" and ",$wh):"")."
			group by c.caddsc, a.caddsc, r.{$campo}, a.cadid
			order by a.cadid, r.{$campo}";
	
	$cadernoatividadesrespostas = $db->carregar($sql);
	
	if($cadernoatividadesrespostas[0]) {
		foreach($cadernoatividadesrespostas as $car) {
			$agrupado[$car['caderno']][$car['atividade']][$campo][$car[$campo]] = $car['total'];
			$agrupadototal[$car['caderno']][$car['atividade']][$campo] += $car['total'];
		}
	}

}

$indice=0;
if($agrupado) {
	foreach($agrupado as $caderno => $arr) {
		
		foreach($arr as $atividade => $arr2) {

			$arrfinal[$indice]['caderno'] = '<span style=font-size:x-small;>'.$caderno.'</span>';

			$arrfinal[$indice]['atividade'] = '<span style=font-size:x-small;>'.$atividade.'</span>';
			
			foreach($arr2 as $campo => $arr3) {

				foreach($arr3 as $valor => $qtd) {
					$html[] = "<span style=font-size:x-small;><b>{$valor}</b> ( {$qtd} / ".round(($qtd/$agrupadototal[$caderno][$atividade][$campo])*100,2)."% )</span>";
				}
				
				$arrfinal[$indice][$campo] = implode("<br>",$html);
				unset($html);
				
			}
			
			$indice++;
		}
	}
}

echo '<p>Pontue a atividade, considerando os critérios a seguir e a escala de 1 a 5 (Considere: 1 – Muito fraca/ 5 – Muito boa)</p>';

$cabecalho = array("Caderno",
					"Atividade",
					"Relação com o conteúdo do Caderno de Formação",
					"Clareza das instruções para realização da atividade",
					"Clareza do objetivo da realização da atividade",
					"Vinculação ao contexto de sala de aula/cotidiano escolar",
					"Articulação entre teoria e prática");

if(!$arrfinal) $arrfinal = array();

$db->monta_lista_simples($arrfinal,$cabecalho,100000,5,'N','100%','N');


$sql = "select foo.caddsc, foo.catdsc, count(*) as qtd, round((count(*)::numeric/(select count(distinct r.iusd) from sismedio.cadernoatividadesrespostas r inner join sismedio.identificacaousuario i on i.iusd = r.iusd where catid is not null ".(($wh)?"and ".implode(" and ",$wh):"").")::numeric)*100,2) as porc from (
select distinct r.iusd, r.catid, t.catdsc, c.caddsc, 1 as total from sismedio.cadernoatividadesrespostas r 
inner join sismedio.identificacaousuario i on i.iusd = r.iusd 
inner join sismedio.cadernoatividadestema t on t.catid = r.catid 
inner join sismedio.cadernoatividades c on c.cadid = t.cadid 
".(($wh)?"where ".implode(" and ",$wh):"")."
) foo 
group by foo.catdsc, foo.caddsc, foo.catid
order by foo.catid";

echo '<p>Temas que gostaria que fossem aprofundados em outros cursos.</p>';

$cabecalho = array("Caderno","Temas","Qtd de professores","%");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
?>