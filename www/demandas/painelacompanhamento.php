<?php

//var_dump($_REQUEST, d);

include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_funcoes.php";

/* Inserir definições (aos poucos...)*/
define("LABEL_EIXOX", 8);
define("LABEL_EIXOX_MES", 8);
define("LABEL_EIXOY", 8);
define("LABEL_EIXOY_MES", 8);
define("PLOT_MES", 9);
define("PLOT_PESSOA", 8);
define("TITULO", 10);

//$cpf_tecnicos
$cpf_tecnicos = array(0 => "",
                      1 => "00009044140",
                      2 => "00009044140",
                      3 => "03950206620",
                      4 => "00013767135",
                      5 => "96083433649",
                      6 => "00576253103",
                      7 => "03185477162",
                      8 => "63571749120",
                      9 => "03389483179",
                      10 => "01826031138",
                      11 => "87885018172",
                      12 => "02648851470",
                      13 => "60658304100",
                      14 => "00437411184",
                      15 => "72071281187",
                      16 => "46214968249",
                      17 => "68999097153",
                      18 => "05381592434",
                      19 => "01239677162",
                      20 => "70899940153",
                      21 => "01729882170",
                      22 => "80209513187",
                      23 => "66603544172",
                      24 => "01774834154",
                      25 => "03034711158",
                      26 => "07169331608",
                      27 => "03098119101",
                      28 => "87642093691",
                      29 => "00738974188",
                      30 => "01607208180",
                      31 => "02217342100",
                      32 => "01262100127",
                      33 => "02783871106",
                      34 => "70453721168",
                      35 => "73236390182",
                      36 => "99711583100",
                      37 => "01278208178",
                      38 => "01278208178",
                      39 => "83474498304",
                      40 => "01681797178",
                      50 => "73106747153",
                      51 => "03392267147",
                      52 => "00978626141",
                      53 => "73596957168",
                      54 => "02866001109",
                      55 => "01799838145",
                      56 => "00013767135",
                      57 => "69751420172",
                      58 => "69751420172",
                      59 => "98414747191",
                      60 => "85734217115",
                      61 => "71731610106",
                      62 => "02365253105",
                      63 => "07944991471",
                      64 => "02409830170",
                      65 => "11789236738",
                      66 => "03402352176",
                      67 => "02352492114",
                      68 => "02575964105",
                      69 => "03331080195",
                      70 => "02038894108",
                      71 => "71812865104",
                      72 => "00392956136",
                      73 => "01926058100",
                      74 => "97979341104",
                      75 => "04018313101",
                      76 => "01141190150",
                      77 => "00437411184",
                      78 => "00576253103",
                      79 => "02866001109",
                      80 => "",
                      81 => "46214968249",
                      82 => "00957919140",
                      83 => "66603544172",
                      84 => "02352492114",
                      85 => "03331080195",
                      86 => "02575964105",
                      87 => "83474498304",
                      88 => "78848288120",
                      89 => "00970087101",
                      90 => "02967052175",
                      91 => "79565417191",
                      92 => "02225240140",
                      93 => "00517599180",
                      94 => "05982506974",
                      95 => "78300398104",
                      96 => "03257852100",
                      97 => "05169195125",
                      98 => "87305615153",
					  99 => "02244622100",
					 100 => "73294071604",
					 101 => "69136726168",
					 102 => "00504442120",
					 103 => "04542555194",
					 104 => "04313869190",
					 105 => "03711845177",
					 106 => "01784397113",
					 107 => "02971865150",
					 108 => "02625715103",
					 109 => "02399910109",
					 110 => "00746341199",
					 111 => "00235345164",
					 112 => "03725186138");

$_SESSION['cpf_tecnicos'] = $cpf_tecnicos; 


function buscardadosgrafico() {
	global $db;

	$sql = "SELECT * FROM demandas.graficos_indicadores WHERE grfid='".$_REQUEST['grfid']."'";
	$grafico = $db->pegaLinha($sql);
	echo $grafico['grfid']."|".$grafico['grfdsc']."|".str_replace("{namecombo}", "[".$grafico['grfid']."][".$_REQUEST['linha']."]", $grafico['grfagp']);
}

function barValueFormat($aLabel) {
    // Format '1000 english style
    // return number_format($aLabel)
    // Format '1000 french style
    return "R$ ".number_format($aLabel, 2, ',', '.');
}

function barValueFormat2($aLabel, $valor = null) {
    return $aLabel." (R$ ".number_format($valor, 2, ',', '.').")";
}

/*
 * Pontuação total por pessoa
 */
function pontuacaototal_pessoa() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   
/*   (select EXTRACT(MONTH FROM h0.htddata)||''||EXTRACT(YEAR FROM h0.htddata)
    from    workflow.historicodocumento h0
    inner join demandas.demanda d0 on h0.docid = d0.docid
    where    d.dmdid = d0.dmdid
    order by h0.htddata desc limit 1) AS dpeid,  
*/
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
   SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END)) AS dshqtde,
   (SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END))* COALESCE(crtvlponto,0) ) AS valor,
   d.usucpfexecutor AS tidid1 ,
   UPPER(u2.usunome) AS nome

FROM
   demandas.demanda d
      LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
      LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
--      LEFT JOIN demandas.prioridade p ON p.priid = d.priid                     
      LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid  
--      LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
      LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--      LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--      LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--      LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
--      LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--      LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--      LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--      LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--      LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
      LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
	  /*
      LEFT JOIN ( SELECT 
                     d1.dmdid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                     MAX(a.htddata) AS datasituacao
                  FROM  workflow.historicodocumento a
                     INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                  GROUP BY d1.dmdid ORDER BY 2 DESC
                ) AS dmd1 ON dmd1.dmdid = d.dmdid
      */          
      LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid = con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
	  
WHERE d.dmdstatus = 'A'
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')
AND t.ordid  IN  ('3')                           
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, tidid1, UPPER(u2.usunome), crtvlponto 
ORDER BY UPPER(u2.usunome), dpeid, tidid1";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['tidid1']]['nome']) {
				$_x_ax_[$data['tidid1']]['nome']  = $data['nome'];
				$_x_ax_[$data['tidid1']]['qtde'] = $data['dshqtde'];
				$_x_ax_[$data['tidid1']]['valor'] = $data['valor'];
			}
			else{
				$_x_ax_[$data['tidid1']]['qtde'] += $data['dshqtde'];
				$_x_ax_[$data['tidid1']]['valor'] += $data['valor'];
			}
			
		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['qtde'];
			$totalizador['qtde'] += $d['qtde'];
			$data_2[] = $d['valor'];
			$totalizador['valor'] += $d['valor'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['qtde']/count($eixo_x));
		$data_2[] = round($totalizador['valor']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['qtde']/count($eixo_x));
		$dat_2 = round($totalizador['valor']/count($eixo_x));
		$eix_x = "MÉDIA";
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['qtde'];
		$data_2[] = $totalizador['valor'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['qtde'];
		$dat_2 = $totalizador['valor'];
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "TOTAL";

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1400,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("PONTUAÇÃO TOTAL POR PESSOA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,7); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Pontuação");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_PESSOA);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Valor(R$)");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_PESSOA);
	$b2plot->value->SetFormatCallback('barValueFormat'); 
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));
	// Set color for the frame of each bar
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}

/*
 * Pontuação total por semana
 */
function pontuacaototal_semana() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   (select EXTRACT(MONTH FROM h0.htddata)||''||EXTRACT(YEAR FROM h0.htddata)
    from    workflow.historicodocumento h0
    inner join demandas.demanda d0 on h0.docid = d0.docid
    where    d.dmdid = d0.dmdid
    order by h0.htddata desc limit 1)
*/
   as dpeid,   
   sum(cast (pt.tsppontuacao as bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END)) as dshqtde,
   (sum(cast (pt.tsppontuacao as bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END))* COALESCE(crtvlponto,0) ) as valor,
   EXTRACT(WEEK FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as semana
/*			EXTRACT(WEEK FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as semana
*/	
FROM
   demandas.demanda d
      LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
      LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
--      LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
      LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
--      LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
--      LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--      LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--      LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--      LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
--      LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--      LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--      LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--      LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--      LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
      LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
	  /*
      LEFT JOIN ( select 
                     d1.dmdid, 
                     to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, 
                     max(a.htddata) as datasituacao
                  from 	workflow.historicodocumento a
                     inner join demandas.demanda d1 on a.docid = d1.docid
                  group by d1.dmdid order by 2 desc
                ) as dmd1 ON dmd1.dmdid = d.dmdid
      */
      LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
      
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')  				  	 	 
AND  ed.esdid  IN  (95,109,170)  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, semana, crtvlponto
ORDER BY dpeid, semana";
	
	$datas = $db->carregar($sql);

	if($datas[0]) {
		$i=1;
		foreach($datas as $data) {
			if(!$_x_ax_[$data['semana']]['semana']) {
				$_x_ax_[$data['semana']]['semana']  = "SEMANA ".$i;
				$i++;
			}
			$_x_ax_[$data['semana']]['qtde'] = $data['dshqtde'];
			$_x_ax_[$data['semana']]['valor'] = $data['valor'];
		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['semana'];
			$data_1[] = $d['qtde'];
			$totalizador['qtde'] += $d['qtde'];
			$data_2[] = $d['valor'];
			$totalizador['valor'] += $d['valor'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['qtde']/count($eixo_x));
		$data_2[] = round($totalizador['valor']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['qtde']/count($eixo_x));
		$dat_2 = round($totalizador['valor']/count($eixo_x));
		$eix_x = "MÉDIA";
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['qtde'];
		$data_2[] = $totalizador['valor'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['qtde'];
		$dat_2 = $totalizador['valor'];
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "TOTAL";

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("PONTUAÇÃO TOTAL POR SEMANA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Pontuação");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Valor(R$)");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormatCallback('barValueFormat'); 
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));
	// Set color for the frame of each bar
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}

/*
 * Pontuação total por mês
 */
function pontuacaototal_mes() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   (select EXTRACT(MONTH FROM h0.htddata)||''||EXTRACT(YEAR FROM h0.htddata)
    from    workflow.historicodocumento h0
    inner join demandas.demanda d0 on h0.docid = d0.docid
    where    d.dmdid = d0.dmdid
    order by h0.htddata desc limit 1)
*/
   AS dpeid,   
   SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END)) AS dshqtde,
   (SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END))* COALESCE(crtvlponto,0) ) AS valor,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS mes,
   EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS ano
/*
            EXTRACT(MONTH FROM (select  h0.htddata
                            from    workflow.historicodocumento h0
                            inner join demandas.demanda d0 on h0.docid = d0.docid
                            where   d.dmdid = d0.dmdid 
                            order by h0.htddata desc 
                            limit 1)) as mes,
                            
            EXTRACT(YEAR FROM (select  h0.htddata
                            from    workflow.historicodocumento h0
                            inner join demandas.demanda d0 on h0.docid = d0.docid
                            where   d.dmdid = d0.dmdid 
                            order by h0.htddata desc 
                            limit 1)) as ano
*/      
FROM
   demandas.demanda d
      LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
      LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
--      LEFT JOIN demandas.prioridade p ON p.priid = d.priid                     
      LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid  
--      LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
--      LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--      LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--      LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--      LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
--      LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--      LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--      LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--      LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--      LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
      LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
	  /*
      LEFT JOIN ( SELECT 
                     d1.dmdid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                     MAX(a.htddata) AS datasituacao
                  FROM  workflow.historicodocumento a
                     INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                  GROUP BY d1.dmdid ORDER BY 2 DESC
                ) AS dmd1 ON dmd1.dmdid = d.dmdid
	  */
      LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim 
      
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                           
AND  ed.esdid  IN  (95,109,170)
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, ano, mes, crtvlponto
ORDER BY ano, mes";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['dpeid']]['mes']) {
				$_x_ax_[$data['dpeid']]['mes']  = trim($db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$data['mes']."'"))."/".$data['ano'];
				$_x_ax_[$data['dpeid']]['qtde'] = $data['dshqtde'];
				$_x_ax_[$data['dpeid']]['valor'] = $data['valor'];
			}
			else{
				$_x_ax_[$data['dpeid']]['qtde'] += $data['dshqtde'];
				$_x_ax_[$data['dpeid']]['valor'] += $data['valor'];
			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['qtde'];
			$totalizador['qtde'] += $d['qtde'];
			$data_2[] = $d['valor'];
			$totalizador['valor'] += $d['valor'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['qtde']/count($eixo_x));
		$data_2[] = round($totalizador['valor']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['qtde']/count($eixo_x));
		$dat_2 = round($totalizador['valor']/count($eixo_x));
		$eix_x = "MÉDIA";
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['qtde'];
		$data_2[] = $totalizador['valor'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['qtde'];
		$dat_2 = $totalizador['valor'];
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "TOTAL";

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("PONTUAÇÃO TOTAL POR MÊS (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD, TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Pontuação");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Valor(R$)");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b2plot->value->SetFormatCallback('barValueFormat'); 
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));
	// Set color for the frame of each bar
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}




/*
 * Pontuação total por setor
 */
function pontuacaototal_setor() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   (select EXTRACT(MONTH FROM h0.htddata)||''||EXTRACT(YEAR FROM h0.htddata)
    from    workflow.historicodocumento h0
    inner join demandas.demanda d0 on h0.docid = d0.docid
    where    d.dmdid = d0.dmdid
    order by h0.htddata desc limit 1)
*/
   AS dpeid,   
   SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END)) AS dshqtde,
   (SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END))* COALESCE(crtvlponto,0) ) AS valor,
   uni.unaid tidid1 ,
   UPPER(uni.unasigla) as nome
   
FROM
   demandas.demanda d
      LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
      LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
--      LEFT JOIN demandas.prioridade p ON p.priid = d.priid                     
      LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid  
--      LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
--      LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--      LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--      LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--      LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
      LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--      LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--      LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--      LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--      LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
      LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
	  /*
      LEFT JOIN ( SELECT 
                     d1.dmdid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                     MAX(a.htddata) AS datasituacao
                  FROM  workflow.historicodocumento a
                     INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                  GROUP BY d1.dmdid ORDER BY 2 DESC
                ) AS dmd1 ON dmd1.dmdid = d.dmdid
	   */	
	   LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
                
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                           
AND  ed.esdid  IN  (95,109,170)
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, tidid1, UPPER(uni.unasigla), crtvlponto 
ORDER BY UPPER(uni.unasigla), dpeid, tidid1";
	
	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['tidid1']]['nome']) {
				$_x_ax_[$data['tidid1']]['nome']  = $data['nome'];
			}
			$_x_ax_[$data['tidid1']]['qtde'] = $data['dshqtde'];
			$_x_ax_[$data['tidid1']]['valor'] = $data['valor'];
		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['qtde'];
			$totalizador['qtde'] += $d['qtde'];
			$data_2[] = $d['valor'];
			$totalizador['valor'] += $d['valor'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['qtde']/count($eixo_x));
		$data_2[] = round($totalizador['valor']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['qtde']/count($eixo_x));
		$dat_2 = round($totalizador['valor']/count($eixo_x));
		$eix_x = "MÉDIA";
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['qtde'];
		$data_2[] = $totalizador['valor'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['qtde'];
		$dat_2 = $totalizador['valor'];
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "TOTAL";

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("PONTUAÇÃO TOTAL POR SETOR (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Pontuação");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_PESSOA);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Valor(R$)");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_PESSOA);
	$b2plot->value->SetFormatCallback('barValueFormat'); 
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));
	// Set color for the frame of each bar
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}



/*
 * Pontuação total por tipo serviço
 */
function pontuacaototal_tiposervico() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   (select EXTRACT(MONTH FROM h0.htddata)||''||EXTRACT(YEAR FROM h0.htddata)
    from    workflow.historicodocumento h0
    inner join demandas.demanda d0 on h0.docid = d0.docid
    where    d.dmdid = d0.dmdid
    order by h0.htddata desc limit 1)
*/
   AS dpeid,   
   SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END)) AS dshqtde,
   (SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END))* COALESCE(crtvlponto,0) ) AS valor,
   t.tipid tidid1 ,
   UPPER(t.tipnome) AS nome
   
FROM
   demandas.demanda d
      LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
      LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
--      LEFT JOIN demandas.prioridade p ON p.priid = d.priid                     
      LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid  
--      LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
--      LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--      LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--      LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--      LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
--      LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--      LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--      LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--      LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--      LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
      LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
	  /*
      LEFT JOIN ( SELECT 
                     d1.dmdid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                     MAX(a.htddata) AS datasituacao
                  FROM  workflow.historicodocumento a
                     INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                  GROUP BY d1.dmdid ORDER BY 2 DESC
                ) AS dmd1 ON dmd1.dmdid = d.dmdid
	  */
      --pega os tipos de serviços mais requisitados           
      INNER JOIN (SELECT COUNT(d.dmdid) AS total, t.tipid 
                  FROM demandas.demanda d
                     INNER JOIN demandas.tiposervico t ON t.tipid = d.tipid AND t.ordid = 3 AND t.tipstatus = 'A'
                  GROUP BY t.tipid ORDER BY total DESC limit 7
                 ) AS tip ON tip.tipid = t.tipid                      

      LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
                 
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                           
AND  ed.esdid  IN  (95,109,170)  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, tidid1, UPPER(t.tipnome), crtvlponto 
ORDER BY UPPER(t.tipnome), dpeid, tidid1";
	
	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['tidid1']]['nome']) {
				$_x_ax_[$data['tidid1']]['nome']  = $data['nome'];
			}
			$_x_ax_[$data['tidid1']]['qtde'] = $data['dshqtde'];
			$_x_ax_[$data['tidid1']]['valor'] = $data['valor'];
		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['qtde'];
			$totalizador['qtde'] += $d['qtde'];
			$data_2[] = $d['valor'];
			$totalizador['valor'] += $d['valor'];
		}
	}

	if($_REQUEST['media']=="1") {
		if(count($eixo_x)>0){
			$data_1[] = round($totalizador['qtde']/count($eixo_x));
			$data_2[] = round($totalizador['valor']/count($eixo_x));
		}else{
			$data_1[] = 0;
			$data_2[] = 0;
		}
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		if(count($eixo_x)>0){
			$dat_1 = round($totalizador['qtde']/count($eixo_x));
			$dat_2 = round($totalizador['valor']/count($eixo_x));
		}else{
			$dat_1 = 0;
			$dat_2 = 0;
		}
		$eix_x = "MÉDIA";
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['qtde'];
		$data_2[] = $totalizador['valor'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['qtde'];
		$dat_2 = $totalizador['valor'];
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "TOTAL";

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("PONTUAÇÃO TOTAL POR TIPO DE SERVIÇO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Pontuação");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_PESSOA);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Valor(R$)");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_PESSOA);
	$b2plot->value->SetFormatCallback('barValueFormat'); 
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));
	// Set color for the frame of each bar
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}


/*
 * Pontuação prioridade por pessoa
 */

function pontuacaoprioridade_pessoa() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   EXTRACT(MONTH FROM ( SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1)) 
                        || '' ||
   EXTRACT(YEAR FROM (  SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1))
*/
   AS dpeid, 
   p.pridsc AS prioridade,

   --sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
   COUNT(d.dmdid) AS qtde,

   d.usucpfexecutor AS tidid1,
   UPPER(u2.usunome) AS nome 

FROM
   demandas.demanda d
      LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
--      LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
      LEFT JOIN demandas.prioridade p ON p.priid = d.priid                    
      LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
--      LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
      LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--      LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--      LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--      LEFT JOIN demandas.andaratendimento aa ON l.andid = aa.andid
