<?php

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "pde/www/_funcoes_cockpit.php";

session_start();




// CPF do administrador de sistemas
if( !$_SESSION['usucpf'] ){
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}

if( !$db ){
	$db = new cls_banco();
}

if (! $_REQUEST['metid'])
	$_REQUEST['metid'] = 1;


if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
	exit;
}


function mostraTexto(){	
	$meta = $_REQUEST['metid'];
	$submeta = $_REQUEST['submeta'];
	$sql = "SELECT s.subnotatecnica,s.subtitulo,m.mettitulo 
	FROM sase.submeta s 
	LEFT JOIN sase.meta m ON(s.metid = m.metid) 
	WHERE s.metid = $meta AND s.subid = $submeta";
	global $db;
	$texto = $db->carregar($sql);	
	echo "<html>";
	echo "<body>";	
	if($texto){		
	foreach($texto as $t){
		echo "<p style=\"text-align: justify;\"><b>Meta $meta</b>: ".$t['mettitulo']."</p>";
		echo "<p style=\"text-align: justify;\"><b>Submeta: ".$t['subtitulo']."</b></p>";
		echo "<p style=\"text-align: justify;\">";
		if(!$t['subnotatecnica']){
			echo "Esta submeta não possui nota técnica.";
		}else{
			echo "<b>Nota Ténica</b><br>";
			echo $t['subnotatecnica'];
		}
		echo "</p>";
	}
	echo "<a onclick=\"window.print();return false;\"><img src=\"/pde/cockpit/images/pne/impressora.png\" alt=\"Imprimir\" title=\"Imprimir\" border=\"0\"></a>";
	}else{
		echo "<p style=\"text-align: justify;\"><b>Meta $meta</b>: ".$t['mettitulo']."</p>";
		echo "<p style=\"text-align: justify;\"><b>Submeta: ".$t['subtitulo']."</b></p>";
		echo "<p style=\"text-align: justify;\">Esta submeta não possui nota técnica</p>";		
	}
	echo "</body>";
	echo "</html>";
}

function montarGrafico(){

	//require_once ('jpgraph/jpgraph.php');
	//require_once ('jpgraph/jpgraph_line.php');
	include APPRAIZ . "includes/jpgraph/jpgraph.php";
	include APPRAIZ . "includes/jpgraph/jpgraph_line.php";

	// Some  data
	$ydata = explode(",", $_REQUEST['brasilCarregados']);
	$xdata = explode(",", $_REQUEST['anosCarregados']);

	if ($_REQUEST['regioes'])
	{
		$_REQUEST['regioes'] = str_replace("\\", "", $_REQUEST['regioes']);
		$regioes = explode(",", $_REQUEST['regioes']);
	}


	if ($_REQUEST['estados'])
		$estados = explode(",", $_REQUEST['estados']);

	if ($_REQUEST['mesoregioes'])
	{
		$_REQUEST['mesoregioes'] = str_replace("\\", "", $_REQUEST['mesoregioes']);
		$mesoregioes = explode(",", $_REQUEST['mesoregioes']);
	}

	if ($_REQUEST['municipios'])
	{
		$_REQUEST['municipios'] = str_replace("\\", "", str_replace("\'", "", $_REQUEST['municipios']));
		$municipios = explode(",", $_REQUEST['municipios']);
	}

	//$ydata = array(10,20);
	//$xdata = array(2010,2011);

	// Size of the overall graph
	$width=600;
	$height=250;

	// Create the graph and set a scale.
	// These two calls are always required
	$graph = new Graph($width,$height);
	$graph->SetScale('intlin');


	$periodo= $xdata[0].'-'.$xdata[sizeof($xdata)-1];
	// Setup margin and titles
	$graph->SetMargin(40,20,20,40);
	$graph->title->Set('Período');
	$graph->subtitle->Set($periodo);
	$graph->xaxis->title->Set('ANO');
	$graph->yaxis->title->Set('PNE');
	$graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
	$graph->legend->SetLineSpacing(5);
	$graph->legend->Pos(0.01,0.17);
	$graph->legend->SetFrameWeight(1);
	$graph->SetMargin(40, 150 ,0, 0);
	// Create the linear plot

	$lineplot=new LinePlot($ydata,$xdata);
	$lineplot ->SetWeight(3);
	$lineplot->SetColor('#0000ff');
	$lineplot->SetLegend('Brasil');

	// Add the plot to the graph
	$graph->Add($lineplot);

	global $db;

	$arrCores = array('#FF0000','#008000','#663300','#99CCCC','#FFA500','#6800FF','#003300','#CC00CC','#CCCCCC');
	$ctCores = 0;
	$ct = 0;

	if ($municipios)
	{
		foreach ($municipios as $mun)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 1) as pnevalor, mundescricao from sase.pne p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subid =" . $_REQUEST['subid'] .  " and pnetipo = 'M' order by pneano" ;

			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}

			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend(str.replace("'","",$arr[0]['mundescricao']));
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;
		}
	}


	if ($estados)
	{
		foreach ($estados as $uf)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 1) as pnevalor, estuf from sase.pne where estuf= '".$uf."'  AND subid =". $_REQUEST['subid'] . " and pnetipo = 'E' order by pneano";

			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}
			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend($arr[0]['estuf']);
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;

		}
	}

	if ($regioes)
	{
		foreach ($regioes as $reg)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 1) as pnevalor, regdescricao from sase.pne p
					inner join territorios.regiao r on p.regcod = r.regcod
					where p.regcod= '$reg' AND subid =" . $_REQUEST['subid'] . " and pnetipo = 'R' order by pneano" ;


			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}
			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend($arr[0]['regdescricao']);
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;

		}
	}

	if ($mesoregioes)
	{
		foreach ($mesoregioes as $mes)
		{
			$ct = $ct + 1;

			$ydata = array();

			$sql = "select round( pnevalor , 1) as pnevalor, mesdsc from sase.pne p
			inner join territorios.mesoregiao m on p.mescod = m.mescod
			where p.mescod= '$mes' AND subid =" . $_REQUEST['subid'] . " and pnetipo = 'S' order by pneano" ;


			$arr = $db->carregar($sql);

			if ($arr)
			{
				foreach($arr as $p)
				{
					array_push ( $ydata, $p['pnevalor']);
				}
			}
			${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
			${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
			${'lineplot'.$ct} ->SetLegend($arr[0]['mesdsc']);
			${'lineplot'.$ct} ->SetWeight(3);
			$graph->Add( ${'lineplot'.$ct});

			$ctCores = $ctCores + 1;

			if ($ctCores == 9 )
				$ctCores = 0;

		}
	}

	$graph->Stroke();
}

function listarNomeMunicipios($municipios, $subid)
{

	global $db;	

	foreach ($municipios as $mun)
	{
		$sql = "select mundescricao from sase.pne p
		inner join territorios.municipio m on m.muncod = p.muncod
		where p.muncod= ".$mun." AND subid =". $subid . " and pnetipo = 'M' order by pneano" ;

		$arr = $db->carregar($sql);
		if ($arr)
		{
			echo str_replace("\'","",$arr[0]['mundescricao']);				
			echo"; ";
		}		
	}			
}

function listarDadosMunicipios($municipios, $subid)
{

	global $db;
	$valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
	
	foreach ($municipios as $mun)
	{
		$sql = "select '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, mundescricao from sase.pne p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subid =". $subid . " and pnetipo = 'M' order by pneano" ;

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".str_replace("\'","",$arr[0]['mundescricao'])."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];
			}
			echo"</tr>";
		}
		else
		{
			return '';
		}
	}

	$sql =  "select estuf from territorios.municipio where muncod in (".implode(",", $municipios).") group by estuf order by 1";
	$arr = $db->carregarColuna($sql);

	return implode(",",$arr);

}

