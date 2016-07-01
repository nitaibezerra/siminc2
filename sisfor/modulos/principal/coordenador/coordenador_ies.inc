<?php 
if( $_REQUEST['requisicao'] == 'exibirListaCustoAtividade'){
	exibirListaCustoAtividade($_REQUEST);
	exit();
}

if($_REQUEST['requisicao']=='excluirCustoAtividade'){
	excluirCustoAtividade($_REQUEST['orcid']);
	exit();
} 

if($_REQUEST['requisicao']=='excluirCursoMEC'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirCursoMEC($_REQUEST['sifid']);
	listarCursosPropostoMEC();
	exit();
}

if($_REQUEST['requisicao']=='excluirCursoCatalogo'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirCursoCatalogo($_REQUEST['cnvid']);
	listarCursosPropostoIES(array(),$_REQUEST['siftipoplanejamento']);
	exit();
} 

if($_REQUEST['requisicao']=='excluirCursoOutros'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirOutrosCursos($_REQUEST['ocuid']);
	listarCursosPropostoIES(array(),$_REQUEST['siftipoplanejamento']);
	exit();
}

if($_REQUEST['requisicao']=='excluirAtividade'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirOutrasAtividades($_REQUEST);
	listarAtividadesPropostaIES($_REQUEST['siftipoplanejamento']);
	exit();
}

if($_REQUEST['requisicao'] == 'removerDocumentoDesignacao'){
	header('Content-Type: text/html; charset=iso-8859-1');
	removerDocumentoDesignacao($_REQUEST['iuaid']);
	listarDocumentoDesignacao($_REQUEST['iusd']);
	exit();
}

if($_REQUEST['requisicao'] == 'anexarDocumentoDesignacao'){
	$_SESSION['REQUEST'] = $_REQUEST;
	anexarDocumentoDesignacao($_REQUEST);
	exit();	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit();
}

if(!$_SESSION['sisfor']['iusd'] || ($_REQUEST['iusd'] && $_REQUEST['iusd'] != $_SESSION['sisfor']['iusd'])){
	$_SESSION['sisfor']['iusd'] = $_REQUEST['iusd'];
}

if($_SESSION['sisfor']['iusd']){
	$unicod = recuperarCodIES($_SESSION['sisfor']['iusd']);
}
	
if($unicod){
	$_SESSION['sisfor']['unicod'] = $unicod;
}

if($_SESSION['sisfor']['unicod']){
	$docid = pegarDocidPlanejamento($_SESSION['sisfor']['unicod']);
	$esdid = pegarEstadoDocumento($docid);
	
	$docidplan2 = pegarDocidPlanejamento2($_SESSION['sisfor']['unicod']);
	$esdidplan2 = pegarEstadoDocumento($docidplan2);
} 

require_once APPRAIZ . "includes/cabecalho.inc";

$perfil = pegarPerfil($_SESSION['usucpf']);

$aba = !$_GET['aba'] ? "principalies" : $_GET['aba'];

$abaAtiva = "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=$aba";

$cadastro = verificarCadastroCoordenador($_SESSION['sisfor']['iusd']);

if(in_array(PFL_SUPER_USUARIO,$perfil) || in_array(PFL_ADMINISTRADOR,$perfil) || in_array(PFL_EQUIPE_MEC,$perfil) || in_array( PFL_COORDENADOR_INST, $perfil) || $cadastro == "S"){
	/*** Array com os itens da aba de identificação ***/
	
	if($esdid == ESD_VALIDADO_MEC){
		$menu = array( 0 => array("id" => 1, "descricao" => "Principal", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=principalies"),
					   1 => array("id" => 2, "descricao" => "Dados Coordenador Institucional", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=dadosies"),
					   2 => array("id" => 3, "descricao" => "Planejamento 2014 - Fase 1", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies"),
					   3 => array("id" => 4, "descricao" => "Planejamento 2014 - Fase 2", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2"),
					   4 => array("id" => 5, "descricao" => "Execução 2014", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=execucao2014"),
					   5 => array("id" => 6, "descricao" => "Programação 2015", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=programacao2015"),
					   6 => array("id" => 7, "descricao" => "Acompanhar bolsas", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=acompanhamentoavaliacaobolsas"));
	} else {
		$menu = array( 0 => array("id" => 1, "descricao" => "Principal", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=principalies"),
					   1 => array("id" => 2, "descricao" => "Dados Coordenador Institucional", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=dadosies"),
					   2 => array("id" => 3, "descricao" => "Planejamento", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies"));
	}
} else {
	/*** Array com os itens da aba de identificação ***/
	$menu = array( 0 => array("id" => 1, "descricao" => "Principal", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=principalies"),
				   1 => array("id" => 2, "descricao" => "Dados Coordenador Institucional", "link" => "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=dadosies"));
}
				   
echo "<br/>"; 
echo montarAbasArray($menu, $abaAtiva);  ?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="js/sisfor.js"></script>

<?php  include_once $aba.".inc"; ?>