--      LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--      LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--      LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--      LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--      LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
--      --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
--      LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
/*
      LEFT JOIN ( SELECT a.docid, 
                     MAX(a.hstid) AS hstid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                     to_char(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                     MAX(htddata) AS dataatendfinalizado                      
                  FROM    workflow.historicodocumento a
                  INNER JOIN workflow.documento c ON c.docid = a.docid and c.tpdid in (31,35)
                  WHERE a.aedid IN (146, 191) 
                  GROUP BY a.docid
                ) AS hst ON hst.docid = d.docid

      LEFT JOIN ( SELECT d1.dmdid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                     MAX(a.htddata) AS datasituacao
                  FROM    workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                  GROUP BY d1.dmdid ORDER BY 2 DESC
                ) AS dmd1 ON dmd1.dmdid = d.dmdid
*/
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, tidid1, p.pridsc, UPPER(u2.usunome)     
ORDER BY UPPER(u2.usunome), dpeid, tidid1";


	$datas = $db->carregar($sql);


	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['tidid1']]['nome']) {
				$_x_ax_[$data['tidid1']]['nome']  = $data['nome'];
				$_x_ax_[$data['tidid1']]['baixa'] = 0;
				$_x_ax_[$data['tidid1']]['alta']  = 0;
				$_x_ax_[$data['tidid1']]['media'] = 0;
			}

			switch($data['prioridade']) {
				case 'Baixa':
					$_x_ax_[$data['tidid1']]['baixa'] = $data['qtde'];
					break;
				case 'Alta':
					$_x_ax_[$data['tidid1']]['alta'] = $data['qtde'];
					break;
				case 'Média':
					$_x_ax_[$data['tidid1']]['media'] = $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['baixa'];
			$totalizador['baixa'] += $d['baixa'];
			$data_2[] = $d['media'];
			$totalizador['media'] += $d['media'];
			$data_3[] = $d['alta'];
			$totalizador['alta'] += $d['alta'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['baixa']/count($eixo_x));
		$data_2[] = round($totalizador['media']/count($eixo_x));
		$data_3[] = round($totalizador['alta']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['baixa']/count($eixo_x));
		$dat_2 = round($totalizador['media']/count($eixo_x));
		$dat_3 = round($totalizador['alta']/count($eixo_x));
		unset($data_1,$data_2,$data_3, $eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "MÉDIA";

	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['baixa'];
		$data_2[] = $totalizador['media'];
		$data_3[] = $totalizador['alta'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['baixa'];
		$dat_2 = $totalizador['media'];
		$dat_3 = $totalizador['alta'];
		unset($data_1,$data_2,$data_3, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("QUANTIDADE POR PRIORIDADE / PESSOA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD, TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Baixa");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Média");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');
	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Alta");
	$b3plot->SetFillColor("red");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot));
	// ...and add it to the graPH
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}


/*
 * Pontuação prioridade por semana
 */
function pontuacaoprioridade_semana() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   EXTRACT(MONTH FROM ( SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1)) 
                        || '' ||
   EXTRACT(YEAR FROM (  SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1))
*/
   AS dpeid, 
   p.pridsc as prioridade,
   --sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
   count(d.dmdid) as qtde,
   EXTRACT(WEEK FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as semana
/*
   EXTRACT(WEEK FROM (select  h0.htddata
                      from workflow.historicodocumento h0
                      inner join demandas.demanda d0 on h0.docid = d0.docid
                      where d.dmdid = d0.dmdid 
                      order by h0.htddata desc 
                      limit 1)) as semana
*/
FROM
   demandas.demanda d
      LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
--      LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
      LEFT JOIN demandas.prioridade p ON p.priid = d.priid                    
      LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
      LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
--      LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
      LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--      LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--      LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--      LEFT JOIN demandas.andaratendimento aa ON l.andid = aa.andid
--      LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--      LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--      LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--      LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--      LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
      -- --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
--      LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
/*
      LEFT JOIN ( SELECT a.docid, 
                     MAX(a.hstid) AS hstid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                     to_char(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                     MAX(htddata) AS dataatendfinalizado                      
                  FROM    workflow.historicodocumento a
                  INNER JOIN workflow.documento c ON c.docid = a.docid and c.tpdid in (31,35)
                  WHERE a.aedid IN (146, 191) 
                  GROUP BY a.docid
                ) AS hst ON hst.docid = d.docid

      LEFT JOIN ( SELECT d1.dmdid, 
                     to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                     MAX(a.htddata) AS datasituacao
                  FROM    workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                  GROUP BY d1.dmdid ORDER BY 2 DESC
                ) AS dmd1 ON dmd1.dmdid = d.dmdid
*/
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
group by dpeid, semana, p.pridsc 	
ORDER BY semana, dpeid";


	$datas = $db->carregar($sql);


	if($datas[0]) {
		$i=1;
		foreach($datas as $data) {

			if(!$_x_ax_[$data['semana']]['semana']) {
				$_x_ax_[$data['semana']]['semana']  = "SEMANA ".$i;
				$_x_ax_[$data['semana']]['baixa'] = 0;
				$_x_ax_[$data['semana']]['alta']  = 0;
				$_x_ax_[$data['semana']]['media'] = 0;
				$i++;
			}

			switch($data['prioridade']) {
				case 'Baixa':
					$_x_ax_[$data['semana']]['baixa'] = $data['qtde'];
					break;
				case 'Alta':
					$_x_ax_[$data['semana']]['alta'] = $data['qtde'];
					break;
				case 'Média':
					$_x_ax_[$data['semana']]['media'] = $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['semana'];
			$data_1[] = $d['baixa'];
			$totalizador['baixa'] += $d['baixa'];
			$data_2[] = $d['media'];
			$totalizador['media'] += $d['media'];
			$data_3[] = $d['alta'];
			$totalizador['alta'] += $d['alta'];
		}
	}
	
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['baixa']/count($eixo_x));
		$data_2[] = round($totalizador['media']/count($eixo_x));
		$data_3[] = round($totalizador['alta']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['baixa']/count($eixo_x));
		$dat_2 = round($totalizador['media']/count($eixo_x));
		$dat_3 = round($totalizador['alta']/count($eixo_x));
		unset($data_1,$data_2,$data_3, $eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "MÉDIA";

	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['baixa'];
		$data_2[] = $totalizador['media'];
		$data_3[] = $totalizador['alta'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['baixa'];
		$dat_2 = $totalizador['media'];
		$dat_3 = $totalizador['alta'];
		unset($data_1,$data_2,$data_3, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("QUANTIDADE POR PRIORIDADE / SEMANA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Baixa");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Média");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Alta");
	$b3plot->SetFillColor("red");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}


/*
 * Pontuação prioridade por mês
 */
function pontuacaoprioridade_mes() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   EXTRACT(MONTH FROM ( SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1)) 
                        || '' ||
   EXTRACT(YEAR FROM (  SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1))
*/
   AS dpeid, 
   p.pridsc as prioridade,
   --sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
   count(d.dmdid) as qtde,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS mes,
   EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS ano
/*		EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano,
		EXTRACT(MONTH FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as mes
*/
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
--   LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
   LEFT JOIN demandas.prioridade p ON p.priid = d.priid                    
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
--   LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
   LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--   LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--   LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--   LEFT JOIN demandas.andaratendimento aa ON l.andid = aa.andid
--   LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--   LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--   LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--   LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--   LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
--   --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
--   LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
/*
   LEFT JOIN ( SELECT a.docid, 
                  MAX(a.hstid) AS hstid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                  to_char(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                  MAX(htddata) AS dataatendfinalizado                      
               FROM    workflow.historicodocumento a
               INNER JOIN workflow.documento c ON c.docid = a.docid and c.tpdid in (31,35)
               WHERE a.aedid IN (146, 191) 
               GROUP BY a.docid
             ) AS hst ON hst.docid = d.docid

   LEFT JOIN ( SELECT d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
*/
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
group by dpeid, ano, mes, p.pridsc 	
ORDER BY ano, mes, dpeid";


	$datas = $db->carregar($sql);


	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['mes']]['mes']) {
				$_x_ax_[$data['mes']]['mes']  = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer ='".$data['mes']."'")."/".$data['ano'];
				$_x_ax_[$data['mes']]['baixa'] = 0;
				$_x_ax_[$data['mes']]['alta']  = 0;
				$_x_ax_[$data['mes']]['media'] = 0;
			}

			switch($data['prioridade']) {
				case 'Baixa':
					$_x_ax_[$data['mes']]['baixa'] = $data['qtde'];
					break;
				case 'Alta':
					$_x_ax_[$data['mes']]['alta'] = $data['qtde'];
					break;
				case 'Média':
					$_x_ax_[$data['mes']]['media'] = $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['baixa'];
			$totalizador['baixa'] += $d['baixa'];
			$data_2[] = $d['media'];
			$totalizador['media'] += $d['media'];
			$data_3[] = $d['alta'];
			$totalizador['alta'] += $d['alta'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['baixa']/count($eixo_x));
		$data_2[] = round($totalizador['media']/count($eixo_x));
		$data_3[] = round($totalizador['alta']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['baixa']/count($eixo_x));
		$dat_2 = round($totalizador['media']/count($eixo_x));
		$dat_3 = round($totalizador['alta']/count($eixo_x));
		unset($data_1,$data_2,$data_3, $eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "MÉDIA";

	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['baixa'];
		$data_2[] = $totalizador['media'];
		$data_3[] = $totalizador['alta'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['baixa'];
		$dat_2 = $totalizador['media'];
		$dat_3 = $totalizador['alta'];
		unset($data_1,$data_2,$data_3, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("QUANTIDADE POR PRIORIDADE / MÊS(".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,PLOT_MES); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Baixa");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Média");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b2plot->value->SetFormat('%01.0f');

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Alta");
	$b3plot->SetFillColor("red");
	$b3plot->value->Show();
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b3plot->value->SetFormat('%01.0f');


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}


/*
 * Pontuação prioridade por semana
 */
function pontuacaoprioridade_pizza() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT

		extract(MONTH FROM (select  h0.htddata
						from 	workflow.historicodocumento h0
						inner join demandas.demanda d0 on h0.docid = d0.docid
						where 	d.dmdid = d0.dmdid 
						order by h0.htddata desc 
						limit 1)) 
		|| '' ||
		EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1))
		as dpeid, 

		p.pridsc as prioridade,
		EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano,
		
		--sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
		count(d.dmdid) as qtde,
		EXTRACT(MONTH FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as mes
		 

		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
		 
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 /*
		 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
						from 	workflow.historicodocumento a
							inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
					where a.aedid in (146, 191) 
					group by a.docid
					) ) as hst ON hst.docid = d.docid
					
				
		 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
						from 	workflow.historicodocumento a
							inner join demandas.demanda d1 on a.docid = d1.docid
				  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
		 */
 	 
		 WHERE d.dmdstatus = 'A'  
		 AND od.ordid  IN  ('3')  				  	 	 
		 AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')	
		 AND  ed.esdid  IN  (95,109,170)  
		 --AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
		 AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
		 group by dpeid, ano, mes, p.pridsc 	
		 ORDER BY ano, mes, dpeid";


	$datas = $db->carregar($sql);


	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['mes']]['mes']) {
				$_x_ax_[$data['mes']]['mes']  = $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod::integer ='".$data['mes']."'")."/".$data['ano'];
				$_x_ax_[$data['mes']]['baixa'] = 0;
				$_x_ax_[$data['mes']]['alta']  = 0;
				$_x_ax_[$data['mes']]['media'] = 0;
			}

			switch($data['prioridade']) {
				case 'Baixa':
					$_x_ax_[$data['mes']]['baixa'] = $data['qtde'];
					break;
				case 'Alta':
					$_x_ax_[$data['mes']]['alta'] = $data['qtde'];
					break;
				case 'Média':
					$_x_ax_[$data['mes']]['media'] = $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['baixa'];
			$totalizador['baixa'] += $d['baixa'];
			$data_2[] = $d['media'];
			$totalizador['media'] += $d['media'];
			$data_3[] = $d['alta'];
			$totalizador['alta'] += $d['alta'];
		}
	}
	
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	require_once ('../../includes/jpgraph/jpgraph_pie3d.php');
	
	$data = array($totalizador['baixa'],
				  $totalizador['media'],
				  $totalizador['alta']);
	
	$graph = new PieGraph(800,440);
	$graph->SetShadow();
	$graph->title->Set("QUANTIDADE POR PRIORIDADE - PIZZA (".$dataini." a ".$datafim.")");
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,8); 
	$xx = new PiePlot3D($data);
	$cores = array("green","yellow","red");
	$xx->SetSliceColors($cores);
	$xx->value->SetFormat('%01.1f%%');
	$xx->value->HideZero();
	$xx->SetSize(0.5);
	$xx->SetCenter(0.45);
	$legendas = array("Baixa","Média","Alta");
	$xx->SetLegends($legendas);
	//$xx->ExplodeAll(10);
	$xx->SetShadow();
	$graph->Add($xx);
	$graph->Stroke();
}




/*
 * Pontuação prioridade por setor
 */
function pontuacaoprioridade_setor() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   EXTRACT(MONTH FROM ( SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1)) 
                        || '' ||
   EXTRACT(YEAR FROM (  SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1))
*/
   AS dpeid, 
   p.pridsc as prioridade,
   --sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
   count(d.dmdid) as qtde,
   uni.unaid tidid1 ,
   UPPER(uni.unasigla) as nome 

FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
--   LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
   LEFT JOIN demandas.prioridade p ON p.priid = d.priid                    
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
--   LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
--   LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--   LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--   LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--   LEFT JOIN demandas.andaratendimento aa ON l.andid = aa.andid
   LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--   LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--   LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--   LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--   LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
--   --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
--   LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
/*
   LEFT JOIN ( SELECT a.docid, 
                  MAX(a.hstid) AS hstid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                  to_char(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                  MAX(htddata) AS dataatendfinalizado                      
               FROM    workflow.historicodocumento a
               INNER JOIN workflow.documento c ON c.docid = a.docid and c.tpdid in (31,35)
               WHERE a.aedid IN (146, 191) 
               GROUP BY a.docid
             ) AS hst ON hst.docid = d.docid

   LEFT JOIN ( SELECT d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
*/
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, tidid1, p.pridsc, UPPER(uni.unasigla) 	
ORDER BY UPPER(uni.unasigla), dpeid, tidid1";


	$datas = $db->carregar($sql);


	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['tidid1']]['nome']) {
				$_x_ax_[$data['tidid1']]['nome']  = $data['nome'];
				$_x_ax_[$data['tidid1']]['baixa'] = 0;
				$_x_ax_[$data['tidid1']]['alta']  = 0;
				$_x_ax_[$data['tidid1']]['media'] = 0;
			}

			switch($data['prioridade']) {
				case 'Baixa':
					$_x_ax_[$data['tidid1']]['baixa'] = $data['qtde'];
					break;
				case 'Alta':
					$_x_ax_[$data['tidid1']]['alta'] = $data['qtde'];
					break;
				case 'Média':
					$_x_ax_[$data['tidid1']]['media'] = $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['baixa'];
			$totalizador['baixa'] += $d['baixa'];
			$data_2[] = $d['media'];
			$totalizador['media'] += $d['media'];
			$data_3[] = $d['alta'];
			$totalizador['alta'] += $d['alta'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['baixa']/count($eixo_x));
		$data_2[] = round($totalizador['media']/count($eixo_x));
		$data_3[] = round($totalizador['alta']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['baixa']/count($eixo_x));
		$dat_2 = round($totalizador['media']/count($eixo_x));
		$dat_3 = round($totalizador['alta']/count($eixo_x));
		unset($data_1,$data_2,$data_3, $eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "MÉDIA";

	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['baixa'];
		$data_2[] = $totalizador['media'];
		$data_3[] = $totalizador['alta'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['baixa'];
		$dat_2 = $totalizador['media'];
		$dat_3 = $totalizador['alta'];
		unset($data_1,$data_2,$data_3, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("QUANTIDADE POR PRIORIDADE / SETOR (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD, TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Baixa");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Média");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');
	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Alta");
	$b3plot->SetFillColor("red");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot));
	// ...and add it to the graPH
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}



/*
 * Pontuação prioridade por tipo servico
 */
function pontuacaoprioridade_tiposervico() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')
/*   EXTRACT(MONTH FROM ( SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1)) 
                        || '' ||
   EXTRACT(YEAR FROM (  SELECT  h0.htddata
                        FROM    workflow.historicodocumento h0
                        INNER JOIN demandas.demanda d0 ON h0.docid = d0.docid
                        WHERE   d.dmdid = d0.dmdid 
                        ORDER BY h0.htddata DESC 
                        limit 1))
*/
   AS dpeid, 
   p.pridsc as prioridade,
   --sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
   count(d.dmdid) as qtde,
   t.tipid as tidid1 ,
   UPPER(t.tipnome) as nome 

FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
--   LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
   LEFT JOIN demandas.prioridade p ON p.priid = d.priid                    
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
--   LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
--   LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
--   LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
--   LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
--   LEFT JOIN demandas.andaratendimento aa ON l.andid = aa.andid
   LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
--   LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
--   LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
--   LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
--   LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
--   --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
--   LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
/*
   LEFT JOIN ( SELECT a.docid, 
                  MAX(a.hstid) AS hstid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                  to_char(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                  MAX(htddata) AS dataatendfinalizado                      
               FROM    workflow.historicodocumento a
               INNER JOIN workflow.documento c ON c.docid = a.docid and c.tpdid in (31,35)
               WHERE a.aedid IN (146, 191) 
               GROUP BY a.docid
             ) AS hst ON hst.docid = d.docid

   LEFT JOIN ( SELECT d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
*/
      --pega os tipos de serviços mais requisitados           
   INNER JOIN (SELECT COUNT(d.dmdid) AS total, t.tipid 
               FROM demandas.demanda d
               INNER JOIN demandas.tiposervico t ON t.tipid = d.tipid AND t.ordid = 3 AND t.tipstatus = 'A'
               GROUP BY t.tipid ORDER BY total DESC limit 7
              ) AS tip ON tip.tipid = t.tipid                      
                               
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, tidid1, p.pridsc, UPPER(t.tipnome) 	
ORDER BY UPPER(t.tipnome), dpeid, tidid1";


	$datas = $db->carregar($sql);


	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['tidid1']]['nome']) {
				$_x_ax_[$data['tidid1']]['nome']  = $data['nome'];
				$_x_ax_[$data['tidid1']]['baixa'] = 0;
				$_x_ax_[$data['tidid1']]['alta']  = 0;
				$_x_ax_[$data['tidid1']]['media'] = 0;
			}

			switch($data['prioridade']) {
				case 'Baixa':
					$_x_ax_[$data['tidid1']]['baixa'] = $data['qtde'];
					break;
				case 'Alta':
					$_x_ax_[$data['tidid1']]['alta'] = $data['qtde'];
					break;
				case 'Média':
					$_x_ax_[$data['tidid1']]['media'] = $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['baixa'];
			$totalizador['baixa'] += $d['baixa'];
			$data_2[] = $d['media'];
			$totalizador['media'] += $d['media'];
			$data_3[] = $d['alta'];
			$totalizador['alta'] += $d['alta'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['baixa']/count($eixo_x));
		$data_2[] = round($totalizador['media']/count($eixo_x));
		$data_3[] = round($totalizador['alta']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['baixa']/count($eixo_x));
		$dat_2 = round($totalizador['media']/count($eixo_x));
		$dat_3 = round($totalizador['alta']/count($eixo_x));
		unset($data_1,$data_2,$data_3, $eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "MÉDIA";

	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['baixa'];
		$data_2[] = $totalizador['media'];
		$data_3[] = $totalizador['alta'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['baixa'];
		$dat_2 = $totalizador['media'];
		$dat_3 = $totalizador['alta'];
		unset($data_1,$data_2,$data_3, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("QUANTIDADE POR PRIORIDADE / TIPO DE SERVIÇO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD, TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Baixa");
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Média");
	$b2plot->SetFillColor("blue");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');
	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Alta");
	$b3plot->SetFillColor("red");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot));
	// ...and add it to the graPH
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}



function atendimentodemandas_pessoa() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   LPAD(CAST(d.dmdid AS VARCHAR), GREATEST(LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
   T.ordid AS ordid,
   cel.celnome AS celula,
   CASE 
      WHEN doc.esdid IN (100,110) THEN '' -- cancelada
      WHEN doc.esdid IN (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE TO_CHAR(now()::TIMESTAMP,'YYYY-MM-DD HH24:MI:00')
   END AS datadocfinalizada,
   CASE 
      WHEN doc.esdid IN (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE ''
   END AS dataconclusao,
   --datasit AS datasituacao, 
   ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,      
   '' AS  prazoatendimento,
   '' AS  tempodecorrido,
   '' AS duracaoatendminutos,
   '' AS  tempopausa,
   TO_CHAR(d.dmddatafimprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatafimprevatendimento, 
   TO_CHAR(d.dmddatainiprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatainiprevatendimento,
   CASE 
      WHEN u2.usunome <> '' THEN u2.usunome
      ELSE 'Não informado'
   END AS tecnico,       
   d.dmdhorarioatendimento,
   d.usucpfexecutor

FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid     
   LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
   LEFT JOIN  demandas.celula AS cel ON cel.celid = D.CELID --smc.celid
   /*
   LEFT JOIN ( SELECT 
                  a.docid, 
                  MAX(a.hstid) AS hstid, 
                  TO_CHAR(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                  TO_CHAR(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                  MAX(htddata) AS dataatendfinalizado                       
               FROM     workflow.historicodocumento a
               WHERE a.aedid IN (146, 191) 
               GROUP BY a.docid
             ) AS hst ON hst.docid = d.docid
   
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  TO_CHAR(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM  workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')
AND  ed.esdid  IN  (95,109,170) 
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'   
ORDER BY  tecnico, datadocfinalizada";
	$dados = $db->carregar($sql);

	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['usucpfexecutor']]['tecnico']) {
				$dados_bruto[$dado['usucpfexecutor']]['tecnico'] = $dado['tecnico'];
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}



				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['usucpfexecutor']]['vermelho']++;
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['usucpfexecutor']]['azul']++;
				}
			}

		}
	}
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$nome = explode(' ', $d['tecnico']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['tecnico'];
			$data_1[] = $d['azul'];
			$totalizador['azul'] += $d['azul'];
			$data_2[] = $d['vermelho'];
			$totalizador['vermelho'] += $d['vermelho'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['azul']/count($eixo_x));
		$data_2[] = round($totalizador['vermelho']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['azul']/count($eixo_x));
		$dat_2 = round($totalizador['vermelho']/count($eixo_x));
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['azul'];
		$data_2[] = $totalizador['vermelho'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['azul'];
		$dat_2 = $totalizador['vermelho'];
		unset($data_1,$data_2,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,190,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO POR TÉCNICO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,8);
	$graph->title->SetColor("darkred");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_FONT0,FS_NORMAL,8);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(90);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Dentro do prazo");
	$b1plot->SetFillColor("blue");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Fora do prazo");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();



}


function atendimentodemandas_semana() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   LPAD(CAST(d.dmdid AS VARCHAR), GREATEST(LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
   t.ordid as ordid,
   CASE 
      WHEN doc.esdid in (100,110) THEN '' -- cancelada
      WHEN doc.esdid in (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE to_char(now()::timestamp,'YYYY-MM-DD HH24:MI:00')
   END as datadocfinalizada,

   CASE 
      WHEN doc.esdid in (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE ''
   END as dataconclusao,
							  
   --datasit as datasituacao,
   ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,		  
   '' as  prazoatendimento,
   '' as  tempodecorrido,
   '' as duracaoatendminutos,
   '' as  tempopausa,
   to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
   to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
   d.dmdhorarioatendimento,
   EXTRACT(WEEK FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where aedid in (146, 191) and docid = d.docid )) as semana,
   ed.esdid 
							 
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
   /*
   LEFT JOIN ( select 
                  a.docid, 
                  max(a.hstid) as hstid, 
                  to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, 
                  to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, 
                  max(htddata) as dataatendfinalizado						
               from 	workflow.historicodocumento a
               where a.aedid in (146, 191) 
               group by a.docid
             ) as hst ON hst.docid = d.docid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM  workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')
AND  ed.esdid  IN  (95,109,170) 
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
ORDER BY  semana, datadocfinalizada";

	$dados = $db->carregar($sql);

	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {
			$i=1;
			if(!$dados_bruto[$dado['semana']]['semana']) {
				$dados_bruto[$dado['semana']]['semana'] = "Semana ".$i;
				$i++;
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}
				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			
			//verifica se a situação é 'Validada Fora do Prazo' se sim, despreza o tempo da pausa
			if($dado['esdid'] == DEMANDA_ESTADO_VALIDADA_FORA_PRAZO) $tempototalpausa = 0; 
			
			
			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['semana']]['vermelho']++;
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['semana']]['azul']++;
				}
			}

		}
	}
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['semana'];
			$data_1[] = $d['azul'];
			$totalizador['azul'] += $d['azul'];
			$data_2[] = $d['vermelho'];
			$totalizador['vermelho'] += $d['vermelho'];
		}
	}
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['azul']/count($eixo_x));
		$data_2[] = round($totalizador['vermelho']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['azul']/count($eixo_x));
		$dat_2 = round($totalizador['vermelho']/count($eixo_x));
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['azul'];
		$data_2[] = $totalizador['vermelho'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['azul'];
		$dat_2 = $totalizador['vermelho'];
		unset($data_1,$data_2,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
	}
	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,190,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO POR SEMANA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,8);
	$graph->title->SetColor("darkred");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_FONT0,FS_NORMAL,8);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(90);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Dentro do prazo");
	$b1plot->SetFillColor("blue");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Fora do prazo");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();



}

function atendimentodemandas_mes() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   LPAD(CAST(d.dmdid AS VARCHAR), GREATEST(LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
   t.ordid as ordid,
   CASE 
      WHEN doc.esdid in (100,110) THEN '' -- cancelada
      WHEN doc.esdid in (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE to_char(now()::timestamp,'YYYY-MM-DD HH24:MI:00')
   END as datadocfinalizada,

   CASE 
      WHEN doc.esdid in (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE ''
   END as dataconclusao,
   '' as  prazoatendimento,
   '' as  tempodecorrido,
   '' as duracaoatendminutos,
   '' as  tempopausa,
   to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
   to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
   d.dmdhorarioatendimento,
   COALESCE(pt.tsppontuacao,'0') AS pontuacao,		 
   CASE 
      WHEN d.dmdqtde > 0 THEN d.dmdqtde
      ELSE '1'	 
   END AS qtdservico,
   ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC )) as mes,
   EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC )) as ano,
   ed.esdid,
   COALESCE(crtvlponto,0) as valorponto

FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
   LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
   /*
   LEFT JOIN ( select 
                  a.docid, 
                  max(a.hstid) as hstid, 
                  to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, 
                  to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, 
                  max(htddata) as dataatendfinalizado						
               from 	workflow.historicodocumento a
               where a.aedid in (146, 191) 
               group by a.docid
             ) as hst ON hst.docid = d.docid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM  workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */          
   LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con 
   			  on od.ordid=con.ordid 
   			  and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim				
						 	 
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')
AND  ed.esdid  IN  (95,109,170) 
AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
ORDER BY  ano, mes,  datadocfinalizada";


	$dados = $db->carregar($sql);
	
	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['mes']]['mes']) {
				$dados_bruto[$dado['mes']]['mes'] = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$dado['mes']."'")."/".$dado['ano'];
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}
				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			
			//verifica se a situação é 'Validada Fora do Prazo' se sim, despreza o tempo da pausa
			if($dado['esdid'] == DEMANDA_ESTADO_VALIDADA_FORA_PRAZO) $tempototalpausa = 0; 
			
			
			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['mes']]['vermelho']++;
					$dados_bruto[$dado['mes']]['valorvermelho'] = $dados_bruto[$dado['mes']]['valorvermelho'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['mes']]['azul']++;
					$dados_bruto[$dado['mes']]['valorazul'] = $dados_bruto[$dado['mes']]['valorazul'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
			}

		}
	}
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['azul'];
			$data_1_valorAzul[$d['azul']] = $d['valorazul'];
			$totalizador['azul'] += $d['azul'];
			$data_2[] = $d['vermelho'];
			$data_2_valorVermelho[$d['vermelho']] = $d['valorvermelho'];
			$totalizador['vermelho'] += $d['vermelho'];
		}
	}
	
	$arValores = array();
	if($data_1_valorAzul){
		foreach($data_1_valorAzul as $key=>$valorazul){
			$arValores['azul'][$key] = $valorazul;
		}
	}
	if($data_2_valorVermelho){
		foreach($data_2_valorVermelho as $key=>$valorvermelho){
			$arValores['vermelho'][$key] = $valorvermelho;
		}
	}
	
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['azul']/count($eixo_x));
		$data_2[] = round($totalizador['vermelho']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['azul']/count($eixo_x));
		$dat_2 = round($totalizador['vermelho']/count($eixo_x));
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['azul'];
		$data_2[] = $totalizador['vermelho'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['azul'];
		$dat_2 = $totalizador['vermelho'];
		unset($data_1,$data_2,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;		
		$data_2[] = $dat_2;
	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO POR MÊS (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Dentro do prazo");
	$b1plot->SetFillColor("blue");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetTypeGraph('azul');
	$b1plot->value->SetFormatCallbackParam('barValueFormat2',$arValores);
	
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Fora do prazo");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b2plot->value->SetTypeGraph('vermelho');
	$b2plot->value->SetFormatCallbackParam('barValueFormat2',$arValores);


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();

}



function atendimentodemandas_pizza() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   LPAD(CAST(d.dmdid AS VARCHAR), GREATEST(LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
   od.ordid AS ordid,
   CASE 
      WHEN doc.esdid IN (100,110) THEN '' -- cancelada
      WHEN doc.esdid IN (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE to_char(now()::TIMESTAMP,'YYYY-MM-DD HH24:MI:00')
   END AS datadocfinalizada,

   CASE 
      WHEN doc.esdid IN (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE ''
   END AS dataconclusao,
   '' AS  prazoatendimento,
   '' AS  tempodecorrido,
   '' AS duracaoatendminutos,
   '' AS  tempopausa,
   to_char(d.dmddatafimprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatafimprevatendimento, 
   to_char(d.dmddatainiprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatainiprevatendimento,
   d.dmdhorarioatendimento,
   ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC )) AS mes
                     
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
   LEFT JOIN demandas.prioridade p ON p.priid = d.priid                  
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid     
   /*
   LEFT JOIN ( SELECT 
                  a.docid, 
                  MAX(a.hstid) AS hstid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                  to_char(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                  MAX(htddata) AS dataatendfinalizado                       
               FROM     workflow.historicodocumento a
               WHERE a.aedid IN (146, 191) 
               GROUP BY a.docid
             ) AS hst ON hst.docid = d.docid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM  workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
     */               
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')
AND  ed.esdid  IN  (95,109,170) 
AND  ( SELECT MAX(htddata) FROM workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59' 
ORDER BY  mes, datadocfinalizada";

	$dados = $db->carregar($sql);

	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['mes']]['mes']) {
				$dados_bruto[$dado['mes']]['mes'] = $dado['mes'];
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}



				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['usucpfexecutor']]['vermelho']++;
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['usucpfexecutor']]['azul']++;
				}
			}

		}
	}
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['mes'];
			$totalizador['dentro'] += $d['azul'];
			$totalizador['fora'] += $d['vermelho'];
		}
	}

	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	require_once ('../../includes/jpgraph/jpgraph_pie3d.php');
	
	$data = array($totalizador['dentro'],
				  $totalizador['fora']);
	
	$graph = new PieGraph(800,440);
	$graph->SetShadow();
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO - PIZZA (".$dataini." a ".$datafim.")");
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,8); 
	$xx = new PiePlot3D($data);
	$cores = array("blue","red");
	$xx->SetSliceColors($cores);
	$xx->value->SetFormat('%01.1f%%');
	$xx->value->HideZero();
	$xx->SetSize(0.5);
	$xx->SetCenter(0.45);
	$legendas = array("Dentro do prazo (".$totalizador['dentro'].")","Fora do prazo (".$totalizador['fora'].")");
	$xx->SetLegends($legendas);
	//$xx->ExplodeAll(10);
	$xx->SetShadow();
	$graph->Add($xx);
	$graph->Stroke();

}



