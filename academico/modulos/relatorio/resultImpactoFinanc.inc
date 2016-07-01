<?
if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","250M");
set_time_limit(0);


$sql = monta_sql();

$dados = $db->carregar($sql);

?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<table cellspacing="1" cellpadding="5" border="0" align="center" width="100%" class="tabela">
	<tr style="background: rgb(217, 217, 217) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;">
		<td rowspan="2">
			<div style="font-weight: bold;"><div>Unidade</div></div>
		</td>
		<td colspan="2" align="center" valign="top" style="font-weight: bold;">Docentes do Magisterio Superior</td>
		<td colspan="2" align="center" valign="top" style="font-weight: bold;">Professores da Educação Básica, Técnica e Tecnológica</td>
		<td colspan="2" align="center" valign="top" style="font-weight: bold;">TA Nível B</td>
		<td colspan="2" align="center" valign="top" style="font-weight: bold;">TA Nível C</td>
		<td colspan="2" align="center" valign="top" style="font-weight: bold;">TA Nível D</td>
		<td colspan="2" align="center" valign="top" style="font-weight: bold;">TA Nível E</td>		
		<td colspan="2" align="center" valign="top" style="font-weight: bold;">Total</td>				
	</tr>
	<tr style="background: #E9E9E9;">
		<td align="center" valign="top" style="font-weight: bold;">Físico</td>
		<td align="center" valign="top" style="font-weight: bold;">Financeiro</td>
		<td align="center" valign="top" style="font-weight: bold;">Físico</td>
		<td align="center" valign="top" style="font-weight: bold;">Financeiro</td>
		<td align="center" valign="top" style="font-weight: bold;">Físico</td>
		<td align="center" valign="top" style="font-weight: bold;">Financeiro</td>
		<td align="center" valign="top" style="font-weight: bold;">Físico</td>
		<td align="center" valign="top" style="font-weight: bold;">Financeiro</td>
		<td align="center" valign="top" style="font-weight: bold;">Físico</td>
		<td align="center" valign="top" style="font-weight: bold;">Financeiro</td>
		<td align="center" valign="top" style="font-weight: bold;">Físico</td>
		<td align="center" valign="top" style="font-weight: bold;">Financeiro</td>				
		<td align="center" valign="top" style="font-weight: bold;">Físico</td>
		<td align="center" valign="top" style="font-weight: bold;">Financeiro</td>						
	</tr>
<?php
$orgid = $_POST['tipoensino']; 
$ano   = $_POST['exercicio'] ? $_POST['exercicio'] : date('Y');
$impid = $_POST['impid'];

switch ($ano) {
	case 2008:
			$select = "bslvlr2008";	 		
		break;	
	case 2009:
			$select = "bslvlr2009";	
		break;	
	case 2010:
			$select = "bslvlr2010";	
		break;	
	case 2011:
			$select = "bslvlr2011";	
		break;	
	case 2012:
			$select = "bslvlr2012";	
		break;
	default:
			die('O sistema não contempla este ano!');
		break;			
}

$sql = "SELECT
			clsid,
			$select
		FROM
			academico.basesalarial
		WHERE
			orgid = " . $orgid;

$dadoAno = $db->carregar($sql);

if ( !empty($dadoAno) ){
	foreach ( $dadoAno as $dAno ){
		switch ($dAno['clsid']){
			case CLASSE_DOC:
				$rem_doc = $dAno[$select];
				break;
			case CLASSE_DOC2:
				$rem_doc2 = $dAno[$select];
				break;
			case CLASSE_E:
				$rem_e = $dAno[$select];
				break;
			case CLASSE_D:
				$rem_d = $dAno[$select];				
				break;
			case CLASSE_C:
				$rem_c = $dAno[$select];				
				break;
			case CLASSE_B:
				$rem_b = $dAno[$select];				
				break;
		}
	}	
}			

$sql = "SELECT
			impvalor * " . ENCARGO . "
		FROM
			academico.indiceimpacto
		WHERE
			impid = " . $impid;

$rtx = $db->pegaUm($sql);