function listarDadosRegioes($regioes, $subid)
{
	global $db;
	$valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
	foreach ($regioes as $r)
	{
		$sql = "select pneid,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, regdescricao from sase.pne p
				inner join territorios.regiao r on r.regcod = p.regcod
				where r.regcod= '$r'  AND subid = $subid and pnetipo = 'R' order by pneano";
		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".$arr[0]['regdescricao']."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];
			}
			echo"</tr>";
		}
	}
}

function listarNomeRegioes($regioes, $subid)
{
	global $db;
	
	foreach ($regioes as $r)
	{
		$sql = "select regdescricao from sase.pne p
		inner join territorios.regiao r on r.regcod = p.regcod
		where r.regcod= '$r'  AND subid = $subid and pnetipo = 'R' order by pneano";
		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo $arr[0]['regdescricao'];			
			echo "; ";
		}
		
	}
}

function listarNomeMesoregioes($mesoregioes, $subid)
{
	global $db;
	
	foreach ($mesoregioes as $m)
	{
		$sql = "select mesdsc from sase.pne p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subid = $subid and pnetipo = 'S' order by pneano";

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo $arr[0]['mesdsc'];			
			echo"; ";
		}
		
	}
}

function listarDadosMesoregioes($mesoregioes, $subid)
{
	global $db;

	$valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
	foreach ($mesoregioes as $m)
	{
		$sql = "select pneid,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, mesdsc from sase.pne p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subid = $subid and pnetipo = 'S' order by pneano";

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".$arr[0]['mesdsc']."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];

				//array_push ( $arrIDEstadosCarregado, $p['pneid']);
			}
			echo"</tr>";
		}
	}
}

function listarNomeEstados($estados, $subid)
{
	global $db;

	$arrIDEstadosCarregado = array();	
	foreach ($estados as $uf)
	{
		$sql = "select estuf from sase.pne where estuf= '".$uf."'  AND subid =". $subid . " and pnetipo = 'E' order by pneano";

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo $uf;
			echo"; ";
		}
		
	}

	return $arrIDEstadosCarregado;
}

function listarDadosEstados($estados, $subid)
{
	global $db;

	$arrIDEstadosCarregado = array();
	$valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
	foreach ($estados as $uf)
	{
		$sql = "select pneid,   '<td>'||round( pnevalor ,$valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, estuf from sase.pne where estuf= '".$uf."'  AND subid =". $subid . " and pnetipo = 'E' order by pneano";

		$arr = $db->carregar($sql);

		if ($arr)
		{
			echo "<tr><td>".$uf."</td>";
			foreach($arr as $p)
			{
				$p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

				echo $p['pnevalor'];

				array_push ( $arrIDEstadosCarregado, $p['pneid']);
			}
			echo"</tr>";
		}
	}

	return $arrIDEstadosCarregado;
}

