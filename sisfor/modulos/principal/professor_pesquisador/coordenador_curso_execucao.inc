<?php

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

if(!$_SESSION['sisfor']['unicod'] || ($_REQUEST['unicod'] && $_REQUEST['unicod'] != $_SESSION['sisfor']['unicod'])){
	$_SESSION['sisfor']['unicod'] = $_REQUEST['unicod'];
}

if(!$_SESSION['sisfor']['curid'] || ($_REQUEST['curid'] && $_REQUEST['curid'] != $_SESSION['sisfor']['curid'])){
	$_SESSION['sisfor']['curid'] = $_REQUEST['curid'];
}

if(!$_SESSION['sisfor']['sifid'] || ($_REQUEST['sifid'] && $_REQUEST['sifid'] != $_SESSION['sisfor']['sifid'])){
	$_SESSION['sisfor']['sifid'] = $_REQUEST['sifid'];
}

if($_SESSION['sisfor']['unicod'] && $_SESSION['sisfor']['curid']) {
	$_SESSION['sisfor']['ieoid'] = $db->pegaUm("SELECT ieoid FROM catalogocurso2014.iesofertante WHERE curid='".$_SESSION['sisfor']['curid']."' AND unicod='".$_SESSION['sisfor']['unicod']."' AND ieostatus='A'");
	if($_SESSION['sisfor']['ieoid']) $_SESSION['sisfor']['sifid'] = $db->pegaUm("SELECT sifid FROM sisfor.sisfor WHERE ieoid='".$_SESSION['sisfor']['ieoid']."' AND unicod='".$_SESSION['sisfor']['unicod']."'");
}

if($_SESSION['sisfor']['sifid']) {

	$_SESSION['sisfor']['unicod'] = $db->pegaUm("SELECT unicod FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");

	if(!$_SESSION['sisfor']['curid']) $_SESSION['sisfor']['curid'] = $db->pegaUm("SELECT curid FROM sisfor.cursonaovinculado c
																				 INNER JOIN sisfor.sisfor s ON s.cnvid = c.cnvid
																				 WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");

	$_SESSION['sisfor']['docidprojeto'] = $db->pegaUm("SELECT docidprojeto FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");

	if(!$_SESSION['sisfor']['docidprojeto']) {
		$_SESSION['sisfor']['docidprojeto'] = wf_cadastrarDocumento(WF_TPDID_PROJETO,"Projeto do curso {$_SESSION['sisfor']['curid']} e universidade {$_SESSION['sisfor']['unicod']}");
		$db->executar("UPDATE sisfor.sisfor SET docidprojeto='".$_SESSION['sisfor']['docidprojeto']."' WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
		$db->commit();
	}

}

require_once APPRAIZ . "includes/cabecalho.inc";

// Aba Principal
$db->cria_aba($abacod_tela, $url, $parametros);

montaCabecalhoCoordenadorCurso();

$perfil = pegarPerfil($_SESSION['usucpf']);

$aba = !$_GET['aba'] ? "principalcurso" : $_GET['aba'];

$abaAtiva = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=$aba";
/*** Array com os itens da aba de identificação ***/
$menu = array(
            0 => array("id" => 1, "descricao" => "Principal", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=principalcurso"),
			1 => array("id" => 2, "descricao" => "Compor Equipe", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=compor_equipe"),
			2 => array("id" => 3, "descricao" => "Avaliar Equipe", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=avaliarusuario"),
			3 => array("id" => 4, "descricao" => "Cursistas", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista"),
                        4 => array("id" => 5, "descricao" => "Gerenciar equipe", "link" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=gerenciarequipe"),
        );

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
<?php  include_once $aba.".inc"; ?>