function atendimentodemandas_setor() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   LPAD(CAST(d.dmdid AS VARCHAR), GREATEST(LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
   t.ordid as ordid,
   CASE 
      WHEN doc.esdid in (100,110) THEN '' -- cancelada
      WHEN doc.esdid in (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE to_char(now()::timestamp,'YYYY-MM-DD HH24:MI:00')
   END as datadocfinalizada,

   CASE 
      WHEN doc.esdid in (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE ''
   END as dataconclusao,
   --datasit as datasituacao,
   ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,		  
   '' as  prazoatendimento,
   '' as  tempodecorrido,
   '' as duracaoatendminutos,
   '' as  tempopausa,
   to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
   to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
   upper(unasigla) as setor, 	
   d.dmdhorarioatendimento, 
   d.unaid
							 
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
   LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
   /*
   LEFT JOIN ( select 
                  a.docid, 
                  max(a.hstid) as hstid, 
                  to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, 
                  to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, 
                  max(htddata) as dataatendfinalizado						
               from 	workflow.historicodocumento a
               where a.aedid in (146, 191) 
               group by a.docid
             ) as hst ON hst.docid = d.docid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM  workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')
AND  ed.esdid  IN  (95,109,170) 
AND  ( SELECT MAX(htddata) FROM workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
ORDER BY  setor, datadocfinalizada";
	$dados = $db->carregar($sql);

	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['unaid']]['setor']) {
				$dados_bruto[$dado['unaid']]['setor'] = $dado['setor'];
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}



				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['unaid']]['vermelho']++;
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['unaid']]['azul']++;
				}
			}

		}
	}
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['setor'];
			$data_1[] = $d['azul'];
			$totalizador['azul'] += $d['azul'];
			$data_2[] = $d['vermelho'];
			$totalizador['vermelho'] += $d['vermelho'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['azul']/count($eixo_x));
		$data_2[] = round($totalizador['vermelho']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['azul']/count($eixo_x));
		$dat_2 = round($totalizador['vermelho']/count($eixo_x));
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['azul'];
		$data_2[] = $totalizador['vermelho'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['azul'];
		$dat_2 = $totalizador['vermelho'];
		unset($data_1,$data_2,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,190,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO POR SETOR (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,8);
	$graph->title->SetColor("darkred");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_FONT0,FS_NORMAL,8);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(90);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Dentro do prazo");
	$b1plot->SetFillColor("blue");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Fora do prazo");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();



}





function atendimentodemandas_tiposervico() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   LPAD(CAST(d.dmdid AS VARCHAR), GREATEST(LENGTH(CAST(d.dmdid AS VARCHAR)), 5), '0') AS nudemanda,
   t.ordid AS ordid,
   t.tipnome AS tipodemanda,
   CASE 
      WHEN doc.esdid IN (100,110) THEN '' -- cancelada
      WHEN doc.esdid IN (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE to_char(now()::TIMESTAMP,'YYYY-MM-DD HH24:MI:00')
   END AS datadocfinalizada,
   CASE 
      WHEN doc.esdid IN (93,95,109,111,170) THEN (
												  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
												  from 	workflow.historicodocumento
												  where aedid in (146, 191) and docid = d.docid
												 ) -- finalizada
      ELSE ''
   END AS dataconclusao,
   '' AS  prazoatendimento,
   '' AS  tempodecorrido,
   '' AS duracaoatendminutos,
   '' AS  tempopausa,
   to_char(d.dmddatafimprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatafimprevatendimento, 
   to_char(d.dmddatainiprevatendimento::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS dmddatainiprevatendimento,
   d.dmdhorarioatendimento,
   t.tipid,
   ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid     
   /*
   LEFT JOIN ( SELECT 
                  a.docid, 
                  MAX(a.hstid) AS hstid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'YYYY-MM-DD HH24:MI:00') AS datadoc, 
                  to_char(MAX(htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS dataconc, 
                  MAX(htddata) AS dataatendfinalizado                       
               FROM     workflow.historicodocumento a
               WHERE a.aedid IN (146, 191) 
               GROUP BY a.docid
             ) AS hst ON hst.docid = d.docid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM  workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */                 
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')  
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')
AND  ed.esdid  IN  (95,109,170) 
AND  ( SELECT MAX(htddata) FROM workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
ORDER BY  tipodemanda, datadocfinalizada";
	
	$dados = $db->carregar($sql);

	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['tipid']]['tipodemanda']) {
				$dados_bruto[$dado['tipid']]['tipodemanda'] = $dado['tipodemanda'];
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}



				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['tipid']]['vermelho']++;
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['tipid']]['azul']++;
				}
			}

		}
	}
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['tipodemanda'];
			$data_1[] = $d['azul'];
			$totalizador['azul'] += $d['azul'];
			$data_2[] = $d['vermelho'];
			$totalizador['vermelho'] += $d['vermelho'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['azul']/count($eixo_x));
		$data_2[] = round($totalizador['vermelho']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['azul']/count($eixo_x));
		$dat_2 = round($totalizador['vermelho']/count($eixo_x));
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['azul'];
		$data_2[] = $totalizador['vermelho'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['azul'];
		$dat_2 = $totalizador['vermelho'];
		unset($data_1,$data_2,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,190,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO POR TIPO DE SERVIÇO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_FONT1,FS_BOLD,8);
	$graph->title->SetColor("darkred");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_FONT0,FS_NORMAL,8);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(90);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Dentro do prazo");
	$b1plot->SetFillColor("blue");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Fora do prazo");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();



}





function avaliacaoportecnico_pessoa() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
   (CASE WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '1' THEN 'Ruim'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '2' THEN 'Regular'   
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '3' THEN 'Bom'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '4' THEN 'Ótimo'
      ELSE  'Não Avaliado'
    END)   AS avaliacao,
    --sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
    COUNT(d.dmdid) AS qtde,
    d.usucpfexecutor AS cpf,
    UPPER(u2.usunome) AS usunome,
    d.docid  
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
   LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
   /*
   LEFT JOIN ( SELECT 
                  MAX(a.avdid), 
                  a.dmdid, 
                  CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END AS avaliacao 
               FROM demandas.avaliacaodemanda a
               INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
               WHERE a.avdstatus='A' 
               GROUP BY a.avdgeral, a.dmdid, a.avnegatividade
             ) AS avd ON avd.dmdid = d.dmdid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
    */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')   
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, d.usucpfexecutor, UPPER(u2.usunome), avaliacao, d.docid
ORDER BY UPPER(u2.usunome)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['cpf']]['nome']) {
				$_x_ax_[$data['cpf']]['nome']  	 = $data['usunome'];
				$_x_ax_[$data['cpf']]['naval'] 	 = 0;
				$_x_ax_[$data['cpf']]['ruim'] 	 = 0;
				$_x_ax_[$data['cpf']]['regular'] = 0;
				$_x_ax_[$data['cpf']]['bom'] 	 = 0;
				$_x_ax_[$data['cpf']]['otimo']   = 0;
			}

			switch($data['avaliacao']) {
				case 'Não Avaliado':
					$_x_ax_[$data['cpf']]['naval'] 	 = $data['qtde'];
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Ruim':
					$_x_ax_[$data['cpf']]['ruim'] 	 = $data['qtde'];
					$totalizador['ruim'] += $data['qtde'];
					break;
				case 'Regular':
					$_x_ax_[$data['cpf']]['regular'] = $data['qtde'];
					$totalizador['regular'] += $data['qtde'];
					break;
				case 'Bom':
					$_x_ax_[$data['cpf']]['bom'] 	 = $data['qtde'];
					$totalizador['bom'] += $data['qtde'];
					break;
				case 'Ótimo':
					$_x_ax_[$data['cpf']]['otimo']   = $data['qtde'];
					$totalizador['otimo'] += $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			$data_1[] = $d['naval'];
			$data_2[] = $d['ruim'];
			$data_3[] = $d['regular'];
			$data_4[] = $d['bom'];
			$data_5[] = $d['otimo'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['naval']/count($eixo_x));
		$data_2[] = round($totalizador['ruim']/count($eixo_x));
		$data_3[] = round($totalizador['regular']/count($eixo_x));
		$data_4[] = round($totalizador['bom']/count($eixo_x));
		$data_5[] = round($totalizador['otimo']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['naval']/count($eixo_x));
		$dat_2 = round($totalizador['ruim']/count($eixo_x));
		$dat_3 = round($totalizador['regular']/count($eixo_x));
		$dat_4 = round($totalizador['bom']/count($eixo_x));
		$dat_5 = round($totalizador['otimo']/count($eixo_x));
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$data_4[] = $dat_4;
		$data_5[] = $dat_5;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,200,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("DEMANDAS AVALIADAS PELOS USUÁRIOS POR TÉCNICO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Não avaliado");
	$b1plot->SetFillColor("gray");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');
	$b1plot->value->HideZero();

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Ruim");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');
	$b2plot->value->HideZero();

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Regular");
	$b3plot->SetFillColor("brown");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');
	$b3plot->value->HideZero();

	$b4plot = new BarPlot($data_4);
	$b4plot->SetLegend("Bom");
	$b4plot->SetFillColor("blue");
	$b4plot->value->Show();
	$b4plot->value->SetAngle(90);
	$b4plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b4plot->value->SetFormat('%01.0f');
	$b4plot->value->HideZero();

	$b5plot = new BarPlot($data_5);
	$b5plot->SetLegend("Ótimo");
	$b5plot->SetFillColor("green");
	$b5plot->value->Show();
	$b5plot->value->SetAngle(90);
	$b5plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b5plot->value->SetFormat('%01.0f');
	$b5plot->value->HideZero();

	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot,$b4plot,$b5plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}


function avaliacaoportecnico_mes() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
		   (select EXTRACT(MONTH FROM h0.htddata)
			   || '' ||
			   EXTRACT(YEAR FROM h0.htddata)
		   from    workflow.historicodocumento h0
		   inner join demandas.demanda d0 on h0.docid = d0.docid
		   where    d.dmdid = d0.dmdid
		   order by h0.htddata desc
		   limit 1)
		   as dpeid,
		(CASE WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '1' THEN 'Ruim'
		      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '2' THEN 'Regular'	
		      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '3' THEN 'Bom'
		      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '4' THEN 'Ótimo'
		  ELSE 	'Não Avaliado'
		 END)   as avaliacao,
		EXTRACT(MONTH FROM (select  h0.htddata
						from 	workflow.historicodocumento h0
						inner join demandas.demanda d0 on h0.docid = d0.docid
						where 	d.dmdid = d0.dmdid 
						order by h0.htddata desc 
						limit 1)) as mes,
 		EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano,
		 
		
		count(d.dmdid) as qtde

		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 /*
 		 LEFT JOIN  (select max(a.avdid), a.dmdid, 
 		                    case 
 		                      when a.avnegatividade = '1' then '3'
 		                      else a.avdgeral
 		                    end as avaliacao 
 			 from demandas.avaliacaodemanda a
 			 INNER JOIN ( SELECT dmdid, MAX(avdid) as avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
 			 where a.avdstatus='A' 
 			 group by a.avdgeral, a.dmdid, a.avnegatividade) AS avd ON avd.dmdid = d.dmdid
 		 	 		 			
		 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
						from 	workflow.historicodocumento a
							inner join demandas.demanda d1 on a.docid = d1.docid
				  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
		 */
		 WHERE d.dmdstatus = 'A'
		 AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
		 AND od.ordid  IN  ('3')  				  	 	 
		 AND  ed.esdid  IN  (95,109,170)
		 --AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
		 AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
		 GROUP BY dpeid, mes, ano, avaliacao
		 ORDER BY ano, mes";
//echo'<pre>'; die($sql);
	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['mes']]['mes']) {
				$_x_ax_[$data['mes']]['mes']  	 = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$data['mes']."'")."/".$data['ano'];
				$_x_ax_[$data['mes']]['naval'] 	 = 0;
				$_x_ax_[$data['mes']]['ruim'] 	 = 0;
				$_x_ax_[$data['mes']]['regular'] = 0;
				$_x_ax_[$data['mes']]['bom'] 	 = 0;
				$_x_ax_[$data['mes']]['otimo']   = 0;
			}

			switch($data['avaliacao']) {
				case 'Não Avaliado':
					$_x_ax_[$data['mes']]['naval'] 	 = $data['qtde'];
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Ruim':
					$_x_ax_[$data['mes']]['ruim'] 	 = $data['qtde'];
					$totalizador['ruim'] += $data['qtde'];
					break;
				case 'Regular':
					$_x_ax_[$data['mes']]['regular'] = $data['qtde'];
					$totalizador['regular'] += $data['qtde'];
					break;
				case 'Bom':
					$_x_ax_[$data['mes']]['bom'] 	 = $data['qtde'];
					$totalizador['bom'] += $data['qtde'];
					break;
				case 'Ótimo':
					$_x_ax_[$data['mes']]['otimo']   = $data['qtde'];
					$totalizador['otimo'] += $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['naval'];
			$data_2[] = $d['ruim'];
			$data_3[] = $d['regular'];
			$data_4[] = $d['bom'];
			$data_5[] = $d['otimo'];
		}
	}
	if($_REQUEST['media']=="1") {
		if(count($eixo_x) > 0){
			$data_1[] = round($totalizador['naval']/count($eixo_x));
			$data_2[] = round($totalizador['ruim']/count($eixo_x));
			$data_3[] = round($totalizador['regular']/count($eixo_x));
			$data_4[] = round($totalizador['bom']/count($eixo_x));
			$data_5[] = round($totalizador['otimo']/count($eixo_x));
		}
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		if(count($eixo_x) > 0){
			$dat_1 = round($totalizador['naval']/count($eixo_x));
			$dat_2 = round($totalizador['ruim']/count($eixo_x));
			$dat_3 = round($totalizador['regular']/count($eixo_x));
			$dat_4 = round($totalizador['bom']/count($eixo_x));
			$dat_5 = round($totalizador['otimo']/count($eixo_x));
		}
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$data_4[] = $dat_4;
		$data_5[] = $dat_5;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("DEMANDAS AVALIADAS PELOS USUÁRIOS POR MÊS (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Não avaliado");
	$b1plot->SetFillColor("gray");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Ruim");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b2plot->value->SetFormat('%01.0f');

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Regular");
	$b3plot->SetFillColor("brown");
	$b3plot->value->Show();
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b3plot->value->SetFormat('%01.0f');

	$b4plot = new BarPlot($data_4);
	$b4plot->SetLegend("Bom");
	$b4plot->SetFillColor("blue");
	$b4plot->value->Show();
	$b4plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b4plot->value->SetFormat('%01.0f');

	$b5plot = new BarPlot($data_5);
	$b5plot->SetLegend("Ótimo");
	$b5plot->SetFillColor("green");
	$b5plot->value->Show();
	$b5plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b5plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot,$b4plot,$b5plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}

function classdemanda_semana() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
		   (select EXTRACT(MONTH FROM h0.htddata)
			   || '' ||
			   EXTRACT(YEAR FROM h0.htddata)
		   from    workflow.historicodocumento h0
		   inner join demandas.demanda d0 on h0.docid = d0.docid
		   where    d.dmdid = d0.dmdid
		   order by h0.htddata desc
		   limit 1)
		   as dpeid,
		(CASE WHEN dmdclassificacao = 'I' THEN 'Incidente'
		      WHEN dmdclassificacao = 'P' THEN 'Resolução de problema'	
		      WHEN dmdclassificacao = 'M' THEN 'Requisição de mudança'
		      WHEN dmdclassificacao = 'S' THEN 'Solicitação de Serviço'
		  ELSE 	'Não classificado'
		 END)   as classificacao,
		EXTRACT(WEEK FROM (select  h0.htddata
                      from workflow.historicodocumento h0
                      inner join demandas.demanda d0 on h0.docid = d0.docid
                      where d.dmdid = d0.dmdid 
                      order by h0.htddata desc 
                      limit 1)) as semana,
 		EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano,
		 
		
		count(d.dmdid) as qtde

		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 
		 /*
 		 LEFT JOIN  (select max(a.avdid), a.dmdid, 
 		                    case 
 		                      when a.avnegatividade = '1' then '3'
 		                      else a.avdgeral
 		                    end as avaliacao 
 			 from demandas.avaliacaodemanda a
 			 INNER JOIN ( SELECT dmdid, MAX(avdid) as avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
 			 where a.avdstatus='A' 
 			 group by a.avdgeral, a.dmdid, a.avnegatividade) AS avd ON avd.dmdid = d.dmdid
 		 	 		 
		 
					
		 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
						from 	workflow.historicodocumento a
							inner join demandas.demanda d1 on a.docid = d1.docid
				  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
		 */
		 WHERE d.dmdstatus = 'A'
		 AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
		 AND od.ordid  IN  ('3')  				  	 	 
		 AND  ed.esdid  IN  (95,109,170)
		 --AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
		 AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
		 GROUP BY dpeid, semana, ano, classificacao
		 ORDER BY ano, semana";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		$i = 0;
		foreach($datas as $data) {
			$i++;			
			if(!$_x_ax_[$data['semana']]['semana']) {
				$_x_ax_[$data['semana']]['semana']  	 = "Semana ".$i;
				$_x_ax_[$data['semana']]['naval'] 	 = 0;
				$_x_ax_[$data['semana']]['incidente'] 	 = 0;
				$_x_ax_[$data['semana']]['problema'] = 0;
				$_x_ax_[$data['semana']]['mudanca'] 	 = 0;
				$_x_ax_[$data['semana']]['servico']   = 0;
			}

			switch($data['classificacao']) {
				case 'Não classificado':
					$_x_ax_[$data['semana']]['naval'] 	 = $data['qtde'];
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Incidente':
					$_x_ax_[$data['semana']]['incidente'] 	 = $data['qtde'];
					$totalizador['ruim'] += $data['qtde'];
					break;
				case 'Resolução de problema':
					$_x_ax_[$data['semana']]['problema'] = $data['qtde'];
					$totalizador['regular'] += $data['qtde'];
					break;
				case 'Requisição de mudança':
					$_x_ax_[$data['semana']]['mudanca'] 	 = $data['qtde'];
					$totalizador['bom'] += $data['qtde'];
					break;
				case 'Solicitação de Serviço':
					$_x_ax_[$data['semana']]['servico']   = $data['qtde'];
					$totalizador['otimo'] += $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['semana'];
			$data_1[] = $d['naval'];
			$data_2[] = $d['incidente'];
			$data_3[] = $d['problema'];
			$data_4[] = $d['mudanca'];
			$data_5[] = $d['servico'];
		}
	}
	if($_REQUEST['media']=="1") {
		if(count($eixo_x) > 0){
			$data_1[] = round($totalizador['naval']/count($eixo_x));
			$data_2[] = round($totalizador['incidente']/count($eixo_x));
			$data_3[] = round($totalizador['problema']/count($eixo_x));
			$data_4[] = round($totalizador['mudanca']/count($eixo_x));
			$data_5[] = round($totalizador['servico']/count($eixo_x));
		}
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		if(count($eixo_x) > 0){
			$dat_1 = round($totalizador['naval']/count($eixo_x));
			$dat_2 = round($totalizador['incidente']/count($eixo_x));
			$dat_3 = round($totalizador['problema']/count($eixo_x));
			$dat_4 = round($totalizador['mudanca']/count($eixo_x));
			$dat_5 = round($totalizador['servico']/count($eixo_x));
		}
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$data_4[] = $dat_4;
		$data_5[] = $dat_5;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("CLASSIFICAÇÃO DAS DEMANDAS POR SEMANA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Não classificado");
	$b1plot->SetFillColor("gray");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Incidente");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b2plot->value->SetFormat('%01.0f');

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Resolução de problema");
	$b3plot->SetFillColor("brown");
	$b3plot->value->Show();
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b3plot->value->SetFormat('%01.0f');

	$b4plot = new BarPlot($data_4);
	$b4plot->SetLegend("Requisição de mudança");
	$b4plot->SetFillColor("blue");
	$b4plot->value->Show();
	$b4plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b4plot->value->SetFormat('%01.0f');

	$b5plot = new BarPlot($data_5);
	$b5plot->SetLegend("Solicitação de serviços");
	$b5plot->SetFillColor("green");
	$b5plot->value->Show();
	$b5plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b5plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot,$b4plot,$b5plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}

