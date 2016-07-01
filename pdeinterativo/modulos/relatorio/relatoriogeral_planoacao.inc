<script>
function linkBensServicoes(esfera,estuf,muncod,ciaid) {
	var janela = window.open( 'pdeinterativo.php?modulo=relatorio/relatoriogeraldimensoes&acao=A&buscar=1&dimensao=planoacao&ciaid='+ciaid+'&linkbensservicoes=1&esfera='+esfera+'&estuf='+estuf+'&muncod='+muncod, 'link', 'width=800height=600,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
}
function linkObjeto(esfera,estuf,muncod,oapid) {
	var janela = window.open( 'pdeinterativo.php?modulo=relatorio/relatoriogeraldimensoes&acao=A&buscar=1&dimensao=planoacao&oapid='+oapid+'&linkobjetos=1&esfera='+esfera+'&estuf='+estuf+'&muncod='+muncod, 'link', 'width=800height=600,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
}
</script>
<?php

$where[] = "p.pdistatus='A'";

if($_REQUEST['intesfera']) {
	$where[] = "p.pdiesfera='".$_REQUEST['intesfera']."'";
}

if($_REQUEST['estuf']) {
	$where[] = "m.estuf='".$_REQUEST['estuf']."'";
}

if($_REQUEST['muncod']) {
	$where[] = "m.muncod='".$_REQUEST['muncod']."'";
}

if($_REQUEST['ciaid']) {
	$where[] = "ci.ciaid='".$_REQUEST['ciaid']."'";
}

if($_REQUEST['oapid']) {
	$where[] = "oa.oapid='".$_REQUEST['oapid']."'";
}

if($_REQUEST['pditempdeescola']) {
	$where[] = "p.pditempdeescola='".$_REQUEST['pditempdeescola']."'";
}

if($_REQUEST['pabfonte']) {
	$where[] = "pb.pabfonte='".$_REQUEST['pabfonte']."'";
}

if($_REQUEST['linkbensservicoes']) {
	$sql = "select p.pdenome, SUM(pb.pabqtd) as qtd, SUM(COALESCE(pb.pabvalorcapital,0)+COALESCE(pb.pabvalorcusteiro,0)) as vlr, ROUND(SUM(COALESCE(pb.pabvalorcapital,0)+COALESCE(pb.pabvalorcusteiro,0))/SUM(pb.pabqtd),2) as media
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.planoacaoproblema pp on pp.pdeid = p.pdeid 
			inner join pdeinterativo.planoacaoestrategia pa on pa.papid = pp.papid 
			inner join pdeinterativo.planoacaoacao aa on aa.paeid = pa.paeid
			inner join pdeinterativo.planoacaobemservico pb on pb.paaid = aa.paaid 
			inner join pdeinterativo.categoriaitemacao ci on ci.ciaid = pb.ciaid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where pdistatus='A' and papstatus='A' and paastatus='A' and pabstatus='A' and ciastatus='A' ".(($where)?"AND ".implode(" AND ",$where):"")."
			GROUP BY p.pdenome 
			ORDER BY p.pdenome";
	$cabecalho = array("Escola","Quantidade","Total (R$)","Valor médio (R$)");
	$db->monta_lista_simples($sql,$cabecalho,1000000,5,'S','100%',$par2);
	echo "<p align=center><input type=button name=fechar value=Fechar onclick=window.close();></p>";
	exit;
}

if($_REQUEST['linkobjetos']) {
	
	$sql = "select p.pdenome, count(paaid) as ocorrencias
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.planoacaoproblema pp on pp.pdeid = p.pdeid 
			inner join pdeinterativo.planoacaoestrategia pa on pa.papid = pp.papid 
			inner join pdeinterativo.planoacaoacao aa on aa.paeid = pa.paeid
			inner join pdeinterativo.objetoapoio oa on oa.oapid = aa.oapid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where pdistatus='A' and papstatus='A' and paastatus='A' and oapstatus='A' ".(($where)?"AND ".implode(" AND ",$where):"")."
			GROUP BY p.pdenome 
			ORDER BY p.pdenome";
	
	$cabecalho = array("Escola","Nº ocorrências");
	$db->monta_lista_simples($sql,$cabecalho,1000000,5,'S','100%',$par2);
	echo "<p align=center><input type=button name=fechar value=Fechar onclick=window.close();></p>";
	exit;
	
}


echo "<h1>Tela Bens e Serviços</h1>";

echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Bens e Serviços</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select '<a style=cursor:pointer; onclick=\"linkBensServicoes(\'".$_REQUEST['intesfera']."\',\'".$_REQUEST['estuf']."\',\'".$_REQUEST['muncod']."\','||ci.ciaid||')\">'||ci.ciadesc||'</a>' as ciadesc, SUM(pb.pabqtd) as qtd, SUM(COALESCE(pb.pabvalorcapital,0)+COALESCE(pb.pabvalorcusteiro,0)) as vlr, ROUND(SUM(COALESCE(pb.pabvalorcapital,0)+COALESCE(pb.pabvalorcusteiro,0))/SUM(pb.pabqtd),2) as media
		from pdeinterativo.pdinterativo p 
		inner join pdeinterativo.planoacaoproblema pp on pp.pdeid = p.pdeid 
		inner join pdeinterativo.planoacaoestrategia pa on pa.papid = pp.papid 
		inner join pdeinterativo.planoacaoacao aa on aa.paeid = pa.paeid
		inner join pdeinterativo.planoacaobemservico pb on pb.paaid = aa.paaid 
		inner join pdeinterativo.categoriaitemacao ci on ci.ciaid = pb.ciaid 
		inner join territorios.municipio m on m.muncod=p.muncod
		where pdistatus='A' and papstatus='A' and paastatus='A' and pabstatus='A' and ciastatus='A' ".(($where)?"AND ".implode(" AND ",$where):"")."
		GROUP BY ci.ciaid, ci.ciadesc 
		ORDER BY ci.ciadesc";

$cabecalho = array("Item","Quantidade","Total (R$)","Valor médio (R$)");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Bens e Serviços</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select ci.ciadesc, MAX(pabvalor) as maximo, MIN(pabvalor) as minimo
		from pdeinterativo.pdinterativo p 
		inner join pdeinterativo.planoacaoproblema pp on pp.pdeid = p.pdeid 
		inner join pdeinterativo.planoacaoestrategia pa on pa.papid = pp.papid 
		inner join pdeinterativo.planoacaoacao aa on aa.paeid = pa.paeid
		inner join pdeinterativo.planoacaobemservico pb on pb.paaid = aa.paaid 
		inner join pdeinterativo.categoriaitemacao ci on ci.ciaid = pb.ciaid 
		inner join territorios.municipio m on m.muncod=p.muncod 
		where pdistatus='A' and papstatus='A' and paastatus='A' and pabstatus='A' and ciastatus='A' ".(($where)?"AND ".implode(" AND ",$where):"")."
		GROUP BY ci.ciadesc 
		ORDER BY ci.ciadesc";

$cabecalho = array("Item","Valor Unitário (Máx)","Valor Unitário (Min)");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";


echo "</table>";

echo "<h1>Tela Ações</h1>";

echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Ações</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select '<a style=cursor:pointer; onclick=\"linkObjeto(\'".$_REQUEST['intesfera']."\',\'".$_REQUEST['estuf']."\',\'".$_REQUEST['muncod']."\','||oa.oapid||')\" >'||oa.oapdesc||'</a>', count(aa.paaid) as ocorrencias
		from pdeinterativo.pdinterativo p 
		inner join pdeinterativo.planoacaoproblema pp on pp.pdeid = p.pdeid 
		inner join pdeinterativo.planoacaoestrategia pa on pa.papid = pp.papid 
		inner join pdeinterativo.planoacaoacao aa on aa.paeid = pa.paeid
		inner join pdeinterativo.planoacaobemservico pb on pb.paaid = aa.paaid 
		inner join pdeinterativo.objetoapoio oa on oa.oapid = aa.oapid 
		inner join territorios.municipio m on m.muncod=p.muncod
		where pdistatus='A' and papstatus='A' and paastatus='A' and oapstatus='A' ".(($where)?"AND ".implode(" AND ",$where):"")."
		GROUP BY oa.oapid, oa.oapdesc 
		ORDER BY oa.oapdesc";

$cabecalho = array("Objeto da ação","Nº ocorrências");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

echo "</table>";

?>