foreach ($dados as $item):
	
	// Cálculos das colunas
	$fis_doc = $item['provimento_doc'];
	$fis_doc = $fis_doc > 0 ? $fis_doc : 0;
	$fin_doc = number_format(bcmul($fis_doc, bcmul($rem_doc, $rtx, 4), 4), 2, ',', '.');

	$fis_doc2 = $item['provimento_doc2'];
	$fis_doc2 = $fis_doc2 > 0 ? $fis_doc2 : 0;
	$fin_doc2 = number_format(bcmul($fis_doc2, bcmul($rem_doc2, $rtx, 4), 4), 2, ',', '.');

	$fis_e = $item['provimento_e'];
	$fis_e = $fis_e > 0 ? $fis_e : 0;	
	$fin_e = number_format(bcmul($fis_e, bcmul($rem_e, $rtx, 4), 4), 2, ',', '.');
	
	$fis_d = $item['provimento_d'];
	$fis_d = $fis_d > 0 ? $fis_d : 0;	 
	$fin_d = number_format(bcmul($fis_d, bcmul($rem_d, $rtx, 4), 4), 2, ',', '.');
	
	$fis_c = $item['provimento_c']; 
	$fis_c = $fis_c > 0 ? $fis_c : 0;	
	$fin_c = number_format(bcmul($fis_c, bcmul($rem_c, $rtx, 4), 4), 2, ',', '.');
	
	$fis_b = $item['provimento_b'];
	$fis_b = $fis_b > 0 ? $fis_b : 0;	 
	$fin_b = number_format(bcmul($fis_b, bcmul($rem_b, $rtx, 4), 4), 2, ',', '.');

	// Totalizador da linha
	$fisTotalLinha = $fis_doc + $fis_doc2 + $fis_e + $fis_d + $fis_c + $fis_b;
	$finTotalLinha = number_format(formatNum($fin_doc) + formatNum($fin_doc2) + formatNum($fin_e) + formatNum($fin_d) + formatNum($fin_c) + formatNum($fin_b), 2, ',', '.');

	// Totalizadores do final
	$fis_doc_total += $fis_doc; 
	$fin_doc_total += formatNum($fin_doc);

	$fis_doc2_total += $fis_doc2; 
	$fin_doc2_total += formatNum($fin_doc2);

	$fis_e_total += $fis_e;
	$fin_e_total += formatNum($fin_e);

	$fis_d_total += $fis_d;
	$fin_d_total += formatNum($fin_d);
	
	$fis_c_total += $fis_c; 
	$fin_c_total += formatNum($fin_c);

	$fis_b_total += $fis_b;
	$fin_b_total += formatNum($fin_b);
	
	// Controle de cor da tr
	$z++;
	if ( is_int($z/2) ){
		$color = '#F1F1F1';	
	}else{
		$color = '#FAFAFA';
	}
?>	
	<tr style="background:<?=$color; ?>;">
		<td><?=$item['unidade']?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fis_doc?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fin_doc?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fis_doc2?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fin_doc2?></td> 
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fis_b?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fin_b?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fis_c?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fin_c?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fis_d?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fin_d?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fis_e?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fin_e?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$fisTotalLinha?></td>
		<td style="text-align: right; color: rgb(0, 102, 204);"><?=$finTotalLinha?></td>		
	</tr>
<? 
endforeach;

// Soma final total
$fisTotalFinal = $fis_doc_total + $fis_doc2_total + $fis_b_total + $fis_c_total + $fis_d_total + $fis_e_total;
$finTotalFinal = number_format($fin_doc_total + $fin_doc2_total + $fin_b_total + $fin_c_total + $fin_d_total + $fin_e_total, 2, ',', '.');

// Soma final colunas
$fin_doc_total = number_format($fin_doc_total, 2, ',', '.');
$fin_doc2_total = number_format($fin_doc2_total, 2, ',', '.');
$fin_e_total   = number_format($fin_e_total, 2, ',', '.');	
$fin_d_total   = number_format($fin_d_total, 2, ',', '.');
$fin_c_total   = number_format($fin_c_total, 2, ',', '.');
$fin_b_total   = number_format($fin_b_total, 2, ',', '.');



?>	
	<tr style="background:#DFDFDF;">
		<td><b>Totais</b></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fis_doc_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fin_doc_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fis_doc2_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fin_doc2_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fis_b_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fin_b_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fis_c_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fin_c_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fis_d_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fin_d_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fis_e_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fin_e_total?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$fisTotalFinal?></td>
		<td style="font-weight: bold; text-align: right; color: rgb(0, 102, 204);"><?=$finTotalFinal?></td>		
	</tr>
