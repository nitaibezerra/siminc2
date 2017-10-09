<?php

if($_REQUEST['xls'] == 1){
	ini_set("memory_limit", "1024M");
	set_time_limit(30000);
	
	header("Pragma: no-cache");
	header("Content-type: application/xls; name=PDE_dimensao4.xls");
	header("Content-Disposition: attachment; filename=PDE_dimensao4.xls");
}

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

if($_REQUEST['equipegestora']=="naoexiste_equipegestora") {
	$where2[] = "fl.naoexisteequipe=TRUE";
}

if($_REQUEST['equipegestora']=="naoexiste_equipegestora") {
	$where2[] = "fl.naoexisteequipe=TRUE";
}

if($_REQUEST['equipegestora']=="naorespondeu_equipegestora") {
	$where3[] = "foo.numequipe=0";
	$where2[] = "(fl.naoexisteequipe=FALSE OR fl.naoexisteequipe IS NULL)";
}

if($_REQUEST['xls'] == 0){
	echo '<table width="100%" border="0" cellpadding="0" cellspacing="0"  style="border-bottom: 1px solid;">
		  <tr bgcolor="#ffffff"> 
		  <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
		  <td height="20" nowrap>
	  	  '. NOME_SISTEMA. '<br>
		  Ministério da Educação / Secretaria de Educação Básica<br>
		  PDEInterativo<br>
		  4. Gestão
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
}

echo "<br>";

echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";

if($_REQUEST['prgdetalhe']=="equipegestora" || !$_REQUEST['prgdetalhe']) { 

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Equipe Gestora - Perfis</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select pdicodinep, pdenome, tpedesc, pesnome, dpeemail, dpetelefone, tendesc, tempo, forma, direscolagestor, dircursodistan  from (
		select tp.tpeid, p.pdicodinep, p.pdenome, tp.tpedesc, pe.pesnome, dp.dpeemail, dp.dpetelefone, te.tendesc, COALESCE(di.dirqtdanoexerce, 0)||' ano(s) - '||COALESCE(di.dirqtdmesesexerce,0)||' mes(es)' as tempo,
		CASE WHEN dirformescolha='M' then 'Misto'
		     WHEN dirformescolha='O' then 'Outro'
		     WHEN dirformescolha='E' then 'Eleição'
		     WHEN dirformescolha='C' then 'Concurso'
		     WHEN dirformescolha='I' then 'Indicação' END as forma,
		direscolagestor,
		dircursodistan,
		(select count(*) from pdeinterativo.pessoa pp 
		 inner join pdeinterativo.pessoatipoperfil ptp on pe.pesid = pp.pesid  
		 where ptp.pdeid=p.pdeid and ptp.tpeid in(1,6,9,10,11) and pp.pesstatus='A') as numequipe,
		m.estuf, m.mundescricao, p.pdiesfera
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod
		left JOIN pdeinterativo.pessoatipoperfil pt ON pt.pdeid = p.pdeid 
		left join pdeinterativo.pessoa pe on pe.pesid = pt.pesid 
		left JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = pe.pesid 
		left JOIN pdeinterativo.tipoperfil tp ON tp.tpeid = pt.tpeid 
		left JOIN pdeinterativo.tipoescolaridade te ON te.tenid = dp.tenid 
		left JOIN pdeinterativo.direcao di ON di.pesid = pe.pesid 
		left JOIN pdeinterativo.flag fl ON fl.pdeid = p.pdeid 
		where ( tp.tpeid is null or tp.tpeid in(2,7,8,1,6,9,10,11)) and p.pdistatus='A' ".(($_REQUEST['tpeid'])?"AND tp.tpeid in('".implode("','",$_REQUEST['tpeid'])."')":"")." ".(($where2)?"AND ".implode(" AND ",$where2):"")." ) foo 
		".(($where3)?"WHERE ".implode(" AND ",$where3):"")."order by pdenome, tpedesc";
		
		  
		
//Atendendo requisito de colunas adicionais quando o usuario selecionar alguns perfis 
if ($_REQUEST['tpeid'] && $_REQUEST['xls'] == 1)
	$cabecalho = array("INEP","Escola","Perfil","Nome","E-mail","Telefone","Escolaridade","Tempo que exerce função","Forma de escolha","Conhece Escola Gestores","Deseja participar de curso", "UF", "Município", "Rede");
else
	$cabecalho = array("INEP","Escola","Perfil","Nome","E-mail","Telefone","Escolaridade","Tempo que exerce função","Forma de escolha","Conhece Escola Gestores","Deseja participar de curso");


//ver($sql,d);
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

}

if($_REQUEST['prgdetalhe']!="equipegestora") {

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
			where prgmodulo='G' ".(($where)?" AND ".implode(" AND ",$where):"")."";
$totalescolas = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='G' and r.oppid=".OPP_SEMPRE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_sempre = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='G' and r.oppid=".OPP_MAIORIA_DAS_VEZES." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_maioriadasvezes = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='G' and r.oppid=".OPP_RARAMENTE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_raramente = $db->pegaUm($sql);
$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='G' and r.oppid=".OPP_NUNCA." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_nunca = $db->pegaUm($sql);



echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr><td class=SubTituloDireita>Sempre</td><td>".$total_sempre." / ".(($totalescolas)?round(($total_sempre/$totalescolas)*100,1):"0")."%</td></tr>";
echo "<tr><td class=SubTituloDireita>A maioria das vezes</td><td>".$total_maioriadasvezes." / ".(($totalescolas)?round(($total_maioriadasvezes/$totalescolas)*100,1):"0")."%</td></tr>";
echo "<tr><td class=SubTituloDireita>Raramente</td><td>".$total_raramente." / ".(($totalescolas)?round(($total_raramente/$totalescolas)*100,1):"0")."%</td></tr>";
echo "<tr><td class=SubTituloDireita>Nunca</td><td>".(($total_nunca)?$total_nunca:"0")." / ".(($totalescolas)?round(($total_nunca/$totalescolas)*100,1):"0")."%</td></tr>";
echo "</table>";

echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Detalhamento das Perguntas</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";
	
$sql = "select 
			CASE WHEN prgsubmodulo='D' THEN '4.1. Direção'
				 WHEN prgsubmodulo='P' THEN '4.2. Processos'
				 WHEN prgsubmodulo='F' THEN '4.3. Finanças'
				 else prgsubmodulo END as submodulo, 
			pdicodinep, pdenome, prgdetalhe, prgdesc, 
			case when r.oppid=".OPP_SEMPRE." then '<center>X</center>' else '' end as sempre,
			case when r.oppid=".OPP_MAIORIA_DAS_VEZES." then '<center>X</center>' else '' end as maioria,
			case when r.oppid=".OPP_RARAMENTE." then '<center>X</center>' else '' end as raramente,
			case when r.oppid=".OPP_NUNCA." then '<center>X</center>' else '' end as nunca 
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='G' ".(($where)?" AND ".implode(" AND ",$where):"")."
			order by submodulo, pdenome, prgdetalhe, prgdesc";
	
$cabecalho = array("Subdimensão","INEP","Escola","Tema","Pergunta","Sempre","A maioria das vezes","Raramente","Nunca");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);
	
echo "</td>";
echo "</tr>";

}
		
echo "</table>";


?>