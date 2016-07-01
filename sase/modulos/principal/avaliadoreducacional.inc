<?php

ob_start();

global $db;

// --------------- Dependências
include_once '../../sase/classes/AvaliadorEducacional.class.inc';
include_once '../../sase/classes/ExperienciaAvaliadorEducacional.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//include APPRAIZ . 'includes/funcoes.inc';
//include APPRAIZ . 'www/sase/_funcoes.php';
// --

$avaliadoreducacional = new AvaliadorEducacional();
$experienciaavaliadoreducacional = new ExperienciaAvaliadorEducacional();

$avediretoria = $_GET['avediretoria'] ? $_GET['avediretoria'] : 1;

if (!empty($_GET['aveid'])){
	$avaliadoreducacional->carregarPorId($_GET['aveid']);
	$experienciaavaliadoreducacional->aveid = $_GET['aveid'];
	
	$sqlUfMun = "SELECT estuf FROM territorios.municipio WHERE muncod = '{$avaliadoreducacional->arAtributos['muncod']}'";
	$estuf = $db->carregar($sqlUfMun);
	$estuf = $estuf[0]['estuf'];
	
	$telcom = trim($avaliadoreducacional->arAtributos['avedddtelcomercial']) . trim(str_replace('-','',$avaliadoreducacional->arAtributos['avetelcomercial']));
	$telcel = trim($avaliadoreducacional->arAtributos['avedddtelcelular']) . trim(str_replace('-','',$avaliadoreducacional->arAtributos['avetelcelular']));
	$telres = trim($avaliadoreducacional->arAtributos['avedddtelresidencial']) . trim(str_replace('-','',$avaliadoreducacional->arAtributos['avetelresidencial']));
}

switch ($_REQUEST['requisicao']){
	case 'carrega_municipio':
		echo $avaliadoreducacional->getMunicipios('muncod', $_POST['seluf']);
		exit();
	
	case 'carrega_subarea':
		echo $avaliadoreducacional->getSubAreaCursos($_POST['arcid']);
		exit();
		
	case 'carrega_experiencia':
		
		$experienciaavaliadoreducacional->carregarPorId($_POST['eaeid']);
		
		$t = implode("|", $experienciaavaliadoreducacional->arAtributos);
		
		$t = $_POST['eaeid'] . $t;
		
		if ($experienciaavaliadoreducacional->arAtributos['eaecargoqueaposentou'] == 't'){
			$t .= '|labeleaecargoqueaposentou1|eaecargoqueaposentou1';
		} else {
			$t .= '|labeleaecargoqueaposentou2|eaecargoqueaposentou2';
		}
		
		echo $t;
		exit();
		
	case 'modalResponsabilidade':
		//ver($_REQUEST['usucpf'], $_REQUEST['taeid']);
		$avaliadoreducacional->listaPerfil($_REQUEST['usucpf'], $_REQUEST['taeid']);
		exit();
	
	case 'detalhaResponsabilidade':
		$avaliadoreducacional->listaResponsabilidade($_REQUEST['dados'][0], $_REQUEST['dados'][1]);
		exit();
		
}

include APPRAIZ . 'includes/cabecalho.inc';

// --------------- Decisão da Página
$pagina = "";
switch ($_GET['aba']){
	case 'cadastro':
		$pagina = 'edicaoavaliadoreducacional';
		break;
		
	case 'lista':
		$pagina = 'listaavaliadoreducacional';
		break;
		
	default:
		$pagina = 'listaavaliadoreducacional';
		break;
}
// --


// --------------- Cabeçalho
//$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
//$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";

$dir = "DICOPE";
if($_GET['avediretoria'] == '2'){
    $dir = "DIVAPE";
}

$titulo = 'Avaliador Educacional - '.$dir;


?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<div id='cabecalhoSistema'>
	<center><h3><?=$titulo?></h3></center>
</div>

<div class="row">
<br />
	<div class="col-md-12">
		<ul class="nav nav-tabs">
		  	<li <?=($_GET['aba']=='lista' || $_GET['aba']=='')?'class="active"':''?>><a href="sase.php?modulo=principal/avaliadoreducacional&avediretoria=<?=$avediretoria?>&acao=A&aba=lista">Lista</a></li>
			<li <?=($_GET['aba']=='cadastro')?'class="active"':''?>><a href="sase.php?modulo=principal/avaliadoreducacional&avediretoria=<?=$avediretoria?>&acao=A&aba=cadastro">Cadastro</a></li>
		</ul>
	</div>
</div>
  <?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/avaliadoreducacional/" . $pagina . ".inc";
// --

