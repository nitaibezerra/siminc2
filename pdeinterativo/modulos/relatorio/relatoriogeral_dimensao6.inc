<?php

$where[] = "p.pdistatus='A'";
$where2[] = "p.pdistatus='A'";

if($_REQUEST['intesfera']) {
	$where[] = "p.pdiesfera='".$_REQUEST['intesfera']."'";
	$where2[] = "p.pdiesfera='".$_REQUEST['intesfera']."'";
}

if($_REQUEST['estuf']) {
	$where[] = "m.estuf='".$_REQUEST['estuf']."'";
	$where2[] = "m.estuf='".$_REQUEST['estuf']."'";
}

if($_REQUEST['muncod']) {
	$where[] = "m.muncod='".$_REQUEST['muncod']."'";
	$where2[] = "m.muncod='".$_REQUEST['muncod']."'";
}

if($_REQUEST['prgsubmodulo']) {
	$where[] = "pe.prgsubmodulo='".$_REQUEST['prgsubmodulo']."'";
}

if($_REQUEST['prgdetalhe']) {
	$where[] = "pe.prgdetalhe='".$_REQUEST['prgdetalhe']."'";
}

if($_REQUEST['prgid']) {
	foreach($_REQUEST['prgid'] as $prgid) {
		$pp[] = $prgid;
	}
	$where[] = "pe.prgid in('".implode("','",$pp)."')";
}

echo '<table width="100%" border="0" cellpadding="0" cellspacing="0"  style="border-bottom: 1px solid;">
	  <tr bgcolor="#ffffff"> 
	  <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
	  <td height="20" nowrap>
  	  SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação<br>
	  Ministério da Educação / Secretaria de Educação Básica<br>
	  PDEInterativo<br>
	  6. Infraestrutura
	  </td>
	  <td height="20" align="right">
	  Impresso por: <strong>'.$_SESSION['usunome'].'</strong><br>
	  Hora da Impressão: '.date("d/m/Y").' - '.date("h:i:s").'<br>
	  <i>Filtros aplicados:</i><br>
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
echo "<td class=SubTituloCentro>Resumo das Perguntas</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='I' ".(($where)?" AND ".implode(" AND ",$where):"")."";
$totalescolas = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='I' and r.oppid=".OPP_SEMPRE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_sempre = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='I' and r.oppid=".OPP_MAIORIA_DAS_VEZES." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_maioriadasvezes = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='I' and r.oppid=".OPP_RARAMENTE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_raramente = $db->pegaUm($sql);
$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='I' and r.oppid=".OPP_NUNCA." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_nunca = $db->pegaUm($sql);



echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr><td class=SubTituloDireita>Sempre</td><td>".$total_sempre." / ".round(($total_sempre/$totalescolas)*100,1)."%</td></tr>";
echo "<tr><td class=SubTituloDireita>A maioria das vezes</td><td>".$total_maioriadasvezes." / ".round(($total_maioriadasvezes/$totalescolas)*100,1)."%</td></tr>";
echo "<tr><td class=SubTituloDireita>Raramente</td><td>".$total_raramente." / ".round(($total_raramente/$totalescolas)*100,1)."%</td></tr>";
echo "<tr><td class=SubTituloDireita>Nunca</td><td>".(($total_nunca)?$total_nunca:"0")." / ".round(($total_nunca/$totalescolas)*100,1)."%</td></tr>";
echo "</table>";

echo "</td>";
echo "</tr>";



echo "<tr>";
echo "<td class=SubTituloCentro>Perguntas</td>";
echo "</tr>";
		
echo "<tr>";
echo "<td>";
$sql = "select CASE WHEN prgsubmodulo='I' THEN '6.1. Instalações'
					WHEN prgsubmodulo='E' THEN '6.2. Equipamentos' END as submodulo, 
			   pdicodinep, pdenome, prgdetalhe, prgdesc, 
				case when r.oppid=".OPP_SEMPRE." then '<center>X</center>' else '' end as sempre,
				case when r.oppid=".OPP_MAIORIA_DAS_VEZES." then '<center>X</center>' else '' end as maioria,
				case when r.oppid=".OPP_RARAMENTE." then '<center>X</center>' else '' end as raramente,
				case when r.oppid=".OPP_NUNCA." then '<center>X</center>' else '' end as nunca 
				from pdeinterativo.pdinterativo p 
				inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
				inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
				inner join territorios.municipio m on m.muncod=p.muncod
				where prgmodulo='I' ".(($where)?" AND ".implode(" AND ",$where):"")." 
				order by submodulo, pdenome, prgdetalhe, prgdesc";
		
$cabecalho = array("Subdimensão","INEP","Escola","Tema","Pergunta","Sempre","A maioria das vezes","Raramente","Nunca");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);
		
echo "</td>";
echo "</tr>";
		
echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Instalações - Adequação : ADEQUADO</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";

$sql = "select 
				* 
			from 
				pdeinterativo.infrainstalacaofisica 
			where 
				ifistatus = 'A' 
			order 
				by ifidesc";

$infra = $db->carregar($sql);

$cabecalho_a[] = "INEP";
$cabecalho_a[] = "Escola";

