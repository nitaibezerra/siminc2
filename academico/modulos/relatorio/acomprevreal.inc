<?php

include '_funcoesacomprevreal.php';

include APPRAIZ . 'includes/cabecalho.inc';


$_LISTA[TPENSSUP] = array('Docente' => array('realizado' => 25,'previsto' => 3),
			    		  'Técnico' => array('realizado' => 26,'previsto' => 4),
						  'Matrícula' => array('realizado' => 45,'previsto' => false),
						  'Vagas' => array('realizado' => 28,'previsto' => 2),
						  'Cursos' => array('realizado' => 29,'previsto' => 10),
						  'Investimento' => array('realizado' => 30,'previsto' => 5)
//						  'Bolsas de mestrado' => array('realizado' => 42,'previsto' => 38),
//						  'Bolsas de doutorado' => array('realizado' => 43,'previsto' => 40),
//						  'Bolsas de pós-doutorado' => array('realizado' => 44,'previsto' => 41)
						  );
						  
						  
$_LISTA[TPENSPROF] = array('Docentes' => array('realizado' => 50,'previsto' => 49),
			    		   'Técnicos' => array('realizado' => 52,'previsto' => 51),
						   'Matrículas' => array('realizado' => 34,'previsto' => 11, 'anomax' => '2008'),
						   'Vagas' => array('realizado' => 35,'previsto' => 31),
						   'Cursos' => array('realizado' => 36,'previsto' => 32),
						   'Investimentos (2005-2009)' => array('realizado' => 37,'previsto' => 14)
						  );


echo "<br>";

$menu = array(0 => array("id" => 1, "descricao" => "Educação Superior",   	"link" => "/academico/academico.php?modulo=relatorio/acomprevreal&acao=A&orgid=1"),
			  1 => array("id" => 2, "descricao" => "Educação Profissional", "link" => "/academico/academico.php?modulo=relatorio/acomprevreal&acao=A&orgid=2")
		  	  );
		  	  
if(!$_REQUEST['orgid'])
	$_REQUEST['orgid'] = 1;

if(is_null($_REQUEST['fontedados']))
	$_REQUEST['fontedados'] = 1;
	
?>
<script>

function trocarfontedados(value) {
	window.location='/academico/academico.php?modulo=relatorio/acomprevreal&acao=A&orgid=<? echo $_REQUEST['orgid']; ?>&fontedados='+value;
}

function trocarentid(value) {
	window.location='/academico/academico.php?modulo=relatorio/acomprevreal&acao=A&orgid=<? echo $_REQUEST['orgid']; ?>&fontedados=<? echo $_REQUEST['fontedados']; ?>&filtroentid='+value;	
}

function trocarentid2(value) {
	window.location='/academico/academico.php?modulo=relatorio/acomprevreal&acao=A&orgid=<? echo $_REQUEST['orgid']; ?>&fontedados=<? echo $_REQUEST['fontedados']; ?>&filtroentid=<? echo $_REQUEST['filtroentid']; ?>&filtroentid2='+value;	
}

</script>
<?					  						  
/* montando o menu */
echo montarAbasArray($menu, "/academico/academico.php?modulo=relatorio/acomprevreal&acao=A&orgid=".$_REQUEST['orgid']);
						  

$titulo_modulo = "Previsto/Realizado";
monta_titulo( $titulo_modulo,'Acompanhamento previsto/realizado');

	
/*
 * ESPAÇO PARA FILTROS
 * - Fonte : Fonte dos dados podendo ser REUNI OU TOTAL (este filtro somente deve aparece quando se tratar de educação superior)
 * - Universidades : Filtrar todos os dados por uma determinada universidade 
 */
echo '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela" >';

// testando se eh educação superior
if($_REQUEST['orgid'] == TPENSSUP) {
	
	echo "<tr><td class=SubTituloDireita>Fonte:</td><td>";
	
	$dados = array(0 => array('codigo' => '1','descricao' => 'TOTAL'),
				   1 => array('codigo' => '0','descricao' => 'REUNI'));
				   
	$fontedados = $_REQUEST['fontedados'];
	$db->monta_combo('fontedados', $dados, 'S', 'Selecione', 'trocarfontedados', '', '', '200', 'S', 'fontedados');
	echo "</td></tr>";
	
}

echo "<tr><td class=SubTituloDireita>Instituições:</td><td>";
	
switch($_REQUEST['orgid']) {
	case TPENSSUP:
		$fune = ACA_ID_UNIVERSIDADE;
		$func = ACA_ID_CAMPUS;
		break;
	case TPENSPROF:
		$fune = ACA_ID_ESCOLAS_TECNICAS;
		$func = ACA_ID_UNED;
		break;
}

$sql = "SELECT
			e.entid as codigo,
			COALESCE(e.entsig, '') || ' - ' || e.entnome || ' (' || COALESCE(e.entunicod, '') || ')' as descricao
		FROM
			entidade.entidade e
		INNER JOIN
			entidade.funcaoentidade ef ON ef.entid = e.entid
		WHERE
			e.entstatus = 'A' AND ef.funid  in ('" . $fune . "')
		ORDER BY
			COALESCE(e.entsig, '') || ' - ' || e.entnome || ' (' || COALESCE(e.entunicod, '') || ')'";

