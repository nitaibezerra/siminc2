<?php

if($_REQUEST['xls'] == 1){
	ini_set("memory_limit", "1024M");
	set_time_limit(30000);

	header("Pragma: no-cache");
	header("Content-type: application/xls; name=PDE_dimensao5.xls");
	header("Content-Disposition: attachment; filename=PDE_dimensao5.xls");
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



if($_REQUEST['xls'] == 0){

echo '<table width="100%" border="0" cellpadding="0" cellspacing="0"  style="border-bottom: 1px solid;">
	  <tr bgcolor="#ffffff"> 
	  <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
	  <td height="20" nowrap>
  	  '. NOME_SISTEMA. '<br>
	  Ministério da Educação / Secretaria de Educação Básica<br>
	  PDEInterativo<br>
	  5. Comunidade escolar
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

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='C' ".(($where)?" AND ".implode(" AND ",$where):"")."";
$totalescolas = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='C' and r.oppid=".OPP_SEMPRE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_sempre = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='C' and r.oppid=".OPP_MAIORIA_DAS_VEZES." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_maioriadasvezes = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='C' and r.oppid=".OPP_RARAMENTE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_raramente = $db->pegaUm($sql);
$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='C' and r.oppid=".OPP_NUNCA." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_nunca = $db->pegaUm($sql);

echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";

if ($totalescolas > 0)
{

echo "<tr>";
echo "<td class=SubTituloCentro>Resumo das Perguntas</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";
	
echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
echo "<tr><td class=SubTituloDireita>Sempre</td><td>".$total_sempre." / ".round(($total_sempre/$totalescolas)*100,1)."%</td></tr>";
echo "<tr><td class=SubTituloDireita>A maioria das vezes</td><td>".$total_maioriadasvezes." / ".round(($total_maioriadasvezes/$totalescolas)*100,1)."%</td></tr>";
echo "<tr><td class=SubTituloDireita>Raramente</td><td>".$total_raramente." / ".round(($total_raramente/$totalescolas)*100,1)."%</td></tr>";
echo "<tr><td class=SubTituloDireita>Nunca</td><td>".(($total_nunca)?$total_nunca:"0")." / ".round(($total_nunca/$totalescolas)*100,1)."%</td></tr>";
echo "</table>";


echo "</td>";
echo "</tr>";
}


//--------------------------------------------------
if (  $_REQUEST['prgsubmodulo'] == '' || $_REQUEST['prgdetalhe'] == 'Colegiado' ||  ( $_REQUEST['prgsubmodulo'] == 'C' && $_REQUEST['prgdetalhe'] == 'perfil' ) )
{
echo "<tr>";
echo "<td class=SubTituloCentro>Conselho</td>";
echo "</tr>";


echo "<tr>";
echo "<td>";

$sql = "select  pdicodinep, pdenome,  
case when  r.rpcpossuiconcelho  then 'Sim' else 'Não' end  as rpcpossuiconcelho, 
case when r.rpcperiodicidade='E' then 'Semanal' 
when r.rpcperiodicidade='M' then 'Mensal' 
when r.rpcperiodicidade='B' then 'Bimestral' 
when r.rpcperiodicidade='T' then 'Trimestral' 
when r.rpcperiodicidade='S' then 'Semestral' 
when r.rpcperiodicidade='A' then 'Anual' else 'Outra' end as rpcperiodicidade,
case when  r.rpcunidadeexecutora = 'S' then 'Sim' when r.rpcunidadeexecutora = 'N' then 'Não' end  as rpcunidadeexecutora
from pdeinterativo.pdinterativo p 
inner join pdeinterativo.respostapaiscomunidade r on p.pdeid = r.pdeid 
inner join territorios.municipio m on m.muncod=p.muncod
where rpcstatus = 'A' ".(($where)?" AND ".implode(" AND ",$where):"")."order by pdenome";

$cabecalho = array("INEP","Escola","Possui Conselho Escolar","Periodicidade do Conselho", "Conselho Unidade Gestora");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);


//-----------------------

echo "<tr>";
echo "<td class=SubTituloCentro>Membros do Conselho</td>";
echo "</tr>";


echo "<tr>";
echo "<td>";

$sql = "select pdicodinep, pdenome, pesnome, usucpf, SUBSTR(dpetelefone,1,2) ||'-'|| SUBSTR(dpetelefone,3,8) as dpetelefone,dpeemail, tendesc from 
pdeinterativo.membroconselho me
inner join pdeinterativo.pdinterativo p on p.pdeid = me.pdeid
inner join  pdeinterativo.pessoa pe on me.pesid = pe.pesid 
LEFT JOIN pdeinterativo.detalhepessoa d ON d.pesid = pe.pesid 
left join pdeinterativo.tipoescolaridade t on t.tenid = d.tenid
inner join territorios.municipio m on m.muncod=p.muncod
where mcestatus = 'A' ".(($where)?" AND ".implode(" AND ",$where):"")."order by pdenome, me.pdeid, pesnome";

$cabecalho = array("INEP","Escola", "Nome","CPF","Telefone","E-Mail","Formação");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

//-----------------


}


if($_REQUEST['prgsubmodulo']) {
	//solicitação de aparecer os dados de colegiados na dimensao 5.4 Demais profissionais (módulo c)
	if ($_REQUEST['prgsubmodulo'] == 'C' && $_REQUEST['prgdetalhe'] == 'Colegiado' )
		$where[] = "pe.prgsubmodulo='P'";
	else
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

if ( $_REQUEST['prgdetalhe'] != 'perfil')
{
echo "<tr>";
echo "<td class=SubTituloCentro>Detalhamento Perguntas</td>";
echo "</tr>";


echo "<tr>";
echo "<td>";


//ver ($_REQUEST['prgdetalhe'],d); 	
$sql = "select CASE WHEN prgsubmodulo='D' THEN '5.2. Docentes'
				 	WHEN prgsubmodulo='E' THEN '5.1. Estudantes'
				 	WHEN prgsubmodulo='P' THEN '5.3. Demais Profissionais' 
				 	WHEN prgsubmodulo='C' THEN '5.4. Comunidade e Pais'
				 	 END as submodulo,
				 pdicodinep, pdenome, prgdetalhe, prgdesc, 
			case when r.oppid=".OPP_SEMPRE." then '<center>X</center>' else '' end as sempre,
			case when r.oppid=".OPP_MAIORIA_DAS_VEZES." then '<center>X</center>' else '' end as maioria,
			case when r.oppid=".OPP_RARAMENTE." then '<center>X</center>' else '' end as raramente,
			case when r.oppid=".OPP_NUNCA." then '<center>X</center>' else '' end as nunca 
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='C' ".(($where)?" AND ".implode(" AND ",$where):"")." 
			order by submodulo, pdenome, prgdetalhe, prgdesc";
	
$cabecalho = array("Subdimensão","INEP","Escola","Tema","Pergunta","Sempre","A maioria das vezes","Raramente","Nunca");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);
	
echo "</td>";
echo "</tr>";
}

if (  $_REQUEST['prgsubmodulo'] == '' ||  ($_REQUEST['prgsubmodulo'] == 'D' && $_REQUEST['prgdetalhe'] == ''  ) || ($_REQUEST['prgsubmodulo'] == 'D' && $_REQUEST['prgdetalhe'] == 'perfil'  ) )
{

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Docentes - Perfis</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";


$sql = "select p.pdicodinep, p.pdenome, td.no_docente, CASE WHEN rd.rdovinculo='T' THEN 'Temporário' WHEN rd.rdovinculo='E' THEN 'Efetivo' WHEN rd.rdovinculo='D' THEN 'Desvinculado da escola' ELSE 'Em branco' END as rdovinculo, e.no_escolaridade, CASE WHEN rdoformapro=TRUE THEN 'Sim' WHEN rdoformapro=FALSE THEN 'Não' ELSE 'Em branco' END as rdoformapro, 'ddd' as curso 
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		left join pdeinterativo.respostadocente rd on rd.pdeid=p.pdeid  
		left join educacenso_2010.tab_docente td on rd.pk_cod_docente=td.pk_cod_docente 
		left join  educacenso_2010.tab_escolaridade e ON e.pk_cod_escolaridade = td.fk_cod_escolaridade
		where (rdostatus='A' or rdostatus is null)  ".(($where2)?"AND ".implode(" AND ",$where2):"")."
		order by pdenome, no_docente";

$cabecalho = array("INEP","Escola","Nome","Vínculo","Escolaridade","Deseja participar de curso","Curso desejado");

$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";
}

if (  $_REQUEST['prgsubmodulo'] == '' ||  ($_REQUEST['prgsubmodulo'] == 'P' && $_REQUEST['prgdetalhe'] == ''  ) || ($_REQUEST['prgsubmodulo'] == 'P' && $_REQUEST['prgdetalhe'] == 'perfil'  ) )
{

	echo "<tr>";
	echo "<td class=SubTituloCentro>Relatório Demais Profissionais - Perfis</td>";
	echo "</tr>";

	echo "<tr>";
	echo "<td>";


	$sql = "select p.pdicodinep, p.pdenome, pesnome, usucpf , tendesc 
		from pdeinterativo.pdinterativo p 
		inner join territorios.municipio m on m.muncod=p.muncod 
		left join pdeinterativo.demaisprofissionais d on p.pdeid = d.pdeid
		left join pdeinterativo.pessoa pe on pe.pesid = d.pesid 
		left JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = d.pesid 
		left JOIN pdeinterativo.tipoescolaridade te ON te.tenid = dp.tenid 
		where (d.dpestatus = 'A' or d.dpestatus is null) and p.pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"")."
		order by pdicodinep, pdenome, pesnome";
		
	$cabecalho = array("INEP","Escola","Nome","CPF","Escolaridade");

	$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

	echo "</td>";
	echo "</tr>";
}
echo "</table>";
?>