function listagemPrincipal()
{
	global $db;
	$contador=0;

	$sql = "select sub.subid, sub.subtitulo,meta.metfontemunicipio,meta.metfonteestado,meta.mettitulo, sub.subnotatecnica from sase.submeta as sub left join sase.meta meta on (sub.metid = meta.metid) where sub.substatus = 'A' and sub.metid = ".$_REQUEST['metid']." order by sub.subordem ASC";
	$arr = $db->carregar($sql);

	$regioes = trim( $_REQUEST['regioes'], ',');
	$regioes = str_replace("\'", '',$regioes);

	$estados = trim( $_REQUEST['estados'], ',');
	$estados = str_replace("\'", '',$estados);

	$mesoregioes = trim( $_REQUEST['mesoregioes'], ',');
	$mesoregioes = str_replace("\'", '',$mesoregioes);

	$municipios = trim( $_REQUEST['municipios'], ',');
	$municipios = str_replace("\\", '',$municipios);
	$municipios = str_replace("\'", "",$municipios);
	
	switch( $_REQUEST['metid'] ){
		case 1:
			$titulo = "Meta 1 – Educação Infantil";
		break;
		case 2:
			$titulo = "Meta 2 – Ensino Fundamental";
		break;
		case 3:
			$titulo = "Meta 3 – Ensino Médio";
		break;
		case 4:
			$titulo = "Meta 4 – Inclusão";
		break;
		case 5:
			$titulo = "Meta 5 – Alfabetização Infantil";
		break;
		case 6:
			$titulo = "Meta 6 – Educação Integral";
		break;
		case 7:
			$titulo = "Meta 7 – Qualidade da Educação Básica/IDEB";
		break;
		case 8:
			$titulo = "Meta 8 – Elevação da escolaridade/Diversidade";
		break;
		case 9:
			$titulo = "Meta 9 – Alfabetização de jovens e adultos";
		break;
		case 10:
			$titulo = "Meta 10 – EJA Integrada";
		break;
		case 11:
			$titulo = "Meta 11 – Educação Profissional";
		break;
		case 12:
			$titulo = "Meta 12 – Educação Superior";
		break;
		case 13:
			$titulo = "Meta 13 – Qualidade da Educação Superior";
		break;
		case 14:
			$titulo = "Meta 14 – Pós-Graduação";
		break;
		case 15:
			$titulo = "Meta 15 – Profissionais de Educação";
		break;
		case 16:
			$titulo = "Meta 16 – Formação";
		break;
		case 17:
			$titulo = "Meta 17 – Valorização dos Profissionais do Magistério";
		break;
		case 18:
			$titulo = "Meta 18 – Planos de Carreira";
		break;
		case 19:
			$titulo = "Meta 19 – Gestão Democrática";
		break;
		case 20:
			$titulo = "Meta 20 – Financiamento da Educação";
		break;
    }

	foreach($arr as $count2 => $s):
		if($contador==0){
			if($regioes){
				$arrRegioes = explode(',',$regioes);
			}
			if($estados){
				$arrEstados = explode(',',$estados);
			}
			
			if($mesoregioes){
				$arrMesoregioes = explode(',',$mesoregioes);
			}
			
			if($municipios){
				$arrMunicipios = explode(',',$municipios);
			}
			
			echo"			
			<div style=\"margin-top:12px;\">
				<div id=\"titulo-meta-some\" style=\"float:left;\" class=\"titulo_box\" >
					$titulo 
				</div>";
			if($_REQUEST['metid'] != 7 || $_REQUEST['metid'] != 18 || $_REQUEST['metid'] != 19 || $_REQUEST['metid'] !=20){	
			echo "
				<div id=\"mostraDados\" style=\"display: none;font-size:16px;font-weight:bold; \">	
					Região: ";if($regioes)listarNomeRegioes($arrRegioes,$s['subid']);
			 echo "	<br>
					UF: ";if($estados)listarNomeEstados($arrEstados, $s['subid']); 
			 echo " <br>
					Mesorregião: ";if($mesoregioes)listarNomeMesoregioes($arrMesoregioes, $s['subid']);
			echo  " <br>
					Município: ";if($municipios)listarNomeMunicipios($arrMunicipios, $s['subid']);
			echo  " <br>						
				</div>";
			}
			echo "
				<div id=\"titulo-meta-aparece\" style=\"display:none;text-align:justify;\">
					<h2 >Meta {$_REQUEST['metid']}: {$s['mettitulo']}</h2>					
				</div>				
				<br style=\"clear:both;\">
				<div id=\"titulo-meta-desaparece\" style=\"text-align:justify;font-weight:normal;color: #222;\">
					<h2 >{$s['mettitulo']}</h2>					
				</div>
				
			</div>";
		}	
	
		echo "
			<table class=\"tabela_box_azul_escuro\" style=\"margin-top: 10px !important;margin-bottom: 10px !important;\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >";					

		$subtitulo = $s['subtitulo'] === 'http://ideb.inep.gov.br/resultado/home.seam?cid=4212113'? 'Acesse as metas do IDEB em: <a target="_blank" href="http://ideb.inep.gov.br/resultado/home.seam?cid=4212113">ideb.inep.gov.br</a>': $s['subtitulo'];

        switch ($_REQUEST['metid']) {
            case(1): case(2): case(3): case(5): case(8): case(9):
            $pneanoBrasil    = $pneanoUf = $pneanoRegiao = 2012;
            $pneanoMunicipio = $pneanoMesorregiao = 2010;
            break;
            case(4):
                $pneanoBrasil = $pneanoUf = $pneanoRegiao = $pneanoMunicipio = $pneanoMesorregiao = $pneanoRegiao = 2010;
                break;
            case(12): case(13): case(14): case(17):
            $pneanoBrasil = $pneanoUf = $pneanoRegiao = $pneanoMunicipio = $pneanoMesorregiao = $pneanoRegiao = 2012;
            break;
            default:
                $pneanoBrasil = $pneanoUf = $pneanoRegiao = $pneanoMunicipio = $pneanoMesorregiao = $pneanoRegiao = 2013;
                break;
        }

        // ATENÇÃO: RETIRAR ESTA LINHA QUANDO OS DADOS DOS ANOS ANTERIORES ESTIVEREM PRONTOS
//        $pneanoBrasil = $pneanoUf = $pneanoRegiao = $pneanoMunicipio = $pneanoMesorregiao = $pneanoRegiao = 2013;

        $qtdLegenda = 1;
        $where = $legenda = '';

        if ($_REQUEST['regioes']) {
            $where .= " OR (pne.regcod IN (" . substr(str_replace('\\', '', $_REQUEST['regioes']), 0, -1) . ") and pnetipo = 'R' and pneano = $pneanoRegiao) ";
            $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #FF70EC; float: left; margin-right: 2px'></span>Região</div>";
            $qtdLegenda++;
        }
        if ($_REQUEST['estados']) {
            $where .= " OR (pne.estuf IN (" . substr(str_replace('\\', '', $_REQUEST['estados']), 0, -1) . ") and pnetipo = 'E' and pneano = $pneanoUf) ";
            $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #2843FF; float: left; margin-right: 2px'></span>Estado</div>";
            $qtdLegenda++;
        }
        if ($_REQUEST['mesoregioes']) {
            $where .= " OR (pne.mescod IN (" . substr(str_replace('\\', '', $_REQUEST['mesoregioes']), 0, -1) . ") and pnetipo = 'S' and pneano = $pneanoMesorregiao) ";
            $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #ff0000; float: left; margin-right: 2px'></span>Mesorregião</div>";
            $qtdLegenda++;
        }
        if ($_REQUEST['municipios']) {
            $where .= " OR (pne.muncod IN (" . str_replace("\'","",$municipios) . ") and pnetipo = 'M' and pneano = $pneanoMunicipio) ";
            $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #FFCB00; float: left; margin-right: 2px'></span>Município</div>";
            $qtdLegenda++;
        }

        $valor = ($_REQUEST['metid']==11 || $_REQUEST['metid']==14) ?'0':'1';

        $sql ="
                        SELECT
                            CASE
                                WHEN pnetipo = 'M' THEN m.estuf || ' - ' || mundescricao
                                WHEN pnetipo = 'R' THEN regdescricao
                                WHEN pnetipo = 'S' THEN mesdsc
                                ELSE coalesce(e.estdescricao, '1Brasil')
                            END as descricao,
                            CASE
                                WHEN pnetipo = 'M' THEN 5
                                WHEN pnetipo = 'R' THEN 2
                                WHEN pnetipo = 'S' THEN 4
                                WHEN pnetipo = 'E' and coalesce(e.estdescricao, '') != '' THEN 3
                                ELSE 1
                            END as ordem,
	                            ROUND(pnevalor, $valor) as pnevalor2,
	                            pneano,
	                            pnetipo,
	                            pne.subid,
	                            sub.metid,
	                            sub.subtitulo, *
							FROM sase.pne  pne
                            	inner join sase.submeta sub on sub.subid = pne.subid
                            	left join territorios.estado e on e.estuf = pne.estuf
                                left join territorios.municipio m on m.muncod = pne.muncod
                                left join territorios.regiao r on r.regcod = pne.regcod
                                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
                            WHERE sub.subid = {$s['subid']}
								AND (
								  (pne.estuf is null and pnetipo = 'E' and pneano = $pneanoBrasil)
								  $where
                                )
								-- and pneano = 2013
							ORDER BY ordem, sub.subordem, pne.subid, pne.pneano, pnetipo, descricao
						";

        $dados = $db->carregar($sql);
        $existeResultado = (bool) $dados;

        $subnotatecnica = '';
        $file = 'pne/notas_tecnicas/' . $s['subnotatecnica'];
        if($s['subnotatecnica'] && file_exists($file)){
            $subnotatecnica = '<a target="_blank" href="' . $file . '"><img height="30px" src="img/nt.png" alt="Nota Técnica" title="Nota Técnica" /></a>';
        }
		echo
			"<tr>
				<td style=\"font-size:16px;padding:5px 0 5px 5px;font-weight:bold;color: #333;\">{$subnotatecnica} {$subtitulo}</td>
			</tr>";

        // Exibe somente se tiver resultado
        if($existeResultado){

        echo "
			<tr>
				<td class=\"tabela_painel\">
					<table class=\"tabela_painel\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >
					<tr>
						<td align=center  style=\"background-color: #fff;\">
						";
echo
<<<HTML
<div style='width: {$qtdLegenda}0%;'>
    <div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'></span>Brasil</div>
    {$legenda}
    <div style='clear: both;'></div>
</div>
<div style='clear: both;'></div>
HTML;


                        $anos = array();
                        $dadosAgrupados = array();
						if($dados)
						{
							foreach ($dados as $dado) 
							{
								$dadosAgrupados[str_replace('1Brasil', 'Brasil', tirar_acentos($dado['descricao']))] = $dado;
								$anos[$dado['pneano']] = $dado['pneano'];
							}

                            $count = 0;
                            foreach($dadosAgrupados as $descricao => $dados){
                                $count++;
                                $metaTotal  = 'P' == $dados['subtipometabrasil'] ? 100 : $dados['subvalormetabrasil'];
                                $metaBrasil = $dados['subvalormetabrasil'];

                                $cor = '#000000';
                                switch ($dados['pnetipo']) {

                                    // Amarelo  (Município)
                                    case ('M'): $cor = '#FFCB00'; break;

                                    // Rosinha  (Região)
                                    case ('R'): $cor = '#FF70EC'; break;

                                    // Vermelho (Mesoregião)
                                    case ('S'): $cor = '#ff0000'; break;

                                    // Verde (Brasil)
                                    // Azul  (Estado)
                                    case ('E'): $cor = 'Brasil' == $descricao ? '#00BF0A' : '#2843FF'; break;
                                }

                                $dadosGrafico = array(
                                    'cor'=>$cor,
                                    'descricao'=>$descricao,
                                    'valor'=>$dados['pnevalor2'],
                                    'metaTotal'=>$metaTotal,
                                    'metaBrasil'=>$metaBrasil,
                                    'tipo'=>$dados['subtipometabrasil']
                                );
							    echo geraGraficoPNE('grafico_'.$count.'_'.$count2, $dadosGrafico);
                            }

//							echo '<div style="margin-bottom: 10px;">' . geraGraficoLinha($aDados, array_values($anos), "grafico_" . $s['subid'], '',200) . '</div>';


						}
					echo "
					</td>
				</tr>
			</table>
		</td>
	</tr>

	<span style='color: #ffcb00'></span>
	";
					 
		if($s['subtitulo'] != "Não foi traçada trajetória para esta meta" && $dados)
		{
			
		?>
			
		<?php
		}

	if($arr && ($s['subtitulo'] !== "Não foi traçada trajetória para esta meta"))
	{
		if($s['metfontemunicipio'] === $s['metfonteestado'])
		{
		echo "
		<tr>
		<td>Fonte: {$s['metfonteestado']}</td>
		</tr>";
		}else{
		echo "
		<tr>
		<td>Fonte: Estado, Região e Brasil - {$s['metfonteestado']}</td>
		</tr>";
		}
	}

	if(($municipios || $mesoregioes)  && ($_REQUEST['metid'] != 14 && $_REQUEST['metid'] != 17 && $_REQUEST['metid'] != 13) && ($s['subtitulo'] != "Não foi traçada trajetória para esta meta"))
	{
		if($s['metfontemunicipio'] !== $s['metfonteestado']){
	echo "
	<tr>
		<td>Fonte: Município e Mesorregião - {$s['metfontemunicipio']}</td>
	</tr>";
		}
	}

    // Somente no Indicador 9B
    if ('36' == $s['subid']) {
        echo "
        <tr>
            <td><strong>Nota: O objetivo desse indicador é reduzir em 50% a taxa de analfabetismo funcional.</strong></td>
        </tr>";
    }
	}
	echo "
</table>";
$contador += 1;
endforeach;
?>

	<head>
	<script type="text/javascript">
		$(function(){
			$('.tabelaPne').hide();
		});
	</script>
	</head>
<?php 		
}

