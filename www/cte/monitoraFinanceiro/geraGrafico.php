<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();

include ("../../../includes/open_flash_chart/open-flash-chart.php");
include ("../../../includes/open_flash_chart/ofc_sugar.php");

$arParametro = explode(";",$_REQUEST['tipo']);

$prsid 	= $arParametro[1];

$stWhere  = $arParametro[2] ? " AND to_char(hmcdatamonitoramento, 'YYYYMM') >= '" . $arParametro[2] . "'" : '';
$stWhere .= $arParametro[3] ? " AND to_char(hmcdatamonitoramento, 'YYYYMM') <= '" . $arParametro[3] . "'" : '';

$sql = "SELECT  p.prsvalorconvenio AS total,
		to_char(hmcdatamonitoramento, 'MM') as mes,
		to_char(hmcdatamonitoramento, 'YYYY')  as ano,
		hmc.hmcid,
		coalesce( ( SELECT 
			sum(hici.hmsvalortotalpago) AS gasto
			FROM cte.historicomonitoramentoconvenio hmci 
			INNER JOIN cte.historicomonitoramentoconvsubac hmsi ON hmci.hmcid = hmsi.hmcid
			INNER JOIN cte.historicoconvitemcomposicao hici ON hici.hmsid = hmsi.hmsid
			INNER JOIN cte.projetosape pi ON pi.prsid = hmci.prsid
			WHERE hmci.prsid = ".$prsid."  and hmci.hmcid = hmc.hmcid
			AND hici.scsid in (1,2)
			GROUP BY hmci.hmcid, to_char(hmcdatamonitoramento, 'MM'),to_char(hmcdatamonitoramento, 'YYYY')
		) , 0 ) AS gasto
	FROM cte.historicomonitoramentoconvenio hmc 
	INNER JOIN cte.historicomonitoramentoconvsubac hms ON hmc.hmcid = hms.hmcid
	INNER JOIN cte.historicoconvitemcomposicao hic ON hic.hmsid = hic.hmsid
	INNER JOIN cte.projetosape p ON p.prsid = hmc.prsid
	WHERE hmc.prsid = ".$prsid."
	AND hic.scsid in (1,2)
	$stWhere
	GROUP BY hmc.hmcid, p.prsvalorconvenio, mes, ano
	ORDER BY ano,mes";

$dados 	= $db->carregar($sql);
$dados = $dados ? $dados : array();

$soma = 0;
	foreach($dados as $dado){
		$arrMes[] = $dado['mes']."/".$dado['ano'];
		$arrGasto[] = round($dado['gasto']);
		$soma = $soma + round($dado['gasto']);
		$arrAcum[] = $soma;
		
	}
	
	$totalAteMomento = max($arrAcum);

	$title = new title( "Relatorio" );

	$line_1_default_dot = new dot();
	$line_1_default_dot->size(3)->halo_size(3)->colour('#6495ED');
		
	$line_1 = new line();
	$line_1->set_default_dot_style($line_1_default_dot);
	$line_1->set_width( 2 );
	$line_1->set_colour("#6495ED");
	
	$line_1->set_values( $arrGasto );
	$line_1->set_key("Valor executado no mes. Dados nao Cumulativos",10);
	
	//////////////////////////////////////////////////////
	
	$line_2_default_dot = new dot();
	$line_2_default_dot->size(3)->halo_size(3)->colour('#990000');
		
	$line_2 = new line();
	$line_2->set_default_dot_style($line_2_default_dot);
	$line_2->set_width( 2 );
	$line_2->set_colour("#990000");
	
	$line_2->set_values( $arrAcum );
	$line_2->set_key("Valor executado ate o momento. Dados cumulativos mensalmente.",10);
	
	//dbg($totalAteMomento);

	$arrMax = explode(".",round($totalAteMomento,2));
	$MaxValue = substr($arrMax[0],0,1);
	$MaxValue = $MaxValue + 1;
	for($i = 1; $i < strlen($arrMax[0]);$i++){
		$MaxValue .= "0";
	}
				
	//Eixo Y
	$eixo_y = new y_axis();
	//adiciona o minimo, maximo, intervalo do eixo y
	$max = round($MaxValue);
	$div_range = count($sehqtd) == 1 || count($sehqtd) == 0 ? 1 : ($MaxValue / 4);
	//$range = round($MaxValue) / (int)($div_range);
	$eixo_y->set_range( 0, round($MaxValue), $div_range);
	
	//Definindo Eixo X
	$x_labels = new x_axis_labels();
	//adiciona a cor dos elementos do eixo x
	$x_labels->set_colour( '#990000' );
	//adiciona os elementos do eixo x
	$x_labels->set_labels( $arrMes );
	//rotaciona o eixo x
	$x_labels->rotate(30);
	//Eixo x
	$eixo_x = new x_axis();
	//Adiciona a definição do eixo x
	$eixo_x->set_labels($x_labels) ;	
	//novo gráfico
	$chart = new open_flash_chart();
	//adiciona o eixo x ao grafico
	$chart->set_x_axis( $eixo_x );
	//adiciona o eixo y ao grafico
	$chart->set_y_axis( $eixo_y );
	
	//Eixo Y
	//$eixo_y = new y_axis();
	//$eixo_y->set_range( 0, $dados[0]['total']);


	
//	$chart = new open_flash_chart();
//	$chart->set_x_axis( $eixo_x );
//	$chart->set_y_axis( $eixo_y );
	
	$chart->add_element( $line_1 );
	$chart->add_element( $line_2 );
	$chart->set_bg_colour( '#ffffff' );
	
	echo $chart->toPrettyString();
?>