function classdemanda_mes() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
		   (select EXTRACT(MONTH FROM h0.htddata)
			   || '' ||
			   EXTRACT(YEAR FROM h0.htddata)
		   from    workflow.historicodocumento h0
		   inner join demandas.demanda d0 on h0.docid = d0.docid
		   where    d.dmdid = d0.dmdid
		   order by h0.htddata desc
		   limit 1)
		   as dpeid,
		(CASE WHEN dmdclassificacao = 'I' THEN 'Incidente'
		      WHEN dmdclassificacao = 'P' THEN 'Resolução de problema'	
		      WHEN dmdclassificacao = 'M' THEN 'Requisição de mudança'
		      WHEN dmdclassificacao = 'S' THEN 'Solicitação de Serviço'
		  ELSE 	'Não classificado'
		 END)   as classificacao,
		EXTRACT(MONTH FROM (select  h0.htddata
						from 	workflow.historicodocumento h0
						inner join demandas.demanda d0 on h0.docid = d0.docid
						where 	d.dmdid = d0.dmdid 
						order by h0.htddata desc 
						limit 1)) as mes,
 		EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano,
		 
		
		count(d.dmdid) as qtde

		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 /*
 		 LEFT JOIN  (select max(a.avdid), a.dmdid, 
 		                    case 
 		                      when a.avnegatividade = '1' then '3'
 		                      else a.avdgeral
 		                    end as avaliacao 
 			 from demandas.avaliacaodemanda a
 			 INNER JOIN ( SELECT dmdid, MAX(avdid) as avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
 			 where a.avdstatus='A' 
 			 group by a.avdgeral, a.dmdid, a.avnegatividade) AS avd ON avd.dmdid = d.dmdid
 			 		 
		 
					
		 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
						from 	workflow.historicodocumento a
							inner join demandas.demanda d1 on a.docid = d1.docid
				  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
		 */
		 WHERE d.dmdstatus = 'A'
		 AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
		 AND od.ordid  IN  ('3')  				  	 	 
		 AND  ed.esdid  IN  (95,109,170)
		 --AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
		 AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
		 GROUP BY dpeid, mes, ano, classificacao
		 ORDER BY ano, mes";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['mes']]['mes']) {
				$_x_ax_[$data['mes']]['mes']  	 = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$data['mes']."'")."/".$data['ano'];
				$_x_ax_[$data['mes']]['naval'] 	 = 0;
				$_x_ax_[$data['mes']]['incidente'] 	 = 0;
				$_x_ax_[$data['mes']]['problema'] = 0;
				$_x_ax_[$data['mes']]['mudanca'] 	 = 0;
				$_x_ax_[$data['mes']]['servico']   = 0;
			}

			switch($data['classificacao']) {
				case 'Não classificado':
					$_x_ax_[$data['mes']]['naval'] 	 = $data['qtde'];
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Incidente':
					$_x_ax_[$data['mes']]['incidente'] 	 = $data['qtde'];
					$totalizador['ruim'] += $data['qtde'];
					break;
				case 'Resolução de problema':
					$_x_ax_[$data['mes']]['problema'] = $data['qtde'];
					$totalizador['regular'] += $data['qtde'];
					break;
				case 'Requisição de mudança':
					$_x_ax_[$data['mes']]['mudanca'] 	 = $data['qtde'];
					$totalizador['bom'] += $data['qtde'];
					break;
				case 'Solicitação de Serviço':
					$_x_ax_[$data['mes']]['servico']   = $data['qtde'];
					$totalizador['otimo'] += $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['naval'];
			$data_2[] = $d['incidente'];
			$data_3[] = $d['problema'];
			$data_4[] = $d['mudanca'];
			$data_5[] = $d['servico'];
		}
	}
	if($_REQUEST['media']=="1") {
		if(count($eixo_x) > 0){
			$data_1[] = round($totalizador['naval']/count($eixo_x));
			$data_2[] = round($totalizador['incidente']/count($eixo_x));
			$data_3[] = round($totalizador['problema']/count($eixo_x));
			$data_4[] = round($totalizador['mudanca']/count($eixo_x));
			$data_5[] = round($totalizador['servico']/count($eixo_x));
		}
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		if(count($eixo_x) > 0){
			$dat_1 = round($totalizador['naval']/count($eixo_x));
			$dat_2 = round($totalizador['incidente']/count($eixo_x));
			$dat_3 = round($totalizador['problema']/count($eixo_x));
			$dat_4 = round($totalizador['mudanca']/count($eixo_x));
			$dat_5 = round($totalizador['servico']/count($eixo_x));
		}
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$data_4[] = $dat_4;
		$data_5[] = $dat_5;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("CLASSIFICAÇÃO DAS DEMANDAS POR MÊS (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Não classificado");
	$b1plot->SetFillColor("gray");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Incidente");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b2plot->value->SetFormat('%01.0f');

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Resolução de problema");
	$b3plot->SetFillColor("brown");
	$b3plot->value->Show();
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b3plot->value->SetFormat('%01.0f');

	$b4plot = new BarPlot($data_4);
	$b4plot->SetLegend("Requisição de mudança");
	$b4plot->SetFillColor("blue");
	$b4plot->value->Show();
	$b4plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b4plot->value->SetFormat('%01.0f');

	$b5plot = new BarPlot($data_5);
	$b5plot->SetLegend("Solicitação de serviços");
	$b5plot->SetFillColor("green");
	$b5plot->value->Show();
	$b5plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b5plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot,$b4plot,$b5plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}

function classdemanda_pizza() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
    (CASE WHEN dmdclassificacao = 'I' THEN 'Incidente'
		      WHEN dmdclassificacao = 'P' THEN 'Resolução de problema'	
		      WHEN dmdclassificacao = 'M' THEN 'Requisição de mudança'
		      WHEN dmdclassificacao = 'S' THEN 'Solicitação de Serviço'
		  ELSE 	'Não classificado'
		 END)   as classificacao,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS mes,
   EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS ano,
   COUNT(d.dmdid) AS qtde
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35) 
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
   /*
   LEFT JOIN ( SELECT 
                  MAX(a.avdid), 
                  a.dmdid, 
                  CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END AS avaliacao 
               FROM demandas.avaliacaodemanda a
               INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
               WHERE a.avdstatus='A' 
               GROUP BY a.avdgeral, a.dmdid, a.avnegatividade
             ) AS avd ON avd.dmdid = d.dmdid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')   
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, mes, ano, classificacao
ORDER BY ano, mes";
        
    	$datas = $db->carregar($sql);

	if($datas[0]) {
		
		$totalizador['incidente'] = 0;
		$totalizador['problema'] = 0;
		$totalizador['mudanca'] = 0;
		$totalizador['servico'] = 0;
		
		foreach($datas as $data) {

			switch($data['classificacao']) {
				case 'Incidente':
					$totalizador['incidente'] += $data['qtde'];
					break;
				case 'Resolução de problema':
					$totalizador['problema'] += $data['qtde'];
					break;
				case 'Requisição de mudança':
					$totalizador['mudanca'] += $data['qtde'];
					break;
				case 'Solicitação de Serviço':
					$totalizador['servico'] += $data['qtde'];
					break;

			}

		}
	}

	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	require_once ('../../includes/jpgraph/jpgraph_pie3d.php');
	
	$data = array($totalizador['incidente'],
				  $totalizador['problema'],
				  $totalizador['mudanca'],
				  $totalizador['servico'],0);
				  
	  $pbom = 0;
	  if($totalizador['mudanca']>0){
	  	$pbom = ($totalizador['mudanca']/ ($totalizador['incidente']+$totalizador['problema']+$totalizador['mudanca']+$totalizador['servico']) ) * 100;
	  }
	  $potimo = 0;
	  if($totalizador['otimo']>0){
	  	$potimo = ($totalizador['servico']/ ($totalizador['incidente']+$totalizador['problema']+$totalizador['mudanca']+$totalizador['servico']) ) * 100;
	  }
	  
	$graph = new PieGraph(900,550);
	$graph->SetShadow();
	
	$graph->title->Set("CLASSIFICAÇÃO DAS DEMANDAS - PIZZA (".$dataini." a ".$datafim.")");
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,8); 
	
	$xx = new PiePlot3D($data);
	$xx->SetSize(0.5);
	$xx->SetCenter(0.35);
	$cores = array("red","#EEDD82","blue","#006400","black");
	$xx->SetSliceColors($cores);
	$xx->value->SetFormat('%.1f%%');
	$xx->value->HideZero();
	$xx->SetLabelMargin(20);
	$legendas = array("Incidente","Resolução de problema","Requisição de mudança","Solicitação de serviço","Mudanças + Serviços(".(number_format($pbom+$potimo,2)).")");
	$xx->SetLegends($legendas);
	//$xx->ExplodeAll(10);
	$xx->SetShadow();
	$graph->Add($xx);
	$graph->Stroke();

}

function classdemanda_pessoa() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
    (CASE WHEN dmdclassificacao = 'I' THEN 'Incidente'
		      WHEN dmdclassificacao = 'P' THEN 'Resolução de problema'	
		      WHEN dmdclassificacao = 'M' THEN 'Requisição de mudança'
		      WHEN dmdclassificacao = 'S' THEN 'Solicitação de Serviço'
		  ELSE 	'Não classificado'
		 END)   as classificacao,
    --sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
    COUNT(d.dmdid) AS qtde,
    d.usucpfexecutor AS cpf,
    UPPER(u2.usunome) AS usunome  
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
   LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
   /*
   LEFT JOIN ( SELECT 
                  MAX(a.avdid), 
                  a.dmdid, 
                  CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END AS avaliacao 
               FROM demandas.avaliacaodemanda a
               INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
               WHERE a.avdstatus='A' 
               GROUP BY a.avdgeral, a.dmdid, a.avnegatividade
             ) AS avd ON avd.dmdid = d.dmdid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
    */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')   
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, d.usucpfexecutor, UPPER(u2.usunome), classificacao
ORDER BY UPPER(u2.usunome)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['cpf']]['nome']) {
				$_x_ax_[$data['cpf']]['nome']  	 = $data['usunome'];
				$_x_ax_[$data['cpf']]['naval'] 	 = 0;
				$_x_ax_[$data['cpf']]['incidente'] 	 = 0;
				$_x_ax_[$data['cpf']]['problema'] = 0;
				$_x_ax_[$data['cpf']]['mudanca'] 	 = 0;
				$_x_ax_[$data['cpf']]['servico']   = 0;
			}

			switch($data['classificacao']) {
				case 'Não classificado':
					$_x_ax_[$data['cpf']]['naval'] 	 = $data['qtde'];
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Incidente':
					$_x_ax_[$data['cpf']]['incidente'] 	 = $data['qtde'];
					$totalizador['incidente'] += $data['qtde'];
					break;
				case 'Resolução de problema':
					$_x_ax_[$data['cpf']]['problema'] = $data['qtde'];
					$totalizador['problema'] += $data['qtde'];
					break;
				case 'Requisição de mudança':
					$_x_ax_[$data['cpf']]['mudanca'] 	 = $data['qtde'];
					$totalizador['mudanca'] += $data['qtde'];
					break;
				case 'Solicitação de Serviço':
					$_x_ax_[$data['cpf']]['servico']   = $data['qtde'];
					$totalizador['servico'] += $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			$data_1[] = $d['naval'];
			$data_2[] = $d['incidente'];
			$data_3[] = $d['problema'];
			$data_4[] = $d['mudanca'];
			$data_5[] = $d['servico'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['naval']/count($eixo_x));
		$data_2[] = round($totalizador['incidente']/count($eixo_x));
		$data_3[] = round($totalizador['problema']/count($eixo_x));
		$data_4[] = round($totalizador['mudanca']/count($eixo_x));
		$data_5[] = round($totalizador['servico']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['naval']/count($eixo_x));
		$dat_2 = round($totalizador['incidente']/count($eixo_x));
		$dat_3 = round($totalizador['problema']/count($eixo_x));
		$dat_4 = round($totalizador['mudanca']/count($eixo_x));
		$dat_5 = round($totalizador['servico']/count($eixo_x));
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$data_4[] = $dat_4;
		$data_5[] = $dat_5;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['incidente'];
		$data_3[] = $totalizador['problema'];
		$data_4[] = $totalizador['mudanca'];
		$data_5[] = $totalizador['servico'];
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['incidente'];
		$data_3[] = $totalizador['problema'];
		$data_4[] = $totalizador['mudanca'];
		$data_5[] = $totalizador['servico'];

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,200,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("CLASSIFICAÇÃO DAS DEMANDAS POR TÉCNICO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Não avaliado");
	$b1plot->SetFillColor("gray");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');
	$b1plot->value->HideZero();

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Incidente");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');
	$b2plot->value->HideZero();

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Resolução de problema");
	$b3plot->SetFillColor("brown");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');
	$b3plot->value->HideZero();

	$b4plot = new BarPlot($data_4);
	$b4plot->SetLegend("Requisição de mudança");
	$b4plot->SetFillColor("blue");
	$b4plot->value->Show();
	$b4plot->value->SetAngle(90);
	$b4plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b4plot->value->SetFormat('%01.0f');
	$b4plot->value->HideZero();

	$b5plot = new BarPlot($data_5);
	$b5plot->SetLegend("Solicitação de serviço");
	$b5plot->SetFillColor("green");
	$b5plot->value->Show();
	$b5plot->value->SetAngle(90);
	$b5plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b5plot->value->SetFormat('%01.0f');
	$b5plot->value->HideZero();

	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot,$b4plot,$b5plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}


function avaliacaoportecnico_pizza() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
   (CASE WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '1' THEN 'Ruim'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '2' THEN 'Regular'   
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '3' THEN 'Bom'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '4' THEN 'Ótimo'
      ELSE  'Não Avaliado'
    END)   AS avaliacao,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS mes,
   EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS ano,
   COUNT(d.dmdid) AS qtde
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
   /*
   LEFT JOIN ( SELECT 
                  MAX(a.avdid), 
                  a.dmdid, 
                  CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END AS avaliacao 
               FROM demandas.avaliacaodemanda a
               INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
               WHERE a.avdstatus='A' 
               GROUP BY a.avdgeral, a.dmdid, a.avnegatividade
             ) AS avd ON avd.dmdid = d.dmdid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')   
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, mes, ano, avaliacao
ORDER BY ano, mes";
        
    	$datas = $db->carregar($sql);

	if($datas[0]) {
		
		$totalizador['ruim'] = 0;
		$totalizador['regular'] = 0;
		$totalizador['bom'] = 0;
		$totalizador['otimo'] = 0;
		
		foreach($datas as $data) {

			switch($data['avaliacao']) {
				case 'Ruim':
					$totalizador['ruim'] += $data['qtde'];
					break;
				case 'Regular':
					$totalizador['regular'] += $data['qtde'];
					break;
				case 'Bom':
					$totalizador['bom'] += $data['qtde'];
					break;
				case 'Ótimo':
					$totalizador['otimo'] += $data['qtde'];
					break;

			}

		}
	}

	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	require_once ('../../includes/jpgraph/jpgraph_pie3d.php');
	
	$data = array($totalizador['ruim'],
				  $totalizador['regular'],
				  $totalizador['bom'],
				  $totalizador['otimo'],0);
				  
	  $pbom = 0;
	  if($totalizador['bom']>0){
	  	$pbom = ($totalizador['bom']/ ($totalizador['ruim']+$totalizador['regular']+$totalizador['bom']+$totalizador['otimo']) ) * 100;
	  }
	  $potimo = 0;
	  if($totalizador['otimo']>0){
	  	$potimo = ($totalizador['otimo']/ ($totalizador['ruim']+$totalizador['regular']+$totalizador['bom']+$totalizador['otimo']) ) * 100;
	  }
	  
	$graph = new PieGraph(800,440);
	$graph->SetShadow();
	
	$graph->title->Set("DEMANDAS AVALIADAS PELOS USUÁRIOS - PIZZA (".$dataini." a ".$datafim.")");
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,8); 
	
	$xx = new PiePlot3D($data);
	$xx->SetSize(0.5);
	$xx->SetCenter(0.45);
	$cores = array("red","#EEDD82","blue","#006400","black");
	$xx->SetSliceColors($cores);
	$xx->value->SetFormat('%.1f%%');
	$xx->value->HideZero();
	$xx->SetLabelMargin(20);
	$legendas = array("Ruim","Regular","Bom","Ótimo","Bom + Ótimo (".(number_format($pbom+$potimo,2)).")");
	$xx->SetLegends($legendas);
	//$xx->ExplodeAll(10);
	$xx->SetShadow();
	$graph->Add($xx);
	$graph->Stroke();

}


function avaliacaoportecnico_pizza2() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
        
        $sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
   (CASE WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '1' THEN 'Ruim'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '2' THEN 'Regular'   
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '3' THEN 'Bom'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '4' THEN 'Ótimo'
      ELSE  'Não Avaliado'
    END)   AS avaliacao,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS mes,
   EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS ano,
   COUNT(d.dmdid) AS qtde
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
   /*
   LEFT JOIN ( SELECT 
                  MAX(a.avdid), 
                  a.dmdid, 
                  CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END AS avaliacao 
               FROM demandas.avaliacaodemanda a
               INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
               WHERE a.avdstatus='A' 
               GROUP BY a.avdgeral, a.dmdid, a.avnegatividade
             ) AS avd ON avd.dmdid = d.dmdid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')   
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, mes, ano, avaliacao
ORDER BY ano, mes";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			switch($data['avaliacao']) {
				case 'Não Avaliado':
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Ruim':
				case 'Regular':
				case 'Bom':
				case 'Ótimo':
					$totalizador['aval'] += $data['qtde'];
					break;

			}

		}
	}

	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	require_once ('../../includes/jpgraph/jpgraph_pie3d.php');
	
	$data = array($totalizador['naval'],
				  $totalizador['aval']);
	
	$graph = new PieGraph(800,440);
	$graph->SetShadow();
	
	$graph->title->Set("DEMANDAS AVALIADAS PELOS USUÁRIOS - AVALIADAS / NÃO AVALIADAS (".$dataini." a ".$datafim.")");
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,8); 
	
	$xx = new PiePlot3D($data);
	$xx->SetSize(0.5);
	$xx->SetCenter(0.45);
	$cores = array("red","blue");
	$xx->SetSliceColors($cores);
	$xx->SetLabelMargin(20);
	$xx->value->SetFormat('%01.1f%%');
	$xx->value->HideZero();
	$legendas = array("Não avaliadas","Avaliadas");
	$xx->SetLegends($legendas);
	//$xx->ExplodeAll(10);
	$xx->SetShadow();
	$graph->Add($xx);
	$graph->Stroke();

}

function avaliacaoportecnico_setor() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
   (CASE WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '1' THEN 'Ruim'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '2' THEN 'Regular'   
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '3' THEN 'Bom'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '4' THEN 'Ótimo'
      ELSE  'Não Avaliado'
    END)   AS avaliacao,
   COUNT(d.dmdid) AS qtde,
   d.unaid AS unaid,
   UPPER(uni.unasigla) AS nome  

FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
   LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
   /*
   LEFT JOIN ( SELECT 
                  MAX(a.avdid), 
                  a.dmdid, 
                  CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END AS avaliacao 
               FROM demandas.avaliacaodemanda a
               INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
               WHERE a.avdstatus='A' 
               GROUP BY a.avdgeral, a.dmdid, a.avnegatividade
             ) AS avd ON avd.dmdid = d.dmdid

   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')   
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, d.unaid, UPPER(uni.unasigla), avaliacao
ORDER BY UPPER(uni.unasigla)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['unaid']]['nome']) {
				$_x_ax_[$data['unaid']]['nome']  	 = $data['nome'];
				$_x_ax_[$data['unaid']]['naval'] 	 = 0;
				$_x_ax_[$data['unaid']]['ruim'] 	 = 0;
				$_x_ax_[$data['unaid']]['regular'] = 0;
				$_x_ax_[$data['unaid']]['bom'] 	 = 0;
				$_x_ax_[$data['unaid']]['otimo']   = 0;
			}

			switch($data['avaliacao']) {
				case 'Não Avaliado':
					$_x_ax_[$data['unaid']]['naval'] 	 = $data['qtde'];
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Ruim':
					$_x_ax_[$data['unaid']]['ruim'] 	 = $data['qtde'];
					$totalizador['ruim'] += $data['qtde'];
					break;
				case 'Regular':
					$_x_ax_[$data['unaid']]['regular'] = $data['qtde'];
					$totalizador['regular'] += $data['qtde'];
					break;
				case 'Bom':
					$_x_ax_[$data['unaid']]['bom'] 	 = $data['qtde'];
					$totalizador['bom'] += $data['qtde'];
					break;
				case 'Ótimo':
					$_x_ax_[$data['unaid']]['otimo']   = $data['qtde'];
					$totalizador['otimo'] += $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['naval'];
			$data_2[] = $d['ruim'];
			$data_3[] = $d['regular'];
			$data_4[] = $d['bom'];
			$data_5[] = $d['otimo'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['naval']/count($eixo_x));
		$data_2[] = round($totalizador['ruim']/count($eixo_x));
		$data_3[] = round($totalizador['regular']/count($eixo_x));
		$data_4[] = round($totalizador['bom']/count($eixo_x));
		$data_5[] = round($totalizador['otimo']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['naval']/count($eixo_x));
		$dat_2 = round($totalizador['ruim']/count($eixo_x));
		$dat_3 = round($totalizador['regular']/count($eixo_x));
		$dat_4 = round($totalizador['bom']/count($eixo_x));
		$dat_5 = round($totalizador['otimo']/count($eixo_x));
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$data_4[] = $dat_4;
		$data_5[] = $dat_5;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,200,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("DEMANDAS AVALIADAS PELOS USUÁRIOS POR SETOR (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Não avaliado");
	$b1plot->SetFillColor("gray");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Ruim");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Regular");
	$b3plot->SetFillColor("brown");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');

	$b4plot = new BarPlot($data_4);
	$b4plot->SetLegend("Bom");
	$b4plot->SetFillColor("blue");
	$b4plot->value->Show();
	$b4plot->value->SetAngle(90);
	$b4plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b4plot->value->SetFormat('%01.0f');

	$b5plot = new BarPlot($data_5);
	$b5plot->SetLegend("Ótimo");
	$b5plot->SetFillColor("green");
	$b5plot->value->Show();
	$b5plot->value->SetAngle(90);
	$b5plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b5plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot,$b4plot,$b5plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}




function avaliacaoportecnico_tiposervico() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
   to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
   (CASE WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '1' THEN 'Ruim'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '2' THEN 'Regular'   
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '3' THEN 'Bom'
      WHEN ( SELECT 
                CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END 
                FROM demandas.avaliacaodemanda a
                INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
                WHERE a.avdstatus='A' and b.dmdid = d.dmdid
                GROUP BY a.avdgeral, b.dmdid, a.avnegatividade
              ) = '4' THEN 'Ótimo'
      ELSE  'Não Avaliado'
    END)   AS avaliacao,
   COUNT(d.dmdid) AS qtde,
   t.tipid AS tipid,
   UPPER(t.tipnome) AS nome  
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid     
   LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
   LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
   /*
   LEFT JOIN ( SELECT 
                  MAX(a.avdid), 
                  a.dmdid, 
                  CASE WHEN a.avnegatividade = '1' THEN '3' ELSE a.avdgeral END AS avaliacao 
               FROM demandas.avaliacaodemanda a
               INNER JOIN ( SELECT dmdid, MAX(avdid) AS avdid FROM demandas.avaliacaodemanda GROUP BY dmdid ) b ON b.avdid = a.avdid 
               WHERE a.avdstatus='A' 
               GROUP BY a.avdgeral, a.dmdid, a.avnegatividade
             ) AS avd ON avd.dmdid = d.dmdid
   LEFT JOIN ( SELECT 
                  d1.dmdid, 
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                  MAX(a.htddata) AS datasituacao
               FROM    workflow.historicodocumento a
               INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
   INNER JOIN (SELECT COUNT(d.dmdid) AS total, t.tipid 
               FROM demandas.demanda d
               INNER JOIN demandas.tiposervico t ON t.tipid = d.tipid AND t.ordid = 3 AND t.tipstatus = 'A'
               GROUP BY t.tipid ORDER BY total DESC limit 7
              ) AS tip ON tip.tipid = t.tipid
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')   
AND  ed.esdid  IN  (95,109,170)  
--AND  ( dmd1.datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND dmd1.datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, t.tipid, UPPER(t.tipnome), avaliacao
ORDER BY UPPER(t.tipnome)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['tipid']]['nome']) {
				$_x_ax_[$data['tipid']]['nome']  	 = $data['nome'];
				$_x_ax_[$data['tipid']]['naval'] 	 = 0;
				$_x_ax_[$data['tipid']]['ruim'] 	 = 0;
				$_x_ax_[$data['tipid']]['regular'] = 0;
				$_x_ax_[$data['tipid']]['bom'] 	 = 0;
				$_x_ax_[$data['tipid']]['otimo']   = 0;
			}

			switch($data['avaliacao']) {
				case 'Não Avaliado':
					$_x_ax_[$data['tipid']]['naval'] 	 = $data['qtde'];
					$totalizador['naval'] += $data['qtde'];
					break;
				case 'Ruim':
					$_x_ax_[$data['tipid']]['ruim'] 	 = $data['qtde'];
					$totalizador['ruim'] += $data['qtde'];
					break;
				case 'Regular':
					$_x_ax_[$data['tipid']]['regular'] = $data['qtde'];
					$totalizador['regular'] += $data['qtde'];
					break;
				case 'Bom':
					$_x_ax_[$data['tipid']]['bom'] 	 = $data['qtde'];
					$totalizador['bom'] += $data['qtde'];
					break;
				case 'Ótimo':
					$_x_ax_[$data['tipid']]['otimo']   = $data['qtde'];
					$totalizador['otimo'] += $data['qtde'];
					break;

			}

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$nome = explode(' ', $d['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $d['nome'];
			$data_1[] = $d['naval'];
			$data_2[] = $d['ruim'];
			$data_3[] = $d['regular'];
			$data_4[] = $d['bom'];
			$data_5[] = $d['otimo'];
		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['naval']/count($eixo_x));
		$data_2[] = round($totalizador['ruim']/count($eixo_x));
		$data_3[] = round($totalizador['regular']/count($eixo_x));
		$data_4[] = round($totalizador['bom']/count($eixo_x));
		$data_5[] = round($totalizador['otimo']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['naval']/count($eixo_x));
		$dat_2 = round($totalizador['ruim']/count($eixo_x));
		$dat_3 = round($totalizador['regular']/count($eixo_x));
		$dat_4 = round($totalizador['bom']/count($eixo_x));
		$dat_5 = round($totalizador['otimo']/count($eixo_x));
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$data_4[] = $dat_4;
		$data_5[] = $dat_5;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1,$data_2,$data_3,$data_4,$data_5,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['naval'];
		$data_2[] = $totalizador['ruim'];
		$data_3[] = $totalizador['regular'];
		$data_4[] = $totalizador['bom'];
		$data_5[] = $totalizador['otimo'];

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(50,200,35,230);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("DEMANDAS AVALIADAS PELOS USUÁRIOS POR TIPO DE SERVIÇO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Não avaliado");
	$b1plot->SetFillColor("gray");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Ruim");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetAngle(90);
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b2plot->value->SetFormat('%01.0f');

	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Regular");
	$b3plot->SetFillColor("brown");
	$b3plot->value->Show();
	$b3plot->value->SetAngle(90);
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b3plot->value->SetFormat('%01.0f');

	$b4plot = new BarPlot($data_4);
	$b4plot->SetLegend("Bom");
	$b4plot->SetFillColor("blue");
	$b4plot->value->Show();
	$b4plot->value->SetAngle(90);
	$b4plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b4plot->value->SetFormat('%01.0f');

	$b5plot = new BarPlot($data_5);
	$b5plot->SetLegend("Ótimo");
	$b5plot->SetFillColor("green");
	$b5plot->value->Show();
	$b5plot->value->SetAngle(90);
	$b5plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b5plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot,$b3plot,$b4plot,$b5plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();


}



function numerodemandas_pessoa() {
	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
    TO_CHAR( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
    COUNT(d.dmdid) AS qtde,
    d.usucpfexecutor AS cpf,
    UPPER(u2.usunome) AS usunome
FROM demandas.demanda d
    LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
    LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
    LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
    /*
    LEFT JOIN  (SELECT 
                    d1.dmdid, 
                    TO_CHAR(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                    MAX(a.htddata) AS datasituacao
                FROM    workflow.historicodocumento a
                INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                GROUP BY d1.dmdid ORDER BY 2 DESC
               ) AS dmd1 ON dmd1.dmdid = d.dmdid
     */
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'    
GROUP BY dpeid, d.usucpfexecutor, UPPER(u2.usunome)
ORDER BY UPPER(u2.usunome)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {
			$nome = explode(' ', $data['usunome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $data['usunome'];
			$data_1[] = $data['qtde'];
			$totalizador += $data['qtde'];
		}
	}
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador/count($eixo_x));
		unset($data_1, $eixo_x);
		$data_1[] = $dat_1;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("NÚMERO DE DEMANDAS POR PESSOA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	//$gbplot = new GroupBarPlot(array($b1plot));

	// Set color for the frame of each bar
	$graph->Add($b1plot);

	// Finally send the graph to the browser
	$graph->Stroke();
}