function criarAbasMetasPNE()
{
	global $db;
	$sql = "select metid, mettitulo from sase.meta where metstatus = 'A' order by metid";
	$arrMetas = $db->carregar($sql);

	$abas = array();

	foreach($arrMetas as $meta)
	{
		$arrMeta = array("descricao" =>  'Meta '.$meta['metid'], "meta" => $meta['metid'], "extenso" => $meta['mettitulo']);
		array_push($abas, $arrMeta);
	}

	return $abas;
}

function listarEstados()
{
	$sql = " Select	estuf AS codigo, estdescricao AS descricao From territorios.estado ";
	$regioes =   $_REQUEST['regioes'];

	if ($regioes)
	{
		$regioes =  str_replace("\\", "", trim($regioes, ',')) ;
		$sql .= "where regcod in(".$regioes.")";
	}

	$estados =   $_REQUEST['estados'];
	if ($estados)
	{
		$estados =  str_replace("\\", "", trim($estados, ',')) ;
		$sqlselecionados = "select estuf as codigo, estdescricao as descricao from territorios.estado where estuf in ($estados) ";

		if ($regioes)
		{
			$sqlselecionados .= " and regcod in ($regioes)";
		}
	}
	$grupoExcluido = array(7,15,18,19,20);
	
	if($_REQUEST['metid'] != 7
		&& $_REQUEST['metid'] != 15
		&& $_REQUEST['metid'] != 18
		&& $_REQUEST['metid'] != 19
		&& $_REQUEST['metid'] != 20)
	{
		mostrarComboPopupLocal( 'Estado', 'slEstado',  $sql,$sqlselecionados, 'Selecione os Estados', null,'atualizarRelacionadosRegiao(2)',false);
	}
}

function listarMesoregioes()
{	
	$sql = " Select	mescod AS codigo, mesdsc AS descricao From territorios.mesoregiao ";
	$regioes =   $_REQUEST['regioes'];

	if ($regioes)
	{
		$regioes =  str_replace("\\", "", trim($regioes, ',')) ;

		$sql .= "where estuf in (select estuf from territorios.estado where regcod in ($regioes))";
	}

	$estados =   $_REQUEST['estados'];

	if ($estados)
	{
		$estados =  str_replace("\\", "", trim($estados, ',')) ;
		if ($regioes)
			$sql .= "and estuf in(".$estados.")";
		else
			$sql .= "where estuf in(".$estados.")";
	}
	$grupoExcluido = array(7,15,18,19,20);
	if($_REQUEST['metid'] != 14 
		&& $_REQUEST['metid'] != 17 
		&& $_REQUEST['metid'] != 13 
		&& $_REQUEST['metid'] != 7
		&& $_REQUEST['metid'] != 15
		&& $_REQUEST['metid'] != 18
		&& $_REQUEST['metid'] != 19
		&& $_REQUEST['metid'] != 20){
		mostrarComboPopupLocal( 'Mesorregião', 'slMesoregiao',  $sql,$sqlselecionados, 'Selecione os Estados', null,'atualizarRelacionadosRegiao(3)',false);
	}
}

