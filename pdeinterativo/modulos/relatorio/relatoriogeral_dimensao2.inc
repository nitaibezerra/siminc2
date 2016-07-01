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
	  2. Distorção e aproveitamento
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
			where prgmodulo='D' ".(($where)?" AND ".implode(" AND ",$where):"")."";
$totalescolas = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='D' and r.oppid=".OPP_SEMPRE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_sempre = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='D' and r.oppid=".OPP_MAIORIA_DAS_VEZES." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_maioriadasvezes = $db->pegaUm($sql);

$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='D' and r.oppid=".OPP_RARAMENTE." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_raramente = $db->pegaUm($sql);
$sql = "select count(pdicodinep) as total
			from pdeinterativo.pdinterativo p 
			inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
			inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
			inner join territorios.municipio m on m.muncod=p.muncod
			where prgmodulo='D' and r.oppid=".OPP_NUNCA." ".(($where)?" AND ".implode(" AND ",$where):"")."";
$total_nunca = $db->pegaUm($sql);


echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
if($totalescolas) {
	echo "<tr><td class=SubTituloDireita>Sempre</td><td>".$total_sempre." / ".round(($total_sempre/$totalescolas)*100,1)."%</td></tr>";
	echo "<tr><td class=SubTituloDireita>A maioria das vezes</td><td>".$total_maioriadasvezes." / ".round(($total_maioriadasvezes/$totalescolas)*100,1)."%</td></tr>";
	echo "<tr><td class=SubTituloDireita>Raramente</td><td>".$total_raramente." / ".round(($total_raramente/$totalescolas)*100,1)."%</td></tr>";
	echo "<tr><td class=SubTituloDireita>Nunca</td><td>".(($total_nunca)?$total_nunca:"0")." / ".round(($total_nunca/$totalescolas)*100,1)."%</td></tr>";
} else {
	echo "<tr><td class=SubTituloEsquerda>Nenhuma escola identificada com os filtros passados.</td></tr>";
}
echo "</table>";

echo "</td>";
echo "</tr>";



echo "<tr>";
echo "<td class=SubTituloCentro>Perguntas</td>";
echo "</tr>";
		
echo "<tr>";
echo "<td>";
$sql = "select CASE WHEN prgsubmodulo='D' THEN '2.2. Distorção idade-série'
					WHEN prgsubmodulo='A' THEN '2.3. Aproveitamento escolar'
					WHEN prgsubmodulo='C' THEN '2.4. Aproveitamento escolar' END as submodulo, 
			   pdicodinep, pdenome, prgdetalhe, prgdesc, 
				case when r.oppid=".OPP_SEMPRE." then '<center>X</center>' else '' end as sempre,
				case when r.oppid=".OPP_MAIORIA_DAS_VEZES." then '<center>X</center>' else '' end as maioria,
				case when r.oppid=".OPP_RARAMENTE." then '<center>X</center>' else '' end as raramente,
				case when r.oppid=".OPP_NUNCA." then '<center>X</center>' else '' end as nunca 
				from pdeinterativo.pdinterativo p 
				inner join pdeinterativo.respostapergunta r on r.pdeid=p.pdeid 
				inner join pdeinterativo.pergunta pe on pe.prgid=r.prgid 
				inner join territorios.municipio m on m.muncod=p.muncod
				where prgmodulo='D' ".(($where)?" AND ".implode(" AND ",$where):"")." 
				order by submodulo, pdenome, prgdetalhe, prgdesc";
		
$cabecalho = array("Subdimensão","INEP","Escola","Tema","Pergunta","Sempre","A maioria das vezes","Raramente","Nunca");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);
		
echo "</td>";
echo "</tr>";
		
echo "<tr>";
echo "<td class=SubTituloCentro>Relatório - Matrículas</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";

$sql = "select *,
               case when num_turmas > 0
                               then (num_turmas_acima_cne*100)/num_turmas 
                               else 0
                end as porcentagem
		from
                (select
                			   pdicodinep as inep,
                               pdenome as escola,
                               (select distinct count(pk_cod_turma) from educacenso_2010.tab_turma turma where fk_cod_entidade = p.pdicodinep::bigint) as num_turmas,
                               (select distinct count(fk_cod_turma) from pdeinterativo.distorcaoaproveitamento turma2 where pdeid = p.pdeid and diasubmodulo = 'M' and diastatus = 'A') as num_turmas_acima_cne
                from pdeinterativo.pdinterativo p 
				inner join territorios.municipio m on m.muncod=p.muncod
                where pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"")."
                ) as tbl;";
	
$cabecalho = array("INEP","Escola","Nº Total Turmas","Nº Turmas acima Parâmetro","% Turmas acima Parâmetros CNE");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Distorção Idade-série</td>";
echo "</tr>";
	
echo "<tr>";
echo "<td>";

