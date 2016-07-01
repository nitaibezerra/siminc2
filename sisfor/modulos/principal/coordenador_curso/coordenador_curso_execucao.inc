<?php

if(!$_SESSION['sisfor']['sifid']) {
	$al = array("alert" => "Nenhum curso foi selecionado. Tente novamente", "location" => "sisfor.php?modulo=inicio&acao=C");
	alertlocation($al);
}

if($_REQUEST['atualizarFormulario']){
    ob_clean();
    montarFormularioEquipe($_REQUEST['pflcod']);
    die;
}

include_once APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao'] == 'removerDocumentoDesignacao') {
	header('Content-Type: text/html; charset=iso-8859-1');
	removerDocumentoDesignacao($_REQUEST['iuaid']);
	listarDocumentoDesignacao($_REQUEST['iusd']);
	exit();
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit();
}

$sifexecucaosisfor = $db->pegaUm("SELECT sifexecucaosisfor FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");

if($sifexecucaosisfor!='t') {
	$al = array("alert" => "Este curso não será executado pelo SISFOR", "location" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=visualizacao_projeto");
	alertlocation($al);
}

require_once APPRAIZ . "includes/cabecalho.inc";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

montaCabecalhoCoordenadorCurso();

verificarFluxoValidado($_SESSION['sisfor']['docidprojeto']);

$perfil = pegarPerfil($_SESSION['usucpf']);

$aba = !$_GET['aba'] ? "principalcurso" : $_GET['aba'];

$abaAtiva = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=$aba";
/*** Array com os itens da aba de identificação ***/
$menu[] = array("id" => 1, "descricao" => "Principal", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=principalcurso");
$menu[] = array("id" => 2, "descricao" => "Dados Coordenador", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=dadoscurso");
$menu[] = array("id" => 3, "descricao" => "Compor Equipe", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=compor_equipe");
$menu[] = array("id" => 4, "descricao" => "Cursistas", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista");
if($aba=='importar_cursista') $menu[] = array("id" => 5, "descricao" => "Importar cursistas", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=importar_cursista");
$menu[] = array("id" => 6, "descricao" => "Avaliar Equipe", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=avaliarusuario");
$menu[] = array("id" => 7, "descricao" => "Gerenciar equipe", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=gerenciarequipe");
$menu[] = array("id" => 8, "descricao" => "Acompanhar bolsas", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=acompanhamentoavaliacaobolsas");


$ultimomes = $db->pegaUm("SELECT (max((fpbanoreferencia||'-'||fpbmesreferencia||'-15')::date) <= now()) as res FROM sisfor.folhapagamentoprojeto fp 
						  INNER JOIN sisfor.folhapagamento f ON f.fpbid = fp.fpbid 
						  WHERE fp.sifid=".$_SESSION['sisfor']['sifid']);

if($ultimomes=='t') {
	$menu[] = array("id" => 8, "descricao" => "Relatório Final", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=avaliacaofinal");
}

echo montarAbasArray($menu, $abaAtiva);  ?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script language="javascript" type="text/javascript" src="js/sisfor.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	<?php if(in_array(PFL_EQUIPE_MEC,$perfil)){ ?>

	<?php } ?>
</script>
<?php  

if(is_file(APPRAIZ_SISFOR.'coordenador_curso/'.$aba.'.inc')) {
	include $aba.".inc";
} else {
	echo '<p align=center><b>Página não encontrada : '.$aba.'.inc</b></p>';
}

?>