function numerodemandas_semana() {
	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
	
			count(d.dmdid) as qtde,
			EXTRACT(WEEK FROM (select  h0.htddata
						from 	workflow.historicodocumento h0
						inner join demandas.demanda d0 on h0.docid = d0.docid
						where 	d.dmdid = d0.dmdid 
						order by h0.htddata desc 
						limit 1)) as semana,
			EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano
					 
		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 
		 --LEFT JOIN  (select max(avdid), dmdid, avdgeral as avaliacao from demandas.avaliacaodemanda where avdstatus='A' group by avdgeral, dmdid) AS avd ON avd.dmdid = d.dmdid
		 
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 /*
		 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
						from 	workflow.historicodocumento a
							--inner join workflow.documento c on c.docid = a.docid
					where a.aedid in (146, 191) 
					group by a.docid
					) ) as hst ON hst.docid = d.docid
		 			
		 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
						from 	workflow.historicodocumento a
							inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
							--inner join workflow.documento c on c.docid = a.docid
					where a.aedid in (146, 191) 
					group by a.docid, a.hstid, b.cmddsc 
					) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
					
		 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
						from 	workflow.historicodocumento a
							inner join demandas.demanda d1 on a.docid = d1.docid
				  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid

		 LEFT JOIN (  select a.docid, u.usunome as nometecnicoclassif, a.usucpf AS usucpftecnicoclassif, to_char(min(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as dataclassif
						from 	workflow.historicodocumento a
							inner join seguranca.usuario u on u.usucpf = a.usucpf
							INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (143,184) --143,184=Em analise (classificação da demanda)
					group by a.docid, u.usunome, a.usucpf ) as cla ON cla.docid = d.docid
				  
		LEFT JOIN (select max(a.hstid), a.docid, u.usunome as nomegestor, a.usucpf AS usucpfgestor --to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datagestor
						from 	workflow.historicodocumento a
							inner join seguranca.usuario u on u.usucpf = a.usucpf
							INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (224,186,165,278,279,368) --224,186,165=finalizada/validada -  278,279=invalidada - 368=validada fora do prazo
					group by a.docid, u.usunome, a.usucpf) as ges ON ges.docid = d.docid
		*/		  	 	 
	 	 
		 WHERE d.dmdstatus = 'A'  
		 AND od.ordid  IN  ('3')  				  	 	 
	 	 
		AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')	
		AND  ed.esdid  IN  (95,109,170)  
		--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
		AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
		group by ano, semana
		ORDER BY ano, semana";
	
	$datas = $db->carregar($sql);

	if($datas[0]) {
		$i=1;
		foreach($datas as $data) {

			$eixo_x[] = "SEMANA ".$i;
			$data_1[] = $data['qtde'];
			$totalizador += $data['qtde'];
			$i++;

		}
	}

	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador/count($eixo_x));
		unset($data_1, $eixo_x);
		$data_1[] = $dat_1;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("NÚMERO DE DEMANDAS POR SEMANA (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');
	// Set color for the frame of each bar
	$graph->Add($b1plot);
	// Finally send the graph to the browser
	$graph->Stroke();
}

function numerodemandas_mes() {
	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	
        
        $sql = "SELECT DISTINCT
                count(d.dmdid) as qtde,
                EXTRACT(MONTH FROM (select  h0.htddata
                                    from  workflow.historicodocumento h0
                                    inner join demandas.demanda d0 on h0.docid = d0.docid
                                    where d.dmdid = d0.dmdid
                                    order by h0.htddata desc
                                    limit 1)) as mes,
                EXTRACT(YEAR FROM (select  h0.htddata
                                   from  workflow.historicodocumento h0
                                   inner join demandas.demanda d0 on h0.docid = d0.docid
                                   where d.dmdid = d0.dmdid
                                   order by h0.htddata desc
                                   limit 1)) as ano
                FROM
                demandas.demanda d
                LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
                LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid                  
                LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
                LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid     
                --LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
				/*
                LEFT JOIN ( select
                               d1.dmdid,
                               to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit,
                               max(a.htddata) as datasituacao
                                from     workflow.historicodocumento a
                                    inner join demandas.demanda d1 on a.docid = d1.docid
                              group by d1.dmdid
                              order by 2 desc) as dmd1 ON dmd1.dmdid = d.dmdid
                */
                WHERE d.dmdstatus = 'A'
                AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
                AND od.ordid  IN  ('3')                              
                AND ed.esdid  IN  (95,109,170)
				--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
				AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
				GROUP BY mes, ano
				ORDER BY ano, mes";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {
			$eixo_x[] = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer='".$data['mes']."'")."/".$data['ano'];
			$data_1[] = $data['qtde'];
			$totalizador += $data['qtde'];
		}
	}
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador/count($eixo_x));
		unset($data_1, $eixo_x);
		$data_1[] = $dat_1;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("NÚMERO DE DEMANDAS POR MÊS (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,6); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');
	// Set color for the frame of each bar
	$graph->Add($b1plot);
	// Finally send the graph to the browser
	$graph->Stroke();
}



function numerodemandas_setor() {
	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT

		EXTRACT(MONTH FROM (select  h0.htddata
						from 	workflow.historicodocumento h0
						inner join demandas.demanda d0 on h0.docid = d0.docid
						where 	d.dmdid = d0.dmdid 
						order by h0.htddata desc 
						limit 1)) 
		|| '' ||
		EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1))
		as dpeid, 

		--sum(cast (pt.tsppontuacao as bigint)*d.dmdqtde) as qtde,
		count(d.dmdid) as qtde,
		 
		d.unaid as unaid,
		UPPER(uni.unasigla) as nome

		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 
		 --LEFT JOIN  (select max(avdid), dmdid, avdgeral as avaliacao from demandas.avaliacaodemanda where avdstatus='A' group by avdgeral, dmdid) AS avd ON avd.dmdid = d.dmdid
		 
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 /*
		 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
						from 	workflow.historicodocumento a
							--inner join workflow.documento c on c.docid = a.docid
					where a.aedid in (146, 191) 
					group by a.docid
					) ) as hst ON hst.docid = d.docid
					
		 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
						from 	workflow.historicodocumento a
							inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
							--inner join workflow.documento c on c.docid = a.docid
					where a.aedid in (146, 191) 
					group by a.docid, a.hstid, b.cmddsc 
					) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
					
		 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
						from 	workflow.historicodocumento a
							inner join demandas.demanda d1 on a.docid = d1.docid
				  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid

		 LEFT JOIN (  select a.docid, u.usunome as nometecnicoclassif, a.usucpf AS usucpftecnicoclassif, to_char(min(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as dataclassif
						from 	workflow.historicodocumento a
							inner join seguranca.usuario u on u.usucpf = a.usucpf
							INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (143,184) --143,184=Em analise (classificação da demanda)
					group by a.docid, u.usunome, a.usucpf ) as cla ON cla.docid = d.docid
				  
		LEFT JOIN (select max(a.hstid), a.docid, u.usunome as nomegestor, a.usucpf AS usucpfgestor --to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datagestor
						from 	workflow.historicodocumento a
							inner join seguranca.usuario u on u.usucpf = a.usucpf
							INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (224,186,165,278,279,368) --224,186,165=finalizada/validada -  278,279=invalidada - 368=validada fora do prazo
					group by a.docid, u.usunome, a.usucpf) as ges ON ges.docid = d.docid
		*/		  	 	 
	 	 
		 WHERE d.dmdstatus = 'A'  
		 AND od.ordid  IN  ('3')  				  	 	 
	 	 
		AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')	
		AND  ed.esdid  IN  (95,109,170)  
		--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
		AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
		group by dpeid, d.unaid, UPPER(uni.unasigla)
		ORDER BY UPPER(uni.unasigla)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {
			$nome = explode(' ', $data['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $data['nome'];
			$data_1[] = $data['qtde'];
			$totalizador += $data['qtde'];
		}
	}
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador/count($eixo_x));
		unset($data_1, $eixo_x);
		$data_1[] = $dat_1;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("NÚMERO DE DEMANDAS POR SETOR (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	//$gbplot = new GroupBarPlot(array($b1plot));

	// Set color for the frame of each bar
	$graph->Add($b1plot);

	// Finally send the graph to the browser
	$graph->Stroke();
}




function numerodemandas_tiposervico() {
	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
    to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
    t.tipid AS tipid, 
    UPPER(t.tipnome) AS nome,
    COUNT(d.dmdid) AS qtde

FROM demandas.demanda d
    LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
    LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
	/*
    LEFT JOIN  (SELECT 
                    d1.dmdid, 
                    TO_CHAR(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                    MAX(a.htddata) AS datasituacao
                FROM    workflow.historicodocumento a
                INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                GROUP BY d1.dmdid ORDER BY 2 DESC
               ) AS dmd1 ON dmd1.dmdid = d.dmdid
	*/
    --pega os tipos de serviços mais requisitados           
    INNER JOIN (SELECT COUNT(d.dmdid) AS total, t.tipid 
                FROM demandas.demanda d
                INNER JOIN demandas.tiposervico t ON t.tipid = d.tipid AND t.ordid = 3 AND t.tipstatus = 'A'
                GROUP BY t.tipid ORDER BY total DESC limit 7
               ) AS TIP ON TIP.TIPID = T.TIPID                 


WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'    
GROUP BY dpeid, t.tipid, UPPER(t.tipnome)
ORDER BY UPPER(t.tipnome)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {
			$nome = explode(' ', $data['nome']);
			$eixo_x[] = $nome[0] .' '.end($nome);
			//$eixo_x[] = $data['nome'];
			$data_1[] = $data['qtde'];
			$totalizador += $data['qtde'];
		}
	}
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador/count($eixo_x));
		unset($data_1, $eixo_x);
		$data_1[] = $dat_1;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("NÚMERO DE DEMANDAS POR TIPO DE SERVIÇO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	//$gbplot = new GroupBarPlot(array($b1plot));

	// Set color for the frame of each bar
	$graph->Add($b1plot);

	// Finally send the graph to the browser
	$graph->Stroke();
}


/*
 * aguardando validação demandas total por mês
 */
function agvalidacao_pormes() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
   TO_CHAR( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY')   AS dpeid,  
   COUNT(d.dmdid) AS qtddemandas,
   SUM(cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END)) AS dshqtde,
   SUM((CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END)) AS qtdservicos,                     
   SUM((cast (pt.tsppontuacao AS bigint) * (CASE WHEN d.dmdqtde > 0 THEN d.dmdqtde ELSE 1 END))* COALESCE(crtvlponto,0) ) AS valor,
   EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS mes,
   EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) AS ano
FROM demandas.demanda d
   LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
   LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
   LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid    
   LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid AND pt.priid = d.priid AND pt.tspstatus = 'A'
   /*
   LEFT JOIN ( SELECT
                  d1.dmdid,
                  to_char(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit,
                  MAX(a.htddata) AS datasituacao
               FROM  workflow.historicodocumento a
                  INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
               GROUP BY d1.dmdid ORDER BY 2 DESC
             ) AS dmd1 ON dmd1.dmdid = d.dmdid
   */
   LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
             
