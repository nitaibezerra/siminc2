<?php

include_once APPRAIZ . "includes/funcoes.inc";
//include_once APPRAIZ . "pde/www/_funcoes_cockpit.php";

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
	FROM sase.submetadial s 
	LEFT JOIN sase.meta m ON(s.metid = m.metid) 
	WHERE s.metid = $meta AND s.subiddial = $submeta";
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

            $sql = "select round( pnevalor , 1) as pnevalor, estuf from sase.pnedial where estuf= '".$uf."'  AND subiddial =". $_REQUEST['subiddial'] . " and pnetipo = 'E' order by pneano";

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

            $sql = "select round( pnevalor , 1) as pnevalor, regdescricao from sase.pnedial p
					inner join territorios.regiao r on p.regcod = r.regcod
					where p.regcod= '$reg' AND subiddial =" . $_REQUEST['subiddial'] . " and pnetipo = 'R' order by pneano" ;


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

            $sql = "select round( pnevalor , 1) as pnevalor, mesdsc from sase.pnedial p
			inner join territorios.mesoregiao m on p.mescod = m.mescod
			where p.mescod= '$mes' AND subiddial =" . $_REQUEST['subiddial'] . " and pnetipo = 'S' order by pneano" ;


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

function listarNomeMunicipios($municipios, $subiddial)
{

    global $db;

    foreach ($municipios as $mun)
    {
        $sql = "select mundescricao from sase.pnedial p
		inner join territorios.municipio m on m.muncod = p.muncod
		where p.muncod= ".$mun." AND subiddial =". $subiddial . " and pnetipo = 'M' order by pneano" ;

        $arr = $db->carregar($sql);
        if ($arr)
        {
            echo str_replace("\'","",$arr[0]['mundescricao']);
            echo"; ";
        }
    }
}

function listarDadosMunicipios($municipios, $subiddial)
{

    global $db;
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';

    foreach ($municipios as $mun)
    {
        $sql = "select '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, mundescricao from sase.pnedial p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subiddial =". $subiddial . " and pnetipo = 'M' order by pneano" ;

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

function listarDadosRegioes($regioes, $subiddial)
{
    global $db;
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($regioes as $r)
    {
        $sql = "select pneiddial,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, regdescricao from sase.pnedial p
				inner join territorios.regiao r on r.regcod = p.regcod
				where r.regcod= '$r'  AND subiddial = $subiddial and pnetipo = 'R' order by pneano";
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

function listarNomeRegioes($regioes, $subiddial)
{
    global $db;

    foreach ($regioes as $r)
    {
        $sql = "select regdescricao from sase.pnedial p
		inner join territorios.regiao r on r.regcod = p.regcod
		where r.regcod= '$r'  AND subiddial = $subiddial and pnetipo = 'R' order by pneano";
        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo $arr[0]['regdescricao'];
            echo "; ";
        }

    }
}

function listarNomeMesoregioes($mesoregioes, $subiddial)
{
    global $db;

    foreach ($mesoregioes as $m)
    {
        $sql = "select mesdsc from sase.pnedial p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subiddial = $subiddial and pnetipo = 'S' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo $arr[0]['mesdsc'];
            echo"; ";
        }

    }
}

function listarDadosMesoregioes($mesoregioes, $subiddial)
{
    global $db;

    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($mesoregioes as $m)
    {
        $sql = "select pneiddial,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, mesdsc from sase.pnedial p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subiddial = $subiddial and pnetipo = 'S' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo "<tr><td>".$arr[0]['mesdsc']."</td>";
            foreach($arr as $p)
            {
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

                echo $p['pnevalor'];

                //array_push ( $arrIDEstadosCarregado, $p['pneiddial']);
            }
            echo"</tr>";
        }
    }
}

function listarNomeEstados($estados, $subiddial)
{
    global $db;

    $arrIDEstadosCarregado = array();
    foreach ($estados as $uf)
    {
        $sql = "select estuf from sase.pnedial where estuf= '".$uf."'  AND subiddial =". $subiddial . " and pnetipo = 'E' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo $uf;
            echo"; ";
        }

    }

    return $arrIDEstadosCarregado;
}

