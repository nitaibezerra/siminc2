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

$arrEns = array("I" => "Ensino Fundamental Regular - Séries Iniciais (até a 4ª série)",
			    "F" => "Ensino Fundamental Regular - Séries Finais (5ª a 8ª série)",
				"M" => "Ensino Médio Regular",
				"U" => "Ensino Fundamental");

$_CAMP = array("I"=>"ridinicialum","F"=>"ridfinalum","M"=>"ridmedioum");

$_CAMP2 = array("U" => array("aprovacao"=>"rtrfunaprova","reprovacao"=>"rtrfunreprova","abandono"=>"rtrfunabandono"),
				"M" => array("aprovacao"=>"rtrmedaprova","reprovacao"=>"rtrmedreprova","abandono"=>"rtrmedabandono"));

$_CAMP3 = array("I"=>array("portugues"=>"rpbinicialport","matematica"=>"rpbinicialmat"),"F"=>array("portugues"=>"rpbfinalport","matematica"=>"rpbfinalmat"),"M"=>array("portugues"=>"rbpmedioport","matematica"=>"rbpmediomat"));

echo '<table width="100%" border="0" cellpadding="0" cellspacing="0"  style="border-bottom: 1px solid;">
	  <tr bgcolor="#ffffff"> 
	  <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
	  <td height="20" nowrap>
  	  SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação<br>
	  Ministério da Educação / Secretaria de Educação Básica<br>
	  PDEInterativo<br>
	  1. Indicadores e Taxas
	  </td>
	  <td height="20" align="right">
	  Impresso por: <strong>'.$_SESSION['usunome'].'</strong><br>
	  Hora da Impressão: '.date("d/m/Y").' - '.date("h:i:s").'<br>
	  Filtros aplicados:<br>
	  '.(($_REQUEST['estuf'])?'<b>Estado:</b> '.$db->pegaUm('select estdescricao from territorios.estado where estuf=\''.$_REQUEST['estuf'].'\''):'').'<br> 
	  '.(($_REQUEST['muncod'])?'<b>Município:</b> '.$db->pegaUm('select mundescricao from territorios.municipio where muncod=\''.$_REQUEST['muncod'].'\''):'').'</td>
	  </tr>
	  <tr bgcolor="#ffffff"> 
      <td colspan="2">&nbsp;</td>
	  </tr>
	  </table>';



echo "<br>";

echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";

echo "<tr>";
echo "<td class=SubTituloCentro>".$arrEns[$_REQUEST['intensino']]." - Resumo - IDEB</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select count(pdicodinep) from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaideb rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where);

$totalescolas = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaideb rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP[$_REQUEST['intensino']]."='S'";

$totalsim = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaideb rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP[$_REQUEST['intensino']]."='N'";

$totalnao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaideb rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP[$_REQUEST['intensino']]."='A'";

$totalnaoaplica = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaideb rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP[$_REQUEST['intensino']]." IS NULL";

$totalnaoresponderam = $db->pegaUm($sql);


echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr>";
echo "<td class=SubTituloCentro>Respostas</td>";
echo "<td class=SubTituloCentro>Escolas</td>";
echo "<td class=SubTituloCentro>%</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>IDEB melhorou nas 2 últimas medições? SIM</td>";
echo "<td align=right>".$totalsim."</td>";
echo "<td align=right>".round($totalsim/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>IDEB melhorou nas 2 últimas medições? NÃO</td>";
echo "<td align=right>".$totalnao."</td>";
echo "<td align=right>".round($totalnao/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>IDEB melhorou nas 2 últimas medições? Não se aplica</td>";
echo "<td align=right>".$totalnaoaplica."</td>";
echo "<td align=right>".round($totalnaoaplica/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Não responderam</td>";
echo "<td align=right>".$totalnaoresponderam."</td>";
echo "<td align=right>".round($totalnaoresponderam/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "</table>";


echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>".$arrEns[$_REQUEST['intensino']]." - Perguntas - IDEB</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select pdicodinep,pdenome,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='I' and intensino='".$_REQUEST['intensino']."' and intano='2005' and intstatus='A' and intesfera='S') as v_2005,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='I' and intensino='".$_REQUEST['intensino']."' and intano='2007' and intstatus='A' and intesfera='S') as v_2007,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='I' and intensino='".$_REQUEST['intensino']."' and intano='2009' and intstatus='A' and intesfera='S') as v_2009,
			   CASE WHEN ".$_CAMP[$_REQUEST['intensino']]."='S' THEN '<center>X</center>' ELSE '' END as rsim,
			   CASE WHEN ".$_CAMP[$_REQUEST['intensino']]."='N' THEN '<center>X</center>' ELSE '' END  as rnao,
			   CASE WHEN ".$_CAMP[$_REQUEST['intensino']]."='A' THEN '<center>X</center>' ELSE '' END  as rnaoaplica
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaideb rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." 
		order by p.pdenome";

$cabecalho = array("INEP","Escola","2005 (Resultados)","2007 (Resultados)","2009 (Resultados)","IDEB melhorou nas 2 últimas medições? SIM","IDEB melhorou nas 2 últimas medições? NÃO","IDEB melhorou nas 2 últimas medições? Não se aplica");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

if($_REQUEST['intensino']=="I" || $_REQUEST['intensino']=="F") $_REQUEST['intensino2']="U";
else $_REQUEST['intensino2']="M";

echo "<tr>";
echo "<td class=SubTituloCentro>".$arrEns[$_REQUEST['intensino2']]." - Resumo - Taxas de Rendimentos</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select count(pdicodinep) from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where);

$totalescolas = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['aprovacao']."='S'";

$totalsim_aprovacao = $db->pegaUm($sql);



$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['reprovacao']."='S'";

$totalsim_reprovacao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['abandono']."='S'";

$totalsim_abandono = $db->pegaUm($sql);


$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['aprovacao']."='N'";

$totalnao_aprovacao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['reprovacao']."='N'";

$totalnao_reprovacao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['abandono']."='N'";

$totalnao_abandono = $db->pegaUm($sql);


$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['aprovacao']."='A'";

$totalnaoaplica_aprovacao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['reprovacao']."='A'";

$totalnaoaplica_reprovacao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['abandono']."='A'";

$totalnaoaplica_abandono = $db->pegaUm($sql);


$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['aprovacao']." IS NULL";

$totalnaoresponderam_aprovacao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['reprovacao']." IS NULL";

$totalnaoresponderam_reprovacao = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP2[$_REQUEST['intensino2']]['abandono']." IS NULL";

$totalnaoresponderam_abandono = $db->pegaUm($sql);


echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr>";
echo "<td class=SubTituloCentro>Respostas</td>";
echo "<td class=SubTituloCentro>Escolas</td>";
echo "<td class=SubTituloCentro>%</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>&nbsp;</td>";
echo "<td class=SubTituloCentro colspan=2><b>Taxa de aprovação</b></td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de aprovação da escola vem melhorando nos últimos dois anos? SIM</td>";
echo "<td align=right>".$totalsim_aprovacao."</td>";
echo "<td align=right>".round($totalsim_aprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de aprovação da escola vem melhorando nos últimos dois anos? NÃO</td>";
echo "<td align=right>".$totalnao_aprovacao."</td>";
echo "<td align=right>".round($totalnao_aprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de aprovação da escola vem melhorando nos últimos dois anos? Não se aplica</td>";
echo "<td align=right>".$totalnaoaplica_aprovacao."</td>";
echo "<td align=right>".round($totalnaoaplica_aprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Não responderam a taxa de aprovação</td>";
echo "<td align=right>".$totalnaoresponderam_aprovacao."</td>";
echo "<td align=right>".round($totalnaoresponderam_aprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>&nbsp;</td>";
echo "<td class=SubTituloCentro colspan=2><b>Taxa de reprovação</b></td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de reprovação da escola tem diminuído nos dois últimos anos? SIM</td>";
echo "<td align=right>".$totalsim_reprovacao."</td>";
echo "<td align=right>".round($totalsim_reprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de reprovação da escola tem diminuído nos dois últimos anos? NÃO</td>";
echo "<td align=right>".$totalnao_reprovacao."</td>";
echo "<td align=right>".round($totalnao_reprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de reprovação da escola tem diminuído nos dois últimos anos? Não se aplica</td>";
echo "<td align=right>".$totalnaoaplica_reprovacao."</td>";
echo "<td align=right>".round($totalnaoaplica_reprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Não responderam</td>";
echo "<td align=right>".$totalnaoresponderam_reprovacao."</td>";
echo "<td align=right>".round($totalnaoresponderam_reprovacao/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>&nbsp;</td>";
echo "<td class=SubTituloCentro colspan=2><b>Taxa de abandono</b></td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de abandono tem diminuído nos dois últimos anos? SIM</td>";
echo "<td align=right>".$totalsim_abandono."</td>";
echo "<td align=right>".round($totalsim_abandono/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de abandono tem diminuído nos dois últimos anos? NÃO</td>";
echo "<td align=right>".$totalnao_abandono."</td>";
echo "<td align=right>".round($totalnao_abandono/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Taxa de abandono tem diminuído nos dois últimos anos? Não se aplica</td>";
echo "<td align=right>".$totalnaoaplica_abandono."</td>";
echo "<td align=right>".round($totalnaoaplica_abandono/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Não responderam</td>";
echo "<td align=right>".$totalnaoresponderam_abandono."</td>";
echo "<td align=right>".round($totalnaoresponderam_abandono/$totalescolas*100,1)."</td>";
echo "</tr>";
echo "</table>";



echo "</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloCentro>".$arrEns[$_REQUEST['intensino2']]." - Perguntas - Taxas de Rendimentos</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

echo "<b>Taxa de Aprovação</b><br>";

$sql = "select pdicodinep,pdenome,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2008' and intstatus='A' and intesfera='S' and intaprrepaba='A') as v_2008,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2009' and intstatus='A' and intesfera='S' and intaprrepaba='A') as v_2009,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2010' and intstatus='A' and intesfera='S' and intaprrepaba='A') as v_2010,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['aprovacao']."='S' THEN '<center>X</center>' ELSE '' END as rsim,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['aprovacao']."='N' THEN '<center>X</center>' ELSE '' END  as rnao,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['aprovacao']."='A' THEN '<center>X</center>' ELSE '' END  as rnaoaplica
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rt on rt.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." 
		order by p.pdenome";

$cabecalho = array("INEP","Escola","2008 (Resultados)","2009 (Resultados)","2010 (Resultados)","Taxa de aprovação da escola vem melhorando nos últimos dois anos? SIM","Taxa de aprovação da escola vem melhorando nos últimos dois anos? NÃO","Taxa de aprovação da escola vem melhorando nos últimos dois anos? Não se aplica");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "<br>";
echo "<b>Taxa de Reprovação</b><br>";

$sql = "select pdicodinep,pdenome,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2008' and intstatus='A' and intesfera='S' and intaprrepaba='R') as v_2008,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2009' and intstatus='A' and intesfera='S' and intaprrepaba='R') as v_2009,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2010' and intstatus='A' and intesfera='S' and intaprrepaba='R') as v_2010,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['reprovacao']."='S' THEN '<center>X</center>' ELSE '' END as rsim,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['reprovacao']."='N' THEN '<center>X</center>' ELSE '' END  as rnao,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['reprovacao']."='A' THEN '<center>X</center>' ELSE '' END  as rnaoaplica
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rt on rt.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." 
		order by p.pdenome";

$cabecalho = array("INEP","Escola","2008 (Resultados)","2009 (Resultados)","2010 (Resultados)","Taxa de reprovação da escola tem diminuído nos dois últimos anos? SIM","Taxa de reprovação da escola tem diminuído nos dois últimos anos? NÃO","Taxa de reprovação da escola tem diminuído nos dois últimos anos? Não se aplica");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "<br>";
echo "<b>Taxa de Abandono</b><br>";

$sql = "select pdicodinep,pdenome,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2008' and intstatus='A' and intesfera='S' and intaprrepaba='B') as v_2008,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2009' and intstatus='A' and intesfera='S' and intaprrepaba='B') as v_2009,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='T' and intensino='".$_REQUEST['intensino2']."' and intano='2010' and intstatus='A' and intesfera='S' and intaprrepaba='B') as v_2010,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['abandono']."='S' THEN '<center>X</center>' ELSE '' END as rsim,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['abandono']."='N' THEN '<center>X</center>' ELSE '' END  as rnao,
			   CASE WHEN ".$_CAMP2[$_REQUEST['intensino2']]['abandono']."='A' THEN '<center>X</center>' ELSE '' END  as rnaoaplica
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostataxarendimento rt on rt.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." 
		order by p.pdenome";

$cabecalho = array("INEP","Escola","2008 (Resultados)","2009 (Resultados)","2010 (Resultados)","Taxa de abandono tem diminuído nos dois últimos anos? SIM","Taxa de abandono tem diminuído nos dois últimos anos? NÃO","Taxa de abandono tem diminuído nos dois últimos anos? Não se aplica");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>".$arrEns[$_REQUEST['intensino']]." - Resumo - Prova Brasil</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select count(pdicodinep) from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where);

$totalescolas = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['portugues']."='S'";

$totalsim_portugues = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['matematica']."='S'";

$totalsim_matematica = $db->pegaUm($sql);


$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['portugues']."='N'";

$totalnao_portugues = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['matematica']."='N'";

$totalnao_matematica = $db->pegaUm($sql);


$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['portugues']."='A'";

$totalnaoaplica_portugues = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['matematica']."='A'";

$totalnaoaplica_matematica = $db->pegaUm($sql);


$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['portugues']." IS NULL";

$totalnaoresponderam_portugues = $db->pegaUm($sql);

$sql = "select count(pdicodinep)
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rd on rd.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." AND ".$_CAMP3[$_REQUEST['intensino']]['matematica']." IS NULL";

$totalnaoresponderam_matematica = $db->pegaUm($sql);


echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr>";
echo "<td class=SubTituloCentro>Respostas</td>";
echo "<td class=SubTituloCentro>Escolas</td>";
echo "<td class=SubTituloCentro>%</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>&nbsp;</td>";
echo "<td class=SubTituloCentro colspan=2>Língua portuguesa</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Língua portuguesa na prova brasil demonstram evolução nos últimos anos? SIM</td>";
echo "<td align=right>".$totalsim_portugues."</td>";
echo "<td align=right>".round($totalsim_portugues/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Língua portuguesa na prova brasil demonstram evolução nos últimos anos? NÃO</td>";
echo "<td align=right>".$totalnao_portugues."</td>";
echo "<td align=right>".round($totalnao_portugues/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Língua portuguesa na prova brasil demonstram evolução nos últimos anos? Não se aplica</td>";
echo "<td align=right>".$totalnaoaplica_portugues."</td>";
echo "<td align=right>".round($totalnaoaplica_portugues/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Não responderam</td>";
echo "<td align=right>".$totalnaoresponderam_portugues."</td>";
echo "<td align=right>".round($totalnaoresponderam_portugues/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>&nbsp;</td>";
echo "<td class=SubTituloCentro colspan=2>Matemática</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Matemática na prova brasil demonstram evolução nos últimos anos? SIM</td>";
echo "<td align=right>".$totalsim_matematica."</td>";
echo "<td align=right>".round($totalsim_matematica/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Matemática na prova brasil demonstram evolução nos últimos anos? NÃO</td>";
echo "<td align=right>".$totalnao_matematica."</td>";
echo "<td align=right>".round($totalnao_matematica/$totalescolas*100,1)."</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloDireita>Matemática na prova brasil demonstram evolução nos últimos anos? Não se aplica</td>";
echo "<td align=right>".$totalnaoaplica_matematica."</td>";
echo "<td align=right>".round($totalnaoaplica_matematica/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloDireita>Não responderam</td>";
echo "<td align=right>".$totalnaoresponderam_matematica."</td>";
echo "<td align=right>".round($totalnaoresponderam_matematica/$totalescolas*100,1)."</td>";
echo "</tr>";


echo "</table>";


echo "</td>";
echo "</tr>";




echo "<tr>";
echo "<td class=SubTituloCentro>".$arrEns[$_REQUEST['intensino']]." - Perguntas - Prova Brasil</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

echo "<b>Língua Portuguesa</b><br>";

$sql = "select pdicodinep,pdenome,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='P' and intensino='".$_REQUEST['intensino']."' and intano='2005' and intstatus='A' and intesfera='S' and intpormat='P') as v_2005,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='P' and intensino='".$_REQUEST['intensino']."' and intano='2007' and intstatus='A' and intesfera='S' and intpormat='P') as v_2007,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='P' and intensino='".$_REQUEST['intensino']."' and intano='2009' and intstatus='A' and intesfera='S' and intpormat='P') as v_2009,
			   CASE WHEN ".$_CAMP3[$_REQUEST['intensino']]['portugues']."='S' THEN '<center>X</center>' ELSE '' END as rsim,
			   CASE WHEN ".$_CAMP3[$_REQUEST['intensino']]['portugues']."='N' THEN '<center>X</center>' ELSE '' END  as rnao,
			   CASE WHEN ".$_CAMP3[$_REQUEST['intensino']]['portugues']."='A' THEN '<center>X</center>' ELSE '' END  as rnaoaplica
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rt on rt.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." 
		order by p.pdenome";

$cabecalho = array("INEP","Escola","2005 (Resultados)","2007 (Resultados)","2009 (Resultados)","Língua portuguesa na prova brasil demonstram evolução nos últimos anos? SIM","Língua portuguesa na prova brasil demonstram evolução nos últimos anos? NÃO","Língua portuguesa na prova brasil demonstram evolução nos últimos anos? Não se aplica");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "<br>";
echo "<b>Matemática</b><br>";

$sql = "select pdicodinep,pdenome,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='P' and intensino='".$_REQUEST['intensino']."' and intano='2005' and intstatus='A' and intesfera='S' and intpormat='M') as v_2005,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='P' and intensino='".$_REQUEST['intensino']."' and intano='2007' and intstatus='A' and intesfera='S' and intpormat='M') as v_2007,
			   (select intvalor from pdeinterativo.indicadorestaxas where intinep=p.pdicodinep::numeric and intsubmodulo='P' and intensino='".$_REQUEST['intensino']."' and intano='2009' and intstatus='A' and intesfera='S' and intpormat='M') as v_2009,
			   CASE WHEN ".$_CAMP3[$_REQUEST['intensino']]['matematica']."='S' THEN '<center>X</center>' ELSE '' END as rsim,
			   CASE WHEN ".$_CAMP3[$_REQUEST['intensino']]['matematica']."='N' THEN '<center>X</center>' ELSE '' END  as rnao,
			   CASE WHEN ".$_CAMP3[$_REQUEST['intensino']]['matematica']."='A' THEN '<center>X</center>' ELSE '' END  as rnaoaplica
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprovabrasil rt on rt.pdeid=p.pdeid 
		where ".implode(" AND ",$where)." 
		order by p.pdenome";

$cabecalho = array("INEP","Escola","2005 (Resultados)","2007 (Resultados)","2009 (Resultados)","Matemática na prova brasil demonstram evolução nos últimos anos? SIM","Matemática na prova brasil demonstram evolução nos últimos anos? NÃO","Matemática na prova brasil demonstram evolução nos últimos anos? Não se aplica");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";




echo "</table>";


?>