$sql = "select 
                *,
                case when num_turmas > 0
                               then (num_turmas_acima_media*100)/num_turmas 
                               else 0
                end as porcentagem
from
                (select
                			   pdicodinep as inep,
                               pdenome as escola,
                               (select distinct count(pk_cod_turma) from educacenso_2010.tab_turma turma where fk_cod_entidade = p.pdicodinep::bigint) as num_turmas,
                               (select distinct count(fk_cod_turma) from pdeinterativo.distorcaoaproveitamento turma2 where pdeid = p.pdeid and diasubmodulo = 'D' and diamarcado = 'D' and diastatus = 'A') as num_turmas_acima_media
                from pdeinterativo.pdinterativo p 
                inner join territorios.municipio m on m.muncod=p.muncod
                where pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"")."
                ) as tbl;";
	
$cabecalho = array("INEP","Escola","Nº Total Turmas","Nº Turmas acima Média","% Turmas acima da média Brasil");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Aproveitamento Escolar - Abandono</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select 
                *,
                case when num_turmas > 0
                               then (num_turmas_acima_media*100)/num_turmas 
                               else 0
                end as porcentagem
from
                (select
                			   pdicodinep as inep,
                               pdenome as escola,
                               (select distinct count(pk_cod_turma) from educacenso_2010.tab_turma turma where fk_cod_entidade = p.pdicodinep::bigint) as num_turmas,
                               (select distinct count(fk_cod_turma) from pdeinterativo.distorcaoaproveitamento turma2 where pdeid = p.pdeid and diasubmodulo = 'A' and diamarcado = 'A' and diastatus = 'A') as num_turmas_acima_media
                from
                               pdeinterativo.pdinterativo p 
                inner join territorios.municipio m on m.muncod=p.muncod
                where
                               pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"")."
                ) as tbl;";

$cabecalho = array("INEP","Escola","Nº Total Turmas","Nº Turmas acima da média","% Turmas acima da média");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Aproveitamento Escolar - Reprovação</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";

$sql = "select 
                *,
                case when num_turmas > 0
                               then (num_turmas_acima_media*100)/num_turmas 
                               else 0
                end as porcentagem
from
                (select
                			   pdicodinep as inep,
                               pdenome as escola,
                               (select distinct count(pk_cod_turma) from educacenso_2010.tab_turma turma where fk_cod_entidade = p.pdicodinep::bigint) as num_turmas,
                               (select distinct count(fk_cod_turma) from pdeinterativo.distorcaoaproveitamento turma2 where pdeid = p.pdeid and diasubmodulo = 'A' and diamarcado = 'R' and diastatus = 'A') as num_turmas_acima_media
                from
                               pdeinterativo.pdinterativo p 
                inner join territorios.municipio m on m.muncod=p.muncod
                where
                               pdistatus='A' ".(($where2)?"AND ".implode(" AND ",$where2):"")."
                ) as tbl;";

$cabecalho = array("INEP","Escola","Nº Total Turmas","Nº Turmas acima da média","% Turmas acima da média");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);

echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td class=SubTituloCentro>Relatório Áreas de conhecimento - Reprovação (por Disciplina)</td>";
echo "</tr>";


echo "<tr>";
echo "<td>";


$sql = "select foo.no_disciplina, SUM(foo.dtdnumreprovado) from (

select distinct
				pk_cod_turma,
				pk_cod_disciplina,
				no_disciplina,
				dtdnumreprovado
			from 
				educacenso_2010.tab_turma turma
			inner join
				educacenso_2010.tab_etapa_ensino etapa ON etapa.pk_cod_etapa_ensino = turma.fk_cod_etapa_ensino
			inner join
				pdeinterativo.distorcaoaproveitamento dia ON dia.fk_cod_turma = turma.pk_cod_turma
			inner join
				educacenso_2010.tab_disciplina_turma disturma ON disturma.fk_cod_turma = turma.pk_cod_turma
			inner join
				educacenso_2010.tab_disciplina disc ON disc.pk_cod_disciplina = disturma.fk_cod_disciplina 
			inner join 
				pdeinterativo.distorcaodisciplina dst ON dst.fk_cod_disciplina = disc.pk_cod_disciplina and dst.fk_cod_turma = turma.pk_cod_turma and dia.pdeid = dst.pdeid 
			inner join 
				pdeinterativo.pdinterativo p ON p.pdeid = dst.pdeid 
			inner join 
				territorios.municipio m on m.muncod=p.muncod
			where 
				dia.diamarcado = 'R'
			and
				dia.diastatus = 'A'
			".(($where2)?"AND ".implode(" AND ",$where2):"")."
			order by
				no_disciplina,
				pk_cod_turma

) foo 
group by foo.no_disciplina
";

$cabecalho = array("Disciplina","Número de Reprovação");
$db->monta_lista_simples($sql,$cabecalho,1000000,5,'N','100%',$par2);


echo "</td>";
echo "</tr>";


	
echo "</table>";


?>