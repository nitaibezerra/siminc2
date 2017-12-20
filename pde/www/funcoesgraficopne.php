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
if (! $_REQUEST['metid']){
    $_REQUEST['metid'] = 1;
}
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
    global $db;
    $meta = $_REQUEST['metid'];
    $submeta = $_REQUEST['submeta'];
    $sql = "SELECT s.subnotatecnica,s.subtitulo,m.mettitulo
	FROM sase.submeta s 
	LEFT JOIN sase.meta m ON(s.metid = m.metid) 
	WHERE s.metid = $meta AND s.subid = $submeta";
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
    global $db;
    //require_once ('jpgraph/jpgraph.php');
    //require_once ('jpgraph/jpgraph_line.php');
    include APPRAIZ . "includes/jpgraph/jpgraph.php";
    include APPRAIZ . "includes/jpgraph/jpgraph_line.php";

    // Some  data
    $ydata = explode(",", $_REQUEST['brasilCarregados']);
    $xdata = explode(",", $_REQUEST['anosCarregados']);
    if ($_REQUEST['regioes']){
        $_REQUEST['regioes'] = str_replace("\\", "", $_REQUEST['regioes']);
        $regioes = explode(",", $_REQUEST['regioes']);
    }
    if ($_REQUEST['estados']){
        $estados = explode(",", $_REQUEST['estados']);
    }
    if ($_REQUEST['mesoregioes']){
        $_REQUEST['mesoregioes'] = str_replace("\\", "", $_REQUEST['mesoregioes']);
        $mesoregioes = explode(",", $_REQUEST['mesoregioes']);
    }
    if ($_REQUEST['municipios']){
        $_REQUEST['municipios'] = str_replace("\\", "", str_replace("\'", "", $_REQUEST['municipios']));
        $municipios = explode(",", $_REQUEST['municipios']);
    }

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
    $arrCores = array('#FF0000','#008000','#663300','#99CCCC','#FFA500','#6800FF','#003300','#CC00CC','#CCCCCC');
    $ctCores = 0;
    $ct = 0;
    if ($municipios){
        foreach ($municipios as $mun){
            $ct = $ct + 1;
            $ydata = array();
            $sql = "select round( pnevalor , 1) as pnevalor, mundescricao from sase.pne p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subid =" . $_REQUEST['subid'] .  " and pnetipo = 'M' order by pneano" ;
            $arr = $db->carregar($sql);
            if ($arr){
                foreach($arr as $p){
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
    if ($estados){
        foreach ($estados as $uf){
            $ct = $ct + 1;
            $ydata = array();
            $sql = "select round( pnevalor , 1) as pnevalor, estuf from sase.pne where estuf= '".$uf."'  AND subid =". $_REQUEST['subid'] . " and pnetipo = 'E' order by pneano";
            $arr = $db->carregar($sql);
            if ($arr){
                foreach($arr as $p){
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
    if ($regioes){
        foreach ($regioes as $reg){
            $ct = $ct + 1;
            $ydata = array();
            $sql = "select round( pnevalor , 1) as pnevalor, regdescricao from sase.pne p
					inner join territorios.regiao r on p.regcod = r.regcod
					where p.regcod= '$reg' AND subid =" . $_REQUEST['subid'] . " and pnetipo = 'R' order by pneano" ;
            $arr = $db->carregar($sql);
            if ($arr){
                foreach($arr as $p){
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
    if ($mesoregioes){
        foreach ($mesoregioes as $mes){
            $ct = $ct + 1;
            $ydata = array();
            $sql = "select round( pnevalor , 1) as pnevalor, mesdsc from sase.pne p
			inner join territorios.mesoregiao m on p.mescod = m.mescod
			where p.mescod= '$mes' AND subid =" . $_REQUEST['subid'] . " and pnetipo = 'S' order by pneano" ;
            $arr = $db->carregar($sql);
            if ($arr){
                foreach($arr as $p){
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

function listarNomeMunicipios($municipios, $subid){
    global $db;
    foreach ($municipios as $mun){
        $sql = "select mundescricao from sase.pne p
		inner join territorios.municipio m on m.muncod = p.muncod
		where p.muncod= ".$mun." AND subid =". $subid . " and pnetipo = 'M' order by pneano" ;
        $arr = $db->carregar($sql);
        if ($arr){
            echo str_replace("\'","",$arr[0]['mundescricao']);
            echo"; ";
        }
    }
}

function listarDadosMunicipios($municipios, $subid){
    global $db;
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($municipios as $mun){
        $sql = "select '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, mundescricao from sase.pne p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subid =". $subid . " and pnetipo = 'M' order by pneano" ;
        $arr = $db->carregar($sql);
        if ($arr){
            echo "<tr><td>".str_replace("\'","",$arr[0]['mundescricao'])."</td>";
            foreach($arr as $p){
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);
                echo $p['pnevalor'];
            }
            echo"</tr>";
        }else{
            return '';
        }
    }
    $sql =  "select estuf from territorios.municipio where muncod in (".implode(",", $municipios).") group by estuf order by 1";
    $arr = $db->carregarColuna($sql);
    return implode(",",$arr);
}

function listarDadosRegioes($regioes, $subid){
    global $db;
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($regioes as $r){
        $sql = "select pneid,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, regdescricao from sase.pne p
				inner join territorios.regiao r on r.regcod = p.regcod
				where r.regcod= '$r'  AND subid = $subid and pnetipo = 'R' order by pneano";
        $arr = $db->carregar($sql);
        if ($arr){
            echo "<tr><td>".$arr[0]['regdescricao']."</td>";
            foreach($arr as $p){
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);
                echo $p['pnevalor'];
            }
            echo"</tr>";
        }
    }
}

function listarNomeRegioes($regioes, $subid){
    global $db;
    foreach ($regioes as $r){
        $sql = "select regdescricao from sase.pne p
		inner join territorios.regiao r on r.regcod = p.regcod
		where r.regcod= '$r'  AND subid = $subid and pnetipo = 'R' order by pneano";
        $arr = $db->carregar($sql);
        if ($arr){
            echo $arr[0]['regdescricao'];
            echo "; ";
        }
    }
}

function listarNomeMesoregioes($mesoregioes, $subid){
    global $db;
    foreach ($mesoregioes as $m){
        $sql = "select mesdsc from sase.pne p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subid = $subid and pnetipo = 'S' order by pneano";
        $arr = $db->carregar($sql);
        if ($arr){
            echo $arr[0]['mesdsc'];
            echo"; ";
        }
    }
}

function listarDadosMesoregioes($mesoregioes, $subid){
    global $db;
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($mesoregioes as $m){
        $sql = "select pneid,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, mesdsc from sase.pne p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subid = $subid and pnetipo = 'S' order by pneano";
        $arr = $db->carregar($sql);
        if ($arr){
            echo "<tr><td>".$arr[0]['mesdsc']."</td>";
            foreach($arr as $p){
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);
                echo $p['pnevalor'];
            }
            echo"</tr>";
        }
    }
}

function listarNomeEstados($estados, $subid){
    global $db;
    $arrIDEstadosCarregado = array();
    foreach ($estados as $uf){
        $sql = "select estuf from sase.pne where estuf= '".$uf."'  AND subid =". $subid . " and pnetipo = 'E' order by pneano";
        $arr = $db->carregar($sql);
        if ($arr){
            echo $uf;
            echo"; ";
        }
    }
    return $arrIDEstadosCarregado;
}

function listarDadosEstados($estados, $subid){
    global $db;
    $arrIDEstadosCarregado = array();
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($estados as $uf){
        $sql = "select pneid,   '<td>'||round( pnevalor ,$valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, estuf from sase.pne where estuf= '".$uf."'  AND subid =". $subid . " and pnetipo = 'E' order by pneano";
        $arr = $db->carregar($sql);
        if ($arr){
            echo "<tr><td>".$uf."</td>";
            foreach($arr as $p){
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);
                echo $p['pnevalor'];
                array_push ( $arrIDEstadosCarregado, $p['pneid']);
            }
            echo"</tr>";
        }
    }
    return $arrIDEstadosCarregado;
}

function listagemPrincipal(){
    global $db;
    $contador=0;
    $arr = carregarSubMetas($_REQUEST['metid']);
    $regioes = trim( $_REQUEST['regioes'], ',');
    $regioes = str_replace(array("\'", "'"), '',$regioes);
    $estados = trim( $_REQUEST['estados'], ',');
    $estados = str_replace(array("\'", "'"), '',$estados);
    $mesoregioes = trim( $_REQUEST['mesoregioes'], ',');
    $mesoregioes = str_replace(array("\'", "'"), '',$mesoregioes);
    $municipios = trim( $_REQUEST['municipios'], ',');
    $municipios = str_replace("\\", '',$municipios);
    $municipios = str_replace("\'", "",$municipios);
    $titulo = selecionaTitulo($_REQUEST['metid']);

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
            $metnotatecnica = '';
            $file = 'pne/notas_tecnicas/' . "Nota_Tecnica_Meta_{$_REQUEST['metid']}.pdf";
            if(file_exists($file)){
                $metnotatecnica = '<a target="_blank" href="' . $file . '"><img height="30px" src="img/nt.png" alt="Nota Técnica" title="Nota Técnica" /></a>';
            }
            echo"<div style=\"margin-top:12px;\">
                    <div id=\"titulo-meta-some\" style=\"float:left;\" class=\"titulo_box\" >
                        {$metnotatecnica} {$titulo}
                    </div>";
            if($_REQUEST['metid'] != 7 || $_REQUEST['metid'] != 18 || $_REQUEST['metid'] != 19 || $_REQUEST['metid'] !=20){
                echo "      <div id=\"mostraDados\" style=\"display: none;font-size:16px;font-weight:bold; \">	
                                Região: ";if($regioes)listarNomeRegioes($arrRegioes,$s['subid']);
                echo "          <br>
				UF: ";if($estados)listarNomeEstados($arrEstados, $s['subid']);
                echo "          <br>
				Mesorregião: ";if($mesoregioes)listarNomeMesoregioes($arrMesoregioes, $s['subid']);
                echo  "         <br>
				Município: ";if($municipios)listarNomeMunicipios($arrMunicipios, $s['subid']);
                echo  "         <br>
                            </div>";
            }
            echo "          <div id=\"titulo-meta-aparece\" style=\"display:none;text-align:justify;\">
                                <h2 >Meta {$_REQUEST['metid']}: {$s['mettitulo']}</h2>					
                            </div>				
                            <br style=\"clear:both;\">
                            <div id=\"titulo-meta-desaparece\" style=\"text-align:justify;font-weight:normal;color: #222;\">
                                <h2 >{$s['mettitulo']}</h2>					
                            </div>
			</div>";
        }
        if($s['subid'] == 15){
            include_once('meta5html.inc');
            break;
        }
        $subtitulo = $s['subtitulo'] === 'http://ideb.inep.gov.br/resultado/home.seam?cid=4212113'? 'Acesse as metas do IDEB em: <a target="_blank" href="http://ideb.inep.gov.br/resultado/home.seam?cid=4212113">ideb.inep.gov.br</a>': $s['subtitulo'];
        $qtdLegenda = 1;
        $where = $legenda = '';
        $aSemLegendaOutros = array(11, 12, 13, 14, 17);
        if ($_REQUEST['regioes']) {
            $pneanoRegiao = selecionaAno($s['subid'], 'R');
            $where .= " OR (pne.regcod IN (" . substr(str_replace('\\', '', $_REQUEST['regioes']), 0, -1) . ") and pnetipo = 'R' and pneano = $pneanoRegiao) ";
            $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #FF70EC; float: left; margin-right: 2px'></span>Região</div>";
            $qtdLegenda++;
        }
        if ($_REQUEST['estados']) {
            $pneanoUf = selecionaAno($s['subid'], 'E');
            $where .= " OR (pne.estuf IN (" . substr(str_replace('\\', '', $_REQUEST['estados']), 0, -1) . ") and pnetipo = 'E' and pneano = $pneanoUf) ";
            $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #2843FF; float: left; margin-right: 2px'></span>Estado</div>";
            $qtdLegenda++;
        }
        if ($_REQUEST['mesoregioes']) {
            $pneanoMesorregiao = selecionaAno($s['subid'], 'S');
            $where .= " OR (pne.mescod IN (" . substr(str_replace('\\', '', $_REQUEST['mesoregioes']), 0, -1) . ") and pnetipo = 'S' and pneano = $pneanoMesorregiao) ";
            if (!in_array($_REQUEST['metid'], $aSemLegendaOutros)) {
                $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #ff0000; float: left; margin-right: 2px'></span>Mesorregião</div>";
                $qtdLegenda++;
            }
        }
        if ($_REQUEST['municipios']) {
            $pneanoMunicipio = selecionaAno($s['subid'], 'M');
            $where .= " OR (pne.muncod IN (" . str_replace("\'","",$municipios) . ") and pnetipo = 'M' and pneano = $pneanoMunicipio) ";
            if (!in_array($_REQUEST['metid'], $aSemLegendaOutros)) {
                $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #FFCB00; float: left; margin-right: 2px'></span>Município</div>";
                $qtdLegenda++;
            }
        }
        $valor = ($_REQUEST['metid']==11 || $_REQUEST['metid']==14) ?'0':'1';
        $dados = retornaDadosPne($valor, $s['subid'], selecionaAno($s['subid'], 'B'), $where);
        $existeResultado = (bool) $dados;
        echo "<table class=\"tabela_box_azul_escuro\" style=\"margin-top: 10px !important;margin-bottom: 10px !important;\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >";        
        echo "  <tr>";
        echo "      <td style=\"font-size:16px;padding:5px 0 5px 5px;font-weight:bold;color: #333;\">";
        echo "          {$subtitulo}";
        echo "      </td>";
        echo "  </tr>";
        // Exibe somente se tiver resultado
        if($existeResultado){
            echo "  <tr>";
            echo "      <td class=\"tabela_painel\">";
            echo "          <table class=\"tabela_painel\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >";
            echo "              <tr>";
            echo "                  <td align=center  style=\"background-color: #fff;\">";            
            echo "                      <div style='width: {$qtdLegenda}0%;'>";
            echo "                          <div style='float: left; margin-right: 20px;'>";
            echo "                              <span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'></span>Brasil";
            echo "                          </div>";
            echo "                          {$legenda}";
            echo "                          <div style='clear: both;'></div>";
            echo "                      </div>";
            echo "                      <div style='clear: both;'></div>";
            $anos = array();
            $dadosAgrupados = array();
            if($dados){
                foreach ($dados as $dado){
                    $dadosAgrupados[str_replace('1Brasil', 'Brasil', tirar_acentos($dado['descricao'] . '::' . $dado['pnetipo']))] = $dado;
                    $anos[$dado['pneano']] = $dado['pneano'];
                }
                $count = 0;
                foreach($dadosAgrupados as $descricao => $dados){
                    $count++;
                    $descricao = substr($descricao, 0, strrpos($descricao, '::'));
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
                        'meta'=>$_REQUEST['metid'],
                        'descricao'=>str_replace("'", '', $descricao),
                        'valor'=>$dados['pnevalor2'],
                        'metaTotal'=>$metaTotal,
                        'metaBrasil'=>$metaBrasil,
                        'tipo'=>$dados['subtipometabrasil']
                    );
                    echo geraGraficoPNE('grafico_'.$count.'_'.$count2, $dadosGrafico);
                }
            }
            echo "                  </td>";
            echo "              </tr>";
            echo "          </table>";
            echo "      </td>";
            echo "  </tr>";
            echo "  <span style='color: #ffcb00'></span>";

            if($arr && ($s['subtitulo'] !== "Não foi traçada trajetória para esta meta")){
                if($s['subfontemunicipio'] === $s['subfonteestado']){
                    echo "  <tr>";
                    echo "      <td>Fonte: {$s['subfonteestado']}</td>";
                    echo "  </tr>";
                }else{
                    echo "  <tr>";
                    echo "      <td>Fonte: Estado, Região e Brasil - {$s['subfonteestado']}</td>";
                    echo "  </tr>";
                }
            }
            if(($municipios || $mesoregioes)  && ($_REQUEST['metid'] != 14 && $_REQUEST['metid'] != 17 && $_REQUEST['metid'] != 13) && ($s['subtitulo'] != "Não foi traçada trajetória para esta meta")){
                if($s['subfontemunicipio'] !== $s['subfonteestado']){
                    echo "  <tr>";
                    echo "      <td>Fonte: Município e Mesorregião - {$s['subfontemunicipio']}</td>";
                    echo "  </tr>";
                }
            }
            // Somente no Indicador 9B
            if ('36' == $s['subid']) {
                echo "  <tr>";
                echo "      <td><strong>Nota: O objetivo desse indicador é reduzir em 50% a taxa de analfabetismo funcional.</strong></td>";
                echo "  </tr>";
            }
        }
        echo "</table>";
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

/**
 * 
 * @global type $db
 * @param type $metid
 * @return type
 */
function carregarSubMetas($metid){
    global $db;
    $sql = "select sub.subid, sub.subtitulo,meta.metfontemunicipio,meta.metfonteestado, sub.subfontemunicipio, sub.subfonteestado,meta.mettitulo, sub.subnotatecnica
            from sase.submeta as sub
                left join sase.meta meta on (sub.metid = meta.metid)
            where sub.substatus = 'A'
            and sub.metid = ".$metid."
            order by sub.subordem ASC";
    return $db->carregar($sql);
}

/**
 * 
 * @param type $metid
 */
function selecionaTitulo($metid){
    $titulo='';
    switch( $metid ){
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
    return $titulo;
}

/**
 * 
 * @global type $db
 * @param type $subid
 * @param type $paetipo
 * @return string
 */
function selecionaAno($subid, $paetipo){
    global $db;
    $sql = "select paeano
              from sase.pneanoexibicao
             where subid in ({$subid})
               and paetipo = '{$paetipo}'
               and paestatus = 'A'";                
    $rs = $db->pegaUm($sql);   
    
    if ($rs==''){
        $rs='2015';
    }
   
    return $rs;
}

/**
 * 
 * @global type $db
 * @param type $valor
 * @param type $subid
 * @param type $pneanoBrasil
 * @param type $where
 * @return type
 */
function retornaDadosPne($valor, $subid, $pneanoBrasil, $where){
    global $db;
    $sql ="SELECT CASE WHEN pnetipo = 'M' THEN m.estuf || ' - ' || mundescricao
                       WHEN pnetipo = 'R' THEN regdescricao
                       WHEN pnetipo = 'S' THEN mesdsc
                       ELSE coalesce(e.estdescricao, '1Brasil')
                   END as descricao,
                  CASE WHEN pnetipo = 'M' THEN 5
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
            WHERE sub.subid = {$subid}
              AND ((coalesce(pne.estuf, '') = '' and pnetipo = 'E' and pneano = $pneanoBrasil)$where)								
            ORDER BY ordem, sub.subordem, pne.subid, pne.pneano, pnetipo, descricao";
    return $db->carregar($sql);
}
?>