function listarDadosEstados($estados, $subiddial)
{
    global $db;

    $arrIDEstadosCarregado = array();
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($estados as $uf)
    {
        $sql = "select pneiddial,   '<td>'||round( pnevalor ,$valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, estuf from sase.pnedial where estuf= '".$uf."'  AND subiddial =". $subiddial . " and pnetipo = 'E' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo "<tr><td>".$uf."</td>";
            foreach($arr as $p)
            {
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

                echo $p['pnevalor'];

                array_push ( $arrIDEstadosCarregado, $p['pneiddial']);
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
    $sql = "select count(*) as qtd from sase.pnedial where subiddial = {$_REQUEST['subiddial']} and pnetipo = '{$pnetipo}'";        
    $rs = $db->carregar($sql);           
    if ($rs[0]['qtd']>0){
        $sql = "update sase.pnedial 
                   set pnepossuiplanoremvigente = " . $_REQUEST['pnepossuiplanoremvigente'] . ",
                       pneplanorefcaput = " . $_REQUEST['pneplanorefcaput'] . ",
                       pneanoprevisto = '" . $_REQUEST['pneanoprevisto'] . "'
                 where subiddial = {$_REQUEST['subiddial']}";                     
    }else{
        $sql = "insert into sase.pnedial (subiddial, estuf, muncod, pnetipo, pnepossuiplanoremvigente, pneplanorefcaput, pneanoprevisto) ".
               "values ({$_REQUEST['subiddial']}, '{$_SESSION['par']['estuf']}', '{$_SESSION['par']['muncod']}', '{$pnetipo}', "
               . "      {$_REQUEST['pnepossuiplanoremvigente']}, {$_REQUEST['pneplanorefcaput']}, {$_REQUEST['pneanoprevisto']})";            
    }

    $db->executar($sql);
    $db->commit();
}

function listagemPrincipal(){  
    global $db;
    $tabela = '';
    $contador=0;
    $arr = carregarMetas($_REQUEST['metid'], $_SESSION['par']['itrid']);
    $titulo = selecionaTitulo($_REQUEST['metid']);
    $valor = ($_REQUEST['metid']==11 || $_REQUEST['metid']==14) ?'0':'1';        
    foreach($arr as $count2 => $s):  
        if ($_SESSION['par']['itrid'] == 1) {
            if ($_SESSION['par']['estuf']) {
                if (!in_array($_REQUEST['metid'], array(15,19,20))) { 
                    $pneanoUf = selecionaAno($s['subiddial'], 'E');
                    $dadosMun = retornaDadosEst($valor, $s['subiddial'], $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneanoUf, $_SESSION['par']['itrid']);
                }else{                    
                    $dadosMun = retornaDadosEst($valor, $s['subiddial'], $_REQUEST['metid'], $_SESSION['par']['estuf'], '', $_SESSION['par']['itrid']);
                }
                $existeDadosMun = (bool)$dadosMun;
            }
        } else {
            if ($_SESSION['par']['muncod']) {
                if (!in_array($_REQUEST['metid'], array(11,12,13,14,15,17,19,20))) {  
                    $pneanoMunicipio=selecionaAno($s['subiddial'], 'M');
                    $dadosMun = retornaDadosMun($valor, $s['subiddial'], $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneanoMunicipio, $_SESSION['par']['itrid']);
                }else{                    
                    $dadosMun = retornaDadosMun($valor, $s['subiddial'], $_REQUEST['metid'], $_SESSION['par']['muncod'], '', $_SESSION['par']['itrid']);                    
                }
                $where = " and pne.muncod = '".$_SESSION['par']['muncod']."'";
                $existeDadosMun = (bool)$dadosMun;
            }
        }
        $subtitulo = $s['subtitulo'] === 'http://ideb.inep.gov.br/resultado/home.seam?cid=4212113'? 'Acesse as metas do IDEB em: <a target="_blank" href="http://ideb.inep.gov.br/resultado/home.seam?cid=4212113">ideb.inep.gov.br</a>': $s['subtitulo'];        
        $qtdLegenda = 1;
        $subnotatecnica = '';
        $file = 'pne/notas_tecnicas/' . $s['subnotatecnica'];
        if($s['subnotatecnica'] && file_exists($file)){
            $subnotatecnica = '<a target="_blank" href="' . $file . '"><img height="30px" src="img/nt.png" alt="Nota Técnica" title="Nota Técnica" /></a>';
        }
        if($contador==0){
            echo "<table border=0 class=\"tabela_box_azul_escuro\" style=\"margin-top: 10px !important;margin-bottom: 10px !important;\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >";        
            echo '    <tr>';
            echo '        <td style=\"font-size:16px;padding:25px 0 5px 5px;font-weight:bold;color: #333;\">';            
            echo '            <div style=\"margin-top:12px;\"><div id=\"titulo-meta-some\" style=\"float:left;\" class=\"titulo_box\" >'.$titulo.' </div>';
            echo '            <div id=\"titulo-meta-aparece\" style=\"display:none;text-align:justify;\">';
	    echo '                <h2 >Meta '.$_REQUEST['metid'].':'.$s['mettitulo'].'</h2>';
            echo '            </div>';
            echo '        </td>';
            echo '    </tr>';            
        } 
        if ((in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
            (in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {            
            echo '    <tr>';
            echo '        <td style=\"font-size:16px;padding:25px 0 5px 5px;font-weight:bold;color: #333;\">';
            if (!$existeDadosMun) {
                echo " <span style='padding:10px;'><input type=\"button\" name=\"btnNaoInfo\" id=\"btnNaoInfo\" onclick=\"naoInformado({$valor}, '{$pneano}', ".$s['subiddial'].", ".$_REQUEST['metid'].", '".$dados[0]['pneiddial']."', 'div_grfMun_" . $s['subiddial'] . "', true)\" value=\"Não Informado\" class=\"btn btn-primary\"/></span>";
            }
            echo '        </td>';
            echo '    </tr>';
        }  
        if ((!in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
            (!in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {
            $pneanoBrasil = selecionaAno($s['subiddial'], 'B');
            $dados = retornaDadosPne($valor, $_REQUEST['metid'], $s['subiddial'], $pneanoBrasil, $_SESSION['par']['itrid'], '');
            $existeResultado = (bool) $dados;
            if ($_SESSION['par']['itrid'] == 1) {
                $pneano = selecionaAno($s['subiddial'], 'E');
                $meta = 'Estado';
            } else {
                $pneano = selecionaAno($s['subiddial'], 'M');
                $meta = 'Município';
            }            
            echo '    <tr>';
            echo "        <td style=\"font-size:16px;padding:5px 0 5px 5px;font-weight:bold;color: #333;\">";
            echo $subnotatecnica." ".$subtitulo;
            echo "        </td>";
            echo "    </tr>";
        }else{
            $dados = retornaDadosPne($valor, $_REQUEST['metid'], $s['subiddial'], '', $_SESSION['par']['itrid'], '');
            $existeResultado = (bool) $dados;
        }
        if($existeResultado){
            if ((!in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
                (!in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {
                echo "    <tr>";
                echo "        <td class=\"tabela_painel\">";
                echo "            <table class=\"tabela_painel\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\">";
                if($existeDadosMun) {          
                    $metaTotal = 100;
                    $anos = array(2015, 2025);
                    echo "    <tr>";
                    echo "        <td align=\"center\" style=\"background-color: #fff;\">&nbsp;";                    
                    echo "        </td>";
                    echo "        <td align=\"center\" style=\"background-color: #fff;\">";
                    echo "            <div style='width: 50%;'>";
                    echo "                <div style='float: left; margin-right: 0px;'>";
                    echo "                    <span style='width: 12px; height: 12px; display: block; background-color: #f7b850; float: left; margin-right: 2px'>";
                    echo "                    </span>Meta {$meta}";
                    echo "                </div>";
                    echo "                <div style='float: left; margin-right: 20px;'>";
                    echo "                    <span style='width: 12px; height: 12px; display: block; background-color: #236B8E; float: left; margin-right: 2px'>";
                    echo "                    </span>Valor Atual";
                    echo "                </div>";
                    echo "            </div>";
                    echo "            <br/>";
                    echo "        </td>";
                    echo "    </tr>";        
                    echo "    <tr>";
                    echo "        <td align=\"center\" style=\"background-color: #fff;\">";
                    echo "            <table class=\"tbNormal\">";
                    echo "                <tr>";
                    echo "                    <td>";
                    echo "                        <div class=\"div_lbl_grfMun\">";
                    echo "                            <label>Meta {$meta}:</label>";
                    echo "                        </div>";
                    echo "                        <div class=\"div_slider_grfMun\">";
                    echo "                            <input type=\"hidden\" id=\"hidID\" value=\"" . $s['subiddial'] . "\"/>";
                    echo "                            <div class=\"slider-range\" id=\"slider_" . $s['subiddial'] . "\" pneiddial=\"" . $dadosMun['pneiddial'] . "\" maxval=\"".$metaTotal."\" tipometa=\"".$dadosMun['subtipometabrasil']."\" sliderval=\"" . round($dadosMun['subvalormetabrasil']) . "\" pneano=\"" . $pneano . "\" valor=\"{$valor}\" subiddial=\"" . $s['subiddial'] . "\">";
                    echo "                            </div>";
                    echo "                        </div>";
                    echo "                        <div class=\"div_text_grfMun\">";
                    echo "                            <input type=\"text\" onchange=\"changeValue(this, {$_SESSION['par']['itrid']})\" subiddial=\"" . $s['subiddial'] . "\" id=\"txtSlider_" . $s['subiddial'] . "\" />";
                    echo "                        </div>";
                    echo "                        <div class=\"div_btn_NaoInfo\">";
                    echo "                            <input type=\"button\" name=\"btnNaoInfo\" id=\"btnNaoInfo\" onclick=\"naoInformado({$valor}, '{$pneano}', ".$s['subiddial'].", ".$dadosMun['pneiddial'].", 'div_grfMun_" . $s['subiddial'] . "')\" value=\"Não Informado\" class=\"btn btn-primary\"/>";
                    echo "                        </div>";
                    echo "                    </td>";      
                     
                    echo "                </tr>";
                    echo "                <tr>";
                    echo "                    <td>";
                    echo "                        <div class=\"div_lbl_grfAno\">";
                    echo "                            <label>Ano Previsto:</label>";
                    echo "                        </div>";
                    echo "                        <div class=\"div_combo_grfAno\">";
                    echo "                            <select id=\"selAnoCorrente_".$s['subiddial']."\" onchange=\"changeValue($('#txtSlider_".$s['subiddial']."'), {$_SESSION['par']['itrid']})\" name=\"selAnoCorrente_".$s['subiddial']."\" class=\"form-control\">";
                                                                $a = $anos[0];
                                                                while($a <= $anos[1]){
                                                                    $sel = $dadosMun['anoprevisto'] == $a ? 'selected' : '';
                                                                    echo "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                                                    $a++;
                                                                }
                    echo "                            </select>";
                    echo "                        </div>";
                    echo "                    </td>";                     
                    echo "                </tr>";
                    echo "            </table>";
                    echo "        </td>";
                    echo "        <td style=\"background-color: #fff;\" colspan=2>";
                    echo "          <div class=\"div_grfMun\" id=\"div_grfMun_" . $s['subiddial'] . "\">";                    
                    if ($dadosMun && is_array($dadosMun)) {
                        $cor = '#236B8E';
                        $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                        $mundescricao = $dadosMun['mundescricao'];
                       
                        $metaBrasil = $metaBrasil;                        
                        $dadosGrafico = array(
                                'cor' => '#f7b850',
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['subvalormetabrasil'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $dadosMun['pnevalor2'],
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => $cor,
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                        );
                        echo geraGraficoPNE('grafico_municipio_' . $s['subiddial'], $dadosGrafico);
                    }                    
                    echo "          </div>";
                    echo "      </td>";                     
                    echo "  </tr>";    
                    echo "  <tr>";
                    echo "      <td align=center  style=\"background-color: #fff;\" colspan=2>";
                    echo "          <div style='width: {$qtdLegenda}0%;'>";
                    echo "              <div style='float: left; margin-right: 20px;'>";
                    echo "                  <span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'>";
                    echo "                  </span>Brasil";
                    echo "              </div>";
                    echo                $legenda;
                    echo "              <div style='clear: both;'>";
                    echo "              </div>";
                    echo "          </div>";
                    echo "          <div style='clear: both;'>";
                    echo "          </div>";
                    echo "      </td>";
                    echo "  </tr>";    
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
                        $dadosBrasil = retornaDadosBrasil($s['subiddial'], $pneanoBrasil);   
                        //ver($dadosBrasil);
                        $descricaoBrasil = substr($descricaoBrasil, 0, strrpos($descricaoBrasil, '::'));
                        $metaTotalBrasil = 100;
                        $metaBrasil = $dadosBrasil[0]['pnevalor'];                        
                        $dadosGrafico = array(
                            'cor' => '#00BF0A',
                            'descricao' => str_replace("'", '', $descricaoBrasil),
                            'valor' => $dadosBrasil[0]['pnevalor'],
                            'metaTotal' => $metaTotalBrasil,
                            'metaBrasil' => $metaTotalBrasil,//$dadosBrasil[0]['pnevalormeta'],
                            'tipo' => 'P'//$dados['subtipometabrasil']
                        );
                        echo "   <tr>";
                        echo "      <td style=\"background-color: #fff;\" colspan=2>";                        
                        echo "          <div style='width: {$qtdLegenda}0%;'>";
                        echo "              <div style='float: left; margin-right: 20px;'>";
                        echo "                  ".geraGraficoPNE('grafico_' . $count . '_' . $count2, $dadosGrafico);
                        echo "              </div>";                        
                        echo "              <div style='clear: both;'>";
                        echo "              </div>";
                        echo "          </div>";
                        echo "          <div style='clear: both;'>";
                        echo "          </div>";   
                        echo "      </td>";
                        echo "  </tr>";                            
                    }                                          
                                                                        
                }
                echo "            </table>";
                echo "        </td>";
                echo "    </tr>";                                    
            }else if ($_REQUEST['metid']==18){
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
                $tabela .= "        <tr>";
                $tabela .= "            <td>";
                $tabela .= "                <table>";
                $tabela .= "                    <tr>";
                $tabela .= "                        <td colspan='2'>";
                $tabela .= "                            Possui um plano de cargos e remuneração vigente?";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr>";
                $tabela .= "                        <td>";               
                $tabela .= "                            <input type='hidden' id='pnepossuiplanoremvigenteH' value='{$checkpnepossuiplanoremvigenteH}'>";
                $tabela .= "                            <input type='radio' id='pnepossuiplanoremvigenteS' name='pnepossuiplanoremvigente' value='true' $checkpnepossuiplanoremvigentetrue onchange='javascript:changeRadio(this, \"trplanovigente\", \"tranoprevisto\", \"pneplanorefcaput\")'>Sim";
                $tabela .= "                        </td>";
                $tabela .= "                        <td>";
                $tabela .= "                            <input type='radio' id='pnepossuiplanoremvigenteN' name='pnepossuiplanoremvigente' value='false' $checkpnepossuiplanoremvigentefalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"trplanovigente\", \"pneplanorefcaput\")'>Não";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr class='trplanovigente' {$habilitaRadio}>";
                $tabela .= "                        <td colspan='2'>";
                $tabela .= "                            Plano de cargos e remuneração, em vigor, toma como referência o caput da meta 18";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr class='trplanovigente' {$habilitaRadio}>";
                $tabela .= "                        <td>";
                $tabela .= "                            <input type='hidden' id='pneplanorefcaputH' value='{$checkpneplanorefcaputH}'>";
                $tabela .= "                            <input type='radio' id='pneplanorefcaputS' name='pneplanorefcaput' value=true $checkpneplanorefcaputtrue onchange='javascript:changeRadio(this, \"\",\"tranoprevisto\", \"\")'>Sim";
                $tabela .= "                        </td>";
                $tabela .= "                        <td>";
                $tabela .= "                            <input type='radio' id='pneplanorefcaputN' name='pneplanorefcaput' value=false $checkpneplanorefcaputfalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"\", \"\")'>Não";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr class='tranoprevisto' {$habilitaAno}>";
                $tabela .= "                        <td colspan='2' id='tdtextoanoprevisto'>";
                $tabela .= "                            <div class=\"div_lbl_grfAno\">";
                $tabela .= "                                <label>Ano Previsto:</label>";
                $tabela .= "                            </div>";
                $tabela .= "                            <div class=\"div_combo_grfAno\">";
                $tabela .= "                                <select id=\"pneanoprevisto\" name=\"pneanoprevisto\">";
                                                                $anos = array(2015, 2025);
                                                                $a = $anos[0];
                                                                while($a <= $anos[1]){
                                                                    $sel = $dados[0]['pneanoprevisto'] == $a ? 'selected' : '';
                                                                    $tabela .= "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                                                    $a++;
                                                                }
                $tabela .= "                                </select>";
                $tabela .= "                            </div>";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                </table>";
                $tabela .= '                <input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarMeta18(' . $s['subiddial'] . ')" value="Salvar Informações Complementares" class="btn btn-primary">';
                $tabela .= "            </td>";
                $tabela .= "        </tr>";                        
            }else{
                if($dados){                    
                    foreach ($dados as $dado){
                        $dadosAgrupados[str_replace('1Brasil', 'Brasil', tirar_acentos($dado['descricao'] . '::' . $dado['pnetipo']))] = $dado;
                        $anos[$dado['pneano']] = $dado['pneano'];                
                        $tabela .= "        <tr>";
                        $tabela .= "            <td>";
                        $tabela .= '                <div align="left" class="metanaoinformada_' . $dado['metid'] . '">';
                        $tabela .= "                    <textarea placeHolder=\"Informações Complementares\" data-pneiddial=\"{$dado['pneiddial']}\" id=\"pneinformcomplementar_{$s['metid']}\" name=\"pneinformcomplementar[]\" rows=\"5\" style=\"width: 96%; margin: 10px;\">{$dado['micinfcomplementar']}</textarea>";
                        $tabela .= '                    <input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarNaoInformado(' . $s['metid'] . ')" value="Salvar Informações Complementares" class="btn btn-primary">';
                        $tabela .= '                </div>';
                        $tabela .= "            </td>";
                        $tabela .= "        </tr>";
                    }
                }
            }
        }else{
            if ((!in_array($_REQUEST['metid'], array(11,12,13,14,15,17,18,19,20))&&($_SESSION['par']['itrid'] != 1))||
                (!in_array($_REQUEST['metid'], array(15,18,19,20))&&($_SESSION['par']['itrid'] == 1))) {
                echo "    <tr>";
                echo "        <td class=\"tabela_painel\">";
                echo "            <table class=\"tabela_painel\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\">";

                $metaTotal = 100;
                $anos = array(2015, 2025);
                echo "    <tr>";
                echo "        <td align=\"center\" style=\"background-color: #fff;\">&nbsp;";                    
                echo "        </td>";                
                echo "        <td align=\"center\" style=\"background-color: #fff;\">";
                echo "            <div style='width: 50%;'>";
                echo "                <div style='float: left; margin-right: 50px;'>";
                echo "                    <span style='width: 12px; height: 12px; display: block; background-color: #f7b850; float: left; margin-right: 2px'>";
                echo "                    </span>Meta {$meta}";
                echo "                </div>";
                echo "                <div style='float: left; margin-right: 20px;'>";
                echo "                    <span style='width: 12px; height: 12px; display: block; background-color: #236B8E; float: left; margin-right: 2px'>";
                echo "                    </span>Valor Atual";
                echo "                </div>";
                echo "            </div>";
                echo "            <br/>";
                echo "        </td>";
                echo "    </tr>";        
                echo "    <tr>";
                echo "        <td align=\"center\" style=\"background-color: #fff;\">";
                echo "            <table class=\"tbNormal\">";
                echo "                <tr>";
                echo "                    <td>";
                echo "                        <div class=\"div_lbl_grfMun\">";
                echo "                            <label>Meta {$meta}:</label>";
                echo "                        </div>";
                echo "                        <div class=\"div_slider_grfMun\">";
                echo "                            <input type=\"hidden\" id=\"hidID\" value=\"" . $s['subiddial'] . "\"/>";
                echo "                            <div class=\"slider-range\" id=\"slider_" . $s['subiddial'] . "\" pneiddial=\"" . $dadosMun['pneiddial'] . "\" maxval=\"".$metaTotal."\" tipometa=\"".$dadosMun['subtipometabrasil']."\" sliderval=\"" . round($dadosMun['subvalormetabrasil']) . "\" pneano=\"" . $pneano . "\" valor=\"{$valor}\" subiddial=\"" . $s['subiddial'] . "\">";
                echo "                            </div>";
                echo "                        </div>";
                echo "                        <div class=\"div_text_grfMun\">";
                echo "                            <input type=\"text\" onchange=\"changeValue(this, {$_SESSION['par']['itrid']})\" subiddial=\"" . $s['subiddial'] . "\" id=\"txtSlider_" . $s['subiddial'] . "\" />";
                echo "                        </div>";
                echo "                        <div class=\"div_btn_NaoInfo\">";
                echo "                            <input type=\"button\" name=\"btnNaoInfo\" id=\"btnNaoInfo\" onclick=\"naoInformado({$valor}, '{$pneano}', ".$s['subiddial'].", ".$dadosMun['pneiddial'].", 'div_grfMun_" . $s['subiddial'] . "')\" value=\"Não Informado\" class=\"btn btn-primary\"/>";
                echo "                        </div>";
                echo "                    </td>";      

                echo "                </tr>";
                echo "                <tr>";
                echo "                    <td>";
                echo "                        <div class=\"div_lbl_grfAno\">";
                echo "                            <label>Ano Previsto:</label>";
                echo "                        </div>";
                echo "                        <div class=\"div_combo_grfAno\">";
                echo "                            <select id=\"selAnoCorrente_".$s['subiddial']."\" onchange=\"changeValue($('#txtSlider_".$s['subiddial']."'), {$_SESSION['par']['itrid']})\" name=\"selAnoCorrente_".$s['subiddial']."\" class=\"form-control\">";
                                                            $a = $anos[0];
                                                            while($a <= $anos[1]){
                                                                $sel = $dadosMun['anoprevisto'] == $a ? 'selected' : '';
                                                                echo "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                                                $a++;
                                                            }
                echo "                            </select>";
                echo "                        </div>";
                echo "                    </td>";                     
                echo "                </tr>";
                echo "            </table>";
                echo "        </td>";
                echo "        <td style=\"background-color: #fff;\" colspan=2>";
                echo "          <div class=\"div_grfMun\" id=\"div_grfMun_" . $s['subiddial'] . "\">";                
                if ($dadosMun && is_array($dadosMun)) {
                    $cor = '#236B8E';
                    $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                    $mundescricao = $dadosMun['nome'];
                    $metaBrasil = $dadosMun['subvalormetabrasil'];
                    $dadosGrafico = array(
                        'cor' => '#f7b850',
                        'descricao' => str_replace("'", '', $descricao),
                        'valor' => $dadosMun['pnevalor2'],
                        'metaTotal' => $metaTotal,
                        'metaBrasil' => $metaBrasil,
                        'tipo' => $dadosMun['subtipometabrasil'],
                        'plotBandsCor' => $cor,
                        'plotBandsOuterRadius' => '115%',
                        'title' => "Meta {$mundescricao}:",
                        'anoprevisto' => $dadosMun['anoprevisto']
                    );
                    echo geraGraficoPNE('grafico_municipio_' . $s['subiddial'], $dadosGrafico);
                }                    
                echo "          </div>";
                echo "      </td>";                     
                echo "  </tr>";    
                echo "  <tr>";
                echo "      <td align=center  style=\"background-color: #fff;\" colspan=2>";                    
                echo "          <div class=\"div_grfMun\" id=\"div_grfMun_" . $s['subiddial'] . "\">";
                echo "          </div>";
                echo "      </td>";
                echo "  </tr>";
                echo "  <tr>";
                echo "      <td align=center  style=\"background-color: #fff;\" colspan=2>";
                echo "          <div style='width: {$qtdLegenda}0%;'>";
                echo "              <div style='float: left; margin-right: 20px;'>";
                echo "                  <span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'>";
                echo "                  </span>Brasil";
                echo "              </div>";
                echo                $legenda;
                echo "              <div style='clear: both;'>";
                echo "              </div>";
                echo "          </div>";
                echo "          <div style='clear: both;'>";
                echo "          </div>";
                echo "      </td>";
                echo "  </tr>";    
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
                    $dados = retornaDadosBrasil($s['subiddial'], $pneanoBrasil);            
                    $descricao = substr($descricao, 0, strrpos($descricao, '::'));
                    $metaTotal = 100;
                    $metaBrasil = $dados[0]['pnevalor'];

                    $dadosGrafico = array(
                        'cor' => '#00BF0A',
                        'descricao' => str_replace("'", '', $descricao),
                        'valor' => $dados[0]['pnevalor'],
                        'metaTotal' => $metaTotal,
                        'metaBrasil' => $metaBrasil,
                        'tipo' => 'P'//$dados['subtipometabrasil']
                    );
                    echo "   <tr>";
                    echo "      <td style=\"background-color: #fff;\" colspan=2>";                        
                    echo "          <div style='width: {$qtdLegenda}0%;'>";
                    echo "              <div style='float: left; margin-right: 20px;'>";
                    echo "                  ".geraGraficoPNE('grafico_' . $count . '_' . $count2, $dadosGrafico);
                    echo "              </div>";                        
                    echo "              <div style='clear: both;'>";
                    echo "              </div>";
                    echo "          </div>";
                    echo "          <div style='clear: both;'>";
                    echo "          </div>";   
                    echo "      </td>";
                    echo "  </tr>";                            
                }                                          
                echo "            </table>";
                echo "        </td>";
                echo "    </tr>";                                    
            }else if ($_REQUEST['metid']==18){
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
                $tabela .= "        <tr>";
                $tabela .= "            <td>";
                $tabela .= "                <table>";
                $tabela .= "                    <tr>";
                $tabela .= "                        <td colspan='2'>";
                $tabela .= "                            Possui um plano de cargos e remuneração vigente?";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr>";
                $tabela .= "                        <td>";               
                $tabela .= "                            <input type='hidden' id='pnepossuiplanoremvigenteH' value='{$checkpnepossuiplanoremvigenteH}'>";
                $tabela .= "                            <input type='radio' id='pnepossuiplanoremvigenteS' name='pnepossuiplanoremvigente' value='true' $checkpnepossuiplanoremvigentetrue onchange='javascript:changeRadio(this, \"trplanovigente\", \"tranoprevisto\", \"pneplanorefcaput\")'>Sim";
                $tabela .= "                        </td>";
                $tabela .= "                        <td>";
                $tabela .= "                            <input type='radio' id='pnepossuiplanoremvigenteN' name='pnepossuiplanoremvigente' value='false' $checkpnepossuiplanoremvigentefalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"trplanovigente\", \"pneplanorefcaput\")'>Não";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr class='trplanovigente' {$habilitaRadio}>";
                $tabela .= "                        <td colspan='2'>";
                $tabela .= "                            Plano de cargos e remuneração, em vigor, toma como referência o caput da meta 18";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr class='trplanovigente' {$habilitaRadio}>";
                $tabela .= "                        <td>";
                $tabela .= "                            <input type='hidden' id='pneplanorefcaputH' value='{$checkpneplanorefcaputH}'>";
                $tabela .= "                            <input type='radio' id='pneplanorefcaputS' name='pneplanorefcaput' value=true $checkpneplanorefcaputtrue onchange='javascript:changeRadio(this, \"\",\"tranoprevisto\", \"\")'>Sim";
                $tabela .= "                        </td>";
                $tabela .= "                        <td>";
                $tabela .= "                            <input type='radio' id='pneplanorefcaputN' name='pneplanorefcaput' value=false $checkpneplanorefcaputfalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"\", \"\")'>Não";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                    <tr class='tranoprevisto' {$habilitaAno}>";
                $tabela .= "                        <td colspan='2' id='tdtextoanoprevisto'>";
                $tabela .= "                            <div class=\"div_lbl_grfAno\">";
                $tabela .= "                                <label>Ano Previsto:</label>";
                $tabela .= "                            </div>";
                $tabela .= "                            <div class=\"div_combo_grfAno\">";
                $tabela .= "                                <select id=\"pneanoprevisto\" name=\"pneanoprevisto\">";
                                                                $anos = array(2015, 2025);
                                                                $a = $anos[0];
                                                                while($a <= $anos[1]){
                                                                    $sel = $dados[0]['pneanoprevisto'] == $a ? 'selected' : '';
                                                                    $tabela .= "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                                                    $a++;
                                                                }
                $tabela .= "                                </select>";
                $tabela .= "                            </div>";
                $tabela .= "                        </td>";
                $tabela .= "                    </tr>";
                $tabela .= "                </table>";
                $tabela .= '                <input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarMeta18(' . $s['subiddial'] . ')" value="Salvar Informações Complementares" class="btn btn-primary">';
                $tabela .= "            </td>";
                $tabela .= "        </tr>";                        
            }else{
                $tabela .= "        <tr>";
                $tabela .= "            <td>";
                $tabela .= '                <div align="left" class="metanaoinformada_' . $dado['metid'] . '">';
                $tabela .= "                    <textarea placeHolder=\"Informações Complementares\" data-pneiddial=\"{$dado['pneiddial']}\" id=\"pneinformcomplementar_{$s['subiddial']}\" name=\"pneinformcomplementar[]\" rows=\"5\" style=\"width: 96%; margin: 10px;\">{$dado['micinfcomplementar']}</textarea>";
                $tabela .= '                    <input type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarNaoInformado(' . $s['subiddial'] . ')" value="Salvar Informações Complementares" class="btn btn-primary">';
                $tabela .= '                </div>';
                $tabela .= "            </td>";
                $tabela .= "        </tr>";                    
            }
        }
        $contador += 1;
    endforeach;
    echo '</table>';
    echo $tabela;
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

    $menu = '<table id="apres" width="100%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
        . '<tr>'
        . '<td width="15%" valign="bottom" class="titulo_box link" onclick="retornaInicio();">Página Inicial</td>'
        . '<td width="85%">'
        . '<table cellpadding="0" cellspacing="0" align="right" border="0">'
        .'<tr>'
        .'<td class="titulo_box titulo-grfpne" colspan="40" align="center">'
        .'Metas PNE - '.$titulo.'<br/><br/>'
        .'</td>'
        .'<td style="padding-right: 10px;">'
        ."<input type=\"button\" name=\"btnValidar\" id=\"btnValidar\" onclick=\"validaIndicadores('Teste')\" value=\"Validar\" class=\"btn btn-primary\"/>"
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
					pegaSelecionados(\'slEstado[]\'),
					pegaSelecionados(\'slMesoregiao[]\'),
                                        '.$_SESSION['par']['itrid'].')">';

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

function naoInformado(){
    global $db;
    $pneiddial       = '';
    $subiddial       = $_REQUEST['subiddial'];
    $metid       = $_REQUEST['metid'];
    $pneano      = $_REQUEST['pneano'];
    $valor       = $_REQUEST['valor'];
    $anoprevisto = $_REQUEST['anoprevisto'];

    if ($_SESSION['par']['itrid'] == 1) {
        $sql = "select pneiddial from sase.pnedial where subiddial = {$subiddial} and estuf = '{$_SESSION['par']['estuf']}' and pneano = {$anoprevisto}";
    } else {
        $sql = "select pneiddial from sase.pnedial where subiddial = {$subiddial} and muncod = '{$_SESSION['par']['muncod']}' and pneano = {$anoprevisto}";
    }    
    $pneiddial = $db->pegaUm($sql);

    if($pneiddial != ''){
        if($db->commit()){
            if ($_SESSION['par']['itrid'] == 1){
                if ($_SESSION['par']['estuf']) {
                    if (in_array($metid, array(15,19,20))){
                        $sql = "update sase.metainfcomplementar set micinfcomplementar = null where estuf = '" . $_SESSION['par']['estuf'] . "' and metid = {$metid}";
                        $db->executar($sql);
                    }
                    $sql = "update sase.pnedial set pnevalormeta = null, pnetipometa = null, pnesemvalor = 't' where estuf = '" . $_SESSION['par']['estuf'] . "' and subiddial = {$subiddial}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $sql = "update sase.pnedial set pnevalormeta = 0, pnesemvalor = 'f' where pneiddial = {$pneiddial}";
                        $db->executar($sql);

                        if($db->commit()) {
                            $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneano, $_SESSION['par']['itrid']);

                            if ($dadosMun && is_array($dadosMun)) {
                                $cor = '#236B8E';
                                $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                                $mundescricao = $dadosMun['estdescricao'];
                                $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                                $metaBrasil = $dadosMun['subvalormetabrasil'];
                                $dadosGrafico = array(
                                    'cor' => '#f7b850',
                                    'descricao' => str_replace("'", '', $descricao),
                                    'valor' => $dadosMun['pnevalor2'],
                                    'metaTotal' => $metaTotal,
                                    'metaBrasil' => $metaBrasil,
                                    'tipo' => $dadosMun['subtipometabrasil'],
                                    'plotBandsCor' => $cor,
                                    'plotBandsOuterRadius' => '115%',
                                    'title' => "Meta {$mundescricao}:",
                                    'anoprevisto' => $anoprevisto
                                );
                                echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
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
                    $sql = "update sase.pnedial set pnevalormeta = null, pnetipometa = null, pnesemvalor = 't' where muncod = '" . $_SESSION['par']['muncod'] . "' and subiddial = {$subiddial}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $sql = "update sase.pnedial set pnevalormeta = 0, pnesemvalor = 'f' where pneiddial = {$pneiddial}";
                        $db->executar($sql);

                        if($db->commit()) {
                            $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneano, $_SESSION['par']['itrid']);

                            if ($dadosMun && is_array($dadosMun)) {
                                $cor = '#236B8E';
                                $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                                $mundescricao = $dadosMun['mundescricao'];
                                $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                                $metaBrasil = $dadosMun['subvalormetabrasil'];
                                $dadosGrafico = array(
                                    'cor' => '#f7b850',
                                    'descricao' => str_replace("'", '', $descricao),
                                    'valor' => $dadosMun['pnevalor2'],
                                    'metaTotal' => $metaTotal,
                                    'metaBrasil' => $metaBrasil,
                                    'tipo' => $dadosMun['subtipometabrasil'],
                                    'plotBandsCor' => $cor,
                                    'plotBandsOuterRadius' => '115%',
                                    'title' => "Meta {$mundescricao}:",
                                    'anoprevisto' => $anoprevisto
                                );
                                echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
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
 * @param type $subiddial
 * @param type $estuf
 * @param type $pneano
 * @return type
 */
function retornaDadosEst($valor, $subiddial, $metid, $estuf, $pneano, $itrid){
    global $db;
    if ((in_array($metid, array(11,12,13,14,15,17,19,20))&&($itrid != 1))||
        (in_array($metid, array(15,19,20))&&($itrid == 1))) {            
        $sql = "
            with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta when sub.subvalormetabrasil is not null then sub.subvalormetabrasil else 0 end as valormeta, case when pne.pnetipometa is not null then  pne.pnetipometa else sub.subtipometabrasil end as tipometa, pneano from sase.submetadial sub left join sase.pnedial pne on sub.subiddial = pne.subiddial and estuf = '" . $_SESSION['par']['estuf'] . "' and pnetipo = 'E' and pnevalormeta is not null and pne.subiddial = sub.subiddial where sub.metid = {$metid} )
            SELECT
                pne.pneiddial,
                m.estuf || ' - ' || mundescricao as descricao,
                estdescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subiddial,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pnedial  pne
                inner join sase.submetadial sub on sub.subiddial = pne.subiddial
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
            WHERE sub.metid = {$metid}
                AND (
                  (pne.estuf = '" . $estuf . "' and pnetipo = 'E')
                )
            ORDER BY ordem, sub.subordem, pne.subiddial, pne.pneano, pnetipo, descricao"; 
    }else{
        $sql = "
            with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta when sub.subvalormetabrasil is not null then sub.subvalormetabrasil else 0 end as valormeta, case when pne.pnetipometa is not null then  pne.pnetipometa else sub.subtipometabrasil end as tipometa, pneano from sase.submetadial sub left join sase.pnedial pne on sub.subiddial = pne.subiddial and estuf = '" . $_SESSION['par']['estuf'] . "' and pnetipo = 'E' and pnevalormeta is not null and pne.subiddial = sub.subiddial where sub.subiddial = {$subiddial} )
            SELECT
                pne.pneiddial,
                m.estuf || ' - ' || mundescricao as descricao,
                estdescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subiddial,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pnedial  pne
                inner join sase.submetadial sub on sub.subiddial = pne.subiddial
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
            WHERE sub.subiddial = {$subiddial}  and pneano = $pneano
                AND (
                  (pne.estuf = '" . $estuf . "' and pnetipo = 'E')
                )
            ORDER BY ordem, sub.subordem, pne.subiddial, pne.pneano, pnetipo, descricao";         
    }
   // ver($sql);
    return $db->pegaLinha($sql);    
}

/**
 * 
 * @global type $db
 * @param type $valor
 * @param type $subiddial
 * @param type $muncod
 * @param type $pneano
 * @return type
 */
function retornaDadosMun($valor, $subiddial, $metid, $muncod, $pneano, $itrid){
    global $db;
    if ((in_array($metid, array(11,12,13,14,15,17,19,20))&&($itrid != 1))||
        (in_array($metid, array(15,19,20))&&($itrid == 1))) {        
    $sql = "
            with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta 
                                       when sub.subvalormetabrasil is not null then sub.subvalormetabrasil 
                                       else 0 
                                   end as valormeta, 
                                  case when pne.pnetipometa is not null then  pne.pnetipometa 
                                       else sub.subtipometabrasil 
                                   end as tipometa, 
                                pneano 
                           from sase.submetadial sub 
                           left join sase.pnedial pne 
                             on sub.subiddial = pne.subiddial 
                            and muncod = '" . $muncod . "' 
                            and pnetipo = 'M' 
                            and pnevalormeta is not null 
                            and pne.subiddial = sub.subiddial 
                          where sub.metid = {$metid} )
            SELECT
                pne.pneiddial,
                m.estuf || ' - ' || mundescricao as descricao,
                mundescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subiddial,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pnedial  pne
                inner join sase.submetadial sub on sub.subiddial = pne.subiddial
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
           where sub.metid = {$metid} ";
    }else{
        
        $sql = "             with tempV as (select case when pne.pnevalormeta is not null then pne.pnevalormeta 
                                                        when sub.subvalormetabrasil is not null then sub.subvalormetabrasil 
                                                        else 0 
                                                    end as valormeta, 
                                                   case when pne.pnetipometa is not null then  pne.pnetipometa 
                                                        else sub.subtipometabrasil 
                                                    end as tipometa, 
                                                   pneano 
                                              from sase.submetadial sub 
                                              left join sase.pnedial pne 
                                                on sub.subiddial = pne.subiddial 
                                               and muncod = '" . $muncod . "' 
                                               and pnetipo = 'M' 
                                               and pnevalormeta is not null 
                                               and pne.subiddial = sub.subiddial 
                                             where sub.subiddial = {$subiddial} )
            SELECT
                pne.pneiddial,
                m.estuf || ' - ' || mundescricao as descricao,
                mundescricao,
                5 as ordem,
                ROUND(pnevalor, $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subiddial,
                sub.metid,
                sub.subtitulo,
                (select tipometa from tempV) as subtipometabrasil,
                (select valormeta from tempV) as subvalormetabrasil,
                (select pneano from tempV) as anoprevisto
            FROM sase.pnedial  pne
                inner join sase.submetadial sub on sub.subiddial = pne.subiddial
                left join territorios.estado e on e.estuf = pne.estuf
                left join territorios.municipio m on m.muncod = pne.muncod
                left join territorios.regiao r on r.regcod = pne.regcod
                left join territorios.mesoregiao mr on mr.mescod = pne.mescod
           WHERE sub.subiddial = {$subiddial}  and pneano = $pneano ";
    }
    $sql .= "  AND ((pne.muncod = '" . $muncod . "' and pnetipo = 'M'))
            ORDER BY ordem, sub.subordem, pne.subiddial, pne.pneano, pnetipo, descricao"; 
    //ver($sql);
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
                from sase.pnedial pne
                inner join sase.submetadial sub on sub.subiddial = pne.subiddial and sub.substatus = 'A'
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
    $subiddial        = $_REQUEST['subiddial'];
    $valor        = $_REQUEST['valor'];

    $pneiddial        = $_POST['pneiddial'];
    $muncod       = $_POST['muncod'];
    $pneano       = $_POST['pneano'];
    $anoprevisto  = $_POST['anoprevisto'];
    $pnetipometa  = $_POST['pnetipometa'];
    $pnevalormeta = str_replace(',', '.', $_POST['pnevalormeta']);
    $tipo         = $_POST['tipo'];    
    $pneanoBrasil = selecionaAno($subiddial, 'B');
    if ($_SESSION['par']['itrid'] == 1){
        if ($_SESSION['par']['estuf']) {
            
            $sql = "update sase.pnedial set pnevalormeta = null, pnetipometa = null where estuf = '".$_SESSION['par']['estuf']."' and subiddial = {$subiddial}";
            $db->executar($sql);
            if($db->commit()) {

                $sql = "select pneiddial from sase.pnedial where subiddial = {$subiddial} and estuf = '{$_SESSION['par']['estuf']}' and pneano = '{$anoprevisto}'";
                $pneiddial = $db->pegaUm($sql);

                if(!empty($pneiddial)) {
                    $sql = "update sase.pnedial set pnetipometa = '{$pnetipometa}', pnevalormeta = {$pnevalormeta} where pneiddial = {$pneiddial}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneano, $_SESSION['par']['itrid']);

                        if ($dadosMun && is_array($dadosMun)) {
                            $dadosBrasil = retornaDadosBrasil($subiddial, $pneanoBrasil);            
                            $descricaoBrasil = substr($descricaoBrasil, 0, strrpos($descricaoBrasil, '::'));
                            $metaTotalBrasil = 100;
                            $metaBrasil = $dadosBrasil[0]['pnevalor'];
                            $cor = '#f7b850';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['estdescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                            //$metaBrasil = $dadosMun['subvalormetabrasil'];
                            $dadosGrafico = array(
                                'cor' => '#236B8E',
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['subvalormetabrasil'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $dadosMun['pnevalor2'],
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => $cor,
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
                        }
                    }
                } else {
                    $sql = "insert into sase.pnedial (subiddial, pneano, estuf, pnetipo, pnetipometa, pnevalormeta, pnesemvalor) values ({$subiddial}, {$anoprevisto}, '{$_SESSION['par']['estuf']}', 'E', '{$pnetipometa}', {$pnevalormeta}, 't') returning pneiddial";
                    $pneiddial = $db->pegaUm($sql);
//                            ver($sql, $pneiddial, d);
                    if($db->commit()){
                        $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $anoprevisto, $_SESSION['par']['itrid']);                        

                        if ($dadosMun && is_array($dadosMun)) {
                            $dadosBrasil = retornaDadosBrasil($subiddial, $pneanoBrasil);            
                            $descricaoBrasil = substr($descricaoBrasil, 0, strrpos($descricaoBrasil, '::'));
                            $metaTotalBrasil = 100;
                            $metaBrasil = $dadosBrasil[0]['pnevalor'];                            
                            $cor = '#236B8E';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['estdescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                            //$metaBrasil = $dadosMun['subvalormetabrasil'];
                            $dadosGrafico = array(
                                'cor' => '#f7b850',
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['subvalormetabrasil'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $dadosMun['pnevalor2'],
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => $cor,
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
                        }
                    }
                }
            }
        }
    } else {
        if ($_SESSION['par']['muncod']) {           
            $sql = "update sase.pnedial set pnevalormeta = null, pnetipometa = null where muncod = '".$_SESSION['par']['muncod']."' and subiddial = {$subiddial}";
            $db->executar($sql);
            if($db->commit()) {

                $sql = "select pneiddial from sase.pnedial where subiddial = {$subiddial} and muncod = '{$_SESSION['par']['muncod']}' and pneano = '{$pneano}'";                
                $pneiddial = $db->pegaUm($sql);

                if (!empty($pneiddial)) {
                    $sql = "update sase.pnedial set pnetipometa = '{$pnetipometa}', pnevalormeta = {$pnevalormeta}, pnesemvalor = 't' where pneiddial = {$pneiddial}";
                    
//                        ver($pneiddial, $sql, d);

                    $db->executar($sql);
                    if ($db->commit()) {
                        $dadosMun = retornaDadosMun($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneano, $_SESSION['par']['itrid']);

                        if ($dadosMun && is_array($dadosMun)) {
                            $cor = '#236B8E';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['mundescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];                                                       
                            $dadosGrafico = array(
                                'cor' => '#f7b850',
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['subvalormetabrasil'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $dadosMun['pnevalor2'],
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => $cor,
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
                        }
                    }
                } else {
                    
                    $sql = "insert into sase.pnedial (subiddial, pneano, muncod, pnetipo, pnetipometa, pnevalormeta, pnesemvalor, pnevalor) "
                            . "values ({$subiddial}, {$pneano}, '{$_SESSION['par']['muncod']}', 'M', '{$pnetipometa}', {$pnevalormeta}, 't', {$valor}) returning pneiddial";
//                    ver($sql,d);
                    $pneiddial = $db->pegaUm($sql);

                    if ($db->commit()) {
                        $dadosMun = retornaDadosMun($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneano, $_SESSION['par']['itrid']);

                        if ($dadosMun && is_array($dadosMun)) {
                            $dadosBrasil = retornaDadosBrasil($subiddial, $pneanoBrasil);            
                            $descricaoBrasil = substr($descricaoBrasil, 0, strrpos($descricaoBrasil, '::'));
                            $metaTotalBrasil = 100;
                            $metaBrasil = $dadosBrasil[0]['pnevalor'];
                            $cor = '#236B8E';
                            $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                            $mundescricao = $dadosMun['mundescricao'];
                            $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                            //$metaBrasil = $dadosMun['subvalormetabrasil'];
                            $dadosGrafico = array(
                                'cor' => '#f7b850',
                                'descricao' => str_replace("'", '', $descricao),
                                'valor' => $dadosMun['subvalormetabrasil'],
                                'metaTotal' => $metaTotal,
                                'metaBrasil' => $dadosMun['pnevalor2'],
                                'tipo' => $dadosMun['subtipometabrasil'],
                                'plotBandsCor' => $cor,
                                'plotBandsOuterRadius' => '115%',
                                'title' => "Meta {$mundescricao}:",
                                'anoprevisto' => $anoprevisto                                
                            );
                            echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
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

function selecionaAno($subiddial, $paetipo){
    global $db;
    $sql = "select paeano
              from sase.pneanoexibicao
             where subid in ({$subiddial})
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
 * @param type $valor
 * @param type $subiddial
 * @param type $pneanoBrasil
 * @param type $where
 * @return type
 */
function retornaDadosPne($valor, $metid, $subiddial, $pneanoBrasil, $itrid, $where){
    if ($_SESSION['par']['itrid']==1){
        $pnetipo = 'E';
    }else{
        $pnetipo = 'M';
    }  
    //$where='';
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
                        pne.pneiddial,
                        pne.subiddial,
                        sub.metid,
                        sub.subtitulo,
                        subtipometabrasil,
                        subvalormetabrasil,
                        pne.pnepossuiplanoremvigente,
                        pne.pneplanorefcaput,
                        pne.pneanoprevisto
                    FROM sase.pnedial  pne
                    inner join sase.submetadial sub on sub.subiddial = pne.subiddial
                    left join territorios.estado e on e.estuf = pne.estuf
                    left join territorios.municipio m on m.muncod = pne.muncod
                    left join territorios.regiao r on r.regcod = pne.regcod
                    left join territorios.mesoregiao mr on mr.mescod = pne.mescod
                WHERE sub.subiddial = {$subiddial}";
                if ($_REQUEST['metid']!=18){
                    $sql .= " AND ((pne.estuf is null and pnetipo = '{$pnetipo}' and pneano = $pneanoBrasil)$where)";
                }else{
                    $sql .= "$where";
                }     
    }
    //ver($sql);
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
                  left join sase.metainfcomplementar mic
                    on meta.metid = mic.metid
                 where meta.metid = ".$metid;
    }else if($metid==18){
        $sql = "select sub.subiddial, 
                       sub.subtitulo,
                       meta.metfontemunicipio,
                       meta.metfonteestado,
                       meta.mettitulo, 
                       sub.subnotatecnica 
                  from sase.submetadial as sub 
                  left join sase.meta meta 
                    on (sub.metid = meta.metid) 
                 where sub.substatus = 'A' 
                   and sub.metid = ".$metid." 
                 order by sub.subordem ASC";        
    }else{
        $sql = "select sub.subiddial, 
                       sub.subtitulo,
                       meta.metfontemunicipio,
                       meta.metfonteestado,
                       meta.mettitulo, 
                       sub.subnotatecnica 
                  from sase.submetadial as sub 
                  left join sase.meta meta 
                    on (sub.metid = meta.metid) 
                 where sub.substatus = 'A' 
                   and sub.metid = ".$metid." 
                 order by sub.subordem ASC";
    }    
    //ver($sql,d);
    return $db->carregar($sql);
}

function retornaDadosBrasil($subiddial, $pneano){
    global $db;
    $sql = "select ROUND(pnevalor, 1) as pnevalor,
                   pnetipo,
                   pnevalormeta
            from sase.pnedial 
           where subiddial= {$subiddial}
             and pneano = {$pneano}
             and pnetipo = 'E'
             and coalesce(estuf, '') = ''"; 
    //ver($sql);    
    return $db->carregar($sql);
}
?>