WHERE d.dmdstatus = 'A' 
AND t.ordid  IN  ('3')                          
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
AND  ed.esdid  IN  (93,111) 
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'
GROUP BY dpeid, mes, ano, crtvlponto
ORDER BY ano, mes";
	
	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['dpeid']]['mes']) {
				$_x_ax_[$data['dpeid']]['mes']  = trim($db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$data['mes']."'"))."/".$data['ano'];
			}

			$_x_ax_[$data['dpeid']]['qtddemandas'] = $data['qtddemandas'];
			$_x_ax_[$data['dpeid']]['qtdservicos'] = $data['qtdservicos'];
			$_x_ax_[$data['dpeid']]['qtde'] = $data['dshqtde'];
			$_x_ax_[$data['dpeid']]['valor'] = $data['valor'];

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['mes'];
			$data_0[] = $d['qtddemandas'];
			$totalizador['qtddemandas'] += $d['qtddemandas'];
			$data_1[] = $d['qtdservicos'];
			$totalizador['qtdservicos'] += $d['qtdservicos'];
			$data_2[] = $d['qtde'];
			$totalizador['qtde'] += $d['qtde'];
			$data_3[] = $d['valor'];
			$totalizador['valor'] += $d['valor'];			
		}
	}

	if($_REQUEST['media']=="1") {
		$data_0[] = round($totalizador['qtddemandas']/count($eixo_x));
		$data_1[] = round($totalizador['qtdservicos']/count($eixo_x));
		$data_2[] = round($totalizador['qtde']/count($eixo_x));
		$data_3[] = round($totalizador['valor']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_0 = round($totalizador['qtddemandas']/count($eixo_x));
		$dat_1 = round($totalizador['qtdservicos']/count($eixo_x));
		$dat_2 = round($totalizador['qtde']/count($eixo_x));
		$dat_3 = round($totalizador['valor']/count($eixo_x));
		$eix_x = "MÉDIA";
		unset($data_0,$data_1,$data_2,$data_3,$eixo_x);
		$data_0[] = $dat_0;
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_0[] = $totalizador['qtddemandas'];
		$data_1[] = $totalizador['qtdservicos'];
		$data_2[] = $totalizador['qtde'];
		$data_3[] = $totalizador['valor'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_0 = $totalizador['qtddemandas'];
		$dat_1 = $totalizador['qtdservicos'];
		$dat_2 = $totalizador['qtde'];
		$dat_3 = $totalizador['valor'];
		unset($data_0,$data_1,$data_2,$data_3,$eixo_x);
		$data_0[] = $dat_0;
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "TOTAL";

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("DEMANDAS AGUARDANDO VALIDAÇÃO POR MÊS - (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD, TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b0plot = new BarPlot($data_0);
	$b0plot->SetLegend("Demandas");
	$b0plot->SetFillColor("green");
	$b0plot->value->Show();
	$b0plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b0plot->value->SetFormat('%01.0f');
	
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Serviços");
	$b1plot->SetFillColor("red");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');
	
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Pontuação");
	$b2plot->SetFillColor("orange");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b2plot->value->SetFormat('%01.0f');
	
	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Valor(R$)");
	$b3plot->SetFillColor("blue");
	$b3plot->value->Show();
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b3plot->value->SetFormatCallback('barValueFormat'); 
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b0plot,$b1plot,$b2plot,$b3plot));
	// Set color for the frame of each bar
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}

function demandasValidadas_pormes() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
	
			EXTRACT(MONTH FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) 
			|| '' ||
			EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1))
			as dpeid, 
			
			count(d.dmdid) as qtddemandas,
			
			sum(cast (pt.tsppontuacao as bigint)*
	
						 CASE WHEN d.dmdqtde > 0 THEN 
	
										   d.dmdqtde
	
							       ELSE
	
										    1          
						 END) 
	
						 as dshqtde,
						 
			sum((CASE WHEN d.dmdqtde > 0 THEN 
	
										   d.dmdqtde
	
							       ELSE
	
										    1          
						 END)) 
	
						 as qtdservicos,						 
						 
			(sum(cast (pt.tsppontuacao as bigint)*
	
			 (CASE WHEN d.dmdqtde > 0 THEN 
	
							   d.dmdqtde
	
				       ELSE
	
							    1          
	
			 END))* COALESCE(crtvlponto,0) ) as valor,
			 
			EXTRACT(MONTH FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as mes,
							
			EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano
	
			 FROM
			 demandas.demanda d
			 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
			 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
			 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
			 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
			 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
			 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
			 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
			 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
			 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
			 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
			 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
			 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
			 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
			 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
			 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
			 --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
			 
			 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
			 /*
			 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
							from 	workflow.historicodocumento a
								inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
						--where a.aedid in (146, 191)
                                                where c.esdid in (95)
						group by a.docid
						) ) as hst ON hst.docid = d.docid
						
			 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
							from 	workflow.historicodocumento a
								inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
								inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
						--where a.aedid in (146, 191)
                                                where c.esdid in (95)
						group by a.docid, a.hstid, b.cmddsc 
						) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
						
			 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
							from 	workflow.historicodocumento a
								inner join demandas.demanda d1 on a.docid = d1.docid
					  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
					  
			 LEFT JOIN (  select a.docid, u.usunome as nometecnicoclassif, a.usucpf AS usucpftecnicoclassif, to_char(min(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as dataclassif
							from 	workflow.historicodocumento a
								inner join seguranca.usuario u on u.usucpf = a.usucpf
								INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
						where a.aedid in (143,184) --143,184=Em analise (classificação da demanda)
						group by a.docid, u.usunome, a.usucpf ) as cla ON cla.docid = d.docid
					  
			 LEFT JOIN (select a.docid, u.usunome as nomegestor, a.usucpf AS usucpfgestor --to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datagestor
							from 	workflow.historicodocumento a
								inner join seguranca.usuario u on u.usucpf = a.usucpf
								INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
						where a.aedid in (224,186,165,278,279,368) --224,186,165=finalizada/validada -  278,279=invalidada - 368=validada fora do prazo
						group by a.docid, u.usunome, a.usucpf) as ges ON ges.docid = d.docid
			 */
			 LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
						
			 WHERE d.dmdstatus = 'A'  
			 AND od.ordid  IN  ('3')  				  	 	 
			 AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')	
			 AND  ed.esdid  IN  (93,111)  
			 --AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao <= '".formata_data_sql($datafim)." 23:59:59' )
			 AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
			 GROUP BY dpeid, mes, ano, crtvlponto
			 ORDER BY ano, mes ";
	
	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {

			if(!$_x_ax_[$data['dpeid']]['mes']) {
				$_x_ax_[$data['dpeid']]['mes']  = trim($db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$data['mes']."'"))."/".$data['ano'];
			}

			$_x_ax_[$data['dpeid']]['qtddemandas'] = $data['qtddemandas'];
			$_x_ax_[$data['dpeid']]['qtdservicos'] = $data['qtdservicos'];
			$_x_ax_[$data['dpeid']]['qtde'] = $data['dshqtde'];
			$_x_ax_[$data['dpeid']]['valor'] = $data['valor'];

		}
	}

	if($_x_ax_) {
		foreach($_x_ax_ as $d) {
			$eixo_x[] = $d['mes'];
			$data_0[] = $d['qtddemandas'];
			$totalizador['qtddemandas'] += $d['qtddemandas'];
			$data_1[] = $d['qtdservicos'];
			$totalizador['qtdservicos'] += $d['qtdservicos'];
			$data_2[] = $d['qtde'];
			$totalizador['qtde'] += $d['qtde'];
			$data_3[] = $d['valor'];
			$totalizador['valor'] += $d['valor'];			
		}
	}

	if($_REQUEST['media']=="1") {
		$data_0[] = round($totalizador['qtddemandas']/count($eixo_x));
		$data_1[] = round($totalizador['qtdservicos']/count($eixo_x));
		$data_2[] = round($totalizador['qtde']/count($eixo_x));
		$data_3[] = round($totalizador['valor']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_0 = round($totalizador['qtddemandas']/count($eixo_x));
		$dat_1 = round($totalizador['qtdservicos']/count($eixo_x));
		$dat_2 = round($totalizador['qtde']/count($eixo_x));
		$dat_3 = round($totalizador['valor']/count($eixo_x));
		$eix_x = "MÉDIA";
		unset($data_0,$data_1,$data_2,$data_3,$eixo_x);
		$data_0[] = $dat_0;
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_0[] = $totalizador['qtddemandas'];
		$data_1[] = $totalizador['qtdservicos'];
		$data_2[] = $totalizador['qtde'];
		$data_3[] = $totalizador['valor'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_0 = $totalizador['qtddemandas'];
		$dat_1 = $totalizador['qtdservicos'];
		$dat_2 = $totalizador['qtde'];
		$dat_3 = $totalizador['valor'];
		unset($data_0,$data_1,$data_2,$data_3,$eixo_x);
		$data_0[] = $dat_0;
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$data_3[] = $dat_3;
		$eixo_x[] = "TOTAL";

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("DEMANDAS VALIDADAS POR MÊS - (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD, TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 
	// Create the bar plots
	$b0plot = new BarPlot($data_0);
	$b0plot->SetLegend("Demandas");
	$b0plot->SetFillColor("green");
	$b0plot->value->Show();
	$b0plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b0plot->value->SetFormat('%01.0f');
	
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Serviços");
	$b1plot->SetFillColor("red");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');
	
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Pontuação");
	$b2plot->SetFillColor("orange");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b2plot->value->SetFormat('%01.0f');
	
	$b3plot = new BarPlot($data_3);
	$b3plot->SetLegend("Valor(R$)");
	$b3plot->SetFillColor("blue");
	$b3plot->value->Show();
	$b3plot->value->SetFont(FF_VERDANA,FS_NORMAL, PLOT_MES);
	$b3plot->value->SetFormatCallback('barValueFormat'); 
	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b0plot,$b1plot,$b2plot,$b3plot));
	// Set color for the frame of each bar
	$graph->Add($gbplot);
	// Finally send the graph to the browser
	$graph->Stroke();
}

function numerodemandas_mes_impressao() {
	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT 
	
			count(d.dmdid) as qtde,
			EXTRACT(MONTH FROM (select  h0.htddata
						from 	workflow.historicodocumento h0
						inner join demandas.demanda d0 on h0.docid = d0.docid
						where 	d.dmdid = d0.dmdid 
						order by h0.htddata desc 
						limit 1)) as mes,
			EXTRACT(YEAR FROM (select  h0.htddata
							from 	workflow.historicodocumento h0
							inner join demandas.demanda d0 on h0.docid = d0.docid
							where 	d.dmdid = d0.dmdid 
							order by h0.htddata desc 
							limit 1)) as ano
			 		 
		 FROM
		 
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
		 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
		 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
		 
		 --LEFT JOIN  (select max(avdid), dmdid, avdgeral as avaliacao from demandas.avaliacaodemanda where avdstatus='A' group by avdgeral, dmdid) AS avd ON avd.dmdid = d.dmdid
		 
		 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
		 /*
		 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
						from 	workflow.historicodocumento a
							--inner join workflow.documento c on c.docid = a.docid
					where a.aedid in (146, 191) 
					group by a.docid
					) ) as hst ON hst.docid = d.docid
					
		 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
						from 	workflow.historicodocumento a
							inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
							--inner join workflow.documento c on c.docid = a.docid
					where a.aedid in (146, 191) 
					group by a.docid, a.hstid, b.cmddsc 
					) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
					
		 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
						from 	workflow.historicodocumento a
							inner join demandas.demanda d1 on a.docid = d1.docid
				  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid

		 LEFT JOIN (  select a.docid, u.usunome as nometecnicoclassif, a.usucpf AS usucpftecnicoclassif, to_char(min(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as dataclassif
						from 	workflow.historicodocumento a
							inner join seguranca.usuario u on u.usucpf = a.usucpf
							INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (143,184) --143,184=Em analise (classificação da demanda)
					group by a.docid, u.usunome, a.usucpf ) as cla ON cla.docid = d.docid
				  
		LEFT JOIN (select max(a.hstid), a.docid, u.usunome as nomegestor, a.usucpf AS usucpfgestor --to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datagestor
						from 	workflow.historicodocumento a
							inner join seguranca.usuario u on u.usucpf = a.usucpf
							INNER JOIN ( SELECT DOCID, MAX(HSTID) as hstid FROM workflow.historicodocumento GROUP BY DOCID ) b ON b.hstid = a.hstid
					where a.aedid in (224,186,165,278,279,368) --224,186,165=finalizada/validada -  278,279=invalidada - 368=validada fora do prazo
					group by a.docid, u.usunome, a.usucpf) as ges ON ges.docid = d.docid
		 */		  	 	 
	 	 
		 WHERE d.dmdstatus = 'A'  
		 AND od.ordid  IN  ('12')  				  	 	 
	 	 
		 --AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')	
		 AND  ed.esdid  NOT IN (100,110) --CANCELADA
		 --AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' and datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
		 AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
		 GROUP BY mes, ano
		 ORDER BY ano, mes";
	
	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {
			$eixo_x[] = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer='".$data['mes']."'")."/".$data['ano'];
			$data_1[] = $data['qtde'];
			$totalizador += $data['qtde'];
		}
	}
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador/count($eixo_x));
		unset($data_1, $eixo_x);
		$data_1[] = $dat_1;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;

	}

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();
	// Set up the title for the graph
	$graph->title->Set("NÚMERO DE DEMANDAS POR MÊS - SERVIÇO DE IMPRESSÃO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");
	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);
	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);
	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);
	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,6); 
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetFormat('%01.0f');
	// Set color for the frame of each bar
	$graph->Add($b1plot);
	// Finally send the graph to the browser
	$graph->Stroke();
}


function numerodemandas_setor_impressao() {
	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	

	$sql = "SELECT  DISTINCT
    to_char( ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ,'MMYYYY') AS dpeid,   
    COUNT(d.dmdid) AS qtde,
    d.unaid AS unaid, -- Pode sair
    UPPER(uni.unasigla) AS nome
FROM demandas.demanda d
    LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
    LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid   
    LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
	/*
    LEFT JOIN  (SELECT 
                    d1.dmdid, 
                    TO_CHAR(MAX(a.htddata)::TIMESTAMP,'DD/MM/YYYY HH24:MI') AS datasit, 
                    MAX(a.htddata) AS datasituacao
                FROM    workflow.historicodocumento a
                INNER JOIN demandas.demanda d1 ON a.docid = d1.docid
                GROUP BY d1.dmdid ORDER BY 2 DESC
               ) AS dmd1 ON dmd1.dmdid = d.dmdid
	*/
WHERE d.dmdstatus = 'A'  
AND t.ordid  IN  ('3')                         
AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."') 
AND  ed.esdid  IN  (95,109,170)  
--AND  ( datasituacao >= '".formata_data_sql($dataini)." 00:00:00' AND datasituacao < '".formata_data_sql($datafim)." 23:59:59' )
AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'  
GROUP BY dpeid, d.unaid, UPPER(uni.unasigla)
ORDER BY UPPER(uni.unasigla)";

	$datas = $db->carregar($sql);

	if($datas[0]) {
		foreach($datas as $data) {
			$eixo_x[] = $data['nome'];
			//$eixo_x[] = $data['nome'];
			$data_1[] = $data['qtde'];
			$totalizador += $data['qtde'];
		}
	}
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador/count($eixo_x));
		unset($data_1, $eixo_x);
		$data_1[] = $dat_1;
		$eixo_x[] = "MÉDIA";
	}

	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;
	} elseif($_REQUEST['totalizador']=="2") {
		unset($data_1, $eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador;

	}


	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,200);
	$graph->SetScale("textlin");
	//$graph->SetY2Scale("lin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("NÚMERO DE DEMANDAS POR SETOR - SERVIÇO IMPRESSÃO (".$dataini." a ".$datafim.")");
	$graph->title->SetMargin(8);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL,6);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);

	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetFillColor("orange");
	$b1plot->value->Show();
	$b1plot->value->SetAngle(90);
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,8);
	$b1plot->value->SetFormat('%01.0f');

	// Create the grouped bar plot
	//$gbplot = new GroupBarPlot(array($b1plot));

	// Set color for the frame of each bar
	$graph->Add($b1plot);

	// Finally send the graph to the browser
	$graph->Stroke();
}


function atenddentroforaprazo_mes_impressao() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT DISTINCT
							lpad(cast(d.dmdid as varchar), 
								 case when length(cast(d.dmdid as varchar)) > 5 then 
								 	length(cast(d.dmdid as varchar)) 
							   	 else 
							   	 	5 
							   	 end 
							     , '0') AS nudemanda,
						     od.ordid as ordid,
							 od.orddescricao AS origemdemanda,
							 t.tipnome as tipodemanda,
							 smd.sidabrev || ' - ' || smd.siddescricao as sistema,
							 cel.celnome as celula,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    --servico
								    (
									    SELECT 
	                                    	(SELECT b.cmddsc FROM workflow.comentariodocumento b
	                                         WHERE b.hstid = max(a.hstid)
	                                         ) AS servico
				                        FROM workflow.historicodocumento a 
				                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid
			                        )
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada 
								    --servico
								    (
									    SELECT 
	                                    	(SELECT b.cmddsc FROM workflow.comentariodocumento b
	                                         WHERE b.hstid = max(a.hstid)
	                                         ) AS servico
				                        FROM workflow.historicodocumento a 
				                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid
			                        )
							       ELSE
								   ''
							  END) as servicoexec,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    ''
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								   --datadoc
								   (
									  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
									  from 	workflow.historicodocumento
									  where aedid in (146, 191) and docid = d.docid
									)
							       ELSE
								   to_char(now()::timestamp,'YYYY-MM-DD HH24:MI:00')
							  END) as datadocfinalizada,
					
							 (CASE WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								   --dataconc
								   (
									  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
									  from 	workflow.historicodocumento
									  where aedid in (146, 191) and docid = d.docid
									)
							       ELSE
								   ''
							  END) as dataconclusao,
							  
							 ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,		  
							 '' as observacao, 				
							 (CASE WHEN    d.dmddatafimprevatendimento < now() AND doc.esdid in (91,107,92,108)   THEN
							 		'' || d.dmdtitulo || ''
							 	   ELSE
							 	   	d.dmdtitulo
							 END) as assunto,
							 d.dmddsc as descricao,
							 to_char(d.dmddatainclusao::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
							 to_char(d.dmddatainiprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS datainicio,
							 to_char(d.dmddatafimprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS datafim,
							 '' as  prazoatendimento,
							 '' as  tempodecorrido,
							 '' as duracaoatendminutos,
							 '' as  tempopausa,
							 to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
							 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
							(CASE WHEN u.usunome <> '' THEN
							 	 		u.usunome
							 	   ELSE 
							 	   		u3.usunome || 'Solicitado em nome de: ' || d.dmdnomedemandante
							  END) AS solicitante,
							(CASE WHEN u2.usunome <> '' THEN
							 	 		u2.usunome
							 	   ELSE 
							 	   		'Não informado'
							  END) AS tecnico,		 
							 d.dmdnomedemandante as demandantegeral,
							 '(' || u.usufoneddd || ') ' || u.usufonenum AS tel,
							 upper(unasigla)||' - '||unadescricao as setor, 	
							 loc.lcadescricao as edificio,
							 aa.anddescricao AS andar,
							 d.dmdsalaatendimento as sala,
							 (CASE d.dmdclassificacaosistema
							 	WHEN '1' THEN 'Inicial'
							 	WHEN '2' THEN 'Consultiva'
							 	WHEN '3' THEN 'Investigativa'
							 	WHEN '4' THEN 'Manutenção corretiva'
							 	WHEN '5' THEN 'Manutenção evolutiva'
							 	ELSE 'Não Classificada'
							 END) AS classifsistema,
							 '' AS avaliacao,
							 (CASE WHEN ed.esddsc <> '' THEN
							 	 		ed.esddsc
							 	   ELSE 
							 	   		'Em processamento'
							 END) AS situacao,		 
							 (CASE WHEN p.pridsc <> '' THEN
							 	 		p.pridsc
							 	   ELSE 
							 	   		'Não Informado'
							 END) AS prioridade,
							 (CASE EXTRACT(month from d.dmddatainclusao)
								   			WHEN '1' THEN 'Janeiro/' || to_char(d.dmddatainclusao::date,'YYYY') 
											WHEN '2' THEN 'Fevereiro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '3' THEN 'Março/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '4' THEN 'Abril/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '5' THEN 'Maio/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '6' THEN 'Junho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '7' THEN 'Julho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '8' THEN 'Agosto/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '9' THEN 'Setembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '10' THEN 'Outubro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '11' THEN 'Novembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '12' THEN 'Dezembro/' || to_char(d.dmddatainclusao::date,'YYYY')
							 END) as mesano,
							 d.dmdhorarioatendimento,
						    (CASE WHEN pt.tsppontuacao is not null THEN 
							 			pt.tsppontuacao
							 	   ELSE
							 	   		'0'	 
							 END) AS pontuacao,		 
							 '' as tempoclassif,
							 '0' as totalpontuacao,
							(CASE WHEN d.dmdqtde > 0 THEN 
							 			d.dmdqtde
							 	   ELSE
							 	   		'1'	 
							 END) AS qtdservico,
							 '1' AS totaldemandas,
							 CASE
				                 WHEN od.ordid in (18,19,20,21) THEN
				            		 lpad(tsphora::varchar,3,'0') || ':' || lpad(tspminuto::varchar,2,'0')
				            	 ELSE
				            		 lpad(tsphora::varchar,2,'0') || ':' || lpad(tspminuto::varchar,2,'0')
				             END as prazocatalogo,
							 to_char(d.dmdtempoadicional, 'HH24:MI') as tempoadicional,
							 d.dmdobstempoadicional as justtempoadicional,
							 '0' as valordemanda,
							 d.usucpfexecutor,
							 EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as mes,
							 EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as ano,
							 ed.esdid,
							 ed.esddsc,
							 COALESCE(crtvlponto,0) as valorponto
							 FROM
							 demandas.demanda d
							 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
							 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
							 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
							 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
							 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
							 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
							 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
							 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
							 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
							 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
							 --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
							 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
							 
							 /*
							 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
											from 	workflow.historicodocumento a
												inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
										where a.aedid in (146, 191) 
										group by a.docid
										) ) as hst ON hst.docid = d.docid
										
							 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
											from 	workflow.historicodocumento a
												inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
												inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
										where a.aedid in (146, 191) 
										group by a.docid, a.hstid, b.cmddsc 
										) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
										
							 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
											from 	workflow.historicodocumento a
												inner join demandas.demanda d1 on a.docid = d1.docid
									  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
							 */
							 
							 LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
						 	 
							 WHERE d.dmdstatus = 'A'  AND od.ordid  IN  ('12')  
							 --AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
							 --AND  ed.esdid NOT IN (100,110) --CANCELADA
							 AND  ed.esdid IN (95,170) --VALIDADA/VALIDADA SEM PAUSA
							 AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'
							 ORDER BY  ano, mes, tecnico, datadocfinalizada";
	
	$dados = $db->carregar($sql);
	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['mes']]['mes']) {
				$dados_bruto[$dado['mes']]['mes'] = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$dado['mes']."'")."/".$dado['ano'];
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}
				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			
			//verifica se a situação é 'Validada Fora do Prazo' se sim, despreza o tempo da pausa
			if($dado['esdid'] == DEMANDA_ESTADO_VALIDADA_FORA_PRAZO) $tempototalpausa = 0; 
			
			
			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['mes']]['vermelho']++;
					$dados_bruto[$dado['mes']]['valorvermelho'] = $dados_bruto[$dado['mes']]['valorvermelho'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['mes']]['azul']++;
					$dados_bruto[$dado['mes']]['valorazul'] = $dados_bruto[$dado['mes']]['valorazul'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
			}
			else{
				$dados_bruto[$dado['mes']]['vermelho']++;
			}

		}
	}
	
	
	
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['azul'];
			$data_1_valorAzul[$d['azul']] = $d['valorazul'];
			$totalizador['azul'] += $d['azul'];
			$data_2[] = $d['vermelho'];
			$data_2_valorVermelho[$d['vermelho']] = $d['valorvermelho'];
			$totalizador['vermelho'] += $d['vermelho'];
		}
	}
	
	$arValores = array();
	if($data_1_valorAzul){
		foreach($data_1_valorAzul as $key=>$valorazul){
			$arValores['azul'][$key] = $valorazul;
		}
	}
	if($data_2_valorVermelho){
		foreach($data_2_valorVermelho as $key=>$valorvermelho){
			$arValores['vermelho'][$key] = $valorvermelho;
		}
	}
	
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['azul']/count($eixo_x));
		$data_2[] = round($totalizador['vermelho']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['azul']/count($eixo_x));
		$dat_2 = round($totalizador['vermelho']/count($eixo_x));
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['azul'];
		$data_2[] = $totalizador['vermelho'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['azul'];
		$dat_2 = $totalizador['vermelho'];
		unset($data_1,$data_2,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;		
		$data_2[] = $dat_2;
	}
	
	//if(!$data_1) $data_1[] = 0;
	//if(!$data_2) $data_2[] = 0;
	

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO POR MÊS (".$dataini." a ".$datafim.") - SERVIÇO DE IMPRESSÃO");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Dentro do prazo");
	$b1plot->SetFillColor("blue");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetTypeGraph('azul');
	$b1plot->value->SetFormatCallbackParam('barValueFormat2',$arValores);
	
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Fora do prazo");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b2plot->value->SetTypeGraph('vermelho');
	$b2plot->value->SetFormatCallbackParam('barValueFormat2',$arValores);


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();

}