$filtroentid = $_REQUEST['filtroentid'];
$db->monta_combo('filtroentid', $sql, 'S', 'Todas', 'trocarentid', '', '', '', 'S', 'filtroentid');
echo "</td></tr>";

// se tiver o filtro de entidade criar a clausula
if($_REQUEST['filtroentid']) {
	
	$sql = "SELECT e2.entid as codigo,
				   e2.entnome||'('||e2.entid||')' as descricao,
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
				e2.entnome";
	
	$campus = $db->carregar($sql);
	
	unset($camp);
	if($campus[0]) {
		foreach($campus as $cam) {
			$camp[] = $cam['cmpid']; 
		}
		$filtrocampus = " AND cpm.cmpid IN('".implode("','",$camp)."')";
	}
	
	echo "<tr><td class=SubTituloDireita>Campus:</td><td>";

	$filtroentid2 = $_REQUEST['filtroentid2'];
	$db->monta_combo('filtroentid2', $campus, 'S', 'Todos', 'trocarentid2', '', '', '', 'S', 'filtroentid2');
	
	if($_REQUEST['filtroentid2']) {
		$filtrocampus = " AND cpm.cmpid='".$db->pegaUm("SELECT cmpid FROM academico.campus WHERE entid='".$_REQUEST['filtroentid2']."'")."'";
	}
	
	echo "</td></tr>";
	
	
}



echo "</table>";

/*
 * FIM - ESPAÇO PARA FILTROS
 */



echo '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela" >';
echo "<tr><td rowspan=2 class=SubTituloCentro>Item</td>";
foreach($anosanalisados[$_REQUEST['orgid']] as $ano) {
	
	if( (int)date("Y") > (int)$ano ){
		$colspan = 3;		
	}else{
		$colspan = 1;
	}
	
	echo "<td colspan=$colspan class=SubTituloCentro>".$ano."</td>";
}
echo "</tr>";

echo "<tr >";
foreach($anosanalisados[$_REQUEST['orgid']] as $ano) {
	
	if( (int)date("Y") > (int)$ano ){
		echo "<td class=SubTituloCentro>Previsto</td><td class=SubTituloCentro>Realizado</td><td class=SubTituloCentro>%</td>";		
	}else{
		echo "<td class=SubTituloCentro>Previsto</td>";
	}
	
}
echo "</tr>";

$cont = 0;

foreach($_LISTA[$_REQUEST['orgid']] as $n => $l) {
	
	$corLinha = $cont%2 ? "#f7f7f7" : "#ffffff";
	$cont++;
	
	$link_n = base64_encode($n);
	
	echo "<tr bgcolor=\"$corLinha\" onmouseout=\"this.bgColor='$corLinha';\" onmouseover=\"this.bgColor='#ffffcc';\" ><td class=SubTituloDireita><a href=academico.php?modulo=relatorio/acomprevrealitem&acao=A&filtro[orgid]=".$_REQUEST['orgid']."&filtro[nome]=".$link_n."&filtro[itmprevisto]=".$l['previsto']."&filtro[itmrealizado]=".$l['realizado']."&fontedados=".$_REQUEST['fontedados'].">".$n."</a></td>";
	
	foreach($anosanalisados[$_REQUEST['orgid']] as $ano) {
		
		$valorreal = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cpm
									 LEFT JOIN academico.item itm ON itm.itmid = cpm.itmid
								  	 LEFT JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid 
								  	 WHERE cpm.itmid='".$l['realizado']."' ".$filtrocampus." AND cpiano='".$ano."' AND cpitabnum=".$_REQUEST['fontedados']." 
								  	 GROUP BY tpimascara");
		unset($valorprev);
		if($l['previsto']) {
			$valorprev = $db->pegaLinha("SELECT SUM(cpivalor) as v, tpimascara FROM academico.campusitem cpm
										 LEFT JOIN academico.item itm ON itm.itmid = cpm.itmid
									  	 LEFT JOIN academico.tipoitem tpm ON tpm.tpiid = itm.tpiid
									  	 WHERE cpm.itmid='".$l['previsto']."' ".$filtrocampus." AND cpiano='".$ano."' AND cpitabnum=".$_REQUEST['fontedados']." 
									  	 GROUP BY tpimascara");
		}
		
	
		if( (int)date("Y") > (int)$ano ){
			echo "<td align=right style=\"color:#888888\" >".(($valorprev['v'])?mascaraglobal($valorprev['v'],$valorprev['tpimascara']):"-")."</td><td align=right style=\"color:#888888\" >".(($valorreal['v'])?mascaraglobal($valorreal['v'],$valorreal['tpimascara']):"-")."</td>";		
		}else{
			echo "<td align=right style=\"color:#888888\" >".(($valorprev['v'])?mascaraglobal($valorprev['v'],$valorprev['tpimascara']):"-")."</td>";
		}
		
		if( (int)date("Y") > (int)$ano ){
			echo "<td align=center>".barraDeProgresso($valorprev['v'], $valorreal['v'])."</td>";
		}
		
	}
	
	echo "</tr>";
}

echo "</table>";
?>