<?php
include_once APPRAIZ . "includes/funcoes.inc";

if ( $_POST['requisicao'] == 'valida_indicadores' ){
    include(APPRAIZ."par/modulos/relatorio/listaIndicadoresPendentes.inc");
    exit;
}

$pneanoBrasil=0;
$pneanoUf=0;
$pneanoRegiao=0;
$pneanoMunicipio=0;
$pneanoMesorregiao=0;

if($_SESSION['par']['muncod']){
    $sql = "select mundescricao from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
    $_SESSION['par']['mundescricao'] = $db->pegaUm($sql);
}
if($_SESSION['par']['estuf']) {
    $sql = "select estdescricao from territorios.estado where estuf = '{$_SESSION['par']['estuf']}'";
    $_SESSION['par']['estdescricao'] = $db->pegaUm($sql);
}
//include_once APPRAIZ . "www/par/_funcoes.php";
if($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'nao_informado':            
            naoInformado();
            exit;

        case 'valida_indicadores':
            validaIndicadores();
            exit;

        case 'altera_grafico':
            alteraGrafico();
            exit;
    }
}

//echo $_SESSION['par']['muncod'];

// CPF do administrador de sistemas
if( !$_SESSION['usucpf'] ){
    $_SESSION['usucpforigem'] = '';
    $_SESSION['usucpf'] = '';
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

    include APPRAIZ . "includes/jpgraph/jpgraph.php";
    include APPRAIZ . "includes/jpgraph/jpgraph_line.php";

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

    $width=600;
    $height=250;

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

function salvarNaoInformado() {
    global $db;
    $tabela = "sase.metainfcomplementar";
    if ($_SESSION['par']['itrid']==1){
        $sql = "select count(*) as qtd from {$tabela} where metid = {$_REQUEST['metid']} and estuf = '{$_SESSION['par']['estuf']}'";
    }else{
        $sql = "select count(*) as qtd from {$tabela} where metid = {$_REQUEST['metid']} and muncod = '{$_SESSION['par']['muncod']}'";
    }            
    
    $rs = $db->carregar($sql); 
    if ($rs[0]['qtd']>0){
        if ($_SESSION['par']['itrid']==1){
            $sql = "update {$tabela} 
                       set micinfcomplementar = '" . $_REQUEST['pneinformcomplementar'] . "' 
                     where metid = {$_REQUEST['metid']} and estuf = '{$_SESSION['par']['estuf']}'";
        }else{
            $sql = "update {$tabela} 
                       set micinfcomplementar = '" . $_REQUEST['pneinformcomplementar'] . "' 
                     where metid = {$_REQUEST['metid']} and muncod = '{$_SESSION['par']['muncod']}'";            
        }
    }else{
        if ($_SESSION['par']['itrid']==1){
            $sql = "insert into {$tabela} (metid, estuf, micinfcomplementar) ".
                   "values ({$_REQUEST['metid']}, '{$_SESSION['par']['estuf']}', '" . $_REQUEST['pneinformcomplementar'] . "')";            
        }else{
            $sql = "insert into {$tabela} (metid, muncod, micinfcomplementar) ".
                   "values ({$_REQUEST['metid']}, '{$_SESSION['par']['muncod']}', '" . $_REQUEST['pneinformcomplementar'] . "')";                        
        }
    }
    $db->executar($sql);
    $db->commit();
}



function salvarMeta18() {
    global $db;
    if ($_SESSION['par']['itrid']==1){
        $pnetipo = 'E';
    }else{
        $pnetipo = 'M';
    }        
    $sql = "select count(*) as qtd from sase.pne where subid = {$_REQUEST['subid']} and pnetipo = '{$pnetipo}'";        
    $rs = $db->carregar($sql);           
    if ($rs[0]['qtd']>0){
        $sql = "update sase.pne 
                   set pnepossuiplanoremvigente = " . $_REQUEST['pnepossuiplanoremvigente'] . ",
                       pneplanorefcaput = " . $_REQUEST['pneplanorefcaput'] . ",
                       pneanoprevisto = '" . $_REQUEST['pneanoprevisto'] . "'
                 where subid = {$_REQUEST['subid']}";                     
    }else{
        $sql = "insert into sase.pne (subid, estuf, muncod, pnetipo, pnepossuiplanoremvigente, pneplanorefcaput, pneanoprevisto) ".
               "values ({$_REQUEST['subid']}, '{$_SESSION['par']['estuf']}', '{$_SESSION['par']['muncod']}', '{$pnetipo}', "
               . "      {$_REQUEST['pnepossuiplanoremvigente']}, {$_REQUEST['pneplanorefcaput']}, {$_REQUEST['pneanoprevisto']})";            
    }

    $db->executar($sql);
    $db->commit();
}

function listagemPrincipal(){
    global $pneanoBrasil;
    global $pneanoUf;
    global $pneanoRegiao;
    global $pneanoMunicipio;
    global $pneanoMesorregiao;    
    global $db;
    $contador=0;
    
    $arr = carregarMetas($_REQUEST['metid'], $_SESSION['par']['itrid']);

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

            echo"<div style=\"margin-top:12px;\"><div id=\"titulo-meta-some\" style=\"float:left;\" class=\"titulo_box\" >$titulo </div>";
            if($_REQUEST['metid'] != 7 || $_REQUEST['metid'] != 19 || $_REQUEST['metid'] !=20){
                echo "<div id=\"mostraDados\" style=\"display: none;font-size:16px;font-weight:bold; \">	
			Região: ";if($regioes)listarNomeRegioes($arrRegioes,$s['subid']);
                echo "	<br>
			UF: ";if($estados)listarNomeEstados($arrEstados, $s['subid']);
                echo " <br>
			Mesorregião: ";if($mesoregioes)listarNomeMesoregioes($arrMesoregioes, $s['subid']);
                echo " <br>
		      </div>";
            }
            echo "<div id=\"titulo-meta-aparece\" style=\"display:none;text-align:justify;\">
		     <h2 >Meta {$_REQUEST['metid']}: {$s['mettitulo']}</h2>					
		  </div>				
		     <br style=\"clear:both;\">
	          <div id=\"titulo-meta-desaparece\" style=\"text-align:justify;font-weight:normal;color: #222;\">
		      <div class='well' style='padding: 15px;'>
		      	<h2><small><b style='color: #333'>{$s['mettitulo']}</b></small></h2>
		      </div>					
	          </div>";
        }  
        if ((in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
            (in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {
            echo"<tr>
                    <td style=\"font-size:16px;padding:25px 0 5px 5px;font-weight:bold;color: #333;\">";
                        if (!$existeDadosMun) {
                            echo " <span style='padding:10px;'><input type=\"button\" name=\"btnNaoInfo\" id=\"btnNaoInfo\" onclick=\"naoInformado({$valor}, '{$pneano}', ".$s['subid'].", ".$_REQUEST['metid'].", '".$dados[0]['pneid']."', 'div_grfMun_" . $s['subid'] . "', true)\" value=\"Não Informado\" class=\"btn btn-primary\"/></span>";
                        }
            echo "</td>
                    </tr>";
        }      
        echo "<table class=\"tabela_box_azul_escuro well\" style=\"padding: 10px !important;\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >";
        $subtitulo = $s['subtitulo'] === 'http://ideb.inep.gov.br/resultado/home.seam?cid=4212113'? 'Acesse as metas do IDEB em: <a target="_blank" href="http://ideb.inep.gov.br/resultado/home.seam?cid=4212113">ideb.inep.gov.br</a>': $s['subtitulo'];        

        $qtdLegenda = 1;
        $where = $legenda = '';
        $valor = ($_REQUEST['metid']==11 || $_REQUEST['metid']==14) ?'0':'1';

        $aSemLegendaOutros = array(11, 12, 13, 14, 17);
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
            if (!in_array($_REQUEST['metid'], $aSemLegendaOutros)) {
                $legenda .= "<div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #ff0000; float: left; margin-right: 2px'></span>Mesorregião</div>";
                $qtdLegenda++;
            }
        }
        if ($_SESSION['par']['itrid'] == 1) {
            if ($_SESSION['par']['estuf']) {
                $pneanoUf=selecionaAno($_REQUEST['metid'], 'E');
                $dadosMun = retornaDadosEst($valor, $s['subid'], $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneanoUf);
                $existeDadosMun = (bool)$dadosMun;
            }
        } else {
            if ($_SESSION['par']['muncod']) {
                $pneanoMunicipio=selecionaAno($_REQUEST['metid'], 'M');
                $dadosMun = retornaDadosMun($valor, $s['subid'], $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneanoMunicipio);
                $existeDadosMun = (bool)$dadosMun;
            }
        }
        $pneanoBrasil = selecionaAno($_REQUEST['metid'], 'B');
        $dados = retornaDadosPne($valor, $_REQUEST['metid'], $s['subid'], $pneanoBrasil, $_SESSION['par']['itrid'], $where);
        $existeResultado = (bool) $dados;

        $file = 'pne/notas_tecnicas/' . $s['subnotatecnica'];
        if($s['subnotatecnica'] && file_exists($file)){
            $subnotatecnica = '<a target="_blank" href="' . $file . '"><img height="30px" src="img/nt.png" alt="Nota Técnica" title="Nota Técnica" /></a>';
        }
        
        if ($_SESSION['par']['itrid'] == 1) {
            $pneano = selecionaAno($_REQUEST['metid'], 'E');
            $meta = 'Estado';
        } else {
            $pneano = selecionaAno($_REQUEST['metid'], 'M');
            $meta = 'Município';
        }

        if ((!in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
            (!in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {
            echo"<tr>
                    <td style=\"font-size:16px;padding:10px;font-weight:bold;color: #333;\">
                        {$subnotatecnica} {$subtitulo} ";
            echo "</td>
                    </tr>";
        }
        // Exibe somente se tiver resultado
 
                   
        if ($existeResultado) {
            if ((!in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
                (!in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {
                echo "<tr>
                		<td class=\"tabela_painel\">
                			<table class=\"tabela_painel\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >";

                if($existeDadosMun) {
                    $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                    $anos = array(2015, 2025);
                    echo "<tr>
                    		<td align=\"center\" style=\"padding: 20px; background-color: #fff; border-bottom: 1px solid #e3e3e3\">
                                <div style=\"display: block; margin-top: 5px; width: 100%; float: left;\">
                                    <div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #f7b850; float: left; margin-right: 2px; margin-top: 3px;'></span>Meta {$meta}</div>
                                    <div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #236B8E; float: left; margin-right: 2px; margin-top: 3px;'></span>Valor Atual</div>
                                </div>
                                <div class=\"div_grfMun\" id=\"div_grfMun_" . $s['subid'] . "\">";
		                    if ($dadosMun && is_array($dadosMun)) {
		                        $cor = '#236B8E';
		                        $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
		                        $mundescricao = $dadosMun['nome'];
		                        $metaBrasil = $dadosMun['subvalormetabrasil'];
		                        $dadosGrafico = array(
		                            'cor' => $cor,
		                            'descricao' => str_replace("'", '', $descricao),
		                            'valor' => $dadosMun['pnevalor2'],
		                            'metaTotal' => $metaTotal,
		                            'metaBrasil' => $metaBrasil,
		                            'tipo' => $dadosMun['subtipometabrasil'],
		                            'plotBandsCor' => '#f7b850',
		                            'plotBandsOuterRadius' => '115%',
		                            'title' => "Meta {$mundescricao}:",
		                            'anoprevisto' => $dadosMun['anoprevisto']
		                        );
		                        echo geraGraficoPNE('grafico_municipio_' . $s['subid'], $dadosGrafico);
		                    }
		                    echo "</div>
                                <table class=\"tbNormal\">
                                    <tr>
                                        <td class=\"row\" style=\"padding-top: 50px;\">
                                            <div class=\"div_lbl_grfMun col-md-2\" style='width: 140px;'>
                                                <label>Meta {$meta}:</label>
                                            </div>
                                            <div class=\"div_slider_grfMun col-md-6\" style=\"margin-top: 4px\">
                                                <input type=\"hidden\" id=\"hidID\" value=\"" . $s['subid'] . "\"/>
                                                <div class=\"slider-range\" id=\"slider_" . $s['subid'] . "\" pneid=\"" . $dadosMun['pneid'] . "\" =\"".$metaTotal."\" tipometa=\"".$dadosMun['subtipometabrasil']."\" sliderval=\"" . round($dadosMun['subvalormetabrasil']) . "\" pneano=\"" . $pneano . "\" valor=\"{$valor}\" subid=\"" . $s['subid'] . "\"></div>
                                            </div>
                                            <div class=\"div_text_grfMun col-md-2\" style=\"margin-top: -6px\">
                                                <input type=\"text\" style=\"min-width: 60px;\" class=\"form-control\" onchange=\"changeValue(this, {$_SESSION['par']['itrid']})\" subid=\"" . $s['subid'] . "\" id=\"txtSlider_" . $s['subid'] . "\" />
                                            </div>
                                        </td>
                                        <td style='padding-top: 50px;'>
                                            <div class=\"div_btn_NaoInfo\">
                                                <input type=\"button\" name=\"btnNaoInfo\" id=\"btnNaoInfo\" onclick=\"naoInformado({$valor}, '{$pneano}', ".$s['subid'].", ".$dadosMun['pneid'].", 'div_grfMun_" . $s['subid'] . "')\" value=\"Não Informado\" class=\"btn btn-primary\"/>
                                            </div>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            <div class=\"div_lbl_grfAno\" style='width: 100px;'>
                                                <label>Ano Previsto:</label>
                                            </div>
                                            <div class=\"div_combo_grfAno\">
                                                <select id=\"selAnoCorrente_".$s['subid']."\" onchange=\"changeValue($('#txtSlider_".$s['subid']."'), {$_SESSION['par']['itrid']})\" name=\"selAnoCorrente_".$s['subid']."\" class=\"form-control\">";

                                                    $a = $anos[0];
                                                    while($a <= $anos[1]){
                                                        $sel = $dadosMun['anoprevisto'] == $a ? 'selected' : '';
                                                        echo "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                                        $a++;
                                                    }
                    echo "                      </select>
                                            </div>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                   		</tr>";
                }
                echo "<tr><td align=center  style=\"padding: 20px; background-color: #fff;\">";
                if ($existeDadosMun) {
                    echo"<div>
                            <div style='float: left; margin-right: 20px;'><span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'></span>Brasil</div>
                                {$legenda}
                            <div style='clear: both;'></div>
                        </div>
                        <div style='clear: both;'></div>";
                }
            }
            if ($_REQUEST['metid']==18){          
                
                if ($dados[0]['pnepossuiplanoremvigente']=='t'){
                    $checkpnepossuiplanoremvigentetrue='checked';
                    $checkpnepossuiplanoremvigentefalse='';
                    $checkpnepossuiplanoremvigenteH='true';
                    $habilitaRadio = "";
                    $habilitaAno = "";
                }else if ($dados[0]['pnepossuiplanoremvigente']=='f'){
                    $checkpnepossuiplanoremvigentetrue='';
                    $checkpnepossuiplanoremvigentefalse='checked';
                    $checkpnepossuiplanoremvigenteH='false';
                    $habilitaRadio = "style='display:none;'";
                    $habilitaAno = "";                    
                }else{
                    $checkpnepossuiplanoremvigentetrue='';
                    $checkpnepossuiplanoremvigentefalse='';
                    $checkpnepossuiplanoremvigenteH='null';                    
                    $habilitaRadio = "style='display:none;'";
                    $habilitaAno = "style='display:none;'";                                  
                }
                if ($dados[0]['pneplanorefcaput']=='t'){
                    $checkpneplanorefcaputtrue='checked';
                    $checkpneplanorefcaputfalse='';
                    $checkpneplanorefcaputH='true';
                    $habilitaAno = "style='display:none;'";                                                      
                }else if ($dados[0]['pneplanorefcaput']=='f'){
                    $checkpneplanorefcaputtrue='';
                    $checkpneplanorefcaputfalse='checked';                    
                    $habilitaAno = "";             
                    $checkpneplanorefcaputH='false';                                         
                }else{
                    $checkpneplanorefcaputtrue='';
                    $checkpneplanorefcaputfalse='';
                    $habilitaAno = "style='display:none;'";
                    $checkpneplanorefcaputH='null';
                }                                                                
                echo "<tr><td><table><tr><td colspan='2'>Possui um plano de cargos e remuneração vigente?</td></tr>
                      <tr><td>
                      <input type='hidden' id='pnepossuiplanoremvigenteH' value='{$checkpnepossuiplanoremvigenteH}'>
                      <input type='radio' id='pnepossuiplanoremvigenteS' name='pnepossuiplanoremvigente' value='true' $checkpnepossuiplanoremvigentetrue onchange='javascript:changeRadio(this, \"trplanovigente\", \"tranoprevisto\", \"pneplanorefcaput\")'>Sim</td>
                          <td><input type='radio' id='pnepossuiplanoremvigenteN' name='pnepossuiplanoremvigente' value='false' $checkpnepossuiplanoremvigentefalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"trplanovigente\", \"pneplanorefcaput\")'>Não</td>
                      </tr>
                      <tr class='trplanovigente' {$habilitaRadio}><td colspan='2'>Plano de cargos e remuneração, em vigor, toma como referência o caput da meta 18</td></tr>
                      <tr class='trplanovigente' {$habilitaRadio}>                      
                          <td><input type='hidden' id='pneplanorefcaputH' value='{$checkpneplanorefcaputH}'>
                          <input type='radio' id='pneplanorefcaputS' name='pneplanorefcaput' value=true $checkpneplanorefcaputtrue onchange='javascript:changeRadio(this, \"\",\"tranoprevisto\", \"\")'>Sim</td>
                          <td><input type='radio' id='pneplanorefcaputN' name='pneplanorefcaput' value=false $checkpneplanorefcaputfalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"\", \"\")'>Não</td>
                      </tr>
                      <tr class='tranoprevisto' {$habilitaAno}>
                         <td colspan='2' id='tdtextoanoprevisto'>
                            <div class=\"div_lbl_grfAno\">
                                <label>Ano Previsto:</label>
                            </div>
                            <div class=\"div_combo_grfAno\">
                                <select id=\"pneanoprevisto\" name=\"pneanoprevisto\">";
                                    $anos = array(2015, 2025);
                                    $a = $anos[0];
                                    while($a <= $anos[1]){
                                        $sel = $dados[0]['pneanoprevisto'] == $a ? 'selected' : '';
                                        echo "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                        $a++;
                                    }

                     echo '     </select>
                            </div></td></tr>
                      <tr><td></td>                          
                      </tr></table><input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarMeta18(' . $s['subid'] . ')" value="Salvar Informações Complementares" class="btn btn-primary"></td></tr>';
                      
            } 
            $anos = array();
            $dadosAgrupados = array();
            if($dados)
            {
                foreach ($dados as $dado)
                {
                    $dadosAgrupados[str_replace('1Brasil', 'Brasil', tirar_acentos($dado['descricao'] . '::' . $dado['pnetipo']))] = $dado;
                    $anos[$dado['pneano']] = $dado['pneano'];
                    if ((in_array($_REQUEST['metid'], array(11,12,13,14,15,17,19,20))&&($_SESSION['par']['itrid'] != 1))||
                        (in_array($_REQUEST['metid'], array(15,19,20))&&($_SESSION['par']['itrid'] == 1))) {
                    	echo '<div align="left" class="metanaoinformada_' . $dado['metid'] . '">';
                    	echo "<textarea placeHolder=\"Informações Complementares\" data-pneid=\"{$dado['pneid']}\" id=\"pneinformcomplementar_{$s['subid']}\" name=\"pneinformcomplementar[]\" rows=\"5\" style=\"width: 96%; margin: 10px;\">{$dado['micinfcomplementar']}</textarea>";
                    	echo '<input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarNaoInformado(' . $s['subid'] . ')" value="Salvar Informações Complementares" class="btn btn-primary">';
            			echo '</div>';
                    }
                }

                $count = 0;
                foreach($dadosAgrupados as $descricao => $dados){
                    $habilitaBrasil = true;
                    if ((in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
                        (in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {
                        $habilitaBrasil = false;
                    }else if ($_SESSION['par']['itrid'] == 1) {
                        if($dados['metid'] == '15' || $dados['metid'] == '18' || $dados['metid'] == '19'){
                            $habilitaBrasil = false;
                        }
                    } else {
                        if($dados['metid'] == '11' || $dados['metid'] == '12' || $dados['metid'] == '13' || $dados['metid'] == '14' || $dados['metid'] == '15' || $dados['metid'] == '17' || $dados['metid'] == '18' || $dados['metid'] == '19'){
                            $habilitaBrasil = false;
                        }
                    }
                    

                    if($habilitaBrasil) {
                        $count++;
                        $descricao = substr($descricao, 0, strrpos($descricao, '::'));
                        $metaTotal = 'P' == $dados['subtipometabrasil'] ? 100 : $dados['subvalormetabrasil'];
                        $metaBrasil = $dados['subvalormetabrasil'];

                        $cor = '#000000';
                        switch ($dados['pnetipo']) {

                            // Amarelo  (Município)
                            case ('M'):
                                $cor = '#FFCB00';
                                break;

                            // Rosinha  (Região)
                            case ('R'):
                                $cor = '#FF70EC';
                                break;

                            // Vermelho (Mesoregião)
                            case ('S'):
                                $cor = '#ff0000';
                                break;

                            // Verde (Brasil)
                            // Azul  (Estado)
                            case ('E'):
                                $cor = 'Brasil' == $descricao ? '#00BF0A' : '#2843FF';
                                break;
                        }

                        $dadosGrafico = array(
                            'cor' => $cor,
                            'descricao' => str_replace("'", '', $descricao),
                            'valor' => $dados['pnevalor2'],
                            'metaTotal' => $metaTotal,
                            'metaBrasil' => $metaBrasil,
                            'tipo' => $dados['subtipometabrasil']
                        );
                        echo geraGraficoPNE('grafico_' . $count . '_' . $count2, $dadosGrafico);
                    }
                }
            }
            echo "</td></tr></table></td></tr><span style='color: #ffcb00'></span>";
            if($arr && ($s['subtitulo'] !== "Não foi traçada trajetória para esta meta")){
                if($s['metfontemunicipio'] === $s['metfonteestado']){
                    echo "<tr><td style=\"padding: 5px;\">Fonte: {$s['metfonteestado']}</td></tr>";
                }else{
                    echo "<tr><td style=\"padding: 5px;\">Fonte: Estado, Região e Brasil - {$s['metfonteestado']}</td></tr>";
                }
            }

            if(($municipios || $mesoregioes)  && ($_REQUEST['metid'] != 14 && $_REQUEST['metid'] != 17 && $_REQUEST['metid'] != 13) && ($s['subtitulo'] != "Não foi traçada trajetória para esta meta")){
                if($s['metfontemunicipio'] !== $s['metfonteestado']){
                    echo "<tr><td>Fonte: Município e Mesorregião - {$s['metfontemunicipio']}</td></tr>";
                }
            }

            // Somente no Indicador 9B
            if ('36' == $s['subid']) {
                echo "<tr><td><strong>Nota: O objetivo desse indicador é reduzir em 50% a taxa de analfabetismo funcional.</strong></td></tr>";
            }
        }else{
            if ($_REQUEST['metid']==18){              
                if ($dados[0]['pnepossuiplanoremvigente']=='t'){
                    $checkpnepossuiplanoremvigentetrue='checked';
                    $checkpnepossuiplanoremvigentefalse='';
                    $checkpnepossuiplanoremvigenteH='true';
                    $habilitaRadio = "";
                    $habilitaAno = "";
                }else if ($dados[0]['pnepossuiplanoremvigente']=='f'){
                    $checkpnepossuiplanoremvigentetrue='';
                    $checkpnepossuiplanoremvigentefalse='checked';
                    $checkpnepossuiplanoremvigenteH='false';
                    $habilitaRadio = "style='display:none;'";
                    $habilitaAno = "";                    
                }else{
                    $checkpnepossuiplanoremvigentetrue='';
                    $checkpnepossuiplanoremvigentefalse='';
                    $checkpnepossuiplanoremvigenteH='null';                    
                    $habilitaRadio = "style='display:none;'";
                    $habilitaAno = "style='display:none;'";                                  
                }
                if ($dados[0]['pneplanorefcaput']=='t'){
                    $checkpneplanorefcaputtrue='checked';
                    $checkpneplanorefcaputfalse='';
                    $checkpneplanorefcaputH='true';
                    $habilitaAno = "style='display:none;'";                                                      
                }else if ($dados[0]['pneplanorefcaput']=='f'){
                    $checkpneplanorefcaputtrue='';
                    $checkpneplanorefcaputfalse='checked';                    
                    $habilitaAno = "";             
                    $checkpneplanorefcaputH='false';                                         
                }else{
                    $checkpneplanorefcaputtrue='';
                    $checkpneplanorefcaputfalse='';
                    $habilitaAno = "style='display:none;'";
                    $checkpneplanorefcaputH='null';
                }                                                                
                echo "<tr><td><table><tr><td colspan='2'>Possui um plano de cargos e remuneração vigente?</td></tr>
                      <tr><td>
                      <input type='hidden' id='pnepossuiplanoremvigenteH' value='{$checkpnepossuiplanoremvigenteH}'>
                      <input type='radio' id='pnepossuiplanoremvigenteS' name='pnepossuiplanoremvigente' value='true' $checkpnepossuiplanoremvigentetrue onchange='javascript:changeRadio(this, \"trplanovigente\", \"tranoprevisto\", \"pneplanorefcaput\")'>Sim</td>
                          <td><input type='radio' id='pnepossuiplanoremvigenteN' name='pnepossuiplanoremvigente' value='false' $checkpnepossuiplanoremvigentefalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"trplanovigente\", \"pneplanorefcaput\")'>Não</td>
                      </tr>
                      <tr class='trplanovigente' {$habilitaRadio}><td colspan='2'>Plano de cargos e remuneração, em vigor, toma como referência o caput da meta 18</td></tr>
                      <tr class='trplanovigente' {$habilitaRadio}>                      
                          <td><input type='hidden' id='pneplanorefcaputH' value='{$checkpneplanorefcaputH}'>
                          <input type='radio' id='pneplanorefcaputS' name='pneplanorefcaput' value=true $checkpneplanorefcaputtrue onchange='javascript:changeRadio(this, \"\",\"tranoprevisto\", \"\")'>Sim</td>
                          <td><input type='radio' id='pneplanorefcaputN' name='pneplanorefcaput' value=false $checkpneplanorefcaputfalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"\", \"\")'>Não</td>
                      </tr>
                      <tr class='tranoprevisto' {$habilitaAno}>
                         <td colspan='2' id='tdtextoanoprevisto'>
                            <div class=\"div_lbl_grfAno\">
                                <label>Ano Previsto:</label>
                            </div>
                            <div class=\"div_combo_grfAno\">
                                <select id=\"pneanoprevisto\" name=\"pneanoprevisto\">";
                                    $anos = array(2015, 2025);
                                    $a = $anos[0];
                                    while($a <= $anos[1]){
                                        $sel = $dados[0]['pneanoprevisto'] == $a ? 'selected' : '';
                                        echo "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                        $a++;
                                    }

                     echo '     </select>
                            </div></td></tr>
                      <tr><td></td>                          
                      </tr></table><input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarMeta18(' . $s['subid'] . ')" value="Salvar Informações Complementares" class="btn btn-primary"></td></tr>';
                      
            }             
            if ((in_array($_REQUEST['metid'], array(11,12,13,14,15,17,19,20))&&($_SESSION['par']['itrid'] != 1))||
                (in_array($_REQUEST['metid'], array(15,19,20))&&($_SESSION['par']['itrid'] == 1))) {
                echo '<div align="left" class="metanaoinformada_' . $dado['metid'] . '">';
                echo "<textarea placeHolder=\"Informações Complementares\" data-pneid=\"{$dado['pneid']}\" id=\"pneinformcomplementar_{$s['subid']}\" name=\"pneinformcomplementar[]\" rows=\"5\" style=\"width: 96%; margin: 10px;\">{$dado['micinfcomplementar']}</textarea>";
                echo '<input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarNaoInformado(' . $s['subid'] . ')" value="Salvar Informações Complementares" class="btn btn-primary">';
                echo '</div>';
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
    $grupoExcluido = array(7,15,19,20);

    if($_REQUEST['metid'] != 7
        && $_REQUEST['metid'] != 15
        && $_REQUEST['metid'] != 181
        && $_REQUEST['metid'] != 19
        && $_REQUEST['metid'] != 20)
    {
        mostrarComboPopupLocal( 'Estado', 'slEstado',  $sql,$sqlselecionados, 'Selecione os Estados', null,'atualizarRelacionadosRegiao(2)',false, $_SESSION['par']['itrid']);
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

    $titulo = "";
    if($_SESSION['par']['itrid'] == 1){
        $titulo = $_SESSION['par']['estdescricao'];
    } else {
        $titulo = $_SESSION['par']['mundescricao'];
    }
    
    $menu = '<div class="ibox ibox float-e-margins" style="margin-bottom: -40px;">' .
    			'<div class="ibox-title titulo_box titulo-grfpne" style="border-width: 0px; min-height: 20px;">' . 
    				'<h5>Metas PNE - ' . $titulo . '</h5>' . 
	            	'<div class="ibox-tools">' .
	            		'<a style="margin-right: 10px; color: #1ab394;" name="btnValidar" onclick="validaIndicadores(\'Teste\')" href="#">' .
	    					'<i class="fa fa-check"></i>' .
	    				'</a>' .
	    				'<a style="color: #1a7bb9;" onclick="printFunction();" href="#">' .
	    					'<i class="fa fa-print"></i>' .
	    				'</a>' .
	            	'</div>' .
    			'</div>';
    
    $menu .= '<table id="apres" width="100%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint table">'
        . '		<tr>';
    $menu .= '		<td>';
    $menu .= '			<div class="btn-group">';
    
    $nlinhas = count($rs) - 1;
        
    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);

        if ($url != $meta && $j == 0)
            $gifaba = 'aba_nosel_ini2.gif';
        elseif ($url == $meta && $j == 0)
            $gifaba = 'aba_esq_sel_ini2.gif';
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
            $giffundo_aba = 'aba_fundo_nosel2.gif';
            $cor_fonteaba = '#4488cc';
        }

        if ($meta != $url)
        {
            $menu .= '<a style="width: 70px; padding: 6px 0px;" href="#meta' . $meta . '" onclick="selecionaAba('.$meta.'); $(\'#metid\').val('.$meta.');
					listarSubmetas(pegaSelecionados(\'slRegiao[]\'),pegaSelecionados(\'slEstado[]\'),pegaSelecionados(\'slMesoregiao[]\'),
                                        '.$_SESSION['par']['itrid'].')" class="btn btn-success">' . $descricao . '</a>';
        } 
        else
        {
            $menu .= $descricao;
        }
    }

    if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
        $gifaba = 'aba_dir_sel_fim.gif';
    else
        $gifaba = 'aba_nosel_fim.gif';

    $menu .= '			</div>
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

function naoInformado(){
    global $db;
    $pneid       = '';
    $subid       = $_REQUEST['subid'];
    $metid       = $_REQUEST['metid'];
    $pneano      = $_REQUEST['pneano'];
    $valor       = $_REQUEST['valor'];
    $anoprevisto = $_REQUEST['anoprevisto'];

    if ($_SESSION['par']['itrid'] == 1) {
        $sql = "select pneid from sase.pne where subid = {$subid} and estuf = '{$_SESSION['par']['estuf']}' and pneano = {$anoprevisto}";
    } else {
        $sql = "select pneid from sase.pne where subid = {$subid} and muncod = '{$_SESSION['par']['muncod']}' and pneano = {$anoprevisto}";
    }    
    $pneid = $db->pegaUm($sql);

    if($pneid != ''){
        if($db->commit()){
            if ($_SESSION['par']['itrid'] == 1){
                if ($_SESSION['par']['estuf']) {
                    if (in_array($metid, array(15,19,20))){
                        $sql = "update sase.metainfcomplementar set micinfcomplementar = null where estuf = '" . $_SESSION['par']['estuf'] . "' and metid = {$metid}";
                        $db->executar($sql);
                    }
                    $sql = "update sase.pne set pnevalormeta = null, pnetipometa = null, pnesemvalor = 't' where estuf = '" . $_SESSION['par']['estuf'] . "' and subid = {$subid}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $sql = "update sase.pne set pnevalormeta = 0, pnesemvalor = 'f' where pneid = {$pneid}";
                        $db->executar($sql);

                        if($db->commit()) {
                            $dadosMun = retornaDadosEst($valor, $subid, $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneano);

                            if ($dadosMun && is_array($dadosMun)) {
                                $cor = '#236B8E';
                                $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                                $mundescricao = $dadosMun['estdescricao'];
                                $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                                $metaBrasil = $dadosMun['subvalormetabrasil'];
                                $dadosGrafico = array(
                                    'cor' => $cor,
                                    'descricao' => str_replace("'", '', $descricao),
                                    'valor' => $dadosMun['pnevalor2'],
                                    'metaTotal' => $metaTotal,
                                    'metaBrasil' => $metaBrasil,
                                    'tipo' => $dadosMun['subtipometabrasil'],
                                    'plotBandsCor' => '#f7b850',
                                    'plotBandsOuterRadius' => '115%',
                                    'title' => "Meta {$mundescricao}:",
                                    'anoprevisto' => $anoprevisto
                                );
                                echo geraGraficoPNE('grafico_municipio_' . $subid, $dadosGrafico);
                            }
                        }
                    }
                }
            } else {
                if ($_SESSION['par']['muncod']) {
                    if (in_array($metid, array(11,12,13,14,15,17,19,20))){
                        $sql = "update sase.metainfcomplementar set micinfcomplementar = null where muncod = '" . $_SESSION['par']['muncod'] . "' and metid = {$metid}";
                        $db->executar($sql);
                    }
                    $sql = "update sase.pne set pnevalormeta = null, pnetipometa = null, pnesemvalor = 't' where muncod = '" . $_SESSION['par']['muncod'] . "' and subid = {$subid}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $sql = "update sase.pne set pnevalormeta = 0, pnesemvalor = 'f' where pneid = {$pneid}";
                        $db->executar($sql);

                        if($db->commit()) {
                            $dadosMun = retornaDadosEst($valor, $subid, $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneano);

                            if ($dadosMun && is_array($dadosMun)) {
                                $cor = '#236B8E';
                                $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                                $mundescricao = $dadosMun['mundescricao'];
                                $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                                $metaBrasil = $dadosMun['subvalormetabrasil'];
                                $dadosGrafico = array(
                                    'cor' => $cor,
                                    'descricao' => str_replace("'", '', $descricao),
                                    'valor' => $dadosMun['pnevalor2'],
                                    'metaTotal' => $metaTotal,
                                    'metaBrasil' => $metaBrasil,
                                    'tipo' => $dadosMun['subtipometabrasil'],
                                    'plotBandsCor' => '#f7b850',
                                    'plotBandsOuterRadius' => '115%',
                                    'title' => "Meta {$mundescricao}:",
                                    'anoprevisto' => $anoprevisto
                                );
                                echo geraGraficoPNE('grafico_municipio_' . $subid, $dadosGrafico);
                            }
                        }
                    }
                }
            }
        }
    }else{
        
        if ($_SESSION['par']['itrid'] == 1){            
            if ($_SESSION['par']['estuf']) {                
                if (in_array($metid, array(15,19,20))){
                    
                    $sql = "update sase.metainfcomplementar set micinfcomplementar = null where estuf = '" . $_SESSION['par']['estuf'] . "' and metid = {$metid}";
                    $db->executar($sql);
                    $db->commit();
                }
            }
        } else {
            if ($_SESSION['par']['muncod']) {
                if (in_array($metid, array(11,12,13,14,15,17,19,20))){
                    $sql = "update sase.metainfcomplementar set micinfcomplementar = null where muncod = '" . $_SESSION['par']['muncod'] . "' and metid = {$metid}";
                    $db->executar($sql);
                    $db->commit();
                }
            }
        }
    }

}

/**
 * 
 * @global type $db
 * @param type $valor
 * @param type $subid
 * @param type $estuf
 * @param type $pneano
 * @return type
 */
function retornaDadosEst($valor, $subid, $metid, $estuf, $pneano){
    global $db;
    if ((in_array($metid, array(11,12,13,14,15,17,19,20))&&($itrid != 1))||
        (in_array($metid, array(15,19,20))&&($itrid == 1))) {            
        $sql = "
            with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta when sub.subvalormetabrasil is not null then sub.subvalormetabrasil else 0 end as valormeta, case when pne.pnetipometa is not null then  pne.pnetipometa else sub.subtipometabrasil end as tipometa, pneano from sase.submeta sub left join sase.pne pne on sub.subid = pne.subid and estuf = '" . $_SESSION['par']['estuf'] . "' and pnetipo = 'E' and pnevalormeta is not null and pne.subid = sub.subid where sub.metid = {$metid} )
            SELECT
                pne.pneid,
                m.estuf || ' - ' || mundescricao as descricao,
                estdescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subid,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pne  pne
                inner join sase.submeta sub on sub.subid = pne.subid
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
            WHERE sub.metid = {$metid}  and pneano = $pneano
                AND (
                  (pne.estuf = '" . $estuf . "' and pnetipo = 'E')
                )
            ORDER BY ordem, sub.subordem, pne.subid, pne.pneano, pnetipo, descricao";
    }else{
        $sql = "
            with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta when sub.subvalormetabrasil is not null then sub.subvalormetabrasil else 0 end as valormeta, case when pne.pnetipometa is not null then  pne.pnetipometa else sub.subtipometabrasil end as tipometa, pneano from sase.submeta sub left join sase.pne pne on sub.subid = pne.subid and estuf = '" . $_SESSION['par']['estuf'] . "' and pnetipo = 'E' and pnevalormeta is not null and pne.subid = sub.subid where sub.subid = {$subid} )
            SELECT
                pne.pneid,
                m.estuf || ' - ' || mundescricao as descricao,
                estdescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subid,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pne  pne
                inner join sase.submeta sub on sub.subid = pne.subid
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
            WHERE sub.subid = {$subid}  and pneano = $pneano
                AND (
                  (pne.estuf = '" . $estuf . "' and pnetipo = 'E')
                )
            ORDER BY ordem, sub.subordem, pne.subid, pne.pneano, pnetipo, descricao";
    }
    return $db->pegaLinha($sql);    
}

/**
 * 
 * @global type $db
 * @param type $valor
 * @param type $subid
 * @param type $muncod
 * @param type $pneano
 * @return type
 */
function retornaDadosMun($valor, $subid, $metid, $muncod, $pneano){
    global $db;
    if ((in_array($metid, array(11,12,13,14,15,17,19,20))&&($itrid != 1))||
        (in_array($metid, array(15,19,20))&&($itrid == 1))) {        
    $sql = "
            with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta when sub.subvalormetabrasil is not null then sub.subvalormetabrasil else 0 end as valormeta, case when pne.pnetipometa is not null then  pne.pnetipometa else sub.subtipometabrasil end as tipometa, pneano from sase.submeta sub left join sase.pne pne on sub.subid = pne.subid and muncod = '" . $_SESSION['par']['muncod'] . "' and pnetipo = 'M' and pnevalormeta is not null and pne.subid = sub.subid where sub.metid = {$metid} )
            SELECT
                pne.pneid,
                m.estuf || ' - ' || mundescricao as descricao,
                mundescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subid,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pne  pne
                inner join sase.submeta sub on sub.subid = pne.subid
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
           where sub.metid = {$metid} ";
    }else{
        $sql = "             with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta when sub.subvalormetabrasil is not null then sub.subvalormetabrasil else 0 end as valormeta, case when pne.pnetipometa is not null then  pne.pnetipometa else sub.subtipometabrasil end as tipometa, pneano from sase.submeta sub left join sase.pne pne on sub.subid = pne.subid and muncod = '" . $_SESSION['par']['muncod'] . "' and pnetipo = 'M' and pnevalormeta is not null and pne.subid = sub.subid where sub.subid = {$subid} )
            SELECT
                pne.pneid,
                m.estuf || ' - ' || mundescricao as descricao,
                mundescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subid,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pne  pne
                inner join sase.submeta sub on sub.subid = pne.subid
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
           WHERE sub.subid = {$subid}  and pneano = $pneano ";
    }
    $sql .= "  AND ((pne.muncod = '" . $muncod . "' and pnetipo = 'M'))
            ORDER BY ordem, sub.subordem, pne.subid, pne.pneano, pnetipo, descricao"; 
    //ver($sql, d);
    return $db->pegaLinha($sql);    
}

function validaIndicadores(){
    global $db;
    $where = '';
    if($_REQUEST['tipo'] == 'est'){
        $where = "where pne.estuf = '{$_SESSION['par']['estuf']}'";
    } else {
        $where = "where pne.muncod = '{$_SESSION['par']['muncod']}'";
    }

    if($where != '') {
        $sql = "select
                    sub.subtitulo
                from sase.pne pne
                inner join sase.submeta sub on sub.subid = pne.subid and sub.substatus = 'A'
                inner join sase.meta met on met.metid = sub.metid
                {$where}
                and case when
                        met.metid = 1 or
                        met.metid = 2 or
                        met.metid = 3 or
                        met.metid = 5 or
                        met.metid = 8 or
                        met.metid = 9
                    then case
                        when pne.muncod is not null then pne.pneano = 2010
                        when pne.estuf is not null then pne.pneano = 2012
                        else pne.pneano = 2010
                         end
                    when
                        met.metid = 4
                    then pne.pneano = 2010
                    when
                        met.metid = 12 or
                        met.metid = 13 or
                        met.metid = 14 or
                        met.metid = 17
                    then pne.pneano = 2012
                    else pne.pneano = 2013
                    end
                and pne.pnesemvalor = 't'
                and (pne.pnevalormeta is null or pne.pnevalormeta = 0)
                order by met.metid";
        $dados = $db->carregar($sql);
        if(is_array($dados)) {
            $msg = "Os indicadores com inconsistência são:\n";
            foreach ($dados as $d) {
                $msg .= "    " . $d['subtitulo'] . "\n";
            }
            echo $msg;
        } else {
            echo "Não existem indicadores com inconsistência.";
        }
    } else {
        echo "Dados invalidos.";
    }    
}

function alteraGrafico(){
    global $db;
    $pneano       = $_REQUEST['pneano'];
    $subid        = $_REQUEST['subid'];
    $valor        = $_REQUEST['valor'];
    $metid        = $_REQUEST['metid'];
    $pneid        = $_POST['pneid'];
    $muncod       = $_POST['muncod'];
    $pneano       = $_POST['pneano'];
    $anoprevisto  = $_POST['anoprevisto'];
    $pnetipometa  = $_POST['pnetipometa'];
    $pnevalormeta = str_replace(',', '.', $_POST['pnevalormeta']);
    $tipo         = $_POST['tipo'];
    
    if ($_SESSION['par']['itrid'] == 1){
        if ($_SESSION['par']['estuf']) {

            $sql = "update sase.pne set pnevalormeta = null, pnetipometa = null where estuf = '".$_SESSION['par']['estuf']."' and subid = {$subid}";
            $db->executar($sql);
            if($db->commit()) {

                $sql = "select pneid from sase.pne where subid = {$subid} and estuf = '{$_SESSION['par']['estuf']}' and pneano = '{$anoprevisto}'";
                $pneid = $db->pegaUm($sql);

                if(!empty($pneid)) {
                    $sql = "update sase.pne set pnetipometa = '{$pnetipometa}', pnevalormeta = {$pnevalormeta} where pneid = {$pneid}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $dadosMun = retornaDadosEst($valor, $subid, $metid, $_SESSION['par']['estuf'], $pneano);

                        if ($dadosMun && is_array($dadosMun)) {
                            $cor = '#236B8E';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['estdescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                            $metaBrasil = $dadosMun['subvalormetabrasil'];
                            $dadosGrafico = array(
                                'cor' => $cor,
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['pnevalor2'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $metaBrasil,
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => '#f7b850',
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subid, $dadosGrafico);
                        }
                    }
                } else {
                    $sql = "insert into sase.pne (subid, pneano, estuf, pnetipo, pnetipometa, pnevalormeta, pnesemvalor) values ({$subid}, {$anoprevisto}, '{$_SESSION['par']['estuf']}', 'E', '{$pnetipometa}', {$pnevalormeta}, 't') returning pneid";
                    $pneid = $db->pegaUm($sql);
//                            ver($sql, $pneid, d);
                    if($db->commit()){
                        $dadosMun = retornaDadosEst($valor, $subid, $metid, $_SESSION['par']['estuf'], $pneano);                        

                        if ($dadosMun && is_array($dadosMun)) {
                            $cor = '#236B8E';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['estdescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                            $metaBrasil = $dadosMun['subvalormetabrasil'];
                            $dadosGrafico = array(
                                'cor' => $cor,
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['pnevalor2'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $metaBrasil,
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => '#f7b850',
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subid, $dadosGrafico);
                        }
                    }
                }
            }
        }
    } else {
        if ($_SESSION['par']['muncod']) {

            $sql = "update sase.pne set pnevalormeta = null, pnetipometa = null where muncod = '".$_SESSION['par']['muncod']."' and subid = {$subid}";

            $db->executar($sql);
            if($db->commit()) {

                $sql = "select pneid from sase.pne where subid = {$subid} and muncod = '{$_SESSION['par']['muncod']}' and pneano = '{$anoprevisto}'";
                $pneid = $db->pegaUm($sql);

                if (!empty($pneid)) {
                    $sql = "update sase.pne set pnetipometa = '{$pnetipometa}', pnevalormeta = {$pnevalormeta}, pnesemvalor = 't' where pneid = {$pneid}";
//                        ver($pneid, $sql, d);

                    $db->executar($sql);
                    if ($db->commit()) {
                        $dadosMun = retornaDadosMun($valor, $subid, $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneano);

                        if ($dadosMun && is_array($dadosMun)) {
                            $cor = '#236B8E';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['mundescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                            $metaBrasil = $dadosMun['subvalormetabrasil'];
                            $dadosGrafico = array(
                                'cor' => $cor,
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['pnevalor2'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $metaBrasil,
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => '#f7b850',
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subid, $dadosGrafico);
                        }
                    }
                } else {
                    $sql = "insert into sase.pne (subid, pneano, muncod, pnetipo, pnetipometa, pnevalormeta, pnesemvalor) values ({$subid}, {$anoprevisto}, '{$_SESSION['par']['muncod']}', 'M', '{$pnetipometa}', {$pnevalormeta}, 't') returning pneid";

                    $pneid = $db->pegaUm($sql);

                    if ($db->commit()) {
                        $dadosMun = retornaDadosMun($valor, $subid, $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneano);

                        if ($dadosMun && is_array($dadosMun)) {
                            $cor = '#236B8E';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['mundescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                            $metaBrasil = $dadosMun['subvalormetabrasil'];
                            $dadosGrafico = array(
                                'cor' => $cor,
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['pnevalor2'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $metaBrasil,
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => '#f7b850',
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subid, $dadosGrafico);
                        }
                    }
                }
            }
        }
    }
}

/**
 * 
 * @param type $metid
 */
function selecionaTitulo($metid){
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
}

function selecionaAno($metid, $paetipo){
//    global $pneanoBrasil;
//    global $pneanoUf;
//    global $pneanoRegiao;
//    global $pneanoMunicipio;
//    global $pneanoMesorregiao;
//    switch ($metid) {
//        case(1): case(2): case(3): case(5): case(8): case(9):
//            $pneanoBrasil    = $pneanoUf = $pneanoRegiao = 2012;
//            $pneanoMunicipio = $pneanoMesorregiao = 2010;
//            break;
//        case(4):
//            $pneanoBrasil = $pneanoUf = $pneanoRegiao = $pneanoMunicipio = $pneanoMesorregiao = 2010;
//            break;
//        case(12): case(13): case(14): case(17):
//            $pneanoBrasil = $pneanoUf = $pneanoRegiao = $pneanoMunicipio = $pneanoMesorregiao = 2012;
//            break;
//        default:
//            $pneanoBrasil = $pneanoUf = $pneanoRegiao = $pneanoMunicipio = $pneanoMesorregiao = 2013;
//            break;
//    }    
    global $db;
    $sql = "select paeano
              from sase.pneanoexibicao
             where subid in (select subid
                               from sase.submeta
                              where metid = {$metid})
               and paetipo = '{$paetipo}'";       
    $rs = $db->pegaUm($sql);   
    
    if ($rs==''){
        $rs='2015';
    }

    return $rs;
}

/**
 * 
 * @param type $valor
 * @param type $subid
 * @param type $pneanoBrasil
 * @param type $where
 * @return type
 */
function retornaDadosPne($valor, $metid, $subid, $pneanoBrasil, $itrid, $where){
    global $db;    
    if ((in_array($metid, array(11,12,13,14,15,17,19,20))&&($itrid != 1))||
        (in_array($metid, array(15,19,20))&&($itrid == 1))) {    
        $sql ="
            SELECT meta.metid,
                   pne.micinfcomplementar
            FROM sase.metainfcomplementar  pne
            inner join sase.meta meta on pne.metid = meta.metid
            left join territorios.estado e on e.estuf = pne.estuf
            left join territorios.municipio m on m.muncod = pne.muncod
            WHERE pne.metid = {$metid} ".$where;
    }else{
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
                        pne.pneid,
                        pne.subid,
                        sub.metid,
                        sub.subtitulo,
                        subtipometabrasil,
                        subvalormetabrasil,
                        pne.pnepossuiplanoremvigente,
                        pne.pneplanorefcaput,
                        pne.pneanoprevisto
                    FROM sase.pne  pne
                    inner join sase.submeta sub on sub.subid = pne.subid
                    left join territorios.estado e on e.estuf = pne.estuf
                    left join territorios.municipio m on m.muncod = pne.muncod
                    left join territorios.regiao r on r.regcod = pne.regcod
                    left join territorios.mesoregiao mr on mr.mescod = pne.mescod
                WHERE sub.subid = {$subid}";
                if ($_REQUEST['metid']!=18){
                    $sql .= " AND ((pne.estuf is null and pnetipo = 'E' and pneano = $pneanoBrasil)$where)";
                }else{
                    $sql .= "$where";
                }     
    }
    //ver($sql,d);
    return $db->carregar($sql);                    
}

function carregarMetas($metid, $itrid){
    global $db;
    if ((in_array($metid, array(11,12,13,14,15,17,19,20))&&($itrid != 1))||
        (in_array($metid, array(15,19,20))&&($itrid == 1))) {
        $sql = "select meta.metid, 
                       meta.mettitulo as subtitulo,
                       meta.metfontemunicipio,
                       meta.metfonteestado,
                       meta.mettitulo, 
                       '' as subnotatecnica                       
                  from sase.meta meta   
                 where meta.metid = ".$metid;
    }else if($metid==18){
        $sql = "select sub.subid, 
                       sub.subtitulo,
                       meta.metfontemunicipio,
                       meta.metfonteestado,
                       meta.mettitulo, 
                       sub.subnotatecnica 
                  from sase.submeta as sub 
                  left join sase.meta meta 
                    on (sub.metid = meta.metid) 
                 where sub.substatus = 'A' 
                   and sub.metid = ".$metid." 
                 order by sub.subordem ASC";        
    }else{
        $sql = "select sub.subid, 
                       sub.subtitulo,
                       meta.metfontemunicipio,
                       meta.metfonteestado,
                       meta.mettitulo, 
                       sub.subnotatecnica 
                  from sase.submeta as sub 
                  left join sase.meta meta 
                    on (sub.metid = meta.metid) 
                 where sub.substatus = 'A' 
                   and sub.metid = ".$metid." 
                 order by sub.subordem ASC";
    }    
    //ver($sql,d);
    return $db->carregar($sql);
}


function geraGraficoPNE($nomeUnico, $dados)
{
	$meta = $dados['metaBrasil'];
	$metaTotal = $dados['metaTotal'];
	$metaFormatada = number_format($meta, 0, ',', '.');

	$complemento = $simbolo = '';

	$casasDecimais = 0;

	switch ($dados['tipo']) {
		case ('P'):
			$complemento = $simbolo = '%';
			$metaFormatada = $meta;
			$casasDecimais = 1;
			break;
		case ('A'):
			$complemento = ' anos';
			/**
			 * Victor Martins Machado
			 * Incluído para mostrar o valor com uma casa decimal
			 */
			$metaFormatada = $meta;
			$casasDecimais = 1;
			break;
		case ('M'):
			$complemento = ' matrículas';
			break;
		case ('T'):
			$complemento = ' títulos';
			break;
	}

	$formatter = "function () { return '<div style=\"text-align:center\"><span style=\"font-size:20px;color:black; font-weight: normal;\">' + number_format(this.y, " . $casasDecimais .", ',', '.') + '" . $simbolo ."</span><br/>' +
                                       '<span style=\"font-size:13px;color:black; font-weight: normal;\">" . $dados['descricao'] . "</span></div>'; }";

	?>

    <script type="text/javascript">
        jQuery(function () {

            var gaugeOptions = {

                chart: {
                    type: 'solidgauge'
                },

                title: null,


                credits: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },

                credits: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },

                pane: {
                    center: ['50%', '85%'],
                    size: '120%',
                    startAngle: -90,
                    endAngle: 90,
                    background: {
                        backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                        innerRadius: '60%',
                        outerRadius: '100%',
                        shape: 'arc'
                    }
                },

                tooltip: {
                    enabled: false
                },

                // the value axis
                yAxis: {
                    stops: [
                        [0, '<?php echo $dados['cor']; ?>']
                    ],
                    lineWidth: 0,
                    minorTickInterval: null,
                    tickPixelInterval: 400,
                    tickWidth: 0,
                    title: {
                        y: -55
                    },
                    labels: {
                        enabled: false
                    },
                    plotBands: [{
                        from: 0,
                        to: parseFloat('<?php echo $meta; ?>'),
                        color: '<?php echo $dados['plotBandsCor'] == '' ? '#726D70' : $dados['plotBandsCor']; ?>',
                        innerRadius: '100%',
                        outerRadius: '<?php echo $dados['plotBandsOuterRadius'] == '' ? '110%' : $dados['plotBandsOuterRadius']; ?>'
                    }]
                },

                plotOptions: {
                    solidgauge: {
                        dataLabels: {
                            y: 5,
                            borderWidth: 0,
                            useHTML: true
                        }
                    }
                }
            };

            // The speed gauge
            jQuery('#<?=$nomeUnico?>').highcharts(Highcharts.merge(gaugeOptions, {
                yAxis: {
                    min: 0,
                    max: parseFloat('<?php echo $metaTotal; ?>'),
                    title: {
                        text: '<span style="color:black;"><?php echo $dados['title'] == '' ? 'Meta Brasil:' : $dados['title']; ?> <?php echo $metaFormatada . $complemento; ?><?php echo $dados['anoprevisto'] != '' ? ' - '.$dados['anoprevisto'] : ''; ?></span>'
                    }
                },

                credits: {
                    enabled: false
                },

                series: [{
                    name: 'Meta',
                    data: [0],
                    dataLabels: {
                        y: 25,
                        formatter: <?php echo $formatter; ?>
                    },
                    tooltip: {
                        valueSuffix: ' '
                    }
                }]

            }));

            // Bring life to the dials
            var chart = $('#<?=$nomeUnico?>').highcharts();
            if (chart) {
                var point = chart.series[0].points[0],
                    newVal = parseFloat('<?php echo $dados['valor']; ?>');
                point.update(newVal);
            }
        });
    </script>
    <div id="<?=$nomeUnico?>" style="width: 200px; height: 150px; float: left"></div>
<?
}
?>