if($infra[0]) {
	foreach($infra as $i) {
		$sql_c[] = "(select rifqtdadequado from pdeinterativo.respostainfrainstalacaofisica where rifstatus = 'A' and pdeid=p.pdeid and ifiid=".$i['ifiid'].") as c".$i['ifiid'];
		$sql_d[] = "(select rifqtdinadequado from pdeinterativo.respostainfrainstalacaofisica where rifstatus = 'A' and pdeid=p.pdeid and ifiid=".$i['ifiid'].") as c".$i['ifiid'];
		$cabecalho_a[] = $i['ifidesc'];
	}
}

$sql = "select pdicodinep, pdenome,
		".implode(",",$sql_c)." 
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join (select pdeid from pdeinterativo.respostainfrainstalacaofisica where rifstatus = 'A' group by pdeid) foo on foo.pdeid = p.pdeid 
		where pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"");

$db->monta_lista_simples($sql,$cabecalho_a,1000000,5,'N','100%',$par2);



echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Instalações - Adequação : INADEQUADO</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";

$sql = "select pdicodinep, pdenome,
		".implode(",",$sql_d)." 
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join (select pdeid from pdeinterativo.respostainfrainstalacaofisica where rifstatus = 'A' group by pdeid) foo on foo.pdeid = p.pdeid 
		where pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"");

$db->monta_lista_simples($sql,$cabecalho_a,1000000,5,'N','100%',$par2);


echo "</td>";
echo "</tr>";


echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Instalações - Espaços SIM</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";

$sql = "select 
				* 
			from 
				pdeinterativo.tipoinstalacaoespaco 
			where 
				tiestatus = 'A' 
			order 
				by tiedesc";
$tie = $db->carregar($sql);

$cabecalho_b[] = "INEP";
$cabecalho_b[] = "Escola";

if($tie[0]) {
	foreach($tie as $f) {
		$sql_e[] = "(select CASE WHEN tieid IS NULL THEN '&nbsp;' ELSE '<center>X</center>' END as resp from pdeinterativo.respostainstalacaoespaco where riestatus = 'A' and pdeid=p.pdeid and tieid=".$f['tieid']." group by tieid) as c".$f['tieid'];
		$cabecalho_b[] = $f['tiedesc'];
	}
}


$sql = "select pdicodinep, pdenome,
		".implode(",",$sql_e)." 
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprojetoprograma r on r.pdeid=p.pdeid 
		where p.pdistatus='A' and rppresposta=true and rpptipo = 'E' and rppstatus = 'A' and rppmodulo = 'F' ".(($where2)?"AND ".implode(" AND ",$where2):"");

$db->monta_lista_simples($sql,$cabecalho_b,1000000,5,'N','100%',$par2);


echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Instalações - Espaços NÃO</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";

$sql = "select pdicodinep, pdenome
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join pdeinterativo.respostaprojetoprograma r on r.pdeid=p.pdeid 
		where p.pdistatus='A' and rppresposta=false and rpptipo = 'E' and rppstatus = 'A' and rppmodulo = 'F' ".(($where2)?"AND ".implode(" AND ",$where2):"");

$db->monta_lista_simples($sql,array("INEP","Escola"),1000000,5,'N','100%',$par2);


echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Equipamentos</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";

if($_REQUEST['tmeid'][0]) $w .= " AND tmeid IN('".implode("','",$_REQUEST['tmeid'])."')";
$sql = "select * from pdeinterativo.tipomaterialequipamento where tmestatus = 'A'".$w." order by tmeid";
$tie = $db->carregar($sql);

$cabecalho_c[] = "INEP";
$cabecalho_c[] = "Escola";

if($tie[0]) {
	foreach($tie as $f) {
		$sql_f[] = "(select rmeqtdbom from pdeinterativo.respostamaterialequipamento where rmestatus = 'A' and pdeid=p.pdeid and tmeid=".$f['tmeid'].") as cb".$f['tmeid'];
		$cabecalho_c[] = $f['tmedesc']." (Bom)";
		$sql_f[] = "(select rmeqtdtotal from pdeinterativo.respostamaterialequipamento where rmestatus = 'A' and pdeid=p.pdeid and tmeid=".$f['tmeid'].") as ct".$f['tmeid'];
		$cabecalho_c[] = $f['tmedesc']." (Total)";
		$sql_f[] = "(select rmeqtdideal from pdeinterativo.respostamaterialequipamento where rmestatus = 'A' and pdeid=p.pdeid and tmeid=".$f['tmeid'].") as ci".$f['tmeid'];
		$cabecalho_c[] = $f['tmedesc']." (Ideal)";
		
	}
}


$sql = "select pdicodinep, pdenome,
		".implode(",",$sql_f)." 
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		inner join (select pdeid from pdeinterativo.respostamaterialequipamento where rmestatus='A' group by pdeid) foo on foo.pdeid=p.pdeid 
		where pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"");

$db->monta_lista_simples($sql,$cabecalho_c,1000000,5,'N','100%',$par2);


echo "</td>";
echo "</tr>";


echo "</table>";


?>