function atenddentroforaprazo_pizza_impressao() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

	$sql = "SELECT  DISTINCT
							lpad(cast(d.dmdid as varchar), 
								 case when length(cast(d.dmdid as varchar)) > 5 then 
								 	length(cast(d.dmdid as varchar)) 
							   	 else 
							   	 	5 
							   	 end 
							     , '0') AS nudemanda,
						     od.ordid as ordid,
							 od.orddescricao AS origemdemanda,
							 t.tipnome as tipodemanda,
							 smd.sidabrev || ' - ' || smd.siddescricao as sistema,
							 cel.celnome as celula,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    --servico
								    (
									    SELECT 
	                                    	(SELECT b.cmddsc FROM workflow.comentariodocumento b
	                                         WHERE b.hstid = max(a.hstid)
	                                         ) AS servico
				                        FROM workflow.historicodocumento a 
				                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid
			                        )
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada 
								    --servico
								    (
									    SELECT 
	                                    	(SELECT b.cmddsc FROM workflow.comentariodocumento b
	                                         WHERE b.hstid = max(a.hstid)
	                                         ) AS servico
				                        FROM workflow.historicodocumento a 
				                        WHERE a.aedid in (146, 191,224, 703) and docid=d.docid
			                        )
							       ELSE
								   ''
							  END) as servicoexec,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    ''
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								    --datadoc
								    (
									  select to_char(max(htddata)::timestamp,'YYYY-MM-DD HH24:MI:00')
									  from 	workflow.historicodocumento
									  where aedid in (146, 191) and docid = d.docid
									 )
							       ELSE
								   to_char(now()::timestamp,'YYYY-MM-DD HH24:MI:00')
							  END) as datadocfinalizada,
					
							 (CASE WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								    --dataconc
								    (
									  select to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI')
									  from 	workflow.historicodocumento
									  where aedid in (146, 191) and docid = d.docid
									 )
							       ELSE
								   ''
							  END) as dataconclusao,
							  
							 ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,		  
							 '' as observacao, 				
							 (CASE WHEN    d.dmddatafimprevatendimento < now() AND doc.esdid in (91,107,92,108)   THEN
							 		'' || d.dmdtitulo || ''
							 	   ELSE
							 	   	d.dmdtitulo
							 END) as assunto,
							 d.dmddsc as descricao,
							 to_char(d.dmddatainclusao::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
							 to_char(d.dmddatainiprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS datainicio,
							 to_char(d.dmddatafimprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS datafim,
							 '' as  prazoatendimento,
							 '' as  tempodecorrido,
							 '' as duracaoatendminutos,
							 '' as  tempopausa,
							 to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
							 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
							(CASE WHEN u.usunome <> '' THEN
							 	 		u.usunome
							 	   ELSE 
							 	   		u3.usunome || 'Solicitado em nome de: ' || d.dmdnomedemandante
							  END) AS solicitante,
							(CASE WHEN u2.usunome <> '' THEN
							 	 		u2.usunome
							 	   ELSE 
							 	   		'Não informado'
							  END) AS tecnico,		 
							 d.dmdnomedemandante as demandantegeral,
							 '(' || u.usufoneddd || ') ' || u.usufonenum AS tel,
							 upper(unasigla)||' - '||unadescricao as setor, 	
							 loc.lcadescricao as edificio,
							 aa.anddescricao AS andar,
							 d.dmdsalaatendimento as sala,
							 (CASE d.dmdclassificacaosistema
							 	WHEN '1' THEN 'Inicial'
							 	WHEN '2' THEN 'Consultiva'
							 	WHEN '3' THEN 'Investigativa'
							 	WHEN '4' THEN 'Manutenção corretiva'
							 	WHEN '5' THEN 'Manutenção evolutiva'
							 	ELSE 'Não Classificada'
							 END) AS classifsistema,
							 '' AS avaliacao,
							 (CASE WHEN ed.esddsc <> '' THEN
							 	 		ed.esddsc
							 	   ELSE 
							 	   		'Em processamento'
							 END) AS situacao,		 
							 (CASE WHEN p.pridsc <> '' THEN
							 	 		p.pridsc
							 	   ELSE 
							 	   		'Não Informado'
							 END) AS prioridade,
							 (CASE EXTRACT(month from d.dmddatainclusao)
								   			WHEN '1' THEN 'Janeiro/' || to_char(d.dmddatainclusao::date,'YYYY') 
											WHEN '2' THEN 'Fevereiro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '3' THEN 'Março/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '4' THEN 'Abril/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '5' THEN 'Maio/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '6' THEN 'Junho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '7' THEN 'Julho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '8' THEN 'Agosto/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '9' THEN 'Setembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '10' THEN 'Outubro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '11' THEN 'Novembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '12' THEN 'Dezembro/' || to_char(d.dmddatainclusao::date,'YYYY')
							 END) as mesano,
							 d.dmdhorarioatendimento,
						    (CASE WHEN pt.tsppontuacao is not null THEN 
							 			pt.tsppontuacao
							 	   ELSE
							 	   		'0'	 
							 END) AS pontuacao,		 
							 '' as tempoclassif,
							 '0' as totalpontuacao,
							(CASE WHEN d.dmdqtde > 0 THEN 
							 			d.dmdqtde
							 	   ELSE
							 	   		'1'	 
							 END) AS qtdservico,
							 '1' AS totaldemandas,
							 CASE
				                 WHEN od.ordid in (18,19,20,21) THEN
				            		 lpad(tsphora::varchar,3,'0') || ':' || lpad(tspminuto::varchar,2,'0')
				            	 ELSE
				            		 lpad(tsphora::varchar,2,'0') || ':' || lpad(tspminuto::varchar,2,'0')
				             END as prazocatalogo,
							 to_char(d.dmdtempoadicional, 'HH24:MI') as tempoadicional,
							 d.dmdobstempoadicional as justtempoadicional,
							 '0' as valordemanda,
							 d.usucpfexecutor,
							 EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as mes,
							 ed.esdid,
							 ed.esddsc
					 
							 FROM
							 demandas.demanda d
							 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
							 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
							 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
							 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
							 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
							 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
							 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
							 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
							 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
							 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
							 --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
							 
							 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
							 /*
							 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
											from 	workflow.historicodocumento a
												inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
										where a.aedid in (146, 191) 
										group by a.docid
										) ) as hst ON hst.docid = d.docid
										
							 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
											from 	workflow.historicodocumento a
												inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
												inner join workflow.documento c on c.docid = a.docid and c.tpdid in (31,35)
										where a.aedid in (146, 191) 
										group by a.docid, a.hstid, b.cmddsc 
										) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
										
							 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
											from 	workflow.historicodocumento a
												inner join demandas.demanda d1 on a.docid = d1.docid
									  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
							 */
						 	 
							 WHERE d.dmdstatus = 'A'  AND od.ordid  IN  ('12')  
							 --AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
							 --AND  ed.esdid  NOT IN (100,110) --CANCELADA
							 AND  ed.esdid IN (95,170) --VALIDADA/VALIDADA SEM PAUSA
							 AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'
							 ORDER BY  mes, tecnico, datadocfinalizada";
	
	$dados = $db->carregar($sql);
	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['mes']]['mes']) {
				$dados_bruto[$dado['mes']]['mes'] = $dado['mes'];
			}

			$total_minuto = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['dmddatafimprevatendimento'], $dado['dmdhorarioatendimento'], $dado['ordid']);
			//verifica pausa da demanda
			$sql = "select t.tpadsc, p.pdmdatainiciopausa, p.pdmdatafimpausa, p.pdmjustificativa, to_char(p.pdmdatainiciopausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausaini, to_char(p.pdmdatafimpausa::timestamp,'DD/MM/YYYY HH24:MI') AS datapausafim
					from demandas.pausademanda p 
					inner join demandas.tipopausademanda t ON t.tpaid = p.tpaid
					where p.dmdid = ". (int) $dado['nudemanda'];

			$dadosp = $db->carregar($sql);


			$flagIndeterminado = '';
			$tempototalpausa = 0;
			$textotempopausa = "<div align='left'>";
			$horasx = 0;
			$minutosx = 0;

			if($dadosp){
				foreach($dadosp as $dadop){

					if($dadop['pdmdatainiciopausa'] && $dadop['pdmdatafimpausa']){

						$ano_inip	= substr($dadop['pdmdatainiciopausa'],0,4);
						$mes_inip	= substr($dadop['pdmdatainiciopausa'],5,2);
						$dia_inip	= substr($dadop['pdmdatainiciopausa'],8,2);
						$hor_inip	= substr($dadop['pdmdatainiciopausa'],11,2);
						$min_inip	= substr($dadop['pdmdatainiciopausa'],14,2);
							
						$ano_fimp	= substr($dadop['pdmdatafimpausa'],0,4);
						$mes_fimp	= substr($dadop['pdmdatafimpausa'],5,2);
						$dia_fimp	= substr($dadop['pdmdatafimpausa'],8,2);
						$hor_fimp	= substr($dadop['pdmdatafimpausa'],11,2);
						$min_fimp	= substr($dadop['pdmdatafimpausa'],14,2);

						$dinip = mktime($hor_inip,$min_inip,0,$mes_inip,$dia_inip,$ano_inip); // timestamp da data inicial
						$dfimp = mktime($hor_fimp,$min_fimp,0,$mes_fimp,$dia_fimp,$ano_fimp); // timestamp da data final

						// pega o tempo total da pausa
						$tempototalpausa = $tempototalpausa + ($dfimp - $dinip);


						$dtiniinvert = $ano_inip.'-'.$mes_inip.'-'.$dia_inip.' '.$hor_inip.':'.$min_inip.':00';
						$dtfiminvert = $ano_fimp.'-'.$mes_fimp.'-'.$dia_fimp.' '.$hor_fimp.':'.$min_fimp.':00';

					}

					//monta o texto da tempopausa
					$textotempopausa .= "<b>Tipo:</b> ". $dadop['tpadsc'];
					$textotempopausa .= "<br><b>Justificativa:</b> ". $dadop['pdmjustificativa']."";
					$textotempopausa .= "<br><b>Data início:</b> ". $dadop['datapausaini']."";
					if($dadop['datapausafim']){
						$textotempopausa .= "<br><b>Data término:</b> ". $dadop['datapausafim']."";
					}else{
						$textotempopausa .= "<br><b>Data término:</b> Indeterminado";
					}

					if($dadop['pdmdatafimpausa']){
						$tempop = $classdata->diferencaEntreDatas(  $dtiniinvert, $dtfiminvert, 'tempoEntreDadas', 'string','yyyy/mm/dd');
						if(!$tempop) $tempop = '0 minuto';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> ".$tempop;
					}else{
						$flagIndeterminado = ' + <font color=red>Tempo Indeterminado</font>';
						$textotempopausa .= "<br><b>Tempo da Pausa:</b> Indeterminado";
					}

					$textotempopausa .= "<BR><BR>";

				}



				//if($flagIndeterminado == '1')
				//	$textotempopausa .= "TOTAL (Tempo da Pausa): Indeterminado";
				//else{
				$datainiaux = date('Y-m-d H:i').':00';
				$ano_aux	= substr($datainiaux,0,4);
				$mes_aux	= substr($datainiaux,5,2);
				$dia_aux	= substr($datainiaux,8,2);
				$hor_aux	= substr($datainiaux,11,2);
				$min_aux	= substr($datainiaux,14,2);
					
				$datafinalaux = mktime($hor_aux,$min_aux,0+$tempototalpausa,$mes_aux,$dia_aux,$ano_aux);
				$datafinalaux2 = strftime("%Y-%m-%d %H:%M:%S", $datafinalaux);
				$tempototalp = $classdata->diferencaEntreDatas(  $datainiaux, $datafinalaux2, 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$textotempopausa .= "<b>TOTAL (Tempo da Pausa):</b> ". $tempototalp . $flagIndeterminado;
				//}

					
				//pega prioridade e data termino
				$sql = "select dmdhorarioatendimento as dmdhorarioatendimentop, to_char(dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatafimprevatendimentop
						from demandas.demanda 
						where dmdid = ". (int) $dado['nudemanda'];
				$dadosdmd = $db->carregar($sql);

				$resto = $tempototalpausa;
				$horas 			= $resto/3600; //quantidade de horas
				$intHoras 		= floor($horas);
				if($intHoras >= 1){	//se houver horas
					$horasx = $intHoras;
					$resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
				}

				$minutos 		= $resto/60; //quantidade de minutos
				$intMinutos 	= floor($minutos);
				if($intMinutos >= 1){ //se houver minutos
					$minutosx = $intMinutos;
					$resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
				}

				if(!$horasx) $horasx = "00";
				if(strlen($horasx) == 1) $horasx = "0".$horasx;
				if(!$minutosx) $minutosx = "00";
				if(strlen($minutosx) == 1) $minutosx = "0".$minutosx;
					
				$hormin = $horasx.":".$minutosx;

				$vfdtfim = verificaCalculoTempoDtfim($dadosdmd[0]['dmddatafimprevatendimentop'], $hormin, $dadosdmd[0]['dmdhorarioatendimentop'], $dado['dataconclusao'], $dado['ordid']);

				if($flagIndeterminado){
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=red>Data Indeterminada</font>";
				}
				else{
					$textotempopausa .= "<br><br><b>Data Prevista de Término:</b> <font color=black>".$vfdtfim."</font>";
				}
					
			}

			$textotempopausa .= "</div>";

			//atribui o campo tem tempo da pausa
			$dado['tempopausa'] = $textotempopausa;

			$ano_ini	= substr($dado['dmddatainiprevatendimento'],0,4);
			$mes_ini	= substr($dado['dmddatainiprevatendimento'],5,2);
			$dia_ini	= substr($dado['dmddatainiprevatendimento'],8,2);
			$hor_ini	= substr($dado['dmddatainiprevatendimento'],11,2);
			$min_ini	= substr($dado['dmddatainiprevatendimento'],14,2);

			$dataFinal = mktime($hor_ini,$min_ini+$total_minuto,0+$tempototalpausa,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$dataFinalPrazoPrev = strftime("%Y-%m-%d %H:%M:%S", $dataFinal);

			$dado['prazoatendimento'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalPrazoPrev , 'tempoEntreDadas', 'string','yyyy/mm/dd');
			if($dado['datadocfinalizada']){
					
				//calcula Duração do atendimento
				$total_minuto_conclusao = calculaTempoMinuto($dado['dmddatainiprevatendimento'], $dado['datadocfinalizada'], $dado['dmdhorarioatendimento'], $dado['ordid']);
				$dataFinalConc = mktime($hor_ini,$min_ini+$total_minuto_conclusao,0,$mes_ini,$dia_ini,$ano_ini);
				$dataFinalConclusao = strftime("%Y-%m-%d %H:%M:%S", $dataFinalConc);
				$total_prazoatendimento = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalPrazoPrev))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );
				$total_tempodecorrido = (float) ( str_replace(':','',str_replace(' ','',str_replace('-','',$dataFinalConclusao))) - str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dmddatainiprevatendimento']))) );

				$dado['tempodecorrido'] = $classdata->diferencaEntreDatas(  $dado['dmddatainiprevatendimento'], $dataFinalConclusao , 'tempoEntreDadas', 'string','yyyy/mm/dd');
				$dado['duracaoatendminutos'] = $total_minuto_conclusao;


				if($total_tempodecorrido > $total_prazoatendimento){
					$dado['tempodecorrido'] = "<font color=red>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=red>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['usucpfexecutor']]['vermelho']++;
				}
				else{
					$dado['tempodecorrido'] = "<font color=blue>". $dado['tempodecorrido'] . "</font>";
					$dado['dataconclusao'] = "<font color=blue>". $dado['dataconclusao'] . "</font>";
					$dados_bruto[$dado['usucpfexecutor']]['azul']++;
				}
			}
			else{
				$dados_bruto[$dado['usucpfexecutor']]['vermelho']++;
			}

		}
	}
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['mes'];
			$totalizador['dentro'] += $d['azul'];
			$totalizador['fora'] += $d['vermelho'];
		}
	}

	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	require_once ('../../includes/jpgraph/jpgraph_pie3d.php');
	
	$data = array($totalizador['dentro'],
				  $totalizador['fora']);
	
	$graph = new PieGraph(800,440);
	$graph->SetShadow();
	$graph->title->Set("ATENDIMENTO DENTRO/FORA DO PRAZO - PIZZA (".$dataini." a ".$datafim.") - SERVIÇO DE IMPRESSÃO");
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,8); 
	$xx = new PiePlot3D($data);
	$cores = array("blue","red");
	$xx->SetSliceColors($cores);
	$xx->value->SetFormat('%01.1f%%');
	$xx->value->HideZero();
	$xx->SetSize(0.5);
	$xx->SetCenter(0.45);
	$legendas = array("Dentro do prazo (".$totalizador['dentro'].")","Fora do prazo (".$totalizador['fora'].")");
	$xx->SetLegends($legendas);
	//$xx->ExplodeAll(10);
	$xx->SetShadow();
	$graph->Add($xx);
	$graph->Stroke();

}



function classdentroforaprazo_mes_impressao() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);
	
	$sql = "SELECT  DISTINCT
							lpad(cast(d.dmdid as varchar), 
								 case when length(cast(d.dmdid as varchar)) > 5 then 
								 	length(cast(d.dmdid as varchar)) 
							   	 else 
							   	 	5 
							   	 end 
							     , '0') AS nudemanda,
						     od.ordid as ordid,
							 od.orddescricao AS origemdemanda,
							 t.tipnome as tipodemanda,
							 smd.sidabrev || ' - ' || smd.siddescricao as sistema,
							 cel.celnome as celula,
					
							 ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,		  
							 '' as observacao, 				
							 (CASE WHEN    d.dmddatafimprevatendimento < now() AND doc.esdid in (91,107,92,108)   THEN
							 		'' || d.dmdtitulo || ''
							 	   ELSE
							 	   	d.dmdtitulo
							 END) as assunto,
							 d.dmddsc as descricao,
							 to_char(d.dmddatainclusao::timestamp,'YYYY-MM-DD HH24:MI:00') AS dataabertura,
							 to_char(d.dmddatainiprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS datainicio,
							 to_char(d.dmddatafimprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS datafim,
							 '' as  prazoatendimento,
							 '' as  tempodecorrido,
							 '' as duracaoatendminutos,
							 '' as  tempopausa,
							 to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
							 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
							(CASE WHEN u.usunome <> '' THEN
							 	 		u.usunome
							 	   ELSE 
							 	   		u3.usunome || 'Solicitado em nome de: ' || d.dmdnomedemandante
							  END) AS solicitante,
							(CASE WHEN u2.usunome <> '' THEN
							 	 		u2.usunome
							 	   ELSE 
							 	   		'Não informado'
							  END) AS tecnico,		 
							 d.dmdnomedemandante as demandantegeral,
							 '(' || u.usufoneddd || ') ' || u.usufonenum AS tel,
							 upper(unasigla)||' - '||unadescricao as setor, 	
							 loc.lcadescricao as edificio,
							 aa.anddescricao AS andar,
							 d.dmdsalaatendimento as sala,
							 (CASE d.dmdclassificacaosistema
							 	WHEN '1' THEN 'Inicial'
							 	WHEN '2' THEN 'Consultiva'
							 	WHEN '3' THEN 'Investigativa'
							 	WHEN '4' THEN 'Manutenção corretiva'
							 	WHEN '5' THEN 'Manutenção evolutiva'
							 	ELSE 'Não Classificada'
							 END) AS classifsistema,
							 '' AS avaliacao,
							 (CASE WHEN ed.esddsc <> '' THEN
							 	 		ed.esddsc
							 	   ELSE 
							 	   		'Em processamento'
							 END) AS situacao,		 
							 (CASE WHEN p.pridsc <> '' THEN
							 	 		p.pridsc
							 	   ELSE 
							 	   		'Não Informado'
							 END) AS prioridade,
							 (CASE EXTRACT(month from d.dmddatainclusao)
								   			WHEN '1' THEN 'Janeiro/' || to_char(d.dmddatainclusao::date,'YYYY') 
											WHEN '2' THEN 'Fevereiro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '3' THEN 'Março/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '4' THEN 'Abril/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '5' THEN 'Maio/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '6' THEN 'Junho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '7' THEN 'Julho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '8' THEN 'Agosto/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '9' THEN 'Setembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '10' THEN 'Outubro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '11' THEN 'Novembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '12' THEN 'Dezembro/' || to_char(d.dmddatainclusao::date,'YYYY')
							 END) as mesano,
							 d.dmdhorarioatendimento,
						    (CASE WHEN pt.tsppontuacao is not null THEN 
							 			pt.tsppontuacao
							 	   ELSE
							 	   		'0'	 
							 END) AS pontuacao,		 
							 '' as tempoclassif,
							 '0' as totalpontuacao,
							(CASE WHEN d.dmdqtde > 0 THEN 
							 			d.dmdqtde
							 	   ELSE
							 	   		'1'	 
							 END) AS qtdservico,
							 '1' AS totaldemandas,
							 
							 to_char(d.dmdtempoadicional, 'HH24:MI') as tempoadicional,
							 d.dmdobstempoadicional as justtempoadicional,
							 '0' as valordemanda,
							 d.usucpfexecutor,
							 EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as mes,
							 EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as ano,
							 ed.esdid,
							 ed.esddsc,
							 COALESCE(crtvlponto,0) as valorponto,
							 d.dmddataclassificacao as dataclassificacao,
							 d.dmddataclassificacaosi as dataclassificacaosi,
							 pt.tsptempoclassif
							 FROM
							 demandas.demanda d
							 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
							 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
							 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
							 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
							 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
							 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
							 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
							 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
							 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
							 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
							 
							 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
							 
							 /*			
							 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
											from 	workflow.historicodocumento a
												inner join demandas.demanda d1 on a.docid = d1.docid
									  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
					         */
						 	 LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
									  
							 WHERE d.dmdstatus = 'A'  AND od.ordid  IN  ('12')  
							 --AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
							 --AND  ed.esdid  NOT IN (100,110) --CANCELADA 
							 AND  ed.esdid IN (95,170) --VALIDADA/VALIDADA SEM PAUSA
							 --AND  datasituacao between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'
							 AND  ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59' 
							 ORDER BY  ano, mes, tecnico";
	
	/*
	$sql = "SELECT  DISTINCT
							lpad(cast(d.dmdid as varchar), 
								 case when length(cast(d.dmdid as varchar)) > 5 then 
								 	length(cast(d.dmdid as varchar)) 
							   	 else 
							   	 	5 
							   	 end 
							     , '0') AS nudemanda,
						     od.ordid as ordid,
							 od.orddescricao AS origemdemanda,
							 t.tipnome as tipodemanda,
							 smd.sidabrev || ' - ' || smd.siddescricao as sistema,
							 cel.celnome as celula,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    servico
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada 
								    servico
							       ELSE
								   ''
							  END) as servicoexec,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    ''
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								    datadoc
							       ELSE
								   to_char(now()::timestamp,'YYYY-MM-DD HH24:MI:00')
							  END) as datadocfinalizada,
					
							 (CASE WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								    dataconc
							       ELSE
								   ''
							  END) as dataconclusao,
							  
							 datasit as datasituacao,		  
							 '' as observacao, 				
							 (CASE WHEN    d.dmddatafimprevatendimento < now() AND doc.esdid in (91,107,92,108)   THEN
							 		'' || d.dmdtitulo || ''
							 	   ELSE
							 	   	d.dmdtitulo
							 END) as assunto,
							 d.dmddsc as descricao,
							 to_char(d.dmddatainclusao::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
							 to_char(d.dmddatainiprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS datainicio,
							 to_char(d.dmddatafimprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS datafim,
							 '' as  prazoatendimento,
							 '' as  tempodecorrido,
							 '' as duracaoatendminutos,
							 '' as  tempopausa,
							 to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
							 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
							(CASE WHEN u.usunome <> '' THEN
							 	 		u.usunome
							 	   ELSE 
							 	   		u3.usunome || 'Solicitado em nome de: ' || d.dmdnomedemandante
							  END) AS solicitante,
							(CASE WHEN u2.usunome <> '' THEN
							 	 		u2.usunome
							 	   ELSE 
							 	   		'Não informado'
							  END) AS tecnico,		 
							 d.dmdnomedemandante as demandantegeral,
							 '(' || u.usufoneddd || ') ' || u.usufonenum AS tel,
							 upper(unasigla)||' - '||unadescricao as setor, 	
							 loc.lcadescricao as edificio,
							 aa.anddescricao AS andar,
							 d.dmdsalaatendimento as sala,
							 (CASE d.dmdclassificacaosistema
							 	WHEN '1' THEN 'Inicial'
							 	WHEN '2' THEN 'Consultiva'
							 	WHEN '3' THEN 'Investigativa'
							 	WHEN '4' THEN 'Manutenção corretiva'
							 	WHEN '5' THEN 'Manutenção evolutiva'
							 	ELSE 'Não Classificada'
							 END) AS classifsistema,
							 '' AS avaliacao,
							 (CASE WHEN ed.esddsc <> '' THEN
							 	 		ed.esddsc
							 	   ELSE 
							 	   		'Em processamento'
							 END) AS situacao,		 
							 (CASE WHEN p.pridsc <> '' THEN
							 	 		p.pridsc
							 	   ELSE 
							 	   		'Não Informado'
							 END) AS prioridade,
							 (CASE EXTRACT(month from d.dmddatainclusao)
								   			WHEN '1' THEN 'Janeiro/' || to_char(d.dmddatainclusao::date,'YYYY') 
											WHEN '2' THEN 'Fevereiro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '3' THEN 'Março/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '4' THEN 'Abril/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '5' THEN 'Maio/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '6' THEN 'Junho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '7' THEN 'Julho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '8' THEN 'Agosto/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '9' THEN 'Setembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '10' THEN 'Outubro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '11' THEN 'Novembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '12' THEN 'Dezembro/' || to_char(d.dmddatainclusao::date,'YYYY')
							 END) as mesano,
							 d.dmdhorarioatendimento,
						    (CASE WHEN pt.tsppontuacao is not null THEN 
							 			pt.tsppontuacao
							 	   ELSE
							 	   		'0'	 
							 END) AS pontuacao,		 
							 '' as tempoclassif,
							 '0' as totalpontuacao,
							(CASE WHEN d.dmdqtde > 0 THEN 
							 			d.dmdqtde
							 	   ELSE
							 	   		'1'	 
							 END) AS qtdservico,
							 '1' AS totaldemandas,
							 to_char(pt.tsptempo, 'HH24:MI') as prazocatalogo,
							 to_char(d.dmdtempoadicional, 'HH24:MI') as tempoadicional,
							 d.dmdobstempoadicional as justtempoadicional,
							 '0' as valordemanda,
							 d.usucpfexecutor,
							 EXTRACT(MONTH FROM datasituacao) as mes,
							 EXTRACT(YEAR FROM datasituacao) as ano,
							 ed.esdid,
							 (select crtvlponto from demandas.contrato where crtstatus='A' and (current_timestamp between crtdtinicio and crtdtfim) limit 1) as valorponto
							 FROM
							 demandas.demanda d
							 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
							 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
							 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
							 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
							 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
							 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
							 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
							 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
							 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
							 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
							 --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
							 
							 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
							 
							 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
											from 	workflow.historicodocumento a
												inner join workflow.documento c on c.docid = a.docid
										where a.aedid in (146, 191) 
										group by a.docid
										) ) as hst ON hst.docid = d.docid
										
							 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
											from 	workflow.historicodocumento a
												inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
												inner join workflow.documento c on c.docid = a.docid
										where a.aedid in (146, 191) 
										group by a.docid, a.hstid, b.cmddsc 
										) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
										
							 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
											from 	workflow.historicodocumento a
												inner join demandas.demanda d1 on a.docid = d1.docid
									  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
					
						 	 
							 WHERE d.dmdstatus = 'A'  AND od.ordid  IN  ('12')  
							 --AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
							 AND  ed.esdid  IN  (95,109,170)  
							 AND  datasituacao between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59' 
							 ORDER BY  ano, mes, tecnico, datadocfinalizada";
	*/
	
	$dados = $db->carregar($sql);
	
	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['mes']]['mes']) {
				$dados_bruto[$dado['mes']]['mes'] = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$dado['mes']."'")."/".$dado['ano'];
				$dados_bruto[$dado['mes']]['vermelho'] = 0;
				$dados_bruto[$dado['mes']]['azul'] = 0;
			}
			
			$dtabertura = $dado['dataclassificacao'];

			$hor_tc	= substr($dado['tsptempoclassif'],0,2);
			$min_tc	= substr($dado['tsptempoclassif'],3,2);
			
			$ano_ini	= substr($dtabertura,0,4);
			$mes_ini	= substr($dtabertura,5,2);
			$dia_ini	= substr($dtabertura,8,2);
			$hor_ini	= substr($dtabertura,11,2);
			$min_ini	= substr($dtabertura,14,2);

			
			$datainif = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $datainif);
			

			if($datainiFinal && $dado['dataclassificacaosi']){
				$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
				$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dataclassificacaosi'])));
				
				if($dtxfim > $dtxini){
					$dados_bruto[$dado['mes']]['vermelho']++;
					//$dados_bruto[$dado['mes']]['valorvermelho'] = $dados_bruto[$dado['mes']]['valorvermelho'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
				else{
					$dados_bruto[$dado['mes']]['azul']++;
					//$dados_bruto[$dado['mes']]['valorazul'] = $dados_bruto[$dado['mes']]['valorazul'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
			}	
			else{
				$dados_bruto[$dado['mes']]['vermelho']++;
			}		
			
		}
	}
	
		
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['mes'];
			$data_1[] = $d['azul'];
			$data_1_valorAzul[$d['azul']] = $d['valorazul'];
			$totalizador['azul'] += $d['azul'];
			$data_2[] = $d['vermelho'];
			$data_2_valorVermelho[$d['vermelho']] = $d['valorvermelho'];
			$totalizador['vermelho'] += $d['vermelho'];
		}
	}
	
	$arValores = array();
	if($data_1_valorAzul){
		foreach($data_1_valorAzul as $key=>$valorazul){
			$arValores['azul'][$key] = $valorazul;
		}
	}
	if($data_2_valorVermelho){
		foreach($data_2_valorVermelho as $key=>$valorvermelho){
			$arValores['vermelho'][$key] = $valorvermelho;
		}
	}
	
	if($_REQUEST['media']=="1") {
		$data_1[] = round($totalizador['azul']/count($eixo_x));
		$data_2[] = round($totalizador['vermelho']/count($eixo_x));
		$eixo_x[] = "MÉDIA";
	} elseif($_REQUEST['media']=="2") {
		$dat_1 = round($totalizador['azul']/count($eixo_x));
		$dat_2 = round($totalizador['vermelho']/count($eixo_x));
		unset($data_1,$data_2,$eixo_x);
		$data_1[] = $dat_1;
		$data_2[] = $dat_2;
		$eixo_x[] = "MÉDIA";
	}
	
	if($_REQUEST['totalizador']=="1") {
		$eixo_x[] = "TOTAL";
		$data_1[] = $totalizador['azul'];
		$data_2[] = $totalizador['vermelho'];
	} elseif($_REQUEST['totalizador']=="2") {
		$dat_1 = $totalizador['azul'];
		$dat_2 = $totalizador['vermelho'];
		unset($data_1,$data_2,$eixo_x);
		$eixo_x[] = "TOTAL";
		$data_1[] = $dat_1;		
		$data_2[] = $dat_2;
	}
	
	//if(!$data_1) $data_1[] = 0;
	//if(!$data_2) $data_2[] = 0;
	

	// Example for use of JpGraph,
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');

	// Setup the graph.
	$graph = new Graph(1800,440);
	$graph->img->SetMargin(100,150,90,100);
	$graph->SetScale("textlin");
	$graph->SetMarginColor("white");
	$graph->SetShadow();

	// Set up the title for the graph
	$graph->title->Set("CLASSIFICAÇÃO DENTRO/FORA DO PRAZO POR MÊS (".$dataini." a ".$datafim.") - SERVIÇO DE IMPRESSÃO");
	$graph->title->SetMargin(5);
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,TITULO);
	$graph->title->SetColor("black");

	// Setup font for axis
	$graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOX_MES);
	$graph->yaxis->SetFont(FF_ARIAL,FS_NORMAL, LABEL_EIXOY_MES);

	// Show 0 label on Y-axis (default is not to show)
	$graph->yscale->ticks->SupressZeroLabel(false);

	// Setup X-axis labels
	$graph->xaxis->SetTickSide(SIDE_BOTTOM);
	$graph->yaxis->SetTickSide(SIDE_LEFT);

	$graph->xaxis->SetTickLabels($eixo_x);
	$graph->xaxis->SetLabelAngle(45);
	$graph->legend->SetFont(FF_ARIAL,FS_NORMAL,8); 

	
	// Create the bar plots
	$b1plot = new BarPlot($data_1);
	$b1plot->SetLegend("Dentro do prazo");
	$b1plot->SetFillColor("blue");
	$b1plot->value->Show();
	$b1plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b1plot->value->SetTypeGraph('azul');
	$b1plot->value->SetFormatCallbackParam('barValueFormat2',$arValores);
	
	$b2plot = new BarPlot($data_2);
	$b2plot->SetLegend("Fora do prazo");
	$b2plot->SetFillColor("red");
	$b2plot->value->Show();
	$b2plot->value->SetFont(FF_VERDANA,FS_NORMAL,PLOT_MES);
	$b2plot->value->SetTypeGraph('vermelho');
	$b2plot->value->SetFormatCallbackParam('barValueFormat2',$arValores);


	// Create the grouped bar plot
	$gbplot = new GroupBarPlot(array($b1plot,$b2plot));

	// ...and add it to the graPH
	$graph->Add($gbplot);


	// Finally send the graph to the browser
	$graph->Stroke();

}



