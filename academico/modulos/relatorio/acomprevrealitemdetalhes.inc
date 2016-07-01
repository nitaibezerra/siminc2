<?php

include '_funcoesacomprevreal.php';

include APPRAIZ . 'includes/cabecalho.inc';

$_ORG = array(TPENSSUP => "Educação Superior",
			  TPENSPROF => "Educação Profissional");
			  
switch($_REQUEST['filtro']['orgid']) {
	case TPENSSUP:
		$fune = ACA_ID_UNIVERSIDADE;
		$func = ACA_ID_CAMPUS;
		break;
	case TPENSPROF:
		$fune = ACA_ID_ESCOLAS_TECNICAS;
		$func = ACA_ID_UNED;
		break;
}

?>
<script>
function trocarfontedados(value) {
	window.location='academico.php?modulo=relatorio/acomprevrealitem&acao=A&filtro[orgid]=<? echo $_REQUEST['filtro']['orgid']; ?>&filtro[nome]=<? echo $_REQUEST['filtro']['nome']; ?>&filtro[itmprevisto]=<? echo $_REQUEST['filtro']['itmprevisto']; ?>&filtro[itmrealizado]=<? echo $_REQUEST['filtro']['itmrealizado']; ?>&fontedados='+value;
}
</script>
<?

	$sql = "SELECT e2.entid as codigo,
				   e2.entnome as descricao,
				   cmp.cmpid
			FROM
				entidade.entidade e2
			INNER JOIN
				entidade.entidade e ON e2.entid = e.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid
			INNER JOIN
				academico.campus cmp ON cmp.entid = e2.entid 
			WHERE
				ea.entid = ".$_REQUEST['filtroentid']." AND
				e.entstatus = 'A' AND ef.funid = ".$func."
			ORDER BY
				e.entnome";

$campus = $db->carregar($sql);
						  
echo "<br>";
monta_titulo( base64_decode($_REQUEST['filtro']['nome'])." (".$_ORG[$_REQUEST['filtro']['orgid']].")",'Acompanhamento previsto/realizado');


if(is_null($_REQUEST['fontedados']))
	$_REQUEST['fontedados'] = 1;

if($_REQUEST['filtro']['orgid'] == TPENSSUP) {
	echo '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela" >';
	
	echo "<tr><td class=SubTituloDireita>Dados:</td><td>";
	
	$dados = array(0 => array('codigo' => '1','descricao' => 'TOTAL'),
				   1 => array('codigo' => '0','descricao' => 'REUNI'));
				   
	$fontedados = $_REQUEST['fontedados'];
	$db->monta_combo('fontedados', $dados, 'S', 'Selecione', 'trocarfontedados', '', '', '200', 'S', 'fontedados');
	echo "</td></tr>";
	
	echo "</table>";
}
						  
echo '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela" >';

echo "<tr><td rowspan=2 class=SubTituloCentro>Campus</td>";

foreach($anosanalisados[$_REQUEST['filtro']['orgid']] as $ano) {
	
if( (int)date("Y") > (int)$ano ){
		$colspan = 3;		
	}else{
		$colspan = 1;
	}
	
	echo "<td colspan=$colspan class=SubTituloCentro>".$ano."</td>";
}

echo "</tr>";

echo "<tr>";
foreach($anosanalisados[$_REQUEST['filtro']['orgid']] as $ano) {

	if( (int)date("Y") > (int)$ano ){
		echo "<td class=SubTituloCentro>Previsto</td><td class=SubTituloCentro>Realizado</td><td class=SubTituloCentro>%</td>";		
	}else{
		echo "<td class=SubTituloCentro>Previsto</td>";
	}
}
echo "</tr>";

$num = 0;

foreach($campus as $l) {

	$corLinha = $num%2 ? "#f7f7f7" : "#ffffff";
	$num++;
	
	echo "<tr bgcolor=\"$corLinha\" onmouseout=\"this.bgColor='$corLinha';\" onmouseover=\"this.bgColor='#ffffcc';\" ><td>".$l['descricao']."</td>";
		

	$cont = 0;
	foreach($anosanalisados[TPENSSUP] as $ano) {
		
		$corLinha = $cont%2 ? "#f7f7f7" : "#ffffff";
		$cont++;
				
		$valorreal = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cmp
									 LEFT JOIN academico.item itm ON itm.itmid = cmp.itmid
								  	 LEFT JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid 
								  	 WHERE cmp.itmid='".$_REQUEST['filtro']['itmrealizado']."' AND cpiano='".$ano."' AND cmp.cmpid='".$l['cmpid']."' AND cpitabnum=".$_REQUEST['fontedados']." 
								  	 GROUP BY tpimascara");
		$tot_valorreal[$ano] += $valorreal['v'];
		
		$valorprev = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cmp
									 LEFT JOIN academico.item itm ON itm.itmid = cmp.itmid
								  	 LEFT JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid
								  	 WHERE cmp.itmid='".$_REQUEST['filtro']['itmprevisto']."' AND cpiano='".$ano."' AND cmp.cmpid='".$l['cmpid']."' AND cpitabnum=".$_REQUEST['fontedados']." 
								  	 GROUP BY tpimascara");
		$tot_valorprev[$ano] += $valorprev['v'];
		
		if( (int)date("Y") > (int)$ano ){
			echo "<td style=\"color:#888888\" align=right>".(($valorprev['v'])?mascaraglobal($valorprev['v'],$valorprev['tpimascara']):"-")."</td><td style=\"color:#888888\" align=right>".(($valorreal['v'])?mascaraglobal($valorreal['v'],$valorreal['tpimascara']):"-")."</td>";		
		}else{
			echo "<td style=\"color:#888888\" align=right>".(($valorprev['v'])?mascaraglobal($valorprev['v'],$valorprev['tpimascara']):"-")."</td>";
		}
		
		if( (int)date("Y") > (int)$ano ){
			echo "<td align=center>".barraDeProgresso($valorprev['v'], $valorreal['v'])."</td>";
		}
				
	}
	echo "</tr>";
	
	echo "<tr>";
}

echo "<tr bgcolor='#DCDCDC' >";
echo "<td><b>Total:</b></td>";
foreach($anosanalisados[TPENSSUP] as $ano) {
	
	if( (int)date("Y") > (int)$ano ){
		echo "<td style=\"#888888;font-weight:bold\" align=right>".(($tot_valorprev[$ano])?formata_valor($tot_valorprev[$ano],0):"-")."</td><td style=\"#888888;font-weight:bold\" align=right>".(($tot_valorreal[$ano])?formata_valor($tot_valorreal[$ano],0):"-")."</td><td style=\"#888888;font-weight:bold\" align=right>".barraDeProgresso($tot_valorprev[$ano], $tot_valorreal[$ano])."</td>";		
	}else{
		echo "<td  style=\"#888888;font-weight:bold\" align=right>".(($tot_valorprev[$ano])?formata_valor($tot_valorprev[$ano],0):"-")."</td>";
	}
	
}
echo "</tr>";
echo "</table>";
echo '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela" >';
echo "<tr><td align=center ><input type=\"button\" value=\"Voltar\" onclick=\"window.location='academico.php?modulo=relatorio/acomprevrealitem&acao=A&filtro[orgid]=".$_REQUEST['filtro']['orgid']."&filtro[nome]=".$_REQUEST['filtro']['nome']."&filtro[itmprevisto]=".$_REQUEST['filtro']['itmprevisto']."&filtro[itmrealizado]=".$_REQUEST['filtro']['itmrealizado']."&fontedados=".$_REQUEST['fontedados']."'\" /></td></tr>";
echo "</table>";

?>