function listarMunicipios()
{
	$sql = " Select	muncod AS codigo, REPLACE(mundescricao,'\'','') AS descricao From territorios.municipio ";


	$regioes =   $_REQUEST['regioes'];

	if ($regioes)
	{
		$regioes =  str_replace("\\", "", trim($regioes, ',')) ;

		$sql .= "where estuf in (select estuf from territorios.estado where regcod in ($regioes))";
	}


	$estados =   $_REQUEST['estados'];

	if ($estados)
	{
		$estados =  str_replace("\\", "", trim($estados, ',')) ;

		if ($regioes)
			$sql .= "and estuf in(".$estados.")";
		else
			$sql .= "where estuf in(".$estados.")";
	}


	$municipios =   $_REQUEST['municipios'];

	if ($municipios)
	{
		$municipios =  str_replace("\\", "", trim($municipios, ',')) ;
		$sqlselecionados = "select muncod as codigo, REPLACE(mundescricao,'\'','')  as descricao from territorios.municipio where muncod in ($municipios) ";

		if ($estados)
		{
			$sqlselecionados .= " and estuf in ($estados)";
		}
	}
	
	if($_REQUEST['metid'] != 14 
			&& $_REQUEST['metid'] != 17 
			&& $_REQUEST['metid'] != 13 
			&& $_REQUEST['metid'] != 7
			&& $_REQUEST['metid'] != 15
			&& $_REQUEST['metid'] != 18
			&& $_REQUEST['metid'] != 19
			&& $_REQUEST['metid'] != 20)
	{
		mostrarComboPopupLocal( 'Município', 'slMunicipio',  $sql, $sqlselecionados, 'Selecione os Municipios', null, 'atualizarRelacionadosRegiao(3)' ,false);
	}
}