function classdentroforaprazo_pizza_impressao() {

	global $db;
	
	$dataini = md5_decrypt($_REQUEST['dataini']);
	$datafim = md5_decrypt($_REQUEST['datafim']);

$sql = "SELECT  DISTINCT
							lpad(cast(d.dmdid as varchar), 
								 case when length(cast(d.dmdid as varchar)) > 5 then 
								 	length(cast(d.dmdid as varchar)) 
							   	 else 
							   	 	5 
							   	 end 
							     , '0') AS nudemanda,
						     od.ordid as ordid,
							 od.orddescricao AS origemdemanda,
							 t.tipnome as tipodemanda,
							 smd.sidabrev || ' - ' || smd.siddescricao as sistema,
							 cel.celnome as celula,
							 
							 ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) as datasituacao,		  
							 '' as observacao, 				
							 (CASE WHEN    d.dmddatafimprevatendimento < now() AND doc.esdid in (91,107,92,108)   THEN
							 		'' || d.dmdtitulo || ''
							 	   ELSE
							 	   	d.dmdtitulo
							 END) as assunto,
							 d.dmddsc as descricao,
							 to_char(d.dmddatainclusao::timestamp,'YYYY-MM-DD HH24:MI:00') AS dataabertura,
							 to_char(d.dmddatainiprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS datainicio,
							 to_char(d.dmddatafimprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS datafim,
							 '' as  prazoatendimento,
							 '' as  tempodecorrido,
							 '' as duracaoatendminutos,
							 '' as  tempopausa,
							 to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
							 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
							(CASE WHEN u.usunome <> '' THEN
							 	 		u.usunome
							 	   ELSE 
							 	   		u3.usunome || 'Solicitado em nome de: ' || d.dmdnomedemandante
							  END) AS solicitante,
							(CASE WHEN u2.usunome <> '' THEN
							 	 		u2.usunome
							 	   ELSE 
							 	   		'Não informado'
							  END) AS tecnico,		 
							 d.dmdnomedemandante as demandantegeral,
							 '(' || u.usufoneddd || ') ' || u.usufonenum AS tel,
							 upper(unasigla)||' - '||unadescricao as setor, 	
							 loc.lcadescricao as edificio,
							 aa.anddescricao AS andar,
							 d.dmdsalaatendimento as sala,
							 (CASE d.dmdclassificacaosistema
							 	WHEN '1' THEN 'Inicial'
							 	WHEN '2' THEN 'Consultiva'
							 	WHEN '3' THEN 'Investigativa'
							 	WHEN '4' THEN 'Manutenção corretiva'
							 	WHEN '5' THEN 'Manutenção evolutiva'
							 	ELSE 'Não Classificada'
							 END) AS classifsistema,
							 '' AS avaliacao,
							 (CASE WHEN ed.esddsc <> '' THEN
							 	 		ed.esddsc
							 	   ELSE 
							 	   		'Em processamento'
							 END) AS situacao,		 
							 (CASE WHEN p.pridsc <> '' THEN
							 	 		p.pridsc
							 	   ELSE 
							 	   		'Não Informado'
							 END) AS prioridade,
							 (CASE EXTRACT(month from d.dmddatainclusao)
								   			WHEN '1' THEN 'Janeiro/' || to_char(d.dmddatainclusao::date,'YYYY') 
											WHEN '2' THEN 'Fevereiro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '3' THEN 'Março/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '4' THEN 'Abril/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '5' THEN 'Maio/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '6' THEN 'Junho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '7' THEN 'Julho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '8' THEN 'Agosto/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '9' THEN 'Setembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '10' THEN 'Outubro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '11' THEN 'Novembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '12' THEN 'Dezembro/' || to_char(d.dmddatainclusao::date,'YYYY')
							 END) as mesano,
							 d.dmdhorarioatendimento,
						    (CASE WHEN pt.tsppontuacao is not null THEN 
							 			pt.tsppontuacao
							 	   ELSE
							 	   		'0'	 
							 END) AS pontuacao,		 
							 '' as tempoclassif,
							 '0' as totalpontuacao,
							(CASE WHEN d.dmdqtde > 0 THEN 
							 			d.dmdqtde
							 	   ELSE
							 	   		'1'	 
							 END) AS qtdservico,
							 '1' AS totaldemandas,
							 
							 to_char(d.dmdtempoadicional, 'HH24:MI') as tempoadicional,
							 d.dmdobstempoadicional as justtempoadicional,
							 '0' as valordemanda,
							 d.usucpfexecutor,
							 EXTRACT(MONTH FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as mes,
							 EXTRACT(YEAR FROM ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) ) as ano,
							 ed.esdid,
							 ed.esddsc,
							 COALESCE(crtvlponto,0) as valorponto,
							 d.dmddataclassificacao as dataclassificacao,
							 d.dmddataclassificacaosi as dataclassificacaosi,
							 pt.tsptempoclassif
							 FROM
							 demandas.demanda d
							 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
							 LEFT JOIN workflow.documento doc ON doc.docid = d.docid and doc.tpdid in (31,35)
							 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
							 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
							 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
							 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
							 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
							 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
							 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
							 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
							 
							 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
							 
							 /*			
							 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
											from 	workflow.historicodocumento a
												inner join demandas.demanda d1 on a.docid = d1.docid
									  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
					         */
							 LEFT JOIN (select crtvlponto, crtdtinicio, crtdtfim, ordid from demandas.contrato where crtstatus='A') as con on od.ordid=con.ordid and ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between con.crtdtinicio and con.crtdtfim
						 	 
							 WHERE d.dmdstatus = 'A'  AND od.ordid  IN  ('12')  
							 --AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
							 --AND  ed.esdid  NOT IN (100,110) --CANCELADA  
							 AND  ed.esdid IN (95,170) --VALIDADA/VALIDADA SEM PAUSA
							 --AND  datasituacao between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59'
							 AND ( SELECT MAX(htddata) FROM  workflow.historicodocumento where docid = d.docid ORDER BY 1 DESC ) between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59' 
							 ORDER BY  ano, mes, tecnico";

	/*
	$sql = "SELECT  DISTINCT
							lpad(cast(d.dmdid as varchar), 
								 case when length(cast(d.dmdid as varchar)) > 5 then 
								 	length(cast(d.dmdid as varchar)) 
							   	 else 
							   	 	5 
							   	 end 
							     , '0') AS nudemanda,
						     od.ordid as ordid,
							 od.orddescricao AS origemdemanda,
							 t.tipnome as tipodemanda,
							 smd.sidabrev || ' - ' || smd.siddescricao as sistema,
							 cel.celnome as celula,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    servico
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada 
								    servico
							       ELSE
								   ''
							  END) as servicoexec,
					
							 (CASE WHEN doc.esdid in (100,110) THEN --cancelada
								    ''
							       WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								    datadoc
							       ELSE
								   to_char(now()::timestamp,'YYYY-MM-DD HH24:MI:00')
							  END) as datadocfinalizada,
					
							 (CASE WHEN doc.esdid in (93,95,109,111,170) THEN --finalizada
								    dataconc
							       ELSE
								   ''
							  END) as dataconclusao,
							  
							 datasit as datasituacao,		  
							 '' as observacao, 				
							 (CASE WHEN    d.dmddatafimprevatendimento < now() AND doc.esdid in (91,107,92,108)   THEN
							 		'' || d.dmdtitulo || ''
							 	   ELSE
							 	   	d.dmdtitulo
							 END) as assunto,
							 d.dmddsc as descricao,
							 to_char(d.dmddatainclusao::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
							 to_char(d.dmddatainiprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS datainicio,
							 to_char(d.dmddatafimprevatendimento::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS datafim,
							 '' as  prazoatendimento,
							 '' as  tempodecorrido,
							 '' as duracaoatendminutos,
							 '' as  tempopausa,
							 to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
							 to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento,
							(CASE WHEN u.usunome <> '' THEN
							 	 		u.usunome
							 	   ELSE 
							 	   		u3.usunome || 'Solicitado em nome de: ' || d.dmdnomedemandante
							  END) AS solicitante,
							(CASE WHEN u2.usunome <> '' THEN
							 	 		u2.usunome
							 	   ELSE 
							 	   		'Não informado'
							  END) AS tecnico,		 
							 d.dmdnomedemandante as demandantegeral,
							 '(' || u.usufoneddd || ') ' || u.usufonenum AS tel,
							 upper(unasigla)||' - '||unadescricao as setor, 	
							 loc.lcadescricao as edificio,
							 aa.anddescricao AS andar,
							 d.dmdsalaatendimento as sala,
							 (CASE d.dmdclassificacaosistema
							 	WHEN '1' THEN 'Inicial'
							 	WHEN '2' THEN 'Consultiva'
							 	WHEN '3' THEN 'Investigativa'
							 	WHEN '4' THEN 'Manutenção corretiva'
							 	WHEN '5' THEN 'Manutenção evolutiva'
							 	ELSE 'Não Classificada'
							 END) AS classifsistema,
							 '' AS avaliacao,
							 (CASE WHEN ed.esddsc <> '' THEN
							 	 		ed.esddsc
							 	   ELSE 
							 	   		'Em processamento'
							 END) AS situacao,		 
							 (CASE WHEN p.pridsc <> '' THEN
							 	 		p.pridsc
							 	   ELSE 
							 	   		'Não Informado'
							 END) AS prioridade,
							 (CASE EXTRACT(month from d.dmddatainclusao)
								   			WHEN '1' THEN 'Janeiro/' || to_char(d.dmddatainclusao::date,'YYYY') 
											WHEN '2' THEN 'Fevereiro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '3' THEN 'Março/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '4' THEN 'Abril/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '5' THEN 'Maio/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '6' THEN 'Junho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '7' THEN 'Julho/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '8' THEN 'Agosto/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '9' THEN 'Setembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '10' THEN 'Outubro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '11' THEN 'Novembro/' || to_char(d.dmddatainclusao::date,'YYYY')
											WHEN '12' THEN 'Dezembro/' || to_char(d.dmddatainclusao::date,'YYYY')
							 END) as mesano,
							 d.dmdhorarioatendimento,
						    (CASE WHEN pt.tsppontuacao is not null THEN 
							 			pt.tsppontuacao
							 	   ELSE
							 	   		'0'	 
							 END) AS pontuacao,		 
							 '' as tempoclassif,
							 '0' as totalpontuacao,
							(CASE WHEN d.dmdqtde > 0 THEN 
							 			d.dmdqtde
							 	   ELSE
							 	   		'1'	 
							 END) AS qtdservico,
							 '1' AS totaldemandas,
							 to_char(pt.tsptempo, 'HH24:MI') as prazocatalogo,
							 to_char(d.dmdtempoadicional, 'HH24:MI') as tempoadicional,
							 d.dmdobstempoadicional as justtempoadicional,
							 '0' as valordemanda,
							 d.usucpfexecutor,
							 EXTRACT(MONTH FROM datasituacao) as mes,
							 EXTRACT(YEAR FROM datasituacao) as ano,
							 ed.esdid,
							 (select crtvlponto from demandas.contrato where crtstatus='A' and (current_timestamp between crtdtinicio and crtdtfim) limit 1) as valorponto
							 FROM
							 demandas.demanda d
							 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
							 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
							 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
							 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
							 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
							 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
							 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
							 LEFT JOIN seguranca.usuario u3 ON u3.usucpf = d.usucpfanalise
							 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
							 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
							 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
							 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
							 LEFT JOIN  demandas.sistemadetalhe AS smd ON smd.sidid = d.sidid
							 LEFT JOIN  demandas.sistemacelula AS smc ON smc.sidid = d.sidid
							 LEFT JOIN  demandas.celula AS cel ON cel.celid = smc.celid
							 --LEFT JOIN  demandas.avaliacaodemanda AS avd ON avd.dmdid = d.dmdid
							 
							 LEFT JOIN demandas.tiposervicoprioridade pt ON pt.tipid = d.tipid and pt.priid = d.priid and pt.tspstatus = 'A'
							 
							 LEFT JOIN ( (select a.docid, max(a.hstid) as hstid, to_char(max(a.htddata)::timestamp,'YYYY-MM-DD HH24:MI:00') as datadoc, to_char(max(htddata)::timestamp,'DD/MM/YYYY HH24:MI') as dataconc, max(htddata) as dataatendfinalizado						
											from 	workflow.historicodocumento a
												inner join workflow.documento c on c.docid = a.docid
										where a.aedid in (146, 191) 
										group by a.docid
										) ) as hst ON hst.docid = d.docid
										
							 LEFT JOIN ( (select a.docid, a.hstid, b.cmddsc as servico
											from 	workflow.historicodocumento a
												inner join workflow.comentariodocumento b on a.hstid = b.hstid and a.docid = b.docid 
												inner join workflow.documento c on c.docid = a.docid
										where a.aedid in (146, 191) 
										group by a.docid, a.hstid, b.cmddsc 
										) ) as hst2 ON hst2.docid = hst.docid and hst2.hstid = hst.hstid
										
							 LEFT JOIN (  (select d1.dmdid, to_char(max(a.htddata)::timestamp,'DD/MM/YYYY HH24:MI') as datasit, max(a.htddata) as datasituacao
											from 	workflow.historicodocumento a
												inner join demandas.demanda d1 on a.docid = d1.docid
									  group by d1.dmdid order by 2 desc) ) as dmd1 ON dmd1.dmdid = d.dmdid
					
						 	 
							 WHERE d.dmdstatus = 'A'  AND od.ordid  IN  ('12')  
							 --AND  d.usucpfexecutor  IN  ('".implode("','",$_SESSION['cpf_tecnicos'])."')  
							 AND  ed.esdid  IN  (95,109,170)  
							 AND  datasituacao between '".formata_data_sql($dataini)." 00:00:00' and '".formata_data_sql($datafim)." 23:59:59' 
							 ORDER BY  ano, mes, tecnico, datadocfinalizada";
	*/
	
	$dados = $db->carregar($sql);
	
	$classdata = new Data;

	if($dados[0]) {
		foreach($dados as $dado) {

			if(!$dados_bruto[$dado['mes']]['mes']) {
				$dados_bruto[$dado['mes']]['mes'] = $db->pegaUm("SELECT UPPER(mesdsc) FROM public.meses WHERE mescod::integer = '".$dado['mes']."'")."/".$dado['ano'];
				$dados_bruto[$dado['mes']]['vermelho'] = 0;
				$dados_bruto[$dado['mes']]['azul'] = 0;
			}
			

			$dtabertura = $dado['dataclassificacao'];

			$hor_tc	= substr($dado['tsptempoclassif'],0,2);
			$min_tc	= substr($dado['tsptempoclassif'],3,2);
			
			$ano_ini	= substr($dtabertura,0,4);
			$mes_ini	= substr($dtabertura,5,2);
			$dia_ini	= substr($dtabertura,8,2);
			$hor_ini	= substr($dtabertura,11,2);
			$min_ini	= substr($dtabertura,14,2);

			$datainif = mktime($hor_ini+$hor_tc,$min_ini+$min_tc,0,$mes_ini,$dia_ini,$ano_ini); // timestamp da data final
			$datainiFinal = strftime("%Y-%m-%d %H:%M:%S", $datainif);

			if($datainiFinal && $dado['dataclassificacaosi']){
				$dtxini = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$datainiFinal)));
				$dtxfim = (float) str_replace(':','',str_replace(' ','',str_replace('-','',$dado['dataclassificacaosi'])));
				
				if($dtxfim > $dtxini){
					$dados_bruto[$dado['mes']]['vermelho']++;
					//$dados_bruto[$dado['mes']]['valorvermelho'] = $dados_bruto[$dado['mes']]['valorvermelho'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
				else{
					$dados_bruto[$dado['mes']]['azul']++;
					//$dados_bruto[$dado['mes']]['valorazul'] = $dados_bruto[$dado['mes']]['valorazul'] + (($dado['pontuacao'] * $dado['qtdservico']) * $dado['valorponto']);
				}
			}
			else{
				$dados_bruto[$dado['mes']]['vermelho']++;
			}			
			
		}
	}
	
	if($dados_bruto) {
		foreach($dados_bruto as $d) {
			$eixo_x[] = $d['mes'];
			$totalizador['dentro'] += $d['azul'];
			$totalizador['fora'] += $d['vermelho'];
		}
	}

	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	require_once ('../../includes/jpgraph/jpgraph_pie3d.php');
	
	$data = array($totalizador['dentro'],
				  $totalizador['fora']);
	
	$graph = new PieGraph(800,440);
	$graph->SetShadow();
	$graph->title->Set("CLASSIFICAÇÃO DENTRO/FORA DO PRAZO - PIZZA (".$dataini." a ".$datafim.") - SERVIÇO DE IMPRESSÃO");
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,9);
	$graph->legend->SetFont(FF_VERDANA,FS_NORMAL,8); 
	$xx = new PiePlot3D($data);
	$cores = array("blue","red");
	$xx->SetSliceColors($cores);
	$xx->value->SetFormat('%01.1f%%');
	$xx->value->HideZero();
	$xx->SetSize(0.5);
	$xx->SetCenter(0.45);
	$legendas = array("Dentro do prazo (".$totalizador['dentro'].")","Fora do prazo (".$totalizador['fora'].")");
	$xx->SetLegends($legendas);
	//$xx->ExplodeAll(10);
	$xx->SetShadow();
	$graph->Add($xx);
	$graph->Stroke();

}



/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$_REQUEST['consulta']();

?>