</table>
</body>
<?php

function formatNum($num){
	$num = str_replace(array('.',','), array('','.'), $num);
	return $num;
}

function monta_sql(){
		
	extract($_POST);
	
	$select = array();
	$from	= array();

	if ( $tipoensino ){
		$where[] = " orgid = '" . $tipoensino . "'";		
	}else{
		die("<script>alert('Faltam parametros, tente novamente!'); history.go(-1);</script>");
	}
	
	$sql = "SELECT
				unidade,
				COALESCE(SUM(provimento_doc), 0) AS provimento_doc,
				COALESCE(SUM(provimento_doc2), 0) AS provimento_doc2,
				COALESCE(SUM(provimento_e), 0) AS provimento_e,
				COALESCE(SUM(provimento_d), 0) AS provimento_d,
				COALESCE(SUM(provimento_c), 0) AS provimento_c,
				COALESCE(SUM(provimento_b), 0) AS provimento_b
			
			FROM 
			(
					SELECT
						orgid,
						entnome AS unidade,
						sum(provimento_pendente) AS provimento_doc,
						0 as provimento_doc2,
						0 as provimento_e,
						0 as provimento_d,
						0 as provimento_c,
						0 as provimento_b
					FROM
						academico.provimento_pendente pp
					INNER JOIN 
						entidade.entidade e ON e.entid = pp.entidentidade
					WHERE
						clsid = " . CLASSE_DOC . "
					GROUP BY
						orgid,
						entnome
			
				UNION ALL
			
					SELECT
						orgid,
						entnome AS unidade,
						0 AS provimento_doc,
						sum(provimento_pendente) AS provimento_doc2,
						0 as provimento_e,
						0 as provimento_d,
						0 as provimento_c,
						0 as provimento_b
			
					FROM
						academico.provimento_pendente pp
					INNER JOIN 
						entidade.entidade e ON e.entid = pp.entidentidade
					WHERE
						clsid = " . CLASSE_DOC2 . "
					GROUP BY
						orgid,
						entnome
			
				UNION ALL
				
					SELECT
						orgid,
						entnome AS unidade,
						0 AS provimento_doc,
						0 as provimento_doc2,
						sum(provimento_pendente) AS provimento_e,
						0 as provimento_d,
						0 as provimento_c,
						0 as provimento_b
			
					FROM
						academico.provimento_pendente pp
					INNER JOIN 
						entidade.entidade e ON e.entid = pp.entidentidade
					WHERE
						clsid = " . CLASSE_E . "
					GROUP BY
						orgid,
						entnome
			
				UNION ALL
			
					SELECT
						orgid,
						entnome AS unidade,
						0 AS provimento_doc,
						0 as provimento_doc2,
						0 AS provimento_e,
						sum(provimento_pendente) AS provimento_d,
						0 as provimento_c,
						0 as provimento_b
			
					FROM
						academico.provimento_pendente pp
					INNER JOIN 
						entidade.entidade e ON e.entid = pp.entidentidade
					WHERE
						clsid = " . CLASSE_D . "
					GROUP BY
						orgid,
						entnome
			
				UNION ALL
			
					SELECT
						orgid,
						entnome AS unidade,	
						0 AS provimento_doc,
						0 as provimento_doc2,
						0 AS provimento_e,
						0 AS provimento_d,
						sum(provimento_pendente) AS provimento_c,
						0 as provimento_b
					FROM
						academico.provimento_pendente pp
					INNER JOIN 
						entidade.entidade e ON e.entid = pp.entidentidade
					WHERE
						clsid = " . CLASSE_C . "
					GROUP BY
						orgid,
						entnome
			
				UNION ALL
			
					SELECT
						orgid,
						entnome AS unidade,
						0 AS provimento_doc,
						0 as provimento_doc2,
						0 AS provimento_e,
						0 AS provimento_d,
						0 AS provimento_c,
						sum(provimento_pendente) AS provimento_b
					FROM
						academico.provimento_pendente pp
					INNER JOIN 
						entidade.entidade e ON e.entid = pp.entidentidade
					WHERE
						clsid = " . CLASSE_B . "
					GROUP BY
						orgid,
						entnome
			) AS f
			" . (is_array($where) ? " WHERE ".implode(' AND ', $where) : '') . "
			GROUP BY
				unidade
			ORDER BY
				unidade";

	return $sql;
}
?>