function montarAbasArrayLocal($itensMenu, $url = false, $boOpenWin = false)
{
	$url = $url ? $url : $_SERVER['REQUEST_URI'];

	if (is_array($itensMenu)) 
	{
		$rs = $itensMenu;
	} else 
	{
		global $db;
		$rs = $db->carregar($itensMenu);
	}

	$menu = '<table id="apres" width="100%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
			. '<tr>'
					. '<td width="15%" valign="bottom" class="titulo_box link" onclick="retornaInicio();">Página Inicial</td>'
					. '<td width="85%">'
						. '<table cellpadding="0" cellspacing="0" align="right" border="0">'
							.'<tr>'
								.'<td class="titulo_box" colspan="40" align="left">'
									.'Situação de estados e municípios em relação à meta nacional<br/><br/>'
								.'</td>'
								.'<td colspan="1">'
									.'<a onClick="printFunction();">
										<img  src="/pde/cockpit/images/pne/impressora.png" alt="Imprimir" title="Imprimir" border="0">'
									.'</a>'
								.'</td>'
							.'</tr>'
							.'<tr>';

	$nlinhas = count($rs) - 1;

	for ($j = 0; $j <= $nlinhas; $j++) {
		extract($rs[$j]);

		if ($url != $meta && $j == 0)
			$gifaba = 'aba_nosel_ini.gif';
		elseif ($url == $meta && $j == 0)
			$gifaba = 'aba_esq_sel_ini.gif';
		elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
			$gifaba = 'aba_dir_sel.gif';
		elseif ($url != $meta)
			$gifaba = 'aba_nosel.gif';
		elseif ($url == $meta)
			$gifaba = 'aba_esq_sel.gif';

		if ($url == $meta) {
			$giffundo_aba = 'aba_fundo_sel.gif';
			$cor_fonteaba = '#000055';
		} else {
			$giffundo_aba = 'aba_fundo_nosel.gif';
			$cor_fonteaba = '#4488cc';
		}
		
		$menu .= 
				'<td height="20" valign="top">
					<img id="abameta'.$meta.'" src="/pde/cockpit/images/pne/'.$gifaba.'" width="11" height="20" border="0">
				</td>'
				.'<td height="20" title="'.$descricao.'"  id="abametaf'.$meta.'" align="center" 
					valign="middle" background="/pde/cockpit/images/pne/'.$giffundo_aba.'" 
					style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;cursor:pointer;" 
					onclick="selecionaAba('.$meta.');
					$(\'#metid\').val('.$meta.');
					listarSubmetas(pegaSelecionados(\'slRegiao[]\'),					
					pegaSelecionados(\'slEstado[]\'),pegaSelecionados(\'slMesoregiao[]\'),pegaSelecionados(\'slMunicipio[]\'))">';

		if ($meta != $url) 
		{
			$menu .= $descricao;
		} else 
		{
			$menu .= $descricao . '</td>';
		}
	}

	if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
		$gifaba = 'aba_dir_sel_fim.gif';
	else
		$gifaba = 'aba_nosel_fim.gif';

	$menu .= '<td height="20" valign="top">
				<img src="/pde/cockpit/images/pne/'.$gifaba.'" width="11" height="20" alt="" border="0">
			</td>
		</tr>
	</table>
	</td>
	</tr>
	</table>';

	return $menu;
}

function mostrarComboPopupLocal( $stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao, Array $where=null, $funcaoJS=null, $semTR=false, $intervalo=false , $arrVisivel = null , $arrOrdem = null, $obrig = false, $campoContem = true ){
	global $db, $$stNomeCampo;
	if ( $_REQUEST[$stNomeCampo] && $_REQUEST[$stNomeCampo][0] && !empty( $sql_carregados ) ) {
		if(!is_array($_REQUEST[$stNomeCampo])){
			$_REQUEST[$stNomeCampo][0] = $_REQUEST[$stNomeCampo];
		}
		$sql_carregados = sprintf( $sql_carregados, "'" . implode( "','", $_REQUEST[$stNomeCampo] ) . "'" );
		$$stNomeCampo = $db->carregar( sprintf( $sql_combo, $sql_carregados ) );
		var_dump($stNomeCampo);
	}
	if( !empty($sql_carregados) ){
		$$stNomeCampo = $db->carregar($sql_carregados);
	}

	if(!$semTR)
	{
		echo '<tr id="tr_'.$stNomeCampo.'">';
	}

	echo '<td width="25%" class="fundo_td" valign="top" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
			' . $stDescricao . '
			<input type="hidden" id="' . $stNomeCampo . '_campo_flag" name="' . $stNomeCampo . '_campo_flag" value="' . ( empty( $$stNomeCampo ) ? '0' : '1' ) . '"/>
		</td>
		<td class="fundo_td">';

	echo '<div id="' . $stNomeCampo . '_campo_off" style="color:#a0a0a0;';
	echo !empty( $$stNomeCampo ) ? 'display:none;' : '';
	echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>';
	echo '<div id="' . $stNomeCampo . '_campo_on" ';
	echo empty( $$stNomeCampo ) ? 'style="display:none;"' : '';
	echo '>';
	combo_popupLocal( $stNomeCampo, sprintf( $sql_combo, '' ), $stTextoSelecao, '400x400', 0, array(), '', 'S', false, false, 10, 400, null, null, '', $where, null, true, false, $funcaoJS, $intervalo , $arrVisivel, $arrOrdem);

	if( $obrig )
	{
		echo '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
	}

	echo '</div>
			</td>';

	if(!$semTR)	echo '</tr>';
}

function combo_popupLocal( $nome, $sql, $titulo, $tamanho_janela = '400x400', $maximo_itens = 0,
		$codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
		$campo_flag_contem = false, $size = 10, $width = 200 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null)
{

	global ${$nome};
	unset($dados_sessao);
	// prepara parametros
	$maximo_itens = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	// prepara sessão
	$dados_sessao = array(
			'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
			'titulo' => $titulo,
			'indice' => $indice_visivel,
			'maximo' => $maximo_itens,
			'codigos_fixos' => $codigos_fixos,
			'mensagem_fixo' => $mensagem_fixo,
			'param_conexao' => $param_conexao,
			'where'			=> $where,
			'mostraPesquisa'=> $mostraPesquisa,
			'intervalo'     => $intervalo,
			'arrVisivel'    => $arrVisivel,
			'arrOrdem'     => $arrOrdem
	);

	if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
	{
		$_SESSION['indice_sessao_combo_popup'] = array();
	}
	unset($_SESSION['indice_sessao_combo_popup'][$nome]);
	$_SESSION['indice_sessao_combo_popup'][$nome] = $dados_sessao;

	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca( this );" ';

	/*** Adiciona a função Javascript ***/
	$funcaoJS = (is_null($funcaoJS)) ? 'false' : "'" . $funcaoJS . "'";

	$ondblclick = ' ondblclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );" ';
	$ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados( event, \'' . $nome . '\' );" ';
	$onpop		= ( $onpop == null ) ? $onpop = '' : ' onpop="' . $onpop . '"';
	$onpush		= ( $onpush == null ) ? $onpush = '' : ' onpush="' . $onpush . '"';
	$habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;
	$select =
	'<select ' .
	'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
	'multiple="multiple" size="' . $size . '" ' .
	'name="' . $nome . '[]" id="' . $nome . '" '.
	$onclick . $ondblclick . $ondelete . $onpop . $onpush  .
	'class="CampoEstilo" style="width:250px;" ' .
	$habilitado_select .
	'>';

	if($value && count( $value ) > 0)
	{
		$itens_criados = 0;
		foreach ( $value as $item )
		{
			$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
			$itens_criados++;
			if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
			{
				break;
			}
		}
	} elseif ( ${$nome} && count( ${$nome} ) > 0 ) 
	{
		$itens_criados = 0;
		if( is_array(${$nome}) ){
			foreach ( ${$nome} as $item )
			{
				$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
				$itens_criados++;
				if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
				{
					break;
				}
			}
		}
	}
	else if ( $habilitado == 'S' )
	{
		$select .= '<option value="">Duplo clique para selecionar da lista</option>';
	}
	else
	{
		$select .= '<option value="">Nenhum</option>';
	}
	$select .= '</select>';
	$buscaCodigo = '';

	#Alteração feita por wesley romualdo
	#caso a consulta não seja por descrição e sim por codigo, não permitir digitar string no campo de consulta.
	if($campo_busca_descricao == true )
	{
		$paramentro = "";
		$complOnblur = "";
	}else 
	{
		$paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
		$complOnblur = "this.value=mascaraglobal('[#]',this.value);";
	}

	if ( $campo_busca_codigo == true && $habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $nome . '" onkeypress="combo_popup_keypress_buscar_codigo( event, \'' . $nome . '\', this.value );" '.$paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
		$buscaCodigo .= '<br/>';
	}
	#Fim da alteração realizada por wesley romualdo

	$flagContem = '';
	if ( $campo_flag_contem == true )
	{
		$nomeFlagContemGlobal = $nome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $nome . '_campo_excludente" name="' . $nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $nome . '_campo_excludente">Não contém</label>';
	}
	$cabecalho = '';
	if ( $buscaCodigo != '' || $flagContem != '' )
	{
		$cabecalho .= '<table width="' . $width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
		$cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
		$cabecalho .= '<td align="right">' . $flagContem . '</td>';
		$cabecalho .= '</tr></table>';
	}
	print $cabecalho . $select;
}

?>
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="X-UA-Compatible" content="IE=7" />
	<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
	<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
	<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
	<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.pieRenderer.min.js"></script>
	<script type="text/javascript" src="../includes/jquery-jqplot-1.0.0/plugins/jqplot.donutRenderer.min.js"></script>

<!--    <script src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>-->
<!--	<script src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>-->

    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
    <script language="javascript" src="../includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>


<!--	<script type="text/javascript" src="js/estrategico.js"></script>-->
	<script src="../includes/funcoes.js"></script>
    <script language="javascript" src="/estrutura/js/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" />

	<script type="text/javascript" 			src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

	  <style>
	  	.fundo_td{background-color:#ececec;}
	  	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
	  	.titulo_box{font-weight:bold;font-size:18px;color:#0d7ec2;text-shadow:#b1b1b1 0px 1px 2px;}
	  	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF;  text-align:left;}
	  	.fundo_td{text-align:left;vertical-align:top;}
	  	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy;}
	  	.lista_metas{float:left}
	  	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
	  	.tabela_box{color:#FFFFFF;}
	  	.tabela_box td{background-color:#3CB371;text-shadow:#000000 0px 2px 2px;}
	  	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
	  	.tabela_box_azul_escuro2 td{background-color:#ececec;text-shadow:#b1b1b1 0px 2px 2px;color:#6a6a6a;margin-bottom:5px;}
	  	.tabela_box_azul_escuro td{background-color:#ececec;text-shadow:#b1b1b1 0px 2px 2px;color:#000000;margin-bottom:5px;}
	  	.fundo_td_azul{background-color:#2B86EE}
	  	.fundo_td_azul:hover{background-color:#01A2D8}
	  	.fundo_td_verde{background-color:#0F6D39}
	  	.fundo_td_verde:hover{background-color:#32CD32}
	   	.fundo_td_laranja{background-color:#EE9200}
	  	.fundo_td_laranja:hover{background-color:#EBB513}
	  	.fundo_td_vermelho{background-color:#BB0000}
	  	.fundo_td_vermelho:hover{background-color:#DD0000}
	  	.fundo_td_roxo{background-color:#5333AD}
	  	.fundo_td_roxo:hover{background-color:#6A5ACD}
	  	.fundo_td_azul_escuro{background-color:#152D56}
	  	.fundo_td_azul_escuro:hover{background-color:#1F3864}
	  	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:450px;margin-bottom:2px}
	  	 body{background-color:#fff;margin:0px;padding-top:0px;}
		 #bg_topo{background:url('/pde/cockpit/images/pne/topo.jpg') repeat center; background-position:center; height:110px; width: 100%; margin:1px 0 0 0;}
		 
	  	.fundo_titulo{font: 2em 'Dejavu Serif',Constantia;text-rendering: optimizeLegibility;color:#FFFFFF;}
	  	.numero{text-align:right;font-size:11px;font-family:verdana}
	  	.center{text-align:center}
	  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
	  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
	  	.div_fotos_interno{width:98%;height:85px}
	  	.bold{font-weight:bold}
	  	.esquerda{text-align:left;}
	  	.link{cursor:pointer}
	  	#observacao{font-size: 10px; margin-top:3px; line-height: 100%;  	}
	  	
	  	
	  	/* CSS Para o header da página */
	  	/*#header { width:100%; height:110px; background:url('/pde/cockpit/images/pne/topo.jpg') repeat center; margin:1px 0 0 0;}*/
	  	#header { width:100%; height:110px; background:url('/pde/cockpit/images/pne/topo4.png') repeat center; margin:1px 0 0 0;}
	  	.header-content { width:909px; height:209px; margin: 0 auto;}
	  	#bg-logo1 { width:429px; height:110px; background-color:#65a83c; margin:0 2px 0 0; float:right;*margin:0 2px 0 0;}
	  	#logo1 { width:362px; height:100px; background:url('/pde/cockpit/images/pne/logo.png') no-repeat; margin:5px 45px 0 0; float:right;}
	  	#texto-topo { width:380px; height:30px; margin:85px -390px 0 0; float:right; color:#FFFFFF; font-weight:bold; font-size:1.3em; line-height:1.3em;}
	  	#social-icons { margin: 0; padding: 0; position: absolute; width: 900px; text-align: right;}
	  	#social-icons ul { margin: 5px 0 0 0; padding: 0;}
		#social-icons ul li{ display: inline; margin-right: 2px;}

	  	/* Css Acima do Header -> Barra do Brasil*/
	  	section #wrapper-barra-brasil {position: relative;overflow: hidden;margin: 0 auto !important;width: 100%;max-width: 960px;}
	  	#barra-brasil {height: 32px;background: #f1f1f1;font-weight: bold;font-size: 12px;line-height: 32px;font-family: "Open Sans",Arial,Helvetica,sans-serif;}  	
		#barra-brasil div, #barra-brasil a, #barra-brasil ul, #barra-brasil li {margin: 0;padding: 0;border: 0;font-size: 100%;font-family: inherit;vertical-align: baseline;}
		#barra-brasil .brasil-flag {float: left;padding: 7px 0 6px;width: 115px;height: 19px;border-right: 2px solid #dfdfdf;}
		#barra-brasil .brasil-flag .link-barra {display: block;padding-left: 42px;width: 43px;background: url('/pde/cockpit/images/pne/brasil.png') 8px center no-repeat;text-transform: uppercase;line-height: 19px;}
		#barra-brasil .link-barra {color: #606060;}
		#barra-brasil a {text-decoration: none;}
		#barra-brasil .acesso-info {float: left;padding: 0 13px;}
		#barra-brasil .copa-counter {position: absolute;top: 0;left: 343px;padding: 0;color: #138542;width: 210px;text-align: center;}
		#barra-brasil .list {position: absolute;top: 0;right: 0;}
		#barra-brasil ul {list-style: none;}
		#barra-brasil .list-item {display: inline-block;padding: 0 15px 0 13px;height: 32px;border-right: 2px solid #dfdfdf;}
		#barra-brasil .list .first {border-left: 2px solid #dfdfdf;}
		
		#mask{position:absolute;left:0;top:0;z-index:9000;background-color:#000;display:none;}
		#boxes .window{position:fixed;left:0;top:0;width:440px;height:200px;display:none;z-index:9999;padding:20px;}
		#boxes #dialog{width:375px;height:300px;padding:10px;background-color:#ffffff;}	 
		
		.desaparece{display: none;}
	</style>
	
	<script type="text/javascript">
	
		function printFunction(){
			document.getElementById("apres").style.display="none"; //Menu de metas
			//document.getElementById("ti").style.display="block"; //Texto que deve aparecer no pdf
			document.getElementById("esconder").style.display="none";
			document.getElementById("barra-brasil").style.display="none";
			document.getElementById("h1contribuicao").style.display="inline";
			document.getElementById("mostraDados").style.display="block";
			document.getElementById("obs").style.display="none";
			document.getElementById("titulo-meta-some").style.display="none";
			document.getElementById("titulo-meta-aparece").style.display="inline";
			document.getElementById("titulo-meta-desaparece").style.display="none";		
			
							
			window.print();

			document.getElementById("titulo-meta-desaparece").style.display="inherit";	
			document.getElementById("obs").style.display="inline";
			document.getElementById("titulo-meta-some").style.display="inline";
			document.getElementById("titulo-meta-aparece").style.display="none"; 
			//document.getElementById("ti").style.display="none";
			document.getElementById("mostraDados").style.display="none";
			document.getElementById("apres").style.display="inherit";
			document.getElementById("h1contribuicao").style.display="none";
			document.getElementById("esconder").style.display="inherit";
			document.getElementById("barra-brasil").style.display="inherit";
		
		}   			

		function printAlerta(){							
			window.print();
			
		}
		
		function retornaInicio(){
			window.location='http://pne.mec.gov.br';
		}
		
		function selecionaAba(metaaba)
		{			
			$('[id^="abametaf"]').css("color","#4488cc");
			$('[id^="abametaf"]').css("font-weight","");		
			$('#abametaf'+metaaba).css("font-weight","bold");
			$('#abametaf'+metaaba).css("color","navy");
			 
			if(metaaba == 14 || metaaba == 17 || metaaba == 13 || metaaba == 11 || metaaba ==  12){
				alert("Não foi calculada a situação das mesorregiões e municípios nesta meta nacional.");
				$('#pesquisa').css("display","inherit");
				$('#tabelaMesoregioes').css("display","none");
				$('#tabelaMunicipios').css("display","none");
				$('#tabelaEstados').css("display","table");
				$('#tabelaRegioes').css("display","table");
			}else{
				if(metaaba == 7 || metaaba == 15 || metaaba == 18 || metaaba == 19 || metaaba == 20)
				{
					$('#tabelaMesoregioes').css("display","none");
					$('#tabelaMunicipios').css("display","none");
					$('#tabelaEstados').css("display","none");
					$('#tabelaRegioes').css("display","none");
					$('#pesquisa').css("display","none");				
				}else{
					$('#pesquisa').css("display","inherit");
					$('#tabelaMesoregioes').css("display","table");
					$('#tabelaMunicipios').css("display","table");
					$('#tabelaEstados').css("display","table");
					$('#tabelaRegioes').css("display","table");
				}
				//$('#tabelaMesoregioes').css("display","table");
				//$('#tabelaMunicipios').css("display","table");
			}

			
		}
		
		
		function onOffCampo( campo )
		{
			var div_on = document.getElementById( campo + '_campo_on' );
			var div_off = document.getElementById( campo + '_campo_off' );
			var input = document.getElementById( campo + '_campo_flag' );
			if ( div_on.style.display == 'none' )
			{
				div_on.style.display = 'block';
				div_off.style.display = 'none';
				input.value = '1';
			}
			else
			{
				div_on.style.display = 'none';
				div_off.style.display = 'block';
				input.value = '0';
			}
		}
		
		function pegaSelecionados(elemento)
		{
			var result = '';
			var elemento = document.getElementsByName(elemento)[0];
		
			for (var i=0; i<elemento.options.length; i++){
				if (elemento.options[i].value != '')
				{
					result += "'"+elemento.options[i].value + "',";
				}
			}
		
			return result;
		}
		
		
		function selecionaSubmeta(metid)
		{
			this.formulario.metid.value = metid;
			this.formulario.submit();
		}
		
		function atualizarRelacionadosRegiao(requisicao) {
			//alert(requisicao);
		
			//1-estado(chamado pela lista de regiao),2-mesoregiao(chamado pela lista de estado), 3-municipio (chamado pela lista de mesoregiao)
			if (requisicao == 1)
			{
				requisicaoAjax = 'listarEstados';
			}
			else if (requisicao == 2)
			{
				requisicaoAjax = 'listarMesoregioes';
			}
			else if (requisicao == 3)
			{
				requisicaoAjax = 'listarMunicipios';
			}
		
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax="+requisicaoAjax+"&regioes="+pegaSelecionados('slRegiao[]')+"&estados="+pegaSelecionados('slEstado[]')+"&mesoregioes="+pegaSelecionados('slMesoregiao[]')+"&municipios="+pegaSelecionados('slMunicipio[]'),
				success: function(retorno)
				{
					if (requisicao == 1)
					{
						$('#tabelaEstados').html(retorno);
						atualizarRelacionadosRegiao(2);
					}
					else if (requisicao == 2)
					{
						$('#tabelaMesoregioes').html(retorno);
						atualizarRelacionadosRegiao(3);
					}
					else if (requisicao == 3)
					{
						$('#tabelaMunicipios').html(retorno);
					}
		
					if (requisicao == 3 || requisicao == 4)
						listarSubmetas(pegaSelecionados('slRegiao[]'),pegaSelecionados('slEstado[]'),pegaSelecionados('slMesoregiao[]'),pegaSelecionados('slMunicipio[]'));		
				}
			});
		}
		
		function listarSubmetas(regioes, estados, mesoregioes, municipios)
		{
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax=listagemPrincipal&metid="+$('#metid').val()+"&regioes="+regioes+"&estados="+estados+"&mesoregioes="+mesoregioes+"&municipios="+municipios,
				success: function(retorno){
						$('#divListagem').html(retorno);
				}
			});
		}

		function abreNota(submeta){			
			jQuery.ajax({
	            type: "POST",	            
	            url: window.location.href,
	            data: "&requisicaoAjax=mostraTexto&submeta="+submeta,
	            async: false,
	            success: function(msg){		            

	            	var janela = window.open('','Popup','width=800,height=500');
	    			janela.document.write(msg);	    			
					
	            }
	        });
		}
		
		$(document).ready(function() {
			listarSubmetas('','','','');
		   	selecionaAba(1);
		   	
		   	$('.linkExibirTabela').live('click', function(){
				if( $('#tabelaPne_' + $(this).attr('contador')).is(':hidden')) {
					$('#tabelaPne_' + $(this).attr('contador')).show();
					$(this).html('Ocultar tabela');
				} else {
					$('#tabelaPne_' + $(this).attr('contador')).hide();
					$(this).html('Exibir tabela');
				}
		   	});	   
		   						   
		});
	</script>
</head>
<body>

	<section id="ti" style="display: none;margin-top: 30px; width: 400px;">
		<p style="font-size: 22px;font-weight: bold; margin:0 0 5px 0;">Ministério da Educação</p>														
		<p style="font-size: 16px;">			
			Secretaria de Articulação com os Sistemas de Ensino <br/>
			Diretoria de Cooperação e Planos de Educação <br/>
			Projeção da Contribuição para a Meta Nacional
		</p>								
	</section>
	<section id="barra-brasil">
		<section id="wrapper-barra-brasil">
			<section class="brasil-flag">
				<a href="http://brasil.gov.br" class="link-barra">
					Brasil
				</a>
			</section>
			<span class="acesso-info">
				<a href="http://brasil.gov.br/barra#acesso-informacao" class="link-barra">
				Acesso à informação
				</a>
			</span>			
			<ul class="list">
				<li class="list-item first">
					<a href="http://brasil.gov.br/barra#participe" class="link-barra">
					Participe
					</a>
				</li>
				<li class="list-item">
					<a href="http://www.servicos.gov.br/" class="link-barra">
					Serviços
					</a>
				</li>
				<li class="list-item">
					<a href="http://www.planalto.gov.br/legislacao" class="link-barra">
					Legislação
					</a>
				</li>
				<li class="list-item last last-item">
					<a href="http://brasil.gov.br/barra#orgaos-atuacao-canais" class="link-barra">
					Canais
					</a>
				</li>
			</ul>
		</section>
	</section>
	<section id="header">
		<article class="header-content">
			<section id="bg-logo">
		    	<a href="http://webdes.mec.gov.br/acoespdemunicipio/2013/site/?pagina=inicial" id="logo1"></a>
		    	<section id="texto-topo">Construindo as Metas</section>
			</section>
		    <section id="social-icons">
				<ul class="pull-right" style="margin-right:1px">
		        	<li class="portalredes-item">
		            	<a title="Twitter" href="http://twitter.com/mec_comunicacao" target="blank"><img src="/pde/cockpit/images/pne/twitter.png" border="0"></a>
					</li>
		            <li class="portalredes-item">
		            	<a title="YouTube" href="http://youtube.com/ministeriodaeducacao" target="blank"><img src="/pde/cockpit/images/pne/youtube.png" border="0"></a>
		            </li>
		            <li class="portalredes-item">
		                <a title="Facebook" href="http://www.facebook.com/pages/Minist%C3%A9rio-da-Educa%C3%A7%C3%A3o/188209857893503" target="blank"><img src="/pde/cockpit/images/pne/facebook.png" border="0"></a>
		            </li>
				</ul>
			</section>
		</article>
	</section>

	<div id="dialog-detalhe"></div>
	
	<div id="obs" style="margin-top:6px;text-align:center;font-size:11px;"><p>Recomendamos a utilização dos navegadores Google Chrome ou Mozilla Firefox.</p></div>
	<div style="width: 70%;margin:0 auto; padding: 10px 0 0 0;">
		<h2 id="h1contribuicao" style="display:none; margin:15px auto 0 auto; text-align: center;">Situação de estados e municípios em relação à meta nacional</h2>
	</div>
	<div class="container">
	<form name='formulario' action='graficopne_new.php' method='post' style="width:909px;display:table;align:center;border:0; margin: 0 auto;"  >
		<input type='hidden' name='metid' value = ''  id = "metid" >
		<table style="width: 100%!important;">	
			<tr>
				<td colspan="2">
					<?php
					 echo montarAbasArrayLocal( criarAbasMetasPNE() , "" );
					?>
				</td>
			</tr>
			<tr>
				<td  id="esconder" class="" width="20%" valign="top">
					<div style="margin-top:20px;" id="pesquisa">
		                <div style="float:left;" class="titulo_box" >
							Pesquisa<br/><br/>
		                </div>
		            </div>
					<table  cellpadding="5" cellspacing="1" width="100%" id="tabelaRegioes">
						<?php
						#Região
						$sql = " Select	regcod AS codigo, regdescricao AS descricao From territorios.regiao order by regdescricao";
		
						mostrarComboPopupLocal( 'Região', 'slRegiao',  $sql, "", 'Selecione as Regiões', null,'atualizarRelacionadosRegiao(1)',false);
						?>
					</table>
					<table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaEstados" class="filtro_combo">
						<?php						
						listarEstados();
						?>
					</table>
					<table  cellpadding="5" cellspacing="1" width="100%" id = "tabelaMesoregioes" class="filtro_combo">	
						<?php 
						listarMesoregioes();
						?>
					</table>	
					<table  cellpadding="5" cellspacing="1" width="100%" id ="tabelaMunicipios" class="filtro_combo">														
						<?php 
						listarMunicipios();
						?>												
					</table>
				</td>
		        <td class="" width="80%" id="divListagem" valign="top">           	
				</td>
			</tr>	
		</table>
	</form>
	</div>
</body>
</html>